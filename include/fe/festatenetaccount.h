#ifndef FE_FESTATENETACCOUNT_H
#define FE_FESTATENETACCOUNT_H

// The online account screens (EA's d:/ssx/code/source/fe/festatenetaccount.cpp: the preview's assert at line 852
// sits in retail func_001AD9C8, a cFEStateSelectPersona popup handler; cFEUseExistingAccount's code comes right
// before it in both builds, after cFEBlankPopup). Two cFEStateNet states (fe/festatenet.h, 0x6D0) live here:
//
// cFEUseExistingAccount: screen "204use_existing_account": account name and password boxes, "forgot password" /
// "forgot account" requests (by e-mail) and the verify step, then the memory-card save popup
// (cFEStateNetAccountMem) and cFEStateSelectPersona. EA's name: the preview's __21cFEUseExistingAccount and
// _vt$21cFEUseExistingAccount. Size 0x6E4: cMemMan_alloc(0x6E4, "cFEUseExistingAccount") + cFEUseExistingAccount_cFEUseExistingAccount(mem,
// pEngine) (cFEStateLogin_onWidgetEvent, the only site). Constructors cFEUseExistingAccount_cFEUseExistingAccount (cFEStateNet(pEngine, 0), vtable cFEUseExistingAccount_vtbl,
// mHashName 0x35) and the default cFEUseExistingAccount_cFEUseExistingAccount1 (fe/fepopupmisc); destructor cFEUseExistingAccount__cFEUseExistingAccount (slot 1: destroys
// mszEmail, then cFEStateNet's cFEStateNet__cFEStateNet). Retail vtable cFEUseExistingAccount_vtbl (32 slots: 1 dtor, 4 onCreateScreen
// cFEUseExistingAccount_onCreateScreen, 12 onUpdate cFEUseExistingAccount_onUpdate, 17 onPopupEvent cFEUseExistingAccount_onPopupEvent, 18 onWidgetCreate cFEUseExistingAccount_onWidgetCreate,
// 19 onWidgetEvent cFEUseExistingAccount_onWidgetEvent, 20 cFEUseExistingAccount_onWidgetMenu, 25 func_001AC4F0 (help texts), 29 func_001E08D0,
// 31 func_001ACEF8 (net events 0xC9/0xCF/0xD2/0xD4)). Methods: fe/feasyncfile 0x1AC010-0x1ACEF8.
//
// cFEStateSelectPersona: screen "220select_persona": the account's persona list ("PersonaMenu"), create / delete
// persona. EA's name: the preview's __21cFEStateSelectPersona / _$_21cFEStateSelectPersona. Size 0x6D8:
// cMemMan_alloc(0x6D8, "cFEStateSelectPersona") + cFEStateSelectPersona_cFEStateSelectPersona(mem, pEngine) at all 10 sites (fe/feasyncfile).
// Constructors cFEStateSelectPersona_cFEStateSelectPersona (vtable cFEStateSelectPersona_vtbl, mHashName 0x38) and the default cFEStateSelectPersona_cFEStateSelectPersona1 (fe/fepopupmisc);
// destructor cFEStateSelectPersona__cFEStateSelectPersona (fe/fepopupmisc; slot 1; the preview's _$_21cFEStateSelectPersona). Retail vtable
// cFEStateSelectPersona_vtbl (33 slots: 4 onCreateScreen cFEStateSelectPersona_onCreateScreen, 12 onUpdate cFEStateSelectPersona_onUpdate, 17 onPopupEvent cFEStateSelectPersona_onPopupEvent,
// 18 onWidgetCreate cFEStateSelectPersona_onWidgetCreate, 19 onWidgetEvent cFEStateSelectPersona_onWidgetEvent, 25 func_001AD4F0 (help), 29 func_001E0940,
// 31 func_001ADA30 (net events), 32 func_001E0948, 33 func_001AD9C8). Methods: fe/feasyncfile 0x1AD130-0x1ADC30.
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

