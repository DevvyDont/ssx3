#ifndef FE_FESTATENETQUICKPLAYPOPUP_H
#define FE_FESTATENETQUICKPLAYPOPUP_H

// cFEStateNetQuickPlayPopup: the "quick match" popup of the online main menu: five yes/no options (ranked, race, big
// air, superpipe, slopestyle) and an OK / Cancel button that starts or aborts the match search. A cFEStateNet state
// (fe/festatenet.h, 0x6D0). EA's name: the preview's __25cFEStateNetQuickPlayPopup, _$_25cFEStateNetQuickPlayPopup
// and
// onLoseTransition__25cFEStateNetQuickPlayPopup. EA's source file is not known (its code follows
// cFEStateOnlineAgreement's, after cFEStateOnlineMainMenu's festatenetmainmenu.cpp, in both builds).
//
// Size 0x734: cMemMan_alloc(0x734, "cFEStateNetQuickPlayPopup") + cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup(mem, pEngine, pParent)
// (cFEStateOnlineMainMenu_onWidgetEvent,
// cFEStateOnlineMainMenu's onWidgetEvent; the only site); the constructor zeroes up to 0x730. Constructors
// cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup (cFEStateNet(pEngine, pParent), vtable cFEStateNetQuickPlayPopup_vtbl, mHashName 0x48) and the default cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1
// (fe/fepopupmisc); destructor cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup (fe/fepopupmisc; slot 1; the preview's _$_25cFEStateNetQuickPlayPopup).
// Retail vtable cFEStateNetQuickPlayPopup_vtbl (32 slots: 4 onCreateScreen cFEStateNetQuickPlayPopup_onCreateScreen, 10 cFEStateNetQuickPlayPopup_onLoseTransition, 12
// onUpdate cFEStateNetQuickPlayPopup_onUpdate, 17 onPopupEvent cFEStateNetQuickPlayPopup_onPopupEvent, 18 onWidgetCreate cFEStateNetQuickPlayPopup_onWidgetCreate, 19 onWidgetEvent
// cFEStateNetQuickPlayPopup_onWidgetEvent, 20 cFEStateNetQuickPlayPopup_onWidgetMenu, 21 cFEStateNetQuickPlayPopup_onWidgetPair, 25 func_001C1488 (help), 31 func_001C19C8 (net events)).
// Methods: fe/feasyncfile 0x1C0F40-0x1C1B88.
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

struct cUIThing;
struct cUIText;
struct cUIListBox;

struct cFEStateNetQuickPlayPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateNetQuickPlayPopup_vtbl (vtables.tsv; 32 slots), stored by cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup /
                                    //       cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1 after the cFEStateNet ctor and by the destructor
                                    //       cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup stores
                                    //       0x48
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
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h)
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override cFEStateNet_onGainFocus sets 1; cFEStateNet_isProcessingEvents (own
                                    //       virtual, slot 29) returns mpScreen != 0 && unk_048 != 0 (the preview's
                                    //       isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] func_001AAB68 (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] func_001AAB68 stores the frame when the session's count
                                    //       (func_00262938) becomes > 0 and 0 when it drops; func_001AA9B0 (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter cAppMan_TheAppMan + 0x18)
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
                                    //       cFEStateNet_onPopupEvent compares the closing popup with it
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
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent cFEStateNet_handleEvent: 1 while a net event (0xC8..0x116)
                                    //       waits for slot 30 to accept it, 0 once delivered
    void* unk_6B8;                  // 0x6B8 [R] 0; [U] a heap text buffer: func_001A9CC0 frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int unk_6BC;                    // 0x6BC [R] 2; [U] func_001A92D8 (popup events): 2 on event 21, 0 on event 15;
                                    //       on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from str_sp (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_operatorE1), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateNetQuickPlayPopup
    int miSearchOptions[5];         // 0x6D0 [U] onWidgetEvent cFEStateNetQuickPlayPopup_onWidgetEvent stores the five list boxes' values ([0]
                                    //       mpLBRanked .. [4] mpLBSlopeStyle) when the search starts and passes the
                                    //       array to func_0025EE20(LobbyMan, miSearchOptions, mCurrentCtrl)
    cUIText* mpTextSearching;       // 0x6E4 [U] onWidgetCreate cFEStateNetQuickPlayPopup_onWidgetCreate: widget "TextSearching" (hidden); shown
                                    //       while searching, "kT_233AbortingQuickmatch" on abort; func_001C1B88 hides
                                    //       it
    int miBlinkFrames;              // 0x6E8 [U] onWidgetEvent sets 30 when the search starts; onUpdate cFEStateNetQuickPlayPopup_onUpdate
                                    //       counts it down while mbSearching and wraps to 30
    int mbSearching;                // 0x6EC [R] 0 (both ctors); [U] onWidgetEvent: 1 when OK starts the search, the
                                    //       OK button then aborts it (func_0025EED8); func_001C1B88 (popup 0xF/0x14)
                                    //       and the net-event handler func_001C19C8 (0xE7) clear it; message 7 passes
                                    //       to cFEStateNet only while it is 0
    int unk_6F0;                    // 0x6F0 [U] onWidgetEvent clears it when the search starts; the net-event handler
                                    //       func_001C19C8 sets 1 on 0xE6 and, on 0xE7, opens
                                    //       "kT_POPUP233QuickMatchTimeout" when set, else ends the search
    cUIText* mpTextOK;              // 0x6F4 [U] onWidgetCreate: widget "TextOK"; onWidgetEvent compares the selected
                                    //       widget with it and relabels it "kT_BTNCancel" while searching ("kT_BTNOK"
                                    //       again in func_001C1B88)
    cUIThing* mpStaticRanked;       // 0x6F8 [R] 0; [U] onWidgetCreate: widget "StaticRanked"; func_001C17F8(self, on)
                                    //       calls slot 8 of this and the 14 widgets after it
    cUIThing* mpStaticRace;         // 0x6FC [R] 0; [U] widget "StaticRace"
    cUIThing* mpStaticBigAir;       // 0x700 [R] 0; [U] widget "StaticBigAir"
    cUIThing* mpStaticSuperpipe;    // 0x704 [R] 0; [U] widget "StaticSuperpipe"
    cUIThing* mpStaticSlopeStyle;   // 0x708 [R] 0; [U] widget "StaticSlopeStyle"
    cUIListBox* mpLBRanked;         // 0x70C [R] 0; [U] widget "LBRanked" (yes / no, func_001A9AB8); its value is
                                    //       miSearchOptions[0] and D_00534B34 (ranked)
    cUIListBox* mpLBRace;           // 0x710 [R] 0; [U] widget "LBRace"; miSearchOptions[1]
    cUIListBox* mpLBBigAir;         // 0x714 [R] 0; [U] widget "LBBigAir"; miSearchOptions[2]
    cUIListBox* mpLBSuperpipe;      // 0x718 [R] 0; [U] widget "LBSuperpipe"; miSearchOptions[3]
    cUIListBox* mpLBSlopeStyle;     // 0x71C [R] 0; [U] widget "LBSlopeStyle"; miSearchOptions[4]
    cUIThing* mpRankedPair;         // 0x720 [R] 0; [U] widget "RankedPair"
    cUIThing* mpRacePair;           // 0x724 [R] 0; [U] widget "RacePair"
    cUIThing* mpBigAirPair;         // 0x728 [R] 0; [U] widget "BigAirPair"
    cUIThing* mpSuperpipePair;      // 0x72C [R] 0; [U] widget "SuperpipePair"
    cUIThing* mpSlopestylePair;     // 0x730 [R] 0; [U] widget "SlopestylePair"
};

CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpPrev, 0x000);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpNext, 0x004);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mVtbl, 0x008);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mHashName, 0x00C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpEngine, 0x010);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_016, 0x016);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, miMiscData, 0x018);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpParentState, 0x020);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mScreens, 0x024);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpScreen, 0x040);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mUserNum, 0x044);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_045, 0x045);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_048, 0x048);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_058, 0x058);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, miSearchOptions, 0x6D0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpTextSearching, 0x6E4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, miBlinkFrames, 0x6E8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mbSearching, 0x6EC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, unk_6F0, 0x6F0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpTextOK, 0x6F4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticRanked, 0x6F8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticRace, 0x6FC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticBigAir, 0x700);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticSuperpipe, 0x704);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticSlopeStyle, 0x708);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBRanked, 0x70C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBRace, 0x710);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBBigAir, 0x714);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBSuperpipe, 0x718);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBSlopeStyle, 0x71C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpRankedPair, 0x720);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpRacePair, 0x724);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpBigAirPair, 0x728);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpSuperpipePair, 0x72C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpSlopestylePair, 0x730);
CHECK_SIZE(cFEStateNetQuickPlayPopup, 0x734);

#endif /* FE_FESTATENETQUICKPLAYPOPUP_H */
