#include "common.h"
#include "animation/animsequencer.h"
#include "animation/rideranimbase.h"

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_cRiderAnimBase);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cAnimSequencer_init(void* self);
extern "C" void cRiderAnimBase_reset(cRiderAnimBase* self);
extern char cRiderAnimBase_vtbl[];
extern const char str_Sequencers[];

/**
 * Constructs a rider animation player with six empty sequencers, in its reset state.
 *
 * Clears AnimModel, stores cRiderAnimBase's vtable, allocates the 0x30-byte "Sequencers" array (6 x 8 bytes,
 * each cAnimSequencer_init'ed) into Sequencer and calls cRiderAnimBase_reset().
 *
 * @return @p self
 */
extern "C" void* cRiderAnimBase_cRiderAnimBase(cRiderAnimBase* self)
{
    self->AnimModel = (cAnimModel*)0;
    self->mVtbl = (void**)cRiderAnimBase_vtbl;
    self->Sequencer = (cAnimSequencer*)operator_new_tag(0x30, str_Sequencers, 0, 0);
    for (int i = 0; i < 6; i++)
    {
        cAnimSequencer_init((char*)self->Sequencer + (i << 3));
    }
    cRiderAnimBase_reset(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase__cRiderAnimBase);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* p);
extern "C" void cAnimSequencer_expireAll(void* self);
extern char cRiderAnimBase_vtbl[];

/**
 * Destroys a rider animation player: expires every sequence and frees the sequencers, then the object when
 * asked to.
 *
 * Restores cRiderAnimBase's vtable, runs cAnimSequencer_expireAll() on each of the six sequencers and frees
 * the Sequencer array with cMemMan_free().
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRiderAnimBase__cRiderAnimBase(cRiderAnimBase* self, int flags)
{
    self->mVtbl = (void**)cRiderAnimBase_vtbl;
    for (int i = 0; i < 6; i++)
    {
        cAnimSequencer_expireAll((char*)self->Sequencer + (i << 3));
    }
    void* sequencer = self->Sequencer;
    if (sequencer)
        cMemMan_free(sequencer);
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getSequencer);
#ifdef SKIP_ASM
/**
 * Returns the address of animation sequencer @p slot of the rider animation (Sequencer + @p slot * 8).
 *
 * @param riderAnim  the cRiderAnimBase
 * @param slot  the sequencer slot, 0-5
 * @return the sequencer, as an int
 */
extern "C" int cRiderAnimBase_getSequencer(cRiderAnimBase* riderAnim, int slot) {
    return ((int)riderAnim->Sequencer) + (slot * 8);
}
#endif
//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getAIAnimSequencerName);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getAnimPlayAttr(int anim);
extern int D_0044AF80[];

/**
 * Returns the name of the sequencer that AI anim @p anim plays on.
 *
 * Word 3 of the anim's attribute record (cAIAnimEventMap_getAnimPlayAttr) is the sequencer slot; the name comes from
 * D_0044AF80: "GraftCycle", "Graft", "Body", "HeadRel2nd", "TorsoRel2nd", "LegsRel2nd".
 *
 * @param anim  the AI anim id
 * @return the sequencer name (as an int)
 */
extern "C" int cRiderAnimBase_getAIAnimSequencerName(cRiderAnimBase* self, int anim)
{
    int i = cAIAnimEventMap_getAnimPlayAttr(anim)[3];
    return D_0044AF80[i];
}
#endif
//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getAIAnimName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_00311860 is matched as (void) but this caller passes the anim in $4.
extern "C" const char* func_00311860_anim(int anim) __asm__("func_00311860");

/**
 * Returns the name of AI anim @p anim via func_00311860, which in this build always returns str_error.
 *
 * @param anim  the AI anim id (passed through; func_00311860 ignores it)
 * @return str_error
 */
extern "C" const char* cRiderAnimBase_getAIAnimName(cRiderAnimBase* self, int anim)
{
    return func_00311860_anim(anim);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_reset);
#ifdef SKIP_ASM
extern "C" void cAnimSequencer_expireAll(void* self);

struct sRabQuad32 {
    int w[8];
} __attribute__((aligned(16)));

extern sRabQuad32 tPose_Identity;


/**
 * Resets the animation player: expires all sequences and returns to the default playback state.
 *
 * Expires each sequencer and sets its CurrentAnimID to 0x1B6 (no anim), then IsFlipped = 0, FrameRateFactor =
 * 1.0, OrientationOffset = the identity pose (tPose_Identity) and GraftMask = -1 (cleared).
 *
 * @param riderAnimBase  the animation player (this)
 */
extern "C" void cRiderAnimBase_reset(cRiderAnimBase* riderAnimBase)
{
    cRiderAnimBase* self = riderAnimBase;
    for (int i = 0; i < 6; i++)
    {
        cAnimSequencer_expireAll(((char*)self->Sequencer) + (i << 3));
        self->CurrentAnimID[i] = 438;
    }
    //START
    self->IsFlipped = 0;
    self->FrameRateFactor = 1.0f;
    *(sRabQuad32*)&self->OrientationOffset = tPose_Identity;
    self->GraftMask = -1;
    //END
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getCurrentAnimCategory);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getAnimPlayAttr(int anim);

/**
 * Returns the animation category of what a rider sequencer is currently playing.
 *
 * Reads the current AI anim of sequencer @p i (+@p i * 4); 0x1B6 (no anim) gives 0, otherwise word 0 of its attribute
 * record. cAIAnimEventMap_getAnimPlayAttr returns the anim's whole attribute record (On Tour release:
 * cAIAnimEventMap::getAnimPlayAttr): word 0 is the category, 1 the update rule, 3 the sequencer and 4 the default
 * blend-in time (On Tour debug: getCategory, getUpdateRule, getSequencerID, getBlendInTime).
 *
 * @param i  the sequencer (On Tour tAIAnimSequencerID sid)
 * @return the anim category, 0 when the sequencer plays nothing
 */
extern "C" int cRiderAnimBase_getCurrentAnimCategory(cRiderAnimBase* self, int i)
{
    int anim = *(int*)((char*)self + (i << 2));
    if (anim == 438) {
        return 0;
    }
    return *cAIAnimEventMap_getAnimPlayAttr(anim);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getCurrentAnimSequence);
#ifdef SKIP_ASM
struct cAnimSequenceNode;


cAnimSequenceNode* cAnimSequencer_getSequence(cAnimSequencer* self, int index);

/**
 * Returns the newest sequence playing on one of the rider's sequencers.
 *
 * cAnimSequencer_getSequence(&sequencers[@p i], 0), with the sequencer array at +0x50.
 *
 * @param i  the sequencer (On Tour tAIAnimSequencerID sid)
 * @return the sequence, or null
 */
extern "C" cAnimSequenceNode* cRiderAnimBase_getCurrentAnimSequence(cRiderAnimBase* self, int i)
{
    cAnimSequencer* seqs = self->Sequencer;
    return cAnimSequencer_getSequence(&seqs[i], 0);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_changeHeadingOffset);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

