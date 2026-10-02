#include "common.h"

INCLUDE_ASM("bx/memblockman", cMemMan_initialize);

//100%
INCLUDE_ASM("bx/memblockman", func_00319A90);
#ifdef SKIP_ASM
extern "C" void* func_00319E48(unsigned int size);
extern "C" void func_003E6220(void* p, unsigned int n);
extern char D_004FF250[];
// PORT: func_00320058__FPv / func_00320078__FPv take (heap, align, size); the defining unit declares 1 arg.
void* func_00320058_K2(void* heap, unsigned int align, unsigned int size) __asm__("func_00320058__FPv");
void* func_00320078_K2(void* heap, unsigned int align, unsigned int size) __asm__("func_00320078__FPv");

extern "C" void* func_00319A90(unsigned int size, unsigned int align, int flags, int top)
{
    void* p;
    if (top == 0 && size != 0 && size <= 0x200) {
        p = func_00319E48(size);
    } else {
        if (align <= 15)
            align = 16;
        if (top != 0)
            p = func_00320078_K2(D_004FF250, align, size);
        else
            p = func_00320058_K2(D_004FF250, align, size);
    }
    if (p != 0 && flags < 0)
        func_003E6220(p, size);
    return p;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319B48);
#ifdef SKIP_ASM
extern "C" int func_0031A088(void* p);
extern "C" void func_00319F68(void* p);
extern char D_004FF250[];
// PORT: func_003200C0__FPv is called with (list, item) here; bind the 2-arg form to that symbol.
int func_003200C0_2(void* list, void* item) __asm__("func_003200C0__FPv");

