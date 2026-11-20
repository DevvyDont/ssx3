#ifndef FE_FESTATEAUTHDNAS_H
#define FE_FESTATEAUTHDNAS_H

// The DNAS authentication popups of the network configuration (EA's d:/ssx/code/source/fe/festatenetconfig.cpp: the
// preview's assert at line 641 sits in cFEStateAuthDNAS's onCreateScreen, retail cFEStateAuthDNAS_onCreateScreen, and
// cFEStateAuthDNASError's code lies between it and cFEStateNetAccountMem's, whose asserts name the same file). Both
// are cFEStateNet states (fe/festatenet.h, 0x6D0) pushed by cFEStateNetConfig (fe/festatenetconfig.h); this header
// could also be merged into fe/festatenetconfig.h.
//
// cFEStateAuthDNAS: screen "205AuthenticatingDNAS": runs the DNAS authentication object and reports to its parent
// (slot 17: 0xF ok, 0x10 error with the three texts copied into cFEStateNetConfig's mpustrAuthError[], 0x14 cancel).
// EA's name: the preview's __16cFEStateAuthDNAS, onGainTransition__16cFEStateAuthDNASb, onLoseTransition__16...
// Size 0x700: cMemMan_alloc(0x700, "NetConfigAuthPopup") + cFEStateAuthDNAS_cFEStateAuthDNAS(mem, pEngine, pParent) (func_001B55B0, the
// only site). Constructors cFEStateAuthDNAS_cFEStateAuthDNAS (cFEStateNet(pEngine, pParent), vtable cFEStateAuthDNAS_vtbl, mHashName 0x44, unk_6A8 =
// 0) and the default cFEStateAuthDNAS_cFEStateAuthDNAS1 (fe/fepopupmisc); destructor cFEStateAuthDNAS__cFEStateAuthDNAS (slot 1: deletes mpAuth, frees
// mpustrText). Retail vtable cFEStateAuthDNAS_vtbl (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateAuthDNAS_onCreateScreen, 7
// cFEStateAuthDNAS_onGainTransition
// (onGainTransition), 8 cFEStateAuthDNAS_onInputBegin, 10 cFEStateAuthDNAS_onLoseTransition, 12 onUpdate cFEStateAuthDNAS_onUpdate, 18
// onWidgetCreate cFEStateAuthDNAS_onWidgetCreate, 19 onWidgetEvent cFEStateAuthDNAS_onWidgetEvent, 20 cFEStateAuthDNAS_onWidgetMenu, 24 handleEvent cFEStateAuthDNAS_handleEvent, 29
// func_001E0C28, 32 func_001E0C20).
// Methods: fe/feasyncfile 0x1B6288-0x1B6A90.
//
// cFEStateAuthDNASError: screen "207DNASerror": shows the three DNAS error texts. EA's name: the allocation tag.
// Size 0x6E8: cMemMan_alloc(0x6E8, "cFEStateAuthDNASError") + cFEStateAuthDNASError_cFEStateAuthDNASError(mem, pEngine, pParent,
// mpustrAuthError[1],
// [2], [0]) (cFEStateNetConfig's func_001B5830, the only site). Constructors cFEStateAuthDNASError_cFEStateAuthDNASError (vtable cFEStateAuthDNASError_vtbl,
// mHashName
// 0x45) and the default cFEStateAuthDNASError_cFEStateAuthDNASError1 (fe/fepopupmisc); destructor cFEStateAuthDNASError__cFEStateAuthDNASError (slot 1). Retail vtable cFEStateAuthDNASError_vtbl
// (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateAuthDNASError_onCreateScreen, 7 cFEStateAuthDNASError_onGainTransition, 10 cFEStateAuthDNASError_onLoseTransition, 12 onUpdate
// cFEStateNet's cFEStateNet_onUpdate, 18 onWidgetCreate cFEStateAuthDNASError_onWidgetCreate, 19 onWidgetEvent cFEStateAuthDNASError_onWidgetEvent, 20 cFEStateAuthDNASError_onWidgetMenu, 24
// handleEvent cFEStateAuthDNASError_handleEvent, 29 func_001E0C88, 32 func_001E0C80). Methods: fe/feasyncfile 0x1B6AB8-0x1B6EA8.
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

