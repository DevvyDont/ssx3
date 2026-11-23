#include "common.h"
#include "be/bedbprofile.h"
#include "ui/uiengine.h"
#include "world/worldcache.h"
#include "ui/uistate.h"
#include "ai/rider.h"
#include "worldpainter/worldpainterquery.h"
#include "ui/uithing.h"
#include "vtable.h"
#include "fe/fereal.h"
#include "bx/appman.h"
#include "visualfx/boltonfx.h"
#include "visualfx/ps2lensfx.h"
#include "visualfx/renderstateman.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "object/being.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2LensFxMan_cPS2LensFxMan(void* self);
extern const char str_LensFX[];

//99.23%
INCLUDE_ASM("visualfx/lensfx", cLensFxMan_construct__Fv);
#ifdef SKIP_ASM
/**
 * Creates the lens effects manager ("LensFX", 0x6820 bytes) through the PS2 constructor cPS2LensFxMan_cPS2LensFxMan.
 *
 * @return the new manager
 */
void* cLensFxMan_construct()
{
    void* mem = cMemMan_alloc(26656, str_LensFX, 0, 0);
    return cPS2LensFxMan_cPS2LensFxMan(mem);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cPS2LensFxMan_cPS2LensFxMan);
#ifdef SKIP_ASM
extern "C" void cLensFxMan_cLensFxMan(void* self);
extern void* cPS2LensFxMan_vtbl[];

// PORT: asm label because the unit's forward declaration above has C++ linkage;
// fix that declaration to extern "C" and this can be a plain extern "C" definition.
void* cPS2LensFxMan_cPS2LensFxMan_impl(void* self) __asm__("cPS2LensFxMan_cPS2LensFxMan");
/**
 * Constructs the PS2 lens flare effects manager.
 *
 * Runs cLensFxMan_cLensFxMan and stores the PS2 vtable cPS2LensFxMan_vtbl at +0x649C.
 *
 * @return this
 */
void* cPS2LensFxMan_cPS2LensFxMan_impl(void* vself)
{
    cPS2LensFxMan* self = (cPS2LensFxMan*)vself;
    cLensFxMan_cLensFxMan(self);
    self->mVtbl = (void**)((int)(void*)cPS2LensFxMan_vtbl);
    return ((void*)self);
}
#endif

extern void* cPS2LensFxMan_vtbl[];
extern "C" void* cLensFxMan__cLensFxMan(void*);

//100%
INCLUDE_ASM("visualfx/lensfx", cPS2LensFxMan__cPS2LensFxMan__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the PS2 lens flare effects manager.
 *
 * Stores cPS2LensFxMan_vtbl at +0x649C again and tail-calls cLensFxMan__cLensFxMan, which also frees the object when the
 * in-charge flag asks for it.
 *
 * @return cLensFxMan__cLensFxMan's result
 */
void* cPS2LensFxMan__cPS2LensFxMan(void* vself)
{
    cPS2LensFxMan* self = (cPS2LensFxMan*)vself;
    self->mVtbl = (void**)((int)(void*)cPS2LensFxMan_vtbl);
    return cLensFxMan__cLensFxMan(self);
}
#endif

/**
 * Measures how much of a lens-flare halo is visible by reading the depth buffer back from the GS.
 *
 * Converts the halo's screen rectangle (floats at +0x0..+0x24 of the second argument) to ints, gets a depth threshold
 * from fptoui, builds a GS-to-host transfer of that area of the Z buffer in the object's packet buffer
 * (+0x64A0), flushes the cache, reads the 24-bit samples back into +0x6510 over the GIF (sceGsSyncPath, func_00371940),
 * then counts the samples inside the rectangle and those at or below the threshold (unoccluded). When the count equals
 * the halo's area (+0x8 x +0xC) the visible fraction is stored through the third argument; otherwise (a clipped
 * rectangle) it stores max((visible - area / 2) / (area / 2), 0). Virtual slot 4. Asm only, no C body yet: the second
 * argument is On Tour's cHalo& halo, the third its float& value.
 *
 * @return always 1
 */
INCLUDE_ASM("visualfx/lensfx", cPS2LensFxMan_sampleZBuffer);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EC9D0__FPv);
#ifdef SKIP_ASM
void* func_002EC9D0(void* self)
{
    *(int*)self = -1;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cBoltOnFXInterface_cBoltOnFXInterface);
#ifdef SKIP_ASM
extern void* cBoltOnFXInterface_vtbl[];
struct sLensFx;
extern "C" void func_002ECAF0(cBoltOnFXInterface* self);

/**
 * Constructs a rider's bolt-on effect set ("cBoltOnFXInterface"): the attached particle emitters its equipment carries.
 *
 * Installs the vtable cBoltOnFXInterface_vtbl at +0xC4, marks the set dirty (+0x0 = 1), stores the rider (+0x4) and the
 * attached-emitter owner (+0xC), clears the character pointer (+0x8) and empties the 15 entries (+0x10, 0xC bytes each:
 * emitter index, item, kind) through func_002EC9D0 and func_002ECAF0. cRider_cRider builds it on 200 bytes with the
 * rider's mAttachedEmitterFX and stores it as mBoltOnFX.
 *
 * @param rider  the rider
 * @param emitterOwner  the attached-emitter owner the emitters are created in
 * @return @p self
 */
