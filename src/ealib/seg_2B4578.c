#include "common.h"

INCLUDE_ASM("ealib/seg_2B4578", SHAPE_unpack);

INCLUDE_ASM("ealib/seg_2B4578", func_003B38B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3900);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_locate);
#ifdef SKIP_ASM
extern void func_003B3900();

void SHAPE_locate(void) {
    func_003B3900();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B39D8);
#ifdef SKIP_ASM
extern unsigned char D_0044C3D0[];

unsigned char func_003B39D8(unsigned char *arg0) {
    return D_0044C3D0[*arg0 & 0x7F];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B39F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044C3D0_39F8[128] __asm__("D_0044C3D0");

unsigned char func_003B39F8(int arg0) {
    return D_0044C3D0_39F8[arg0 & 0x7F];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B3A10);
#ifdef SKIP_ASM
unsigned char *func_003B3A10(unsigned char *p) {
    while (p) {
        int off;
        unsigned char *n;
        if (*p == 0x70) {
            return p + 4;
        }
        off = *(int *)p >> 8;
        n = p + off;
        p = off ? n : 0;
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B3A50);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3BB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3CCC);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3CD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3D40);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3DA8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3E40);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4010);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4218);

INCLUDE_ASM("ealib/seg_2B4578", func_003B42B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4338);

INCLUDE_ASM("ealib/seg_2B4578", func_003B43C0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B4448);
#ifdef SKIP_ASM
int func_003B4448(int arg0, int arg1, int arg2) {
    return ((arg1 * arg2) + arg0) << 5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B4458);
#ifdef SKIP_ASM
extern int D_0044C450[];
extern int D_0044C454[];
extern int D_0044C458[];

void func_003B4458(int arg0, int arg1, int arg2) {
    D_0044C450[0] = arg0;
    D_0044C454[0] = arg1;
    D_0044C458[0] = arg2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B4478);

INCLUDE_ASM("ealib/seg_2B4578", SHAPE_cloneat);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4690);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4708);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4740);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B47F8);
#ifdef SKIP_ASM
extern void func_003B4740();

void func_003B47F8(void) {
    func_003B4740();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B4818);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B49B0);
#ifdef SKIP_ASM
extern void func_003B4818();

void func_003B49B0(void) {
    func_003B4818();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B49D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4DD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B4FC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B5158);

INCLUDE_ASM("ealib/seg_2B4578", func_003B5320);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5440);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern signed char D_0050AA5C_5440[16] __asm__("D_0050AA5C");

signed char func_003B5440(void) {
    return D_0050AA5C_5440[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5450);
#ifdef SKIP_ASM
extern void func_003C60E0();

int func_003B5450(void) {
    func_003C60E0();
    return 0;
}
#endif

void func_003B5470(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5478);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003BC910(void);
extern void SYNCTASK_add_5478(void *, int, int) __asm__("SYNCTASK_add");
extern void func_003E4A30_5478(void *) __asm__("func_003E4A30");
extern int D_0044C464_C464[] __asm__("D_0044C464");
extern void (*D_0050A9B0_A9B0[])() __asm__("D_0050A9B0");

int func_003B5478(void) {
    D_0050A9B0_A9B0[0] = func_003B5470;
    if (D_0044C464_C464[0] == 0) {
        SYNCTASK_add_5478(func_003B5450, 0, 1);
        D_0044C464_C464[0] = 1;
    }
    func_003E4A30_5478(func_003BC910);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B54E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct N_54E0 { struct N_54E0 *next; int pad; void (*fn)(int); int arg; } N_54E0;
typedef struct { char pad[0x1B4]; N_54E0 *list; int pad2[2]; int cnt; } G_54E0;
extern int D_0050A8E8_A8E8_54E0[] __asm__("D_0050A8E8");
extern int D_0044C468_C468[] __asm__("D_0044C468");

void func_003B54E0(void) {
    N_54E0 *n;
    G_54E0 *g = (G_54E0 *)D_0050A8E8_A8E8_54E0;

    if (g->cnt != D_0044C468_C468[0]) {
        n = g->list;
        D_0044C468_C468[0] = g->cnt;
        if (n != 0) {
            do {
                n->fn(n->arg);
                n = n->next;
            } while (n != 0);
        }
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5540);

INCLUDE_ASM("ealib/seg_2B4578", func_003B55F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5860);
#ifdef SKIP_ASM
extern void func_003C3380();

void func_003B5860(void) {
    func_003C3380();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5880);
#ifdef SKIP_ASM
extern void func_003C33E0();

void func_003B5880(void) {
    func_003C33E0();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B58A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*D_0044C46C[])(void);
extern char D_0050A8E8_58A0[] __asm__("D_0050A8E8");

void func_003B58A0(void) {
    D_0044C46C[0]();
    D_0050A8E8_58A0[0x176]++;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B58D8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5910);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5910 {
    char pad0[0x174];
    signed char f174;
    char pad1[2];
    signed char f177;
    char pad2[0xC];
    int q[1];
};
extern struct S_5910 D_0050A8E8_5910 __asm__("D_0050A8E8");

void func_003B5910(int arg0) {
    if (D_0050A8E8_5910.f174 != 0) {
        D_0050A8E8_5910.q[D_0050A8E8_5910.f177] = arg0;
        D_0050A8E8_5910.f177++;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5948);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5A28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5A28 {
    char pad0[0x174];
    signed char f174;
    char pad1[4];
    signed char f179;
    char pad2[0x4E];
    int q[1];
};
extern struct S_5A28 D_0050A8E8_5A28 __asm__("D_0050A8E8");

void func_003B5A28(int arg0) {
    if (D_0050A8E8_5A28.f174 != 0) {
        D_0050A8E8_5A28.q[D_0050A8E8_5A28.f179] = arg0;
        D_0050A8E8_5A28.f179++;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5A60);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5B20);
#ifdef SKIP_ASM
extern void func_003B58A0();
extern void func_003B58D8();
extern int func_003C4E50(int, int);

int func_003B5B20(int arg0, int arg1) {
    int temp_16;

    func_003B58A0();
    temp_16 = func_003C4E50(arg0, arg1);
    func_003B58D8();
    return temp_16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5B68);
#ifdef SKIP_ASM
extern void func_003B58A0();
extern void func_003B58D8();
extern int func_003C4EC8(int);

int func_003B5B68(int arg0) {
    int temp_16;

    func_003B58A0();
    temp_16 = func_003C4EC8(arg0);
    func_003B58D8();
    return temp_16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5BA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_5BA0 { char pad[0xC]; unsigned int lim; };
extern struct G_5BA0 *D_0050AB1C_5BA0[] __asm__("D_0050AB1C");
void func_003B5BA0(int *a, int *b) {
    int x = *a;
    unsigned int lim = D_0050AB1C_5BA0[0]->lim;
    if (lim < (unsigned int)(x + *b))
        *b = lim - x;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5BD0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5C40);
#ifdef SKIP_ASM
extern void func_003C6240();

void func_003B5C40(void) {
    func_003C6240();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5C60);

INCLUDE_ASM("ealib/seg_2B4578", func_003B5E98);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5F60);
#ifdef SKIP_ASM
int func_003C51E0(void);
extern char D_0050AA5C[];

int func_003B5F60(void) {
    int r;
    if (D_0050AA5C[0] != 0) {
        r = func_003C51E0();
    } else {
        r = 0;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5F98);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6098);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6300);

INCLUDE_ASM("ealib/seg_2B4578", func_003B64D8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6528);
#ifdef SKIP_ASM
typedef struct { int a; int b; int c; } B6528_inner;
typedef struct { B6528_inner *p; int pad[3]; } B6528_ent;
extern B6528_ent *D_0050AADC[];

int func_003B6528(int arg0) {
    return D_0050AADC[0][arg0].p->c;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B6548);

INCLUDE_ASM("ealib/seg_2B4578", func_003B65D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6670);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6788);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6820);

INCLUDE_ASM("ealib/seg_2B4578", func_003B68B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6948);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B69D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x46]; unsigned char cnt; } G_69D8;
extern void func_003B7838(int);
extern G_69D8 D_0050A8E8_A8E8_69D8 __asm__("D_0050A8E8");

int func_003B69D8(void) {
    int i;

    i = 0;
    if (D_0050A8E8_A8E8_69D8.cnt != 0) {
        do {
            func_003B7838(i);
            i += 1;
        } while (i < (int)D_0050A8E8_A8E8_69D8.cnt);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6A38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0050A0A0[];
extern unsigned char D_0050A92E_6A38[] __asm__("D_0050A92E");

int func_003B6A38(int arg0) {
    if (arg0 >= D_0050A92E_6A38[0] || arg0 < 0) {
        return 0;
    }
    return D_0050A0A0[arg0];
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B6A70);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6AD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E13E8_6AD8(int, unsigned char *) __asm__("func_003E13E8");
extern int *D_0050A0A0_6AD8[] __asm__("D_0050A0A0");

void func_003B6AD8(void *arg0) {
    unsigned char *t = ((unsigned char **)arg0)[-1];
    func_003E13E8_6AD8(*D_0050A0A0_6AD8[*t], t);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B6B10);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6BD0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B6EA8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7080);

INCLUDE_ASM("ealib/seg_2B4578", func_003B70F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B71D8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7410);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0050A0A0_7410[] __asm__("D_0050A0A0");
extern unsigned char D_0050A92E_7410[] __asm__("D_0050A92E");

int func_003B7410(void) {
    int n = 0;
    int i;

    for (i = 0; i < D_0050A92E_7410[0]; i++) {
        if (D_0050A0A0_7410[i] != 0) {
            n++;
        }
    }
    return n;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B7450);

INCLUDE_ASM("ealib/seg_2B4578", func_003B76E8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7818);
#ifdef SKIP_ASM
void func_003B7818(int a, int b, int c, int d, int e) {
    func_003B7450(a, b, c, d, e, 0, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B7838);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B78D8);
#ifdef SKIP_ASM
extern void func_003B76E8(int, int, int, int, int);

void func_003B78D8(int a, int b, int c, int d) {
    func_003B76E8(a, b, c, d, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B78F8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7A70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *func_003B6A38_7A70(int) __asm__("func_003B6A38");

char *func_003B7A70(int arg0) {
    char *t;
    char *v;
    if (arg0 < 0) return 0;
    t = func_003B6A38_7A70(arg0 & 0xFF);
    if (t == 0) return 0;
    for (v = *(char **)(t + 0x11C); v != 0; v = *(char **)v) {
        if (*(int *)(v + 0xC) == arg0) break;
    }
    return v;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7AC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003C65F0(int);
int func_003B7AC8_7AC8(int arg0, int arg1) __asm__("func_003B7AC8");

int func_003B7AC8_7AC8(int arg0, int arg1) {
    int t = arg0 * 0x28 + 0x138;
    return t + func_003C65F0(arg1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7B00);
#ifdef SKIP_ASM
extern int func_003B7AC8();
extern int func_003E06B0(int, int, int);

int func_003B7B00(int arg0) {
    int temp_17;

    temp_17 = func_003B7AC8();
    return temp_17 + func_003E06B0(arg0 + 2, 1, 1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B7B48);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7C40);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7D80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *func_003B7A70_7D80(int) __asm__("func_003B7A70");

int func_003B7D80(int arg0, int arg1) {
    int var_18;
    char *temp_2;

    var_18 = -8;
    func_003B58A0();
    temp_2 = func_003B7A70_7D80(arg0);
    if (temp_2 != 0) {
        var_18 = 0;
        *(int *)(temp_2 + 0x20) = (short)arg1;
    }
    func_003B58D8();
    return var_18;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7DE8);
#ifdef SKIP_ASM
extern void func_003B76E8(int, int, int, int, int);

void func_003B7DE8(int a, int b, int c) {
    func_003B76E8(a, b, 0, c, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7E10);
#ifdef SKIP_ASM
extern void func_003B76E8(int, int, int, int, int);

void func_003B7E10(int a, int b, int c, int d) {
    func_003B76E8(a, b, c + d, 0, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7E38);
#ifdef SKIP_ASM
extern void func_003B7450(int, int, int, int, int, int, int);

void func_003B7E38(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
    func_003B7450(arg1, arg2, arg3, arg4, arg5, arg0, 1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B7E70);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7EB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7F18);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7F60);

INCLUDE_ASM("ealib/seg_2B4578", func_003B7FB8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8010);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8050);

INCLUDE_ASM("ealib/seg_2B4578", func_003B80A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B80E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8120);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int *func_003B6A38_8120(void) __asm__("func_003B6A38");
void func_003E0B28_8120(int, int) __asm__("func_003E0B28");

int func_003B8120(int arg0, int arg1) {
    int *temp_2;

    temp_2 = func_003B6A38_8120();
    if (temp_2 == 0) {
        return -8;
    }
    func_003E0B28_8120(*temp_2, arg1);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8160);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8218);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8290);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int func_003B8290(unsigned long a) {
    int t = (a & 0x40) >> 3;
    if (a & 0x10) t |= 4;
    if (a & 0x20) t |= 0x100;
    return t;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B82B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B83A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B84E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8530);

INCLUDE_ASM("ealib/seg_2B4578", func_003B85F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B86C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_0050AAD8_86C0[] __asm__("D_0050AAD8");

short func_003B86C0(void) {
    int i = (int)func_003BA6E8();
    if (i < 0) {
        return -8;
    }
    return *(short *)(D_0050AAD8_86C0[0] + i * 0x8C + 0x3A);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8700);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8790);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8838);

INCLUDE_ASM("ealib/seg_2B4578", func_003B88C8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8978);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8978(int, int *) __asm__("func_003BAA00");
extern void func_003C3178_8978(int, int) __asm__("func_003C3178");

int func_003B8978(int arg0, int arg1) {
    int sp0;
    int h;

    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00_8978(h, &sp0) != 0) {
            func_003C3178_8978(sp0, arg1);
        }
    }
    return h;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B89E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003BA6E8_89E0(void) __asm__("func_003BA6E8");
int func_003BAA00_89E0(int, int *) __asm__("func_003BAA00");
void func_003C2440_89E0(int) __asm__("func_003C2440");

int func_003B89E0(void) {
    int sp0;
    int temp_2;

    temp_2 = func_003BA6E8_89E0();
    if (temp_2 >= 0) {
        sp0 = -1;
        while (func_003BAA00_89E0(temp_2, &sp0) != 0) {
            func_003C2440_89E0(sp0);
        }
    }
    return temp_2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8A38);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8AB8);
#ifdef SKIP_ASM
extern unsigned int func_003BA6E8();

unsigned int func_003B8AB8(void) {
    return func_003BA6E8() >> 0x1F;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8AD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8C00);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8CC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8CC8(int, int *) __asm__("func_003BAA00");
extern void func_003C5068_8CC8(int, int) __asm__("func_003C5068");

int func_003B8CC8(int arg0, int arg1) {
    int sp0;
    int h;

    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00_8CC8(h, &sp0) != 0) {
            func_003C5068_8CC8(sp0, arg1);
        }
    }
    return h;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8D30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8D30(int, int *) __asm__("func_003BAA00");
extern void func_003C5128_8D30(int, int) __asm__("func_003C5128");

int func_003B8D30(int arg0, int arg1) {
    int sp0;
    int h;

    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00_8D30(h, &sp0) != 0) {
            func_003C5128_8D30(sp0, arg1);
        }
    }
    return h;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8D98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x10]; int key; char pad2[0x5C]; } E_8D98;
extern E_8D98 D_0050A120_8D98[] __asm__("D_0050A120");

int func_003B8D98(int arg0) {
    int idx;
    int v = arg0;
    if (v < 0)
        return -8;
    idx = v & 0x1F;
    if (idx < 0x10)
        return (D_0050A120_8D98[idx].key ^ v) == 0 ? idx : -8;
    return -8;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8DE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8EE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B8F78);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9140);

INCLUDE_ASM("ealib/seg_2B4578", func_003B91E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B93C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B94B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003B96B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B97D8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B9880);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B5948_9880(void (*)(void)) __asm__("func_003B5948");
extern void func_003B96B8_9880(void) __asm__("func_003B96B8");
extern void func_003B98C0_9880(void) __asm__("func_003B98C0");
extern int D_0050AAD0_9880[] __asm__("D_0050AAD0");

int func_003B9880(void) {
    func_003B58A0();
    func_003B98C0_9880();
    func_003B5948_9880(func_003B96B8_9880);
    D_0050AAD0_9880[0] = 0;
    func_003B58D8();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B98C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a[4]; int v; int pad[0x17]; } E_98C0;
extern void func_003B58A0();
extern void func_003B58D8();
extern void func_003B9920(int);
extern E_98C0 D_0050A120_A120_98C0[] __asm__("D_0050A120");

int func_003B98C0(void) {
    E_98C0 *tbl;
    int *p;
    int i;

    i = 0xF;
    func_003B58A0();
    tbl = D_0050A120_A120_98C0;
    p = &tbl->v;
    do {
        i -= 1;
        func_003B9920(*p);
        p += 0x1C;
    } while (i >= 0);
    func_003B58D8();
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B9920);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9A00);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9B68);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9C20);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9DC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9E48);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9F38);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B9FC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char func_003B8D98_9FC8(void) __asm__("func_003B8D98");
extern char D_0050A120_A120[] __asm__("D_0050A120");

signed char func_003B9FC8(void) {
    signed char var_5;
    char *temp_2;

    var_5 = func_003B8D98_9FC8();
    if (var_5 >= 0) {
        temp_2 = D_0050A120_A120 + var_5 * 0x70;
        var_5 = *(signed char *)(temp_2 + 0x28);
        if (var_5 == 3) {
            var_5 = (*(int *)(temp_2 + 0x18) != 0) ? 6 : var_5;
        }
    }
    return var_5;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BA020);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BA078);
#ifdef SKIP_ASM
extern short D_0050AC20[];

