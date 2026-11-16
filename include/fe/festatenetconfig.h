#ifndef FE_FESTATENETCONFIG_H
#define FE_FESTATENETCONFIG_H

// The online network-configuration screens (EA's d:/ssx/code/source/fe/festatenetconfig.cpp: the preview's asserts
// at lines 554 and 641 sit between retail func_001B0F28 and func_001B63E8 (cFEStateNetConfig / cFEStateAuthDNAS),
// those at 1381-1394 between func_001B7CB8 and func_001B8860, both cFEStateNetAccountMem methods). Two classes
// live here, both cFEStateNet states (fe/festatenet.h, 0x6D0):
//
// cFEStateNetConfig: screen "200net_config": picks a network adapter and a saved network configuration, connects
// (a state machine over the network object D_004A1B78 = func_002C22C8()) and runs DNAS authentication. Size 0x91C:
// cMemMan_alloc(0x91C, "cFEStateNetConfig") + cFEStateNetConfig_cFEStateNetConfig(mem, pEngine) (func_001A9710, func_001BF5A8); its
// unicode popup buffer at 0x71C runs to the end. Constructors cFEStateNetConfig_cFEStateNetConfig and func_001E0B28 (fe/fepopupmisc, the
// default one: zeroes the same fields), destructor cFEStateNetConfig__cFEStateNetConfig (frees the heap texts). Retail vtable cFEStateNetConfig_vtbl
// (32 slots: 1 dtor, 4 onCreateScreen func_001B40D0, 5 onDestroyScreen func_001B4168, 12 onUpdate func_001B41A8,
// 17 onPopupEvent func_001B5D58, 18 onWidgetCreate func_001B52C8, 19 onWidgetEvent func_001B60A8, 21 func_001B6078,
// 22 func_001B6080, 25 func_001B54B8 (help), 29 func_001E0BA8, 31 func_001B5E28, 32 func_001E0BA0). Methods:
// fe/feasyncfile 0x1B3F78-0x1B60A8. The DNAS popup cFEStateAuthDNAS (func_001B6798) writes mpustrAuthError
// through its mpParentState.
//
// cFEStateNetAccountMem: the memory-card popup that loads / saves / deletes the online account (name and password)
// on the card (EA's names: the preview's onGainTransition__21cFEStateNetAccountMemb, onLoseTransition__21...).
// Base: cFEBlankPopup (constructor func_001ABF70 = cFEStateNet's + vtable D_0046B6F8; it adds no members), so its
// own fields start at 0x6D0. Size 0xB10: every allocation is cMemMan_alloc(0xB10, "cFEStateNetAccountMem") +
// cFEStateNetAccountMem_cFEStateNetAccountMem(mem, pEngine, pParent, mode, szName, szPassword) (14 sites); the constructor stores 0xB0C last.
// Destructor cFEStateNetAccountMem__cFEStateNetAccountMem. Retail vtable cFEStateNetAccountMem_vtbl (24 slots: 1 dtor, 4 onCreateScreen func_001B7088, 7
// onGainTransition func_001E0C90, 10 onLoseTransition func_001E0C98, 12 onUpdate func_001B7CB8, 17 onPopupEvent
// func_001B84D8, 24 handleEvent func_001B8860). Methods: fe/feasyncfile 0x1B6ED0-0x1B8890.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). There is no On Tour DWARF for
// these classes and the preview keeps no accessors for them, so the own fields are named from their retail use.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events: 0xF + i = option i chosen, 0x14 = back, 0x16 = the popup closed (fe/screenpopup.h).

#include "layout.h"
#include "fe/festatenet.h"

struct cUIThing;
struct cUIMenu;
struct cUIListBox;

