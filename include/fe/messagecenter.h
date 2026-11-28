#ifndef FE_MESSAGECENTER_H
#define FE_MESSAGECENTER_H
#pragma interface

// The in-game message center (EA's d:/ssx/code/source/fe/messagecenter.cpp, preview assert strings; retail keeps its
// code in fe/fepopupmisc 0x1E12F0-0x1E5AA0). Two UI states live here:
//
// cOVState_MESSAGECENTER: the overlay listing the saved messages (screen "112messagecenter": From / Subject / Number
// columns, unread-mail and FAQ icons, a scrollable "MenuFroms" menu). EA's name from the preview's linkonce sections
// (__22cOVState_MESSAGECENTERP9cUIEngine12tOVStateEnum, _$_22cOVState_MESSAGECENTER). Size 0x180: the overlay
// factory (cOVStateManager_triggerOverlay, fe/ovtemplatedialog.cpp) does operator new(0x180, "cOVStateOverlay") and
// calls the out-of-line constructor cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(this, pEngine, 0x22). Derives from
// cOVState (fe/ovstate.h, 0x9C). Retail vtable cOVState_MESSAGECENTER_vtbl (stored by
// cOVState_MESSAGECENTER_cOVState_MESSAGECENTER after cOVState's cOVState_vtbl; destructor
// cOVState_MESSAGECENTER__cOVState_MESSAGECENTER in fe/all_in_one_fe).
//
// cOVState_ViewPopup: the popup that shows one message (screen "113ViewMessage"), pushed by
// cOVState_MESSAGECENTER (cOVState_MESSAGECENTER_openViewPopup: cMemMan_alloc(0x70, "cOVState_ViewPopup") + constructor cOVState_ViewPopup_cOVState_ViewPopup(this,
// pEngine, pParent, message, number)). Size 0x70. Base cUIState (ui/uistate.h, 0x48) (it is not a cOVState: the
// constructor is cUIState(cUIEngine*, cUIState*) cUIState_cUIState1 and nothing stores 0x48..0x9C as cOVState does).
// Retail vtable cOVState_ViewPopup_vtbl (destructor cOVState_ViewPopup__cOVState_ViewPopup in fe/all_in_one_fe).
//
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. There is no SSX On Tour DWARF for
// these classes and the preview keeps no accessors, so the own members carry [R]/[U]
// evidence only; widget pointers are named after the UI object onWidgetCreate binds to them (names read from the
// retail ELF). Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*) cUIState_cUIState / cUIState(cUIEngine*, cUIState*) cUIState_cUIState1,
//       cOVState(cUIEngine*, tOVStateEnum) (inlined in cOVState_MESSAGECENTER_cOVState_MESSAGECENTER), cOVState_MESSAGECENTER_cOVState_MESSAGECENTER, cOVState_ViewPopup_cOVState_ViewPopup, and the
//       allocation sizes above;
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the inherited cListNode / cUIState / cOVState members (see fe/ovstate.h).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThing;
struct cUIText;
struct cUIMenu;

// The message group type (+0x0 of a messageCenter_groupTable group) of the FAQ folders (names ours). [U] the message center counts
// the FAQ rows with countMessageRows / getMessageForRow for it, cOVState_ViewPopup never deletes such a message, and its
// next-message step (cOVState_ViewPopup_showNextMessage) walks the folder instead of the queue.
enum { kMessageGroupFAQ = 4 };

