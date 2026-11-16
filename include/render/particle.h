#ifndef RENDER_PARTICLE_H
#define RENDER_PARTICLE_H

// EA's particle emitters (EA header ../source/render/particle.h, source d:/ssx/code/source/render/particle.cpp: both
// named by the July 2003 preview's assert strings, e.g. "(unsigned int)id < (unsigned int)m_numSubEmitters" in
// cBaseClass_DynamicEmitter::getSubEmitterPositionVelocity). One header for the five classes, as EA keeps them:
//   tParticleEmitter          the particle system itself (no vtable). Size 0x150: cMemMan_alloc(0x150, "Board Spark
//                             Emitter") +
//                             ctor func_002F7A68 (empty) in cBoardSparkFX's ctor; embedded at +0x10 / +0x20 below.
//   cStaticEmitter            a fixed emitter (cParticleModifier, cMultiParticle). Size 0x190: cMemMan_alloc(0x190,
//                             "Emitter") + ctor cStaticEmitter_cStaticEmitter in cMultiParticle_setupMultiParticle. Vtable cStaticEmitter_vtbl.
//   cBaseClass_DynamicEmitter an emitter that follows a moving source and keeps a ring of sub-emitter positions.
//                             Size 0x200: embedded at cDynamicParticleModifier +0x60 with ParticleData at +0x260, and
//                             cDynamicColourEmitter's own fields start at +0x200. Vtable cBaseClass_DynamicEmitter_vtbl
//                             (7 slots: 1 purge, 2 reset, 3 setupEmitter, 4 dtor, 5 Allocate, 6 writeToReplayFrame,
//                             7 readFromReplayFrame).
//   cDynamicEmitter           : cBaseClass_DynamicEmitter, no members (size 0x200, On Tour too); vtable
//                             cDynamicEmitter_vtbl (own dtor in slot 4); constructed inline by func_00345C90.
//   cDynamicColourEmitter     : cBaseClass_DynamicEmitter + a colour per sub-emitter. Size 0x210: cMemMan_alloc(0x210,
//                             "mEmitter") + ctor cDynamicColourEmitter_cDynamicColourEmitter in cBoardSparkFX's ctor. Vtable
//                             cDynamicColourEmitter_vtbl.
// (cDynamicParticleModifier/cParticleModifier are in object/effectmodifier.h; another 0x210-byte subclass with vtable
// D_004881E8 is built by func_002F1768 in seg/seg_1F1548.)
//
// Methods: tParticleEmitter and cStaticEmitter are in src/render/graphicsman.cpp (0x36CBF8-0x370AF8; EA's particle.cpp
// starts there: the preview pairs cStaticEmitter::render with particle.cpp line 410), the dynamic emitters in
// src/render/graphicsman.cpp (ctor cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter, dtor) and src/render/particle.cpp (0x370C60-0x3717C0); see each
// class's methods.txt. The draw calls are cPS2GraphicsMan virtuals: slot 82 RenderParticleEmitter(const
// tParticleEmitter&) cPS2GraphicsMan_RenderParticleEmitter, slot 83 (cDynamicEmitter*) func_003807A0, slot 84 (cDynamicColourEmitter*)
// func_00380CE0.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet; the base's fields are
// written out inline in each subclass (keep them in step). Evidence tags:
//   [R] retail constructors, destructors and allocation sizes (above);
//   [P] the preview's out-of-line inline accessors (preview_linkonce_t: enabled/setEnabled, numSubEmitters,
//       subEmitterId, getEnabledCount, setSeed of cBaseClass_DynamicEmitter; tools/layout_xlate.py: preview == retail
//       for every observed offset of these classes);
//   [U] how matched retail code uses the field (method names from On Tour by body shape and call order);
//   [O] SSX On Tour's DWARF (same member order and names). On Tour's bools are 1 byte, retail's are words; On Tour has
//       no vptr in cStaticEmitter/cBaseClass_DynamicEmitter, keeps an extra m_useLargeParticles at 0x1F4 (retail: the
//       draw state's texture ids) and t4Vector colours (retail: 4-byte packed RGBA).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"       // tBoundBox
#include "render/ps2graphicsman.h" // sDrawState

struct tParticleData;   // On Tour: 0xD8 bytes (NumParticles, NumBlur, Duration, Damp, Size, Life, ...); retail
                        // copies 0xD8 (cMultiParticle, the modifiers' replay ctors)
struct tPackedRGBA;     // 4 bytes {r, g, b, a} (the preview mangles getSubEmitterColour with it)

