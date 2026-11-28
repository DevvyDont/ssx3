#include "vtable.h"
#include "common.h"
#include "world/patchrecord.h"
#include "render/beziercache.h"
#include "visualfx/snowbox.h"
#include "render/font.h"
#include "render/graphicsman.h"
#include "render/playerlights.h"
#include "render/bxps2deferman.h"
#include "ealib/shape.h"
#include "visualfx/worldlightman.h"
#include "world/instancerecord.h"
#include "world/worldrecords.h"
#include "visualfx/transitionman.h"
#include "ui/uithing.h"
#include "render/2drender.h"
#include "render/simpletube.h"
#include "render/ps2gridmesh.h"
#include "render/irradiance.h"
#include "render/playershadow.h"
#include "render/particle.h"
#include "render/ps2bezierman.h"
#include "render/ps2graphicsman.h"
#include "render/ps2scratchpad.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_Allocate);
#ifdef SKIP_ASM
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
// as operator new[] so the array new-expressions below match.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_DynEmitterData_Positions[];
extern char str_DynEmitterData_Velocities[];

// t4Vector with an empty default constructor (as On Tour's t4Vector()): the array new-expressions below keep their
// construction loop.
struct t4VectorElem : t4Vector {
    t4VectorElem() {}
};

/**
 * Allocates a dynamic emitter's position and velocity arrays, one 16-byte vector per sub-emitter.
 *
 * The count is m_numSubEmitters; positions go to m_positions ("DynEmitterData Positions") and velocities to
 * m_velocities ("DynEmitterData Velocities").
 */
void cBaseClass_DynamicEmitter::Allocate()
{
    // each destination's address is taken before its allocation, as in retail
    t4Vector** positions = &m_positions;
    *positions = new (str_DynEmitterData_Positions, 0, 0) t4VectorElem[m_numSubEmitters];
    t4Vector** velocities = &m_velocities;
    *velocities = new (str_DynEmitterData_Velocities, 0, 0) t4VectorElem[m_numSubEmitters];
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
void cBaseClass_DynamicEmitter::purge(int reset)
{
    if (m_positions != 0) {
        cMemMan_free(m_positions);
        m_positions = 0;
    }
    if (m_velocities != 0) {
        cMemMan_free(m_velocities);
        m_velocities = 0;
    }
    if (reset) {
        m_initialized = 0;
        m_numSubEmitters = -1;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_reset);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero;

/**
 * Resets a dynamic emitter: zeroes both of its vector arrays and its counters.
 *
 * For each of the m_numSubEmitters entries, m_positions[i] and m_velocities[i] are set to zero; m_subEmitterId and
 * m_enabledCount are cleared.
 */
void cBaseClass_DynamicEmitter::reset()
{
    int i;
    m_subEmitterId = 0;
    for (i = 0; i < m_numSubEmitters; i++) {
        m_positions[i] = t4vect_Zero;
        m_velocities[i] = t4vect_Zero;
        m_subEmitterId = 0;
        m_enabledCount = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_setupEmitter);
#ifdef SKIP_ASM
int BXrand();

extern "C" void tParticleEmitter_init(void* sys, int count, int a2, t4Vector* v, float a, float b, float c);
extern "C" void tParticleEmitter_setupEmitter(void* sys, void* a1, void* def, void* a3, float f);
extern t4Matrix t4mat_Identity;

extern sDrawState sDrawState_s_defaultDrawState_0DC8 __asm__("sDrawState_s_defaultDrawState");

// The emitter's virtual slots setupEmitter calls (cBaseClass_DynamicEmitter_vtbl: 1 purge(int), 2 reset, 5 Allocate).
typedef sVEntry<void(void*, int)> tEmitterPurgeSlot;
typedef sVEntry<void(void*)> tEmitterSlot;

// Sets one field of a draw-state word.
static inline void setDrawStateBits(unsigned int& word, int mask, int shift, int v)
{
    word = ((int)word & ~mask) | ((v << shift) & mask);
}

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

static inline float life_00370DC8(char* vdef)
{
    tParticleData* def = (tParticleData*)vdef;
    float t = def->Duration;
    if (t >= 0.0f) {
        return t;
    }
    return def->Life + def->LifeR * 0.5f;
}

static inline float start_00370DC8(char* vdef)
{
    tParticleData* def = (tParticleData*)vdef;
    float t = def->Duration;
    if (t < 0.0f) {
        return t;
    }
    return t + def->Life + def->LifeR * 0.5f;
}

/**
 * Configures a dynamic emitter from particle data: sub-emitter count, timing, ring size, the particle system and its
 * draw state.
 *
 * Copies the flip-texture count (NumFlipTextures to mNumFlipTextures) and rate (FlipTextureRate to
 * mFlipTextureRate); with two or more flip textures mCurrentFlipTexture starts at count * a BXrand fraction, else 0.
 * TextureId and BlendMode are copied. Duration and the emission life come from the data's Duration, Life and LifeR.
 * The sub-emitter count m_numSubEmitters is ceil(life * 60); when it changes the emitter rebuilds its buffers through
 * vtable slots 1 (purge(0)), 5 (Allocate) and 2 (reset) (On Tour: rebuild). Zeroes m_currPosition, m_currVelocity and
 * m_currColour, then tParticleEmitter_init on m_emitter with NumParticles particles per sub-emitter and
 * tParticleEmitter_setupEmitter with t4mat_Identity and m_boundBox. Sets m_initialized and copies
 * sDrawState_s_defaultDrawState to mDrawState with four bit fields overridden.
 *
 * @param def  the particle data (const tParticleData*)
 * @param seed    the random seed passed on to tParticleEmitter_setupEmitter (-1.0 from the effect modifiers)
 */
void cBaseClass_DynamicEmitter::setupEmitter(tParticleData* def, float seed)
{
    int n = def->NumFlipTextures;
    mNumFlipTextures = n;
    mFlipTextureRate = def->FlipTextureRate;
    if (n >= 2) {
        float fn = (float)n;
        mCurrentFlipTexture = fn * randf_00370DC8();
    } else {
        mCurrentFlipTexture = 0.0f;
    }
    TextureId = def->TextureId;
    BlendMode = def->BlendMode;
    Duration = start_00370DC8(((char*)def));
    int changed = 0;
    float frames = life_00370DC8(((char*)def)) * 60.0f;
    if (m_numSubEmitters != (int)fceil_00370DC8(frames)) {
        m_numSubEmitters = (int)fceil_00370DC8(frames);
        changed = 1;
    }
    m_currPosition = t4vect_Zero;
    m_currVelocity = t4vect_Zero;
    m_currColour = t4vect_Zero;
    float life = life_00370DC8(((char*)def));
    t4Vector force;
    force.x = def->ForceX;
    force.y = def->ForceY;
    force.z = def->ForceZ;
    force.w = 0.0f;
    tParticleEmitter_init(&m_emitter, def->NumParticles * m_numSubEmitters, def->NumBlur, &force, life,
                  def->BlurStep, def->Damp);
    tParticleEmitter_setupEmitter(&m_emitter, &t4mat_Identity, def, &m_boundBox, seed);
    if (changed) {
        tEmitterPurgeSlot* purgeSlot = (tEmitterPurgeSlot*)mVtbl;
        purgeSlot[1].fn(vthis(this, purgeSlot[1]), 0);
        tEmitterSlot* vt = (tEmitterSlot*)mVtbl;
        vt[5].fn(vthis(this, vt[5]));
        tEmitterSlot* vtReset = (tEmitterSlot*)mVtbl;
        vtReset[2].fn(vthis(this, vtReset[2]));
    }
    m_initialized = 1;
    mDrawState = sDrawState_s_defaultDrawState_0DC8;
    setDrawStateBits(mDrawState.m_bitGroups[1], 0x400000, 22, 1);
    setDrawStateBits(mDrawState.m_bitGroups[1], 0x300000, 20, 0);
    setDrawStateBits(mDrawState.m_bitGroups[1], 0x3, 0, 2);
    setDrawStateBits(mDrawState.m_bitGroups[2], 0x1FFFFC00, 10, 0);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_updateBASE);
#ifdef SKIP_ASM

static inline float randUnit10D0()
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

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
 * Callers: cDynamicColourEmitter_update, cDynamicParticleModifier_update and cAttachedEmitterFX_update.
 *
 * @param pos  the new position, or null to reuse m_currPosition
 * @param vel  the new velocity, or null to reuse m_currVelocity
 * @param rnd  non-zero when the sub-emitter is enabled this frame
 * @param dt  the frame time
 */
void cBaseClass_DynamicEmitter::updateBASE(t4Vector* pos, t4Vector* vel, int rnd, float dt)
{
    if (m_enabled == 0) {
        return;
    }
    int alive = m_velocities[m_subEmitterId].w > 0.0f;
    if (alive) {
        if (rnd == 0) {
            m_enabledCount = m_enabledCount - 1;
        }
    } else if (rnd) {
        m_enabledCount = m_enabledCount + 1;
    }
    if (pos) {
        m_currPosition = *pos;
    } else {
        pos = (t4Vector*)(&m_currPosition);
    }
    if (vel) {
        m_currVelocity = *vel;
    } else {
        vel = (t4Vector*)(&m_currVelocity);
    }
    m_positions[m_subEmitterId] = *pos;
    m_positions[m_subEmitterId].w = 0.0f;
    m_velocities[m_subEmitterId] = *vel;
    m_velocities[m_subEmitterId].w = rnd ? randUnit10D0() + 1.0f : 0.0f;
    int idx = m_subEmitterId - 1;
    m_subEmitterId = idx;
    if (idx < 0) {
        m_subEmitterId = m_numSubEmitters - 1;
    }
    float frameTime = mCurrentFlipTexture + mFlipTextureRate * dt;
    mCurrentFlipTexture = frameTime;
    if ((int)frameTime >= mNumFlipTextures) {
        mCurrentFlipTexture = 0.0f;
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
void cBaseClass_DynamicEmitter::setTime(float duration)
{
    if (duration == 0.0f) {
        tParticleEmitter_setTime(&m_emitter, 0.0f);
        return;
    }
    *(float*)this = duration;
    tParticleEmitter_setTime(&m_emitter, duration);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves a dynamic emitter to a replay frame: writes 0x14 bytes (Duration, the emitter's EndTime, m_enabled,
 * m_initialized, m_subEmitterId) with the stream's slot 1.
 *
 * @param stream  the replay frame to write to (On Tour data)
 */
void cBaseClass_DynamicEmitter::writeToReplayFrame(cBxReplayFrame* stream)
{
    struct {
        float duration;
        float endTime;
        int enabled;
        int initialized;
        int subEmitterId;
    } buf;
    buf.duration = Duration;
    buf.endTime = m_emitter.EndTime;
    buf.initialized = m_initialized;
    buf.enabled = m_enabled;
    buf.subEmitterId = m_subEmitterId;
    stream->write((const char*)&buf, 20);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores a dynamic emitter from a replay frame: reads the 0x14 bytes written by writeToReplayFrame (Duration,
 * EndTime, m_enabled, m_initialized, m_subEmitterId) with the stream's slot 2.
 *
 * @param stream  the replay frame to read from (On Tour data)
 */
void cBaseClass_DynamicEmitter::readFromReplayFrame(cBxReplayFrame* stream)
{
    struct {
        float duration;
        float endTime;
        int enabled;
        int initialized;
        int subEmitterId;
    } buf;
    stream->read((char*)&buf, 20);
    Duration = buf.duration;
    m_emitter.EndTime = buf.endTime;
    m_initialized = buf.initialized;
    m_enabled = buf.enabled;
    m_subEmitterId = buf.subEmitterId;
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicEmitter_render);
#ifdef SKIP_ASM



extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern cPS2GraphicsMan* cGraphicsMan_TheGraphicsMan_ctx __asm__("cGraphicsMan_TheGraphicsMan");
extern int gBlendTable[];


/**
 * Draws a dynamic emitter's particles with its current texture frame and draw state.
 *
 * Returns unless the words at +0x174 and +0x1E0 are set and the graphics manager's effect texture
 * _EffectTexMap[base (+0x4) + (int)frame (+0x10)] is valid. Sets bits 2-6 of the emitter's draw state
 * (+0x1E4) word 1 from its kind (+0x8) through gBlendTable {7, 5, 9, 0}, bits 5-9 of word 2 from @p blend, and
 * the texture id; pushes that state on the graphics manager's draw-state stack, draws through its virtual
 * slot 83 (cPS2GraphicsMan_RenderDynamicParticleEmitter) and pops the state.
 *
 * @param blend  value for bits 5-9 of the draw state's word 2 (On Tour: drawLayer, an eGMDrawLayer)
 */
void cDynamicEmitter::render(int blend)
{
    if (m_enabled == 0) {
        return;
    }
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan_ctx;
    if (m_enabledCount == 0) {
        return;
    }
    int tex = ctx->_EffectTexMap[TextureId + (int)mCurrentFlipTexture];
    if (tex < 0) {
        return;
    }
    mDrawState.m_bitGroups[1] = (unsigned int)(((int)mDrawState.m_bitGroups[1] & ~0x7C) | ((gBlendTable[BlendMode] << 2) & 0x7C));
    mDrawState.m_bitGroups[2] = (unsigned int)(((int)mDrawState.m_bitGroups[2] & ~0x3E0) | ((blend << 5) & 0x3E0));
    mDrawState.m_textureState.m_texid[0] = tex;
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
    *ctx->m_pCurrentDrawState = (*(&mDrawState));
    ctx->RenderDynamicParticleEmitter((this));
    ctx->m_pCurrentDrawState--;
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
    self->purge(0);
    cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(self, flags);
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_Allocate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_DynEmitterData_Colours[];


/**
 * Allocates a colour emitter's arrays: the base positions and velocities plus one RGBA colour each.
 *
 * The colours (4 bytes per sub-emitter, count at +0x178) go to +0x204, tagged "DynEmitterData Colours".
 */
void cDynamicColourEmitter::Allocate()
{
    tPackedRGBA** pc = (tPackedRGBA**)(&m_colours);
    ((cBaseClass_DynamicEmitter*)this)->Allocate();
    *pc = new (str_DynEmitterData_Colours, 0, 0) tPackedRGBA[m_numSubEmitters];
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
void cDynamicColourEmitter::purge(int resetEmitter)
{
    void* colours = m_colours;
    if (colours != 0) {
        cMemMan_free(colours);
        m_colours = 0;
    }
    ((cBaseClass_DynamicEmitter*)this)->purge(resetEmitter);
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_reset);
#ifdef SKIP_ASM

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
void cDynamicColourEmitter::reset()
{
    int i;
    ((cBaseClass_DynamicEmitter*)this)->reset();
    for (i = 0; i < m_numSubEmitters; i++) {
        if (D_004A599C == 0) {
            D_004A5998 = 0;
            D_004A5999 = 0;
            D_004A599A = 0;
            D_004A599B = 0;
            D_004A599C = 1;
        }
        m_colours[i] = *(tPackedRGBA*)&D_004A5998;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cDynamicColourEmitter_render);
#ifdef SKIP_ASM



extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern cPS2GraphicsMan* cGraphicsMan_TheGraphicsMan_ctx1688 __asm__("cGraphicsMan_TheGraphicsMan");
extern int gBlendTable[];


/**
 * Draws the colour emitter's sub-emitters.
 *
 * Returns when the emitter is disabled, has no enabled sub-emitters, or its texture (TextureId + mCurrentFlipTexture)
 * is not loaded. Otherwise it writes the blend mode (gBlendTable[BlendMode]) and @p blend (bits 5-9 of draw-state word
 * 2) into mDrawState, sets its texture, pushes it on the graphics manager's draw-state stack, calls GrMan slot 84 with
 * the emitter and pops the state.
 *
 * @param blend  the draw layer (On Tour drawLayer)
 */
void cDynamicColourEmitter::render(int blend)
{
    if (m_enabled == 0) {
        return;
    }
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan_ctx1688;
    if (m_enabledCount == 0) {
        return;
    }
    int tex = ctx->_EffectTexMap[TextureId + (int)mCurrentFlipTexture];
    if (tex < 0) {
        return;
    }
    mDrawState.m_bitGroups[1] = (unsigned int)(((int)mDrawState.m_bitGroups[1] & ~0x7C) | ((gBlendTable[BlendMode] << 2) & 0x7C));
    mDrawState.m_bitGroups[2] = (unsigned int)(((int)mDrawState.m_bitGroups[2] & ~0x3E0) | ((blend << 5) & 0x3E0));
    mDrawState.m_textureState.m_texid[0] = tex;
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
    *ctx->m_pCurrentDrawState = (*(&mDrawState));
    ctx->RenderDynamicParticleEmitter1((this));
    ctx->m_pCurrentDrawState--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cDynamicColourEmitter_update);
#ifdef SKIP_ASM



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
void cDynamicColourEmitter::update(int position, int velocity, t4Vector* col, int enable)
{
    if (col != 0) {
        *(&m_currColour) = *col;
    } else {
        col = (&m_currColour);
    }
    m_colours[m_subEmitterId].a = (int)clamp17C0(col->w * 255.0f, 0.0f, 255.0f);
    m_colours[m_subEmitterId].r = (int)clamp17C0(col->x * 255.0f, 0.0f, 255.0f);
    m_colours[m_subEmitterId].g = (int)clamp17C0(col->y * 255.0f, 0.0f, 255.0f);
    m_colours[m_subEmitterId].b = (int)clamp17C0(col->z * 255.0f, 0.0f, 255.0f);
    cBaseClass_DynamicEmitter_updateBASE(this, position, velocity, enable);
}
#endif

//100%
INCLUDE_ASM("render/particle", dma_waitGifSetIntermittent);
#ifdef SKIP_ASM
/**
 * Waits until the GIF DMA channel (D2, CHCR 0x1000A000) is idle, then writes 4 to 0x10003010.
 *
 * 0x10003010 is GIF_MODE (4 sets IMT, intermittent mode). No caller.
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel CHCR, 0x10003010 = GIF_MODE).
extern "C" void dma_waitGifSetIntermittent(void)
{
    while (*(volatile int*)0x1000A000 & 0x100) {
    }
    *(volatile int*)0x10003010 = 4;
}
#endif

//100%
INCLUDE_ASM("render/particle", dma_sendGifNormal);
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
 * transfer to finish. Callers: cPS2GraphicsMan_flipDisplayBuffers and cPS2LensFxMan_sampleZBuffer.
 *
 * @param madr  the source address
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel, VIF1 FIFO, GIF_MODE).
extern "C" void dma_sendGifNormal(unsigned int madr, int qwc, int flags)
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
INCLUDE_ASM("render/particle", dma_sendVif0Normal);
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
extern "C" void dma_sendVif0Normal(unsigned int madr, int qwc, int flags)
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
INCLUDE_ASM("render/particle", dma_sendVif1Normal);
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
extern "C" void dma_sendVif1Normal(unsigned int madr, int qwc, int flags)
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
INCLUDE_ASM("render/particle", dma_receiveVif1Normal);
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
extern "C" void dma_receiveVif1Normal(unsigned int madr, int qwc, int flags)
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
INCLUDE_ASM("render/particle", dma_copyToScratchpad);
#ifdef SKIP_ASM
/**
 * Copies @p qwc quadwords from main memory @p madr to scratchpad offset @p sadr by toSPR DMA (D9, 0x1000D400).
 *
 * SADR is @p sadr & 0x3FFF. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer
 * to finish. Callers: cPS2ScratchPad_claimAndLoadBuffer and scratchpad_loadSpareArea.
 *
 * @param madr  the main-memory source
 * @param sadr  the scratchpad destination
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
extern "C" void dma_copyToScratchpad(unsigned int madr, int sadr, int qwc, int flags)
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
INCLUDE_ASM("render/particle", dma_copyFromScratchpad);
#ifdef SKIP_ASM
/**
 * Copies @p qwc quadwords from scratchpad offset @p sadr to main memory @p madr by fromSPR DMA (D8, 0x1000D000).
 *
 * SADR is @p sadr & 0x3FFF. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer
 * to finish. Callers: cPS2ScratchPad_flushBuffer and scratchpad_storeSpareArea.
 *
 * @param sadr  the scratchpad source
 * @param madr  the main-memory destination
 * @param qwc  the quadword count
 * @param flags  4: wait before; 2: wait after
 */
extern "C" void dma_copyFromScratchpad(int sadr, unsigned int madr, int qwc, int flags)
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

extern "C" void* cSimpleTube_updateColours(cSimpleTube* self);

//100%
INCLUDE_ASM("render/particle", dma_sendVif1Chain);
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
extern "C" void dma_sendVif1Chain(unsigned int tadr, int flags)
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
INCLUDE_ASM("render/particle", dma_sendGifChain);
#ifdef SKIP_ASM
/**
 * Starts a chain-mode GIF DMA (D2) from the tag at @p tadr after setting GIF_MODE to 4.
 *
 * An address above 0x6FFFFFFF (the scratchpad) is turned into (address & 0x3FF0) | 0x80000000, the DMA's scratchpad
 * form. CHCR is 0x144 (chain, tag transfer) when @p flags & 1 is set, else 0x104; unlike dma_sendVif1Chain it leaves QWC
 * alone. @p flags & 4 waits for the channel to be idle first and @p flags & 2 waits for the transfer to finish. Caller:
 * gs_uploadImage.
 *
 * @param tadr  the first DMA tag
 * @param flags  1: transfer tags; 4: wait before; 2: wait after
 */
// PORT: PS2 hardware registers (D2 = GIF DMA channel, chain mode via TADR; GIF_MODE).
extern "C" void dma_sendGifChain(unsigned int tadr, int flags)
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
INCLUDE_ASM("render/particle", dma_setControllerEnable);
#ifdef SKIP_ASM
/**
 * Turns DMA on or off: sets or clears bit 0 of D_CTRL (0x1000E000).
 *
 * When enabling it also reads D_PCR (0x1000E020) and writes 1 to it. No caller.
 *
 * @param enable  non-zero to enable
 */
// PORT: PS2 hardware registers (0x1000E000 = D_CTRL, 0x1000E020 = D_PCR).
extern "C" void dma_setControllerEnable(int enable)
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
INCLUDE_ASM("render/particle", dma_rebaseChainRefs);
#ifdef SKIP_ASM
/**
 * Walks a DMA chain and rebases every ref tag's address onto @p chain, stopping after the ret tag.
 *
 * Tag ids: kDMATag_CNT (1) and kDMATag_RET (6) carry QWC quadwords of data, kDMATag_REF (3) points elsewhere. Likely
 * bug: ids 0, 2, 4, 5 and 7 (eDMATagID) do not advance the walk, so a chain containing one never ends. No caller.
 *
 * @param chain  the first tag of the chain
 * @return the address just past the ret tag's data
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Walks a DMA chain, rebasing REF tag addresses onto the chain start; returns the address past the RET tag.
extern "C" unsigned long* dma_rebaseChainRefs(unsigned long* chain)
{
    unsigned long* tagPtr = chain;
    int done = 0;

    do {
        unsigned long tag = *tagPtr;
        int id = (int)(tag >> 28) & 7;
        int addr = (long)tag >> 32;
        int qwc = (int)tag & 0xFFFF;

        switch (id) {
        case kDMATag_REFE:
        case kDMATag_NEXT:
        case kDMATag_REFS:
        case kDMATag_CALL:
        case kDMATag_END:
            break;
        case kDMATag_CNT:
            tagPtr += 2;
            tagPtr += qwc * 2;
            break;
        case kDMATag_REF:
            *tagPtr = ((unsigned long)(unsigned int)((char*)chain + addr) << 32) | (*tagPtr & 0xFFFFFFFF);
            tagPtr += 2;
            break;
        case kDMATag_RET:
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
INCLUDE_ASM("render/particle", dma_flattenChain);
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
 * low word, gets the copied quadword count as QWC, and loses its address word. Tag ids: kDMATag_CNT (1) and kDMATag_RET
 * (6) carry QWC quadwords of data, kDMATag_REF (3) points elsewhere. Likely bug: ids 0, 2, 4, 5 and 7 (eDMATagID) do
 * not advance the walk, so a chain containing one never ends. A ref tag whose address matches none of the three leaves
 * the walk stuck the same way. No caller.
 *
 * @param buf  the chain, rewritten in place
 * @param addrA  a ref address to blank
 * @param addrB  a ref address to blank
 * @param addrC  the ref address to replace with the fixed quadword
 */
extern "C" void dma_flattenChain(char* buf, int addrA, int addrB, int addrC)
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
        case kDMATag_REFE:
        case kDMATag_NEXT:
        case kDMATag_REFS:
        case kDMATag_CALL:
        case kDMATag_END:
            break;
        case kDMATag_CNT: {
            int size = qwc << 4;
            src += 0x10;
            MEM_copy(dst, src, size);
            src += size;
            dst += size;
            count += qwc;
            break;
        }
        case kDMATag_REF:
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
        case kDMATag_RET:
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
INCLUDE_ASM("render/particle", dma_packChainCntBlocks);
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
 * closed if non-empty and the ret tag with its data is copied. Tag ids: kDMATag_CNT (1) and kDMATag_RET (6) carry QWC
 * quadwords of data, kDMATag_REF (3) points elsewhere. Likely bug: ids 0, 2, 4, 5 and 7 (eDMATagID) do not advance the
 * walk, so a chain containing one never ends. A ref tag whose address matches none of the three leaves the walk stuck
 * the same way. No caller.
 *
 * @param buf  the chain, rewritten in place
 * @param addrA  a ref address to blank
 * @param addrB  a ref address to blank
 * @param addrC  the ref address kept as a block boundary
 */
extern "C" void dma_packChainCntBlocks(char* buf, int addrA, int addrB, int addrC)
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
        case kDMATag_REFE:
        case kDMATag_NEXT:
        case kDMATag_REFS:
        case kDMATag_CALL:
        case kDMATag_END:
            break;
        case kDMATag_CNT: {
            int size = qwc << 4;
            src += 0x10;
            MEM_copy(dst, src, size);
            src += size;
            dst += size;
            count += qwc;
            break;
        }
        case kDMATag_REF:
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
        case kDMATag_RET:
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
    return ((cSimpleTube*)self)->updateColours();
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_cSimpleTube);
#ifdef SKIP_ASM
extern int cSimpleTube_m_initialized;
extern int cTube_m_R;
extern int cTube_m_G;
extern int cTube_m_B;
extern void* cSimpleTube_vtbl[];

extern tBxVertex cSimpleTube_m_vertices[];


/**
 * Constructs a segmented tube effect with @p owner segments and, the first time, fills the shared tube vertex array.
 *
 * Stores @p owner at +0x4, the tube vtable cSimpleTube_vtbl and a null point array at +0x8. While the flag cSimpleTube_m_initialized is
 * clear it sets up all 262 shared vertices of cSimpleTube_m_vertices: alpha 0xFF, red/green/blue from the tube colour cTube_m_R,
 * cTube_m_G and cTube_m_B, position x 0, z 1 and w 0, and y alternating 0 (even) and 1 (odd); then sets the flag.
 * cFlexBridgeNode's constructors build one (0xC bytes, "Tube") with NumNodes * 8 - 9 segments.
 *
 * @param owner  the number of segments (misnamed: an int count, not an owner pointer)
 * @return @p self
 */
extern "C" void* cSimpleTube_cSimpleTube(cSimpleTube* self, void* owner)
{
    int i;
    self->m_numSegments = (int)owner;
    self->mVtbl = cSimpleTube_vtbl;
    self->m_positions = 0;
    if (cSimpleTube_m_initialized == 0) {
        tBxVertex* vertex = cSimpleTube_m_vertices;
        for (i = 0; i < 262; i++, vertex++) {
            vertex->a = 0xFF;
            vertex->r = cTube_m_R;
            vertex->g = cTube_m_G;
            vertex->b = cTube_m_B;
            vertex->one = 1.0f;
            (*(int*)&vertex->pad) = 0;
            vertex->u = 0.0f;
            if (i & 1) {
                vertex->v = 1.0f;
            } else {
                vertex->v = 0.0f;
            }
        }
        cSimpleTube_m_initialized = 1;
    }
    return (self);
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_updateColours);
#ifdef SKIP_ASM
extern float cSimpleTube_updateColours_r;
extern float cSimpleTube_updateColours_g;
extern float cSimpleTube_updateColours_b;

/**
 * Refreshes the colour of the 262 shared tube vertices (cSimpleTube_m_vertices) when the tube colour has changed.
 *
 * Compares the cached floats cSimpleTube_updateColours_r, cSimpleTube_updateColours_g and cSimpleTube_updateColours_b with the tube colour ints cTube_m_R, cTube_m_G and
 * cTube_m_B; if any differs, it writes the three ints into every vertex's r, g and b and caches the new values as
 * floats. @p self is not used. Called by cTube_update.
 *
 * @return nothing meaningful: declared as returning void* but falls off the end without a return value
 */
void* cSimpleTube::updateColours()
{
    if (cSimpleTube_updateColours_r != (float)cTube_m_R || cSimpleTube_updateColours_g != (float)cTube_m_G || cSimpleTube_updateColours_b != (float)cTube_m_B) {
        int i;
        for (i = 0; i < 262; i++) {
            tBxVertex* vertex = &cSimpleTube_m_vertices[i];
            vertex->r = cTube_m_R;
            vertex->g = cTube_m_G;
            vertex->b = cTube_m_B;
        }
        cSimpleTube_updateColours_r = (float)cTube_m_R;
        cSimpleTube_updateColours_g = (float)cTube_m_G;
        cSimpleTube_updateColours_b = (float)cTube_m_B;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube__cSimpleTube);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern void* cSimpleTube_vtbl[];
extern void* cVisualEffect_vtbl[];

/**
 * Destroys a segmented tube effect: frees its point array and the object itself when asked.
 *
 * Re-installs the tube vtable cSimpleTube_vtbl, frees the array at +0x8 with cMemMan_free when it is set, installs the base
 * visual-effect vtable cVisualEffect_vtbl and calls operator_delete when bit 0 of @p flags is set. Slot 1 of cSimpleTube_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSimpleTube__cSimpleTube(cSimpleTube* self, int flags)
{
    *(void***)self = cSimpleTube_vtbl;
    if (self->m_positions != 0) {
        cMemMan_free(self->m_positions);
    }
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cSimpleTube_render);
#ifdef SKIP_ASM

extern int cTube_m_Enable;
extern float cTube_m_ScaleWidth;
extern float cTube_m_ScaleHeight;
extern t4Vector t4vect_ZAxis;
extern sDrawState sDrawState_s_defaultDrawState_rs26C8 __asm__("sDrawState_s_defaultDrawState");
extern cPS2GraphicsMan* cGraphicsMan_TheGraphicsMan_ctx26C8 __asm__("cGraphicsMan_TheGraphicsMan");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_26C8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline void SubEq_26C8(t4Vector& a, const t4Vector& b)
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
static inline t4Vector Add_26C8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector Norm_26C8(const t4Vector& v)
{
    t4Vector r;
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
static inline t4Vector Cross_26C8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector Scale_26C8(const t4Vector& v, float s)
{
    t4Vector r;
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
 * Does nothing while the tube enable flag cTube_m_Enable is 0. The direction is the normalised vector from point 0 to point
 * n (n = m_numSegments, the points m_positions); the side vector is the normalised cross product of it with
 * t4vect_ZAxis scaled by the width cTube_m_ScaleWidth, and both it and its negation are lowered along Z by the height
 * cTube_m_ScaleHeight. For each of the n + 1 points it fills four of the shared vertices cSimpleTube_m_vertices: point + negated side and
 * the point for the first strip, the point and point + side for the second; two extra vertices copy their neighbours so
 * the two strips join in one degenerate tri strip.
 *
 * Pushes a copy of the current draw state and loads sDrawState_s_defaultDrawState, draws 4 * (n + 1) + 2 vertices
 * through cGraphicsMan_TheGraphicsMan's virtual slot 71 (cPS2GraphicsMan_RenderTriStrip), then copies the points into
 * the first n + 1 vertices and draws them through slot 73 (cPS2GraphicsMan_RenderLineStrip, a line strip), and pops the draw state. Slot
 * 3 (render) of cSimpleTube_vtbl.
 */
void cSimpleTube::render()
{
    if (cTube_m_Enable == 0) {
        return;
    }
    t4Vector dir = Norm_26C8(Sub_26C8(m_positions[m_numSegments], m_positions[0]));
    t4Vector sideOffset = Scale_26C8(Norm_26C8(Cross_26C8(dir, t4vect_ZAxis)), cTube_m_ScaleWidth);
    t4Vector otherSideOffset = Scale_26C8(sideOffset, -1.0f);
    SubEq_26C8(sideOffset, Scale_26C8(t4vect_ZAxis, cTube_m_ScaleHeight));
    SubEq_26C8(otherSideOffset, Scale_26C8(t4vect_ZAxis, cTube_m_ScaleHeight));
    for (int i = 0; i < m_numSegments + 1; i++) {
        cSimpleTube_m_vertices[2 * i].p = Add_26C8(m_positions[i], otherSideOffset);
        cSimpleTube_m_vertices[2 * i + 1].p = m_positions[i];
        cSimpleTube_m_vertices[2 * (i + m_numSegments + 2)].p = m_positions[i];
        cSimpleTube_m_vertices[((m_numSegments + i) << 1) + 5].p = Add_26C8(m_positions[i], sideOffset);
    }
    cSimpleTube_m_vertices[2 * m_numSegments + 2] = cSimpleTube_m_vertices[2 * m_numSegments + 1];
    cSimpleTube_m_vertices[2 * m_numSegments + 3] = cSimpleTube_m_vertices[2 * m_numSegments + 4];
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan_ctx26C8;
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
    *ctx->m_pCurrentDrawState = sDrawState_s_defaultDrawState_rs26C8;
    ctx->RenderTriStrip(((m_numSegments + 1) << 2) | 2, cSimpleTube_m_vertices, 0);
    for (int i = 0; i < m_numSegments + 1; i++) {
        cSimpleTube_m_vertices[i].p = m_positions[i];
    }
    ctx->RenderLineStrip(m_numSegments + 1, cSimpleTube_m_vertices, 0);
    ctx->m_pCurrentDrawState--;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPathArrow_cPathArrow);
#ifdef SKIP_ASM
extern int cPathArrow_m_initialized;
extern int cPathArrow_m_R;
extern int cPathArrow_m_G;
extern int cPathArrow_m_B;
extern int cPathArrow_m_A;
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
extern sPtVert_2AA0 cPathArrow_m_vertices[];

struct sPtObj_2AA0 {
    void** vt;      // 0x0
};

/**
 * Constructs the path-arrow visual effect and, the first time, fills its 8 shared vertices.
 *
 * Stores the vtable cPathArrow_vtbl. While the flag cPathArrow_m_initialized is clear it sets each of the 8 vertices of cPathArrow_m_vertices to
 * red/green/blue/alpha cPathArrow_m_R, cPathArrow_m_G, cPathArrow_m_B and cPathArrow_m_A, z 1 and x, y, w 0, then sets the flag.
 * cPS2VisualEffectsMan_cPS2VisualEffectsMan allocates it (4 bytes, "PathArrow") as its object 16.
 *
 * @return @p self
 */
extern "C" void* cPathArrow_cPathArrow(sPtObj_2AA0* self)
{
    int i;
    self->vt = cPathArrow_vtbl;
    if (cPathArrow_m_initialized == 0) {
        for (i = 0; i < 8; i++) {
            cPathArrow_m_vertices[i].a = cPathArrow_m_A;
            cPathArrow_m_vertices[i].r = cPathArrow_m_R;
            cPathArrow_m_vertices[i].g = cPathArrow_m_G;
            cPathArrow_m_vertices[i].b = cPathArrow_m_B;
            cPathArrow_m_vertices[i].z = 1.0f;
            cPathArrow_m_vertices[i].w = 0;
            cPathArrow_m_vertices[i].x = 0.0f;
            cPathArrow_m_vertices[i].y = 0.0f;
        }
        cPathArrow_m_initialized = 1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", compareIntsAscending);
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
extern "C" int compareIntsAscending(int *lhs, int *rhs) {
    return *lhs - *rhs;
}
#endif

//100%
INCLUDE_ASM("render/particle", __12cBezierCache);
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
 * m6x6CacheRecord, m8x8CacheRecord and check they are NULL before cBezierCache_allocateCaches allocates them). The six placement-new
 * array constructions (4, 6 and 8 t4Vectors, twice: the basis and derivative-basis tables) compile to empty loops;
 * their offsets in the C (+0x20 to +0x1E0) do not match where cBezierCache_allocateCaches keeps the tables (+0x70, +0x1F0), which is
 * harmless since they emit no stores. cPS2BezierMan_cPS2BezierMan calls it.
 */
cBezierCache::cBezierCache()
{
    new ((char*)&mCacheRecord[2]) sPtElem2B30[4];
    new ((char*)&mLMUVCache[0]) sPtElem2B30[6];
    new ((char*)&m6x6Basis[1]) sPtElem2B30[8];
    new ((char*)&m8x8Basis[3]) sPtElem2B30[4];
    new ((char*)&m8x8Basis[7]) sPtElem2B30[6];
    new ((char*)&mFreeNrmListSP[2]) sPtElem2B30[8];
    m4x4CacheRecord = 0;
    m6x6CacheRecord = 0;
    m8x8CacheRecord = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierCache_allocateCaches);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern "C" void MEM_fill(void* dst, int c, int n);
// cBezierCache (render/beziercache.cpp): per patch order (4x4, 6x6, 8x8 control points) a record cache with a free
// list, point / uv grids, a normal cache with its own free list, and the cubic basis tables.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

// One cache record (12 bytes; cBezierCache_cachePatch's sPEnt_47A0 names its words from use, names ours).
struct sBzRec_2B78 {
    int patch;                  // [U] the cached patch record (tPatchRecord*)
    int flags;                  // [U] in-use / view bits (cBezierCache_cachePatch ORs 0xC | 1 << view)
    int nrmSlot;                // [U] the normal-cache index; -1 = none
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
    sBzUV4_2B78* m4x4UVCache;       // 0x054 [R] "4x4UVCache" (cBezierCache::mUVCache[0])
    sBzUV6_2B78* m6x6UVCache;       // 0x058 [R] "6x6UVCache"
    sBzUV8_2B78* m8x8UVCache;       // 0x05C [R] "8x8UVCache"
    sBzUV4_2B78* m4x4LMUVCache;     // 0x060 [R] "4x4LMUVCache" (cBezierCache::mLMUVCache[0])
    sBzUV6_2B78* m6x6LMUVCache;     // 0x064 [R] "6x6LMUVCache"
    sBzUV8_2B78* m8x8LMUVCache;     // 0x068 [R] "8x8LMUVCache"
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
extern "C" void cBezierCache_allocateCaches(sBzCache_2B78* self, int count4x4, int count6x6, int count8x8, int nn4, int nn6, int nn8)
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
        sBzUV4_2B78*& cacheSlot = self->m4x4UVCache;
        cacheSlot = new (str_4x4UVCache, 0, 0) sBzUV4_2B78[count4x4];
    }
    {
        sBzUV6_2B78*& cacheSlot = self->m6x6UVCache;
        cacheSlot = new (str_6x6UVCache, 0, 0) sBzUV6_2B78[count6x6];
    }
    {
        sBzUV8_2B78*& cacheSlot = self->m8x8UVCache;
        cacheSlot = new (str_8x8UVCache, 0, 0) sBzUV8_2B78[count8x8];
    }
    {
        sBzUV4_2B78*& cacheSlot = self->m4x4LMUVCache;
        cacheSlot = new (str_4x4LMUVCache, 0, 0) sBzUV4_2B78[count4x4];
    }
    {
        sBzUV6_2B78*& cacheSlot = self->m6x6LMUVCache;
        cacheSlot = new (str_6x6LMUVCache, 0, 0) sBzUV6_2B78[count6x6];
    }
    {
        sBzUV8_2B78*& cacheSlot = self->m8x8LMUVCache;
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
            self->records[k][j].nrmSlot = -1;
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
INCLUDE_ASM("render/particle", cBezierCache_freeCaches);
#ifdef SKIP_ASM
struct sPtBufs_37D0 {
    void* p[3];
};

struct sPtSys_37D0 {
    char pad0[0xC];
    sPtBufs_37D0 cacheRecord;      // 0x0C [R] the "4x4CacheRecord" .. 8x8 tables
    char pad18[0xC];
    sPtBufs_37D0 freeList;         // 0x24 [R] "4x4FreeList" .. 8x8
    char pad30[0x18];
    sPtBufs_37D0 posCache;         // 0x48 [R] "4x4PosCache" .. 8x8
    sPtBufs_37D0 uvCache;          // 0x54 [R] "4x4UVCache" .. 8x8
    sPtBufs_37D0 lmuvCache;        // 0x60 [R] "4x4LMUVCache" .. 8x8
    char pad6C[0x148];
    sPtBufs_37D0 nrmCache;         // 0x1B4 [R] "4x4NrmCache" .. 8x8
    sPtBufs_37D0 freeNrmList;      // 0x1C0 [R] "4x4FreeNrmList" .. 8x8
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
 * Covers the seven three-pointer sets that cBezierCache_allocateCaches allocates: the records (+0xC), free lists (+0x24), position
 * grids (+0x48), UV grids (+0x54), lightmap UV grids (+0x60), normal grids (+0x1B4) and normal free lists (+0x1C0);
 * each non-null pointer goes to cMemMan_free and is then set to 0. No caller in retail references it.
 */
extern "C" void cBezierCache_freeCaches(sPtSys_37D0* self)
{
    PT_FREE_BUFS_37D0(self->cacheRecord);
    PT_FREE_BUFS_37D0(self->freeList);
    PT_FREE_BUFS_37D0(self->posCache);
    PT_FREE_BUFS_37D0(self->uvCache);
    PT_FREE_BUFS_37D0(self->lmuvCache);
    PT_FREE_BUFS_37D0(self->nrmCache);
    PT_FREE_BUFS_37D0(self->freeNrmList);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierCache_clearRecords);
#ifdef SKIP_ASM
// One cache record (as cBezierCache_allocateCaches's sBzRec_2B78; names ours).
struct sPtSlot39D0 {
    int patch;                      // [U] the cached patch record
    int flags;                      // [U] in-use / view bits
    int nrmSlot;                    // [U] the normal-cache index; -1 = none
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
extern "C" void cBezierCache_clearRecords(sPtPools39D0* self)
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
            self->slots[k][j].nrmSlot = -1;
            self->slots[k][j].flags = 0;
            self->slots[k][j].patch = 0;
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
INCLUDE_ASM("render/particle", blendTransformPointXYZ);
#ifdef SKIP_ASM


static inline float ptBlend_3C28(t3Vector* p, t4Matrix* mt, float* w, int j)
{
    float x = p->x;
    float y = p->y;
    float z = p->z;
    return (((float (*)[4])&mt[0].row[0])[0][j] * x + ((float (*)[4])&mt[0].row[0])[1][j] * y + ((float (*)[4])&mt[0].row[0])[2][j] * z + ((float (*)[4])&mt[0].row[0])[3][j]) * w[0]
         + (((float (*)[4])&mt[1].row[0])[0][j] * x + ((float (*)[4])&mt[1].row[0])[1][j] * y + ((float (*)[4])&mt[1].row[0])[2][j] * z + ((float (*)[4])&mt[1].row[0])[3][j]) * w[1]
         + (((float (*)[4])&mt[2].row[0])[0][j] * x + ((float (*)[4])&mt[2].row[0])[1][j] * y + ((float (*)[4])&mt[2].row[0])[2][j] * z + ((float (*)[4])&mt[2].row[0])[3][j]) * w[2]
         + (((float (*)[4])&mt[3].row[0])[0][j] * x + ((float (*)[4])&mt[3].row[0])[1][j] * y + ((float (*)[4])&mt[3].row[0])[2][j] * z + ((float (*)[4])&mt[3].row[0])[3][j]);
}

/**
 * Transforms the 3D point @p points by four matrices and blends the results with weights into @p out (x, y, z).
 *
 * Each output component j is sum over k of (row 0 * x + row 1 * y + row 2 * z + row 3)[j] of matrix k, with matrices 0
 * to 2 weighted by w[0] to w[2] and matrix 3 added unweighted (likely bug, or the caller is meant to fold w[3] in; the
 * four-weight version blendTransformPointXYZW weights all four). @p self is not used. No caller in retail references it.
 *
 * @param points  the point (x, y, z)
 * @param mt  four consecutive 4x4 matrices (rows 0-2 scale x, y, z; row 3 is the translation)
 * @param w  the blend weights (only w[0] to w[2] are read)
 * @param out  receives x, y, z
 */
extern "C" void blendTransformPointXYZ(void* self, t3Vector* points, t4Matrix* mt, float* w, float* out)
{
    out[0] = ptBlend_3C28(points, mt, w, 0);
    out[1] = ptBlend_3C28(points, mt, w, 1);
    out[2] = ptBlend_3C28(points, mt, w, 2);
}
#endif
//100%
INCLUDE_ASM("render/particle", blendTransformPointXYZW);
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
extern "C" void blendTransformPointXYZW(void* self, sPtVec4_3EA8* points, sPtMtx_3EA8* mt, float* w, float* out)
{
    out[0] = ptBlend4_3EA8(points, mt, w, 0);
    out[1] = ptBlend4_3EA8(points, mt, w, 1);
    out[2] = ptBlend4_3EA8(points, mt, w, 2);
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierCache_evalPatchPositions);
#ifdef SKIP_ASM


struct sPatchSet {
    char pad_0x00[0x190];
    t4Vector* basis[3];    // 0x190
    int count[3];           // 0x19C
};

/**
 * Evaluates a cubic Bezier patch on an order x order grid into a position cache entry (VU0 macro code).
 *
 * Loads the patch's 16 coefficient vectors (four 4x4 power-basis blocks, from +0x40 of @p mat) into VU0, and for each
 * row i and column j of the grid (order from +0x19C[@p idx], basis rows (t^3, t^2, t, 1) from +0x190[@p idx]) combines
 * them with basis[i] and basis[j]; writes x, y, z of each point (12 bytes) to @p out in row order. cBezierCache_cachePatch calls
 * it.
 *
 * @param mat  the patch record (tPatchRecord): coefficients m_fBm0..m_fBm3 (+0x40)
 * @param out  the order x order position grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
// PORT: PS2-only VU0 inline asm (tensor-product patch evaluation: out[i][j] =
// sum over basis[i] (x) basis[j] of the 4x4 control block at mat->m_fBm0).
extern "C" void cBezierCache_evalPatchPositions(sPatchSet* self, tPatchRecord* mat, t3Vector* out, int idx)
{
    int n = self->count[idx];
    t4Vector* basis = self->basis[idx];
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
        : "r"(mat->m_fBm0));
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
            t4Vector point;
            __asm__(
                "lqc2      $vf22, 0x0(%1)\n"
                "vmulax.xyzw ACC, $vf18, $vf22x\n"
                "vmadday.xyzw ACC, $vf19, $vf22y\n"
                "vmaddaz.xyzw ACC, $vf20, $vf22z\n"
                "vmaddw.xyzw $vf23, $vf21, $vf22w\n"
                "sqc2      $vf23, %0\n"
                : "=m"(point)
                : "r"(&basis[j]));
            *out++ = *(t3Vector*)&point;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierCache_fillPatchUVs);
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
 * The corners come from @p src's m_fUV0: [0][0..1] and [0][2..3] (first row's ends), [1][0..1] and [1][2..3] (last
 * row's ends). With order n from mPatchOrder[@p idx], each of the n rows steps its two ends by 1 / (n - 1) of the way
 * to the last row and writes n evenly spaced uv pairs from one end to the other. cBezierCache_cachePatch calls it.
 *
 * @param src  the patch record (tPatchRecord)
 * @param out  the n x n uv grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
extern "C" void cBezierCache_fillPatchUVs(cBezierCache* self, tPatchRecord* src, sV2_4298* out, int idx)
{
    int n = self->mPatchOrder[idx];
    sV2_4298 leftEdge = *(sV2_4298*)&src->m_fUV0[0][0];
    sV2_4298 rightEdge = *(sV2_4298*)&src->m_fUV0[0][2];
    sV2_4298 bottomLeft = *(sV2_4298*)&src->m_fUV0[1][0];
    sV2_4298 bottomRight = *(sV2_4298*)&src->m_fUV0[1][2];
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
INCLUDE_ASM("render/particle", cBezierCache_fillPatchLightmapUVs);
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
 * With order n from mPatchOrder[@p idx], the step is the rectangle's size (+0x18) / (n - 1); for each column x from the
 * origin (+0x10) it writes n points stepping y. cBezierCache_cachePatch calls it for the lightmap UV cache.
 *
 * @param rect  the patch record (tPatchRecord): lightmap origin at +0x10, size at +0x18
 * @param out  the n x n uv grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
extern "C" void cBezierCache_fillPatchLightmapUVs(cBezierCache* self, sPtRect* rect, sPtVec2* out, int idx)
{
    int i, j;
    int n = self->mPatchOrder[idx];
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
INCLUDE_ASM("render/particle", cBezierCache_computePatchNormals);
#ifdef SKIP_ASM

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


/**
 * Computes a patch's n x n grid of unit normals (VU0 macro code).
 *
 * Loads the 16 coefficient vectors of @p mat (m_fBm0 .. m_fBm3). A first pass evaluates the patch with the
 * derivative basis (3t^2, 2t, 1, 0; mDBasis[@p idx]) in one direction and the basis (mBasis[@p idx]) in the other,
 * storing that tangent in @p out; a second pass evaluates the other tangent and replaces each entry with the
 * normalised cross product of the two (left unnormalised when its length is 0). cBezierCache_cachePatch calls it for patches
 * that need normals.
 *
 * @param mat  the patch record: its coefficient rows m_fBm0 .. m_fBm3
 * @param out  the n x n normal grid to fill
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 */
// PORT: PS2-only VU0 inline asm (tensor-product patch evaluation, then normals
// as the normalised cross product of the two partial derivatives).
extern "C" void cBezierCache_computePatchNormals(cBezierCache* self, tPatchRecord* mat, sPatchOut4518* out, int idx)
{
    int n = self->mPatchOrder[idx];
    t4Vector* basis = self->mBasis[idx];
    t4Vector* dbasis = self->mDBasis[idx];
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
        : "r"(mat->m_fBm0));
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
            t4Vector point;
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
            t4Vector point;
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
INCLUDE_ASM("render/particle", cBezierCache_cachePatch);
#ifdef SKIP_ASM


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
    t3Vector (*pos0)[16];          // 0x48
    t3Vector (*pos1)[36];          // 0x4C
    t3Vector (*pos2)[64];          // 0x50
    t2Vector (*uv0)[16];           // 0x54
    t2Vector (*uv1)[36];           // 0x58
    t2Vector (*uv2)[64];           // 0x5C
    t2Vector (*col0)[16];          // 0x60
    t2Vector (*col1)[36];          // 0x64
    t2Vector (*col2)[64];          // 0x68
    char pad_0x6C[0x1B4 - 0x6C];
    t3Vector (*nrm0)[16];          // 0x1B4
    t3Vector (*nrm1)[36];          // 0x1B8
    t3Vector (*nrm2)[64];          // 0x1BC
    char pad_0x1C0[0xC];
    int* nfreeList[3];              // 0x1CC
    int nfreeCount[3];              // 0x1D8
};

// cBezierCache_computePatchNormals is declared through a local view so the unit's own definition keeps its types
extern "C" void cBezierCache_computePatchNormals_47A0(sPMgr_47A0* self, char* mat, t3Vector* out, int idx) __asm__("cBezierCache_computePatchNormals");

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

static inline t3Vector* PosOf_47A0(sPMgr_47A0* self, int idx, int slot)
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

static inline t2Vector* UVOf_47A0(sPMgr_47A0* self, int idx, int slot)
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

static inline t2Vector* ColOf_47A0(sPMgr_47A0* self, int idx, int slot)
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

static inline t3Vector* NrmOf_47A0(sPMgr_47A0* self, int idx, int slot)
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
 * Pops a record index from the free list of order @p idx (-1 when empty), evaluates the positions (cBezierCache_evalPatchPositions), UVs
 * (cBezierCache_fillPatchUVs) and lightmap UVs (cBezierCache_fillPatchLightmapUVs) into that entry of each cache, stores @p mat in the record, the
 * index in the patch's halfword +0x1A6[@p idx], and ORs 0xC | (1 << @p bit) into the record's flags (marked in use for
 * two ageing passes of cBezierCache_ageRecords). With @p normals it also pops a normal-grid index, fills it with cBezierCache_computePatchNormals
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
extern "C" int cBezierCache_cachePatch(sPMgr_47A0* self, tPatchRecord* mat, int idx, int bit, int normals)
{
    int slot = PopSlot_47A0(self, idx);
    t3Vector* pos = PosOf_47A0(self, idx, slot);
    t2Vector* uv = UVOf_47A0(self, idx, slot);
    t2Vector* col = ColOf_47A0(self, idx, slot);
    cBezierCache_evalPatchPositions((sPatchSet*)self, (tPatchRecord*)((char*)mat), (t3Vector*)pos, idx);
    cBezierCache_fillPatchUVs((cBezierCache*)self, (tPatchRecord*)((char*)mat), (sV2_4298*)uv, idx);
    cBezierCache_fillPatchLightmapUVs((cBezierCache*)self, (sPtRect*)mat, (sPtVec2*)col, idx);
    sPEnt_47A0* entry = &self->entries[idx][slot];
    entry->mat = ((char*)mat);
    mat->m_sCacheRecord[idx] = slot;
    entry->flags |= 0xC | (1 << bit);
    if (normals) {
        int nslot = self->nfreeList[idx][--self->nfreeCount[idx]];
        t3Vector* nrm = NrmOf_47A0(self, idx, nslot);
        cBezierCache_computePatchNormals_47A0(self, ((char*)mat), nrm, idx);
        entry->nslot = nslot;
    }
    return slot;
}
#endif

//100%
INCLUDE_ASM("render/particle", cBezierCache_markPatchesUsed);
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
 * calls it for the 4x4, 6x6 and 8x8 lists; cBezierCache_markMixedPatchesUsed is the same for the 20-byte mixed-edge list.
 *
 * @param count  number of queued patches
 * @param items  the queued patches (tBezierPatchEntry, 16 bytes each)
 * @param idx  the patch order: 0 = 4x4, 1 = 6x6, 2 = 8x8
 * @param bit  the view index
 */
extern "C" void cBezierCache_markPatchesUsed(sPartOwner* self, int count, sPartItem* items, int idx, int bit)
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
INCLUDE_ASM("render/particle", cBezierCache_markMixedPatchesUsed);
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
 * The same loop as cBezierCache_markPatchesUsed over 20-byte entries (tBezierMixedPatchEntry): for each one with a slot (+0xC) of 0 or
 * more, ORs 0xC | (1 << @p bit) into that record's flags in the record array +0x18[@p idx]. cPS2BezierMan_DrawPatches
 * calls it with order 2 (8x8).
 *
 * @param count  number of mixed patches
 * @param items  the mixed patches (tBezierMixedPatchEntry, 20 bytes each)
 * @param idx  the patch order (2 = 8x8)
 * @param bit  the view index
 */
extern "C" void cBezierCache_markMixedPatchesUsed(sPartOwner* self, int count, sPartItem14* items, int idx, int bit)
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
INCLUDE_ASM("render/particle", cBezierCache_ageRecords);
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
extern "C" void cBezierCache_ageRecords(sPartLists* self)
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
void cPlayerShadow::InitBones(int head, int lHand, int rHand, int lFoot, int rFoot, int hips)
{
    ShadowBone[0] = head;
    ShadowBone[1] = lHand;
    ShadowBone[2] = rHand;
    ShadowBone[3] = lFoot;
    ShadowBone[4] = rFoot;
    ShadowBone[5] = hips;
}
#endif

INCLUDE_ASM("render/particle", func_00374D00);

//100%
INCLUDE_ASM("render/particle", cPlayerShadow_AddShadow);
#ifdef SKIP_ASM
// A t4Vector with constructors (ours has none), for the by-value temporaries and VU0 helpers below
// (On Tour t4Vector(), t4Vector(float, float, float, float)).
struct t4VectorInit : t4Vector {
    t4VectorInit() {}
    t4VectorInit(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

/**
 * Registers a rider's shadow with the renderer as a projective texture.
 *
 * Calls cGraphicsMan_TheGraphicsMan's virtual slot 67 (cPS2GraphicsMan_AddProjectiveTexture) with the position
 * (mProjPos, w = 1), the matrices mCProjMat and mUVProjMat, intensity 1.0 and the texture id mLockTexture.
 * cRider_RenderShadow calls it.
 */
void cPlayerShadow::AddShadow()
{
    cGraphicsMan_TheGraphicsMan->AddProjectiveTexture(
        t4VectorInit(mProjPos[0], mProjPos[1], mProjPos[2], 1.0f), mCProjMat, mUVProjMat,
        1.0f, mLockTexture);
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
void* cPlayerShadow_cPlayerShadow(void* vself)
{
    cPlayerShadow* self = (cPlayerShadow*)vself;
    self->mLockTexture = -1;
    self->mShadowTexWidth = 0x80;
    self->mShadowTexHeight = 0x80;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("render/particle", _$_13cPlayerShadow);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void cTextureMan_unlockTexture(void* mgr, int id);
void operator_delete(int* ptr);


/**
 * Destroys a rider's shadow: releases its locked texture and frees it if asked.
 *
 * When mLockTexture is a valid id, frees it in cGraphicsMan_TheGraphicsMan's texture manager (mTexMan,
 * cTextureMan_unlockTexture), removes it through the graphics manager's vtable slot 50 (cPS2GraphicsMan_RemoveTexture) and resets
 * it to -1. Then operator_delete on g++'s delete flag bit 0 (cRider__cRider passes 3).
 */
cPlayerShadow::~cPlayerShadow()
{
    int tex = mLockTexture;
    if (tex >= 0) {
        cTextureMan_unlockTexture(cGraphicsMan_TheGraphicsMan->mTexMan, tex);
        cPS2GraphicsMan* mgr = cGraphicsMan_TheGraphicsMan;
        mgr->RemoveTexture(mLockTexture);
        mLockTexture = -1;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPlayerShadow_LockShadowTexture);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_particle __asm__("cGraphicsMan_TheGraphicsMan");


/**
 * Creates the render target texture for a rider's shadow if it has none yet.
 *
 * When the texture id at +0x170 (On Tour's cPlayerShadow mLockTexture) is negative, it asks
 * cGraphicsMan_TheGraphicsMan's virtual slot 52 (cPS2GraphicsMan_AddLockedFrameTexture__FPv; On Tour
 * AddLockedFrameTexture(width, height, texeltype)) for a texture of the size at +0x174 and +0x178 (mShadowTexWidth,
 * mShadowTexHeight) in texel format 2 and stores the id. cRider_loadRider calls it.
 */
void cPlayerShadow::LockShadowTexture()
{
    cPlayerShadow* shadow = (cPlayerShadow*)this;
    if (shadow->mLockTexture < 0) {
        cPS2GraphicsMan* mgr = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_particle;
        shadow->mLockTexture = mgr->AddLockedFrameTexture(shadow->mShadowTexWidth, shadow->mShadowTexHeight, 2);
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
 * Hand-checked from the asm only (no C body yet): it calls a run of setup functions (sceDmaGetChan, vu0_uploadProgramChainA,
 * vu0_uploadProgramChainB, ...), allocates two objects with operator_new, creates two semaphores and the kick thread (CreateSema,
 * CreateThread, StartThread), calls DisableIntc, AddIntcHandler2, EnableIntc, DisableDmac, AddDmacHandler2 and
 * EnableDmac (the interrupt-handler setup that Purge undoes), then allocates the texture manager (cMemMan_alloc +
 * cTextureMan_init) and the defer manager and sets up the default render state. cPS2GraphicsMan_Purge undoes these steps.
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
extern "C" void cTextureMan_purgeAll(void*);
void* tIntcHandlerTable_shutdown(void* self);
void operator_delete(int* ptr);
extern char D_005059D8[];

/**
 * Shuts the PS2 renderer down, undoing cPS2GraphicsMan_Init.
 *
 * Calls DisableIntc(2) and removes the INTC 2 handler (mIntcHandlerId), calls DisableDmac(1) and removes the DMAC 0
 * handler (mDmacHandlerId), terminates and deletes the kick thread (mKickThreadId), deletes mKickSema and mQueueMutex,
 * deletes the defer manager (mpDeferMan), purges and deletes the texture manager (cTextureMan_purgeAll, mTexMan), and
 * finally calls tIntcHandlerTable_shutdown on the render queue D_005059D8.
 */
void cPS2GraphicsMan::Purge()
{
    DisableIntc(2);
    RemoveIntcHandler(2, mIntcHandlerId);
    DisableDmac(1);
    RemoveDmacHandler(0, mDmacHandlerId);
    TerminateThread(mKickThreadId);
    DeleteThread(mKickThreadId);
    DeleteSema(mKickSema);
    DeleteSema(mQueueMutex);
    operator_delete((int*)mpDeferMan);
    cTextureMan_purgeAll(mTexMan);
    operator_delete((int*)mTexMan);
    tIntcHandlerTable_shutdown(D_005059D8);
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
 * and id (+0xA0) equal @p width, @p height and @p id, its frame-buffer format (+0x90) fits @p bpp (32 -> kGS_PSMCT32,
 * 24 -> kGS_PSMCT24, 16 -> kGS_PSMCT16), its second buffer format (+0x94) fits @p clutBpp the same way, and @p fmt is
 * 24 with a z-buffer format (+0x98) of kGS_PSMZ24. Likely bugs: case 16 of the @p bpp switch falls through into the 24
 * test, so a 16-bit request also accepts 24-bit modes; and any @p fmt other than 24 can never match. Slot 123 of
 * cPS2GraphicsMan_vtbl; cPS2GraphicsMan_SetDisplayMode calls it.
 *
 * @param width  the screen width
 * @param height  the screen height
 * @param bpp  frame-buffer bits per pixel (16, 24 or 32)
 * @param clutBpp  bits per pixel of the second buffer format (16, 24 or 32)
 * @param fmt  z-buffer bits; only 24 can match
 * @param id  the mode id (+0xA0) to match
 * @return the matching mode index, or -1
 */
int cPS2GraphicsMan::GetDisplayMode(int width, int height, unsigned int bpp, unsigned int clutBpp, int fmt, int id)
{
    int i;
    sPartTexEntA4* displayMode = (sPartTexEntA4*)DisplayModes;
    for (i = 0; i < NumDisplayModes; i++, displayMode++) {
        if (displayMode->width == width && displayMode->height == height && displayMode->id == id) {
            int ok1 = 0;
            int ok2 = 0;
            int ok3 = 0;
            switch (bpp) {
            case 16:
                if (displayMode->psm == kGS_PSMCT16) ok1 = 1; else ok1 = 0;
            case 24:
                if (displayMode->psm == kGS_PSMCT24) ok1 = 1;
                break;
            case 32:
                if (displayMode->psm == kGS_PSMCT32) ok1 = 1; else ok1 = 0;
                break;
            }
            switch (clutBpp) {
            case 16:
                if (displayMode->clutPsm == kGS_PSMCT16) ok2 = 1; else ok2 = 0;
                break;
            case 24:
                if (displayMode->clutPsm == kGS_PSMCT24) ok2 = 1; else ok2 = 0;
                break;
            case 32:
                if (displayMode->clutPsm == kGS_PSMCT32) ok2 = 1; else ok2 = 0;
                break;
            }
            if (fmt == 24) {
                if (displayMode->fmt == kGS_PSMZ24) ok3 = 1; else ok3 = 0;
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
    sVEntry<void>* vt = (sVEntry<void>*)VPTR_OF(cGraphicsMan, self);
    int i = ((int (*)(void*))vt[123].fn)(vthis(self, vt[123]));
    if (i >= 0) {
        vt = (sVEntry<void>*)VPTR_OF(cGraphicsMan, self);
        return ((void* (*)(void*, int))vt[4].fn)(vthis(self, vt[4]), i);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetDisplayMode1);
#ifdef SKIP_ASM


int cPS2GraphicsMan_applyDisplayMode_3764C0(void* self, unsigned int a1, unsigned int a2, unsigned int w, unsigned int h,
                         unsigned int psm, unsigned int psm2, unsigned int zpsm, short s0, short s1, short s2,
                         int flag) __asm__("cPS2GraphicsMan_applyDisplayMode");

/**
 * Switches the display to mode @p idx of DisplayModes unless it is already CurrentDisplayMode.
 *
 * Calls cPS2GraphicsMan_applyDisplayMode with the mode record's words at +0x80 to +0x98 (two addresses, width, height and three pixel
 * formats), 0, the +0x9C flag and its inverse, and the id at +0xA0, then stores @p idx in CurrentDisplayMode. Slot 4 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param idx  the display-mode index (records of 0xA4 bytes)
 * @return 1, always
 */
bool cPS2GraphicsMan::SetDisplayMode1(int idx)
{
    if (CurrentDisplayMode == idx) {
        return true;
    }
    cPS2GraphicsMan_applyDisplayMode_3764C0(this, ((unsigned int)(DisplayModes)[idx].mDisplayWidth),
                         ((unsigned int)(DisplayModes)[idx].mDisplayHeight),
                         ((unsigned int)(DisplayModes)[idx].mFrameWidth),
                         ((unsigned int)(DisplayModes)[idx].mFrameHeight),
                         ((unsigned int)(DisplayModes)[idx].mDisplayPSM),
                         ((unsigned int)(DisplayModes)[idx].mFramePSM),
                         ((unsigned int)(DisplayModes)[idx].mZBufferPSM), 0,
                         (*(bool*)&DisplayModes[idx].mInterlace) ? 1 : 0,
                         (*(bool*)&DisplayModes[idx].mInterlace) ? 0 : 1, DisplayModes[idx].mDTV);
    CurrentDisplayMode = idx;
    return true;
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
const char* cPS2GraphicsMan::GetDisplayModeName(int mode)
{
    return (char*)&DisplayModes[mode];
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
int cPS2GraphicsMan::SetDTV(int enable)
{
    sPartVEntry122* vt = (sPartVEntry122*)VPTR_OF(cGraphicsMan, this);
    return vt[122].fn(vthis(this, vt[122]), 0x200, 0x1C0, 0x18, 0x20, 0x18, enable);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PushViewport);
#ifdef SKIP_ASM




struct sPtGfx65B8 {
    char pad_0x0[0x10DC];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    char* keys;                     // 0x18F0
    char pad_0x18F4[0x5780 - 0x18F4];
    t4Matrix m5780;               // 0x5780
    t4Matrix m57C0;               // 0x57C0
    t4Matrix m5800;               // 0x5800
    t4Vector q5840;                 // 0x5840
    t4Vector q5850;                 // 0x5850
    t4Matrix m5860;               // 0x5860
    t4Matrix m58A0;               // 0x58A0
    t4Vector q58E0;                 // 0x58E0
    t4Vector q58F0;                 // 0x58F0
    tDeferViewPort scissor;            // 0x5900
    float f5908;                    // 0x5908
    float f590C;                    // 0x590C
    float f5910;                    // 0x5910
    char pad_0x5914[0xC];
    t4Vector q5920;                 // 0x5920
    char pad_0x5930[0x6AF0 - 0x5930];
    t4Matrix m6AF0;               // 0x6AF0
    char pad_0x6B30[0x40];
    t4Vector q6B70;                 // 0x6B70
    t4Vector q6B80;                 // 0x6B80
    char pad_0x6B90[0x6D20 - 0x6B90];
    sViewportStack views[1];           // 0x6D20
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtx65B8(t4Matrix* dst, t4Matrix* src)
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
 * view-projection and net matrices, the guard and screen scales and offsets, and the clip bounds (mClipMin /
 * mClipMax) into mViewportStack[mViewportDepth], then increments mViewportDepth and copies the entry into the new top
 * (cPS2GraphicsMan_sViewportStack_operatorE). No overflow check.
 */
void cPS2GraphicsMan::PushViewport()
{
    sViewportStack* viewportStack = &((&mViewportStack[0]))[mViewportDepth];
    *(tDeferViewPort*)&viewportStack->mScissor[0] = *(tDeferViewPort*)&mScissor[0];
    viewportStack->mDeferScissorIndex = mpDeferMan->mActivePortIndex;
    viewportStack->mPerspFar = mPerspFar;
    viewportStack->mPerspFov = mPerspFov;
    vu0CopyMtx65B8((&viewportStack->mScreenMatrix), (&mScreenMatrix));
    *(&viewportStack->mClipMin) = *(&mClipMin);
    *(&viewportStack->mClipMax) = *(&mClipMax);
    vu0CopyMtx65B8((&viewportStack->mWorld), (&mWorld));
    vu0CopyMtx65B8((&viewportStack->mView), (&mView));
    vu0CopyMtx65B8((&viewportStack->mGuardBandMatrix), (&mGuardBandMatrix));
    *(&viewportStack->mGuardScale) = *(&mGuardScale);
    *(&viewportStack->mGuardOffset) = *(&mGuardOffset);
    vu0CopyMtx65B8((&viewportStack->mViewProj), (&mViewProj));
    vu0CopyMtx65B8((&viewportStack->mNetMatrix), (&mNetMatrix));
    *(&viewportStack->mScreenScale) = *(&mScreenScale);
    *(&viewportStack->mScreenOffset) = *(&mScreenOffset);
    viewportStack->mSortZFar = mSortZFar;
    *(&viewportStack->mSortZScale) = *(&mSortZScale);
    mViewportDepth++;
    cPS2GraphicsMan_sViewportStack_operatorE(&((&mViewportStack[0]))[mViewportDepth], viewportStack);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PopViewport);
#ifdef SKIP_ASM




struct sPtGfx6768 {
    char pad_0x0[0x10DC];
    int curView;                    // 0x10DC
    char pad_0x10E0[0x18F0 - 0x10E0];
    char* keys;                     // 0x18F0
    char pad_0x18F4[0x5780 - 0x18F4];
    t4Matrix m5780;               // 0x5780
    t4Matrix m57C0;               // 0x57C0
    t4Matrix m5800;               // 0x5800
    t4Vector q5840;                 // 0x5840
    t4Vector q5850;                 // 0x5850
    t4Matrix m5860;               // 0x5860
    t4Matrix m58A0;               // 0x58A0
    t4Vector q58E0;                 // 0x58E0
    t4Vector q58F0;                 // 0x58F0
    tDeferViewPort scissor;            // 0x5900
    float f5908;                    // 0x5908
    float f590C;                    // 0x590C
    float f5910;                    // 0x5910
    char pad_0x5914[0xC];
    t4Vector q5920;                 // 0x5920
    char pad_0x5930[0x6AF0 - 0x5930];
    t4Matrix m6AF0;               // 0x6AF0
    char pad_0x6B30[0x40];
    t4Vector q6B70;                 // 0x6B70
    t4Vector q6B80;                 // 0x6B80
    int i6B90;                      // 0x6B90
    char pad_0x6B94[0x6D20 - 0x6B94];
    sViewportStack views[1];           // 0x6D20
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtx6768(t4Matrix* dst, t4Matrix* src)
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

extern "C" void cPS2DeferManager_findOrAddScissor(void* table, void* key);

/**
 * Pops the viewport stack: decrements mViewportDepth and restores everything PushViewport saved.
 *
 * The scissor is re-registered with DeferMan (cPS2DeferManager_findOrAddScissor) when the saved scissor index is kNoActivePort, otherwise
 * the index is put back. Clears mMatricesValid.
 */
void cPS2GraphicsMan::PopViewport()
{
    mViewportDepth--;
    sViewportStack* viewportStack = &((&mViewportStack[0]))[mViewportDepth];
    *(tDeferViewPort*)&mScissor[0] = *(tDeferViewPort*)&viewportStack->mScissor[0];
    if (viewportStack->mDeferScissorIndex == cPS2DeferManager::kNoActivePort) {
        cPS2DeferManager_findOrAddScissor((char*)mpDeferMan, (tDeferViewPort*)&mScissor[0]);
    } else {
        mpDeferMan->mActivePortIndex = viewportStack->mDeferScissorIndex;
    }
    mPerspFar = viewportStack->mPerspFar;
    mPerspFov = viewportStack->mPerspFov;
    vu0CopyMtx6768((&mScreenMatrix), (&viewportStack->mScreenMatrix));
    *(&mClipMin) = *(&viewportStack->mClipMin);
    *(&mClipMax) = *(&viewportStack->mClipMax);
    vu0CopyMtx6768((&mWorld), (&viewportStack->mWorld));
    vu0CopyMtx6768((&mView), (&viewportStack->mView));
    vu0CopyMtx6768((&mGuardBandMatrix), (&viewportStack->mGuardBandMatrix));
    *(&mGuardScale) = *(&viewportStack->mGuardScale);
    *(&mGuardOffset) = *(&viewportStack->mGuardOffset);
    vu0CopyMtx6768((&mViewProj), (&viewportStack->mViewProj));
    vu0CopyMtx6768((&mNetMatrix), (&viewportStack->mNetMatrix));
    *(&mScreenScale) = *(&viewportStack->mScreenScale);
    *(&mScreenOffset) = *(&viewportStack->mScreenOffset);
    mSortZFar = viewportStack->mSortZFar;
    *(&mSortZScale) = *(&viewportStack->mSortZScale);
    mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setClippedScissor);
#ifdef SKIP_ASM


/**
 * Clips a rectangle to the screen and stores it as the scissor box (mScissor).
 *
 * Gets the screen size from the renderer's virtual slots 10 and 11 (cPS2GraphicsMan_GetScreenWidth/Height). A negative
 * @p x or @p y is moved to 0 and the width or height shrinks by the same amount; mScissor[0] and [1] become the origin
 * clamped to width - 1 / height - 1, and [2] and [3] the size clamped to what is left of the screen (0 when negative).
 * cPS2GraphicsMan_SetViewportWindow and cPS2GraphicsMan_SetViewportPerspective call it.
 *
 * @param x  the left edge in pixels
 * @param y  the top edge in pixels
 * @param w  the width in pixels
 * @param rectHeight  the height in pixels
 */
void cPS2GraphicsMan::setClippedScissor(float x, float y, float w, float rectHeight)
{
    cPS2GraphicsMan* self = this;
    int width = self->GetScreenWidth();
    int height = self->GetScreenHeight();
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



extern "C" void cPS2GraphicsMan_setClippedScissor(cPS2GraphicsMan* self, float x, float y, float w, float h);
extern "C" void cPS2DeferManager_findOrAddScissor(void* table, void* key);

/**
 * Sets the current viewport rectangle, kept inside the TV safe band, and updates the scissor.
 *
 * When mTVDisplayFormat.mDisplayFormat is 0 to 2, @p y is raised to at least mfTVYOffset * the slot 11 screen height
 * and @p rectHeight is limited to mfTVYScale * that height (the height, not the bottom edge, so a raised top moves the
 * bottom down). Stores the rectangle in mViewportStack[mViewportDepth], then calls cPS2GraphicsMan_setClippedScissor and
 * cPS2DeferManager_findOrAddScissor(mpDeferMan, mScissor). Slot 23 of cPS2GraphicsMan_vtbl.
 *
 * @param x  the left edge
 * @param y  the top edge
 * @param w  the width
 * @param rectHeight  the height
 */
void cPS2GraphicsMan::SetViewportWindow(float x, float y, float w, float rectHeight, int)
{
    int height = GetScreenHeight();
    int mode = mTVDisplayFormat.mDisplayFormat;
    if (mode < 3) {
        if (mode >= 0) {
            float fh = (float)height;
            float top = mTVDisplayFormat.mfTVYOffset * fh;
            if (y < top) {
                y = top;
            }
            float bottom = mTVDisplayFormat.mfTVYScale * fh;
            if (bottom < rectHeight) {
                rectHeight = bottom;
            }
        }
    }
    ((&mViewportStack[0]))[mViewportDepth].mWindow[0] = x;
    ((&mViewportStack[0]))[mViewportDepth].mWindow[1] = y;
    ((&mViewportStack[0]))[mViewportDepth].mWindow[2] = w;
    ((&mViewportStack[0]))[mViewportDepth].mWindow[3] = rectHeight;
    this->setClippedScissor(x, y, w, rectHeight);
    cPS2DeferManager_findOrAddScissor(((void*)mpDeferMan), mScissor);
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
extern "C" sPartVec4i* cPS2GraphicsMan_GetViewportWindow(sPartVec4i* out, cPS2GraphicsMan* self)
{
    int i = self->mViewportDepth;
    float* window = &self->mViewportStack[i].mWindow[0];
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
 * Returns the current viewport's field of view (mProjParams[2] of viewport stack entry mViewportDepth).
 *
 * @return the field of view
 */
float cPS2GraphicsMan::GetViewportFOV()
{
    int i = mViewportDepth;
    return mViewportStack[i].mProjParams[2];
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
 * The viewport entry stores them as mProjParams[0] = near, [1] = far, [2] = field of view.
 *
 * @param fov  receives the field of view (mProjParams[2])
 * @param minz  receives the near distance (mProjParams[0])
 * @param maxz  receives the far distance (mProjParams[1])
 */
extern "C" void cPS2GraphicsMan_GetViewportPerspective(cPS2GraphicsMan* self, float* fov, float* minz, float* maxz)
{
    sViewportStack* viewportStack = &((&self->mViewportStack[0]))[self->mViewportDepth];
    *fov = viewportStack->mProjParams[2];
    *minz = viewportStack->mProjParams[0];
    *maxz = viewportStack->mProjParams[1];
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



struct sPtVtU6C58 {
    short delta;
    short index;
    unsigned (*fn)(void*);
};


// PORT: the unit declares fSinCos as (s, c, angle); its real parameter order is (angle, s, c).
extern void SinCos_6C58(float angle, float* s, float* c) __asm__("fSinCos");
extern "C" float tanf(float x);
extern "C" void cPS2GraphicsMan_setClippedScissor_6C58(void* self, float x, float y, float w, float h) __asm__("cPS2GraphicsMan_setClippedScissor");

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fsAbs_6C58(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Scales a projection pair by the active letterbox mode's factors.
static inline void ClampScale6C58(cPS2GraphicsMan* self, float& x, float& y)
{
    int mode = self->mTVDisplayFormat.mDisplayFormat;
    if (mode < 3) {
        if (mode >= 0) {
            x *= self->mTVDisplayFormat.mfXAspectAdjust;
            y *= self->mTVDisplayFormat.mfYAspectAdjust;
        }
    }
}

/**
 * Sets a perspective projection with field-of-view angle @p fov for the current viewport.
 *
 * Marks the stack entry perspective (mProjectionType = 0) and stores @p zNear, @p zFar and @p fov in it and in
 * mPerspFar/mPerspFov. The viewport rectangle comes from mViewportStack[mViewportDepth] (x, y, w, h); the depth range
 * is 2^24 - 1 when mCurrentZBFormat is 0x31 (24-bit Z), else 65535, and offX/offY = 32768 - 8 * the slot 10/11 screen
 * width/height place the screen in the GS's 4096 space (1/16 pixels). The horizontal scale is (w / 2) / tan(@p fov) and
 * the vertical one that times the screen's height * 4/3 / width; when mTVDisplayFormat.mDisplayFormat is 0 to 2 both
 * are multiplied by mfXAspectAdjust and mfYAspectAdjust.
 *
 * It writes mScreenMatrix (with the entry's depth bias mZBias in the z offset), the screen bounds mClipMin/mClipMax
 * (w = 1 / zNear and 1 / zFar), mViewProj, mScreenScale/mScreenOffset and the guard-band
 * mView/mGuardScale/mGuardOffset, clears mMatricesValid, sets the scissor (cPS2GraphicsMan_setClippedScissor), mSortZFar = 40000 and
 * mSortZScale = (1023, ..., 1023 / 40000). Slot 25 of cPS2GraphicsMan_vtbl.
 *
 * @param fov  the field-of-view angle (radians) whose tangent sets the scale
 * @param zNear  the near plane
 * @param zFar  the far plane
 */
void cPS2GraphicsMan::SetViewportPerspective(float fov, float zNear, float zFar)
{
    sViewportStack* viewEntry = &((&mViewportStack[0]))[mViewportDepth];
    viewEntry->mProjectionType = 0;
    viewEntry->mProjParams[0] = zNear;
    viewEntry->mProjParams[1] = zFar;
    viewEntry->mProjParams[2] = fov;
    // PORT: int-arithmetic view address (as in cPS2GraphicsMan_GetViewportFOV); the typed views[i] reads score 94-95%.
    char* viewBase = (char*)this + mViewportDepth * 0x230;
    float x = *(float*)(viewBase + 0x6D20);
    float y = *(float*)(viewBase + 0x6D24);
    float w = *(float*)(viewBase + 0x6D28);
    float viewHeight = *(float*)(viewBase + 0x6D2C);
    float zlo, zhi, zscale;
    if (mCurrentZBFormat == kGS_PSMZ24) {
        zscale = 16777215.0f;
        zlo = -8388467.5f;
        zhi = 8388467.5f;
    } else {
        zscale = 65535.0f;
        zlo = -32767.5f;
        zhi = 32767.5f;
    }
    mPerspFar = zFar;
    mPerspFov = fov;
    sPtVtU6C58* vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[11];
    unsigned height = vtEntry->fn(vthis(this, *vtEntry));
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[10];
    unsigned width = vtEntry->fn(vthis(this, *vtEntry));
    float aspect = (float)height * 1.3333334f / (float)width;
    float sinFov, cosFov;
    SinCos_6C58(fov, &sinFov, &cosFov);
    float sx = (w * 0.5f) / (sinFov / cosFov);
    float sy = sx * aspect;
    ClampScale6C58(this, sx, sy);
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[10];
    float offX = 32768.0f - (float)(vtEntry->fn(vthis(this, *vtEntry)) * 8);
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[11];
    float offY = 32768.0f - (float)(vtEntry->fn(vthis(this, *vtEntry)) * 8);
    float zA = (-zscale * zNear) / (zFar - zNear);
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][0] = sx * 16.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][0] = (w * 0.5f + x) * 16.0f + offX;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][1] = -sy * 16.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][1] = (viewHeight * 0.5f + y) * 16.0f + offY;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][2] = zA;
    float zB = -zFar * zA + viewEntry->mZBias;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][2] = zB;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][3] = 1.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][3] = 0.0f;
    ((float*)&(*(&mClipMin)).x)[0] = x * 16.0f + offX;
    ((float*)&(*(&mClipMin)).x)[1] = y * 16.0f + offY;
    ((float*)&(*(&mClipMin)).x)[2] = zA * zNear + zB;
    ((float*)&(*(&mClipMax)).x)[0] = (x + w) * 16.0f + offX;
    ((float*)&(*(&mClipMax)).x)[1] = (y + viewHeight) * 16.0f + offY;
    ((float*)&(*(&mClipMax)).x)[2] = zA * zFar + zB;
    ((float*)&(*(&mClipMin)).x)[3] = 1.0f / zNear;
    ((float*)&(*(&mClipMax)).x)[3] = 1.0f / zFar;
    mMatricesValid = 0;
    float aspect2 = aspect * (w / viewHeight);
    float px = 1.0f / tanf(fov);
    float py = 1.0f / tanf(fov) * aspect2;
    ClampScale6C58(this, px, py);
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[10];
    float sw = (float)vtEntry->fn(vthis(this, *vtEntry));
    float cx = (w * 0.5f - sw * 0.5f) + x;
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[11];
    float sh = (float)vtEntry->fn(vthis(this, *vtEntry));
    float cy = (viewHeight * 0.5f - sh * 0.5f) + y;
    float zz = (zFar + zNear) / (zFar - zNear);
    float zw = -(2.0f * zFar * zNear) / (zFar - zNear);
    (*(sPtMatrix_6C58*)&mViewProj).m[0][0] = px;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][1] = py;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][2] = zz;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][3] = 1.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][2] = zw;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][3] = 0.0f;
    ((float*)&(*(&mScreenScale)).x)[0] = w * 0.5f;
    ((float*)&(*(&mScreenScale)).x)[1] = -(viewHeight * 0.5f);
    ((float*)&(*(&mScreenScale)).x)[2] = zlo;
    ((float*)&(*(&mScreenScale)).x)[3] = 0.0f;
    ((float*)&(*(&mScreenOffset)).x)[0] = cx + 2047.5f;
    ((float*)&(*(&mScreenOffset)).x)[1] = cy + 2047.5f;
    ((float*)&(*(&mScreenOffset)).x)[2] = zhi + viewEntry->mZBias;
    ((float*)&(*(&mScreenOffset)).x)[3] = 0.0f;
    float gw = 2048.0f - 2.0f * fsAbs_6C58(cx);
    float gh = 2048.0f - 2.0f * fsAbs_6C58(cy);
    (*(sPtMatrix_6C58*)&mView).m[0][0] = (w / gw) * px;
    (*(sPtMatrix_6C58*)&mView).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][1] = (viewHeight / gh) * py;
    (*(sPtMatrix_6C58*)&mView).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][2] = zz;
    (*(sPtMatrix_6C58*)&mView).m[2][3] = 1.0f;
    (*(sPtMatrix_6C58*)&mView).m[3][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[3][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[3][2] = zw;
    (*(sPtMatrix_6C58*)&mView).m[3][3] = 0.0f;
    ((float*)&(*(&mGuardScale)).x)[0] = gw * 0.5f;
    ((float*)&(*(&mGuardScale)).x)[1] = -(gh * 0.5f);
    ((float*)&(*(&mGuardScale)).x)[2] = zlo;
    ((float*)&(*(&mGuardScale)).x)[3] = 0.0f;
    ((float*)&(*(&mGuardOffset)).x)[0] = cx + 2047.5f;
    ((float*)&(*(&mGuardOffset)).x)[1] = cy + 2047.5f;
    ((float*)&(*(&mGuardOffset)).x)[2] = zhi + viewEntry->mZBias;
    ((float*)&(*(&mGuardOffset)).x)[3] = 0.0f;
    cPS2GraphicsMan_setClippedScissor_6C58(this, x, y, w, viewHeight);
    mSortZFar = 40000.0f;
    ((float*)&(*(&mSortZScale)).x)[0] = 1023.0f;
    ((float*)&(*(&mSortZScale)).x)[1] = 0.0f;
    ((float*)&(*(&mSortZScale)).x)[2] = 0.0f;
    ((float*)&(*(&mSortZScale)).x)[3] = 1023.0f / 40000.0f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetViewportZBias);
#ifdef SKIP_ASM
// (t4VectorInit, the t4Vector with constructors, is defined with cPlayerShadow_AddShadow above.)





// PORT: PS2-only VU0 inline asm (4x4 matrix * vector); the PC port needs a C fallback.
static inline t4VectorInit MulMat7278(const t4Matrix* m, const t4VectorInit& v)
{
    t4VectorInit r;
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
static inline t4VectorInit Scale7278(const t4VectorInit& v, float s)
{
    t4VectorInit r;
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
static inline t4VectorInit Add7278(const t4VectorInit& a, const t4VectorInit& b)
{
    t4VectorInit r;
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

extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

static inline float DepthOffset7278(cPS2GraphicsMan* self, t4VectorInit* pos, float dist)
{
    if (dist != 0.0f) {
    t4VectorInit dir(self->mCurrentView->row[0].z, self->mCurrentView->row[1].z, self->mCurrentView->row[2].z, 0.0f);
    t4VectorInit a = MulMat7278((&self->mNetMatrix), *pos);
    t4VectorInit c = MulMat7278((&self->mNetMatrix), Add7278(*pos, Scale7278(dir, dist)));
    float za = a.z / a.w;
    float zc = c.z / c.w;
    if (-1.0f <= za && za <= 1.0f && -1.0f <= zc && zc <= 1.0f) {
        return self->mScreenScale.z * (zc - za);
    }
    }
    return 0.0f;
}

/**
 * Sets a depth bias for the current viewport from a world position and a world-space distance.
 *
 * Updates the clip matrices first when they are stale. The offset comes from DepthOffset7278(); when it differs from
 * the viewport's stored depth it is saved there and added to 32767.5 (8388467.5 when the mode word is kGS_PSMZ24) to
 * give the two Z constants at +0x58F8 and +0x5858, and texReady is cleared so the state is re-sent.
 *
 * @param pos  the world position the bias is measured at
 * @param dist  the bias distance in world units
 */
extern "C" void cPS2GraphicsMan_SetViewportZBias(cPS2GraphicsMan* self, t4VectorInit* pos, float dist)
{
    sViewportStack* view = &((&self->mViewportStack[0]))[self->mViewportDepth];
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    float depthOffset = DepthOffset7278(self, pos, dist);
    if (depthOffset != view->mZBias) {
        view->mZBias = depthOffset;
        float k = 32767.5f;
        if (self->mCurrentZBFormat == kGS_PSMZ24) {
            k = 8388467.5f;
        }
        self->mScreenOffset.z = k + depthOffset;
        self->mGuardOffset.z = k + view->mZBias;
        self->mMatricesValid = 0;
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
static inline void ZScale7458(cPS2GraphicsMan* self, float& zscale)
{
    if (self->mCurrentZBFormat == 0x31) {
        zscale = 16777215.0f;
    } else {
        zscale = 65535.0f;
    }
}

/**
 * Sets an orthographic projection mapping the box from (@p cx, @p cy), @p fx wide and @p fy high, onto the current
 * viewport.
 *
 * Marks the stack entry orthographic (mProjectionType = 1) and stores @p zNear and @p zFar in it. The viewport
 * rectangle comes from mViewportStack[mViewportDepth] (x, y, w, h); the depth range is 2^24 - 1 when mCurrentZBFormat
 * is 0x31 (24-bit Z), else 65535, and offX/offY = 32768 - 8 * the slot 10/11 screen width/height place the screen in
 * the GS's 4096 space (1/16 pixels). It writes mScreenMatrix (scale w / @p fx * 16 and h / @p fy * 16, depth -range /
 * (zFar - zNear)), the screen bounds mClipMin/mClipMax, mViewProj (2 / @p fx, 2 / @p fy, offset -1), mScreenScale and
 * mScreenOffset, and the guard-band mView, mGuardScale and mGuardOffset, and clears mMatricesValid.
 *
 * Then cPS2GraphicsMan_setClippedScissor sets the scissor to the viewport, mSortZFar = 40000 and mSortZScale = (1023, 0, 0, 1023 / 40000).
 * Slot 26 of cPS2GraphicsMan_vtbl.
 *
 * @param cx  the box's left edge
 * @param cy  the box's top edge
 * @param fx  the box width
 * @param fy  the box height
 * @param zNear  the near depth
 * @param zFar  the far depth
 */
void cPS2GraphicsMan::SetViewportOrthographic(float cx, float cy, float fx, float fy, float zNear, float zFar, int, int)
{
    sViewportStack* viewEntry = &((&mViewportStack[0]))[mViewportDepth];
    viewEntry->mProjectionType = 1;
    viewEntry->mProjParams[0] = zNear;
    viewEntry->mProjParams[1] = zFar;
    // PORT: int-arithmetic view address, as in the perspective sibling cPS2GraphicsMan_SetViewportPerspective.
    char* viewBase = (char*)this + mViewportDepth * 0x230;
    float x = *(float*)(viewBase + 0x6D20);
    float y = *(float*)(viewBase + 0x6D24);
    float w = *(float*)(viewBase + 0x6D28);
    float viewHeight = *(float*)(viewBase + 0x6D2C);
    float zscale;
    ZScale7458(this, zscale);
    sPtVtU6C58* vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[10];
    float offX = 32768.0f - (float)(vtEntry->fn(vthis(this, *vtEntry)) * 8);
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[11];
    float offY = 32768.0f - (float)(vtEntry->fn(vthis(this, *vtEntry)) * 8);
    float sx = w / fx * 16.0f;
    float sy = viewHeight / fy * 16.0f;

    float zA = -zscale / (zFar - zNear);
    float zB = -zFar * zA;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][0] = sx;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][0] = x * 16.0f - sx * cx + offX;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][1] = sy;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][1] = y * 16.0f - sy * cy + offY;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][2] = zA;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][2] = zB;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[2][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mScreenMatrix).m[3][3] = 1.0f;
    ((float*)&(*(&mClipMin)).x)[0] = x * 16.0f + offX;
    ((float*)&(*(&mClipMin)).x)[1] = y * 16.0f + offY;
    ((float*)&(*(&mClipMin)).x)[2] = zA * zNear + zB;
    ((float*)&(*(&mClipMin)).x)[3] = 1.0f;
    ((float*)&(*(&mClipMax)).x)[0] = (x + w) * 16.0f + offX;
    ((float*)&(*(&mClipMax)).x)[1] = (y + viewHeight) * 16.0f + offY;
    ((float*)&(*(&mClipMax)).x)[2] = zA * zFar + zB;
    ((float*)&(*(&mClipMax)).x)[3] = 1.0f;
    mMatricesValid = 0;
    float zlo, zhi;
    if (mCurrentZBFormat == kGS_PSMZ24) {
        zlo = -8388467.5f;
        zhi = 8388467.5f;
    } else {
        zlo = -32767.5f;
        zhi = 32767.5f;
    }
    float px = 2.0f / fx;
    float py = 2.0f / fy;
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[10];
    float sw = (float)vtEntry->fn(vthis(this, *vtEntry));
    float ox = (w * 0.5f - sw * 0.5f) + x;
    vtEntry = &((sPtVtU6C58*)VPTR_OF(cGraphicsMan, this))[11];
    float sh = (float)vtEntry->fn(vthis(this, *vtEntry));
    float oy = (viewHeight * 0.5f - sh * 0.5f) + y;
    float zz = 2.0f / (zFar - zNear);
    float zw = -(zFar + zNear) / (zFar - zNear);
    (*(sPtMatrix_6C58*)&mViewProj).m[0][0] = px;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][1] = py;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][2] = zz;
    (*(sPtMatrix_6C58*)&mViewProj).m[2][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][0] = -1.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][1] = -1.0f;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][2] = zw;
    (*(sPtMatrix_6C58*)&mViewProj).m[3][3] = 1.0f;
    ((float*)&(*(&mScreenScale)).x)[0] = w * 0.5f;
    ((float*)&(*(&mScreenScale)).x)[1] = viewHeight * 0.5f;
    ((float*)&(*(&mScreenScale)).x)[2] = zlo;
    ((float*)&(*(&mScreenScale)).x)[3] = 0.0f;
    float tx = (ox + 2047.5f) - cx * w / fx;
    float ty = (oy + 2047.5f) - cy * viewHeight / fy;
    ((float*)&(*(&mScreenOffset)).x)[0] = tx;
    ((float*)&(*(&mScreenOffset)).x)[1] = ty;
    ((float*)&(*(&mScreenOffset)).x)[2] = zhi;
    ((float*)&(*(&mScreenOffset)).x)[3] = 0.0f;
    float gw = 2048.0f - 2.0f * fsAbs_6C58(ox);
    float gh = 2048.0f - 2.0f * fsAbs_6C58(oy);
    float gx = w / gw;
    float gy = viewHeight / gh;
    (*(sPtMatrix_6C58*)&mView).m[0][0] = gx * px;
    (*(sPtMatrix_6C58*)&mView).m[0][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[0][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[0][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][1] = gy * py;
    (*(sPtMatrix_6C58*)&mView).m[1][2] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[1][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][0] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][1] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[2][2] = zz;
    (*(sPtMatrix_6C58*)&mView).m[2][3] = 0.0f;
    (*(sPtMatrix_6C58*)&mView).m[3][0] = -gx;
    (*(sPtMatrix_6C58*)&mView).m[3][1] = -gy;
    (*(sPtMatrix_6C58*)&mView).m[3][2] = zw;
    (*(sPtMatrix_6C58*)&mView).m[3][3] = 1.0f;
    ((float*)&(*(&mGuardScale)).x)[0] = gw * 0.5f;
    ((float*)&(*(&mGuardScale)).x)[1] = gh * 0.5f;
    ((float*)&(*(&mGuardScale)).x)[2] = zlo;
    ((float*)&(*(&mGuardScale)).x)[3] = 0.0f;
    ((float*)&(*(&mGuardOffset)).x)[0] = tx;
    ((float*)&(*(&mGuardOffset)).x)[1] = ty;
    ((float*)&(*(&mGuardOffset)).x)[2] = zhi;
    ((float*)&(*(&mGuardOffset)).x)[3] = 0.0f;
    this->setClippedScissor(x, y, w, viewHeight);
    mSortZFar = 40000.0f;
    ((float*)&(*(&mSortZScale)).x)[0] = 1023.0f;
    ((float*)&(*(&mSortZScale)).x)[1] = 0.0f;
    ((float*)&(*(&mSortZScale)).x)[2] = 0.0f;
    ((float*)&(*(&mSortZScale)).x)[3] = 1023.0f / 40000.0f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetTVDisplayFormat);
#ifdef SKIP_ASM
/**
 * Sets the TV display format (mTVDisplayFormat) from a format number.
 *
 * Stores @p mode in mDisplayFormat, then: 0 (GM_FULLSCREEN) sets mfTVYOffset 0 and mfTVYScale, mfXAspectAdjust,
 * mfYAspectAdjust 1; 1 (GM_WIDESCREEN, letterboxed) sets the offset 0.125 and the three scales 0.75; 2 (GM_ANIMORPHIC)
 * sets mfXAspectAdjust 0.75, the offset 0 and the other two 1. Other values change only mDisplayFormat. Slot 40 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param mode  the display format (GMDisplayFormat): 0 full screen, 1 widescreen (letterbox), 2 anamorphic
 */
void cPS2GraphicsMan::SetTVDisplayFormat(int mode)
{
    mTVDisplayFormat.mDisplayFormat = mode;
    switch (mode) {
    case GM_FULLSCREEN:
        mTVDisplayFormat.mfTVYOffset = 0.0f;
        mTVDisplayFormat.mfTVYScale = 1.0f;
        mTVDisplayFormat.mfXAspectAdjust = 1.0f;
        mTVDisplayFormat.mfYAspectAdjust = 1.0f;
        break;
    case GM_WIDESCREEN:
        mTVDisplayFormat.mfTVYOffset = 0.125f;
        mTVDisplayFormat.mfTVYScale = 0.75f;
        mTVDisplayFormat.mfXAspectAdjust = 0.75f;
        mTVDisplayFormat.mfYAspectAdjust = 0.75f;
        break;
    case GM_ANIMORPHIC:
        mTVDisplayFormat.mfXAspectAdjust = 0.75f;
        mTVDisplayFormat.mfTVYOffset = 0.0f;
        mTVDisplayFormat.mfTVYScale = 1.0f;
        mTVDisplayFormat.mfYAspectAdjust = 1.0f;
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
void cPS2GraphicsMan::SetTVDisplayFormat1(const tTVDisplayFormat& src)
{
    *(&mTVDisplayFormat) = src;
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


extern sDrawState sDrawState_s_defaultDrawState_rs7A10 __asm__("sDrawState_s_defaultDrawState");
extern char D_005059D8[];
extern "C" void cPS2DeferManager_buildDmaChains(void* ring, unsigned int* a, int* x, unsigned int* b, int* y);
extern "C" void* WaitSema(int sema);  // WaitSema (returns in $2: the call clobbers v0)
extern "C" void SignalSema(int sema);
extern "C" void InvalidDCache(unsigned int start, unsigned int end);
extern "C" void cPS2DeferManager_resetFrame(void* ring);
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
    sVEnt_7A10* vt = (sVEnt_7A10*)VPTR_OF(cGraphicsMan, this);
    void (**pf)(void*, const sRect_7A10&, int) = &vt[24].fn;
    (*pf)(vthis(this, vt[24]), sRect_7A10(0.0f, 0.0f, (float)GetWidth(), (float)GetHeight()), 1);
}

/**
 * Ends the frame: queues this frame's packets for the kick thread and switches to the other packet buffer.
 *
 * Defined in C as cGfx_7A10::EndFrame_impl(). It calls cPS2DeferManager_buildDmaChains on the render ring, waits on mQueueMutex
 * (+0x5ACC), stores the packet-buffer words at +0x5AA4..+0x5AB4, signals mQueueMutex and mKickSema (+0x5AC8), swaps the
 * double-buffered packet pointers around +0x5A00..+0x5A24, calls InvalidDCache on the new buffer ranges, resets the
 * draw state to sDrawState_s_defaultDrawState and calls cPS2DeferManager_resetFrame on the ring.
 */
void cGfx_7A10::EndFrame_impl()
{
    cPS2GraphicsMan* gfxBase = (cPS2GraphicsMan*)this;
    ResetViewport();
    DrawClear();
    unsigned int vif1 = gfxBase->mVifCursor;
    unsigned int gif1 = gfxBase->mGifCursor;
    int vif2;
    int gif2;
    cPS2DeferManager_buildDmaChains(gfxBase->mpDeferMan, (unsigned int*)(&gfxBase->mVifCursor), &vif2, (unsigned int*)(&gfxBase->mGifCursor), &gif2);
    WaitSema(gfxBase->mQueueMutex);
    // bufState[2] (+0x5A90) and queuedIndex (+0x5AA4) are shared with the DMA kick thread
    // (cPS2GraphicsMan_runDmaKickThread) and the VIF1 DMA interrupt: volatile, like cPS2GraphicsMan_claimBuildBuffer's bufState.
    volatile int* bufState = (volatile int*)(&gfxBase->mBufState[0]);
    gfxBase->mQueuedIndex = gfxBase->mBuildIndex;
    volatile int* state = &bufState[gfxBase->mQueuedIndex];
    gfxBase->mQueuedVif1 = vif1;
    gfxBase->mQueuedVif2 = vif2;
    gfxBase->mQueuedGif1 = gif1;
    gfxBase->mQueuedGif2 = gif2;
    *state = cPS2GraphicsMan::kBuf_Queued;
    SignalSema(gfxBase->mQueueMutex);
    SignalSema(gfxBase->mKickSema);
    gfxBase->mBuildIndex = gfxBase->mBuildIndex == 0;
    gfxBase->mGifBuildIndex = gfxBase->mGifBuildIndex == 0;
    gfxBase->mVifCursor = gfxBase->mVifHalfStart = gfxBase->mVifBuffBase + gfxBase->mBuildIndex * 0xCCC80;
    gfxBase->mGifCursor = gfxBase->mGifHalfStart = gfxBase->mGifBuffBase + gfxBase->mGifBuildIndex * 0x26600;
    InvalidDCache(gfxBase->mVifCursor, gfxBase->mVifCursor + 0xCCC80);
    InvalidDCache(gfxBase->mGifCursor, gfxBase->mGifCursor + 0x26600);
    *gfxBase->m_pCurrentDrawState = sDrawState_s_defaultDrawState_rs7A10;
    cPS2DeferManager_resetFrame(gfxBase->mpDeferMan);
    func_003673D0(gfxBase->mTexMan);
    ResetViewport();
    func_00361F90(D_005059D8);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawSprite);
#ifdef SKIP_ASM
// PORT: 128-bit GPR quadword (TImode); the PC port needs a 16-byte struct.
typedef int cQuad_7CF0 __attribute__((mode(TI)));


// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline t4Vector mtxApply_7CF0(t4Matrix* m, const t4Vector& v)
{
    t4Vector out;
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
static inline t4Vector Add_7CF0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector Sub_7CF0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline int ClipCode_7CF0(cPS2GraphicsMan* self, const t4Vector& v)
{
    int code = 0;
    if (v.x < self->mClipMin.x) {
        code |= 2;
    }
    if (self->mClipMax.x < v.x) {
        code |= 1;
    }
    if (v.y < self->mClipMin.y) {
        code |= 8;
    }
    if (self->mClipMax.y < v.y) {
        code |= 4;
    }
    if (v.w < 0.0f) {
        code ^= 0x2F;
    } else {
        if (v.z > self->mClipMin.z) {
            code |= 0x20;
        }
        if (v.z < self->mClipMax.z) {
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
extern "C" void cPS2GraphicsMan_DrawSprite(cPS2GraphicsMan* self, t4Vector* pos, float* size, int* uv0, int* uv1,
    float* col)
{
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    t4Vector clipPos = mtxApply_7CF0(&self->mScreenWorld, *pos);
    float sw = size[0];
    float sh = size[1];
    float w = 1.0f / clipPos.w;
    float sx = self->mScreenMatrix.row[0].x * sw;
    float sy = self->mScreenMatrix.row[1].y * sh;
    clipPos.x *= w;
    clipPos.y *= w;
    clipPos.w = w;
    t4Vector halfSize;
    halfSize.x = sx * w;
    halfSize.y = sy * w;
    halfSize.z = 0.0f;
    halfSize.w = 0.0f;
    t4Vector lo = Sub_7CF0(clipPos, halfSize);
    t4Vector hi = Add_7CF0(clipPos, halfSize);
    if (ClipCode_7CF0(self, lo) & ClipCode_7CF0(self, hi)) {
        return;
    }

    float gsQ = 1.0f;
    unsigned int buf = self->mVifCursor;
    // ORIGINAL-PTR: the GIF packet and the defer record are written through the EE's uncached-accelerated alias
    // (address | 0x30000000, a hardware address segment)
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
    self->mVifCursor = next;
    cPS2DeferManager* ring = self->mpDeferMan;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = ((int)rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry =
            (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)ring->mSubmits | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
    }
    self->mVifCursor += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawRotatedSprite);
#ifdef SKIP_ASM
// GrMan slot 92, GetScreenCoord (the projected position, returned by value).
typedef sVEntry<t4VectorInit(void*, const t4Vector*)> tScreenCoordSlot;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorInit Scale_81A0(const t4Vector& v, float s)
{
    t4VectorInit r;
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
static inline t4VectorInit Add_81A0(const t4Vector& a, const t4Vector& b)
{
    t4VectorInit r;
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
static inline t4VectorInit Sub_81A0(const t4Vector& a, const t4Vector& b)
{
    t4VectorInit r;
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

static inline void Rot_81A0(t4Vector* out, const t4Matrix* m, const t4Vector& v)
{
    float x = v.x * m->row[0].x + v.y * m->row[0].y;
    float y = v.x * m->row[1].x + v.y * m->row[1].y;
    float z = v.x * m->row[2].x + v.y * m->row[2].y;
    float w = v.x * m->row[3].x + v.y * m->row[3].y;
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;
}

static inline t4VectorInit Project_81A0(cPS2GraphicsMan* self, const t4Vector* v)
{
    tScreenCoordSlot* vt = (tScreenCoordSlot*)VPTR_OF(cGraphicsMan, self);
    return vt[92].fn(vthis(self, vt[92]), v);
}

extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);
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
extern "C" void cPS2GraphicsMan_DrawRotatedSprite(cPS2GraphicsMan* self, const t4Vector* pos, float* size, int* uv0,
    int* uv1, float* col, int count, float angle)
{
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    float gsQ = 1.0f;
    t4VectorInit center = Project_81A0(self, pos);
    float sinAngle, cosAngle;
    fSinCos(&sinAngle, &cosAngle, angle);
    t4VectorInit axisU;
    t4VectorInit axisV(cosAngle, sinAngle, 0.0f, 0.0f);
    Rot_81A0(&axisU, self->mCurrentView, axisV);
    Rot_81A0(&axisV, self->mCurrentView, t4VectorInit(-axisV.y, axisV.x, axisV.z, axisV.w));
    t4VectorInit edgeU = Add_81A0(*pos, Scale_81A0(axisU, size[0]));
    t4VectorInit edgeV = Add_81A0(*pos, Scale_81A0(axisV, size[1]));
    edgeU.w = gsQ;
    edgeV.w = gsQ;
    t4VectorInit halfU = Sub_81A0(Project_81A0(self, &edgeU), center);
    t4VectorInit halfV = Sub_81A0(Project_81A0(self, &edgeV), center);
    t4VectorInit corner0 = Sub_81A0(Sub_81A0(center, halfU), halfV);
    t4VectorInit corner1 = Sub_81A0(Add_81A0(center, halfU), halfV);
    t4VectorInit corner2 = Add_81A0(Sub_81A0(center, halfU), halfV);
    t4VectorInit corner3 = Add_81A0(Add_81A0(center, halfU), halfV);

    unsigned int buf = self->mVifCursor;
    // ORIGINAL-PTR: the GIF packet and the defer record are written through the EE's uncached-accelerated alias
    // (address | 0x30000000, a hardware address segment)
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    int xoff = (0x800 - (((unsigned int)self->mFBWidth) >> 1)) << 4;
    int yoff = (0x800 - (((unsigned int)self->mFBHeight) >> 1)) << 4;
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
    self->mVifCursor = next;
    cPS2DeferManager* ring = self->mpDeferMan;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
    }
    self->mVifCursor += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawString3D);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
struct sPtRenderState {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words)
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

union uPtFloatBits {
    float f;
    int i;
};

typedef int cQuad128_CF0 __attribute__((mode(TI)));

// PORT: PS2-only VU0 inline asm (vector subtract); the PC port needs a C fallback.
static inline t4VectorInit Sub_CF0(const t4VectorInit& a, const t4VectorInit& b)
{
    t4VectorInit r;
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




// Homogeneous position (w given) with a const-reference ctor: its stores, w included, go straight to the stack
// slot, where the by-value t4VectorInit ctor keeps `this` in a register for a constant w.
struct t4VectorRefInit : t4VectorInit {
    t4VectorRefInit(const float& ax, const float& ay, const float& az, const float& aw)
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
static inline tGlyph* Find8808(cFont* f, int c)
{
    if (c < f->FirstCode)
        return 0;
    if (c <= f->LastLinearCode)
        return (f->Glyphs) + c - f->FirstCode;
    int lo = f->LastLinearCode - f->FirstCode;
    int hi = f->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* p = f->Glyphs + mid;
        if (c == p->code)
            return p;
        if (c < p->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (f->MissingGlyph);
}

/**
 * Draws string @p text in @p font as one textured sprite per glyph, queued as a single GS packet.
 *
 * Rebuilds the matrices first (cPS2GraphicsMan_computeMatrices) when mMatricesValid is 0. Each character's glyph is found by code (direct
 * index from FirstCode to LastLinearCode, then a binary search, else MissingGlyph). When the current viewport is
 * orthographic (word +0x20 of mViewportStack[mViewportDepth] non-zero) the corners are scaled and offset by
 * mScreenMatrix directly; otherwise each glyph's centre goes through mScreenWorld with a perspective divide and the
 * half-size is scaled by 1/w. UVs are the glyph's u/v times @p su and @p sv, and @p x advances by the glyph advance
 * times @p sx.
 *
 * The packet at mVifCursor (uncached) holds a DMA cnt tag, a VIF DIRECT code, PRIM 0x56 (sprite, textured, blended)
 * with RGBAQ R, G, B = color[1..3] * 204, A = color[0] * 128, Q = 1, and a REGLIST GIF tag of ST/XYZ2 pairs. Then,
 * while mpDeferMan holds fewer than 2600 entries, a 0x80-byte entry is appended to it with the current draw state
 * (*m_pCurrentDrawState, low 5 bits of its third word from the manager's mActivePortIndex), the packet's start and
 * end and the view index mActiveViewIndex. mVifCursor moves past the packet. Slot 80 of cPS2GraphicsMan_vtbl.
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
extern "C" void cPS2GraphicsMan_DrawString3D(cPS2GraphicsMan* self, cFont* font, char* text, float* color,
                              float x, float y, float sx, float sy, float su, float sv, float z)
{
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    unsigned int buf = self->mVifCursor;
    // ORIGINAL-PTR: the GIF packet and the defer record are written through the EE's uncached-accelerated alias
    // (address | 0x30000000, a hardware address segment)
    unsigned long* packetHead = (unsigned long*)(buf | 0x30000000);
    unsigned long* glyphOut = packetHead + 10;
    int nloop = 0;
    if (((&self->mViewportStack[0]))[self->mViewportDepth].mProjectionType != 0) {
        float vz = self->mScreenMatrix.row[2].z * z + self->mScreenMatrix.row[3].z;
        t4VectorInit cornerMin;
        t4VectorInit cornerMax;
        cornerMin.z = vz;
        cornerMin.w = 1.0f;
        cornerMax.z = vz;
        cornerMax.w = 1.0f;
        for (; *text; text++) {
            tGlyph* glyphRec = Find8808(font, (unsigned char)*text);
            if (glyphRec) {
                nloop++;
                float uv0[2];
                float uv1[2];
                Set2_8808(uv0, glyphRec->u * su, glyphRec->v * sv);
                Set2_8808(uv1, (glyphRec->u + glyphRec->width) * su, (glyphRec->v + glyphRec->height) * sv);
                cornerMin.x = (x + glyphRec->xoffset * sx) * self->mScreenMatrix.row[0].x + self->mScreenMatrix.row[3].x;
                cornerMin.y = (y + glyphRec->yoffset * sy) * self->mScreenMatrix.row[1].y + self->mScreenMatrix.row[3].y;
                cornerMax.x = cornerMin.x + glyphRec->width * sx * self->mScreenMatrix.row[0].x;
                cornerMax.y = cornerMin.y + glyphRec->height * sy * self->mScreenMatrix.row[1].y;
                glyphOut[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                glyphOut[1] = (int)cornerMin.x | ((long)(int)cornerMin.y << 16) | ((long)(int)cornerMin.z << 32);
                glyphOut[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                glyphOut[3] = (int)cornerMax.x | ((long)(int)cornerMax.y << 16) | ((long)(int)cornerMax.z << 32);
                glyphOut += 4;
                x += glyphRec->advance * sx;
            }
        }
    } else {
        for (; *text; text++) {
            tGlyph* glyphRec = Find8808(font, (unsigned char)*text);
            if (glyphRec) {
                nloop++;
                float uv0[2];
                float uv1[2];
                float size[2];
                Set2_8808(uv0, glyphRec->u * su, glyphRec->v * sv);
                Set2_8808(uv1, (glyphRec->u + glyphRec->width) * su, (glyphRec->v + glyphRec->height) * sv);
                Set2_8808(size, glyphRec->width * sx * 0.5f, glyphRec->height * sy * 0.5f);
                t4VectorRefInit pos(x + glyphRec->xoffset * sx + size[0], y + glyphRec->yoffset * sy + size[1], z,
                    1.0f);
                t4VectorInit projCentre = MulMat7278((&self->mScreenWorld), pos);
                float inv = 1.0f / projCentre.w;
                projCentre.x *= inv;
                projCentre.y *= inv;
                float dx = self->mScreenMatrix.row[0].x * size[0] * inv;
                projCentre.w = inv;
                float dy = self->mScreenMatrix.row[1].y * size[1] * inv;
                t4VectorInit halfExtent(dx, dy, 0.0f, 0.0f);
                t4VectorInit cornerMin = Sub_CF0(projCentre, halfExtent);
                t4VectorInit cornerMax = Add7278(projCentre, halfExtent);
                glyphOut[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                glyphOut[1] = (int)cornerMin.x | ((long)(int)cornerMin.y << 16) | ((long)(int)(cornerMin.z * cornerMin.w) << 32);
                glyphOut[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                glyphOut[3] = (int)cornerMax.x | ((long)(int)cornerMax.y << 16) | ((long)(int)(cornerMax.z * cornerMax.w) << 32);
                glyphOut += 4;
                x += glyphRec->advance * sx;
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
    cPS2DeferManager* ring = self->mpDeferMan;
    sDrawState* rs = self->m_pCurrentDrawState;
    self->mVifCursor = next + 0x10;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = ((int)rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7)
            + ((unsigned int)ring->mSubmits | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
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




extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern int D_004A4474;
typedef int cQuad128_CF0 __attribute__((mode(TI)));

// PORT: PS2-only VU0 inline asm (vector subtract); the PC port needs a C fallback.







// Homogeneous position (w given) with a const-reference ctor: its stores, w included, go straight to the stack
// slot, where the by-value t4VectorInit ctor keeps `this` in a register for a constant w.
struct sPtPos9028 : t4VectorInit {
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
static inline tGlyph* Find9028(cFont* f, int c)
{
    if (c < f->FirstCode)
        return 0;
    if (c <= f->LastLinearCode)
        return (f->Glyphs) + c - f->FirstCode;
    int lo = f->LastLinearCode - f->FirstCode;
    int hi = f->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* p = f->Glyphs + mid;
        if (c == p->code)
            return p;
        if (c < p->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (f->MissingGlyph);
}

/**
 * Draws the 16-bit character string @p wideText in @p font as one textured sprite per glyph, queued as a single GS packet.
 *
 * The wide-character twin of cPS2GraphicsMan_DrawString3D (slot 80). Rebuilds the matrices first (cPS2GraphicsMan_computeMatrices) when
 * mMatricesValid is 0. Each character's glyph is found by code (direct index from the font's first to last linear code,
 * then a binary search, else the default glyph). When the current viewport is orthographic (word +0x20 of
 * mViewportStack[mViewportDepth] non-zero) the corners are scaled and offset by mScreenMatrix directly; otherwise each
 * glyph's centre goes through mScreenWorld with a perspective divide and its half-size is scaled by 1/w. UVs are the
 * glyph's u/v times @p su and @p sv, and @p x advances by the glyph advance times @p sx.
 *
 * The packet at mVifCursor (uncached) holds a DMA cnt tag, a VIF DIRECT code, PRIM 0x56 (sprite, textured, blended)
 * with RGBAQ R, G, B = color[1..3] * 204, A = color[0] * 128, Q = 1, and a REGLIST GIF tag of ST/XYZ2 pairs. Then,
 * while mpDeferMan holds fewer than 2600 entries, a 0x80-byte entry is appended with the current draw state (low 5 bits
 * of its third word from the manager's mActivePortIndex), the packet's start and end and the view index
 * mActiveViewIndex. mVifCursor moves past the packet. Slot 81 of cPS2GraphicsMan_vtbl.
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
extern "C" void cPS2GraphicsMan_DrawString3D1(cPS2GraphicsMan* self, cFont* font, unsigned short* wideText, float* color,
                              float x, float y, float sx, float sy, float su, float sv, float z)
{
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    unsigned int buf = self->mVifCursor;
    unsigned long* packetHead = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
    unsigned long* packetCursor = packetHead + 10;
    int nloop = 0;
    if (((&self->mViewportStack[0]))[self->mViewportDepth].mProjectionType != 0) {
        float vz = self->mScreenMatrix.row[2].z * z + self->mScreenMatrix.row[3].z;
        t4VectorInit topLeft;
        t4VectorInit bottomRight;
        topLeft.z = vz;
        topLeft.w = 1.0f;
        bottomRight.z = vz;
        bottomRight.w = 1.0f;
        for (; *wideText; wideText++) {
            tGlyph* glyphInfo = Find9028(font, *wideText);
            if (glyphInfo) {
                nloop++;
                float uv0[2];
                float uv1[2];
                Set2_9028(uv0, glyphInfo->u * su, glyphInfo->v * sv);
                Set2_9028(uv1, (glyphInfo->u + glyphInfo->width) * su, (glyphInfo->v + glyphInfo->height) * sv);
                topLeft.x = (x + glyphInfo->xoffset * sx) * self->mScreenMatrix.row[0].x + self->mScreenMatrix.row[3].x;
                topLeft.y = (y + glyphInfo->yoffset * sy) * self->mScreenMatrix.row[1].y + self->mScreenMatrix.row[3].y;
                bottomRight.x = topLeft.x + glyphInfo->width * sx * self->mScreenMatrix.row[0].x;
                bottomRight.y = topLeft.y + glyphInfo->height * sy * self->mScreenMatrix.row[1].y;
                packetCursor[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                packetCursor[1] = (int)topLeft.x | ((long)(int)topLeft.y << 16) | ((long)(int)topLeft.z << 32);
                packetCursor[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                packetCursor[3] = (int)bottomRight.x | ((long)(int)bottomRight.y << 16) | ((long)(int)bottomRight.z << 32);
                packetCursor += 4;
                x += glyphInfo->advance * sx;
            }
        }
    } else {
        for (; *wideText; wideText++) {
            tGlyph* glyphInfo = Find9028(font, *wideText);
            if (glyphInfo) {
                nloop++;
                float uv0[2];
                float uv1[2];
                float size[2];
                Set2_9028(uv0, glyphInfo->u * su, glyphInfo->v * sv);
                Set2_9028(uv1, (glyphInfo->u + glyphInfo->width) * su, (glyphInfo->v + glyphInfo->height) * sv);
                Set2_9028(size, glyphInfo->width * sx * 0.5f, glyphInfo->height * sy * 0.5f);
                sPtPos9028 pos(x + glyphInfo->xoffset * sx + size[0], y + glyphInfo->yoffset * sy + size[1], z, 1.0f);
                t4VectorInit screenCentre = MulMat7278((&self->mScreenWorld), pos);
                float inv = 1.0f / screenCentre.w;
                screenCentre.x *= inv;
                screenCentre.y *= inv;
                float dx = self->mScreenMatrix.row[0].x * size[0] * inv;
                screenCentre.w = inv;
                float dy = self->mScreenMatrix.row[1].y * size[1] * inv;
                t4VectorInit halfExtent(dx, dy, 0.0f, 0.0f);
                t4VectorInit topLeft = Sub_CF0(screenCentre, halfExtent);
                t4VectorInit bottomRight = Add7278(screenCentre, halfExtent);
                packetCursor[0] = Bits_CF0(uv0[0]) | ((long)Bits_CF0(uv0[1]) << 32);
                packetCursor[1] = (int)topLeft.x | ((long)(int)topLeft.y << 16) | ((long)(int)(topLeft.z * topLeft.w) << 32);
                packetCursor[2] = Bits_CF0(uv1[0]) | ((long)Bits_CF0(uv1[1]) << 32);
                packetCursor[3] = (int)bottomRight.x | ((long)(int)bottomRight.y << 16) | ((long)(int)(bottomRight.z * bottomRight.w) << 32);
                packetCursor += 4;
                x += glyphInfo->advance * sx;
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
    unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, packetCursor, 4);
    cPS2DeferManager* ring = self->mpDeferMan;
    sPtRenderState* rs = ((sPtRenderState*)self->m_pCurrentDrawState);
    self->mVifCursor = next + 0x10;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtRingEnt* deferEntry = (sPtRingEnt*)((ring->mNumSubmitDMA++ << 7)
            + ((unsigned int)ring->mSubmits | 0x30000000));
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

struct sPtEnt_9860 {
    sDrawState rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};




extern int D_004A4380;
extern int D_004A4474;
extern char D_12B0[];
extern char D_12B8[];
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

static inline void QueueRecord_9860(cPS2GraphicsMan* self, unsigned int buf, unsigned int next, int flag)
{
    cPS2DeferManager* ring = self->mpDeferMan;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 0xA28) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_9860* e = (sPtEnt_9860*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_9860*)&ring->mSubmits[0]) | 0x30000000));
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
 * vertices back so the strip stays connected. Packets that would overflow the buffer are flushed with cPS2ScratchPad_flushBufferTo.
 * The bool argument On Tour has (isstatic) is not read.
 *
 * @param numVerts  the number of vertices (On Tour numverts)
 * @param src  the vertices
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
void cPS2GraphicsMan::RenderTriStrip(int numVerts, tBxVertex* src, int)
{
    unsigned int prog;
    if (D_004A4380) {
        prog = (unsigned int)D_12B0 >> 3;
    } else {
        prog = (unsigned int)D_12B8 >> 3;
    }
    D_004A4380 = 0;
    m_pCurrentDrawState->m_bitGroups[0] = (((int)(m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0xC0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    int prim;
    unsigned long regs;
    if (m_pCurrentDrawState->m_textureState.m_texid[0] >= 0) {
        prim = 92;
        regs = 1042;
    } else {
        prim = 76;
        regs = 1055;
    }
    int dma = D_004A4474;
    unsigned int buf = mVifCursor;
    unsigned long* packet = cPS2ScratchPad_claimBuffer(dma, buf, -1, 0);
    int qwc = ((cPS2ScratchPad*)dma)->mBlockSize;
    unsigned long* end = (unsigned long*)((char*)packet + qwc * 16);
    for (; numVerts >= 3; ) {
        int k = numVerts < 73 ? numVerts : 72;
        int vertQwords = k * 3;
        numVerts -= k;
        if (packet + (vertQwords + 5) * 2 >= end) {
            int sz = qwc * 16;
            unsigned int newBufAddr = cPS2ScratchPad_flushBufferTo(dma, packet, 4);
            mVifCursor = newBufAddr;
            packet = cPS2ScratchPad_claimBuffer(dma, newBufAddr, -1, 0);
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
        tBxVertex* dstVert = (tBxVertex*)packet;
        int i = k;
        while (i--) {
            *dstVert++ = *src++;
        }
        numVerts += 2;
        src -= 2;
        packet = (unsigned long*)((tBxVertex*)packet + k);
        packet[0] = 0;
        packet[1] = (unsigned long)(prog | 0x14000000) << 32;
        packet += 2;
    }
    unsigned int next = cPS2ScratchPad_flushBufferTo(dma, packet, 4);
    mVifCursor = next;
    QueueRecord_9860(this, buf, next, 0);
    mVifCursor += 0x10;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
}
#endif

INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderIndexedTriList);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_beginTriStripPacket);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 4x4 float matrix (POD, 16-byte aligned).
struct sPtMatrix_A260 {
    float m[4][4];
} __attribute__((aligned(16)));

// 4x4 float matrix with an empty ctor and a VU0 copy ctor (copies go through vf1..vf4).
// g++ 2.95 vtable entry for a method taking (self, pointer).
struct sPtVtVP_A260 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);

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
 * the matrices (cPS2GraphicsMan_computeMatrices) when mMatricesValid is 0. At mVifCursor (remembered in mStripPacketStart, the packet
 * cursor in mStripCursor) it writes a DMA cnt tag (n * 3 + 4 qwords), a VIF UNPACK header and a PACKED GIF tag for @p n
 * loops of three registers: PRIM 0x5C (Gouraud, textured, blended tri strip) with ST, RGBAQ, XYZ2 when the draw state
 * has a texture, else PRIM 0x4C with NOP, RGBAQ, XYZ2. After the n * 3 quadwords of vertex space it adds a VIF MSCAL of
 * the VU1 program at D_12B0 when the flag D_004A4380 is set (clearing it) or D_12B8 otherwise. cPS2GraphicsMan_endTriStripPacket closes the
 * packet. cSpeedBoostFX_render uses the pair.
 *
 * @param n  the number of vertices
 * @return the vertex area: @p n entries of three quadwords (ST or unused, RGBAQ, XYZ2)
 */
// PORT: PS2-only GS packet build (DMA tag + GIF tag + PRIM/TEX registers).
unsigned long* cPS2GraphicsMan::beginTriStripPacket(int n)
{
    unsigned int addr = (D_004A4380 ? (unsigned int)D_12B0 : (unsigned int)D_12B8) >> 3;
    int primType;
    long regList;
    D_004A4380 = 0;
    ((sPtRenderState*)m_pCurrentDrawState)->m_bitGroups[0]
            = (((sPtRenderState*)m_pCurrentDrawState)->m_bitGroups[0] & ~0x3C0) | 0xC0;
    ((sPtRenderState*)m_pCurrentDrawState)->m_bitGroups[3] &= 0xC0000000;
    if (mMatricesValid == 0) {
        computeMatrices();
    }
    if (((sPtRenderState*)m_pCurrentDrawState)->tex >= 0) {
        primType = 92;
        regList = 1042;
    } else {
        primType = 76;
        regList = 1055;
    }
    unsigned int buf = mVifCursor;
    mStripPacketStart = buf;
    (*(unsigned long**)&mStripCursor) = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
    ((unsigned long*)mStripCursor)[0] = (n * 3 + 4) | 0x10000000;
    ((unsigned long*)mStripCursor)[1] = 0;
    ((unsigned long*)mStripCursor)[2] = 0;
    ((unsigned long*)mStripCursor)[3] = (unsigned long)((n * 3 + 2) << 16 | 0x6C008000U) << 32;
    ((unsigned long*)mStripCursor)[4] = (unsigned long)(n | 0x8000) | gsPrim_A260(primType)
            | ((unsigned long)0xC000 << 46);
    ((unsigned long*)mStripCursor)[5] = regList;
    ((unsigned long*)mStripCursor)[6] = (long)n * 3;
    ((unsigned long*)mStripCursor)[7] = 0;
    unsigned long* verts = ((unsigned long*)mStripCursor) + 8;
    (*(unsigned long**)&mStripCursor) = verts + n * 6;
    ((unsigned long*)mStripCursor)[0] = 0;
    ((unsigned long*)mStripCursor)[1] = (unsigned long)(addr | 0x14000000) << 32;
    (*(unsigned long**)&mStripCursor) += 2;
    return verts;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_endTriStripPacket);
#ifdef SKIP_ASM

struct sPtEnt_A430 {
    sDrawState rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};



extern int D_004A4474;
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);

/**
 * Closes the packet opened by cPS2GraphicsMan_beginTriStripPacket and queues it in the deferred draw list.
 *
 * Ends the packet at mStripCursor with cPS2ScratchPad_flushBufferTo (DMA channel D_004A4474) and moves mVifCursor there. While
 * mpDeferMan holds fewer than 2600 entries it appends a 0x80-byte entry with the current draw state (low 5 bits of its
 * third word from the manager's mActivePortIndex), the packet's start (mStripPacketStart) and end and the view index
 * mActiveViewIndex. Then mVifCursor skips 0x10 bytes, bits 6-9 of the draw state's first word and the low 30 bits of
 * its fourth word are cleared, and mStripPacketStart/mStripCursor are reset to 0. cSpeedBoostFX_render calls it after
 * filling the vertices.
 */
// PORT: uncached (0x30000000) pointers held in int.
void cPS2GraphicsMan::endTriStripPacket()
{
    unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, ((unsigned long*)mStripCursor), 4);
    mVifCursor = next;
    unsigned int buf = mStripPacketStart;
    cPS2DeferManager* ring = mpDeferMan;
    sDrawState* rs = m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_A430* deferEntry = (sPtEnt_A430*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_A430*)&ring->mSubmits[0]) | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = buf;
        deferEntry->next = next;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
    mVifCursor += 0x10;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    mStripPacketStart = 0;
    mStripCursor = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_CreateGridMesh);
#ifdef SKIP_ASM



extern "C" cGridMesh* cPS2GridMesh_cPS2GridMesh(void* mem, void* a1, void* a2, void* a3, unsigned int flags);
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
extern "C" cGridMesh* cPS2GraphicsMan_CreateGridMesh(cPS2GraphicsMan* pS2GraphicsMan, void* width, void* height, unsigned int flags)
{
    cGridMesh* gridMesh = cPS2GridMesh_cPS2GridMesh(cMemMan_alloc(460, str_cPS2GridMesh, flags, 0), width, height, pS2GraphicsMan, flags);
    if (flags & 0x40000000) {
        if (gridMesh == 0) {
            return 0;
        }
        sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)((void*)gridMesh->mVtbl);
        if (vt[2].fn(vthis(gridMesh, vt[2])) == 0) {
            sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)((void*)gridMesh->mVtbl);
            vt2[1].fn(vthis(gridMesh, vt2[1]), 3);
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
extern int cRenderToggles_m_DisablePlayerStrips;
extern int cRenderToggles_m_DisableCharacterSpecular;



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


extern "C" void cPS2GraphicsMan_setTextureFunction_A610(void* self, int tex, int mode) __asm__("cPS2GraphicsMan_setTextureFunction__FPv");

// PORT: PS2-only VU0 inline asm (matrix * vector, clamp, depth read back from vi1).
static inline int Depth_A610(cPS2GraphicsMan* self, t4Vector v)
{
    long r;
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
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
        : "m"(*(sPtQuadF*)&self->mSortZScale), "m"(v), "r"((&self->mNetMatrix))
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

static inline void SetTest_A610(cPS2GraphicsMan* self)
{
    if (cRenderToggles_m_DisableCharacterSpecular == 0) {
        SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 7, 5, 0x11);
    } else {
        SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 7, 5, 3);
    }
}

static inline void SetTex_A610(cPS2GraphicsMan* self, int t)
{
    self->m_pCurrentDrawState->m_textureState.m_texid[0] = t;
}

static inline void SetTex2_A610(cPS2GraphicsMan* self, int t)
{
    self->m_pCurrentDrawState->m_textureState.m_texid[1] = t;
}

#define MIN_A610(a, b) ((a) < (b) ? (a) : (b))

/**
 * Draws a skinned character model.
 *
 * Unless cRenderToggles_m_DisablePlayerStrips is set: pushes the draw state, starts the batch with slot 124 (cPS2GraphicsMan_InitCharMatrices, InitCharMatrices) and
 * walks the model's chunks, binding each chunk's texture and emitting its geometry with the bone matrices. With @p
 * shadow set it changes the draw state for the shadow pass (alpha and Z-test fields) and skips the per-chunk material
 * flags.
 *
 * @param mdl  the model header
 * @param base  the model data the header's offsets are relative to
 * @param shadow  non-zero for the shadow pass
 */
// PORT: PS2-only DMA/VIF packet build; uncached (0x30000000) pointers held in int.
extern "C" void cPS2GraphicsMan_DrawCharModel(cPS2GraphicsMan* self, sMdl_A610* mdl, char* base, int shadow)
{
    if (cRenderToggles_m_DisablePlayerStrips == 0) {
        (self->m_pCurrentDrawState)[1] = *(self->m_pCurrentDrawState);
        (self->m_pCurrentDrawState)++;
        ((sVEntry<void(void*, float)>*)VPTR_OF(cGraphicsMan, self))[124].fn(vthis(self, ((sVEntry<void(void*, float)>*)VPTR_OF(cGraphicsMan, self))[124]), 1.0f);
        if (self->mMatricesValid == 0) {
            self->computeMatrices();
        }
        int matIdx = 0;
        sChunk_A610* chunk = (sChunk_A610*)(base + mdl->chunkOfs);
        t4Matrix* bones = ((t4Matrix*)self->mpScratchpad);
        SetTex2_A610(self, self->mCharSpecTexID[chunk->texSlot]);
        SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[0]), 4, 2, 3);
        int z = MIN_A610(0x400, Depth_A610(self, bones->row[3]) + 0x10);
        if (shadow) {
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[0]), 20, 8, 0);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 2, 5, 7);
            SetTest_A610(self);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 0, 2, 2);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[2]), 10, 19, z);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 23, 2, 1);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 20, 2, 0);
            SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 12, 8, 20);
        }
        for (int i = 0; i < mdl->numChunks; i++) {
            cPS2GraphicsMan_setTextureFunction_A610(self, chunk->tex, 3);
            SetTex_A610(self, chunk->tex);
            if (!shadow) {
                if (chunk->flags & 0x8000) {
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 2, 5, 5);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[0]), 20, 8, chunk->alpha);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 7, 5, 0xC);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 0, 2, 2);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[2]), 10, 19, z);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 23, 2, 1);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 20, 2, 3);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 12, 8, 92);
                } else {
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 2, 5, 1);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[0]), 20, 8, 0);
                    SetTest_A610(self);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 0, 2, 0);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[2]), 10, 19, 0);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 23, 2, 1);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 20, 2, 0);
                    SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[1]), 12, 8, 20);
                }
            }
            if (chunk->flags & 1) {
                unsigned int buf = self->mVifCursor;
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
                cPS2DeferManager* ring = self->mpDeferMan;
                self->mVifCursor = next;
                sDrawState* rs = self->m_pCurrentDrawState;
                if (ring->mNumSubmitDMA < 2600) {
                    rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
                    int key = ring->mActiveViewIndex;
                    sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->mNumSubmitDMA++ << 7)
                        + ((unsigned int)ring->mSubmits | 0x30000000));
                    *(sDrawState*)&ringEntry->rs = *rs;
                    ringEntry->buf = buf;
                    ringEntry->next = next;
                    ringEntry->key = key;
                    ringEntry->flag = 0;
                }
                self->mVifCursor += 0x10;
            } else if (chunk->flags & 0x100) {
                unsigned int buf = self->mVifCursor;
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
                cPS2DeferManager* ring = self->mpDeferMan;
                self->mVifCursor = next;
                sDrawState* rs = self->m_pCurrentDrawState;
                if (ring->mNumSubmitDMA < 2600) {
                    rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
                    int key = ring->mActiveViewIndex;
                    sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->mNumSubmitDMA++ << 7)
                        + ((unsigned int)ring->mSubmits | 0x30000000));
                    *(sDrawState*)&ringEntry->rs = *rs;
                    ringEntry->buf = buf;
                    ringEntry->next = next;
                    ringEntry->key = key;
                    ringEntry->flag = 0;
                }
                self->mVifCursor += 0x10;
            }
            matIdx += chunk->numSubs;
            chunk++;
        }
        (self->m_pCurrentDrawState)--;
        SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[0]), 6, 4, 0);
        SetField_A610(((int)(self->m_pCurrentDrawState)->m_bitGroups[3]), 0, 30, 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawCharShadowModel);
#ifdef SKIP_ASM
struct sPtRS_B548 {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words)
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


struct sMat_B548 {
    float m[4][4];
} __attribute__((aligned(16)));


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
    int bonesOffset;                // 0x20 [U] the bone table's offset from the model data
                                    //      (cPS2GraphicsMan_ResolveCharModel)
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

static inline void QueueRecord_B548(cPS2GraphicsMan* self, unsigned int buf, unsigned int next, int flag)
{
    cPS2DeferManager* ring = self->mpDeferMan;
    sPtRS_B548* rs = ((sPtRS_B548*)self->m_pCurrentDrawState);
    if (ring->mNumSubmitDMA < 0xA28) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_B548* e = (sPtEnt_B548*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_B548*)&ring->mSubmits[0]) | 0x30000000));
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
extern "C" void cPS2GraphicsMan_DrawCharShadowModel(cPS2GraphicsMan* self, sMdl_B548* model, char* base, float* col)
{
    unsigned int buf = self->mVifCursor;
    unsigned long* packet = (unsigned long*)(buf | 0x30000000);
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[0] |= 0x30;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1] & ~0x7C) | 4;
    int cnt = 0;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1] & ~0xF80) | 0x580;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[0]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[0] & 0xF00FFFFF) | 0x06400000;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1] & 0xFE7FFFFF) | 0x01000000;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1] & 0xFFCFFFFF) | 0x00300000;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1]
            = (((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[1] & 0xFFF00FFF) | 0x00014000;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->tex = -1;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->tex2 = -1;
    sBone_B548* bone = (sBone_B548*)(base + model->bonesOffset);
    for (int i = 0; i < model->nbones; i++, bone++) {
        if (bone->flags & 1) {
            packet[0] = 0x10000003;
            BoneHeader_B548(&packet, col);
            sTrk_B548* tr = &model->tracks[cnt];
            for (int j = 0; j < bone->ntracks; j++, tr++) {
                AddMats_B548(&packet, ((sMat_B548*)self->mpScratchpad), tr->n, tr->idx);
                packet[0] = ((unsigned long)*bone->refs[j].p << 32) | 0x50000000;
                packet[1] = 0;
                packet += 2;
            }
        }
        cnt += bone->ntracks;
    }
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    self->mVifCursor = next;
    QueueRecord_B548(self, buf, next, 1);
    self->mVifCursor += 0x10;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[0] &= ~0x3C0;
    ((sPtRS_B548*)self->m_pCurrentDrawState)->m_bitGroups[3] &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetShadowCharMatrices);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only GS packet build; uncached (0x30000000) pointers held in int and 128-bit qword copies.
typedef int cQuad128_BA50 __attribute__((mode(TI)));

struct sRS_BA50 {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words)
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
    rs->m_bitGroups[0] = (rs->m_bitGroups[0] & ~0x3C0) | 0x1C0;
    sRS_BA50* rs2 = (sRS_BA50*)self->m_pCurrentDrawState;
    rs2->m_bitGroups[3] = (rs2->m_bitGroups[3] & 0xC0000000) | (buf & 0x3FFFFFFF);
}
#endif

INCLUDE_ASM("render/particle", func_0037BB10);

//100%
INCLUDE_ASM("render/particle", gs_calcBufferBlocks);
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
extern "C" int gs_calcBufferBlocks(void* self, int w, int height, int fbPsm, int texPsm, int zPsm)
{
    int bw = (w + 63) >> 6;
    int fb = pages_37BC40(fbPsm, bw, height);
    int tex = pages_37BC40(texPsm, bw, height);
    int z = zpages_37BC40(zPsm, bw, height);
    return (fb + tex + z) << 5;
}
#endif

//100%
INCLUDE_ASM("render/particle", gs_isZFormatCompatible);
#ifdef SKIP_ASM
/**
 * Tells whether colour format @p psmA (eGSPixelStorage) can be used with z format @p psmB.
 *
 * 32-bit, 24-bit and PSMCT16S colour (0, 1, 10) and PSMZ32/24/16S (48, 49, 58) go with PSMZ32, PSMZ24 or PSMZ16S;
 * PSMCT16 and PSMZ16 (2, 50) go only with PSMZ16. cPS2GraphicsMan_applyDisplayMode checks the draw-buffer and z formats with it.
 *
 * @param psmA  the colour (or z) format
 * @param psmB  the z format
 * @return true when the pair is allowed
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" bool gs_isZFormatCompatible(void* self, int psmA, int psmB)
{
    switch (psmA) {
    case kGS_PSMCT32:
    case kGS_PSMCT24:
    case kGS_PSMCT16S:
    case kGS_PSMZ32:
    case kGS_PSMZ24:
    case kGS_PSMZ16S:
        if (psmB == kGS_PSMZ32 || psmB == kGS_PSMZ24 || psmB == kGS_PSMZ16S) {
            return true;
        }
        break;
    case kGS_PSMCT16:
    case kGS_PSMZ16:
        if (psmB == kGS_PSMZ16) {
            return true;
        }
        break;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_applyDisplayMode);
#ifdef SKIP_ASM
extern "C" void FlushCache(int);
extern "C" void cTextureMan_freeAllHeapBlocks(int gs);
void* cTextureMan_initTempHeap(void* self, int a1, int a2);
extern "C" int cTextureMan_allocLockedPages(int gs, int size);
extern "C" void cPS2GraphicsMan_flipDisplayBuffers(cPS2GraphicsMan* self, int arg1, int arg2);
extern "C" void TIMER_setHblankRate(int a);
extern "C" void sceGsResetGraph(int a, int b, int c, int d);


/**
 * Sets a GS display mode: checks the formats, stores the display and frame buffer settings, lays out VRAM again and
 * resets the GS.
 *
 * Returns false at once when gs_isZFormatCompatible rejects @p psm2 with @p zpsm. Otherwise stores @p displayWidth x @p
 * displayHeight as the display size (mDisplayWidth, mDisplayHeight), @p w x @p bufferHeight as the frame buffer size
 * (mFBWidth, mFBHeight), @p psm as the display format (mDisplayFormat), @p psm2 as mCurrentFBFormat, @p zpsm as
 * mCurrentZBFormat, @p interlaceMode and @p fieldMode in mInterlace and mFieldMode, 2 in mOutputMode (the output mode)
 * and @p flag in mDTV; clears mCurrentFrameCount and the vertical magnification (mMagV), and sets the horizontal one
 * (mMagH) from the display width: 256 -> 9, 320 -> 7, 384 -> 6, 512 -> 4, 640 -> 3 (other widths leave it unchanged).
 * It computes the buffer widths in 64-pixel units (mFrameBufferWidth, mDisplayBufferWidth), the page counts of the
 * display, draw and z buffers (mDisplayPages, mDrawPages, mZBufferPages: pages of 64 x 32 pixels for 32/24-bit formats,
 * 64 x 64 for 16-bit ones) and the dither flag mDither (1 for a 16-bit display format).
 *
 * Then it empties the texture manager's VRAM heaps (cTextureMan_freeAllHeapBlocks), allocates the draw, display and z buffers in that
 * order (cTextureMan_allocLockedPages; their pages go to mDrawBufferPage, mDisplayBufferPage and mCurrentZBufferPage) and sets up the
 * texture manager's second heap (cTextureMan_initTempHeap): from the draw buffer over a1 * a2 + 2 * w * h words when both colour
 * formats are PSMCT24, from the display buffer over a1 * a2 + w * h when only the display format is, else from the z
 * buffer over w * h. Finally it flushes the data cache, waits for the next vertical sync on GS CSR (0x12001000), calls
 * sceGsResetGraph(@p resetMode, @p interlaceMode, @p flag ? 80 : 2, @p fieldMode) and TIMER_setHblankRate(@p flag ? 3 : 1),
 * and sets the display registers with cPS2GraphicsMan_flipDisplayBuffers(self, 0, 1). cPS2GraphicsMan_SetDisplayMode1 calls it.
 *
 * @param displayWidth  display width in pixels
 * @param displayHeight  display height in pixels
 * @param w  frame (draw) buffer width in pixels
 * @param bufferHeight  frame buffer height in pixels
 * @param psm  GS pixel format of the display buffer
 * @param psm2  GS pixel format of the draw buffer
 * @param zpsm  GS z format of the z buffer
 * @param resetMode  first argument of the GS reset call sceGsResetGraph
 * @param interlaceMode  interlace setting (mInterlace), passed on to sceGsResetGraph
 * @param fieldMode  field/frame setting (mFieldMode), passed on to sceGsResetGraph
 * @param flag  nonzero for output mode 80 (0x50) instead of 2 and TIMER_setHblankRate(3) instead of 1; kept in mDTV
 * @return true when the mode was set, false when the draw and z formats do not go together
 */
bool cPS2GraphicsMan::applyDisplayMode(unsigned int displayWidth, unsigned int displayHeight, unsigned int w,
                                       unsigned int bufferHeight, unsigned int psm, unsigned int psm2,
                                       unsigned int zpsm, short resetMode, short interlaceMode, short fieldMode,
                                       int flag)
{
    if (!gs_isZFormatCompatible((this), psm2, zpsm))
        return false;

    mDisplayHeight = (int)displayHeight;
    mFBWidth = (int)w;
    mFBHeight = (int)bufferHeight;
    mCurrentFBFormat = (int)psm2;
    mDisplayFormat = (int)psm;
    mCurrentZBFormat = (int)zpsm;
    mInterlace = interlaceMode;
    mFieldMode = fieldMode;
    mCurrentFrameCount = 0;
    mDisplayWidth = (int)displayWidth;
    mOutputMode = 2;
    mDTV = flag;
    mMagV = 0;

    switch (displayWidth) {
    case 256:
        mMagH = 9;
        break;
    case 320:
        mMagH = 7;
        break;
    case 384:
        mMagH = 6;
        break;
    case 512:
        mMagH = 4;
        break;
    case 640:
        mMagH = 3;
        break;
    }

    mFrameBufferWidth = (int)(((unsigned int)mFBWidth + 0x3F) >> 6);
    mDisplayBufferWidth = (int)(((unsigned int)mDisplayWidth + 0x3F) >> 6);

    switch ((unsigned int)mDisplayFormat) {
    case kGS_PSMCT32:
    case kGS_PSMCT24:
    case kGS_PSMZ32:
    case kGS_PSMZ24:
        mDither = 0;
        mDisplayPages = (unsigned int)mDisplayBufferWidth * (((unsigned int)mDisplayHeight
                + 0x1F) >> 5);
        break;
    case kGS_PSMCT16:
    case kGS_PSMCT16S:
    case kGS_PSMZ16:
    case kGS_PSMZ16S:
        mDither = 1;
        mDisplayPages = (unsigned int)mDisplayBufferWidth * (((unsigned int)mDisplayHeight
                + 0x3F) >> 6);
        break;
    }

    switch ((unsigned int)mCurrentFBFormat) {
    case kGS_PSMCT32:
    case kGS_PSMCT24:
    case kGS_PSMZ32:
    case kGS_PSMZ24:
        mDrawPages = (unsigned int)mFrameBufferWidth * (((unsigned int)mFBHeight + 0x1F) >> 5);
        break;
    case kGS_PSMCT16:
    case kGS_PSMCT16S:
    case kGS_PSMZ16:
    case kGS_PSMZ16S:
        mDrawPages = (unsigned int)mFrameBufferWidth * (((unsigned int)mFBHeight + 0x3F) >> 6);
        break;
    }

    switch ((unsigned int)mCurrentZBFormat) {
    case kGS_PSMZ32:
    case kGS_PSMZ24:
        mZBufferPages = (unsigned int)mFrameBufferWidth * (((unsigned int)mFBHeight + 0x1F) >> 5);
        break;
    case kGS_PSMZ16:
    case kGS_PSMZ16S:
        mZBufferPages = (unsigned int)mFrameBufferWidth * (((unsigned int)mFBHeight + 0x3F) >> 6);
        break;
    }

    int gs = (int)mTexMan;
    cTextureMan_freeAllHeapBlocks(gs);
    mDrawBufferPage = cTextureMan_allocLockedPages(gs, mDrawPages << 13);
    mDisplayBufferPage = cTextureMan_allocLockedPages(gs, mDisplayPages << 13);
    mCurrentZBufferPage = cTextureMan_allocLockedPages(gs, mZBufferPages << 13);

    // PORT: reads psm and psm2 as one 64-bit word.
    if (*(long*)(unsigned int*)&mDisplayFormat == ((long)kGS_PSMCT24 << 32 | kGS_PSMCT24))
        cTextureMan_initTempHeap((void*)(int)mTexMan, mDrawBufferPage << 11,
                (unsigned int)mDisplayWidth * (unsigned int)mDisplayHeight
                        + (unsigned int)mFBWidth * (unsigned int)mFBHeight * 2);
    else if ((unsigned int)mDisplayFormat == kGS_PSMCT24)
        cTextureMan_initTempHeap((void*)(int)mTexMan, mDisplayBufferPage << 11,
                (unsigned int)mDisplayWidth * (unsigned int)mDisplayHeight
                        + (unsigned int)mFBWidth * (unsigned int)mFBHeight);
    else
        cTextureMan_initTempHeap((void*)(int)mTexMan, mCurrentZBufferPage << 11, (unsigned int)mFBWidth * (unsigned int)mFBHeight);

    FlushCache(0);
    *(volatile long*)0x12001000 = 8;
    while (!(*(volatile long*)0x12001000 & 8)) {
    }
    *(volatile long*)0x12001000;

    sceGsResetGraph(resetMode, mInterlace, flag ? 80 : mOutputMode, mFieldMode);
    TIMER_setHblankRate(flag ? 3 : 1);
    flipDisplayBuffers(0, 1);
    return true;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setDitherMatrix);
#ifdef SKIP_ASM
// The GS DIMX register (names: the GS manual's DIMX fields DM00 .. DM33, each followed by a reserved bit).
struct sPtBits_C198 {
    unsigned long DM00 : 3;
    unsigned long DM00Reserved : 1;
    unsigned long DM01 : 3;
    unsigned long DM01Reserved : 1;
    unsigned long DM02 : 3;
    unsigned long DM02Reserved : 1;
    unsigned long DM03 : 3;
    unsigned long DM03Reserved : 1;
    unsigned long DM10 : 3;
    unsigned long DM10Reserved : 1;
    unsigned long DM11 : 3;
    unsigned long DM11Reserved : 1;
    unsigned long DM12 : 3;
    unsigned long DM12Reserved : 1;
    unsigned long DM13 : 3;
    unsigned long DM13Reserved : 1;
    unsigned long DM20 : 3;
    unsigned long DM20Reserved : 1;
    unsigned long DM21 : 3;
    unsigned long DM21Reserved : 1;
    unsigned long DM22 : 3;
    unsigned long DM22Reserved : 1;
    unsigned long DM23 : 3;
    unsigned long DM23Reserved : 1;
    unsigned long DM30 : 3;
    unsigned long DM30Reserved : 1;
    unsigned long DM31 : 3;
    unsigned long DM31Reserved : 1;
    unsigned long DM32 : 3;
    unsigned long DM32Reserved : 1;
    unsigned long DM33 : 3;
    unsigned long DM33Reserved : 1;
};

struct sPtGfx_C198 {
    char pad_0x0[0x59E0];
    sPtBits_C198 bits;              // 0x59E0
};

/**
 * Packs a 4 x 4 dither matrix into the GS DIMX register value mDitherMatrix.
 *
 * Entry n of @p src goes into bits 4n to 4n + 2 and bit 4n + 3 is cleared (DIMX holds sixteen 3-bit entries, DM00 to
 * DM33, one nibble apart). cPS2GraphicsMan_Init calls it.
 *
 * @param src  the 16 dither values, row by row (only the low 3 bits are kept)
 */
// PORT: 64-bit `long` bitfields (the GS-style register word).
void cPS2GraphicsMan::setDitherMatrix(int* src)
{
    (*(sPtBits_C198*)&mDitherMatrix).DM00 = src[0];
    (*(sPtBits_C198*)&mDitherMatrix).DM01 = src[1];
    (*(sPtBits_C198*)&mDitherMatrix).DM02 = src[2];
    (*(sPtBits_C198*)&mDitherMatrix).DM03 = src[3];
    (*(sPtBits_C198*)&mDitherMatrix).DM10 = src[4];
    (*(sPtBits_C198*)&mDitherMatrix).DM11 = src[5];
    (*(sPtBits_C198*)&mDitherMatrix).DM12 = src[6];
    (*(sPtBits_C198*)&mDitherMatrix).DM13 = src[7];
    (*(sPtBits_C198*)&mDitherMatrix).DM20 = src[8];
    (*(sPtBits_C198*)&mDitherMatrix).DM21 = src[9];
    (*(sPtBits_C198*)&mDitherMatrix).DM22 = src[10];
    (*(sPtBits_C198*)&mDitherMatrix).DM23 = src[11];
    (*(sPtBits_C198*)&mDitherMatrix).DM30 = src[12];
    (*(sPtBits_C198*)&mDitherMatrix).DM31 = src[13];
    (*(sPtBits_C198*)&mDitherMatrix).DM32 = src[14];
    (*(sPtBits_C198*)&mDitherMatrix).DM33 = src[15];
    (*(sPtBits_C198*)&mDitherMatrix).DM00Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM01Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM02Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM03Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM10Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM11Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM12Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM13Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM20Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM21Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM22Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM23Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM30Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM31Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM32Reserved = 0;
    (*(sPtBits_C198*)&mDitherMatrix).DM33Reserved = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setBrightnessColour);
#ifdef SKIP_ASM
extern "C" int fptoui(float f);

// PORT: g++ `<?` (min) operator; needs a macro / std::min on a modern compiler.
static inline float clamp_C570(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}


/**
 * Sets the screen brightness colour: mBrightnessColour[0..2] = @p colour's r, g and b, each clamped to 0..1 and times
 * 128, converted with fptoui (float to unsigned).
 *
 * cPS2GraphicsMan_flipDisplayBuffers multiplies these into the colour of the sprites that copy the draw buffer to the display buffer.
 * cPS2GraphicsMan_Init calls it.
 *
 * @param colour  the brightness per channel (1.0 = full)
 */
void cPS2GraphicsMan::setBrightnessColour(tRGB* colour)
{
    float one = 1.0f;
    mBrightnessColour[0] = fptoui(clamp_C570(colour->R, one) * 128.0f);
    mBrightnessColour[1] = fptoui(clamp_C570(colour->G, one) * 128.0f);
    mBrightnessColour[2] = fptoui(clamp_C570(colour->B, one) * 128.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_getBrightnessColour);
#ifdef SKIP_ASM
struct sRGB_C638 {
    float x, y, z;
    sRGB_C638() {}
    sRGB_C638(const sRGB_C638& o) { x = o.x; y = o.y; z = o.z; }
};

/**
 * Returns the screen brightness colour: mBrightnessColour[0..2] divided by 128 (the inverse of cPS2GraphicsMan_setBrightnessColour).
 *
 * @return the r, g and b brightness, 1.0 = full
 */
extern "C" sRGB_C638 cPS2GraphicsMan_getBrightnessColour(cPS2GraphicsMan* self)
{
    sRGB_C638 colour;
    colour.x = (float)(unsigned int)self->mBrightnessColour[0] * 0.0078125f;
    colour.y = (float)(unsigned int)self->mBrightnessColour[1] * 0.0078125f;
    colour.z = (float)(unsigned int)self->mBrightnessColour[2] * 0.0078125f;
    return colour;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setScreenFade);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator; needs a macro / std::min on a modern compiler.
static inline float clamp_C720(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

/**
 * Sets the screen alpha mScreenAlpha to (1 - @p alpha clamped to 0..1) * 128, converted with fptoui.
 *
 * cPS2GraphicsMan_flipDisplayBuffers uses it as the alpha of the sprites that copy the draw buffer to the display buffer and turns blending
 * on when it is not 128. cPS2GraphicsMan_Init and cSpeedBoostFX__cSpeedBoostFX (with 0, i.e. 128) call it.
 *
 * @param alpha  the amount to fade the screen (0 = none, 1 = full)
 */
void cPS2GraphicsMan::setScreenFade(float alpha)
{
    mScreenAlpha = fptoui((1.0f - clamp_C720(alpha, 1.0f)) * 128.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_getScreenFade);
#ifdef SKIP_ASM
/**
 * Returns the screen fade set by cPS2GraphicsMan_setScreenFade: 1 - mScreenAlpha / 128.
 *
 * @return the fade amount (0 = none)
 */
float cPS2GraphicsMan::getScreenFade()
{
    unsigned int rawAlpha = (unsigned int)mScreenAlpha;
    return 1.0f - (float)rawAlpha * 0.0078125f;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_InitTextures);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
void cTextureMan_SetTextureMemType(void* p, int a1);

/**
 * Clears the texture table mTextureAdded (0x1F40 bytes) and sets the texture manager's memory type
 * (cTextureMan_SetTextureMemType).
 *
 * @param memtype  the memory type for textures (On Tour memtype)
 */
void cPS2GraphicsMan::InitTextures(int memtype)
{
    MEM_fill(&mTextureAdded[0], 0, 8000);
    cTextureMan_SetTextureMemType(mTexMan, memtype);
}
#endif

extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PlatformUploadTextures__FPv);
#ifdef SKIP_ASM
/**
 * Makes the staged texture table current.
 *
 * MEM_copy()s 8000 bytes (0x1F40) from mTextureAdded to mTextureUploaded. Called by cGraphicsMan_UploadTextures()
 * through slot 121.
 */
void cPS2GraphicsMan::PlatformUploadTextures()
{
    MEM_copy(&mTextureUploaded[0], &mTextureAdded[0], 8000);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_PlatformPurgeTextures);
#ifdef SKIP_ASM

/**
 * Removes every loaded texture: for each of the 2000 slots with a non-zero mTextureUploaded entry calls virtual slot 50
 * (the remove) with its index.
 */
void cPS2GraphicsMan::PlatformPurgeTextures()
{
    int i;
    for (i = 0; i < 2000; i++) {
        if (mTextureUploaded[i] != 0) {
            RemoveTexture(i);
        }
    }
}
#endif

// declared by mangled name so we can call it with a single argument, the way
// the target does (its real signature takes a second arg the caller leaves set)
extern "C" void cTextureMan_SetTextureMemType__FPvi(void*);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetTextureMemType);
#ifdef SKIP_ASM
/**
 * Passes a memory type to the texture manager (cTextureMan_SetTextureMemType).
 *
 * Declared with only the this pointer: On Tour's SetTextureMemType(int memtype) passes the type, which stays in $a1
 * and reaches cTextureMan_SetTextureMemType.
 */
void cPS2GraphicsMan::SetTextureMemType()
{
    cTextureMan_SetTextureMemType__FPvi(mTexMan);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddTexture);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" double fptodp(float f);
extern "C" double dlog_C8C0(double) __asm__("log");
extern "C" double dpmul(double, double);
extern "C" double dpsub(double, double);
extern "C" int dtoi_C8C0(double) __asm__("dptoli");
extern "C" void cPS2GraphicsMan_setTextureFilter_C8C0(void* self, int h, int a, int b, int c, int d) __asm__("cPS2GraphicsMan_setTextureFilter__FPv");

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
 * The record is On Tour's SHAPE: type is the pixel format and next the offset to the next record (the palette);
 * flags bit 0x1000 means the texels sit at dataoffset (else right after the 16-byte header), bit 0x2000 picks the
 * swizzled format, and its top four bits are the mipmap count. Type kSHAPE_PAL4 gives texel format 11 (12 when
 * swizzled) and kSHAPE_PAL8 gives 9 (10), both with CLUT format 1 and the palette record's texels; kSHAPE_RGB15 gives 2
 * and kSHAPE_RGB32 gives 0, without a CLUT. A palettised record whose offset is 0 would make the palette record null
 * and be dereferenced.
 *
 * For a mipmapped record with a non-zero centerx it then computes k = -16 * log2(240 / (centerx / 16384)), clamped to
 * -2047..-135, and calls cPS2GraphicsMan_setTextureFilter(this, id, 5, 1, 0, k). Slot 46 of cPS2GraphicsMan_vtbl.
 *
 * @param pShp  the shape record
 * @param textureName  the texture name, passed through
 * @param lockType  the lock type, passed through
 * @param copyMem  the copy-memory flag, passed through
 * @param bindId  the bind id, passed through
 * @return the texture id slot 47 returns
 */
int cPS2GraphicsMan::AddTexture(const SHAPE* pShp, const char* textureName, int lockType, int copyMem, int bindId)
{
    const SHAPEIMAGE* src = (const SHAPEIMAGE*)pShp;
    int fmt = 0;
    int cls = 0;
    void* pal = 0;
    void* data;
    unsigned int type = *(unsigned char*)src;
    // ORIGINAL-PTR: a SHAPE file blob: the palette shape starts next bytes after this one, and the pixels start
    // dataoffset bytes into a shape (flag 0x1000) or right after its 16-byte header
    switch (type) {
    case kSHAPE_PAL4: {
        const SHAPEIMAGE* base = src->next ? (const SHAPEIMAGE*)((char*)src + src->next) : 0;
        fmt = 11;
        if (src->flags & 0x2000) fmt = 0xC;
        cls = 1;
        if (base->flags & 0x1000)
            pal = (char*)base + base->dataoffset;
        else
            pal = (char*)base + 0x10;
        break;
    }
    case kSHAPE_PAL8: {
        const SHAPEIMAGE* base = src->next ? (const SHAPEIMAGE*)((char*)src + src->next) : 0;
        fmt = 9;
        if (src->flags & 0x2000) fmt = 10;
        cls = 1;
        if (base->flags & 0x1000)
            pal = (char*)base + base->dataoffset;
        else
            pal = (char*)base + 0x10;
        break;
    }
    case kSHAPE_RGB15:
        fmt = 2;
        break;
    case kSHAPE_RGB32:
        fmt = 0;
        break;
    default:
        break;
    }
    unsigned int flags = src->flags;
    data = (char*)src + 0x10;
    if (flags & 0x1000)
        data = (char*)src + src->dataoffset;
    int textureId = AddTexture1((const char*)data, (const char*)textureName, src->width, src->height,
        flags >> 28, fmt, cls, (const char*)pal, lockType, copyMem, bindId);
    if ((src->flags & 0xF0000000) != 0 && src->centerx != 0) {
        double lodValue = fptodp(240.0f / ((float)src->centerx * 6.1035156e-05f));
        lodValue = dlog_C8C0(lodValue);
        lodValue = dpmul(lodValue, 1.4426950408889634);
        lodValue = dpsub(0.0, lodValue);
        lodValue = dpmul(lodValue, 16.0);
        int k = dtoi_C8C0(lodValue);
        k = clampZ_C8C0(k, -0x7FF, -135);
        cPS2GraphicsMan_setTextureFilter_C8C0(((char*)this), textureId, 5, 1, 0, k);
    }
    return textureId;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_AddTexture1);
#ifdef SKIP_ASM
extern "C" int cTextureMan_AddTexture(void* mgr, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);

/**
 * Creates a texture in the texture manager (cTextureMan_AddTexture on mTexMan) and marks its id in mTextureAdded.
 *
 * All eleven arguments go through unchanged; mTextureAdded[id] is set to 1. Slot 47 of cPS2GraphicsMan_vtbl;
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
    int i = cTextureMan_AddTexture(self->mTexMan, pixels, name, width, height, mipmapCount, texelFormat, clutFormat, clutData, lockType, copyMem, bindId);
    self->mTextureAdded[i] = 1;
    return i;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RemoveTexture);
#ifdef SKIP_ASM
extern "C" void cTextureMan_RemoveTexture(void* tbl, int idx);

/**
 * Removes texture @p idx from the texture manager (cTextureMan_RemoveTexture) and clears its slot in mTextureUploaded.
 *
 * @param idx  the texture id (On Tour texid)
 */
void cPS2GraphicsMan::RemoveTexture(int idx)
{
    cTextureMan_RemoveTexture(mTexMan, idx);
    mTextureUploaded[idx] = 0;
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
bool cPS2GraphicsMan::IsValidTexture(int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)mTexMan;
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

extern "C" void* cTextureMan_setTextureFunction(int);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setTextureFunction__FPv);
#ifdef SKIP_ASM
/**
 * Sets a texture's texture function (TEX0.TFX): forwards to cTextureMan_setTextureFunction on mTexMan.
 *
 * The definition declares only self; the caller's texture id and TFX value pass through untouched in the second and
 * third argument registers. cPS2GraphicsMan_DrawCharModel calls it with TFX 3 (highlight 2).
 *
 * @return nothing meaningful (cTextureMan_setTextureFunction returns nothing; the definition is declared void*)
 */
void* cPS2GraphicsMan::setTextureFunction()
{
    return cTextureMan_setTextureFunction((int)mTexMan);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_CreateScreenGrabTexture);
#ifdef SKIP_ASM
/**
 * Creates a screen-grab texture: stores the texture id returned by virtual slot 52 (@p width, @p height, 0) in the handle's
 * second word.
 *
 * @param out  the tScreenGrabHandle to fill
 * @param width  the texture width (On Tour width)
 * @param height  the texture height (On Tour height)
 */
void cPS2GraphicsMan::CreateScreenGrabTexture(tScreenGrabHandle& out, unsigned int width, unsigned int height)
{
    out.mTextureId = AddLockedFrameTexture(width, height, 0);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RemoveScreenGrabTexture);
#ifdef SKIP_ASM

extern "C" void cTextureMan_unlockTexture(void* mgr, int id);

/**
 * Releases a screen-grab texture: when the handle's texture id is not kNoScreenGrab (-1), frees it in the texture
 * manager (cTextureMan_unlockTexture), removes it (virtual slot 50) and sets the id to kNoScreenGrab.
 *
 * @param item  the tScreenGrabHandle to release
 */
void cPS2GraphicsMan::RemoveScreenGrabTexture(tScreenGrabHandle& item)
{
    if (item.mTextureId != tScreenGrabHandle::kNoScreenGrab) {
        cTextureMan_unlockTexture(mTexMan, item.mTextureId);
        RemoveTexture(item.mTextureId);
        item.mTextureId = tScreenGrabHandle::kNoScreenGrab;
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



extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
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
    char pad_0xE88[0xF44 - 0xE88];
    int mSkipNextPageFlip;          // 0xF44 cGraphicsMan::mSkipNextPageFlip
    char pad_0xF48[0x18F0 - 0xF48];
    cPS2DeferManager* ring;    // 0x18F0
    sPtTexTbl_CC98* tbl;            // 0x18F4
    char pad_0x18F8[0x5900 - 0x18F8];
    unsigned short vx;              // 0x5900
    unsigned short vy;              // 0x5902
    unsigned short vw;              // 0x5904
    unsigned short vh;              // 0x5906
    char pad_0x5908[0x5A00 - 0x5908];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x5A54 - 0x5A04];
    int mDisplayWidth;              // 0x5A54 (cPS2GraphicsMan's names at these offsets)
    int mDisplayHeight;             // 0x5A58
    int mDisplayBufferWidth;        // 0x5A5C
    char pad_0x5A60[0x5A80 - 0x5A60];
    unsigned int mCurrentZBufferPage;   // 0x5A80
    char pad_0x5A84[4];
    int mDisplayBufferAddress;      // 0x5A88
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
    self->top->m_bitGroups[0] &= ~0x3C0;
    self->top->m_bitGroups[3] &= 0xC0000000;
    unsigned int buf = self->bufAddr;
    int sz = tex->size / 32;
    int dispBlock = self->mDisplayBufferAddress << 5;
    int dispBufferWidth = self->mDisplayBufferWidth;
    unsigned int zbufPage = self->mCurrentZBufferPage;
    unsigned long* packet = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
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
    packet[18] = (long)dispBlock | (long)dispBufferWidth << 14 | 0x2a8000000L;
    packet[19] = 6;
    packet[20] = 278;
    packet[21] = 0;
    uPtFloatBits one;
    one.f = 1.0f;
    packet[22] = ((unsigned long)one.i << 32) | 0x80808080UL;
    packet[23] = 1;
    packet += 24;
    int dispHeight = self->mDisplayHeight;
    int dispWidth = self->mDisplayWidth;
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
    unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, packet, 4);
    self->bufAddr = next;
    cPS2DeferManager* ring = self->ring;
    sPtRenderState* rs = self->top;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtRingEnt* ringEntry = (sPtRingEnt*)((ring->mNumSubmitDMA++ << 7)
            + ((unsigned int)ring->mSubmits | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    self->mSkipNextPageFlip = 1;
    self->bufAddr += 0x10;
    self->top--;
}
#endif

extern "C" void* cTextureMan_setTextureFilter(int);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_setTextureFilter__FPv);
#ifdef SKIP_ASM
/**
 * Sets a texture's filtering (TEX1 MMIN, MMAG, L and K): forwards to cTextureMan_setTextureFilter on mTexMan.
 *
 * The definition declares only self; the caller's texture id and four filter values pass through untouched in the
 * argument registers. cPS2GraphicsMan_AddTexture calls it with MMIN 5, MMAG 1, L 0 and a K it computes; the tile
 * constructor in mem/memstd (SSX_VD_PS2_SINGLE_BUFFERD_SSX_VD_PS2_SINGLE_BUFFERD) calls it with zeros.
 *
 * @return nothing meaningful (cTextureMan_setTextureFilter returns nothing; the definition is declared void*)
 */
void* cPS2GraphicsMan::setTextureFilter()
{
    return cTextureMan_setTextureFilter((int)mTexMan);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetNewTextureData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
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
 * Formats as in cPS2GraphicsMan_AddTexture: kSHAPE_PAL4 gives texel format 11 (12 swizzled), kSHAPE_PAL8 gives 9 (10),
 * both with CLUT format 1 and the next record's palette; kSHAPE_RGB15 gives 2 and kSHAPE_RGB32 gives 0. When the bytes
 * after the header start kREFPACK_TYPE kREFPACK_MAGIC (0x10 0xFB: a packed stream whose 24-bit size follows) it
 * allocates size + 0x10 bytes (label str_unpack), copies the header, unpacks the data with unpack, clears bit 7 of the
 * type byte and uses the copy, freeing it with MEM_free afterwards.
 *
 * Calls vtable slot 49 (cPS2GraphicsMan_SetNewTextureData1) with (@p textureId, texels, width, height, mipmap count, texel format, CLUT format,
 * palette, @p copyMem). Slot 48 of cPS2GraphicsMan_vtbl.
 *
 * @param textureId  the texture id
 * @param objp  the shape record
 * @param copyMem  the copy-memory flag, passed through
 */
void cPS2GraphicsMan::SetNewTextureData(int textureId, const SHAPE* objp, int copyMem)
{
    cPS2GraphicsMan* self = this;
    SHAPEIMAGE* obj = (SHAPEIMAGE*)objp;
    int mode = 0;
    char* extra = 0;
    int flagB = 0;
    unsigned int type = *(unsigned char*)obj;
    // ORIGINAL-PTR: a SHAPE file blob: the palette shape starts next bytes after this one, the pixels start
    // dataoffset bytes into a shape (flag 0x1000) or right after its 16-byte header, and a packed shape's stream
    // header is read byte by byte
    switch (type) {
    case kSHAPE_PAL4: {
        mode = 11;
        int w = obj->next;
        flagB = 1;
        char* sub = w ? (char*)obj + w : 0;
        int hi = obj->flags & 0x2000;
        if (hi) mode = 0xC;
        if (((SHAPEIMAGE*)sub)->flags & 0x1000)
            extra = sub + *(int*)(sub + 0x10);
        else
            extra = sub + 0x10;
        break;
    }
    case kSHAPE_PAL8: {
        mode = 9;
        int w = obj->next;
        flagB = 1;
        char* sub = w ? (char*)obj + w : 0;
        int hi = obj->flags & 0x2000;
        if (hi) mode = 10;
        if (((SHAPEIMAGE*)sub)->flags & 0x1000)
            extra = sub + *(int*)(sub + 0x10);
        else
            extra = sub + 0x10;
        break;
    }
    case kSHAPE_RGB15:
        mode = 2;
        break;
    case kSHAPE_RGB32:
        mode = 0;
        break;
    }
    int allocated = 0;
    char* unpackedCopy = 0;
    char* afterHeader = (char*)obj + 0x10;
    unsigned int fC;
    char* data;
    unsigned char* ob = (unsigned char*)obj;
    if (ob[16] == kREFPACK_TYPE && ob[17] == kREFPACK_MAGIC) {
        unsigned int size = (ob[18] << 16 | ob[19] << 8 | ob[20]) + 0x10;
        allocated = 1;
        unpackedCopy = (char*)operator_new_D0B0(size, str_unpack, 0x80000100, 0);
        MEM_move(unpackedCopy, obj, 0x10);
        char* payload = unpackedCopy + 0x10;
        unpack(afterHeader, payload);
        *(unsigned char*)unpackedCopy &= 0x7F;
        if (((SHAPEIMAGE*)unpackedCopy)->flags & 0x1000) {
            data = unpackedCopy + *(int*)(unpackedCopy + 0x10);
            fC = obj->flags;
        } else {
            data = unpackedCopy + 0x10;
            fC = obj->flags;
        }
    } else {
        if (obj->flags & 0x1000)
            data = (char*)obj + *(int*)((char*)obj + 0x10);
        else
            data = afterHeader;
        fC = obj->flags;
    }
    sVE_D0B0* slotEntry = &((sVE_D0B0*)VPTR_OF(cGraphicsMan, self))[49];
    slotEntry->fn(vthis(self, *slotEntry), textureId, data, obj->width, obj->height, fC >> 28, mode, flagB, extra,
        copyMem);
    if (allocated) {
        MEM_free(unpackedCopy);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetNewTextureData1);
#ifdef SKIP_ASM
extern "C" void cTextureMan_SetNewTextureData(void* gm, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

/**
 * Replaces a texture's pixels and CLUT from raw data: forwards all arguments to cTextureMan_SetNewTextureData on mTexMan.
 *
 * cTextureMan_SetNewTextureData copies @p pixels and @p clutData into the texture's own buffers when @p copyData is set and drops its VRAM copy so
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
    cTextureMan_SetNewTextureData(self->mTexMan, texId, pixels, width, height, mipCount, texelFormat, clutFormat, clutData, copyData);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_LockFrameToTexture);
#ifdef SKIP_ASM




extern "C" void cTextureMan_LockFrameToTexture_D348(void* texset, int idx, unsigned int* buf) __asm__("cTextureMan_LockFrameToTexture");

/**
 * Redirects drawing into a locked texture.
 *
 * Builds a GS packet with cTextureMan_LockFrameToTexture that sets FRAME, the scissor and XYOFFSET for texture @p idx, and queues it on
 * the render ring.
 *
 * @param idx  the texture id from cPS2GraphicsMan_AddLockedFrameTexture()
 * @return 1
 */
// PORT: uncached (0x30000000) pointers held in int.
// Uploads texture idx into the uncached buffer and queues a ring entry for it.
bool cPS2GraphicsMan::LockFrameToTexture(int idx, int)
{
    unsigned int buf = mVifCursor;
    unsigned int packetAddr = buf | 0x30000000;
    cTextureMan_LockFrameToTexture_D348(((void*)mTexMan), idx, &packetAddr);
    unsigned int next = packetAddr & 0x0FFFFFFF;
    mVifCursor = next;
    cPS2DeferManager* ring = mpDeferMan;
    sDrawState* rs = m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        // ORIGINAL-PTR: the GS packet and the defer record are written through the EE's uncached-accelerated alias
        // (address | 0x30000000, a hardware address segment), so the index is added to the aliased address
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
    }
    mVifCursor += 0x10;
    return true;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RestoreFrameLock);
#ifdef SKIP_ASM




/**
 * Points drawing back at the normal frame buffer after cPS2GraphicsMan_LockFrameToTexture().
 *
 * Writes a GS packet that restores FRAME (base, width, pixel format), the scissor from the screen rectangle and
 * XYOFFSET to the screen centre, and queues it. Returns 0, where On Tour's bool version presumably reports success.
 *
 * @return 0
 */
// PORT: 64-bit `ulong` GS packet words; uncached (0x30000000) pointers held in int.
bool cPS2GraphicsMan::RestoreFrameLock()
{
    unsigned int buf = mVifCursor;
    // ORIGINAL-PTR: the GS packet and the defer record are written through the EE's uncached-accelerated alias
    // (address | 0x30000000, a hardware address segment)
    ulong* packet = (ulong*)(buf | 0x30000000);
    packet[0] = 0x10000006;
    packet[1] = 0;
    packet[2] = (ulong)34816 << 45;
    packet[3] = (ulong)0x50000005 << 32;
    packet[4] = ((ulong)0x10000000 << 32) | 0x8004;
    packet[5] = 0xE;
    packet[6] = (ulong)((unsigned int)mCurrentFrameBufferPage) | ((ulong)((unsigned int)mFrameBufferWidth) << 16) | ((ulong)((unsigned int)mCurrentFBFormat) << 24);
    packet[7] = 76;
    packet[8] = (ulong)mScissor[0] | ((ulong)(mScissor[0] + (mScissor[2] - 1)) << 16) | ((ulong)mScissor[1] << 32) | ((ulong)(mScissor[1] + (mScissor[3] - 1)) << 48);
    packet[9] = 0x40;
    packet[10] = (ulong)mScissor[0] | ((ulong)(mScissor[0] + (mScissor[2] - 1)) << 16) | ((ulong)mScissor[1] << 32) | ((ulong)(mScissor[1] + (mScissor[3] - 1)) << 48);
    packet[11] = 65;
    packet[12] = (ulong)((0x800 - (((unsigned int)mFBWidth) >> 1)) << 4) | ((ulong)((0x800 - (((unsigned int)mFBHeight) >> 1)) << 4) << 32);
    packet[13] = 0x18;
    packet += 14;
    unsigned int next = (unsigned int)packet & 0x0FFFFFFF;
    cPS2DeferManager* ring = mpDeferMan;
    sDrawState* rs = m_pCurrentDrawState;
    mVifCursor = next;
    int one = 1;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mContinuation = one;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mPacket = buf;
    }
    mVifCursor += 0x10;
    return false;
}
#endif

extern "C" void* cTextureMan_AddLockedFrameTexture(int);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_writeFrameRegPacket);
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
 * @param mode  kGS_Context1 (0) or kGS_Context2 (1)
 */
extern "C" void cPS2GraphicsMan_writeFrameRegPacket(cPS2GraphicsMan* self, ulong** pp, int frameMask, int mode)
{
    ulong* packet = *pp;
    switch (mode) {
    case kGS_Context1:
        packet[1] = ((ulong)0x50000001 << 32) | 0x11000000;
        packet[2] = ((ulong)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[0] = 0;
        packet[4] = (ulong)(unsigned int)self->mCurrentFrameBufferPage | ((ulong)(unsigned int)self->mFrameBufferWidth << 16) | ((ulong)(unsigned int)self->mCurrentFBFormat << 24) | ((ulong)frameMask << 32);
        packet[5] = 76;
        break;
    case kGS_Context2:
        packet[1] = ((ulong)0x50000002 << 32) | 0x11000000;
        packet[2] = ((ulong)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[0] = 0;
        packet[4] = (ulong)(unsigned int)self->mCurrentFrameBufferPage | ((ulong)(unsigned int)self->mFrameBufferWidth << 16) | ((ulong)(unsigned int)self->mCurrentFBFormat << 24) | ((ulong)frameMask << 32);
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
 * Calls cTextureMan_AddLockedFrameTexture on the texture manager (mTexMan, +0x18F4), which allocates the "LockTex" texture. The C
 * declaration has only self: the width, height and texel format arrive in a1-a3 and pass through to cTextureMan_AddLockedFrameTexture
 * unchanged.
 *
 * @return the new texture id
 */
void* cPS2GraphicsMan_AddLockedFrameTexture(void* vself)
{
    cPS2GraphicsMan* self = (cPS2GraphicsMan*)vself;
    return cTextureMan_AddLockedFrameTexture((int)self->mTexMan);
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
 * Appends a projected texture to mProjTextures and increments mNumShdw (On Tour AddProjectiveTexture(pos,
 * clipMat, projMat, intensity, texid)). There is no bound check on the 8-entry array.
 *
 * @param pos  the projector position (mShdwProjPos)
 * @param clipMat  the clip matrix (mShdwClipMat)
 * @param projMat  the projection matrix (mShdwProjMat)
 * @param intensity  the intensity (mShdwIntensity)
 * @param i  the texture id (On Tour texid; mShdwTexID)
 */
extern "C" void cPS2GraphicsMan_AddProjectiveTexture(cPS2GraphicsMan* self, sPartQ16* pos, sPartMtx* clipMat, sPartMtx* projMat, float intensity, int i)
{
    (*(sPartQ16*)&((&self->mProjTextures[0]))[self->mNumShdw].mShdwProjPos) = *pos;
    vu0CopyMtxP(&(*(sPartMtx*)&((&self->mProjTextures[0]))[self->mNumShdw].mShdwClipMat), clipMat);
    vu0CopyMtxP(&(*(sPartMtx*)&((&self->mProjTextures[0]))[self->mNumShdw].mShdwProjMat), projMat);
    ((&self->mProjTextures[0]))[self->mNumShdw].mShdwIntensity = intensity;
    ((&self->mProjTextures[0]))[self->mNumShdw].mShdwTexID = i;
    self->mNumShdw++;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_ShadowClearLockTexture);
#ifdef SKIP_ASM




extern "C" void cTextureMan_ShadowClearLockTexture_D800(void* texset, int idx, float* color, unsigned int* buf) __asm__("cTextureMan_ShadowClearLockTexture");

// Release-build assert: expands to an empty do { } while (0); its loop notes keep the target's scheduling.
#define ASSERT_D800(cond) do { } while (0)

/**
 * Clears a locked shadow texture to a colour.
 *
 * Builds a packet with cTextureMan_ShadowClearLockTexture that draws a full-size sprite of @p color over texture @p idx, and queues it on
 * the render ring.
 *
 * @param idx  the texture id
 * @param color  the clear colour
 */
// PORT: uncached (0x30000000) pointers held in int.
// Like cPS2GraphicsMan_LockFrameToTexture for the two-argument upload; the queued entry is flagged.
extern "C" void cPS2GraphicsMan_ShadowClearLockTexture(cPS2GraphicsMan* self, int idx, float* color)
{
    unsigned int buf = self->mVifCursor;
    unsigned int packetAddr = buf | 0x30000000;
    cTextureMan_ShadowClearLockTexture_D800(((void*)self->mTexMan), idx, color, &packetAddr);
    unsigned int next = packetAddr & 0x0FFFFFFF;
    self->mVifCursor = next;
    cPS2DeferManager* ring = self->mpDeferMan;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        // ORIGINAL-PTR: the GS packet and the defer record are written through the EE's uncached-accelerated alias
        // (address | 0x30000000, a hardware address segment), so the index is added to the aliased address
        tDeferSubmit* ringEntry =
            (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ASSERT_D800(ringEntry != 0);
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 1;
    }
    self->mVifCursor += 0x10;
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
int cPS2GraphicsMan::GetTextureWidth(int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)mTexMan;
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
int cPS2GraphicsMan::GetTextureHeight(int idx)
{
    sPartTexTable* texTable = (sPartTexTable*)mTexMan;
    return 1 << texTable->entries[idx]->tex0.TH;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_computeMatrices);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_D968(t4Matrix* d, const t4Matrix* a, const t4Matrix* b)
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
static inline void vu0CopyMat_D968(t4Matrix* d, const t4Matrix* s)
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

extern "C" void cPS2DeferManager_findOrAddViewState(void* ring, t4Matrix* m);

/**
 * Recomputes the cached transform matrices from the current view matrix and sets mMatricesValid to 1.
 *
 * With VU0 (row vectors, so the left matrix applies first): mScreenWorld = *mCurrentView x mScreenMatrix; mWorld =
 * *mCurrentView; mGuardBandMatrix = mWorld x mView; mNetMatrix = mWorld x mViewProj (each product goes through a stack
 * copy). Then hands mWorld to the defer manager with cPS2DeferManager_findOrAddViewState on mpDeferMan. cPS2GraphicsMan_updateNetMat calls it
 * when mMatricesValid is 0, and many draw paths in render/particle and render/bezierman call it directly.
 *
 */
void cPS2GraphicsMan::computeMatrices()
{
    mMatricesValid = 1;
    t4Matrix copy;
    t4Matrix product;
    vu0MulMat_D968(&product, ((&mScreenMatrix)), mCurrentView);
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968(((&mScreenWorld)), &copy);
    vu0CopyMat_D968(((&mWorld)), mCurrentView);
    vu0MulMat_D968(&product, ((&mView)), ((&mWorld)));
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968(((&mGuardBandMatrix)), &copy);
    vu0MulMat_D968(&product, ((&mViewProj)), ((&mWorld)));
    vu0CopyMat_D968(&copy, &product);
    vu0CopyMat_D968(((&mNetMatrix)), &copy);
    cPS2DeferManager_findOrAddViewState(mpDeferMan, ((&mWorld)));
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetOutCode);
#ifdef SKIP_ASM


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

extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline t4Vector mtxApplyDBE8(t4Matrix* m, const t4Vector& v)
{
    t4Vector out;
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
 * Refreshes the matrices if mMatricesValid is clear (cPS2GraphicsMan_computeMatrices), transforms @p point by mScreenWorld and divides x
 * and y by w. Bits: 2 / 1 for x below mClipMin.x / above mClipMax.x, 8 / 4 for y below / above, and when w is
 * positive 0x20 for z above mClipMin.z and 0x10 for z below mClipMax.z (z is not divided by w). A point behind the
 * eye (w < 0) has its code xor'd with 0x2F.
 *
 * @param point  the world-space point (On Tour point)
 * @return the outcode bits (0 = inside)
 */
int cPS2GraphicsMan::GetOutCode(const t4Vector& point)
{
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    t4Vector screenPos = mtxApplyDBE8(&(*(&mScreenWorld)), point);
    int code = 0;
    float w = 1.0f / screenPos.w;
    screenPos.x *= w;
    screenPos.y *= w;
    screenPos.w = w;
    if (screenPos.x < mClipMin.x) {
        code |= 2;
    }
    if (mClipMax.x < screenPos.x) {
        code |= 1;
    }
    if (screenPos.y < mClipMin.y) {
        code |= 8;
    }
    if (mClipMax.y < screenPos.y) {
        code |= 4;
    }
    float z = screenPos.z;
    if (w < 0.0f) {
        code ^= 0x2F;
    } else {
        if (mClipMin.z < z) {
            code |= 0x20;
        }
        if (z < mClipMax.z) {
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


// cPS2GraphicsMan_projectToScreen is defined later in this unit with its own vector type; bind a view.
extern "C" sPtVecDD20 cPS2GraphicsMan_projectToScreen_DD20(void* self, const sPtVecDD20* p) __asm__("cPS2GraphicsMan_projectToScreen");

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
 * Refreshes the clip matrices when stale, projects @p point (cPS2GraphicsMan_projectToScreen_DD20), subtracts the GS offset (2048 minus
 * half the screen size) from x and y and truncates them to whole pixels, and clamps z to the depth range.
 *
 * @param point  the world-space point
 * @return the screen position (x, y in pixels, z the depth)
 */
extern "C" sPtVecDD20 cPS2GraphicsMan_GetScreenCoord(cPS2GraphicsMan* self, const sPtVecDD20* point)
{
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    sPtVecDD20 screen = cPS2GraphicsMan_projectToScreen_DD20(self, point);
    screen.X() = (float)(int)(screen.X() - (2048.0f - (float)(((unsigned int)self->mFBWidth) >> 1)));
    screen.Y() = (float)(int)(screen.Y() - (2048.0f - (float)(((unsigned int)self->mFBHeight) >> 1)));
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
 * @return GM_CLIPCODE_OUTSIDE when the microprogram's second result is set, else GM_CLIPCODE_CROSSING when its first
 *         is set, else GM_CLIPCODE_INSIDE
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
        return GM_CLIPCODE_OUTSIDE;
    }
    if (vi1Result != 0) {
        return GM_CLIPCODE_CROSSING;
    }
    return GM_CLIPCODE_INSIDE;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_projectToScreen);
#ifdef SKIP_ASM

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
extern "C" t4Vector cPS2GraphicsMan_projectToScreen(cPS2GraphicsMan* self, t4Vector* point)
{
    t4Vector result;
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
        : "r"(point), "r"((char*)&self->mNetMatrix),
          "m"(self->mScreenScale),
          "m"(self->mScreenOffset));
    return result;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_transformByNetMatrix);
#ifdef SKIP_ASM

/**
 * Transforms @p point by mNetMatrix with VU0 and returns the result, without the divide by w.
 *
 * Nothing in the retail code references it.
 *
 * @param point  the point to transform
 * @return the transformed (homogeneous) point
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
extern "C" t4Vector cPS2GraphicsMan_transformByNetMatrix(cPS2GraphicsMan* self, t4Vector* point)
{
    char* mat = (char*)&self->mNetMatrix;
    t4Vector result;
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
 * Returns a bit mask of the projective textures whose clip volume a box is not outside of (GM_CLIPCODE_OUTSIDE).
 *
 * For each of the mNumShdw entries of mProjTextures it calls virtual slot 93 (cPS2GraphicsMan_ClipBoundBox) with @p
 * boxMin, @p boxMax and the entry's clip matrix (tProjTexInfo +0x10) and sets bit i when the result is not
 * GM_CLIPCODE_OUTSIDE. cPS2BezierMan_testProjectiveTextureHits in render/bezierman calls it.
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
        if (vtEntry->fn(vthis(self, *vtEntry), boxMin, boxMax, &self->items[i]) != GM_CLIPCODE_OUTSIDE) {
            mask |= 1 << i;
        }
    }
    return mask;
}
#endif

//100%
INCLUDE_ASM("render/particle", vif_appendMatrixUpload);
#ifdef SKIP_ASM
// PORT: 128-bit GPR quadword (TImode); the PC port needs a 16-byte struct.
typedef int cPartQuad128 __attribute__((mode(TI)));

/**
 * Appends a 4 x 4 matrix as a VIF upload: a DMA cnt tag (5 quadwords), a VIF UNPACK V4-32 of 4 quadwords to VU address
 * 5 (TOPS-relative) and the matrix @p mat.
 *
 * Advances *@p pp by 6 quadwords. vif_appendObjectConstants calls it for its optional second matrix.
 *
 * @param mat  the matrix to send (16-byte aligned)
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words and PS2-only VU0 asm (lqc2/sqc2 copy).
extern "C" void vif_appendMatrixUpload(void* mat, char** pp)
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
INCLUDE_ASM("render/particle", vif_appendHeaderAndDataUpload);
#ifdef SKIP_ASM
struct sPartPktQuad {
    int w[4];
} __attribute__((aligned(16)));

/**
 * Appends two VIF uploads: 3 header quadwords from @p hdr to VU address 9 and 10 quadwords from @p src to VU address 15
 * (both UNPACK V4-32, TOPS-relative), under one DMA cnt tag of 15 quadwords.
 *
 * Advances *@p pp by 16 quadwords. The second parameter is not used (vif_appendObjectConstants passes the translation row of its
 * matrix there). vif_appendObjectConstants calls it when bit 0 of its flags is set.
 *
 * @param src  the 10 quadwords to send
 * @param hdr  the 3 header quadwords to send
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words.
extern "C" void vif_appendHeaderAndDataUpload(sPartPktQuad* src, void* self, sPartPktQuad* hdr, char** pp)
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
INCLUDE_ASM("render/particle", vif_appendObjectConstants);
#ifdef SKIP_ASM
/**
 * Appends the per-object VU constants for a draw: a matrix and a flags word, plus optional extra uploads.
 *
 * Writes a DMA cnt tag (6 quadwords), VIF STMASK 0 / STCYCL 1,1 and UNPACK V4-32 of 5 quadwords to VU address 0
 * (TOPS-relative), the matrix @p mat (copied with VU0) and a quadword whose w is @p flags. When @p extraMat is not 0 it
 * appends that matrix with vif_appendMatrixUpload; when bit 0 of @p flags is set it appends @p hdr and @p src with
 * vif_appendHeaderAndDataUpload. @p scale is not used. cPS2GraphicsMan_RenderModel and cPS2GridMesh_Render1 (cPS2GridMesh's render,
 * which passes its texture matrix as @p extraMat) call it.
 *
 * @param mat  the object matrix
 * @param hdr  3 quadwords for vif_appendHeaderAndDataUpload
 * @param scale  not used (cPS2GridMesh_Render1 passes its mesh scale vector here)
 * @param extraMat  an optional second matrix, or 0
 * @param src  10 quadwords for vif_appendHeaderAndDataUpload
 * @param flags  the flags word sent to the VU; bit 0 adds the vif_appendHeaderAndDataUpload upload
 * @param pp  the packet cursor
 */
// PORT: 64-bit `ulong` packet words and PS2-only VU0 asm (lqc2/sqc2 copy).
extern "C" void vif_appendObjectConstants(void* self, sPartPktQuad* mat, sPartPktQuad* hdr, int scale, void* extraMat,
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
        vif_appendMatrixUpload(extraMat, &packet);
    }
    if (flags & 1) {
        sPartPktQuad translation = mat[3];
        vif_appendHeaderAndDataUpload(src, &translation, hdr, &packet);
    }
    *pp = packet;
}
#endif

INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderModel);

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_GetMeshQuad);
#ifdef SKIP_ASM

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
    t3Vector scale;         // 0x18
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
extern "C" void cPS2GraphicsMan_GetMeshQuad(cPS2GraphicsMan* self, tInstanceRecord* obj, int idx, float* pos, float* uv, float* col)
{
    sMdl0380* mdl = (sMdl0380*)obj->m_Model;
    char* base = *(char**)(*(char**)(*(char**)(mdl->ents[idx].node + 0x20)) + 0x4);
    char* data = *(char**)(base + mdl->dataOff + 0x4);
    short* texCoords = (short*)(data + 0x80);
    short* positions = (short*)(data + 0xA0);
    sCol0380* colours = (sCol0380*)(*(char**)(base + *(int*)(((char*)obj) + 0x98) + 0x14) + 0x10);
    t3Vector k;
    t3Vector modelScale = mdl->scale;
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
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words); [U] word 0
                                    //      bits 6..9 = layer, word 2 bits 0..4 = key
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



struct sPtGfx_0518 {
    char pad_0x0[0xE84];
    sPtRS_0518* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    cPS2DeferManager* ring;             // 0x18F0
    char pad_0x18F4[0x5A00 - 0x18F4];
    unsigned int bufAddr;           // 0x5A00
    char pad_0x5A04[0x6B90 - 0x5A04];
    int texReady;                   // 0x6B90
};

extern int cRenderToggles_m_DisableStaticEmitters;
extern int D_004A4474;
extern char D_0[];
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

static inline void CopyQuads_0518(unsigned long** pp, const void* srcp, int n)
{
    const t4Vector* src = (const t4Vector*)srcp;
    t4Vector* d = (t4Vector*)*pp;
    t4Vector* end = d + n;
    while (d != end) {
        *d++ = *src++;
    }
    *pp = (unsigned long*)d;
}

/**
 * Draws a static particle system.
 *
 * Does nothing while cRenderToggles_m_DisableStaticEmitters is set. Pushes the render state with the particle mode, copies 21 qwords of @p obj (the
 * tParticleEmitter) into a packet followed by a VIF call of the microprogram, queues it on the render ring and pops the
 * state.
 *
 * @param obj  the particle emitter (tParticleEmitter)
 */
extern "C" void cPS2GraphicsMan_RenderParticleEmitter(sPtGfx_0518* self, t4Vector* obj)
{
    if (cRenderToggles_m_DisableStaticEmitters != 0)
        return;
    self->top[1] = *self->top;
    self->top++;
    self->top->m_bitGroups[0] = (self->top->m_bitGroups[0] & ~0x3C0) | 0x100;
    self->top->m_bitGroups[3] &= 0xC0000000;
    self->top->m_bitGroups[1] = (self->top->m_bitGroups[1] & ~3) | 2;
    self->top->m_bitGroups[2] &= 0xE00003FF;
    self->top->m_bitGroups[2] = (self->top->m_bitGroups[2] & ~0x3E0) | 0xE0;
    unsigned int buf = self->bufAddr;
    unsigned long* packet = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
    packet[0] = 0x10000017;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55339 << 47;
    packet += 4;
    CopyQuads_0518(&packet, obj, 21);
    packet[0] = (unsigned long)(((unsigned int)D_0 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    self->bufAddr = cPS2ScratchPad_flushBufferTo(D_004A4474, packet, 4);
    if (self->texReady == 0) {
        ((cPS2GraphicsMan*)self)->computeMatrices();
    }
    cPS2DeferManager* ring = self->ring;
    unsigned int next = self->bufAddr;
    sPtRS_0518* rs = self->top;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_0518* ringEntry = (sPtEnt_0518*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_0518*)&ring->mSubmits[0]) | 0x30000000));
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

struct sPtEnt_07A0 {
    sDrawState rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};


struct sQuad_07A0 {
    unsigned int w[4];
} __attribute__((aligned(16)));



struct sHdr_07A0 {
    int seg;
    int step;
    float fstep;
    float v;
} __attribute__((aligned(16)));

extern int cRenderToggles_m_DisableDynamicEmitters;
extern int D_004A4474;
extern char D_510[];
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

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
 * Returns at once when cRenderToggles_m_DisableDynamicEmitters (menu "Disable Dynamic Emitters") is set. Sets bits 6-9 of the draw state's first
 * word to 4 and keeps only the top two bits of its fourth, rebuilding the matrices first (cPS2GraphicsMan_computeMatrices) when
 * mMatricesValid is 0. For each block of up to 64 of the m_numSubEmitters entries (n / 64 + 1 blocks) it claims a
 * scratchpad ring buffer (cPS2ScratchPad_claimBuffer at mVifCursor) and writes a DMA/VIF header, the 21 quadwords of m_emitter, a
 * block header {count, NumParticles / m_numSubEmitters, its float, that float * first index * TimeStep}, the block's
 * m_positions entries (oldest first from m_subEmitterId + 1, wrapping) plus the next entry, padding to 64, the same for
 * m_velocities, and a VIF MSCAL of the microprogram at D_510.
 *
 * It flushes the buffer (cPS2ScratchPad_flushBufferTo) and stores the result in mVifCursor; while mpDeferMan holds fewer than 2600
 * entries, a 0x80-byte entry is appended to it with the current draw state (*m_pCurrentDrawState, low 5 bits of its
 * third word from the manager's mActivePortIndex), the packet's start and end and the view index mActiveViewIndex;
 * and it advances mVifCursor by 0x10. Finally it clears the draw-state bits it set. When m_numSubEmitters is a
 * multiple of 64 the last block is empty but still sent. Slot 83 of cPS2GraphicsMan_vtbl.
 *
 * @param obj  the emitter (cBaseClass_DynamicEmitter layout)
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
void cPS2GraphicsMan::RenderDynamicParticleEmitter(const cDynamicEmitter* obj)
{
    if (cRenderToggles_m_DisableDynamicEmitters != 0)
        return;
    m_pCurrentDrawState->m_bitGroups[0] = (((int)(m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0x100;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    int n = obj->m_numSubEmitters;
    sHdr_07A0 hdr;
    int done = 0;
    hdr.step = obj->m_emitter.NumParticles / n;
    hdr.fstep = (float)hdr.step;
    int nb = n / 64 + 1;
    for (int i = 0; i < nb; i++) {
        unsigned int buf = mVifCursor;
        unsigned long* writePtr = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
        writePtr[0] = 0x1000009A;
        writePtr[1] = 0;
        writePtr[2] = 0;
        writePtr[3] = (unsigned long)55601 << 47;
        hdr.v = hdr.fstep * (float)(i * 64) * obj->m_emitter.TimeStep;
        int seg = obj->m_numSubEmitters - done;
        if (seg >= 65)
            seg = 0x40;
        hdr.seg = seg;
        writePtr += 4;
        CopyQuads_07A0(&writePtr, ((sQuad_07A0*)&obj->m_emitter), 21);
        CopyQuads_07A0(&writePtr, &hdr, 1);
        int wrap = obj->m_subEmitterId + 1;
        int ringSize = obj->m_numSubEmitters;
        int firstIndex = (done + wrap) % ringSize;
        done += hdr.seg;
        int over = firstIndex + hdr.seg - ringSize;
        if (over < 0)
            over = 0;
        int first = firstIndex + hdr.seg - firstIndex;
        if (ringSize - firstIndex < first)
            first = ringSize - firstIndex;
        int skipQuads = 0x40 - first - over;
        sQuad_07A0* lastA;
        sQuad_07A0* lastB;
        if (done < ringSize) {
            int lastIndex = Wrap_07A0(done, wrap, ringSize);
            lastB = ((sQuad_07A0*)obj->m_velocities) + lastIndex;
            lastA = ((sQuad_07A0*)obj->m_positions) + lastIndex;
        } else {
            int lastIndex = Wrap_07A0(done - 1, wrap, ringSize);
            lastB = ((sQuad_07A0*)obj->m_velocities) + lastIndex;
            lastA = ((sQuad_07A0*)obj->m_positions) + lastIndex;
        }
        CopyQuads_07A0(&writePtr, ((sQuad_07A0*)obj->m_positions) + firstIndex, first);
        if (over > 0)
            CopyQuads_07A0(&writePtr, ((sQuad_07A0*)obj->m_positions), over);
        CopyQuads_07A0(&writePtr, lastA, 1);
        writePtr = (unsigned long*)((sQuad_07A0*)writePtr + skipQuads);
        CopyQuads_07A0(&writePtr, ((sQuad_07A0*)obj->m_velocities) + firstIndex, first);
        if (over > 0)
            CopyQuads_07A0(&writePtr, ((sQuad_07A0*)obj->m_velocities), over);
        CopyQuads_07A0(&writePtr, lastB, 1);
        writePtr = (unsigned long*)((sQuad_07A0*)writePtr + skipQuads);
        writePtr[0] = (unsigned long)(((unsigned int)D_510 >> 3) | 0x14000000) << 32;
        writePtr[1] = (unsigned long)34816 << 45;
        writePtr += 2;
        unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, writePtr, 4);
        mVifCursor = next;
        cPS2DeferManager* ring = mpDeferMan;
        sDrawState* rs = m_pCurrentDrawState;
        if (ring->mNumSubmitDMA < 2600) {
            rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
            int key = ring->mActiveViewIndex;
            sPtEnt_07A0* deferEntry = (sPtEnt_07A0*)((ring->mNumSubmitDMA++ << 7)
                    + ((unsigned int)((sPtEnt_07A0*)&ring->mSubmits[0]) | 0x30000000));
            deferEntry->rs = *rs;
            deferEntry->buf = buf;
            deferEntry->next = next;
            deferEntry->key = key;
            deferEntry->flag = 0;
        }
        mVifCursor += 0x10;
    }
    (*(int*)&m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
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

extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void MEM_fill(void* dst, int c, int n);
extern int D_004A4474;
extern int cRenderToggles_m_DisableDynamicEmitters;
struct sPS2GfxView_0CE0 {  // PORT: a view of cPS2GraphicsMan (render/ps2graphicsman.h); the typed members schedule cPS2GraphicsMan_RenderDynamicParticleEmitter1 differently
    char pad_0x0[0xE84];
    sPtRenderState* top;            // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int texIds[(0x10D8 - 0xF50) / 4];   // 0xF50
    sPtVtVP* vtable;                // 0x10D8
    char pad_0x10DC[0x18F0 - 0x10DC];
    cPS2DeferManager* ring;         // 0x18F0
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
 * Returns at once when cRenderToggles_m_DisableDynamicEmitters (menu "Disable Dynamic Emitters") is set. Sets bits 6-9 of the draw state's first
 * word to 4 and keeps only the top two bits of its fourth, rebuilding the matrices first (cPS2GraphicsMan_computeMatrices) when
 * mMatricesValid is 0. Works like cPS2GraphicsMan_RenderDynamicParticleEmitter with blocks of 43 entries (n / 43 + 1
 * blocks) and, after the positions and velocities, a VIF code and the block's 4-byte m_colours entries (wrapping, plus
 * the next one) as 11 quadwords. The microprogram is D_F10 when m_IsAvalanche is set, else D_A00.
 *
 * Each block is flushed (cPS2ScratchPad_flushBufferTo) into mVifCursor and, while mpDeferMan holds fewer than 2600 entries, a
 * 0x80-byte entry is appended to it with the current draw state (*m_pCurrentDrawState, low 5 bits of its third word
 * from the manager's mActivePortIndex), the packet's start and end and the view index mActiveViewIndex. Slot 84 of
 * cPS2GraphicsMan_vtbl.
 *
 * @param obj  the emitter (cDynamicColourEmitter layout)
 */
// PORT: uncached (0x30000000) pointers and VU microprogram addresses held in int; 64-bit GS words are `ulong`.
extern "C" void cPS2GraphicsMan_RenderDynamicParticleEmitter1(sPS2GfxView_0CE0* self, sTrail_0CE0* obj)
{
    if (cRenderToggles_m_DisableDynamicEmitters != 0)
        return;
    self->top->m_bitGroups[0] = (self->top->m_bitGroups[0] & ~0x3C0) | 0x100;
    self->top->m_bitGroups[3] &= 0xC0000000;
    if (self->texReady == 0) {
        ((cPS2GraphicsMan*)self)->computeMatrices();
    }
    int n = obj->count;
    sPtSegHdr hdr;
    int done = 0;
    hdr.step = ((cDynamicColourEmitter*)obj)->m_emitter.NumParticles / n;
    hdr.fstep = (float)hdr.step;
    int nb = n / 43 + 1;
    unsigned int prog = (unsigned int)(obj->alt != 0 ? D_F10 : D_A00) >> 3;
    for (int i = 0; i < nb; i++) {
        unsigned int buf = self->bufAddr;
        unsigned long* writePtr = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
        writePtr[0] = 0x1000007C;
        writePtr[1] = 0;
        writePtr[2] = 0;
        writePtr[3] = (unsigned long)55517 << 47;
        hdr.v = hdr.fstep * (float)(i * 43) * ((cDynamicColourEmitter*)obj)->m_emitter.TimeStep;
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
        int skipQuads = 43 - first - over;
        sGifQuad* lastA;
        sGifQuad* lastB;
        sCol_0CE0* lastC;
        if (done < ringSize) {
            int lastIndex = wrapIndex(done, wrap, ringSize);
            lastA = obj->posA + lastIndex;
            lastB = obj->posB + lastIndex;
            lastC = obj->cols + lastIndex;
        } else {
            int lastIndex = wrapIndex(done - 1, wrap, ringSize);
            lastA = obj->posA + lastIndex;
            lastB = obj->posB + lastIndex;
            lastC = obj->cols + lastIndex;
        }
        CopyQuads(&writePtr, obj->posA + firstIndex, first);
        if (over > 0)
            CopyQuads(&writePtr, obj->posA, over);
        CopyQuads(&writePtr, lastA, 1);
        writePtr = (unsigned long*)((sGifQuad*)writePtr + skipQuads);
        CopyQuads(&writePtr, obj->posB + firstIndex, first);
        if (over > 0)
            CopyQuads(&writePtr, obj->posB, over);
        CopyQuads(&writePtr, lastB, 1);
        writePtr = (unsigned long*)((sGifQuad*)writePtr + skipQuads);
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
        unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, writePtr, 4);
        self->bufAddr = next;
        cPS2DeferManager* ring = self->ring;
        sPtRenderState* rs = self->top;
        if (ring->mNumSubmitDMA < 2600) {
            rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
            int key = ring->mActiveViewIndex;
            sPtRingEnt* deferEntry = (sPtRingEnt*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)ring->mSubmits
                    | 0x30000000));
            deferEntry->rs = *rs;
            deferEntry->buf = buf;
            deferEntry->next = next;
            deferEntry->key = key;
            deferEntry->flag = 0;
        }
        self->bufAddr += 0x10;
    }
    self->top->m_bitGroups[0] &= ~0x3C0;
    self->top->m_bitGroups[3] &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSnowBoxFlakes);
#ifdef SKIP_ASM

struct sPtEnt_1310 {
    sDrawState rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
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




extern int D_004A4474;
extern char D_0[];
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

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
void cPS2GraphicsMan::RenderSnowBoxFlakes(const tSnowBoxData& obj)
{
    m_pCurrentDrawState->m_bitGroups[0] = (((int)(m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0x140;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    m_pCurrentDrawState->m_bitGroups[1] = (((int)(m_pCurrentDrawState)->m_bitGroups[1]) & ~3) | 2;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[2]) &= 0xE00003FF;
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    sMat_1310 xform = MulMat_1310(*(sMat_1310*)&mViewProj, (*(sMat_1310*)&obj.mSnowBoxToWorldMatrix));
    unsigned int buf = mVifCursor;
    unsigned long* packet = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
    packet[0] = 0x1000000D;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55319 << 47;
    packet += 4;
    t4Vector zeroVec;
    zeroVec.x = 0.0f;
    zeroVec.y = 0.0f;
    zeroVec.z = 0.0f;
    zeroVec.w = 0.0f;
    t4Vector onesXY;
    onesXY.x = 1.0f;
    onesXY.y = 1.0f;
    onesXY.z = 0.0f;
    onesXY.w = 0.0f;
    CopyQuads_1310(&packet, (const sQuad_1310*)&xform, 4);
    CopyQuads_1310(&packet, (const sQuad_1310*)&zeroVec, 1);
    CopyQuads_1310(&packet, (const sQuad_1310*)&onesXY, 1);
    CopyQuads_1310(&packet, ((sQuad_1310*)&obj.mParticleOffset), 5);
    packet[0] = (unsigned long)(((unsigned int)D_0 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, packet, 4);
    mVifCursor = next;
    cPS2DeferManager* ring = mpDeferMan;
    sDrawState* rs = m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_1310* ringEntry = (sPtEnt_1310*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_1310*)&ring->mSubmits[0]) | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    mVifCursor += 0x10;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[1]) &= ~3;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[2]) &= 0xE00003FF;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSnowBoxFluffs);
#ifdef SKIP_ASM

struct sPtEnt_16F0 {
    sDrawState rs;                  // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
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




extern int D_004A4474;
extern char D_408[];
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

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
void cPS2GraphicsMan::RenderSnowBoxFluffs(const tSnowBoxData& obj)
{
    m_pCurrentDrawState->m_bitGroups[0] = (((int)(m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0x140;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    m_pCurrentDrawState->m_bitGroups[1] = (((int)(m_pCurrentDrawState)->m_bitGroups[1]) & ~3) | 2;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[2]) &= 0xE00003FF;
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    sMat_16F0 xform = MulMat_16F0(*(sMat_16F0*)&mViewProj, (*(sMat_16F0*)&obj.mSnowBoxToWorldMatrix));
    unsigned int buf = mVifCursor;
    unsigned long* packet = cPS2ScratchPad_claimBuffer(D_004A4474, buf, -1, 0);
    packet[0] = 0x1000000D;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (unsigned long)55319 << 47;
    packet += 4;
    t4Vector zeroVec;
    zeroVec.x = 0.0f;
    zeroVec.y = 0.0f;
    zeroVec.z = 0.0f;
    zeroVec.w = 0.0f;
    t4Vector onesXY;
    onesXY.x = 1.0f;
    onesXY.y = 1.0f;
    onesXY.z = 0.0f;
    onesXY.w = 0.0f;
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&xform, 4);
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&zeroVec, 1);
    CopyQuads_16F0(&packet, (const sQuad_16F0*)&onesXY, 1);
    CopyQuads_16F0(&packet, ((sQuad_16F0*)&obj.mParticleOffset), 5);
    packet[0] = (unsigned long)(((unsigned int)D_408 >> 3) | 0x14000000) << 32;
    packet[1] = (unsigned long)34816 << 45;
    packet += 2;
    unsigned int next = cPS2ScratchPad_flushBufferTo(D_004A4474, packet, 4);
    mVifCursor = next;
    cPS2DeferManager* ring = mpDeferMan;
    sDrawState* rs = m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_16F0* ringEntry = (sPtEnt_16F0*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((sPtEnt_16F0*)&ring->mSubmits[0]) | 0x30000000));
        ringEntry->rs = *rs;
        ringEntry->buf = buf;
        ringEntry->next = next;
        ringEntry->key = key;
        ringEntry->flag = 0;
    }
    mVifCursor += 0x10;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[1]) &= ~3;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[2]) &= 0xE00003FF;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderLineStrip);
#ifdef SKIP_ASM
struct sPartGsCtx;
struct sPartGsVtx;
struct sPartGsPos;
extern "C" void lineStripPacket_addVertex(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos);
extern "C" void lineStripPacket_beginStrip(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos);
extern "C" void* lineStripPacket_open(void* self, unsigned int* src);
extern void lineStripPacket_close_1AD0(void* ctx, int mode) __asm__("lineStripPacket_close");

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

extern "C" bool cPS2GraphicsMan_clipLineSegment(void* self, sVtx1AD0* v, sPt1AD0* p, int* codes);

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
 * Recomputes the matrices (cPS2GraphicsMan_computeMatrices) when mMatricesValid is 0 and opens a packet builder on mVifCursor
 * (lineStripPacket_open). Each position is transformed by mScreenWorld; x and y are divided by w (whose reciprocal is kept as
 * Q) and get clip codes against the guard band mClipMin / mClipMax (x 2/1, y 8/4, z 0x20/0x10; a negative w flips
 * them). A segment with both ends inside extends the open strip (lineStripPacket_addVertex); one that enters or leaves the band is
 * clipped with cPS2GraphicsMan_clipLineSegment and the inside part is drawn, starting a new strip (lineStripPacket_beginStrip) where it enters; a
 * segment with both ends outside but not on the same side is clipped and drawn as a strip of its own if anything is
 * left. Finally the builder is closed and its packet queued on the defer manager (lineStripPacket_close). cPS2GraphicsMan_vtbl
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
        self->computeMatrices();
    }
    prevCode = -1;
    lineStripPacket_open(&ctx, (unsigned int*)(&self->mVifCursor));
    for (i = 0; i < n; i++, vtx++) {
        sPt1AD0 cur = MulMat1AD0((sMat1AD0*)(&self->mScreenWorld), vtx->pos);
        int code = 0;
        cur.w = 1.0f / cur.w;
        cur.x *= cur.w;
        cur.y *= cur.w;
        if (cur.x < self->mClipMin.x) code |= 2;
        if (self->mClipMax.x < cur.x) code |= 1;
        if (cur.y < self->mClipMin.y) code |= 8;
        if (self->mClipMax.y < cur.y) code |= 4;
        if (cur.w < 0.0f) {
            code ^= 0x2F;
        } else {
            if (cur.z > self->mClipMin.z) code |= 0x20;
            if (cur.z < self->mClipMax.z) code |= 0x10;
        }
        if (code == 0) {
            if (prevCode == 0) {
                lineStripPacket_addVertex((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
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
                cPS2GraphicsMan_clipLineSegment(((char*)self), edgeVerts, edgePoints, codes);
                lineStripPacket_beginStrip((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[0], (sPartGsPos*)&edgePoints[0]);
                lineStripPacket_addVertex((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
            } else {
                lineStripPacket_beginStrip((sPartGsCtx*)&ctx, (sPartGsVtx*)vtx, (sPartGsPos*)&cur);
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
            cPS2GraphicsMan_clipLineSegment(((char*)self), edgeVerts, edgePoints, codes);
            lineStripPacket_addVertex((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[1], (sPartGsPos*)&edgePoints[1]);
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
            if (cPS2GraphicsMan_clipLineSegment(((char*)self), edgeVerts, edgePoints, codes)) {
                lineStripPacket_beginStrip((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[0], (sPartGsPos*)&edgePoints[0]);
                lineStripPacket_addVertex((sPartGsCtx*)&ctx, (sPartGsVtx*)&edgeVerts[1], (sPartGsPos*)&edgePoints[1]);
            }
        }
        prev = cur;
        prevCode = code;
    }
    lineStripPacket_close_1AD0(&ctx, 2);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderSpline);
#ifdef SKIP_ASM





extern char D_3BF0[];

// PORT: PS2-only VU0 inline asm (matrix copy); the PC port needs a C fallback.
static inline void CopyMat_1F10(t4Matrix* dst, const t4Matrix* src)
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

extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);

/**
 * Draws a cubic spline on PS2: writes a GIF/VIF packet with the colour (scaled by 128), the point count and @p BezMat into
 * the uncached render buffer, and records it as a DeferMan entry while the entry count is below 0xA28.
 *
 * @param BezMat  the Bezier basis matrix (On Tour BezMat)
 * @param col  the colour as four floats a, r, g, b in 0..1
 * @param n  the number of points
 */
// PORT: uncached (0x30000000) pointers held in int; 64-bit GS words are `unsigned long`.
extern "C" void cPS2GraphicsMan_RenderSpline(cPS2GraphicsMan* self, t4Matrix* BezMat, float* col, int n)
{
    self->m_pCurrentDrawState->m_bitGroups[0] = (((int)(self->m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0xC0;
    (*(int*)&self->m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    unsigned int buf = self->mVifCursor;
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
    CopyMat_1F10((t4Matrix*)(packet + 10), BezMat);
    packet[18] = n;
    packet[19] = 0;
    packet[20] = 0;
    packet[21] = (unsigned long)(((unsigned int)D_3BF0 >> 3) | 0x14000000) << 32;
    packet += 22;
    self->mVifCursor = (unsigned int)packet & 0x0FFFFFFF;
    cPS2DeferManager* ring = self->mpDeferMan;
    unsigned int next = self->mVifCursor;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
    }
    self->mVifCursor += 0x10;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_DrawSphere);
#ifdef SKIP_ASM




extern t4Vector D_004FFBC0[];
extern int D_004A59A0;

// PORT: PS2-only VU0 inline asm (matrix copy); the PC port needs a C fallback.
static inline void CopyMat2170(t4Matrix* dst, const t4Matrix* src)
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
static inline t4Vector MulMat2170(const t4Matrix* m, const t4Vector& v)
{
    t4Vector r;
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
static inline t4Vector Scale2170(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector Add2170(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
 * @param pos     the centre
 * @param col     the colour, four floats A, R, G, B (On Tour tARGB)
 * @param radius  the radius
 * @param segs    the detail step, or 0 to pick it from the distance
 */
void cPS2GraphicsMan::DrawSphere(t4Vector* pos, float* col, float radius, int segs)
{
    t4Matrix viewMat;
    int step;
    int i;
    int k;

    CopyMat2170(&viewMat, (t4Matrix *)GetView());
    t4Vector viewCenter = MulMat2170(&viewMat, *pos);
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
    m_pCurrentDrawState->m_textureState.m_texid[0] = -1;
    m_pCurrentDrawState->m_bitGroups[1] = (((int)(m_pCurrentDrawState)->m_bitGroups[1]) & ~0x7C) | 4;
    PushView();
    IdentityView();

    tBxVertex verts[17];
    i = 0;
    for (k = 0; k < 16; k += step) {
        verts[i].r = (int)(col[1] * 128.0f);
        verts[i].g = (int)(col[2] * 128.0f);
        verts[i].b = (int)(col[3] * 128.0f);
        verts[i].a = (int)(col[0] * 128.0f);
        t4Vector circlePoint = Add2170(viewCenter, Scale2170(D_004FFBC0[k], radius));
        verts[i].p = circlePoint;
        i++;
    }
    verts[i].r = (int)(col[1] * 128.0f);
    verts[i].g = (int)(col[2] * 128.0f);
    verts[i].b = (int)(col[3] * 128.0f);
    verts[i].a = (int)(col[0] * 128.0f);
    t4Vector circlePoint = Add2170(viewCenter, Scale2170(D_004FFBC0[0], radius));
    verts[i].p = circlePoint;
    i++;
    RenderLineStrip(i, verts, 0);
    m_pCurrentDrawState->m_bitGroups[1] = (((int)(m_pCurrentDrawState)->m_bitGroups[1]) & ~0x7C) | 4;
    PopView();
}
#endif

//100%
INCLUDE_ASM("render/particle", intc_handleVblankStart);
#ifdef SKIP_ASM
extern "C" void cPS2GraphicsMan_handleVblank(cPS2GraphicsMan* arg);

/**
 * Handles the VBlank-start interrupt: runs cPS2GraphicsMan_handleVblank on the graphics manager and returns 1.
 *
 * cPS2GraphicsMan_Init installs it as the INTC handler for cause 2 with the graphics manager as its argument (handler
 * id in mIntcHandlerId). Any other @p cause hits a break 0 trap. It re-enables interrupts (sync.l; ei) before
 * returning.
 *
 * @param cause  the interrupt cause (must be 2)
 * @param graphicsMan  the cPS2GraphicsMan the handler was installed with
 * @return 1
 */
extern "C" int intc_handleVblankStart(int cause, void* graphicsMan)
{
    if (cause != kINTC_VBLANK_S) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    ((cPS2GraphicsMan*)graphicsMan)->handleVblank();
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_handleVblank);
#ifdef SKIP_ASM
extern "C" void iSignalSema(int sema);

/**
 * Counts a vertical blank and, once a finished frame has waited mMinFlipWait blanks, wakes the DMA kick thread to flip.
 *
 * Increments mVblankCounter. When mKickState is 4 (both DMA phases done) and the counter has reached mMinFlipWait, sets
 * mKickState to 5, clears the counter and signals mKickSema (iSignalSema). Called from the VBlank handler
 * intc_handleVblankStart.
 */
void cPS2GraphicsMan::handleVblank()
{
    int vblanks = ++mVblankCounter;
    if (mKickState == cPS2GraphicsMan::kKick_Phase2Done) {
        if (vblanks >= mMinFlipWait) {
            mKickState = cPS2GraphicsMan::kKick_FlipDue;
            mVblankCounter = 0;
            iSignalSema(mKickSema);
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", dmac_handleVif1End);
#ifdef SKIP_ASM
extern "C" void cPS2GraphicsMan_handleVif1DmaEnd(cPS2GraphicsMan* arg);

/**
 * Handles the end of a VIF1 DMA transfer: runs cPS2GraphicsMan_handleVif1DmaEnd on the graphics manager and returns 1.
 *
 * cPS2GraphicsMan_Init installs it as the DMAC handler for channel 1 (VIF1) with the graphics manager as its argument
 * (handler id in mDmacHandlerId). Any other @p cause hits a break 0 trap. It re-enables interrupts (sync.l; ei) before
 * returning.
 *
 * @param cause  the DMA channel that finished (must be 1)
 * @param graphicsMan  the cPS2GraphicsMan the handler was installed with
 * @return 1
 */
extern "C" int dmac_handleVif1End(int cause, void* graphicsMan)
{
    if (cause != kDMAC_VIF1) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    ((cPS2GraphicsMan*)graphicsMan)->handleVif1DmaEnd();
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_handleVif1DmaEnd);
#ifdef SKIP_ASM
extern "C" void iSignalSema(int sema);

/**
 * Advances the frame's DMA state when a VIF1 transfer ends.
 *
 * mKickState 1 (first phase sent) becomes 2 and mKickSema is signalled (iSignalSema) so the kick thread starts the
 * second phase; 3 (second phase sent) becomes 4, after which cPS2GraphicsMan_handleVblank waits for the flip. Called from the VIF1 DMA
 * handler dmac_handleVif1End.
 */
void cPS2GraphicsMan::handleVif1DmaEnd()
{
    if (mKickState == cPS2GraphicsMan::kKick_Phase1Sent) {
        mKickState = cPS2GraphicsMan::kKick_Phase1Done;
        iSignalSema(mKickSema);
    } else if (mKickState == cPS2GraphicsMan::kKick_Phase2Sent) {
        mKickState = cPS2GraphicsMan::kKick_Phase2Done;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_claimBuildBuffer);
#ifdef SKIP_ASM
/**
 * Tells whether the packet buffer being built (mBufState[mBuildIndex]) is free or already being built, marking a free
 * one as being built (kBuf_Free -> kBuf_Building); false while it is queued or being sent.
 *
 * cPS2GraphicsMan_IsRenderReady__FPv returns its result.
 *
 * @return true when rendering into the buffer may start
 */
bool cPS2GraphicsMan::claimBuildBuffer()
{
    volatile int* arr = (volatile int*)(&mBufState[0]);
    volatile int* bufState = &arr[mBuildIndex];
    if (*bufState == cPS2GraphicsMan::kBuf_Free || *bufState == cPS2GraphicsMan::kBuf_Building) {
        if (*bufState == cPS2GraphicsMan::kBuf_Free) {
            *bufState = cPS2GraphicsMan::kBuf_Building;
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_isKickIdle);
#ifdef SKIP_ASM
/**
 * Tells whether no frame is being sent (mKickState is kKick_Idle).
 *
 * cPS2GraphicsMan_IsRenderIdle__FPv returns its result.
 *
 * @return true when the DMA kick state is idle
 */
bool cPS2GraphicsMan::isKickIdle()
{
    return (volatile unsigned int)mKickState == cPS2GraphicsMan::kKick_Idle;
}
#endif

extern "C" void* cPS2GraphicsMan_runDmaKickThread(cPS2GraphicsMan* self);

//100%
INCLUDE_ASM("render/particle", dma_runKickThread__FPv);
#ifdef SKIP_ASM
/**
 * Entry point of the DMA kick thread: runs cPS2GraphicsMan_runDmaKickThread on the graphics manager, which never returns.
 *
 * cPS2GraphicsMan_Init creates the thread with this entry and its stack in mKickThreadStack and starts it with the
 * graphics manager as its argument.
 *
 * @return never returns
 */
void* dma_runKickThread(void* self)
{
    return ((cPS2GraphicsMan*)self)->runDmaKickThread();
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_runDmaKickThread);
#ifdef SKIP_ASM


extern t4Vector D_0044B9C0;
extern "C" void* WaitSema(int sema);
extern "C" void SignalSema(int sema);
extern "C" void FlushCache(int);
extern "C" void cPS2GraphicsMan_flipDisplayBuffers(cPS2GraphicsMan* self, int a, int b);

/**
 * Runs the DMA kick thread: sends each queued frame to the GS in two VIF1 + GIF DMA phases and flips the display when
 * it is done.
 *
 * Loops forever: waits on mKickSema, then takes mQueueMutex. When mKickState is 5 (frame sent and the flip wait over)
 * it frees the in-flight buffer (mBufState[mInFlightIndex] = 0), sets the state to 0 and flips with cPS2GraphicsMan_flipDisplayBuffers(self,
 * 1, 0). When idle and the queued buffer is ready (mBufState[mQueuedIndex] == 2) it waits for the VIF1 and GIF
 * channels, VIF1 and the GS FIFO to go idle, makes that buffer the in-flight one (state 3), copies its second-phase
 * addresses (mInFlightVif2, mInFlightGif2), sets mKickState to 1, flushes the cache, pushes the quadword D_0044B9C0
 * into the VIF1 FIFO, sets GIF_MODE to 4 and starts the GIF (mQueuedGif1) and VIF1 (mQueuedVif1) chain DMAs. When
 * mKickState is 2 (first phase done) it calls mPhase2Callback(mPhase2CallbackArg) if set, does the same FIFO/GIF_MODE
 * setup, starts the second-phase chains and sets the state to 3. Then releases mQueueMutex. The interrupt handlers
 * (cPS2GraphicsMan_handleVif1DmaEnd, cPS2GraphicsMan_handleVblank) move the state on and signal mKickSema.
 *
 * @param selfp  the cPS2GraphicsMan
 * @return never returns
 */
// Renderer_DmaKickThread in REF structs/render.md.
// PORT: PS2 hardware registers (D1/D2/D8 CHCR+TADR, VIF1 FIFO/STAT, GIF_MODE, GS CSR).
void* cPS2GraphicsMan::runDmaKickThread()
{
    cPS2GraphicsMan* self = this;
    for (;;) {
        WaitSema(self->mKickSema);
        WaitSema(self->mQueueMutex);
        if (self->mKickState == cPS2GraphicsMan::kKick_FlipDue) {
            self->mBufState[self->mInFlightIndex] = cPS2GraphicsMan::kBuf_Free;
            self->mKickState = cPS2GraphicsMan::kKick_Idle;
            self->flipDisplayBuffers(1, 0);
        }
        if (self->mKickState == cPS2GraphicsMan::kKick_Idle
            && self->mBufState[self->mQueuedIndex] == cPS2GraphicsMan::kBuf_Queued) {
            while (*(volatile int*)0x10009000 & 0x100) {
            }
            while (*(volatile int*)0x1000A000 & 0x100) {
            }
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & kGS_CSR_FIFO_MASK) != kGS_CSR_FIFO_EMPTY) {
            }
            self->mInFlightIndex = self->mQueuedIndex;
            self->mBufState[self->mInFlightIndex] = cPS2GraphicsMan::kBuf_Sending;
            self->mKickState = cPS2GraphicsMan::kKick_Phase1Sent;
            self->mInFlightVif2 = self->mQueuedVif2;
            self->mInFlightGif2 = self->mQueuedGif2;
            FlushCache(0);
            while (*(volatile int*)0x1000D000 & 0x100) {
            }
            *(t4Vector*)0x10005000 = D_0044B9C0;
            *(volatile int*)0x10003010 = 4;
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & kGS_CSR_FIFO_MASK) != kGS_CSR_FIFO_EMPTY) {
            }
            *(volatile int*)0x1000A030 = self->mQueuedGif1;
            *(volatile int*)0x1000A000 = 261;
            *(volatile int*)0x10009030 = self->mQueuedVif1;
            *(volatile int*)0x10009000 = 389;
        } else if (self->mKickState == cPS2GraphicsMan::kKick_Phase1Done) {
            if (self->mPhase2Callback) {
                self->mPhase2Callback(self->mPhase2CallbackArg);
            }
            *(t4Vector*)0x10005000 = D_0044B9C0;
            *(volatile int*)0x10003010 = 4;
            while (*(volatile int*)0x10003C00 & 0x1F000003) {
            }
            while ((*(volatile long*)0x12001000 & kGS_CSR_FIFO_MASK) != kGS_CSR_FIFO_EMPTY) {
            }
            *(volatile int*)0x1000A030 = self->mInFlightGif2;
            *(volatile int*)0x1000A000 = 261;
            self->mKickState = cPS2GraphicsMan::kKick_Phase2Sent;
            *(volatile int*)0x10009030 = self->mInFlightVif2;
            *(volatile int*)0x10009000 = 389;
        }
        SignalSema(self->mQueueMutex);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_flipDisplayBuffers);
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
extern "C" void dma_sendGifNormal(unsigned int madr, int qwc, int flags);

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
 * mFilterEnabled clear and an even count the draw page (mCurrentFrameBufferPage) is mDisplayBufferPage and the display
 * page (mDisplayBufferAddress) mDrawBufferPage, otherwise the other way round. Waits for the VIF1, GIF and fromSPR DMA
 * channels, VIF1 and the GS FIFO to go idle, and for the next vertical sync when @p waitVsync is set. Writes PMODE (EN1
 * = @p en1, MMOD 1, ALP 0xFF), SMODE2 (interlace mInterlace, field mode mFieldMode), DISPFB1 (display page,
 * mDisplayBufferWidth width, mDisplayFormat format), DISPLAY1 (position from m_screenOffset; in progressive mode, mDTV,
 * the magnification fits the display width into 1550 clocks, else MAGH mMagH, MAGV mMagV and a height doubled in
 * interlaced field mode) and BGCOLOR 0.
 *
 * Then builds a GS packet in D_004FFCC0 (uncached accelerated): when mFilterEnabled is set, a pass that copies the draw
 * buffer into the display buffer as 32-pixel-wide textured sprite strips coloured by the brightness mBrightnessColour
 * times mScreenBrightness and alpha mScreenAlpha (blended when not 128); the draw context (FRAME_1/2, ZBUF_1/2,
 * XYOFFSET_1/2, FBA, TEXA, PRMODECONT, COLCLAMP, DTHE from mDither, DIMX from mDitherMatrix, PABE, SCANMSK, TEXFLUSH);
 * a clear of the frame and z buffer in 32-pixel sprite strips with mClearColour (z test always); TEST_1 0x500AD (alpha
 * greater than 10, z GEQUAL); and SCISSOR_1/2 from mScissor. Sends it with dma_sendGifNormal. The kick thread
 * cPS2GraphicsMan_runDmaKickThread, cPS2GraphicsMan_applyDisplayMode and cPS2GraphicsMan_PageFlip call it.
 *
 * @param vself  the cPS2GraphicsMan
 * @param en1  PMODE.EN1: 1 to enable read circuit 1
 * @param waitVsync  nonzero to wait for a vertical sync before writing the registers
 */
// Renderer_SetDisplayRegs in REF structs/render.md.
void cPS2GraphicsMan::flipDisplayBuffers(int en1, int waitVsync)
{
    ulong* gsPacket;
    int count;

    count = ++mCurrentFrameCount;
    if (mSkipNextPageFlip != 0) {
        mSkipNextPageFlip = 0;
    } else {
        if (mFilterEnabled == 0 && (count & 1) == 0) {
            mCurrentFrameBufferPage = (int)((unsigned int)mDisplayBufferPage);
            mDisplayBufferAddress = (int)((unsigned int)mDrawBufferPage);
        } else {
            mCurrentFrameBufferPage = (int)((unsigned int)mDrawBufferPage);
            mDisplayBufferAddress = (int)((unsigned int)mDisplayBufferPage);
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
    while ((*(volatile ulong*)0x12001000 & kGS_CSR_FIFO_MASK) != kGS_CSR_FIFO_EMPTY) { // GS CSR.FIFO empty
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
    *(volatile ulong*)0x12000020 = (ulong)mInterlace | ((ulong)mFieldMode << 1);
    // DISPFB1: FBP, FBW, PSM
    *(volatile ulong*)0x12000070 = (ulong)(unsigned int)mDisplayBufferAddress
                                 | ((ulong)(unsigned int)mDisplayBufferWidth << 9)
                                 | ((ulong)(unsigned int)mDisplayFormat << 15);
    // DISPLAY1: DX, DY, MAGH, MAGV, DW, DH
    if (mDTV != 0) {
        int w = (unsigned int)mDisplayWidth;
        int displayHeight = (unsigned int)mDisplayHeight;
        *(volatile ulong*)0x12000080 = (ulong)((int)(m_screenOffset[0] * 4.0f + -43.0f) + 344)
                                     | ((ulong)((int)(12.0f - m_screenOffset[1] * 2.0f) + 35) << 12)
                                     | ((ulong)(1550 / w - 1) << 23)
                                     | ((ulong)(w * (1550 / w) - 1) << 32)
                                     | ((ulong)(displayHeight - 1) << 44);
    } else {
        int fieldMul = 1;
        if (mInterlace != 0) {
            fieldMul = mFieldMode == kGS_FFMD_FRAME ? 2 : 1;
        }
        *(volatile ulong*)0x12000080 = (ulong)(m_screenOffset[0] * 8.0f + 5.0f + 636.0f)
                                     | ((ulong)(50.0f - m_screenOffset[1] * 2.0f) << 12)
                                     | ((ulong)(unsigned int)mMagH << 23)
                                     | ((ulong)(unsigned int)mMagV << 27)
                                     | ((ulong)2560 << 32)
                                     | ((ulong)(((unsigned int)mFBHeight - 1) * fieldMul) << 44);
    }
    // BGCOLOR
    *(volatile ulong*)0x120000E0 = 0;

    gsPacket = (ulong*)((unsigned int)D_004FFCC0 | 0x30000000);

    if (mFilterEnabled != 0) {
        // Copy the draw buffer into the display buffer: GIFtag (11 x A+D)
        gsPacket[0] = ((ulong)1 << 60) | 11;
        gsPacket[1] = 0xE;
        gsPacket[2] = (ulong)(unsigned int)mDisplayBufferAddress
                | ((ulong)(unsigned int)mDisplayBufferWidth << 16)
             | ((ulong)(unsigned int)mDisplayFormat << 24) | ((ulong)0xFF000000 << 32);
        gsPacket[3] = 76; // FRAME_1 (display buffer)
        gsPacket[4] = 97;
        gsPacket[5] = 20; // TEX1_1
        gsPacket[6] = (ulong)((unsigned int)mCurrentFrameBufferPage << 5) | ((ulong)(unsigned int)mFrameBufferWidth << 14)
             | ((ulong)(unsigned int)mCurrentFBFormat << 20) | ((ulong)10 << 26) | ((ulong)9 << 30);
        gsPacket[7] = 6; // TEX0_1 (draw buffer)
        gsPacket[8] = ((ulong)0x80 << 32) | 0x64;
        gsPacket[9] = 66; // ALPHA_1
        gsPacket[10] = 5;
        gsPacket[11] = 8; // CLAMP_1
        gsPacket[12] = ((ulong)((unsigned int)mDisplayWidth - 1) << 16)
                | ((ulong)((unsigned int)mDisplayHeight - 1) << 48);
        gsPacket[13] = 0x40; // SCISSOR_1
        gsPacket[14] = ((ulong)((unsigned int)mDisplayWidth - 1) << 16)
                | ((ulong)((unsigned int)mDisplayHeight - 1) << 48);
        gsPacket[15] = 65; // SCISSOR_2
        gsPacket[16] = (ulong)((0x800 - ((unsigned int)mDisplayWidth >> 1)) << 4)
              | ((ulong)((0x800 - ((unsigned int)mDisplayHeight >> 1)) << 4) << 32);
        gsPacket[17] = 0x18; // XYOFFSET_1
        gsPacket[18] = (ulong)(unsigned int)mCurrentZBufferPage | ((ulong)((unsigned int)mCurrentZBFormat - 0x30) << 24) | ((ulong)1 << 32);
        gsPacket[19] = 78; // ZBUF_1
        gsPacket[20] = 0x30000;
        gsPacket[21] = 71; // TEST_1
        gsPacket[22] = 0;
        gsPacket[23] = 0x3F; // TEXFLUSH
        {
            int abe = (unsigned int)mScreenAlpha != kGSAlphaOne;
            // GIFtag: 1 loop, REGLIST {PRIM, RGBAQ}
            gsPacket[24] = ((ulong)2 << 60) | ((ulong)1 << 58) | 1;
            gsPacket[25] = 0x10;
            gsPacket[26] = ((ulong)abe << 6) | 0x11E; // sprite, IIP, TME, FST
        }
        {
            int scaledRed = clamp_382AF0((int)((float)(unsigned int)mBrightnessColour[0]
                    * mScreenBrightness), 0, 255);
            int scaledGreen = clamp_382AF0((int)((float)(unsigned int)mBrightnessColour[1]
                    * mScreenBrightness), 0, 255);
            int scaledBlue = clamp_382AF0((int)((float)(unsigned int)mBrightnessColour[2]
                    * mScreenBrightness), 0, 255);
            union { float f; int i; } unitQ;
            unitQ.f = 1.0f;
            gsPacket[27] = (ulong)scaledRed | ((ulong)scaledGreen << 8) | ((ulong)scaledBlue << 16)
                    | ((ulong)(unsigned int)mScreenAlpha << 24)
                  | ((ulong)unitQ.i << 32);
        }
        {
            // Vertical sprite strips, 32 pixels wide.
            unsigned int w = (unsigned int)mDisplayWidth;
            int stripW = 32;
            int n = w / stripW;
            int du = ((unsigned int)mFBWidth / n) << 4;
            int x0 = gsCoord_382AF0(-(w >> 1));
            int ybase = 0x8000 - (((unsigned int)mDisplayHeight >> 1) << 4);
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
            if (mDTV != 0) {
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
            stripV1 = (((unsigned int)mFBHeight - 1) << 4) + 8;
            y1 = y0 + (((unsigned int)mDisplayHeight - 1) << 4);
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
            gsPacket[0] = (ulong)uoff | ((ulong)(((unsigned int)mFBHeight << 4) - 8) << 16);
            gsPacket[1] = (ulong)(unsigned int)gsCoord_382AF0(-((unsigned int)mDisplayWidth >> 1))
                 | ((ulong)((((unsigned int)mDisplayHeight >> 1) << 4) + 32744) << 16) | ((ulong)0x8000 << 32);
            gsPacket[2] = (ulong)((unsigned int)mFBWidth << 4) | ((ulong)(((unsigned int)mFBHeight << 4) - 8) << 16);
            gsPacket[3] = (ulong)((((unsigned int)mDisplayWidth >> 1) << 4) + 0x8000)
                 | ((ulong)((((unsigned int)mDisplayHeight >> 1) << 4) + 0x7FF8) << 16) | ((ulong)0x8000 << 32);
            gsPacket += 4;
        }
    }

    // Draw context: GIFtag (16 x A+D, EOP)
    gsPacket[0] = ((ulong)1 << 60) | 0x8000 | 16;
    gsPacket[1] = 0xE;
    gsPacket[2] = (ulong)(unsigned int)mCurrentFrameBufferPage | ((ulong)(unsigned int)mFrameBufferWidth << 16)
         | ((ulong)(unsigned int)mCurrentFBFormat << 24);
    gsPacket[3] = 76; // FRAME_1
    gsPacket[4] = (ulong)(unsigned int)mCurrentFrameBufferPage | ((ulong)(unsigned int)mFrameBufferWidth << 16)
         | ((ulong)(unsigned int)mCurrentFBFormat << 24) | ((ulong)0xFF000000 << 32);
    gsPacket[5] = 77; // FRAME_2
    gsPacket[6] = (ulong)(unsigned int)mCurrentZBufferPage | ((ulong)((unsigned int)mCurrentZBFormat - 0x30) << 24);
    gsPacket[7] = 78; // ZBUF_1
    gsPacket[8] = (ulong)(unsigned int)mCurrentZBufferPage | ((ulong)((unsigned int)mCurrentZBFormat - 0x30) << 24);
    gsPacket[9] = 79; // ZBUF_2
    gsPacket[10] = (ulong)((0x800 - ((unsigned int)mFBWidth >> 1)) << 4)
          | ((ulong)((0x800 - ((unsigned int)mFBHeight >> 1)) << 4) << 32);
    gsPacket[11] = 0x18; // XYOFFSET_1
    gsPacket[12] = (ulong)((0x800 - ((unsigned int)mFBWidth >> 1)) << 4)
          | ((ulong)((0x800 - ((unsigned int)mFBHeight >> 1)) << 4) << 32);
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
    gsPacket[24] = (unsigned int)mDither;
    gsPacket[25] = 69; // DTHE
    gsPacket[26] = mDitherMatrix;
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
        unsigned int tw = (unsigned int)mFBWidth;
        unsigned int th = (unsigned int)mFBHeight;
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
        gsPacket[6] = (ulong)(mClearColour | 0x80000000) | ((ulong)0x3F800000 << 32);
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
    gsPacket[2] = (ulong)mScissor[0]
         | ((ulong)(mScissor[0] + (mScissor[2] - 1)) << 16)
         | ((ulong)mScissor[1] << 32)
         | ((ulong)(mScissor[1] + (mScissor[3] - 1)) << 48);
    gsPacket[3] = 0x40;
    gsPacket[4] = (ulong)mScissor[0]
         | ((ulong)(mScissor[0] + (mScissor[2] - 1)) << 16)
         | ((ulong)mScissor[1] << 32)
         | ((ulong)(mScissor[1] + (mScissor[3] - 1)) << 48);
    gsPacket[5] = 65;
    gsPacket += 6;
    {
        unsigned int size = ((unsigned int)gsPacket & 0x0FFFFFFF) - (unsigned int)D_004FFCC0;
        __asm__ __volatile__("sync.l"); // PORT: EE memory barrier before the DMA kick
        dma_sendGifNormal((unsigned int)D_004FFCC0, size >> 4, 6);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_uploadImageToDrawBuffer);
#ifdef SKIP_ASM
extern "C" void gs_uploadImage(void *, int, int, int, int, int, int, int, int);
extern "C" void FlushCache(int);

/**
 * Uploads an image from main memory into the current draw buffer at (@p destX, @p destY) and returns 1.
 *
 * Flushes the data cache and calls gs_uploadImage with the draw buffer (mCurrentFrameBufferPage << 5 blocks), mFBWidth
 * and mCurrentFBFormat. Nothing in the retail code references it.
 *
 * @param destX  destination x in pixels
 * @param destY  destination y in pixels
 * @param width  image width in pixels
 * @param height  image height in pixels
 * @param src  address of the pixels
 * @return 1
 */
int cPS2GraphicsMan::uploadImageToDrawBuffer(int destX, int destY, int width, int height, int src)
{
    FlushCache(0);
    gs_uploadImage(this, src, mCurrentFrameBufferPage << 5, width, height, mFBWidth, mCurrentFBFormat, destX, destY);
    return 1;
}
#endif
//100%
INCLUDE_ASM("render/particle", gs_uploadImage);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Uploads an image to GS memory: works out bytes per row from the pixel format, waits for DMA, then builds
// GIF/DMA packets in scratchpad in chunks of at most 0x7FFF0 bytes and kicks the transfer.
extern "C" void dma_sendGifChain(unsigned int tadr, int flags);

/**
 * Uploads an image to GS memory with GIF image transfers built in the scratchpad, in chunks of at most 0x7FFF0 bytes.
 *
 * Bytes per row come from @p psm (PSMCT32 4, PSMCT24 3, PSMCT16/16S 2, PSMT8 1, PSMT4 1/2; other formats give 0). Waits
 * for the GIF DMA channel, then writes at 0x70000000 per chunk: a DMA cnt tag, an A+D GIF tag with BITBLTBUF (@p dbp,
 * @p dbw / 64, @p psm), TRXPOS (@p dsax, @p dsay), TRXREG (@p w x rows), TEXFLUSH and TRXDIR 0, an IMAGE GIF tag, and a
 * DMA ref (or refe on the last chunk) to the rows of @p src. Kicks the chain with dma_sendGifChain(0x70000000, 6).
 * cPS2GraphicsMan_uploadImageToDrawBuffer calls it.
 *
 * @param src  address of the pixels
 * @param dbp  destination base pointer, in 256-byte blocks
 * @param w  width in pixels
 * @param height  height in pixels
 * @param dbw  destination buffer width in pixels
 * @param psm  GS pixel format of the data (eGSPixelStorage)
 * @param dsax  destination x
 * @param dsay  destination y
 */
extern "C" void gs_uploadImage(void* self, int src, int dbp, int w, int height, int dbw, int psm, int dsax, int dsay)
{
    int bpr = 0;
    unsigned long* packet = (unsigned long*)0x70000000;
    unsigned long bitbltbuf;
    int id = 3;

    switch (psm) {
    case kGS_PSMCT32:
        bpr = w << 2;
        break;
    case kGS_PSMCT24:
        bpr = w * 3;
        break;
    case kGS_PSMCT16:
    case kGS_PSMCT16S:
        bpr = w << 1;
        break;
    case kGS_PSMT8:
        bpr = w;
        break;
    case kGS_PSMT4:
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

    dma_sendGifChain(0x70000000, 6);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_clipLineSegment);
#ifdef SKIP_ASM



extern "C" bool cPS2GraphicsMan_clipLineSegment_impl(cPS2GraphicsMan* self, tBxVertex* v, t4Vector* p, int* codes)
    __asm__("cPS2GraphicsMan_clipLineSegment");

/**
 * Clips the segment between two projected points to the guard band, moving the outside end(s) onto it, and returns
 * false when nothing is left.
 *
 * @p codes holds the two ends' clip codes from cPS2GraphicsMan_RenderLineStrip. For each plane crossed (far 0x20 and near 0x10 in w, then
 * y min 8 and max 4, then x min 2 and max 1, bounds from mClipMin / mClipMax) the end outside is moved to the crossing:
 * x, y, z and w of @p points are interpolated and s, t and the four colour ints of @p verts are interpolated with the
 * same fraction (for the w planes the fraction is corrected by the w ratio); that end's codes are recomputed, and it
 * returns false as soon as both ends are outside the same plane. cPS2GraphicsMan_RenderLineStrip calls it.
 *
 * @param verts  the two vertices (s, t, colour), updated in place
 * @param points  the two projected points, updated in place
 * @param codes  the two clip codes, updated in place
 * @return true when part of the segment is inside, false when it is all outside
 */
bool cPS2GraphicsMan::clipLineSegment(tBxVertex* verts, t4Vector* points, int* codes)
{
    int unionCodes = codes[0] | codes[1];

    if (unionCodes & 0x20) {
        float w0 = points[0].w;
        float plane = mClipMin.w;
        float frac = (w0 - plane) / (w0 - points[1].w);
        int k = (codes[0] & 0x20) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dy = frac * (points[0].y - points[1].y);
        float tv = frac * points[1].w / plane;
        points[k].x = points[0].x - dx;
        points[k].y = points[0].y - dy;
        points[k].z = mClipMin.z;
        points[k].w = mClipMin.w;
        verts[k].u = verts[0].u + tv * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + tv * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + tv * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + tv * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + tv * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + tv * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
        if (points[k].x < mClipMin.x) codes[k] |= 2;
        if (points[k].x > mClipMax.x) codes[k] |= 1;
        if (points[k].y < mClipMin.y) codes[k] |= 8;
        if (points[k].y > mClipMax.y) codes[k] |= 4;
        if (codes[0] & codes[1]) return false;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 0x10) {
        float w0 = points[0].w;
        float plane = mClipMax.w;
        float frac = (w0 - plane) / (w0 - points[1].w);
        int k = (codes[0] & 0x10) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dy = frac * (points[0].y - points[1].y);
        float tv = frac * points[1].w / plane;
        points[k].x = points[0].x - dx;
        points[k].y = points[0].y - dy;
        points[k].z = mClipMax.z;
        points[k].w = mClipMax.w;
        verts[k].u = verts[0].u + tv * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + tv * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + tv * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + tv * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + tv * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + tv * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
        if (points[k].x < mClipMin.x) codes[k] |= 2;
        if (points[k].x > mClipMax.x) codes[k] |= 1;
        if (points[k].y < mClipMin.y) codes[k] |= 8;
        if (points[k].y > mClipMax.y) codes[k] |= 4;
        if (codes[0] & codes[1]) return false;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 8) {
        float frac = (points[0].y - mClipMin.y) / (points[0].y - points[1].y);
        int k = (codes[0] & 8) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = points[0].x - dx;
        points[k].y = mClipMin.y;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].u = verts[0].u + frac * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + frac * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + frac * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + frac * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + frac * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + frac * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
        if (points[k].x < mClipMin.x) codes[k] |= 2;
        if (points[k].x > mClipMax.x) codes[k] |= 1;
        if (codes[0] & codes[1]) return false;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 4) {
        float frac = (points[0].y - mClipMax.y) / (points[0].y - points[1].y);
        int k = (codes[0] & 4) == 0;
        float dx = frac * (points[0].x - points[1].x);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = points[0].x - dx;
        points[k].y = mClipMax.y;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].u = verts[0].u + frac * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + frac * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + frac * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + frac * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + frac * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + frac * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
        if (points[k].x < mClipMin.x) codes[k] |= 2;
        if (points[k].x > mClipMax.x) codes[k] |= 1;
        if (codes[0] & codes[1]) return false;
        unionCodes = codes[0] | codes[1];
    }
    if (unionCodes & 2) {
        float plane = mClipMin.x;
        float frac = (points[0].x - plane) / (points[0].x - points[1].x);
        int k = (codes[0] & 2) == 0;
        float dy = frac * (points[0].y - points[1].y);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = plane;
        points[k].y = points[0].y - dy;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].u = verts[0].u + frac * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + frac * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + frac * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + frac * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + frac * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + frac * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
    }
    if (unionCodes & 1) {
        float plane = mClipMax.x;
        float frac = (points[0].x - plane) / (points[0].x - points[1].x);
        int k = (codes[0] & 1) == 0;
        float dy = frac * (points[0].y - points[1].y);
        float dz = frac * (points[0].z - points[1].z);
        float dw = frac * (points[0].w - points[1].w);
        points[k].x = plane;
        points[k].y = points[0].y - dy;
        points[k].z = points[0].z - dz;
        points[k].w = points[0].w - dw;
        verts[k].u = verts[0].u + frac * (verts[1].u - verts[0].u);
        verts[k].v = verts[0].v + frac * (verts[1].v - verts[0].v);
        *(int*)&((unsigned int*)&verts[k].r)[0] = (int)((float)((unsigned int*)&verts[0].r)[0] + frac * (float)(((unsigned int*)&verts[1].r)[0] - ((unsigned int*)&verts[0].r)[0]));
        *(int*)&((unsigned int*)&verts[k].r)[1] = (int)((float)((unsigned int*)&verts[0].r)[1] + frac * (float)(((unsigned int*)&verts[1].r)[1] - ((unsigned int*)&verts[0].r)[1]));
        *(int*)&((unsigned int*)&verts[k].r)[2] = (int)((float)((unsigned int*)&verts[0].r)[2] + frac * (float)(((unsigned int*)&verts[1].r)[2] - ((unsigned int*)&verts[0].r)[2]));
        *(int*)&((unsigned int*)&verts[k].r)[3] = (int)((float)((unsigned int*)&verts[0].r)[3] + frac * (float)(((unsigned int*)&verts[1].r)[3] - ((unsigned int*)&verts[0].r)[3]));
        codes[k] = 0;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("render/particle", lineStripPacket_open);
#ifdef SKIP_ASM
/**
 * Starts a packet builder that writes at the packet cursor *@p src: stores @p src, the cursor as an
 * uncached-accelerated address (| 0x30000000), and no packet and no open strip.
 *
 * cPS2GraphicsMan_RenderLineStrip opens one on mVifCursor; lineStripPacket_close closes it.
 *
 * @param src  where the packet cursor lives (mVifCursor)
 * @return the builder
 */
extern "C" void* lineStripPacket_open(void* self, unsigned int* src)
{
    *(unsigned int**)((char*)self + 0x0) = src;
    *(unsigned int*)((char*)self + 0x4) = *src | 0x30000000;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", lineStripPacket_closeStrip);
#ifdef SKIP_ASM
/**
 * Closes the builder's open line strip: pads it to a quadword and fills in its two GIF tags.
 *
 * When the vertex count (+0x10) is odd one 64-bit zero is appended. At the strip's reserved slot (+0xC) it writes a
 * REGLIST tag for PRIM with value 0x4A (Gouraud, alpha-blended line strip) and a REGLIST tag of count loops of RGBAQ,
 * ST, XYZ2 with EOP, then advances the slot past them. lineStripPacket_beginStrip and lineStripPacket_close call it.
 */
// PORT: 64-bit `ulong` packet words.
extern "C" void lineStripPacket_closeStrip(void* self)
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
INCLUDE_ASM("render/particle", lineStripPacket_close);
#ifdef SKIP_ASM
struct sPtRS_4E50 {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words); [U] word 2
                                    //      bits 0..4 = key
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

struct sPtGfx_4E50 {
    char pad_0x0[0xE84];
    sPtRS_4E50* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    cPS2DeferManager* ring;         // 0x18F0
};

struct sPktBuilder_4E50 {
    unsigned int* where;            // 0x0
    unsigned int cur;               // 0x4 (uncached)
    unsigned int start;             // 0x8 (uncached)
    int open;                       // 0xC
};

extern sPtGfx_4E50* cGraphicsMan_TheGraphicsMan_4E50 __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void lineStripPacket_closeStrip(void* self);
void operator_delete(int*);

/**
 * Closes a packet builder: finishes its packet and queues it on the defer manager, then frees the builder when bit 0 of
 * @p flags is set.
 *
 * When a packet was started (+0x8): closes the open strip (lineStripPacket_closeStrip), writes a DMA cnt tag and a VIF DIRECT code
 * covering the packet at its start, and, while the defer manager (mpDeferMan) holds fewer than 2600 entries, adds a
 * 0x80-byte entry with a copy of *m_pCurrentDrawState (its low 5 bits of word +0x8 set from the defer manager's scissor
 * index mActivePortIndex), the packet's start and end (physical addresses), the manager's view index
 * (mActiveViewIndex) and flag 0. It then moves the cursor one quadword on and writes it back to the cursor it was
 * opened on. cPS2GraphicsMan_RenderLineStrip calls it with 2.
 *
 * @param flags  bit 0: delete the builder afterwards
 */
// PORT: uncached (0x30000000) pointers held in int; 64-bit GS words are `ulong`.
extern "C" void lineStripPacket_close(sPktBuilder_4E50* self, int flags)
{
    if (self->start != 0) {
        if (self->open != 0)
            lineStripPacket_closeStrip(self);
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
        sPtGfx_4E50* graphicsMan = cGraphicsMan_TheGraphicsMan_4E50;
        cPS2DeferManager* ring = graphicsMan->ring;
        sPtRS_4E50* rs = graphicsMan->top;
        if (ring->mNumSubmitDMA < 2600) {
            rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
            int key = ring->mActiveViewIndex;
            sPtEnt_4E50* deferEntry = (sPtEnt_4E50*)((ring->mNumSubmitDMA++ << 7)
                + ((unsigned int)ring->mSubmits | 0x30000000));
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
INCLUDE_ASM("render/particle", lineStripPacket_addVertex);
#ifdef SKIP_ASM
// The line-strip packet builder (lineStripPacket_close's sPktBuilder_4E50 names the same words; names ours, from use).
struct sPartGsCtx {
    int where;           // 0x0 [U] where lineStripPacket_close stores the final cursor
    ulong* p;            // 0x4
    int packetStart;     // 0x8 [U] the packet's DMA / VIF header, 0 before the first strip
    int stripTags;       // 0xC [U] the open strip's tags, 0 when none is open
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
 * cPS2GraphicsMan_RenderLineStrip and lineStripPacket_beginStrip call it.
 *
 * @param ctx  the packet builder
 * @param vtx  the vertex (s, t, colour)
 * @param pos  the projected position (x, y, z and q = 1 / w)
 */
// PORT: 64-bit `ulong` GS packet words; float bits reinterpreted via *(int*)&f.
extern "C" void lineStripPacket_addVertex(sPartGsCtx* ctx, sPartGsVtx* vtx, sPartGsPos* pos)
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
INCLUDE_ASM("render/particle", lineStripPacket_beginStrip);
#ifdef SKIP_ASM
/**
 * Starts a new line strip in the packet builder at the given vertex.
 *
 * When the builder has no packet yet, reserves two quadwords for its DMA and VIF header (+0x8); otherwise closes the
 * open strip (lineStripPacket_closeStrip). Reserves three quadwords for the new strip's tags (+0xC), resets the vertex count and
 * appends the vertex with lineStripPacket_addVertex. cPS2GraphicsMan_RenderLineStrip calls it.
 *
 * @param ctx  the packet builder
 * @param stripVertex  the first vertex (s, t, colour)
 * @param pos  its projected position
 */
// PORT: packet pointers held in int fields (packetStart / stripTags).
extern "C" void lineStripPacket_beginStrip(sPartGsCtx* ctx, sPartGsVtx* stripVertex, sPartGsPos* pos)
{
    if (ctx->packetStart == 0) {
        ulong* packetStart = ctx->p;
        ctx->packetStart = (int)packetStart;
        ctx->p = packetStart + 4;
    } else if (ctx->stripTags != 0) {
        lineStripPacket_closeStrip(ctx);
    }
    ulong* stripTags = ctx->p;
    ctx->count = 0;
    ctx->stripTags = (int)stripTags;
    ctx->p = stripTags + 6;
    lineStripPacket_addVertex(ctx, stripVertex, pos);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_cPS2GridMesh);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern void* cPS2GridMesh_vtbl[];
extern char str_GridMeshMem[];
extern "C" int cPS2GridMesh_splitIntoBatches(void* self, int rows, int pitch);
extern "C" void func_00386128(void* self, int rows, int pitch, char* dst, char* src);


// PORT: the unit declares cPS2GridMesh_cPS2GridMesh(void*, void*, void*, void*, unsigned int); the body takes ints.
extern "C" cPS2GridMesh* cPS2GridMesh_cPS2GridMesh_impl(cPS2GridMesh* self, int w, int h, int a3, int allocFlags) __asm__("cPS2GridMesh_cPS2GridMesh");
/**
 * Constructs a grid mesh of @p w x @p gridHeight vertices drawn as h - 1 strips, with two vertex buffers to update and draw in
 * turn.
 *
 * Sets the vtable cPS2GridMesh_vtbl, the size (+0x190, +0x192), the strip count (+0x1BC = h - 1), the vertices per strip
 * (+0x1C0 = 2w), @p graphicsMan (+0x1C4), the range 20000 (+0x194) and the position scale 32767 / 20000 (+0x198). cPS2GridMesh_splitIntoBatches
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
extern "C" cPS2GridMesh* cPS2GridMesh_cPS2GridMesh_impl(cPS2GridMesh* self, int w, int gridHeight, int graphicsMan, int allocFlags)
{
    int pitch = w * 2;
    int rows = gridHeight - 1;
    self->mVtbl = cPS2GridMesh_vtbl;
    self->m_iNumStrip = rows;
    self->m_iStripLen = pitch;
    *(int*)&self->m_pGfxMan = graphicsMan;
    self->mScale = 20000.0f;
    self->mPosToShort = 1.638350009918213f;
    self->m_iWidth = w;
    self->m_iHeight = gridHeight;
    int size = cPS2GridMesh_splitIntoBatches(self, rows, pitch);
    char* buf = (char*)operator_new_tag(size * 2 + (self->m_iNumStrip << 5), str_GridMeshMem, allocFlags, 0);
    self->m_pMem = buf;
    if (buf == 0) {
        self->mbOK = 0;
    } else {
        char* bufCursor = buf + size;
        int rowCount = self->m_iNumStrip;
        self->mPacketBuffer[1] = bufCursor;
        bufCursor += size;
        *(char**)&self->mStripTable[0] = bufCursor;
        self->mbOK = 1;
        bufCursor += rowCount * 16;
        *(char**)&self->mStripTable[1] = bufCursor;
        self->mPacketBuffer[0] = buf;
        func_00386128(self, rowCount, self->m_iStripLen, ((char*)self->mStripTable[0]), buf);
        func_00386128(self, self->m_iNumStrip, self->m_iStripLen, ((char*)self->mStripTable[1]),
                      self->mPacketBuffer[1]);
        self->m_bUpdate = 0;
        self->mScreenBuffer = -1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", cGridMesh__cGridMesh);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGarbageMan_vtbl[];

/**
 * Destroys a grid mesh's base part: stores the base vtable cGarbageMan_vtbl (+0x4) and frees the object when bit 0 of
 * @p flags is set.
 *
 * Slot 1 of the abstract grid-mesh vtable (whose other slots are pure virtual, apart from cGridMesh_SetScale__FPv);
 * cPS2GridMesh__cPS2GridMesh calls it.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cGridMesh__cGridMesh(cGridMesh* self, int flags)
{
    self->mVtbl = (void**)cGarbageMan_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cPS2GridMesh__cPS2GridMesh(cPS2GridMesh* self, int flags)
{
    self->mVtbl = cPS2GridMesh_vtbl;
    if (self->m_pMem != 0) {
        cMemMan_free(self->m_pMem);
    }
    cGridMesh__cGridMesh((cGridMesh*)((int*)self), flags);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update);
#ifdef SKIP_ASM

extern "C" void cPS2GridMesh_storePosition(cPS2GridMesh* self, short* out, int i, t3Vector* v);

/**
 * Updates the mesh's vertex positions: writes them into the strips of the buffer not in use and marks the mesh updated.
 *
 * For each strip i (the rows of vertices i and i + 1) of the strip table mStripTable picked by the buffer index
 * (mScreenBuffer; the other buffer), stores the positions of row i and row i + 1 alternately with cPS2GridMesh_storePosition (scaled
 * to shorts). Sets the updated flag m_bUpdate, so the next render swaps buffers. Slot 3 of the grid-mesh vtable
 * cPS2GridMesh_vtbl.
 *
 * @param verts  the grid positions, row by row (x, y, z floats)
 */
void cPS2GridMesh::Update(t3Vector* verts)
{
    int idx = mScreenBuffer == 0;
    for (int i = 0; i < m_iNumStrip; i++) {
        int w = m_iWidth;
        tGridMeshStrip* row = &mStripTable[idx][i];
        int base = i * w;
        int next = base + w;
        int k = 0;
        int j = 0;
        if (j < m_iWidth) {
            t3Vector* lowerVert = &verts[next];
            t3Vector* upperVert = &verts[base];
            do {
                this->storePosition(row->mPositions, k, upperVert);
                this->storePosition(row->mPositions, k + 1, lowerVert);
                k += 2;
                upperVert++;
                lowerVert++;
                j++;
            } while (j < m_iWidth);
        }
    }
    m_bUpdate = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update1);
#ifdef SKIP_ASM

struct sShortUV;

extern "C" void cPS2GridMesh_storeUV(cPS2GridMesh* self, sShortUV* dst, int idx, float* src);

/**
 * Updates the mesh's texture coordinates: writes them into the strips of the buffer not in use and marks the mesh
 * updated.
 *
 * For each strip i, picked as in cPS2GridMesh_Update, stores the u, v of row i and row i + 1 alternately with cPS2GridMesh_storeUV
 * (fixed point, 4096 = 1.0). Sets the updated flag m_bUpdate. Slot 4 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param uvs  the grid texture coordinates, row by row (u, v floats)
 */
void cPS2GridMesh::Update1(t2Vector* uvs)
{
    int idx = mScreenBuffer == 0;
    for (int i = 0; i < m_iNumStrip; i++) {
        int w = m_iWidth;
        tGridMeshStrip* row = &mStripTable[idx][i];
        int base = i * w;
        int next = base + w;
        int k = 0;
        int j = 0;
        if (j < m_iWidth) {
            // PORT: pointers in int (retail adds the offset first; &uvs[next] adds uvs first).
            t2Vector* lowerUv = (t2Vector*)((next << 3) + (int)uvs);
            t2Vector* upperUv = (t2Vector*)((base << 3) + (int)uvs);
            do {
                cPS2GridMesh_storeUV(this, row->mUVs, k, (float*)upperUv);
                cPS2GridMesh_storeUV(this, row->mUVs, k + 1, (float*)lowerUv);
                k += 2;
                upperUv++;
                lowerUv++;
                j++;
            } while (j < m_iWidth);
        }
    }
    m_bUpdate = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Update2);
#ifdef SKIP_ASM
extern "C" void cPS2GridMesh_storePosition(cPS2GridMesh* self, short* out, int i, t3Vector* v);
extern "C" void cPS2GridMesh_storeColour(cPS2GridMesh* self, unsigned short* buf, int idx, const float* c);



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
 * For each strip i, picked as in cPS2GridMesh_Update, stores the colour (cPS2GridMesh_storeColour, 16-bit) and position (cPS2GridMesh_storePosition)
 * of row i and row i + 1 alternately. Sets the updated flag +0x1A4. Slot 5 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param pos  the grid positions, row by row (x, y, z floats)
 * @param col  the grid colours, row by row (a, r, g, b floats, 0..1)
 */
extern "C" void cPS2GridMesh_Update2(sPartGrid5530* self, t3Vector* pos, tARGB* col)
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
            t3Vector* nextPos = &pos[nextRowStart];
            t3Vector* curPos = &pos[rowStart];
            tARGB* nextCol = (tARGB*)((nextRowStart << 4) + (int)col);  // PORT: pointer in int
            tARGB* curCol = (tARGB*)((rowStart << 4) + (int)col);  // PORT: pointer in int
            do {
                ((cPS2GridMesh*)self)->storeColour(strip->col, k, ((float*)&curCol->A));
                ((cPS2GridMesh*)self)->storePosition(strip->pos, k, (t3Vector*)curPos);
                ((cPS2GridMesh*)self)->storeColour(strip->col, k + 1, ((float*)&nextCol->A));
                ((cPS2GridMesh*)self)->storePosition(strip->pos, k + 1, (t3Vector*)nextPos);
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
extern "C" void cPS2GridMesh_storeUV_uv(void* self, void* dst, int idx, float* src) __asm__("cPS2GridMesh_storeUV");

extern "C" void cPS2GridMesh_storePosition_vt(void* self, void* dst, int idx, float* src) __asm__("cPS2GridMesh_storePosition");

extern "C" void cPS2GridMesh_storeColour_col(void* self, void* dst, int idx, float* src) __asm__("cPS2GridMesh_storeColour");

/**
 * Updates the mesh's positions, colours and texture coordinates; on the first call fills both buffers.
 *
 * When the buffer index (mScreenBuffer) is still -1 it becomes 0 and the call counts as the first. For each strip of
 * the buffer not in use it stores, for row i and row i + 1 alternately, the uv (cPS2GridMesh_storeUV), colour (cPS2GridMesh_storeColour)
 * and position (cPS2GridMesh_storePosition); on the first call it then fills buffer 0's strips the same way. Sets the updated flag
 * m_bUpdate. Slot 6 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param verts  the grid positions, row by row (x, y, z floats)
 * @param colors  the grid colours, row by row (a, r, g, b floats, 0..1)
 * @param uvs  the grid texture coordinates, row by row (u, v floats)
 */
void cPS2GridMesh::Update3(float* verts, float* colors, float* uvs)
{
    int first = 0;
    if (mScreenBuffer < 0) {
        mScreenBuffer = 0;
        first = 1;
    }
    int z = mScreenBuffer == 0;
    int rowIndex;
    for (rowIndex = 0; rowIndex < m_iNumStrip; rowIndex++) {
        tGridMeshStrip* strip = &mStripTable[z][rowIndex];
        int topIdx = rowIndex * m_iWidth;
        int bottomIdx = topIdx + m_iWidth;
        int idx = 0;
        for (int col = 0; col < m_iWidth; col++) {
            cPS2GridMesh_storeUV_uv(this, strip->mUVs, idx, uvs + topIdx * 2);
            cPS2GridMesh_storeColour_col(this, strip->mColours, idx, colors + topIdx * 4);
            cPS2GridMesh_storePosition_vt(this, strip->mPositions, idx, verts + topIdx * 3);
            cPS2GridMesh_storeUV_uv(this, strip->mUVs, idx + 1, uvs + bottomIdx * 2);
            cPS2GridMesh_storeColour_col(this, strip->mColours, idx + 1, colors + bottomIdx * 4);
            cPS2GridMesh_storePosition_vt(this, strip->mPositions, idx + 1, verts + bottomIdx * 3);
            idx += 2;
            topIdx++;
            bottomIdx++;
        }
    }
    // First update: fill the other buffer too. The binary has a second copy of the loop sharing the
    // row counter (one spill slot) and indexing rows[z] with z reset to 0, so it is written out here.
    if (first) {
        z = 0;
        for (rowIndex = 0; rowIndex < m_iNumStrip; rowIndex++) {
            tGridMeshStrip* strip = &mStripTable[z][rowIndex];
            int topIdx = rowIndex * m_iWidth;
            int bottomIdx = topIdx + m_iWidth;
            int idx = 0;
            for (int col = 0; col < m_iWidth; col++) {
                cPS2GridMesh_storeUV_uv(this, strip->mUVs, idx, uvs + topIdx * 2);
                cPS2GridMesh_storeColour_col(this, strip->mColours, idx, colors + topIdx * 4);
                cPS2GridMesh_storePosition_vt(this, strip->mPositions, idx, verts + topIdx * 3);
                cPS2GridMesh_storeUV_uv(this, strip->mUVs, idx + 1, uvs + bottomIdx * 2);
                cPS2GridMesh_storeColour_col(this, strip->mColours, idx + 1, colors + bottomIdx * 4);
                cPS2GridMesh_storePosition_vt(this, strip->mPositions, idx + 1, verts + bottomIdx * 3);
                idx += 2;
                topIdx++;
                bottomIdx++;
            }
        }
    }
    m_bUpdate = 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_storePosition);
#ifdef SKIP_ASM

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
void cPS2GridMesh::storePosition(short* out, int i, t3Vector* src)
{
    float scale = mPosToShort;
    t3Vector scaled;
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
INCLUDE_ASM("render/particle", cPS2GridMesh_storeColour);
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
void cPS2GridMesh::storeColour(unsigned short* buf, int idx, const float* colour)
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
INCLUDE_ASM("render/particle", cPS2GridMesh_storeUV);
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
extern "C" void cPS2GridMesh_storeUV(cPS2GridMesh* self, sShortUV* dst, int idx, float* src)
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
    virtual void Render1(int, int, int);
};

/**
 * Draws the mesh with a material's texture: calls virtual slot 8 (cPS2GridMesh_Render1) with @p meshMatrix, the texture
 * id of @p materialBlock's first material (m_sTexture) and @p texMatrix.
 *
 * Slot 7 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @param meshMatrix  the object matrix
 * @param materialBlock  the material list (On Tour const tMaterialBlockRecord* material)
 * @param texMatrix  passed on as cPS2GridMesh_Render1's last argument (a texture matrix, or 0)
 */
void cPS2GridMesh::Render(int meshMatrix, const tMaterialBlockRecord* materialBlock, int texMatrix)
{
    ((cPartVirt*)this)->Render1(meshMatrix, materialBlock->GetMaterialRecordList()[0]->m_sTexture, texMatrix);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GridMesh_Render1);
#ifdef SKIP_ASM
struct sPtRS_5C10 {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words); [U] word 0
                                    //      bits 6..9 = layer, word 2 bits 0..4 = key
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

struct sPtGfx_5C10 {
    char pad_0x0[0xE84];
    sPtRS_5C10* top;                // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    cPS2DeferManager* ring;         // 0x18F0
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
static inline sMat_5C10 ScaleMat_5C10(const sMat_5C10& m, const t4Vector& v)
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
extern "C" unsigned long* cPS2ScratchPad_claimBuffer(int dma, unsigned int buf, int a2, int a3);
extern "C" unsigned int cPS2ScratchPad_flushBufferTo(int dma, unsigned long* p, int a2);
extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);
extern "C" void vif_appendObjectConstants_5C10(sPtGfx_5C10* gfx, sMat_5C10* m, sMat_5C10* hdr, t4Vector* scale, int flag, int a5, int prim, char** pp) __asm__("vif_appendObjectConstants");

/**
 * Draws the mesh with texture @p tex: queues a call of its current vertex buffer on the defer manager.
 *
 * When the mesh was updated since the last draw (+0x1A4) it clears the flag and swaps the buffer index (+0x1A0).
 * Recomputes the matrices (cPS2GraphicsMan_computeMatrices) when mMatricesValid is 0. Sets the current draw state's (m_pCurrentDrawState)
 * bits 6-9 of word 0 to 3, clears the low 30 bits of word +0xC and sets its texture to @p tex. Opens a packet at
 * mVifCursor (cPS2ScratchPad_claimBuffer on D_004A4474) and writes vif_appendObjectConstants's VU constants: *@p mat scaled by the mesh range
 * (+0x194) in x, y and z, the unscaled matrix, @p flag as the extra matrix, and flags 34 when @p flag is set else 32;
 * then a DMA call to the current buffer (+0x1AC or +0x1B0). Closes the packet (cPS2ScratchPad_flushBufferTo), adds a 0x80-byte defer
 * manager entry as lineStripPacket_close does (when fewer than 2600) and moves mVifCursor past it. Slot 8 of the grid-mesh
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
        ((cPS2GraphicsMan*)self->gfx)->computeMatrices();
    }
    self->gfx->top->m_bitGroups[0] = (self->gfx->top->m_bitGroups[0] & ~0x3C0) | 0xC0;
    self->gfx->top->m_bitGroups[3] &= 0xC0000000;
    self->gfx->top->tex = tex;
    unsigned int addr = self->gfx->bufAddr;
    sMat_5C10 baseMatrix = *mat;
    float meshRange = self->scale;
    t4Vector sc;
    sc.x = meshRange;
    sc.y = meshRange;
    sc.z = meshRange;
    sc.w = 1.0f;
    sMat_5C10 scaledMatrix = ScaleMat_5C10(*mat, sc);
    char* packetCursor = (char*)cPS2ScratchPad_claimBuffer(D_004A4474, addr, -1, 0);
    vif_appendObjectConstants_5C10(self->gfx, &scaledMatrix, &baseMatrix, &sc, flag, 0, flag ? 34 : 0x20, &packetCursor);
    *(ulong*)packetCursor = ((ulong)self->tags[self->buf] << 32) | 0x50000000;
    *(ulong*)(packetCursor + 8) = 0;
    packetCursor += 0x10;
    addr = cPS2ScratchPad_flushBufferTo(D_004A4474, (unsigned long*)packetCursor, 4);
    cPS2DeferManager* ring = self->gfx->ring;
    sPtRS_5C10* rs = self->gfx->top;
    unsigned int cur = self->gfx->bufAddr;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sPtEnt_5C10* deferEntry = (sPtEnt_5C10*)((ring->mNumSubmitDMA++ << 7)
            + ((unsigned int)ring->mSubmits | 0x30000000));
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
INCLUDE_ASM("render/particle", cPS2GridMesh_splitIntoBatches);
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

int cPS2GridMesh_splitIntoBatches_impl(sPkt_5EB0* self, int n, int m) __asm__("cPS2GridMesh_splitIntoBatches");
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
int cPS2GridMesh_splitIntoBatches_impl(sPkt_5EB0* self, int n, int stripVerts)
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
INCLUDE_ASM("render/particle", list_pushFront);
#ifdef SKIP_ASM
/**
 * Pushes @p node onto the front of the singly linked list at *@p head (link at +0x8).
 *
 * Nothing in the retail code references it.
 *
 * @param node  the object to add
 * @param head  the list head
 */
extern "C" void list_pushFront(void *node, void **head) {
    (*(void **)((char*)(node) + 8)) = (void *) *head;
    *head = node;
}
#endif
//100%
INCLUDE_ASM("render/particle", list_deleteAll);
#ifdef SKIP_ASM

/**
 * Deletes every object of the singly linked list starting at @p head (link at +0x8), calling each one's virtual
 * destructor (vtable at +0x4, slot 1) with flags 3.
 *
 * The next link is read before each delete. Nothing in the retail code references it.
 *
 * @param head  the first object, or 0
 */
extern "C" void list_deleteAll(void* head)
{
    void* node = head;
    if (node != 0) {
        do {
            void* next = *(void**)((char*)node + 8);
            if (node != 0) {
                sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)node + 4);
                vt[1].fn(vthis(node, vt[1]), 3);
            }
            node = next;
        } while (node != 0);
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_UpdateFullScreenEffects);
#ifdef SKIP_ASM
extern "C" void cPS2GraphicsMan_copyDepthFog(void* self, int a1);
extern "C" void cPS2GraphicsMan_copyScreenTint(void* self, int a1);
extern "C" void cPS2GraphicsMan_copyLightGlow(void* self, int a1);

/**
 * Updates the full-screen effects of a view: cPS2GraphicsMan_copyDepthFog, cPS2GraphicsMan_copyScreenTint and cPS2GraphicsMan_copyLightGlow, each with @p view.
 *
 * @param view  the view index (On Tour View)
 */
void cPS2GraphicsMan::UpdateFullScreenEffects(int view)
{
    cPS2GraphicsMan_copyDepthFog(this, view);
    cPS2GraphicsMan_copyScreenTint(this, view);
    cPS2GraphicsMan_copyLightGlow(this, view);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_RenderFullScreenEffects);
#ifdef SKIP_ASM
// The 8-byte scissor rectangle (x, y, w, h halfwords) as the two words retail copies it in.
struct sPartPair {
    int a, b;
};

// Copies one view's scissor rectangle into another, as two words.
static inline void CopyScissor(unsigned short* dst, const unsigned short* src)
{
    *(sPartPair*)dst = *(const sPartPair*)src;
}

// PORT: PS2-only VU0 asm (lqc2/sqc2 matrix copy).
static inline void CopyMatrix_FSE(const t4Matrix* src, t4Matrix* dst)
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
 * Records view @p i for the full-screen effects rather than drawing anything: copies mScreenMatrix to
 * mViewScreenMatrix[i] and the scissor to mViewScissor[i], and sets mNumViews = i + 1.
 *
 * @param i  the view index (On Tour View)
 */
void cPS2GraphicsMan::RenderFullScreenEffects(int i)
{
    // the helper's argument forms &mViewScreenMatrix[i] index first, as retail does
    CopyMatrix_FSE(&mScreenMatrix, &mViewScreenMatrix[i]);
    CopyScissor(mViewScissor[i], mScissor);
    mNumViews = i + 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_SetLensFXCallBack__FPvii);
#ifdef SKIP_ASM
/**
 * Sets the per-frame callback that the DMA kick thread (cPS2GraphicsMan_runDmaKickThread) calls with @p callbackArg before sending each
 * frame's second DMA phase: mPhase2Callback = @p callback, mPhase2CallbackArg = @p callbackArg.
 *
 * cPS2GraphicsMan_vtbl slot 117; cLensFxMan_cLensFxMan installs cLensFxMan__drawFrameCallBack__FPv with it.
 *
 * @param callback  the function to call, or 0
 * @param callbackArg  the argument passed to it
 */
void cPS2GraphicsMan_SetLensFXCallBack(void* vself, int callback, int callbackArg)
{
    cPS2GraphicsMan* self = (cPS2GraphicsMan*)vself;
    self->mPhase2Callback = (void (*)(int))callback;
    self->mPhase2CallbackArg = callbackArg;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_ResolveCharModel);
#ifdef SKIP_ASM
extern "C" int cGraphicsMan_GetTextureID(void* self, const char* name);
extern "C" int strncmp(const char* a, const char* b, int n);
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


struct sRef_66F0 {
    int n;
    int* idx;
};


// The model header (On Tour tMdfModelHeader, 0x50; its DWARF lists no members): field names ours, from use.
struct sMdl_66F0 {
    char pad0[0x18];
    int matsOffset;                 // 0x18 [U] the 0x50-byte material records' offset from the model data
    char pad1c[4];
    int bonesOffset;                // 0x20 [U] the bone table's offset
    char pad24[0xC];
    int refsOffset;                 // 0x30 [U] the reference table's offset (resolved into refs)
    int blendsOffset;               // 0x34 [U] the blended-matrix table's offset (resolved into blends)
    tMdfBlendedMatrix* blends;
    sRef_66F0* refs;
    int morphWeights;               // 0x40 [U] stored from the last argument ([O] ResolveCharModel's
                                    //      float* morphWeights)
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
 * @p base, with every bone's animation tracks and keys, and stores @p morphWeights at +0x40. Each bone reads its
 * 32-byte name record: an alpha, flag 0x8000 for names starting with str_alph (4 chars) or "ea", an environment-map
 * number from an "envN" second name (+0x10), and the name itself is replaced by cGraphicsMan_GetTextureID's result.
 * Material matrix indices and blend indices are rebased by @p tbl; each blended matrix is registered
 * (cGraphicsMan_AddBlendedMatrix) and matched against the caller's list (areBlendedMatricesEqual) or appended to it,
 * and the reference table is remapped to the shared indices. cPS2GraphicsMan vtable slot 109.
 *
 * @param gfx    the graphics manager (this)
 * @param inModel      the model header (tMdfModelHeader*)
 * @param base   the model data the offsets are relative to
 * @param tbl    per-part bone offsets (On Tour partBoneOffset)
 * @param count  in/out: number of entries in @p slots (On Tour numBlendedMats)
 * @param slots  the shared blended-matrix list (On Tour blendedMats)
 * @param morphWeights  stored in the model at +0x40 (On Tour: float* morphWeights)
 */
extern "C" void cPS2GraphicsMan_ResolveCharModel(cPS2GraphicsMan* gfx, sMdl_66F0* inModel, char* base, int* tbl,
        int* count, tMdfBlendedMatrix* slots, int morphWeights)
{
    inModel->blends = (tMdfBlendedMatrix*)(base + inModel->blendsOffset);
    inModel->refs = (sRef_66F0*)(base + inModel->refsOffset);
    char* mats = base + inModel->matsOffset;
    sBone_66F0* bones = (sBone_66F0*)(base + inModel->bonesOffset);
    inModel->morphWeights = morphWeights;
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
        if (strncmp(nameRecord, str_alph, 4) == 0 || (nameRecord[0] == 'e' && nameRecord[1] == 'a'))
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
        tMdfBlendedMatrix* blendEntry = &inModel->blends[i];
        (*(unsigned char**)&blendEntry->m_matrixList) = (unsigned char*)(base + (int)((unsigned char*)blendEntry->m_matrixList));
        for (int j = 0; j < blendEntry->m_iNumMatrices; j++)
            ((unsigned char*)blendEntry->m_matrixList)[j * 4 + 2] += tbl[((unsigned char*)blendEntry->m_matrixList)[j * 4 + 3]];
        cGraphicsMan_AddBlendedMatrix(gfx, blendEntry);
        for (int k = 0;; k++) {
            if (k == *count) {
                slots[*count].m_iNumMatrices = blendEntry->m_iNumMatrices;
                (*(unsigned char**)&slots[*count].m_matrixList) = ((unsigned char*)blendEntry->m_matrixList);
                slots[*count].m_iMatrixId = *count;
                blendEntry->m_iMatrixId = *count;
                (*count)++;
                goto next;
            }
            if (areBlendedMatricesEqual(blendEntry, &slots[k], 0, 0, 0)) {
                blendEntry->m_iMatrixId = k;
                goto next;
            }
        }
    next:;
    }
    for (int i = 0; i < inModel->nrefs; i++) {
        sRef_66F0* refEntry = &inModel->refs[i];
        for (int j = 0; j < refEntry->n; j++)
            refEntry->idx[j] = inModel->blends[refEntry->idx[j]].m_iMatrixId;
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
 * For each of @p n entries, sums its links' matrices from @p matlist (index and weight per link) into one matrix with
 * VU0; the VU0 I register is set to 0.01 first. Output goes to @p blendedmatlist, or to mpScratchpad when
 * @p blendedmatlist is null.
 *
 * @param matlist  the bone matrices (On Tour matlist)
 * @param partBoneOffsets  not read (On Tour partBoneOffsets)
 * @param blendedMatrixTranslationTable  the blend table: per output matrix a link count and (index, weight) links
 * @param n  the number of matrices to build (On Tour numMatrices)
 * @param blendedmatlist  the output array, or null for the scratchpad (On Tour blendedmatlist)
 */
void cPS2GraphicsMan::GenerateBlendedMatrices(const t4Matrix* matlist, int* partBoneOffsets,
                                              const tMdfBlendedMatrix* blendedMatrixTranslationTable, int n,
                                              t4Matrix* blendedmatlist)
{
    sBlendMtx_6BD0* mats = (sBlendMtx_6BD0*)matlist;
    sBlendEnt_6BD0* ents = (sBlendEnt_6BD0*)blendedMatrixTranslationTable;
    sBlendMtx_6BD0* out = (sBlendMtx_6BD0*)blendedmatlist;
    if (out == 0) {
        out = (sBlendMtx_6BD0*)mpScratchpad;
    }
    setI_6BD0(0x3C23D70A);
    int i;
    for (i = 0; i < n; i++) {
        sBlendMtx_6BD0* dst = out++;
        sBlendEnt_6BD0* entry = &ents[i];
        sBlendLink_6BD0* link = entry->links;
        const sBlendMtx_6BD0* bone = &mats[link->idx];
        blendFirst_6BD0(bone, link->w);
        int cnt = entry->count;
        link++;
        int k;
        for (k = 1; k < cnt; k++) {
            const sBlendMtx_6BD0* nextBone = &mats[link->idx];
            blendAdd_6BD0(nextBone, link->w);
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
void cPS2GraphicsMan::update()
{
    cGraphicsMan_update(this);
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_drawTrailRibbons);
#ifdef SKIP_ASM

extern "C" void cPS2GraphicsMan_sendTrailRibbon(void* self, void* verts, void* cols, int count, void* a4, void* a5, void* a6, float f0, float f1);

/**
 * Draws the ribbon between rings @p firstRing and @p secondRing of each of @p n board trails with cPS2GraphicsMan_sendTrailRibbon, with the same alpha
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
void cPS2GraphicsMan::drawTrailRibbons(BoardTrailInfoT* strips, int n, void* firstRing, void* secondRing,
                                       void* textured, float alphaScale, float alphaStep)
{
    for (; n > 0; n--, strips++) {
        if (strips->stripLength >= 2) {
            cPS2GraphicsMan_sendTrailRibbon(this, ((void*)strips->verts), ((void*)strips->startIndex), strips->stripLength, firstRing, secondRing, textured, alphaScale, alphaStep);
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_drawTrailRibbonsOwnAlpha);
#ifdef SKIP_ASM
struct sPartStrip_6DD0 {
    void* verts;    // 0x0
    void* cols;     // 0x4
    int count;      // 0x8
    float alphaScale;   // 0xC (name ours, as cPS2GraphicsMan_drawTrailRibbons's) [U] the trail's alpha multiplier
    float alphaStep;    // 0x10 (name ours) [U] the trail's alpha step
};

extern "C" void cPS2GraphicsMan_sendTrailRibbon(void* self, void* verts, void* cols, int count, void* a4, void* a5, void* a6, float f0, float f1);

/**
 * Draws the ribbon between rings @p firstRing and @p secondRing of each of @p n board trails with cPS2GraphicsMan_sendTrailRibbon, using each trail's
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
extern "C" void cPS2GraphicsMan_drawTrailRibbonsOwnAlpha(void* self, sPartStrip_6DD0* strips, int n, void* firstRing, void* secondRing, void* textured)
{
    for (; n > 0; n--, strips++) {
        if (strips->count >= 2) {
            cPS2GraphicsMan_sendTrailRibbon(self, strips->verts, strips->cols, strips->count, firstRing, secondRing, textured,
                    strips->alphaScale, strips->alphaStep);
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
// then draws the strips through cPS2GraphicsMan_drawTrailRibbons / cPS2GraphicsMan_drawTrailRibbonsOwnAlpha.
// PORT: uncached (0x30000000) packet pointers held in int; ulong is 64-bit (GIF/VIF words).

struct sRS_6E78 {
    int m_bitGroups[4];             // 0x00 [O] sDrawState::m_bitGroups (the draw state's bit words)
    short tex;
    short pad;
};


struct sQ_6E78 {
    int w[4];
} __attribute__((aligned(16)));


struct sEnt_6E78 {
    sRS_6E78 rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
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
extern sRS_6E78 sDrawState_s_defaultDrawState_rs6E78 __asm__("sDrawState_s_defaultDrawState");

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

static inline void AddQuad_6E78(sPkt_6E78* pkt, const t4Vector& q)
{
    *(t4Vector*)pkt->cur = q;
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

static inline void DmaEnd_6E78(sPkt_6E78* pkt, cPS2GraphicsMan* self)
{
    pkt->dma[0] = ((pkt->cur - pkt->dma) / 2 - 1) | 0x10000000;
    pkt->dma[1] = 0;
    ulong* p = pkt->cur;
    pkt->cur = 0;
    self->mVifCursor = (unsigned int)p & 0x0FFFFFFF;
}

/**
 * Draws @p n board trails as a multi-pass destination-alpha effect and queues the passes on the defer manager.
 *
 * Resets the current draw state (m_pCurrentDrawState) to sDrawState_s_defaultDrawState, sets its layer and blend bits
 * and its texture to _EffectTexMap[D_004A45D4], recomputes the matrices (cPS2GraphicsMan_computeMatrices) when mMatricesValid is 0 and
 * reads the screen width (virtual slot 10). Each pass is a DMA packet at mVifCursor built through D_004A5B84 (VIF
 * FLUSH, A+D GS registers, optionally mGuardScale / mGuardOffset unpacked to the VU), followed by trail draws: 1) FRAME
 * writing alpha only, z writes masked, z test always, guard band, ribbon 0-1 untextured (cPS2GraphicsMan_drawTrailRibbons); 2) z test
 * GEQUAL with FBA_1 on, ribbon 0-1 again; 3) z writes on, destination alpha test, FBA_1 off, guard band with z = 0,
 * ribbon 0-1 again; 4) full FRAME, z GEQUAL, FBA_1 when D_004A45CC is set, guard band, then the textured ribbons 2-0,
 * 0-3, 3-4, 4-1 and 1-5 with each trail's alpha (cPS2GraphicsMan_drawTrailRibbonsOwnAlpha). When D_004A45CC is set a last pass (destination alpha
 * test with z GEQUAL, FBA_1 off) draws ribbon 0-1 textured and then restores the z GEQUAL test. Adds one defer manager
 * entry for all of it (when fewer than 2600), moves mVifCursor on and restores the default draw state.
 * cPS2GraphicsMan_vtbl slot 89.
 *
 * @param strips  the trails (0x14 bytes each: ring table, start index, vertex count, alpha, alpha step)
 * @param n  number of trails
 */
void cPS2GraphicsMan::RenderMultipleBoardTrails(BoardTrailInfoT* strips, int n)
{
    *((sRS_6E78*)m_pCurrentDrawState) = sDrawState_s_defaultDrawState_rs6E78;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[2] = (((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[2] & ~0x3E0)
            | 0x60;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[0] = (((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[0] & ~0x3C0)
            | 0xC0;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[3] &= 0xC0000000;
    if (mMatricesValid == 0) {
        this->computeMatrices();
    }
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1]
            = (((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] & ~0x400000) | 0x400000;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1]
            = (((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] & ~0x1800000) | 0x1000000;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] &= ~0x300000;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] &= ~0xFF000;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] = (((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[1] & ~0x7C)
            | 0x14;
    int tex = _EffectTexMap[D_004A45D4.texIndex];
    ((sRS_6E78*)m_pCurrentDrawState)->tex = tex;

    int width = GetScreenWidth();
    sPkt_6E78* pkt = (sPkt_6E78*)D_004A5B84;
    unsigned int fbp = ((unsigned int)mCurrentFrameBufferPage);
    unsigned int zbp = ((unsigned int)mCurrentZBufferPage);
    unsigned int start = mVifCursor;
    MEM_fill(pkt, 0, 0x30);

    // pass 1: alpha-only FRAME, ZBUF masked, TEST ztst always; guard band
    {
        ulong* packetBase = (ulong*)(mVifCursor | 0x30000000);
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
    AddQuad_6E78(pkt, mGuardScale);
    AddQuad_6E78(pkt, mGuardOffset);
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, this);
    this->drawTrailRibbons(strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 2: TEST ztst gequal, FBA on
    DmaBegin_6E78(pkt, mVifCursor);
    Flush_6E78(pkt);
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, 0x50000, 71);
    AddReg_6E78(pkt, 1, 74);
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    DmaEnd_6E78(pkt, this);
    this->drawTrailRibbons(strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 3: ZBUF unmasked, destination-alpha test, FBA off; guard band without depth
    DmaBegin_6E78(pkt, mVifCursor);
    Flush_6E78(pkt);
    GifBegin_6E78(pkt);
    AddReg_6E78(pkt, (ulong)(zbp | 0x1000000), 78);
    AddReg_6E78(pkt, 0x3C000, 71);
    AddReg_6E78(pkt, 0, 74);
    GifEnd_6E78(pkt);
    DirectEnd_6E78(pkt);
    UnpackBegin_6E78(pkt);
    {
        t4Vector flatGuardScale;
        t4Vector flatGuardOffset;
        flatGuardScale.x = (mGuardScale).x;
        flatGuardScale.y = (mGuardScale).y;
        flatGuardScale.z = 0.0f;
        flatGuardScale.w = (mGuardScale).w;
        flatGuardOffset.x = (mGuardOffset).x;
        flatGuardOffset.y = (mGuardOffset).y;
        flatGuardOffset.z = 0.0f;
        flatGuardOffset.w = (mGuardOffset).w;
        AddQuad_6E78(pkt, flatGuardScale);
        AddQuad_6E78(pkt, flatGuardOffset);
    }
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, this);
    this->drawTrailRibbons(strips, n, 0, (void*)1, 0, 0.0f, 0.0f);

    // pass 4: full FRAME, TEST ztst gequal, FBA if enabled; then the edges
    DmaBegin_6E78(pkt, mVifCursor);
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
    AddQuad_6E78(pkt, mGuardScale);
    AddQuad_6E78(pkt, mGuardOffset);
    UnpackEnd_6E78(pkt);
    DmaEnd_6E78(pkt, this);
    cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, (void*)2, 0, (void*)1);
    cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, 0, (void*)3, (void*)1);
    cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, (void*)3, (void*)4, (void*)1);
    cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, (void*)4, (void*)1, (void*)1);
    cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, (void*)1, (void*)5, (void*)1);

    if (D_004A45CC.fba) {
        DmaBegin_6E78(pkt, mVifCursor);
        Flush_6E78(pkt);
        GifBegin_6E78(pkt);
        AddReg_6E78(pkt, 0x54000, 71);
        AddReg_6E78(pkt, 0, 74);
        GifEnd_6E78(pkt);
        DirectEnd_6E78(pkt);
        DmaEnd_6E78(pkt, this);
        cPS2GraphicsMan_drawTrailRibbonsOwnAlpha((void*)this, (sPartStrip_6DD0*)strips, n, 0, (void*)1, (void*)1);

        DmaBegin_6E78(pkt, mVifCursor);
        Flush_6E78(pkt);
        GifBegin_6E78(pkt);
        AddReg_6E78(pkt, 0x50000, 71);
        GifEnd_6E78(pkt);
        DirectEnd_6E78(pkt);
        DmaEnd_6E78(pkt, this);
    }

    cPS2DeferManager* ring = mpDeferMan;
    unsigned int next = mVifCursor;
    sRS_6E78* rs = ((sRS_6E78*)m_pCurrentDrawState);
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (rs->m_bitGroups[2] & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        sEnt_6E78* deferEntry = (sEnt_6E78*)((ring->mNumSubmitDMA++ << 7)
            + ((unsigned int)ring->mSubmits | 0x30000000));
        deferEntry->rs = *rs;
        deferEntry->buf = start;
        deferEntry->next = next;
        deferEntry->key = key;
        deferEntry->flag = 0;
    }
    mVifCursor += 0x10;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[0] &= ~0x3C0;
    ((sRS_6E78*)m_pCurrentDrawState)->m_bitGroups[3] &= 0xC0000000;
    *((sRS_6E78*)m_pCurrentDrawState) = sDrawState_s_defaultDrawState_rs6E78;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_sendTrailRibbon);
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
void cPS2GraphicsMan_sendTrailRibbon_impl(sPtGfx_7EC0* self, sVtx_7EC0** tbl, int start, int n, int ia, int ib, int flag, float fa, float fb) __asm__("cPS2GraphicsMan_sendTrailRibbon");
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
 * cursor is stored back to mVifCursor. cPS2GraphicsMan_drawTrailRibbons and cPS2GraphicsMan_drawTrailRibbonsOwnAlpha call it.
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
void cPS2GraphicsMan_sendTrailRibbon_impl(sPtGfx_7EC0* self, sVtx_7EC0** tbl, int start, int n, int ia, int ib, int flag, float fa, float fb)
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




extern int D_004A5B84;
extern sDrawState sDrawState_s_defaultDrawState_rs __asm__("sDrawState_s_defaultDrawState");

extern "C" void cPS2GraphicsMan_computeMatrices(cPS2GraphicsMan* self);
extern "C" void cPS2GraphicsMan_sendWakeRibbon(void* self, void* a, void* b, void* c, void* d, int i0, int i1, float f0, float f1);

/**
 * Draws a board's snow wake.
 *
 * Sets the wake render mode, clears the 0x30-byte scratch at D_004A5B84 and calls cPS2GraphicsMan_sendWakeRibbon for each pair of
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
extern "C" void cPS2GraphicsMan_RenderBoardWake(cPS2GraphicsMan* self, void* verts, int numSegmentsWide, void* startIndex, void* numIndices, void* stripLength, float alphaMultiplier, float alphaDelta)
{
    self->m_pCurrentDrawState->m_bitGroups[0] = (((int)(self->m_pCurrentDrawState)->m_bitGroups[0]) & ~0x3C0) | 0xC0;
    (*(int*)&self->m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    if (self->mMatricesValid == 0) {
        self->computeMatrices();
    }
    unsigned int buf = self->mVifCursor;
    MEM_fill((void*)D_004A5B84, 0, 0x30);
    int i;
    for (i = 0; i < numSegmentsWide - 1; i++) {
        cPS2GraphicsMan_sendWakeRibbon(self, verts, startIndex, numIndices, stripLength, i, i + 1, alphaMultiplier, alphaDelta);
    }
    cPS2DeferManager* ring = self->mpDeferMan;
    unsigned int next = self->mVifCursor;
    sDrawState* rs = self->m_pCurrentDrawState;
    if (ring->mNumSubmitDMA < 2600) {
        rs->m_bitGroups[2] = (((int)rs->m_bitGroups[2]) & ~0x1F) | (ring->mActivePortIndex & 0x1F);
        int key = ring->mActiveViewIndex;
        tDeferSubmit* ringEntry = (tDeferSubmit*)((ring->mNumSubmitDMA++ << 7) + ((unsigned int)((&ring->mSubmits[0])) | 0x30000000));
        ringEntry->mDrawState = *rs;
        ringEntry->mPacket = buf;
        ringEntry->mTail = next;
        ringEntry->mViewIndex = key;
        ringEntry->mContinuation = 0;
    }
    self->mVifCursor += 0x10;
    (*(int*)&self->m_pCurrentDrawState->m_bitGroups[0]) &= ~0x3C0;
    (*(int*)&self->m_pCurrentDrawState->m_bitGroups[3]) &= 0xC0000000;
    *(self->m_pCurrentDrawState) = sDrawState_s_defaultDrawState_rs;
}
#endif

//100%
INCLUDE_ASM("render/particle", cPS2GraphicsMan_sendWakeRibbon);
#ifdef SKIP_ASM


extern int D_004A43B0;
extern char D_3A00[];
extern char D_3A08[];

// VIF UNPACK V4-32 code: VU address, qword count, FLG bit.
static inline unsigned int Unpack_85E0(int addr, int num, int flg, unsigned int cmd)
{
    return addr | ((num << 16) | (flg << 15)) | cmd;
}

// The unit declares cPS2GraphicsMan_sendWakeRibbon with void* parameters (caller cPS2GraphicsMan_RenderBoardWake); bind the real types.
extern "C" void cPS2GraphicsMan_sendWakeRibbon_impl(cPS2GraphicsMan* self, tBxVertex** strips, int start, int ring, int count, int i0, int i1,
                                   float t, float dt) __asm__("cPS2GraphicsMan_sendWakeRibbon");

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
void cPS2GraphicsMan::sendWakeRibbon(tBxVertex** strips, int start, int ring, int count, int firstRow, int secondRow,
                                     float alphaStart, float dt)
{
    unsigned int prog = (unsigned int)(D_004A43B0 ? D_3A00 : D_3A08) >> 3;
    int prim = 92;
    unsigned long regs = 1042;
    int first = 1;
    unsigned long* packetCursor = (unsigned long*)(mVifCursor | 0x30000000);
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
            *(tBxVertex*)packetCursor = strips[firstRow][k];
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
            *(tBxVertex*)packetCursor = strips[secondRow][k];
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
    mVifCursor = (unsigned int)packetCursor & 0x0FFFFFFF;
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
    self->mClearColour = packed;
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
void cPS2GraphicsMan::loadexecpurge()
{
    DisableIntc(2);
    RemoveIntcHandler(2, mIntcHandlerId);
    DisableDmac(1);
    RemoveDmacHandler(0, mDmacHandlerId);
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_setVectors);
#ifdef SKIP_ASM
/**
 * Fills @p dst with ten vectors built from 30 floats taken three at a time: vector i = (f[3i], f[3i + 1], f[3i + 2], 0)
 * and returns @p dst.
 *
 * The floats arrive in fparg0-7 and then on the stack (arg_sp0 to arg_spA8); the w words are stored as 0 (sw $zero).
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
extern "C" t4Vector* cIrradiance_setVectors(t4Vector* dst, float fparg0, float fparg1, float fparg2, float fparg3, float fparg4, float fparg5, float fparg6, float fparg7, float arg_sp0, float arg_sp8, float arg_sp10, float arg_sp18, float arg_sp20, float arg_sp28, float arg_sp30, float arg_sp38, float arg_sp40, float arg_sp48, float arg_sp50, float arg_sp58, float arg_sp60, float arg_sp68, float arg_sp70, float arg_sp78, float arg_sp80, float arg_sp88, float arg_sp90, float arg_sp98, float arg_spA0, float arg_spA8) {
    dst[1].x = fparg3;
    dst[2].x = fparg6;
    dst[3].x = arg_sp8;
    dst[4].x = arg_sp20;
    dst[5].x = arg_sp38;
    dst[6].x = arg_sp50;
    dst[7].x = arg_sp68;
    dst[8].x = arg_sp80;
    dst[9].x = arg_sp98;
    dst[0].y = fparg1;
    dst[0].x = fparg0;
    dst[1].y = fparg4;
    dst[2].y = fparg7;
    dst[3].y = arg_sp10;
    dst[4].y = arg_sp28;
    dst[5].y = arg_sp40;
    dst[6].y = arg_sp58;
    dst[7].y = arg_sp70;
    dst[8].y = arg_sp88;
    dst[9].y = arg_spA0;
    dst[0].z = fparg2;
    dst[1].z = fparg5;
    dst[2].z = arg_sp0;
    dst[3].z = arg_sp18;
    dst[4].z = arg_sp30;
    dst[5].z = arg_sp48;
    dst[6].z = arg_sp60;
    dst[7].z = arg_sp78;
    dst[8].z = arg_sp90;
    dst[9].z = arg_spA8;
    dst[0].w = 0.0f;
    dst[1].w = 0.0f;
    dst[2].w = 0.0f;
    dst[3].w = 0.0f;
    dst[4].w = 0.0f;
    dst[5].w = 0.0f;
    dst[6].w = 0.0f;
    dst[7].w = 0.0f;
    dst[8].w = 0.0f;
    dst[9].w = 0.0f;
    return dst;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_Clear);
#ifdef SKIP_ASM

/**
 * Zeroes an irradiance set: all four components of its ten coefficient vectors.
 *
 * cIrradiance_buildDefault, the front-end rider lighting in fe/feridermanager and cRider_SetRiderLights call it before adding
 * lights.
 *
 * @param vecs  the ten vectors
 */
extern "C" void cIrradiance_Clear(t4Vector* vecs)
{
    for (int i = 0; i < 10; i++) {
        vecs[i].x = vecs[i].y = vecs[i].z = vecs[i].w = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_unpackFloatRows);
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
extern "C" void cIrradiance_unpackFloatRows(float* dst, float* src)
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
INCLUDE_ASM("render/particle", cIrradiance_addLightFromDirection);
#ifdef SKIP_ASM
extern "C" void func_00389308(void* self, void* dst, float w, float x, float y, float z);

extern "C" void cIrradiance_addLightFromDirection(void* self, const float* normal, void* dst);

/**
 * Adds a light of colour @p dst arriving from direction @p normal to irradiance set @p self: calls func_00389308(self,
 * dst, 1.0, normal x, y, z).
 *
 * The parameter named dst is an input (the colour). cIrradiance_addWorldLight and cRider_SetRiderLights call it.
 *
 * @param normal  the direction the light comes from (x, y, z)
 * @param dst  the light colour (r, g, b)
 */
void cIrradiance::addLightFromDirection(const float* normal, void* dst)
{
    func_00389308(this, dst, 1.0f, normal[0], normal[1], normal[2]);
}
#endif

//100%
INCLUDE_ASM("render/particle", addVec3InPlace);
#ifdef SKIP_ASM
/**
 * Adds @p src's first three floats to @p dst's.
 *
 * cRider_SetRiderLights uses it to add a colour to the first vector of the rider's irradiance set.
 *
 * @param dst  the three floats to add to
 * @param src  the three floats to add
 */
extern "C" void addVec3InPlace(float* dst, float* src)
{
    dst[0] += src[0];
    dst[1] += src[1];
    dst[2] += src[2];
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_addTinted);
#ifdef SKIP_ASM
/**
 * Adds a tinted copy of irradiance set @p src to @p dst: each of the ten vectors gains src * @p scale times the tint, x
 * by @p weights' y, y by its z, z by its w and w by its x (the tint is a, r, g, b).
 *
 * cIrradiance_buildDefault adds the irradiance database's default entry with it, and the front-end rider lighting
 * (cFERider_render) adds the entry for the rider's name, both tinted by cIrradiance_kv4White.
 *
 * @param dst  the set to add to
 * @param src  the set to add
 * @param weights  the tint, as a, r, g, b
 * @param scale  overall factor
 */
extern "C" void cIrradiance_addTinted(t4Vector* dst, t4Vector* src, t4Vector* weights, float scale)
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
INCLUDE_ASM("render/particle", cIrradiance_addScaledW);
#ifdef SKIP_ASM

/**
 * Adds @p src's fourth components times @p dt to @p dst's, for the ten vectors of an irradiance set; x, y and z are not
 * touched.
 *
 * cIrradiance_addSunTerm adds its sun term with it.
 *
 * @param src  the set whose w components are added
 * @param dt  the factor
 */
void cIrradiance::addScaledW(cIrradiance* src, float dt)
{
    float* srcW = &src->mVectors[0].w;
    float* dstW = &mVectors[0].w;
    for (int i = 0; i < 10; i++) {
        *dstW += *srcW * dt;
        dstW += 4;
        srcW += 4;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_tint);
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
extern "C" void cIrradiance_tint(sV4_9660* vecs, float* weights)
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
extern "C" void cIrradiance_Scale(t4Vector* vecs, float* weights, float scale)
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
INCLUDE_ASM("render/particle", cIrradiance_operatorPlus);
#ifdef SKIP_ASM
struct sPartVec4x10 {
    t4Vector v[10];
};

// PORT: VU0 macro-mode vector add; the PC port needs plain C.
static inline t4Vector sPartVec4_add(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
extern "C" sPartVec4x10 cIrradiance_operatorPlus(sPartVec4x10* lhs, sPartVec4x10* rhs)
{
    sPartVec4x10 sum;
    for (int i = 0; i < 10; i++) {
        sum.v[i] = sPartVec4_add(lhs->v[i], rhs->v[i]);
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_operatorMul1);
#ifdef SKIP_ASM
// PORT: VU0 macro-mode vector scale; the PC port needs plain C.
static inline t4Vector sPartVec4_scale(const t4Vector& a, float s)
{
    t4Vector r;
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
extern "C" sPartVec4x10 cIrradiance_operatorMul1(sPartVec4x10* src, float scale)
{
    sPartVec4x10 scaled;
    for (int i = 0; i < 10; i++) {
        scaled.v[i] = sPartVec4_scale(src->v[i], scale);
    }
    return scaled;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_operatorMul);
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
extern "C" t4Vector* cIrradiance_operatorMul(t4Vector* vecs, float scale)
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
INCLUDE_ASM("render/particle", cIrradiance_buildDefault);
#ifdef SKIP_ASM
extern "C" cIrradiance* cIrradianceDataBase_Get(cIrradianceDataBase* self, int i);
extern "C" void cIrradiance_addSunTerm(t4Vector* self, void* p, float s);
extern int cIrradianceDataBase_mDefaultId;

extern cIrradianceDataBase cIrradianceDataBase_TheDataBase;
extern t4Vector cIrradiance_kv4White;

/**
 * Builds the default irradiance set in @p self: clears it, adds the irradiance database's default entry and, when
 * @p sunPos is given, a sun term.
 *
 * Clears the set (cIrradiance_Clear); when the database cIrradianceDataBase_TheDataBase has entries, adds entry cIrradianceDataBase_mDefaultId (cIrradianceDataBase_Get)
 * tinted by cIrradiance_kv4White (cIrradiance_addTinted, scale 1); when @p sunPos is not 0 adds the sun term for that position with
 * cIrradiance_addSunTerm (scale 1). setDefaultIrradiance in ai/rider calls it with @p sunPos = 0.
 *
 * @param sunPos  the sun position passed to cIrradiance_addSunTerm, or 0 for none
 */
extern "C" void cIrradiance_buildDefault(t4Vector* self, void* sunPos)
{
    cIrradiance_Clear(self);
    cIrradianceDataBase* lib = &cIrradianceDataBase_TheDataBase;
    if (lib->mNumItem > 0) {
        cIrradiance_addTinted(self, (t4Vector*)lib->Get(cIrradianceDataBase_mDefaultId), &cIrradiance_kv4White, 1.0f);
    }
    if (sunPos != 0) {
        cIrradiance_addSunTerm(self, sunPos, 1.0f);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", cIrradiance_addSunTerm);
#ifdef SKIP_ASM

struct sMat_9CB8 {
    t4Vector r[4];
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
extern char* cGraphicsMan_TheGraphicsMan_9CB8 __asm__("cGraphicsMan_TheGraphicsMan");
extern t4Vector t4vect_Origin;
extern t4Vector t4vect_XAxis_9CB8 __asm__("t4vect_XAxis");
extern t4Vector t4vect_YAxis_9CB8 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_9CB8 __asm__("t4vect_ZAxis");
extern sMat_9CB8 t4mat_Identity_9CB8 __asm__("t4mat_Identity");
extern sMat_9CB8 D_00500C60;

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C128(float x);
extern "C" float func_0031C228(float x);
extern "C" void func_00389840(t4Vector* out, sMat3_9CB8* m, int a, int b);
extern "C" void cIrradiance_addScaledW_9CB8(void* dst, t4Vector* src, float dt) __asm__("cIrradiance_addScaledW");

static inline t4Vector GetPos_9CB8(const sMat_9CB8& m)
{
    return m.r[3];
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_9CB8(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector Sub_9CB8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector Norm_9CB8(const t4Vector& v)
{
    t4Vector r;
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
static inline float Len_9CB8(const t4Vector& v)
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
static inline float Dot_9CB8(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector MulVec_9CB8(const sMat_9CB8* m, const t4Vector& v)
{
    t4Vector r;
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
static inline void Translate_9CB8(sMat_9CB8* m, const t4Vector& v)
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
static inline void RotateAxis_9CB8(sMat_9CB8* R, const t4Vector* axis, float angle)
{
    sMat_9CB8 old = *R;
    t4Vector t = old.r[3];
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
 * lobe by func_00389840; the result times @p scale * 0.7500066 is added with cIrradiance_addScaledW. cIrradiance_buildDefault calls it.
 *
 * @param out  the set to add to
 * @param sunp  the sun position
 * @param scale  factor for the sun term
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm and the
// g++ `<?` operator (retail's min.s); a port replaces them with a plain C transpose, C vector math and a ternary min.
extern "C" void cIrradiance_addSunTerm(t4Vector* out, void* sunp, float scale)
{
    t4Vector* sun = (t4Vector*)sunp;
    t4Vector prm[10];
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
    cPS2GraphicsMan* mgr = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_9CB8;
    sMat_9CB8 cam = *(sMat_9CB8*)mgr->GetView();
    t4Vector eye = Scale_9CB8(GetPos_9CB8(cam), -1.0f);
    MMI_TRANSPOSE4(&cam);
    eye = MulVec_9CB8(&cam, eye);
    eye.w = 1.0f;
    t4Vector sunDir = Norm_9CB8(Sub_9CB8(*sun, eye));
    float z = sunDir.z;
    sunDir.z = 0.0f;
    float len = Len_9CB8(sunDir);
    float elev = -func_0031C228(z / len);
    float azim;
    if (0.0f < len) {
        float normProduct = Sqrt_9CB8(Dot_9CB8(sunDir, sunDir) * Dot_9CB8(t4vect_XAxis_9CB8, t4vect_XAxis_9CB8));
        if (normProduct < 0.0010000000474974513f) {
            azim = 0.0f;
        } else {
            azim = 1.5707963705062866f - func_0031C128(Clamp_9CB8(Dot_9CB8(sunDir, t4vect_XAxis_9CB8) / normProduct, -1.0f, 1.0f));
        }
        if (sunDir.y < 0.0f) {
            azim = 6.2831854820251465f - azim;
        }
    } else {
        azim = 0.0f;
    }
    sMat_9CB8 sunFrame = t4mat_Identity_9CB8;
    Translate_9CB8(&sunFrame, eye);
    RotateAxis_9CB8(&sunFrame, &t4vect_YAxis_9CB8, -elev);
    RotateAxis_9CB8(&sunFrame, &t4vect_ZAxis_9CB8, -azim);
    MMI_TRANSPOSE4(&sunFrame);
    // NOTE: D_00500C60 is a function-static matrix and D_004A59A4 its first-use guard.
    if (D_004A59A4 == 0) {
        SetCols_9CB8(&D_00500C60, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        D_004A59A4 = 1;
    }
    sMat_9CB8 basisRotation = Mul_9CB8(D_00500C60, sunFrame);
    sMat3_9CB8 rotation3x3 = ToMat3_9CB8(basisRotation);
    func_00389840(prm, &rotation3x3, 3, 3);
    cIrradiance_addScaledW_9CB8(out, prm, scale * 0.7500065565109253f);
}
#endif

//100%
INCLUDE_ASM("render/particle", testSpotLightReach);
#ifdef SKIP_ASM
/**
 * Tests whether @p point is inside a spot light's radius; if so returns true with the unit direction to the light, the
 * distance, its inverse and the cosine against the light's direction.
 *
 * The light position is at +0x38 and the radius at +0x1C; returns false when the point is farther than the radius. When
 * the point is exactly at the light the direction stays zero and the distance and inverse are 1. @p planeD gets minus
 * the dot product of the direction with the light's direction (+0x2C), i.e. the cosine of the point's angle off the
 * beam. cIrradiance_addWorldLight calls it for type 1 lights.
 *
 * @param point  the lit point
 * @param n  receives the unit direction from the point to the light (w = 1)
 * @param forceSrc  the light
 * @param dist  receives the distance
 * @param inv  receives 1 / distance
 * @param planeD  receives the cosine against the light's direction
 * @return true when the point is within the radius
 */
extern "C" bool testSpotLightReach(float* point, float* n, tLightRecord* forceSrc, float* dist, float* inv, float* planeD)
{
    float length;
    float k;
    float dx = forceSrc->m_fvWorldPos[0] - point[0];
    float dy = forceSrc->m_fvWorldPos[1] - point[1];
    float radius = forceSrc->m_fMaxDistance;
    float dz = forceSrc->m_fvWorldPos[2] - point[2];
    float distSq = dx * dx + dy * dy + dz * dz;
    if (radius * radius < distSq) {
        return false;
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
    *planeD = -(dx * forceSrc->m_fvDirection[0] + dy * forceSrc->m_fvDirection[1] + dz * forceSrc->m_fvDirection[2]);
    *dist = length;
    *inv = k;
    n[0] = dx;
    n[1] = dy;
    n[2] = dz;
    n[3] = 1.0f;
    return true;
}
#endif

//100%
INCLUDE_ASM("render/particle", testPointLightReach);
#ifdef SKIP_ASM
/**
 * Tests whether @p point is inside a point light's radius; if so returns true with the unit direction to the light, the
 * distance and its inverse.
 *
 * The light position is at +0x38 and the radius at +0x1C; returns false when the point is farther than the radius.
 * Unlike testSpotLightReach there is no check for a zero distance before dividing. cIrradiance_addWorldLight calls it for type 2 lights.
 *
 * @param point  the lit point
 * @param n  receives the unit direction from the point to the light (w = 1)
 * @param forceSrc  the light
 * @param dist  receives the distance
 * @param inv  receives 1 / distance
 * @return true when the point is within the radius
 */
extern "C" bool testPointLightReach(float* point, float* n, tLightRecord* forceSrc, float* dist, float* inv)
{
    float dx = forceSrc->m_fvWorldPos[0] - point[0];
    float dy = forceSrc->m_fvWorldPos[1] - point[1];
    float radius = forceSrc->m_fMaxDistance;
    float dz = forceSrc->m_fvWorldPos[2] - point[2];
    float distSq = dx * dx + dy * dy + dz * dz;
    if (radius * radius < distSq) {
        return false;
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
    return true;
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradiance_addWorldLight);
#ifdef SKIP_ASM



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

static inline float fade_38A6A8(tLightRecord* s, float dist)
{
    if (s->m_fMaxDistance < 5000.0f || dist <= 3750.0f)
        return 1.0f;
    return 1.0f - (dist - 3750.0f) * 0.0008f;
}

static inline float curve_38A6A8(tLightRecord* s, float t)
{
    int c = s->m_cDecayRate;
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
 * Type (+0x10, eNEWLightType) eNEWLight_Directional adds nothing. eNEWLight_Spot: testSpotLightReach gives the direction,
 * distance and beam cosine; nothing is added outside the radius or when the cosine is below the inner limit (+0x60).
 * The strength (+0x14) is shaped by the falloff curve (+0x64: 1, 2 or 3 for (100 / distance) to that power once the
 * distance passes 100), a fade from 3750 to 5000 units for lights whose radius is at least 5000, and the beam term (the
 * cosine to the power +0x65 above the outer limit +0x5C, a linear ramp between the limits), then clamped to 0..5.
 * eNEWLight_Point: testPointLightReach, the same without the beam term. The light's colour (+0x20, which the C calls dir)
 * times that factor is added from the light's direction with cIrradiance_addLightFromDirection. cWorldLightMan_IntersectLights1 and
 * cRider_SetRiderLights call it.
 *
 * @param point  the lit point
 * @param forceSrc  the world light
 */
void cIrradiance::addWorldLight(float* point, tLightRecord* forceSrc)
{
    t4Vector n;
    t3Vector out;
    float dist;
    float inv;
    float planeD;
    float k;
    float fade;

    switch (forceSrc->m_eLightType) {
    case eNEWLight_Directional:
        break;
    case eNEWLight_Spot:
        if (!testSpotLightReach(point, (float*)&n, (tLightRecord*)forceSrc, &dist, &inv, &planeD))
            return;
        if (!(planeD >= forceSrc->mSpotOuterCos))
            return;
        fade = fade_38A6A8(forceSrc, dist);
        k = curve_38A6A8(forceSrc, inv) * forceSrc->m_fIntensity * fade;
        if (planeD >= forceSrc->mSpotInnerCos)
            k *= powi_38A6A8(planeD, forceSrc->m_cDropOff);
        else {
            k *= powi_38A6A8(forceSrc->mSpotInnerCos, forceSrc->m_cDropOff);
            k *= (planeD - forceSrc->mSpotOuterCos) / (forceSrc->mSpotInnerCos - forceSrc->mSpotOuterCos);
        }
        k = clamp_38A6A8(k, 5.0f);
        {
            float x = forceSrc->m_fvColor[0] * k;
            float y = forceSrc->m_fvColor[1] * k;
            float z = forceSrc->m_fvColor[2] * k;
            out.x = x;
            out.y = y;
            out.z = z;
        }
        cIrradiance_addLightFromDirection(this, (float*)&n, &out);
        break;
    case eNEWLight_Point:
        if (!testPointLightReach(point, (float*)&n, (tLightRecord*)forceSrc, &dist, &inv))
            return;
        fade = fade_38A6A8(forceSrc, dist);
        k = curve_38A6A8(forceSrc, inv) * forceSrc->m_fIntensity * fade;
        k = clamp_38A6A8(k, 5.0f);
        out.x = forceSrc->m_fvColor[0] * k;
        out.y = forceSrc->m_fvColor[1] * k;
        out.z = forceSrc->m_fvColor[2] * k;
        cIrradiance_addLightFromDirection(this, (float*)&n, &out);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradianceDataBase_Get);
#ifdef SKIP_ASM
/**
 * Returns irradiance entry @p i of the irradiance database (mIrradianceArray), clamping @p i to the last entry
 * (mNumItem - 1).
 *
 * cIrradiance_buildDefault and cIrradianceDataBase_Get1 call it.
 *
 * @param i  the entry index
 * @return the entry
 */
cIrradiance* cIrradianceDataBase::Get(int i)
{
    int count = mNumItem;
    if (i >= count) {
        i = count - 1;
    }
    return &mIrradianceArray[i];
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
cIrradiance* cIrradianceDataBase::Get1(const char* name)
{
    return Get(GetIDByName(name));
}
#endif

//100%
INCLUDE_ASM("render/particle", cIrradianceDataBase_GetIDByName);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);

/**
 * Looks up an irradiance entry by name.
 *
 * Compares @p name with strcmp (strcmp) against each 8-byte name entry of the table at +0xC, for the count at
 * +0x4. Returns 0 (the first entry) when nothing matches, as On Tour does. cSectionMan_setSky uses it for the sky
 * irradiance sets ("APBR1".."EPBR1").
 *
 * @param name  the irradiance name
 * @return the entry's index, or 0 when the name is unknown
 */
int cIrradianceDataBase::GetIDByName(const char* name)
{
    int i;
    for (i = 0; i < mNumItem; i++) {
        if (strcmp(name, mIrradianceNames + (i << 3)) == 0) {
            return i;
        }
    }
    return 0;
}
#endif

