#ifndef FE_FESTATENETCHALLENGE_H
#define FE_FESTATENETCHALLENGE_H

// The online challenge popups (EA's d:/ssx/code/source/fe/festatenetchallenge.cpp: the preview's assert sits in
// cFEStateChallengePopup's onWidgetEvent, retail cFEStateChallengePopup_onWidgetEvent; cFEStateChallengeReceivedPopup's code follows it in
// both builds, so it is placed here too). Both are cFEStateNet states (fe/festatenet.h, 0x6D0) pushed by the
// cFEStateNet method func_001ABCD0 (a challenge from / to LobbyMan's mOpponent):
//
// cFEStateChallengePopup: screen "240challenge_popup": sets up the race options of a challenge (ranked game, CPU
// riders, power-ups, multipliers, point icons, event) and sends it. EA's name: the preview's
// __22cFEStateChallengePopup,
// _$_22cFEStateChallengePopup and onLoseTransition__22cFEStateChallengePopup. Size 0x6F0: cMemMan_alloc(0x6F0,
// "cFEStateChallengePopup") + cFEStateChallengePopup_cFEStateChallengePopup(mem, pEngine, pParent) (func_001ABCD0, the only site). Constructors
// cFEStateChallengePopup_cFEStateChallengePopup (cFEStateNet(pEngine, pParent), vtable cFEStateChallengePopup_vtbl, mHashName 0x3F; resets the new-race interface) and
// the default func_001E0950 (fe/fepopupmisc); destructor cFEStateChallengePopup__cFEStateChallengePopup (fe/fepopupmisc; slot 1: calls mpParentState's
// slot 32, then cFEStateNet's func_001A85D0). Retail vtable cFEStateChallengePopup_vtbl (33 slots; vtables.tsv lists 24: 4
// onCreateScreen cFEStateChallengePopup_onCreateScreen,
// 6 onGainFocus cFEStateChallengePopup_onGainFocus, 7 cFEStateChallengePopup_onGainTransition, 10 cFEStateChallengePopup_onLoseTransition, 12 onUpdate cFEStateChallengePopup_onUpdate, 17 onPopupEvent
// cFEStateChallengePopup_onPopupEvent, 18 onWidgetCreate cFEStateChallengePopup_onWidgetCreate, 19 onWidgetEvent cFEStateChallengePopup_onWidgetEvent, 20 cFEStateChallengePopup_onWidgetMenu, 31 func_001AF470
// (net events), 32 func_001AEBF0, 33 func_001AF3E0).
// Methods: fe/feasyncfile 0x1ADDD0-0x1AFD88.
//
// cFEStateChallengeReceivedPopup: screen "241challenge_received_popup": shows a received challenge's options and
// accepts / declines it. EA's name: the preview's __30cFEStateChallengeReceivedPopup, _$_30...,
// onGainTransition__30...
// and onLoseTransition__30.... Size 0x6D8: cMemMan_alloc(0x6D8, "cFEStateChallengeReceivedPopup") +
// cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup(mem,
// pEngine, pParent, szChallenger) (func_001ABCD0, the only site). Constructors cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup (vtable cFEStateChallengeReceivedPopup_vtbl,
// mHashName 0x40) and the default func_001E0A08 (fe/fepopupmisc); destructor cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup (fe/fepopupmisc; slot 1:
// mpParentState's slot 32, destroys mszChallenger, cFEStateNet's dtor). Retail vtable cFEStateChallengeReceivedPopup_vtbl (33 slots;
// vtables.tsv lists 24: 4
// onCreateScreen cFEStateChallengeReceivedPopup_onCreateScreen, 7 cFEStateChallengeReceivedPopup_onGainTransition, 10 cFEStateChallengeReceivedPopup_onLoseTransition, 12 onUpdate cFEStateChallengeReceivedPopup_onUpdate, 17 onPopupEvent
// cFEStateChallengeReceivedPopup_onPopupEvent, 18 onWidgetCreate cFEStateChallengeReceivedPopup_onWidgetCreate, 19 onWidgetEvent cFEStateChallengeReceivedPopup_onWidgetEvent, 20 cFEStateChallengeReceivedPopup_onWidgetMenu, 24 handleEvent
// func_001A8B88, 31 func_001B0708 (net events), 32 func_001B00B8, 33 func_001B0628). Methods: fe/feasyncfile
// 0x1AFE08-0x1B0F28.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). There is no On Tour DWARF for
// these classes (On Tour has no online FE), so the own fields are named from their retail use unless noted.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (its default constructors, destructors and inline accessors, see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (onPopupEvent / the cFEStateNet popup handlers): 0xF + i = option i chosen, 0x14 = back, 0x16 = the
// popup closed (fe/screenpopup.h). The popup id is the cScreenPopup's miMiscData (+ 0x18), set to 0x8xx by its
// creator.