// The particle system (On Tour struct tParticleEmitter, 0x150). Its methods take the Damp value as their time scale.
struct tParticleEmitter {
    int NumParticles;               // 0x000 [O]; [U] init sw (num); stepContinous/calcBoundBox read it (lwc1 + cvt.s.w)
    int NumBlur;                    // 0x004 [O]; [U] init sw (numblur), BlurAlpha = 1 / NumBlur
    float EndTime;                  // 0x008 [O]; [U] the running time: setTime (t * Damp), step (+= dt * Damp),
                                    //       stepContinous; the emitters' replay records save it (cStaticEmitter +0x18,
                                    //       cBaseClass_DynamicEmitter +0x28)
    float TimeStep;                 // 0x00C [O]; [U] init: duration * damp / num; setMotion, stepContinous
    float SizeR;                    // 0x010 [O]; [U] setSize stores sizerandomness
    float LifeR;                    // 0x014 [O]; [U] setLife ((max - min) * Damp), setEternal (0), calcBoundBox
    float Size0;                    // 0x018 [O]; [U] setSize (start - 1.5 * sizerandomness)
    float Life0;                    // 0x01C [O]; [U] setLife, setEternal (1e20), calcBoundBox
    float SizeFinalDelta;           // 0x020 [O]; [U] setSize (end - start)
    float Dummy1;                   // 0x024 [O] (no retail access seen)
    float Dummy2;                   // 0x028 [O] (no retail access seen)
    float Dummy3;                   // 0x02C [O] (no retail access seen)
    float BlurAlpha;                // 0x030 [O]; [U] init: 1 / NumBlur
    float BlurStep;                 // 0x034 [O]; [U] init: blurstep * damp
    float SeedX0;                   // 0x038 [O]; [U] randomize; setupEmitter stores seed >= 0 here; stepContinous moves
                                    //       it (func_0036D400); [P] cBaseClass_DynamicEmitter::setSeed (swc1 0x58 =
                                    //       0x20 + 0x38)
    float SeedX1;                   // 0x03C [O]; [U] randomize, stepContinous
    float SeedV0;                   // 0x040 [O]; [U] randomize, stepContinous
    float SeedV1;                   // 0x044 [O]; [U] randomize, stepContinous
    float SeedV2;                   // 0x048 [O]; [U] randomize, stepContinous
    float Damp;                     // 0x04C [O]; [U] init stores its damp argument;
                                    //       setTime/step/setLife/setMotion/setVelocity/setColour use it as the time
                                    //       scale (randomize and stepContinous skip it among the seeds)
    float SeedC0;                   // 0x050 [O]; [U] randomize, stepContinous
    float SeedC1;                   // 0x054 [O]; [U] randomize, stepContinous
    float SeedSize;                 // 0x058 [O]; [U] randomize, stepContinous
    float SeedLife;                 // 0x05C [O]; [U] randomize, stepContinous
    t4Vector Force;                 // 0x060 [O]; [U] init: force / damp / damp (sq); calcBoundBox
    t4Vector V0;                    // 0x070 [O]; [U] both setVelocity overloads, setupEmitter, calcBoundBox
    t4Vector Vr0;                   // 0x080 [O]; [U] setVelocity, setupEmitter, calcBoundBox
    t4Vector Vr1;                   // 0x090 [O]; [U] setVelocity, setupEmitter, calcBoundBox
    t4Vector Vr2;                   // 0x0A0 [O]; [U] setVelocity, setupEmitter, calcBoundBox
    t4Vector X0;                    // 0x0B0 [O]; [U] setPoint/setLine/setPlane, setupEmitter, stepContinous (+= Xi);
                                    //       cStaticEmitter render(bbox, pos) stores the position here (its +0xC0);
                                    //       cMultiParticle ctor reads it
    t4Vector Xr0;                   // 0x0C0 [O]; [U] setPoint/setLine/setPlane, setupEmitter, calcBoundBox
    t4Vector Xr1;                   // 0x0D0 [O]; [U] setPoint/setLine/setPlane, setupEmitter, calcBoundBox
    t4Vector Xi;                    // 0x0E0 [O]; [U] setMotion, stopMotion (zero), stepContinous, calcBoundBox
    t4Vector C0;                    // 0x0F0 [O]; [U] setColour (cstart - 1.5 * (cr0 + cr1))
    t4Vector Cr0;                   // 0x100 [O]; [U] setColour
    t4Vector Cr1;                   // 0x110 [O]; [U] setColour
    t4Vector Ct;                    // 0x120 [O]; [U] setColour ((cend - cstart) / (time * Damp))
    t4Vector UpperUV;               // 0x130 [O]; [U] init zeroes it
    t4Vector LowerUV;               // 0x140 [O]; [U] init stores {1, 1, 0, 0}
};

