#include "common.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("camera/cameraalgorithms", cChaseCameraController_createChaseAlgorithmBlend);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern char D_0045B250[], D_0045B278[], D_0045B298[], D_0045B2B0[], D_0045B2D8[], str_DEFAULT_4_Chase_Camera_Algorithm[];
extern char D_0045B328[], D_0045B348[], D_0045B370[], D_0045B390[], str_Lazy_Chase_Camera_Algorithm[];
extern char D_0045B3D0[], D_0045B3F8[], D_0045B420[], D_0045B448[], D_0045B470[], D_0045B498[], D_0045B4C0[], str_Direction_NE_Chase_Camera_Algorithm[];
extern char D_0045B510[], D_0045B538[], D_0045B558[], D_0045B578[], str_POST_RACE_1_Chase_Camera_Algorithm[];

struct sCamTmp_D0A0 { float x, y, z, w; } __attribute__((aligned(16)));

class cCamAlgo_D0A0 {
public:
    char pad[0x10];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void activate();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void setTarget(void* target);
};

extern "C" void func_00170240(sCamTmp_D0A0* out, void* target);
extern "C" void func_001707B0(sCamTmp_D0A0* out, void* target);
extern "C" void func_00170D20(sCamTmp_D0A0* out, void* target, int corner);
extern "C" cCamAlgo_D0A0* cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(void* mem, sCamTmp_D0A0* a, sCamTmp_D0A0* b);
extern "C" cCamAlgo_D0A0* func_00174190(void* mem);
extern "C" cCamAlgo_D0A0* cSplineCamAlgorithm_cSplineCamAlgorithm(void* mem, sCamTmp_D0A0* a, sCamTmp_D0A0* b, sCamTmp_D0A0* c, sCamTmp_D0A0* d,
                               float f0, float f1, float f2, float f3);
extern "C" cCamAlgo_D0A0* cIdleChaseAlgorithm_cIdleChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cManualChaseAlgorithm_cManualChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cLazyChaseAlgorithm_cLazyChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(void* mem, int index);
extern "C" cCamAlgo_D0A0* cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(void* mem, float angle);
extern "C" cCamAlgo_D0A0* cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(void* mem, void* target);
extern "C" void cChaseCameraController_addAlgorithm(char* self, cCamAlgo_D0A0* algo, float blend);

static inline int camNetActive_D0A0(int n) { return n != 0 && n < 10; }

/**
 * Creates the chase-camera algorithm for a camera type, aims it at the controller's target and blends it in.
 *
 * Types include idle, mid left and right, manual, lazy, look back, matrix, eight "Direction N" and
 * relative-angle
 * cameras, superpipe (91, falling back to a bounded camera), spline (92) and post-race; most others use
 * DEFAULT_3.
 * Type 67 and unlisted types leave no algorithm and then call through a null pointer.
 *
 * @param kind  the camera algorithm type (On Tour eCamAlgoType type)
 * @param ctx  the camera action that asked for it (On Tour cCTAction* action)
 * @param blend  blend rate; bit 1 of the controller's flags (+0x30) forces an instant switch (1.0)
 */