#include "layout.h"
#include "fe/festatenet.h"

struct cUIListBox;

struct cFEStateChallengePopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateChallengePopup_vtbl (33 slots), stored by cFEStateChallengePopup_cFEStateChallengePopup / func_001E0950 after
                                    //       the cFEStateNet ctor and by the destructor cFEStateChallengePopup__cFEStateChallengePopup
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateChallengePopup_cFEStateChallengePopup stores
                                    //       0x3F
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
    // --- cFEStateChallengePopup
    int mbChallengeSent;            // 0x6D0 [R] 0 (both ctors); [U] func_001AF568 sets 1 when the challenge goes out
                                    //       ("staticTextSetchallenge" = "kT_240WaitingChallenge", options locked:
                                    //       func_001AEC60(self, 1)), func_001AF688 sets 0
                                    //       ("kT_241SetChallengeOptions"); onWidgetEvent cFEStateChallengePopup_onWidgetEvent ignores
                                    //       "submit" (9) while it is set and passes it to func_0025F628 (cancel);
                                    //       onGainFocus cFEStateChallengePopup_onGainFocus re-locks the options with it
    int mbFirstCreate;              // 0x6D4 [R] 1 (cFEStateChallengePopup_cFEStateChallengePopup, after resetting the race interface); [U]
                                    //       onCreateScreen cFEStateChallengePopup_onCreateScreen clears it on the first screen, and selects
                                    //       "MenuChallenge" row 6 when it is already 0
    int* mpEventOptions;            // 0x6D8 [R] D_00441268 (both ctors); [U] onGainFocus cFEStateChallengePopup_onGainFocus points it at
                                    //       one of six 4-word tables (D_00441268 .. D_004412B8) by the event kind
                                    //       D_00535C12: [0] CPU riders, [1] power-ups, [2] point icons, [3]
                                    //       multipliers allowed (func_001AEC60, func_001AF098)
    cUIListBox* mpLBRankedGame;     // 0x6DC [R] 0 (cFEStateChallengePopup_cFEStateChallengePopup); [U] onWidgetCreate cFEStateChallengePopup_onWidgetCreate: widget
                                    //       "ListBoxRankedGame" (yes / no); func_001AE680 / onWidgetEvent read its
                                    //       value (cUIListBox_getCurrentEntryData) into D_00534B34; onGainFocus sets it
                                    //       (cUIListBox_setCurrentEntryByIndex)
    cUIListBox* mpLBCPURiders;      // 0x6E0 [U] onWidgetCreate: widget "ListBoxCPURiders" (none, 1-4); its value is
                                    //       the new-race interface's number of AI riders
                                    //       (cBENewRaceInterface_setNumberAI); func_001AF728 reads its slot 8
    cUIListBox* mpLBPowerUps;       // 0x6E4 [U] onWidgetCreate: widget "ListBoxPowerUps"; its value is race-option
                                    //       bit 8 of D_005308B8 (func_001AE680, onWidgetEvent)
    cUIListBox* mpLBMultipliers;    // 0x6E8 [U] onWidgetCreate: widget "ListBoxMultipliers"; race-option bit 6 of
                                    //       D_005308B8
    cUIListBox* mpLBPointIcons;     // 0x6EC [U] onWidgetCreate: widget "ListBoxPointicons"; race-option bit 7 of
                                    //       D_005308B8
};