CHECK_OFFSET(tParticleEmitter, NumParticles, 0x00);
CHECK_OFFSET(tParticleEmitter, NumBlur, 0x04);
CHECK_OFFSET(tParticleEmitter, EndTime, 0x08);
CHECK_OFFSET(tParticleEmitter, TimeStep, 0x0C);
CHECK_OFFSET(tParticleEmitter, SizeR, 0x10);
CHECK_OFFSET(tParticleEmitter, LifeR, 0x14);
CHECK_OFFSET(tParticleEmitter, Size0, 0x18);
CHECK_OFFSET(tParticleEmitter, Life0, 0x1C);
CHECK_OFFSET(tParticleEmitter, SizeFinalDelta, 0x20);
CHECK_OFFSET(tParticleEmitter, Dummy1, 0x24);
CHECK_OFFSET(tParticleEmitter, Dummy2, 0x28);
CHECK_OFFSET(tParticleEmitter, Dummy3, 0x2C);
CHECK_OFFSET(tParticleEmitter, BlurAlpha, 0x30);
CHECK_OFFSET(tParticleEmitter, BlurStep, 0x34);
CHECK_OFFSET(tParticleEmitter, SeedX0, 0x38);
CHECK_OFFSET(tParticleEmitter, SeedX1, 0x3C);
CHECK_OFFSET(tParticleEmitter, SeedV0, 0x40);
CHECK_OFFSET(tParticleEmitter, SeedV1, 0x44);
CHECK_OFFSET(tParticleEmitter, SeedV2, 0x48);
CHECK_OFFSET(tParticleEmitter, Damp, 0x4C);
CHECK_OFFSET(tParticleEmitter, SeedC0, 0x50);
CHECK_OFFSET(tParticleEmitter, SeedC1, 0x54);
CHECK_OFFSET(tParticleEmitter, SeedSize, 0x58);
CHECK_OFFSET(tParticleEmitter, SeedLife, 0x5C);
CHECK_OFFSET(tParticleEmitter, Force, 0x60);
CHECK_OFFSET(tParticleEmitter, V0, 0x70);
CHECK_OFFSET(tParticleEmitter, Vr0, 0x80);
CHECK_OFFSET(tParticleEmitter, Vr1, 0x90);
CHECK_OFFSET(tParticleEmitter, Vr2, 0xA0);
CHECK_OFFSET(tParticleEmitter, X0, 0xB0);
CHECK_OFFSET(tParticleEmitter, Xr0, 0xC0);
CHECK_OFFSET(tParticleEmitter, Xr1, 0xD0);
CHECK_OFFSET(tParticleEmitter, Xi, 0xE0);
CHECK_OFFSET(tParticleEmitter, C0, 0xF0);
CHECK_OFFSET(tParticleEmitter, Cr0, 0x100);
CHECK_OFFSET(tParticleEmitter, Cr1, 0x110);
CHECK_OFFSET(tParticleEmitter, Ct, 0x120);
CHECK_OFFSET(tParticleEmitter, UpperUV, 0x130);
CHECK_OFFSET(tParticleEmitter, LowerUV, 0x140);
CHECK_SIZE(tParticleEmitter, 0x150);

// A fixed emitter (On Tour class cStaticEmitter, 0x190).
struct cStaticEmitter {
    float Duration;                 // 0x000 [O]; [R] setupEmitter (cStaticEmitter_setupEmitter); [U] update counts it down by one app
                                    //       tick (negative = endless, 0 = stopped), setTime func_00370708, replay
                                    //       record; cParticleModifier_update watches it
    int TextureId;                  // 0x004 [O]; [R] setupEmitter (particle data +0xC4); [U] render: the effect texture
                                    //       is TheGraphicsMan->texIds[TextureId + (int)mCurrentFlipTexture]
    int BlendMode;                  // 0x008 [O]; [R] setupEmitter (data +0xC8); [U] render maps it through D_0044B420
                                    //       to the draw layer
    int m_initialized;              // 0x00C [O] bool (retail 4 bytes); [R] ctor cStaticEmitter_cStaticEmitter zeroes it; [U] update
                                    //       sets 1, render returns while 0, replay record
    tParticleEmitter Emitter;       // 0x010 [O]; [R] ctor func_002F7A68(this + 0x10); [U] setupEmitter (init +
                                    //       setupEmitter), update (step / stepContinous), render (cPS2GraphicsMan slot
                                    //       82)
    tBoundBox BoundBox;             // 0x160 [O]; [U] setupEmitter fills it (calcBoundBox), render(bbox, pos) stores it,
                                    //       render culls with it (slot 93), readFromReplayFrame recomputes it
    int mNumFlipTextures;           // 0x180 [O]; [R] setupEmitter (data +0xD0); [U] update wraps the frame at it
    float mCurrentFlipTexture;      // 0x184 [O]; [R] ctor zeroes it (sw); [U] update, render
    float mFlipTextureRate;         // 0x188 [O]; [R] setupEmitter (data +0xD4); [U] update
    void** mVtbl;                   // 0x18C [R] ctor cStaticEmitter_cStaticEmitter and dtor cStaticEmitter__cStaticEmitter store cStaticEmitter_vtbl (1 slot: the
                                    //       dtor); [P] the preview's cStaticEmitter slot-1 pair touches 0x18C (On Tour
                                    //       has no virtuals here)
};

