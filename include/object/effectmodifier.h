#ifndef OBJECT_EFFECTMODIFIER_H
#define OBJECT_EFFECTMODIFIER_H

// The effect modifiers a cMoveNode chains through its tModifierBlock (EA source d:/ssx/code/source/object/
// effectmodifier.cpp, header ../source/object/effectmodifier.h: preview asserts; retail unit object/effectlink,
// 0x345798-0x3460A0, plus the one-line virtuals in object/railmodifier 0x360D00-0x360D28). Chain:
//   cEffectLink (object/effectlink.h: Next, Prev, vptr) -> cEffectModifier (+ Instance; 0x10, On Tour 0x10; ctors
//   cEffectModifier_cEffectModifier(inst) / cEffectModifier_cEffectModifier1(replay), dtor cEffectModifier__cEffectModifier, vtable cEffectModifier_vtbl;
//   live count cEffectModifier::EffectCount = D_004A3FF4)
//     -> cParticleModifier (0x1F0: cMemMan_alloc(0x1F0, "ParticleModifier") in cMoveNode_addParticle; ctors
//        cParticleModifier_cParticleModifier(pdata, inst) / cParticleModifier_cParticleModifier1(replay); vtable cParticleModifier_vtbl; ParticleCount D_004A3FF8)
//     -> cDynamicParticleModifier (0x270: cMemMan_alloc(0x270, "DynamicParticleModifier") in
//        cMoveNode_addDynamicParticle; ctors cDynamicParticleModifier_cDynamicParticleModifier1(pdata, inst, mesh, offset) / cDynamicParticleModifier_cDynamicParticleModifier(replay); vtable
//        cDynamicParticleModifier_vtbl; DynamicParticleCount D_004A3FFC). Its second base cDynamicParticleModifierState
//        (On Tour, 0x50) occupies 0x10-0x60: the replay record writes/reads it as one 0x50 block at +0x10.
//     -> cHaloModifier (0x40: cMemMan_alloc(0x40, "HaloModifier") in cMoveNode_addHalo; ctors
//        cHaloModifier_cHaloModifier(hdata, inst) / cHaloModifier_cHaloModifier1(replay); vtable cHaloModifier_vtbl;
//        HaloCount D_004A4000). Its second base cHaloModifierState (On Tour, 0x30) occupies 0x10-0x40: the replay record
//        writes/reads it as one 0x30 block at +0x10.
// Vtables (7 slots): 1 dtor, 2 getType, 3 fadeEffect, 4 writeToReplayFrame, 5 update, 6 render, 7 signal.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet; cEffectLink and
// cEffectModifier are written out inline (keep them in step with object/effectlink.h). Evidence tags: [R] ctors/dtors/
// allocation sizes; [P] tools/layout_xlate.py (preview == retail at 0x8, 0x14, 0x18, 0x1E0, 0x1E4, 0x260); [U] matched
// retail use; [O] SSX On Tour's DWARF (same members, names and offsets; its bools are bytes, retail's words).

#include "layout.h"
#include "bx/math3d.h"
#include "render/particle.h"

struct cEffectLink;
struct tInstanceRecord;
struct tParticleData;

// cEffectModifier: an effect attached to a placed instance (On Tour class cEffectModifier, 0x10).
struct cEffectModifier {
    // --- cEffectLink (object/effectlink.h)
    cEffectLink* Next;              // 0x000 [O] cEffectLink::Next; [R] ctors zero it; [U] cEffectLink_add, unlink
                                    //       (cEffectLink_unlink)
    cEffectLink* Prev;              // 0x004 [O] cEffectLink::Prev; [R] ditto
    void** mVtbl;                   // 0x008 [R] each ctor/dtor stores its vtable (cEffectLink_vtbl,
                                    //       cEffectModifier_vtbl, ...)
    // --- cEffectModifier
    tInstanceRecord* Instance;      // 0x00C [O] cEffectModifier::Instance; [R] ctor cEffectModifier_cEffectModifier stores its inst
                                    //       argument, the replay ctor cEffectModifier_cEffectModifier1 reads it from the stream; [U]
                                    //       cEffectModifier_writeToReplayFrame, cParticleModifier ctor (root
                                    //       transform), updatePosition
};

