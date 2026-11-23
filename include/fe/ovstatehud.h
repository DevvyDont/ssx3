#ifndef FE_OVSTATEHUD_H
#define FE_OVSTATEHUD_H

// cOVStateHUD1P: the in-game HUD overlay state (both players' HUDs in one state): place, time, trick/boost meter,
// wipeout meter, "Now Playing" big-radio popup, slope-style places, countdown, message and skip lines.
// EA's source is fe/ovstatehud.cpp (the July 2003 preview's strings; On Tour's asserts name ovstatehud.cpp too).
// Retail code: fe/ovstatehud (ctor 0x001E9A30 ... cOVStateHUD1P_renderMultiText1 0x001F2DB0) and the out-of-line
// helpers in fe/all_in_one_fe (func_0021CBA0, the destructor, cOVStateHUD1P_renderWipeoutMeter 0x0021D9A0 ...
// cOVStateHUD1P_renderNISSkipMessage 0x00220AD0).
//
// Size 0xC98:
//   [R] operator new(3224, "cOVStateHUD1P") + cOVStateHUD1P_cOVStateHUD1P(mem, engine, 0) in main/gameload; the last field the code
//       touches is the float at 0xC94 (onCreateScreen, onUpdate, onRender2D).
//   [O] On Tour's class is 0xCC0: its tail from mTextUberIndexLast to mNISSkipMessageFrame sits 0x20 higher
//       (0x57C ... 0xCB4) and its rider infos are 0x60 instead of retail's 0x8C (no course-progress tracker).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8) - no cFEState / cOVState: the ctors run
// cUIState_cUIState and the destructor goes straight from cOVStateHUD1P_vtbl to cUIState__cUIState. Vtable
// cOVStateHUD1P_vtbl (0x00474160, 24 slots: 1 dtor, 4 onCreateScreen, 12 onUpdate, 15 onRender2D, 24 handleEvent; the
// rest cListNode_* / cUIState_*).
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. bools are ints (ee-gcc 2.95's bool is
// 4 bytes). On Tour's member order maps onto retail with these differences: the 0x3C
// tracker inside each rider info, the three words at 0x160, no mEventStateAdditionalData (retail's mRenderFlags is
// the word after mEventStateData: handleEvent's flag bits 0x10000 / 0x20000 / 0x40000 and the reset mask 0xFFBDC3FF
// are On Tour's), 0x18-byte slope-style infos (no isTime), a second font (0x428) and a divider string (0x430), the
// "switch" texture (0x470), 26 gen textures, 5x3 boost textures, the "Sphere" texture and two extra bar configs.
// Evidence tags:
//   [R] retail's constructors (cOVStateHUD1P_cOVStateHUD1P, func_0021CBA0), destructor and allocation size;
//   [U] how retail code uses the field (C bodies in src/fe/ovstatehud.cpp and src/fe/all_in_one_fe.cpp; the asm of
//       onUpdate / onRender2D / func_002204A0);
//   [O] SSX On Tour's DWARF (class cOVStateHUD1P, tHUDRiderInfo, tBigRadioText, tSSPlaceInfo, tBarConfig,
//       cTextureElement) and its debug build's onCreateScreen / handleEvent (tools/ontour_asm.py), which do the same
//       stores in the same order as retail.
// (No preview evidence: layout_xlate has no paired cOVStateHUD1P accessors.)

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"
#include "ui/uithing.h" // tARGB
#include "fe/hudradar.h" // cHUDRadar
#include "fe/textpictline.h" // tTextPictLine

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cFont;

// A texture-bank element (On Tour cTextureElement, 0x20; the bank's elements in ui/uitexturebank.h have this layout:
// ovstatehud.cpp's onCreateScreen reads the handle at 0x0 and the UVs at 0xC ... 0x18 from them).
struct cTextureElement {
    int mSourceShape;               // 0x00 [O]; [R] -1 (the HUD ctors' inlined default ctor)
    float mWidth;                   // 0x04 [O]; [R] 1.0
    float mHeight;                  // 0x08 [O]; [R] 1.0
    float mTop;                     // 0x0C [O]; [R] 0 (stored as int 0)
    float mLeft;                    // 0x10 [O]; [R] 0 (stored as int 0)
    float mRight;                   // 0x14 [O]; [R] 1.0
    float mBottom;                  // 0x18 [O]; [R] 1.0
    unsigned int mHashID;           // 0x1C [O]
};

// One player's HUD state (On Tour tHUDRiderInfo, 0x60; retail 0x8C with the tracker at 0x8). Placed by the two
// onCreateScreens' identical store sequences (currRank -1, rider -1, playerID -1, 1.0, -1.0, white, -1.0, 0.5, -1,
// -1.0, 0, -1.0, 0, 0, 0, pointOffset 0).
struct tHUDRiderInfo {
    int rider;                      // 0x00 [O]; [U] onCreateScreen: the local rider's cBeing getAIIndex() (On Tour stores
                                    //       the same call's result here)
    int playerID;                   // 0x04 [O]; [U] onCreateScreen: the rider's index in the race (On Tour: player index)
    cHUDRadar unk_08;               // 0x08 [R] course-progress tracker (cHUDRadar_cHUDRadar constructs it, vptr cHUDRadar_vtbl at its
                                    //       0x38; cHUDRadar__cHUDRadar destroys it; func_0020EB50 lays it out); retail only
    int currRank;                   // 0x44 [O]; [U] -1
    float rankAnimFrame;            // 0x48 [O]; [U] 1.0
    float rankGlowFrame;            // 0x4C [O]; [U] -1.0
    int rankType;                   // 0x50 [O]
    tARGB uberGlowColour;           // 0x54 [O] (On Tour 16-aligned at 0x20); [U] white, copied whole
    float uberGlowFrame;            // 0x64 [O]; [U] -1.0
    float uberSpinFrame;            // 0x68 [O]; [U] 0.5
    int lastPotentialTrickLevel;    // 0x6C [O]; [U] -1
    float potentialTrickLevelChangeAnimFrame; // 0x70 [O]; [U] -1.0
    int beatPointChallenge;         // 0x74 [O] bool (On Tour 1 byte); [U] 0
    float pointChallengeSuccessAnimFrame; // 0x78 [O]; [U] -1.0
    int pointOffset;                // 0x7C [O]; [U] 0 (onCreateScreen, onUpdate)
    unsigned int renderFilterMask;  // 0x80 [O]; [U] 0 (onCreateScreen, onUpdate)
    unsigned int renderAddMask;     // 0x84 [O]; [U] 0
    int renderCenterAreaDisplay;    // 0x88 [O] eRenderCenterAreaDisplay; [U] 0
};

// One line of the "Now Playing" popup (On Tour tBigRadioText, 0x68 - same in retail).
struct tBigRadioText {
    char text[96];                  // 0x00 [O]; [U] onUpdate measures it (cFont_textExtent at 0x19C + i * 0x68)
    float width;                    // 0x60 [O]; [U] onUpdate stores the extent (0x1FC + i * 0x68)
    float height;                   // 0x64 [O]; [U] onUpdate (0x200 + i * 0x68)
};

// One slope-style place line (On Tour tSSPlaceInfo is 0x1C with a bool isTime at 0x18; retail's stride is 0x18).
struct tSSPlaceInfo {
    char scoreText[16];             // 0x00 [O]; [U] onUpdate sprintf()s the score into it
    int aiIndex;                    // 0x10 [O]; [U] onCreateScreen -1; onUpdate stores the rider's getAIIndex()
    float scoreOffsetX;             // 0x14 [O]; [U] onUpdate stores the text's extent
};

// A segmented bar's layout (On Tour tBarConfig, 0x8).
struct tBarConfig {
    float partScale;                // 0x0 [O]; [U] onCreateScreen: the scale that fits the segments
    int numMidSegments;             // 0x4 [O]; [U] onCreateScreen: the segment count
};

// One HUD element's layout (On Tour struct tHUDDef, 0x24): the const tables in .rodata the HUD renderers take a
// pointer into (D_00477000 ...; renderTime1 / renderText index them by 0x24). [O] tHUDDef (same members and offsets);
// [P] the preview mangles the type (renderTrickLevel__13cOVStateHUD1PPC7tHUDDef ...); [R] retail's out-of-line
// accessors tHUDDef_getPos / tHUDDef_getSize / tHUDDef_getScale / tHUDDef_getColour. Replaces ovstatehud.cpp's
// sHudElem_9AD0 / sHudElemDef_001F16C0 and cOVStateHUDElem views (same layout).
struct tHUDDef {
    short px;                       // 0x00 [O]; [U] tHUDDef_getPos: the x position (short -> float)
    short py;                       // 0x02 [O]; [U] tHUDDef_getPos: the y position
    short sx;                       // 0x04 [O]; [U] tHUDDef_getSize: the width (negative = not set)
    short sy;                       // 0x06 [O]; [U] tHUDDef_getSize: the height
    float xx;                       // 0x08 [O]; [U] tHUDDef_getScale: the x scale
    float xy;                       // 0x0C [O]; [U] tHUDDef_getScale: the y scale
    float a;                        // 0x10 [O]; [U] tHUDDef_getColour copies a, r, g, b into a tARGB
    float r;                        // 0x14 [O]
    float g;                        // 0x18 [O]
    float b;                        // 0x1C [O]
    char pivotX;                    // 0x20 [O]; [U] renderTime1 / renderText: the horizontal alignment (signed)
    char pivotY;                    // 0x21 [O]; [U] the vertical alignment
    char layer;                     // 0x22 [O]; [U] the element's draw layer (renderMultiText passes +0x22 on)
    char unk_23[0x1];               // 0x23 tail padding
};

struct cOVStateHUD1P : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVStateHUD1P_vtbl (both ctors and the dtor)
    // cOVStateHUD1P's notes on cUIState's fields:
    //   mHashName: [U] onCreateScreen stores GetHashValue32("hud")
    //   mpEngine: [U] onCreateScreen looks textures (+0x58) and fonts (+0x8) up through it
    // --- cOVStateHUD1P
    tHUDRiderInfo mRiderInfo[2];    // 0x048 [O] (On Tour 0x50, 0x60 each); [R] the ctors build two 0x8C-byte elements
                                    //       here, the dtor destroys them last to first; [U] onCreateScreen fills one per
                                    //       local rider
    int unk_160;                    // 0x160 [U] handleEvent 8 sets it, 9 clears it; while set (and the HUD is shown)
                                    //       onUpdate animates 0x164 / 0x168 and sets mRenderFlags 0x80000. Retail only
    float unk_164;                  // 0x164 [U] onUpdate: frame counter += 1/60, wrapped to [0, 1]; onRender2D reads it
    float unk_168;                  // 0x168 [U] onUpdate: += 1/60 up to 5.0, then -1.0; handleEvent 8 / 9 clear it
    float mTimeSeparatorWidth;      // 0x16C [O]; [U] onCreateScreen: extent of the divider string (":" or "'");
                                    //       renderTime reads it
    float mTimeValueWidth;          // 0x170 [O]; [U] extent of "00"
    float mTimePrefixWidth;         // 0x174 [O]; [U] extent of "-"
    float mDigitOneOffsetX;         // 0x178 [O]; [U] ("00" - "11") / 4
    int mTimeHour;                  // 0x17C [O]; [U] onUpdate: time / 3600
    int mTimeMin;                   // 0x180 [O]; [U] onUpdate
    int mTimeSec;                   // 0x184 [O]; [U] onUpdate
    int mTimeSecFrac;               // 0x188 [O]; [U] onUpdate (an int made with cvt.w.s and stored with swc1)
    unsigned int mLastMissionID;    // 0x18C [O]; [U] onCreateScreen / handleEvent 7 (On Tour's RESET) store -1;
                                    //       func_0021FD38 (the mission indicator) reads it
    float mBigRadioAnimFrame;       // 0x190 [O]; [U] -1.0 in onCreateScreen and handleEvent 5 (On Tour's
                                    //       SHOWSONGSTATS)
    float mBigRadioBkgdAnimFrame;   // 0x194 [O]; [U] handleEvent 5 zeroes it; onUpdate, func_002204A0
    float mBigRadioTextAnimFrame;   // 0x198 [O]; [U] ditto
    tBigRadioText mBigRadioText[5]; // 0x19C [O] (On Tour 0x140); [U] onUpdate, func_002204A0
    int mBigRadioTextLinesNum;      // 0x3A4 [O]; [U] onUpdate's line loop; handleEvent 5 zeroes it
    int mBigRadioTextLineSongIndexMax; // 0x3A8 [O]; [U] handleEvent 5 zeroes it; func_002204A0
    float mBigRadioBkgdSize[2];     // 0x3AC [O] t2Vector; [U] onUpdate stores x, y; func_002204A0 copies it whole (ldl/ldr)
    unsigned int mLastSongNameHashId; // 0x3B4 [O]; [U] 0xF0000000 (onCreateScreen); handleEvent 5 compares and stores
                                    //       the song name's hash
    int unk_3B8;                    // 0x3B8 no retail access (On Tour: mShowLoadMessage here)
    float unk_3BC;                  // 0x3BC no retail access (On Tour: mLoadAnimFrame here)
    float mEventStateFrame;         // 0x3C0 [O]; [U] -1.0 (onCreateScreen); onUpdate / onRender2D (handleEvent 6 / 7
                                    //       store int 0)
    int mEventStateHFX;             // 0x3C4 [O]; [U] 80 (onCreateScreen, handleEvent 7), 8 (handleEvent 6)
    int mEventStateData;            // 0x3C8 [O]; [U] 0 (onCreateScreen, handleEvent 6)
    unsigned int mRenderFlags;      // 0x3CC [O] (On Tour 0x374, after mEventStateAdditionalData, which retail lacks);
                                    //       [U] handleEvent's bits 0x10000 / 0x20000 / 0x40000 and mask 0xFFBDC3FF, as
                                    //       On Tour's; onUpdate / onRender2D test and set many more
    float mTimeAlertAnimFrame;      // 0x3D0 [O]; [U] -1.0 (onCreateScreen); onUpdate
    int mbHideHUD;                  // 0x3D4 [O] bool (On Tour 1 byte at 0x37C); [R] ctor 0; [U] handleEvent 0 / 1 set /
                                    //       clear it (On Tour HUD_HIDESCRIPT / SHOWSCRIPT); onUpdate tests it
    int mHideHUDOV;                 // 0x3D8 [O]; [R] ctor 0; [U] handleEvent 2 zeroes, 3 increments, 4 decrements it
    tSSPlaceInfo mSlopeStylePlaceInfo[3]; // 0x3DC [O]; [U] onCreateScreen / onUpdate / onRender2D, stride 0x18
    float mMaxScoreTextWidth;       // 0x424 [O]; [U] onUpdate zeroes it and keeps the widest score text
    cFont* unk_428;                 // 0x428 [U] onCreateScreen: the "fefont" font; the text-and-picture lines and
                                    //       func_002204A0 draw with it. Retail only
    cFont* mpHUDFont;               // 0x42C [O]; [U] onCreateScreen: the "hudfont" font (On Tour asserts mpHUDFont)
    char* unk_430;                  // 0x430 [U] onCreateScreen: the time divider string (":" for languages 0 / 2, "'" for
                                    //       1 / 3); renderTime draws it (On Tour has five char dividers instead)
    cTextureElement* mpCountdownElements[4]; // 0x434 [O]; [U] "go", "nm1w", "nm2w", "nm3w"
    cTextureElement* mpNumWhite[6]; // 0x444 [O]; [U] "nm1w" ... "nm6w"; renderRank indexes it
    cTextureElement* mpNumOrdinal[4]; // 0x45C [O]; [U] "st_w", "nd_w", "rd_w", "th_w"
    cTextureElement* mpNumSlash;    // 0x46C [O]; [U] "slash"; renderRank
    cTextureElement* unk_470;       // 0x470 [U] onCreateScreen: the "switch" texture; onRender2D. Retail only
    cTextureElement mGlow;          // 0x474 [O]; [R] the ctors' inlined cTextureElement() (-1, 1, 1, 0, 0, 1, 1);
                                    //       [U] onCreateScreen: GrMan's effect texture, its size and hash 0xDEEDDEED (On
                                    //       Tour does the same into mGlow); cOVStateHUD1P_renderGlow / renderUberChar draw it
    cTextureElement* mpGenTexElem[26]; // 0x494 [O] (On Tour [13]); [U] onCreateScreen fills it from the hash table
                                    //       D_004C88D8 (On Tour: kGenTexElemID); the render helpers index it
    cTextureElement* mpAdrenalinElem[5][3]; // 0x4FC [O] (On Tour [4][3]); [U] filled from D_004C8940[5][3] (On Tour:
                                    //       kAdrenalinElemTexID)
    cTextureElement* unk_538;       // 0x538 [U] onCreateScreen: the "Sphere" texture; onRender2D. Retail only
    tBarConfig unk_53C[2];          // 0x53C [U] onCreateScreen: vertical bar configs from the 1P / 2P HUD tables'
                                    //       first element (D_00476940, D_00477480); onRender2D. Retail only
    tBarConfig mWipeoutMeterConfig1P; // 0x54C [O]; [U] horizontal bar config from D_00477048 (count 1 / scale 1, or 0
                                    //       and width / 18, as On Tour's code)
    tBarConfig mWipeoutMeterConfig2P; // 0x554 [O]; [U] ditto from D_00477B88
    int mTextUberIndexLast;         // 0x55C [O]; [U] BXrand() & 1: which Uber hint mTextUberHelp shows; onUpdate
    tTextPictLine mTextUberHelp;      // 0x560 [O] tTextPictLine (0x14C, as On Tour); [U] func_001E94E0 builds it
    tTextPictLine mTextRecoverHelp;   // 0x6AC [O] tTextPictLine; [U] built from D_004A21D8 (RECOVER);
                                    //       renderWipeoutMeter draws it
    tTextPictLine mTextRecoverHelpAlt; // 0x7F8 [O] tTextPictLine; [U] ditto
    unsigned short mMessage[128];   // 0x944 [O]; [U] handleEvent 12 copies the text in, 13 / 7 clear it
    float mMessageFrame;            // 0xA44 [O]; [U] onUpdate animates it (handleEvent / onCreateScreen store int 0)
    unsigned short mNetStatusMessage[128]; // 0xA48 [O]; [U] handleEvent 14 copies the text in, 15 clears it
    tTextPictLine mSkipMessage;       // 0xB48 [O] tTextPictLine; [U] makeTextPictLine builds "%s @skip %s";
                                    //       renderNISSkipMessage draws it (reads its font pointer at +0x8)
    float mNISSkipMessageFrame;     // 0xC94 [O]; [U] -1.0 (onCreateScreen); onUpdate / onRender2D
};

