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
                char* o = D_lensRows_CFA0[i].objs[j];
                if (o != 0) {
                    sLensVE_CFA0* vt = *(sLensVE_CFA0**)(o + 4);
                    vt[1].fn(o + vt[1].delta, 3);
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
extern cWorldPainterManArg D_004A5560;
extern cWorldPainterManArg D_004A5568;
extern cWorldPainterManArg D_004A5570;
extern cWorldPainterManArg D_004A5578;
extern cWorldPainterManArg D_004A5580;
extern cWorldPainterManArg D_004A5588;
extern cWorldPainterManArg D_004A5590;
extern cWorldPainterManArg kWPR_Lighting;
extern cWorldPainterManArg D_004A55A0;
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
 * stores cWorldPainterMan_getQuery's result for the nine region types D_004A5560..D_004A55A0 (kWPR_Lighting among
 * them), clears +0x24 and sets both colours at +0x28 and +0x38 to one. Then sets cRenderStateMan_m_initialized.
 */
extern "C" void cRenderStateMan_init(void)
{
    void* pm = *(void**)((char*)SSXApp->Game->World + 0x8);
    int i;
    cWorldPainterManArg* a0 = &D_004A5560;
    cWorldPainterManArg* a1 = &D_004A5568;
    cWorldPainterManArg* a2 = &D_004A5570;
    cWorldPainterManArg* a3 = &D_004A5578;
    cWorldPainterManArg* a4 = &D_004A5580;
    cWorldPainterManArg* a5 = &D_004A5588;
    cWorldPainterManArg* a6 = &D_004A5590;
    cWorldPainterManArg* a7 = &kWPR_Lighting;
    cWorldPainterManArg* a8 = &D_004A55A0;
    for (i = 0; i < 8; i++) {
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[0] = cWorldPainterMan_getQuery(pm, a0);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[1] = cWorldPainterMan_getQuery(pm, a1);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[2] = cWorldPainterMan_getQuery(pm, a2);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[3] = cWorldPainterMan_getQuery(pm, a3);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[4] = cWorldPainterMan_getQuery(pm, a4);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[5] = cWorldPainterMan_getQuery(pm, a5);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[6] = cWorldPainterMan_getQuery(pm, a6);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[7] = cWorldPainterMan_getQuery(pm, a7);
        ((sLensRow_ED048*)&D_lensRows_ED048)[i].q[8] = cWorldPainterMan_getQuery(pm, a8);
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

extern "C" void func_002EE368(void)
{
    func_002EE010(func_002EF198());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE390);
#ifdef SKIP_ASM
extern "C" void* func_002EF1F0();

extern "C" void func_002EE390(void)
{
    func_002EE010(func_002EF1F0());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE3B8);
#ifdef SKIP_ASM
extern sLensFxSlot cRenderStateMan_m_state[];

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

extern "C" float func_002EE780(int i)
{
    sLensFxSlot* s = &cRenderStateMan_m_state[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[54].fn((char*)obj + obj->vt[54].delta);
}
#endif

