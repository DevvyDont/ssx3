#ifndef FE_FESTATENET_H
#define FE_FESTATENET_H

// cFEStateNet: the base of every online front-end state (the 29 classes in fe/feasyncfile.cpp: cFEStateNetMessenger,
// cFEStateLogin, cFEStateLobbyMenu, cFEStateNetConfig, cFEStateCreateStep1/2, cFEStateStats, cFEBlankPopup, ...,
// plus cFEStateBraggingRights in festateruleselect.cpp). A cUIState that adds the net popups (mpPopup, mpKeyboard),
// three unicode text buffers, an ascii mask buffer and a queue of pending net messages.
//
// Size 0x6D0: cFEStateOnlineMainMenu, cFEStateNetNews and cFEStateNetSSXWorld are allocated with
// cMemMan_alloc(0x6D0, "<class name>") and their constructors (cFEStateOnlineMainMenu_cFEStateOnlineMainMenu, cFEStateNetNews_cFEStateNetNews, cFEStateNetSSXWorld_cFEStateNetSSXWorld) only
// call this class's constructor and store mHashName and the vtable; every other subclass's own fields start at 0x6D0.
// Constructors: cFEStateNet(cUIEngine*, cUIState*) = cFEStateNet_cFEStateNet (unit fe/feasyncfile), default copy cFEStateNet_cFEStateNet1
// (main/all_in_one_main); destructor cFEStateNet__cFEStateNet. Retail vtable _vt$11cFEStateNet = cFEStateNet_vtbl (32 slots; slots 24-31
// are its own virtuals: func_001DE868, func_001A89B0, func_001AAB68, func_001AA9B0, func_001DE870, cFEStateNet_isProcessingEvents,
// func_001A8E40, func_001A8830). Methods: fe/feasyncfile 0x1A8500-0x1ABED8, plus the out-of-line inlines
// func_001DE860/func_001DE868/func_001DE870/func_001DE890/func_001DE8A0/cFEStateNet_isProcessingEvents in main/all_in_one_main.
// EA source: probably d:/ssx/code/source/fe/festatenet.cpp (the preview's assert strings name its subclasses' files,
// festatenetmessenger.cpp, festatenetlobby.cpp ..., not this one).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet (cListNode and cUIState
// written out inline, as in fe/ovstate.h). Subclasses: retype a subclass method with this header only when it touches
// nothing past 0x6D0; otherwise use the subclass's own header (e.g. fe/festatenetmessenger.h).
// Evidence tags:
//   [R] retail's constructors cFEStateNet_cFEStateNet / cFEStateNet_cFEStateNet1 and destructor cFEStateNet__cFEStateNet (and the cUIState
//       constructor cUIState_cUIState1 for the base);
//   [P] the July 2003 preview: _vt$11cFEStateNet, its inline isProcessingEvents (lw 0x38 = preview mpScreen),
//       getFEPath/fePathIsOnline (a global, D_004A1A70 in retail, not a member) and its default constructor
//       __11cFEStateNet (a 0xAC-byte class; its list at 0xA4/0xA8 is retail 0x6C8/0x6CC, +0x624: the preview had no
//       text buffers). Preview offsets are not used directly;
//   [U] how retail code uses the field (the methods named above and the subclasses' methods);
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only (On Tour has no cFEStateNet).
// Names: base fields are EA's (ui/uistate.h); cFEStateNet's own fields carry no EA names (no DWARF, no preview
// accessors), so only fields whose use is unambiguous are named, the rest are unk_.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIText;
struct cScreenPopup;
struct cKeyboardPopup;

// The pending-message queue at 0x6C8: an EA STL list (8 bytes). The constructor allocates its sentinel node with
// cMemMan_alloc(0xC, "StlBlock") and links it to itself; nodes are {next, prev, data} (func_001A8AE8 inserts one per
// message, handleEvent cFEStateNet_handleEvent walks them, the destructor deletes each message through its vtable).
struct tFEStateNetMsgList {
    int unk_00;                     // 0x0 never read or written directly (list allocator / first word); the list's
                                    //     address (0x6C8) is what the list helpers take
    void* mpNode;                   // 0x4 [R] the sentinel node (cMemMan_alloc(0xC, "StlBlock")); [U] node->next == mpNode
                                    //     means empty (cFEStateNet_handleEvent, the destructor)
};

CHECK_OFFSET(tFEStateNetMsgList, unk_00, 0x0);
CHECK_OFFSET(tFEStateNetMsgList, mpNode, 0x4);
CHECK_SIZE(tFEStateNetMsgList, 0x8);

struct cFEStateNet {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] _vt$11cFEStateNet cFEStateNet_vtbl, stored by cFEStateNet_cFEStateNet/cFEStateNet_cFEStateNet1;
                                    //       subclasses store theirs after calling it
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] the subclass ctors store
                                    //       their state id here (0x3B cFEStateOnlineMainMenu, 0x43 cFEStateNetNews, 0x46
                                    //       cFEStateNetMessenger, 0x4A cFEStateNetSSXWorld ...)
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
    // --- cFEStateNet
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
                                    //       [U] while set and the lobby is disconnected, func_001A89B0 (slot 25) refuses
                                    //       to push any state but 0x31/0x33/0x4D/0x4E (net config, login, account
                                    //       creation steps 1/2: mHashName) and opens the disconnect popup (func_001A9930)
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
};

CHECK_OFFSET(cFEStateNet, mpPrev, 0x000);
CHECK_OFFSET(cFEStateNet, mpNext, 0x004);
CHECK_OFFSET(cFEStateNet, mVtbl, 0x008);
CHECK_OFFSET(cFEStateNet, mHashName, 0x00C);
CHECK_OFFSET(cFEStateNet, mpEngine, 0x010);
CHECK_OFFSET(cFEStateNet, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateNet, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateNet, unk_016, 0x016);
CHECK_OFFSET(cFEStateNet, miMiscData, 0x018);
CHECK_OFFSET(cFEStateNet, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateNet, mpParentState, 0x020);
CHECK_OFFSET(cFEStateNet, mScreens, 0x024);
CHECK_OFFSET(cFEStateNet, mpScreen, 0x040);
CHECK_OFFSET(cFEStateNet, mUserNum, 0x044);
CHECK_OFFSET(cFEStateNet, unk_045, 0x045);
CHECK_OFFSET(cFEStateNet, unk_048, 0x048);
CHECK_OFFSET(cFEStateNet, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNet, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNet, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNet, unk_058, 0x058);
CHECK_OFFSET(cFEStateNet, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNet, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNet, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateNet, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateNet, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNet, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNet, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNet, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateNet, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateNet, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateNet, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateNet, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateNet, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateNet, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateNet, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateNet, mMsgList, 0x6C8);
CHECK_SIZE(cFEStateNet, 0x6D0);

#endif /* FE_FESTATENET_H */
