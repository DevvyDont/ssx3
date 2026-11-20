#include "common.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "world/worldview.h"

//100%
INCLUDE_ASM("world/worldview", cWorldView_getNumSections__FP10cWorldView);
#ifdef SKIP_ASM
/**
 * Returns the number of sections in the world view.
 *
 * @return mp_Hdr->m_uiNumSections, or 0 when there is no section table
 */
int cWorldView_getNumSections(cWorldView* self)
{
    if (self->mp_Hdr == 0) {
        return 0;
    }
    return self->mp_Hdr->m_uiNumSections;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_getSectionName);
#ifdef SKIP_ASM
/**
 * Returns a world section's record, which starts with its name.
 *
 * @param i  section id
 * @return the 0x58-byte section record, or 0 when there is no header or i is out of range
 */
extern "C" void* cWorldView_getSectionName(cWorldView* self, unsigned int i)
{
    tStreamDataHeader* list = self->mp_Hdr;
    if (list == 0 || i >= (unsigned int)list->m_uiNumSections) {
        return 0;
    }
    return (char*)self->mp_Section + i * 0x58;
}
#endif

struct cWorldViewEntry {
    char pad_0x00[0x14];
    int field_0x14;
};

//100%
INCLUDE_ASM("world/worldview", cWorldView_isSectionLoaded__FP10cWorldViewi);
#ifdef SKIP_ASM
/**
 * Tells whether a world section is loaded (stream state 5, or 6 when it is also active).
 *
 * @param section  section id
 * @return 1 when loaded, 0 otherwise (also for an out-of-range id)
 */