CHECK_OFFSET(cStaticEmitter, Duration, 0x00);
CHECK_OFFSET(cStaticEmitter, TextureId, 0x04);
CHECK_OFFSET(cStaticEmitter, BlendMode, 0x08);
CHECK_OFFSET(cStaticEmitter, m_initialized, 0x0C);
CHECK_OFFSET(cStaticEmitter, Emitter, 0x10);
CHECK_OFFSET(cStaticEmitter, BoundBox, 0x160);
CHECK_OFFSET(cStaticEmitter, mNumFlipTextures, 0x180);
CHECK_OFFSET(cStaticEmitter, mCurrentFlipTexture, 0x184);
CHECK_OFFSET(cStaticEmitter, mFlipTextureRate, 0x188);
CHECK_OFFSET(cStaticEmitter, mVtbl, 0x18C);
CHECK_SIZE(cStaticEmitter, 0x190);

// The moving emitter base (On Tour class cBaseClass_DynamicEmitter, 0x200).
struct cBaseClass_DynamicEmitter {
    float Duration;                 // 0x000 [O]; [U] setupEmitter (cBaseClass_DynamicEmitter_setupEmitter), setTime (cBaseClass_DynamicEmitter_setTime), replay
                                    //       record; cDynamicParticleModifier_update tests it (its +0x60)
    int TextureId;                  // 0x004 [O]; [U] setupEmitter (particle data +0xC4); render: texIds[TextureId +
                                    //       (int)mCurrentFlipTexture]
    int BlendMode;                  // 0x008 [O]; [U] setupEmitter (data +0xC8); render maps it through D_0044B420
    int mNumFlipTextures;           // 0x00C [O]; [R] ctor cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter zeroes it; [U] setupEmitter (data +0xD0),
                                    //       updateBASE wraps the frame at it
    float mCurrentFlipTexture;      // 0x010 [O]; [R] ctor (sw 0); [U] setupEmitter (random start frame), updateBASE,
                                    //       render
    float mFlipTextureRate;         // 0x014 [O]; [R] ctor (sw 0); [U] setupEmitter (data +0xD4), updateBASE
    char unk_018[0x8];              // 0x018 padding (m_emitter is 16-aligned)
    tParticleEmitter m_emitter;     // 0x020 [O]; [R] ctor func_002F7A68(this + 0x20); [U] setupEmitter (init,
                                    //       setupEmitter), setTime; [P] getBaseEmitter
    int m_initialized;              // 0x170 [O] bool (retail 4 bytes); [R] ctor 0; [U] setupEmitter sets 1,
                                    //       purge(resetEmitter) clears it, replay record
    int m_enabled;                  // 0x174 [O] bool (retail 4 bytes); [P] enabled/setEnabled lw/sw 0x174; [R] ctor 1;
                                    //       [U] updateBASE and render return while 0, replay record
    int m_numSubEmitters;           // 0x178 [O]; [P] numSubEmitters lw 0x178; [R] ctor -1; [U] setupEmitter sets
                                    //       ceil(life * 60) and re-Allocates on change, Allocate sizes the arrays,
                                    //       purge(reset) sets -1
    int m_subEmitterId;             // 0x17C [O]; [P] subEmitterId lw 0x17C; [R] ctor 0; [U] updateBASE's ring index
                                    //       (counts down, wraps to m_numSubEmitters - 1), reset zeroes it, replay
                                    //       record
    tBoundBox m_boundBox;           // 0x180 [O]; [U] setupEmitter passes this + 0x180 to tParticleEmitter::setupEmitter
    t4Vector* m_positions;          // 0x1A0 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Positions"), purge frees,
                                    //       reset zeroes, updateBASE writes [m_subEmitterId]
    t4Vector* m_velocities;         // 0x1A4 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Velocities"), purge, reset;
                                    //       updateBASE keeps each entry's age in .w (> 0 = enabled)
    char unk_1A8[0x8];              // 0x1A8 padding
    t4Vector m_currPosition;        // 0x1B0 [O]; [U] setupEmitter zeroes it; updateBASE stores the position argument
                                    //       (or uses this when it is null)
    t4Vector m_currVelocity;        // 0x1C0 [O]; [U] setupEmitter zeroes it; updateBASE, as above
    t4Vector m_currColour;          // 0x1D0 [O]; [U] setupEmitter zeroes it; cDynamicColourEmitter::update stores the
                                    //       colour argument (or uses this)
    int m_enabledCount;             // 0x1E0 [O]; [P] getEnabledCount lw 0x1E0; [R] ctor 0; [U] updateBASE +/- 1, reset
                                    //       zeroes it, render returns at 0; cDynamicParticleModifier_update waits for 0
                                    //       (its +0x240)
    sDrawState mDrawState;          // 0x1E4 [O] (On Tour 0x10; retail's carries 2 texture ids at +0x10,
                                    //       render/ps2graphicsman.h); [R] ctor: memset 0x10 + both ids -1; [U]
                                    //       setupEmitter copies the default state sDrawState_s_defaultDrawState and sets bit fields,
                                    //       render patches the layer/blend bits and texture id and pushes it
    void** mVtbl;                   // 0x1F8 [R] ctor/dtor store cBaseClass_DynamicEmitter_vtbl (7 slots); subclasses
                                    //       store theirs; [U] setupEmitter calls slots 1, 5, 2; [P] the preview's slot
                                    //       pairs touch 0x1F8
};

