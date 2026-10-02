#include "common.h"

INCLUDE_ASM("bx/ps2main", systemInit);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", main);
#ifdef SKIP_ASM
struct cExecutionMan;
struct cExecutionManMainK {
    int field_0x0;
    int field_0x4;
    void* field_0x8; // vtable
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
cExecutionMan* cExecutionMan_halt(cExecutionMan* self);
extern "C" void func_0040FC90(void); // __main
extern "C" void systemInit(void);
extern "C" int cAppMan_run(void* self, cExecutionMan* exec, int argc, char** argv);
extern void* D_004A5B64;
extern const char D_0048DAE8[];
extern void* D_0048DB68[];

// PORT: bound to "main" by asm label so gcc doesn't emit its own __main call;
// the target calls func_0040FC90 (__main) explicitly. Off-PS2 this is plain main().
int main_impl(int argc, char** argv) __asm__("main");

int main_impl(int argc, char** argv)
{
    func_0040FC90();
    systemInit();
    cExecutionManMainK* exec = (cExecutionManMainK*)cMemMan_alloc(0x142C0, D_0048DAE8, 0, 0);
    cExecutionMan_halt((cExecutionMan*)exec);
    exec->field_0x8 = D_0048DB68;
    return cAppMan_run(D_004A5B64, (cExecutionMan*)exec, argc, argv);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", func_0031B008);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cAppManB008 {
public:
    char pad_0x00[0x5C];
    virtual void v01();
    virtual void v02();
};
// Typed view of D_004A5B64 (main() in this unit declares it as void*).
extern cAppManB008* D_004A5B64_B008 __asm__("D_004A5B64");
extern "C" void func_00402450(int);
extern "C" void func_00401718(int);
extern "C" void func_00425FB8(void);
extern "C" void func_0042C6A0(int, int, int);

extern "C" void func_0031B008(int a0, int a1, int a2)
{
    D_004A5B64_B008->v02();
    func_00402450(2);
    func_00401718(0);
    func_00401718(5);
    func_00425FB8();
    func_0042C6A0(a0, a1, a2);
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", func_0031B088);
#ifdef SKIP_ASM
extern "C" float func_0031B088(float a, float b, float c, float d)
{
    return a * d - b * c;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", func_0031B098);
#ifdef SKIP_ASM
extern "C" float func_0031B088(float a, float b, float c, float d);

// 3x3 determinant by cofactor expansion along the first column.
extern "C" float func_0031B098(float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
    return a * func_0031B088(e, f, h, i) - d * func_0031B088(b, c, h, i) + g * func_0031B088(b, c, e, f);
}
#endif

INCLUDE_ASM("bx/ps2main", func_0031B178);

INCLUDE_ASM("bx/ps2main", func_0031B310);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/ps2main", func_0031B6C8);
#ifdef SKIP_ASM
extern "C" float func_0031B178(void* m);
extern "C" void func_0031B310(void* m);

// Matrix inverse: adjoint (func_0031B310) scaled by 1/determinant (func_0031B178).
// PORT: PS2-only VU0 inline asm (4x4 matrix times scalar, in place).
extern "C" void func_0031B6C8(void* m)
{
    float det = func_0031B178(m);
    func_0031B310(m);
    float s = 1.0f / det;
    int t;
    __asm__ __volatile__(
        "mfc1      %0, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "qmtc2.ni  %0, $vf3\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulx.xyzw $vf8, $vf4, $vf3x\n"
        "vmulx.xyzw $vf9, $vf5, $vf3x\n"
        "vmulx.xyzw $vf10, $vf6, $vf3x\n"
        "vmulx.xyzw $vf11, $vf7, $vf3x\n"
        "sqc2      $vf8, 0x0(%1)\n"
        "sqc2      $vf9, 0x10(%1)\n"
        "sqc2      $vf10, 0x20(%1)\n"
        "sqc2      $vf11, 0x30(%1)\n"
        : "=&r"(t)
        : "r"(m), "f"(s)
        : "memory");
}
#endif

INCLUDE_ASM("bx/ps2main", func_0031B748);

INCLUDE_ASM("bx/ps2main", func_0031B7A8);

INCLUDE_ASM("bx/ps2main", func_0031BB30);

INCLUDE_ASM("bx/ps2main", func_0031BCB0);

INCLUDE_ASM("bx/ps2main", func_0031BE50);

//100%
INCLUDE_ASM("bx/ps2main", func_0031BF60);
#ifdef SKIP_ASM
// sin(x): quadrant reduction by pi/2, then a cos or sin polynomial (Horner form).
extern "C" float func_0031BF60(float x)
{
    float t = x * 0.6366197466850281f;
    if (x < 0.0f)
        t -= 0.5f;
    else
        t += 0.5f;
    int q;
    float fq;
    // PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
    __asm__("cvt.w.s %0, %0\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "+f"(t), "=r"(q));
    fq = t;
    x = x - fq * 1.5707963705062866f;
    float x2 = x * x;
    float r;
    if (q & 1)
    {
        r = x2 * 2.480159128026571e-05f;
        r = (r + -0.0013888890389353037f) * x2;
        r = (r + 0.0416666679084301f) * x2;
        r = (r + -0.5f) * x2;
        r = r + 1.0f;
    }
    else
    {
        r = x2 * 2.755732339210226e-06f;
        r = (r + -0.0001984127302421257f) * x2;
        r = (r + 0.008333333767950535f) * x2;
        r = (r + -0.1666666716337204f) * x2;
        r = r * x + x;
    }
    if (q & 2)
        r = -r;
    return r;
}
#endif

//100%
INCLUDE_ASM("bx/ps2main", func_0031C040);
#ifdef SKIP_ASM
// cos(x): quadrant reduction by pi/2, then a sin or cos polynomial (Horner form).
extern "C" float func_0031C040(float x)
{
    float t = x * 0.6366197466850281f;
    if (x < 0.0f)
        t -= 0.5f;
    else
        t += 0.5f;
    int q;
    float fq;
    // PORT: PS2 float->int->float round trip kept in the FPU (cvt.w.s / mfc1 / cvt.s.w).
    __asm__("cvt.w.s %0, %0\n\tmfc1 %1, %0\n\tcvt.s.w %0, %0" : "+f"(t), "=r"(q));
    fq = t;
    x = x - fq * 1.5707963705062866f;
    float x2 = x * x;
    float r;
    if (q & 1)
    {
        r = x2 * 2.755732339210226e-06f;
        r = (r + -0.0001984127302421257f) * x2;
        r = (r + 0.008333333767950535f) * x2;
        r = (r + -0.1666666716337204f) * x2;
        r = r * x + x;
    }
    else
    {
        r = x2 * 2.480159128026571e-05f;
        r = (r + -0.0013888890389353037f) * x2;
        r = (r + 0.0416666679084301f) * x2;
        r = (r + -0.5f) * x2;
        r = r + 1.0f;
    }
    if ((q + 1) & 2)
        r = -r;
    return r;
}
#endif

INCLUDE_ASM("bx/ps2main", func_0031C128);

INCLUDE_ASM("bx/ps2main", func_0031C228);

