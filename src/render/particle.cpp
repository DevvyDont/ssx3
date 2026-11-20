#include "common.h"
#include "render/irradiance.h"
#include "render/playershadow.h"
#include "render/particle.h"
#include "render/ps2bezierman.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_Allocate);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the array new-expressions below match.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_DynEmitterData_Positions[];
extern char str_DynEmitterData_Velocities[];

struct sEmitParticle_0C60 {
    float v[4];
    sEmitParticle_0C60() {}
};

struct sEmitState_0C60 {
    float v[4];
    sEmitState_0C60() {}
};

/**
 * Allocates a dynamic emitter's position and velocity arrays, one 16-byte vector per sub-emitter.
 *
 * The count is the word at +0x178; positions go to +0x1A0 ("DynEmitterData Positions") and velocities to
 * +0x1A4 ("DynEmitterData Velocities").
 */
extern "C" void cBaseClass_DynamicEmitter_Allocate(cBaseClass_DynamicEmitter* self)
{
    sEmitParticle_0C60** pp = (sEmitParticle_0C60**)(((char*)self) + 0x1A0);
    *pp = new (str_DynEmitterData_Positions, 0, 0) sEmitParticle_0C60[self->m_numSubEmitters];
    sEmitState_0C60** ps = (sEmitState_0C60**)(((char*)self) + 0x1A4);
    *ps = new (str_DynEmitterData_Velocities, 0, 0) sEmitState_0C60[self->m_numSubEmitters];
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Frees the sub-emitter position and velocity buffers (m_positions, m_velocities); with @p reset also clears
 * m_initialized and sets m_numSubEmitters to -1.
 *
 * @param reset  non-zero to reset the emitter as well (On Tour resetEmitter)
 */
extern "C" void cBaseClass_DynamicEmitter_purge(cBaseClass_DynamicEmitter* self, int reset)
{
    if (self->m_positions != 0) {
        cMemMan_free(self->m_positions);
        self->m_positions = 0;
    }
    if (self->m_velocities != 0) {
        cMemMan_free(self->m_velocities);
        self->m_velocities = 0;
    }
    if (reset) {
        self->m_initialized = 0;
        self->m_numSubEmitters = -1;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_reset);
#ifdef SKIP_ASM
struct sVec4A {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4A t4vect_Zero;

/**
 * Resets a dynamic emitter: zeroes both of its vector arrays and its counters.
 *
 * For each of the +0x178 entries, the vectors at +0x1A0 and +0x1A4 are set to zero; the words at +0x17C and
 * +0x1E0 are cleared.
 */
extern "C" void cBaseClass_DynamicEmitter_reset(cBaseClass_DynamicEmitter* self)
{
    int i;
    self->m_subEmitterId = 0;
    for (i = 0; i < self->m_numSubEmitters; i++) {
        ((sVec4A*)self->m_positions)[i] = t4vect_Zero;
        ((sVec4A*)self->m_velocities)[i] = t4vect_Zero;
        self->m_subEmitterId = 0;
        self->m_enabledCount = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_setupEmitter);
#ifdef SKIP_ASM
int BXrand();

extern "C" void tParticleEmitter_init(void* sys, int count, int a2, sVec4A* v, float a, float b, float c);
extern "C" void tParticleEmitter_setupEmitter(void* sys, void* a1, void* def, void* a3, float f);
extern char t4mat_Identity[];

struct sGsState_00370DC8 {
    int w0;
    unsigned int a : 2;
    unsigned int pad2 : 18;
    unsigned int b : 2;
    unsigned int c : 1;
    unsigned int pad23 : 9;
    unsigned int pad0_ : 10;
    unsigned int d : 19;
    unsigned int pad29 : 3;
    int w3;
    int w4;
};

extern sGsState_00370DC8 sDrawState_s_defaultDrawState;


class cEmitter_00370DC8 {
public:
    float start;            // 0x0
    int f4;                 // 0x4
    int f8;                 // 0x8
    int fC;                 // 0xC
    float f10;              // 0x10
    float f14;              // 0x14
    char pad18[0x8];
    char sys[0x150];        // 0x20
    int active;             // 0x170
    int f174;               // 0x174
    int count;              // 0x178
    int f17C;               // 0x17C
    char f180[0x30];        // 0x180
    char v1B0[0x10];        // 0x1B0
    char v1C0[0x10];        // 0x1C0
    char v1D0[0x10];        // 0x1D0
    int f1E0;               // 0x1E0
    sGsState_00370DC8 gs;   // 0x1E4
    virtual void v01(int a);
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();

    void setA(int v) { gs.a = v; }
    void setB(int v) { gs.b = v; }
    void setC(int v) { gs.c = v; }
    void setD(int v) { gs.d = v; }
};

static inline float randf_00370DC8()
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float fceil_00370DC8(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (t < x) {
        t += 1.0f;
    }
    return t;
}

static inline float life_00370DC8(char* def)
{
    float t = *(float*)(def + 0x8);
    if (t >= 0.0f) {
        return t;
    }
    return *(float*)(def + 0x14) + *(float*)(def + 0x1C) * 0.5f;
}

static inline float start_00370DC8(char* def)
{
    float t = *(float*)(def + 0x8);
    if (t < 0.0f) {
        return t;
    }
    return t + *(float*)(def + 0x14) + *(float*)(def + 0x1C) * 0.5f;
}

/**
 * Configures a dynamic emitter from particle data: sub-emitter count, timing, ring size, the particle system and its
 * draw state.
 *
 * Copies the flip-texture count (+0xD0 of the data, to +0xC, On Tour mNumFlipTextures) and rate (+0xD4, to +0x14); with
 * two or more flip textures the current one (+0x10) starts at count * a BXrand fraction, else 0. +0x4/+0x8 take the
 * data's +0xC4/+0xC8 (On Tour TextureId, BlendMode). Duration (+0x0) and the emission life come from the data's +0x8,
 * +0x14 and +0x1C. The sub-emitter count (+0x178, On Tour m_numSubEmitters) is ceil(life * 60); when it changes the
 * emitter rebuilds its buffers through vtable slots 1 (with 0), 5 and 2 (On Tour: rebuild). Zeroes the three vectors
 * +0x1B0..+0x1D0, then tParticleEmitter_init on the embedded system (+0x20) with data[0] particles per sub-emitter and
 * tParticleEmitter_setupEmitter with t4mat_Identity and the box at +0x180. Marks it active (+0x170) and copies
 * sDrawState_s_defaultDrawState to +0x1E4 with four bitfields overridden.
 *
 * @param def  the particle data (const tParticleData*)
 * @param seed    the random seed passed on to tParticleEmitter_setupEmitter (-1.0 from the effect modifiers)
 */
extern "C" void cBaseClass_DynamicEmitter_setupEmitter(cEmitter_00370DC8* self, char* def, float seed)
{
    int n = *(int*)(def + 0xD0);
    self->fC = n;
    self->f14 = *(float*)(def + 0xD4);
    if (n >= 2) {
        float fn = (float)n;
        self->f10 = fn * randf_00370DC8();
    } else {
        self->f10 = 0.0f;
    }
    self->f4 = *(int*)(def + 0xC4);
    self->f8 = *(int*)(def + 0xC8);
    self->start = start_00370DC8(def);
    int changed = 0;
    float frames = life_00370DC8(def) * 60.0f;
    if (self->count != (int)fceil_00370DC8(frames)) {
        self->count = (int)fceil_00370DC8(frames);
        changed = 1;
    }
    *(sVec4A*)self->v1B0 = t4vect_Zero;
    *(sVec4A*)self->v1C0 = t4vect_Zero;
    *(sVec4A*)self->v1D0 = t4vect_Zero;
    float life = life_00370DC8(def);
    sVec4A force;
    force.x = *(float*)(def + 0x78);
    force.y = *(float*)(def + 0x7C);
    force.z = *(float*)(def + 0x80);
    force.w = 0.0f;
    tParticleEmitter_init(self->sys, *(int*)(def + 0x0) * self->count, *(int*)(def + 0x4), &force, life,
                  *(float*)(def + 0x20), *(float*)(def + 0xC));
    tParticleEmitter_setupEmitter(self->sys, t4mat_Identity, def, self->f180, seed);
    if (changed) {
        self->v01(0);
        self->v05();
        self->v02();
    }
    self->active = 1;
    self->gs = sDrawState_s_defaultDrawState;
    self->setC(1);
    self->setB(0);
    self->setA(2);
    self->setD(0);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_updateBASE);
#ifdef SKIP_ASM
struct sPtQ10D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

static inline float randUnit10D0()
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

// PORT: the unit's callers use a 4-arg declaration; bind the real 5-arg body by asm label.
extern "C" void cBaseClass_DynamicEmitter_updateBASE_impl(char* self, sPtQ10D0* pos, sPtQ10D0* vel, int rnd, float dt) __asm__("cBaseClass_DynamicEmitter_updateBASE");
/**
 * Records a dynamic emitter's new sub-emitter position and velocity for this frame and advances its ring index and
 * texture flip frame.
 *
 * Returns at once unless m_enabled. m_enabledCount goes down when the entry at m_subEmitterId was enabled (velocity .w
 * > 0) and @p rnd is 0, and up in the opposite case. @p pos and @p vel, when given, replace m_currPosition and
 * m_currVelocity (else those are used). The entry m_positions[m_subEmitterId] gets the position with w = 0 and
 * m_velocities[m_subEmitterId] the velocity with w = 1 + a random value in [0, 1) when @p rnd is set, else 0.
 * m_subEmitterId then steps down, wrapping to m_numSubEmitters - 1, and mCurrentFlipTexture advances by
 * mFlipTextureRate * @p dt, back to 0 at mNumFlipTextures.
 *
 * Callers: cDynamicColourEmitter_update, cDynamicParticleModifier_update and func_002F1A08.
 *
 * @param pos  the new position, or null to reuse m_currPosition
 * @param vel  the new velocity, or null to reuse m_currVelocity
 * @param rnd  non-zero when the sub-emitter is enabled this frame
 * @param dt  the frame time
 */
extern "C" void cBaseClass_DynamicEmitter_updateBASE_impl(char* self, sPtQ10D0* pos, sPtQ10D0* vel, int rnd, float dt)
{
    if (*(int*)(self + 0x174) == 0) {
        return;
    }
    int alive = (*(sPtQ10D0**)(self + 0x1A4))[*(int*)(self + 0x17C)].w > 0.0f;
    if (alive) {
        if (rnd == 0) {
            *(int*)(self + 0x1E0) = *(int*)(self + 0x1E0) - 1;
        }
    } else if (rnd) {
        *(int*)(self + 0x1E0) = *(int*)(self + 0x1E0) + 1;
    }
    if (pos) {
        *(sPtQ10D0*)(self + 0x1B0) = *pos;
    } else {
        pos = (sPtQ10D0*)(self + 0x1B0);
    }
    if (vel) {
        *(sPtQ10D0*)(self + 0x1C0) = *vel;
    } else {
        vel = (sPtQ10D0*)(self + 0x1C0);
    }
    (*(sPtQ10D0**)(self + 0x1A0))[*(int*)(self + 0x17C)] = *pos;
    (*(sPtQ10D0**)(self + 0x1A0))[*(int*)(self + 0x17C)].w = 0.0f;
    (*(sPtQ10D0**)(self + 0x1A4))[*(int*)(self + 0x17C)] = *vel;
    // PORT: pointer held in int (index-first addu)
    sPtQ10D0* velSlot = (sPtQ10D0*)((*(int*)(self + 0x17C) << 4) + *(int*)(self + 0x1A4));
    if (rnd) {
        velSlot->w = randUnit10D0() + 1.0f;
    } else {
        velSlot->w = 0.0f;
    }
    int idx = *(int*)(self + 0x17C) - 1;
    *(int*)(self + 0x17C) = idx;
    if (idx < 0) {
        *(int*)(self + 0x17C) = *(int*)(self + 0x178) - 1;
    }
    float frameTime = *(float*)(self + 0x10) + *(float*)(self + 0x14) * dt;
    *(float*)(self + 0x10) = frameTime;
    if ((int)frameTime >= *(int*)(self + 0xC)) {
        *(float*)(self + 0x10) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_setTime);
#ifdef SKIP_ASM
extern "C" void tParticleEmitter_setTime(void* p, float v);

/**
 * Sets the dynamic emitter's particle time.
 *
 * For a non-zero @p duration it stores it in the first word and calls tParticleEmitter_setTime() on the base emitter at
 * +0x20 with it. For 0 it only calls tParticleEmitter_setTime(.., 0) and leaves the stored duration unchanged.
 *
 * @param duration  the time
 */
extern "C" void cBaseClass_DynamicEmitter_setTime(cBaseClass_DynamicEmitter* self, float duration)
{
    if (duration == 0.0f) {
        tParticleEmitter_setTime(((char*)self) + 0x20, 0.0f);
        return;
    }
    *(float*)self = duration;
    tParticleEmitter_setTime(((char*)self) + 0x20, duration);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream003712B8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

/**
 * Saves a dynamic emitter to a replay frame: writes 0x14 bytes (Duration, the emitter's EndTime, m_enabled,
 * m_initialized, m_subEmitterId) with the stream's slot 1.
 *
 * @param stream  the replay frame to write to (On Tour data)
 */
extern "C" void cBaseClass_DynamicEmitter_writeToReplayFrame(cBaseClass_DynamicEmitter* self, cStream003712B8* stream)
{
    struct {
        float a;
        float b;
        int c;
        int d;
        int e;
    } buf;
    buf.a = self->Duration;
    buf.b = self->m_emitter.EndTime;
    buf.d = self->m_initialized;
    buf.c = self->m_enabled;
    buf.e = self->m_subEmitterId;
    stream->v01(&buf, 20);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream00371318 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

/**
 * Restores a dynamic emitter from a replay frame: reads the 0x14 bytes written by writeToReplayFrame (Duration,
 * EndTime, m_enabled, m_initialized, m_subEmitterId) with the stream's slot 2.
 *
 * @param stream  the replay frame to read from (On Tour data)
 */
extern "C" void cBaseClass_DynamicEmitter_readFromReplayFrame(cBaseClass_DynamicEmitter* self, cStream00371318* stream)
{
    struct {
        float a;
        float b;
        int c;
        int d;
        int e;
    } buf;
    stream->v02(&buf, 20);
    self->Duration = buf.a;
    self->m_emitter.EndTime = buf.b;
    self->m_initialized = buf.d;
    self->m_enabled = buf.c;
    self->m_subEmitterId = buf.e;
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicEmitter_render);
#ifdef SKIP_ASM

struct sPtVEntry1380 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sPtCtx1380 {
    char pad_0x0[0xE84];
    sDrawState* top;             // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sPtVEntry1380* vtable;      // 0x10D8
};

extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern sPtCtx1380* D_004A5B80_ctx __asm__("cGraphicsMan_TheGraphicsMan");
extern int D_0044B420[];


/**
 * Draws a dynamic emitter's particles with its current texture frame and draw state.
 *
 * Returns unless the words at +0x174 and +0x1E0 are set and the graphics manager's effect texture
 * _EffectTexMap[base (+0x4) + (int)frame (+0x10)] is valid. Sets bits 2-6 of the emitter's draw state
 * (+0x1E4) word 1 from its kind (+0x8) through D_0044B420 {7, 5, 9, 0}, bits 5-9 of word 2 from @p blend, and
 * the texture id; pushes that state on the graphics manager's draw-state stack, draws through its virtual
 * slot 83 (cPS2GraphicsMan_RenderDynamicParticleEmitter) and pops the state.
 *
 * @param blend  value for bits 5-9 of the draw state's word 2 (On Tour: drawLayer, an eGMDrawLayer)
 */
extern "C" void cDynamicEmitter_render(cDynamicEmitter* self, int blend)
{
    if (self->m_enabled == 0) {
        return;
    }
    sPtCtx1380* ctx = D_004A5B80_ctx;
    if (self->m_enabledCount == 0) {
        return;
    }
    int tex = ctx->texIds[self->TextureId + (int)self->mCurrentFlipTexture];
    if (tex < 0) {
        return;
    }
    self->mDrawState.unk_00[1] = (unsigned int)(((int)self->mDrawState.unk_00[1] & ~0x7C) | ((D_0044B420[self->BlendMode] << 2) & 0x7C));
    self->mDrawState.unk_00[2] = (unsigned int)(((int)self->mDrawState.unk_00[2] & ~0x3E0) | ((blend << 5) & 0x3E0));
    self->mDrawState.m_textureState.m_texid[0] = tex;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = (*(&self->mDrawState));
    sPtVEntry1380* vt = ctx->vtable;
    vt[83].fn((char*)ctx + vt[83].delta, (self));
    ctx->top--;
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_cDynamicColourEmitter);
#ifdef SKIP_ASM
extern "C" void* cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(void* self);
extern void* cDynamicColourEmitter_vtbl[];

/**
 * Constructs a dynamic colour emitter: clears m_IsAvalanche and m_colours.
 *
 * @return @p self
 */
extern "C" void* cDynamicColourEmitter_cDynamicColourEmitter(cDynamicColourEmitter* self)
{
    cBaseClass_DynamicEmitter_cBaseClass_DynamicEmitter(self);
    self->mVtbl = cDynamicColourEmitter_vtbl;
    self->m_IsAvalanche = 0;
    self->m_colours = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter__cDynamicColourEmitter);
#ifdef SKIP_ASM
extern void* cDynamicColourEmitter_vtbl[];
extern "C" void cDynamicColourEmitter_purge(cDynamicColourEmitter* self, int a1);
extern "C" void cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(void* self, int flags);

/**
 * Destroys a dynamic colour emitter: purges its particles, then runs the base emitter destructor.
 *
 * Stores cDynamicColourEmitter's vtable at +0x1F8 and calls cDynamicColourEmitter_purge(self, 0) first.
 *
 * @param flags  g++ destructor flags, passed to cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter()
 */
extern "C" void cDynamicColourEmitter__cDynamicColourEmitter(cDynamicColourEmitter* self, int flags)
{
    self->mVtbl = cDynamicColourEmitter_vtbl;
    cDynamicColourEmitter_purge(self, 0);
    cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(self, flags);
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_Allocate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_DynEmitterData_Colours[];

struct sEmitColour_1548 {
    unsigned char rgba[4];
    sEmitColour_1548() {}
};

/**
 * Allocates a colour emitter's arrays: the base positions and velocities plus one RGBA colour each.
 *
 * The colours (4 bytes per sub-emitter, count at +0x178) go to +0x204, tagged "DynEmitterData Colours".
 */
extern "C" void cDynamicColourEmitter_Allocate(cDynamicColourEmitter* self)
{
    sEmitColour_1548** pc = (sEmitColour_1548**)(((char*)self) + 0x204);
    cBaseClass_DynamicEmitter_Allocate((cBaseClass_DynamicEmitter*)self);
    *pc = new (str_DynEmitterData_Colours, 0, 0) sEmitColour_1548[self->m_numSubEmitters];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cDynamicColourEmitter_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void cBaseClass_DynamicEmitter_purge(cBaseClass_DynamicEmitter* self, int a1);

/**
 * Frees a colour emitter's colour array, then its base arrays.
 *
 * The base purge (cBaseClass_DynamicEmitter_purge) frees the position and velocity arrays and, when @p resetEmitter is set,
 * zeroes +0x170 and sets the count at +0x178 to -1.
 *
 * @param resetEmitter  nonzero to also reset the emitter's count and state word
 */
extern "C" void cDynamicColourEmitter_purge(cDynamicColourEmitter* self, int resetEmitter)
{
    void* colours = self->m_colours;
    if (colours != 0) {
        cMemMan_free(colours);
        self->m_colours = 0;
    }
    cBaseClass_DynamicEmitter_purge((cBaseClass_DynamicEmitter*)self, resetEmitter);
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_reset);
#ifdef SKIP_ASM
struct sRGBA_1600 {
    unsigned char r, g, b, a;
};

// Function-local static `sRGBA black(0,0,0,0)` and its init guard, spelled out
// as the globals splat named.
extern unsigned char D_004A5998;
extern unsigned char D_004A5999;
extern unsigned char D_004A599A;
extern unsigned char D_004A599B;
extern int D_004A599C;

/**
 * Resets a colour emitter: runs the base reset, then sets every colour to transparent black.
 *
 * The (0, 0, 0, 0) colour is a function-local static, constructed on first use.
 */
extern "C" void cDynamicColourEmitter_reset(cDynamicColourEmitter* self)
{
    int i;
    cBaseClass_DynamicEmitter_reset((cBaseClass_DynamicEmitter*)self);
    for (i = 0; i < self->m_numSubEmitters; i++) {
        if (D_004A599C == 0) {
            D_004A5998 = 0;
            D_004A5999 = 0;
            D_004A599A = 0;
            D_004A599B = 0;
            D_004A599C = 1;
        }
        ((sRGBA_1600*)self->m_colours)[i] = *(sRGBA_1600*)&D_004A5998;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_render);
#ifdef SKIP_ASM

struct sPtVEntry1688 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sPtCtx1688 {
    char pad_0x0[0xE84];
    sDrawState* top;             // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sPtVEntry1688* vtable;      // 0x10D8
};

extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern sPtCtx1688* D_004A5B80_ctx1688 __asm__("cGraphicsMan_TheGraphicsMan");
extern int D_0044B420[];


/**
 * Draws the colour emitter's sub-emitters.
 *
 * Returns when the emitter is disabled, has no enabled sub-emitters, or its texture (TextureId + mCurrentFlipTexture)
 * is not loaded. Otherwise it writes the blend mode (D_0044B420[BlendMode]) and @p blend (bits 5-9 of draw-state word
 * 2) into mDrawState, sets its texture, pushes it on the graphics manager's draw-state stack, calls GrMan slot 84 with
 * the emitter and pops the state.
 *
 * @param blend  the draw layer (On Tour drawLayer)
 */
extern "C" void cDynamicColourEmitter_render(cDynamicColourEmitter* self, int blend)
{
    if (self->m_enabled == 0) {
        return;
    }
    sPtCtx1688* ctx = D_004A5B80_ctx1688;
    if (self->m_enabledCount == 0) {
        return;
    }
    int tex = ctx->texIds[self->TextureId + (int)self->mCurrentFlipTexture];
    if (tex < 0) {
        return;
    }
    self->mDrawState.unk_00[1] = (unsigned int)(((int)self->mDrawState.unk_00[1] & ~0x7C) | ((D_0044B420[self->BlendMode] << 2) & 0x7C));
    self->mDrawState.unk_00[2] = (unsigned int)(((int)self->mDrawState.unk_00[2] & ~0x3E0) | ((blend << 5) & 0x3E0));
    self->mDrawState.m_textureState.m_texid[0] = tex;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = (*(&self->mDrawState));
    sPtVEntry1688* vt = ctx->vtable;
    vt[84].fn((char*)ctx + vt[84].delta, (self));
    ctx->top--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cDynamicColourEmitter_update);
#ifdef SKIP_ASM
struct sPtCol17C0 {
    float r, g, b, a;
} __attribute__((aligned(16)));

struct sPtRGBA17C0 {
    unsigned char r, g, b, a;
};

struct sPtObj17C0 {
    char pad_0x0[0x17C];
    int idx;                    // 0x17C
    char pad_0x180[0x50];
    sPtCol17C0 col;             // 0x1D0
    char pad_0x1E0[0x24];
    sPtRGBA17C0* colors;        // 0x204
};

extern "C" void cBaseClass_DynamicEmitter_updateBASE(void* self, int a1, int a2, int a3);

// PORT: g++ minimum operator (<?).
static inline float clamp17C0(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Feeds a dynamic colour emitter one frame: records the colour for the current sub-emitter, then updates position and
 * velocity.
 *
 * When @p col is given it becomes the emitter's current colour (+0x1D0), otherwise the stored one is used; its
 * components times 255, clamped to 0..255, are written as RGBA bytes to m_colours (+0x204) at the current sub-emitter
 * index (+0x17C). Then cBaseClass_DynamicEmitter_updateBASE (On Tour updateBASE) gets (self, @p position, @p velocity, @p enable) with the frame time still in
 * $f12: the C declaration omits On Tour's float dt, which every caller passes (1/60 or dt).
 *
 * @param position   the emitter position (const t4Vector*)
 * @param velocity   the emitter velocity (const t4Vector*, may be 0)
 * @param col  the colour (r, g, b, a floats) or 0 to keep the previous one
 * @param enable   nonzero to emit this frame (On Tour: bool enable)
 */
extern "C" void cDynamicColourEmitter_update(sPtObj17C0* self, int position, int velocity, sPtCol17C0* col, int enable)
{
    if (col != 0) {
        self->col = *col;
    } else {
        col = &self->col;
    }
    self->colors[self->idx].a = (int)clamp17C0(col->a * 255.0f, 0.0f, 255.0f);
    self->colors[self->idx].r = (int)clamp17C0(col->r * 255.0f, 0.0f, 255.0f);
    self->colors[self->idx].g = (int)clamp17C0(col->g * 255.0f, 0.0f, 255.0f);
    self->colors[self->idx].b = (int)clamp17C0(col->b * 255.0f, 0.0f, 255.0f);
    cBaseClass_DynamicEmitter_updateBASE(self, position, velocity, enable);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003718E8);
#ifdef SKIP_ASM
/**
 * Waits until the GIF DMA channel (D2, CHCR 0x1000A000) is idle, then writes 4 to 0x10003010.
 *
 * 0x10003010 is GIF_MODE (4 sets IMT, intermittent mode). No caller.
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel CHCR, 0x10003010 = GIF_MODE).
extern "C" void func_003718E8(void)
{
    while (*(volatile int*)0x1000A000 & 0x100) {
    }
    *(volatile int*)0x10003010 = 4;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371940);
#ifdef SKIP_ASM
struct sGifQuad {
    unsigned int w[4];
} __attribute__((aligned(16)));

extern sGifQuad D_0044B430;

/**
 * Starts a normal-mode GIF DMA (D2) of @p qwc quadwords from @p madr.
 *
 * Writes the quadword D_0044B430 to the VIF1 FIFO (0x10005000) and 4 (intermittent mode) to GIF_MODE first. An address
 * above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad form. CHCR
 * 0x101 (start, from memory). @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the
 * transfer to finish. Callers: func_00382AF0 and cPS2LensFxMan_sampleZBuffer.
 *
 * @param madr  the source address
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel, VIF1 FIFO, GIF_MODE).
extern "C" void func_00371940(unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
    *(sGifQuad*)0x10005000 = D_0044B430;
    *(volatile int*)0x10003010 = 4;
    if (madr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x1000A010 = (madr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x1000A010 = madr;
    }
    *(volatile int*)0x1000A020 = qwc;
    *(volatile int*)0x1000A000 = 257;
    if (flags & 2) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371A58);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode VIF0 DMA (D0, 0x10008000) of @p qwc quadwords at @p madr with CHCR 0x100.
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer to finish. No
 * caller.
 *
 * @param madr  the memory address
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D0 = VIF0 DMA channel).
extern "C" void func_00371A58(unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x10008000 & 0x100) {
        }
    }
    if (madr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x10008010 = (madr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x10008010 = madr;
    }
    *(volatile int*)0x10008020 = qwc;
    *(volatile int*)0x10008000 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x10008000 & 0x100) {
        }
    }
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00371B40);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode VIF1 DMA (D1, 0x10009000) of @p qwc quadwords from @p madr (CHCR 0x101, from memory).
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer to finish. No
 * caller.
 *
 * @param madr  the source address
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D1 = VIF1 DMA channel).
extern "C" void func_00371B40(unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
    if (madr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x10009010 = (madr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x10009010 = madr;
    }
    *(volatile int*)0x10009020 = qwc;
    *(volatile int*)0x10009000 = 257;
    if (flags & 2) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00371C28);
#ifdef SKIP_ASM
/**
 * Starts a normal-mode VIF1 DMA (D1, 0x10009000) of @p qwc quadwords to @p madr (CHCR 0x100, direction bit clear: to
 * memory).
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer to finish. No
 * caller.
 *
 * @param madr  the memory address
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D1 = VIF1 DMA channel).
extern "C" void func_00371C28(unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
    if (madr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x10009010 = (madr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x10009010 = madr;
    }
    *(volatile int*)0x10009020 = qwc;
    *(volatile int*)0x10009000 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371D10);
#ifdef SKIP_ASM
/**
 * Copies @p qwc quadwords from main memory @p madr to scratchpad offset @p sadr by toSPR DMA (D9, 0x1000D400).
 *
 * SADR is @p sadr & 0x3FFF. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer
 * to finish. Callers: func_0038F4F8 and func_0038F708.
 *
 * @param madr  the main-memory source
 * @param sadr  the scratchpad destination
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
extern "C" void func_00371D10(unsigned int madr, int sadr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000D400 & 0x100) {
        }
    }
    *(volatile int*)0x1000D410 = madr;
    *(volatile int*)0x1000D480 = sadr & 0x3FFF;
    *(volatile int*)0x1000D420 = qwc;
    *(volatile int*)0x1000D400 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x1000D400 & 0x100) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371DD8);
#ifdef SKIP_ASM
/**
 * Copies @p qwc quadwords from scratchpad offset @p sadr to main memory @p madr by fromSPR DMA (D8, 0x1000D000).
 *
 * SADR is @p sadr & 0x3FFF. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer
 * to finish. Callers: func_0038F598 and func_0038F738.
 *
 * @param sadr  the scratchpad source
 * @param madr  the main-memory destination
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
extern "C" void func_00371DD8(int sadr, unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000D000 & 0x100) {
        }
    }
    *(volatile int*)0x1000D010 = madr;
    *(volatile int*)0x1000D080 = sadr & 0x3FFF;
    *(volatile int*)0x1000D020 = qwc;
    *(volatile int*)0x1000D000 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x1000D000 & 0x100) {
        }
    }
}
#endif

extern "C" void* cSimpleTube_updateColours(void* self);

//100%
INCLUDE_ASM("render/particle", func_00371EA0);
#ifdef SKIP_ASM
/**
 * Starts a chain-mode VIF1 DMA (D1) from the tag at @p tadr.
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. QWC is zeroed; CHCR is 0x145 (chain, tag transfer) when @p flags & 1 is set, else 0x105. @p flags & 4 waits for
 * the channel to be idle first and @p flags & 2 waits for the transfer to finish. No caller.
 *
 * @param tadr  the first DMA tag
 * @param flags  1: transfer tags; 4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D1 = VIF1 DMA channel, chain mode via TADR).
extern "C" void func_00371EA0(unsigned int tadr, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
    if (tadr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x10009030 = (tadr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x10009030 = tadr;
    }
    *(volatile int*)0x10009020 = 0;
    if (flags & 1) {
        *(volatile int*)0x10009000 = 325;
    } else {
        *(volatile int*)0x10009000 = 261;
    }
    if (flags & 2) {
        while (*(volatile int*)0x10009000 & 0x100) {
        }
    }
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00371F98);
#ifdef SKIP_ASM
/**
 * Starts a chain-mode GIF DMA (D2) from the tag at @p tadr after setting GIF_MODE to 4.
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. CHCR is 0x144 (chain, tag transfer) when @p flags & 1 is set, else 0x104; unlike func_00371EA0 it leaves QWC
 * alone. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer to finish. Caller:
 * func_00383808.
 *
 * @param tadr  the first DMA tag
 * @param flags  1: transfer tags; 4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel, chain mode via TADR; GIF_MODE).
extern "C" void func_00371F98(unsigned int tadr, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
    *(volatile int*)0x10003010 = 4;
    if (tadr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x1000A030 = (tadr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x1000A030 = tadr;
    }
    if (flags & 1) {
        *(volatile int*)0x1000A000 = 324;
    } else {
        *(volatile int*)0x1000A000 = 260;
    }
    if (flags & 2) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00372098);
#ifdef SKIP_ASM
/**
 * Turns DMA on or off: sets or clears bit 0 of D_CTRL (0x1000E000).
 *
 * When enabling it also reads D_PCR (0x1000E020) and writes 1 to it. No caller.
 *
 * @param enable  non-zero to enable
 */
// PORT: PS2 hardware registers (0x1000E000 = D_CTRL, 0x1000E020 = D_PCR).
extern "C" void func_00372098(int enable)
{
    if (enable != 0) {
        *(volatile unsigned int*)0x1000E000 |= 1;
        *(volatile int*)0x1000E020;
        *(volatile int*)0x1000E020 = 1;
        return;
    }
    *(volatile unsigned int*)0x1000E000 &= 0xFFFFFFFE;
}
#endif
//100%
INCLUDE_ASM("render/particle", func_003720F0);
#ifdef SKIP_ASM
/**
 * Walks a DMA chain and rebases every ref tag's address onto @p chain, stopping after the ret tag.
 *
 * Tag ids: 1 (cnt) and 6 (ret) carry QWC quadwords of data, 3 (ref) points elsewhere. Likely bug: ids 0, 2, 4, 5 and 7
 * do not advance the walk, so a chain containing one never ends. No caller.
 *
 * @param chain  the first tag of the chain
 * @return the address just past the ret tag's data
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Walks a DMA chain, rebasing REF tag addresses onto the chain start; returns the address past the RET tag.
extern "C" unsigned long* func_003720F0(unsigned long* chain)
{
    unsigned long* tagPtr = chain;
    int done = 0;

    do {
        unsigned long tag = *tagPtr;
        int id = (int)(tag >> 28) & 7;
        int addr = (long)tag >> 32;
        int qwc = (int)tag & 0xFFFF;

        switch (id) {
        case 0:
        case 2:
        case 4:
        case 5:
        case 7:
            break;
        case 1:
            tagPtr += 2;
            tagPtr += qwc * 2;
            break;
        case 3:
            *tagPtr = ((unsigned long)(unsigned int)((char*)chain + addr) << 32) | (*tagPtr & 0xFFFFFFFF);
            tagPtr += 2;
            break;
        case 6:
            tagPtr += 2;
            tagPtr += qwc * 2;
            done = 1;
            break;
        }
    } while (done == 0);
    return tagPtr;
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00372198);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.

extern "C" void* MEM_copy(void*, void*, int);

/**
 * Flattens a DMA chain in place into one block: copies the cnt payloads after the first quadword, replaces selected ref
 * tags, and writes the total size into the first tag.
 *
 * Ref tags pointing at @p addrA or @p addrB become a zero quadword; one pointing at @p addrC becomes the quadword
 * 0x3000000005000001 (low dword first). The ret tag and its data are copied last. The first tag keeps bits 16-31 of its
 * low word, gets the copied quadword count as QWC, and loses its address word. Tag ids: 1 (cnt) and 6 (ret) carry QWC
 * quadwords of data, 3 (ref) points elsewhere. Likely bug: ids 0, 2, 4, 5 and 7 do not advance the walk, so a chain
 * containing one never ends. A ref tag whose address matches none of the three leaves the walk stuck the same way. No
 * caller.
 *
 * @param buf  the chain, rewritten in place
 * @param addrA  a ref address to blank
 * @param addrB  a ref address to blank
 * @param addrC  the ref address to replace with the fixed quadword
 */
extern "C" void func_00372198(char* buf, int addrA, int addrB, int addrC)
{
    char* dst = buf + 0x10;
    char* src = buf;
    int count = 0;
    int done = 0;
    unsigned long tag;

    while (done == 0) {
        tag = *(unsigned long*)src;
        int id = (int)(tag >> 28) & 7;
        int addr = (long)tag >> 32;
        int qwc = (int)tag & 0xFFFF;

        switch (id) {
        case 0:
        case 2:
        case 4:
        case 5:
        case 7:
            break;
        case 1: {
            int size = qwc << 4;
            src += 0x10;
            MEM_copy(dst, src, size);
            src += size;
            dst += size;
            count += qwc;
            break;
        }
        case 3:
            if (addr == addrA) {
                *(unsigned long*)dst = 0;
                count++;
                dst += 8;
                src += 0x10;
                *(unsigned long*)dst = 0;
                dst += 8;
            } else if (addr == addrB) {
                *(unsigned long*)dst = 0;
                count++;
                dst += 8;
                src += 0x10;
                *(unsigned long*)dst = 0;
                dst += 8;
            } else if (addr == addrC) {
                *(unsigned long*)dst = 0x3000000005000001;
                count++;
                src += 0x10;
                dst += 8;
                *(unsigned long*)dst = 0;
                dst += 8;
            }
            break;
        case 6:
            MEM_copy(dst, src, (qwc + 1) * 0x10);
            src += 0x10;
            dst += 0x10;
            src += qwc * 0x10;
            dst += qwc * 0x10;
            done = 1;
            break;
        }
    }

    tag = *(unsigned long*)buf;
    *(unsigned long*)buf = (tag & 0xFFFF0000) | (count & 0xFFFF);
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00372348);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Repacks a DMA chain in place: copies CNT payloads, blanks REF tags to addrA/addrB, rebases a REF to addrC, stops after RET.
extern "C" void* MEM_copy(void*, void*, int);

/**
 * Rewrites a DMA chain in place as cnt blocks: copies the cnt payloads, blanks some ref tags and keeps the ref to @p
 * addrC as a rebased ref between blocks.
 *
 * Ref tags pointing at @p addrA or @p addrB become a zero quadword inside the current block. A ref to @p addrC closes
 * the block (its header becomes a cnt tag with the block's quadword count), is written as a ref with address @p buf +
 * @p addrC and the source tag's low word, and opens a new block header after it. At the ret tag the open block is
 * closed if non-empty and the ret tag with its data is copied. Tag ids: 1 (cnt) and 6 (ret) carry QWC quadwords of
 * data, 3 (ref) points elsewhere. Likely bug: ids 0, 2, 4, 5 and 7 do not advance the walk, so a chain containing one
 * never ends. A ref tag whose address matches none of the three leaves the walk stuck the same way. No caller.
 *
 * @param buf  the chain, rewritten in place
 * @param addrA  a ref address to blank
 * @param addrB  a ref address to blank
 * @param addrC  the ref address kept as a block boundary
 */
extern "C" void func_00372348(char* buf, int addrA, int addrB, int addrC)
{
    char* src = buf;
    char* dst = buf + 0x10;
    int count = 0;
    unsigned long* hdr = (unsigned long*)buf;
    int done = 0;

    while (done == 0) {
        unsigned long tag = *(unsigned long*)src;
        int id = (int)(tag >> 28) & 7;
        int addr = (long)tag >> 32;
        int qwc = (int)tag & 0xFFFF;

        switch (id) {
        case 0:
        case 2:
        case 4:
        case 5:
        case 7:
            break;
        case 1: {
            int size = qwc << 4;
            src += 0x10;
            MEM_copy(dst, src, size);
            src += size;
            dst += size;
            count += qwc;
            break;
        }
        case 3:
            if (addr == addrA || addr == addrB) {
                *(unsigned long*)dst = 0;
                count++;
                dst += 8;
                src += 0x10;
                *(unsigned long*)dst = 0;
                dst += 8;
            } else if (addr == addrC) {
                hdr[0] = count | 0x10000000;
                hdr[1] = 0;
                count = 0;
                *(unsigned long*)dst = ((unsigned long)(int)(buf + addr) << 32) | *(unsigned int*)src;
                src += 0x10;
                dst += 8;
                *(unsigned long*)dst = 0;
                dst += 8;
                hdr = (unsigned long*)dst;
                dst += 0x10;
            }
            break;
        case 6:
            if (count > 0) {
                hdr[0] = count | 0x10000000;
                hdr[1] = 0;
                count = 0;
            }
            MEM_copy(dst, src, (qwc + 1) * 0x10);
            src += 0x10;
            dst += 0x10;
            src += qwc * 0x10;
            dst += qwc * 0x10;
            done = 1;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cTube_update__FPv);
#ifdef SKIP_ASM
/**
 * Updates a tube effect for the frame: refreshes the shared tube vertex colours (cSimpleTube_updateColours).
 *
 * It is the update slot (slot 2) of both tube vtables, cTube_vtbl (the 4-byte "Tube" effect) and cSimpleTube_vtbl (the
 * segmented tube built by cSimpleTube_cSimpleTube). @p self is only passed on.
 *
 * @return whatever cSimpleTube_updateColours leaves in its return register (nothing meaningful)
 */
void* cTube_update(void* self)
{
    return cSimpleTube_updateColours(self);
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_cSimpleTube);
#ifdef SKIP_ASM
extern int D_004A422C;
extern int D_004A41E0;
extern int D_004A41E4;
extern int D_004A41E8;
extern void* cSimpleTube_vtbl[];

struct sPtVert_2520 {
    float x;        // 0x0
    float y;        // 0x4
    float z;        // 0x8
    int w;          // 0xC
    int r;          // 0x10
    int g;          // 0x14
    int b;          // 0x18
    int a;          // 0x1C
    char pad[0x10];
};
extern sPtVert_2520 D_00501440[];

struct sPtObj_2520 {
    void** vt;      // 0x0
    void* owner;    // 0x4
    int f8;         // 0x8
};

/**
 * Constructs a segmented tube effect with @p owner segments and, the first time, fills the shared tube vertex array.
 *
 * Stores @p owner at +0x4, the tube vtable cSimpleTube_vtbl and a null point array at +0x8. While the flag D_004A422C is
 * clear it sets up all 262 shared vertices of D_00501440: alpha 0xFF, red/green/blue from the tube colour D_004A41E0,
 * D_004A41E4 and D_004A41E8, position x 0, z 1 and w 0, and y alternating 0 (even) and 1 (odd); then sets the flag.
 * cFlexBridgeNode's constructors build one (0xC bytes, "Tube") with NumNodes * 8 - 9 segments.
 *
 * @param owner  the number of segments (misnamed: an int count, not an owner pointer)
 * @return @p self
 */
extern "C" void* cSimpleTube_cSimpleTube(sPtObj_2520* self, void* owner)
{
    int i;
    self->owner = owner;
    self->vt = cSimpleTube_vtbl;
    self->f8 = 0;
    if (D_004A422C == 0) {
        sPtVert_2520* vertex = D_00501440;
        for (i = 0; i < 262; i++, vertex++) {
            vertex->a = 0xFF;
            vertex->r = D_004A41E0;
            vertex->g = D_004A41E4;
            vertex->b = D_004A41E8;
            vertex->z = 1.0f;
            vertex->w = 0;
            vertex->x = 0.0f;
            if (i & 1) {
                vertex->y = 1.0f;
            } else {
                vertex->y = 0.0f;
            }
        }
        D_004A422C = 1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_updateColours);
#ifdef SKIP_ASM
extern float D_004A4230;
extern float D_004A4234;
extern float D_004A4238;

/**
 * Refreshes the colour of the 262 shared tube vertices (D_00501440) when the tube colour has changed.
 *
 * Compares the cached floats D_004A4230, D_004A4234 and D_004A4238 with the tube colour ints D_004A41E0, D_004A41E4 and
 * D_004A41E8; if any differs, it writes the three ints into every vertex's r, g and b and caches the new values as
 * floats. @p self is not used. Called by cTube_update.
 *
 * @return nothing meaningful: declared as returning void* but falls off the end without a return value
 */
extern "C" void* cSimpleTube_updateColours(void* self)
{
    if (D_004A4230 != (float)D_004A41E0 || D_004A4234 != (float)D_004A41E4 || D_004A4238 != (float)D_004A41E8) {
        int i;
        for (i = 0; i < 262; i++) {
            sPtVert_2520* vertex = &D_00501440[i];
            vertex->r = D_004A41E0;
            vertex->g = D_004A41E4;
            vertex->b = D_004A41E8;
        }
        D_004A4230 = (float)D_004A41E0;
        D_004A4234 = (float)D_004A41E4;
        D_004A4238 = (float)D_004A41E8;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube__cSimpleTube);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern void* cSimpleTube_vtbl[];
extern void* D_00488680[];

/**
 * Destroys a segmented tube effect: frees its point array and the object itself when asked.
 *
 * Re-installs the tube vtable cSimpleTube_vtbl, frees the array at +0x8 with cMemMan_free when it is set, installs the base
 * visual-effect vtable D_00488680 and calls operator_delete when bit 0 of @p flags is set. Slot 1 of cSimpleTube_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSimpleTube__cSimpleTube(void* self, int flags)
{
    *(void***)self = cSimpleTube_vtbl;
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_render);
#ifdef SKIP_ASM
struct sPtV4_26C8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPtVtx_26C8 {
    sPtV4_26C8 a;
    sPtV4_26C8 b;
    sPtV4_26C8 pos;                 // 0x20
};

struct sPtRS_26C8 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

struct sPtVEntry_26C8 {
    short delta;
    short index;
    void (*fn)(void*, int, sPtVtx_26C8*, int);
};

struct sPtCtx_26C8 {
    char pad_0x0[0xE84];
    sPtRS_26C8* top;                // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sPtVEntry_26C8* vtable;         // 0x10D8
};

struct sPtTrail_26C8 {
    int f0;
    int n;                          // 0x4
    sPtV4_26C8* pts;                // 0x8
};

extern int D_004A41D4;
extern float D_004A41D8;
extern float D_004A41DC;
extern sPtV4_26C8 t4vect_ZAxis;
extern sPtVtx_26C8 D_00501440_v26C8[] __asm__("D_00501440");
extern sPtRS_26C8 D_00501420_rs26C8 __asm__("sDrawState_s_defaultDrawState");
extern sPtCtx_26C8* D_004A5B80_ctx26C8 __asm__("cGraphicsMan_TheGraphicsMan");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sPtV4_26C8 Sub_26C8(const sPtV4_26C8& a, const sPtV4_26C8& b)
{
    sPtV4_26C8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void SubEq_26C8(sPtV4_26C8& a, const sPtV4_26C8& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sPtV4_26C8 Add_26C8(const sPtV4_26C8& a, const sPtV4_26C8& b)
{
    sPtV4_26C8 r;
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sPtV4_26C8 Norm_26C8(const sPtV4_26C8& v)
{
    sPtV4_26C8 r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sPtV4_26C8 Cross_26C8(const sPtV4_26C8& a, const sPtV4_26C8& b)
{
    sPtV4_26C8 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sPtV4_26C8 Scale_26C8(const sPtV4_26C8& v, float s)
{
    sPtV4_26C8 r;
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

/**
 * Draws a segmented tube effect: a ribbon of two tilted sides along its points, then a line strip along the points.
 *
 * Does nothing while the tube enable flag D_004A41D4 is 0. The direction is the normalised vector from point 0 to point
 * n (n = +0x4, the points at +0x8 are t4Vectors); the side vector is the normalised cross product of it with
 * t4vect_ZAxis scaled by the width D_004A41D8, and both it and its negation are lowered along Z by the height
 * D_004A41DC. For each of the n + 1 points it fills four of the shared vertices D_00501440: point + negated side and
 * the point for the first strip, the point and point + side for the second; two extra vertices copy their neighbours so
 * the two strips join in one degenerate tri strip.
 *
 * Pushes a copy of the current draw state and loads sDrawState_s_defaultDrawState, draws 4 * (n + 1) + 2 vertices
 * through cGraphicsMan_TheGraphicsMan's virtual slot 71 (cPS2GraphicsMan_RenderTriStrip), then copies the points into
 * the first n + 1 vertices and draws them through slot 73 (cPS2GraphicsMan_RenderLineStrip, a line strip), and pops the draw state. Slot
 * 3 (render) of cSimpleTube_vtbl.
 */
extern "C" void cSimpleTube_render(sPtTrail_26C8* self)
{
    if (D_004A41D4 == 0) {
        return;
    }
    sPtV4_26C8 dir = Norm_26C8(Sub_26C8(self->pts[self->n], self->pts[0]));
    sPtV4_26C8 sideOffset = Scale_26C8(Norm_26C8(Cross_26C8(dir, t4vect_ZAxis)), D_004A41D8);
    sPtV4_26C8 otherSideOffset = Scale_26C8(sideOffset, -1.0f);
    SubEq_26C8(sideOffset, Scale_26C8(t4vect_ZAxis, D_004A41DC));
    SubEq_26C8(otherSideOffset, Scale_26C8(t4vect_ZAxis, D_004A41DC));
    for (int i = 0; i < self->n + 1; i++) {
        D_00501440_v26C8[2 * i].pos = Add_26C8(self->pts[i], otherSideOffset);
        D_00501440_v26C8[2 * i + 1].pos = self->pts[i];
        D_00501440_v26C8[2 * (i + self->n + 2)].pos = self->pts[i];
        D_00501440_v26C8[((self->n + i) << 1) + 5].pos = Add_26C8(self->pts[i], sideOffset);
    }
    D_00501440_v26C8[2 * self->n + 2] = D_00501440_v26C8[2 * self->n + 1];
    D_00501440_v26C8[2 * self->n + 3] = D_00501440_v26C8[2 * self->n + 4];
    sPtCtx_26C8* ctx = D_004A5B80_ctx26C8;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = D_00501420_rs26C8;
    sPtVEntry_26C8* vt = ctx->vtable;
    vt[71].fn((char*)ctx + vt[71].delta, ((self->n + 1) << 2) | 2, D_00501440_v26C8, 0);
    for (int i = 0; i < self->n + 1; i++) {
        D_00501440_v26C8[i].pos = self->pts[i];
    }
    vt = ctx->vtable;
    vt[73].fn((char*)ctx + vt[73].delta, self->n + 1, D_00501440_v26C8, 0);
    ctx->top--;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPathArrow_cPathArrow);
#ifdef SKIP_ASM
extern int D_004A423C;
extern int D_004A4208;
extern int D_004A420C;
extern int D_004A4210;
extern int D_004A4214;
extern void* cPathArrow_vtbl[];

struct sPtVert_2AA0 {
    float x;        // 0x0
    float y;        // 0x4
    float z;        // 0x8
    int w;          // 0xC
    int r;          // 0x10
    int g;          // 0x14
    int b;          // 0x18
    int a;          // 0x1C
    char pad[0x10];
};
extern sPtVert_2AA0 D_00504560[];

struct sPtObj_2AA0 {
    void** vt;      // 0x0
};

/**
 * Constructs the path-arrow visual effect and, the first time, fills its 8 shared vertices.
 *
 * Stores the vtable cPathArrow_vtbl. While the flag D_004A423C is clear it sets each of the 8 vertices of D_00504560 to
 * red/green/blue/alpha D_004A4208, D_004A420C, D_004A4210 and D_004A4214, z 1 and x, y, w 0, then sets the flag.
 * cPS2VisualEffectsMan_cPS2VisualEffectsMan allocates it (4 bytes, "PathArrow") as its object 16.
 *
 * @return @p self
 */
extern "C" void* cPathArrow_cPathArrow(sPtObj_2AA0* self)
{
    int i;
    self->vt = cPathArrow_vtbl;
    if (D_004A423C == 0) {
        for (i = 0; i < 8; i++) {
            D_00504560[i].a = D_004A4214;
            D_00504560[i].r = D_004A4208;
            D_00504560[i].g = D_004A420C;
            D_00504560[i].b = D_004A4210;
            D_00504560[i].z = 1.0f;
            D_00504560[i].w = 0;
            D_00504560[i].x = 0.0f;
            D_00504560[i].y = 0.0f;
        }
        D_004A423C = 1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00372B20);
#ifdef SKIP_ASM
/**
 * Compares two ints for sorting: returns *@p lhs - *@p rhs.
 *
 * A qsort-style comparator (ascending order). No caller or table in retail references it.
 *
 * @param lhs  the first int
 * @param rhs  the second int
 * @return negative, zero or positive as *lhs is below, equal to or above *rhs
 */
extern "C" int func_00372B20(int *lhs, int *rhs) {
    return *lhs - *rhs;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00372B30);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }

struct sPtElem2B30 {
    float v[4];
    sPtElem2B30() {}
};

/**
 * Constructs the Bezier patch cache that cPS2BezierMan embeds at +0x10 (mPatchCache): clears its three record pointers.
 *
 * Zeroes the 4x4, 6x6 and 8x8 record arrays at +0xC, +0x10 and +0x14 (the preview's asserts name them m4x4CacheRecord,
 * m6x6CacheRecord, m8x8CacheRecord and check they are NULL before func_00372B78 allocates them). The six placement-new
 * array constructions (4, 6 and 8 t4Vectors, twice: the basis and derivative-basis tables) compile to empty loops;
 * their offsets in the C (+0x20 to +0x1E0) do not match where func_00372B78 keeps the tables (+0x70, +0x1F0), which is
 * harmless since they emit no stores. cPS2BezierMan_cPS2BezierMan calls it.
 *
 * @return @p self
 */
extern "C" void* func_00372B30(void* self)
{
    new ((char*)self + 0x20) sPtElem2B30[4];
    new ((char*)self + 0x60) sPtElem2B30[6];
    new ((char*)self + 0xC0) sPtElem2B30[8];
    new ((char*)self + 0x140) sPtElem2B30[4];
    new ((char*)self + 0x180) sPtElem2B30[6];
    new ((char*)self + 0x1E0) sPtElem2B30[8];
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00372B78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern "C" void MEM_fill(void* dst, int c, int n);
// cBezierCache (render/beziercache.cpp): per patch order (4x4, 6x6, 8x8 control points) a record cache with a free
// list, point / uv grids, a normal cache with its own free list, and the cubic basis tables.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sBzRec_2B78 {
    int f0;
    int f4;
    int f8;                     // -1 = free
};

struct sBzV3_2B78 {
    float x, y, z;
    sBzV3_2B78() {}
};

struct sBzV2_2B78 {
    float u, v;
    sBzV2_2B78() {}
};

struct sBzPts4_2B78 { sBzV3_2B78 p[4][4]; };
struct sBzPts6_2B78 { sBzV3_2B78 p[6][6]; };
struct sBzPts8_2B78 { sBzV3_2B78 p[8][8]; };
struct sBzUV4_2B78 { sBzV2_2B78 p[4][4]; };
struct sBzUV6_2B78 { sBzV2_2B78 p[6][6]; };
struct sBzUV8_2B78 { sBzV2_2B78 p[8][8]; };

struct sBzCache_2B78 {
    int cacheSize[3];               // 0x000
    sBzRec_2B78* m4x4CacheRecord;   // 0x00C
    sBzRec_2B78* m6x6CacheRecord;   // 0x010
    sBzRec_2B78* m8x8CacheRecord;   // 0x014
    sBzRec_2B78* records[3];        // 0x018
    int* freeBuf4;                  // 0x024
    int* freeBuf6;                  // 0x028
    int* freeBuf8;                  // 0x02C
    int* freeList[3];               // 0x030
    int freeListSP[3];              // 0x03C
    sBzPts4_2B78* pts4;             // 0x048
    sBzPts6_2B78* pts6;             // 0x04C
    sBzPts8_2B78* pts8;             // 0x050
    sBzUV4_2B78* uvA4;              // 0x054
    sBzUV6_2B78* uvA6;              // 0x058
    sBzUV8_2B78* uvA8;              // 0x05C
    sBzUV4_2B78* uvB4;              // 0x060
    sBzUV6_2B78* uvB6;              // 0x064
    sBzUV8_2B78* uvB8;              // 0x068
    int pad06C;
    t4Vector basis4[4];             // 0x070
    t4Vector basis6[6];             // 0x0B0
    t4Vector basis8[8];             // 0x110
    t4Vector* basis[3];             // 0x190
    int order[3];                   // 0x19C
    int nrmCacheSize[3];            // 0x1A8
    sBzPts4_2B78* nrm4;             // 0x1B4
    sBzPts6_2B78* nrm6;             // 0x1B8
    sBzPts8_2B78* nrm8;             // 0x1BC
    int* nrmFreeBuf4;               // 0x1C0
    int* nrmFreeBuf6;               // 0x1C4
    int* nrmFreeBuf8;               // 0x1C8
    int* nrmFreeList[3];            // 0x1CC
    int nrmFreeListSP[3];           // 0x1D8
    int pad1E4[3];
    t4Vector dbasis4[4];            // 0x1F0
    t4Vector dbasis6[6];            // 0x230
    t4Vector dbasis8[8];            // 0x290
    t4Vector* dbasis[3];            // 0x310
};

extern char str_4x4CacheRecord[];
extern char str_6x6CacheRecord[];
extern char str_8x8CacheRecord[];
extern char str_4x4FreeList[];
extern char str_6x6FreeList[];
extern char str_8x8FreeList[];
extern char str_4x4PosCache[];
extern char str_6x6PosCache[];
extern char str_8x8PosCache[];
extern char str_4x4UVCache[];
extern char str_6x6UVCache[];
extern char str_8x8UVCache[];
extern char str_4x4LMUVCache[];
extern char str_6x6LMUVCache[];
extern char str_8x8LMUVCache[];
extern char str_4x4NrmCache[];
extern char str_6x6NrmCache[];
extern char str_8x8NrmCache[];
extern char str_4x4FreeNrmList[];
extern char str_6x6FreeNrmList[];
extern char str_8x8FreeNrmList[];

/**
 * Sets up the Bezier patch cache: allocates the record, free-list, position and UV caches for the 4x4, 6x6 and 8x8
 * patch orders and builds their cubic basis tables.
 *
 * For each order it stores the size (mCacheSize, +0x0), allocates that many 12-byte records ("4x4CacheRecord", ...) at
 * +0xC and zero-fills them, an int free list ("4x4FreeList", ...) at +0x24 filled with every index (free-list top
 * +0x3C), a grid of order x order points ("4x4PosCache", 12 bytes each) at +0x48, and two grids of uv pairs at +0x54
 * ("4x4UVCache") and +0x60 ("4x4LMUVCache", the lightmap UVs). The basis tables at +0x70/+0xB0/+0x110 hold (t^3, t^2,
 * t, 1) for t = i / (order - 1); +0x190 points at them and +0x19C holds the orders 4, 6, 8. Every record's third word
 * is set to -1 (no normal entry).
 *
 * The normal cache is optional per order: when @p nn4, @p nn6 or @p nn8 is positive it allocates that many normal grids
 * ("4x4NrmCache", +0x1B4) and a free list ("4x4FreeNrmList", +0x1C0, top +0x1D8) filled with every index; the sizes go
 * to +0x1A8. The derivative tables at +0x1F0/+0x230/+0x290 hold (3t^2, 2t, 1, 0), with pointers at +0x310.
 * cPS2BezierMan_Init calls it with 900, 220, 140 records and 0, 146, 93 normal grids.
 *
 * @param count4x4  number of 4x4 cache records
 * @param count6x6  number of 6x6 cache records
 * @param count8x8  number of 8x8 cache records
 * @param nn4  number of 4x4 normal grids (none when 0)
 * @param nn6  number of 6x6 normal grids
 * @param nn8  number of 8x8 normal grids
 */
extern "C" void func_00372B78(sBzCache_2B78* self, int count4x4, int count6x6, int count8x8, int nn4, int nn6, int nn8)
{
    int i, k;
    float curveT;

    self->cacheSize[0] = count4x4;
    self->cacheSize[1] = count6x6;
    self->cacheSize[2] = count8x8;
    self->m4x4CacheRecord = new (str_4x4CacheRecord, 0, 0) sBzRec_2B78[count4x4];
    self->m6x6CacheRecord = new (str_6x6CacheRecord, 0, 0) sBzRec_2B78[count6x6];
    self->m8x8CacheRecord = new (str_8x8CacheRecord, 0, 0) sBzRec_2B78[count8x8];
    self->records[0] = self->m4x4CacheRecord;
    self->records[1] = self->m6x6CacheRecord;
    self->records[2] = self->m8x8CacheRecord;
    MEM_fill(self->m4x4CacheRecord, 0, count4x4 * sizeof(sBzRec_2B78));
    MEM_fill(self->m6x6CacheRecord, 0, count6x6 * sizeof(sBzRec_2B78));
    MEM_fill(self->m8x8CacheRecord, 0, count8x8 * sizeof(sBzRec_2B78));

    self->freeBuf4 = new (str_4x4FreeList, 0, 0) int[count4x4];
    self->freeBuf6 = new (str_6x6FreeList, 0, 0) int[count6x6];
    self->freeBuf8 = new (str_8x8FreeList, 0, 0) int[count8x8];
    self->freeList[0] = self->freeBuf4;
    self->freeList[1] = self->freeBuf6;
    self->freeList[2] = self->freeBuf8;
    self->freeListSP[0] = 0;
    self->freeListSP[1] = 0;
    self->freeListSP[2] = 0;
    for (i = 0; i < count4x4; i++)
        self->freeList[0][self->freeListSP[0]++] = i;
    for (i = 0; i < count6x6; i++)
        self->freeList[1][self->freeListSP[1]++] = i;
    for (i = 0; i < count8x8; i++)
        self->freeList[2][self->freeListSP[2]++] = i;

    {
        sBzPts4_2B78*& cacheSlot = self->pts4;
        cacheSlot = new (str_4x4PosCache, 0, 0) sBzPts4_2B78[count4x4];
    }
    {
        sBzPts6_2B78*& cacheSlot = self->pts6;
        cacheSlot = new (str_6x6PosCache, 0, 0) sBzPts6_2B78[count6x6];
    }
    {
        sBzPts8_2B78*& cacheSlot = self->pts8;
        cacheSlot = new (str_8x8PosCache, 0, 0) sBzPts8_2B78[count8x8];
    }
    {
        sBzUV4_2B78*& cacheSlot = self->uvA4;
        cacheSlot = new (str_4x4UVCache, 0, 0) sBzUV4_2B78[count4x4];
    }
    {
        sBzUV6_2B78*& cacheSlot = self->uvA6;
        cacheSlot = new (str_6x6UVCache, 0, 0) sBzUV6_2B78[count6x6];
    }
    {
        sBzUV8_2B78*& cacheSlot = self->uvA8;
        cacheSlot = new (str_8x8UVCache, 0, 0) sBzUV8_2B78[count8x8];
    }
    {
        sBzUV4_2B78*& cacheSlot = self->uvB4;
        cacheSlot = new (str_4x4LMUVCache, 0, 0) sBzUV4_2B78[count4x4];
    }
    {
        sBzUV6_2B78*& cacheSlot = self->uvB6;
        cacheSlot = new (str_6x6LMUVCache, 0, 0) sBzUV6_2B78[count6x6];
    }
    {
        sBzUV8_2B78*& cacheSlot = self->uvB8;
        cacheSlot = new (str_8x8LMUVCache, 0, 0) sBzUV8_2B78[count8x8];
    }

    for (i = 0; i < 4; i++) {
        t4Vector basisRow;
        curveT = i * 0.33333334f;
        basisRow.x = curveT * curveT * curveT;
        basisRow.y = curveT * curveT;
        basisRow.z = curveT;
        basisRow.w = 1.0f;
        self->basis4[i] = basisRow;
    }
    for (i = 0; i < 6; i++) {
        t4Vector basisRow;
        curveT = i * 0.2f;
        basisRow.x = curveT * curveT * curveT;
        basisRow.y = curveT * curveT;
        basisRow.z = curveT;
        basisRow.w = 1.0f;
        self->basis6[i] = basisRow;
    }
    for (i = 0; i < 8; i++) {
        t4Vector basisRow;
        curveT = i * 0.14285715f;
        basisRow.x = curveT * curveT * curveT;
        basisRow.y = curveT * curveT;
        basisRow.z = curveT;
        basisRow.w = 1.0f;
        self->basis8[i] = basisRow;
    }
    self->basis[0] = self->basis4;
    self->basis[1] = self->basis6;
    self->basis[2] = self->basis8;
    self->order[0] = 4;
    self->order[1] = 6;
    self->order[2] = 8;
    self->nrmCacheSize[0] = nn4;
    self->nrmCacheSize[1] = nn6;
    self->nrmCacheSize[2] = nn8;

    for (k = 0; k < 3; k++) {
        int j;
        for (j = 0; j < self->cacheSize[k]; j++)
            self->records[k][j].f8 = -1;
    }

    if (nn4 > 0)
        {
        sBzPts4_2B78*& cacheSlot = self->nrm4;
        cacheSlot = new (str_4x4NrmCache, 0, 0) sBzPts4_2B78[nn4];
    }
    if (nn6 > 0)
        {
        sBzPts6_2B78*& cacheSlot = self->nrm6;
        cacheSlot = new (str_6x6NrmCache, 0, 0) sBzPts6_2B78[nn6];
    }
    if (nn8 > 0)
        {
        sBzPts8_2B78*& cacheSlot = self->nrm8;
        cacheSlot = new (str_8x8NrmCache, 0, 0) sBzPts8_2B78[nn8];
    }
    if (nn4 > 0)
        self->nrmFreeBuf4 = new (str_4x4FreeNrmList, 0, 0) int[nn4];
    if (nn6 > 0)
        self->nrmFreeBuf6 = new (str_6x6FreeNrmList, 0, 0) int[nn6];
    if (nn8 > 0)
        self->nrmFreeBuf8 = new (str_8x8FreeNrmList, 0, 0) int[nn8];
    self->nrmFreeList[0] = self->nrmFreeBuf4;
    self->nrmFreeList[1] = self->nrmFreeBuf6;
    self->nrmFreeList[2] = self->nrmFreeBuf8;
    self->nrmFreeListSP[0] = 0;
    self->nrmFreeListSP[1] = 0;
    self->nrmFreeListSP[2] = 0;
    for (i = 0; i < nn4; i++)
        self->nrmFreeList[0][self->nrmFreeListSP[0]++] = i;
    for (i = 0; i < nn6; i++)
        self->nrmFreeList[1][self->nrmFreeListSP[1]++] = i;
    for (i = 0; i < nn8; i++)
        self->nrmFreeList[2][self->nrmFreeListSP[2]++] = i;

    for (i = 0; i < 4; i++) {
        t4Vector basisRow;
        curveT = i * 0.33333334f;
        basisRow.x = curveT * (curveT * 3.0f);
        basisRow.y = curveT + curveT;
        basisRow.z = 1.0f;
        basisRow.w = 0.0f;
        self->dbasis4[i] = basisRow;
    }
    for (i = 0; i < 6; i++) {
        t4Vector basisRow;
        curveT = i * 0.2f;
        basisRow.x = curveT * (curveT * 3.0f);
        basisRow.y = curveT + curveT;
        basisRow.z = 1.0f;
        basisRow.w = 0.0f;
        self->dbasis6[i] = basisRow;
    }
    for (i = 0; i < 8; i++) {
        t4Vector basisRow;
        curveT = i * 0.14285715f;
        basisRow.x = curveT * (curveT * 3.0f);
        basisRow.y = curveT + curveT;
        basisRow.z = 1.0f;
        basisRow.w = 0.0f;
        self->dbasis8[i] = basisRow;
    }
    self->dbasis[0] = self->dbasis4;
    self->dbasis[1] = self->dbasis6;
    self->dbasis[2] = self->dbasis8;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003737D0);
#ifdef SKIP_ASM
struct sPtBufs_37D0 {
    void* p[3];
};

struct sPtSys_37D0 {
    char pad0[0xC];
    sPtBufs_37D0 b0C;       // 0x0C
    char pad18[0xC];
    sPtBufs_37D0 b24;       // 0x24
    char pad30[0x18];
    sPtBufs_37D0 b48;       // 0x48
    sPtBufs_37D0 b54;       // 0x54
    sPtBufs_37D0 b60;       // 0x60
    char pad6C[0x148];
    sPtBufs_37D0 b1B4;      // 0x1B4
    sPtBufs_37D0 b1C0;      // 0x1C0
};

#define PT_FREE_BUFS_37D0(t) \
    if ((t).p[0]) cMemMan_free((t).p[0]); \
    if ((t).p[1]) cMemMan_free((t).p[1]); \
    if ((t).p[2]) cMemMan_free((t).p[2]); \
    (t).p[0] = 0; \
    (t).p[1] = 0; \
    (t).p[2] = 0

/**
 * Frees every buffer of the Bezier patch cache and clears the pointers.
 *
 * Covers the seven three-pointer sets that func_00372B78 allocates: the records (+0xC), free lists (+0x24), position
 * grids (+0x48), UV grids (+0x54), lightmap UV grids (+0x60), normal grids (+0x1B4) and normal free lists (+0x1C0);
 * each non-null pointer goes to cMemMan_free and is then set to 0. No caller in retail references it.
 */
extern "C" void func_003737D0(sPtSys_37D0* self)
{
    PT_FREE_BUFS_37D0(self->b0C);
    PT_FREE_BUFS_37D0(self->b24);
    PT_FREE_BUFS_37D0(self->b48);
    PT_FREE_BUFS_37D0(self->b54);
    PT_FREE_BUFS_37D0(self->b60);
    PT_FREE_BUFS_37D0(self->b1B4);
    PT_FREE_BUFS_37D0(self->b1C0);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003739D0);
#ifdef SKIP_ASM
struct sPtSlot39D0 {
    int f0;
    int f4;
    int f8;
};

struct sPtPools39D0 {
    int cnt[3];                     // 0x0
    sPtSlot39D0* elems[3];          // 0xC
    sPtSlot39D0* slots[3];          // 0x18
    char pad_0x24[0xC];
    int* freeList[3];               // 0x30
    int freeCount[3];               // 0x3C
    char pad_0x48[0x1A8 - 0x48];
    int cntB[3];                    // 0x1A8
    char pad_0x1B4[0x1CC - 0x1B4];
    int* freeListB[3];              // 0x1CC
    int freeCountB[3];              // 0x1D8
};

extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Empties the Bezier patch cache: zero-fills the records and refills both free lists with every index.
 *
 * Zero-fills the 4x4, 6x6 and 8x8 record arrays (+0xC, 12 bytes each, counts from +0x0), resets the free-list tops
 * (+0x3C) and pushes every record index again, marks every record's normal entry -1 and clears its other two words,
 * then does the same for the normal free lists (+0x1CC, tops +0x1D8, counts +0x1A8). cPS2BezierMan_Reset calls it on
 * mPatchCache.
 */
extern "C" void func_003739D0(sPtPools39D0* self)
{
    int count4x4 = self->cnt[0];
    int count6x6 = self->cnt[1];
    int count8x8 = self->cnt[2];
    int size = 0xC;
    MEM_fill(self->elems[0], 0, count4x4 * size);
    MEM_fill(self->elems[1], 0, count6x6 * size);
    MEM_fill(self->elems[2], 0, count8x8 * size);
    self->freeCount[0] = 0;
    self->freeCount[1] = 0;
    self->freeCount[2] = 0;
    int i;
    for (i = 0; i < count4x4; i++) {
        self->freeList[0][self->freeCount[0]++] = i;
    }
    for (i = 0; i < count6x6; i++) {
        self->freeList[1][self->freeCount[1]++] = i;
    }
    for (i = 0; i < count8x8; i++) {
        self->freeList[2][self->freeCount[2]++] = i;
    }
    int nrmCount4x4 = self->cntB[0];
    int nrmCount6x6 = self->cntB[1];
    int nrmCount8x8 = self->cntB[2];
    int k;
    for (k = 0; k < 3; k++) {
        int j;
        for (j = 0; j < self->cnt[k]; j++) {
            self->slots[k][j].f8 = -1;
            self->slots[k][j].f4 = 0;
            self->slots[k][j].f0 = 0;
        }
    }
    self->freeCountB[0] = 0;
    self->freeCountB[1] = 0;
    self->freeCountB[2] = 0;
    for (i = 0; i < nrmCount4x4; i++) {
        self->freeListB[0][self->freeCountB[0]++] = i;
    }
    for (i = 0; i < nrmCount6x6; i++) {
        self->freeListB[1][self->freeCountB[1]++] = i;
    }
    for (i = 0; i < nrmCount8x8; i++) {
        self->freeListB[2][self->freeCountB[2]++] = i;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00373C28);
#ifdef SKIP_ASM
struct sPtVec3_3C28 {
    float x, y, z;
};

// One bone matrix: 4 rows of 4 floats; column j of each row is used for output component j.
struct sPtMtx_3C28 {
    float m[4][4];
};

static inline float ptBlend_3C28(sPtVec3_3C28* p, sPtMtx_3C28* mt, float* w, int j)
{
    float x = p->x;
    float y = p->y;
    float z = p->z;
    return (mt[0].m[0][j] * x + mt[0].m[1][j] * y + mt[0].m[2][j] * z + mt[0].m[3][j]) * w[0]
         + (mt[1].m[0][j] * x + mt[1].m[1][j] * y + mt[1].m[2][j] * z + mt[1].m[3][j]) * w[1]
         + (mt[2].m[0][j] * x + mt[2].m[1][j] * y + mt[2].m[2][j] * z + mt[2].m[3][j]) * w[2]
         + (mt[3].m[0][j] * x + mt[3].m[1][j] * y + mt[3].m[2][j] * z + mt[3].m[3][j]);
}

/**
 * Transforms the 3D point @p points by four matrices and blends the results with weights into @p out (x, y, z).
 *
 * Each output component j is sum over k of (row 0 * x + row 1 * y + row 2 * z + row 3)[j] of matrix k, with matrices 0
 * to 2 weighted by w[0] to w[2] and matrix 3 added unweighted (likely bug, or the caller is meant to fold w[3] in; the
 * four-weight version func_00373EA8 weights all four). @p self is not used. No caller in retail references it.
 *
 * @param points  the point (x, y, z)
 * @param mt  four consecutive 4x4 matrices (rows 0-2 scale x, y, z; row 3 is the translation)
 * @param w  the blend weights (only w[0] to w[2] are read)
 * @param out  receives x, y, z
 */
extern "C" void func_00373C28(void* self, sPtVec3_3C28* points, sPtMtx_3C28* mt, float* w, float* out)
{
    out[0] = ptBlend_3C28(points, mt, w, 0);
    out[1] = ptBlend_3C28(points, mt, w, 1);
    out[2] = ptBlend_3C28(points, mt, w, 2);
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00373EA8);
#ifdef SKIP_ASM
struct sPtVec4_3EA8 {
    float x, y, z, w;
};

// One bone matrix: 4 rows of 4 floats; column j of each row is used for output component j.
struct sPtMtx_3EA8 {
    float m[4][4];
};

static inline float ptBlend4_3EA8(sPtVec4_3EA8* p, sPtMtx_3EA8* mt, float* w, int j)
{
    float x = p->x;
    float y = p->y;
    float z = p->z;
    float h = p->w;
    return (mt[0].m[0][j] * x + mt[0].m[1][j] * y + mt[0].m[2][j] * z + mt[0].m[3][j] * h) * w[0]
         + (mt[1].m[0][j] * x + mt[1].m[1][j] * y + mt[1].m[2][j] * z + mt[1].m[3][j] * h) * w[1]
         + (mt[2].m[0][j] * x + mt[2].m[1][j] * y + mt[2].m[2][j] * z + mt[2].m[3][j] * h) * w[2]
         + (mt[3].m[0][j] * x + mt[3].m[1][j] * y + mt[3].m[2][j] * z + mt[3].m[3][j] * h) * w[3];
}

/**
 * Transforms the homogeneous point @p points by four matrices and blends the results by four weights into @p out.
 *
 * Each output component j is sum over k of w[k] * (row 0 * x + row 1 * y + row 2 * z + row 3 * w)[j] of matrix k; only
 * x, y and z are written. @p self is not used. No caller in retail references it.
 *
 * @param points  the point (x, y, z, w)
 * @param mt  four consecutive 4x4 matrices
 * @param w  the four blend weights
 * @param out  receives x, y, z
 */
extern "C" void func_00373EA8(void* self, sPtVec4_3EA8* points, sPtMtx_3EA8* mt, float* w, float* out)
{
    out[0] = ptBlend4_3EA8(points, mt, w, 0);
    out[1] = ptBlend4_3EA8(points, mt, w, 1);
    out[2] = ptBlend4_3EA8(points, mt, w, 2);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374180);
#ifdef SKIP_ASM
struct sPatchVec {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPatchOut {
    float x, y, z;
};

struct sPatchSet {
    char pad_0x00[0x190];
    sPatchVec* basis[3];    // 0x190
    int count[3];           // 0x19C
};

/**
 * Evaluates a cubic Bezier patch on an order x order grid into a position cache entry (VU0 macro code).
 *
 * Loads the patch's 16 coefficient vectors (four 4x4 power-basis blocks, from +0x40 of @p mat) into VU0, and for each
 * row i and column j of the grid (order from +0x19C[@p idx], basis rows (t^3, t^2, t, 1) from +0x190[@p idx]) combines
 * them with basis[i] and basis[j]; writes x, y, z of each point (12 bytes) to @p out in row order. func_003747A0 calls
 * it.
 *
 * @param mat  the patch record (tPatchRecord): coefficients at +0x40
 * @param out  the order x order position grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
// PORT: PS2-only VU0 inline asm (tensor-product patch evaluation: out[i][j] =
// sum over basis[i] (x) basis[j] of the 4x4 control block at mat+0x40).
extern "C" void func_00374180(sPatchSet* self, char* mat, sPatchOut* out, int idx)
{
    int n = self->count[idx];
    sPatchVec* basis = self->basis[idx];
    int i;
    int j;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "lqc2      $vf5, 0x40(%0)\n"
        "lqc2      $vf6, 0x50(%0)\n"
        "lqc2      $vf7, 0x60(%0)\n"
        "lqc2      $vf8, 0x70(%0)\n"
        "lqc2      $vf9, 0x80(%0)\n"
        "lqc2      $vf10, 0x90(%0)\n"
        "lqc2      $vf11, 0xA0(%0)\n"
        "lqc2      $vf12, 0xB0(%0)\n"
        "lqc2      $vf13, 0xC0(%0)\n"
        "lqc2      $vf14, 0xD0(%0)\n"
        "lqc2      $vf15, 0xE0(%0)\n"
        "lqc2      $vf16, 0xF0(%0)\n"
        :
        : "r"(mat + 0x40));
    for (i = 0; i < n; i++) {
        __asm__ __volatile__(
            "lqc2      $vf17, 0x0(%0)\n"
            "vmulax.xyzw ACC, $vf1, $vf17x\n"
            "vmadday.xyzw ACC, $vf2, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf3, $vf17z\n"
            "vmaddw.xyzw $vf18, $vf4, $vf17w\n"
            "vmulax.xyzw ACC, $vf5, $vf17x\n"
            "vmadday.xyzw ACC, $vf6, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf7, $vf17z\n"
            "vmaddw.xyzw $vf19, $vf8, $vf17w\n"
            "vmulax.xyzw ACC, $vf9, $vf17x\n"
            "vmadday.xyzw ACC, $vf10, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf11, $vf17z\n"
            "vmaddw.xyzw $vf20, $vf12, $vf17w\n"
            "vmulax.xyzw ACC, $vf13, $vf17x\n"
            "vmadday.xyzw ACC, $vf14, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf15, $vf17z\n"
            "vmaddw.xyzw $vf21, $vf16, $vf17w\n"
            :
            : "r"(&basis[i]));
        for (j = 0; j < n; j++) {
            sPatchVec point;
            __asm__(
                "lqc2      $vf22, 0x0(%1)\n"
                "vmulax.xyzw ACC, $vf18, $vf22x\n"
                "vmadday.xyzw ACC, $vf19, $vf22y\n"
                "vmaddaz.xyzw ACC, $vf20, $vf22z\n"
                "vmaddw.xyzw $vf23, $vf21, $vf22w\n"
                "sqc2      $vf23, %0\n"
                : "=m"(point)
                : "r"(&basis[j]));
            *out++ = *(sPatchOut*)&point;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374298);
#ifdef SKIP_ASM
struct sV2_4298 {
    float x, y;
    sV2_4298() {}
    sV2_4298(float ax, float ay) : x(ax), y(ay) {}
    sV2_4298& operator+=(const sV2_4298& b) { x += b.x; y += b.y; return *this; }
};

static inline sV2_4298 operator-(const sV2_4298& a, const sV2_4298& b)
{
    return sV2_4298(a.x - b.x, a.y - b.y);
}

static inline sV2_4298 operator*(const sV2_4298& a, float k)
{
    return sV2_4298(a.x * k, a.y * k);
}

/**
 * Fills a patch's texture-UV grid by bilinear interpolation between its four corner UVs.
 *
 * The corners come from @p src: +0x20 and +0x28 (first row's ends) and +0x30 and +0x38 (last row's ends). With order n
 * from +0x19C[@p idx], each of the n rows steps its two ends by 1 / (n - 1) of the way to the last row and writes n
 * evenly spaced uv pairs from one end to the other. func_003747A0 calls it.
 *
 * @param src  the patch record (tPatchRecord)
 * @param out  the n x n uv grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
extern "C" void func_00374298(char* self, char* src, sV2_4298* out, int idx)
{
    int n = *(int*)(self + (idx << 2) + 0x19C);
    sV2_4298 leftEdge = *(sV2_4298*)(src + 0x20);
    sV2_4298 rightEdge = *(sV2_4298*)(src + 0x28);
    sV2_4298 bottomLeft = *(sV2_4298*)(src + 0x30);
    sV2_4298 bottomRight = *(sV2_4298*)(src + 0x38);
    float step = 1.0f / (float)(n - 1);
    sV2_4298 dl = (bottomLeft - leftEdge) * step;
    sV2_4298 dr = (bottomRight - rightEdge) * step;
    int i;
    for (i = 0; i < n; i++) {
        sV2_4298 point = leftEdge;
        sV2_4298 colStep = (rightEdge - leftEdge) * step;
        int j;
        for (j = 0; j < n; j++) {
            *out = point;
            out++;
            point += colStep;
        }
        leftEdge += dl;
        rightEdge += dr;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374440);
#ifdef SKIP_ASM
struct sPtVec2 {
    float x, y;
};

struct sPtRect {
    char pad_0x0[0x10];
    sPtVec2 origin;
    sPtVec2 size;
};

/**
 * Fills a patch's lightmap-UV grid: an even n x n grid over the rectangle at +0x10 of @p rect.
 *
 * With order n from +0x19C[@p idx], the step is the rectangle's size (+0x18) / (n - 1); for each column x from the
 * origin (+0x10) it writes n points stepping y. func_003747A0 calls it for the lightmap UV cache.
 *
 * @param rect  the patch record (tPatchRecord): lightmap origin at +0x10, size at +0x18
 * @param out  the n x n uv grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
extern "C" void func_00374440(char* self, sPtRect* rect, sPtVec2* out, int idx)
{
    int i, j;
    int n = *(int*)(self + (idx << 2) + 0x19C);
    sPtVec2 pos = rect->origin;
    sPtVec2 step = rect->size;
    float inv = 1.0f / (float)(n - 1);
    step.x *= inv;
    step.y *= inv;
    for (i = 0; i < n; i++) {
        sPtVec2 cur = pos;
        for (j = 0; j < n; j++) {
            *out++ = cur;
            cur.y += step.y;
        }
        pos.x += step.x;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374518);
#ifdef SKIP_ASM
struct sPatchVec4518 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPatchOut4518 {
    float x, y, z;
    sPatchOut4518() {}
    sPatchOut4518(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sPatchOut4518(const sPatchOut4518& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline sPatchOut4518 Cross4518(const sPatchOut4518& a, const sPatchOut4518& b)
{
    return sPatchOut4518(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline sPatchOut4518 Normalize4518(const sPatchOut4518& v)
{
    float d2 = v.x * v.x + v.y * v.y + v.z * v.z;
    float d;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    if (d != 0.0f) {
        float k = 1.0f / d;
        return sPatchOut4518(v.x * k, v.y * k, v.z * k);
    }
    return v;
}

struct sPatchSet4518 {
    char pad_0x00[0x190];
    sPatchVec4518* basis[3];    // 0x190
    int count[3];               // 0x19C
    char pad_0x1A8[0x310 - 0x1A8];
    sPatchVec4518* dbasis[3];   // 0x310
};

/**
 * Computes a patch's n x n grid of unit normals (VU0 macro code).
 *
 * Loads the 16 coefficient vectors from +0x40 of @p mat. A first pass evaluates the patch with the derivative basis
 * (3t^2, 2t, 1, 0; +0x310[@p idx]) in one direction and the basis (+0x190[@p idx]) in the other, storing that tangent
 * in
 * @p out; a second pass evaluates the other tangent and replaces each entry with the normalised cross product of the
 * two (left unnormalised when its length is 0). func_003747A0 calls it for patches that need normals.
 *
 * @param mat  the patch record (tPatchRecord): coefficients at +0x40
 * @param out  the n x n normal grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
// PORT: PS2-only VU0 inline asm (tensor-product patch evaluation, then normals
// as the normalised cross product of the two partial derivatives).
extern "C" void func_00374518(sPatchSet4518* self, char* mat, sPatchOut4518* out, int idx)
{
    int n = self->count[idx];
    sPatchVec4518* basis = self->basis[idx];
    sPatchVec4518* dbasis = self->dbasis[idx];
    sPatchOut4518* cursor = out;
    int i;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "lqc2      $vf5, 0x40(%0)\n"
        "lqc2      $vf6, 0x50(%0)\n"
        "lqc2      $vf7, 0x60(%0)\n"
        "lqc2      $vf8, 0x70(%0)\n"
        "lqc2      $vf9, 0x80(%0)\n"
        "lqc2      $vf10, 0x90(%0)\n"
        "lqc2      $vf11, 0xA0(%0)\n"
        "lqc2      $vf12, 0xB0(%0)\n"
        "lqc2      $vf13, 0xC0(%0)\n"
        "lqc2      $vf14, 0xD0(%0)\n"
        "lqc2      $vf15, 0xE0(%0)\n"
        "lqc2      $vf16, 0xF0(%0)\n"
        :
        : "r"(mat + 0x40));
    for (i = 0; i < n; i++) {
        __asm__ __volatile__(
            "lqc2      $vf17, 0x0(%0)\n"
            "vmulax.xyzw ACC, $vf1, $vf17x\n"
            "vmadday.xyzw ACC, $vf2, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf3, $vf17z\n"
            "vmaddw.xyzw $vf18, $vf4, $vf17w\n"
            "vmulax.xyzw ACC, $vf5, $vf17x\n"
            "vmadday.xyzw ACC, $vf6, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf7, $vf17z\n"
            "vmaddw.xyzw $vf19, $vf8, $vf17w\n"
            "vmulax.xyzw ACC, $vf9, $vf17x\n"
            "vmadday.xyzw ACC, $vf10, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf11, $vf17z\n"
            "vmaddw.xyzw $vf20, $vf12, $vf17w\n"
            "vmulax.xyzw ACC, $vf13, $vf17x\n"
            "vmadday.xyzw ACC, $vf14, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf15, $vf17z\n"
            "vmaddw.xyzw $vf21, $vf16, $vf17w\n"
            :
            : "r"(&dbasis[i]));
        for (int j = 0; j < n; j++) {
            sPatchVec4518 point;
            __asm__(
                "lqc2      $vf22, 0x0(%1)\n"
                "vmulax.xyzw ACC, $vf18, $vf22x\n"
                "vmadday.xyzw ACC, $vf19, $vf22y\n"
                "vmaddaz.xyzw ACC, $vf20, $vf22z\n"
                "vmaddw.xyzw $vf23, $vf21, $vf22w\n"
                "sqc2      $vf23, %0\n"
                : "=m"(point)
                : "r"(&basis[j]));
            *cursor++ = *(sPatchOut4518*)&point;
        }
    }
    cursor = out;
    for (i = 0; i < n; i++) {
        __asm__ __volatile__(
            "lqc2      $vf17, 0x0(%0)\n"
            "vmulax.xyzw ACC, $vf1, $vf17x\n"
            "vmadday.xyzw ACC, $vf2, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf3, $vf17z\n"
            "vmaddw.xyzw $vf18, $vf4, $vf17w\n"
            "vmulax.xyzw ACC, $vf5, $vf17x\n"
            "vmadday.xyzw ACC, $vf6, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf7, $vf17z\n"
            "vmaddw.xyzw $vf19, $vf8, $vf17w\n"
            "vmulax.xyzw ACC, $vf9, $vf17x\n"
            "vmadday.xyzw ACC, $vf10, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf11, $vf17z\n"
            "vmaddw.xyzw $vf20, $vf12, $vf17w\n"
            "vmulax.xyzw ACC, $vf13, $vf17x\n"
            "vmadday.xyzw ACC, $vf14, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf15, $vf17z\n"
            "vmaddw.xyzw $vf21, $vf16, $vf17w\n"
            :
            : "r"(&basis[i]));
        for (int j = 0; j < n; j++) {
            sPatchVec4518 point;
            __asm__(
                "lqc2      $vf22, 0x0(%1)\n"
                "vmulax.xyzw ACC, $vf18, $vf22x\n"
                "vmadday.xyzw ACC, $vf19, $vf22y\n"
                "vmaddaz.xyzw ACC, $vf20, $vf22z\n"
                "vmaddw.xyzw $vf23, $vf21, $vf22w\n"
                "sqc2      $vf23, %0\n"
                : "=m"(point)
                : "r"(&dbasis[j]));
            sPatchOut4518 normal = Normalize4518(Cross4518(*(sPatchOut4518*)&point, *cursor));
            *cursor++ = normal;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003747A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPV3_47A0 {
    float x, y, z;
};

struct sPV2_47A0 {
    float x, y;
};

struct sPEnt_47A0 {
    char* mat;      // 0x0
    int flags;      // 0x4
    int nslot;      // 0x8
};

struct sPMgr_47A0 {
    char pad_0x0[0x18];
    sPEnt_47A0* entries[3];         // 0x18
    char pad_0x24[0xC];
    int* freeList[3];               // 0x30
    int freeCount[3];               // 0x3C
    sPV3_47A0 (*pos0)[16];          // 0x48
    sPV3_47A0 (*pos1)[36];          // 0x4C
    sPV3_47A0 (*pos2)[64];          // 0x50
    sPV2_47A0 (*uv0)[16];           // 0x54
    sPV2_47A0 (*uv1)[36];           // 0x58
    sPV2_47A0 (*uv2)[64];           // 0x5C
    sPV2_47A0 (*col0)[16];          // 0x60
    sPV2_47A0 (*col1)[36];          // 0x64
    sPV2_47A0 (*col2)[64];          // 0x68
    char pad_0x6C[0x1B4 - 0x6C];
    sPV3_47A0 (*nrm0)[16];          // 0x1B4
    sPV3_47A0 (*nrm1)[36];          // 0x1B8
    sPV3_47A0 (*nrm2)[64];          // 0x1BC
    char pad_0x1C0[0xC];
    int* nfreeList[3];              // 0x1CC
    int nfreeCount[3];              // 0x1D8
};

// func_00374518 is declared through a local view so the unit's own definition keeps its types
extern "C" void func_00374518_47A0(sPMgr_47A0* self, char* mat, sPV3_47A0* out, int idx) __asm__("func_00374518");

static inline int PopSlot_47A0(sPMgr_47A0* self, int idx)
{
    int r;
    if (self->freeCount[idx] > 0) {
        r = self->freeList[idx][--self->freeCount[idx]];
    } else {
        r = -1;
    }
    return r;
}

static inline sPV3_47A0* PosOf_47A0(sPMgr_47A0* self, int idx, int slot)
{
    switch (idx) {
    case 0:
        return self->pos0[slot];
    case 1:
        return self->pos1[slot];
    case 2:
        return self->pos2[slot];
    }
    return 0;
}

static inline sPV2_47A0* UVOf_47A0(sPMgr_47A0* self, int idx, int slot)
{
    switch (idx) {
    case 0:
        return self->uv0[slot];
    case 1:
        return self->uv1[slot];
    case 2:
        return self->uv2[slot];
    }
    return 0;
}

static inline sPV2_47A0* ColOf_47A0(sPMgr_47A0* self, int idx, int slot)
{
    switch (idx) {
    case 0:
        return self->col0[slot];
    case 1:
        return self->col1[slot];
    case 2:
        return self->col2[slot];
    }
    return 0;
}

static inline sPV3_47A0* NrmOf_47A0(sPMgr_47A0* self, int idx, int slot)
{
    switch (idx) {
    case 0:
        return self->nrm0[slot];
    case 1:
        return self->nrm1[slot];
    case 2:
        return self->nrm2[slot];
    }
    return 0;
}

/**
 * Caches a patch in the Bezier patch cache: takes a free record and fills its position, UV, lightmap UV and
 * (optionally) normal grids.
 *
 * Pops a record index from the free list of order @p idx (-1 when empty), evaluates the positions (func_00374180), UVs
 * (func_00374298) and lightmap UVs (func_00374440) into that entry of each cache, stores @p mat in the record, the
 * index in the patch's halfword +0x1A6[@p idx], and ORs 0xC | (1 << @p bit) into the record's flags (marked in use for
 * two ageing passes of func_00374B38). With @p normals it also pops a normal-grid index, fills it with func_00374518
 * and stores it in the record. Likely bug: there is no check for an empty free list (the preview asserts
 * mFreeListSP[Size] > 0), so a full cache writes entry -1. cPS2BezierMan_DrawPatches calls it for each queued patch
 * without a cached slot.
 *
 * @param mat  the patch record (tPatchRecord)
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 * @param bit  the view index; sets that bit in the record's flags
 * @param normals  nonzero to also cache normals
 * @return the record index (-1 when the cache was full)
 */
extern "C" int func_003747A0(sPMgr_47A0* self, char* mat, int idx, int bit, int normals)
{
    int slot = PopSlot_47A0(self, idx);
    sPV3_47A0* pos = PosOf_47A0(self, idx, slot);
    sPV2_47A0* uv = UVOf_47A0(self, idx, slot);
    sPV2_47A0* col = ColOf_47A0(self, idx, slot);
    func_00374180((sPatchSet*)self, mat, (sPatchOut*)pos, idx);
    func_00374298((char*)self, mat, (sV2_4298*)uv, idx);
    func_00374440((char*)self, (sPtRect*)mat, (sPtVec2*)col, idx);
    sPEnt_47A0* entry = &self->entries[idx][slot];
    entry->mat = mat;
    *(short*)(mat + (idx << 1) + 0x1A6) = slot;
    entry->flags |= 0xC | (1 << bit);
    if (normals) {
        int nslot = self->nfreeList[idx][--self->nfreeCount[idx]];
        sPV3_47A0* nrm = NrmOf_47A0(self, idx, nslot);
        func_00374518_47A0(self, mat, nrm, idx);
        entry->nslot = nslot;
    }
    return slot;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374A88);
#ifdef SKIP_ASM
struct sPartItem {
    char pad[0xC];
    short v;
    short pad2;
};

struct sPartOwner {
    char pad[0x18];
    char* lists[1];
};

/**
 * Marks the cached records of a list of queued patches as used in this frame and view.
 *
 * For each of the @p count 16-byte entries of @p items whose slot (+0xC, a short) is not negative, ORs 0xC | (1 <<
 * @p bit) into the flags (+0x4) of that record in the cache's record array +0x18[@p idx]. cPS2BezierMan_DrawPatches
 * calls it for the 4x4, 6x6 and 8x8 lists; func_00374AE0 is the same for the 20-byte mixed-edge list.
 *
 * @param count  number of queued patches
 * @param items  the queued patches (tBezierPatchEntry, 16 bytes each)
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 * @param bit  the view index
 */
extern "C" void func_00374A88(sPartOwner* self, int count, sPartItem* items, int idx, int bit)
{
    int mask = (1 << bit) | 0xC;
    for (int i = 0; i < count; i++) {
        short listIndex = items[i].v;
        if (listIndex >= 0) {
            char* entry = self->lists[idx] + listIndex * 0xC;
            *(int*)(entry + 4) |= mask;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374AE0);
#ifdef SKIP_ASM
struct sPartItem14 {
    char pad[0xC];
    short v;
    short pad2;
    int pad3;
};

/**
 * Marks the cached records of the mixed-edge patch list as used in this frame and view.
 *
 * The same loop as func_00374A88 over 20-byte entries (tBezierMixedPatchEntry): for each one with a slot (+0xC) of 0 or
 * more, ORs 0xC | (1 << @p bit) into that record's flags in the record array +0x18[@p idx]. cPS2BezierMan_DrawPatches
 * calls it with order 2 (8x8).
 *
 * @param count  number of mixed patches
 * @param items  the mixed patches (tBezierMixedPatchEntry, 20 bytes each)
 * @param idx  the patch order (2 = 8x8)
 * @param bit  the view index
 */
extern "C" void func_00374AE0(sPartOwner* self, int count, sPartItem14* items, int idx, int bit)
{
    int mask = (1 << bit) | 0xC;
    for (int i = 0; i < count; i++) {
        short listIndex = items[i].v;
        if (listIndex >= 0) {
            char* entry = self->lists[idx] + listIndex * 0xC;
            *(int*)(entry + 4) |= mask;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374B38);
#ifdef SKIP_ASM
struct sPartRef {
    char pad_0x00[0x1A6];
    short slot[3];          // 0x1A6
};

struct sPartSlotEntry {
    sPartRef* owner;      // 0x00
    int flags;              // 0x04
    int slot;               // 0x08
};

struct sPartLists {
    int count[3];               // 0x00
    char pad_0x0C[0xC];
    sPartSlotEntry* entries[3];     // 0x18
    char pad_0x24[0xC];
    int* freeList[3];           // 0x30
    int freeCount[3];           // 0x3C
    char pad_0x48[0x1CC - 0x48];
    int* slotList[3];           // 0x1CC
    int slotCount[3];           // 0x1D8
};

/**
 * Ages the Bezier patch cache records and frees those unused for two passes.
 *
 * For each order and record: a set bit 2 is cleared first, then bit 3 on the next pass; a record with other flags but
 * neither bit is freed: flags 0, the owning patch's slot halfword (+0x1A6[order]) set to -1, its normal-grid index (if
 * 0 or more) pushed back on the normal free list and set to -1, and the record index pushed on the free list. Records
 * with no flags are left alone. cPS2BezierMan_DrawPatches calls it once a frame (for view 0) after marking the used
 * ones.
 */
extern "C" void func_00374B38(sPartLists* self)
{
    int i;
    for (i = 0; i < 3; i++) {
        int j;
        int n = self->count[i];
        sPartSlotEntry* list = self->entries[i];
        for (j = 0; j < n; j++) {
            sPartSlotEntry* entry = &list[j];
            int curFlags = entry->flags;
            if (curFlags & 4) {
                entry->flags = curFlags & ~4;
            } else if (curFlags & 8) {
                entry->flags = curFlags & ~8;
            } else if (curFlags != 0) {
                entry->flags = 0;
                entry->owner->slot[i] = -1;
                if (entry->slot >= 0) {
                    self->slotList[i][self->slotCount[i]++] = entry->slot;
                    entry->slot = -1;
                }
                self->freeList[i][self->freeCount[i]++] = j;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierMan_cBezierMan);
#ifdef SKIP_ASM
extern void* cBezierMan_TheBezierMan;
extern void* cBezierMan_vtbl[];

/**
 * Constructs the Bezier patch manager base: installs its vtable cBezierMan_vtbl and registers the object as
 * cBezierMan_TheBezierMan.
 *
 * cPS2BezierMan_cPS2BezierMan__FPv calls it first.
 *
 * @return @p self
 */
extern "C" void* cBezierMan_cBezierMan(cPS2BezierMan* self)
{
    self->mVtbl = cBezierMan_vtbl;
    cBezierMan_TheBezierMan = ((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierMan__cBezierMan);
#ifdef SKIP_ASM
extern void* cBezierMan_TheBezierMan;
extern void* cBezierMan_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys the Bezier patch manager base: re-installs its vtable cBezierMan_vtbl, clears cBezierMan_TheBezierMan and frees
 * the object when asked.
 *
 * Slot 1 of cBezierMan_vtbl; cPS2BezierMan__cPS2BezierMan chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cBezierMan__cBezierMan(cPS2BezierMan* self, int flags)
{
    self->mVtbl = cBezierMan_vtbl;
    cBezierMan_TheBezierMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPlayerShadow_InitBones);
#ifdef SKIP_ASM
/**
 * Sets the six bones the rider's shadow is projected from.
 *
 * Stores the bone indices into ShadowBone[0..5] (+0x17C..+0x190), right after mShadowTexHeight.
 *
 * @param head  the head bone
 * @param lHand  the left hand bone
 * @param rHand  the right hand bone
 * @param lFoot  the left foot bone
 * @param rFoot  the right foot bone
 * @param hips  the hips bone
 */
extern "C" void cPlayerShadow_InitBones(cPlayerShadow* self, int head, int lHand, int rHand, int lFoot, int rFoot, int hips)
{
    self->ShadowBone[0] = head;
    self->ShadowBone[1] = lHand;
    self->ShadowBone[2] = rHand;
    self->ShadowBone[3] = lFoot;
    self->ShadowBone[4] = rFoot;
    self->ShadowBone[5] = hips;
}
#endif

INCLUDE_ASM("render/particle", func_00374D00);

//100%
INCLUDE_ASM("render/particle", cPlayerShadow_AddShadow);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");

struct sPtVec4_5890 {
    float x, y, z, w;
    sPtVec4_5890(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
};

class cGfxMan_5890 {
public:
    char pad[0x10D8];
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
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67(const sPtVec4_5890& pos, void* a, void* b, int tex, float scale);
};

/**
 * Registers a rider's shadow with the renderer as a projective texture.
 *
 * Calls cGraphicsMan_TheGraphicsMan's virtual slot 67 (cPS2GraphicsMan_AddProjectiveTexture) with the position (+0xC,
 * +0x10, +0x14, w = 1; On Tour's cPlayerShadow mProjPos), the matrices at +0x90 and +0xD0 (On Tour mCProjMat and
 * mUVProjMat), intensity 1.0 and the texture id at +0x170 (On Tour mLockTexture). cRider_RenderShadow calls it.
 */
extern "C" void cPlayerShadow_AddShadow(void* self)
{
    char* shadow = (char*)self;
    ((cGfxMan_5890*)cGraphicsMan_TheGraphicsMan_particle)->v67(sPtVec4_5890(*(float*)(shadow + 0xC), *(float*)(shadow + 0x10), *(float*)(shadow + 0x14), 1.0f),
                                     shadow + 0x90, shadow + 0xD0, *(int*)(shadow + 0x170), 1.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPlayerShadow_cPlayerShadow__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a rider's shadow: no locked texture yet and a 128 x 128 shadow texture size.
 *
 * Sets mLockTexture (+0x170) to -1 and mShadowTexWidth/mShadowTexHeight (+0x174/+0x178) to 0x80, the values On Tour's
 * constructor stores at the same offsets. cRider_cRider_impl builds its mShadow with it on a 0x1A0-byte "BoaderShadow"
 * block.
 *
 * @return the constructed shadow (self)
 */
void* cPlayerShadow_cPlayerShadow(void* self)
{
    *(int*)((char*)self + 0x170) = -1;
    *(int*)((char*)self + 0x174) = 0x80;
    *(int*)((char*)self + 0x178) = 0x80;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPlayerShadow__cPlayerShadow);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void func_00367B60(void* mgr, int id);
void operator_delete(int* ptr);

struct sPtVEntry_5918 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys a rider's shadow: releases its locked texture and frees it if asked.
 *
 * When mLockTexture (+0x170) is a valid id, frees it in cGraphicsMan_TheGraphicsMan's texture manager (+0x18F4,
 * func_00367B60), removes it through the graphics manager's vtable slot 50 (cPS2GraphicsMan_RemoveTexture) and resets
 * it to -1. Then operator_delete on @p flags bit 0 (cRider__cRider passes 3).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPlayerShadow__cPlayerShadow(cPlayerShadow* self, int flags)
{
    int tex = self->mLockTexture;
    if (tex >= 0) {
        func_00367B60(*(void**)(cGraphicsMan_TheGraphicsMan_particle + 0x18F4), tex);
        char* mgr = cGraphicsMan_TheGraphicsMan_particle;
        sPtVEntry_5918* vt = *(sPtVEntry_5918**)(mgr + 0x10D8);
        vt[50].fn(mgr + vt[50].delta, self->mLockTexture);
        self->mLockTexture = -1;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00375990);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");

struct sPtVEntry_5990 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

/**
 * Creates the render target texture for a rider's shadow if it has none yet.
 *
 * When the texture id at +0x170 (On Tour's cPlayerShadow mLockTexture) is negative, it asks
 * cGraphicsMan_TheGraphicsMan's virtual slot 52 (cPS2GraphicsMan_AddLockedFrameTexture__FPv; On Tour
 * AddLockedFrameTexture(width, height, texeltype)) for a texture of the size at +0x174 and +0x178 (mShadowTexWidth,
 * mShadowTexHeight) in texel format 2 and stores the id. cRider_loadRider calls it.
 */
extern "C" void func_00375990(void* self)
{
    char* shadow = (char*)self;
    if (*(int*)(shadow + 0x170) < 0) {
        char* mgr = cGraphicsMan_TheGraphicsMan_particle;
        sPtVEntry_5990* vt = *(sPtVEntry_5990**)(mgr + 0x10D8);
        *(int*)(shadow + 0x170) = vt[52].fn(mgr + vt[52].delta, *(int*)(shadow + 0x174), *(int*)(shadow + 0x178), 2);
    }
}
#endif

extern void* cDetailSystem_vtbl[];

//100%
INCLUDE_ASM("render/particle", cDetailSystem_cDetailSystem__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the detail-system visual effect (the terrain sparkle effect): stores its vtable cDetailSystem_vtbl.
 *
 * cPS2VisualEffectsMan_cPS2VisualEffectsMan allocates it (4 bytes, "DetailSystem") as its object 6.
 *
 * @return @p self
 */
void* cDetailSystem_cDetailSystem(void* self)
{
    *(int*)self = (int)(void*)cDetailSystem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", cDetailSystem_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the detail-system effect's per-frame update (slot 2 of cDetailSystem_vtbl).
 */
void cDetailSystem_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/particle", cGraphicsMan_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_PS2GraphicsMan[];
extern "C" void* cPS2GraphicsMan_cPS2GraphicsMan(void* self);

/**
 * Creates the renderer: allocates 0x75E0 bytes tagged "PS2GraphicsMan" and constructs a cPS2GraphicsMan in them.
 *
 * cPS2GraphicsMan_cPS2GraphicsMan stores the instance in cGraphicsMan_TheGraphicsMan. cSSXApp_init calls it.
 *
 * @return the new cPS2GraphicsMan
 */
extern "C" void* cGraphicsMan_construct(void)
{
    return cPS2GraphicsMan_cPS2GraphicsMan(cMemMan_alloc(30176, str_PS2GraphicsMan, 0, 0));
}
#endif

/**
 * Initialises the PS2 renderer: kick thread and semaphores, interrupt handlers, texture and defer managers.
 *
 * Hand-checked from the asm only (no C body yet): it calls a run of setup functions (func_003FDF88, func_003912A8,
 * func_00391360, ...), allocates two objects with operator_new, creates two semaphores and the kick thread (CreateSema,
 * CreateThread, StartThread), calls DisableIntc, func_00423A90, EnableIntc, DisableDmac, func_00423AC0 and
 * EnableDmac (the interrupt-handler setup that Purge undoes), then allocates the texture manager (cMemMan_alloc +
 * func_00366FE8) and the defer manager and sets up the default render state. cPS2GraphicsMan_Purge undoes these steps.
 */
INCLUDE_ASM("render/particle", cPS2GraphicsMan_Init);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_Purge);
#ifdef SKIP_ASM
extern "C" void DisableIntc(int);
extern "C" void RemoveIntcHandler(int, int);
extern "C" void DisableDmac(int);
extern "C" void RemoveDmacHandler(int, int);
extern "C" void TerminateThread(int);
extern "C" void DeleteThread(int);
extern "C" void DeleteSema(int);
extern "C" void func_00367360(void*);
void* func_00361F40(void* self);
void operator_delete(int* ptr);
extern char D_005059D8[];

/**
 * Shuts the PS2 renderer down, undoing cPS2GraphicsMan_Init.
 *
 * Calls DisableIntc(2) and removes the INTC 2 handler (mIntcHandlerId), calls DisableDmac(1) and removes the DMAC 0
 * handler (mDmacHandlerId), terminates and deletes the kick thread (mKickThreadId), deletes mKickSema and mQueueMutex,
 * deletes the defer manager (mpDeferMan), purges and deletes the texture manager (func_00367360, mpTextureMan), and
 * finally calls func_00361F40 on the render queue D_005059D8.
 */
extern "C" void cPS2GraphicsMan_Purge(cPS2GraphicsMan* self)
{
    DisableIntc(2);
    RemoveIntcHandler(2, self->mIntcHandlerId);
    DisableDmac(1);
    RemoveDmacHandler(0, self->mDmacHandlerId);
    TerminateThread(self->mKickThreadId);
    DeleteThread(self->mKickThreadId);
    DeleteSema(self->mKickSema);
    DeleteSema(self->mQueueMutex);
    operator_delete((int*)self->mpDeferMan);
    func_00367360(self->mpTextureMan);
    operator_delete((int*)self->mpTextureMan);
    func_00361F40(D_005059D8);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetDisplayMode);
#ifdef SKIP_ASM
struct sPartTexEntA4 {
    char pad_0x00[0x88];
    int width;          // 0x88
    int height;         // 0x8C
    int psm;            // 0x90
    int clutPsm;        // 0x94
    int fmt;            // 0x98
    char pad_0x9C[4];
    int id;             // 0xA0
};

struct sPartTexCache {
    char pad_0x00[0x59C4];
    int count;              // 0x59C4
    char pad_0x59C8[4];
    sPartTexEntA4* textures;     // 0x59CC
};

/**
 * Returns the index of the display mode matching a resolution and pixel formats, or -1.
 *
 * Walks the NumDisplayModes 0xA4-byte entries of DisplayModes. An entry matches when its width (+0x88), height (+0x8C)
 * and id (+0xA0) equal @p width, @p height and @p id, its frame-buffer format (+0x90) fits @p bpp (32 -> 0, 24 -> 1, 16
 * -> 2), its second buffer format (+0x94) fits @p clutBpp the same way, and @p fmt is 24 with a z-buffer format (+0x98)
 * of 49 (PSMZ24). Likely bugs: case 16 of the @p bpp switch falls through into the 24 test, so a 16-bit request also
 * accepts 24-bit modes; and any @p fmt other than 24 can never match. Slot 123 of cPS2GraphicsMan_vtbl; cPS2GraphicsMan_SetDisplayMode
 * calls it.
 *
 * @param width  the screen width
 * @param height  the screen height
 * @param bpp  frame-buffer bits per pixel (16, 24 or 32)
 * @param clutBpp  bits per pixel of the second buffer format (16, 24 or 32)
 * @param fmt  z-buffer bits; only 24 can match
 * @param id  the mode id (+0xA0) to match
 * @return the matching mode index, or -1
 */
extern "C" int cPS2GraphicsMan_GetDisplayMode(cPS2GraphicsMan* self, int width, int height, unsigned int bpp,
                             unsigned int clutBpp, int fmt, int id)
{
    int i;
    sPartTexEntA4* displayMode = (sPartTexEntA4*)self->DisplayModes;
    for (i = 0; i < self->NumDisplayModes; i++, displayMode++) {
        if (displayMode->width == width && displayMode->height == height && displayMode->id == id) {
            int ok1 = 0;
            int ok2 = 0;
            int ok3 = 0;
            switch (bpp) {
            case 16:
                if (displayMode->psm == 2) ok1 = 1; else ok1 = 0;
            case 24:
                if (displayMode->psm == 1) ok1 = 1;
                break;
            case 32:
                if (displayMode->psm == 0) ok1 = 1; else ok1 = 0;
                break;
            }
            switch (clutBpp) {
            case 16:
                if (displayMode->clutPsm == 2) ok2 = 1; else ok2 = 0;
                break;
            case 24:
                if (displayMode->clutPsm == 1) ok2 = 1; else ok2 = 0;
                break;
            case 32:
                if (displayMode->clutPsm == 0) ok2 = 1; else ok2 = 0;
                break;
            }
            if (fmt == 24) {
                if (displayMode->fmt == 49) ok3 = 1; else ok3 = 0;
            }
            if (ok1 && ok2 && ok3) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetDisplayMode);
#ifdef SKIP_ASM
struct sVEntry00376468 {
    short delta;
    short index;
    void* fn;
};

/**
 * Switches to the display mode that matches a resolution and pixel formats.
 *
 * Calls its own virtual slot 123 (cPS2GraphicsMan_GetDisplayMode) to find the mode, then slot 4 (cPS2GraphicsMan_SetDisplayMode1) with
 * the index. Declared with only @p self: the caller's width, height, format and id arguments are passed on untouched in
 * their registers to slot 123. Slot 122 of cPS2GraphicsMan_vtbl.
 *
 * @return slot 4's result (1), or 0 when no mode matches
 */
extern "C" void* cPS2GraphicsMan_SetDisplayMode(cPS2GraphicsMan* self)
{
    sVEntry00376468* vt = (sVEntry00376468*)self->mVtbl;
    int i = ((int (*)(void*))vt[123].fn)((char*)self + vt[123].delta);
    if (i >= 0) {
        vt = (sVEntry00376468*)self->mVtbl;
        return ((void* (*)(void*, int))vt[4].fn)((char*)self + vt[4].delta, i);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetDisplayMode1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPartTexEnt_3764C0 {
    char pad_0x00[0x80];
    unsigned int a1;    // 0x80
    unsigned int a2;    // 0x84
    unsigned int w;     // 0x88
    unsigned int h;     // 0x8C
    unsigned int psm;   // 0x90
    unsigned int psm2;  // 0x94
    unsigned int zpsm;  // 0x98
    bool flag;             // 0x9C
    int id;             // 0xA0
};

struct sPartTexCache_3764C0 {
    char pad_0x00[0x59C8];
    int cur;                         // 0x59C8
    sPartTexEnt_3764C0* textures;    // 0x59CC
};

int func_0037BD98_3764C0(void* self, unsigned int a1, unsigned int a2, unsigned int w, unsigned int h,
                         unsigned int psm, unsigned int psm2, unsigned int zpsm, short s0, short s1, short s2,
                         int flag) __asm__("func_0037BD98");

/**
 * Switches the display to mode @p idx of DisplayModes unless it is already CurrentDisplayMode.
 *
 * Calls func_0037BD98 with the mode record's words at +0x80 to +0x98 (two addresses, width, height and three pixel
 * formats), 0, the +0x9C flag and its inverse, and the id at +0xA0, then stores @p idx in CurrentDisplayMode. Slot 4 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param idx  the display-mode index (records of 0xA4 bytes)
 * @return 1, always
 */
extern "C" int cPS2GraphicsMan_SetDisplayMode1(sPartTexCache_3764C0* self, int idx)
{
    if (self->cur == idx) {
        return 1;
    }
    func_0037BD98_3764C0(self, self->textures[idx].a1, self->textures[idx].a2, self->textures[idx].w,
                         self->textures[idx].h, self->textures[idx].psm, self->textures[idx].psm2,
                         self->textures[idx].zpsm, 0, self->textures[idx].flag ? 1 : 0,
                         self->textures[idx].flag ? 0 : 1, self->textures[idx].id);
    self->cur = idx;
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetDisplayModeName);
#ifdef SKIP_ASM
/**
 * Returns the name of display mode @p mode: the start of DisplayModes[@p mode] (0xA4-byte entries). @p mode is not
 * range-checked.
 *
 * @param mode  the display mode index (On Tour mode)
 * @return the mode's name string
 */
extern "C" void* cPS2GraphicsMan_GetDisplayModeName(cPS2GraphicsMan* self, int mode)
{
    return (char*)self->DisplayModes + mode * 0xa4;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetDTV);
#ifdef SKIP_ASM
struct sPartVEntry122 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int, int, int, int);
};

/**
 * Switches the digital TV output: calls virtual slot 122 with a 512 x 448 mode (0x200, 0x1C0, 0x18, 0x20, 0x18) and
 * @p enable.
 *
 * @param enable  non-zero to enable DTV output (On Tour Enable)
 * @return the result of slot 122
 */
extern "C" int cPS2GraphicsMan_SetDTV(cPS2GraphicsMan* self, int enable)
{
    sPartVEntry122* vt = (sPartVEntry122*)self->mVtbl;
    return vt[122].fn((char*)self + vt[122].delta, 0x200, 0x1C0, 0x18, 0x20, 0x18, enable);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PushViewport);
#ifdef SKIP_ASM
struct sPtQ65B8 {
    float v[4];
} __attribute__((aligned(16)));

struct sPtMtx65B8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPtScis65B8 {
    unsigned short v[4];
};


struct sPtGfx65B8 {
    char pad_0x0[0x10DC];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    char* keys;                     // 0x18F0
    char pad_0x18F4[0x5780 - 0x18F4];
    sPtMtx65B8 m5780;               // 0x5780
    sPtMtx65B8 m57C0;               // 0x57C0
    sPtMtx65B8 m5800;               // 0x5800
    sPtQ65B8 q5840;                 // 0x5840
    sPtQ65B8 q5850;                 // 0x5850
    sPtMtx65B8 m5860;               // 0x5860
    sPtMtx65B8 m58A0;               // 0x58A0
    sPtQ65B8 q58E0;                 // 0x58E0
    sPtQ65B8 q58F0;                 // 0x58F0
    sPtScis65B8 scissor;            // 0x5900
    float f5908;                    // 0x5908
    float f590C;                    // 0x590C
    float f5910;                    // 0x5910
    char pad_0x5914[0xC];
    sPtQ65B8 q5920;                 // 0x5920
    char pad_0x5930[0x6AF0 - 0x5930];
    sPtMtx65B8 m6AF0;               // 0x6AF0
    char pad_0x6B30[0x40];
    sPtQ65B8 q6B70;                 // 0x6B70
    sPtQ65B8 q6B80;                 // 0x6B80
    char pad_0x6B90[0x6D20 - 0x6B90];
    sViewportStack views[1];           // 0x6D20
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtx65B8(sPtMtx65B8* dst, sPtMtx65B8* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(src), "r"(dst)
        : "memory");
}

extern "C" void cPS2GraphicsMan_sViewportStack_operatorE(sViewportStack* dst, sViewportStack* src);

/**
 * Pushes the viewport stack: saves the scissor, the DeferMan scissor index, the screen, world, view, guard-band,
 * view-projection and net matrices, the guard and screen scales and offsets, and the clip bounds (unk_6B70 /
 * unk_6B80) into mViewportStack[mViewportDepth], then increments mViewportDepth and copies the entry into the new top
 * (cPS2GraphicsMan_sViewportStack_operatorE). No overflow check.
 */
extern "C" void cPS2GraphicsMan_PushViewport(cPS2GraphicsMan* self)
{
    sViewportStack* viewportStack = &((&self->mViewportStack[0]))[self->mViewportDepth];
    (*(sPtScis65B8*)&viewportStack->mScissor[0]) = (*(sPtScis65B8*)&self->mScissor[0]);
    viewportStack->mDeferScissorIndex = *(int*)((char*)self->mpDeferMan + 0x69CC4);
    viewportStack->unk_024 = self->unk_590C;
    viewportStack->unk_028 = self->unk_5908;
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mScreenMatrix), &(*(sPtMtx65B8*)&self->mScreenMatrix));
    (*(sPtQ65B8*)&viewportStack->unk_030) = (*(sPtQ65B8*)&self->unk_6B70);
    (*(sPtQ65B8*)&viewportStack->unk_040) = (*(sPtQ65B8*)&self->unk_6B80);
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mWorld), &(*(sPtMtx65B8*)&self->mWorld));
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mView), &(*(sPtMtx65B8*)&self->mView));
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mGuardBandMatrix), &(*(sPtMtx65B8*)&self->mGuardBandMatrix));
    (*(sPtQ65B8*)&viewportStack->mGuardScale) = (*(sPtQ65B8*)&self->mGuardScale);
    (*(sPtQ65B8*)&viewportStack->mGuardOffset) = (*(sPtQ65B8*)&self->mGuardOffset);
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mViewProj), &(*(sPtMtx65B8*)&self->mViewProj));
    vu0CopyMtx65B8(&(*(sPtMtx65B8*)&viewportStack->mNetMatrix), &(*(sPtMtx65B8*)&self->mNetMatrix));
    (*(sPtQ65B8*)&viewportStack->mScreenScale) = (*(sPtQ65B8*)&self->mScreenScale);
    (*(sPtQ65B8*)&viewportStack->mScreenOffset) = (*(sPtQ65B8*)&self->mScreenOffset);
    viewportStack->unk_02C = self->unk_5910;
    (*(sPtQ65B8*)&viewportStack->unk_050) = (*(sPtQ65B8*)&self->unk_5920);
    self->mViewportDepth++;
    cPS2GraphicsMan_sViewportStack_operatorE(&((&self->mViewportStack[0]))[self->mViewportDepth], viewportStack);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PopViewport);
#ifdef SKIP_ASM
struct sPtQ6768 {
    float v[4];
} __attribute__((aligned(16)));

struct sPtMtx6768 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPtScis6768 {
    unsigned short v[4];
};


struct sPtGfx6768 {
    char pad_0x0[0x10DC];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    char* keys;                     // 0x18F0
    char pad_0x18F4[0x5780 - 0x18F4];
    sPtMtx6768 m5780;               // 0x5780
    sPtMtx6768 m57C0;               // 0x57C0
    sPtMtx6768 m5800;               // 0x5800
    sPtQ6768 q5840;                 // 0x5840
    sPtQ6768 q5850;                 // 0x5850
    sPtMtx6768 m5860;               // 0x5860
    sPtMtx6768 m58A0;               // 0x58A0
    sPtQ6768 q58E0;                 // 0x58E0
    sPtQ6768 q58F0;                 // 0x58F0
    sPtScis6768 scissor;            // 0x5900
    float f5908;                    // 0x5908
    float f590C;                    // 0x590C
    float f5910;                    // 0x5910
    char pad_0x5914[0xC];
    sPtQ6768 q5920;                 // 0x5920
    char pad_0x5930[0x6AF0 - 0x5930];
    sPtMtx6768 m6AF0;               // 0x6AF0
    char pad_0x6B30[0x40];
    sPtQ6768 q6B70;                 // 0x6B70
    sPtQ6768 q6B80;                 // 0x6B80
    int i6B90;                      // 0x6B90
    char pad_0x6B94[0x6D20 - 0x6B94];
    sViewportStack views[1];           // 0x6D20
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtx6768(sPtMtx6768* dst, sPtMtx6768* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(src), "r"(dst)
        : "memory");
}

extern "C" void func_00364B88(void* table, void* key);

/**
 * Pops the viewport stack: decrements mViewportDepth and restores everything PushViewport saved.
 *
 * The scissor is re-registered with DeferMan (func_00364B88) when the saved scissor index is -1, otherwise the index
 * is put back. Clears mMatricesValid.
 */
extern "C" void cPS2GraphicsMan_PopViewport(cPS2GraphicsMan* self)
{
    self->mViewportDepth--;
    sViewportStack* viewportStack = &((&self->mViewportStack[0]))[self->mViewportDepth];
    (*(sPtScis6768*)&self->mScissor[0]) = (*(sPtScis6768*)&viewportStack->mScissor[0]);
    if (viewportStack->mDeferScissorIndex == -1) {
        func_00364B88((char*)self->mpDeferMan, &(*(sPtScis6768*)&self->mScissor[0]));
    } else {
        *(int*)((char*)self->mpDeferMan + 0x69CC4) = viewportStack->mDeferScissorIndex;
    }
    self->unk_590C = viewportStack->unk_024;
    self->unk_5908 = viewportStack->unk_028;
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mScreenMatrix), &(*(sPtMtx6768*)&viewportStack->mScreenMatrix));
    (*(sPtQ6768*)&self->unk_6B70) = (*(sPtQ6768*)&viewportStack->unk_030);
    (*(sPtQ6768*)&self->unk_6B80) = (*(sPtQ6768*)&viewportStack->unk_040);
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mWorld), &(*(sPtMtx6768*)&viewportStack->mWorld));
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mView), &(*(sPtMtx6768*)&viewportStack->mView));
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mGuardBandMatrix), &(*(sPtMtx6768*)&viewportStack->mGuardBandMatrix));
    (*(sPtQ6768*)&self->mGuardScale) = (*(sPtQ6768*)&viewportStack->mGuardScale);
    (*(sPtQ6768*)&self->mGuardOffset) = (*(sPtQ6768*)&viewportStack->mGuardOffset);
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mViewProj), &(*(sPtMtx6768*)&viewportStack->mViewProj));
    vu0CopyMtx6768(&(*(sPtMtx6768*)&self->mNetMatrix), &(*(sPtMtx6768*)&viewportStack->mNetMatrix));
    (*(sPtQ6768*)&self->mScreenScale) = (*(sPtQ6768*)&viewportStack->mScreenScale);
    (*(sPtQ6768*)&self->mScreenOffset) = (*(sPtQ6768*)&viewportStack->mScreenOffset);
    self->unk_5910 = viewportStack->unk_02C;
    (*(sPtQ6768*)&self->unk_5920) = (*(sPtQ6768*)&viewportStack->unk_050);
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00376938);
#ifdef SKIP_ASM
struct sPtVEntryI6938 {
    short delta;
    short index;
    int (*fn)(void*);
};


/**
 * Clips a rectangle to the screen and stores it as the scissor box (mScissor).
 *
 * Gets the screen size from the renderer's virtual slots 10 and 11 (cPS2GraphicsMan_GetScreenWidth/Height). A negative
 * @p x or @p y is moved to 0 and the width or height shrinks by the same amount; mScissor[0] and [1] become the origin
 * clamped to width - 1 / height - 1, and [2] and [3] the size clamped to what is left of the screen (0 when negative).
 * cPS2GraphicsMan_SetViewportWindow and cPS2GraphicsMan_SetViewportPerspective call it.
 *
 * @param pS2GraphicsMan  the renderer (this)
 * @param x  the left edge in pixels
 * @param y  the top edge in pixels
 * @param w  the width in pixels
 * @param rectHeight  the height in pixels
 */
extern "C" void func_00376938(cPS2GraphicsMan* pS2GraphicsMan, float x, float y, float w, float rectHeight)
{
    cPS2GraphicsMan* self = pS2GraphicsMan;
    sPtVEntryI6938* vtEntry = &((sPtVEntryI6938*)self->mVtbl)[10];
    int width = vtEntry->fn((char*)self + vtEntry->delta);
    vtEntry = &((sPtVEntryI6938*)self->mVtbl)[11];
    int height = vtEntry->fn((char*)self + vtEntry->delta);
    if (x < 0.0f) {
        w += x;
        x = 0.0f;
    }
    if (y < 0.0f) {
        rectHeight += y;
        y = 0.0f;
    }
    int ix = (int)x;
    self->mScissor[0] = ix < width - 1 ? ix : width - 1;
    int iy = (int)y;
    self->mScissor[1] = iy < height - 1 ? iy : height - 1;
    int iw = (int)w;
    int maxw = width - self->mScissor[0];
    int rw;
    if (iw >= 0) {
        rw = maxw < iw ? maxw : iw;
    } else {
        rw = 0;
    }
    self->mScissor[2] = rw;
    int ih = (int)rectHeight;
    int maxh = height - self->mScissor[1];
    int rh;
    if (ih >= 0) {
        rh = maxh < ih ? maxh : ih;
    } else {
        rh = 0;
    }
    self->mScissor[3] = rh;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetViewportWindow);
#ifdef SKIP_ASM
struct sPtVEntryI6A70 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sPtView6A70 {
    float x, y, w, h;               // 0x6D20 in the first view
    char pad_0x10[0x220];
};

struct sPtGfx6A70 {
    char pad_0x0[0x10D8];
    sPtVEntryI6A70* vtable;         // 0x10D8
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    void* keys;                     // 0x18F0
    char pad_0x18F4[0x5900 - 0x18F4];
    unsigned short scissor[4];      // 0x5900
    char pad_0x5908[0x6B94 - 0x5908];
    int clampMode;                  // 0x6B94
    float clampTop;                 // 0x6B98
    float clampBottom;              // 0x6B9C
    char pad_0x6BA0[0x6D20 - 0x6BA0];
    sPtView6A70 views[1];           // 0x6D20
};

extern "C" void func_00376938(cPS2GraphicsMan* self, float x, float y, float w, float h);
extern "C" void func_00364B88(void* table, void* key);

/**
 * Sets the current viewport rectangle, kept inside the TV safe band, and updates the scissor.
 *
 * When mTVDisplayFormat.mDisplayFormat is 0 to 2, @p y is raised to at least mfTVYOffset * the slot 11 screen height
 * and @p rectHeight is limited to mfTVYScale * that height (the height, not the bottom edge, so a raised top moves the
 * bottom down). Stores the rectangle in mViewportStack[mViewportDepth], then calls func_00376938 and
 * func_00364B88(mpDeferMan, mScissor). Slot 23 of cPS2GraphicsMan_vtbl.
 *
 * @param x  the left edge
 * @param y  the top edge
 * @param w  the width
 * @param rectHeight  the height
 */
extern "C" void cPS2GraphicsMan_SetViewportWindow(sPtGfx6A70* self, float x, float y, float w, float rectHeight)
{
    sPtVEntryI6A70* vtEntry = &self->vtable[11];
    int height = vtEntry->fn((char*)self + vtEntry->delta);
    int mode = self->clampMode;
    if (mode < 3) {
        if (mode >= 0) {
            float fh = (float)height;
            float top = self->clampTop * fh;
            if (y < top) {
                y = top;
            }
            float bottom = self->clampBottom * fh;
            if (bottom < rectHeight) {
                rectHeight = bottom;
            }
        }
    }
    self->views[self->curView].x = x;
    self->views[self->curView].y = y;
    self->views[self->curView].w = w;
    self->views[self->curView].h = rectHeight;
    func_00376938((cPS2GraphicsMan*)self, x, y, w, rectHeight);
    func_00364B88(self->keys, self->scissor);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetViewportWindow);
#ifdef SKIP_ASM
struct sPartVec4i {
    float x, y, z, w;
};

/**
 * Returns the current viewport's window rectangle.
 *
 * Reads the four floats at the start of mViewportStack[mViewportDepth] (0x230-byte entries from +0x6D20; depth at
 * +0x10DC), truncates each to an integer and stores them back as floats in @p out. The truncation means a fractional
 * window comes back rounded toward zero.
 *
 * @param out  the rectangle to fill (the hidden struct-return pointer)
 * @return @p out
 */
extern "C" sPartVec4i* cPS2GraphicsMan_GetViewportWindow(sPartVec4i* out, void* self)
{
    int i = *(int*)((char*)self + 0x10dc);
    float* window = (float*)((char*)self + (i * 0x230 + 0x6d20));
    int x = (int)window[0];
    int y = (int)window[1];
    int z = (int)window[2];
    int w = (int)window[3];
    out->x = (float)x;
    out->y = (float)y;
    out->z = (float)z;
    out->w = (float)w;
    return out;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetViewportFOV);
#ifdef SKIP_ASM
/**
 * Returns the current viewport's field of view (the float at +0x6D38 in viewport stack entry mViewportDepth, 0x230
 * bytes each).
 *
 * @return the field of view
 */
extern "C" float cPS2GraphicsMan_GetViewportFOV(cPS2GraphicsMan* self)
{
    int i = self->mViewportDepth;
    char* stackEntry = (char*)self + i * 0x230;
    return *(float*)(stackEntry + 0x6d38);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetViewportPerspective);
#ifdef SKIP_ASM

struct sPartSys {
    char pad[0x10dc];
    int cur;
    char pad2[0x6d20 - 0x10e0];
    sViewportStack emitters[1];
};

/**
 * Reads the current viewport's perspective (On Tour GetViewportPerspective(float& fov, float& minz, float& maxz)).
 *
 * The viewport entry stores them as unk_010[0] = near, [1] = far, [2] = field of view.
 *
 * @param fov  receives the field of view (unk_010[2])
 * @param minz  receives the near distance (unk_010[0])
 * @param maxz  receives the far distance (unk_010[1])
 */
extern "C" void cPS2GraphicsMan_GetViewportPerspective(cPS2GraphicsMan* self, float* fov, float* minz, float* maxz)
{
    sViewportStack* viewportStack = &((&self->mViewportStack[0]))[self->mViewportDepth];
    *fov = viewportStack->unk_010[2];
    *minz = viewportStack->unk_010[0];
    *maxz = viewportStack->unk_010[1];
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetViewportPerspective);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 4x4 float matrix (POD, 16-byte aligned).
struct sPtMatrix_6C58 {
    float m[4][4];
} __attribute__((aligned(16)));

// Four floats as an aligned array (indexed access; same bytes as t4Vector).
struct sPtQuadF_6C58 {
    float v[4];
} __attribute__((aligned(16)));

struct sPtView6C58 {
    float x, y, w, h;               // 0x00
    float zNear;                    // 0x10
    float zFar;                     // 0x14
    float fov;                      // 0x18
    float depth;                    // 0x1C
    int f20;                        // 0x20
    char pad_0x24[0x230 - 0x24];
};

struct sPtVtU6C58 {
    short delta;
    short index;
    unsigned (*fn)(void*);
};

struct sPtGfx6C58 {
    char pad_0x0[0x10D8];
    sPtVtU6C58* vtable;             // 0x10D8
    int curView;                    // 0x10DC
    char pad_0x10E0[0x57C0 - 0x10E0];
    sPtMatrix_6C58 m57C0;                // 0x57C0
    char pad_0x5800[0x5840 - 0x5800];
    sPtQuadF_6C58 q5840;                 // 0x5840
    sPtQuadF_6C58 q5850;                 // 0x5850
    sPtMatrix_6C58 m5860;                // 0x5860
    char pad_0x58A0[0x58E0 - 0x58A0];
    sPtQuadF_6C58 q58E0;                 // 0x58E0
    sPtQuadF_6C58 q58F0;                 // 0x58F0
    char pad_0x5900[0x8];
    float fov;                      // 0x5908
    float zFar;                     // 0x590C
    float f5910;                    // 0x5910
    char pad_0x5914[0xC];
    sPtQuadF_6C58 q5920;                 // 0x5920
    char pad_0x5930[0x5A40 - 0x5930];
    int mode;                       // 0x5A40
    char pad_0x5A44[0x6AF0 - 0x5A44];
    sPtMatrix_6C58 m6AF0;                // 0x6AF0
    char pad_0x6B30[0x40];
    sPtQuadF_6C58 q6B70;                 // 0x6B70
    sPtQuadF_6C58 q6B80;                 // 0x6B80
    int texReady;                   // 0x6B90
    int clampMode;                  // 0x6B94
    float clampTop;                 // 0x6B98
    float clampBottom;              // 0x6B9C
    float scaleX;                   // 0x6BA0
    float scaleY;                   // 0x6BA4
    char pad_0x6BA8[0x6D20 - 0x6BA8];
    sPtView6C58 views[1];           // 0x6D20
};

// PORT: the unit declares fSinCos as (s, c, angle); its real parameter order is (angle, s, c).
extern void SinCos_6C58(float angle, float* s, float* c) __asm__("fSinCos");
extern "C" float tanf(float x);
extern "C" void func_00376938_6C58(void* self, float x, float y, float w, float h) __asm__("func_00376938");

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fsAbs_6C58(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Scales a projection pair by the active letterbox mode's factors.
static inline void ClampScale6C58(sPtGfx6C58* self, float& x, float& y)
{
    int mode = self->clampMode;
    if (mode < 3) {
        if (mode >= 0) {
            x *= self->scaleX;
            y *= self->scaleY;
        }
    }
}

/**
 * Sets a perspective projection with field-of-view angle @p fov for the current viewport.
 *
 * Marks the stack entry perspective (+0x20 = 0) and stores @p zNear, @p zFar and @p fov in it and in unk_590C/unk_5908.
 * The viewport rectangle comes from mViewportStack[mViewportDepth] (x, y, w, h); the depth range is 2^24 - 1 when
 * mCurrentZBFormat is 0x31 (24-bit Z), else 65535, and offX/offY = 32768 - 8 * the slot 10/11 screen width/height place
 * the screen in the GS's 4096 space (1/16 pixels). The horizontal scale is (w / 2) / tan(@p fov) and the vertical one
 * that times the screen's height * 4/3 / width; when mTVDisplayFormat.mDisplayFormat is 0 to 2 both are multiplied by
 * mfXAspectAdjust and mfYAspectAdjust.
 *
 * It writes mScreenMatrix (with the entry's depth bias at +0x1C in the z offset), the screen bounds unk_6B70/unk_6B80
 * (w = 1 / zNear and 1 / zFar), mViewProj, mScreenScale/mScreenOffset and the guard-band
 * mView/mGuardScale/mGuardOffset, clears mMatricesValid, sets the scissor (func_00376938), unk_5910 = 40000 and
 * unk_5920 = (1023, ..., 1023 / 40000). Slot 25 of cPS2GraphicsMan_vtbl.
 *
 * @param fov  the field-of-view angle (radians) whose tangent sets the scale
 * @param zNear  the near plane
 * @param zFar  the far plane
 */
extern "C" void cPS2GraphicsMan_SetViewportPerspective(sPtGfx6C58* self, float fov, float zNear, float zFar)
{
    sPtView6C58* viewEntry = &self->views[self->curView];
    viewEntry->f20 = 0;
    viewEntry->zNear = zNear;
    viewEntry->zFar = zFar;
    viewEntry->fov = fov;
    // PORT: int-arithmetic view address (as in cPS2GraphicsMan_GetViewportFOV); the typed self->views[i] reads score 94-95%.
    char* viewBase = (char*)self + self->curView * 0x230;
    float x = *(float*)(viewBase + 0x6D20);
    float y = *(float*)(viewBase + 0x6D24);
    float w = *(float*)(viewBase + 0x6D28);
    float viewHeight = *(float*)(viewBase + 0x6D2C);
    float zlo, zhi, zscale;
    if (self->mode == 49) {
        zscale = 16777215.0f;
        zlo = -8388467.5f;
        zhi = 8388467.5f;
    } else {
        zscale = 65535.0f;
        zlo = -32767.5f;
        zhi = 32767.5f;
    }
    self->zFar = zFar;
    self->fov = fov;
    sPtVtU6C58* vtEntry = &self->vtable[11];
    unsigned height = vtEntry->fn((char*)self + vtEntry->delta);
    vtEntry = &self->vtable[10];
    unsigned width = vtEntry->fn((char*)self + vtEntry->delta);
    float aspect = (float)height * 1.3333334f / (float)width;
    float sinFov, cosFov;
    SinCos_6C58(fov, &sinFov, &cosFov);
    float sx = (w * 0.5f) / (sinFov / cosFov);
    float sy = sx * aspect;
    ClampScale6C58(self, sx, sy);
    vtEntry = &self->vtable[10];
    float offX = 32768.0f - (float)(vtEntry->fn((char*)self + vtEntry->delta) * 8);
    vtEntry = &self->vtable[11];
    float offY = 32768.0f - (float)(vtEntry->fn((char*)self + vtEntry->delta) * 8);
    float zA = (-zscale * zNear) / (zFar - zNear);
    self->m6AF0.m[0][0] = sx * 16.0f;
    self->m6AF0.m[1][0] = 0.0f;
    self->m6AF0.m[2][0] = (w * 0.5f + x) * 16.0f + offX;
    self->m6AF0.m[3][0] = 0.0f;
    self->m6AF0.m[0][1] = 0.0f;
    self->m6AF0.m[1][1] = -sy * 16.0f;
    self->m6AF0.m[2][1] = (viewHeight * 0.5f + y) * 16.0f + offY;
    self->m6AF0.m[3][1] = 0.0f;
    self->m6AF0.m[0][2] = 0.0f;
    self->m6AF0.m[1][2] = 0.0f;
    self->m6AF0.m[2][2] = zA;
    float zB = -zFar * zA + viewEntry->depth;
    self->m6AF0.m[3][2] = zB;
    self->m6AF0.m[0][3] = 0.0f;
    self->m6AF0.m[1][3] = 0.0f;
    self->m6AF0.m[2][3] = 1.0f;
    self->m6AF0.m[3][3] = 0.0f;
    self->q6B70.v[0] = x * 16.0f + offX;
    self->q6B70.v[1] = y * 16.0f + offY;
    self->q6B70.v[2] = zA * zNear + zB;
    self->q6B80.v[0] = (x + w) * 16.0f + offX;
    self->q6B80.v[1] = (y + viewHeight) * 16.0f + offY;
    self->q6B80.v[2] = zA * zFar + zB;
    self->q6B70.v[3] = 1.0f / zNear;
    self->q6B80.v[3] = 1.0f / zFar;
    self->texReady = 0;
    float aspect2 = aspect * (w / viewHeight);
    float px = 1.0f / tanf(fov);
    float py = 1.0f / tanf(fov) * aspect2;
    ClampScale6C58(self, px, py);
    vtEntry = &self->vtable[10];
    float sw = (float)vtEntry->fn((char*)self + vtEntry->delta);
    float cx = (w * 0.5f - sw * 0.5f) + x;
    vtEntry = &self->vtable[11];
    float sh = (float)vtEntry->fn((char*)self + vtEntry->delta);
    float cy = (viewHeight * 0.5f - sh * 0.5f) + y;
    float zz = (zFar + zNear) / (zFar - zNear);
    float zw = -(2.0f * zFar * zNear) / (zFar - zNear);
    self->m5860.m[0][0] = px;
    self->m5860.m[0][1] = 0.0f;
    self->m5860.m[0][2] = 0.0f;
    self->m5860.m[0][3] = 0.0f;
    self->m5860.m[1][0] = 0.0f;
    self->m5860.m[1][1] = py;
    self->m5860.m[1][2] = 0.0f;
    self->m5860.m[1][3] = 0.0f;
    self->m5860.m[2][0] = 0.0f;
    self->m5860.m[2][1] = 0.0f;
    self->m5860.m[2][2] = zz;
    self->m5860.m[2][3] = 1.0f;
    self->m5860.m[3][0] = 0.0f;
    self->m5860.m[3][1] = 0.0f;
    self->m5860.m[3][2] = zw;
    self->m5860.m[3][3] = 0.0f;
    self->q58E0.v[0] = w * 0.5f;
    self->q58E0.v[1] = -(viewHeight * 0.5f);
    self->q58E0.v[2] = zlo;
    self->q58E0.v[3] = 0.0f;
    self->q58F0.v[0] = cx + 2047.5f;
    self->q58F0.v[1] = cy + 2047.5f;
    self->q58F0.v[2] = zhi + viewEntry->depth;
    self->q58F0.v[3] = 0.0f;
    float gw = 2048.0f - 2.0f * fsAbs_6C58(cx);
    float gh = 2048.0f - 2.0f * fsAbs_6C58(cy);
    self->m57C0.m[0][0] = (w / gw) * px;
    self->m57C0.m[0][1] = 0.0f;
    self->m57C0.m[0][2] = 0.0f;
    self->m57C0.m[0][3] = 0.0f;
    self->m57C0.m[1][0] = 0.0f;
    self->m57C0.m[1][1] = (viewHeight / gh) * py;
    self->m57C0.m[1][2] = 0.0f;
    self->m57C0.m[1][3] = 0.0f;
    self->m57C0.m[2][0] = 0.0f;
    self->m57C0.m[2][1] = 0.0f;
    self->m57C0.m[2][2] = zz;
    self->m57C0.m[2][3] = 1.0f;
    self->m57C0.m[3][0] = 0.0f;
    self->m57C0.m[3][1] = 0.0f;
    self->m57C0.m[3][2] = zw;
    self->m57C0.m[3][3] = 0.0f;
    self->q5840.v[0] = gw * 0.5f;
    self->q5840.v[1] = -(gh * 0.5f);
    self->q5840.v[2] = zlo;
    self->q5840.v[3] = 0.0f;
    self->q5850.v[0] = cx + 2047.5f;
    self->q5850.v[1] = cy + 2047.5f;
    self->q5850.v[2] = zhi + viewEntry->depth;
    self->q5850.v[3] = 0.0f;
    func_00376938_6C58(self, x, y, w, viewHeight);
    self->f5910 = 40000.0f;
    self->q5920.v[0] = 1023.0f;
    self->q5920.v[1] = 0.0f;
    self->q5920.v[2] = 0.0f;
    self->q5920.v[3] = 1023.0f / 40000.0f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetViewportZBias);
#ifdef SKIP_ASM
struct sPt7278 {
    float x, y, z, w;
    sPt7278() {}
    sPt7278(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sMat7278 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPtView7278 {
    char pad_0x0[0x1C];
    float depth;                    // 0x1C
    char pad_0x20[0x210];
};

struct sPtCam7278 {
    char pad_0x0[0x8];
    float x;                        // 0x8
    char pad_0xC[0xC];
    float y;                        // 0x18
    char pad_0x1C[0xC];
    float z;                        // 0x28
};

struct sPtGfx7278 {
    char pad_0x0[0x10DC];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x13E4 - 0x10E0];
    sPtCam7278* cam;                // 0x13E4
    char pad_0x13E8[0x5858 - 0x13E8];
    float f5858;                    // 0x5858
    char pad_0x585C[0x58A0 - 0x585C];
    sMat7278 m58A0;                 // 0x58A0
    char pad_0x58E0[0x8];
    float depthScale;               // 0x58E8
    char pad_0x58EC[0xC];
    float f58F8;                    // 0x58F8
    char pad_0x58FC[0x5A40 - 0x58FC];
    int mode;                       // 0x5A40
    char pad_0x5A44[0x6B90 - 0x5A44];
    int texReady;                   // 0x6B90
    char pad_0x6B94[0x6D20 - 0x6B94];
    sPtView7278 views[1];           // 0x6D20
};

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector); the PC port needs a C fallback.
static inline sPt7278 MulMat7278(const sMat7278* m, const sPt7278& v)
{
    sPt7278 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar); the PC port needs a C fallback.
static inline sPt7278 Scale7278(const sPt7278& v, float s)
{
    sPt7278 r;
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

// PORT: PS2-only VU0 inline asm (vector add); the PC port needs a C fallback.
static inline sPt7278 Add7278(const sPt7278& a, const sPt7278& b)
{
    sPt7278 r;
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

extern "C" void func_0037D968(cPS2GraphicsMan* self);

static inline float DepthOffset7278(sPtGfx7278* self, sPt7278* pos, float dist)
{
    if (dist != 0.0f) {
    sPt7278 dir(self->cam->x, self->cam->y, self->cam->z, 0.0f);
    sPt7278 a = MulMat7278(&self->m58A0, *pos);
    sPt7278 c = MulMat7278(&self->m58A0, Add7278(*pos, Scale7278(dir, dist)));
    float za = a.z / a.w;
    float zc = c.z / c.w;
    if (-1.0f <= za && za <= 1.0f && -1.0f <= zc && zc <= 1.0f) {
        return self->depthScale * (zc - za);
    }
    }
    return 0.0f;
}

/**
 * Sets a depth bias for the current viewport from a world position and a world-space distance.
 *
 * Updates the clip matrices first when they are stale. The offset comes from DepthOffset7278(); when it differs from
 * the viewport's stored depth it is saved there and added to 32767.5 (8388467.5 when the mode word is 49) to give the
 * two Z constants at +0x58F8 and +0x5858, and texReady is cleared so the state is re-sent.
 *
 * @param pos  the world position the bias is measured at
 * @param dist  the bias distance in world units
 */
extern "C" void cPS2GraphicsMan_SetViewportZBias(sPtGfx7278* self, sPt7278* pos, float dist)
{
    sPtView7278* view = &self->views[self->curView];
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    float depthOffset = DepthOffset7278(self, pos, dist);
    if (depthOffset != view->depth) {
        view->depth = depthOffset;
        float k = 32767.5f;
        if (self->mode == 49) {
            k = 8388467.5f;
        }
        self->f58F8 = k + depthOffset;
        self->f5858 = k + view->depth;
        self->texReady = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetViewportOrthographic);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 4x4 float matrix (POD, 16-byte aligned).
struct sPtMatrix_7458 {
    float m[4][4];
} __attribute__((aligned(16)));

// Four floats as an aligned array (indexed access; same bytes as t4Vector).
struct sPtQuadF_7458 {
    float v[4];
} __attribute__((aligned(16)));

// PORT: SN abs.s asm helper (PS2 FPU).
// Depth-buffer range of the current Z format (0x31 = 24-bit Z).
static inline void ZScale7458(sPtGfx6C58* self, float& zscale)
{
    if (self->mode == 0x31) {
        zscale = 16777215.0f;
    } else {
        zscale = 65535.0f;
    }
}

/**
 * Sets an orthographic projection mapping the box from (@p cx, @p cy), @p fx wide and @p fy high, onto the current
 * viewport.
 *
 * Marks the stack entry orthographic (+0x20 = 1) and stores @p zNear and @p zFar in it. The viewport rectangle comes
 * from mViewportStack[mViewportDepth] (x, y, w, h); the depth range is 2^24 - 1 when mCurrentZBFormat is 0x31 (24-bit
 * Z), else 65535, and offX/offY = 32768 - 8 * the slot 10/11 screen width/height place the screen in the GS's 4096
 * space (1/16 pixels). It writes mScreenMatrix (scale w / @p fx * 16 and h / @p fy * 16, depth -range / (zFar -
 * zNear)), the screen bounds unk_6B70/unk_6B80, mViewProj (2 / @p fx, 2 / @p fy, offset -1), mScreenScale and
 * mScreenOffset, and the guard-band mView, mGuardScale and mGuardOffset, and clears mMatricesValid.
 *
 * Then func_00376938 sets the scissor to the viewport, unk_5910 = 40000 and unk_5920 = (1023, 0, 0, 1023 / 40000). Slot
 * 26 of cPS2GraphicsMan_vtbl.
 *
 * @param cx  the box's left edge
 * @param cy  the box's top edge
 * @param fx  the box width
 * @param fy  the box height
 * @param zNear  the near depth
 * @param zFar  the far depth
 */
extern "C" void cPS2GraphicsMan_SetViewportOrthographic(sPtGfx6C58* self, float cx, float cy, float fx, float fy, float zNear, float zFar)
{
    sPtView6C58* viewEntry = &self->views[self->curView];
    viewEntry->f20 = 1;
    viewEntry->zNear = zNear;
    viewEntry->zFar = zFar;
    // PORT: int-arithmetic view address, as in the perspective sibling cPS2GraphicsMan_SetViewportPerspective.
    char* viewBase = (char*)self + self->curView * 0x230;
    float x = *(float*)(viewBase + 0x6D20);
    float y = *(float*)(viewBase + 0x6D24);
    float w = *(float*)(viewBase + 0x6D28);
    float viewHeight = *(float*)(viewBase + 0x6D2C);
    float zscale;
    ZScale7458(self, zscale);
    sPtVtU6C58* vtEntry = &self->vtable[10];
    float offX = 32768.0f - (float)(vtEntry->fn((char*)self + vtEntry->delta) * 8);
    vtEntry = &self->vtable[11];
    float offY = 32768.0f - (float)(vtEntry->fn((char*)self + vtEntry->delta) * 8);
    float sx = w / fx * 16.0f;
    float sy = viewHeight / fy * 16.0f;

    float zA = -zscale / (zFar - zNear);
    float zB = -zFar * zA;
    self->m6AF0.m[0][0] = sx;
    self->m6AF0.m[1][0] = 0.0f;
    self->m6AF0.m[2][0] = 0.0f;
    self->m6AF0.m[3][0] = x * 16.0f - sx * cx + offX;
    self->m6AF0.m[0][1] = 0.0f;
    self->m6AF0.m[1][1] = sy;
    self->m6AF0.m[2][1] = 0.0f;
    self->m6AF0.m[3][1] = y * 16.0f - sy * cy + offY;
    self->m6AF0.m[0][2] = 0.0f;
    self->m6AF0.m[1][2] = 0.0f;
    self->m6AF0.m[2][2] = zA;
    self->m6AF0.m[3][2] = zB;
    self->m6AF0.m[0][3] = 0.0f;
    self->m6AF0.m[1][3] = 0.0f;
    self->m6AF0.m[2][3] = 0.0f;
    self->m6AF0.m[3][3] = 1.0f;
    self->q6B70.v[0] = x * 16.0f + offX;
    self->q6B70.v[1] = y * 16.0f + offY;
    self->q6B70.v[2] = zA * zNear + zB;
    self->q6B70.v[3] = 1.0f;
    self->q6B80.v[0] = (x + w) * 16.0f + offX;
    self->q6B80.v[1] = (y + viewHeight) * 16.0f + offY;
    self->q6B80.v[2] = zA * zFar + zB;
    self->q6B80.v[3] = 1.0f;
    self->texReady = 0;
    float zlo, zhi;
    if (self->mode == 49) {
        zlo = -8388467.5f;
        zhi = 8388467.5f;
    } else {
        zlo = -32767.5f;
        zhi = 32767.5f;
    }
    float px = 2.0f / fx;
    float py = 2.0f / fy;
    vtEntry = &self->vtable[10];
    float sw = (float)vtEntry->fn((char*)self + vtEntry->delta);
    float ox = (w * 0.5f - sw * 0.5f) + x;
    vtEntry = &self->vtable[11];
    float sh = (float)vtEntry->fn((char*)self + vtEntry->delta);
    float oy = (viewHeight * 0.5f - sh * 0.5f) + y;
    float zz = 2.0f / (zFar - zNear);
    float zw = -(zFar + zNear) / (zFar - zNear);
    self->m5860.m[0][0] = px;
    self->m5860.m[0][1] = 0.0f;
    self->m5860.m[0][2] = 0.0f;
    self->m5860.m[0][3] = 0.0f;
    self->m5860.m[1][0] = 0.0f;
    self->m5860.m[1][1] = py;
    self->m5860.m[1][2] = 0.0f;
    self->m5860.m[1][3] = 0.0f;
    self->m5860.m[2][0] = 0.0f;
    self->m5860.m[2][1] = 0.0f;
    self->m5860.m[2][2] = zz;
    self->m5860.m[2][3] = 0.0f;
    self->m5860.m[3][0] = -1.0f;
    self->m5860.m[3][1] = -1.0f;
    self->m5860.m[3][2] = zw;
    self->m5860.m[3][3] = 1.0f;
    self->q58E0.v[0] = w * 0.5f;
    self->q58E0.v[1] = viewHeight * 0.5f;
    self->q58E0.v[2] = zlo;
    self->q58E0.v[3] = 0.0f;
    float tx = (ox + 2047.5f) - cx * w / fx;
    float ty = (oy + 2047.5f) - cy * viewHeight / fy;
    self->q58F0.v[0] = tx;
    self->q58F0.v[1] = ty;
    self->q58F0.v[2] = zhi;
    self->q58F0.v[3] = 0.0f;
    float gw = 2048.0f - 2.0f * fsAbs_6C58(ox);
    float gh = 2048.0f - 2.0f * fsAbs_6C58(oy);
    float gx = w / gw;
    float gy = viewHeight / gh;
    self->m57C0.m[0][0] = gx * px;
    self->m57C0.m[0][1] = 0.0f;
    self->m57C0.m[0][2] = 0.0f;
    self->m57C0.m[0][3] = 0.0f;
    self->m57C0.m[1][0] = 0.0f;
    self->m57C0.m[1][1] = gy * py;
    self->m57C0.m[1][2] = 0.0f;
    self->m57C0.m[1][3] = 0.0f;
    self->m57C0.m[2][0] = 0.0f;
    self->m57C0.m[2][1] = 0.0f;
    self->m57C0.m[2][2] = zz;
    self->m57C0.m[2][3] = 0.0f;
    self->m57C0.m[3][0] = -gx;
    self->m57C0.m[3][1] = -gy;
    self->m57C0.m[3][2] = zw;
    self->m57C0.m[3][3] = 1.0f;
    self->q5840.v[0] = gw * 0.5f;
    self->q5840.v[1] = gh * 0.5f;
    self->q5840.v[2] = zlo;
    self->q5840.v[3] = 0.0f;
    self->q5850.v[0] = tx;
    self->q5850.v[1] = ty;
    self->q5850.v[2] = zhi;
    self->q5850.v[3] = 0.0f;
    func_00376938((cPS2GraphicsMan*)self, x, y, w, viewHeight);
    self->f5910 = 40000.0f;
    self->q5920.v[0] = 1023.0f;
    self->q5920.v[1] = 0.0f;
    self->q5920.v[2] = 0.0f;
    self->q5920.v[3] = 1023.0f / 40000.0f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetTVDisplayFormat);
#ifdef SKIP_ASM
/**
 * Sets the TV display format (mTVDisplayFormat) from a format number.
 *
 * Stores @p mode in mDisplayFormat, then: 0 (On Tour GM_FULLSCREEN) sets mfTVYOffset 0 and mfTVYScale, mfXAspectAdjust,
 * mfYAspectAdjust 1; 1 (GM_WIDESCREEN, letterboxed) sets the offset 0.125 and the three scales 0.75; 2 (GM_ANIMORPHIC)
 * sets mfXAspectAdjust 0.75, the offset 0 and the other two 1. Other values change only mDisplayFormat. Slot 40 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param mode  the display format: 0 full screen, 1 widescreen (letterbox), 2 anamorphic
 */
extern "C" void cPS2GraphicsMan_SetTVDisplayFormat(cPS2GraphicsMan* self, int mode)
{
    self->mTVDisplayFormat.mDisplayFormat = mode;
    switch (mode) {
    case 0:
        self->mTVDisplayFormat.mfTVYOffset = 0.0f;
        self->mTVDisplayFormat.mfTVYScale = 1.0f;
        self->mTVDisplayFormat.mfXAspectAdjust = 1.0f;
        self->mTVDisplayFormat.mfYAspectAdjust = 1.0f;
        break;
    case 1:
        self->mTVDisplayFormat.mfTVYOffset = 0.125f;
        self->mTVDisplayFormat.mfTVYScale = 0.75f;
        self->mTVDisplayFormat.mfXAspectAdjust = 0.75f;
        self->mTVDisplayFormat.mfYAspectAdjust = 0.75f;
        break;
    case 2:
        self->mTVDisplayFormat.mfXAspectAdjust = 0.75f;
        self->mTVDisplayFormat.mfTVYOffset = 0.0f;
        self->mTVDisplayFormat.mfTVYScale = 1.0f;
        self->mTVDisplayFormat.mfYAspectAdjust = 1.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetTVDisplayFormat1);
#ifdef SKIP_ASM

/**
 * Sets the TV display format by copying the 0x14-byte tTVDisplayFormat @p src into mTVDisplayFormat.
 *
 * Slot 41 of cPS2GraphicsMan_vtbl.
 *
 * @param src  the format to copy (mode, y offset and scale, x and y aspect adjust)
 */
extern "C" void cPS2GraphicsMan_SetTVDisplayFormat1(cPS2GraphicsMan* self, tTVDisplayFormat* src)
{
    *(&self->mTVDisplayFormat) = *src;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_7A10 {
    float x, y, w, h;
    sRect_7A10(float ax, float ay, float aw, float ah) : x(ax), y(ay), w(aw), h(ah) {}
};

struct sPtRS_7A10 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

extern sPtRS_7A10 D_00501420_rs7A10 __asm__("sDrawState_s_defaultDrawState");
extern char D_005059D8[];
extern "C" void func_00363490(void* ring, unsigned int* a, int* x, unsigned int* b, int* y);
extern "C" void* WaitSema(int sema);  // WaitSema (returns in $2: the call clobbers v0)
extern "C" void SignalSema(int sema);
extern "C" void InvalidDCache(unsigned int start, unsigned int end);
extern "C" void func_00362CC8(void* ring);
void func_003673D0(void* p);
void func_00361F90(void* p);

// PS2GraphicsMan (vptr at +0x10D8, old-style g++ 2.x vtable entries)
class cGfx_7A10 {
public:
    char pad_0x0[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void EndFrame();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void SetViewport(const sRect_7A10& r, int a);
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void DrawClear();

    void EndFrame_impl() __asm__("cPS2GraphicsMan_DrawFrame");
    void ResetViewport();
};

// old-style g++ 2.x vtable entry
struct sVEnt_7A10 {
    short delta;
    short index;
    void (*fn)(void*, const sRect_7A10&, int);
};

inline void cGfx_7A10::ResetViewport()
{
    sVEnt_7A10* vt = *(sVEnt_7A10**)((char*)this + 0x10D8);
    void (**pf)(void*, const sRect_7A10&, int) = &vt[24].fn;
    (*pf)((char*)this + vt[24].delta, sRect_7A10(0.0f, 0.0f, (float)GetWidth(), (float)GetHeight()), 1);
}

/**
 * Ends the frame: queues this frame's packets for the kick thread and switches to the other packet buffer.
 *
 * Defined in C as cGfx_7A10::EndFrame_impl(). It calls func_00363490 on the render ring, waits on mQueueMutex
 * (+0x5ACC), stores the packet-buffer words at +0x5AA4..+0x5AB4, signals mQueueMutex and mKickSema (+0x5AC8), swaps the
 * double-buffered packet pointers around +0x5A00..+0x5A24, calls InvalidDCache on the new buffer ranges, resets the
 * draw state to sDrawState_s_defaultDrawState and calls func_00362CC8 on the ring.
 */
void cGfx_7A10::EndFrame_impl()
{
    char* gfxBase = (char*)this;
    ResetViewport();
    DrawClear();
    unsigned int vif1 = *(unsigned int*)(gfxBase + 0x5A00);
    unsigned int gif1 = *(unsigned int*)(gfxBase + 0x5A14);
    int vif2;
    int gif2;
    func_00363490(*(void**)(gfxBase + 0x18F0), (unsigned int*)(gfxBase + 0x5A00), &vif2, (unsigned int*)(gfxBase + 0x5A14), &gif2);
    WaitSema(*(int*)(gfxBase + 0x5ACC));
    // bufState[2] (+0x5A90) and queuedIndex (+0x5AA4) are shared with the DMA kick thread
    // (func_00382760) and the VIF1 DMA interrupt: volatile, like func_003826E0's bufState.
    volatile int* bufState = (volatile int*)(gfxBase + 0x5A90);
    *(volatile int*)(gfxBase + 0x5AA4) = *(int*)(gfxBase + 0x5A10);
    volatile int* state = &bufState[*(volatile int*)(gfxBase + 0x5AA4)];
    *(unsigned int*)(gfxBase + 0x5AA8) = vif1;
    *(int*)(gfxBase + 0x5AAC) = vif2;
    *(unsigned int*)(gfxBase + 0x5AB0) = gif1;
    *(int*)(gfxBase + 0x5AB4) = gif2;
    *state = 2;
    SignalSema(*(int*)(gfxBase + 0x5ACC));
    SignalSema(*(int*)(gfxBase + 0x5AC8));
    *(int*)(gfxBase + 0x5A10) = *(int*)(gfxBase + 0x5A10) == 0;
    *(int*)(gfxBase + 0x5A24) = *(int*)(gfxBase + 0x5A24) == 0;
    *(unsigned int*)(gfxBase + 0x5A00) = *(unsigned int*)(gfxBase + 0x5A04) = *(unsigned int*)(gfxBase + 0x5A0C) + *(int*)(gfxBase + 0x5A10) * 0xCCC80;
    *(unsigned int*)(gfxBase + 0x5A14) = *(unsigned int*)(gfxBase + 0x5A18) = *(unsigned int*)(gfxBase + 0x5A20) + *(int*)(gfxBase + 0x5A24) * 0x26600;
    InvalidDCache(*(unsigned int*)(gfxBase + 0x5A00), *(unsigned int*)(gfxBase + 0x5A00) + 0xCCC80);
    InvalidDCache(*(unsigned int*)(gfxBase + 0x5A14), *(unsigned int*)(gfxBase + 0x5A14) + 0x26600);
    **(sPtRS_7A10**)(gfxBase + 0xE84) = D_00501420_rs7A10;
    func_00362CC8(*(void**)(gfxBase + 0x18F0));
    func_003673D0(*(void**)(gfxBase + 0x18F4));
    ResetViewport();
    func_00361F90(D_005059D8);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawSprite);
#ifdef SKIP_ASM
// PORT: 128-bit GPR quadword (TImode); the PC port needs a 16-byte struct.
typedef int cQuad_7CF0 __attribute__((mode(TI)));

struct sV_7CF0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_7CF0 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sRS_7CF0 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

struct sEnt_7CF0 {
    sRS_7CF0 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sRing_7CF0 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_7CF0 ents[1];              // 0x80
};

struct sGfx_7CF0 {
    char pad_0x0[0xE84];
    sRS_7CF0* top;                  // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_7CF0* ring;               // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6AF0 - 0x5A04];
    sMat_7CF0 projMtx;              // 0x6AF0
    sMat_7CF0 clipMtx;              // 0x6B30
    float minX;                     // 0x6B70
    float minY;                     // 0x6B74
    float maxZ;                     // 0x6B78
    float field_0x6B7C;
    float maxX;                     // 0x6B80
    float maxY;                     // 0x6B84
    float minZ;                     // 0x6B88
    float field_0x6B8C;
    int clipValid;                  // 0x6B90
};

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sV_7CF0 mtxApply_7CF0(sMat_7CF0* m, const sV_7CF0& v)
{
    sV_7CF0 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV_7CF0 Add_7CF0(const sV_7CF0& a, const sV_7CF0& b)
{
    sV_7CF0 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV_7CF0 Sub_7CF0(const sV_7CF0& a, const sV_7CF0& b)
{
    sV_7CF0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// Guard-band outcode of a projected point (as cPS2GraphicsMan_GetOutCode).
static inline int ClipCode_7CF0(sGfx_7CF0* self, const sV_7CF0& v)
{
    int code = 0;
    if (v.x < self->minX) {
        code |= 2;
    }
    if (self->maxX < v.x) {
        code |= 1;
    }
    if (v.y < self->minY) {
        code |= 8;
    }
    if (self->maxY < v.y) {
        code |= 4;
    }
    if (v.w < 0.0f) {
        code ^= 0x2F;
    } else {
        if (v.z > self->maxZ) {
            code |= 0x20;
        }
        if (v.z < self->minZ) {
            code |= 0x10;
        }
    }
    return code;
}

/**
 * Draws a screen-facing textured sprite centred on a world position.
 *
 * Projects @p pos with the clip matrix, divides by w, scales @p size by the projection's x and y scale, and returns
 * early when both corners fall outside the same clip plane; otherwise it emits the sprite with the given UVs and
 * colour.
 *
 * @param pos  the sprite centre in world space
 * @param size  the half width and half height
 * @param uv0  the top-left texture coordinate
 * @param uv1  the bottom-right texture coordinate
 * @param col  the colour
 */
// PORT: uncached (0x30000000) pointers held in int.
// Emits one screen-aligned sprite (GS SPRITE, ST + RGBAQ + two XYZ2) centred on pos.
extern "C" void cPS2GraphicsMan_DrawSprite(sGfx_7CF0* self, sV_7CF0* pos, float* size, int* uv0, int* uv1, float* col)
{
    if (self->clipValid == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    sV_7CF0 clipPos = mtxApply_7CF0(&self->clipMtx, *pos);
    float sw = size[0];
    float sh = size[1];
    float w = 1.0f / clipPos.w;
    float sx = self->projMtx.m[0][0] * sw;
    float sy = self->projMtx.m[1][1] * sh;
    clipPos.x *= w;
    clipPos.y *= w;
    clipPos.w = w;
    sV_7CF0 halfSize;
    halfSize.x = sx * w;
    halfSize.y = sy * w;
    halfSize.z = 0.0f;
    halfSize.w = 0.0f;
    sV_7CF0 lo = Sub_7CF0(clipPos, halfSize);
    sV_7CF0 hi = Add_7CF0(clipPos, halfSize);
    if (ClipCode_7CF0(self, lo) & ClipCode_7CF0(self, hi)) {
        return;
    }

    float gsQ = 1.0f;
    unsigned int buf = self->bufAddr;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    *(cQuad_7CF0*)packet = 0x10000005;
    packet[2] = 0;
    packet[3] = (unsigned long)0x50000004 << 32;
    packet[4] = ((unsigned long)0x64000000 << 32) | 0x8001;
    packet[5] = 0x525120;
    packet[6] = 94;
    packet[7] = (unsigned long)uv0[0] | ((unsigned long)uv0[1] << 32);
    packet[8] = (unsigned long)(int)(col[1] * 128.0f) | ((unsigned long)(int)(col[2] * 128.0f) << 8) |
           ((unsigned long)(int)(col[3] * 128.0f) << 16) | ((unsigned long)(int)(col[0] * 128.0f) << 24) |
           ((unsigned long)*(unsigned int*)&gsQ << 32);
    packet[9] = (unsigned long)(int)lo.x | ((unsigned long)(int)lo.y << 16) | ((unsigned long)(int)(lo.z * lo.w) << 32);
    packet[10] = (unsigned long)uv1[0] | ((unsigned long)uv1[1] << 32);
    packet[11] = (unsigned long)(int)hi.x | ((unsigned long)(int)hi.y << 16) | ((unsigned long)(int)(hi.z * hi.w) << 32);
    packet += 12;
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    self->bufAddr = next;
    sRing_7CF0* ring = self->ring;
    sRS_7CF0* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_7CF0* ringEntry = (sEnt_7CF0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawRotatedSprite);
#ifdef SKIP_ASM
struct sV_81A0 {
    float x, y, z, w;
    sV_81A0() {}
    sV_81A0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sMat_81A0 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVEnt_81A0 {
    short delta;
    short index;
    sV_81A0 (*fn)(void*, const sV_81A0*);
};

struct sRS_81A0 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

struct sEnt_81A0 {
    sRS_81A0 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sRing_81A0 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_81A0 ents[1];              // 0x80
};

struct sGfx_81A0 {
    char pad_0x0[0xE84];
    sRS_81A0* top;                  // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sVEnt_81A0* vtable;             // 0x10D8
    char pad_0x10DC[0x13E4 - 0x10DC];
    sMat_81A0* cam;                 // 0x13E4
    char pad_0x13E8[0x18F0 - 0x13E8];
    sRing_81A0* ring;               // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A30 - 0x5A04];
    unsigned int width;             // 0x5A30
    unsigned int height;            // 0x5A34
    char pad_0x5A38[0x6B90 - 0x5A38];
    int texReady;                   // 0x6B90
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV_81A0 Scale_81A0(const sV_81A0& v, float s)
{
    sV_81A0 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV_81A0 Add_81A0(const sV_81A0& a, const sV_81A0& b)
{
    sV_81A0 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV_81A0 Sub_81A0(const sV_81A0& a, const sV_81A0& b)
{
    sV_81A0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void Rot_81A0(sV_81A0* out, const sMat_81A0* m, const sV_81A0& v)
{
    float x = v.x * m->m[0][0] + v.y * m->m[0][1];
    float y = v.x * m->m[1][0] + v.y * m->m[1][1];
    float z = v.x * m->m[2][0] + v.y * m->m[2][1];
    float w = v.x * m->m[3][0] + v.y * m->m[3][1];
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;
}

static inline sV_81A0 Project_81A0(sGfx_81A0* self, const sV_81A0* v)
{
    sVEnt_81A0* vt = self->vtable;
    return vt[92].fn((char*)self + vt[92].delta, v);
}

extern "C" void func_0037D968(cPS2GraphicsMan* self);
extern "C" void fSinCos(float* s, float* c, float angle);

/**
 * Draws a camera-facing textured sprite rotated about its centre.
 *
 * Builds the two corner axes from the sine and cosine of @p angle (fSinCos) in camera space, scales them by @p
 * size, projects the centre and both axis ends, and forms the four corners from the differences; @p count is the number
 * of passes.
 *
 * @param pos  the sprite centre in world space
 * @param size  the half width and half height
 * @param uv0  the top-left texture coordinate
 * @param uv1  the bottom-right texture coordinate
 * @param col  the colour
 * @param count  the number of passes (On Tour numPass)
 * @param angle  the rotation in radians
 */
// PORT: uncached (0x30000000) pointers held in int.
// Emits count camera-facing rotated sprites (GS tri-strip quads) centred on pos.
extern "C" void cPS2GraphicsMan_DrawRotatedSprite(sGfx_81A0* self, sV_81A0* pos, float* size, int* uv0, int* uv1, float* col, int count, float angle)
{
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    float gsQ = 1.0f;
    sV_81A0 center = Project_81A0(self, pos);
    float sinAngle, cosAngle;
    fSinCos(&sinAngle, &cosAngle, angle);
    sV_81A0 axisU;
    sV_81A0 axisV(cosAngle, sinAngle, 0.0f, 0.0f);
    Rot_81A0(&axisU, self->cam, axisV);
    Rot_81A0(&axisV, self->cam, sV_81A0(-axisV.y, axisV.x, axisV.z, axisV.w));
    sV_81A0 edgeU = Add_81A0(*pos, Scale_81A0(axisU, size[0]));
    sV_81A0 edgeV = Add_81A0(*pos, Scale_81A0(axisV, size[1]));
    edgeU.w = gsQ;
    edgeV.w = gsQ;
    sV_81A0 halfU = Sub_81A0(Project_81A0(self, &edgeU), center);
    sV_81A0 halfV = Sub_81A0(Project_81A0(self, &edgeV), center);
    sV_81A0 corner0 = Sub_81A0(Sub_81A0(center, halfU), halfV);
    sV_81A0 corner1 = Sub_81A0(Add_81A0(center, halfU), halfV);
    sV_81A0 corner2 = Add_81A0(Sub_81A0(center, halfU), halfV);
    sV_81A0 corner3 = Add_81A0(Add_81A0(center, halfU), halfV);

    unsigned int buf = self->bufAddr;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    int xoff = (0x800 - (self->width >> 1)) << 4;
    int yoff = (0x800 - (self->height >> 1)) << 4;
    int x0 = xoff + (int)(corner0.x * 16.0f);
    int y0 = yoff + (int)(corner0.y * 16.0f);
    int x1 = xoff + (int)(corner1.x * 16.0f);
    int y1 = yoff + (int)(corner1.y * 16.0f);
    int x2 = xoff + (int)(corner2.x * 16.0f);
    int y2 = yoff + (int)(corner2.y * 16.0f);
    int x3 = xoff + (int)(corner3.x * 16.0f);
    int y3 = yoff + (int)(corner3.y * 16.0f);
    int z = (int)center.z;
    int cr = (int)(col[1] * 128.0f);
    int cg = (int)(col[2] * 128.0f);
    int cb = (int)(col[3] * 128.0f);
    int ca = (int)(col[0] * 128.0f);
    for (int i = 0; i < count; i++) {
        packet[0] = 0x10000007;
        packet[1] = 0;
        packet[2] = 0;
        packet[3] = (unsigned long)0x50000006 << 32;
        packet[4] = ((unsigned long)0xA4000000 << 32) | 0x8001;
        packet[5] = 0x5252525210;
        packet[6] = 84;
        packet[7] = (unsigned long)cr | ((unsigned long)cg << 8) | ((unsigned long)cb << 16) | ((unsigned long)ca << 24) | ((unsigned long)*(unsigned int*)&gsQ << 32);
        packet[8] = (unsigned long)uv0[0] | ((unsigned long)uv0[1] << 32);
        packet[9] = (unsigned long)x0 | ((unsigned long)y0 << 16) | ((unsigned long)z << 32);
        packet[10] = (unsigned long)uv1[0] | ((unsigned long)uv0[1] << 32);
        packet[11] = (unsigned long)x1 | ((unsigned long)y1 << 16) | ((unsigned long)z << 32);
        packet[12] = (unsigned long)uv0[0] | ((unsigned long)uv1[1] << 32);
        packet[13] = (unsigned long)x2 | ((unsigned long)y2 << 16) | ((unsigned long)z << 32);
        packet[14] = (unsigned long)uv1[0] | ((unsigned long)uv1[1] << 32);
        packet[15] = (unsigned long)x3 | ((unsigned long)y3 << 16) | ((unsigned long)z << 32);
        packet += 16;
    }
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    self->bufAddr = next;
    sRing_81A0* ring = self->ring;
    sRS_81A0* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_81A0* ringEntry = (sEnt_81A0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawString3D);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
struct sPtRenderState {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

// One 0x80-byte entry of the graphics manager's draw ring (ring at 0x18F0): its render state, packet buffer, link
// and sort key.
struct sPtRingEnt {
    sPtRenderState rs;       // 0x00
    unsigned int buf;        // 0x14
    unsigned int next;       // 0x18
    short key;               // 0x1C
    short flag;              // 0x1E
    char pad_0x20[0x60];
};

// The graphics manager draw ring (cPS2GraphicsMan + 0x18F0): entry count, then the 0x80-byte entries from 0x80.
struct sPtRing {
    int count;               // 0x0
    char pad_0x4[0x7C];
    sPtRingEnt ents[1];      // 0x80
};

union uPtFloatBits {
    float f;
    int i;
};

typedef int cQuad128_CF0 __attribute__((mode(TI)));

// PORT: PS2-only VU0 inline asm (vector subtract); the PC port needs a C fallback.
static inline sPt7278 Sub_CF0(const sPt7278& a, const sPt7278& b)
{
    sPt7278 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline int Bits_CF0(float f)
{
    uPtFloatBits u;
    u.f = f;
    return u.i;
}
struct sGlyph8808 {
    unsigned short code;    // 0x0
    unsigned char w;        // 0x2
    unsigned char h;        // 0x3
    unsigned short u;       // 0x4
    unsigned short v;       // 0x6
    signed char adv;        // 0x8
    signed char xoff;       // 0x9
    signed char yoff;       // 0xA
    char pad;
};

struct sFontRes8808 {
    int count;              // 0x0
    sGlyph8808* table;      // 0x4
    sGlyph8808* def;        // 0x8
    int first;              // 0xC
    int last;               // 0x10
};

struct sView8808 {
    char pad_0x0[0x20];
    int mode;               // 0x20
    char pad_0x24[0x230 - 0x24];
};

struct sGfx8808 {
    char pad_0x0[0xE84];
    sPtRenderState* top;            // 0xE84
    char pad_0xE88[0x10DC - 0xE88];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    sPtRing* ring;                  // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6AF0 - 0x5A04];
    float f6AF0;                    // 0x6AF0
    char pad_0x6AF4[0x6B04 - 0x6AF4];
    float f6B04;                    // 0x6B04
    char pad_0x6B08[0x6B18 - 0x6B08];
    float f6B18;                    // 0x6B18
    char pad_0x6B1C[0x6B20 - 0x6B1C];
    float f6B20;                    // 0x6B20
    float f6B24;                    // 0x6B24
    float f6B28;                    // 0x6B28
    char pad_0x6B2C[0x6B30 - 0x6B2C];
    sMat7278 m6B30;                // 0x6B30
    char pad_0x6B70[0x6B90 - 0x6B70];
    int texReady;                   // 0x6B90
    char pad_0x6B94[0x6D20 - 0x6B94];
    sView8808 views[1];             // 0x6D20
};

// Homogeneous position (w given) with a const-reference ctor: its stores, w included, go straight to the stack
// slot, where the by-value sPt7278 ctor keeps `this` in a register for a constant w.
struct sPtPos8808 : sPt7278 {
    sPtPos8808(const float& ax, const float& ay, const float& az, const float& aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

// Stores a two-float pair (uv, half size); both values are computed before either store.
static inline void Set2_8808(float* d, float a, float b)
{
    d[0] = a;
    d[1] = b;
}

// Glyph lookup: direct index for [first, last], binary search above, default glyph if not found.
static inline sGlyph8808* Find8808(sFontRes8808* f, int c)
{
    if (c < f->first)
        return 0;
    if (c <= f->last)
        return f->table + c - f->first;
    int lo = f->last - f->first;
    int hi = f->count;
    do {
        int mid = (lo + hi) >> 1;
        sGlyph8808* p = f->table + mid;
        if (c == p->code)
            return p;
        if (c < p->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return f->def;
}

/**
 * Draws string @p text in @p font as one textured sprite per glyph, queued as a single GS packet.
 *
 * Rebuilds the matrices first (func_0037D968) when mMatricesValid is 0. Each character's glyph is found by code (direct
 * index from FirstCode to LastLinearCode, then a binary search, else MissingGlyph). When the current viewport is
 * orthographic (word +0x20 of mViewportStack[mViewportDepth] non-zero) the corners are scaled and offset by
 * mScreenMatrix directly; otherwise each glyph's centre goes through mScreenWorld with a perspective divide and the
 * half-size is scaled by 1/w. UVs are the glyph's u/v times @p su and @p sv, and @p x advances by the glyph advance
 * times @p sx.
 *
 * The packet at mVifCursor (uncached) holds a DMA cnt tag, a VIF DIRECT code, PRIM 0x56 (sprite, textured, blended)
 * with RGBAQ R, G, B = color[1..3] * 204, A = color[0] * 128, Q = 1, and a REGLIST GIF tag of ST/XYZ2 pairs. Then,
 * while mpDeferMan holds fewer than 2600 entries, a 0x80-byte entry is appended to it with the current draw state
 * (*m_pCurrentDrawState, low 5 bits of its third word from the manager's +0x69CC4), the packet's start and end and the
 * sort key at +0x69CC0. mVifCursor moves past the packet. Slot 80 of cPS2GraphicsMan_vtbl.
 *
 * @param font  the font (cFont layout)
 * @param text  the zero-terminated string
 * @param color  the colour {a, r, g, b} in 0..1
 * @param x  the left edge
 * @param y  the top edge
 * @param sx  horizontal scale
 * @param sy  vertical scale
 * @param su  texture u scale
 * @param sv  texture v scale
 * @param z  the depth
 */
// PORT: PS2-only GS packet build and VU0 inline asm; uncached (0x30000000) pointers.
extern "C" void cPS2GraphicsMan_DrawString3D(sGfx8808* self, sFontRes8808* font, char* text, float* color,
                              float x, float y, float sx, float sy, float su, float sv, float z)
{
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    unsigned int buf = self->bufAddr;
    unsigned long* packetHead = (unsigned long*)(buf | 0x30000000);
    unsigned long* glyphOut = packetHead + 10;
    int nloop = 0;
    if (self->views[self->curView].mode != 0) {
        float vz = self->f6B18 * z + self->f6B28;
        sPt7278 cornerMin;
        sPt7278 cornerMax;
        cornerMin.z = vz;
        cornerMin.w = 1.0f;
        cornerMax.z = vz;
        cornerMax.w = 1.0f;
        for (; *text; text++) {
            sGlyph8808* glyphRec = Find8808(font, (unsigned char)*text);
            if (glyphRec) {
                nloop++;
                float uv0[2];
                float uv1[2];
                Set2_8808(uv0, glyphRec->u * su, glyphRec->v * sv);
                Set2_8808(uv1, (glyphRec->u + glyphRec->w) * su, (glyphRec->v + glyphRec->h) * sv);
                cornerMin.x = (x + glyphRec->xoff * sx) * self->f6AF0 + self->f6B20;
                cornerMin.y = (y + glyphRec->yoff * sy) * self->f6B04 + self->f6B24;
                cornerMax.x = cornerMin.x + glyphRec->w * sx * self->f6AF0;
                cornerMax.y = cornerMin.y + glyphRec->h * sy * self->f6B04;
                glyphOut[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                glyphOut[1] = (int)cornerMin.x | ((long)(int)cornerMin.y << 16) | ((long)(int)cornerMin.z << 32);
                glyphOut[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                glyphOut[3] = (int)cornerMax.x | ((long)(int)cornerMax.y << 16) | ((long)(int)cornerMax.z << 32);
                glyphOut += 4;
                x += glyphRec->adv * sx;
            }
        }
    } else {
        for (; *text; text++) {
            sGlyph8808* glyphRec = Find8808(font, (unsigned char)*text);
            if (glyphRec) {
                nloop++;
                float uv0[2];
                float uv1[2];
                float size[2];
                Set2_8808(uv0, glyphRec->u * su, glyphRec->v * sv);
                Set2_8808(uv1, (glyphRec->u + glyphRec->w) * su, (glyphRec->v + glyphRec->h) * sv);
                Set2_8808(size, glyphRec->w * sx * 0.5f, glyphRec->h * sy * 0.5f);
                sPtPos8808 pos(x + glyphRec->xoff * sx + size[0], y + glyphRec->yoff * sy + size[1], z, 1.0f);
                sPt7278 projCentre = MulMat7278(&self->m6B30, pos);
                float inv = 1.0f / projCentre.w;
                projCentre.x *= inv;
                projCentre.y *= inv;
                float dx = self->f6AF0 * size[0] * inv;
                projCentre.w = inv;
                float dy = self->f6B04 * size[1] * inv;
                sPt7278 halfExtent(dx, dy, 0.0f, 0.0f);
                sPt7278 cornerMin = Sub_CF0(projCentre, halfExtent);
                sPt7278 cornerMax = Add7278(projCentre, halfExtent);
                glyphOut[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                glyphOut[1] = (int)cornerMin.x | ((long)(int)cornerMin.y << 16) | ((long)(int)(cornerMin.z * cornerMin.w) << 32);
                glyphOut[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                glyphOut[3] = (int)cornerMax.x | ((long)(int)cornerMax.y << 16) | ((long)(int)(cornerMax.z * cornerMax.w) << 32);
                glyphOut += 4;
                x += glyphRec->adv * sx;
            }
        }
    }
    packetHead[0] = (nloop * 2 + 4) | 0x10000000;
    packetHead[1] = 0;
    packetHead[2] = 0;
    packetHead[3] = (unsigned long)((nloop * 2 + 3) | 0x50000000U) << 32;
    packetHead[4] = 0x2400000000000001UL;
    packetHead[5] = 0x10;
    packetHead[6] = 86;
    packetHead[7] = (long)(int)(color[1] * 204.0f) | ((long)(int)(color[2] * 204.0f) << 8) |
           ((long)(int)(color[3] * 204.0f) << 16) | ((long)(int)(color[0] * 128.0f) << 24) |
           ((long)Bits_CF0(1.0f) << 32);
    packetHead[8] = (unsigned long)(nloop | 0x8000) | ((unsigned long)34816 << 47);
    packetHead[9] = 21074;
    unsigned int next = (unsigned int)glyphOut & 0x0FFFFFFF;
    sPtRing* ring = self->ring;
    sPtRenderState* rs = self->top;
    self->bufAddr = next + 0x10;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawString3D1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"


// One 0x80-byte entry of the graphics manager's draw ring (ring at 0x18F0): its render state, packet buffer, link
// and sort key.


// The graphics manager draw ring (cPS2GraphicsMan + 0x18F0): entry count, then the 0x80-byte entries from 0x80.




extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern int D_004A4474;
typedef int cQuad128_CF0 __attribute__((mode(TI)));

// PORT: PS2-only VU0 inline asm (vector subtract); the PC port needs a C fallback.



struct sGlyph9028 {
    unsigned short code;    // 0x0
    unsigned char w;        // 0x2
    unsigned char h;        // 0x3
    unsigned short u;       // 0x4
    unsigned short v;       // 0x6
    signed char adv;        // 0x8
    signed char xoff;       // 0x9
    signed char yoff;       // 0xA
    char pad;
};

struct sFontRes9028 {
    int count;              // 0x0
    sGlyph9028* table;      // 0x4
    sGlyph9028* def;        // 0x8
    int first;              // 0xC
    int last;               // 0x10
};

struct sView9028 {
    char pad_0x0[0x20];
    int mode;               // 0x20
    char pad_0x24[0x230 - 0x24];
};

struct sGfx9028 {
    char pad_0x0[0xE84];
    sPtRenderState* top;            // 0xE84
    char pad_0xE88[0x10DC - 0xE88];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    sPtRing* ring;                  // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6AF0 - 0x5A04];
    float f6AF0;                    // 0x6AF0
    char pad_0x6AF4[0x6B04 - 0x6AF4];
    float f6B04;                    // 0x6B04
    char pad_0x6B08[0x6B18 - 0x6B08];
    float f6B18;                    // 0x6B18
    char pad_0x6B1C[0x6B20 - 0x6B1C];
    float f6B20;                    // 0x6B20
    float f6B24;                    // 0x6B24
    float f6B28;                    // 0x6B28
    char pad_0x6B2C[0x6B30 - 0x6B2C];
    sMat7278 m6B30;                // 0x6B30
    char pad_0x6B70[0x6B90 - 0x6B70];
    int texReady;                   // 0x6B90
    char pad_0x6B94[0x6D20 - 0x6B94];
    sView9028 views[1];             // 0x6D20
};

// Homogeneous position (w given) with a const-reference ctor: its stores, w included, go straight to the stack
// slot, where the by-value sPt7278 ctor keeps `this` in a register for a constant w.
struct sPtPos9028 : sPt7278 {
    sPtPos9028(const float& ax, const float& ay, const float& az, const float& aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

// Stores a two-float pair (uv, half size); both values are computed before either store.
static inline void Set2_9028(float* d, float a, float b)
{
    d[0] = a;
    d[1] = b;
}

// Glyph lookup: direct index for [first, last], binary search above, default glyph if not found.
static inline sGlyph9028* Find9028(sFontRes9028* f, int c)
{
    if (c < f->first)
        return 0;
    if (c <= f->last)
        return f->table + c - f->first;
    int lo = f->last - f->first;
    int hi = f->count;
    do {
        int mid = (lo + hi) >> 1;
        sGlyph9028* p = f->table + mid;
        if (c == p->code)
            return p;
        if (c < p->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return f->def;
}

/**
 * Draws the 16-bit character string @p wideText in @p font as one textured sprite per glyph, queued as a single GS packet.
 *
 * The wide-character twin of cPS2GraphicsMan_DrawString3D (slot 80). Rebuilds the matrices first (func_0037D968) when
 * mMatricesValid is 0. Each character's glyph is found by code (direct index from the font's first to last linear code,
 * then a binary search, else the default glyph). When the current viewport is orthographic (word +0x20 of
 * mViewportStack[mViewportDepth] non-zero) the corners are scaled and offset by mScreenMatrix directly; otherwise each
 * glyph's centre goes through mScreenWorld with a perspective divide and its half-size is scaled by 1/w. UVs are the
 * glyph's u/v times @p su and @p sv, and @p x advances by the glyph advance times @p sx.
 *
 * The packet at mVifCursor (uncached) holds a DMA cnt tag, a VIF DIRECT code, PRIM 0x56 (sprite, textured, blended)
 * with RGBAQ R, G, B = color[1..3] * 204, A = color[0] * 128, Q = 1, and a REGLIST GIF tag of ST/XYZ2 pairs. Then,
 * while mpDeferMan holds fewer than 2600 entries, a 0x80-byte entry is appended with the current draw state (low 5 bits
 * of its third word from the manager's +0x69CC4), the packet's start and end and the sort key at +0x69CC0. mVifCursor
 * moves past the packet. Slot 81 of cPS2GraphicsMan_vtbl.
 *
 * @param font  the font (cFont layout)
 * @param wideText  the zero-terminated 16-bit character string
 * @param color  the colour {a, r, g, b} in 0..1
 * @param x  the left edge
 * @param y  the top edge
 * @param sx  horizontal scale
 * @param sy  vertical scale
 * @param su  texture u scale
 * @param sv  texture v scale
 * @param z  the depth
 */
// PORT: PS2-only GS packet build and VU0 inline asm; uncached (0x30000000) pointers.
extern "C" void cPS2GraphicsMan_DrawString3D1(sGfx9028* self, sFontRes9028* font, unsigned short* wideText, float* color,
                              float x, float y, float sx, float sy, float su, float sv, float z)
{
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    unsigned int buf = self->bufAddr;
    unsigned long* packetHead = func_0038F460(D_004A4474, buf, -1, 0);
    unsigned long* packetCursor = packetHead + 10;
    int nloop = 0;
    if (self->views[self->curView].mode != 0) {
        float vz = self->f6B18 * z + self->f6B28;
        sPt7278 topLeft;
        sPt7278 bottomRight;
        topLeft.z = vz;
        topLeft.w = 1.0f;
        bottomRight.z = vz;
        bottomRight.w = 1.0f;
        for (; *wideText; wideText++) {
            sGlyph9028* glyphInfo = Find9028(font, *wideText);
            if (glyphInfo) {
                nloop++;
                float uv0[2];
                float uv1[2];
                Set2_9028(uv0, glyphInfo->u * su, glyphInfo->v * sv);
                Set2_9028(uv1, (glyphInfo->u + glyphInfo->w) * su, (glyphInfo->v + glyphInfo->h) * sv);
                topLeft.x = (x + glyphInfo->xoff * sx) * self->f6AF0 + self->f6B20;
                topLeft.y = (y + glyphInfo->yoff * sy) * self->f6B04 + self->f6B24;
                bottomRight.x = topLeft.x + glyphInfo->w * sx * self->f6AF0;
                bottomRight.y = topLeft.y + glyphInfo->h * sy * self->f6B04;
                packetCursor[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                packetCursor[1] = (int)topLeft.x | ((long)(int)topLeft.y << 16) | ((long)(int)topLeft.z << 32);
                packetCursor[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                packetCursor[3] = (int)bottomRight.x | ((long)(int)bottomRight.y << 16) | ((long)(int)bottomRight.z << 32);
                packetCursor += 4;
                x += glyphInfo->adv * sx;
            }
        }
    } else {
        for (; *wideText; wideText++) {
            sGlyph9028* glyphInfo = Find9028(font, *wideText);
            if (glyphInfo) {
                nloop++;
                float uv0[2];
                float uv1[2];
                float size[2];
                Set2_9028(uv0, glyphInfo->u * su, glyphInfo->v * sv);
                Set2_9028(uv1, (glyphInfo->u + glyphInfo->w) * su, (glyphInfo->v + glyphInfo->h) * sv);
                Set2_9028(size, glyphInfo->w * sx * 0.5f, glyphInfo->h * sy * 0.5f);
                sPtPos9028 pos(x + glyphInfo->xoff * sx + size[0], y + glyphInfo->yoff * sy + size[1], z, 1.0f);
                sPt7278 screenCentre = MulMat7278(&self->m6B30, pos);
                float inv = 1.0f / screenCentre.w;
                screenCentre.x *= inv;
                screenCentre.y *= inv;
                float dx = self->f6AF0 * size[0] * inv;
                screenCentre.w = inv;
                float dy = self->f6B04 * size[1] * inv;
                sPt7278 halfExtent(dx, dy, 0.0f, 0.0f);
                sPt7278 topLeft = Sub_CF0(screenCentre, halfExtent);
                sPt7278 bottomRight = Add7278(screenCentre, halfExtent);
                packetCursor[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                packetCursor[1] = (int)topLeft.x | ((long)(int)topLeft.y << 16) | ((long)(int)(topLeft.z * topLeft.w) << 32);
                packetCursor[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                packetCursor[3] = (int)bottomRight.x | ((long)(int)bottomRight.y << 16) | ((long)(int)(bottomRight.z * bottomRight.w) << 32);
                packetCursor += 4;
                x += glyphInfo->adv * sx;
            }
        }
    }
    packetHead[0] = (nloop * 2 + 4) | 0x10000000;
    packetHead[1] = 0;
    packetHead[2] = 0;
    packetHead[3] = (unsigned long)((nloop * 2 + 3) | 0x50000000U) << 32;
    packetHead[4] = 0x2400000000000001UL;
    packetHead[5] = 0x10;
    packetHead[6] = 86;
    packetHead[7] = (long)(int)(color[1] * 204.0f) | ((long)(int)(color[2] * 204.0f) << 8) |
           ((long)(int)(color[3] * 204.0f) << 16) | ((long)(int)(color[0] * 128.0f) << 24) |
           ((long)Bits_CF0(1.0f) << 32);
    packetHead[8] = (unsigned long)(nloop | 0x8000) | ((unsigned long)34816 << 47);
    packetHead[9] = 21074;
    unsigned int next = func_0038F668(D_004A4474, packetCursor, 4);
    sPtRing* ring = self->ring;
    sPtRenderState* rs = self->top;
    self->bufAddr = next + 0x10;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtRingEnt* deferEntry = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = buf;
        deferEntry->next = next;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderTriStrip);
#ifdef SKIP_ASM
struct sPtRS_9860 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_9860 {
    sPtRS_9860 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_9860 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_9860 ents[1];            // 0x80
};

struct sPtGfx_9860 {
    char pad_0x0[0xE84];
    sPtRS_9860* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_9860* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sVtx_9860 {
    unsigned long q[6];
} __attribute__((aligned(16)));

extern int D_004A4380;
extern int D_004A4474;
extern char D_12B0[];
extern char D_12B8[];
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);

static inline void QueueRecord_9860(sPtGfx_9860* self, unsigned int buf, unsigned int next, int flag)
{
    sPtRing_9860* ring = self->ring;
    sPtRS_9860* rs = self->top;
    if (ring->count < 0xA28) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_9860* e = (sPtEnt_9860*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = flag;
    }
}

/**
 * Draws a triangle strip.
 *
 * Picks the VU program (D_12B0 once after D_004A4380 is set, else D_12B8) and the textured or untextured primitive from
 * the top draw state, then copies the vertices into DMA packets of at most 72 vertices; each new chunk starts again two
 * vertices back so the strip stays connected. Packets that would overflow the buffer are flushed with func_0038F668.
 * The bool argument On Tour has (isstatic) is not read.
 *
 * @param numVerts  the number of vertices (On Tour numverts)
 * @param src  the vertices
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderTriStrip(sPtGfx_9860* self, int numVerts, sVtx_9860* src)
{
    unsigned int prog;
    if (D_004A4380) {
        prog = (unsigned int)D_12B0 >> 3;
    } else {
        prog = (unsigned int)D_12B8 >> 3;
    }
    D_004A4380 = 0;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0xC0;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    int prim;
    unsigned long regs;
    if (self->top->tex >= 0) {
        prim = 92;
        regs = 1042;
    } else {
        prim = 76;
        regs = 1055;
    }
    int dma = D_004A4474;
    unsigned int buf = self->bufAddr;
    unsigned long* packet = func_0038F460(dma, buf, -1, 0);
    int qwc = *(int*)(dma + 0x10);
    unsigned long* end = (unsigned long*)((char*)packet + qwc * 16);
    for (; numVerts >= 3; ) {
        int k = numVerts < 73 ? numVerts : 72;
        int vertQwords = k * 3;
        numVerts -= k;
        if (packet + (vertQwords + 5) * 2 >= end) {
            int sz = qwc * 16;
            unsigned int newBufAddr = func_0038F668(dma, packet, 4);
            self->bufAddr = newBufAddr;
            packet = func_0038F460(dma, newBufAddr, -1, 0);
            end = (unsigned long*)((char*)packet + sz);
        }
        packet[0] = (vertQwords + 4) | 0x10000000;
        packet[1] = 0;
        packet[2] = 0;
        packet[3] = (unsigned long)(unsigned int)(((vertQwords + 2) << 16) | 0x6C008000) << 32;
        packet[4] = (unsigned long)k | ((unsigned long)1 << 15) | ((unsigned long)1 << 46) | ((unsigned long)prim << 47) | ((unsigned long)3 << 60);
        packet[5] = regs;
        packet[6] = (long)k * 3;
        packet[7] = 0;
        packet += 8;
        sVtx_9860* dstVert = (sVtx_9860*)packet;
        int i = k;
        while (i--) {
            *dstVert++ = *src++;
        }
        numVerts += 2;
        src -= 2;
        packet = (unsigned long*)((sVtx_9860*)packet + k);
        packet[0] = 0;
        packet[1] = (unsigned long)(prog | 0x14000000) << 32;
        packet += 2;
    }
    unsigned int next = func_0038F668(dma, packet, 4);
    self->bufAddr = next;
    QueueRecord_9860(self, buf, next, 0);
    self->bufAddr += 0x10;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderIndexedTriList);

//100%
INCLUDE_ASM("render/particle", func_0037A260);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 4x4 float matrix (POD, 16-byte aligned).
struct sPtMatrix_A260 {
    float m[4][4];
} __attribute__((aligned(16)));

// 4x4 float matrix with an empty ctor and a VU0 copy ctor (copies go through vf1..vf4).
struct sPtMatrixV_A260 {
    float m[4][4];
    sPtMatrixV_A260() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sPtMatrixV_A260(const sPtMatrixV_A260& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
} __attribute__((aligned(16)));

// g++ 2.95 vtable entry for a method taking (self, pointer).
struct sPtVtVP_A260 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

// cPS2GraphicsMan_A260 (prototype class) as the particle renderers see it: render-state stack top, texture ids, draw
// ring, texture set, scissor, frame-buffer setup, bounds. Fields named by their users; the rest is padding.
struct cPS2GraphicsMan_A260 {
    char pad_0x0[0xE84];
    sPtRenderState* top;            // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sPtVtVP_A260* vtable;                // 0x10D8
    char pad_0x10DC[0x18F0 - 0x10DC];
    sPtRing* ring;                  // 0x18F0
    void* texset;                   // 0x18F4
    char pad_0x18F8[0x5860 - 0x18F8];
    sPtMatrixV_A260 m5860;               // 0x5860
    char pad_0x58A0[0x5900 - 0x58A0];
    unsigned short scX;             // 0x5900
    unsigned short scY;             // 0x5902
    unsigned short scW;             // 0x5904
    unsigned short scH;             // 0x5906
    char pad_0x5908[0x5A00 - 0x5908];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A28 - 0x5A04];
    unsigned int pendBuf;           // 0x5A28
    unsigned long* pend;            // 0x5A2C
    unsigned int width;             // 0x5A30
    unsigned int height;            // 0x5A34
    char pad_0x5A38[0x4];
    unsigned int psm;               // 0x5A3C
    char pad_0x5A40[0x5A50 - 0x5A40];
    unsigned int fbw;               // 0x5A50
    char pad_0x5A54[0x5A84 - 0x5A54];
    unsigned int fbp;               // 0x5A84
    char pad_0x5A88[0x6AF0 - 0x5A88];
    float f6AF0;                    // 0x6AF0
    char pad_0x6AF4[0x6B04 - 0x6AF4];
    float f6B04;                    // 0x6B04
    char pad_0x6B08[0x6B30 - 0x6B08];
    sPtMatrix_A260 m6B30;                // 0x6B30
    float xmin;                     // 0x6B70
    float ymin;                     // 0x6B74
    float zmin;                     // 0x6B78
    float wmin;                     // 0x6B7C
    float xmax;                     // 0x6B80
    float ymax;                     // 0x6B84
    float zmax;                     // 0x6B88
    float wmax;                     // 0x6B8C
    int texReady;                   // 0x6B90
};

extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);

extern int D_004A4474;

extern int D_004A4380;
extern char D_12B0[];
extern char D_12B8[];

static inline unsigned long gsPrim_A260(int prim)
{
    return ((unsigned long)prim << 47) | ((unsigned long)0x8000 << 31);
}

/**
 * Opens a GS triangle-strip packet of @p n vertices for the caller to fill and returns where the vertices go.
 *
 * Sets bits 6-9 of the current draw state's first word to 3 and clears the low 30 bits of its fourth word, and rebuilds
 * the matrices (func_0037D968) when mMatricesValid is 0. At mVifCursor (remembered in unk_5A28, the packet cursor in
 * unk_5A2C) it writes a DMA cnt tag (n * 3 + 4 qwords), a VIF UNPACK header and a PACKED GIF tag for @p n loops of
 * three registers: PRIM 0x5C (Gouraud, textured, blended tri strip) with ST, RGBAQ, XYZ2 when the draw state has a
 * texture, else PRIM 0x4C with NOP, RGBAQ, XYZ2. After the n * 3 quadwords of vertex space it adds a VIF MSCAL of the
 * VU1 program at D_12B0 when the flag D_004A4380 is set (clearing it) or D_12B8 otherwise. func_0037A430 closes the
 * packet. cSpeedBoostFX_render uses the pair.
 *
 * @param n  the number of vertices
 * @return the vertex area: @p n entries of three quadwords (ST or unused, RGBAQ, XYZ2)
 */
// PORT: PS2-only GS packet build (DMA tag + GIF tag + PRIM/TEX registers).
extern "C" unsigned long* func_0037A260(cPS2GraphicsMan_A260* self, int n)
{
    unsigned int addr = (D_004A4380 ? (unsigned int)D_12B0 : (unsigned int)D_12B8) >> 3;
    int primType;
    long regList;
    D_004A4380 = 0;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0xC0;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    if (self->top->tex >= 0) {
        primType = 92;
        regList = 1042;
    } else {
        primType = 76;
        regList = 1055;
    }
    unsigned int buf = self->bufAddr;
    self->pendBuf = buf;
    self->pend = func_0038F460(D_004A4474, buf, -1, 0);
    self->pend[0] = (n * 3 + 4) | 0x10000000;
    self->pend[1] = 0;
    self->pend[2] = 0;
    self->pend[3] = (unsigned long)((n * 3 + 2) << 16 | 0x6C008000U) << 32;
    self->pend[4] = (unsigned long)(n | 0x8000) | gsPrim_A260(primType) | ((unsigned long)0xC000 << 46);
    self->pend[5] = regList;
    self->pend[6] = (long)n * 3;
    self->pend[7] = 0;
    unsigned long* verts = self->pend + 8;
    self->pend = verts + n * 6;
    self->pend[0] = 0;
    self->pend[1] = (unsigned long)(addr | 0x14000000) << 32;
    self->pend += 2;
    return verts;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037A430);
#ifdef SKIP_ASM
struct sPtRS_A430 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_A430 {
    sPtRS_A430 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_A430 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_A430 ents[1];            // 0x80
};

struct sPtGfx_A430 {
    char pad_0x0[0xE84];
    sPtRS_A430* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_A430* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A28 - 0x5A04];
    unsigned int pendBuf;           // 0x5A28
    unsigned long* pend;            // 0x5A2C
};

extern int D_004A4474;
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);

/**
 * Closes the packet opened by func_0037A260 and queues it in the deferred draw list.
 *
 * Ends the packet at unk_5A2C with func_0038F668 (DMA channel D_004A4474) and moves mVifCursor there. While mpDeferMan
 * holds fewer than 2600 entries it appends a 0x80-byte entry with the current draw state (low 5 bits of its third word
 * from the manager's +0x69CC4), the packet's start (unk_5A28) and end and the sort key at +0x69CC0. Then mVifCursor
 * skips 0x10 bytes, bits 6-9 of the draw state's first word and the low 30 bits of its fourth word are cleared, and
 * unk_5A28/unk_5A2C are reset to 0. cSpeedBoostFX_render calls it after filling the vertices.
 */
// PORT: uncached (0x30000000) pointers held in int.
extern "C" void func_0037A430(sPtGfx_A430* self)
{
    unsigned int next = func_0038F668(D_004A4474, self->pend, 4);
    self->bufAddr = next;
    unsigned int buf = self->pendBuf;
    sPtRing_A430* ring = self->ring;
    sPtRS_A430* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_A430* deferEntry = (sPtEnt_A430*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = buf;
        deferEntry->next = next;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
    self->pendBuf = 0;
    self->pend = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_CreateGridMesh);
#ifdef SKIP_ASM
struct sPartVEntryI_A540 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sPartVEntryD_A540 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sPartEmitter_A540 {
    int field_0x0;
    void* vt;       // 0x4
};

extern "C" sPartEmitter_A540* cPS2GridMesh_cPS2GridMesh(void* mem, void* a1, void* a2, void* a3, unsigned int flags);
extern const char str_cPS2GridMesh[];

/**
 * Creates a grid mesh of the given size.
 *
 * Allocates 460 bytes tagged str_cPS2GridMesh with @p flags and constructs a cPS2GridMesh there (cPS2GridMesh_cPS2GridMesh, given
 * @p width, @p height, the manager and @p flags). When bit 30 of @p flags is set it calls the mesh's virtual slot 2 (On
 * Tour cPSPGridMesh::IsOk) and, if that fails, deletes the mesh (slot 1 with flags 3) and returns 0.
 *
 * @param pS2GraphicsMan  the graphics manager (this)
 * @param width           the grid width
 * @param height          the grid height
 * @param flags           the memory flags (On Tour memtype)
 * @return the new mesh, or 0
 */
extern "C" sPartEmitter_A540* cPS2GraphicsMan_CreateGridMesh(cPS2GraphicsMan* pS2GraphicsMan, void* width, void* height, unsigned int flags)
{
    sPartEmitter_A540* gridMesh = cPS2GridMesh_cPS2GridMesh(cMemMan_alloc(460, str_cPS2GridMesh, flags, 0), width, height, pS2GraphicsMan, flags);
    if (flags & 0x40000000) {
        if (gridMesh == 0) {
            return 0;
        }
        sPartVEntryI_A540* vt = (sPartVEntryI_A540*)gridMesh->vt;
        if (vt[2].fn((char*)gridMesh + vt[2].delta) == 0) {
            sPartVEntryD_A540* vt2 = (sPartVEntryD_A540*)gridMesh->vt;
            vt2[1].fn((char*)gridMesh + vt2[1].delta, 3);
            return 0;
        }
    }
    return gridMesh;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawCharModel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
// One 0x80-byte entry of the graphics manager's draw ring (ring at 0x18F0): its render state, packet buffer, link
// and sort key.
// The graphics manager draw ring (cPS2GraphicsMan + 0x18F0): entry count, then the 0x80-byte entries from 0x80.
struct sPtQuadF {
    float v[4];
} __attribute__((aligned(16)));
extern char D_80[];
extern char D_70[];
extern char D_398[];
extern int D_004A4510;
extern int cRenderToggles_m_DisableCharacterSpecular;

// The render-state record as this function sees it: bit-field words (f0 bits 4..5, 6..9 mode, 20..27 alpha; f4 test,
// layer, blend, ...; f8 bits 0..4 key, 10..28 depth; fC address) and two texture ids.
struct sRS_A610 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short tex2;
};

struct sPtVtVF_A610 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

// One matrix list of a model chunk: count + bone indices.
struct sMtxList_A610 {
    int count;
    int* index;
};

// A DMA reference: packet address + data (each data block starts with its qword count).
struct sDmaRef_A610 {
    unsigned int addr;
    unsigned short* data;
};

struct sSub_A610 {
    sDmaRef_A610* refs;
    int count;
};

struct sChunk_A610 {
    unsigned int flags;         // 0x0
    int tex;                    // 0x4
    short texSlot;              // 0x8
    unsigned char alpha;        // 0xA
    char pad_0xB;
    int numSubs;                // 0xC
    sSub_A610* subs;            // 0x10
};

struct sMdl_A610 {
    char pad_0x0[0x20];
    int chunkOfs;               // 0x20
    char pad_0x24[0x3C - 0x24];
    sMtxList_A610* lists;       // 0x3C
    float* lodW;                // 0x40
    char pad_0x44[0x4];
    short numChunks;            // 0x48
    char pad_0x4A[0x50 - 0x4A];
    short numLods;              // 0x50
};

struct sGfx_A610 {
    char pad_0x0[0xE84];
    sRS_A610* top;                  // 0xE84
    char pad_0xE88[0x10B0 - 0xE88];
    int texSlots[(0x10D8 - 0x10B0) / 4];   // 0x10B0
    sPtVtVF_A610* vtable;                // 0x10D8
    char pad_0x10DC[0x13E8 - 0x10DC];
    t4Matrix* bones;                // 0x13E8
    char pad_0x13EC[0x18F0 - 0x13EC];
    sPtRing* ring;                  // 0x18F0
    char pad_0x18F4[0x58A0 - 0x18F4];
    sMat7278 m58A0;                // 0x58A0
    char pad_0x58E0[0x5920 - 0x58E0];
    sPtQuadF q5920;                 // 0x5920
    char pad_0x5930[0x5A00 - 0x5930];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

extern "C" void func_0037CBC8_A610(void* self, int tex, int mode) __asm__("func_0037CBC8__FPv");

// PORT: PS2-only VU0 inline asm (matrix * vector, clamp, depth read back from vi1).
static inline int Depth_A610(sGfx_A610* self, t4Vector v)
{
    long r;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    __asm__ __volatile__(
        "lqc2      $vf5, %1\n"
        "lqc2      $vf10, %2\n"
        "lqc2      $vf1, 0x0(%3)\n"
        "lqc2      $vf2, 0x10(%3)\n"
        "lqc2      $vf3, 0x20(%3)\n"
        "lqc2      $vf4, 0x30(%3)\n"
        "vmulax.xyzw ACC, $vf1, $vf10x\n"
        "vmadday.xyzw ACC, $vf2, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf10z\n"
        "vmaddw.xyzw $vf11, $vf4, $vf10w\n"
        "vmulw.w    $vf11, $vf11, $vf5w\n"
        "vmaxx.w    $vf11, $vf11, $vf0x\n"
        "vminix.w   $vf11, $vf11, $vf5x\n"
        "vftoi0.xyzw $vf11, $vf11\n"
        "vmtir      $vi1, $vf11w\n"
        "cfc2.ni    %0, $vi1\n"
        : "=&r"(r)
        : "m"(self->q5920), "m"(v), "r"(&self->m58A0)
        : "memory");
    return (int)r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMtx_A610(void* dst, const void* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// Inline copy of cPS2GraphicsMan_SetBlendMatrices: uploads the list's matrices and calls the skinning program.
static inline void UploadMtx_A610(sMtxList_A610* list, t4Matrix* mats, unsigned long** cur)
{
    int n = list->count;
    int* idx = list->index;
    unsigned long* p = *cur;
    p[0] = (n * 4 + 2) | 0x10000000;
    p[1] = 0;
    p[2] = (unsigned long)0x8000 << 45;
    p[3] = ((unsigned long)(unsigned int)((n << 18) | 0x6C000011) << 32) | 0x1000101;
    p += 4;
    for (int i = 0; i < n; i++) {
        CopyMtx_A610(p, &mats[idx[i]]);
        p += 8;
    }
    p[0] = ((unsigned long)n << 32) | 0x600103FF;
    p[1] = (unsigned long)(((unsigned int)D_80 >> 3) | 0x14000000) | ((unsigned long)(((unsigned int)D_70 >> 3) | 0x14000000) << 32);
    p += 2;
    *cur = p;
}

// Sets the width-bit field at shift in a render-state word.
static inline void SetField_A610(int& w, int shift, int width, int v)
{
    int mask = ((1 << width) - 1) << shift;
    w = (w & ~mask) | ((v << shift) & mask);
}

static inline void SetTest_A610(sGfx_A610* self)
{
    if (cRenderToggles_m_DisableCharacterSpecular == 0) {
        SetField_A610(self->top->f4, 7, 5, 0x11);
    } else {
        SetField_A610(self->top->f4, 7, 5, 3);
    }
}

static inline void SetTex_A610(sGfx_A610* self, int t)
{
    self->top->tex = t;
}

static inline void SetTex2_A610(sGfx_A610* self, int t)
{
    self->top->tex2 = t;
}

#define MIN_A610(a, b) ((a) < (b) ? (a) : (b))

/**
 * Draws a skinned character model.
 *
 * Unless D_004A4510 is set: pushes the draw state, starts the batch with slot 124 (cPS2GraphicsMan_InitCharMatrices, InitCharMatrices) and
 * walks the model's chunks, binding each chunk's texture and emitting its geometry with the bone matrices. With @p
 * shadow set it changes the draw state for the shadow pass (alpha and Z-test fields) and skips the per-chunk material
 * flags.
 *
 * @param mdl  the model header
 * @param base  the model data the header's offsets are relative to
 * @param shadow  non-zero for the shadow pass
 */
// PORT: PS2-only DMA/VIF packet build; uncached (0x30000000) pointers held in int.
extern "C" void cPS2GraphicsMan_DrawCharModel(sGfx_A610* self, sMdl_A610* mdl, char* base, int shadow)
{
    if (D_004A4510 == 0) {
        self->top[1] = *self->top;
        self->top++;
        self->vtable[124].fn((char*)self + self->vtable[124].delta, 1.0f);
        if (self->texReady == 0) {
            func_0037D968((cPS2GraphicsMan*)self);
        }
        int matIdx = 0;
        sChunk_A610* chunk = (sChunk_A610*)(base + mdl->chunkOfs);
        t4Matrix* bones = self->bones;
        SetTex2_A610(self, self->texSlots[chunk->texSlot]);
        SetField_A610(self->top->f0, 4, 2, 3);
        int z = MIN_A610(0x400, Depth_A610(self, bones->row[3]) + 0x10);
        if (shadow) {
            SetField_A610(self->top->f0, 20, 8, 0);
            SetField_A610(self->top->f4, 2, 5, 7);
            SetTest_A610(self);
            SetField_A610(self->top->f4, 0, 2, 2);
            SetField_A610(self->top->f8, 10, 19, z);
            SetField_A610(self->top->f4, 23, 2, 1);
            SetField_A610(self->top->f4, 20, 2, 0);
            SetField_A610(self->top->f4, 12, 8, 20);
        }
        for (int i = 0; i < mdl->numChunks; i++) {
            func_0037CBC8_A610(self, chunk->tex, 3);
            SetTex_A610(self, chunk->tex);
            if (!shadow) {
                if (chunk->flags & 0x8000) {
                    SetField_A610(self->top->f4, 2, 5, 5);
                    SetField_A610(self->top->f0, 20, 8, chunk->alpha);
                    SetField_A610(self->top->f4, 7, 5, 0xC);
                    SetField_A610(self->top->f4, 0, 2, 2);
                    SetField_A610(self->top->f8, 10, 19, z);
                    SetField_A610(self->top->f4, 23, 2, 1);
                    SetField_A610(self->top->f4, 20, 2, 3);
                    SetField_A610(self->top->f4, 12, 8, 92);
                } else {
                    SetField_A610(self->top->f4, 2, 5, 1);
                    SetField_A610(self->top->f0, 20, 8, 0);
                    SetTest_A610(self);
                    SetField_A610(self->top->f4, 0, 2, 0);
                    SetField_A610(self->top->f8, 10, 19, 0);
                    SetField_A610(self->top->f4, 23, 2, 1);
                    SetField_A610(self->top->f4, 20, 2, 0);
                    SetField_A610(self->top->f4, 12, 8, 20);
                }
            }
            if (chunk->flags & 1) {
                unsigned int buf = self->bufAddr;
                unsigned long* packet = (unsigned long*)(buf | 0x30000000);
                sMtxList_A610* list = &mdl->lists[matIdx];
                sSub_A610* send = chunk->subs + chunk->numSubs;
                for (sSub_A610* subMesh = chunk->subs; subMesh != send; list++, subMesh++) {
                    UploadMtx_A610(list, bones, &packet);
                    packet[0] = ((unsigned long)subMesh->refs->addr << 32) | 0x50000000;
                    packet[1] = 0;
                    packet += 2;
                }
                unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
                sPtRing* ring = self->ring;
                self->bufAddr = next;
                sRS_A610* rs = self->top;
                if (ring->count < 2600) {
                    rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
                    int key = *(int*)((char*)ring + 0x69CC0);
                    sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
                    *(sRS_A610*)&ringEntry->rs = *rs;
                    ringEntry->buf = buf;
                    ringEntry->next = next;
                    ringEntry->key = key;
                    ringEntry->flag = 0;
                }
                self->bufAddr += 0x10;
            } else if (chunk->flags & 0x100) {
                unsigned int buf = self->bufAddr;
                unsigned int prog = (unsigned int)D_398 >> 3;
                unsigned long* packet = (unsigned long*)(buf | 0x30000000);
                int nlod = mdl->numLods;
                float* w = mdl->lodW;
                int found = 0;
                for (int j = 0; j < nlod; j++) {
                    if (w[j] > 0.01f) {
                        found = 1;
                        break;
                    }
                }
                if (found) {
                    // scale stays a variable: the target rematerialises its bits per branch arm and carries them in a register
                    // into the second element (a literal folds into one 64-bit constant).
                    float scale = 12800.0f;
                    sMtxList_A610* list = &mdl->lists[matIdx];
                    sSub_A610* send = chunk->subs + chunk->numSubs;
                    for (sSub_A610* subMesh = chunk->subs; subMesh != send; list++, subMesh++) {
                        UploadMtx_A610(list, bones, &packet);
                        int n = subMesh->count;
                        sDmaRef_A610* dmaRef = subMesh->refs;
                        int even = (n & 1) == 0;
                        for (int k = 0; k < n; k += 2, dmaRef++) {
                            unsigned short* firstRefData = dmaRef->data;
                            packet[0] = ((unsigned long)dmaRef->addr << 32) | 0x50000000;
                            packet[1] = 0;
                            packet[2] = ((unsigned long)(unsigned int)firstRefData << 32) | 0x50000001;
                            packet[3] = 0;
                            packet[4] = (unsigned long)0x640180B4 << 32;
                            if (w[0] < 0.0f) {
                                packet[5] = (unsigned long)*(int*)&scale << 32;
                            } else {
                                packet[5] = (long)*(int*)&w[0] | ((unsigned long)*(int*)&scale << 32);
                            }
                            packet += 6;
                            if (even || k < n - 1) {
                                dmaRef++;
                            }
                            unsigned short* secondRefData = dmaRef->data;
                            packet[0] = ((unsigned long)dmaRef->addr << 32) | 0x50000000;
                            packet[1] = 0;
                            packet[2] = ((unsigned long)(unsigned int)secondRefData << 32) | 0x50000001;
                            packet[3] = 0;
                            packet[4] = ((unsigned long)0x640180B4 << 32) | 0x11000000;
                            if (w[0] < 0.0f) {
                                packet[5] = (unsigned long)*(int*)&scale << 32;
                            } else {
                                packet[5] = (long)*(int*)&w[0] | ((unsigned long)*(int*)&scale << 32);
                            }
                            packet += 6;
                            for (int j = 1; j < nlod; j++) {
                                firstRefData += (*firstRefData + 1) * 8;
                                secondRefData += (*secondRefData + 1) * 8;
                                if (w[j] > 0.01f) {
                                    packet[0] = ((unsigned long)(unsigned int)firstRefData << 32) | 0x50000001;
                                    packet[1] = 0;
                                    packet[2] = (unsigned long)0x640180B4 << 32;
                                    packet[3] = (long)*(int*)&w[j] | ((unsigned long)*(int*)&scale << 32);
                                    packet[4] = ((unsigned long)(unsigned int)secondRefData << 32) | 0x50000001;
                                    packet[5] = 0;
                                    packet[6] = (unsigned long)0x640180B4 << 32;
                                    packet[7] = (long)*(int*)&w[j] | ((unsigned long)*(int*)&scale << 32);
                                    packet += 8;
                                }
                            }
                            packet[0] = 0x10000001;
                            packet[1] = 0;
                            packet[2] = ((unsigned long)(prog | 0x14000000) << 32) | 0x11000000;
                            packet[3] = ((unsigned long)(prog | 0x14000000) << 32) | 0x11000000;
                            packet += 4;
                        }
                    }
                } else {
                    sMtxList_A610* list = &mdl->lists[matIdx];
                    sSub_A610* send = chunk->subs + chunk->numSubs;
                    for (sSub_A610* subMesh = chunk->subs; subMesh != send; list++, subMesh++) {
                        UploadMtx_A610(list, bones, &packet);
                        sDmaRef_A610* dmaRef = subMesh->refs;
                        sDmaRef_A610* end = dmaRef + subMesh->count;
                        packet[0] = ((unsigned long)dmaRef->addr << 32) | 0x50000000;
                        packet[1] = 0;
                        packet += 2;
                        for (dmaRef++; dmaRef != end; dmaRef++) {
                            packet[0] = ((unsigned long)dmaRef->addr << 32) | 0x50000001;
                            packet[1] = 0;
                            packet[2] = (unsigned long)0x11000000 << 32;
                            packet[3] = (unsigned long)(prog | 0x14000000) << 32;
                            packet += 4;
                        }
                        packet[0] = 0x10000001;
                        packet[1] = 0;
                        packet[2] = (unsigned long)0x11000000 << 32;
                        packet[3] = (unsigned long)(prog | 0x14000000) << 32;
                        packet += 4;
                    }
                }
                unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
                sPtRing* ring = self->ring;
                self->bufAddr = next;
                sRS_A610* rs = self->top;
                if (ring->count < 2600) {
                    rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
                    int key = *(int*)((char*)ring + 0x69CC0);
                    sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
                    *(sRS_A610*)&ringEntry->rs = *rs;
                    ringEntry->buf = buf;
                    ringEntry->next = next;
                    ringEntry->key = key;
                    ringEntry->flag = 0;
                }
                self->bufAddr += 0x10;
            }
            matIdx += chunk->numSubs;
            chunk++;
        }
        self->top--;
        SetField_A610(self->top->f0, 6, 4, 0);
        SetField_A610(self->top->fC, 0, 30, 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawCharShadowModel);
#ifdef SKIP_ASM
struct sPtRS_B548 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short tex2;
};

struct sPtEnt_B548 {
    sPtRS_B548 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_B548 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_B548 ents[1];            // 0x80
};

struct sMat_B548 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPtGfx_B548 {
    char pad_0x0[0xE84];
    sPtRS_B548* top;                // 0xE84
    char pad_0xE88[0x13E8 - 0xE88];
    sMat_B548* mats;                // 0x13E8
    char pad_0x13EC[0x18F0 - 0x13EC];
    sPtRing_B548* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
};

struct sTrk_B548 {
    int n;
    int* idx;
};

struct sRef_B548 {
    unsigned int* p;
    int pad;
};

struct sBone_B548 {
    int flags;
    int name;
    int x8;
    int ntracks;
    sRef_B548* refs;
};

struct sMdl_B548 {
    char pad0[0x20];
    int o20;
    char pad24[0x18];
    sTrk_B548* tracks;              // 0x3C
    char pad40[8];
    short nbones;                   // 0x48
};

extern char D_80[];
extern char D_70[];

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void CopyMtx_B548(void* dst, const sMat_B548* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(src), "r"(dst)
        : "memory");
}

static inline void AddMats_B548(unsigned long** pp, sMat_B548* mats, int n, int* idx)
{
    unsigned long* q = *pp;
    q[0] = (long)(((n << 2) + 2) | 0x10000000);
    q[1] = 0;
    q[2] = (unsigned long)0x8000 << 45;
    q[3] = ((unsigned long)(unsigned int)((n << 18) | 0x6C000011) << 32) | 0x1000101;
    q += 4;
    for (int k = 0; k < n; k++) {
        CopyMtx_B548(q, &mats[idx[k]]);
        q += 8;
    }
    q[0] = ((unsigned long)n << 32) | 0x600103FF;
    q[1] = (unsigned long)(((unsigned int)D_80 >> 3) | 0x14000000)
           | ((unsigned long)(((unsigned int)D_70 >> 3) | 0x14000000) << 32);
    q += 2;
    *pp = q;
}

static inline void BoneHeader_B548(unsigned long** pp, float* col)
{
    float one = 1.0f;
    (*pp)[1] = 0;
    (*pp)[2] = 0;
    (*pp)[3] = (unsigned long)0x50000002 << 32 | 0x11000000;
    (*pp)[4] = (unsigned long)0x10000000 << 32 | 0x8001;
    (*pp)[5] = 0xE;
    (*pp)[6] = (unsigned long)(int)(col[1] * 128.0f) | ((unsigned long)(int)(col[2] * 128.0f) << 8)
             | ((unsigned long)(int)(col[3] * 128.0f) << 16) | ((unsigned long)(int)(col[0] * 128.0f) << 24)
             | ((unsigned long)*(unsigned int*)&one << 32);
    (*pp)[7] = 1;
    *pp += 8;
}

static inline void QueueRecord_B548(sPtGfx_B548* self, unsigned int buf, unsigned int next, int flag)
{
    sPtRing_B548* ring = self->ring;
    sPtRS_B548* rs = self->top;
    if (ring->count < 0xA28) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_B548* e = (sPtEnt_B548*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = flag;
    }
}

/**
 * Draws a character model as a flat-coloured shadow.
 *
 * Sets the top draw state for the shadow (no textures, fixed blend and test fields) and, for each bone flagged in the
 * model, writes a header in @p col and the matrices of its tracks into the packet.
 *
 * @param model  the model header
 * @param base  the model data the header's offsets are relative to
 * @param col  the shadow colour
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_DrawCharShadowModel(sPtGfx_B548* self, sMdl_B548* model, char* base, float* col)
{
    unsigned int buf = self->bufAddr;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    self->top->f0 |= 0x30;
    self->top->f4 = (self->top->f4 & ~0x7C) | 4;
    int cnt = 0;
    self->top->f4 = (self->top->f4 & ~0xF80) | 0x580;
    self->top->f0 = (self->top->f0 & 0xF00FFFFF) | 0x06400000;
    self->top->f4 = (self->top->f4 & 0xFE7FFFFF) | 0x01000000;
    self->top->f4 = (self->top->f4 & 0xFFCFFFFF) | 0x00300000;
    self->top->f4 = (self->top->f4 & 0xFFF00FFF) | 0x00014000;
    self->top->tex = -1;
    self->top->tex2 = -1;
    sBone_B548* bone = (sBone_B548*)(base + model->o20);
    for (int i = 0; i < model->nbones; i++, bone++) {
        if (bone->flags & 1) {
            packet[0] = 0x10000003;
            BoneHeader_B548(&packet, col);
            sTrk_B548* tr = &model->tracks[cnt];
            for (int j = 0; j < bone->ntracks; j++, tr++) {
                AddMats_B548(&packet, self->mats, tr->n, tr->idx);
                packet[0] = ((unsigned long)*bone->refs[j].p << 32) | 0x50000000;
                packet[1] = 0;
                packet += 2;
            }
        }
        cnt += bone->ntracks;
    }
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    self->bufAddr = next;
    QueueRecord_B548(self, buf, next, 1);
    self->bufAddr += 0x10;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetShadowCharMatrices);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only GS packet build; uncached (0x30000000) pointers held in int and 128-bit qword copies.
typedef int cQuad128_BA50 __attribute__((mode(TI)));

struct sRS_BA50 {
    int f0;
    int f4;
    int f8;
    int fC;
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMat_BA50(void* dst, const void* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

/**
 * Sends the shadow projection matrix plus its scale and translation to VU1.
 *
 * Writes a GIF header, the four rows of @p mat and the qwords @p sscale and @p strans at the packet address (+0x5A00), advances
 * it, and sets the top render state's mode to 7 (0x1C0) pointing at the packet.
 *
 * @param mat  the shadow projection matrix
 * @param sscale  the shadow scale
 * @param strans  the shadow translation
 */
extern "C" void cPS2GraphicsMan_SetShadowCharMatrices(cPS2GraphicsMan* self, void* mat, cQuad128_BA50* sscale, cQuad128_BA50* strans)
{
    unsigned int buf = self->mVifCursor;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    packet[0] = 0x60000007;
    packet[2] = (unsigned long)0x8000 << 45;
    packet[3] = (unsigned long)55308 << 47;
    packet[1] = 0;
    CopyMat_BA50(packet + 4, mat);
    ((cQuad128_BA50*)packet)[6] = *sscale;
    ((cQuad128_BA50*)packet)[7] = *strans;
    packet += 16;
    self->mVifCursor = (unsigned int)packet & 0x0FFFFFFF;
    sRS_BA50* rs = (sRS_BA50*)self->m_pCurrentDrawState;
    rs->f0 = (rs->f0 & ~0x3C0) | 0x1C0;
    sRS_BA50* rs2 = (sRS_BA50*)self->m_pCurrentDrawState;
    rs2->fC = (rs2->fC & 0xC0000000) | (buf & 0x3FFFFFFF);
}
#endif

INCLUDE_ASM("render/particle", func_0037BB10);

//100%
INCLUDE_ASM("render/particle", func_0037BC40);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Pages used by a w x h buffer of the given GS pixel format (32-bit pages are 64x32, 16-bit 64x64).
static inline int pages_37BC40(int psm, int bw, int h)
{
    switch (psm) {
    case 0:
    case 1:
    case 48:
    case 49:
        return bw * ((h + 31) >> 5);
    case 2:
    case 10:
    case 50:
    case 58:
        return bw * ((h + 63) >> 6);
    }
    return 0;
}

static inline int zpages_37BC40(int psm, int bw, int h)
{
    switch (psm) {
    case 48:
    case 49:
        return bw * ((h + 31) >> 5);
    case 50:
    case 58:
        return bw * ((h + 63) >> 6);
    }
    return 0;
}

/**
 * Returns the GS memory, in 256-byte blocks, that three @p w x @p height buffers need: two colour buffers of formats
 * @p fbPsm and @p texPsm and a z buffer of format @p zPsm.
 *
 * Each buffer takes (w + 63) / 64 page columns times its row of pages: 32 rows of pixels per page for 32/24-bit formats
 * (PSMCT32, PSMCT24, PSMZ32, PSMZ24) and 64 for 16-bit ones (PSMCT16, PSMCT16S, PSMZ16, PSMZ16S); any other format
 * counts 0, and the z buffer only accepts the PSMZ formats. The page total is multiplied by 32 (blocks per 8 KB page).
 * Its only caller is func_0037BB10, which lists the display modes.
 *
 * @param w  buffer width in pixels
 * @param height  buffer height in pixels
 * @param fbPsm  GS pixel format of the first colour buffer
 * @param texPsm  GS pixel format of the second colour buffer
 * @param zPsm  GS z format of the z buffer
 * @return the total size in 256-byte blocks
 */
extern "C" int func_0037BC40(void* self, int w, int height, int fbPsm, int texPsm, int zPsm)
{
    int bw = (w + 63) >> 6;
    int fb = pages_37BC40(fbPsm, bw, height);
    int tex = pages_37BC40(texPsm, bw, height);
    int z = zpages_37BC40(zPsm, bw, height);
    return (fb + tex + z) << 5;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037BD38);
#ifdef SKIP_ASM
/**
 * Returns 1 when colour format @p psmA can be used with z format @p psmB, else 0.
 *
 * 32-bit, 24-bit and PSMCT16S colour (0, 1, 10) and PSMZ32/24/16S (48, 49, 58; written as the characters '0', '1', ':')
 * go with PSMZ32, PSMZ24 or PSMZ16S; PSMCT16 and PSMZ16 (2, 50 = '2') go only with PSMZ16. func_0037BD98 checks the
 * draw-buffer and z formats with it.
 *
 * @param psmA  the colour (or z) format
 * @param psmB  the z format
 * @return 1 when the pair is allowed, 0 otherwise
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_0037BD38(void* self, int psmA, int psmB)
{
    switch (psmA) {
    case 0:
    case 1:
    case 10:
    case '0':
    case '1':
    case ':':
        if (psmB == '0' || psmB == '1' || psmB == ':') {
            return 1;
        }
        break;
    case 2:
    case '2':
        if (psmB == '2') {
            return 1;
        }
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037BD98);
#ifdef SKIP_ASM
extern "C" void FlushCache(int);
extern "C" void func_00367078(int gs);
void* func_00367340(void* self, int a1, int a2);
extern "C" int func_003673D8(int gs, int size);
extern "C" void func_00382AF0(void* self, int arg1, int arg2);
extern "C" void func_003E6690(int a);
extern "C" void func_003FD910(int a, int b, int c, int d);

struct sParticleTarget_37BD98 {
    char pad0[0x18F4];
    int gs;                     // 0x18F4
    char pad18F8[0x59D0 - 0x18F8];
    int f59D0;                  // 0x59D0
    char pad59D4[0x5A30 - 0x59D4];
    unsigned int width;         // 0x5A30
    unsigned int height;        // 0x5A34
    unsigned int psm;           // 0x5A38
    unsigned int psm2;          // 0x5A3C
    unsigned int zpsm;          // 0x5A40
    short f5A44;                // 0x5A44
    short f5A46;                // 0x5A46
    short f5A48;                // 0x5A48
    int f5A4C;                  // 0x5A4C
    unsigned int f5A50;         // 0x5A50
    unsigned int f5A54;         // 0x5A54
    unsigned int f5A58;         // 0x5A58
    unsigned int f5A5C;         // 0x5A5C
    int f5A60;                  // 0x5A60
    int f5A64;                  // 0x5A64
    int f5A68;                  // 0x5A68
    int f5A6C;                  // 0x5A6C
    int f5A70;                  // 0x5A70
    int f5A74;                  // 0x5A74
    int f5A78;                  // 0x5A78
    int f5A7C;                  // 0x5A7C
    int f5A80;                  // 0x5A80
};

/**
 * Sets a GS display mode: checks the formats, stores the display and frame buffer settings, lays out VRAM again and
 * resets the GS.
 *
 * Returns 0 at once when func_0037BD38 rejects @p psm2 with @p zpsm. Otherwise stores @p displayWidth x @p displayHeight as the display size
 * (unk_5A54, unk_5A58), @p w x @p bufferHeight as the frame buffer size (mFBWidth, mFBHeight), @p psm as the display format
 * (unk_5A38), @p psm2 as mCurrentFBFormat, @p zpsm as mCurrentZBFormat, @p interlaceMode and @p fieldMode in unk_5A44 and unk_5A46, 2 in
 * unk_5A48 (the output mode) and @p flag in unk_5A4C; clears mCurrentFrameCount and the vertical magnification
 * (unk_5A60), and sets the horizontal one (unk_5A64) from the display width: 256 -> 9, 320 -> 7, 384 -> 6, 512 -> 4,
 * 640 -> 3 (other widths leave it unchanged). It computes the buffer widths in 64-pixel units (mFrameBufferWidth,
 * unk_5A5C), the page counts of the display, draw and z buffers (unk_5A68, unk_5A6C, unk_5A70: pages of 64 x 32 pixels
 * for 32/24-bit formats, 64 x 64 for 16-bit ones) and the dither flag unk_59D0 (1 for a 16-bit display format).
 *
 * Then it empties the texture manager's VRAM heaps (func_00367078), allocates the draw, display and z buffers in that
 * order (func_003673D8; their pages go to unk_5A78, unk_5A7C and mCurrentZBufferPage) and sets up the texture manager's
 * second heap (func_00367340): from the draw buffer over a1 * a2 + 2 * w * h words when both colour formats are
 * PSMCT24, from the display buffer over a1 * a2 + w * h when only the display format is, else from the z buffer over w
 * * h. Finally it flushes the data cache, waits for the next vertical sync on GS CSR (0x12001000), calls
 * func_003FD910(@p resetMode, @p interlaceMode, @p flag ? 80 : 2, @p fieldMode) and func_003E6690(@p flag ? 3 : 1), and sets the display
 * registers with func_00382AF0(self, 0, 1). cPS2GraphicsMan_SetDisplayMode1 calls it.
 *
 * @param displayWidth  display width in pixels
 * @param displayHeight  display height in pixels
 * @param w  frame (draw) buffer width in pixels
 * @param bufferHeight  frame buffer height in pixels
 * @param psm  GS pixel format of the display buffer
 * @param psm2  GS pixel format of the draw buffer
 * @param zpsm  GS z format of the z buffer
 * @param resetMode  first argument of the GS reset call func_003FD910
 * @param interlaceMode  interlace setting (unk_5A44), passed on to func_003FD910
 * @param fieldMode  field/frame setting (unk_5A46), passed on to func_003FD910
 * @param flag  nonzero for output mode 80 (0x50) instead of 2 and func_003E6690(3) instead of 1; kept in unk_5A4C
 * @return 1 when the mode was set, 0 when the draw and z formats do not go together
 */
extern "C" int func_0037BD98(sParticleTarget_37BD98* self, unsigned int displayWidth, unsigned int displayHeight, unsigned int w, unsigned int bufferHeight,
                             unsigned int psm, unsigned int psm2, unsigned int zpsm, short resetMode, short interlaceMode, short fieldMode, int flag)
{
    if (!func_0037BD38(self, psm2, zpsm))
        return 0;

    self->f5A58 = displayHeight;
    self->width = w;
    self->height = bufferHeight;
    self->psm2 = psm2;
    self->psm = psm;
    self->zpsm = zpsm;
    self->f5A44 = interlaceMode;
    self->f5A46 = fieldMode;
    self->f5A74 = 0;
    self->f5A54 = displayWidth;
    self->f5A48 = 2;
    self->f5A4C = flag;
    self->f5A60 = 0;

    switch (displayWidth) {
    case 0x100:
        self->f5A64 = 9;
        break;
    case 320:
        self->f5A64 = 7;
        break;
    case 0x180:
        self->f5A64 = 6;
        break;
    case 0x200:
        self->f5A64 = 4;
        break;
    case 640:
        self->f5A64 = 3;
        break;
    }

    self->f5A50 = (self->width + 0x3F) >> 6;
    self->f5A5C = (self->f5A54 + 0x3F) >> 6;

    switch (self->psm) {
    case 0:
    case 1:
    case 0x30:
    case 49:
        self->f59D0 = 0;
        self->f5A68 = self->f5A5C * ((self->f5A58 + 0x1F) >> 5);
        break;
    case 2:
    case 10:
    case 50:
    case 58:
        self->f59D0 = 1;
        self->f5A68 = self->f5A5C * ((self->f5A58 + 0x3F) >> 6);
        break;
    }

    switch (self->psm2) {
    case 0:
    case 1:
    case 0x30:
    case 49:
        self->f5A6C = self->f5A50 * ((self->height + 0x1F) >> 5);
        break;
    case 2:
    case 10:
    case 50:
    case 58:
        self->f5A6C = self->f5A50 * ((self->height + 0x3F) >> 6);
        break;
    }

    switch (self->zpsm) {
    case 0x30:
    case 49:
        self->f5A70 = self->f5A50 * ((self->height + 0x1F) >> 5);
        break;
    case 50:
    case 58:
        self->f5A70 = self->f5A50 * ((self->height + 0x3F) >> 6);
        break;
    }

    int gs = self->gs;
    func_00367078(gs);
    self->f5A78 = func_003673D8(gs, self->f5A6C << 13);
    self->f5A7C = func_003673D8(gs, self->f5A68 << 13);
    self->f5A80 = func_003673D8(gs, self->f5A70 << 13);

    // PORT: reads psm and psm2 as one 64-bit word.
    if (*(long*)&self->psm == 0x100000001L)
        func_00367340((void*)self->gs, self->f5A78 << 11, self->f5A54 * self->f5A58 + self->width * self->height * 2);
    else if (self->psm == 1)
        func_00367340((void*)self->gs, self->f5A7C << 11, self->f5A54 * self->f5A58 + self->width * self->height);
    else
        func_00367340((void*)self->gs, self->f5A80 << 11, self->width * self->height);

    FlushCache(0);
    *(volatile long*)0x12001000 = 8;
    while (!(*(volatile long*)0x12001000 & 8)) {
    }
    *(volatile long*)0x12001000;

    func_003FD910(resetMode, self->f5A44, flag ? 80 : self->f5A48, self->f5A46);
    func_003E6690(flag ? 3 : 1);
    func_00382AF0(self, 0, 1);
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C198);
#ifdef SKIP_ASM
struct sPtBits_C198 {
    unsigned long f0 : 3;
    unsigned long g0 : 1;
    unsigned long f1 : 3;
    unsigned long g1 : 1;
    unsigned long f2 : 3;
    unsigned long g2 : 1;
    unsigned long f3 : 3;
    unsigned long g3 : 1;
    unsigned long f4 : 3;
    unsigned long g4 : 1;
    unsigned long f5 : 3;
    unsigned long g5 : 1;
    unsigned long f6 : 3;
    unsigned long g6 : 1;
    unsigned long f7 : 3;
    unsigned long g7 : 1;
    unsigned long f8 : 3;
    unsigned long g8 : 1;
    unsigned long f9 : 3;
    unsigned long g9 : 1;
    unsigned long f10 : 3;
    unsigned long g10 : 1;
    unsigned long f11 : 3;
    unsigned long g11 : 1;
    unsigned long f12 : 3;
    unsigned long g12 : 1;
    unsigned long f13 : 3;
    unsigned long g13 : 1;
    unsigned long f14 : 3;
    unsigned long g14 : 1;
    unsigned long f15 : 3;
    unsigned long g15 : 1;
};

struct sPtGfx_C198 {
    char pad_0x0[0x59E0];
    sPtBits_C198 bits;              // 0x59E0
};

/**
 * Packs a 4 x 4 dither matrix into the GS DIMX register value unk_59E0.
 *
 * Entry n of @p src goes into bits 4n to 4n + 2 and bit 4n + 3 is cleared (DIMX holds sixteen 3-bit entries, DM00 to
 * DM33, one nibble apart). cPS2GraphicsMan_Init calls it.
 *
 * @param src  the 16 dither values, row by row (only the low 3 bits are kept)
 */
// PORT: 64-bit `long` bitfields (the GS-style register word).
extern "C" void func_0037C198(cPS2GraphicsMan* self, int* src)
{
    (*(sPtBits_C198*)&self->unk_59E0).f0 = src[0];
    (*(sPtBits_C198*)&self->unk_59E0).f1 = src[1];
    (*(sPtBits_C198*)&self->unk_59E0).f2 = src[2];
    (*(sPtBits_C198*)&self->unk_59E0).f3 = src[3];
    (*(sPtBits_C198*)&self->unk_59E0).f4 = src[4];
    (*(sPtBits_C198*)&self->unk_59E0).f5 = src[5];
    (*(sPtBits_C198*)&self->unk_59E0).f6 = src[6];
    (*(sPtBits_C198*)&self->unk_59E0).f7 = src[7];
    (*(sPtBits_C198*)&self->unk_59E0).f8 = src[8];
    (*(sPtBits_C198*)&self->unk_59E0).f9 = src[9];
    (*(sPtBits_C198*)&self->unk_59E0).f10 = src[10];
    (*(sPtBits_C198*)&self->unk_59E0).f11 = src[11];
    (*(sPtBits_C198*)&self->unk_59E0).f12 = src[12];
    (*(sPtBits_C198*)&self->unk_59E0).f13 = src[13];
    (*(sPtBits_C198*)&self->unk_59E0).f14 = src[14];
    (*(sPtBits_C198*)&self->unk_59E0).f15 = src[15];
    (*(sPtBits_C198*)&self->unk_59E0).g0 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g1 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g2 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g3 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g4 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g5 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g6 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g7 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g8 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g9 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g10 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g11 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g12 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g13 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g14 = 0;
    (*(sPtBits_C198*)&self->unk_59E0).g15 = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C570);
#ifdef SKIP_ASM
extern "C" int fptoui(float f);

// PORT: g++ `<?` (min) operator; needs a macro / std::min on a modern compiler.
static inline float clamp_C570(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

struct sPartColour_C570 {
    float r, g, b;
};

/**
 * Sets the screen brightness colour: unk_59EC[0..2] = @p colour's r, g and b, each clamped to 0..1 and times 128,
 * converted with fptoui (float to unsigned).
 *
 * func_00382AF0 multiplies these into the colour of the sprites that copy the draw buffer to the display buffer.
 * cPS2GraphicsMan_Init calls it.
 *
 * @param colour  the brightness per channel (1.0 = full)
 */
extern "C" void func_0037C570(void* self, sPartColour_C570* colour)
{
    float one = 1.0f;
    *(int*)((char*)self + 0x59EC) = fptoui(clamp_C570(colour->r, one) * 128.0f);
    *(int*)((char*)self + 0x59F0) = fptoui(clamp_C570(colour->g, one) * 128.0f);
    *(int*)((char*)self + 0x59F4) = fptoui(clamp_C570(colour->b, one) * 128.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C638);
#ifdef SKIP_ASM
struct sRGB_C638 {
    float x, y, z;
    sRGB_C638() {}
    sRGB_C638(const sRGB_C638& o) { x = o.x; y = o.y; z = o.z; }
};

/**
 * Returns the screen brightness colour: unk_59EC[0..2] divided by 128 (the inverse of func_0037C570).
 *
 * @return the r, g and b brightness, 1.0 = full
 */
extern "C" sRGB_C638 func_0037C638(void* self)
{
    sRGB_C638 colour;
    colour.x = (float)*(unsigned int*)((char*)self + 0x59EC) * 0.0078125f;
    colour.y = (float)*(unsigned int*)((char*)self + 0x59F0) * 0.0078125f;
    colour.z = (float)*(unsigned int*)((char*)self + 0x59F4) * 0.0078125f;
    return colour;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C720);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator; needs a macro / std::min on a modern compiler.
static inline float clamp_C720(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

/**
 * Sets the screen alpha unk_59F8 to (1 - @p alpha clamped to 0..1) * 128, converted with fptoui.
 *
 * func_00382AF0 uses it as the alpha of the sprites that copy the draw buffer to the display buffer and turns blending
 * on when it is not 128. cPS2GraphicsMan_Init and cSpeedBoostFX__cSpeedBoostFX (with 0, i.e. 128) call it.
 *
 * @param alpha  the amount to fade the screen (0 = none, 1 = full)
 */
extern "C" void func_0037C720(void* self, float alpha)
{
    *(int*)((char*)self + 0x59F8) = fptoui((1.0f - clamp_C720(alpha, 1.0f)) * 128.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C770);
#ifdef SKIP_ASM
/**
 * Returns the screen fade set by func_0037C720: 1 - unk_59F8 / 128.
 *
 * @return the fade amount (0 = none)
 */
extern "C" float func_0037C770(void* self)
{
    unsigned int rawAlpha = *(unsigned int*)((char*)self + 0x59F8);
    return 1.0f - (float)rawAlpha * 0.0078125f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_InitTextures);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
void func_00366FE0(void* p, int a1);

/**
 * Clears the texture table (0x1F40 bytes at +0x3838) and sets the texture manager's memory type (func_00366FE0).
 *
 * @param memtype  the memory type for textures (On Tour memtype)
 */
extern "C" void cPS2GraphicsMan_InitTextures(cPS2GraphicsMan* self, int memtype)
{
    MEM_fill((char*)self + 0x3838, 0, 8000);
    func_00366FE0(self->mpTextureMan, memtype);
}
#endif

extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PlatformUploadTextures__FPv);
#ifdef SKIP_ASM
/**
 * Makes the staged texture table current.
 *
 * MEM_copy()s 8000 bytes (0x1F40) from +0x3838 to +0x18F8. Called by cGraphicsMan_UploadTextures() through slot 121.
 *
 * @return no meaningful value (MEM_copy returns nothing)
 */
void* cPS2GraphicsMan_PlatformUploadTextures(void* self)
{
    return MEM_copy((char*)self + 0x18f8, (char*)self + 0x3838, 8000);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PlatformPurgeTextures);
#ifdef SKIP_ASM
struct sVEntry0037C830 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Removes every loaded texture: for each of the 2000 slots with a non-zero entry at +0x18F8 calls virtual slot 50
 * (the remove) with its index.
 */
extern "C" void cPS2GraphicsMan_PlatformPurgeTextures(cPS2GraphicsMan* self)
{
    int i;
    for (i = 0; i < 2000; i++) {
        if (((int*)((char*)self + 0x18F8))[i] != 0) {
            sVEntry0037C830* vt = (sVEntry0037C830*)self->mVtbl;
            vt[50].fn((char*)self + vt[50].delta, i);
        }
    }
}
#endif

// declared by mangled name so we can call it with a single argument, the way
// the target does (its real signature takes a second arg the caller leaves set)
extern "C" void func_00366FE0__FPvi(void*);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetTextureMemType);
#ifdef SKIP_ASM
/**
 * Passes a memory type to the texture manager (func_00366FE0).
 *
 * Declared with only the this pointer: On Tour's SetTextureMemType(int memtype) passes the type, which stays in $a1
 * and reaches func_00366FE0.
 */
extern "C" void cPS2GraphicsMan_SetTextureMemType(cPS2GraphicsMan* self)
{
    func_00366FE0__FPvi(self->mpTextureMan);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddTexture);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sTexHdr_C8C0 {
    int f0;                         // 0x0: low byte = type, upper 24 bits = offset to base header
    short s4;                       // 0x4
    short s6;                       // 0x6
    short s8;                       // 0x8
    char pad_0xA[2];
    unsigned int fC;                // 0xC
    int f10;                        // 0x10
};

struct sVEntry0037C8C0 {
    short delta;
    short index;
    int (*fn)(void*, void*, int, int, int, int, int, int, void*, int, int, int);
};

extern "C" double fptodp(float f);
extern "C" double dlog_C8C0(double) __asm__("log");
extern "C" double dpmul(double, double);
extern "C" double dpsub(double, double);
extern "C" int dtoi_C8C0(double) __asm__("dptoli");
extern "C" void func_0037D090_C8C0(void* self, int h, int a, int b, int c, int d) __asm__("func_0037D090__FPv");

static inline int clampX_C8C0(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
static inline int clampY_C8C0(int v, int lo, int hi)
{
    int r;
    if (v < lo) r = lo;
    else { r = hi; if (v <= hi) r = v; }
    return r;
}
static inline int clampZ_C8C0(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}
static inline int clampW_C8C0(int v, int lo, int hi)
{
    return v < lo ? lo : (v <= hi ? v : hi);
}
/**
 * Registers a texture from an EA shape record: works out its texel and CLUT formats and passes it on to vtable slot 47
 * (cPS2GraphicsMan_AddTexture1).
 *
 * The record is On Tour's SHAPE: f0's low byte is the type and its upper 24 bits the offset to the next record (the
 * palette); s4/s6 are width and height; fC bit 0x1000 means the texels sit at the offset in f10 (else right after the
 * 16-byte header), bit 0x2000 picks the swizzled format, and its top four bits are the mipmap count. Type 1 gives texel
 * format 11 (12 when swizzled) and type 2 gives 9 (10), both with CLUT format 1 and the palette record's texels; type 3
 * gives 2 and type 5 gives 0, without a CLUT. A palettised record whose offset is 0 would make the palette record null
 * and be dereferenced.
 *
 * For a mipmapped record with a non-zero s8 it then computes k = -16 * log2(240 / (s8 / 16384)), clamped to
 * -2047..-135, and calls func_0037D090(self, id, 5, 1, 0, k). Slot 46 of cPS2GraphicsMan_vtbl.
 *
 * @param src  the shape record
 * @param textureName  the texture name, passed through
 * @param lockType  the lock type, passed through
 * @param copyMem  the copy-memory flag, passed through
 * @param bindId  the bind id, passed through
 * @return the texture id slot 47 returns
 */
extern "C" int cPS2GraphicsMan_AddTexture(cPS2GraphicsMan* self, sTexHdr_C8C0* src, int textureName, int lockType, int copyMem, int bindId)
{
    int fmt = 0;
    int cls = 0;
    void* pal = 0;
    void* data;
    unsigned int type = *(unsigned char*)src;
    switch (type) {
    case 1: {
        sTexHdr_C8C0* base = src->f0 >> 8 ? (sTexHdr_C8C0*)((char*)src + (src->f0 >> 8)) : 0;
        fmt = 11;
        if (src->fC & 0x2000) fmt = 0xC;
        cls = 1;
        if (base->fC & 0x1000)
            pal = (char*)base + base->f10;
        else
            pal = (char*)base + 0x10;
        break;
    }
    case 2: {
        sTexHdr_C8C0* base = src->f0 >> 8 ? (sTexHdr_C8C0*)((char*)src + (src->f0 >> 8)) : 0;
        fmt = 9;
        if (src->fC & 0x2000) fmt = 10;
        cls = 1;
        if (base->fC & 0x1000)
            pal = (char*)base + base->f10;
        else
            pal = (char*)base + 0x10;
        break;
    }
    case 3:
        fmt = 2;
        break;
    case 5:
        fmt = 0;
        break;
    default:
        break;
    }
    unsigned int flags = src->fC;
    data = (char*)src + 0x10;
    if (flags & 0x1000)
        data = (char*)src + src->f10;
    sVEntry0037C8C0* vt = (sVEntry0037C8C0*)self->mVtbl;
    int textureId = vt[47].fn(((char*)self) + vt[47].delta, data, textureName, src->s4, src->s6, flags >> 28, fmt, cls, pal, lockType, copyMem, bindId);
    if ((src->fC & 0xF0000000) != 0 && src->s8 != 0) {
        double lodValue = fptodp(240.0f / ((float)src->s8 * 6.1035156e-05f));
        lodValue = dlog_C8C0(lodValue);
        lodValue = dpmul(lodValue, 1.4426950408889634);
        lodValue = dpsub(0.0, lodValue);
        lodValue = dpmul(lodValue, 16.0);
        int k = dtoi_C8C0(lodValue);
        k = clampZ_C8C0(k, -0x7FF, -135);
        func_0037D090_C8C0(((char*)self), textureId, 5, 1, 0, k);
    }
    return textureId;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddTexture1);
#ifdef SKIP_ASM
extern "C" int func_00367440(void* mgr, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);

/**
 * Creates a texture in the texture manager (func_00367440 on mpTextureMan) and marks its id in unk_3838.
 *
 * All eleven arguments go through unchanged; unk_3838[id] is set to 1. Slot 47 of cPS2GraphicsMan_vtbl;
 * cPS2GraphicsMan_AddTexture calls it through the vtable.
 *
 * @param pixels  the texel data
 * @param name  the texture name
 * @param width  the width in texels
 * @param height  the height in texels
 * @param mipmapCount  the mipmap count
 * @param texelFormat  the texel format
 * @param clutFormat  the CLUT format
 * @param clutData  the CLUT data
 * @param lockType  the lock type
 * @param copyMem  the copy-memory flag
 * @param bindId  the bind id
 * @return the new texture id
 */
extern "C" int cPS2GraphicsMan_AddTexture1(cPS2GraphicsMan* self, int pixels, int name, int width, int height, int mipmapCount, int texelFormat, int clutFormat,
                              int clutData, int lockType, int copyMem, int bindId)
{
    int i = func_00367440(self->mpTextureMan, pixels, name, width, height, mipmapCount, texelFormat, clutFormat, clutData, lockType, copyMem, bindId);
    *(int*)((char*)self + (i << 2) + 0x3838) = 1;
    return i;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RemoveTexture);
#ifdef SKIP_ASM
extern "C" void func_00367BC0(void* tbl, int idx);

/**
 * Removes texture @p idx from the texture manager (func_00367BC0) and clears its slot at +0x18F8.
 *
 * @param idx  the texture id (On Tour texid)
 */
extern "C" void cPS2GraphicsMan_RemoveTexture(cPS2GraphicsMan* self, int idx)
{
    func_00367BC0(self->mpTextureMan, idx);
    *(int*)((char*)self + (idx << 2) + 0x18F8) = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_IsValidTexture);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
struct sGsTex0 {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sPartTex {
    char pad[0x38];
    sGsTex0 tex0;
};

struct sPartTexTable {
    int pad[2];
    sPartTex* entries[2000];
};

/**
 * Tells whether a texture id is usable: negative ids count as valid (no texture), ids below 2000 are valid when the
 * texture manager has an entry, larger ids are not.
 *
 * @param idx  the texture id (On Tour texid)
 * @return 1 if valid, else 0
 */
extern "C" int cPS2GraphicsMan_IsValidTexture(cPS2GraphicsMan* self, int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)self->mpTextureMan;
    int valid;
    if (idx >= 0) {
        if (idx < 2000) {
            valid = texTable->entries[idx] != 0;
        } else {
            valid = 0;
        }
    } else {
        valid = 1;
    }
    return valid;
}
#endif

extern "C" void* func_003691B0(int);

//100%
INCLUDE_ASM("render/particle", func_0037CBC8__FPv);
#ifdef SKIP_ASM
/**
 * Sets a texture's texture function (TEX0.TFX): forwards to func_003691B0 on mpTextureMan.
 *
 * The definition declares only self; the caller's texture id and TFX value pass through untouched in the second and
 * third argument registers. cPS2GraphicsMan_DrawCharModel calls it with TFX 3 (highlight 2).
 *
 * @return nothing meaningful (func_003691B0 returns nothing; the definition is declared void*)
 */
void* func_0037CBC8(void* self)
{
    return func_003691B0(*(int*)((char*)self + 0x18f4));
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_CreateScreenGrabTexture);
#ifdef SKIP_ASM
struct sPartVEntry52 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

/**
 * Creates a screen-grab texture: stores the texture id returned by virtual slot 52 (@p width, @p height, 0) in the handle's
 * second word.
 *
 * @param out  the tScreenGrabHandle to fill
 * @param width  the texture width (On Tour width)
 * @param height  the texture height (On Tour height)
 */
extern "C" void cPS2GraphicsMan_CreateScreenGrabTexture(cPS2GraphicsMan* self, void* out, int width, int height)
{
    sPartVEntry52* vt = (sPartVEntry52*)self->mVtbl;
    *(int*)((char*)out + 4) = vt[52].fn((char*)self + vt[52].delta, width, height, 0);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RemoveScreenGrabTexture);
#ifdef SKIP_ASM
struct sVEntry0037CC30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00367B60(void* mgr, int id);

/**
 * Releases a screen-grab texture: when the handle's texture id is not -1, frees it in the texture manager
 * (func_00367B60), removes it (virtual slot 50) and sets the id to -1.
 *
 * @param item  the tScreenGrabHandle to release
 */
extern "C" void cPS2GraphicsMan_RemoveScreenGrabTexture(cPS2GraphicsMan* self, void* item)
{
    if (*(int*)((char*)item + 0x4) != -1) {
        func_00367B60(self->mpTextureMan, *(int*)((char*)item + 0x4));
        sVEntry0037CC30* vt = (sVEntry0037CC30*)self->mVtbl;
        vt[50].fn((char*)self + vt[50].delta, *(int*)((char*)item + 0x4));
        *(int*)((char*)item + 0x4) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_TakeScreenGrab);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"


// One 0x80-byte entry of the graphics manager's draw ring (ring at 0x18F0): its render state, packet buffer, link
// and sort key.


// The graphics manager draw ring (cPS2GraphicsMan + 0x18F0): entry count, then the 0x80-byte entries from 0x80.



extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern int D_004A4474;
typedef int cQuad128_CC98 __attribute__((mode(TI)));

struct sPtTex_CC98 {
    char pad_0x0[0x28];
    int size;                       // 0x28
};

struct sPtTexTbl_CC98 {
    int pad[2];
    sPtTex_CC98* entries[1];        // 0x8
};

struct sPtGfxB_CC98 {
    char pad_0x0[0xE84];
    sPtRenderState* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing* ring;             // 0x18F0
    sPtTexTbl_CC98* tbl;            // 0x18F4
    char pad_0x18F8[0x5900 - 0x18F8];
    unsigned short vx;              // 0x5900
    unsigned short vy;              // 0x5902
    unsigned short vw;              // 0x5904
    unsigned short vh;              // 0x5906
    char pad_0x5908[0x5A00 - 0x5908];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A54 - 0x5A04];
    int f5A54;
    int f5A58;
    int f5A5C;
    char pad_0x5A60[0x5A80 - 0x5A60];
    unsigned int f5A80;
    char pad_0x5A84[4];
    int f5A88;
};

/**
 * Copies the current frame buffer into a screen-grab texture.
 *
 * Pushes the render state, looks up the texture named by @p obj[1] in the texture table, and builds a GS packet that
 * points FRAME at that texture and draws the display buffer into it as a textured sprite, then restores the scissor and
 * queues the packet on the render ring.
 *
 * @param obj  the screen-grab handle (word 1: the texture id)
 */
extern "C" void cPS2GraphicsMan_TakeScreenGrab(sPtGfxB_CC98* self, int* obj)
{
    self->top[1] = *self->top;
    self->top++;
    sPtTex_CC98* tex = self->tbl->entries[obj[1]];
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
    unsigned int buf = self->bufAddr;
    int sz = tex->size / 32;
    int w5A88 = self->f5A88 << 5;
    int h5A5C = self->f5A5C;
    unsigned int zbufPage = self->f5A80;
    unsigned long* packet = func_0038F460(D_004A4474, buf, -1, 0);
    *(cQuad128_CC98*)packet = 0x10000013;
    packet[2] = 0;
    packet[3] = 0x5000001200000000UL;
    packet += 4;
    packet[0] = 0x1000000000008011UL;
    packet[1] = 0xE;
    packet[2] = (long)sz | 0x40000;
    packet[3] = 76;
    packet[4] = (unsigned long)(zbufPage | 0x1000000) | 0x100000000UL;
    packet[5] = 78;
    packet[6] = 0xff000000ff0000UL;
    packet[7] = 0x40;
    packet[8] = 0x30000;
    packet[9] = 71;
    packet[10] = 0;
    packet[11] = 0x18;
    packet[12] = 42;
    packet[13] = 66;
    packet[14] = 0x8000000080UL;
    packet[15] = 59;
    packet[16] = 0;
    packet[17] = 20;
    packet[18] = (long)w5A88 | (long)h5A5C << 14 | 0x2a8000000L;
    packet[19] = 6;
    packet[20] = 278;
    packet[21] = 0;
    uPtFloatBits one;
    one.f = 1.0f;
    packet[22] = ((unsigned long)one.i << 32) | 0x80808080UL;
    packet[23] = 1;
    packet += 24;
    int dispHeight = self->f5A58;
    int dispWidth = self->f5A54;
    packet[0] = 0x80008;
    packet[1] = 3;
    packet[2] = 0xffff00000000UL;
    packet[3] = 5;
    packet[4] = (long)(dispWidth * 16 - 8) | (long)(dispHeight * 16 - 8) << 16;
    packet[5] = 3;
    packet[6] = 0xffff10001000UL;
    packet[7] = 5;
    packet += 8;
    packet[0] = 0;
    packet[1] = 0x3f;
    packet[2] = (unsigned long)self->vx | (unsigned long)(self->vx + (self->vw - 1)) << 16 | (unsigned long)self->vy << 32 | (unsigned long)(self->vy + (self->vh - 1)) << 48;
    packet[3] = 0x40;
    packet += 4;
    unsigned int next = func_0038F668(D_004A4474, packet, 4);
    self->bufAddr = next;
    sPtRing* ring = self->ring;
    sPtRenderState* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    *(int*)((char*)self + 0xF44) = 1;
    self->bufAddr += 0x10;
    self->top--;
}
#endif

extern "C" void* func_00369130(int);

//100%
INCLUDE_ASM("render/particle", func_0037D090__FPv);
#ifdef SKIP_ASM
/**
 * Sets a texture's filtering (TEX1 MMIN, MMAG, L and K): forwards to func_00369130 on mpTextureMan.
 *
 * The definition declares only self; the caller's texture id and four filter values pass through untouched in the
 * argument registers. cPS2GraphicsMan_AddTexture calls it with MMIN 5, MMAG 1, L 0 and a K it computes; the tile
 * constructor in mem/memstd (func_00254410) calls it with zeros.
 *
 * @return nothing meaningful (func_00369130 returns nothing; the definition is declared void*)
 */
void* func_0037D090(void* self)
{
    return func_00369130(*(int*)((char*)self + 0x18f4));
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetNewTextureData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sTexSrc_D0B0 {
    int f0;                         // 0x0: low byte = type, upper 24 bits = offset
    short s4;                       // 0x4
    short s6;                       // 0x6
    char pad8[4];
    unsigned int fC;                // 0xC
};

struct sVE_D0B0 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int, int, unsigned int, int, int, void*, int);
};


extern char str_unpack[];
void* operator_new_D0B0(unsigned int, char*, unsigned int, int) __asm__("operator_new__FUi");
extern "C" int MEM_move(void* dst, void* src, int n);
extern "C" unsigned int unpack(void* src, void* dst);
extern "C" void* MEM_free(void* p);

/**
 * Replaces the texels of texture @p textureId with an EA shape record's, unpacking a compressed record first.
 *
 * Formats as in cPS2GraphicsMan_AddTexture: type 1 gives texel format 11 (12 swizzled), type 2 gives 9 (10), both with
 * CLUT format 1 and the next record's palette; type 3 gives 2 and type 5 gives 0. When the bytes after the header start
 * 0x10 0xFB (a packed stream whose 24-bit size follows) it allocates size + 0x10 bytes (label str_unpack), copies the
 * header, unpacks the data with unpack, clears bit 7 of the type byte and uses the copy, freeing it with MEM_free
 * afterwards.
 *
 * Calls vtable slot 49 (cPS2GraphicsMan_SetNewTextureData1) with (@p textureId, texels, width, height, mipmap count, texel format, CLUT format,
 * palette, @p copyMem). Slot 48 of cPS2GraphicsMan_vtbl.
 *
 * @param selfp  the graphics manager
 * @param textureId  the texture id
 * @param objp  the shape record
 * @param copyMem  the copy-memory flag, passed through
 */
extern "C" void cPS2GraphicsMan_SetNewTextureData(cPS2GraphicsMan* selfp, int textureId, void* objp, int copyMem)
{
    cPS2GraphicsMan* self = selfp;
    sTexSrc_D0B0* obj = (sTexSrc_D0B0*)objp;
    int mode = 0;
    char* extra = 0;
    int flagB = 0;
    unsigned int type = *(unsigned char*)obj;
    switch (type) {
    case 1: {
        mode = 11;
        int w = obj->f0 >> 8;
        flagB = 1;
        char* sub = w ? (char*)obj + w : 0;
        int hi = obj->fC & 0x2000;
        if (hi) mode = 0xC;
        if (((sTexSrc_D0B0*)sub)->fC & 0x1000)
            extra = sub + *(int*)(sub + 0x10);
        else
            extra = sub + 0x10;
        break;
    }
    case 2: {
        mode = 9;
        int w = obj->f0 >> 8;
        flagB = 1;
        char* sub = w ? (char*)obj + w : 0;
        int hi = obj->fC & 0x2000;
        if (hi) mode = 10;
        if (((sTexSrc_D0B0*)sub)->fC & 0x1000)
            extra = sub + *(int*)(sub + 0x10);
        else
            extra = sub + 0x10;
        break;
    }
    case 3:
        mode = 2;
        break;
    case 5:
        mode = 0;
        break;
    }
    int allocated = 0;
    char* unpackedCopy = 0;
    char* afterHeader = (char*)obj + 0x10;
    unsigned int fC;
    char* data;
    unsigned char* ob = (unsigned char*)obj;
    if (ob[0x10] == 0x10 && ob[0x11] == 251) {
        unsigned int size = (ob[0x12] << 16 | ob[0x13] << 8 | ob[0x14]) + 0x10;
        allocated = 1;
        unpackedCopy = (char*)operator_new_D0B0(size, str_unpack, 0x80000100, 0);
        MEM_move(unpackedCopy, obj, 0x10);
        char* payload = unpackedCopy + 0x10;
        unpack(afterHeader, payload);
        *(unsigned char*)unpackedCopy &= 0x7F;
        if (((sTexSrc_D0B0*)unpackedCopy)->fC & 0x1000) {
            data = unpackedCopy + *(int*)(unpackedCopy + 0x10);
            fC = obj->fC;
        } else {
            data = unpackedCopy + 0x10;
            fC = obj->fC;
        }
    } else {
        if (obj->fC & 0x1000)
            data = (char*)obj + *(int*)((char*)obj + 0x10);
        else
            data = afterHeader;
        fC = obj->fC;
    }
    sVE_D0B0* slotEntry = &((sVE_D0B0*)self->mVtbl)[0x188 / 8];
    slotEntry->fn((char*)self + slotEntry->delta, textureId, data, obj->s4, obj->s6, fC >> 28, mode, flagB, extra, copyMem);
    if (allocated) {
        MEM_free(unpackedCopy);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetNewTextureData1);
#ifdef SKIP_ASM
extern "C" void func_00367D20(void* gm, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

/**
 * Replaces a texture's pixels and CLUT from raw data: forwards all arguments to func_00367D20 on mpTextureMan.
 *
 * func_00367D20 copies @p pixels and @p clutData into the texture's own buffers when @p copyData is set and drops its VRAM copy so
 * it is uploaded again; the size and format arguments are not used there.
 *
 * @param texId  the texture id
 * @param pixels  the new pixel data, or 0
 * @param width  width in texels (unused)
 * @param height  height in texels (unused)
 * @param mipCount  mip level count (unused)
 * @param texelFormat  texel format (unused)
 * @param clutFormat  CLUT format (unused)
 * @param clutData  the new CLUT, or 0
 * @param copyData  nonzero to copy the data into the texture's buffers
 */
extern "C" void cPS2GraphicsMan_SetNewTextureData1(cPS2GraphicsMan* self, int texId, int pixels, int width, int height, int mipCount, int texelFormat, int clutFormat, int clutData, int copyData)
{
    func_00367D20(self->mpTextureMan, texId, pixels, width, height, mipCount, texelFormat, clutFormat, clutData, copyData);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_LockFrameToTexture);
#ifdef SKIP_ASM
struct sPtRS_D348 {
    int f0;
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_D348 {
    sPtRS_D348 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_D348 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_D348 ents[1];            // 0x80
};

struct sPtGfx_D348 {
    char pad_0x0[0xE84];
    sPtRS_D348* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_D348* ring;             // 0x18F0
    void* texset;                   // 0x18F4
    char pad_0x18F8[0x5A00 - 0x18F8];
    unsigned int bufAddr;           // 0x5A00
};

extern "C" void func_00367DB8_D348(void* texset, int idx, unsigned int* buf) __asm__("func_00367DB8");

/**
 * Redirects drawing into a locked texture.
 *
 * Builds a GS packet with func_00367DB8 that sets FRAME, the scissor and XYOFFSET for texture @p idx, and queues it on
 * the render ring.
 *
 * @param idx  the texture id from cPS2GraphicsMan_AddLockedFrameTexture()
 * @return 1
 */
// PORT: uncached (0x30000000) pointers held in int.
// Uploads texture idx into the uncached buffer and queues a ring entry for it.
extern "C" int cPS2GraphicsMan_LockFrameToTexture(sPtGfx_D348* self, int idx)
{
    unsigned int buf = self->bufAddr;
    unsigned int packetAddr = buf | 0x30000000;
    func_00367DB8_D348(self->texset, idx, &packetAddr);
    unsigned int next = packetAddr & 0x0FFFFFFF;
    self->bufAddr = next;
    sPtRing_D348* ring = self->ring;
    sPtRS_D348* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_D348* ringEntry = (sPtEnt_D348*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RestoreFrameLock);
#ifdef SKIP_ASM
struct sPtRS_D450 {
    int f0;
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_D450 {
    sPtRS_D450 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short one;                      // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_D450 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_D450 ents[1];            // 0x80
};

struct sPtGfx_D450 {
    char pad_0x0[0xE84];
    sPtRS_D450* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_D450* ring;             // 0x18F0
    char pad_0x18F4[0x5900 - 0x18F4];
    unsigned short scX;             // 0x5900
    unsigned short scY;             // 0x5902
    unsigned short scW;             // 0x5904
    unsigned short scH;             // 0x5906
    char pad_0x5908[0x5A00 - 0x5908];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x2C];
    unsigned int width;             // 0x5A30
    unsigned int height;            // 0x5A34
    char pad_0x5A38[0x4];
    unsigned int psm;               // 0x5A3C
    char pad_0x5A40[0x10];
    unsigned int fbw;               // 0x5A50
    char pad_0x5A54[0x30];
    unsigned int fbp;               // 0x5A84
};

/**
 * Points drawing back at the normal frame buffer after cPS2GraphicsMan_LockFrameToTexture().
 *
 * Writes a GS packet that restores FRAME (base, width, pixel format), the scissor from the screen rectangle and
 * XYOFFSET to the screen centre, and queues it. Returns 0, where On Tour's bool version presumably reports success.
 *
 * @return 0
 */
// PORT: 64-bit `ulong` GS packet words; uncached (0x30000000) pointers held in int.
extern "C" int cPS2GraphicsMan_RestoreFrameLock(sPtGfx_D450* self)
{
    unsigned int buf = self->bufAddr;
    ulong* packet = (ulong*)(buf | 0x30000000);
    packet[0] = 0x10000006;
    packet[1] = 0;
    packet[2] = (ulong)34816 << 45;
    packet[3] = (ulong)0x50000005 << 32;
    packet[4] = ((ulong)0x10000000 << 32) | 0x8004;
    packet[5] = 0xE;
    packet[6] = (ulong)self->fbp | ((ulong)self->fbw << 16) | ((ulong)self->psm << 24);
    packet[7] = 76;
    packet[8] = (ulong)self->scX | ((ulong)(self->scX + (self->scW - 1)) << 16) | ((ulong)self->scY << 32) | ((ulong)(self->scY + (self->scH - 1)) << 48);
    packet[9] = 0x40;
    packet[10] = (ulong)self->scX | ((ulong)(self->scX + (self->scW - 1)) << 16) | ((ulong)self->scY << 32) | ((ulong)(self->scY + (self->scH - 1)) << 48);
    packet[11] = 65;
    packet[12] = (ulong)((0x800 - (self->width >> 1)) << 4) | ((ulong)((0x800 - (self->height >> 1)) << 4) << 32);
    packet[13] = 0x18;
    packet += 14;
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    sPtRing_D450* ring = self->ring;
    sPtRS_D450* rs = self->top;
    self->bufAddr = next;
    int one = 1;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_D450* ringEntry = (sPtEnt_D450*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->one = one;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->buf = buf;
    }
    self->bufAddr += 0x10;
    return 0;
}
#endif

extern "C" void* func_00367CD0(int);

//100%
INCLUDE_ASM("render/particle", func_0037D668);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPtGfx_D668 {
    char pad_0x0[0x5A3C];
    unsigned int psm;               // 0x5A3C
    char pad_0x5A40[0x10];
    unsigned int fbw;               // 0x5A50
    char pad_0x5A54[0x30];
    unsigned int fbp;               // 0x5A84
};

/**
 * Appends a GS packet that sets FRAME_1 (@p mode 0) or FRAME_2 (@p mode 1) to the draw buffer with frame mask
 * @p frameMask; any other @p mode writes nothing.
 *
 * The packet is a zero DMA word pair, VIF FLUSH and DIRECT, an A+D GIF tag and the FRAME value: the draw buffer page
 * (+0x5A84), mFrameBufferWidth and mCurrentFBFormat, with FBMSK = @p frameMask. Advances *@p pp by six words. Likely
 * bug: the DIRECT code covers 1 quadword in mode 0 but 2 in mode 1, although both send a GIF tag and one register
 * quadword. Nothing in the retail code references it.
 *
 * @param pp  the packet cursor
 * @param frameMask  the FBMSK bits (1 = do not write that bit)
 * @param mode  0 for context 1, 1 for context 2
 */
extern "C" void func_0037D668(sPtGfx_D668* self, ulong** pp, int frameMask, int mode)
{
    ulong* packet = *pp;
    switch (mode) {
    case 0:
        packet[1] = ((ulong)0x50000001 << 32) | 0x11000000;
        packet[2] = ((ulong)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[0] = 0;
        packet[4] = (ulong)self->fbp | ((ulong)self->fbw << 16) | ((ulong)self->psm << 24) | ((ulong)frameMask << 32);
        packet[5] = 76;
        break;
    case 1:
        packet[1] = ((ulong)0x50000002 << 32) | 0x11000000;
        packet[2] = ((ulong)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[0] = 0;
        packet[4] = (ulong)self->fbp | ((ulong)self->fbw << 16) | ((ulong)self->psm << 24) | ((ulong)frameMask << 32);
        packet[5] = 77;
        break;
    default:
        *pp = packet;
        return;
    }
    packet += 6;
    *pp = packet;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddLockedFrameTexture__FPv);
#ifdef SKIP_ASM
/**
 * Creates a texture the frame can later be rendered into.
 *
 * Calls func_00367CD0 on the texture manager (mpTextureMan, +0x18F4), which allocates the "LockTex" texture. The C
 * declaration has only self: the width, height and texel format arrive in a1-a3 and pass through to func_00367CD0
 * unchanged.
 *
 * @return the new texture id
 */
void* cPS2GraphicsMan_AddLockedFrameTexture(void* self)
{
    return func_00367CD0(*(int*)((char*)self + 0x18f4));
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddProjectiveTexture);
#ifdef SKIP_ASM
struct sPartQ16 {
    int w[4];
} __attribute__((aligned(16)));

struct sPartMtx {
    float m[4][4];
} __attribute__((aligned(16)));


struct sPartDrawList {
    char pad_0x0[0x13EC];
    int count;                    // 0x13EC
    tProjTexInfo entries[1];    // 0x13F0
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtxP(sPartMtx* dst, sPartMtx* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(src), "r"(dst)
        : "memory");
}

/**
 * Appends a projected texture to mProjTextures and increments mNumProjTextures (On Tour AddProjectiveTexture(pos,
 * clipMat, projMat, intensity, texid)). There is no bound check on the 8-entry array.
 *
 * @param pos  the projector position (unk_00)
 * @param clipMat  the clip matrix (unk_10)
 * @param projMat  the projection matrix (unk_50)
 * @param intensity  the intensity (unk_90)
 * @param i  the texture id (On Tour texid; unk_94)
 */
extern "C" void cPS2GraphicsMan_AddProjectiveTexture(cPS2GraphicsMan* self, sPartQ16* pos, sPartMtx* clipMat, sPartMtx* projMat, float intensity, int i)
{
    (*(sPartQ16*)&((&self->mProjTextures[0]))[self->mNumProjTextures].unk_00) = *pos;
    vu0CopyMtxP(&(*(sPartMtx*)&((&self->mProjTextures[0]))[self->mNumProjTextures].unk_10), clipMat);
    vu0CopyMtxP(&(*(sPartMtx*)&((&self->mProjTextures[0]))[self->mNumProjTextures].unk_50), projMat);
    ((&self->mProjTextures[0]))[self->mNumProjTextures].unk_90 = intensity;
    ((&self->mProjTextures[0]))[self->mNumProjTextures].unk_94 = i;
    self->mNumProjTextures++;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_ShadowClearLockTexture);
#ifdef SKIP_ASM
struct sPtRS_D800 {
    int f0;
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_D800 {
    sPtRS_D800 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_D800 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_D800 ents[1];            // 0x80
};

struct sPtGfx_D800 {
    char pad_0x0[0xE84];
    sPtRS_D800* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_D800* ring;             // 0x18F0
    void* texset;                   // 0x18F4
    char pad_0x18F8[0x5A00 - 0x18F8];
    unsigned int bufAddr;           // 0x5A00
};

extern "C" void func_00367F18_D800(void* texset, int idx, float* color, unsigned int* buf) __asm__("func_00367F18");

// Release-build assert: expands to an empty do { } while (0); its loop notes keep the target's scheduling.
#define ASSERT_D800(cond) do { } while (0)

/**
 * Clears a locked shadow texture to a colour.
 *
 * Builds a packet with func_00367F18 that draws a full-size sprite of @p color over texture @p idx, and queues it on
 * the render ring.
 *
 * @param idx  the texture id
 * @param color  the clear colour
 */
// PORT: uncached (0x30000000) pointers held in int.
// Like cPS2GraphicsMan_LockFrameToTexture for the two-argument upload; the queued entry is flagged.
extern "C" void cPS2GraphicsMan_ShadowClearLockTexture(sPtGfx_D800* self, int idx, float* color)
{
    unsigned int buf = self->bufAddr;
    unsigned int packetAddr = buf | 0x30000000;
    func_00367F18_D800(self->texset, idx, color, &packetAddr);
    unsigned int next = packetAddr & 0x0FFFFFFF;
    self->bufAddr = next;
    sPtRing_D800* ring = self->ring;
    sPtRS_D800* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_D800* e = (sPtEnt_D800*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ASSERT_D800(e != 0);
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = 1;
    }
    self->bufAddr += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetTextureWidth);
#ifdef SKIP_ASM
/**
 * Returns the width of texture @p idx: 1 << TW from its GS TEX0 register. @p idx is not range-checked.
 *
 * @param idx  the texture id (On Tour texid)
 * @return the width in texels
 */
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
extern "C" int cPS2GraphicsMan_GetTextureWidth(cPS2GraphicsMan* self, int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)self->mpTextureMan;
    return 1 << texTable->entries[idx]->tex0.TW;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetTextureHeight);
#ifdef SKIP_ASM
/**
 * Returns the height of texture @p idx: 1 << TH from its GS TEX0 register. @p idx is not range-checked.
 *
 * @param idx  the texture id (On Tour texid)
 * @return the height in texels
 */
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
extern "C" int cPS2GraphicsMan_GetTextureHeight(cPS2GraphicsMan* self, int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)self->mpTextureMan;
    return 1 << texTable->entries[idx]->tex0.TH;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037D968);
#ifdef SKIP_ASM
struct sMat_D968 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_D968(sMat_D968* d, const sMat_D968* a, const sMat_D968* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_D968(sMat_D968* d, const sMat_D968* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

extern "C" void func_003645B8(void* ring, sMat_D968* m);

/**
 * Recomputes the cached transform matrices from the current view matrix and sets mMatricesValid to 1.
 *
 * With VU0 (row vectors, so the left matrix applies first): mScreenWorld = *mCurrentView x mScreenMatrix; mWorld =
 * *mCurrentView; mGuardBandMatrix = mWorld x mView; mNetMatrix = mWorld x mViewProj (each product goes through a stack
 * copy). Then hands mWorld to the defer manager with func_003645B8 on mpDeferMan. cPS2GraphicsMan_updateNetMat calls it
 * when mMatricesValid is 0, and many draw paths in render/particle and render/bezierman call it directly.
 *
 * @param selfp  the cPS2GraphicsMan
 */
extern "C" void func_0037D968(cPS2GraphicsMan* selfp)
{
    selfp->mMatricesValid = 1;
    sMat_D968 copy;
    sMat_D968 product;
    vu0MulMat_D968(&product, (sMat_D968*)(((char*)selfp) + 0x6AF0), (sMat_D968*)selfp->mCurrentView);
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968((sMat_D968*)(((char*)selfp) + 0x6B30), &copy);
    vu0CopyMat_D968((sMat_D968*)(((char*)selfp) + 0x5780), (sMat_D968*)selfp->mCurrentView);
    vu0MulMat_D968(&product, (sMat_D968*)(((char*)selfp) + 0x57C0), (sMat_D968*)(((char*)selfp) + 0x5780));
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968((sMat_D968*)(((char*)selfp) + 0x5800), &copy);
    vu0MulMat_D968(&product, (sMat_D968*)(((char*)selfp) + 0x5860), (sMat_D968*)(((char*)selfp) + 0x5780));
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968((sMat_D968*)(((char*)selfp) + 0x58A0), &copy);
    func_003645B8(selfp->mpDeferMan, (sMat_D968*)(((char*)selfp) + 0x5780));
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetOutCode);
#ifdef SKIP_ASM
struct sPtVecDBE8 {
    float x, y, z, w;
} __attribute__((aligned(16)));


struct sPtGfxDBE8 {
    char pad_0x0[0x6B30];
    t4Matrix clipMtx;         // 0x6B30
    float minX;                 // 0x6B70
    float minY;                 // 0x6B74
    float maxZ;                 // 0x6B78
    float field_0x6B7C;
    float maxX;                 // 0x6B80
    float maxY;                 // 0x6B84
    float minZ;                 // 0x6B88
    float field_0x6B8C;
    int clipValid;              // 0x6B90
};

extern "C" void func_0037D968(cPS2GraphicsMan* self);

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sPtVecDBE8 mtxApplyDBE8(t4Matrix* m, const sPtVecDBE8& v)
{
    sPtVecDBE8 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

/**
 * Computes the clip outcode of a point against the current screen bounds.
 *
 * Refreshes the matrices if mMatricesValid is clear (func_0037D968), transforms @p point by mScreenWorld and divides x
 * and y by w. Bits: 2 / 1 for x below unk_6B70.x / above unk_6B80.x, 8 / 4 for y below / above, and when w is
 * positive 0x20 for z above unk_6B70.z and 0x10 for z below unk_6B80.z (z is not divided by w). A point behind the
 * eye (w < 0) has its code xor'd with 0x2F.
 *
 * @param point  the world-space point (On Tour point)
 * @return the outcode bits (0 = inside)
 */
extern "C" int cPS2GraphicsMan_GetOutCode(cPS2GraphicsMan* self, const sPtVecDBE8* point)
{
    if (self->mMatricesValid == 0) {
        func_0037D968(self);
    }
    sPtVecDBE8 screenPos = mtxApplyDBE8(&(*(&self->mScreenWorld)), *point);
    int code = 0;
    float w = 1.0f / screenPos.w;
    screenPos.x *= w;
    screenPos.y *= w;
    screenPos.w = w;
    if (screenPos.x < self->unk_6B70.x) {
        code |= 2;
    }
    if (self->unk_6B80.x < screenPos.x) {
        code |= 1;
    }
    if (screenPos.y < self->unk_6B70.y) {
        code |= 8;
    }
    if (self->unk_6B80.y < screenPos.y) {
        code |= 4;
    }
    float z = screenPos.z;
    if (w < 0.0f) {
        code ^= 0x2F;
    } else {
        if (self->unk_6B70.z < z) {
            code |= 0x20;
        }
        if (z < self->unk_6B80.z) {
            code |= 0x10;
        }
    }
    return code;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetScreenCoord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPtVecDD20 {
    float x, y, z, w;
    float& X() { return x; }
    float& Y() { return y; }
    float& Z() { return z; }
} __attribute__((aligned(16)));

struct sPtGfxDD20 {
    char pad_0x0[0x5A30];
    unsigned int width;         // 0x5A30
    unsigned int height;        // 0x5A34
    char pad_0x5A38[0x6B90 - 0x5A38];
    int clipValid;              // 0x6B90
};

// func_0037DEE0 is defined later in this unit with its own vector type; bind a view.
extern "C" sPtVecDD20 func_0037DEE0_DD20(void* self, const sPtVecDD20* p) __asm__("func_0037DEE0");

static inline int clampDD20(int n)
{
    int hi = 0xFFFFFF;
    int r;
    if (n >= 0) {
        r = n;
        if (r > hi) {
            r = hi;
        }
    } else {
        r = 0;
    }
    return r;
}

/**
 * Projects a world point to screen coordinates.
 *
 * Refreshes the clip matrices when stale, projects @p point (func_0037DEE0_DD20), subtracts the GS offset (2048 minus
 * half the screen size) from x and y and truncates them to whole pixels, and clamps z to the depth range.
 *
 * @param point  the world-space point
 * @return the screen position (x, y in pixels, z the depth)
 */
extern "C" sPtVecDD20 cPS2GraphicsMan_GetScreenCoord(sPtGfxDD20* self, const sPtVecDD20* point)
{
    if (self->clipValid == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    sPtVecDD20 screen = func_0037DEE0_DD20(self, point);
    screen.X() = (float)(int)(screen.X() - (2048.0f - (float)(self->width >> 1)));
    screen.Y() = (float)(int)(screen.Y() - (2048.0f - (float)(self->height >> 1)));
    screen.Z() = (float)clampDD20((int)screen.Z());
    return screen;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_ClipBoundBox);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x570; resolved via undefined_syms_auto.txt)
extern char D_570[];

/**
 * Classifies a bounding box against the view with the VU0 microprogram at D_570 (On Tour ClipBoundBox(iMinp, iMaxp,
 * iMat), returning GMClipCodes).
 *
 * @param minp  the box's minimum corner
 * @param maxp  the box's maximum corner
 * @param mat  the box's transform
 * @return 1 when the microprogram's second result is set, else 2 when its first is set, else 0
 */
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallmsr/cfc2); the PC port needs a C
// version of the microprogram.
extern "C" int cPS2GraphicsMan_ClipBoundBox(cPS2GraphicsMan* self, void* minp, void* maxp, void* mat)
{
    int vi1Result;
    int vi2Result;
    __asm__ __volatile__(
        "lqc2      $vf15, 0x0(%2)\n"
        "lqc2      $vf16, 0x0(%3)\n"
        "lqc2      $vf10, 0x0(%4)\n"
        "lqc2      $vf11, 0x10(%4)\n"
        "lqc2      $vf12, 0x20(%4)\n"
        "lqc2      $vf13, 0x30(%4)\n"
        "ctc2.ni   %5, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        "cfc2.i    %0, $vi1\n"
        "cfc2.ni   %1, $vi2\n"
        : "=r"(vi1Result), "=r"(vi2Result)
        : "r"(minp), "r"(maxp), "r"(mat), "r"((unsigned int)D_570 >> 3));
    if (vi2Result != 0) {
        return 1;
    }
    if (vi1Result != 0) {
        return 2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037DEE0);
#ifdef SKIP_ASM
struct sPartQVec {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Projects @p point to the screen: transforms it by mNetMatrix with VU0, divides by w, then multiplies by mScreenScale
 * and adds mScreenOffset.
 *
 * cPS2GraphicsMan_GetScreenCoord calls it.
 *
 * @param point  the point to project
 * @return the projected point
 */
// PORT: PS2-only VU0 macro-mode asm; the PC port needs a C version.
extern "C" sPartQVec func_0037DEE0(void* self, sPartQVec* point)
{
    sPartQVec result;
    __asm__ __volatile__(
        "lqc2      $vf10, 0x0(%1)\n"
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf2, 0x10(%2)\n"
        "lqc2      $vf3, 0x20(%2)\n"
        "lqc2      $vf4, 0x30(%2)\n"
        "lqc2      $vf5, %3\n"
        "lqc2      $vf6, %4\n"
        "vmulax.xyzw  ACC, $vf1, $vf10x\n"
        "vmadday.xyzw ACC, $vf2, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf10z\n"
        "vmaddw.xyzw  $vf11, $vf4, $vf10w\n"
        "vdiv      Q, $vf0w, $vf11w\n"
        "vwaitq\n"
        "vmulq.xyzw   $vf12, $vf11, Q\n"
        "vmula.xyzw   ACC, $vf5, $vf12\n"
        "vmaddw.xyzw  $vf13, $vf6, $vf0w\n"
        "sqc2      $vf13, %0\n"
        : "=m"(result)
        : "r"(point), "r"((char*)self + 0x58A0),
          "m"(*(sPartQVec*)((char*)self + 0x58E0)),
          "m"(*(sPartQVec*)((char*)self + 0x58F0)));
    return result;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037DF40);
#ifdef SKIP_ASM
struct sPartVec4_DF40 {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Transforms @p point by mNetMatrix with VU0 and returns the result, without the divide by w.
 *
 * Nothing in the retail code references it.
 *
 * @param point  the point to transform
 * @return the transformed (homogeneous) point
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
extern "C" sPartVec4_DF40 func_0037DF40(void* self, sPartVec4_DF40* point)
{
    char* mat = (char*)self + 0x58A0;
    sPartVec4_DF40 result;
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf10, %1\n"
        "lqc2         $vf1, 0x0(%2)\n"
        "lqc2         $vf2, 0x10(%2)\n"
        "lqc2         $vf3, 0x20(%2)\n"
        "lqc2         $vf4, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf1, $vf10x\n"
        "vmadday.xyzw ACC, $vf2, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf10z\n"
        "vmaddw.xyzw  $vf11, $vf4, $vf10w\n"
        "sqc2         $vf11, %0\n"
        ".set pop\n"
        : "=m"(result)
        : "m"(*point), "r"(mat)
        : "memory");
    return result;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_CheckProjectiveTexturesHits);
#ifdef SKIP_ASM
struct sPartVEntry_DF88 {
    short delta;
    short index;
    int (*fn)(void*, void*, void*, void*);
};

struct sPartItem_DF88 {
    char data[0xA0];
};

struct sPartSys_DF88 {
    char pad_0x0[0x10D8];
    sPartVEntry_DF88* vt;       // 0x10D8
    char pad_0x10dc[0x310];
    unsigned int count;         // 0x13EC
    char pad_0x13f0[0x10];
    sPartItem_DF88 items[1];    // 0x1400
};

/**
 * Returns a bit mask of the projective textures whose clip volume a box is not classified 1 against.
 *
 * For each of the mNumProjTextures entries of mProjTextures it calls virtual slot 93 (cPS2GraphicsMan_ClipBoundBox)
 * with @p boxMin, @p boxMax and the entry's clip matrix (tProjTexInfo +0x10) and sets bit i when the result is not 1.
 * func_0038CA08 in render/bezierman calls it.
 *
 * @param boxMin  the box's minimum corner
 * @param boxMax  the box's maximum corner
 * @return the mask, bit i for projective texture i
 */
extern "C" unsigned int cPS2GraphicsMan_CheckProjectiveTexturesHits(sPartSys_DF88* self, void* boxMin, void* boxMax)
{
    unsigned int mask = 0;
    for (unsigned int i = 0; i < self->count; i++) {
        sPartVEntry_DF88* vtEntry = &self->vt[93];
        if (vtEntry->fn((char*)self + vtEntry->delta, boxMin, boxMax, &self->items[i]) != 1) {
            mask |= 1 << i;
        }
    }
    return mask;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037E040);
#ifdef SKIP_ASM
// PORT: 128-bit GPR quadword (TImode); the PC port needs a 16-byte struct.
typedef int cPartQuad128 __attribute__((mode(TI)));

/**
 * Appends a 4 x 4 matrix as a VIF upload: a DMA cnt tag (5 quadwords), a VIF UNPACK V4-32 of 4 quadwords to VU address
 * 5 (TOPS-relative) and the matrix @p mat.
 *
 * Advances *@p pp by 6 quadwords. func_0037E120 calls it for its optional second matrix.
 *
 * @param mat  the matrix to send (16-byte aligned)
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words and PS2-only VU0 asm (lqc2/sqc2 copy).
extern "C" void func_0037E040(void* mat, char** pp)
{
    char* packet = *pp;
    *(cPartQuad128*)packet = 0x10000005;
    *(ulong*)(packet + 0x18) = (ulong)0x6C048005 << 32;
    *(ulong*)(packet + 0x10) = 0;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(mat), "r"(packet + 0x20)
        : "memory");
    *pp = packet + 0x60;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037E098);
#ifdef SKIP_ASM
struct sPartPktQuad {
    int w[4];
} __attribute__((aligned(16)));

/**
 * Appends two VIF uploads: 3 header quadwords from @p hdr to VU address 9 and 10 quadwords from @p src to VU address 15
 * (both UNPACK V4-32, TOPS-relative), under one DMA cnt tag of 15 quadwords.
 *
 * Advances *@p pp by 16 quadwords. The second parameter is not used (func_0037E120 passes the translation row of its
 * matrix there). func_0037E120 calls it when bit 0 of its flags is set.
 *
 * @param src  the 10 quadwords to send
 * @param hdr  the 3 header quadwords to send
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words.
extern "C" void func_0037E098(sPartPktQuad* src, void* self, sPartPktQuad* hdr, char** pp)
{
    char* packet = *pp;
    int i;
    *(ulong*)(packet + 0x0) = 0x1000000F;
    *(ulong*)(packet + 0x8) = 0;
    *(ulong*)(packet + 0x10) = 0;
    *(ulong*)(packet + 0x18) = (ulong)0x6C038009 << 32;
    ((sPartPktQuad*)(packet + 0x20))[0] = hdr[0];
    ((sPartPktQuad*)(packet + 0x20))[1] = hdr[1];
    ((sPartPktQuad*)(packet + 0x20))[2] = hdr[2];
    *(ulong*)(packet + 0x50) = 0;
    *(ulong*)(packet + 0x58) = (ulong)0x6C0A800F << 32;
    packet += 0x60;
    for (i = 0; i < 10; i++) {
        ((sPartPktQuad*)packet)[i] = src[i];
    }
    packet += 0xA0;
    *pp = packet;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037E120);
#ifdef SKIP_ASM
/**
 * Appends the per-object VU constants for a draw: a matrix and a flags word, plus optional extra uploads.
 *
 * Writes a DMA cnt tag (6 quadwords), VIF STMASK 0 / STCYCL 1,1 and UNPACK V4-32 of 5 quadwords to VU address 0
 * (TOPS-relative), the matrix @p mat (copied with VU0) and a quadword whose w is @p flags. When @p extraMat is not 0 it
 * appends that matrix with func_0037E040; when bit 0 of @p flags is set it appends @p hdr and @p src with
 * func_0037E098. @p a3 is not used. cPS2GraphicsMan_RenderModel and cPS2GridMesh_Render1 (cPS2GridMesh's render, which passes
 * its texture matrix as @p extraMat) call it.
 *
 * @param mat  the object matrix
 * @param hdr  3 quadwords for func_0037E098
 * @param a3  not used
 * @param extraMat  an optional second matrix, or 0
 * @param src  10 quadwords for func_0037E098
 * @param flags  the flags word sent to the VU; bit 0 adds the func_0037E098 upload
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words and PS2-only VU0 asm (lqc2/sqc2 copy).
extern "C" void func_0037E120(void* self, sPartPktQuad* mat, sPartPktQuad* hdr, int a3, void* extraMat,
                              sPartPktQuad* src, int flags, char** pp)
{
    char* packet = *pp;
    *(cPartQuad128*)packet = 0x10000006;
    *(ulong*)(packet + 0x10) = 0x20000000;
    *(ulong*)(packet + 0x18) = 0x6C05800001000101;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(mat), "r"(packet + 0x20)
        : "memory");
    *(ulong*)(packet + 0x60) = 0;
    *(ulong*)(packet + 0x68) = (ulong)flags << 32;
    packet += 0x70;
    if (extraMat != 0) {
        func_0037E040(extraMat, &packet);
    }
    if (flags & 1) {
        sPartPktQuad translation = mat[3];
        func_0037E098(src, &translation, hdr, &packet);
    }
    *pp = packet;
}
#endif

INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderModel);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetMeshQuad);
#ifdef SKIP_ASM
struct sV3_0380 {
    float x, y, z;
};

struct sCol0380 {
    unsigned short r : 5;
    unsigned short g : 5;
    unsigned short b : 5;
    unsigned short a : 1;
};

struct sEnt0380 {
    int f0;
    char* node;
    int f8;
    int fC;
};

struct sMdl0380 {
    char pad_0x0[0x8];
    sEnt0380* ents;         // 0x8
    char pad_0xC[0xC];
    sV3_0380 scale;         // 0x18
    int dataOff;            // 0x24
};

/**
 * Reads the four corners of a quad mesh of an instance (On Tour GetMeshQuad(pInstance, mesh, vert, uv, rgb)).
 *
 * Finds the mesh's vertex data through the instance's model (+0x80); positions are the 16-bit values times scale /
 * 32767, texture coordinates the 16-bit values / 4096, colours r, g, b / 31 with alpha copied raw.
 *
 * @param obj  the tInstanceRecord
 * @param idx  the mesh index
 * @param pos  receives 4 x (x, y, z)
 * @param uv  receives 4 x (u, v)
 * @param col  receives 4 x (a, r, g, b)
 */
extern "C" void cPS2GraphicsMan_GetMeshQuad(cPS2GraphicsMan* self, char* obj, int idx, float* pos, float* uv, float* col)
{
    sMdl0380* mdl = *(sMdl0380**)(obj + 0x80);
    char* base = *(char**)(*(char**)(*(char**)(mdl->ents[idx].node + 0x20)) + 0x4);
    char* data = *(char**)(base + mdl->dataOff + 0x4);
    short* texCoords = (short*)(data + 0x80);
    short* positions = (short*)(data + 0xA0);
    sCol0380* colours = (sCol0380*)(*(char**)(base + *(int*)(obj + 0x98) + 0x14) + 0x10);
    sV3_0380 k;
    sV3_0380 modelScale = mdl->scale;
    k.x = modelScale.x * 3.0518509447574615e-05f;
    k.y = modelScale.y * 3.0518509447574615e-05f;
    k.z = modelScale.z * 3.0518509447574615e-05f;
    int i;
    for (i = 0; i < 4; i++) {
        pos[i * 3 + 0] = (float)positions[0] * k.x;
        pos[i * 3 + 1] = (float)positions[1] * k.y;
        pos[i * 3 + 2] = (float)positions[2] * k.z;
        positions += 3;
        uv[i * 2 + 0] = (float)texCoords[0] * 0.000244140625f;
        uv[i * 2 + 1] = (float)texCoords[1] * 0.000244140625f;
        texCoords += 2;
        col[1] = (float)colours->r * 0.032258063554763794f;
        col[2] = (float)colours->g * 0.032258063554763794f;
        col[3] = (float)colours->b * 0.032258063554763794f;
        col[0] = (float)colours->a;
        colours++;
        col += 4;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderParticleEmitter);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPtRS_0518 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_0518 {
    sPtRS_0518 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_0518 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_0518 ents[1];            // 0x80
};

struct sQuad_0518 {
    unsigned int w[4];
} __attribute__((aligned(16)));

struct sPtGfx_0518 {
    char pad_0x0[0xE84];
    sPtRS_0518* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_0518* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

extern int D_004A44F8;
extern int D_004A4474;
extern char D_0[];
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);

static inline void CopyQuads_0518(unsigned long** pp, const void* srcp, int n)
{
    const sQuad_0518* src = (const sQuad_0518*)srcp;
    sQuad_0518* d = (sQuad_0518*)*pp;
    sQuad_0518* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}

/**
 * Draws a static particle system.
 *
 * Does nothing while D_004A44F8 is set. Pushes the render state with the particle mode, copies 21 qwords of @p obj (the
 * tParticleEmitter) into a packet followed by a VIF call of the microprogram, queues it on the render ring and pops the
 * state.
 *
 * @param obj  the particle emitter (tParticleEmitter)
 */
extern "C" void cPS2GraphicsMan_RenderParticleEmitter(sPtGfx_0518* self, sQuad_0518* obj)
{
    if (D_004A44F8 != 0)
        return;
    self->top[1] = *self->top;
    self->top++;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0x100;
    self->top->fC &= 0xC0000000;
    self->top->f4 = (self->top->f4 & ~3) | 2;
    self->top->f8 &= 0xE00003FF;
    self->top->f8 = (self->top->f8 & ~0x3E0) | 0xE0;
    unsigned int buf = self->bufAddr;
    unsigned long* packet = func_0038F460(D_004A4474, buf, -1, 0);
    packet[0] = 0x10000017;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55339 << 47;
    packet += 4;
    CopyQuads_0518(&packet, obj, 21);
    packet[0] = (unsigned long)(((unsigned int)D_0 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    self->bufAddr = func_0038F668(D_004A4474, packet, 4);
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    sPtRing_0518* ring = self->ring;
    unsigned int next = self->bufAddr;
    sPtRS_0518* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_0518* ringEntry = (sPtEnt_0518*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top--;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderDynamicParticleEmitter);
#ifdef SKIP_ASM
struct sPtRS_07A0 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_07A0 {
    sPtRS_07A0 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_07A0 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_07A0 ents[1];            // 0x80
};

struct sQuad_07A0 {
    unsigned int w[4];
} __attribute__((aligned(16)));

struct sPtGfx_07A0 {
    char pad_0x0[0xE84];
    sPtRS_07A0* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_07A0* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sTrail_07A0 {
    char pad_0x0[0x20];
    sQuad_07A0 q[0x15];             // 0x20
    float scale;                    // 0x170 (unused here)
    char pad_0x174[4];
    int count;                      // 0x178
    int head;                       // 0x17C
    char pad_0x180[0x20];
    sQuad_07A0* posA;               // 0x1A0
    sQuad_07A0* posB;               // 0x1A4
};

struct sHdr_07A0 {
    int seg;
    int step;
    float fstep;
    float v;
} __attribute__((aligned(16)));

extern int D_004A44FC;
extern int D_004A4474;
extern char D_510[];
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);

static inline int Wrap_07A0(int i, int start, int m)
{
    return (i + start) % m;
}

static inline void CopyQuads_07A0(unsigned long** pp, const void* srcp, int n)
{
    const sQuad_07A0* src = (const sQuad_07A0*)srcp;
    sQuad_07A0* d = (sQuad_07A0*)*pp;
    sQuad_07A0* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}

/**
 * Uploads a dynamic emitter's sub-emitter trail to VU1 in blocks of 64 and queues each block for drawing.
 *
 * Returns at once when D_004A44FC (menu "Disable Dynamic Emitters") is set. Sets bits 6-9 of the draw state's first
 * word to 4 and keeps only the top two bits of its fourth, rebuilding the matrices first (func_0037D968) when
 * mMatricesValid is 0. For each block of up to 64 of the m_numSubEmitters entries (n / 64 + 1 blocks) it claims a
 * scratchpad ring buffer (func_0038F460 at mVifCursor) and writes a DMA/VIF header, the 21 quadwords of m_emitter, a
 * block header {count, NumParticles / m_numSubEmitters, its float, that float * first index * TimeStep}, the block's
 * m_positions entries (oldest first from m_subEmitterId + 1, wrapping) plus the next entry, padding to 64, the same for
 * m_velocities, and a VIF MSCAL of the microprogram at D_510.
 *
 * It flushes the buffer (func_0038F668) and stores the result in mVifCursor; while mpDeferMan holds fewer than 2600
 * entries, a 0x80-byte entry is appended to it with the current draw state (*m_pCurrentDrawState, low 5 bits of its
 * third word from the manager's +0x69CC4), the packet's start and end and the sort key at +0x69CC0; and it advances
 * mVifCursor by 0x10. Finally it clears the draw-state bits it set. When m_numSubEmitters is a multiple of 64 the last
 * block is empty but still sent. Slot 83 of cPS2GraphicsMan_vtbl.
 *
 * @param obj  the emitter (cBaseClass_DynamicEmitter layout)
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderDynamicParticleEmitter(sPtGfx_07A0* self, sTrail_07A0* obj)
{
    if (D_004A44FC != 0)
        return;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0x100;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    int n = obj->count;
    sHdr_07A0 hdr;
    int done = 0;
    hdr.step = *(int*)((char*)obj + 0x20) / n;
    hdr.fstep = (float)hdr.step;
    int nb = n / 64 + 1;
    for (int i = 0; i < nb; i++) {
        unsigned int buf = self->bufAddr;
        unsigned long* writePtr = func_0038F460(D_004A4474, buf, -1, 0);
        writePtr[0] = 0x1000009A;
        writePtr[1] = 0;
        writePtr[2] = 0;
        writePtr[3] = (unsigned long)55601 << 47;
        hdr.v = hdr.fstep * (float)(i * 64) * *(float*)((char*)obj + 0x2C);
        int seg = obj->count - done;
        if (seg >= 65)
            seg = 0x40;
        hdr.seg = seg;
        writePtr += 4;
        CopyQuads_07A0(&writePtr, obj->q, 21);
        CopyQuads_07A0(&writePtr, &hdr, 1);
        int wrap = obj->head + 1;
        int ringSize = obj->count;
        int firstIndex = (done + wrap) % ringSize;
        done += hdr.seg;
        int over = firstIndex + hdr.seg - ringSize;
        if (over < 0)
            over = 0;
        int first = firstIndex + hdr.seg - firstIndex;
        if (ringSize - firstIndex < first)
            first = ringSize - firstIndex;
        int pad = 0x40 - first - over;
        sQuad_07A0* lastA;
        sQuad_07A0* lastB;
        if (done < ringSize) {
            int e = Wrap_07A0(done, wrap, ringSize);
            lastB = obj->posB + e;
            lastA = obj->posA + e;
        } else {
            int e = Wrap_07A0(done - 1, wrap, ringSize);
            lastB = obj->posB + e;
            lastA = obj->posA + e;
        }
        CopyQuads_07A0(&writePtr, obj->posA + firstIndex, first);
        if (over > 0)
            CopyQuads_07A0(&writePtr, obj->posA, over);
        CopyQuads_07A0(&writePtr, lastA, 1);
        writePtr = (unsigned long*)((sQuad_07A0*)writePtr + pad);
        CopyQuads_07A0(&writePtr, obj->posB + firstIndex, first);
        if (over > 0)
            CopyQuads_07A0(&writePtr, obj->posB, over);
        CopyQuads_07A0(&writePtr, lastB, 1);
        writePtr = (unsigned long*)((sQuad_07A0*)writePtr + pad);
        writePtr[0] = (unsigned long)(((unsigned int)D_510 >> 3) | 0x14000000) << 32;
        writePtr[1] = (unsigned long)34816 << 45;
        writePtr += 2;
        unsigned int next = func_0038F668(D_004A4474, writePtr, 4);
        self->bufAddr = next;
        sPtRing_07A0* ring = self->ring;
        sPtRS_07A0* rs = self->top;
        if (ring->count < 2600) {
            rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
            int key = *(int*)((char*)ring + 0x69CC0);
            sPtEnt_07A0* e = (sPtEnt_07A0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
            e->rs = *rs;
            e->buf = buf;
            e->next = next;
            e->key = key;
            e->flag = 0;
        }
        self->bufAddr += 0x10;
    }
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderDynamicParticleEmitter1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
// One 0x80-byte entry of the graphics manager's draw ring (ring at 0x18F0): its render state, packet buffer, link
// and sort key.
// The graphics manager draw ring (cPS2GraphicsMan + 0x18F0): entry count, then the 0x80-byte entries from 0x80.
// 4x4 float matrix (POD, 16-byte aligned).
struct sPtMatrix {
    float m[4][4];
} __attribute__((aligned(16)));

// 4x4 float matrix with an empty ctor and a VU0 copy ctor (copies go through vf1..vf4).
struct sPtMatrixV {
    float m[4][4];
    sPtMatrixV() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sPtMatrixV(const sPtMatrixV& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
} __attribute__((aligned(16)));

// One 128-bit GIF / DMA packet quadword (four unsigned words, 16-byte aligned).
struct sPtVtVP {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sPtSegHdr {
    int seg;
    int step;
    float fstep;
    float v;
} __attribute__((aligned(16)));

extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void MEM_fill(void* dst, int c, int n);
extern int D_004A4474;
extern int D_004A44FC;
struct sPS2GfxView_0CE0 {  // PORT: a view of cPS2GraphicsMan (render/ps2graphicsman.h); the typed members schedule cPS2GraphicsMan_RenderDynamicParticleEmitter1 differently
    char pad_0x0[0xE84];
    sPtRenderState* top;            // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sPtVtVP* vtable;                // 0x10D8
    char pad_0x10DC[0x18F0 - 0x10DC];
    sPtRing* ring;                  // 0x18F0
    void* texset;                   // 0x18F4
    char pad_0x18F8[0x5860 - 0x18F8];
    sPtMatrixV m5860;               // 0x5860
    char pad_0x58A0[0x5900 - 0x58A0];
    unsigned short scX;             // 0x5900
    unsigned short scY;             // 0x5902
    unsigned short scW;             // 0x5904
    unsigned short scH;             // 0x5906
    char pad_0x5908[0x5A00 - 0x5908];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A28 - 0x5A04];
    unsigned int pendBuf;           // 0x5A28
    unsigned long* pend;            // 0x5A2C
    unsigned int width;             // 0x5A30
    unsigned int height;            // 0x5A34
    char pad_0x5A38[0x4];
    unsigned int psm;               // 0x5A3C
    char pad_0x5A40[0x5A50 - 0x5A40];
    unsigned int fbw;               // 0x5A50
    char pad_0x5A54[0x5A84 - 0x5A54];
    unsigned int fbp;               // 0x5A84
    char pad_0x5A88[0x6AF0 - 0x5A88];
    float f6AF0;                    // 0x6AF0
    char pad_0x6AF4[0x6B04 - 0x6AF4];
    float f6B04;                    // 0x6B04
    char pad_0x6B08[0x6B30 - 0x6B08];
    sPtMatrix m6B30;                // 0x6B30
    float xmin;                     // 0x6B70
    float ymin;                     // 0x6B74
    float zmin;                     // 0x6B78
    float wmin;                     // 0x6B7C
    float xmax;                     // 0x6B80
    float ymax;                     // 0x6B84
    float zmax;                     // 0x6B88
    float wmax;                     // 0x6B8C
    int texReady;                   // 0x6B90
};
// Copies n packet quadwords to *pp and advances it.
static inline void CopyQuads(unsigned long** pp, const void* srcp, int n)
{
    const sGifQuad* src = (const sGifQuad*)srcp;
    sGifQuad* d = (sGifQuad*)*pp;
    sGifQuad* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}
static inline int wrapIndex(int i, int start, int m)
{
    return (i + start) % m;
}

struct sCol_0CE0 {
    char b[4];
    sCol_0CE0() {}
};

struct sTrail_0CE0 {
    char pad_0x0[0x20];
    sGifQuad q[0x15];             // 0x20
    float scale;                    // 0x170 (unused here)
    char pad_0x174[4];
    int count;                      // 0x178
    int head;                       // 0x17C
    char pad_0x180[0x20];
    sGifQuad* posA;               // 0x1A0
    sGifQuad* posB;               // 0x1A4
    char pad_0x1A8[0x200 - 0x1A8];
    int alt;                        // 0x200
    sCol_0CE0* cols;                // 0x204
};

extern char D_A00[];
extern char D_F10[];

/**
 * Uploads a coloured dynamic emitter's sub-emitter trail to VU1 in blocks of 43 and queues each block for drawing.
 *
 * Returns at once when D_004A44FC (menu "Disable Dynamic Emitters") is set. Sets bits 6-9 of the draw state's first
 * word to 4 and keeps only the top two bits of its fourth, rebuilding the matrices first (func_0037D968) when
 * mMatricesValid is 0. Works like cPS2GraphicsMan_RenderDynamicParticleEmitter with blocks of 43 entries (n / 43 + 1
 * blocks) and, after the positions and velocities, a VIF code and the block's 4-byte m_colours entries (wrapping, plus
 * the next one) as 11 quadwords. The microprogram is D_F10 when m_IsAvalanche is set, else D_A00.
 *
 * Each block is flushed (func_0038F668) into mVifCursor and, while mpDeferMan holds fewer than 2600 entries, a
 * 0x80-byte entry is appended to it with the current draw state (*m_pCurrentDrawState, low 5 bits of its third word
 * from the manager's +0x69CC4), the packet's start and end and the sort key at +0x69CC0. Slot 84 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param obj  the emitter (cDynamicColourEmitter layout)
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderDynamicParticleEmitter1(sPS2GfxView_0CE0* self, sTrail_0CE0* obj)
{
    if (D_004A44FC != 0)
        return;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0x100;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    int n = obj->count;
    sPtSegHdr hdr;
    int done = 0;
    hdr.step = *(int*)((char*)obj + 0x20) / n;
    hdr.fstep = (float)hdr.step;
    int nb = n / 43 + 1;
    unsigned int prog = (unsigned int)(obj->alt != 0 ? D_F10 : D_A00) >> 3;
    for (int i = 0; i < nb; i++) {
        unsigned int buf = self->bufAddr;
        unsigned long* writePtr = func_0038F460(D_004A4474, buf, -1, 0);
        writePtr[0] = 0x1000007C;
        writePtr[1] = 0;
        writePtr[2] = 0;
        writePtr[3] = (unsigned long)55517 << 47;
        hdr.v = hdr.fstep * (float)(i * 43) * *(float*)((char*)obj + 0x2C);
        int seg = obj->count - done;
        if (seg >= 44)
            seg = 43;
        hdr.seg = seg;
        writePtr += 4;
        CopyQuads(&writePtr, obj->q, 21);
        CopyQuads(&writePtr, &hdr, 1);
        int wrap = obj->head + 1;
        int ringSize = obj->count;
        int firstIndex = (done + wrap) % ringSize;
        done += hdr.seg;
        int over = firstIndex + hdr.seg - ringSize;
        if (over < 0)
            over = 0;
        int first = firstIndex + hdr.seg - firstIndex;
        if (ringSize - firstIndex < first)
            first = ringSize - firstIndex;
        int pad = 43 - first - over;
        sGifQuad* lastA;
        sGifQuad* lastB;
        sCol_0CE0* lastC;
        if (done < ringSize) {
            int e = wrapIndex(done, wrap, ringSize);
            lastA = obj->posA + e;
            lastB = obj->posB + e;
            lastC = obj->cols + e;
        } else {
            int e = wrapIndex(done - 1, wrap, ringSize);
            lastA = obj->posA + e;
            lastB = obj->posB + e;
            lastC = obj->cols + e;
        }
        CopyQuads(&writePtr, obj->posA + firstIndex, first);
        if (over > 0)
            CopyQuads(&writePtr, obj->posA, over);
        CopyQuads(&writePtr, lastA, 1);
        writePtr = (unsigned long*)((sGifQuad*)writePtr + pad);
        CopyQuads(&writePtr, obj->posB + firstIndex, first);
        if (over > 0)
            CopyQuads(&writePtr, obj->posB, over);
        CopyQuads(&writePtr, lastB, 1);
        writePtr = (unsigned long*)((sGifQuad*)writePtr + pad);
        writePtr[0] = 0;
        writePtr[1] = (unsigned long)0x6E2CC06E << 32;
        writePtr += 2;
        sCol_0CE0 cols[44];
        MEM_copy(cols, obj->cols + firstIndex, first * 4);
        if (over > 0)
            MEM_copy(cols + first, obj->cols, over * 4);
        cols[first + over] = *lastC;
        CopyQuads(&writePtr, cols, 11);
        writePtr[0] = (unsigned long)(prog | 0x14000000) << 32;
        writePtr[1] = (unsigned long)34816 << 45;
        writePtr += 2;
        unsigned int next = func_0038F668(D_004A4474, writePtr, 4);
        self->bufAddr = next;
        sPtRing* ring = self->ring;
        sPtRenderState* rs = self->top;
        if (ring->count < 2600) {
            rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
            int key = *(int*)((char*)ring + 0x69CC0);
            sPtRingEnt* e = (sPtRingEnt*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
            e->rs = *rs;
            e->buf = buf;
            e->next = next;
            e->key = key;
            e->flag = 0;
        }
        self->bufAddr += 0x10;
    }
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSnowBoxFlakes);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPtRS_1310 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_1310 {
    sPtRS_1310 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_1310 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_1310 ents[1];            // 0x80
};

struct sQuad_1310 {
    unsigned int w[4];
} __attribute__((aligned(16)));

struct sMat_1310 {
    float m[4][4];
    sMat_1310() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sMat_1310(const sMat_1310& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
} __attribute__((aligned(16)));

struct sVec4_1310 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPtGfx_1310 {
    char pad_0x0[0xE84];
    sPtRS_1310* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_1310* ring;             // 0x18F0
    char pad_0x18F4[0x5860 - 0x18F4];
    sMat_1310 m5860;                // 0x5860
    char pad_0x58A0[0x5A00 - 0x58A0];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sPtObj_1310 {
    sQuad_1310 q[5];                // 0x00
    sMat_1310 mat;                  // 0x50
};

extern int D_004A4474;
extern char D_0[];
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);

// PORT: PS2-only VU0 inline asm (4x4 matrix product).
static inline sMat_1310 MulMat_1310(const sMat_1310& a, const sMat_1310& b)
{
    sMat_1310 d;
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(&d), "r"(&a), "r"(&b)
        : "memory");
    return d;
}

static inline void CopyQuads_1310(unsigned long** pp, const void* srcp, int n)
{
    const sQuad_1310* src = (const sQuad_1310*)srcp;
    sQuad_1310* d = (sQuad_1310*)*pp;
    sQuad_1310* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}

/**
 * Draws the falling snow flakes of a snow box.
 *
 * Sets the snow render mode, multiplies the box matrix with the view (+0x5860) and writes the flake packet.
 * cSnowBox_render calls it for snow-box type 0; cPS2GraphicsMan_RenderSnowBoxFluffs is the fluff variant.
 *
 * @param obj  the snow box data
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderSnowBoxFlakes(sPtGfx_1310* self, sPtObj_1310* obj)
{
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0x140;
    self->top->fC &= 0xC0000000;
    self->top->f4 = (self->top->f4 & ~3) | 2;
    self->top->f8 &= 0xE00003FF;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    sMat_1310 xform = MulMat_1310(self->m5860, obj->mat);
    unsigned int buf = self->bufAddr;
    unsigned long* packet = func_0038F460(D_004A4474, buf, -1, 0);
    packet[0] = 0x1000000D;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55319 << 47;
    packet += 4;
    sVec4_1310 zeroVec;
    zeroVec.x = 0.0f;
    zeroVec.y = 0.0f;
    zeroVec.z = 0.0f;
    zeroVec.w = 0.0f;
    sVec4_1310 onesXY;
    onesXY.x = 1.0f;
    onesXY.y = 1.0f;
    onesXY.z = 0.0f;
    onesXY.w = 0.0f;
    CopyQuads_1310(&packet, (const sQuad_1310*)&xform, 4);
    CopyQuads_1310(&packet, (const sQuad_1310*)&zeroVec, 1);
    CopyQuads_1310(&packet, (const sQuad_1310*)&onesXY, 1);
    CopyQuads_1310(&packet, obj->q, 5);
    packet[0] = (unsigned long)(((unsigned int)D_0 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    unsigned int next = func_0038F668(D_004A4474, packet, 4);
    self->bufAddr = next;
    sPtRing_1310* ring = self->ring;
    sPtRS_1310* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_1310* ringEntry = (sPtEnt_1310*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top->f4 &= ~3;
    self->top->f8 &= 0xE00003FF;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSnowBoxFluffs);
#ifdef SKIP_ASM
struct sPtRS_16F0 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_16F0 {
    sPtRS_16F0 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_16F0 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_16F0 ents[1];            // 0x80
};

struct sQuad_16F0 {
    unsigned int w[4];
} __attribute__((aligned(16)));

struct sMat_16F0 {
    float m[4][4];
    sMat_16F0() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sMat_16F0(const sMat_16F0& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
} __attribute__((aligned(16)));

struct sVec4_16F0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPtGfx_16F0 {
    char pad_0x0[0xE84];
    sPtRS_16F0* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_16F0* ring;             // 0x18F0
    char pad_0x18F4[0x5860 - 0x18F4];
    sMat_16F0 m5860;                // 0x5860
    char pad_0x58A0[0x5A00 - 0x58A0];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sPtObj_16F0 {
    sQuad_16F0 q[5];                // 0x00
    sMat_16F0 mat;                  // 0x50
};

extern int D_004A4474;
extern char D_408[];
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);

// PORT: PS2-only VU0 inline asm (4x4 matrix product).
static inline sMat_16F0 MulMat_16F0(const sMat_16F0& a, const sMat_16F0& b)
{
    sMat_16F0 d;
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(&d), "r"(&a), "r"(&b)
        : "memory");
    return d;
}

static inline void CopyQuads_16F0(unsigned long** pp, const void* srcp, int n)
{
    const sQuad_16F0* src = (const sQuad_16F0*)srcp;
    sQuad_16F0* d = (sQuad_16F0*)*pp;
    sQuad_16F0* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}

/**
 * Draws the snow fluff (larger soft flakes) of a snow box.
 *
 * The same setup as cPS2GraphicsMan_RenderSnowBoxFlakes: snow render mode, box matrix times the view (+0x5860), then
 * the packet. cSnowBox_render calls it for snow-box type 1.
 *
 * @param obj  the snow box data
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderSnowBoxFluffs(sPtGfx_16F0* self, sPtObj_16F0* obj)
{
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0x140;
    self->top->fC &= 0xC0000000;
    self->top->f4 = (self->top->f4 & ~3) | 2;
    self->top->f8 &= 0xE00003FF;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    sMat_16F0 xform = MulMat_16F0(self->m5860, obj->mat);
    unsigned int buf = self->bufAddr;
    unsigned long* packet = func_0038F460(D_004A4474, buf, -1, 0);
    packet[0] = 0x1000000D;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55319 << 47;
    packet += 4;
    sVec4_16F0 zeroVec;
    zeroVec.x = 0.0f;
    zeroVec.y = 0.0f;
    zeroVec.z = 0.0f;
    zeroVec.w = 0.0f;
    sVec4_16F0 onesXY;
    onesXY.x = 1.0f;
    onesXY.y = 1.0f;
    onesXY.z = 0.0f;
    onesXY.w = 0.0f;
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&xform, 4);
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&zeroVec, 1);
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&onesXY, 1);
    CopyQuads_16F0(&packet, obj->q, 5);
    packet[0] = (unsigned long)(((unsigned int)D_408 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    unsigned int next = func_0038F668(D_004A4474, packet, 4);
    self->bufAddr = next;
    sPtRing_16F0* ring = self->ring;
    sPtRS_16F0* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_16F0* ringEntry = (sPtEnt_16F0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top->f4 &= ~3;
    self->top->f8 &= 0xE00003FF;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderLineStrip);
#ifdef SKIP_ASM
struct sPartGsCtx;
struct sPartGsVtx;
struct sPartGsPos;
extern "C" void func_00384FD0(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos);
extern "C" void func_003850A8(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos);
extern "C" void* func_00384D98(void* self, unsigned int* src);
extern void func_00384E50_1AD0(void* ctx, int mode) __asm__("func_00384E50");

struct sPt1AD0 {
    float x, y, z, w;
    sPt1AD0() {}
} __attribute__((aligned(16)));

struct sMat1AD0 {
    sPt1AD0 r[4];
};

struct sVtx1AD0 {
    sPt1AD0 st;
    sPt1AD0 rgba;
    sPt1AD0 pos;
    sVtx1AD0() {}
};

struct sCtx1AD0 {
    int w[5];
};

extern "C" int func_00383A10(void* self, sVtx1AD0* v, sPt1AD0* p, int* codes);

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector); the PC port needs a C fallback.
static inline sPt1AD0 MulMat1AD0(const sMat1AD0* m, const sPt1AD0& v)
{
    sPt1AD0 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

/**
 * Draws a line strip of @p n vertices, clipping each segment to the guard band.
 *
 * Recomputes the matrices (func_0037D968) when mMatricesValid is 0 and opens a packet builder on mVifCursor
 * (func_00384D98). Each position is transformed by mScreenWorld; x and y are divided by w (whose reciprocal is kept as
 * Q) and get clip codes against the guard band unk_6B70 / unk_6B80 (x 2/1, y 8/4, z 0x20/0x10; a negative w flips
 * them). A segment with both ends inside extends the open strip (func_00384FD0); one that enters or leaves the band is
 * clipped with func_00383A10 and the inside part is drawn, starting a new strip (func_003850A8) where it enters; a
 * segment with both ends outside but not on the same side is clipped and drawn as a strip of its own if anything is
 * left. Finally the builder is closed and its packet queued on the defer manager (func_00384E50). cPS2GraphicsMan_vtbl
 * slot 73.
 *
 * @param n  number of vertices
 * @param vtx  the vertices (0x30 bytes each: s, t, colour r, g, b, a as ints, position)
 */
extern "C" void cPS2GraphicsMan_RenderLineStrip(cPS2GraphicsMan* self, int n, sVtx1AD0* vtx)
{
    sPt1AD0 prev;
    sCtx1AD0 ctx;
    int i;
    int prevCode;

    if (self->mMatricesValid == 0) {
        func_0037D968(self);
    }
    prevCode = -1;
    func_00384D98(&ctx, (unsigned int*)(((char*)self) + 0x5A00));
    for (i = 0; i < n; i++, vtx++) {
        sPt1AD0 cur = MulMat1AD0((sMat1AD0*)(((char*)self) + 0x6B30), vtx->pos);
        int code = 0;
        cur.w = 1.0f / cur.w;
        cur.x *= cur.w;
        cur.y *= cur.w;
        if (cur.x < self->unk_6B70.x) code |= 2;
        if (self->unk_6B80.x < cur.x) code |= 1;
        if (cur.y < self->unk_6B70.y) code |= 8;
        if (self->unk_6B80.y < cur.y) code |= 4;
        if (cur.w < 0.0f) {
            code ^= 0x2F;
        } else {
            if (cur.z > self->unk_6B70.z) code |= 0x20;
            if (cur.z < self->unk_6B80.z) code |= 0x10;
        }
        if (code == 0) {
            if (prevCode == 0) {
                func_00384FD0((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
            } else if (i != 0) {
                int codes[2];
                sVtx1AD0 edgeVerts[2];
                sPt1AD0 edgePoints[2];
                edgePoints[0] = prev;
                edgeVerts[0] = *(sVtx1AD0*)((int)vtx - sizeof(sVtx1AD0));
                codes[0] = prevCode;
                edgePoints[1] = cur;
                edgeVerts[1] = *vtx;
                codes[1] = code;
                func_00383A10(((char*)self), edgeVerts, edgePoints, codes);
                func_003850A8((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[0], (sPartGsPos*)&edgePoints[0]);
                func_00384FD0((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
            } else {
                func_003850A8((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
            }
        } else if (prevCode == 0) {
            sVtx1AD0 edgeVerts[2];
            sPt1AD0 edgePoints[2];
            int codes[2];
            edgePoints[0] = prev;
            edgeVerts[0] = *(sVtx1AD0*)((int)vtx - sizeof(sVtx1AD0));
            codes[0] = prevCode;
            edgePoints[1] = cur;
            edgeVerts[1] = *vtx;
            codes[1] = code;
            func_00383A10(((char*)self), edgeVerts, edgePoints, codes);
            func_00384FD0((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[1], (sPartGsPos*)&edgePoints[1]);
        } else if ((prevCode & code) == 0) {
            sVtx1AD0 edgeVerts[2];
            sPt1AD0 edgePoints[2];
            int codes[2];
            edgePoints[0] = prev;
            edgeVerts[0] = *(sVtx1AD0*)((int)vtx - sizeof(sVtx1AD0));
            codes[0] = prevCode;
            edgePoints[1] = cur;
            edgeVerts[1] = *vtx;
            codes[1] = code;
            if (func_00383A10(((char*)self), edgeVerts, edgePoints, codes)) {
                func_003850A8((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[0], (sPartGsPos*)&edgePoints[0]);
                func_00384FD0((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[1], (sPartGsPos*)&edgePoints[1]);
            }
        }
        prev = cur;
        prevCode = code;
    }
    func_00384E50_1AD0(&ctx, 2);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSpline);
#ifdef SKIP_ASM
struct sPtRS_1F10 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_1F10 {
    sPtRS_1F10 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_1F10 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_1F10 ents[1];            // 0x80
};

struct sPtGfx_1F10 {
    char pad_0x0[0xE84];
    sPtRS_1F10* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_1F10* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sMat_1F10 {
    float m[4][4];
} __attribute__((aligned(16)));

extern char D_3BF0[];

// PORT: PS2-only VU0 inline asm (matrix copy); the PC port needs a C fallback.
static inline void CopyMat_1F10(sMat_1F10* dst, const sMat_1F10* src)
{
    __asm__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

extern "C" void func_0037D968(cPS2GraphicsMan* self);

/**
 * Draws a cubic spline on PS2: writes a GIF/VIF packet with the colour (scaled by 128), the point count and @p BezMat into
 * the uncached render buffer, and records it as a DeferMan entry while the entry count is below 0xA28.
 *
 * @param BezMat  the Bezier basis matrix (On Tour BezMat)
 * @param col  the colour as four floats a, r, g, b in 0..1
 * @param n  the number of points
 */
// PORT: uncached (0x30000000) pointers held in int; 64-bit GS words are `unsigned long`.
extern "C" void cPS2GraphicsMan_RenderSpline(sPtGfx_1F10* self, sMat_1F10* BezMat, float* col, int n)
{
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0xC0;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    unsigned int buf = self->bufAddr;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    packet[0] = 0x1000000A;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55313 << 47;
    packet[4] = ((unsigned long)0x10014000 << 32) | 1;
    packet[5] = 0xE;
    packet[6] = (long)(int)(col[1] * 128.0f) | ((long)(int)(col[2] * 128.0f) << 8) | ((long)(int)(col[3] * 128.0f) << 16) | ((long)(int)(col[0] * 128.0f) << 24) | ((unsigned long)0x3F800000 << 32);
    packet[7] = 1;
    packet[8] = (unsigned long)(n | 0x8000) | ((unsigned long)32778 << 45);
    packet[9] = 4;
    CopyMat_1F10((sMat_1F10*)(packet + 10), BezMat);
    packet[18] = n;
    packet[19] = 0;
    packet[20] = 0;
    packet[21] = (unsigned long)(((unsigned int)D_3BF0 >> 3) | 0x14000000) << 32;
    packet += 22;
    self->bufAddr = (unsigned int)packet & 0x0FFFFFFF;
    sPtRing_1F10* ring = self->ring;
    unsigned int next = self->bufAddr;
    sPtRS_1F10* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_1F10* ringEntry = (sPtEnt_1F10*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawSphere);
#ifdef SKIP_ASM
struct sPt2170 {
    float x, y, z, w;
    sPt2170() {}
} __attribute__((aligned(16)));

struct sMat2170 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVtx2170 {
    float s, t, q, pad;     // 0x00
    int r, g, b, a;         // 0x10
    sPt2170 pos;            // 0x20
    sVtx2170() {}
};


struct cRMgr2170 {
    char pad_0x0[0x10D8];
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
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual sMat2170* v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73(int n, sVtx2170* v, int f);
};

extern sPt2170 D_004FFBC0[];
extern int D_004A59A0;

// PORT: PS2-only VU0 inline asm (matrix copy); the PC port needs a C fallback.
static inline void CopyMat2170(sMat2170* dst, const sMat2170* src)
{
    __asm__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector); the PC port needs a C fallback.
static inline sPt2170 MulMat2170(const sMat2170* m, const sPt2170& v)
{
    sPt2170 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar); the PC port needs a C fallback.
static inline sPt2170 Scale2170(const sPt2170& v, float s)
{
    sPt2170 r;
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

// PORT: PS2-only VU0 inline asm (vector add); the PC port needs a C fallback.
static inline sPt2170 Add2170(const sPt2170& a, const sPt2170& b)
{
    sPt2170 r;
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
 * Draws a debug sphere as a camera-facing circle.
 *
 * Transforms @p pos by the view matrix (virtual GetView, slot 35) and returns when its view depth is more than 3000.
 * @p segs > 0 sets the step through the 16-point circle table; otherwise the step is 4 beyond 2000 units, 2 beyond 750
 * and 1 nearer. The first call fills the static unit-circle table D_004FFBC0 (flag D_004A59A0).
 *
 * Sets the current draw state untextured (texture id -1, bits 0x7C of its word 1 to 4), pushes the view and loads the
 * identity (slots 31 and 37), then builds the points centre + table[k] * radius in view space, closed back to the
 * first, coloured from @p col (A, R, G, B scaled by 128), and draws them with slot 73 (cPS2GraphicsMan_RenderLineStrip) before popping
 * the view (slot 32).
 *
 * @param mgr     the graphics manager (this)
 * @param pos     the centre
 * @param col     the colour, four floats A, R, G, B (On Tour tARGB)
 * @param radius  the radius
 * @param segs    the detail step, or 0 to pick it from the distance
 */
extern "C" void cPS2GraphicsMan_DrawSphere(cPS2GraphicsMan* mgr, sPt2170* pos, float* col, float radius, int segs)
{
    sMat2170 viewMat;
    int step;
    int i;
    int k;

    CopyMat2170(&viewMat, ((cRMgr2170*)mgr)->v35());
    sPt2170 viewCenter = MulMat2170(&viewMat, *pos);
    if (3000.0f < viewCenter.z) {
        return;
    }
    if (segs > 0) {
        step = segs;
    } else if (2000.0f < viewCenter.z) {
        step = 4;
    } else if (750.0f < viewCenter.z) {
        step = 2;
    } else {
        step = 1;
    }
    if (D_004A59A0 == 0) {
        D_004FFBC0[0].x = 0.0f;
        D_004FFBC0[0].y = 1.0f;
        D_004FFBC0[0].z = 0.0f;
        D_004FFBC0[0].w = 0.0f;
        D_004FFBC0[1].x = 0.3799999952316284f;
        D_004FFBC0[1].y = 0.9200000166893005f;
        D_004FFBC0[1].z = 0.0f;
        D_004FFBC0[1].w = 0.0f;
        D_004FFBC0[2].x = 0.7099999785423279f;
        D_004FFBC0[2].y = 0.7099999785423279f;
        D_004FFBC0[2].z = 0.0f;
        D_004FFBC0[2].w = 0.0f;
        D_004FFBC0[3].x = 0.9200000166893005f;
        D_004FFBC0[3].y = 0.3799999952316284f;
        D_004FFBC0[3].z = 0.0f;
        D_004FFBC0[3].w = 0.0f;
        D_004FFBC0[4].x = 1.0f;
        D_004FFBC0[4].y = 0.0f;
        D_004FFBC0[4].z = 0.0f;
        D_004FFBC0[4].w = 0.0f;
        D_004FFBC0[5].x = 0.9200000166893005f;
        D_004FFBC0[5].y = -0.3799999952316284f;
        D_004FFBC0[5].z = 0.0f;
        D_004FFBC0[5].w = 0.0f;
        D_004FFBC0[6].x = 0.7099999785423279f;
        D_004FFBC0[6].y = -0.7099999785423279f;
        D_004FFBC0[6].z = 0.0f;
        D_004FFBC0[6].w = 0.0f;
        D_004FFBC0[7].x = 0.3799999952316284f;
        D_004FFBC0[7].y = -0.9200000166893005f;
        D_004FFBC0[7].z = 0.0f;
        D_004FFBC0[7].w = 0.0f;
        D_004FFBC0[8].x = 0.0f;
        D_004FFBC0[8].y = -1.0f;
        D_004FFBC0[8].z = 0.0f;
        D_004FFBC0[8].w = 0.0f;
        D_004FFBC0[9].x = -0.3799999952316284f;
        D_004FFBC0[9].y = -0.9200000166893005f;
        D_004FFBC0[9].z = 0.0f;
        D_004FFBC0[9].w = 0.0f;
        D_004FFBC0[10].x = -0.7099999785423279f;
        D_004FFBC0[10].y = -0.7099999785423279f;
        D_004FFBC0[10].z = 0.0f;
        D_004FFBC0[10].w = 0.0f;
        D_004FFBC0[11].x = -0.9200000166893005f;
        D_004FFBC0[11].y = -0.3799999952316284f;
        D_004FFBC0[11].z = 0.0f;
        D_004FFBC0[11].w = 0.0f;
        D_004FFBC0[12].x = -1.0f;
        D_004FFBC0[12].y = 0.0f;
        D_004FFBC0[12].z = 0.0f;
        D_004FFBC0[12].w = 0.0f;
        D_004FFBC0[13].x = -0.9200000166893005f;
        D_004FFBC0[13].y = 0.3799999952316284f;
        D_004FFBC0[13].z = 0.0f;
        D_004FFBC0[13].w = 0.0f;
        D_004FFBC0[14].x = -0.7099999785423279f;
        D_004FFBC0[14].y = 0.7099999785423279f;
        D_004FFBC0[14].z = 0.0f;
        D_004FFBC0[14].w = 0.0f;
        D_004FFBC0[15].x = -0.3799999952316284f;
        D_004FFBC0[15].y = 0.9200000166893005f;
        D_004FFBC0[15].z = 0.0f;
        D_004FFBC0[15].w = 0.0f;
        D_004A59A0 = 1;
    }
    (mgr->m_pCurrentDrawState)->m_textureState.m_texid[0] = -1;
    (mgr->m_pCurrentDrawState)->unk_00[1] = (((int)(mgr->m_pCurrentDrawState)->unk_00[1]) & ~0x7C) | 4;
    ((cRMgr2170*)mgr)->v31();
    ((cRMgr2170*)mgr)->v37();

    sVtx2170 verts[17];
    i = 0;
    for (k = 0; k < 16; k += step) {
        verts[i].r = (int)(col[1] * 128.0f);
        verts[i].g = (int)(col[2] * 128.0f);
        verts[i].b = (int)(col[3] * 128.0f);
        verts[i].a = (int)(col[0] * 128.0f);
        sPt2170 circlePoint = Add2170(viewCenter, Scale2170(D_004FFBC0[k], radius));
        verts[i].pos = circlePoint;
        i++;
    }
    verts[i].r = (int)(col[1] * 128.0f);
    verts[i].g = (int)(col[2] * 128.0f);
    verts[i].b = (int)(col[3] * 128.0f);
    verts[i].a = (int)(col[0] * 128.0f);
    sPt2170 circlePoint = Add2170(viewCenter, Scale2170(D_004FFBC0[0], radius));
    verts[i].pos = circlePoint;
    i++;
    ((cRMgr2170*)mgr)->v73(i, verts, 0);
    (mgr->m_pCurrentDrawState)->unk_00[1] = (((int)(mgr->m_pCurrentDrawState)->unk_00[1]) & ~0x7C) | 4;
    ((cRMgr2170*)mgr)->v32();
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003825C0);
#ifdef SKIP_ASM
extern "C" void func_003825F8(void* arg);

/**
 * Handles the VBlank-start interrupt: runs func_003825F8 on the graphics manager and returns 1.
 *
 * cPS2GraphicsMan_Init installs it as the INTC handler for cause 2 with the graphics manager as its argument (handler
 * id in mIntcHandlerId). Any other @p cause hits a break 0 trap. It re-enables interrupts (sync.l; ei) before
 * returning.
 *
 * @param cause  the interrupt cause (must be 2)
 * @param graphicsMan  the cPS2GraphicsMan the handler was installed with
 * @return 1
 */
extern "C" int func_003825C0(int cause, void* graphicsMan)
{
    if (cause != 2) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    func_003825F8(graphicsMan);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003825F8);
#ifdef SKIP_ASM
extern "C" void iSignalSema(int sema);

/**
 * Counts a vertical blank and, once a finished frame has waited mMinFlipWait blanks, wakes the DMA kick thread to flip.
 *
 * Increments mVblankCounter. When mKickState is 4 (both DMA phases done) and the counter has reached mMinFlipWait, sets
 * mKickState to 5, clears the counter and signals mKickSema (iSignalSema). Called from the VBlank handler
 * func_003825C0.
 */
extern "C" void func_003825F8(void* self)
{
    int vblanks = ++*(int*)((char*)self + 0x5ABC);
    if (*(volatile int*)((char*)self + 0x5A8C) == 4) {
        if (vblanks >= *(int*)((char*)self + 0x5AB8)) {
            *(volatile int*)((char*)self + 0x5A8C) = 5;
            *(int*)((char*)self + 0x5ABC) = 0;
            iSignalSema(*(int*)((char*)self + 0x5AC8));
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382650);
#ifdef SKIP_ASM
extern "C" void func_00382688(void* arg);

/**
 * Handles the end of a VIF1 DMA transfer: runs func_00382688 on the graphics manager and returns 1.
 *
 * cPS2GraphicsMan_Init installs it as the DMAC handler for channel 1 (VIF1) with the graphics manager as its argument
 * (handler id in mDmacHandlerId). Any other @p cause hits a break 0 trap. It re-enables interrupts (sync.l; ei) before
 * returning.
 *
 * @param cause  the DMA channel that finished (must be 1)
 * @param graphicsMan  the cPS2GraphicsMan the handler was installed with
 * @return 1
 */
extern "C" int func_00382650(int cause, void* graphicsMan)
{
    if (cause != 1) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    func_00382688(graphicsMan);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382688);
#ifdef SKIP_ASM
extern "C" void iSignalSema(int sema);

/**
 * Advances the frame's DMA state when a VIF1 transfer ends.
 *
 * mKickState 1 (first phase sent) becomes 2 and mKickSema is signalled (iSignalSema) so the kick thread starts the
 * second phase; 3 (second phase sent) becomes 4, after which func_003825F8 waits for the flip. Called from the VIF1 DMA
 * handler func_00382650.
 */
extern "C" void func_00382688(void* self)
{
    if (*(volatile int*)((char*)self + 0x5A8C) == 1) {
        *(volatile int*)((char*)self + 0x5A8C) = 2;
        iSignalSema(*(int*)((char*)self + 0x5AC8));
    } else if (*(volatile int*)((char*)self + 0x5A8C) == 3) {
        *(volatile int*)((char*)self + 0x5A8C) = 4;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003826E0);
#ifdef SKIP_ASM
/**
 * Returns 1 when the packet buffer being built (mBufState[mBuildIndex]) is free or already being built, marking a free
 * one as being built (0 -> 1); returns 0 while it is queued or being sent.
 *
 * cPS2GraphicsMan_IsRenderReady__FPv returns its result.
 *
 * @return 1 when rendering into the buffer may start, else 0
 */
extern "C" int func_003826E0(void* self)
{
    volatile int* arr = (volatile int*)((char*)self + 0x5a90);
    volatile int* bufState = &arr[*(int*)((char*)self + 0x5a10)];
    if (*bufState == 0 || *bufState == 1) {
        if (*bufState == 0) {
            *bufState = 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382730);
#ifdef SKIP_ASM
/**
 * Returns 1 when no frame is being sent (mKickState is 0), else 0.
 *
 * cPS2GraphicsMan_IsRenderIdle__FPv returns its result.
 *
 * @return 1 when the DMA kick state is idle
 */
extern "C" int func_00382730(void* self)
{
    return *(volatile unsigned int*)((char*)self + 0x5a8c) == 0;
}
#endif

extern "C" void* func_00382760(cPS2GraphicsMan* self);

//100%
INCLUDE_ASM("render/particle", func_00382740__FPv);
#ifdef SKIP_ASM
/**
 * Entry point of the DMA kick thread: runs func_00382760 on the graphics manager, which never returns.
 *
 * cPS2GraphicsMan_Init creates the thread with this entry and its stack in mKickThreadStack and starts it with the
 * graphics manager as its argument.
 *
 * @return never returns
 */
void* func_00382740(void* self)
{
    return func_00382760((cPS2GraphicsMan*)self);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382760);
#ifdef SKIP_ASM
struct sPtQuad_2760 {
    unsigned int w[4];
} __attribute__((aligned(16)));

struct sPtKickGfx_2760 {
    char pad_0x0[0x5A8C];
    volatile int kickState;                  // 0x5A8C (also written by the VIF1-end / VBlank IRQ handlers)
    volatile int bufState[2];                // 0x5A90
    volatile int inFlightIndex;              // 0x5A98
    int inFlightVif2;               // 0x5A9C
    int inFlightGif2;               // 0x5AA0
    volatile int queuedIndex;                // 0x5AA4
    int queuedVif1;                 // 0x5AA8
    int queuedVif2;                 // 0x5AAC
    int queuedGif1;                 // 0x5AB0
    int queuedGif2;                 // 0x5AB4
    char pad_0x5AB8[0x5AC8 - 0x5AB8];
    int kickSema;                   // 0x5AC8
    int queueMutex;                 // 0x5ACC
    char pad_0x5AD0[0x6D0C - 0x5AD0];
    void (*phase2Callback)(int);    // 0x6D0C
    int phase2CallbackArg;          // 0x6D10
};

extern sPtQuad_2760 D_0044B9C0;
extern "C" void* WaitSema(int sema);
extern "C" void SignalSema(int sema);
extern "C" void FlushCache(int);
extern "C" void func_00382AF0(void* self, int a, int b);

/**
 * Runs the DMA kick thread: sends each queued frame to the GS in two VIF1 + GIF DMA phases and flips the display when
 * it is done.
 *
 * Loops forever: waits on mKickSema, then takes mQueueMutex. When mKickState is 5 (frame sent and the flip wait over)
 * it frees the in-flight buffer (mBufState[mInFlightIndex] = 0), sets the state to 0 and flips with func_00382AF0(self,
 * 1, 0). When idle and the queued buffer is ready (mBufState[mQueuedIndex] == 2) it waits for the VIF1 and GIF
 * channels, VIF1 and the GS FIFO to go idle, makes that buffer the in-flight one (state 3), copies its second-phase
 * addresses (mInFlightVif2, mInFlightGif2), sets mKickState to 1, flushes the cache, pushes the quadword D_0044B9C0
 * into the VIF1 FIFO, sets GIF_MODE to 4 and starts the GIF (mQueuedGif1) and VIF1 (mQueuedVif1) chain DMAs. When
 * mKickState is 2 (first phase done) it calls mPhase2Callback(mPhase2CallbackArg) if set, does the same FIFO/GIF_MODE
 * setup, starts the second-phase chains and sets the state to 3. Then releases mQueueMutex. The interrupt handlers
 * (func_00382688, func_003825F8) move the state on and signal mKickSema.
 *
 * @param selfp  the cPS2GraphicsMan
 * @return never returns
 */
// Renderer_DmaKickThread in REF structs/render.md.
// PORT: PS2 hardware registers (D1/D2/D8 CHCR+TADR, VIF1 FIFO/STAT, GIF_MODE, GS CSR).
extern "C" void* func_00382760(cPS2GraphicsMan* selfp)
{
    sPtKickGfx_2760* self = (sPtKickGfx_2760*)selfp;
    for (;;) {
        WaitSema(self->kickSema);
        WaitSema(self->queueMutex);
        if (self->kickState == 5) {
            self->bufState[self->inFlightIndex] = 0;
            self->kickState = 0;
            func_00382AF0(self, 1, 0);
        }
        if (self->kickState == 0 && self->bufState[self->queuedIndex] == 2) {
            while (*(volatile int*)0x10009000 & 0x100) {
            }
            while (*(volatile int*)0x1000A000 & 0x100) {
            }
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & 0xC000) != 0x4000) {
            }
            self->inFlightIndex = self->queuedIndex;
            self->bufState[self->inFlightIndex] = 3;
            self->kickState = 1;
            self->inFlightVif2 = self->queuedVif2;
            self->inFlightGif2 = self->queuedGif2;
            FlushCache(0);
            while (*(volatile int*)0x1000D000 & 0x100) {
            }
            *(sPtQuad_2760*)0x10005000 = D_0044B9C0;
            *(volatile int*)0x10003010 = 4;
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & 0xC000) != 0x4000) {
            }
            *(volatile int*)0x1000A030 = self->queuedGif1;
            *(volatile int*)0x1000A000 = 261;
            *(volatile int*)0x10009030 = self->queuedVif1;
            *(volatile int*)0x10009000 = 389;
        } else if (self->kickState == 2) {
            if (self->phase2Callback) {
                self->phase2Callback(self->phase2CallbackArg);
            }
            *(sPtQuad_2760*)0x10005000 = D_0044B9C0;
            *(volatile int*)0x10003010 = 4;
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & 0xC000) != 0x4000) {
            }
            *(volatile int*)0x1000A030 = self->inFlightGif2;
            *(volatile int*)0x1000A000 = 261;
            self->kickState = 3;
            *(volatile int*)0x10009030 = self->inFlightVif2;
            *(volatile int*)0x10009000 = 389;
        }
        SignalSema(self->queueMutex);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382AF0);
#ifdef SKIP_ASM
// PORT: PS2 hardware registers (DMA CHCR, VIF1_STAT, GS privileged PMODE/SMODE2/DISPFB1/DISPLAY1/BGCOLOR/CSR),
// an uncached-accelerated packet buffer (| 0x30000000) and 64-bit `ulong` GS packet words (8 bytes on EE).
// The `(ulong)float` casts in DISPLAY1 compile to libgcc's __fixunssfdi (func_00411340).
// PS2GraphicsMan fields (no shared header yet; names from refs/ssx3-rust docs/investigation/structs/render.md):
// 0x0/0x4 screenOffset x/y, 0xF44 forceDisplayFlip, 0x5900 scissor x,y,w,h (u16), 0x59D0 dither, 0x59E0 dimx (u64),
// 0x59E8 blitToDisplay, 0x59EC brightness r,g,b, 0x59F8 alpha, 0x59FC brightness scale, 0x5A30/0x5A34 fbWidth/Height,
// 0x5A38 displayPsm, 0x5A3C drawPsm, 0x5A40 zPsm, 0x5A44 interlace, 0x5A46 ffmd, 0x5A4C progressive, 0x5A50 fbw,
// 0x5A54/0x5A58 dispWidth/Height, 0x5A5C dispFbw, 0x5A60 magv, 0x5A64 magh, 0x5A74 vsyncFlipCount, 0x5A78/0x5A7C fbp0/1,
// 0x5A80 zbp, 0x5A84 drawFbp, 0x5A88 dispFbp.
extern "C" void func_00371940(unsigned int madr, int qwc, int flags);

extern ulong D_004FFCC0[];

static inline int clamp_382AF0(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// GS primitive coordinate: 12.4 fixed point, centred on 2048.
static inline int gsCoord_382AF0(int v)
{
    return (v << 4) + 0x8000;
}

/**
 * Flips the display buffers, programs the CRTC registers and sends the per-frame GS setup packet.
 *
 * Increments mCurrentFrameCount. Unless mSkipNextPageFlip is set (it is then cleared), picks the buffers: with
 * mFilterEnabled clear and an even count the draw page (mCurrentFrameBufferPage) is unk_5A7C and the display page
 * (mDisplayBufferAddress) unk_5A78, otherwise the other way round. Waits for the VIF1, GIF and fromSPR DMA channels,
 * VIF1 and the GS FIFO to go idle, and for the next vertical sync when @p waitVsync is set. Writes PMODE (EN1 = @p en1,
 * MMOD 1, ALP 0xFF), SMODE2 (interlace unk_5A44, field mode unk_5A46), DISPFB1 (display page, unk_5A5C width, unk_5A38
 * format), DISPLAY1 (position from m_screenOffset; in progressive mode, unk_5A4C, the magnification fits the display
 * width into 1550 clocks, else MAGH unk_5A64, MAGV unk_5A60 and a height doubled in interlaced field mode) and BGCOLOR
 * 0.
 *
 * Then builds a GS packet in D_004FFCC0 (uncached accelerated): when mFilterEnabled is set, a pass that copies the draw
 * buffer into the display buffer as 32-pixel-wide textured sprite strips coloured by the brightness unk_59EC times
 * mScreenBrightness and alpha unk_59F8 (blended when not 128); the draw context (FRAME_1/2, ZBUF_1/2, XYOFFSET_1/2,
 * FBA, TEXA, PRMODECONT, COLCLAMP, DTHE from unk_59D0, DIMX from unk_59E0, PABE, SCANMSK, TEXFLUSH); a clear of the
 * frame and z buffer in 32-pixel sprite strips with mClearColour (z test always); TEST_1 0x500AD (alpha greater than
 * 10, z GEQUAL); and SCISSOR_1/2 from mScissor. Sends it with func_00371940. The kick thread func_00382760,
 * func_0037BD98 and cPS2GraphicsMan_PageFlip call it.
 *
 * @param vself  the cPS2GraphicsMan
 * @param en1  PMODE.EN1: 1 to enable read circuit 1
 * @param waitVsync  nonzero to wait for a vertical sync before writing the registers
 */
// Renderer_SetDisplayRegs in REF structs/render.md.
extern "C" void func_00382AF0(void* vself, int en1, int waitVsync)
{
    char* self = (char*)vself;
    ulong* gsPacket;
    int count;

    count = ++*(int*)(self + 0x5A74);
    if (*(int*)(self + 0xF44) != 0) {
        *(int*)(self + 0xF44) = 0;
    } else {
        if (*(int*)(self + 0x59E8) == 0 && (count & 1) == 0) {
            *(unsigned int*)(self + 0x5A84) = *(unsigned int*)(self + 0x5A7C);
            *(unsigned int*)(self + 0x5A88) = *(unsigned int*)(self + 0x5A78);
        } else {
            *(unsigned int*)(self + 0x5A84) = *(unsigned int*)(self + 0x5A78);
            *(unsigned int*)(self + 0x5A88) = *(unsigned int*)(self + 0x5A7C);
        }
    }

    while (*(volatile int*)0x10009000 & 0x100) { // D1_CHCR.STR (VIF1)
    }
    while (*(volatile int*)0x1000A000 & 0x100) { // D2_CHCR.STR (GIF)
    }
    while (*(volatile int*)0x1000D000 & 0x100) { // D8_CHCR.STR (fromSPR)
    }
    while (*(volatile int*)0x10003C00 & 0x1F000003) { // VIF1_STAT
    }
    while ((*(volatile ulong*)0x12001000 & 0xC000) != 0x4000) { // GS CSR.FIFO empty
    }
    if (waitVsync != 0) {
        *(volatile ulong*)0x12001000 = 8; // CSR.VSINT
        while (!(*(volatile ulong*)0x12001000 & 8)) {
        }
        *(volatile ulong*)0x12001000;
    }

    // PMODE: EN1, MMOD 1, ALP 0xFF
    {
        ulong pmode = (unsigned int)en1;
        *(volatile ulong*)0x12000000 = pmode | ((ulong)1 << 5) | ((ulong)0xFF << 8);
    }
    // SMODE2: INT, FFMD
    *(volatile ulong*)0x12000020 = (ulong)*(short*)(self + 0x5A44) | ((ulong)*(short*)(self + 0x5A46) << 1);
    // DISPFB1: FBP, FBW, PSM
    *(volatile ulong*)0x12000070 = (ulong)*(unsigned int*)(self + 0x5A88)
                                 | ((ulong)*(unsigned int*)(self + 0x5A5C) << 9)
                                 | ((ulong)*(unsigned int*)(self + 0x5A38) << 15);
    // DISPLAY1: DX, DY, MAGH, MAGV, DW, DH
    if (*(int*)(self + 0x5A4C) != 0) {
        int w = *(unsigned int*)(self + 0x5A54);
        int displayHeight = *(unsigned int*)(self + 0x5A58);
        *(volatile ulong*)0x12000080 = (ulong)((int)(*(float*)(self + 0x0) * 4.0f + -43.0f) + 344)
                                     | ((ulong)((int)(12.0f - *(float*)(self + 0x4) * 2.0f) + 35) << 12)
                                     | ((ulong)(1550 / w - 1) << 23)
                                     | ((ulong)(w * (1550 / w) - 1) << 32)
                                     | ((ulong)(displayHeight - 1) << 44);
    } else {
        int fieldMul = 1;
        if (*(short*)(self + 0x5A44) != 0) {
            fieldMul = *(short*)(self + 0x5A46) == 1 ? 2 : 1;
        }
        *(volatile ulong*)0x12000080 = (ulong)(*(float*)(self + 0x0) * 8.0f + 5.0f + 636.0f)
                                     | ((ulong)(50.0f - *(float*)(self + 0x4) * 2.0f) << 12)
                                     | ((ulong)*(unsigned int*)(self + 0x5A64) << 23)
                                     | ((ulong)*(unsigned int*)(self + 0x5A60) << 27)
                                     | ((ulong)2560 << 32)
                                     | ((ulong)((*(unsigned int*)(self + 0x5A34) - 1) * fieldMul) << 44);
    }
    // BGCOLOR
    *(volatile ulong*)0x120000E0 = 0;

    gsPacket = (ulong*)((unsigned int)D_004FFCC0 | 0x30000000);

    if (*(int*)(self + 0x59E8) != 0) {
        // Copy the draw buffer into the display buffer: GIFtag (11 x A+D)
        gsPacket[0] = ((ulong)1 << 60) | 11;
        gsPacket[1] = 0xE;
        gsPacket[2] = (ulong)*(unsigned int*)(self + 0x5A88) | ((ulong)*(unsigned int*)(self + 0x5A5C) << 16)
             | ((ulong)*(unsigned int*)(self + 0x5A38) << 24) | ((ulong)0xFF000000 << 32);
        gsPacket[3] = 76; // FRAME_1 (display buffer)
        gsPacket[4] = 97;
        gsPacket[5] = 20; // TEX1_1
        gsPacket[6] = (ulong)(*(unsigned int*)(self + 0x5A84) << 5) | ((ulong)*(unsigned int*)(self + 0x5A50) << 14)
             | ((ulong)*(unsigned int*)(self + 0x5A3C) << 20) | ((ulong)10 << 26) | ((ulong)9 << 30);
        gsPacket[7] = 6; // TEX0_1 (draw buffer)
        gsPacket[8] = ((ulong)0x80 << 32) | 0x64;
        gsPacket[9] = 66; // ALPHA_1
        gsPacket[10] = 5;
        gsPacket[11] = 8; // CLAMP_1
        gsPacket[12] = ((ulong)(*(unsigned int*)(self + 0x5A54) - 1) << 16) | ((ulong)(*(unsigned int*)(self + 0x5A58) - 1) << 48);
        gsPacket[13] = 0x40; // SCISSOR_1
        gsPacket[14] = ((ulong)(*(unsigned int*)(self + 0x5A54) - 1) << 16) | ((ulong)(*(unsigned int*)(self + 0x5A58) - 1) << 48);
        gsPacket[15] = 65; // SCISSOR_2
        gsPacket[16] = (ulong)((0x800 - (*(unsigned int*)(self + 0x5A54) >> 1)) << 4)
              | ((ulong)((0x800 - (*(unsigned int*)(self + 0x5A58) >> 1)) << 4) << 32);
        gsPacket[17] = 0x18; // XYOFFSET_1
        gsPacket[18] = (ulong)*(unsigned int*)(self + 0x5A80) | ((ulong)(*(unsigned int*)(self + 0x5A40) - 0x30) << 24) | ((ulong)1 << 32);
        gsPacket[19] = 78; // ZBUF_1
        gsPacket[20] = 0x30000;
        gsPacket[21] = 71; // TEST_1
        gsPacket[22] = 0;
        gsPacket[23] = 0x3F; // TEXFLUSH
        {
            int abe = *(unsigned int*)(self + 0x59F8) != 0x80;
            // GIFtag: 1 loop, REGLIST {PRIM, RGBAQ}
            gsPacket[24] = ((ulong)2 << 60) | ((ulong)1 << 58) | 1;
            gsPacket[25] = 0x10;
            gsPacket[26] = ((ulong)abe << 6) | 0x11E; // sprite, IIP, TME, FST
        }
        {
            int scaledRed = clamp_382AF0((int)((float)*(unsigned int*)(self + 0x59EC) * *(float*)(self + 0x59FC)), 0, 255);
            int scaledGreen = clamp_382AF0((int)((float)*(unsigned int*)(self + 0x59F0) * *(float*)(self + 0x59FC)), 0, 255);
            int scaledBlue = clamp_382AF0((int)((float)*(unsigned int*)(self + 0x59F4) * *(float*)(self + 0x59FC)), 0, 255);
            union { float f; int i; } unitQ;
            unitQ.f = 1.0f;
            gsPacket[27] = (ulong)scaledRed | ((ulong)scaledGreen << 8) | ((ulong)scaledBlue << 16) | ((ulong)*(unsigned int*)(self + 0x59F8) << 24)
                  | ((ulong)unitQ.i << 32);
        }
        {
            // Vertical sprite strips, 32 pixels wide.
            unsigned int w = *(unsigned int*)(self + 0x5A54);
            int stripW = 32;
            int n = w / stripW;
            int du = (*(unsigned int*)(self + 0x5A30) / n) << 4;
            int x0 = gsCoord_382AF0(-(w >> 1));
            int ybase = 0x8000 - ((*(unsigned int*)(self + 0x5A58) >> 1) << 4);
            int y0;
            int uoff;
            int xoff;
            int yoff;
            int x;
            int stripU0;
            int stripU1;
            int stripV1;
            int y1;
            int i;
            if (*(int*)(self + 0x5A4C) != 0) {
                uoff = 8;
                xoff = 0;
                yoff = 0;
            } else {
                uoff = 0;
                xoff = -8;
                yoff = -8;
            }
            y0 = yoff + ybase;
            x = xoff + x0;
            stripV1 = ((*(unsigned int*)(self + 0x5A34) - 1) << 4) + 8;
            y1 = y0 + ((*(unsigned int*)(self + 0x5A58) - 1) << 4);
            stripU0 = uoff;
            stripU1 = stripU0 + du;
            // GIFtag: n + 1 loops, REGLIST {UV, XYZ2, UV, XYZ2}
            gsPacket[28] = (ulong)(n + 1) | ((ulong)1 << 58) | ((ulong)4 << 60);
            gsPacket[29] = 21331;
            gsPacket += 30;
            for (i = 0; i < n; i++) {
                gsPacket[0] = (ulong)stripU0 | ((ulong)8 << 16);
                gsPacket[1] = (ulong)x | ((ulong)y0 << 16);
                gsPacket[2] = (ulong)stripU1 | ((ulong)stripV1 << 16);
                gsPacket[3] = (ulong)(x + 0x200) | ((ulong)y1 << 16);
                x += 0x200;
                stripU0 += du;
                stripU1 += du;
                gsPacket += 4;
            }
            gsPacket[0] = (ulong)uoff | ((ulong)((*(unsigned int*)(self + 0x5A34) << 4) - 8) << 16);
            gsPacket[1] = (ulong)(unsigned int)gsCoord_382AF0(-(*(unsigned int*)(self + 0x5A54) >> 1))
                 | ((ulong)(((*(unsigned int*)(self + 0x5A58) >> 1) << 4) + 32744) << 16) | ((ulong)0x8000 << 32);
            gsPacket[2] = (ulong)(*(unsigned int*)(self + 0x5A30) << 4) | ((ulong)((*(unsigned int*)(self + 0x5A34) << 4) - 8) << 16);
            gsPacket[3] = (ulong)(((*(unsigned int*)(self + 0x5A54) >> 1) << 4) + 0x8000)
                 | ((ulong)(((*(unsigned int*)(self + 0x5A58) >> 1) << 4) + 0x7FF8) << 16) | ((ulong)0x8000 << 32);
            gsPacket += 4;
        }
    }

    // Draw context: GIFtag (16 x A+D, EOP)
    gsPacket[0] = ((ulong)1 << 60) | 0x8000 | 16;
    gsPacket[1] = 0xE;
    gsPacket[2] = (ulong)*(unsigned int*)(self + 0x5A84) | ((ulong)*(unsigned int*)(self + 0x5A50) << 16)
         | ((ulong)*(unsigned int*)(self + 0x5A3C) << 24);
    gsPacket[3] = 76; // FRAME_1
    gsPacket[4] = (ulong)*(unsigned int*)(self + 0x5A84) | ((ulong)*(unsigned int*)(self + 0x5A50) << 16)
         | ((ulong)*(unsigned int*)(self + 0x5A3C) << 24) | ((ulong)0xFF000000 << 32);
    gsPacket[5] = 77; // FRAME_2
    gsPacket[6] = (ulong)*(unsigned int*)(self + 0x5A80) | ((ulong)(*(unsigned int*)(self + 0x5A40) - 0x30) << 24);
    gsPacket[7] = 78; // ZBUF_1
    gsPacket[8] = (ulong)*(unsigned int*)(self + 0x5A80) | ((ulong)(*(unsigned int*)(self + 0x5A40) - 0x30) << 24);
    gsPacket[9] = 79; // ZBUF_2
    gsPacket[10] = (ulong)((0x800 - (*(unsigned int*)(self + 0x5A30) >> 1)) << 4)
          | ((ulong)((0x800 - (*(unsigned int*)(self + 0x5A34) >> 1)) << 4) << 32);
    gsPacket[11] = 0x18; // XYOFFSET_1
    gsPacket[12] = (ulong)((0x800 - (*(unsigned int*)(self + 0x5A30) >> 1)) << 4)
          | ((ulong)((0x800 - (*(unsigned int*)(self + 0x5A34) >> 1)) << 4) << 32);
    gsPacket[13] = 25; // XYOFFSET_2
    gsPacket[14] = 0;
    gsPacket[15] = 74; // FBA_1
    gsPacket[16] = 0;
    gsPacket[17] = 75; // FBA_2
    gsPacket[18] = ((ulong)0x80 << 32) | 0x80;
    gsPacket[19] = 59; // TEXA
    gsPacket[20] = 1;
    gsPacket[21] = 26; // PRMODECONT
    gsPacket[22] = 1;
    gsPacket[23] = 70; // COLCLAMP
    gsPacket[24] = *(unsigned int*)(self + 0x59D0);
    gsPacket[25] = 69; // DTHE
    gsPacket[26] = *(ulong*)(self + 0x59E0);
    gsPacket[27] = 68; // DIMX
    gsPacket[28] = 0;
    gsPacket[29] = 73; // PABE
    gsPacket[30] = 0;
    gsPacket[31] = 34; // SCANMSK
    gsPacket[32] = 0;
    gsPacket[33] = 0x3F; // TEXFLUSH
    gsPacket += 34;
    {
        // Brightness pass: sprite strips over the frame, 32 pixels wide.
        unsigned int tw = *(unsigned int*)(self + 0x5A30);
        unsigned int th = *(unsigned int*)(self + 0x5A34);
        int n = tw >> 5;
        int oy = 0x800 - (th >> 1);
        int x = (0x800 - (tw >> 1)) << 4;
        int y0 = oy << 4;
        int y1 = (th + oy) << 4;
        // GIFtag: 2n + 4 x A+D, EOP
        gsPacket[0] = (ulong)(n * 2 + 4) | 0x8000 | ((ulong)1 << 60);
        gsPacket[1] = 0xE;
        gsPacket[2] = 0x30000;
        gsPacket[3] = 71; // TEST_1
        gsPacket[4] = 6;
        gsPacket[5] = 0; // PRIM: sprite
        gsPacket[6] = (ulong)(*(unsigned int*)(self + 0x6AE0) | 0x80000000) | ((ulong)0x3F800000 << 32);
        gsPacket[7] = 1; // RGBAQ
        gsPacket += 8;
        for (; n != 0; n--) {
            gsPacket[0] = (ulong)x | ((ulong)y0 << 16);
            gsPacket[1] = 5; // XYZ2
            gsPacket[2] = (ulong)(x + 0x200) | ((ulong)y1 << 16);
            gsPacket[3] = 5; // XYZ2
            x += 0x200;
            gsPacket += 4;
        }
    }
    gsPacket[0] = 0x500AD;
    gsPacket[1] = 71; // TEST_1
    gsPacket += 2;
    // GIFtag (2 x A+D, EOP): SCISSOR_1/2 from the scissor rect
    gsPacket[0] = ((ulong)1 << 60) | 0x8000 | 2;
    gsPacket[1] = 0xE;
    gsPacket[2] = (ulong)*(unsigned short*)(self + 0x5900)
         | ((ulong)(*(unsigned short*)(self + 0x5900) + (*(unsigned short*)(self + 0x5904) - 1)) << 16)
         | ((ulong)*(unsigned short*)(self + 0x5902) << 32)
         | ((ulong)(*(unsigned short*)(self + 0x5902) + (*(unsigned short*)(self + 0x5906) - 1)) << 48);
    gsPacket[3] = 0x40;
    gsPacket[4] = (ulong)*(unsigned short*)(self + 0x5900)
         | ((ulong)(*(unsigned short*)(self + 0x5900) + (*(unsigned short*)(self + 0x5904) - 1)) << 16)
         | ((ulong)*(unsigned short*)(self + 0x5902) << 32)
         | ((ulong)(*(unsigned short*)(self + 0x5902) + (*(unsigned short*)(self + 0x5906) - 1)) << 48);
    gsPacket[5] = 65;
    gsPacket += 6;
    {
        unsigned int size = ((unsigned int)gsPacket & 0x0FFFFFFF) - (unsigned int)D_004FFCC0;
        __asm__ __volatile__("sync.l"); // PORT: EE memory barrier before the DMA kick
        func_00371940((unsigned int)D_004FFCC0, size >> 4, 6);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00383770);
#ifdef SKIP_ASM
extern "C" void func_00383808(void *, int, int, int, int, int, int, int, int);
extern "C" void FlushCache(int);

/**
 * Uploads an image from main memory into the current draw buffer at (@p destX, @p destY) and returns 1.
 *
 * Flushes the data cache and calls func_00383808 with the draw buffer (mCurrentFrameBufferPage << 5 blocks), mFBWidth
 * and mCurrentFBFormat. Nothing in the retail code references it.
 *
 * @param graphicsMan  the cPS2GraphicsMan
 * @param destX  destination x in pixels
 * @param destY  destination y in pixels
 * @param width  image width in pixels
 * @param height  image height in pixels
 * @param src  address of the pixels
 * @return 1
 */
extern "C" int func_00383770(void *graphicsMan, int destX, int destY, int width, int height, int src) {
    FlushCache(0);
    func_00383808(graphicsMan, src, (*(int *)((char*)(graphicsMan) + (0x5A84))) << 5, width, height, (*(int *)((char*)(graphicsMan) + (0x5A30))), (*(int *)((char*)(graphicsMan) + (0x5A3C))), destX, destY);
    return 1;
}
#endif
//100%
INCLUDE_ASM("render/particle", func_00383808);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Uploads an image to GS memory: works out bytes per row from the pixel format, waits for DMA, then builds
// GIF/DMA packets in scratchpad in chunks of at most 0x7FFF0 bytes and kicks the transfer.
extern "C" void func_00371F98(unsigned int tadr, int flags);

/**
 * Uploads an image to GS memory with GIF image transfers built in the scratchpad, in chunks of at most 0x7FFF0 bytes.
 *
 * Bytes per row come from @p psm (PSMCT32 4, PSMCT24 3, PSMCT16/16S 2, PSMT8 1, PSMT4 1/2; other formats give 0). Waits
 * for the GIF DMA channel, then writes at 0x70000000 per chunk: a DMA cnt tag, an A+D GIF tag with BITBLTBUF (@p dbp,
 * @p dbw / 64, @p psm), TRXPOS (@p dsax, @p dsay), TRXREG (@p w x rows), TEXFLUSH and TRXDIR 0, an IMAGE GIF tag, and a
 * DMA ref (or refe on the last chunk) to the rows of @p src. Kicks the chain with func_00371F98(0x70000000, 6).
 * func_00383770 calls it.
 *
 * @param src  address of the pixels
 * @param dbp  destination base pointer, in 256-byte blocks
 * @param w  width in pixels
 * @param height  height in pixels
 * @param dbw  destination buffer width in pixels
 * @param psm  GS pixel format of the data
 * @param dsax  destination x
 * @param dsay  destination y
 */
extern "C" void func_00383808(void* self, int src, int dbp, int w, int height, int dbw, int psm, int dsax, int dsay)
{
    int bpr = 0;
    unsigned long* packet = (unsigned long*)0x70000000;
    unsigned long bitbltbuf;
    int id = 3;

    switch (psm) {
    case 0:
        bpr = w << 2;
        break;
    case 1:
        bpr = w * 3;
        break;
    case 2:
    case 10:
        bpr = w << 1;
        break;
    case 19:
        bpr = w;
        break;
    case 20:
        bpr = w >> 1;
        break;
    }

    while (*(volatile int*)0x1000A000 & 0x100) {
    }
    bitbltbuf = ((unsigned long)dbp << 32) | ((unsigned long)(dbw >> 6) << 48) | ((unsigned long)psm << 56);

    do {
        int rows;
        int qwc;
        unsigned long* dmaTag = packet + 16;
        rows = 0x7FFF0 / bpr;
        if (height < rows) {
            rows = height;
        }
        packet[0] = 0x10000007;
        packet[1] = 0;
        packet[2] = 0x1000000000000005;
        packet[3] = 0xE;
        packet[4] = bitbltbuf;
        packet[5] = 80;
        packet[6] = ((unsigned long)dsax << 32) | ((unsigned long)dsay << 48);
        packet[7] = 81;
        packet[9] = 82;
        packet[10] = 0;
        packet[11] = 0x3F;
        packet[12] = 0;
        packet[13] = 83;
        packet[8] = w | ((unsigned long)rows << 32);
        height -= rows;
        qwc = (bpr * rows) >> 4;
        packet[14] = qwc | 0x8000 | 0x0800000000000000;
        packet[15] = 0;
        unsigned long srcHi = (unsigned long)src << 32;
        *dmaTag = qwc | (height != 0 ? srcHi | ((unsigned long)id << 28) : srcHi);
        dsay += rows;
        packet[17] = 0;
        packet += 18;
        src += bpr * rows;
    } while (height != 0);

    func_00371F98(0x70000000, 6);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00383A10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Clips the segment p[0]-p[1] (projected points, v[] their vertices) against the six
// guard-band planes; codes[] are the outcodes from cPS2GraphicsMan_RenderLineStrip (0x20/0x10 depth, 8/4 y, 2/1 x).
// The clipped end replaces the outside point; returns 0 when the segment is fully outside.
struct sClipPt_3A10 {
    float x, y, z, w;
};

struct sClipVtx_3A10 {
    float s, t, pad0, pad1;         // 0x00
    unsigned int c[4];              // 0x10 rgba
    float pos[4];                   // 0x20
};

struct sClipCtx_3A10 {
    char pad_0x0[0x6B70];
    float xmin;                     // 0x6B70
    float ymin;                     // 0x6B74
    float zfar;                     // 0x6B78
    float wfar;                     // 0x6B7C
    float xmax;                     // 0x6B80
    float ymax;                     // 0x6B84
    float znear;                    // 0x6B88
    float wnear;                    // 0x6B8C
};

extern "C" int func_00383A10_impl(sClipCtx_3A10* self, sClipVtx_3A10* v, sClipPt_3A10* p, int* codes) __asm__("func_00383A10");

/**
 * Clips the segment between two projected points to the guard band, moving the outside end(s) onto it, and returns 0
 * when nothing is left.
 *
 * @p codes holds the two ends' clip codes from cPS2GraphicsMan_RenderLineStrip. For each plane crossed (far 0x20 and near 0x10 in w, then
 * y min 8 and max 4, then x min 2 and max 1, bounds from unk_6B70 / unk_6B80) the end outside is moved to the crossing:
 * x, y, z and w of @p points are interpolated and s, t and the four colour ints of @p verts are interpolated with the
 * same fraction (for the w planes the fraction is corrected by the w ratio); that end's codes are recomputed, and it
 * returns 0 as soon as both ends are outside the same plane. cPS2GraphicsMan_RenderLineStrip calls it.
 *
 * @param verts  the two vertices (s, t, colour), updated in place
 * @param points  the two projected points, updated in place
 * @param codes  the two clip codes, updated in place
 * @return 1 when part of the segment is inside, 0 when it is all outside
 */
extern "C" int func_00383A10_impl(sClipCtx_3A10* self, sClipVtx_3A10* verts, sClipPt_3A10* points, int* codes)
{
    int unionCodes = codes[0] | codes[1];

    if (unionCodes & 0x20) {
        float w0 = points[0].w;
        float plane = self->wfar;
        float frac = (w0 - plane) / (w0 - points[1].w);
        int k = (codes[0] & 0x20) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dy = frac * (points[0].y - points[1].y);
        float tv = frac * points[1].w / plane;
        points[k].x = points[0].x - dx;
        points[k].y = points[0].y - dy;
        points[k].z = self->zfar;
        points[k].w = self->wfar;
        verts[k].s = verts[0].s + tv * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + tv * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + tv * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + tv * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + tv * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + tv * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
        if (points[k].x < self->xmin) codes[k] |= 2;
        if (points[k].x > self->xmax) codes[k] |= 1;
        if (points[k].y < self->ymin) codes[k] |= 8;
        if (points[k].y > self->ymax) codes[k] |= 4;
        if (codes[0] & codes[1]) return 0;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 0x10) {
        float w0 = points[0].w;
        float plane = self->wnear;
        float frac = (w0 - plane) / (w0 - points[1].w);
        int k = (codes[0] & 0x10) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dy = frac * (points[0].y - points[1].y);
        float tv = frac * points[1].w / plane;
        points[k].x = points[0].x - dx;
        points[k].y = points[0].y - dy;
        points[k].z = self->znear;
        points[k].w = self->wnear;
        verts[k].s = verts[0].s + tv * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + tv * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + tv * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + tv * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + tv * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + tv * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
        if (points[k].x < self->xmin) codes[k] |= 2;
        if (points[k].x > self->xmax) codes[k] |= 1;
        if (points[k].y < self->ymin) codes[k] |= 8;
        if (points[k].y > self->ymax) codes[k] |= 4;
        if (codes[0] & codes[1]) return 0;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 8) {
        float frac = (points[0].y - self->ymin) / (points[0].y - points[1].y);
        int k = (codes[0] & 8) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = points[0].x - dx;
        points[k].y = self->ymin;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].s = verts[0].s + frac * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + frac * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + frac * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + frac * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + frac * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + frac * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
        if (points[k].x < self->xmin) codes[k] |= 2;
        if (points[k].x > self->xmax) codes[k] |= 1;
        if (codes[0] & codes[1]) return 0;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 4) {
        float frac = (points[0].y - self->ymax) / (points[0].y - points[1].y);
        int k = (codes[0] & 4) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = points[0].x - dx;
        points[k].y = self->ymax;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].s = verts[0].s + frac * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + frac * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + frac * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + frac * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + frac * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + frac * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
        if (points[k].x < self->xmin) codes[k] |= 2;
        if (points[k].x > self->xmax) codes[k] |= 1;
        if (codes[0] & codes[1]) return 0;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 2) {
        float plane = self->xmin;
        float frac = (points[0].x - plane) / (points[0].x - points[1].x);
        int k = (codes[0] & 2) == 0;
        float dy = frac * (points[0].y - points[1].y);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = plane;
        points[k].y = points[0].y - dy;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].s = verts[0].s + frac * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + frac * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + frac * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + frac * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + frac * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + frac * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
    }
    if (unionCodes & 1) {
        float plane = self->xmax;
        float frac = (points[0].x - plane) / (points[0].x - points[1].x);
        int k = (codes[0] & 1) == 0;
        float dy = frac * (points[0].y - points[1].y);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = plane;
        points[k].y = points[0].y - dy;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].s = verts[0].s + frac * (verts[1].s - verts[0].s);
        verts[k].t = verts[0].t + frac * (verts[1].t - verts[0].t);
        *(int*)&verts[k].c[0] = (int)((float)verts[0].c[0] + frac * (float)(verts[1].c[0] - verts[0].c[0]));
        *(int*)&verts[k].c[1] = (int)((float)verts[0].c[1] + frac * (float)(verts[1].c[1] - verts[0].c[1]));
        *(int*)&verts[k].c[2] = (int)((float)verts[0].c[2] + frac * (float)(verts[1].c[2] - verts[0].c[2]));
        *(int*)&verts[k].c[3] = (int)((float)verts[0].c[3] + frac * (float)(verts[1].c[3] - verts[0].c[3]));
        codes[k] = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00384D98);
#ifdef SKIP_ASM
/**
 * Starts a packet builder that writes at the packet cursor *@p src: stores @p src, the cursor as an
 * uncached-accelerated address (| 0x30000000), and no packet and no open strip.
 *
 * cPS2GraphicsMan_RenderLineStrip opens one on mVifCursor; func_00384E50 closes it.
 *
 * @param src  where the packet cursor lives (mVifCursor)
 * @return the builder
 */
extern "C" void* func_00384D98(void* self, unsigned int* src)
{
    *(unsigned int**)((char*)self + 0x0) = src;
    *(unsigned int*)((char*)self + 0x4) = *src | 0x30000000;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00384DC0);
#ifdef SKIP_ASM
/**
 * Closes the builder's open line strip: pads it to a quadword and fills in its two GIF tags.
 *
 * When the vertex count (+0x10) is odd one 64-bit zero is appended. At the strip's reserved slot (+0xC) it writes a
 * REGLIST tag for PRIM with value 0x4A (Gouraud, alpha-blended line strip) and a REGLIST tag of count loops of RGBAQ,
 * ST, XYZ2 with EOP, then advances the slot past them. func_003850A8 and func_00384E50 call it.
 */
// PORT: 64-bit `ulong` packet words.
extern "C" void func_00384DC0(void* self)
{
    if (*(int*)((char*)self + 0x10) & 1) {
        *(*(ulong**)((char*)self + 0x4))++ = 0;
    }
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x0) = ((ulong)0x14000000 << 32) | 1;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x8) = 0;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x10) = 74;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x18) = 0;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x20) =
        (*(int*)((char*)self + 0x10) | 0x8000) | ((ulong)53248 << 46);
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x28) = 1313;
    *(char**)((char*)self + 0xC) += 0x30;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00384E50);
#ifdef SKIP_ASM
struct sPtRS_4E50 {
    int f0;
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_4E50 {
    sPtRS_4E50 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_4E50 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_4E50 ents[1];            // 0x80
};

struct sPtGfx_4E50 {
    char pad_0x0[0xE84];
    sPtRS_4E50* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_4E50* ring;             // 0x18F0
};

struct sPktBuilder_4E50 {
    unsigned int* where;            // 0x0
    unsigned int cur;               // 0x4 (uncached)
    unsigned int start;             // 0x8 (uncached)
    int open;                       // 0xC
};

extern sPtGfx_4E50* D_004A5B80_4E50 __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void func_00384DC0(void* self);
void operator_delete(int*);

/**
 * Closes a packet builder: finishes its packet and queues it on the defer manager, then frees the builder when bit 0 of
 * @p flags is set.
 *
 * When a packet was started (+0x8): closes the open strip (func_00384DC0), writes a DMA cnt tag and a VIF DIRECT code
 * covering the packet at its start, and, while the defer manager (mpDeferMan) holds fewer than 2600 entries, adds a
 * 0x80-byte entry with a copy of *m_pCurrentDrawState (its low 5 bits of word +0x8 set from the defer manager's scissor
 * index +0x69CC4), the packet's start and end (physical addresses), the manager's key (+0x69CC0) and flag 0. It then
 * moves the cursor one quadword on and writes it back to the cursor it was opened on. cPS2GraphicsMan_RenderLineStrip calls it with 2.
 *
 * @param flags  bit 0: delete the builder afterwards
 */
// PORT: uncached (0x30000000) pointers held in int; 64-bit GS words are `ulong`.
extern "C" void func_00384E50(sPktBuilder_4E50* self, int flags)
{
    if (self->start != 0) {
        if (self->open != 0)
            func_00384DC0(self);
        unsigned long* tagWords = (unsigned long*)self->start;
        int qwc = ((unsigned long*)self->cur - tagWords) / 2;
        tagWords[0] = (qwc - 1) | 0x10000000;
        ((unsigned long*)self->start)[1] = 0;
        ((unsigned long*)self->start)[2] = 0;
        unsigned int n = qwc;
        ((unsigned long*)self->start)[3] = (unsigned long)((n - 2) | 0x50000000) << 32;
        unsigned int buf = self->start & 0x0FFFFFFF;
        unsigned int next = self->cur & 0x0FFFFFFF;
        self->cur = next;
        self->start = buf;
        sPtGfx_4E50* graphicsMan = D_004A5B80_4E50;
        sPtRing_4E50* ring = graphicsMan->ring;
        sPtRS_4E50* rs = graphicsMan->top;
        if (ring->count < 2600) {
            rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
            int key = *(int*)((char*)ring + 0x69CC0);
            sPtEnt_4E50* deferEntry = (sPtEnt_4E50*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
            deferEntry->rs = *rs;
            deferEntry->buf = buf;
            deferEntry->next = next;
            deferEntry->key = key;
            deferEntry->flag = 0;
        }
        self->cur += 0x10;
        *self->where = self->cur;
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00384FD0);
#ifdef SKIP_ASM
struct sPartGsCtx {
    int field_0x0;
    ulong* p;            // 0x4
    int field_0x8;
    int field_0xc;
    int count;           // 0x10
};

struct sPartGsVtx {
    float s;             // 0x0
    float t;             // 0x4
    int field_0x8;
    int field_0xc;
    unsigned int r;      // 0x10
    unsigned int g;      // 0x14
    unsigned int b;      // 0x18
    unsigned int a;      // 0x1c
};

struct sPartGsPos {
    float x, y, z, q;
};

/**
 * Appends one vertex to the open line strip: RGBAQ, ST and XYZ2, and counts it.
 *
 * RGBAQ takes the vertex's r, g, b, a and Q = @p pos's q; ST = (s * q, t * q); XYZ2 = (int x, int y, int (z * q)).
 * cPS2GraphicsMan_RenderLineStrip and func_003850A8 call it.
 *
 * @param ctx  the packet builder
 * @param vtx  the vertex (s, t, colour)
 * @param pos  the projected position (x, y, z and q = 1 / w)
 */
// PORT: 64-bit `ulong` GS packet words; float bits reinterpreted via *(int*)&f.
extern "C" void func_00384FD0(sPartGsCtx* ctx, sPartGsVtx* vtx, sPartGsPos* pos)
{
    ctx->p[0] = (ulong)vtx->r | ((ulong)vtx->g << 8) | ((ulong)vtx->b << 16) | ((ulong)vtx->a << 24)
              | ((ulong)*(int*)&pos->q << 32);
    union { float f; int i; } us, ut;
    us.f = vtx->s * pos->q;
    ut.f = vtx->t * pos->q;
    ctx->p[1] = (ulong)us.i | ((ulong)ut.i << 32);
    ctx->p[2] = (ulong)(int)pos->x | ((ulong)(int)pos->y << 16) | ((ulong)(int)(pos->z * pos->q) << 32);
    ctx->p += 3;
    ctx->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_003850A8);
#ifdef SKIP_ASM
/**
 * Starts a new line strip in the packet builder at the given vertex.
 *
 * When the builder has no packet yet, reserves two quadwords for its DMA and VIF header (+0x8); otherwise closes the
 * open strip (func_00384DC0). Reserves three quadwords for the new strip's tags (+0xC), resets the vertex count and
 * appends the vertex with func_00384FD0. cPS2GraphicsMan_RenderLineStrip calls it.
 *
 * @param ctx  the packet builder
 * @param stripVertex  the first vertex (s, t, colour)
 * @param pos  its projected position
 */
// PORT: packet pointers held in int fields (field_0x8 / field_0xc).
extern "C" void func_003850A8(sPartGsCtx* ctx, sPartGsVtx* stripVertex, sPartGsPos* pos)
{
    if (ctx->field_0x8 == 0) {
        ulong* packetStart = ctx->p;
        ctx->field_0x8 = (int)packetStart;
        ctx->p = packetStart + 4;
    } else if (ctx->field_0xc != 0) {
        func_00384DC0(ctx);
    }
    ulong* stripTags = ctx->p;
    ctx->count = 0;
    ctx->field_0xc = (int)stripTags;
    ctx->p = stripTags + 6;
    func_00384FD0(ctx, stripVertex, pos);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_cPS2GridMesh);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern void* cPS2GridMesh_vtbl[];
extern char str_GridMeshMem[];
extern "C" int func_00385EB0(void* self, int rows, int pitch);
extern "C" void func_00386128(void* self, int rows, int pitch, char* dst, char* src);

struct sPtSurf5138 {
    int field_0x0;
    void** vtable;          // 0x4
    char pad_0x8[0x188];
    short w;                // 0x190
    short h;                // 0x192
    float field_0x194;      // 0x194
    float field_0x198;      // 0x198
    char* buf;              // 0x19C
    int field_0x1A0;        // 0x1A0
    int field_0x1A4;        // 0x1A4
    int pad_0x1A8;
    char* bufA;             // 0x1AC
    char* bufB;             // 0x1B0
    char* rowsA;            // 0x1B4
    char* rowsB;            // 0x1B8
    int rows;               // 0x1BC
    int pitch;              // 0x1C0
    int field_0x1C4;        // 0x1C4
    int valid;              // 0x1C8
};

// PORT: the unit declares cPS2GridMesh_cPS2GridMesh(void*, void*, void*, void*, unsigned int); the body takes ints.
extern "C" sPtSurf5138* cPS2GridMesh_cPS2GridMesh_impl(sPtSurf5138* self, int w, int h, int a3, int allocFlags) __asm__("cPS2GridMesh_cPS2GridMesh");
/**
 * Constructs a grid mesh of @p w x @p gridHeight vertices drawn as h - 1 strips, with two vertex buffers to update and draw in
 * turn.
 *
 * Sets the vtable cPS2GridMesh_vtbl, the size (+0x190, +0x192), the strip count (+0x1BC = h - 1), the vertices per strip
 * (+0x1C0 = 2w), @p graphicsMan (+0x1C4), the range 20000 (+0x194) and the position scale 32767 / 20000 (+0x198). func_00385EB0
 * gives one buffer's packet size; one allocation tagged "GridMeshMem" with @p allocFlags (+0x19C) holds the two buffers
 * (+0x1AC, +0x1B0) and two strip tables of 16 bytes per strip (+0x1B4, +0x1B8). On success it sets the OK flag +0x1C8
 * to 1, lays out each buffer's packets with func_00386128, clears the updated flag (+0x1A4) and sets the buffer index
 * (+0x1A0) to -1; when the allocation fails the OK flag is 0. cPS2GraphicsMan_CreateGridMesh calls it on a 460-byte
 * block tagged "cPS2GridMesh".
 *
 * @param w  vertices per row
 * @param gridHeight  number of rows of vertices
 * @param graphicsMan  the graphics manager that draws it
 * @param allocFlags  memory flags for the buffer allocation
 * @return the mesh
 */
extern "C" sPtSurf5138* cPS2GridMesh_cPS2GridMesh_impl(sPtSurf5138* self, int w, int gridHeight, int graphicsMan, int allocFlags)
{
    int pitch = w * 2;
    int rows = gridHeight - 1;
    self->vtable = cPS2GridMesh_vtbl;
    self->rows = rows;
    self->pitch = pitch;
    self->field_0x1C4 = graphicsMan;
    self->field_0x194 = 20000.0f;
    self->field_0x198 = 1.638350009918213f;
    self->w = w;
    self->h = gridHeight;
    int size = func_00385EB0(self, rows, pitch);
    char* buf = (char*)operator_new_tag(size * 2 + (self->rows << 5), str_GridMeshMem, allocFlags, 0);
    self->buf = buf;
    if (buf == 0) {
        self->valid = 0;
    } else {
        char* bufCursor = buf + size;
        int rowCount = self->rows;
        self->bufB = bufCursor;
        bufCursor += size;
        self->rowsA = bufCursor;
        self->valid = 1;
        bufCursor += rowCount * 16;
        self->rowsB = bufCursor;
        self->bufA = buf;
        func_00386128(self, rowCount, self->pitch, self->rowsA, buf);
        func_00386128(self, self->rows, self->pitch, self->rowsB, self->bufB);
        self->field_0x1A4 = 0;
        self->field_0x1A0 = -1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", cGridMesh__cGridMesh);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00493938[];

/**
 * Destroys a grid mesh's base part: stores the base vtable D_00493938 (+0x4) and frees the object when bit 0 of
 * @p flags is set.
 *
 * Slot 1 of the abstract grid-mesh vtable (whose other slots are pure virtual, apart from cGridMesh_SetScale__FPv);
 * cPS2GridMesh__cPS2GridMesh calls it.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cGridMesh__cGridMesh(int* self, int flags)
{
    *(void**)((char*)self + 0x4) = D_00493938;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cPS2GridMesh__cPS2GridMesh);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern void* cPS2GridMesh_vtbl[];

/**
 * Destroys a grid mesh: stores its vtable cPS2GridMesh_vtbl, frees its buffer (+0x19C, cMemMan_free) when it has one, then
 * runs the base destructor cGridMesh__cGridMesh with @p flags.
 *
 * Slot 1 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cPS2GridMesh__cPS2GridMesh(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = cPS2GridMesh_vtbl;
    if (*(void**)((char*)self + 0x19C) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x19C));
    }
    cGridMesh__cGridMesh((int*)self, flags);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update);
#ifdef SKIP_ASM
struct sPtVtx52E8 {
    float x, y, z;
};

struct sPtRow52E8 {
    int field_0x0;
    short* out;             // 0x4
    int field_0x8;
    int field_0xC;
};

struct sPtSurf52E8 {
    char pad_0x0[0x190];
    unsigned short w;           // 0x190
    unsigned short h;           // 0x192
    char pad_0x194[0xC];
    int field_0x1A0;            // 0x1A0
    int field_0x1A4;            // 0x1A4
    char pad_0x1A8[0xC];
    sPtRow52E8* rowTabs[2];     // 0x1B4
    int rows;                   // 0x1BC
};

struct sPartVec3;
extern "C" void func_00385A38(void* self, short* out, int i, sPartVec3* v);

/**
 * Updates the mesh's vertex positions: writes them into the strips of the buffer not in use and marks the mesh updated.
 *
 * For each strip i (the rows of vertices i and i + 1) of the strip table +0x1B4 / +0x1B8 picked by the buffer index
 * (+0x1A0; the other buffer), stores the positions of row i and row i + 1 alternately with func_00385A38 (scaled to
 * shorts). Sets the updated flag +0x1A4, so the next render swaps buffers. Slot 3 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param verts  the grid positions, row by row (x, y, z floats)
 */
extern "C" void cPS2GridMesh_Update(sPtSurf52E8* self, sPtVtx52E8* verts)
{
    int idx = self->field_0x1A0 == 0;
    for (int i = 0; i < self->rows; i++) {
        int w = self->w;
        sPtRow52E8* row = &self->rowTabs[idx][i];
        int base = i * w;
        int next = base + w;
        int k = 0;
        int j = 0;
        if (j < self->w) {
            sPtVtx52E8* lowerVert = &verts[next];
            sPtVtx52E8* upperVert = &verts[base];
            do {
                func_00385A38(self, row->out, k, (sPartVec3*)upperVert);
                func_00385A38(self, row->out, k + 1, (sPartVec3*)lowerVert);
                k += 2;
                upperVert++;
                lowerVert++;
                j++;
            } while (j < self->w);
        }
    }
    self->field_0x1A4 = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update1);
#ifdef SKIP_ASM
struct sPtUV5410 {
    float u, v;
};

struct sShortUV;

struct sPtRow5410 {
    int field_0x0;
    int field_0x4;
    sShortUV* uv;           // 0x8
    int field_0xC;
};

struct sPtSurf5410 {
    char pad_0x0[0x190];
    unsigned short w;           // 0x190
    unsigned short h;           // 0x192
    char pad_0x194[0xC];
    int field_0x1A0;            // 0x1A0
    int field_0x1A4;            // 0x1A4
    char pad_0x1A8[0xC];
    sPtRow5410* rowTabs[2];     // 0x1B4
    int rows;                   // 0x1BC
};

extern "C" void func_00385BA0(void* self, sShortUV* dst, int idx, float* src);

/**
 * Updates the mesh's texture coordinates: writes them into the strips of the buffer not in use and marks the mesh
 * updated.
 *
 * For each strip i, picked as in cPS2GridMesh_Update, stores the u, v of row i and row i + 1 alternately with func_00385BA0
 * (fixed point, 4096 = 1.0). Sets the updated flag +0x1A4. Slot 4 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param uvs  the grid texture coordinates, row by row (u, v floats)
 */
extern "C" void cPS2GridMesh_Update1(sPtSurf5410* self, sPtUV5410* uvs)
{
    int idx = self->field_0x1A0 == 0;
    for (int i = 0; i < self->rows; i++) {
        int w = self->w;
        sPtRow5410* row = &self->rowTabs[idx][i];
        int base = i * w;
        int next = base + w;
        int k = 0;
        int j = 0;
        if (j < self->w) {
            sPtUV5410* lowerUv = (sPtUV5410*)((next << 3) + (int)uvs);  // PORT: pointer in int
            sPtUV5410* upperUv = (sPtUV5410*)((base << 3) + (int)uvs);  // PORT: pointer in int
            do {
                func_00385BA0(self, row->uv, k, (float*)upperUv);
                func_00385BA0(self, row->uv, k + 1, (float*)lowerUv);
                k += 2;
                upperUv++;
                lowerUv++;
                j++;
            } while (j < self->w);
        }
    }
    self->field_0x1A4 = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update2);
#ifdef SKIP_ASM
struct sPartVec3;
extern "C" void func_00385A38(void* self, short* out, int i, sPartVec3* v);
extern "C" void func_00385AA0(void* self, unsigned short* buf, int idx, const float* c);

struct sPartPos5530 {
    float x, y, z;
};

struct sPartCol5530 {
    float c[4];
};

struct sPartStrip5530 {
    int field_0x0;
    short* pos;                 // 0x4
    int field_0x8;
    unsigned short* col;        // 0xC
};

struct sPartGrid5530 {
    char pad_0x0[0x190];
    unsigned short cols;            // 0x190
    char pad_0x192[0x1A0 - 0x192];
    int sel;                        // 0x1A0
    int dirty;                      // 0x1A4
    char pad_0x1A8[0x1B4 - 0x1A8];
    sPartStrip5530* strips[2];      // 0x1B4
    int rows;                       // 0x1BC
};

/**
 * Updates the mesh's vertex positions and colours: writes them into the strips of the buffer not in use and marks the
 * mesh updated.
 *
 * For each strip i, picked as in cPS2GridMesh_Update, stores the colour (func_00385AA0, 16-bit) and position (func_00385A38)
 * of row i and row i + 1 alternately. Sets the updated flag +0x1A4. Slot 5 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param pos  the grid positions, row by row (x, y, z floats)
 * @param col  the grid colours, row by row (a, r, g, b floats, 0..1)
 */
extern "C" void cPS2GridMesh_Update2(sPartGrid5530* self, sPartPos5530* pos, sPartCol5530* col)
{
    int back = self->sel == 0;
    for (int i = 0; i < self->rows; i++) {
        sPartStrip5530* strip = &self->strips[back][i];
        int n = self->cols;
        int rowStart = i * n;
        int nextRowStart = rowStart + n;
        int k = 0;
        int j = 0;
        if (j < self->cols) {
            sPartPos5530* nextPos = &pos[nextRowStart];
            sPartPos5530* curPos = &pos[rowStart];
            sPartCol5530* nextCol = (sPartCol5530*)((nextRowStart << 4) + (int)col);  // PORT: pointer in int
            sPartCol5530* curCol = (sPartCol5530*)((rowStart << 4) + (int)col);  // PORT: pointer in int
            do {
                func_00385AA0(self, strip->col, k, curCol->c);
                func_00385A38(self, strip->pos, k, (sPartVec3*)curPos);
                func_00385AA0(self, strip->col, k + 1, nextCol->c);
                func_00385A38(self, strip->pos, k + 1, (sPartVec3*)nextPos);
                k += 2;
                curPos++;
                curCol++;
                nextPos++;
                nextCol++;
                j++;
            } while (j < self->cols);
        }
    }
    self->dirty = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update3);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00385BA0_uv(void* self, void* dst, int idx, float* src) __asm__("func_00385BA0");

extern "C" void func_00385A38_vt(void* self, void* dst, int idx, float* src) __asm__("func_00385A38");

extern "C" void func_00385AA0_col(void* self, void* dst, int idx, float* src) __asm__("func_00385AA0");

struct sPartRow_56B8 {
    int cnt;
    void* verts;                    // 0x4
    void* uv;                       // 0x8
    void* colors;                   // 0xC
};

struct sPartGrid_56B8 {
    char pad_0x0[0x190];
    unsigned short width;           // 0x190
    char pad_0x192[0x1A0 - 0x192];
    int sel;                        // 0x1A0
    int done;                       // 0x1A4
    char pad_0x1A8[0xC];
    sPartRow_56B8* rows[2];         // 0x1B4
    int height;                     // 0x1BC
};

/**
 * Updates the mesh's positions, colours and texture coordinates; on the first call fills both buffers.
 *
 * When the buffer index (+0x1A0) is still -1 it becomes 0 and the call counts as the first. For each strip of the
 * buffer not in use it stores, for row i and row i + 1 alternately, the uv (func_00385BA0), colour (func_00385AA0) and
 * position (func_00385A38); on the first call it then fills buffer 0's strips the same way. Sets the updated flag
 * +0x1A4. Slot 6 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param verts  the grid positions, row by row (x, y, z floats)
 * @param colors  the grid colours, row by row (a, r, g, b floats, 0..1)
 * @param uvs  the grid texture coordinates, row by row (u, v floats)
 */
extern "C" void cPS2GridMesh_Update3(sPartGrid_56B8* self, float* verts, float* colors, float* uvs)
{
    int first = 0;
    if (self->sel < 0) {
        self->sel = 0;
        first = 1;
    }
    int z = self->sel == 0;
    int rowIndex;
    for (rowIndex = 0; rowIndex < self->height; rowIndex++) {
        sPartRow_56B8* strip = &self->rows[z][rowIndex];
        int topIdx = rowIndex * self->width;
        int bottomIdx = topIdx + self->width;
        int idx = 0;
        for (int col = 0; col < self->width; col++) {
            func_00385BA0_uv(self, strip->uv, idx, uvs + topIdx * 2);
            func_00385AA0_col(self, strip->colors, idx, colors + topIdx * 4);
            func_00385A38_vt(self, strip->verts, idx, verts + topIdx * 3);
            func_00385BA0_uv(self, strip->uv, idx + 1, uvs + bottomIdx * 2);
            func_00385AA0_col(self, strip->colors, idx + 1, colors + bottomIdx * 4);
            func_00385A38_vt(self, strip->verts, idx + 1, verts + bottomIdx * 3);
            idx += 2;
            topIdx++;
            bottomIdx++;
        }
    }
    // First update: fill the other buffer too. The binary has a second copy of the loop sharing the
    // row counter (one spill slot) and indexing rows[z] with z reset to 0, so it is written out here.
    if (first) {
        z = 0;
        for (rowIndex = 0; rowIndex < self->height; rowIndex++) {
            sPartRow_56B8* strip = &self->rows[z][rowIndex];
            int topIdx = rowIndex * self->width;
            int bottomIdx = topIdx + self->width;
            int idx = 0;
            for (int col = 0; col < self->width; col++) {
                func_00385BA0_uv(self, strip->uv, idx, uvs + topIdx * 2);
                func_00385AA0_col(self, strip->colors, idx, colors + topIdx * 4);
                func_00385A38_vt(self, strip->verts, idx, verts + topIdx * 3);
                func_00385BA0_uv(self, strip->uv, idx + 1, uvs + bottomIdx * 2);
                func_00385AA0_col(self, strip->colors, idx + 1, colors + bottomIdx * 4);
                func_00385A38_vt(self, strip->verts, idx + 1, verts + bottomIdx * 3);
                idx += 2;
                topIdx++;
                bottomIdx++;
            }
        }
    }
    self->done = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385A38);
#ifdef SKIP_ASM
struct sPartVec3 {
    float x, y, z;
};

/**
 * Stores vertex @p i of a strip as three shorts: @p src's x, y and z times the mesh's position scale (+0x198, 32767 /
 * range).
 *
 * The grid-mesh updates (cPS2GridMesh_Update, cPS2GridMesh_Update2, cPS2GridMesh_Update3) call it.
 *
 * @param out  the strip's position array (3 shorts per vertex)
 * @param i  the vertex index within the strip
 * @param src  the position
 */
extern "C" void func_00385A38(void* self, short* out, int i, sPartVec3* src)
{
    float scale = *(float*)((char*)self + 0x198);
    sPartVec3 scaled;
    scaled.x = src->x * scale;
    scaled.y = src->y * scale;
    scaled.z = src->z * scale;
    int j = i * 3;
    out[j] = (short)scaled.x;
    out[j + 1] = (short)scaled.y;
    out[j + 2] = (short)scaled.z;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385AA0);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator.
static inline float partClampf(float x, float lo, float hi)
{
    if (x >= lo) {
        return x <? hi;
    }
    return lo;
}

/**
 * Stores vertex @p idx's colour as a 16-bit value: 5 bits each of red, green and blue and a 1-bit alpha.
 *
 * Each channel of @p colour is clamped to 0..1, times 32, capped at 31; alpha is 1 when its 0..1 value times 32 is at
 * least 1. The grid-mesh colour updates (cPS2GridMesh_Update2, cPS2GridMesh_Update3) call it.
 *
 * @param buf  the strip's colour array
 * @param idx  the vertex index within the strip
 * @param colour  the colour as a, r, g, b floats
 */
extern "C" void func_00385AA0(void* self, unsigned short* buf, int idx, const float* colour)
{
    unsigned char red = (int)(partClampf(colour[1], 0.0f, 1.0f) * 32.0f);
    unsigned char green = (int)(partClampf(colour[2], 0.0f, 1.0f) * 32.0f);
    unsigned char blue = (int)(partClampf(colour[3], 0.0f, 1.0f) * 32.0f);
    unsigned char alpha = (int)(partClampf(colour[0], 0.0f, 1.0f) * 32.0f);
    if (red > 31) red = 31;
    if (green > 31) green = 31;
    if (blue > 31) blue = 31;
    if (alpha > 0) alpha = 1;
    buf[idx] = (alpha << 15) | (blue << 10) | (green << 5) | red;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385BA0);
#ifdef SKIP_ASM
struct sShortUV {
    short u;
    short v;
};

/**
 * Stores vertex @p idx's texture coordinates as two shorts: @p src's u and v times 4096.
 *
 * The grid-mesh uv updates (cPS2GridMesh_Update1, cPS2GridMesh_Update3) call it.
 *
 * @param dst  the strip's uv array
 * @param idx  the vertex index within the strip
 * @param src  the u, v floats
 */
extern "C" void func_00385BA0(void* self, sShortUV* dst, int idx, float* src)
{
    dst[idx].u = (short)(src[0] * 4096.0f);
    dst[idx].v = (short)(src[1] * 4096.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Render);
#ifdef SKIP_ASM
class cPartVirt {
public:
    int field_0x0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int, int, int);
};

/**
 * Draws the mesh with a material's texture: calls virtual slot 8 (cPS2GridMesh_Render1) with @p meshMatrix, the texture id (the first
 * short of the record that @p materialBlock's +0x4 points to) and @p texMatrix.
 *
 * Slot 7 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param meshMatrix  the object matrix
 * @param materialBlock  the material record
 * @param texMatrix  passed on as cPS2GridMesh_Render1's last argument (a texture matrix, or 0)
 */
extern "C" void cPS2GridMesh_Render(cPartVirt* self, int meshMatrix, void* materialBlock, int texMatrix)
{
    self->v08(meshMatrix, **(short**)((char*)materialBlock + 4), texMatrix);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Render1);
#ifdef SKIP_ASM
struct sPtRS_5C10 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_5C10 {
    sPtRS_5C10 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_5C10 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_5C10 ents[1];            // 0x80
};

struct sPtGfx_5C10 {
    char pad_0x0[0xE84];
    sPtRS_5C10* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_5C10* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

struct sMat_5C10 {
    float m[4][4];
    sMat_5C10() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sMat_5C10(const sMat_5C10& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
} __attribute__((aligned(16)));

struct sVec4_5C10 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBez_5C10 {
    char pad_0x0[0x194];
    float scale;                    // 0x194
    char pad_0x198[0x8];
    int buf;                        // 0x1A0
    int flip;                       // 0x1A4
    char pad_0x1A8[0x4];
    unsigned int tags[2];           // 0x1AC
    char pad_0x1B4[0x1C4 - 0x1B4];
    sPtGfx_5C10* gfx;               // 0x1C4
};

// PORT: PS2-only VU0 inline asm (scale the rows of a 4x4 matrix by a vector).
static inline sMat_5C10 ScaleMat_5C10(const sMat_5C10& m, const sVec4_5C10& v)
{
    sMat_5C10 d;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulx.xyzw $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw $vf11, $vf7, $vf1w\n"
        "sqc2      $vf8, 0x0(%0)\n"
        "sqc2      $vf9, 0x10(%0)\n"
        "sqc2      $vf10, 0x20(%0)\n"
        "sqc2      $vf11, 0x30(%0)\n"
        :
        : "r"(&d), "r"(&m), "r"(&v)
        : "memory");
    return d;
}

extern int D_004A4474;
extern "C" unsigned long* func_0038F460(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int func_0038F668(int dma, unsigned long* p, int a2);
extern "C" void func_0037D968(cPS2GraphicsMan* self);
extern "C" void func_0037E120_5C10(sPtGfx_5C10* gfx, sMat_5C10* m, sMat_5C10* hdr, sVec4_5C10* scale, int flag, int a5, int prim, char** pp) __asm__("func_0037E120");

/**
 * Draws the mesh with texture @p tex: queues a call of its current vertex buffer on the defer manager.
 *
 * When the mesh was updated since the last draw (+0x1A4) it clears the flag and swaps the buffer index (+0x1A0).
 * Recomputes the matrices (func_0037D968) when mMatricesValid is 0. Sets the current draw state's (m_pCurrentDrawState)
 * bits 6-9 of word 0 to 3, clears the low 30 bits of word +0xC and sets its texture to @p tex. Opens a packet at
 * mVifCursor (func_0038F460 on D_004A4474) and writes func_0037E120's VU constants: *@p mat scaled by the mesh range
 * (+0x194) in x, y and z, the unscaled matrix, @p flag as the extra matrix, and flags 34 when @p flag is set else 32;
 * then a DMA call to the current buffer (+0x1AC or +0x1B0). Closes the packet (func_0038F668), adds a 0x80-byte defer
 * manager entry as func_00384E50 does (when fewer than 2600) and moves mVifCursor past it. Slot 8 of the grid-mesh
 * vtable cPS2GridMesh_vtbl.
 *
 * @param mat  the object matrix
 * @param tex  the texture id
 * @param flag  a texture matrix (sent to the VU), or 0
 */
// PORT: uncached (0x30000000) pointers held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GridMesh_Render1(sBez_5C10* self, sMat_5C10* mat, int tex, int flag)
{
    if (self->flip) {
        self->flip = 0;
        self->buf = (self->buf + 1) & 1;
    }
    if (self->gfx->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self->gfx);
    }
    self->gfx->top->f0 = (self->gfx->top->f0 & ~0x3C0) | 0xC0;
    self->gfx->top->fC &= 0xC0000000;
    self->gfx->top->tex = tex;
    unsigned int addr = self->gfx->bufAddr;
    sMat_5C10 baseMatrix = *mat;
    float meshRange = self->scale;
    sVec4_5C10 sc;
    sc.x = meshRange;
    sc.y = meshRange;
    sc.z = meshRange;
    sc.w = 1.0f;
    sMat_5C10 scaledMatrix = ScaleMat_5C10(*mat, sc);
    char* packetCursor = (char*)func_0038F460(D_004A4474, addr, -1, 0);
    func_0037E120_5C10(self->gfx, &scaledMatrix, &baseMatrix, &sc, flag, 0, flag ? 34 : 0x20, &packetCursor);
    *(ulong*)packetCursor = ((ulong)self->tags[self->buf] << 32) | 0x50000000;
    *(ulong*)(packetCursor + 8) = 0;
    packetCursor += 0x10;
    addr = func_0038F668(D_004A4474, (unsigned long*)packetCursor, 4);
    sPtRing_5C10* ring = self->gfx->ring;
    sPtRS_5C10* rs = self->gfx->top;
    unsigned int cur = self->gfx->bufAddr;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_5C10* deferEntry = (sPtEnt_5C10*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = cur;
        deferEntry->next = addr;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
    self->gfx->bufAddr = addr + 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385EB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: stand-in locals `hdrNext = 2` / `hdrFirst = 0x1D` set the hoisted constants' allocation order; plain literals 99.89%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sStrip_5EB0 {
    int len;    // 0x0
    int n;      // 0x4
    int m;      // 0x8
};

struct sPkt_5EB0 {
    char pad_0x0[0xC];
    sStrip_5EB0 s[32];      // 0xC
    int count;              // 0x18C
    char pad_0x190[0x1A8 - 0x190];
    int total;              // 0x1A8
};

#define QW_5EB0(b) (((b) & 15 ? 1 : 0) + (b) / 16)

int func_00385EB0_impl(sPkt_5EB0* self, int n, int m) __asm__("func_00385EB0");
/**
 * Splits the mesh's @p n strips of @p stripVerts vertices into VU batches and returns the size in bytes of one vertex buffer's
 * packet.
 *
 * A batch takes strips while its size estimate stays below 219: a header (29 for the first batch, 2 for later ones)
 * plus 1 per strip and 3 per vertex. Each batch's estimate, strip count and vertex count go in the table at +0xC (12
 * bytes per batch) and the batch count at +0x18C. The total (+0x1A8) is 1 + 2 quadwords plus, per batch, 6 + 4
 * quadwords of tags and the quadword-rounded sizes of the strip lengths (2 bytes each), uvs (4), colours (2) and
 * positions (6 per vertex). The grid-mesh constructor cPS2GridMesh_cPS2GridMesh calls it.
 *
 * @param n  number of strips
 * @param stripVerts  vertices per strip
 * @return the packet size in bytes (total quadwords * 16)
 */
int func_00385EB0_impl(sPkt_5EB0* self, int n, int stripVerts)
{
    int add = stripVerts * 3 + 1;
    int hdrNext = 2;
    int hdrFirst = 29;
    self->count = 0;
    self->s[0].n = 0;
    self->s[0].m = 0;
    do {
        if (self->count == 0) {
            self->s[self->count].len = hdrFirst;
        } else {
            self->s[self->count].len = hdrNext;
        }
        self->s[self->count].len += self->s[self->count].n;
        self->s[self->count].len += self->s[self->count].m * 3;
        int len = self->s[self->count].len + add;
        if (len < 219) {
            self->s[self->count].len = len;
            n--;
            self->s[self->count].n++;
            self->s[self->count].m += stripVerts;
            if (n == 0) {
                self->count++;
            }
        } else {
            self->count++;
            self->s[self->count].n = 0;
            self->s[self->count].m = 0;
        }
    } while (n > 0);
    self->total = 1;
    for (int i = 0; i < self->count; i++) {
        int batchQwords = 6;
        batchQwords += QW_5EB0(self->s[i].n * 2);
        batchQwords += 1;
        batchQwords += QW_5EB0(self->s[i].m * 4);
        batchQwords += 1;
        batchQwords += QW_5EB0(self->s[i].m * 2);
        batchQwords += 1;
        batchQwords += QW_5EB0(self->s[i].m * 6);
        batchQwords += 1;
        self->total += batchQwords;
    }
    self->total += 2;
    return self->total << 4;
}
#endif

INCLUDE_ASM("render/particle", func_00386128);

//100%
INCLUDE_ASM("render/particle", func_003865D8);
#ifdef SKIP_ASM
/**
 * Pushes @p node onto the front of the singly linked list at *@p head (link at +0x8).
 *
 * Nothing in the retail code references it.
 *
 * @param node  the object to add
 * @param head  the list head
 */
extern "C" void func_003865D8(void *node, void **head) {
    (*(void **)((char*)(node) + (8))) = (void *) *head;
    *head = node;
}
#endif
//100%
INCLUDE_ASM("render/particle", func_003865E8);
#ifdef SKIP_ASM
struct sVE3865E8 { short delta; short index; void (*fn)(void*, int); };

/**
 * Deletes every object of the singly linked list starting at @p head (link at +0x8), calling each one's virtual
 * destructor (vtable at +0x4, slot 1) with flags 3.
 *
 * The next link is read before each delete. Nothing in the retail code references it.
 *
 * @param head  the first object, or 0
 */
extern "C" void func_003865E8(void* head)
{
    void* node = head;
    if (node != 0) {
        do {
            void* next = *(void**)((char*)node + 8);
            if (node != 0) {
                sVE3865E8* vt = *(sVE3865E8**)((char*)node + 4);
                vt[1].fn((char*)node + vt[1].delta, 3);
            }
            node = next;
        } while (node != 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_UpdateFullScreenEffects);
#ifdef SKIP_ASM
extern "C" void func_0036ABA0(void* self, int a1);
extern "C" void func_00390458(void* self, int a1);
extern "C" void func_0036C740(void* self, int a1);

/**
 * Updates the full-screen effects of a view: func_0036ABA0, func_00390458 and func_0036C740, each with @p view.
 *
 * @param view  the view index (On Tour View)
 */
extern "C" void cPS2GraphicsMan_UpdateFullScreenEffects(cPS2GraphicsMan* self, int view)
{
    func_0036ABA0(self, view);
    func_00390458(self, view);
    func_0036C740(self, view);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderFullScreenEffects);
#ifdef SKIP_ASM
struct sPartPair {
    int a, b;
};

/**
 * Records view @p i for the full-screen effects rather than drawing anything: copies mScreenMatrix (+0x6AF0) to
 * unk_5930[i] and the scissor to unk_59B0[i], and sets unk_59C0 = i + 1.
 *
 * @param i  the view index (On Tour View)
 */
// PORT: PS2-only VU0 asm (lqc2/sqc2 matrix copy).
extern "C" void cPS2GraphicsMan_RenderFullScreenEffects(cPS2GraphicsMan* self, int i)
{
    // PORT: (int)self pointer arithmetic (index added first, as in the target).
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"((char*)&self->mScreenMatrix), "r"((char*)((i << 6) + (int)self) + 0x5930)
        : "memory");
    ((sPartPair*)((char*)self + 0x59B0))[i] = *(sPartPair*)&self->mScissor[0];
    self->unk_59C0 = i + 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetLensFXCallBack__FPvii);
#ifdef SKIP_ASM
/**
 * Sets the per-frame callback that the DMA kick thread (func_00382760) calls with @p callbackArg before sending each
 * frame's second DMA phase: mPhase2Callback = @p callback, mPhase2CallbackArg = @p callbackArg.
 *
 * cPS2GraphicsMan_vtbl slot 117; cLensFxMan_cLensFxMan installs cLensFxMan__drawFrameCallBack__FPv with it.
 *
 * @param callback  the function to call, or 0
 * @param callbackArg  the argument passed to it
 */
void cPS2GraphicsMan_SetLensFXCallBack(void* self, int callback, int callbackArg)
{
    *(int*)((char*)self + 0x6d0c) = callback;
    *(int*)((char*)self + 0x6d10) = callbackArg;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_ResolveCharModel);
#ifdef SKIP_ASM
extern "C" int cGraphicsMan_GetTextureID(void* self, const char* name);
extern "C" int func_00416B18(const char* a, const char* b, int n);
extern "C" void cGraphicsMan_AddBlendedMatrix(void* gfx, void* m);
extern "C" int areBlendedMatricesEqual(void* m, void* slot, int a, int b, int c);
extern char str_alph[];

struct sKey_66F0 {
    int v;
    int d;
};

struct sTrack_66F0 {
    sKey_66F0* keys;
    int n;
};

struct sBone_66F0 {
    int flags;
    int name;
    short env;
    short alpha;
    int ntracks;
    sTrack_66F0* tracks;
};

struct sBlend_66F0 {
    int n;
    unsigned char* w;
    int idx;
};

struct sRef_66F0 {
    int n;
    int* idx;
};

struct sSlot_66F0 {
    int n;
    unsigned char* w;
    int idx;
};

struct sMdl_66F0 {
    char pad0[0x18];
    int o18;
    char pad1c[4];
    int o20;
    char pad24[0xC];
    int o30;
    int o34;
    sBlend_66F0* blends;
    sRef_66F0* refs;
    int x40;
    short nblends;
    short nrefs;
    short nbones;
    short nmats;
};

/**
 * Fixes up a loaded character model: turns its offsets into pointers, resolves bone textures and shares the blended
 * matrices.
 *
 * Relocates the model's blend table (+0x34), reference table (+0x30), material block (+0x18) and bones (+0x20) against
 * @p base, with every bone's animation tracks and keys, and stores @p x at +0x40. Each bone reads its 32-byte name
 * record: an alpha, flag 0x8000 for names starting with str_alph (4 chars) or "ea", an environment-map number from an
 * "envN" second name (+0x10), and the name itself is replaced by cGraphicsMan_GetTextureID's result. Material matrix
 * indices and blend indices are rebased by @p tbl; each blended matrix is registered (cGraphicsMan_AddBlendedMatrix)
 * and matched against the caller's list (areBlendedMatricesEqual) or appended to it, and the reference table is remapped to the
 * shared indices. cPS2GraphicsMan vtable slot 109.
 *
 * @param gfx    the graphics manager (this)
 * @param inModel      the model header (tMdfModelHeader*)
 * @param base   the model data the offsets are relative to
 * @param tbl    per-part bone offsets (On Tour partBoneOffset)
 * @param count  in/out: number of entries in @p slots (On Tour numBlendedMats)
 * @param slots  the shared blended-matrix list (On Tour blendedMats)
 * @param x      stored in the model at +0x40 (On Tour: float* morphWeights)
 */
extern "C" void cPS2GraphicsMan_ResolveCharModel(cPS2GraphicsMan* gfx, sMdl_66F0* inModel, char* base, int* tbl, int* count, sSlot_66F0* slots, int x)
{
    inModel->blends = (sBlend_66F0*)(base + inModel->o34);
    inModel->refs = (sRef_66F0*)(base + inModel->o30);
    char* mats = base + inModel->o18;
    sBone_66F0* bones = (sBone_66F0*)(base + inModel->o20);
    inModel->x40 = x;
    for (int i = 0; i < inModel->nrefs; i++)
        inModel->refs[i].idx = (int*)(base + (int)inModel->refs[i].idx);
    for (int i = 0; i < inModel->nbones; i++) {
        bones[i].tracks = (sTrack_66F0*)(base + (int)bones[i].tracks);
        for (int j = 0; j < bones[i].ntracks; j++) {
            bones[i].tracks[j].keys = (sKey_66F0*)(base + (int)bones[i].tracks[j].keys);
            for (int k = 0; k < bones[i].tracks[j].n; k++) {
                bones[i].tracks[j].keys[k].v = (int)base + bones[i].tracks[j].keys[k].v;
                bones[i].tracks[j].keys[k].d += bones[i].tracks[j].keys[k].v;
            }
        }
        char* nameRecord = base + bones[i].name * 32;
        char* secondName = nameRecord + 0x10;
        if (nameRecord[0x10])
            bones[i].alpha = (int)(*(float*)(nameRecord + 0x14) * 128.0f);
        else
            bones[i].alpha = 0;
        if (func_00416B18(nameRecord, str_alph, 4) == 0 || (nameRecord[0] == 'e' && nameRecord[1] == 'a'))
            bones[i].flags |= 0x8000;
        int env = 0;
        if (secondName[0] != 0) {
            if (secondName[0] == 'e' && secondName[1] == 'n' && secondName[2] == 'v') {
                unsigned char envDigit = secondName[3];
                if ((unsigned)(envDigit - '0') < 10)
                    env = (char)envDigit - '0';
            }
        }
        bones[i].name = cGraphicsMan_GetTextureID(gfx, nameRecord);
        bones[i].env = env;
    }
    for (int i = 0; i < inModel->nmats; i++) {
        short* mt = (short*)(mats + i * 0x50);
        if (mt[9] >= 0)
            mt[9] = tbl[mt[8]] + mt[9];
    }
    for (int i = 0; i < inModel->nblends; i++) {
        sBlend_66F0* blendEntry = &inModel->blends[i];
        blendEntry->w = (unsigned char*)(base + (int)blendEntry->w);
        for (int j = 0; j < blendEntry->n; j++)
            blendEntry->w[j * 4 + 2] += tbl[blendEntry->w[j * 4 + 3]];
        cGraphicsMan_AddBlendedMatrix(gfx, blendEntry);
        for (int k = 0;; k++) {
            if (k == *count) {
                slots[*count].n = blendEntry->n;
                slots[*count].w = blendEntry->w;
                slots[*count].idx = *count;
                blendEntry->idx = *count;
                (*count)++;
                goto next;
            }
            if (areBlendedMatricesEqual(blendEntry, &slots[k], 0, 0, 0)) {
                blendEntry->idx = k;
                goto next;
            }
        }
    next:;
    }
    for (int i = 0; i < inModel->nrefs; i++) {
        sRef_66F0* refEntry = &inModel->refs[i];
        for (int j = 0; j < refEntry->n; j++)
            refEntry->idx[j] = inModel->blends[refEntry->idx[j]].idx;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GenerateBlendedMatrices);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBlendMtx_6BD0 {
    float m[16];
} __attribute__((aligned(16)));

struct sBlendLink_6BD0 {
    short w;
    unsigned char idx;
    unsigned char pad;
};

struct sBlendEnt_6BD0 {
    int count;
    sBlendLink_6BD0* links;
    int pad;
};

// PORT: PS2-only VU0 inline asm (set the I register).
static inline void setI_6BD0(int bits)
{
    __asm__ volatile("ctc2.ni    %0, $vi21\n" : : "r"(bits) : "$21");
}

// PORT: PS2-only VU0 inline asm (first weighted matrix, kept in vf1-vf4).
static inline void blendFirst_6BD0(const sBlendMtx_6BD0* m, int w)
{
    __asm__ volatile(
        "qmtc2.ni   %4, $vf5\n"
        "vitof0.x   $vf5, $vf5\n"
        "lqc2       $vf1, %0\n"
        "lqc2       $vf2, %1\n"
        "vmuli.x    $vf5, $vf5, I\n"
        "lqc2       $vf3, %2\n"
        "lqc2       $vf4, %3\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        : : "m"(m->m[0]), "m"(m->m[4]), "m"(m->m[8]), "m"(m->m[12]), "r"(w));
}

// PORT: PS2-only VU0 inline asm (accumulate next weighted matrix into vf1-vf4).
static inline void blendAdd_6BD0(const sBlendMtx_6BD0* m, int w)
{
    __asm__ volatile(
        "qmtc2.ni   %4, $vf5\n"
        "vitof0.x   $vf5, $vf5\n"
        "lqc2       $vf6, %0\n"
        "lqc2       $vf7, %1\n"
        "vmuli.x    $vf5, $vf5, I\n"
        "lqc2       $vf8, %2\n"
        "lqc2       $vf9, %3\n"
        "vmulaw.xyzw ACC, $vf1, $vf0w\n"
        "vmaddx.xyzw $vf1, $vf6, $vf5x\n"
        "vmulaw.xyzw ACC, $vf2, $vf0w\n"
        "vmaddx.xyzw $vf2, $vf7, $vf5x\n"
        "vmulaw.xyzw ACC, $vf3, $vf0w\n"
        "vmaddx.xyzw $vf3, $vf8, $vf5x\n"
        "vmulaw.xyzw ACC, $vf4, $vf0w\n"
        "vmaddx.xyzw $vf4, $vf9, $vf5x\n"
        : : "m"(m->m[0]), "m"(m->m[4]), "m"(m->m[8]), "m"(m->m[12]), "r"(w));
}

// PORT: PS2-only VU0 inline asm (store vf1-vf4).
static inline void blendStore_6BD0(sBlendMtx_6BD0* d)
{
    __asm__ volatile(
        "sqc2       $vf1, %0\n"
        "sqc2       $vf2, %1\n"
        "sqc2       $vf3, %2\n"
        "sqc2       $vf4, %3\n"
        : "=m"(d->m[0]), "=m"(d->m[4]), "=m"(d->m[8]), "=m"(d->m[12]));
}

/**
 * Builds skinning matrices as weighted sums of bone matrices.
 *
 * For each of @p n entries, sums its links' matrices from @p mats (index and weight per link) into one matrix with
 * VU0; the VU0 I register is set to 0.01 first. Output goes to @p out, or to mpScratchpad when @p out is null.
 *
 * @param mats  the bone matrices (On Tour matlist)
 * @param partBoneOffsets  not read (On Tour partBoneOffsets)
 * @param ents  the blend table: per output matrix a link count and (index, weight) links
 * @param n  the number of matrices to build (On Tour numMatrices)
 * @param out  the output array, or null for the scratchpad (On Tour blendedmatlist)
 */
extern "C" void cPS2GraphicsMan_GenerateBlendedMatrices(cPS2GraphicsMan* self, sBlendMtx_6BD0* mats, int partBoneOffsets, sBlendEnt_6BD0* ents, int n, sBlendMtx_6BD0* out)
{
    if (out == 0) {
        out = (sBlendMtx_6BD0*)self->mpScratchpad;
    }
    setI_6BD0(0x3C23D70A);
    int i;
    for (i = 0; i < n; i++) {
        sBlendMtx_6BD0* dst = out++;
        sBlendEnt_6BD0* entry = &ents[i];
        sBlendLink_6BD0* link = entry->links;
        blendFirst_6BD0((sBlendMtx_6BD0*)((char*)mats + (link->idx << 6)), link->w);
        int cnt = entry->count;
        link++;
        int k;
        for (k = 1; k < cnt; k++) {
            blendAdd_6BD0((sBlendMtx_6BD0*)((char*)mats + (link->idx << 6)), link->w);
            link++;
        }
        blendStore_6BD0(dst);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_update);
#ifdef SKIP_ASM
void cGraphicsMan_update(void* self);

/**
 * Per-frame graphics update: calls cGraphicsMan_update.
 */
extern "C" void cPS2GraphicsMan_update(cPS2GraphicsMan* self)
{
    cGraphicsMan_update(self);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00386D10);
#ifdef SKIP_ASM
struct sPartStrip_6D10 {
    void* verts;    // 0x0
    void* cols;     // 0x4
    int count;      // 0x8
    int field_0xc;
    int field_0x10;
};

extern "C" void func_00387EC0(void* self, void* verts, void* cols, int count, void* a4, void* a5, void* a6, float f0, float f1);

/**
 * Draws the ribbon between rings @p firstRing and @p secondRing of each of @p n board trails with func_00387EC0, with the same alpha
 * values for every trail.
 *
 * Each 0x14-byte trail entry gives the table of vertex rings (+0x0), the start index (+0x4) and the vertex count (+0x8;
 * the C names the first two verts and cols); trails with fewer than 2 vertices are skipped. cPS2GraphicsMan_RenderMultipleBoardTrails calls it for
 * its untextured passes.
 *
 * @param strips  the trail entries
 * @param n  number of trails
 * @param firstRing  index of the ring for the ribbon's first edge
 * @param secondRing  index of the ring for its second edge
 * @param textured  nonzero for a textured strip
 * @param alphaScale  alpha multiplier passed for every trail
 * @param alphaStep  alpha step per vertex passed for every trail
 */
extern "C" void func_00386D10(void* self, sPartStrip_6D10* strips, int n, void* firstRing, void* secondRing, void* textured, float alphaScale, float alphaStep)
{
    for (; n > 0; n--, strips++) {
        if (strips->count >= 2) {
            func_00387EC0(self, strips->verts, strips->cols, strips->count, firstRing, secondRing, textured, alphaScale, alphaStep);
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00386DD0);
#ifdef SKIP_ASM
struct sPartStrip_6DD0 {
    void* verts;    // 0x0
    void* cols;     // 0x4
    int count;      // 0x8
    float f0;       // 0xC
    float f1;       // 0x10
};

extern "C" void func_00387EC0(void* self, void* verts, void* cols, int count, void* a4, void* a5, void* a6, float f0, float f1);

/**
 * Draws the ribbon between rings @p firstRing and @p secondRing of each of @p n board trails with func_00387EC0, using each trail's
 * own alpha multiplier and step (+0xC, +0x10).
 *
 * Trails with fewer than 2 vertices are skipped. cPS2GraphicsMan_RenderMultipleBoardTrails calls it for its textured passes.
 *
 * @param strips  the trail entries (0x14 bytes: ring table, start index, vertex count, two floats)
 * @param n  number of trails
 * @param firstRing  index of the ring for the ribbon's first edge
 * @param secondRing  index of the ring for its second edge
 * @param textured  nonzero for a textured strip
 */
extern "C" void func_00386DD0(void* self, sPartStrip_6DD0* strips, int n, void* firstRing, void* secondRing, void* textured)
{
    for (; n > 0; n--, strips++) {
        if (strips->count >= 2) {
            func_00387EC0(self, strips->verts, strips->cols, strips->count, firstRing, secondRing, textured, strips->f0, strips->f1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderMultipleBoardTrails);
#ifdef SKIP_ASM
// PS2GraphicsMan::DrawShadowVolume (REF structs/render.md slot 89): multi-pass destination-alpha
// shadow. Each pass builds a DMA CNT packet in the uncached scratch buffer: optional VIF FLUSH,
// a DIRECT GIF A+D block setting FRAME/ZBUF/TEST/FBA, an optional UNPACK of the guard band,
// then draws the strips through func_00386D10 / func_00386DD0.
// PORT: uncached (0x30000000) packet pointers held in int; ulong is 64-bit (GIF/VIF words).

struct sRS_6E78 {
    int f0;
    int f4;
    int f8;
    int fC;
    short tex;
    short pad;
};

struct sVEnt_6E78 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sQ_6E78 {
    int w[4];
} __attribute__((aligned(16)));

struct sV_6E78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEnt_6E78 {
    sRS_6E78 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sRing_6E78 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_6E78 ents[1];              // 0x80
};

struct sGfx_6E78 {
    char pad_0x0[0xE84];
    sRS_6E78* top;                  // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sVEnt_6E78* vtable;             // 0x10D8
    char pad_0x10DC[0x18F0 - 0x10DC];
    sRing_6E78* ring;               // 0x18F0
    char pad_0x18F4[0x5840 - 0x18F4];
    sV_6E78 guard0;                 // 0x5840
    sV_6E78 guard1;                 // 0x5850
    char pad_0x5860[0x5A00 - 0x5860];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A80 - 0x5A04];
    unsigned int zbp;               // 0x5A80
    unsigned int fbp;               // 0x5A84
    char pad_0x5A88[0x6B90 - 0x5A88];
    int texReady;                   // 0x6B90
};

struct sPkt_6E78 {
    ulong* cur;                     // 0x00 next free quad
    ulong* dma;                     // 0x04 open DMA tag
    ulong* vif;                     // 0x08 open VIF code quad
    ulong* gif;                     // 0x0C open GIF tag
    int pad_0x10[2];
    int cl;                         // 0x18
    int wl;                         // 0x1C
    int fmt;                        // 0x20
    int addr;                       // 0x24
    int flg;                        // 0x28
    unsigned int state;             // 0x2C
};

struct sShadowCfg_6E78 {
    int texIndex;
};
extern sShadowCfg_6E78 D_004A45D4;
struct sShadowOpt_6E78 {
    int fba;
};
extern sShadowOpt_6E78 D_004A45CC;
extern int D_004A5B84;
extern sRS_6E78 D_00501420_rs6E78 __asm__("sDrawState_s_defaultDrawState");

static inline void DmaBegin_6E78(sPkt_6E78* pkt, unsigned int addr)
{
    ulong* p = (ulong*)(addr | 0x30000000);
    pkt->state = 0;
    pkt->dma = p;
    pkt->cur = p + 2;
    pkt->cl = 1;
    pkt->wl = 1;
    pkt->fmt = 0xC;
    pkt->addr = 0;
}

static inline void Flush_6E78(sPkt_6E78* pkt)
{
    pkt->cur[0] = 0;
    pkt->cur[1] = (ulong)0x11000000 << 32;
    pkt->cur += 2;
}

static inline void GifBegin_6E78(sPkt_6E78* pkt)
{
    pkt->state = 6;
    ulong* p = pkt->cur;
    pkt->vif = p;
    pkt->gif = p + 2;
    pkt->cur = p + 4;
}

static inline void AddReg_6E78(sPkt_6E78* pkt, ulong data, ulong reg)
{
    pkt->cur[0] = data;
    pkt->cur[1] = reg;
    pkt->cur += 2;
}

static inline void GifEnd_6E78(sPkt_6E78* pkt)
{
    pkt->state &= ~4;
    pkt->gif[0] = (ulong)(((int)pkt->cur - (int)pkt->gif - 16) / 16 | 0x8000) | ((ulong)1 << 60);
    pkt->gif[1] = 0xE;
}

static inline void DirectEnd_6E78(sPkt_6E78* pkt)
{
    int size = (int)pkt->cur - (int)pkt->vif;
    pkt->vif[0] = 0;
    pkt->vif[1] = (ulong)(unsigned int)(((size - 16) / 16 & 0xFF) | 0x50000000) << 32;
    pkt->state = 0;
}

static inline void UnpackBegin_6E78(sPkt_6E78* pkt)
{
    pkt->state = 1;
    ulong* p = pkt->cur;
    pkt->addr = 4;
    pkt->vif = p;
    pkt->cur = p + 2;
    pkt->flg = 0;
}

static inline void AddQuad_6E78(sPkt_6E78* pkt, const sV_6E78& q)
{
    *(sV_6E78*)pkt->cur = q;
    pkt->cur += 2;
}

static inline void UnpackEnd_6E78(sPkt_6E78* pkt)
{
    pkt->state = 0;
    int size = (int)pkt->cur - (int)pkt->vif;
    pkt->vif[0] = (ulong)0x01000101 << 32;
    int num = (size - 16) / 16 & 0xFF;
    ulong* dst = pkt->vif + 1;
    *dst = (ulong)(pkt->addr | (pkt->flg ? 0x8000 : 0) | num << 16 | 0x6C000000) << 32;
}

static inline void DmaEnd_6E78(sPkt_6E78* pkt, sGfx_6E78* self)
{
    pkt->dma[0] = ((pkt->cur - pkt->dma) / 2 - 1) | 0x10000000;
    pkt->dma[1] = 0;
    ulong* p = pkt->cur;
    pkt->cur = 0;
    self->bufAddr = (unsigned int)p & 0x0FFFFFFF;
}

/**
 * Draws @p n board trails as a multi-pass destination-alpha effect and queues the passes on the defer manager.
 *
 * Resets the current draw state (m_pCurrentDrawState) to sDrawState_s_defaultDrawState, sets its layer and blend bits
 * and its texture to _EffectTexMap[D_004A45D4], recomputes the matrices (func_0037D968) when mMatricesValid is 0 and
 * reads the screen width (virtual slot 10). Each pass is a DMA packet at mVifCursor built through D_004A5B84 (VIF
 * FLUSH, A+D GS registers, optionally mGuardScale / mGuardOffset unpacked to the VU), followed by trail draws: 1) FRAME
 * writing alpha only, z writes masked, z test always, guard band, ribbon 0-1 untextured (func_00386D10); 2) z test
 * GEQUAL with FBA_1 on, ribbon 0-1 again; 3) z writes on, destination alpha test, FBA_1 off, guard band with z = 0,
 * ribbon 0-1 again; 4) full FRAME, z GEQUAL, FBA_1 when D_004A45CC is set, guard band, then the textured ribbons 2-0,
 * 0-3, 3-4, 4-1 and 1-5 with each trail's alpha (func_00386DD0). When D_004A45CC is set a last pass (destination alpha
 * test with z GEQUAL, FBA_1 off) draws ribbon 0-1 textured and then restores the z GEQUAL test. Adds one defer manager
 * entry for all of it (when fewer than 2600), moves mVifCursor on and restores the default draw state.
 * cPS2GraphicsMan_vtbl slot 89.
 *
 * @param strips  the trails (0x14 bytes each: ring table, start index, vertex count, alpha, alpha step)
 * @param n  number of trails
 */
extern "C" void cPS2GraphicsMan_RenderMultipleBoardTrails(sGfx_6E78* self, sPartStrip_6D10* strips, int n)
{
    *self->top = D_00501420_rs6E78;
    self->top->f8 = (self->top->f8 & ~0x3E0) | 0x60;
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0xC0;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    self->top->f4 = (self->top->f4 & ~0x400000) | 0x400000;
    self->top->f4 = (self->top->f4 & ~0x1800000) | 0x1000000;
    self->top->f4 &= ~0x300000;
    self->top->f4 &= ~0xFF000;
    self->top->f4 = (self->top->f4 & ~0x7C) | 0x14;
    int tex = self->texIds[D_004A45D4.texIndex];
    self->top->tex = tex;

    sVEnt_6E78* vt = self->vtable;
    int width = vt[10].fn((char*)self + vt[10].delta);
    sPkt_6E78* pkt = (sPkt_6E78*)D_004A5B84;
    unsigned int fbp = self->fbp;
    unsigned int zbp = self->zbp;
    unsigned int start = self->bufAddr;
    MEM_fill(pkt, 0, 0x30);

    // pass 1: alpha-only FRAME, ZBUF masked, TEST ztst always; guard band
    {
        ulong* packetBase = (ulong*)(self->bufAddr | 0x30000000);
        pkt->dma = packetBase;
        pkt->cur = packetBase + 2;
        pkt->cl = 1;
        pkt->wl = 1;
        pkt->fmt = 0xC;
        pkt->addr = 0;
    }
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, (ulong)fbp | ((ulong)(width / 64) << 16) | ((ulong)0xFFFFFF << 32), 76);
    AddReg_6E78(pkt, (ulong)(zbp | 0x1000000) | ((ulong)1 << 32), 78);
    AddReg_6E78(pkt, 0x30000, 71);
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    UnpackBegin_6E78(pkt);
    AddQuad_6E78(pkt, self->guard0);
    AddQuad_6E78(pkt, self->guard1);
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, self);
    func_00386D10(self, strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 2: TEST ztst gequal, FBA on
    DmaBegin_6E78(pkt, self->bufAddr);
    Flush_6E78(pkt);
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, 0x50000, 71);
    AddReg_6E78(pkt, 1, 74);
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    DmaEnd_6E78(pkt, self);
    func_00386D10(self, strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 3: ZBUF unmasked, destination-alpha test, FBA off; guard band without depth
    DmaBegin_6E78(pkt, self->bufAddr);
    Flush_6E78(pkt);
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, (ulong)(zbp | 0x1000000), 78);
    AddReg_6E78(pkt, 0x3C000, 71);
    AddReg_6E78(pkt, 0, 74);
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    UnpackBegin_6E78(pkt);
    {
        sV_6E78 flatGuardScale;
        sV_6E78 flatGuardOffset;
        flatGuardScale.x = self->guard0.x;
        flatGuardScale.y = self->guard0.y;
        flatGuardScale.z = 0.0f;
        flatGuardScale.w = self->guard0.w;
        flatGuardOffset.x = self->guard1.x;
        flatGuardOffset.y = self->guard1.y;
        flatGuardOffset.z = 0.0f;
        flatGuardOffset.w = self->guard1.w;
        AddQuad_6E78(pkt, flatGuardScale);
        AddQuad_6E78(pkt, flatGuardOffset);
    }
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, self);
    func_00386D10(self, strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 4: full FRAME, TEST ztst gequal, FBA if enabled; then the edges
    DmaBegin_6E78(pkt, self->bufAddr);
    Flush_6E78(pkt);
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, (ulong)fbp | ((ulong)(width / 64) << 16), 76);
    AddReg_6E78(pkt, 0x50000, 71);
    if (D_004A45CC.fba) {
        AddReg_6E78(pkt, 1, 74);
    }
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    UnpackBegin_6E78(pkt);
    AddQuad_6E78(pkt, self->guard0);
    AddQuad_6E78(pkt, self->guard1);
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, self);
    func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, (void*)2, 0, (void*)1);
    func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, 0, (void*)3, (void*)1);
    func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, (void*)3, (void*)4, (void*)1);
    func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, (void*)4, (void*)1, (void*)1);
    func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, (void*)1, (void*)5, (void*)1);

    if (D_004A45CC.fba) {
        DmaBegin_6E78(pkt, self->bufAddr);
        Flush_6E78(pkt);
        GifBegin_6E78(pkt);
        AddReg_6E78(pkt, 0x54000, 71);
        AddReg_6E78(pkt, 0, 74);
        GifEnd_6E78(pkt);
        DirectEnd_6E78(pkt);
        DmaEnd_6E78(pkt, self);
        func_00386DD0((void*)self, (sPartStrip_6DD0*)strips, n, 0, (void*)1, (void*)1);

        DmaBegin_6E78(pkt, self->bufAddr);
        Flush_6E78(pkt);
        GifBegin_6E78(pkt);
        AddReg_6E78(pkt, 0x50000, 71);
        GifEnd_6E78(pkt);
        DirectEnd_6E78(pkt);
        DmaEnd_6E78(pkt, self);
    }

    sRing_6E78* ring = self->ring;
    unsigned int next = self->bufAddr;
    sRS_6E78* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_6E78* deferEntry = (sEnt_6E78*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = start;
        deferEntry->next = next;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
    *self->top = D_00501420_rs6E78;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00387EC0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPtGfx_7EC0 {
    char pad_0x0[0x5A00];
    unsigned int bufAddr;           // 0x5A00
};

struct sPair_7EC0 {
    int a;
    int b;
};

struct sVtx_7EC0 {
    sGifQuad q[3];                // 0x30 bytes
};

extern int D_004A43A0;
extern sPair_7EC0 D_004A43A8[];
extern char D_3A00[];
extern char D_3A08[];

// PORT: uncached (0x30000000) GIF/DMA packet pointers; 64-bit GS words are `ulong`.
void func_00387EC0_impl(sPtGfx_7EC0* self, sVtx_7EC0** tbl, int start, int n, int ia, int ib, int flag, float fa, float fb) __asm__("func_00387EC0");
/**
 * Sends one board-trail ribbon to VU1: the triangle strip between vertex rings @p ia and @p ib of @p tbl, @p n vertices
 * from index @p start, in batches of up to 36 vertices.
 *
 * Each ring holds 54 vertices of 3 quadwords and the index wraps at 54. Per batch it writes at mVifCursor: a header
 * unpacked to the VU (a GIF tag for 2 * count vertices with PRIM 0x5C and registers ST, RGBAQ, XYZ2 when @p flag is
 * set, else PRIM 4 and NOP, RGBAQ, XYZ2; the counts; the current alpha @p fa and step @p fb); for ring @p ia then ring
 * @p ib, DMA ref UNPACKs of the vertices from @p start up to the ring end and of any that wrap round to index 0, and on
 * the last batch the last two vertices copied inline; then a VIF MSCAL of the VU1 program at D_3A00 when D_004A43A0 was
 * set (it is cleared) or D_3A08 otherwise. Batches overlap by one vertex and @p fa grows by @p fb per vertex. The
 * cursor is stored back to mVifCursor. func_00386D10 and func_00386DD0 call it.
 *
 * @param tbl  the trail's table of vertex rings
 * @param start  index of the first vertex in the rings
 * @param n  number of vertices
 * @param ia  index of the ring for the strip's first edge
 * @param ib  index of the ring for its second edge
 * @param flag  nonzero for a textured, Gouraud, blended strip; zero for a flat untextured one
 * @param fa  alpha multiplier for the first vertex
 * @param fb  alpha step per vertex
 */
void func_00387EC0_impl(sPtGfx_7EC0* self, sVtx_7EC0** tbl, int start, int n, int ia, int ib, int flag, float fa, float fb)
{
    unsigned int addr = (D_004A43A0 ? (unsigned int)D_3A00 : (unsigned int)D_3A08) >> 3;
    int primBits;
    long regList;
    D_004A43A0 = 0;
    if (flag) {
        primBits = 92;
        regList = 1042;
    } else {
        primBits = 4;
        regList = 1055;
    }
    unsigned long* packetCursor = (unsigned long*)(self->bufAddr | 0x30000000);
    if (n != 0) {
        do {
            unsigned long prim = ((unsigned long)primBits << 47) | ((unsigned long)0x8000 << 31);
            int batchCount = n < 37 ? n : 36;
            int lim = 54 - start;
            int headCount = batchCount < lim ? batchCount : lim;
            int wrapCount = batchCount - headCount;
            sPair_7EC0 lastPair = D_004A43A8[0];
            if (wrapCount < 0) {
                wrapCount = 0;
            }
            if (n <= batchCount) {
                lastPair.a = (start + batchCount - 2) % 54;
                lastPair.b = (start + batchCount - 1) % 54;
                if (wrapCount >= 2) {
                    wrapCount -= 2;
                } else {
                    int trimmedHead = headCount - 2;
                    headCount = trimmedHead + wrapCount;
                    wrapCount = 0;
                }
            }
            unsigned int unpackCmd = 0x6C000000;
            packetCursor[0] = 0x10000003;
            packetCursor[3] = (unsigned long)55301 << 47;
            packetCursor[4] = (unsigned long)((batchCount * 2) | 0x8000) | prim | ((unsigned long)0xC000 << 46);
            packetCursor[6] = (long)(batchCount * 3) | ((long)batchCount << 32);
            packetCursor[7] = *(unsigned int*)&fb | ((unsigned long)*(unsigned int*)&fa << 32);
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x1000101 << 32;
            packetCursor[5] = regList;
            packetCursor += 8;
            unsigned int vuAddr = 2;
            if (headCount != 0) {
                packetCursor[0] = 0x10000001;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                int headQwords = headCount * 3;
                unsigned int hm = (headQwords << 16) | 0x8000;
                packetCursor[3] = (unsigned long)((vuAddr | hm) | unpackCmd) << 32;
                packetCursor[4] = (unsigned long)(headQwords | 0x30000000) | ((unsigned long)(unsigned int)((int)tbl[ia] + start * 0x30) << 32);
                packetCursor[5] = 0;
                vuAddr = headQwords + 2;
                packetCursor += 6;
            }
            if (wrapCount != 0) {
                packetCursor[0] = 0x10000001;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                int wrapQwords = wrapCount * 3;
                unsigned int hr = (wrapQwords << 16) | 0x8000;
                packetCursor[3] = (unsigned long)((vuAddr | hr) | unpackCmd) << 32;
                packetCursor[4] = (unsigned long)(wrapQwords | 0x30000000) | ((unsigned long)(unsigned int)tbl[ia] << 32);
                packetCursor[5] = 0;
                vuAddr += wrapQwords;
                packetCursor += 6;
            }
            if (lastPair.a >= 0) {
                packetCursor[0] = 0x10000007;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                unsigned int lo = vuAddr | 0x68000;
                packetCursor[3] = (unsigned long)(lo | unpackCmd) << 32;
                vuAddr += 6;
                packetCursor += 4;
                *(sVtx_7EC0*)packetCursor = tbl[ia][lastPair.a];
                packetCursor += 6;
                *(sVtx_7EC0*)packetCursor = tbl[ia][lastPair.b];
                packetCursor += 6;
            }
            if (headCount != 0) {
                packetCursor[0] = 0x10000001;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                int headQwords = headCount * 3;
                unsigned int hm = (headQwords << 16) | 0x8000;
                packetCursor[3] = (unsigned long)((vuAddr | hm) | unpackCmd) << 32;
                packetCursor[4] = (unsigned long)(headQwords | 0x30000000) | ((unsigned long)(unsigned int)((int)tbl[ib] + start * 0x30) << 32);
                packetCursor[5] = 0;
                vuAddr += headQwords;
                packetCursor += 6;
            }
            if (wrapCount != 0) {
                packetCursor[0] = 0x10000001;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                int wrapQwords = wrapCount * 3;
                unsigned int hr = (wrapQwords << 16) | 0x8000;
                packetCursor[3] = (unsigned long)((vuAddr | hr) | unpackCmd) << 32;
                packetCursor[4] = (unsigned long)(wrapQwords | 0x30000000) | ((unsigned long)(unsigned int)tbl[ib] << 32);
                packetCursor[5] = 0;
                vuAddr += wrapQwords;
                packetCursor += 6;
            }
            if (lastPair.a >= 0) {
                packetCursor[0] = 0x10000007;
                packetCursor[1] = 0;
                packetCursor[2] = (unsigned long)0x1000101 << 32;
                unsigned int lo2 = vuAddr | 0x68000;
                packetCursor[3] = (unsigned long)(lo2 | unpackCmd) << 32;
                packetCursor += 4;
                *(sVtx_7EC0*)packetCursor = tbl[ib][lastPair.a];
                packetCursor += 6;
                *(sVtx_7EC0*)packetCursor = tbl[ib][lastPair.b];
                packetCursor += 6;
            }
            packetCursor[0] = 0x10000001;
            packetCursor[1] = 0;
            packetCursor[2] = 0;
            packetCursor[3] = (unsigned long)(addr | 0x14000000) << 32;
            packetCursor += 4;
            fa += fb * (float)batchCount;
            n -= batchCount;
            if (n > 0) {
                n += 1;
                start = (start + batchCount - 1) % 54;
            }
        } while (n != 0);
    }
    self->bufAddr = (unsigned int)packetCursor & 0x0FFFFFFF;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderBoardWake);
#ifdef SKIP_ASM
struct sPtRS_83B8 {
    int f0;                         // bits 6..9 = layer
    int f4;
    int f8;                         // bits 0..4 = key
    int fC;
    short tex;
    short pad;
};

struct sPtEnt_83B8 {
    sPtRS_83B8 rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};

struct sPtRing_83B8 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sPtEnt_83B8 ents[1];            // 0x80
};

struct sPtGfx_83B8 {
    char pad_0x0[0xE84];
    sPtRS_83B8* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sPtRing_83B8* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

extern int D_004A5B84;
extern sPtRS_83B8 D_00501420_rs __asm__("sDrawState_s_defaultDrawState");

extern "C" void func_0037D968(cPS2GraphicsMan* self);
extern "C" void func_003885E0(void* self, void* a, void* b, void* c, void* d, int i0, int i1, float f0, float f1);

/**
 * Draws a board's snow wake.
 *
 * Sets the wake render mode, clears the 0x30-byte scratch at D_004A5B84 and calls func_003885E0 for each pair of
 * adjacent segments (0-1, 1-2, ...), then queues the packet on the render ring. The C declares the three index
 * arguments as pointers; they are ints passed straight through.
 *
 * @param verts  the wake vertices, one array per segment
 * @param numSegmentsWide  the number of segments across
 * @param startIndex  the first index
 * @param numIndices  the number of indices
 * @param stripLength  the strip length
 * @param alphaMultiplier  the alpha multiplier
 * @param alphaDelta  the alpha change along the strip
 */
// PORT: uncached (0x30000000) pointers held in int.
extern "C" void cPS2GraphicsMan_RenderBoardWake(sPtGfx_83B8* self, void* verts, int numSegmentsWide, void* startIndex, void* numIndices, void* stripLength, float alphaMultiplier, float alphaDelta)
{
    self->top->f0 = (self->top->f0 & ~0x3C0) | 0xC0;
    self->top->fC &= 0xC0000000;
    if (self->texReady == 0) {
        func_0037D968((cPS2GraphicsMan*)self);
    }
    unsigned int buf = self->bufAddr;
    MEM_fill((void*)D_004A5B84, 0, 0x30);
    int i;
    for (i = 0; i < numSegmentsWide - 1; i++) {
        func_003885E0(self, verts, startIndex, numIndices, stripLength, i, i + 1, alphaMultiplier, alphaDelta);
    }
    sPtRing_83B8* ring = self->ring;
    unsigned int next = self->bufAddr;
    sPtRS_83B8* rs = self->top;
    if (ring->count < 2600) {
        rs->f8 = (rs->f8 & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sPtEnt_83B8* ringEntry = (sPtEnt_83B8*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->bufAddr += 0x10;
    self->top->f0 &= ~0x3C0;
    self->top->fC &= 0xC0000000;
    *self->top = D_00501420_rs;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003885E0);
#ifdef SKIP_ASM
struct sVtx_85E0 {
    unsigned long q[6];
} __attribute__((aligned(16)));

struct sPtGfx_85E0 {
    char pad_0x0[0x5A00];
    unsigned int bufAddr;           // 0x5A00
};

extern int D_004A43B0;
extern char D_3A00[];
extern char D_3A08[];

// VIF UNPACK V4-32 code: VU address, qword count, FLG bit.
static inline unsigned int Unpack_85E0(int addr, int num, int flg, unsigned int cmd)
{
    return addr | ((num << 16) | (flg << 15)) | cmd;
}

// The unit declares func_003885E0 with void* parameters (caller cPS2GraphicsMan_RenderBoardWake); bind the real types.
extern "C" void func_003885E0_impl(sPtGfx_85E0* self, sVtx_85E0** strips, int start, int ring, int count, int i0, int i1,
                                   float t, float dt) __asm__("func_003885E0");

/**
 * Sends one board-wake ribbon to VU1: the textured strip between vertex rows @p firstRow and @p secondRow of @p strips, @p count
 * vertices from index @p start of rings of @p ring vertices, in batches of up to 36.
 *
 * Per batch it writes at mVifCursor: a header unpacked to the VU (a GIF tag for 2 * n vertices with PRIM 0x5C and
 * registers ST, RGBAQ, XYZ2; the counts; @p dt and @p alphaStart); for row @p firstRow then row @p secondRow one vertex copied inline, then
 * DMA ref UNPACKs of the vertices from @p start up to the ring end and of any that wrap round to index 0; then a VIF
 * MSCAL of the VU1 program at D_3A00 when D_004A43B0 was set (it is cleared) or D_3A08 otherwise. The inline vertex is
 * vertex @p start on the first batch (which then starts one later) but always vertex 1 on later batches, so those send
 * one vertex more than the header counts; the k >= 0 tests guarding it are always true (looks unintended). Batches
 * overlap by one vertex and @p alphaStart grows by @p dt per vertex. The cursor is stored back to mVifCursor.
 * cPS2GraphicsMan_RenderBoardWake calls it for each pair of neighbouring rows.
 *
 * @param strips  the wake's table of vertex rows (3 quadwords per vertex)
 * @param start  index of the first vertex
 * @param ring  number of vertices in each row's ring
 * @param count  number of vertices to draw
 * @param firstRow  index of the strip's first row
 * @param secondRow  index of its second row
 * @param alphaStart  alpha multiplier for the first vertex
 * @param dt  alpha step per vertex
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit VIF/GIF words are `ulong`.
void func_003885E0_impl(sPtGfx_85E0* self, sVtx_85E0** strips, int start, int ring, int count, int firstRow, int secondRow, float alphaStart,
                        float dt)
{
    unsigned int prog = (unsigned int)(D_004A43B0 ? D_3A00 : D_3A08) >> 3;
    int prim = 92;
    unsigned long regs = 1042;
    int first = 1;
    unsigned long* packetCursor = (unsigned long*)(self->bufAddr | 0x30000000);
    D_004A43B0 = 0;
    while (count != 0) {
        int n = count < 37 ? count : 36;
        int headCount = ring - start;
        if (n < headCount) {
            headCount = n;
        }
        int k = 1;
        int wrapCount = n - headCount;
        if (wrapCount < 0) {
            wrapCount = 0;
        }
        if (first) {
            k = start;
            headCount--;
            start++;
        }
        unsigned int unpack = 0x6C000000;
        packetCursor[0] = 0x10000003;
        packetCursor[1] = 0;
        packetCursor[2] = (unsigned long)0x01000101 << 32;
        packetCursor[3] = (unsigned long)Unpack_85E0(0, 2, 1, unpack) << 32;
        packetCursor[4] = (unsigned long)(n * 2) | ((unsigned long)1 << 15) | ((unsigned long)1 << 46) | ((unsigned long)prim << 47) |
               ((unsigned long)3 << 60);
        packetCursor[5] = regs;
        packetCursor[6] = (unsigned long)(n * 3) | ((unsigned long)n << 32);
        packetCursor[7] = (unsigned long)*(unsigned int*)&dt | ((unsigned long)*(unsigned int*)&alphaStart << 32);
        packetCursor += 8;
        int addr = 2;
        if (k >= 0) {
            packetCursor[0] = 0x10000004;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, 3, 1, unpack) << 32;
            addr += 3;
            packetCursor += 4;
            *(sVtx_85E0*)packetCursor = strips[firstRow][k];
            packetCursor += 6;
        }
        if (headCount != 0) {
            int chunkQwords = headCount * 3;
            packetCursor[0] = 0x10000001;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, chunkQwords, 1, unpack) << 32;
            addr += chunkQwords;
            packetCursor[4] = (unsigned long)(chunkQwords | 0x30000000) | ((unsigned long)(unsigned int)&strips[firstRow][start] << 32);
            packetCursor[5] = 0;
            packetCursor += 6;
        }
        if (wrapCount != 0) {
            int chunkQwords = wrapCount * 3;
            packetCursor[0] = 0x10000001;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, chunkQwords, 1, unpack) << 32;
            addr += chunkQwords;
            packetCursor[4] = (unsigned long)(chunkQwords | 0x30000000) | ((unsigned long)(unsigned int)strips[firstRow] << 32);
            packetCursor[5] = 0;
            packetCursor += 6;
        }
        if (k >= 0) {
            packetCursor[0] = 0x10000004;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, 3, 1, unpack) << 32;
            addr += 3;
            packetCursor += 4;
            *(sVtx_85E0*)packetCursor = strips[secondRow][k];
            packetCursor += 6;
        }
        if (headCount != 0) {
            int chunkQwords = headCount * 3;
            packetCursor[0] = 0x10000001;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, chunkQwords, 1, unpack) << 32;
            addr += chunkQwords;
            packetCursor[4] = (unsigned long)(chunkQwords | 0x30000000) | ((unsigned long)(unsigned int)&strips[secondRow][start] << 32);
            packetCursor[5] = 0;
            packetCursor += 6;
        }
        if (wrapCount != 0) {
            int chunkQwords = wrapCount * 3;
            packetCursor[0] = 0x10000001;
            packetCursor[1] = 0;
            packetCursor[2] = (unsigned long)0x01000101 << 32;
            packetCursor[3] = (unsigned long)Unpack_85E0(addr, chunkQwords, 1, unpack) << 32;
            addr += chunkQwords;
            packetCursor[4] = (unsigned long)(chunkQwords | 0x30000000) | ((unsigned long)(unsigned int)strips[secondRow] << 32);
            packetCursor[5] = 0;
            packetCursor += 6;
        }
        packetCursor[0] = 0x10000001;
        packetCursor[1] = 0;
        packetCursor[2] = 0;
        packetCursor[3] = (unsigned long)(prog | 0x14000000) << 32;
        packetCursor += 4;
        count -= n;
        alphaStart += dt * (float)n;
        if (count > 0) {
            start = (start + n - 1) % ring;
            count++;
            first = 0;
        }
    }
    self->bufAddr = (unsigned int)packetCursor & 0x0FFFFFFF;
}
#endif

/**
 * Reads the colour of a texture at a texture coordinate.
 *
 * Asm only so far. Looks the texture up in the texture manager's table (+0x18F4); a missing texture gives the colour 1,
 * 1, 0, 1. Otherwise it scales u and v by the texture's width and height (from the TEX0 log2 fields), clamps them to
 * the texture and reads the texel. The filter argument is not used: $7 is overwritten before it is read.
 *
 * @return the colour, through the hidden result pointer
 */
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetTexel);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetScreenClearColour);
#ifdef SKIP_ASM
extern "C" int fptoui(float f);

/**
 * Sets the screen clear colour: mClearColour (+0x6AE0) = the colour's r, g, b times 255 converted (fptoui) and
 * packed as 0x00BBGGRR.
 *
 * @param col  the colour as three floats in 0..1 (On Tour col)
 */
extern "C" void cPS2GraphicsMan_SetScreenClearColour(cPS2GraphicsMan* self, const float* col)
{
    float k = 255.0f;
    int packed = fptoui(col[0] * k);
    packed |= fptoui(col[1] * k) << 8;
    packed |= fptoui(col[2] * k) << 16;
    *(int*)((char*)self + 0x6AE0) = packed;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_loadexecpurge);
#ifdef SKIP_ASM
extern "C" void DisableIntc(int);
extern "C" void RemoveIntcHandler(int, int);
extern "C" void DisableDmac(int);
extern "C" void RemoveDmacHandler(int, int);

/**
 * Removes the renderer's interrupt handlers before another executable is loaded.
 *
 * Calls DisableIntc(2) and removes the INTC 2 handler (+0x5AC0), then DisableDmac(1) and removes the DMAC 0 handler
 * (+0x5AC4): the same first steps as cPS2GraphicsMan_Purge, leaving the threads and memory alone.
 */
extern "C" void cPS2GraphicsMan_loadexecpurge(cPS2GraphicsMan* self)
{
    DisableIntc(2);
    RemoveIntcHandler(2, self->mIntcHandlerId);
    DisableDmac(1);
    RemoveDmacHandler(0, self->mDmacHandlerId);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389160);
#ifdef SKIP_ASM
/**
 * Fills @p dst with ten vectors built from 30 floats taken three at a time: vector i = (f[3i], f[3i + 1], f[3i + 2], 0)
 * and returns @p dst.
 *
 * The floats arrive in fparg0-7 and then on the stack (arg_sp0 to arg_spA8); the w words are stored as integer 0.
 * Nothing in the retail code references it.
 *
 * @param dst  the ten 16-byte vectors to fill
 * @param fparg0  x of vector 0
 * @param fparg1  y of vector 0
 * @param fparg2  z of vector 0
 * @param fparg3  x of vector 1
 * @param fparg4  y of vector 1
 * @param fparg5  z of vector 1
 * @param fparg6  x of vector 2
 * @param fparg7  y of vector 2
 * @param arg_sp0  z of vector 2
 * @param arg_sp8  x of vector 3
 * @param arg_sp10  y of vector 3
 * @param arg_sp18  z of vector 3
 * @param arg_sp20  x of vector 4
 * @param arg_sp28  y of vector 4
 * @param arg_sp30  z of vector 4
 * @param arg_sp38  x of vector 5
 * @param arg_sp40  y of vector 5
 * @param arg_sp48  z of vector 5
 * @param arg_sp50  x of vector 6
 * @param arg_sp58  y of vector 6
 * @param arg_sp60  z of vector 6
 * @param arg_sp68  x of vector 7
 * @param arg_sp70  y of vector 7
 * @param arg_sp78  z of vector 7
 * @param arg_sp80  x of vector 8
 * @param arg_sp88  y of vector 8
 * @param arg_sp90  z of vector 8
 * @param arg_sp98  x of vector 9
 * @param arg_spA0  y of vector 9
 * @param arg_spA8  z of vector 9
 * @return @p dst
 */
extern "C" void *func_00389160(void *dst, float fparg0, float fparg1, float fparg2, float fparg3, float fparg4, float fparg5, float fparg6, float fparg7, float arg_sp0, float arg_sp8, float arg_sp10, float arg_sp18, float arg_sp20, float arg_sp28, float arg_sp30, float arg_sp38, float arg_sp40, float arg_sp48, float arg_sp50, float arg_sp58, float arg_sp60, float arg_sp68, float arg_sp70, float arg_sp78, float arg_sp80, float arg_sp88, float arg_sp90, float arg_sp98, float arg_spA0, float arg_spA8) {
    (*(float *)((char*)(dst) + (0x10))) = fparg3;
    (*(float *)((char*)(dst) + (0x20))) = fparg6;
    (*(float *)((char*)(dst) + (0x30))) = arg_sp8;
    (*(float *)((char*)(dst) + (0x40))) = arg_sp20;
    (*(float *)((char*)(dst) + (0x50))) = arg_sp38;
    (*(float *)((char*)(dst) + (0x60))) = arg_sp50;
    (*(float *)((char*)(dst) + (0x70))) = arg_sp68;
    (*(float *)((char*)(dst) + (0x80))) = arg_sp80;
    (*(float *)((char*)(dst) + (0x90))) = arg_sp98;
    (*(float *)((char*)(dst) + (4))) = fparg1;
    (*(float *)((char*)(dst) + (0))) = fparg0;
    (*(float *)((char*)(dst) + (0x14))) = fparg4;
    (*(float *)((char*)(dst) + (0x24))) = fparg7;
    (*(float *)((char*)(dst) + (0x34))) = arg_sp10;
    (*(float *)((char*)(dst) + (0x44))) = arg_sp28;
    (*(float *)((char*)(dst) + (0x54))) = arg_sp40;
    (*(float *)((char*)(dst) + (0x64))) = arg_sp58;
    (*(float *)((char*)(dst) + (0x74))) = arg_sp70;
    (*(float *)((char*)(dst) + (0x84))) = arg_sp88;
    (*(float *)((char*)(dst) + (0x94))) = arg_spA0;
    (*(float *)((char*)(dst) + (8))) = fparg2;
    (*(float *)((char*)(dst) + (0x18))) = fparg5;
    (*(float *)((char*)(dst) + (0x28))) = arg_sp0;
    (*(float *)((char*)(dst) + (0x38))) = arg_sp18;
    (*(float *)((char*)(dst) + (0x48))) = arg_sp30;
    (*(float *)((char*)(dst) + (0x58))) = arg_sp48;
    (*(float *)((char*)(dst) + (0x68))) = arg_sp60;
    (*(float *)((char*)(dst) + (0x78))) = arg_sp78;
    (*(float *)((char*)(dst) + (0x88))) = arg_sp90;
    (*(float *)((char*)(dst) + (0x98))) = arg_spA8;
    (*(int *)((char*)(dst) + (0xC))) = 0;
    (*(int *)((char*)(dst) + (0x1C))) = 0;
    (*(int *)((char*)(dst) + (0x2C))) = 0;
    (*(int *)((char*)(dst) + (0x3C))) = 0;
    (*(int *)((char*)(dst) + (0x4C))) = 0;
    (*(int *)((char*)(dst) + (0x5C))) = 0;
    (*(int *)((char*)(dst) + (0x6C))) = 0;
    (*(int *)((char*)(dst) + (0x7C))) = 0;
    (*(int *)((char*)(dst) + (0x8C))) = 0;
    (*(int *)((char*)(dst) + (0x9C))) = 0;
    return dst;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_Clear);
#ifdef SKIP_ASM
struct sPartVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

/**
 * Zeroes an irradiance set: all four components of its ten coefficient vectors.
 *
 * func_00389C38, the front-end rider lighting in fe/feridermanager and cRider_SetRiderLights call it before adding
 * lights.
 *
 * @param vecs  the ten vectors
 */
extern "C" void cIrradiance_Clear(sPartVec4* vecs)
{
    for (int i = 0; i < 10; i++) {
        vecs[i].x = vecs[i].y = vecs[i].z = vecs[i].w = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389290);
#ifdef SKIP_ASM
/**
 * Copies ten rows of four floats from @p src, whose rows start three floats apart, into @p dst's 16-byte rows, with row
 * 2 the negation of row 1.
 *
 * dst row 0 = src[0..3], row 1 = src[3..6], row 2 = -src[3..6], and rows 3 to 9 = src[6..9] up to src[24..27]; each row
 * read takes the next row's first float as its fourth. Nothing in the retail code references it.
 *
 * @param dst  the ten output rows
 * @param src  the input floats (at least 28)
 */
extern "C" void func_00389290(float* dst, float* src)
{
    for (int i = 0; i < 4; i++) {
        dst[0] = src[0];
        dst[4] = src[3];
        dst[8] = -src[3];
        dst[12] = src[6];
        dst[16] = src[9];
        dst[20] = src[12];
        dst[24] = src[15];
        dst[28] = src[18];
        dst[32] = src[21];
        dst[36] = src[24];
        dst++;
        src++;
    }
}
#endif

INCLUDE_ASM("render/particle", func_00389308);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_00389520);
#ifdef SKIP_ASM
extern "C" void func_00389308(void* self, void* dst, float w, float x, float y, float z);

/**
 * Adds a light of colour @p dst arriving from direction @p normal to irradiance set @p self: calls func_00389308(self,
 * dst, 1.0, normal x, y, z).
 *
 * The parameter named dst is an input (the colour). func_0038A6A8 and cRider_SetRiderLights call it.
 *
 * @param normal  the direction the light comes from (x, y, z)
 * @param dst  the light colour (r, g, b)
 */
extern "C" void func_00389520(void* self, const float* normal, void* dst)
{
    func_00389308(self, dst, 1.0f, normal[0], normal[1], normal[2]);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389558);
#ifdef SKIP_ASM
/**
 * Adds @p src's first three floats to @p dst's.
 *
 * cRider_SetRiderLights uses it to add a colour to the first vector of the rider's irradiance set.
 *
 * @param dst  the three floats to add to
 * @param src  the three floats to add
 */
extern "C" void func_00389558(float* dst, float* src)
{
    dst[0] += src[0];
    dst[1] += src[1];
    dst[2] += src[2];
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389590);
#ifdef SKIP_ASM
/**
 * Adds a tinted copy of irradiance set @p src to @p dst: each of the ten vectors gains src * @p scale times the tint, x
 * by @p weights' y, y by its z, z by its w and w by its x (the tint is a, r, g, b).
 *
 * func_00389C38 adds the irradiance database's default entry with it, and the front-end rider lighting
 * (cFERider_render) adds the entry for the rider's name, both tinted by D_00504810.
 *
 * @param dst  the set to add to
 * @param src  the set to add
 * @param weights  the tint, as a, r, g, b
 * @param scale  overall factor
 */
extern "C" void func_00389590(sPartVec4* dst, sPartVec4* src, sPartVec4* weights, float scale)
{
    float kx = scale * weights->x;
    float ky = scale * weights->y;
    float kz = scale * weights->z;
    float kw = scale * weights->w;
    for (int i = 0; i < 10; i++) {
        dst[i].x += src[i].x * ky;
        dst[i].y += src[i].y * kz;
        dst[i].z += src[i].z * kw;
        dst[i].w += src[i].w * kx;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389620);
#ifdef SKIP_ASM
struct sVec4_9620 {
    float x, y, z, w;
};

/**
 * Adds @p src's fourth components times @p dt to @p dst's, for the ten vectors of an irradiance set; x, y and z are not
 * touched.
 *
 * func_00389CB8 adds its sun term with it.
 *
 * @param dst  the set to add to
 * @param src  the set whose w components are added
 * @param dt  the factor
 */
extern "C" void func_00389620(sVec4_9620* dst, sVec4_9620* src, float dt)
{
    float* srcW = &src->w;
    float* dstW = &dst->w;
    for (int i = 0; i < 10; i++) {
        *dstW += *srcW * dt;
        dstW += 4;
        srcW += 4;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389660);
#ifdef SKIP_ASM
struct sV4_9660 { float x, y, z, w; };

/**
 * Tints irradiance set @p vecs in place: x of each of the ten vectors times @p weights[1], y times [2], z times [3] and
 * w times [0] (the tint is a, r, g, b).
 *
 * Nothing in the retail code references it.
 *
 * @param vecs  the set to tint
 * @param weights  the tint, as a, r, g, b
 */
extern "C" void func_00389660(sV4_9660* vecs, float* weights)
{
    for (int i = 0; i < 10; i++) {
        vecs[i].x *= weights[1];
        vecs[i].y *= weights[2];
        vecs[i].z *= weights[3];
        vecs[i].w *= weights[0];
    }
}
#endif
//100%
INCLUDE_ASM("render/particle", cIrradiance_Scale);
#ifdef SKIP_ASM
struct sV4_96C0 { float x, y, z, w; };

/**
 * Scales irradiance set @p vecs in place by @p scale and a tint: x of each of the ten vectors times scale *
 * @p weights[1], y times scale * [2], z times scale * [3], w times scale * [0].
 *
 * Nothing in the retail code calls it directly.
 *
 * @param vecs  the set to scale
 * @param weights  the tint, as a, r, g, b
 * @param scale  overall factor
 */
extern "C" void cIrradiance_Scale(sV4_96C0* vecs, float* weights, float scale)
{
    for (int i = 0; i < 10; i++) {
        vecs[i].x = vecs[i].x * scale * weights[1];
        vecs[i].y = vecs[i].y * scale * weights[2];
        vecs[i].z = vecs[i].z * scale * weights[3];
        vecs[i].w = vecs[i].w * scale * weights[0];
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389730);
#ifdef SKIP_ASM
struct sPartVec4x10 {
    sPartVec4 v[10];
};

// PORT: VU0 macro-mode vector add; the PC port needs plain C.
static inline sPartVec4 sPartVec4_add(const sPartVec4& a, const sPartVec4& b)
{
    sPartVec4 r;
    __asm__(
        "lqc2       $vf3, 0x0(%1)\n"
        "lqc2       $vf4, 0x0(%2)\n"
        "vadd.xyzw  $vf5, $vf3, $vf4\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "r"(&a), "r"(&b));
    return r;
}

/**
 * Returns the sum of irradiance sets @p lhs and @p rhs, vector by vector (VU0 adds).
 *
 * cRenderStateMan_update calls it.
 *
 * @param lhs  the first set
 * @param rhs  the second set
 * @return the sum (returned through the hidden result pointer)
 */
extern "C" sPartVec4x10 func_00389730(sPartVec4x10* lhs, sPartVec4x10* rhs)
{
    sPartVec4x10 sum;
    for (int i = 0; i < 10; i++) {
        sum.v[i] = sPartVec4_add(lhs->v[i], rhs->v[i]);
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003897A0);
#ifdef SKIP_ASM
// PORT: VU0 macro-mode vector scale; the PC port needs plain C.
static inline sPartVec4 sPartVec4_scale(const sPartVec4& a, float s)
{
    sPartVec4 r;
    __asm__(
        "mfc1       $2, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "qmtc2.ni   $2, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "r"(&a), "f"(s)
        : "$2");
    return r;
}

/**
 * Returns irradiance set @p src scaled by @p scale (all four components of each vector, VU0).
 *
 * cRenderStateMan_update calls it.
 *
 * @param src  the set to scale
 * @param scale  the factor
 * @return the scaled copy (returned through the hidden result pointer)
 */
extern "C" sPartVec4x10 func_003897A0(sPartVec4x10* src, float scale)
{
    sPartVec4x10 scaled;
    for (int i = 0; i < 10; i++) {
        scaled.v[i] = sPartVec4_scale(src->v[i], scale);
    }
    return scaled;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389810);
#ifdef SKIP_ASM
/**
 * Scales irradiance set @p vecs in place by @p scale (all four components of each vector, VU0) and returns it.
 *
 * cRenderStateMan_update calls it.
 *
 * @param vecs  the set to scale
 * @param scale  the factor
 * @return @p vecs
 */
// PORT: VU0 macro-mode vector scale (vec4 *= s); the PC port needs plain C.
extern "C" sPartVec4* func_00389810(sPartVec4* vecs, float scale)
{
    for (int i = 0; i < 10; i++) {
        __asm__ __volatile__(
            "mfc1       $2, %1\n"
            "lqc2       $vf4, 0x0(%0)\n"
            "qmtc2.ni   $2, $vf3\n"
            "vmulx.xyzw $vf5, $vf4, $vf3x\n"
            "sqc2       $vf5, 0x0(%0)\n"
            :
            : "r"(&vecs[i]), "f"(scale)
            : "$2", "memory");
    }
    return vecs;
}
#endif

INCLUDE_ASM("render/particle", func_00389840);

//100%
INCLUDE_ASM("render/particle", func_00389C38);
#ifdef SKIP_ASM
struct sParticleEntryA0;
extern "C" sParticleEntryA0* cIrradianceDataBase_Get(cIrradianceDataBase* self, int i);
extern "C" void func_00389CB8(sPartVec4* self, void* p, float s);
extern int D_004A43C4;

struct sPartLib_9C38 {
    int f0;
    int count;      // 0x4
    void* items;    // 0x8
};
extern sPartLib_9C38 D_005047F8;
extern sPartVec4 D_00504810;

/**
 * Builds the default irradiance set in @p self: clears it, adds the irradiance database's default entry and, when
 * @p sunPos is given, a sun term.
 *
 * Clears the set (cIrradiance_Clear); when the database D_005047F8 has entries, adds entry D_004A43C4 (cIrradianceDataBase_Get)
 * tinted by D_00504810 (func_00389590, scale 1); when @p sunPos is not 0 adds the sun term for that position with
 * func_00389CB8 (scale 1). func_00122048 in ai/rider calls it with @p sunPos = 0.
 *
 * @param sunPos  the sun position passed to func_00389CB8, or 0 for none
 */
extern "C" void func_00389C38(sPartVec4* self, void* sunPos)
{
    cIrradiance_Clear(self);
    sPartLib_9C38* lib = &D_005047F8;
    if (lib->count > 0) {
        func_00389590(self, (sPartVec4*)cIrradianceDataBase_Get((cIrradianceDataBase*)lib, D_004A43C4), &D_00504810, 1.0f);
    }
    if (sunPos != 0) {
        func_00389CB8(self, sunPos, 1.0f);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_00389CB8);
#ifdef SKIP_ASM
struct sV_9CB8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_9CB8 {
    sV_9CB8 r[4];
    sMat_9CB8() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sMat_9CB8(const sMat_9CB8& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sMat_9CB8& operator=(const sMat_9CB8& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
        return *this;
    }
};

struct sMat3_9CB8 {
    float m[3][3];
};

struct sVEnt_9CB8 {
    short delta;
    short index;
    sMat_9CB8* (*fn)(void*);
};

extern float D_004A43C0[];
extern int D_004A59A4;
extern char* D_004A5B80_9CB8 __asm__("cGraphicsMan_TheGraphicsMan");
extern sV_9CB8 t4vect_Origin;
extern sV_9CB8 D_004FF140_9CB8 __asm__("t4vect_XAxis");
extern sV_9CB8 D_004FF150_9CB8 __asm__("t4vect_YAxis");
extern sV_9CB8 D_004FF160_9CB8 __asm__("t4vect_ZAxis");
extern sMat_9CB8 D_004FF1A0_9CB8 __asm__("t4mat_Identity");
extern sMat_9CB8 D_00500C60;

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C128(float x);
extern "C" float func_0031C228(float x);
extern "C" void func_00389840(sV_9CB8* out, sMat3_9CB8* m, int a, int b);
extern "C" void func_00389620_9CB8(void* dst, sV_9CB8* src, float dt) __asm__("func_00389620");

static inline sV_9CB8 GetPos_9CB8(const sMat_9CB8& m)
{
    return m.r[3];
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV_9CB8 Scale_9CB8(const sV_9CB8& v, float s)
{
    sV_9CB8 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV_9CB8 Sub_9CB8(const sV_9CB8& a, const sV_9CB8& b)
{
    sV_9CB8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sV_9CB8 Norm_9CB8(const sV_9CB8& v)
{
    sV_9CB8 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_9CB8(const sV_9CB8& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_9CB8(const sV_9CB8& a, const sV_9CB8& b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline sV_9CB8 MulVec_9CB8(const sMat_9CB8* m, const sV_9CB8& v)
{
    sV_9CB8 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix product d = a * b).
static inline void MulMat_9CB8(sMat_9CB8* d, const sMat_9CB8* a, const sMat_9CB8* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// Fills m column by column.
static inline void SetCols_9CB8(sMat_9CB8* m, float a00, float a10, float a20, float a30, float a01, float a11,
                                float a21, float a31, float a02, float a12, float a22, float a32, float a03,
                                float a13, float a23, float a33)
{
    m->r[0].x = a00;
    m->r[1].x = a10;
    m->r[2].x = a20;
    m->r[3].x = a30;
    m->r[0].y = a01;
    m->r[1].y = a11;
    m->r[2].y = a21;
    m->r[3].y = a31;
    m->r[0].z = a02;
    m->r[1].z = a12;
    m->r[2].z = a22;
    m->r[3].z = a32;
    m->r[0].w = a03;
    m->r[1].w = a13;
    m->r[2].w = a23;
    m->r[3].w = a33;
}

// Fills m column by column.
static inline void SetColsR_9CB8(sMat_9CB8* m, float a00, float a10, float a20, float a30,
                                 float a01, float a11, float a21, float a31,
                                 float a02, float a12, float a22, float a32,
                                 float a03, float a13, float a23, const float& a33)
{
    m->r[0].x = a00;
    m->r[1].x = a10;
    m->r[2].x = a20;
    m->r[3].x = a30;
    m->r[0].y = a01;
    m->r[1].y = a11;
    m->r[2].y = a21;
    m->r[3].y = a31;
    m->r[0].z = a02;
    m->r[1].z = a12;
    m->r[2].z = a22;
    m->r[3].z = a32;
    m->r[0].w = a03;
    m->r[1].w = a13;
    m->r[2].w = a23;
    m->r[3].w = a33;
}

typedef int tMmiQuad __attribute__((mode(TI)));

// PORT: hand-written MMI 4x4 transpose in place (lq/pextlw/pextuw/pcpyld/pcpyud/sq: no gcc 2.95 pattern); a port
// swaps m[i][j] and m[j][i] in plain C.
#define MMI_TRANSPOSE4(m)                                                                                              \
    {                                                                                                                  \
        tMmiQuad r0_, r1_, r2_, r3_, t0_, t1_, t2_, t3_;                                                               \
        __asm__ __volatile__(                                                                                          \
            "lq      %0, 0x0(%8)\n"                                                                                    \
            "lq      %1, 0x10(%8)\n"                                                                                   \
            "lq      %2, 0x20(%8)\n"                                                                                   \
            "lq      %3, 0x30(%8)\n"                                                                                   \
            "pextlw  %4, %1, %0\n"                                                                                     \
            "pextuw  %5, %1, %0\n"                                                                                     \
            "pextlw  %6, %3, %2\n"                                                                                     \
            "pextuw  %7, %3, %2\n"                                                                                     \
            "pcpyld  %0, %6, %4\n"                                                                                     \
            "pcpyud  %1, %4, %6\n"                                                                                     \
            "pcpyld  %2, %7, %5\n"                                                                                     \
            "pcpyud  %3, %5, %7\n"                                                                                     \
            "sq      %0, 0x0(%8)\n"                                                                                    \
            "sq      %1, 0x10(%8)\n"                                                                                   \
            "sq      %2, 0x20(%8)\n"                                                                                   \
            "sq      %3, 0x30(%8)\n"                                                                                   \
            : "=&r"(r0_), "=&r"(r1_), "=&r"(r2_), "=&r"(r3_), "=&r"(t0_), "=&r"(t1_), "=&r"(t2_), "=&r"(t3_)          \
            : "r"(m)                                                                                                   \
            : "memory");                                                                                               \
    }

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_9CB8(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (row 3 = m * v: translate in the matrix's own frame).
static inline void Translate_9CB8(sMat_9CB8* m, const sV_9CB8& v)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, %1\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, 0x30(%0)\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

static inline sMat_9CB8 Mul_9CB8(const sMat_9CB8& a, const sMat_9CB8& b)
{
    sMat_9CB8 d;
    MulMat_9CB8(&d, &a, &b);
    return d;
}

static inline sMat3_9CB8 ToMat3_9CB8(const sMat_9CB8& m)
{
    sMat3_9CB8 r;
    r.m[0][0] = m.r[0].x;
    r.m[0][1] = m.r[0].y;
    r.m[0][2] = m.r[0].z;
    r.m[1][0] = m.r[1].x;
    r.m[1][1] = m.r[1].y;
    r.m[1][2] = m.r[1].z;
    r.m[2][0] = m.r[2].x;
    r.m[2][1] = m.r[2].y;
    r.m[2][2] = m.r[2].z;
    return r;
}

static inline float Clamp_9CB8(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// Rotates R about axis by angle, keeping its translation: R = Rot(axis, angle, t) * R'.
static inline void RotateAxis_9CB8(sMat_9CB8* R, const sV_9CB8* axis, float angle)
{
    sMat_9CB8 old = *R;
    sV_9CB8 t = old.r[3];
    old.r[3] = t4vect_Origin;
    float s;
    float c;
    fSinCos(&s, &c, angle);
    float one = 1.0f;
    float k = one - c;
    float x = axis->x;
    float y = axis->y;
    float z = axis->z;
    float tx = k * x;
    float ty = k * y;
    float tz = k * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    SetColsR_9CB8(R, tx * x + c, tx * y + sz, tx * z - sy, 0.0f,
                 ty * x - sz, ty * y + c, ty * z + sx, 0.0f,
                 tz * x + sy, tz * y - sx, tz * z + c, 0.0f,
                 0.0f, 0.0f, 0.0f, one);
    R->r[3] = t;
    MulMat_9CB8(R, R, &old);
}

/**
 * Adds a sun term to the fourth (w) components of irradiance set @p out: a fixed lobe of coefficients turned toward the
 * sun as seen from the camera.
 *
 * Ten w coefficients are set to a fixed lobe (0.4954, 0, D_004A43C0[0], -1.48625, 0, 0, 0, 0.20467, 0, 0). The camera
 * position comes from the graphics manager's view (virtual slot 35, cPS2GraphicsMan_GetView__FPv); the direction from
 * it to *@p sunp gives an elevation (func_0031C228) and an azimuth (from func_0031C128 against the x axis). A rotation
 * built from them (translate to the eye, rotate about y by -elevation and z by -azimuth, then a fixed axis swap held in
 * the function-static D_00500C60, set up on first use with guard D_004A59A4) is reduced to 3 x 3 and applied to the
 * lobe by func_00389840; the result times @p scale * 0.7500066 is added with func_00389620. func_00389C38 calls it.
 *
 * @param out  the set to add to
 * @param sunp  the sun position
 * @param scale  factor for the sun term
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm and the
// g++ `<?` operator (retail's min.s); a port replaces them with a plain C transpose, C vector math and a ternary min.
extern "C" void func_00389CB8(sPartVec4* out, void* sunp, float scale)
{
    sV_9CB8* sun = (sV_9CB8*)sunp;
    sV_9CB8 prm[10];
    prm[0].w = 0.49541598558425903f;
    prm[1].w = 0.0f;
    prm[2].w = D_004A43C0[0];
    prm[3].w = -1.4862500429153442f;
    prm[4].w = 0.0f;
    prm[5].w = 0.0f;
    prm[6].w = 0.0f;
    prm[7].w = 0.20466558635234833f;
    prm[8].w = 0.0f;
    prm[9].w = 0.0f;
    char* mgr = D_004A5B80_9CB8;
    sVEnt_9CB8* vt = *(sVEnt_9CB8**)(mgr + 0x10D8);
    sMat_9CB8 cam = *vt[35].fn(mgr + vt[35].delta);
    sV_9CB8 eye = Scale_9CB8(GetPos_9CB8(cam), -1.0f);
    MMI_TRANSPOSE4(&cam);
    eye = MulVec_9CB8(&cam, eye);
    eye.w = 1.0f;
    sV_9CB8 sunDir = Norm_9CB8(Sub_9CB8(*sun, eye));
    float z = sunDir.z;
    sunDir.z = 0.0f;
    float len = Len_9CB8(sunDir);
    float elev = -func_0031C228(z / len);
    float azim;
    if (0.0f < len) {
        float normProduct = Sqrt_9CB8(Dot_9CB8(sunDir, sunDir) * Dot_9CB8(D_004FF140_9CB8, D_004FF140_9CB8));
        if (normProduct < 0.0010000000474974513f) {
            azim = 0.0f;
        } else {
            azim = 1.5707963705062866f - func_0031C128(Clamp_9CB8(Dot_9CB8(sunDir, D_004FF140_9CB8) / normProduct, -1.0f, 1.0f));
        }
        if (sunDir.y < 0.0f) {
            azim = 6.2831854820251465f - azim;
        }
    } else {
        azim = 0.0f;
    }
    sMat_9CB8 R = D_004FF1A0_9CB8;
    Translate_9CB8(&R, eye);
    RotateAxis_9CB8(&R, &D_004FF150_9CB8, -elev);
    RotateAxis_9CB8(&R, &D_004FF160_9CB8, -azim);
    MMI_TRANSPOSE4(&R);
    // NOTE: D_00500C60 is a function-static matrix and D_004A59A4 its first-use guard.
    if (D_004A59A4 == 0) {
        SetCols_9CB8(&D_00500C60, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        D_004A59A4 = 1;
    }
    sMat_9CB8 basisRotation = Mul_9CB8(D_00500C60, R);
    sMat3_9CB8 rotation3x3 = ToMat3_9CB8(basisRotation);
    func_00389840(prm, &rotation3x3, 3, 3);
    func_00389620_9CB8(out, prm, scale * 0.7500065565109253f);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038A530);
#ifdef SKIP_ASM
/**
 * Tests whether @p point is inside a spot light's radius; if so returns 1 with the unit direction to the light, the
 * distance, its inverse and the cosine against the light's direction.
 *
 * The light position is at +0x38 and the radius at +0x1C; returns 0 when the point is farther than the radius. When the
 * point is exactly at the light the direction stays zero and the distance and inverse are 1. @p planeD gets minus the
 * dot product of the direction with the light's direction (+0x2C), i.e. the cosine of the point's angle off the beam.
 * func_0038A6A8 calls it for type 1 lights.
 *
 * @param point  the lit point
 * @param n  receives the unit direction from the point to the light (w = 1)
 * @param forceSrc  the light
 * @param dist  receives the distance
 * @param inv  receives 1 / distance
 * @param planeD  receives the cosine against the light's direction
 * @return 1 when the point is within the radius, else 0
 */
extern "C" int func_0038A530(float* point, float* n, void* forceSrc, float* dist, float* inv, float* planeD)
{
    float length;
    float k;
    float dx = *(float*)((char*)forceSrc + 0x38) - point[0];
    float dy = *(float*)((char*)forceSrc + 0x3C) - point[1];
    float radius = *(float*)((char*)forceSrc + 0x1C);
    float dz = *(float*)((char*)forceSrc + 0x40) - point[2];
    float distSq = dx * dx + dy * dy + dz * dz;
    if (radius * radius < distSq) {
        return 0;
    }
    if (distSq != 0.0f) {
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(length) : "f"(distSq));
        k = 1.0f / length;
    } else {
        k = 1.0f;
        length = k;
    }
    dx *= k;
    dy *= k;
    dz *= k;
    *planeD = -(dx * *(float*)((char*)forceSrc + 0x2C) + dy * *(float*)((char*)forceSrc + 0x30) + dz * *(float*)((char*)forceSrc + 0x34));
    *dist = length;
    *inv = k;
    n[0] = dx;
    n[1] = dy;
    n[2] = dz;
    n[3] = 1.0f;
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038A618);
#ifdef SKIP_ASM
/**
 * Tests whether @p point is inside a point light's radius; if so returns 1 with the unit direction to the light, the
 * distance and its inverse.
 *
 * The light position is at +0x38 and the radius at +0x1C; returns 0 when the point is farther than the radius. Unlike
 * func_0038A530 there is no check for a zero distance before dividing. func_0038A6A8 calls it for type 2 lights.
 *
 * @param point  the lit point
 * @param n  receives the unit direction from the point to the light (w = 1)
 * @param forceSrc  the light
 * @param dist  receives the distance
 * @param inv  receives 1 / distance
 * @return 1 when the point is within the radius, else 0
 */
extern "C" int func_0038A618(float* point, float* n, void* forceSrc, float* dist, float* inv)
{
    float dx = *(float*)((char*)forceSrc + 0x38) - point[0];
    float dy = *(float*)((char*)forceSrc + 0x3C) - point[1];
    float radius = *(float*)((char*)forceSrc + 0x1C);
    float dz = *(float*)((char*)forceSrc + 0x40) - point[2];
    float distSq = dx * dx + dy * dy + dz * dz;
    if (radius * radius < distSq) {
        return 0;
    }
    float length;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(length) : "f"(distSq));
    *dist = length;
    float k = 1.0f / length;
    *inv = k;
    dy *= k;
    dz *= k;
    dx *= k;
    n[3] = 1.0f;
    n[0] = dx;
    n[1] = dy;
    n[2] = dz;
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038A6A8);
#ifdef SKIP_ASM
struct sForce_38A6A8 {
    char pad0[0x10];
    int type;           // 0x10
    float strength;     // 0x14
    char pad18[4];
    float radius;       // 0x1C
    float dir[3];       // 0x20
    char pad2C[0x5C - 0x2C];
    float outer;        // 0x5C
    float inner;        // 0x60
    signed char curve;  // 0x64
    signed char power;  // 0x65
};

struct sVec3_38A6A8 {
    float x, y, z;
};

struct sVec4_38A6A8 {
    float x, y, z, w;
};

static inline float powgen_38A6A8(float x, int n)
{
    if (x == 0.0f)
        return 0.0f;
    if (n < 0) {
        n = -n;
        x = 1.0f / x;
    }
    float r = 1.0f;
    while (n != 0) {
        if (n & 1)
            r *= x;
        n >>= 1;
        x *= x;
    }
    return r;
}

static inline float powi_38A6A8(float x, int n)
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
    }
    return powgen_38A6A8(x, n);
}

static inline float fade_38A6A8(sForce_38A6A8* s, float dist)
{
    if (s->radius < 5000.0f || dist <= 3750.0f)
        return 1.0f;
    return 1.0f - (dist - 3750.0f) * 0.0008f;
}

static inline float curve_38A6A8(sForce_38A6A8* s, float t)
{
    int c = s->curve;
    t *= 100.0f;
    if (t < 1.0f) {
        switch (c) {
        case 1:
            return t;
        case 2:
            return t * t;
        case 3:
            return t * t * t;
        }
    }
    return 1.0f;
}

// PORT: g++ `<?` (min) operator; needs a macro / std::min on a modern compiler.
static inline float clamp_38A6A8(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

/**
 * Adds one world light's contribution at @p point to irradiance set @p self.
 *
 * Type (+0x10) 0 adds nothing. Type 1 (spot): func_0038A530 gives the direction, distance and beam cosine; nothing is
 * added outside the radius or when the cosine is below the inner limit (+0x60). The strength (+0x14) is shaped by the
 * falloff curve (+0x64: 1, 2 or 3 for (100 / distance) to that power once the distance passes 100), a fade from 3750 to
 * 5000 units for lights whose radius is at least 5000, and the beam term (the cosine to the power +0x65 above the outer
 * limit +0x5C, a linear ramp between the limits), then clamped to 0..5. Type 2 (point): func_0038A618, the same without
 * the beam term. The light's colour (+0x20, which the C calls dir) times that factor is added from the light's
 * direction with func_00389520. cWorldLightMan_IntersectLights1 and cRider_SetRiderLights call it.
 *
 * @param point  the lit point
 * @param forceSrc  the world light
 */
extern "C" void func_0038A6A8(void* self, float* point, sForce_38A6A8* forceSrc)
{
    sVec4_38A6A8 n;
    sVec3_38A6A8 out;
    float dist;
    float inv;
    float planeD;
    float k;
    float fade;

    switch (forceSrc->type) {
    case 0:
        break;
    case 1:
        if (!func_0038A530(point, (float*)&n, forceSrc, &dist, &inv, &planeD))
            return;
        if (!(planeD >= forceSrc->inner))
            return;
        fade = fade_38A6A8(forceSrc, dist);
        k = curve_38A6A8(forceSrc, inv) * forceSrc->strength * fade;
        if (planeD >= forceSrc->outer)
            k *= powi_38A6A8(planeD, forceSrc->power);
        else {
            k *= powi_38A6A8(forceSrc->outer, forceSrc->power);
            k *= (planeD - forceSrc->inner) / (forceSrc->outer - forceSrc->inner);
        }
        k = clamp_38A6A8(k, 5.0f);
        {
            float x = forceSrc->dir[0] * k;
            float y = forceSrc->dir[1] * k;
            float z = forceSrc->dir[2] * k;
            out.x = x;
            out.y = y;
            out.z = z;
        }
        func_00389520(self, (float*)&n, &out);
        break;
    case 2:
        if (!func_0038A618(point, (float*)&n, forceSrc, &dist, &inv))
            return;
        fade = fade_38A6A8(forceSrc, dist);
        k = curve_38A6A8(forceSrc, inv) * forceSrc->strength * fade;
        k = clamp_38A6A8(k, 5.0f);
        out.x = forceSrc->dir[0] * k;
        out.y = forceSrc->dir[1] * k;
        out.z = forceSrc->dir[2] * k;
        func_00389520(self, (float*)&n, &out);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradianceDataBase_Get);
#ifdef SKIP_ASM
struct sParticleEntryA0 {
    char pad_0x00[0xA0];
};

/**
 * Returns irradiance entry @p i of the irradiance database (0xA0-byte entries at +0x8), clamping @p i to the last entry
 * (count at +0x4).
 *
 * func_00389C38 and cIrradianceDataBase_Get1 call it.
 *
 * @param i  the entry index
 * @return the entry
 */
extern "C" sParticleEntryA0* cIrradianceDataBase_Get(cIrradianceDataBase* self, int i)
{
    int count = self->mNumItem;
    if (i >= count) {
        i = count - 1;
    }
    return (sParticleEntryA0*)self->mIrradianceArray + i;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradianceDataBase_Get1);
#ifdef SKIP_ASM
extern "C" int cIrradianceDataBase_GetIDByName(cIrradianceDataBase* self, const char* name);

/**
 * Returns the irradiance entry named @p name: cIrradianceDataBase_Get with the index from cIrradianceDataBase_GetIDByName (entry
 * 0 when the name is unknown).
 *
 * The front-end rider lighting (cFERider_render) looks up each rider's entry by name with it.
 *
 * @param name  the entry name
 * @return the entry
 */
extern "C" sParticleEntryA0* cIrradianceDataBase_Get1(cIrradianceDataBase* self, const char* name)
{
    return cIrradianceDataBase_Get(self, cIrradianceDataBase_GetIDByName(self, name));
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradianceDataBase_GetIDByName);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

/**
 * Looks up an irradiance entry by name.
 *
 * Compares @p name with func_004165A8 (strcmp) against each 8-byte name entry of the table at +0xC, for the count at
 * +0x4. Returns 0 (the first entry) when nothing matches, as On Tour does. cSectionMan_setSky uses it for the sky
 * irradiance sets ("APBR1".."EPBR1").
 *
 * @param name  the irradiance name
 * @return the entry's index, or 0 when the name is unknown
 */
extern "C" int cIrradianceDataBase_GetIDByName(cIrradianceDataBase* self, const char* name)
{
    int i;
    for (i = 0; i < self->mNumItem; i++) {
        if (func_004165A8(name, self->mIrradianceNames + (i << 3)) == 0) {
            return i;
        }
    }
    return 0;
}
#endif