CHECK_OFFSET(cTextureElement, mSourceShape, 0x00);
CHECK_OFFSET(cTextureElement, mWidth, 0x04);
CHECK_OFFSET(cTextureElement, mHeight, 0x08);
CHECK_OFFSET(cTextureElement, mTop, 0x0C);
CHECK_OFFSET(cTextureElement, mLeft, 0x10);
CHECK_OFFSET(cTextureElement, mRight, 0x14);
CHECK_OFFSET(cTextureElement, mBottom, 0x18);
CHECK_OFFSET(cTextureElement, mHashID, 0x1C);
CHECK_SIZE(cTextureElement, 0x20);

CHECK_OFFSET(tHUDRiderInfo, rider, 0x00);
CHECK_OFFSET(tHUDRiderInfo, playerID, 0x04);
CHECK_OFFSET(tHUDRiderInfo, unk_08, 0x08);
CHECK_OFFSET(tHUDRiderInfo, currRank, 0x44);
CHECK_OFFSET(tHUDRiderInfo, rankAnimFrame, 0x48);
CHECK_OFFSET(tHUDRiderInfo, rankGlowFrame, 0x4C);
CHECK_OFFSET(tHUDRiderInfo, rankType, 0x50);
CHECK_OFFSET(tHUDRiderInfo, uberGlowColour, 0x54);
CHECK_OFFSET(tHUDRiderInfo, uberGlowFrame, 0x64);
CHECK_OFFSET(tHUDRiderInfo, uberSpinFrame, 0x68);
CHECK_OFFSET(tHUDRiderInfo, lastPotentialTrickLevel, 0x6C);
CHECK_OFFSET(tHUDRiderInfo, potentialTrickLevelChangeAnimFrame, 0x70);
CHECK_OFFSET(tHUDRiderInfo, beatPointChallenge, 0x74);
CHECK_OFFSET(tHUDRiderInfo, pointChallengeSuccessAnimFrame, 0x78);
CHECK_OFFSET(tHUDRiderInfo, pointOffset, 0x7C);
CHECK_OFFSET(tHUDRiderInfo, renderFilterMask, 0x80);
CHECK_OFFSET(tHUDRiderInfo, renderAddMask, 0x84);
CHECK_OFFSET(tHUDRiderInfo, renderCenterAreaDisplay, 0x88);
CHECK_SIZE(tHUDRiderInfo, 0x8C);

