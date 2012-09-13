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

#ifndef HOPKINS_GRAPHICS_H
#define HOPKINS_GRAPHICS_H

#include "common/scummsys.h"
#include "common/endian.h"
#include "common/str.h"
#include "graphics/surface.h"

namespace Hopkins {

#define PALETTE_SIZE 256
#define PALETTE_BLOCK_SIZE (PALETTE_SIZE * 3)

struct RGB8 {
	byte r;
	byte g;
	byte b;
};

class GraphicsManager {
private:
	void CHARGE_ECRAN(const Common::String &file);
public:
	int _lockCtr;
	bool SDL_MODEYES;
	int SDL_ECHELLE;
	int XSCREEN;
	int YSCREEN;
	int WinScan;
	int Winbpp;
	byte SD_PIXELS[PALETTE_SIZE * 2];
	byte *PAL_PIXELS;
	int nbrligne;
	byte TABLE_COUL[PALETTE_SIZE];
	byte cmap[PALETTE_BLOCK_SIZE];
	byte Palette[PALETTE_BLOCK_SIZE];
	bool Linear;
	Graphics::Surface *VideoPtr;
	Graphics::Surface VESA_SCREEN;
	Graphics::Surface VESA_BUFFER;
	int start_x;
	int ofscroll;
	int SCROLL;
	byte HEADER_PCX[128];
	int PCX_L, PCX_H;
	bool DOUBLE_ECRAN;
	int OLD_SCROLL;
	int MANU_SCROLL;
	int SPEED_SCROLL;
	int nbrligne2;
	int Agr_x, Agr_y;
	int Agr_Flag_x, Agr_Flag_y;
	int FADESPD;
	byte PALPCX[800];
public:
	GraphicsManager();
	~GraphicsManager();

	void SET_MODE(int width, int height);
	void DD_Lock();
	void DD_Unlock();
	void Cls_Video();
	void LOAD_IMAGE(const Common::String &file);
	void INIT_TABLE(int a1, int a2, byte *a3);
	int SCROLL_ECRAN(int amount);
	void Trans_bloc(byte *destP, byte *srcP, int count, int param1, int param2);
	void A_PCX640_480(byte *surface, const Common::String &file, byte *palette, bool typeFlag);
	void Cls_Pal();
	void souris_max();
	void SCANLINE(int pitch);
	void m_scroll(const byte *surface, int xs, int ys, int width, int height, int destX, int destY);
	void m_scroll2(const byte *surface, int xs, int ys, int width, int height, int destX, int destY);
	void m_scroll2A(const byte *surface, int xs, int ys, int width, int height, int destX, int destY);
	void m_scroll16(const byte *surface, int xs, int ys, int width, int height, int destX, int destY);
	void m_scroll16A(const byte *surface, int xs, int ys, int width, int height, int destX, int destY);
	void fade_in(const byte *palette, int step, const byte *surface);
	void fade_out(const byte *palette, int step, const byte *surface);
	void FADE_INS();
	void FADE_OUTS();
	void FADE_INW();
	void FADE_OUTW();
	void setpal_vga256(const byte *palette);
	void CHANGE_PALETTE(const byte *palette);
	uint16 MapRGB(byte r, byte g, byte b);
	void DD_VBL();
};

class ObjectManager {
public:
	static byte *CHANGE_OBJET(int objIndex);
	static byte *CAPTURE_OBJET(int objIndex, int mode);

	static int Get_Largeur(const byte *objectData, int objIndex);
	static int Get_Hauteur(const byte *objectData, int objIndex);
	static int sprite_alone(const byte *objectData, byte *sprite, int objIndex);
	static byte *DEL_FICHIER_OBJ();

	static byte *CHARGE_SPRITE(const Common::String &file);
	static int capture_mem_sprite(const byte *objectData, byte *sprite, int objIndex);
	static int AJOUTE_OBJET(int objIndex);
};

} // End of namespace Hopkins

#endif /* HOPKINS_GRAPHICS_H */