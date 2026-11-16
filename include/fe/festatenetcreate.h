#ifndef FE_FESTATENETCREATE_H
#define FE_FESTATENETCREATE_H

// The online account-creation screens (EA's d:/ssx/code/source/fe/festatenetcreate.cpp: the preview's assert at line
// 1102 sits in cFEStateCreateStep2's onPopupEvent, retail func_001BAA88; Step1's code comes right before it in
// link order). Both are cFEStateNet states (fe/festatenet.h, 0x6D0) that edit a LobbyMan account form
// (tLobbyAccount, online/lobbyman.h). Two classes live here:
//
// cFEStateCreateStep1: screen "210create_step1": account name, password, confirmation, e-mail and the two
// "inform me" / "share info" yes-no boxes. Size 0x6F4: cFEStateCreateStep1 is allocated with
// cMemMan_alloc(0x6F4, "cFEStateCreateStep1") (func_001AD5E8, func_001BA618, func_001BF758, func_001C0D28) and
// constructed by func_001B88F0(mem, pEngine, bEdit); every own field ends below 0x6F4. Constructors func_001B88F0
// and the default func_001E0CA0 (fe/fepopupmisc; the preview's __19cFEStateCreateStep1), destructor func_001E0CF8
// (fe/fepopupmisc; the preview's _$_19cFEStateCreateStep1). Retail vtable D_004686C0 (32 slots: 1 dtor, 4
// onCreateScreen func_001B8960, 7 onGainTransition func_001B8A08, 12 onUpdate func_001B89E8, 17 onPopupEvent
// func_001B9428, 18 onWidgetCreate func_001B8AA0, 19 onWidgetEvent func_001B8C38, 25 func_001B9088 (help text),
// 29 func_001E0D60, 31 func_001B9488 (net event 0xCE)). Methods: fe/feasyncfile 0x1B88F0-0x1BA358.
//
// cFEStateCreateStep2: screen "211create_step2": birth date, gender and the spam option of the same form. Size
// 0x6E0: cMemMan_alloc(0x6E0, "cFEStateCreateStep2") + func_001BA3E0(mem, pEngine) (func_001B8C38, func_001C0D28);
// own fields end below 0x6E0. Constructors func_001BA3E0 and the default func_001E0D68 (fe/fepopupmisc),
// destructor func_001E0DA8 (fe/fepopupmisc; the preview's _$_19cFEStateCreateStep2). Retail vtable D_004685B0
// (slots 4 onCreateScreen func_001BA428, 12 onUpdate func_001BA4E0, 17 onPopupEvent func_001BAA88, 18
// onWidgetCreate func_001BA500, 19 onWidgetEvent func_001BA618, 25 func_001BA8B8, 29 func_001E0DD0, 31
// func_001BAB28). Methods: fe/feasyncfile 0x1BA3E0-0x1BBC40.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). There is no On Tour DWARF for
// these classes (On Tour has no online FE) and the preview keeps no accessors (tools/layout_xlate.py: preview
// 0xC4/0xC8 = retail 0x6EC/0x6F0 for Step1, a +0x628 shift; Step2 +0x624), so the own fields are named from their
// retail use. Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (cScreenPopup_onPopupEvent / onWidgetEvent): 0xF + i = option i chosen, 0x14 = back, 0x16 = the popup
// closed (fe/screenpopup.h).

#include "layout.h"
#include "fe/festatenet.h"

struct tLobbyAccount;
struct cUIText;

