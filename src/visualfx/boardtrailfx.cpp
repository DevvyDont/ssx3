#include "common.h"
#include "animation/animsequencer.h"
#include "ai/rider.h"
#include "vtable.h"
#include "bx/math3d.h"
#include "render/2drender.h"
#include "bx/appman.h"
#include "ui/uithing.h"
#include "visualfx/transitionobj.h"
#include "visualfx/cutscenetransitionman.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_initialize);
#ifdef SKIP_ASM
extern "C" void* cRider_cRider(void* self);
extern "C" void MEM_fill(void* p, int c, int n);
struct cBoardTrailFX_reset_sTrail;
extern "C" void cBoardTrailFX_reset(cBoardTrailFX_reset_sTrail* self);
extern char D_00459B90[];
extern char str_BoardTrailVertices[];

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sBTVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sBTVt9 {
    sBTVtEnt e[9];
} __attribute__((aligned(8)));
struct sBTVt22 {
    sBTVtEnt e[22];
} __attribute__((aligned(8)));

extern const sBTVt9 D_00488BB8;
extern const sBTVt22 D_00488C00;

struct sBTTrailVtx {
    float v[12];
    sBTTrailVtx() {}
};

struct sBoardTrailFXView {  // this unit's view of cBoardTrailFX (ai/riderhandler.h, where the FX parts are not yet mapped)
    char* vbase;                // 0x00
    int f04;                    // 0x04
    int f08;                    // 0x08
    int fC;                     // 0x0C
    sBTTrailVtx* strip[6];      // 0x10
    int f28;                    // 0x28
    short ids[48];              // 0x2C
};

/**
 * Constructs a board trail effect; despite its name, this is cBoardTrailFX's constructor.
 *
 * As g++ 2.95 does for a class with a virtual base (cRider at +0xF0): with inChrg it also builds the cRider
 * base, and it stores the three rider vtables (on stack copies with fixed deltas when not in charge). Then
 * clears 48 ids to -1, allocates the 324 trail vertices ("BoardTrailVertices", 6 strips of 54), zeroes them
 * and resets the trail.
 *
 * @param inChrg  g++ in-charge flag: nonzero when this is the most-derived object and must build cRider
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xF0): vtable copies with delta fixups when not in charge.
extern "C" void* cBoardTrailFX_initialize(sBoardTrailFXView* self, int inChrg)
{
    if (inChrg) {
        self->vbase = (char*)self + 0xF0;
        cRider_cRider((char*)self + 0xF0);
    }
    *(const void**)(self->vbase + 0x6E8) = &D_00488BB8;
    *(void**)(self->vbase + 0x6D0) = D_00459B90;
    *(const void**)(self->vbase + 0x6C0) = &D_00488C00;
    if (inChrg == 0) {
        int vc;
        sBTVt9 vtbl1 = D_00488BB8;
        *(void**)(self->vbase + 0x6E8) = &vtbl1;
        {
            char* vbo = self->vbase - 0xF0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488BB8.e[1].delta + vc;
        sBTVt22 vtbl2 = D_00488C00;
        *(void**)(self->vbase + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_00488C00.e[1].delta + vc;
    }
    self->f28 = 0;
    short neg = -1;
    int i;
    for (i = 47; i >= 0; i--) {
        self->ids[i] = neg;
    }
    sBTTrailVtx** strips = self->strip;
    strips[0] = new (str_BoardTrailVertices, 0, 0) sBTTrailVtx[324];
    for (i = 1; i < 6; i++) {
        strips[i] = self->strip[0] + i * 54;
    }
    self->fC = 0;
    MEM_fill(self->strip[0], 0, 15552);
    cBoardTrailFX_reset((cBoardTrailFX_reset_sTrail*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX__cBoardTrailFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardtrailfx_classes.h"

/**
 * Destroys a board trail effect, freeing its trail vertex buffer (+0x10).
 *
 * g++ adds the virtual-base vtable fix-ups and the delete.
 */
cVbDtor002E83F0::~cVbDtor002E83F0()
{
    if (m10 != 0) cMemMan_free(m10);
}

