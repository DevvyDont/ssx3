#ifndef FE_FESTATELOAD_H
#define FE_FESTATELOAD_H

// cFEStateEventSelect: the front end's event-select map (screens "Map" and "femap_template"): a cFEState around an
// embedded mountain-map template (cUITemplate_MAP, fe/uitemplatemap.h) that does all the work. Every virtual except
// onWidgetEvent only forwards to the same-named cUITemplate_MAP_* method with &mMap; onWidgetEvent pushes the next or
// previous FE state (cFECustom_getNextState / getPrevState) when the template accepts the event.
// EA's source file is not confirmed (the unit is named fe/festateload after the preview's strings). Retail code: the
// engine ctor cFEStateEventSelect_cFEStateEventSelect1 in fe/festatecredits (0x00186610), cFEStateEventSelect_* in
// fe/festateload (0x00186658 ... 0x001869D8), the engine-less ctor copy cFEStateEventSelect_cFEStateEventSelect,
// the destructor and onMenuGainFocus in main/all_in_one_main (0x001DE4E8 ... 0x001DE550).
//
// Size 0x33C:
//   [R] cMemMan_alloc(0x33C, "cFEStateEventSelect") + cFEStateEventSelect_cFEStateEventSelect1(mem, engine) in
//       cFECustom_getNextState (twice), cFECustom_getPrevState (twice) and cFEStateChallengePopup_onWidgetEvent;
//       0x48 + sizeof(cUITemplate_MAP) (0x2F4) = 0x33C, so nothing follows the template.
//   [O] On Tour 0x16C = cFEState 0x48 + cUITemplate_MAP mMap (0x124), nothing else; retail's template is 0x2F4.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// runs cUIState_cUIState(this, pEngine), stores 19 and cFEStateEventSelect_vtbl (D_0046BD28, 24 slots: 1 dtor, 4
// onCreateScreen, 5 onDestroyScreen, 6 onGainFocus, 7 onGainTransition__FPv, 8 onInputBegin, 9 onInputAlways, 12
// onUpdate, 13 onMenuGainFocus, 18 onWidgetCreate__FPv, 19 onWidgetEvent, 20 onWidgetMenu), then cUITemplate_MAP_init
// on &mMap. The destructor stores cFEState_vtbl and runs cUIState__cUIState (the template has no destructor).
//
// Derives from cFEState (fe/festate.h); its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFEStateEventSelect, cUIState, cListNode): names, types and order.
// (No preview evidence: layout_xlate finds no paired cFEStateEventSelect methods.)

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"
#include "fe/uitemplatemap.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateEventSelect : public cFEState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateEventSelect_vtbl (both ctors), cFEState_vtbl (dtor)
    // cFEStateEventSelect's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "Map" / "femap_template"; onWidgetEvent reaches the FE custom object and the
    //       state stack (+0x18) through it
    //   mpScreen: [U] onCreateScreen keeps the "Map" screen here; onWidgetEvent plays its "TransitionOut" frame
    // cFEState (fe/festate.h, 0x48)
    // --- cFEStateEventSelect
    cUITemplate_MAP mMap;           // 0x048 [O]; [R] the ctors run cUITemplate_MAP_init on it; [U] every method passes
                                    //       &mMap (addiu 0x48) to the same-named cUITemplate_MAP_* method

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateEventSelect1() __asm__("cFEStateEventSelect_cFEStateEventSelect1");
    // retail's destructor cFEStateEventSelect__cFEStateEventSelect stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateEventSelect();
    virtual void onCreateScreen() __asm__("cFEStateEventSelect_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cFEStateEventSelect_onDestroyScreen");
    virtual void onUpdate() __asm__("cFEStateEventSelect_onUpdate");
};

CHECK_OFFSET(cFEStateEventSelect, mMap, 0x048);
CHECK_SIZE(cFEStateEventSelect, 0x33C);

#endif /* FE_FESTATELOAD_H */