int cWorldView_isSectionLoaded(cWorldView* self, int section)
{
    if (self->mp_Hdr == 0 || (unsigned int)section >= (unsigned int)self->mp_Hdr->m_uiNumSections) {
        return 0;
    }
    cWorldViewEntry* entry = (cWorldViewEntry*)((char*)self + section * 8);
    return (unsigned int)(entry->field_0x14 - 5) < 2;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_isSectionUnLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a world section is fully unloaded (stream state 0).
 *
 * @param i  section id
 * @return 1 when unloaded, 0 otherwise (also for an out-of-range id)
 */
extern "C" int cWorldView_isSectionUnLoaded(cWorldView* self, unsigned int i)
{
    tStreamDataHeader* list = self->mp_Hdr;
    if (list == 0 || i >= (unsigned int)list->m_uiNumSections) {
        return 0;
    }
    return *(int*)((char*)self + (i << 3) + 0x14) == 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_loadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sViewEnt_3A98C8 { int state; float value; };
struct sView_3A98C8 { int f0; char* info; char pad8[0xC]; sViewEnt_3A98C8 ents[1]; };
/**
 * Asks the world view to stream in a world section.
 *
 * If the view has section info and @p idx is below its section count, stores @p value as the section's priority and
 * moves its state 0 (unloaded) to 1 (load requested) and 3 to 5; in state 4 the request is refused.
 *
 * @param idx  section id (On Tour id)
 * @param value  load priority (On Tour fPriority)
 * @return 0 when the section is in state 4, else 1
 */
extern "C" int cWorldView_loadSection(sView_3A98C8* self, unsigned int idx, float value)
{
    if (self->info != 0 && idx < *(unsigned int*)(self->info + 8)) {
        self->ents[idx].value = value;
        switch (self->ents[idx].state) {
        case 0:
            self->ents[idx].state = 1;
            break;
        case 3:
            self->ents[idx].state = 5;
            break;
        case 4:
            return 0;
        case 1: case 2: case 5: case 6:
            break;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_unloadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sViewEnt_3A9958 { int state; int pad; };
struct sView_3A9958 { int f0; char* info; char pad8[0xC]; sViewEnt_3A9958 ents[1]; };
/**
 * Asks for section @p idx to be unloaded.
 *
 * Loaded and Active sections go to UnLoadQueue and a LoadQueue section goes back to Empty; a section still Loading
 * refuses (returns 0). Empty, UnLoadQueue and UnLoading are left as they are. An index past m_uiNumSections, or no
 * mp_Hdr, also returns 1. Called by cSectionMan_update.
 *
 * @param idx  the section id
 * @return 0 while the section is loading, else 1
 */
extern "C" int cWorldView_unloadSection(sView_3A9958* self, unsigned int idx)
{
    if (self->info != 0 && idx < *(unsigned int*)(self->info + 8)) {
        switch (self->ents[idx].state) {
        case 2:
            return 0;
        case 5: case 6:
            self->ents[idx].state = 3;
            break;
        case 1:
            self->ents[idx].state = 0;
            break;
        case 0: case 3: case 4:
            break;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_activateSection);
#ifdef SKIP_ASM
extern "C" void cWorldCache_activateSectionPatches(void* world, unsigned int i);
struct cWorldView_activateSection_sEntry { int state; int field_0x4; };
struct cWorldView_activateSection_sView { void* world; tStreamDataHeader* mp_Hdr; };
/**
 * Activates a loaded world section.
 *
 * A loaded section (state 5) becomes active (6) and the world cache activates its contents (cWorldCache_activateSectionPatches).
 *
 * @param i  section id
 * @return 1 if the section is now active (or already was), 0 if it is not loaded or the id is out of range
 */
extern "C" int cWorldView_activateSection(cWorldView* self, unsigned int i)
{
    tStreamDataHeader* list = self->mp_Hdr;
    if (list != 0 && i < (unsigned int)list->m_uiNumSections) {
        int off = i << 3;
        char* base = (char*)self + 0x14;
        cWorldView_activateSection_sEntry* entry = (cWorldView_activateSection_sEntry*)(base + off);
        int st = entry->state;
        if (st == 5) {
            entry->state = 6;
            cWorldCache_activateSectionPatches(self->mp_WorldCache, i);
            return 1;
        } else if (st == 6) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_deactivateSection);
#ifdef SKIP_ASM
struct sWVSlot_9A48 { int state; int pad; };
struct sWV_9A48 { char pad0[4]; tStreamDataHeader* sec; char pad8[0xC]; sWVSlot_9A48 slots[1]; };
/**
 * Moves section @p idx from Active back to Loaded; any other state is kept.
 *
 * No C caller in the source.
 *
 * @param idx  the section id
 * @return 1 for a valid index, else 0
 */
extern "C" int cWorldView_deactivateSection(sWV_9A48* self, unsigned int idx)
{
    if (self->sec != 0 && idx < (unsigned int)self->sec->m_uiNumSections) {
        if (self->slots[idx].state == 6) {
            self->slots[idx].state = 5;
        }
        return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("world/worldview", cWorldView_getWorldBoundBox);
#ifdef SKIP_ASM
/**
 * Returns the address of the view's m_WorldBoundBox (+0x220). No C caller in the source.
 *
 * @param worldView  the cWorldView (this)
 * @return the box's address
 */
extern "C" int cWorldView_getWorldBoundBox(int worldView) {
    return worldView + 544;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_getHull);
#ifdef SKIP_ASM
/**
 * Returns one of the world's BSP hulls.
 *
 * @param uiIdx  hull index (no range check)
 * @return the 0x44-byte hull (On Tour const tBxBSPHull*)
 */
extern "C" void* cWorldView_getHull(cWorldView* self, int uiIdx)
{
    return (char*)(self->mp_Hulls) + uiIdx * 0x44;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_getNumHull__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of hull pages: mp_Hdr->m_uiNumBSPHulls.
 *
 * cWorldCache_initPageMap stores it in m_uiNumHullPage.
 *
 * @return the hull count
 */
int cWorldView_getNumHull(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x10);
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_isBoxValid);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cWorldCache_isPageLoaded(void* self, int a1);
extern void cWorldView_recursiveIsValid_9AC0(void* self, int sec, int box, int* ok) __asm__("cWorldView_recursiveIsValid");

/**
 * Returns whether every hull page that @p box touches is loaded.
 *
 * First section 0's own hull page (m_BSP.m_iGlobalHull) must be loaded when it has chunks; then each Active section's
 * tree is searched with cWorldView_recursiveIsValid. No C caller in the source.
 *
 * @param vself  the cWorldView
 * @param box  the box (minimum and maximum corners)
 * @return 1 when all those pages are loaded
 */
extern "C" int cWorldView_isBoxValid(cWorldView* vself, int box)
{
    char* cur = (char*)vself->mp_Section;
    int ok;
    unsigned i;
    int off = *(int*)(cur + 0x18);
    off *= 68;
    if (*(unsigned short*)(off + (int)vself->mp_Hulls) != 0) {
        if (cWorldCache_isPageLoaded(vself->mp_WorldCache, *(int*)(cur + 0x18)) == 0)
            return 0;
    }
    ok = 1;
    for (i = 0; i < ((cWorldView*)vself)->mp_Hdr->m_uiNumSections; i++) {
        if (*(int*)(((char*)vself) + 0x14 + i * 8) == 6)
            cWorldView_recursiveIsValid_9AC0(((char*)vself), *(int*)((char*)vself->mp_Section + i * 0x58 + 0x1C), box, &ok);
    }
    return ok;
}
#endif
//100%
INCLUDE_ASM("world/worldview", cWorldView_recursiveIsValid);
#ifdef SKIP_ASM
extern "C" int cWorldCache_isPageLoaded(void* self, int a1);

struct sV4_9BB8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSecInfo_9BB8 {
    short f0;
    unsigned short id;  // 0x02
};

struct sSection_9BB8;
struct sSection_9BB8 {
    float bmin[4];      // 0x00
    float bmax[4];      // 0x10
    sV4_9BB8 n0;        // 0x20
    sV4_9BB8 n1;        // 0x30
    char pad40[0x10];
    sSection_9BB8* child0; // 0x50
    sSection_9BB8* child1; // 0x54
    sSecInfo_9BB8* info; // 0x58
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_9BB8(const sV4_9BB8& a, const sV4_9BB8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline int overlap_9BB8(const sV4_9BB8* c, const sSection_9BB8* s)
{
    int r = 0;
    if (c[0].x <= s->bmax[0] && c[1].x >= s->bmin[0] &&
        c[0].y <= s->bmax[1] && c[1].y >= s->bmin[1] &&
        c[0].z <= s->bmax[2] && c[1].z >= s->bmin[2]) {
        r = 1;
    }
    return r;
}

/**
 * Clears @p ok when a hull page under a BSP node that overlaps @p box is not loaded (cWorldCache_isPageLoaded).
 *
 * Nodes whose box (+0x0 / +0x10) does not overlap @p box are skipped. The first child (+0x50) is visited when the
 * node's first plane (+0x20) gives a negative dot with the box's minimum, the second (+0x54) when the second plane
 * (+0x30) does with its maximum. Stops once @p ok is 0.
 *
 * @param world  the cWorldView
 * @param sec  the tBxBSPNode
 * @param box  the box (minimum and maximum corners)
 * @param ok  set on entry; cleared when a page is missing
 */
extern "C" void cWorldView_recursiveIsValid(cWorldView* world, sSection_9BB8* sec, sV4_9BB8* box, int* ok)
{
    if (*ok == 0)
        return;
    if (!overlap_9BB8(box, sec))
        return;
    if (sec->info != 0) {
        if (cWorldCache_isPageLoaded(world->mp_WorldCache, sec->info->id) == 0) {
            *ok = 0;
            return;
        }
    }
    if (sec->child0 != 0) {
        if (Dot_9BB8(sec->n0, box[0]) < 0.0f) {
            cWorldView_recursiveIsValid(world, sec->child0, box, ok);
            if (*ok == 0)
                return;
        }
    }
    if (sec->child1 != 0) {
        if (Dot_9BB8(sec->n1, box[1]) < 0.0f)
            cWorldView_recursiveIsValid(world, sec->child1, box, ok);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_refreshStreamFocus);
#ifdef SKIP_ASM
struct sWVSlot_9D60 {
    int state;
    int f4;
};

struct sWVItem_9D60 {
    char pad0[0x1C];
    void* section;
    char pad20[0x58 - 0x20];
};

struct sWVCam_9D60 {
    int active;
    int f4;
    int frame;
    char padC[0x40 - 0xC];
    float f40;
    float f44;
    float f48;
    char pad4C[0x50 - 0x4C];
};

struct sWV_9D60 {
    void* world;
    tStreamDataHeader* mp_Hdr;
    sWVItem_9D60* items;
    int fC;
    int f10;
    sWVSlot_9D60 slots[0x45];
    char pad23C[0x240 - 0x23C];
    sWVCam_9D60 cams[4];
};

extern char* cAppMan_TheAppMan;
extern "C" void cWorldView_recursiveTreeFocus(void* self, void* section, int cam, int loaded);

/**
 * Recomputes which hull pages stream focus @p cam needs.
 *
 * Only for an active focus (m_StreamFocus[cam].m_bActive): resets m_fSafeRadius and m_fSafeTestRadius to m_fLoadRadius,
 * runs cWorldView_recursiveTreeFocus on the root node of every Loaded or Active section (setting the safe radius only for Active ones),
 * then stores the app frame (cAppMan_TheAppMan + 0x18) in m_iFocusTick and clears m_bFocusUpdate.
 *
 * @param cam  the focus index (0 or 1)
 */
extern "C" void cWorldView_refreshStreamFocus(sWV_9D60* self, int cam)
{
    if (*(int*)((char*)self + cam * 0x50 + 0x240) != 0) {
        int frame = *(int*)(cAppMan_TheAppMan + 0x18);
        float loadRadius = *(float*)((char*)self + cam * 0x50 + 0x280);
        *(float*)((char*)self + cam * 0x50 + 0x284) = loadRadius;
        *(float*)((char*)self + cam * 0x50 + 0x288) = loadRadius;
        unsigned int i;
        for (i = 0; i < (unsigned int)self->mp_Hdr->m_uiNumSections; i++) {
            int st = self->slots[i].state;
            if (st == 5 || st == 6) {
                cWorldView_recursiveTreeFocus(self, self->items[i].section, cam, st == 6);
            }
        }
        *(int*)((char*)self + cam * 0x50 + 0x248) = frame;
        *(int*)((char*)self + cam * 0x50 + 0x244) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_getDistanceToBox);
#ifdef SKIP_ASM
// PORT: sqrt.s via inline asm (as an SDK math-header sqrtf would).
static inline float sqrtf_3A9E50(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct t4Vector_3A9E50 {
    float mX, mY, mZ, mW;
    float x() const { return mX; }
    float y() const { return mY; }
    float z() const { return mZ; }
};

// cWorldView::getDistanceToBox(const t4Vector& pos, const t4Vector& boxMin, const t4Vector& boxMax)
extern "C" float cWorldView_getDistanceToBox_impl(void* self, const t4Vector_3A9E50& pos, const t4Vector_3A9E50& boxMin,
                                    const t4Vector_3A9E50& boxMax) __asm__("cWorldView_getDistanceToBox");

/**
 * Returns the distance from @p pos to an axis-aligned box, or -1 when the point is inside it.
 *
 * When the point lies outside along one axis only, that axis's gap is returned as is; otherwise the gaps of the outside
 * axes are combined as a Euclidean distance.
 *
 * @param pos  the point
 * @param boxMin  the box's minimum corner
 * @param boxMax  the box's maximum corner
 * @return the distance, or -1 inside
 */
extern "C" float cWorldView_getDistanceToBox_impl(void* self, const t4Vector_3A9E50& pos, const t4Vector_3A9E50& boxMin,
                                    const t4Vector_3A9E50& boxMax)
{
    bool bSpanX = boxMin.x() <= pos.x() && pos.x() <= boxMax.x();
    bool bSpanY = boxMin.y() <= pos.y() && pos.y() <= boxMax.y();
    bool bSpanZ = boxMin.z() <= pos.z() && pos.z() <= boxMax.z();
    if (bSpanX && bSpanY && bSpanZ) {
        return -1.0f;
    }
    float fDist = 0.0f;
    bool bX = false;
    bool bY = false;
    bool bZ = false;
    bool bSqrt = false;
    if (bSpanX) {
        if (bSpanY) {
            bZ = true;
        } else if (bSpanZ) {
            bY = true;
        } else {
            bY = true;
            bZ = true;
            bSqrt = true;
        }
    } else {
        if (bSpanY) {
            if (bSpanZ) {
                bX = true;
            } else {
                bX = true;
                bZ = true;
                bSqrt = true;
            }
        } else {
            if (bSpanZ) {
                bX = true;
                bY = true;
                bSqrt = true;
            } else {
                bX = true;
                bY = true;
                bZ = true;
                bSqrt = true;
            }
        }
    }
    if (bX) {
        float fXdist = pos.x() - boxMax.x();
        if (fXdist < 0.0f) {
            fXdist = boxMin.x() - pos.x();
        }
        if (bSqrt) {
            fDist += fXdist * fXdist;
        } else {
            fDist += fXdist;
        }
    }
    if (bY) {
        float fYdist = pos.y() - boxMax.y();
        if (fYdist < 0.0f) {
            fYdist = boxMin.y() - pos.y();
        }
        if (bSqrt) {
            fDist += fYdist * fYdist;
        } else {
            fDist += fYdist;
        }
    }
    if (bZ) {
        float fZdist = pos.z() - boxMax.z();
        if (fZdist < 0.0f) {
            fZdist = boxMin.z() - pos.z();
        }
        if (bSqrt) {
            fDist += fZdist * fZdist;
        } else {
            fDist += fZdist;
        }
    }
    if (bSqrt) {
        fDist = sqrtf_3A9E50(fDist);
    }
    return fDist;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldview", cWorldView_recursiveTreeFocus);
#ifdef SKIP_ASM
extern "C" float cWorldView_getDistanceToBox(void* self, void* pos, void* bmin, void* bmax);
extern "C" int cWorldCache_isPageLoaded(void* self, int a1);
extern "C" void cWorldCache_touchPage(void* self, int i, float f);

struct sV4_A028 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sWVCam_A028 {
    int active;         // 0x00
    int f4;
    int frame;          // 0x08
    int fC;
    sV4_A028 pos;       // 0x10
    sV4_A028 bmin;      // 0x20
    sV4_A028 bmax;      // 0x30
    float range;        // 0x40
    float near;         // 0x44
    float best;         // 0x48
    int f4C;
};

struct sWV_A028 {
    void* world;        // 0x00
    char pad4[0x23C];
    sWVCam_A028 cams[4]; // 0x240
};

struct sSecInfo_A028 {
    short f0;
    unsigned short id;  // 0x02
};

struct sSection_A028 {
    float bmin[4];      // 0x00
    float bmax[4];      // 0x10
    sV4_A028 n0;        // 0x20
    sV4_A028 n1;        // 0x30
    char pad40[0x10];
    void* child0;       // 0x50
    void* child1;       // 0x54
    sSecInfo_A028* info; // 0x58
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_A028(const sV4_A028& a, const sV4_A028& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline int overlap_A028(const sV4_A028* c, const sSection_A028* s)
{
    int r = 0;
    if (c[0].x <= s->bmax[0] && c[1].x >= s->bmin[0] &&
        c[0].y <= s->bmax[1] && c[1].y >= s->bmin[1] &&
        c[0].z <= s->bmax[2] && c[1].z >= s->bmin[2]) {
        r = 1;
    }
    return r;
}

/**
 * Touches the hull pages near stream focus @p cam under a BSP node, with a priority that falls with distance.
 *
 * For a node whose box overlaps the focus's m_FocusBox and that has a hull: inside the node's box the priority is 0.8
 * and the safe radius drops to 0 when the page is not loaded; outside it, a node farther than m_fLoadRadius is dropped
 * (with its children), the priority is (1 - distance / m_fLoadRadius) x 0.8 and the safe radius shrinks to the distance
 * of an unloaded page. The radius goes to m_fSafeRadius when @p loaded is set and to m_fSafeTestRadius when smaller;
 * the page is touched (cWorldCache_touchPage). Children are visited by the same plane tests as cWorldView_recursiveIsValid, against the focus
 * box.
 *
 * @param self_  the cWorldView
 * @param sec_  the tBxBSPNode
 * @param cam  the focus index
 * @param loaded  set for an Active section (updates the safe radius)
 */
extern "C" void cWorldView_recursiveTreeFocus(void* self_, void* sec_, int cam, int loaded)
{
    if (!overlap_A028(&((sWV_A028*)self_)->cams[cam].bmin, (sSection_A028*)sec_))
        return;
    if (((sSection_A028*)sec_)->info != 0) {
        float dist = cWorldView_getDistanceToBox(self_, &((sWV_A028*)self_)->cams[cam].pos, sec_, ((sSection_A028*)sec_)->bmax);
        float safeRadius = ((sWV_A028*)self_)->cams[cam].near;
        float w;
        if (dist < 0.0f) {
            w = 0.800000011920929f;
            if (cWorldCache_isPageLoaded(((sWV_A028*)self_)->world, ((sSection_A028*)sec_)->info->id) == 0)
                safeRadius = 0.0f;
        } else {
            if (((sWV_A028*)self_)->cams[cam].range < dist)
                return;
            w = (1.0f - dist / ((sWV_A028*)self_)->cams[cam].range) * 0.800000011920929f;
            if (dist < safeRadius) {
                if (cWorldCache_isPageLoaded(((sWV_A028*)self_)->world, ((sSection_A028*)sec_)->info->id) == 0)
                    safeRadius = dist;
            }
        }
        if (loaded)
            ((sWV_A028*)self_)->cams[cam].near = safeRadius;
        if (safeRadius < ((sWV_A028*)self_)->cams[cam].best)
            ((sWV_A028*)self_)->cams[cam].best = safeRadius;
        cWorldCache_touchPage(((sWV_A028*)self_)->world, ((sSection_A028*)sec_)->info->id, w);
    }
    if (((sSection_A028*)sec_)->child0 != 0) {
        if (Dot_A028(((sSection_A028*)sec_)->n0, ((sWV_A028*)self_)->cams[cam].bmin) < 0.0f)
            cWorldView_recursiveTreeFocus(self_, ((sSection_A028*)sec_)->child0, cam, loaded);
    }
    if (((sSection_A028*)sec_)->child1 != 0) {
        if (Dot_A028(((sSection_A028*)sec_)->n1, ((sWV_A028*)self_)->cams[cam].bmax) < 0.0f)
            cWorldView_recursiveTreeFocus(self_, ((sSection_A028*)sec_)->child1, cam, loaded);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AA2D8);
#ifdef SKIP_ASM
/**
 * Sets the word at +0x8 to -1 and returns @p self. No C caller in the source.
 *
 * @return @p self
 */
extern "C" void* func_003AA2D8(void* self)
{
    *(unsigned int*)((char*)self + 8) = 0xFFFFFFFF;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_cChunkResolve);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

struct cChunkResolve_cChunkResolve_sEntry {
    float dist;
    float scale;
    short a;
    short b;
};

struct cChunkResolve_cChunkResolve_sState {
    int f00;
    unsigned int flags;
    unsigned int f08;
    int f0C;
    cChunkResolve_cChunkResolve_sEntry entries[24];
};

extern cChunkResolve_cChunkResolve_sState D_00508F50;

struct cChunkResolve_cChunkResolve_sView {
    int f00;
    cChunkResolve_cChunkResolve_sState* state;
    void** vt;
};
// splat labels D_00508F50+8 separately
extern unsigned int D_00508F58[];
extern int D_004A5A68;
extern void* cChunkResolve_vtbl[];

// PORT: the unit declares this `void`, but the body returns self ($2); bound via asm label.
cChunkResolve_cChunkResolve_sView* cChunkResolve_cChunkResolve_impl(cChunkResolve_cChunkResolve_sView* self) __asm__("cChunkResolve_cChunkResolve");

/**
 * Constructs the stream chunk resolver and resets its shared state.
 *
 * Sets cChunkResolve_vtbl, clears +0x0, initializes the static state D_00508F50 on first use, points the resolver at it
 * and zeroes it with MEM_fill, then sets its ids to -1, each of its 24 entries to (1000.0, 0.5, 0, -1) and its flag
 * bits.
 *
 * @return this
 */
cChunkResolve_cChunkResolve_sView* cChunkResolve_cChunkResolve_impl(cChunkResolve_cChunkResolve_sView* self)
{
    self->vt = cChunkResolve_vtbl;
    self->f00 = 0;
    if (D_004A5A68 == 0) {
        D_00508F50.f08 = 0xFFFFFFFF;
        D_004A5A68 = 1;
    }
    cChunkResolve_cChunkResolve_sState* sharedState = &D_00508F50;
    self->state = sharedState;
    MEM_fill(sharedState, 0, sizeof(cChunkResolve_cChunkResolve_sState));
    D_00508F58[0] = 0xFFFFFFFF;
    sharedState->f0C = -1;
    D_00508F50.f00 = 0;
    int i;
    for (i = 0; i < 24; i++) {
        sharedState->entries[i].dist = 1000.0f;
        sharedState->entries[i].scale = 0.5f;
        sharedState->entries[i].a = 0;
        sharedState->entries[i].b = -1;
    }
    D_00508F50.flags = (((D_00508F50.flags | 1) & ~4) | 2) << 16;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveModel);
#ifdef SKIP_ASM
class cChunkResolve_resolveModel_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06(void* a1);
};

/**
 * Fixes up a streamed model record: turns its offsets at +0x8 and +0xC into pointers, then resolves the rest
 * through virtual slot 6 (probably resolveModelHeirarchy, On Tour's next method).
 *
 * @param pModel  the model record (On Tour tModelRecord*)
 * @return always 1
 */
// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveModel(cChunkResolve_resolveModel_cObj* self, void* pModel)
{
    *(int*)((char*)pModel + 0x8) += (int)pModel;
    *(int*)((char*)pModel + 0xC) += (int)pModel;
    self->v06(pModel);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveModelHeirarchy);
#ifdef SKIP_ASM
class cChunkResolve_resolveModelHeirarchy_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07(void* p, void* base);
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int v11(void* p, void* base);
};

struct cChunkResolve_resolveModelHeirarchy_sEnt {
    int f0;
    int f4;
    int f8;
    unsigned int fC;
};

extern char t4mat_Identity[];

/**
 * Fixes up the pointers of a model's mesh hierarchy after it is loaded.
 *
 * For each of the @p pModel +0x4 entries in the array at +0x8: a mesh offset is made absolute and passed to virtual slot 7
 * with the model, a matrix offset of -1 becomes t4mat_Identity (otherwise made absolute), and an animation offset is
 * made absolute and passed to virtual slot 11 with the model as base.
 *
 * @param pModel  the loaded model record (On Tour pModel)
 * @return 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveModelHeirarchy(cChunkResolve_resolveModelHeirarchy_cObj* self, void* pModel)
{
    int n = *(int*)((char*)pModel + 0x4);
    cChunkResolve_resolveModelHeirarchy_sEnt* meshHierarchy = *(cChunkResolve_resolveModelHeirarchy_sEnt**)((char*)pModel + 0x8);
    int i;
    for (i = 0; i < n; i++, meshHierarchy++) {
        if (meshHierarchy->f4 != 0) {
            meshHierarchy->f4 += (int)pModel;
            self->v07((void*)meshHierarchy->f4, pModel);
        }
        if (meshHierarchy->fC == 0xFFFFFFFF) {
            meshHierarchy->fC = (unsigned int)t4mat_Identity;
        } else {
            meshHierarchy->fC += (int)pModel;
        }
        if (meshHierarchy->f8 != 0) {
            meshHierarchy->f8 += (int)pModel;
            self->v11((void*)meshHierarchy->f8, pModel);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveMesh);
#ifdef SKIP_ASM
class cChunkResolve_resolveMesh_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual int v08(void* p, int base);
};

/**
 * Fixes up a mesh's context pointers after its model is loaded.
 *
 * Adds @p base to the table pointer at @p pMesh +0x20 and to each of its @p pMesh +0x1C entries, passing every entry to
 * virtual slot 8 with @p base.
 *
 * @param pMesh  the mesh record (On Tour pMesh)
 * @param base  the model record its offsets are relative to (On Tour pModel)
 * @return 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveMesh(cChunkResolve_resolveMesh_cObj* self, void* pMesh, int base)
{
    int i = 0;
    int* contextEntry = (int*)(*(int*)((char*)pMesh + 0x20) += base);
    for (; i < *(int*)((char*)pMesh + 0x1C); i++) {
        *contextEntry += base;
        self->v08((void*)*contextEntry++, base);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveParticleModel);
#ifdef SKIP_ASM
class cChunkResolve_resolveParticleModel_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual int v15(void* a1);
};

/**
 * Fixes up a particle model's pointers after it is loaded.
 *
 * Adds the record's address to the offset at +0x8 and calls virtual slot 15 with the record.
 *
 * @param pModel  the particle model record (On Tour pModel)
 * @return 1
 */
// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveParticleModel(cChunkResolve_resolveParticleModel_cObj* self, void* pModel)
{
    *(int*)((char*)pModel + 0x8) += (int)pModel;
    self->v15(pModel);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveParticleModelHeirarchy);
#ifdef SKIP_ASM
class cChunkResolve_resolveParticleModelHeirarchy_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int v11(void* p, void* base);
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int v16(void* p);
};

struct cChunkResolve_resolveParticleModelHeirarchy_sEnt {
    int f0;
    int f4;
    int f8;
    unsigned int fC;
};

extern char t4mat_Identity[];

/**
 * Fixes up the pointers of a particle model's mesh hierarchy.
 *
 * For each of the @p pModel +0x4 entries in the array at +0x8: a mesh offset is made absolute and passed to virtual slot
 * 16, a matrix offset of -1 becomes t4mat_Identity (otherwise made absolute), and an animation offset is made absolute
 * and passed to virtual slot 11 with a base of 0 (cChunkResolve_resolveModelHeirarchy passes the model).
 *
 * @param pModel  the particle model record (On Tour pModel)
 * @return 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveParticleModelHeirarchy(cChunkResolve_resolveParticleModelHeirarchy_cObj* self, void* pModel)
{
    int n = *(int*)((char*)pModel + 0x4);
    cChunkResolve_resolveParticleModelHeirarchy_sEnt* meshHierarchy = *(cChunkResolve_resolveParticleModelHeirarchy_sEnt**)((char*)pModel + 0x8);
    int i;
    for (i = 0; i < n; i++, meshHierarchy++) {
        if (meshHierarchy->f4 != 0) {
            meshHierarchy->f4 += (int)pModel;
            self->v16((void*)meshHierarchy->f4);
        }
        if (meshHierarchy->fC == 0xFFFFFFFF) {
            meshHierarchy->fC = (unsigned int)t4mat_Identity;
        } else {
            meshHierarchy->fC += (int)pModel;
        }
        if (meshHierarchy->f8 != 0) {
            meshHierarchy->f8 += (int)pModel;
            self->v11((void*)meshHierarchy->f8, 0);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveParticleMesh);
#ifdef SKIP_ASM
/**
 * Fixes up a particle mesh's data pointer.
 *
 * Adds the record's address to the offset at +0x20.
 *
 * @param pMesh  the particle mesh record (On Tour pMesh)
 * @return 1
 */
extern "C" int cChunkResolve_resolveParticleMesh(void* self, void* pMesh)
{
    *(int*)((char*)pMesh + 0x20) += (int)pMesh;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveParticleInstance);
#ifdef SKIP_ASM
/**
 * Resolves a particle instance's model reference after its section is loaded.
 *
 * The id at @p pInstance +0x64 selects a section (low byte) and an entry (upper bits) of the cache table; the entry's address
 * (its value >> 8, times 4) replaces the id, or 0 when the section is not loaded or the entry is empty.
 *
 * @param pInstance  the particle instance record (On Tour pInstance)
 * @return 1
 */
// PORT: decodes an int handle into a pointer ((void*)(i << 2)); not 64-bit safe.
extern "C" int cChunkResolve_resolveParticleInstance(void* self, void* pInstance)
{
    unsigned int resourceId = *(unsigned int*)((char*)pInstance + 0x64);
    char* sec = (*(char***)(**(char***)self + 8))[resourceId & 0xFF];
    void* res;
    unsigned int i;
    if (sec == 0 || (i = (*(unsigned int**)(sec + 0x24))[resourceId >> 8] >> 8) == 0) {
        res = 0;
    } else {
        res = (void*)(i << 2);
    }
    *(void**)((char*)pInstance + 0x64) = res;
    *(int*)((char*)pInstance + 0x84) = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveMaterial);
#ifdef SKIP_ASM
/**
 * Fixes up a material record's data pointer.
 *
 * Sets @p pMaterial +0x10 to 0 when it holds -1, otherwise to the address of the data that follows at +0x14.
 *
 * @param pMaterial  the material record (On Tour pMaterial)
 * @return 1
 */
// PORT: stores a pointer in an int field (int typing needed to match); not 64-bit safe.
extern "C" int cChunkResolve_resolveMaterial(void* self, void* pMaterial)
{
    if (*(int*)((char*)pMaterial + 0x10) == -1) {
        *(int*)((char*)pMaterial + 0x10) = 0;
    } else {
        *(int*)((char*)pMaterial + 0x10) = (int)((char*)pMaterial + 0x14);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveMaterialBlocks);
#ifdef SKIP_ASM
/**
 * Resolves the material references of a list of material blocks.
 *
 * Each of the @p n blocks starts with a count followed by that many ids; every id (section in the low byte, entry in
 * the upper bits) is replaced by the cached material's address, or 0 when its section is not loaded or the entry is
 * empty. The next block follows the last id.
 *
 * @param n  number of blocks (On Tour iNumBlocks)
 * @param data  the first block (On Tour pMaterialBlocks)
 * @return 1
 */
// PORT: decodes int handles into pointers ((void*)(i << 2)); not 64-bit safe.
extern "C" int cChunkResolve_resolveMaterialBlocks(void* self, int n, int* data)
{
    int blockIndex;
    for (blockIndex = 0; blockIndex < n; blockIndex++) {
        int j;
        unsigned int* hp = (unsigned int*)(data + 1);
        for (j = 0; j < *data; j++, hp++) {
            unsigned int materialId = *hp;
            char* sec = (*(char***)(**(char***)self + 8))[materialId & 0xFF];
            void* res;
            unsigned int i;
            if (sec == 0 || (i = (*(unsigned int**)(sec + 0x4))[materialId >> 8] >> 8) == 0) {
                res = 0;
            } else {
                res = (void*)(i << 2);
            }
            *(void**)hp = res;
        }
        data += *data + 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveAnimation);
#ifdef SKIP_ASM
class cChunkResolve_resolveAnimation_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual int v12(void* p, int base);
};

/**
 * Fixes up an animation's channel pointers.
 *
 * Adds @p base to the table pointer at @p pAnimation +0x20 and to each of its @p pAnimation +0x1C entries, passing every channel to
 * virtual slot 12 with @p base.
 *
 * @param pAnimation  the animation record (On Tour pAnimation)
 * @param base  address its offsets are relative to (On Tour pBase)
 * @return 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
extern "C" int cChunkResolve_resolveAnimation(cChunkResolve_resolveAnimation_cObj* self, void* pAnimation, int base)
{
    int i = 0;
    int* channelId = (int*)(*(int*)((char*)pAnimation + 0x20) += base);
    for (; i < *(int*)((char*)pAnimation + 0x1C); i++) {
        *channelId += base;
        self->v12((void*)*channelId++, base);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveChannel);
#ifdef SKIP_ASM
/**
 * Fixes up an animation channel's data pointer.
 *
 * Adds @p pBase to the offset at @p pChannel +0x4.
 *
 * @param pChannel  the channel record (On Tour pChannel)
 * @param pBase  address the offset is relative to (On Tour pBase)
 * @return 1
 */
extern "C" int cChunkResolve_resolveChannel(void* self, void* pChannel, int pBase)
{
    *(int*)((char*)pChannel + 0x4) += pBase;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveInstance);
#ifdef SKIP_ASM
static inline char *refToPtr_A8F0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_A8F0(unsigned i) { if (i == 0) return 0; return refToPtr_A8F0(i); }
/**
 * Fixes up a streamed instance record: replaces its model resource id (+0x80) with the model pointer.
 *
 * The id's low byte picks the section, the rest the entry of the section's resource table. Also copies the
 * model's +0xC word to +0x94 and clears +0x8C.
 *
 * @param node  the instance record (On Tour tInstanceRecord*)
 * @return always 1
 */
// Resolves the node's object handle (0x80) to its object pointer and caches the object's 0xC field.
extern "C" int cChunkResolve_resolveInstance(char** self, char* node)
{
    unsigned id = *(unsigned*)(node + 0x80);
    char *section = (*(char***)(**(char***)self + 8))[id & 0xFF];
    unsigned i;
    char *resolved;
    if (!section)
        resolved = 0;
    else {
        i = ((unsigned*)*(char**)(section + 0x14))[id >> 8] >> 8;
        char *converted = conv_A8F0(i);
        resolved = converted;
    }
    *(char**)(node + 0x80) = resolved;
    int activeNode = *(int*)(resolved + 0xC);
    *(int*)(node + 0x8C) = 0;
    *(int*)(node + 0x94) = activeNode;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveSpline);
#ifdef SKIP_ASM
struct cChunkResolve_resolveSpline_sNode {
    char pad[0x60];
    int prev;       // 0x60 (index fixed up into a pointer)
    int next;       // 0x64
    void* owner;    // 0x68
    char pad6C[0x90 - 0x6C];
};

/**
 * Links a loaded spline's segments together.
 *
 * Points +0x24 at the segment array at +0x30. For each of the +0x20 segments (0x90 bytes each), a previous or next
 * index that is not -1 becomes a pointer to the neighbouring segment (wrapping around the ends), -1 becomes 0, and
 * +0x68 is set to the spline.
 *
 * @param list  the spline record (On Tour pSpline)
 * @return 1
 */
// PORT: stores pointers in int fields (int typing needed to match); not 64-bit safe.
extern "C" int cChunkResolve_resolveSpline(void* self, void* list)
{
    int i;
    int count = *(int*)((char*)list + 0x20);
    cChunkResolve_resolveSpline_sNode* nodes = (cChunkResolve_resolveSpline_sNode*)((char*)list + 0x30);
    int next = 1;
    *(cChunkResolve_resolveSpline_sNode**)((char*)list + 0x24) = nodes;
    int prev = count - 1;
    for (i = 0; i < *(int*)((char*)list + 0x20); i++) {
        if (nodes[i].prev != -1) {
            nodes[i].prev = (int)&nodes[prev];
        } else {
            nodes[i].prev = 0;
        }
        if (nodes[i].next != -1) {
            nodes[i].next = (int)&nodes[next];
        } else {
            nodes[i].next = 0;
        }
        prev++;
        next++;
        if (prev >= *(int*)((char*)list + 0x20)) {
            prev = 0;
        }
        if (next >= *(int*)((char*)list + 0x20)) {
            next = 0;
        }
        nodes[i].owner = list;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveCollideModel);
#ifdef SKIP_ASM
extern "C" void SphereTree_init(void* self);
extern "C" int bxSphereTree_Load(void* self, char* data, void* ctx);

struct sList_3AAA08 {
    char* items;
    int count;
};

struct sNode_3AAA08 {
    unsigned short n;
    unsigned short pad;
    char* p4;
    char* p8;
    char* pC;
    char* p10;
};

struct sChunk_3AAA08 {
    unsigned short type;
    unsigned short n;
    char* data;             // 0x4
    sList_3AAA08* list;     // 0x8
    char* items;            // 0xC
};

/**
 * Builds a collision model's runtime data after it is loaded.
 *
 * Offsets of -1 at +0x4 (data), +0x8 (list) and +0xC (items) become 0, others become pointers into the record. For type
 * 3 the list gets the record's mesh count and item array; every 0x140-byte item is initialized with SphereTree_init and
 * then loaded from consecutive data with bxSphereTree_Load. For type 1 each mesh's four offsets are made absolute and
 * stored in the item array.
 *
 * @param ctx  resource id of the model, passed on to bxSphereTree_Load (On Tour RID)
 * @param pCollideModel  the collision model record (On Tour pCollideModel)
 * @return 1
 */
extern "C" int cChunkResolve_resolveCollideModel(void* self, void* ctx, sChunk_3AAA08* pCollideModel)
{
    if ((int)pCollideModel->data == -1) {
        pCollideModel->data = 0;
    } else {
        pCollideModel->data = (char*)pCollideModel + (int)pCollideModel->data;
    }
    if ((int)pCollideModel->list == -1) {
        pCollideModel->list = 0;
    } else {
        pCollideModel->list = (sList_3AAA08*)((char*)pCollideModel + (int)pCollideModel->list);
    }
    if ((int)pCollideModel->items == -1) {
        pCollideModel->items = 0;
    } else {
        pCollideModel->items = (char*)pCollideModel + (int)pCollideModel->items;
    }
    if (pCollideModel->type == 3) {
        sList_3AAA08* modelList = pCollideModel->list;
        modelList->count = 0;
        modelList->items = 0;
        modelList->count = pCollideModel->n;
        modelList->items = pCollideModel->items;
        for (int i = 0; i < modelList->count; i++) {
            SphereTree_init(modelList->items + i * 320);
        }
        char* data = pCollideModel->data;
        for (int i = 0; i < modelList->count; i++) {
            data += bxSphereTree_Load(modelList->items + i * 320, data, ctx);
        }
    } else if (pCollideModel->type == 1) {
        sList_3AAA08* modelList = pCollideModel->list;
        modelList->count = 0;
        modelList->items = 0;
        modelList->count = pCollideModel->n;
        modelList->items = pCollideModel->items;
        sNode_3AAA08* node = (sNode_3AAA08*)pCollideModel->data;
        for (int i = 0; i < modelList->count; i++) {
            ((sNode_3AAA08**)modelList->items)[i] = node;
            node->p4 = (char*)node + (int)node->p4;
            node->p8 = (char*)node + (int)node->p8;
            node->pC = (char*)node + (int)node->pC;
            node->p10 = (char*)node + (int)node->p10;
            node = (sNode_3AAA08*)(node->p10 + node->n * 16);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveHalo);
#ifdef SKIP_ASM
// PORT: func_003AD230__FPv is declared (void*) but takes a flags int and returns a float.
float func_003AD230_f(int flags) __asm__("func_003AD230__FPv");

struct cChunkResolve_resolveHalo_sObj {
    char pad_0x00[0xC];
    int flags;          // 0xC
    char pad_0x10[0xC];
    float pos[3];       // 0x1C
    float min[3];       // 0x28
    float max[3];       // 0x34
    int field_0x40;
    int field_0x44;
};

/**
 * Sets up a light halo's bounds after it is loaded.
 *
 * Gets the radius for the halo type (flags & 0x70) from func_003AD230, clears +0x40 and +0x44 and sets the box
 * +0x28..+0x3C to the position at +0x1C plus or minus that radius.
 *
 * @param obj  the halo record (On Tour pLightHalo)
 * @return 1
 */
extern "C" int cChunkResolve_resolveHalo(void* self, cChunkResolve_resolveHalo_sObj* obj)
{
    float haloRadius = func_003AD230_f(obj->flags & 0x70);
    obj->field_0x44 = 0;
    obj->field_0x40 = 0;
    obj->min[0] = obj->pos[0] - haloRadius;
    obj->min[1] = obj->pos[1] - haloRadius;
    obj->min[2] = obj->pos[2] - haloRadius;
    obj->max[0] = obj->pos[0] + haloRadius;
    obj->max[1] = obj->pos[1] + haloRadius;
    obj->max[2] = obj->pos[2] + haloRadius;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveLight);
#ifdef SKIP_ASM
/**
 * Accepts a loaded real-time light, limiting its range.
 *
 * Returns 0 for a light with flag 0x100 or a type (+0x10) other than 1 or 2; otherwise caps the range at +0x1C to 5000
 * and returns 1.
 *
 * @param obj  the light record (On Tour pRTLight)
 * @return 1 when the light is used, else 0
 */
extern "C" int cChunkResolve_resolveLight(void* self, void* obj)
{
    if (*(int*)((char*)obj + 0xC) & 0x100) {
        return 0;
    }
    int lightType = *(int*)((char*)obj + 0x10);
    if (lightType < 3) {
        if (lightType > 0) {
            if (*(float*)((char*)obj + 0x1C) > 5000.0f) {
                *(float*)((char*)obj + 0x1C) = 5000.0f;
            }
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_unresolveLight);
#ifdef SKIP_ASM
/**
 * Tells whether a real-time light being unloaded was one the resolver accepted.
 *
 * @param pRTLight  the light record (On Tour pRTLight)
 * @return 1 for a light of type 1 or 2 without flag 0x100, else 0
 */
extern "C" int cChunkResolve_unresolveLight(void* self, void* pRTLight)
{
    if (*(int*)((char*)pRTLight + 0xC) & 0x100) {
        return 0;
    }
    switch (*(int*)((char*)pRTLight + 0x10)) {
    case 1:
    case 2:
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_calcMaxLightDistance);
#ifdef SKIP_ASM
extern "C" float func_0040D758(float);
extern "C" float func_0040DA10(float);

/**
 * Returns how far a light reaches before its strength falls to @p divisor.
 *
 * The strength is the light's +0x14 times +0x18. By the falloff type at +0x64: 1 gives strength / @p divisor x 100, 2
 * the square root of that ratio x 100, 3 the cube root (func_0040DA10 log, func_0040D758 exp of a third) x 100; any
 * other type gives 1e7. No C caller in the source.
 *
 * @param light  the tLightRecord
 * @param divisor  the threshold strength
 * @return the distance
 */
extern "C" float cChunkResolve_calcMaxLightDistance(void* self, void* light, float divisor)
{
    float prod = *(float*)((char*)light + 0x14) * *(float*)((char*)light + 0x18);
    float ratio = prod / divisor;
    switch (*(char*)((char*)light + 0x64)) {
    case 1:
        return ratio * 100.0f;
    case 2: {
        float root;
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(root) : "f"(ratio));
        return root * 100.0f;
    }
    case 3:
    {
        float third = 0.33333334f;
        float logRatio = func_0040DA10(ratio);
        return func_0040D758(logRatio * third) * 100.0f;
    }
    default:
        return 1e7f;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldview", cChunkResolve_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cChunkResolve_cChunkResolve(void* self);
extern char str_cPS2ChunkResolve[];
extern void* cPS2ChunkResolve_vtbl[];

/**
 * Creates the PS2 chunk resolver: allocates it ("cPS2ChunkResolve", 0x14 bytes), runs the base constructor
 * and stores the cPS2ChunkResolve vtable.
 *
 * @return the new resolver
 */
extern "C" void* cChunkResolve_construct()
{
    void* chunkResolve = cMemMan_alloc(20, str_cPS2ChunkResolve, 0, 0);
    cChunkResolve_cChunkResolve(chunkResolve);
    *(void***)((char*)chunkResolve + 0x8) = cPS2ChunkResolve_vtbl;
    return chunkResolve;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve__cPS2ChunkResolve);
#ifdef SKIP_ASM
void cPS2ChunkResolve_purge(void*);
void operator_delete(int*);
extern void* cPS2ChunkResolve_vtbl[];
extern void* cChunkResolve_vtbl[];

/**
 * Destroys a PS2 chunk resolver: runs its empty cleanup, restores the base cChunkResolve vtable, then frees it.
 *
 * Stores cPS2ChunkResolve's vtable at +0x8 and calls cPS2ChunkResolve_purge(), whose body is empty. On Tour calls this
 * destructor cPspChunkResolve::~cPspChunkResolve.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2ChunkResolve__cPS2ChunkResolve(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = cPS2ChunkResolve_vtbl;
    cPS2ChunkResolve_purge(self);
    *(void***)((char*)self + 0x8) = cChunkResolve_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_init);
#ifdef SKIP_ASM

/**
 * Initialises the PS2 chunk resolver for the current world.
 *
 * Stores cWorld_m_pTheWorld at +0x0 (On Tour mp_World), @p pNash at +0xC (On Tour mp_Nash) and the world cache header's
 * image count (the short at +0x2A; On Tour m_iNumImages = getNumImages()) at +0x10. Virtual slot 2, called by
 * cWorldCache_init.
 *
 * @param pNash  the texture manager handle cWorldCache_init passes on (On Tour Nash* pNash)
 */
extern "C" void cPS2ChunkResolve_init(void* self, int pNash)
{
    cWorld* w = cWorld_m_pTheWorld;
    *(int*)((char*)self + 0xC) = pNash;
    *(char****)self = (char***)w;
    *(int*)((char*)self + 0x10) = *(short*)(*(char**)w->m_pWorldCache + 0x2A);
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_purge__FPv);
#ifdef SKIP_ASM
/**
 * Releases the PS2 chunk resolver's platform data; does nothing in this build.
 */
void cPS2ChunkResolve_purge(void* self)
{
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_resolve);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sVEntry_AE68 {
    short delta;
    short index;
    void* fn;
};
typedef void* (*tMsg1_AE68)(void*, void*);
typedef void* (*tMsg2_AE68)(void*, unsigned int, void*);
typedef void* (*tMsgS_AE68)(void*, void*, const char*, int, int, unsigned int);

struct sHashKey_AE68 {
    int level;
    int x;
    int y;
    int z;
};
struct sHashCell_AE68 {
    char data[0x14];
};
struct sBox_AE68 {
    float x0, y0, z0, w0;
    float x1, y1, z1, w1;
};

// nash_Nash_findkey is declared again further down with the removal helper's types; bind this one by symbol.
void nash_Nash_findkey_AE68(sHashKey_AE68* out, sBox_AE68* box) __asm__("nash_Nash_findkey");
extern "C" void nash_Octant_addnode(sHashCell_AE68* cell, int type, void* node, sHashKey_AE68* key);
extern char str_strm_tex[];
extern char str_strm_lpg[];
extern "C" void* func_003AD198();
extern "C" void func_00242540(void* self, void* obj);
// PORT: the unit declares this later returning void*; bind this char* view by symbol.
char* cAudioInterface_getGame_AE68() __asm__("cAudioInterface_getGame");
extern "C" void func_002B6B08(void* self, void* obj, unsigned int arg);
extern char* D_004A28A8_AE68 __asm__("SSXApp");
extern "C" int cAI_swaploadInjectedPathFile(void* self, int a, unsigned int b, void* obj);
void cMemMan_free(void* p);
extern "C" void cWorldPainterMan_load(void* self, int a, void* obj);
extern "C" void func_003ACA10(void* self, int a, void* obj);
extern "C" void* func_003AD188();
extern "C" void cGame_loadWScriptManSection(void* self, int a);
void gCamTriggerMan_resolve(void* a, int b);
extern "C" void cSSXScriptEngine_AttachAnchorDataTable(void* self, int a, void* obj);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void func_001547D8_AE68(void* self, int a, void* p, int v) __asm__("func_001547D8__FPv");
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_00286CA8(void* self, unsigned int arg, void* obj);
extern "C" void func_002105D0(void* obj);
extern "C" void cAvalanche_resolve(void* obj);

// Puts obj into the space hash; the box is the 6 floats at obj+off.
static inline void insert_AE68(void* self, int type, char* obj, int off)
{
    sBox_AE68 b;
    sHashKey_AE68 k;
    float one = 1.0f;
    b.x0 = *(float*)(obj + off + 0);
    b.y0 = *(float*)(obj + off + 4);
    b.z0 = *(float*)(obj + off + 8);
    b.w0 = one;
    b.x1 = *(float*)(obj + off + 12);
    b.y1 = *(float*)(obj + off + 16);
    b.z1 = *(float*)(obj + off + 20);
    b.w1 = one;
    sHashCell_AE68* r = *(sHashCell_AE68**)((char*)self + 0xC);
    nash_Nash_findkey_AE68(&k, &b);
    sHashCell_AE68* c1 = k.x < 0 ? r + 4 : r;
    sHashCell_AE68* c2 = k.y < 0 ? c1 + 2 : c1;
    sHashCell_AE68* c3 = k.z < 0 ? c2 + 1 : c2;
    nash_Octant_addnode(c3, type, obj, &k);
}

#define VT_AE68(self, off) ((sVEntry_AE68*)(*(char**)((char*)(self) + 8) + (off)))
#define CALL1_AE68(self, off, obj) \
    ((tMsg1_AE68)VT_AE68(self, off)->fn)((char*)(self) + VT_AE68(self, off)->delta, obj)

/**
 * Fixes up a chunk of streamed world data after it loads, by data type, and hands it to its owner.
 *
 * Models, instances, splines and the like are resolved through the resolver's own virtuals, and several go
 * into the space hash by their bounding box. Textures ("strm_tex") and light pages ("strm_lpg") go to the
 * graphics manager; others go to the world painter (15), the world-script cache (16), the script engine's
 * anchor table (18), the AI path data (14, freed if refused) and other systems.
 *
 * @param msg  the data type (On Tour eBxStreamDataType)
 * @param RID  the resource id (low byte: section; the rest: index)
 * @param obj  the loaded data (On Tour pData)
 * @return @p obj
 */
extern "C" void* cPS2ChunkResolve_resolve(void* self, int msg, unsigned int RID, char* obj)
{
    switch (msg) {
    case 1:
        *(unsigned short*)(obj + 0x1A2) += *(int*)((char*)self + 0x10);
        insert_AE68(self, 1, obj, 344);
        break;
    case 9: {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        sVEntry_AE68* vtEntry = (sVEntry_AE68*)((char*)w->mVtbl + 0x170);
        ((tMsgS_AE68)vtEntry->fn)((char*)w + vtEntry->delta, obj, str_strm_tex, 0, 0, RID >> 8);
        break;
    }
    case 10: {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        sVEntry_AE68* vtEntry = (sVEntry_AE68*)((char*)w->mVtbl + 0x170);
        ((tMsgS_AE68)vtEntry->fn)((char*)w + vtEntry->delta, obj, str_strm_lpg, 0, 0, (RID >> 8) + *(int*)((char*)self + 0x10));
        break;
    }
    case 3:
        CALL1_AE68(self, 104, obj);
        insert_AE68(self, 0, obj, 0x60);
        break;
    case 2:
        CALL1_AE68(self, 40, obj);
        break;
    case 0:
        CALL1_AE68(self, 72, obj);
        break;
    case 6:
        if (CALL1_AE68(self, 168, obj)) {
            *(int*)(obj + 8) = 6;
            insert_AE68(self, 2, obj, 68);
        }
        break;
    case 7:
        CALL1_AE68(self, 160, obj);
        *(int*)(obj + 8) = 8;
        insert_AE68(self, 2, obj, 40);
        break;
    case 8: {
        CALL1_AE68(self, 144, obj);
        char* child = *(char**)(obj + 0x24);
        for (int i = 0; i < *(int*)(obj + 0x20); i++) {
            *(int*)(child + 8) = 1;
            insert_AE68(self, 2, child, 108);
            child = *(char**)(child + 0x64);
        }
        break;
    }
    case 5:
        CALL1_AE68(self, 136, obj);
        *(int*)(obj + 8) = 7;
        insert_AE68(self, 2, obj, 104);
        break;
    case 4:
        CALL1_AE68(self, 0x70, obj);
        break;
    case 12:
        ((tMsg2_AE68)VT_AE68(self, 152)->fn)((char*)self + VT_AE68(self, 0x98)->delta, RID, obj);
        break;
    case 13:
        func_002B6B08(*(void**)(cAudioInterface_getGame_AE68() + 0x4C), obj, RID);
        break;
    case 17:
        gCamTriggerMan_resolve((void*)RID, (int)obj);
        break;
    case 15:
        if (cWorld_m_pTheWorld) {
            cWorldPainterMan_load((char**)cWorld_m_pTheWorld->m_pWorldPainterMan, RID & 0xFF, obj);
        }
        break;
    case 16:
        if (cWorld_m_pTheWorld) {
            int sectionIdx = RID & 0xFF;
            func_003ACA10((char**)cWorld_m_pTheWorld->m_pWScriptCache, sectionIdx, obj);
            cGame_loadWScriptManSection(func_003AD188(), sectionIdx);
        }
        break;
    case 18:
        cSSXScriptEngine_AttachAnchorDataTable(ScriptEngine, RID & 0xFF, obj);
        break;
    case 20:
        func_00286CA8(cAudioInterface_bxAudio(), RID, obj);
        break;
    case 19:
        func_001547D8_AE68(cBE_getInterface_Fv(cBE_getBE(), 10), RID & 0xFF, obj + 4, *(int*)obj);
        break;
    case 21:
        func_002105D0(obj);
        break;
    case 11:
        func_00242540(func_003AD198(), obj);
        break;
    case 14:
        if (!cAI_swaploadInjectedPathFile(*(void**)(*(char**)(D_004A28A8_AE68 + 0x84) + 0xC), RID & 0xFF, RID >> 8, obj)) {
            if (obj) {
                cMemMan_free(obj);
            }
        }
        break;
    case 22:
        cAvalanche_resolve(obj);
        break;
    }
    return obj;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_unresolve);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void operator_delete(int*);
extern "C" void cAI_removeInjectedFile(void* p, unsigned char id);
// PORT: func_001547E0__FPv is called with a second (byte) argument here.
void func_001547E0_2(void* iface, unsigned char id) __asm__("func_001547E0__FPv");
extern "C" void func_0016CF18(unsigned int param);
extern "C" void func_00210608(void* data);
extern "C" void func_00242570(void* p, void* data);
extern "C" void cSSXScriptEngine_DetachAnchorDataTable(void* p, unsigned char id);
extern "C" void cSSXAudio_UnAttachAudioBank(void* p, unsigned int param, void* data);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void* cAudioInterface_getGame();
extern "C" void func_002B6B30(void* p, void* data, unsigned int param);
extern "C" void cWorldPainterSection_unload(void* entry);
extern "C" void cAvalanche_reset();
extern "C" void nash_Cell_remnode(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void* func_003AD198();

struct sWVVec4_B498 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sWVBox_B498 {
    sWVVec4_B498 min;
    sWVVec4_B498 max;
};
struct sWVCell_B498 {
    int level;
    int x, y, z;
};
struct sWVNode_B498 {
    sWVNode_B498* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sWVRoot_B498 {
    sWVCell_B498 cell;              // 0x00
    sWVNode_B498* node;             // 0x10
};
struct sWVEntry_B498 {
    short delta;
    short index;
    void* fn;
};

static inline sWVRoot_B498* findRoot_B498(char* w, int x, int y, int z)
{
    sWVRoot_B498* r = (sWVRoot_B498*)w;
    sWVRoot_B498* rx = x >= 0 ? r : r + 4;
    sWVRoot_B498* ry = y >= 0 ? rx : rx + 2;
    sWVRoot_B498* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

#define WV_MAKEBOX_B498(box, base, off)               \
    (box).min.x = *(float*)((base) + (off) + 0x0);     \
    (box).min.y = *(float*)((base) + (off) + 0x4);     \
    (box).min.z = *(float*)((base) + (off) + 0x8);     \
    (box).min.w = 1.0f;                                \
    (box).max.x = *(float*)((base) + (off) + 0xC);     \
    (box).max.y = *(float*)((base) + (off) + 0x10);    \
    (box).max.z = *(float*)((base) + (off) + 0x14);    \
    (box).max.w = 1.0f

static inline void update_B498(char* w, int idx, void* item, const sWVBox_B498* box)
{
    (*(int*)(w + 0xA0))++;
    sWVCell_B498 c;
    nash_Nash_findkey(&c, box);
    sWVRoot_B498* r = findRoot_B498(w, c.x, c.y, c.z);
    nash_Cell_remnode(r->node, idx, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

/**
 * Undoes the resolve of a stream data chunk that is about to be unloaded.
 *
 * Dispatches on @p msg: spatial types take their nodes out of the world's spatial hash (nash_Nash_findkey, nash_Cell_remnode)
 * and free them; type 13 purges a world audio file (func_002B6B30), 15 unloads a world-painter section (cWorldPainterSection_unload),
 * 18 detaches a script anchor table, 20 unattaches an audio bank, 19 removes mission stream data through BE interface
 * 10, 11 removes an occlusion curtain (func_00242570), 14 drops an AI file (cAI_removeInjectedFile) and 22 resets the avalanche;
 * types 17 and 21 go to func_0016CF18 and func_00210608. Other types need nothing.
 *
 * @param msg  stream data type of the chunk (On Tour eBxStreamDataType type)
 * @param resourceId  resource id of the chunk (On Tour RID)
 * @param data  the chunk's data (On Tour pData)
 * @return @p data, or 0 for type 14
 */
extern "C" void* cPS2ChunkResolve_unresolve(char* self, unsigned int msg, unsigned int resourceId, char* data)
{
    switch (msg) {
    case 0:
        break;
    case 1: {
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 344);
        update_B498(*(char**)(self + 0xC), 1, data, &box);
        break;
    }
    case 9: {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        sWVEntry_B498* vt = (sWVEntry_B498*)ps2GraphicsMan->mVtbl;
        ((void (*)(void*, unsigned int))vt[50].fn)((char*)ps2GraphicsMan + vt[50].delta, resourceId >> 8);
        break;
    }
    case 10: {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        sWVEntry_B498* vt = (sWVEntry_B498*)ps2GraphicsMan->mVtbl;
        ((void (*)(void*, unsigned int))vt[50].fn)((char*)ps2GraphicsMan + vt[50].delta, (resourceId >> 8) + *(int*)(self + 0x10));
        break;
    }
    case 3: {
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 0x60);
        update_B498(*(char**)(self + 0xC), 0, data, &box);
        break;
    }
    case 8: {
        int i = 0;
        char* e = *(char**)(data + 0x24);
        if (i < *(int*)(data + 0x20)) {
            int one = 1;
            do {
                *(int*)(e + 0x8) = one;
                sWVBox_B498 box;
                WV_MAKEBOX_B498(box, e, 108);
                update_B498(*(char**)(self + 0xC), 2, e, &box);
                e = *(char**)(e + 0x64);
                i++;
            } while (i < *(int*)(data + 0x20));
        }
        break;
    }
    case 6: {
        sWVEntry_B498* vt = *(sWVEntry_B498**)(self + 0x8);
        if (((int (*)(void*, void*))vt[22].fn)(self + vt[22].delta, data)) {
            *(int*)(data + 0x8) = 6;
            sWVBox_B498 box;
            WV_MAKEBOX_B498(box, data, 68);
            update_B498(*(char**)(self + 0xC), 2, data, &box);
        }
        break;
    }
    case 7: {
        *(int*)(data + 0x8) = 8;
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 40);
        update_B498(*(char**)(self + 0xC), 2, data, &box);
        break;
    }
    case 5: {
        *(int*)(data + 0x8) = 7;
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 104);
        update_B498(*(char**)(self + 0xC), 2, data, &box);
        break;
    }
    case 13:
        func_002B6B30(*(void**)((char*)cAudioInterface_getGame() + 0x4C), data, resourceId);
        break;
    case 17:
        func_0016CF18(resourceId);
        break;
    case 15:
        if (cWorld_m_pTheWorld != 0) {
            cWorldPainterSection_unload(((char**)(char**)cWorld_m_pTheWorld->m_pWorldPainterMan)[1] + (unsigned char)resourceId * 12);
        }
        break;
    case 18:
        cSSXScriptEngine_DetachAnchorDataTable(ScriptEngine, resourceId);
        break;
    case 20:
        cSSXAudio_UnAttachAudioBank(cAudioInterface_bxAudio(), resourceId, data);
        break;
    case 19:
        func_001547E0_2(cBE_getInterface_Fv(cBE_getBE(), 10), resourceId);
        break;
    case 21:
        func_00210608(data);
        break;
    case 11:
        func_00242570(func_003AD198(), data);
        break;
    case 14:
        data = 0;
        cAI_removeInjectedFile(SSXApp->Game->AI, resourceId);
        break;
    case 22:
        cAvalanche_reset();
        break;
    }
    return data;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_resolveModel);
#ifdef SKIP_ASM
struct cPS2ChunkResolve_resolveModel_sTab {
    int f0;
    unsigned int* entries;
};
struct cPS2ChunkResolve_resolveModel_sLib {
    int f0;
    int f4;
    cPS2ChunkResolve_resolveModel_sTab** tables;
};

class cPS2ChunkResolve_resolveModel_cObj {
public:
    cPS2ChunkResolve_resolveModel_sLib** f0;
    int f4;
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06(void* a1);
};

extern "C" int func_003ABF20(void* self, void* a1);

static inline unsigned int cPS2ChunkResolve_resolveModel_lookup(cPS2ChunkResolve_resolveModel_cObj* self, unsigned int id)
{
    cPS2ChunkResolve_resolveModel_sTab* t = (*self->f0)->tables[id & 0xFF];
    unsigned int r;
    unsigned int v;
    if (t == 0 || (v = t->entries[id >> 8] >> 8) == 0) {
        r = 0;
    } else {
        r = v << 2;
    }
    return r;
}

/**
 * Fixes up a streamed model record on the PS2.
 *
 * Turns its offsets at +0x8 and +0xC into pointers, replaces each resource id in the table at +0xC (a count,
 * then the ids) with its pointer, then calls virtual slot 6 (probably resolveModelHeirarchy) and
 * func_003ABF20().
 *
 * @param pModel  the model record (On Tour tModelRecord*)
 * @return always 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
extern "C" int cPS2ChunkResolve_resolveModel(cPS2ChunkResolve_resolveModel_cObj* self, void* pModel)
{
    int i = 0;
    *(int*)((char*)pModel + 0x8) += (int)pModel;
    *(int*)((char*)pModel + 0xC) += (int)pModel;
    int* tbl = *(int**)((char*)pModel + 0xC);
    unsigned int* entry = (unsigned int*)(tbl + 1);
    for (; i < *tbl; i++, entry++) {
        *entry = cPS2ChunkResolve_resolveModel_lookup(self, *entry);
    }
    self->v06(pModel);
    func_003ABF20(self, pModel);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003ABF20);
#ifdef SKIP_ASM
extern "C" void func_003ABF50(void* self, void* a1);

/**
 * Turns a streamed model's DMA data offset (+0x24) into a pointer and fixes up its mesh packets (func_003ABF50).
 *
 * Called by cPS2ChunkResolve_resolveModel.
 *
 * @param list  the tModelRecord
 * @return 1
 */
// PORT: adds a pointer to an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int func_003ABF20(void* self, void* list)
{
    *(int*)((char*)list + 0x24) += (int)list;
    func_003ABF50(self, list);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003ABF50);
#ifdef SKIP_ASM
extern "C" void func_003ABFC0(void* self, void* item, void* list);

struct func_003ABF50_sEntry {
    int field_0x0;
    void* item;     // 0x4
    int field_0x8;
    int field_0xC;
};

struct func_003ABF50_sList {
    int field_0x0;
    int count;                       // 0x4
    func_003ABF50_sEntry* entries;   // 0x8
};

// PORT: the unit declares func_003ABF50 as returning void (func_003ABF20 calls it that way),
// but the body returns 1; bind the int-returning body to the symbol with an asm label.
int func_003ABF50_impl(void* self, func_003ABF50_sList* list) __asm__("func_003ABF50");

/**
 * Fixes up the DMA packets of each mesh of a model: func_003ABFC0 on every non-null mesh in the model's list (count
 * +0x4, 16-byte entries at +0x8, mesh at +0x4).
 *
 * @param list  the tModelRecord
 * @return 1
 */
int func_003ABF50_impl(void* self, func_003ABF50_sList* list)
{
    int n = list->count;
    func_003ABF50_sEntry* entry = list->entries;
    int i;
    for (i = 0; i < n; i++, entry++) {
        if (entry->item != 0) {
            func_003ABFC0(self, entry->item, list);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003ABFC0);
#ifdef SKIP_ASM
extern "C" void func_003AC048(void* self, void* obj, void* list, int index);

// PORT: the unit declares func_003ABFC0 as returning void (func_003ABF50 calls it that way),
// but the body returns 1; bind the int-returning body to the symbol with an asm label.
int func_003ABFC0_impl(void* self, void* item, void* list) __asm__("func_003ABFC0");

/**
 * Fixes up a mesh's DMA packets: func_003AC048 on each of its +0x1C sub-meshes (pointers at +0x20), with its index.
 *
 * @param item  the mesh
 * @param list  the tModelRecord
 * @return 1
 */
int func_003ABFC0_impl(void* self, void* item, void* list)
{
    void** childPtr = *(void***)((char*)item + 0x20);
    int i;
    for (i = 0; i < *(int*)((char*)item + 0x1C); i++, childPtr++) {
        func_003AC048(self, *childPtr, list, i);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AC048);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern char D_2270[];
extern char D_22C8[];
extern char D_00509400[];
extern char D_00509410[];
extern char D_00509420[];

// PORT: func_003ABFC0 declares this symbol as returning void; bound by asm label.
extern "C" int func_003AC048_impl(void* self, char* obj, char* ctx, int index) __asm__("func_003AC048");
/**
 * Patches one sub-mesh's DMA chain in place so it can be sent to VU1.
 *
 * Walks the chain from @p obj's +0x4 offset, relative to the model's data (+0x24), until a ret tag (ID 6). Each pair of
 * ref tags (ID 3) gets its offsets turned into addresses, and the last quadword of the second block gets the VIF code
 * MSCAL of a VU1 microprogram (0x14000000 | address >> 3): D_2270 for the first pair, D_22C8 for later ones. When bit
 * 31 of a tag's address is set (cleared here), or @p obj's flags have 0x60000, extra ref tags to the shared packets
 * D_00509400, D_00509410 (first) or D_00509420 that func_003AD290 builds are written into the chain, ending with a ret
 * tag (0x60000000) before the chain's own end.
 *
 * @param obj  the sub-mesh
 * @param ctx  the tModelRecord
 * @param index  the sub-mesh index (unused)
 * @return 1
 */
extern "C" int func_003AC048_impl(void* self, char* obj, char* ctx, int index)
{
    int done = 0;
    int first = 1;
    int split = 0;
    if (*(unsigned int*)obj & 0x60000) split = 1;
    char* tagPtr = (char*)(*(int*)(obj + 4) + *(int*)(ctx + 0x24));
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case 3: {
            if (*(unsigned int*)(tagPtr + 4) & 0x80000000) {
                split = 1;
                *(unsigned int*)(tagPtr + 4) &= 0x7FFFFFFF;
            }
            *(int*)(tagPtr + 4) = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
            tagPtr += 0x10;
            tag = *(long*)tagPtr;
            int* ap = (int*)(tagPtr + 4);
            *ap = *(int*)(ctx + 0x24) + *ap;
            int qwc = ((int)tag) & 0xFFFF;
            tagPtr += 0x10;
            char* lastQword = *(char**)ap;
            lastQword += (qwc - 1) * 0x10;
            if (first) {
                *(int*)(lastQword + 8) = ((unsigned int)D_2270 >> 3) | 0x14000000;
            } else {
                *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
            }
            if (split) {
                char* next = tagPtr + 0x10;
                if ((((int)((*(long*)next << 4) >> 32)) & 7) == 6) {
                    *(long*)(tagPtr + 0x8) = 0;
                    *(long*)tagPtr = ((unsigned long)(unsigned int)D_00509400 << 32) | 0x30000001;
                    if (first) {
                        *(long*)(tagPtr + 0x10) = ((unsigned long)(unsigned int)D_00509410 << 32) | 0x30000001;
                        *(long*)(tagPtr + 0x18) = 0;
                    } else {
                        *(long*)(tagPtr + 0x10) = ((unsigned long)(unsigned int)D_00509420 << 32) | 0x30000001;
                        *(long*)(tagPtr + 0x18) = 0;
                    }
                    *(long*)(tagPtr + 0x20) = 0x60000000;
                    *(long*)(tagPtr + 0x28) = 0;
                } else {
                    if (*(unsigned int*)(tagPtr + 4) & 0x80000000) {
                        split = 1;
                        *(unsigned int*)(tagPtr + 4) &= 0x7FFFFFFF;
                    }
                    *(int*)(tagPtr + 4) = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
                    tagPtr = next;
                    tag = *(long*)tagPtr;
                    ap = (int*)(tagPtr + 4);
                    *ap = *(int*)(ctx + 0x24) + *ap;
                    qwc = ((int)tag) & 0xFFFF;
                    tagPtr += 0x10;
                    lastQword = *(char**)ap;
                    lastQword += (qwc - 1) * 0x10;
                    *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
                    if (first) {
                        *(long*)tagPtr = ((unsigned long)(unsigned int)D_00509410 << 32) | 0x30000001;
                    } else {
                        *(long*)tagPtr = ((unsigned long)(unsigned int)D_00509420 << 32) | 0x30000001;
                    }
                    *(long*)(tagPtr + 0x8) = 0;
                    *(long*)(tagPtr + 0x10) = ((unsigned long)(unsigned int)D_00509420 << 32) | 0x30000001;
                    *(long*)(tagPtr + 0x18) = 0;
                }
                tagPtr += 0x20;
            }
            first = 0;
            break;
        }
        case 6:
            tagPtr += 0x10;
            done = 1;
            break;
        }
    } while (done == 0);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_resolveInstance);
#ifdef SKIP_ASM
struct cPS2ChunkResolve_resolveInstance_sRes {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct cPS2ChunkResolve_resolveInstance_sTab {
    int pad[5];
    unsigned int* entries;
};
struct cPS2ChunkResolve_resolveInstance_sLib {
    int f0;
    int f4;
    cPS2ChunkResolve_resolveInstance_sTab** tables;
};
struct cPS2ChunkResolve_resolveInstance_sView {
    int f0;
    int f4;
};
struct cPS2ChunkResolve_resolveInstance_cObj {
    cPS2ChunkResolve_resolveInstance_sLib** f0;
    cPS2ChunkResolve_resolveInstance_sView* f4;
};

extern "C" int func_003AC358(void* self, void* a1);

static inline cPS2ChunkResolve_resolveInstance_sRes* cPS2ChunkResolve_resolveInstance_lookup(cPS2ChunkResolve_resolveInstance_cObj* self, unsigned int id)
{
    cPS2ChunkResolve_resolveInstance_sTab* t = (*self->f0)->tables[id & 0xFF];
    unsigned int v;
    if (t == 0 || (v = t->entries[id >> 8] >> 8) == 0) {
        return 0;
    }
    return (cPS2ChunkResolve_resolveInstance_sRes*)(v << 2);
}

/**
 * Fixes up a streamed instance record on the PS2.
 *
 * Replaces its model resource id (+0x80) with the model pointer, copies the model's +0xC word to +0x94,
 * clears +0x8C, stores the resolver's view at +0x88, rebuilds the flag word (+0x8) from it and calls
 * func_003AC358().
 *
 * @param pInstance  the instance record (On Tour tInstanceRecord*)
 * @return always 1
 */
// PORT: stores pointers in int fields; not 64-bit safe.
extern "C" int cPS2ChunkResolve_resolveInstance(cPS2ChunkResolve_resolveInstance_cObj* self, void* pInstance)
{
    cPS2ChunkResolve_resolveInstance_sRes* resolved = cPS2ChunkResolve_resolveInstance_lookup(self, *(unsigned int*)((char*)pInstance + 0x80));
    *(cPS2ChunkResolve_resolveInstance_sRes**)((char*)pInstance + 0x80) = resolved;
    *(int*)((char*)pInstance + 0x94) = resolved->fC;
    *(int*)((char*)pInstance + 0x8C) = 0;
    *(cPS2ChunkResolve_resolveInstance_sView**)((char*)pInstance + 0x88) = self->f4;
    int x = self->f4->f4 & 0xFFFF0000;
    *(int*)((char*)pInstance + 0x8) = x | (x >> 16) | 2;
    func_003AC358(self, pInstance);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AC358);
#ifdef SKIP_ASM
struct func_003AC358_sObj {
    char pad[0x1C];
    int count;
};
struct func_003AC358_sEnt {
    int f0;
    func_003AC358_sObj* obj;
    int f8;
    int fC;
};
struct func_003AC358_sList {
    int f0;
    int count;
    func_003AC358_sEnt* entries;
};

extern "C" void* func_003AC508(void* self, void* a1, func_003AC358_sList* list, void* cur);

/**
 * Turns a streamed instance's DMA data offset (+0x98) into a pointer and fixes up its per-instance packets.
 *
 * For every sub-mesh of every mesh of the instance's model (+0x80, as in func_003ABF50), func_003AC508 patches the next
 * chain in the instance's data, continuing where the previous one ended. Called by cPS2ChunkResolve_resolveInstance.
 *
 * @param instance  the tInstanceRecord
 * @return 1
 */
// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int func_003AC358(void* self, void* instance)
{
    int i;
    func_003AC358_sList* list;
    void* cur;
    *(int*)((char*)instance + 0x98) += (int)instance;
    list = *(func_003AC358_sList**)((char*)instance + 0x80);
    cur = *(void**)((char*)instance + 0x98);
    int n = list->count;
    func_003AC358_sEnt* entry = list->entries;
    for (i = 0; i < n; i++, entry++) {
        func_003AC358_sObj* object = entry->obj;
        if (object != 0) {
            int j;
            for (j = 0; j < object->count; j++) {
                cur = func_003AC508(self, instance, list, cur);
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AC438);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern char D_2320[];
extern char D_2358[];

/**
 * Patches a DMA chain in place like func_003AC048 without the split handling, using the microprograms D_2320 (first
 * pair) and D_2358.
 *
 * Offsets are relative to @p ctx's +0x24 data. No C caller in the source.
 *
 * @param a0  unused
 * @param a1  unused
 * @param ctx  the record whose +0x24 data the offsets are relative to
 * @param tagPtr  the first DMA tag
 * @return the address after the ret tag
 */
extern "C" char* func_003AC438(int a0, int a1, char* ctx, char* tagPtr)
{
    int done = 0;
    int first = 1;
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case 3: {
            *(int*)(tagPtr + 4) = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
            tagPtr += 0x10;
            tag = *(long*)tagPtr;
            int* ap = (int*)(tagPtr + 4);
            *ap = *(int*)(ctx + 0x24) + *ap;
            int qwc = ((int)tag) & 0xFFFF;
            tagPtr += 0x10;
            char* tail = *(char**)ap;
            tail += (qwc - 1) * 0x10;
            if (first) {
                *(int*)(tail + 8) = ((unsigned int)D_2320 >> 3) | 0x14000000;
                first = 0;
            } else {
                *(int*)(tail + 8) = ((unsigned int)D_2358 >> 3) | 0x14000000;
            }
            break;
        }
        case 6:
            tagPtr += 0x10;
            done = 1;
            break;
        }
    } while (done == 0);
    return tagPtr;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AC508);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); pointers held in int; VU microprogram addresses.
extern char D_2270[];
extern char D_22C8[];
extern char D_00509400[];

/**
 * Patches one per-instance DMA chain in place, like func_003AC048.
 *
 * The first ref of each pair points into the model's data (+0x24 of @p list), the second into the instance's own data
 * (+0x98 of @p instance). MSCAL addresses are D_2270 for the first pair and D_22C8 after; a pair whose address has bit 31 set
 * gets the extra refs to D_00509400 (or two more converted refs) as in func_003AC048.
 *
 * @param instance  the tInstanceRecord
 * @param list  the tModelRecord
 * @param cur  the first DMA tag of this chain
 * @return the address after the ret tag (the next chain)
 */
// Walks one instance DMA chain: relocates the ref tags (+0x98 instance base for geometry, +0x24 for the
// rest) and patches each packet's MSCAL (first one D_2270, later ones D_22C8).
extern "C" void* func_003AC508(void* self, void* instance, func_003AC358_sList* list, void* cur)
{
    char* ctx = (char*)list;
    char* tagPtr = (char*)cur;
    int done = 0;
    int first = 1;
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case 3: {
            int flag = 0;
            if (*(int*)(tagPtr + 4) & 0x80000000) {
                *(int*)(tagPtr + 4) &= 0x7FFFFFFF;
                flag = 1;
            }
            *(int*)(tagPtr + 4) = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
            tagPtr += 0x10;
            tag = *(long*)tagPtr;
            int* ap = (int*)(tagPtr + 4);
            *ap = *(int*)((char*)instance + 0x98) + *ap;
            int qwc = ((int)tag) & 0xFFFF;
            tagPtr += 0x10;
            char* lastQword = *(char**)ap;
            lastQword += (qwc - 1) * 0x10;
            if (first) {
                *(int*)(lastQword + 8) = ((unsigned int)D_2270 >> 3) | 0x14000000;
            } else {
                *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
            }
            if (flag) {
                char* next = tagPtr + 0x10;
                if ((((int)((*(long*)next << 4) >> 32)) & 7) == 6) {
                    tag = *(long*)tagPtr;
                    int addr = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
                    qwc = ((int)tag) & 0xFFFF;
                    lastQword = (char*)addr + (qwc - 1) * 0x10;
                    if (first) {
                        *(int*)(lastQword + 8) = ((unsigned int)D_2270 >> 3) | 0x14000000;
                    } else {
                        *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
                    }
                    *(long*)(tagPtr + 0x20) = 0x60000000;
                    *(long*)(tagPtr + 0x10) = (long)(0x30000000 | qwc) | ((long)addr << 32);
                    *(long*)(tagPtr + 0x00) = ((unsigned long)(unsigned int)D_00509400 << 32) | 0x30000001;
                    *(long*)(tagPtr + 0x08) = 0;
                    *(long*)(tagPtr + 0x18) = 0;
                    *(long*)(tagPtr + 0x28) = 0;
                    tagPtr += 0x20;
                } else {
                    if (*(int*)(tagPtr + 4) & 0x80000000) {
                        *(int*)(tagPtr + 4) &= 0x7FFFFFFF;
                    }
                    *(int*)(tagPtr + 4) = *(int*)(ctx + 0x24) + *(int*)(tagPtr + 4);
                    tagPtr = next;
                    tag = *(long*)tagPtr;
                    ap = (int*)(tagPtr + 4);
                    *ap = *(int*)((char*)instance + 0x98) + *ap;
                    qwc = ((int)tag) & 0xFFFF;
                    tagPtr += 0x10;
                    lastQword = *(char**)ap;
                    lastQword += (qwc - 1) * 0x10;
                    *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
                    tag = *(long*)tagPtr;
                    ap = (int*)(tagPtr + 4);
                    *ap = *(int*)(ctx + 0x24) + *ap;
                    qwc = ((int)tag) & 0xFFFF;
                    tagPtr += 0x10;
                    lastQword = *(char**)ap;
                    lastQword += (qwc - 1) * 0x10;
                    if (first) {
                        *(int*)(lastQword + 8) = ((unsigned int)D_2270 >> 3) | 0x14000000;
                    } else {
                        *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
                    }
                    tag = *(long*)tagPtr;
                    ap = (int*)(tagPtr + 4);
                    *ap = *(int*)(ctx + 0x24) + *ap;
                    qwc = ((int)tag) & 0xFFFF;
                    tagPtr += 0x10;
                    lastQword = *(char**)ap;
                    lastQword += (qwc - 1) * 0x10;
                    *(int*)(lastQword + 8) = ((unsigned int)D_22C8 >> 3) | 0x14000000;
                }
            }
            first = 0;
            break;
        }
        case 6:
            tagPtr += 0x10;
            done = 1;
            break;
        }
    } while (done == 0);
    return tagPtr;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWScriptCache_cWScriptCache);
#ifdef SKIP_ASM
extern "C" void cWScriptCache_init(void* self, int a1);

/**
 * Constructs the world-script cache for a world, sized for one section to start with.
 *
 * @param world  the owning world (On Tour cWorld*)
 * @return self
 */
extern "C" void* cWScriptCache_cWScriptCache(void* self, int world)
{
    *(int*)((char*)self + 0x0) = world;
    *(int*)((char*)self + 0x4) = 0;
    cWScriptCache_init(self, 1);
    return self;
}
#endif

