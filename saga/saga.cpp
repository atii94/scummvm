/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 The ScummVM project
 *
 * The ReInherit Engine is (C)2000-2003 by Daniel Balsom.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */
#include "stdafx.h"

#include "base/gameDetector.h"
#include "base/plugins.h"
#include "backends/fs/fs.h"

#include "sound/mixer.h"
#include "common/file.h"
#include "common/config-manager.h"

#include "saga/saga.h"

#include "saga/gfx.h"
#include "saga/rscfile_mod.h"
#include "saga/render.h"
#include "saga/actor.h"
#include "saga/animation.h"
#include "saga/console.h"
#include "saga/events.h"
#include "saga/font.h"
#include "saga/game.h"
#include "saga/interface.h"
#include "saga/isomap.h"
#include "saga/script.h"
#include "saga/scene.h"
#include "saga/sdata.h"
#include "saga/sndres.h"
#include "saga/sprite.h"
#include "saga/sound.h"
#include "saga/music.h"
#include "saga/palanim.h"

static const GameSettings saga_games[] = {
	{"ite", "Inherit the Earth", 0},
	{"ite-demo", "Inherit the Earth (Demo)",  0},
	{"ihnm", "I Have No Mouth and I Must Scream", GF_DEFAULT_TO_1X_SCALER },
	{"ihnm-demo", "I Have No Mouth and I Must Scream (Demo)", GF_DEFAULT_TO_1X_SCALER },
	{0, 0, 0}
};

GameList Engine_SAGA_gameList() {
	GameList games;
	const GameSettings *g = saga_games;

	while (g->name) {
		games.push_back(*g);
		g++;
	}

	return games;
}

DetectedGameList Engine_SAGA_detectGames(const FSList &fslist) {
	return Saga::GAME_ProbeGame(fslist);
}

Engine *Engine_SAGA_create(GameDetector *detector, OSystem *syst) {
	return new Saga::SagaEngine(detector, syst);
}

REGISTER_PLUGIN("SAGA Engine", Engine_SAGA_gameList, Engine_SAGA_create, Engine_SAGA_detectGames)

namespace Saga {

#define MAX_TIME_DELTA 100

SagaEngine *_vm = NULL;

SagaEngine::SagaEngine(GameDetector *detector, OSystem *syst)
	: Engine(syst) {

	_console = NULL;

	// The Linux version of Inherit the Earth puts all data files in an
	// 'itedata' sub-directory, except for voices.rsc
	File::addDefaultDirectory(_gameDataPath + "itedata/");

	// The Windows version of Inherit the Earth puts various data files in
	// other subdirectories.
	File::addDefaultDirectory(_gameDataPath + "graphics/");
	File::addDefaultDirectory(_gameDataPath + "music/");
	File::addDefaultDirectory(_gameDataPath + "sound/");

	// Setup mixer
	if (!_mixer->isReady()) {
		warning("Sound initialization failed.");
	}

	_mixer->setVolume(ConfMan.getInt("sfx_volume"));
	_mixer->setMusicVolume(ConfMan.getInt("music_volume"));

	_vm = this;
}

SagaEngine::~SagaEngine() {
}

void SagaEngine::errorString(const char *buf1, char *buf2) {
	strcpy(buf2, buf1);
}

int SagaEngine::init(GameDetector &detector) {
	_soundEnabled = 1;
	_musicEnabled = 1;

	// Add some default directories
	// Win32 demo & full game
	File::addDefaultDirectory("graphics");
	File::addDefaultDirectory("music");
	File::addDefaultDirectory("sound");

	// Linux demo
	File::addDefaultDirectory("itedata");

	// Mac demos & full game
	File::addDefaultDirectory("patch");

	// Process command line

	// Detect game and open resource files
	if (initGame() != SUCCESS) {
		return -1;
	}

	// Initialize engine modules
	_sndRes = new SndRes(this);
	_events = new Events(this);
	_font = new Font(this);
	_sprite = new Sprite(this);
	_anim = new Anim(this);
	_script = new Script();
	_sdata = new SData();
	_interface = new Interface(this); // requires script module
	_actor = new Actor(this);
	_palanim = new PalAnim(this);
	_scene = new Scene(this);

	if (!_scene->initialized()) {
		warning("Couldn't initialize scene module");
		// TODO/FIXME: We are leaking here
		return -1;
	}

	// System initialization

	_previousTicks = _system->getMillis();

	// Initialize graphics
	GAME_DISPLAYINFO disp_info;
	getDisplayInfo(&disp_info);
	_gfx = new Gfx(_system, disp_info.logical_w, disp_info.logical_h, detector);

	// Graphics driver should be initialized before console
	_console = new Console(this);

	// Graphics should be initialized before music
	int midiDriver = MidiDriver::detectMusicDriver(MDT_NATIVE | MDT_ADLIB | MDT_PREFER_NATIVE);
	bool native_mt32 = (ConfMan.getBool("native_mt32") || (midiDriver == MD_MT32));

	bool adlib = false;

	MidiDriver *driver = MidiDriver::createMidi(midiDriver);
	if (!driver) {
		driver = MidiDriver_ADLIB_create(_mixer);
		adlib = true;
	} else if (native_mt32)
		driver->property(MidiDriver::PROP_CHANNEL_MASK, 0x03FE);

	_music = new Music(_mixer, driver, _musicEnabled);
	_music->setNativeMT32(native_mt32);
	_music->setAdlib(adlib);

	if (!_musicEnabled) {
		debug(0, "Music disabled.");
	}

	_isoMap = new IsoMap(_gfx);
	
	_render = new Render(this, _system);
	if (!_render->initialized()) {
		// TODO/FIXME: We are leaking here
		return -1;
	}

	// Initialize system specific sound
	_sound = new Sound(this, _mixer, _soundEnabled);
	if (!_soundEnabled) {
		debug(0, "Sound disabled.");
	}

	return 0;
}

int SagaEngine::go() {
	int msec = 0;

	_previousTicks = _system->getMillis();

	_sprite->loadList(ITE_MAIN_SPRITES, &_mainSprites);

	// Begin Main Engine Loop

	_scene->startScene();
	uint32 currentTicks;

	for (;;) {
		if (_console->isAttached())
			_console->onFrame();

		if (_render->getFlags() & RF_RENDERPAUSE) {
			// Freeze time while paused
			_previousTicks = _system->getMillis();
		} else {
			currentTicks = _system->getMillis();
			// Timer has rolled over after 49 days
			if (currentTicks < _previousTicks)
				msec = 0;
			else {
				msec = currentTicks - _previousTicks;
				_previousTicks = currentTicks;
			}
			if (msec > MAX_TIME_DELTA) {
				msec = MAX_TIME_DELTA;
			}
			_actor->direct(msec);
			_events->handleEvents(msec);
			_script->executeThreads(msec);
		}
		// Per frame processing
		_render->drawScene();
		_system->delayMillis(10);
	}
	
	return 0;
}

void SagaEngine::shutdown() {
	delete _scene;
	delete _actor;
	delete _script;
	delete _sprite;
	delete _font;
	delete _console;
	delete _events;
	delete _palanim;

	delete _interface;
	delete _render;
	delete _isoMap;
	delete _sndRes;
	delete _sdata;
	// Shutdown system modules */
	delete _music;
	delete _sound;
	delete _anim;

	_system->quit();
}

} // End of namespace Saga