CHECK_OFFSET(cBaseClass_DynamicEmitter, Duration, 0x00);
CHECK_OFFSET(cBaseClass_DynamicEmitter, TextureId, 0x04);
CHECK_OFFSET(cBaseClass_DynamicEmitter, BlendMode, 0x08);
CHECK_OFFSET(cBaseClass_DynamicEmitter, mNumFlipTextures, 0x0C);
CHECK_OFFSET(cBaseClass_DynamicEmitter, mCurrentFlipTexture, 0x10);
CHECK_OFFSET(cBaseClass_DynamicEmitter, mFlipTextureRate, 0x14);
CHECK_OFFSET(cBaseClass_DynamicEmitter, unk_018, 0x18);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_emitter, 0x20);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_initialized, 0x170);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_enabled, 0x174);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_numSubEmitters, 0x178);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_subEmitterId, 0x17C);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_boundBox, 0x180);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_positions, 0x1A0);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_velocities, 0x1A4);
CHECK_OFFSET(cBaseClass_DynamicEmitter, unk_1A8, 0x1A8);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_currPosition, 0x1B0);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_currVelocity, 0x1C0);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_currColour, 0x1D0);
CHECK_OFFSET(cBaseClass_DynamicEmitter, m_enabledCount, 0x1E0);
CHECK_OFFSET(cBaseClass_DynamicEmitter, mDrawState, 0x1E4);
CHECK_OFFSET(cBaseClass_DynamicEmitter, mVtbl, 0x1F8);
CHECK_SIZE(cBaseClass_DynamicEmitter, 0x200);

