#ifndef FE_MESSAGECENTER_H
#define FE_MESSAGECENTER_H

// The in-game message center (EA's d:/ssx/code/source/fe/messagecenter.cpp, preview assert strings; retail keeps its
// code in fe/fepopupmisc 0x1E12F0-0x1E5AA0). Two UI states live here:
//
// cOVState_MESSAGECENTER: the overlay listing the saved messages (screen "112messagecenter": From / Subject / Number
// columns, unread-mail and FAQ icons, a scrollable "MenuFroms" menu). EA's name from the preview's linkonce sections
// (__22cOVState_MESSAGECENTERP9cUIEngine12tOVStateEnum, _$_22cOVState_MESSAGECENTER). Size 0x180: the overlay
// factory (cOVStateManager_triggerOverlay, fe/ovtemplatedialog.cpp) does operator new(0x180, "cOVStateOverlay") and
// calls the out-of-line constructor cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(this, pEngine, 0x22). Base cOVState (fe/ovstate.h, 0x9C), written
// out flat below. Retail vtable D_00474318 (stored by cOVState_MESSAGECENTER_cOVState_MESSAGECENTER after cOVState's D_00474E08; destructor
// cOVState_MESSAGECENTER__cOVState_MESSAGECENTER in fe/all_in_one_fe).
//
// cOVState_ViewPopup: the popup that shows one message (screen "113ViewMessage"), pushed by
// cOVState_MESSAGECENTER (func_001E50C8: cMemMan_alloc(0x70, "cOVState_ViewPopup") + constructor cOVState_ViewPopup_cOVState_ViewPopup(this,
// pEngine, pParent, message, number)). Size 0x70. Base cUIState (ui/uistate.h, 0x48), written out flat (it is not a
// cOVState: the constructor is cUIState(cUIEngine*, cUIState*) cUIState_cUIState1 and nothing stores 0x48..0x9C as
// cOVState does). Retail vtable cOVState_ViewPopup_vtbl (destructor cOVState_ViewPopup__cOVState_ViewPopup in fe/all_in_one_fe).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. There is no
// SSX On Tour DWARF for these classes and the preview keeps no accessors, so the own members carry [R]/[U]
// evidence only; widget pointers are named after the UI object onWidgetCreate binds to them (names read from the
// retail ELF). Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*) cUIState_cUIState / cUIState(cUIEngine*, cUIState*) cUIState_cUIState1,
//       cOVState(cUIEngine*, tOVStateEnum) (inlined in cOVState_MESSAGECENTER_cOVState_MESSAGECENTER), cOVState_MESSAGECENTER_cOVState_MESSAGECENTER, cOVState_ViewPopup_cOVState_ViewPopup, and the
//       allocation sizes above;
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the inherited cListNode / cUIState / cOVState members (see fe/ovstate.h).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThing;
struct cUIText;
struct cUIMenu;