struct cFEStateNetConfig {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateNetConfig_vtbl (32 slots), stored by cFEStateNetConfig_cFEStateNetConfig / func_001E0B28 after the cFEStateNet ctor and
                                    //       by the destructor cFEStateNetConfig__cFEStateNetConfig
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetConfig_cFEStateNetConfig stores 0x31
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] passed to every child state's ctor
                                    //       (cMemMan_alloc(..) + ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word
                                    //       (ui/uistate.h); [R] cUIState ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's owner argument; [U] func_001A92D8 calls its slot 17
                                    //       (onPopupEvent)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
                                    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then func_001A8500 stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h)
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override func_001A87D0 sets 1; func_001DE8A8 (own
                                    //       virtual, slot 29) returns mpScreen != 0 && unk_048 != 0 (the preview's
                                    //       isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] func_001AAB68 (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] func_001AAB68 stores the frame when the session's count
                                    //       (func_00262938) becomes > 0 and 0 when it drops; func_001AA9B0 (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter D_004A5B64 + 0x18)
    unsigned int mChallengeHintFrame; // 0x054 [R] 0; [U] func_001AAB68, the same for the session's other list (the
                                    //       vector at session + 0xBC); func_001AA9B0 blinks "IconChallengeHint" with it
    int unk_058;                    // 0x058 [R] 0; [U] func_001A9668/func_001A9710 set 1 before pushing cFEStateLogin;
                                    //       func_001DE860 (slot 7, the onInputBegin override) returns it
    char mszMaskedText[0x40];       // 0x05C [U] func_001A98B8 writes one '*' per character of its string (at most 0x3F;
                                    //       " " when empty) and a NUL; cUIText_setAsciiString(.., mszMaskedText) in
                                    //       func_001ACDD0, func_001B9610, func_001B9678, func_001B9820 (password boxes)
    unsigned short mustrMessage[0x200]; // 0x09C [U] sprintf1 (EA's unicode sprintf) target that becomes the popup's
                                    //       message (func_001A4E28); extent: up to the next used offset
    unsigned short unk_49C[0x80];   // 0x49C [U] func_001AB478: sprintf1 of a status line, then setPopupConfirmation
                                    //       (func_001C5838, unicode) with it
    unsigned short unk_59C[0x80];   // 0x59C [U] func_001AB478: the second status line, used the same way
    cKeyboardPopup* mpKeyboard;     // 0x69C [R] 0; [U] func_001A91B0 stores cMemMan_alloc(0x444, "cKeyboardPopup") +
                                    //       cKeyboardPopup_cKeyboardPopup and pushes it; func_001A9090/90F0/9150 set its texts;
                                    //       func_001A97B8 (onPopupEvent) compares the closing popup with it
    cScreenPopup* mpPopup;          // 0x6A0 [R] 0; [U] createPopup func_001A9C50: cMemMan_alloc(0x360, "createPopup") +
                                    //       cScreenPopup_cScreenPopup, reused (cPopupInfo::clear) when set; func_001A9BC0
                                    //       shows it; func_001A93D0 clears it when that popup closes (event 22)
    cUIText* mpKeyboardText;        // 0x6A4 [R] 0; [U] func_001A91B0 stores its a1 (the text box the
                                    //       keyboard edits); cUIText_setAsciiString(mpKeyboardText, mszMaskedText)
    int unk_6A8;                    // 0x6A8 [R] 1; 0 in the ctors of cFEUseExistingAccount, cFEStateNetConfig,
                                    //       cFEStateAuthDNAS, cFEStateCreateStep1/2, cFEStateCreateDuplicate, cFEStateLogin;
                                    //       [U] while set, func_001A89B0 (slot 25) drops net messages other than kinds
                                    //       0x31/0x33/0x4D/0x4E and opens the disconnect popup (func_001A9930)
    int unk_6AC;                    // 0x6AC [R] 0; [U] func_001A9930 sets it when it opens the "kT_POPUPCMNNetInterrupted"/
                                    //       "kT_POPUPCMNDisconnected" popup (only when clear); func_001A9668/func_001A9710
                                    //       clear it, func_001A5010 tests it
    int unk_6B0;                    // 0x6B0 [R] 0; [U] set to 1 by func_001DE890 (inline setter), func_001BF5A8,
                                    //       func_001C0138; no read seen
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent func_001A8B88: 1 while a net event (0xC8..0x116)
                                    //       waits for slot 30 to accept it, 0 once delivered
    void* unk_6B8;                  // 0x6B8 [R] 0; [U] a heap text buffer: func_001A9CC0 frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int unk_6BC;                    // 0x6BC [R] 2; [U] func_001A92D8 (popup events): 2 on event 21, 0 on event 15;
                                    //       on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_cBXString4), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateNetConfig
    int miSelectedSetting;          // 0x6D0 [R] 0; [U] onWidgetEvent func_001B60A8 stores the "LBSettings" row (list + 0x319) and
                                    //       selects it in mpLBAdapters; func_001B45E8 passes it to the connect call (func_002636A8)
    int miNumSettings;              // 0x6D4 [R] 0; [U] func_001B4CD8: cCommSystem_mNumNetConn clamped to 4, one list row each;
                                    //       func_001B6080 tests > 1
    int unk_6D8;                    // 0x6D8 [R] 0; [U] func_001B55B0 / func_001B5830: 1 when the popup is cancelled (events 0xF/0x14);
                                    //       on 0x16 they push "NetConfigAuthPopup" / cFEStateNetAccountMem only while it is 0, then clear it
    int unk_6DC;                    // 0x6DC [U] func_001B5A58: 1 on event 0xF, 0 on 0x15; on 0x16 it pushes cFEStateLogin when set
    int miWaitFrames;               // 0x6E0 [R] -1; [U] func_001B4CD8 sets 15; onUpdate func_001B41A8 counts it down in state 5
    cUIListBox* mpLBAdapters;       // 0x6E4 [R] 0; [U] onWidgetCreate func_001B52C8: widget "LBAdapters"; func_001B4CD8 adds a row
                                    //       per configuration (cUIListBox_addEntryByAsciiString)
    cUIListBox* mpLBSettings;       // 0x6E8 [R] 0; [U] widget "LBSettings"; func_001B4CD8 reads its font/scale (+ 0x50) and adds
                                    //       the numbered configuration names
    cUIThing* mpPairSettings;       // 0x6EC [U] widget "PairSettings"; enabled / disabled (its slot 8) by func_001B5068 and the popup
                                    //       handlers
    int unk_6F0;                    // 0x6F0 [R] 0 in the default constructor func_001E0B28 only; no other access seen
    cUIMenu* mpMenuConfig;          // 0x6F4 [R] 0; [U] widget "MenuConfig"; func_001B5068 cUIMenu_setSelectedByIndex(.., 0 / 1)
    cUIThing* mpConfigRectangle;    // 0x6F8 [R] 0; [U] widget "configrectangle1"; func_001B5068 shows / hides it (slot 9)
    cUIThing* mpNoConfiguration;    // 0x6FC [R] 0; [U] widget "NoConfiguration"; func_001B5068 shows it when there is no
                                    //       configuration
    int unk_700;                    // 0x700 [R] 0; [U] func_001B5830: 1 on event 0x10, 0 on 0xF; on 0x16 it pushes cFEStateAuthDNASError
                                    //       (with mpustrAuthError) when set
    unsigned short* mpustrPopupText; // 0x704 [R] 0; [U] func_001B5E28 (slot 31) frees it and stores a heap copy of a
                                    //       unicode / ascii message (operator_new_tag + USTR_copy / strcpy1), then points the popup's message (+ 0xC4)
                                    //       at it; the destructor frees it
    int miState;                    // 0x708 [R] 0; [U] the connect state machine of onUpdate func_001B41A8 (0..8); func_001B4198 sets 6,
                                    //       func_001B43A8 / func_001B4470 / func_001B45E8 / func_001B4C40 / func_001B4CD8 / func_001B5268 advance it
    int miResult;                   // 0x70C [U] func_001B4470 / func_001B4C40 / func_001B4CD8 / func_001B5268: the last connect result
                                    //       (1, -200, the number of configurations)
    unsigned short* mpustrAuthError[3]; // 0x710 [R] 0; [U] cFEStateAuthDNAS func_001B6798 stores three heap unicode
                                    //       copies of the DNAS error texts here (through its mpParentState); func_001B5830 passes [1], [2], [0] to
                                    //       cFEStateAuthDNASError's constructor func_001B6AB8; the destructor frees them
    unsigned short mustrPopupText[0x100]; // 0x71C [U] sprintf1 (unicode) target of func_001B45E8 / func_001B47C0 /
                                    //       func_001B4888 that becomes the popup's message (+ 0xC4); extent: to the end of the object
};