// cDynamicEmitter: cBaseClass_DynamicEmitter with its own vtable (cDynamicEmitter_vtbl) and render; no members.
struct cDynamicEmitter {
    // --- cBaseClass_DynamicEmitter (0x000-0x1FC)
    float Duration;                 // 0x000 [O]; [U] setupEmitter (cBaseClass_DynamicEmitter_setupEmitter), setTime (cBaseClass_DynamicEmitter_setTime), replay
                                    //       record; cDynamicParticleModifier_update tests it (its +0x60)
    int TextureId;                  // 0x004 [O]; [U] setupEmitter (particle data +0xC4); render: texIds[TextureId +
                                    //       (int)mCurrentFlipTexture]
    int BlendMode;                  // 0x008 [O]; [U] setupEmitter (data +0xC8); render maps it through D_0044B420
    int mNumFlipTextures;           // 0x00C [O]; [R] ctor cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter zeroes it; [U] setupEmitter (data +0xD0),
                                    //       updateBASE wraps the frame at it
    float mCurrentFlipTexture;      // 0x010 [O]; [R] ctor (sw 0); [U] setupEmitter (random start frame), updateBASE,
                                    //       render
    float mFlipTextureRate;         // 0x014 [O]; [R] ctor (sw 0); [U] setupEmitter (data +0xD4), updateBASE
    char unk_018[0x8];              // 0x018 padding (m_emitter is 16-aligned)
    tParticleEmitter m_emitter;     // 0x020 [O]; [R] ctor func_002F7A68(this + 0x20); [U] setupEmitter (init,
                                    //       setupEmitter), setTime; [P] getBaseEmitter
    int m_initialized;              // 0x170 [O] bool (retail 4 bytes); [R] ctor 0; [U] setupEmitter sets 1,
                                    //       purge(resetEmitter) clears it, replay record
    int m_enabled;                  // 0x174 [O] bool (retail 4 bytes); [P] enabled/setEnabled lw/sw 0x174; [R] ctor 1;
                                    //       [U] updateBASE and render return while 0, replay record
    int m_numSubEmitters;           // 0x178 [O]; [P] numSubEmitters lw 0x178; [R] ctor -1; [U] setupEmitter sets
                                    //       ceil(life * 60) and re-Allocates on change, Allocate sizes the arrays,
                                    //       purge(reset) sets -1
    int m_subEmitterId;             // 0x17C [O]; [P] subEmitterId lw 0x17C; [R] ctor 0; [U] updateBASE's ring index
                                    //       (counts down, wraps to m_numSubEmitters - 1), reset zeroes it, replay
                                    //       record
    tBoundBox m_boundBox;           // 0x180 [O]; [U] setupEmitter passes this + 0x180 to tParticleEmitter::setupEmitter
    t4Vector* m_positions;          // 0x1A0 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Positions"), purge frees,
                                    //       reset zeroes, updateBASE writes [m_subEmitterId]
    t4Vector* m_velocities;         // 0x1A4 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Velocities"), purge, reset;
                                    //       updateBASE keeps each entry's age in .w (> 0 = enabled)
    char unk_1A8[0x8];              // 0x1A8 padding
    t4Vector m_currPosition;        // 0x1B0 [O]; [U] setupEmitter zeroes it; updateBASE stores the position argument
                                    //       (or uses this when it is null)
    t4Vector m_currVelocity;        // 0x1C0 [O]; [U] setupEmitter zeroes it; updateBASE, as above
    t4Vector m_currColour;          // 0x1D0 [O]; [U] setupEmitter zeroes it; cDynamicColourEmitter::update stores the
                                    //       colour argument (or uses this)
    int m_enabledCount;             // 0x1E0 [O]; [P] getEnabledCount lw 0x1E0; [R] ctor 0; [U] updateBASE +/- 1, reset
                                    //       zeroes it, render returns at 0; cDynamicParticleModifier_update waits for 0
                                    //       (its +0x240)
    sDrawState mDrawState;          // 0x1E4 [O] (On Tour 0x10; retail's carries 2 texture ids at +0x10,
                                    //       render/ps2graphicsman.h); [R] ctor: memset 0x10 + both ids -1; [U]
                                    //       setupEmitter copies the default state sDrawState_s_defaultDrawState and sets bit fields,
                                    //       render patches the layer/blend bits and texture id and pushes it
    void** mVtbl;                   // 0x1F8 [R] ctor/dtor store cBaseClass_DynamicEmitter_vtbl (7 slots); subclasses
                                    //       store theirs; [U] setupEmitter calls slots 1, 5, 2; [P] the preview's slot
                                    //       pairs touch 0x1F8
};

CHECK_OFFSET(cDynamicEmitter, Duration, 0x00);
CHECK_OFFSET(cDynamicEmitter, TextureId, 0x04);
CHECK_OFFSET(cDynamicEmitter, BlendMode, 0x08);
CHECK_OFFSET(cDynamicEmitter, mNumFlipTextures, 0x0C);
CHECK_OFFSET(cDynamicEmitter, mCurrentFlipTexture, 0x10);
CHECK_OFFSET(cDynamicEmitter, mFlipTextureRate, 0x14);
CHECK_OFFSET(cDynamicEmitter, unk_018, 0x18);
CHECK_OFFSET(cDynamicEmitter, m_emitter, 0x20);
CHECK_OFFSET(cDynamicEmitter, m_initialized, 0x170);
CHECK_OFFSET(cDynamicEmitter, m_enabled, 0x174);
CHECK_OFFSET(cDynamicEmitter, m_numSubEmitters, 0x178);
CHECK_OFFSET(cDynamicEmitter, m_subEmitterId, 0x17C);
CHECK_OFFSET(cDynamicEmitter, m_boundBox, 0x180);
CHECK_OFFSET(cDynamicEmitter, m_positions, 0x1A0);
CHECK_OFFSET(cDynamicEmitter, m_velocities, 0x1A4);
CHECK_OFFSET(cDynamicEmitter, unk_1A8, 0x1A8);
CHECK_OFFSET(cDynamicEmitter, m_currPosition, 0x1B0);
CHECK_OFFSET(cDynamicEmitter, m_currVelocity, 0x1C0);
CHECK_OFFSET(cDynamicEmitter, m_currColour, 0x1D0);
CHECK_OFFSET(cDynamicEmitter, m_enabledCount, 0x1E0);
CHECK_OFFSET(cDynamicEmitter, mDrawState, 0x1E4);
CHECK_OFFSET(cDynamicEmitter, mVtbl, 0x1F8);
CHECK_SIZE(cDynamicEmitter, 0x200);

