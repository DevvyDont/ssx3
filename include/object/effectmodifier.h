#ifndef OBJECT_EFFECTMODIFIER_H
#define OBJECT_EFFECTMODIFIER_H

// The effect modifiers a cMoveNode chains through its tModifierBlock (EA source d:/ssx/code/source/object/
// effectmodifier.cpp, header ../source/object/effectmodifier.h: preview asserts; retail unit object/effectlink,
// 0x345798-0x3460A0, plus the one-line virtuals in object/railmodifier 0x360D00-0x360D28). Chain:
//   cEffectLink (object/effectlink.h: Next, Prev, vptr) -> cEffectModifier (+ Instance; 0x10, On Tour 0x10; ctors
//   cEffectModifier_cEffectModifier(inst) / cEffectModifier_cEffectModifier1(replay), dtor cEffectModifier__cEffectModifier, vtable cEffectModifier_vtbl;
//   live count cEffectModifier::EffectCount = cEffectModifier_EffectCount)
//     -> cParticleModifier (0x1F0: cMemMan_alloc(0x1F0, "ParticleModifier") in cMoveNode_addParticle; ctors
//        cParticleModifier_cParticleModifier(pdata, inst) / cParticleModifier_cParticleModifier1(replay); vtable cParticleModifier_vtbl; ParticleCount cParticleModifier_ParticleCount)
//     -> cDynamicParticleModifier (0x270: cMemMan_alloc(0x270, "DynamicParticleModifier") in
//        cMoveNode_addDynamicParticle; ctors cDynamicParticleModifier_cDynamicParticleModifier1(pdata, inst, mesh, offset) / cDynamicParticleModifier_cDynamicParticleModifier(replay); vtable
//        cDynamicParticleModifier_vtbl; DynamicParticleCount cDynamicParticleModifier_DynamicParticleCount). Its second base cDynamicParticleModifierState
//        (On Tour, 0x50) occupies 0x10-0x60: the replay record writes/reads it as one 0x50 block at +0x10.
//     -> cHaloModifier (0x40: cMemMan_alloc(0x40, "HaloModifier") in cMoveNode_addHalo; ctors
//        cHaloModifier_cHaloModifier(hdata, inst) / cHaloModifier_cHaloModifier1(replay); vtable cHaloModifier_vtbl;
//        HaloCount cHaloModifier_HaloCount). Its second base cHaloModifierState (On Tour, 0x30) occupies 0x10-0x40: the replay record
//        writes/reads it as one 0x30 block at +0x10.
// Vtables (7 slots): 1 dtor, 2 getType, 3 fadeEffect, 4 writeToReplayFrame, 5 update, 6 render, 7 signal.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet; cEffectLink and
// cEffectModifier are written out inline (keep them in step with object/effectlink.h). Evidence tags: [R] ctors/dtors/
// allocation sizes; [P] tools/layout_xlate.py (preview == retail at 0x8, 0x14, 0x18, 0x1E0, 0x1E4, 0x260); [U] matched
// retail use; [O] SSX On Tour's DWARF (same members, names and offsets; its bools are bytes, retail's words).

#include "layout.h"
#include "object/effectlink.h"
#include "bx/math3d.h"
#include "render/particle.h"

struct cEffectLink;
struct tInstanceRecord;
struct tParticleData;

// cEffectModifier: an effect attached to a placed instance (On Tour class cEffectModifier, 0x10).
struct cEffectModifier : public cEffectLink {
    // cEffectLink (object/effectlink.h, 0xC): the vptr at 0x08:
    //   [R] each ctor/dtor stores its vtable (cEffectLink_vtbl, cEffectModifier_vtbl, ...)
    // cEffectModifier's notes on cEffectLink's fields:
    //   Next: [R] ctors zero it; [U] cEffectLink_add, unlink (cEffectLink_unlink)
    //   Prev: [R] ditto
    // --- cEffectModifier
    // cEffectModifier's own slots ([R] cEffectModifier_vtbl; types [O] On Tour's class cEffectModifier):
    virtual void update();                                      // 5
    virtual void render();                                      // 6
    virtual void signal(int signal, float arg);                 // 7
    tInstanceRecord* Instance;      // 0x00C [O] cEffectModifier::Instance; [R] ctor cEffectModifier_cEffectModifier stores its inst
                                    //       argument, the replay ctor cEffectModifier_cEffectModifier1 reads it from the stream; [U]
                                    //       cEffectModifier_writeToReplayFrame, cParticleModifier ctor (root
                                    //       transform), updatePosition

    // Methods bound to their retail symbols by asm labels.
    void* cEffectModifier1(cBxReplayFrame* obj) __asm__("cEffectModifier_cEffectModifier1");
    virtual void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cEffectModifier_writeToReplayFrame");
};

CHECK_OFFSET(cEffectModifier, Instance, 0x0C);
CHECK_SIZE(cEffectModifier, 0x10);

// cParticleModifier: a one-shot or looping static emitter at the instance's root transform.
struct cParticleModifier : public cEffectModifier {
    // cEffectModifier (object/effectmodifier.h, 0x10): the vptr at 0x08:
    //   [R] each ctor/dtor stores its vtable (cEffectLink_vtbl, cEffectModifier_vtbl, ...)
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

    // Methods bound to their retail symbols by asm labels.
    void* cParticleModifier1(cBxReplayFrame* stream) __asm__("cParticleModifier_cParticleModifier1");
    // retail's destructor cParticleModifier__cParticleModifier stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cParticleModifier();
    virtual void update() __asm__("cParticleModifier_update");
    virtual void render() __asm__("cParticleModifier_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cParticleModifier_writeToReplayFrame");
};

