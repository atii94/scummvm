#ifndef TASMRECOVER_DREAMGEN_STUBS_H__
#define TASMRECOVER_DREAMGEN_STUBS_H__

/* PLEASE DO NOT MODIFY THIS FILE. ALL CHANGES WILL BE LOST! LOOK FOR README FOR DETAILS */

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

#include "dreamweb/runtime.h"

#include "dreamweb/structs.h"
#include "dreamweb/dreambase.h"

namespace DreamGen {

static const uint16 offset_operand1 = 0x04ad;
static const uint16 offset_keys = 0x0443;
static const uint16 offset_openchangesize = 0x0405;
static const uint16 offset_money1poke = 0x04d7;
static const uint16 offset_rootdir = 0x04bb;
static const uint16 offset_comlist = 0x0407;
static const uint16 offset_money2poke = 0x04dc;
static const uint16 offset_speechfile = 0x04ea;
static const uint16 offset_commandline = 0x04f7;
static const uint16 kStartvars = 0;
static const uint16 kProgresspoints = 1;
static const uint16 kWatchon = 2;
static const uint16 kShadeson = 3;
static const uint16 kSecondcount = 4;
static const uint16 kMinutecount = 5;
static const uint16 kHourcount = 6;
static const uint16 kZoomon = 7;
static const uint16 kLocation = 8;
static const uint16 kExpos = 9;
static const uint16 kExframepos = 10;
static const uint16 kExtextpos = 12;
static const uint16 kCard1money = 14;
static const uint16 kListpos = 16;
static const uint16 kRyanpage = 18;
static const uint16 kWatchingtime = 19;
static const uint16 kReeltowatch = 21;
static const uint16 kEndwatchreel = 23;
static const uint16 kSpeedcount = 25;
static const uint16 kWatchspeed = 26;
static const uint16 kReeltohold = 27;
static const uint16 kEndofholdreel = 29;
static const uint16 kWatchmode = 31;
static const uint16 kDestafterhold = 32;
static const uint16 kNewsitem = 33;
static const uint16 kLiftflag = 34;
static const uint16 kLiftpath = 35;
static const uint16 kLockstatus = 36;
static const uint16 kDoorpath = 37;
static const uint16 kCounttoopen = 38;
static const uint16 kCounttoclose = 39;
static const uint16 kRockstardead = 40;
static const uint16 kGeneraldead = 41;
static const uint16 kSartaindead = 42;
static const uint16 kAidedead = 43;
static const uint16 kBeenmugged = 44;
static const uint16 kGunpassflag = 45;
static const uint16 kCanmovealtar = 46;
static const uint16 kTalkedtoattendant = 47;
static const uint16 kTalkedtosparky = 48;
static const uint16 kTalkedtoboss = 49;
static const uint16 kTalkedtorecep = 50;
static const uint16 kCardpassflag = 51;
static const uint16 kMadmanflag = 52;
static const uint16 kKeeperflag = 53;
static const uint16 kLasttrigger = 54;
static const uint16 kMandead = 55;
static const uint16 kSeed = 56;
static const uint16 kNeedtotravel = 59;
static const uint16 kThroughdoor = 60;
static const uint16 kNewobs = 61;
static const uint16 kRyanon = 62;
static const uint16 kCombatcount = 63;
static const uint16 kLastweapon = 64;
static const uint16 kDreamnumber = 65;
static const uint16 kRoomafterdream = 66;
static const uint16 kShakecounter = 67;
static const uint16 kSpeechcount = 68;
static const uint16 kCharshift = 69;
static const uint16 kKerning = 71;
static const uint16 kBrightness = 72;
static const uint16 kRoomloaded = 73;
static const uint16 kDidzoom = 74;
static const uint16 kLinespacing = 75;
static const uint16 kTextaddressx = 77;
static const uint16 kTextaddressy = 79;
static const uint16 kTextlen = 81;
static const uint16 kLastxpos = 82;
static const uint16 kIcontop = 84;
static const uint16 kIconleft = 86;
static const uint16 kItemframe = 88;
static const uint16 kItemtotran = 89;
static const uint16 kRoomad = 90;
static const uint16 kOldsubject = 92;
static const uint16 kWithobject = 94;
static const uint16 kWithtype = 95;
static const uint16 kLookcounter = 96;
static const uint16 kCommand = 98;
static const uint16 kCommandtype = 99;
static const uint16 kOldcommandtype = 100;
static const uint16 kObjecttype = 101;
static const uint16 kGetback = 102;
static const uint16 kInvopen = 103;
static const uint16 kMainmode = 104;
static const uint16 kPickup = 105;
static const uint16 kLastinvpos = 106;
static const uint16 kExamagain = 107;
static const uint16 kNewtextline = 108;
static const uint16 kOpenedob = 109;
static const uint16 kOpenedtype = 110;
static const uint16 kOldmapadx = 111;
static const uint16 kOldmapady = 113;
static const uint16 kMapadx = 115;
static const uint16 kMapady = 117;
static const uint16 kMapoffsetx = 119;
static const uint16 kMapoffsety = 121;
static const uint16 kMapxstart = 123;
static const uint16 kMapystart = 125;
static const uint16 kMapxsize = 127;
static const uint16 kMapysize = 128;
static const uint16 kHavedoneobs = 129;
static const uint16 kManisoffscreen = 130;
static const uint16 kRainspace = 131;
static const uint16 kFacing = 132;
static const uint16 kLeavedirection = 133;
static const uint16 kTurntoface = 134;
static const uint16 kTurndirection = 135;
static const uint16 kMaintimer = 136;
static const uint16 kIntrocount = 138;
static const uint16 kArrowad = 139;
static const uint16 kCurrentkey = 141;
static const uint16 kOldkey = 142;
static const uint16 kUseddirection = 143;
static const uint16 kCurrentkey2 = 144;
static const uint16 kTimercount = 145;
static const uint16 kOldtimercount = 146;
static const uint16 kMapx = 147;
static const uint16 kMapy = 148;
static const uint16 kNewscreen = 149;
static const uint16 kRyanx = 150;
static const uint16 kRyany = 151;
static const uint16 kLastflag = 152;
static const uint16 kLastflagex = 153;
static const uint16 kFlagx = 154;
static const uint16 kFlagy = 155;
static const uint16 kCurrentex = 156;
static const uint16 kCurrentfree = 157;
static const uint16 kFramesad = 158;
static const uint16 kDataad = 160;
static const uint16 kFrsegment = 162;
static const uint16 kObjectx = 164;
static const uint16 kObjecty = 166;
static const uint16 kOffsetx = 168;
static const uint16 kOffsety = 170;
static const uint16 kSavesize = 172;
static const uint16 kSavesource = 174;
static const uint16 kSavex = 176;
static const uint16 kSavey = 177;
static const uint16 kCurrentob = 178;
static const uint16 kPrioritydep = 179;
static const uint16 kDestpos = 180;
static const uint16 kReallocation = 181;
static const uint16 kRoomnum = 182;
static const uint16 kNowinnewroom = 183;
static const uint16 kResetmanxy = 184;
static const uint16 kNewlocation = 185;
static const uint16 kAutolocation = 186;
static const uint16 kMustload = 187;
static const uint16 kAnswered = 188;
static const uint16 kSaidno = 189;
static const uint16 kDoorcheck1 = 190;
static const uint16 kDoorcheck2 = 191;
static const uint16 kDoorcheck3 = 192;
static const uint16 kDoorcheck4 = 193;
static const uint16 kMousex = 194;
static const uint16 kMousey = 196;
static const uint16 kMousebutton = 198;
static const uint16 kMousebutton1 = 200;
static const uint16 kMousebutton2 = 202;
static const uint16 kMousebutton3 = 204;
static const uint16 kMousebutton4 = 206;
static const uint16 kOldbutton = 208;
static const uint16 kOldx = 210;
static const uint16 kOldy = 212;
static const uint16 kLastbutton = 214;
static const uint16 kOldpointerx = 216;
static const uint16 kOldpointery = 218;
static const uint16 kDelherex = 220;
static const uint16 kDelherey = 222;
static const uint16 kPointerxs = 224;
static const uint16 kPointerys = 225;
static const uint16 kDelxs = 226;
static const uint16 kDelys = 227;
static const uint16 kPointerframe = 228;
static const uint16 kPointerpower = 229;
static const uint16 kAuxpointerframe = 230;
static const uint16 kPointermode = 231;
static const uint16 kPointerspeed = 232;
static const uint16 kPointercount = 233;
static const uint16 kInmaparea = 234;
static const uint16 kSlotdata = 235;
static const uint16 kThisslot = 236;
static const uint16 kSlotflags = 237;
static const uint16 kTalkmode = 238;
static const uint16 kTalkpos = 239;
static const uint16 kCharacter = 240;
static const uint16 kPersondata = 241;
static const uint16 kTalknum = 243;
static const uint16 kNumberinroom = 244;
static const uint16 kCurrentcel = 245;
static const uint16 kOldselection = 246;
static const uint16 kStopwalking = 247;
static const uint16 kMouseon = 248;
static const uint16 kPlayed = 249;
static const uint16 kTimer1 = 251;
static const uint16 kTimer2 = 252;
static const uint16 kTimer3 = 253;
static const uint16 kWholetimer = 254;
static const uint16 kTimer1to = 256;
static const uint16 kTimer2to = 257;
static const uint16 kTimer3to = 258;
static const uint16 kWatchdump = 259;
static const uint16 kLogonum = 260;
static const uint16 kOldlogonum = 261;
static const uint16 kNewlogonum = 262;
static const uint16 kNetseg = 263;
static const uint16 kNetpoint = 265;
static const uint16 kKeynum = 267;
static const uint16 kCursorstate = 268;
static const uint16 kPressed = 269;
static const uint16 kPresspointer = 270;
static const uint16 kGraphicpress = 272;
static const uint16 kPresscount = 273;
static const uint16 kKeypadax = 274;
static const uint16 kKeypadcx = 276;
static const uint16 kLightcount = 278;
static const uint16 kFolderpage = 279;
static const uint16 kDiarypage = 280;
static const uint16 kMenucount = 281;
static const uint16 kSymboltopx = 282;
static const uint16 kSymboltopnum = 283;
static const uint16 kSymboltopdir = 284;
static const uint16 kSymbolbotx = 285;
static const uint16 kSymbolbotnum = 286;
static const uint16 kSymbolbotdir = 287;
static const uint16 kSymboltolight = 288;
static const uint16 kSymbol1 = 289;
static const uint16 kSymbol2 = 290;
static const uint16 kSymbol3 = 291;
static const uint16 kSymbolnum = 292;
static const uint16 kDumpx = 293;
static const uint16 kDumpy = 295;
static const uint16 kWalkandexam = 297;
static const uint16 kWalkexamtype = 298;
static const uint16 kWalkexamnum = 299;
static const uint16 kCursloc = 300;
static const uint16 kCurslocx = 302;
static const uint16 kCurslocy = 304;
static const uint16 kCurpos = 306;
static const uint16 kMonadx = 308;
static const uint16 kMonady = 310;
static const uint16 kGotfrom = 312;
static const uint16 kMonsource = 314;
static const uint16 kNumtodo = 316;
static const uint16 kTimecount = 318;
static const uint16 kCounttotimed = 320;
static const uint16 kTimedseg = 322;
static const uint16 kTimedoffset = 324;
static const uint16 kTimedy = 326;
static const uint16 kTimedx = 327;
static const uint16 kNeedtodumptimed = 328;
static const uint16 kHandle = 329;
static const uint16 kLoadingorsave = 331;
static const uint16 kCurrentslot = 332;
static const uint16 kCursorpos = 333;
static const uint16 kColourpos = 334;
static const uint16 kFadedirection = 335;
static const uint16 kNumtofade = 336;
static const uint16 kFadecount = 337;
static const uint16 kAddtogreen = 338;
static const uint16 kAddtored = 339;
static const uint16 kAddtoblue = 340;
static const uint16 kLastsoundreel = 341;
static const uint16 kSoundbuffer = 343;
static const uint16 kSoundbufferad = 345;
static const uint16 kSoundbufferpage = 347;
static const uint16 kSoundtimes = 348;
static const uint16 kNeedsoundbuff = 349;
static const uint16 kOldint9seg = 350;
static const uint16 kOldint9add = 352;
static const uint16 kOldint8seg = 354;
static const uint16 kOldint8add = 356;
static const uint16 kOldsoundintseg = 358;
static const uint16 kOldsoundintadd = 360;
static const uint16 kSoundbaseadd = 362;
static const uint16 kDsp_status = 364;
static const uint16 kDsp_write = 366;
static const uint16 kDmaaddress = 368;
static const uint16 kSoundint = 369;
static const uint16 kSounddmachannel = 370;
static const uint16 kSampleplaying = 371;
static const uint16 kTestresult = 372;
static const uint16 kCurrentirq = 373;
static const uint16 kSpeechloaded = 374;
static const uint16 kSpeechlength = 375;
static const uint16 kVolume = 377;
static const uint16 kVolumeto = 378;
static const uint16 kVolumedirection = 379;
static const uint16 kVolumecount = 380;
static const uint16 kPlayblock = 381;
static const uint16 kWongame = 382;
static const uint16 kLasthardkey = 383;
static const uint16 kBufferin = 384;
static const uint16 kBufferout = 386;
static const uint16 kExtras = 388;
static const uint16 kWorkspace = 390;
static const uint16 kMapstore = 392;
static const uint16 kCharset1 = 394;
static const uint16 kBuffers = 396;
static const uint16 kMainsprites = 398;
static const uint16 kBackdrop = 400;
static const uint16 kMapdata = 402;
static const uint16 kSounddata = 404;
static const uint16 kSounddata2 = 406;
static const uint16 kRecordspace = 408;
static const uint16 kFreedat = 410;
static const uint16 kSetdat = 412;
static const uint16 kReel1 = 414;
static const uint16 kReel2 = 416;
static const uint16 kReel3 = 418;
static const uint16 kRoomdesc = 420;
static const uint16 kFreedesc = 422;
static const uint16 kSetdesc = 424;
static const uint16 kBlockdesc = 426;
static const uint16 kSetframes = 428;
static const uint16 kFreeframes = 430;
static const uint16 kPeople = 432;
static const uint16 kReels = 434;
static const uint16 kCommandtext = 436;
static const uint16 kPuzzletext = 438;
static const uint16 kTraveltext = 440;
static const uint16 kTempgraphics = 442;
static const uint16 kTempgraphics2 = 444;
static const uint16 kTempgraphics3 = 446;
static const uint16 kTempsprites = 448;
static const uint16 kTextfile1 = 450;
static const uint16 kTextfile2 = 452;
static const uint16 kTextfile3 = 454;
static const uint16 kBlinkframe = 456;
static const uint16 kBlinkcount = 457;
static const uint16 kReasseschanges = 458;
static const uint16 kPointerspath = 459;
static const uint16 kManspath = 460;
static const uint16 kPointerfirstpath = 461;
static const uint16 kFinaldest = 462;
static const uint16 kDestination = 463;
static const uint16 kLinestartx = 464;
static const uint16 kLinestarty = 466;
static const uint16 kLineendx = 468;
static const uint16 kLineendy = 470;
static const uint16 kIncrement1 = 472;
static const uint16 kIncrement2 = 474;
static const uint16 kLineroutine = 476;
static const uint16 kLinepointer = 477;
static const uint16 kLinedirection = 478;
static const uint16 kLinelength = 479;
static const uint16 kLiftsoundcount = 480;
static const uint16 kEmmhandle = 481;
static const uint16 kEmmpageframe = 483;
static const uint16 kEmmhardwarepage = 485;
static const uint16 kCh0emmpage = 486;
static const uint16 kCh0offset = 488;
static const uint16 kCh0blockstocopy = 490;
static const uint16 kCh0playing = 492;
static const uint16 kCh0repeat = 493;
static const uint16 kCh0oldemmpage = 494;
static const uint16 kCh0oldoffset = 496;
static const uint16 kCh0oldblockstocopy = 498;
static const uint16 kCh1playing = 500;
static const uint16 kCh1emmpage = 501;
static const uint16 kCh1offset = 503;
static const uint16 kCh1blockstocopy = 505;
static const uint16 kCh1blocksplayed = 507;
static const uint16 kSoundbufferwrite = 509;
static const uint16 kSoundemmpage = 511;
static const uint16 kSpeechemmpage = 513;
static const uint16 kCurrentsample = 515;
static const uint16 kRoomssample = 516;
static const uint16 kGameerror = 517;
static const uint16 kHowmuchalloc = 518;
static const uint16 kReelroutines = 520;
static const uint16 kBasicsample = 977;
static const uint16 kVolumetabname = 990;
static const uint16 kMonkface = 1003;
static const uint16 kTitle7graphics = 1016;
static const uint16 kCurrentfile = 1225;
static const uint16 kRoomscango = 1368;
static const uint16 kRoompics = 1384;
static const uint16 kOplist = 1399;
static const uint16 kInputline = 1402;
static const uint16 kPresslist = 1530;
static const uint16 kSavenames = 1536;
static const uint16 kQuitrequested = 1655;
static const uint16 kSubtitles = 1656;
static const uint16 kForeignrelease = 1657;
static const uint16 kBlocktextdat = (0);
static const uint16 kPersonframes = (0);
static const uint16 kDebuglevel1 = (0);
static const uint16 kDebuglevel2 = (0);
static const uint16 kPlayback = (0);
static const uint16 kMap = (0);
static const uint16 kSettextdat = (0);
static const uint16 kSpanish = (0);
static const uint16 kFramedata = (0);
static const uint16 kRecording = (0);
static const uint16 kFlags = (0);
static const uint16 kGerman = (0);
static const uint16 kTextunder = (0);
static const uint16 kPathdata = (0);
static const uint16 kDemo = (0);
static const uint16 kExframedata = (0);
static const uint16 kIntextdat = (0);
static const uint16 kFreetextdat = (0);
static const uint16 kFrframedata = (0);
static const uint16 kSettext = (0+(130*2));
static const uint16 kOpeninvlist = (0+(228*13));
static const uint16 kRyaninvlist = (0+(228*13)+32);
static const uint16 kPointerback = (0+(228*13)+32+60);
static const uint16 kMapflags = (0+(228*13)+32+60+(32*32));
static const uint16 kStartpal = (0+(228*13)+32+60+(32*32)+(11*10*3));
static const uint16 kEndpal = (0+(228*13)+32+60+(32*32)+(11*10*3)+768);
static const uint16 kMaingamepal = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768);
static const uint16 kSpritetable = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768);
static const uint16 kSetlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32));
static const uint16 kFreelist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5));
static const uint16 kExlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5));
static const uint16 kPeoplelist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5));
static const uint16 kZoomspace = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5));
static const uint16 kPrintedlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40));
static const uint16 kListofchanges = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80));
static const uint16 kUndertimedtext = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4));
static const uint16 kRainlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30));
static const uint16 kInitialreelrouts = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64));
static const uint16 kInitialvars = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64)+977-520);
static const uint16 kLengthofbuffer = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64)+977-520+68-0);
static const uint16 kReellist = (0+(36*144));
static const uint16 kIntext = (0+(38*2));
static const uint16 kLengthofmap = (0+(66*60));
static const uint16 kFreetext = (0+(82*2));
static const uint16 kBlocktext = (0+(98*2));
static const uint16 kBlocks = (0+192);
static const uint16 kFrframes = (0+2080);
static const uint16 kExframes = (0+2080);
static const uint16 kFrames = (0+2080);
static const uint16 kExdata = (0+2080+30000);
static const uint16 kExtextdat = (0+2080+30000+(16*114));
static const uint16 kExtext = (0+2080+30000+(16*114)+((114+2)*2));
static const uint16 kLengthofextra = (0+2080+30000+(16*114)+((114+2)*2)+18000);
static const uint16 kPersontxtdat = (0+24);
static const uint16 kPersontext = (0+24+(1026*2));
static const uint16 kInputport = (0x63);
static const uint16 kForeign = (1);
static const uint16 kCd = (1);
static const uint16 kNumexobjects = (114);
static const uint16 kUndertextsizey = (13);
static const uint16 kZoomy = (132);
static const uint16 kFreedatlen = (16*80);
static const uint16 kExtextlen = (18000);
static const uint16 kLenofmapstore = (22*8*20*8);
static const uint16 kUndertextsizex = (228);
static const uint16 kNumchanges = (250);
static const uint16 kUndertimedysize = (30);
static const uint16 kExframeslen = (30000);
static const uint16 kTablesize = (32);
static const uint16 kScreenwidth = (320);
static const uint16 kKeypadx = (36+112);
static const uint16 kItempicsize = (44);
static const uint16 kDiaryy = (48+12);
static const uint16 kOpsy = (52);
static const uint16 kSymboly = (56);
static const uint16 kInventy = (58);
static const uint16 kMenuy = (60);
static const uint16 kOpsx = (60);
static const uint16 kMaplength = (60);
static const uint16 kSymbolx = (64);
static const uint16 kSetdatlen = (64*128);
static const uint16 kMapwidth = (66);
static const uint16 kTextstart = (66*2);
static const uint16 kMaplen = (66*60);
static const uint16 kDiaryx = (68+24);
static const uint16 kLengthofvars = (68-0);
static const uint16 kKeypady = (72);
static const uint16 kZoomx = (8);
static const uint16 kInventx = (80);
static const uint16 kMenux = (80+40);
static const uint16 kHeaderlen = (96);
static const uint16 kLenofreelrouts = (977-520);