struct cFEStateAuthDNAS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateAuthDNAS_vtbl (vtables.tsv), stored by cFEStateAuthDNAS_cFEStateAuthDNAS / cFEStateAuthDNAS_cFEStateAuthDNAS1
                                    //       after the cFEStateNet ctor and by the destructor cFEStateAuthDNAS__cFEStateAuthDNAS
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateAuthDNAS_cFEStateAuthDNAS stores
                                    //       0x44
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
    // --- cFEStateAuthDNAS
    void* mpAuth;                   // 0x6D0 [R] 0; [U] onCreateScreen cFEStateAuthDNAS_onCreateScreen stores
                                    //       func_002561B8(cMemMan_alloc(0xA50, "cFEStateAuthDNASCreate")) (the DNAS
                                    //       authentication object, class not named) and starts it in the mode of the
                                    //       profile bits cBEOptionInterface_mOption; onUpdate cFEStateAuthDNAS_onUpdate polls it (its first word:
                                    //       >= 3 ready, 5 done; + 0x1C error; error texts at + 0x450/0x650/0x850),
                                    //       cFEStateAuthDNAS_onInputBegin cancels it (+ 0x14 = 1); the destructor shuts it down
                                    //       (func_002563A8) and deletes it
    cUIText* mpTextAuth;            // 0x6D4 [R] 0; [U] onWidgetCreate cFEStateAuthDNAS_onWidgetCreate: widget "TextAuth"
                                    //       ("kT_ovrcmnloading", then "kT_205Authenticating" in onUpdate)
    cUIText* mpTextCancel;          // 0x6D8 [U] onWidgetCreate: widget "TextCancel" (hidden; onUpdate shows it with
                                    //       the next two once the object is ready)
    cUIThing* mpXButton;            // 0x6DC [U] onWidgetCreate: widget "XButton" (hidden)
    cUIThing* mpSubmitRect;         // 0x6E0 [U] onWidgetCreate: widget "submitrect" (hidden)
    int miState;                    // 0x6E4 [R] 0; [U] onCreateScreen sets 1; onUpdate cFEStateAuthDNAS_onUpdate: 1 -> 2 when the
                                    //       object is ready (starts it), 2 -> 3 when done, 3 reports ok / error to
                                    //       mpParentState, 4 (cancelled by cFEStateAuthDNAS_onInputBegin in state 2) reports 0x14
                                    //       when done
    char unk_6E8[0xC];              // 0x6E8 no access seen
    int unk_6F4;                    // 0x6F4 [R] 2 (both ctors); no other access seen
    int unk_6F8;                    // 0x6F8 [R] 0x1E (both ctors); no other access seen
    unsigned short* mpustrText;     // 0x6FC [R] 0, then operator_new_tag((USTR_length("kT_205Authenticating") + 5) *
                                    //       2, "cFEStateAuthDNAS") (cFEStateAuthDNAS_cFEStateAuthDNAS); freed by the destructor; no other
                                    //       access seen
};

CHECK_OFFSET(cFEStateAuthDNAS, mpPrev, 0x000);
CHECK_OFFSET(cFEStateAuthDNAS, mpNext, 0x004);
CHECK_OFFSET(cFEStateAuthDNAS, mVtbl, 0x008);
CHECK_OFFSET(cFEStateAuthDNAS, mHashName, 0x00C);
CHECK_OFFSET(cFEStateAuthDNAS, mpEngine, 0x010);
CHECK_OFFSET(cFEStateAuthDNAS, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateAuthDNAS, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateAuthDNAS, unk_016, 0x016);
CHECK_OFFSET(cFEStateAuthDNAS, miMiscData, 0x018);
CHECK_OFFSET(cFEStateAuthDNAS, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateAuthDNAS, mpParentState, 0x020);
CHECK_OFFSET(cFEStateAuthDNAS, mScreens, 0x024);
CHECK_OFFSET(cFEStateAuthDNAS, mpScreen, 0x040);
CHECK_OFFSET(cFEStateAuthDNAS, mUserNum, 0x044);
CHECK_OFFSET(cFEStateAuthDNAS, unk_045, 0x045);
CHECK_OFFSET(cFEStateAuthDNAS, unk_048, 0x048);
CHECK_OFFSET(cFEStateAuthDNAS, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateAuthDNAS, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateAuthDNAS, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateAuthDNAS, unk_058, 0x058);
CHECK_OFFSET(cFEStateAuthDNAS, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateAuthDNAS, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateAuthDNAS, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateAuthDNAS, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateAuthDNAS, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateAuthDNAS, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateAuthDNAS, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateAuthDNAS, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateAuthDNAS, mpAuth, 0x6D0);
CHECK_OFFSET(cFEStateAuthDNAS, mpTextAuth, 0x6D4);
CHECK_OFFSET(cFEStateAuthDNAS, mpTextCancel, 0x6D8);
CHECK_OFFSET(cFEStateAuthDNAS, mpXButton, 0x6DC);
CHECK_OFFSET(cFEStateAuthDNAS, mpSubmitRect, 0x6E0);
CHECK_OFFSET(cFEStateAuthDNAS, miState, 0x6E4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6E8, 0x6E8);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6F4, 0x6F4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6F8, 0x6F8);
CHECK_OFFSET(cFEStateAuthDNAS, mpustrText, 0x6FC);
CHECK_SIZE(cFEStateAuthDNAS, 0x700);