// PORT: the compiler names this dtor _$_15cVbDtor002E83F0; expose it under the address-derived name.
__asm__(".globl cBoardTrailFX__cBoardTrailFX\n.type cBoardTrailFX__cBoardTrailFX,@function\ncBoardTrailFX__cBoardTrailFX = _$_15cVbDtor002E83F0\n.size cBoardTrailFX__cBoardTrailFX,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002E83F0$13cRiderVbC_btf = D_00488BB8\n"
        "_vt$15cVbDtor002E83F0$14cRiderVbBC_btf = D_00459B90\n"
        "_vt$15cVbDtor002E83F0$12cRiderVb_btf = D_00488C00\n"
        "_$_12cRiderVb_btf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_reset);
#ifdef SKIP_ASM
struct cBoardTrailFX_reset_sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct cBoardTrailFX_reset_sVtx {
    float x, y, z;
    char pad[0x24];
};

struct cBoardTrailFX_reset_sTrail {
    int f0;
    float f4;
    float f8;
    int fC;
    float* v10;
    float* v14;
    float* v18;
    float* v1C;
    float* v20;
    float* v24;
    char pad28[0x8C - 0x28];
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
    int fA0;
    int fA4;
    char padA8[8];
    cBoardTrailFX_reset_sVec4 colB0;
    cBoardTrailFX_reset_sVec4 colC0;
    cBoardTrailFX_reset_sVec4 colD0;
    int fE0;
};

extern cBoardTrailFX_reset_sVec4 t4vect_Origin;
extern cBoardTrailFX_reset_sVec4 t4vect_Zero;
extern float D_004A45B0;
extern float D_004A45B4;
extern float D_004A45B8;

/**
 * Resets a board trail's state and the texture data of its six vertex strips.
 *
 * Clears the trail's counters, sets the first colour vector to t4vect_Origin and the other two to zero, sets both
 * scales to 1, and rewrites the first three floats of all 54 vertices in each strip: probably texture
 * coordinates, with u fixed per strip (0.25, 0.75, 0.0, 1.0, 0.375, 0.625), v alternating 0 and 1, and q = 1.
 */
extern "C" void cBoardTrailFX_reset(cBoardTrailFX_reset_sTrail* self)
{
    self->f94 = 2;
    self->f90 = 0;
    self->f8C = 0;
    self->f98 = 0;
    self->f9C = 0;
    self->fA0 = 0;
    self->fA4 = 0;
    self->colB0 = t4vect_Origin;
    self->colC0 = t4vect_Zero;
    self->colD0 = t4vect_Zero;
    self->f4 = 1.0f;
    self->f8 = 1.0f;
    self->fE0 = 0;
    for (int i = 0; i < 54; i++) {
        float vCoord = (float)(i & 1);
        self->v18[i * 12] = D_004A45B8;
        self->v10[i * 12] = D_004A45B4;
        self->v1C[i * 12] = D_004A45B0;
        self->v20[i * 12] = 1.0f - D_004A45B0;
        self->v14[i * 12] = 1.0f - D_004A45B4;
        self->v24[i * 12] = 1.0f - D_004A45B8;
        self->v18[i * 12 + 1] = self->v10[i * 12 + 1] = self->v1C[i * 12 + 1] = self->v20[i * 12 + 1] = self->v14[i * 12 + 1] = self->v24[i * 12 + 1] = vCoord;
        self->v18[i * 12 + 2] = self->v10[i * 12 + 2] = self->v1C[i * 12 + 2] = self->v20[i * 12 + 2] = self->v14[i * 12 + 2] = self->v24[i * 12 + 2] = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_shouldSample);
#ifdef SKIP_ASM


// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector btSub_86F0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float btLength_86F0(const t4Vector& v)
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

// Same test as cBoardTrailFX_shouldPlow's boardTrailCanDraw.
static inline int boardTrailCanDraw_86F0(char* vr)
{
    cRider* r = (cRider*)vr;
    int ok = 0;
    if (r->mIsInNIS != 0 && r->unk_AD0 == 0) ok = r->unk_AFC != 0;
    return ok;
}

/**
 * Returns whether the board trail takes a new sample this frame, and if so remembers the rider's ground contact point.
 *
 * While the delay (+0xE0) is positive it counts it down and returns 0. Otherwise the trail samples when its state
 * (+0x94) is 1, when the rider's mGroundContactPoint has moved at least 110 units from the last sample (+0xB0; 30 units
 * when mIsInNIS is set, unk_AD0 is clear and unk_AFC is set), or when @p dirDot is below 0.999. State 1 also sets the delay
 * to 2. On a sample it copies mGroundContactPoint to +0xB0. cBoardTrailFX_update calls it with the dot product of the
 * new and previous travel directions.
 *
 * @param dirDot  dot product of this frame's and the last direction (1 = no turn)
 * @return 1 when a new sample is taken, otherwise 0
 */
extern "C" int cBoardTrailFX_shouldSample(cBoardTrailFX* self, float dirDot)
{
    if (self->DelaySampling > 0) {
        self->DelaySampling--;
        return 0;
    }
    t4Vector dist = btSub_86F0(self->mRider->mGroundContactPoint, (*(&self->LastCen)));
    float maxLen = 110.0f;
    float len = btLength_86F0(dist);
    if (boardTrailCanDraw_86F0(((char*)self->mRider))) {
        maxLen = 30.0f;
    }
    if (self->State == 1 || maxLen <= len || dirDot < 0.999f) {
        if (self->State == 1) {
            self->DelaySampling = 2;
        }
        (*(&self->LastCen)) = self->mRider->mGroundContactPoint;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_shouldPlow);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRider_getMotionState(void* self);
int cRiderAnimBase_getCurrentAnimID(void* self, int a1);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void* self, int a1);
extern "C" int tEventFlags_isSet1(void* self, int a1);
extern float cBoardTrail_m_PackedDepthScale;
extern float cBoardTrail_m_LooseDepthScale;
extern float cBoardTrail_m_PowderDepthScale;
extern float cBoardTrail_m_DeepPowderDepthScale;

static inline int boardTrailCanDraw(char* vr)
{
    cRider* r = (cRider*)vr;
    int ok = 0;
    if (r->mIsInNIS != 0 && r->unk_AD0 == 0) ok = r->unk_AFC != 0;
    return ok;
}
/**
 * Returns whether the rider lays a board trail this frame and sets the trail's fade rate (+0x9C) from the surface.
 *
 * In motion state 2 (wipeout, cRider_getMotionState) there is no trail when the word at mRiderHandler + 0x30 is set. In
 * the other non-ground states it needs mIsInNIS set, unk_AD0 clear and unk_AFC and unk_B00 set. When layer 2's
 * animation (cRiderAnimBase_getCurrentAnimID on mRiderAnim) is 22 and tEventFlags_isSet1 on its sequence (+0xB0) is true for
 * 0 but false for 1, there is no trail either.
 *
 * Then by mSurfaceType: 1 sets cBoardTrail_m_LooseDepthScale (0); 0, 5, 6 and 12 set cBoardTrail_m_PackedDepthScale (5); 2 and 13 set cBoardTrail_m_PowderDepthScale (-20); 3 sets
 * cBoardTrail_m_DeepPowderDepthScale (-40). Any other surface gives no trail. cBoardTrailFX_update closes the trail when it returns 0.
 *
 * @return 1 when a trail is laid, otherwise 0
 */
extern "C" int cBoardTrailFX_shouldPlow(char** self)
{
    if (cRider_getMotionState(self[0]) == 2) {
        if (*(int*)(*(char**)(self[0] + 0x77C) + 0x30) != 0) {
            return 0;
        }
    } else if (cRider_getMotionState(self[0]) != 0) {
        if (!boardTrailCanDraw(self[0])) return 0;
        if (*(int*)(self[0] + 0xB00) == 0) return 0;
    }
    if (cRiderAnimBase_getCurrentAnimID(*(void**)(self[0] + 0x784), 2) == 22) {
        if (tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(*(void**)(self[0] + 0x784), 2) + 0xB0, 0)) {
            if (!tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(*(void**)(self[0] + 0x784), 2) + 0xB0, 1)) {
                return 0;
            }
        }
    }
    switch (*(int*)(self[0] + 0x438)) {
    case 1:
        *(float*)((char*)self + 0x9C) = cBoardTrail_m_LooseDepthScale;
        return 1;
    case 0:
    case 5:
    case 6:
    case 12:
        *(float*)((char*)self + 0x9C) = cBoardTrail_m_PackedDepthScale;
        return 1;
    case 2:
    case 13:
        *(float*)((char*)self + 0x9C) = cBoardTrail_m_PowderDepthScale;
        return 1;
    case 3:
        *(float*)((char*)self + 0x9C) = cBoardTrail_m_DeepPowderDepthScale;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Segments in the trail ring buffer (the prototype asserts BufHead < kTrailNumSeg).
enum { kTrailNumSeg = 54 };

struct sBTV4_8938 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sBTMtx_8938 {
    sBTV4_8938 row[4];
};

// One trail vertex: texture coordinates, integer colour (0..255) and position.
struct sBTVtx_8938 {
    float u, v, q;
    int f0C;
    int r, g, b, a;
    sBTV4_8938 pos;
};

// The trail part of sBoardTrailFXView (the cRider virtual base follows at +0xF0).
struct sBTrail_8938 {
    char* rider;                // 0x00 virtual base pointer (the cRider part)
    float w1;                   // 0x04
    float w2;                   // 0x08
    int flip;                   // 0x0C
    sBTVtx_8938* strip[6];      // 0x10
    int f28;                    // 0x28
    short ids[48];              // 0x2C
    int head;                   // 0x8C
    int count;                  // 0x90
    int state;                  // 0x94
    int f98;                    // 0x98
    float fade;                 // 0x9C
    float lift;                 // 0xA0
    int fA4;                    // 0xA4
    int padA8[2];               // 0xA8
    sBTV4_8938 colB0;           // 0xB0
    sBTV4_8938 lastDir;         // 0xC0
    sBTV4_8938 side;            // 0xD0
};

// Rider colour (a, r, g, b) of the render-state table entry (0xF0 bytes each).
struct sBTCol_8938 {
    float a, r, g, b;
};
struct sBTRSRow_8938 {
    sBTCol_8938 col;
    char pad[0xF0 - 0x10];
};

struct sBTVEnt_8938 {
    short delta;
    short index;
    int (*fn)(void*);
};

// cRenderStateMan_m_state: the lens/surface table, 8 rows of 0xF0 bytes (config size 0x780); sBTRSRow_8938 views a row from its +0x28 field
extern char cRenderStateMan_m_state[];
extern float D_004A4580;
extern float D_004A4584;
extern float D_004A4588;
extern float D_004A458C;
extern float D_004A45AC;
extern float D_004A3B28;
extern float D_004A3B2C;
extern int D_004A3AFC;
extern "C" float func_0031C228(float);
// Decides whether the trail starts a new segment this frame.
extern "C" int cBoardTrailFX_shouldSample_8938(sBTrail_8938* self, float d) __asm__("cBoardTrailFX_shouldSample");

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sBTV4_8938 btScale(const sBTV4_8938& v, float s)
{
    sBTV4_8938 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sBTV4_8938 btAdd(const sBTV4_8938& a, const sBTV4_8938& b)
{
    sBTV4_8938 r;
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sBTV4_8938 btSub(const sBTV4_8938& a, const sBTV4_8938& b)
{
    sBTV4_8938 r;
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

// PORT: PS2-only VU0 inline asm (a += b). Assembled in noreorder mode: retail leaves the final sqc2 out of a
// following branch delay slot.
static inline void btAddEq(sBTV4_8938& a, const sBTV4_8938& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float btLength(const sBTV4_8938& v)
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
static inline float btDot(const sBTV4_8938& a, const sBTV4_8938& b)
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

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sBTV4_8938 btDiv(const sBTV4_8938& v, float s)
{
    sBTV4_8938 r;
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

// PORT: PS2-only VU0 inline asm (normalize).
static inline sBTV4_8938 btNormalize(const sBTV4_8938& v)
{
    sBTV4_8938 r;
    __asm__(
        "lqc2       $vf3, %1\n"
        "vaddw.x    $vf6, $vf0, $vf0w\n"
        "vmul.xyzw  $vf4, $vf3, $vf3\n"
        "vadday.x   ACC, $vf4, $vf4y\n"
        "vmaddaz.x  ACC, $vf6, $vf4z\n"
        "vmaddw.x   $vf4, $vf6, $vf4w\n"
        "vrsqrt     Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sBTV4_8938 btCross(const sBTV4_8938& a, const sBTV4_8938& b)
{
    sBTV4_8938 r;
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

// PORT: PS2-only inline asm (float absolute value).
static inline float btAbs(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ minimum operator (<?).
static inline float btClamp(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

static inline float btAtan2(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

// PORT: the seed is reinterpreted as a float in [1,2) through memory (type pun).
static inline float btRand()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

// A random value in [0, s).
static inline float btRandScale(const float& s)
{
    return s * btRand();
}

// Bone matrix of the rider's model; off is the cRider field holding the bone index.
static inline sBTMtx_8938* btBone(char* r, int off)
{
    return &((sBTMtx_8938*)*(char**)(*(char**)(r + 0x780) + 0x34))[*(int*)(r + off)];
}

static inline sBTV4_8938 btBoneRow(char* r, int off, int row)
{
    return btBone(r, off)->row[row];
}

static inline sBTV4_8938 btBonePos(char* r, int off)
{
    return btBone(r, off)->row[3];
}

static inline int btCanDraw(char* vr)
{
    cRider* r = (cRider*)vr;
    int ok = 0;
    if (r->mIsInNIS != 0 && r->unk_AD0 == 0) ok = r->unk_AFC != 0;
    return ok;
}

static inline void btGrow(sBTrail_8938* self)
{
    if (self->count < kTrailNumSeg) self->count++;
}

/**
 * Adds this frame's segment to the board trail ring (6 strips of 54 vertices).
 *
 * When cBoardTrailFX_shouldPlow says no trail, the trail is closed (faded to alpha 0 or rewound). Otherwise it takes the
 * board's outermost edges across the travel direction (board bone, or bone positions off the ground),
 * projects them to the ground contact, builds the six strip points (random flare widths +0x04/+0x08, lift
 * +0xA0) coloured from the rider's surface, and advances the head when cBoardTrailFX_shouldSample (probably shouldSample)
 * agrees. A new trail starts with 3 segments.
 */
extern "C" void cBoardTrailFX_update(sBTrail_8938* self)
{
    int prev = (self->head + kTrailNumSeg - 1) % kTrailNumSeg;
    if (cBoardTrailFX_shouldPlow((char**)self) == 0) {
        if (self->state == 0) return;
        if (self->state == 1) {
            self->head = (self->head + kTrailNumSeg - 3) % kTrailNumSeg;
            // Unused on this path; retail keeps the divisor load of its (removed) division check.
            prev = (self->head + kTrailNumSeg - 1) % kTrailNumSeg;
            int c = self->count - 3;
            if (c < 0) c = 0;
            self->count = c;
        } else {
            self->strip[2][prev].pos = self->strip[3][prev].pos = self->strip[0][prev].pos;
            self->strip[5][prev].pos = self->strip[4][prev].pos = self->strip[1][prev].pos;
            for (int i = 0; i < 6; i++) {
                self->strip[i][self->head].pos = self->strip[1][prev].pos;
                self->strip[i][self->head].pos.w = 1.0f;
                self->strip[i][self->head].a = self->strip[1][prev].a = 0;
                self->strip[i][self->head].r = self->strip[1][prev].r;
                self->strip[i][self->head].g = self->strip[1][prev].g;
                self->strip[i][self->head].b = self->strip[1][prev].b;
            }
        }
        self->state = 0;
        return;
    }
    if (self->state == 0) {
        self->head = (self->head + 3) % kTrailNumSeg;
        prev = (self->head + kTrailNumSeg - 1) % kTrailNumSeg;
    }
    self->lift = D_004A4588 - self->fade * D_004A458C;
    sBTV4_8938 pts[4];
    const sBTV4_8938& pos = btBoneRow(self->rider, 2212, 3);
    const sBTV4_8938& side = btBoneRow(self->rider, 2212, 2);
    const sBTV4_8938& fwd = btBoneRow(self->rider, 2212, 0);
    sBTV4_8938 halfLengthVec = btScale(fwd, 87.5f);
    sBTV4_8938 halfWidthVec = btScale(side, 17.5f);
    if (cRider_getMotionState(self->rider) == 0 || (btCanDraw(self->rider) && *(int*)(self->rider + 0xB00) != 0)) {
        sBTV4_8938 left = btSub(pos, halfLengthVec);
        float sideSign = 0.0f;
        sBTV4_8938 right = btAdd(pos, halfLengthVec);
        if (*(int*)(self->rider + 0x330) != 0) {
            int k = cRiderAnimBase_getCurrentAnimID(*(void**)(self->rider + 0x784), 2);
            if (k >= 23 && k <= 0x1E) {
                sideSign = -1.0f;
                if (*(int*)(self->rider + 0x320) != 0) sideSign = 1.0f;
            } else {
                sideSign = 1.0f;
                if (*(int*)(self->rider + 0x320) != 0) sideSign = -1.0f;
            }
        }
        if (0.0f < sideSign) {
            btAddEq(left, btScale(fwd, sideSign * 175.0f <? 140.0f));
        } else if (sideSign < 0.0f) {
            btAddEq(right, btScale(fwd, sideSign * 175.0f >? -140.0f));
        }
        pts[0] = btAdd(right, halfWidthVec);
        pts[2] = btAdd(left, halfWidthVec);
        pts[1] = btSub(right, halfWidthVec);
        pts[3] = btSub(left, halfWidthVec);
    } else {
        cRider* riderObj = (cRider*)self->rider;
        pts[0] = btBonePos(((char*)riderObj), 2224);
        pts[1] = btBonePos(((char*)riderObj), 2232);
        if (riderObj->mUseRootPoseOverride != 0) {
            pts[2] = btBonePos(((char*)riderObj), 2328);
            pts[3] = btBonePos(((char*)riderObj), 2280);
        } else {
            sBTV4_8938 left = btSub(pos, halfLengthVec);
            pts[2] = btAdd(left, halfWidthVec);
            pts[3] = btSub(left, halfWidthVec);
        }
    }
    float speed = btLength(*(sBTV4_8938*)(self->rider + 0x1E0));
    sBTV4_8938 dir;
    if (speed < 0.01f) {
        dir = self->lastDir;
    } else {
        dir = btDiv(*(sBTV4_8938*)(self->rider + 0x1E0), speed);
    }
    dir = btNormalize(btSub(dir, btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(dir, *(sBTV4_8938*)(self->rider + 0x370)))));
    sBTV4_8938 across = btCross(dir, *(sBTV4_8938*)(self->rider + 0x370));
    if (btDot(across, self->side) < 0.0f) {
        across = btScale(across, -1.0f);
    }
    sBTV4_8938 contactToPt1 = btSub(pts[1], *(sBTV4_8938*)(self->rider + 0x460));
    sBTV4_8938 contactToPt0 = btSub(pts[0], *(sBTV4_8938*)(self->rider + 0x460));
    sBTV4_8938 contactToPt3 = btSub(pts[3], *(sBTV4_8938*)(self->rider + 0x460));
    sBTV4_8938 contactToPt2 = btSub(pts[2], *(sBTV4_8938*)(self->rider + 0x460));
    float sideDist1 = btDot(contactToPt1, across);
    float sideDist0 = btDot(contactToPt0, across);
    float sideDist3 = btDot(contactToPt3, across);
    float sideDist2 = btDot(contactToPt2, across);
    sBTV4_8938 top;
    sBTV4_8938 bot;
    if (sideDist0 < sideDist1 && sideDist3 < sideDist1 && sideDist2 < sideDist1) {
        top = btSub(pts[1], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt1) - D_004A3B28));
    } else if (sideDist1 < sideDist0 && sideDist3 < sideDist0 && sideDist2 < sideDist0) {
        top = btSub(pts[0], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt0) - D_004A3B28));
    } else if (sideDist1 < sideDist3 && sideDist0 < sideDist3 && sideDist2 < sideDist3) {
        top = btSub(pts[3], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt3) - D_004A3B28));
    } else {
        top = btSub(pts[2], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt2) - D_004A3B28));
    }
    if (sideDist1 < sideDist0 && sideDist1 < sideDist3 && sideDist1 < sideDist2) {
        bot = btSub(pts[1], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt1) - D_004A3B28));
    } else if (sideDist0 < sideDist1 && sideDist0 < sideDist3 && sideDist0 < sideDist2) {
        bot = btSub(pts[0], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt0) - D_004A3B28));
    } else if (sideDist3 < sideDist1 && sideDist3 < sideDist0 && sideDist3 < sideDist2) {
        bot = btSub(pts[3], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt3) - D_004A3B28));
    } else {
        bot = btSub(pts[2], btScale(*(sBTV4_8938*)(self->rider + 0x370), btDot(*(sBTV4_8938*)(self->rider + 0x370), contactToPt2) - D_004A3B28));
    }
    sBTV4_8938 et = btSub(top, *(sBTV4_8938*)(self->rider + 0x460));
    sBTV4_8938 eb = btSub(bot, *(sBTV4_8938*)(self->rider + 0x460));
    float ft = btDot(et, dir);
    float fb = btDot(eb, dir);
    float st = btDot(et, across);
    float sb = btDot(eb, across);
    if (cRider_getMotionState(self->rider) == 2) {
        top = btAdd(btAdd(*(sBTV4_8938*)(self->rider + 0x460), btScale(across, st * 0.75f)), btScale(*(sBTV4_8938*)(self->rider + 0x370), D_004A3B28));
        bot = btAdd(btAdd(*(sBTV4_8938*)(self->rider + 0x460), btScale(across, sb * 0.75f)), btScale(*(sBTV4_8938*)(self->rider + 0x370), D_004A3B28));
    }
    self->strip[2][self->head].pos = top;
    self->strip[0][self->head].pos = top;
    self->strip[3][self->head].pos = top;
    self->strip[4][self->head].pos = bot;
    self->strip[1][self->head].pos = bot;
    self->strip[5][self->head].pos = bot;
    float ang = btAtan2(btAbs(ft), btAbs(st));
    float k;
    if (0.7853982448577881f < ang) {
        k = btClamp(2.0f - ang * 1.2732393741607666f, 0.0f, 1.0f);
    } else {
        k = btClamp(ang * 1.2732393741607666f, 0.0f, 1.0f);
    }
    if (speed * 0.0036000001709908247f <= 1.0f) {
        k *= speed * 0.0036000001709908247f;
    }
    sBTV4_8938 ground = btAdd(*(sBTV4_8938*)(self->rider + 0x460), btScale(*(sBTV4_8938*)(self->rider + 0x370), D_004A3B28));
    sBTV4_8938 mid;
    if (ft < fb) {
        mid = btAdd(ground, btScale(dir, k * ft));
    } else {
        mid = btAdd(ground, btScale(dir, k * fb));
    }
    sBTV4_8938 topEdge = btAdd(mid, btScale(across, st));
    sBTV4_8938 botEdge = btAdd(mid, btScale(across, sb));
    sBTV4_8938 n = btNormalize(btSub(botEdge, topEdge));
    sBTV4_8938 innerWidth = btScale(n, D_004A4580);
    sBTV4_8938 outerWidth = btScale(btScale(n, D_004A4584), btAbs(*(float*)(self->rider + 0x454) <? 0.0f) + 10.0f);
    sBTV4_8938 w = btScale(*(sBTV4_8938*)(self->rider + 0x370), self->lift);
    sBTV4_8938 h = btScale(*(sBTV4_8938*)(self->rider + 0x370), *(float*)(self->rider + 0x454) <? 0.0f);
    self->strip[2][prev].pos = btAdd(btAdd(btSub(topEdge, outerWidth), h), h);
    self->strip[0][prev].pos = btAdd(btSub(topEdge, btScale(innerWidth, self->w1)), w);
    self->strip[3][prev].pos = btAdd(topEdge, h);
    self->strip[4][prev].pos = btAdd(botEdge, h);
    self->strip[1][prev].pos = btAdd(btAdd(botEdge, btScale(innerWidth, self->w2)), w);
    self->strip[5][prev].pos = btAdd(btAdd(btAdd(botEdge, outerWidth), h), h);
    char* being = self->rider + 0x6C0;
    sBTVEnt_8938* vt = *(sBTVEnt_8938**)being;
    sBTCol_8938 c = ((sBTRSRow_8938*)(cRenderStateMan_m_state + 0x28))[vt[7].fn(vthis(being, vt[7]))].col;
    for (int i = 0; i < 6; i++) {
        self->strip[i][self->head].a = self->strip[i][prev].a = (int)btClamp(c.a * 255.0f, 0.0f, 255.0f);
        self->strip[i][self->head].r = self->strip[i][prev].r = (int)btClamp(c.r * 255.0f, 0.0f, 255.0f);
        self->strip[i][self->head].g = self->strip[i][prev].g = (int)btClamp(c.g * 255.0f, 0.0f, 255.0f);
        self->strip[i][self->head].b = self->strip[i][prev].b = (int)btClamp(c.b * 255.0f, 0.0f, 255.0f);
    }
    for (int i = 0; i < 6; i++) {
        self->strip[i][self->head].pos.w = 1.0f;
    }
    sBTV4_8938 stripStep0 = btSub(self->strip[0][self->head].pos, self->strip[0][prev].pos);
    sBTV4_8938 stripStep1 = btSub(self->strip[1][self->head].pos, self->strip[1][prev].pos);
    int back0 = btDot(stripStep0, dir) < 0.0f;
    int back1 = btDot(stripStep1, dir) < 0.0f;
    if (back0) {
        if (back1) {
            self->strip[0][self->head].pos = self->strip[2][self->head].pos = self->strip[3][self->head].pos =
                btAdd(self->strip[0][prev].pos, btScale(dir, D_004A3B2C));
            self->strip[1][self->head].pos = self->strip[5][self->head].pos = self->strip[4][self->head].pos =
                btAdd(self->strip[1][prev].pos, btScale(dir, D_004A3B2C));
        } else {
            self->strip[0][self->head].pos = self->strip[2][self->head].pos = self->strip[3][self->head].pos =
                self->strip[1][self->head].pos;
        }
    } else if (back1) {
        self->strip[1][self->head].pos = self->strip[5][self->head].pos = self->strip[4][self->head].pos =
            self->strip[0][self->head].pos;
    }
    if (self->state == 0) {
        int pp = (prev + kTrailNumSeg - 1) % kTrailNumSeg;
        for (int i = 0; i < 6; i++) {
            self->strip[i][pp].pos = self->strip[1][prev].pos;
            self->strip[i][pp].r = self->strip[1][prev].r;
            self->strip[i][pp].g = self->strip[1][prev].g;
            self->strip[i][pp].b = self->strip[1][prev].b;
            self->strip[i][pp].a = self->strip[i][prev].a = 0;
        }
        self->strip[2][prev].pos = self->strip[3][prev].pos = self->strip[0][prev].pos;
        self->strip[5][prev].pos = self->strip[4][prev].pos = self->strip[1][prev].pos;
        btGrow(self);
        btGrow(self);
        btGrow(self);
        self->state = 1;
    }
    if (cBoardTrailFX_shouldSample_8938(self, btDot(dir, self->lastDir))) {
        int h = self->head;
        self->side = across;
        self->w1 = btRandScale(D_004A45AC) + 1.0f;
        self->w2 = btRandScale(D_004A45AC) + 1.0f;
        self->head = (h + 1) % kTrailNumSeg;
        btGrow(self);
        for (int i = 0; i < 6; i++) {
            self->strip[i][self->head].pos = self->strip[i][h].pos;
            self->strip[i][self->head].a = self->strip[i][h].a;
            self->strip[i][self->head].r = self->strip[i][h].r;
            self->strip[i][self->head].g = self->strip[i][h].g;
            self->strip[i][self->head].b = self->strip[i][h].b;
        }
        self->state = 2;
    }
    self->lastDir = dir;
    self->flip = 1 - self->flip;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_render);
