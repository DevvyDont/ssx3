#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00492EE0[];
extern void* D_00493650[16];

struct cPSPLightMan;
cPSPLightMan* cPSPLightMan_cPSPLightMan(cPSPLightMan* self);

//100%
INCLUDE_ASM("render/lightman", cLightMan_construct__Fv);
#ifdef SKIP_ASM
void* cLightMan_construct()
{
    void* mem = cMemMan_alloc(0x14, D_00492EE0, 0, 0);
    cPSPLightMan_cPSPLightMan((cPSPLightMan*)mem);
    *(void**)mem = D_00493650;
    return mem;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC50__FPv);
#ifdef SKIP_ASM
void func_0038DC50(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC58__FPv);
#ifdef SKIP_ASM
void func_0038DC58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC60__FPv);
#ifdef SKIP_ASM
void func_0038DC60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC68__FPv);
#ifdef SKIP_ASM
void func_0038DC68(void* self)
{
}
#endif

INCLUDE_ASM("render/lightman", func_0038DC70);

INCLUDE_ASM("render/lightman", func_0038DEE8);

INCLUDE_ASM("render/lightman", func_0038DF38);

INCLUDE_ASM("render/lightman", func_0038DF98);

INCLUDE_ASM("render/lightman", func_0038E008);

INCLUDE_ASM("render/lightman", func_0038EC40);

INCLUDE_ASM("render/lightman", func_0038EE78);

INCLUDE_ASM("render/lightman", func_0038F2A8);

INCLUDE_ASM("render/lightman", func_0038F300);

INCLUDE_ASM("render/lightman", func_0038F460);

INCLUDE_ASM("render/lightman", func_0038F4F8);

INCLUDE_ASM("render/lightman", func_0038F598);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", func_0038F668);
#ifdef SKIP_ASM
struct sLmStack {
    char pad_0x00[0xC];
    int depth;          // 0x0C
    char pad_0x10[4];
    int marks[1];       // 0x14
};

extern "C" void func_0038F598(sLmStack* self, int size, int arg, int base);

extern "C" void func_0038F668(sLmStack* self, int end, int arg)
{
    int base = self->marks[self->depth];
    int size = end - base;
    func_0038F598(self, size + (-size & 0xF), arg, base);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F6A8);
#ifdef SKIP_ASM
extern "C" void func_0038F6A8(void* self)
{
    *(int*)((char*)self + 0xC) = (*(int*)((char*)self + 0xC) + 1) % *(int*)((char*)self + 0x4);
}
#endif

INCLUDE_ASM("render/lightman", func_0038F708);

INCLUDE_ASM("render/lightman", func_0038F738);

//100%
INCLUDE_ASM("render/lightman", func_0038F768);
#ifdef SKIP_ASM
// PORT: polls PS2 hardware register 0x1000D000 directly; needs a platform shim.
extern "C" void func_0038F768(void)
{
    while (*(volatile int*)0x1000D000 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F7B0);
#ifdef SKIP_ASM
// PORT: polls PS2 hardware register 0x1000D400 directly; needs a platform shim.
extern "C" void func_0038F7B0(void)
{
    while (*(volatile int*)0x1000D400 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F7F8__FPv);
#ifdef SKIP_ASM
void func_0038F7F8(void* self)
{
}
#endif

extern "C" void* func_002BB6B8(void* self);

//100%
INCLUDE_ASM("render/lightman", func_0038F800__FPv);
#ifdef SKIP_ASM
void* func_0038F800(void* self)
{
    return func_002BB6B8(self);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F930);
#ifdef SKIP_ASM
extern int D_0044BDE0[];

extern "C" int func_0038F930(void* self, unsigned char* src, unsigned char* dst)
{
    unsigned int n = 0;
    unsigned int i;
    for (i = 0; i < 4; i++) {
        unsigned int k = (i & 1) * 64;
        unsigned int j;
        for (j = 0; j < 16; j++) {
            unsigned int m;
            for (m = 0; m < 4; m++) {
                dst[n] = src[D_0044BDE0[k]];
                k++;
                n++;
            }
        }
        src += 0x40;
    }
    return 0;
}
#endif

INCLUDE_ASM("render/lightman", func_0038FC48);

INCLUDE_ASM("render/lightman", func_00390198);

INCLUDE_ASM("render/lightman", func_00390458);

INCLUDE_ASM("render/lightman", func_003904A0);

INCLUDE_ASM("render/lightman", func_003905E8);

INCLUDE_ASM("render/lightman", func_00390C20);

INCLUDE_ASM("render/lightman", func_00390C60);

INCLUDE_ASM("render/lightman", func_00390EC8);

INCLUDE_ASM("render/lightman", func_00390EF8);

INCLUDE_ASM("render/lightman", func_00390F20);

INCLUDE_ASM("render/lightman", func_003912A8);

INCLUDE_ASM("render/lightman", func_00391360);

//100%
INCLUDE_ASM("render/lightman", func_00391418);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (lqc2 x16 into vf1-vf16, then vcallmsr of the
// microprogram at VU0 micro-memory address 0); the PC port needs a C version.
extern "C" void func_00391418(void* self, void* m)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
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
        "lui       $2, 0\n"
        "addiu     $2, $2, 0\n"
        "srl       $2, $2, 3\n"
        "ctc2.ni   $2, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        ".set pop\n"
        :
        : "r"(m)
        : "$2", "memory");
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00391480);
#ifdef SKIP_ASM
struct sQuad16 {
    int w[4];
} __attribute__((aligned(16)));

extern "C" void func_00391480(void* self, sQuad16* dst)
{
    sQuad16* src = (sQuad16*)0x11004200;
    int i;
    for (i = 0; i < 10; i++) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        dst[8] = src[8];
        dst[9] = src[9];
        src += 10;
        dst += 10;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_003914F8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 integer register read (vi1); the PC port needs a C fallback.
extern "C" int func_003914F8(void)
{
    int r;
    __asm__ __volatile__("cfc2.i %0, $vi1" : "=r"(r));
    return r;
}
#endif

INCLUDE_ASM("render/lightman", func_003915E8);

INCLUDE_ASM("render/lightman", func_003916C0);

INCLUDE_ASM("render/lightman", func_00391708);

