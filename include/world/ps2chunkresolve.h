#ifndef WORLD_PS2CHUNKRESOLVE_H
#define WORLD_PS2CHUNKRESOLVE_H

// cPS2ChunkResolve: the PS2 chunk resolver - the only cChunkResolve the game creates. On top of the base's pointer
// fix-ups it resolves whole streamed chunks by type (resolve / unresolve: textures through the graphics manager, models,
// instances, lights, ...) and patches models' and instances' DMA chains for VU1 (resolveModel -> func_003ABF20 ..
// func_003AC048, resolveInstance -> func_003AC358 / func_003AC508). One instance in cWorldCache::mp_ChunkResolve.
// EA's source: d:/ssx/code/source/world/ps2chunkresolve.cpp (preview assert path); retail code is in the decomp's unit
// world/worldview.cpp (0x003AADE8 .. 0x003AC7C4).
// Size 0x14: [R] cChunkResolve_construct (called by cWorldCache_init) does cMemMan_alloc(0x14, "cPS2ChunkResolve"),
// runs cChunkResolve_cChunkResolve and stores cPS2ChunkResolve_vtbl (0x00495090, 22 slots) at 0x8.
// Base cChunkResolve (world/chunkresolve.h, 0xC: mp_World, m_pDummyCollideRecord, vptr), written out inline with the
// same member names. Vtable slots it overrides: 1 destructor, 2 init, 3 resolve, 4 unresolve, 5 resolveModel,
// 13 resolveInstance (the rest are cChunkResolve's; see world/chunkresolve.h).
//
// A FLAT layout, like main/game.h. Tags: [R] cChunkResolve_construct, cPS2ChunkResolve_init (its only initialiser) and
// the destructor; [P] layout_xlate: the preview's paired methods (slots 3 and 4, the destructor) use 0x8, 0xC and 0x10
// at the same offsets (no accessors); [U] how retail code uses the field; [O] SSX On Tour's DWARF: class
// cPspChunkResolve (the PSP counterpart, size 0x14: mp_Nash at 0xC, m_iNumImages at 0x10; init(Nash* pNash)).

#include "layout.h"

struct cWorld;
struct tCollideRecord;
struct Nash;          // the world's nash cell tree / stream (On Tour Nash)

struct tInstanceRecord;
struct tModelRecord;

struct cPS2ChunkResolve {
    // --- cChunkResolve (world/chunkresolve.h, 0xC)
    cWorld* mp_World;                       // 0x000 [O] cChunkResolve::mp_World; [R] cPS2ChunkResolve_init stores its third
                                            //       argument; [U] resolveModel / resolveInstance read it
    tCollideRecord* m_pDummyCollideRecord;  // 0x004 [O]; [R] the base ctor; [U] resolveInstance stores it in each instance
                                            //       record (+0x88)
    void** mVtbl;                           // 0x008 [R] cChunkResolve_construct / the destructor store cPS2ChunkResolve_vtbl
                                            //       / cChunkResolve_vtbl; [U] resolve / unresolve / resolveModel make
                                            //       virtual calls through it (resolveModelHeirarchy, resolveMesh, ...)
    // --- cPS2ChunkResolve
    Nash* mp_Nash;                          // 0x00C [O] cPspChunkResolve::mp_Nash; [R] init stores its second argument;
                                            //       [P] 0xC in the preview's slots 3/4; [U] resolve / unresolve read it
    int m_iNumImages;                       // 0x010 [O] cPspChunkResolve::m_iNumImages; [R] init stores the short at +0x2A of
                                            //       **(void***)pWorld (lh, then sw); [U] resolve / unresolve add it to texture
                                            //       ids (lw; one resolve path reads only the low half, lhu)

    // Methods bound to their retail symbols by asm labels.
    void* resolve(int msg, unsigned int RID, char* obj) __asm__("cPS2ChunkResolve_resolve");
    void* unresolve(unsigned int msg, unsigned int resourceId, char* data) __asm__("cPS2ChunkResolve_unresolve");
    int resolveModel(tModelRecord* pModel) __asm__("cPS2ChunkResolve_resolveModel");
    int resolveInstance(tInstanceRecord* pInstance) __asm__("cPS2ChunkResolve_resolveInstance");
    void purge() __asm__("cPS2ChunkResolve_purge__FPv");
    void init(Nash* pNash) __asm__("cPS2ChunkResolve_init");
};
CHECK_OFFSET(cPS2ChunkResolve, mp_World, 0x0);
CHECK_OFFSET(cPS2ChunkResolve, m_pDummyCollideRecord, 0x4);
CHECK_OFFSET(cPS2ChunkResolve, mVtbl, 0x8);
CHECK_OFFSET(cPS2ChunkResolve, mp_Nash, 0xC);
CHECK_OFFSET(cPS2ChunkResolve, m_iNumImages, 0x10);
CHECK_SIZE(cPS2ChunkResolve, 0x14);

#endif /* WORLD_PS2CHUNKRESOLVE_H */