class DreamGenContext : public DreamBase, public Context {
public:
	DreamGenContext(DreamWeb::DreamWebEngine *en) : DreamBase(en), Context(this) {}

	void __start();
#include "stubs.h" // Allow hand-reversed functions to have a signature different than void f()

	void useWire();
	void getNamePos();
	void identifyOb();
	void useLighter();
	void runEndSeq();
	void useOpenBox();
	void clearBuffers();
	void getObTextStart();
	void dumpDiaryKeys();
	void entryTexts();
	void checkInput();
	void putUnderCentre();
	void checkObjectSize();
	void findText1();
	void isRyanHolding();
	void quitSymbol();
	void useCashCard();
	void moneyPoke();
	void doSomeTalk();
	void resetLocation();
	void greyscaleSum();
	void getOpenedSize();
	void adjustUp();
	void fadeScreenDownHalf();
	void fadeDownMon();
	void bartender();
	void outOfOpen();
	void dirCom();
	void endGameSeq();
	void findFirstPath();
	void useSLab();
	void useAltar();
	void startTalk();
	void getAnyAd();
	void usePipe();
	void reminders();
	void runTap();
	void showMonk();
	void checkForExit();
	void lookInInterface();
	void inToInv();
	void adjustLeft();
	void deleteExText();
	void entryAnims();
	void getFreeAd();
	void removeObFromInv();
	void heavy();
	void useKey();
	void monitorLogo();
	void dirFile();
	void pickupConts();
	void allPalette();
	void cantDrop();
	void nextColon();
	void findInvPos();
	void rollEndCredits();
	void getKeyAndLogo();
	void selectOb();
	void receptionist();
	void selectSlot();
	void fadeUpMon();
	void showDiaryPage();
	void transferToEx();
	void reExFromInv();
	void businessMan();
	void outOfInv();
	void diaryKeyP();
	void transferMap();
	void diaryKeyN();
	void purgeAnItem();
	void purgeALocation();
	void notHeldError();
	void getSetAd();
	void getUnderCentre();
	void showKeys();
	void printmessage2();
	void findOpenPos();
	void describeOb();
	void deleteExFrame();
	void searchForSame();
	void rollEm();
	void poolGuard();
	void lookAtPlace();
	void useAxe();
	void findAllOpen();
	void showSlots();
	void findSetObject();
	void deleteExObject();
	void removeFreeObject();
	void lookAtCard();
	void helicopter();
	void getEitherAd();
	void setPickup();
	void dropObject();
	void openOb();
	void drawItAll();
	void useStereo();
	void dumpCurrent();
	void showDiaryKeys();
	void rollEndCredits2();
	void useOpened();
	void fillOpen();
	void signOn();
	void locationPic();
	void triggerMessage();
	void swapWithOpen();
	void dreamweb();
	void dropError();
	void checkInside();
	void findPathOfPoint();
	void getBack1();
	void getDestInfo();
	void setupTimedUse();
	void makeCaps();
	void read();
	void additionalText();
	void mugger();
	void searchForString();
	void selectOpenOb();
	void useGun();
	void autoAppear();
	void newPlace();
	void useHandle();
	void showPuzText();
	void incRyanPage();
	void findExObject();
	void clearChanges();
	void searchForFiles();
	void monkSpeaking();
	void getExAd();
	void initialMonCols();
	void useButtonA();
	void execCommand();
	void updateSymbolBot();
	void findPuzText();
	void swapWithInv();
	void useControl();
	void adjustRight();
	void updateSymbolTop();
	void createName();
	void getPersonText();
	void parser();
	void showNames();
	void emergencyPurge();
	void quitKey();
	void processTrigger();
	void transferConToEx();
	void adjustDown();
	void withWhat();
};

} // End of namespace DreamGen

#endif