CHECK_OFFSET(cParticleModifier, Transform, 0x10);
CHECK_OFFSET(cParticleModifier, Emitter, 0x50);
CHECK_OFFSET(cParticleModifier, ParticleData, 0x1E0);
CHECK_OFFSET(cParticleModifier, Complete, 0x1E4);
CHECK_OFFSET(cParticleModifier, unk_1E8, 0x1E8);
CHECK_SIZE(cParticleModifier, 0x1F0);

// cDynamicParticleModifier: a dynamic emitter that follows one mesh of the instance.
struct t4Vector;

struct cDynamicParticleModifier : public cEffectModifier {
    // cEffectModifier (object/effectmodifier.h, 0x10): the vptr at 0x08:
    //   [R] each ctor/dtor stores its vtable (cEffectLink_vtbl, cEffectModifier_vtbl, ...)
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

    // Methods bound to their retail symbols by asm labels.
    cDynamicParticleModifier* cDynamicParticleModifier1(tParticleData* particleDesc, int owner, int bone, t4Vector* pos)
        __asm__("cDynamicParticleModifier_cDynamicParticleModifier1");
    void updatePosition() __asm__("cDynamicParticleModifier_updatePosition");
    // retail's destructor cDynamicParticleModifier__cDynamicParticleModifier stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cDynamicParticleModifier();
    virtual void update() __asm__("cDynamicParticleModifier_update");
    virtual void fadeEffect() __asm__("cDynamicParticleModifier_fadeEffect__FPv");
    virtual void render() __asm__("cDynamicParticleModifier_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cDynamicParticleModifier_writeToReplayFrame");
};

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


// World-script data that adds a halo modifier (On Tour tAddHaloData, 0x28; same offsets). [O] On Tour's
// cHaloModifier(const tAddHaloData&, tInstanceRecord*); [U] cHaloModifier_cHaloModifier (view sEffSrc_6120) copies
// Type, Mesh, Radius and Speed into the members of the same names and (ColourR, ColourG, ColourB, ColourA) into Color.
struct tAddHaloData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    int Type;                       // 0x04 [O]; [U] -> Type
    int Mesh;                       // 0x08 [O]; [U] -> Mesh
    float ColourA;                  // 0x0C [O]; [U] -> Color.w
    float ColourR;                  // 0x10 [O]; [U] -> Color.x
    float ColourG;                  // 0x14 [O]; [U] -> Color.y
    float ColourB;                  // 0x18 [O]; [U] -> Color.z
    float Radius;                   // 0x1C [O]; [U] -> Radius
    int Speed;                      // 0x20 [O]; [U] -> Speed
    char unk_024[0x4];              // 0x24 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddHaloData, unk_000, 0x00);
CHECK_OFFSET(tAddHaloData, Type, 0x04);
CHECK_OFFSET(tAddHaloData, Mesh, 0x08);
CHECK_OFFSET(tAddHaloData, ColourA, 0x0C);
CHECK_OFFSET(tAddHaloData, ColourR, 0x10);
CHECK_OFFSET(tAddHaloData, ColourG, 0x14);
CHECK_OFFSET(tAddHaloData, ColourB, 0x18);
CHECK_OFFSET(tAddHaloData, Radius, 0x1C);
CHECK_OFFSET(tAddHaloData, Speed, 0x20);
CHECK_OFFSET(tAddHaloData, unk_024, 0x24);
CHECK_SIZE(tAddHaloData, 0x28);

// cHaloModifier: a light halo drawn at one mesh of the instance, spinning by Speed degrees per update
// (cObjectInterface_renderSimpleLightHalo(P, Color, Type, Radius, Angle) with P = the mesh matrix's translation).
// Size 0x40: [R] cMoveNode_addHalo allocates cMemMan_alloc(0x40, "HaloModifier"); the replay record is the 0x30-byte
// cHaloModifierState at +0x10 (write/readFromReplayFrame), ending at 0x40; [O] On Tour's cHaloModifier is 0x40.
// Bases: cEffectModifier (0x00-0x10, above), then cHaloModifierState (On Tour class, 0x30: Color, Radius, Type, Mesh,
// Speed, Angle) at 0x10. The ctor copies the fields from a tAddHaloData (On Tour 0x28: InstId, Type, Mesh, ColourA,
// ColourR, ColourG, ColourB, Radius, Speed), reordering A,R,G,B into Color = {R, G, B, A}.
// Tags as above, plus [P] tools/layout_xlate.py (preview == retail at 0x24, 0x28, 0x2C, 0x30: render / update).
struct cBxReplayFrame;

struct cHaloModifier : public cEffectModifier {
    // cEffectModifier (object/effectmodifier.h, 0x10): the vptr at 0x08:
    //   [R] cHaloModifier_vtbl (0x00491220) in both ctors and the dtor
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

    // Methods bound to their retail symbols by asm labels.
    void* cHaloModifier1(cBxReplayFrame* stream) __asm__("cHaloModifier_cHaloModifier1");
    // retail's destructor cHaloModifier__cHaloModifier stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cHaloModifier();
    virtual void update() __asm__("cHaloModifier_update");
    virtual void render() __asm__("cHaloModifier_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cHaloModifier_writeToReplayFrame");
};

CHECK_OFFSET(cHaloModifier, Color, 0x010);
CHECK_OFFSET(cHaloModifier, Radius, 0x020);
CHECK_OFFSET(cHaloModifier, Type, 0x024);
CHECK_OFFSET(cHaloModifier, Mesh, 0x028);
CHECK_OFFSET(cHaloModifier, Speed, 0x02C);
CHECK_OFFSET(cHaloModifier, Angle, 0x030);
CHECK_OFFSET(cHaloModifier, unk_034, 0x034);
CHECK_SIZE(cHaloModifier, 0x40);

#endif /* OBJECT_EFFECTMODIFIER_H */