CHECK_OFFSET(cEffectModifier, Next, 0x00);
CHECK_OFFSET(cEffectModifier, Prev, 0x04);
CHECK_OFFSET(cEffectModifier, mVtbl, 0x08);
CHECK_OFFSET(cEffectModifier, Instance, 0x0C);
CHECK_SIZE(cEffectModifier, 0x10);

// cParticleModifier: a one-shot or looping static emitter at the instance's root transform.
struct cParticleModifier {
    // --- cEffectLink (object/effectlink.h)
    cEffectLink* Next;              // 0x000 [O] cEffectLink::Next; [R] ctors zero it; [U] cEffectLink_add, unlink
                                    //       (cEffectLink_unlink)
    cEffectLink* Prev;              // 0x004 [O] cEffectLink::Prev; [R] ditto
    void** mVtbl;                   // 0x008 [R] each ctor/dtor stores its vtable (cEffectLink_vtbl,
                                    //       cEffectModifier_vtbl, ...)
    // --- cEffectModifier
    tInstanceRecord* Instance;      // 0x00C [O] cEffectModifier::Instance; [R] ctor cEffectModifier_cEffectModifier stores its inst
                                    //       argument, the replay ctor cEffectModifier_cEffectModifier1 reads it from the stream; [U]
                                    //       cEffectModifier_writeToReplayFrame, cParticleModifier ctor (root
                                    //       transform), updatePosition
    // --- cParticleModifier
    t4Matrix Transform;             // 0x010 [O]; [R] ctor copies the instance's root transform (VU0 matrix copy), the
                                    //       replay ctor reads 0x40 bytes; [U] setupEmitter's transform,
                                    //       writeToReplayFrame
    cStaticEmitter Emitter;         // 0x050 [O]; [R] ctor cStaticEmitter_cStaticEmitter(this + 0x50), dtor cStaticEmitter__cStaticEmitter(this + 0x50,
                                    //       2); [U] update/render/writeToReplayFrame
    tParticleData* ParticleData;    // 0x1E0 [O]; [R] ctor stores pdata (owned), the replay ctor allocates 0xD8; dtor
                                    //       deletes it; [U] writeToReplayFrame writes 0xD8 bytes
    int Complete;                   // 0x1E4 [O] bool (retail 4 bytes); [R] ctor 0, replay ctor reads it first; [U]
                                    //       update sets 1 once Emitter.Duration runs out, render returns while set
    char unk_1E8[0x8];              // 0x1E8 padding to the 16-byte size
};

CHECK_OFFSET(cParticleModifier, Next, 0x00);
CHECK_OFFSET(cParticleModifier, Prev, 0x04);
CHECK_OFFSET(cParticleModifier, mVtbl, 0x08);
CHECK_OFFSET(cParticleModifier, Instance, 0x0C);
CHECK_OFFSET(cParticleModifier, Transform, 0x10);
CHECK_OFFSET(cParticleModifier, Emitter, 0x50);
CHECK_OFFSET(cParticleModifier, ParticleData, 0x1E0);
CHECK_OFFSET(cParticleModifier, Complete, 0x1E4);
CHECK_OFFSET(cParticleModifier, unk_1E8, 0x1E8);
CHECK_SIZE(cParticleModifier, 0x1F0);

