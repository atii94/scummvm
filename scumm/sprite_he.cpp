/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2005 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include "stdafx.h"

#include "scumm/intern.h"
#include "scumm/resource.h"
#include "scumm/scumm.h"
#include "scumm/sprite_he.h"
#include "scumm/wiz_he.h"

namespace Scumm {

void ScummEngine_v90he::allocateArrays() {
	ScummEngine::allocateArrays();
	spritesAllocTables(_numSprites, 64, 64);
}

//
// spriteInfoGet functions
//

int ScummEngine_v90he::spriteInfoGet_flags_1(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_1: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF15) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_2(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_2: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF14) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_3(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_3: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF16) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_4(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_4: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF13) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_5(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_5: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF20) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_6(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_6: Invalid sprite %d");

	return _spriteTable[spriteId].flags & kSF22;
}

int ScummEngine_v90he::spriteInfoGet_flags_7(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_7: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF23) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_flags_8(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "_spriteTableGet_flags_8: Invalid sprite %d");

	return ((_spriteTable[spriteId].flags & kSF31) != 0) ? 1 : 0;
}

int ScummEngine_v90he::spriteInfoGet_resId(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_resId: Invalid sprite %d");

	return _spriteTable[spriteId].res_id;
}

int ScummEngine_v90he::spriteInfoGet_resState(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_resState: Invalid sprite %d");

	return _spriteTable[spriteId].res_state;
}

int ScummEngine_v90he::spriteInfoGet_groupNum(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_groupNum: Invalid sprite %d");

	return _spriteTable[spriteId].group_num;
}

int ScummEngine_v90he::spriteInfoGet_field_18(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_18: Invalid sprite %d");

	return _spriteTable[spriteId].field_18;
}

int ScummEngine_v90he::spriteInfoGet_grp_tx(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_grp_tx: Invalid sprite %d");

	if (_spriteTable[spriteId].group_num)
		return _spriteTable[spriteId].tx + _spriteGroups[spriteId].tx;
	else
		return _spriteTable[spriteId].tx;
}

int ScummEngine_v90he::spriteInfoGet_grp_ty(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_grp_ty: Invalid sprite %d");

	if (_spriteTable[spriteId].group_num)
		return _spriteTable[spriteId].ty + _spriteGroups[spriteId].ty;
	else
		return _spriteTable[spriteId].ty;
}

int ScummEngine_v90he::spriteInfoGet_field_44(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_44: Invalid sprite %d");

	return _spriteTable[spriteId].field_44;
}

int ScummEngine_v90he::spriteInfoGet_field_54(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_54: Invalid sprite %d");

	return _spriteTable[spriteId].field_54;
}

int ScummEngine_v90he::spriteInfoGet_wizSize(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_wizSize: Invalid sprite %d");

	return _spriteTable[spriteId].res_wiz_states;
}

int ScummEngine_v90he::spriteInfoGet_zoom(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_zoom: Invalid sprite %d");

	return _spriteTable[spriteId].zoom;
}

int ScummEngine_v90he::spriteInfoGet_field_78(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_78: Invalid sprite %d");

	return _spriteTable[spriteId].field_78;
}

int ScummEngine_v90he::spriteInfoGet_field_80(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_80: Invalid sprite %d");

	return _spriteTable[spriteId].field_80;
}

void ScummEngine_v90he::getSpriteImageDim(int spriteId, int32 &w, int32 &h) {
	checkRange(_numSprites, 1, spriteId, "getSpriteImageDim: Invalid sprite %d");

	getWizImageDim(_spriteTable[spriteId].res_id, _spriteTable[spriteId].res_state, w, h);
}

void ScummEngine_v90he::spriteInfoGet_tx_ty(int spriteId, int32 &tx, int32 &ty) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_tx_ty: Invalid sprite %d");

	tx = _spriteTable[spriteId].tx;
	ty = _spriteTable[spriteId].ty;
}

void ScummEngine_v90he::spriteInfoGet_field_2C_30(int spriteId, int32 &field_2C, int32 &field_30) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_2C_30: Invalid sprite %d");

	field_2C = _spriteTable[spriteId].field_2C;
	field_30 = _spriteTable[spriteId].field_30;
}

//
// spriteGroupGet functions
//

int ScummEngine_v90he::spriteGroupGet_field_10(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_10: Invalid sprite %d");

	return _spriteGroups[spriteId].field_10;
}

int ScummEngine_v90he::spriteGroupGet_field_20(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_20: Invalid sprite %d");

	return _spriteGroups[spriteId].field_20;
}

int ScummEngine_v90he::spriteGroupGet_field_30(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_30: Invalid sprite %d");

	return _spriteGroups[spriteId].field_30;
}

int ScummEngine_v90he::spriteGroupGet_field_34(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_34: Invalid sprite %d");

	return _spriteGroups[spriteId].field_34;
}