struct cOVState_MESSAGECENTER {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_00474318, stored by cOVState_MESSAGECENTER_cOVState_MESSAGECENTER (after cOVState's D_00474E08)
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] cUIEngine_addScreenByHashName
                                    //       (cOVState_MESSAGECENTER_onCreateScreen), cUITextureBank_getElementByHashname(mpEngine + 0x58, hash) in the ctor
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData; [U] here the opening mode: the ctor stores
                                    //       0/1 (hint 7 not yet / already seen, func_001E3A30(7)) or 2 (D_004A20CC set);
                                    //       onCreateScreen/onDestroyScreen/onGainFocus switch on it
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] 0 (1-argument cUIState ctor)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens; [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] cOVState_MESSAGECENTER_cOVState_MESSAGECENTER zeroes it (inlined cOVState ctor)
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] cOVState_MESSAGECENTER_cOVState_MESSAGECENTER stores its third argument (0x22)
    // --- cOVState_MESSAGECENTER
    cUIThing* mpUnreadMail[8];      // 0x09C [R] ctor zeroes 8; [U] onWidgetCreate (cOVState_MESSAGECENTER_onWidgetCreate): widgets "UnreadMail%d";
                                    //       func_001E48B0/func_001E4C78 show/hide row i's
    cUIThing* mpFAQIcon[8];         // 0x0BC [R] zeroed; [U] widgets "FAQIcon%d"; func_001E4C78 sets them to the folder /
                                    //       minus-folder / question-mark texture (miFolderTex ...)
    cUIText* mpFrom[8];             // 0x0DC [R] zeroed; [U] widgets "From%d"; func_001E4C78 cUIText_setUnicodeStringByID and
                                    //       stores the message id in the widget's word at 0x18
    cUIText* mpSubject[8];          // 0x0FC [R] zeroed; [U] widgets "Subject%d" (vt slot 27 called with 1 on creation);
                                    //       func_001E4C78 / func_001E4F70 set the subject text
    cUIText* mpNumber[8];           // 0x11C [R] zeroed; [U] widgets "Number%d"; func_001E4C78 cUIText_setAsciiString("%d.")
    cUIText* mpTotalMessagesLabel;  // 0x13C [R] zeroed; [U] widget "TotalMessagesLabel"; func_001E4758 prints the count
                                    //       (kT_OVRCMNTotalMsg)
    cUIText* mpUnreadMessagesLabel; // 0x140 [R] zeroed; [U] widget "UnreadMessagesLabel"; func_001E4758 (kT_OVRCMNUnread)
    cUIMenu* mpMenu;                // 0x144 [R] zeroed; [U] widget "MenuFroms"; cUIMenu_setScrollableMenu (cOVState_MESSAGECENTER_onCreateScreen,
                                    //       func_001E48B0), cUIMenu_setSelectedByIndex (cOVState_MESSAGECENTER_onGainFocus); its byte 0x95 is
                                    //       read/written as the selected row (func_001E4458, func_001E5148, cOVState_MESSAGECENTER_onWidgetEvent)
    cUIThing* mpNoMessage;          // 0x148 [U] widget "NoMessage" (hidden on creation, vt slot 9); not zeroed by the ctor
    cUIThing* mpButtonSquare;       // 0x14C [R] zeroed; [U] widget named by D_004A20A8 ("ps2sq"); func_001E4458 /
                                    //       func_001E48B0 show or hide it
    cUIThing* mpButtonText3;        // 0x150 [R] zeroed; [U] widget "btext3"; shown/hidden with mpButtonSquare
    cUIThing* mpButtonCross;        // 0x154 [R] zeroed; [U] widget named by D_004A2098 ("ps2x"); func_001E48B0
    cUIText* mpButtonText1;         // 0x158 [R] zeroed; [U] widget "btext1"; func_001E4458 sets kT_BTNView / Expand /
                                    //       Collapse
    cUIThing* mpArrowUp;            // 0x15C [R] zeroed; [U] widget "arr_up"; func_001E4658 shows it when mScrollOffset != 0
    cUIThing* mpArrowDown;          // 0x160 [R] zeroed; [U] widget "arr_down"; func_001E4658 shows it while rows remain below
    int miFolderTex;                // 0x164 [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("folder")); [U] func_001E4C78
    int miMinusFolderTex;           // 0x168 [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("minus folder")); [U] func_001E4C78
    int miQuestionMarkTex;          // 0x16C [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("questmark")); [U] func_001E4C78
    int mbMessageDeleted;           // 0x170 [U] onWidgetEvent (cOVState_MESSAGECENTER_onWidgetEvent) event 7 stores 0, then 1 once func_001E5148
                                    //       has removed the selected message (func_001E31B8); onWidgetMenu (cOVState_MESSAGECENTER_onWidgetMenu)
                                    //       event 8 returns it != 0; not set by the ctor (name is a guess from that use)
    int mScrollOffset;              // 0x174 [U] onCreateScreen zeroes it; onWidgetMenu event 4 stores the menu's value;
                                    //       func_001E48B0 / func_001E4658 / func_001E4C78 offset the rows by it (lw;
                                    //       func_001E48B0 also reads its low byte with lbu for the menu's byte 0x98)
    int unk_178;                    // 0x178 no access seen (placed by the 0x180 size)
    cUIScreen* mpMessageScreen;     // 0x17C [U] onCreateScreen (cOVState_MESSAGECENTER_onCreateScreen) stores the "112messagecenter" screen
                                    //       (cUIEngine_addScreenByHashName, then cUIScreen_playFrame); func_001E50C8 reads
                                    //       its word at 0xD0 (the screen's engine path to the state stack); onWidgetEvent
                                    //       plays its "TransitionOut" frame
};

