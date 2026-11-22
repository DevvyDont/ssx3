#ifndef FE_FESTATERULESELECT_H
#define FE_FESTATERULESELECT_H

// cFEStateRuleSelect: the rule select screen (a cUIState); cFEReal creates it with cMemMan_alloc(156, "cFEStateRuleSelect").
// Size 0x9C [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "fe/festatenet.h" // tFEStateNetMsgList (cFEStateBraggingRights's cFEStateNet base)

struct cListNode;
struct cUIEngine;
struct cUIPair;
struct cUIScreen;
struct cUIState;

struct cFEStateRuleSelect {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 the vtable
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateRuleSelect
    char* mpGameEventMapping;        // 0x048 [O]
    int mCurrentMenuEnableState;     // 0x04C [O]
    char mcArraySize;                // 0x050 [O]
    char mcRuleConflictRefTable[15]; // 0x051 [O]
    cUIPair* mpMenuItemTable[15];    // 0x060 [O]
};

CHECK_OFFSET(cFEStateRuleSelect, mpGameEventMapping, 0x048);
CHECK_OFFSET(cFEStateRuleSelect, mCurrentMenuEnableState, 0x04C);
CHECK_OFFSET(cFEStateRuleSelect, mcArraySize, 0x050);
CHECK_OFFSET(cFEStateRuleSelect, mcRuleConflictRefTable, 0x051);
CHECK_OFFSET(cFEStateRuleSelect, mpMenuItemTable, 0x060);
CHECK_SIZE(cFEStateRuleSelect, 0x9C);

// ---------------------------------------------------------------------------------------------------------------------
// cBraggingRightsMenu: one player's column of the bragging-rights screen (a cUIMenu, its group and up to eight list
// boxes, one per rule). Embedded twice in cFEStateBraggingRights.
// Size 0x34: [R] cFEStateBraggingRights_cFEStateBraggingRights constructs mMenuCtrl[2] with a 0x34 stride (0x6D0, 0x704;
// cFEStateBraggingRights's next member is at 0x738); [O] On Tour's class is 0x34 too. It has virtuals (EA's
// ~cBraggingRightsMenu): g++ 2.95 puts the vptr after the data, at 0x30.
// [R] cBraggingRightsMenu_cBraggingRightsMenu (fe/festateruleselect 0x001926F0) stores its vtable cBraggingRightsMenu_vtbl (1 slot:
// the destructor cBraggingRightsMenu__cBraggingRightsMenu, fe/fepopupmisc) at 0x30 and zeroes 0x0, 0x4, the eight list boxes, 0x28 and 0x2C.

struct cUIMenu;
struct cUIGroup;
struct cUIListBox;

struct cBraggingRightsMenu {
    cUIMenu* mpPlayerMenu;          // 0x000 [O]; [R] 0; [U] setMenuItemColor reads it
    cUIGroup* mpGroup;              // 0x004 [O]; [R] 0; [U] cFEStateBraggingRights_onWidgetCreate stores the player's
                                    //       group
    cUIListBox* mpListBox[8];       // 0x008 [O]; [R] all 0 (loop from 0x24 down); [U] setConfirmed / setMenuItemColor /
                                    //       setEnableItem walk them, findListBoxP searches them, onWidgetCreate stores them
    int mbConfirmed;                // 0x028 [O] (bool); [R] 0; [U] setConfirmed stores it; cFEStateBraggingRights's
                                    //       onGainFocus / onWidgetEvent / onWidgetMenu / updateConfirmedState test it
    int mcItemEnableFlag;           // 0x02C [O]; [R] 0; [U] setEnableFlag stores it, isItemEnable / setEnableItem test its
                                    //       bits
    void** mVtbl;                   // 0x030 [R] cBraggingRightsMenu_vtbl (ctor); the dtor cBraggingRightsMenu__cBraggingRightsMenu stores it again
};

CHECK_OFFSET(cBraggingRightsMenu, mpPlayerMenu, 0x000);
CHECK_OFFSET(cBraggingRightsMenu, mpGroup, 0x004);
CHECK_OFFSET(cBraggingRightsMenu, mpListBox, 0x008);
CHECK_OFFSET(cBraggingRightsMenu, mbConfirmed, 0x028);
CHECK_OFFSET(cBraggingRightsMenu, mcItemEnableFlag, 0x02C);
CHECK_OFFSET(cBraggingRightsMenu, mVtbl, 0x030);
CHECK_SIZE(cBraggingRightsMenu, 0x34);