struct sRabQuat_00311B48
{
    float x, y, z, w;
    sRabQuat_00311B48() {}
    sRabQuat_00311B48(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sRabXform_00311B48
{
    sRabQuat_00311B48 pos;
    sRabQuat_00311B48 rot;
};

extern sRabQuat_00311B48 t4vect_Origin;
extern sRabQuat_00311B48 t4vect_ZAxis;
extern "C" void cRiderAnimBase_changeOrientationOffset(cRiderAnimBase* self, sRabXform_00311B48* xf);

static inline sRabQuat_00311B48 AxisAngle_00311B48(const sRabQuat_00311B48& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sRabQuat_00311B48(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Turns the orientation offset of every playing sequence by an angle about the heading axis.
 *
 * Builds a transform with position t4vect_Origin and the rotation quaternion of -angle about t4vect_ZAxis
 * (half-angle sine and cosine from fSinCos) and applies it with
 * cRiderAnimBase_changeOrientationOffset().
 *
 * @param angle  the heading change (probably radians); applied with the opposite sign
 */
extern "C" void cRiderAnimBase_changeHeadingOffset(cRiderAnimBase* self, float angle)
{
    sRabXform_00311B48 xf;
    xf.pos = t4vect_Origin;
    xf.rot = AxisAngle_00311B48(t4vect_ZAxis, -angle);
    cRiderAnimBase_changeOrientationOffset(self, &xf);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_changeOrientationOffset);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sRabQuat_00311B48 QuatMul_00311BF0(const sRabQuat_00311B48& a, const sRabQuat_00311B48& b)
{
    sRabQuat_00311B48 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sRabQuat_00311B48 Sub_00311BF0(const sRabQuat_00311B48& a, const sRabQuat_00311B48& b)
{
    sRabQuat_00311B48 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sRabQuat_00311B48 Add_00311BF0(const sRabQuat_00311B48& a, const sRabQuat_00311B48& b)
{
    sRabQuat_00311B48 r;
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

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sRabQuat_00311B48 Rot_00311BF0(const sRabQuat_00311B48& q, const sRabQuat_00311B48& v)
{
    sRabQuat_00311B48 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

static inline sRabXform_00311B48 Combine_00311BF0(const sRabXform_00311B48& a, const sRabXform_00311B48& b)
{
    sRabXform_00311B48 r;
    r.rot = QuatMul_00311BF0(a.rot, b.rot);
    r.pos = Add_00311BF0(Rot_00311BF0(a.rot, Sub_00311BF0(b.pos, t4vect_Origin)), a.pos);
    return r;
}

/**
 * Applies a transform on top of the orientation offset of every sequence in the rider's six sequencers.
 *
 * For each sequence's offset (sequence +0x60): rotation = xf rotation * offset rotation, and position = xf
 * rotation applied to (offset position - t4vect_Origin), plus xf position. VU0 code. The OrientationOffset field
 * itself is not changed; cRiderAnimBase_changeHeadingOffset() calls this with a rotation about t4vect_ZAxis.
 *
 * @param xf  transform to apply: a position, then a rotation quaternion
 */
extern "C" void cRiderAnimBase_changeOrientationOffset(cRiderAnimBase* self, sRabXform_00311B48* xf)
{
    for (int i = 0; i < 6; i++) {
        int j = 0;
        if (j < (self->Sequencer)[i].NumPlaying) {
            do {
            sRabXform_00311B48* src = (sRabXform_00311B48*)((char*)cAnimSequencer_getSequence(&(self->Sequencer)[i], j) + 0x60);
            sRabXform_00311B48* dst = (sRabXform_00311B48*)((char*)cAnimSequencer_getSequence(&(self->Sequencer)[i], j) + 0x60);
            *dst = Combine_00311BF0(*xf, *src);
                j++;
            } while (j < (self->Sequencer)[i].NumPlaying);
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_changeIsFlipped);
#ifdef SKIP_ASM
/**
 * Sets the flip flag (+0x80) of every playing sequence in all six sequencers to @p value.
 *
 * Only the sequences change; IsFlipped of the rider animation itself is not written (cRiderAnimBase_setIsFlipped sets that).
 *
 * @param value  nonzero to mirror the animations
 */
// Sets the field at 0x80 of every sequence node of all 6 sequencers.
extern "C" void cRiderAnimBase_changeIsFlipped(cRiderAnimBase* self, int value)
{
    for (int i = 0; i < 6; i++)
    {
        int j = 0;
        if (j < ((cAnimSequencer*)(char*)self->Sequencer)[i].NumPlaying)
        {
            do
            {
                *(int*)((char*)cAnimSequencer_getSequence(&((cAnimSequencer*)(char*)self->Sequencer)[i], j++) + 0x80) = value;
            } while (j < ((cAnimSequencer*)(char*)self->Sequencer)[i].NumPlaying);
        }
    }
}
#endif
//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_removeAll);
#ifdef SKIP_ASM
/**
 * Stops everything on sequencer @p i: expires all its sequences and sets CurrentAnimID[@p i] to 0x1B6 (no anim).
 *
 * Uses cAnimSequencer_expireAll on Sequencer + @p i * 8. cRiderAnimBase_blendOutAll fades them out instead.
 *
 * @param i  the sequencer slot, 0-5
 */
// Resets sequencer i and marks its slot empty (0x1B6 = no anim, see cRiderAnimBase_getCurrentAnimCategory).
extern "C" void cRiderAnimBase_removeAll(cRiderAnimBase* self, int i)
{
    cAnimSequencer_expireAll((char*)self->Sequencer + (i << 3));
    self->CurrentAnimID[i] = 438;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_blendOutAll);
#ifdef SKIP_ASM
extern "C" void cAnimSequencer_removeAllEndTriggers(void* self, int v);
extern "C" void cAnimSequencer_fadeOutAndExpireAll(void* self, float v);

/**
 * Fades out everything one of the rider's sequencers is playing.
 *
 * On sequencer @p i: cAnimSequencer_removeAllEndTriggers (removeAllEndTriggers, id 0x3F), cAnimSequencer_fadeOutAndExpireAll (fadeOutAndExpireAll, @p blendOut), then its
 * current AI anim becomes 0x1B6 (none).
 *
 * @param i  the sequencer (On Tour tAIAnimSequencerID sid)
 * @param blendOut  the fade-out time in seconds (On Tour blendOut)
 */
extern "C" void cRiderAnimBase_blendOutAll(cRiderAnimBase* self, int i, float blendOut)
{
    cAnimSequencer_removeAllEndTriggers((char*)self->Sequencer + (i << 3), 0x3F);
    cAnimSequencer_fadeOutAndExpireAll((char*)self->Sequencer + (i << 3), blendOut);
    *(int*)(((char*)self) + (i << 2)) = 438;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_onInput);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getAnimPlayAttr(int anim);
struct cAnimSequence;
void cAnimSequence_fadeWeight(cAnimSequence* self, float targetWeight, float time);
extern "C" void cAnimSequencer_expire(void* seq, void* node);
extern "C" char* cAnimSequencer_playAndHold(cAnimSequencer* seq, int anim, unsigned int h, float t);
extern "C" char* cAnimSequencer_playAndLoop(cAnimSequencer* seq, int anim, unsigned int h, float t);
extern "C" void cAnimSequence_addEndTrigger(void* node, int key);
extern "C" void cAnimSequence_removeEndTrigger(void* node, int key);
extern "C" void cAnimSequence_fadeOutAndExpire(void* node, float v);
extern "C" void cAnimSequence_addTrigger(void* node, int key, float t);
extern "C" void cAnimSequence_setSubAnim(void* node, int i, int anim);
extern "C" void cAnimSequence_setSubAnimTime(void* node, int i, float t);
extern "C" void cAnimSequence_setSubAnimWeight(void* node, int i, float speed);
extern char** cAnimDataBase_TheAnimDataBase;

// A sequence node: three tracks, then the clip state (raw offsets from cRiderAnimBase_onInput's uses).
struct sSeqTrack_1F00 {
    int id;           // 0x00
    int anim;         // 0x04
    float time;       // 0x08
    char pad0C[0x8];
    float speed;      // 0x14
    int active;       // 0x18
};

struct sSeqNode_1F00 {
    sSeqTrack_1F00 tracks[3]; // 0x00
    char pad54[0xC];
    sRabQuad32 xform;         // 0x60
    int m80;                  // 0x80
    int m84;                  // 0x84
    long m88;                 // 0x88 PORT: 64-bit long (sd)
    float m90;                // 0x90
    float weight;             // 0x94
    float targetWeight;       // 0x98
    char pad9C[0x8];
    float blendIn;            // 0xA4
};

// Element i of a quaternion/vector (x, y, z, w), by reference (an operator[]-style accessor).
static inline const float& rabQuatElem_1F00(const sRabQuat_00311B48& q, int i)
{
    return (&q.x)[i];
}

static inline bool rabVecEq_1F00(const sRabQuat_00311B48& a, const sRabQuat_00311B48& b)
{
    return rabQuatElem_1F00(a, 0) == rabQuatElem_1F00(b, 0) && rabQuatElem_1F00(a, 1) == rabQuatElem_1F00(b, 1)
        && rabQuatElem_1F00(a, 2) == rabQuatElem_1F00(b, 2) && rabQuatElem_1F00(a, 3) == rabQuatElem_1F00(b, 3);
}

static inline bool rabXformEq_1F00(const sRabXform_00311B48& a, const sRabXform_00311B48& b)
{
    return rabVecEq_1F00(a.rot, b.rot) && rabVecEq_1F00(a.pos, b.pos);
}

/**
 * Starts an AI animation on a sequencer, blending it in over the sequences already playing.
 *
 * A zero @p blend expires everything first. Update rule 2 loops (cAnimSequencer_playAndLoop), others hold (cAnimSequencer_playAndHold), with an
 * end trigger 0x3F for rule 0 or when attribute word 2 is set. The new sequence copies the rider's offset transform and
 * its +0x18, +0x1C and +0x20 state, attribute word 6 (+0xA4) and priority @p priority, and gets a timed trigger
 * (cAnimSequence_addTrigger) per key frame of clip @p animid. Alone it fades in over attribute word 5 when that is nonzero; otherwise
 * all older sequences fade out over @p blend, and one already playing the same anim with the same transform hands its
 * sub-anims, times, speeds and weight to the new one unless @p isDupSeqAllowed allows a duplicate.
 *
 * cAIAnimEventMap_getAnimPlayAttr returns the anim's whole attribute record (On Tour release:
 * cAIAnimEventMap::getAnimPlayAttr): word 0 is the category, 1 the update rule, 3 the sequencer and 4 the default
 * blend-in time (On Tour debug: getCategory, getUpdateRule, getSequencerID, getBlendInTime).
 *
 * @param vself  the rider anim (this)
 * @param vseq  the sequencer to play on (On Tour pSeqr)
 * @param anim  the AI anim (On Tour tAIAnimID anid)
 * @param animid  the animation clip: bank in the low byte, clip above it (On Tour tAnimID animid)
 * @param isDupSeqAllowed  On Tour isDupSeqAllowed: never take over a matching sequence
 * @param priority  the sequence priority (On Tour priority)
 * @param blend  the blend-in time in seconds; 0 cuts (On Tour blendIn)
 * @return @p anim
 */
// Starts anim (packed clip id h) on sequencer vseq, blending over `blend` seconds. A sequence already
// playing the same thing (same anim, flags and offset transform) hands over its tracks and weight.
extern "C" int cRiderAnimBase_onInput(cRiderAnimBase* vself, void* vseq, int anim, int animid, int isDupSeqAllowed, int priority, float blend)
{
    cRiderAnimBase* self = vself;
    cAnimSequencer* seq = (cAnimSequencer*)vseq;
    char* node;
    if (blend == 0.0f)
        cAnimSequencer_expireAll(seq);
    if (cAIAnimEventMap_getAnimPlayAttr(anim)[1] == 2)
    {
        node = cAnimSequencer_playAndLoop(seq, anim, animid, 0.0f);
    }
    else
    {
        node = cAnimSequencer_playAndHold(seq, anim, animid, 0.0f);
        if (cAIAnimEventMap_getAnimPlayAttr(anim)[1] == 0 || cAIAnimEventMap_getAnimPlayAttr(anim)[2] != 0)
            cAnimSequence_addEndTrigger(node, 0x3F);
    }
    sSeqNode_1F00* nd = (sSeqNode_1F00*)node;
    nd->m80 = self->IsFlipped;
    nd->xform = *(sRabQuad32*)&self->OrientationOffset;
    nd->m88 = ((long)self->GraftMask);
    nd->m90 = self->FrameRateFactor;
    nd->weight = nd->targetWeight = 1.0f;
    nd->blendIn = ((float*)cAIAnimEventMap_getAnimPlayAttr(anim))[6];
    nd->m84 = priority;
    // PORT: (int) address arithmetic for the clip header (index-first addu).
    int n = *(unsigned short*)((char*)(((unsigned int)animid >> 8) * 0x14 + *(int*)(*(char**)((char*)cAnimDataBase_TheAnimDataBase + (((unsigned int)animid & 0xFF) << 2)) + 0x8)) + 0x12);
    for (int i = 0; i < n; i++)
    {
        char* bank = *(char**)((char*)cAnimDataBase_TheAnimDataBase + (((unsigned int)animid & 0xFF) << 2));
        char* clip = (char*)(((unsigned int)animid >> 8) * 0x14 + *(int*)(bank + 0x8));
        unsigned short* keys = *(unsigned short**)(bank + 0x10);
        cAnimSequence_addTrigger(node, i, keys[(*(unsigned short*)(clip + 0x10) + i) * 2] * 0.03333333507180214f);
    }
    if (seq->NumPlaying == 1)
    {
        float fadeTime = ((float*)cAIAnimEventMap_getAnimPlayAttr(anim))[5];
        if (fadeTime != 0.0f)
        {
            nd->weight = nd->targetWeight = 0.0f;
            cAnimSequence_fadeWeight((cAnimSequence*)node, 1.0f, fadeTime);
        }
    }
    else if (blend != 0.0f)
    {
        sSeqNode_1F00* match = 0;
        for (int i = 1; i < seq->NumPlaying; i++)
        {
            sSeqNode_1F00* other = (sSeqNode_1F00*)cAnimSequencer_getSequence(seq, i);
            cAnimSequence_removeEndTrigger(other, 0x3F);
            cAnimSequence_fadeOutAndExpire(other, blend);
            int type = cAIAnimEventMap_getAnimPlayAttr(anim)[1];
            bool ok = other->tracks[0].anim == nd->tracks[0].anim || (type != 0 && type != 2 && type != 1);
            if (ok && other->tracks[0].id == nd->tracks[0].id && other->m88 == nd->m88
                && other->m80 == nd->m80)
            {
                if (rabXformEq_1F00(*(sRabXform_00311B48*)&other->xform, *(sRabXform_00311B48*)&nd->xform))
                    match = other;
            }
        }
        if (match != 0 && isDupSeqAllowed == 0)
        {
            for (int k = 0; k < 3; k++)
            {
                if (match->tracks[k].active != 0)
                {
                    cAnimSequence_setSubAnim(node, k, match->tracks[k].anim);
                    cAnimSequence_setSubAnimTime(node, k, match->tracks[k].time);
                    cAnimSequence_setSubAnimWeight(node, k, match->tracks[k].speed);
                }
            }
            nd->weight = nd->targetWeight = match->weight;
            if (match->weight < 1.0f)
                cAnimSequence_fadeWeight((cAnimSequence*)node, 1.0f, blend * (1.0f - match->weight));
            cAnimSequencer_expire(seq, match);
        }
        else
        {
            nd->targetWeight = 0.0f;
            nd->weight = 0.0f;
            cAnimSequence_fadeWeight((cAnimSequence*)node, 1.0f, blend);
        }
    }
    return anim;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_updateAnimation);
#ifdef SKIP_ASM
struct sVEntry003123C0 {
    short delta;
    short index;
    void (*fn)(void*, cAnimSequencer*, cAnimSequenceNode*, float);
};

extern "C" void cRiderAnimBase_setupAnimation(cRiderAnimBase* self, int a1);

/**
 * Advances every playing animation sequence by a time step and re-blends the rider's pose.
 *
 * For each of the 6 sequencers (array at +0x50), calls vtable slot 2 (vtable at +0x58) with the sequencer, the sequence
 * and @p timescale; the next pointer (+0xC8) is read first so the call may expire the sequence. Then cRiderAnimBase_setupAnimation
 * (setupAnimation) with @p animMinParts.
 *
 * @param animMinParts  On Tour animMinParts: blend only the minimum parts
 * @param timescale  the time step (On Tour timescale)
 */
extern "C" void cRiderAnimBase_updateAnimation(cRiderAnimBase* self, int animMinParts, float timescale)
{
    for (int i = 0; i < 6; i++)
    {
        cAnimSequenceNode* node = cAnimSequencer_getSequence(&(self->Sequencer)[i], 0);
        while (node)
        {
            cAnimSequenceNode* next = *(cAnimSequenceNode**)((char*)node + 0xC8);
            sVEntry003123C0* vt = (sVEntry003123C0*)self->mVtbl;
            vt[2].fn(((char*)self) + vt[2].delta, &(self->Sequencer)[i], node, timescale);
            node = next;
        }
    }
    cRiderAnimBase_setupAnimation(self, animMinParts);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_expireAnimation);
#ifdef SKIP_ASM
extern "C" int tEventFlags_isSet1(void* self, int mask);

struct sVEntry00312490 {
    short delta;
    short index;
    void (*fn)(void*, cAnimSequencer*, cAnimSequenceNode*);
};

struct sSeqRef_00312490 {
    cAnimSequenceNode* node;
    cAnimSequencer* seq;
};

/**
 * Expires every sequence that has reached its end trigger.
 *
 * Collects up to 32 (sequence, sequencer) pairs whose event 0x3F is set (tEventFlags_isSet1 on the sequence's +0xB0 flags),
 * then calls vtable slot 3 (vtable at +0x58) on each. The release build drops On Tour's bound check on the 32-entry
 * list.
 */
extern "C" void cRiderAnimBase_expireAnimation(cRiderAnimBase* self)
{
    sSeqRef_00312490 list[32];
    int n = 0;
    for (int i = 0; i < 6; i++)
    {
        cAnimSequenceNode* node = cAnimSequencer_getSequence(&(self->Sequencer)[i], 0);
        while (node)
        {
            if (tEventFlags_isSet1((char*)node + 0xB0, 0x3F) != 0)
            {
                list[n].node = node;
                list[n].seq = &(self->Sequencer)[i];
                n++;
            }
            node = *(cAnimSequenceNode**)((char*)node + 0xC8);
        }
    }
    for (int k = 0; k < n; k++)
    {
        sVEntry00312490* vt = (sVEntry00312490*)self->mVtbl;
        vt[3].fn(((char*)self) + vt[3].delta, list[k].seq, list[k].node);
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_setupAnimation);
#ifdef SKIP_ASM
struct sAnimState_00312598
{
    char data[0x50];
    sAnimState_00312598() {}
};

extern "C" int cAnimSequencer_getAnimBlendFrames(void* seq, sAnimState_00312598* out, int max);
extern "C" void func_0030F2B0(void* model, int count, sAnimState_00312598* states, int a3);

/**
 * Blends the rider's model pose from the frames of all playing sequences.
 *
 * Fills a 64-entry frame array from each of the 6 sequencers (cAnimSequencer_getAnimBlendFrames, getAnimBlendFrames) and hands it to
 * func_0030F2B0 (On Tour cAnimModel::blend) on the model at +0x54.
 *
 * @param riderAnim  the rider anim (this)
 * @param animMinParts  On Tour animMinParts: blend only the minimum parts
 */
extern "C" void cRiderAnimBase_setupAnimation(cRiderAnimBase* riderAnim, int animMinParts)
{
    int n = 0;
    sAnimState_00312598 states[0x40];
    for (int i = 0; i < 6; i++)
    {
        n += cAnimSequencer_getAnimBlendFrames((char*)riderAnim->Sequencer + (i << 3), &states[n], 0x40 - n);
    }
    func_0030F2B0(riderAnim->AnimModel, n, states, animMinParts);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getAIAnimFrame);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_onInput(cRiderAnimBase* self, void* seq, int anim, int a3, int a4, int a5, float blend);
extern "C" void cAnimSequence_setSubAnimTime(void* node, int i, float t);
extern char* cGameAnimMap_TheGameAnimMap;
extern int kAnimIDInvalid;

struct sVEntry00312660 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// The 0x50-byte state cAnimSequencer_getAnimBlendFrames fills (sAnimState_00312598), 16-byte aligned: it is returned by value (lq/sq copy).
struct sAnimState_00312660
{
    char data[0x50];
    sAnimState_00312660() {}
} __attribute__((aligned(16)));

/**
 * Samples one frame of an AI animation on a scratch sequencer and returns its first blend state.
 *
 * Starts the anim on a local cAnimSequencer (cRiderAnimBase_onInput, no blend), sets sequence 0's track 0 time
 * (cAnimSequence_setSubAnimTime), gathers up to 64 states (cAnimSequencer_getAnimBlendFrames) and expires the scratch sequencer.
 *
 * Despite their names, @p startFrameTime is the clip and @p animid the time: On Tour's order is (anid, float
 * startFrameTime, tAnimID animid), and the int and the float travel in different registers.
 *
 * @param anim            the AI anim to sample (On Tour tAIAnimID anid)
 * @param startFrameTime  the clip to play (On Tour tAnimID animid); kAnimIDInvalid (-1) means the AI anim's own
 *                        clip, found from its index (vtable slot 4) in the table at cGameAnimMap_TheGameAnimMap + 0x1030
 * @param animid          the time to sample, given to cAnimSequence_setSubAnimTime (On Tour startFrameTime)
 * @return the first 0x50-byte blend state (On Tour tAnimBlendFrame)
 */
// Samples anim at time t on a scratch sequencer and returns the first resulting state.
extern "C" sAnimState_00312660 cRiderAnimBase_getAIAnimFrame(cRiderAnimBase* self, int anim, int animid, float startFrameTime)
{
    sAnimState_00312660 states[0x40];
    cAnimSequencer seq;
    cAnimSequencer_init(&seq);
    if (animid == kAnimIDInvalid)
    {
        sVEntry00312660* vt = (sVEntry00312660*)self->mVtbl;
        animid = *(int*)(vt[4].fn(((char*)self) + vt[4].delta, anim) * 4 + cGameAnimMap_TheGameAnimMap + 0x1030);
    }
    cRiderAnimBase_onInput(self, &seq, anim, animid, 1, 0, 0.0f);
    cAnimSequence_setSubAnimTime(cAnimSequencer_getSequence(&seq, 0), 0, startFrameTime);
    cAnimSequencer_getAnimBlendFrames(&seq, (sAnimState_00312598*)states, 0x40);
    cAnimSequencer_expireAll(&seq);
    return states[0];
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getAIAnimDuration);
#ifdef SKIP_ASM
struct sVEntry00312790 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
extern char* cGameAnimMap_TheGameAnimMap;
extern char** cAnimDataBase_TheAnimDataBase;

/**
 * Returns how long the clip behind an AI animation lasts, in seconds.
 *
 * Vtable slot 4 (vtable at +0x58) gives the game anim for @p aid; 0x207 (none) returns 0. Otherwise the clip id comes
 * from cGameAnimMap_TheGameAnimMap (+0x1030 table) and its length is (frame count - 1) / 30 from
 * cAnimDataBase_TheAnimDataBase.
 *
 * @param aid  the AI animation (On Tour aid)
 * @return the clip length in seconds, 0 when the anim has no clip
 */
extern "C" float cRiderAnimBase_getAIAnimDuration(cRiderAnimBase* self, int aid)
{
    sVEntry00312790* vt = (sVEntry00312790*)self->mVtbl;
    int id = vt[4].fn(((char*)self) + vt[4].delta, aid);
    if (id == 519)
        return 0.0f;
    unsigned int animId = *(unsigned int*)(id * 4 + cGameAnimMap_TheGameAnimMap + 0x1030);
    char* clip = *(char**)((char*)cAnimDataBase_TheAnimDataBase + ((animId & 0xFF) << 2));
    char* clipEntry = (char*)((animId >> 8) * 0x14 + *(int*)(clip + 0x8));
    return (*(unsigned short*)(clipEntry + 0xC) - 1) * 0.03333333507180214f;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getAIAnimEventTime);
#ifdef SKIP_ASM
struct sVEntry00312820 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sAnimKey00312820 {
    unsigned short value;
    unsigned short pad;
};

/**
 * Returns the time in seconds of tag number @p frame in the clip that AI anim @p anim plays, or -1 if it has fewer.
 *
 * Maps the AI anim to a game animation with the virtual at vtable slot 4 (0 when that gives 519, no game anim), then
 * through the map (+0x1030) of cGameAnimMap_TheGameAnimMap to a bank and clip of cAnimDataBase_TheAnimDataBase. The
 * clip's 0x14-byte record holds the first tag (u16 at +0x10) and tag count (u16 at +0x12); tag frames are u16 values
 * in the bank's table at +0x10, converted at 30 frames per second. ai/computer uses tags 1 and 2 as trick markers.
 *
 * @param anim  the AI anim id
 * @param frame  the tag number in the clip
 * @return the tag's time in seconds, -1 when the clip has no such tag, 0 when the anim has no game animation
 */
extern "C" float cRiderAnimBase_getAIAnimEventTime(cRiderAnimBase* self, int anim, int frame)
{
    sVEntry00312820* vt = (sVEntry00312820*)self->mVtbl;
    int id = vt[4].fn(((char*)self) + vt[4].delta, anim);
    if (id == 519)
        return 0.0f;
    unsigned int mapEntry = *(unsigned int*)(id * 4 + cGameAnimMap_TheGameAnimMap + 0x1030);
    char* clip = *(char**)((char*)cAnimDataBase_TheAnimDataBase + ((mapEntry & 0xFF) << 2));
    char* animRec = (char*)((mapEntry >> 8) * 0x14 + *(int*)(clip + 0x8));
    float frameTime;
    if (frame < *(unsigned short*)(animRec + 0x12))
    {
        sAnimKey00312820* keys = *(sAnimKey00312820**)(clip + 0x10);
        frameTime = keys[*(unsigned short*)(animRec + 0x10) + frame].value * 0.03333333507180214f;
    }
    else
    {
        frameTime = -1.0f;
    }
    return frameTime;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_play);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getAnimPlayAttr(int anim);
extern "C" int cRiderAnimBase_onInput(cRiderAnimBase* self, void* seq, int anim, int a3, int a4, int a5, float blend);
extern int kSequencerPriority[];
extern char* cGameAnimMap_TheGameAnimMap;

struct sVEntry003128E8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares cRiderAnimBase_play as void for its callers, but it returns the new
// sequence id (callers in ai/ai read $v0); the int-returning body is bound by asm label.
int cRiderAnimBase_play_impl(char* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");

/**
 * Starts an AI animation on its sequencer slot and records it as that slot's current anim.
 *
 * Maps @p anim through vtable slot 4; the result 0x207 means it has no animation. A negative @p blend takes
 * the anim's default blend-in time from its event-map record (cAIAnimEventMap_getAnimPlayAttr()[4]), whose [3]
 * also gives the sequencer slot; cRiderAnimBase_onInput starts the clip there with the slot's constant from kSequencerPriority
 * (3, 2, 1, 0, 0, 0).
 *
 * @param anim   the AI anim to play (On Tour tAIAnimID anid)
 * @param flags  nonzero allows a duplicate sequence: a matching playing sequence then does not hand over its
 *               tracks (On Tour isDupSeqAllowed)
 * @param blend  blend-in time in seconds; negative for the anim's default
 * @return the started AI anim (also stored in CurrentAnimID[slot]), or 0x1B6 when it has no animation
 */
int cRiderAnimBase_play_impl(char* self, int anim, int flags, float blend)
{
    sVEntry003128E8* vt = *(sVEntry003128E8**)(self + 0x58);
    int id = vt[4].fn(self + vt[4].delta, anim);
    if (id == 519)
        return 438;
    if (blend < 0.0f)
    {
        blend = *(float*)&cAIAnimEventMap_getAnimPlayAttr(anim)[4];
    }
    int animId = *(int*)(id * 4 + cGameAnimMap_TheGameAnimMap + 0x1030);
    int slot = cAIAnimEventMap_getAnimPlayAttr(anim)[3];
    return *(int*)(self + (slot << 2)) = cRiderAnimBase_onInput((cRiderAnimBase*)self, *(char**)(self + 0x50) + (slot << 3), anim, animId, flags, kSequencerPriority[slot], blend);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_playFromAnimID);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getAnimPlayAttr(int anim);
extern "C" int cRiderAnimBase_onInput(cRiderAnimBase* self, void* seq, int anim, int a3, int a4, int a5, float blend);
extern int kSequencerPriority[];

/**
 * Plays a specific animation clip under an AI animation id, on that animation's sequencer.
 *
 * The sequencer is attribute word 3; a negative @p blend takes attribute word 4. Calls cRiderAnimBase_onInput with
 * duplicates allowed and the sequencer's priority from kSequencerPriority, and stores the result as the sequencer's current AI
 * anim (+slot * 4). cAIAnimEventMap_getAnimPlayAttr returns the anim's whole attribute record (On Tour release:
 * cAIAnimEventMap::getAnimPlayAttr): word 0 is the category, 1 the update rule, 3 the sequencer and 4 the default
 * blend-in time (On Tour debug: getCategory, getUpdateRule, getSequencerID, getBlendInTime).
 *
 * @param anim  the AI animation (On Tour anid)
 * @param animid  the clip to play: bank in the low byte, clip above it (On Tour tAnimID animid)
 * @param blend  the blend-in time in seconds; negative uses the anim's default (On Tour blendIn)
 */
extern "C" void cRiderAnimBase_playFromAnimID(cRiderAnimBase* self, int anim, int animid, float blend)
{
    int slot = cAIAnimEventMap_getAnimPlayAttr(anim)[3];
    if (blend < 0.0f)
    {
        blend = *(float*)&cAIAnimEventMap_getAnimPlayAttr(anim)[4];
    }
    *(int*)(((char*)self) + (slot << 2)) = cRiderAnimBase_onInput(self, (char*)self->Sequencer + (slot << 3), anim, animid, 1, kSequencerPriority[slot], blend);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getCurrentAnimID__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the AI animation a rider sequencer is currently playing.
 *
 * The current AI anim array starts at +0x0, one int per sequencer; 0x1B6 means none.
 *
 * @param i  the sequencer (On Tour tAIAnimSequencerID sid)
 * @return the current AI anim id
 */
int cRiderAnimBase_getCurrentAnimID(void* vself, int i)
{
    cRiderAnimBase* self = (cRiderAnimBase*)vself;
    return *(int*)((char*)self + (i << 2));
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_getCurrentAnimFramePercent);
#ifdef SKIP_ASM
/**
 * Returns how far the newest sequence of sequencer @p i has played: its first sub-animation's time over its length.
 *
 * Takes sequence 0 (cAnimSequencer_getSequence) of Sequencer[@p i] and divides its time (+0x8) by its total time
 * (+0x10). There is no check for an empty sequencer.
 *
 * @param i  the sequencer slot, 0-5
 * @return the fraction played, 0-1
 */
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(cRiderAnimBase* self, int i)
{
    cAnimSequencer* seqs = self->Sequencer;
    cAnimSequenceNode* node = cAnimSequencer_getSequence(&seqs[i], 0);
    return *(float*)((char*)node + 0x8) / *(float*)((char*)node + 0x10);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_isCurrentAnimEndFlag);
#ifdef SKIP_ASM
/**
 * Tells whether the animation a rider sequencer is playing has reached its end.
 *
 * Reads +0xC0 of cAnimSequencer_getSequence(&sequencers[@p i], 0) without a null check (the release build drops On
 * Tour's assert).
 *
 * @param i  the sequencer (On Tour tAIAnimSequencerID sid)
 * @return nonzero once the end event has fired
 */
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(cRiderAnimBase* self, int i)
{
    cAnimSequencer* seqs = self->Sequencer;
    return *(int*)((char*)cAnimSequencer_getSequence(&seqs[i], 0) + 0xC0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_playInto);
#ifdef SKIP_ASM
struct cAnimSequence;
void cAnimSequence_fadeWeight(cAnimSequence* self, float targetWeight, float time);
extern "C" void cAnimSequencer_expire(void* seq, void* node);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Replaces sequence @p node with AI anim @p anim, carrying the old sequence's blend weight and fade over.
 *
 * Saves the node's CurrentWeight (+0x94), TargetWeight (+0x98) and WeightTimer (+0x9C, the fade time left), expires it
 * (cAnimSequencer_expire), plays @p anim with cRiderAnimBase_play(anim, 0, -1 = default blend), then gives the new
 * first sequence of @p seq the old current weight as both current and target weight and, if a fade was running,
 * restarts it with cAnimSequence_fadeWeight(old target, old time left).
 *
 * @param seq  the sequencer the sequence plays on
 * @param node  the sequence to replace
 * @param anim  the AI anim to play
 * @return @p anim
 */
extern "C" int cRiderAnimBase_playInto(cRiderAnimBase* self, cAnimSequencer* seq, char* node, int anim)
{
    float currentWeight = *(float*)(node + 0x94);
    float timeLeft = *(float*)(node + 0x9C);
    float targetWeight = *(float*)(node + 0x98);
    cAnimSequencer_expire(seq, node);
    cRiderAnimBase_play(self, anim, 0, -1.0f);
    cAnimSequence* sequence = (cAnimSequence*)((char*)cAnimSequencer_getSequence(seq, 0));
    sequence->TargetWeight = currentWeight;
    sequence->CurrentWeight = currentWeight;
    if (timeLeft != 0.0f)
    {
        cAnimSequence_fadeWeight((cAnimSequence*)sequence, targetWeight, timeLeft);
    }
    return anim;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_playBlendIn);
#ifdef SKIP_ASM
extern "C" void tEventFlags_clear1(void* self, int bit);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Clears event bit 63 of sequence @p sequence's event flags (+0xB0) and plays AI anim @p anim with the default blend-in.
 *
 * The bit is cleared with tEventFlags_clear1 (the same clear-and-mark-changed operation as tEventFlags_clear); the anim starts
 * with cRiderAnimBase_play(anim, 0, -1). Unlike cRiderAnimBase_playInto the old sequence is left to blend out.
 *
 * @param sequencer  the sequencer the sequence plays on (unused)
 * @param sequence  the sequence (cAnimSequence) that ended
 * @param anim  the AI anim to play
 */
extern "C" void cRiderAnimBase_playBlendIn(cRiderAnimBase* self, void* sequencer, void* sequence, int anim)
{
    tEventFlags_clear1((char*)sequence + 0xB0, 0x3F);
    cRiderAnimBase_play(self, anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", dcmpGetFrame);
#ifdef SKIP_ASM
// Packed animation channel table (built by dcmpInitFrameAccessor, sampled here).
struct sRabChannel {
    int type;      // 0x0
    float v4;      // 0x4
    float v8;      // 0x8
    float vC;      // 0xC (key index for types 6..9)
    float v10;     // 0x10
};

struct sRabChannelSet {
    sRabChannel* channels;   // 0x0
    unsigned char* data4;    // 0x4: type 6 keys, u8, one row per frame
    unsigned char* data8;    // 0x8: type 7 keys, u8, one row per 2 frames
    unsigned char* dataC;    // 0xC: type 8 keys, u16
    unsigned char* data10;   // 0x10: type 9 keys, float
    unsigned char count6;    // 0x14
    unsigned char count7;    // 0x15
    unsigned char count8;    // 0x16
    unsigned char count9;    // 0x17
};

/**
 * Decodes @p count compressed animation channels, starting at channel @p first, at frame index @p frameIndex into @p out.
 *
 * Each 0x14-byte channel has a type and four constants (v4, v8, vC, v10): 0 gives 0, 1 the constant v4, 2-4 a
 * polynomial in @p frameIndex of degree 1-3, 5 a line t * v4 + v8 that continues with slope vC after t = v10. Types 6-9 read a
 * key from per-frame tables (vC is the key's column): 6 u8 per frame, 7 u8 every second frame, 8 u16 per frame (all
 * three scaled by v8 plus v4), 9 a raw float per frame. Keys are interpolated linearly to the next row by the fraction
 * of @p frameIndex (the half-frame fraction for type 7); fractions below 0.001 count as 0, and with both 0 no interpolation is
 * done. The loop tests for the end after each channel, so @p count must be at least 1.
 *
 * @param first  the first channel to decode
 * @param count  the number of channels
 * @param frameIndex  the frame index (time x 30, may be fractional)
 * @param out  receives @p count floats
 */
extern "C" void dcmpGetFrame(sRabChannelSet* self, int first, int count, float frameIndex, float* out)
{
    float half = frameIndex * 0.5f;
    int it = (int)frameIndex;
    int ih = (int)half;
    float frac = frameIndex - (float)it;
    float fracHalf = half - (float)ih;
    sRabChannel* ch = self->channels + first;
    sRabChannel* end = ch + count;
    unsigned char* row4 = self->data4 + it * self->count6;
    unsigned char* row8 = self->data8 + ih * self->count7;
    unsigned short* rowC = (unsigned short*)self->dataC + it * self->count8;
    float* row10 = (float*)self->data10 + it * self->count9;
    if (frac < 0.001f) {
        frac = 0.0f;
    }
    if (fracHalf < 0.001f) {
        fracHalf = 0.0f;
    }
    if (frac != 0.0f || fracHalf != 0.0f) {
        unsigned char* next4 = row4 + self->count6;
        unsigned char* next8 = row8 + self->count7;
        unsigned short* nextC = rowC + self->count8;
        float* next10 = row10 + self->count9;
        for (;;) {
            switch (ch->type) {
            case 0:
                *out++ = 0.0f;
                break;
            case 1:
                *out++ = ch->v4;
                break;
            case 2:
                *out++ = frameIndex * ch->v4 + ch->v8;
                break;
            case 3:
                *out++ = frameIndex * (frameIndex * ch->v4 + ch->v8) + ch->vC;
                break;
            case 4:
                *out++ = frameIndex * (frameIndex * (frameIndex * ch->v4 + ch->v8) + ch->vC) + ch->v10;
                break;
            case 5: {
                float split = ch->v10;
                float a = ch->v4;
                float intercept = ch->v8;
                float tailSlope = ch->vC;
                if (frameIndex <= split) {
                    *out++ = frameIndex * a + intercept;
                } else {
                    *out++ = a * split + intercept + tailSlope * (frameIndex - split);
                }
                break;
            }
            case 6: {
                int k = *(int*)&ch->vC;
                float a = row4[k];
                *out++ = (a + frac * ((float)next4[k] - a)) * ch->v8 + ch->v4;
                break;
            }
            case 7: {
                int k = *(int*)&ch->vC;
                float a = row8[k];
                *out++ = (a + fracHalf * ((float)next8[k] - a)) * ch->v8 + ch->v4;
                break;
            }
            case 8: {
                int k = *(int*)&ch->vC;
                float a = rowC[k];
                *out++ = (a + frac * ((float)nextC[k] - a)) * ch->v8 + ch->v4;
                break;
            }
            case 9: {
                int k = *(int*)&ch->vC;
                float a = row10[k];
                *out++ = a + frac * (next10[k] - a);
                break;
            }
            }
            ch++;
            if (ch == end) return;
        }
    } else {
        for (;;) {
            switch (ch->type) {
            case 0:
                *out++ = 0.0f;
                break;
            case 1:
                *out++ = ch->v4;
                break;
            case 2:
                *out++ = frameIndex * ch->v4 + ch->v8;
                break;
            case 3:
                *out++ = frameIndex * (frameIndex * ch->v4 + ch->v8) + ch->vC;
                break;
            case 4:
                *out++ = frameIndex * (frameIndex * (frameIndex * ch->v4 + ch->v8) + ch->vC) + ch->v10;
                break;
            case 5: {
                float split = ch->v10;
                float a = ch->v4;
                float intercept = ch->v8;
                float tailSlope = ch->vC;
                if (frameIndex <= split) {
                    *out++ = frameIndex * a + intercept;
                } else {
                    *out++ = a * split + intercept + tailSlope * (frameIndex - split);
                }
                break;
            }
            case 6:
                *out++ = (float)row4[*(int*)&ch->vC] * ch->v8 + ch->v4;
                break;
            case 7:
                *out++ = (float)row8[*(int*)&ch->vC] * ch->v8 + ch->v4;
                break;
            case 8:
                *out++ = (float)rowC[*(int*)&ch->vC] * ch->v8 + ch->v4;
                break;
            case 9:
                *out++ = row10[*(int*)&ch->vC];
                break;
            }
            ch++;
            if (ch == end) break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", dcmpInitFrameAccessor);
#ifdef SKIP_ASM
// PORT: reinterprets packed 24-bit float bits
static inline float bitsToFloat_003130A8(int x)
{
    union { int i; float f; } u;
    u.i = x;
    return u.f;
}

#define READ24_003130A8(p, o) bitsToFloat_003130A8(((p)[o] << 8) | ((p)[(o) + 1] << 16) | ((p)[(o) + 2] << 24))

static inline void setType_003130A8(sRabChannelSet* self, sRabChannel* ch, int type)
{
    if (type == 6) {
        *(int*)&ch->vC = self->count6++;
    } else if (type == 7) {
        *(int*)&ch->vC = self->count7++;
    } else if (type == 8) {
        *(int*)&ch->vC = self->count8++;
    } else if (type == 9) {
        *(int*)&ch->vC = self->count9++;
    }
    ch->type = type;
}

/**
 * Sets up a compressed-animation frame accessor from a packed channel buffer.
 *
 * Reads the channel count (first byte; 0 leaves the accessor untouched), one 4-bit type per channel (high nibble
 * first) and each channel's constants as 24-bit floats (the top three bytes; type 5 adds a 16-bit integer).
 * Channels of types 6-9 get an index within their type, and the four key streams that follow are located: type 6
 * (a byte per frame), type 7 (half rate), type 8 (16-bit, padded to an even offset) and type 9 (floats, after
 * further padding).
 *
 * @param data    the packed channel buffer (On Tour: chanBuf)
 * @param frames  number of frames in the animation (On Tour: numFrames)
 */
extern "C" void dcmpInitFrameAccessor(sRabChannelSet* self, unsigned char* data, int frames)
{
    unsigned char* p = data;
    int n = *p;
    p++;
    if (n == 0) {
        return;
    }
    sRabChannel* ch = self->channels;
    self->count6 = 0;
    self->count7 = 0;
    self->count8 = 0;
    self->count9 = 0;
    sRabChannel* end = ch + n;
    while (1) {
        setType_003130A8(self, ch, *p >> 4);
        if (++ch == end) break;
        setType_003130A8(self, ch, *p & 0xF);
        if (++ch == end) break;
        p++;
    }
    p++;
    ch = self->channels;
    do {
        switch (ch->type) {
        case 1:
            ch->v4 = READ24_003130A8(p, 0);
            p += 3;
            break;
        case 3:
            ch->v4 = READ24_003130A8(p, 0);
            ch->v8 = READ24_003130A8(p, 3);
            ch->vC = READ24_003130A8(p, 6);
            p += 9;
            break;
        case 4:
            ch->v4 = READ24_003130A8(p, 0);
            ch->v8 = READ24_003130A8(p, 3);
            ch->vC = READ24_003130A8(p, 6);
            ch->v10 = READ24_003130A8(p, 9);
            p += 12;
            break;
        case 5:
            ch->v4 = READ24_003130A8(p, 0);
            ch->v8 = READ24_003130A8(p, 3);
            ch->vC = READ24_003130A8(p, 6);
            ch->v10 = (float)(p[9] | (p[10] << 8));
            p += 11;
            break;
        case 2:
        case 6:
        case 7:
        case 8:
            ch->v4 = READ24_003130A8(p, 0);
            ch->v8 = READ24_003130A8(p, 3);
            p += 6;
            break;
        }
        ch++;
    } while (ch != end);
    int sizeQB = frames * self->count6;
    int sizeQB2 = ((frames - 2) / 2 + 2) * self->count7;
    int sizeShort = frames * self->count8 * 2;
    int ofs = (p - data) + sizeQB + sizeQB2;
    self->data4 = p;
    self->data8 = p + sizeQB;
    int alignPad = ofs & 1;
    self->dataC = p + sizeQB + sizeQB2 + alignPad;
    self->data10 = p + sizeQB + sizeQB2 + sizeShort + ((ofs + sizeShort + alignPad) & 3);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_cAnimSequence);
#ifdef SKIP_ASM
struct sAnimSlot_00313508
{
    char pad0[0x14];
    int active;      // 0x14
    char pad18[0x4];
    sAnimSlot_00313508() {}
};

struct sRiderAnim_00313508
{
    int owner;                      // 0x0
    sAnimSlot_00313508 slots[3];    // 0x4
    char pad58[0x8];
    sRabQuad32 pair;                // 0x60
    int u80;                        // 0x80
    int u84;                        // 0x84
    long u88;                       // 0x88
    float f90;                      // 0x90
    float value;                    // 0x94
    float target;                   // 0x98
    float remaining;                // 0x9C
    int notify;                     // 0xA0
    int uA4;                        // 0xA4
    int uA8;                        // 0xA8
    void* head;                     // 0xAC
    ulong active;                   // 0xB0
    ulong triggered;                // 0xB8
    int dirty;                      // 0xC0
    int uC4;                        // 0xC4
    sRiderAnim_00313508(int owner);
};

inline void* operator new(unsigned int, void* p) { return p; }

// PORT: long/ulong are 64-bit here
inline sRiderAnim_00313508::sRiderAnim_00313508(int o)
{
    owner = o;
    for (int i = 2; i >= 0; i--)
    {
        slots[i].active = 0;
    }
    pair = tPose_Identity;
    u88 = -1;
    u80 = 0;
    u84 = 0;
    f90 = 1.0f;
    remaining = 0.0f;
    value = 1.0f;
    target = 1.0f;
    notify = 0;
    uA4 = 0;
    uA8 = 0;
    triggered = 0;
    active = 0;
    head = 0;
    dirty = 0;
    uC4 = 0;
}

/**
 * Constructs an animation sequence with id @p owner (stored at +0x0) and default state; returns @p self.
 *
 * The three sub-animations are marked inactive, the orientation offset (+0x60) becomes tPose_Identity, the bone mask
 * (+0x88) -1, flip and priority (+0x80, +0x84) 0, time scale (+0x90), current and target weight (+0x94, +0x98) 1, and
 * the fade timer, fade-out-expire flag, fade times, trigger list (+0xAC), event flags (+0xB0, +0xB8) and the flags at
 * +0xC0 and +0xC4 are cleared. The local struct names value/target/remaining/notify for +0x94 .. +0xA0.
 *
 * @param owner  the sequence id (the AI anim id for rider sequences)
 * @return @p self
 */
extern "C" sRiderAnim_00313508* cAnimSequence_cAnimSequence(cAnimSequence* self, int owner)
{
    return new (((sRiderAnim_00313508*)self)) sRiderAnim_00313508(owner);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_updateSubAnim);
#ifdef SKIP_ASM
struct sAnimEvents_00313868;
struct sAnimEvents_00313938;
extern "C" void cAnimSequence_checkTriggers(cAnimSequence* self, float from, float to);
extern "C" void cAnimSequence_checkTriggersAtTime(cAnimSequence* self, float time);
extern "C" void cAnimSequence_setEndTriggers(cAnimSequence* self);

struct sTrack_003135B0 {
    int field_0x0;
    float time;     // 0x4
    float speed;    // 0x8
    float length;   // 0xC
    char pad10[0x8];
    int loop;       // 0x18
};

// PORT: ulong is 64-bit here
struct sRab_003135B0 {
    int field_0x0;
    sTrack_003135B0 tracks[3];  // 0x4
    char pad58[0x90 - 0x58];
    float scale;                // 0x90
    char pad94[0xB8 - 0x94];
    ulong triggered;            // 0xB8
    int fC0;                    // 0xC0
    int fC4;                    // 0xC4
};

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).

static inline float ffloor_003135B0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline void fire_003135B0(sRab_003135B0* self, float from, float to)
{
    cAnimSequence_checkTriggers((cAnimSequence*)self, from, to);
}

/**
 * Advances sub-animation @p i of the sequence by @p dt, holding or wrapping at its ends and firing its triggers.
 *
 * The step is dt x the slot's time scale (+0x8 of the slot) x the sequence's time scale (+0x90). For slot 0 only: the
 * just-changed event bits (+0xB8) and the end flag (+0xC0) are cleared first, and if the +0xC4 flag is set (by
 * cAnimSequence_setSubAnimTime) the triggers at the old time fire (cAnimSequence_checkTriggersAtTime) and the flag is cleared. A hold slot (type 0)
 * clamps to [0, length] and at either end sets the end triggers (cAnimSequence_setEndTriggers); a loop slot outside the range is
 * wrapped by whole lengths, the end triggers are set, and the timed triggers over the wrapped span fire
 * (cAnimSequence_checkTriggers, split at the wrap point). Triggers fire only for slot 0. Stores the new time in the slot.
 *
 * @param i  the sub-animation slot, 0-2
 * @param dt  the time step
 * @return always 0
 */
extern "C" int cAnimSequence_updateSubAnim(sRab_003135B0* self, int i, float dt)
{
    float startTime = self->tracks[i].time;
    float len = self->tracks[i].length;
    float newTime = startTime + self->tracks[i].speed * self->scale * dt;
    if (i == 0) {
        self->triggered = 0;
        self->fC0 = 0;
        if (self->fC4 != 0) {
            cAnimSequence_checkTriggersAtTime((cAnimSequence*)self, startTime);
            self->fC4 = 0;
        }
    }
    if (self->tracks[i].loop == 0) {
        if (newTime <= 0.0f) {
            newTime = 0.0f;
            if (i == 0) {
                cAnimSequence_setEndTriggers((cAnimSequence*)self);
            }
        } else if (len <= newTime) {
            newTime = len;
            if (i == 0) {
                cAnimSequence_setEndTriggers((cAnimSequence*)self);
            }
        }
        if (i == 0) {
            fire_003135B0(self, startTime, newTime);
        }
    } else if (newTime < 0.0f || len <= newTime) {
        int n = (int)ffloor_003135B0(newTime / len);
        newTime -= len * (float)n;
        if (i == 0) {
            cAnimSequence_setEndTriggers((cAnimSequence*)self);
            if (n >= 2) {
                fire_003135B0(self, 0.0f, len);
            } else if (n < -1) {
                fire_003135B0(self, len, 0.0f);
            } else if (n == 1) {
                if (startTime <= newTime) {
                    fire_003135B0(self, 0.0f, len);
                } else {
                    fire_003135B0(self, startTime, len);
                    fire_003135B0(self, 0.0f, newTime);
                }
            } else if (n == -1) {
                if (newTime <= startTime) {
                    fire_003135B0(self, len, 0.0f);
                } else {
                    fire_003135B0(self, startTime, 0.0f);
                    fire_003135B0(self, len, newTime);
                }
            }
        }
    } else if (i == 0) {
        fire_003135B0(self, startTime, newTime);
    }
    self->tracks[i].time = newTime;
    return 0;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_updateWeight);
#ifdef SKIP_ASM

/**
 * Steps the sequence's weight fade by @p dt; returns 1 when the fade ends on a sequence marked to expire.
 *
 * While the fade time left (+0x9C) is nonzero the current weight (+0x94) moves linearly toward the target (+0x98):
 * by dt / time left of the remaining difference. When @p dt reaches the time left the weight snaps to the target, the
 * timer is cleared, and the result is 1 if the fade-out-expire flag (+0xA0, set by cAnimSequence_fadeOutAndExpire) is
 * set.
 *
 * @param dt  the time step
 * @return 1 when a fade-out-and-expire just finished, else 0
 */
// Moves value toward target over the remaining time; returns 1 when the blend
// finishes and notify is set.
extern "C" int cAnimSequence_updateWeight(cAnimSequence* self, float dt)
{
    float timeLeft = self->WeightTimer;
    if (timeLeft != 0.0f)
    {
        if (timeLeft <= dt)
        {
            self->CurrentWeight = self->TargetWeight;
            self->WeightTimer = 0.0f;
            if (self->IsFadeOutExpire != 0)
            {
                return 1;
            }
        }
        else
        {
            float current = self->CurrentWeight;
            self->WeightTimer = timeLeft - dt;
            self->CurrentWeight = current + (dt / timeLeft) * (self->TargetWeight - current);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_checkTriggers);
#ifdef SKIP_ASM
struct sAnimEvent_00313868
{
    int bit;                        // 0x0
    int kind;                       // 0x4
    float time;                     // 0x8
    sAnimEvent_00313868* next;      // 0xC
};


/**
 * Fires every timed trigger whose time lies in (@p from, @p to], or in [@p to, @p from) when playing backwards.
 *
 * Walks the trigger list (+0xAC) and skips end triggers. Firing sets the trigger's id bit in the event flags (+0xB0)
 * and, if it was clear, also in the just-changed flags (+0xB8). Does nothing when @p from equals @p to.
 *
 * @param from  the time before the step
 * @param to  the time after the step
 */
// Fires every kind-0 event whose time lies in (from, to] (or [to, from) when
// playing backwards).
// PORT: ulong is 64-bit here
extern "C" void cAnimSequence_checkTriggers(cAnimSequence* self, float from, float to)
{
    if (from == to)
    {
        return;
    }
    int forward = from <= to;
    for (sAnimEvent_00313868* event = ((sAnimEvent_00313868*)self->Triggers); event != 0; event = event->next)
    {
        if (event->kind == 0)
        {
            float eventTime = event->time;
            if (forward)
            {
                if (!(from < eventTime && eventTime <= to))
                    continue;
            }
            else
            {
                if (!(to <= eventTime && eventTime < from))
                    continue;
            }
            ulong bit = (ulong)1 << event->bit;
            ulong old = self->Events.IsSet;
            self->Events.IsSet = old | bit;
            self->Events.IsJustChanged |= bit & ~old;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_checkTriggersAtTime);
#ifdef SKIP_ASM
struct sAnimEvent_00313938
{
    int bit;                        // 0x0
    int kind;                       // 0x4
    float time;                     // 0x8
    sAnimEvent_00313938* next;      // 0xC
};


/**
 * Fires every timed trigger whose time equals @p time exactly (used after the playback time is set directly).
 *
 * Sets each match's id bit in the event flags (+0xB0) and, if newly set, in the just-changed flags (+0xB8).
 *
 * @param time  the time to check
 */
// PORT: ulong is 64-bit here
extern "C" void cAnimSequence_checkTriggersAtTime(cAnimSequence* self, float time)
{
    for (sAnimEvent_00313938* event = ((sAnimEvent_00313938*)self->Triggers); event != 0; event = event->next)
    {
        if (event->kind == 0 && event->time == time)
        {
            ulong bit = (ulong)1 << event->bit;
            ulong old = self->Events.IsSet;
            self->Events.IsSet = old | bit;
            self->Events.IsJustChanged |= bit & ~old;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", cAnimSequence_setEndTriggers);
#ifdef SKIP_ASM
/**
 * Marks the sequence ended (+0xC0 = 1) and fires every end trigger of its list (+0xAC).
 *
 * Each end trigger's id bit is set in the event flags (+0xB0) and, if newly set, in the just-changed flags (+0xB8).
 */
// PORT: ulong is 64-bit here
extern "C" void cAnimSequence_setEndTriggers(cAnimSequence* self)
{
    self->EndEvent = 1;
    for (sAnimEvent_00313938* event = ((sAnimEvent_00313938*)self->Triggers); event != 0; event = event->next)
    {
        if (event->kind != 0)
        {
            ulong bit = (ulong)1 << event->bit;
            ulong old = self->Events.IsSet;
            self->Events.IsSet = old | bit;
            self->Events.IsJustChanged |= bit & ~old;
        }
    }
}
#endif

