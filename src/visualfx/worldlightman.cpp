#include "common.h"
#include "visualfx/attachedemitterfx.h"
#include "ai/rider.h"
#include "worldpainter/worldpainterquery.h"
#include "world/instancerecord.h"
#include "vtable.h"
#include "render/irradiance.h"
#include "world/worldcache.h"
#include "visualfx/worldlightman.h"
#include "bx/appman.h"
#include "ui/uithing.h"
#include "visualfx/snowfall.h"
#include "visualfx/visualeffectsman.h"
#include "visualfx/transitionobj.h"
#include "visualfx/aivisualeffectsman.h"
#include "visualfx/lensfx.h"
#include "visualfx/cutscenetransitionman.h"
#include "visualfx/avalanche.h"
#include "visualfx/transitionman.h"
#include "bx/math3d.h"
#include "visualfx/fog.h"
#include "ai/riderhandler.h"
#include "be/beintnetwork.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_initLightCache);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_RTLOverlapList[];

struct sWLCVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sWLCVec4 t4vect_Zero;

struct sWLCList {
    unsigned int head;
    char pad04[0x100];
};

struct sWLCBuf {
    int owner;
    int count;
    sWLCList lists[3];
};

struct sWLCNode {
    unsigned int id;
    int priority;
    char pad08[0xA8];
    sWLCVec4 v;
    int fC0;
    char padC4[0xAC];
    sWLCNode* next;
    sWLCNode* prev;
    char pad178[8];
};

// Light slot view based at self+0x40*i: id/f14 at +0x10/+0x14; slot i's cache buffer
// lives at +0x40 (cWorldLightMan_purgeLightCache views it as slots[i].buf at self+0x40).
struct sWLCLightHdr {
    char pad00[0x10];
    unsigned int id;
    int f14;
    char pad18[0x28];
};

struct sWLCMan {
    union {
        int f00;
        sWLCLightHdr hdr[8];
    };
    char pad200[0x10];
    sWLCNode nodes[32];
    sWLCNode* heads[32];
    int count;
};

/**
 * Sets up the world light cache: eight empty light slots with their "RTLOverlapList" buffers and 32 free
 * light nodes.
 */
extern "C" void cWorldLightMan_initLightCache(sWLCMan* self)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        sWLCLightHdr* light = &self->hdr[i];
        light->id = 0xFFFFFFFF;
        light->f14 = 0;
        sWLCBuf* buf = (sWLCBuf*)operator new(2580, str_RTLOverlapList, 0x20000000, 0);
        buf->owner = self->f00;
        buf->count = 0;
        buf->lists[0].head = 0xFFFFFFFF;
        buf->lists[1].head = 0xFFFFFFFF;
        buf->lists[2].head = 0xFFFFFFFF;
        *(sWLCBuf**)((char*)light + 0x40) = buf;
    }
    for (i = 0; i < 32; i++) {
        self->nodes[i].priority = 0;
        self->nodes[i].id = 0xFFFFFFFF;
        self->nodes[i].v = t4vect_Zero;
        self->nodes[i].fC0 = 0;
        self->nodes[i].next = 0;
        self->nodes[i].prev = 0;
    }
    self->count = 0;
    for (i = 0; i < 32; i++) {
        self->heads[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_purgeLightCache);
#ifdef SKIP_ASM
void operator_delete(int*);

struct cWorldLightMan_purgeLightCache_sSlot {
    int* buf;
    char pad04[0x3C];
};

struct cWorldLightMan_purgeLightCache_sMan {
    char pad00[0x40];
    cWorldLightMan_purgeLightCache_sSlot slots[8];
};

/**
 * Frees the world light manager's light cache.
 *
 * Calls operator delete on the buffer pointer at the start of each of the eight 0x40-byte slots from +0x40.
 */
extern "C" void cWorldLightMan_purgeLightCache(cWorldLightMan_purgeLightCache_sMan* self)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        operator_delete(self->slots[i].buf);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_getRealTimeLights);
#ifdef SKIP_ASM
extern char cIrradianceDataBase_TheDataBase[];
extern int cIrradianceDataBase_mDefaultId;
extern int cRenderStateMan_m_hasCustomIrradiance;

