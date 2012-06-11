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

#ifndef TONY_TONYCHAR_H
#define TONY_TONYCHAR_H

#include "common/coroutines.h"
#include "tony/loc.h"

namespace Tony {

class RMTony : public RMCharacter {
private:
	enum DIRECTION {
		UP, DOWN, LEFT, RIGHT
	};

public:
	enum TALKTYPE {
		TALK_NORMAL,
		TALK_FIANCHI,
		TALK_CANTA,
		TALK_RIDE,
		TALK_SIINDICA,
		TALK_SPAVENTATO,
		TALK_SPAVENTATO2,
		TALK_CONBICCHIERE,
		TALK_CONMARTELLO,
		TALK_CONVERME,
		TALK_CONCORDA,
		TALK_CONCONIGLIO,
		TALK_CONRICETTA,
		TALK_CONCARTE,
		TALK_CONPUPAZZO,
		TALK_CONPUPAZZOSTATIC,
		TALK_CONCONIGLIOSTATIC,
		TALK_CONRICETTASTATIC,
		TALK_CONCARTESTATIC,
		TALK_WITH_NOTEBOOK,
		TALK_CONMEGAFONOSTATIC,
		TALK_CONBARBASTATIC,
		TALK_RIDE2,
		TALK_SCHIFATO,
		TALK_NAAH,
		TALK_MACBETH1,
		TALK_MACBETH2,
		TALK_MACBETH3,
		TALK_MACBETH4,
		TALK_MACBETH5,
		TALK_MACBETH6,
		TALK_MACBETH7,
		TALK_MACBETH8,
		TALK_MACBETH9,
		TALK_SPAVENTATOSTATIC,
		TALK_CONSEGRETARIA
	};

private:
	bool _bShow;
	bool _bShowOmbra;
	bool _bCorpoDavanti;
	RMGfxSourceBuffer8AB _ombra;
	bool _bActionPending;
	RMItem *_ActionItem;
	int _Action;
	int _ActionParm;
	static bool _bAction;

	bool _bPastorella;

	bool _bIsStaticTalk;
	bool _bIsTalking;
	int _nPatB4Talking;
	TALKTYPE _nTalkType;
	DIRECTION _TalkDirection;
	RMPoint _nBodyOffset;

	int _nTimeLastStep;

	RMItem _body;
	uint32 hActionThread;

protected:
	// Overload of the allocation allocation of sprites
	virtual RMGfxSourceBuffer *newItemSpriteBuffer(int dimx, int dimy, bool bPreRLE);

	// Watch thread which waits for the end of an action
	static void waitEndOfAction(CORO_PARAM, const void *param);

public:
	enum PATTERNS {
		PAT_TAKEUP_UP1 = 9,
		PAT_TAKEUP_UP2,
		PAT_TAKEUP_MID1,
		PAT_TAKEUP_MID2,
		PAT_TAKEUP_DOWN1,
		PAT_TAKEUP_DOWN2,

		PAT_TAKELEFT_UP1,
		PAT_TAKELEFT_UP2,
		PAT_TAKELEFT_MID1,
		PAT_TAKELEFT_MID2,
		PAT_TAKELEFT_DOWN1,
		PAT_TAKELEFT_DOWN2,

		PAT_TAKERIGHT_UP1,
		PAT_TAKERIGHT_UP2,
		PAT_TAKERIGHT_MID1,
		PAT_TAKERIGHT_MID2,
		PAT_TAKERIGHT_DOWN1,
		PAT_TAKERIGHT_DOWN2,

		PAT_SIRIALZALEFT,
		PAT_PERTERRALEFT,
		PAT_SIRIALZARIGHT,
		PAT_PERTERRARIGHT,

		// Sheperdess!
		PAT_PAST_WALKUP,
		PAT_PAST_WALKDOWN,
		PAT_PAST_WALKLEFT,
		PAT_PAST_WALKRIGHT,

		PAT_PAST_STANDUP,
		PAT_PAST_STANDDOWN,
		PAT_PAST_STANDLEFT,
		PAT_PAST_STANDRIGHT,

		// Speech
		PAT_TALK_UP,
		PAT_TALK_DOWN,
		PAT_TALK_LEFT,
		PAT_TALK_RIGHT,

		// Static head
		PAT_TESTA_UP,
		PAT_TESTA_DOWN,
		PAT_TESTA_LEFT,
		PAT_TESTA_RIGHT,

		// Laugh
		PAT_RIDELEFT_START,
		PAT_RIDELEFT_LOOP,
		PAT_RIDELEFT_END,
		PAT_RIDERIGHT_START,
		PAT_RIDERIGHT_LOOP,
		PAT_RIDERIGHT_END,

		// Speaking as a shepherdess
		PAT_PAST_TALKUP,
		PAT_PAST_TALKDOWN,
		PAT_PAST_TALKLEFT,
		PAT_PAST_TALKRIGHT,

