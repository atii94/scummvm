/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2003 The ScummVM project
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

#include <PalmOS.h>
#include <SonyClie.h>

#include "StarterRsc.h"
#include "skin.h"
#include "globals.h"
#include "scumm_globals.h"
#include "engine.h"
#include "vibrate.h"
#include "pa1lib.h"
#include "start.h"
#include "games.h"

#ifndef DISABLE_TAPWAVE
//#include "tapwave.h"
#endif

/***********************************************************************
 *
 *	Internal Structures
 *
 ***********************************************************************/

typedef struct {
	UInt16 volRefNum;
	Char nameP[expCardInfoStringMaxLen+1];

} CardInfoType;

/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/
GlobalsPreferencePtr gPrefs;
GlobalsDataPtr gVars;

static UInt16 sknLastOn = skinButtonNone;
static Boolean bStartScumm = false;

// form list draw
#define	ITEM_TYPE_UNKNOWN	'U'
#define ITEM_TYPE_CARD		'C'
#define ITEM_TYPE_SKIN		'S'

#define frmRedrawUpdateMS		(frmRedrawUpdateCode+1)
#define frmRedrawUpdateMSImport	(frmRedrawUpdateCode+2)

static Char **itemsText = NULL;
static void  *itemsList = NULL;
static Char   itemsType = ITEM_TYPE_UNKNOWN;
/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

// Define the minimum OS version we support (3.5 for now).
#define kOurMinVersion	sysMakeROMVersion(3,5,0,sysROMStageRelease,0)
#define kPalmOS10Version	sysMakeROMVersion(1,0,0,sysROMStageRelease,0)
#define kOS5Version		sysMakeROMVersion(5,0,0,sysROMStageRelease,0)

// edit game mode
enum {
	edtModeAdd,
	edtModeEdit,
	edtModeParams
} editMode;
/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/


static void FrmReturnToMain(UInt16 updateCode = frmRedrawUpdateMS) {
	// if there is a form loaded, prevent crash on OS5
	if (FrmGetFirstForm()) {
		FrmUpdateForm(MainForm, updateCode);
		FrmReturnToForm(MainForm);
	}
}



 /////////////////////////////////////////////////////////////////////////

// Callback for ExgDBWrite to send data with Exchange Manager
static Err WriteDBData(const void* dataP, UInt32* sizeP, void* userDataP)
{
	Err err;
	*sizeP = ExgSend((ExgSocketPtr)userDataP, (void*)dataP, *sizeP, &err);
	return err;
}

static Err SendDatabase (UInt16 cardNo, LocalID dbID, Char *nameP, Char *descriptionP)
{
	ExgSocketType exgSocket;
	Err err;

	// Create exgSocket structure
	MemSet(&exgSocket, sizeof(exgSocket), 0);
	exgSocket.description = descriptionP;
	exgSocket.name = nameP;

	// Start an exchange put operation
	err = ExgPut(&exgSocket);
	if (!err) {
		err = ExgDBWrite(WriteDBData, &exgSocket, NULL, dbID, cardNo);
		err = ExgDisconnect(&exgSocket, err);
	}

	return err;
}

