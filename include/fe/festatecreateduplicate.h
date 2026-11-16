#ifndef FE_FESTATECREATEDUPLICATE_H
#define FE_FESTATECREATEDUPLICATE_H

// cFEStateCreateDuplicate: screen "212create_duplicate": the server refused a new account name (mbAccount) or persona
// name; it lists the server's suggested names (tLobbyAccount mNameOptions) and lets the player pick one or type
// another, then resubmits. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the preview's
// __23cFEStateCreateDuplicate / _$_23cFEStateCreateDuplicate. EA's source file is probably
// d:/ssx/code/source/fe/festatenetcreate.cpp (its code follows cFEStateCreateStep2's, see fe/festatenetcreate.h, in
// both builds; no assert of its own), so this header could also be merged into fe/festatenetcreate.h.
//
// Size 0x6E0: cMemMan_alloc(0x6E0, "cFEStateCreateDuplicate") + cFEStateCreateDuplicate_cFEStateCreateDuplicate(mem, pEngine, bAccount) at both sites
// (func_001ADC30 with 0, func_001BAC90). Constructors cFEStateCreateDuplicate_cFEStateCreateDuplicate (cFEStateNet(pEngine, 0), vtable cFEStateCreateDuplicate_vtbl,
// mHashName 0x4F, unk_6A8 = 0) and the default cFEStateCreateDuplicate_cFEStateCreateDuplicate1 (fe/fepopupmisc); destructor cFEStateCreateDuplicate__cFEStateCreateDuplicate
// (fe/fepopupmisc;
// slot 1; the preview's _$_23cFEStateCreateDuplicate). Retail vtable cFEStateCreateDuplicate_vtbl (32 slots: 4 onCreateScreen
// cFEStateCreateDuplicate_onCreateScreen, 12 onUpdate cFEStateCreateDuplicate_onUpdate, 17 onPopupEvent cFEStateCreateDuplicate_onPopupEvent, 18 onWidgetCreate cFEStateCreateDuplicate_onWidgetCreate,
// 19 onWidgetEvent cFEStateCreateDuplicate_onWidgetEvent, 25 func_001BC190 (help), 29 func_001E0E40, 31 func_001BC4F0 (net events)).
// Methods: fe/feasyncfile 0x1BBD08-0x1BC990.
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

struct tLobbyAccount;

struct cFEStateCreateDuplicate {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateCreateDuplicate_vtbl (vtables.tsv; 32 slots), stored by cFEStateCreateDuplicate_cFEStateCreateDuplicate /
                                    //       cFEStateCreateDuplicate_cFEStateCreateDuplicate1 after the cFEStateNet ctor and by the destructor
                                    //       cFEStateCreateDuplicate__cFEStateCreateDuplicate
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateCreateDuplicate_cFEStateCreateDuplicate stores
                                    //       0x4F
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
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override cFEStateNet_onGainFocus sets 1; func_001DE8A8 (own
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
                                    //       cFEStateNet_onPopupEvent (onPopupEvent) compares the closing popup with it
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
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_cBXString4), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateCreateDuplicate
    int mbAccount;                  // 0x6D0 [R] the ctor's 3rd argument (0 from cFEStateSelectPersona func_001ADC30:
                                    //       a persona name; func_001BAC90: the account name); [U] onWidgetCreate
                                    //       cFEStateCreateDuplicate_onWidgetCreate / onWidgetEvent cFEStateCreateDuplicate_onWidgetEvent / help func_001BC190 pick the
                                    //       account or persona texts and actions by it
    int mbAskSave;                  // 0x6D4 [U] func_001BC990 (popup 0x82C, the cFEStateNetAccountMem result): 1 on
                                    //       0x11, 0 on 0xF/0x10/0x14; on 0x16 it opens "kT_POPUP204SaveAccount"
                                    //       (0x81F) when set, else verifies the account (func_0025C610); then clears
                                    //       it
    tLobbyAccount* mpAccount;       // 0x6D8 [U] onCreateScreen cFEStateCreateDuplicate_onCreateScreen stores (char*)LobbyMan_mLobbyMan + 0x260
                                    //       (the new-account form, online/lobbyman.h); func_001BC400 shows its
                                    //       mNameOptions[0..mNumNameOptions) in "SelectName0".."3"; onWidgetEvent
                                    //       assigns the chosen option or the typed name to its mName; func_001BC818 /
                                    //       func_001BC990 pass mName / mPassword on
    int unk_6DC;                    // 0x6DC no access seen (inside the 0x6E0 allocation)
};

CHECK_OFFSET(cFEStateCreateDuplicate, mpPrev, 0x000);
CHECK_OFFSET(cFEStateCreateDuplicate, mpNext, 0x004);
CHECK_OFFSET(cFEStateCreateDuplicate, mVtbl, 0x008);
CHECK_OFFSET(cFEStateCreateDuplicate, mHashName, 0x00C);
CHECK_OFFSET(cFEStateCreateDuplicate, mpEngine, 0x010);
CHECK_OFFSET(cFEStateCreateDuplicate, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateCreateDuplicate, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_016, 0x016);
CHECK_OFFSET(cFEStateCreateDuplicate, miMiscData, 0x018);
CHECK_OFFSET(cFEStateCreateDuplicate, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateCreateDuplicate, mpParentState, 0x020);
CHECK_OFFSET(cFEStateCreateDuplicate, mScreens, 0x024);
CHECK_OFFSET(cFEStateCreateDuplicate, mpScreen, 0x040);
CHECK_OFFSET(cFEStateCreateDuplicate, mUserNum, 0x044);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_045, 0x045);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_048, 0x048);
CHECK_OFFSET(cFEStateCreateDuplicate, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateCreateDuplicate, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateCreateDuplicate, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_058, 0x058);
CHECK_OFFSET(cFEStateCreateDuplicate, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateCreateDuplicate, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateCreateDuplicate, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateCreateDuplicate, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateCreateDuplicate, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateCreateDuplicate, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateCreateDuplicate, mbAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateDuplicate, mbAskSave, 0x6D4);
CHECK_OFFSET(cFEStateCreateDuplicate, mpAccount, 0x6D8);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6DC, 0x6DC);
CHECK_SIZE(cFEStateCreateDuplicate, 0x6E0);

#endif /* FE_FESTATECREATEDUPLICATE_H */
