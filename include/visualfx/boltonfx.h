#ifndef VISUALFX_BOLTONFX_H
#define VISUALFX_BOLTONFX_H

// cBoltOnFXInterface: a rider's (or a front-end character's) bolt-on effect set - the attached particle emitters its
// equipment ("bolts": board, outfit and accessory parts) carries. It keeps up to 15 emitters, each created on an item
// (attach point) in the rider's cAttachedEmitterFX, and rebuilds them from the character's bolt definitions when marked
// dirty. EA source: d:/ssx/code/source/visualfx/boltonfx.cpp (July 2003 preview: assert strings
// "mAttachedEmitterFX!=NULL", "Ran out of bolt on emitters", "The bolt on interface has to have some sort of rider
// known to it", "pCharBolt!=NULL" with that file name). The retail methods sit in the splat unit visualfx/lensfx
// (0x2EC9D0..0x2ECFA0).
// Class name: [R] the allocation tag "cBoltOnFXInterface" (cRider_cRider and cFERider_init: cMemMan_alloc(200,
// "cBoltOnFXInterface") + its constructor) and the preview's vtable _vt$18cBoltOnFXInterface (builds/x/v2: its slot 1
// matches cBoltOnFXInterface__cBoltOnFXInterface).
//
// Size 0xC8 [R] (cMemMan_alloc(200, ...)). No base class; the class declares a virtual destructor, so g++ 2.95 puts
// the vptr after its data, at +0xC4. Vtable cBoltOnFXInterface_vtbl (1 slot: the destructor cBoltOnFXInterface__cBoltOnFXInterface), stored by both
// constructors and the destructor.
// Owners: cRider::mBoltOnFX (+0x890, ai/rider.h; built with the rider's mAttachedEmitterFX), cFERider::mpBoltOnFX
// (+0xC74, fe/ferider.h). Each frame cRider_updateSplitMiscFX / cFERider_update call func_002ECF78 (rebuild when dirty).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructors cBoltOnFXInterface_cBoltOnFXInterface (rider) / cBoltOnFXInterface_cBoltOnFXInterface1 (front-end character id), the destructor cBoltOnFXInterface__cBoltOnFXInterface,
//       the allocation;
//   [P] the preview's assert strings (member names);
//   [U] how retail code uses the field.
// On Tour has no bolt-on class: no [O] evidence. Names marked "(our name)" describe the use.

#include "layout.h"

struct cRider;
struct cAttachedEmitterFX; // preview class name (cAttachedEmitterFX::ShouldDisableSnowTrailSpray); no header yet

// One bolt-on emitter slot (0xC; our name and member names). [R] func_002EC9D0 (the element constructor, run 15 times
// by both constructors) sets mEmitter = -1; func_002ECAF0 empties all 15.
struct tBoltOnFXEntry {
    // A free slot's mEmitter (name ours): [R] func_002EC9D0 sets it; [U] the dtor, func_002ECBC0, func_002ECC28 and
    // func_002ECCB8 treat it as no emitter.
    enum { kNoEmitter = -1 };
    int mEmitter;   // 0x0 (our name) the cAttachedEmitterFX emitter index, -1 = free: [U] func_002ECC28 stores
                    //     func_002F2130's result; cBoltOnFXInterface__cBoltOnFXInterface / func_002ECCB8 release it with func_002F2218
    int mItem;      // 0x4 (our name) the item (attach point) id: [U] func_002ECC28 stores, func_002ECBC0 matches
    int mKind;      // 0x8 (our name) the particle effect kind: [U] func_002ECC28 stores, func_002ECBC0 matches
};

struct cBoltOnFXInterface {
    int mDirty;                             // 0x00 (our name) [R] ctors 1; [U] func_002ECF78 rebuilds while set,
                                            //      func_002ECCB8 clears it
    cRider* mpRider;                        // 0x04 (our name) [R] cBoltOnFXInterface_cBoltOnFXInterface stores the rider, cBoltOnFXInterface_cBoltOnFXInterface1 0;
                                            //      [P] "The bolt on interface has to have some sort of rider known to
                                            //      it"; [U] func_002ECCB8 reads its AI index and identity
    int* mpCharId;                          // 0x08 (our name) [R] cBoltOnFXInterface_cBoltOnFXInterface1 stores a pointer to the front-end
                                            //      character id, cBoltOnFXInterface_cBoltOnFXInterface 0; [U] func_002ECCB8 uses it without a rider
    cAttachedEmitterFX* mAttachedEmitterFX; // 0x0C [P] "mAttachedEmitterFX!=NULL"; [R] both ctors store their third
                                            //      argument (cRider::mAttachedEmitterFX / cFERider's emitter owner);
                                            //      [U] passed to func_002F2130 / func_002F2218 (create / release)
    tBoltOnFXEntry mEntries[15];            // 0x10 (our name) [R] ctors construct 15 (func_002EC9D0) and free them
                                            //      (func_002ECAF0); [U] func_002ECBC0 / func_002ECC28 / func_002ECCB8 /
                                            //      cBoltOnFXInterface__cBoltOnFXInterface walk 15; [P] "Ran out of bolt on emitters"
    void** mVtbl;                           // 0xC4 [R] cBoltOnFXInterface_vtbl (_vt$18cBoltOnFXInterface), stored by the ctors / dtor

    // Methods bound to their retail symbols by asm labels.
    void* cBoltOnFXInterface1(int charIdPtr, int emitterOwner) __asm__("cBoltOnFXInterface_cBoltOnFXInterface1");
};

CHECK_OFFSET(tBoltOnFXEntry, mEmitter, 0x0);
CHECK_OFFSET(tBoltOnFXEntry, mItem, 0x4);
CHECK_OFFSET(tBoltOnFXEntry, mKind, 0x8);
CHECK_SIZE(tBoltOnFXEntry, 0xC);

CHECK_OFFSET(cBoltOnFXInterface, mDirty, 0x00);
CHECK_OFFSET(cBoltOnFXInterface, mpRider, 0x04);
CHECK_OFFSET(cBoltOnFXInterface, mpCharId, 0x08);
CHECK_OFFSET(cBoltOnFXInterface, mAttachedEmitterFX, 0x0C);
CHECK_OFFSET(cBoltOnFXInterface, mEntries, 0x10);
CHECK_OFFSET(cBoltOnFXInterface, mVtbl, 0xC4);
CHECK_SIZE(cBoltOnFXInterface, 0xC8);

#endif /* VISUALFX_BOLTONFX_H */