int func_003BA078(int arg0, int arg1) {
    int i;
    for (i = 0; i < arg1; i++) {
        if (D_0050AC20[i] == arg0) {
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BA0B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BA550);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BA6E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A8E8_A8E8[] __asm__("D_0050A8E8");

int func_003BA6E8_A6E8(int arg0) __asm__("func_003BA6E8");
int func_003BA6E8_A6E8(int arg0) {
    int var_5;
    char *temp_2;
    char *g;

    if ((arg0 < 0) || (g = D_0050A8E8_A8E8, var_5 = arg0 & 0xFF, (var_5 < *(short *)(g + 0x17E)) == 0)) {
        return -8;
    }
    temp_2 = *(char **)(g + 0x1F0) + (var_5 * 0x8C);
    if ((*(signed char *)(temp_2 + 0x69) == 0) || (*(int *)temp_2 != arg0)) {
        var_5 = -8;
    }
    return var_5;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BA740);

INCLUDE_ASM("ealib/seg_2B4578", func_003BA7B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BA888);

INCLUDE_ASM("ealib/seg_2B4578", func_003BA938);

INCLUDE_ASM("ealib/seg_2B4578", func_003BAA00);

INCLUDE_ASM("ealib/seg_2B4578", func_003BAAC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BB0C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BB140);

INCLUDE_ASM("ealib/seg_2B4578", func_003BB588);

INCLUDE_ASM("ealib/seg_2B4578", func_003BB820);

INCLUDE_ASM("ealib/seg_2B4578", func_003BBDA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC000);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC0B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC4C0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC530);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003B5E98_C530(int) __asm__("func_003B5E98");
extern int D_0050AC30_C530[] __asm__("D_0050AC30");
extern char D_0050AC38_C530[] __asm__("D_0050AC38");

void func_003BC530(void) {
    func_003B58A0();
    if (D_0050AC38_C530 != 0) {
        func_003B5E98_C530(D_0050AC30_C530[0]);
    }
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC570);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short D_0044C590_C570[128] __asm__("D_0044C590");

unsigned short func_003BC570(int arg0) {
    return D_0044C590_C570[arg0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC588);
#ifdef SKIP_ASM
extern int D_0044C690[];
extern int D_0050AC50[];

void func_003BC588(int arg0) {
    int i;
    for (i = 0; i < 6; i++) {
        D_0050AC50[i] = D_0044C690[i] + arg0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BC5C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC6B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC770);

INCLUDE_ASM("ealib/seg_2B4578", func_003BC840);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC910);
#ifdef SKIP_ASM
extern void func_003C5E08();

void func_003BC910(void) {
    func_003C5E08();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC930);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003BECA0_C930(void) __asm__("func_003BECA0");
extern void func_003BED30_C930(void) __asm__("func_003BED30");
extern void func_003BF0F0_C930(void) __asm__("func_003BF0F0");
extern void *D_0050C840[];
extern void *D_0050C844[];
extern void *D_0050C848[];

void func_003BC930(void) {
    D_0050C840[0] = func_003BECA0_C930;
    D_0050C844[0] = func_003BED30_C930;
    D_0050C848[0] = func_003BF0F0_C930;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BC968);

INCLUDE_ASM("ealib/seg_2B4578", func_003BCA28);

INCLUDE_ASM("ealib/seg_2B4578", func_003BCAE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BCBA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD078);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD220);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD5B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD9C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BDE08);

INCLUDE_ASM("ealib/seg_2B4578", func_003BE6E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BECA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BED30);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF0F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF3E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF440);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF4C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF5A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF6B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF800);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF8D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BF960);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFAC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFB90);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFC50);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFD60);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFE80);

INCLUDE_ASM("ealib/seg_2B4578", func_003BFFA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C01A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C03E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C04F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0564);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C05E0);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed bytes).
void func_003C05E0(void *in, void *out, float s) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        "vftoi0.xyzw $vf1, $vf1\n"
        "vftoi0.xyzw $vf2, $vf2\n"
        "vftoi0.xyzw $vf3, $vf3\n"
        "vftoi0.xyzw $vf4, $vf4\n"
        "qmfc2.ni  $6, $vf1\n"
        "qmfc2.ni  $7, $vf2\n"
        "qmfc2.ni  $8, $vf3\n"
        "qmfc2.ni  $9, $vf4\n"
        "ppach     $6, $7, $6\n"
        "ppach     $8, $9, $8\n"
        "ppacb     $6, $8, $6\n"
        "sq        $6, 0x0(%1)\n"
        :
        : "r"(in), "r"(out), "f"(s)
        : "$6", "$7", "$8", "$9", "memory");
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C0640);

INCLUDE_ASM("ealib/seg_2B4578", func_003C06B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C07A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0858);

INCLUDE_ASM("ealib/seg_2B4578", func_003C08F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C09B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0A60);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0B10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C0BE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003BA550(int, unsigned char);
struct S_0BE8 { char pad[0x2F]; unsigned char a; unsigned char b; };
extern struct S_0BE8 D_0050A8E8_0BE8 __asm__("D_0050A8E8");

void func_003C0BE8(int arg0) {
    func_003BA550(arg0 + D_0050A8E8_0BE8.b + D_0050A8E8_0BE8.a, D_0050A8E8_0BE8.a);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C0C18);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0C90);

INCLUDE_ASM("ealib/seg_2B4578", func_003C0E28);

INCLUDE_ASM("ealib/seg_2B4578", func_003C1298);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C1578);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0xC]; int v; int mode; } A_1578;
typedef struct { char pad[0x1AA4]; int a; char pad2[0x5C]; int b; } G_1578;
extern void func_003C4918_1578(int) __asm__("func_003C4918");
extern void func_00423DD0(int);
extern G_1578 D_0050AD00_AD00 __asm__("D_0050AD00");

void func_003C1578(A_1578 *arg0) {
    switch (arg0->mode) {
    case 2:
        func_003C4918_1578(arg0->v);
        return;
    case 3:
        break;
    case 0:
        D_0050AD00_AD00.b = arg0->v;
        func_00423DD0(D_0050AD00_AD00.a);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C15D8);
#ifdef SKIP_ASM
void func_003C15D8(char *dst, char *src) {
    while (*dst) dst++;
    while (*src) {
        *dst++ = *src++;
    }
    *dst = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C1638);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C1980);
#ifdef SKIP_ASM
struct S_1980 { char pad[0x1C0]; int cnt; };
extern struct S_1980 D_0050A8E8_1980 __asm__("D_0050A8E8");
int func_003C1980(void) {
    D_0050A8E8_1980.cnt = D_0050A8E8_1980.cnt + 1;
    // PORT: MIPS sync + EE ei (no C equivalent)
    __asm__ volatile("sync\n\tei");
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C19A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C1B80);

INCLUDE_ASM("ealib/seg_2B4578", func_003C2268);

void func_003C2438(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", func_003C2440);

INCLUDE_ASM("ealib/seg_2B4578", func_003C25E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C27E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C2A50);

INCLUDE_ASM("ealib/seg_2B4578", func_003C2EC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3010);

INCLUDE_ASM("ealib/seg_2B4578", func_003C30C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3178);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3250);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3300);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3358);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3380);

INCLUDE_ASM("ealib/seg_2B4578", func_003C33E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3450);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3500);

INCLUDE_ASM("ealib/seg_2B4578", func_003C35F8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C36A8);
#ifdef SKIP_ASM
extern void func_003C35F8(int);

int func_003C36A8(int a, int b) {
    func_003C35F8(b);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C36C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3728);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3780);

INCLUDE_ASM("ealib/seg_2B4578", func_003C38E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3EA8);
#ifdef SKIP_ASM
int func_003C3EA8(void *arg0, int arg1, int *arg2) {
    if (*arg2 == 0) {
        *arg2 = (*(int *)((char*)(arg0) + (0x68)));
    }
    *(*(int **)((char*)(arg0) + (0x64))) = arg1 + ((*(int *)((char*)(arg0) + (0x68))) - *arg2);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C3EE0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C42D8);
#ifdef SKIP_ASM
extern void func_003C4788(int, int, int, int, int);

void func_003C42D8(int a, int b, int c, int d) {
    func_003C4788(a, b, c, d, 10);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C42F8);
#ifdef SKIP_ASM
extern void func_003C4898();

void func_003C42F8(void) {
    func_003C4898();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C4318);

INCLUDE_ASM("ealib/seg_2B4578", func_003C43C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4450);