		// Fear
		PAT_SPAVENTOLEFT_START,
		PAT_SPAVENTOLEFT_LOOP,
		PAT_SPAVENTOLEFT_END,
		PAT_SPAVENTORIGHT_START,
		PAT_SPAVENTORIGHT_LOOP,
		PAT_SPAVENTORIGHT_END,
		PAT_SPAVENTODOWN_START,
		PAT_SPAVENTODOWN_LOOP,
		PAT_SPAVENTODOWN_END,

		// With objects: full body
		PAT_CONBICCHIERE,
		PAT_CONCORDA,
		PAT_CONVERME,
		PAT_CONMARTELLO,

		// Sound the whistle
		PAT_FISCHIETTORIGHT,

		// Head with beard
		PAT_TALKBARBA_LEFT,
		PAT_TALKBARBA_RIGHT,

		// Sniff
		PAT_SNIFFA_LEFT,
		PAT_SNIFFA_RIGHT,

		// Disgusted
		PAT_SCHIFATOLEFT_START,
		PAT_SCHIFATOLEFT_LOOP,
		PAT_SCHIFATOLEFT_END,
		PAT_SCHIFATORIGHT_START,
		PAT_SCHIFATORIGHT_LOOP,
		PAT_SCHIFATORIGHT_END,
		PAT_NAAHLEFT_START,
		PAT_NAAHLEFT_LOOP,
		PAT_NAAHLEFT_END,
		PAT_NAAHRIGHT_START,
		PAT_NAAHRIGHT_LOOP,
		PAT_NAAHRIGHT_END,

		// Stand scared
		PAT_SPAVENTOLEFT_STAND,
		PAT_SPAVENTORIGHT_STAND,
		PAT_SPAVENTODOWN_STAND,

		PAT_PUTLEFT_UP1,
		PAT_PUTLEFT_UP2,
		PAT_PUTRIGHT_UP1,
		PAT_PUTRIGHT_UP2,
		PAT_PUTLEFT_MID1,
		PAT_PUTLEFT_MID2,
		PAT_PUTRIGHT_MID1,
		PAT_PUTRIGHT_MID2,
		PAT_PUTLEFT_DOWN1,
		PAT_PUTLEFT_DOWN2,
		PAT_PUTRIGHT_DOWN1,
		PAT_PUTRIGHT_DOWN2,
		PAT_PUTUP_UP1,
		PAT_PUTUP_UP2,
		PAT_PUTUP_MID1,
		PAT_PUTUP_MID2,
		PAT_PUTUP_DOWN1,
		PAT_PUTUP_DOWN2,

		PAT_CONSEGRETARIA
	};

	enum BODYPATTERNS {
		BPAT_STANDUP = 1,
		BPAT_STANDDOWN,
		BPAT_STANDLEFT,
		BPAT_STANDRIGHT,

		BPAT_MARTELLO,
		BPAT_PUPAZZO,
		BPAT_VERME,
		BPAT_BICCHIERE,

		BPAT_CANTALEFT_START,
		BPAT_CANTALEFT_LOOP,
		BPAT_CANTALEFT_END,

		BPAT_FIANCHILEFT_START,
		BPAT_FIANCHILEFT_LOOP,
		BPAT_FIANCHILEFT_END,
		BPAT_FIANCHIRIGHT_START,
		BPAT_FIANCHIRIGHT_LOOP,
		BPAT_FIANCHIRIGHT_END,
		BPAT_FIANCHIUP_START,
		BPAT_FIANCHIUP_LOOP,
		BPAT_FIANCHIUP_END,
		BPAT_FIANCHIDOWN_START,
		BPAT_FIANCHIDOWN_LOOP,
		BPAT_FIANCHIDOWN_END,

		BPAT_RIDELEFT,
		BPAT_RIDERIGHT,

		BPAT_SIINDICALEFT,
		BPAT_SIINDICARIGHT,

		BPAT_SPAVENTODOWN_START,
		BPAT_SPAVENTODOWN_LOOP,
		BPAT_SPAVENTODOWN_END,
		BPAT_SPAVENTOLEFT_START,
		BPAT_SPAVENTOLEFT_LOOP,
		BPAT_SPAVENTOLEFT_END,
		BPAT_SPAVENTORIGHT_START,
		BPAT_SPAVENTORIGHT_LOOP,
		BPAT_SPAVENTORIGHT_END,
		BPAT_SPAVENTOUP_START,
		BPAT_SPAVENTOUP_LOOP,
		BPAT_SPAVENTOUP_END,

		BPAT_CORDA,

		BPAT_CONCONIGLIOLEFT_START,
		BPAT_CONCONIGLIOLEFT_LOOP,
		BPAT_CONCONIGLIOLEFT_END,
		BPAT_CONCONIGLIORIGHT_START,
		BPAT_CONCONIGLIORIGHT_LOOP,
		BPAT_CONCONIGLIORIGHT_END,

		BPAT_CONRICETTALEFT_START,
		BPAT_CONRICETTALEFT_LOOP,
		BPAT_CONRICETTALEFT_END,
		BPAT_CONRICETTARIGHT_START,
		BPAT_CONRICETTARIGHT_LOOP,
		BPAT_CONRICETTARIGHT_END,