struct sVec_5400 {
    float x, y, z, w;
    sVec_5400() {}
    sVec_5400(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// Plain (ctor-less) quadword for matrix rows, so a local matrix needs no construction loop.
struct sQuad_5400 {
    float x, y, z, w;
} __attribute__((aligned(16)));


struct sAABox_5400 {
    sVec_5400 min;
    sVec_5400 max;
    sAABox_5400() {}
    sAABox_5400(const sVec_5400& a, const sVec_5400& b) { min = a; max = b; }
};


struct sBox_5400 {
    unsigned int id;
    int priority;
    char pad08[0x8];
    sAABox_5400 bounds; // 0x10
    void* cell;         // 0x30
};


struct sMan_5400 {
    char pad000[0x4];
    char* world;              // 0x4
    char pad008[0x210 - 0x8];
    tLightCacheEntry nodes[32];     // 0x210
    tLightCacheEntry* heads[32];    // 0x3210
    unsigned int count;       // 0x3290
};

// Light-set reference: a name until first use, then a cached handle tagged 0x123400.
struct sLightRef_5400 {
    int handle;
    int magic;
};

struct sVEf_5400 {
    short delta;
    short index;
    int (*fn)(void*, float, float, float);
};
struct sVEr_5400 {
    short delta;
    short index;
    sLightRef_5400* (*fn)(void*);
};

// The unit defines cWorldLightMan_evictHashTableEntry later with its own node type.
extern "C" tLightCacheEntry* cWorldLightMan_evictHashTableEntry_n(sMan_5400* self, int priority) __asm__("cWorldLightMan_evictHashTableEntry");
extern "C" sBox_5400* cWorldLightMan_evictOverlapNode(sMan_5400* self, int priority);
extern "C" void cWorldLightMan_IntersectLights1(void* self, void* a1, void* a2, void* a3, int count);
extern "C" void cInstanceNode_buildMeshMat(void* obj, int a1, t4Matrix* out);
extern "C" void cOverlapList_gettyped(void* cell, sAABox_5400* box);
extern "C" cIrradiance* cRenderStateMan_getCustomIrradiance(void);
extern "C" int cIrradianceDataBase_GetIDByName(void*, void*);
extern "C" cIrradiance* cIrradianceDataBase_Get(void* self, int i);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_5400 Sub_5400(const sVec_5400& a, const sVec_5400& b)
{
    sVec_5400 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_5400 Add_5400(const sVec_5400& a, const sVec_5400& b)
{
    sVec_5400 r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_5400(const sVec_5400& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

static inline sVec_5400 Trans_5400(const t4Matrix& m)
{
    return *(const sVec_5400*)&((sQuad_5400*)&m.row[0])[3];
}

static inline sVec_5400 ObjPos_5400(char* vobj)
{
    tInstanceRecord* obj = (tInstanceRecord*)vobj;
    return *(sVec_5400*)&obj->m_fmTransform.m[3][0];
}

static inline cIrradiance* Resolve_5400(sLightRef_5400* ref)
{
    if (ref->magic != 0x123400) {
        ref->handle = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, ref);
        ref->magic = 0x123400;
    }
    return cIrradianceDataBase_Get(cIrradianceDataBase_TheDataBase, ref->handle);
}

// Pushes node onto the front of hash chain heads[slot].
static inline void Link_5400(tLightCacheEntry** heads, int slot, tLightCacheEntry* node)
{
    tLightCacheEntry** p = heads + slot;
    tLightCacheEntry* old = *p;
    node->pPrev = 0;
    node->pNext = old;
    if (old != 0) {
        old->pPrev = node;
    }
    *p = node;
}

static inline sLightRef_5400* GetRef_5400(char* vo, int slot)
{
    tWPInGameData* o = (tWPInGameData*)vo;
    sVEr_5400* vt = (sVEr_5400*)o->mVtbl;
    return vt[slot].fn(vthis(o, vt[slot]));
}

/**
 * Returns an instance's cached real-time lighting, finding or creating its light-cache entry and relighting it when the
 * instance is new or has moved.
 *
 * The entry is looked up by the instance id (+0x78) in the 32-chain hash at +0x3210 (chain (id >> 8) & 0x1F); a new one
 * comes from the 32 entries at +0x210 (count +0x3290) or, when they are used up, from cWorldLightMan_evictHashTableEntry, the least recently
 * used. Its tick (+0x4) becomes cAppMan_TheAppMan +0x18. The position is the translation of cInstanceNode_buildMeshMat
 * when the instance has a mesh node (+0xC), else the instance's +0x40; with @p force it is re-read for a known entry,
 * which relights after a move of more than 10 and reloads its irradiance after more than 500.
 *
 * On a relight the entry's overlap node (+0xC0, taken from cWorldLightMan_evictOverlapNode unless it still belongs to this entry) gets a
 * 2000-unit box around the position, rebuilt with cOverlapList_gettyped when new or left. A reload sets the target
 * irradiance (+0xD0) to cRenderStateMan_getCustomIrradiance's custom one while cRenderStateMan_m_hasCustomIrradiance is set; otherwise it queries the world painter
 * (+0x4) at the position and uses the object irradiance name (accessor 40), else the light irradiance name (37), else
 * the default cIrradianceDataBase_mDefaultId. The current set (+0x10) is then the target plus the 4 strongest lights of the overlap list
 * (cWorldLightMan_IntersectLights1). getRealTimeLights calls it with bit 12 of the instance's +0x8 as @p force.
 *
 * @param obj  the instance record
 * @param force  nonzero for a moving instance: re-check its position
 * @return the entry's current light set (+0x10)
 */
extern "C" cIrradiance* cWorldLightMan_getRealTimeLights(sMan_5400* self, tInstanceRecord* obj, int force)
{
    unsigned int key = obj->m_RID;
    int slot = (key >> 8) & 0x1F;
    int found = 0;
    tLightCacheEntry* node;
    for (node = self->heads[slot]; node != 0; node = node->pNext) {
        if (node->RID == obj->m_RID) {
            found = 1;
            break;
        }
    }
    int prio = cAppMan_TheAppMan->TimerTick;
    if (!found) {
        if (self->count < 32) {
            node = &self->nodes[self->count++];
        } else {
            node = cWorldLightMan_evictHashTableEntry_n(self, prio);
        }
        node->RID = obj->m_RID;
        Link_5400(self->heads, slot, node);
    }
    node->iTick = prio;
    int reload = 0;
    int moved = 0;
    sVec_5400 pos;
    if (!found) {
        reload = 1;
        moved = 1;
        if ((int)obj->m_pActiveNode != 0) {
            t4Matrix meshMat;
            cInstanceNode_buildMeshMat(obj, 0, &meshMat);
            pos = Trans_5400(meshMat);
        } else {
            pos = ObjPos_5400(((char*)obj));
        }
    } else if (force) {
        if ((int)obj->m_pActiveNode != 0) {
            t4Matrix meshMat;
            cInstanceNode_buildMeshMat(obj, 0, &meshMat);
            pos = Trans_5400(meshMat);
        } else {
            pos = ObjPos_5400(((char*)obj));
        }
        float moveDist = Length_5400(Sub_5400(pos, (*(sVec_5400*)&node->pos)));
        if (10.0f < moveDist) moved = 1;
        if (500.0f < moveDist) reload = 1;
    }
    if (moved) {
        (*(sVec_5400*)&node->pos) = pos;
        int rebuild = 0;
        if (((sBox_5400*)node->pOverlapNode) == 0 || ((sBox_5400*)node->pOverlapNode)->id != node->RID) {
            (*(sBox_5400**)&node->pOverlapNode) = cWorldLightMan_evictOverlapNode(self, node->iTick);
            ((sBox_5400*)node->pOverlapNode)->id = node->RID;
            rebuild = 1;
        }
        ((sBox_5400*)node->pOverlapNode)->priority = node->iTick;
        if (!rebuild) {
            sBox_5400* overlapBox = ((sBox_5400*)node->pOverlapNode);
            int inside = 0;
            if (overlapBox->bounds.min.x < (*(sVec_5400*)&node->pos).x && (*(sVec_5400*)&node->pos).x < overlapBox->bounds.max.x && overlapBox->bounds.min.y < (*(sVec_5400*)&node->pos).y && (*(sVec_5400*)&node->pos).y < overlapBox->bounds.max.y &&
                overlapBox->bounds.min.z < (*(sVec_5400*)&node->pos).z && (*(sVec_5400*)&node->pos).z < overlapBox->bounds.max.z) {
                inside = 1;
            }
            if (!inside) rebuild = 1;
        }
        if (rebuild) {
            float extent = 1000.0f;
            ((sBox_5400*)node->pOverlapNode)->bounds = sAABox_5400(Sub_5400((*(sVec_5400*)&node->pos), sVec_5400(extent, extent, extent, 0.0f)),
                                            Add_5400((*(sVec_5400*)&node->pos), sVec_5400(extent, extent, extent, 0.0f)));
            cOverlapList_gettyped(((sBox_5400*)node->pOverlapNode)->cell, &((sBox_5400*)node->pOverlapNode)->bounds);
        }
        if (reload) {
            if (cRenderStateMan_m_hasCustomIrradiance != 0) {
                node->ObjIrradiance = *cRenderStateMan_getCustomIrradiance();
            } else {
                tWorldPainterQuery* world = (tWorldPainterQuery*)self->world;
                sVEf_5400* vt = (sVEf_5400*)world->mVtbl;
                vt[2].fn(vthis(world, vt[2]), (*(sVec_5400*)&node->pos).x, (*(sVec_5400*)&node->pos).y, -99999.0f);
                sLightRef_5400* ref = GetRef_5400(*(char**)self->world, 40);
                if (*(signed char*)ref != 0) {
                    node->ObjIrradiance = *Resolve_5400(ref);
                } else {
                    ref = GetRef_5400(*(char**)self->world, 37);
                    if (*(signed char*)ref != 0) {
                        node->ObjIrradiance = *Resolve_5400(ref);
                    } else {
                        node->ObjIrradiance = *cIrradianceDataBase_Get(cIrradianceDataBase_TheDataBase, cIrradianceDataBase_mDefaultId);
                    }
                }
            }
        }
        node->RTlightInfo = node->ObjIrradiance;
        cWorldLightMan_IntersectLights1(self, &(*(sVec_5400*)&node->pos), ((sBox_5400*)node->pOverlapNode)->cell, &node->RTlightInfo, 4);
    }
    return &node->RTlightInfo;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_flushOverlapCache);
#ifdef SKIP_ASM
struct sWorldLight {
    char pad00[0x10];
    unsigned int id;
    char pad14[0x2C];
};

/**
 * Invalidates the world light manager's 8 overlap nodes (id -1), so the next relight rebuilds their light lists.
 *
 * The parameter is the manager itself: the 0x40-byte records start with the id at +0x10, the overlap nodes at +0x10.
 * flushWorldLightCache (world script cache) calls it on the game's WorldLightMan.
 *
 * @param lights  the world light manager (this), walked as 0x40-byte overlap nodes
 */
extern "C" void cWorldLightMan_flushOverlapCache(sWorldLight* lights)
{
    unsigned int i;
    for (i = 0; i < 8; i++) {
        lights[i].id = 0xFFFFFFFF;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_evictHashTableEntry);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sWorldLightNode {
    long key;
    char pad08[0x168];
    sWorldLightNode* next;
    sWorldLightNode* prev;
    char pad178[8];
};

struct sWorldLightMan {
    char pad000[0x210];
    sWorldLightNode nodes[32];
    sWorldLightNode* heads[32];
};

/**
 * Takes the least recently used light-cache entry for reuse: the one of the 32 with the lowest tick, unlinked from its
 * hash chain.
 *
 * Entries with a tick (+0x4) of @p priority or more are never picked. The chain is (id >> 8) & 0x1F of the entry's
 * first word; the entry is unlinked through its next (+0x170) and prev (+0x174) links. cWorldLightMan_getRealTimeLights calls it when all
 * 32 entries are in use.
 *
 * @param priority  the current tick (cAppMan_TheAppMan +0x18)
 * @return the entry, or null when every tick is at least @p priority
 */
extern "C" sWorldLightNode* cWorldLightMan_evictHashTableEntry(sWorldLightMan* self, int priority)
{
    sWorldLightNode* best = 0;
    int min = priority + 1;
    unsigned int i;
    for (i = 0; i < 32; i++) {
        int nodePriority = *(int*)((char*)&self->nodes[i] + 4);
        if (nodePriority < min) {
            min = nodePriority;
            best = &self->nodes[i];
        }
    }
    int slot = (int)(best->key >> 8) & 0x1F;
    if (best->prev == 0) {
        self->heads[slot] = best->next;
        if (best->next != 0) {
            best->next->prev = 0;
        }
    } else {
        if (best->next == 0) {
            best->prev->next = 0;
        } else {
            best->prev->next = best->next;
            best->next->prev = best->prev;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_evictOverlapNode);
#ifdef SKIP_ASM



// The unit declares this function as `sBox_5400* cWorldLightMan_evictOverlapNode(sMan_5400*, int)` (line ~198); define it under its own types.
extern "C" tOverlapListNode* cWorldLightMan_evictOverlapNode_5A70(cWorldLightMan* self, int iCurrentTick) __asm__("cWorldLightMan_evictOverlapNode");
/**
 * Takes an overlap node for reuse: the first free one (id -1) of the 8, else the one with the oldest tick.
 *
 * Nodes with a tick of @p iCurrentTick or more are never picked. cWorldLightMan_getRealTimeLights calls it when an entry needs an overlap
 * node.
 *
 * @param iCurrentTick  the current tick (cAppMan_TheAppMan +0x18)
 * @return the node, or null when none qualifies
 */
extern "C" tOverlapListNode* cWorldLightMan_evictOverlapNode_5A70(cWorldLightMan* self, int iCurrentTick)
{
    tOverlapListNode* pNode = 0;
    int iOldest = iCurrentTick + 1;
    unsigned int i;
    for (i = 0; i < 8; i++) {
        if (self->OverlapNodeList[i].RID == tResourceID::Invalid()) {
            pNode = &self->OverlapNodeList[i];
            break;
        }
        if (self->OverlapNodeList[i].iTick < iOldest) {
            iOldest = self->OverlapNodeList[i].iTick;
            pNode = &self->OverlapNodeList[i];
        }
    }
    return pNode;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_IntersectLights1);
#ifdef SKIP_ASM
extern "C" void cWorldLightMan_IntersectLights(void* self, void* a1, void* a2, void** out, int count);
extern "C" void func_0038A6A8(void* a, void* b, void* c);

/**
 * Adds the strongest lights of an overlap list at a point to a light set.
 *
 * cWorldLightMan_IntersectLights picks up to @p count lights (at most 32) by intensity at @p pos; each found one is
 * added to @p irradiance with func_0038A6A8. cWorldLightMan_getRealTimeLights calls it with 4 lights and the cache
 * entry's light set.
 *
 * @param pos  the point to light
 * @param overlapList  the overlap list to search
 * @param irradiance  the light set (On Tour cIrradiance&) to add the lights to
 * @param count  the maximum number of lights
 */
extern "C" void cWorldLightMan_IntersectLights1(void* self, void* pos, void* overlapList, void* irradiance, int count)
{
    void* buf[32];
    int i;
    cWorldLightMan_IntersectLights(self, pos, overlapList, buf, count);
    for (i = 0; i < count; i++) {
        if (buf[i] != 0) {
            func_0038A6A8(irradiance, pos, buf[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_IntersectLights);
#ifdef SKIP_ASM
extern "C" float cWorldLightMan_GetIntensity(void* a, t4Vector* b, void* light);


struct sLightMgr_5B68 {
    char pad_0x0[0x210];
    unsigned int count;         // 0x210
    tLightRecord* lights[1];     // 0x214
};

/**
 * Picks the up to @p max strongest lights of an overlap list at a point, best first.
 *
 * Walks the list's records (count +0x210, pointers from +0x214) of resource type 6 (+0x8), scores each with
 * cWorldLightMan_GetIntensity and keeps the best @p max in @p outp in descending order (cleared to null first); a light must beat the
 * weakest kept score, and scores of 0 or less never count. Called by cWorldLightMan_IntersectLights1 and cRider_updateActiveLights (8
 * lights at the rider's hips).
 *
 * @param lightMan  the world light manager (passed on to cWorldLightMan_GetIntensity)
 * @param point  the point to light
 * @param mgrp  the overlap list
 * @param outp  receives the lights; @p max entries
 * @param max  how many lights to keep, at most 32
 */
extern "C" void cWorldLightMan_IntersectLights(void* lightMan, void* point, void* mgrp, void** outp, int max)
{
    sLightMgr_5B68* mgr = (sLightMgr_5B68*)mgrp;
    tLightRecord** out = (tLightRecord**)outp;
    float score[32];
    int count = 0;
    int i;
    for (i = 0; i < max; i++) {
        out[i] = 0;
        score[i] = 0.0f;
    }
    float minScore = 0.0f;
    unsigned int n = mgr->count;
    for (unsigned int j = 0; j < n; j++) {
        tLightRecord* lightRec = mgr->lights[j];
        if (lightRec->m_iIntersect[2] == 6) {
            float lightScore = cWorldLightMan_GetIntensity(lightMan, (t4Vector*)point, lightRec);
            if (minScore < lightScore) {
                int k;
                for (k = 0; k < count && lightScore < score[k]; k++) {
                }
                if (k < max) {
                    count += count < max;
                    for (int shiftIndex = count - 1; k < shiftIndex; shiftIndex--) {
                        out[shiftIndex] = out[shiftIndex - 1];
                        score[shiftIndex] = score[shiftIndex - 1];
                    }
                    out[k] = lightRec;
                    score[k] = lightScore;
                }
                minScore = score[max - 1];
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_GetIntensity);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" float cWorldLightMan_GetIntensity(void* a, t4Vector* b, void* light);

struct cWorldLightMan_CalcLightDist_sVec3;
struct cWorldLightMan_CalcLightDist_sLight;
extern "C" int cWorldLightMan_CalcLightDist(void* self, cWorldLightMan_CalcLightDist_sVec3* p, tLightRecord* light,
                             float* outDist, float* outInvDist, float* outDot);


static inline float Atten_5D30(tLightRecord* l, float dist)
{
    if (l->m_fMaxDistance < 5000.0f || dist <= 3750.0f) {
        return 1.0f;
    }
    return 1.0f - (dist - 3750.0f) * 0.0008f;
}

static inline float Fall_5D30(tLightRecord* l, float inv)
{
    int p = l->m_cDecayRate;
    float s = inv * 100.0f;
    if (s < 1.0f) {
        switch (p) {
        case 1:
            return s;
        case 2:
            return s * s;
        case 3:
            return s * s * s;
        }
    }
    return 1.0f;
}

static inline float PowN_5D30(float x, int n)
{
    if (x == 0.0f) {
        return 0.0f;
    }
    if (n < 0) {
        x = 1.0f / x;
        n = -n;
    }
    float r = 1.0f;
    while (n) {
        if (n & 1) {
            r *= x;
        }
        n >>= 1;
        x *= x;
    }
    return r;
}

static inline float Pow_5D30(float x, int n)
{
    switch (n) {
    case 0:
        return 1.0f;
    case 1:
        return x;
    case 2:
        return x * x;
    case 3:
        return x * x * x;
    case 4:
        return x * x * x * x;
    default:
        return PowN_5D30(x, n);
    }
}

/**
 * Returns a light's intensity at a point, by light type: 0 ambient, 1 spot, 2 point, 3 none.
 *
 * Type (+0x10) 0 returns the scale (+0x18). Type 1 and 2 need the point within the radius (+0x1C, cWorldLightMan_CalcLightDist); the
 * result is scale * intensity (+0x14) * the falloff (with s = 100 / distance below 1: s, s^2 or s^3 for falloff 1, 2 or
 * 3 at +0x64, else 1) * the distance fade (1, or 1 - (distance - 3750) * 0.0008 for a radius of 5000 or more past
 * 3750). A spot (type 1) also needs the cosine to the light direction at least the outer value (+0x60) and multiplies
 * by cosine^n (n = +0x65) inside the inner value (+0x5C), else by inner^n scaled linearly between outer and inner.
 * Anything else gives 0. Likely bug: for a radius above 5000 the fade turns negative past a distance of 5000.
 *
 * @param lightMan  the world light manager (passed on to cWorldLightMan_CalcLightDist)
 * @param point  the point to light
 * @param lp  the light record
 * @return the intensity
 */
extern "C" float cWorldLightMan_GetIntensity(void* lightMan, t4Vector* point, void* lp)
{
    tLightRecord* light = (tLightRecord*)lp;
    float dist;
    float inv;
    float dot;
    float att;
    float amount;
    switch (light->m_eLightType) {
    case 0:
        return light->m_fRGBIntensity;
    case 1:
        if (cWorldLightMan_CalcLightDist(lightMan, (cWorldLightMan_CalcLightDist_sVec3*)point, light, &dist, &inv, &dot)) {
            if (dot >= light->unk_060) {
                att = Atten_5D30(light, dist);
                amount = Fall_5D30(light, inv) * light->m_fIntensity * att;
                if (light->unk_05C <= dot) {
                    amount *= Pow_5D30(dot, light->m_cDropOff);
                } else {
                    amount *= Pow_5D30(light->unk_05C, light->m_cDropOff);
                    amount *= (dot - light->unk_060) / (light->unk_05C - light->unk_060);
                }
                return light->m_fRGBIntensity * amount;
            }
        }
        break;
    case 2:
        if (cWorldLightMan_CalcLightDist(lightMan, (cWorldLightMan_CalcLightDist_sVec3*)point, light, &dist, &inv, 0)) {
            att = Atten_5D30(light, dist);
            amount = Fall_5D30(light, inv) * light->m_fIntensity * att;
            return light->m_fRGBIntensity * amount;
        }
        break;
    case 3:
        return 0.0f;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWorldLightMan_CalcLightDist);
#ifdef SKIP_ASM
struct cWorldLightMan_CalcLightDist_sVec3 {
    float x;
    float y;
    float z;
};


/**
 * Measures a point against a light: fails outside the light's radius, else returns the distance, its inverse and the
 * cosine to the light's direction.
 *
 * Uses the light's position (+0x38), radius (+0x1C) and direction (+0x2C); the cosine is -(offset . direction) /
 * distance, written only when @p outDot is not null. A zero distance gives distance and inverse 1.
 *
 * @param point  the point
 * @param light  the light record
 * @param outDist  receives the distance
 * @param outInvDist  receives 1 / distance
 * @param outDot  receives the cosine, or null
 * @return 1 when the point is within the radius, else 0
 */
extern "C" int cWorldLightMan_CalcLightDist(void* self, cWorldLightMan_CalcLightDist_sVec3* point, tLightRecord* light,
                             float* outDist, float* outInvDist, float* outDot)
{
    float dx = (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvWorldPos[0]).x - point->x;
    float dy = (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvWorldPos[0]).y - point->y;
    float dz = (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvWorldPos[0]).z - point->z;
    float distSq = dx * dx + dy * dy + dz * dz;
    float dist;
    float inv;
    if (light->m_fMaxDistance * light->m_fMaxDistance < distSq) {
        return 0;
    }
    if (distSq != 0.0f) {
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(distSq));
        inv = 1.0f / dist;
    } else {
        inv = 1.0f;
        dist = 1.0f;
    }
    if (outDot != 0) {
        *outDot = -(dx * (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvDirection[0]).x + dy * (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvDirection[0]).y + dz * (*(cWorldLightMan_CalcLightDist_sVec3*)&light->m_fvDirection[0]).z) * inv;
    }
    *outDist = dist;
    *outInvDist = inv;
    return 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_cCheatFX);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEnt6238 {
    short delta;
    short index;
    void* fn;
};

struct sVt9_6238 {
    sVtEnt6238 e[9];
} __attribute__((aligned(8)));

struct sVt22_6238 {
    sVtEnt6238 e[22];
} __attribute__((aligned(8)));

extern const sVt9_6238 D_004886E0;
extern const sVt22_6238 D_00488728;
extern char D_00459B90[];

extern "C" void cRider_cRider(void* self);
void* cCheatFX_resetStates(void* self);

/**
 * Constructs a rider's cheat-code tracker (g++ 2.95 constructor for a class with the virtual base cRider).
 *
 * When in charge it also constructs the cRider base; otherwise it patches the vtable deltas for the virtual
 * base
 * offset. The cheat state starts cleared.
 *
 * @param inChrg  nonzero when this constructor also builds the virtual base
 * @return @p self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0x10): vtable copies with delta fixups when not in charge.
extern "C" void* cCheatFX_cCheatFX(cCheatFX* self, int inChrg)
{
    sVt9_6238 ctorVtbl9;
    sVt22_6238 ctorVtbl22;
    if (inChrg) {
        *(void**)self = (char*)self + 0x10;
        cRider_cRider((char*)self + 0x10);
    }
    *(void**)(*(char**)self + 0x6E8) = (void*)&D_004886E0;
    *(void**)(*(char**)self + 0x6D0) = D_00459B90;
    *(void**)(*(char**)self + 0x6C0) = (void*)&D_00488728;
    if (inChrg == 0) {
        int vc;
        ctorVtbl9 = D_004886E0;
        *(void**)(*(char**)self + 0x6E8) = &ctorVtbl9;
        {
            char* vbo = *(char**)self - 0x10;
            vc = (char*)self - vbo;
        }
        ctorVtbl9.e[1].delta = D_004886E0.e[1].delta + vc;
        ctorVtbl22 = D_00488728;
        *(void**)(*(char**)self + 0x6C0) = &ctorVtbl22;
        ctorVtbl22.e[1].delta = D_00488728.e[1].delta + vc;
    }
    cCheatFX_resetStates(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX__cCheatFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/worldlightman_classes.h"

/**
 * Destroys a rider's cheat-code tracker: restores its vtables, then destroys the virtual cRider base when in
 * charge.
 */
cVbDtor002F6388::~cVbDtor002F6388()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor002F6388; expose it under the address-derived name.
__asm__(".globl cCheatFX__cCheatFX\n.type cCheatFX__cCheatFX,@function\ncCheatFX__cCheatFX = _$_15cVbDtor002F6388\n.size cCheatFX__cCheatFX,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002F6388$13cRiderVbC_wlm = D_004886E0\n"
        "_vt$15cVbDtor002F6388$14cRiderVbBC_wlm = D_00459B90\n"
        "_vt$15cVbDtor002F6388$12cRiderVb_wlm = D_00488728\n"
        "_$_12cRiderVb_wlm = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_reset__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty reset).
 */
void cCheatFX_reset(void* self)
{
}
#endif

extern "C" void* memset(void*, int, int);

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_resetStates__FPv);
#ifdef SKIP_ASM
/**
 * Clears the cheat state (state, step, count and input bits).
 *
 * @return the cleared state's address (the memset result; On Tour's version returns nothing)
 */
void* cCheatFX_resetStates(void* vself)
{
    cCheatFX* self = (cCheatFX*)vself;
    return memset((&self->m_cheatState.m_state), 0, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_update);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: called for effect only; bound by asm label so the call has no value
void cCheatFX_resetStates_v(void* self) __asm__("cCheatFX_resetStates__FPv");
struct sWLCode6910;
extern "C" void cCheatFX_updateState(sWLCode6910* self);
extern int cCheatFX_m_resetStates;

struct sVec6518 {
    float x, y, z, w;
} __attribute__((aligned(16)));
extern sVec6518 t4vect_Origin;

struct sVE6518 {
    short delta;
    short index;
    int (*fn)(void*);
};



// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float len6518(const sVec6518& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec6518 norm6518(const sVec6518& v)
{
    sVec6518 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec6518 scale6518(const sVec6518& v, float s)
{
    sVec6518 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void scaleEq6518(sVec6518& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec6518 add6518(const sVec6518& a, const sVec6518& b)
{
    sVec6518 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}


/**
 * Runs the rider's cheat for a tick: reads the entered inputs and, while the cheat is on, applies its rider
 * overrides.
 *
 * The cheat is forced off in network games. While on, the rider is held in mode 11 and its speed is pulled
 * toward
 * a set maximum and steered toward its facing direction. The closing test (x < 0 and x >= 0) can never be
 * true,
 * so the velocity reset after it is dead code.
 */
extern "C" void cCheatFX_update(cCheatFX* self)
{
    char* sub = (char*)&self->mRider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)sub;
    if (vt[9].fn(vthis(sub, vt[9])) != 0) {
        return;
    }
    if (cCheatFX_m_resetStates >= 2) {
        cCheatFX_resetStates_v(self);
        cCheatFX_m_resetStates--;
    }
    cCheatFX_updateState((sWLCode6910*)self);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        (*(signed char*)&self->m_cheatState) = 0;
    }
    if ((*(signed char*)&self->m_cheatState) == 1) {
        (self->mRider)->mUberLevel = 11;
        (self->mRider)->mAllowNaturalAir = ((*(unsigned char*)&self->m_cheatState.m_misc) >> 1) & 1;
        (self->mRider)->BoostPowerBar = 1.0f;
        (self->mRider)->mUberTimer = 60.0f;
        (self->mRider)->mPowerUpTrickTimer = 60.0f;
        (self->mRider)->mPowerUpBoostTimer = 60.0f;
        (self->mRider)->mBoostMode = 0;
    } else {
        (self->mRider)->mAllowNaturalAir = 1;
        if ((self->mRider)->mUberLevel != 11) {
            return;
        }
        (self->mRider)->mUberLevel = 0;
        (self->mRider)->BoostPowerBar = 0.0f;
        (self->mRider)->mUberTimer = 0.0f;
        (self->mRider)->mPowerUpTrickTimer = 0.0f;
        (self->mRider)->mPowerUpBoostTimer = 0.0f;
        (self->mRider)->mBoostMode = 1;
    }
    cRider* curRider = (self->mRider);
    if (curRider->mUberLevel != 11) {
        return;
    }
    if (!(0.0f < curRider->mBoostStrength)) {
        return;
    }
    float len = len6518((*(sVec6518*)&curRider->mVelocity));
    if (len < curRider->mBoostMaxSpeed && len != 0.0f) {
        (*(sVec6518*)&curRider->mVelocity) = norm6518((*(sVec6518*)&curRider->mVelocity));
        scaleEq6518((*(sVec6518*)&(self->mRider)->mVelocity), (self->mRider)->mBoostMaxSpeed * (self->mRider)->mCheatSpeedLerp + len * (1.0f - (self->mRider)->mCheatSpeedLerp));
        len = len6518((*(sVec6518*)&(self->mRider)->mVelocity));
    }
    curRider = (self->mRider);
    if (curRider->mCheatSteerAlign == 0.0f) {
        return;
    }
    if (!(0.0010000000474974513f < len6518((*(sVec6518*)&curRider->mGroundForward)))) {
        return;
    }
    if (!(9.999999747378752e-05f < len6518((*(sVec6518*)&curRider->mVelocity)))) {
        return;
    }
    (*(sVec6518*)&curRider->mVelocity) = add6518(scale6518(norm6518((*(sVec6518*)&curRider->mGroundForward)), len * (self->mRider)->mCheatSteerAlign),
                     scale6518(norm6518((*(sVec6518*)&(self->mRider)->mVelocity)), len * (1.0f - (self->mRider)->mCheatSteerAlign)));
    curRider = (self->mRider);
    if ((*(sVec6518*)&curRider->mVelocity).x < 0.0f && (*(sVec6518*)&curRider->mVelocity).x >= 0.0f) {
        (*(sVec6518*)&curRider->mVelocity) = t4vect_Origin;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_render__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty render).
 */
void cCheatFX_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_updateState);
#ifdef SKIP_ASM
struct sWLCode6910 {
    char pad0[4];
    signed char state;          // 0x4
    unsigned char step;         // 0x5
    signed char count;          // 0x6
    unsigned char flags;        // 0x7
};

extern signed char cCheatFX_m_cheatStateEnterCounts[];
extern int cCheatFX_m_resetStates;
extern "C" void cCheatFX_clearInput(cCheatFX* self);
extern "C" int cCheatFX_getInput(cCheatFX* self);

/**
 * Advances a cheat effect's code state machine (m_cheatState) by this frame's cheat input, then clears the input.
 *
 * The input is cCheatFX_getInput's bits 0x10, 0x20 and 0x40 of m_misc (set by cCheatFX_setCheatInput), cleared by
 * cCheatFX_clearInput. With no input it clears the held bit 0 of m_misc; while bit 0 is set further input is ignored, so each
 * press counts once. State -1 (off): 0x40 starts the code (state, index, count and misc 0). State 0: 0x10 counts up
 * m_count; 0x20 compares m_count with cCheatFX_m_cheatStateEnterCounts[m_index] and either advances m_index (after 9 steps state 1) or drops
 * back to -1. State 1: exactly 0x60 sets cCheatFX_m_resetStates to 8; otherwise 0x40 toggles bit 1 of m_misc. cCheatFX_update calls
 * it.
 */
extern "C" void cCheatFX_updateState(sWLCode6910* self)
{
    int cheatInput = cCheatFX_getInput((cCheatFX*)self);
    cCheatFX_clearInput((cCheatFX*)self);
    if (cheatInput == 0) {
        self->flags &= ~1;
        return;
    }
    if (self->flags & 1) {
        return;
    }
    self->flags |= 1;
    switch (self->state) {
    case -1:
        if (cheatInput & 0x40) {
            self->state = 0;
            self->count = 0;
            self->step = 0;
            self->flags = 0;
        }
        break;
    case 0:
        if (cheatInput & 0x10) {
            self->count++;
        } else if (cheatInput & 0x20) {
            if (self->count == cCheatFX_m_cheatStateEnterCounts[(signed char)self->step]) {
                self->count = 0;
                self->step++;
                if (self->step == 9) {
                    self->step = 0;
                    self->count = 0;
                    self->state = 1;
                }
            } else {
                self->state = -1;
            }
        }
        break;
    case 1:
        if (cheatInput == 0x60) {
            cCheatFX_m_resetStates = 8;
        } else if (cheatInput & 0x40) {
            self->flags ^= 2;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_writeToReplayFrame);
#ifdef SKIP_ASM
class cCheatFX_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual int write(void* dst, int size);
    virtual int read(void* src, int size);
};

/**
 * Writes the cheat state (4 bytes) to a replay frame.
 *
 * @param stream  the replay frame (On Tour cReplayFramePtr* data)
 */
extern "C" void cCheatFX_writeToReplayFrame(cCheatFX* self, cCheatFX_writeToReplayFrame_cObj* stream)
{
    stream->write((char*)&self->m_cheatState, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads the cheat state (4 bytes) back from a replay frame.
 *
 * @param stream  the replay frame (On Tour cReplayFramePtr* data)
 */
extern "C" void cCheatFX_readFromReplayFrame(cCheatFX* self, cCheatFX_writeToReplayFrame_cObj* stream)
{
    stream->read((char*)&self->m_cheatState, 4);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_setCheatInput);
#ifdef SKIP_ASM
/**
 * Records one of the three cheat-sequence inputs (bits 0x10, 0x20, 0x40 of m_misc) for the next update.
 *
 * The rider code sends them: inputs 0 and 1 from two trick paths, input 2 from the handplant code. Nine
 * counted
 * steps that match cCheatFX_m_cheatStateEnterCounts turn the cheat on.
 *
 * @param type  the input, 0 to 2 (On Tour flag)
 */
extern "C" void cCheatFX_setCheatInput(cCheatFX* self, int type)
{
    switch (type) {
    case 0:
        *(unsigned char*)&self->m_cheatState.m_misc |= 0x10;
        break;
    case 1:
        *(unsigned char*)&self->m_cheatState.m_misc |= 0x20;
        break;
    case 2:
        *(unsigned char*)&self->m_cheatState.m_misc |= 0x40;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_clearInput);
#ifdef SKIP_ASM
/**
 * Clears a cheat effect's pending cheat input (bits 0x10, 0x20, 0x40 of m_cheatState.m_misc).
 *
 * cCheatFX_updateState calls it after reading the input.
 */
extern "C" void cCheatFX_clearInput(cCheatFX* self)
{
    unsigned char* flagByte = (unsigned char*)(&self->m_cheatState.m_misc);
    *flagByte &= 0x8f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCheatFX_getInput);
#ifdef SKIP_ASM
/**
 * Returns a cheat effect's pending cheat input: bits 0x10, 0x20 and 0x40 of m_cheatState.m_misc.
 *
 * cCheatFX_updateState reads it.
 *
 * @return the input bits
 */
extern "C" int cCheatFX_getInput(cCheatFX* self)
{
    return *(unsigned char*)&self->m_cheatState.m_misc & 0x70;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B48);
#ifdef SKIP_ASM
/**
 * Returns @p self unchanged.
 *
 * The definition is variadic, so it first spills the argument registers. Nothing in the retail code references it.
 *
 * @return @p self
 */
extern "C" void* func_002F6B48(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F6B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_004D57B0[4];
extern float D_004D57C0[4];
extern float D_004D57D0[4];
extern float D_004D57E0[4];
extern float D_004D57F0[4];
extern float D_004D5800[4];
extern float D_004D5810[4];
extern float D_004D5820[4];
extern float D_004D5830[4];
extern float D_004D5840[4];
extern float D_004D5850[4];
extern float D_004D5860[4];
extern float D_004D5870[4];
extern float D_004D5880[4];
extern float D_004D5890[4];
extern float D_004D58A0[4];
extern float D_004D58B0[4];
extern float D_004D58C0[4];
extern float D_004D58D0[4];
extern float D_004D58E0[4];
extern float D_004D58F0[4];
extern float D_004D5900[4];
extern float D_004D5910[4];
extern float D_004D5920[4];
extern float D_004D5930[4];
extern float D_004D5940[4];
extern float D_004D5950[4];
extern float D_004D5960[4];
extern float D_004D5970[4];
extern float D_004D5980[4];
extern float D_004D5990[4];
extern float D_004D59A0[4];
extern float D_004D59B0[4];
extern float D_004D59C0[4];
extern float D_004D59D0[4];
extern float D_004D59E0[4];
extern float D_004D59F0[4];
extern float D_004D5A00[4];
extern float D_004D5A10[4];
extern float D_004D5A20[4];
extern float D_004D5A30[4];
extern float D_004D5A40[4];
extern float D_004D5A50[4];
extern float D_004D5A60[4];
extern float D_004D5A70[4];
extern float D_004D5A80[4];
extern float D_004D5A90[4];
extern float D_004D5AA0[4];
extern float D_004D5AB0[4];
extern float D_004D5AC0[4];
extern float D_004D5AD0[4];
extern float D_004D5AE0[4];
extern float D_004D5AF0[4];
extern float D_004D5B00[4];
extern float D_004D5B10[4];
extern float D_004D5B20[4];
extern float D_004D5B30[4];
extern float D_004D5B40[4];
extern float D_004D5B50[4];
extern float D_004D5B60[4];
extern float D_004FAAF0[4];
extern float D_004FAB00[4];
extern float D_004FAB10[4];
extern float D_004FAB20[4];
extern float D_004FAB30[4];
extern float D_004FAB40[4];
extern float D_004FAB50[4];
extern float D_004FAB60[4];
extern float D_004FAB70[4];
extern float D_004FAB80[4];
extern float D_004FAB90[4];
extern float D_004FABA0[4];
extern float D_004FABB0[4];
extern float D_004FABC0[4];
extern float D_004FABD0[4];
extern float D_004FABE0[4];
extern float D_004FABF0[4];
extern float D_004FAC00[4];
extern float D_004FAC10[4];
extern float D_004FAC20[4];
extern float D_004FAC30[4];
extern float D_004FAC40[4];
extern float D_004FAC50[4];
extern float D_004FAC60[4];
extern float D_004FAC70[4];
extern float D_004FAC80[4];
extern int D_004A5530;
extern int D_004A5538;
extern int D_004A553C;
extern int kWPR_MusicTrigger[2];
extern int kWPR_Mix[2];
extern int kWPR_Ambience[2];
extern int kWPR_Speech[2];
extern int kWPR_Camera[2];
extern int kWPR_Fog[2];
extern int kWPR_LightGlow[2];
extern int kWPR_ScreenTint[2];
extern int kWPR_SkyBox[2];
extern int kWPR_Sun[2];
extern int kWPR_Surface[2];
extern int kWPR_Lighting[2];
extern int kWPR_Weather[2];
extern int kWPR_Danger[2];
extern float D_004A55AC;
extern int cRenderStateMan_m_customIrradiance;
extern int D_004A55B4;
extern float D_004A55B8;
extern float D_004A55BC;
struct sWlV3 {
    float x, y, z;
    sWlV3() {}
    sWlV3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};
static inline void setCol(float* c, float a, const sWlV3& v)
{
    c[0] = a;
    c[1] = v.x;
    c[2] = v.y;
    c[3] = v.z;
}
struct sWlAvElem {
    char pad[0x30];
    sWlAvElem() {}
};
extern "C" void* cDynamicColourEmitter_cDynamicColourEmitter(void* self);
extern "C" void tActiveAvalancheNode__tActiveAvalancheNode(void* self, int flags);
struct sWlFx {
    char pad[0xD0];
    char emitter[0x2F0 - 0xD0];
    sWlFx() { cDynamicColourEmitter_cDynamicColourEmitter(emitter); }
};
struct sWlRsm {
    char pad[0xF0];
    sWlRsm() {}
};
inline void* operator new[](unsigned int, void* p) { return p; }
extern sWlAvElem vertlist[];
extern sWlAvElem collisionvertlist[];
extern sWlFx cAvalanche_m_activeNodes[];
// one past the end of cAvalanche_m_activeNodes[64] (the linker names that address cRenderStateMan_m_state)
extern sWlFx cRenderStateMan_m_state_end[] __asm__("cRenderStateMan_m_state");
extern sWlRsm cRenderStateMan_m_state[];
static inline void initE(int* p, int n)
{
    *(char*)p = n;
    *p = (*p & 0xFF) | (n << 8);
}
// Static constructors/destructors of this lump: the colour table, the avalanche particle
// arrays and fx objects (vertlist, collisionvertlist, cAvalanche_m_activeNodes), the render-state
// entries (cRenderStateMan_m_state) and the light colour presets.
void sti_002F6B88(int init, int prio) __asm__("func_002F6B88");
/**
 * Runs the static constructors (@p init nonzero) or destructors (@p init 0) of this visual-effects unit's globals.
 *
 * Acts only for @p prio 0xFFFF. Construction fills the colour preset table D_004D57B0..D_004D5B60 (a, r, g, b), sets
 * the region type constants kWPR_MusicTrigger..kWPR_Danger to 0..13, constructs vertlist (2048), collisionvertlist (64)
 * and cAvalanche_m_activeNodes (64, each with a cDynamicColourEmitter at +0xD0), sets D_004A55AC to 2 pi, constructs
 * the 8 cRenderStateMan_m_state rows, clears the custom irradiance name cRenderStateMan_m_customIrradiance, copies presets into the colours
 * D_004FAAF0..D_004FAC80 and sets D_004A55B8, D_004A55BC to 320, 240. Destruction runs
 * tActiveAvalancheNode__tActiveAvalancheNode on the 64 active avalanche nodes, last first. func_002F8370 and
 * func_002F8390 call it.
 *
 * @param init  nonzero to construct, 0 to destroy
 * @param prio  the init priority; only 0xFFFF does anything
 */
void sti_002F6B88(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init) {
            D_004D57B0[0] = 1.0f;
            D_004D57B0[1] = 1.0f;
            D_004D57B0[2] = 1.0f;
            D_004D57B0[3] = 0.5f;
            D_004D57C0[0] = 0.4998680055141449f;
            D_004D57C0[1] = 1.0f;
            D_004D57C0[2] = 1.0f;
            D_004D57C0[3] = 0.0f;
            D_004D57D0[0] = 0.12951700389385223f;
            D_004D57D0[1] = 1.0f;
            D_004D57D0[2] = 1.0f;
            D_004D57D0[3] = 0.0f;
            D_004D57E0[0] = 0.22426900267601013f;
            D_004D57E0[1] = 0.9402310252189636f;
            D_004D57E0[2] = 0.5425059795379639f;
            D_004D57E0[3] = 0.0004030000127386302f;
            D_004D57F0[0] = 0.4652079939842224f;
            D_004D57F0[1] = 0.32170501351356506f;
            D_004D57F0[2] = 0.9943439960479736f;
            D_004D57F0[3] = 0.0f;
            D_004D5800[0] = 0.17792600393295288f;
            D_004D5800[1] = 1.0f;
            D_004D5800[2] = 1.0f;
            D_004D5800[3] = 0.0f;
            D_004D5810[0] = 0.4411740005016327f;
            D_004D5810[1] = 0.41388601064682007f;
            D_004D5810[2] = 0.3211260139942169f;
            D_004D5810[3] = 0.4398239850997925f;
            D_004D5820[0] = 0.500997006893158f;
            D_004D5820[1] = 0.24849599599838257f;
            D_004D5820[2] = 1.0f;
            D_004D5820[3] = 1.0f;
            D_004D5830[0] = 0.27687299251556396f;
            D_004D5830[1] = 0.6395620107650757f;
            D_004D5830[2] = 0.13423900306224823f;
            D_004D5830[3] = 0.0f;
            D_004D5840[0] = 0.8936439752578735f;
            D_004D5840[1] = 0.5263469815254211f;
            D_004D5840[2] = 0.5051450133323669f;
            D_004D5840[3] = 0.8687300086021423f;
            D_004D5850[0] = 1.0f;
            D_004D5850[1] = 1.0f;
            D_004D5850[2] = 1.0f;
            D_004D5850[3] = 1.0f;
            D_004D5860[0] = 1.0f;
            D_004D5860[1] = 0.0f;
            D_004D5860[2] = 0.0f;
            D_004D5860[3] = 0.0f;
            D_004D5870[0] = 1.0f;
            D_004D5870[1] = 0.550000011920929f;
            D_004D5870[2] = 0.5350000262260437f;
            D_004D5870[3] = 0.550000011920929f;
            D_004D5880[0] = 1.0f;
            D_004D5880[1] = 1.0f;
            D_004D5880[2] = 1.0f;
            D_004D5880[3] = 1.0f;
            D_004D5890[0] = 1.0f;
            D_004D5890[1] = 1.0f;
            D_004D5890[2] = 1.0f;
            D_004D5890[3] = 1.0f;
            D_004D58A0[0] = 1.0f;
            D_004D58A0[1] = 0.0f;
            D_004D58A0[2] = 0.0f;
            D_004D58A0[3] = 0.0f;
            D_004D58B0[0] = 1.0f;
            D_004D58B0[1] = 0.0f;
            D_004D58B0[2] = 0.0f;
            D_004D58B0[3] = 0.0f;
            D_004D58C0[0] = 1.0f;
            D_004D58C0[1] = 1.0f;
            D_004D58C0[2] = 1.0f;
            D_004D58C0[3] = 1.0f;
            D_004D58D0[0] = 1.0f;
            D_004D58D0[1] = 0.5f;
            D_004D58D0[2] = 1.0f;
            D_004D58D0[3] = 0.5f;
            D_004D58E0[0] = 1.0f;
            D_004D58E0[1] = 0.5f;
            D_004D58E0[2] = 1.0f;
            D_004D58E0[3] = 0.5f;
            D_004D58F0[0] = 1.0f;
            D_004D58F0[1] = 0.5f;
            D_004D58F0[2] = 1.0f;
            D_004D58F0[3] = 0.5f;
            D_004D5900[0] = 1.0f;
            D_004D5900[1] = 0.7828119993209839f;
            D_004D5900[2] = 0.06171900033950806f;
            D_004D5900[3] = 0.03984399884939194f;
            D_004D5910[0] = 1.0f;
            D_004D5910[1] = 1.0f;
            D_004D5910[2] = 0.7988259792327881f;
            D_004D5910[3] = 0.0f;
            D_004D5920[0] = 0.9886710047721863f;
            D_004D5920[1] = 0.9230499863624573f;
            D_004D5920[2] = 0.34311699867248535f;
            D_004D5920[3] = 0.0f;
            D_004D5930[0] = 1.0f;
            D_004D5930[1] = 1.0f;
            D_004D5930[2] = 1.0f;
            D_004D5930[3] = 1.0f;
            D_004D5940[0] = 1.0f;
            D_004D5940[1] = 1.0f;
            D_004D5940[2] = 0.7976430058479309f;
            D_004D5940[3] = 0.0f;
            D_004D5950[0] = 1.0f;
            D_004D5950[1] = 0.9222670197486877f;
            D_004D5950[2] = 0.34179699420928955f;
            D_004D5950[3] = 0.0f;
            D_004D5960[0] = 1.0f;
            D_004D5960[1] = 0.7847639918327332f;
            D_004D5960[2] = 0.06054700165987015f;
            D_004D5960[3] = 0.04062499850988388f;
            D_004D5970[0] = 1.0f;
            D_004D5970[1] = 0.800000011920929f;
            D_004D5970[2] = 0.44999998807907104f;
            D_004D5970[3] = 1.0f;
            D_004D5980[0] = 1.0f;
            D_004D5980[1] = 0.3281260132789612f;
            D_004D5980[2] = 1.0f;
            D_004D5980[3] = 0.3539060056209564f;
            D_004D5990[0] = 1.0f;
            D_004D5990[1] = 1.0f;
            D_004D5990[2] = 1.0f;
            D_004D5990[3] = 1.0f;
            D_004D59A0[0] = 1.0f;
            D_004D59A0[1] = 1.0f;
            D_004D59A0[2] = 1.0f;
            D_004D59A0[3] = 1.0f;
            D_004D59B0[0] = 1.0f;
            D_004D59B0[1] = 0.8613290190696716f;
            D_004D59B0[2] = 0.38124799728393555f;
            D_004D59B0[3] = 0.0f;
            D_004D59C0[0] = 1.0f;
            D_004D59C0[1] = 1.0f;
            D_004D59C0[2] = 1.0f;
            D_004D59C0[3] = 1.0f;
            D_004D59D0[0] = 1.0f;
            D_004D59D0[1] = 0.3244380056858063f;
            D_004D59D0[2] = 1.0f;
            D_004D59D0[3] = 0.0f;
            D_004D59E0[0] = 1.0f;
            D_004D59E0[1] = 1.0f;
            D_004D59E0[2] = 1.0f;
            D_004D59E0[3] = 1.0f;
            D_004D59F0[0] = 1.0f;
            D_004D59F0[1] = 1.0f;
            D_004D59F0[2] = 0.0f;
            D_004D59F0[3] = 0.0f;
            D_004D5A00[0] = 0.7007060050964355f;
            D_004D5A00[1] = 0.0f;
            D_004D5A00[2] = 0.0f;
            D_004D5A00[3] = 0.0f;
            D_004D5A10[0] = 1.0f;
            D_004D5A10[1] = 0.7828530073165894f;
            D_004D5A10[2] = 0.06140600144863129f;
            D_004D5A10[3] = 0.04146299883723259f;
            D_004D5A20[0] = 1.0f;
            D_004D5A20[1] = 0.32487601041793823f;
            D_004D5A20[2] = 1.0f;
            D_004D5A20[3] = 0.35280099511146545f;
            D_004D5A30[0] = 1.0f;
            D_004D5A30[1] = 1.0f;
            D_004D5A30[2] = 0.7977359890937805f;
            D_004D5A30[3] = 0.0f;
            D_004D5A40[0] = 1.0f;
            D_004D5A40[1] = 0.4937489926815033f;
            D_004D5A40[2] = 1.0f;
            D_004D5A40[3] = 0.49179700016975403f;
            D_004D5A50[0] = 1.0f;
            D_004D5A50[1] = 1.0f;
            D_004D5A50[2] = 1.0f;
            D_004D5A50[3] = 1.0f;
            D_004D5A60[0] = 1.0f;
            D_004D5A60[1] = 0.800000011920929f;
            D_004D5A60[2] = 1.0f;
            D_004D5A60[3] = 0.800000011920929f;
            D_004D5A70[0] = 1.0f;
            D_004D5A70[1] = 1.0f;
            D_004D5A70[2] = 0.0f;
            D_004D5A70[3] = 0.0f;
            D_004D5A80[0] = 1.0f;
            D_004D5A80[1] = 1.0f;
            D_004D5A80[2] = 0.0f;
            D_004D5A80[3] = 0.0f;
            D_004D5A90[0] = 1.0f;
            D_004D5A90[1] = 1.0f;
            D_004D5A90[2] = 0.0f;
            D_004D5A90[3] = 0.0f;
            D_004D5AA0[0] = 1.0f;
            D_004D5AA0[1] = 0.3273420035839081f;
            D_004D5AA0[2] = 1.0f;
            D_004D5AA0[3] = 0.35390499234199524f;
            D_004D5AB0[0] = 1.0f;
            D_004D5AB0[1] = 1.0f;
            D_004D5AB0[2] = 0.061000000685453415f;
            D_004D5AB0[3] = 0.039000000804662704f;
            D_004D5AC0[0] = 1.0f;
            D_004D5AC0[1] = 1.0f;
            D_004D5AC0[2] = 0.7999979853630066f;
            D_004D5AC0[3] = 0.0f;
            D_004D5AD0[0] = 1.0f;
            D_004D5AD0[1] = 0.9589840173721313f;
            D_004D5AD0[2] = 0.6143789887428284f;
            D_004D5AD0[3] = 0.0f;
            D_004D5AE0[0] = 1.0f;
            D_004D5AE0[1] = 0.9285150170326233f;
            D_004D5AE0[2] = 0.3399839997291565f;
            D_004D5AE0[3] = 0.0f;
            D_004D5AF0[0] = 1.0f;
            D_004D5AF0[1] = 0.8738250136375427f;
            D_004D5AF0[2] = 0.24931700527668f;
            D_004D5AF0[3] = 0.017578000202775f;
            D_004D5B00[0] = 1.0f;
            D_004D5B00[1] = 0.7843739986419678f;
            D_004D5B00[2] = 0.06132800132036209f;
            D_004D5B00[3] = 0.03945299983024597f;
            D_004D5B10[0] = 1.0f;
            D_004D5B10[1] = 0.0f;
            D_004D5B10[2] = 1.0f;
            D_004D5B10[3] = 0.0f;
            D_004D5B20[0] = 1.0f;
            D_004D5B20[1] = 1.0f;
            D_004D5B20[2] = 0.0f;
            D_004D5B20[3] = 0.0f;
            *(char*)&D_004A5530 = -1;
            D_004A5530 = (D_004A5530 & 0xFF) | 0xFFFFFF00u;
            D_004D5B30[0] = 1.0f;
            D_004D5B30[1] = 1.0f;
            D_004D5B30[2] = D_004D57C0[3];
            D_004D5B30[3] = D_004D57C0[3];
            D_004D5B40[0] = 1.0f;
            D_004D5B40[1] = 1.0f;
            D_004D5B40[2] = 0.5f;
            D_004D5B40[3] = D_004D57C0[3];
            D_004D5B50[0] = 1.0f;
            D_004D5B50[1] = 1.0f;
            D_004D5B50[2] = 1.0f;
            D_004D5B50[3] = D_004D57C0[3];
            D_004D5B60[0] = 1.0f;
            D_004D5B60[1] = 1.0f;
            D_004D5B60[2] = 1.0f;
            D_004D5B60[3] = 1.0f;
            D_004A553C = 0;
            D_004A5538 = 0;
            initE(kWPR_MusicTrigger, 0);
            initE(kWPR_Mix, 1);
            initE(kWPR_Ambience, 2);
            initE(kWPR_Speech, 3);
            initE(kWPR_Camera, 4);
            initE(kWPR_Fog, 5);
            initE(kWPR_LightGlow, 6);
            initE(kWPR_ScreenTint, 7);
            initE(kWPR_SkyBox, 8);
            initE(kWPR_Sun, 9);
            initE(kWPR_Surface, 10);
            initE(kWPR_Lighting, 11);
            initE(kWPR_Weather, 12);
            initE(kWPR_Danger, 13);
            new (vertlist) sWlAvElem[2048];
            new (collisionvertlist) sWlAvElem[64];
            new (cAvalanche_m_activeNodes) sWlFx[64];
            D_004A55AC = 6.283186f;
            new (cRenderStateMan_m_state) sWlRsm[8];
            D_004A55B4 = 0;
            cRenderStateMan_m_customIrradiance = 0;
            D_004FAAF0[0] = 1.0f;
            D_004FAAF0[1] = 0.55f;
            D_004FAAF0[2] = 0.535f;
            D_004FAAF0[3] = 0.55f;
            __builtin_memcpy(D_004FAB00, D_004D5870, 16);
            D_004FAB10[0] = 1.0f;
            D_004FAB10[1] = 1.0f;
            D_004FAB10[2] = 1.0f;
            D_004FAB10[3] = 1.0f;
            __builtin_memcpy(D_004FAB20, D_004D5880, 16);
            D_004FAB30[0] = 1.0f;
            D_004FAB30[1] = 1.0f;
            D_004FAB30[2] = 1.0f;
            D_004FAB30[3] = 1.0f;
            __builtin_memcpy(D_004FAB40, D_004D5890, 16);
            D_004A55B8 = 320.0f;
            D_004A55BC = 240.0f;
            setCol(D_004FAB50, 1.0f, sWlV3(1.0f, 1.0f, 0.5f));
            __builtin_memcpy(D_004FAB60, D_004D57B0, 16);
            D_004FAB70[0] = 1.0f;
            D_004FAB70[1] = 1.0f;
            D_004FAB70[2] = 1.0f;
            D_004FAB70[3] = 0.0f;
            __builtin_memcpy(D_004FAB80, D_004D57C0, 16);
            D_004FAB90[0] = 1.0f;
            D_004FAB90[1] = 1.0f;
            D_004FAB90[2] = 1.0f;
            D_004FAB90[3] = 0.0f;
            __builtin_memcpy(D_004FABA0, D_004D57D0, 16);
            D_004FABB0[0] = 1.0f;
            D_004FABB0[1] = 1.0f;
            D_004FABB0[2] = 1.0f;
            D_004FABB0[3] = 0.0f;
            __builtin_memcpy(D_004FABC0, D_004D57E0, 16);
            D_004FABD0[0] = 1.0f;
            D_004FABD0[1] = 1.0f;
            D_004FABD0[2] = 1.0f;
            D_004FABD0[3] = 0.0f;
            __builtin_memcpy(D_004FABE0, D_004D57F0, 16);
            D_004FABF0[0] = 1.0f;
            D_004FABF0[1] = 1.0f;
            D_004FABF0[2] = 1.0f;
            D_004FABF0[3] = 0.0f;
            __builtin_memcpy(D_004FAC00, D_004D5800, 16);
            D_004FAC10[0] = 1.0f;
            D_004FAC10[1] = 1.0f;
            D_004FAC10[2] = 1.0f;
            D_004FAC10[3] = 0.0f;
            __builtin_memcpy(D_004FAC20, D_004D5810, 16);
            D_004FAC30[0] = 1.0f;
            D_004FAC30[1] = 1.0f;
            D_004FAC30[2] = 1.0f;
            D_004FAC30[3] = 0.0f;
            __builtin_memcpy(D_004FAC40, D_004D5820, 16);
            D_004FAC50[0] = 1.0f;
            D_004FAC50[1] = 1.0f;
            D_004FAC50[2] = 1.0f;
            D_004FAC50[3] = 0.0f;
            __builtin_memcpy(D_004FAC60, D_004D5830, 16);
            D_004FAC70[0] = 1.0f;
            D_004FAC70[1] = 1.0f;
            D_004FAC70[2] = 1.0f;
            D_004FAC70[3] = 0.0f;
            __builtin_memcpy(D_004FAC80, D_004D5840, 16);
        } else {
            if (cAvalanche_m_activeNodes != 0) {
                sWlFx* avalancheNode = cRenderStateMan_m_state_end;
                if (cAvalanche_m_activeNodes != cRenderStateMan_m_state_end) {
                    do {
                        avalancheNode--;
                        tActiveAvalancheNode__tActiveAvalancheNode(avalancheNode, 0);
                    } while (avalancheNode != cAvalanche_m_activeNodes);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A50);
#ifdef SKIP_ASM
extern char t4mat_Identity[];

/**
 * Returns the identity matrix t4mat_Identity.
 *
 * Nothing in the retail code references it.
 *
 * @return pointer to t4mat_Identity
 */
extern "C" void* func_002F7A50()
{
    return t4mat_Identity;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7A60);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing in the retail code references it.
 */
extern "C" void func_002F7A60(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", tParticleEmitter_tParticleEmitter__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a particle emitter in place; there is nothing to initialise, so it returns @p self.
 *
 * The cStaticEmitter and cDynamicEmitter constructors run it on their embedded emitter (+0x10, +0x20), and
 * cBoardSparkFX_cBoardSparkFX on a fresh 336-byte "Board_Spark_Emitter" block before tParticleEmitter_randomize.
 *
 * @return @p self
 */
void* tParticleEmitter_tParticleEmitter(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardWakeFX_IsWakeActive);
#ifdef SKIP_ASM
/**
 * Returns whether the int at +0x4 of @p arg0 is greater than 0.
 *
 * Nothing in the retail code references it.
 *
 * @param arg0  the object to test
 * @return 1 when +0x4 > 0, else 0
 */
extern "C" int cBoardWakeFX_IsWakeActive(cBoardWakeFX* arg0) {
    return (arg0->mNumWakeLines) > 0;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardWakeFX_GetChunkySprayVelocity);
#ifdef SKIP_ASM
/**
 * Returns the wake's chunky-spray velocity: the 16-byte vector (lq/sq) mWakeLines[mCurrentWakeLine].mVel[mWakeLineSegs - 1],
 * i.e. the newest segment's velocity row of the current wake line (0x70-byte lines from mWakeLines).
 *
 * The preview's GetChunkySprayVelocity__12cBoardWakeFX has the same body. Nothing in the retail code references this copy.
 *
 * @param out  receives the velocity (the hidden t4Vector result)
 * @param arg1  the board wake effect
 * @return @p out
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cBoardWakeFX_GetChunkySprayVelocity(long long *out, cBoardWakeFX* arg1) {
    *out = *(long long*)&arg1->mWakeLines[arg1->mCurrentWakeLine].mVel[arg1->mWakeLineSegs - 1];
    return out;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardWakeFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cBoardWakeFX_readFromReplayFrame, which forwards to the cBoardWakeFX function cBoardWakeFX_reset.
 * Nothing in the retail code references it.
 */
extern "C" void cBoardWakeFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardWakeFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cBoardWakeFX_reset();

/**
 * Calls cBoardWakeFX_reset, passing its argument registers through untouched.
 *
 * cBoardWakeFX_reset sits between the cBoardWakeFX destructor and update, where On Tour
 * declares reset. Nothing in the retail code references it.
 */
extern "C" void cBoardWakeFX_readFromReplayFrame(void) {
    cBoardWakeFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardSparkFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cBoardSparkFX_readFromReplayFrame, which forwards to the cBoardSparkFX function cBoardSparkFX_reset.
 * Nothing in the retail code references it.
 */
extern "C" void cBoardSparkFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardSparkFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cBoardSparkFX_reset();

/**
 * Calls cBoardSparkFX_reset, passing its argument registers through untouched.
 *
 * cBoardSparkFX_reset sits between the cBoardSparkFX destructor and update, where On
 * Tour declares reset. Nothing in the retail code references it.
 */
extern "C" void cBoardSparkFX_readFromReplayFrame(void) {
    cBoardSparkFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardTrailFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cBoardTrailFX_readFromReplayFrame, which forwards to the cBoardTrailFX function cBoardTrailFX_reset. Nothing in the
 * retail code references it.
 */
extern "C" void cBoardTrailFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardTrailFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cBoardTrailFX_reset();

/**
 * Calls cBoardTrailFX_reset, passing its argument registers through untouched.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cBoardTrailFX_readFromReplayFrame(void) {
    cBoardTrailFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAngerFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cAngerFX_readFromReplayFrame, which forwards to the cAngerFX function func_002D4BE0. Nothing in the retail code
 * references it.
 */
extern "C" void cAngerFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAngerFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void func_002D4BE0();

/**
 * Calls func_002D4BE0, passing its argument registers through untouched.
 *
 * func_002D4BE0 is the cAngerFX function after its constructor and destructor. Nothing in the retail code references
 * it.
 */
extern "C" void cAngerFX_readFromReplayFrame(void) {
    func_002D4BE0();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cPlayerIndicatorFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cPlayerIndicatorFX_readFromReplayFrame, which forwards to the cPlayerIndicatorFX function
 * cPlayerIndicatorFX_reset. Nothing in the retail code references it.
 */
extern "C" void cPlayerIndicatorFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cPlayerIndicatorFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cPlayerIndicatorFX_reset();

/**
 * Calls cPlayerIndicatorFX_reset, passing its argument registers through untouched.
 *
 * cPlayerIndicatorFX_reset sits between the cPlayerIndicatorFX destructor and update,
 * where On Tour declares reset. Nothing in the retail code references it.
 */
extern "C" void cPlayerIndicatorFX_readFromReplayFrame(void) {
    cPlayerIndicatorFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAirStreamerFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cAirStreamerFX_readFromReplayFrame, which forwards to the cAirStreamerFX function cAirStreamerFX_reset.
 * Nothing in the retail code references it.
 */
extern "C" void cAirStreamerFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAirStreamerFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAirStreamerFX_reset();

/**
 * Calls cAirStreamerFX_reset, passing its argument registers through untouched.
 *
 * cAirStreamerFX_reset sits between the cAirStreamerFX destructor and update, where On
 * Tour declares reset. Nothing in the retail code references it.
 */
extern "C" void cAirStreamerFX_readFromReplayFrame(void) {
    cAirStreamerFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cPunchFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cPunchFX_readFromReplayFrame, which forwards to the cPunchFX function cPunchFX_reset. Nothing in the
 * retail code references it.
 */
extern "C" void cPunchFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cPunchFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cPunchFX_reset();

/**
 * Calls cPunchFX_reset, passing its argument registers through untouched.
 *
 * cPunchFX_reset sits between the cPunchFX destructor and update, where On Tour
 * declares reset. Nothing in the retail code references it.
 */
extern "C" void cPunchFX_readFromReplayFrame(void) {
    cPunchFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BC8);
#ifdef SKIP_ASM
/**
 * Returns @p arg0 + 4.
 *
 * Nothing in the retail code references it.
 *
 * @param arg0  the base address
 * @return @p arg0 + 4
 */
extern "C" int func_002F7BC8(int arg0) {
    return arg0 + 4;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cLensFxMan_SetNumViews);
#ifdef SKIP_ASM
/**
 * Sets the lens effect manager's view count (+0x6490).
 *
 * The lens code reads it as the number of views to process. Nothing in the retail code references it.
 *
 * @param lensMan  the lens effect manager
 * @param numViews  the number of views
 */
extern "C" void cLensFxMan_SetNumViews(cLensFxMan* lensMan, int numViews) {
    lensMan->mNumViews = numViews;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cLensFxMan_Lock);
#ifdef SKIP_ASM
/**
 * Sets the lens effect manager's lock flag (+0x6494).
 *
 * cGame_renderLightHalos sets the same flag to 1 while it adds and renders halos and back to 0 after. Nothing in the
 * retail code references it.
 *
 * @param lensMan  the lens effect manager
 * @param locked  the flag
 */
extern "C" void cLensFxMan_Lock(cLensFxMan* lensMan, int locked) {
    lensMan->mbLocked = locked;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F7BE0);
#ifdef SKIP_ASM
/**
 * Returns whether a replay's state (its first word) is 0.
 *
 * cVisualEffectsMan_commonUpdate calls it on the game's Replay: lightning is enabled when the state is 1 or this
 * returns true.
 *
 * @return 1 when the state is 0, else 0
 */
extern "C" int func_002F7BE0(void* self)
{
    return *(unsigned int*)self == 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cVisualEffect__cVisualEffect);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys a visual effect: re-installs the base vtable cVisualEffect_vtbl and frees the object when bit 0 of @p flags is set.
 *
 * It is the destructor slot of cVisualEffect_vtbl, whose next slots are cVisualEffect_update__FPv, cVisualEffect_render__FPv and
 * cVisualEffect_deferredRender__FPv.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cVisualEffect__cVisualEffect(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cVisualEffect_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base visual effect's per-frame update.
 *
 * It is the slot after the destructor in cVisualEffect_vtbl and in the effect vtables built on it (D_00487EB8 and others).
 */
void cVisualEffect_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cVisualEffect_render__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base visual effect's render.
 *
 * It is the slot after cVisualEffect_update__FPv in cVisualEffect_vtbl and in the effect vtables built on it.
 */
void cVisualEffect_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cVisualEffect_deferredRender__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cVisualEffect's deferredRender has an empty body.
 */
void cVisualEffect_deferredRender(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_cAvalanche);
#ifdef SKIP_ASM
extern void* cAvalanche_vtbl[];
extern "C" void* cAvalanche_init(void* self);

/**
 * Constructs an avalanche manager: installs cAvalanche_vtbl and runs cAvalanche_init.
 *
 * Nothing in the retail code references it.
 *
 * @return @p self
 */
extern "C" void* cAvalanche_cAvalanche(cAvalanche* self)
{
    *(void***)self = cAvalanche_vtbl;
    cAvalanche_init(self);
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Enable);
#ifdef SKIP_ASM
extern int cAvalanche_m_Enable;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Enable.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Enable
 */
extern "C" int *cAvalanche_Enable(void) {
    return &cAvalanche_m_Enable;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Recalc);
#ifdef SKIP_ASM
extern int cAvalanche_m_Recalc;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Recalc.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Recalc
 */
extern "C" int *cAvalanche_Recalc(void) {
    return &cAvalanche_m_Recalc;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Brightness);
#ifdef SKIP_ASM
extern int cAvalanche_m_Brightness;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Brightness (the word after cAvalanche_m_Recalc).
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Brightness
 */
extern "C" int *cAvalanche_Brightness(void) {
    return &cAvalanche_m_Brightness;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_UseLightMapColour);
#ifdef SKIP_ASM
extern int cAvalanche_m_UseLightMapColour;

/**
 * Returns the address of the avalanche setting cAvalanche_m_UseLightMapColour.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_UseLightMapColour
 */
extern "C" int *cAvalanche_UseLightMapColour(void) {
    return &cAvalanche_m_UseLightMapColour;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_UseEndColour);
#ifdef SKIP_ASM
extern int cAvalanche_m_UseEndColour;

/**
 * Returns the address of the avalanche setting cAvalanche_m_UseEndColour.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_UseEndColour
 */
extern "C" int *cAvalanche_UseEndColour(void) {
    return &cAvalanche_m_UseEndColour;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_SquareLerp);
#ifdef SKIP_ASM
extern int cAvalanche_m_SquareLerp;

/**
 * Returns the address of the avalanche setting cAvalanche_m_SquareLerp.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_SquareLerp
 */
extern "C" int *cAvalanche_SquareLerp(void) {
    return &cAvalanche_m_SquareLerp;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Save);
#ifdef SKIP_ASM
extern int cAvalanche_m_Save;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Save.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Save
 */
extern "C" int *cAvalanche_Save(void) {
    return &cAvalanche_m_Save;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Load);
#ifdef SKIP_ASM
extern int cAvalanche_m_Load;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Load.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Load
 */
extern "C" int *cAvalanche_Load(void) {
    return &cAvalanche_m_Load;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_MinImpactVelocity);
#ifdef SKIP_ASM
extern int cAvalanche_m_MinImpactVelocity;

/**
 * Returns the address of the avalanche setting cAvalanche_m_MinImpactVelocity.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_MinImpactVelocity
 */
extern "C" int *cAvalanche_MinImpactVelocity(void) {
    return &cAvalanche_m_MinImpactVelocity;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RenderEmitters);
#ifdef SKIP_ASM
extern int cAvalanche_m_RenderEmitters;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RenderEmitters.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RenderEmitters
 */
extern "C" int *cAvalanche_RenderEmitters(void) {
    return &cAvalanche_m_RenderEmitters;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RenderLines);
#ifdef SKIP_ASM
extern int cAvalanche_m_RenderLines;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RenderLines (the word before cAvalanche_m_Trigger).
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RenderLines
 */
extern "C" int *cAvalanche_RenderLines(void) {
    return &cAvalanche_m_RenderLines;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Trigger);
#ifdef SKIP_ASM
extern int cAvalanche_m_Trigger;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Trigger.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Trigger
 */
extern "C" int *cAvalanche_Trigger(void) {
    return &cAvalanche_m_Trigger;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Update);
#ifdef SKIP_ASM
extern int cAvalanche_m_Update;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Update.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Update
 */
extern "C" int *cAvalanche_Update(void) {
    return &cAvalanche_m_Update;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_UpdateTimeStep);
#ifdef SKIP_ASM
extern int cAvalanche_m_UpdateTimeStep;

/**
 * Returns the address of the avalanche setting cAvalanche_m_UpdateTimeStep.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_UpdateTimeStep
 */
extern "C" int *cAvalanche_UpdateTimeStep(void) {
    return &cAvalanche_m_UpdateTimeStep;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RotationalVelocityScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_RotationalVelocityScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RotationalVelocityScale.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RotationalVelocityScale
 */
extern "C" int *cAvalanche_RotationalVelocityScale(void) {
    return &cAvalanche_m_RotationalVelocityScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RandomBounceChance);
#ifdef SKIP_ASM
extern int cAvalanche_m_RandomBounceChance;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RandomBounceChance.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RandomBounceChance
 */
extern "C" int *cAvalanche_RandomBounceChance(void) {
    return &cAvalanche_m_RandomBounceChance;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RandomBounceVelocityScaleXY);
#ifdef SKIP_ASM
extern int cAvalanche_m_RandomBounceVelocityScaleXY;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RandomBounceVelocityScaleXY.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RandomBounceVelocityScaleXY
 */
extern "C" int *cAvalanche_RandomBounceVelocityScaleXY(void) {
    return &cAvalanche_m_RandomBounceVelocityScaleXY;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RandomBounceVelocityScaleZ);
#ifdef SKIP_ASM
extern int cAvalanche_m_RandomBounceVelocityScaleZ;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RandomBounceVelocityScaleZ.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RandomBounceVelocityScaleZ
 */
extern "C" int *cAvalanche_RandomBounceVelocityScaleZ(void) {
    return &cAvalanche_m_RandomBounceVelocityScaleZ;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_RandomBounceVelocityScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_RandomBounceVelocityScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_RandomBounceVelocityScale.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_RandomBounceVelocityScale
 */
extern "C" int *cAvalanche_RandomBounceVelocityScale(void) {
    return &cAvalanche_m_RandomBounceVelocityScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_AvalancheId);
#ifdef SKIP_ASM
extern int cAvalanche_m_AvalancheId;

/**
 * Returns the address of the avalanche setting cAvalanche_m_AvalancheId.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_AvalancheId
 */
extern "C" int *cAvalanche_AvalancheId(void) {
    return &cAvalanche_m_AvalancheId;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Gravity);
#ifdef SKIP_ASM
extern int cAvalanche_m_Gravity;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Gravity.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Gravity
 */
extern "C" int *cAvalanche_Gravity(void) {
    return &cAvalanche_m_Gravity;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_VelocityDamping);
#ifdef SKIP_ASM
extern int cAvalanche_m_VelocityDamping;

/**
 * Returns the address of the avalanche setting cAvalanche_m_VelocityDamping (the word between cAvalanche_m_Gravity and
 * cAvalanche_m_NormalVelocityScale).
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_VelocityDamping
 */
extern "C" int *cAvalanche_VelocityDamping(void) {
    return &cAvalanche_m_VelocityDamping;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_NormalVelocityScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_NormalVelocityScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_NormalVelocityScale.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_NormalVelocityScale
 */
extern "C" int *cAvalanche_NormalVelocityScale(void) {
    return &cAvalanche_m_NormalVelocityScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_ForwardVelocityScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_ForwardVelocityScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_ForwardVelocityScale (the word after cAvalanche_m_NormalVelocityScale).
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_ForwardVelocityScale
 */
extern "C" int *cAvalanche_ForwardVelocityScale(void) {
    return &cAvalanche_m_ForwardVelocityScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_NormalToForwardScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_NormalToForwardScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_NormalToForwardScale.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_NormalToForwardScale
 */
extern "C" int *cAvalanche_NormalToForwardScale(void) {
    return &cAvalanche_m_NormalToForwardScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_MaxVelocity);
#ifdef SKIP_ASM
extern int cAvalanche_m_MaxVelocity;

/**
 * Returns the address of the avalanche setting cAvalanche_m_MaxVelocity.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_MaxVelocity
 */
extern "C" int *cAvalanche_MaxVelocity(void) {
    return &cAvalanche_m_MaxVelocity;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Damping);
#ifdef SKIP_ASM
extern int cAvalanche_m_Damping;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Damping.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Damping
 */
extern "C" int *cAvalanche_Damping(void) {
    return &cAvalanche_m_Damping;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_NumParticles);
#ifdef SKIP_ASM
extern int cAvalanche_m_NumParticles;

/**
 * Returns the address of the avalanche setting cAvalanche_m_NumParticles.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_NumParticles
 */
extern "C" int *cAvalanche_NumParticles(void) {
    return &cAvalanche_m_NumParticles;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_DefaultSize);
#ifdef SKIP_ASM
extern int cAvalanche_m_DefaultSize;

/**
 * Returns the address of the avalanche setting cAvalanche_m_DefaultSize.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_DefaultSize
 */
extern "C" int *cAvalanche_DefaultSize(void) {
    return &cAvalanche_m_DefaultSize;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_PatchColourScale);
#ifdef SKIP_ASM
extern int cAvalanche_m_PatchColourScale;

/**
 * Returns the address of the avalanche setting cAvalanche_m_PatchColourScale.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_PatchColourScale
 */
extern "C" int *cAvalanche_PatchColourScale(void) {
    return &cAvalanche_m_PatchColourScale;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_PatchColourScale2);
#ifdef SKIP_ASM
extern int cAvalanche_m_PatchColourScale2;

/**
 * Returns the address of the avalanche setting cAvalanche_m_PatchColourScale2.
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_PatchColourScale2
 */
extern "C" int *cAvalanche_PatchColourScale2(void) {
    return &cAvalanche_m_PatchColourScale2;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_Seed);
#ifdef SKIP_ASM
extern int cAvalanche_m_Seed;

/**
 * Returns the address of the avalanche setting cAvalanche_m_Seed (the word before cAvalanche_m_avalancheList).
 *
 * The C definition takes no arguments; the accessor's this register is ignored. Nothing in the retail code references
 * it.
 *
 * @return pointer to cAvalanche_m_Seed
 */
extern "C" int *cAvalanche_Seed(void) {
    return &cAvalanche_m_Seed;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_unresolve);
#ifdef SKIP_ASM
extern "C" void cAvalanche_reset();

/**
 * Calls cAvalanche_reset, passing its argument registers through untouched.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cAvalanche_unresolve(void) {
    cAvalanche_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_fileDataDeleteable);
#ifdef SKIP_ASM
extern int cAvalanche_m_fileDataDeleteable;

/**
 * Returns cAvalanche_m_fileDataDeleteable.
 *
 * Nothing in the retail code references it.
 *
 * @return the flag
 */
extern "C" int cAvalanche_fileDataDeleteable(void) {
    return cAvalanche_m_fileDataDeleteable;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAvalanche_isBadCookie);
#ifdef SKIP_ASM
extern int cAvalanche_m_badCookie;

/**
 * Returns cAvalanche_m_badCookie.
 *
 * Nothing in the retail code references it.
 *
 * @return the flag
 */
extern "C" int cAvalanche_isBadCookie(void) {
    return cAvalanche_m_badCookie;
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cSpeedBoostFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cSpeedBoostFX_readFromReplayFrame, which forwards to the cSpeedBoostFX function cSpeedBoostFX_reset.
 * Nothing in the retail code references it.
 */
extern "C" void cSpeedBoostFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cSpeedBoostFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cSpeedBoostFX_reset();

/**
 * Calls cSpeedBoostFX_reset, passing its argument registers through untouched.
 *
 * cSpeedBoostFX_reset sits between the cSpeedBoostFX destructor and update, where On
 * Tour declares reset. Nothing in the retail code references it.
 */
extern "C" void cSpeedBoostFX_readFromReplayFrame(void) {
    cSpeedBoostFX_reset();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cTrickBoostFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It is paired with cTrickBoostFX_readFromReplayFrame, which forwards to the cTrickBoostFX function cTrickBoostFX_reset__FPv.
 * Nothing in the retail code references it.
 */
extern "C" void cTrickBoostFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cTrickBoostFX_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cTrickBoostFX_reset__FPv();

/**
 * Calls cTrickBoostFX_reset__FPv, passing its argument registers through untouched.
 *
 * cTrickBoostFX_reset__FPv sits between the cTrickBoostFX destructor and update, where
 * On Tour declares reset. Nothing in the retail code references it.
 */
extern "C" void cTrickBoostFX_readFromReplayFrame(void) {
    cTrickBoostFX_reset__FPv();
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cDynamicSprayFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cDynamicSprayFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cDynamicSprayFX_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cDynamicSprayFX_readFromReplayFrame(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cPowerUpFXToggles__cPowerUpFXToggles);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "PowerUpFXToggles" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit
 * 0 of @p flags is set.
 *
 * It is the destructor slot of D_00488330, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "PowerUpFXToggles".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cPowerUpFXToggles__cPowerUpFXToggles(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cFogParticleMan__cFogParticleMan);
#ifdef SKIP_ASM
extern void* cFogParticleMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the fog particle manager.
 *
 * Stores its vtable cFogParticleMan_vtbl and frees the object when bit 0 of @p flags is set; there is nothing else to release.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFogParticleMan__cFogParticleMan(cFogParticleMan* self, int flags)
{
    self->mVtbl = cFogParticleMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cPS2FogParticleMan__cPS2FogParticleMan);
#ifdef SKIP_ASM
extern void* cFogParticleMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the PS2 fog particle manager.
 *
 * The 256 sprites need no cleanup, so it only stores the base cFogParticleMan vtable cFogParticleMan_vtbl back and frees the
 * object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2FogParticleMan__cPS2FogParticleMan(cPS2FogParticleMan* self, int flags)
{
    self->mVtbl = cFogParticleMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cSplashToggles__cSplashToggles);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "SplashToggles" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0
 * of @p flags is set.
 *
 * It is the destructor slot of D_004882B0, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "SplashToggles".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSplashToggles__cSplashToggles(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cAttachedEmitterFX_ShouldDisableSnowTrailSpray);
#ifdef SKIP_ASM
/**
 * Returns the int at +0xA0 of @p arg0.
 *
 * Nothing in the retail code references it.
 *
 * @param arg0  the object
 * @return the value
 */
extern "C" int cAttachedEmitterFX_ShouldDisableSnowTrailSpray(cAttachedEmitterFX* arg0) {
    return (arg0->unk_0A0);
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAttachedEmitterFX_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cAttachedEmitterFX_writeToReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cAttachedEmitterFX_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing in the retail code references it.
 */
extern "C" void cAttachedEmitterFX_readFromReplayFrame(void) {

}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionMan_IsTransitionActive);
#ifdef SKIP_ASM
/**
 * Returns whether a transition is playing (+0x44) of a screen transition manager.
 *
 * It reads the flag cTransitionMan_StartTransition sets to 1 when it starts and checks to end a running one. Nothing in
 * the retail code references it.
 *
 * @param transitionMan  the transition manager
 * @return the playing flag
 */
extern "C" int cTransitionMan_IsTransitionActive(cTransitionMan* transitionMan) {
    return (transitionMan->mbPlayingTransition);
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionMan_IsWaitingForScreenGrab);
#ifdef SKIP_ASM
/**
 * Returns whether the manager is still requesting its screen grab (+0x64) of a screen transition manager.
 *
 * It reads the flag cTransitionMan_StartTransition sets with a screen grab, right after the using-screen-grab flag
 * (+0x60). Nothing in the retail code references it.
 *
 * @param transitionMan  the transition manager
 * @return the requesting flag
 */
extern "C" int cTransitionMan_IsWaitingForScreenGrab(cTransitionMan* transitionMan) {
    return (transitionMan->mRequestingScreenGrab);
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionMan_GetTransitionInitiator);
#ifdef SKIP_ASM
/**
 * Returns the initiator of the current transition (+0x78) of a screen transition manager.
 *
 * It reads the initiator cTransitionMan_StartTransition stores. Nothing in the retail code references it.
 *
 * @param transitionMan  the transition manager
 * @return the initiator
 */
extern "C" int cTransitionMan_GetTransitionInitiator(cTransitionMan* transitionMan) {
    return (transitionMan->mCurrentInitiator);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionMan_GetViewIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns the view index the manager serves (+0x74) of a screen transition manager.
 *
 * It reads the view index cTransitionMan_cTransitionMan stores. It is the slot after cTransitionMan_IsTransitionPending in the
 * cTransitionMan script-object vtable cTransitionMan_cScriptObjTransition_vtbl.
 *
 * @return the view index
 */
int cTransitionMan_GetViewIndex(void* vself)
{
    cTransitionMan* self = (cTransitionMan*)vself;
    return self->mViewIndex;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionMan_IsUsingTexture);
#ifdef SKIP_ASM
/**
 * Returns whether the transition uses the screen grab texture (+0x60) of a screen transition manager.
 *
 * It reads the useScreenGrab flag cTransitionMan_StartTransition stores. Nothing in the retail code references it.
 *
 * @param transitionMan  the transition manager
 * @return the flag
 */
extern "C" int cTransitionMan_IsUsingTexture(cTransitionMan* transitionMan) {
    return (transitionMan->mUsingScreenGrab);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cTransitionObj__cTransitionObj);
#ifdef SKIP_ASM
extern void* cTransitionObj_vtbl[];
extern "C" void cTransitionObj_operator_delete(void* p);

/**
 * Destroys a screen transition object and returns its buffer to the pool.
 *
 * Sets the vtable at +0x0 to cTransitionObj_vtbl and, when bit 0 of @p flags is set, gives the memory back with cTransitionObj_operator_delete
 * instead of the global operator delete.
 *
 * @param flags  g++ in-charge flags; bit 0 frees the object
 */
extern "C" void cTransitionObj__cTransitionObj(void* self, int flags)
{
    *(void***)self = cTransitionObj_vtbl;
    if (flags & 1) {
        cTransitionObj_operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cFade_TObj__cFade_TObj);
#ifdef SKIP_ASM
extern void* cTransitionObj_vtbl[];
extern "C" void cTransitionObj_operator_delete(void* p);

/**
 * Destroys a fade transition object: stores the base vtable (cTransitionObj_vtbl), then releases its slot when asked to.
 *
 * Release goes through cTransitionObj_operator_delete(), which clears the in-use flag of the matching slot in a 4-entry static
 * pool (cTransitionObj_mBuffer) instead of freeing heap memory.
 *
 * @param flags  g++ destructor flags; bit 0 set releases the object
 */
extern "C" void cFade_TObj__cFade_TObj(cFade_TObj* self, int flags)
{
    *(void***)self = cTransitionObj_vtbl;
    if (flags & 1) {
        cTransitionObj_operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cFade_TObj_cFade_TObj);
#ifdef SKIP_ASM

extern void* cFade_TObj_vtbl[];
void cTransitionObj_cTransitionObj(void* self);

/**
 * Constructs a fade transition object of a colour.
 *
 * Runs the transition object constructor cTransitionObj_cTransitionObj, installs cFade_TObj_vtbl and stores @p colour (a, r, g, b) at +0x4.
 * Nothing in the retail code references it.
 *
 * @param colour  the fade colour
 * @return @p self
 */
extern "C" void* cFade_TObj_cFade_TObj(cFade_TObj* self, tARGB colour)
{
    cTransitionObj_cTransitionObj(self);
    *(void***)self = cFade_TObj_vtbl;
    *(&self->mColour) = colour;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cWipe_TObj__cWipe_TObj);
#ifdef SKIP_ASM
extern void* cTransitionObj_vtbl[];
extern "C" void cTransitionObj_operator_delete(void* p);

/**
 * Destroys a wipe transition object: stores the base vtable (cTransitionObj_vtbl), then releases its slot when asked to.
 *
 * Release goes through cTransitionObj_operator_delete(), which clears the in-use flag of the matching slot in a 4-entry static
 * pool (cTransitionObj_mBuffer) instead of freeing heap memory.
 *
 * @param flags  g++ destructor flags; bit 0 set releases the object
 */
extern "C" void cWipe_TObj__cWipe_TObj(cWipe_TObj* self, int flags)
{
    *(void***)self = cTransitionObj_vtbl;
    if (flags & 1) {
        cTransitionObj_operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCircle_TObj__cCircle_TObj);
#ifdef SKIP_ASM
extern void* cTransitionObj_vtbl[];
extern "C" void cTransitionObj_operator_delete(void* p);

/**
 * Destroys a circle transition object: stores the base vtable (cTransitionObj_vtbl), then releases its slot when asked
 * to.
 *
 * Release goes through cTransitionObj_operator_delete(), which clears the in-use flag of the matching slot in a 4-entry static
 * pool (cTransitionObj_mBuffer) instead of freeing heap memory.
 *
 * @param flags  g++ destructor flags; bit 0 set releases the object
 */
extern "C" void cCircle_TObj__cCircle_TObj(cCircle_TObj* self, int flags)
{
    *(void***)self = cTransitionObj_vtbl;
    if (flags & 1) {
        cTransitionObj_operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cSnowfallToggles__cSnowfallToggles);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "SnowfallToggles" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit
 * 0 of @p flags is set.
 *
 * It is the destructor slot of D_00488050, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "SnowfallToggles".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSnowfallToggles__cSnowfallToggles(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cSnowFall_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cSnowFall's writeToReplayFrame has an empty body.
 */
void cSnowFall_writeToReplayFrame(void* vself)
{
    cSnowFall* self = (cSnowFall*)vself;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8170);
#ifdef SKIP_ASM
/**
 * Returns whether bit @p n of the 64-bit word at @p bits is set.
 *
 * Nothing in the retail code references it.
 *
 * @param bits  the 64-bit flag word
 * @param n  the bit number, 0..63
 * @return 1 when the bit is set, else 0
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_002F8170(unsigned long* bits, int n)
{
    unsigned long mask = (unsigned long)1 << n;
    return (*bits & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cBoardTrail__cBoardTrail);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "BoardTrail" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is the destructor slot of D_00487FA8, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "BoardTrail".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cBoardTrail__cBoardTrail(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* cObjNode__cObjNode(void* self);

//100%
INCLUDE_ASM("visualfx/worldlightman", cCutSceneTransitionMan__cCutSceneTransitionMan__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the cut-scene transition manager through the cObjNode destructor.
 *
 * Only @p self is declared here; any further argument the caller passed (such as destructor flags) reaches
 * cObjNode__cObjNode() unchanged in its register.
 *
 * @return the result of cObjNode__cObjNode()
 */
void* cCutSceneTransitionMan__cCutSceneTransitionMan(void* self)
{
    return cObjNode__cObjNode(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cCutSceneTransitionMan_GetCurrentState);
#ifdef SKIP_ASM
/**
 * Returns the state (+0x10) of a cut-scene transition manager.
 *
 * The state is the one cCutSceneTransitionMan_StartCutScene switches on (0 regular, 1 cut scene, 2 going out, 3 going
 * in). Nothing in the retail code references it.
 *
 * @param cutSceneMan  the cut-scene transition manager
 * @return the state
 */
extern "C" int cCutSceneTransitionMan_GetCurrentState(cCutSceneTransitionMan* cutSceneMan) {
    return (cutSceneMan->mCurrentState);
}
#endif
//100%
INCLUDE_ASM("visualfx/worldlightman", cBoltOnFXInterface_BoltOnsHaveBeenUpdated);
#ifdef SKIP_ASM
/**
 * Stores 1 in the int at @p arg0.
 *
 * Nothing in the retail code references it.
 *
 * @param arg0  the int to set
 */
extern "C" void cBoltOnFXInterface_BoltOnsHaveBeenUpdated(int *arg0) {
    *arg0 = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cLightGlow__cLightGlow);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "LightGlow" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is the destructor slot of D_00487EB8, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "LightGlow".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cLightGlow__cLightGlow(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cDepthFog__cDepthFog);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "DepthFog" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of @p
 * flags is set.
 *
 * It is the destructor slot of D_00487E88, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "DepthFog".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cDepthFog__cDepthFog(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cHeightFog__cHeightFog);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "HeightFog" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is the destructor slot of D_00487E58, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "HeightFog".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cHeightFog__cHeightFog(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cShadow__cShadow);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "Shadow" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of @p
 * flags is set.
 *
 * It is the destructor slot of D_00487E28, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "Shadow".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cShadow__cShadow(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cSnowSurface__cSnowSurface);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "SnowSurface" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is the destructor slot of D_00487DF8, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "SnowSurface".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSnowSurface__cSnowSurface(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cFlags__cFlags);
#ifdef SKIP_ASM
extern void* cVisualEffect_vtbl[];
void operator_delete(int*);

/**
 * Destroys the "Flags" visual effect: falls back to the base vtable cVisualEffect_vtbl and frees the object when bit 0 of @p
 * flags is set.
 *
 * It is the destructor slot of D_00487DC8, the vtable cPS2VisualEffectsMan_cPS2VisualEffectsMan installs on the 4-byte
 * effect it allocates as "Flags".
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFlags__cFlags(void* self, int flags)
{
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cVisualEffectsMan__cVisualEffectsMan);
#ifdef SKIP_ASM
extern void* cVisualEffectsMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the visual effects manager base.
 *
 * Stores cVisualEffectsMan_vtbl back at +0x44 (the vptr) and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVisualEffectsMan__cVisualEffectsMan(cVisualEffectsMan* self, int flags)
{
    self->mVtbl = cVisualEffectsMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", cAIVisualEffectsMan__cAIVisualEffectsMan);
#ifdef SKIP_ASM
extern void* cAIVisualEffectsMan_vtbl[];
void operator_delete(int*);

/**
 * Destroys the AI-rider visual effects manager base.
 *
 * Stores cAIVisualEffectsMan_vtbl back at +0x4 (the vptr) and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAIVisualEffectsMan__cAIVisualEffectsMan(cAIVisualEffectsMan* self, int flags)
{
    self->mVtbl = cAIVisualEffectsMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_002F6B88(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F8370__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: func_002F6B88(1, 0xFFFF).
 *
 * It is listed in the global constructor table (with func_002F9818__FPv and func_002FAE18__FPv).
 *
 * @return nothing useful (func_002F6B88 returns void)
 */
void* func_002F8370(void* self)
{
    return func_002F6B88(1, 0xffff);
}
#endif

extern "C" void* func_002F6B88(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F8390__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static destructors: func_002F6B88(0, 0xFFFF).
 *
 * It is listed in the global destructor table.
 *
 * @return nothing useful (func_002F6B88 returns void)
 */
void* func_002F8390(void* self)
{
    return func_002F6B88(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F83B0);
#ifdef SKIP_ASM
struct sAttachedEmitterType_83B0 {
    int unk_0;
    int unk_4;
    float unk_8;
    float unk_C;
    float unk_10;
    float unk_14;
    float unk_18;
    float unk_1C;
    float unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
    float unk_30;
    int unk_34;
    int unk_38;
    int unk_3C;
    float unk_40;
    int unk_44;
    int unk_48;
    int unk_4C;
    int unk_50;
    float unk_54;
    int unk_58;
    int unk_5C;
    float unk_60;
    float unk_64;
    int unk_68;
    int unk_6C;
    float unk_70;
    float unk_74;
    int unk_78;
    int unk_7C;
    float unk_80;
    float unk_84;
    float unk_88;
    float unk_8C;
    float unk_90;
    float unk_94;
    float unk_98;
    float unk_9C;
    float unk_A0;
    float unk_A4;
    float unk_A8;
    int unk_AC;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    int unk_C0;
    int unk_C4;
    int unk_C8;
    float unk_CC;
    int unk_D0;
    float unk_D4;
    float unk_D8;
    int unk_DC;
    int unk_E0;
    float unk_E4;
    float unk_E8;
    float unk_EC;
    float unk_F0;
    int unk_F4;
    float unk_F8;
    float unk_FC;
    float unk_100;
    float unk_104;
    float unk_108;
    float unk_10C;
};

extern sAttachedEmitterType_83B0* cAttachedEmitterFX_mEmitterDataArray;

/**
 * Fills attached-emitter types 0 to 9 of the table cAttachedEmitterFX_mEmitterDataArray (0x110-byte particle data records).
 *
 * func_002F17D0 allocates the 20-record table (5440 bytes, "AttachedEmitterTypeData") and then calls this and
 * func_002F9838 (types 10 to 19). func_002F2130 creates attached emitters from it by kind.
 */
extern "C" void func_002F83B0(void)
{
    sAttachedEmitterType_83B0* emitterTypes = cAttachedEmitterFX_mEmitterDataArray;
    emitterTypes[0].unk_0 = 1;
    emitterTypes[0].unk_4 = 0;
    emitterTypes[0].unk_8 = -1.0f;
    emitterTypes[0].unk_C = 1.0f;
    emitterTypes[0].unk_10 = 40.0f;
    emitterTypes[0].unk_14 = 1.0f;
    emitterTypes[0].unk_18 = 0.0f;
    emitterTypes[0].unk_1C = 0.0f;
    emitterTypes[0].unk_20 = 0.0f;
    emitterTypes[0].unk_24 = 0;
    emitterTypes[0].unk_28 = 0;
    emitterTypes[0].unk_2C = 0;
    emitterTypes[0].unk_30 = 0.0f;
    emitterTypes[0].unk_34 = 0;
    emitterTypes[0].unk_38 = 0;
    emitterTypes[0].unk_3C = 0;
    emitterTypes[0].unk_40 = 0.0f;
    emitterTypes[0].unk_44 = 0;
    emitterTypes[0].unk_48 = 0;
    emitterTypes[0].unk_4C = 0;
    emitterTypes[0].unk_50 = 0;
    emitterTypes[0].unk_54 = 150.0f;
    emitterTypes[0].unk_58 = 0;
    emitterTypes[0].unk_5C = 0;
    emitterTypes[0].unk_60 = 0.0f;
    emitterTypes[0].unk_64 = 150.0f;
    emitterTypes[0].unk_68 = 0;
    emitterTypes[0].unk_6C = 0;
    emitterTypes[0].unk_70 = 0.0f;
    emitterTypes[0].unk_74 = 150.0f;
    emitterTypes[0].unk_78 = 0;
    emitterTypes[0].unk_7C = 0;
    emitterTypes[0].unk_80 = 0.0f;
    emitterTypes[0].unk_84 = 1.0f;
    emitterTypes[0].unk_88 = 1.0f;
    emitterTypes[0].unk_8C = 1.0f;
    emitterTypes[0].unk_90 = 1.0f;
    emitterTypes[0].unk_94 = 1.0f;
    emitterTypes[0].unk_98 = 1.0f;
    emitterTypes[0].unk_9C = 1.0f;
    emitterTypes[0].unk_A0 = 1.0f;
    emitterTypes[0].unk_A4 = 0.0f;
    emitterTypes[0].unk_A8 = 0.0f;
    emitterTypes[0].unk_AC = 0;
    emitterTypes[0].unk_B0 = 0;
    emitterTypes[0].unk_B4 = 0;
    emitterTypes[0].unk_B8 = 0;
    emitterTypes[0].unk_BC = 0;
    emitterTypes[0].unk_C0 = 0;
    emitterTypes[0].unk_C8 = 1;
    emitterTypes[0].unk_CC = 20.0f;
    emitterTypes[0].unk_C4 = 5;
    emitterTypes[0].unk_D0 = 1;
    emitterTypes[0].unk_D4 = 20.0f;
    emitterTypes[0].unk_D8 = 1.0f;
    emitterTypes[0].unk_DC = 0;
    emitterTypes[0].unk_E0 = 0;
    emitterTypes[0].unk_E4 = 0.0f;
    emitterTypes[0].unk_E8 = 0.0f;
    emitterTypes[0].unk_EC = 0.0f;
    emitterTypes[0].unk_F0 = 0.0f;
    emitterTypes[0].unk_F4 = 0;
    emitterTypes[0].unk_F8 = 0.0f;
    emitterTypes[0].unk_FC = 0.0f;
    emitterTypes[0].unk_100 = 140.0f;
    emitterTypes[0].unk_104 = 1.0f;
    emitterTypes[0].unk_108 = 1.0f;
    emitterTypes[0].unk_10C = 1.0f;
    emitterTypes[1].unk_0 = 1;
    emitterTypes[1].unk_4 = 0;
    emitterTypes[1].unk_8 = -1.0f;
    emitterTypes[1].unk_C = 1.5f;
    emitterTypes[1].unk_10 = 70.0f;
    emitterTypes[1].unk_14 = 0.25f;
    emitterTypes[1].unk_18 = 50.0f;
    emitterTypes[1].unk_1C = 0.0f;
    emitterTypes[1].unk_20 = 0.0f;
    emitterTypes[1].unk_24 = 0;
    emitterTypes[1].unk_28 = 0;
    emitterTypes[1].unk_2C = 0;
    emitterTypes[1].unk_30 = 10.0f;
    emitterTypes[1].unk_34 = 0;
    emitterTypes[1].unk_38 = 0;
    emitterTypes[1].unk_3C = 0;
    emitterTypes[1].unk_40 = 10.0f;
    emitterTypes[1].unk_44 = 0;
    emitterTypes[1].unk_48 = 0;
    emitterTypes[1].unk_4C = 0;
    emitterTypes[1].unk_50 = 0;
    emitterTypes[1].unk_54 = 100.0f;
    emitterTypes[1].unk_58 = 0;
    emitterTypes[1].unk_5C = 0;
    emitterTypes[1].unk_60 = 0.0f;
    emitterTypes[1].unk_64 = 100.0f;
    emitterTypes[1].unk_68 = 0;
    emitterTypes[1].unk_6C = 0;
    emitterTypes[1].unk_70 = 0.0f;
    emitterTypes[1].unk_74 = 100.0f;
    emitterTypes[1].unk_78 = 0;
    emitterTypes[1].unk_7C = 0;
    emitterTypes[1].unk_80 = 1690.0f;
    emitterTypes[1].unk_84 = 1.0f;
    emitterTypes[1].unk_88 = 1.0f;
    emitterTypes[1].unk_8C = 1.0f;
    emitterTypes[1].unk_90 = 1.0f;
    emitterTypes[1].unk_94 = 0.0f;
    emitterTypes[1].unk_98 = 1.0f;
    emitterTypes[1].unk_9C = 0.0f;
    emitterTypes[1].unk_A0 = 0.0f;
    emitterTypes[1].unk_A4 = 0.0f;
    emitterTypes[1].unk_A8 = 0.0f;
    emitterTypes[1].unk_AC = 0;
    emitterTypes[1].unk_B0 = 0;
    emitterTypes[1].unk_B4 = 0;
    emitterTypes[1].unk_B8 = 0;
    emitterTypes[1].unk_BC = 0;
    emitterTypes[1].unk_C0 = 0;
    emitterTypes[1].unk_C8 = 0;
    emitterTypes[1].unk_CC = 0.0f;
    emitterTypes[1].unk_C4 = 24;
    emitterTypes[1].unk_D0 = 1;
    emitterTypes[1].unk_D4 = 20.0f;
    emitterTypes[1].unk_D8 = 1.0f;
    emitterTypes[1].unk_DC = 0;
    emitterTypes[1].unk_E0 = 0;
    emitterTypes[1].unk_E4 = 3.0f;
    emitterTypes[1].unk_E8 = 0.0f;
    emitterTypes[1].unk_EC = 3.0f;
    emitterTypes[1].unk_F0 = 0.0f;
    emitterTypes[1].unk_F4 = 0;
    emitterTypes[1].unk_F8 = 0.0f;
    emitterTypes[1].unk_FC = 0.0f;
    emitterTypes[1].unk_100 = 140.0f;
    emitterTypes[1].unk_104 = 1.0f;
    emitterTypes[1].unk_108 = 1.0f;
    emitterTypes[1].unk_10C = 1.0f;
    emitterTypes[2].unk_0 = 1;
    emitterTypes[2].unk_4 = 0;
    emitterTypes[2].unk_8 = -1.0f;
    emitterTypes[2].unk_C = 1.0f;
    emitterTypes[2].unk_10 = 30.0f;
    emitterTypes[2].unk_14 = 0.16501f;
    emitterTypes[2].unk_18 = 0.0f;
    emitterTypes[2].unk_1C = 0.0f;
    emitterTypes[2].unk_20 = 0.0f;
    emitterTypes[2].unk_24 = 0;
    emitterTypes[2].unk_28 = 0;
    emitterTypes[2].unk_2C = 0;
    emitterTypes[2].unk_30 = 0.0f;
    emitterTypes[2].unk_34 = 0;
    emitterTypes[2].unk_38 = 0;
    emitterTypes[2].unk_3C = 0;
    emitterTypes[2].unk_40 = 0.0f;
    emitterTypes[2].unk_44 = 0;
    emitterTypes[2].unk_48 = 0;
    emitterTypes[2].unk_4C = 0;
    emitterTypes[2].unk_50 = 0;
    emitterTypes[2].unk_54 = 50.0f;
    emitterTypes[2].unk_58 = 0;
    emitterTypes[2].unk_5C = 0;
    emitterTypes[2].unk_60 = 0.0f;
    emitterTypes[2].unk_64 = 100.0f;
    emitterTypes[2].unk_68 = 0;
    emitterTypes[2].unk_6C = 0;
    emitterTypes[2].unk_70 = 0.0f;
    emitterTypes[2].unk_74 = 100.0f;
    emitterTypes[2].unk_78 = 0;
    emitterTypes[2].unk_7C = 0;
    emitterTypes[2].unk_80 = 655.0f;
    emitterTypes[2].unk_84 = 1.0f;
    emitterTypes[2].unk_88 = 1.0f;
    emitterTypes[2].unk_8C = 1.0f;
    emitterTypes[2].unk_90 = 1.0f;
    emitterTypes[2].unk_94 = 0.0f;
    emitterTypes[2].unk_98 = 1.0f;
    emitterTypes[2].unk_9C = 0.0f;
    emitterTypes[2].unk_A0 = 0.0f;
    emitterTypes[2].unk_A4 = 0.0f;
    emitterTypes[2].unk_A8 = 0.0f;
    emitterTypes[2].unk_AC = 0;
    emitterTypes[2].unk_B0 = 0;
    emitterTypes[2].unk_B4 = 0;
    emitterTypes[2].unk_B8 = 0;
    emitterTypes[2].unk_BC = 0;
    emitterTypes[2].unk_C0 = 0;
    emitterTypes[2].unk_C8 = 0;
    emitterTypes[2].unk_CC = 0.0f;
    emitterTypes[2].unk_C4 = 24;
    emitterTypes[2].unk_D0 = 1;
    emitterTypes[2].unk_D4 = 20.0f;
    emitterTypes[2].unk_D8 = 1.0f;
    emitterTypes[2].unk_DC = 0;
    emitterTypes[2].unk_E0 = 0;
    emitterTypes[2].unk_E4 = 10.0f;
    emitterTypes[2].unk_E8 = 0.0f;
    emitterTypes[2].unk_EC = 0.0f;
    emitterTypes[2].unk_F0 = 0.0f;
    emitterTypes[2].unk_F4 = 0;
    emitterTypes[2].unk_F8 = 0.0f;
    emitterTypes[2].unk_FC = 0.0f;
    emitterTypes[2].unk_100 = 140.0f;
    emitterTypes[2].unk_104 = 1.0f;
    emitterTypes[2].unk_108 = 1.0f;
    emitterTypes[2].unk_10C = 1.0f;
    emitterTypes[3].unk_0 = 1;
    emitterTypes[3].unk_4 = 0;
    emitterTypes[3].unk_8 = -1.0f;
    emitterTypes[3].unk_C = 1.0f;
    emitterTypes[3].unk_10 = 55.55201f;
    emitterTypes[3].unk_14 = 0.1f;
    emitterTypes[3].unk_18 = 0.0f;
    emitterTypes[3].unk_1C = 0.0f;
    emitterTypes[3].unk_20 = 0.0f;
    emitterTypes[3].unk_24 = 0;
    emitterTypes[3].unk_28 = 0;
    emitterTypes[3].unk_2C = 0;
    emitterTypes[3].unk_30 = 0.0f;
    emitterTypes[3].unk_34 = 0;
    emitterTypes[3].unk_38 = 0;
    emitterTypes[3].unk_3C = 0;
    emitterTypes[3].unk_40 = 0.0f;
    emitterTypes[3].unk_44 = 0;
    emitterTypes[3].unk_48 = 0;
    emitterTypes[3].unk_4C = 0;
    emitterTypes[3].unk_50 = 0;
    emitterTypes[3].unk_54 = 50.0f;
    emitterTypes[3].unk_58 = 0;
    emitterTypes[3].unk_5C = 0;
    emitterTypes[3].unk_60 = 0.0f;
    emitterTypes[3].unk_64 = 500.0f;
    emitterTypes[3].unk_68 = 0;
    emitterTypes[3].unk_6C = 0;
    emitterTypes[3].unk_70 = 0.0f;
    emitterTypes[3].unk_74 = 500.0f;
    emitterTypes[3].unk_78 = 0;
    emitterTypes[3].unk_7C = 0;
    emitterTypes[3].unk_80 = -200.0f;
    emitterTypes[3].unk_84 = 1.0f;
    emitterTypes[3].unk_88 = 1.0f;
    emitterTypes[3].unk_8C = 0.2f;
    emitterTypes[3].unk_90 = 0.2f;
    emitterTypes[3].unk_94 = 0.0f;
    emitterTypes[3].unk_98 = 1.0f;
    emitterTypes[3].unk_9C = 0.2f;
    emitterTypes[3].unk_A0 = 0.2f;
    emitterTypes[3].unk_A4 = 0.0f;
    emitterTypes[3].unk_A8 = 0.0f;
    emitterTypes[3].unk_AC = 0;
    emitterTypes[3].unk_B0 = 0;
    emitterTypes[3].unk_B4 = 0;
    emitterTypes[3].unk_B8 = 0;
    emitterTypes[3].unk_BC = 0;
    emitterTypes[3].unk_C0 = 0;
    emitterTypes[3].unk_C8 = 0;
    emitterTypes[3].unk_CC = 0.0f;
    emitterTypes[3].unk_C4 = 24;
    emitterTypes[3].unk_D0 = 1;
    emitterTypes[3].unk_D4 = 20.0f;
    emitterTypes[3].unk_D8 = 1.0f;
    emitterTypes[3].unk_DC = 0;
    emitterTypes[3].unk_E0 = 0;
    emitterTypes[3].unk_E4 = 10.0f;
    emitterTypes[3].unk_E8 = 0.0f;
    emitterTypes[3].unk_EC = 0.0f;
    emitterTypes[3].unk_F0 = 0.0f;
    emitterTypes[3].unk_F4 = 0;
    emitterTypes[3].unk_F8 = 0.0f;
    emitterTypes[3].unk_FC = 0.0f;
    emitterTypes[3].unk_100 = 140.0f;
    emitterTypes[3].unk_104 = 1.0f;
    emitterTypes[3].unk_108 = 1.0f;
    emitterTypes[3].unk_10C = 1.0f;
    emitterTypes[4].unk_0 = 2;
    emitterTypes[4].unk_4 = 0;
    emitterTypes[4].unk_8 = -1.0f;
    emitterTypes[4].unk_C = 1.0f;
    emitterTypes[4].unk_10 = 100.0f;
    emitterTypes[4].unk_14 = 0.4f;
    emitterTypes[4].unk_18 = 0.0f;
    emitterTypes[4].unk_1C = 0.0f;
    emitterTypes[4].unk_20 = 0.0f;
    emitterTypes[4].unk_24 = 0;
    emitterTypes[4].unk_28 = 0;
    emitterTypes[4].unk_2C = 0;
    emitterTypes[4].unk_30 = 0.0f;
    emitterTypes[4].unk_34 = 0;
    emitterTypes[4].unk_38 = 0;
    emitterTypes[4].unk_3C = 0;
    emitterTypes[4].unk_40 = 0.0f;
    emitterTypes[4].unk_44 = 0;
    emitterTypes[4].unk_48 = 0;
    emitterTypes[4].unk_4C = 0;
    emitterTypes[4].unk_50 = 0;
    emitterTypes[4].unk_54 = 100.0f;
    emitterTypes[4].unk_58 = 0;
    emitterTypes[4].unk_5C = 0;
    emitterTypes[4].unk_60 = 0.0f;
    emitterTypes[4].unk_64 = 100.0f;
    emitterTypes[4].unk_68 = 0;
    emitterTypes[4].unk_6C = 0;
    emitterTypes[4].unk_70 = 0.0f;
    emitterTypes[4].unk_74 = 100.0f;
    emitterTypes[4].unk_78 = 0;
    emitterTypes[4].unk_7C = 0;
    emitterTypes[4].unk_80 = 500.0f;
    emitterTypes[4].unk_84 = 1.0f;
    emitterTypes[4].unk_88 = 1.0f;
    emitterTypes[4].unk_8C = 1.0f;
    emitterTypes[4].unk_90 = 1.0f;
    emitterTypes[4].unk_94 = 0.0f;
    emitterTypes[4].unk_98 = 1.0f;
    emitterTypes[4].unk_9C = 0.0f;
    emitterTypes[4].unk_A0 = 0.0f;
    emitterTypes[4].unk_A4 = 0.0f;
    emitterTypes[4].unk_A8 = 0.0f;
    emitterTypes[4].unk_AC = 0;
    emitterTypes[4].unk_B0 = 0;
    emitterTypes[4].unk_B4 = 0;
    emitterTypes[4].unk_B8 = 0;
    emitterTypes[4].unk_BC = 0;
    emitterTypes[4].unk_C0 = 0;
    emitterTypes[4].unk_C8 = 0;
    emitterTypes[4].unk_CC = 40.0f;
    emitterTypes[4].unk_C4 = 24;
    emitterTypes[4].unk_D0 = 1;
    emitterTypes[4].unk_D4 = 20.0f;
    emitterTypes[4].unk_D8 = 1.25f;
    emitterTypes[4].unk_DC = 0;
    emitterTypes[4].unk_E0 = 0;
    emitterTypes[4].unk_E4 = 0.0f;
    emitterTypes[4].unk_E8 = 8.0f;
    emitterTypes[4].unk_EC = 0.0f;
    emitterTypes[4].unk_F0 = 0.0f;
    emitterTypes[4].unk_F4 = 0;
    emitterTypes[4].unk_F8 = 0.0f;
    emitterTypes[4].unk_FC = 0.0f;
    emitterTypes[4].unk_100 = 140.0f;
    emitterTypes[4].unk_104 = 1.0f;
    emitterTypes[4].unk_108 = 1.0f;
    emitterTypes[4].unk_10C = 1.0f;
    emitterTypes[5].unk_0 = 1;
    emitterTypes[5].unk_4 = 0;
    emitterTypes[5].unk_8 = -1.0f;
    emitterTypes[5].unk_C = 1.5f;
    emitterTypes[5].unk_10 = 10.0f;
    emitterTypes[5].unk_14 = 0.5f;
    emitterTypes[5].unk_18 = 0.0f;
    emitterTypes[5].unk_1C = 0.0f;
    emitterTypes[5].unk_20 = 0.0f;
    emitterTypes[5].unk_24 = 0;
    emitterTypes[5].unk_28 = 0;
    emitterTypes[5].unk_2C = 0;
    emitterTypes[5].unk_30 = 0.0f;
    emitterTypes[5].unk_34 = 0;
    emitterTypes[5].unk_38 = 0;
    emitterTypes[5].unk_3C = 0;
    emitterTypes[5].unk_40 = 0.0f;
    emitterTypes[5].unk_44 = 0;
    emitterTypes[5].unk_48 = 0;
    emitterTypes[5].unk_4C = 0;
    emitterTypes[5].unk_50 = 0;
    emitterTypes[5].unk_54 = 40.0f;
    emitterTypes[5].unk_58 = 0;
    emitterTypes[5].unk_5C = 0;
    emitterTypes[5].unk_60 = 0.0f;
    emitterTypes[5].unk_64 = 40.0f;
    emitterTypes[5].unk_68 = 0;
    emitterTypes[5].unk_6C = 0;
    emitterTypes[5].unk_70 = 0.0f;
    emitterTypes[5].unk_74 = 40.0f;
    emitterTypes[5].unk_78 = 0;
    emitterTypes[5].unk_7C = 0;
    emitterTypes[5].unk_80 = 500.0f;
    emitterTypes[5].unk_84 = 0.999f;
    emitterTypes[5].unk_88 = 0.3995f;
    emitterTypes[5].unk_8C = 0.4f;
    emitterTypes[5].unk_90 = 0.4f;
    emitterTypes[5].unk_94 = 0.0f;
    emitterTypes[5].unk_98 = 0.09875f;
    emitterTypes[5].unk_9C = 0.115f;
    emitterTypes[5].unk_A0 = 0.1075f;
    emitterTypes[5].unk_A4 = 0.0f;
    emitterTypes[5].unk_A8 = 0.0f;
    emitterTypes[5].unk_AC = 0;
    emitterTypes[5].unk_B0 = 0;
    emitterTypes[5].unk_B4 = 0;
    emitterTypes[5].unk_B8 = 0;
    emitterTypes[5].unk_BC = 0;
    emitterTypes[5].unk_C0 = 0;
    emitterTypes[5].unk_C8 = 1;
    emitterTypes[5].unk_CC = 120.0f;
    emitterTypes[5].unk_C4 = 25;
    emitterTypes[5].unk_D0 = 1;
    emitterTypes[5].unk_D4 = 20.0f;
    emitterTypes[5].unk_D8 = 0.8f;
    emitterTypes[5].unk_DC = 0;
    emitterTypes[5].unk_E0 = 0;
    emitterTypes[5].unk_E4 = 0.0f;
    emitterTypes[5].unk_E8 = 7.0f;
    emitterTypes[5].unk_EC = 0.0f;
    emitterTypes[5].unk_F0 = 0.0f;
    emitterTypes[5].unk_F4 = 0;
    emitterTypes[5].unk_F8 = 0.0f;
    emitterTypes[5].unk_FC = 0.0f;
    emitterTypes[5].unk_100 = 140.0f;
    emitterTypes[5].unk_104 = 1.0f;
    emitterTypes[5].unk_108 = 1.0f;
    emitterTypes[5].unk_10C = 1.0f;
    emitterTypes[6].unk_0 = 2;
    emitterTypes[6].unk_4 = 0;
    emitterTypes[6].unk_8 = -1.0f;
    emitterTypes[6].unk_C = 1.0f;
    emitterTypes[6].unk_10 = 24.0f;
    emitterTypes[6].unk_14 = 0.1f;
    emitterTypes[6].unk_18 = 6.0f;
    emitterTypes[6].unk_1C = 0.1f;
    emitterTypes[6].unk_20 = 0.09f;
    emitterTypes[6].unk_24 = 0;
    emitterTypes[6].unk_28 = 0;
    emitterTypes[6].unk_2C = 0;
    emitterTypes[6].unk_30 = 0.0f;
    emitterTypes[6].unk_34 = 0;
    emitterTypes[6].unk_38 = 0;
    emitterTypes[6].unk_3C = 0;
    emitterTypes[6].unk_40 = 0.0f;
    emitterTypes[6].unk_44 = 0;
    emitterTypes[6].unk_48 = 0;
    emitterTypes[6].unk_4C = 0;
    emitterTypes[6].unk_50 = 0;
    emitterTypes[6].unk_54 = 0.0f;
    emitterTypes[6].unk_58 = 0;
    emitterTypes[6].unk_5C = 0;
    emitterTypes[6].unk_60 = 0.0f;
    emitterTypes[6].unk_64 = 0.0f;
    emitterTypes[6].unk_68 = 0;
    emitterTypes[6].unk_6C = 0;
    emitterTypes[6].unk_70 = 0.0f;
    emitterTypes[6].unk_74 = 0.0f;
    emitterTypes[6].unk_78 = 0;
    emitterTypes[6].unk_7C = 0;
    emitterTypes[6].unk_80 = 0.0f;
    emitterTypes[6].unk_84 = 1.0f;
    emitterTypes[6].unk_88 = 0.045f;
    emitterTypes[6].unk_8C = 0.00875f;
    emitterTypes[6].unk_90 = 1.0f;
    emitterTypes[6].unk_94 = 0.0f;
    emitterTypes[6].unk_98 = 1.0f;
    emitterTypes[6].unk_9C = 0.96675f;
    emitterTypes[6].unk_A0 = 1.0f;
    emitterTypes[6].unk_A4 = 0.5f;
    emitterTypes[6].unk_A8 = 0.0f;
    emitterTypes[6].unk_AC = 0;
    emitterTypes[6].unk_B0 = 0;
    emitterTypes[6].unk_B4 = 0;
    emitterTypes[6].unk_B8 = 0;
    emitterTypes[6].unk_BC = 0;
    emitterTypes[6].unk_C0 = 0;
    emitterTypes[6].unk_C8 = 0;
    emitterTypes[6].unk_CC = 0.0f;
    emitterTypes[6].unk_C4 = 5;
    emitterTypes[6].unk_D0 = 1;
    emitterTypes[6].unk_D4 = 20.0f;
    emitterTypes[6].unk_D8 = 1.2f;
    emitterTypes[6].unk_DC = 0;
    emitterTypes[6].unk_E0 = 0;
    emitterTypes[6].unk_E4 = -21.5f;
    emitterTypes[6].unk_E8 = -13.75f;
    emitterTypes[6].unk_EC = -21.5f;
    emitterTypes[6].unk_F0 = -372.75f;
    emitterTypes[6].unk_F4 = 0;
    emitterTypes[6].unk_F8 = -3.0f;
    emitterTypes[6].unk_FC = 0.0f;
    emitterTypes[6].unk_100 = 140.0f;
    emitterTypes[6].unk_104 = 1.0f;
    emitterTypes[6].unk_108 = 1.0f;
    emitterTypes[6].unk_10C = 0.0f;
    emitterTypes[7].unk_0 = 2;
    emitterTypes[7].unk_4 = 0;
    emitterTypes[7].unk_8 = -1.0f;
    emitterTypes[7].unk_C = 1.0f;
    emitterTypes[7].unk_10 = 24.0f;
    emitterTypes[7].unk_14 = 0.1f;
    emitterTypes[7].unk_18 = 6.0f;
    emitterTypes[7].unk_1C = 0.1f;
    emitterTypes[7].unk_20 = 0.09f;
    emitterTypes[7].unk_24 = 0;
    emitterTypes[7].unk_28 = 0;
    emitterTypes[7].unk_2C = 0;
    emitterTypes[7].unk_30 = 0.0f;
    emitterTypes[7].unk_34 = 0;
    emitterTypes[7].unk_38 = 0;
    emitterTypes[7].unk_3C = 0;
    emitterTypes[7].unk_40 = 0.0f;
    emitterTypes[7].unk_44 = 0;
    emitterTypes[7].unk_48 = 0;
    emitterTypes[7].unk_4C = 0;
    emitterTypes[7].unk_50 = 0;
    emitterTypes[7].unk_54 = 0.0f;
    emitterTypes[7].unk_58 = 0;
    emitterTypes[7].unk_5C = 0;
    emitterTypes[7].unk_60 = 0.0f;
    emitterTypes[7].unk_64 = 0.0f;
    emitterTypes[7].unk_68 = 0;
    emitterTypes[7].unk_6C = 0;
    emitterTypes[7].unk_70 = 0.0f;
    emitterTypes[7].unk_74 = 0.0f;
    emitterTypes[7].unk_78 = 0;
    emitterTypes[7].unk_7C = 0;
    emitterTypes[7].unk_80 = 0.0f;
    emitterTypes[7].unk_84 = 1.0f;
    emitterTypes[7].unk_88 = 0.045f;
    emitterTypes[7].unk_8C = 0.00875f;
    emitterTypes[7].unk_90 = 1.0f;
    emitterTypes[7].unk_94 = 0.0f;
    emitterTypes[7].unk_98 = 1.0f;
    emitterTypes[7].unk_9C = 0.96675f;
    emitterTypes[7].unk_A0 = 1.0f;
    emitterTypes[7].unk_A4 = 0.5f;
    emitterTypes[7].unk_A8 = 0.0f;
    emitterTypes[7].unk_AC = 0;
    emitterTypes[7].unk_B0 = 0;
    emitterTypes[7].unk_B4 = 0;
    emitterTypes[7].unk_B8 = 0;
    emitterTypes[7].unk_BC = 0;
    emitterTypes[7].unk_C0 = 0;
    emitterTypes[7].unk_C8 = 0;
    emitterTypes[7].unk_CC = 0.0f;
    emitterTypes[7].unk_C4 = 5;
    emitterTypes[7].unk_D0 = 1;
    emitterTypes[7].unk_D4 = 20.0f;
    emitterTypes[7].unk_D8 = 1.2f;
    emitterTypes[7].unk_DC = 0;
    emitterTypes[7].unk_E0 = 0;
    emitterTypes[7].unk_E4 = -21.5f;
    emitterTypes[7].unk_E8 = 16.25f;
    emitterTypes[7].unk_EC = -21.5f;
    emitterTypes[7].unk_F0 = -372.75f;
    emitterTypes[7].unk_F4 = 0;
    emitterTypes[7].unk_F8 = -3.0f;
    emitterTypes[7].unk_FC = 0.0f;
    emitterTypes[7].unk_100 = 140.0f;
    emitterTypes[7].unk_104 = 1.0f;
    emitterTypes[7].unk_108 = 1.0f;
    emitterTypes[7].unk_10C = 54.5f;
    emitterTypes[8].unk_0 = 3;
    emitterTypes[8].unk_4 = 0;
    emitterTypes[8].unk_8 = -1.0f;
    emitterTypes[8].unk_C = 1.0f;
    emitterTypes[8].unk_10 = 6.97601f;
    emitterTypes[8].unk_14 = 0.1f;
    emitterTypes[8].unk_18 = 6.208f;
    emitterTypes[8].unk_1C = 0.1f;
    emitterTypes[8].unk_20 = 0.01f;
    emitterTypes[8].unk_24 = 0;
    emitterTypes[8].unk_28 = 0;
    emitterTypes[8].unk_2C = 0;
    emitterTypes[8].unk_30 = 6.0f;
    emitterTypes[8].unk_34 = 0;
    emitterTypes[8].unk_38 = 0;
    emitterTypes[8].unk_3C = 0;
    emitterTypes[8].unk_40 = 6.0f;
    emitterTypes[8].unk_44 = 0;
    emitterTypes[8].unk_48 = 0;
    emitterTypes[8].unk_4C = 0;
    emitterTypes[8].unk_50 = 0;
    emitterTypes[8].unk_54 = 0.0f;
    emitterTypes[8].unk_58 = 0;
    emitterTypes[8].unk_5C = 0;
    emitterTypes[8].unk_60 = 87.5f;
    emitterTypes[8].unk_64 = 0.0f;
    emitterTypes[8].unk_68 = 0;
    emitterTypes[8].unk_6C = 0;
    emitterTypes[8].unk_70 = 270.0f;
    emitterTypes[8].unk_74 = 0.0f;
    emitterTypes[8].unk_78 = 0;
    emitterTypes[8].unk_7C = 0;
    emitterTypes[8].unk_80 = 0.0f;
    emitterTypes[8].unk_84 = 0.92425f;
    emitterTypes[8].unk_88 = 1.0f;
    emitterTypes[8].unk_8C = 1.0f;
    emitterTypes[8].unk_90 = 0.42425f;
    emitterTypes[8].unk_94 = 0.0f;
    emitterTypes[8].unk_98 = 0.93575f;
    emitterTypes[8].unk_9C = 0.0f;
    emitterTypes[8].unk_A0 = 0.0f;
    emitterTypes[8].unk_A4 = 0.0f;
    emitterTypes[8].unk_A8 = 0.115f;
    emitterTypes[8].unk_AC = 0;
    emitterTypes[8].unk_B0 = 0;
    emitterTypes[8].unk_B4 = 0;
    emitterTypes[8].unk_B8 = 0;
    emitterTypes[8].unk_BC = 0;
    emitterTypes[8].unk_C0 = 0;
    emitterTypes[8].unk_C8 = 0;
    emitterTypes[8].unk_CC = 20.6f;
    emitterTypes[8].unk_C4 = 5;
    emitterTypes[8].unk_D0 = 1;
    emitterTypes[8].unk_D4 = 20.0f;
    emitterTypes[8].unk_D8 = 1.2f;
    emitterTypes[8].unk_DC = 0;
    emitterTypes[8].unk_E0 = 0;
    emitterTypes[8].unk_E4 = -39.5f;
    emitterTypes[8].unk_E8 = 0.0f;
    emitterTypes[8].unk_EC = -21.5f;
    emitterTypes[8].unk_F0 = -394.25f;
    emitterTypes[8].unk_F4 = 0;
    emitterTypes[8].unk_F8 = 0.0f;
    emitterTypes[8].unk_FC = 0.0f;
    emitterTypes[8].unk_100 = 110.0f;
    emitterTypes[8].unk_104 = 1.0f;
    emitterTypes[8].unk_108 = 1.0f;
    emitterTypes[8].unk_10C = 1.0f;
    emitterTypes[9].unk_0 = 8;
    emitterTypes[9].unk_4 = 0;
    emitterTypes[9].unk_8 = -1.0f;
    emitterTypes[9].unk_C = 1.0f;
    emitterTypes[9].unk_10 = 6.97601f;
    emitterTypes[9].unk_14 = 0.2f;
    emitterTypes[9].unk_18 = 6.208f;
    emitterTypes[9].unk_1C = 0.2f;
    emitterTypes[9].unk_20 = 0.01f;
    emitterTypes[9].unk_24 = 0;
    emitterTypes[9].unk_28 = 0;
    emitterTypes[9].unk_2C = 0;
    emitterTypes[9].unk_30 = 6.0f;
    emitterTypes[9].unk_34 = 0;
    emitterTypes[9].unk_38 = 0;
    emitterTypes[9].unk_3C = 0;
    emitterTypes[9].unk_40 = 6.0f;
    emitterTypes[9].unk_44 = 0;
    emitterTypes[9].unk_48 = 0;
    emitterTypes[9].unk_4C = 0;
    emitterTypes[9].unk_50 = 0;
    emitterTypes[9].unk_54 = 0.0f;
    emitterTypes[9].unk_58 = 0;
    emitterTypes[9].unk_5C = 0;
    emitterTypes[9].unk_60 = 87.5f;
    emitterTypes[9].unk_64 = 0.0f;
    emitterTypes[9].unk_68 = 0;
    emitterTypes[9].unk_6C = 0;
    emitterTypes[9].unk_70 = 270.0f;
    emitterTypes[9].unk_74 = 0.0f;
    emitterTypes[9].unk_78 = 0;
    emitterTypes[9].unk_7C = 0;
    emitterTypes[9].unk_80 = 0.0f;
    emitterTypes[9].unk_84 = 0.92425f;
    emitterTypes[9].unk_88 = 1.0f;
    emitterTypes[9].unk_8C = 1.0f;
    emitterTypes[9].unk_90 = 0.42425f;
    emitterTypes[9].unk_94 = 0.0f;
    emitterTypes[9].unk_98 = 0.93575f;
    emitterTypes[9].unk_9C = 0.0f;
    emitterTypes[9].unk_A0 = 0.0f;
    emitterTypes[9].unk_A4 = 0.0f;
    emitterTypes[9].unk_A8 = 0.115f;
    emitterTypes[9].unk_AC = 0;
    emitterTypes[9].unk_B0 = 0;
    emitterTypes[9].unk_B4 = 0;
    emitterTypes[9].unk_B8 = 0;
    emitterTypes[9].unk_BC = 0;
    emitterTypes[9].unk_C0 = 0;
    emitterTypes[9].unk_C8 = 0;
    emitterTypes[9].unk_CC = 20.6f;
    emitterTypes[9].unk_C4 = 5;
    emitterTypes[9].unk_D0 = 1;
    emitterTypes[9].unk_D4 = 20.0f;
    emitterTypes[9].unk_D8 = 1.2f;
    emitterTypes[9].unk_DC = 0;
    emitterTypes[9].unk_E0 = 0;
    emitterTypes[9].unk_E4 = -39.5f;
    emitterTypes[9].unk_E8 = 0.0f;
    emitterTypes[9].unk_EC = -21.5f;
    emitterTypes[9].unk_F0 = -394.25f;
    emitterTypes[9].unk_F4 = 0;
    emitterTypes[9].unk_F8 = 0.0f;
    emitterTypes[9].unk_FC = 110.0f;
    emitterTypes[9].unk_100 = 140.0f;
    emitterTypes[9].unk_104 = 1.0f;
    emitterTypes[9].unk_108 = 1.0f;
    emitterTypes[9].unk_10C = 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F8FF8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * func_002F1810 calls it, with func_002FA5F8__FPv, before freeing the attached-emitter type table cAttachedEmitterFX_mEmitterDataArray.
 */
void func_002F8FF8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9000);
#ifdef SKIP_ASM
/**
 * Returns @p self unchanged.
 *
 * The definition is variadic, so it first spills the argument registers. Nothing in the retail code references it.
 *
 * @return @p self
 */
extern "C" void* func_002F9000(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9040);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_2F9040 {
    float r, g, b, a;
    sColor_2F9040(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_2F9040 D_004FAC90;
extern sColor_2F9040 D_004FACA0;
extern sColor_2F9040 D_004FACB0;
extern sColor_2F9040 D_004FACC0;
extern sColor_2F9040 D_004FACD0;
extern sColor_2F9040 D_004FACE0;
extern sColor_2F9040 D_004FACF0;
extern sColor_2F9040 D_004FAD00;
extern sColor_2F9040 D_004FAD10;
extern sColor_2F9040 D_004FAD20;
extern sColor_2F9040 D_004FAD30;
extern sColor_2F9040 D_004FAD40;
extern sColor_2F9040 D_004FAD50;
extern sColor_2F9040 D_004FAD60;
extern sColor_2F9040 D_004FAD70;
extern sColor_2F9040 D_004FAD80;
extern sColor_2F9040 D_004FAD90;
extern sColor_2F9040 D_004FADA0;
extern sColor_2F9040 D_004FADB0;
extern sColor_2F9040 D_004FADC0;
extern sColor_2F9040 D_004FADD0;
extern sColor_2F9040 D_004FADE0;
extern sColor_2F9040 D_004FADF0;
extern sColor_2F9040 D_004FAE00;
extern sColor_2F9040 D_004FAE10;
extern sColor_2F9040 D_004FAE20;
extern sColor_2F9040 D_004FAE30;
extern sColor_2F9040 D_004FAE40;
extern sColor_2F9040 D_004FAE50;
extern sColor_2F9040 D_004FAE60;
extern sColor_2F9040 D_004FAE70;
extern sColor_2F9040 D_004FAE80;
extern sColor_2F9040 D_004FAE90;
extern sColor_2F9040 D_004FAEA0;
extern sColor_2F9040 D_004FAEB0;
extern sColor_2F9040 D_004FAEC0;
extern sColor_2F9040 D_004FAED0;
extern sColor_2F9040 D_004FAEE0;
extern sColor_2F9040 D_004FAEF0;
extern sColor_2F9040 D_004FAF00;
extern sColor_2F9040 D_004FAF10;
extern sColor_2F9040 D_004FAF20;
extern sColor_2F9040 D_004FAF30;
extern sColor_2F9040 D_004FAF40;
extern sColor_2F9040 D_004FAF50;
extern sColor_2F9040 D_004FAF60;
extern sColor_2F9040 D_004FAF70;
extern sColor_2F9040 D_004FAF80;
extern sColor_2F9040 D_004FAF90;
extern sColor_2F9040 D_004FAFA0;
extern sColor_2F9040 D_004FAFB0;
extern sColor_2F9040 D_004FAFC0;
extern sColor_2F9040 D_004FAFD0;
extern sColor_2F9040 D_004FAFE0;
extern sColor_2F9040 D_004FAFF0;
extern sColor_2F9040 D_004FB000;

// The unit declares func_002F9040 later as returning void*; bind this body by asm label.
extern "C" void func_002F9040_impl(int initialize, int priority) __asm__("func_002F9040");

/**
 * Runs the static constructors of a second copy of the visual-effects colour presets (D_004FAC90..D_004FB000).
 *
 * Acts only for @p priority 0xFFFF with @p initialize set: builds the 56 (a, r, g, b) colours, the same presets
 * func_002F6B88 writes to D_004D57B0 onward. func_002F9818__FPv calls it.
 *
 * @param initialize  nonzero to construct; 0 does nothing
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_002F9040_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004FAC90) sColor_2F9040(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FACA0) sColor_2F9040(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACB0) sColor_2F9040(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACC0) sColor_2F9040(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FACD0) sColor_2F9040(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FACE0) sColor_2F9040(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FACF0) sColor_2F9040(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FAD00) sColor_2F9040(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FAD10) sColor_2F9040(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FAD20) sColor_2F9040(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FAD30) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD40) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAD50) sColor_2F9040(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FAD60) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD70) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAD80) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAD90) sColor_2F9040(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FADA0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FADB0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADC0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADD0) sColor_2F9040(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FADE0) sColor_2F9040(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FADF0) sColor_2F9040(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FAE00) sColor_2F9040(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FAE10) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE20) sColor_2F9040(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FAE30) sColor_2F9040(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FAE40) sColor_2F9040(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FAE50) sColor_2F9040(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FAE60) sColor_2F9040(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FAE70) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE80) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAE90) sColor_2F9040(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FAEA0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAEB0) sColor_2F9040(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FAEC0) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAED0) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAEE0) sColor_2F9040(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FAEF0) sColor_2F9040(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FAF00) sColor_2F9040(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FAF10) sColor_2F9040(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FAF20) sColor_2F9040(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FAF30) sColor_2F9040(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FAF40) sColor_2F9040(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FAF50) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF60) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF70) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FAF80) sColor_2F9040(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FAF90) sColor_2F9040(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FAFA0) sColor_2F9040(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FAFB0) sColor_2F9040(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FAFC0) sColor_2F9040(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FAFD0) sColor_2F9040(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FAFE0) sColor_2F9040(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FAFF0) sColor_2F9040(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FB000) sColor_2F9040(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_002F9040(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002F9818__FPv);
#ifdef SKIP_ASM
/**
 * Runs func_002F9040(1, 0xFFFF): a global constructor listed in the constructor table.
 *
 * @return nothing useful (func_002F9040 returns void)
 */
void* func_002F9818(void* self)
{
    return func_002F9040(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002F9838);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sAttachedEmitterType_9838 {
    int unk_0;
    int unk_4;
    float unk_8;
    float unk_C;
    float unk_10;
    float unk_14;
    float unk_18;
    float unk_1C;
    float unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
    float unk_30;
    int unk_34;
    float unk_38;
    float unk_3C;
    float unk_40;
    int unk_44;
    int unk_48;
    int unk_4C;
    int unk_50;
    float unk_54;
    int unk_58;
    int unk_5C;
    int unk_60;
    float unk_64;
    int unk_68;
    int unk_6C;
    int unk_70;
    float unk_74;
    int unk_78;
    int unk_7C;
    float unk_80;
    float unk_84;
    float unk_88;
    float unk_8C;
    float unk_90;
    float unk_94;
    float unk_98;
    float unk_9C;
    float unk_A0;
    int unk_A4;
    float unk_A8;
    float unk_AC;
    float unk_B0;
    int unk_B4;
    float unk_B8;
    float unk_BC;
    float unk_C0;
    int unk_C4;
    int unk_C8;
    float unk_CC;
    int unk_D0;
    float unk_D4;
    float unk_D8;
    int unk_DC;
    int unk_E0;
    float unk_E4;
    float unk_E8;
    float unk_EC;
    float unk_F0;
    int unk_F4;
    float unk_F8;
    float unk_FC;
    float unk_100;
    float unk_104;
    float unk_108;
    float unk_10C;
};

extern sAttachedEmitterType_9838* cAttachedEmitterFX_mEmitterDataArray_9838 __asm__("cAttachedEmitterFX_mEmitterDataArray");

/**
 * Fills attached-emitter types 10 to 19 of the table cAttachedEmitterFX_mEmitterDataArray (0x110-byte particle data records).
 *
 * func_002F17D0 calls it right after func_002F83B0 (types 0 to 9).
 */
extern "C" void func_002F9838(void)
{
    sAttachedEmitterType_9838* emitterTypes = cAttachedEmitterFX_mEmitterDataArray_9838;
    emitterTypes[10].unk_0 = 5;
    emitterTypes[10].unk_4 = 0;
    emitterTypes[10].unk_8 = -1.0f;
    emitterTypes[10].unk_C = 0.8f;
    emitterTypes[10].unk_10 = 5.0f;
    emitterTypes[10].unk_14 = 0.25f;
    emitterTypes[10].unk_18 = 0.0f;
    emitterTypes[10].unk_1C = 0.1f;
    emitterTypes[10].unk_20 = 0.0f;
    emitterTypes[10].unk_24 = 0;
    emitterTypes[10].unk_28 = 0;
    emitterTypes[10].unk_2C = 0;
    emitterTypes[10].unk_30 = 0.0f;
    emitterTypes[10].unk_34 = 0;
    emitterTypes[10].unk_38 = 0.0f;
    emitterTypes[10].unk_3C = 0.0f;
    emitterTypes[10].unk_40 = 0.0f;
    emitterTypes[10].unk_44 = 0;
    emitterTypes[10].unk_48 = 0;
    emitterTypes[10].unk_4C = 0;
    emitterTypes[10].unk_50 = 0;
    emitterTypes[10].unk_54 = 400.0f;
    emitterTypes[10].unk_58 = 0;
    emitterTypes[10].unk_5C = 0;
    emitterTypes[10].unk_60 = 0;
    emitterTypes[10].unk_64 = 400.0f;
    emitterTypes[10].unk_68 = 0;
    emitterTypes[10].unk_6C = 0;
    emitterTypes[10].unk_70 = 0;
    emitterTypes[10].unk_74 = 400.0f;
    emitterTypes[10].unk_78 = 0;
    emitterTypes[10].unk_7C = 0;
    emitterTypes[10].unk_80 = -185.0f;
    emitterTypes[10].unk_84 = 1.0f;
    emitterTypes[10].unk_88 = 1.0f;
    emitterTypes[10].unk_8C = 0.391f;
    emitterTypes[10].unk_90 = 0.059f;
    emitterTypes[10].unk_94 = 0.0f;
    emitterTypes[10].unk_98 = 1.0f;
    emitterTypes[10].unk_9C = 0.97275f;
    emitterTypes[10].unk_A0 = 0.0f;
    emitterTypes[10].unk_A4 = 0;
    emitterTypes[10].unk_A8 = 0.0f;
    emitterTypes[10].unk_AC = 0.0f;
    emitterTypes[10].unk_B0 = 0.0f;
    emitterTypes[10].unk_B4 = 0;
    emitterTypes[10].unk_B8 = 0.3515f;
    emitterTypes[10].unk_BC = 0.0f;
    emitterTypes[10].unk_C0 = 0.0f;
    emitterTypes[10].unk_C8 = 0;
    emitterTypes[10].unk_CC = 0.0f;
    emitterTypes[10].unk_C4 = 41;
    emitterTypes[10].unk_D0 = 1;
    emitterTypes[10].unk_D4 = 20.0f;
    emitterTypes[10].unk_D8 = 1.2f;
    emitterTypes[10].unk_DC = 0;
    emitterTypes[10].unk_E0 = 0;
    emitterTypes[10].unk_E4 = 16.25f;
    emitterTypes[10].unk_E8 = 3.0f;
    emitterTypes[10].unk_EC = 18.0f;
    emitterTypes[10].unk_F0 = 0.0f;
    emitterTypes[10].unk_F4 = 0;
    emitterTypes[10].unk_F8 = 0.0f;
    emitterTypes[10].unk_FC = 0.0f;
    emitterTypes[10].unk_100 = 140.0f;
    emitterTypes[10].unk_104 = 1.0f;
    emitterTypes[10].unk_108 = 0.45f;
    emitterTypes[10].unk_10C = 0.7f;
    emitterTypes[11].unk_0 = 10;
    emitterTypes[11].unk_4 = 0;
    emitterTypes[11].unk_8 = -1.0f;
    emitterTypes[11].unk_C = 1.0f;
    emitterTypes[11].unk_10 = 4.0f;
    emitterTypes[11].unk_14 = 1.2f;
    emitterTypes[11].unk_18 = 5.0f;
    emitterTypes[11].unk_1C = 2.0f;
    emitterTypes[11].unk_20 = 0.0f;
    emitterTypes[11].unk_24 = 0;
    emitterTypes[11].unk_28 = 0;
    emitterTypes[11].unk_2C = 0;
    emitterTypes[11].unk_30 = 17.75f;
    emitterTypes[11].unk_34 = 0;
    emitterTypes[11].unk_38 = 0.0f;
    emitterTypes[11].unk_3C = 0.0f;
    emitterTypes[11].unk_40 = 17.75f;
    emitterTypes[11].unk_44 = 0;
    emitterTypes[11].unk_48 = 0;
    emitterTypes[11].unk_4C = 0;
    emitterTypes[11].unk_50 = 0;
    emitterTypes[11].unk_54 = 75.0f;
    emitterTypes[11].unk_58 = 0;
    emitterTypes[11].unk_5C = 0;
    emitterTypes[11].unk_60 = 0;
    emitterTypes[11].unk_64 = 75.0f;
    emitterTypes[11].unk_68 = 0;
    emitterTypes[11].unk_6C = 0;
    emitterTypes[11].unk_70 = 0;
    emitterTypes[11].unk_74 = 120.0f;
    emitterTypes[11].unk_78 = 0;
    emitterTypes[11].unk_7C = 0;
    emitterTypes[11].unk_80 = 10.0f;
    emitterTypes[11].unk_84 = 0.56375f;
    emitterTypes[11].unk_88 = 0.76525f;
    emitterTypes[11].unk_8C = 0.75725f;
    emitterTypes[11].unk_90 = 0.7755f;
    emitterTypes[11].unk_94 = 0.0f;
    emitterTypes[11].unk_98 = 1.0f;
    emitterTypes[11].unk_9C = 1.0f;
    emitterTypes[11].unk_A0 = 1.0f;
    emitterTypes[11].unk_A4 = 0;
    emitterTypes[11].unk_A8 = 0.02975f;
    emitterTypes[11].unk_AC = 0.02975f;
    emitterTypes[11].unk_B0 = 0.0f;
    emitterTypes[11].unk_B4 = 0;
    emitterTypes[11].unk_B8 = 0.4395f;
    emitterTypes[11].unk_BC = 0.43625f;
    emitterTypes[11].unk_C0 = 0.4485f;
    emitterTypes[11].unk_C8 = 1;
    emitterTypes[11].unk_CC = 70.0f;
    emitterTypes[11].unk_C4 = 5;
    emitterTypes[11].unk_D0 = 1;
    emitterTypes[11].unk_D4 = 20.0f;
    emitterTypes[11].unk_D8 = 1.1f;
    emitterTypes[11].unk_DC = 0;
    emitterTypes[11].unk_E0 = 0;
    emitterTypes[11].unk_E4 = 75.0f;
    emitterTypes[11].unk_E8 = 0.0f;
    emitterTypes[11].unk_EC = -60.0f;
    emitterTypes[11].unk_F0 = 172.5f;
    emitterTypes[11].unk_F4 = 0;
    emitterTypes[11].unk_F8 = -223.0f;
    emitterTypes[11].unk_FC = 0.0f;
    emitterTypes[11].unk_100 = 55.0f;
    emitterTypes[11].unk_104 = 1.0f;
    emitterTypes[11].unk_108 = 0.42f;
    emitterTypes[11].unk_10C = 0.2f;
    emitterTypes[12].unk_0 = 8;
    emitterTypes[12].unk_4 = 4;
    emitterTypes[12].unk_8 = -1.0f;
    emitterTypes[12].unk_C = 1.0f;
    emitterTypes[12].unk_10 = 1e-05f;
    emitterTypes[12].unk_14 = 0.01f;
    emitterTypes[12].unk_18 = 10.112f;
    emitterTypes[12].unk_1C = 0.0f;
    emitterTypes[12].unk_20 = 0.001f;
    emitterTypes[12].unk_24 = 0;
    emitterTypes[12].unk_28 = 0;
    emitterTypes[12].unk_2C = 0;
    emitterTypes[12].unk_30 = 0.0f;
    emitterTypes[12].unk_34 = 0;
    emitterTypes[12].unk_38 = 0.0f;
    emitterTypes[12].unk_3C = 0.0f;
    emitterTypes[12].unk_40 = 0.0f;
    emitterTypes[12].unk_44 = 0;
    emitterTypes[12].unk_48 = 0;
    emitterTypes[12].unk_4C = 0;
    emitterTypes[12].unk_50 = 0;
    emitterTypes[12].unk_54 = 0.0f;
    emitterTypes[12].unk_58 = 0;
    emitterTypes[12].unk_5C = 0;
    emitterTypes[12].unk_60 = 0;
    emitterTypes[12].unk_64 = 0.0f;
    emitterTypes[12].unk_68 = 0;
    emitterTypes[12].unk_6C = 0;
    emitterTypes[12].unk_70 = 0;
    emitterTypes[12].unk_74 = 0.0f;
    emitterTypes[12].unk_78 = 0;
    emitterTypes[12].unk_7C = 0;
    emitterTypes[12].unk_80 = 0.0f;
    emitterTypes[12].unk_84 = 1.0f;
    emitterTypes[12].unk_88 = 1.0f;
    emitterTypes[12].unk_8C = 0.37575f;
    emitterTypes[12].unk_90 = 0.21975f;
    emitterTypes[12].unk_94 = 0.0f;
    emitterTypes[12].unk_98 = 1.0f;
    emitterTypes[12].unk_9C = 0.97275f;
    emitterTypes[12].unk_A0 = 0.0f;
    emitterTypes[12].unk_A4 = 0;
    emitterTypes[12].unk_A8 = 0.045f;
    emitterTypes[12].unk_AC = 0.045f;
    emitterTypes[12].unk_B0 = 0.0485f;
    emitterTypes[12].unk_B4 = 0;
    emitterTypes[12].unk_B8 = 0.2725f;
    emitterTypes[12].unk_BC = 0.0f;
    emitterTypes[12].unk_C0 = 0.0f;
    emitterTypes[12].unk_C8 = 0;
    emitterTypes[12].unk_CC = 0.0f;
    emitterTypes[12].unk_C4 = 41;
    emitterTypes[12].unk_D0 = 1;
    emitterTypes[12].unk_D4 = 20.0f;
    emitterTypes[12].unk_D8 = 1.2f;
    emitterTypes[12].unk_DC = 0;
    emitterTypes[12].unk_E0 = 0;
    emitterTypes[12].unk_E4 = 16.25f;
    emitterTypes[12].unk_E8 = 3.0f;
    emitterTypes[12].unk_EC = 18.0f;
    emitterTypes[12].unk_F0 = 0.0f;
    emitterTypes[12].unk_F4 = 0;
    emitterTypes[12].unk_F8 = 0.0f;
    emitterTypes[12].unk_FC = 0.0f;
    emitterTypes[12].unk_100 = 140.0f;
    emitterTypes[12].unk_104 = 1.0f;
    emitterTypes[12].unk_108 = 1.0f;
    emitterTypes[12].unk_10C = 1.0f;
    emitterTypes[13].unk_0 = 15;
    emitterTypes[13].unk_4 = 0;
    emitterTypes[13].unk_8 = -1.0f;
    emitterTypes[13].unk_C = 1.0f;
    emitterTypes[13].unk_10 = 2.0f;
    emitterTypes[13].unk_14 = 1.2f;
    emitterTypes[13].unk_18 = 2.5f;
    emitterTypes[13].unk_1C = 2.0f;
    emitterTypes[13].unk_20 = 0.0f;
    emitterTypes[13].unk_24 = 0;
    emitterTypes[13].unk_28 = 0;
    emitterTypes[13].unk_2C = 0;
    emitterTypes[13].unk_30 = 17.75f;
    emitterTypes[13].unk_34 = 0;
    emitterTypes[13].unk_38 = 0.0f;
    emitterTypes[13].unk_3C = 0.0f;
    emitterTypes[13].unk_40 = 17.75f;
    emitterTypes[13].unk_44 = 0;
    emitterTypes[13].unk_48 = 0;
    emitterTypes[13].unk_4C = 0;
    emitterTypes[13].unk_50 = 0;
    emitterTypes[13].unk_54 = 87.5f;
    emitterTypes[13].unk_58 = 0;
    emitterTypes[13].unk_5C = 0;
    emitterTypes[13].unk_60 = 0;
    emitterTypes[13].unk_64 = 152.5f;
    emitterTypes[13].unk_68 = 0;
    emitterTypes[13].unk_6C = 0;
    emitterTypes[13].unk_70 = 0;
    emitterTypes[13].unk_74 = 100.0f;
    emitterTypes[13].unk_78 = 0;
    emitterTypes[13].unk_7C = 0;
    emitterTypes[13].unk_80 = 10.0f;
    emitterTypes[13].unk_84 = 0.5755f;
    emitterTypes[13].unk_88 = 0.66125f;
    emitterTypes[13].unk_8C = 0.6485f;
    emitterTypes[13].unk_90 = 0.66825f;
    emitterTypes[13].unk_94 = 0.0f;
    emitterTypes[13].unk_98 = 0.2725f;
    emitterTypes[13].unk_9C = 0.2725f;
    emitterTypes[13].unk_A0 = 0.28475f;
    emitterTypes[13].unk_A4 = 0;
    emitterTypes[13].unk_A8 = 0.02975f;
    emitterTypes[13].unk_AC = 0.02975f;
    emitterTypes[13].unk_B0 = 0.0f;
    emitterTypes[13].unk_B4 = 0;
    emitterTypes[13].unk_B8 = 0.24525f;
    emitterTypes[13].unk_BC = 0.24525f;
    emitterTypes[13].unk_C0 = 0.26675f;
    emitterTypes[13].unk_C8 = 1;
    emitterTypes[13].unk_CC = 200.0f;
    emitterTypes[13].unk_C4 = 5;
    emitterTypes[13].unk_D0 = 1;
    emitterTypes[13].unk_D4 = 20.0f;
    emitterTypes[13].unk_D8 = 1.2f;
    emitterTypes[13].unk_DC = 0;
    emitterTypes[13].unk_E0 = 0;
    emitterTypes[13].unk_E4 = 75.0f;
    emitterTypes[13].unk_E8 = 0.0f;
    emitterTypes[13].unk_EC = -60.0f;
    emitterTypes[13].unk_F0 = 172.5f;
    emitterTypes[13].unk_F4 = 0;
    emitterTypes[13].unk_F8 = -223.0f;
    emitterTypes[13].unk_FC = 55.0f;
    emitterTypes[13].unk_100 = 105.0f;
    emitterTypes[13].unk_104 = 1.0f;
    emitterTypes[13].unk_108 = 0.5f;
    emitterTypes[13].unk_10C = 0.3f;
    emitterTypes[14].unk_0 = 10;
    emitterTypes[14].unk_4 = 0;
    emitterTypes[14].unk_8 = -1.0f;
    emitterTypes[14].unk_C = 1.0f;
    emitterTypes[14].unk_10 = 3.52001f;
    emitterTypes[14].unk_14 = 1.2f;
    emitterTypes[14].unk_18 = 2.24f;
    emitterTypes[14].unk_1C = 2.3f;
    emitterTypes[14].unk_20 = 0.0f;
    emitterTypes[14].unk_24 = 0;
    emitterTypes[14].unk_28 = 0;
    emitterTypes[14].unk_2C = 0;
    emitterTypes[14].unk_30 = 17.75f;
    emitterTypes[14].unk_34 = 0;
    emitterTypes[14].unk_38 = 0.0f;
    emitterTypes[14].unk_3C = 0.0f;
    emitterTypes[14].unk_40 = 17.75f;
    emitterTypes[14].unk_44 = 0;
    emitterTypes[14].unk_48 = 0;
    emitterTypes[14].unk_4C = 0;
    emitterTypes[14].unk_50 = 0;
    emitterTypes[14].unk_54 = 87.5f;
    emitterTypes[14].unk_58 = 0;
    emitterTypes[14].unk_5C = 0;
    emitterTypes[14].unk_60 = 0;
    emitterTypes[14].unk_64 = 152.5f;
    emitterTypes[14].unk_68 = 0;
    emitterTypes[14].unk_6C = 0;
    emitterTypes[14].unk_70 = 0;
    emitterTypes[14].unk_74 = 270.0f;
    emitterTypes[14].unk_78 = 0;
    emitterTypes[14].unk_7C = 0;
    emitterTypes[14].unk_80 = 10.0f;
    emitterTypes[14].unk_84 = 0.33f;
    emitterTypes[14].unk_88 = 0.36125f;
    emitterTypes[14].unk_8C = 0.3485f;
    emitterTypes[14].unk_90 = 0.391f;
    emitterTypes[14].unk_94 = 0.0f;
    emitterTypes[14].unk_98 = 0.0f;
    emitterTypes[14].unk_9C = 0.0f;
    emitterTypes[14].unk_A0 = 0.0f;
    emitterTypes[14].unk_A4 = 0;
    emitterTypes[14].unk_A8 = 0.1295f;
    emitterTypes[14].unk_AC = 0.1295f;
    emitterTypes[14].unk_B0 = 0.0f;
    emitterTypes[14].unk_B4 = 0;
    emitterTypes[14].unk_B8 = 0.44525f;
    emitterTypes[14].unk_BC = 0.44525f;
    emitterTypes[14].unk_C0 = 0.4665f;
    emitterTypes[14].unk_C8 = 1;
    emitterTypes[14].unk_CC = 300.0f;
    emitterTypes[14].unk_C4 = 5;
    emitterTypes[14].unk_D0 = 1;
    emitterTypes[14].unk_D4 = 20.0f;
    emitterTypes[14].unk_D8 = 1.1f;
    emitterTypes[14].unk_DC = 0;
    emitterTypes[14].unk_E0 = 0;
    emitterTypes[14].unk_E4 = 75.0f;
    emitterTypes[14].unk_E8 = 0.0f;
    emitterTypes[14].unk_EC = -60.0f;
    emitterTypes[14].unk_F0 = 200.0f;
    emitterTypes[14].unk_F4 = 0;
    emitterTypes[14].unk_F8 = -273.0f;
    emitterTypes[14].unk_FC = 105.0f;
    emitterTypes[14].unk_100 = 140.0f;
    emitterTypes[14].unk_104 = 1.0f;
    emitterTypes[14].unk_108 = 1.0f;
    emitterTypes[14].unk_10C = 0.4f;
    emitterTypes[15].unk_0 = 1;
    emitterTypes[15].unk_4 = 0;
    emitterTypes[15].unk_8 = -1.0f;
    emitterTypes[15].unk_C = 1.0f;
    emitterTypes[15].unk_10 = 4.0f;
    emitterTypes[15].unk_14 = 1.0f;
    emitterTypes[15].unk_18 = 0.0f;
    emitterTypes[15].unk_1C = 0.0f;
    emitterTypes[15].unk_20 = 0.0f;
    emitterTypes[15].unk_24 = 0;
    emitterTypes[15].unk_28 = 0;
    emitterTypes[15].unk_2C = 0;
    emitterTypes[15].unk_30 = 0.0f;
    emitterTypes[15].unk_34 = 0;
    emitterTypes[15].unk_38 = 0.0f;
    emitterTypes[15].unk_3C = 0.0f;
    emitterTypes[15].unk_40 = 0.0f;
    emitterTypes[15].unk_44 = 0;
    emitterTypes[15].unk_48 = 0;
    emitterTypes[15].unk_4C = 0;
    emitterTypes[15].unk_50 = 0;
    emitterTypes[15].unk_54 = 0.0f;
    emitterTypes[15].unk_58 = 0;
    emitterTypes[15].unk_5C = 0;
    emitterTypes[15].unk_60 = 0;
    emitterTypes[15].unk_64 = 0.0f;
    emitterTypes[15].unk_68 = 0;
    emitterTypes[15].unk_6C = 0;
    emitterTypes[15].unk_70 = 0;
    emitterTypes[15].unk_74 = 0.0f;
    emitterTypes[15].unk_78 = 0;
    emitterTypes[15].unk_7C = 0;
    emitterTypes[15].unk_80 = 0.0f;
    emitterTypes[15].unk_84 = 1.0f;
    emitterTypes[15].unk_88 = 1.0f;
    emitterTypes[15].unk_8C = 1.0f;
    emitterTypes[15].unk_90 = 1.0f;
    emitterTypes[15].unk_94 = 1.0f;
    emitterTypes[15].unk_98 = 1.0f;
    emitterTypes[15].unk_9C = 1.0f;
    emitterTypes[15].unk_A0 = 1.0f;
    emitterTypes[15].unk_A4 = 0;
    emitterTypes[15].unk_A8 = 0.0f;
    emitterTypes[15].unk_AC = 0.0f;
    emitterTypes[15].unk_B0 = 0.0f;
    emitterTypes[15].unk_B4 = 0;
    emitterTypes[15].unk_B8 = 0.0f;
    emitterTypes[15].unk_BC = 0.0f;
    emitterTypes[15].unk_C0 = 0.0f;
    emitterTypes[15].unk_C8 = 1;
    emitterTypes[15].unk_CC = 0.0f;
    emitterTypes[15].unk_C4 = 5;
    emitterTypes[15].unk_D0 = 1;
    emitterTypes[15].unk_D4 = 20.0f;
    emitterTypes[15].unk_D8 = 0.0f;
    emitterTypes[15].unk_DC = 0;
    emitterTypes[15].unk_E0 = 0;
    emitterTypes[15].unk_E4 = 0.0f;
    emitterTypes[15].unk_E8 = 0.0f;
    emitterTypes[15].unk_EC = 0.0f;
    emitterTypes[15].unk_F0 = 0.0f;
    emitterTypes[15].unk_F4 = 0;
    emitterTypes[15].unk_F8 = 0.0f;
    emitterTypes[15].unk_FC = 0.0f;
    emitterTypes[15].unk_100 = 140.0f;
    emitterTypes[15].unk_104 = 1.0f;
    emitterTypes[15].unk_108 = 1.0f;
    emitterTypes[15].unk_10C = 1.0f;
    emitterTypes[16].unk_0 = 3;
    emitterTypes[16].unk_4 = 0;
    emitterTypes[16].unk_8 = -1.0f;
    emitterTypes[16].unk_C = 1.0f;
    emitterTypes[16].unk_10 = 9.21601f;
    emitterTypes[16].unk_14 = 0.09001f;
    emitterTypes[16].unk_18 = 12.416f;
    emitterTypes[16].unk_1C = 0.07625f;
    emitterTypes[16].unk_20 = 0.0f;
    emitterTypes[16].unk_24 = 0;
    emitterTypes[16].unk_28 = 0;
    emitterTypes[16].unk_2C = 0;
    emitterTypes[16].unk_30 = 0.1f;
    emitterTypes[16].unk_34 = 0;
    emitterTypes[16].unk_38 = 0.0f;
    emitterTypes[16].unk_3C = 0.0f;
    emitterTypes[16].unk_40 = 0.1f;
    emitterTypes[16].unk_44 = 0;
    emitterTypes[16].unk_48 = 0;
    emitterTypes[16].unk_4C = 0;
    emitterTypes[16].unk_50 = 0;
    emitterTypes[16].unk_54 = 20.0f;
    emitterTypes[16].unk_58 = 0;
    emitterTypes[16].unk_5C = 0;
    emitterTypes[16].unk_60 = 0;
    emitterTypes[16].unk_64 = 20.0f;
    emitterTypes[16].unk_68 = 0;
    emitterTypes[16].unk_6C = 0;
    emitterTypes[16].unk_70 = 0;
    emitterTypes[16].unk_74 = 20.0f;
    emitterTypes[16].unk_78 = 0;
    emitterTypes[16].unk_7C = 0;
    emitterTypes[16].unk_80 = 0.0f;
    emitterTypes[16].unk_84 = 0.45425f;
    emitterTypes[16].unk_88 = 0.0f;
    emitterTypes[16].unk_8C = 0.0f;
    emitterTypes[16].unk_90 = 1.0f;
    emitterTypes[16].unk_94 = 0.0f;
    emitterTypes[16].unk_98 = 1.0f;
    emitterTypes[16].unk_9C = 0.62725f;
    emitterTypes[16].unk_A0 = 0.98175f;
    emitterTypes[16].unk_A4 = 0;
    emitterTypes[16].unk_A8 = 0.0f;
    emitterTypes[16].unk_AC = 0.0f;
    emitterTypes[16].unk_B0 = 0.0f;
    emitterTypes[16].unk_B4 = 0;
    emitterTypes[16].unk_B8 = 0.0f;
    emitterTypes[16].unk_BC = 0.0f;
    emitterTypes[16].unk_C0 = 0.0f;
    emitterTypes[16].unk_C8 = 0;
    emitterTypes[16].unk_CC = 0.0f;
    emitterTypes[16].unk_C4 = 5;
    emitterTypes[16].unk_D0 = 1;
    emitterTypes[16].unk_D4 = 20.0f;
    emitterTypes[16].unk_D8 = 1.2f;
    emitterTypes[16].unk_DC = 0;
    emitterTypes[16].unk_E0 = 0;
    emitterTypes[16].unk_E4 = 9.0f;
    emitterTypes[16].unk_E8 = 6.25f;
    emitterTypes[16].unk_EC = 9.0f;
    emitterTypes[16].unk_F0 = 0.0f;
    emitterTypes[16].unk_F4 = 0;
    emitterTypes[16].unk_F8 = 0.0f;
    emitterTypes[16].unk_FC = 0.0f;
    emitterTypes[16].unk_100 = 140.0f;
    emitterTypes[16].unk_104 = 1.0f;
    emitterTypes[16].unk_108 = 0.45f;
    emitterTypes[16].unk_10C = 0.24f;
    emitterTypes[17].unk_0 = 8;
    emitterTypes[17].unk_4 = 0;
    emitterTypes[17].unk_8 = -1.0f;
    emitterTypes[17].unk_C = 1.0f;
    emitterTypes[17].unk_10 = 19.39201f;
    emitterTypes[17].unk_14 = 0.1f;
    emitterTypes[17].unk_18 = 36.416f;
    emitterTypes[17].unk_1C = 0.09f;
    emitterTypes[17].unk_20 = 0.0f;
    emitterTypes[17].unk_24 = 0;
    emitterTypes[17].unk_28 = 0;
    emitterTypes[17].unk_2C = 0;
    emitterTypes[17].unk_30 = 3.0f;
    emitterTypes[17].unk_34 = 0;
    emitterTypes[17].unk_38 = 0.0f;
    emitterTypes[17].unk_3C = 0.0f;
    emitterTypes[17].unk_40 = 3.0f;
    emitterTypes[17].unk_44 = 0;
    emitterTypes[17].unk_48 = 0;
    emitterTypes[17].unk_4C = 0;
    emitterTypes[17].unk_50 = 0;
    emitterTypes[17].unk_54 = 100.0f;
    emitterTypes[17].unk_58 = 0;
    emitterTypes[17].unk_5C = 0;
    emitterTypes[17].unk_60 = 0;
    emitterTypes[17].unk_64 = 100.0f;
    emitterTypes[17].unk_68 = 0;
    emitterTypes[17].unk_6C = 0;
    emitterTypes[17].unk_70 = 0;
    emitterTypes[17].unk_74 = 100.0f;
    emitterTypes[17].unk_78 = 0;
    emitterTypes[17].unk_7C = 0;
    emitterTypes[17].unk_80 = 0.0f;
    emitterTypes[17].unk_84 = 0.4605f;
    emitterTypes[17].unk_88 = 0.0f;
    emitterTypes[17].unk_8C = 0.0f;
    emitterTypes[17].unk_90 = 0.0f;
    emitterTypes[17].unk_94 = 0.0f;
    emitterTypes[17].unk_98 = 1.0f;
    emitterTypes[17].unk_9C = 1.0f;
    emitterTypes[17].unk_A0 = 1.0f;
    emitterTypes[17].unk_A4 = 0;
    emitterTypes[17].unk_A8 = 1.0f;
    emitterTypes[17].unk_AC = 0.0f;
    emitterTypes[17].unk_B0 = 0.0f;
    emitterTypes[17].unk_B4 = 0;
    emitterTypes[17].unk_B8 = 0.0f;
    emitterTypes[17].unk_BC = 1.0f;
    emitterTypes[17].unk_C0 = 0.0f;
    emitterTypes[17].unk_C8 = 0;
    emitterTypes[17].unk_CC = 1.75f;
    emitterTypes[17].unk_C4 = 38;
    emitterTypes[17].unk_D0 = 1;
    emitterTypes[17].unk_D4 = 20.0f;
    emitterTypes[17].unk_D8 = 1.2f;
    emitterTypes[17].unk_DC = 0;
    emitterTypes[17].unk_E0 = 0;
    emitterTypes[17].unk_E4 = 36.0f;
    emitterTypes[17].unk_E8 = -15.25f;
    emitterTypes[17].unk_EC = -12.0f;
    emitterTypes[17].unk_F0 = 0.0f;
    emitterTypes[17].unk_F4 = 0;
    emitterTypes[17].unk_F8 = 0.0f;
    emitterTypes[17].unk_FC = 0.0f;
    emitterTypes[17].unk_100 = 140.0f;
    emitterTypes[17].unk_104 = 1.0f;
    emitterTypes[17].unk_108 = 0.7425f;
    emitterTypes[17].unk_10C = 4.25f;
    emitterTypes[18].unk_0 = 3;
    emitterTypes[18].unk_4 = 0;
    emitterTypes[18].unk_8 = -1.0f;
    emitterTypes[18].unk_C = 1.0f;
    emitterTypes[18].unk_10 = 16.25601f;
    emitterTypes[18].unk_14 = 0.27251f;
    emitterTypes[18].unk_18 = 11.968f;
    emitterTypes[18].unk_1C = 0.13625f;
    emitterTypes[18].unk_20 = 0.0f;
    emitterTypes[18].unk_24 = 0;
    emitterTypes[18].unk_28 = 0;
    emitterTypes[18].unk_2C = 0;
    emitterTypes[18].unk_30 = 0.0f;
    emitterTypes[18].unk_34 = 0;
    emitterTypes[18].unk_38 = 10.0f;
    emitterTypes[18].unk_3C = 10.0f;
    emitterTypes[18].unk_40 = 10.0f;
    emitterTypes[18].unk_44 = 0;
    emitterTypes[18].unk_48 = 0;
    emitterTypes[18].unk_4C = 0;
    emitterTypes[18].unk_50 = 0;
    emitterTypes[18].unk_54 = 177.5f;
    emitterTypes[18].unk_58 = 0;
    emitterTypes[18].unk_5C = 0;
    emitterTypes[18].unk_60 = 0;
    emitterTypes[18].unk_64 = 242.5f;
    emitterTypes[18].unk_68 = 0;
    emitterTypes[18].unk_6C = 0;
    emitterTypes[18].unk_70 = 0;
    emitterTypes[18].unk_74 = 242.5f;
    emitterTypes[18].unk_78 = 0;
    emitterTypes[18].unk_7C = 0;
    emitterTypes[18].unk_80 = -240.0f;
    emitterTypes[18].unk_84 = 1.0f;
    emitterTypes[18].unk_88 = 0.90275f;
    emitterTypes[18].unk_8C = 0.545f;
    emitterTypes[18].unk_90 = 0.0f;
    emitterTypes[18].unk_94 = 0.0f;
    emitterTypes[18].unk_98 = 0.16975f;
    emitterTypes[18].unk_9C = 0.00875f;
    emitterTypes[18].unk_A0 = 0.0f;
    emitterTypes[18].unk_A4 = 0;
    emitterTypes[18].unk_A8 = 0.0f;
    emitterTypes[18].unk_AC = 0.0f;
    emitterTypes[18].unk_B0 = 0.1665f;
    emitterTypes[18].unk_B4 = 0;
    emitterTypes[18].unk_B8 = 0.2425f;
    emitterTypes[18].unk_BC = 0.0f;
    emitterTypes[18].unk_C0 = 0.0f;
    emitterTypes[18].unk_C8 = 0;
    emitterTypes[18].unk_CC = 2.4f;
    emitterTypes[18].unk_C4 = 41;
    emitterTypes[18].unk_D0 = 1;
    emitterTypes[18].unk_D4 = 20.0f;
    emitterTypes[18].unk_D8 = 1.5f;
    emitterTypes[18].unk_DC = 0;
    emitterTypes[18].unk_E0 = 0;
    emitterTypes[18].unk_E4 = 15.25f;
    emitterTypes[18].unk_E8 = -9.25f;
    emitterTypes[18].unk_EC = 0.0f;
    emitterTypes[18].unk_F0 = 0.0f;
    emitterTypes[18].unk_F4 = 0;
    emitterTypes[18].unk_F8 = 0.0f;
    emitterTypes[18].unk_FC = 0.0f;
    emitterTypes[18].unk_100 = 140.0f;
    emitterTypes[18].unk_104 = 1.0f;
    emitterTypes[18].unk_108 = 1.0f;
    emitterTypes[18].unk_10C = 1.0f;
    emitterTypes[19].unk_0 = 3;
    emitterTypes[19].unk_4 = 0;
    emitterTypes[19].unk_8 = -1.0f;
    emitterTypes[19].unk_C = 1.0f;
    emitterTypes[19].unk_10 = 9.21601f;
    emitterTypes[19].unk_14 = 0.09001f;
    emitterTypes[19].unk_18 = 12.416f;
    emitterTypes[19].unk_1C = 0.07625f;
    emitterTypes[19].unk_20 = 0.0f;
    emitterTypes[19].unk_24 = 0;
    emitterTypes[19].unk_28 = 0;
    emitterTypes[19].unk_2C = 0;
    emitterTypes[19].unk_30 = 0.1f;
    emitterTypes[19].unk_34 = 0;
    emitterTypes[19].unk_38 = 0.0f;
    emitterTypes[19].unk_3C = 0.0f;
    emitterTypes[19].unk_40 = 0.1f;
    emitterTypes[19].unk_44 = 0;
    emitterTypes[19].unk_48 = 0;
    emitterTypes[19].unk_4C = 0;
    emitterTypes[19].unk_50 = 0;
    emitterTypes[19].unk_54 = 20.0f;
    emitterTypes[19].unk_58 = 0;
    emitterTypes[19].unk_5C = 0;
    emitterTypes[19].unk_60 = 0;
    emitterTypes[19].unk_64 = 20.0f;
    emitterTypes[19].unk_68 = 0;
    emitterTypes[19].unk_6C = 0;
    emitterTypes[19].unk_70 = 0;
    emitterTypes[19].unk_74 = 20.0f;
    emitterTypes[19].unk_78 = 0;
    emitterTypes[19].unk_7C = 0;
    emitterTypes[19].unk_80 = 0.0f;
    emitterTypes[19].unk_84 = 0.45425f;
    emitterTypes[19].unk_88 = 0.0f;
    emitterTypes[19].unk_8C = 0.0f;
    emitterTypes[19].unk_90 = 1.0f;
    emitterTypes[19].unk_94 = 0.0f;
    emitterTypes[19].unk_98 = 1.0f;
    emitterTypes[19].unk_9C = 0.62725f;
    emitterTypes[19].unk_A0 = 0.98175f;
    emitterTypes[19].unk_A4 = 0;
    emitterTypes[19].unk_A8 = 0.0f;
    emitterTypes[19].unk_AC = 0.0f;
    emitterTypes[19].unk_B0 = 0.0f;
    emitterTypes[19].unk_B4 = 0;
    emitterTypes[19].unk_B8 = 0.0f;
    emitterTypes[19].unk_BC = 0.0f;
    emitterTypes[19].unk_C0 = 0.0f;
    emitterTypes[19].unk_C8 = 0;
    emitterTypes[19].unk_CC = 0.0f;
    emitterTypes[19].unk_C4 = 5;
    emitterTypes[19].unk_D0 = 1;
    emitterTypes[19].unk_D4 = 20.0f;
    emitterTypes[19].unk_D8 = 1.2f;
    emitterTypes[19].unk_DC = 0;
    emitterTypes[19].unk_E0 = 0;
    emitterTypes[19].unk_E4 = 9.0f;
    emitterTypes[19].unk_E8 = -6.25f;
    emitterTypes[19].unk_EC = 9.0f;
    emitterTypes[19].unk_F0 = 0.0f;
    emitterTypes[19].unk_F4 = 0;
    emitterTypes[19].unk_F8 = 0.0f;
    emitterTypes[19].unk_FC = 0.0f;
    emitterTypes[19].unk_100 = 140.0f;
    emitterTypes[19].unk_104 = 1.0f;
    emitterTypes[19].unk_108 = 0.47275f;
    emitterTypes[19].unk_10C = 0.2f;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA5F8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * func_002F1810 calls it after func_002F8FF8__FPv, before freeing the attached-emitter type table cAttachedEmitterFX_mEmitterDataArray.
 */
void func_002FA5F8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA600);
#ifdef SKIP_ASM
/**
 * Returns @p self unchanged.
 *
 * The definition is variadic, so it first spills the argument registers. Nothing in the retail code references it.
 *
 * @return @p self
 */
extern "C" void* func_002FA600(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/worldlightman", func_002FA640);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_2FA640 {
    float r, g, b, a;
    sColor_2FA640(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

extern sColor_2FA640 D_004FB010;
extern sColor_2FA640 D_004FB020;
extern sColor_2FA640 D_004FB030;
extern sColor_2FA640 D_004FB040;
extern sColor_2FA640 D_004FB050;
extern sColor_2FA640 D_004FB060;
extern sColor_2FA640 D_004FB070;
extern sColor_2FA640 D_004FB080;
extern sColor_2FA640 D_004FB090;
extern sColor_2FA640 D_004FB0A0;
extern sColor_2FA640 D_004FB0B0;
extern sColor_2FA640 D_004FB0C0;
extern sColor_2FA640 D_004FB0D0;
extern sColor_2FA640 D_004FB0E0;
extern sColor_2FA640 D_004FB0F0;
extern sColor_2FA640 D_004FB100;
extern sColor_2FA640 D_004FB110;
extern sColor_2FA640 D_004FB120;
extern sColor_2FA640 D_004FB130;
extern sColor_2FA640 D_004FB140;
extern sColor_2FA640 D_004FB150;
extern sColor_2FA640 D_004FB160;
extern sColor_2FA640 D_004FB170;
extern sColor_2FA640 D_004FB180;
extern sColor_2FA640 D_004FB190;
extern sColor_2FA640 D_004FB1A0;
extern sColor_2FA640 D_004FB1B0;
extern sColor_2FA640 D_004FB1C0;
extern sColor_2FA640 D_004FB1D0;
extern sColor_2FA640 D_004FB1E0;
extern sColor_2FA640 D_004FB1F0;
extern sColor_2FA640 D_004FB200;
extern sColor_2FA640 D_004FB210;
extern sColor_2FA640 D_004FB220;
extern sColor_2FA640 D_004FB230;
extern sColor_2FA640 D_004FB240;
extern sColor_2FA640 D_004FB250;
extern sColor_2FA640 D_004FB260;
extern sColor_2FA640 D_004FB270;
extern sColor_2FA640 D_004FB280;
extern sColor_2FA640 D_004FB290;
extern sColor_2FA640 D_004FB2A0;
extern sColor_2FA640 D_004FB2B0;
extern sColor_2FA640 D_004FB2C0;
extern sColor_2FA640 D_004FB2D0;
extern sColor_2FA640 D_004FB2E0;
extern sColor_2FA640 D_004FB2F0;
extern sColor_2FA640 D_004FB300;
extern sColor_2FA640 D_004FB310;
extern sColor_2FA640 D_004FB320;
extern sColor_2FA640 D_004FB330;
extern sColor_2FA640 D_004FB340;
extern sColor_2FA640 D_004FB350;
extern sColor_2FA640 D_004FB360;
extern sColor_2FA640 D_004FB370;
extern sColor_2FA640 D_004FB380;

// The unit declares func_002FA640 later as returning void*; bind this body by asm label.
extern "C" void func_002FA640_impl(int initialize, int priority) __asm__("func_002FA640");

/**
 * Runs the static constructors of a third copy of the visual-effects colour presets (56 colours from D_004FB010).
 *
 * Acts only for @p priority 0xFFFF with @p initialize set; the same presets as func_002F9040. func_002FAE18__FPv calls
 * it.
 *
 * @param initialize  nonzero to construct; 0 does nothing
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_002FA640_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004FB010) sColor_2FA640(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FB020) sColor_2FA640(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB030) sColor_2FA640(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB040) sColor_2FA640(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FB050) sColor_2FA640(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FB060) sColor_2FA640(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FB070) sColor_2FA640(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FB080) sColor_2FA640(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FB090) sColor_2FA640(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FB0A0) sColor_2FA640(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FB0B0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB0C0) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB0D0) sColor_2FA640(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FB0E0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB0F0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB100) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB110) sColor_2FA640(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB120) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB130) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB140) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB150) sColor_2FA640(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FB160) sColor_2FA640(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FB170) sColor_2FA640(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FB180) sColor_2FA640(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FB190) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB1A0) sColor_2FA640(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FB1B0) sColor_2FA640(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FB1C0) sColor_2FA640(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FB1D0) sColor_2FA640(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FB1E0) sColor_2FA640(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FB1F0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB200) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB210) sColor_2FA640(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FB220) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB230) sColor_2FA640(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FB240) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB250) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB260) sColor_2FA640(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FB270) sColor_2FA640(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FB280) sColor_2FA640(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FB290) sColor_2FA640(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FB2A0) sColor_2FA640(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FB2B0) sColor_2FA640(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FB2C0) sColor_2FA640(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FB2D0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB2E0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB2F0) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FB300) sColor_2FA640(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FB310) sColor_2FA640(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FB320) sColor_2FA640(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FB330) sColor_2FA640(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FB340) sColor_2FA640(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FB350) sColor_2FA640(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FB360) sColor_2FA640(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FB370) sColor_2FA640(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FB380) sColor_2FA640(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_002FA640(int, int);

//99.38%
INCLUDE_ASM("visualfx/worldlightman", func_002FAE18__FPv);
#ifdef SKIP_ASM
/**
 * Runs func_002FA640(1, 0xFFFF): a global constructor listed in the constructor table.
 *
 * @return nothing useful (func_002FA640 returns void)
 */
void* func_002FAE18(void* self)
{
    return func_002FA640(1, 0xffff);
}
#endif

