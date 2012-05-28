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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/coroutines.h"
#include "tony/debugger.h"
#include "tony/globals.h"
#include "tony/tony.h"

namespace Tony {

Debugger::Debugger() : GUI::Debugger() {
	DCmd_Register("continue",		WRAP_METHOD(Debugger, Cmd_Exit));
	DCmd_Register("scene",			WRAP_METHOD(Debugger, Cmd_Scene));
}

static int strToInt(const char *s) {
	if (!*s)
		// No string at all
		return 0;
	else if (toupper(s[strlen(s) - 1]) != 'H')
		// Standard decimal string
		return atoi(s);

	// Hexadecimal string
	uint tmp = 0;
	int read = sscanf(s, "%xh", &tmp);
	if (read < 1)
		error("strToInt failed on string \"%s\"", s);
	return (int)tmp;
}

/**
 * Support process for changing the scene
 */
struct ChangeSceneDetails {
	int sceneNumber;
	int x;
	int y;
};

void DebugChangeScene(CORO_PARAM, const void *param) {
	CORO_BEGIN_CONTEXT;
	CORO_END_CONTEXT(_ctx);

	uint32 result;
	const ChangeSceneDetails *details = (const ChangeSceneDetails *)param;
	RMPoint scenePos(details->x, details->y);

	CORO_BEGIN_CODE(_ctx);

	CORO_INVOKE_2(GLOBALS.UnloadLocation, false, &result);

	GLOBALS.LoadLocation(details->sceneNumber, scenePos, RMPoint(-1, -1));

	MainEnableGUI();

	CORO_END_CODE;
}


/**
 * This command loads up the specified new scene number
 */
bool Debugger::Cmd_Scene(int argc, const char **argv) {
	if (argc < 2) {
		DebugPrintf("Usage: %s <scene number> [<x> <y>]\n", argv[0]);
		return true;
	}

	int sceneNumber = strToInt(argv[1]);
	if (sceneNumber >= _vm->_theBoxes.GetLocBoxesCount()) {
		DebugPrintf("Invalid scene\n");
		return true;
	}

	RMPoint scenePos;
	if (argc >= 4) {
		scenePos.x = strToInt(argv[2]);
		scenePos.y = strToInt(argv[3]);
	} else {
		// Get the box areas for the scene, and choose one so as to have a default
		// position for Tony that will be in the walkable areas
		RMBoxLoc *box = _vm->_theBoxes.GetBoxes(sceneNumber);
		scenePos.Set(box->boxes[0].hotspot[0].hotx, box->boxes[0].hotspot[0].hoty);
	}

	// Set up a process to change the scene
	ChangeSceneDetails details;
	details.sceneNumber = sceneNumber;
	details.x = scenePos.x;
	details.y = scenePos.y;
	CoroScheduler.createProcess(DebugChangeScene, &details, sizeof(ChangeSceneDetails));

	return false;
}

} // End of namespace Tony