#ifdef SKIP_ASM
struct sMtxEntry_A480 {
    int v[5];
};
extern int cRenderToggles_m_DisableBoardTrail;
extern int cRenderToggles_m_DisableAIBoardTrail;
struct sRenderCtx_A480 {
    char pad[0xE84];
    sMtxEntry_A480* top;
};

class cBoardTrailFX_render_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int isComputer();
};

static inline void push_A480(sRenderCtx_A480* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void pop_A480(sRenderCtx_A480* ctx)
{
    ctx->top--;
}

/**
 * Board trail render stub: pushes and pops a draw state but draws nothing.
 *
 * Returns early when cRenderToggles_m_DisableBoardTrail is set, when cRenderToggles_m_DisableAIBoardTrail is set and the rider part's virtual slot 9 is true,
 * or with fewer than 2 segments (+0x90).
 */
extern "C" void cBoardTrailFX_render(cBoardTrailFX* self)
{
    if (cRenderToggles_m_DisableBoardTrail != 0) {
        return;
    }
    if (cRenderToggles_m_DisableAIBoardTrail != 0) {
        cBoardTrailFX_render_cObj* being = (cBoardTrailFX_render_cObj*)(*(char**)self + 0x6C0);
        if (being->isComputer() != 0) {
            return;
        }
    }
    if (self->BufLen < 2) {
        return;
    }
    push_A480((sRenderCtx_A480*)GrMan);
    pop_A480((sRenderCtx_A480*)GrMan);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA538);
#ifdef SKIP_ASM
extern int D_004A45C8;

struct sTrailStripA538 {
    char* pts;                  // 0x0
    int start;                  // 0x4
    int count;                  // 0x8
    int fC;                     // 0xC
    float scale;                // 0x10
};
struct sRctxVtA538 { short delta; short index; void (*fn)(void*, sTrailStripA538*, int); };

/**
 * Draws the board trails of several riders in one batch through the graphics manager.
 *
 * Pushes a render state, then for each trail (a cBoardTrailFX, cRiderHandler + 0x520) with at least 2 samples (+0x90)
 * builds a 0x14-byte strip record: the vertex ring (+0x10), the start index in the 54-entry ring derived from the head
 * (+0x8C), the length (samples - 2, at most 48), 0 and 1 / D_004A45C8 (1/16). At most 16 strips are kept. Passes them
 * to GrMan's virtual slot 89 (strips, count) and pops the state. cAI_render calls it with every rider's trail.
 *
 * @param trails  array of trail pointers
 * @param n  number of entries in @p trails
 */
extern "C" void func_002EA538(char** trails, int n)
{
    sTrailStripA538 strips[16];
    int count = 0;
    push_A480((sRenderCtx_A480*)GrMan);
    if (n > 0) {
    do {
        cBoardTrailFX* trail = (cBoardTrailFX*)(*trails);
        int len = trail->BufLen;
        if (len < 2) continue;
        int k = len - 57;
        int start = trail->BufHead - k;
        int cnt = len - 2;
        int over = len - 50;
        if (over > 0) {
            cnt -= over;
            start += over;
        }
        if (cnt < 2) continue;
        int st = start % 54;
        float sc = 1.0f / (float)D_004A45C8;
        if (count < 16) {
            strips[count].pts = (char*)&trail->BufVerts[0];
            strips[count].start = st;
            strips[count].count = cnt;
            strips[count].fC = 0;
            strips[count].scale = sc;
            count++;
        }
    } while (trails++, --n > 0);
    }
    {
        cPS2GraphicsMan* ctx = GrMan;
        sRctxVtA538* vt = (sRctxVtA538*)ctx->mVtbl;
        vt[89].fn(vthis(ctx, vt[89]), strips, count);
    }
    pop_A480((sRenderCtx_A480*)GrMan);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_cCutSceneTransitionMan);
#ifdef SKIP_ASM
extern void* cCutSceneTransitionMan_vtbl[];
extern "C" void* cObjNode_cObjNode1(void* self);
extern "C" void cCutSceneTransitionMan_FindTVParameters(cCutSceneTransitionMan* self);

/**
 * Constructs the cut-scene transition manager as a render object.
 *
 * Runs cObjNode_cObjNode1() (the caller's bucket, 3, reaches it in the second argument register), stores the
 * vtable, clears the words at 0x10 and 0x1C, and calls cCutSceneTransitionMan_FindTVParameters(), which saves the
 * graphics manager's current display settings and picks the ones to use during a transition.
 *
 * @return @p self
 */
extern "C" void* cCutSceneTransitionMan_cCutSceneTransitionMan(cCutSceneTransitionMan* self)
{
    cObjNode_cObjNode1(self);
    self->mVtbl = cCutSceneTransitionMan_vtbl;
    self->mCurrentState = 0;
    self->mNumPause = 0;
    cCutSceneTransitionMan_FindTVParameters(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_StartCutScene);
#ifdef SKIP_ASM
extern "C" void cCutSceneTransitionMan_FindTVParameters(cCutSceneTransitionMan* self);

struct cCutSceneTransitionMan_StartCutScene_sFade {
    char pad[0x10];
    int state;
    float t;
    float dur;
};

/**
 * Starts the transition into cut-scene mode over a given time.
 *
 * From state 0 it reads the TV parameters (cCutSceneTransitionMan_FindTVParameters) and starts from 0. In state 1
 * (already in cut-scene mode) it does nothing. A transition back out (state 2) is reversed and one already going in
 * (state 3) keeps its progress, both rescaled to @p dur. The state becomes 3.
 *
 * @param dur  transition length (On Tour time)
 */
extern "C" void cCutSceneTransitionMan_StartCutScene(cCutSceneTransitionMan* self, float dur)
{
    switch (self->mCurrentState) {
    case 0:
        cCutSceneTransitionMan_FindTVParameters(self);
        self->mfLengthOfTime = dur;
        self->mfCurrentTime = 0.0f;
        break;
    case 1:
        return;
    case 2: {
        float progress = self->mfCurrentTime / self->mfLengthOfTime;
        self->mfLengthOfTime = dur;
        self->mfCurrentTime = (1.0f - progress) * dur;
        break;
    }
    case 3: {
        float progress = self->mfCurrentTime / self->mfLengthOfTime;
        self->mfLengthOfTime = dur;
        self->mfCurrentTime = progress * dur;
        break;
    }
    }
    self->mCurrentState = 3;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_EndCutScene);
#ifdef SKIP_ASM
struct sTrailFade {
    char pad[0x10];
    int state;
    float value;
    float time;
};

/**
 * Starts the transition out of cut-scene mode over @p fadeTime.
 *
 * In state 0 (regular mode) it does nothing. From state 1 (cut-scene mode) it starts from 0; a transition already going
 * out (state 2) keeps its progress and one going in (state 3) is reversed, both rescaled to @p fadeTime. The elapsed
 * time is +0x14 and the length +0x18; the state (+0x10) becomes 2. tNISList_PlayNextScript calls it on the game's
 * CutSceneTransitionMan.
 *
 * @param fadeTime  transition length
 */
extern "C" void cCutSceneTransitionMan_EndCutScene(cCutSceneTransitionMan* self, float fadeTime)
{
    switch (self->mCurrentState) {
    case 0:
        return;
    case 1:
        self->mfLengthOfTime = fadeTime;
        self->mfCurrentTime = 0.0f;
        break;
    case 2:
        self->mfCurrentTime = (self->mfCurrentTime / self->mfLengthOfTime) * fadeTime;
        self->mfLengthOfTime = fadeTime;
        break;
    case 3:
        self->mfCurrentTime = (1.0f - self->mfCurrentTime / self->mfLengthOfTime) * fadeTime;
        self->mfLengthOfTime = fadeTime;
        break;
    }
    self->mCurrentState = 2;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_ForceToRegularMode);
#ifdef SKIP_ASM
struct sBTVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


/**
 * Switches the screen straight back to regular (non cut-scene) mode.
 *
 * When the mode flag at +0x10 is set, clears it and passes the regular parameters at +0x20 to the graphics manager's
 * virtual slot 41.
 */
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(cCutSceneTransitionMan* self)
{
    if (self->mCurrentState != 0) {
        self->mCurrentState = 0;
        cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
        sBTVEntry* vtblEntry = &(((sBTVEntry*)obj->mVtbl))[41];
        vtblEntry->fn(vthis(obj, *vtblEntry), (char*)&self->mRegularTVFormat);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_ForceToCutSceneMode);
#ifdef SKIP_ASM

/**
 * Switches the screen straight into cut-scene mode.
 *
 * Reads the TV parameters first if the mode flag at +0x10 is clear, sets the flag and passes the cut-scene parameters
 * at +0x34 to the graphics manager's virtual slot 41.
 */
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(cCutSceneTransitionMan* self)
{
    if (self->mCurrentState == 0) {
        cCutSceneTransitionMan_FindTVParameters(self);
    }
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    self->mCurrentState = 1;
    sBTVEntry* vtblEntry = &(((sBTVEntry*)obj->mVtbl))[41];
    vtblEntry->fn(vthis(obj, *vtblEntry), (char*)&self->mCutSceneTVFormat);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_Pause);
#ifdef SKIP_ASM
/**
 * Counts a pause or resume request for the cut-scene transition.
 *
 * While a transition runs (state +0x10 is 2 or 3) it clears the pause count (+0x1C). Otherwise @p up increments the
 * count and a zero @p up decrements it when positive. tNISList_DoPause calls it with 1 and 0 on the game's
 * CutSceneTransitionMan.
 *
 * @param up  nonzero to pause, zero to resume
 */
extern "C" void cCutSceneTransitionMan_Pause(cCutSceneTransitionMan* self, int up)
{
    int state = self->mCurrentState;
    if (state == 2 || state == 3) {
        self->mNumPause = 0;
        return;
    }
    if (up) {
        self->mNumPause += 1;
        return;
    }
    if (self->mNumPause > 0) {
        self->mNumPause -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(cCutSceneTransitionMan* self);
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(cCutSceneTransitionMan* self);

struct sFadeCol_A900 {
    int id;
    float c[4];
};

struct sVE_A900 { short delta; short index; void (*fn)(void*, sFadeCol_A900*); };

struct sFade_A900 {
    char pad0[0x10];
    int mode;           // 0x10
    float t;            // 0x14
    float dur;          // 0x18
    int done;           // 0x1C
    int id;             // 0x20
    float from[4];      // 0x24
    int f34;
    float to[4];        // 0x38
};

extern char* D_004A5B64_A900 __asm__("cAppMan_TheAppMan");

/**
 * Blends the TV display format between regular and cut-scene mode.
 *
 * Only while not paused (+0x1C) and in state 2 (to regular) or 3 (to cut-scene): advances the time by 1 / the
 * app manager's word at +0x10 and, at the end, forces regular (cCutSceneTransitionMan_ForceToRegularMode) or cut-scene mode
 * (cCutSceneTransitionMan_ForceToCutSceneMode). Otherwise it lerps the four floats of the regular (+0x20) and cut-scene (+0x34) formats
 * and applies them through the graphics manager's virtual slot 41.
 */
extern "C" void cCutSceneTransitionMan_update(cCutSceneTransitionMan* self)
{
    if (self->mNumPause != 0)
        return;
    int mode = self->mCurrentState;
    if (mode != 2 && mode != 3)
        return;
    self->mfCurrentTime += 1.0f / (float)*(int*)(D_004A5B64_A900 + 0x10);
    if (self->mfLengthOfTime < self->mfCurrentTime) {
        if (mode == 2)
            cCutSceneTransitionMan_ForceToRegularMode(self);
        else
            cCutSceneTransitionMan_ForceToCutSceneMode(self);
        return;
    }
    float k = self->mfCurrentTime / self->mfLengthOfTime;
    if (mode == 2)
        k = 1.0f - k;
    sFadeCol_A900 col;
    col.id = self->mRegularTVFormat.mDisplayFormat;
    col.c[2] = ((float*)&self->mRegularTVFormat.mfTVYOffset)[2] + (((float*)&self->mCutSceneTVFormat.mfTVYOffset)[2] - ((float*)&self->mRegularTVFormat.mfTVYOffset)[2]) * k;
    col.c[3] = ((float*)&self->mRegularTVFormat.mfTVYOffset)[3] + (((float*)&self->mCutSceneTVFormat.mfTVYOffset)[3] - ((float*)&self->mRegularTVFormat.mfTVYOffset)[3]) * k;
    col.c[0] = ((float*)&self->mRegularTVFormat.mfTVYOffset)[0] + (((float*)&self->mCutSceneTVFormat.mfTVYOffset)[0] - ((float*)&self->mRegularTVFormat.mfTVYOffset)[0]) * k;
    col.c[1] = ((float*)&self->mRegularTVFormat.mfTVYOffset)[1] + (((float*)&self->mCutSceneTVFormat.mfTVYOffset)[1] - ((float*)&self->mRegularTVFormat.mfTVYOffset)[1]) * k;
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    sVE_A900* vt = (sVE_A900*)ps2GraphicsMan->mVtbl;
    vt[41].fn(vthis(ps2GraphicsMan, vt[41]), &col);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCutSceneTransitionMan_FindTVParameters);
#ifdef SKIP_ASM
struct sBTState {
    int mode;
    float a;
    float b;
    float c;
    float d;
};

struct sBTFx {
    char pad[0x20];
    sBTState saved;
    sBTState cur;
};

struct sBTVEntryGet {
    short delta;
    short index;
    sBTState* (*fn)(void*);
};


/**
 * Saves the current TV display format and derives the cut-scene format from it.
 *
 * Reads the format from the graphics manager (virtual slot 42) into +0x20. Display format 2 gets the
 * cut-scene format (2, y offset 0, y scale 1.0, aspect 0.75/0.63); any other gets (1, 0.125, 0.75,
 * 0.75/0.63), probably a letterbox.
 */
extern "C" void cCutSceneTransitionMan_FindTVParameters(cCutSceneTransitionMan* self)
{
    sBTFx* transMan = (sBTFx*)self;
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    sBTVEntryGet* vtEntry = &(((sBTVEntryGet*)obj->mVtbl))[42];
    transMan->saved = *vtEntry->fn(vthis(obj, *vtEntry));
    float xAspectAngle = 0.75f;
    float yAspectAngle = 0.6299999952316284f;
    if (transMan->saved.mode == 2) {
        transMan->cur.d = yAspectAngle;
        transMan->cur.mode = 2;
        transMan->cur.b = 1.0f;
        transMan->cur.c = xAspectAngle;
        transMan->cur.a = 0.0f;
    } else {
        transMan->cur.mode = 1;
        transMan->cur.a = 0.125f;
        transMan->cur.b = xAspectAngle;
        transMan->cur.c = xAspectAngle;
        transMan->cur.d = yAspectAngle;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cTrickBoostFX_cTrickBoostFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardtrailfx_classes.h"

/**
 * Constructs a trick boost effect and clears its ring of boost quads.
 *
 * Calls cTrickBoostFX_reset__FPv, which zeroes the count and cursor; g++ adds the cRider
 * virtual-base set-up.
 */
cBtf_2EAAE0::cBtf_2EAAE0()
{
    cTrickBoostFX_reset_AAE0(this);
}

// PORT: cRiderVb_btf stands in for cRider; its constructor is the retail cRider_cRider.
__asm__("__12cRiderVb_btf = cRider_cRider\n");

// PORT: the compiler names this ctor __11cBtf_2EAAE0i; expose it under the address-derived name.
__asm__(".globl cTrickBoostFX_cTrickBoostFX\n.type cTrickBoostFX_cTrickBoostFX,@function\ncTrickBoostFX_cTrickBoostFX = __11cBtf_2EAAE0i\n.size cTrickBoostFX_cTrickBoostFX,0x180\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$11cBtf_2EAAE0$13cRiderVbC_btf = D_00488458\n"
        "_vt$11cBtf_2EAAE0$14cRiderVbBC_btf = D_00459B90\n"
        "_vt$11cBtf_2EAAE0$12cRiderVb_btf = D_004884A0\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cTrickBoostFX__cTrickBoostFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardtrailfx_classes.h"

/**
 * Destroys a trick boost effect; the body is empty.
 *
 * g++ adds the virtual-base vtable fix-ups and the delete.
 */
cVbDtor002EAC60::~cVbDtor002EAC60()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor002EAC60; expose it under the address-derived name.
__asm__(".globl cTrickBoostFX__cTrickBoostFX\n.type cTrickBoostFX__cTrickBoostFX,@function\ncTrickBoostFX__cTrickBoostFX = _$_15cVbDtor002EAC60\n.size cTrickBoostFX__cTrickBoostFX,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002EAC60$13cRiderVbC_btf = D_00488458\n"
        "_vt$15cVbDtor002EAC60$14cRiderVbBC_btf = D_00459B90\n"
        "_vt$15cVbDtor002EAC60$12cRiderVb_btf = D_004884A0\n"
        "_$_12cRiderVb_btf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cTrickBoostFX_reset__FPv);
#ifdef SKIP_ASM
/**
 * Clears the trick boost effect's quad count (+0xC) and ring cursor (+0x10).
 *
 * Takes no replay data and reads nothing, so it acts as a reset (On Tour has both reset and
 * readFromReplayFrame). Called by the constructor, cRiderHandler_resetRiderFX,
 * cRiderHandler_readFromReplayFrame and func_002F7ED0.
 */
void cTrickBoostFX_reset(void* vself)
{
    cTrickBoostFX* self = (cTrickBoostFX*)vself;
    self->mNumActiveBlurBuffers = 0;
    self->mCurrentBlurBuffer = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cTrickBoostFX_update);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
void cPlayerLights_BoardIsAdditive(void* self, int val);
extern "C" void cPlayerLights_AddDirectional(void* self, void* q, void* v);
extern int cPowerUpFXToggles_m_TestTrickBoost;

struct sV4_2EADD0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sV3_2EADD0
{
    float x, y, z;
};

struct sSlot_2EADD0
{
    sV4_2EADD0 v[4];
};

struct sNode_2EADD0
{
    sV4_2EADD0 fwd;     // 0x00
    sV4_2EADD0 pad10;   // 0x10
    sV4_2EADD0 side;    // 0x20
    sV4_2EADD0 pos;     // 0x30
};

struct sTrail_2EADD0
{
    char* rider;        // 0x00
    int pad4[2];
    int count;          // 0x0C
    int cur;            // 0x10
    int pad14[3];
    sSlot_2EADD0 slots[3];  // 0x20
    sV4_2EADD0 e0;      // 0xE0
    sV4_2EADD0 f0;      // 0xF0
};

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV4_2EADD0 Add_2EADD0(const sV4_2EADD0& a, const sV4_2EADD0& b)
{
    sV4_2EADD0 r;
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_2EADD0 Sub_2EADD0(const sV4_2EADD0& a, const sV4_2EADD0& b)
{
    sV4_2EADD0 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_2EADD0 Scale_2EADD0(const sV4_2EADD0& v, float s)
{
    sV4_2EADD0 r;
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
 * Records the board outline for the trick boost effect while the rider's trick power-up runs.
 *
 * While mPowerUpTrickTimer (+0x2EC) is positive (or cPowerUpFXToggles_m_TestTrickBoost is set) it drives an
 * effect in the rider handler at +0xD30 (cPlayerLights_BoardIsAdditive, cPlayerLights_AddDirectional; blue, size 2). In the air (motion
 * state 1) it stores a 140 x 36 quad around the board bone in a ring of 3 (up to 3 kept), plus two points 12
 * to each side; otherwise the count drops to 0.
 */
extern "C" void cTrickBoostFX_update(sTrail_2EADD0* self)
{
    float speed = *(float*)(self->rider + 0x2EC);
    if (cPowerUpFXToggles_m_TestTrickBoost != 0)
        speed = 10.0f;
    float zeroF = 0.0f;
    if (zeroF < speed)
    {
        cPlayerLights_BoardIsAdditive(*(char**)(self->rider + 0x77C) + 0xD30, 1);
        cRider* riderBase = (cRider*)self->rider;
        sV3_2EADD0 sz;
        sz.x = 2.0f;
        sz.y = 2.0f;
        sz.z = 2.0f;
        char* fs = (char*)&riderBase->mRiderHandler->PlayerLights;
        sV4_2EADD0 col;
        col.x = zeroF;
        col.y = zeroF;
        col.z = 1.0f;
        col.w = zeroF;
        cPlayerLights_AddDirectional(fs, &col, &sz);
    }
    if (zeroF < speed && cRider_getMotionState(self->rider) == 1)
    {
        self->cur = (self->cur + 2) % 3;
        int n = self->count + 1;
        self->count = n < 4 ? n : 3;
        cRider* riderPtr = (cRider*)self->rider;
        sV4_2EADD0 pos = ((sNode_2EADD0*)((char*)riderPtr->mAnimModel->unk_030 + (riderPtr->mBoneBoardChild << 6)))->pos;
        sV4_2EADD0 fwd = ((sNode_2EADD0*)((char*)riderPtr->mAnimModel->unk_030 + (riderPtr->mBoneBoardChild << 6)))->fwd;
        sV4_2EADD0 side = ((sNode_2EADD0*)((char*)riderPtr->mAnimModel->unk_030 + (riderPtr->mBoneBoardChild << 6)))->side;
        self->e0 = Sub_2EADD0(pos, Scale_2EADD0(side, 12.0f));
        self->f0 = Add_2EADD0(pos, Scale_2EADD0(side, 12.0f));
        self->slots[self->cur].v[0] = Sub_2EADD0(Add_2EADD0(pos, Scale_2EADD0(fwd, 70.0f)), Scale_2EADD0(side, 18.0f));
        self->slots[self->cur].v[1] = Add_2EADD0(Add_2EADD0(pos, Scale_2EADD0(fwd, 70.0f)), Scale_2EADD0(side, 18.0f));
        self->slots[self->cur].v[2] = Add_2EADD0(Sub_2EADD0(pos, Scale_2EADD0(fwd, 70.0f)), Scale_2EADD0(side, 18.0f));
        self->slots[self->cur].v[3] = Sub_2EADD0(Sub_2EADD0(pos, Scale_2EADD0(fwd, 70.0f)), Scale_2EADD0(side, 18.0f));
    }
    else
    {
        self->count = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cTrickBoostFX_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Render state record (20 bytes) on the render context's state stack.
struct sRS_B198 {
    int f0;         // bits 2..3 = 3 here
    int f4;         // bits 0..1, 2..6, 12..19, 20..21, 22, 23..24
    int f8;         // bits 5..9, 10..28
    int fC;
    short tex;      // 0x10
    short pad;
};

struct sRctxVE_B198 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int);
};

struct sRctx_B198 {
    char pad_0x0[0xE84];
    sRS_B198* top;              // 0xE84
    char pad_0xE88[0x1048 - 0xE88];
    int curTex;                 // 0x1048
    char pad_0x104C[0x10D8 - 0x104C];
    sRctxVE_B198* vtable;       // 0x10D8
};

extern sRS_B198 D_00501420_B198 __asm__("sDrawState_s_defaultDrawState");

// One strip vertex: texture coords, 0..255 colour (alpha 0..128), position.
struct sTrailVtx_B198 {
    float s;                    // 0x00
    float t;                    // 0x04
    float q;                    // 0x08
    int pad;                    // 0x0C
    int r;                      // 0x10
    int g;                      // 0x14
    int b;                      // 0x18
    int a;                      // 0x1C
    sV4_2EADD0 pos;             // 0x20
    sTrailVtx_B198() {}
};

struct sTrailCol_B198 {
    float a;
    float r;
    float g;
    float b;
};

static inline void push_B198(sRctx_B198* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void addVtx_B198(sTrailVtx_B198* v, const sTrailCol_B198& c, float s, float t, const sV4_2EADD0& pos)
{
    v->r = (int)(c.r * 255.0f);
    v->g = (int)(c.g * 255.0f);
    v->b = (int)(c.b * 255.0f);
    v->a = (int)(c.a * 128.0f);
    v->q = 1.0f;
    v->pos = pos;
    v->s = s;
    v->t = t;
}

/**
 * Draws the trick boost effect as four textured strips through its stored board quads.
 *
 * Needs at least 2 quads. Uses effect texture 62 and its own draw state; each quad edge pair forms a strip
 * whose v coordinate runs from 1 (newest) to 0, drawn through the graphics manager's virtual slot 71.
 */
extern "C" void cTrickBoostFX_render(sTrail_2EADD0* self)
{
    if (self->count < 2) {
        return;
    }
    push_B198((sRctx_B198*)GrMan);
    *((sRS_B198*)GrMan->m_pCurrentDrawState) = D_00501420_B198;
    int tex = GrMan->_EffectTexMap[62];
    *(short*)((char*)(sRS_B198*)GrMan->m_pCurrentDrawState + 0x10) = tex;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f0 |= 0xC;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 &= 0xFE7FFFFF;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 &= 0xFFCFFFFF;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 = (((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 & 0xFFF00FFF) | 0x14000;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 = (((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 & ~3) | 2;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f8 &= 0xE00003FF;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f8 = (((sRS_B198*)GrMan->m_pCurrentDrawState)->f8 & ~0x3E0) | 0xE0;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 = (((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 & ~0x7C) | 0x1C;
    ((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 = (((sRS_B198*)GrMan->m_pCurrentDrawState)->f4 & 0xFFBFFFFF) | 0x400000;

    sTrailVtx_B198 verts[4][7];
    int counts[4];
    for (int k = 3; k >= 0; k--) {
        counts[k] = 0;
    }
    sTrailCol_B198 col;
    col.a = 1.0f;
    col.r = 1.0f;
    col.g = 1.0f;
    col.b = 1.0f;
    addVtx_B198(&verts[1][counts[1]++], col, 0.5f, 0.0f, self->f0);
    addVtx_B198(&verts[3][counts[3]++], col, 0.5f, 0.0f, self->e0);
    for (int i = 0; i < self->count; i++) {
        int slotIdx = (self->cur + i) % 3;
        col.a = 0.5f;
        col.r = 0.5f;
        col.g = 0.5f;
        col.b = 0.5f;
        float texV = 1.0f - (float)i / (float)(self->count - 1);
        addVtx_B198(&verts[0][counts[0]++], col, 0.0f, texV, self->slots[slotIdx].v[0]);
        addVtx_B198(&verts[0][counts[0]++], col, 1.0f, texV, self->slots[slotIdx].v[1]);
        addVtx_B198(&verts[1][counts[1]++], col, 1.0f, texV, self->slots[slotIdx].v[1]);
        addVtx_B198(&verts[1][counts[1]++], col, 0.0f, texV, self->slots[slotIdx].v[2]);
        addVtx_B198(&verts[2][counts[2]++], col, 0.0f, texV, self->slots[slotIdx].v[2]);
        addVtx_B198(&verts[2][counts[2]++], col, 1.0f, texV, self->slots[slotIdx].v[3]);
        addVtx_B198(&verts[3][counts[3]++], col, 1.0f, texV, self->slots[slotIdx].v[3]);
        addVtx_B198(&verts[3][counts[3]++], col, 0.0f, texV, self->slots[slotIdx].v[0]);
    }
    for (int k = 0; k < 4; k++) {
        cPS2GraphicsMan* ctx = GrMan;
        sRctxVE_B198* vt = (sRctxVE_B198*)ctx->mVtbl;
        vt[71].fn(vthis(ctx, vt[71]), counts[k], verts[k], 0);
    }
    (*(sRS_B198**)&GrMan->m_pCurrentDrawState)--;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", FigureOutTransitionUVs);
#ifdef SKIP_ASM
struct sBTVEntryR {
    short delta;
    short index;
    float* (*fn)(void*);
};

extern float gVStart;
extern float gVEnd;
extern float gVHeight;

/**
 * Reads the screen rectangle from the graphics manager and stores the transition texture coordinates derived from it.
 *
 * Calls cGraphicsMan_TheGraphicsMan's virtual slot 42, which returns a float array r, and sets gVStart = r[1],
 * gVEnd = r[1] + r[2] and gVHeight = r[2]. cFade_TObj_Draw, cWipe_TObj_Draw and cCircle_TObj_Draw use these as
 * the texture rectangle; cTransitionMan_render calls it before drawing.
 */
extern "C" void FigureOutTransitionUVs(void)
{
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    sBTVEntryR* vtEntry = &(((sBTVEntryR*)obj->mVtbl))[42];
    float* screenRect = vtEntry->fn(vthis(obj, *vtEntry));
    float top = screenRect[1];
    float w = screenRect[2];
    gVStart = top;
    float bottom = w + top;
    gVEnd = bottom;
    gVHeight = bottom - top;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB938);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardtrailfx", cFade_TObj_Draw);
#ifdef SKIP_ASM
struct sBTRect4 {
    float x, y, z, w;
};

extern float gUStart;
extern float gUWidth;
extern float gVStart;
extern float gVHeight;

extern "C" void func_002EB938(sBTRect4* pos, sBTRect4* rect, sBTRect4* color, float f);

/**
 * Draws the fade transition: a full-screen (640x480) rectangle in the fade colour with its alpha scaled by @p amount.
 *
 * The texture rectangle is the screen's UV rectangle (gUStart, gVStart, gUWidth, gVHeight); drawn with
 * func_002EB938 (On Tour drawRect).
 *
 * @param amount  how far the fade has covered the screen, 0..1 (On Tour d)
 */
extern "C" void cFade_TObj_Draw(cFade_TObj* self, float amount)
{
    sBTRect4 color = *(sBTRect4*)&self->mColour;
    color.x = ((sBTRect4*)(&self->mColour))->x * amount;
    sBTRect4 pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 640.0f;
    pos.w = 480.0f;
    sBTRect4 rect;
    rect.x = gUStart;
    rect.y = gVStart;
    rect.z = gUWidth;
    rect.w = gVHeight;
    func_002EB938(&pos, &rect, &color, pos.x);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cWipe_TObj_cWipe_TObj);
#ifdef SKIP_ASM
void cTransitionObj_cTransitionObj(void* self);
extern void* cWipe_TObj_vtbl[];

struct cWipe_TObj_cWipe_TObj_sVec4 {
    float x, y, z, w;
};

struct cWipe_TObj_cWipe_TObj_sObj {
    void** vtable;               // 0x0
    cWipe_TObj_cWipe_TObj_sVec4 v;       // 0x4
    int a;                       // 0x14
    int b;                       // 0x18
};

/**
 * Constructs a wipe transition object with a colour and a horizontal and vertical direction.
 *
 * Runs the cTransitionObj base constructor cTransitionObj_cTransitionObj, installs cWipe_TObj_vtbl and stores the colour (+0x4) and the
 * directions (+0x14, +0x18), which cWipe_TObj_Draw uses to slide the rectangle in.
 * cSSXScriptEngine_MakeTransitionObject builds it on 0x1C bytes with (-1, 0), (1, 0), (0, -1) or (0, 1).
 *
 * @param colour  the colour (alpha first)
 * @param horizontalDir  horizontal direction: -1, 0 or 1
 * @param verticalDir  vertical direction: -1, 0 or 1
 * @return @p self
 */
extern "C" cWipe_TObj_cWipe_TObj_sObj* cWipe_TObj_cWipe_TObj(cWipe_TObj* self, const cWipe_TObj_cWipe_TObj_sVec4& colour, int horizontalDir, int verticalDir)
{
    cWipe_TObj_cWipe_TObj_sVec4 vecCopy = colour;
    cTransitionObj_cTransitionObj(self);
    self->mVtbl = cWipe_TObj_vtbl;
    (*(cWipe_TObj_cWipe_TObj_sVec4*)&self->mColour) = vecCopy;
    self->mHDir = horizontalDir;
    self->mVDir = verticalDir;
    return ((cWipe_TObj_cWipe_TObj_sObj*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardtrailfx", cWipe_TObj_Draw);
#ifdef SKIP_ASM
extern float gUStart;
extern float gUEnd;
extern float gUWidth;
extern float gVStart;
extern float gVEnd;
extern float gVHeight;
extern "C" void func_002EB938(sBTRect4* pos, sBTRect4* rect, sBTRect4* color, float f);

/**
 * Draws the wipe transition: a full-screen rectangle in the wipe colour slid in from the side by @p amount.
 *
 * The rectangle is offset by (1 - @p amount) of the screen in the horizontal (+0x14) and vertical (+0x18) directions and
 * cropped to the screen, with its texture rectangle cropped to match; drawn with func_002EB938 (On Tour drawRect).
 *
 * @param amount  how far the wipe has covered the screen, 0..1 (On Tour d)
 */
extern "C" void cWipe_TObj_Draw(cWipe_TObj* self, float amount)
{
    float k = 1.0f - amount;
    float dx = (float)self->mHDir * k * 640.0f;
    float dy = (float)self->mVDir * k * -480.0f;
    float w = 640.0f;
    float uStart, uEnd;
    if (dx < 0.0f) {
        w = dx + w;
        uStart = gUStart + gUWidth * 0.0015625000232830644f * dx;
        uEnd = gUEnd;
        dx = 0.0f;
    } else {
        w = w - dx;
        uStart = gUStart;
        uEnd = uStart + gUWidth * 0.0015625000232830644f * w;
    }
    float height;
    float vStart, vEnd;
    if (dy < 0.0f) {
        height = dy + 480.0f;
        vStart = gVStart + gVHeight * 0.0020833334419876337f * dy;
        vEnd = gVEnd;
        dy = 0.0f;
    } else {
        height = 480.0f - dy;
        vStart = gVStart;
        vEnd = vStart + gVHeight * 0.0020833334419876337f * height;
    }
    float uw = uEnd - uStart;
    float vh = vEnd - vStart;
    sBTRect4 pos;
    pos.x = dx;
    pos.y = dy;
    pos.z = w;
    pos.w = height;
    sBTRect4 rect;
    rect.x = uStart;
    rect.y = vStart;
    rect.z = uw;
    rect.w = vh;
    func_002EB938(&pos, &rect, (sBTRect4*)(&self->mColour), 0.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EBD88);
#ifdef SKIP_ASM

extern void* cCircle_TObj_vtbl[];

/**
 * Constructs a circle transition object with a colour and an explicit circle centre.
 *
 * Runs the cTransitionObj base constructor cTransitionObj_cTransitionObj, installs cCircle_TObj_vtbl and stores the colour (+0x4) and
 * the centre (+0x14, +0x18) that cCircle_TObj_Draw closes the circle around. Nothing in the retail code references it;
 * cCircle_TObj_cCircle_TObj is the variant with a random centre.
 *
 * @param rect  the colour (alpha first)
 * @param posX  centre x in screen units (0..640)
 * @param posY  centre y in screen units (0..480)
 * @return @p self
 */
extern "C" void* func_002EBD88(cCircle_TObj* self, tARGB rect, float posX, float posY)
{
    cTransitionObj_cTransitionObj(self);
    *(void***)self = cCircle_TObj_vtbl;
    *(&self->mScreenColour) = rect;
    self->mOriginX = posX;
    self->mOriginY = posY;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", cCircle_TObj_cCircle_TObj);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
unsigned int BXrand();
// base ctor returns this; the unit declares it void
void* cTransitionObj_cTransitionObj_r(void* self) __asm__("cTransitionObj_cTransitionObj__FPv");
extern void* cCircle_TObj_vtbl[];

struct sBtRectBE20 { float x, y, w, h; };

/**
 * Constructs a circle transition object with a colour and a random circle centre.
 *
 * Runs the cTransitionObj base constructor cTransitionObj_cTransitionObj, installs cCircle_TObj_vtbl, stores the colour (+0x4) and
 * picks the centre with BXrand: x = (rand % 1000) * 0.64 (+0x14) and y = (rand % 1000) * 0.48 (+0x18), so within the
 * 640x480 screen. cSSXScriptEngine_MakeTransitionObject builds it on 0x1C bytes.
 *
 * @param rect  the colour (alpha first)
 * @return @p self
 */
extern "C" void* cCircle_TObj_cCircle_TObj(cCircle_TObj* self, sBtRectBE20 rect)
{
    cTransitionObj_cTransitionObj_r(self);
    *(void***)self = cCircle_TObj_vtbl;
    *(sBtRectBE20*)&self->mScreenColour = rect;
    unsigned int randX = BXrand() % 1000;
    float* outX = (float*)(&self->mOriginX);
    *outX = (float)randX * 0.64000004529953f;
    unsigned int randY = BXrand() % 1000;
    float* outY = (float*)(&self->mOriginY);
    *outY = (float)randY * 0.48000001907348633f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", InitVertex);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sBTQuad_BF48 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sBTVtx_BF48 {
    float x, y, z;
    int pad;
    int r, g, b, a;
    sBTQuad_BF48 st;
};

/**
 * Fills a screen-space vertex for the transition effects from a 2D position and a colour.
 *
 * Colour channels come from @p colour as (a, r, g, b) floats, scaled by 128 when the current draw state has a texture (its
 * +0x10 is not -1) and by 255 otherwise (alpha always by 128). z is 1, the texture coordinates are (x, y, 0, 1) and the
 * position maps @p x over 640 and @p y over 480 into the screen rectangle at gUStart..gVHeight.
 *
 * @param vertex  the vertex to fill (On Tour v)
 * @param colour  the colour, as a, r, g, b floats (On Tour colour)
 * @param x  horizontal position on a 640-wide screen
 * @param y  vertical position on a 480-high screen
 */
extern "C" void InitVertex(sBTVtx_BF48* vertex, float* colour, float x, float y)
{
    if (cGraphicsMan_TheGraphicsMan->m_pCurrentDrawState->m_textureState.m_texid[0] != -1) {
        vertex->r = (int)(colour[1] * 128.0f);
        vertex->g = (int)(colour[2] * 128.0f);
        vertex->b = (int)(colour[3] * 128.0f);
        vertex->a = (int)(colour[0] * 128.0f);
    } else {
        vertex->r = (int)(colour[1] * 255.0f);
        vertex->g = (int)(colour[2] * 255.0f);
        vertex->b = (int)(colour[3] * 255.0f);
        vertex->a = (int)(colour[0] * 128.0f);
    }
    vertex->z = 1.0f;
    sBTQuad_BF48 texCoord;
    texCoord.w = 1.0f;
    texCoord.x = x;
    texCoord.y = y;
    texCoord.z = 0.0f;
    vertex->st = texCoord;
    vertex->x = gUStart + gUWidth * 0.0015625f * x;
    vertex->y = gVStart + gVHeight * 0.0020833334f * y;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardtrailfx", cCircle_TObj_Draw);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern "C" void func_002EB938(sBTRect4* pos, sBTRect4* rect, sBTRect4* color, float f);

extern float gUStart;

extern float gUWidth;

extern float gVStart;

extern float gVHeight;

// InitVertex = On Tour InitVertex(tBxVertex&, float x, float y, const tARGB&); float view of its vertex, same
// parameter order as its definition in this unit (EABI gives the same registers for any interleaving).
extern "C" void InitVertex_C060(float* out, float* col, float x, float y) __asm__("InitVertex");

extern float cSinCosTable_gSinCosTable[];


struct sBTVEntryDraw {
    short delta;
    short index;
    void (*fn)(void*, int, tBxVertex*, int);
};

static inline float clampBT(float v, float lo, float hi)
{
    float r;
    if (lo <= v)
        r = v <? hi;
    else
        r = lo;
    return r;
}

// Points of a half circle of radius r around x = cx, one every `step` radians: ys gets the sine part, xs the
// x position clamped to the screen. cSinCosTable_gSinCosTable is a 512-entry sine table (index = angle * 256/pi, +0x80 = cosine).
static inline void BuildArc_C060(float* ys, float* xs, const float& cx, float r, float step)
{
    float ang = 0.0f;
    for (int i = 0; i < 40; i++, ang += step) {
        int idx = (int)(ang * 81.4873275756836f) & 0x1FF;
        ys[i] = r * cSinCosTable_gSinCosTable[idx];
        float x = cx + r * cSinCosTable_gSinCosTable[idx + 0x80];
        xs[i] = clampBT(x, 0.0f, 640.0f);
    }
}

/**
 * Draws the circle (iris) transition: the screen outside a circle around the origin is covered in the colour.
 *
 * The circle's radius is (1 - @p amount) * 800 around (+0x14, +0x18). Draws the rectangles left and right of it, then
 * the parts above and below the circle as two 80-vertex strips over 40 columns (BuildArc_C060, step pi / 39; y
 * clamped to 0..480) through GrMan slot 71.
 *
 * @param amount  how far the circle has closed, 0..1 (On Tour delta)
 */
extern "C" void cCircle_TObj_Draw(cCircle_TObj* self, float amount)
{
    float radius = (1.0f - amount) * 800.0f;
    float x1 = self->mOriginX + radius;
    float x0 = self->mOriginX - radius;
    sBTRect4 pos;
    sBTRect4 rect;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = x0;
    pos.w = 480.0f;
    rect.z = gUStart + gUWidth * 0.0015625000232830644f * x0;
    rect.x = gUStart;
    rect.y = gVStart;
    rect.w = gVHeight;
    func_002EB938(&pos, &rect, (sBTRect4*)(&self->mScreenColour), 0.0f);
    float w = 640.0f - x1;
    pos.x = x1;
    pos.y = 0.0f;
    pos.z = w;
    pos.w = 480.0f;
    rect.x = gUStart + gUWidth * 0.0015625000232830644f * x1;
    rect.y = gVStart;
    rect.z = gUStart + gUWidth * w * 0.0015625000232830644f;
    rect.w = gVHeight;
    func_002EB938(&pos, &rect, (sBTRect4*)(&self->mScreenColour), 0.0f);

    float ys[40];
    float xs[40];
    BuildArc_C060(ys, xs, self->mOriginX, radius, 0.08055365830659866f); // pi / 39

    tBxVertex top[80];
    tBxVertex bot[80];
    for (int i = 0; i < 40; i++) {
        tBxVertex* topVert = top + i * 2;
        InitVertex_C060(((float*)&topVert->u), (float*)(&self->mScreenColour), xs[i], 0.0f);
        tBxVertex* topArcVert = top + i * 2 + 1;
        float y0 = clampBT(self->mOriginY - ys[i], 0.0f, 480.0f);
        InitVertex_C060(((float*)&topArcVert->u), (float*)(&self->mScreenColour), xs[i], y0);
        float y1 = clampBT(self->mOriginY + ys[i], 0.0f, 480.0f);
        tBxVertex* botArcVert = bot + i * 2;
        InitVertex_C060(((float*)&botArcVert->u), (float*)(&self->mScreenColour), xs[i], y1);
        tBxVertex* botBaseVert = bot + i * 2 + 1;
        InitVertex_C060(((float*)&botBaseVert->u), (float*)(&self->mScreenColour), xs[i], 480.0f);
    }

    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    sBTVEntryDraw* drawEntry = &((sBTVEntryDraw*)obj->mVtbl)[71];
    drawEntry->fn(vthis(obj, *drawEntry), 80, top, 0);
    obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan;
    drawEntry = &((sBTVEntryDraw*)obj->mVtbl)[71];
    drawEntry->fn(vthis(obj, *drawEntry), 80, bot, 0);
}
#endif

