#ifndef VISUALFX_ATTACHEDEMITTERFX_H
#define VISUALFX_ATTACHEDEMITTERFX_H

// cAttachedEmitterFX: a rider's attached particle emitters (the bolt-on / snow trail sprays that follow the rider's
// bones). EA's source: d:/ssx/code/source/visualfx/attachedemitterfx.cpp (preview assert path). The class name: the
// preview's linkonce ShouldDisableSnowTrailSpray__18cAttachedEmitterFX, read/writeToReplayFrame__18cAttachedEmitterFX
// and _vt$18cAttachedEmitterFX; retail's allocation tag "AttachedEmitterFX". No On Tour class.
//
// Size 0xA8: [R] the cRider ctor does cAttachedEmitterFX_cAttachedEmitterFX(cMemMan_alloc(168, "AttachedEmitterFX"), self) and stores the
// result in cRider::mAttachedEmitterFX; the front-end rider builds one with cAttachedEmitterFX_cAttachedEmitterFX1 (src/seg/seg_1F1548.cpp).
// No base class with data; the vptr sits at the end (0xA4): vtable cAttachedEmitterFX_vtbl (slot 1 the dtor
// cAttachedEmitterFX__cAttachedEmitterFX). Methods: cAttachedEmitterFX_cAttachedEmitterFX / cAttachedEmitterFX_cAttachedEmitterFX1 (ctors), cAttachedEmitterFX_init (shared set-up),
// cAttachedEmitterFX_clearDisableSnowTrailSpray, cAttachedEmitterFX__cAttachedEmitterFX, cAttachedEmitterFX_ShouldDisableSnowTrailSpray.
// A FLAT layout, like main/game.h. The data members' names are ours (from their uses; no On Tour class).

#include "layout.h"

struct cRider;
struct cFERider;
struct cAnimModel;
struct cBaseClass_DynamicEmitter;    // render/particle.h

struct cAttachedEmitterFX {
    int mBonesLookedUp;             // 0x000 [U] cAttachedEmitterFX_lookUpBones fills mBoneIndices and sets it, cAttachedEmitterFX_setModel
                                    //       clears it with a new model, cAttachedEmitterFX_update tests it (name ours)
    int mBoneIndices[26];           // 0x004 [U] bone indices into mModel's matrices, one per attach point
                                    //       (cAttachedEmitterFX_lookUpBones; cAttachedEmitterFX_update reads them) (name ours)
    cBaseClass_DynamicEmitter* mEmitters[10]; // 0x06C [R] cAttachedEmitterFX_init clears the 10 emitter slots; the dtor
                                    //       deletes each through its virtual destructor; [U] cAttachedEmitterFX_createEmitter
                                    //       fills a free one with a cAttachedDynamicEmitter (a
                                    //       cBaseClass_DynamicEmitter subclass, 0x210 bytes) (name ours)
    cAnimModel* mModel;             // 0x094 [R] both ctors store 0; [U] cAttachedEmitterFX_setModel stores the cAnimModel
                                    //       the emitters follow (name ours)
    cRider* mRider;                 // 0x098 [R] cAttachedEmitterFX_cAttachedEmitterFX stores the owning cRider; cAttachedEmitterFX_cAttachedEmitterFX1 stores
                                    //       0 (name ours)
    cFERider* mFERider;             // 0x09C [R] cAttachedEmitterFX_cAttachedEmitterFX1 stores the front-end rider; cAttachedEmitterFX_cAttachedEmitterFX
                                    //       stores 0 (name ours)
    int mDisableSnowTrailSpray;     // 0x0A0 [U] cAttachedEmitterFX_ShouldDisableSnowTrailSpray returns it;
                                    //       cAttachedEmitterFX_clearDisableSnowTrailSpray clears it; cAttachedEmitterFX_update sets it while an emitter
                                    //       sits on attach point 25 (name ours)
    void** mVtbl;                   // 0x0A4 [R] cAttachedEmitterFX_vtbl (both ctors and the dtor store it)

    // Methods bound to their retail symbols by asm labels.
    int ShouldDisableSnowTrailSpray() __asm__("cAttachedEmitterFX_ShouldDisableSnowTrailSpray");
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
