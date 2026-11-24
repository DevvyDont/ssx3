#ifndef VISUALFX_ATTACHEDEMITTERFX_H
#define VISUALFX_ATTACHEDEMITTERFX_H

// cAttachedEmitterFX: a rider's attached particle emitters (the bolt-on / snow trail sprays that follow the rider's
// bones). EA's source: d:/ssx/code/source/visualfx/attachedemitterfx.cpp (preview assert path). The class name: the
// preview's linkonce ShouldDisableSnowTrailSpray__18cAttachedEmitterFX, read/writeToReplayFrame__18cAttachedEmitterFX
// and _vt$18cAttachedEmitterFX; retail's allocation tag "AttachedEmitterFX". No On Tour class.
//
// Size 0xA8: [R] the cRider ctor does func_002F1850(cMemMan_alloc(168, "AttachedEmitterFX"), self) and stores the
// result in cRider::mAttachedEmitterFX; the front-end rider builds one with func_002F1890 (src/seg/seg_1F1548.cpp).
// No base class with data; the vptr sits at the end (0xA4): vtable cAttachedEmitterFX_vtbl (slot 1 the dtor
// cAttachedEmitterFX__cAttachedEmitterFX). Methods: func_002F1850 / func_002F1890 (ctors), func_002F18D8 (shared set-up),
// func_002F1A00, cAttachedEmitterFX__cAttachedEmitterFX, cAttachedEmitterFX_ShouldDisableSnowTrailSpray.
// A FLAT layout, like main/game.h. The data members' names are ours (from their uses; no On Tour class).

#include "layout.h"

struct cRider;
struct cFERider;
struct cAnimModel;
struct cBaseClass_DynamicEmitter;    // render/particle.h

struct cAttachedEmitterFX {
    int mBonesLookedUp;             // 0x000 [U] func_002F22C8 fills mBoneIndices and sets it, func_002F19F0
                                    //       clears it with a new model, func_002F1A08 tests it (name ours)
    int mBoneIndices[26];           // 0x004 [U] bone indices into mModel's matrices, one per attach point
                                    //       (func_002F22C8; func_002F1A08 reads them) (name ours)
    cBaseClass_DynamicEmitter* mEmitters[10]; // 0x06C [R] func_002F18D8 clears the 10 emitter slots; the dtor
                                    //       deletes each through its virtual destructor; [U] func_002F2130
                                    //       fills a free one with a cAttachedDynamicEmitter (a
                                    //       cBaseClass_DynamicEmitter subclass, 0x210 bytes) (name ours)
    cAnimModel* mModel;             // 0x094 [R] both ctors store 0; [U] func_002F19F0 stores the cAnimModel
                                    //       the emitters follow (name ours)
    cRider* mRider;                 // 0x098 [R] func_002F1850 stores the owning cRider; func_002F1890 stores
                                    //       0 (name ours)
    cFERider* mFERider;             // 0x09C [R] func_002F1890 stores the front-end rider; func_002F1850
                                    //       stores 0 (name ours)
    int mDisableSnowTrailSpray;     // 0x0A0 [U] cAttachedEmitterFX_ShouldDisableSnowTrailSpray returns it;
                                    //       func_002F1A00 clears it; func_002F1A08 sets it while an emitter
                                    //       sits on attach point 25 (name ours)
    void** mVtbl;                   // 0x0A4 [R] cAttachedEmitterFX_vtbl (both ctors and the dtor store it)
};

CHECK_OFFSET(cAttachedEmitterFX, mBonesLookedUp, 0x000);
CHECK_OFFSET(cAttachedEmitterFX, mBoneIndices, 0x004);
CHECK_OFFSET(cAttachedEmitterFX, mEmitters, 0x06C);
CHECK_OFFSET(cAttachedEmitterFX, mModel, 0x094);
CHECK_OFFSET(cAttachedEmitterFX, mRider, 0x098);
CHECK_OFFSET(cAttachedEmitterFX, mFERider, 0x09C);
CHECK_OFFSET(cAttachedEmitterFX, mDisableSnowTrailSpray, 0x0A0);
CHECK_OFFSET(cAttachedEmitterFX, mVtbl, 0x0A4);
CHECK_SIZE(cAttachedEmitterFX, 0xA8);

#endif /* VISUALFX_ATTACHEDEMITTERFX_H */
