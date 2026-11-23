#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "util/objectinterface.h"
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

extern "C" void* cAvalanche_getTransform(void* self);

//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_getAvaSplineTransform__FPv);
#ifdef SKIP_ASM
/**
 * Gets an instance's avalanche-spline transform: forwards to cAvalanche_getTransform with @p self and, in the second register,
 * the output.
 *
 * cAvaSplineModifier's update calls it with its Instance and its Transform (+0x40). This definition declares only the
 * first argument.
 *
 * @return cAvalanche_getTransform's result
 */
void* cObjectInterface_getAvaSplineTransform(void* self)
{
    return cAvalanche_getTransform(self);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_renderSimpleLightHalo);
#ifdef SKIP_ASM

static inline void setA_1D10(sDrawState* drawState, int v, int mask, int shift)
{
    drawState->m_bitGroups[1] = (((int)drawState->m_bitGroups[1]) & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void setB_1D10(sDrawState* drawState, int v, int mask, int shift)
{
    drawState->m_bitGroups[2] = (((int)drawState->m_bitGroups[2]) & ~(mask << shift)) | ((v << shift) & (mask << shift));
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


typedef int (*IsHiddenFn_1D10)(void*, void*);
typedef void (*DrawFn_1D10)(void*, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*);
typedef void (*DrawRotFn_1D10)(void*, float, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*, int);

struct sRCtx_1D10 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
    char padE88[0xF50 - 0xE88];
    int texs[0x40];             // 0xF50
    char pad1050[0x10D8 - 0x1050];
    sVEntry<void>* vt;               // 0x10D8
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
extern "C" void cObjectInterface_renderSimpleLightHalo(t4Vector* obj, float* src, int kind, float size, float angle)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    if (((IsHiddenFn_1D10)((sVEntry<void>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan))[91].fn)(vthis(ps2GraphicsMan, ((sVEntry<void>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan))[91]), obj) != 0) {
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
        ((DrawRotFn_1D10)((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[79].fn)(vthis(ctx, ((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[79]), angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        if (kind == 1 || kind == 4) {
            ((DrawRotFn_1D10)((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[79].fn)(vthis(ctx, ((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[79]), -angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        }
    } else {
        ((DrawFn_1D10)((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[78].fn)(vthis(ctx, ((sVEntry<void>*)VPTR_OF(cGraphicsMan, ctx))[78]), obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col);
    }
    (*(sDrawState**)&ctx->m_pCurrentDrawState)--;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_isSphereOccluded);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x828)
extern char D_828[];

struct sQ128 { float v[4]; } __attribute__((aligned(16)));

/**
 * Tells whether @p box is hidden behind one of the active occluders, using a VU0 microprogram.
 *
 * Walks the occluders of SSXApp->Game->OcclusionMan (count at +0x10C, enable mask at +0x110, entry i's pointer at +0x8
 * + 8i): for each enabled one it loads five quadwords of its data (+0x50) and @p box into vf1-vf6, runs the VU0 micro
 * routine at 0x828 (vcallmsr) and reads vi1; a result of 1 means occluded. cMoveNode's render skips drawing when it
 * returns 1.
 *
 * @param box  the bounds to test (one quadword)
 * @return 1 when an occluder hides it, else 0
 */
// PORT: PS2-only VU0 microprogram call (ctc2/vcallmsr/cfc2); the PC port needs a C version.
extern "C" int cObjectInterface_isSphereOccluded(sQ128* box)
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
INCLUDE_ASM("util/objectinterface", SSXStatsMan);
#ifdef SKIP_ASM
extern void* gSSXStatsMan;

/**
 * Returns the stats manager, gSSXStatsMan (set by cSSXStatsMan_cSSXStatsMan).
 *
 * @return the stats manager (On Tour's global gSSXStatsMan)
 */
extern "C" int SSXStatsMan(void) {
    return (*(int*)&gSSXStatsMan);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_SSXStatsMan_b[];
extern "C" void* cSSXStatsMan_cSSXStatsMan(void* p);

/**
 * Creates the stats manager: allocates its one byte with cMemMan_alloc (tag "SSXStatsMan") and constructs it
 * (cSSXStatsMan_cSSXStatsMan, which stores it in gSSXStatsMan).
 *
 * The game load calls it.
 *
 * @return the new stats manager
 */
extern "C" void* cSSXStatsMan_construct(void)
{
    return cSSXStatsMan_cSSXStatsMan(cMemMan_alloc(1, str_SSXStatsMan_b, 0, 0));
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_nextCamera);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty stats-manager camera stepper (the first of two).
 */
extern "C" void cSSXStatsMan_nextCamera(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_prevCamera);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty stats-manager camera stepper (the second of two).
 */
extern "C" void cSSXStatsMan_prevCamera(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_playerPosition);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

/**
 * Returns the zero vector (t4vect_Zero, a quadword copy) through the hidden return pointer @p vec.
 *
 * The first of three identical stats-manager vector stubs (cSSXStatsMan_playerPosition, cSSXStatsMan_playerFacing, cSSXStatsMan_cameraPosition).
 *
 * @param vec  where the vector is returned
 * @return @p vec
 */
extern "C" long long *cSSXStatsMan_playerPosition(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_playerFacing);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

/**
 * Returns the zero vector (t4vect_Zero) through the hidden return pointer @p vec; identical to cSSXStatsMan_playerPosition.
 *
 * @param vec  where the vector is returned
 * @return @p vec
 */
extern "C" long long *cSSXStatsMan_playerFacing(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_cameraPosition);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long t4vect_Zero;

/**
 * Returns the zero vector (t4vect_Zero) through the hidden return pointer @p vec; identical to cSSXStatsMan_playerPosition.
 *
 * @param vec  where the vector is returned
 * @return @p vec
 */
extern "C" long long *cSSXStatsMan_cameraPosition(long long *vec) {
    *vec = t4vect_Zero;
    return vec;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_cameraPitch);
#ifdef SKIP_ASM
/**
 * Returns 0.0 (a stats-manager float stub, the first of two).
 *
 * @return 0.0
 */
extern "C" float cSSXStatsMan_cameraPitch(void) {
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_cameraHeading);
#ifdef SKIP_ASM
/**
 * Returns 0.0 (a stats-manager float stub, the second of two).
 *
 * @return 0.0
 */
extern "C" float cSSXStatsMan_cameraHeading(void) {
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
INCLUDE_ASM("util/objectinterface", cSSXStatsMan_cSSXStatsMan);
#ifdef SKIP_ASM
extern void* gSSXStatsMan;

/**
 * Constructs the stats manager: stores @p statsMan in gSSXStatsMan, the global SSXStatsMan returns.
 *
 * @param statsMan  the object being constructed
 * @return @p statsMan
 */
extern "C" void* cSSXStatsMan_cSSXStatsMan(void* statsMan)
{
    gSSXStatsMan = statsMan;
    return statsMan;
}
#endif


//100%
INCLUDE_ASM("util/objectinterface", put_float);
#ifdef SKIP_ASM
// PORT: later callers in the unit declare put_float as void; bound by asm label with its real int return.
int put_float_impl(signed char **stream, float v) __asm__("put_float");

union uFloatBits_002D21D0 {
    float f;
    unsigned int u;
};

/**
 * Writes @p value to the byte stream @p stream as 4 little-endian bytes of its bit pattern and advances the stream.
 *
 * @param stream  pointer to the write cursor
 * @param value  the float to write
 * @return 4, the bytes written
 */
int put_float_impl(signed char **stream, float value)
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
INCLUDE_ASM("util/objectinterface", put_uint);
#ifdef SKIP_ASM
/**
 * Writes @p value to the byte stream @p stream as 4 little-endian bytes and advances the stream.
 *
 * @param stream  pointer to the write cursor
 * @param value  the value to write
 * @return 4, the bytes written
 */
extern "C" int put_uint(signed char **stream, unsigned int value)
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
INCLUDE_ASM("util/objectinterface", put_t3Vector);
#ifdef SKIP_ASM
extern "C" void put_float(void* self, float v);

/**
 * Writes the 3 floats of @p vec3 to the byte stream @p self (put_float each).
 *
 * @param vec3  three floats
 * @return 12, the bytes written
 */
extern "C" int put_t3Vector(void* self, float* vec3)
{
    int i;
    for (i = 0; i < 3; i++) {
        put_float(self, vec3[i]);
    }
    return 0xC;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", put_t4Vector);
#ifdef SKIP_ASM
extern "C" void put_float(void* self, float v);

/**
 * Writes the 4 floats of @p vec4 to the byte stream @p self (put_float each).
 *
 * @param vec4  four floats
 * @return 16, the bytes written
 */
extern "C" int put_t4Vector(void* self, float* vec4)
{
    int i;
    for (i = 0; i < 4; i++) {
        put_float(self, vec4[i]);
    }
    return 0x10;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", put_t4Matrix);
#ifdef SKIP_ASM
extern "C" void put_float(void* self, float v);

/**
 * Writes the 16 floats of @p matrix to the byte stream @p self (put_float each).
 *
 * @param matrix  a 4x4 float matrix
 * @return 64, the bytes written
 */
extern "C" int put_t4Matrix(void* self, float* matrix)
{
    int i;
    for (i = 0; i < 16; i++) {
        put_float(self, matrix[i]);
    }
    return 0x40;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", put_tQuaternion);
#ifdef SKIP_ASM
extern "C" void put_float(void* self, float v);

/**
 * Writes the 4 floats of @p quat to the byte stream @p self, element 3 first, then elements 0, 1 and 2 (put_float
 * each).
 *
 * @param quat  four floats
 * @return 16, the bytes written
 */
extern "C" int put_tQuaternion(void* self, float* quat)
{
    put_float(self, quat[3]);
    put_float(self, quat[0]);
    put_float(self, quat[1]);
    put_float(self, quat[2]);
    return 0x10;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", put_char);
#ifdef SKIP_ASM
/**
 * Writes the byte @p value to the byte stream @p stream and advances it by one.
 *
 * @param stream  pointer to the write cursor
 * @param value  the byte
 * @return 1
 */
extern "C" int put_char(signed char **stream, signed char value) {
    **stream = value;
    *stream += 1;
    return 1;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", put_string);
#ifdef SKIP_ASM
extern "C" int put_char(signed char **arg0, signed char arg1);
extern "C" unsigned int strlen(const char* s);

/**
 * Writes the string @p str and its terminating 0 to the byte stream @p out (put_char per byte; strlen is
 * re-evaluated every iteration).
 *
 * @param out  pointer to the write cursor
 * @param str  the string
 * @return the bytes written, strlen + 1
 */
extern "C" int put_string(signed char** out, const char* str)
{
    unsigned int i;
    int n = 0;
    for (i = 0; i < strlen(str); i++) {
        n += put_char(out, str[i]);
    }
    put_char(out, 0);
    return n + 1;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", put_data);
#ifdef SKIP_ASM
extern "C" int put_uint(signed char** out, unsigned int len);


/**
 * Writes a sized data block to the byte stream @p out: the block's byte count (put_uint), then that many bytes of
 * its data (put_char).
 *
 * @param out  pointer to the write cursor
 * @param pp  reference to the block pointer (a count, then a byte pointer)
 * @return the bytes written, 4 + count
 */
extern "C" int put_data(signed char** out, tCommDataBlock** pp)
{
    unsigned int i;
    int n = put_uint(out, (*pp)->Size);
    for (i = 0; i < (*pp)->Size; i++) {
        n += put_char(out, ((signed char*)(*pp)->Data)[i]);
    }
    return n;
}
#endif
