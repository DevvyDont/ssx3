#include "common.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

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
void* cPS2LensFxMan_cPS2LensFxMan_impl(void* self)
{
    cLensFxMan_cLensFxMan(self);
    *(int*)((char*)self + 0x649c) = (int)(void*)cPS2LensFxMan_vtbl;
    return self;
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
void* cPS2LensFxMan__cPS2LensFxMan(void* self)
{
    *(int*)((char*)self + 0x649c) = (int)(void*)cPS2LensFxMan_vtbl;
    return cLensFxMan__cLensFxMan(self);
}
#endif

/**
 * Measures how much of a lens-flare halo is visible by reading the depth buffer back from the GS.
 *
 * Converts the halo's screen rectangle (floats at +0x0..+0x24 of the second argument) to ints, gets a depth threshold
 * from func_004139F8, builds a GS-to-host transfer of that area of the Z buffer in the object's packet buffer
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
INCLUDE_ASM("visualfx/lensfx", func_002EC9E0);
#ifdef SKIP_ASM
extern void* D_00487EE8[];
struct sLensFx;
extern "C" void func_002ECAF0(sLensFx* self);

/**
 * Constructs a rider's bolt-on effect set ("cBoltOnFXInterface"): the attached particle emitters its equipment carries.
 *
 * Installs the vtable D_00487EE8 at +0xC4, marks the set dirty (+0x0 = 1), stores the rider (+0x4) and the
 * attached-emitter owner (+0xC), clears the character pointer (+0x8) and empties the 15 entries (+0x10, 0xC bytes each:
 * emitter index, item, kind) through func_002EC9D0 and func_002ECAF0. cRider_cRider builds it on 200 bytes with the
 * rider's mAttachedEmitterFX and stores it as mBoltOnFX.
 *
 * @param a1  the rider
 * @param a2  the attached-emitter owner the emitters are created in
 * @return @p self
 */
extern "C" void* func_002EC9E0(void* self, int a1, int a2)
{
    *(void***)((char*)self + 0xC4) = D_00487EE8;
    *(int*)((char*)self + 0x0) = 1;
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0xC) = a2;
    *(int*)((char*)self + 0x8) = 0;
    char* p = (char*)self + 0x10;
    int i;
    for (i = 14; i != -1; i--, p += 0xC) {
        func_002EC9D0(p);
    }
    func_002ECAF0((sLensFx*)self);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002ECA68);
#ifdef SKIP_ASM
extern void* D_00487EE8[];
struct sLensFx;
extern "C" void func_002ECAF0(sLensFx* self);

/**
 * Constructs a front-end character's bolt-on effect set ("cBoltOnFXInterface"), keyed by a character id instead of a
 * rider.
 *
 * Same as func_002EC9E0 except that it stores a pointer to the character id (+0x8) and leaves the rider (+0x4) 0.
 * cFERider_init builds it on 200 bytes.
 *
 * @param a1  pointer to the character id
 * @param a2  the attached-emitter owner the emitters are created in
 * @return @p self
 */