struct cFEUseExistingAccount {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEUseExistingAccount_vtbl (vtables.tsv; 32 slots), stored by cFEUseExistingAccount_cFEUseExistingAccount /
                                    //       cFEUseExistingAccount_cFEUseExistingAccount1 after the cFEStateNet ctor and by the destructor
                                    //       cFEUseExistingAccount__cFEUseExistingAccount
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEUseExistingAccount_cFEUseExistingAccount stores
                                    //       0x35
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
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_operatorE1), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEUseExistingAccount
    int unk_6D0;                    // 0x6D0 [R] 0 (cFEUseExistingAccount_cFEUseExistingAccount, cFEUseExistingAccount_cFEUseExistingAccount1); no other access seen
    int mbAskSave;                  // 0x6D4 [U] func_001ACB48 (popup 0x82C, the cFEStateNetAccountMem result): 1 on
                                    //       0x11 (option 2), 0 on 0xF/0x10 (then pushes cFEStateSelectPersona); on
                                    //       0x16 it opens the "kT_POPUP204SaveAccount" yes/no popup (0x81E) when set,
                                    //       then clears it
    int miRequest;                  // 0x6D8 [U] the pending LobbyMan request: onWidgetEvent cFEUseExistingAccount_onWidgetEvent stores 4
                                    //       (verify, "kT_POPUP201Verify") and 2 (forgotten password), func_001AC9C0
                                    //       stores 3 (forgotten account name); the net-event handler func_001ACEF8 on
                                    //       0xC9 sends it: 2 -> func_0025C3F8(mUserName), 3 ->
                                    //       func_0025C1E0(mszEmail), 4 -> func_0025C610
    char* mszEmail;                 // 0x6DC [R] cBXString: the empty-string rep cBXString_EmptyString (both ctors), destroyed by
                                    //       cFEUseExistingAccount__cFEUseExistingAccount (cBXString__cBXString(.., 2)); [U] func_001AC9C0 assigns
                                    //       the e-mail address the request popup returned (popup + 0x2B0;
                                    //       cBXString_operatorE1); func_001AC448 opens "kT_POPUP204RequestAccount"
                                    //       when its length (the word before the text) is 0; func_001ACEF8 passes it
                                    //       to func_0025C1E0
    cUIThing* mpTextForgotPassword; // 0x6E0 [R] 0; [U] onWidgetCreate cFEUseExistingAccount_onWidgetCreate: widget "TextForgotPassword"
                                    //       (miMiscData 2), then its slots 25 and 8 with 1; func_001ACDD0 calls its
                                    //       slot 8 with 0 once an account name is entered, 1 when the name is empty
};

CHECK_OFFSET(cFEUseExistingAccount, mpPrev, 0x000);
CHECK_OFFSET(cFEUseExistingAccount, mpNext, 0x004);
CHECK_OFFSET(cFEUseExistingAccount, mVtbl, 0x008);
CHECK_OFFSET(cFEUseExistingAccount, mHashName, 0x00C);
CHECK_OFFSET(cFEUseExistingAccount, mpEngine, 0x010);
CHECK_OFFSET(cFEUseExistingAccount, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEUseExistingAccount, mCtrlBits, 0x015);
CHECK_OFFSET(cFEUseExistingAccount, unk_016, 0x016);
CHECK_OFFSET(cFEUseExistingAccount, miMiscData, 0x018);
CHECK_OFFSET(cFEUseExistingAccount, mAttrState, 0x01C);
CHECK_OFFSET(cFEUseExistingAccount, mpParentState, 0x020);
CHECK_OFFSET(cFEUseExistingAccount, mScreens, 0x024);
CHECK_OFFSET(cFEUseExistingAccount, mpScreen, 0x040);
CHECK_OFFSET(cFEUseExistingAccount, mUserNum, 0x044);
CHECK_OFFSET(cFEUseExistingAccount, unk_045, 0x045);
CHECK_OFFSET(cFEUseExistingAccount, unk_048, 0x048);
CHECK_OFFSET(cFEUseExistingAccount, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEUseExistingAccount, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEUseExistingAccount, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEUseExistingAccount, unk_058, 0x058);
CHECK_OFFSET(cFEUseExistingAccount, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEUseExistingAccount, mustrMessage, 0x09C);
CHECK_OFFSET(cFEUseExistingAccount, unk_49C, 0x49C);
CHECK_OFFSET(cFEUseExistingAccount, unk_59C, 0x59C);
CHECK_OFFSET(cFEUseExistingAccount, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEUseExistingAccount, mpPopup, 0x6A0);
CHECK_OFFSET(cFEUseExistingAccount, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEUseExistingAccount, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEUseExistingAccount, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEUseExistingAccount, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEUseExistingAccount, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEUseExistingAccount, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEUseExistingAccount, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEUseExistingAccount, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEUseExistingAccount, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEUseExistingAccount, mMsgList, 0x6C8);
CHECK_OFFSET(cFEUseExistingAccount, unk_6D0, 0x6D0);
CHECK_OFFSET(cFEUseExistingAccount, mbAskSave, 0x6D4);
CHECK_OFFSET(cFEUseExistingAccount, miRequest, 0x6D8);
CHECK_OFFSET(cFEUseExistingAccount, mszEmail, 0x6DC);
CHECK_OFFSET(cFEUseExistingAccount, mpTextForgotPassword, 0x6E0);
CHECK_SIZE(cFEUseExistingAccount, 0x6E4);

