#include "common.h"

INCLUDE_ASM("fe/all_in_one_fe", __sti__all_in_one_fe_cpp);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C640);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_xi(void*, int) __asm__("func_0039E2A0");
extern char D_00474E08[];
extern "C" void* func_0021C640(void *self) {
    func_0039E2A0_xi(self, 0);
    *(char**)((char*)self + 8) = D_00474E08;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C678);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021C678(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C6D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C6D8(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C700);
#ifdef SKIP_ASM
extern "C" int func_0021C700(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x4C)));
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C708(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C730);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00474D30[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021C730(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00474D30;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C798);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C798(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C7C0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00474C58[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021C7C0(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00474C58;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C828);
#ifdef SKIP_ASM
extern "C" void func_0020E900();

extern "C" void func_0021C828(void) {
    func_0020E900();
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C848);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C848(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C870);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C870(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C898);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00474AA8[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021C898(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00474AA8;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C900);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C900(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C928);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C928(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021C950);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C9C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C9C0(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C9E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021C9E8(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CA10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CA10(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CA38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CA38(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CA60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CA60(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CA88);
#ifdef SKIP_ASM
extern "C" void func_0039E390();

extern "C" void func_0021CA88(void) {
    func_0039E390();
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CAA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_x(void*) __asm__("func_0039E2A0");
extern char D_004743F0[];
extern "C" void* func_0021CAA8(void *self) {
    func_0039E2A0_x(self);
    *(char**)((char*)self + 8) = D_004743F0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CAE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CAE0(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_xi(void*, int) __asm__("func_0039E2A0");
extern char D_00474248[];
extern "C" void* func_0021CB08(void *self) {
    func_0039E2A0_xi(self, 0);
    *(char**)((char*)self + 8) = D_00474248;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474248[];

extern "C" void func_0021CB40(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474248;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB68);
#ifdef SKIP_ASM
extern "C" int func_0021CB68(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB70);
#ifdef SKIP_ASM
extern "C" void func_0021CB70(int *p) {
    p[0]=1;
    p[1]=0;
    p[2]=0;
    p[3]=0;
    p[4]=0;
    p[5]=0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB90);
#ifdef SKIP_ASM
extern int D_004A275C;

extern "C" void func_0021CB90(int arg0) {
    D_004A275C = arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB98);
#ifdef SKIP_ASM
extern int D_004A275C;

extern "C" int func_0021CB98(void) {
    return D_004A275C;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021CBA0);

INCLUDE_ASM("fe/all_in_one_fe", func_0021CC40);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CCC8);
#ifdef SKIP_ASM
extern "C" unsigned char *func_0021CCC8(unsigned char *arg0) {
    *arg0 &= 0xF0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CCE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CCE0(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CD08);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00474088[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021CD08(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00474088;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CD70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CD70(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CD98);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473FB0[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021CD98(void *arg0, int arg1, int arg2) {
    func_0039E2A0(arg0);
    (*(int *)((char*)(arg0) + (0x98))) = arg2;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473FB0;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CE00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CE00(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021CE28);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CE90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CE90(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021CEB8);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CF38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CF38(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021CF60);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CFE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021CFE0(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021D008);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021D070);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473B78[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_0021D070(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473B78;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021D0D0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021D138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_0021D138(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021D160);
#ifdef SKIP_ASM
#define func_0021D160_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern "C" void func_00391C48(int, int, char*);

extern "C" void func_0021D160(float x, float y, int c, int d, ...) {
    char* ap;
    func_0021D160_va_start(ap);
    func_00391C48(c, d, ap);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021D1A0);

INCLUDE_ASM("fe/all_in_one_fe", func_0021D9A0);

INCLUDE_ASM("fe/all_in_one_fe", func_0021E1B0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021E750);
#ifdef SKIP_ASM
extern "C" float func_0021E750(int unused, int a, int flags, float x, float y, float z) {
    int v = a;
    if (flags & 1) {
        x = 640.0f - x;
        v = (flags & 2) ? a : -v;
    }
    if (v == 0) {
        x -= z * y * 0.5f;
    } else if (v > 0) {
        x -= z * y;
    }
    return x;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021E7A8);
#ifdef SKIP_ASM
extern "C" float func_0021E7A8(int arg0, int mode, float a, float b, float c) {
    if (mode == 0) {
        a -= c * b * 0.5f;
    } else if (mode > 0) {
        a -= c * b;
    }
    return a;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_0021E7E0);

INCLUDE_ASM("fe/all_in_one_fe", func_0021EA00);

INCLUDE_ASM("fe/all_in_one_fe", func_0021ED48);

INCLUDE_ASM("fe/all_in_one_fe", func_0021F338);

INCLUDE_ASM("fe/all_in_one_fe", func_0021F660);

INCLUDE_ASM("fe/all_in_one_fe", func_0021F9B0);

INCLUDE_ASM("fe/all_in_one_fe", func_0021FD38);

INCLUDE_ASM("fe/all_in_one_fe", func_002200C0);

INCLUDE_ASM("fe/all_in_one_fe", func_002204A0);

INCLUDE_ASM("fe/all_in_one_fe", func_00220AD0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220D38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_xi(void*, int) __asm__("func_0039E2A0");
extern char D_00473AA8[];
extern "C" void* func_00220D38(void *self) {
    func_0039E2A0_xi(self, 0);
    *(char**)((char*)self + 8) = D_00473AA8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220D70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_0046D0D0[];

extern "C" void func_00220D70(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_0046D0D0;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220D98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_xi(void*, int) __asm__("func_0039E2A0");
extern char D_004739D8[];
extern "C" void* func_00220D98(void *self) {
    func_0039E2A0_xi(self, 0);
    *(char**)((char*)self + 8) = D_004739D8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220DD0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_0046D0D0[];

extern "C" void func_00220DD0(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_0046D0D0;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220DF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E2A0_xi(void*, int) __asm__("func_0039E2A0");
extern char D_00473908[];
extern "C" void* func_00220DF8(void *self) {
    func_0039E2A0_xi(self, 0);
    *(char**)((char*)self + 8) = D_00473908;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220E30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_0046D0D0[];

extern "C" void func_00220E30(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_0046D0D0;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220E58);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void*, ...);
extern char D_00473838[];

extern "C" void *func_00220E58(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(char **)((char*)(arg0) + (8))) = D_00473838;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220E90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_0046D0D0[];

extern "C" void func_00220E90(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_0046D0D0;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220EB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_00220EB8(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00220EE0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220F48);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473688[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00220F48(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473688;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00220FA8);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221018);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_00221018(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221040);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_004735B0[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00221040(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_004735B0;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_002210A0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221110);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_00221110(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0039E390_a(void *) __asm__("func_0039E390");
extern char D_00474E08[];

extern "C" void func_00221138(void *arg0) {
    *(char **)((char*)arg0 + 8) = D_00474E08;
    func_0039E390_a(arg0);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221160);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002211C8);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_004733F8[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002211C8(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_004733F8;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221228);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221298);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221298(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

extern "C" void func_002212C0(void) {
}

INCLUDE_ASM("fe/all_in_one_fe", func_002212C8);

INCLUDE_ASM("fe/all_in_one_fe", func_00221350);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002213D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002213D8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

extern "C" void func_00221400(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221408);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473248[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00221408(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473248;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221468);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002214D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002214D0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002214F8);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473170[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002214F8(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473170;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221558);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002215C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002215C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002215F0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00473098[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002215F0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00473098;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221658);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002216C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002216C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002216F0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472FC0[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002216F0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472FC0;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221758);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002217C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002217C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002217F0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472EE8[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002217F0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472EE8;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221858);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002218C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002218C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002218F0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472E10[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002218F0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472E10;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221958);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002219C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002219C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002219F0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472D38[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002219F0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472D38;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221A58);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221AC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221AC8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221AF0);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472C60[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00221AF0(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472C60;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221B58);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221BC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221BC8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221BF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221BF0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221C18);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221C80);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00472AB0[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00221C80(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00472AB0;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221CE0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221D58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221D58(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

extern "C" void func_00221D80(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221D88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_x(void*) __asm__("func_001D5428");
extern char D_00472928[];
extern "C" void* func_00221D88(void *self) {
    *(char**)((char*)self + 8) = D_00472928;
    return func_001D5428_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221DB0);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern char D_00472928[];

extern "C" void *func_00221DB0(void *arg0) {
    func_001D53B0(arg0, 0, 0);
    (*(char **)((char*)(arg0) + (8))) = D_00472928;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221DF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221DF0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221E18);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221E80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221E80(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221EA8);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221F10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221F10(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221F38);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221FA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_00221FA0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00221FC8);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222030);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_x(void*) __asm__("func_001D5428");
extern char D_00472440[];
extern "C" void* func_00222030(void *self) {
    *(char**)((char*)self + 8) = D_00472440;
    return func_001D5428_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222058);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern char D_00472440[];

extern "C" void *func_00222058(void *arg0) {
    func_001D53B0(arg0, 0, 0);
    (*(char **)((char*)(arg0) + (8))) = D_00472440;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222098);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_x(void*) __asm__("func_001D5428");
extern char D_004722B8[];
extern "C" void* func_00222098(void *self) {
    *(char**)((char*)self + 8) = D_004722B8;
    return func_001D5428_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002220C0);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern char D_004722B8[];

extern "C" void *func_002220C0(void *arg0) {
    func_001D53B0(arg0, 0, 0);
    (*(char **)((char*)(arg0) + (8))) = D_004722B8;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222100);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_004721E0[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_00222100(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_004721E0;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00222160);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002221C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002221C8(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002221F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002221F0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00222218);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222280);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_001D5428_x(void*) __asm__("func_001D5428");
extern char D_00471F80[];
extern "C" void* func_00222280(void *self) {
    *(char**)((char*)self + 8) = D_00471F80;
    return func_001D5428_x(self);
}
#endif

extern "C" void func_002222A8(void) {
}

extern "C" void func_002222B0(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002222B8);
#ifdef SKIP_ASM
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern char D_00471F80[];

extern "C" void *func_002222B8(void *arg0) {
    func_001D53B0(arg0, 0, 0);
    (*(char **)((char*)(arg0) + (8))) = D_00471F80;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002222F8);
#ifdef SKIP_ASM
extern "C" void *func_0039E2A0(void*, ...);
extern char D_00471EA8[];
extern char D_00474E08[];
extern unsigned short D_004A2088[];

extern "C" void *func_002222F8(void *arg0) {
    func_0039E2A0(arg0, 0);
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 0;
    (*(char **)((char*)(arg0) + (8))) = D_00474E08;
    (*(int *)((char*)(arg0) + (0x4C))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = 0;
    unsigned short t = D_004A2088[0];
    (*(char **)((char*)(arg0) + (8))) = D_00471EA8;
    (*(unsigned short *)((char*)(arg0) + (0x58))) = t;
    return arg0;
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_00222358);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002223C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E390_x(void*) __asm__("func_0039E390");
extern char D_00474E08[];
extern "C" void* func_002223C0(void *self) {
    *(char**)((char*)self + 8) = D_00474E08;
    return func_0039E390_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002223E8);
#ifdef SKIP_ASM
#define func_002223E8_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern "C" void func_003929F8(int, int, char*);

extern "C" void func_002223E8(float x, float y, int c, int d, ...) {
    char* ap;
    func_002223E8_va_start(ap);
    func_003929F8(c, d, ap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/all_in_one_fe", func_00222428);
#ifdef SKIP_ASM
extern "C" void __sti__all_in_one_fe_cpp(int, int);

extern "C" void func_00222428(void) {
    __sti__all_in_one_fe_cpp(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222448);
#ifdef SKIP_ASM
extern unsigned short D_00441D38[];

extern "C" unsigned int func_00222448(const unsigned char *p, int n) {
    const unsigned char *end = p + n;
    unsigned int crc = 0xFBEA;
    while (p < end) {
        crc = D_00441D38[(*p++ ^ crc) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002224A0);
#ifdef SKIP_ASM
extern "C" int func_002224A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002224A8);
#ifdef SKIP_ASM
extern "C" int func_002224A8(int arg0) {
    return arg0;
}
#endif

extern "C" void func_002224B0(void) {
}

INCLUDE_ASM("fe/all_in_one_fe", func_002224B8);

INCLUDE_ASM("fe/all_in_one_fe", func_00222648);