extern "C" void* func_002ECA68(void* self, int a1, int a2)
{
    *(void***)((char*)self + 0xC4) = D_00487EE8;
    *(int*)((char*)self + 0x0) = 1;
    *(int*)((char*)self + 0x8) = a1;
    *(int*)((char*)self + 0xC) = a2;
    *(int*)((char*)self + 0x4) = 0;
    char* p = (char*)self + 0x10;
    int i;
    for (i = 14; i != -1; i--, p += 0xC) {
        func_002EC9D0(p);
    }
    func_002ECAF0((sLensFx*)self);
    return self;
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
 * The two constructors func_002EC9E0 and func_002ECA68 call it.
 */
extern "C" void func_002ECAF0(sLensFx* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        self->entries[i].id = -1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECB28);
#ifdef SKIP_ASM
extern void* D_00487EE8[];
extern "C" void func_002F2218(int a, int id);
void operator_delete(int* ptr);

/**
 * Destroys a bolt-on effect set: releases every attached emitter it still holds, then frees the object when asked.
 *
 * Re-installs the vtable D_00487EE8, releases each used entry's emitter from the owner (+0xC) with func_002F2218 and
 * marks it free, then frees the object with operator_delete when bit 0 of @p flags is set. It is the destructor slot of
 * D_00487EE8.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void func_002ECB28(sLensFx* self, int flags)
{
    *(void***)((char*)self + 0xC4) = D_00487EE8;
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

extern "C" int func_002ECC28(void* self, int a1, int a2);

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
    return func_002ECC28(self, kind, itemId);
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
extern "C" int func_002ECC28(void* lensMgr, int x, int y)
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
extern "C" void* func_0014ABE0();
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
extern "C" void func_002ECCB8(void* vself)
{
    sLensMgr_CCB8* self = (sLensMgr_CCB8*)vself;
    int used[15];
    for (int z = 14; z >= 0; z--) {
        used[z] = 0;
    }
    void* lib = func_0014ABE0();
    char* tbl = 0;
    int id = -1;
    char* rider = self->rider;
    if (rider != 0) {
        char* being = rider + 0x6C0;
        sVEi_CCB8* vt = *(sVEi_CCB8**)being;
        tbl = (char*)func_0014AD50(lib, vt[7].fn(being + vt[7].delta));
        id = cRider_getCheatIdentity(self->rider);
        if (id < 10) {
            id = cRider_getIdentity(self->rider);
        }
    } else if (self->charId != 0) {
        char* st = (char*)cUIStateStack_getCurrentState(*(char**)((char*)SSXApp->RealFE + 0xC) + 0x18);
        tbl = (char*)func_0014AD28(lib, *(signed char*)(st + 0x44), *self->charId);
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
        short tableIndex = (*(short**)(tbl + 0x288))[item->slot];
        char* tableEntry;
        if (tableIndex >= 0) {
            tableEntry = tbl + (tableIndex * 4 + 656);
        } else {
            tableEntry = 0;
        }
        if (id < 10 && (*(unsigned short*)(tableEntry + 2) & 0x10) == 0) {
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
extern "C" void func_002ECCB8(void* self);

/**
 * Rebuilds a bolt-on effect set (func_002ECCB8) when it is marked dirty (+0x0).
 *
 * cRider_updateSplitMiscFX and cFERider_update call it each frame.
 */
extern "C" void func_002ECF78(void* self)
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
                char* queryObj = D_lensRows_CFA0[i].objs[j];
                if (queryObj != 0) {
                    sLensVE_CFA0* vt = *(sLensVE_CFA0**)(queryObj + 4);
                    vt[1].fn(queryObj + vt[1].delta, 3);
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
    void* pm = *(void**)((char*)SSXApp->Game->World + 0x8);
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
INCLUDE_ASM("visualfx/lensfx", func_002ED1D0);
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
extern "C" void func_002ED1D0(sLensLerp_D1D0* self, const sLerpV4_D1D0* targetA, const sLerpV4_D1D0* targetB, int full)
{
    float weight = 0.8999999761581421f;
    float keep;
    if (full) {
        weight = 1.0f;
    }
    keep = 1.0f - weight;
    self->a = sLerpV4_D1D0(self->a.x * keep, self->a.y * keep, self->a.z * keep, self->a.w * keep) + *targetA * weight;
    self->b = sLerpV4_D1D0(self->b.x * keep, self->b.y * keep, self->b.z * keep, self->b.w * keep) + *targetB * weight;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ED338);
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
extern "C" void func_002ED338(sLensLerp* self, const sLerpV4* targetA, const sLerpV4* targetB, float weight)
{
    float keep = 1.0f - weight;
    self->a = sLerpV4(self->a.x * keep, self->a.y * keep, self->a.z * keep, self->a.w * keep) + *targetA * weight;
    self->b = sLerpV4(self->b.x * keep, self->b.y * keep, self->b.z * keep, self->b.w * keep) + *targetB * weight;
}
#endif

INCLUDE_ASM("visualfx/lensfx", cRenderStateMan_update);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDB20);
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
    char* wc = (char*)cWorld_m_pTheWorld->m_pWorldCache;
    if (s->slot < 0)
        return 0;
    int ok = *(int*)(wc + (s->page << 3) + 0x24) == 6;
    return ok ? *(int*)(wc + s->slot * 0x18 + 0x3F0) == 3 : 0;
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
    sVE57_DB20* vt = (sVE57_DB20*)GrMan->mVtbl;
    return vt[58].fn((char*)GrMan + vt[58].delta, u, v, tex, 1);
}

