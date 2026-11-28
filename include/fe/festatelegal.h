#ifndef FE_FESTATELEGAL_H
#define FE_FESTATELEGAL_H
#pragma interface

// cFEStateLegal: the front end's legal screen: shows the screen hashed from "122bAutosave" for 240 frames, then plays
// its "TransitionOut" and pops itself. Retail never constructs it (no caller of either ctor).
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: the (pEngine, pParent) ctor cFEStateLegal_cFEStateLegal1
// (splat puts it at the end of fe/fememcard), onCreateScreen / onUpdate in fe/festatelegal (0x00187C50, 0x00187CB8),
// and the engine-less ctor, the dtor and onGainTransition in main/all_in_one_main (0x001DE990 ... 0x001DE9F0).
//
// Size 0x4C:
//   [O] On Tour's class is 0x4C: int mDisplayTicks at 0x48;
//   [R] cFEStateLegal_cFEStateLegal1 clears the word at 0x48 (the last one).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (vtable slot 6 is
// cUIState_onGainFocus, and the dtor runs cUIState__cUIState right after its own vtable store). cFEStateLegal_cFEStateLegal1 runs
// cUIState_cUIState1(this, pEngine, pParent) (the C definition declares only this) and stores cFEStateLegal_vtbl
// (0x0046B4A0, 24 slots: 1 dtor, 4 onCreateScreen, 7 onGainTransition, 12 onUpdate).
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

struct cFEStateLegal : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateLegal_vtbl (ctors, dtor)
    // cFEStateLegal's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds the screen through it; onUpdate pops its state stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen; onUpdate plays its "TransitionOut" frame
    // --- cFEStateLegal
    int mDisplayTicks;              // 0x048 [O]; [R] cFEStateLegal_cFEStateLegal1 stores 0; [U] onCreateScreen 0; onUpdate counts it
                                    //       up and leaves at kDisplayTicks

    // How many frames the notice stays up (names ours): [U] onUpdate leaves when mDisplayTicks reaches it.
    enum { kDisplayTicks = 240 };

    // Methods bound to their retail symbols by asm labels.
    virtual ~cFEStateLegal();   // g++'s own symbol _$_13cFEStateLegal (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateLegal_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateLegal_onUpdate");
};

CHECK_OFFSET(cFEStateLegal, mDisplayTicks, 0x048);
CHECK_SIZE(cFEStateLegal, 0x4C);
__asm__("_vt$13cFEStateLegal = cFEStateLegal_vtbl");   // g++'s name for the retail vtable

#endif /* FE_FESTATELEGAL_H */
