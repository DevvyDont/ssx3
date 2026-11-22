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
// A FLAT layout, like main/game.h. No member names have two signals yet: every data field is unk_.

#include "layout.h"

struct cRider;

struct cAttachedEmitterFX {
    int unk_000;                    // 0x000 [U] the bones-looked-up flag (seg_1F1548 notes)
    int unk_004[26];                // 0x004 [U] 26 bone indices
    void* unk_06C[10];              // 0x06C [R] func_002F18D8 clears the 10 emitter slots; the dtor deletes each through
                                    //       its virtual destructor
    void* unk_094;                  // 0x094 [R] both ctors store 0 (the model)
    cRider* unk_098;                // 0x098 [R] func_002F1850 stores the owning cRider; func_002F1890 stores 0
    void* unk_09C;                  // 0x09C [R] func_002F1890 stores the front-end rider; func_002F1850 stores 0
    int unk_0A0;                    // 0x0A0 [U] cAttachedEmitterFX_ShouldDisableSnowTrailSpray returns it; func_002F1A00
                                    //       clears it (the spread flag)
    void** mVtbl;                   // 0x0A4 [R] cAttachedEmitterFX_vtbl (both ctors and the dtor store it)
};

CHECK_OFFSET(cAttachedEmitterFX, unk_000, 0x000);
CHECK_OFFSET(cAttachedEmitterFX, unk_004, 0x004);
CHECK_OFFSET(cAttachedEmitterFX, unk_06C, 0x06C);
CHECK_OFFSET(cAttachedEmitterFX, unk_094, 0x094);
CHECK_OFFSET(cAttachedEmitterFX, unk_098, 0x098);
CHECK_OFFSET(cAttachedEmitterFX, unk_09C, 0x09C);
CHECK_OFFSET(cAttachedEmitterFX, unk_0A0, 0x0A0);
CHECK_OFFSET(cAttachedEmitterFX, mVtbl, 0x0A4);
CHECK_SIZE(cAttachedEmitterFX, 0xA8);

#endif /* VISUALFX_ATTACHEDEMITTERFX_H */