extern "C" void func_00319B48(void* p)
{
    if (p != 0)
    {
        if (func_0031A088(p) != 0)
        {
            func_00319F68(p);
        }
        else
        {
            func_003200C0_2(D_004FF250, p);
        }
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", cMemMan_internalResizeBlock);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern void* D_00538B00[16];
extern char D_0048DA08[];
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: func_00319B98 returns the block size; the unit declares it void.
extern "C" unsigned int func_00319B98_K2(void* p) __asm__("func_00319B98");

static inline unsigned int cMemMan_blockSizeK2(void* p)
{
    MUTEX_lock(D_00538B00);
    unsigned int n = func_00319B98_K2(p);
    MUTEX_unlock(D_00538B00);
    return n;
}

static inline void cMemMan_freeBlockK2(void* p)
{
    MUTEX_lock(D_00538B00);
    func_00319B48(p);
    MUTEX_unlock(D_00538B00);
}

extern "C" void* cMemMan_internalResizeBlock(void* old, unsigned int size)
{
    void* p = operator_new_K2(size, D_0048DA08, 0x100, 0);
    func_0041605C(p, old, size < cMemMan_blockSizeK2(old) ? size : cMemMan_blockSizeK2(old));
    cMemMan_freeBlockK2(old);
    return p;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031A3C0);
#ifdef SKIP_ASM
extern "C" void func_0031A268(int status);

// PORT: PS2-only VU0 state restore (inverse of func_0031A308): VF1-VF31, flags, ACC.
extern "C" void func_0031A3C0(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "lw         $9, 0x208($8)\n"
        "ctc2.ni    $9, $vi22\n"
        "lw         $9, 0x20C($8)\n"
        "ctc2.ni    $9, $vi18\n"
        "lqc2       $vf1, 0x1F0($8)\n"
        "vaddax.xyzw ACC, $vf1, $vf0x\n"
        "lqc2       $vf1, 0x0($8)\n"
        "lqc2       $vf2, 0x10($8)\n"
        "lqc2       $vf3, 0x20($8)\n"
        "lqc2       $vf4, 0x30($8)\n"
        "lqc2       $vf5, 0x40($8)\n"
        "lqc2       $vf6, 0x50($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
    func_0031A268(*(int*)((char*)buf + 0x200));
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "lqc2       $vf7, 0x60($8)\n"
        "lqc2       $vf8, 0x70($8)\n"
        "lqc2       $vf9, 0x80($8)\n"
        "lqc2       $vf10, 0x90($8)\n"
        "lqc2       $vf11, 0xA0($8)\n"
        "lqc2       $vf12, 0xB0($8)\n"
        "lw         $9, 0x204($8)\n"
        "lqc2       $vf13, 0xC0($8)\n"
        "lqc2       $vf14, 0xD0($8)\n"
        "ctc2.ni    $9, $vi16\n"
        "lqc2       $vf15, 0xE0($8)\n"
        "lqc2       $vf16, 0xF0($8)\n"
        "lqc2       $vf17, 0x100($8)\n"
        "lqc2       $vf18, 0x110($8)\n"
        "lqc2       $vf19, 0x120($8)\n"
        "lqc2       $vf20, 0x130($8)\n"
        "lqc2       $vf21, 0x140($8)\n"
        "lqc2       $vf22, 0x150($8)\n"
        "lqc2       $vf23, 0x160($8)\n"
        "lqc2       $vf24, 0x170($8)\n"
        "lqc2       $vf25, 0x180($8)\n"
        "lqc2       $vf26, 0x190($8)\n"
        "lqc2       $vf27, 0x1A0($8)\n"
        "lqc2       $vf28, 0x1B0($8)\n"
        "lqc2       $vf29, 0x1C0($8)\n"
        "lqc2       $vf30, 0x1D0($8)\n"
        "lqc2       $vf31, 0x1E0($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

INCLUDE_ASM("bx/memblockman", func_0031A490);

INCLUDE_ASM("bx/memblockman", func_0031A6B8);

INCLUDE_ASM("bx/memblockman", func_0031A6D8);

//100%
INCLUDE_ASM("bx/memblockman", func_0031A920);
#ifdef SKIP_ASM
extern "C" void func_00424880(int);
extern "C" void func_00423AA0(int, int);
extern "C" int func_004248E8(int);
extern "C" void func_00423DC0(int);
extern "C" void func_00423CC0();
extern "C" void func_00423BF0(int);
extern "C" void func_00423BB0(int);
extern "C" void func_00423DB0(int);

extern "C" void func_0031A920(void* self)
{
    func_00424880(3);
    func_00423AA0(3, *(int*)((char*)self + 0x14290));
    func_004248E8(3);
    *(int*)((char*)self + 0x20) = 1;
    func_00423DC0(*(int*)((char*)self + 0x14044));
    func_00423DC0(*(int*)((char*)self + 0x4034));
    func_00423CC0();
    func_00423CC0();
    func_00423BF0(*(int*)((char*)self + 0x4030));
    func_00423BB0(*(int*)((char*)self + 0x4030));
    func_00423BF0(*(int*)((char*)self + 0x14040));
    func_00423BB0(*(int*)((char*)self + 0x14040));
    func_00423DB0(*(int*)((char*)self + 0x4034));
    func_00423DB0(*(int*)((char*)self + 0x14044));
    func_00423DB0(*(int*)((char*)self + 0x18));
    func_00423DB0(*(int*)((char*)self + 0xC));
}
#endif

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

//100%
INCLUDE_ASM("bx/memblockman", func_0031AA58);
#ifdef SKIP_ASM
extern "C" void* func_00423DE0(int);
extern "C" int func_00423DF0(int);

struct sMemBlkVEntryA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMemBlkVEntryB {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0031AA58(void* self)
{
    *(int*)((char*)self + 0x1C) = 1;
    sMemBlkVEntryA* vt = *(sMemBlkVEntryA**)((char*)self + 0x8);
    vt[11].fn((char*)self + vt[11].delta);
    func_00423DE0(*(int*)((char*)self + 0x18));
    *(int*)((char*)self + 0x1C) = 0;
    while (func_00423DF0(*(int*)((char*)self + 0x18)) == *(int*)((char*)self + 0x18))
    {
    }
    sMemBlkVEntryB* vt2 = *(sMemBlkVEntryB**)((char*)self + 0x8);
    vt2[12].fn((char*)self + vt2[12].delta);
}
#endif

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

//100%
INCLUDE_ASM("bx/memblockman", func_0031AC08);
#ifdef SKIP_ASM
extern "C" void* func_00423DE0(int);
extern "C" void func_00423CD0(int);
extern "C" void func_00423CC0();
// PORT: func_00317500__Fv is called with self here; bind the 1-arg form to that symbol.
int func_00317500_1(void* self) __asm__("func_00317500__Fv");

extern "C" void func_0031AC08(void* self)
{
    while (func_00423DE0(*(int*)((char*)self + 0x4034)), *(int*)((char*)self + 0x20) == 0)
    {
        func_00317500_1(self);
    }
    func_00423CD0(*(int*)((char*)self + 0x10));
    func_00423CC0();
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AC60);
#ifdef SKIP_ASM
struct sMemJobVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0031AC60(void* self)
{
    while (func_00423DE0(*(int*)((char*)self + 0x14044)), *(int*)((char*)self + 0x20) == 0)
    {
        void* obj = *(void* volatile*)((char*)self + 0x14048);
        sMemJobVEntry* vt = *(sMemJobVEntry**)obj;
        vt[2].fn((char*)obj + vt[2].delta);
        *(void* volatile*)((char*)self + 0x14048) = 0;
    }
    func_00423CD0(*(int*)((char*)self + 0x10));
    func_00423CC0();
}
#endif

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

