#include "vtable.h"
#include "common.h"
#include "bx/appman.h"
#include "object/flexbridgenode.h"
#include "object/objnode.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("object/spring", cSpring_setupNodes);
#ifdef SKIP_ASM
struct sSpVec_53EF0 {
    float x, y, z, w;
    sSpVec_53EF0() {}
    sSpVec_53EF0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sSpNode_53EF0 {
    sSpVec_53EF0 p;         // 0x00
    sSpVec_53EF0 v;         // 0x10
    float a;                // 0x20
    float av;               // 0x24
    int pad28[2];           // 0x28
    sSpVec_53EF0 f;         // 0x30
    float t;                // 0x40
    int pad44[3];           // 0x44
};


// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_SpringNodes[];
extern sSpVec_53EF0 t4vect_Zero_53EF0 __asm__("t4vect_Zero");
/**
 * Allocates the spring's chain of nodes ("SpringNodes") with zero position, velocity, force, angle and
 * torque.
 */
extern "C" void cSpring_setupNodes(cSpring* self)
{
    int i;
    sSpNode_53EF0** dst = &(*(sSpNode_53EF0**)&self->Nodes);
    *dst = new (str_SpringNodes, 0x20000000, 0) sSpNode_53EF0[self->Width];
    sSpNode_53EF0* nd = ((sSpNode_53EF0*)self->Nodes);
    for (i = 0; i < self->Width; i++, nd++) {
        nd->p = t4vect_Zero_53EF0;
        nd->v = t4vect_Zero_53EF0;
        nd->f = t4vect_Zero_53EF0;
        nd->av = 0;
        nd->a = 0;
        nd->t = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/spring", cSpring_update);
#ifdef SKIP_ASM
struct sSpVec_53FC0 {
    float x, y, z, w;
    sSpVec_53FC0() {}
    sSpVec_53FC0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));



extern sSpVec_53FC0 t4vect_Zero_sp __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sSpVec_53FC0 spSub_53FC0(const sSpVec_53FC0& a, const sSpVec_53FC0& b)
{
    sSpVec_53FC0 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sSpVec_53FC0 spAdd_53FC0(const sSpVec_53FC0& a, const sSpVec_53FC0& b)
{
    sSpVec_53FC0 r;
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void spAddEq_53FC0(sSpVec_53FC0& a, const sSpVec_53FC0& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sSpVec_53FC0 spScale_53FC0(const sSpVec_53FC0& v, float s)
{
    sSpVec_53FC0 r;
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

// PORT: PS2-only VU0 inline asm (scalar times vector).
static inline sSpVec_53FC0 spScaleF_53FC0(float s, const sSpVec_53FC0& v)
{
    sSpVec_53FC0 r;
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void spScaleEq_53FC0(sSpVec_53FC0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sSpVec_53FC0 spDiv_53FC0(const sSpVec_53FC0& v, float s)
{
    sSpVec_53FC0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float spLength_53FC0(const sSpVec_53FC0& v)
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

/**
 * Steps the spring chain by one app tick: stretch and twist springs between neighbours, damping, gravity and
 * applied forces.
 *
 * The end nodes stay fixed. Velocities are solved semi-implicitly with each node's neighbours for stability;
 * gravity is -980 on z. The applied forces and torques are cleared afterwards.
 */
extern "C" void cSpring_update(cSpring* self)
{
    sSpVec_53FC0 gravity(0.0f, 0.0f, -980.0f, 0.0f);
    float dt = 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    sSpVec_53FC0 forces[16];
    float torques[16];
    forces[0] = forces[self->Width - 1] = t4vect_Zero_sp;
    torques[0] = torques[self->Width - 1] = 0.0f;
    for (int i = 1; i < self->Width - 1; i++) {
        sSpVec_53FC0 toPrev = spSub_53FC0((*(sSpVec_53FC0*)&self->Nodes[i - 1].P), (*(sSpVec_53FC0*)&self->Nodes[i].P));
        sSpVec_53FC0 toNext = spSub_53FC0((*(sSpVec_53FC0*)&self->Nodes[i + 1].P), (*(sSpVec_53FC0*)&self->Nodes[i].P));
        spScaleEq_53FC0(toPrev, 1.0f - self->Length * self->SegLen / spLength_53FC0(toPrev));
        spScaleEq_53FC0(toNext, 1.0f - self->Length * self->SegLen / spLength_53FC0(toNext));
        forces[i] = spScaleF_53FC0(self->LSpring, spAdd_53FC0(toPrev, toNext));
        spAddEq_53FC0(forces[i], spScaleF_53FC0(dt * self->LSpring, spSub_53FC0(spAdd_53FC0((*(sSpVec_53FC0*)&self->Nodes[i - 1].V), (*(sSpVec_53FC0*)&self->Nodes[i + 1].V)), spScale_53FC0((*(sSpVec_53FC0*)&self->Nodes[i].V), 2.0f))));
        // PORT: pointer arithmetic in int (only form that gives the target's index-first addu and
        // signed -0x30 offsets; g++ 2.95 turns nd[-1] into an unsigned 0xFFFFFFB0 offset).
        tSpringNode* nd = (tSpringNode*)(i * 0x50 + (int)self->Nodes);
        torques[i] = self->ASpring * (((tSpringNode*)((int)nd - 0x50))->O - nd->O + nd[1].O - nd->O);
        torques[i] += dt * self->ASpring * (((tSpringNode*)((int)nd - 0x50))->W + nd[1].W - nd->W * 2.0f);
        spAddEq_53FC0(forces[i], spAdd_53FC0(spScale_53FC0((*(sSpVec_53FC0*)&nd->V), -self->LDamp), gravity));
        torques[i] += -self->ADamp * self->Nodes[i].W;
    }
    for (int i = 1; i < self->Width - 1; i++) {
        spAddEq_53FC0(forces[i], (*(sSpVec_53FC0*)&self->Nodes[i].F));
        torques[i] += self->Nodes[i].T;
    }
    for (int i = 1; i < self->Width - 1; i++) {
        sSpVec_53FC0 deltaVel = spAdd_53FC0(spScale_53FC0(forces[i], dt), spDiv_53FC0(spScaleF_53FC0(dt * dt * dt * self->LSpring, spAdd_53FC0(forces[i - 1], forces[i + 1])), dt * dt * 2.0f * self->LSpring + 1.0f));
        deltaVel = spDiv_53FC0(deltaVel, dt * dt * 2.0f * self->LSpring + 1.0f);
        spAddEq_53FC0((*(sSpVec_53FC0*)&self->Nodes[i].V), deltaVel);
        spAddEq_53FC0((*(sSpVec_53FC0*)&self->Nodes[i].P), spScale_53FC0((*(sSpVec_53FC0*)&self->Nodes[i].V), dt));
        self->Nodes[i].W += (dt * torques[i] + dt * dt * dt * self->ASpring * (torques[i - 1] + torques[i + 1]) / (dt * dt * 2.0f * self->ASpring + 1.0f)) / (dt * dt * 2.0f * self->ASpring + 1.0f);
        self->Nodes[i].O += dt * self->Nodes[i].W;
    }
    for (int i = 0; i < self->Width; i++) {
        (*(sSpVec_53FC0*)&self->Nodes[i].F) = t4vect_Zero_sp;
        self->Nodes[i].T = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/spring", cSpring_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Saves a spring into a replay frame: its parameters, then its nodes.
 *
 * Writes the first 0x24 bytes of the spring (count and parameters, On Tour cSpringState) and then the node array
 * (Nodes +0x24, count * 0x50 bytes) through the frame's virtual write (slot 1). cSpring_cSpring1 reads them back.
 *
 * @param stream  the replay frame to write to (On Tour data)
 */
extern "C" void cSpring_writeToReplayFrame(cSpring* self, cBxReplayFrame* stream)
{
    stream->write((const char*)self, 36);
    stream->write((const char*)self->Nodes, *(int*)self * 80);
}
#endif

//100%
INCLUDE_ASM("object/spring", cObjNode_cObjNode1);
#ifdef SKIP_ASM
extern "C" void cBucketMan_add(void* mgr, void* node, void* param);
extern void* cObjNode_vtbl[16];
extern char gBucketMan;


/**
 * Constructs an object node and adds it to a render bucket.
 *
 * Stores cObjNode's vtable and calls cBucketMan_add(&gBucketMan, self, param2), which appends the node to one
 * of that bucket's two lists depending on the bucket's flags.
 *
 * @param bucket  bucket index (callers pass 3, 4, 6 and others)
 * @return @p self
 */
extern "C" cObjNode* cObjNode_cObjNode1(cObjNode* self, void* bucket)
{
    (*(void**)&VPTR_OF(cObjNode, self)) = cObjNode_vtbl;
    cBucketMan_add(&gBucketMan, self, bucket);
    return self;
}
#endif