CHECK_OFFSET(cOVState_MESSAGECENTER, mpPrev, 0x0);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpNext, 0x4);
CHECK_OFFSET(cOVState_MESSAGECENTER, mVtbl, 0x8);
CHECK_OFFSET(cOVState_MESSAGECENTER, mHashName, 0xC);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpEngine, 0x10);
CHECK_OFFSET(cOVState_MESSAGECENTER, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_MESSAGECENTER, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_MESSAGECENTER, unk_016, 0x16);
CHECK_OFFSET(cOVState_MESSAGECENTER, miMiscData, 0x18);
CHECK_OFFSET(cOVState_MESSAGECENTER, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpParentState, 0x20);
CHECK_OFFSET(cOVState_MESSAGECENTER, mScreens, 0x24);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpScreen, 0x40);
CHECK_OFFSET(cOVState_MESSAGECENTER, mUserNum, 0x44);
CHECK_OFFSET(cOVState_MESSAGECENTER, unk_045, 0x45);
CHECK_OFFSET(cOVState_MESSAGECENTER, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_MESSAGECENTER, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_MESSAGECENTER, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_MESSAGECENTER, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_MESSAGECENTER, mOVID, 0x98);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpUnreadMail, 0x9C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpFAQIcon, 0xBC);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpFrom, 0xDC);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpSubject, 0xFC);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpNumber, 0x11C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpTotalMessagesLabel, 0x13C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpUnreadMessagesLabel, 0x140);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpMenu, 0x144);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpNoMessage, 0x148);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpButtonSquare, 0x14C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpButtonText3, 0x150);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpButtonCross, 0x154);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpButtonText1, 0x158);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpArrowUp, 0x15C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpArrowDown, 0x160);
CHECK_OFFSET(cOVState_MESSAGECENTER, miFolderTex, 0x164);
CHECK_OFFSET(cOVState_MESSAGECENTER, miMinusFolderTex, 0x168);
CHECK_OFFSET(cOVState_MESSAGECENTER, miQuestionMarkTex, 0x16C);
CHECK_OFFSET(cOVState_MESSAGECENTER, mbMessageDeleted, 0x170);
CHECK_OFFSET(cOVState_MESSAGECENTER, mScrollOffset, 0x174);
CHECK_OFFSET(cOVState_MESSAGECENTER, unk_178, 0x178);
CHECK_OFFSET(cOVState_MESSAGECENTER, mpMessageScreen, 0x17C);
CHECK_SIZE(cOVState_MESSAGECENTER, 0x180);