int ScummEngine_v90he::spriteGroupGet_field_38(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_38: Invalid sprite %d");

	return _spriteGroups[spriteId].field_38;
}

int ScummEngine_v90he::spriteGroupGet_field_3C(int spriteId) {
	checkRange(_numSprites, 1, spriteId, "spriteInfoGet_field_3C: Invalid sprite %d");

	return _spriteGroups[spriteId].field_3C;
}

void ScummEngine_v90he::spriteGroupGet_tx_ty(int spriteId, int32 &tx, int32 &ty) {
	checkRange(_numSprites, 1, spriteId, "spriteGroupGet_tx_ty: Invalid sprite %d");

	tx = _spriteGroups[spriteId].tx;
	ty = _spriteGroups[spriteId].ty;
}

//
// spriteInfoSet functions
//
void ScummEngine_v90he::spriteInfoSet_field_14(int spriteId, int value) {
	checkRange(_varNumSprites, 1, spriteId, "Invalid sprite %d");

	if (_spriteTable[spriteId].field_14 != value) {
		_spriteTable[spriteId].field_14 = value;
		_spriteTable[spriteId].flags |= kSF01 | kSF02;
	}
}

void ScummEngine_v90he::spriteInfoSet_field_7C(int spriteId, int value) {
	checkRange(_varNumSprites, 1, spriteId, "Invalid sprite %d");

	if (_spriteTable[spriteId].field_7C != value) {
		_spriteTable[spriteId].field_7C = value;
		_spriteTable[spriteId].flags |= kSF01 | kSF02;
	}
}

void ScummEngine_v90he::spriteInfoSet_field_80(int spriteId, int value) {
	checkRange(_varNumSprites, 1, spriteId, "Invalid sprite %d");

	_spriteTable[spriteId].field_80 = value;
}

void ScummEngine_v90he::spriteInfoSet_resState(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (_spriteTable[spriteNum].res_id) {
		int state;

		state = MAX(value, _spriteTable[spriteNum].res_wiz_states - 1);
		if (state < 0)
			state = 0;
	
		if (_spriteTable[spriteNum].res_state != state) {
			_spriteTable[spriteNum].res_state = state;
			_spriteTable[spriteNum].flags |= kSF01 | kSF02;
		}
	}
}

void ScummEngine_v90he::spriteInfoSet_tx_ty(int spriteNum, int value1, int value2) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (_spriteTable[spriteNum].tx != value1 || _spriteTable[spriteNum].ty != value2) {
		_spriteTable[spriteNum].tx = value1;
		_spriteTable[spriteNum].ty = value2;
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
	}
}

void ScummEngine_v90he::spriteInfoSet_groupNum(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");
	checkRange(_varNumSpriteGroups, 1, value, "Invalid sprite group%d");

	_spriteTable[spriteNum].group_num = value;
}

void ScummEngine_v90he::spriteInfoSet_field_2C_30(int spriteNum, int value1, int value2) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].field_2C = value1;
	_spriteTable[spriteNum].field_30 = value2;
}

void ScummEngine_v90he::spriteInfoSet_field_54(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].field_54 = value;
	if (_spriteTable[spriteNum].res_id)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_field_44(int spriteNum, int value1, int value2) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].field_44 = value2;
}

void ScummEngine_v90he::spriteInfoSet_field_18(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].field_18 = value;
}

void ScummEngine_v90he::spriteInfoSet_Inc_tx_ty(int spriteNum, int value1, int value2) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].tx += value1;
	_spriteTable[spriteNum].ty += value2;

	if  (_spriteTable[spriteNum].tx || _spriteTable[spriteNum].ty)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_zoom(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].flags |= kSFZoomed;

	if (_spriteTable[spriteNum].zoom != value) {
		_spriteTable[spriteNum].zoom = value;

		if (_spriteTable[spriteNum].res_id)
			_spriteTable[spriteNum].flags |= kSF01 | kSF02;
	}
}

void ScummEngine_v90he::spriteInfoSet_rotAngle(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].flags |= kSFRotated;

	if (_spriteTable[spriteNum].rot_angle != value) {
		_spriteTable[spriteNum].rot_angle = value;

		if (_spriteTable[spriteNum].res_id)
			_spriteTable[spriteNum].flags |= kSF01 | kSF02;
	}
}

void ScummEngine_v90he::spriteInfoSet_flag22(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSF22;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF22);
}

void ScummEngine_v90he::spriteInfoSet_flag7(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSF07;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF07);

	if (_spriteTable[spriteNum].res_id)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_flagRotated(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSFRotated;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSFRotated);

	if (_spriteTable[spriteNum].res_id)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_flag8(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSF08;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF08);
}