CHECK_OFFSET(cFEStateNetConfig, mpPrev, 0x000);
CHECK_OFFSET(cFEStateNetConfig, mpNext, 0x004);
CHECK_OFFSET(cFEStateNetConfig, mVtbl, 0x008);
CHECK_OFFSET(cFEStateNetConfig, mHashName, 0x00C);
CHECK_OFFSET(cFEStateNetConfig, mpEngine, 0x010);
CHECK_OFFSET(cFEStateNetConfig, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateNetConfig, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateNetConfig, unk_016, 0x016);
CHECK_OFFSET(cFEStateNetConfig, miMiscData, 0x018);
CHECK_OFFSET(cFEStateNetConfig, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateNetConfig, mpParentState, 0x020);
CHECK_OFFSET(cFEStateNetConfig, mScreens, 0x024);
CHECK_OFFSET(cFEStateNetConfig, mpScreen, 0x040);
CHECK_OFFSET(cFEStateNetConfig, mUserNum, 0x044);
CHECK_OFFSET(cFEStateNetConfig, unk_045, 0x045);
CHECK_OFFSET(cFEStateNetConfig, unk_048, 0x048);
CHECK_OFFSET(cFEStateNetConfig, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNetConfig, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNetConfig, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNetConfig, unk_058, 0x058);
CHECK_OFFSET(cFEStateNetConfig, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNetConfig, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNetConfig, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateNetConfig, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateNetConfig, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNetConfig, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNetConfig, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNetConfig, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateNetConfig, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateNetConfig, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateNetConfig, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateNetConfig, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateNetConfig, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateNetConfig, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateNetConfig, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateNetConfig, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateNetConfig, miSelectedSetting, 0x6D0);
CHECK_OFFSET(cFEStateNetConfig, miNumSettings, 0x6D4);
CHECK_OFFSET(cFEStateNetConfig, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateNetConfig, unk_6DC, 0x6DC);
CHECK_OFFSET(cFEStateNetConfig, miWaitFrames, 0x6E0);
CHECK_OFFSET(cFEStateNetConfig, mpLBAdapters, 0x6E4);
CHECK_OFFSET(cFEStateNetConfig, mpLBSettings, 0x6E8);
CHECK_OFFSET(cFEStateNetConfig, mpPairSettings, 0x6EC);
CHECK_OFFSET(cFEStateNetConfig, unk_6F0, 0x6F0);
CHECK_OFFSET(cFEStateNetConfig, mpMenuConfig, 0x6F4);
CHECK_OFFSET(cFEStateNetConfig, mpConfigRectangle, 0x6F8);
CHECK_OFFSET(cFEStateNetConfig, mpNoConfiguration, 0x6FC);
CHECK_OFFSET(cFEStateNetConfig, unk_700, 0x700);
CHECK_OFFSET(cFEStateNetConfig, mpustrPopupText, 0x704);
CHECK_OFFSET(cFEStateNetConfig, miState, 0x708);
CHECK_OFFSET(cFEStateNetConfig, miResult, 0x70C);
CHECK_OFFSET(cFEStateNetConfig, mpustrAuthError, 0x710);
CHECK_OFFSET(cFEStateNetConfig, mustrPopupText, 0x71C);
CHECK_SIZE(cFEStateNetConfig, 0x91C);