struct cOVState_MESSAGECENTER : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_MESSAGECENTER_vtbl, stored by cOVState_MESSAGECENTER_cOVState_MESSAGECENTER (after cOVState's
    //       cOVState_vtbl)
    // cOVState_MESSAGECENTER's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument; [U] cUIEngine_addScreenByHashName
    //       (cOVState_MESSAGECENTER_onCreateScreen), cUITextureBank_getElementByHashname(mpEngine + 0x58, hash) in the
    //       ctor
    //   miMiscData: [U] here the opening mode: the ctor stores 0/1 (hint 7 not yet / already seen, isMessageFlagSet(7))
    //       or 2 (cOVState_MESSAGECENTER_openRequest set); onCreateScreen/onDestroyScreen/onGainFocus switch on it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_MESSAGECENTER
    cUIThing* mpUnreadMail[8];      // 0x09C [R] ctor zeroes 8; [U] onWidgetCreate (cOVState_MESSAGECENTER_onWidgetCreate): widgets "UnreadMail%d";
                                    //       cOVState_MESSAGECENTER_fillRows/cOVState_MESSAGECENTER_fillRow show/hide row i's
    cUIThing* mpFAQIcon[8];         // 0x0BC [R] zeroed; [U] widgets "FAQIcon%d"; cOVState_MESSAGECENTER_fillRow sets them to the folder /
                                    //       minus-folder / question-mark texture (miFolderTex ...)
    cUIText* mpFrom[8];             // 0x0DC [R] zeroed; [U] widgets "From%d"; cOVState_MESSAGECENTER_fillRow cUIText_setUnicodeStringByID and
                                    //       stores the message id in the widget's word at 0x18
    cUIText* mpSubject[8];          // 0x0FC [R] zeroed; [U] widgets "Subject%d" (vt slot 27 called with 1 on creation);
                                    //       cOVState_MESSAGECENTER_fillRow / cOVState_MESSAGECENTER_setRowSubject set the subject text
    cUIText* mpNumber[8];           // 0x11C [R] zeroed; [U] widgets "Number%d"; cOVState_MESSAGECENTER_fillRow cUIText_setAsciiString("%d.")
    cUIText* mpTotalMessagesLabel;  // 0x13C [R] zeroed; [U] widget "TotalMessagesLabel"; cOVState_MESSAGECENTER_showMessageCounts prints the count
                                    //       (kT_OVRCMNTotalMsg)
    cUIText* mpUnreadMessagesLabel; // 0x140 [R] zeroed; [U] widget "UnreadMessagesLabel"; cOVState_MESSAGECENTER_showMessageCounts (kT_OVRCMNUnread)
    cUIMenu* mpMenu;                // 0x144 [R] zeroed; [U] widget "MenuFroms"; cUIMenu_setScrollableMenu (cOVState_MESSAGECENTER_onCreateScreen,
                                    //       cOVState_MESSAGECENTER_fillRows), cUIMenu_setSelectedByIndex (cOVState_MESSAGECENTER_onGainFocus); its byte 0x95 is
                                    //       read/written as the selected row (cOVState_MESSAGECENTER_updateButtons, cOVState_MESSAGECENTER_deleteMessage, cOVState_MESSAGECENTER_onWidgetEvent)
    cUIThing* mpNoMessage;          // 0x148 [U] widget "NoMessage" (hidden on creation, vt slot 9); not zeroed by the ctor
    cUIThing* mpButtonSquare;       // 0x14C [R] zeroed; [U] widget named by cOVState_MESSAGECENTER_squareButtonName ("ps2sq"); cOVState_MESSAGECENTER_updateButtons /
                                    //       cOVState_MESSAGECENTER_fillRows show or hide it
    cUIThing* mpButtonText3;        // 0x150 [R] zeroed; [U] widget "btext3"; shown/hidden with mpButtonSquare
    cUIThing* mpButtonCross;        // 0x154 [R] zeroed; [U] widget named by cOVState_MESSAGECENTER_crossButtonWidgetName ("ps2x"); cOVState_MESSAGECENTER_fillRows
    cUIText* mpButtonText1;         // 0x158 [R] zeroed; [U] widget "btext1"; cOVState_MESSAGECENTER_updateButtons sets kT_BTNView / Expand /
                                    //       Collapse
    cUIThing* mpArrowUp;            // 0x15C [R] zeroed; [U] widget "arr_up"; cOVState_MESSAGECENTER_updateScrollArrows shows it when mScrollOffset != 0
    cUIThing* mpArrowDown;          // 0x160 [R] zeroed; [U] widget "arr_down"; cOVState_MESSAGECENTER_updateScrollArrows shows it while rows remain below
    int miFolderTex;                // 0x164 [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("folder")); [U] cOVState_MESSAGECENTER_fillRow
    int miMinusFolderTex;           // 0x168 [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("minus folder")); [U] cOVState_MESSAGECENTER_fillRow
    int miQuestionMarkTex;          // 0x16C [R] = cUITextureBank_getElementByHashname(mpEngine + 0x58, hash("questmark")); [U] cOVState_MESSAGECENTER_fillRow
    int mbMessageDeleted;           // 0x170 [U] onWidgetEvent (cOVState_MESSAGECENTER_onWidgetEvent) event 7 stores 0, then 1 once cOVState_MESSAGECENTER_deleteMessage
                                    //       has removed the selected message (removeMessageAt); onWidgetMenu (cOVState_MESSAGECENTER_onWidgetMenu)
                                    //       event 8 returns it != 0; not set by the ctor (name is a guess from that use)
    int mScrollOffset;              // 0x174 [U] onCreateScreen zeroes it; onWidgetMenu event 4 stores the menu's value;
                                    //       cOVState_MESSAGECENTER_fillRows / cOVState_MESSAGECENTER_updateScrollArrows / cOVState_MESSAGECENTER_fillRow offset the rows by it (lw;
                                    //       cOVState_MESSAGECENTER_fillRows also reads its low byte with lbu for the menu's byte 0x98)
    int unk_178;                    // 0x178 no access seen (placed by the 0x180 size)
    cUIScreen* mpMessageScreen;     // 0x17C [U] onCreateScreen (cOVState_MESSAGECENTER_onCreateScreen) stores the "112messagecenter" screen
                                    //       (cUIEngine_addScreenByHashName, then cUIScreen_playFrame); cOVState_MESSAGECENTER_openViewPopup reads
                                    //       its word at 0xD0 (the screen's engine path to the state stack); onWidgetEvent
                                    //       plays its "TransitionOut" frame

    // Methods bound to their retail symbols by asm labels.
    virtual ~cOVState_MESSAGECENTER();   // g++'s own symbol _$_22cOVState_MESSAGECENTER (the retail label renamed)
    virtual void onCreateScreen() __asm__("cOVState_MESSAGECENTER_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cOVState_MESSAGECENTER_onDestroyScreen");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cOVState_MESSAGECENTER_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cOVState_MESSAGECENTER_onWidgetEvent");
    virtual void onGainFocus(cUIState*) __asm__("cOVState_MESSAGECENTER_onGainFocus");
    virtual int onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int value)
        __asm__("cOVState_MESSAGECENTER_onWidgetMenu");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cOVState_MESSAGECENTER_onPopupEvent");
    void updateButtons() __asm__("cOVState_MESSAGECENTER_updateButtons");
    void updateScrollArrows() __asm__("cOVState_MESSAGECENTER_updateScrollArrows");
    void showMessageCounts() __asm__("cOVState_MESSAGECENTER_showMessageCounts");
    void hideWidget(cUIThing* obj) __asm__("cOVState_MESSAGECENTER_hideWidget");
    void showWidget(cUIThing* obj) __asm__("cOVState_MESSAGECENTER_showWidget");
    void fillRows() __asm__("cOVState_MESSAGECENTER_fillRows");
    unsigned char getFAQSubjectVariant(int idx) __asm__("cOVState_MESSAGECENTER_getFAQSubjectVariant");
    void fillRow(int idx, int slot, int id, unsigned char sub) __asm__("cOVState_MESSAGECENTER_fillRow");
    void setRowSubject(int slot, int id, unsigned char lvl) __asm__("cOVState_MESSAGECENTER_setRowSubject");
    void openViewPopup(int messageIndex, int messageNumber) __asm__("cOVState_MESSAGECENTER_openViewPopup");
    void deleteMessage(int idx) __asm__("cOVState_MESSAGECENTER_deleteMessage");
};

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
__asm__("_vt$22cOVState_MESSAGECENTER = cOVState_MESSAGECENTER_vtbl");   // g++'s name for the retail vtable