// ---------------------------------------------------------------------------------------------------------------------
// cFEStateBraggingRights: the online "bragging rights" screen (state id 20): two player columns of rule list boxes that
// both players confirm before an online race; the rule rows come from one of six game-event mapping tables.
// EA's source is fe/festateruleselect.cpp (+ fe/festateruleselect.h): builds/x/v2 file_hints for retail
// cFEStateBraggingRights_onCreateScreen (lines 887 ... 934, header line 182). Retail code: the ctor at the end of
// fe/festateruleselect (0x00192D00), the methods in fe/festatebigradio (0x00192D80 ... 0x001941B0) and the out-of-line
// inlines in fe/fepopupmisc (0x001DF470 ... 0x001DF578).
//
// Size 0x73C:
//   [R] cMemMan_alloc(1852, "cFEStateBraggingRights") + cFEStateBraggingRights_cFEStateBraggingRights(mem, engine) in
//       cFECustom_getNextState (fe/fereal) and fe/feasyncfile; the ctor writes mMenuCtrl at 0x6D0 / 0x704 and 0x738.
//   [O] On Tour 0xB4 = a 0x48 cFEState base + mMenuCtrl[2] (0x68) + mpGameEventMapping. Retail's base is cFEStateNet
//       (0x6D0; On Tour has no cFEStateNet), so the same two members sit at 0x6D0 and 0x738.
// Base: cFEStateNet (fe/festatenet.h, 0x6D0) : cUIState : cListNode (vptr at 0x8), written out inline with
// fe/festatenet.h's names (see that header for the inherited fields' evidence). The ctor runs cFEStateNet_cFEStateNet(this,
// pEngine, 0), stores cFEStateBraggingRights_vtbl (0x0046AF68, 32 slots: 1 dtor, 4 onCreateScreen, 6 onGainFocus, 7
// onGainTransition, 12 onUpdate, 13 onMenuGainFocus, 15 onRender2D, 17 onPopupEvent, 18 onWidgetCreate, 19
// onWidgetEvent, 20 onWidgetMenu, 24 handleEvent, 25 func_00193C00, 31 func_001941B0; the rest cFEStateNet's /
// cUIState's), constructs both menus and stores mHashName 20.
// Evidence tags: [R] retail's constructor / allocation size; [U] how retail code uses the field (C bodies in
// src/fe/festatebigradio.cpp, src/fe/festateruleselect.cpp, src/fe/fepopupmisc.cpp, and the asm); [O] SSX On Tour's
// DWARF (class cFEStateBraggingRights, cBraggingRightsMenu): names, types and order.

struct cFEStateBraggingRights {
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
    // --- cFEStateNet (fe/festatenet.h, 0x6D0; copied from it, keep the two in step)
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
                                    //       cFEStateNet_onInputBegin (slot 7, the onInputBegin override) returns it
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
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from str_sp (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_operatorE1), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateBraggingRights
    cBraggingRightsMenu mMenuCtrl[2]; // 0x6D0 [O]; [R] cBraggingRightsMenu_cBraggingRightsMenu x2 (0x6D0, 0x704); [U]
                                    //       onWidgetCreate stores each column's group and list boxes; onGainFocus /
                                    //       onWidgetEvent / onWidgetMenu / updateConfirmedState test mbConfirmed
                                    //       (0x6F8 / 0x72C); onCreateScreen / func_001941B0 index the list boxes
    char* mpGameEventMapping;       // 0x738 [O]; [R] 0; [U] onCreateScreen picks one of six rule-id byte tables
                                    //       (D_004A1710 ... D_004A1738) by game mode; onCreateScreen / onWidgetEvent /
                                    //       func_001941B0 read rule ids from it (lb; EE char is signed)
};

CHECK_OFFSET(cFEStateBraggingRights, mpPrev, 0x000);
CHECK_OFFSET(cFEStateBraggingRights, mpNext, 0x004);
CHECK_OFFSET(cFEStateBraggingRights, mVtbl, 0x008);
CHECK_OFFSET(cFEStateBraggingRights, mHashName, 0x00C);
CHECK_OFFSET(cFEStateBraggingRights, mpEngine, 0x010);
CHECK_OFFSET(cFEStateBraggingRights, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateBraggingRights, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateBraggingRights, unk_016, 0x016);
CHECK_OFFSET(cFEStateBraggingRights, miMiscData, 0x018);
CHECK_OFFSET(cFEStateBraggingRights, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateBraggingRights, mpParentState, 0x020);
CHECK_OFFSET(cFEStateBraggingRights, mScreens, 0x024);
CHECK_OFFSET(cFEStateBraggingRights, mpScreen, 0x040);
CHECK_OFFSET(cFEStateBraggingRights, mUserNum, 0x044);
CHECK_OFFSET(cFEStateBraggingRights, unk_045, 0x045);
CHECK_OFFSET(cFEStateBraggingRights, unk_048, 0x048);
CHECK_OFFSET(cFEStateBraggingRights, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateBraggingRights, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateBraggingRights, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateBraggingRights, unk_058, 0x058);
CHECK_OFFSET(cFEStateBraggingRights, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateBraggingRights, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateBraggingRights, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateBraggingRights, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateBraggingRights, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateBraggingRights, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateBraggingRights, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateBraggingRights, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateBraggingRights, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateBraggingRights, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateBraggingRights, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateBraggingRights, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateBraggingRights, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateBraggingRights, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateBraggingRights, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateBraggingRights, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateBraggingRights, mMenuCtrl, 0x6D0);
CHECK_OFFSET(cFEStateBraggingRights, mpGameEventMapping, 0x738);
CHECK_SIZE(cFEStateBraggingRights, 0x73C);

#endif /* FE_FESTATERULESELECT_H */
