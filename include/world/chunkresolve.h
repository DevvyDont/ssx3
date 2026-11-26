#ifndef WORLD_CHUNKRESOLVE_H
#define WORLD_CHUNKRESOLVE_H

// cChunkResolve: fixes up streamed world chunks after loading: turns the offsets in model / mesh / material /
// animation / instance / particle / spline / collision / halo / light records into pointers (the resolve* virtuals).
// Abstract-ish base of cPS2ChunkResolve (the only class instantiated: cChunkResolve_construct, called by
// cWorldCache_init, does cMemMan_alloc(0x14, "cPS2ChunkResolve"), runs cChunkResolve_cChunkResolve and stores
// cPS2ChunkResolve_vtbl at +0x8; cWorldCache::mp_ChunkResolve keeps it).
//
// Size 0xC: [R] the constructor stores the vptr at 0x8, after the two data words, and cPS2ChunkResolve's own members
// start at 0xC (its init / resolve / unresolve use 0xC and 0x10; 0x14 allocated) - the same as On Tour's size 0xC.
// No base class; the class declares the virtuals, so g++ 2.95 puts the vptr after its data (0x8).
// EA's source: d:/ssx/code/source/world/chunkresolve.cpp (cPS2ChunkResolve: world/ps2chunkresolve.cpp), both in the
// preview's strings. Retail methods: unit world/worldview 0x003AA2F0-0x003AADE4 and unit world/wscriptcache
// (cChunkResolve__cChunkResolve, cChunkResolve_resolveMeshContext__FPv).
//
// Vtable cChunkResolve_vtbl (0x00495150, 22 slots, On Tour's method order): 1 dtor, 2-4 init / resolve / unresolve
// (the shared empty stub func_00411530), 5 resolveModel, 6 resolveModelHeirarchy, 7 resolveMesh, 8 resolveMeshContext,
// 9 resolveMaterial, 10 resolveMaterialBlocks, 11 resolveAnimation, 12 resolveChannel, 13 resolveInstance,
// 14 resolveParticleModel, 15 resolveParticleModelHeirarchy, 16 resolveParticleMesh, 17 resolveParticleInstance,
// 18 resolveSpline, 19 resolveCollideModel, 20 resolveHalo, 21 resolveLight, 22 unresolveLight.
//
// A FLAT layout, like main/game.h. Member names and types are EA's, from SSX On Tour's PSP DWARF ([O] class
// cChunkResolve, same offsets).

#include "layout.h"

struct cWorld;
struct tCollideRecord;      // retail: 0x130 bytes (the ctor MEM_fill's the static dummy record)

struct tAnimRecord;
struct tChannelRecord;
struct tCollideModelRecord;
struct tHaloRecord;
struct tInstanceRecord;
struct tLightRecord;
struct tMaterialRecord;
struct tMeshRecord;
struct tModelRecord;
struct tParticleInstanceRecord;
struct tParticleMeshRecord;
struct tParticleModelRecord;
struct tSplineRecord;

struct cChunkResolve {
    cWorld* mp_World;                       // 0x0 [O]; [R] the ctor zeroes it; [U] cPS2ChunkResolve_init stores it;
                                            //     resolveInstance / resolveMaterialBlocks / resolveParticleInstance
                                            //     read it
    tCollideRecord* m_pDummyCollideRecord;  // 0x4 [O]; [R] the ctor points it at the static cChunkResolve_cChunkResolve_colliderecord and
                                            //     initialises that record (0x130 bytes); [U]
                                            //       cPS2ChunkResolve_resolveInstance
                                            //     stores it in each instance record (+0x88)
    void** mVtbl;                           // 0x8 [R] the ctor / dtor store cChunkResolve_vtbl, cChunkResolve_construct
                                            //     cPS2ChunkResolve_vtbl; [U] resolveModel, resolveModelHeirarchy,
                                            //       resolveMesh,
                                            //     resolveAnimation and the particle resolvers make virtual calls
                                            //       through it

    // A streamed record's offset or index field that points nowhere, before the fix-up (name ours): [U]
    // resolveModelHeirarchy (a mesh matrix: the identity), resolveMaterial (no texture flip table), resolveSpline (no
    // previous / next segment) and resolveCollideModel (no data, list or sphere trees) turn it into 0 or a default.
    enum { kNullOffset = -1 };

    // Methods bound to their retail symbols by asm labels.
    int resolveModel(tModelRecord* pModel) __asm__("cChunkResolve_resolveModel");
    int resolveModelHeirarchy(tModelRecord* pModel) __asm__("cChunkResolve_resolveModelHeirarchy");
    int resolveMesh(tMeshRecord* pMesh, int base) __asm__("cChunkResolve_resolveMesh");
    int resolveParticleModel(tParticleModelRecord* pModel) __asm__("cChunkResolve_resolveParticleModel");
    int resolveParticleModelHeirarchy(tParticleModelRecord* pModel)
        __asm__("cChunkResolve_resolveParticleModelHeirarchy");
    int resolveParticleMesh(tParticleMeshRecord* pMesh) __asm__("cChunkResolve_resolveParticleMesh");
    int resolveParticleInstance(tParticleInstanceRecord* pInstance) __asm__("cChunkResolve_resolveParticleInstance");
    int resolveMaterial(tMaterialRecord* pMaterial) __asm__("cChunkResolve_resolveMaterial");
    int resolveMaterialBlocks(int n, int* data) __asm__("cChunkResolve_resolveMaterialBlocks");
    int resolveAnimation(tAnimRecord* pAnimation, int base) __asm__("cChunkResolve_resolveAnimation");
    int resolveChannel(tChannelRecord* pChannel, int pBase) __asm__("cChunkResolve_resolveChannel");
    int resolveInstance(tInstanceRecord* node) __asm__("cChunkResolve_resolveInstance");
    int resolveSpline(tSplineRecord* list) __asm__("cChunkResolve_resolveSpline");
    int resolveCollideModel(void* ctx, tCollideModelRecord* pCollideModel) __asm__("cChunkResolve_resolveCollideModel");
    int resolveHalo(tHaloRecord* obj) __asm__("cChunkResolve_resolveHalo");
    bool resolveLight(tLightRecord* obj) __asm__("cChunkResolve_resolveLight");
    bool unresolveLight(tLightRecord* pRTLight) __asm__("cChunkResolve_unresolveLight");
    float calcMaxLightDistance(tLightRecord* light, float divisor) __asm__("cChunkResolve_calcMaxLightDistance");
};

CHECK_OFFSET(cChunkResolve, mp_World, 0x0);
CHECK_OFFSET(cChunkResolve, m_pDummyCollideRecord, 0x4);
CHECK_OFFSET(cChunkResolve, mVtbl, 0x8);
CHECK_SIZE(cChunkResolve, 0xC);

#endif /* WORLD_CHUNKRESOLVE_H */