CHECK_OFFSET(tBigRadioText, text, 0x00);
CHECK_OFFSET(tBigRadioText, width, 0x60);
CHECK_OFFSET(tBigRadioText, height, 0x64);
CHECK_SIZE(tBigRadioText, 0x68);

CHECK_OFFSET(tSSPlaceInfo, scoreText, 0x00);
CHECK_OFFSET(tSSPlaceInfo, aiIndex, 0x10);
CHECK_OFFSET(tSSPlaceInfo, scoreOffsetX, 0x14);
CHECK_SIZE(tSSPlaceInfo, 0x18);

CHECK_OFFSET(tBarConfig, partScale, 0x0);
CHECK_OFFSET(tBarConfig, numMidSegments, 0x4);
CHECK_SIZE(tBarConfig, 0x8);

CHECK_OFFSET(tHUDDef, px, 0x00);
CHECK_OFFSET(tHUDDef, py, 0x02);
CHECK_OFFSET(tHUDDef, sx, 0x04);
CHECK_OFFSET(tHUDDef, sy, 0x06);
CHECK_OFFSET(tHUDDef, xx, 0x08);
CHECK_OFFSET(tHUDDef, xy, 0x0C);
CHECK_OFFSET(tHUDDef, a, 0x10);
CHECK_OFFSET(tHUDDef, r, 0x14);
CHECK_OFFSET(tHUDDef, g, 0x18);
CHECK_OFFSET(tHUDDef, b, 0x1C);
CHECK_OFFSET(tHUDDef, pivotX, 0x20);
CHECK_OFFSET(tHUDDef, pivotY, 0x21);
CHECK_OFFSET(tHUDDef, layer, 0x22);
CHECK_SIZE(tHUDDef, 0x24);

