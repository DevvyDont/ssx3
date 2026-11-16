#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"



//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_getInstanceMan__Fv);
#ifdef SKIP_ASM
/**
 * Returns the game's instance manager.
 *
 * @return mInstanceMan of SSXApp->Game
 */
void* cObjectInterface_getInstanceMan()
{
    return ((void*)(SSXApp->Game)->InstanceMan);
}
#endif

extern "C" void* func_002D9C00(void* self);

//100%
INCLUDE_ASM("util/objectinterface", func_002D1CF0__FPv);
#ifdef SKIP_ASM
void* func_002D1CF0(void* self)
{
    return func_002D9C00(self);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_renderSimpleLightHalo);
#ifdef SKIP_ASM

static inline void setA_1D10(sDrawState* drawState, int v, int mask, int shift)
{
    drawState->unk_00[1] = (((int)drawState->unk_00[1]) & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void setB_1D10(sDrawState* drawState, int v, int mask, int shift)
{
    drawState->unk_00[2] = (((int)drawState->unk_00[2]) & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

// 2D vector (8 bytes, BLKmode: its temporaries share 16-byte frame slots that combine when freed).
// The ctor takes const float&: a by-value float ctor folds the 1.0f into the inlined
// stores, which then fail to validate and leave the uv1 temp addressed through a register.
struct sVec2_1D10 {
    float x, y;
    sVec2_1D10(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sCol_1D10 {
    float a, r, g, b;
};

struct sVE_1D10 {
    short delta;
    short index;
    void* fn;
};

typedef int (*IsHiddenFn_1D10)(void*, void*);
typedef void (*DrawFn_1D10)(void*, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*);
typedef void (*DrawRotFn_1D10)(void*, float, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*, int);

struct sRCtx_1D10 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
    char padE88[0xF50 - 0xE88];
    int texs[0x40];             // 0xF50
    char pad1050[0x10D8 - 0x1050];
    sVE_1D10* vt;               // 0x10D8
};

static inline int tex_1D10(sRCtx_1D10* c, int i)
{
    return c->texs[i];
}

extern sDrawState sDrawState_s_defaultDrawState;

/**
 * Draws a light halo sprite at a world position.
 *
 * Returns early when the graphics manager's virtual slot 91 says the position is hidden. Otherwise pushes a copy of the
 * default draw state with additive-style blend bits, binds texture @p kind + 0x25 and draws a @p size square sprite in
 * the colour @p src: rotated by @p angle degrees through virtual slot 79 (a second, counter-rotated sprite for kinds 1
 * and 4) or unrotated through slot 78. The draw state is popped at the end.
 *
 * @param obj  the halo's world position (On Tour P)
 * @param src  the colour as r, g, b, a floats (On Tour color)
 * @param kind  halo type; selects the texture (On Tour type)
 * @param size  sprite width and height (On Tour Radius)
 * @param angle  rotation in degrees, 0 for none (On Tour Angle)
 */
extern "C" void cObjectInterface_renderSimpleLightHalo(void* obj, float* src, int kind, float size, float angle)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    if (((IsHiddenFn_1D10)((sVE_1D10*)ps2GraphicsMan->mVtbl)[91].fn)((char*)ps2GraphicsMan + ((sVE_1D10*)ps2GraphicsMan->mVtbl)[91].delta, obj) != 0) {
        return;
    }
    sCol_1D10 col;
    col.a = src[3];
    col.r = src[0];
    col.g = src[1];
    col.b = src[2];
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    (ctx->m_pCurrentDrawState)[1] = (ctx->m_pCurrentDrawState)[0];
    (*(sDrawState**)&ctx->m_pCurrentDrawState)++;
    *(ctx->m_pCurrentDrawState) = sDrawState_s_defaultDrawState;
    setA_1D10((ctx->m_pCurrentDrawState), 1, 1, 22);
    setA_1D10((ctx->m_pCurrentDrawState), 0, 3, 0);
    setB_1D10((ctx->m_pCurrentDrawState), 0, 0x7FFFF, 10);
    setA_1D10((ctx->m_pCurrentDrawState), 7, 0x1F, 2);
    setB_1D10((ctx->m_pCurrentDrawState), 8, 0x1F, 5);
    setA_1D10((ctx->m_pCurrentDrawState), 1, 3, 23);
    setA_1D10((ctx->m_pCurrentDrawState), 0, 3, 20);
    setA_1D10((ctx->m_pCurrentDrawState), 20, 0xFF, 12);
    (ctx->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex_1D10((sRCtx_1D10*)ctx, kind + 37);
    if (angle != 0.0f) {
        ((DrawRotFn_1D10)((sVE_1D10*)ctx->mVtbl)[79].fn)((char*)ctx + ((sVE_1D10*)ctx->mVtbl)[79].delta, angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        if (kind == 1 || kind == 4) {
            ((DrawRotFn_1D10)((sVE_1D10*)ctx->mVtbl)[79].fn)((char*)ctx + ((sVE_1D10*)ctx->mVtbl)[79].delta, -angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        }
    } else {
        ((DrawFn_1D10)((sVE_1D10*)ctx->mVtbl)[78].fn)((char*)ctx + ((sVE_1D10*)ctx->mVtbl)[78].delta, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col);
    }
    (*(sDrawState**)&ctx->m_pCurrentDrawState)--;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2058);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x828)
extern char D_828[];

struct sQ128 { float v[4]; } __attribute__((aligned(16)));

// PORT: PS2-only VU0 microprogram call (ctc2/vcallmsr/cfc2); the PC port needs a C version.
extern "C" int func_002D2058(sQ128* box)
{
    char* inst = (char*)(void*)SSXApp->Game->OcclusionMan;
    int mask = *(int*)(inst + 0x110);
    for (int i = 0; i < *(int*)(inst + 0x10C); i++) {
        int bit = 1 << i;
        if (mask & bit) {
            sQ128* occluder = (sQ128*)(*(char**)(inst + 8 + i * 8) + 0x50);
            int result;
            __asm__ __volatile__(
                ".set push\n"
                ".set noreorder\n"
                "lqc2      $vf1, %1\n"
                "lqc2      $vf2, %2\n"
                "lqc2      $vf3, %3\n"
                "lqc2      $vf4, %4\n"
                "lqc2      $vf5, %5\n"
                "lqc2      $vf6, %6\n"
                "ctc2.ni   %7, $vi27\n"
                "vnop\n"
                "vnop\n"
                "vcallmsr  $vi27\n"
                "cfc2.i    %0, $vi1\n"
                ".set pop\n"
                : "=&r"(result)
                : "m"(occluder[0]), "m"(occluder[1]), "m"(occluder[2]), "m"(occluder[3]), "m"(occluder[4]), "m"(*box), "r"((int)D_828 >> 3)
                : "memory");
            int ok = result == 1;
            if (ok) return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D20F8);
#ifdef SKIP_ASM
extern void* D_004A3A00;

extern "C" int func_002D20F8(void) {
    return (*(int*)&D_004A3A00);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2100);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_SSXStatsMan_b[];
extern "C" void* func_002D21C0(void* p);

extern "C" void* func_002D2100(void)
{
    return func_002D21C0(cMemMan_alloc(1, str_SSXStatsMan_b, 0, 0));
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2138);
#ifdef SKIP_ASM
extern "C" void func_002D2138(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2140);
#ifdef SKIP_ASM
extern "C" void func_002D2140(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2148);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

extern "C" long long *func_002D2148(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2160);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

extern "C" long long *func_002D2160(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2178);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

extern "C" long long *func_002D2178(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2190);
#ifdef SKIP_ASM
extern "C" float func_002D2190(void) {
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D21A0);
#ifdef SKIP_ASM
extern "C" float func_002D21A0(void) {
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_update__FPv);
#ifdef SKIP_ASM
/**
 * Per-frame update of the stats manager; does nothing in this build.
 */
void cSSXStatsMan_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_render__FPv);
#ifdef SKIP_ASM
/**
 * Renders the stats manager; does nothing in this build.
 */
void cSSXStatsMan_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D21C0);
#ifdef SKIP_ASM
extern void* D_004A3A00;

extern "C" void* func_002D21C0(void* p)
{
    D_004A3A00 = p;
    return p;
}
#endif


//100%
INCLUDE_ASM("util/objectinterface", func_002D21D0);
#ifdef SKIP_ASM
// PORT: later callers in the unit declare func_002D21D0 as void; bound by asm label with its real int return.
int func_002D21D0_impl(signed char **stream, float v) __asm__("func_002D21D0");

union uFloatBits_002D21D0 {
    float f;
    unsigned int u;
};

int func_002D21D0_impl(signed char **stream, float value)
{
    uFloatBits_002D21D0 floatBits;
    floatBits.f = value;
    unsigned int bits = floatBits.u;
    signed char *cursor = *stream;
    cursor[0] = bits;
    cursor[1] = bits >> 8;
    cursor[2] = bits >> 16;
    cursor[3] = bits >> 24;
    *stream += 4;
    return 4;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2210);
#ifdef SKIP_ASM
extern "C" int func_002D2210(signed char **stream, unsigned int value)
{
    signed char *cursor = *stream;
    cursor[0] = value;
    cursor[1] = value >> 8;
    cursor[2] = value >> 16;
    cursor[3] = value >> 24;
    *stream += 4;
    return 4;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2248);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2248(void* self, float* vec3)
{
    int i;
    for (i = 0; i < 3; i++) {
        func_002D21D0(self, vec3[i]);
    }
    return 0xC;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D22A8);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D22A8(void* self, float* vec4)
{
    int i;
    for (i = 0; i < 4; i++) {
        func_002D21D0(self, vec4[i]);
    }
    return 0x10;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2308);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2308(void* self, float* matrix)
{
    int i;
    for (i = 0; i < 16; i++) {
        func_002D21D0(self, matrix[i]);
    }
    return 0x40;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2368);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2368(void* self, float* quat)
{
    func_002D21D0(self, quat[3]);
    func_002D21D0(self, quat[0]);
    func_002D21D0(self, quat[1]);
    func_002D21D0(self, quat[2]);
    return 0x10;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D23C8);
#ifdef SKIP_ASM
extern "C" int func_002D23C8(signed char **stream, signed char value) {
    **stream = value;
    *stream += 1;
    return 1;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D23E8);
#ifdef SKIP_ASM
extern "C" int func_002D23C8(signed char **arg0, signed char arg1);
extern "C" unsigned int strlen(const char* s);

extern "C" int func_002D23E8(signed char** out, const char* str)
{
    unsigned int i;
    int n = 0;
    for (i = 0; i < strlen(str); i++) {
        n += func_002D23C8(out, str[i]);
    }
    func_002D23C8(out, 0);
    return n + 1;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2478);
#ifdef SKIP_ASM
extern "C" int func_002D2210(signed char** out, unsigned int len);

struct sStr_2478 {
    unsigned int len;
    signed char* data;
};

extern "C" int func_002D2478(signed char** out, sStr_2478** pp)
{
    unsigned int i;
    int n = func_002D2210(out, (*pp)->len);
    for (i = 0; i < (*pp)->len; i++) {
        n += func_002D23C8(out, (*pp)->data[i]);
    }
    return n;
}
#endif