// cDynamicColourEmitter: cBaseClass_DynamicEmitter + one packed colour per sub-emitter.
struct cDynamicColourEmitter {
    // --- cBaseClass_DynamicEmitter (0x000-0x1FC)
    float Duration;                 // 0x000 [O]; [U] setupEmitter (cBaseClass_DynamicEmitter_setupEmitter), setTime (cBaseClass_DynamicEmitter_setTime), replay
                                    //       record; cDynamicParticleModifier_update tests it (its +0x60)
    int TextureId;                  // 0x004 [O]; [U] setupEmitter (particle data +0xC4); render: texIds[TextureId +
                                    //       (int)mCurrentFlipTexture]
    int BlendMode;                  // 0x008 [O]; [U] setupEmitter (data +0xC8); render maps it through D_0044B420
    int mNumFlipTextures;           // 0x00C [O]; [R] ctor cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter zeroes it; [U] setupEmitter (data +0xD0),
                                    //       updateBASE wraps the frame at it
    float mCurrentFlipTexture;      // 0x010 [O]; [R] ctor (sw 0); [U] setupEmitter (random start frame), updateBASE,
                                    //       render
    float mFlipTextureRate;         // 0x014 [O]; [R] ctor (sw 0); [U] setupEmitter (data +0xD4), updateBASE
    char unk_018[0x8];              // 0x018 padding (m_emitter is 16-aligned)
    tParticleEmitter m_emitter;     // 0x020 [O]; [R] ctor func_002F7A68(this + 0x20); [U] setupEmitter (init,
                                    //       setupEmitter), setTime; [P] getBaseEmitter
    int m_initialized;              // 0x170 [O] bool (retail 4 bytes); [R] ctor 0; [U] setupEmitter sets 1,
                                    //       purge(resetEmitter) clears it, replay record
    int m_enabled;                  // 0x174 [O] bool (retail 4 bytes); [P] enabled/setEnabled lw/sw 0x174; [R] ctor 1;
                                    //       [U] updateBASE and render return while 0, replay record
    int m_numSubEmitters;           // 0x178 [O]; [P] numSubEmitters lw 0x178; [R] ctor -1; [U] setupEmitter sets
                                    //       ceil(life * 60) and re-Allocates on change, Allocate sizes the arrays,
                                    //       purge(reset) sets -1
    int m_subEmitterId;             // 0x17C [O]; [P] subEmitterId lw 0x17C; [R] ctor 0; [U] updateBASE's ring index
                                    //       (counts down, wraps to m_numSubEmitters - 1), reset zeroes it, replay
                                    //       record
    tBoundBox m_boundBox;           // 0x180 [O]; [U] setupEmitter passes this + 0x180 to tParticleEmitter::setupEmitter
    t4Vector* m_positions;          // 0x1A0 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Positions"), purge frees,
                                    //       reset zeroes, updateBASE writes [m_subEmitterId]
    t4Vector* m_velocities;         // 0x1A4 [O]; [R] ctor 0; [U] Allocate ("DynEmitterData Velocities"), purge, reset;
                                    //       updateBASE keeps each entry's age in .w (> 0 = enabled)
    char unk_1A8[0x8];              // 0x1A8 padding
    t4Vector m_currPosition;        // 0x1B0 [O]; [U] setupEmitter zeroes it; updateBASE stores the position argument
                                    //       (or uses this when it is null)
    t4Vector m_currVelocity;        // 0x1C0 [O]; [U] setupEmitter zeroes it; updateBASE, as above
    t4Vector m_currColour;          // 0x1D0 [O]; [U] setupEmitter zeroes it; cDynamicColourEmitter::update stores the
                                    //       colour argument (or uses this)
    int m_enabledCount;             // 0x1E0 [O]; [P] getEnabledCount lw 0x1E0; [R] ctor 0; [U] updateBASE +/- 1, reset
                                    //       zeroes it, render returns at 0; cDynamicParticleModifier_update waits for 0
                                    //       (its +0x240)
    sDrawState mDrawState;          // 0x1E4 [O] (On Tour 0x10; retail's carries 2 texture ids at +0x10,
                                    //       render/ps2graphicsman.h); [R] ctor: memset 0x10 + both ids -1; [U]
                                    //       setupEmitter copies the default state sDrawState_s_defaultDrawState and sets bit fields,
                                    //       render patches the layer/blend bits and texture id and pushes it
    void** mVtbl;                   // 0x1F8 [R] ctor/dtor store cBaseClass_DynamicEmitter_vtbl (7 slots); subclasses
                                    //       store theirs; [U] setupEmitter calls slots 1, 5, 2; [P] the preview's slot
                                    //       pairs touch 0x1F8
    // --- cDynamicColourEmitter
    char unk_1FC[0x4];              // 0x1FC the base's tail padding (cBaseClass_DynamicEmitter is 0x200)
    int m_IsAvalanche;              // 0x200 [O] bool (retail 4 bytes); [R] ctor cDynamicColourEmitter_cDynamicColourEmitter zeroes it; [U]
                                    //       cPS2GraphicsMan slot 84 (func_00380CE0) reads it
    tPackedRGBA* m_colours;         // 0x204 [O] (On Tour t4Vector*; retail 4 bytes per entry: Allocate "DynEmitterData
                                    //       Colours", the preview's getSubEmitterPositionVelocityColour(int, ...,
                                    //       tPackedRGBA&)); [R] ctor 0; [U] Allocate, purge, reset (0, 0, 0, 0), update
                                    //       writes [m_subEmitterId] (bytes r, g, b, a = clamp(c * 255))
    char unk_208[0x8];              // 0x208 padding to the 16-byte size
};

