#ifndef FE_FEPOPUPOPTIONRESET_H
#define FE_FEPOPUPOPTIONRESET_H

// cFEPopupOptionReset: the options screens' yes / no popup ("111resetopts": save or reset the changed options?). The
// options states allocate it, construct it inline and push it as a popup; their onPopupEvent then read the answer
// through getData (vtable slot 23).
// EA's source file is not known (On Tour: fe/all_in_one_fe; the users are in fe/festateoptions.cpp). Retail code: the
// dtor, onCreateScreen, onWidgetMenu, onWidgetEvent and getData in main/all_in_one_main (0x001DE9F8 ... 0x001DEB50);
// there is no out-of-line ctor.
//
// Size 0x4C:
//   [R] cMemMan_alloc(76, "cFEPopupOptionReset") + cUIState_cUIState1(mem, mpEngine, this state), vtable, byte 0x48 = 0
//       in cFEStateOptions_saveOptionPrompt and cFEStateOptionsGame / HUD / Sound / Controller_onWidgetEvent;
//   [O] On Tour's class is 0x4C: char mAccept at 0x48.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (slot 6 is cUIState_onGainFocus; the
// dtor stores cFEPopupOptionReset_vtbl and runs cUIState__cUIState). Vtable cFEPopupOptionReset_vtbl (0x0046B3D0,
// 24 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent, 20 onWidgetMenu, 23 getData).
// Evidence tags as below, and [P] the July 2003 preview's getData (an lb, like retail's).
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags: [R] retail's
// constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEPopupOptionReset : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEPopupOptionReset_vtbl (the allocators, dtor)
    // cFEPopupOptionReset's notes on cUIState's fields:
    //   mHashName: [U] onCreateScreen stores the screen's hash ("111resetopts")
    //   mpEngine: [U] onCreateScreen adds the screen; onWidgetEvent pops its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen
    // --- cFEPopupOptionReset
    char mAccept;                   // 0x048 [O][P] getData (lb); [R] the allocators store 0 (sb); [U] onWidgetEvent: 1
                                    //       on "yes", 0 on "no" (sb), then pops the popup; getData returns it
    char unk_049[0x3];              // 0x049 padding

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEPopupOptionReset__cFEPopupOptionReset stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEPopupOptionReset();
    virtual void onCreateScreen() __asm__("cFEPopupOptionReset_onCreateScreen");
};

CHECK_OFFSET(cFEPopupOptionReset, mAccept, 0x048);
CHECK_OFFSET(cFEPopupOptionReset, unk_049, 0x049);
CHECK_SIZE(cFEPopupOptionReset, 0x4C);

#endif /* FE_FEPOPUPOPTIONRESET_H */
