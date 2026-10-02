#include "common.h"

INCLUDE_ASM("bx/ps2main", systemInit);

INCLUDE_ASM("bx/ps2main", main);

INCLUDE_ASM("bx/ps2main", func_0031B008);

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

INCLUDE_ASM("bx/ps2main", func_0031BF60);

INCLUDE_ASM("bx/ps2main", func_0031C040);

INCLUDE_ASM("bx/ps2main", func_0031C128);

INCLUDE_ASM("bx/ps2main", func_0031C228);

