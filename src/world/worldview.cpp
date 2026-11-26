#include "common.h"
#include "bx/math3d.h"
#include "be/beinterface.h"
#include "visualfx/lensfx.h"
#include "object/rollermodifier.h"
#include "world/instancerecord.h"
#include "visualfx/worldlightman.h"
#include "world/worldrecords.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "bx/appman.h"
#include "util/list.h"
#include "world/ps2chunkresolve.h"
#include "world/chunkresolve.h"
#include "wscript/wscriptcache.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "world/worldview.h"
#include "world/worldcache.h"
#include "render/graphicsman.h"
#include "render/ps2lightman.h"

//100%
INCLUDE_ASM("world/worldview", cWorldView_getNumSections__FP10cWorldView);
#ifdef SKIP_ASM
/**
 * Returns the number of sections in the world view.
 *
 * @return mp_Hdr->m_uiNumSections, or 0 when there is no section table
 */
int cWorldView::getNumSections()
{
    if (mp_Hdr == 0) {
        return 0;
    }
    return mp_Hdr->m_uiNumSections;
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
void* cWorldView::getSectionName(unsigned int i)
{
    tStreamDataHeader* list = mp_Hdr;
    if (list == 0 || i >= (unsigned int)list->m_uiNumSections) {
        return 0;
    }
    return (char*)&mp_Section[i];
}
#endif


//100%
INCLUDE_ASM("world/worldview", cWorldView_isSectionLoaded__FP10cWorldViewi);
#ifdef SKIP_ASM
/**
 * Tells whether a world section is loaded (stream state 5, or 6 when it is also active).
 *
 * @param section  section id
 * @return true when loaded, false otherwise (also for an out-of-range id)
 */
bool cWorldView::isSectionLoaded(int section)
{
    if (mp_Hdr == 0 || (unsigned int)section >= (unsigned int)mp_Hdr->m_uiNumSections) {
        return false;
    }
    return (unsigned int)(mp_SectionState[section].state - kSection_Loaded) < 2;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_isSectionUnLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a world section is fully unloaded (stream state 0).
 *
 * @param i  section id
 * @return true when unloaded, false otherwise (also for an out-of-range id)
 */
bool cWorldView::isSectionUnLoaded(unsigned int i)
{
    tStreamDataHeader* list = mp_Hdr;
    if (list == 0 || i >= (unsigned int)list->m_uiNumSections) {
        return false;
    }
    return mp_SectionState[i].state == 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_loadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Asks the world view to stream in a world section.
 *
 * If the view has section info and @p idx is below its section count, stores @p value as the section's priority and
 * moves its state 0 (unloaded) to 1 (load requested) and 3 to 5; in state 4 the request is refused.
 *
 * @param idx  section id (On Tour id)
 * @param value  load priority (On Tour fPriority)
 * @return false when the section is in state 4 (kSection_UnLoading), else true
 */
bool cWorldView::loadSection(unsigned int idx, float value)
{
    if (mp_Hdr != 0 && idx < mp_Hdr->m_uiNumSections) {
        mp_SectionState[idx].priority = value;
        switch (mp_SectionState[idx].state) {
        case kSection_Empty:
            mp_SectionState[idx].state = kSection_LoadQueue;
            break;
        case kSection_UnLoadQueue:
            mp_SectionState[idx].state = kSection_Loaded;
            break;
        case kSection_UnLoading:
            return false;
        case kSection_LoadQueue: case kSection_Loading: case kSection_Loaded: case kSection_Active:
            break;
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_unloadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Asks for section @p idx to be unloaded.
 *
 * Loaded and Active sections go to UnLoadQueue and a LoadQueue section goes back to Empty; a section still Loading
 * refuses (returns 0). Empty, UnLoadQueue and UnLoading are left as they are. An index past m_uiNumSections, or no
 * mp_Hdr, also returns 1. Called by cSectionMan_update.
 *
 * @param idx  the section id
 * @return false while the section is loading, else true
 */
bool cWorldView::unloadSection(unsigned int idx)
{
    if (mp_Hdr != 0 && idx < mp_Hdr->m_uiNumSections) {
        switch (mp_SectionState[idx].state) {
        case kSection_Loading:
            return false;
        case kSection_Loaded: case kSection_Active:
            mp_SectionState[idx].state = kSection_UnLoadQueue;
            break;
        case kSection_LoadQueue:
            mp_SectionState[idx].state = kSection_Empty;
            break;
        case kSection_Empty: case kSection_UnLoadQueue: case kSection_UnLoading:
            break;
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_activateSection);
#ifdef SKIP_ASM
extern "C" void cWorldCache_activateSectionPatches(void* world, unsigned int i);
/**
 * Activates a loaded world section.
 *
 * A loaded section (state 5) becomes active (6) and the world cache activates its contents (cWorldCache_activateSectionPatches).
 *
 * @param i  section id
 * @return true if the section is now active (or already was), false if it is not loaded or the id is out of range
 */
bool cWorldView::activateSection(unsigned int i)
{
    tStreamDataHeader* list = mp_Hdr;
    if (list != 0 && i < (unsigned int)list->m_uiNumSections) {
        int st = mp_SectionState[i].state;
        if (st == kSection_Loaded) {
            mp_SectionState[i].state = kSection_Active;
            cWorldCache_activateSectionPatches(mp_WorldCache, i);
            return true;
        } else if (st == kSection_Active) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_deactivateSection);
#ifdef SKIP_ASM
/**
 * Moves section @p idx from Active back to Loaded; any other state is kept.
 *
 * No C caller in the source.
 *
 * @param idx  the section id
 * @return true for a valid index, else false
 */
bool cWorldView::deactivateSection(unsigned int idx)
{
    if (mp_Hdr != 0 && idx < (unsigned int)mp_Hdr->m_uiNumSections) {
        if (mp_SectionState[idx].state == kSection_Active) {
            mp_SectionState[idx].state = kSection_Loaded;
        }
        return true;
    }
    return false;
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
void* cWorldView::getHull(int uiIdx)
{
    return (char*)&mp_Hulls[uiIdx];
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
int cWorldView::getNumHull()
{
    return (int)mp_Hdr->m_uiNumBSPHulls;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_isBoxValid);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cWorldCache_isPageLoaded(void* self, int a1);
extern void cWorldView_recursiveIsValid_9AC0(void* self, int sec, int box, int* ok) __asm__("cWorldView_recursiveIsValid");

/**
 * Returns whether every hull page that @p box touches is loaded.
 *
 * First section 0's own hull page (m_BSP.m_iGlobalHull) must be loaded when it has chunks; then each Active section's
 * tree is searched with cWorldView_recursiveIsValid. No C caller in the source.
 *
 * @param box  the box (minimum and maximum corners)
 * @return 1 when all those pages are loaded
 */
int cWorldView::isBoxValid(int box)
{
    tBxSection* cur = mp_Section;
    int ok;
    unsigned i;
    int off = cur->m_BSP.m_iGlobalHull;
    off *= 68;
    if (*(unsigned short*)(off + (int)mp_Hulls) != 0) {
        if (cWorldCache_isPageLoaded(mp_WorldCache, cur->m_BSP.m_iGlobalHull) == 0)
            return 0;
    }
    ok = 1;
    for (i = 0; i < ((cWorldView*)this)->mp_Hdr->m_uiNumSections; i++) {
        if (mp_SectionState[i].state == kSection_Active)
            cWorldView_recursiveIsValid_9AC0(((char*)this), (int)mp_Section[i].m_BSP.m_pRoot, box, &ok);
    }
    return ok;
}
#endif
//100%
INCLUDE_ASM("world/worldview", cWorldView_recursiveIsValid);
#ifdef SKIP_ASM
extern "C" bool cWorldCache_isPageLoaded(void* self, int a1);


struct sSecInfo_9BB8 {
    short f0;
    unsigned short id;  // 0x02
};

struct sSection_9BB8;

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_9BB8(const t4Vector& a, const t4Vector& b)
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

static inline int overlap_9BB8(const t4Vector* c, const tBxBSPNode* s)
{
    int r = 0;
    if (c[0].x <= s->m_v4BoxMax.m[0] && c[1].x >= s->m_v4BoxMin.m[0] &&
        c[0].y <= s->m_v4BoxMax.m[1] && c[1].y >= s->m_v4BoxMin.m[1] &&
        c[0].z <= s->m_v4BoxMax.m[2] && c[1].z >= s->m_v4BoxMin.m[2]) {
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
 * @param sec  the tBxBSPNode
 * @param box  the box (minimum and maximum corners)
 * @param ok  set on entry; cleared when a page is missing
 */
void cWorldView::recursiveIsValid(tBxBSPNode* sec, t4Vector* box, int* ok)
{
    if (*ok == 0)
        return;
    if (!overlap_9BB8(box, sec))
        return;
    if (((sSecInfo_9BB8*)sec->m_pHull) != 0) {
        if (cWorldCache_isPageLoaded(mp_WorldCache, ((sSecInfo_9BB8*)sec->m_pHull)->id) == 0) {
            *ok = 0;
            return;
        }
    }
    if (sec->m_pInChild != 0) {
        if (Dot_9BB8((*(t4Vector*)&sec->m_v4SplitPlaneIn), box[0]) < 0.0f) {
            recursiveIsValid(sec->m_pInChild, box, ok);
            if (*ok == 0)
                return;
        }
    }
    if (sec->m_pOutChild != 0) {
        if (Dot_9BB8((*(t4Vector*)&sec->m_v4SplitPlaneOut), box[1]) < 0.0f)
            recursiveIsValid(sec->m_pOutChild, box, ok);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldview", cWorldView_refreshStreamFocus);
#ifdef SKIP_ASM


extern "C" void cWorldView_recursiveTreeFocus(cWorldView* self, tBxBSPNode* section, int cam, int loaded);

/**
 * Recomputes which hull pages stream focus @p cam needs.
 *
 * Only for an active focus (m_StreamFocus[cam].m_bActive): resets m_fSafeRadius and m_fSafeTestRadius to m_fLoadRadius,
 * runs cWorldView_recursiveTreeFocus on the root node of every Loaded or Active section (setting the safe radius only for Active ones),
 * then stores the app frame (cAppMan_TheAppMan + 0x18) in m_iFocusTick and clears m_bFocusUpdate.
 *
 * @param cam  the focus index (0 or 1)
 */
void cWorldView::refreshStreamFocus(int cam)
{
    if (m_StreamFocus[cam].m_bActive != 0) {
        int frame = cAppMan_TheAppMan->TimerTick;
        float loadRadius = m_StreamFocus[cam].m_fLoadRadius;
        m_StreamFocus[cam].m_fSafeRadius = loadRadius;
        m_StreamFocus[cam].m_fSafeTestRadius = loadRadius;
        unsigned int i;
        for (i = 0; i < (unsigned int)mp_Hdr->m_uiNumSections; i++) {
            int st = ((&mp_SectionState[0]))[i].state;
            if (st == kSection_Loaded || st == kSection_Active) {
                recursiveTreeFocus(mp_Section[i].m_BSP.m_pRoot, cam, st == kSection_Active);
            }
        }
        m_StreamFocus[cam].m_iFocusTick = frame;
        m_StreamFocus[cam].m_bFocusUpdate = 0;
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

// cWorldView::getDistanceToBox(const t4Vector& pos, const t4Vector& boxMin, const t4Vector& boxMax)
extern "C" float cWorldView_getDistanceToBox_impl(void* self, const t4Vector& pos, const t4Vector& boxMin,
                                    const t4Vector& boxMax) __asm__("cWorldView_getDistanceToBox");

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
extern "C" float cWorldView_getDistanceToBox_impl(void* self, const t4Vector& pos, const t4Vector& boxMin,
                                    const t4Vector& boxMax)
{
    bool bSpanX = boxMin.x <= pos.x && pos.x <= boxMax.x;
    bool bSpanY = boxMin.y <= pos.y && pos.y <= boxMax.y;
    bool bSpanZ = boxMin.z <= pos.z && pos.z <= boxMax.z;
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
        float fXdist = pos.x - boxMax.x;
        if (fXdist < 0.0f) {
            fXdist = boxMin.x - pos.x;
        }
        if (bSqrt) {
            fDist += fXdist * fXdist;
        } else {
            fDist += fXdist;
        }
    }
    if (bY) {
        float fYdist = pos.y - boxMax.y;
        if (fYdist < 0.0f) {
            fYdist = boxMin.y - pos.y;
        }
        if (bSqrt) {
            fDist += fYdist * fYdist;
        } else {
            fDist += fYdist;
        }
    }
    if (bZ) {
        float fZdist = pos.z - boxMax.z;
        if (fZdist < 0.0f) {
            fZdist = boxMin.z - pos.z;
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
extern "C" bool cWorldCache_isPageLoaded(void* self, int a1);
extern "C" void cWorldCache_touchPage(void* self, int i, float f);


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_A028(const t4Vector& a, const t4Vector& b)
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

static inline int overlap_A028(const t4Vector* c, const tBxBSPNode* s)
{
    int r = 0;
    if (c[0].x <= s->m_v4BoxMax.m[0] && c[1].x >= s->m_v4BoxMin.m[0] &&
        c[0].y <= s->m_v4BoxMax.m[1] && c[1].y >= s->m_v4BoxMin.m[1] &&
        c[0].z <= s->m_v4BoxMax.m[2] && c[1].z >= s->m_v4BoxMin.m[2]) {
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
 * @param sec_  the tBxBSPNode
 * @param cam  the focus index
 * @param loaded  set for an Active section (updates the safe radius)
 */
void cWorldView::recursiveTreeFocus(tBxBSPNode* sec_, int cam, int loaded)
{
    if (!overlap_A028(&m_StreamFocus[cam].m_FocusBox.MinPoint, sec_))
        return;
    if (sec_->m_pHull != 0) {
        float dist =
            cWorldView_getDistanceToBox(this, &m_StreamFocus[cam].m_v4LoadPos, sec_, sec_->m_v4BoxMax.m);
        float safeRadius = m_StreamFocus[cam].m_fSafeRadius;
        float w;
        if (dist < 0.0f) {
            w = 0.800000011920929f;
            if (cWorldCache_isPageLoaded(mp_WorldCache, sec_->m_pHull->m_uiHullIndex) == 0)
                safeRadius = 0.0f;
        } else {
            if (m_StreamFocus[cam].m_fLoadRadius < dist)
                return;
            w = (1.0f - dist / m_StreamFocus[cam].m_fLoadRadius) * 0.800000011920929f;
            if (dist < safeRadius) {
                if (cWorldCache_isPageLoaded(mp_WorldCache, sec_->m_pHull->m_uiHullIndex) == 0)
                    safeRadius = dist;
            }
        }
        if (loaded)
            m_StreamFocus[cam].m_fSafeRadius = safeRadius;
        if (safeRadius < m_StreamFocus[cam].m_fSafeTestRadius)
            m_StreamFocus[cam].m_fSafeTestRadius = safeRadius;
        cWorldCache_touchPage(mp_WorldCache, sec_->m_pHull->m_uiHullIndex, w);
    }
    if (sec_->m_pInChild != 0) {
        if (Dot_A028((*(t4Vector*)&sec_->m_v4SplitPlaneIn), m_StreamFocus[cam].m_FocusBox.MinPoint) < 0.0f)
            recursiveTreeFocus(sec_->m_pInChild, cam, loaded);
    }
    if (sec_->m_pOutChild != 0) {
        if (Dot_A028((*(t4Vector*)&sec_->m_v4SplitPlaneOut), m_StreamFocus[cam].m_FocusBox.MaxPoint) < 0.0f)
            recursiveTreeFocus(sec_->m_pOutChild, cam, loaded);
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

extern cChunkResolve_cChunkResolve_sState cChunkResolve_cChunkResolve_colliderecord;

struct cChunkResolve_cChunkResolve_sView {
    int f00;
    cChunkResolve_cChunkResolve_sState* state;
    void** vt;
};
// splat labels cChunkResolve_cChunkResolve_colliderecord+8 separately
extern unsigned int D_00508F58[];
extern int D_004A5A68;
extern void* cChunkResolve_vtbl[];

// PORT: the unit declares this `void`, but the body returns self ($2); bound via asm label.
cChunkResolve_cChunkResolve_sView* cChunkResolve_cChunkResolve_impl(cChunkResolve_cChunkResolve_sView* self) __asm__("cChunkResolve_cChunkResolve");

/**
 * Constructs the stream chunk resolver and resets its shared state.
 *
 * Sets cChunkResolve_vtbl, clears +0x0, initializes the static state cChunkResolve_cChunkResolve_colliderecord on first use, points the resolver at it
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
        cChunkResolve_cChunkResolve_colliderecord.f08 = 0xFFFFFFFF;
        D_004A5A68 = 1;
    }
    cChunkResolve_cChunkResolve_sState* sharedState = &cChunkResolve_cChunkResolve_colliderecord;
    self->state = sharedState;
    MEM_fill(sharedState, 0, sizeof(cChunkResolve_cChunkResolve_sState));
    D_00508F58[0] = 0xFFFFFFFF;
    sharedState->f0C = -1;
    cChunkResolve_cChunkResolve_colliderecord.f00 = 0;
    int i;
    for (i = 0; i < 24; i++) {
        sharedState->entries[i].dist = 1000.0f;
        sharedState->entries[i].scale = 0.5f;
        sharedState->entries[i].a = 0;
        sharedState->entries[i].b = -1;
    }
    cChunkResolve_cChunkResolve_colliderecord.flags = (((cChunkResolve_cChunkResolve_colliderecord.flags | 1) & ~4) | 2) << 16;
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
    virtual int resolveModelHeirarchy(void* a1);
};

/**
 * Fixes up a streamed model record: turns its offsets at +0x8 and +0xC into pointers, then resolves the rest
 * through virtual slot 6 (probably resolveModelHeirarchy, On Tour's next method).
 *
 * @param pModel  the model record (On Tour tModelRecord*)
 * @return always 1
 */
// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
int cChunkResolve::resolveModel(tModelRecord* pModel)
{
    *(int*)&pModel->m_MeshHeirarchy += (int)pModel;
    *(int*)&pModel->m_MaterialBlock += (int)pModel;
    ((cChunkResolve_resolveModel_cObj*)this)->resolveModelHeirarchy(pModel);
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
    virtual int resolveMesh(void* p, void* base);
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int resolveAnimation(void* p, void* base);
};


extern t4Matrix t4mat_Identity;

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
int cChunkResolve::resolveModelHeirarchy(tModelRecord* pModel)
{
    int n = pModel->m_iNumMeshes;
    tMeshHeirarchyRecord* meshHierarchy = pModel->m_MeshHeirarchy;
    int i;
    for (i = 0; i < n; i++, meshHierarchy++) {
        if (((int)meshHierarchy->m_Mesh) != 0) {
            (*(int*)&meshHierarchy->m_Mesh) += (int)pModel;
            ((cChunkResolve_resolveModelHeirarchy_cObj*)this)->resolveMesh((void*)((int)meshHierarchy->m_Mesh), pModel);
        }
        if (((unsigned int)meshHierarchy->m_pMeshMatrix) == kNullOffset) {
            (*(unsigned int*)&meshHierarchy->m_pMeshMatrix) = (unsigned int)&t4mat_Identity;
        } else {
            (*(unsigned int*)&meshHierarchy->m_pMeshMatrix) += (int)pModel;
        }
        if (((int)meshHierarchy->m_MeshAnim) != 0) {
            (*(int*)&meshHierarchy->m_MeshAnim) += (int)pModel;
            ((cChunkResolve_resolveModelHeirarchy_cObj*)this)->resolveAnimation((void*)((int)meshHierarchy->m_MeshAnim), pModel);
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
    virtual int resolveMeshContext(void* p, int base);
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
int cChunkResolve::resolveMesh(tMeshRecord* pMesh, int base)
{
    int i = 0;
    int* contextEntry = (int*)(*(int*)&pMesh->m_pContexts += base);
    for (; i < pMesh->m_iNumContexts; i++) {
        *contextEntry += base;
        ((cChunkResolve_resolveMesh_cObj*)this)->resolveMeshContext((void*)*contextEntry++, base);
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
    virtual int resolveParticleModelHeirarchy(void* a1);
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
int cChunkResolve::resolveParticleModel(tParticleModelRecord* pModel)
{
    *(int*)&pModel->m_ParticleMeshHeirarchy += (int)pModel;
    ((cChunkResolve_resolveParticleModel_cObj*)this)->resolveParticleModelHeirarchy(pModel);
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
    virtual int resolveAnimation(void* p, void* base);
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int resolveParticleMesh(void* p);
};


extern t4Matrix t4mat_Identity;

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
int cChunkResolve::resolveParticleModelHeirarchy(tParticleModelRecord* pModel)
{
    int n = pModel->m_iNumParticleMeshes;
    tParticleMeshHeirarchyRecord* meshHierarchy = (tParticleMeshHeirarchyRecord*)pModel->m_ParticleMeshHeirarchy;
    int i;
    for (i = 0; i < n; i++, meshHierarchy++) {
        if (((int)meshHierarchy->m_ParticleMesh) != 0) {
            (*(int*)&meshHierarchy->m_ParticleMesh) += (int)pModel;
            ((cChunkResolve_resolveParticleModelHeirarchy_cObj*)this)->resolveParticleMesh((void*)((int)meshHierarchy->m_ParticleMesh));
        }
        if (((unsigned int)meshHierarchy->m_pParticleMeshMatrix) == kNullOffset) {
            (*(unsigned int*)&meshHierarchy->m_pParticleMeshMatrix) = (unsigned int)&t4mat_Identity;
        } else {
            (*(unsigned int*)&meshHierarchy->m_pParticleMeshMatrix) += (int)pModel;
        }
        if (((int)meshHierarchy->m_ParticleMeshAnim) != 0) {
            (*(int*)&meshHierarchy->m_ParticleMeshAnim) += (int)pModel;
            ((cChunkResolve_resolveParticleModelHeirarchy_cObj*)this)->resolveAnimation((void*)((int)meshHierarchy->m_ParticleMeshAnim), 0);
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
int cChunkResolve::resolveParticleMesh(tParticleMeshRecord* pMesh)
{
    *(int*)&pMesh->m_ParticleVolumes += (int)pMesh;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveParticleInstance);
#ifdef SKIP_ASM
/**
 * Resolves a particle instance's model reference after its section is loaded.
 *
 * The id in @p pInstance's m_Model selects a section (low byte) and an entry (upper bits) of the section's
 * particle-model cache table; the entry's address (m_bfuiMemLoc, times 4) replaces the id, or 0 when the section is not
 * loaded or the entry is empty.
 *
 * @param pInstance  the particle instance record (On Tour pInstance)
 * @return 1
 */
// PORT: decodes an int handle into a pointer ((void*)(i << 2)); not 64-bit safe.
int cChunkResolve::resolveParticleInstance(tParticleInstanceRecord* pInstance)
{
    unsigned int resourceId = (unsigned int)pInstance->m_Model;
    cWorldCacheTable* sec = mp_World->m_pWorldCache->m_pSectionCacheTable[resourceId & 0xFF];
    void* res;
    unsigned int i;
    // m_pCacheTable[4]: kBxStreamData_ParticleModel
    if (sec == 0 || (i = sec->m_pCacheTable[4].m_pDataTag[resourceId >> 8].m_bfuiMemLoc) == 0) {
        res = 0;
    } else {
        res = (void*)(i << 2);
    }
    pInstance->m_Model = (tParticleModelRecord*)res;
    pInstance->m_pInstanceAudio = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveMaterial);
#ifdef SKIP_ASM
/**
 * Fixes up a material record's data pointer.
 *
 * Sets @p pMaterial's m_TextureFlip to 0 when it holds -1, otherwise to the address of the flip table that follows the
 * record.
 *
 * @param pMaterial  the material record (On Tour pMaterial)
 * @return 1
 */
// PORT: stores a pointer in an int field (int typing needed to match); not 64-bit safe.
int cChunkResolve::resolveMaterial(tMaterialRecord* pMaterial)
{
    if (pMaterial->m_TextureFlip.m_iTextureFlip == kNullOffset) {
        pMaterial->m_TextureFlip.m_iTextureFlip = 0;
    } else {
        pMaterial->m_TextureFlip.m_iTextureFlip = (int)(pMaterial + 1); // the flip table follows the record
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
int cChunkResolve::resolveMaterialBlocks(int n, int* data)
{
    int blockIndex;
    for (blockIndex = 0; blockIndex < n; blockIndex++) {
        int j;
        // ORIGINAL-PTR: the blocks are variable-length records (a count, then that many ids); the next block
        // follows the last id
        unsigned int* hp = (unsigned int*)(data + 1);
        for (j = 0; j < *data; j++, hp++) {
            unsigned int materialId = *hp;
            cWorldCacheTable* sec = mp_World->m_pWorldCache->m_pSectionCacheTable[materialId & 0xFF];
            void* res;
            unsigned int i;
            // m_pCacheTable[0]: kBxStreamData_Material
            if (sec == 0 || (i = sec->m_pCacheTable[0].m_pDataTag[materialId >> 8].m_bfuiMemLoc) == 0) {
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
    virtual int resolveChannel(void* p, int base);
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
int cChunkResolve::resolveAnimation(tAnimRecord* pAnimation, int base)
{
    int i = 0;
    int* channelId = (int*)(*(int*)&pAnimation->m_pChannels += base);
    for (; i < pAnimation->m_NumChannels; i++) {
        *channelId += base;
        ((cChunkResolve_resolveAnimation_cObj*)this)->resolveChannel((void*)*channelId++, base);
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
int cChunkResolve::resolveChannel(tChannelRecord* pChannel, int pBase)
{
    *(int*)&pChannel->m_Segments += pBase;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveInstance);
#ifdef SKIP_ASM
static inline tModelRecord *refToPtr_A8F0(unsigned i) { return (tModelRecord*)(i << 2); }
static inline tModelRecord *conv_A8F0(unsigned i) { if (i == 0) return 0; return refToPtr_A8F0(i); }
/**
 * Fixes up a streamed instance record: replaces its model resource id (m_Model) with the model pointer.
 *
 * The id's low byte picks the section, the rest the entry of the section's model table. Also copies the
 * model's m_MaterialBlock to the instance's and clears m_pInstanceAudio.
 *
 * @param node  the instance record (On Tour tInstanceRecord*)
 * @return always 1
 */
// Resolves the node's object handle (0x80) to its object pointer and caches the object's 0xC field.
int cChunkResolve::resolveInstance(tInstanceRecord* node)
{
    unsigned id = (unsigned int)node->m_Model;
    cWorldCacheTable *section = mp_World->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    tModelRecord *resolved;
    if (!section)
        resolved = 0;
    else {
        i = section->m_pCacheTable[2].m_pDataTag[id >> 8].m_bfuiMemLoc; // [2]: kBxStreamData_Model
        tModelRecord *converted = conv_A8F0(i);
        resolved = converted;
    }
    node->m_Model = resolved;
    tMaterialBlockRecord* materialBlock = resolved->m_MaterialBlock;
    node->m_pInstanceAudio = 0;
    node->m_MaterialBlock = materialBlock;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveSpline);
#ifdef SKIP_ASM
/**
 * Links a loaded spline's segments together.
 *
 * Points m_HeaderSeg at the segment array that follows the record. For each of the m_iNumSegments segments, a
 * previous or next index (m_PrevSeg / m_NextSeg) that is not -1 becomes a pointer to the neighbouring segment (wrapping
 * around the ends), -1 becomes 0, and m_Parent is set to the spline.
 *
 * @param list  the spline record (On Tour pSpline)
 * @return 1
 */
// PORT: the link fields hold -1 or an index before the fix-up (tested as ints); not 64-bit safe.
int cChunkResolve::resolveSpline(tSplineRecord* list)
{
    int i;
    int count = list->m_iNumSegments;
    // ORIGINAL-PTR: the segment records follow the spline record in the loaded chunk (a variable-length record)
    tSplineSegmentRecord* nodes = (tSplineSegmentRecord*)(list + 1);
    int next = 1;
    list->m_HeaderSeg = nodes;
    int prev = count - 1;
    for (i = 0; i < list->m_iNumSegments; i++) {
        if ((int)nodes[i].m_PrevSeg != kNullOffset) {
            nodes[i].m_PrevSeg = &nodes[prev];
        } else {
            nodes[i].m_PrevSeg = 0;
        }
        if ((int)nodes[i].m_NextSeg != kNullOffset) {
            nodes[i].m_NextSeg = &nodes[next];
        } else {
            nodes[i].m_NextSeg = 0;
        }
        prev++;
        next++;
        if (prev >= list->m_iNumSegments) {
            prev = 0;
        }
        if (next >= list->m_iNumSegments) {
            next = 0;
        }
        nodes[i].m_Parent = list;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveCollideModel);
#ifdef SKIP_ASM
extern "C" void SphereTree_init(void* self);
extern "C" int bxSphereTree_Load(void* self, char* data, void* ctx);




/**
 * Builds a collision model's runtime data after it is loaded.
 *
 * Offsets of -1 (kNullOffset) at +0x4 (data), +0x8 (list) and +0xC (items) become 0, others become pointers into the
 * record. For type 3 (CR_SphereTree) the list gets the record's mesh count and item array; every 0x140-byte item is
 * initialized with SphereTree_init and then loaded from consecutive data with bxSphereTree_Load. For type 1
 * (CR_Polygon) each mesh's four offsets are made absolute and stored in the item array.
 *
 * @param ctx  resource id of the model, passed on to bxSphereTree_Load (On Tour RID)
 * @param pCollideModel  the collision model record (On Tour pCollideModel)
 * @return 1
 */
int cChunkResolve::resolveCollideModel(void* ctx, tCollideModelRecord* pCollideModel)
{
    // ORIGINAL-PTR: the loaded record (a file blob) holds byte offsets from itself in m_pColData / m_pPolyModelList /
    // m_pSphTree, and each tPolyModelMesh byte offsets from itself in its four arrays; they are made absolute here
    if ((int)pCollideModel->m_pColData == kNullOffset) {
        pCollideModel->m_pColData = 0;
    } else {
        pCollideModel->m_pColData = (char*)pCollideModel + (int)pCollideModel->m_pColData;
    }
    if ((int)((tSphTreeList*)pCollideModel->m_pPolyModelList) == kNullOffset) {
        (*(tSphTreeList**)&pCollideModel->m_pPolyModelList) = 0;
    } else {
        (*(tSphTreeList**)&pCollideModel->m_pPolyModelList) = (tSphTreeList*)((char*)pCollideModel + (int)((tSphTreeList*)pCollideModel->m_pPolyModelList));
    }
    if ((int)((char*)pCollideModel->m_pSphTree) == kNullOffset) {
        (*(char**)&pCollideModel->m_pSphTree) = 0;
    } else {
        (*(char**)&pCollideModel->m_pSphTree) = (char*)pCollideModel + (int)((char*)pCollideModel->m_pSphTree);
    }
    if (pCollideModel->m_usMethod == CR_SphereTree) {
        tSphTreeList* modelList = ((tSphTreeList*)pCollideModel->m_pPolyModelList);
        modelList->NumMesh = 0;
        (*(char**)&modelList->MeshTree) = 0;
        modelList->NumMesh = pCollideModel->m_usNumMesh;
        (*(char**)&modelList->MeshTree) = ((char*)pCollideModel->m_pSphTree);
        for (int i = 0; i < modelList->NumMesh; i++) {
            SphereTree_init(&modelList->MeshTree[i]);
        }
        char* data = pCollideModel->m_pColData;
        for (int i = 0; i < modelList->NumMesh; i++) {
            data += bxSphereTree_Load(&modelList->MeshTree[i], data, ctx);
        }
    } else if (pCollideModel->m_usMethod == CR_Polygon) {
        tSphTreeList* modelList = ((tSphTreeList*)pCollideModel->m_pPolyModelList);
        modelList->NumMesh = 0;
        (*(char**)&modelList->MeshTree) = 0;
        modelList->NumMesh = pCollideModel->m_usNumMesh;
        (*(char**)&modelList->MeshTree) = ((char*)pCollideModel->m_pSphTree);
        tPolyModelMesh* node = (tPolyModelMesh*)pCollideModel->m_pColData;
        for (int i = 0; i < modelList->NumMesh; i++) {
            ((tPolyModelMesh**)modelList->MeshTree)[i] = node;
            (*(char**)&node->Face) = (char*)node + (int)((char*)node->Face);
            (*(char**)&node->Boxes) = (char*)node + (int)((char*)node->Boxes);
            (*(char**)&node->Vertex) = (char*)node + (int)((char*)node->Vertex);
            (*(char**)&node->Normal) = (char*)node + (int)((char*)node->Normal);
            node = (tPolyModelMesh*)&node->Normal[node->NumFaces];
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveHalo);
#ifdef SKIP_ASM
// PORT: getHaloRadius__FPv is declared (void*) but takes a flags int and returns a float.
float getHaloRadius_f(int flags) __asm__("getHaloRadius__FPv");


/**
 * Sets up a light halo's bounds after it is loaded.
 *
 * Gets the radius for the halo type (flags & 0x70) from getHaloRadius, clears +0x40 and +0x44 and sets the box
 * +0x28..+0x3C to the position at +0x1C plus or minus that radius.
 *
 * @param obj  the halo record (On Tour pLightHalo)
 * @return 1
 */
int cChunkResolve::resolveHalo(tHaloRecord* obj)
{
    float haloRadius = getHaloRadius_f(((int)obj->m_iFlags) & 0x70);
    (*(int*)&obj->m_fOcclude[1]) = 0;
    (*(int*)&obj->m_fOcclude[0]) = 0;
    obj->m_fvMin[0] = obj->m_fvWorldPos[0] - haloRadius;
    obj->m_fvMin[1] = obj->m_fvWorldPos[1] - haloRadius;
    obj->m_fvMin[2] = obj->m_fvWorldPos[2] - haloRadius;
    obj->m_fvMax[0] = obj->m_fvWorldPos[0] + haloRadius;
    obj->m_fvMax[1] = obj->m_fvWorldPos[1] + haloRadius;
    obj->m_fvMax[2] = obj->m_fvWorldPos[2] + haloRadius;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_resolveLight);
#ifdef SKIP_ASM
/**
 * Accepts a loaded real-time light, limiting its range.
 *
 * Returns false for a light with flag 0x100 or a type (+0x10) other than 1 or 2; otherwise caps the range at +0x1C to
 * 5000 and returns true.
 *
 * @param obj  the light record (On Tour pRTLight)
 * @return true when the light is used, else false
 */
bool cChunkResolve::resolveLight(tLightRecord* obj)
{
    if (obj->m_iFlags & 0x100) {
        return false;
    }
    int lightType = obj->m_eLightType;
    if (lightType < 3) {
        if (lightType > 0) {
            if (obj->m_fMaxDistance > 5000.0f) {
                obj->m_fMaxDistance = 5000.0f;
            }
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_unresolveLight);
#ifdef SKIP_ASM
/**
 * Tells whether a real-time light being unloaded was one the resolver accepted.
 *
 * @param pRTLight  the light record (On Tour pRTLight)
 * @return true for a spot or point light (type 1 or 2) without flag 0x100, else false
 */
bool cChunkResolve::unresolveLight(tLightRecord* pRTLight)
{
    if (pRTLight->m_iFlags & 0x100) {
        return false;
    }
    switch (pRTLight->m_eLightType) {
    case eNEWLight_Spot:
    case eNEWLight_Point:
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("world/worldview", cChunkResolve_calcMaxLightDistance);
#ifdef SKIP_ASM
extern "C" float expf(float);
extern "C" float logf(float);

/**
 * Returns how far a light reaches before its strength falls to @p divisor.
 *
 * The strength is the light's m_fIntensity times m_fRGBIntensity. By the falloff type m_cDecayRate: 1 gives
 * strength / @p divisor x 100, 2
 * the square root of that ratio x 100, 3 the cube root (logf log, expf exp of a third) x 100; any
 * other type gives 1e7. No C caller in the source.
 *
 * @param light  the tLightRecord
 * @param divisor  the threshold strength
 * @return the distance
 */
float cChunkResolve::calcMaxLightDistance(tLightRecord* light, float divisor)
{
    float prod = light->m_fIntensity * light->m_fRGBIntensity;
    float ratio = prod / divisor;
    switch (light->m_cDecayRate) {
    case DECAY_LINEAR:
        return ratio * 100.0f;
    case DECAY_EXPONENTIAL2: {
        float root;
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(root) : "f"(ratio));
        return root * 100.0f;
    }
    case DECAY_EXPONENTIAL3:
    {
        float third = 0.33333334f;
        float logRatio = logf(ratio);
        return expf(logRatio * third) * 100.0f;
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
    cPS2ChunkResolve* chunkResolve = (cPS2ChunkResolve*)cMemMan_alloc(20, str_cPS2ChunkResolve, 0, 0);
    cChunkResolve_cChunkResolve(chunkResolve);
    chunkResolve->mVtbl = cPS2ChunkResolve_vtbl;
    return ((void*)chunkResolve);
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
extern "C" void cPS2ChunkResolve__cPS2ChunkResolve(cPS2ChunkResolve* self, int flags)
{
    self->mVtbl = cPS2ChunkResolve_vtbl;
    self->purge();
    self->mVtbl = cChunkResolve_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cPS2ChunkResolve_init(cPS2ChunkResolve* self, int pNash)
{
    cWorld* w = cWorld_m_pTheWorld;
    self->mp_Nash = (Nash*)pNash;
    *(char****)self = (char***)w;
    self->m_iNumImages = w->m_pWorldCache->mp_DataHeader->m_iTable[9];
}
#endif

//100%
INCLUDE_ASM("world/worldview", cPS2ChunkResolve_purge__FPv);
#ifdef SKIP_ASM
/**
 * Releases the PS2 chunk resolver's platform data; does nothing in this build.
 */
void cPS2ChunkResolve::purge()
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
extern "C" void* getOcclusionMan();
extern "C" void cOcclusionMan_addVisCurtain(void* self, void* obj);
// PORT: the unit declares this later returning void*; bind this char* view by symbol.
char* cAudioInterface_getGame_AE68() __asm__("cAudioInterface_getGame");
extern "C" void cWorldSound_InitWorldAudioFile(void* self, void* obj, unsigned int arg);
extern char* SSXApp_AE68 __asm__("SSXApp");
extern "C" bool cAI_swaploadInjectedPathFile(void* self, int a, unsigned int b, void* obj);
void cMemMan_free(void* p);
extern "C" void cWorldPainterMan_load1(void* self, int a, void* obj);
extern "C" void cWScriptCache_load1(void* self, int a, void* obj);
extern "C" void* getGame();
extern "C" void cGame_loadWScriptManSection(void* self, int a);
void gCamTriggerMan_resolve(void* a, int b);
extern "C" void cSSXScriptEngine_AttachAnchorDataTable(void* self, int a, void* obj);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cBEMissionInterface_addStreamData_AE68(void* self, int a, void* p, int v) __asm__("cBEMissionInterface_addStreamData__FPv");
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_AttachAudioBank(void* self, unsigned int arg, void* obj);
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
void* cPS2ChunkResolve::resolve(int msg, unsigned int RID, char* obj)
{
    switch (msg) {
    case kBxStreamData_Patch:
        *(unsigned short*)(obj + 0x1A2) += m_iNumImages;
        insert_AE68(this, 1, obj, 344);
        break;
    case kBxStreamData_Image: {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        (void*)w->AddTexture((const SHAPE*)obj, str_strm_tex, 0, 0, RID >> 8);
        break;
    }
    case kBxStreamData_LightPage: {
        cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
        (void*)w->AddTexture((const SHAPE*)obj, str_strm_lpg, 0, 0, (RID >> 8) + m_iNumImages);
        break;
    }
    case kBxStreamData_Instance:
        CALL1_AE68(this, 104, obj);
        insert_AE68(this, 0, obj, 0x60);
        break;
    case kBxStreamData_Model:
        CALL1_AE68(this, 40, obj);
        break;
    case kBxStreamData_Material:
        CALL1_AE68(this, 72, obj);
        break;
    case kBxStreamData_RealTimeLight:
        if (CALL1_AE68(this, 168, obj)) {
            *(int*)(obj + 8) = 6;
            insert_AE68(this, 2, obj, 68);
        }
        break;
    case kBxStreamData_HaloLight:
        CALL1_AE68(this, 160, obj);
        *(int*)(obj + 8) = 8;
        insert_AE68(this, 2, obj, 40);
        break;
    case kBxStreamData_Spline: {
        CALL1_AE68(this, 144, obj);
        tSplineSegmentRecord* child = (tSplineSegmentRecord*)(*(char**)(obj + 0x24));
        for (int i = 0; i < *(int*)(obj + 0x20); i++) {
            child->m_iIntersect[2] = 1;
            insert_AE68(this, 2, ((char*)child), 108);
            child = (tSplineSegmentRecord*)child->m_NextSeg;
        }
        break;
    }
    case kBxStreamData_ParticleInstance:
        CALL1_AE68(this, 136, obj);
        *(int*)(obj + 8) = 7;
        insert_AE68(this, 2, obj, 104);
        break;
    case kBxStreamData_ParticleModel:
        CALL1_AE68(this, 0x70, obj);
        break;
    case kBxStreamData_CollideModel:
        ((tMsg2_AE68)VT_AE68(this, 152)->fn)((char*)this + VT_AE68(this, 0x98)->delta, RID, obj);
        break;
    case kBxStreamData_IcePick:
        cWorldSound_InitWorldAudioFile(*(void**)(cAudioInterface_getGame_AE68() + 0x4C), obj, RID);
        break;
    case kBxStreamData_CameraFile:
        gCamTriggerMan_resolve((void*)RID, (int)obj);
        break;
    case kBxStreamData_WorldPainter:
        if (cWorld_m_pTheWorld) {
            cWorldPainterMan_load1((char**)cWorld_m_pTheWorld->m_pWorldPainterMan, RID & 0xFF, obj);
        }
        break;
    case kBxStreamData_ColdSpell:
        if (cWorld_m_pTheWorld) {
            int sectionIdx = RID & 0xFF;
            cWScriptCache_load1((char**)cWorld_m_pTheWorld->m_pWScriptCache, sectionIdx, obj);
            cGame_loadWScriptManSection(getGame(), sectionIdx);
        }
        break;
    case kBxStreamData_NISTable:
        cSSXScriptEngine_AttachAnchorDataTable(ScriptEngine, RID & 0xFF, obj);
        break;
    case kBxStreamData_AudioBank:
        cSSXAudio_AttachAudioBank(cAudioInterface_bxAudio(), RID, obj);
        break;
    case kBxStreamData_Mission:
        cBEMissionInterface_addStreamData_AE68(cBE_getInterface_Fv(cBE_getBE(), 10), RID & 0xFF, obj + 4, *(int*)obj);
        break;
    case kBxStreamData_Radar:
        func_002105D0(obj);
        break;
    case kBxStreamData_VisCurtain:
        cOcclusionMan_addVisCurtain(getOcclusionMan(), obj);
        break;
    case kBxStreamData_AIPath:
        if (!cAI_swaploadInjectedPathFile(*(void**)(*(char**)(SSXApp_AE68 + 0x84) + 0xC), RID & 0xFF, RID >> 8, obj)) {
            if (obj) {
                cMemMan_free(obj);
            }
        }
        break;
    case kBxStreamData_Avalanche:
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void operator_delete(int*);
extern "C" void cAI_removeInjectedFile(void* p, unsigned char id);
// PORT: cBEMissionInterface_remStreamData__FPv is called with a second (byte) argument here.
void cBEMissionInterface_remStreamData_2(void* iface, unsigned char id) __asm__("cBEMissionInterface_remStreamData__FPv");
extern "C" void func_0016CF18(unsigned int param);
extern "C" void func_00210608(void* data);
extern "C" void cOcclusionMan_removeVisCurtain(void* p, void* data);
extern "C" void cSSXScriptEngine_DetachAnchorDataTable(void* p, unsigned char id);
extern "C" void cSSXAudio_UnAttachAudioBank(void* p, unsigned int param, void* data);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" cGame* cAudioInterface_getGame();
extern "C" void cWorldSound_PurgeWorldAudioFile(void* p, void* data, unsigned int param);
extern "C" void cWorldPainterSection_unload(void* entry);
extern "C" void cAvalanche_reset();
extern "C" void nash_Cell_remnode(void* node, int idx, void* item, const void* target, const void* cur);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void* getOcclusionMan();

struct sWVBox_B498 {
    t4Vector min;
    t4Vector max;
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
 * and free them; type 13 purges a world audio file (cWorldSound_PurgeWorldAudioFile), 15 unloads a world-painter section (cWorldPainterSection_unload),
 * 18 detaches a script anchor table, 20 unattaches an audio bank, 19 removes mission stream data through BE interface
 * 10, 11 removes an occlusion curtain (cOcclusionMan_removeVisCurtain), 14 drops an AI file (cAI_removeInjectedFile) and 22 resets the avalanche;
 * types 17 and 21 go to func_0016CF18 and func_00210608. Other types need nothing.
 *
 * @param msg  stream data type of the chunk (On Tour eBxStreamDataType type)
 * @param resourceId  resource id of the chunk (On Tour RID)
 * @param data  the chunk's data (On Tour pData)
 * @return @p data, or 0 for type 14
 */
void* cPS2ChunkResolve::unresolve(unsigned int msg, unsigned int resourceId, char* data)
{
    switch (msg) {
    case kBxStreamData_Material:
        break;
    case kBxStreamData_Patch: {
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 344);
        update_B498((char*)mp_Nash, 1, data, &box);
        break;
    }
    case kBxStreamData_Image: {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        ps2GraphicsMan->RemoveTexture(resourceId >> 8);
        break;
    }
    case kBxStreamData_LightPage: {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        ps2GraphicsMan->RemoveTexture((resourceId >> 8) + m_iNumImages);
        break;
    }
    case kBxStreamData_Instance: {
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 0x60);
        update_B498((char*)mp_Nash, 0, data, &box);
        break;
    }
    case kBxStreamData_Spline: {
        int i = 0;
        char* node = *(char**)(data + 0x24);
        if (i < *(int*)(data + 0x20)) {
            int one = 1;
            do {
                *(int*)(node + 0x8) = one;
                sWVBox_B498 box;
                WV_MAKEBOX_B498(box, node, 108);
                update_B498((char*)mp_Nash, 2, node, &box);
                node = *(char**)(node + 0x64);
                i++;
            } while (i < *(int*)(data + 0x20));
        }
        break;
    }
    case kBxStreamData_RealTimeLight: {
        sVEntry<void>* vt = (sVEntry<void>*)mVtbl;
        if (((int (*)(void*, void*))vt[22].fn)(vthis(this, vt[22]), data)) {
            *(int*)(data + 0x8) = 6;
            sWVBox_B498 box;
            WV_MAKEBOX_B498(box, data, 68);
            update_B498((char*)mp_Nash, 2, data, &box);
        }
        break;
    }
    case kBxStreamData_HaloLight: {
        *(int*)(data + 0x8) = 8;
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 40);
        update_B498((char*)mp_Nash, 2, data, &box);
        break;
    }
    case kBxStreamData_ParticleInstance: {
        *(int*)(data + 0x8) = 7;
        sWVBox_B498 box;
        WV_MAKEBOX_B498(box, data, 104);
        update_B498((char*)mp_Nash, 2, data, &box);
        break;
    }
    case kBxStreamData_IcePick:
        cWorldSound_PurgeWorldAudioFile(cAudioInterface_getGame()->WorldSound, data, resourceId);
        break;
    case kBxStreamData_CameraFile:
        func_0016CF18(resourceId);
        break;
    case kBxStreamData_WorldPainter:
        if (cWorld_m_pTheWorld != 0) {
            cWorldPainterSection_unload(((char**)(char**)cWorld_m_pTheWorld->m_pWorldPainterMan)[1] + (unsigned char)resourceId * 12);
        }
        break;
    case kBxStreamData_NISTable:
        cSSXScriptEngine_DetachAnchorDataTable(ScriptEngine, resourceId);
        break;
    case kBxStreamData_AudioBank:
        cSSXAudio_UnAttachAudioBank(cAudioInterface_bxAudio(), resourceId, data);
        break;
    case kBxStreamData_Mission:
        cBEMissionInterface_remStreamData_2(cBE_getInterface_Fv(cBE_getBE(), 10), resourceId);
        break;
    case kBxStreamData_Radar:
        func_00210608(data);
        break;
    case kBxStreamData_VisCurtain:
        cOcclusionMan_removeVisCurtain(getOcclusionMan(), data);
        break;
    case kBxStreamData_AIPath:
        data = 0;
        cAI_removeInjectedFile(SSXApp->Game->AI, resourceId);
        break;
    case kBxStreamData_Avalanche:
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
    virtual int resolveModelHeirarchy(void* a1);
};

extern "C" int func_003ABF20(cPS2ChunkResolve* self, cList* a1);

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
 * Turns its m_MeshHeirarchy and m_MaterialBlock offsets into pointers, replaces each resource id in the material
 * block's list (GetMaterialRecordList) with its pointer, then calls virtual slot 6 (probably resolveModelHeirarchy)
 * and func_003ABF20().
 *
 * @param pModel  the model record (On Tour tModelRecord*)
 * @return always 1
 */
// PORT: stores pointers in int fields (offset -> pointer fixup); not 64-bit safe.
int cPS2ChunkResolve::resolveModel(tModelRecord* pModel)
{
    int i = 0;
    *(int*)&pModel->m_MeshHeirarchy += (int)pModel;
    *(int*)&pModel->m_MaterialBlock += (int)pModel;
    tMaterialBlockRecord* block = pModel->m_MaterialBlock;
    tMaterialRecord** entry = block->GetMaterialRecordList();
    for (; i < block->m_iNumMaterials; i++, entry++) {
        *entry = (tMaterialRecord*)cPS2ChunkResolve_resolveModel_lookup(((cPS2ChunkResolve_resolveModel_cObj*)this),
                                                                         (unsigned int)*entry);
    }
    ((cPS2ChunkResolve_resolveModel_cObj*)this)->resolveModelHeirarchy(pModel);
    func_003ABF20(this, (cList*)pModel);
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
extern "C" int func_003ABF20(cPS2ChunkResolve* self, cList* list)
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



// PORT: the unit declares func_003ABF50 as returning void (func_003ABF20 calls it that way),
// but the body returns 1; bind the int-returning body to the symbol with an asm label.
int func_003ABF50_impl(void* self, tModelRecord* list) __asm__("func_003ABF50");

/**
 * Fixes up the DMA packets of each mesh of a model: func_003ABFC0 on every non-null mesh in the model's list (count
 * +0x4, 16-byte entries at +0x8, mesh at +0x4).
 *
 * @param list  the tModelRecord
 * @return 1
 */
int func_003ABF50_impl(void* self, tModelRecord* list)
{
    int n = list->m_iNumMeshes;
    tMeshHeirarchyRecord* entry = list->m_MeshHeirarchy;
    int i;
    for (i = 0; i < n; i++, entry++) {
        if (((void*)entry->m_Mesh) != 0) {
            func_003ABFC0(self, ((void*)entry->m_Mesh), list);
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
 * @param vitem  the mesh
 * @param list  the tModelRecord
 * @return 1
 */
int func_003ABFC0_impl(void* self, void* vitem, void* list)
{
    tMeshRecord* item = (tMeshRecord*)vitem;
    void** childPtr = (void**)item->m_pContexts;
    int i;
    for (i = 0; i < item->m_iNumContexts; i++, childPtr++) {
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
extern "C" int func_003AC048_impl(void* vself, char* obj, char* ctx, int index)
{
    tModelRecord* self = (tModelRecord*)vself;
    int done = 0;
    int first = 1;
    int split = 0;
    if (*(unsigned int*)obj & 0x60000) split = 1;
    char* tagPtr = (char*)(*(int*)(obj + 4) + *(int*)(ctx + 0x24));
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case kDMATag_REF: {
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
                if ((((int)((*(long*)next << 4) >> 32)) & 7) == kDMATag_RET) {
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
        case kDMATag_RET:
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

extern "C" int func_003AC358(cPS2ChunkResolve* self, tInstanceRecord* a1);

static inline cPS2ChunkResolve_resolveInstance_sRes* cPS2ChunkResolve_resolveInstance_lookup(cPS2ChunkResolve* self, unsigned int id)
{
    cPS2ChunkResolve_resolveInstance_sTab* t = (*((cPS2ChunkResolve_resolveInstance_sLib**)self->mp_World))->tables[id & 0xFF];
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
int cPS2ChunkResolve::resolveInstance(tInstanceRecord* pInstance)
{
    cPS2ChunkResolve_resolveInstance_sRes* resolved = cPS2ChunkResolve_resolveInstance_lookup(this,
            (unsigned int)pInstance->m_Model);
    pInstance->m_Model = (tModelRecord*)resolved;
    pInstance->m_MaterialBlock = (tMaterialBlockRecord*)resolved->fC;
    pInstance->m_pInstanceAudio = 0;
    pInstance->m_pCollideRecord = (tCollideRecord*)(m_pDummyCollideRecord);
    int x = m_pDummyCollideRecord->InitFlags & 0xFFFF0000;
    pInstance->m_iFlags = x | (x >> 16) | 2;
    func_003AC358((cPS2ChunkResolve*)this, pInstance);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AC358);
#ifdef SKIP_ASM

extern "C" void* func_003AC508(cPS2ChunkResolve* self, tInstanceRecord* a1, tModelRecord* list, void* cur);

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
extern "C" int func_003AC358(cPS2ChunkResolve* self, tInstanceRecord* instance)
{
    int i;
    tModelRecord* list;
    void* cur;
    *(int*)((char*)instance + 0x98) += (int)instance;
    list = instance->m_Model;
    cur = *(void**)((char*)instance + 0x98);
    int n = list->m_iNumMeshes;
    tMeshHeirarchyRecord* entry = list->m_MeshHeirarchy;
    for (i = 0; i < n; i++, entry++) {
        tMeshRecord* object = entry->m_Mesh;
        if (object != 0) {
            int j;
            for (j = 0; j < object->m_iNumContexts; j++) {
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
 * @param ignored0  unused
 * @param ignored1  unused
 * @param ctx  the record whose +0x24 data the offsets are relative to
 * @param tagPtr  the first DMA tag
 * @return the address after the ret tag
 */
extern "C" char* func_003AC438(int ignored0, int ignored1, char* ctx, char* tagPtr)
{
    int done = 0;
    int first = 1;
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case kDMATag_REF: {
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
        case kDMATag_RET:
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
extern "C" void* func_003AC508(cPS2ChunkResolve* self, tInstanceRecord* instance, tModelRecord* list, void* cur)
{
    tModelRecord* ctx = (tModelRecord*)list;
    char* tagPtr = (char*)cur;
    int done = 0;
    int first = 1;
    long tag;
    do {
        tag = *(long*)tagPtr;
        switch (((int)((tag << 4) >> 32)) & 7) {
        case kDMATag_REF: {
            int flag = 0;
            if (*(int*)(tagPtr + 4) & 0x80000000) {
                *(int*)(tagPtr + 4) &= 0x7FFFFFFF;
                flag = 1;
            }
            *(int*)(tagPtr + 4) = (int)ctx->unk_024 + *(int*)(tagPtr + 4);
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
                if ((((int)((*(long*)next << 4) >> 32)) & 7) == kDMATag_RET) {
                    tag = *(long*)tagPtr;
                    int addr = (int)ctx->unk_024 + *(int*)(tagPtr + 4);
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
                    *(int*)(tagPtr + 4) = (int)ctx->unk_024 + *(int*)(tagPtr + 4);
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
                    *ap = (int)ctx->unk_024 + *ap;
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
                    *ap = (int)ctx->unk_024 + *ap;
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
        case kDMATag_RET:
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
extern "C" void* cWScriptCache_cWScriptCache(cWScriptCache* self, int world)
{
    self->mpWorld = (cWorld*)world;
    self->mpCache = 0;
    cWScriptCache_init(self, 1);
    return ((void*)self);
}
#endif

