#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "bx/math3d.h"
#include "ai/rider.h"
#include "object/instancenode.h"
#include "world/instancerecord.h"
#include "ai/dangerman.h"
#include "render/particle.h"
#include "sound/ssxaudio.h"
#include "visualfx/avalanche.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "replay/bxreplayframe.h"
#include "main/gameviewman.h"
#include "camera/camera.h"

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_getFrameData);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_5778(i + 1 < (int)self->data->length)`, `do { } while (0)`) in the emit loop, perhaps EA's compiled-out assert but unconfirmed; plain form 98.57%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sV4_5778 {
    float x, y, z, w;
    sV4_5778() {}
    sV4_5778(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    sV4_5778(const struct sV3_5778& v);
} __attribute__((aligned(16)));

struct sV3_5778 {
    float x, y, z;
    sV3_5778() {}
    sV3_5778(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

inline sV4_5778::sV4_5778(const sV3_5778& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMatrix_5778(void* dst, const void* src)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void MulMatrix_5778(void* d, const void* a, const void* b)
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

struct sMat_5778 {
    sV4_5778 r0, r1, r2, r3;
    sMat_5778() {}
    sMat_5778(const sMat_5778& m) { CopyMatrix_5778(this, &m); }
    sMat_5778& operator=(const sMat_5778& m) { CopyMatrix_5778(this, &m); return *this; }
};

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_5778 vAdd_5778(const sV4_5778& a, const sV4_5778& b)
{
    sV4_5778 r;
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

// PORT: PS2-only VU0 inline asm (vector add in place).
static inline void vAddEq_5778(sV4_5778& a, const sV4_5778& b)
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
static inline sV4_5778 vScale_5778(const sV4_5778& v, float s)
{
    sV4_5778 r;
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

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_5778(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline sV3_5778 Normalize_5778(const sV3_5778& v)
{
    float len = Sqrt_5778(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return sV3_5778(v.x * inv, v.y * inv, v.z * inv);
    }
    return sV3_5778(v.x, v.y, v.z);
}

// Avalanche path data: 10 bytes per frame (30 frames per second):
// signed velocity[3], unsigned colour[3], signed rotation axis[3], unsigned rotation angle.
enum {
    kAvFrameSize_5778 = 10,
    kAvVel_5778 = 0,
    kAvCol_5778 = 3,
    kAvAxis_5778 = 6,
    kAvRot_5778 = 9
};

struct sAvData_5778 {
    char pad0[0xE0];
    sV4_5778 pos;               // 0xE0
    unsigned short type;        // 0xF0
    unsigned char emits;        // 0xF2
    char padF3[0xFC - 0xF3];
    char* frames;               // 0xFC
    float length;               // 0x100, in frames
    float f104;                 // 0x104
    float f108;                 // 0x108
    float rate;                 // 0x10C
};

struct sAvAct_5778 {
    int frame;                  // 0x0
    int pad4[3];
    sV4_5778 pos;               // 0x10
    sMat_5778 mat;              // 0x20
    sV4_5778 curPos;            // 0x60
    sMat_5778 curMat;           // 0x70
    float fB0;                  // 0xB0
    float fB4;                  // 0xB4
    char padB8[0xC0 - 0xB8];
    sV4_5778 curColour;         // 0xC0
    char emitter[0x12C];        // 0xD0
    float f1FC;                 // 0x1FC
    char pad200[0x2E0 - 0x200];
    sAvData_5778* data;         // 0x2E0
    sAvAct_5778* next;          // 0x2E4
    int f2E8;                   // 0x2E8
    int emitting;               // 0x2EC
};

struct tFrameData_5778 {
    sV4_5778 pos;               // 0x0
    sV4_5778 colour;            // 0x10
    sMat_5778 mat;              // 0x20
};

extern float cAvalanche_m_UpdateTimeStep;
extern float cAvalanche_m_PatchColourScale2;
extern sV4_5778 t4vect_Zero_5778 __asm__("t4vect_Zero");
extern sV4_5778 t4vect_Origin_5778 __asm__("t4vect_Origin");
extern sMat_5778 t4mat_Identity_5778 __asm__("t4mat_Identity");
extern "C" void tActiveAvalancheNode_calculateScale_Data(void* self, float t) __asm__("tActiveAvalancheNode_calculateScale");
// PORT: sine/cosine; the angle is set up first at both calls here (other units declare it (s, c, angle),
// which uses the same registers), so it is declared angle-first under its own name.
extern "C" void fSinCos_5778(float angle, float* s, float* c) __asm__("fSinCos");
extern "C" void cDynamicColourEmitter_update_Data(float dt, void* emitter, sV4_5778* pos, sV4_5778* vel, sV4_5778* col, int on) __asm__("cDynamicColourEmitter_update");

// Rotates obj->mat by ANGLE about AXIS, keeping its translation (used on the active node and on the
// returned frame data, which both keep their matrix in `mat`).
template <class T>
static inline void RotateFrame_5778(T* obj, const sV4_5778& axis, float angle)
{
    sMat_5778& m = obj->mat;
    sMat_5778 saved = m;
    sV4_5778 trans = saved.r3;
    saved.r3 = t4vect_Origin_5778;
    float s;
    float c;
    fSinCos_5778(angle, &s, &c);
    float t = 1.0f - c;
    float tx = t * axis.x;
    float ty = t * axis.y;
    float tz = t * axis.z;
    float sx = s * axis.x;
    float sy = s * axis.y;
    float sz = s * axis.z;
    obj->mat.r3.x = 0.0f;
    obj->mat.r3.y = 0.0f;
    obj->mat.r3.z = 0.0f;
    obj->mat.r3.w = 1.0f;
    obj->mat.r3 = trans;
    obj->mat.r0.w = 0.0f;
    obj->mat.r1.w = 0.0f;
    obj->mat.r2.w = 0.0f;
    obj->mat.r0.x = tx * axis.x + c;
    obj->mat.r1.x = tx * axis.y + sz;
    obj->mat.r2.x = tx * axis.z - sy;
    obj->mat.r0.y = ty * axis.x - sz;
    obj->mat.r1.y = ty * axis.y + c;
    obj->mat.r2.y = ty * axis.z + sx;
    obj->mat.r0.z = tz * axis.x + sy;
    obj->mat.r1.z = tz * axis.y - sx;
    obj->mat.r2.z = tz * axis.z + c;
    MulMatrix_5778(&m, &m, &saved);
}

// Compiled-out (release) assert.
#define ASSERT_5778(cond) do { } while (0)

static inline float Min_5778(float a, float b)
{
    return a < b ? a : b;
}

static inline void Reset_5778(sAvAct_5778* self)
{
    self->frame = 0;
    self->pos = self->data->pos;
    self->mat = t4mat_Identity_5778;
}

/**
 * Steps an active avalanche node to the frame at time t and returns its interpolated state.
 *
 * Frames run at 30 per second. Moving to a new frame steps the node's position (packed deltas x 2) and
 * rotation (axis plus an angle byte for 0-120 degrees) one frame at a time, restarting from the start for
 * frame 0 or going back; while m_updateEmitters is set, each step also emits debris at m_UpdateTimeStep x
 * speed intervals, and the flag is cleared afterwards. Updates the scales first (calculateScale).
 *
 * @param currTime  time since the avalanche started, in seconds
 * @return the frame data: position, colour (packed bytes / 128, alpha = m_emitterAlphaScale) and rotation at
 *         t
 */
extern "C" tFrameData_5778 tActiveAvalancheNode_getFrameData(sAvAct_5778* self, float currTime)
{
    tActiveAvalancheNode_calculateScale_Data(self, currTime);
    float x = currTime * 30.0f;
    int frame = (int)x;
    float frac = x - (float)frame;
    if (frame >= (int)self->data->length - 1) {
        frac = 1.0f;
        frame = (int)self->data->length - 2;
    }
    float ifrac = 1.0f - frac;
    if (frame != self->frame) {
        if (frame == 0) {
            Reset_5778(self);
        } else {
            if (frame < self->frame) {
                Reset_5778(self);
            }
            float time = 0.0f;
            while (self->frame != frame) {
                if (self->emitting) {
                    while ((int)(time * 30.0f) == self->frame) {
                        float ft = time * 30.0f;
                        int i = (int)ft;
                        float blend = ft - (float)i;
                        if (i >= (int)self->data->length - 1) {
                            blend = 1.0f;
                            i = (int)self->data->length - 2;
                        }
                        float invBlend = 1.0f - blend;
                        time = Min_5778(time + cAvalanche_m_UpdateTimeStep * self->data->rate, self->data->length * (1.0f / 30.0f));
                        if (!(ft < self->data->length)) {
                            break;
                        }
                        ASSERT_5778(i + 1 < (int)self->data->length);
                        sV4_5778 pos = vAdd_5778(sV4_5778(self->data->frames[i * kAvFrameSize_5778 + kAvVel_5778] * blend * 2.0f,
                                                          self->data->frames[i * kAvFrameSize_5778 + kAvVel_5778 + 1] * blend * 2.0f,
                                                          self->data->frames[i * kAvFrameSize_5778 + kAvVel_5778 + 2] * blend * 2.0f, 0.0f),
                                                 self->pos);
                        sV4_5778 col = vScale_5778(sV4_5778(((unsigned char)self->data->frames[i * kAvFrameSize_5778 + kAvCol_5778] * invBlend +
                                                             (unsigned char)self->data->frames[(i + 1) * kAvFrameSize_5778 + kAvCol_5778] * blend) * (1.0f / 128.0f),
                                                            ((unsigned char)self->data->frames[i * kAvFrameSize_5778 + kAvCol_5778 + 1] * invBlend +
                                                             (unsigned char)self->data->frames[(i + 1) * kAvFrameSize_5778 + kAvCol_5778 + 1] * blend) * (1.0f / 128.0f),
                                                            ((unsigned char)self->data->frames[i * kAvFrameSize_5778 + kAvCol_5778 + 2] * invBlend +
                                                             (unsigned char)self->data->frames[(i + 1) * kAvFrameSize_5778 + kAvCol_5778 + 2] * blend) * (1.0f / 128.0f),
                                                            self->fB4),
                                                   cAvalanche_m_PatchColourScale2);
                        self->f1FC = self->fB4;
                        cDynamicColourEmitter_update_Data(1.0f / 60.0f, self->emitter, &pos, &t4vect_Zero_5778, &col, ft < self->data->length);
                    }
                }
                vAddEq_5778(self->pos, sV4_5778(self->data->frames[self->frame * kAvFrameSize_5778 + kAvVel_5778] * 2.0f,
                                                self->data->frames[self->frame * kAvFrameSize_5778 + kAvVel_5778 + 1] * 2.0f,
                                                self->data->frames[self->frame * kAvFrameSize_5778 + kAvVel_5778 + 2] * 2.0f, 0.0f));
                sV4_5778 axis(Normalize_5778(sV3_5778(self->data->frames[self->frame * kAvFrameSize_5778 + kAvAxis_5778] * (1.0f / 127.0f),
                                                      self->data->frames[self->frame * kAvFrameSize_5778 + kAvAxis_5778 + 1] * (1.0f / 127.0f),
                                                      self->data->frames[self->frame * kAvFrameSize_5778 + kAvAxis_5778 + 2] * (1.0f / 127.0f))));
                // The angle byte spans 0..120 degrees (2.0943951 rad) per frame.
                RotateFrame_5778(self, axis, (unsigned char)self->data->frames[self->frame * kAvFrameSize_5778 + kAvRot_5778] * (2.0943951f * (1.0f / 255.0f)));
                self->frame++;
            }
            self->emitting = 0;
        }
    }
    tFrameData_5778 fd;
    fd.pos = vAdd_5778(sV4_5778(self->data->frames[frame * kAvFrameSize_5778 + kAvVel_5778] * frac * 2.0f,
                                self->data->frames[frame * kAvFrameSize_5778 + kAvVel_5778 + 1] * frac * 2.0f,
                                self->data->frames[frame * kAvFrameSize_5778 + kAvVel_5778 + 2] * frac * 2.0f, 0.0f),
                       self->pos);
    fd.colour = vScale_5778(sV4_5778(((unsigned char)self->data->frames[frame * kAvFrameSize_5778 + kAvCol_5778] * ifrac +
                                      (unsigned char)self->data->frames[(frame + 1) * kAvFrameSize_5778 + kAvCol_5778] * frac) * (1.0f / 128.0f),
                                     ((unsigned char)self->data->frames[frame * kAvFrameSize_5778 + kAvCol_5778 + 1] * ifrac +
                                      (unsigned char)self->data->frames[(frame + 1) * kAvFrameSize_5778 + kAvCol_5778 + 1] * frac) * (1.0f / 128.0f),
                                     ((unsigned char)self->data->frames[frame * kAvFrameSize_5778 + kAvCol_5778 + 2] * ifrac +
                                      (unsigned char)self->data->frames[(frame + 1) * kAvFrameSize_5778 + kAvCol_5778 + 2] * frac) * (1.0f / 128.0f),
                                     self->fB4),
                            cAvalanche_m_PatchColourScale2);
    fd.mat = self->mat;
    sV4_5778 axis(Normalize_5778(sV3_5778(self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778] * (1.0f / 127.0f),
                                          self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778 + 1] * (1.0f / 127.0f),
                                          self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778 + 2] * (1.0f / 127.0f))));
    RotateFrame_5778(&fd, axis, frac * ((unsigned char)self->data->frames[frame * kAvFrameSize_5778 + kAvRot_5778] * 2.0943951f) * (1.0f / 255.0f));
    return fd;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_getColour);
#ifdef SKIP_ASM
// t4Vector with EA's inline constructors (On Tour t4Vector() and t4Vector(float, float, float, float)); the functions
// below in this unit use it too.
struct t4VectorCtor : public t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};
extern float cAvalanche_m_PatchColourScale2;
extern t4VectorCtor t4vect_Origin;
extern tAvalancheNode* cAvalanche_m_nodeList[];

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorCtor vScale_6290(const t4VectorCtor& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

/**
 * Returns the colour of the avalanche node bound to an instance, alpha first.
 *
 * The node is cAvalanche_m_nodeList[high byte of the instance's m_sRenderFlags]. When the node has an active node
 * (m_activeNode) it returns that active node's m_colour reordered as (w, x, y, z). Otherwise, when it has a packed
 * history (m_history), it returns (1, r, g, b) from the first frame's colour bytes divided by 128 (then scaled by
 * cAvalanche_m_PatchColourScale2, 1.0). With neither it returns t4vect_Origin. Nothing in the retail code references
 * it.
 *
 * @param inst  the instance record bound to the node
 * @return the colour as (alpha, red, green, blue)
 */
extern "C" t4VectorCtor cAvalanche_getColour(tInstanceRecord* inst)
{
    tAvalancheNode* node = cAvalanche_m_nodeList[inst->m_sRenderFlags >> 8];
    tActiveAvalancheNode* activeNode = node->m_activeNode;
    if (activeNode != 0) {
        return t4VectorCtor(activeNode->m_colour.w, activeNode->m_colour.x, activeNode->m_colour.y,
                             activeNode->m_colour.z);
    }
    tAvalanchePackedFrameData* history = node->m_history;
    if (history != 0) {
        return vScale_6290(t4VectorCtor(1.0f, (float)history->m_r * 0.0078125f, (float)history->m_g * 0.0078125f,
                                         (float)history->m_b * 0.0078125f),
                           cAvalanche_m_PatchColourScale2);
    }
    return t4vect_Origin;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche__cAvalanche);
#ifdef SKIP_ASM
extern void* cAvalanche_vtbl[];
extern void* cVisualEffect_vtbl[];
struct cAvalanche__cAvalanche_sElem {
    char pad[0x2F0];
};
extern cAvalanche__cAvalanche_sElem D_004EE840[];
extern "C" void cAvalanche_reset(cAvalanche* self);
extern "C" void cDynamicColourEmitter_purge(void* p, int flags);
void operator_delete(int* ptr);

/**
 * Destroys the avalanche effect: resets it, purges its 64 dynamic colour emitters (D_004EE840), restores the base
 * vtable (cVisualEffect_vtbl) and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAvalanche__cAvalanche(cAvalanche* self, int flags)
{
    *(void***)self = cAvalanche_vtbl;
    ((cAvalanche*)self)->reset();
    int i;
    for (i = 0; i < 64; i++) {
        cDynamicColourEmitter_purge(&D_004EE840[i], 1);
    }
    *(void***)self = cVisualEffect_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_init);
#ifdef SKIP_ASM
struct cAvalanche_init_sElem {
    float x;
    float y;
    float z;
    float w;
    int r;
    int g;
    int b;
    int a;
    char pad[0x10];
};
extern cAvalanche_init_sElem vertlist[];
extern cAvalanche_init_sElem collisionvertlist[];
extern tAvalancheNode* cAvalanche_m_nodeList[];
extern int cAvalanche_m_numNodes;
extern "C" void memset(void* dst, int c, int n);

/**
 * Initialises the avalanche's static vertex tables and clears its state.
 *
 * Sets the 0x800 entries of vertlist to alternating black and white (alpha 0xFF, z 1.0, x / y / w 0) and the 0x20
 * entries of collisionvertlist to yellow, clears cAvalanche_m_numNodes and zeroes 0x400 bytes at cAvalanche_m_nodeList. On Tour's init is a
 * member (this); this C takes no object.
 */
extern "C" void cAvalanche_init(void)
{
    int i;
    for (i = 0; i < 0x800; i++) {
        int shade = (i & 1) ? 0xFF : 0;
        cAvalanche_init_sElem* elem = &vertlist[i];
        elem->a = 0xFF;
        elem->r = shade;
        elem->g = shade;
        elem->b = shade;
        elem->z = 1.0f;
        elem->w = 0.0f;
        elem->x = 0.0f;
        elem->y = 0.0f;
    }
    for (i = 0; i < 0x20; i++) {
        cAvalanche_init_sElem* elem = &collisionvertlist[i];
        elem->a = 0xFF;
        elem->r = 0xFF;
        elem->g = 0xFF;
        elem->b = 0;
        elem->z = 1.0f;
        elem->w = 0.0f;
        elem->x = 0.0f;
        elem->y = 0.0f;
    }
    cAvalanche_m_numNodes = 0;
    memset(cAvalanche_m_nodeList, 0, 0x400);
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_fixVelocity);
#ifdef SKIP_ASM
extern float cAvalanche_m_MaxVelocity;

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_64D8(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4VectorCtor vNorm_64D8(const t4Vector& v)
{
    t4VectorCtor r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorCtor vScale_64D8(const t4Vector& v, float s)
{
    t4VectorCtor r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

static inline int isBad_64D8(const float* f)
{
    unsigned int b = *(const unsigned int*)f;
    int r = 0;
    if (b == 0x7F800000 || b == 0x7FC00000 || b == 0xFF800000 || b == 0xFFC00000)
        r = 1;
    return r;
}

/**
 * Limits an avalanche node's velocity to 70 units and replaces a non-finite velocity with (10, 10, 10, 0).
 *
 * When the 4-component length of @p velocity exceeds cAvalanche_m_MaxVelocity (70), the vector is normalised and scaled to 70. Then, if any
 * component is +-infinity or a quiet NaN (bit patterns 0x7F800000, 0x7FC00000, 0xFF800000, 0xFFC00000), the whole
 * vector is reset. tAvalancheNode_calculate calls it.
 *
 * @param velocity  the velocity, changed in place
 */
void tAvalancheNode::fixVelocity(t4Vector* velocity)
{
    if (cAvalanche_m_MaxVelocity < vLen_64D8(*velocity))
        *velocity = vScale_64D8(vNorm_64D8(*velocity), cAvalanche_m_MaxVelocity);
    int bad = isBad_64D8(&velocity->x) || isBad_64D8(&velocity->y) || isBad_64D8(&velocity->z) || isBad_64D8(&velocity->w);
    if (bad)
        *velocity = t4VectorCtor(10.0f, 10.0f, 10.0f, 0.0f);
}
#endif

INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_calculate);

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_calculateScale);
#ifdef SKIP_ASM

/**
 * Computes an active node's object scale (+0xB0) and emitter alpha (+0xB4) for a time.
 *
 * In frames (time x 30): during the fade-in the alpha ramps up with scale 1; in the middle both are 1; during
 * the fade-out both fall linearly to 0 and the scale becomes the square root of that, except for base type 2
 * nodes, which keep their last values.
 *
 * @param currTime  time since the avalanche started, in seconds
 */
void tActiveAvalancheNode::calculateScale(float currTime)
{
    tAvalancheNode* avData = m_node;
    float x = currTime * 30.0f;
    if (x < avData->m_fadein) {
        m_objectScale = 1.0f;
        m_emitterAlphaScale = x / avData->m_fadein;
        return;
    }
    if (x < avData->m_life - avData->m_fadeout) {
        m_emitterAlphaScale = 1.0f;
        m_objectScale = 1.0f;
        return;
    }
    if (avData->m_baseType != tAvalancheNode::kBase_Permanent) {
        float fadeOut = (avData->m_life - x) / avData->m_fadeout;
        m_emitterAlphaScale = fadeOut;
        m_objectScale = fadeOut;
        if (fadeOut > 0.0f) {
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(fadeOut) : "f"(fadeOut));
            m_objectScale = fadeOut;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_update);
#ifdef SKIP_ASM
// The frame tActiveAvalancheNode_getFrameData returns by value (its own view: tFrameData_5778).
struct sAvFrameCA8 {
    t4Vector pos;             // 0x0
    t4Vector colour;          // 0x10
    t4Matrix rot;             // 0x20
};
extern "C" sAvFrameCA8 getFrameData_CA8(tActiveAvalancheNode* self, float t)
    __asm__("tActiveAvalancheNode_getFrameData");

extern "C" void tActiveAvalancheNode_end(tActiveAvalancheNode* self, int force);
extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern t4Vector t4vect_Zero_7CA8 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixCA8(t4Matrix* dst, t4Matrix* src)
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
 * Advances one avalanche chunk to a given time.
 *
 * Returns 0 when the node has no data (m_node). The time @p currTime is multiplied by the data's m_speed and
 * tActiveAvalancheNode_getFrameData gives that frame's position, colour and rotation. While t * 30 is below the
 * data's frame count (m_life) they are copied into the node and, unless the base type is 2 or the data has no emitter,
 * the emitter's start colour alpha (m_emitter.m_emitter.Cr0.w) takes m_emitterAlphaScale and the emitter is updated
 * (cDynamicColourEmitter_update) for a 1/60 s step. Past the end, type-2 nodes stay alive; others are ended with
 * tActiveAvalancheNode_end(node, 0).
 *
 * @param currTime  time since the avalanche started, in seconds (On Tour currTime)
 * @return true while the node is still active, else false
 */
bool tActiveAvalancheNode::update(float currTime)
{
    if (m_node == 0) return false;
    currTime *= m_node->m_speed;
    sAvFrameCA8 fd = getFrameData_CA8(this, currTime);
    if (currTime * 30.0f < m_node->m_life) {
        t4Vector* pos = &m_position;
        m_position = fd.pos;
        vu0CopyMatrixCA8(&m_rotation, &fd.rot);
        m_colour = fd.colour;
        tAvalancheNode* nodeData = m_node;
        t4Vector* colour = &m_colour;
        if (nodeData->m_baseType != tAvalancheNode::kBase_Permanent && nodeData->m_hasEmitter != 0) {
            void* org = &t4vect_Zero_7CA8;
            void* em = &m_emitter;
            m_emitter.m_emitter.Cr0.w = m_emitterAlphaScale;
            int alive = currTime < nodeData->m_life;
            cDynamicColourEmitter_update(em, pos, org, colour, alive, 0.01666666753590107f);
        }
        return true;
    }
    if (m_node->m_baseType == tAvalancheNode::kBase_Permanent) return true;
    end(0);
    return false;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_end);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_reset(void* self);


/**
 * Releases an active node from its avalanche node.
 *
 * Only when it has a node and either force is set or the node's base type is not 2: calls virtual slot 1 with
 * 3 on the object at +0xC of the node's instance (unknown), clears the node's m_activeNode and this node's
 * m_node, and resets the emitter.
 *
 * @param force  end even base type 2 nodes
 */
void tActiveAvalancheNode::end(int force)
{
    tAvalancheNode* avData = m_node;
    if (((char*)avData) != 0 && (force != 0 || avData->m_baseType != tAvalancheNode::kBase_Permanent)) {
        tInstanceRecord* inst = avData->m_instance;
        if (((char*)inst) != 0) {
            cInstanceNode* obj = inst->m_pActiveNode;
            delete obj;
        }
        m_node->m_activeNode = 0;
        m_node = 0;
        cDynamicColourEmitter_reset(&m_emitter);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_buildArray);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_ActiveNodeArray[];

struct tActiveAvalanche_node {
    char pad[0x2E4];
    tActiveAvalanche_node* next; // 0x2E4
};


/**
 * Builds an array of pointers to the active avalanche's nodes, in list order.
 *
 * Counts the nodes linked from the list head, allocates that many pointers tagged "ActiveNodeArray", stores the
 * array in the avalanche and fills it.
 */
void tActiveAvalanche::buildArray()
{
    tActiveAvalanche_node* node;
    int n = 0;
    for (node = ((tActiveAvalanche_node*)m_activeNodeList); node != 0; node = node->next) {
        n++;
    }
    *(tActiveAvalanche_node***)&m_activeNodeArray = (tActiveAvalanche_node**)operator_new_tag(n * 4,
            str_ActiveNodeArray, 0, 0);
    int i = 0;
    for (node = ((tActiveAvalanche_node*)m_activeNodeList); node != 0; node = node->next) {
        m_activeNodeArray[i++] = (tActiveAvalancheNode*)node;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

struct sAvObj_7EF8 {
    char pad_0x00[0xC];
    void* spawned;          // 0xC
    char pad_0x10[0x30];
    t4Vector pos;         // 0x40
    char pad_0x50[0x10];
    t4Vector sndPos;      // 0x60
};

struct sAvEvent_7EF8 {
    float time;             // 0x0
    sAvObj_7EF8* obj;       // 0x4
};

struct sAvSound_7EF8 {
    unsigned short frame;   // 0x0
    unsigned short obj;     // 0x2
};

struct sAvData_7EF8 {
    char pad_0x00[0xC];
    unsigned short nEvents;     // 0xC
    unsigned short nSounds;     // 0xE
    sAvEvent_7EF8 events[32];   // 0x10
    sAvSound_7EF8 sounds[1];    // 0x110
};

struct sAvNode_7EF8 {
    char pad_0x000[0x2E4];
    sAvNode_7EF8* next;     // 0x2E4
};


struct sAvEnd_7EF8 {};  // empty trailing member of the defaults record; its value-init copies a byte through a stack temp

extern float cAvalanche_m_UpdateTimeStep;
extern int D_004A54C8;
// Defaults record of an animated-float spawn (read by cAnimObjectNode_cAnimObjectNode1 as a tMakeAnimObjectData,
// object/animobject.h).
// The original keeps it as a function-local static; the ctor's float parameters are what lets loop.c hoist -1.0f/30.0f.
struct sAvFloatDef_7EF8 {
    unsigned id;        // 0x00
    int f04;            // 0x04
    int dir;            // 0x08
    float min;          // 0x0C
    float max;          // 0x10
    float speed;        // 0x14
    float speedVar;     // 0x18
    float value;        // 0x1C
    int random;         // 0x20
    int f24;            // 0x24
    sAvEnd_7EF8 end;    // 0x28
    sAvFloatDef_7EF8(float aMin = -1.0f, float aMax = -1.0f, float aSpeed = 30.0f) : id(0xFFFFFFFF), f04(1), dir(0), min(aMin), max(aMax), speed(aSpeed), speedVar(0), value(-1.0f), random(0), f24(1), end() {}
};

// PORT: placement new; builds the defaults record in place in its retail storage.
inline void* operator new(unsigned int, void* p) { return p; }

extern sAvFloatDef_7EF8 D_004D5770;
extern char str_AnimObject_b[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cAnimObjectNode_cAnimObjectNode1(void* self, int a, int type, void* b, void* c);
extern "C" bool tActiveAvalancheNode_update_7EF8(void* node, float t) __asm__("tActiveAvalancheNode_update");
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void tActiveAvalanche_end_7EF8(void* self) __asm__("tActiveAvalanche_end");
void cSSXAudio_AvalancheGroundHit_7EF8(void* snd, const t4Vector& pos) __asm__("cSSXAudio_AvalancheGroundHit__FPv");

static inline unsigned short soundFrame_7EF8(const tActiveAvalanche* a)
{
    return ((sAvData_7EF8*)a->m_avalanche)->sounds[a->m_nextGroundCollision].frame;
}

static inline float toFrames_7EF8(float t)
{
    return t * 30.0f;
}

static inline t4Vector pos_7EF8(const sAvObj_7EF8* o)
{
    return o->pos;
}

/**
 * Runs one update step of an active avalanche.
 *
 * Adds cAvalanche_m_UpdateTimeStep to the clock and updates each node in the list (tActiveAvalancheNode_update), forgetting the
 * object of each node that finished. Events whose time has come spawn their object as a new 0x6C-byte "AnimObject" node
 * if it has none, and play a ground-hit sound at its position. Sound cues whose frame has been reached play at their
 * object's sound position. When no node is active any more the avalanche ends (tActiveAvalanche_end).
 */
void tActiveAvalanche::update()
{
    if (((sAvData_7EF8*)m_avalanche) == 0) {
        return;
    }
    bool alive = false;
    m_currTime += cAvalanche_m_UpdateTimeStep;
    int i = 0;
    for (sAvNode_7EF8* n = ((sAvNode_7EF8*)m_activeNodeList); n != 0; n = n->next, i++) {
        bool ok = tActiveAvalancheNode_update_7EF8(n, m_currTime);
        alive = ok | alive;
        if (!ok) {
            m_activeNodeArray[i] = 0;
        }
    }
    while (m_nextCollision < ((sAvData_7EF8*)m_avalanche)->nEvents && ((sAvData_7EF8*)m_avalanche)->events[m_nextCollision].time <= m_currTime) {
        if (((sAvData_7EF8*)m_avalanche)->events[m_nextCollision].obj->spawned == 0) {
            // NOTE: function-local static of the original (guard D_004A54C8), written out as its guarded in-place construction.
            if (D_004A54C8 == 0) {
                new (&D_004D5770) sAvFloatDef_7EF8();
                D_004A54C8 = 1;
            }
            cAnimObjectNode_cAnimObjectNode1(cMemMan_alloc(108, str_AnimObject_b, 0x20000000, 0), 1, 1,
                    ((sAvData_7EF8*)m_avalanche)->events[m_nextCollision].obj, &D_004D5770);
        }
        cSSXAudio_AvalancheGroundHit_7EF8(cAudioInterface_bxAudio(),
                pos_7EF8(((sAvData_7EF8*)m_avalanche)->events[m_nextCollision].obj));
        m_nextCollision++;
    }
    while (m_nextGroundCollision < ((sAvData_7EF8*)m_avalanche)->nSounds && (float)soundFrame_7EF8(this) <= toFrames_7EF8(m_currTime)) {
        sAvObj_7EF8* cueObj = ((sAvObj_7EF8*)m_activeNodeArray[((sAvData_7EF8*)m_avalanche)->sounds[m_nextGroundCollision].obj]);
        if (cueObj != 0) {
            cSSXAudio_AvalancheGroundHit_7EF8(cAudioInterface_bxAudio(), cueObj->sndPos);
        }
        m_nextGroundCollision++;
    }
    if (!alive) {
        tActiveAvalanche_end_7EF8(this);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_end);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_Avalanche(void* self, int a1);

/**
 * Stops an active avalanche and releases what it holds.
 *
 * Frees the array at +0x8. If the avalanche is running (+0x0), every node in the list from +0x4 is ended with
 * tActiveAvalancheNode_end(node, 1), the running flag is cleared, the audio interface's avalanche sound is switched off
 * (cSSXAudio_Avalanche with 0) and the counter at +0x708 of the AI's +0xA8 object is decremented if non-zero.
 */
void tActiveAvalanche::end()
{
    if (m_activeNodeArray != 0) {
        cMemMan_free(m_activeNodeArray);
        m_activeNodeArray = 0;
    }
    if (*(int*)this != 0) {
        tActiveAvalancheNode* n = m_activeNodeList;
        while (((char*)n) != 0) {
            n->end(1);
            n = (tActiveAvalancheNode*)n->m_next;
        }
        *(int*)this = 0;
        cSSXAudio_Avalanche(cAudioInterface_bxAudio(), 0);
        cDangerMan* dangerMan = SSXApp->Game->AI->DangerMan;
        if (dangerMan->mNumActiveAvalanche != 0) {
            dangerMan->mNumActiveAvalanche = dangerMan->mNumActiveAvalanche - 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_SaveData);
#ifdef SKIP_ASM
extern char str_xav[];
extern char str_sav[];
extern char str_gav[];
extern "C" void cAvalanche_SaveData1(int, char*);

/**
 * Saves the avalanche data through cAvalanche_SaveData1() three times, for the extensions "xav", "sav" and
 * "gav".
 *
 * The first argument is 1 for "xav" and "sav" and 0 for "gav".
 */
extern "C" void cAvalanche_SaveData(void)
{
    cAvalanche_SaveData1(1, str_xav);
    cAvalanche_SaveData1(1, str_sav);
    cAvalanche_SaveData1(0, str_gav);
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_writeData);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void FILESYS_writesync(void* a, void* b, void* data, int size, int prio);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_buffer[];

/**
 * Writes @p rows units of @p stride bytes to an open file, reversing the bytes of each unit unless @p direct is set.
 *
 * With @p direct it writes the data as it is; otherwise it copies it into a temporary buffer (operator new tagged
 * "buffer") with each unit's bytes reversed, writes that and frees it. Both paths use FILESYS_writesync with priority
 * 100. cAvalanche_SaveData1 calls it for every field of the avalanche data file, passing direct 1 for the byte-sized
 * packed history records.
 *
 * @param direct  nonzero to write the bytes unchanged; zero to byte-swap each unit
 * @param handle  the file handle
 * @param offset  the file offset to write at
 * @param src  the data
 * @param stride  bytes per unit
 * @param rows  number of units
 * @return the number of bytes written (@p stride * @p rows)
 */
extern "C" int cAvalanche_writeData(int direct, void* handle, void* offset, char* src, int stride, int rows)
{
    if (direct) {
        FILESYS_writesync(handle, offset, src, stride * rows, 100);
    } else {
        char* buf = (char*)operator_new_tag(stride * rows, str_buffer, 0, 0);
        char* dstCursor = buf;
        for (int i = 0; i < rows; i++) {
            char* end = src + stride;
            char* srcCursor = end;
            while (src != srcCursor) {
                *dstCursor++ = *--srcCursor;
            }
            src = end;
        }
        FILESYS_writesync(handle, offset, buf, stride * rows, 100);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    return stride * rows;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_SaveData1);
#ifdef SKIP_ASM
extern tAvalanche* cAvalanche_m_avalancheList;
extern int cAvalanche_m_Save;
extern char str_pct_s_pct_s_j[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" char* cBENewRaceInterface_getLocationFile(void* iface, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int FILESYS_opensync(char* name, int mode, int prio, int* fd);
extern "C" void FILESYS_closesync(int fd, int prio);
extern "C" int cAvalanche_writeData(int direct, void* a, void* b, char* src, int stride, int rows);


/**
 * Writes the avalanche data of the current location to "<location file>.<name>".
 *
 * File layout: magic 0x2BEEF00 and the data size, then per avalanche, for each node 0xBEEF, the record size,
 * the start position and life x 10-byte packed frames, followed by the collision and ground-collision counts
 * and records (a collision stores its time and its instance's m_RID). Clears the save request
 * cAvalanche_m_Save (On Tour m_Save).
 *
 * @param direct  byte-order flag for the writer cAvalanche_writeData (On Tour: littleEndian); the packed frames
 *                always pass 1
 * @param name  file extension
 */
// PORT: cAvalanche_writeData takes the file handle and offset as void* (ints passed through pointers).
extern "C" void cAvalanche_SaveData1(int direct, char* name)
{
    if (cAvalanche_m_avalancheList == 0) {
        return;
    }
    int off = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[0x100];
    sprintf(buf, str_pct_s_pct_s_j, cBENewRaceInterface_getLocationFile(iface, cBENewRaceInterface_getLocationID(iface)), name);
    int fd;
    if (FILESYS_opensync(buf, 38, 100, &fd)) {
        tAvalanche* avalanche;
        for (avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
            for (tAvalancheNode* node = (avalanche->m_nodeList); node != 0; node = node->m_next) {
                off += 0x10 + (int)node->m_life * 10;
            }
            off += 4;
            int k;
            for (k = 0; k < avalanche->m_numCollisions; k++) {
                off += 8;
            }
            for (k = 0; k < avalanche->m_numGroundCollisions; k++) {
                off += 4;
            }
        }
        int magic = cAvalanche::kFileMagic;
        cAvalanche_writeData(direct, (void*)fd, 0, (char*)&magic, 4, 1);
        cAvalanche_writeData(direct, (void*)fd, (void*)4, (char*)&off, 4, 1);
        off = 8;
        for (avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
            unsigned short header;
            for (tAvalancheNode* node = (avalanche->m_nodeList); node != 0; node = node->m_next) {
                header = 48879;
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
                header = (int)node->m_life * 10 + 0xC;
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)((float*)&node->m_start), 4, 3);
                off += cAvalanche_writeData(1, (void*)fd, (void*)off, ((char*)node->m_history), 10, (int)node->m_life);
            }
            header = avalanche->m_numCollisions;
            off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
            header = avalanche->m_numGroundCollisions;
            off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
            int k;
            for (k = 0; k < avalanche->m_numCollisions; k++) {
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off, (char*)&avalanche->m_collisions[k].m_time,
                                            4, 1);
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off,
                                            (char*)&avalanche->m_collisions[k].m_inst->m_RID, 4, 1);
            }
            for (k = 0; k < avalanche->m_numGroundCollisions; k++) {
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off,
                                            (char*)&avalanche->m_groundCollisions[k].m_nodeId, 2, 1);
                off += cAvalanche_writeData(direct, (void*)fd, (void*)off,
                                            (char*)&avalanche->m_groundCollisions[k].m_time, 2, 1);
            }
        }
        FILESYS_closesync(fd, 100);
    }
    cAvalanche_m_Save = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_LoadData);
#ifdef SKIP_ASM
extern void* cAvalanche_m_filedata;
extern int cAvalanche_m_fileDataDeleteable;
extern int cAvalanche_m_resolved;
extern int cAvalanche_m_badCookie;
extern int cAvalanche_m_Load;
extern char str_sav[];
// PORT: the read call returns syncblockio's result (bytes read); the ealib unit declares it void.
extern "C" int FILESYS_readsync_i(int fd, int off, void* data, int size, int prio) __asm__("FILESYS_readsync");
extern "C" void cAvalanche_end(void);
extern "C" void cAvalanche_resolveDataPointers(void);

/**
 * Loads the current location's avalanche data file ("<location file>.sav") and resolves it.
 *
 * On a good magic (0x2BEEF00) the whole file is read into a buffer of the stored data size, but size + 8
 * bytes are read into it (header included), so the read runs 8 bytes past the allocation. With a bad magic
 * the file is not closed. Clears cAvalanche_m_Load (On Tour m_Load) and resolves the data pointers.
 */
extern "C" void cAvalanche_LoadData(void)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[0x100];
    sprintf(buf, str_pct_s_pct_s_j, cBENewRaceInterface_getLocationFile(iface, cBENewRaceInterface_getLocationID(iface)), str_sav);
    cAvalanche_m_resolved = 0;
    cAvalanche_m_badCookie = 1;
    int fd;
    if (FILESYS_opensync(buf, 1, 100, &fd)) {
        cAvalanche_end();
        if (cAvalanche_m_filedata != 0 && cAvalanche_m_fileDataDeleteable != 0) {
            cMemMan_free(cAvalanche_m_filedata);
        }
        int* hdr = (int*)operator_new_tag(8, buf, 0, 0);
        FILESYS_readsync_i(fd, 0, hdr, 8, 100);
        if (hdr[0] != cAvalanche::kFileMagic) {
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            cAvalanche_m_filedata = 0;
            cAvalanche_m_fileDataDeleteable = 0;
        } else {
            int size = hdr[1];
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            void* data = operator_new_tag(size, buf, 0, 0);
            cAvalanche_m_fileDataDeleteable = 1;
            cAvalanche_m_filedata = data;
            FILESYS_readsync_i(fd, 0, data, size + 8, 100);
            FILESYS_closesync(fd, 100);
            cAvalanche_m_badCookie = 0;
        }
    }
    cAvalanche_m_Load = 0;
    cAvalanche_resolveDataPointers();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_update);
#ifdef SKIP_ASM
struct sAvalancheSlot {
    int active;
    char pad[0x18];
};
extern int cAvalanche_m_Enable;
extern int cAvalanche_m_Recalc;
extern int cAvalanche_m_Trigger;
extern int cAvalanche_m_Update;
extern int cAvalanche_m_AvalancheId;
extern int cAvalanche_m_resolved;
extern int cAvalanche_m_badCookie;
extern sAvalancheSlot cAvalanche_m_activeAvalanches[];
extern "C" void cAvalanche_calculate(void);
extern "C" void cAvalanche_resolveDataPointers(void);
extern "C" int cAvalanche_triggerAvalanche(int a);
extern "C" void tActiveAvalanche_update(void* self);

/**
 * Runs the avalanche system for one frame while it is enabled.
 *
 * When enabled (cAvalanche_m_Enable, On Tour m_Enable): recalculates everything when the recalc flag (cAvalanche_m_Recalc) is set
 * and clears it and cAvalanche_m_badCookie; resolves the data pointers; while the trigger flag (cAvalanche_m_Trigger) is set,
 * triggers avalanche cAvalanche_m_AvalancheId (On Tour m_AvalancheId; a negative flag fires once); once pointers are resolved
 * and the update flag (cAvalanche_m_Update) is set, updates all 16 active avalanches (tActiveAvalanche_update, On Tour
 * tActiveAvalanche::update), a negative update flag again counting as one-shot.
 */
extern "C" void cAvalanche_update(void)
{
    if (cAvalanche_m_Enable != 0) {
        if (cAvalanche_m_Recalc != 0) {
            cAvalanche_calculate();
            cAvalanche_m_Recalc = 0;
            cAvalanche_m_badCookie = 0;
        }
        cAvalanche_resolveDataPointers();
        if (cAvalanche_m_Trigger != 0) {
            cAvalanche_triggerAvalanche(cAvalanche_m_AvalancheId);
            if (cAvalanche_m_Trigger < 0) {
                cAvalanche_m_Trigger = 0;
            }
        }
        if (cAvalanche_m_Enable != 0 && cAvalanche_m_resolved != 0 && cAvalanche_m_Update != 0) {
            int i;
            for (i = 0; i < 16; i++) {
                tActiveAvalanche_update(&cAvalanche_m_activeAvalanches[i]);
            }
            if (cAvalanche_m_Update < 0) {
                cAvalanche_m_Update = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_renderLines);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// t3Vector with EA's inline constructors (On Tour t3Vector(), t3Vector(float, float, float) and its copy
// constructor; the copy constructor makes the by-value returns below go through memory, as in retail).
struct t3VectorCtor : public t3Vector {
    t3VectorCtor() {}
    t3VectorCtor(float ax, float ay, float az)
    {
        x = ax;
        y = ay;
        z = az;
    }
    t3VectorCtor(const t3VectorCtor& o)
    {
        x = o.x;
        y = o.y;
        z = o.z;
    }
};

static inline float Sqrt_8A00(float x)
{
    float r;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline t3VectorCtor Normalize_8A00(const t3VectorCtor& a)
{
    float len = Sqrt_8A00(a.x * a.x + a.y * a.y + a.z * a.z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return t3VectorCtor(a.x * inv, a.y * inv, a.z * inv);
    }
    return a;
}





struct sOut_8A00 {
    float x, y, z, w;
    int r;
    int g;
    int b;
    int a;
    t4Vector pos;
};




extern int cAvalanche_m_RenderLines;
extern sOut_8A00 vertlist_8A00[] __asm__("vertlist");
extern t4Matrix t4mat_Identity_8A00 __asm__("t4mat_Identity");
extern t4Vector t4vect_Origin_8A00 __asm__("t4vect_Origin");
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (matrix copy).
static inline void CopyMat_8A00(t4Matrix* d, const t4Matrix* s)
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

// PORT: PS2-only VU0 inline asm (vector add in place).
static inline void AddTo_8A00(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply: a = a * b).
static inline void MulMat_8A00(t4Matrix* a, const t4Matrix* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(a), "r"(b)
        : "memory");
}

// matrix *= matrix through VU0. Declared to return the matrix by value but there is no return statement (g++ 2.95
// accepts it); the ignored by-value result is the unwritten 64-byte call temp in the retail frame.
static inline t4Matrix operator*=(t4Matrix& a, const t4Matrix& b)
{
    MulMat_8A00(&a, &b);
}

// Rotate m about axis by angle, keeping its translation.
static inline void Rotate_8A00(t4Matrix& m, const t4Vector& axis, float angle)
{
    t4Matrix old;
    float s;
    float c;
    CopyMat_8A00(&old, &m);
    t4Vector trans = old.row[3];
    old.row[3] = t4vect_Origin_8A00;
    fSinCos(&s, &c, angle);
    float t = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    m.row[3].x = 0.0f;
    m.row[3].y = 0.0f;
    m.row[3].z = 0.0f;
    m.row[3].w = 1.0f;
    m.row[3] = trans;
    m.row[0].w = 0.0f;
    m.row[1].w = 0.0f;
    m.row[2].w = 0.0f;
    m.row[0].x = tx * x + c;
    m.row[1].x = tx * y + sz;
    m.row[2].x = tx * z - sy;
    m.row[0].y = ty * x - sz;
    m.row[1].y = ty * y + c;
    m.row[2].y = ty * z + sx;
    m.row[0].z = tz * x + sy;
    m.row[1].z = tz * y - sx;
    m.row[2].z = tz * z + c;
    m *= old;
}

static inline void PushRS_8A00(cPS2GraphicsMan* ctx)
{
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
}

static inline void SetBits_8A00(int& w, int mask, int shift, int v)
{
    w = (w & ~mask) | ((v << shift) & mask);
}

/**
 * Debug-draws an active node's recorded path when cAvalanche_m_RenderLines (On Tour m_RenderLines) is set.
 *
 * Walks the avalanche node's packed frames from its start (position deltas x 2, frame colours) into the
 * vertex array vertlist and draws them with the graphics manager's virtual at vtable offset 0x248. The
 * rotation it builds per frame is not used for the vertices.
 *
 */
void tActiveAvalancheNode::renderLines()
{
    tActiveAvalancheNode* node = this;
    if ((node->m_node) == 0) {
        return;
    }
    t4Matrix matrix;
    CopyMat_8A00(&matrix, &t4mat_Identity_8A00);
    PushRS_8A00(GrMan);
    SetBits_8A00(((int)(GrMan->m_pCurrentDrawState)->m_bitGroups[1]), 0x7C, 2, 1);
    if (cAvalanche_m_RenderLines != 0) {
        int i = 0;
        t4Vector pos = (*(t4Vector*)&(node->m_node)->m_start);
        for (; i < (int)(node->m_node)->m_life; i++) {
            t4VectorCtor scratchVec(((node->m_node)->m_history)[i].m_x * 2.0f,
                                    ((node->m_node)->m_history)[i].m_y * 2.0f,
                                    ((node->m_node)->m_history)[i].m_z * 2.0f, 0.0f);
            AddTo_8A00(pos, scratchVec);
            t3VectorCtor n = Normalize_8A00(t3VectorCtor(((node->m_node)->m_history)[i].m_rx * 0.007874015718698502f,
                                                         ((node->m_node)->m_history)[i].m_ry * 0.007874015718698502f,
                                                         ((node->m_node)->m_history)[i].m_rz * 0.007874015718698502f));
            scratchVec.x = n.x;
            scratchVec.y = n.y;
            scratchVec.z = n.z;
            scratchVec.w = 0.0f;
            Rotate_8A00(matrix, scratchVec, ((node->m_node)->m_history)[i].m_rs * 0.008213315159082413f);
            matrix.row[3] = pos;
            vertlist_8A00[i].r = ((node->m_node)->m_history)[i].m_r;
            vertlist_8A00[i].g = ((node->m_node)->m_history)[i].m_g;
            vertlist_8A00[i].b = ((node->m_node)->m_history)[i].m_b;
            vertlist_8A00[i].pos = pos;
        }
        // vtable slot 73 (byte offset 0x248)
        sVEntry<void(void*, int, void*, int)>* vtable =
            (sVEntry<void(void*, int, void*, int)>*)VPTR_OF(cGraphicsMan, GrMan);
        vtable[73].fn(vthis(GrMan, vtable[73]), (int)(node->m_node)->m_life, vertlist_8A00, 0);
    }
    (*(sDrawState**)&GrMan->m_pCurrentDrawState)--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void tActiveAvalancheNode_renderLines(tActiveAvalancheNode* node);
extern "C" void cDynamicColourEmitter_render(void* p, int a);
extern int cAvalanche_m_RenderEmitters;
extern int cAvalanche_m_RenderLines;


struct sAvVert_D8EA8 {
    t4Vector uv;           // 0x0
    int r, g, b, a;             // 0x10
    t4Vector pos;          // 0x20
};
extern sAvVert_D8EA8 D_vtx_D8EA8[] __asm__("collisionvertlist");

class cRiderPos_D8EA8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual t4Vector* getPos();
};

/**
 * Renders one active avalanche: its emitters and, in debug mode, its paths and collision lines.
 *
 * For each active node: path lines for the first cAvalanche_m_RenderLines nodes, and cDynamicColourEmitter_render(emitter, 7) when
 * cAvalanche_m_RenderEmitters (On Tour m_RenderEmitters, default 1) is set and the node has an emitter. With cAvalanche_m_RenderLines set it
 * also draws a line from each collision instance to the player's rider, red once the collision time has
 * passed and green before.
 *
 */
void tActiveAvalanche::render()
{
    tActiveAvalanche* slot = this;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (slot->m_avalanche == 0)
        return;
    int i = 0;
    for (tActiveAvalancheNode* n = slot->m_activeNodeList; n != 0; n = n->m_next, i++) {
        if (n->m_node) {
            if (i < cAvalanche_m_RenderLines)
                n->renderLines();
            if (cAvalanche_m_RenderEmitters && n->m_node->m_hasEmitter)
                cDynamicColourEmitter_render(&n->m_emitter, 7);
        }
    }
    if (cAvalanche_m_RenderLines == 0)
        return;
    for (int j = 0; j < slot->m_avalanche->m_numCollisions; j++) {
        // the instance's position: the translation row of its transform (a float[4] read as a t4Vector)
        t4Vector nodePos = *(t4Vector*)slot->m_avalanche->m_collisions[j].m_inst->m_fmTransform.m[3];
        D_vtx_D8EA8[2 * j].pos = nodePos;
        D_vtx_D8EA8[2 * j].r = slot->m_avalanche->m_collisions[j].m_time < slot->m_currTime ? 0xFF : 0;
        D_vtx_D8EA8[2 * j].g = slot->m_avalanche->m_collisions[j].m_time >= slot->m_currTime ? 0xFF : 0;
        cRider* rider = SSXApp->Game->AI->Riders[0];
        D_vtx_D8EA8[2 * j + 1].pos = *((cRiderPos_D8EA8*)(&VPTR_AT(rider, sizeof(cRiderState))))->getPos();
        D_vtx_D8EA8[2 * j + 1].r = slot->m_avalanche->m_collisions[j].m_time < slot->m_currTime ? 0xFF : 0;
        D_vtx_D8EA8[2 * j + 1].g = slot->m_avalanche->m_collisions[j].m_time >= slot->m_currTime ? 0xFF : 0;
    }
    sDrawState* top = ctx->m_pCurrentDrawState;
    unsigned int bits = top->m_bitGroups[1];
    top->m_bitGroups[1] = (bits & 0xFE7FFFFF) | 0x1000000;
    int saved = (bits & 0x1800000) >> 23;
    ctx->RenderLineStrip(slot->m_avalanche->m_numCollisions, (const tBxVertex*)D_vtx_D8EA8, 0);
    ctx->m_pCurrentDrawState->m_bitGroups[1] =
        (ctx->m_pCurrentDrawState->m_bitGroups[1] & 0xFE7FFFFF) | ((saved << 23) & 0x1800000);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_render);
#ifdef SKIP_ASM
extern int cPathArrow_m_Enable;
extern sDrawState sDrawState_s_defaultDrawState;
extern "C" void tActiveAvalanche_render(tActiveAvalanche* slot);

static inline cGameViewMan* getViewMan_9130() { return SSXApp->Game->ViewMan; }

// Sets the 5-bit field at bits 5..9 of the draw state's third bit group.
static inline void setB5_9130(sDrawState* drawState, int v)
{
    drawState->m_bitGroups[2] = (((int)drawState->m_bitGroups[2]) & ~(0x1F << 5)) | ((v << 5) & (0x1F << 5));
}

/**
 * Renders all active avalanches with the current camera's view.
 *
 * Runs when the system is enabled (cAvalanche_m_Enable) and resolved, or when cPathArrow_m_Enable is set: pushes a draw state
 * set up for the camera's matrix, position and rotation and, if enabled, renders the 16 active slots.
 */
extern "C" void cAvalanche_render(void)
{
    if ((cAvalanche_m_Enable != 0 && cAvalanche_m_resolved != 0) || cPathArrow_m_Enable != 0)
    {
        cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
        cCamera* camera = getViewMan_9130()->PlayerCamera[getViewMan_9130()->m_uiCurrentView];
        ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
        ctx->m_pCurrentDrawState++;
        *ctx->m_pCurrentDrawState = sDrawState_s_defaultDrawState;
        setB5_9130(ctx->m_pCurrentDrawState, 7);
        // Stored through a plain short pointer: retail keeps the view-manager loads below this store.
        short* texid = &ctx->m_pCurrentDrawState->m_textureState.m_texid[0];
        *texid = -1;
        ctx->SetViewportWindow1(getViewMan_9130()->m_ViewRect[getViewMan_9130()->m_uiCurrentView], 1);
        ctx->SetViewportPerspective(camera->View, camera->Near, camera->Far);
        ctx->SetView1(camera->CamMat);
        if (cAvalanche_m_Enable != 0)
        {
            for (int i = 0; i < 16; i++)
                tActiveAvalanche_render((tActiveAvalanche*)(&cAvalanche_m_activeAvalanches[i]));
        }
        ctx->m_pCurrentDrawState--;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_Avalanche[];
extern char str_AvalancheNode[];
extern tAvalanche* cAvalanche_m_avalancheList;
extern tAvalancheNode* cAvalanche_m_nodeList[];
extern int cAvalanche_m_numNodes;
extern int cAvalanche_m_resolved;

// PORT: abs.s via inline asm (as an SDK math-header fabsf would).
static inline float Abs_D92D0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define AV_MIN_D92D0(a, b) ((a) < (b) ? (a) : (b))

/**
 * Adds a node (a falling object) to the avalanche with the given id, creating the avalanche if needed.
 *
 * The new node takes the instance, life/fade-in/fade-out in frames (seconds x 30), the speed, a radius of 0.2
 * x the instance's smallest bound extent and a base type (kScriptType_Permanent gives kBase_Permanent,
 * kScriptType_Bounce kBase_Bounce, else kBase_NoBounce). It is registered in
 * the node table cAvalanche_m_nodeList and its index is written into the instance's m_sRenderFlags ((index << 8) | 2).
 *
 * @param id  avalanche id
 * @param obj  the world instance that falls
 * @param type  the instance's kind, mapped to the node's base type
 * @param life  life in seconds
 * @param fadeIn  fade-in time in seconds
 * @param fadeOut  fade-out time in seconds
 * @param nodeSpeed  speed (scales the emitter time step)
 */
extern "C" void cAvalanche_addAvalancheNode(int id, tInstanceRecord* obj, int type, float life, float fadeIn,
                                           float fadeOut, float nodeSpeed)
{
    tAvalanche* avalanche = cAvalanche_m_avalancheList;
    while (avalanche != 0 && avalanche->m_id != id)
        avalanche = avalanche->m_next;
    if (avalanche == 0) {
        avalanche = new (str_Avalanche, 0, 0) tAvalanche;
        // tAvalanche::init(id) (On Tour declares it; inlined). m_next is overwritten right after by the list link.
        avalanche->m_id = id;
        avalanche->m_next = 0;
        avalanche->m_numCollisions = 0;
        avalanche->m_numGroundCollisions = 0;
        avalanche->m_nodeList = 0;
        avalanche->m_next = cAvalanche_m_avalancheList;
        cAvalanche_m_avalancheList = avalanche;
    }
    tAvalancheNode* n = new (str_AvalancheNode, 0, 0) tAvalancheNode;
    n->m_next = avalanche->m_nodeList;
    avalanche->m_nodeList = n;
    n->m_speed = nodeSpeed;
    n->m_life = life * 30.0f;
    n->m_fadein = fadeIn * 30.0f;
    n->m_fadeout = fadeOut * 30.0f;
    n->m_instance = obj;
    // The target materialises &n->radius (addiu v1,a0,0x110; swc1 0(v1)): the source bound the field by reference.
    float& radius = n->m_radius;
    radius = AV_MIN_D92D0(AV_MIN_D92D0(Abs_D92D0(obj->m_fvMax.m[0] - obj->m_fvMin.m[0]),
                                       Abs_D92D0(obj->m_fvMax.m[1] - obj->m_fvMin.m[1])),
                          Abs_D92D0(obj->m_fvMax.m[2] - obj->m_fvMin.m[2])) *
             0.20000000298023224f;
    n->m_history = 0;
    n->m_historyDeleteable = 0;
    switch (type) {
    case tAvalancheNode::kScriptType_Permanent:
        n->m_baseType = tAvalancheNode::kBase_Permanent;
        break;
    case tAvalancheNode::kScriptType_Bounce:
        n->m_baseType = tAvalancheNode::kBase_Bounce;
        break;
    default:
        n->m_baseType = tAvalancheNode::kBase_NoBounce;
        break;
    }
    n->m_hasEmitter = 0;
    n->m_activeNode = 0;
    int idx = cAvalanche_m_numNodes;
    cAvalanche_m_nodeList[idx] = n;
    n->m_instance->m_sRenderFlags = (unsigned char)n->m_instance->m_sRenderFlags;
    n->m_instance->m_sRenderFlags |= (idx << 8) | 2;
    cAvalanche_m_numNodes = idx + 1;
    cAvalanche_m_resolved = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_setAvalancheNodeEmitterData);
#ifdef SKIP_ASM
extern tAvalanche* cAvalanche_m_avalancheList;

/**
 * Sets the emitter parameters of the avalanche node bound to an instance and marks it as having an emitter.
 *
 * Searches every avalanche in cAvalanche_m_avalancheList (linked through +0x8) and each one's node list (+0x4, linked
 * through +0xF4) for the node whose instance (+0xF8) is @p id. Nodes of type 2 (+0xF0) are left alone; otherwise it
 * sets the emitter flag (+0xF2) to 1 and copies the 0xD8 bytes of @p params over the node's start. Stops at the first
 * match. The world script function SetAvalancheEmitterDataFunc calls it.
 *
 * @param id  the instance record the node belongs to
 * @param params  the 0xD8-byte emitter parameters to copy
 */
extern "C" void cAvalanche_setAvalancheNodeEmitterData(int id, tParticleData* params)
{
    for (tAvalanche* group = (tAvalanche*)cAvalanche_m_avalancheList; group != 0; group = group->m_next) {
        for (tAvalancheNode* n = (group->m_nodeList); n != 0; n = n->m_next) {
            if (((int)n->m_instance) == id) {
                if (n->m_baseType == tAvalancheNode::kBase_Permanent) return;
                n->m_hasEmitter = 1;
                n->m_data = *params;
                return;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tAvalanche_calculate);
#ifdef SKIP_ASM
extern "C" void tAvalancheNode_calculate(void* node, void* self);

/**
 * Runs tAvalancheNode_calculate() on every node of the avalanche, following each node's next link (+0xF4).
 */
void tAvalanche::calculate()
{
    tAvalancheNode* n = m_nodeList;
    while (((void*)n) != 0) {
        tAvalancheNode_calculate(n, this);
        n = (tAvalancheNode*)((void*)n->m_next);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tAvalanche_getNode);
#ifdef SKIP_ASM
/**
 * Returns the node of this avalanche bound to an instance, or 0.
 *
 * Walks the node list (m_nodeList, linked through m_next) for the node whose m_instance is @p id. Nothing in the
 * retail code references it.
 *
 * @param id  the instance record to look for
 * @return the matching node, or 0
 */
tAvalancheNode* tAvalanche::getNode(tInstanceRecord* id)
{
    for (tAvalancheNode* n = m_nodeList; n != 0; n = n->m_next) {
        if (n->m_instance == id) {
            return n;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_calculate);
#ifdef SKIP_ASM
extern "C" void cAvalanche_end(void);
extern "C" void cAvalanche_SaveData(void);
extern "C" void cAvalanche_LoadData(void);
extern tAvalanche* cAvalanche_m_avalancheList;

/**
 * Recalculates every avalanche, then saves and reloads the data file.
 *
 * Ends the active avalanches, runs tAvalanche_calculate on each, then cAvalanche_SaveData and
 * cAvalanche_LoadData.
 */
extern "C" void cAvalanche_calculate(void)
{
    cAvalanche_end();
    for (tAvalanche* avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
        avalanche->calculate();
    }
    cAvalanche_SaveData();
    cAvalanche_LoadData();
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tAvalanche_addGroundCollision);
#ifdef SKIP_ASM
/**
 * Records a ground collision in this avalanche's table: a time and the position of @p stop in the node list.
 *
 * Does nothing once m_numGroundCollisions reaches 64. Otherwise counts the nodes from m_nodeList up to @p stop
 * (through m_next) and stores (@p time, that index) as the next m_groundCollisions entry. tAvalancheNode_calculate
 * calls it; the entries are later sorted with compareAvalancheGroundCollisions.
 *
 * @param time  the time to record (stored as a short)
 * @param stop  the node that hit the ground
 */
void tAvalanche::addGroundCollision(int time, tAvalancheNode* stop)
{
    if (m_numGroundCollisions < 64) {
        int depth = 0;
        tAvalancheNode* node = m_nodeList;
        while (node != 0 && node != stop) {
            depth++;
            node = node->m_next;
        }
        m_groundCollisions[m_numGroundCollisions].m_time = time;
        m_groundCollisions[m_numGroundCollisions].m_nodeId = depth;
        m_numGroundCollisions++;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);
#ifdef SKIP_ASM
extern "C" float cRenderStateMan_SnowFlakeColourR(int a);
extern "C" float cRenderStateMan_SnowFlakeColourG(int a);
extern "C" float cRenderStateMan_SnowFlakeColourB(int a);
extern "C" void cBaseClass_DynamicEmitter_setupEmitter(void* self, void* node, float k);
extern "C" void tActiveAvalancheNode_calculateScale(tActiveAvalancheNode* self, float t);

extern "C" void cSSXAudio_Avalanche(void* self, int a1);
extern tAvalanche* cAvalanche_m_avalancheList;

extern t4Matrix t4mat_Identity_D97A8 __asm__("t4mat_Identity");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMatrix_D97A8(t4Matrix* dst, t4Matrix* src)
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

extern tActiveAvalancheNode D_act_D97A8[] __asm__("cAvalanche_m_activeNodes");

extern tActiveAvalanche D_slots_D97A8[] __asm__("cAvalanche_m_activeAvalanches");

/**
 * Starts the avalanche with the given id in a free active slot.
 *
 * Gives each of its nodes a free active node from the 64-entry pool (position, rotation, scale, and for nodes
 * with an emitter a reset emitter in the snow-flake colour), builds the node array, calls cSSXAudio_Avalanche(sound
 * manager, 1) and counts the avalanche in the danger manager (mNumActiveAvalanche).
 *
 * @param id  avalanche id
 * @return the slot index (0-15), or 0x80000000 when the id is unknown, already active, or no slot or node is
 *         free
 */
extern "C" int cAvalanche_triggerAvalanche(int id)
{
    cAvalanche_resolveDataPointers();
    for (tAvalanche* avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
        if (avalanche->m_id != id)
            continue;
        for (int j = 0; j < 16; j++) {
            if ((D_slots_D97A8[j].m_avalanche) == avalanche)
                return (int)0x80000000;
        }
        tActiveAvalanche* slot = D_slots_D97A8;
        for (int i = 0; i < 16; i++, slot++) {
            if ((slot->m_avalanche) != 0)
                continue;
            (*(tAvalanche**)&slot->m_avalanche) = avalanche;
            (*(tActiveAvalancheNode**)&slot->m_activeNodeList) = 0;
            slot->m_currTime = 0.0f;
            slot->m_nextCollision = 0;
            slot->m_nextGroundCollision = 0;
            tActiveAvalancheNode* act = D_act_D97A8;
            int k = 0;
            for (tAvalancheNode* n = (avalanche->m_nodeList); n != 0; n = n->m_next) {
                for (; k < cAvalanche::kMaxActiveNodes; k++, act++) {
                    if ((act->m_node) == 0) {
                        (*(tAvalancheNode**)&act->m_node) = n;
                        n->m_activeNode = act;
                        act->m_next = (slot->m_activeNodeList);
                        (*(tActiveAvalancheNode**)&slot->m_activeNodeList) = act;
                        act->m_lastKeyFrame = 0;
                        act->m_lastKeyFramePosition = n->m_start;
                        CopyMatrix_D97A8(&act->m_lastKeyFrameRotation, &t4mat_Identity_D97A8);
                        act->m_updateEmitters = 0;
                        {
                            // the instance transform's translation row
                            t4Vector objPosition = *(t4Vector*)&n->m_instance->m_fmTransform.m[3][0];
                            act->m_position = objPosition;
                        }
                        CopyMatrix_D97A8(&act->m_rotation, &t4mat_Identity_D97A8);
                        act->m_colour = t4VectorCtor(1.0f, 1.0f, 1.0f, 1.0f);
                        act->calculateScale(0.0f);
                        if (n->m_hasEmitter) {
                            cDynamicColourEmitter_reset(((char*)&act->m_emitter));
                            cBaseClass_DynamicEmitter_setupEmitter(((char*)&act->m_emitter), n, 2.0f);
                            act->m_emitter.m_IsAvalanche = 1;
                            float flakeRed = cRenderStateMan_SnowFlakeColourR(0);
                            float gg = cRenderStateMan_SnowFlakeColourG(0);
                            float flakeBlue = cRenderStateMan_SnowFlakeColourB(0);
                            act->m_emitter.m_emitter.C0 = t4VectorCtor(flakeRed, gg, flakeBlue, 0.0f);
                        }
                        break;
                    }
                }
                if (k == cAvalanche::kMaxActiveNodes) {
                    (*(tAvalanche**)&slot->m_avalanche) = 0;
                    for (act = (slot->m_activeNodeList); act != 0; act = act->m_next) {
                        if ((act->m_node))
                            (act->m_node)->m_activeNode = 0;
                        (*(tAvalancheNode**)&act->m_node) = 0;
                    }
                    return (int)0x80000000;
                }
            }
            slot->buildArray();
            cSSXAudio_Avalanche(cAudioInterface_bxAudio(), 1);
            (SSXApp->Game->AI->DangerMan->mNumActiveAvalanche)++;
            return slot - D_slots_D97A8;
        }
    }
    return (int)0x80000000;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_end);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cAvalanche_m_activeAvalanches as its real type (cAvalanche_update declares it earlier in the unit with its own
// element type).
extern tActiveAvalanche activeAvalanchesForEnd[] __asm__("cAvalanche_m_activeAvalanches");
extern int cAvalanche_m_fileDataDeleteable;
void cMemMan_free(void*);
extern "C" void tActiveAvalanche_end(tActiveAvalanche* self);

static inline void freeSlot_9A80(tActiveAvalanche* slot)
{
    if (slot->m_avalanche != 0) {
        slot->end();
    }
}

/**
 * Stops all active avalanches and, for loaded data, drops each node's owned frame history.
 */
extern "C" void cAvalanche_end(void)
{
    int i;
    for (i = 0; i < 16; i++) {
        freeSlot_9A80(&activeAvalanchesForEnd[i]);
    }
    if (cAvalanche_m_fileDataDeleteable != 0) {
        for (tAvalanche* avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
            for (tAvalancheNode* n = avalanche->m_nodeList; n != 0; n = n->m_next) {
                if (n->m_historyDeleteable != 0 && n->m_history != 0) {
                    cMemMan_free(n->m_history);
                }
                n->m_historyDeleteable = 0;
                n->m_history = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_reset);
#ifdef SKIP_ASM
extern "C" void cAvalanche_end(void);
extern "C" void memset(void* dst, int c, int n);
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern tAvalancheNode* cAvalanche_m_nodeList[];
extern int cAvalanche_m_numNodes;
extern void* cAvalanche_m_filedata;
extern int cAvalanche_m_fileDataDeleteable;
extern int cAvalanche_m_resolved;

/**
 * Deletes all avalanches and nodes and frees the loaded avalanche data.
 *
 * Also clears the node table and its count and the resolved flag. self is unused (On Tour: a static method).
 */
void cAvalanche::reset()
{
    cAvalanche_end();
    tAvalanche* avalanche = cAvalanche_m_avalancheList;
    while (avalanche != 0) {
        tAvalanche* cur = avalanche;
        tAvalancheNode* n = avalanche->m_nodeList;
        while (n != 0) {
            tAvalancheNode* next = n->m_next;
            operator_delete((int*)n);
            n = next;
        }
        avalanche = avalanche->m_next;
        operator_delete((int*)cur);
    }
    cAvalanche_m_avalancheList = 0;
    cAvalanche_m_numNodes = 0;
    memset(cAvalanche_m_nodeList, 0, 0x400);
    if (cAvalanche_m_filedata != 0 && cAvalanche_m_fileDataDeleteable != 0) {
        cMemMan_free(cAvalanche_m_filedata);
        cAvalanche_m_filedata = 0;
        cAvalanche_m_fileDataDeleteable = 0;
    }
    cAvalanche_m_resolved = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_getTransform);
#ifdef SKIP_ASM

extern tActiveAvalancheNode cAvalanche_m_activeNodes[64];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixAV(t4Matrix* dst, t4Matrix* src)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix times scalar).
static inline void vu0ScaleMatrixAV(t4Matrix* dst, t4Matrix* src, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %0, %3\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "qmtc2.ni  %0, $vf3\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulx.xyzw $vf8, $vf4, $vf3x\n"
        "vmulx.xyzw $vf9, $vf5, $vf3x\n"
        "vmulx.xyzw $vf10, $vf6, $vf3x\n"
        "vmulx.xyzw $vf11, $vf7, $vf3x\n"
        "sqc2      $vf8, 0x0(%1)\n"
        "sqc2      $vf9, 0x10(%1)\n"
        "sqc2      $vf10, 0x20(%1)\n"
        "sqc2      $vf11, 0x30(%1)\n"
        : "=&r"(t)
        : "r"(dst), "r"(src), "f"(s)
        : "memory");
}

/**
 * Builds the world transform of the active avalanche node bound to an instance.
 *
 * Searches the 64 cAvalanche_m_activeNodes for one whose node (m_node) has instance (m_instance) @p id, then writes
 * its m_rotation scaled by its m_objectScale to @p out with m_position as row 3. Leaves @p out unchanged when there is
 * no match. cObjectInterface_getAvaSplineTransform (util/objectinterface) forwards to it.
 *
 * @param id  the instance record to look for
 * @param out  receives the transform
 */
extern "C" void cAvalanche_getTransform(tInstanceRecord* id, t4Matrix* out)
{
    int i;
    for (i = 0; i < 64; i++) {
        tActiveAvalancheNode* node = &cAvalanche_m_activeNodes[i];
        if (node->m_node != 0 && node->m_node->m_instance == id) {
            vu0CopyMatrixAV(out, &node->m_rotation);
            vu0ScaleMatrixAV(out, out, node->m_objectScale);
            out->row[3] = node->m_position;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cAvalanche_writeToReplayFrame_sSlot {
    int* av;
    int pad4[5];
    float t;
    int isActive() { return av != 0; }
};
extern cAvalanche_writeToReplayFrame_sSlot cAvalanche_m_activeAvalanches_9CB0[16] __asm__("cAvalanche_m_activeAvalanches");

struct cAvalanche_writeToReplayFrame_sSave {
    int ids[16];
    float times[16];
};

/**
 * Writes the avalanche replay state to a replay stream: the id and current time of each of the 16 active-avalanche
 * slots.
 *
 * For each slot of cAvalanche_m_activeAvalanches it stores the avalanche's id (first word of the slot's avalanche) or
 * -1 when the slot is empty, and the slot's time (+0x18) or 0. The 0x80-byte block goes out through the stream's first
 * virtual function (data, size). cReplay_saveFrame calls it.
 *
 * @param stream  the replay stream
 */
extern "C" void cAvalanche_writeToReplayFrame(cBxReplayFrame* stream)
{
    cAvalanche_writeToReplayFrame_sSave state;
    for (int i = 0; i < 16; i++) {
        state.ids[i] = cAvalanche_m_activeAvalanches_9CB0[i].isActive() ? *cAvalanche_m_activeAvalanches_9CB0[i].av : -1;
        state.times[i] = cAvalanche_m_activeAvalanches_9CB0[i].isActive() ? cAvalanche_m_activeAvalanches_9CB0[i].t : 0.0f;
    }
    stream->write((const char*)&state, 0x80);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sFrame_RR {
    char b[96];
};
// cAvalanche_m_activeAvalanches as its real type (cAvalanche_update declares it earlier in the unit with its own view).
extern tActiveAvalanche D_slots_RR[] __asm__("cAvalanche_m_activeAvalanches");
extern "C" int cAvalanche_triggerAvalanche(int id);
extern "C" void tActiveAvalanche_end(tActiveAvalanche* slot);
extern "C" void tActiveAvalancheNode_calculateScale(tActiveAvalancheNode* self, float t);
sFrame_RR getFrameData_RR(void* self, float t) __asm__("tActiveAvalancheNode_getFrameData");

static inline tActiveAvalanche* slots_RR()
{
    return D_slots_RR;
}

static inline void clearSlot_RR(tActiveAvalanche* s)
{
    if (s->m_avalanche != 0) {
        s->end();
    }
}

/**
 * Restores the active avalanches from a replay frame.
 *
 * self is the replay stream, not a cAvalanche: it reads 16 avalanche ids and 16 times (0x80 bytes) through
 * the stream's virtual slot 2. Each id of 0 or more is re-triggered and wound to its time: node scales and
 * frames are recomputed (emitter nodes re-emit) and the slot's collision cursors (m_nextCollision,
 * m_nextGroundCollision) skip events
 * already past.
 */
extern "C" void cAvalanche_readFromReplayFrame(cBxReplayFrame* self)
{
    struct {
        int ids[16];
        float times[16];
    } buf;
    int i;
    for (i = 0; i < 16; i++) {
        clearSlot_RR(&slots_RR()[i]);
    }
    {
        self->read((char*)buf.ids, 0x80);
    }
    for (i = 0; i < 16; i++) {
        if (buf.ids[i] >= 0) {
            int n = cAvalanche_triggerAvalanche(buf.ids[i]);
            if (n >= 0) {
                tActiveAvalanche* slot = &slots_RR()[n];
                slot->m_currTime = buf.times[i];
                if (buf.times[i] != 0.0f) {
                    tActiveAvalancheNode* activeNode = slot->m_activeNodeList;
                    while (activeNode != 0) {
                        if (activeNode->m_node->m_hasEmitter != 0) {
                            activeNode->m_updateEmitters = 1;
                        }
                        activeNode->calculateScale(buf.times[i]);
                        getFrameData_RR(activeNode, slot->m_currTime);
                        activeNode = activeNode->m_next;
                    }
                    while (slot->m_nextCollision < slot->m_avalanche->m_numCollisions
                           && slot->m_avalanche->m_collisions[slot->m_nextCollision].m_time <= slot->m_currTime) {
                        slot->m_nextCollision++;
                    }
                    while (slot->m_nextGroundCollision < slot->m_avalanche->m_numGroundCollisions
                           && (float)slot->m_avalanche->m_groundCollisions[slot->m_nextGroundCollision].m_time
                                  <= slot->m_currTime) {
                        slot->m_nextGroundCollision++;
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolve);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cAvalanche_resolveDataPointers(void);
extern void* cAvalanche_m_filedata;
extern int cAvalanche_m_fileDataDeleteable;
extern int cAvalanche_m_badCookie;

/**
 * Installs a loaded avalanche data chunk as the current avalanche file.
 *
 * Calls cAvalanche_reset, frees cAvalanche_m_filedata if it is deletable, then stores @p data as the (non-deletable)
 * file. A file whose first word is not 0x2BEEF00 is dropped and cAvalanche_m_badCookie is set; otherwise the flag is
 * cleared and cAvalanche_resolveDataPointers fixes up the file.
 *
 * @param data  the loaded avalanche chunk
 */
extern "C" void cAvalanche_resolve(void* data)
{
    ((cAvalanche*)data)->reset();
    if (cAvalanche_m_filedata != 0 && cAvalanche_m_fileDataDeleteable != 0) {
        cMemMan_free(cAvalanche_m_filedata);
    }
    cAvalanche_m_filedata = data;
    cAvalanche_m_fileDataDeleteable = 0;
    if (*(int*)data != cAvalanche::kFileMagic) {
        cAvalanche_m_filedata = 0;
        cAvalanche_m_badCookie = 1;
    } else {
        cAvalanche_m_badCookie = 0;
        cAvalanche_resolveDataPointers();
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern tAvalanche* cAvalanche_m_avalancheList;
extern void* cAvalanche_m_filedata;
extern int cAvalanche_m_resolved;

/**
 * Points the avalanche nodes at their records in the loaded data file.
 *
 * Once per load: each node gets its start position and a borrowed pointer to its packed frames (an owned one
 * is freed first); each avalanche reads its collision times (instances cleared) and ground collisions. The
 * collision count is then set back to 0, so the loaded collisions are not used.
 */
extern "C" void cAvalanche_resolveDataPointers(void)
{
    if (cAvalanche_m_avalancheList == 0)
        return;
    if (cAvalanche_m_filedata == 0)
        return;
    if (cAvalanche_m_resolved != 0)
        return;
    // ORIGINAL-PTR: the loaded avalanche file is a packed byte stream read through a byte cursor: the cookie and a
    // word, then per node a word, its start (3 floats) and m_life x 10 bytes of packed frames, per avalanche the two
    // halfword counts, 8-byte collision records and 4-byte ground collision records.
    char* cursor = (char*)cAvalanche_m_filedata + 8;
    for (tAvalanche* avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->m_next) {
        for (tAvalancheNode* n = avalanche->m_nodeList; n != 0; n = n->m_next) {
            cursor += 4;
            float* start = (float*)cursor;
            n->m_start = t4VectorCtor(start[0], start[1], start[2], 1.0f);
            cursor += 12;
            if (n->m_historyDeleteable && n->m_history)
                cMemMan_free(n->m_history);
            n->m_history = (tAvalanchePackedFrameData*)cursor;
            n->m_historyDeleteable = 0;
            cursor += (int)n->m_life * 10;
        }
        avalanche->m_numCollisions = *(unsigned short*)cursor;
        cursor += 2;
        avalanche->m_numGroundCollisions = *(unsigned short*)cursor;
        cursor += 2;
        for (int i = 0; i < avalanche->m_numCollisions; i++) {
            avalanche->m_collisions[i].m_time = *(float*)cursor;
            avalanche->m_collisions[i].m_inst = 0;
            cursor += 8;
        }
        for (int j = 0; j < avalanche->m_numGroundCollisions; j++) {
            avalanche->m_groundCollisions[j].m_nodeId = *(unsigned short*)cursor;
            cursor += 2;
            avalanche->m_groundCollisions[j].m_time = *(unsigned short*)cursor;
            cursor += 2;
        }
        avalanche->m_numCollisions = 0;
    }
    cAvalanche_m_resolved = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_getAvalancheDataByPosition);
#ifdef SKIP_ASM
// t4Vector with EA's inline constructors (On Tour t4Vector(), t4Vector(const float&, ...)).
struct sVec4_A1C0 : public t4Vector {
    sVec4_A1C0() {}
    sVec4_A1C0(const float& ax, const float& ay, const float& az, const float& aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};
struct sAvInfo_A1C0 {
    sVec4_A1C0 center;          // 0x0
    float total;                // 0x10
    float minDist;              // 0x14
    int count;                  // 0x18
    bool any;                   // 0x1C
};
extern sVec4_A1C0 t4vect_Zero;
// cAvalanche_m_activeAvalanches as its real type (cAvalanche_update declares it earlier in the unit with its own view).
extern tActiveAvalanche activeAvalanches_A1C0[] __asm__("cAvalanche_m_activeAvalanches");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_A1C0 vSub_A1C0(const t4Vector& a, const t4Vector& b)
{
    sVec4_A1C0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_A1C0(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vAddEq_A1C0(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void vDivEq_A1C0(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

/**
 * Gathers the active avalanche nodes near a position: their average position and scale, the nearest distance, their
 * count and whether any is a type-2 node.
 *
 * Walks each in-use slot of cAvalanche_m_activeAvalanches and its active-node list (m_activeNodeList, linked through
 * m_next), skipping nodes without a source node (m_node). Every node whose 4-component distance from (@p position, 1)
 * is below 225000000 is summed: m_position into +0x0, m_objectScale into +0x10, count +0x18, and the type-2 flag
 * (+0x1C, source node m_baseType == 2). +0x14 gets the smallest distance (999999 when none). The sums are then divided
 * by the count.
 *
 * Likely bug: the threshold 225000000 is 15000 squared but is compared with a plain distance. With no node in range it
 * divides by a zero count. Callers: cDangerMan_CalcAppliedDangerLevel (src/ai/control/handplantcontrol.cpp) and two
 * functions of src/sound/soundsys.cpp.
 *
 * @param position  the position to measure from
 * @return the 0x20-byte summary: average position, average scale, nearest distance, node count, type-2 flag
 */
// PORT: g++ `<?` (min) operator.
extern "C" sAvInfo_A1C0 cAvalanche_getAvalancheDataByPosition(t3Vector position)
{
    sAvInfo_A1C0 posData;
    posData.center = t4vect_Zero;
    posData.minDist = 999999.0f;
    posData.count = 0;
    posData.total = 0.0f;
    posData.any = false;
    for (int i = 0; i < 16; i++) {
        tActiveAvalanche* activeSlot = &activeAvalanches_A1C0[i];
        if (activeSlot->m_avalanche == 0)
            continue;
        for (tActiveAvalancheNode* n = activeSlot->m_activeNodeList; n != 0; n = n->m_next) {
            if (n->m_node == 0)
                continue;
            sVec4_A1C0 offset = vSub_A1C0(sVec4_A1C0(position.x, position.y, position.z, 1.0f), n->m_position);
            float dist = vLen_A1C0(offset);
            if (dist < 225000000.0f) {
                vAddEq_A1C0(posData.center, n->m_position);
                posData.total += n->m_objectScale;
                posData.minDist = posData.minDist <? dist;
                posData.any |= n->m_node->m_baseType == tAvalancheNode::kBase_Permanent;
                posData.count++;
            }
        }
    }
    vDivEq_A1C0(posData.center, (float)posData.count);
    posData.total = posData.total / (float)posData.count;
    return posData;
}
#endif

