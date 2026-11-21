#ifndef BE_BEINTOPTION_H
#define BE_BEINTOPTION_H

// cBEOptionInterface: the back end's options interface - language, units, widescreen, brightness, replay, autosave,
// the sound settings (mode, mix, volumes, arcade effects, DJ speech, voice chat), the HUD detail, the screen offsets
// and the online quick-chat messages, with their defaults (defaultSound/Game/HUD/Ctrl). The object holds no options:
// like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and
// work on the static cBEOptionInterface::mOption (retail cBEOptionInterface_mOption, D_00535610, a 0x288-byte
// tBEOptionDB: getOptions / setOptions copy it whole; not modelled here); submit copies it to the saved copy cBELibrary_optionDB
// (On Tour: cBELibrary::optionDB), discard copies it back, revert resets the defaults. One instance, created on demand
// by cBEOptionInterface_getThis
// (`cMemMan_alloc(0x10, "BEOptionInt", 0, 0)`, vtable cBEOptionInterface_vtbl, then cBEOptionInterface_revert, then
// bIsActive = 0) into cBEOptionInterface::instance (retail cBEOptionInterface_instance, D_004A120C); the rest of the
// game reaches it as cBE_getInterface(cBE_getBE(), 4). EA's source: d:/ssx/code/source/be/beintoption.cpp, with
// inline methods in ../source/be/beintoption.h (preview assert strings).
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEOptionInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// cBEOptionInterface_vtbl (0x45ADC8), slot 1 submit = cBEOptionInterface_submit, slot 2 discard =
// cBEOptionInterface_discard, slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, vptr, bIsActive) and constructor cBEOptionInterface_cBEOptionInterface (vptr);
//   [P] the July 2003 preview's inline methods: __18cBEOptionInterface stores the vtable at 0xC; its 22 other inline
//       accessors address mOption, not `this`;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"

struct cBEOptionInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U]
                                     //       cBEOptionInterface_setOptionActive (static: instance->bIsActive = arg);
                                     //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit,
                                     //       cBERaceState_onEnter/onExit set 0/1
    void** mVtbl;                    // 0x00C [R] getThis and cBEOptionInterface_cBEOptionInterface store
                                     //       cBEOptionInterface_vtbl; [P] __18cBEOptionInterface; [U]
                                     //       defaultSound/defaultGame/defaultHUD/defaultCtrl and func_001C2BA8 call
                                     //       slot 1 (submit) through it, cBESaveInterface_loadSaveGame slot 2 (discard)
};

extern cBEOptionInterface* cBEOptionInterface_instance;

CHECK_OFFSET(cBEOptionInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEOptionInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEOptionInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEOptionInterface, mVtbl, 0x0C);
CHECK_SIZE(cBEOptionInterface, 0x10);

// The one options interface (On Tour: cBEOptionInterface::instance), created by cBEOptionInterface_getThis.

#endif /* BE_BEINTOPTION_H */
