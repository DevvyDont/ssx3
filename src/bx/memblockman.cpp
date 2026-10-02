#include "common.h"

INCLUDE_ASM("bx/memblockman", cMemMan_initialize);

INCLUDE_ASM("bx/memblockman", func_00319A90);

INCLUDE_ASM("bx/memblockman", func_00319B48);

//100%
INCLUDE_ASM("bx/memblockman", func_00319B98);
#ifdef SKIP_ASM
extern "C" int func_0031A130(void* self);
void func_0031FBB8(void* list, void* item);
extern char D_004FF250[];

extern "C" void func_00319B98(void* self)
{
    if (func_0031A130(self) == 0) {
        func_0031FBB8(D_004FF250, self);
    }
}
#endif

INCLUDE_ASM("bx/memblockman", cMemMan_internalResizeBlock);

//100%
INCLUDE_ASM("bx/memblockman", func_00319CC8);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void func_0031A200(void);
extern void* D_00538B00[16];

extern "C" void func_00319CC8(void)
{
    MUTEX_lock(D_00538B00);
    func_0031A200();
    MUTEX_unlock(D_00538B00);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319D10__FPv);
#ifdef SKIP_ASM
void func_00319D10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319D18__FPv);
#ifdef SKIP_ASM
int func_00319D18(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("bx/memblockman", cMemBlockMan_initialize);

INCLUDE_ASM("bx/memblockman", func_00319E48);

INCLUDE_ASM("bx/memblockman", func_00319F68);

INCLUDE_ASM("bx/memblockman", func_0031A088);

INCLUDE_ASM("bx/memblockman", func_0031A130);

INCLUDE_ASM("bx/memblockman", func_0031A200);

INCLUDE_ASM("bx/memblockman", func_0031A268);

//100%
INCLUDE_ASM("bx/memblockman", func_0031A308);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 state save (VF1-VF31, MAC/status/Q/clip flags, ACC) to a 0x210-byte buffer.
extern "C" void func_0031A308(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "cfc2.i     $9, $vi17\n"
        "sqc2       $vf1, 0x0($8)\n"
        "sqc2       $vf2, 0x10($8)\n"
        "sw         $9, 0x200($8)\n"
        "sqc2       $vf3, 0x20($8)\n"
        "sqc2       $vf4, 0x30($8)\n"
        "cfc2.ni    $9, $vi16\n"
        "sqc2       $vf5, 0x40($8)\n"
        "sqc2       $vf6, 0x50($8)\n"
        "sw         $9, 0x204($8)\n"
        "vwaitq\n"
        "cfc2.ni    $9, $vi22\n"
        "sw         $9, 0x208($8)\n"
        "cfc2.ni    $9, $vi18\n"
        "sw         $9, 0x20C($8)\n"
        "sqc2       $vf7, 0x60($8)\n"
        "sqc2       $vf8, 0x70($8)\n"
        "vmaddx.xyzw $vf1, $vf0, $vf0x\n"
        "sqc2       $vf9, 0x80($8)\n"
        "sqc2       $vf10, 0x90($8)\n"
        "sqc2       $vf11, 0xA0($8)\n"
        "sqc2       $vf12, 0xB0($8)\n"
        "sqc2       $vf13, 0xC0($8)\n"
        "sqc2       $vf14, 0xD0($8)\n"
        "sqc2       $vf15, 0xE0($8)\n"
        "sqc2       $vf16, 0xF0($8)\n"
        "sqc2       $vf17, 0x100($8)\n"
        "sqc2       $vf18, 0x110($8)\n"
        "sqc2       $vf19, 0x120($8)\n"
        "sqc2       $vf20, 0x130($8)\n"
        "sqc2       $vf21, 0x140($8)\n"
        "sqc2       $vf22, 0x150($8)\n"
        "sqc2       $vf23, 0x160($8)\n"
        "sqc2       $vf24, 0x170($8)\n"
        "sqc2       $vf25, 0x180($8)\n"
        "sqc2       $vf26, 0x190($8)\n"
        "sqc2       $vf27, 0x1A0($8)\n"
        "sqc2       $vf28, 0x1B0($8)\n"
        "sqc2       $vf29, 0x1C0($8)\n"
        "sqc2       $vf30, 0x1D0($8)\n"
        "sqc2       $vf31, 0x1E0($8)\n"
        "sqc2       $vf1, 0x1F0($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031A3C0);

INCLUDE_ASM("bx/memblockman", func_0031A490);

INCLUDE_ASM("bx/memblockman", func_0031A6B8);

INCLUDE_ASM("bx/memblockman", func_0031A6D8);

INCLUDE_ASM("bx/memblockman", func_0031A920);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031A9D8);
#ifdef SKIP_ASM
extern "C" void func_0031A3C0(void* buf);

extern "C" void func_0031A9D8(void* self)
{
    if (*(int*)((char*)self + 0x14048) != 0) {
        func_0031A3C0((char*)self + 0x14080);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031AA18);
#ifdef SKIP_ASM
extern "C" void func_0031AA18(void* self)
{
    if (*(int*)((char*)self + 0x14048) != 0) {
        func_0031A308((char*)self + 0x14080);
    }
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031AA58);

//100%
INCLUDE_ASM("bx/memblockman", func_0031AAC8);
#ifdef SKIP_ASM
extern "C" void func_00423DC0(int);

extern "C" void func_0031AAC8(void* self)
{
    if (*(int*)((char*)self + 0x1C) != 0) {
        func_00423DC0(*(int*)((char*)self + 0x18));
    }
}
#endif

extern "C" void* func_00423DE0(int);

//100%
INCLUDE_ASM("bx/memblockman", func_0031AAF0__FPv);
#ifdef SKIP_ASM
void* func_0031AAF0(void* self)
{
    return func_00423DE0(*(int*)((char*)self + 0xc));
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031AB10);

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABC0__FPv);
#ifdef SKIP_ASM
void func_0031ABC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABC8__FPv);
#ifdef SKIP_ASM
int func_0031ABC8(void* self)
{
    return 0x3C;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABD0);
#ifdef SKIP_ASM
void* func_00317520();
extern "C" void func_00423DD0(int);

extern "C" void func_0031ABD0(void)
{
    char* app = (char*)func_00317520();
    if (*(int*)(app + 0x4038) == 0) {
        func_00423DD0(*(int*)(app + 0x4034));
    }
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031AC08);

INCLUDE_ASM("bx/memblockman", func_0031AC60);

//100%
INCLUDE_ASM("bx/memblockman", func_0031ACD8);
#ifdef SKIP_ASM
extern "C" void func_00423DC0(int);

extern "C" void func_0031ACD8(void* self, int a1)
{
    *(int*)((char*)self + 0x14048) = a1;
    func_00423DC0(*(int*)((char*)self + 0x14044));
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AD00);
#ifdef SKIP_ASM
extern "C" int func_0031AD00(void* self)
{
    return *(int*)((char*)self + 0x14048) == 0;
}
#endif

extern "C" unsigned int D_004A3ED0;

//99.5% - single $gp-relative sw; SN's assembler doesn't support %gp_rel(),
// so our object always carries a real relocation here while the target's
// raw disassembly has the offset already baked in as a literal immediate.
INCLUDE_ASM("bx/memblockman", func_0031AD18);
#ifdef SKIP_ASM
extern "C" void func_0031AD18()
{
    D_004A3ED0 = 0;
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031AD20);