struct cOVState_ViewPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_ViewPopup_vtbl, stored by cOVState_ViewPopup_cOVState_ViewPopup and the destructor cOVState_ViewPopup__cOVState_ViewPopup
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument (the message center's mpEngine);
                                    //       [U] cUIEngine_addScreenByHashName (cOVState_ViewPopup_onCreateScreen)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's parent argument (the cOVState_MESSAGECENTER);
                                    //       [U] cOVState_ViewPopup_onWidgetEvent / func_001E58E0 send it onPopupEvent(this, 0x14)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens; [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen (cOVState_ViewPopup_onCreateScreen) stores the "113ViewMessage" screen
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState_ViewPopup
    cUIText* mpNumberLabel;         // 0x048 [R] cOVState_ViewPopup_cOVState_ViewPopup zeroes it; [U] onWidgetCreate (cOVState_ViewPopup_onWidgetCreate): widget "NumberLabel"
    cUIText* mpNumber;              // 0x04C [R] zeroed; [U] widget "Number"; func_001E5428 prints the message number
    cUIText* mpFrom;                // 0x050 [R] zeroed; [U] widget "From"; func_001E5428 cUIText_setUnicodeStringByID
    cUIText* mpSubject;             // 0x054 [R] zeroed; [U] widget "Subject"; func_001E55D0 / func_001E5638 set it
    cUIText* mpMessageLine0;        // 0x058 [R] zeroed; [U] widget "MessageLine0"; func_001E5780 sets the body text
    cUIThing* mpButtonSquare;       // 0x05C [R] zeroed; [U] widget named by D_004A20A8 ("ps2sq"); func_001E5428 shows/hides it
    cUIThing* mpDeleteMessage;      // 0x060 [R] zeroed; [U] widget "MV_DeleteMessage"; func_001E5428 shows/hides it
    cUIText* mpKeepMessage;         // 0x064 [R] zeroed; [U] widget "MV_KeepMessage"; func_001E5428 sets kT_BTNKeepMsg
    int miMessage;                  // 0x068 [R] the ctor's 4th argument; [U] index into the message list D_004C6C08
                                    //       (func_001E5428, cOVState_ViewPopup_onWidgetMenu); func_001E58E0 steps it to the next message
    int miNumber;                   // 0x06C [R] the ctor's 5th argument; [U] func_001E5428 prints it with "%d"
                                    //       (D_004A2138); func_001E58E0 increments / recomputes it
};

CHECK_OFFSET(cOVState_ViewPopup, mpPrev, 0x0);
CHECK_OFFSET(cOVState_ViewPopup, mpNext, 0x4);
CHECK_OFFSET(cOVState_ViewPopup, mVtbl, 0x8);
CHECK_OFFSET(cOVState_ViewPopup, mHashName, 0xC);
CHECK_OFFSET(cOVState_ViewPopup, mpEngine, 0x10);
CHECK_OFFSET(cOVState_ViewPopup, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_ViewPopup, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_ViewPopup, unk_016, 0x16);
CHECK_OFFSET(cOVState_ViewPopup, miMiscData, 0x18);
CHECK_OFFSET(cOVState_ViewPopup, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_ViewPopup, mpParentState, 0x20);
CHECK_OFFSET(cOVState_ViewPopup, mScreens, 0x24);
CHECK_OFFSET(cOVState_ViewPopup, mpScreen, 0x40);
CHECK_OFFSET(cOVState_ViewPopup, mUserNum, 0x44);
CHECK_OFFSET(cOVState_ViewPopup, unk_045, 0x45);
CHECK_OFFSET(cOVState_ViewPopup, mpNumberLabel, 0x48);
CHECK_OFFSET(cOVState_ViewPopup, mpNumber, 0x4C);
CHECK_OFFSET(cOVState_ViewPopup, mpFrom, 0x50);
CHECK_OFFSET(cOVState_ViewPopup, mpSubject, 0x54);
CHECK_OFFSET(cOVState_ViewPopup, mpMessageLine0, 0x58);
CHECK_OFFSET(cOVState_ViewPopup, mpButtonSquare, 0x5C);
CHECK_OFFSET(cOVState_ViewPopup, mpDeleteMessage, 0x60);
CHECK_OFFSET(cOVState_ViewPopup, mpKeepMessage, 0x64);
CHECK_OFFSET(cOVState_ViewPopup, miMessage, 0x68);
CHECK_OFFSET(cOVState_ViewPopup, miNumber, 0x6C);
CHECK_SIZE(cOVState_ViewPopup, 0x70);

#endif /* FE_MESSAGECENTER_H */