// cDynamicParticleModifier: a dynamic emitter that follows one mesh of the instance.
struct cDynamicParticleModifier {
    // --- cEffectLink (object/effectlink.h)
    cEffectLink* Next;              // 0x000 [O] cEffectLink::Next; [R] ctors zero it; [U] cEffectLink_add, unlink
                                    //       (cEffectLink_unlink)
    cEffectLink* Prev;              // 0x004 [O] cEffectLink::Prev; [R] ditto
    void** mVtbl;                   // 0x008 [R] each ctor/dtor stores its vtable (cEffectLink_vtbl,
                                    //       cEffectModifier_vtbl, ...)
    // --- cEffectModifier
    tInstanceRecord* Instance;      // 0x00C [O] cEffectModifier::Instance; [R] ctor cEffectModifier_cEffectModifier stores its inst
                                    //       argument, the replay ctor cEffectModifier_cEffectModifier1 reads it from the stream; [U]
                                    //       cEffectModifier_writeToReplayFrame, cParticleModifier ctor (root
                                    //       transform), updatePosition
    // --- cDynamicParticleModifierState (On Tour second base, 0x50)
    int Mesh;                       // 0x010 [O] cDynamicParticleModifierState::Mesh; [R] ctor stores the mesh index;
                                    //       [U] updatePosition passes it to cInstanceNode_buildMeshMat (the mesh's matrix)
    int Fade;                       // 0x014 [O] (state); [R] ctor 0; [U] fadeEffect sets 1, update then stops emitting
    int Complete;                   // 0x018 [O] (state); [R] ctor 0, replay ctor tests it; [U] update sets 1, render
                                    //       and writeToReplayFrame test it
    char unk_01C[0x4];              // 0x01C padding (Offset is 16-aligned)
    t4Vector Offset;                // 0x020 [O] (state); [R] ctor copies the offset argument; [U] updatePosition
    t4Vector Pos;                   // 0x030 [O] (state); [U] updatePosition: mesh matrix * Offset; update passes it to
                                    //       updateBASE
    t4Vector Vel;                   // 0x040 [O] (state); [U] updatePosition: mesh matrix * InitVel; update
    t4Vector InitVel;               // 0x050 [O] (state); [R] ctor takes the particle data's +0x48 vector (w = 0) and
                                    //       zeroes it in the data
    // --- cDynamicParticleModifier
    cDynamicEmitter Emitter;        // 0x060 [O]; [R] ctor cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(this + 0x60) + cDynamicEmitter_vtbl at +0x258;
                                    //       dtor cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(this + 0x60, 2)
    tParticleData* ParticleData;    // 0x260 [O]; [R] ctor stores pdata (owned), the replay ctor allocates 0xD8; dtor
                                    //       deletes it; [U] setupEmitter, writeToReplayFrame
    char unk_264[0xC];              // 0x264 padding to the 16-byte size
};

CHECK_OFFSET(cDynamicParticleModifier, Next, 0x00);
CHECK_OFFSET(cDynamicParticleModifier, Prev, 0x04);
CHECK_OFFSET(cDynamicParticleModifier, mVtbl, 0x08);
CHECK_OFFSET(cDynamicParticleModifier, Instance, 0x0C);
CHECK_OFFSET(cDynamicParticleModifier, Mesh, 0x10);
CHECK_OFFSET(cDynamicParticleModifier, Fade, 0x14);
CHECK_OFFSET(cDynamicParticleModifier, Complete, 0x18);
CHECK_OFFSET(cDynamicParticleModifier, unk_01C, 0x1C);
CHECK_OFFSET(cDynamicParticleModifier, Offset, 0x20);
CHECK_OFFSET(cDynamicParticleModifier, Pos, 0x30);
CHECK_OFFSET(cDynamicParticleModifier, Vel, 0x40);
CHECK_OFFSET(cDynamicParticleModifier, InitVel, 0x50);
CHECK_OFFSET(cDynamicParticleModifier, Emitter, 0x60);
CHECK_OFFSET(cDynamicParticleModifier, ParticleData, 0x260);
CHECK_OFFSET(cDynamicParticleModifier, unk_264, 0x264);
CHECK_SIZE(cDynamicParticleModifier, 0x270);