CHECK_OFFSET(cOVStateHUD1P, mRiderInfo, 0x048);
CHECK_OFFSET(cOVStateHUD1P, unk_160, 0x160);
CHECK_OFFSET(cOVStateHUD1P, unk_164, 0x164);
CHECK_OFFSET(cOVStateHUD1P, unk_168, 0x168);
CHECK_OFFSET(cOVStateHUD1P, mTimeSeparatorWidth, 0x16C);
CHECK_OFFSET(cOVStateHUD1P, mTimeValueWidth, 0x170);
CHECK_OFFSET(cOVStateHUD1P, mTimePrefixWidth, 0x174);
CHECK_OFFSET(cOVStateHUD1P, mDigitOneOffsetX, 0x178);
CHECK_OFFSET(cOVStateHUD1P, mTimeHour, 0x17C);
CHECK_OFFSET(cOVStateHUD1P, mTimeMin, 0x180);
CHECK_OFFSET(cOVStateHUD1P, mTimeSec, 0x184);
CHECK_OFFSET(cOVStateHUD1P, mTimeSecFrac, 0x188);
CHECK_OFFSET(cOVStateHUD1P, mLastMissionID, 0x18C);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioAnimFrame, 0x190);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioBkgdAnimFrame, 0x194);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioTextAnimFrame, 0x198);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioText, 0x19C);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioTextLinesNum, 0x3A4);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioTextLineSongIndexMax, 0x3A8);
CHECK_OFFSET(cOVStateHUD1P, mBigRadioBkgdSize, 0x3AC);
CHECK_OFFSET(cOVStateHUD1P, mLastSongNameHashId, 0x3B4);
CHECK_OFFSET(cOVStateHUD1P, unk_3B8, 0x3B8);
CHECK_OFFSET(cOVStateHUD1P, unk_3BC, 0x3BC);
CHECK_OFFSET(cOVStateHUD1P, mEventStateFrame, 0x3C0);
CHECK_OFFSET(cOVStateHUD1P, mEventStateHFX, 0x3C4);
CHECK_OFFSET(cOVStateHUD1P, mEventStateData, 0x3C8);
CHECK_OFFSET(cOVStateHUD1P, mRenderFlags, 0x3CC);
CHECK_OFFSET(cOVStateHUD1P, mTimeAlertAnimFrame, 0x3D0);
CHECK_OFFSET(cOVStateHUD1P, mbHideHUD, 0x3D4);
CHECK_OFFSET(cOVStateHUD1P, mHideHUDOV, 0x3D8);
CHECK_OFFSET(cOVStateHUD1P, mSlopeStylePlaceInfo, 0x3DC);
CHECK_OFFSET(cOVStateHUD1P, mMaxScoreTextWidth, 0x424);
CHECK_OFFSET(cOVStateHUD1P, unk_428, 0x428);
CHECK_OFFSET(cOVStateHUD1P, mpHUDFont, 0x42C);
CHECK_OFFSET(cOVStateHUD1P, unk_430, 0x430);
CHECK_OFFSET(cOVStateHUD1P, mpCountdownElements, 0x434);
CHECK_OFFSET(cOVStateHUD1P, mpNumWhite, 0x444);
CHECK_OFFSET(cOVStateHUD1P, mpNumOrdinal, 0x45C);
CHECK_OFFSET(cOVStateHUD1P, mpNumSlash, 0x46C);
CHECK_OFFSET(cOVStateHUD1P, unk_470, 0x470);
CHECK_OFFSET(cOVStateHUD1P, mGlow, 0x474);
CHECK_OFFSET(cOVStateHUD1P, mpGenTexElem, 0x494);
CHECK_OFFSET(cOVStateHUD1P, mpAdrenalinElem, 0x4FC);
CHECK_OFFSET(cOVStateHUD1P, unk_538, 0x538);
CHECK_OFFSET(cOVStateHUD1P, unk_53C, 0x53C);
CHECK_OFFSET(cOVStateHUD1P, mWipeoutMeterConfig1P, 0x54C);
CHECK_OFFSET(cOVStateHUD1P, mWipeoutMeterConfig2P, 0x554);
CHECK_OFFSET(cOVStateHUD1P, mTextUberIndexLast, 0x55C);
CHECK_OFFSET(cOVStateHUD1P, mTextUberHelp, 0x560);
CHECK_OFFSET(cOVStateHUD1P, mTextRecoverHelp, 0x6AC);
CHECK_OFFSET(cOVStateHUD1P, mTextRecoverHelpAlt, 0x7F8);
CHECK_OFFSET(cOVStateHUD1P, mMessage, 0x944);
CHECK_OFFSET(cOVStateHUD1P, mMessageFrame, 0xA44);
CHECK_OFFSET(cOVStateHUD1P, mNetStatusMessage, 0xA48);
CHECK_OFFSET(cOVStateHUD1P, mSkipMessage, 0xB48);
CHECK_OFFSET(cOVStateHUD1P, mNISSkipMessageFrame, 0xC94);
CHECK_SIZE(cOVStateHUD1P, 0xC98);


