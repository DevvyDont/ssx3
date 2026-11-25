#ifndef FE_FECUSTOM_H
#define FE_FECUSTOM_H

// cFECustom: the front end's custom interface to the UI engine (the engine's mpCustom, *(cUIEngine*)): builds the next
// / previous FE state for a state id and choice (getNextState / getPrevState), answers the "whitefade" screen event,
// keeps each player's controller mask (copied into new states' mCtrlBits) and owns the transition state.
// EA's source file is not known (no assert of its own; the retail unit is fe/fereal, EA's fereal.cpp); retail code:
// fe/fereal (0x001A0720 ... 0x001A1CD0) and the default-ctor copy, the destructor, onUpdate and setProfileLaunchScreen
// in main/all_in_one_main (0x001DCC60 ... 0x001DCCB0).
//
// Size 0x14:
//   [R] cMemMan_alloc(0x14, "FEUICustom") + cFECustom_cFECustom(mem, engine) in cRealFE_load; the ctor writes 0x0,
//       0x4 and 0x10, the methods reach 0x10.
//   [O] On Tour's class is 0x14 too, with the same members at the same offsets.
// Base: cUICustomInterface (On Tour size 0x8: mpEngine at 0x0, then the vptr at 0x4; vtable cUICustomInterface_vtbl
// 0x0046DD28, 5 slots). Vtable cFECustom_vtbl (0x0046D810, 5 slots: 1 dtor, 2 onUpdate, 3 onScreenEvent, 4
// getNextState, 5 getPrevState); the destructor stores cUICustomInterface_vtbl back.
//
// A real subclass of cUICustomInterface (ui/uiengine.h), its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x0 and 0x4 unchanged, from the paired ctor / dtor);
//   [U] how retail code uses the field (the C bodies in src/fe/fereal.cpp, main/all_in_one_main.cpp and the asm);
//   [O] SSX On Tour's DWARF (class cFECustom, cUICustomInterface): names, types and order.

#include "layout.h"
#include "ui/uiengine.h"

struct cFEStateTransition;

struct cFECustom : public cUICustomInterface {
    // cUICustomInterface (ui/uiengine.h): mpEngine 0x000 ([R] the ctor's engine; the default copy stores 0;
    // [U] getNextState / getPrevState build the states on it, the ctor pushes the transition state on its
    // stack, +0x18), the vptr 0x004 ([R] cFECustom_vtbl in the ctors, cUICustomInterface_vtbl in the dtor; [P])
    // --- cFECustom
    unsigned char mCtrlMask[2];     // 0x008 [O]; [U] setPlayerCtrlMask / getPlayerCtrlMask (sb / lbu at 0x8 + index);
                                    //       state ctors copy it into mCtrlBits
    char unk_00A[0x2];              // 0x00A padding
    unsigned int mProfileLaunchScreen; // 0x00C [O]; [U] setProfileLaunchScreen and getNextState store it,
                                    //       getNextState / getPrevState switch on it (24 options save/load, 9 character
                                    //       select, 39 lodge)
    cFEStateTransition* mpTransitionState; // 0x010 [O]; [R] ctor: the "cFEStateTransition" state it allocates and
                                    //       pushes; [U] onScreenEvent sends it handleEvent on "whitefade",
                                    //       getNextState reads it

    // Methods bound to their retail symbols by asm labels.
    void setPlayerCtrlMask(int index, int val) __asm__("cFECustom_setPlayerCtrlMask");
    unsigned char getPlayerCtrlMask(int index) __asm__("cFECustom_getPlayerCtrlMask");
    void * cFECustom1() __asm__("cFECustom_cFECustom1");
    void setProfileLaunchScreen(int launchScreen) __asm__("cFECustom_setProfileLaunchScreen");
};

CHECK_OFFSET(cFECustom, mCtrlMask, 0x008);
CHECK_OFFSET(cFECustom, unk_00A, 0x00A);
CHECK_OFFSET(cFECustom, mProfileLaunchScreen, 0x00C);
CHECK_OFFSET(cFECustom, mpTransitionState, 0x010);
CHECK_SIZE(cFECustom, 0x14);

#endif /* FE_FECUSTOM_H */