extern "C" void* cBoltOnFXInterface_cBoltOnFXInterface(cBoltOnFXInterface* self, int rider, int emitterOwner)
{
    self->mVtbl = cBoltOnFXInterface_vtbl;
    self->mDirty = 1;
    self->mpRider = (cRider*)rider;
    self->mAttachedEmitterFX = (cAttachedEmitterFX*)emitterOwner;
    self->mpCharId = 0;
    char* entry = (char*)&self->mEntries[0];
    int i;
    for (i = 14; i != -1; i--, entry += 0xC) {
        func_002EC9D0(entry);
    }
    func_002ECAF0(self);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cBoltOnFXInterface_cBoltOnFXInterface1);
#ifdef SKIP_ASM
extern void* cBoltOnFXInterface_vtbl[];
struct sLensFx;
extern "C" void func_002ECAF0(cBoltOnFXInterface* self);

/**
 * Constructs a front-end character's bolt-on effect set ("cBoltOnFXInterface"), keyed by a character id instead of a
 * rider.
 *
 * Same as cBoltOnFXInterface_cBoltOnFXInterface except that it stores a pointer to the character id (+0x8) and leaves the rider (+0x4) 0.
 * cFERider_init builds it on 200 bytes.
 *
 * @param charIdPtr  pointer to the character id
 * @param emitterOwner  the attached-emitter owner the emitters are created in
 * @return @p self
 */