// Creates the chase-camera algorithm for the given kind and blends it in.
extern "C" void cChaseCameraController_createChaseAlgorithmBlend(char* self, int kind, char* ctx, float blend)
{
    cCamAlgo_D0A0* algo = 0;
    float blendRate = blend;
    if (*(int*)(self + 0x30) & 2) {
        blendRate = 1.0f;
    }
    switch (kind) {
    case 91: {
        int n = *(int*)SSXApp->Game->Replay;
        if (!camNetActive_D0A0(n)) {
            char* target = *(char**)(ctx + 8);
            if (*(int*)(target + 0x28) == 2) {
                void* mem = cMemMan_alloc(0x3B0, D_0045B250, 0x20000000, 0);
                sCamTmp_D0A0 lineStart;
                func_00170240(&lineStart, target);
                sCamTmp_D0A0 lineEnd;
                func_001707B0(&lineEnd, target);
                algo = cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(mem, &lineStart, &lineEnd);
                break;
            }
        }
        algo = func_00174190(cMemMan_alloc(0x90, D_0045B278, 0x20000000, 0));
        *(char**)((char*)algo + 0x80) = ctx;
        break;
    }
    case 92: {
        char* target = *(char**)(ctx + 8);
        void* mem = cMemMan_alloc(0x3D0, D_0045B298, 0x20000000, 0);
        sCamTmp_D0A0 c0;
        func_00170D20(&c0, target, 0);
        sCamTmp_D0A0 c1;
        func_00170D20(&c1, target, 1);
        sCamTmp_D0A0 c2;
        func_00170D20(&c2, target, 2);
        sCamTmp_D0A0 c3;
        func_00170D20(&c3, target, 3);
        algo = cSplineCamAlgorithm_cSplineCamAlgorithm(mem, &c0, &c1, &c2, &c3,
                          *(float*)(ctx + 0x18), *(float*)(ctx + 0x14), *(float*)(ctx + 0x1C), *(float*)(ctx + 0x10));
        break;
    }
    case 7:
        algo = cIdleChaseAlgorithm_cIdleChaseAlgorithm(cMemMan_alloc(0x390, D_0045B328, 0x20000000, 0));
        break;
    case 9:
        algo = cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(cMemMan_alloc(0x390, D_0045B348, 0x20000000, 0));
        break;
    case 10:
        algo = cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(cMemMan_alloc(0x390, D_0045B370, 0x20000000, 0));
        break;
    case 11:
        algo = cManualChaseAlgorithm_cManualChaseAlgorithm(cMemMan_alloc(0x390, D_0045B390, 0x20000000, 0));
        break;
    case 13:
        algo = cLazyChaseAlgorithm_cLazyChaseAlgorithm(cMemMan_alloc(0x390, str_Lazy_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case 18:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B3D0, 0x20000000, 0), 0);
        break;
    case 19:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B3F8, 0x20000000, 0), 1);
        break;
    case 20:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B420, 0x20000000, 0), 2);
        break;
    case 21:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B448, 0x20000000, 0), 3);
        break;
    case 22:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B470, 0x20000000, 0), 4);
        break;
    case 23:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B498, 0x20000000, 0), 5);
        break;
    case 24:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B4C0, 0x20000000, 0), 6);
        break;
    case 25:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(0x3A0, str_Direction_NE_Chase_Camera_Algorithm, 0x20000000, 0), 7);
        break;
    case 32:
        algo = cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(cMemMan_alloc(0x390, D_0045B510, 0x20000000, 0));
        break;
    case 41:
        algo = cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B538, 0x20000000, 0));
        break;
    case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40: {
        float angle = (float)(kind - 0x21) * 0.7853982f;
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), angle);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    case 2: case 60:
        algo = cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(cMemMan_alloc(0x390, D_0045B2D8, 0x20000000, 0));
        break;
    case 1: case 62:
        algo = cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(cMemMan_alloc(0x390, str_DEFAULT_4_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case 0: case 3: case 4: case 5: case 6: case 8: case 12: case 14: case 15: case 16: case 17:
    case 26: case 27: case 28: case 29: case 30: case 31:
    case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49: case 50:
    case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58: case 59:
    case 61: case 63: case 64: case 65: case 74:
        algo = cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(cMemMan_alloc(0x390, D_0045B2B0, 0x20000000, 0));
        break;
    case 66:
        algo = cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(cMemMan_alloc(0x3A0, D_0045B578, 0x20000000, 0));
        break;
    case 67:
        algo = 0;
        break;
    case 68:
        algo = cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(cMemMan_alloc(0x3B0, str_POST_RACE_1_Chase_Camera_Algorithm, 0x20000000, 0), *(void**)(self + 0x20));
        break;
    case 94:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 95:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 96:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 97:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    algo->setTarget(*(void**)(self + 0x20));
    cChaseCameraController_addAlgorithm(self, algo, blendRate);
    algo->activate();
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D698);
#ifdef SKIP_ASM
struct func_0015D698_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* cSimpleCamera_vtbl[];
extern func_0015D698_sVec4 D_004FF140;

extern "C" void* func_0015D698(void* self)
{
    func_0015D698_sVec4 v;
    *(void***)((char*)self + 0x90) = cSimpleCamera_vtbl;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D700);
#ifdef SKIP_ASM
extern "C" void* func_0015D928(void* self);

extern "C" void func_0015D700(void* self, float x, float y, float z)
{
    func_0015D698_sVec4 v;
    *(float*)((char*)self + 0xC) = x;
    *(float*)((char*)self + 0x10) = y;
    *(float*)((char*)self + 0x14) = z;
    *(float*)((char*)self + 0x0) = x;
    *(float*)((char*)self + 0x4) = y;
    *(float*)((char*)self + 0x8) = z;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    func_0015D928(self);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D780);
#ifdef SKIP_ASM
struct func_0015D780_V { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" float func_0031C228(float);

static inline float atan2_15D780(float y, float x, float zero)
{
    if (x == zero) {
        if (y == zero)
            return y;
        return zero <= y ? 1.5707963705062866f : -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < zero) {
        if (zero < y)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

extern "C" float func_0015D780(void* self)
{
    func_0015D780_V v;
    float x = 2.0f * (*(float*)((char*)self + 0x34) * *(float*)((char*)self + 0x30) - *(float*)((char*)self + 0x3C) * *(float*)((char*)self + 0x38));
    float y = 1.0f - 2.0f * (*(float*)((char*)self + 0x38) * *(float*)((char*)self + 0x38) + *(float*)((char*)self + 0x30) * *(float*)((char*)self + 0x30));
    float z = 2.0f * (*(float*)((char*)self + 0x34) * *(float*)((char*)self + 0x38) + *(float*)((char*)self + 0x3C) * *(float*)((char*)self + 0x30));
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = 0;
    float w = v.w;
    return atan2_15D780(y, x, w);
}
#endif
//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D880);
#ifdef SKIP_ASM
struct func_0015D880_V { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float func_0015D880_Length(const func_0015D880_V& v)
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

extern "C" float func_0015D880(void* self)
{
    func_0015D880_V v;
    v.x = 2.0f * (*(float*)((char*)self + 0x34) * *(float*)((char*)self + 0x30) - *(float*)((char*)self + 0x3C) * *(float*)((char*)self + 0x38));
    v.y = 1.0f - 2.0f * (*(float*)((char*)self + 0x38) * *(float*)((char*)self + 0x38) + *(float*)((char*)self + 0x30) * *(float*)((char*)self + 0x30));
    v.z = 2.0f * (*(float*)((char*)self + 0x34) * *(float*)((char*)self + 0x38) + *(float*)((char*)self + 0x3C) * *(float*)((char*)self + 0x30));
    v.w = 0;
    return func_0031C128(v.z / func_0015D880_Length(v));
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D928);
#ifdef SKIP_ASM
struct sV_D928 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_D928 {
    float m[4][4];
} __attribute__((aligned(16)));

extern char D_004C53A0[];

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_D928(void* dst, void* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_D928(void* dst, void* a, void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (unit quaternion -> rotation matrix, row 3 = identity).
static inline void vu0QuatToMtx_D928(void* m, const sV_D928& q)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, %1\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_D928(void* m, const sV_D928& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

extern "C" void func_0015D928_impl(void* self) __asm__("func_0015D928");
extern "C" void func_0015D928_impl(void* self)
{
    char* view = (char*)self + 0x40;
    vu0CopyMtx_D928(view, D_004C53A0);
    sM_D928 rot;
    vu0QuatToMtx_D928(&rot, *(sV_D928*)((char*)self + 0x30));
    vu0MulMtx_D928(view, view, &rot);
    sV_D928 t;
    sV_D928 neg;
    sV_D928 pos;
    pos.x = *(float*)((char*)self + 0x20);
    pos.y = *(float*)((char*)self + 0x24);
    pos.z = *(float*)((char*)self + 0x28);
    neg.x = -pos.x;
    neg.y = -pos.y;
    neg.z = -pos.z;
    t.x = neg.x;
    t.y = neg.y;
    t.z = neg.z;
    t.w = 1.0f;
    vu0TransMtx_D928(view, t);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DAC0);
#ifdef SKIP_ASM

extern "C" void func_0015DAC0(void)
{
    void* cam = SSXApp->Game;
    void* chase = *(void**)((char*)cam + 0x84);
    void* ctrl = *(void**)((char*)chase + 0x4);
    *(int*)((char*)ctrl + 0x4A4) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DAE0);
#ifdef SKIP_ASM
struct func_0015DAE0_VE { short delta; short index; void (*fn)(void*); };

extern "C" void func_0015DAE0(void)
{
    void* cam = SSXApp->Game;
    char* ctx = *(char**)((char*)cam + 0x84);
    int count = *(int*)ctx;
    char* a = *(char**)(*(char**)(ctx + 4) + 0xA8);
    func_0015DAE0_VE* e = &(*(func_0015DAE0_VE**)(a + 0x14))[4];
    e->fn(a + e->delta);
    if (count >= 2) {
        char* b = *(char**)(*(char**)(ctx + 8) + 0xA8);
        func_0015DAE0_VE* e2 = &(*(func_0015DAE0_VE**)(b + 0x14))[4];
        e2->fn(b + e2->delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DB58);
#ifdef SKIP_ASM
extern "C" void func_0016C968(void* self);
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void cChaseCameraController_switchCamera_Restart(void* cam);
extern char D_004C5830[];

struct sCamVE_0015DB58 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0015DB58(void)
{
    func_0016C968(D_004C5830);
    cCameraTriggerMan_setInGameTriggers(D_004C5830);
    char* riders = (char*)SSXApp->Game->ViewMan;
    int n = *(int*)riders;
    cChaseCameraController_switchCamera_Restart(*(void**)(*(char**)(riders + 4) + 0xA8));
    char* cam = *(char**)(*(char**)(riders + 4) + 0xA8);
    sCamVE_0015DB58* e = &(*(sCamVE_0015DB58**)(cam + 0x14))[4];
    e->fn(cam + e->delta);
    *(int*)(*(char**)(riders + 4) + 0x460) = 0;
    *(int*)(*(char**)(riders + 4) + 0x4A4) = 1;
    if (n >= 2)
    {
        cChaseCameraController_switchCamera_Restart(*(void**)(*(char**)(riders + 8) + 0xA8));
        char* cam2 = *(char**)(*(char**)(riders + 8) + 0xA8);
        sCamVE_0015DB58* e2 = &(*(sCamVE_0015DB58**)(cam2 + 0x14))[4];
        e2->fn(cam2 + e2->delta);
        *(int*)(*(char**)(riders + 8) + 0x460) = 0;
    }
}
#endif