struct cFEStateCreateStep1 {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_004686C0 (32 slots), stored by func_001B88F0 / func_001E0CA0 after the cFEStateNet ctor
                                    //       and by the destructor func_001E0CF8
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] func_001B88F0 stores 0x4D (edit) or 0x50 (create)
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
                                    //       func_001CB030 and pushes it; func_001A9090/90F0/9150 set its texts;
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
    // --- cFEStateCreateStep1
    tLobbyAccount* mpAccount;       // 0x6D0 [U] the account form being edited: onCreateScreen func_001B8960 stores
                                    //       (char*)LobbyMan_mLobbyMan + 0x2B0 when mbEditAccount, else + 0x260 (online/lobbyman.h); every text box
                                    //       reads / writes its mName/mPassword/unk_08/mMail/mSpam fields (func_001B95C0..func_001B97B0, func_001B9820)
    char* mszSavedPassword;         // 0x6D4 [R] cBXString (func_001B88F0 stores the empty-string rep D_004A3E90, func_001E0CF8
                                    //       destroys it); [U] func_001BA178 saves mpAccount->mPassword here when editing, func_001BA218 restores it,
                                    //       func_001BA358 sets mpAccount->mChangePassword when it differs
    char* mszSavedConfirm;          // 0x6D8 [R] cBXString (as above); [U] the saved mpAccount->unk_08 (func_001BA178 / func_001BA218)
    char* mszSavedMail;             // 0x6DC [R] cBXString (as above); [U] the saved mpAccount->mMail; func_001BA358 sets mChangeMail when
                                    //       it differs
    int miSavedSpam[2];             // 0x6E0 [U] func_001BA178 saves mpAccount->mSpam[0..1], func_001BA218 restores them, func_001BA358
                                    //       sets mpAccount->mChangeSpam when either differs
    int unk_6E8;                    // 0x6E8 [U] func_001B92C8 (popup 0x82C events): 1 on 0x11 (option 2), 0 on 0xF/0x10/0x14; on 0x16
                                    //       it opens the "kT_POPUP204SaveAccount" yes/no popup when set, else pushes cFEStateSelectPersona
    int mbEditAccount;              // 0x6EC [R] func_001B88F0's 3rd argument (mHashName 0x4D when set, 0x50 when not), 1 in the default
                                    //       ctor; [U] onCreateScreen picks the form from it, onWidgetCreate/onWidgetEvent/func_001BA178 test it
    cUIText* mpStaticContinue;      // 0x6F0 [R] 0; [U] onWidgetCreate func_001B8AA0: widget "StaticContinue"; onGainTransition
                                    //       func_001B8A08 enables it (its slot 8) only when all four strings are set (func_001B9558)
};

CHECK_OFFSET(cFEStateCreateStep1, mpPrev, 0x000);
CHECK_OFFSET(cFEStateCreateStep1, mpNext, 0x004);
CHECK_OFFSET(cFEStateCreateStep1, mVtbl, 0x008);
CHECK_OFFSET(cFEStateCreateStep1, mHashName, 0x00C);
CHECK_OFFSET(cFEStateCreateStep1, mpEngine, 0x010);
CHECK_OFFSET(cFEStateCreateStep1, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateCreateStep1, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateCreateStep1, unk_016, 0x016);
CHECK_OFFSET(cFEStateCreateStep1, miMiscData, 0x018);
CHECK_OFFSET(cFEStateCreateStep1, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateCreateStep1, mpParentState, 0x020);
CHECK_OFFSET(cFEStateCreateStep1, mScreens, 0x024);
CHECK_OFFSET(cFEStateCreateStep1, mpScreen, 0x040);
CHECK_OFFSET(cFEStateCreateStep1, mUserNum, 0x044);
CHECK_OFFSET(cFEStateCreateStep1, unk_045, 0x045);
CHECK_OFFSET(cFEStateCreateStep1, unk_048, 0x048);
CHECK_OFFSET(cFEStateCreateStep1, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateCreateStep1, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateCreateStep1, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateCreateStep1, unk_058, 0x058);
CHECK_OFFSET(cFEStateCreateStep1, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateCreateStep1, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateCreateStep1, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateCreateStep1, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateCreateStep1, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateCreateStep1, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateCreateStep1, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateCreateStep1, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateCreateStep1, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateCreateStep1, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateCreateStep1, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateCreateStep1, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateCreateStep1, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateCreateStep1, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateCreateStep1, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateCreateStep1, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateCreateStep1, mpAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedPassword, 0x6D4);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedConfirm, 0x6D8);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedMail, 0x6DC);
CHECK_OFFSET(cFEStateCreateStep1, miSavedSpam, 0x6E0);
CHECK_OFFSET(cFEStateCreateStep1, unk_6E8, 0x6E8);
CHECK_OFFSET(cFEStateCreateStep1, mbEditAccount, 0x6EC);
CHECK_OFFSET(cFEStateCreateStep1, mpStaticContinue, 0x6F0);
CHECK_SIZE(cFEStateCreateStep1, 0x6F4);