CHECK_OFFSET(cFEStateChallengePopup, mpPrev, 0x000);
CHECK_OFFSET(cFEStateChallengePopup, mpNext, 0x004);
CHECK_OFFSET(cFEStateChallengePopup, mVtbl, 0x008);
CHECK_OFFSET(cFEStateChallengePopup, mHashName, 0x00C);
CHECK_OFFSET(cFEStateChallengePopup, mpEngine, 0x010);
CHECK_OFFSET(cFEStateChallengePopup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateChallengePopup, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateChallengePopup, unk_016, 0x016);
CHECK_OFFSET(cFEStateChallengePopup, miMiscData, 0x018);
CHECK_OFFSET(cFEStateChallengePopup, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateChallengePopup, mpParentState, 0x020);
CHECK_OFFSET(cFEStateChallengePopup, mScreens, 0x024);
CHECK_OFFSET(cFEStateChallengePopup, mpScreen, 0x040);
CHECK_OFFSET(cFEStateChallengePopup, mUserNum, 0x044);
CHECK_OFFSET(cFEStateChallengePopup, unk_045, 0x045);
CHECK_OFFSET(cFEStateChallengePopup, unk_048, 0x048);
CHECK_OFFSET(cFEStateChallengePopup, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateChallengePopup, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateChallengePopup, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateChallengePopup, unk_058, 0x058);
CHECK_OFFSET(cFEStateChallengePopup, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateChallengePopup, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateChallengePopup, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateChallengePopup, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateChallengePopup, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateChallengePopup, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateChallengePopup, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateChallengePopup, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateChallengePopup, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateChallengePopup, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateChallengePopup, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateChallengePopup, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateChallengePopup, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateChallengePopup, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateChallengePopup, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateChallengePopup, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateChallengePopup, mbChallengeSent, 0x6D0);
CHECK_OFFSET(cFEStateChallengePopup, mbFirstCreate, 0x6D4);
CHECK_OFFSET(cFEStateChallengePopup, mpEventOptions, 0x6D8);
CHECK_OFFSET(cFEStateChallengePopup, mpLBRankedGame, 0x6DC);
CHECK_OFFSET(cFEStateChallengePopup, mpLBCPURiders, 0x6E0);
CHECK_OFFSET(cFEStateChallengePopup, mpLBPowerUps, 0x6E4);
CHECK_OFFSET(cFEStateChallengePopup, mpLBMultipliers, 0x6E8);
CHECK_OFFSET(cFEStateChallengePopup, mpLBPointIcons, 0x6EC);
CHECK_SIZE(cFEStateChallengePopup, 0x6F0);

struct cFEStateChallengeReceivedPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateChallengeReceivedPopup_vtbl (33 slots), stored by cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup / func_001E0A08 after
                                    //       the cFEStateNet ctor and by the destructor cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup stores
                                    //       0x40
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
    // --- cFEStateChallengeReceivedPopup
    char* mszChallenger;            // 0x6D0 [R] cBXString: built from the ctor's 4th argument (cBXString_cBXString2),
                                    //       the empty rep D_004A3E90 in the default ctor, destroyed by cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup;
                                    //       [U] onCreateScreen cFEStateChallengeReceivedPopup_onCreateScreen looks the challenge up by it
                                    //       (func_00263270(BuddyMan, ..)) and copies it to LobbyMan (+ 0x98);
                                    //       func_001B00B8 (func_00262768) and func_001B0708 (net event 0xEF,
                                    //       func_00263018) pass its text
    int mbAccepted;                 // 0x6D4 [R] 0 (cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup; not set by the default ctor); [U] func_001B0C08
                                    //       sets 1 when the challenge is accepted ("TextAccept" enabled,
                                    //       "kT_OVRCMNWaitPlayer"), func_001B0D30 sets 0; onWidgetEvent cFEStateChallengeReceivedPopup_onWidgetEvent
                                    //       accepts (row 0xB) only while it is 0; func_001B0E10 (decline) cancels the
                                    //       accepted game (func_0025F740) when it is set
};

CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpPrev, 0x000);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpNext, 0x004);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mVtbl, 0x008);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mHashName, 0x00C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpEngine, 0x010);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_016, 0x016);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, miMiscData, 0x018);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpParentState, 0x020);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mScreens, 0x024);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpScreen, 0x040);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mUserNum, 0x044);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_045, 0x045);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_048, 0x048);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_058, 0x058);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mszChallenger, 0x6D0);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mbAccepted, 0x6D4);
CHECK_SIZE(cFEStateChallengeReceivedPopup, 0x6D8);

#endif /* FE_FESTATENETCHALLENGE_H */
