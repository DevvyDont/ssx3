#ifndef FE_FESTATENETQUICKMESSAGES_H
#define FE_FESTATENETQUICKMESSAGES_H

// cFEStateNetQuickMessages: screen "206QuickMessages": edits the ten online quick-chat messages ("Q1: ..".."Q10: ..",
// stored by the option interface, cBE_getInterface(.., 4), at D_00535617 + 0x40 * i) and offers to save them. A
// cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the allocation tag only (the preview keeps no inline of it).
// EA's source file is not known (its code follows cFEStateNetSSXWorld's and precedes cFEStateStats', in both builds).
//
// Size 0x6FC: cMemMan_alloc(0x6FC, "cFEStateNetQuickMessages") + cFEStateNetQuickMessages_cFEStateNetQuickMessages(mem, pEngine) (cFEStateNetSSXWorld_onWidgetEvent, the only
// site); mpMessage[9] ends at 0x6FC. Constructors cFEStateNetQuickMessages_cFEStateNetQuickMessages (cFEStateNet(pEngine, 0), vtable cFEStateNetQuickMessages_vtbl,
// mHashName
// 0x4B) and the default cFEStateNetQuickMessages_cFEStateNetQuickMessages1 (fe/fepopupmisc); destructor cFEStateNetQuickMessages__cFEStateNetQuickMessages (fe/fepopupmisc; slot 1). Retail
// vtable cFEStateNetQuickMessages_vtbl (32 slots: 4 onCreateScreen cFEStateNetQuickMessages_onCreateScreen, 7 cFEStateNetQuickMessages_onGainTransition, 12 onUpdate cFEStateNetQuickMessages_onUpdate, 17
// onPopupEvent cFEStateNetQuickMessages_onPopupEvent, 18 onWidgetCreate cFEStateNetQuickMessages_onWidgetCreate, 19 onWidgetEvent cFEStateNetQuickMessages_onWidgetEvent, 25 cFEStateNetQuickMessages_fillHelpPopup
// (help), 31 cFEStateNet's cFEStateNet_onNetEvent). Methods: fe/feasyncfile 0x1C20F8-0x1C2C50.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
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
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cFEStateNetQuickMessages : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetQuickMessages_vtbl (vtables.tsv; 32 slots), stored by
    //       cFEStateNetQuickMessages_cFEStateNetQuickMessages / cFEStateNetQuickMessages_cFEStateNetQuickMessages1
    //       after the cFEStateNet ctor and by the destructor cFEStateNetQuickMessages__cFEStateNetQuickMessages
    // cFEStateNetQuickMessages's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetQuickMessages_cFEStateNetQuickMessages
    //       stores 0x4B
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetQuickMessages
    int mbChanged;                  // 0x6D0 [U] onCreateScreen cFEStateNetQuickMessages_onCreateScreen clears it; cFEStateNetQuickMessages_handleKeyboardText (a message
                                    //       edited) and cFEStateNetQuickMessages_onResetMessagesPopupEvent (popup 0xF: defaults restored, cBEOptionInterface_resetAllQuickKeys)
                                    //       set 1; onWidgetEvent cFEStateNetQuickMessages_onWidgetEvent on "back" asks
                                    //       "kT_POPUP206SaveQuickMessage" when set, else returns to
                                    //       cFEStateNetSSXWorld
    cUIText* mpMessage[10];         // 0x6D4 [U] onWidgetCreate cFEStateNetQuickMessages_onWidgetCreate: widgets "message00".."message09"
                                    //       (miMiscData = i); cFEStateNetQuickMessages_showMessageRow writes "Q<i+1>: " + message i into
                                    //       mpMessage[i] (cFEStateNetQuickMessages_setFittedText)

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateNetQuickMessages1() __asm__("cFEStateNetQuickMessages_cFEStateNetQuickMessages1");
    // retail's destructor cFEStateNetQuickMessages__cFEStateNetQuickMessages stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetQuickMessages();
    virtual void onPopupEvent(cUIState*pPopup, unsigned int Event) __asm__("cFEStateNetQuickMessages_onPopupEvent");
    virtual void onCreateScreen() __asm__("cFEStateNetQuickMessages_onCreateScreen");
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateNetQuickMessages_onGainTransition");
    virtual void onUpdate() __asm__("cFEStateNetQuickMessages_onUpdate__FPv");
    virtual void onWidgetEvent(cUIThing* w, unsigned int msg) __asm__("cFEStateNetQuickMessages_onWidgetEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateNetQuickMessages_onWidgetCreate");
};

CHECK_OFFSET(cFEStateNetQuickMessages, mbChanged, 0x6D0);
CHECK_OFFSET(cFEStateNetQuickMessages, mpMessage, 0x6D4);
CHECK_SIZE(cFEStateNetQuickMessages, 0x6FC);

#endif /* FE_FESTATENETQUICKMESSAGES_H */