struct cFEStateNetAccountMem {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateNetAccountMem_vtbl (24 slots), stored by cFEStateNetAccountMem_cFEStateNetAccountMem after cFEBlankPopup's ctor (func_001ABF70, vtable
                                    //       D_0046B6F8) and by the destructor cFEStateNetAccountMem__cFEStateNetAccountMem (which restores D_0046B6F8 before cFEStateNet's)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] 0 (not set by cFEStateNetAccountMem_cFEStateNetAccountMem)
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] passed to every child state's ctor
                                    //       (cMemMan_alloc(..) + ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word
                                    //       (ui/uistate.h); [R] cUIState ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's owner argument; [U] func_001A92D8 calls its slot 17
                                    //       (onPopupEvent)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
                                    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then func_001A8500 stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h); cFEBlankPopup adds no members
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override func_001A87D0 sets 1; func_001DE8A8 (own
                                    //       virtual, slot 29) returns mpScreen != 0 && unk_048 != 0 (the preview's
                                    //       isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] func_001AAB68 (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] func_001AAB68 stores the frame when the session's count
                                    //       (func_00262938) becomes > 0 and 0 when it drops; func_001AA9B0 (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter D_004A5B64 + 0x18)
    unsigned int mChallengeHintFrame; // 0x054 [R] 0; [U] func_001AAB68, the same for the session's other list (the
                                    //       vector at session + 0xBC); func_001AA9B0 blinks "IconChallengeHint" with it
    int unk_058;                    // 0x058 [R] 0; [U] func_001A9668/func_001A9710 set 1 before pushing cFEStateLogin;
                                    //       func_001DE860 (slot 7, the onInputBegin override) returns it
    char mszMaskedText[0x40];       // 0x05C [U] func_001A98B8 writes one '*' per character of its string (at most 0x3F;
                                    //       " " when empty) and a NUL; cUIText_setAsciiString(.., mszMaskedText) in
                                    //       func_001ACDD0, func_001B9610, func_001B9678, func_001B9820 (password boxes)
    unsigned short mustrMessage[0x200]; // 0x09C [U] sprintf1 (EA's unicode sprintf) target that becomes the popup's
                                    //       message (func_001A4E28); extent: up to the next used offset
    unsigned short unk_49C[0x80];   // 0x49C [U] func_001AB478: sprintf1 of a status line, then setPopupConfirmation
                                    //       (func_001C5838, unicode) with it
    unsigned short unk_59C[0x80];   // 0x59C [U] func_001AB478: the second status line, used the same way
    cKeyboardPopup* mpKeyboard;     // 0x69C [R] 0; [U] func_001A91B0 stores cMemMan_alloc(0x444, "cKeyboardPopup") +
                                    //       cKeyboardPopup_cKeyboardPopup and pushes it; func_001A9090/90F0/9150 set its texts;
                                    //       func_001A97B8 (onPopupEvent) compares the closing popup with it
    cScreenPopup* mpPopup;          // 0x6A0 [R] 0; [U] createPopup func_001A9C50: cMemMan_alloc(0x360, "createPopup") +
                                    //       cScreenPopup_cScreenPopup, reused (cPopupInfo::clear) when set; func_001A9BC0
                                    //       shows it; func_001A93D0 clears it when that popup closes (event 22)
    cUIText* mpKeyboardText;        // 0x6A4 [R] 0; [U] func_001A91B0 stores its a1 (the text box the
                                    //       keyboard edits); cUIText_setAsciiString(mpKeyboardText, mszMaskedText)
    int unk_6A8;                    // 0x6A8 [R] 1; 0 in the ctors of cFEUseExistingAccount, cFEStateNetConfig,
                                    //       cFEStateAuthDNAS, cFEStateCreateStep1/2, cFEStateCreateDuplicate, cFEStateLogin;
                                    //       [U] while set, func_001A89B0 (slot 25) drops net messages other than kinds
                                    //       0x31/0x33/0x4D/0x4E and opens the disconnect popup (func_001A9930)
    int unk_6AC;                    // 0x6AC [R] 0; [U] func_001A9930 sets it when it opens the "kT_POPUPCMNNetInterrupted"/
                                    //       "kT_POPUPCMNDisconnected" popup (only when clear); func_001A9668/func_001A9710
                                    //       clear it, func_001A5010 tests it
    int unk_6B0;                    // 0x6B0 [R] 0; [U] set to 1 by func_001DE890 (inline setter), func_001BF5A8,
                                    //       func_001C0138; no read seen
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent func_001A8B88: 1 while a net event (0xC8..0x116)
                                    //       waits for slot 30 to accept it, 0 once delivered
    void* unk_6B8;                  // 0x6B8 [R] 0; [U] a heap text buffer: func_001A9CC0 frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int unk_6BC;                    // 0x6BC [R] 2; [U] func_001A92D8 (popup events): 2 on event 21, 0 on event 15;
                                    //       on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_cBXString4), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateNetAccountMem
    int miResult;                   // 0x6D0 [U] onPopupEvent func_001B84D8: the event (0xF/0x10/0x11) reported to mpParentState's
                                    //       onPopupEvent (slot 17) when the popup closes (0x16)
    int miError;                    // 0x6D4 [R] 0; [U] func_001B70D0 / func_001B7A98 / func_001B84D8 store the card result (2, 3, 6, 0x18 ..)
    int unk_6D8;                    // 0x6D8 [R] 0; [U] func_001B70D0 clears it in states 0/5/7/11/13/19/21 and skips to the next state when
                                    //       it is set (8 -> 9, 22 -> 23); func_001B7A98 / func_001B84D8 set it on errors
    int miState;                    // 0x6DC [U] func_001B70D0(self, state) stores it and sets up the state's popup text; onUpdate
                                    //       func_001B7CB8 and onPopupEvent func_001B84D8 switch on it
    char mszUserName[0x11];         // 0x6E0 [R] strcpy of the constructor's 5th argument (LobbyMan mUserName / tLobbyAccount mName at
                                    //       the call sites), D_00536760 in mode 3; [U] func_001B7778 hands it to the card code (func_00265F18),
                                    //       func_001B7A98 compares it with D_00536760
    char mszPassword[0x11];         // 0x6F1 [R] strcpy of the 6th argument (mPassword), D_00536760 + 0x11 in mode 3; [U] func_00265F68,
                                    //       func_001B7A98 compares it with D_00536760 + 0x11
    unsigned short mustrText[0x201]; // 0x702 [U] func_001B70D0: sprintf1 / strcpy1 (unicode) target of the state's message,
                                    //       passed to the popup (func_001B7650); func_001B7E40 / func_001B8238 / func_001B83C0; extent: up to 0xB04
    int mbCardBusy;                 // 0xB04 [R] 0; [U] func_001B70D0: 1 while a card operation runs (states 8/14/16/22), 0 otherwise;
                                    //       func_001B7E40 tests it
    int unk_B08;                    // 0xB08 [R] 0; [U] func_001B7A98 / func_001B84D8 set it with miError 0x18; func_001B84D8 reads it
    int miStartState;               // 0xB0C [R] by the constructor's mode: 0 -> 0, 1 -> 5, 2 -> 0xB, 3 -> 0x13 (func_001B70D0 starts there);
                                    //       [U] func_001B84D8 restarts from it
};