INCLUDE_ASM("ealib/seg_2B4578", func_003C45E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4668);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4788);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4898);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4918);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4978);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4A38);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4AF8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4B70);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4D78);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4E50);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4EC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5068);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5128);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C51E0);
#ifdef SKIP_ASM
int func_003C51E0(void) {
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C51E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C52C8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C52F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_52F8(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_52F8(void *) __asm__("func_003B4FC0");

int func_003C52F8(int arg0) {
    char sp[0xE0];
    func_003B4DD8_52F8(sp);
    *(short *)(sp + 0x28) = arg0;
    func_003B4FC0_52F8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5330);
#ifdef SKIP_ASM
extern void func_003B4DD8(void *);

int func_003C5330(int *arg0) {
    char sp[0xE0];
    func_003B4DD8(sp);
    *arg0 = *(unsigned short *)(sp + 0x28);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5368);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5368(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5368(void *) __asm__("func_003B4FC0");

int func_003C5368(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5368(sp);
    *(signed char *)(sp + 0x46) = arg0;
    func_003B4FC0_5368(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C53A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_53A0(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_53A0(void *) __asm__("func_003B4FC0");

int func_003C53A0(int arg0) {
    char sp[0xE0];
    func_003B4DD8_53A0(sp);
    *(signed char *)(sp + 0x3C) = arg0;
    func_003B4FC0_53A0(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C53D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_53D8(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_53D8(void *) __asm__("func_003B4FC0");

int func_003C53D8(int arg0) {
    char sp[0xE0];
    func_003B4DD8_53D8(sp);
    *(int *)(sp + 0x24) = arg0;
    func_003B4FC0_53D8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5410);
#ifdef SKIP_ASM
extern void func_003B4FC0(void *);

int func_003C5410(float f) {
    char buf[0xE0];

    if (f >= 0.0f) {
        f = 1000.0f / f;
    }
    func_003B4DD8(buf);
    *(float *)(buf + 0x34) = f;
    func_003B4FC0(buf);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5468);
#ifdef SKIP_ASM
int func_003C5468(float *a0) {
    char buf[0xE0];
    float f;

    func_003B4DD8(buf);
    f = *(float *)(buf + 0x34);
    *a0 = f;
    if (f >= 0.0f) {
        *a0 = 1000.0f / f;
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C54C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5538);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5600);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044E1C4_5600[] __asm__("D_0044E1C4");
int func_003C54C0_5600(int, int *) __asm__("func_003C54C0");

int func_003C5600(int arg0) {
    int buf[4];
    func_003C54C0_5600(arg0, buf);
    if (buf[0] == 0) {
        return -5;
    }
    D_0044E1C4_5600[0] = arg0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5648);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044E1C4_5648[4] __asm__("D_0044E1C4");

int func_003C5648(int *arg0) {
    *arg0 = D_0044E1C4_5648[0];
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C5660);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5710);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5788);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct Buf_5788 { char pad[0x2A]; unsigned short a; unsigned short b; char rest[0xE0 - 0x2E]; } Buf_5788;
extern void func_003B4DD8_5788(Buf_5788 *) __asm__("func_003B4DD8");

int func_003C5788(int arg0, int *arg1) {
    Buf_5788 sp;

    *arg1 = 0;
    func_003B4DD8_5788(&sp);
    switch (arg0) {
    case 0:
        *arg1 = sp.a;
        break;
    case 1:
        *arg1 = sp.b;
        break;
    default:
        return -5;
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C57F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5880);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5908);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5998);

INCLUDE_ASM("ealib/seg_2B4578", func_003C5A28);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AA0);
#ifdef SKIP_ASM
int func_003C5AA0(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AA8);
#ifdef SKIP_ASM
int func_003C5AA8(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AB0);
#ifdef SKIP_ASM
int func_003C5AB0(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AB8);
#ifdef SKIP_ASM
int func_003C5AB8(void) {
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5AC0(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5AC0(void *) __asm__("func_003B4FC0");

int func_003C5AC0(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5AC0(sp);
    *(signed char *)(sp + 0x3F) = arg0;
    func_003B4FC0_5AC0(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5AF8(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5AF8(void *) __asm__("func_003B4FC0");

int func_003C5AF8(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5AF8(sp);
    *(int *)(sp + 0x6C) = arg0;
    func_003B4FC0_5AF8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5B30);
#ifdef SKIP_ASM
typedef struct { char pad[0x4A]; signed char v; char pad2[0xE0 - 0x4B]; } S_5B30;
extern void func_003B4FC0(void *);

int func_003C5B30(float f) {
    S_5B30 s;
    func_003B4DD8(&s);
    s.v = (int)(f * 100.0f);
    func_003B4FC0(&s);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5B80(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5B80(void *) __asm__("func_003B4FC0");

int func_003C5B80(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5B80(sp);
    *(signed char *)(sp + 0x42) = arg0;
    func_003B4FC0_5B80(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5BB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5BB8(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5BB8(void *) __asm__("func_003B4FC0");

int func_003C5BB8(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5BB8(sp);
    *(signed char *)(sp + 0x43) = arg0;
    func_003B4FC0_5BB8(sp);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5BF0);
#ifdef SKIP_ASM
int func_003C5BF0(void) {
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5BF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5BF8(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5BF8(void *) __asm__("func_003B4FC0");

int func_003C5BF8(int arg0) {
    char sp[0xE0];
    func_003B4DD8_5BF8(sp);
    *(int *)(sp + 0xC8) = arg0;
    func_003B4FC0_5BF8(sp);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C5C30);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5DF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern signed char D_0050AA5C_5DF0[16] __asm__("D_0050AA5C");

int func_003C5DF0(void) {
    return D_0050AA5C_5DF0[0] == 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5E08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044E1C0_5E08[] __asm__("D_0044E1C0");
void func_003B5320_5E08(void) __asm__("func_003B5320");
int func_003C5E08_5E08(void) __asm__("func_003C5E08");
int func_003C5E08_5E08(void) {
    func_003B5320_5E08();
    D_0044E1C0_5E08[0] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5E30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044E1C0_5E30[] __asm__("D_0044E1C0");
extern int D_005157B0_5E30[] __asm__("D_005157B0");
int func_003C5E30(int a, int b, int c) {
    if (a == 0) {
        D_0044E1C0_5E30[0] = b;
        D_005157B0_5E30[0] = c;
        return 0;
    }
    return -5;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C5E58);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5ED0);
#ifdef SKIP_ASM
typedef struct { char pad0[0x64]; int a; int b; char pad1[0xE0 - 0x6C]; } S_5ED0;
extern void func_003B4FC0(void *);

int func_003C5ED0(int arg0, int arg1) {
    S_5ED0 s;
    func_003B4DD8(&s);
    s.a = arg0;
    s.b = arg1;
    func_003B4FC0(&s);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5F18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003B4FC0_5F18(char *) __asm__("func_003B4FC0");

int func_003C5F18(int arg0, float f) {
    char buf[0xE0];
    if (arg0 == 0) {
        func_003B4DD8(buf);
        buf[0x4B] = (int)(f * 100.0f);
        func_003B4FC0_5F18(buf);
        return 0;
    }
    return -5;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C5F70);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5FD0);
#ifdef SKIP_ASM
typedef struct { char pad0[0xCC]; int a; int b; char pad1[0xE0 - 0xD4]; } S_5FD0;
extern void func_003B4FC0(void *);

int func_003C5FD0(int arg0, int arg1) {
    S_5FD0 s;
    func_003B4DD8(&s);
    s.a = arg0;
    s.b = arg1;
    func_003B4FC0(&s);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6018);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_6018 { char pad[0x178]; signed char n; char pad2[0x23]; int v[1]; };
extern struct S_6018 D_0050A8E8_6018 __asm__("D_0050A8E8");
void func_003C6018(int a) {
    D_0050A8E8_6018.v[D_0050A8E8_6018.n] = a;
    D_0050A8E8_6018.n = D_0050A8E8_6018.n + 1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6040);

INCLUDE_ASM("ealib/seg_2B4578", func_003C60E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C6158);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6240);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_0050AB1C_6240[] __asm__("D_0050AB1C");
int func_003C6240_6240(void) __asm__("func_003C6240");

int func_003C6240_6240(void) {
    int temp_3;

    temp_3 = (*(int *)((char*)(D_0050AB1C_6240[0]) + (8)));
    return (int) ((temp_3 - (*(int *)((char*)(D_0050AB1C_6240[0]) + (0x10)))) * 0x64) / temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6278);
#ifdef SKIP_ASM
void func_003C6278(void *arg0) {
    *(int *)((char *)arg0 + 0) = 0;
    *(int *)((char *)arg0 + 4) = 0;
    *(int *)((char *)arg0 + 8) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6288);
#ifdef SKIP_ASM
void func_003C6288(void *arg0, void *arg1) {
    void *temp_2;

    (*(void **)((char*)(arg1) + (0))) = (void *) (*(void **)((char*)(arg0) + (0)));
    (*(int *)((char*)(arg1) + (4))) = 0;
    temp_2 = (*(void **)((char*)(arg0) + (0)));
    if (temp_2 != 0) {
        (*(void **)((char*)(temp_2) + (4))) = arg1;
    } else {
        (*(void **)((char*)(arg0) + (4))) = arg1;
    }
    (*(void **)((char*)(arg0) + (0))) = arg1;
    (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) + 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C62B8);
#ifdef SKIP_ASM
void func_003C62B8(void *arg0, int *arg1) {
    int *temp_2;

    (*(int **)((char*)(arg1) + (0))) = 0;
    (*(int **)((char*)(arg1) + (4))) = (int *) (*(int **)((char*)(arg0) + (4)));
    temp_2 = (*(int **)((char*)(arg0) + (4)));
    if (temp_2 != 0) {
        *temp_2 = (int)arg1;
    } else {
        (*(int **)((char*)(arg0) + (0))) = arg1;
    }
    (*(int **)((char*)(arg0) + (4))) = arg1;
    (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) + 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C62E8);
#ifdef SKIP_ASM
int *func_003C62E8(void *arg0) {
    int *temp_2;
    int *temp_3;

    temp_3 = (*(int **)((char*)(arg0) + (0)));
    if (temp_3 != 0) {
        temp_2 = *temp_3;
        (*(int **)((char*)(arg0) + (0))) = temp_2;
        if (temp_2 == 0) {
            (*(int *)((char*)(arg0) + (4))) = 0;
        } else {
            (*(int *)((char*)(temp_2) + (4))) = 0;
        }
        (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) - 1);
    }
    return temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6320);
#ifdef SKIP_ASM
void func_003C6320(void *arg0, void *arg1) {
    void **temp_3;
    void *temp_3_2;

    if (arg1 == (*(void **)((char*)(arg0) + (0)))) {
        (*(void **)((char*)(arg0) + (0))) = (void *) (*(void **)((char*)(arg1) + (0)));
    }
    if (arg1 == (*(void ***)((char*)(arg0) + (4)))) {
        (*(void ***)((char*)(arg0) + (4))) = (void **) (*(void ***)((char*)(arg1) + (4)));
    }
    temp_3 = (*(void ***)((char*)(arg1) + (4)));
    if (temp_3 != 0) {
        *temp_3 = (*(void **)((char*)(arg1) + (0)));
    }
    temp_3_2 = (*(void **)((char*)(arg1) + (0)));
    if (temp_3_2 != 0) {
        (*(void ***)((char*)(temp_3_2) + (4))) = (void **) (*(void ***)((char*)(arg1) + (4)));
    }
    (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) - 1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6380);

INCLUDE_ASM("ealib/seg_2B4578", func_003C64A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C6500);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6540);
#ifdef SKIP_ASM
void *func_003C6540(char *arg0, int arg1) {
    int temp_3;

    if (arg1 >= *(unsigned short *)(arg0 + 6)) {
        return 0;
    }
    temp_3 = *(int *)(arg0 + (arg1 << 2) + 0x14);
    if (temp_3 == 0) {
        return 0;
    }
    return arg0 + ((arg1 << 2) + 0x14) + temp_3;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6580);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C65E0);
#ifdef SKIP_ASM
int func_003C65E0(int arg0) {
    return (arg0 << 5) + 0x9C;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C65F0);
#ifdef SKIP_ASM
int func_003CE198(void);
int func_003C65F0(int arg0) {
    int t = func_003C65E0(arg0);
    return t + func_003CE198();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6620);

INCLUDE_ASM("ealib/seg_2B4578", func_003C6760);

INCLUDE_ASM("ealib/seg_2B4578", func_003C6AB8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C6C28);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6C98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Inner_6C98 { char pad[0x48]; int a; int b; };
struct S_6C98 { char pad[0x304]; struct Inner_6C98 *p[1]; };
extern struct S_6C98 D_005157B8_6C98 __asm__("D_005157B8");
int func_003C6C98(int i) {
    struct Inner_6C98 *q = (&D_005157B8_6C98)[0].p[i];
    int b = q->b; int a = q->a; return a + b;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6CC0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6CE0);
#ifdef SKIP_ASM
int func_003C6CE0(void) {
    return -0xF;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6CE8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6DA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CE1A8_6DA0(void) __asm__("func_003CE1A8");
typedef struct { char pad[0x304]; int arr[100]; } W_6DA0;
extern W_6DA0 D_005157B8_6DA0 __asm__("D_005157B8");

int func_003C6DA0(int arg0) {
    func_003CE1A8_6DA0();
    D_005157B8_6DA0.arr[arg0] = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C6DE0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7010);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7080);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7138);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7218);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C72A0);
#ifdef SKIP_ASM
int func_003C72A0(int arg0) {
    if (arg0 > 0x10000) {
        arg0 = 0x10000;
    } else {
        arg0 = (arg0 <= -1) ? 0 : arg0;
    }
    return (int)(((func_003BC5C8() & 0x7FFF) - 0x4000) * arg0) >> 0xE;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C72F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7388);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7480);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7600);

INCLUDE_ASM("ealib/seg_2B4578", func_003C7828);

void func_003C7AA8(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", func_003C7AB0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7CC0);
#ifdef SKIP_ASM
extern void func_003B5C60();

void func_003C7CC0(void) {
    func_003B5C60();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7CE0);
#ifdef SKIP_ASM
extern void func_003B5E98();

void func_003C7CE0(void) {
    func_003B5E98();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C7D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8018);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8128);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad0[0xC]; short s; char pad1[0x1E0 - 0xE]; void (*f)(void); } C8128_S;
extern C8128_S D_00515B40;
extern void func_003CBB78_8128(void) __asm__("func_003CBB78");

void func_003C8128(void) {
    D_00515B40.f = func_003CBB78_8128;
    D_00515B40.s = 0x400;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8148);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern signed char D_00515B4E_8148[16] __asm__("D_00515B4E");

void func_003C8148(signed char arg0) {
    D_00515B4E_8148[0] = arg0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8158);

INCLUDE_ASM("ealib/seg_2B4578", func_003C83A0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8430);
#ifdef SKIP_ASM
extern void func_003C8468();
extern int (*D_00515B48[])(int);

void func_003C8430(int arg0) {
    func_003C8468();
    D_00515B48[0](arg0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8468);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8560);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_8560 {
    char pad0[1];
    char flag;
    char pad1[0x1A];
    float v[1];
    char pad2[0x40];
};
struct G_8560 {
    char pad[0x1DC];
    struct E_8560 *tab;
};
extern struct G_8560 D_00515B40_8560 __asm__("D_00515B40");

void func_003C8560(int idx, int k, float f) {
    D_00515B40_8560.tab[idx].v[k] = f;
    D_00515B40_8560.tab[idx].flag = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8598);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_8598 {
    char pad0[1];
    char flag;
    char pad1[0x36];
    float v[1];
    char pad2[0x24];
};
struct G_8598 {
    char pad[0x1DC];
    struct E_8598 *tab;
};
extern struct G_8598 D_00515B40_8598 __asm__("D_00515B40");

void func_003C8598(int idx, int k, float f) {
    D_00515B40_8598.tab[idx].v[k] = f;
    D_00515B40_8598.tab[idx].flag = 1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C85D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8968);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8A40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_00515D1C_8A40[] __asm__("D_00515D1C");

int func_003C8A40(int idx) {
    char *e = D_00515D1C_8A40[0] + idx * 0x60;
    int (*fn)(int) = *(int (**)(int))(e + 0x44);
    if (fn != 0) {
        return fn(*(int *)(e + 0x48));
    }
    return -1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8A88);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8B40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x4C]; int f; char pad2[0x10]; } E_8B40;
typedef struct { E_8B40 arr[100]; } W_8B40;
extern W_8B40 *D_00515D1C_8B40[] __asm__("D_00515D1C");
void func_003CB110_8B40(int, int) __asm__("func_003CB110");

void func_003C8B40(int arg0, int arg1) {
    int temp_4;

    temp_4 = D_00515D1C_8B40[0]->arr[arg0].f;
    if (temp_4 != 0) {
        func_003CB110_8B40(temp_4, arg1);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8B80);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8C30);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8E28);

INCLUDE_ASM("ealib/seg_2B4578", func_003C8EC0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8F48);
#ifdef SKIP_ASM
void func_003C8F48(char **arg0, char *arg1) {
    char *cur;
    char *prev;

    cur = *arg0;
    prev = 0;
    while (cur != 0 && *(unsigned short *)(cur + 0x18) < *(unsigned short *)(arg1 + 0x18)) {
        prev = cur;
        cur = *(char **)(cur + 8);
    }
    *(char **)(arg1 + 8) = cur;
    if (cur != 0) {
        *(char **)(cur + 0x10) = arg1;
    }
    if (prev == 0) {
        *arg0 = arg1;
    } else {
        *(char **)(prev + 8) = arg1;
        *(char **)(arg1 + 0x10) = prev;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8FB0);
#ifdef SKIP_ASM
typedef struct N_8FB0 { char pad0[8]; struct N_8FB0 *next; int pad1; struct N_8FB0 *prev; } N_8FB0;

void func_003C8FB0(N_8FB0 **arg0, N_8FB0 *arg1) {
    N_8FB0 *temp_5;
    N_8FB0 *var_2;
    N_8FB0 *var_6;

    var_6 = *arg0;
    var_2 = var_6->next;
    if (arg1 == var_6) {
        *arg0 = var_2;
        return;
    }
    while (var_2 != 0 && var_2 != arg1) {
        var_6 = var_2;
        var_2 = var_6->next;
    }
    if ((var_2 != 0) && (var_2 == arg1)) {
        temp_5 = arg1->next;
        if (temp_5 != 0) {
            temp_5->prev = var_6;
        }
        var_6->next = var_6->next->next;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9010);
#ifdef SKIP_ASM
int func_003C9010(void *arg0) {
    int (*temp_2)();

    temp_2 = (*(int (**)())((char*)(arg0) + (4)));
    if (temp_2 != 0) {
        temp_2();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9040);
#ifdef SKIP_ASM
void func_003C9040(void *arg0) {
    int (*temp_2)(void *);
    void *temp_4;
    void *temp_4_2;

    temp_4 = (*(void **)((char*)(arg0) + (8)));
    if ((temp_4 != 0) && ((*(unsigned char *)((char*)(temp_4) + (0x1B))) == 0)) {
        func_003C9040(temp_4);
    }
    temp_4_2 = (*(void **)((char*)(arg0) + (0xC)));
    if ((temp_4_2 != 0) && ((*(unsigned char *)((char*)(temp_4_2) + (0x1B))) == 0)) {
        func_003C9040(temp_4_2);
    }
    temp_2 = (*(int (**)(void *))((char*)(arg0) + (4)));
    if (temp_2 != 0) {
        temp_2(arg0);
    }
    (*(signed char *)((char*)(arg0) + (0x1B))) = 1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C90C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9188);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9268);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x18];
    void *p18, *p1c, *p20;
    char pad1[0x60 - 0x24];
    int i60, i64, i68;
} G_9268;
extern G_9268 D_00515B40_9268 __asm__("D_00515B40");
extern void f_9268_a(void) __asm__("func_003C9AC8");
extern void f_9268_b(void) __asm__("func_003C9BC8");
extern void f_9268_c(void) __asm__("func_003C9D28");

void func_003C9268(void) {
D_00515B40_9268.p18 = f_9268_a;
    D_00515B40_9268.i60 = 0x2C;
    D_00515B40_9268.p1c = f_9268_b;
    D_00515B40_9268.i64 = 0x30;
    D_00515B40_9268.p20 = f_9268_c;
    D_00515B40_9268.i68 = 0x34;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C92B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x24];
    void *p24, *p28, *p2c;
    char pad1[0x6C - 0x30];
    int i6c, i70, i74;
} G_92B0;
extern G_92B0 D_00515B40_92B0 __asm__("D_00515B40");
extern void f_92B0_a(void) __asm__("func_003CA028");
extern void f_92B0_b(void) __asm__("func_003CA388");
extern void f_92B0_c(void) __asm__("func_003CA610");

void func_003C92B0(void) {
D_00515B40_92B0.p24 = f_92B0_a;
    D_00515B40_92B0.i6c = 0x2C;
    D_00515B40_92B0.i70 = 0x54;
    D_00515B40_92B0.p28 = f_92B0_b;
    D_00515B40_92B0.p2c = f_92B0_c;
    D_00515B40_92B0.i74 = 0x3C;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C92F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad[0x30];
    void *f30, *f34, *f38, *f3C, *f40, *f44;
    char pad2[0x30];
    int i78, i7C, i80, i84, i88, i8C;
} G_92F8;
extern void func_003C9420_9420(void) __asm__("func_003C9420");
extern void func_003C9618_9618(void) __asm__("func_003C9618");
extern void func_003C9960_9960(void) __asm__("func_003C9960");
extern G_92F8 D_00515B40_5B40 __asm__("D_00515B40");

void func_003C92F8(void) {
    D_00515B40_5B40.f30 = func_003C9420_9420;
    D_00515B40_5B40.i78 = 0x2C;
    D_00515B40_5B40.f38 = func_003C9960_9960;
    D_00515B40_5B40.i80 = 0x3C;
    D_00515B40_5B40.f34 = func_003C9618_9618;
    D_00515B40_5B40.i7C = 0x30;
D_00515B40_5B40.f3C = func_003C9420_9420;
    D_00515B40_5B40.i84 = 0x2C;
    D_00515B40_5B40.f44 = func_003C9960_9960;
    D_00515B40_5B40.i8C = 0x3C;
    D_00515B40_5B40.f40 = func_003C9618_9618;
    D_00515B40_5B40.i88 = 0x30;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9358);
#ifdef SKIP_ASM
int func_003C9358(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9360);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C93F8);
#ifdef SKIP_ASM
extern void func_003CD1D0(int);

void func_003C93F8(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x28)));
    if (temp_4 != 0) {
        func_003CD1D0(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9420);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9518);
#ifdef SKIP_ASM
int func_003C9518(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9520);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C95F0);
#ifdef SKIP_ASM
extern void func_003CD1D0(int);

void func_003C95F0(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003CD1D0(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9618);

INCLUDE_ASM("ealib/seg_2B4578", func_003C96F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9938);
#ifdef SKIP_ASM
extern void func_003CD1D0(int);

void func_003C9938(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x34)));
    if (temp_4 != 0) {
        func_003CD1D0(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9960);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C99F8);
#ifdef SKIP_ASM
int func_003C99F8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9A00);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C9AC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003C99F8_9AC8(void) __asm__("func_003C99F8");
void func_003C9A00_9AC8(void) __asm__("func_003C9A00");

void func_003C9AC8(char *arg0, char *arg1) {
    int t;
    *(void (**)(void))(arg0 + 0) = func_003C9A00_9AC8;
    t = *(int *)(arg1 + 0);
    *(int *)(arg0 + 0x20) = 0;
    *(int *)(arg0 + 0x1C) = t;
    *(int *)(arg0 + 0x24) = *(int *)(arg1 + 0xC);
    *(int *)(arg0 + 0x28) = *(int *)(arg1 + 0x18);
    *(void (**)(void))(arg1 + 0x28) = func_003C99F8_9AC8;
    *(int *)(arg0 + 4) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9B08);
#ifdef SKIP_ASM
int func_003C9B08(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9B10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C9BC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003C9B10_9BC8(void) __asm__("func_003C9B10");
void func_003C9B08_9BC8(void) __asm__("func_003C9B08");

void func_003C9BC8(int *a, int *b) {
    a[0] = (int)func_003C9B10_9BC8;
    a[7] = b[0];
    a[8] = 0;
    a[9] = b[4];
    a[10] = b[5];
    a[11] = b[6];
    b[10] = (int)func_003C9B08_9BC8;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9C08);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9D28);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9DA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9E50);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9F58);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9FF8);
#ifdef SKIP_ASM
int func_003C9FF8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x28)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA000);
#ifdef SKIP_ASM
extern void func_003CCF18(int);

void func_003CA000(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003CCF18(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA028);

INCLUDE_ASM("ealib/seg_2B4578", func_003CA0B8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA358);
#ifdef SKIP_ASM
int func_003CA358(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA360);
#ifdef SKIP_ASM
extern void func_003CCF18(int);

void func_003CA360(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003CCF18(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA388);

INCLUDE_ASM("ealib/seg_2B4578", func_003CA460);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA5E8);
#ifdef SKIP_ASM
extern void func_003CCF18(int);

void func_003CA5E8(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003CCF18(temp_4);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA610);

INCLUDE_ASM("ealib/seg_2B4578", func_003CA6A8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA7A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CA6A8_A7A0(void) __asm__("func_003CA6A8");

void func_003CA7A0(void *arg0) {
    *(int *)((char *)arg0 + 4) = 0;
    *(void (**)(void))((char *)arg0 + 0) = func_003CA6A8_A7A0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA7B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA870);
#ifdef SKIP_ASM
extern void func_003CA7B8();

int func_003CA870(void *arg0) {
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(void (**)())((char*)(arg0) + (0))) = func_003CA7B8;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    (*(signed char *)((char*)(arg0) + (0x1B))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA8A0);
#ifdef SKIP_ASM
void func_003CA8A0(void *arg0, void *arg1) {
    float temp_f1;
    float temp_f2;

    temp_f1 = (2.0f * (float) (*(int *)((char*)(arg1) + (0)))) / (float) (*(int *)((char*)(arg1) + (4)));
    temp_f2 = 1.0f - temp_f1;
    (*(float *)((char*)(arg0) + (0x24))) = temp_f1;
    (*(float *)((char*)(arg0) + (0x20))) = temp_f2;
    (*(float *)((char*)(arg0) + (0x24))) = (float) (temp_f1 * ((float) (*(int *)((char*)(arg1) + (8))) * 0.00390625f));
    (*(float *)((char*)(arg0) + (0x20))) = (float) (temp_f2 * ((float) (*(int *)((char*)(arg1) + (8))) * 0.00390625f));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA900);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA990);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_A990;
extern void func_003CEB90(void *);
extern void func_003CA900_x(void) __asm__("func_003CA900");

int func_003CA990(S_A990 *self) {
    self->vt = func_003CA900_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    func_003CEB90(&self->sub);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CA9D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAA28);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAB28);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAC88);

INCLUDE_ASM("ealib/seg_2B4578", func_003CADF0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAEA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAFD0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB110);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB1A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB280);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB328);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB4A8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CB528);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CB328_B528(void) __asm__("func_003CB328");

void func_003CB528(void (**arg0)(void)) {
    *arg0 = func_003CB328_B528;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CB538);
#ifdef SKIP_ASM
void func_003CB538(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x1C))) = arg1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CB540);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CBB28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_BB28_vt(void) __asm__("func_003CB540");

void func_003CBB28(int *self) {
    int i;
    self[1] = 0;
    self[8] = 0;
    self[0] = (int)f_BB28_vt;
    ((short *)self)[0x24 / 2] = 0;
    ((short *)self)[0x26 / 2] = 0;
    for (i = 3; i >= 0; i--)
        self[0xA + i] = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CBB78);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00515B54_BC00[4] __asm__("D_00515B54");

void func_003CBC00(int arg0) {
    D_00515B54_BC00[0] = arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00515D80_BC10[4] __asm__("D_00515D80");

int *func_003CBC10(void) {
    return D_00515D80_BC10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044E770_BC20[][6][6] __asm__("D_0044E770");

unsigned char func_003CBC20(int arg0, int arg1, int arg2) {
    return D_0044E770_BC20[arg0 - 1][arg1 - 1][arg2 - 1];
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CBC58);

INCLUDE_ASM("ealib/seg_2B4578", func_003CBEC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CC070);

INCLUDE_ASM("ealib/seg_2B4578", func_003CC630);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CC848);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003CBEC8();
struct S_C848 { char pad[0xF]; char a; int b; };
extern struct S_C848 D_00515B40_C848 __asm__("D_00515B40");

void func_003CC848(void) {
    D_00515B40_C848.b = 0;
    func_003CBEC8();
    D_00515B40_C848.a = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CC878);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CC918);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00517604_C918[4] __asm__("D_00517604");

