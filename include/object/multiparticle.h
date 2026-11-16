#ifndef OBJECT_MULTIPARTICLE_H
#define OBJECT_MULTIPARTICLE_H

// cMultiParticle / cMultiParticleMan: one static emitter drawn at many placed instances (scripted "MultiParticle" /
// "MultiParticleInst" in On Tour's wscript). Retail: cMultiParticleMan methods and cMultiParticle's ctors/dtor in
// src/object/movenode.cpp (0x3579C8-0x357FA0), the rest in src/object/multiparticle.cpp (0x358020-0x358380). No EA
// path in the preview (On Tour: all_in_one_object.cpp), so the header takes the retail unit name.
//   cMultiParticle    0x50: cMemMan_alloc(0x50, "MultiParticle") + ctor func_00357DD8(pdata, maxInst) in
//                     cMultiParticleMan::addParticle (func_00357A78) and + func_00357E80(replay) in its
//                     readFromReplayFrame. No vtable (dtor func_00357FA0 is called directly).
//   cMultiParticleMan 0x10: operator new(0x10, "MultiParticleMan") + ctor func_003579C8 in cGame (main/gameload.cpp;
//                     cGame::MultiParticleMan, main/game.h +0x40). Instance cMultiParticleMan::TheMultiParticleMan =
//                     D_004A4028 (gp-relative; the ctor stores it, the dtor clears it; playbackman reads it).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] ctors/dtor/allocation sizes; [U] matched retail use; [O] SSX On
// Tour's DWARF (identical members and offsets).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h" // tBoundBox

struct cStaticEmitter;
struct tParticleData;
struct tResourceID;     // 4 bytes; the low byte is the world section (unloadSection compares it)

struct cMultiParticle {
    int MaxInst;                    // 0x000 [O]; [R] ctor stores maxInst; [U] setupMultiParticle sizes InstIds, addInst
                                    //       caps NumInst at it
    int NumInst;                    // 0x004 [O]; [R] ctor 0; [U] addInst/remInst/unloadSection, render loops over it
    tParticleData* Particle;        // 0x008 [O]; [R] ctor: cMemMan_alloc(0xD8, "Particle") + MEM_copy of pdata; dtor
                                    //       deletes; [U] setupEmitter, replay record (0xD8 bytes)
    tResourceID* InstIds;           // 0x00C [O]; [U] setupMultiParticle: operator new(MaxInst * 4, "InstIds"), filled
                                    //       with -1; unloadSection compares each id's low byte (section); dtor
                                    //       cMemMan_free
    cStaticEmitter* Emitter;        // 0x010 [O]; [U] setupMultiParticle: cMemMan_alloc(0x190, "Emitter") +
                                    //       func_00370018; update/render/replay; dtor calls its virtual dtor (slot 1,
                                    //       flags 3)
    char unk_014[0xC];              // 0x014 padding (BoundBox is 16-aligned)
    tBoundBox BoundBox;             // 0x020 [O]; [R] ctor copies Emitter->BoundBox; [U] render offsets it by each
                                    //       instance's position; replay record
    t4Vector Offset;                // 0x040 [O]; [R] ctor: Emitter->Emitter.X0 with w = 0; [U] render adds it to each
                                    //       instance's position; replay record
};

CHECK_OFFSET(cMultiParticle, MaxInst, 0x00);
CHECK_OFFSET(cMultiParticle, NumInst, 0x04);
CHECK_OFFSET(cMultiParticle, Particle, 0x08);
CHECK_OFFSET(cMultiParticle, InstIds, 0x0C);
CHECK_OFFSET(cMultiParticle, Emitter, 0x10);
CHECK_OFFSET(cMultiParticle, unk_014, 0x14);
CHECK_OFFSET(cMultiParticle, BoundBox, 0x20);
CHECK_OFFSET(cMultiParticle, Offset, 0x40);
CHECK_SIZE(cMultiParticle, 0x50);

struct cMultiParticleMan {
    cMultiParticle* Particles[4];   // 0x000 [O]; [R] ctor func_003579C8 zeroes all 4; [U] addParticle allocates [id]
                                    //       (cMemMan_alloc(0x50, "MultiParticle")), reset/dtor delete them,
                                    //       update/render/replay/unloadSection loop over 4
};

CHECK_OFFSET(cMultiParticleMan, Particles, 0x00);
CHECK_SIZE(cMultiParticleMan, 0x10);

#endif /* OBJECT_MULTIPARTICLE_H */
