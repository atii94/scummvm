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

#include "common/textconsole.h"
#include "common/file.h"
#include "hopkins/globals.h"
#include "hopkins/files.h"
#include "hopkins/font.h"
#include "hopkins/graphics.h"
#include "hopkins/hopkins.h"

namespace Hopkins {

Globals::Globals() {
	FR = 0;
	SVGA = 2;
	internet = 1;
	PUBEXIT = 0;
	vitesse = 1;
	INSTALL_TYPE = 1;
	MUSICVOL = 6;
	SOUNDVOL = 6;
	VOICEVOL = 6;
	XSETMODE = 0;
	XZOOM = 0;
	lItCounter = 0;
	lOldItCounter = 0;
	g_old_anim = 0;
	g_old_sens = 0;
	police_l = police_h = 0;
	TETE = NULL;
	texte_long = 0;
	TEXTE_FORMATE = 0;
	OBJET_EN_COURS = 0;
	NUM_FICHIER_OBJ = 0;
	nbrligne = 0;
	largeur_boite = 0;
	hauteur_boite = 0;
	FORET = 0;
	OBJL = OBJH = 0;
	HELICO = 0;
	CAT_POSI = 0;
	CAT_TAILLE = 0;
	Nouv_objet = 0;
	iRegul = 0;
	SORTIE = 0;
	PLANX = PLANY = 0;
	PERSO = 0;
	PASSWORD = 0;
	ECRAN = 0;
	NOSPRECRAN = 0;
	OLD_ECRAN = 0;
	Max_Propre_Gen = 0;
	Max_Ligne_Long = 0;
	Max_Perso_Y = 0;
	Max_Propre = 0;
	NBBLOC = 0;
	opt_scrtype = 0;
	opt_scrspeed = 0;
	opt_vitesse = 0;
	opt_sound = 0;
	opt_voice = 0;
	opt_music = 0;
	opt_txt = 0;
	opt_anm = 0;
	NBTRI = 0;
	AFFINVEN = 0;

	// Initialise pointers
	ICONE = NULL;
	BUF_ZONE = NULL;
	CACHE_BANQUE[6] = NULL;
	Winventaire = NULL;
	texte_tmp = NULL;
	SPRITE_ECRAN = NULL;
	SAUVEGARDE = NULL;
	BUFFERTAPE = NULL;
	essai0 = NULL;
	essai1 = NULL;
	essai2 = NULL;
	Bufferobjet = NULL;
	inventaire2 = NULL;
	GESTE = NULL;
	INVENTAIRE_OBJET = NULL;
	FORETSPR = NULL;
	COUCOU = NULL;
	chemin = NULL;
	cache_souris = NULL;
	BufLig = NULL;
	Bufferdecor = NULL;
	ADR_FICHIER_OBJ = NULL;
	police = NULL;
	PERSO = NULL;
	OPTION_SPR = NULL;

	// Reset flags
	MUSICOFF = false;
	SOUNDOFF = false;
	VOICEOFF = false;
	XFULLSCREEN = false;
	XFORCE16 = false;
	XFORCE8 = false;
	CARD_SB = false;
	SOUNDOFF = false;
	MUSICOFF = false;
	VOICEOFF = false;
	CENSURE = false;
	GESTE_FLAG = false;
	redraw = false;
	BPP_NOAFF = false;
	DESACTIVE_INVENT = false;
	FLAG_VISIBLE = false;
	netscape = false;
	NOMARCHE = false;
	NO_VISU = false;
	OPTION_FLAG = false;
	CACHEFLAG = false;
	NOPARLE = false;
}

Globals::~Globals() {
	free(ICONE);
	free(BUF_ZONE);
	free(CACHE_BANQUE[6]);
	free(Winventaire);
	free(texte_tmp);
	free(SPRITE_ECRAN);
	free(SAUVEGARDE);
	free(BUFFERTAPE);
	free(Bufferobjet);
	free(inventaire2);
	free(GESTE);
	free(INVENTAIRE_OBJET);
	free(FORETSPR);
	free(COUCOU);
	free(chemin);
	free(cache_souris);
	free(Bufferdecor);
	free(ADR_FICHIER_OBJ);
	free(PERSO);
}

void Globals::setParent(HopkinsEngine *vm) {
	_vm = vm;
}

void Globals::setConfig() {
	HOPIMAGE = "BUFFER";
	HOPANIM = "ANIM";
	HOPLINK = "LINK";
	HOPSAVE = "SAVE";
	HOPSOUND = "SOUND";
	HOPMUSIC = "MUSIC";
	HOPVOICE = "VOICE";
	HOPANM = "ANM";
	HOPSEQ = "SEQ";

	switch (FR) {
	case 0:
		FICH_ZONE = "ZONEAN.TXT";
		FICH_TEXTE = "TEXTEAN.TXT";
		break;
	case 1:
		FICH_ZONE = "ZONE01.TXT";
		FICH_TEXTE = "TEXTE01.TXT";
		break;
	case 2:
		FICH_ZONE = "ZONEES.TXT";
		FICH_TEXTE = "TEXTEES.TXT";
		break;
	}
}

void Globals::clearAll() {
	// TODO: The original allocated an explicit memory block for the null pointer
	// to point to. For now, we're seeing if the NULL value will do as well
	
	for (int idx = 0; idx < 6; ++idx)
		CACHE_BANQUE[idx] = PTRNUL;

	nbrligne = 80;
	INIT_ANIM();
  
	texte_tmp = PTRNUL;
	texte_long = 0;
	police = PTRNUL;
	police_h = 0;
	police_l = 0;
	hauteur_boite = 0;
	largeur_boite = 0;
	
	_vm->_fontManager.clearAll();

	INIT_VBOB();
	ADR_FICHIER_OBJ = PTRNUL;
	NUM_FICHIER_OBJ = 0;
	Bufferdecor = PTRNUL;
	Bufferobjet = PTRNUL;
	Winventaire = PTRNUL;
	inventaire2 = PTRNUL;
	COUCOU = PTRNUL;
	SPRITE_ECRAN = PTRNUL;
	SAUVEGARDE = (Sauvegarde *)PTRNUL;
	OBJET_EN_COURS = 0;
  
	for (int idx = 0; idx < 105; ++idx) {
		ZONEP[idx].field0 = 0;
		ZONEP[idx].field2 = 0;
		ZONEP[idx].field4 = 0;
	}

	essai0 = PTRNUL;
	essai1 = PTRNUL;
	essai2 = PTRNUL;
	BufLig = PTRNUL;
	chemin = PTRNUL;

	for (int idx = 0; idx < 400; ++idx) {
		_vm->_linesManager.Ligne[idx].field0 = 0;
		_vm->_linesManager.Ligne[idx].field2 = 0;
		_vm->_linesManager.Ligne[idx].field4 = 0;
		_vm->_linesManager.Ligne[idx].field6 = 0;
		_vm->_linesManager.Ligne[idx].field8 = 0;
		_vm->_linesManager.Ligne[idx].field12 = PTRNUL;

		_vm->_linesManager.LigneZone[idx].field0 = 0;
		_vm->_linesManager.LigneZone[idx].field2 = 0;
		_vm->_linesManager.LigneZone[idx].field4 = PTRNUL;
	}

	for (int idx = 0; idx < 100; ++idx) {
		CarreZone[idx].field0 = 0;
	}

	texte_long = 0;
	texte_tmp = PTRNUL;
	BUFFERTAPE = dos_malloc2(85000);

	SAUVEGARDE = (Sauvegarde *)malloc(sizeof(Sauvegarde));
	memset(SAUVEGARDE, 0, sizeof(Sauvegarde));

	essai0 = BUFFERTAPE;
	essai1 = BUFFERTAPE + 25000;
	essai2 = BUFFERTAPE + 50000;
	BufLig = (BUFFERTAPE + 75000);
	largeur_boite = 240;
	TEXTE_FORMATE = 300;

	Bufferobjet = dos_malloc2(2500);
	INVENTAIRE_OBJET = dos_malloc2(2500);

	ADR_FICHIER_OBJ = PTRNUL;
	FORETSPR = PTRNUL;
	FORET = 0;

	cache_souris = dos_malloc2(2500);
	GESTE = PTRNUL;
	GESTE_FLAG = false;
}

void Globals::HOPKINS_DATA() {
	// TODO: Replace all the '/ 2' with constant values
	switch (PERSO_TYPE) {
	case 0:
		HopkinsArr[0 / 2] = 0;
		HopkinsArr[2 / 2] = -2;
		HopkinsArr[4 / 2] = 0;
		HopkinsArr[6 / 2] = -3;
		HopkinsArr[8 / 2] = 0;
		HopkinsArr[10 / 2] = -6;
		HopkinsArr[12 / 2] = 0;
		HopkinsArr[14 / 2] = -1;
		HopkinsArr[16 / 2] = 0;
		HopkinsArr[18 / 2] = -3;
		HopkinsArr[20 / 2] = 0;
		HopkinsArr[22 / 2] = -3;
		HopkinsArr[24 / 2] = 0;
		HopkinsArr[26 / 2] = -5;
		HopkinsArr[28 / 2] = 0;
		HopkinsArr[30 / 2] = -3;
		HopkinsArr[32 / 2] = 0;
		HopkinsArr[34 / 2] = -6;
		HopkinsArr[36 / 2] = 0;
		HopkinsArr[38 / 2] = -3;
		HopkinsArr[40 / 2] = 0;
		HopkinsArr[42 / 2] = -3;
		HopkinsArr[44 / 2] = 0;
		HopkinsArr[46 / 2] = -3;
		HopkinsArr[48 / 2] = 9;
		HopkinsArr[50 / 2] = -4;
		HopkinsArr[52 / 2] = 8;
		HopkinsArr[54 / 2] = -4;
		HopkinsArr[56 / 2] = 6;
		HopkinsArr[58 / 2] = -2;
		HopkinsArr[60 / 2] = 9;
		HopkinsArr[62 / 2] = -2;
		HopkinsArr[64 / 2] = 9;
		HopkinsArr[66 / 2] = -3;
		HopkinsArr[68 / 2] = 9;
		HopkinsArr[70 / 2] = -3;
		HopkinsArr[72 / 2] = 9;
		HopkinsArr[74 / 2] = -4;
		HopkinsArr[76 / 2] = 9;
		HopkinsArr[78 / 2] = -2;
		HopkinsArr[80 / 2] = 9;
		HopkinsArr[82 / 2] = -2;
		HopkinsArr[84 / 2] = 8;
		HopkinsArr[86 / 2] = -2;
		HopkinsArr[88 / 2] = 9;
		HopkinsArr[90 / 2] = -3;
		HopkinsArr[92 / 2] = 9;
		HopkinsArr[94 / 2] = -2;
		HopkinsArr[96 / 2] = 13;
		HopkinsArr[98 / 2] = 0;
		HopkinsArr[100 / 2] = 13;
		HopkinsArr[102 / 2] = 0;
		HopkinsArr[104 / 2] = 13;
		HopkinsArr[106 / 2] = 0;
		HopkinsArr[108 / 2] = 13;
		HopkinsArr[110 / 2] = 0;
		HopkinsArr[112 / 2] = 14;
		HopkinsArr[114 / 2] = 0;
		HopkinsArr[116 / 2] = 13;
		HopkinsArr[118 / 2] = 0;
		HopkinsArr[120 / 2] = 13;
		HopkinsArr[122 / 2] = 0;
		HopkinsArr[124 / 2] = 12;
		HopkinsArr[126 / 2] = 0;
		HopkinsArr[128 / 2] = 12;
		HopkinsArr[130 / 2] = 0;
		HopkinsArr[132 / 2] = 14;
		HopkinsArr[134 / 2] = 0;
		HopkinsArr[136 / 2] = 13;
		HopkinsArr[138 / 2] = 0;
		HopkinsArr[140 / 2] = 14;
		HopkinsArr[142 / 2] = 0;
		HopkinsArr[144 / 2] = 10;
		HopkinsArr[146 / 2] = 3;
		HopkinsArr[148 / 2] = 9;
		HopkinsArr[150 / 2] = 3;
		HopkinsArr[152 / 2] = 10;
		HopkinsArr[154 / 2] = 4;
		HopkinsArr[156 / 2] = 8;
		HopkinsArr[158 / 2] = 2;
		HopkinsArr[160 / 2] = 7;
		HopkinsArr[162 / 2] = 1;
		HopkinsArr[164 / 2] = 10;
		HopkinsArr[166 / 2] = 2;
		HopkinsArr[168 / 2] = 9;
		HopkinsArr[170 / 2] = 2;
		HopkinsArr[172 / 2] = 7;
		HopkinsArr[174 / 2] = 4;
		HopkinsArr[176 / 2] = 7;
		HopkinsArr[178 / 2] = 3;
		HopkinsArr[180 / 2] = 8;
		HopkinsArr[182 / 2] = 0;
		HopkinsArr[184 / 2] = 9;
		HopkinsArr[186 / 2] = 1;
		HopkinsArr[188 / 2] = 9;
		HopkinsArr[190 / 2] = 1;
		HopkinsArr[192 / 2] = 0;
		HopkinsArr[194 / 2] = 4;
		HopkinsArr[196 / 2] = 0;
		HopkinsArr[198 / 2] = 4;
		HopkinsArr[200 / 2] = 0;
		HopkinsArr[202 / 2] = 6;
		HopkinsArr[204 / 2] = 0;
		HopkinsArr[206 / 2] = 3;
		HopkinsArr[208 / 2] = 0;
		HopkinsArr[210 / 2] = 4;
		HopkinsArr[212 / 2] = 0;
		HopkinsArr[214 / 2] = 3;
		HopkinsArr[216 / 2] = 0;
		HopkinsArr[218 / 2] = 4;
		HopkinsArr[220 / 2] = 0;
		HopkinsArr[222 / 2] = 4;
		HopkinsArr[224 / 2] = 0;
		HopkinsArr[226 / 2] = 6;
		HopkinsArr[228 / 2] = 0;
		HopkinsArr[230 / 2] = 3;
		HopkinsArr[232 / 2] = 0;
		HopkinsArr[234 / 2] = 3;
		HopkinsArr[236 / 2] = 0;
		HopkinsArr[238 / 2] = 3;
		break;
	case 1:
		HopkinsArr[0] = 0;
		HopkinsArr[2 / 2] = -2;
		HopkinsArr[4 / 2] = 0;
		HopkinsArr[6 / 2] = -2;
		HopkinsArr[8 / 2] = 0;
		HopkinsArr[10 / 2] = -5;
		HopkinsArr[12 / 2] = 0;
		HopkinsArr[14 / 2] = -1;
		HopkinsArr[16 / 2] = 0;
		HopkinsArr[18 / 2] = -2;
		HopkinsArr[20 / 2] = 0;
		HopkinsArr[22 / 2] = -2;
		HopkinsArr[24 / 2] = 0;
		HopkinsArr[26 / 2] = -4;
		HopkinsArr[28 / 2] = 0;
		HopkinsArr[30 / 2] = -2;
		HopkinsArr[32 / 2] = 0;
		HopkinsArr[34 / 2] = -5;
		HopkinsArr[36 / 2] = 0;
		HopkinsArr[38 / 2] = -2;
		HopkinsArr[40 / 2] = 0;
		HopkinsArr[42 / 2] = -2;
		HopkinsArr[44 / 2] = 0;
		HopkinsArr[46 / 2] = -2;
		HopkinsArr[48 / 2] = 11;
		HopkinsArr[50 / 2] = 0;
		HopkinsArr[52 / 2] = 10;
		HopkinsArr[54 / 2] = 0;
		HopkinsArr[56 / 2] = 11;
		HopkinsArr[58 / 2] = 0;
		HopkinsArr[60 / 2] = 11;
		HopkinsArr[62 / 2] = 0;
		HopkinsArr[64 / 2] = 11;
		HopkinsArr[66 / 2] = 0;
		HopkinsArr[68 / 2] = 11;
		HopkinsArr[70 / 2] = 0;
		HopkinsArr[72 / 2] = 12;
		HopkinsArr[74 / 2] = 0;
		HopkinsArr[76 / 2] = 11;
		HopkinsArr[78 / 2] = 0;
		HopkinsArr[80 / 2] = 9;
		HopkinsArr[82 / 2] = 0;
		HopkinsArr[84 / 2] = 10;
		HopkinsArr[86 / 2] = 0;
		HopkinsArr[88 / 2] = 11;
		HopkinsArr[90 / 2] = 0;
		HopkinsArr[92 / 2] = 11;
		HopkinsArr[94 / 2] = 0;
		HopkinsArr[96 / 2] = 11;
		HopkinsArr[98 / 2] = 0;
		HopkinsArr[100 / 2] = 10;
		HopkinsArr[102 / 2] = 0;
		HopkinsArr[104 / 2] = 11;
		HopkinsArr[106 / 2] = 0;
		HopkinsArr[108 / 2] = 11;
		HopkinsArr[110 / 2] = 0;
		HopkinsArr[112 / 2] = 11;
		HopkinsArr[114 / 2] = 0;
		HopkinsArr[116 / 2] = 11;
		HopkinsArr[118 / 2] = 0;
		HopkinsArr[120 / 2] = 12;
		HopkinsArr[122 / 2] = 0;
		HopkinsArr[124 / 2] = 11;
		HopkinsArr[126 / 2] = 0;
		HopkinsArr[128 / 2] = 9;
		HopkinsArr[130 / 2] = 0;
		HopkinsArr[132 / 2] = 10;
		HopkinsArr[134 / 2] = 0;
		HopkinsArr[136 / 2] = 11;
		HopkinsArr[138 / 2] = 0;
		HopkinsArr[140 / 2] = 11;
		HopkinsArr[142 / 2] = 0;
		HopkinsArr[144 / 2] = 11;
		HopkinsArr[146 / 2] = 0;
		HopkinsArr[148 / 2] = 10;
		HopkinsArr[150 / 2] = 0;
		HopkinsArr[152 / 2] = 11;
		HopkinsArr[154 / 2] = 0;
		HopkinsArr[156 / 2] = 11;
		HopkinsArr[158 / 2] = 0;
		HopkinsArr[160 / 2] = 11;
		HopkinsArr[162 / 2] = 0;
		HopkinsArr[164 / 2] = 11;
		HopkinsArr[166 / 2] = 0;
		HopkinsArr[168 / 2] = 12;
		HopkinsArr[170 / 2] = 0;
		HopkinsArr[172 / 2] = 11;
		HopkinsArr[174 / 2] = 0;
		HopkinsArr[176 / 2] = 9;
		HopkinsArr[178 / 2] = 0;
		HopkinsArr[180 / 2] = 10;
		HopkinsArr[182 / 2] = 0;
		HopkinsArr[184 / 2] = 11;
		HopkinsArr[186 / 2] = 0;
		HopkinsArr[188 / 2] = 11;
		HopkinsArr[190 / 2] = 0;
		HopkinsArr[192 / 2] = 0;
		HopkinsArr[194 / 2] = 3;
		HopkinsArr[196 / 2] = 0;
		HopkinsArr[198 / 2] = 3;
		HopkinsArr[200 / 2] = 0;
		HopkinsArr[202 / 2] = 5;
		HopkinsArr[204 / 2] = 0;
		HopkinsArr[206 / 2] = 3;
		HopkinsArr[208 / 2] = 0;
		HopkinsArr[210 / 2] = 3;
		HopkinsArr[212 / 2] = 0;
		HopkinsArr[214 / 2] = 3;
		HopkinsArr[216 / 2] = 0;
		HopkinsArr[218 / 2] = 3;
		HopkinsArr[220 / 2] = 0;
		HopkinsArr[222 / 2] = 3;
		HopkinsArr[224 / 2] = 0;
		HopkinsArr[226 / 2] = 5;
		HopkinsArr[228 / 2] = 0;
		HopkinsArr[230 / 2] = 3;
		HopkinsArr[232 / 2] = 0;
		HopkinsArr[234 / 2] = 3;
		HopkinsArr[236 / 2] = 0;
		HopkinsArr[238 / 2] = 3;
		break;
	case 2:
		HopkinsArr[0] = 0;
		HopkinsArr[2 / 2] = -2;
		HopkinsArr[4 / 2] = 0;
		HopkinsArr[6 / 2] = 0;
		HopkinsArr[8 / 2] = 0;
		HopkinsArr[10 / 2] = -3;
		HopkinsArr[12 / 2] = 0;
		HopkinsArr[14 / 2] = -2;
		HopkinsArr[16 / 2] = 0;
		HopkinsArr[18 / 2] = -2;
		HopkinsArr[20 / 2] = 0;
		HopkinsArr[22 / 2] = -1;
		HopkinsArr[24 / 2] = 0;
		HopkinsArr[26 / 2] = -2;
		HopkinsArr[28 / 2] = 0;
		HopkinsArr[30 / 2] = -1;
		HopkinsArr[32 / 2] = 0;
		HopkinsArr[34 / 2] = -3;
		HopkinsArr[36 / 2] = 0;
		HopkinsArr[38 / 2] = -2;
		HopkinsArr[40 / 2] = 0;
		HopkinsArr[42 / 2] = -2;
		HopkinsArr[44 / 2] = 0;
		HopkinsArr[46 / 2] = -2;
		HopkinsArr[48 / 2] = 8;
		HopkinsArr[50 / 2] = 0;
		HopkinsArr[52 / 2] = 9;
		HopkinsArr[54 / 2] = 0;
		HopkinsArr[56 / 2] = 5;
		HopkinsArr[58 / 2] = 0;
		HopkinsArr[60 / 2] = 9;
		HopkinsArr[62 / 2] = 0;
		HopkinsArr[64 / 2] = 7;
		HopkinsArr[66 / 2] = 0;
		HopkinsArr[68 / 2] = 7;
		HopkinsArr[70 / 2] = 0;
		HopkinsArr[72 / 2] = 7;
		HopkinsArr[74 / 2] = 0;
		HopkinsArr[76 / 2] = 7;
		HopkinsArr[78 / 2] = 0;
		HopkinsArr[80 / 2] = 6;
		HopkinsArr[82 / 2] = 0;
		HopkinsArr[84 / 2] = 7;
		HopkinsArr[86 / 2] = 0;
		HopkinsArr[88 / 2] = 6;
		HopkinsArr[90 / 2] = 0;
		HopkinsArr[92 / 2] = 9;
		HopkinsArr[94 / 2] = 0;
		HopkinsArr[96 / 2] = 8;
		HopkinsArr[98 / 2] = 0;
		HopkinsArr[100 / 2] = 9;
		HopkinsArr[102 / 2] = 0;
		HopkinsArr[104 / 2] = 5;
		HopkinsArr[106 / 2] = 0;
		HopkinsArr[108 / 2] = 9;
		HopkinsArr[110 / 2] = 0;
		HopkinsArr[112 / 2] = 7;
		HopkinsArr[114 / 2] = 0;
		HopkinsArr[116 / 2] = 7;
		HopkinsArr[118 / 2] = 0;
		HopkinsArr[120 / 2] = 7;
		HopkinsArr[122 / 2] = 0;
		HopkinsArr[124 / 2] = 7;
		HopkinsArr[126 / 2] = 0;
		HopkinsArr[128 / 2] = 6;
		HopkinsArr[130 / 2] = 0;
		HopkinsArr[132 / 2] = 7;
		HopkinsArr[134 / 2] = 0;
		HopkinsArr[136 / 2] = 6;
		HopkinsArr[138 / 2] = 0;
		HopkinsArr[140 / 2] = 9;
		HopkinsArr[142 / 2] = 0;
		HopkinsArr[144 / 2] = 8;
		HopkinsArr[146 / 2] = 0;
		HopkinsArr[148 / 2] = 9;
		HopkinsArr[150 / 2] = 0;
		HopkinsArr[152 / 2] = 5;
		HopkinsArr[154 / 2] = 0;
		HopkinsArr[156 / 2] = 9;
		HopkinsArr[158 / 2] = 0;
		HopkinsArr[160 / 2] = 7;
		HopkinsArr[162 / 2] = 0;
		HopkinsArr[164 / 2] = 7;
		HopkinsArr[166 / 2] = 0;
		HopkinsArr[168 / 2] = 7;
		HopkinsArr[170 / 2] = 0;
		HopkinsArr[172 / 2] = 7;
		HopkinsArr[174 / 2] = 0;
		HopkinsArr[176 / 2] = 6;
		HopkinsArr[178 / 2] = 0;
		HopkinsArr[180 / 2] = 7;
		HopkinsArr[182 / 2] = 0;
		HopkinsArr[184 / 2] = 6;
		HopkinsArr[186 / 2] = 0;
		HopkinsArr[188 / 2] = 9;
		HopkinsArr[190 / 2] = 0;
		HopkinsArr[192 / 2] = 0;
		HopkinsArr[194 / 2] = 2;
		HopkinsArr[196 / 2] = 0;
		HopkinsArr[198 / 2] = 0;
		HopkinsArr[200 / 2] = 0;
		HopkinsArr[202 / 2] = 2;
		HopkinsArr[204 / 2] = 0;
		HopkinsArr[206 / 2] = 1;
		HopkinsArr[208 / 2] = 0;
		HopkinsArr[210 / 2] = 2;
		HopkinsArr[212 / 2] = 0;
		HopkinsArr[214 / 2] = 2;
		HopkinsArr[216 / 2] = 0;
		HopkinsArr[218 / 2] = 2;
		HopkinsArr[220 / 2] = 0;
		HopkinsArr[222 / 2] = 2;
		HopkinsArr[224 / 2] = 0;
		HopkinsArr[226 / 2] = 2;
		HopkinsArr[228 / 2] = 0;
		HopkinsArr[230 / 2] = 1;
		HopkinsArr[232 / 2] = 0;
		HopkinsArr[234 / 2] = 2;
		HopkinsArr[236 / 2] = 0;
		HopkinsArr[238 / 2] = 2;
		break;
	default:
		break;
	}

	g_old_anim = -1;
	g_old_sens = -1;
}

void Globals::INIT_ANIM() {
	for (int idx = 0; idx < 35; ++idx) {
		Bqe_Anim[idx].data = PTRNUL;
		Bqe_Anim[idx].field4 = 0;
	}

	for (int idx = 0; idx < 8; ++idx) {
		Bank[idx].data = PTRNUL;
		Bank[idx].field4 = 0;
		Bank[idx].filename1 = "";
		Bank[idx].fileHeader = 0;
		Bank[idx].field1C = 0;
		Bank[idx].filename2 = "";
	}
}

void Globals::INIT_VBOB() {
	for (int idx = 0; idx < 30; ++idx) {
		VBob[idx].field4 = 0;
		VBob[idx].field6 = 0;
		VBob[idx].field8 = 0;
		VBob[idx].fieldA = 0;
		VBob[idx].fieldC = 0;
		VBob[idx].field10 = PTRNUL;
		VBob[idx].field0 = PTRNUL;
		VBob[idx].field1C = PTRNUL;
	}
}

void Globals::CLEAR_VBOB() {
	for (int idx = 0; idx < 30; ++idx) {
		VBob[idx].field4 = 0;
		VBob[idx].field6 = 0;
		VBob[idx].field8 = 0;
		VBob[idx].fieldA = 0;
		VBob[idx].fieldC = 0;
		VBob[idx].field10 = PTRNUL;
		VBob[idx].field0 = PTRNUL;
		VBob[idx].field1C = PTRNUL;
	} 
}

void Globals::CHARGE_OBJET() {
	FileManager::CONSTRUIT_SYSTEM("OBJET.DAT");
	byte *data = FileManager::CHARGE_FICHIER(NFICHIER);
	byte *srcP = data;

	for (int idx = 0; idx < 300; ++idx) {
		ObjetW[idx].field0 = *srcP++;
		ObjetW[idx].field1 = *srcP++;
		ObjetW[idx].field2 = *srcP++;
		ObjetW[idx].field3 = *srcP++;
		ObjetW[idx].field4 = *srcP++;
		ObjetW[idx].field5 = *srcP++;
		ObjetW[idx].field6 = *srcP++;
		ObjetW[idx].field7 = *srcP++;
	}

	free(data);
}

byte *Globals::dos_malloc2(int count) {
	byte *result = (byte *)malloc(count);
	if (!result)
		result = PTRNUL;
	return result;
}

byte *Globals::dos_free2(byte *p) {
	free(p);
	return PTRNUL;
}

byte *Globals::LIBERE_FICHIER(byte *p) {
	dos_free2(p);
	return PTRNUL;
}

void Globals::RESET_CACHE() {
	byte *dataP;

	for (int idx = 1; idx <= 5; ++idx) {
		dataP = CACHE_BANQUE[idx];
		if (dataP != PTRNUL && dataP)
			CACHE_BANQUE[idx] = dos_free2(CACHE_BANQUE[idx]);
		CACHE_BANQUE[idx] = PTRNUL;
	}

	for (int idx = 0; idx <= 20; ++idx) {
		Cache[idx].fieldC = PTRNUL;
		Cache[idx].field0 = 0;
		Cache[idx].field4 = 0;
		Cache[idx].field2 = 0;
		Cache[idx].fieldA = 0;
		Cache[idx].field6 = 0;
		Cache[idx].field8 = 0;
		Cache[idx].field10 = 0;
		Cache[idx].field12 = 0;
		Cache[idx].field14 = 0;
	}

	CACHEFLAG = 0;
}

void Globals::CACHE_ON() {
	CACHEFLAG = 1;
}
// TODO: Find why some calls have a parameter value
void Globals::CACHE_OFF(int v1) {
	CACHEFLAG = 0;
}

void Globals::CACHE_SUB(int idx) {
	Cache[idx].fieldA = 0;
}

void Globals::CACHE_ADD(int idx) {
	Cache[idx].fieldA = 1;
}

void Globals::CHARGE_CACHE(const Common::String &file) {
	byte *v2 = PTRNUL;
	int v4;
	int v5;
	int v6; 
	int v8; 
	int v9; 
	int v11;
	byte *spriteData; 
	byte *ptr; 
	int v14; 
	int v15;
	Common::String v16;
	Common::File f;

	RESET_CACHE();
	FileManager::CONSTRUIT_FICHIER(HOPLINK, file);
	ptr = FileManager::CHARGE_FICHIER(NFICHIER);
	v16 = Common::String((const char *)ptr);

	FileManager::CONSTRUIT_FICHIER(HOPLINK, v16);
	
	if (!f.exists(NFICHIER)) {
		spriteData = FileManager::CHARGE_FICHIER(NFICHIER);
		CACHE_BANQUE[1] = spriteData;
		v15 = 60;
		v14 = 0;
		do {
			v11 = READ_LE_UINT16((uint16 *)ptr + v15);
			v4 = READ_LE_UINT16((uint16 *)ptr + v15 + 1);
			v5 = READ_LE_UINT16((uint16 *)ptr + v15 + 2);
			v6 = v14;
			Cache[v6].field14 = READ_LE_UINT16((uint16 *)ptr + v15 + 4);
			Cache[v6].field2 = v11;
			Cache[v6].field0 = v4;
			Cache[v6].field4 = v5;
			Cache[v6].field12 = 1;
			if (spriteData == PTRNUL) {
				Cache[v14].fieldA = 0;
			} else {
				v8 = _vm->_objectsManager.Get_Largeur(spriteData, v11);
				v9 = _vm->_objectsManager.Get_Hauteur(spriteData, v11);
				Cache[v14].fieldC = spriteData;
				Cache[v14].field6 = v8;
				Cache[v14].field8 = v9;
				Cache[v14].fieldA = 1;
			}
      
			if ( !Cache[v14].field0 && !Cache[v14].field4 && !Cache[v14].field2)
				Cache[v14].fieldA = 0;
			v15 += 5;
			++v14;
		} while (v14 <= 21);
		CACHE_ON();
		v2 = ptr;
	}
	dos_free2(v2);
}

void Globals::B_CACHE_OFF(int idx) {
	Bob[idx].field34 = 1;
}


} // End of namespace Hopkins