CHECK_OFFSET(cDynamicColourEmitter, Duration, 0x00);
CHECK_OFFSET(cDynamicColourEmitter, TextureId, 0x04);
CHECK_OFFSET(cDynamicColourEmitter, BlendMode, 0x08);
CHECK_OFFSET(cDynamicColourEmitter, mNumFlipTextures, 0x0C);
CHECK_OFFSET(cDynamicColourEmitter, mCurrentFlipTexture, 0x10);
CHECK_OFFSET(cDynamicColourEmitter, mFlipTextureRate, 0x14);
CHECK_OFFSET(cDynamicColourEmitter, unk_018, 0x18);
CHECK_OFFSET(cDynamicColourEmitter, m_emitter, 0x20);
CHECK_OFFSET(cDynamicColourEmitter, m_initialized, 0x170);
CHECK_OFFSET(cDynamicColourEmitter, m_enabled, 0x174);
CHECK_OFFSET(cDynamicColourEmitter, m_numSubEmitters, 0x178);
CHECK_OFFSET(cDynamicColourEmitter, m_subEmitterId, 0x17C);
CHECK_OFFSET(cDynamicColourEmitter, m_boundBox, 0x180);
CHECK_OFFSET(cDynamicColourEmitter, m_positions, 0x1A0);
CHECK_OFFSET(cDynamicColourEmitter, m_velocities, 0x1A4);
CHECK_OFFSET(cDynamicColourEmitter, unk_1A8, 0x1A8);
CHECK_OFFSET(cDynamicColourEmitter, m_currPosition, 0x1B0);
CHECK_OFFSET(cDynamicColourEmitter, m_currVelocity, 0x1C0);
CHECK_OFFSET(cDynamicColourEmitter, m_currColour, 0x1D0);
CHECK_OFFSET(cDynamicColourEmitter, m_enabledCount, 0x1E0);
CHECK_OFFSET(cDynamicColourEmitter, mDrawState, 0x1E4);
CHECK_OFFSET(cDynamicColourEmitter, mVtbl, 0x1F8);
CHECK_OFFSET(cDynamicColourEmitter, unk_1FC, 0x1FC);
CHECK_OFFSET(cDynamicColourEmitter, m_IsAvalanche, 0x200);
CHECK_OFFSET(cDynamicColourEmitter, m_colours, 0x204);
CHECK_OFFSET(cDynamicColourEmitter, unk_208, 0x208);
CHECK_SIZE(cDynamicColourEmitter, 0x210);

#endif /* RENDER_PARTICLE_H */