struct cFEStateCreateStep2 {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_004685B0 (32 slots), stored by func_001BA3E0 / func_001E0D68 and the destructor func_001E0DA8
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] func_001BA3E0 stores 0x4E
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
                                    //       func_001CB030 and pushes it; func_001A9090/90F0/9150 set its texts;
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
    // --- cFEStateCreateStep2
    tLobbyAccount* mpAccount;       // 0x6D0 [U] onCreateScreen func_001BA428 stores (char*)LobbyMan_mLobbyMan + 0x260 (online/lobbyman.h:
                                    //       the new-account form); the boxes read/write its mBornMonth/Day/Year (0x28/0x2C/0x30), mGender 0x34, 0x40
                                    //       (func_001BB2D0..func_001BB4B0, func_001BA618, func_001BB840); func_001BAED8 sends mName/mPassword
    int mbScreenReady;              // 0x6D4 [U] onCreateScreen func_001BA428 clears it first and sets it once every box is filled;
                                    //       onWidgetEvent func_001BA618 stores box changes into mpAccount only while it is set
    int unk_6D8;                    // 0x6D8 [U] func_001BAED8 (popup events): 1 on 0x11 (option 2), 0 on 0xF/0x10/0x14; on 0x16 it opens
                                    //       "kT_POPUP204SaveAccount" when set, else "kT_POPUP201Verify" and submits the account (func_0025C610)
    int unk_6DC;                    // 0x6DC [U] onCreateScreen func_001BA428 stores 0; no read seen
};

CHECK_OFFSET(cFEStateCreateStep2, mpPrev, 0x000);
CHECK_OFFSET(cFEStateCreateStep2, mpNext, 0x004);
CHECK_OFFSET(cFEStateCreateStep2, mVtbl, 0x008);
CHECK_OFFSET(cFEStateCreateStep2, mHashName, 0x00C);
CHECK_OFFSET(cFEStateCreateStep2, mpEngine, 0x010);
CHECK_OFFSET(cFEStateCreateStep2, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateCreateStep2, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateCreateStep2, unk_016, 0x016);
CHECK_OFFSET(cFEStateCreateStep2, miMiscData, 0x018);
CHECK_OFFSET(cFEStateCreateStep2, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateCreateStep2, mpParentState, 0x020);
CHECK_OFFSET(cFEStateCreateStep2, mScreens, 0x024);
CHECK_OFFSET(cFEStateCreateStep2, mpScreen, 0x040);
CHECK_OFFSET(cFEStateCreateStep2, mUserNum, 0x044);
CHECK_OFFSET(cFEStateCreateStep2, unk_045, 0x045);
CHECK_OFFSET(cFEStateCreateStep2, unk_048, 0x048);
CHECK_OFFSET(cFEStateCreateStep2, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateCreateStep2, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateCreateStep2, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateCreateStep2, unk_058, 0x058);
CHECK_OFFSET(cFEStateCreateStep2, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateCreateStep2, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateCreateStep2, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateCreateStep2, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateCreateStep2, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateCreateStep2, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateCreateStep2, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateCreateStep2, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateCreateStep2, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateCreateStep2, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateCreateStep2, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateCreateStep2, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateCreateStep2, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateCreateStep2, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateCreateStep2, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateCreateStep2, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateCreateStep2, mpAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateStep2, mbScreenReady, 0x6D4);
CHECK_OFFSET(cFEStateCreateStep2, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateCreateStep2, unk_6DC, 0x6DC);
CHECK_SIZE(cFEStateCreateStep2, 0x6E0);

#endif /* FE_FESTATENETCREATE_H */
