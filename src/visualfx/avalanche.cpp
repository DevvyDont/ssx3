#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

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

extern float D_004A3A64;
extern float D_004A3AA8;
extern sV4_5778 D_004FF120_5778 __asm__("t4vect_Zero");
extern sV4_5778 D_004FF130_5778 __asm__("D_004FF130");
extern sMat_5778 D_004FF1A0_5778 __asm__("t4mat_Identity");
extern "C" void tActiveAvalancheNode_calculateScale_Data(void* self, float t) __asm__("tActiveAvalancheNode_calculateScale");
// PORT: sine/cosine; the angle is set up first at both calls here (other units declare it (s, c, angle),
// which uses the same registers), so it is declared angle-first under its own name.
extern "C" void func_0031BE50_5778(float angle, float* s, float* c) __asm__("func_0031BE50");
extern "C" void func_003717C0_Data(float dt, void* emitter, sV4_5778* pos, sV4_5778* vel, sV4_5778* col, int on) __asm__("func_003717C0");

// Rotates obj->mat by ANGLE about AXIS, keeping its translation (used on the active node and on the
// returned frame data, which both keep their matrix in `mat`).
template <class T>
static inline void RotateFrame_5778(T* obj, const sV4_5778& axis, float angle)
{
    sMat_5778& m = obj->mat;
    sMat_5778 saved = m;
    sV4_5778 trans = saved.r3;
    saved.r3 = D_004FF130_5778;
    float s;
    float c;
    func_0031BE50_5778(angle, &s, &c);
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
    self->mat = D_004FF1A0_5778;
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
// Steps the active node's path up to the frame at time t (emitting debris for the skipped frames while
// it is emitting) and returns the interpolated position, colour and orientation at t.
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
                        time = Min_5778(time + D_004A3A64 * self->data->rate, self->data->length * (1.0f / 30.0f));
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
                                                   D_004A3AA8);
                        self->f1FC = self->fB4;
                        func_003717C0_Data(1.0f / 60.0f, self->emitter, &pos, &D_004FF120_5778, &col, ft < self->data->length);
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
                            D_004A3AA8);
    fd.mat = self->mat;
    sV4_5778 axis(Normalize_5778(sV3_5778(self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778] * (1.0f / 127.0f),
                                          self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778 + 1] * (1.0f / 127.0f),
                                          self->data->frames[frame * kAvFrameSize_5778 + kAvAxis_5778 + 2] * (1.0f / 127.0f))));
    RotateFrame_5778(&fd, axis, frac * ((unsigned char)self->data->frames[frame * kAvFrameSize_5778 + kAvRot_5778] * 2.0943951f) * (1.0f / 255.0f));
    return fd;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D6290);