struct cFEStateSelectPersona {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateSelectPersona_vtbl (vtables.tsv; 33 slots), stored by cFEStateSelectPersona_cFEStateSelectPersona /
                                    //       cFEStateSelectPersona_cFEStateSelectPersona1 after the cFEStateNet ctor and by the destructor
                                    //       cFEStateSelectPersona__cFEStateSelectPersona
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateSelectPersona_cFEStateSelectPersona stores
                                    //       0x38
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
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_operatorE1), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateSelectPersona
    int mbExistingPersona;          // 0x6D0 [R] 0 (cFEStateSelectPersona_cFEStateSelectPersona; not set by the default ctor); [U] onWidgetEvent
                                    //       cFEStateSelectPersona_onWidgetEvent: 1 when an existing persona row is chosen (func_0025CD28
                                    //       selects it), 0 when the "create new persona" row opens the keyboard; the
                                    //       net-event handler func_001ADC30 picks "kT_POPUPCMNProfanity" (set) or
                                    //       "kT_POPUPCMNProfanityPersona" (clear) on the "filt" reply
    int miDeletePersona;            // 0x6D4 [U] onWidgetEvent cFEStateSelectPersona_onWidgetEvent (message 8, the delete key) stores the
                                    //       persona row and opens "kT_POPUP220DeleteInitial"; func_001AD9C8 (popup
                                    //       0x816, option 0xF) deletes it: func_0025D428(LobbyMan, it) (the C body
                                    //       reads it as a void*)
};

CHECK_OFFSET(cFEStateSelectPersona, mpPrev, 0x000);
CHECK_OFFSET(cFEStateSelectPersona, mpNext, 0x004);
CHECK_OFFSET(cFEStateSelectPersona, mVtbl, 0x008);
CHECK_OFFSET(cFEStateSelectPersona, mHashName, 0x00C);
CHECK_OFFSET(cFEStateSelectPersona, mpEngine, 0x010);
CHECK_OFFSET(cFEStateSelectPersona, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateSelectPersona, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateSelectPersona, unk_016, 0x016);
CHECK_OFFSET(cFEStateSelectPersona, miMiscData, 0x018);
CHECK_OFFSET(cFEStateSelectPersona, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateSelectPersona, mpParentState, 0x020);
CHECK_OFFSET(cFEStateSelectPersona, mScreens, 0x024);
CHECK_OFFSET(cFEStateSelectPersona, mpScreen, 0x040);
CHECK_OFFSET(cFEStateSelectPersona, mUserNum, 0x044);
CHECK_OFFSET(cFEStateSelectPersona, unk_045, 0x045);
CHECK_OFFSET(cFEStateSelectPersona, unk_048, 0x048);
CHECK_OFFSET(cFEStateSelectPersona, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateSelectPersona, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateSelectPersona, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateSelectPersona, unk_058, 0x058);
CHECK_OFFSET(cFEStateSelectPersona, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateSelectPersona, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateSelectPersona, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateSelectPersona, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateSelectPersona, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateSelectPersona, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateSelectPersona, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateSelectPersona, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateSelectPersona, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateSelectPersona, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateSelectPersona, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateSelectPersona, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateSelectPersona, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateSelectPersona, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateSelectPersona, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateSelectPersona, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateSelectPersona, mbExistingPersona, 0x6D0);
CHECK_OFFSET(cFEStateSelectPersona, miDeletePersona, 0x6D4);
CHECK_SIZE(cFEStateSelectPersona, 0x6D8);

#endif /* FE_FESTATENETACCOUNT_H */