/**
 * Samples a world patch's textures at a point and derives the two colours for the render state from them.
 *
 * Does nothing for a null patch or one whose texture page is not loaded (world cache checks). Of the patch's three
 * layers (3 type bits each at +0xC), a type-5 layer is sampled at the patch rectangle (+0x10..+0x1C) point given by @p
 * tx, @p ty clamped to 0..1, and a type-1 layer at the UV interpolated from the corner values (+0x20..+0x3C), wrapped
 * to 0..1; samples come from GrMan virtual slot 58 with the layer texture (+0x1A0). Then *@p o4 = (type-1 sample -
 * type-5 sample) * D_004FAB00 * (2 * type-5 sample.x) and *@p o5 = *@p o4 / type-1 sample, with both alphas (x) set to
 * 0.5. cRenderStateMan_update and tAvalancheNode_calculate call it.
 *
 * @param tx  u position on the patch, 0..1
 * @param ty  v position on the patch, 0..1
 * @param o4  receives the first colour
 * @param o5  receives the second colour
 */
extern "C" void func_002EDB20(sLensSrc_DB20* self, float tx, float ty, sV4_DB20* o4, sV4_DB20* o5)
{
    sV4_DB20 a(0.0f, 0.0f, 0.0f, 0.0f);
    sV4_DB20 b(0.0f, 0.0f, 0.0f, 0.0f);
    if (self && IsLoaded_DB20(self)) {
        for (int i = 0; i < 3; i++) {
            if (((self->types >> (i * 3)) & 7) == 5) {
                a = Sample_DB20(self->x0 + self->w0 * Clamp_DB20(tx, 0.0f, 1.0f), self->y0 + self->h0 * Clamp_DB20(ty, 0.0f, 1.0f), self->tex[i]);
            }
            if (((self->types >> (i * 3)) & 7) == 1) {
                float top = Lerp_DB20(self->c00, self->c10, tx);
                float bot = Lerp_DB20(self->c02, self->c12, tx);
                float lft = Lerp_DB20(self->c01, self->c03, ty);
                float rgt = Lerp_DB20(self->c11, self->c13, ty);
                float u = Lerp_DB20(top, bot, ty) + 1000.0f;
                float v = Lerp_DB20(lft, rgt, tx) + 1000.0f;
                b = Sample_DB20(u - (float)(int)u, v - (float)(int)v, self->tex[i]);
            }
        }
        float scale = a.x * 2.0f;
        *o4 = (b - a) * D_004FAB00 * scale;
        *o5 = *o4 / b;
        o4->x = 0.5f;
        o5->x = 0.5f;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDF00);
#ifdef SKIP_ASM
extern "C" float func_0040DA10(float x);
extern "C" float func_0040D758(float x);

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
 * func_0040D758 (expf) of func_0040DA10 (logf) times 0.6. cRenderStateMan_update calls it.
 *
 * @return the factor, 0..1
 */
extern "C" float func_002EDF00(void* self)
{
    float lum = *(float*)((char*)self + 0x4) * 0.29899999499320984f
              + *(float*)((char*)self + 0x8) * 0.5870000123977661f
              + *(float*)((char*)self + 0xC) * 0.11400000005960464f;
    if (lum <= 0.10000000149011612f) {
        return 0.0f;
    }
    if (lum >= 0.44999998807907104f) {
        return 1.0f;
    }
    float k = 0.600117564201355f;
    float norm = (lum - 0.10000000149011612f) / absf_DF00(0.3499999940395355f);
    return clamp_DF00(func_0040D758(func_0040DA10(norm) * k), 0.0f, 1.0f);
}
#endif

// the lens table cRenderStateMan_m_state (8 rows of 0xF0 bytes, config size 0x780); the unit types it as sLensFxSlot[] further down
extern char D_lensTable_DFB8[] __asm__("cRenderStateMan_m_state");

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDFB8);
#ifdef SKIP_ASM
/**
 * Returns the irradiance of render state @p index: the address of its row's +0x50 in cRenderStateMan_m_state.
 *
 * cRider_SetRiderLights copies it for the rider's kRiderVt_getAIIndex.
 *
 * @param index  the render state (view) index
 * @return pointer to the row's irradiance block
 */
extern "C" void* func_002EDFB8(int index)
{
    return D_lensTable_DFB8 + 0x50 + index * 0xf0;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDFD0);
#ifdef SKIP_ASM
extern char D_005047F8[];
extern "C" int cIrradianceDataBase_GetIDByName(void*, void*);