struct cOVState_ViewPopup : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_ViewPopup_vtbl, stored by cOVState_ViewPopup_cOVState_ViewPopup and the destructor
    //       cOVState_ViewPopup__cOVState_ViewPopup
    // cOVState_ViewPopup's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument (the message center's mpEngine); [U] cUIEngine_addScreenByHashName
    //       (cOVState_ViewPopup_onCreateScreen)
    //   mpParentState: [R] the ctor's parent argument (the cOVState_MESSAGECENTER); [U]
    //       cOVState_ViewPopup_onWidgetEvent / cOVState_ViewPopup_showNextMessage send it onPopupEvent(this, 0x14)
    //   mpScreen: [U] onCreateScreen (cOVState_ViewPopup_onCreateScreen) stores the "113ViewMessage" screen
    // --- cOVState_ViewPopup
    cUIText* mpNumberLabel;         // 0x048 [R] cOVState_ViewPopup_cOVState_ViewPopup zeroes it; [U] onWidgetCreate (cOVState_ViewPopup_onWidgetCreate): widget "NumberLabel"
    cUIText* mpNumber;              // 0x04C [R] zeroed; [U] widget "Number"; cOVState_ViewPopup_fillMessage prints the message number
    cUIText* mpFrom;                // 0x050 [R] zeroed; [U] widget "From"; cOVState_ViewPopup_fillMessage cUIText_setUnicodeStringByID
    cUIText* mpSubject;             // 0x054 [R] zeroed; [U] widget "Subject"; cOVState_ViewPopup_setSubject / cOVState_ViewPopup_setVariantSubject set it
    cUIText* mpMessageLine0;        // 0x058 [R] zeroed; [U] widget "MessageLine0"; cOVState_ViewPopup_setBody sets the body text
    cUIThing* mpButtonSquare;       // 0x05C [R] zeroed; [U] widget named by cOVState_MESSAGECENTER_squareButtonName ("ps2sq"); cOVState_ViewPopup_fillMessage shows/hides it
    cUIThing* mpDeleteMessage;      // 0x060 [R] zeroed; [U] widget "MV_DeleteMessage"; cOVState_ViewPopup_fillMessage shows/hides it
    cUIText* mpKeepMessage;         // 0x064 [R] zeroed; [U] widget "MV_KeepMessage"; cOVState_ViewPopup_fillMessage sets kT_BTNKeepMsg
    int miMessage;                  // 0x068 [R] the ctor's 4th argument; [U] index into the message list messageCenter_messageTable
                                    //       (cOVState_ViewPopup_fillMessage, cOVState_ViewPopup_onWidgetMenu); cOVState_ViewPopup_showNextMessage steps it to the next message
    int miNumber;                   // 0x06C [R] the ctor's 5th argument; [U] cOVState_ViewPopup_fillMessage prints it with "%d"
                                    //       (str_pct_d_j); cOVState_ViewPopup_showNextMessage increments / recomputes it

    // Methods bound to their retail symbols by asm labels.
    virtual ~cOVState_ViewPopup();   // g++'s own symbol _$_18cOVState_ViewPopup (the retail label renamed)
    virtual void onCreateScreen() __asm__("cOVState_ViewPopup_onCreateScreen");
    virtual bool onGainTransition(int on) __asm__("cOVState_ViewPopup_onGainTransition");
    virtual void onWidgetCreate(cUIThing* widget) __asm__("cOVState_ViewPopup_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cOVState_ViewPopup_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
        __asm__("cOVState_ViewPopup_onWidgetMenu");
    void fillMessage() __asm__("cOVState_ViewPopup_fillMessage");
    void setSubject(int idx) __asm__("cOVState_ViewPopup_setSubject");
    void setVariantSubject(int id) __asm__("cOVState_ViewPopup_setVariantSubject");
    void setBody(int idx) __asm__("cOVState_ViewPopup_setBody");
    void showNextMessage() __asm__("cOVState_ViewPopup_showNextMessage");
    unsigned char getSubjectVariant(int idx) __asm__("cOVState_ViewPopup_getSubjectVariant");
};

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
__asm__("_vt$18cOVState_ViewPopup = cOVState_ViewPopup_vtbl");   // g++'s name for the retail vtable

#endif /* FE_MESSAGECENTER_H */