extern "C" void* cBoltOnFXInterface_cBoltOnFXInterface1(cBoltOnFXInterface* self, int charIdPtr, int emitterOwner)
{
    self->mVtbl = cBoltOnFXInterface_vtbl;
    self->mDirty = 1;
    self->mpCharId = (int*)charIdPtr;
    self->mAttachedEmitterFX = (cAttachedEmitterFX*)emitterOwner;
    self->mpRider = 0;
    char* entry = (char*)&self->mEntries[0];
    int i;
    for (i = 14; i != -1; i--, entry += 0xC) {
        func_002EC9D0(entry);
    }
    func_002ECAF0(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECAF0);
#ifdef SKIP_ASM
struct sLensEntry {
    int id;
    int a;
    int b;
};

struct sLensFx {
    char pad00[0x10];
    sLensEntry entries[15];
};

/**
 * Marks all 15 entries of a bolt-on effect set as free (emitter index -1).
 *
 * The two constructors cBoltOnFXInterface_cBoltOnFXInterface and cBoltOnFXInterface_cBoltOnFXInterface1 call it.
 */
extern "C" void func_002ECAF0(cBoltOnFXInterface* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        ((sLensEntry*)&self->mEntries[0])[i].id = -1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cBoltOnFXInterface__cBoltOnFXInterface);
#ifdef SKIP_ASM
extern void* cBoltOnFXInterface_vtbl[];
extern "C" void func_002F2218(int a, int id);
void operator_delete(int* ptr);

/**
 * Destroys a bolt-on effect set: releases every attached emitter it still holds, then frees the object when asked.
 *
 * Re-installs the vtable cBoltOnFXInterface_vtbl, releases each used entry's emitter from the owner (+0xC) with func_002F2218 and
 * marks it free, then frees the object with operator_delete when bit 0 of @p flags is set. It is the destructor slot of
 * cBoltOnFXInterface_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cBoltOnFXInterface__cBoltOnFXInterface(sLensFx* self, int flags)
{
    *(void***)((char*)self + 0xC4) = cBoltOnFXInterface_vtbl;
    int i;
    for (i = 0; i < 15; i++) {
        if (self->entries[i].id != -1) {
            func_002F2218(*(int*)((char*)self + 0xC), self->entries[i].id);
            self->entries[i].id = -1;
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECBC0);
#ifdef SKIP_ASM
struct func_002ECBC0_sEntry {
    int id;
    int b;
    int c;
};

struct func_002ECBC0_sMgr {
    char pad[0x10];
    func_002ECBC0_sEntry entries[15];
};

extern "C" int func_002ECC28(cBoltOnFXInterface* self, int a1, int a2);

/**
 * Returns the entry of a bolt-on effect set that holds an emitter of @p kind on @p itemId, creating one when there is
 * none.
 *
 * Searches the 15 entries for a used one with item +0x4 == @p itemId and kind +0x8 == @p kind; otherwise func_002ECC28
 * creates it. func_002ECCB8 calls it.
 *
 * @param kind  the particle effect kind
 * @param itemId  the item (attach point) id
 * @return the entry index
 */
extern "C" int func_002ECBC0(func_002ECBC0_sMgr* self, int kind, int itemId)
{
    int i;
    for (i = 0; i < 15; i++) {
        if (self->entries[i].id != -1 && self->entries[i].b == itemId && self->entries[i].c == kind) {
            return i;
        }
    }
    return func_002ECC28((cBoltOnFXInterface*)self, kind, itemId);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECC28);
#ifdef SKIP_ASM
extern "C" int func_002F2130(int a, int x, int y);

/**
 * Creates an attached emitter of a given kind on an item in the first free entry of a bolt-on effect set.
 *
 * The emitter comes from func_002F2130 on the owner (+0xC); the entry stores its index, @p y as the item (+0x4) and @p
 * x as the kind (+0x8). Returns 0 when all 15 entries are in use, so the caller then marks entry 0.
 *
 * @param lensMgr  the bolt-on effect set
 * @param x  the particle effect kind
 * @param y  the item (attach point) id
 * @return the entry index, or 0 when full
 */
extern "C" int func_002ECC28(cBoltOnFXInterface* lensMgr, int x, int y)
{
    sLensFx* self = (sLensFx*)lensMgr;
    int i;
    for (i = 0; i < 15; i++) {
        if (self->entries[i].id == -1) {
            self->entries[i].id = func_002F2130(*(int*)((char*)self + 0xC), x, y);
            self->entries[i].a = y;
            self->entries[i].b = x;
            return i;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECCB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBEBoltInterface_getThis();
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void* func_0014AD50(void* self, int rider);
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" int cRider_getCheatIdentity(void* self);
extern "C" int cRider_getIdentity(void* self);
extern "C" void* cUIStateStack_getCurrentState(void* self);
extern "C" void func_002F2218(int a, int id);

struct sVEi_CCB8 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sLensItem_CCB8 {
    char pad0[4];
    short slot;                 // 0x4
    char pad6[0x11 - 6];
    signed char kind;           // 0x11
    signed char ids[2];         // 0x12
    char pad14[0x38 - 0x14];
};

struct sLensMgr_CCB8 {
    int active;                 // 0x0
    char* rider;                // 0x4
    int* charId;                // 0x8
    int fxSys;                  // 0xC
    sLensEntry entries[15];     // 0x10
};

/**
 * Rebuilds a bolt-on effect set from the character's equipment: makes sure every needed emitter exists and releases the
 * rest.
 *
 * Finds the character record: for a rider (+0x4) func_0014AD50 with its kRiderVt_getAIIndex and the id from
 * cRider_getCheatIdentity (cRider_getIdentity below 10); for a front-end character (+0x8) func_0014AD28 with the
 * current UI state's +0x44 and the id. Then walks the character's 0x38-byte bolt definitions (func_0014D988 count,
 * func_0014D998) and, for each one with a kind (+0x11, not -1) whose part is fitted (the record's table at +0x288 for
 * slot +0x4; normal characters below 10 also need flag 0x10), marks func_002ECBC0's entry for each of its two items
 * (+0x12, +0x13) as used; kinds of 50 and up use kinds k-50 and k-49. Unused entries' emitters are released with
 * func_002F2218. Clears the dirty flag (+0x0).
 *
 * @param vself  the bolt-on effect set (this)
 */
extern "C" void func_002ECCB8(cBoltOnFXInterface* vself)
{
    sLensMgr_CCB8* self = (sLensMgr_CCB8*)vself;
    int used[15];
    for (int z = 14; z >= 0; z--) {
        used[z] = 0;
    }
    void* lib = cBEBoltInterface_getThis();
    cBECharProfileDB* tbl = 0;
    int id = -1;
    cRider* rider = (cRider*)self->rider;
    if (((char*)rider) != 0) {
        tbl = (cBECharProfileDB*)((char*)func_0014AD50(lib, rider->getAIIndex()));
        id = cRider_getCheatIdentity(self->rider);
        if (id < 10) {
            id = cRider_getIdentity(self->rider);
        }
    } else if (self->charId != 0) {
        cUIState* st = (cUIState*)cUIStateStack_getCurrentState(&SSXApp->RealFE->mpUI->mStateStack);
        tbl = (cBECharProfileDB*)func_0014AD28(lib, st->mUserNum, *self->charId);
        id = *self->charId;
    }
    void* lib2 = func_0014BDB8_noarg();
    int n;
    sLensEntry* ents = self->entries;
    n = func_0014D988(lib2, id);
    sLensItem_CCB8* item = (sLensItem_CCB8*)func_0014D998(lib2, id);
    for (int i = 0; i < n; i++, item++) {
        int k = item->kind;
        if (k == -1) {
            continue;
        }
        short tableIndex = (tbl->mpBoltsMap)[item->slot];
        tBEProfileBolt* tableEntry;
        if (tableIndex >= 0) {
            tableEntry = &tbl->mBolts[tableIndex];
        } else {
            tableEntry = 0;
        }
        if (id < 10 && (tableEntry->flags & 0x10) == 0) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            int itemId = item->ids[j];
            if (itemId == -1) {
                continue;
            }
            if (k >= 50) {
                used[func_002ECBC0((func_002ECBC0_sMgr*)self, k - 50, itemId)] = 1;
                used[func_002ECBC0((func_002ECBC0_sMgr*)self, k - 49, itemId)] = 1;
            } else {
                used[func_002ECBC0((func_002ECBC0_sMgr*)self, k, itemId)] = 1;
            }
        }
    }
    for (int entryIdx = 0; entryIdx < 15; entryIdx++) {
        if (ents[entryIdx].id != -1 && used[entryIdx] == 0) {
            func_002F2218(self->fxSys, ents[entryIdx].id);
            ents[entryIdx].id = -1;
        }
    }
    self->active = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002ECF78);
#ifdef SKIP_ASM
extern "C" void func_002ECCB8(cBoltOnFXInterface* self);

/**
 * Rebuilds a bolt-on effect set (func_002ECCB8) when it is marked dirty (+0x0).
 *
 * cRider_updateSplitMiscFX and cFERider_update call it each frame.
 */
extern "C" void func_002ECF78(cBoltOnFXInterface* self)
{
    if (*(int*)self != 0) {
        func_002ECCB8(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sLensRow_CFA0 {
    char* objs[9];
    char pad[0xF0 - 0x24];
};

struct sLensVE_CFA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// The unit declares cRenderStateMan_m_state later as sLensFxSlot[]; bind a row view to the same symbol.
extern sLensRow_CFA0 D_lensRows_CFA0[] __asm__("cRenderStateMan_m_state");
extern int cRenderStateMan_m_initialized;

/**
 * Releases the render-state manager's world-painter queries.
 *
 * When cRenderStateMan_m_initialized is set, every non-null query object in the 9 slots of each of the 8 rows of
 * cRenderStateMan_m_state is destroyed through its virtual slot 1 (deleting destructor, flag 3). The flag is then
 * cleared.
 */
extern "C" void cRenderStateMan_purge(void)
{
    if (cRenderStateMan_m_initialized != 0) {
        int i;
        for (i = 0; i < 8; i++) {
            int j;
            for (j = 0; j < 9; j++) {
                tWorldPainterQuery* queryObj = (tWorldPainterQuery*)D_lensRows_CFA0[i].objs[j];
                if (((char*)queryObj) != 0) {
                    delete queryObj;
                }
            }
        }
    }
    cRenderStateMan_m_initialized = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cWorldPainterManArg {
    void* field_0x0;
};

struct sLensVec_ED048 {
    float x, y, z, w;
};

struct sLensRow_ED048 {
    void* q[9];                 // 0x00
    int f24;                    // 0x24
    sLensVec_ED048 v28;         // 0x28
    sLensVec_ED048 v38;         // 0x38
    char pad48[0xF0 - 0x48];
};

// The unit declares cRenderStateMan_m_state later as sLensFxSlot[]; bind views to the labels.
extern sLensRow_ED048 D_lensRows_ED048[] __asm__("cRenderStateMan_m_state");
extern int cRenderStateMan_m_initialized;
extern cWorldPainterManArg kWPR_Camera;
extern cWorldPainterManArg kWPR_Fog;
extern cWorldPainterManArg kWPR_LightGlow;
extern cWorldPainterManArg kWPR_ScreenTint;
extern cWorldPainterManArg kWPR_SkyBox;
extern cWorldPainterManArg kWPR_Sun;
extern cWorldPainterManArg kWPR_Surface;
extern cWorldPainterManArg kWPR_Lighting;
extern cWorldPainterManArg kWPR_Weather;
void* cWorldPainterMan_getQuery(void* self, cWorldPainterManArg* x);

static inline void lensVecOneED048(sLensVec_ED048* v)
{
    v->x = 1.0f;
    v->y = 1.0f;
    v->z = 1.0f;
    v->w = 1.0f;
}

/**
 * Sets up the render-state manager's world-painter queries for a newly loaded track.
 *
 * Takes the world painter manager from the game's world (+0x8) and, for each of the 8 rows of cRenderStateMan_m_state,
 * stores cWorldPainterMan_getQuery's result for the nine region types kWPR_Camera, kWPR_Fog, kWPR_LightGlow,
 * kWPR_ScreenTint, kWPR_SkyBox, kWPR_Sun, kWPR_Surface, kWPR_Lighting and kWPR_Weather (q[0]..q[8]), clears +0x24
 * and sets both colours at +0x28 and +0x38 to one. Then sets cRenderStateMan_m_initialized.
 */
extern "C" void cRenderStateMan_init(void)
{
    void* pm = SSXApp->Game->World->m_pWorldPainterMan;
    int i;
    cWorldPainterManArg* cameraRegion = &kWPR_Camera;
    cWorldPainterManArg* fogRegion = &kWPR_Fog;
    cWorldPainterManArg* lightGlowRegion = &kWPR_LightGlow;
    cWorldPainterManArg* screenTintRegion = &kWPR_ScreenTint;
    cWorldPainterManArg* skyBoxRegion = &kWPR_SkyBox;
    cWorldPainterManArg* sunRegion = &kWPR_Sun;
    cWorldPainterManArg* surfaceRegion = &kWPR_Surface;
    cWorldPainterManArg* lightingRegion = &kWPR_Lighting;
    cWorldPainterManArg* weatherRegion = &kWPR_Weather;
    for (i = 0; i < 8; i++) {
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[0] = cWorldPainterMan_getQuery(pm, cameraRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[1] = cWorldPainterMan_getQuery(pm, fogRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[2] = cWorldPainterMan_getQuery(pm, lightGlowRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[3] = cWorldPainterMan_getQuery(pm, screenTintRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[4] = cWorldPainterMan_getQuery(pm, skyBoxRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[5] = cWorldPainterMan_getQuery(pm, sunRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[6] = cWorldPainterMan_getQuery(pm, surfaceRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[7] = cWorldPainterMan_getQuery(pm, lightingRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[8] = cWorldPainterMan_getQuery(pm, weatherRegion);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].f24 = 0;
        lensVecOneED048(&D_lensRows_ED048[i].v28);
        lensVecOneED048(&D_lensRows_ED048[i].v38);
    }
    cRenderStateMan_m_initialized = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", tRenderState_blendToColour);
#ifdef SKIP_ASM
struct sLerpV4_D1D0 {
    float x, y, z, w;
    sLerpV4_D1D0() {}
    sLerpV4_D1D0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline sLerpV4_D1D0 operator*(const sLerpV4_D1D0& a, float s)
{
    return sLerpV4_D1D0(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sLerpV4_D1D0 operator+(const sLerpV4_D1D0& a, const sLerpV4_D1D0& b)
{
    return sLerpV4_D1D0(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

struct sLensLerp_D1D0 {
    char pad00[0x28];
    sLerpV4_D1D0 a;
    sLerpV4_D1D0 b;
};

/**
 * Blends a render state's two colours toward new targets: 90% of the way, or all the way when @p full is set.
 *
 * The colours are at +0x28 and +0x38 of the 0xF0-byte cRenderStateMan_m_state row. cRenderStateMan_update calls it.
 *
 * @param targetA  the target for the colour at +0x28
 * @param targetB  the target for the colour at +0x38
 * @param full  nonzero to jump straight to the targets
 */
extern "C" void tRenderState_blendToColour(tRenderState* self, const sLerpV4_D1D0* targetA, const sLerpV4_D1D0* targetB, int full)
{
    float weight = 0.8999999761581421f;
    float keep;
    if (full) {
        weight = 1.0f;
    }
    keep = 1.0f - weight;
    (*(sLerpV4_D1D0*)&self->m_colour[0]) = sLerpV4_D1D0(self->m_colour[0] * keep, self->m_colour[1] * keep, self->m_colour[2] * keep, self->m_colour[3] * keep) + *targetA * weight;
    (*(sLerpV4_D1D0*)&self->m_lightMapColour[0]) = sLerpV4_D1D0(self->m_lightMapColour[0] * keep, self->m_lightMapColour[1] * keep, self->m_lightMapColour[2] * keep, self->m_lightMapColour[3] * keep) + *targetB * weight;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", tRenderState_blendToColour1);
#ifdef SKIP_ASM
struct sLerpV4 {
    float x, y, z, w;
    sLerpV4() {}
    sLerpV4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline sLerpV4 operator*(const sLerpV4& a, float s)
{
    return sLerpV4(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sLerpV4 operator+(const sLerpV4& a, const sLerpV4& b)
{
    return sLerpV4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

struct sLensLerp {
    char pad00[0x28];
    sLerpV4 a;
    sLerpV4 b;
};

/**
 * Blends a render state's two colours toward new targets by @p weight.
 *
 * The colours at +0x28 and +0x38 become colour * (1 - @p weight) + target * @p weight. cRenderStateMan_update calls it.
 *
 * @param targetA  the target for the colour at +0x28
 * @param targetB  the target for the colour at +0x38
 * @param weight  the blend weight, 0..1
 */
extern "C" void tRenderState_blendToColour1(tRenderState* self, const sLerpV4* targetA, const sLerpV4* targetB, float weight)
{
    float keep = 1.0f - weight;
    (*(sLerpV4*)&self->m_colour[0]) = sLerpV4(self->m_colour[0] * keep, self->m_colour[1] * keep, self->m_colour[2] * keep, self->m_colour[3] * keep) + *targetA * weight;
    (*(sLerpV4*)&self->m_lightMapColour[0]) = sLerpV4(self->m_lightMapColour[0] * keep, self->m_lightMapColour[1] * keep, self->m_lightMapColour[2] * keep, self->m_lightMapColour[3] * keep) + *targetB * weight;
}
#endif

INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_update);

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_calculatePatchColour);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_DB20 {
    float x, y, z, w;
    sV4_DB20() {}
    sV4_DB20(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline sV4_DB20 operator-(const sV4_DB20& a, const sV4_DB20& b)
{
    return sV4_DB20(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

static inline sV4_DB20 operator*(const sV4_DB20& a, const sV4_DB20& b)
{
    return sV4_DB20(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline sV4_DB20 operator*(const sV4_DB20& a, float s)
{
    return sV4_DB20(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sV4_DB20 operator/(const sV4_DB20& a, const sV4_DB20& b)
{
    return sV4_DB20(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

struct sVE57_DB20 {
    short delta;
    short index;
    sV4_DB20 (*fn)(void*, float, float, short, int);
};

struct sLensSrc_DB20 {
    char pad00[0xC];
    int types;              // 0x0C: 3 bits per layer
    float x0, y0, w0, h0;   // 0x10
    float c00, c01, c02, c03, c10, c11, c12, c13; // 0x20
    char pad40[0x155 - 0x40];
    unsigned char page;     // 0x155
    short slot;             // 0x156
    char pad158[0x1A0 - 0x158];
    short tex[3];           // 0x1A0
};

extern sV4_DB20 D_004FAB00;

static inline int IsLoaded_DB20(sLensSrc_DB20* s)
{
    cWorldCache* wc = cWorld_m_pTheWorld->m_pWorldCache;
    if (s->slot < 0)
        return 0;
    int ok = wc->m_WorldView.mp_SectionState[s->page].state == 6;
    return ok ? wc->m_HullPages[s->slot].m_State == 3 : 0;
}

// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float Clamp_DB20(float f, float lo, float hi)
{
    return f >= lo ? f <? hi : lo;
}

static inline float Lerp_DB20(float a, float b, float t)
{
    return a + (b - a) * t;
}

static inline sV4_DB20 Sample_DB20(float u, float v, short tex)
{
    sVE57_DB20* vt = (sVE57_DB20*)VPTR_OF(cGraphicsMan, GrMan);
    return vt[58].fn(vthis(GrMan, vt[58]), u, v, tex, 1);
}

/**
 * Samples a world patch's textures at a point and derives the two colours for the render state from them.
 *
 * Does nothing for a null patch or one whose texture page is not loaded (world cache checks). Of the patch's three
 * layers (3 type bits each at +0xC), a type-5 layer is sampled at the patch rectangle (+0x10..+0x1C) point given by @p
 * tx, @p ty clamped to 0..1, and a type-1 layer at the UV interpolated from the corner values (+0x20..+0x3C), wrapped
 * to 0..1; samples come from GrMan virtual slot 58 with the layer texture (+0x1A0). Then *@p baseColour = (type-1 sample -
 * type-5 sample) * D_004FAB00 * (2 * type-5 sample.x) and *@p lightmapColour = *@p baseColour / type-1 sample, with both alphas (x) set to
 * 0.5. cRenderStateMan_update and tAvalancheNode_calculate call it.
 *
 * @param tx  u position on the patch, 0..1
 * @param ty  v position on the patch, 0..1
 * @param baseColour  receives the first colour
 * @param lightmapColour  receives the second colour
 */
extern "C" void cRenderStateMan_calculatePatchColour(sLensSrc_DB20* self, float tx, float ty, sV4_DB20* baseColour, sV4_DB20* lightmapColour)
{
    sV4_DB20 type5Sample(0.0f, 0.0f, 0.0f, 0.0f);
    sV4_DB20 type1Sample(0.0f, 0.0f, 0.0f, 0.0f);
    if (self && IsLoaded_DB20(self)) {
        for (int i = 0; i < 3; i++) {
            if (((self->types >> (i * 3)) & 7) == 5) {
                type5Sample = Sample_DB20(self->x0 + self->w0 * Clamp_DB20(tx, 0.0f, 1.0f), self->y0 + self->h0 * Clamp_DB20(ty, 0.0f, 1.0f), self->tex[i]);
            }
            if (((self->types >> (i * 3)) & 7) == 1) {
                float top = Lerp_DB20(self->c00, self->c10, tx);
                float bot = Lerp_DB20(self->c02, self->c12, tx);
                float lft = Lerp_DB20(self->c01, self->c03, ty);
                float rgt = Lerp_DB20(self->c11, self->c13, ty);
                float texU = Lerp_DB20(top, bot, ty) + 1000.0f;
                float texV = Lerp_DB20(lft, rgt, tx) + 1000.0f;
                type1Sample = Sample_DB20(texU - (float)(int)texU, texV - (float)(int)texV, self->tex[i]);
            }
        }
        float scale = type5Sample.x * 2.0f;
        *baseColour = (type1Sample - type5Sample) * D_004FAB00 * scale;
        *lightmapColour = *baseColour / type1Sample;
        baseColour->x = 0.5f;
        lightmapColour->x = 0.5f;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_calculateIrradianceBlend);
#ifdef SKIP_ASM
extern "C" float logf(float x);
extern "C" float expf(float x);

// PORT: abs.s via inline asm (as an SDK math-header fabsf would); gcc folds __builtin_fabsf of a constant.
static inline float absf_DF00(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float clamp_DF00(float f, float lo, float hi)
{
    if (f >= lo) {
        return f <? hi;
    }
    return lo;
}

/**
 * Maps a colour's luminance to a 0..1 factor: 0 at or below 0.1, 1 at or above 0.45, ((lum - 0.1) / 0.35)^0.6 between.
 *
 * The luminance is 0.299 r + 0.587 g + 0.114 b of the floats at +0x4, +0x8, +0xC (an a, r, g, b colour); the power uses
 * expf (expf) of logf (logf) times 0.6. cRenderStateMan_update calls it.
 *
 * @return the factor, 0..1
 */
extern "C" float cRenderStateMan_calculateIrradianceBlend(tARGB* self)
{
    float lum = self->R * 0.29899999499320984f
              + self->G * 0.5870000123977661f
              + self->B * 0.11400000005960464f;
    if (lum <= 0.10000000149011612f) {
        return 0.0f;
    }
    if (lum >= 0.44999998807907104f) {
        return 1.0f;
    }
    float k = 0.600117564201355f;
    float norm = (lum - 0.10000000149011612f) / absf_DF00(0.3499999940395355f);
    return clamp_DF00(expf(logf(norm) * k), 0.0f, 1.0f);
}
#endif

// the lens table cRenderStateMan_m_state (8 rows of 0xF0 bytes, config size 0x780); the unit types it as sLensFxSlot[] further down
extern char D_lensTable_DFB8[] __asm__("cRenderStateMan_m_state");

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_getIrradiance);
#ifdef SKIP_ASM
/**
 * Returns the irradiance of render state @p index: the address of its row's +0x50 in cRenderStateMan_m_state.
 *
 * cRider_SetRiderLights copies it for the rider's kRiderVt_getAIIndex.
 *
 * @param index  the render state (view) index
 * @return pointer to the row's irradiance block
 */
extern "C" void* cRenderStateMan_getIrradiance(int index)
{
    return D_lensTable_DFB8 + 0x50 + index * 0xf0;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_resolveIrradianceName);
#ifdef SKIP_ASM
extern char cIrradianceDataBase_TheDataBase[];
extern "C" int cIrradianceDataBase_GetIDByName(void*, void*);

/**
 * Resolves an irradiance name field in place: replaces its first word with the name's id and sets +0x4 to 0x123400 to
 * mark it resolved.
 *
 * Looks the name up with cIrradianceDataBase_GetIDByName in the irradiance database cIrradianceDataBase_TheDataBase. cRenderStateMan_Irradiance calls it
 * on first use.
 */
extern "C" void cRenderStateMan_resolveIrradianceName(tWPNameField* self)
{
    *(int*)self = cIrradianceDataBase_GetIDByName(cIrradianceDataBase_TheDataBase, self);
    self->m_nameId[1] = 0x123400;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_Irradiance);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cIrradianceDataBase_Get(void* self, int i);
extern int cIrradianceDataBase_mDefaultId;

extern "C" void* cRenderStateMan_Irradiance_r(void* self) __asm__("cRenderStateMan_Irradiance");
/**
 * Returns the irradiance record for an irradiance name field, resolving the name on first use.
 *
 * A field not yet tagged 0x123400 at +0x4 is resolved with cRenderStateMan_resolveIrradianceName, or, when its name is empty, gives the default
 * id cIrradianceDataBase_mDefaultId. The record comes from cIrradianceDataBase_Get on the irradiance database cIrradianceDataBase_TheDataBase. cRenderStateMan_getCustomIrradiance,
 * cRenderStateMan_LightIrradiance, cRenderStateMan_ShadowIrradiance, cRenderStateMan_TunnelIrradiance and cRenderStateMan_ObjectIrradiance call it.
 *
 * @return the irradiance record
 */
extern "C" void* cRenderStateMan_Irradiance_r(void* vself)
{
    tWPNameField* self = (tWPNameField*)vself;
    if (self->m_nameId[1] != 0x123400) {
        if (*(signed char*)self == 0) {
            return cIrradianceDataBase_Get(cIrradianceDataBase_TheDataBase, cIrradianceDataBase_mDefaultId);
        }
        cRenderStateMan_resolveIrradianceName((tWPNameField*)self);
        return cIrradianceDataBase_Get(cIrradianceDataBase_TheDataBase, *(int*)self);
    }
    return cIrradianceDataBase_Get(cIrradianceDataBase_TheDataBase, *(int*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunPitchOffset);
#ifdef SKIP_ASM
struct sLensFxVEntry {
    short delta;
    short index;
    float* (*fn)(void*);
};

struct sLensFxObj {
    int unk0;
    sLensFxVEntry* vt; // 0x4
};


extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun's pitch offset for render state @p i.
 *
 * Calls virtual slot 27 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun processing cSunHalo_Process uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunPitchOffset(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunPitchOffset();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunYawOffset);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun's yaw offset for render state @p i.
 *
 * Calls virtual slot 28 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun processing cSunHalo_Process uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunYawOffset(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunYawOffset();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunColorR);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun colour's red for render state @p i.
 *
 * Calls virtual slot 29 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunColorR(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunColorR();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunColorG);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun colour's green for render state @p i.
 *
 * Calls virtual slot 30 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunColorG(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunColorG();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunColorB);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun colour's blue for render state @p i.
 *
 * Calls virtual slot 31 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunColorB(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunColorB();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun's intensity for render state @p i.
 *
 * Calls virtual slot 32 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunType);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun type for render state @p i (an integer; -1 for no sun).
 *
 * Calls virtual slot 33 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it as a texture choice; the sun
 * processing cSunHalo_Process treats -1 for state 6 as no sun.
 *
 * @param i  the render state (view) index
 * @return the sun type
 */
extern "C" int cRenderStateMan_SunType(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return *(int*)((sLensFxVEntry*)VPTR_OF(tWPInGameData, obj))[33].fn(vthis(obj, ((sLensFxVEntry*)VPTR_OF(tWPInGameData, obj))[33]));
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_LensIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the lens-flare intensity for render state @p i.
 *
 * Calls virtual slot 34 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSunHalo_RenderLens uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LensIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->LensIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SunRadius);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sun's radius for render state @p i.
 *
 * Calls virtual slot 35 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw cSunHalo_Render uses it as the sprite size.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SunRadius(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[5]->m_data;
    return obj->SunRadius();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_getCustomIrradiance);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_Irradiance(void* p);

// $gp-relative object at gp+0x24C0 (no symbol in the target)
extern int cRenderStateMan_m_customIrradiance;

/**
 * Returns the irradiance record for the custom irradiance name (the name field cRenderStateMan_m_customIrradiance), through cRenderStateMan_Irradiance.
 *
 * The name is the one cRenderStateMan_setCustomIrradiance sets. The world light code (cWorldLightMan_getRealTimeLights) uses the result as
 * a light target. The C declaration says void; the record is passed through in the return register.
 *
 * @return the irradiance record
 */
extern "C" void cRenderStateMan_getCustomIrradiance(void)
{
    cRenderStateMan_Irradiance(&cRenderStateMan_m_customIrradiance);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_LightIrradiance);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_Irradiance(void* p);
extern "C" void* cRenderStateMan_LightIrradianceName(void);

/**
 * Returns the light irradiance record for a render state: the record for the name field cRenderStateMan_LightIrradianceName returns, through
 * cRenderStateMan_Irradiance.
 *
 * cRenderStateMan_LightIrradianceName reads query accessor slot 37 (the light irradiance name in On Tour's accessor order), or gives the
 * custom name cRenderStateMan_m_customIrradiance while cRenderStateMan_m_hasCustomIrradiance is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches cRenderStateMan_LightIrradianceName untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void cRenderStateMan_LightIrradiance(void)
{
    cRenderStateMan_Irradiance(cRenderStateMan_LightIrradianceName());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_ShadowIrradiance);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_Irradiance(void* p);
extern "C" void* cRenderStateMan_ShadowIrradianceName(void);

/**
 * Returns the shadow irradiance record for a render state: the record for the name field cRenderStateMan_ShadowIrradianceName returns, through
 * cRenderStateMan_Irradiance.
 *
 * cRenderStateMan_ShadowIrradianceName reads query accessor slot 38 (the shadow irradiance name in On Tour's accessor order), or gives the
 * custom name cRenderStateMan_m_customIrradiance while cRenderStateMan_m_hasCustomIrradiance is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches cRenderStateMan_ShadowIrradianceName untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void cRenderStateMan_ShadowIrradiance(void)
{
    cRenderStateMan_Irradiance(cRenderStateMan_ShadowIrradianceName());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_TunnelIrradiance);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_Irradiance(void* p);
extern "C" void* cRenderStateMan_TunnelIrradianceName(void);

/**
 * Returns the tunnel irradiance record for a render state: the record for the name field cRenderStateMan_TunnelIrradianceName returns, through
 * cRenderStateMan_Irradiance.
 *
 * cRenderStateMan_TunnelIrradianceName reads query accessor slot 39 (the tunnel irradiance name in On Tour's accessor order), or gives the
 * custom name cRenderStateMan_m_customIrradiance while cRenderStateMan_m_hasCustomIrradiance is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches cRenderStateMan_TunnelIrradianceName untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void cRenderStateMan_TunnelIrradiance(void)
{
    cRenderStateMan_Irradiance(cRenderStateMan_TunnelIrradianceName());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_ObjectIrradiance);
#ifdef SKIP_ASM
extern "C" void* cRenderStateMan_ObjectIrradianceName();

/**
 * Returns the object irradiance record for a render state: the record for the name field cRenderStateMan_ObjectIrradianceName returns, through
 * cRenderStateMan_Irradiance.
 *
 * cRenderStateMan_ObjectIrradianceName reads query accessor slot 40 (the object irradiance name in On Tour's accessor order), or gives the
 * custom name cRenderStateMan_m_customIrradiance while cRenderStateMan_m_hasCustomIrradiance is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches cRenderStateMan_ObjectIrradianceName untouched and the record comes back in v0. Nothing in the retail code
 * references it.
 *
 * @return the irradiance record
 */
extern "C" void cRenderStateMan_ObjectIrradiance(void)
{
    cRenderStateMan_Irradiance(cRenderStateMan_ObjectIrradianceName());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_DrawDistance);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the draw distance for render state @p i.
 *
 * Calls virtual slot 6 of the state's world-painter query object m_query[0] (+0x0) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cCamera_updateCamMat clamps the far plane to it and
 * cVisualEffectsMan_commonUpdate stores it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_DrawDistance(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[0]->m_data;
    return obj->DrawDistance();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SkyBoxRotation);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the sky box rotation for render state @p i.
 *
 * Calls virtual slot 26 of the state's world-painter query object m_query[4] (+0x10) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cGame_update sets cAppMan_TheAppMan +0x34 when it is above 0.9
 * for state 6.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SkyBoxRotation(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[4]->m_data;
    return obj->SkyBoxRotation();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowIntensity);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the snow intensity for render state @p i, reduced inside tunnels.
 *
 * Calls virtual slot 43 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The value is scaled by (1 - the row's +0x24, On Tour m_inTunnel),
 * and is 0 while cRenderStateMan_m_indoor is set. cSnowFall_update uses it as snow parameter 0.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowIntensity(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* obj = renderState->m_query[8]->m_data;
        return obj->SnowIntensity() * (1.0f - renderState->m_inTunnel);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_FlakeSize);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow flake size for render state @p i.
 *
 * Calls virtual slot 44 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 4.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FlakeSize(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->FlakeSize();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_WindIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the wind intensity for render state @p i, reduced inside tunnels.
 *
 * Calls virtual slot 45 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The value is scaled by (1 - the row's +0x24). cSnowFall_update
 * uses it as snow parameter 1 (wind strength).
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_WindIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->WindIntensity() * (1.0f - renderState->m_inTunnel);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_WindRotation);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the wind direction angle for render state @p i, in degrees.
 *
 * Calls virtual slot 46 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 5 (the wind angle) and
 * cRider_updateWind reads it too.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_WindRotation(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->WindRotation();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowFlurries);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow flurry amount for render state @p i.
 *
 * Calls virtual slot 49 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 6, the mix of random
 * flurries into the wind.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFlurries(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->SnowFlurries();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowFluffItensity);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the snow fluff intensity for render state @p i.
 *
 * Calls virtual slot 51 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It is 0 while cRenderStateMan_m_indoor is set. cSnowFall_update uses it as snow
 * parameter 2, the flake count of mode-1 (fluff) boxes.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffItensity(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* obj = renderState->m_query[8]->m_data;
        return obj->SnowFluffItensity();
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowFluffWindIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow fluff wind intensity for render state @p i.
 *
 * Calls virtual slot 52 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 3, the wind strength
 * of mode-1 (fluff) boxes.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffWindIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->SnowFluffWindIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowGravityMultiplier);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow gravity multiplier for render state @p i.
 *
 * Calls virtual slot 50 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 7, the fall-speed
 * factor.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowGravityMultiplier(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->SnowGravityMultiplier();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_Temperature);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the temperature for render state @p i (accessor slot 48).
 *
 * Calls virtual slot 48 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. func_002E1120 fades the rider's head puffs out as it rises from
 * -5 to 20.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_Temperature(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->Temperature();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_LightningChance);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the lightning chance for render state @p i.
 *
 * Calls virtual slot 53 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_LightningChance.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightningChance(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->LightningChance();
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_SnowFlakeColourA);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow flake colour's alpha for render state @p i.
 *
 * Calls virtual slot 54 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as the first value of the first snow
 * colour set.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFlakeColourA(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* obj = renderState->m_query[8]->m_data;
    return obj->SnowFlakeColourA();
}
#endif

