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

/*
 * This code is based on the original source code of Lord Avalot d'Argent version 1.3.
 * Copyright (c) 1994-1995 Mike, Mark and Thomas Thurman.
 */

 /* SCROLLS		The scroll driver. */

#include "avalanche/avalanche.h"

#include "avalanche/scrolls2.h"
#include "avalanche/gyro2.h"
#include "avalanche/logger2.h"
#include "avalanche/enhanced2.h"
#include "avalanche/lucerna2.h"
#include "avalanche/trip6.h"
#include "avalanche/acci2.h"
#include "avalanche/visa2.h"
#include "avalanche/timeout2.h"
#include "avalanche/basher2.h"

#include "common/textconsole.h"
#include "common/file.h"

//#include "avalanche/joystick2.h" - Will be implemented later, if it will be implemented at all...

namespace Avalanche {

Scrolls::Scrolls(AvalancheEngine *vm) {
	_vm = vm;
}

void Scrolls::init() {
	loadfont();
	resetscrolldriver();
}

void Scrolls::state(byte x) {     /* Sets "Ready" light to whatever */
	byte page_;

	if (_vm->_gyro->ledstatus == x)
		return; /* Already like that! */

	byte color;
	switch (x) {
	case 0:
		color = black;
		break; /* Off */
	case 1:
	case 2:
	case 3:
		color = green;
		break; /* Hit a key */
	}

	//switch (x) {
	//case 0:
	//	setfillstyle(1, black);
	//	break; /* Off */
	//case 1:
	//	setfillstyle(9, green);
	//	break; /* Half-on (menus) */
	//case 2:
	//	setfillstyle(1, green);
	//	break; /* On (kbd) */
	//case 3:
	//	setfillstyle(6, green);
	//	break; /* Hit a key */
	//}
	warning("STUB: Scrolls::state()");

	_vm->_gyro->super_off();

	_vm->_graphics->drawBar(419, 195, 438, 197, color);

	_vm->_gyro->super_on();
	_vm->_gyro->ledstatus = x;
}

void Scrolls::easteregg() {
	warning("STUB: Scrolls::easteregg()");
}

void Scrolls::say(int16 x, int16 y, Common::String z) { /* Fancy FAST screenwriting */
	warning("STUB: Scrolls::say()");
}

/* Here are the func2edures that Scroll calls */ /* So they must be... */ /*$F+*/

void Scrolls::normscroll() {
	warning("STUB: Scrolls::normscroll()");
}

void Scrolls::dialogue() {
	warning("STUB: Scrolls::dialogue()");
}




void Scrolls::store_(byte what, tunetype &played) {
	memcpy(played+1, played+2, sizeof(played) - 1);
	played[31] = what;
}

bool Scrolls::they_match(tunetype &played) {
	byte fv, mistakes;

	mistakes = 0;

	for (fv = 1; fv <= sizeof(played); fv ++)
		if (played[fv] != _vm->_gyro->tune[fv]) {
			mistakes += 1;
		}

	return mistakes < 5;
}

void Scrolls::music_scroll() {
	warning("STUB: Scrolls::music_scroll()");
}

/* ThatsAll, so put us back to */ /*$F-*/

void Scrolls::resetscrolldriver() {   /* phew */
	_vm->_gyro->scrollbells = 0;
	cfont = roman;
	_vm->_logger->log_epsonroman();
	use_icon = 0;
	_vm->_gyro->interrogation = 0; /* always reset after a scroll comes up. */
}

void Scrolls::dingdongbell() {   /* Pussy's in the well. Who put her in? Little... */
	byte fv;

	for (fv = 1; fv <= _vm->_gyro->scrollbells; fv ++) _vm->_lucerna->errorled(); /* ring the bell "x" times */
}

void Scrolls::dodgem() {     /* This moves the mouse pointer off the scroll so that you can read it. */
	_vm->_gyro->xycheck(); /* Mx & my now contain xy pos of mouse */
	dodgex = _vm->_gyro->mx;
	dodgey = _vm->_gyro->my; /* Store 'em */
	_vm->_gyro->hopto(dodgex, _vm->_gyro->underscroll); /* Move the pointer off the scroll. */
}

void Scrolls::undodgem() {   /* This is the opposite of Dodgem. It moves the
 mouse pointer back, IF you haven't moved it in the meantime. */
	_vm->_gyro->xycheck();
	if ((_vm->_gyro->mx == dodgex) && (_vm->_gyro->my == _vm->_gyro->underscroll))
		/* No change, so restore the pointer's original position. */
		_vm->_gyro->hopto(dodgex, dodgey);
}

void Scrolls::geticon(int16 x, int16 y, byte which) {
	Common::File f;
	byte *p;

	if (!f.open("icons.avd")) {
		warning("AVALANCHE: Scrolls: File not found: icons.avd");
		return;
	}

	which--;
	f.seek(which * 426);

	p = new byte[426];
	f.read(p, 426);

	//putimage(x, y, p, 0);
	warning("STUB: Scrolls::geticon()");

	delete[] p;
	f.close();
}

void Scrolls::block_drop(Common::String fn, int16 xl, int16 yl, int16 y) {
	Common::File f;
	byte bit;
	int16 fv;
	uint16 st;

	st = (y - 1) * 80 + (40 - xl / 2) + ((1 - _vm->_gyro->cp) * _vm->_gyro->pagetop);

	Common::String filename;
	filename = filename.format("%s.avd", fn.c_str());
	if (!f.open(filename)) {
		warning("AVALANCHE: Scrolls: File not found: %s", filename.c_str());
		return;
	}

	/*for (fv = 1; fv <= yl; fv ++)
	for (bit = 0; bit <= 3; bit ++) {
	port[0x3c4] = 2;
	port[0x3ce] = 4;
	port[0x3c5] = 1 << bit;
	port[0x3cf] = bit;
	blockread(f, mem[0xa000 * st + (fv * 80)], xl);
	}
	bit = getpixel(0, 0);*/
	warning("STUB: Scrolls::block_drop()");

	f.close();
}

void Scrolls::drawscroll(func2 gotoit) {     /* This is one of the oldest func2s in the game. */
	warning("STUB: Scrolls::drawscroll()");
}

void Scrolls::bubble(func2 gotoit) {
	warning("STUB: Scrolls::bubble()");
}

bool Scrolls::ask(Common::String question) {
	warning("STUB: Scrolls::ask()");
	return true;
}

void Scrolls::resetscroll() {
	_vm->_gyro->scrolln = 1;
	for (int j = 0; j < 15; j ++)
		for (int i = 0; i < _vm->_gyro->scroll[j].size(); i++)
			_vm->_gyro->scroll[j].setChar(0, i);
}

void Scrolls::natural() {   /* Natural state of bubbles */
	_vm->_gyro->talkx = 320;
	_vm->_gyro->talky = 200;
	_vm->_gyro->talkb = 8;
	_vm->_gyro->talkf = 15;
}

Common::String Scrolls::lsd() {
	Common::String x;

	Common::String lsd_result;
	if (_vm->_gyro->dna.pence < 12) {
		/* just pence */
		x = _vm->_gyro->strf(_vm->_gyro->dna.pence) + 'd';
	} else if (_vm->_gyro->dna.pence < 240) {
		/* shillings & pence */
		x = _vm->_gyro->strf(_vm->_gyro->dna.pence / int32(12)) + '/';
		if ((_vm->_gyro->dna.pence % int32(12)) == 0)  x = x + '-';
		else x = x + _vm->_gyro->strf(_vm->_gyro->dna.pence % int32(12));
	} else   /* L, s & d */
		x = Common::String('�') + _vm->_gyro->strf(_vm->_gyro->dna.pence / int32(240)) + '.' + _vm->_gyro->strf((_vm->_gyro->dna.pence / int32(12)) % int32(20))
			+ '.' + _vm->_gyro->strf(_vm->_gyro->dna.pence % int32(12));
	if (_vm->_gyro->dna.pence > 12)  x = x + " (that's " + _vm->_gyro->strf(_vm->_gyro->dna.pence) + "d)";
	lsd_result = x;
	return lsd_result;
}




void Scrolls::strip(Common::String &q) { // Strip trailing spaces.
	while (q[q.size() - 1] == ' ') {
		q.deleteLastChar();
	}
}

void Scrolls::solidify(byte n) {
	if (!_vm->_gyro->scroll[n].contains(' '))
		return; // No spaces.

	// So there MUST be a space there, somewhere...
	do {
		_vm->_gyro->scroll[n + 1] = _vm->_gyro->scroll[n][_vm->_gyro->scroll[n].size() - 1] + _vm->_gyro->scroll[n + 1];
		_vm->_gyro->scroll[n].deleteLastChar();
	} while (!_vm->_gyro->scroll[n][_vm->_gyro->scroll[n].size() - 1] != ' ');

	strip(_vm->_gyro->scroll[n]);
}

void Scrolls::calldrivers() {
	uint16 fv;
	byte nn;
	char nnn;
	bool mouthnext;
	bool call_spriterun; // Only call sprite_run the FIRST time.
	bool was_virtual; // Was the mouse cursor virtual on entry to this proc?


	//nosound();
	warning("STUB: Scrolls::calldrivers()");

	state(0);
	_vm->_gyro->screturn = false;
	mouthnext = false;
	call_spriterun = true;

	switch (_vm->_gyro->buffer[_vm->_gyro->bufsize - 1]) {
	case kControlToBuffer:
		_vm->_gyro->bufsize--;
		break; // ^D = (D)on't include pagebreak
	case kControlSpeechBubble:
	case kControlQuestion:
		break; // ^B = speech (B)ubble, ^Q = (Q)uestion in dialogue box
	default: {
		_vm->_gyro->bufsize++;
		_vm->_gyro->buffer[_vm->_gyro->bufsize - 1] = kControlParagraph;
		}
	}

	for (fv = 0; fv <= _vm->_gyro->bufsize; fv++)
		if (mouthnext) {
			if (_vm->_gyro->buffer[fv] == kControlRegister)
				param = 0;
			else
				if (('0' < _vm->_gyro->buffer[fv]) && (_vm->_gyro->buffer[fv] < '9'))
					param = _vm->_gyro->buffer[fv] - 48;
				else if (('A' < _vm->_gyro->buffer[fv]) && (_vm->_gyro->buffer[fv] < 'Z'))
					param = _vm->_gyro->buffer[fv] - 55;

			mouthnext = false;
		} else
			switch (_vm->_gyro->buffer[fv]) {
			case kControlParagraph: {
				if ((_vm->_gyro->scrolln == 0) && (_vm->_gyro->scroll[0].empty()))
					break;

				if (call_spriterun)
					_vm->_lucerna->sprite_run();
				call_spriterun = false;

				was_virtual = _vm->_gyro->visible == _vm->_gyro->m_virtual;
				if (was_virtual)
					_vm->_gyro->off_virtual();

				drawscroll(&Avalanche::Scrolls::normscroll);

				if (was_virtual)
					_vm->_gyro->on_virtual();

				resetscroll();

				if (_vm->_gyro->screturn)
					return;
			}
			break;
			case kControlBell:
				_vm->_gyro->scrollbells++;
				break; // #7 = "Bel"
			case kControlSpeechBubble: {
				if ((_vm->_gyro->scrolln == 0) && (_vm->_gyro->scroll[0].empty()))
					break;

				if (call_spriterun)
					_vm->_lucerna->sprite_run();
				call_spriterun = false;

				if (param == 0)
					natural();
				else if ((1 <= param) && (param <= 9))
					if ((param > _vm->_trip->numtr) || (!_vm->_trip->tr[param].quick)) { // Not valid.
						_vm->_lucerna->errorled();
						natural();
					} else
						_vm->_trip->tr[param].chatter(); // Normal sprite talking routine.
				else if ((10 <= param) && (param <= 36)) {
					/* Quasi-peds. (This routine performs the same
					thing with QPs as triptype.chatter does with the
					sprites.) */
					_vm->_gyro->talkx = _vm->_gyro->peds[_vm->_gyro->quasipeds[param].whichped].x;
					_vm->_gyro->talky = _vm->_gyro->peds[_vm->_gyro->quasipeds[param].whichped].y; // Position.
		
					_vm->_gyro->talkf = _vm->_gyro->quasipeds[param].fgc;
					_vm->_gyro->talkb = _vm->_gyro->quasipeds[param].bgc; // Colours.
				} else {
					_vm->_lucerna->errorled(); // Not valid.
					natural();
				}

				was_virtual = _vm->_gyro->visible == _vm->_gyro->m_virtual;
				if (was_virtual)
					_vm->_gyro->off_virtual();

				bubble(&Avalanche::Scrolls::normscroll);

				if (was_virtual)
					_vm->_gyro->on_virtual();

				resetscroll();

				if (_vm->_gyro->screturn)
					return;
			}
			break;
			case kControlNegative: {
				switch (param) {
				case 1:
					display(lsd() + kControlToBuffer);
					break; /* insert cash balance (recursion) */
				case 2:
					display(_vm->_acci->words[_vm->_acci->first_password + _vm->_gyro->dna.pass_num].w + kControlToBuffer);
					break;
				case 3:
					display(_vm->_gyro->dna.like2drink + kControlToBuffer);
					break;
				case 4:
					display(_vm->_gyro->dna.favourite_song + kControlToBuffer);
					break;
				case 5:
					display(_vm->_gyro->dna.worst_place_on_earth + kControlToBuffer);
					break;
				case 6:
					display(_vm->_gyro->dna.spare_evening + kControlToBuffer);
					break;
				case 9:
					display(_vm->_gyro->strf(_vm->_gyro->dna.cat_x) + ',' + _vm->_gyro->strf(_vm->_gyro->dna.cat_y) + kControlToBuffer);
					break;
				case 10:
					switch (_vm->_gyro->dna.box_contents) {
					case 0: { // Sixpence.
						_vm->_visa->dixi('q', 37); // You find the sixpence.
						_vm->_gyro->dna.pence += 6;
						_vm->_gyro->dna.box_contents = _vm->_acci->nowt;
						_vm->_lucerna->points(2);
						return;
					}
					break;
					case _vm->_acci->nowt:
						display("nothing at all. It's completely empty.");
						break;
					default:
						display(_vm->_gyro->get_better(_vm->_gyro->dna.box_contents) + '.');
					}
					break;
				case 11: {
					nn = 1;
					for (nnn = 0; nnn < numobjs; nnn++)
						if (_vm->_gyro->dna.obj[nnn]) {
							nn ++;
							display(_vm->_gyro->get_better(nnn) + ", " + kControlToBuffer);
						}
					}
					break;
				}
				
				}
				break;
			case kControlIcon:
				use_icon = param;
				break;
			case kControlNewLine:
				_vm->_gyro->scrolln++;
				break;
			case kControlQuestion: {
				if (call_spriterun)
					_vm->_lucerna->sprite_run();
				call_spriterun = false;

				_vm->_gyro->scrolln++;
				_vm->_gyro->scroll[_vm->_gyro->scrolln] = kControlQuestion;

				was_virtual = _vm->_gyro->visible == _vm->_gyro->m_virtual;
				if (was_virtual)
					_vm->_gyro->off_virtual();

				drawscroll(&Avalanche::Scrolls::dialogue);

				if (was_virtual)
					_vm->_gyro->on_virtual();

				resetscroll();
				}
				break;
			case kControlRegister:
				mouthnext = true;
				break;
			case kControlInsertSpaces:
				for (nn = 0; nn < 9; nn++) _vm->_gyro->scroll[_vm->_gyro->scrolln] = _vm->_gyro->scroll[_vm->_gyro->scrolln] + ' ';
				break;
			default: { // Add new char.
				if (_vm->_gyro->scroll[_vm->_gyro->scrolln].size() == 50) {
					solidify(_vm->_gyro->scrolln);
					_vm->_gyro->scrolln++;
				}
				_vm->_gyro->scroll[_vm->_gyro->scrolln] = _vm->_gyro->scroll[_vm->_gyro->scrolln] + _vm->_gyro->buffer[fv];
				}
			}
}

void Scrolls::display(Common::String z) {
	_vm->_gyro->bufsize = z.size();
	memcpy(_vm->_gyro->buffer, z.c_str(), _vm->_gyro->bufsize);
	calldrivers();
}

void Scrolls::loadfont() {
	Common::File f;

	if (!f.open("avalot.fnt")) {
		warning("AVALANCHE: Scrolls: File not found: avalot.fnt");
		return;
	}
	for (int16 i = 0; i < 256; i++)
		f.read(ch[0][i],16);
	f.close();

	if (!f.open("avitalic.fnt")) {
		warning("AVALANCHE: Scrolls: File not found: avitalic.fnt");
		return;
	}
	for (int16 i = 0; i < 256; i++)
		f.read(ch[1][i],16);
	f.close();
	
	if (!f.open("ttsmall.fnt")) {
		warning("AVALANCHE: Scrolls: File not found: ttsmall.fnt");
		return;
	}
	for (int16 i = 0; i < 256; i++)
		f.read(_vm->_gyro->characters[i],16);
	f.close();
}

void Scrolls::okay() {
	display("Okay!");
}

void Scrolls::musical_scroll() {
	bool was_virtual;

	display(Common::String("To play the harp...\r\rUse these keys:\r\n") +
	        "Q W E R T Y U I O P [ ]\r\rOr press Enter to stop playing.\4");

	_vm->_lucerna->sprite_run();

	was_virtual = _vm->_gyro->visible == _vm->_gyro->m_virtual;

	if (was_virtual)
		_vm->_gyro->off_virtual();

	drawscroll(&Avalanche::Scrolls::music_scroll);

	if (was_virtual)
		_vm->_gyro->on_virtual();
	resetscroll();
}


} // End of namespace Avalanche