struct cFEStateAuthDNASError {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateAuthDNASError_vtbl (vtables.tsv), stored by cFEStateAuthDNASError_cFEStateAuthDNASError / cFEStateAuthDNASError_cFEStateAuthDNASError1
                                    //       after the cFEStateNet ctor and by the destructor cFEStateAuthDNASError__cFEStateAuthDNASError
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateAuthDNASError_cFEStateAuthDNASError stores
                                    //       0x45
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
    // --- cFEStateAuthDNASError
    unsigned short* mpustrBody;     // 0x6D0 [R] the ctor's 4th argument (cFEStateNetConfig's mpustrAuthError[1]), 0
                                    //       in the default ctor; [U] cFEStateAuthDNASError_onGainTransition (slot 7) wraps it into mpBody, or
                                    //       hides mpBody when empty (strlen1)
    unsigned short* mpustrFooter;   // 0x6D4 [R] the 5th argument (mpustrAuthError[2]), 0; [U] cFEStateAuthDNASError_onGainTransition plays
                                    //       frame "full" and wraps it into mpFooter, or plays "short" and hides
                                    //       mpFooter when empty
    unsigned short* mpustrTitle;    // 0x6D8 [R] the 6th argument (mpustrAuthError[0]), 0; [U] cFEStateAuthDNASError_onGainTransition wraps it
                                    //       into mpTitle, or hides mpTitle when empty
    cUIText* mpFooter;              // 0x6DC [U] onWidgetCreate cFEStateAuthDNASError_onWidgetCreate: widget "Footer"
    cUIText* mpBody;                // 0x6E0 [U] onWidgetCreate: widget "Body"
    cUIText* mpTitle;               // 0x6E4 [U] onWidgetCreate: widget "Title"
};

CHECK_OFFSET(cFEStateAuthDNASError, mpPrev, 0x000);
CHECK_OFFSET(cFEStateAuthDNASError, mpNext, 0x004);
CHECK_OFFSET(cFEStateAuthDNASError, mVtbl, 0x008);
CHECK_OFFSET(cFEStateAuthDNASError, mHashName, 0x00C);
CHECK_OFFSET(cFEStateAuthDNASError, mpEngine, 0x010);
CHECK_OFFSET(cFEStateAuthDNASError, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateAuthDNASError, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateAuthDNASError, unk_016, 0x016);
CHECK_OFFSET(cFEStateAuthDNASError, miMiscData, 0x018);
CHECK_OFFSET(cFEStateAuthDNASError, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateAuthDNASError, mpParentState, 0x020);
CHECK_OFFSET(cFEStateAuthDNASError, mScreens, 0x024);
CHECK_OFFSET(cFEStateAuthDNASError, mpScreen, 0x040);
CHECK_OFFSET(cFEStateAuthDNASError, mUserNum, 0x044);
CHECK_OFFSET(cFEStateAuthDNASError, unk_045, 0x045);
CHECK_OFFSET(cFEStateAuthDNASError, unk_048, 0x048);
CHECK_OFFSET(cFEStateAuthDNASError, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateAuthDNASError, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateAuthDNASError, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateAuthDNASError, unk_058, 0x058);
CHECK_OFFSET(cFEStateAuthDNASError, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateAuthDNASError, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateAuthDNASError, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateAuthDNASError, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateAuthDNASError, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateAuthDNASError, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateAuthDNASError, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateAuthDNASError, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateAuthDNASError, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateAuthDNASError, mpustrBody, 0x6D0);
CHECK_OFFSET(cFEStateAuthDNASError, mpustrFooter, 0x6D4);
CHECK_OFFSET(cFEStateAuthDNASError, mpustrTitle, 0x6D8);
CHECK_OFFSET(cFEStateAuthDNASError, mpFooter, 0x6DC);
CHECK_OFFSET(cFEStateAuthDNASError, mpBody, 0x6E0);
CHECK_OFFSET(cFEStateAuthDNASError, mpTitle, 0x6E4);
CHECK_SIZE(cFEStateAuthDNASError, 0x6E8);

#endif /* FE_FESTATEAUTHDNAS_H */