#ifdef SKIP_ASM
struct sVec4_6290 {
    float x, y, z, w;
    sVec4_6290() {}
    sVec4_6290(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
extern float D_004A3AA8;
extern sVec4_6290 D_004FF130;
extern char D_00538450[];

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_6290 vScale_6290(const sVec4_6290& v, float s)
{
    sVec4_6290 r;
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

extern "C" sVec4_6290 func_002D6290(void* self)
{
    char* node = ((char**)D_00538450)[*(unsigned short*)((char*)self + 0x92) >> 8];
    char* p = *(char**)(node + 0x114);
    if (p != 0) {
        return sVec4_6290(*(float*)(p + 0xCC), *(float*)(p + 0xC0), *(float*)(p + 0xC4), *(float*)(p + 0xC8));
    }
    unsigned char* c = *(unsigned char**)(node + 0xFC);
    if (c != 0) {
        return vScale_6290(sVec4_6290(1.0f, (float)c[3] * 0.0078125f, (float)c[4] * 0.0078125f, (float)c[5] * 0.0078125f), D_004A3AA8);
    }
    return D_004FF130;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche__cAvalanche);
#ifdef SKIP_ASM
extern void* cAvalanche_vtbl[];
extern void* D_00488680[];
struct cAvalanche__cAvalanche_sElem {
    char pad[0x2F0];
};
extern cAvalanche__cAvalanche_sElem D_004EE840[];
extern "C" void cAvalanche_reset(void* self);
extern "C" void cDynamicColourEmitter_purge(void* p, int flags);
void operator_delete(int* ptr);

/**
 * Destroys the avalanche effect: resets it, purges its 64 dynamic colour emitters (D_004EE840), restores the base
 * vtable (D_00488680) and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAvalanche__cAvalanche(int* self, int flags)
{
    *(void***)self = cAvalanche_vtbl;
    cAvalanche_reset(self);
    int i;
    for (i = 0; i < 64; i++) {
        cDynamicColourEmitter_purge(&D_004EE840[i], 1);
    }
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
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
extern cAvalanche_init_sElem D_004D5B70[];
extern cAvalanche_init_sElem D_004EDB70[];
extern char D_00538450[];
extern int D_004A3AC0;
extern "C" void func_00416210(void* dst, int c, int n);

/**
 * Initialises the avalanche's static vertex tables and clears its state.
 *
 * Sets the 0x800 entries of D_004D5B70 to alternating black and white (alpha 0xFF, z 1.0, x / y / w 0) and the 0x20
 * entries of D_004EDB70 to yellow, clears D_004A3AC0 and zeroes 0x400 bytes at D_00538450. On Tour's init is a
 * member (this); this C takes no object.
 */
extern "C" void cAvalanche_init(void)
{
    int i;
    for (i = 0; i < 0x800; i++) {
        int shade = (i & 1) ? 0xFF : 0;
        cAvalanche_init_sElem* elem = &D_004D5B70[i];
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
        cAvalanche_init_sElem* elem = &D_004EDB70[i];
        elem->a = 0xFF;
        elem->r = 0xFF;
        elem->g = 0xFF;
        elem->b = 0;
        elem->z = 1.0f;
        elem->w = 0.0f;
        elem->x = 0.0f;
        elem->y = 0.0f;
    }
    D_004A3AC0 = 0;
    func_00416210(D_00538450, 0, 0x400);
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D64D8);
#ifdef SKIP_ASM
struct sVec4_64D8 {
    float x, y, z, w;
    sVec4_64D8() {}
    sVec4_64D8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
extern float D_004A3A94;

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_64D8(const sVec4_64D8& v)
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
static inline sVec4_64D8 vNorm_64D8(const sVec4_64D8& v)
{
    sVec4_64D8 r;
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
static inline sVec4_64D8 vScale_64D8(const sVec4_64D8& v, float s)
{
    sVec4_64D8 r;
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

extern "C" void func_002D64D8(void* self, sVec4_64D8* v)
{
    if (D_004A3A94 < vLen_64D8(*v))
        *v = vScale_64D8(vNorm_64D8(*v), D_004A3A94);
    int bad = isBad_64D8(&v->x) || isBad_64D8(&v->y) || isBad_64D8(&v->z) || isBad_64D8(&v->w);
    if (bad)
        *v = sVec4_64D8(10.0f, 10.0f, 10.0f, 0.0f);
}
#endif

INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_calculate);

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_calculateScale);
#ifdef SKIP_ASM
struct sAvalancheData {
    char pad[0xF0];
    unsigned short type;
    char padF2[0xE];
    float f100;
    float f104;
    float f108;
};

/**
 * Computes an active node's object scale (+0xB0) and emitter alpha (+0xB4) for a time.
 *
 * In frames (time x 30): during the fade-in the alpha ramps up with scale 1; in the middle both are 1; during
 * the fade-out both fall linearly to 0 and the scale becomes the square root of that, except for base type 2
 * nodes, which keep their last values.
 *
 * @param currTime  time since the avalanche started, in seconds
 */
extern "C" void tActiveAvalancheNode_calculateScale(void* self, float currTime)
{
    sAvalancheData* avData = *(sAvalancheData**)((char*)self + 0x2E0);
    float x = currTime * 30.0f;
    if (x < avData->f104) {
        *(float*)((char*)self + 0xB0) = 1.0f;
        *(float*)((char*)self + 0xB4) = x / avData->f104;
        return;
    }
    if (x < avData->f100 - avData->f108) {
        *(float*)((char*)self + 0xB4) = 1.0f;
        *(float*)((char*)self + 0xB0) = 1.0f;
        return;
    }
    if (avData->type != 2) {
        float fadeOut = (avData->f100 - x) / avData->f108;
        *(float*)((char*)self + 0xB4) = fadeOut;
        *(float*)((char*)self + 0xB0) = fadeOut;
        if (fadeOut > 0.0f) {
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(fadeOut) : "f"(fadeOut));
            *(float*)((char*)self + 0xB0) = fadeOut;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", func_002D7CA8);
#ifdef SKIP_ASM
struct sAvVec4CA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sAvMatrixCA8 {
    sAvVec4CA8 row[4];
};
struct sAvFrameCA8 {
    sAvVec4CA8 pos;             // 0x0
    sAvVec4CA8 vel;             // 0x10
    sAvMatrixCA8 rot;           // 0x20
};
struct sAvDataCA8 {
    char pad0[0xF0];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    char padF3[0x100 - 0xF3];
    float duration;             // 0x100
    char pad104[0x10C - 0x104];
    float speed;                // 0x10C
};
struct sAvNodeCA8 {
    sAvFrameCA8 getFrameData(float t) __asm__("tActiveAvalancheNode_getFrameData");
    char pad0[0x60];
    sAvVec4CA8 pos;             // 0x60
    sAvMatrixCA8 rot;           // 0x70
    float scale;                // 0xB0
    float fB4;                  // 0xB4
    char padB8[0xC0 - 0xB8];
    sAvVec4CA8 vel;             // 0xC0
    char emitter[0x1FC - 0xD0]; // 0xD0
    float f1FC;                 // 0x1FC
    char pad200[0x2E0 - 0x200];
    sAvDataCA8* data;           // 0x2E0
};

extern "C" void tActiveAvalancheNode_end(void* self, int force);
extern "C" void func_003717C0(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern sAvVec4CA8 D_004FF120_7CA8 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixCA8(sAvMatrixCA8* dst, sAvMatrixCA8* src)
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

extern "C" int func_002D7CA8(sAvNodeCA8* self, float t)
{
    if (self->data == 0) return 0;
    t *= self->data->speed;
    sAvFrameCA8 fd = self->getFrameData(t);
    if (t * 30.0f < self->data->duration) {
        sAvVec4CA8* pos = &self->pos;
        self->pos = fd.pos;
        vu0CopyMatrixCA8(&self->rot, &fd.rot);
        self->vel = fd.vel;
        sAvDataCA8* d = self->data;
        sAvVec4CA8* vel = &self->vel;
        if (d->type != 2 && d->emit != 0) {
            void* org = &D_004FF120_7CA8;
            void* em = self->emitter;
            self->f1FC = self->fB4;
            int alive = t < d->duration;
            func_003717C0(em, pos, org, vel, alive, 0.01666666753590107f);
        }
        return 1;
    }
    if (self->data->type == 2) return 1;
    tActiveAvalancheNode_end(self, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_end);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_reset(void* self);

struct sVEntry2D7DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Releases an active node from its avalanche node.
 *
 * Only when it has a node and either force is set or the node's base type is not 2: calls virtual slot 1 with
 * 3 on the object at +0xC of the node's instance (unknown), clears the node's m_activeNode and this node's
 * m_node, and resets the emitter.
 *
 * @param force  end even base type 2 nodes
 */
extern "C" void tActiveAvalancheNode_end(void* self, int force)
{
    char* avData = *(char**)((char*)self + 0x2E0);
    if (avData != 0 && (force != 0 || *(unsigned short*)(avData + 0xF0) != 2)) {
        char* inst = *(char**)(avData + 0xF8);
        if (inst != 0) {
            char* obj = *(char**)(inst + 0xC);
            if (obj != 0) {
                sVEntry2D7DD8* vt = *(sVEntry2D7DD8**)(obj + 0xC);
                vt[1].fn(obj + vt[1].delta, 3);
            }
        }
        *(int*)(*(char**)((char*)self + 0x2E0) + 0x114) = 0;
        *(char**)((char*)self + 0x2E0) = 0;
        cDynamicColourEmitter_reset((char*)self + 0xD0);
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

struct tActiveAvalanche {
    int unk0;
    tActiveAvalanche_node* head;   // 0x4
    tActiveAvalanche_node** array; // 0x8
};

/**
 * Builds an array of pointers to the active avalanche's nodes, in list order.
 *
 * Counts the nodes linked from the list head, allocates that many pointers tagged "ActiveNodeArray", stores the
 * array in the avalanche and fills it.
 */
extern "C" void tActiveAvalanche_buildArray(tActiveAvalanche* self)
{
    tActiveAvalanche_node* node;
    int n = 0;
    for (node = self->head; node != 0; node = node->next) {
        n++;
    }
    self->array = (tActiveAvalanche_node**)operator_new_tag(n * 4, str_ActiveNodeArray, 0, 0);
    int i = 0;
    for (node = self->head; node != 0; node = node->next) {
        self->array[i++] = node;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D7EF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sAvV4_7EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvObj_7EF8 {
    char pad_0x00[0xC];
    void* spawned;          // 0xC
    char pad_0x10[0x30];
    sAvV4_7EF8 pos;         // 0x40
    char pad_0x50[0x10];
    sAvV4_7EF8 sndPos;      // 0x60
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

struct sAvActive_7EF8 {
    sAvData_7EF8* data;         // 0x0
    sAvNode_7EF8* nodes;        // 0x4
    sAvObj_7EF8** objs;         // 0x8
    int pad_0xC;
    int nextEvent;              // 0x10
    int nextSound;              // 0x14
    float time;                 // 0x18
};

struct sAvEnd_7EF8 {};  // empty trailing member of the defaults record; its value-init copies a byte through a stack temp

extern float D_004A3A64;
extern int D_004A54C8;
// Defaults record of an animated-float spawn (read by func_00341AA0, sSphFloatDef_1AA0 in intersect/worldsphtree).
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
extern "C" void* func_00341AA0(void* self, int a, int type, void* b, void* c);
extern "C" bool func_002D7CA8_7EF8(void* node, float t) __asm__("func_002D7CA8");
extern "C" void* func_0028B180();
extern "C" void func_002D81B0_7EF8(void* self) __asm__("func_002D81B0");
void func_0029E560_7EF8(void* snd, const sAvV4_7EF8& pos) __asm__("func_0029E560__FPv");

static inline unsigned short soundFrame_7EF8(const sAvActive_7EF8* a)
{
    return a->data->sounds[a->nextSound].frame;
}

static inline float toFrames_7EF8(float t)
{
    return t * 30.0f;
}

static inline sAvV4_7EF8 pos_7EF8(const sAvObj_7EF8* o)
{
    return o->pos;
}

extern "C" void func_002D7EF8_7EF8(sAvActive_7EF8* self) __asm__("func_002D7EF8");
extern "C" void func_002D7EF8_7EF8(sAvActive_7EF8* self)
{
    if (self->data == 0) {
        return;
    }
    bool alive = false;
    self->time += D_004A3A64;
    int i = 0;
    for (sAvNode_7EF8* n = self->nodes; n != 0; n = n->next, i++) {
        bool ok = func_002D7CA8_7EF8(n, self->time);
        alive = ok | alive;
        if (!ok) {
            self->objs[i] = 0;
        }
    }
    while (self->nextEvent < self->data->nEvents && self->data->events[self->nextEvent].time <= self->time) {
        if (self->data->events[self->nextEvent].obj->spawned == 0) {
            // NOTE: function-local static of the original (guard D_004A54C8), written out as its guarded in-place construction.
            if (D_004A54C8 == 0) {
                new (&D_004D5770) sAvFloatDef_7EF8();
                D_004A54C8 = 1;
            }
            func_00341AA0(cMemMan_alloc(0x6C, str_AnimObject_b, 0x20000000, 0), 1, 1, self->data->events[self->nextEvent].obj, &D_004D5770);
        }
        func_0029E560_7EF8(func_0028B180(), pos_7EF8(self->data->events[self->nextEvent].obj));
        self->nextEvent++;
    }
    while (self->nextSound < self->data->nSounds && (float)soundFrame_7EF8(self) <= toFrames_7EF8(self->time)) {
        sAvObj_7EF8* o = self->objs[self->data->sounds[self->nextSound].obj];
        if (o != 0) {
            func_0029E560_7EF8(func_0028B180(), o->sndPos);
        }
        self->nextSound++;
    }
    if (!alive) {
        func_002D81B0_7EF8(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D81B0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void* func_0028B180();
extern "C" void func_0029DEF0(void* self, int a1);

extern "C" void func_002D81B0(void* self)
{
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
        *(void**)((char*)self + 0x8) = 0;
    }
    if (*(int*)self != 0) {
        char* n = *(char**)((char*)self + 0x4);
        while (n != 0) {
            tActiveAvalancheNode_end(n, 1);
            n = *(char**)(n + 0x2E4);
        }
        *(int*)self = 0;
        func_0029DEF0(func_0028B180(), 0);
        char* p = *(char**)((char*)SSXApp->Game->AI + 0xA8);
        if (*(int*)(p + 0x708) != 0) {
            *(int*)(p + 0x708) = *(int*)(p + 0x708) - 1;
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
INCLUDE_ASM("visualfx/avalanche", func_002D82A0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void func_003DEC80(void* a, void* b, void* data, int size, int prio);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_buffer[];

extern "C" int func_002D82A0(int direct, void* a, void* b, char* src, int stride, int rows)
{
    if (direct) {
        func_003DEC80(a, b, src, stride * rows, 100);
    } else {
        char* buf = (char*)operator_new_tag(stride * rows, str_buffer, 0, 0);
        char* d = buf;
        for (int i = 0; i < rows; i++) {
            char* end = src + stride;
            char* p = end;
            while (src != p) {
                *d++ = *--p;
            }
            src = end;
        }
        func_003DEC80(a, b, buf, stride * rows, 100);
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
extern void* cAvalanche_m_avalancheList;
extern int D_004A3A48;
extern char str_pct_s_pct_s_j[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" char* cBENewRaceInterface_getLocationFile(void* iface, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_003DEBF0(char* name, int mode, int prio, int* fd);
extern "C" void func_003DECA0(int fd, int prio);
extern "C" int func_002D82A0(int direct, void* a, void* b, char* src, int stride, int rows);

struct sAvPart_83B8 {
    char pad_0x0[0xE0];
    float pos[3];                   // 0xE0
    char pad_0xEC[0xF4 - 0xEC];
    sAvPart_83B8* next;             // 0xF4
    char pad_0xF8[0x4];
    char* data;                     // 0xFC
    float count;                    // 0x100
};

struct sAvLink_83B8 {
    int id;                         // 0x0
    char* obj;                      // 0x4
};

struct sAvPair_83B8 {
    short a;
    short b;
};

struct sAvGroup_83B8 {
    int f0;
    sAvPart_83B8* parts;            // 0x4
    sAvGroup_83B8* next;            // 0x8
    unsigned short nlinks;          // 0xC
    unsigned short npairs;          // 0xE
    sAvLink_83B8 links[0x20];       // 0x10
    sAvPair_83B8 pairs[1];          // 0x110
};

/**
 * Writes the avalanche data of the current location to "<location file>.<name>".
 *
 * File layout: magic 0x2BEEF00 and the data size, then per avalanche, for each node 0xBEEF, the record size,
 * the start position and life x 10-byte packed frames, followed by the collision and ground-collision counts
 * and records (a collision stores its time and its instance's word at +0x78). Clears the save request
 * D_004A3A48 (On Tour m_Save).
 *
 * @param direct  byte-order flag for the writer func_002D82A0 (On Tour: littleEndian); the packed frames
 *                always pass 1
 * @param name  file extension
 */
// PORT: func_002D82A0 takes the file handle and offset as void* (ints passed through pointers).
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
    if (func_003DEBF0(buf, 0x26, 0x64, &fd)) {
        sAvGroup_83B8* avalanche;
        for (avalanche = (sAvGroup_83B8*)cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->next) {
            for (sAvPart_83B8* node = avalanche->parts; node != 0; node = node->next) {
                off += 0x10 + (int)node->count * 10;
            }
            off += 4;
            int k;
            for (k = 0; k < avalanche->nlinks; k++) {
                off += 8;
            }
            for (k = 0; k < avalanche->npairs; k++) {
                off += 4;
            }
        }
        int magic = 0x2BEEF00;
        func_002D82A0(direct, (void*)fd, 0, (char*)&magic, 4, 1);
        func_002D82A0(direct, (void*)fd, (void*)4, (char*)&off, 4, 1);
        off = 8;
        for (avalanche = (sAvGroup_83B8*)cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->next) {
            unsigned short header;
            for (sAvPart_83B8* node = avalanche->parts; node != 0; node = node->next) {
                header = 0xBEEF;
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
                header = (int)node->count * 10 + 0xC;
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)node->pos, 4, 3);
                off += func_002D82A0(1, (void*)fd, (void*)off, node->data, 10, (int)node->count);
            }
            header = avalanche->nlinks;
            off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
            header = avalanche->npairs;
            off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&header, 2, 1);
            int k;
            for (k = 0; k < avalanche->nlinks; k++) {
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&avalanche->links[k].id, 4, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, avalanche->links[k].obj + 0x78, 4, 1);
            }
            for (k = 0; k < avalanche->npairs; k++) {
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&avalanche->pairs[k].b, 2, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&avalanche->pairs[k].a, 2, 1);
            }
        }
        func_003DECA0(fd, 0x64);
    }
    D_004A3A48 = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_LoadData);
#ifdef SKIP_ASM
extern void* D_004A3AB4;
extern int cAvalanche_m_fileDataDeleteable;
extern int cAvalanche_m_resolved;
extern int D_004A3AC4;
extern int D_004A3A4C;
extern char str_sav[];
// PORT: the read call returns func_003DEB50's result (bytes read); the ealib unit declares it void.
extern "C" int func_003DEC60_i(int fd, int off, void* data, int size, int prio) __asm__("func_003DEC60");
extern "C" void cAvalanche_end(void);
extern "C" void cAvalanche_resolveDataPointers(void);

/**
 * Loads the current location's avalanche data file ("<location file>.sav") and resolves it.
 *
 * On a good magic (0x2BEEF00) the whole file is read into a buffer of the stored data size, but size + 8
 * bytes are read into it (header included), so the read runs 8 bytes past the allocation. With a bad magic
 * the file is not closed. Clears D_004A3A4C (On Tour m_Load) and resolves the data pointers.
 */
// Loads the avalanche data file for the current mountain (magic 0x2BEEF00, then the data size) into a fresh buffer.
extern "C" void cAvalanche_LoadData(void)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[0x100];
    sprintf(buf, str_pct_s_pct_s_j, cBENewRaceInterface_getLocationFile(iface, cBENewRaceInterface_getLocationID(iface)), str_sav);
    cAvalanche_m_resolved = 0;
    D_004A3AC4 = 1;
    int fd;
    if (func_003DEBF0(buf, 1, 0x64, &fd)) {
        cAvalanche_end();
        if (D_004A3AB4 != 0 && cAvalanche_m_fileDataDeleteable != 0) {
            cMemMan_free(D_004A3AB4);
        }
        int* hdr = (int*)operator_new_tag(8, buf, 0, 0);
        func_003DEC60_i(fd, 0, hdr, 8, 0x64);
        if (hdr[0] != 0x2BEEF00) {
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            D_004A3AB4 = 0;
            cAvalanche_m_fileDataDeleteable = 0;
        } else {
            int size = hdr[1];
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            void* data = operator_new_tag(size, buf, 0, 0);
            cAvalanche_m_fileDataDeleteable = 1;
            D_004A3AB4 = data;
            func_003DEC60_i(fd, 0, data, size + 8, 0x64);
            func_003DECA0(fd, 0x64);
            D_004A3AC4 = 0;
        }
    }
    D_004A3A4C = 0;
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
extern int D_004A3A30;
extern int D_004A3A34;
extern int D_004A3A5C;
extern int D_004A3A60;
extern int D_004A3A7C;
extern int cAvalanche_m_resolved;
extern int D_004A3AC4;
extern sAvalancheSlot cAvalanche_m_activeAvalanches[];
extern "C" void cAvalanche_calculate(void);
extern "C" void cAvalanche_resolveDataPointers(void);
extern "C" int cAvalanche_triggerAvalanche(int a);
extern "C" void func_002D7EF8(void* self);

/**
 * Runs the avalanche system for one frame while it is enabled.
 *
 * When enabled (D_004A3A30, On Tour m_Enable): recalculates everything when the recalc flag (D_004A3A34) is set
 * and clears it and D_004A3AC4; resolves the data pointers; while the trigger flag (D_004A3A5C) is set,
 * triggers avalanche D_004A3A7C (On Tour m_AvalancheId; a negative flag fires once); once pointers are resolved
 * and the update flag (D_004A3A60) is set, updates all 16 active avalanches (func_002D7EF8, On Tour
 * tActiveAvalanche::update), a negative update flag again counting as one-shot.
 */
extern "C" void cAvalanche_update(void)
{
    if (D_004A3A30 != 0) {
        if (D_004A3A34 != 0) {
            cAvalanche_calculate();
            D_004A3A34 = 0;
            D_004A3AC4 = 0;
        }
        cAvalanche_resolveDataPointers();
        if (D_004A3A5C != 0) {
            cAvalanche_triggerAvalanche(D_004A3A7C);
            if (D_004A3A5C < 0) {
                D_004A3A5C = 0;
            }
        }
        if (D_004A3A30 != 0 && cAvalanche_m_resolved != 0 && D_004A3A60 != 0) {
            int i;
            for (i = 0; i < 16; i++) {
                func_002D7EF8(&cAvalanche_m_activeAvalanches[i]);
            }
            if (D_004A3A60 < 0) {
                D_004A3A60 = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_renderLines);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sRow_8A00 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVec4_8A00 : sRow_8A00 {
    sVec4_8A00() {}
    sVec4_8A00(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

struct sVec3_8A00 {
    float x, y, z;
    sVec3_8A00() {}
    sVec3_8A00(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sVec3_8A00(const sVec3_8A00& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline float Sqrt_8A00(float x)
{
    float r;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline sVec3_8A00 Normalize_8A00(const sVec3_8A00& a)
{
    float len = Sqrt_8A00(a.x * a.x + a.y * a.y + a.z * a.z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return sVec3_8A00(a.x * inv, a.y * inv, a.z * inv);
    }
    return a;
}

struct sMat_8A00 {
    sRow_8A00 v[4];
} __attribute__((aligned(16)));

struct sRec_8A00 {
    signed char dx, dy, dz;
    unsigned char r, g, b;
    signed char ax, ay, az;
    unsigned char angle;
};

struct sObj_8A00 {
    char pad[0xE0];
    sRow_8A00 pos;              // 0xE0
    char padF0[0xC];
    sRec_8A00* recs;            // 0xFC
    float count;                // 0x100
};

struct sNode_8A00 {
    char pad[0x2E0];
    sObj_8A00* obj;             // 0x2E0
};

struct sOut_8A00 {
    float x, y, z, w;
    int r;
    int g;
    int b;
    int a;
    sRow_8A00 pos;
};


struct sVE_8A00 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int);
};


extern int D_004A3A58;
extern sOut_8A00 D_004D5B70_8A00[] __asm__("D_004D5B70");
extern sMat_8A00 D_004FF1A0_8A00 __asm__("t4mat_Identity");
extern sRow_8A00 D_004FF130_8A00 __asm__("D_004FF130");
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (matrix copy).
static inline void CopyMat_8A00(sMat_8A00* d, const sMat_8A00* s)
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
static inline void AddTo_8A00(sRow_8A00& a, const sRow_8A00& b)
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
static inline void MulMat_8A00(sMat_8A00* a, const sMat_8A00* b)
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
static inline sMat_8A00 operator*=(sMat_8A00& a, const sMat_8A00& b)
{
    MulMat_8A00(&a, &b);
}

// Rotate m about axis by angle, keeping its translation.
static inline void Rotate_8A00(sMat_8A00& m, const sRow_8A00& axis, float angle)
{
    sMat_8A00 old;
    float s;
    float c;
    CopyMat_8A00(&old, &m);
    sRow_8A00 trans = old.v[3];
    old.v[3] = D_004FF130_8A00;
    func_0031BE50(&s, &c, angle);
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
    m.v[3].x = 0.0f;
    m.v[3].y = 0.0f;
    m.v[3].z = 0.0f;
    m.v[3].w = 1.0f;
    m.v[3] = trans;
    m.v[0].w = 0.0f;
    m.v[1].w = 0.0f;
    m.v[2].w = 0.0f;
    m.v[0].x = tx * x + c;
    m.v[1].x = tx * y + sz;
    m.v[2].x = tx * z - sy;
    m.v[0].y = ty * x - sz;
    m.v[1].y = ty * y + c;
    m.v[2].y = ty * z + sx;
    m.v[0].z = tz * x + sy;
    m.v[1].z = tz * y - sx;
    m.v[2].z = tz * z + c;
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
 * Debug-draws an active node's recorded path when D_004A3A58 (On Tour m_RenderLines) is set.
 *
 * Walks the avalanche node's packed frames from its start (position deltas x 2, frame colours) into the
 * vertex array D_004D5B70 and draws them with the graphics manager's virtual at vtable offset 0x248. The
 * rotation it builds per frame is not used for the vertices.
 *
 * @param nodep  the tActiveAvalancheNode; nothing happens without an avalanche node
 */
// Draws the debris particles of an avalanche node.
extern "C" void tActiveAvalancheNode_renderLines(void* nodep)
{
    sNode_8A00* node = (sNode_8A00*)nodep;
    if (node->obj == 0) {
        return;
    }
    sMat_8A00 matrix;
    CopyMat_8A00(&matrix, &D_004FF1A0_8A00);
    PushRS_8A00(GrMan);
    SetBits_8A00(((int)(GrMan->m_pCurrentDrawState)->unk_00[1]), 0x7C, 2, 1);
    if (D_004A3A58 != 0) {
        int i = 0;
        sRow_8A00 pos = node->obj->pos;
        for (; i < (int)node->obj->count; i++) {
            sVec4_8A00 v(node->obj->recs[i].dx * 2.0f, node->obj->recs[i].dy * 2.0f, node->obj->recs[i].dz * 2.0f, 0.0f);
            AddTo_8A00(pos, v);
            sVec3_8A00 n = Normalize_8A00(sVec3_8A00(node->obj->recs[i].ax * 0.007874015718698502f, node->obj->recs[i].ay * 0.007874015718698502f, node->obj->recs[i].az * 0.007874015718698502f));
            v.x = n.x;
            v.y = n.y;
            v.z = n.z;
            v.w = 0.0f;
            Rotate_8A00(matrix, v, node->obj->recs[i].angle * 0.008213315159082413f);
            matrix.v[3] = pos;
            D_004D5B70_8A00[i].r = node->obj->recs[i].r;
            D_004D5B70_8A00[i].g = node->obj->recs[i].g;
            D_004D5B70_8A00[i].b = node->obj->recs[i].b;
            D_004D5B70_8A00[i].pos = pos;
        }
        ((sVE_8A00*)GrMan->mVtbl)[0x248 / 8].fn((char*)GrMan + ((sVE_8A00*)GrMan->mVtbl)[0x248 / 8].delta, (int)node->obj->count, D_004D5B70_8A00, 0);
    }
    (*(sDrawState**)&GrMan->m_pCurrentDrawState)--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void tActiveAvalancheNode_renderLines(void* node);
extern "C" void func_00371688(void* p, int a);
extern int D_004A3A54;
extern int D_004A3A58;

struct sAvVec4_D8EA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvVert_D8EA8 {
    sAvVec4_D8EA8 uv;           // 0x0
    int r, g, b, a;             // 0x10
    sAvVec4_D8EA8 pos;          // 0x20
};
extern sAvVert_D8EA8 D_vtx_D8EA8[] __asm__("D_004EDB70");

struct sAvPair_D8EA8 { float v; char* node; };
struct sAvGroup_D8EA8 {
    int f0;
    void* nodes;                // 0x4
    void* next;                 // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    sAvPair_D8EA8 pairs[32];    // 0x10
};

struct sAvActive_D8EA8 {
    char pad0[0xD0];
    char fx[0x2E0 - 0xD0];      // 0xD0
    char* data;                 // 0x2E0
    sAvActive_D8EA8* next;      // 0x2E4
};

struct sAvSlot_D8EA8 {
    sAvGroup_D8EA8* group;      // 0x0
    sAvActive_D8EA8* active;    // 0x4
    char pad8[0x18 - 0x8];
    float limit;                // 0x18
};

struct sRState_D8EA8 {
    int f0;
    unsigned int f4;
};

class cRCtx_D8EA8 {
public:
    char pad[0xE84];
    sRState_D8EA8* top;         // 0xE84
    char padE88[0x10D8 - 0xE88];
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
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void drawLines(int n, sAvVert_D8EA8* v, int f);
};

class cRiderPos_D8EA8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual sAvVec4_D8EA8* getPos();
};

/**
 * Renders one active avalanche: its emitters and, in debug mode, its paths and collision lines.
 *
 * For each active node: path lines for the first D_004A3A58 nodes, and func_00371688(emitter, 7) when
 * D_004A3A54 (On Tour m_RenderEmitters, default 1) is set and the node has an emitter. With D_004A3A58 set it
 * also draws a line from each collision instance to the player's rider, red once the collision time has
 * passed and green before.
 *
 * @param slotv  the tActiveAvalanche slot; nothing happens when it has no avalanche
 */
extern "C" void tActiveAvalanche_render(void* slotv)
{
    sAvSlot_D8EA8* slot = (sAvSlot_D8EA8*)slotv;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (slot->group == 0)
        return;
    int i = 0;
    for (sAvActive_D8EA8* n = slot->active; n != 0; n = n->next, i++) {
        if (n->data) {
            if (i < D_004A3A58)
                tActiveAvalancheNode_renderLines(n);
            if (D_004A3A54 && ((unsigned char*)n->data)[0xF2])
                func_00371688(n->fx, 7);
        }
    }
    if (D_004A3A58 == 0)
        return;
    for (int j = 0; j < slot->group->nPairs; j++) {
        sAvVec4_D8EA8 nodePos = *(sAvVec4_D8EA8*)(slot->group->pairs[j].node + 0x40);
        D_vtx_D8EA8[2 * j].pos = nodePos;
        D_vtx_D8EA8[2 * j].r = slot->group->pairs[j].v < slot->limit ? 0xFF : 0;
        D_vtx_D8EA8[2 * j].g = slot->group->pairs[j].v >= slot->limit ? 0xFF : 0;
        char* rider = *(char**)((char*)SSXApp->Game->AI + 0x28);
        D_vtx_D8EA8[2 * j + 1].pos = *((cRiderPos_D8EA8*)(rider + 0x6C0))->getPos();
        D_vtx_D8EA8[2 * j + 1].r = slot->group->pairs[j].v < slot->limit ? 0xFF : 0;
        D_vtx_D8EA8[2 * j + 1].g = slot->group->pairs[j].v >= slot->limit ? 0xFF : 0;
    }
    sRState_D8EA8* top = ((cRCtx_D8EA8*)ctx)->top;
    unsigned int w = top->f4;
    top->f4 = (w & 0xFE7FFFFF) | 0x1000000;
    int saved = (w & 0x1800000) >> 23;
    ((cRCtx_D8EA8*)ctx)->drawLines(slot->group->nPairs, D_vtx_D8EA8, 0);
    ((cRCtx_D8EA8*)ctx)->top->f4 = (((cRCtx_D8EA8*)ctx)->top->f4 & 0xFE7FFFFF) | ((saved << 23) & 0x1800000);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_render);
#ifdef SKIP_ASM
struct sRState_9130 {
    int f0;
    int f4;
    unsigned b0 : 5;
    unsigned b5 : 5;
    unsigned b10 : 22;
    int fC;
    short f10;
    short f12;
};
struct sRCtx_9130 {
    char pad[0xE84];
    sRState_9130* top;          // 0xE84
    char padE88[0x10D8 - 0xE88];
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
    virtual void setMatrix(void* m, int a);
    virtual void setPos(float x, float y, float z);
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void setRot(void* r);
};
struct sCam_9130 {
    int f0;
    float* views[4];            // 0x4
    int cur;                    // 0x14
    float mats[4][4];           // 0x18
};
extern int D_004A41EC;
extern sRState_9130 D_00501420;
extern "C" void tActiveAvalanche_render(void* slot);

static inline sCam_9130* getCam_9130() { return (sCam_9130*)SSXApp->Game->ViewMan; }

static inline void setB5_9130(sRCtx_9130* c, int v) { c->top->b5 = v; }

/**
 * Renders all active avalanches with the current camera's view.
 *
 * Runs when the system is enabled (D_004A3A30) and resolved, or when D_004A41EC is set: pushes a draw state
 * set up for the camera's matrix, position and rotation and, if enabled, renders the 16 active slots.
 */
extern "C" void cAvalanche_render(void)
{
    if ((D_004A3A30 != 0 && cAvalanche_m_resolved != 0) || D_004A41EC != 0)
    {
        cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
        float* view = getCam_9130()->views[getCam_9130()->cur];
        ((sRCtx_9130*)ctx)->top[1] = ((sRCtx_9130*)ctx)->top[0];
        ((sRCtx_9130*)ctx)->top++;
        *((sRCtx_9130*)ctx)->top = D_00501420;
        setB5_9130((sRCtx_9130*)ctx, 7);
        *(short*)((char*)((sRCtx_9130*)ctx)->top + 0x10) = -1;
        ((sRCtx_9130*)ctx)->setMatrix(getCam_9130()->mats[getCam_9130()->cur], 1);
        ((sRCtx_9130*)ctx)->setPos(view[0], view[1], view[2]);
        ((sRCtx_9130*)ctx)->setRot(view + 0x10);
        if (D_004A3A30 != 0)
        {
            for (int i = 0; i < 16; i++)
                tActiveAvalanche_render(&cAvalanche_m_activeAvalanches[i]);
        }
        ((sRCtx_9130*)ctx)->top--;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_Avalanche[];
extern char str_AvalancheNode[];
extern void* cAvalanche_m_avalancheList;
extern char D_00538450[];
extern int D_004A3AC0;
extern int cAvalanche_m_resolved;

struct sAvNodeD92D0 {
    char pad0[0xF0];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNodeD92D0* next;         // 0xF4
    char* obj;                  // 0xF8
    char* data;                 // 0xFC
    float count;                // 0x100
    float sy;                   // 0x104
    float sz;                   // 0x108
    float rate;                 // 0x10C
    float radius;               // 0x110
    int f114;                   // 0x114
    char pad118[0x120 - 0x118];
};
struct sAvGroupD92D0 {
    int id;                     // 0x0
    sAvNodeD92D0* nodes;        // 0x4
    sAvGroupD92D0* next;        // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    char pad10[0x210 - 0x10];
    // tAvalanche::init(int id) (On Tour declares it; body inlined here). The caller links the new
    // record in afterwards, overwriting m_next.
    void init(int inId)
    {
        id = inId;
        next = 0;
        nPairs = 0;
        nIdx = 0;
        nodes = 0;
    }
};

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
 * x the instance's smallest bound extent and a base type (5 gives 2, 2 gives 0, else 1). It is registered in
 * the node table D_00538450 and its index is written into the instance's flags at +0x92 ((index << 8) | 2).
 *
 * @param id  avalanche id
 * @param obj  the world instance that falls
 * @param type  the instance's kind, mapped to the node's base type
 * @param life  life in seconds
 * @param fadeIn  fade-in time in seconds
 * @param fadeOut  fade-out time in seconds
 * @param nodeSpeed  speed (scales the emitter time step)
 */
extern "C" void cAvalanche_addAvalancheNode(int id, char* obj, int type, float life, float fadeIn, float fadeOut, float nodeSpeed)
{
    sAvGroupD92D0* avalanche = (sAvGroupD92D0*)cAvalanche_m_avalancheList;
    while (avalanche != 0 && avalanche->id != id)
        avalanche = avalanche->next;
    if (avalanche == 0) {
        avalanche = new (str_Avalanche, 0, 0) sAvGroupD92D0;
        avalanche->init(id);
        avalanche->next = (sAvGroupD92D0*)cAvalanche_m_avalancheList;
        cAvalanche_m_avalancheList = avalanche;
    }
    sAvNodeD92D0* n = new (str_AvalancheNode, 0, 0) sAvNodeD92D0;
    n->next = avalanche->nodes;
    avalanche->nodes = n;
    n->rate = nodeSpeed;
    n->count = life * 30.0f;
    n->sy = fadeIn * 30.0f;
    n->sz = fadeOut * 30.0f;
    n->obj = obj;
    // The target materialises &n->radius (addiu v1,a0,0x110; swc1 0(v1)): the source bound the field by reference.
    float& radius = n->radius;
    radius = AV_MIN_D92D0(AV_MIN_D92D0(Abs_D92D0(*(float*)(obj + 0x6C) - *(float*)(obj + 0x60)),
                                         Abs_D92D0(*(float*)(obj + 0x70) - *(float*)(obj + 0x64))),
                             Abs_D92D0(*(float*)(obj + 0x74) - *(float*)(obj + 0x68))) *
                0.20000000298023224f;
    n->data = 0;
    n->owned = 0;
    switch (type) {
    case 5:
        n->type = 2;
        break;
    case 2:
        n->type = 0;
        break;
    default:
        n->type = 1;
        break;
    }
    n->emit = 0;
    n->f114 = 0;
    int idx = D_004A3AC0;
    ((sAvNodeD92D0**)D_00538450)[idx] = n;
    *(unsigned short*)(n->obj + 0x92) = *(unsigned char*)(n->obj + 0x92);
    *(unsigned short*)(n->obj + 0x92) |= (idx << 8) | 2;
    D_004A3AC0 = idx + 1;
    cAvalanche_m_resolved = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9538);
#ifdef SKIP_ASM
struct sAvParams9538 { int data[0xD8 / 4]; };
struct sAvNode9538 {
    sAvParams9538 params;       // 0x0
    char padD8[0xF0 - 0xD8];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    char padF3;
    sAvNode9538* next;          // 0xF4
    int id;                     // 0xF8
};
struct sAvGroup9538 {
    int f0;
    sAvNode9538* nodes;         // 0x4
    sAvGroup9538* next;         // 0x8
};
extern void* cAvalanche_m_avalancheList;

extern "C" void func_002D9538(int id, sAvParams9538* params)
{
    for (sAvGroup9538* g = (sAvGroup9538*)cAvalanche_m_avalancheList; g != 0; g = g->next) {
        for (sAvNode9538* n = g->nodes; n != 0; n = n->next) {
            if (n->id == id) {
                if (n->type == 2) return;
                n->emit = 1;
                n->params = *params;
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
extern "C" void tAvalanche_calculate(void* self)
{
    void* n = *(void**)((char*)self + 0x4);
    while (n != 0) {
        tAvalancheNode_calculate(n, self);
        n = *(void**)((char*)n + 0xF4);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D96B0);
#ifdef SKIP_ASM
extern "C" void* func_002D96B0(void* self, int id)
{
    for (void* n = *(void**)((char*)self + 0x4); n != 0; n = *(void**)((char*)n + 0xF4)) {
        if (*(int*)((char*)n + 0xF8) == id) {
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
extern void* cAvalanche_m_avalancheList;

/**
 * Recalculates every avalanche, then saves and reloads the data file.
 *
 * Ends the active avalanches, runs tAvalanche_calculate on each, then cAvalanche_SaveData and
 * cAvalanche_LoadData.
 */
extern "C" void cAvalanche_calculate(void)
{
    cAvalanche_end();
    for (void* avalanche = cAvalanche_m_avalancheList; avalanche != 0; avalanche = *(void**)((char*)avalanche + 0x8)) {
        tAvalanche_calculate(avalanche);
    }
    cAvalanche_SaveData();
    cAvalanche_LoadData();
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9738);
#ifdef SKIP_ASM
struct sAvNode_9738 { char pad[0xF4]; sAvNode_9738* next; };
struct sAvEnt_9738 { short a; short depth; };
struct sAv_9738 {
    int f0;
    sAvNode_9738* head;      // 0x4
    char pad8[6];
    unsigned short count;    // 0xE
    char pad10[0x100];
    sAvEnt_9738 ents[64];    // 0x110
};
// Records (a, depth of stop in the node list) while there is room for it.
extern "C" void func_002D9738(sAv_9738* self, int a, sAvNode_9738* stop)
{
    if (self->count < 64) {
        int depth = 0;
        sAvNode_9738* p = self->head;
        while (p != 0 && p != stop) {
            depth++;
            p = p->next;
        }
        self->ents[self->count].a = a;
        self->ents[self->count].depth = depth;
        self->count++;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);
#ifdef SKIP_ASM
extern "C" float cRenderStateMan_SnowFlakeColourR(int a);
extern "C" float cRenderStateMan_SnowFlakeColourG(int a);
extern "C" float cRenderStateMan_SnowFlakeColourB(int a);
extern "C" void func_00370DC8(void* self, void* node, float k);
extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t);

extern "C" void func_0029DEF0(void* self, int a1);
extern void* cAvalanche_m_avalancheList;

struct sVec_D97A8 {
    float x, y, z, w;
    sVec_D97A8() {}
    sVec_D97A8(const float& a, const float& b, const float& c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sMat_D97A8 {
    sVec_D97A8 row[4];
};
extern sMat_D97A8 D_004FF1A0_D97A8 __asm__("t4mat_Identity");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMatrix_D97A8(sMat_D97A8* dst, sMat_D97A8* src)
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

struct sAvAct_D97A8;
struct sAvNode_D97A8 {
    char pad0[0xE0];
    sVec_D97A8 pos;             // 0xE0
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNode_D97A8* next;        // 0xF4
    char* obj;                  // 0xF8
    char padFC[0x114 - 0xFC];
    sAvAct_D97A8* act;          // 0x114
};

struct sAvGroup_D97A8 {
    int id;                     // 0x0
    sAvNode_D97A8* nodes;       // 0x4
    sAvGroup_D97A8* next;       // 0x8
};

struct sAvAct_D97A8 {
    int f0;                     // 0x0
    char pad4[0x10 - 0x4];
    sVec_D97A8 pos;             // 0x10
    sMat_D97A8 m20;             // 0x20
    sVec_D97A8 objPos;          // 0x60
    sMat_D97A8 m70;             // 0x70
    char padB0[0xC0 - 0xB0];
    sVec_D97A8 scale;           // 0xC0
    char emitter[0x1E0 - 0xD0]; // 0xD0
    sVec_D97A8 colour;          // 0x1E0
    char pad1F0[0x2D0 - 0x1F0];
    int f2D0;                   // 0x2D0
    char pad2D4[0x2E0 - 0x2D4];
    sAvNode_D97A8* data;        // 0x2E0
    sAvAct_D97A8* next;         // 0x2E4
    int pad2E8;
    int f2EC;                   // 0x2EC
};
extern sAvAct_D97A8 D_act_D97A8[] __asm__("D_004EE770");

struct sAvSlot_D97A8 {
    sAvGroup_D97A8* group;      // 0x0
    sAvAct_D97A8* active;       // 0x4
    void* arr;                  // 0x8
    int fC;
    int f10;                    // 0x10
    int f14;                    // 0x14
    float time;                 // 0x18
};
extern sAvSlot_D97A8 D_slots_D97A8[] __asm__("cAvalanche_m_activeAvalanches");

/**
 * Starts the avalanche with the given id in a free active slot.
 *
 * Gives each of its nodes a free active node from the 64-entry pool (position, rotation, scale, and for nodes
 * with an emitter a reset emitter in the snow-flake colour), builds the node array, calls func_0029DEF0(sound
 * manager, 1) and counts the avalanche in the danger manager (+0x708).
 *
 * @param id  avalanche id
 * @return the slot index (0-15), or 0x80000000 when the id is unknown, already active, or no slot or node is
 *         free
 */
extern "C" int cAvalanche_triggerAvalanche(int id)
{
    cAvalanche_resolveDataPointers();
    for (sAvGroup_D97A8* avalanche = (sAvGroup_D97A8*)cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->next) {
        if (avalanche->id != id)
            continue;
        for (int j = 0; j < 16; j++) {
            if (D_slots_D97A8[j].group == avalanche)
                return (int)0x80000000;
        }
        sAvSlot_D97A8* slot = D_slots_D97A8;
        for (int i = 0; i < 16; i++, slot++) {
            if (slot->group != 0)
                continue;
            slot->group = avalanche;
            slot->active = 0;
            slot->time = 0.0f;
            slot->f10 = 0;
            slot->f14 = 0;
            sAvAct_D97A8* act = D_act_D97A8;
            int k = 0;
            for (sAvNode_D97A8* n = avalanche->nodes; n != 0; n = n->next) {
                for (; k < 64; k++, act++) {
                    if (act->data == 0) {
                        act->data = n;
                        n->act = act;
                        act->next = slot->active;
                        slot->active = act;
                        act->f0 = 0;
                        act->pos = n->pos;
                        CopyMatrix_D97A8(&act->m20, &D_004FF1A0_D97A8);
                        act->f2EC = 0;
                        {
                            sVec_D97A8 objPosition = *(sVec_D97A8*)(n->obj + 0x40);
                            act->objPos = objPosition;
                        }
                        CopyMatrix_D97A8(&act->m70, &D_004FF1A0_D97A8);
                        act->scale = sVec_D97A8(1.0f, 1.0f, 1.0f, 1.0f);
                        tActiveAvalancheNode_calculateScale(act, 0.0f);
                        if (n->emit) {
                            cDynamicColourEmitter_reset(act->emitter);
                            func_00370DC8(act->emitter, n, 2.0f);
                            act->f2D0 = 1;
                            float flakeRed = cRenderStateMan_SnowFlakeColourR(0);
                            float gg = cRenderStateMan_SnowFlakeColourG(0);
                            float flakeBlue = cRenderStateMan_SnowFlakeColourB(0);
                            act->colour = sVec_D97A8(flakeRed, gg, flakeBlue, 0.0f);
                        }
                        break;
                    }
                }
                if (k == 64) {
                    slot->group = 0;
                    for (act = slot->active; act != 0; act = act->next) {
                        if (act->data)
                            act->data->act = 0;
                        act->data = 0;
                    }
                    return (int)0x80000000;
                }
            }
            tActiveAvalanche_buildArray((tActiveAvalanche*)slot);
            func_0029DEF0(func_0028B180(), 1);
            (*(int*)(*(char**)((char*)SSXApp->Game->AI + 0xA8) + 0x708))++;
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
struct cAvalanche_end_sSlot {
    int active;
    char pad[0x18];
};
// Typed view of cAvalanche_m_activeAvalanches (cAvalanche_update declares it earlier in the unit with its own element type).
extern cAvalanche_end_sSlot D_slots_9A80[] __asm__("cAvalanche_m_activeAvalanches");
extern int cAvalanche_m_fileDataDeleteable;
void cMemMan_free(void*);
extern "C" void func_002D81B0(void* self);

static inline void freeSlot_9A80(cAvalanche_end_sSlot* s)
{
    if (s->active != 0) {
        func_002D81B0(s);
    }
}

/**
 * Stops all active avalanches and, for loaded data, drops each node's owned frame history.
 */
extern "C" void cAvalanche_end(void)
{
    int i;
    for (i = 0; i < 16; i++) {
        freeSlot_9A80(&D_slots_9A80[i]);
    }
    if (cAvalanche_m_fileDataDeleteable != 0) {
        for (char* avalanche = (char*)cAvalanche_m_avalancheList; avalanche != 0; avalanche = *(char**)(avalanche + 0x8)) {
            for (char* n = *(char**)(avalanche + 0x4); n != 0; n = *(char**)(n + 0xF4)) {
                if (*(unsigned char*)(n + 0xF3) != 0 && *(void**)(n + 0xFC) != 0) {
                    cMemMan_free(*(void**)(n + 0xFC));
                }
                *(unsigned char*)(n + 0xF3) = 0;
                *(void**)(n + 0xFC) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_reset);
#ifdef SKIP_ASM
extern "C" void cAvalanche_end(void);
extern "C" void func_00416210(void* dst, int c, int n);
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char D_00538450[];
extern int D_004A3AC0;
extern void* D_004A3AB4;
extern int cAvalanche_m_fileDataDeleteable;
extern int cAvalanche_m_resolved;

/**
 * Deletes all avalanches and nodes and frees the loaded avalanche data.
 *
 * Also clears the node table and its count and the resolved flag. self is unused (On Tour: a static method).
 */
extern "C" void cAvalanche_reset(void* self)
{
    cAvalanche_end();
    char* avalanche = (char*)cAvalanche_m_avalancheList;
    while (avalanche != 0) {
        char* cur = avalanche;
        char* n = *(char**)(avalanche + 0x4);
        while (n != 0) {
            char* next = *(char**)(n + 0xF4);
            operator_delete((int*)n);
            n = next;
        }
        avalanche = *(char**)(avalanche + 0x8);
        operator_delete((int*)cur);
    }
    cAvalanche_m_avalancheList = 0;
    D_004A3AC0 = 0;
    func_00416210(D_00538450, 0, 0x400);
    if (D_004A3AB4 != 0 && cAvalanche_m_fileDataDeleteable != 0) {
        cMemMan_free(D_004A3AB4);
        D_004A3AB4 = 0;
        cAvalanche_m_fileDataDeleteable = 0;
    }
    cAvalanche_m_resolved = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9C00);
#ifdef SKIP_ASM
struct sAvVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvMatrix {
    sAvVec4 row[4];
};

struct sAvNode {
    char pad[0x60];
    sAvVec4 pos;            // 0x60
    sAvMatrix rot;          // 0x70
    float scale;            // 0xB0
    char padB4[0x2E0 - 0xB4];
    void* data;             // 0x2E0
    char pad2E4[0x2F0 - 0x2E4];
};

extern sAvNode D_004EE770[64];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixAV(sAvMatrix* dst, sAvMatrix* src)
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
static inline void vu0ScaleMatrixAV(sAvMatrix* dst, sAvMatrix* src, float s)
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

extern "C" void func_002D9C00(int id, sAvMatrix* out)
{
    int i;
    for (i = 0; i < 64; i++) {
        sAvNode* node = &D_004EE770[i];
        if (node->data != 0 && *(int*)((char*)node->data + 0xF8) == id) {
            vu0CopyMatrixAV(out, &node->rot);
            vu0ScaleMatrixAV(out, out, node->scale);
            out->row[3] = node->pos;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9CB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002D9CB0_sSlot {
    int* av;
    int pad4[5];
    float t;
    int isActive() { return av != 0; }
};
extern func_002D9CB0_sSlot D_00538938_9CB0[16] __asm__("cAvalanche_m_activeAvalanches");

struct func_002D9CB0_sSave {
    int ids[16];
    float times[16];
};

class func_002D9CB0_cStream {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* data, int size);
};

extern "C" void func_002D9CB0(func_002D9CB0_cStream* stream)
{
    func_002D9CB0_sSave state;
    for (int i = 0; i < 16; i++) {
        state.ids[i] = D_00538938_9CB0[i].isActive() ? *D_00538938_9CB0[i].av : -1;
        state.times[i] = D_00538938_9CB0[i].isActive() ? D_00538938_9CB0[i].t : 0.0f;
    }
    stream->v01(&state, 0x80);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVEntry_RR {
    short delta;
    short index;
    void (*fn)(void*, int*, int);
};
struct sFrame_RR {
    char b[96];
};
struct sActRR {
    char pad_0x0[0x2E0];
    char* data;                     // 0x2E0
    sActRR* next;                   // 0x2E4
    int pad_0x2E8;
    int f2EC;                       // 0x2EC
};
struct sKeyRR { float t; int v; };
struct sKey2RR { unsigned short t; unsigned short v; };
struct sDataRR {
    char pad_0x0[0xC];
    unsigned short n0;              // 0xC
    unsigned short n1;              // 0xE
    sKeyRR a[32];                   // 0x10
    sKey2RR b[1];                   // 0x110
};
struct sSlotRR {
    sDataRR* group;                    // 0x0
    sActRR* active;                 // 0x4
    int pad_0x8;
    int pad_0xC;
    int cur0;                       // 0x10
    int cur1;                       // 0x14
    float time;                     // 0x18
};
extern sSlotRR D_slots_RR[] __asm__("cAvalanche_m_activeAvalanches");
extern "C" int cAvalanche_triggerAvalanche(int id);
extern "C" void func_002D81B0(void* slot);
extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t);
sFrame_RR getFrameData_RR(void* self, float t) __asm__("tActiveAvalancheNode_getFrameData");

static inline sSlotRR* slots_RR()
{
    return D_slots_RR;
}

static inline void clearSlot_RR(sSlotRR* s)
{
    if (s->group != 0) {
        func_002D81B0(s);
    }
}

/**
 * Restores the active avalanches from a replay frame.
 *
 * self is the replay stream, not a cAvalanche: it reads 16 avalanche ids and 16 times (0x80 bytes) through
 * the stream's virtual slot 2. Each id of 0 or more is re-triggered and wound to its time: node scales and
 * frames are recomputed (emitter nodes re-emit) and the slot's collision cursors (+0x10, +0x14) skip events
 * already past.
 */
extern "C" void cAvalanche_readFromReplayFrame(int* self)
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
        sVEntry_RR* vtEntry = &(*(sVEntry_RR**)self)[2];
        vtEntry->fn((char*)self + vtEntry->delta, buf.ids, 0x80);
    }
    for (i = 0; i < 16; i++) {
        if (buf.ids[i] >= 0) {
            int n = cAvalanche_triggerAvalanche(buf.ids[i]);
            if (n >= 0) {
                sSlotRR* slot = &slots_RR()[n];
                slot->time = buf.times[i];
                if (buf.times[i] != 0.0f) {
                    sActRR* activeNode = slot->active;
                    while (activeNode != 0) {
                        if (*(unsigned char*)(activeNode->data + 0xF2) != 0) {
                            activeNode->f2EC = 1;
                        }
                        tActiveAvalancheNode_calculateScale(activeNode, buf.times[i]);
                        getFrameData_RR(activeNode, slot->time);
                        activeNode = activeNode->next;
                    }
                    while (slot->cur0 < slot->group->n0 && slot->group->a[slot->cur0].t <= slot->time) {
                        slot->cur0++;
                    }
                    while (slot->cur1 < slot->group->n1 && (float)slot->group->b[slot->cur1].t <= slot->time) {
                        slot->cur1++;
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9FB8);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cAvalanche_resolveDataPointers(void);
extern void* D_004A3AB4;
extern int cAvalanche_m_fileDataDeleteable;
extern int D_004A3AC4;

extern "C" void func_002D9FB8(void* data)
{
    cAvalanche_reset(data);
    if (D_004A3AB4 != 0 && cAvalanche_m_fileDataDeleteable != 0) {
        cMemMan_free(D_004A3AB4);
    }
    D_004A3AB4 = data;
    cAvalanche_m_fileDataDeleteable = 0;
    if (*(int*)data != 0x2BEEF00) {
        D_004A3AB4 = 0;
        D_004A3AC4 = 1;
    } else {
        D_004A3AC4 = 0;
        cAvalanche_resolveDataPointers();
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern void* cAvalanche_m_avalancheList;
extern void* D_004A3AB4;
extern int cAvalanche_m_resolved;

struct sAvVec4_A028 {
    float x, y, z, w;
    sAvVec4_A028() {}
    sAvVec4_A028(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sAvNodeA028 {
    char pad0[0xE0];
    sAvVec4_A028 pos;           // 0xE0
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNodeA028* next;          // 0xF4
    int id;                     // 0xF8
    char* data;                 // 0xFC
    float count;                // 0x100
};
struct sAvPairA028 { float v; int f4; };
struct sAvIdxA028 { unsigned short a; unsigned short b; };
struct sAvGroupA028 {
    int f0;
    sAvNodeA028* nodes;         // 0x4
    sAvGroupA028* next;         // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    sAvPairA028 pairs[32];      // 0x10
    sAvIdxA028 idx[1];          // 0x110
};

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
    if (D_004A3AB4 == 0)
        return;
    if (cAvalanche_m_resolved != 0)
        return;
    char* cursor = (char*)D_004A3AB4 + 8;
    for (sAvGroupA028* avalanche = (sAvGroupA028*)cAvalanche_m_avalancheList; avalanche != 0; avalanche = avalanche->next) {
        for (sAvNodeA028* n = avalanche->nodes; n != 0; n = n->next) {
            cursor += 4;
            float* start = (float*)cursor;
            n->pos = sAvVec4_A028(start[0], start[1], start[2], 1.0f);
            cursor += 12;
            if (n->owned && n->data)
                cMemMan_free(n->data);
            n->data = cursor;
            n->owned = 0;
            cursor += (int)n->count * 10;
        }
        avalanche->nPairs = *(unsigned short*)cursor;
        cursor += 2;
        avalanche->nIdx = *(unsigned short*)cursor;
        cursor += 2;
        for (int i = 0; i < avalanche->nPairs; i++) {
            avalanche->pairs[i].v = *(float*)cursor;
            avalanche->pairs[i].f4 = 0;
            cursor += 8;
        }
        for (int j = 0; j < avalanche->nIdx; j++) {
            avalanche->idx[j].b = *(unsigned short*)cursor;
            cursor += 2;
            avalanche->idx[j].a = *(unsigned short*)cursor;
            cursor += 2;
        }
        avalanche->nPairs = 0;
    }
    cAvalanche_m_resolved = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002DA1C0);
#ifdef SKIP_ASM
struct sVec4_A1C0 {
    float x, y, z, w;
    sVec4_A1C0() {}
    sVec4_A1C0(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sVec3_A1C0 { float x, y, z; };
struct sAvInfo_A1C0 {
    sVec4_A1C0 center;          // 0x0
    float total;                // 0x10
    float minDist;              // 0x14
    int count;                  // 0x18
    bool any;                   // 0x1C
};
struct sAvNode_A1C0 {
    char pad0[0x60];
    sVec4_A1C0 pos;             // 0x60
    char pad70[0xB0 - 0x70];
    float weight;               // 0xB0
    char padB4[0x2E0 - 0xB4];
    char* src;                  // 0x2E0
    sAvNode_A1C0* next;         // 0x2E4
};
extern sVec4_A1C0 t4vect_Zero;

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_A1C0 vSub_A1C0(const sVec4_A1C0& a, const sVec4_A1C0& b)
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
static inline float vLen_A1C0(const sVec4_A1C0& v)
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
static inline void vAddEq_A1C0(sVec4_A1C0& dst, const sVec4_A1C0& b)
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
static inline void vDivEq_A1C0(sVec4_A1C0& v, float s)
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

// PORT: g++ `<?` (min) operator.
extern "C" sAvInfo_A1C0 func_002DA1C0(sVec3_A1C0 p)
{
    sAvInfo_A1C0 r;
    r.center = t4vect_Zero;
    r.minDist = 999999.0f;
    r.count = 0;
    r.total = 0.0f;
    r.any = 0;
    for (int i = 0; i < 16; i++) {
        sAvalancheSlot* s = &cAvalanche_m_activeAvalanches[i];
        if (s->active == 0)
            continue;
        for (sAvNode_A1C0* n = *(sAvNode_A1C0**)s->pad; n != 0; n = n->next) {
            if (n->src == 0)
                continue;
            sVec4_A1C0 d = vSub_A1C0(sVec4_A1C0(p.x, p.y, p.z, 1.0f), n->pos);
            float dist = vLen_A1C0(d);
            if (dist < 225000000.0f) {
                vAddEq_A1C0(r.center, n->pos);
                r.total += n->weight;
                r.minDist = r.minDist <? dist;
                r.any |= *(unsigned short*)(n->src + 0xF0) == 2;
                r.count++;
            }
        }
    }
    vDivEq_A1C0(r.center, (float)r.count);
    r.total = r.total / (float)r.count;
    return r;
}
#endif