// cHaloModifier: a light halo drawn at one mesh of the instance, spinning by Speed degrees per update
// (cObjectInterface_renderSimpleLightHalo(P, Color, Type, Radius, Angle) with P = the mesh matrix's translation).
// Size 0x40: [R] cMoveNode_addHalo allocates cMemMan_alloc(0x40, "HaloModifier"); the replay record is the 0x30-byte
// cHaloModifierState at +0x10 (write/readFromReplayFrame), ending at 0x40; [O] On Tour's cHaloModifier is 0x40.
// Bases: cEffectModifier (0x00-0x10, above), then cHaloModifierState (On Tour class, 0x30: Color, Radius, Type, Mesh,
// Speed, Angle) at 0x10. The ctor copies the fields from a tAddHaloData (On Tour 0x28: InstId, Type, Mesh, ColourA,
// ColourR, ColourG, ColourB, Radius, Speed), reordering A,R,G,B into Color = {R, G, B, A}.
// Tags as above, plus [P] tools/layout_xlate.py (preview == retail at 0x24, 0x28, 0x2C, 0x30: render / update).
struct cHaloModifier {
    // --- cEffectLink (object/effectlink.h)
    cEffectLink* Next;              // 0x000 [O] cEffectLink::Next; [R] the cEffectModifier ctors zero it; [U]
                                    //       cEffectLink_add, unlink (cEffectLink_unlink)
    cEffectLink* Prev;              // 0x004 [O] cEffectLink::Prev; [R] ditto
    void** mVtbl;                   // 0x008 [R] cHaloModifier_vtbl (0x00491220) in both ctors and the dtor
    // --- cEffectModifier
    tInstanceRecord* Instance;      // 0x00C [O] cEffectModifier::Instance; [R] cEffectModifier_cEffectModifier(inst) /
                                    //       the replay ctor; [U] render passes it to cInstanceNode_buildMeshMat
    // --- cHaloModifierState (On Tour second base, 0x30)
    t4Vector Color;                 // 0x010 [O] (state); [R] ctor = {ColourR, ColourG, ColourB, ColourA} of the
                                    //       tAddHaloData (one sq); [U] render passes &Color as the halo colour
    float Radius;                   // 0x020 [O] (state); [R] ctor = hdata.Radius (swc1); [U] render passes it (lwc1)
    int Type;                       // 0x024 [O] (state); [R] ctor = hdata.Type; [P]; [U] render passes it as the halo
                                    //       type
    int Mesh;                       // 0x028 [O] (state); [R] ctor = hdata.Mesh; [P]; [U] render:
                                    //       cInstanceNode_buildMeshMat(Instance, Mesh, &m)
    int Speed;                      // 0x02C [O] (state); [R] ctor = hdata.Speed; [P]; [U] update adds (float)Speed to
                                    //       Angle
    float Angle;                    // 0x030 [O] (state); [R] ctor 0 (sw $0); [P]; [U] update advances it and resets it
                                    //       to 0 past +-360; render passes it
    char unk_034[0xC];              // 0x034 padding (16-byte aligned: Color is a t4Vector)
};

CHECK_OFFSET(cHaloModifier, Next, 0x000);
CHECK_OFFSET(cHaloModifier, Prev, 0x004);
CHECK_OFFSET(cHaloModifier, mVtbl, 0x008);
CHECK_OFFSET(cHaloModifier, Instance, 0x00C);
CHECK_OFFSET(cHaloModifier, Color, 0x010);
CHECK_OFFSET(cHaloModifier, Radius, 0x020);
CHECK_OFFSET(cHaloModifier, Type, 0x024);
CHECK_OFFSET(cHaloModifier, Mesh, 0x028);
CHECK_OFFSET(cHaloModifier, Speed, 0x02C);
CHECK_OFFSET(cHaloModifier, Angle, 0x030);
CHECK_OFFSET(cHaloModifier, unk_034, 0x034);
CHECK_SIZE(cHaloModifier, 0x40);

#endif /* OBJECT_EFFECTMODIFIER_H */
