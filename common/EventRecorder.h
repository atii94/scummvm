/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef COMMON_EVENTRECORDER_H
#define COMMON_EVENTRECORDER_H

#include "common/scummsys.h"
#include "common/events.h"
#include "common/singleton.h"
#include "common/mutex.h"
#include "common/array.h"
#include "common/memstream.h"
#include "backends/mixer/sdl/sdl-mixer.h"
#include "backends/mixer/nullmixer/nullsdl-mixer.h"
#include "common/hashmap.h"
#include "common/hash-str.h"
#include "engines/advancedDetector.h"

#define g_eventRec (Common::EventRecorder::instance())

namespace Common {

class RandomSource;
class SeekableReadStream;
class WriteStream;

struct RecorderEvent :Common::Event {
	uint32 time;
	uint32 count;
};

/**
 * Our generic event recorder.
 *
 * TODO: Add more documentation.
 */
class EventRecorder : private EventSource, private EventObserver, public Singleton<EventRecorder> {
	friend class Singleton<SingletonBaseType>;
	EventRecorder();
	~EventRecorder();
public:
	void init();
	void deinit();
	/** Register random source so it can be serialized in game test purposes */
	void RegisterEventSource();
	bool processDelayMillis();
	/** TODO: Add documentation, this is only used by the backend */
	void processMillis(uint32 &millis);
	SdlMixerManager *getMixerManager();
	void registerMixerManager(SdlMixerManager *mixerManager);
	uint32 getRandomSeed(const String &name);
	void init(Common::String gameid, const ADGameDescription* desc = NULL);
	uint32 getTimer() {return _fakeTimer;}
private:
	MutexRef _recorderMutex;
	SdlMixerManager* _realMixerManager;
	NullSdlMixerManager* _fakeMixerManager;
	void switchMixer();
	void switchFastMode();
	typedef HashMap<String, uint32, IgnoreCase_Hash, IgnoreCase_EqualTo> randomSeedsDictionary;
	void openRecordFile(Common::String gameId);
	void checkGameHash(const ADGameDescription* desc);
	void writeGameHash(const ADGameDescription* desc);
	bool notifyEvent(const Event &ev);
	String findMd5ByFileName(const ADGameDescription* gameDesc, String fileName);
	Common::String readString();
	bool notifyPoll();
	bool pollEvent(Event &ev);
	bool allowMapping() const { return false; }
	void getNextEvent();
	void readEvent(RecorderEvent &event);
	void writeEvent(const RecorderEvent &event);
	void checkForKeyCode(const Event &event);
	void togglePause();
	class RandomSourceRecord {
	public:
		String name;
		uint32 seed;
	};
	RecorderEvent _nextEvent;
	randomSeedsDictionary _randomSourceRecords;

	volatile uint32 _recordCount;
	volatile uint32 _lastRecordEvent;
	volatile uint32 _lastEventMillis;
	volatile uint32 _delayMillis;
	WriteStream *_recordFile;
	MutexRef _timeMutex;
	volatile uint32 _lastMillis;
	volatile uint32 _fakeTimer;
	volatile uint32 _randomNumber;
	volatile uint32 _playbackDiff;
	volatile bool _hasPlaybackEvent;
	Event _playbackEvent;
	SeekableReadStream *_playbackFile;

	volatile uint32 _eventCount;
	volatile uint32 _lastEventCount;

	enum RecordMode {
		kPassthrough = 0,
		kRecorderRecord = 1,
		kRecorderPlayback = 2,
		kRecorderPlaybackPause = 3
	};
	volatile RecordMode _recordMode;
	String _recordFileName;
	bool _fastPlayback;
};

} // End of namespace Common

#endif