CHECK_OFFSET(cFEStateNetAccountMem, mpPrev, 0x000);
CHECK_OFFSET(cFEStateNetAccountMem, mpNext, 0x004);
CHECK_OFFSET(cFEStateNetAccountMem, mVtbl, 0x008);
CHECK_OFFSET(cFEStateNetAccountMem, mHashName, 0x00C);
CHECK_OFFSET(cFEStateNetAccountMem, mpEngine, 0x010);
CHECK_OFFSET(cFEStateNetAccountMem, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateNetAccountMem, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateNetAccountMem, unk_016, 0x016);
CHECK_OFFSET(cFEStateNetAccountMem, miMiscData, 0x018);
CHECK_OFFSET(cFEStateNetAccountMem, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateNetAccountMem, mpParentState, 0x020);
CHECK_OFFSET(cFEStateNetAccountMem, mScreens, 0x024);
CHECK_OFFSET(cFEStateNetAccountMem, mpScreen, 0x040);
CHECK_OFFSET(cFEStateNetAccountMem, mUserNum, 0x044);
CHECK_OFFSET(cFEStateNetAccountMem, unk_045, 0x045);
CHECK_OFFSET(cFEStateNetAccountMem, unk_048, 0x048);
CHECK_OFFSET(cFEStateNetAccountMem, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNetAccountMem, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNetAccountMem, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNetAccountMem, unk_058, 0x058);
CHECK_OFFSET(cFEStateNetAccountMem, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNetAccountMem, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNetAccountMem, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateNetAccountMem, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateNetAccountMem, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNetAccountMem, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNetAccountMem, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateNetAccountMem, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateNetAccountMem, miResult, 0x6D0);
CHECK_OFFSET(cFEStateNetAccountMem, miError, 0x6D4);
CHECK_OFFSET(cFEStateNetAccountMem, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateNetAccountMem, miState, 0x6DC);
CHECK_OFFSET(cFEStateNetAccountMem, mszUserName, 0x6E0);
CHECK_OFFSET(cFEStateNetAccountMem, mszPassword, 0x6F1);
CHECK_OFFSET(cFEStateNetAccountMem, mustrText, 0x702);
CHECK_OFFSET(cFEStateNetAccountMem, mbCardBusy, 0xB04);
CHECK_OFFSET(cFEStateNetAccountMem, unk_B08, 0xB08);
CHECK_OFFSET(cFEStateNetAccountMem, miStartState, 0xB0C);
CHECK_SIZE(cFEStateNetAccountMem, 0xB10);

#endif /* FE_FESTATENETCONFIG_H */
