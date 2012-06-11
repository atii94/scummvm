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

/*
 * This code is based on original Tony Tough source code
 *
 * Copyright (c) 1997-2003 Nayma Software
 */

#ifndef TONY_GFXENGINE_H
#define TONY_GFXENGINE_H

#include "common/scummsys.h"
#include "common/system.h"
#include "common/rect.h"
#include "tony/mpal/memory.h"
#include "tony/game.h"
#include "tony/gfxcore.h"
#include "tony/input.h"
#include "tony/inventory.h"
#include "tony/tonychar.h"
#include "tony/utils.h"

namespace Tony {

class RMGfxEngine {
private:
	RMGfxTargetBuffer _bigBuf;
	RMInput _input;
	RMPointer _point;
	RMLocation _loc;
	RMOptionScreen _opt;
	RMTony _tony;
	RMInventory _inv;
	RMInterface _inter;
	RMTextItemName _itemName;

	bool _bOption;
	bool _bLocationLoaded;

	bool _bInput;
	bool _bAlwaysDrawMouse;

	int _nCurLoc;
	RMTonyAction _curAction;
	int _curActionObj;
	OSystem::MutexRef _csMainLoop;

	int _nWipeType;
	uint32 _hWipeEvent;
	int _nWipeStep;

	bool _bMustEnterMenu;
protected:
	static void itemIrq(uint32 dwItem, int nPattern, int nStatus);
	void initForNewLocation(int nLoc, RMPoint ptTonyStart, RMPoint start);
public:
	bool _bWiping;
	Common::Rect _rcWipeEllipse;
	bool _bGUIOption;
	bool _bGUIInterface;
	bool _bGUIInventory;
public:
	RMGfxEngine();
	virtual ~RMGfxEngine();

	// Draw the next frame
	void doFrame(CORO_PARAM, bool bDrawLocation);

	// Initialises the graphics engine
	void init();

	// Closes the graphics engine
	void close(void);

	// Warns when changing
	void switchFullscreen(bool bFull);

	// Warn that we are guided by the GDI
	void GDIControl(bool bCon);

	// Warns when entering or exits the options menu
	void openOptionScreen(CORO_PARAM, int type);

	// Enables or disables mouse input
	void enableInput(void);
	void disableInput(void);

	// Enables and disables mouse draw
	void enableMouse(void);
	void disableMouse(void);

	operator byte *() {
		return (byte *)_bigBuf;
	}
	RMInput &getInput() {
		return _input;
	}

	// Link to the custom function list
	void initCustomDll(void);

	// Link to graphic task
	void linkGraphicTask(RMGfxTask *task) {
		_bigBuf.addPrim(new RMGfxPrimitive(task));
	};

	// Manage a location
	uint32 loadLocation(int nLoc, RMPoint ptTonyStart, RMPoint start);
	void unloadLocation(CORO_PARAM, bool bDoOnExit, uint32 *result);

	// Freeze and unfreeze
	void freeze(void);
	void unfreeze(void);

	// State management
	void saveState(const Common::String &fn, byte *curThumb, const Common::String &name);
	void loadState(CORO_PARAM, const Common::String &fn);

	// Pauses sound
	void pauseSound(bool bPause);

	// Wipe
	void initWipe(int type);
	void closeWipe(void);
	void waitWipeEnd(CORO_PARAM);

	void setPalesati(bool bpal) {
		_inter.setPalesati(bpal);
	}
	bool canLoadSave();
};

} // End of namespace Tony

#endif