/**
 * Resolves an irradiance name field in place: replaces its first word with the name's id and sets +0x4 to 0x123400 to
 * mark it resolved.
 *
 * Looks the name up with cIrradianceDataBase_GetIDByName in the irradiance database D_005047F8. func_002EE010 calls it
 * on first use.
 */
extern "C" void func_002EDFD0(void* self)
{
    *(int*)self = cIrradianceDataBase_GetIDByName(D_005047F8, self);
    *(int*)((char*)self + 4) = 0x123400;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE010);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0038ABF8(void* self, int i);
extern int D_004A43C4;

extern "C" void* func_002EE010_r(void* self) __asm__("func_002EE010");
/**
 * Returns the irradiance record for an irradiance name field, resolving the name on first use.
 *
 * A field not yet tagged 0x123400 at +0x4 is resolved with func_002EDFD0, or, when its name is empty, gives the default
 * id D_004A43C4. The record comes from func_0038ABF8 on the irradiance database D_005047F8. func_002EE2F8,
 * func_002EE318, func_002EE340, func_002EE368 and func_002EE390 call it.
 *
 * @return the irradiance record
 */
extern "C" void* func_002EE010_r(void* self)
{
    if (*(int*)((char*)self + 4) != 0x123400) {
        if (*(signed char*)self == 0) {
            return func_0038ABF8(D_005047F8, D_004A43C4);
        }
        func_002EDFD0(self);
        return func_0038ABF8(D_005047F8, *(int*)self);
    }
    return func_0038ABF8(D_005047F8, *(int*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE070);
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

struct sLensFxSlot {
    sLensFxObj** p0;   // 0x0
    int pad4[3];
    sLensFxObj** p10;  // 0x10
    sLensFxObj** p14;  // 0x14
    int pad18[2];
    sLensFxObj** p20;  // 0x20
    char pad24[0xF0 - 0x24];
};

extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun's pitch offset for render state @p i.
 *
 * Calls virtual slot 27 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun processing func_002F4DB8 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE070(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[27].fn((char*)obj + obj->vt[27].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE0B8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun's yaw offset for render state @p i.
 *
 * Calls virtual slot 28 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun processing func_002F4DB8 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE0B8(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[28].fn((char*)obj + obj->vt[28].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE100);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun colour's red for render state @p i.
 *
 * Calls virtual slot 29 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE100(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[29].fn((char*)obj + obj->vt[29].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE148);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun colour's green for render state @p i.
 *
 * Calls virtual slot 30 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE148(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[30].fn((char*)obj + obj->vt[30].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE190);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun colour's blue for render state @p i.
 *
 * Calls virtual slot 31 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE190(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[31].fn((char*)obj + obj->vt[31].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE1D8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun's intensity for render state @p i.
 *
 * Calls virtual slot 32 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE1D8(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[32].fn((char*)obj + obj->vt[32].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE220);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun type for render state @p i (an integer; -1 for no sun).
 *
 * Calls virtual slot 33 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it as a texture choice; the sun
 * processing func_002F4DB8 treats -1 for state 6 as no sun.
 *
 * @param i  the render state (view) index
 * @return the sun type
 */
extern "C" int func_002EE220(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *(int*)obj->vt[33].fn((char*)obj + obj->vt[33].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE268);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the lens-flare intensity for render state @p i.
 *
 * Calls virtual slot 34 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. func_002F4690 uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE268(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[34].fn((char*)obj + obj->vt[34].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE2B0);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the sun's radius for render state @p i.
 *
 * Calls virtual slot 35 of the state's world-painter query object m_query[5] (+0x14) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The sun draw func_002F4A08 uses it as the sprite size.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE2B0(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[35].fn((char*)obj + obj->vt[35].delta);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE2F8);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);

// $gp-relative object at gp+0x24C0 (no symbol in the target)
extern int D_004A55B0;

/**
 * Returns the irradiance record for the custom irradiance name (the name field D_004A55B0), through func_002EE010.
 *
 * The name is the one cRenderStateMan_setCustomIrradiance sets. The world light code (func_002F5400) uses the result as
 * a light target. The C declaration says void; the record is passed through in the return register.
 *
 * @return the irradiance record
 */
extern "C" void func_002EE2F8(void)
{
    func_002EE010(&D_004A55B0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE318);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF0E8(void);

/**
 * Returns the light irradiance record for a render state: the record for the name field func_002EF0E8 returns, through
 * func_002EE010.
 *
 * func_002EF0E8 reads query accessor slot 37 (the light irradiance name in On Tour's accessor order), or gives the
 * custom name D_004A55B0 while D_004A3B64 is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches func_002EF0E8 untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void func_002EE318(void)
{
    func_002EE010(func_002EF0E8());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE340);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF140(void);

/**
 * Returns the shadow irradiance record for a render state: the record for the name field func_002EF140 returns, through
 * func_002EE010.
 *
 * func_002EF140 reads query accessor slot 38 (the shadow irradiance name in On Tour's accessor order), or gives the
 * custom name D_004A55B0 while D_004A3B64 is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches func_002EF140 untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void func_002EE340(void)
{
    func_002EE010(func_002EF140());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE368);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF198(void);

/**
 * Returns the tunnel irradiance record for a render state: the record for the name field func_002EF198 returns, through
 * func_002EE010.
 *
 * func_002EF198 reads query accessor slot 39 (the tunnel irradiance name in On Tour's accessor order), or gives the
 * custom name D_004A55B0 while D_004A3B64 is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches func_002EF198 untouched and the record comes back in v0. cRenderStateMan_update calls it.
 *
 * @return the irradiance record
 */
extern "C" void func_002EE368(void)
{
    func_002EE010(func_002EF198());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE390);
#ifdef SKIP_ASM
extern "C" void* func_002EF1F0();

/**
 * Returns the object irradiance record for a render state: the record for the name field func_002EF1F0 returns, through
 * func_002EE010.
 *
 * func_002EF1F0 reads query accessor slot 40 (the object irradiance name in On Tour's accessor order), or gives the
 * custom name D_004A55B0 while D_004A3B64 is set. The C definition declares no parameters and a void result: the render
 * state index in a0 reaches func_002EF1F0 untouched and the record comes back in v0. Nothing in the retail code
 * references it.
 *
 * @return the irradiance record
 */
extern "C" void func_002EE390(void)
{
    func_002EE010(func_002EF1F0());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE3B8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE3B8(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p0;
    return *obj->vt[6].fn((char*)obj + obj->vt[6].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE400);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE400(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p10;
    return *obj->vt[26].fn((char*)obj + obj->vt[26].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE448);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the snow intensity for render state @p i, reduced inside tunnels.
 *
 * Calls virtual slot 43 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. The value is scaled by (1 - the row's +0x24, On Tour m_inTunnel),
 * and is 0 while D_004A3B60 is set. cSnowFall_update uses it as snow parameter 0.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE448(int i)
{
    if (D_004A3B60 == 0) {
        sLensFxSlot* s = &cRenderStateMan_m_state[i];
        sLensFxObj* obj = *s->p20;
        return *obj->vt[43].fn((char*)obj + obj->vt[43].delta) * (1.0f - *(float*)((char*)s + 0x24));
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE4C0);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

/**
 * Returns the snow flake size for render state @p i.
 *
 * Calls virtual slot 44 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as snow parameter 4.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE4C0(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[44].fn((char*)obj + obj->vt[44].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE508);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE508(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[45].fn((char*)obj + obj->vt[45].delta) * (1.0f - *(float*)((char*)s + 0x24));
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE570);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE570(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[46].fn((char*)obj + obj->vt[46].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE5B8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE5B8(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[49].fn((char*)obj + obj->vt[49].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE600);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the snow fluff intensity for render state @p i.
 *
 * Calls virtual slot 51 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It is 0 while D_004A3B60 is set. cSnowFall_update uses it as snow
 * parameter 2, the flake count of mode-1 (fluff) boxes.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float func_002EE600(int i)
{
    if (D_004A3B60 == 0) {
        sLensFxSlot* s = &cRenderStateMan_m_state[i];
        sLensFxObj* obj = *s->p20;
        return *obj->vt[51].fn((char*)obj + obj->vt[51].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE660);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE660(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[52].fn((char*)obj + obj->vt[52].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE6A8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE6A8(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[50].fn((char*)obj + obj->vt[50].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE6F0);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE6F0(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[48].fn((char*)obj + obj->vt[48].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE738);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE738(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[53].fn((char*)obj + obj->vt[53].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE780);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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
extern "C" float func_002EE780(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[54].fn((char*)obj + obj->vt[54].delta);
}
#endif