// ---------------------------------------------------------------------------------------------------------------------
// cOVStateHiScoreList: the in-game high-score list overlay ("hiscorelist"; help texts by game mode and
// whether there are scores): picking the entry with misc data 1 (event 5) asks the custom interface (the engine's first
// word, slot 4) for the next state and pushes it.
// EA's source: d:/ssx/code/source/fe/ovstatehud.cpp (preview assert strings; the unit is fe/ovstatehud). Retail code:
// onCreateScreen / onWidgetEvent in fe/ovstatehud (0x001E8FF0, 0x001E9130); the ctors func_0021D070 (engine 0) /
// cOVStateHiScoreList_cOVStateHiScoreList (pEngine, id) and the dtor in fe/all_in_one_fe (0x0021D070 ... 0x0021D138).
//
// Size 0x9C: [O] On Tour's class is 0x90 = On Tour's cOVState (no members of its own); retail's cOVState is 0x9C
// (fe/ovstate.h) and the ctors write nothing past mOVID (0x98). Retail never calls the ctors (no allocation site).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). Both ctors (engine 0 / (pEngine, id)) run
// cUIState_cUIState, the inlined cOVState ctor (mOVID, the three bools, cOVState_vtbl, mszChatBuff = " ") and store
// cOVStateHiScoreList_vtbl (0x00473B78, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent,
// 25 cOVState_allowPauseMenu); the dtor stores cOVState_vtbl and runs cUIState__cUIState.
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. fe/ovstate.h has the inherited fields'
// full evidence. Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail
// code uses the field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

struct cOVStateHiScoreList : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVStateHiScoreList_vtbl (ctors), cOVState_vtbl (dtor)
    // cOVStateHiScoreList's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "hiscorelist" through it; onWidgetEvent calls its custom interface (+0x0)
    //       slot 4 and pushes the result on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen; cUIState_showObjSafe / hideObjSafe(this, ...) use it
    // cOVState (fe/ovstate.h, 0x9C)
};

CHECK_SIZE(cOVStateHiScoreList, 0x9C);

#endif /* FE_OVSTATEHUD_H */