		BPAT_CONCARTELEFT_START,
		BPAT_CONCARTELEFT_LOOP,
		BPAT_CONCARTELEFT_END,
		BPAT_CONCARTERIGHT_START,
		BPAT_CONCARTERIGHT_LOOP,
		BPAT_CONCARTERIGHT_END,

		BPAT_CONPUPAZZOLEFT_START,
		BPAT_CONPUPAZZOLEFT_LOOP,
		BPAT_CONPUPAZZOLEFT_END,
		BPAT_CONPUPAZZORIGHT_START,
		BPAT_CONPUPAZZORIGHT_LOOP,
		BPAT_CONPUPAZZORIGHT_END,

		BPAT_CONTACCUINOLEFT_START,
		BPAT_CONTACCUINOLEFT_LOOP,
		BPAT_CONTACCUINOLEFT_END,
		BPAT_CONTACCUINORIGHT_START,
		BPAT_CONTACCUINORIGHT_LOOP,
		BPAT_CONTACCUINORIGHT_END,

		BPAT_CONMEGAFONOLEFT_START,
		BPAT_CONMEGAFONOLEFT_LOOP,
		BPAT_CONMEGAFONOLEFT_END,
		BPAT_CONMEGAFONORIGHT_START,
		BPAT_CONMEGAFONORIGHT_LOOP,
		BPAT_CONMEGAFONORIGHT_END,

		BPAT_CONBARBALEFT_START,
		BPAT_CONBARBALEFT_END,
		BPAT_CONBARBARIGHT_START,
		BPAT_CONBARBARIGHT_END,
		BPAT_CONBARBALEFT_STATIC,
		BPAT_CONBARBARIGHT_STATIC,

		BPAT_MACBETH1,
		BPAT_MACBETH2,
		BPAT_MACBETH3,
		BPAT_MACBETH4,
		BPAT_MACBETH5,
		BPAT_MACBETH6,
		BPAT_MACBETH7,
		BPAT_MACBETH8,
		BPAT_MACBETH9,

		BPAT_CONSEGRETARIA
	};

public:
	static void initStatics();
	RMTony();

	// Initialise Tony
	void init(void);

	// Free all memory
	void close(void);

	// Tony makes a frame, updating the movement, etc.
	void doFrame(CORO_PARAM, RMGfxTargetBuffer *bigBuf, int curLoc);

	// Draw method, which controls chararacter display
	virtual void draw(CORO_PARAM, RMGfxTargetBuffer &bigBuf, RMGfxPrimitive *prim);

	// Show or hide
	void show(void);
	void hide(bool bShowOmbra = false);

	// Move and make an action, if necessary
	void moveAndDoAction(CORO_PARAM, RMPoint dst, RMItem *item, int nAction, int nActionParm = 0);

	// Tony stops (on the right side with respect to any subject)
	virtual void stop(CORO_PARAM);
	void stopNoAction(CORO_PARAM);

	// Set a pattern
	void setPattern(int npatt, bool bPlayP0 = false);

	// Reads the current pattern
	int getCurPattern();

	// Waits until the end of a pattern
	void waitForEndPattern(CORO_PARAM, uint32 hCustomSkip = CORO_INVALID_PID_VALUE) {
		RMCharacter::waitForEndPattern(coroParam, hCustomSkip);
	}

	// Check if currently in an action
	bool inAction() {
		return (_bActionPending && _Action != 0) | _bAction;
	}

	// Check if there needs to be an update for scrolling movement
	bool mustUpdateScrolling() {
		return ((!inAction()) || (isMoving()));
	}

	// Returns Tony's position
	RMPoint position() {
		return _pos;
	}

	// Set the scrolling position
	void setScrollPosition(const RMPoint &pt) {
		RMCharacter::setScrollPosition(pt);
	}

	// Set the take animation
	void take(int nWhere, int nPart);
	void put(int nWhere, int nPart);

	// Start or End Talk
	bool startTalkCalculate(TALKTYPE nTalkType, int &headStartPat, int &bodyStartPat,
	                        int &headLoopPat, int &bodyLoopPat);
	void startTalk(CORO_PARAM, TALKTYPE nTalkType);
	bool endTalkCalculate(int &headStandPat, int &headEndPat, int &bodyEndPat, int &finalPat, bool &bStatic);
	void endTalk(CORO_PARAM);

	// Start or End Static
	void startStaticCalculate(TALKTYPE nTalk, int &headPat, int &headLoopPat,
	                          int &bodyStartPat, int &bodyLoopPat);
	void startStatic(CORO_PARAM, TALKTYPE nTalkType);
	void endStaticCalculate(TALKTYPE nTalk, int &bodyEndPat, int &finalPat, int &headEndPat);
	void endStatic(CORO_PARAM, TALKTYPE nTalkType);

	// Tony disguises himself!
	void setPastorella(bool bIsPast) {
		_bPastorella = bIsPast;
	}
	int getPastorella(void) {
		return _bPastorella;
	}

	// Perform an action
	void executeAction(int nAction, int nActionItem, int nParm);

	void playSfx(int nSfx) {
		RMItem::playSfx(nSfx);
	}
};

} // End of namespace Tony

#endif