static Err BeamMe() {
	UInt16 cardNo;
	LocalID dbID;
	Err err;

	err = SysCurAppDatabase(&cardNo, &dbID);
	if (dbID)
		err = SendDatabase(0, dbID, "ScummVM.prc", "\nPlay your favorite LucasArts games");
	else
		err = DmGetLastErr();
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Palm OS 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.
			if (romVersion <= kPalmOS10Version)
				{
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		return sysErrRomIncompatible;
		}

	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  formId - id of the form to display
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void * GetObjectPtr(UInt16 objectID)
{
	FormPtr frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}

static void FrmList(EventPtr eventP, UInt16 objectID)
{
	ListType *listP;
	UInt16 listItem;

	listP = (ListType *)GetObjectPtr(objectID);
	listItem = LstPopupList(listP);
	CtlSetLabel(eventP->data.ctlSelect.pControl, LstGetSelectionText(listP, LstGetSelection(listP)));
}

//#############################################################################
//#############################################################################


static void GamUnselect() {
	GameInfoType modGame;
	GameInfoType *game;

	MemHandle recordH;
	UInt16 index;

	index = GamGetSelected();
	
	if (index != dmMaxRecordIndex) {
		recordH = DmGetRecord(gameDB, index);
		game = (GameInfoType *)MemHandleLock(recordH);

		MemMove(&modGame, game, sizeof(GameInfoType));	
		modGame.selected = !modGame.selected;
		DmWrite(game, 0, &modGame, sizeof(GameInfoType));

		MemHandleUnlock(recordH);
		DmReleaseRecord (gameDB, index, 0);
	}
}

static void FldTrimText(FieldType * fldP) {
	MemHandle tmpH;
	Char *tmpP;

	tmpH = FldGetTextHandle(fldP);
	FldSetTextHandle(fldP, NULL);
	tmpP = (Char *)MemHandleLock(tmpH);
	TxtGlueStripSpaces(tmpP, true, true);
	MemHandleUnlock(tmpH);
	FldSetTextHandle(fldP, tmpH);
}


/***********************************************************************
 *
 * FUNCTION:    EditGameFormSave
 * FUNCTION:    EditGameFormInit
 * FUNCTION:    EditGameFormHandleEvent
 *
 * DESCRIPTION: 
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void EditGameFormDelete(Boolean direct) {
	UInt16 index = GamGetSelected();

	if (index == dmMaxRecordIndex) {
		FrmCustomAlert(FrmWarnAlert, "Select an entry first.",0,0);
		return;

	} else if (FrmCustomAlert(FrmConfirmAlert, "Do you really want to delete this entry ?", 0, 0) == FrmConfirmYes) {
		DmRemoveRecord(gameDB, index);
		if (!direct)
			FrmReturnToMain();
		GamSortList();
		SknUpdateList();
	}
}

static void EditGameFormSave(UInt16 index) {

	FieldType *fld1P, *fld2P, *fld3P, *fld4P, *fld5P, *fld6P;	// need to change this with good names
	ControlType *cck1P, *cck2P, *cck3P, *cck4P, *cck5P;	
	ListType *list1P, *list2P, *list3P;
	FormPtr frmP;

	MemHandle recordH;
	GameInfoType *gameInfo, newGameInfo;
//	UInt16 index;

	list1P = (ListType *)GetObjectPtr(EditGameGfxListList);
	list2P = (ListType *)GetObjectPtr(EditGameLanguageList);
	list3P = (ListType *)GetObjectPtr(EditGamePlatformList);
	
	fld1P = (FieldType *)GetObjectPtr(EditGameEntryNameField);
	fld2P = (FieldType *)GetObjectPtr(EditGamePathField);
	fld3P = (FieldType *)GetObjectPtr(EditGameGameField);
	fld4P = (FieldType *)GetObjectPtr(EditGameLoadSlotField);
	fld5P = (FieldType *)GetObjectPtr(EditGameStartRoomField);
	fld6P = (FieldType *)GetObjectPtr(EditGameTalkSpeedField);
	
	cck1P = (ControlType *)GetObjectPtr(EditGameLoadSlotCheckbox);
	cck2P = (ControlType *)GetObjectPtr(EditGameStartRoomCheckbox);
	cck3P = (ControlType *)GetObjectPtr(EditGameAmigaCheckbox);
	cck4P = (ControlType *)GetObjectPtr(EditGameSubtitlesCheckbox);
	cck5P = (ControlType *)GetObjectPtr(EditGameTalkSpeedCheckbox);

	frmP = FrmGetActiveForm();

	FldTrimText(fld1P);
	FldTrimText(fld2P);
	FldTrimText(fld3P);

	if (FldGetTextLength(fld1P) == 0) {
		FrmCustomAlert(FrmWarnAlert,"You must specified an entry name.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, EditGameEntryNameField));
		return;
	} else if (FldGetTextLength(fld2P) == 0) {
		FrmCustomAlert(FrmWarnAlert,"You must specified a path.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, EditGamePathField));
		return;
	} else if (FldGetTextLength(fld3P) == 0) {
		FrmCustomAlert(FrmWarnAlert,"You must specified a game.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, EditGameGameField));
		return;
	} else if (FldGetTextLength(fld5P) == 0 && CtlGetValue(cck2P) == 1) {
		FrmCustomAlert(FrmWarnAlert,"You must specified a room number.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, EditGameStartRoomField));
		return;
	} else if (FldGetTextLength(fld6P) == 0 && CtlGetValue(cck5P) == 1) {
		FrmCustomAlert(FrmWarnAlert,"You must specified a talk speed.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, EditGameTalkSpeedField));
		return;
	}

//	index = GamGetSelected();

	if (index != dmMaxRecordIndex) {
		recordH = DmGetRecord(gameDB, index);
	} else {
		index = dmMaxRecordIndex;
		GamUnselect();
		recordH = DmNewRecord(gameDB, &index, sizeof(GameInfoType));
	}

	gameInfo = (GameInfoType *)MemHandleLock(recordH);

	StrCopy(newGameInfo.nameP, FldGetTextPtr(fld1P));
	StrCopy(newGameInfo.pathP, FldGetTextPtr(fld2P));
	StrCopy(newGameInfo.gameP, FldGetTextPtr(fld3P));

	if (newGameInfo.pathP[StrLen(newGameInfo.pathP)-1] != '/')
		StrCat(newGameInfo.pathP, "/");

	newGameInfo.version	= curItemVersion;
	newGameInfo.icnID = 0xFFFF;
	newGameInfo.gfxMode = LstGetSelection(list1P);
	newGameInfo.language = LstGetSelection(list2P);
	newGameInfo.platform = LstGetSelection(list3P);
	newGameInfo.selected = true;
	newGameInfo.autoLoad = CtlGetValue(cck1P);
	newGameInfo.loadSlot = StrAToI(FldGetTextPtr(fld4P));
	newGameInfo.autoRoom = CtlGetValue(cck2P);
	newGameInfo.roomNum = StrAToI(FldGetTextPtr(fld5P));
	newGameInfo.setPlatform = CtlGetValue(cck3P);
	newGameInfo.subtitles = !(CtlGetValue(cck4P));
	newGameInfo.talkValue = StrAToI(FldGetTextPtr(fld6P));
	newGameInfo.talkSpeed = CtlGetValue(cck5P);

	DmWrite(gameInfo, 0, &newGameInfo, sizeof(GameInfoType));

	MemHandleUnlock(recordH);
	DmReleaseRecord (gameDB, index, 0);
	GamSortList();
	// update list position
	{
		RectangleType rArea;
		UInt16 posIndex, maxIndex, maxView;
		
		// get the sorted index
		index = GamGetSelected();
		// if new item is out of the list bounds, change current list pos
		SknGetListBounds(&rArea, NULL);
		maxView = rArea.extent.y / sknInfoListItemSize;
		posIndex = gPrefs->listPosition;
		maxIndex = DmNumRecords(gameDB);
		
		if (index == 0 && posIndex > 0) {
			gPrefs->listPosition = 0;
		} else if ((maxView + posIndex) <= index) {
			posIndex = index - (maxView - posIndex) + 1;
			gPrefs->listPosition = posIndex;
		}
	}

	FrmReturnToMain();
	SknUpdateList();
}

static void EditGameFormInit(UInt16 index) {

	FieldType *fld1P, *fld2P, *fld3P, *fld4P, *fld5P, *fld6P;
	FormPtr frmP;
	ListType *list1P, *list2P, *list3P;

	Char *nameP, *pathP, *gameP, *loadP, *roomP, *talkP;
	MemHandle nameH, pathH, gameH, loadH, roomH, talkH;

	MemHandle recordH = NULL;
	GameInfoType *game;

	list1P = (ListType *)GetObjectPtr(EditGameGfxListList);
	list2P = (ListType *)GetObjectPtr(EditGameLanguageList);
	list3P = (ListType *)GetObjectPtr(EditGamePlatformList);

	fld1P = (FieldType *)GetObjectPtr(EditGameEntryNameField);
	fld2P = (FieldType *)GetObjectPtr(EditGamePathField);
	fld3P = (FieldType *)GetObjectPtr(EditGameGameField);
	fld4P = (FieldType *)GetObjectPtr(EditGameLoadSlotField);
	fld5P = (FieldType *)GetObjectPtr(EditGameStartRoomField);
	fld6P = (FieldType *)GetObjectPtr(EditGameTalkSpeedField);

	nameH = MemHandleNew(FldGetMaxChars(fld1P)+1);
	pathH = MemHandleNew(FldGetMaxChars(fld2P)+1);
	gameH = MemHandleNew(FldGetMaxChars(fld3P)+1);
	loadH = MemHandleNew(FldGetMaxChars(fld4P)+1);
	roomH = MemHandleNew(FldGetMaxChars(fld5P)+1);
	talkH = MemHandleNew(FldGetMaxChars(fld6P)+1);

	nameP = (Char *)MemHandleLock(nameH);
	pathP = (Char *)MemHandleLock(pathH);
	gameP = (Char *)MemHandleLock(gameH);
	loadP = (Char *)MemHandleLock(loadH);
	roomP = (Char *)MemHandleLock(roomH);
	talkP = (Char *)MemHandleLock(talkH);

//	index = GamGetSelected();

	if (index != dmMaxRecordIndex) {
		recordH = DmQueryRecord(gameDB, index);
		game = (GameInfoType *)MemHandleLock(recordH);
		StrCopy(nameP, game->nameP);
		StrCopy(pathP, game->pathP);
		StrCopy(gameP, game->gameP);

		LstSetSelection(list1P, game->gfxMode);
		LstSetSelection(list2P, game->language);
		LstSetSelection(list3P, game->platform);
		
		StrIToA(loadP, game->loadSlot);
		StrIToA(roomP, game->roomNum);
		StrIToA(talkP, game->talkValue);

		CtlSetValue((ControlType *)GetObjectPtr(EditGameLoadSlotCheckbox), game->autoLoad);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameStartRoomCheckbox), game->autoRoom);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameAmigaCheckbox), game->setPlatform);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameSubtitlesCheckbox), !game->subtitles);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameTalkSpeedCheckbox), game->talkSpeed);
		
		MemHandleUnlock(recordH);
		CtlSetUsable((ControlType *)GetObjectPtr(EditGameDeleteButton),true);
	}
	else {
		MemSet(nameP,MemHandleSize(nameH),0);
		MemSet(pathP,MemHandleSize(pathH),0);
		MemSet(gameP,MemHandleSize(gameH),0);

		StrIToA(loadP, 0);
		StrIToA(roomP, 0);
		StrIToA(talkP, 60);

		CtlSetValue((ControlType *)GetObjectPtr(EditGameLoadSlotCheckbox), 0);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameStartRoomCheckbox), 0);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameAmigaCheckbox), 0);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameSubtitlesCheckbox), 0);
		CtlSetValue((ControlType *)GetObjectPtr(EditGameTalkSpeedCheckbox), 0);

		LstSetSelection(list1P, 1);
		LstSetSelection(list2P, 0);
		LstSetSelection(list3P, 0);
		CtlSetUsable((ControlType *)GetObjectPtr(EditGameDeleteButton),false);
	}

	MemHandleUnlock(nameH);
	MemHandleUnlock(pathH);
	MemHandleUnlock(gameH);
	MemHandleUnlock(loadH);
	MemHandleUnlock(roomH);
	MemHandleUnlock(talkH);

	FldSetTextHandle(fld1P, nameH);
	FldSetTextHandle(fld2P, pathH);
	FldSetTextHandle(fld3P, gameH);
	FldSetTextHandle(fld4P, loadH);
	FldSetTextHandle(fld5P, roomH);
	FldSetTextHandle(fld6P, talkH);

	CtlSetLabel((ControlType *)GetObjectPtr(EditGameGfxPopupPopTrigger), LstGetSelectionText(list1P, LstGetSelection(list1P)));
	CtlSetLabel((ControlType *)GetObjectPtr(EditGameLanguagePopTrigger), LstGetSelectionText(list2P, LstGetSelection(list2P)));
	CtlSetLabel((ControlType *)GetObjectPtr(EditGamePlatformPopTrigger), LstGetSelectionText(list3P, LstGetSelection(list3P)));

	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);
}

static Boolean EditGameFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			switch (editMode) {
				case edtModeAdd:
					EditGameFormInit(dmMaxRecordIndex);
					break;
				case edtModeEdit:
				case edtModeParams:
				default :
					EditGameFormInit(GamGetSelected());
					break;
			}
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case EditGameOKButton:
					switch (editMode) {
						case edtModeAdd:
							EditGameFormSave(dmMaxRecordIndex);
							break;
						case edtModeEdit:
						case edtModeParams:
						default :
							EditGameFormSave(GamGetSelected());
							break;
					}
					break;

				case EditGameCancelButton:
					FrmReturnToMain();
					break;
				
				case EditGameDeleteButton:
					EditGameFormDelete(false);
					break;

				case EditGameGfxPopupPopTrigger:
					FrmList(eventP, EditGameGfxListList);
					break;

				case EditGameLanguagePopTrigger:
					FrmList(eventP, EditGameLanguageList);
					break;

				case EditGamePlatformPopTrigger:
					FrmList(eventP, EditGamePlatformList);
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    SystemInfoFormInit
 * FUNCTION:     SystemInfoFormHandleEvent
 *
 * DESCRIPTION: 
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void GetMemory(UInt32* storageMemoryP, UInt32* dynamicMemoryP, UInt32 *storageFreeP, UInt32 *dynamicFreeP) {
	UInt32		free, max;

	Int16		i;
	Int16		nCards;
	UInt16		cardNo;
	UInt16		heapID;

	UInt32		storageMemory = 0;
	UInt32		dynamicMemory = 0;
	UInt32		storageFree = 0;
	UInt32		dynamicFree = 0;

	// Iterate through each card to support devices with multiple cards.
	nCards = MemNumCards();		

	for (cardNo = 0; cardNo < nCards; cardNo++) {
		// Iterate through the RAM heaps on a card (excludes ROM).
		for (i=0; i< MemNumRAMHeaps(cardNo); i++) {
			// Obtain the ID of the heap.
			heapID = MemHeapID(cardNo, i);
			// Calculate the total memory and free memory of the heap.
			MemHeapFreeBytes(heapID, &free, &max);
			
			// If the heap is dynamic, increment the dynamic memory total.
			if (MemHeapDynamic(heapID)) {
				dynamicMemory += MemHeapSize(heapID);
				dynamicFree += free;

			// The heap is nondynamic (storage ?).
			} else {
				storageMemory += MemHeapSize(heapID);
				storageFree += free;
			}
		}
	}
	// Reduce the stats to KB.  Round the results.
	dynamicMemory = dynamicMemory / 1024L;
	storageMemory = storageMemory / 1024L;

	dynamicFree = dynamicFree / 1024L;
	storageFree = storageFree / 1024L;

	if (dynamicMemoryP) *dynamicMemoryP = dynamicMemory;
	if (storageMemoryP) *storageMemoryP = storageMemory;
	if (dynamicFreeP) *dynamicFreeP = dynamicFree;
	if (storageFreeP) *storageFreeP = storageFree;
}

static void SystemInfoFormInit() {
	FormPtr frmP;
	Coord x;
	UInt32 dm, sm, df, sf;
	Char num[10];

	GetMemory(&sm, &dm, &sf, &df);
	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);

	WinSetTextColor(255);
	FntSetFont(stdFont);
	
	StrIToA(num, dm);
	x = 149 - FntCharsWidth(num, StrLen(num));
	WinDrawChars(num, StrLen(num), x, 30);

	StrIToA(num, sm);
	x = 149 - FntCharsWidth(num, StrLen(num));
	WinDrawChars(num, StrLen(num), x, 42);

	StrIToA(num, df);
	x = 109 - FntCharsWidth(num, StrLen(num));
	WinDrawChars(num, StrLen(num), x, 30);

	StrIToA(num, sf);
	x = 109 - FntCharsWidth(num, StrLen(num));
	WinDrawChars(num, StrLen(num), x, 42);
}

static Boolean SystemInfoFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			SystemInfoFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			// OK button only
			FrmReturnToMain();
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}
/***********************************************************************
 *
 * FUNCTION:    VolumeFormSave
 * FUNCTION:    VolumeFormInit
 * FUNCTION:    VolumeFormHandleEvent
 *
 * DESCRIPTION: 
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void VolumeFormSave() {
	SliderControlType *slid1P, *slid2P, *slid3P, *slid4P, *slid5P;
	
	slid1P = (SliderControlType *)GetObjectPtr(VolumeSpeakerSliderControl);
	slid2P = (SliderControlType *)GetObjectPtr(VolumeHeadphoneSliderControl);

	slid3P = (SliderControlType *)GetObjectPtr(VolumeMasterSliderControl);
	slid4P = (SliderControlType *)GetObjectPtr(VolumeMusicSliderControl);
	slid5P = (SliderControlType *)GetObjectPtr(VolumeSfxSliderControl);
	
	CtlGetSliderValues ((ControlType *)slid1P, 0, 0, 0, &gPrefs->volume.speaker) ;
	CtlGetSliderValues ((ControlType *)slid2P, 0, 0, 0, &gPrefs->volume.headphone) ;

	CtlGetSliderValues ((ControlType *)slid3P, 0, 0, 0, &gPrefs->volume.master);
	CtlGetSliderValues ((ControlType *)slid4P, 0, 0, 0, &gPrefs->volume.music);
	CtlGetSliderValues ((ControlType *)slid5P, 0, 0, 0, &gPrefs->volume.sfx);
	
	FrmReturnToMain();
}

static void VolumeFormInit() {
	SliderControlType *slid1P, *slid2P, *slid3P, *slid4P, *slid5P;
	FormPtr frmP;
	UInt16 value;
	
	slid1P = (SliderControlType *)GetObjectPtr(VolumeSpeakerSliderControl);
	slid2P = (SliderControlType *)GetObjectPtr(VolumeHeadphoneSliderControl);

	slid3P = (SliderControlType *)GetObjectPtr(VolumeMasterSliderControl);
	slid4P = (SliderControlType *)GetObjectPtr(VolumeMusicSliderControl);
	slid5P = (SliderControlType *)GetObjectPtr(VolumeSfxSliderControl);

	value = gPrefs->volume.speaker;
	CtlSetSliderValues ((ControlType *)slid1P, 0, 0, 0, &value);
	value = gPrefs->volume.headphone;
	CtlSetSliderValues ((ControlType *)slid2P, 0, 0, 0, &value);

	value = gPrefs->volume.master;
	CtlSetSliderValues ((ControlType *)slid3P, 0, 0, 0, &value);
	value = gPrefs->volume.music;
	CtlSetSliderValues ((ControlType *)slid4P, 0, 0, 0, &value);
	value = gPrefs->volume.sfx;
	CtlSetSliderValues ((ControlType *)slid5P, 0, 0, 0, &value);

	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);
}

static Boolean VolumeFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			VolumeFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case VolumeOKButton:
					VolumeFormSave();
					break;

				case VolumeCancelButton:
					FrmReturnToMain();
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}

static void SoundFormSave() {
	ControlType *cck1P, *cck2P, *cck3P, *cck4P;
	ListType *list1P;
	FieldType *fld1P, *fld2P, *fld3P;
	UInt8 tempo;
	UInt16 firstTrack;

	cck1P = (ControlType *)GetObjectPtr(SoundMusicCheckbox);
	cck2P = (ControlType *)GetObjectPtr(SoundMultiMidiCheckbox);
	cck3P = (ControlType *)GetObjectPtr(SoundMP3Checkbox);
	cck4P = (ControlType *)GetObjectPtr(SoundTrackLengthCheckbox);
	list1P = (ListType *)GetObjectPtr(SoundDriverList);
	fld1P = (FieldType *)GetObjectPtr(SoundTempoField);
	fld2P = (FieldType *)GetObjectPtr(SoundLengthSecsField);
	fld3P = (FieldType *)GetObjectPtr(SoundFirstTrackField);

	tempo = StrAToI(FldGetTextPtr(fld1P));
	if (tempo < 50 || tempo > 200) {
		FrmCustomAlert(FrmErrorAlert, "Invalid tempo value (50...200)", 0, 0);
		return;
	}

	firstTrack = StrAToI(FldGetTextPtr(fld3P));
	if (firstTrack < 1 || firstTrack > 999) {
		FrmCustomAlert(FrmErrorAlert, "Invalid track value (1...999)", 0, 0);
		return;
	}

	gPrefs->sound.music = CtlGetValue(cck1P);
	gPrefs->sound.multiMidi = CtlGetValue(cck2P);
	gPrefs->sound.MP3 = CtlGetValue(cck3P);
	gPrefs->sound.setDefaultTrackLength = CtlGetValue(cck4P);

	gPrefs->sound.driver = LstGetSelection(list1P);
	gPrefs->sound.tempo = tempo;
	gPrefs->sound.defaultTrackLength = StrAToI(FldGetTextPtr(fld2P));
	gPrefs->sound.firstTrack = firstTrack;
	FrmReturnToMain();
}

static void SoundFormInit() {
	ControlType *cck1P, *cck2P, *cck3P, *cck4P;
	ListType *list1P;
	FieldType *fld1P, *fld2P, *fld3P;
	FormPtr frmP;
	MemHandle tempoH, lengthH, firstTrackH;
	Char *tempoP, *lengthP, *firstTrackP;

	cck1P = (ControlType *)GetObjectPtr(SoundMusicCheckbox);
	cck2P = (ControlType *)GetObjectPtr(SoundMultiMidiCheckbox);
	cck3P = (ControlType *)GetObjectPtr(SoundMP3Checkbox);
	cck4P = (ControlType *)GetObjectPtr(SoundTrackLengthCheckbox);
	list1P = (ListType *)GetObjectPtr(SoundDriverList);
	fld1P = (FieldType *)GetObjectPtr(SoundTempoField);
	fld2P = (FieldType *)GetObjectPtr(SoundLengthSecsField);
	fld3P = (FieldType *)GetObjectPtr(SoundFirstTrackField);

	CtlSetValue(cck1P, gPrefs->sound.music);
	CtlSetValue(cck2P, gPrefs->sound.multiMidi);
	CtlSetValue(cck3P, gPrefs->sound.MP3);
	CtlSetValue(cck4P, gPrefs->sound.setDefaultTrackLength);

	LstSetSelection(list1P, gPrefs->sound.driver);
	CtlSetLabel((ControlType *)GetObjectPtr(SoundDriverPopTrigger), LstGetSelectionText(list1P, LstGetSelection(list1P)));

	tempoH = MemHandleNew(FldGetMaxChars(fld1P)+1);
	tempoP = (Char *)MemHandleLock(tempoH);
	StrIToA(tempoP, gPrefs->sound.tempo);
	MemHandleUnlock(tempoH);
	FldSetTextHandle(fld1P, tempoH);

	lengthH = MemHandleNew(FldGetMaxChars(fld2P)+1);
	lengthP = (Char *)MemHandleLock(lengthH);
	StrIToA(lengthP, gPrefs->sound.defaultTrackLength);
	MemHandleUnlock(lengthH);
	FldSetTextHandle(fld2P, lengthH);

	firstTrackH = MemHandleNew(FldGetMaxChars(fld3P)+1);
	firstTrackP = (Char *)MemHandleLock(firstTrackH);
	StrIToA(firstTrackP, gPrefs->sound.firstTrack);
	MemHandleUnlock(firstTrackH);
	FldSetTextHandle(fld3P, firstTrackH);

	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);
}

static Boolean SoundFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			SoundFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case SoundOKButton:
					SoundFormSave();
					break;

				case SoundCancelButton:
					FrmReturnToMain();
					break;

				case SoundDriverPopTrigger:
					FrmList(eventP, SoundDriverList);
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}
/***********************************************************************
 *
 * FUNCTION:    MiscOptionsFormSave
 * FUNCTION:    MiscOptionsFormInit
 * FUNCTION:    MiscOptionsFormHandleEvent
 *
 * DESCRIPTION: Misc. Options form functions
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void MiscOptionsFormSave() {

	FieldType *fld1P;
	ControlType *cck1P, *cck2P, *cck3P, *cck4P, *cck5P, *cck6P, *cck7P, *cck8P;	
	FormPtr frmP;

	fld1P = (FieldType *)GetObjectPtr(MiscOptionsDebugLevelField);
	
	cck1P = (ControlType *)GetObjectPtr(MiscOptionsVibratorCheckbox);
	cck2P = (ControlType *)GetObjectPtr(MiscOptionsNoAutoOffCheckbox);
	cck3P = (ControlType *)GetObjectPtr(MiscOptionsStdPaletteCheckbox);
	cck4P = (ControlType *)GetObjectPtr(MiscOptionsDebugCheckbox);
	cck5P = (ControlType *)GetObjectPtr(MiscOptionsWriteIniCheckbox);
	cck6P = (ControlType *)GetObjectPtr(MiscOptionsAutoResetCheckbox);
	cck7P = (ControlType *)GetObjectPtr(MiscOptionsDemoCheckbox);
	cck8P = (ControlType *)GetObjectPtr(MiscOptionsFullscreenCheckbox);

	frmP = FrmGetActiveForm();

	if (FldGetTextLength(fld1P) == 0 && CtlGetValue(cck4P) == 1) {
		FrmCustomAlert(FrmWarnAlert,"You must specified a debug level.",0,0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MiscOptionsDebugLevelField));
		return;
	}

	gPrefs->vibrator = CtlGetValue(cck1P);
	gPrefs->autoOff = !CtlGetValue(cck2P);
	gPrefs->stdPalette = CtlGetValue(cck3P);
	gPrefs->debug = CtlGetValue(cck4P);
	gPrefs->saveConfig = CtlGetValue(cck5P);
	gPrefs->autoReset = CtlGetValue(cck6P);
	gPrefs->demoMode = CtlGetValue(cck7P);
	gPrefs->fullscreen = CtlGetValue(cck8P);

	gPrefs->debugLevel = StrAToI(FldGetTextPtr(fld1P));
	
	FrmReturnToMain();
}

static void MiscOptionsFormInit() {

	FieldType *fld1P;
	FormPtr frmP;

	Char *levelP;
	MemHandle levelH;

	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsVibratorCheckbox), gPrefs->vibrator);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsNoAutoOffCheckbox), !gPrefs->autoOff);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsStdPaletteCheckbox), gPrefs->stdPalette);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsAutoResetCheckbox), gPrefs->autoReset);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsDebugCheckbox), gPrefs->debug);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsWriteIniCheckbox), gPrefs->saveConfig);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsDemoCheckbox), gPrefs->demoMode);
	CtlSetValue((ControlType *)GetObjectPtr(MiscOptionsFullscreenCheckbox), gPrefs->fullscreen);

	fld1P = (FieldType *)GetObjectPtr(MiscOptionsDebugLevelField);

	levelH = MemHandleNew(FldGetMaxChars(fld1P)+1);
	levelP = (Char *)MemHandleLock(levelH);
	StrIToA(levelP, gPrefs->debugLevel);
	MemHandleUnlock(levelH);

	FldSetTextHandle(fld1P, levelH);
	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);
}

static Boolean MiscOptionsFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			MiscOptionsFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MiscOptionsOKButton:
					MiscOptionsFormSave();
					break;

				case MiscOptionsCancelButton:
					FrmReturnToMain();
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}
///////////////////////////////////////////////////////////////////////
static void CardSlotCreateDirs() {
	if (gPrefs->card.volRefNum != sysInvalidRefNum) {
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Games");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Saved");
	}
}

static void CardSlotFromShowHideOptions() {
	ControlType *cck1P;
	FormPtr frmP = FrmGetActiveForm();

	cck1P = (ControlType *)GetObjectPtr(CardSlotMoveCheckbox);

	if (CtlGetValue(cck1P)) {
		FrmShowObject(frmP, FrmGetObjectIndex (frmP, CardSlotDeleteCheckbox));
		FrmShowObject(frmP, FrmGetObjectIndex (frmP, CardSlotConfirmCheckbox));
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex (frmP, CardSlotDeleteCheckbox));
		FrmHideObject(frmP, FrmGetObjectIndex (frmP, CardSlotConfirmCheckbox));
	}
}

static UInt16 CardSlotFormInit(Boolean display, Boolean bDraw) {
	Err err;
	UInt16 volRefNum;
	UInt32 volIterator = vfsIteratorStart;
	UInt8 counter = 0;
	UInt32 other = 1;

	MemHandle cards = NULL;
	CardInfoType *cardsInfo;

	while (volIterator != vfsIteratorStop) {
		err = VFSVolumeEnumerate(&volRefNum, &volIterator);

		if (!err) {
			Char labelP[expCardInfoStringMaxLen+1];
			err = VFSVolumeGetLabel(volRefNum, labelP, expCardInfoStringMaxLen+1);

			if (err || StrLen(labelP) == 0) {	// if no label try to retreive card type
				VolumeInfoType volInfo;
				err = VFSVolumeInfo(volRefNum, &volInfo);
				
				if (!err) {
					ExpCardInfoType info;
					err = ExpCardInfo(volInfo.slotRefNum, &info);
					StrCopy(labelP, info.deviceClassStr);
				}
				
				if (err)	// if err default name
					StrPrintF(labelP,"Other Card %ld", other++);
			}

			if (!cards)
				cards = MemHandleNew(sizeof(CardInfoType));
			else
				MemHandleResize(cards, MemHandleSize(cards) + sizeof(CardInfoType));
				
			cardsInfo = (CardInfoType *)MemHandleLock(cards);
			cardsInfo[counter].volRefNum = volRefNum;
			StrCopy(cardsInfo[counter].nameP, labelP);
			MemHandleUnlock(cards);
			counter++;
		}
	}

	if (display) {
		FormPtr frmP;
		ListPtr listP;
		ControlType *cck1P, *cck2P, *cck3P;
		UInt16 index;
		Int16 selected = -1;
		
		CardInfoType *cardsInfo;
		MemHandle items = NULL;

		listP = (ListType *)GetObjectPtr(CardSlotSlotList);
		cck1P = (ControlType *)GetObjectPtr(CardSlotMoveCheckbox);
		cck2P = (ControlType *)GetObjectPtr(CardSlotDeleteCheckbox);
		cck3P = (ControlType *)GetObjectPtr(CardSlotConfirmCheckbox);

		if (counter > 0) {
			cardsInfo = (CardInfoType *)MemHandleLock(cards);

			for (index = 0; index < counter; index++) {
				if (!items)
					items = MemHandleNew(sizeof(Char *));
				else
					MemHandleResize(items, MemHandleSize(items) + sizeof(Char *));

				itemsText = (Char **)MemHandleLock(items);
				itemsText[index] = cardsInfo[index].nameP;
				MemHandleUnlock(items);
				
				if (cardsInfo[index].volRefNum == gPrefs->card.volRefNum)
					selected = index;
			}

			itemsText = (Char **)MemHandleLock(items);
			LstSetListChoices (listP, itemsText, counter);
			LstSetSelection(listP, selected);
			// save globals and set list
			itemsText = (Char **)MemHandleLock(items);
			itemsList = (void *)cardsInfo;
			itemsType = ITEM_TYPE_CARD;

		} else {
			LstSetListChoices(listP, NULL, 0);
			// save globals and set list
			itemsText = NULL;
			itemsList = NULL;
			itemsType = ITEM_TYPE_CARD;
		}

		// bDraw = true -> draw whole from
		// bDraw = false -> redraw list
		if (bDraw) {
			CtlSetValue(cck1P, gPrefs->card.moveDB);
			CtlSetValue(cck2P, gPrefs->card.deleteDB);
			CtlSetValue(cck3P, gPrefs->card.confirmMoveDB);
			CardSlotFromShowHideOptions();
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);

		} else {
			WinScreenLock(winLockCopy);
			LstDrawList(listP);
			WinScreenUnlock();
		}
	} else {	// if !display, we just want to retreive an avaliable card
		if (counter > 0) {
			UInt16 volRefNum;
			cardsInfo = (CardInfoType *)MemHandleLock(cards);
			volRefNum =  cardsInfo[0].volRefNum;	// return the first volref
			MemHandleUnlock(cards);
			MemHandleFree(cards);
			return volRefNum;
		}
	}

	return sysInvalidRefNum; // default
}

static void CardSlotFormExit(Boolean bSave) {
	MemHandle cards;
	MemHandle items;
	CardInfoType *cardsInfo;
	UInt16 updateCode = frmRedrawUpdateMS;

	if (itemsText && itemsList) {
		cardsInfo = (CardInfoType *)itemsList;
		cards = MemPtrRecoverHandle(cardsInfo);
		items = MemPtrRecoverHandle(itemsText);
	
		itemsText = NULL;
		itemsList = NULL;
	} else {
		cards = NULL;
		items = NULL;
	}
	itemsType = ITEM_TYPE_UNKNOWN;

	if (bSave) {
		ListType *listP;
		ControlType *cck1P, *cck2P, *cck3P;
		Int16 selected;

		listP = (ListType *)GetObjectPtr(CardSlotSlotList);
		cck1P = (ControlType *)GetObjectPtr(CardSlotMoveCheckbox);
		cck2P = (ControlType *)GetObjectPtr(CardSlotDeleteCheckbox);
		cck3P = (ControlType *)GetObjectPtr(CardSlotConfirmCheckbox);
		selected = LstGetSelection(listP);

		if (selected == -1) {
			gPrefs->card.volRefNum = sysInvalidRefNum;
		} else if (gPrefs->card.volRefNum != cardsInfo[selected].volRefNum) {
			updateCode = frmRedrawUpdateMSImport;
			gPrefs->card.volRefNum = cardsInfo[selected].volRefNum;
		}

		gPrefs->card.moveDB = CtlGetValue(cck1P);
		gPrefs->card.deleteDB = CtlGetValue(cck2P);
		gPrefs->card.confirmMoveDB = CtlGetValue(cck3P);
		CardSlotCreateDirs();
	}

	FrmReturnToMain(updateCode);

	if (items && cards) {
		MemHandleUnlock(items);
		MemHandleUnlock(cards);
		MemHandleFree(items);
		MemHandleFree(cards);
	}
}

static void CardSlotFormUpdate() {
	if (itemsType == ITEM_TYPE_CARD) {
		if (itemsText && itemsList) {
			MemHandle cards;
			MemHandle items;
			ListType *listP;

			listP = (ListType *)GetObjectPtr(CardSlotSlotList);
			cards = MemPtrRecoverHandle(itemsList);
			items = MemPtrRecoverHandle(itemsText);

			itemsText = NULL;
			itemsList = NULL;
			itemsType = ITEM_TYPE_UNKNOWN;

			MemHandleUnlock(items);
			MemHandleUnlock(cards);
			MemHandleFree(items);
			MemHandleFree(cards);
		}
		CardSlotFormInit(true, false);
	}
}

static Boolean CardSlotFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
	
		case frmOpenEvent:
			CardSlotFormInit(true, true);
			handled = true;
			break;

		case frmCloseEvent:
			CardSlotFormExit(false);
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case CardSlotOkButton:
					CardSlotFormExit(true);
					break;

				case CardSlotCancelButton:
					CardSlotFormExit(false);
					break;
				
				case CardSlotMoveCheckbox:
					CardSlotFromShowHideOptions();
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}

static UInt16 parseCards() {
	UInt16 volRefNum = CardSlotFormInit(false, false);
	CardSlotFormExit(false);
	
	return volRefNum;
}

/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormInit()
{
	SknApplySkin();
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/


static Int16 SkinsFormCompare(SkinInfoType *a, SkinInfoType *b, SortRecordInfoPtr, SortRecordInfoPtr, MemHandle) {
	return StrCompare(a->nameP, b->nameP);
}

static void SkinsFormInit(Boolean bDraw) {
	MemHandle skins = NULL;
	SkinInfoType *skinsInfo;
	UInt16 numSkins = 0;

	FormPtr frmP;
	ListType *listP;
	MemHandle items = NULL;
	ControlType *cck1P;
	DmSearchStateType stateInfo;
	UInt16 cardNo;
	LocalID dbID;

	Err errInfo;
	Char nameP[32];

	itemsText = NULL;
	
	// parse and save skins
	Err err = DmGetNextDatabaseByTypeCreator(true, &stateInfo, 'skin', appFileCreator, false, &cardNo, &dbID);
	while (!err && dbID) {
		errInfo = DmDatabaseInfo (cardNo, dbID, nameP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		if (!errInfo)
		{
			if (!skins)
				skins = MemHandleNew(sizeof(SkinInfoType));
			else
				MemHandleResize(skins, MemHandleSize(skins) + sizeof(SkinInfoType));
			
			skinsInfo = (SkinInfoType *)MemHandleLock(skins);
			StrCopy(skinsInfo[numSkins].nameP, nameP);
			skinsInfo[numSkins].cardNo = cardNo;
			skinsInfo[numSkins].dbID = dbID;
			MemHandleUnlock(skins);
			numSkins++;
		}		
		err = DmGetNextDatabaseByTypeCreator(false, &stateInfo, 'skin', appFileCreator, false, &cardNo, &dbID);
	}

	Int16 selected = -1;

	cck1P = (ControlType *)GetObjectPtr(SkinsSoundClickCheckbox);
	CtlSetValue(cck1P, gPrefs->soundClick);

	listP = (ListType *)GetObjectPtr(SkinsSkinList);
	skinsInfo = (SkinInfoType *)MemHandleLock(skins);
	SysQSort(skinsInfo, numSkins, sizeof(SkinInfoType), (CmpFuncPtr)SkinsFormCompare, 0);

	// create itemsText (TODO: create a custom draw function)
	for (UInt16 index=0; index < numSkins; index++)
	{
		if (!items)
			items = MemHandleNew(sizeof(Char *));
		else
			MemHandleResize(items, MemHandleSize(items) + sizeof(Char *));

		itemsText = (Char **)MemHandleLock(items);
		itemsText[index] = skinsInfo[index].nameP;
		MemHandleUnlock(items);
		
		if (	gPrefs->skin.cardNo == skinsInfo[index].cardNo &&
				gPrefs->skin.dbID == skinsInfo[index].dbID &&
				StrCompare(gPrefs->skin.nameP, skinsInfo[index].nameP) == 0)
			selected = index;
	}
	// save globals and set list
	itemsText = (Char **)MemHandleLock(items);
	itemsList = (void *)skinsInfo;
	itemsType = ITEM_TYPE_SKIN;

	LstSetListChoices (listP, itemsText, numSkins);
	LstSetSelection(listP, selected);

	// bDraw = true -> draw whole from
	// bDraw = false -> redraw list
	if (bDraw) {
		frmP = FrmGetActiveForm();
		FrmDrawForm(frmP);
	} else {
		WinScreenLock(winLockCopy);
		LstDrawList(listP);
		WinScreenUnlock();
//		LstSetSelection(listP, 0);
	}
}

static void SkinsFormExit(Boolean bSave) {
	MemHandle skins;
	MemHandle items;
	SkinInfoType *skinsInfo;

	ListType *listP;
	Int16 selected;

	listP = (ListType *)GetObjectPtr(SkinsSkinList);
	selected = LstGetSelection(listP);
	
	if (bSave && selected == -1)	{	// may never occured...
		FrmCustomAlert(FrmWarnAlert, "You didn't select a skin.", 0, 0);
		return;
	}

	skinsInfo = (SkinInfoType *)itemsList;
	skins = MemPtrRecoverHandle(skinsInfo);
	items = MemPtrRecoverHandle(itemsText);
	
	itemsText = NULL;
	itemsList = NULL;
	itemsType = ITEM_TYPE_UNKNOWN;

	if (bSave) {
		ControlType *cck1P;

		StrCopy(gPrefs->skin.nameP, skinsInfo[selected].nameP);
		gPrefs->skin.cardNo = skinsInfo[selected].cardNo;
		gPrefs->skin.dbID =  skinsInfo[selected].dbID;

		cck1P = (ControlType *)GetObjectPtr(SkinsSoundClickCheckbox);
		gPrefs->soundClick = CtlGetValue(cck1P);
	}

	FrmReturnToMain();

	MemHandleUnlock(items);
	MemHandleUnlock(skins);
	MemHandleFree(items);
	MemHandleFree(skins);

	if (bSave)
		SknApplySkin();
}

static void SkinsFormBeam() {
	SkinInfoType *skinsInfo;

	ListType *listP;
	Int16 selected;
	Err err;

	listP = (ListType *)GetObjectPtr(SkinsSkinList);
	selected = LstGetSelection(listP);

	if (selected == -1)	{	// may never occured...
		FrmCustomAlert(FrmWarnAlert, "You didn't select a skin.", 0, 0);
		return;
	}

	skinsInfo = (SkinInfoType *)itemsList;
	err = SendDatabase(0, skinsInfo[selected].dbID, "skin.pdb", "\nScummVM Skin");

//	if (err)
//		FrmCustomAlert(FrmErrorAlert, "Unable to beam this skin.",0,0);
}

static void SkinsFormDelete() {
	MemHandle skins;
	MemHandle items;
	SkinInfoType *skinsInfo;

	ListType *listP;
	Int16 selected;

	listP = (ListType *)GetObjectPtr(SkinsSkinList);
	selected = LstGetSelection(listP);

	if (selected == -1)	{	// may never occured...
		FrmCustomAlert(FrmInfoAlert, "You didn't select a skin.", 0, 0);
		return;
	}

	skinsInfo = (SkinInfoType *)itemsList;
	skins = MemPtrRecoverHandle(skinsInfo);
	items = MemPtrRecoverHandle(itemsText);

	if (	gPrefs->skin.cardNo == skinsInfo[selected].cardNo &&
			gPrefs->skin.dbID == skinsInfo[selected].dbID &&
			StrCompare(gPrefs->skin.nameP, skinsInfo[selected].nameP) == 0) {
			FrmCustomAlert(FrmInfoAlert, "You cannot delete the active skin.",0,0);
			return;

	} else {
		Err err = DmDeleteDatabase(0, skinsInfo[selected].dbID);
		if (!err) {

			itemsText = NULL;
			itemsList = NULL;
			itemsType = ITEM_TYPE_UNKNOWN;

			MemHandleUnlock(items);
			MemHandleUnlock(skins);
			MemHandleFree(items);
			MemHandleFree(skins);

			SkinsFormInit(false);
		} else {
			FrmCustomAlert(FrmErrorAlert, "Skin deletion failed.",0,0);
		}
	}

}
static Boolean SkinsFormHandleEvent(EventPtr eventP) {
	Boolean handled = false;

	switch (eventP->eType) {
	
		case frmOpenEvent:
			SkinsFormInit(true);
			handled = true;
			break;

		case frmCloseEvent:
			SkinsFormExit(false);
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case SkinsOKButton:
					SkinsFormExit(true);
					break;

				case SkinsCancelButton:
					SkinsFormExit(false);
					break;
					
				case SkinsBeamButton:
					SkinsFormBeam();
					break;
				
				case SkinsDeleteButton:
					SkinsFormDelete();
					break;
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}


static void MainFormAbout() {
	FormPtr frmP;
	FormLabelType *versionP;

	frmP = FrmInitForm(AboutForm);
	versionP = FrmNewLabel(&frmP, 1111, gScummVMVersion, 4, 142, stdFont);
	FrmDoDialog (frmP);					// Display the About Box.
	FrmDeleteForm (frmP);
}

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	switch (command) {
		case MainGamesMemoryCards:
			FrmPopupForm(CardSlotForm);
			handled = true;
			break;

		case MainGamesViewMemory:
			FrmPopupForm(SystemInfoForm);
			handled = true;
			break;

		case MainGamesNewEdit:
			editMode = edtModeParams;
			FrmPopupForm(EditGameForm);
			handled = true;
			break;

		case MainGamesBeam:
			BeamMe();
			//if (BeamMe())
				//FrmCustomAlert(FrmErrorAlert,"Unable to beam ScummVM for PalmOS.",0,0);
			handled = true;
			break;

		case MainOptionsAbout:
 			MainFormAbout();
 			handled = true;
			break;
		
		case MainOptionsVolumeControl:
			FrmPopupForm(VolumeForm);
			handled = true;
			break;

		case MainOptionsMusic:
			FrmPopupForm(SoundForm);
			handled = true;
			break;
		
		case MainOptionsSkins:
			FrmPopupForm(SkinsForm);
			handled = true;
			break;

		case MainOptionsMisc:
			FrmPopupForm(MiscOptionsForm);
			handled = true;
			break;
		}

	MenuEraseStatus(0);
	return handled;
}


static void SavePrefs() {
	if (gPrefs) {
		PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, gPrefs, sizeof (GlobalsPreferenceType), true);
		MemPtrFree(gPrefs);
		gPrefs = NULL;
	}
}

static Boolean CheckVibratorExists() {
	UInt32 romVersion;
	Err err;
	Boolean exists = false;

	err = FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (!err) {
		if (romVersion >= sysMakeROMVersion(4,0,0,sysROMStageRelease,0)) {
			Boolean active = false;
			err = HwrVibrateAttributes(0, kHwrVibrateActive, &active);
			exists = (!err) ? true : exists;
		}
	}

	return exists;
}

// need to move this on a .h file
#define sonySysFileCSystem      'SsYs'  /* Sony overall System */
#define sonySysFtrCreatorSystem     sonySysFileCSystem

#define sonySysFtrNumSystemBase         10000
#define sonySysFtrNumSystemAOutSndStateOnHandlerP   (sonySysFtrNumSystemBase + 4)
#define sonySysFtrNumSystemAOutSndStateOffHandlerP  (sonySysFtrNumSystemBase + 5)

typedef void (*sndStateOnType)(UInt8 /* kind */, UInt8 /* L volume 0-31 */, UInt8 /* R volume 0-31 */);
typedef void (*sndStateOffType)(UInt8 /* kind */);

/* kind */
#define aOutSndKindSp       (0) /* Speaker volume */
#define aOutSndKindHp       (2) /* HeadPhone volume */
////////////////////////////////////////////////////////////
#define MAX_ARG	25

static Boolean checkPath(const Char *pathP) {
	FileRef *tmpRef;

	if (!(tmpRef = fopen(pathP, "r"))) {
		return false;
	} else {
		fclose(tmpRef);
		return true;
	}
}

static void AddArg(Char **argvP, const Char *argP, const Char *parmP, UInt8 *countArgP)
{
	if (argP)
	{
		UInt16 len2 = 0;
		UInt16 len1 = StrLen(argP);

		if (len1 > 0)
		{	
			if (parmP)
				len2 = StrLen(parmP);

			(*countArgP)++;
			*argvP = (Char *)MemPtrNew(len1 + len2 + 1);
			StrCopy(*argvP, argP);
			
			if (parmP)
				StrCat(*argvP, parmP);
		}
	}
}

static void StartScummVM() {
	Char *argvP[MAX_ARG];
	UInt8 argc	= 0;
	UInt8 count;

	Boolean autoOff;
	UInt16 autoOffDelay;
	Boolean debug;
	UInt16 musicDriver = sysInvalidRefNum; // for launch call

	bStartScumm = false;	//
	UInt16 index = GamGetSelected();

	for(count = 0; count < MAX_ARG; count++)
		argvP[count] = NULL;

	if (index != dmMaxRecordIndex) {
		Char pathP[256];
		Char num[4];
		MemHandle recordH;
		GameInfoType *gameInfoP;

		recordH = DmQueryRecord(gameDB,index);
		gameInfoP = (GameInfoType *)MemHandleLock(recordH);

		// build path
		StrCopy(pathP,"/Palm/Programs/ScummVM/Games/");
		if (gameInfoP->pathP[0] == '/')
			StrCopy(pathP,gameInfoP->pathP);
		else
			StrCat(pathP,gameInfoP->pathP);
		
		// path exists ?
/*		if (!checkPath(pathP)) {
			MemHandleUnlock(recordH);
			FrmCustomAlert(FrmErrorAlert,"The specified path was not found !",0,0);
			return;
		}
*/
		AddArg(&argvP[argc], "ScummVM", NULL, &argc);

		// save scummvm.ini ?
		if (gPrefs->saveConfig)
			AddArg(&argvP[argc], "-w", NULL, &argc);

		// path
		AddArg(&argvP[argc], "-p", pathP, &argc);

		// language
		if (gameInfoP->language > 0) {
			const Char *lang = "en\0de\0fr\0it\0p\0es\0jp\0z\0kr\0hb\0";
			AddArg(&argvP[argc], "-q", (lang + (gameInfoP->language - 1) * 3), &argc);
		}

		// fullscreen ?
		if (gPrefs->fullscreen) {
			AddArg(&argvP[argc], "-f", NULL, &argc);
		}

		// gfx mode
		switch (gameInfoP->gfxMode)
		{
			case 1:
				AddArg(&argvP[argc], "-g", "flipping", &argc);
				break;
			case 2:
				AddArg(&argvP[argc], "-g", "dbuffer", &argc);
				break;
			case 3:
				AddArg(&argvP[argc], "-g", "wide", &argc);
				break;
			default:
				AddArg(&argvP[argc], "-g", "normal", &argc);
				break;
		}

		// load state
		if (gameInfoP->autoLoad) {
			StrIToA(num, gameInfoP->loadSlot);
			AddArg(&argvP[argc], "-x", num, &argc);
		}
		// start in room
		if (gameInfoP->autoRoom) {
			StrIToA(num, gameInfoP->roomNum);
			AddArg(&argvP[argc], "-b", num, &argc);
		}
		// not a PC version
		if (gameInfoP->setPlatform) {
			switch (gPrefs->sound.driver) {
				case 0:
					AddArg(&argvP[argc], "--platform=", "amiga", &argc);
					break;
				case 1:
					AddArg(&argvP[argc], "--platform=", "atari-st", &argc);
					break;
				case 2:
					AddArg(&argvP[argc], "--platform=", "machintosh", &argc);
					break;
			}		
		}

		// subtitles
		if (!gameInfoP->subtitles) {
			AddArg(&argvP[argc], "-n", NULL, &argc);
		}
		// talk speed
		if (gameInfoP->talkSpeed) {
			StrIToA(num, gameInfoP->talkValue);
			AddArg(&argvP[argc], "-y", num, &argc);
		}
		// debug level
		debug = gPrefs->debug;
		if (gPrefs->debug) {
			StrIToA(num, gPrefs->debugLevel);
			AddArg(&argvP[argc], "-d", num, &argc);
		}
		// demo mode ?
		if (gPrefs->demoMode)
			AddArg(&argvP[argc], "--demo-mode", NULL, &argc);

		// multi midi ?
		if (gPrefs->sound.multiMidi)
			AddArg(&argvP[argc], "--multi-midi", NULL, &argc);

		// music driver
		musicDriver = gPrefs->sound.music;
		if (musicDriver) {
			switch (gPrefs->sound.driver) {
				case 0:	// NULL
					AddArg(&argvP[argc], "-e", "null", &argc);
					break;
				case 1:	// yamaha Pa1
					AddArg(&argvP[argc], "-e", "ypa1", &argc);
					break;
			}		
		}
		else	// NULL as default
			AddArg(&argvP[argc], "-e", "null", &argc);

		// music tempo
		StrIToA(num, gPrefs->sound.tempo);
		AddArg(&argvP[argc], "-t", num, &argc);

		// volume control
		StrIToA(num, gPrefs->volume.master);
		AddArg(&argvP[argc], "-o", num, &argc);
		StrIToA(num, gPrefs->volume.sfx);
		AddArg(&argvP[argc], "-s", num, &argc);
		StrIToA(num, gPrefs->volume.music);
		AddArg(&argvP[argc], "-m", num, &argc);

		// game name
		AddArg(&argvP[argc], gameInfoP->gameP, NULL, &argc);

		MemHandleUnlock(recordH);
	}

	gVars->skinSet = false;
	GamCloseDatabase(false);
	FrmCloseAllForms();

	autoOff = gPrefs->autoOff;
	if (!autoOff) {
		autoOffDelay = SysSetAutoOffTime(0);
		EvtResetAutoOffTimer();
	}

	// gVars values
	//gVars->HRrefNum defined in checkHRmode on Cli� OS4
	//gVars->logFile defined bellow, must be defined only if debug option is checked
	gVars->screenLocked = false;
	gVars->volRefNum = gPrefs->card.volRefNum;
	gVars->vibrator = gPrefs->vibrator;
	gVars->stdPalette = gPrefs->stdPalette;
	gVars->autoReset = gPrefs->autoReset;
	gVars->music.MP3 = gPrefs->sound.MP3;
	gVars->music.setDefaultTrackLength = gPrefs->sound.setDefaultTrackLength;
	gVars->music.defaultTrackLength = gPrefs->sound.defaultTrackLength;
	gVars->music.firstTrack = gPrefs->sound.firstTrack;

	// TODO : support tapwave rumble
	if (gVars->vibrator)
	{
		if (CheckVibratorExists()) {
			UInt16 cycle	= (SysTicksPerSecond())/2;
			UInt32 pattern	= 0xFF000000;
			UInt16 delay	= 1;
			UInt16 repeat	= 1;

			HwrVibrateAttributes(1, kHwrVibrateRate, &cycle);
			HwrVibrateAttributes(1, kHwrVibratePattern, &pattern);
			HwrVibrateAttributes(1, kHwrVibrateDelay, &delay);
			HwrVibrateAttributes(1, kHwrVibrateRepeatCount, &repeat);

		} else {
			gVars->vibrator = false;
		}
	}

	if (debug) {
		VFSFileDelete(gVars->volRefNum,"PALM/Programs/ScummVM/scumm.log");
		VFSFileCreate(gVars->volRefNum,"PALM/Programs/ScummVM/scumm.log");
		VFSFileOpen(gVars->volRefNum,"PALM/Programs/ScummVM/scumm.log",vfsModeWrite, &gVars->logFile);
	}

	void *sndStateOnFuncP = NULL,
		 *sndStateOffFuncP = NULL;

	if (musicDriver == 1 || musicDriver == sysInvalidRefNum) {

		Pa1Lib_Open();

		// Don't work on T4xx and T6xx series ?
		FtrGet(sonySysFtrCreatorSystem, sonySysFtrNumSystemAOutSndStateOnHandlerP, (UInt32*) &sndStateOnFuncP);
		FtrGet(sonySysFtrCreatorSystem, sonySysFtrNumSystemAOutSndStateOffHandlerP, (UInt32*) &sndStateOffFuncP);

		if (sndStateOnFuncP && sndStateOffFuncP) {
			((sndStateOnType)(sndStateOnFuncP))(aOutSndKindSp, gPrefs->volume.headphone, gPrefs->volume.headphone);
			((sndStateOnType)(sndStateOnFuncP))(aOutSndKindHp, gPrefs->volume.speaker, gPrefs->volume.speaker);

		}

		Pa1Lib_devHpVolume(gPrefs->volume.headphone, gPrefs->volume.headphone);
		Pa1Lib_devSpVolume(gPrefs->volume.speaker);
	}

	SavePrefs();	// free globals pref memory
	GlbOpen();
	main(argc, argvP);
	GlbClose();

	if (musicDriver == 1 || musicDriver == sysInvalidRefNum) {
		if (sndStateOnFuncP && sndStateOffFuncP) {
			((sndStateOffType)(sndStateOffFuncP))(aOutSndKindSp);
			((sndStateOffType)(sndStateOffFuncP))(aOutSndKindHp);
		}

		Pa1Lib_Close();
	}

	if (debug)
		VFSFileClose(gVars->logFile);

	for(count = 0; count < MAX_ARG; count++)
		if (argvP[count])
			MemPtrFree(argvP[count]);
	
	if (gVars->vibrator) {
		Boolean active = false;
		HwrVibrateAttributes(1, kHwrVibrateActive, &active);
	}

	if (!autoOff) {
		SysSetAutoOffTime(autoOffDelay);SystemPreferencesChoice
		EvtResetAutoOffTimer();
	}
}

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PenDownRepeat() {
	Coord x,y;
	Boolean penDown, handled = false;
	EvtGetPen(&x, &y, &penDown);

	if (penDown && sknLastOn) {
		RectangleType r;
		DmOpenRef skinDBP;
	
		skinDBP = SknOpenSkin();
		SknGetObjectBounds(skinDBP, sknLastOn, &r);

		if (RctPtInRectangle(x*2, y*2, &r)) {
			if (SknGetState(skinDBP, sknLastOn) != sknStateSelected) {
				SknSetState(skinDBP, sknLastOn, sknStateSelected);
				SknShowObject(skinDBP, sknLastOn);
			}
			
			switch (sknLastOn) {
				case skinSliderUpArrow:
				case skinSliderDownArrow:
					handled = SknProcessArrowAction(sknLastOn);
					break;
			}
			
		} else {
			if (SknGetState(skinDBP, sknLastOn) != sknStateNormal) {
				SknSetState(skinDBP, sknLastOn, sknStateNormal);
				SknShowObject(skinDBP, sknLastOn);
			}
		}

		SknCloseSkin(skinDBP);
	}
	
	return handled;
}


static Boolean MainFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP;
	Coord x,y;
	DmOpenRef skinDBP;
	
	switch (eventP->eType) {
		case frmUpdateEvent:
		frmP = FrmGetFormPtr(MainForm);
			if (gPrefs->card.volRefNum != sysInvalidRefNum)
				FrmShowObject(frmP, FrmGetObjectIndex (frmP, MainMSBitMap));
			else
				FrmShowObject(frmP, FrmGetObjectIndex (frmP, MainMSNoneBitMap));
			if (eventP->data.frmUpdate.updateCode == frmRedrawUpdateMSImport) {
				GamImportDatabase();
				SknUpdateList();
			}
			handled = true;
			break;

		case winDisplayChangedEvent:
			if (gVars) {
				if (gVars->skinSet)
					SknApplySkin();
			}
			break;

		case menuEvent:
			handled = MainFormDoCommand(eventP->data.menu.itemID);
			break;

		case frmOpenEvent:
			MainFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainCardsButton:
					//gPrefs->card.volRefNum = parseCards(true);
					FrmPopupForm(CardSlotForm);
					break;
			
				case MainAboutButton:
					MainFormAbout();
					break;
				
//				case MainListTypeSelTrigger:
//					FrmList(eventP, MainListTypeList);
//					break;
			}
			handled = true;
			break;

		case penUpEvent:
			x = eventP->screenX;
			y = eventP->screenY;
			lastIndex = dmMaxRecordIndex;	// enable select/unselect list item
			
			if (sknLastOn != skinButtonNone) {
				RectangleType r;
				skinDBP = SknOpenSkin();
				SknGetObjectBounds(skinDBP, sknLastOn, &r);
				SknSetState(skinDBP, sknLastOn, sknStateNormal);
				SknShowObject(skinDBP, sknLastOn);
				SknCloseSkin(skinDBP);
				
				if (RctPtInRectangle(x*2, y*2, &r)) {
					switch (sknLastOn) {
						case skinButtonGameAdd:
							editMode = edtModeAdd;
							FrmPopupForm(EditGameForm);
							handled = true;
							break;

						case skinButtonGameEdit:
						case skinButtonGameParams:
							editMode = edtModeParams;
							FrmPopupForm(EditGameForm);
							handled = true;
							break;

						case skinButtonGameStart:
							if (gPrefs->card.volRefNum == sysInvalidRefNum)
								FrmCustomAlert(FrmWarnAlert,"Please select/insert a memory card.", 0, 0);
							else
								bStartScumm = true;
							handled = true;
							break;

						case skinButtonGameDelete:
							EditGameFormDelete(true);
							break;
					}
				}
				sknLastOn = skinButtonNone;
			}
			break;

		case penDownEvent:
		case penMoveEvent:
			if (sknLastOn == skinButtonNone) {
				x = eventP->screenX;
				y = eventP->screenY;
				skinDBP = SknOpenSkin();

				switch (sknLastOn = SknCheckClick(skinDBP, x,y)) {
					case skinButtonNone:
						break;
					case skinSliderUpArrow:
					case skinSliderDownArrow:
					case skinButtonGameAdd:
					case skinButtonGameEdit:
					case skinButtonGameParams:
					case skinButtonGameStart:
					case skinButtonGameDelete:
						SknSetState(skinDBP, sknLastOn, sknStateSelected);
						SknShowObject(skinDBP, sknLastOn);				
						if (gPrefs->soundClick)
							SndPlaySystemSound(sndClick);
						handled = true;
						break;
					default:
						FrmCustomAlert(FrmWarnAlert,"Unknown button !",0,0);
				}
				SknCloseSkin(skinDBP);
				SknSelect(x, y);
			}
			break;
		
		default:
			handled = PenDownRepeat();
	}

	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent)
		{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
			{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

			case SkinsForm:
				FrmSetEventHandler(frmP, SkinsFormHandleEvent);
				break;

			case EditGameForm:
				FrmSetEventHandler(frmP, EditGameFormHandleEvent);
				break;

			case MiscOptionsForm:
				FrmSetEventHandler(frmP, MiscOptionsFormHandleEvent);
				break;

			case VolumeForm:
				FrmSetEventHandler(frmP, VolumeFormHandleEvent);
				break;
			
			case SoundForm:
				FrmSetEventHandler(frmP, SoundFormHandleEvent);
				break;

			case SystemInfoForm:
				FrmSetEventHandler(frmP, SystemInfoFormHandleEvent);
				break;

			case CardSlotForm:
				FrmSetEventHandler(frmP, CardSlotFormHandleEvent);
				break;

			default:
//				ErrFatalDisplay("Invalid Form Load Event");
				break;

			}
		return true;
		}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do {
		EvtGetEvent(&event, evtNoWait);

		if(bStartScumm)
			StartScummVM();

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStartCheckHRmode()
{
	SonySysFtrSysInfoP sonySysFtrSysInfoP;
	UInt32 version;
	Err error = errNone;

	// test if sonyHR is present
	if (!(error = FtrGet(sonySysFtrCreator, sonySysFtrNumSysInfoP, (UInt32*)&sonySysFtrSysInfoP))) {
		if (sonySysFtrSysInfoP->libr & sonySysFtrSysInfoLibrHR) {		// HR available

			if ((error = SysLibFind(sonySysLibNameHR, &gVars->HRrefNum)))
				if (error == sysErrLibNotFound)							// couldn't find lib
					error = SysLibLoad( 'libr', sonySysFileCHRLib, &gVars->HRrefNum);

			if (!error) {	// Now we can use HR lib. Executes Open library.
				error = HROpen(gVars->HRrefNum);
				gVars->options |= optIsClieDevice;
			}
		}
	}
	// if not Hi-Density ?
	if (error && !(error = FtrGet(sysFtrCreator, sysFtrNumWinVersion, &version))) {
		gVars->HRrefNum = sysInvalidRefNum;	// Not sony HR
		gVars->options &= ~optIsClieDevice;

		if (version<4)
			error = 1;
	}

	if (!error) { // Not, error processing
		UInt32 width, height, depth;
		Boolean color;

		width = hrWidth;
		height= hrHeight;
		depth = 8;
		color = true;

		if (gVars->HRrefNum != sysInvalidRefNum)
			error = HRWinScreenMode (gVars->HRrefNum, winScreenModeSet, &width, &height, &depth, &color );
		else
			error = WinScreenMode (winScreenModeSet, &width, &height, &depth, &color );

		// high-resolution mode entered from here if no error
		if (error != errNone)
			FrmCustomAlert(FrmErrorAlert,"Your device doesn't seems to support 256 colors mode.",0,0);

		// check for 16bit mode
		if (!WinScreenMode(winScreenModeGetSupportedDepths, NULL, NULL, &depth, &color))
			gVars->options |= ((depth & 0x8000) ? optHas16BitMode : 0);
	}
	else
		FrmCustomAlert(FrmErrorAlert,"This device doesn't seems to support\nHi-Res mode.",0,0);

	return error;
}

static void AppStopHRMode() {
	if (gVars->HRrefNum != sysInvalidRefNum) {
			HRClose(gVars->HRrefNum);
			//SysLibRemove(gVars->HRrefNum);	// never call this !!
	}
}

static Err AppStartCheckNotify() {
	UInt32 romVersion;
	Err err;

	err = FtrGet(sysFtrCreator, sysFtrNumNotifyMgrVersion, &romVersion); 
	if (!err) {
		UInt16 cardNo;
		LocalID dbID;

		err = SysCurAppDatabase(&cardNo, &dbID);
		if (!err) {
			SysNotifyRegister(cardNo, dbID, sysNotifyVolumeMountedEvent, NULL, sysNotifyNormalPriority, NULL);
			SysNotifyRegister(cardNo, dbID, sysNotifyVolumeUnmountedEvent, NULL, sysNotifyNormalPriority, NULL);
			SysNotifyRegister(cardNo, dbID, sonySysNotifyMsaEnforceOpenEvent, NULL, sysNotifyNormalPriority, NULL);
			SysNotifyRegister(cardNo, dbID, sysNotifyDisplayResizedEvent, NULL, sysNotifyNormalPriority, NULL);
		}
	}

	return err;
}

static Err AppStartLoadSkin() {
	Err err = errNone;

	// if skin defined, check if the db still exists
	if (gPrefs->skin.dbID) {
		UInt32 type, creator;

		// remember to check version for next revision of the skin
		err = DmDatabaseInfo (gPrefs->skin.cardNo, gPrefs->skin.dbID, gPrefs->skin.nameP, 0, 0, 0, 0, 0, 0, 0,0, &type, &creator);
		if (!err)
			if (type != 'skin' || creator != appFileCreator)
				err = dmErrInvalidParam;

		if (err)
			MemSet(&(gPrefs->skin),sizeof(SkinInfoType),0);
	}

	// No skin ? try to get the first one
	if (!gPrefs->skin.dbID) {
		DmSearchStateType stateInfo;

		err = DmGetNextDatabaseByTypeCreator(true, &stateInfo, 'skin', appFileCreator, false, &gPrefs->skin.cardNo, &gPrefs->skin.dbID);
		if (!err)
			err = DmDatabaseInfo (gPrefs->skin.cardNo, gPrefs->skin.dbID, gPrefs->skin.nameP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	if (err)
		FrmCustomAlert(FrmErrorAlert,"No skin found.\nPlease install a skin and restart ScummVM.",0,0);
	
	return err;
}

static Err AppStartCheckMathLib()
{
	Err error = errNone;

	if ((error = SysLibFind(MathLibName, &MathLibRef)))
		if (error == sysErrLibNotFound)									// couldn't find lib
			error = SysLibLoad( LibType, MathLibCreator, &MathLibRef);

	if (!error) {// Now we can use lib. Executes Open library.
		error = MathLibOpen(MathLibRef, MathLibVersion);
		if (error)
			FrmCustomAlert(FrmErrorAlert,"Can't open MathLib !",0,0);

	} else {
		FrmCustomAlert(FrmErrorAlert,"Can't find MathLib.\nPlease install it first.",0,0);
	}

	return error;
}

static void AppStopMathLib() {
	UInt16 useCount;

	if (MathLibRef != sysInvalidRefNum) {
		MathLibClose(MathLibRef, &useCount);
		
		if (!useCount)
			SysLibRemove(MathLibRef);
	}
}

static Err AppStartCheckScreenSize() {
	SonySysFtrSysInfoP sonySysFtrSysInfoP;
	Err error = errNone;
	UInt32 ftr = 0;

//	WinGetDisplayExtent(&gVars->screenWidth, &gVars->screenHeight);
	
//	gVars->screenWidth <<= 1;
//	gVars->screenHeight <<= 1;
	gVars->screenWidth = 320;
	gVars->screenHeight = 320;

	gVars->screenFullWidth = gVars->screenWidth;
	gVars->screenFullHeight = gVars->screenHeight;

	// Sony HiRes+
	if (!(error = FtrGet(sonySysFtrCreator, sonySysFtrNumSysInfoP, (UInt32*)&sonySysFtrSysInfoP))) {
		if (sonySysFtrSysInfoP->libr & sonySysFtrSysInfoLibrSilk) {

			if ((error = SysLibFind(sonySysLibNameSilk, &gVars->slkRefNum)))
				if (error == sysErrLibNotFound)	
					error = SysLibLoad( sonySysFileTSilkLib, sonySysFileCSilkLib, &gVars->slkRefNum);

			if (!error) {
				error = FtrGet(sonySysFtrCreator, sonySysFtrNumVskVersion, &gVars->slkVersion);
				// Get screen size
				if (error) {
					// v1 = NR
				 	error = SilkLibOpen(gVars->slkRefNum);
					if(!error) {
						gVars->slkVersion = vskVersionNum1;
						SilkLibEnableResize(gVars->slkRefNum);
						SilkLibResizeDispWin(gVars->slkRefNum, silkResizeMax);
						HRWinGetWindowExtent(gVars->HRrefNum, &gVars->screenFullWidth, &gVars->screenFullHeight);
						SilkLibResizeDispWin(gVars->slkRefNum, silkResizeNormal);
						SilkLibDisableResize(gVars->slkRefNum);
					}
				} else {
					// v2 = NX/NZ
					// v3 = UX
				 	error = VskOpen(gVars->slkRefNum);
					if(!error) {
						VskSetState(gVars->slkRefNum, vskStateEnable, (gVars->slkVersion == vskVersionNum2 ? vskResizeVertically : vskResizeHorizontally));
						gVars->options |= (gVars->slkVersion == vskVersionNum2 ? optIsLandscapeMode : 0);
						VskSetState(gVars->slkRefNum, vskStateResize, vskResizeNone);
						HRWinGetWindowExtent(gVars->HRrefNum, &gVars->screenFullWidth, &gVars->screenFullHeight);
						VskSetState(gVars->slkRefNum, vskStateResize, vskResizeMax);
						VskSetState(gVars->slkRefNum, vskStateEnable, vskResizeDisable);
					}
				}
			}
		}

		if (error)
			gVars->slkRefNum = sysInvalidRefNum;
		else
			gVars->options |= optHasWideMode;
	}

	// Tapwave Zodiac and other DIA API compatible devies, TODO : a better handler, read the doc
	if (error) {
		if (!(error = FtrGet(sysFtrCreator, sysFtrNumInputAreaFlags, &ftr))) {
			if (ftr & grfFtrInputAreaFlagLandscape) {
				UInt16 old = SysGetOrientation();
				error = SysSetOrientation(sysOrientationLandscape);
				error = (error) ? error : PINSetInputAreaState(pinInputAreaClosed);
				error = (error) ? error : StatHide();

				if (!error) {
					RectangleType r;
					WinGetBounds(WinGetDisplayWindow(), &r);
					gVars->screenFullWidth = r.extent.x << 1;
					gVars->screenFullHeight = r.extent.y << 1;
					gVars->options |= optHasWideMode|optIsLandscapeMode;
				}
				
				error = StatShow();
				error = PINSetInputAreaState(pinInputAreaOpen);
				error = SysSetOrientation(old);
			}
		}	
	}
	
	return error;
}

static void AppStopSilk() {
	if (gVars->slkRefNum != sysInvalidRefNum)
		SilkLibClose(gVars->slkRefNum);
}

static Err AppStart(void) {
	UInt16 dataSize, checkSize = 0;
	UInt32 ulProcessorType;
	UInt32 romVersion;
	Err error;

	// allocate global variables space
	dataSize = sizeof(GlobalsDataType);
	gVars = (GlobalsDataType *)MemPtrNew(dataSize);
	MemSet(gVars, dataSize, 0);

	gVars->indicator.on	= 255;
	gVars->indicator.off = 0;
	gVars->HRrefNum = sysInvalidRefNum;
	gVars->volRefNum = sysInvalidRefNum;
	gVars->slkRefNum = sysInvalidRefNum;
	gVars->skinSet = false;
	gVars->options = optNone;

	// OS5 ?
	if (!FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion))
		gVars->options |= ((romVersion >= kOS5Version) ? optIsOS5Device : 0);

	// ARM ?
 	if (!FtrGet(sysFileCSystem, sysFtrNumProcessorID, &ulProcessorType))
 		gVars->options |= ((sysFtrNumProcessorIsARM(ulProcessorType)) ? optIsARMDevice : 0);

	// allocate prefs space
	dataSize = sizeof(GlobalsPreferenceType);
	gPrefs	= (GlobalsPreferenceType *)MemPtrNew(dataSize);
	MemSet(gPrefs, dataSize, 0);

	// Read the saved preferences / saved-state information.
	if (PrefGetAppPreferences(appFileCreator, appPrefID, NULL, &checkSize, true) == noPreferenceFound || checkSize != dataSize) {
		// reset all elements
		MemSet(gPrefs, dataSize, 0);

		gPrefs->card.volRefNum = sysInvalidRefNum;

		gPrefs->autoOff = true;
		gPrefs->vibrator = CheckVibratorExists();
		gPrefs->debug = false;

		gPrefs->stdPalette = (gVars->options & optIsOS5Device);
		
		gPrefs->volume.speaker = 16;
		gPrefs->volume.headphone = 16;
		
		gPrefs->volume.master = 192;
		gPrefs->volume.music = 192;
		gPrefs->volume.sfx = 192;
		
		gPrefs->sound.tempo = 100;
		gPrefs->sound.defaultTrackLength = 10;
		gPrefs->sound.firstTrack = 1;
		
	} else {
		PrefGetAppPreferences(appFileCreator, appPrefID, gPrefs, &dataSize, true);
	}

	error = AppStartCheckMathLib();
	if (error) return (error);

	error = AppStartCheckHRmode();
	if (error) return (error);
	
	error = AppStartLoadSkin();
	if (error) return (error);

	if (gPrefs->card.volRefNum != sysInvalidRefNum) {	// if volref previously defined, check if it's a valid one
		VolumeInfoType volInfo;
		Err err = VFSVolumeInfo(gPrefs->card.volRefNum, &volInfo);
		if (err)
			gPrefs->card.volRefNum = sysInvalidRefNum;
	}
	else
		gPrefs->card.volRefNum = parseCards(); //parseCards(0);	// get first volref

	error = GamOpenDatabase();
	if (error) return (error);
	GamImportDatabase();

	AppStartCheckNotify(); // not fatal error if not avalaible
	AppStartCheckScreenSize();

	return error;
}

/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStopCheckNotify()
{
	UInt32 romVersion;
	Err err;
	
	err = FtrGet(sysFtrCreator, sysFtrNumNotifyMgrVersion, &romVersion); 
	if (!err) {
		UInt16 cardNo;
		LocalID dbID;

		err = SysCurAppDatabase(&cardNo, &dbID);
		if (!err) {
			SysNotifyUnregister(cardNo, dbID, sysNotifyVolumeUnmountedEvent, sysNotifyNormalPriority);
			SysNotifyUnregister(cardNo, dbID, sysNotifyVolumeMountedEvent, sysNotifyNormalPriority);
			// sonySysNotifyMsaEnforceOpenEvent
			SysNotifyUnregister(cardNo, dbID, sysNotifyDisplayResizedEvent, sysNotifyNormalPriority);
		}
	}
	
	return err;
}

static void AppStop(void) {
	// Close all the open forms.
	FrmCloseAllForms();
	WinEraseWindow();
	WinPalette(winPaletteSetToDefault, 0, 256, NULL);

	// Close and move Game list database
	GamCloseDatabase(false);

	// Write the saved preferences / saved-state information.  This data 
	// will saved during a HotSync backup.
	SavePrefs();

	// stop all
	AppStopSilk();
	AppStopCheckNotify();
	AppStopMathLib();
	AppStopHRMode();

	// reset if needed
	if (gVars) {
		Boolean autoReset = gVars->autoReset;
		MemPtrFree(gVars);
		if (autoReset)
			SysReset();
	}
}


/***********************************************************************
 *
 * FUNCTION:    ScummVMPalmMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppLaunchCmdNotify(UInt16 LaunchFlags, SysNotifyParamType * pData)
{
	switch (pData->notifyType)
	{
		case sysNotifyVolumeMountedEvent:
			pData->handled = true;	// don't switch

			if (gPrefs) {	// gPrefs exists ? so we are in the palm selector
				CardSlotFormUpdate(); // redraw card list if needed

				if (gPrefs->card.volRefNum == sysInvalidRefNum) {
					VFSAnyMountParamType *notifyDetailsP = (VFSAnyMountParamType *)pData->notifyDetailsP;
					gPrefs->card.volRefNum = notifyDetailsP->volRefNum;

					if (FrmGetFormPtr(MainForm) == FrmGetActiveForm())
						if (gPrefs->card.volRefNum != sysInvalidRefNum) {
							CardSlotCreateDirs();
							FrmUpdateForm(MainForm, frmRedrawUpdateMSImport);
						}
				}
			}
			break;
		
		case sysNotifyVolumeUnmountedEvent:
			if (gPrefs) {
				CardSlotFormUpdate();

				if (gPrefs->card.volRefNum == (UInt16)pData->notifyDetailsP) {
					gPrefs->card.volRefNum = sysInvalidRefNum;

					if (FrmGetFormPtr(MainForm) == FrmGetActiveForm())
						FrmUpdateForm(MainForm, frmRedrawUpdateMS);
				}
			}
			break;

		case sysNotifyDisplayResizedEvent:
			EventType ev;
			MemSet(&ev, sizeof(EventType), 0);
			ev.eType = (enum eventsEnum)winDisplayChangedEvent;
			EvtAddUniqueEventToQueue(&ev, 0, true);
			break;

		case sonySysNotifyMsaEnforceOpenEvent:
			// what am i supposed to do here ???
			break;
	}
}

static UInt32 ScummVMPalmMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (kOurMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
		{
		case sysAppLaunchCmdNotify:
			AppLaunchCmdNotify(launchFlags, (SysNotifyParamType *) cmdPBP);
			break;

		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				goto end;
				
			FrmGotoForm(MainForm);
			AppEventLoop();
end:
			AppStop();
			break;

		default:
			break;

		}

	return error;
}
/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	return ScummVMPalmMain(cmd, cmdPBP, launchFlags);
}
