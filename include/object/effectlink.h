#ifndef OBJECT_EFFECTLINK_H
#define OBJECT_EFFECTLINK_H

// cEffectLink: the intrusive list node that chains a cMoveNode's effect modifiers (base of cEffectModifier ->
// cParticleModifier, cDynamicParticleModifier, cHaloModifier; tModifierBlock embeds one by value at +0x10 as the head).
// Size 0xC: On Tour 0xC; tModifierBlock's next member (Rail) is at +0x1C; derived ctors store their vtable at +0x8.
// Methods: cEffectLink_add (0x345700) and cEffectLink_append (src/object/effectlink.cpp), destructor cEffectLink__cEffectLink
// (0x360C98), cEffectLink_getType (slot 1), fadeEffect, writeToReplayFrame (src/object/railmodifier.cpp); vtable
// cEffectLink_vtbl (0x00491340, 4 slots). Local definition: src/object/effectlink.cpp `struct cEffectLink {next, prev}`.

#include "layout.h"

struct cBxReplayFrame;

struct cEffectLink;

struct cEffectLink {
    // What getType (slot 2) reports (names ours, from the classes): [R] cEffectLink_getType and
    // cParticleModifier_getType return 1, cDynamicParticleModifier_getType 2, cHaloModifier_getType 3; [U]
    // tModifierBlock_readFromReplayFrame rebuilds each effect by it and remHaloModifier deletes the kEffect_Halo links.
    enum eEffectType { kEffect_Particle = 1, kEffect_DynamicParticle = 2, kEffect_Halo = 3 };
    cEffectLink* Next;              // 0x00 [O]; [U] cEffectLink_add, cEffectLink_append
    cEffectLink* Prev;              // 0x04 [O]; [U] cEffectLink_add
    // 0x08: the vptr. [R] cEffectLink_vtbl (0x00491340): the dtor and tModifierBlock_tModifierBlock (+0x18) store it
    //   Slots:
    virtual ~cEffectLink();                                     // 1  ([R] cEffectLink_vtbl; types [O])
    virtual int getType() __asm__("cEffectLink_getType__FPv");                                      // 2
    virtual void fadeEffect() __asm__("cEffectLink_fadeEffect__FPv");                                  // 3
    virtual void writeToReplayFrame(cBxReplayFrame* data) __asm__("cEffectLink_writeToReplayFrame__FPv");      // 4

    // Methods bound to their retail symbols by asm labels.
    void add(cEffectLink* other) __asm__("cEffectLink_add__FP11cEffectLinkT0");
    void unlink() __asm__("cEffectLink_unlink");
    void append(cEffectLink* other) __asm__("cEffectLink_append");
};

CHECK_OFFSET(cEffectLink, Next, 0x00);
CHECK_OFFSET(cEffectLink, Prev, 0x04);
CHECK_SIZE(cEffectLink, 0xC);

#endif /* OBJECT_EFFECTLINK_H */