void ScummEngine_v90he::spriteInfoSet_flagZoomed(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSFZoomed;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSFZoomed);

	if (_spriteTable[spriteNum].res_id)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_flag20(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSF20;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF20);

	if (_spriteTable[spriteNum].res_id)
		_spriteTable[spriteNum].flags |= kSF01 | kSF02;
}

void ScummEngine_v90he::spriteInfoSet_flags23_26(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	if (value)
		_spriteTable[spriteNum].flags |= kSF23 | kSF26;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF23 | kSF26);
}

void ScummEngine_v90he::spriteInfoSet_flag31(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	// Note that condition is inverted
	if (!value)
		_spriteTable[spriteNum].flags |= kSF31;
	else
		_spriteTable[spriteNum].flags &= ~(kSF01 | kSF31);
}

void ScummEngine_v90he::spriteInfoSet_field_78_64(int spriteNum, int value) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	_spriteTable[spriteNum].field_78 = value;
	_spriteTable[spriteNum].field_64 = value;
}

void ScummEngine_v90he::spriteInfoSet_case183(int spriteNum) {
	checkRange(_varNumSprites, 1, spriteNum, "Invalid sprite %d");

	// TODO
}

void ScummEngine_v90he::spriteInfoSet_addImageToList(int spriteNum, int imageNum, int *spriteIdptr) {
	// TODO
}

void ScummEngine_v90he::spritesAllocTables(int numSprites, int numGroups, int numImgLists) {
	_varNumSpriteGroups = numGroups;
	_numSpritesToProcess = 0;
	_varNumSprites = numSprites;
	_varNumImgLists = numImgLists;
	_spriteGroups = (SpriteGroup *)malloc((_varNumSpriteGroups + 1) * sizeof(SpriteGroup));
	_spriteTable = (SpriteInfo *)malloc((_varNumSprites + 1) * sizeof(SpriteInfo));
	_activeSpritesTable = (SpriteInfo **)malloc((_varNumSprites + 1) * sizeof(SpriteInfo *));
	_imageListTable = (uint32 *)malloc((_varNumImgLists + 1) * sizeof(uint32)); // XXX
	_imageListStack = (uint16 *)malloc((_varNumImgLists + 1) * sizeof(uint16));
}

void ScummEngine_v90he::spritesResetTables(bool refreshScreen) {
	int i;
	for (i = 0; i < _varNumImgLists; ++i) {
		_imageListStack[i] = i;
	}
	memset(_spriteTable, 0, (_varNumSprites + 1) * sizeof(SpriteInfo));
	memset(_spriteGroups, 0, (_varNumSpriteGroups + 1) * sizeof(SpriteGroup));
	for (int curGrp = 1; curGrp < _varNumSpriteGroups; ++curGrp) {
		SpriteGroup *spg = &_spriteGroups[curGrp];
		checkRange(_varNumSpriteGroups, 1, curGrp, "Invalid sprite group %d");
		if (spg->field_10 != 0) {
			spg->field_10 = 0;
			spriteGroupCheck(curGrp);
			for (i = 0; i < _numSpritesToProcess; ++i) {
				SpriteInfo *spi = _activeSpritesTable[i];
				if (spi->group_num == curGrp) {
					spi->flags |= kSF01 | kSF02;
				}
			}
		}
		if (spg->tx != 0 || spg->ty != 0) {
			spg->tx = spg->ty = 0;
			spriteGroupCheck(curGrp);
			for (i = 0; i < _numSpritesToProcess; ++i) {
				SpriteInfo *spi = _activeSpritesTable[i];
				if (spi->group_num == curGrp) {
					spi->flags |= kSF01 | kSF02;
				}
			}
		}
		spg->flags &= ~kSGF01;
		spriteMarkIfInGroup(curGrp, kSF01 | kSF02);
		if (spg->field_20 != 0) {
			spriteGroupCheck(curGrp);
			for (i = 0; i < _numSpritesToProcess; ++i) {
				SpriteInfo *spi = _activeSpritesTable[i];
				if (spi->group_num == curGrp) {
					spi->flags |= kSF01 | kSF02;
				}
			}
		}
		spriteGroupCheck(curGrp);
		spg->scaling = 0;
		spg->scale_x = 0x3F800000;
		spg->field_30 = 0;
		spg->field_34 = 0;
		spg->scale_y = 0x3F800000;
		spg->field_38 = 0;
		spg->field_3C = 0;
	}
	if (refreshScreen) {
		gdi.copyVirtScreenBuffers(Common::Rect(_screenWidth, _screenHeight));
	}
	_numSpritesToProcess = 0;
}

void ScummEngine_v90he::spriteGroupCheck(int sprGrpId) {
	// XXX
}

void ScummEngine_v90he::spriteMarkIfInGroup(int sprGrpId, uint32 flags) {
	// XXX
}

} // End of namespace Scumm