void func_003CC918(int arg0) {
    D_00517604_C918[0] = arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CC928);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00517608_C928[4] __asm__("D_00517608");

void func_003CC928(int arg0) {
    D_00517608_C928[0] = arg0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CC938);

INCLUDE_ASM("ealib/seg_2B4578", func_003CCA08);

INCLUDE_ASM("ealib/seg_2B4578", func_003CCA38);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CCEF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00517604_CEF0[])(void) __asm__("D_00517604");
int func_003CCEF0(void) {
    return D_00517604_CEF0[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CCF18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00517608_CF18[])(void) __asm__("D_00517608");
int func_003CCF18_CF18(int) __asm__("func_003CCF18");
int func_003CCF18_CF18(int a) {
    return D_00517608_CF18[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CCF40);
#ifdef SKIP_ASM
void *func_003CCF40(void *arg0) {
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (0x98))) = 0;
    (*(int *)((char*)(arg0) + (0x9C))) = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CCF60);
#ifdef SKIP_ASM
int func_003CCF60(void *arg0, int arg1, int arg2, int arg3) {
    if (arg1 == 0) {
        return -1;
    }
    if ((*(int *)((char*)(arg0) + (8))) != 0) {
        return -1;
    }
    (*(int *)((char*)(arg0) + (8))) = arg3;
    (*(int *)((char*)(arg0) + (0xC))) = arg2;
    (*(int *)((char*)(arg0) + (0xA0))) = arg1;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CCF90);

INCLUDE_ASM("ealib/seg_2B4578", func_003CD138);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD190);
#ifdef SKIP_ASM
void func_003CD190(void *arg0, void *arg1) {
    (*(float *)((char*)(arg0) + (0x98))) = (float) (*(float *)((char*)(arg1) + (0)));
    (*(float *)((char*)(arg0) + (0x9C))) = (float) (*(float *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD1A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00517604_D1A8[])(void) __asm__("D_00517604");
int func_003CD1A8_D1A8(int) __asm__("func_003CD1A8");
int func_003CD1A8_D1A8(int a) {
    return D_00517604_D1A8[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD1D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00517608_D1D0[])(void) __asm__("D_00517608");
int func_003CD1D0_D1D0(int) __asm__("func_003CD1D0");
int func_003CD1D0_D1D0(int a) {
    return D_00517608_D1D0[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD1F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct BS_D1F8 { unsigned char *ptr; unsigned int bits; int cnt; } BS_D1F8;
extern unsigned int D_0044E8C0_E8C0[] __asm__("D_0044E8C0");

int func_003CD1F8(BS_D1F8 *p, int n) {
    unsigned int v = p->bits;
    int ret = v & D_0044E8C0_E8C0[n];
    p->bits >>= n;
    p->cnt -= n;
    if (p->cnt < 8) {
        p->bits |= *p->ptr++ << p->cnt;
        p->cnt += 8;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD260);
#ifdef SKIP_ASM
typedef struct { unsigned char *p; unsigned int bits; int n; } B_D260;

void func_003CD260(B_D260 *b, int k) {
    unsigned int t = b->bits >> k;
    int n = b->n - k;
    b->bits = t;
    b->n = n;
    if (n < 8) {
        unsigned char *q = b->p;
        unsigned int c = *q;
        b->n = n + 8;
        b->p = q + 1;
        b->bits = t | (c << n);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CD2B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CD518);

INCLUDE_ASM("ealib/seg_2B4578", func_003CD590);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CD690);
#ifdef SKIP_ASM
extern void func_003CD590(char *, void *);
extern void func_003CE410(int, void *, char *, char *);

void func_003CD690(char *arg0, int arg1, int arg2) {
    char buf[0x30];

    func_003CD590(arg0 + 0x114, buf);
    func_003CE410(arg2, buf, arg0 + 0x144, arg0 + (arg1 * 4 + 0x684));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CD6F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CD878);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CDDB8);
#ifdef SKIP_ASM
void *func_003CDDB8(void *arg0) {
    (*(int *)((char*)(arg0) + (0xD44))) = 0;
    (*(int *)((char*)(arg0) + (0xD64))) = 1;
    (*(int *)((char*)(arg0) + (0xD54))) = 0;
    (*(int *)((char*)(arg0) + (0xD58))) = 0;
    (*(short *)((char*)(arg0) + (0xD50))) = 0;
    (*(short *)((char*)(arg0) + (0xD52))) = 0;
    (*(int *)((char*)(arg0) + (0xD5C))) = 0;
    (*(int *)((char*)(arg0) + (0xD60))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CDDE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CDE68);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE078);
#ifdef SKIP_ASM
struct V3_E078 { int a; int b; int c; };

struct V3_E078 func_003CE078(char *self) {
    struct V3_E078 t;
    t.a = *(int *)(self + 4);
    t.c = *(int *)(self + 0xD5C);
    t.b = *(int *)(self + 0xD44);
    return t;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE0B8);
#ifdef SKIP_ASM
void func_003CE0B8(void *arg0, void *arg1) {
    (*(int *)((char*)(arg0) + (4))) = (int) (*(int *)((char*)(arg1) + (0)));
    (*(int *)((char*)(arg0) + (0xD5C))) = (int) (*(int *)((char*)(arg1) + (8)));
    (*(int *)((char*)(arg0) + (0xD44))) = (int) (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE0D8);
#ifdef SKIP_ASM
void func_003CE0D8(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0xD64))) = arg1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE0E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A8E8_E0E0[] __asm__("D_0050A8E8");
typedef struct { char pad[0x28]; short f; char pad2[0x62]; } R_E0E0;
typedef struct { R_E0E0 arr[100]; } W_E0E0;

int func_003CE0E0(int arg0) {
    char *p = D_0050A8E8_E0E0;
    int temp_2;
    int temp_4;

    temp_4 = arg0 + *(unsigned char *)(p + 0x30) + *(unsigned char *)(p + 0x2F);
    temp_2 = (*(W_E0E0 **)(p + 0x1F0))->arr[temp_4].f;
    return (temp_2 == -1) ? temp_4 : temp_2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CE118);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE198);
#ifdef SKIP_ASM
int func_003CE198(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE1A0);
#ifdef SKIP_ASM
int func_003CE1A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE1A8);
#ifdef SKIP_ASM
int func_003CE1A8(void) {
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CE1B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CE378);

INCLUDE_ASM("ealib/seg_2B4578", func_003CE410);

INCLUDE_ASM("ealib/seg_2B4578", func_003CE7C0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CEB90);
#ifdef SKIP_ASM
void func_003CEB90(void *arg0) {
    (*(int *)((char*)(arg0) + (0xC0))) = 0;
    (*(int *)((char*)(arg0) + (0xC4))) = 0;
    (*(int *)((char*)(arg0) + (0xC8))) = 0;
    (*(int *)((char*)(arg0) + (0xCC))) = 0;
    (*(int *)((char*)(arg0) + (0xD0))) = 0;
    (*(int *)((char*)(arg0) + (0xD4))) = 0;
    (*(int *)((char*)(arg0) + (0xD8))) = 0;
    (*(int *)((char*)(arg0) + (0xDC))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x18))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    (*(int *)((char*)(arg0) + (0x2C))) = 0;
    (*(int *)((char*)(arg0) + (0x90))) = 0;
    (*(int *)((char*)(arg0) + (0xA0))) = 0;
    (*(int *)((char*)(arg0) + (0xA4))) = 0;
    (*(int *)((char*)(arg0) + (0xB0))) = 0;
    (*(int *)((char*)(arg0) + (0xB4))) = 0;
    (*(int *)((char*)(arg0) + (0xB8))) = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CEBE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CEE08);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CEE88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_EE88;
extern void func_003CEB90(void *);
extern void func_003CEE08_x(void) __asm__("func_003CEE08");

int func_003CEE88(S_EE88 *self) {
    self->vt = func_003CEE08_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    func_003CEB90(&self->sub);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CEED0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CEF20);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CEFB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_EFB0;
extern void func_003CEB90(void *);
extern void func_003CEF20_x(void) __asm__("func_003CEF20");

int func_003CEFB0(S_EFB0 *self) {
    self->vt = func_003CEF20_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    func_003CEB90(&self->sub);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CEFF8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CF060);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF128);
#ifdef SKIP_ASM
extern void func_003CF060();

int func_003CF128(void *arg0) {
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(void (**)())((char*)(arg0) + (0))) = func_003CF060;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    (*(signed char *)((char*)(arg0) + (0x1B))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF158);
#ifdef SKIP_ASM
void func_003CF158(void *arg0, int *arg1) {
    (*(float *)((char*)(arg0) + (0x1C))) = (float) ((float) *arg1 * 0.00390625f);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF178);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF298);
#ifdef SKIP_ASM
extern void func_003B5E98(int);

void func_003CF298(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003B5E98(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF2C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CF178_F2C0(void) __asm__("func_003CF178");
void func_003CF298_F2C0(void) __asm__("func_003CF298");

int func_003CF2C0(char *arg0) {
    *(void (**)(void))(arg0 + 4) = func_003CF298_F2C0;
    *(void (**)(void))(arg0 + 0) = func_003CF178_F2C0;
    *(int *)(arg0 + 8) = 0;
    *(int *)(arg0 + 0xC) = 0;
    *(int *)(arg0 + 0x10) = 0;
    *(int *)(arg0 + 0x14) = 0;
    *(signed char *)(arg0 + 0x1B) = 0;
    *(int *)(arg0 + 0x1C) = 0;
    *(int *)(arg0 + 0x20) = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF300);

INCLUDE_ASM("ealib/seg_2B4578", func_003CF408);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF498);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CF408_F498(void) __asm__("func_003CF408");

int func_003CF498(char *arg0) {
    *(int *)(arg0 + 8) = 0;
    *(void (**)(void))(arg0 + 0) = func_003CF408_F498;
    *(int *)(arg0 + 0xC) = 0;
    *(int *)(arg0 + 0x10) = 0;
    *(int *)(arg0 + 0x14) = 0;
    *(signed char *)(arg0 + 0x1B) = 0;
    *(int *)(arg0 + 4) = 0;
    *(int *)(arg0 + 0x34) = 0;
    *(int *)(arg0 + 0x38) = 0;
    *(int *)(arg0 + 0x3C) = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF4D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CF5D0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF6D0);
#ifdef SKIP_ASM
extern void func_003B5E98(int);

void func_003CF6D0(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
    if (temp_4 != 0) {
        func_003B5E98(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF6F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; void *f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int f1c; int f20; short f24; short f26; } S_F6F8;
extern void f_F6F8_a(void) __asm__("func_003CF5D0");
extern void f_F6F8_b(void) __asm__("func_003CF6D0");

int func_003CF6F8(S_F6F8 *self) {
self->f26 = 1;
    self->f4 = f_F6F8_b;
    self->vt = f_F6F8_a;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f20 = 0;
    self->f1c = 0;
    self->f24 = 0;
    self->f8 = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF740);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF798);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CF740_F798(void) __asm__("func_003CF740");

void func_003CF798(void *arg0, int arg1) {
    *(int *)((char *)arg0 + 0x1C) = arg1;
    *(int *)((char *)arg0 + 4) = 0;
    *(void (**)(void))((char *)arg0 + 0) = func_003CF740_F798;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF7B0);
#ifdef SKIP_ASM
int func_003CF7B0(void *arg0) {
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    (*(signed char *)((char*)(arg0) + (0x1B))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF7D0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF958);
#ifdef SKIP_ASM
void func_003B5E98(int);

void func_003CF958(char *arg0) {
    int temp_4;

    temp_4 = *(int *)(arg0 + 0x20);
    if (temp_4 != 0) {
        func_003B5E98(temp_4);
        *(int *)(arg0 + 0x20) = 0;
        *(int *)(arg0 + 0x1C) = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF998);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CF7D0_F998(void) __asm__("func_003CF7D0");
void func_003CF958_F998(void) __asm__("func_003CF958");

int func_003CF998(char *arg0) {
    *(void (**)(void))(arg0 + 4) = func_003CF958_F998;
    *(void (**)(void))(arg0 + 0) = func_003CF7D0_F998;
    *(int *)(arg0 + 8) = 0;
    *(int *)(arg0 + 0xC) = 0;
    *(int *)(arg0 + 0x10) = 0;
    *(int *)(arg0 + 0x14) = 0;
    *(signed char *)(arg0 + 0x1B) = 0;
    *(int *)(arg0 + 0x20) = 0;
    *(int *)(arg0 + 0x1C) = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CF9D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003CFA98);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CFB38);
#ifdef SKIP_ASM
extern int func_003CFB98(int, int);

int func_003CFB38(int arg0, int arg1) {
    return func_003CFB98(arg0, arg1 / 16) * 16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CFB68);
#ifdef SKIP_ASM
int func_003CFB68(int n) {
    int i = 0, sq;
    for (;;) {
        sq = i * i;
        i++;
        if (sq != n) {
            if (n < sq) return i - 2;
        } else
            return i - 1;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CFB98);

INCLUDE_ASM("ealib/seg_2B4578", func_003CFC30);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0B18);
#ifdef SKIP_ASM
typedef struct { int a, b, c, d; } E_0B18;
typedef struct { int p0, p1, p2; unsigned lo : 16; unsigned idx : 6; unsigned hi : 10; E_0B18 e[1]; } Q_0B18;

void func_003D0B18(Q_0B18 *q, E_0B18 *src) {
    E_0B18 *arr = q->e;
    unsigned i = q->idx;
    q->idx = i + 1;
    arr[i] = *src;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D0B78);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0C20);
#ifdef SKIP_ASM
void func_003D0C20(unsigned char *a0, unsigned char *a1) {
    unsigned char *p = a0 + 0x10;

    while (a1 >= p && p[9] != 1) {
        *(unsigned int *)(p + 8) |= 0x04000000;
        p += 0x10;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0C78);
#ifdef SKIP_ASM
void func_003D0C78(int arg0, unsigned int arg1) {
    unsigned int temp_4;

    temp_4 = arg0 + 0x10;
    if (arg1 >= temp_4) {
        (*(int *)((char*)(temp_4) + (8))) = (int) ((*(int *)((char*)(temp_4) + (8))) | 0x04000000);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0CA0);
#ifdef SKIP_ASM
extern void func_003D0CC8(int);
extern int func_003D5290();

void func_003D0CA0(void) {
    func_003D0CC8(func_003D5290());
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D0CC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D0D70);

INCLUDE_ASM("ealib/seg_2B4578", func_003D0ED0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1038);

INCLUDE_ASM("ealib/seg_2B4578", func_003D11B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1470);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D15C8);
#ifdef SKIP_ASM
int func_003D15C8(int arg0, int arg1) {
    char *t = (char *)func_003D5290();
    if (t == 0) {
        return -8;
    }
    *(int *)(t + 0x24) = arg1 & 0xFFFF;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1608);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_0044F430_1608[] __asm__("D_0044F430");
extern void func_00411310_1608(int, ...) __asm__("func_00411310") __attribute__((noreturn));

void func_003D1608(int a, ...) {
    D_0044F430_1608[0] = 0xFFFFFFFF;
    func_00411310_1608(0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1658);
#ifdef SKIP_ASM
void func_003D1658(int a, ...) {
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1690);
#ifdef SKIP_ASM
void func_003D1690(int a, ...) {
}
#endif

void func_003D16C8(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D16D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F43C_16D0[4] __asm__("D_0044F43C");

int func_003D16D0_16D0(void) __asm__("func_003D16D0");
int func_003D16D0_16D0(void) {
    return D_0044F43C_16D0[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D16E0);
#ifdef SKIP_ASM
int func_003D16E0(int arg0) {
    return arg0;
}
#endif

void func_003D16E8(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", func_003D16F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1940);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1A88);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1BA8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1D28);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1E80);

INCLUDE_ASM("ealib/seg_2B4578", func_003D1F80);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2068);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2138);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D21C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_0044F420_f420[] __asm__("D_0044F420");

void func_003D21C0(char *arg0, int arg1) {
    char *g = D_0044F420_f420[0];
    char *p = *(char **)(g + 0x34) + (*(short *)(*(char **)(g + 0x40) + *(unsigned char *)(arg0 + 0xC) * 2) * 4);
    if (p != 0) {
        *(unsigned *)(p + 0xC) = (*(unsigned *)(p + 0xC) & 0xDFFFFFFF) | ((arg1 & 1) << 29);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D2218);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2290);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2350);

INCLUDE_ASM("ealib/seg_2B4578", func_003D25F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2A68);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2C20);

INCLUDE_ASM("ealib/seg_2B4578", func_003D2DF8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2FA0);
#ifdef SKIP_ASM
void func_003D2FA0(void) {
    func_003D16C8();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D2FC0);
#ifdef SKIP_ASM
extern void func_003D16D0();

void func_003D2FC0(void) {
    func_003D16D0();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D2FE0);

void func_003D3010(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3018);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F424_3018[4] __asm__("D_0044F424");
extern int D_0044F428_3018[4] __asm__("D_0044F428");

void func_003D3018(int arg0, int arg1) {
    D_0044F424_3018[0] = arg0;
    D_0044F428_3018[0] = arg1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3030);
#ifdef SKIP_ASM
int func_003D3030(void) {
    char *temp_2;

    temp_2 = (char *)func_003D5290();
    if (temp_2 == 0) {
        return -8;
    }
    return *(int *)(temp_2 + 0x54);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3058);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002AE478_3058() __asm__("func_002AE478");

int func_003D3058(int n) {
    int t = func_002AE478_3058() + 0x110;
    t += n * 0x10;
    return (t / 16) * 16 + 0x10;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D30A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D3140);

INCLUDE_ASM("ealib/seg_2B4578", func_003D31E8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D32A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044ED48_32A8[4] __asm__("D_0044ED48");

int *func_003D32A8(void) {
    return D_0044ED48_32A8;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D32B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D3658);

INCLUDE_ASM("ealib/seg_2B4578", func_003D37A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D3900);

INCLUDE_ASM("ealib/seg_2B4578", func_003D3C90);

INCLUDE_ASM("ealib/seg_2B4578", func_003D3D20);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4050);

INCLUDE_ASM("ealib/seg_2B4578", func_003D41A8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4898);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct E_4898 { short v; short w; } E_4898;
typedef struct B_4898 { E_4898 ent[0x40]; } B_4898;
typedef struct A_4898 { char pad0[0x34]; B_4898 *b; short *idx; char pad1[0x50 - 0x3C]; char *arr; } A_4898;
extern A_4898 *D_0044F420_F420[] __asm__("D_0044F420");

static inline int get_4898(char *arr, int i) {
    return *(int *)(arr + i * 8);
}

int func_003D4898(int arg0) {
    int var_7;
    A_4898 *a;
    B_4898 *b;
    short t;

    var_7 = 0;
    if (arg0 < 0) return 0;
    a = D_0044F420_F420[0];
    b = a->b;
    if (arg0 >= *(unsigned short *)((char *)b + 0x12)) return 0;
    t = b->ent[a->idx[arg0]].v;
    if (t > 0) {
        var_7 = get_4898(a->arr, t - 1);
    }
    return var_7;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4900);
#ifdef SKIP_ASM
struct S_4900 {
    int a0;
    int a4;
    int a8;
    int ac;
    unsigned int a10;
    int a14;
    int a18;
    int a1c;
};
extern int func_003D32B8(int, struct S_4900 *);

int func_003D4900(int a0, int *a1) {
    struct S_4900 s;

    if (func_003D32B8(a0, &s) < 0) {
        s.a4 = 0;
        s.a10 = 0xFFFFFFFF;
    }
    if (a1 != 0) {
        *a1 = s.a4;
    }
    return s.a10;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D4950);

INCLUDE_ASM("ealib/seg_2B4578", func_003D49D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4A50);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4B00);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4BB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4C40);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4F10);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5128);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5290);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5330);

INCLUDE_ASM("ealib/seg_2B4578", func_003D53B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5450);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D54C8);
#ifdef SKIP_ASM
extern void func_003D5128(int);

void func_003D54C8(void) {
    func_003D5128(0x20);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D54E8);
#ifdef SKIP_ASM
extern void func_003D5128(int);

void func_003D54E8(void) {
    func_003D5128(0x42);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D5508);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5698);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5800);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5968);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5A98);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5CC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5EE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6108);

INCLUDE_ASM("ealib/seg_2B4578", func_003D61E8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D62C8);
#ifdef SKIP_ASM
extern void func_002AF6C0(int, int);

void func_003D62C8(void *arg0, signed char arg1) {
    (*(signed char *)((char*)(arg0) + (4))) = arg1;
    if ((*(signed char *)((char*)(arg0) + (0xE))) < 0) {
        func_002AF6C0((*(int *)((char*)(arg0) + (0x54))), (int) ((*(signed char *)((char*)(arg0) + (0x34))) * arg1) / 100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6318);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D6318_cb(int, int, int, char) __asm__("func_002AF608");

void func_003D6318(char *self, int v, int w) {
    int x;
    self[0xE] = v;
    x = (v * self[4]) / 100;
    self[0xD] = 0;
    func_003D6318_cb(*(int *)(self + 0x54), x, w, 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6360);
#ifdef SKIP_ASM
void func_003D6360(void *arg0, int arg1, int arg2, int arg3) {
    *(signed char *)((char *)arg0 + 0x16) = arg1;
    if (arg3 >= 0) {
        *(signed char *)((char *)arg0 + 0x15) = arg3;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6378);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D63F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F438_63F8[] __asm__("D_0044F438");
extern signed char f_63F8_cb(int) __asm__("func_002AF4B0");

void func_003D63F8(char *self, int a, int b) {
    signed char t;
    *(int *)(self + 0x10) = D_0044F438_63F8[0];
    self[0x15] = b;
    t = f_63F8_cb(*(int *)(self + 0x54));
    self[0x16] = a;
    self[0x14] = t;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6448);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F438_6448[] __asm__("D_0044F438");
extern signed char f_6448_cb(int) __asm__("func_002AF480");

void func_003D6448(char *self, int a, int b) {
    signed char t;
    *(int *)(self + 0x18) = D_0044F438_6448[0];
    self[0x1D] = b;
    t = f_6448_cb(*(int *)(self + 0x54));
    self[0x1E] = a;
    self[0x1C] = t;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6498);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6618);

INCLUDE_ASM("ealib/seg_2B4578", func_003D66C0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D66F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFB8_FFB8[] __asm__("D_0044FFB8");

int func_003D66F0(int arg0, int arg1) {
    int i;
    if (arg0 == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (D_0044FFB8_FFB8[i * 2] == arg0 && D_0044FFB8_FFB8[i * 2 + 1] == arg1) {
            D_0044FFB8_FFB8[i * 2] = 0;
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6748);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6768);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFA8_6768[4] __asm__("D_0044FFA8");

int *func_003D6768(void) {
    return D_0044FFA8_6768;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6778);

INCLUDE_ASM("ealib/seg_2B4578", func_003D67F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6840);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6940);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_6940 { int pad0; int a; int b; };
extern struct S_6940 D_0044FF90_6940 __asm__("D_0044FF90");
extern int D_004A4820;
void func_003D6940(int x, int y) {
    if (D_004A4820 == 0x01789A34) {
        D_0044FF90_6940.a = x;
        D_0044FF90_6940.b = y;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6968);
#ifdef SKIP_ASM
extern int D_0044FFA0[];
extern int D_004A4820;

void func_003D6968(int arg0) {
    if (D_004A4820 == 0x01789A34) {
        D_0044FFA0[0] = arg0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6988);
#ifdef SKIP_ASM
extern int D_0044FF9C[];
extern int D_004A4820;

void func_003D6988(int arg0) {
    if (D_004A4820 == 0x01789A34) {
        D_0044FF9C[0] = arg0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D69A8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_69A8 { unsigned short h; int w; };
extern struct E_69A8 D_00450660_69A8[] __asm__("D_00450660");

void func_003D69A8(void) {
    long i;
    for (i = 0; i < 8; i++) {
        D_00450660_69A8[(int)i].h = 0xFFFF;
        D_00450660_69A8[(int)i].w = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D69F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6BC0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6C58);
#ifdef SKIP_ASM
int func_003D6C58(int arg0) {
    return arg0 * 4;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_6C60 __asm__("D_004A4824");
extern int D_004A4828_6C60 __asm__("D_004A4828");

void func_003D6C60(int arg0, int *arg1) {
    int i;

    D_004A4828_6C60 = arg0;
    D_004A4824_6C60 = arg1;
    if (arg1 != 0) {
        for (i = 0; i < D_004A4828_6C60; i++) {
            D_004A4824_6C60[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6CA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_6CA0 __asm__("D_004A4824");

void func_003D6CA0(int arg0) {
    if (D_004A4824_6CA0 != 0) {
        D_004A4824_6CA0[arg0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6CC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A4820;
extern int *D_004A4824_6CC0 __asm__("D_004A4824");

int func_003D6CC0(int arg0) {
    int r = 0;
    if (D_004A4820 == 0x01789A34 && D_004A4824_6CC0 != 0) {
        r = D_004A4824_6CC0[arg0];
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6CF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char **D_004A4824_6CF8 __asm__("D_004A4824");

int func_003D6CF8(int arg0, unsigned int arg1) {
    int r = 0;
    unsigned short *p;

    if (D_004A4824_6CF8 != 0 && arg0 >= 0) {
        p = (unsigned short *)(D_004A4824_6CF8[arg0] + 0xA);
        if (*p != 0xFFFF) {
            r = arg1 < *p;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6D30);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

void func_003D6D30(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6D40);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

void func_003D6D40(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6D50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short **D_004A4824_6D50 __asm__("D_004A4824");
extern int D_004A4828_6D50 __asm__("D_004A4828");

int func_003D6D50(unsigned short arg0) {
    unsigned short **p;
    short i;

    p = D_004A4824_6D50;
    if (p != 0) {
        for (i = 0; i < D_004A4828_6D50; i++) {
            if (*p != 0 && **p == arg0) {
                return i;
            }
            p++;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6DB8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6E30);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6EE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6F80);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7038);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D70B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A4824_4824 __asm__("D_004A4824");
extern int D_004A4828_4828 __asm__("D_004A4828");

int func_003D70B0(void) {
    int i;
    int r = -1;

    for (i = 0; i < D_004A4828_4828; i++) {
        if (D_004A4824_4824[i] == 0) {
            r = i;
            break;
        }
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D7110);

INCLUDE_ASM("ealib/seg_2B4578", func_003D71D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D72A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7418);

INCLUDE_ASM("ealib/seg_2B4578", func_003D76F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7760);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7A50);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7B98);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7C38);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7D58);

INCLUDE_ASM("ealib/seg_2B4578", func_003D7EC8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8008);
#ifdef SKIP_ASM
typedef struct { short a; signed char b; signed char c; } D8008_S;

int func_003D8008(int arg0, int arg1, int arg2) {
    D8008_S u;
    u.c = arg0;
    u.b = arg1;
    u.a = arg2;
    return *(int *)&u;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8028);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFF8_8028[128] __asm__("D_0044FFF8");

int func_003D8028(int arg0) {
    return D_0044FFF8_8028[arg0];
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D8040);

INCLUDE_ASM("ealib/seg_2B4578", func_003D80B0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8158);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004506A0_8158[128] __asm__("D_004506A0");

void func_003D8158(int arg0, int arg1) {
    D_004506A0_8158[arg1] = arg0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D8170);

INCLUDE_ASM("ealib/seg_2B4578", func_003D8190);

INCLUDE_ASM("ealib/seg_2B4578", func_003D81F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8278);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFB8_8278[] __asm__("D_0044FFB8");

void func_003D8278(void) {
    int *var_2;
    int var_3;

    var_3 = 7;
    var_2 = D_0044FFB8_8278;
    do {
        var_2[0] = 0;
        var_3 -= 1;
        var_2[1] = 0;
        var_2 += 2;
    } while (var_3 >= 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D82B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D8330);

INCLUDE_ASM("ealib/seg_2B4578", func_003D83B8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8460);
#ifdef SKIP_ASM
unsigned short *func_003D8460(void *arg0, int arg1) {
    int var_7;
    unsigned short *var_2;
    unsigned short *var_6;
    unsigned short temp_8;

    temp_8 = (*(unsigned short *)((char*)(arg0) + (0x10)));
    var_7 = 0;
    if (temp_8 != 0) {
        var_6 = arg0 + 0x18;
loop_2:
        var_2 = arg0 + (*var_6 * 4);
        var_7 += 1;
        if (*var_2 != arg1) {
            var_6 += 1;
            if (var_7 >= (int) temp_8) {
                goto block_4;
            }
            goto loop_2;
        }
    } else {
block_4:
        var_2 = 0;
    }
    return var_2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D84A0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8500);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_8500 { unsigned int z[8]; unsigned int a[8]; unsigned int b[8]; };
extern struct S_8500 D_0044FFF8_8500 __asm__("D_0044FFF8");
int func_003D8500(int i) {
    int r = D_0044FFF8_8500.a[i] < 0x10;
    if (D_0044FFF8_8500.b[i] >= 0x10) r = 0;
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8530);
#ifdef SKIP_ASM
void func_003D8530(void *arg0) {
    *(int *)((char *)arg0 + 0) = 0;
    *(int *)((char *)arg0 + 4) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8540);
#ifdef SKIP_ASM
int func_003D8540(int arg0, int *arg1) {
    unsigned key = arg0 & 0xFFFF;
    int i;
    int found = 0;
    for (i = 0; i < arg1[0]; i++) {
        if ((*(unsigned short **)(arg1 + 1))[i] == key) {
            found = 1;
            break;
        }
    }
    return found;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D8598);

INCLUDE_ASM("ealib/seg_2B4578", func_003D85C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D8780);

INCLUDE_ASM("ealib/seg_2B4578", func_003D88C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D8CB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D8E58);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9088);

INCLUDE_ASM("ealib/seg_2B4578", func_003D93C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D95B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9830);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9A40);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9AC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9BD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9ED0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D9FB0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA3A8);
#ifdef SKIP_ASM
void func_003DA3A8(void *arg0, int *arg1, int *arg2) {
    *arg1 = (int) (*(unsigned char *)((char*)(arg0) + (8)));
    *arg2 = (int) (*(unsigned char *)((char*)(arg0) + (9)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA3C0);
#ifdef SKIP_ASM
extern int D_004A483C;

void func_003DA3C0(int arg0) {
    D_004A483C = arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA3C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_004506E0_A3C8[] __asm__("D_004506E0");
unsigned int func_003DA3C8(unsigned int a) {
    a &= 0xFF;
    return (a & 0x1F) * D_004506E0_A3C8[a >> 5];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA3F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_A3F0 { int a; int b; int c; };
extern struct S_A3F0 D_00450700_A3F0 __asm__("D_00450700");
int func_003DA3F0(int x) {
    int r = 0;
    if (D_00450700_A3F0.a == x)
        r = D_00450700_A3F0.b > 0;
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DA418);

INCLUDE_ASM("ealib/seg_2B4578", func_003DA4E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DA578);

INCLUDE_ASM("ealib/seg_2B4578", func_003DA5C8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA6B0);
#ifdef SKIP_ASM
void func_003DA6B0(char *arg0, int arg1) {
    int temp_8;
    int temp_3;
    unsigned char *temp_4;

    temp_8 = *(unsigned char *)(arg0 + 4) & 0xF;
    temp_3 = arg1 / 8;
    temp_4 = (unsigned char *)(arg0 + ((*(unsigned char *)(arg0 + 5) * (temp_8 + 2) + 0xF) & 0x3FFC) + temp_8 * 4 + (temp_3 + 1));
    *temp_4 &= ~(1 << (arg1 - temp_3 * 8));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA718);
#ifdef SKIP_ASM
int func_003DA718(unsigned char *arg0, int arg1) {
    int temp_3 = arg1 / 8;
    unsigned char m = 1 << (arg1 - temp_3 * 8);
    return arg0[temp_3] & m;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DA750);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA810);
#ifdef SKIP_ASM
struct S_A810 {
    char pad[0x61];
    unsigned char n;
    signed char buf[1];
};

int func_003DA810(struct S_A810 *s, int c) {
    int r = 0;
    if (s->n < 0xC8U) {
        r = 1;
        s->buf[s->n] = c;
        s->n++;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DA848);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA8E0);
#ifdef SKIP_ASM
extern int D_004A4838;

int func_003DA8E0(int arg0) {
    int var_2;

    var_2 = 0;
    if (D_004A4838 != 0) {
        var_2 = (int) (arg0 * 0x64) / (int) D_004A4838;
    }
    return var_2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DA910);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA9C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_A9C8 { unsigned short h; int w; };
extern struct E_A9C8 D_00450660_A9C8[] __asm__("D_00450660");

int func_003DA9C8(char *p, int idx) {
    int r = 1;
    if (*(unsigned short *)p == D_00450660_A9C8[idx].h) {
        int v = p[8];
        if (v > 0) {
            if (!(D_00450660_A9C8[idx].w < v)) {
                r = 0;
            }
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DAA10);
#ifdef SKIP_ASM
int func_003DAA10(void *arg0, int arg1) {
    int temp_3;

    temp_3 = (*(unsigned char *)((char*)(arg0) + (4))) & 3;
    if (((temp_3 == 1) && (arg1 == 2)) || ((temp_3 == 2) && (arg1 != temp_3))) {
        return 1;
    }
    return temp_3 == 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DAA50);
#ifdef SKIP_ASM
void func_003DAA50(unsigned char *arg0) {
    int *q;
    int i;
    q = (int *)(arg0 + ((((arg0[4] >> 2) + 3) & 0x7C) + 8) + (arg0[6] * 4));
    for (i = 0; i < arg0[5]; i++) {
        q[i] = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DAAB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DAB58);

INCLUDE_ASM("ealib/seg_2B4578", func_003DAC20);

INCLUDE_ASM("ealib/seg_2B4578", func_003DACA8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DACE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DAE48);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DAFD0);
#ifdef SKIP_ASM
void func_003DAFD0(void *arg0) {
    int var_3;
    signed char *var_2;

    (*(signed char *)((char*)(arg0) + (0x61))) = 0;
    var_3 = 0xB;
    (*(signed char *)((char*)(arg0) + (0x60))) = 0;
    var_2 = arg0 + 0x5F;
    do {
        *var_2 = 0;
        var_3 -= 1;
        var_2 -= 8;
    } while (var_3 >= 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DB008);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB040);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB208);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB3B8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DB3E0);
#ifdef SKIP_ASM
typedef struct { char pad[6]; unsigned char n; } H_B3E0;
int func_003DB3E0(H_B3E0 *h, unsigned int idx) {
    int c[3];
    int r = -1;
    unsigned char *tbl = (unsigned char *)h + (((h->n * 2 + 3) & 0x3FC) + 0xC);

    if (idx < 8) {
        unsigned char *e = (unsigned char *)(idx * 3 + (unsigned)tbl); // PORT: ptr in int
        c[0] = e[0];
        c[1] = e[1];
        c[2] = e[2];
        r = c[0];
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DB440);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB4D0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB5C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB790);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB838);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DB978);
#ifdef SKIP_ASM
struct S_B978 { int a; int b; int c; unsigned char r, g, bl, al; };
void func_003DB978(struct S_B978 *s) {
    s->b = -1; s->a = 0; s->c = 0; s->r = 0xFF; s->g = 0xFF; s->bl = 0xFF; s->al = 0xFF;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DB9A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DB9D8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBAA8);
#ifdef SKIP_ASM
void func_003DBAA8(void *arg0, int arg1, unsigned int arg2) {
    if (arg1 == 0) {
        *(unsigned short *)((char *)arg0 + 0xE) = arg2;
        *(unsigned char *)((char *)arg0 + 0xE) = arg2;
        *(unsigned char *)((char *)arg0 + 0xF) = arg2 >> 8;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBAC8);
#ifdef SKIP_ASM
int func_003DBAC8(unsigned short *arg0, void *arg1) {
    return *arg0 - (((*(unsigned char *)((char*)(arg1) + (1))) << 8) | (*(unsigned char *)((char*)(arg1) + (0))));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DBAE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DBB68);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBC98);
#ifdef SKIP_ASM
int func_003DBC98(int arg0) {
    return arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", USTR_length);
#ifdef SKIP_ASM
int USTR_length(unsigned short *arg0) {
    int var_3;
    unsigned short *var_4;

    var_4 = arg0;
    var_3 = 0;
    if (*var_4 != 0) {
        do {
            var_4 += 1;
            var_3 += 1;
        } while (*var_4 != 0);
    }
    return var_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBCD8);
#ifdef SKIP_ASM
int func_003DBCD8(unsigned short *arg0, int arg1) {
    int i;
    unsigned short key = arg1;

    i = 0;
    if (*arg0 != 0) {
        do {
            if (*arg0 == key) {
                return i;
            }
            arg0++;
            i++;
        } while (*arg0 != 0);
    }
    return 0xFFFF;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DBD18);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBDB0);
#ifdef SKIP_ASM
extern void func_003DBDD0(int, int, int);

void func_003DBDB0(int a, int b) {
    func_003DBDD0(a, b, 0x7FFFFFFF);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DBDD0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", USTR_copy);
#ifdef SKIP_ASM
extern void USTR_ncopy(int, int, int);

void USTR_copy(int a, int b) {
    USTR_ncopy(a, b, 0x7FFFFFFF);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", USTR_ncopy);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBE90);
#ifdef SKIP_ASM
void func_003DBE90(short *arg0, signed char *arg1) {
    short *var_3;
    int var_6;
    signed char *var_5;
    unsigned char temp_2;

    var_5 = arg1;
    var_6 = 0;
    if (*var_5 != 0) {
        var_3 = arg0;
        do {
            temp_2 = *var_5;
            var_6 += 1;
            var_5 += 1;
            *var_3 = (short) temp_2;
            var_3 += 1;
        } while (*var_5 != 0);
    }
    arg0[var_6] = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBED0);
#ifdef SKIP_ASM
extern void func_003DBEF0(int, int, int, int);

void func_003DBED0(int a, int b, int c) {
    func_003DBEF0(a, b, c, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DBEF0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBFD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
unsigned short *func_003DBFD8_BFD8(unsigned short *out, unsigned int n, unsigned int base, unsigned short *digits) __asm__("func_003DBFD8");
unsigned short *func_003DBFD8_BFD8(unsigned short *out, unsigned int n, unsigned int base, unsigned short *digits) {
    if (n == 0) {
        *--out = digits[0];
    } else {
        do {
            *--out = digits[n % base];
            n /= base;
        } while (n != 0);
    }
    return out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DC038);
#ifdef SKIP_ASM
extern void func_003DBFD8(int, int);

void func_003DC038(int arg0, int arg1) {
    if (arg1 < 0) {
        arg1 = -arg1;
    }
    func_003DBFD8(arg0, arg1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DC068);

INCLUDE_ASM("ealib/seg_2B4578", func_003DC148);

INCLUDE_ASM("ealib/seg_2B4578", USTR_vsprintf);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DC9F8);
#ifdef SKIP_ASM
extern void USTR_vsprintf(char *, const char *, char *);

// PORT: SN-specific va_start
void func_003DC9F8(char *dst, const char *fmt, ...) {
    char *ap;
    ap = (char*)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    USTR_vsprintf(dst, fmt, ap);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DCA40);

INCLUDE_ASM("ealib/seg_2B4578", func_003DCB20);

INCLUDE_ASM("ealib/seg_2B4578", func_003DCBD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DCC88);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DCD98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C5C_CD98[] __asm__("D_00450C5C");

int func_003DCD98(int a, int b, int c) {
    int x = a ? a : 0x10;
    int y = b ? b : 0x3E8;
    int z = c ? c : 0x20;
    int sz = D_00450C5C_CD98[0] + 0xC;
    return z * 0x30 + x * 0x110 + y * sz + 0x40;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DCDE0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DCE90);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DCF10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519AD8_CF10[] __asm__("D_00519AD8");
typedef struct { unsigned char a, b, c, d; } T_CF10;

int func_003DCF10(T_CF10 arg0) {
    return *(int *)((char*)(D_00519AD8_CF10[0]) + arg0.d * 0x30 + 0xC);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DCF40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519AD8_CF40[] __asm__("D_00519AD8");
typedef struct { unsigned char a, b, c, d; } T_CF40;

int func_003DCF40(T_CF40 arg0) {
    return *(int *)((char*)(D_00519AD8_CF40[0]) + arg0.d * 0x30 + 0x14);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DCF70);

INCLUDE_ASM("ealib/seg_2B4578", func_003DCFC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD148);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD1D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD310);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD438);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD4E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD5A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD648);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD720);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD7E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD878);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD8E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DDA10);

INCLUDE_ASM("ealib/seg_2B4578", func_003DDAC0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DDC00);
#ifdef SKIP_ASM
extern void func_003DD310();
extern void iFILESYS_ExecCommand(int);

void func_003DDC00(int arg0, int arg1, int arg2) {
    func_003DD310();
    iFILESYS_ExecCommand(arg2);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DDC30);

INCLUDE_ASM("ealib/seg_2B4578", FILESYS_atomic);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DDE50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519AB8_DE50[] __asm__("D_00519AB8");
void iFILESYS_ExecCommand_DE50(int) __asm__("iFILESYS_ExecCommand");
void func_003DDE50(int a) {
    D_00519AB8_DE50[0] = a;
    iFILESYS_ExecCommand_DE50(0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DDE78);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DDF80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_00519AF0_DF80[] __asm__("D_00519AF0");
void strncpy_DF80(char *, char *, int) __asm__("strncpy");
void func_003DDF80(char *dst) {
    strncpy_DF80(dst, D_00519AF0_DF80, 0x100);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", iFILESYS_ExecCommand);

INCLUDE_ASM("ealib/seg_2B4578", iFILESYS_CommandCompleteCallback);

void func_003DE4C8(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", FILESYS_bypassqueuefileinfo);

void func_003DE668(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", func_003DE670);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DE7B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519AC4_9AC4[] __asm__("D_00519AC4");
extern void MUTEX_unlock(int *);

void func_003DE7B0(int a0) {
    MUTEX_lock(D_00519AC4_9AC4);
    func_003E6448(a0, 0, 0x30);
    MUTEX_unlock(D_00519AC4_9AC4);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DE800);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DE8C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519AC4_9AC4[] __asm__("D_00519AC4");
extern void MUTEX_unlock(int *);

void func_003DE8C0(int a0) {
    MUTEX_lock(D_00519AC4_9AC4);
    func_003E6448(a0, 0, 0x110);
    MUTEX_unlock(D_00519AC4_9AC4);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DE910);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C10_E910[4] __asm__("D_00450C10");

void func_003DE910(int arg0, int arg1) {
    D_00450C10_E910[1] = arg0;
    D_00450C10_E910[2] = arg1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DE928);

INCLUDE_ASM("ealib/seg_2B4578", func_003DE9B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DEA48);

INCLUDE_ASM("ealib/seg_2B4578", func_003DEB50);

INCLUDE_ASM("ealib/seg_2B4578", func_003DEBF0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DEC60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003DD648_EC60(void) __asm__("func_003DD648");
extern void func_003DEB50_EC60(int, int, int, int, int, void (*)(void)) __asm__("func_003DEB50");

void func_003DEC60(int a, int b, int c, int d, int e) {
    func_003DEB50_EC60(a, b, c, d, e, func_003DD648_EC60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DEC80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003DD720_EC80(void) __asm__("func_003DD720");
extern void func_003DEB50_EC60(int, int, int, int, int, void (*)(void)) __asm__("func_003DEB50");

void func_003DEC80(int a, int b, int c, int d, int e) {
    func_003DEB50_EC60(a, b, c, d, e, func_003DD720_EC80);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DECA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003DD1D8_ECA0(int) __asm__("func_003DD1D8");
int func_003DD310_ECA0(int) __asm__("func_003DD310");
int func_003DD5A0_ECA0(int, int, int) __asm__("func_003DD5A0");

int func_003DECA0(int a, int b) {
    int temp_2;
    int var_2 = 0;

    temp_2 = func_003DD5A0_ECA0(a, b, 0);
    if (temp_2 != 0) {
        func_003DD1D8_ECA0(temp_2);
        var_2 = func_003DD310_ECA0(temp_2) != 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DECF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003DD1D8_ECF8(int) __asm__("func_003DD1D8");
int func_003DD310_ECF8(int) __asm__("func_003DD310");
int func_003DD7E0_ECF8(int, int, int) __asm__("func_003DD7E0");

int func_003DECF8(int a, int b) {
    int temp_2;
    int var_2 = 0;

    temp_2 = func_003DD7E0_ECF8(a, b, 0);
    if (temp_2 != 0) {
        func_003DD1D8_ECF8(temp_2);
        var_2 = func_003DD310_ECF8(temp_2);
    }
    return var_2;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DED50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DEDC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003DD1D8_EDC0(int) __asm__("func_003DD1D8");
int func_003DD310_EDC0(int) __asm__("func_003DD310");
int func_003DDC30_EDC0(int, int, int) __asm__("func_003DDC30");

int func_003DEDC0(int a, int b) {
    int temp_2;
    int var_2 = 0;

    temp_2 = func_003DDC30_EDC0(a, b, 0);
    if (temp_2 != 0) {
        func_003DD1D8_EDC0(temp_2);
        var_2 = func_003DD310_EDC0(temp_2) != 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DEE18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003DD1D8_EE18(int) __asm__("func_003DD1D8");
int func_003DD310_EE18(int) __asm__("func_003DD310");
int func_003DD438_EE18(int, int, int) __asm__("func_003DD438");

int func_003DEE18(int a, int b) {
    int temp_2;
    int var_2 = 0;

    temp_2 = func_003DD438_EE18(a, b, 0);
    if (temp_2 != 0) {
        func_003DD1D8_EE18(temp_2);
        var_2 = func_003DD310_EE18(temp_2) != 0;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", queueadd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_qa(void *) __asm__("MUTEX_unlock");
extern char D_00519C08_9C08[] __asm__("D_00519C08");

void queueadd(char *arg0, char *arg1) {
    MUTEX_lock(D_00519C08_9C08);
    if (*(char **)arg0 == 0) {
        *(char **)arg0 = arg1;
    } else {
        *(char **)(*(char **)(arg0 + 4) + 4) = arg1;
    }
    *(char **)(arg0 + 4) = arg1;
    *(int *)(arg1 + 4) = 0;
    MUTEX_unlock_qa(D_00519C08_9C08);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DEED8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_EED8(void *) __asm__("MUTEX_unlock");
extern char D_00519C08_9C08[] __asm__("D_00519C08");

void *func_003DEED8(void **arg0) {
    void *temp_2;
    void *var_16;

    MUTEX_lock(D_00519C08_9C08);
    temp_2 = *arg0;
    var_16 = 0;
    if (temp_2 != 0) {
        var_16 = temp_2;
        *arg0 = *(void **)((char *)var_16 + 4);
    }
    MUTEX_unlock_EED8(D_00519C08_9C08);
    return var_16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DEF40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519C00_EF40[] __asm__("D_00519C00");

void func_003DEF40(unsigned char *arg0) {
    int temp_2;

    temp_2 = D_00519C00_EF40[0] + 0x100;
    D_00519C00_EF40[0] = temp_2;
    if (temp_2 == 0) {
        D_00519C00_EF40[0] = 0x100;
    }
    *(int *)arg0 = (int) (*arg0 | D_00519C00_EF40[0]);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DEF70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C34_0C34[] __asm__("D_00450C34");
extern int D_00519BF0_9BF0[] __asm__("D_00519BF0");

int *func_003DEF70(int arg0) {
    int *temp_2;
    int temp_6;

    temp_6 = arg0 & 0xFF;
    if ((arg0 < 0x100) || (temp_6 >= D_00519BF0_9BF0[0])) {
        return 0;
    }
    temp_2 = (int*)((char*)D_00450C34_0C34[0] + (temp_6 * 0x30));
    return ((*temp_2 ^ arg0) != 0) ? 0 : temp_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", releaserequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void queueadd_rr(char *, char *) __asm__("queueadd");
extern void (*D_00450C18_0C18[])(unsigned int) __asm__("D_00450C18");
extern char D_00519BF8_9BF8[] __asm__("D_00519BF8");

void releaserequest(char *arg0) {
    unsigned int temp_4;

    if (*(int *)(arg0 + 0x10) != 0) {
        temp_4 = *(unsigned int *)(arg0 + 0x14);
        if (temp_4 >= 2U) {
            D_00450C18_0C18[0](temp_4);
        }
    }
    *(int *)(arg0 + 0x1C) = 0;
    *(int *)arg0 = *(unsigned char *)arg0;
    queueadd_rr(D_00519BF8_9BF8, arg0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DF028);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF0B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF0F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF1E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF2C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF3C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF488);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF570);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF690);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF748);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF808);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF8E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF980);

INCLUDE_ASM("ealib/seg_2B4578", ASYNCFILE_release);

INCLUDE_ASM("ealib/seg_2B4578", func_003DFAF0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFBD0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003DFBD0_impl(int **arg0, int **arg1, int ***arg2) __asm__("func_003DFBD0");
int func_003DFBD0_impl(int **arg0, int **arg1, int ***arg2) {
    int *temp_7;

    if (arg0 == 0) {
        return 1;
    }
    temp_7 = *arg0;
    if (*temp_7 != 0x4D525453) {
        return 1;
    }
    *arg2 = arg0;
    *arg1 = temp_7;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFC08);
#ifdef SKIP_ASM
int func_003DFC08(unsigned int a, unsigned int b, unsigned int c) {
    int r;
    if (b >= a) {
        r = 0;
        if (c >= a) {
            if (c < b) r = 1;
        }
        return r;
    }
    r = 0;
    if (!(c < a) || c < b) {
        r = 1;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DFC48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void MUTEX_lock(void *);
void MUTEX_unlock_FC48(void *) __asm__("MUTEX_unlock");
void func_003DCFC0_FC48(int, int) __asm__("func_003DCFC0");

void func_003DFC48(char *arg0, int arg1) {
    char *m;
    int old, nw, lim;

    m = arg0 + 4;
    MUTEX_lock(m);
    old = *(int *)(arg0 + 0x4C);
    nw = old - arg1;
    *(int *)(arg0 + 0x4C) = nw;
    MUTEX_unlock_FC48(m);
    lim = *(int *)(arg0 + 0x44);
    if (old >= lim && nw < lim) {
        *(int *)(arg0 + 0x48) = 1;
        if (*(int *)(arg0 + 0x38) == 1) {
            func_003DCFC0_FC48(*(int *)(arg0 + 0x174), *(int *)(arg0 + 0x40));
        }
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DFCD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DFD58);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFDD0);
#ifdef SKIP_ASM
void *func_003DFDD0(void *arg0, int arg1) {
    int temp_3;
    void *temp_2;

    temp_3 = arg1 & 0xFF;
    if (temp_3 >= (*(int *)((char*)(arg0) + (0x18)))) {
        return 0;
    }
    temp_2 = (*(int *)((char*)(arg0) + (0x14))) + (temp_3 * 0x124);
    if (arg1 == (*(int *)((char*)(temp_2) + (0)))) {
        return ((*(int *)((char*)(temp_2) + (4))) == 0) ? 0 : temp_2;
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DFE18);

INCLUDE_ASM("ealib/seg_2B4578", func_003DFE88);

INCLUDE_ASM("ealib/seg_2B4578", func_003DFED0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E00D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003DD310_00D8(int) __asm__("func_003DD310");
void func_003E03D0_00D8(void *, int) __asm__("func_003E03D0");

void func_003E00D8(int arg0, int arg1, char *arg2) {
    int temp_2;

    temp_2 = func_003DD310_00D8(*(int *)(arg2 + 0x174));
    *(int *)(arg2 + 0x16C) = temp_2;
    if (temp_2 != 0) {
        func_003E03D0_00D8(arg2, *(int *)(arg2 + 0x40));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E0118);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003DCF70_0118(int, void *) __asm__("func_003DCF70");
void func_003DD310_0118(int) __asm__("func_003DD310");
int func_003DD4E8_0118(void *, int, int, void *) __asm__("func_003DD4E8");

void func_003E0118(int a0, int a1, char *arg2) {
    int temp_2;

    func_003DD310_0118(*(int *)(arg2 + 0x174));
    temp_2 = func_003DD4E8_0118(arg2 + 0x6C, 1, *(int *)(arg2 + 0x40), arg2);
    *(int *)(arg2 + 0x174) = temp_2;
    if (temp_2 != 0) {
        func_003DCF70_0118(temp_2, (void *)func_003E00D8);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E0170);

INCLUDE_ASM("ealib/seg_2B4578", func_003E0270);

INCLUDE_ASM("ealib/seg_2B4578", func_003E03D0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E06B0);
#ifdef SKIP_ASM
int func_003E06B0(int a, int b, int c) {
    int t = b * 0xC + 0x180;
    return a * 0x124 + t + c * 0x10 + 0x40;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E06D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E0948);

INCLUDE_ASM("ealib/seg_2B4578", func_003E0A28);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0AD8);
#ifdef SKIP_ASM
void func_003E0AD8(int a0, int a1, int a2) {
    int *sp0;
    int *sp4;

    if (func_003DFBD0(a0, &sp0, &sp4) == 0) {
        sp0[0x3C / 4] = a1;
        sp0[0x40 / 4] = a2;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E0B28);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E0B98);
#ifdef SKIP_ASM
typedef struct { char pad[0x38]; int s38; int pad3C; int f40; char pad44[4]; int f48; char pad4C[0x128]; int f174; } O_0B98;
extern void func_003DCFC0(int, int);

void func_003E0B98(int arg0, int arg1) {
    O_0B98 *o;
    int x;

    if (func_003DFBD0(arg0, &o, &x) == 0) {
        o->f48 = arg1;
        if (arg1 != 0) {
            if (o->s38 == 1) {
                func_003DCFC0(o->f174, o->f40);
            }
        }
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E0BF8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E0C58);
#ifdef SKIP_ASM
extern int func_003DFBD0(int, void *, void *);

int func_003E0C58(int arg0) {
    int loc[2];

    if (func_003DFBD0(arg0, &loc[0], &loc[1]) != 0) {
        return 0;
    }
    return *(int *)((char*)loc[1] + 4);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E0C88);

INCLUDE_ASM("ealib/seg_2B4578", func_003E0D80);

INCLUDE_ASM("ealib/seg_2B4578", func_003E0E90);

INCLUDE_ASM("ealib/seg_2B4578", func_003E1110);

INCLUDE_ASM("ealib/seg_2B4578", func_003E12E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E13E8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E14C8);
#ifdef SKIP_ASM
extern int func_003DFBD0(int, void *, void *);

int func_003E14C8(int arg0) {
    int loc[2];

    if (func_003DFBD0(arg0, &loc[0], &loc[1]) != 0) {
        return 0;
    }
    return *(int *)((char*)loc[1] + 8);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E14F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E1530);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1580);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003DFBD0_1580(void *, void *, void *) __asm__("func_003DFBD0");

int func_003E1580(void *arg0) {
    char *sp0;
    int sp4;

    if (func_003DFBD0_1580(arg0, &sp0, &sp4) != 0) {
        return 0;
    }
    return *(int *)(sp0 + 0x34) - *(int *)(sp0 + 0x2C);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E15B8);
#ifdef SKIP_ASM
extern int func_003DFBD0(int, void *, void *);

int func_003E15B8(int arg0) {
    int loc[2];

    if (func_003DFBD0(arg0, &loc[0], &loc[1]) != 0) {
        return 0;
    }
    return *(int *)((char*)loc[0] + 0x4C);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E15E8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E16B0);
#ifdef SKIP_ASM
int func_003E16B0(int a0, int a1) {
    int *sp0;
    int *sp4;
    int *t;

    if (func_003DFBD0(a0, &sp0, &sp4) != 0) {
        return 0;
    }
    t = func_003DFDD0(sp0, a1);
    if (t == 0) {
        return 0;
    }
    return t[1];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1700);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C38_1700[] __asm__("D_00450C38");
void func_003DEE18_1700(int, int) __asm__("func_003DEE18");
void func_003E1700(int a) {
    func_003DEE18_1700(a, D_00450C38_1700[0]);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1728);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1798);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1798(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1728_1798(void) __asm__("func_003E1728");
extern int D_00450C38_1798[] __asm__("D_00450C38");

void func_003E1798(int arg0) {
    int buf[8];

    buf[0] = arg0;
    buf[3] = 1;
    FILESYS_atomic_1798(func_003E1728_1798, 0, D_00450C38_1798[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E17D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_17D8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1728_17D8(void) __asm__("func_003E1728");
extern int D_00450C38_17D8[] __asm__("D_00450C38");

void func_003E17D8(int arg0) {
    int buf[8];

    buf[0] = arg0;
    buf[3] = 0;
    FILESYS_atomic_17D8(func_003E1728_17D8, 0, D_00450C38_17D8[0], buf);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1810);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", FILE_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1900(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1810_18C8(void) __asm__("func_003E1810");
extern int D_00450C38_18C8[] __asm__("D_00450C38");

void FILE_load(int arg0, int arg1) {
    int buf[8];

    buf[0] = arg0;
    buf[3] = 1;
    buf[4] = arg1;
    FILESYS_atomic_1900(func_003E1810_18C8, 0, D_00450C38_18C8[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1908);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1908(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1810_1908(void) __asm__("func_003E1810");
extern int D_00450C38_1908[] __asm__("D_00450C38");

void func_003E1908(int arg0, int arg1) {
    int buf[8];

    buf[0] = arg0;
    buf[3] = 0;
    buf[4] = arg1;
    FILESYS_atomic_1908(func_003E1810_1908, 0, D_00450C38_1908[0], buf);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1948);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1A10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a; int b; int sz; int c; int d; } FLS_args_1A10;
extern int FILESYS_atomic_1A10(void (*)(void), int, int, void*) __asm__("FILESYS_atomic");
extern int D_00450C38_0C38[] __asm__("D_00450C38");
extern void func_003E1948_1948(void) __asm__("func_003E1948");

int func_003E1A10(int arg0, int *arg1, int arg2) {
    FLS_args_1A10 s;
    int r;

    s.a = arg0;
    s.c = 1;
    s.d = arg2;
    r = FILESYS_atomic_1A10(func_003E1948_1948, 0, D_00450C38_0C38[0], &s);
    if (arg1 != 0) {
        *arg1 = s.sz;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", FILE_loadsizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a; int b; int sz; int c; int d; } FLS_args_1ACC;
extern int FILESYS_atomic_1ACC(void (*)(void), int, int, void*) __asm__("FILESYS_atomic");
extern int D_00450C38_0C38[] __asm__("D_00450C38");
extern void func_003E1948_1948(void) __asm__("func_003E1948");

int FILE_loadsizez(int arg0, int *arg1, int arg2) {
    FLS_args_1ACC s;
    int r;

    s.a = arg0;
    s.c = 0;
    s.d = arg2;
    r = FILESYS_atomic_1ACC(func_003E1948_1948, 0, D_00450C38_0C38[0], &s);
    if (arg1 != 0) {
        *arg1 = s.sz;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1AD0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1B68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C38_0C38[] __asm__("D_00450C38");
extern void FILESYS_atomic(int *, int, int, void *);
extern int func_003E1AD0[];

void func_003E1B68(int a0, int a1, int a2) {
    int s[5];
    s[0] = a0;
    s[1] = a1;
    s[2] = a2;
    s[3] = 1;
    s[4] = 0;
    FILESYS_atomic(func_003E1AD0, 0, D_00450C38_0C38[0], s);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1BB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a; int b; int c; int d; int e; int pad[3]; } S_1BB8;
extern int D_00450C38_1BB8[] __asm__("D_00450C38");
extern void f_1BB8_cb(void) __asm__("func_003E1AD0");
extern int FILESYS_atomic_1BB8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");

void func_003E1BB8(int a, int b, int c) {
    S_1BB8 s;
    s.a = a;
    s.b = b;
    s.c = c;
    s.d = 0;
    s.e = 0;
    FILESYS_atomic_1BB8(f_1BB8_cb, 0, D_00450C38_1BB8[0], &s);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1C00);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1C98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cb_1C98(void) __asm__("func_003E1C00");
typedef struct Args_1C98 { int a0, a1, a2, a3, a4; } Args_1C98;
extern int FILESYS_atomic_1C98(void (*)(void), int, int, Args_1C98 *) __asm__("FILESYS_atomic");
extern int D_00450C38_0C38[] __asm__("D_00450C38");

int func_003E1C98(int arg0, int arg1, int arg2, int arg3) {
    Args_1C98 args;

    if (arg0 <= 0) return 0;
    {
        args.a0 = arg0;
        args.a1 = arg1;
        args.a2 = arg2;
        args.a3 = arg3;
        args.a4 = 1;
        return FILESYS_atomic_1C98(cb_1C98, 0, D_00450C38_0C38[0], &args) == 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1D00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cb_1D00(void) __asm__("func_003E1C00");
typedef struct Args_1D00 { int a0, a1, a2, a3, a4; } Args_1D00;
extern int FILESYS_atomic_1D00(void (*)(void), int, int, Args_1D00 *) __asm__("FILESYS_atomic");
extern int D_00450C38_0C38[] __asm__("D_00450C38");

int func_003E1D00(int arg0, int arg1, int arg2, int arg3) {
    Args_1D00 args;

    if (arg0 <= 0) return 0;
    {
        args.a0 = arg0;
        args.a1 = arg1;
        args.a2 = arg2;
        args.a3 = arg3;
        args.a4 = 0;
        return FILESYS_atomic_1D00(cb_1D00, 0, D_00450C38_0C38[0], &args) == 1;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1D68);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1EC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1EC8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1D68_1EC8(void) __asm__("func_003E1D68");
extern int D_00450C38_1EC8[] __asm__("D_00450C38");

void func_003E1EC8(int arg0, int arg1) {
    int buf[8];

    buf[0] = arg0;
    buf[2] = arg1;
    buf[3] = 1;
    FILESYS_atomic_1EC8(func_003E1D68_1EC8, 0, D_00450C38_1EC8[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1F08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1F08(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void func_003E1D68_1F08(void) __asm__("func_003E1D68");
extern int D_00450C38_1F08[] __asm__("D_00450C38");

void func_003E1F08(int arg0, int arg1) {
    int buf[8];

    buf[0] = arg0;
    buf[3] = 0;
    buf[2] = arg1;
    FILESYS_atomic_1F08(func_003E1D68_1F08, 0, D_00450C38_1F08[0], buf);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E1F48);

INCLUDE_ASM("ealib/seg_2B4578", func_003E2030);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E2130);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003E1BB8_2130(int, void *, int) __asm__("func_003E1BB8");
int func_003B4818_2130(void *) __asm__("func_003B4818");

int func_003E2130(int arg0) {
    char buf[0x80];
    int var_2;

    if (func_003E1BB8_2130(arg0, buf, 0x80) != 0) {
        var_2 = func_003B4818_2130(buf);
    } else {
        var_2 = 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E2168);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003E1BB8_2168(int, char *, int) __asm__("func_003E1BB8");
void func_003B4818_2168(char *) __asm__("func_003B4818");
void func_003E2168(int a) {
    char buf[0x80];
    func_003E1BB8_2168(a, buf, 0x80);
    func_003B4818_2168(buf);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E2190);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E22F0);
#ifdef SKIP_ASM
extern void func_003E2190();

void func_003E22F0(void) {
    func_003E2190();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", FILE_loadpackatz);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", FILE_loadpackat);
#ifdef SKIP_ASM
extern void FILE_loadpackatz();

void FILE_loadpackat(void) {
    FILE_loadpackatz();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E2490);

INCLUDE_ASM("ealib/seg_2B4578", BIG_typeofheader);

INCLUDE_ASM("ealib/seg_2B4578", BIG_sizeofheader);

INCLUDE_ASM("ealib/seg_2B4578", BIG_debuginfo);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E2740);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void BIG_debuginfo_2740(int, int *, int) __asm__("BIG_debuginfo");
int func_003E2740(int a) {
    int x = 0;
    BIG_debuginfo_2740(a, &x, 0);
    return x;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", BIG_locateentryz);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2CE0);
#ifdef SKIP_ASM
extern void BIG_locateentryz();

void func_003E2CE0(void) {
    BIG_locateentryz();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2D00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E2CE0_2D00(int, int, int, void *, int) __asm__("func_003E2CE0");

int func_003E2D00(arg0, arg1)
int arg0;
int arg1;
{
    int sp0;

    sp0 = 0;
    if (arg1 != 0) {
        func_003E2CE0_2D00(arg0, arg1, 0, &sp0, 0);
    }
    return sp0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2D30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void BIG_locateentryz_2D30(int, int, int, void *, int) __asm__("BIG_locateentryz");

int func_003E2D30(arg0, arg1)
int arg0;
int arg1;
{
    int sp0;

    sp0 = 0;
    if (arg1 != 0) {
        BIG_locateentryz_2D30(arg0, arg1, 0, &sp0, 0);
    }
    return sp0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2D60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_2D60_cb(int, int, int, int *, int) __asm__("func_003E2CE0");

int func_003E2D60(int base, int x) {
    int out;
    if (f_2D60_cb(base, 0, x, &out, 0) != 0)
        return base + out;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2DA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_2DA8_cb(int, int, int, int *, int) __asm__("BIG_locateentryz");

int func_003E2DA8(int base, int x) {
    int out;
    if (f_2DA8_cb(base, 0, x, &out, 0) != 0)
        return base + out;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E2DF0);
#ifdef SKIP_ASM
extern int func_003E2D00();

int func_003E2DF0(int arg0) {
    int temp_2;

    temp_2 = func_003E2D00();
    return (temp_2 == 0) ? 0 : (arg0 + temp_2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E2E20);
#ifdef SKIP_ASM
extern int func_003E2D30();

int func_003E2E20(int arg0) {
    int temp_2;

    temp_2 = func_003E2D30();
    return (temp_2 == 0) ? 0 : (arg0 + temp_2);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E2E50);

INCLUDE_ASM("ealib/seg_2B4578", func_003E2EE0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2F70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *strcpy(char *, const char *);
char *func_003E2CE0_2F70(int, int, int, int, int) __asm__("func_003E2CE0");

char *func_003E2F70(int a, int b, char *dst) {
    char *p = func_003E2CE0_2F70(a, 0, b, 0, 0);
    char *r;
    if (!p) {
        *dst = 0;
        r = 0;
    } else {
        strcpy(dst, p);
        r = dst;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2FC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *strcpy(char *, const char *);
char *BIG_locateentryz_2FC8(int, int, int, int, int) __asm__("BIG_locateentryz");

char *func_003E2FC8(int a, int b, char *dst) {
    char *p = BIG_locateentryz_2FC8(a, 0, b, 0, 0);
    char *r;
    if (!p) {
        *dst = 0;
        r = 0;
    } else {
        strcpy(dst, p);
        r = dst;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E3020);

INCLUDE_ASM("ealib/seg_2B4578", func_003E3098);

INCLUDE_ASM("ealib/seg_2B4578", func_003E3208);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E32F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00427FE8(int);
extern int D_00450C40_0C40[] __asm__("D_00450C40");

int func_003E32F8(int arg0) {
    int temp_16;
    int temp_5;

    temp_5 = arg0 >> 0x18;
    temp_16 = arg0 & 0xFFFFFF;
    if (temp_5 == 2) {
        func_00427FE8(temp_16);
        if (D_00450C40_0C40[0] == temp_16) {
            D_00450C40_0C40[0] = -1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E3350);

INCLUDE_ASM("ealib/seg_2B4578", func_003E33B0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E3478);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E34E8);
#ifdef SKIP_ASM
extern void func_003E48D8(int, int, int, int);
extern void func_00428168(int, int, int);

void func_003E34E8(int arg0) {
    int temp_4;

    temp_4 = arg0 & 0xFFFFFF;
    if ((arg0 >> 0x18) == 1) {
        func_003E48D8(temp_4, 0, 0, 0);
        return;
    }
    func_00428168(temp_4, 0, 2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3538);
#ifdef SKIP_ASM
int func_003E3538(int a0) {
    int sp0;
    int idx = a0 & 0xFFFFFF;

    if ((a0 >> 24) == 1) {
        func_003E48D8(idx, 0, 0, &sp0);
    } else {
        sp0 = 0;
    }
    return sp0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3588);
#ifdef SKIP_ASM
void func_00423DD0_3588(int) __asm__("func_00423DD0");
void func_003E3588(int a, int b, int c) {
    func_00423DD0_3588(c);
    // PORT: MIPS sync + EE ei (no C equivalent)
    __asm__ volatile("sync\n\tei");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E35B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct Cfg_35B0 { int f0, f4, f8, fc, f10, f14; } Cfg_35B0;
extern void cb_35B0(void) __asm__("func_003E3588");
extern void func_00423B20_35B0(int, void (*)(void), int) __asm__("func_00423B20");
extern int func_00423DA0_35B0(Cfg_35B0 *) __asm__("func_00423DA0");

void func_003E35B0(unsigned short arg0) {
    Cfg_35B0 c;
    int temp_2;

    c.f4 = 1;
    c.f8 = 0;
    c.f14 = 0;
    temp_2 = func_00423DA0_35B0(&c);
    func_00423B20_35B0(arg0, cb_35B0, temp_2);
    func_00423DE0(temp_2);
    func_00423DB0(temp_2);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E3618);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E36F8);
#ifdef SKIP_ASM
extern int strlen();

int func_003E36F8(unsigned int arg0, int arg1) {
    int h = arg1;
    signed char *p = (signed char *)(arg0 + strlen(arg0)) - 1;

    do {
        signed char c = *p;
        p -= 1;
        h = h * 0x21 + c;
    } while ((unsigned int)p >= arg0);
    return h;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E3758);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3968);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0051ED80_3968[] __asm__("D_0051ED80");

int func_003E3968(unsigned int *a, unsigned int *b) {
    long d = (long)*a - (long)*b;
    if (d < 0) return -1;
    if (d > 0) return 1;
    D_0051ED80_3968[0]++;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E39A8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3AD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519C4C_3AD8[] __asm__("D_00519C4C");
void func_00423DD0_3AD8(int) __asm__("func_00423DD0");
void func_003E3AD8(void) {
    func_00423DD0_3AD8(D_00519C4C_3AD8[0]);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E3B00);

INCLUDE_ASM("ealib/seg_2B4578", func_003E3BE0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E3D78);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4000);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519C40_4000[] __asm__("D_00519C40");
void func_003E3D78_4000(void) __asm__("func_003E3D78");

int func_003E4000(void) {
    int t = D_00519C40_4000[0];
    if ((t & 6) == 6) {
        D_00519C40_4000[0] = t & ~6;
        func_003E3D78_4000();
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E4040);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E44B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SYNCTASK_del_44B0(void (*)(void)) __asm__("SYNCTASK_del");
extern void func_004008A0(int);
extern void func_00401718(int);
extern void func_00423BB0(int);
extern void func_00423BF0(int);
extern void func_00423DB0(int);
extern int D_00519C40_9C40[] __asm__("D_00519C40");
extern void func_003E4000_4000(void) __asm__("func_003E4000");

void func_003E44B0(void) {
    SYNCTASK_del_44B0(func_003E4000_4000);
    func_004008A0(0);
    func_00423BF0(D_00519C40_9C40[2]);
    func_00423BB0(D_00519C40_9C40[2]);
    func_00423DB0(D_00519C40_9C40[3]);
    func_00401718(5);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E4508);

INCLUDE_ASM("ealib/seg_2B4578", func_003E4648);

INCLUDE_ASM("ealib/seg_2B4578", func_003E48D8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E4968);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003E4508(int, int, int *);
extern int D_00450C5C_0C5C[] __asm__("D_00450C5C");
extern int D_00519C78_9C78[] __asm__("D_00519C78");

int func_003E4968(int a0) {
    int sp0;
    int r;

    if (func_003E4508(a0, 0, &sp0) == 0) {
        r = 0;
    } else {
        r = D_00519C78_9C78[0] + (sp0 - 1) * (D_00450C5C_0C5C[0] + 0xC);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E49B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*D_00450C78_0C78[])(void) __asm__("D_00450C78");

void func_003E49B8(void) {
    int i;
    for (i = 0x3F; i >= 0; i--) {
        void (*fn)(void) = D_00450C78_0C78[i];
        if (fn != 0) {
            fn();
        }
        D_00450C78_0C78[i] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4A10);
#ifdef SKIP_ASM
extern void func_003E49B8(void);

void func_003E4A10(void) {
    func_003E49B8();
    __asm__ volatile("break 0xFFFF");  // PORT: trap
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E4A30);

INCLUDE_ASM("ealib/seg_2B4578", func_003E4AA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E4AF0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E4D68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00423AA0(int, int);
extern void func_00424880(int);
extern int D_00450DDC_0DDC[] __asm__("D_00450DDC");

void func_003E4D68(void) {
    volatile int *reg;

    if (D_00450DDC_0DDC[0] != 0) {
        reg = (volatile int *)0x10000810;
        *reg = 0;
        func_00424880(0xA);
        func_00423AA0(0xA, D_00450DDC_0DDC[0]);
        D_00450DDC_0DDC[0] = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E4DB8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E4E98);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4EA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450DEC_4EA8[] __asm__("D_00450DEC");

int func_003E4EA8(void) {
    int old = D_00450DEC_4EA8[0];
    int now = func_003E4E98();
    D_00450DEC_4EA8[0] = now;
    return now - old;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E4EE8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4F08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003E4E98_4F08(void) __asm__("func_003E4E98");
extern int D_00450DE8_4F08[] __asm__("D_00450DE8");

void func_003E4F08(int arg0) {
    int *p = D_00450DE8_4F08;
    *p = func_003E4E98_4F08() + arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E4F40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003E4F80();
extern void SYNCTASK_run_4F40(int) __asm__("SYNCTASK_run");
extern void func_003E5398_4F40(int) __asm__("func_003E5398");

void func_003E4F40(void) {
    while (func_003E4F80() == 0) {
        SYNCTASK_run_4F40(0);
        func_003E5398_4F40(0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4F80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003E4E98();
extern int D_00450DE8_4F80[] __asm__("D_00450DE8");

int func_003E4F80(void) {
    int r = func_003E4E98() - D_00450DE8_4F80[0]; if (r > -1) return 1; return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E4FB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SYNCTASK_run_4FB0(int) __asm__("SYNCTASK_run");
void func_003E5398_4FB0(int) __asm__("func_003E5398");

void func_003E4FB0(int arg0) {
    int end = func_003E4E98() + arg0;
    while (func_003E4E98() - end < 0) {
        SYNCTASK_run_4FB0(0);
        func_003E5398_4FB0(0);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5008);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450DC8_5008[4] __asm__("D_00450DC8");

int func_003E5008(void) {
    return D_00450DC8_5008[0];
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5018);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5068);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E56E0(void *);
extern void func_003E6220_5068(int *, int) __asm__("func_003E6220");
extern int D_00450DF8_5068[] __asm__("D_00450DF8");
extern int D_0051ED88_5068[] __asm__("D_0051ED88");

void func_003E5068(void) {
    func_003E56E0(D_0051ED88_5068);
    func_003E6220_5068(D_00450DF8_5068, 8);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5098);
#ifdef SKIP_ASM
extern void func_003E50C8(int, int, int, int, int, int);

void func_003E5098(int arg0, int arg1, int arg2, int arg3, int arg4) {
    func_003E50C8(arg0, arg1, 0, arg2, arg3, arg4);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E50C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E51A0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5268);
#ifdef SKIP_ASM
extern void func_003E51A0();

void func_003E5268(void) {
    func_003E51A0();
}
#endif

void func_003E5288(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5290);
#ifdef SKIP_ASM
extern void func_00424C50(int);

void func_003E5290(int a, int b, int c) {
    func_00424C50(c);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", THREAD_yieldticks);

INCLUDE_ASM("ealib/seg_2B4578", func_003E5398);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5440);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450DFC_0DFC[] __asm__("D_00450DFC");

int func_003E5440(char *arg0) {
    int cur = func_00423C90();
    if (arg0 == 0) {
        return cur == D_00450DFC_0DFC[0];
    }
    if (arg0 == (char *)-1) {
        return 1;
    }
    return cur == *(int *)(arg0 + 4);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5498);

INCLUDE_ASM("ealib/seg_2B4578", func_003E5508);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5580);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00423CA0(int, void *);
extern void func_00423CB0(int, void *);
extern int D_00450DE4_0DE4[] __asm__("D_00450DE4");
typedef struct { int pad; int f4; } A_5580;

int func_003E5580(A_5580 *arg0) {
    struct { int v; char rest[0x2C]; } st;
    int r;

    if (D_00450DE4_0DE4[0] != 0) {
        func_00423CB0(arg0->f4, &st);
    } else {
        func_00423CA0(arg0->f4, &st);
    }
    r = 0;
    if ((st.v & 0x10) || st.v == 0) {
        r = 1;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E55E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5678);
#ifdef SKIP_ASM
extern void func_00423BE0();

void func_003E5678(void) {
    func_00423BE0();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5698);
#ifdef SKIP_ASM
typedef struct { int a; int b; int c; int d[5]; } S_5698;
extern int func_00423DA0(S_5698 *);

int func_003E5698(int *self) {
    S_5698 s;
    self[2] = 0;
    self[1] = 0;
    s.b = 1;
    s.c = 1;
    self[3] = func_00423DA0(&s);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E56E0);
#ifdef SKIP_ASM
extern void func_00423DB0(int);

void func_003E56E0(void *arg0) {
    func_00423DB0((*(int *)((char*)(arg0) + (0xC))));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MUTEX_lock);
#ifdef SKIP_ASM
extern int func_00423C90();
extern void func_00423DE0(int);

void MUTEX_lock(void *arg0) {
    if ((*(int *)((char*)(arg0) + (4))) == func_00423C90()) {
        (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) + 1);
        return;
    }
    func_00423DE0((*(int *)((char*)(arg0) + (0xC))));
    (*(int *)((char*)(arg0) + (4))) = func_00423C90();
    (*(int *)((char*)(arg0) + (8))) = (int) ((*(int *)((char*)(arg0) + (8))) + 1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", MUTEX_unlock);

//100%
INCLUDE_ASM("ealib/seg_2B4578", SYNCTASK_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0051ED98_57D0[] __asm__("D_0051ED98");
void func_003E6448_57D0(char *, int, int) __asm__("func_003E6448");
void SYNCTASK_init(void) {
    func_003E6448_57D0(D_0051ED98_57D0, 0, 0x100);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", SYNCTASK_add);

//100%
INCLUDE_ASM("ealib/seg_2B4578", SYNCTASK_del);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int fn; int b; int c; int d; } STsk_5924;
extern STsk_5924 D_0051ED98_ED98[] __asm__("D_0051ED98");

void SYNCTASK_del(int arg0) {
    int i = 0;

    while (i < 16 && D_0051ED98_ED98[i].fn != arg0) i++;
    if (i < 16 && D_0051ED98_ED98[i].fn == arg0) D_0051ED98_ED98[i].fn = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", SYNCTASK_run);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5A10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SYNCTASK_run_5A10(int) __asm__("SYNCTASK_run");

int func_003E5A10(int arg0) {
    int temp_16;
    int var_17;

    var_17 = 0;
    temp_16 = func_003E4E98() + arg0;
    while (func_003E4E98() < temp_16 && var_17 == 0) {
        var_17 = SYNCTASK_run_5A10(0);
    }
    return var_17;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5A78);
#ifdef SKIP_ASM
extern char D_00451250[];

void func_003E5A78(int arg0, int arg1) {
    char *temp_4;

    temp_4 = D_00451250 + arg0 * 0xC;
    (*(int *)((char*)(temp_4) + (8))) = (int) (((*(int *)((char*)(temp_4) + (8))) & ~1) | (arg1 & 1));
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5AA8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E5B60);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00451008_5C60[4] __asm__("D_00451008");

void func_003E5C60(void) {
    if (D_00451008_5C60[0] != 0) {
        D_00451008_5C60[0] = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5C78);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5D30);
#ifdef SKIP_ASM
extern void func_003E5C78(char *, const char *, char *);

// PORT: SN-specific va_start
void func_003E5D30(char *dst, const char *fmt, ...) {
    char *ap;
    ap = (char*)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_003E5C78(dst, fmt, ap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5D78);
#ifdef SKIP_ASM
extern void func_003E5C78_5D78(int, int, char *) __asm__("func_003E5C78");

void func_003E5D78(int a0, ...) {
    char *ap;
    // PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
    ap = (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_003E5C78_5D78(2, a0, ap);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5DC8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5E38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E5B60(void);
extern char D_00451250_1250[] __asm__("D_00451250");
extern int D_00451008_1008[] __asm__("D_00451008");

int func_003E5E38(int a0) {
    char *b;
    if (D_00451008_1008[0] == 0) {
        func_003E5B60();
    }
    b = D_00451250_1250 + a0 * 12;
    return *(int *)(b + 8) & 1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5E88);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5EF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E5AA8(int, int);
extern void func_003E5B60(void);
extern int D_00451008_1008[] __asm__("D_00451008");

void func_003E5EF8(int a0, int a1) {
    if (D_00451008_1008[0] == 0) {
        func_003E5B60();
    }
    func_003E5AA8(a0, a1);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5F48);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5F98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450E08_0E08[] __asm__("D_00450E08");
extern int D_00451008_1008[] __asm__("D_00451008");
void func_003E5B60_5F98(void) __asm__("func_003E5B60");

void func_003E5F98(int arg0, int arg1) {
    if (D_00451008_1008[0] == 0) {
        func_003E5B60_5F98();
    }
    D_00450E08_0E08[arg0 * 2] = arg1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E5FF0);

INCLUDE_ASM("ealib/seg_2B4578", REAL_abortmessage);

INCLUDE_ASM("ealib/seg_2B4578", SYSTEM_abortmessage);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6188);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E61F8);
#ifdef SKIP_ASM
extern void func_003E6188(int, int);

void func_003E61F8(int arg0, int arg1) {
    func_003E6188(arg1, arg0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6220);
#ifdef SKIP_ASM
extern void func_003E6448(int, int, int);

void func_003E6220(int a, int b) {
    func_003E6448(a, 0, b);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E6240);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E62B0);
#ifdef SKIP_ASM
extern int D_00450DA4[];

void func_003E62B0(void) {
    if (D_00450DA4[0] != 0) {
        __asm__ volatile("break 6");  // PORT: trap
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E62D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short D_00451018_1018[] __asm__("D_00451018");

int func_003E62D0(unsigned char *arg0, int arg1, int arg2) {
    unsigned char *p;
    unsigned int crc;
    unsigned char *end;
    p = arg0;
    crc = arg2 & 0xFFFF;
    end = p + arg1;
    if (p < end) {
        do {
            unsigned char c = *p;
            p += 1;
            crc = D_00451018_1018[(c ^ crc) & 0xFF] ^ (crc >> 8);
        } while (p < end);
    }
    return crc & 0xFFFF;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E6328);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6428);
#ifdef SKIP_ASM
extern void func_004175C8(int);

void func_003E6428(int a, int b) {
    func_004175C8(b);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E6448);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6574);

INCLUDE_ASM("ealib/seg_2B4578", func_003E665C);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6690);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450DA0_0DA0[] __asm__("D_00450DA0");

void func_003E6690(int arg0) {
    switch (arg0) {
    case 1:
        D_00450DA0_0DA0[0] = 0x3D86;
        return;
    case 2:
        D_00450DA0_0DA0[0] = 0x3D09;
        return;
    case 3:
        D_00450DA0_0DA0[0] = 0x7080;
        return;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E66F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E67F8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6878);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6958);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6A70);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6B10);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6C80);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6D78);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6E08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_6E08(int *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
extern void func_003F4C98_6E08(int *, int, int) __asm__("func_003F4C98");

int func_003E6E08(void) {
    int sp10[4];
    int sp20;

    func_003F4C98_6E08(sp10, 0x676D7574, 0x63726561);
    func_003F4C68_6E08(sp10, 1, 0, 0, 0, 0, 0, 0, 1, &sp20);
    return sp20;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6E70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_6E70(int *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4C98_6E70(int *, int, int) __asm__("func_003F4C98");

void func_003E6E70(int arg0) {
    int sp10[4];

    func_003F4C98_6E70(sp10, 0x676D7574, 0x64657374);
    func_003F4C68_6E70(sp10, 1, arg0, 0, 0, 0, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6ED8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_6ED8(int *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4C98_6ED8(int *, int, int) __asm__("func_003F4C98");

void func_003E6ED8(int arg0) {
    int sp10[4];

    func_003F4C98_6ED8(sp10, 0x676D7574, 0x72736574);
    func_003F4C68_6ED8(sp10, 1, arg0, 0, 0, 0, 0, 0, 1, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E6F40);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6FC8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7038);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E70B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_70B8(int *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4C98_70B8(int *, int, int) __asm__("func_003F4C98");

void func_003E70B8(int arg0) {
    int sp10[4];

    func_003F4C98_70B8(sp10, 0x676D7574, 0x73746174);
    func_003F4C68_70B8(sp10, 1, arg0, 0, 0, 0, 0, 0, 1, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E7120);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E71B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_71B0(int *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4C98_71B0(int *, int, int) __asm__("func_003F4C98");

void func_003E71B0(int arg0) {
    int sp10[4];

    func_003F4C98_71B0(sp10, 0x676D7574, 0x77697468);
    func_003F4C68_71B0(sp10, 1, arg0, 0, 0, 0, 0, 0, 1, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E7218);

INCLUDE_ASM("ealib/seg_2B4578", func_003E72A0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7340);

INCLUDE_ASM("ealib/seg_2B4578", func_003E73E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7450);

INCLUDE_ASM("ealib/seg_2B4578", func_003E74E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7500);

INCLUDE_ASM("ealib/seg_2B4578", func_003E76A8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7718);

INCLUDE_ASM("ealib/seg_2B4578", func_003E77F0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7918);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E7AC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_003E77F0_7AC0(int, void *, int) __asm__("func_003E77F0");

void func_003E7AC0(int arg0) {
    char buf[0x50];
    while (func_003E77F0_7AC0(arg0, buf, 0x44515545) != 0) {
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E7B00);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7BA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7C40);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7F58);
#ifdef SKIP_ASM
void func_003E7F58(void *arg0) {
    (*(unsigned *)((char*)(arg0) + (0x20))) = 0;
    (*(unsigned *)((char*)(arg0) + (0x18))) = 0;
    (*(unsigned *)((char*)(arg0) + (0x1C))) = 0xFFFFFFFF;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E7F70);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7FF0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E80C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E81C8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8248);
#ifdef SKIP_ASM
typedef struct { int a; int b; } E8248_pair;
typedef struct { char pad[0x514]; E8248_pair e[1]; } E8248_S;

void func_003E8248(E8248_S *s, int i, int a, int b) {
    s->e[i].a = a;
    s->e[i].b = b;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8260);

INCLUDE_ASM("ealib/seg_2B4578", func_003E8368);

INCLUDE_ASM("ealib/seg_2B4578", func_003E85B0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8650);
#ifdef SKIP_ASM
typedef struct { int a, b, c, d, e; } Q_8650;
typedef struct { char pad[0x314]; Q_8650 q[8]; } S_8650;

int func_003E8650(S_8650 *s, unsigned int i, int arg2, int arg3) {
    int r = 0;
    if (i < 8U) {
        s->q[i].b = arg2;
        s->q[i].c = arg3;
        r = s->q[i].a;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8688);

INCLUDE_ASM("ealib/seg_2B4578", func_003E8778);

INCLUDE_ASM("ealib/seg_2B4578", func_003E8968);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8A28);
#ifdef SKIP_ASM
extern int func_003EEC88(int, int);

int func_003E8A28(int arg0, unsigned int arg1, int arg2) {
    int temp_4;

    if (arg1 < 8U) {
        temp_4 = (*(int *)((char*)(((arg1 * 0x14) + arg0)) + (0x310)));
        if (temp_4 != 0) {
            return func_003EEC88(temp_4, arg2) != 0;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8A70);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8AF8);
#ifdef SKIP_ASM
void func_003E8AF8(void *arg0, int arg1) {
    if (arg1 == 0x706C6179) {
        (*(int *)((char*)(arg0) + (0x2CC))) = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8B10);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8D70);
#ifdef SKIP_ASM
int func_003E8D70(void *arg0, int arg1) {
    int var_6;
    void **var_4;
    void *temp_2;
    void *temp_3;

    var_6 = 0;
    var_4 = arg0 + 0x53C;
    if ((*(int *)((char*)(arg0) + (0x53C))) != 0) {
        do {
            temp_3 = *var_4;
            if (((*(int *)((char*)(temp_3) + (0))) == arg1) || (arg1 == -1)) {
                (*(int *)((char*)(temp_3) + (0x38))) = 0;
                var_6 = 1;
            }
            temp_2 = *var_4;
            var_4 = temp_2 + 0x40;
        } while ((*(int *)((char*)(temp_2) + (0x40))) != 0);
    }
    return var_6;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8DC0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E8F08);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8FB8);
#ifdef SKIP_ASM
int func_003E8FB8(void *arg0) {
    int temp_2;
    int temp_3;

    temp_2 = (*(int *)((char*)(arg0) + (8)));
    temp_3 = temp_2 & (temp_2 - 1);
    (*(int *)((char*)(arg0) + (8))) = temp_3;
    return temp_2 ^ temp_3;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E8FD0);

INCLUDE_ASM("ealib/seg_2B4578", func_003E9160);

INCLUDE_ASM("ealib/seg_2B4578", func_003E9378);

INCLUDE_ASM("ealib/seg_2B4578", func_003E94F0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E9590);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E8DC0_9590(void *, void *, int, int, void *, int) __asm__("func_003E8DC0");
extern char D_004A4900_9590[] __asm__("D_004A4900");
extern char func_003E94F0_9590[] __asm__("func_003E94F0");

void func_003E9590(void *arg0) {
    func_003E8DC0_9590(arg0, D_004A4900_9590, *(int *)((char *)arg0 + 0x3B4), 0xBB8, func_003E94F0_9590, 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E95C8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003EAE68);
#ifdef SKIP_ASM
unsigned char *func_003EAE68(unsigned char *a0, int *a1) {
    unsigned int c;

    *a1 = 0;
    c = *a0;
    if (c - 0x30 < 10) {
        do {
            a0++;
            *a1 = *a1 * 10 + (c & 0xF);
            c = *a0;
        } while (c - 0x30 < 10);
    }
    return a0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003EAEB8);

INCLUDE_ASM("ealib/seg_2B4578", func_003EB090);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003EB190);
#ifdef SKIP_ASM
int func_003EB190(void) {
    return 0;
}
#endif
