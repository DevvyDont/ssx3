#include "common.h"

INCLUDE_ASM("ealib/seg_2B4578", SHAPE_unpack);

INCLUDE_ASM("ealib/seg_2B4578", func_003B38B8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B3900);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *f_3A10_3900(char *) __asm__("func_003B3A10");
extern void f_3D00_3900(char *, int, void *) __asm__("func_003B3D00");
extern int f_65A8_3900(void *, int) __asm__("func_004165A8");
struct B_3900 { int pad; char c; };
struct H_3900 { char pad[8]; int n; char pad2[8]; struct { int off; int x; } t[1]; };

char *func_003B3900_3900(struct H_3900 *, int) __asm__("func_003B3900");
char *func_003B3900_3900(struct H_3900 *arg0, int arg1) {
    int i;
    int n = arg0->n;
    struct B_3900 buf;
    char *p;
    for (i = 0; i < n; i++) {
        p = f_3A10_3900((char *)arg0 + arg0->t[i].off);
        if (p == 0) {
            buf.c = 0;
            f_3D00_3900((char *)arg0, i, &buf);
            p = (char *)&buf;
        }
        if (f_65A8_3900(p, arg1) == 0)
            return (char *)arg0 + arg0->t[i].off;
    }
    return 0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B3BB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B3A50_3BB0(unsigned char, short, short, unsigned int, int) __asm__("func_003B3A50");
extern int func_003B4D00_3BB0(char *, int *) __asm__("func_003B4D00");
extern int strlen_3BB0(char *) __asm__("strlen");

int func_003B3BB0(char *p) {
    int sp0;
    int r = 0;
    int w;
    unsigned int u;
    char *q;
    unsigned int t = *(unsigned char *)p;
    switch (t) {
    case 0x70:
        return strlen_3BB0(p + 4) + 5;
    case 0x6F:
        return *(int *)(p + 4) + 8;
    case 0x7C:
        return (*(int *)(p + 4) << 3) + 8;
    case 0x69:
        if (*(unsigned short *)(p + 6) & 0x10) {
            u = *(unsigned short *)(p + 4);
            if (u == 0) u = 0xF0;
            return u + 0x10;
        }
        return 0x10;
    default:
        if (*(unsigned char *)p & 0x80) {
            sp0 = 0;
            if (*(int *)(p + 0xC) & 0x1000) {
                q = p + *(int *)(p + 0x10);
            } else {
                q = p + 0x10;
            }
            if (func_003B4D00_3BB0(q, &sp0) != 0) {
                r = sp0 + 0x10;
            }
        } else {
            w = *(int *)(p + 0xC);
            r = func_003B3A50_3BB0(*(unsigned char *)p, *(short *)(p + 4), *(short *)(p + 6), (unsigned int)w >> 28, (w >> 13) & 1);
        }
        return r;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B3CCC);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3CD8);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3D00);

INCLUDE_ASM("ealib/seg_2B4578", func_003B3D40);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B3DA8);
#ifdef SKIP_ASM
int func_003B3DA8(int arg0) {
    int r = 0;
    switch (arg0) {
    case 4: r = 1; break;
    case 8: r = 2; break;
    case 0xF: case 0x10: case 0x22B: case 0x613: r = 3; break;
    case 0x20: case 0x22B8: r = 5; break;
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B4818);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int f_4818(unsigned char *p) __asm__("func_003B4818");
static __inline__ int get24_4818(unsigned char *q) { return (q[0] << 16) | (q[1] << 8) | q[2]; }
static __inline__ int get32_4818(unsigned char *q) { return (q[0] << 24) | (q[1] << 16) | (q[2] << 8) | q[3]; }
int f_4818(unsigned char *p) {
    int v = 0;
    if (p[1] == 0xFB) {
        switch (p[0] & 0xFE) {
        case 0x10: case 0x18: case 0x1A: case 0x1E:
        case 0x30: case 0x32: case 0x34: case 0x46:
            v = get24_4818(p + 2);
            break;
        case 0x90: case 0x98: case 0x9A: case 0x9E:
        case 0xB0: case 0xB2: case 0xB4: case 0xC6:
            v = get32_4818(p + 2);
            break;
        case 0xC0:
            break;
        }
    }
    return v;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5320);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B5440_5320() __asm__("func_003B5440");
extern int func_003B82B8_5320(int, int, int, int, int) __asm__("func_003B82B8");
extern void func_003BC530_5320() __asm__("func_003BC530");
extern void func_003B8A38_5320() __asm__("func_003B8A38");
extern void func_003C2268_5320() __asm__("func_003C2268");
extern void func_003B58A0_5320() __asm__("func_003B58A0");
extern void func_003B5E98_5320(int) __asm__("func_003B5E98");
extern void func_003B58D8_5320() __asm__("func_003B58D8");
extern int func_003B5C40_5320() __asm__("func_003B5C40");
extern void func_003C3358_5320() __asm__("func_003C3358");
struct G_5320 {
    char pad0[0x174]; char f174; char pad1[0x1D8 - 0x175];
    void (*f1D8)(void); void (*f1DC)(void); void (*f1E0)(void); void (*f1E4)(int);
    void (*f1E8)(void); void (*f1EC)(void); int f1F0; int f1F4;
};
extern struct G_5320 D_0050A8E8_5320 __asm__("D_0050A8E8");

int func_003B5320(void) {
    int r;
    if (func_003B5440_5320() == 0) return -14;
    func_003B82B8_5320(0, 0, 0, -1, -1);
    if (D_0050A8E8_5320.f1D8) D_0050A8E8_5320.f1D8();
    if (D_0050A8E8_5320.f1DC) D_0050A8E8_5320.f1DC();
    if (D_0050A8E8_5320.f1E0) D_0050A8E8_5320.f1E0();
    if (D_0050A8E8_5320.f1EC) D_0050A8E8_5320.f1EC();
    if (D_0050A8E8_5320.f1E8) D_0050A8E8_5320.f1E8();
    func_003BC530_5320();
    func_003B8A38_5320();
    if (D_0050A8E8_5320.f1E4) D_0050A8E8_5320.f1E4(-1);
    func_003C2268_5320();
    func_003B58A0_5320();
    func_003B5E98_5320(D_0050A8E8_5320.f1F0);
    func_003B5E98_5320(D_0050A8E8_5320.f1F4);
    func_003B58D8_5320();
    r = func_003B5C40_5320();
    D_0050A8E8_5320.f174 = 0;
    func_003C3358_5320();
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5948);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5948 { char pad[0x174]; char on; char p1; char p2; signed char n; char pad2[12]; int arr[8]; };
extern struct S_5948 D_a8e8_5948 __asm__("D_0050A8E8");
extern void func_003B58A0();
extern void func_003B58D8();

void func_003B5948(int a) {
    int i;
    if (D_a8e8_5948.on) {
        func_003B58A0();
        for (i = 0; i < D_a8e8_5948.n; i++) {
            if (D_a8e8_5948.arr[i] == a) {
                D_a8e8_5948.n--;
                for (; i < D_a8e8_5948.n; i++) D_a8e8_5948.arr[i] = D_a8e8_5948.arr[i + 1];
                break;
            }
        }
        func_003B58D8();
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5A60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_5A60 { char pad[0x179]; signed char cnt; char pad2[0x4E]; int list[1]; };
extern struct G_5A60 D_0050A8E8_5A60[] __asm__("D_0050A8E8");

void func_003B5A60(int arg) {
    int i;
    func_003B58A0();
    for (i = 0; i < D_0050A8E8_5A60[0].cnt; i++) {
        if (D_0050A8E8_5A60[0].list[i] == arg) {
            D_0050A8E8_5A60[0].cnt--;
            if (i < D_0050A8E8_5A60[0].cnt) {
                do {
                    D_0050A8E8_5A60[0].list[i] = D_0050A8E8_5A60[0].list[i + 1];
                    i++;
                } while (i < D_0050A8E8_5A60[0].cnt);
            }
            break;
        }
    }
    func_003B58D8();
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5BD0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct H_5BD0 { int f0; int f4; int f8; int fC; int f10; };
struct G_5BD0 { char pad[0x234]; struct H_5BD0 *cur; };
extern struct G_5BD0 D_0050A8E8_5BD0 __asm__("D_0050A8E8");

void func_003B5BD0(struct H_5BD0 *a0, int a1) {
    D_0050A8E8_5BD0.cur = a0;
    a0->f8 = a1;
    D_0050A8E8_5BD0.cur->f4 = (int)a0 + a1 - 8;
    a1 -= 0xF;
    D_0050A8E8_5BD0.cur->f0 = (int)a0 + 0x18;
    D_0050A8E8_5BD0.cur->f0 += 0xF;
    D_0050A8E8_5BD0.cur->f0 &= 0xFFFFFFF0;
    D_0050A8E8_5BD0.cur->fC = a1 - 0x20;
    D_0050A8E8_5BD0.cur->f10 = a1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5C40);
#ifdef SKIP_ASM
extern void func_003C6240();

void func_003B5C40(void) {
    func_003C6240();
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B5C60);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5E98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003B5E98_5E98(int) __asm__("func_003B5E98");
struct E_5E98 { int key; int val; };
struct S_5E98 { int base; struct E_5E98 *arr; int p2; int x; int p4; int cnt; };
struct G_5E98 { char pad[0x234]; struct S_5E98 *s; };
extern struct G_5E98 D_0050A8E8_5E98[] __asm__("D_0050A8E8");

void func_003B5E98_5E98(int a) {
    int i = 0;
    int d = a - D_0050A8E8_5E98[0].s->base;
    if (D_0050A8E8_5E98[0].s->cnt >= 0) return;
    while (1) {
        struct S_5E98 *s = D_0050A8E8_5E98[0].s;
        if (((struct E_5E98 *)((char *)s->arr + (i << 3)))->key == d) {
            s->cnt++;
            D_0050A8E8_5E98[0].s->x += 8;
            if (!(D_0050A8E8_5E98[0].s->cnt < i)) return;
            do {
                D_0050A8E8_5E98[0].s->arr[i] = D_0050A8E8_5E98[0].s->arr[i - 1];
                i--;
            } while (D_0050A8E8_5E98[0].s->cnt < i);
            return;
        }
        i--;
        if (!(s->cnt < i)) return;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6670);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B64D8_6670() __asm__("func_003B64D8");
extern void func_003BB820_6670(int, int, char *, int, int, int) __asm__("func_003BB820");
extern int func_003C6540_6670(int, int) __asm__("func_003C6540");
struct G_6670 { char pad[0x1F4]; int f1F4; };
extern struct G_6670 D_0050A8E8_6670 __asm__("D_0050A8E8");

int func_003B6670(int a0, int a1, char *a2, int a3, int a4) {
    int off, o, i;
    int *p;
    if (func_003B64D8_6670() < 0) return -8;
    off = a0 * 0x10;
    o = func_003C6540_6670(*(int *)(off + D_0050A8E8_6670.f1F4), a1);
    if (o == 0) return -8;
    i = 0;
    func_003B58A0();
    func_003BB820_6670(*(int *)(off + D_0050A8E8_6670.f1F4), o, a2, a3, a4, 0);
    if (*(unsigned char *)(a2 + 2) != 0) {
        p = (int *)(a3 + 0x18);
        do {
            if (*p != 0) {
                func_003B5E98(*p);
                *p = 0;
                p[6] = 0;
            }
            i++;
            p++;
        } while (i < *(unsigned char *)(a2 + 2));
    }
    func_003B58D8();
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B6788);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B5F98_6788(void *, int) __asm__("func_003B5F98");
extern int func_003BAAC0_6788(char **, void *) __asm__("func_003BAAC0");
extern void func_003C3EA8_6788(void *, int, int) __asm__("func_003C3EA8");

int func_003B6788(int arg0, char *arg1, int arg2, int arg3) {
    char sp[0xD0];
    char *spD0;
    int temp_16;

    spD0 = arg1 + 4;
    do {
        temp_16 = func_003BAAC0_6788(&spD0, sp);
        if (*(unsigned short *)(sp + 0x18) & arg0) {
            func_003C3EA8_6788(sp, arg2, arg3);
            func_003B5F98_6788(sp, 0);
        }
    } while (temp_16 != 0);
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B6820);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B68B8);
#ifdef SKIP_ASM
extern int func_003B8CC8(int, int);
extern int func_003C6760(int, int, int, int);

void func_003B68B8(char *p) {
    int i = 0;
    char *q = p + 0x104;
    int *f = (int*)(p + 0x114);
    *(int *)(p + 4) = func_003C6760(*(int *)(p + 8), (int)(p + 0x14), (int)(p + 0x1C), (int)(p + 0xEC));
    do {
        if (*f != 0) {
            func_003B8CC8(*(int *)(p + 4), (int)q);
        }
        q += 0x18;
        i++;
        f += 6;
    } while (i == 0);
    *(char *)(p + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6948);
#ifdef SKIP_ASM
int func_003B6948(unsigned char *p) {
    int v = 0;
    int w = *(unsigned short *)p;
    int h = p[2];
    int t = p[3];
    w = w * h;
    if (t == 5) v = 0x92;
    else if (t == 10) v = 0x88;
    else if (t == 4) v = 0x33;
    else if (t == 22) v = 0x66;
    else if (t == 8) v = 0x200;
    else if (t == 16) return h << 3;
    else if (t == 23) return h << 3;
    return (w * v) >> 8;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8160);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_A7B0_8160(int) __asm__("func_003BA7B0");
extern void f_2A50_8160(int) __asm__("func_003C2A50");
struct T_8160 { int w0; char pad[0x65]; char c69; char pad2[0x22]; };
struct G_8160 { char pad[0x175]; char c; char pad2[8]; short n; char pad3[0x6F]; struct T_8160 *tab; };
extern struct G_8160 D_0050A8E8_8160[] __asm__("D_0050A8E8");

int func_003B8160(int arg0) {
    int i;
    struct T_8160 *e;
    func_003B58A0();
    D_0050A8E8_8160[0].c = arg0;
    for (i = 0; i < D_0050A8E8_8160[0].n; i++) {
        e = (struct T_8160 *)((char *)D_0050A8E8_8160[0].tab + i * 0x8C);
        if (e->c69 == 1 && e->w0 >= 0) {
            f_A7B0_8160(i);
            f_2A50_8160(i);
        }
    }
    func_003B58D8();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8218);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050AAE0_8218[] __asm__("D_0050AAE0");
extern char D_0050AAF4_8218[] __asm__("D_0050AAF4");
extern char D_0050AB08_8218[] __asm__("D_0050AB08");

void *func_003B8218(int arg0, int arg1) {
    switch (arg1 & 0x71C) {
    case 8:
        return (arg0 * 0x14) + D_0050AB08_8218;
    case 4:
        return (arg0 * 0x14) + D_0050AAE0_8218;
    case 0x100:
        return (arg0 * 0x14) + D_0050AAF4_8218;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8530);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8530(int, int *) __asm__("func_003BAA00");
extern int func_003BA6E8_8530(void) __asm__("func_003BA6E8");
struct G_8530 { char pad[0x1F0]; char *tab; };
extern struct G_8530 D_0050A8E8_8530[] __asm__("D_0050A8E8");

int func_003B8530(int a, int n, int v) {
    int h = func_003BA6E8_8530();
    int sp0;
    if (h >= 0) {
        if (n <= 0) n = 1;
        sp0 = -1;
        v <<= 16;
        while (func_003BAA00_8530(h, &sp0)) {
            char *e = D_0050A8E8_8530[0].tab + sp0 * 0x8C;
            *(int *)(e + 0x34) = v;
            *(int *)(e + 0x30) = (v - *(int *)(e + 0x38)) / n;
        }
    }
    return h;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8700);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8700(int, int *) __asm__("func_003BAA00");
extern void func_003C2EC8_8700(int) __asm__("func_003C2EC8");
struct G_8700 { char pad[0x1F0]; char *tbl; };
extern struct G_8700 D_0050A8E8_8700 __asm__("D_0050A8E8");

int func_003B8700(int arg0, int arg1) {
    int sp0;
    int h;

    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00_8700(h, &sp0) != 0) {
            
            *(char *)(D_0050A8E8_8700.tbl + sp0 * 0x8C + 0x5F) = arg1;
            func_003C2EC8_8700(sp0);
        }
    }
    return h;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8790);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BA6E8_8790(void) __asm__("func_003BA6E8");
extern void func_003BA938_8790(int, int) __asm__("func_003BA938");
extern int func_003BAA00_8790(int, int *) __asm__("func_003BAA00");
extern void func_003C25E0_8790(int) __asm__("func_003C25E0");
extern char D_0050A8E8_8790[] __asm__("D_0050A8E8");
int func_003B8790_8790(int a0, int arg1) __asm__("func_003B8790");

int func_003B8790_8790(int a0, int arg1) {
    int sp0;
    int h;
    char *e;
    char *g;
    int i;
    h = func_003BA6E8_8790();
    if (h >= 0) {
        sp0 = -1;
        g = D_0050A8E8_8790;
        while (func_003BAA00_8790(h, &sp0) != 0) {
            i = sp0;
            e = *(char **)(g + 0x1F0) + i * 0x8C;
            if (*(unsigned short *)(e + 0x88) == arg1) {
                return 0;
            }
            *(unsigned short *)(e + 0x88) = arg1;
            func_003BA938_8790(i, i);
            func_003C25E0_8790(sp0);
        }
    }
    return h;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8838);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BAA00_8838(int, int *) __asm__("func_003BAA00");
extern void func_003C3010_8838(int, int) __asm__("func_003C3010");
struct G_8838 { char pad[0x1F0]; char *tbl; };
extern struct G_8838 D_0050A8E8_8838 __asm__("D_0050A8E8");

int func_003B8838(int arg0, int arg1) {
    int sp0;
    int h;

    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00_8838(h, &sp0) != 0) {
            int i = sp0;
            *(short *)(D_0050A8E8_8838.tbl + i * 0x8C + 0x6A) = arg1;
            func_003C3010_8838(i, arg1);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B88C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003BA6E8_88C8() __asm__("func_003BA6E8");
extern void func_003BA740_88C8(int, int) __asm__("func_003BA740");
extern int func_003BAA00_88C8(int, int *) __asm__("func_003BAA00");
extern void func_003C27E0_88C8(int, int) __asm__("func_003C27E0");
extern char D_0050A8E8_88C8[] __asm__("D_0050A8E8");

int func_003B88C8(int arg0, int arg1, int arg2) {
    int sp0;
    int t;
    char *b;
    t = func_003BA6E8_88C8();
    if (t >= 0) {
        sp0 = -1;
        b = D_0050A8E8_88C8;
        while (func_003BAA00_88C8(t, &sp0) != 0) {
            *(char *)(sp0 * 0x8C + *(int *)(b + 0x1F0) + arg1 + 0x61) = arg2;
            func_003BA740_88C8(arg1, sp0);
            func_003C27E0_88C8(sp0, arg1);
        }
    }
    return t;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_8C00 { char p0[0x1C]; unsigned short s1c; unsigned short s1e; char p1[0x2C]; unsigned short s4c; char p2[0x8C - 0x4E]; };
struct G_8C00 { char pad[0x1F0]; struct E_8C00 *tab; };
extern struct G_8C00 D_0050A8E8_8C00[] __asm__("D_0050A8E8");

int func_003B8C00(int a, int b, int c) {
    int h;
    int sp0;
    if (c >= 0x4000) {
        c = 0x3FFF;
    } else if (c < -0x4000) {
        c = -0x4000;
    }
    h = func_003BA6E8();
    if (h >= 0) {
        sp0 = -1;
        while (func_003BAA00(h, &sp0)) {
            struct E_8C00 *e = &D_0050A8E8_8C00[0].tab[sp0];
            e->s1c = e->s4c + b;
            e->s1e = c;
            func_003C38E0(sp0);
        }
    }
    return h;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8EE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_8EE8 { int id; signed char owner; char pad[3]; };
extern struct E_8EE8 D_0050A820_8EE8[] __asm__("D_0050A820");
extern int func_003BA6E8_8EE8(int) __asm__("func_003BA6E8");

int func_003B8EE8(int arg0) {
    int i;
    int r = 1;
    for (i = 0; i < 0x18; i++) {
        if (D_0050A820_8EE8[i].owner == arg0 && D_0050A820_8EE8[i].id >= 0 && func_003BA6E8_8EE8(D_0050A820_8EE8[i].id) >= 0) {
            r = 0;
            break;
        }
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B8F78);

INCLUDE_ASM("ealib/seg_2B4578", func_003B9140);

INCLUDE_ASM("ealib/seg_2B4578", func_003B91E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B93C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct B_93C8 { unsigned int lo : 17; unsigned int f : 7; unsigned int hi : 8; };
struct E_93C8 { int id; signed char owner; signed char sub; short pad; };
struct R_93C8 { char pad0[8]; unsigned char *cur; char pad1[0x70 - 12]; };
struct G_93C8 { struct R_93C8 r[16]; struct E_93C8 e[24]; };
extern struct G_93C8 D_a120_93C8 __asm__("D_0050A120");
extern void fC6B8_93C8(int, int) __asm__("func_003BC6B8");

void func_003B93C8(int a0, struct B_93C8 *a1, int a2, int a3) {
    struct R_93C8 *r = &D_a120_93C8.r[a0];
    int t, i;
    struct E_93C8 *e = &D_a120_93C8.e[0];
    i = 0x17;
    t = (*r->cur++ << 7) + a3;
    a1->f = t / 128;
    do {
        i -= 1;
        if (e->owner == a0 && e->sub == a2) {
            fC6B8_93C8(e->id, a1->f);
        }
        e += 1;
    } while (i >= 0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B94B0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B96B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B8DE8_96B8(int) __asm__("func_003B8DE8");
extern void func_003B94B0_96B8(int) __asm__("func_003B94B0");
struct E_96B8 { int f0; unsigned int f4; char pad[0x18 - 8]; int f18; char pad2[0x24 - 0x1C]; unsigned short u24; char pad4[2]; signed char st; char pad3[0x70 - 0x29]; };
extern struct E_96B8 D_0050A120_96B8[] __asm__("D_0050A120");

void func_003B96B8(void) {
    int i;
    struct E_96B8 *e;
    signed char st;
    unsigned int lim;
    int t;
    for (i = 0; i < 16; i++) {
        e = &D_0050A120_96B8[i];
        st = e->st;
        if (st == 3 && (e->f18 == 0 || func_003B8DE8_96B8(i) == 0)) {
            t = e->f0;
            lim = (unsigned int)(t * e->u24) / 100u;
            e->f0 = t + 1;
            if (lim >= e->f4 && e->st == st) {
                do {
                    func_003B94B0_96B8(i);
                    e->f4++;
                } while (lim >= e->f4 && e->st == 3);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B9D00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B89E0_9D00(int) __asm__("func_003B89E0");
extern int D_0050A120_9D00[] __asm__("D_0050A120");
typedef struct { int a; signed char b; char pad[3]; } E_9D00;

int func_003B9D00(int arg0) {
    int r, h, i;

    E_9D00 *p;
    char *t;

    func_003B58A0();
    h = func_003B8D98(arg0);
    if (h < 0) {
        func_003B58D8();
        return h;
    }
    t = (char*)D_0050A120_9D00 + h * 0x70;
    if (*(signed char*)(t + 0x28) == 3) {
        p = (E_9D00*)(((char*)D_0050A120_9D00) + 0x700);
        *(signed char*)(t + 0x28) = 5;
        i = 0x17;
        do {
            i--;
            if (p->b == h) {
                if (p->a >= 0) func_003B89E0_9D00(p->a);
            }
            p++;
        } while (i >= 0);
    }
    r = func_003B9FC8(arg0);
    func_003B58D8();
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B9DC8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B9E48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A120_9E48[] __asm__("D_0050A120");
extern int func_003B8D98_9E48() __asm__("func_003B8D98");
extern void func_003B85F0(int, unsigned int);

int func_003B9E48(int a0, int vol) {
    int ret = 0;
    int idx = func_003B8D98_9E48();
    if (idx < 0) {
        return idx;
    }
    {
        int key = vol << 16;
        char *e = D_0050A120_9E48 + idx * 0x70;
        *(int *)(e + 0x18) = 0;
        if (*(int *)(e + 0x14) != key) {
            if (*(signed char *)(e + 0x28) == 3) {
                char *p;
                int i;
                *(int *)(e + 0x14) = key;
                p = D_0050A120_9E48 + 0x700;
                i = 0x17;
                do {
                    if (*(signed char *)(p + 4) == idx) {
                        int v = *(int *)p;
                        if (v >= 0) {
                            unsigned x = *(unsigned *)(e + (*(signed char *)(p + 5) << 2) + 0x30);
                            func_003B85F0(v, ((x >> 10 & 0x7F) * vol) >> 7);
                        }
                    }
                    i -= 1;
                    p += 8;
                } while (i >= 0);
            } else {
                ret = -1;
            }
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B9F38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A120_9F38[] __asm__("D_0050A120");

int func_003B9F38(int arg0, int arg1, int arg2) {
    int r;
    char *e;
    int t;
    r = func_003B8D98(arg0);
    if (r < 0) return r;
    e = D_0050A120_9F38 + r * 0x70;
    if (*(signed char *)(e + 0x28) == 3) {
        if (arg1 <= 0) arg1 = 1;
        t = arg2 << 16;
        *(int *)(e + 0x1C) = t;
        *(int *)(e + 0x18) = (t - *(int *)(e + 0x14)) / arg1;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BA550);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_A550 { int f0; char pad[0x22]; unsigned char f26; unsigned char f27; char pad2[4]; int f2C; char pad3[0x69 - 0x30]; signed char f69; char pad4[0x8C - 0x6A]; };
struct G_A550 { char pad[0x17E]; short cnt; int f180; char pad2[0x1F0 - 0x184]; struct E_A550 *tab; };
extern struct G_A550 D_0050A8E8_A550 __asm__("D_0050A8E8");
void func_003BA550(int i, unsigned char u) {
    struct E_A550 *e = &D_0050A8E8_A550.tab[i];
    unsigned char k = e->f26;
    int n = 0;
    int sel = -1;
    int j;
    if (k != 0) {
        for (j = 0; j < D_0050A8E8_A550.cnt; j++) {
            struct E_A550 *q = &D_0050A8E8_A550.tab[j];
            if (q->f26 == k && q->f0 >= 0 && q->f69 != 0) {
                n++;
                if (q->f27) sel = j;
            }
        }
        e = &D_0050A8E8_A550.tab[i];
        if (n == 1) {
            e->f69 = 0;
            e->f26 = 0;
            e->f27 = 0;
            e->f2C = D_0050A8E8_A550.f180;
            return;
        }
        if (D_0050A8E8_A550.tab[sel].f69 == 2 && i != sel && n == 2) {
            e->f69 = 0;
            e->f26 = 0;
            e->f27 = 0;
            e->f2C = D_0050A8E8_A550.f180;
            D_0050A8E8_A550.tab[sel].f69 = 0;
            D_0050A8E8_A550.tab[sel].f26 = 0;
            D_0050A8E8_A550.tab[sel].f27 = 0;
            D_0050A8E8_A550.tab[sel].f2C = D_0050A8E8_A550.f180;
            return;
        }
        if (D_0050A8E8_A550.tab[sel].f69 == 1 && i == sel) {
            D_0050A8E8_A550.tab[sel].f69 = 2;
            return;
        }
        e->f69 = 0;
        e->f26 = 0;
        e->f27 = 0;
        e->f2C = D_0050A8E8_A550.f180;
        return;
    }
    e->f69 = 0;
    e->f26 = 0;
    e->f27 = 0;
    e->f2C = D_0050A8E8_A550.f180;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BA888);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044C480_A888[] __asm__("D_0044C480");

int func_003BA888(int x) {
    int scale = 0x1000;
    int t;
    while (x >= 0x4B0) {
        x -= 0x4B0;
        scale <<= 1;
    }
    while (x < -0x4AF) {
        x += 0x4B0;
        scale >>= 1;
    }
    t = (x * 0x369D) >> 16;
    if (t <= -0x100)
        t = -0xFF;
    if (t < 0)
        scale = (scale * (D_0044C480_A888[t + 0x100] + 0x100)) >> 9;
    else
        scale = (scale * (D_0044C480_A888[t] + 0x100)) >> 8;
    return scale;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BA938);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BAA00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_AA00 { int w; char p0[0x22]; unsigned char type; char p1[0x42]; signed char b; char p2[0x22]; };
struct G_AA00 { char pad[0x17E]; short cnt; char pad2[0x70]; struct E_AA00 *tab; };
extern struct G_AA00 D_0050A8E8_AA00[] __asm__("D_0050A8E8");

int func_003BAA00(int h, int *pos) {
    struct E_AA00 *e = &D_0050A8E8_AA00[0].tab[h];
    unsigned char t = e->type;
    if (t != 0) {
        (*pos)++;
        while (*pos < D_0050A8E8_AA00[0].cnt) {
            e = &D_0050A8E8_AA00[0].tab[*pos];
            if (e->type == t && e->b == 1 && e->w >= 0) return 1;
            (*pos)++;
        }
    } else if (*pos < 0) {
        *pos = h;
        return 1;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC6B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_A6E8_C6B8() __asm__("func_003BA6E8");
extern void f_A938_C6B8(int) __asm__("func_003BA938");
extern int f_AA00_C6B8(int, int *) __asm__("func_003BAA00");
extern void f_25E0_C6B8(int) __asm__("func_003C25E0");
struct G_C6B8 { char pad[0x1F0]; char *tab; };
extern struct G_C6B8 D_0050A8E8_C6B8[] __asm__("D_0050A8E8");

int func_003BC6B8(int arg0, int arg1) {
    int sp0;
    int r = f_A6E8_C6B8();
    char *e;
    if (r >= 0) {
        sp0 = -1;
        while (f_AA00_C6B8(r, &sp0) != 0) {
            e = D_0050A8E8_C6B8[0].tab + sp0 * 0x8C;
            if (*(char *)(e + 0x64) == arg1)
                return 0;
            if (*(short *)(e + 0x82) != 0) {
                *(short *)(e + 0x86) = 0;
                *(char *)(e + 0x64) = arg1;
                f_A938_C6B8(sp0);
                f_25E0_C6B8(sp0);
            } else
                return 0;
        }
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC968);
#ifdef SKIP_ASM
void func_003BC968(int arg0, int arg1, int arg2, int arg3) {
    if (*(volatile int *)0x1000D400 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
    *(volatile int *)0x1000D410 = arg0;
    *(volatile int *)0x1000D480 = arg1;
    *(volatile int *)0x1000D420 = arg2;
    *(volatile int *)0x1000D400 = 0x100;
    if ((arg3 & 0x10000) && (*(volatile int *)0x1000D400 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BCA28);
#ifdef SKIP_ASM
void func_003BCA28(int arg0, int arg1, int arg2, int arg3) {
    if (*(volatile int *)0x1000D000 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
    *(volatile int *)0x1000D010 = arg1;
    *(volatile int *)0x1000D080 = arg0;
    *(volatile int *)0x1000D020 = arg2;
    *(volatile int *)0x1000D000 = 0x100;
    if ((arg3 & 0x10000) && (*(volatile int *)0x1000D000 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BCAE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char p0[6]; unsigned short pos; unsigned char *buf; } BW_CAE8;
extern BW_CAE8 *D_0050AC68_CAE8[] __asm__("D_0050AC68");

void func_003BCAE8(int arg0, int n) {
    unsigned int v = arg0 << (0x20 - n);
    int byte;
    int room;
    int take;
    int rem;
    v = v >> (0x20 - n);
    while (n > 0) {
        byte = D_0050AC68_CAE8[0]->pos >> 3;
        if (byte & 1)
            byte = byte - 1;
        else
            byte = byte + 1;
        room = 8 - (D_0050AC68_CAE8[0]->pos & 7);
        if (room == 8)
            D_0050AC68_CAE8[0]->buf[byte] = 0;
        take = (room >= n) ? n : room;
        rem = n - take;
        D_0050AC68_CAE8[0]->buf[byte] |= (v >> rem) << (room - take);
        n = rem;
        D_0050AC68_CAE8[0]->pos += take;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BCBA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD078);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD220);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD5B8);

INCLUDE_ASM("ealib/seg_2B4578", func_003BD9C0);

INCLUDE_ASM("ealib/seg_2B4578", func_003BDE08);

INCLUDE_ASM("ealib/seg_2B4578", func_003BE6E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BECA0);
#ifdef SKIP_ASM
int func_003BECA0(int a, int b, int *o1, int *o2) {
    int w, m;
    if (b < 0) return -2;
    if (b < 0x22) w = 0xE0;
    else if (b < 0x43) w = 0x160;
    else if (b < 0x65) w = 0x1E0;
    else return -2;
    if (a == 1) m = 4;
    else if (a == 2) m = 5;
    else return -1;
    *o1 = (w * 4 + 0x3C) * m + 0xA8;
    *o2 = (w + (w + 0x200)) * 4 + 0xC00;
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C04F0);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed halfwords).
void func_003C04F0(void *in, void *out, float s) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0xFFF\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
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
        "pand      $6, $6, $10\n"
        "pand      $8, $8, $10\n"
        "sq        $6, 0x0(%1)\n"
        "sq        $8, 0x10(%1)\n"
        :
        : "r"(in), "r"(out), "f"(s)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0564);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed halfwords).
void func_003C0564(void *in, void *out, float s) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0x3FF\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
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
        "pand      $6, $6, $10\n"
        "pand      $8, $8, $10\n"
        "sq        $6, 0x0(%1)\n"
        "sq        $8, 0x10(%1)\n"
        :
        : "r"(in), "r"(out), "f"(s)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0640);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed bytes).
void func_003C0640(void *in, void *out, float s) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0x1F1F\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
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
        "pand      $6, $6, $10\n"
        "sq        $6, 0x0(%1)\n"
        :
        : "r"(in), "r"(out), "f"(s)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0C18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A8E8_0C18[] __asm__("D_0050A8E8");
extern unsigned char D_0050A918_0C18[] __asm__("D_0050A918");

void func_003C0C18(int arg0, int *arg1, int *arg2) {
    if (arg0 & 8) {
        *arg1 = 0;
        *arg2 = D_0050A918_0C18[0];
        return;
    }
    if (arg0 & 0x100) {
        char *b = D_0050A8E8_0C18;
        *arg1 = *(unsigned char *)(b + 0x30);
        *arg2 = *(unsigned char *)(b + 0x30) + *(unsigned char *)(b + 0x2F);
        return;
    }
    if (arg0 & 4) {
        char *b = D_0050A8E8_0C18;
        *arg1 = *(unsigned char *)(b + 0x30) + *(unsigned char *)(b + 0x2F);
        *arg2 = *(short *)(b + 0x17E);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C2268);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x176];
    unsigned char f176;
    char pad177[0x180 - 0x177];
    unsigned int f180;
} G_2268;
typedef struct { int h; int pad[3]; } V_2268;
typedef struct {
    char pad0[0xE8C];
    V_2268 v[1];
    char padE9C[0x1AA0 - 0xE9C];
    int f1AA0;
    int f1AA4;
    int f1AA8;
    char pad1AAC;
    unsigned char f1AAD;
    char pad1AAE[0x1AD4 - 0x1AAE];
    int f1AD4;
    int f1AD8;
    char pad1ADC[0x1AF8 - 0x1ADC];
    int f1AF8;
    char pad1AFC[0x1B38 - 0x1AFC];
    int f1B38;
} H_2268;
extern volatile G_2268 D_0050A8E8_2268 __asm__("D_0050A8E8");
extern H_2268 D_0050AD00_2268 __asm__("D_0050AD00");
extern int D_00512B20_2268[] __asm__("D_00512B20");
extern void func_003B58D8_2268(void) __asm__("func_003B58D8");
extern void func_003B58A0_2268(void) __asm__("func_003B58A0");
extern void func_003B5E98_2268(int) __asm__("func_003B5E98");
extern void func_003C0B10_2268(int, int, int) __asm__("func_003C0B10");
extern void func_003C60E0_2268(void) __asm__("func_003C60E0");
extern void func_003C8018_2268(void) __asm__("func_003C8018");
extern void func_003CBC00_2268(int) __asm__("func_003CBC00");
extern int func_00423AA0_2268(int, int) __asm__("func_00423AA0");
extern void func_00423BB0_2268(int) __asm__("func_00423BB0");
extern void func_00423BF0_2268(int) __asm__("func_00423BF0");
extern void func_00423CA0_2268(int, void *) __asm__("func_00423CA0");
extern void func_00423DB0_2268(int) __asm__("func_00423DB0");
extern void func_00424880_2268(int) __asm__("func_00424880");
extern void func_004248E8_2268(int) __asm__("func_004248E8");
extern void func_0042AD08_2268(int) __asm__("func_0042AD08");

int func_003C2268(void) {
    int st[12];
    unsigned int t;
    int i;

    while (D_0050A8E8_2268.f176 != 0) {
        func_003B58D8_2268();
    }
    t = D_0050A8E8_2268.f180 + 10;
    while (D_0050A8E8_2268.f180 < t) {
        func_003C60E0_2268();
    }
    if (D_0050AD00_2268.f1AA8 >= 0) {
        func_00424880_2268(2);
        if (func_00423AA0_2268(2, D_0050AD00_2268.f1AA8) > 0) {
            func_004248E8_2268(2);
        }
    }
    func_003C0B10_2268(1, 0, 0);
    func_00423CA0_2268(D_00512B20_2268[0], st);
    if (st[0] == 0x10) {
        func_00423BB0_2268(D_00512B20_2268[0]);
    } else {
        func_00423BF0_2268(D_00512B20_2268[0]);
        func_00423BB0_2268(D_00512B20_2268[0]);
    }
    func_00423DB0_2268(D_0050AD00_2268.f1AA4);
    for (i = 0; i < 1; i++) {
        func_0042AD08_2268(D_0050AD00_2268.v[i].h);
    }
    func_003CBC00_2268(0);
    D_0050AD00_2268.f1AAD = 0;
    func_003B58A0_2268();
    if (D_0050AD00_2268.f1AD4 != 0) {
        func_003B5E98_2268(D_0050AD00_2268.f1AD4);
        D_0050AD00_2268.f1AD4 = 0;
        D_0050AD00_2268.f1AD8 = 0;
    }
    if (D_0050AD00_2268.f1AF8 != 0) {
        func_003B5E98_2268(D_0050AD00_2268.f1AF8);
        D_0050AD00_2268.f1AF8 = 0;
    }
    if (D_0050AD00_2268.f1B38 != 0) {
        func_003B5E98_2268(D_0050AD00_2268.f1B38);
        D_0050AD00_2268.f1B38 = 0;
    }
    func_003C8018_2268();
    func_003B5E98_2268(D_0050AD00_2268.f1AA0);
    func_003B58D8_2268();
    return 0;
}
#endif

void func_003C2438(void) {
}

INCLUDE_ASM("ealib/seg_2B4578", func_003C2440);

INCLUDE_ASM("ealib/seg_2B4578", func_003C25E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C27E0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C2A50);

INCLUDE_ASM("ealib/seg_2B4578", func_003C2EC8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3010);
#ifdef SKIP_ASM
extern void func_003C8B40(int, int);
struct G_3010 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };
struct E_3010 { char pad[4]; short v[15]; char padb; unsigned char cnt; unsigned short flags; };
extern struct G_3010 D_0050A8E8_3010[] __asm__("D_0050A8E8");

int func_003C3010(int arg0, int arg1) {
    struct E_3010 *e;
    int i;
    e = (struct E_3010 *)(D_0050A8E8_3010[0].tab + arg0 * 0x8C);
    if (e->flags & 4) {
        for (i = 0; i < e->cnt; i++)
            func_003C8B40(e->v[i] - (D_0050A8E8_3010[0].b + D_0050A8E8_3010[0].a), arg1);
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C30C8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3178);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3250);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C8C30(int, int);
struct G_3250 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };
struct E_3250 { char pad[4]; short v[15]; char padb; unsigned char cnt; unsigned short flags; };
extern struct G_3250 D_0050A8E8_3250[] __asm__("D_0050A8E8");

void func_003C3250(int arg0, int arg1) {
    struct E_3250 *e;
    int i;
    e = (struct E_3250 *)(D_0050A8E8_3250[0].tab + arg0 * 0x8C);
    if (e->flags & 4) {
        for (i = 0; i < e->cnt; i++)
            func_003C8C30(e->v[i] - (D_0050A8E8_3250[0].b + D_0050A8E8_3250[0].a), arg1);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C3300);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3358);

INCLUDE_ASM("ealib/seg_2B4578", func_003C3380);

INCLUDE_ASM("ealib/seg_2B4578", func_003C33E0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C3450);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_0B10_3450(int, void *, int) __asm__("func_003C0B10");
extern int f_72F8_3450(void *, int) __asm__("func_003C72F8");
extern char D_0050AAF4_3450[] __asm__("D_0050AAF4");
struct G_3450 { char pad[0xF]; char c; void *fn; };
extern struct G_3450 D_00515B40_3450[] __asm__("D_00515B40");
extern char D_00515B4F_3450[] __asm__("D_00515B4F");
extern char D_0044DD70_3450[] __asm__("D_0044DD70");
extern char f_C878_3450[] __asm__("func_003CC878");

void func_003C3450(int arg0) {
    char *b = D_0050AAF4_3450;
    char *t = b - 0x20C;
    char *q;
    if (*(unsigned char *)(t + 0x42) != 0) {
        D_00515B40_3450[0].fn = f_C878_3450;
        D_00515B40_3450[0].c = 1;
    }
    if (arg0 != 1) {
        f_0B10_3450(2, D_0044DD70_3450, 4);
        if (*(unsigned char *)(t + 0x42) != 0)
            D_00515B4F_3450[0] = 0;
    } else {
        q = *(char **)(b + 4);
        if (*(unsigned char *)(t + 0x42) != 0)
            D_00515B4F_3450[0] = arg0;
        f_0B10_3450(2, q, f_72F8_3450(q + 4, 4));
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3500);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044DD48_3500[] __asm__("D_0044DD48");
struct A_3500 {
    char pad0[0xCB0];
    char *p;
    char pad1[0x1A9E - 0xCB4];
    short s;
};
struct B_3500 {
    char pad0[0x224];
    unsigned char a;
    char pad1[3];
    unsigned char b;
};
extern struct A_3500 D_0050AD00_3500 __asm__("D_0050AD00");
extern struct B_3500 D_0050A8E8_3500 __asm__("D_0050A8E8");

void func_003C3500(unsigned a) {
    unsigned v = a;
    if (a < 2) {
        v = 0;
    } else if (a == 5) {
        v = 1;
    } else if (a == 10) {
        v = 2;
    } else if (a == 20) {
        v = 3;
    } else if (a == 30) {
        v = 4;
    } else if (a == 40) {
        v = 5;
    } else if (a == 50) {
        v = 6;
    } else if (a == 100) {
        v = 9;
    } else if (a == 110) {
        v = 8;
    } else if (a == 120) {
        v = 7;
    }
    D_0050AD00_3500.s = (0x200000 - D_0044DD48_3500[v]) >> 6;
    D_0050AD00_3500.p[0x10] = v;
    D_0050AD00_3500.p[0x11] = D_0050A8E8_3500.a;
    D_0050AD00_3500.p[0x12] = D_0050A8E8_3500.b;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C43C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_43C0 { int a, b, c, d; };
extern void func_00424020(int);
extern void func_00424050(int);
extern int func_00424140(int);
extern int func_00424150(int);
extern int func_00424160(void *, int);
extern int func_00424170(void *, int);
extern int D_0044E178_43C0[] __asm__("D_0044E178");

void func_003C43C0(int arg0, int arg1, int arg2, int arg3) {
    struct S_43C0 s;
    s.a = arg0;
    s.c = arg2;
    s.b = arg1;
    s.d = 0;
    if (arg3 != 0) {
        func_00424050(0);
        while (func_00424150(D_0044E178_43C0[0]) >= 0) {}
        D_0044E178_43C0[0] = func_00424170(&s, 1);
    } else {
        func_00424020(0);
        while (func_00424140(D_0044E178_43C0[0]) >= 0) {}
        D_0044E178_43C0[0] = func_00424160(&s, 1);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C4450);

INCLUDE_ASM("ealib/seg_2B4578", func_003C45E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4668);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C4788);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C45E8_4788(int) __asm__("func_003C45E8");
extern int func_003C4668_4788(int, int, int, int, int) __asm__("func_003C4668");
extern volatile unsigned char D_0050AD00_4788[] __asm__("D_0050AD00");
int func_003C4788_4788(int, int, int, int, int) __asm__("func_003C4788");

int func_003C4788_4788(int a0, int a1, int a2, int a3, int a4) {
    int r = 0;
    int t;
    int c;
    while (a3 > 0) {
        t = 0x1000;
        if (!(a3 > 0xFFF)) t = a3;
        c = D_0050AD00_4788[0xCC0];
        while (0x1C - (signed char)c < 5) {
            func_003C45E8_4788(0);
            c = D_0050AD00_4788[0xCC0];
        }
        r = func_003C4668_4788(a0, a1, a2, t, a4);
        a1 += t;
        a2 += t;
        a3 -= t;
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C4898);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4918);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4978);
#ifdef SKIP_ASM
void func_003C4978(int arg0, int arg1, int arg2, int arg3) {
    if (*(volatile int *)0x1000D400 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
    *(volatile int *)0x1000D410 = arg0;
    *(volatile int *)0x1000D480 = arg1;
    *(volatile int *)0x1000D420 = arg2;
    *(volatile int *)0x1000D400 = 0x100;
    if ((arg3 & 0x10000) && (*(volatile int *)0x1000D400 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4A38);
#ifdef SKIP_ASM
void func_003C4A38(int arg0, int arg1, int arg2, int arg3) {
    if (*(volatile int *)0x1000D000 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
    *(volatile int *)0x1000D010 = arg1;
    *(volatile int *)0x1000D080 = arg0;
    *(volatile int *)0x1000D020 = arg2;
    *(volatile int *)0x1000D000 = 0x100;
    if ((arg3 & 0x10000) && (*(volatile int *)0x1000D000 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4AF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050AD00_4AF8[] __asm__("D_0050AD00");

void func_003C4AF8(int *arg0, int *arg1) {
    char *b = D_0050AD00_4AF8;
    int temp_4;
    int temp_6;
    int lo = *(unsigned short *)(b + 0x1A9A);

    temp_6 = *arg0;
    if (temp_6 < lo) {
        *arg0 = lo;
        *arg1 -= lo - temp_6;
    }
    if ((int)*(unsigned short *)(b + 0x1A9C) < *arg0 + *arg1) {
        *arg1 = *(unsigned short *)(b + 0x1A9C) - *arg0;
    }
    temp_4 = *arg0;
    if ((int)*(unsigned short *)(b + 0x1A9E) < temp_4 + *arg1) {
        *arg1 = *(unsigned short *)(b + 0x1A9E) - temp_4;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C4B70);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4D78);

INCLUDE_ASM("ealib/seg_2B4578", func_003C4E50);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4EC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_4EC8 { unsigned short a; unsigned short b; };
struct G_4EC8 { char pad[0x1A98]; unsigned short n; unsigned short lo; unsigned short hi; unsigned short max; struct S_4EC8 *segs; };
extern struct G_4EC8 D_0050AD00_4EC8 __asm__("D_0050AD00");
int f_4EC8(int *out) __asm__("func_003C4EC8");
int f_4EC8(int *out) {
    int start;
    int len;
    int best = 0;
    int i;
    struct S_4EC8 *seg;
    struct S_4EC8 *prev;

    if (D_0050AD00_4EC8.n == 0) {
        start = D_0050AD00_4EC8.lo;
        len = D_0050AD00_4EC8.hi - D_0050AD00_4EC8.lo;
        func_003C4AF8(&start, &len);
        best = len;
        *out = start;
    } else {
        for (i = 0; i < D_0050AD00_4EC8.n; i++) {
            seg = &D_0050AD00_4EC8.segs[i];
            if (i == 0) {
                start = seg->a;
                len = seg->a - 0x141;
                func_003C4AF8(&start, &len);
                if (best < len) {
                    best = len;
                    *out = start;
                }
            } else {
                prev = &D_0050AD00_4EC8.segs[i - 1];
                start = prev->a + prev->b;
                len = seg->a - start;
                func_003C4AF8(&start, &len);
                if (best < len) {
                    best = len;
                    *out = start;
                }
            }
        }
        prev = &D_0050AD00_4EC8.segs[i - 1];
        start = prev->a + prev->b;
        len = D_0050AD00_4EC8.max - start;
        func_003C4AF8(&start, &len);
        if (best < len) {
            best = len;
            *out = start;
        }
    }
    *out <<= 6;
    return best << 6;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C5068);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5128);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C8EC0(int, int);
struct G_5128 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };
struct E_5128 { char pad[4]; short v[15]; char padb; unsigned char cnt; unsigned short flags; };
extern struct G_5128 D_0050A8E8_5128[] __asm__("D_0050A8E8");

int func_003C5128(int arg0, int arg1) {
    struct E_5128 *e;
    int i;
    e = (struct E_5128 *)(D_0050A8E8_5128[0].tab + arg0 * 0x8C);
    if (e->flags & 4) {
        for (i = 0; i < e->cnt; i++)
            func_003C8EC0(e->v[i] - (D_0050A8E8_5128[0].b + D_0050A8E8_5128[0].a), arg1);
    }
    return 0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C5998);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003B4DD8_5998(void *) __asm__("func_003B4DD8");
extern void func_003B4FC0_5998(void *) __asm__("func_003B4FC0");

int func_003C5998(int arg0, float f) {
    char sp[0xE0];
    func_003B4DD8_5998(sp);
    switch (arg0) {
    case 0:
        *(signed char *)(sp + 0x40) = (int)(f * 100.0f);
        break;
    case 1:
        *(signed char *)(sp + 0x41) = (int)(f * 100.0f);
        break;
    default:
        return -5;
    }
    func_003B4FC0_5998(sp);
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5E58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003C5DF0();
extern int D_0044E1C0_5E58[] __asm__("D_0044E1C0");
extern int D_005157B0_5E58[] __asm__("D_005157B0");

int func_003C5E58(int arg0, int *arg1, int *arg2) {
    *arg1 = 0;
    *arg2 = 0;
    if (arg0 == 0) {
        if (func_003C5DF0() == 0) {
            return -4;
        }
        *arg1 = D_0044E1C0_5E58[0];
        *arg2 = D_005157B0_5E58[0];
    } else {
        return -5;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6040);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x178]; signed char n; char pad2[0x23]; int arr[1]; } S_6040;
extern S_6040 D_0050A8E8_6040 __asm__("D_0050A8E8");

void func_003C6040(int arg0) {
    int i;

    for (i = 0; i < D_0050A8E8_6040.n; i++) {
        if (D_0050A8E8_6040.arr[i] == arg0) {
            D_0050A8E8_6040.n--;
            while (i < D_0050A8E8_6040.n) {
                D_0050A8E8_6040.arr[i] = D_0050A8E8_6040.arr[i + 1];
                i++;
            }
            return;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int fA6E8_7138(void) __asm__("func_003BA6E8");
extern int fAA00_7138(int, int *) __asm__("func_003BAA00");
extern void f2440_7138(int) __asm__("func_003C2440");
extern char D_A8E8_7138[] __asm__("D_0050A8E8");

int func_003C7138(void) {
    int sp0;
    int r = fA6E8_7138();
    if (r >= 0) {
        char *g;
        sp0 = -1;
        g = D_A8E8_7138;
        while (fAA00_7138(r, &sp0) != 0) {
            char *e = *(char **)(g + 0x1F0) + sp0 * 0x8C;
            signed char t = *(signed char *)(e + 0x5E);
            if (t < 0) {
                f2440_7138(sp0);
            } else if (*(signed char *)(e + 0x5D) < t) {
                char *q;
                int d;
                *(signed char *)(e + 0x5D) = (unsigned char)*(signed char *)(e + 0x5E);
                q = (char *)(t * 8 + *(int *)(e + 0x6C));
                d = *(int *)q;
                *(int *)(e + 0x44) = d;
                *(int *)(e + 0x3C) = ((*(int *)(q + 4) << 16) - *(int *)(e + 0x40)) / d;
            }
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7218);
#ifdef SKIP_ASM
struct S7218 { short s0; char b2, b3, b4, b5, b6, b7; short s8; short sA; short sC[6]; int i18[6]; char pad[0x48 - 0x30]; int a48[4]; int a58[4]; };

int func_003C7218(struct S7218 *p) {
    int i;
    p->b3 = 0x7F;
    p->b4 = 0x40;
    p->s8 = 8;
    p->b2 = 0;
    p->s0 = 0;
    p->b5 = 0;
    p->b6 = 0;
    p->b7 = 0;
    for (i = 0; i < 4; i++) {
        p->a48[i] = 0;
        p->a58[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        p->sC[i] = 0;
        p->i18[i] = 0;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7388);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_7388 {
    unsigned char *p;
    unsigned cur;
    int r8;
    unsigned char *c;
    unsigned len;
};
extern unsigned func_003C72F8_7388(unsigned char *, unsigned) __asm__("func_003C72F8");

int func_003C7388(struct S_7388 *s) {
    unsigned char *p = s->p;
    unsigned char *q, *r;
    unsigned cur, len;
    if (*p == 0xFC) {
        do {
            q = p;
            s->p = q + 1;
            p = q + 1;
        } while (q[1] == 0xFC);
    }
    r = s->p;
    cur = *r;
    s->cur = cur;
    if (cur == 0xFF) {
        return 0;
    }
    s->p = r + 1;
    if (cur == 0xFD) {
        return 1;
    }
    if (cur == 0xFE) {
        return 1;
    }
    len = r[1];
    s->len = len;
    if (len == 0xFF) {
        s->len = func_003C72F8_7388(r + 2, 4);
        s->p = s->p + 4;
    }
    s->p = s->p + 1;
    s->c = s->p;
    if (s->len < 5) {
        s->r8 = func_003C72F8_7388(s->p, s->len);
    }
    s->p = s->p + s->len;
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C83A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_83A0 { char p0[5]; unsigned char cnt; char pad[0x1D6]; char *tbl; };
extern struct G_83A0 D_00515B40_83A0 __asm__("D_00515B40");

void func_003C83A0(int arg0) {
    int i;
    int j;
    char *e;
    float *a;
    float *b;
    i = 0;
    e = D_00515B40_83A0.tbl + arg0 * 0x60;
    *(int *)(e + 0x3C) = 0;
    a = (float *)(e + 0x34);
    e[1] = 0;
    do {
        i -= 1;
        a[0] = a[1];
        a++;
    } while (i >= 0);
    j = 0;
    if (D_00515B40_83A0.cnt != 0) {
        b = (float *)(e + 4);
        do {
            j += 1;
            b[0] = b[6];
            b++;
        } while (j < D_00515B40_83A0.cnt);
    }
    e[0] = 2;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8468);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Node_8468 {
    int pad0;
    void (*fn)(struct Node_8468 *);
    struct Node_8468 *next;
};
struct Ent_8468 {
    char c0;
    char pad1[3];
    float a[12];
    float b[2];
    float scale;
    struct Node_8468 *node;
};
struct G_8468 {
    char pad0[5];
    unsigned char n;
    char pad1[0xE8 - 6];
    float f[6];
    float g[1];
    char pad2[0x1DC - 0x104];
    char *tab;
};
extern struct G_8468 D_00515B40_8468 __asm__("D_00515B40");
extern void func_003B5E98_8468(struct Node_8468 *) __asm__("func_003B5E98");

void func_003C8468(int idx) {
    struct Ent_8468 *e = (struct Ent_8468 *)(D_00515B40_8468.tab + idx * 0x60);
    int i;
    struct Node_8468 *nd;
    struct Node_8468 *next;
    for (i = 0; i < 1; i++) {
        D_00515B40_8468.g[i] += e->b[i] * e->scale;
    }
    for (i = 0; i < D_00515B40_8468.n; i++) {
        D_00515B40_8468.f[i] += e->a[i] * e->scale;
    }
    do {
        if (e->node->fn != 0) {
            e->node->fn(e->node);
        }
        nd = e->node;
        next = nd->next;
        func_003B5E98_8468(nd);
        e->node = next;
    } while (next != 0);
    e->c0 = 0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C8968);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[5]; unsigned char cnt; char pad2[6]; unsigned short add; } H_8968;
extern H_8968 D_00515B40_8968[] __asm__("D_00515B40");
extern unsigned char D_00515B45_8968[] __asm__("D_00515B45");
extern void func_003C85D0_8968(int *, int) __asm__("func_003C85D0");

void func_003C8968(int *src, int len) {
    int buf[8];
    int i, n;

    for (i = 0; i < D_00515B45_8968[0]; i++) {
        buf[i] = src[i];
    }
    while (len > 0) {
        func_003C85D0_8968(buf, len < 0x201 ? len : 0x200);
        for (i = 0; i < D_00515B40_8968[0].cnt; i++) {
            buf[i] += D_00515B40_8968[0].add;
        }
        len -= 0x200;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *f_5C60_8B80(int) __asm__("func_003B5C60");
extern void f_5E98_8B80(void *) __asm__("func_003B5E98");
extern void f_8F48_8B80(void *, void *) __asm__("func_003C8F48");
extern void f_8FB0_8B80(void *, void *) __asm__("func_003C8FB0");
extern void f_B4A8_8B80(void *, int) __asm__("func_003CB4A8");
extern void f_B528_8B80(void *) __asm__("func_003CB528");
struct Q_8B80 { int w0; int w4; char pad[0x10]; short h18; };
struct E_8B80 { char pad[0x40]; char sub[0x14]; struct Q_8B80 *q; char pad2[8]; };
extern struct E_8B80 *D_00515D1C_8B80[] __asm__("D_00515D1C");

void func_003C8B80(int arg0, int arg1) {
    struct E_8B80 *p = D_00515D1C_8B80[0] + arg0;
    if (arg1 > 0) {
        if (p->q == 0) {
            struct Q_8B80 *n = (struct Q_8B80 *)f_5C60_8B80(0x50);
            p->q = n;
            n->w4 = 0;
            p->q->h18 = 0x78;
            f_B528_8B80(p->q);
            f_8F48_8B80(p->sub, p->q);
        }
        f_B4A8_8B80(p->q, arg1);
        return;
    }
    if (p->q != 0) {
        f_8FB0_8B80(p->sub, p->q);
        f_5E98_8B80(p->q);
        p->q = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8C30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_00515D1C_8C30[] __asm__("D_00515D1C");
extern unsigned short D_0050A912_8C30[] __asm__("D_0050A912");
extern void *func_003B5C60_8C30(int) __asm__("func_003B5C60");
extern void func_003B5E98_8C30(void *) __asm__("func_003B5E98");
extern void func_003CA990_8C30(void *) __asm__("func_003CA990");
extern void func_003C8F48_8C30(void *, void *) __asm__("func_003C8F48");
extern void func_003C8FB0_8C30(void *, void *) __asm__("func_003C8FB0");
extern void func_003CA9D8_8C30(void *, int *) __asm__("func_003CA9D8");

void func_003C8C30(int arg0, int arg1) {
    int sp[2];
    char *e;
    char *buf;

    e = D_00515D1C_8C30[0] + arg0 * 0x60;
    if (arg1 > 0) {
        if (*(char**)(e + 0x5C) == 0) {
            char *t2 = func_003B5C60_8C30(0x120);
            *(char**)(e + 0x5C) = t2;
            *(int*)(t2 + 4) = 0;
            *(short*)(*(char**)(e + 0x5C) + 0x18) = 0x50;
            *(signed char*)(*(char**)(e + 0x5C) + 0x1A) = 0;
            func_003CA990_8C30(*(char**)(e + 0x5C));
            func_003C8F48_8C30(e + 0x40, *(char**)(e + 0x5C));
        }
        sp[0] = arg1 << 8;
        sp[1] = D_0050A912_8C30[0] << 8;
        func_003CA9D8_8C30(*(char**)(e + 0x5C), sp);
        return;
    }
    buf = *(char**)(e + 0x5C);
    if (buf != 0) {
        func_003C8FB0_8C30(e + 0x40, buf);
        func_003B5E98_8C30(*(char**)(e + 0x5C));
        *(char**)(e + 0x5C) = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8D00);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8E28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *func_003B5C60_8E28(int) __asm__("func_003B5C60");
extern void func_003C8F48_8E28(int, void *) __asm__("func_003C8F48");
extern int D_00515D1C_8E28[] __asm__("D_00515D1C");

void func_003C8E28(int arg0, int arg1, void *arg2) {
    int temp_18;
    int temp_7;
    void *temp_2;

    temp_7 = arg0 * 0x60;
    temp_18 = D_00515D1C_8E28[0] + temp_7;
    temp_2 = func_003B5C60_8E28((*(int *)((char*)(arg2) + (4))));
    (*(int (**)(void *, int, int))((char*)(arg2) + (0xC)))(temp_2, (*(int *)((char*)(arg2) + (0))), arg1);
    (*(unsigned short *)((char*)(temp_2) + (0x18))) = (unsigned short) (*(unsigned short *)((char*)(arg2) + (8)));
    (*(int *)((char*)(temp_2) + (0))) = (int) (*(int *)((char*)(arg2) + (0x10)));
    (*(int *)((char*)(temp_2) + (4))) = (int) (*(int *)((char*)(arg2) + (0x14)));
    (*(signed char *)((char*)(temp_2) + (0x1A))) = 0;
    func_003C8F48_8E28(temp_18 + 0x40, temp_2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8EC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_00515D1C_8EC0[] __asm__("D_00515D1C");
extern void func_003C8FB0_8EC0(char *, char *) __asm__("func_003C8FB0");
extern void func_003B5E98_8EC0(char *) __asm__("func_003B5E98");

void func_003C8EC0(int arg0, int arg1) {
    char *rec = D_00515D1C_8EC0[0] + arg0 * 0x60;
    char *node = *(char **)(rec + 0x40);
    void (*fn)(char *);
    do {
        if (*(unsigned short *)(node + 0x18) == arg1) {
            fn = *(void (**)(char *))(node + 4);
            if (fn != 0) {
                fn(node);
            }
            func_003C8FB0_8EC0(rec + 0x40, node);
            func_003B5E98_8EC0(node);
            return;
        }
        node = *(char **)(node + 8);
    } while (node != 0);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C90C0);
#ifdef SKIP_ASM
int func_003C90C0(char *a, char *b, int s, int t) {
    if (s == 1 && t == 1 && *(int*)(b + 8) == 0) {
        *(int*)(a + 0x10) = (int)b;
        *(int*)(b + 8) = (int)a;
        *(char*)(b + 0x1A) = t;
        return 0;
    }
    if (s == 1 && t == 2 && *(int*)(b + 0xC) == 0) {
        *(int*)(a + 0x10) = (int)b;
        *(int*)(b + 0xC) = (int)a;
        *(char*)(b + 0x1A) = s;
        return 0;
    }
    if (s == 2 && t == 1 && *(int*)(b + 8) == 0) {
        *(int*)(a + 0x14) = (int)b;
        *(int*)(b + 8) = (int)a;
        *(char*)(b + 0x1A) = s;
        return 0;
    }
    if (s == 2 && t == 2 && *(int*)(b + 0xC) == 0) {
        *(int*)(a + 0x14) = (int)b;
        *(int*)(b + 0xC) = (int)a;
        *(char*)(b + 0x1A) = t;
        return 0;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9188);
#ifdef SKIP_ASM
int func_003C9188(int *a, int *b, int x, int y) {
    if (x == 1 && y == 1 && a[4] == (int)b && b[2] == (int)a) {
        a[4] = 0;
        b[2] = 0;
        return 0;
    }
    if (x == 1 && y == 2 && a[4] == (int)b && b[3] == (int)a) {
        a[4] = 0;
        b[3] = 0;
        return 0;
    }
    if (x == 2 && y == 1 && a[5] == (int)b && b[2] == (int)a) {
        a[5] = 0;
        b[2] = 0;
        return 0;
    }
    if (x == 2 && y == 2 && a[5] == (int)b && b[3] == (int)a) {
        a[5] = 0;
        b[3] = 0;
        return 0;
    }
    return -1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9360);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003CDE68_9360(int, int **, int) __asm__("func_003CDE68");
extern void func_00416210_9360(int *, int, int) __asm__("func_00416210");

int func_003C9360(void *arg0, int arg1, int arg2, int *arg3) {
    int *sp0;
    int temp_2;

    sp0 = arg3;
    if ((unsigned int) (*(unsigned int *)((char*)(arg0) + (0x20))) >= (unsigned int) (*(unsigned int *)((char*)(arg0) + (0x1C)))) {
        return -1;
    }
    temp_2 = func_003CDE68_9360((*(int *)((char*)(arg0) + (0x28))), &sp0, arg1);
    (*(unsigned int *)((char*)(arg0) + (0x20))) = (unsigned int) ((*(unsigned int *)((char*)(arg0) + (0x20))) + temp_2);
    sp0 = sp0 + temp_2;
    if (temp_2 < arg1) {
        func_00416210_9360(sp0, 0, (arg1 -= temp_2) * 4);
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9520);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a, b, c; } Q_9520;
extern int func_003CDE68_9520(int, int *, int) __asm__("func_003CDE68");
extern void func_003CE0B8_9520(int, void *, int) __asm__("func_003CE0B8");
extern void func_003CDDE8_9520(int, int, int, int) __asm__("func_003CDDE8");

int func_003C9520(char *arg0, int arg1, int arg2, int arg3) {
    Q_9520 q;
    int sp10;
    int n, t;

    sp10 = arg3;
    if (arg1 > 0) {
        do {
            n = func_003CDE68_9520(*(int*)(arg0 + 0x1C), &sp10, arg1);
            arg1 -= n;
            sp10 += n * 4;
            *(int*)(arg0 + 0x20) = *(int*)(arg0 + 0x20) + n;
            if (n < arg1) {
                *(int*)(arg0 + 0x20) = *(int*)(arg0 + 0x24);
                q.b = 0;
                q.a = 0;
                q.c = 1;
                func_003CE0B8_9520(*(int*)(arg0 + 0x1C), &q, 0);
                t = (*(int*)(arg0 + 0x28) - *(int*)(arg0 + 0x24)) + 1;
                func_003CDDE8_9520(*(int*)(arg0 + 0x1C), *(int*)(arg0 + 0x2C), t * 4, t);
            }
        } while (arg1 > 0);
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9A00);
#ifdef SKIP_ASM
struct R_9A00 { char pad[0x1C]; short *data; unsigned int pos; unsigned int cap; int flag; };

int func_003C9A00(struct R_9A00 *r, unsigned int n, int arg2, int *buf) {
    unsigned int pos = r->pos;
    unsigned int cap = r->cap;
    if (pos >= cap) return -1;
    r->pos = pos + n;
    if (pos + n < cap) {
        if (r->flag) {
            func_003C9E50(n, r->data + pos, buf);
        }
    } else {
        unsigned int first = cap - pos;
        if (r->flag) {
            func_003C9E50(first, r->data + pos, buf);
        }
        func_00416210(buf + first, 0, (n - first) * 4);
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9B10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_9E50_9B10(int, void *, void *) __asm__("func_003C9E50");
struct R_9B10 { char pad[0x1C]; short *buf; unsigned cur; unsigned start; unsigned end; int en; };

int func_003C9B10(struct R_9B10 *self, int n, int unused, int *src) {
    int m;
    while (n > 0) {
        m = self->end - self->cur + 1;
        if (n < m)
            m = n;
        if (self->en != 0)
            f_9E50_9B10(m, self->buf + self->cur, src);
        src += m;
        self->cur = self->cur + m;
        n -= m;
        if (self->end < self->cur)
            self->cur = self->start;
    }
    return 1;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C9C08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003C6DE0_9C08(int, unsigned char, int *, int *) __asm__("func_003C6DE0");
extern void func_003C7010_9C08(int, unsigned char, int) __asm__("func_003C7010");
extern void func_003C9E50_9C08(int, int, int *) __asm__("func_003C9E50");
extern void func_00416210_9C08(int *, int, int) __asm__("func_00416210");

int func_003C9C08_9C08(char *o, int n, int unused, int *buf) __asm__("func_003C9C08");
int func_003C9C08_9C08(char *o, int n, int unused, int *buf) {
    int sp0, sp4;
    int t;
    if (*(int *)(o + 0x2C) != 0) {
        func_003C7010_9C08(*(int *)(o + 0x28), *(unsigned char *)(o + 0x31), *(int *)(o + 0x2C));
        *(int *)(o + 0x2C) = 0;
    }
    while (n > 0) {
        if (*(int *)(o + 0x24) >= *(int *)(o + 0x20)) {
            *(int *)(o + 0x1C) = func_003C6DE0_9C08(*(int *)(o + 0x28), *(unsigned char *)(o + 0x31), &sp0, &sp4);
            if (*(int *)(o + 0x1C) == 0) {
                if (*(int *)(o + 0x2C) != 0) {
                    func_00416210_9C08(buf, 0, n * 4);
                }
                return *(int *)(o + 0x2C);
            }
            *(int *)(o + 0x24) = 0;
            *(int *)(o + 0x20) = sp0;
        }
        t = *(int *)(o + 0x20) - *(int *)(o + 0x24);
        if (n < t) t = n;
        if (*(unsigned char *)(o + 0x30) != 0) {
            func_003C9E50_9C08(t, *(int *)(o + 0x1C) + *(int *)(o + 0x24) * 2, buf);
        }
        buf += t;
        n -= t;
        *(int *)(o + 0x24) += t;
        *(int *)(o + 0x2C) += t;
    }
    return *(int *)(o + 0x2C);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C9D28);
#ifdef SKIP_ASM
extern int func_003CE0E0(int);
extern signed char func_003CE118(int);
extern int func_003CE378(int);
extern void func_003C9C08(void);

void func_003C9D28(void *arg0, void *arg1) {
    (*(int **)((char*)(arg0) + (0))) = (int*)&func_003C9C08;
    (*(int *)((char*)(arg0) + (0x28))) = func_003CE378(func_003CE0E0((*(int *)((char*)(arg1) + (0x1C)))));
    (*(signed char *)((char*)(arg0) + (0x31))) = func_003CE118((*(int *)((char*)(arg1) + (0x1C))));
    (*(int *)((char*)(arg0) + (0x20))) = -1;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    (*(int *)((char*)(arg0) + (0x24))) = 0;
    (*(unsigned char *)((char*)(arg0) + (0x30))) = (*(unsigned char *)((char*)(arg1) + (0x18)));
    (*(int *)((char*)(arg0) + (0x2C))) = 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C9DA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003C9E50);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9F58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003CCF90_9F58(int, char **, int) __asm__("func_003CCF90");
extern void func_00416210_9F58(char *, int, int) __asm__("func_00416210");

int func_003C9F58(void *arg0, int arg1, int arg2, int arg3) {
    char *p;
    char *q;
    int n;
    p = (char *)arg3;
    if (*(int *)((char *)arg0 + 0x28) >= *(int *)((char *)arg0 + 0x24)) {
        return -1;
    }
    n = func_003CCF90_9F58(*(int *)((char *)arg0 + 0x1C), &p, arg1);
    *(int *)((char *)arg0 + 0x28) += n;
    p += n * 4;
    q = p;
    if (n < arg1) {
        arg1 = arg1 - n;
        n += arg1;
        func_00416210_9F58(q, 0, arg1 * 4);
    }
    return n;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA028);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char func_003C9F58_A028[] __asm__("func_003C9F58");
extern char func_003C9FF8_A028[] __asm__("func_003C9FF8");
extern char func_003CA000_A028[] __asm__("func_003CA000");
extern int func_003CCEF0_A028(int) __asm__("func_003CCEF0");
extern int func_003CCF40_A028(int) __asm__("func_003CCF40");
extern void func_003CCF60_A028(int, int, int, int) __asm__("func_003CCF60");

void func_003CA028(char *arg0, char *arg1) {
    int n;
    *(char **)(arg0 + 0) = func_003C9F58_A028;
    *(char **)(arg0 + 4) = func_003CA000_A028;
    *(int *)(arg0 + 0x20) = *(int *)(arg1 + 0);
    *(int *)(arg0 + 0x24) = *(int *)(arg1 + 0xC);
    *(int *)(arg0 + 0x28) = 0;
    *(int *)(arg0 + 0x1C) = func_003CCF40_A028(func_003CCEF0_A028(0xA8));
    *(char **)(arg1 + 0x28) = func_003C9FF8_A028;
    n = *(int *)(arg0 + 0x24);
    func_003CCF60_A028(*(int *)(arg0 + 0x1C), *(int *)(arg0 + 0x20), n * 4, n);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA388);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char func_003CA0B8_A388[] __asm__("func_003CA0B8");
extern char func_003CA358_A388[] __asm__("func_003CA358");
extern char func_003CA360_A388[] __asm__("func_003CA360");
extern int func_003CCEF0_A388(int) __asm__("func_003CCEF0");
extern int func_003CCF40_A388(int) __asm__("func_003CCF40");
extern void func_003CCF60_A388(int, int, int, int) __asm__("func_003CCF60");

void func_003CA388(int *self, int *arg1) {
    int n;

    self[0] = (int)func_003CA0B8_A388;
    self[1] = (int)func_003CA360_A388;
    self[11] = arg1[1];
    self[8] = 0;
    self[12] = arg1[9];
    self[9] = arg1[4];
    self[10] = arg1[5];
    self[7] = func_003CCF40_A388(func_003CCEF0_A388(0xA8));
    arg1[10] = (int)func_003CA358_A388;
    if (self[12] < 3) {
        self[13] = arg1[0];
        self[14] = arg1[3];
        self[15] = 0;
        self[16] = 0;
        self[17] = arg1[4];
        self[18] = arg1[5];
    } else {
        n = self[9];
        func_003CCF60_A388(self[7], arg1[0], n * 4, n);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CA610);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003CCEF0_A610(int) __asm__("func_003CCEF0");
extern int func_003CCF40_A610(int) __asm__("func_003CCF40");
extern int func_003CE0E0_A610(int) __asm__("func_003CE0E0");
extern signed char func_003CE118_A610(int) __asm__("func_003CE118");
extern int func_003CE378_A610(int) __asm__("func_003CE378");
extern int func_003CA460_A610[] __asm__("func_003CA460");
extern int func_003CA5E8_A610[] __asm__("func_003CA5E8");

void func_003CA610(void *arg0, void *arg1) {
    (*(int **)((char*)(arg0) + (0))) = func_003CA460_A610;
    (*(int **)((char*)(arg0) + (4))) = func_003CA5E8_A610;
    (*(int *)((char*)(arg0) + (0x30))) = func_003CE378_A610(func_003CE0E0_A610((*(int *)((char*)(arg1) + (0x1C)))));
    (*(signed char *)((char*)(arg0) + (0x38))) = func_003CE118_A610((*(int *)((char*)(arg1) + (0x1C))));
    (*(int *)((char*)(arg0) + (0x2C))) = (int) (*(int *)((char*)(arg1) + (0x24)));
    (*(int *)((char*)(arg0) + (0x20))) = 0;
    (*(int *)((char*)(arg0) + (0x24))) = (int) (*(int *)((char*)(arg1) + (0xC)));
    (*(int *)((char*)(arg0) + (0x28))) = 0;
    (*(int *)((char*)(arg0) + (0x34))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = func_003CCF40_A610(func_003CCEF0_A610(0xA8));
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA7B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef int (*vfn_A7B8)(void *, int, float *, float *, unsigned char);
int func_003CA7B8_A7B8(char *, int, float *, float *) __asm__("func_003CA7B8");
int func_003CA7B8_A7B8(char *self, int n, float *in, float *out) {
    void **o = *(void ***)(self + 8);
    int r;
    float *st;
    int i;
    if (o == 0 || (r = ((vfn_A7B8)(*(void **)o))(o, n, out, in, *(unsigned char *)(self + 0x1A))) > 0) {
        st = (float *)(self + 0x1C);
        for (i = 0; i < n; i++) {
            st[0] = st[0] * st[1] + st[2] * in[i];
            out[i] = st[0];
        }
        r = n;
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CA900);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003CEBE8_A900(void *, int, int, int) __asm__("func_003CEBE8");

int func_003CA900(char *p, int arg1, int arg2, int arg3) {
    int (**obj)(void *, int, int, int, int);
    int n;
    obj = *(int (***)(void *, int, int, int, int))(p + 8);
    if (obj != 0) {
        n = (*obj)(obj, arg1, arg3, arg2, *(unsigned char *)(p + 0x1A));
        if (n <= 0) {
            return n;
        }
    } else {
        n = arg1;
    }
    func_003CEBE8_A900(p + 0x20, n, arg2, arg3);
    return n;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CADF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C51E8_ADF0(int, void *, int) __asm__("func_003C51E8");

int func_003CADF0(void *arg0, int *arg1, int *arg2) {
    int temp_16;
    int temp_16_2;
    int temp_20;
    int temp_3;

    temp_16_2 = (*(int *)((char*)(arg0) + (0x38)));
    temp_3 = *arg1;
    temp_16 = temp_16_2;
    if (temp_16_2 >= temp_3) temp_16 = temp_3;
    temp_20 = temp_16 * 4;
    func_003C51E8_ADF0(*arg2, arg0 + (((*(int *)((char*)(arg0) + (0x3C))) * 4) + 0x838), temp_20);
    (*(int *)((char*)(arg0) + (0x3C))) = (int) ((*(int *)((char*)(arg0) + (0x3C))) + temp_16);
    (*(int *)((char*)(arg0) + (0x38))) = (int) ((*(int *)((char*)(arg0) + (0x38))) - temp_16);
    *arg1 -= temp_16;
    *arg2 += temp_20;
    return temp_16;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CAEA0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CAFD0);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB110);

INCLUDE_ASM("ealib/seg_2B4578", func_003CB1A8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CB280);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003C72F8(int, int);
extern int func_003CE0E0(int);
extern int func_003CE378(int);
extern char func_003CB1A8_B280[] __asm__("func_003CB1A8");

int func_003CB280(void *arg0, void *arg1, int arg2) {
    (*(char **)((char*)(arg0) + (0))) = func_003CB1A8_B280;
    (*(int *)((char*)(arg0) + (4))) = 0;
    if (arg2 >= 0) {
        (*(int *)((char*)(arg0) + (0x20))) = func_003CE378(func_003CE0E0(arg2));
    } else {
        (*(int *)((char*)(arg0) + (0x20))) = -1;
    }
    (*(int *)((char*)(arg0) + (0x1C))) = (int) (arg1 + 6);
    (*(float *)((char*)(arg0) + (0x24))) = 1.0f;
    (*(int *)((char*)(arg0) + (0x28))) = 0;
    (*(int *)((char*)(arg0) + (0x2C))) = (int) ((*(unsigned char *)((char*)(arg1) + (1))) * 2);
    (*(int *)((char*)(arg0) + (0x30))) = func_003C72F8(arg1 + 2, 4);
    (*(int *)((char*)(arg0) + (0x34))) = 0;
    (*(int *)((char*)(arg0) + (0x38))) = 0;
    (*(int *)((char*)(arg0) + (0x3C))) = 0;
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CC938);
#ifdef SKIP_ASM
typedef struct { int pad0; float x; float y; unsigned char *src; float *dst; } S_C938;
typedef union { short s; unsigned char c[2]; } U_C938;

void func_003CC938(S_C938 *s) {
    unsigned char b[2];
    unsigned int i;

    if (*s->src == 0xEE) {
        s->src++;
        b[0] = s->src[1];
        b[1] = s->src[0];
        s->src += 2;
        s->x = (float)*(short*)b;
        b[0] = s->src[1];
        b[1] = s->src[0];
        s->src += 2;
        s->y = (float)*(short*)b;
        for (i = 0; i < 0x1C; i++) {
            b[0] = s->src[1];
            b[1] = s->src[0];
            *s->dst = (float)*(short*)b;
            s->dst++;
            s->src += 2;
        }
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CCF90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_CF90 { int f0; int pending; int remaining; int fC; float *rd; char pad[0x10]; float buf[28]; int n; int pad2[3]; float *out; };
extern void func_003CCA08_CF90(int *) __asm__("func_003CCA08");

int func_003CCF90(struct S_CF90 *s, float **pout, int n) {
    int done = 0;
    int m;
    int k;
    int i;
    int rem;
    float *dst;

    s->out = *pout;
    if (s->remaining == 0) {
        return 0;
    }
    m = s->remaining;
    if (n < m) {
        m = n;
    }
    if (s->pending != 0) {
        if (m < s->pending) {
            k = m;
        } else {
            k = s->pending;
        }
        for (i = 0; i < k; i++) {
            *s->out = *s->rd;
            s->out++;
            s->rd++;
        }
        m -= k;
        s->remaining -= k;
        s->pending -= k;
        done = k;
    }
    i = m / 28 * 28;
    rem = m - i;
    s->n = i;
    if (i > 0) {
        done += i;
        func_003CCA08_CF90(&s->n);
        s->remaining -= i;
    }
    if (rem > 0) {
        dst = s->out;
        s->n = rem;
        s->out = s->buf;
        func_003CCA08_CF90(&s->n);
        s->remaining -= rem;
        s->rd = s->out + s->n;
        s->pending = -s->n;
        s->out -= 28;
        for (i = 0; i < rem; i++) {
            dst[i] = s->out[i];
        }
        done += rem;
    }
    if (s->remaining <= 0) {
        s->pending = 0;
    }
    return done;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CD138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { float x; float y; } V2_D138;
V2_D138 func_003CD138_D138(char *p) __asm__("func_003CD138");

V2_D138 func_003CD138_D138(char *p) {
    V2_D138 v;
    V2_D138 w;
    v.x = *(float *)(p + 0x98);
    v.y = *(float *)(p + 0x9C);
    w = v;
    return w;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CDDE8);
#ifdef SKIP_ASM
extern void func_003CD6F0(char *, unsigned char *, char *, int);

int func_003CDDE8(char *arg0, unsigned char *arg1, int arg2, int arg3) {
    int temp_8;

    if (arg1 == 0) return -1;
    if (*(int *)(arg0 + 0xD54) != 0) return -1;
    temp_8 = *(int *)(arg0 + 0xD64);
    *(int *)(arg0 + 0xD54) = arg3;
    *(int *)(arg0 + 0xD58) = arg2;
    *(int *)(arg0 + 0xD44) = 0;
    *(unsigned char **)(arg0 + 0xD48) = arg1;
    if (temp_8 == 1) {
        if (*arg1 == 0xEE) {
            *(int *)(arg0 + 0xD60) = temp_8;
        } else {
            *(int *)(arg0 + 0xD60) = 0;
        }
        func_003CD6F0(arg0, arg1 + 1, arg0, *(int *)(arg0 + 0xD5C));
    } else {
        func_003CD6F0(arg0, arg1, arg0, *(int *)(arg0 + 0xD5C));
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE118);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050A8E8_E118[] __asm__("D_0050A8E8");

struct E_E118 { char pad[0x28]; short f28; char pad2[0x8C - 0x2A]; };
struct R_E118 { struct E_E118 e[1]; };

int func_003CE118_impl(int arg0) __asm__("func_003CE118");
int func_003CE118_impl(int arg0) {
    short *var_3;
    int temp_2;
    int temp_4;
    int var_5;
    char *b = D_0050A8E8_E118;

    temp_4 = arg0 + *(unsigned char *)(b + 0x30) + *(unsigned char *)(b + 0x2F);
    temp_2 = ((struct R_E118 *)*(char **)(b + 0x1F0))->e[temp_4].f28;
    if (temp_2 == -1) {
        return 0;
    }
    var_3 = (short *)(temp_2 * 0x8C + *(int *)(b + 0x1F0) + 6);
    for (var_5 = 1; var_5 < 6; var_5++, var_3++) {
        if (*var_3 == temp_4) {
            return var_5;
        }
    }
    return 0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CE1B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[4];
    short h[8];
    int f14;
    char pad18[0x23 - 0x18];
    unsigned char f23;
    unsigned short f24;
    char pad26[0x8C - 0x26];
} E_E1B0;
typedef struct {
    char pad0[0x2F];
    unsigned char f2F;
    unsigned char f30;
    char pad31[0x1F0 - 0x31];
    E_E1B0 *ents;
} G_E1B0;
typedef struct { char pad0[3]; unsigned char b3; } A6_E1B0;
typedef struct { char pad0[7]; unsigned char b7; char pad8[0x18 - 8]; int arr[8]; } A7_E1B0;
extern G_E1B0 D_0050A8E8_E1B0 __asm__("D_0050A8E8");
extern void func_003C8158_E1B0(int, int, int, int, unsigned int, int, int, int, int, int, int, int, int, int) __asm__("func_003C8158");
extern void func_003C25E0_E1B0(int) __asm__("func_003C25E0");
extern void func_003C38E0_E1B0(int) __asm__("func_003C38E0");
extern void func_003C27E0_E1B0(int, int) __asm__("func_003C27E0");
extern void func_003C3010_E1B0(int, int) __asm__("func_003C3010");
extern void func_003C30C8_E1B0(int, int) __asm__("func_003C30C8");
extern void func_003C3178_E1B0(int, int) __asm__("func_003C3178");
extern void func_003C3250_E1B0(int, int) __asm__("func_003C3250");
extern void func_003C83A0_E1B0(int) __asm__("func_003C83A0");

int func_003CE1B0(int arg0, int idx, int arg2, int arg3, int arg4, int arg5, A6_E1B0 *arg6, A7_E1B0 *arg7) {
    E_E1B0 *e;
    int i;

    e = &D_0050A8E8_E1B0.ents[idx];
    if (!(e->f24 & 8)) {
    if (e->f24 & 4) {
    for (i = 0; i < e->f23; i++) {
        func_003C8158_E1B0(e->h[i] - (D_0050A8E8_E1B0.f30 + D_0050A8E8_E1B0.f2F), arg6->b3, 0, 0, 0xFFFFFFFF,
                           arg7->arr[i], e->f23, e->f14, -1, -1, 0, 0, arg7->b7, i);
    }
    func_003C25E0_E1B0(e->h[0]);
    func_003C38E0_E1B0(e->h[0]);
    func_003C27E0_E1B0(e->h[0], 0);
    func_003C3010_E1B0(e->h[0], arg2);
    func_003C30C8_E1B0(e->h[0], arg3);
    func_003C3178_E1B0(e->h[0], arg4);
    func_003C3250_E1B0(e->h[0], arg5);
    for (i = 0; i < e->f23; i++) {
        func_003C83A0_E1B0(e->h[i] - (D_0050A8E8_E1B0.f30 + D_0050A8E8_E1B0.f2F));
    }
    }
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CEE08);
#ifdef SKIP_ASM
extern void func_003CEBE8(char *, int, int, int);
int func_003CEE08(char *arg0, int arg1, int arg2, int arg3) {
    int **obj = *(int ***)(arg0 + 8);
    int r;
    if (obj == 0 || (r = ((int (*)(int **, int, int, int, unsigned char))(*obj))(obj, arg1, arg3, arg2, *(unsigned char *)(arg0 + 0x1A)), r > 0)) {
        func_003CEBE8(arg0 + 0x20, arg1, arg2, arg3);
        r = arg1;
    }
    return r;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF408);
#ifdef SKIP_ASM
extern void func_003CF300(void *, int, int, int);

int func_003CF408(char *p, int arg1, int arg2, int arg3) {
    int (**obj)(void *, int, int, int, int);
    int n;
    obj = *(int (***)(void *, int, int, int, int))(p + 8);
    if (obj != 0) {
        n = (*obj)(obj, arg1, arg3, arg2, *(unsigned char *)(p + 0x1A));
        if (n <= 0) {
            return n;
        }
    } else {
        n = arg1;
    }
    func_003CF300(p, n, arg2, arg3);
    return n;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CF5D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B5C60_F5D0(int) __asm__("func_003B5C60");
extern void func_003B5E98_F5D0(int) __asm__("func_003B5E98");
extern void func_003C51E8_F5D0(char *, char *, int) __asm__("func_003C51E8");

int func_003CF5D0(char *self, int n, char *a2, char *a3) {
    int bytes, p, r;
    int *obj;
    if (*(short *)(self + 0x24) != 0) {
        *(short *)(self + 0x24) = 0;
        *(short *)(self + 0x26) = 1;
    }
    bytes = n * 4;
    if (*(int *)(self + 0x20) < n) {
        p = *(int *)(self + 0x1C);
        if (p != 0) {
            func_003B5E98_F5D0(p);
        }
        *(int *)(self + 0x1C) = func_003B5C60_F5D0(bytes);
        *(int *)(self + 0x20) = n;
    }
    if (*(short *)(self + 0x26) != 0) {
        int (**vt)(void *, int, char *, char *, int) = *(int (***)(void *, int, char *, char *, int))(self + 8);
        r = (*vt)(vt, n, a3, a2, 1);
        if (r <= 0) {
            return r;
        }
        func_003C51E8_F5D0(a3, a2, bytes);
        func_003C51E8_F5D0(*(char **)(self + 0x1C), a2, bytes);
        *(short *)(self + 0x26) = 0;
    } else {
        func_003C51E8_F5D0(a3, *(char **)(self + 0x1C), bytes);
        *(short *)(self + 0x24) = 1;
    }
    return n;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CFB98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003CFB68_FB98(int) __asm__("func_003CFB68");

int func_003CFB98(int arg0, int arg1) {
    int lim;
    int t;
    int n;
    int d;

    t = (int) (arg1 * arg0) / 1000;
    n = (t <= 2) ? 3 : t;
    for (;;) {
        lim = func_003CFB68_FB98(n) + 1;
        for (d = 2; lim >= d; d++) {
            if ((n % d) == 0) {
                n += 1;
                break;
            }
            if (d == lim) {
                return n;
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0CC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D32B8_0CC8(char *, int *) __asm__("func_003D32B8");
extern int D_0044F438_0CC8[] __asm__("D_0044F438");
extern char *D_0044F420_0CC8[] __asm__("D_0044F420");
int func_003D0CC8_0CC8(char *arg0) __asm__("func_003D0CC8");

int func_003D0CC8_0CC8(char *arg0) {
    int buf[8];
    unsigned int t;
    int r;
    if (arg0 == 0) {
        return 1;
    }
    {
        t = *(unsigned int *)(arg0 + 0x4C);
        *(int *)(arg0 + 0x100) = 2;
        if (t != 0) {
            if ((unsigned int)D_0044F438_0CC8[0] >= t) {
                *(int *)(arg0 + 0x100) = 4;
            } else {
                *(int *)(arg0 + 0x100) = 3;
            }
            goto done;
        }
        if (*(short *)(arg0 + 0x36) >= 0) {
            *(int *)(arg0 + 0x100) = 7;
            if (func_003D32B8_0CC8(arg0, buf) >= 0
                && (unsigned int)(*(int *)(arg0 + 0x24) + *(int *)(D_0044F420_0CC8[0] + 0xC)) >= (unsigned int)buf[6]) {
                *(int *)(arg0 + 0x100) = 5;
            }
        }
done:
        r = *(int *)(arg0 + 0x100);
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1F80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_1F80 { char pad[0x30]; unsigned int m_a; int m_b; char pad2[0xDC - 0x38]; char *ptrs[16]; char pad3[0x91C - 0x11C]; int count; char pad4[0x928 - 0x920]; };
extern int f2AF370_1F80(void) __asm__("func_002AF370");
extern struct P_1F80 D_7610_1F80[4] __asm__("D_00517610");

int func_003D1F80(unsigned int a0, char *a1) {
    unsigned int m = a0 & 0xFF000000;
    int i, ret = -8;
    for (i = 0; i < 4; i++) {
        if (D_7610_1F80[i].m_b != 0 && (D_7610_1F80[i].m_a & m) == m) {
            int c = D_7610_1F80[i].count;
            if (c >= 0x10) return -0xD;
            ret = 0;
            {
                int t = f2AF370_1F80();
                *(int *)(a1 + 8) = t;
                *(int *)a1 = t;
            }
            D_7610_1F80[i].ptrs[c] = a1;
            c++;
            D_7610_1F80[i].count = c;
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2068);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F420_2068[] __asm__("D_0044F420");
extern int D_0044F43C_2068[] __asm__("D_0044F43C");
extern char D_00495B48_2068[] __asm__("D_00495B48");
extern void func_0041605C_2068(unsigned int, void *, int) __asm__("func_0041605C");

unsigned int func_003D2068(void *arg0) {
    int temp_9;
    unsigned int *p;
    unsigned int temp_3;
    unsigned int temp_6;
    unsigned int cur;
    unsigned int i;

    temp_6 = D_0044F420_2068[0] + 0x91C;
    cur = D_0044F420_2068[0] + 0x11C;
    p = (unsigned int*)(D_0044F420_2068[0] + 0xDC);
    temp_9 = (((*(unsigned short*)((char*)arg0 + 0xE)) & 0x3F) * 0x10) + 0x10;
    for (i = 0; i < 0x10; i++, p++) {
        temp_3 = *p;
        if (temp_3 >= cur) {
            if (temp_3 < temp_6) {
                int sz = (((*(unsigned short*)((char*)temp_3 + 0xE)) & 0x3F) * 0x10) + 0x10;
                cur += sz;
            }
        }
    }
    if (cur + temp_9 < temp_6) {
        func_0041605C_2068(cur, arg0, temp_9);
        return cur;
    }
    if (D_0044F43C_2068[0] & 0x120) {
        func_003D1658(D_00495B48_2068, temp_9, temp_6 - cur);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct I2138 { int w0; int w4; unsigned int w8; int wC; };
struct E2138 { char pad[0xC]; unsigned char b; char padd; unsigned short h; struct I2138 items[1]; };
extern struct E2138 **D_0044F420_2138[] __asm__("D_0044F420");

int func_003D2138(char *arg0, int arg1) {
    int i;
    unsigned int j;
    struct E2138 *e;
    struct I2138 *it;
    for (i = 0; i < 16; i++) {
        e = *(struct E2138 **)((char *)D_0044F420_2138[0] + 0xDC + i * 4);
        if (e != 0 && e->b == *(unsigned char *)(arg0 + 0xC)) {
            for (j = 0; j < (e->h & 0x3F); j++) {
                it = &e->items[j];
                if (((unsigned char *)it)[9] == arg1) {
                    return (it->w8 >> 26) & 1;
                }
            }
        }
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2C20);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D5450_2C20(int) __asm__("func_003D5450");
extern int func_003D4BB0_2C20(int) __asm__("func_003D4BB0");
extern double func_00413AF8_2C20(float) __asm__("func_00413AF8");
extern double func_00411F18_2C20(double, double) __asm__("func_00411F18");
extern int func_004126A0_2C20(double) __asm__("func_004126A0");
extern char *D_0044F420_2C20[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_2C20[] __asm__("D_0044F42C");
extern int D_0044F430_2C20[] __asm__("D_0044F430");

static inline int bitclear_2C20(int x) {
    return !(x & 1);
}

int func_003D2C20(int m, int arg1) {
    float ratio, max;
    unsigned int i;
    char *g;
    int *base;
    int *tab;
    int *e;
    int r;

    ratio = 0.0f;
    max = 0.0f;
    if (D_0044F42C_2C20[0] == 0 || D_0044F430_2C20[0] != 0 || func_003D5450_2C20(-1) == 0) {
        return -0x12;
    }
    if (func_003D4BB0_2C20(m & 0xFF000000) == 0) {
        return 0;
    }
    i = 0;
    while (i < 24 && bitclear_2C20((long)m >> i)) {
        i++;
    }
    g = D_0044F420_2C20[0];
    base = *(int **)(g + 0x34);
    if (*((unsigned char *)base + 0xD) < i) {
        return 0;
    }
    tab = *(int **)(g + 0x4C);
    for (e = base + tab[i]; e < base + tab[i + 1] - 2; e += 2) {
        ratio = (float)((unsigned int)(e[2] - e[0]) << 7) / (float)(unsigned int)e[1];
        if (max < ratio) {
            max = ratio;
        }
    }
    r = func_004126A0_2C20(func_00411F18_2C20(func_00413AF8_2C20(ratio * arg1), 0.5));
    D_0044F430_2C20[0] = 0;
    D_0044F420_2C20[0] = 0;
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2DF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_2DF8 { unsigned int f0; unsigned int f4; unsigned int f8; };
struct R_2DF8 { unsigned short n; unsigned short type; unsigned int f4; unsigned int f8; };
struct T_2DF8 { char pad[0x54]; int f54; char pad2[4]; char f5C[0x104 - 0x5C]; unsigned int f104; unsigned int f108; };
extern struct T_2DF8 *func_003D5290_2DF8(int) __asm__("func_003D5290");
extern int func_003D5450_2DF8(int) __asm__("func_003D5450");
extern struct R_2DF8 *func_003D4A50_2DF8(int) __asm__("func_003D4A50");
extern int func_003D3140_2DF8(int, int) __asm__("func_003D3140");
extern int func_002AE3A8_2DF8(int) __asm__("func_002AE3A8");
extern int func_002AE298_2DF8(int, char *, int, int) __asm__("func_002AE298");
extern int D_0044F420_2DF8[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_2DF8[] __asm__("D_0044F42C");
extern int D_0044F430_2DF8[] __asm__("D_0044F430");

int func_003D2DF8(int h, int idx, int arg2) {
    int ret = 0;
    struct T_2DF8 *t;
    struct R_2DF8 *r;
    struct E_2DF8 *e;

    if (D_0044F42C_2DF8[0] != 0) {
        if (D_0044F430_2DF8[0] != 0) {
            return -0x12;
        }
        if (func_003D5450_2DF8(-1) != 0) {
            goto ok;
        }
    }
    return -0x12;
ok:
    t = func_003D5290_2DF8(h);
    if (t != 0) {
        r = func_003D4A50_2DF8(h);
        e = (struct E_2DF8 *)(r + 1);
        e += idx;
        if (r != 0 && r->type != 2) {
            if (r->f4 < t->f104 + e->f4 || r->f8 < t->f108 + e->f8) {
                if (r->type == 1) {
                    unsigned int min = 0x7FFFFFFF;
                    int sel = -1;
                    int i;
                    e = (struct E_2DF8 *)(r + 1);
                    for (i = 0; i < r->n; i++, e++) {
                        if (e->f0 <= min) {
                            min = e->f0;
                            sel = i;
                        }
                    }
                    if (sel == idx) {
                        return 0;
                    }
                    func_003D3140_2DF8(h, sel);
                } else if (r->type == 0) {
                    func_002AE3A8_2DF8(h);
                }
            }
        }
        ret = func_002AE298_2DF8(t->f54, t->f5C, idx, arg2);
    }
    D_0044F430_2DF8[0] = 0;
    D_0044F420_2DF8[0] = 0;
    return ret;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D30A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_002AE768_30A8(int, int, int) __asm__("func_002AE768");
extern void *func_003D5290_30A8(int) __asm__("func_003D5290");
extern int func_003D5450_30A8(int) __asm__("func_003D5450");
extern int D_0044F420_30A8[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_30A8[] __asm__("D_0044F42C");
extern int D_0044F430_30A8[] __asm__("D_0044F430");

void func_003D30A8(int arg0, int arg1, int arg2) {
    int var_4;
    void *temp_2;

    if ((D_0044F42C_30A8[0] != 0) && (D_0044F430_30A8[0] == 0) && (func_003D5450_30A8(-1) != 0)) {
        temp_2 = func_003D5290_30A8(arg0);
        var_4 = -8;
        if (temp_2 != 0) {
            var_4 = (*(int *)((char*)(temp_2) + (0x54)));
        }
        func_002AE768_30A8(var_4, arg1, arg2);
        D_0044F430_30A8[0] = 0;
        D_0044F420_30A8[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3140);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002AE328_3140(int, int) __asm__("func_002AE328");
extern char *func_003D5290_3140(int) __asm__("func_003D5290");
extern int func_003D5450_3140(int) __asm__("func_003D5450");
extern int D_0044F420_3140[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_3140[] __asm__("D_0044F42C");
extern int D_0044F430_3140[] __asm__("D_0044F430");
int func_003D3140_3140(int arg0, int arg1) __asm__("func_003D3140");

int func_003D3140_3140(int arg0, int arg1) {
    int r;
    char *t;
    r = 0;
    if (D_0044F42C_3140[0] != 0) {
        if (D_0044F430_3140[0] != 0) {
            return -0x12;
        }
        if (func_003D5450_3140(-1) != 0) {
            goto ok;
        }
    }
    return -0x12;
ok:
    t = func_003D5290_3140(arg0);
    if (t != 0) {
        r = func_002AE328_3140(*(int *)(t + 0x54), arg1);
    }
    D_0044F430_3140[0] = 0;
    D_0044F420_3140[0] = 0;
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D31E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_31E8 { char p0[0xC]; int f0c; int f10; char p1[0x1C]; unsigned int f30; int f34; char p2[0x928 - 0x38]; };
extern struct S_31E8 D_00517610_31E8[] __asm__("D_00517610");
extern unsigned char D_0044F42C_31E8[] __asm__("D_0044F42C");
extern int D_0044F430_31E8[] __asm__("D_0044F430");
extern int D_0044F420_31E8[] __asm__("D_0044F420");

void func_003D31E8(unsigned int a, int b, int c) {
    int i;
    if (D_0044F42C_31E8[0] != 0 && D_0044F430_31E8[0] == 0) {
        if (func_003D5450(-1)) {
            a &= 0x0F000000;
            for (i = 0; i < 4; i++) {
                if (D_00517610_31E8[i].f34 != 0 && (D_00517610_31E8[i].f30 & a) != 0) {
                    D_00517610_31E8[i].f0c = b;
                    D_00517610_31E8[i].f10 = c;
                }
            }
            D_0044F430_31E8[0] = 0;
            D_0044F420_31E8[0] = 0;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4950);
#ifdef SKIP_ASM
extern int func_002AEAA8(int, unsigned int);

int func_003D4950(char *arg0) {
    int temp_2;
    int var_16 = 0;
    unsigned int i;

    for (i = 0; i < ((*(unsigned int *)(arg0 + 0x20) >> 0xE) & 0x1F); i++) {
        temp_2 = func_002AEAA8(*(int *)(arg0 + 0x54), i);
        if (temp_2 >= 0) {
            var_16 += temp_2;
        }
    }
    return var_16;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D49D0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4A50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_4A50 { char pad[0x30]; unsigned mask; int w34; char pad2[0x8EC]; int *tab; };
extern struct E_4A50 D_00517610_4A50[] __asm__("D_00517610");

int func_003D4A50(unsigned arg0) {
    unsigned i, b;
    unsigned hi = arg0 & 0xF0000000;
    unsigned mid = arg0 & 0x0F000000;
    unsigned m = arg0 & 0xFFFFFF;
    for (i = 0; i < 4; i++) {
        unsigned mk = D_00517610_4A50[i].mask;
        if (D_00517610_4A50[i].tab != 0 && D_00517610_4A50[i].w34 != 0 &&
            (mk & hi) && (mk & mid)) {
            for (b = 0; b < 0x18; b++) {
                if ((m >> b) & 1)
                    return D_00517610_4A50[i].tab[b];
            }
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D4B00);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4BB0);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4C40);

INCLUDE_ASM("ealib/seg_2B4578", func_003D4F10);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5128);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5290);

INCLUDE_ASM("ealib/seg_2B4578", func_003D5330);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D53B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D11B8_53B8(int, int *) __asm__("func_003D11B8");
extern int func_003D3030_53B8(int) __asm__("func_003D3030");

int func_003D53B8(int arg0, int arg1) {
    int sp0[12];
    int i;
    int t;

    for (i = 1; i >= 0; i--) {
        t = ((0x10000000 << i) | (0x01000000 << arg0)) + (1 << arg1);
        if (func_003D3030_53B8(t) >= 0) {
            func_003D11B8_53B8(t, sp0);
            if (sp0[0] >= 0) return t;
        }
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6108);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_6108 { char pad[0x30]; int m_a; int m_b; char pad2[0x928 - 0x38]; };
extern int f5450_6108(int) __asm__("func_003D5450");
extern int D_f420_6108[] __asm__("D_0044F420");
extern unsigned char D_f42C_6108[] __asm__("D_0044F42C");
extern int D_f430_6108[] __asm__("D_0044F430");
extern struct P_6108 D_7610_6108[4] __asm__("D_00517610");

void func_003D6108(int a0, int a1) {
    int i;
    if (D_f42C_6108[0] != 0 && D_f430_6108[0] == 0 && f5450_6108(-1) != 0) {
        a0 &= 0x0F000000;
        for (i = 0; i < 4; i++) {
            if (D_7610_6108[i].m_b != 0 && (D_7610_6108[i].m_a & a0) && f5450_6108(i & 0xFF) != 0) {
                *(int *)(D_f420_6108[0] + 0x14) = a1;
            }
        }
        D_f430_6108[0] = 0;
        D_f420_6108[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D61E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_61E8 { char pad[0x30]; int m_a; int m_b; char pad2[0x928 - 0x38]; };
extern int f5450_61E8(int) __asm__("func_003D5450");
extern int D_f420_61E8[] __asm__("D_0044F420");
extern unsigned char D_f42C_61E8[] __asm__("D_0044F42C");
extern int D_f430_61E8[] __asm__("D_0044F430");
extern struct P_61E8 D_7610_61E8[4] __asm__("D_00517610");

void func_003D61E8(int a0, int a1) {
    int i;
    if (D_f42C_61E8[0] != 0 && D_f430_61E8[0] == 0 && f5450_61E8(-1) != 0) {
        a0 &= 0x0F000000;
        for (i = 0; i < 4; i++) {
            if (D_7610_61E8[i].m_b != 0 && (D_7610_61E8[i].m_a & a0) && f5450_61E8(i & 0xFF) != 0) {
                *(int *)(D_f420_61E8[0] + 0x18) = a1;
            }
        }
        D_f430_61E8[0] = 0;
        D_f420_61E8[0] = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6E30);
#ifdef SKIP_ASM
int func_003D6E30(unsigned char *p, int key) {
    int cnt = p[6];
    int r = 0;
    if (cnt > 0) {
        unsigned char f = p[4];
        int off = 0;
        unsigned char w = p[5];
        int t = f & 0xF;
        unsigned char *q, *s;
        int i, j;
        q = p + ((w * (t + 2) + 0xF) & 0x3FFC) + t * 4;
        if (f & 0x80)
            off = (w + 7) / 8 + 1;
        s = q + off;
        i = 0;
        j = *s;
        s++;
        j -= 1;
        do {
            if (j <= -1)
                j = cnt - 1;
            if (s[j] == key) {
                r = i + 1;
                break;
            }
            i += 1;
            j -= 1;
        } while (i < cnt);
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D6EE8);

INCLUDE_ASM("ealib/seg_2B4578", func_003D6F80);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D7038);
#ifdef SKIP_ASM
void func_003D7038(unsigned char *p) {
    int n = p[4] & 0xF;
    int m = p[5];
    unsigned char *r = p + ((m * (n + 2) + 0xF) & 0x3FFC) + n * 4;
    if (*r != 0) {
        int cnt = (m + 7) / 8;
        if (cnt != 0) {
            int i = 1;
            do {
                r[i] = 0;
                i++;
            } while (i <= cnt);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D81F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E82B0 { unsigned char *p; int x; };
extern struct E82B0 D_0044FFB8_82B0[] __asm__("D_0044FFB8");

int func_003D81F0(unsigned char *arg0, int *arg1) {
    struct E82B0 *res;
    struct E82B0 *e = D_0044FFB8_82B0;
    int ret = 0;
    struct E82B0 **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *q = e->p;
        if (q != 0 && q[9] == arg0[2] && q[8] == arg0[3]) {
            *pres = e;
            found = 1;
            goto done;
        }
        i++;
        e++;
    } while (i < 8);
done:
    if (found) {
        ret = 1;
        *arg1 = res->x;
    }
    return ret;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D82B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern struct E82B0 D_0044FFB8_82B0[] __asm__("D_0044FFB8");

unsigned short func_003D82B0(unsigned char *arg0) {
    struct E82B0 *res;
    struct E82B0 *e = D_0044FFB8_82B0;
    unsigned short ret = 0;
    struct E82B0 **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *q = e->p;
        if (q != 0 && q[9] == arg0[2] && q[8] == arg0[3]) {
            *pres = e;
            found = 1;
            goto done;
        }
        i++;
        e++;
    } while (i < 8);
done:
    if (found) {
        ret = *(unsigned short *)(res->p + 0x14);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D8330);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern struct E82B0 D_0044FFB8_82B0[] __asm__("D_0044FFB8");

int func_003D8330(unsigned char *arg0, int *arg1) {
    struct E82B0 *res;
    struct E82B0 *e = D_0044FFB8_82B0;
    int ret = 0;
    struct E82B0 **pres = &res;
    int found = 0;
    int i = 0;
    do {
        unsigned char *q = e->p;
        if (q != 0 && q[9] == arg0[2] && q[8] == arg0[3]) {
            *pres = e;
            found = 1;
            goto done;
        }
        i++;
        e++;
    } while (i < 8);
done:
    if (found) {
        ret = 1;
        *arg1 = res->p[9];
    }
    return ret;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D9A40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Q9A40 { short a, b, c, d; };
struct R9A40 { int f0; int f4; unsigned char b8; unsigned char b9; unsigned char ba; unsigned char bb; char pad[0x5C - 12]; };
struct G9A40 { int f0, f4, f8; struct Q9A40 q[0x30]; struct R9A40 r[8]; };
extern struct G9A40 D_00450700_9A40 __asm__("D_00450700");

void func_003D9A40(void) {
    int i;
    D_00450700_9A40.f0 = -1;
    D_00450700_9A40.f4 = 0;
    D_00450700_9A40.f8 = 0;
    for (i = 0; i < 8; i++) {
        D_00450700_9A40.r[i].f0 = 0;
        D_00450700_9A40.r[i].f4 = 0;
        D_00450700_9A40.r[i].b8 = 0;
        D_00450700_9A40.r[i].b9 = 0;
        D_00450700_9A40.r[i].bb = 0;
    }
    for (i = 0; i < 0x30; i++) {
        D_00450700_9A40.q[i].a = -1;
        D_00450700_9A40.q[i].b = -1;
        D_00450700_9A40.q[i].c = 0;
        D_00450700_9A40.q[i].d = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA4E0);
#ifdef SKIP_ASM
void func_003DA4E0(unsigned char *a, unsigned char *b, unsigned char *c) {
    int i;
    signed char n;
    unsigned char t;
    unsigned char *base;
    int *w;
    i = 0;
    n = b[4];
    base = a + ((((a[4] >> 2) + 3) & 0x7C) + 8) + a[6] * 4;
    if (n > 0) {
        unsigned char *q = b + 0x10;
        do {
            if (q[-8] == 0xFE) {
                t = q[0];
                if (t & 0x80) {
                    w = (int *)((t & 0x7F) * 4 + (int)base);
                    *w |= 1 << c[i];
                }
            }
            i++;
            q++;
        } while (i < n);
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DA750);
#ifdef SKIP_ASM
int func_003DA750(char *h, unsigned char *p, int bit) {
    int a = *(short *)(h + 2);
    int res = 0;
    if (a < *(unsigned short *)(p + 0xA)) {
        int k = 0;
        unsigned char *r, *q;
        unsigned char mask;
        int idx;
        bit += p[5] * a;
        q = p + ((p[5] * ((p[4] & 0xF) + 2) + 0xF) & 0x3FFC) + (p[4] & 0xF) * 4;
        if (p[4] & 0x80) {
            k = (p[5] + 7) / 8 + 1;
        }
        r = q + k;
        if (p[6]) {
            r += p[6] + 1;
        }
        idx = bit / 8;
        mask = 1 << (bit - idx * 8);
        res = r[idx] & mask;
    }
    return res;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DB440);
#ifdef SKIP_ASM
int func_003DB440(unsigned char *arg0, int arg1) {
    int i, j;
    int found = 0;
    int n = arg0[4] >> 2;
    unsigned char *e;
    for (i = 0; i < n; i++) {
        e = arg0 + *(arg0 + i + 8) * 4;
        if (e[2] == arg1) {
            found = 1;
            break;
        }
        for (j = 0; j < 4; j++) {
            if (*(e + j + 8) == arg1) {
                found = 1;
                break;
            }
        }
    }
    return found;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DBEF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void ucopy_BEF0(unsigned short *, unsigned short *) __asm__("USTR_copy");
extern int ulen_BEF0(unsigned short *) __asm__("USTR_length");

void fBEF0_BEF0(unsigned short *d, unsigned short *s, unsigned short *a2, unsigned short *a3) __asm__("func_003DBEF0");
void fBEF0_BEF0(unsigned short *d, unsigned short *s, unsigned short *a2, unsigned short *a3) {
    unsigned short c, c2;
    unsigned short *a;
    c = *s;
    if (c != 0) {
        do {
            if (c != 0x25) {
                *d++ = *s++;
            } else {
                c2 = s[1];
                if (c2 == 0x31) {
                    ucopy_BEF0(d, a2);
                    s += 2;
                    a = a2;
                    d += ulen_BEF0(a);
                } else if (c2 == 0x32) {
                    ucopy_BEF0(d, a3);
                    s += 2;
                    a = a3;
                    d += ulen_BEF0(a);
                } else {
                    *d++ = *s++;
                }
            }
            c = *s;
        } while (c != 0);
    }
    *d = 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DCBD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DCC88_CBD8(int, int, int, int) __asm__("func_003DCC88");
extern int func_003DCD98_CBD8(int, int, int) __asm__("func_003DCD98");
extern int (*D_00450C10_CBD8[])(char *, int, int) __asm__("D_00450C10");
extern char D_00495D28_CBD8[] __asm__("D_00495D28");
extern int D_00519AB0_CBD8[] __asm__("D_00519AB0");
extern int D_00519AE8_CBD8[] __asm__("D_00519AE8");

int func_003DCBD8(int arg0, int arg1, int arg2) {
    int r;
    if (D_00519AB0_CBD8[0] == 0) {
        r = D_00450C10_CBD8[1](D_00495D28_CBD8, func_003DCD98_CBD8(arg0, arg1, arg2), 0x100);
        D_00519AE8_CBD8[0] = r;
        return func_003DCC88_CBD8(arg0, arg1, arg2, r);
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DCDE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E44B0_CDE0(void) __asm__("func_003E44B0");
extern void func_003DECA0_CDE0(int, int) __asm__("func_003DECA0");
extern void func_003E56E0_CDE0(char *) __asm__("func_003E56E0");
extern void func_003E6220_CDE0(char *, int) __asm__("func_003E6220");
extern void (*D_00450C10_CDE0[])(int) __asm__("D_00450C10");
extern void (*D_00450C18_CDE0[])(int) __asm__("D_00450C18");
extern char D_00519AC4_CDE0[] __asm__("D_00519AC4");
extern int *D_00519AE4_CDE0[] __asm__("D_00519AE4");
extern int D_00519AE8_CDE0[] __asm__("D_00519AE8");

void func_003DCDE0(void) {
    int *n;
    func_003E44B0_CDE0();
    for (n = D_00519AE4_CDE0[0]; n != 0; n = (int *)n[5]) {
        D_00450C10_CDE0[2](n[0]);
        func_003DECA0_CDE0(*(int *)n[3], 0);
    }
    func_003E56E0_CDE0(D_00519AC4_CDE0);
    func_003E6220_CDE0(D_00519AC4_CDE0 - 0x14, 0x38);
    if (D_00519AE8_CDE0[0] != 0) {
        D_00450C18_CDE0[0](D_00519AE8_CDE0[0]);
        D_00519AE8_CDE0[0] = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD148);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct H_D148 { unsigned int v; };
extern char *D_00519AD8_D148[] __asm__("D_00519AD8");

void func_003DD148(struct H_D148 h) {
    struct H_D148 c;
    unsigned int u = h.v;
    int t;
    int ok = 0;
    c.v = u;
    if (u != 0) {
        ok = ((*(unsigned int *)(((unsigned char *)&c)[3] * 0x30 + D_00519AD8_D148[0]) ^ u) & 0xFFFFF) == 0;
    }
    if (ok != 0) {
        t = (u >> 20) & 0xF;
        if (t != 3 && t != 0xA) {
            *(int *)(((unsigned char *)&h)[3] * 0x30 + D_00519AD8_D148[0] + 4) = 1;
        }
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DD1D8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DD310);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD438);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *f_E670_D438() __asm__("func_003DE670");
extern int f_E800_D438() __asm__("func_003DE800");
extern void f_E668_D438(int *) __asm__("func_003DE668");
extern char *f_strncpy_D438(char *, char *, int) __asm__("strncpy");
extern void f_Exec_D438(int *) __asm__("iFILESYS_ExecCommand");
struct R_D438 { unsigned flags; int pad4, pad8; int wC; int w10; int w14; int w18; int pad1C, pad20; char *w24; };

int func_003DD438(char *arg0, int arg1, int arg2) {
    struct R_D438 *r = (struct R_D438 *)f_E670_D438();
    r->w14 = arg2;
    r->w10 = arg1;
    r->flags = (r->flags & 0xFF0FFFFF) | 0x800000;
    r->w18 = 1;
    r->w24 = (char *)f_E800_D438();
    if (r->w24 == 0) {
        r->wC = 2;
        f_E668_D438((int *)r);
    }
    f_strncpy_D438(r->w24 + 0x10, arg0, 0x100);
    f_Exec_D438((int *)r);
    return r->flags;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD4E8);
#ifdef SKIP_ASM
extern int func_003DE800();
extern void func_003DE668();

int func_003DD4E8(char *arg0, int arg1, int arg2, int arg3) {
    char *p = (char *)func_003DE670();
    int r;
    *(int *)(p + 0x14) = arg3;
    *(int *)(p + 0x18) = arg1;
    *(int *)(p + 0x10) = arg2;
    *(int *)p = (*(int *)p & 0xFF0FFFFF) | 0x200000;
    r = func_003DE800();
    *(int *)(p + 0x24) = r;
    if (r == 0) {
        *(int *)(p + 0xC) = 2;
        func_003DE668(p);
    }
    strncpy(*(char **)(p + 0x24) + 0x10, arg0, 0x100);
    iFILESYS_ExecCommand(p);
    return *(int *)p;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DD5A0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD648);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { unsigned int pad0 : 20; unsigned int f : 4; unsigned int pad1 : 8; int a; int b; int c; int d; int e; int f1; int g; int h; } C_D648;
extern C_D648 *func_003DE670_D648(void) __asm__("func_003DE670");
extern void func_003DE668_D648(C_D648 *) __asm__("func_003DE668");
extern void iFILESYS_ExecCommand_D648(C_D648 *) __asm__("iFILESYS_ExecCommand");

int func_003DD648_impl(int *arg0, int arg1, int arg2, int arg3, int arg4, int arg5) __asm__("func_003DD648");
int func_003DD648_impl(int *arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
    C_D648 *c;
    int n;

    c = func_003DE670_D648();
    *(int*)((char*)c + 0x14) = arg5;
    *(int*)((char*)c + 0x10) = arg4;
    c->f = 4;
    if (arg0 == 0) {
        *(int*)((char*)c + 0xC) = 6;
        func_003DE668_D648(c);
    }
    *(int**)((char*)c + 0x24) = arg0;
    n = arg0[1];
    *(int*)((char*)c + 0x20) = arg2;
    if (n < arg1 + arg3) arg3 = n - arg1;
    *(int*)((char*)c + 0x18) = arg1;
    *(int*)((char*)c + 0x1C) = arg3;
    iFILESYS_ExecCommand_D648(c);
    return *(int*)c;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD720);
#ifdef SKIP_ASM
extern void func_003DE668();

int func_003DD720(int a0, int a1, int a2, int a3, int a4, int a5) {
    char *p = (char *)func_003DE670();
    *(int *)(p + 0x14) = a5;
    *(int *)p = (*(int *)p & 0xFF0FFFFF) | 0x500000;
    if (a0 == 0) {
        *(int *)(p + 0xC) = 6;
        func_003DE668(p);
    }
    *(int *)(p + 0x24) = a0;
    *(int *)(p + 0x10) = a4;
    *(int *)(p + 0x1C) = a3;
    *(int *)(p + 0x18) = a1;
    *(int *)(p + 0x20) = a2;
    iFILESYS_ExecCommand(p);
    return *(int *)p;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD7E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *func_003DE670_D7E0() __asm__("func_003DE670");
extern void func_003DE668_D7E0(int *) __asm__("func_003DE668");

int func_003DD7E0(int *arg0, int arg1, int arg2) {
    int *temp_2;

    temp_2 = func_003DE670_D7E0();
    temp_2[5] = arg2;
    temp_2[4] = arg1;
    temp_2[0] = (temp_2[0] & 0xFF0FFFFF) | 0x600000;
    if (arg0 != 0) {
        temp_2[6] = arg0[1];
        temp_2[2] = 1;
    } else {
        temp_2[3] = 6;
        func_003DE668_D7E0(temp_2);
    }
    return temp_2[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DD878);
#ifdef SKIP_ASM
extern int *func_003DE670();
extern void iFILESYS_ExecCommand(int);

int func_003DD878(int a0, int a1, int a2) {
    int *p = func_003DE670();
    p[5] = a2;
    p[4] = a1;
    p[0] = (p[0] & 0xFF0FFFFF) | 0x700000;
    iFILESYS_ExecCommand((int)p);
    return p[0];
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DDC30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { unsigned int pad0 : 20; unsigned int f : 4; unsigned int pad1 : 8; int a; int b; int c; int d; int e; } R_DC30;
typedef struct N_DC30 { int f0; int pad4[2]; char *key; int pad10; struct N_DC30 *next; } N_DC30;
typedef struct { char *owner; int pad4; int f8; char pad[0x110 - 0xC]; } E_DC30;
typedef struct { int f0; int count; char pad8[0x2C - 8]; E_DC30 *arr; int pad30; N_DC30 *head; } G_DC30;
extern R_DC30 *func_003DE670_DC30(void) __asm__("func_003DE670");
extern int func_003DD5A0_DC30(char *, int, R_DC30 *) __asm__("func_003DD5A0");
extern void func_003DCF70_DC30(int, void *) __asm__("func_003DCF70");
extern void func_003DDC00_DC30(int, int, int) __asm__("func_003DDC00");
extern G_DC30 D_00519AB0_DC30 __asm__("D_00519AB0");
extern int (*D_00450C10_DC30[])(void *) __asm__("D_00450C10");

int func_003DDC30(char *a0, int a1, int a2) {
    N_DC30 *prev;
    N_DC30 *n;
    R_DC30 *r;
    E_DC30 *e;
    int i;

    prev = 0;
    n = D_00519AB0_DC30.head;
    r = func_003DE670_DC30();
    e = D_00519AB0_DC30.arr;
    r->e = a2;
    r->d = a1;
    r->f = 10;
    if (*(int *)(a0 + 8) != 0) {
        r->c = 7;
    }
    for (i = 0; i < D_00519AB0_DC30.count; i++, e++) {
        if (e != 0 && e->f8 != 0 && *(char **)(e->owner + 0xC) == a0) {
            r->b = -2;
            r->c = 8;
            break;
        }
    }
    while (n != 0 && n->key != a0) {
        prev = n;
        n = n->next;
    }
    if (n == 0) {
        r->c = 5;
    }
    if (prev != 0) {
        prev->next = n->next;
    } else {
        D_00519AB0_DC30.head = D_00519AB0_DC30.head->next;
    }
    D_00450C10_DC30[2]((void *)n->f0);
    D_00450C10_DC30[2](n);
    func_003DCF70_DC30(func_003DD5A0_DC30(a0, a1, r), func_003DDC00_DC30);
    return *(int *)r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", FILESYS_atomic);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void iFILESYS_ExecCommand(int);
extern int D_00519AB0_9AB0[] __asm__("D_00519AB0");

int FILESYS_atomic_DDF0(int (*arg0)(int, int), int arg1, int arg2, int arg3) __asm__("FILESYS_atomic");
int FILESYS_atomic_DDF0(int (*arg0)(int, int), int arg1, int arg2, int arg3) {
    int temp_18;
    int temp_2;

    temp_18 = D_00519AB0_9AB0[2];
    D_00519AB0_9AB0[2] = arg2;
    temp_2 = arg0(arg2, arg3);
    D_00519AB0_9AB0[2] = temp_18;
    iFILESYS_ExecCommand(0);
    return temp_2;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DDE78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int strlen_DE78(char *) __asm__("strlen");
extern char *strcpy_DE78(char *, char *) __asm__("strcpy");
extern int func_00416B18_DE78(char *, char *, int) __asm__("func_00416B18");
extern void func_004162D0_DE78(char *, char *) __asm__("func_004162D0");
extern char *D_00450C50_DE78[] __asm__("D_00450C50");
extern char *D_00450C08_DE78[] __asm__("D_00450C08");
extern int D_00450C44_DE78[] __asm__("D_00450C44");
extern int D_00450C48_DE78[] __asm__("D_00450C48");
extern char D_00519AF0_DE78[] __asm__("D_00519AF0");
extern char D_00495D78_DE78[] __asm__("D_00495D78");

void func_003DDE78(char *a) {
    int n, len;
    n = strlen_DE78(D_00450C50_DE78[0]);
    if (func_00416B18_DE78(a, D_00450C08_DE78[0], 3) == 0) {
        if ((D_00450C48_DE78[0] & 3) == 2) return;
        D_00450C44_DE78[0] = 1;
        a += 3;
    } else if (func_00416B18_DE78(a, D_00450C50_DE78[0], n) == 0) {
        if (!(D_00450C48_DE78[0] & 2)) return;
        D_00450C44_DE78[0] = 2;
        a += n;
    }
    strcpy_DE78(D_00519AF0_DE78, a);
    if (D_00519AF0_DE78[0xFF] == 0) {
        len = strlen_DE78(D_00519AF0_DE78);
        if (len > 0 && D_00519AF0_DE78[len - 1] != 0x2F) {
            func_004162D0_DE78(D_00519AF0_DE78, D_00495D78_DE78);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", FILESYS_bypassqueuefileinfo);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct N_E4D0 { int f0; int pad[2]; char *fC; int pad2; struct N_E4D0 *next; };
extern struct N_E4D0 *D_00519AE4_E4D0[] __asm__("D_00519AE4");
extern char *strchr_E4D0(char *, int) __asm__("strchr");
extern int func_003E3208_E4D0(char *, int, int *) __asm__("func_003E3208");
extern int func_003E34E8_E4D0(int) __asm__("func_003E34E8");
extern int func_003E32F8_E4D0(int) __asm__("func_003E32F8");
extern int func_004165A8_E4D0(char *, char *) __asm__("func_004165A8");
extern int BIG_locateentryz_E4D0(int, char *, int, int *, int *) __asm__("BIG_locateentryz");
int FILESYS_bypassqueuefileinfo(char *name, int flags, int *out) {
    char path[256];
    char file[256];
    int h;
    int x;
    int off;
    int found = 0;
    int mode;
    struct N_E4D0 *n;

    *out = 0;
    file[0] = 0;
    if (strchr_E4D0(name, '|')) {
        if (*name == '|') {
            mode = 2;
        } else {
            int len = strchr_E4D0(name, '|') - name;
            mode = 4;
            strncpy(path, name, len);
            path[len] = 0;
        }
        strcpy(file, strchr_E4D0(name, '|') + 1);
    } else {
        mode = 1;
        if (flags & 1) {
            mode = 3;
            strcpy(file, name);
        }
    }
    if ((mode & 1) && func_003E3208_E4D0(name, flags, &h)) {
        found = 1;
        *out = func_003E34E8_E4D0(h);
        func_003E32F8_E4D0(h);
    }
    if (!found && (mode & 6)) {
        for (n = D_00519AE4_E4D0[0]; n != 0 && !found; n = n->next) {
            if ((mode & 4) && func_004165A8_E4D0(n->fC + 0x10, path)) continue;
            if (BIG_locateentryz_E4D0(n->f0, file, 0, &x, &off)) {
                found = 1;
                *out = off;
            }
        }
    }
    return found;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DE928);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct A_E928 { int p0, p4, p8, pC, f10, f14, f18, f1C; };
struct B_E928 { int f0, f4, f8, fC; };
extern struct A_E928 D_00450C10_E928 __asm__("D_00450C10");
extern struct B_E928 D_00450C58_E928 __asm__("D_00450C58");
extern int D_00450C50_E928[] __asm__("D_00450C50");
extern void func_003E6220_E928(int, int) __asm__("func_003E6220");
extern void func_003E6574_E928(int *, void *, int) __asm__("func_003E6574");

int func_003DE928(int *arg0) {
    int n = *arg0;
    if (n >= 0x25) {
        func_003E6220_E928(arg0, n);
    }
    D_00450C10_E928.f18 = D_00450C58_E928.f0;
    D_00450C10_E928.f10 = D_00450C58_E928.f4;
    D_00450C10_E928.f14 = D_00450C58_E928.fC != 0;
    D_00450C10_E928.f1C = D_00450C50_E928[0];
    func_003E6574_E928(arg0, &D_00450C10_E928, n);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DE9B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct A_E9B8 { int p0, p4, p8, pC, f10, f14, f18, f1C; };
struct B_E9B8 { int f0, f4, f8, fC; };
extern struct A_E9B8 D_00450C10_E9B8 __asm__("D_00450C10");
extern struct B_E9B8 D_00450C58_E9B8 __asm__("D_00450C58");
extern int D_00450C50_E9B8[] __asm__("D_00450C50");
extern void func_003E6220_E9B8(void *, int) __asm__("func_003E6220");
extern void func_003E6574_E9B8(void *, int *, int) __asm__("func_003E6574");

int func_003DE9B8(int *arg0) {
    if (*arg0 < 0x24) {
        func_003E6220_E9B8(&D_00450C10_E9B8, 0x24);
    }
    func_003E6574_E9B8(&D_00450C10_E9B8, arg0, *arg0);
    D_00450C58_E9B8.f0 = D_00450C10_E9B8.f18;
    D_00450C58_E9B8.f4 = D_00450C10_E9B8.f10;
    D_00450C58_E9B8.f8 = D_00450C10_E9B8.pC;
    D_00450C58_E9B8.fC = D_00450C10_E9B8.f14;
    D_00450C50_E9B8[0] = D_00450C10_E9B8.f1C;
    return 1;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DEA48);

INCLUDE_ASM("ealib/seg_2B4578", func_003DEB50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DEBF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DD4E8_EBF0(int, int, int, int) __asm__("func_003DD4E8");
extern void func_003DD1D8_EBF0(int) __asm__("func_003DD1D8");
extern int func_003DCE90_EBF0(int) __asm__("func_003DCE90");
extern int func_003DD310_EBF0(int) __asm__("func_003DD310");

int func_003DEBF0_impl(int a0, int a1, int a2, int *a3) __asm__("func_003DEBF0");
int func_003DEBF0_impl(int a0, int a1, int a2, int *a3) {
    int ok = 0;
    int h = func_003DD4E8_EBF0(a0, a1, a2, 0);
    if (h != 0) {
        func_003DD1D8_EBF0(h);
        ok = func_003DCE90_EBF0(h) == 1;
        *a3 = func_003DD310_EBF0(h);
    } else {
        *a3 = 0;
    }
    return ok;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DED50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DDAC0_ED50(int, int, int, int) __asm__("func_003DDAC0");
extern void func_003DD1D8_ED50(int) __asm__("func_003DD1D8");
extern int func_003DCE90_ED50(int) __asm__("func_003DCE90");
extern int func_003DD310_ED50(int) __asm__("func_003DD310");

int func_003DED50(int a0, int a1, int a2, int *a3) {
    int ok = 0;
    int h = func_003DDAC0_ED50(a0, a1, a2, 0);
    if (h != 0) {
        func_003DD1D8_ED50(h);
        ok = func_003DCE90_ED50(h) == 1;
        *a3 = func_003DD310_ED50(h);
    } else {
        *a3 = 0;
    }
    return ok;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DF028);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_00519C08_F028[] __asm__("D_00519C08");
extern void MUTEX_lock_F028(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_F028(void *) __asm__("MUTEX_unlock");
extern void releaserequest_F028(void *) __asm__("releaserequest");

void func_003DF028(void *arg0) {
    int (*temp_19)(int);
    int temp_16;
    int temp_18;

    MUTEX_lock_F028(D_00519C08_F028);
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    temp_16 = (*(int *)((char*)(arg0) + (0x10)));
    temp_19 = (*(int (**)(int))((char*)(arg0) + (0x18)));
    temp_18 = (*(int *)((char*)(arg0) + (0xC)));
    MUTEX_unlock_F028(D_00519C08_F028);
    if (temp_16 != 0) {
        if (temp_18 == 0) {
            releaserequest_F028(arg0);
        }
    } else if (temp_19 != 0) {
        temp_19((*(int *)((char*)(arg0) + (0))));
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DF0B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DF0F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003DCF70(int, void *);
extern int func_003DD310_F0F0(int) __asm__("func_003DD310");
extern int func_003DD5A0(int, int, void *);
extern int func_003DD648(int, int, int, int, int, void *);
extern void func_003DF0B8(void *);

void func_003DF0F0(int a0, int a1, int *s) {
    int n = func_003DD310_F0F0(*(volatile int *)&s[7]);
    s[2] += n;
    s[11] += n;
    if (n <= 0x7FFF || s[4] != 0) {
        *(volatile int *)&s[7] = func_003DD5A0(s[8], 0x63, s);
        if (*(volatile int *)&s[7] != 0) {
            func_003DCF70(*(volatile int *)&s[7], func_003DF0B8);
        }
    } else {
        int off = s[9] + n;
        int rem = s[10] - n;
        s[9] = off;
        s[10] = rem;
        *(volatile int *)&s[7] = func_003DD648(s[8], off, s[11], rem <= 0x8000 ? rem : 0x8000, 0x63, s);
        if (*(volatile int *)&s[7] != 0) {
            func_003DCF70(*(volatile int *)&s[7], func_003DF0F0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DF1E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void fCF70_F1E0(int, char *) __asm__("func_003DCF70");
extern int fD310_F1E0(int) __asm__("func_003DD310");
extern int fD5A0_F1E0(int, int, void *) __asm__("func_003DD5A0");
extern int fD648_F1E0(int, int, int, int, int, void *) __asm__("func_003DD648");
extern int (*D_0C14_F1E0[])(char *, int, int) __asm__("D_00450C14");
extern char D_5DA8_F1E0[] __asm__("D_00495DA8");
extern char fF0B8_F1E0[] __asm__("func_003DF0B8");
extern char fF0F0_F1E0[] __asm__("func_003DF0F0");

void func_003DF1E0(int a, int b, char *s) {
    int t5, v, m, old;
    t5 = fD310_F1E0(*(volatile int *)(s + 0x1C));
    if (*(int *)(s + 0x10) != 0) {
        *(volatile int *)(s + 0x1C) = fD5A0_F1E0(*(int *)(s + 0x20), 0x63, s);
        if (*(volatile int *)(s + 0x1C) != 0) fCF70_F1E0(*(volatile int *)(s + 0x1C), fF0B8_F1E0);
    } else {
        old = *(int *)(s + 0x28);
        *(int *)(s + 0x28) = t5;
        v = D_0C14_F1E0[0](D_5DA8_F1E0, t5, old);
        m = *(int *)(s + 0x28);
        *(int *)(s + 0x14) = v;
        *(int *)(s + 0x2C) = v;
        *(volatile int *)(s + 0x1C) = fD648_F1E0(*(int *)(s + 0x20), *(int *)(s + 0x24), v, m <= 0x8000 ? m : 0x8000, 0x63, s);
        if (*(volatile int *)(s + 0x1C) != 0) fCF70_F1E0(*(volatile int *)(s + 0x1C), fF0F0_F1E0);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DF2C0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DF3C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DD310_F3C0(int) __asm__("func_003DD310");
struct Q_F3C0 { int w0; int p1; int f8; int pc; int f10; int p14; int p18; volatile int h; int f20; int f24; int f28; int f2c; };

void func_003DF3C0(int a, int b, struct Q_F3C0 *s) {
    int n = func_003DD310_F3C0(s->h);
    int x;
    int m;
    s->f8 += n;
    s->f2c += n;
    if (n <= 0x7FFF || s->f10 != 0) {
        func_003DF028(s);
        return;
    }
    x = s->f28 - n;
    m = x > 0x8000 ? 0x8000 : x;
    s->f24 += n;
    s->f28 = x;
    s->h = func_003DD648(s->f20, s->f24, s->f2c, m, 99, s);
    if (s->h != 0) {
        func_003DCF70(s->h, func_003DF3C0);
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DF488);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF570);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DF690);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_00519BF8_F690[] __asm__("D_00519BF8");
extern int *f_EED8_F690(char *) __asm__("func_003DEED8");
extern void f_EF40_F690(void *) __asm__("func_003DEF40");
extern int f_D4E8_F690(int, int, int, void *) __asm__("func_003DD4E8");
extern void f_CF70_F690(int, void *) __asm__("func_003DCF70");
extern char f_F2C0_F690[] __asm__("func_003DF2C0");
struct O_F690 { int w0, w4, w8, wC, w10, w14, w18; volatile int w1C; int w20, w24, w28, w2C; };

int func_003DF690(int arg0, int arg1) {
    struct O_F690 *t = (struct O_F690 *)f_EED8_F690(D_00519BF8_F690);
    if (t != 0) {
        f_EF40_F690(t);
        t->w28 = arg1;
        t->w8 = 0;
        t->wC = 0;
        t->w10 = 0;
        t->w18 = 0;
        t->w24 = 0;
        t->w2C = 0;
        t->w14 = 1;
        t->w1C = f_D4E8_F690(arg0, 1, 0x64, t);
        if (t->w1C != 0) {
            f_CF70_F690(t->w1C, f_F2C0_F690);
            return t->w0;
        }
        return 0;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003DF748);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003DF2C0_F748(void) __asm__("func_003DF2C0");
extern int D_00519BF8_F748[] __asm__("D_00519BF8");
struct Q_F748 { int w0; int p1[1]; int f8; int fc; int f10; int f14; int f18; volatile int h; int p2[1]; int f24; int f28; int f2c; };

int func_003DF748(int a, int b, int c) {
    struct Q_F748 *p = (struct Q_F748 *)func_003DEED8((void **)D_00519BF8_F748);
    if (p == 0) return 0;
    func_003DEF40(p);
    p->f28 = c;
    p->f2c = b;
    p->f8 = 0;
    p->fc = 0;
    p->f10 = 0;
    p->f14 = 0;
    p->f18 = 0;
    p->f24 = 0;
    p->h = func_003DD4E8(a, 1, 100, p);
    if (p->h == 0) return 0;
    func_003DCF70(p->h, func_003DF2C0_F748);
    return p->w0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003DF808);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF8E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003DF980);

INCLUDE_ASM("ealib/seg_2B4578", ASYNCFILE_release);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFAF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void mlock_FAF0(char *) __asm__("MUTEX_lock");
extern void munlock_FAF0(char *) __asm__("MUTEX_unlock");
extern void fD148_FAF0(int) __asm__("func_003DD148");
extern char *fEF70_FAF0(int) __asm__("func_003DEF70");
extern void rel_FAF0(char *) __asm__("releaserequest");
extern char D_9C08_FAF0[] __asm__("D_00519C08");

int func_003DFAF0(int a0) {
    int v18, v19, v20, r;
    char *t;
    v19 = 0;
    v20 = 0;
    mlock_FAF0(D_9C08_FAF0);
    v18 = 0;
    t = fEF70_FAF0(a0);
    if (t != 0 && ((v19 = *(int *)(t + 0x1C), v20 = *(int *)(t + 0x10), v18 = *(int *)(t + 0xC), v19 != 0) || v18 == 0)) {
        *(int *)(t + 0x10) = 1;
    }
    munlock_FAF0(D_9C08_FAF0);
    if (t != 0 && v20 == 0) {
        if (*(int *)(t + 0x10) != 0) {
            if (v19 != 0) {
                fD148_FAF0(v19);
                return 1;
            }
            if (v18 == 0) {
                rel_FAF0(t);
            }
            return 1;
        }
    }
    return -1;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFCD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00519C18_FCD8[] __asm__("D_00519C18");
extern void *func_003DFCD8_impl(char *arg0) __asm__("func_003DFCD8");

void *func_003DFCD8_impl(char *arg0) {
    char *r;
    int t;
    MUTEX_lock(arg0 + 4);
    r = 0;
    if (*(char **)(arg0 + 0x68) != 0) {
        r = *(char **)(arg0 + 0x68);
        t = D_00519C18_FCD8[0] + 0x100;
        *(char **)(arg0 + 0x68) = *(char **)(r + 0xC);
        D_00519C18_FCD8[0] = t;
        if (t == 0) {
            D_00519C18_FCD8[0] = 0x100;
        }
        *(int *)r = *(unsigned char *)r | D_00519C18_FCD8[0];
    }
    MUTEX_unlock((int *)(arg0 + 4));
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFD58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_lock_FD58(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_FD58(void *) __asm__("MUTEX_unlock");

void func_003DFD58(char *arg0, char *arg1) {
    char *temp_2;

    (*(int *)((char*)(arg1) + (0xC))) = 0;
    (*(int *)((char*)(arg1) + (4))) = 1;
    MUTEX_lock_FD58(arg0 + 4);
    temp_2 = (*(char **)((char*)(arg0) + (0x64)));
    if (temp_2 == 0) {
        (*(char **)((char*)(arg1) + (8))) = 0;
        (*(char **)((char*)(arg0) + (0x5C))) = arg1;
        (*(char **)((char*)(arg0) + (0x60))) = arg1;
        (*(char **)((char*)(arg0) + (0x64))) = arg1;
    } else {
        (*(char **)((char*)(arg1) + (8))) = temp_2;
        (*(char **)((char*)((*(char **)((char*)(arg0) + (0x64)))) + (0xC))) = arg1;
        (*(char **)((char*)(arg0) + (0x64))) = arg1;
    }
    MUTEX_unlock_FD58(arg0 + 4);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003DFE18);
#ifdef SKIP_ASM
void func_003DFE18(char *l, char *n) {
    if (n == *(char **)(l + 0x5C)) {
        *(int *)(l + 0x5C) = *(int *)(n + 0xC);
    } else {
        *(int *)(*(char **)(n + 8) + 0xC) = *(int *)(n + 0xC);
    }
    if (n == *(char **)(l + 0x64)) {
        *(int *)(l + 0x64) = *(int *)(n + 8);
    } else {
        *(int *)(*(char **)(n + 0xC) + 8) = *(int *)(n + 8);
    }
    if (n == *(char **)(l + 0x60)) {
        char *t = *(char **)(n + 0xC);
        if (t == 0) t = *(char **)(n + 8);
        *(char **)(l + 0x60) = t;
    }
    *(int *)(n + 4) = 0;
    *(int *)(n + 0xC) = *(int *)(l + 0x68);
    *(char **)(l + 0x68) = n;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0948);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct T_0948 { int a, b, c; };
extern int fFBD0_0948(int, void *, void *) __asm__("func_003DFBD0");

void func_003E0948(int a0, int a1, int a2, int a3, int a4) {
    char *sp0;
    int sp4;
    if (fFBD0_0948(a0, &sp0, &sp4) == 0 && a1 > 0) {
        int t = *(int *)(sp0 + 0x20);
        if (t >= a1 && (a1 != t || (a2 | a3) == 0) && (a4 > 0 || a4 == -1 || a4 == -2)
            && *(int *)(sp0 + 0x28) >= a4 && *(int *)(sp0 + 0x38) == 0) {
            struct T_0948 *p = &(*(struct T_0948 **)(sp0 + 0x1C))[a1 - 1];
            p->a = a2;
            p->b = a3;
            p->c = a4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0A28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_FBD0_0A28(int, void *, void *) __asm__("func_003DFBD0");
extern void f_1110_0A28(int) __asm__("func_003E1110");
extern int f_5440_0A28(int) __asm__("func_003E5440");
extern void f_SYNC_0A28(int) __asm__("SYNCTASK_run");
extern void f_5398_0A28(int) __asm__("func_003E5398");
extern void f_56E0_0A28(void *) __asm__("func_003E56E0");
extern void f_ECA0_0A28(int, int) __asm__("func_003DECA0");
struct S_0A28 { int w0; char pad[0x34]; int w38; char pad2[0x130]; int w16C; };

void func_003E0A28(int arg0) {
    struct S_0A28 *sp0;
    int sp4;
    if (f_FBD0_0A28(arg0, &sp0, &sp4) == 0) {
        f_1110_0A28(arg0);
        while (sp0->w38 == 1) {
            if (f_5440_0A28(0) != 0)
                f_SYNC_0A28(0);
            f_5398_0A28(0);
        }
        sp0->w0 = 0;
        f_56E0_0A28((char *)sp0 + 4);
        if (sp0->w16C != 0)
            f_ECA0_0A28(sp0->w16C, 0x64);
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0B28);
#ifdef SKIP_ASM
extern int func_003DFBD0(int, void *, void *);
extern void func_003E0B98(int, int);

void func_003E0B28(int a0, int a1) {
    char *p;
    int q;
    if (func_003DFBD0(a0, &p, &q) == 0) {
        int old = *(int *)(p + 0x44);
        *(int *)(p + 0x44) = a1;
        {
            int lim = *(int *)(p + 0x4C);
            if ((lim < old) != (lim < a1)) {
                func_003E0B98(a0, lim < a1);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0C88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct L_0C88 { char *p; int q; };
extern int func_003DFBD0_0C88(int, void *, void *) __asm__("func_003DFBD0");
extern char *func_003DFCD8_0C88(char *) __asm__("func_003DFCD8");
extern void func_003DFD58_0C88(char *, char *) __asm__("func_003DFD58");
extern void func_003E0270_0C88(char *, int) __asm__("func_003E0270");
extern void MUTEX_lock_0C88(char *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_0C88(char *) __asm__("MUTEX_unlock");

int func_003E0C88(int a0, char *name, int x, int y) {
    struct L_0C88 l;
    char *node;
    int old;
    if (func_003DFBD0_0C88(a0, &l.p, &l.q) != 0) {
        return 0;
    }
    node = func_003DFCD8_0C88(l.p);
    if (node == 0) {
        return 0;
    }
    *(int *)(node + 0x10) = 0;
    strncpy(node + 0x14, name, 0xFF);
    *(int *)(node + 0x118) = x;
    *(int *)(node + 0x11C) = y;
    func_003DFD58_0C88(l.p, node);
    MUTEX_lock_0C88(l.p + 4);
    old = *(int *)(l.p + 0x38);
    if (old == 0) {
        *(int *)(l.p + 0x38) = 1;
    }
    MUTEX_unlock_0C88(l.p + 4);
    if (old == 0) {
        if (*(int *)(l.p + 0x48) != 0) {
            func_003E0270_0C88(l.p, *(int *)(l.p + 0x40));
        } else {
            func_003E0270_0C88(l.p, *(int *)(l.p + 0x3C));
        }
    }
    return *(int *)node;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E0D80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DFBD0_0D80(int, char **, int *) __asm__("func_003DFBD0");
extern char *func_003DFCD8_0D80(char *) __asm__("func_003DFCD8");
extern void func_003DFD58_0D80(char *, char *) __asm__("func_003DFD58");
extern void MUTEX_lock_0D80(char *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_0D80(char *) __asm__("MUTEX_unlock");
extern void func_003E0270_0D80(char *, int) __asm__("func_003E0270");

int func_003E0D80(int h, int *p, int n, int key) {
    char *a;
    int b;
    char *o;
    int *q;
    int t, f;
    if (func_003DFBD0_0D80(h, &a, &b) != 0) return 0;
    o = func_003DFCD8_0D80(a);
    if (o == 0) return 0;
    if (n == 0) {
        q = p;
        if (*q != key) {
            do {
                t = q[1];
                q = (int *)((char *)q + t);
                n += t;
            } while (*q != key);
        }
        n += q[1];
    }
    *(int *)(o + 0x118) = n;
    *(int **)(o + 0x114) = p;
    *(int *)(o + 0x11C) = key;
    *(int *)(o + 0x10) = 1;
    func_003DFD58_0D80(a, o);
    MUTEX_lock_0D80(a + 4);
    f = *(int *)(a + 0x38);
    if (f == 0) *(int *)(a + 0x38) = 1;
    MUTEX_unlock_0D80(a + 4);
    if (f == 0) func_003E0270_0D80(a, 0);
    return *(int *)o;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E0E90);

INCLUDE_ASM("ealib/seg_2B4578", func_003E1110);

INCLUDE_ASM("ealib/seg_2B4578", func_003E12E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E13E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int fFBD0_13E8(int, void *, void *) __asm__("func_003DFBD0");
extern void fFC48_13E8(char *, int) __asm__("func_003DFC48");
extern void fMLK_13E8(char *) __asm__("MUTEX_lock");
extern void fMUL_13E8(char *) __asm__("MUTEX_unlock");
extern void f03D0_13E8(char *, int) __asm__("func_003E03D0");

void func_003E13E8(int a0, unsigned int a1) {
    char *sp0;
    int sp4;
    int t;
    if (fFBD0_13E8(a0, &sp0, &sp4) == 0 && a1 >= *(unsigned int *)(sp0 + 0x30)
        && (unsigned int)(*(int *)(sp0 + 0x34) - 8) >= a1 && *(int *)a1 != -2) {
        *(int *)a1 = -2;
        fFC48_13E8(sp0, *(int *)(a1 + 4));
        fMLK_13E8(sp0 + 4);
        t = *(int *)(sp0 + 0x38);
        if (t == 2) *(int *)(sp0 + 0x38) = 1;
        fMUL_13E8(sp0 + 4);
        if (t == 2) {
            if (*(int *)(sp0 + 0x48) != 0) f03D0_13E8(sp0, *(int *)(sp0 + 0x40));
            else f03D0_13E8(sp0, *(int *)(sp0 + 0x3C));
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E15E8);
#ifdef SKIP_ASM
struct G_15E8 { char p0[4]; int mtx; char p1[0x48]; int f50; char p2[8]; int *f5c; };
struct P_15E8 { int p0; int f4; char p1[0x118]; int f120; };
struct C_15E8 { int *p0; int *p1; };
struct LL_15E8 { int w0; int p1; int p2; struct P_15E8 *next; };
struct R_15E8 { struct G_15E8 *g; int *s; };
extern int func_003DFC08();

int func_003E15E8(int a) {
    struct R_15E8 r;
    struct LL_15E8 *cur;
    int n, id, q;
    struct P_15E8 *p;
    if (func_003DFBD0(a, &r.g, &r.s)) return 0;
    if (r.s[2] == 0) return 0;
    MUTEX_lock((char *)r.g + 4);
    cur = (struct LL_15E8 *)r.g->f5c;
    n = r.g->f50;
    id = r.s[3];
    while ((p = cur->next) != 0) {
        if (p->f4 == 1) break;
        q = p->f120;
        if (func_003DFC08(n, q, id)) break;
        cur = cur->next;
        n = q;
    }
    MUTEX_unlock((int *)((char *)r.g + 4));
    return cur->w0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E1728);
#ifdef SKIP_ASM
extern int func_003DEBF0(int, int, int, void *);

int func_003E1728(int a0, int *a1) {
    int sp0;
    int r;
    if (func_003DEBF0(*a1, 1, a0, &sp0) != 0) {
        a0 = a0 - 1;
        r = func_003DECF8(sp0, a0);
        func_003DECA0(sp0, a0);
        return r;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E1810);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00450C14_1810[])(int, int, int) __asm__("D_00450C14");

int func_003E1810(int a, int *b) {
    int sp0;
    int r, c;
    if (func_003DEBF0(b[0], 1, a, &sp0)) {
        int m = a - 1;
        r = func_003DECF8(sp0, m);
        c = D_00450C14_1810[0](b[0], r, b[4]);
        if (c) {
            func_003DEC60(sp0, 0, c, r, m);
            func_003DECA0(sp0, m);
            return c;
        }
        func_003DECA0(sp0, m);
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E1948);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*D_00450C14_1948[])(int, int, int) __asm__("D_00450C14");

int func_003E1948(int a, int *b) {
    int sp0;
    int r, c;
    if (func_003DEBF0(b[0], 1, a, &sp0)) {
        int m = a - 1;
        r = func_003DECF8(sp0, m);
        c = D_00450C14_1948[0](b[0], r, b[4]);
        if (c) {
            func_003DEC60(sp0, 0, c, r, m);
            func_003DECA0(sp0, m);
            b[2] = r;
            return c;
        }
        func_003DECA0(sp0, m);
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E1C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003E1908_1C00(int, int) __asm__("func_003E1908");

int func_003E1C00(void *arg0, void *arg1) {
    int temp_16;
    int var_18;
    int var_19;

    var_18 = 0;
    var_19 = 1;
    if ((*(int *)((char*)(arg1) + (0))) > 0) {
        do {
            temp_16 = var_18 * 4;
            var_18 += 1;
            *(int *)(temp_16 + (*(int *)((char*)(arg1) + (8)))) = func_003E1908_1C00(*(int *)(temp_16 + (*(int *)((char*)(arg1) + (4)))), (*(int *)((char*)(arg1) + (0xC))));
            var_19 = (*(int *)(temp_16 + (*(int *)((char*)(arg1) + (8)))) == 0) ? 0 : var_19;
        } while (var_18 < (*(int *)((char*)(arg1) + (0))));
    }
    return var_19;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E1F48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_00450C50_1F48[] __asm__("D_00450C50");
extern int func_00416B18_1F48(char *, char *, int) __asm__("func_00416B18");
extern void func_004162D0_1F48(char *, char *) __asm__("func_004162D0");
extern int func_00427D60_1F48(char *, int) __asm__("func_00427D60");
extern void func_00427FE8_1F48(int) __asm__("func_00427FE8");
extern void func_00428600_1F48(int, char *, int) __asm__("func_00428600");

unsigned int func_003E1F48(char *name, char *data, int len) {
    char buf[256];
    int fd;
    buf[0] = 0;
    if (func_00416B18_1F48(name, D_00450C50_1F48[0], strlen(D_00450C50_1F48[0])) != 0) {
        strcpy(buf, D_00450C50_1F48[0]);
    }
    func_004162D0_1F48(buf, name);
    fd = func_00427D60_1F48(buf, 0x602);
    if (fd >= 0) {
        do {
            if (len > 0x1000) {
                func_00428600_1F48(fd, data, 0x1000);
                len -= 0x1000;
                data += 0x1000;
            } else {
                func_00428600_1F48(fd, data, len);
                len = 0;
            }
        } while (len > 0);
        func_00427FE8_1F48(fd);
    }
    return (unsigned int)~fd >> 31;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2030);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_00450C50_2030[] __asm__("D_00450C50");
extern int func_00416B18_2030(char *, char *, int) __asm__("func_00416B18");
extern void func_004162D0_2030(char *, char *) __asm__("func_004162D0");
extern int func_00427D60_2030(char *, int) __asm__("func_00427D60");
extern int func_00427FE8_2030(int) __asm__("func_00427FE8");
extern int func_00428600_2030(int, char *, int) __asm__("func_00428600");

unsigned int func_003E2030(char *name, char *data, int len) {
    char buf[256];
    int fd;
    buf[0] = 0;
    if (func_00416B18_2030(name, D_00450C50_2030[0], strlen(D_00450C50_2030[0])) != 0) {
        strcpy(buf, D_00450C50_2030[0]);
    }
    func_004162D0_2030(buf, name);
    fd = func_00427D60_2030(buf, 0x602);
    if (fd >= 0) {
        do {
            if (len > 0x1000) {
                if (func_00428600_2030(fd, data, 0x1000) < 0) {
                    return 0;
                }
                len -= 0x1000;
                data += 0x1000;
            } else {
                if (func_00428600_2030(fd, data, len) < 0) {
                    return 0;
                }
                len = 0;
            }
        } while (len > 0);
        if (func_00427FE8_2030(fd) < 0) {
            return 0;
        }
    }
    return (unsigned int)~fd >> 31;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2490);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_00499C81_2490[] __asm__("D_00499C81");
static inline int low_2490(unsigned char *t, int c) {
    int r = c;
    if (*(unsigned char *)(c + (int)t) & 1) r = c + 0x20;
    return r;
}

int func_003E2490(signed char *a, signed char *b) {
    unsigned char *t = D_00499C81_2490;
    signed char *p = a;
    signed char *q = b;
    signed char c1 = *p;
    signed char c2 = *q;
    int d;
    while ((d = low_2490(t, c1) - low_2490(t, c2)) == 0 && c1 != 0) {
        q++;
        p++;
        c1 = *p;
        c2 = *q;
    }
    return d;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", BIG_typeofheader);
#ifdef SKIP_ASM
int BIG_typeofheader(unsigned char *p) {
    int r;
    unsigned char b0 = p[0];
    unsigned char b1 = p[1];
    unsigned int v;
    if (((b0 << 8) | b1) == 0xC0FB) {
        r = 1;
    } else {
        v = (b0 << 24) | (b1 << 16) | (p[2] << 8) | p[3];
        if (v == 0x42494746) {
            r = 2;
        } else {
            r = (((v & 0xFFFFFF00) ^ 0x42494700) != 0) ? 0 : 3;
        }
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E2E50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003E2DF0_2E50(int, signed char *) __asm__("func_003E2DF0");
extern signed char *strchr_2E50(signed char *, int) __asm__("strchr");

void func_003E2E50(int arg0, signed char *arg1, int *arg2) {
    signed char *temp_2;
    signed char *var_17 = arg1;
    do {
        temp_2 = strchr_2E50(var_17, 0x2C);
        if (temp_2 != 0) *temp_2 = 0;
        *arg2 = func_003E2DF0_2E50(arg0, var_17);
        arg2++;
        if (temp_2 == 0) break;
        *temp_2 = 0x2C;
        temp_2++; var_17 = temp_2;
    } while (temp_2 != 0);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3020);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SYNCTASK_add(void (*)(void), int, int);
extern void func_003E33B0(void);
extern int D_00450C44_3020[] __asm__("D_00450C44");
extern int D_00450C48_3020[] __asm__("D_00450C48");

void func_003E3020(int arg0, int arg1) {
    int temp_3;

    D_00450C48_3020[0] = 0;
    SYNCTASK_add(func_003E33B0, 0, 0);
    temp_3 = D_00450C48_3020[0];
    D_00450C48_3020[0] = temp_3 | 2;
    if (arg1 != 0) {
        D_00450C48_3020[0] = temp_3 | 3;
        D_00450C44_3020[0] = 1;
        return;
    }
    D_00450C44_3020[0] = 2;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E3478);
#ifdef SKIP_ASM
int func_003E3478(int a0, int a1, int a2, int a3) {
    int t = a0 & 0xFFFFFF;
    func_00428168(t, a2, 0);
    func_00428600(t, a1, a3);
    iFILESYS_CommandCompleteCallback(1);
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E48D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C5C_48D8[] __asm__("D_00450C5C");
extern char *D_00519C78_48D8[] __asm__("D_00519C78");
extern char *strncpy(char *, const char *, int);

int func_003E48D8_48D8(int, char *, int *, int *) __asm__("func_003E48D8");

int func_003E48D8_48D8(int arg0, char *arg1, int *arg2, int *arg3) {
    char *e;
    e = D_00519C78_48D8[0] + (arg0 - 1) * (D_00450C5C_48D8[0] + 0xC);
    if (arg1 != 0) {
        strncpy(arg1, e + 0xC, D_00450C5C_48D8[0]);
    }
    if (arg2 != 0) {
        *arg2 = *(int *)(e + 8);
    }
    if (arg3 != 0) {
        *arg3 = *(int *)(e + 4);
    }
    return *(int *)(e + 8);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E4A30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C78_4A30[] __asm__("D_00450C78");

void func_003E4A30(int a) {
    int i;
    for (i = 0; i < 0x40; i++) {
        if (D_00450C78_4A30[i] == a) return;
    }
    i = 0;
    while (i < 0x40) {
        if (D_00450C78_4A30[i] == 0) {
            D_00450C78_4A30[i] = a;
            break;
        }
        i++;
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E51A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00423C00_51A0(int) __asm__("func_00423C00");
extern int D_00450DE4_51A0[] __asm__("D_00450DE4");

void func_003E51A0_51A0(char *arg0) __asm__("func_003E51A0");
void func_003E51A0_51A0(char *arg0) {
    int h;
    int st[12];

    if (arg0 == (char*)0xFFFFFFFF) {
        h = func_00423C90();
    } else {
        h = *(int*)(arg0 + 4);
    }
    if (D_00450DE4_51A0[0] != 0) {
        func_00423CB0(h, st);
    } else {
        func_00423CA0(h, st);
    }
    if (st[0] & 0xF) {
        if (D_00450DE4_51A0[0] != 0) {
            func_00423C00_51A0(h);
        } else {
            func_00423BF0(h);
        }
        func_00423BB0(h);
        return;
    }
    if (st[0] & 0x10) {
        func_00423BB0(h);
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", THREAD_yieldticks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void abm_YT(char *) __asm__("REAL_abortmessage");
extern int f5008_YT(void) __asm__("func_003E5008");
extern int f3B20_YT(int, char *, int) __asm__("func_00423B20");
extern void f3C50_YT(int) __asm__("func_00423C50");
extern int f3C90_YT(void) __asm__("func_00423C90");
extern void f3CA0_YT(int, int *) __asm__("func_00423CA0");
extern void f3CB0_YT(int, int *) __asm__("func_00423CB0");
extern void f3CC0_YT(void) __asm__("func_00423CC0");
extern void f4CE8_YT(int) __asm__("func_00424CE8");
extern int D_0DA0_YT[] __asm__("D_00450DA0");
extern int D_0DE4_YT[] __asm__("D_00450DE4");
extern char D_5E20_YT[] __asm__("D_00495E20");
extern char fE5290_YT[] __asm__("func_003E5290");

void THREAD_yieldticks(int a0) {
    int buf[12];
    int t16;
    if (a0 > 0) {
        t16 = ((a0 * D_0DA0_YT[0]) / f5008_YT()) & 0xFFFF;
        if (f3B20_YT(t16, fE5290_YT, f3C90_YT()) < 0) {
            abm_YT(D_5E20_YT);
        }
        f3CC0_YT();
        return;
    }
    if (D_0DE4_YT[0] != 0) {
        f3CB0_YT(f3C90_YT(), buf);
        f4CE8_YT(buf[6]);
        return;
    }
    f3CA0_YT(f3C90_YT(), buf);
    f3C50_YT(buf[6]);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5498);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_00423C90();
extern void func_00423CA0(int, void *);
extern void func_00423CB0(int, void *);
extern int D_00450DE4_5498[] __asm__("D_00450DE4");
extern int D_00450DFC_5498[] __asm__("D_00450DFC");

int func_003E5498(int *a0) {
    int sp[12];
    int t;
    if (a0 == 0) {
        t = D_00450DFC_5498[0];
    } else if (a0 == (int *)-1) {
        t = func_00423C90();
    } else {
        t = a0[1];
    }
    if (D_00450DE4_5498[0] != 0) {
        func_00423CB0(t, sp);
    } else {
        func_00423CA0(t, sp);
    }
    return sp[6];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5508);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00423C30_5508(int, int) __asm__("func_00423C30");
extern void func_00423C40_5508(int, int) __asm__("func_00423C40");
extern int func_00423C90_5508(void) __asm__("func_00423C90");
extern int D_00450DE4_5508[] __asm__("D_00450DE4");
extern int D_00450DFC_5508[] __asm__("D_00450DFC");

int func_003E5508(void *arg0, int arg1) {
    int var_4;

    if (arg0 == 0) {
        var_4 = D_00450DFC_5508[0];
    } else if (arg0 == (void *)0xFFFFFFFF) {
        var_4 = func_00423C90_5508();
    } else {
        var_4 = (*(int *)((char*)(arg0) + (4)));
    }
    if (D_00450DE4_5508[0] != 0) {
        func_00423C40_5508(var_4, arg1);
    } else {
        func_00423C30_5508(var_4, arg1);
    }
    return 1;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E55E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void THREAD_yieldticks_55E0(int) __asm__("THREAD_yieldticks");
extern int func_003E4E98_55E0() __asm__("func_003E4E98");
extern void func_00423CA0_55E0(int, int *) __asm__("func_00423CA0");

int func_003E55E0(int *arg0, int arg1) {
    int sp[12];
    int temp_16;
    int temp_18;

    temp_18 = func_003E4E98_55E0() + arg1;
    temp_16 = arg0[1];
    do {
        func_00423CA0_55E0(temp_16, sp);
        if ((sp[0] & 0x10) || sp[0] == 0) break;
        THREAD_yieldticks_55E0(0);
    } while (arg1 == 0 || temp_18 < func_003E4E98_55E0());
    return sp[0] == 0x10;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", MUTEX_unlock);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_00423C90();
extern void func_00423DC0(int);
extern void func_00423DD0(int);
extern int D_00450DE4_5760[] __asm__("D_00450DE4");

void MUTEX_unlock_5760(void *arg0) __asm__("MUTEX_unlock");
void MUTEX_unlock_5760(void *arg0) {
    int *p = (int *)arg0;
    if (p[1] == func_00423C90()) {
        if (--p[2] != 0) return;
        p[1] = 0;
    }
    if (D_00450DE4_5760[0] != 0) {
        func_00423DD0(p[3]);
    } else {
        func_00423DC0(p[3]);
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E5AA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_5AA8 { int a; unsigned int b0 : 1; unsigned int b1 : 1; unsigned int rest : 30; };
extern struct E_5AA8 D_00450E08_5AA8[] __asm__("D_00450E08");

void func_003E5AA8(int idx, int val) {
    int i;
    if (idx == 0) {
        for (i = 0; i < 64; i++) {
            D_00450E08_5AA8[i].b0 = (val != 0);
        }
    } else if (idx == 1) {
        for (i = 0; i < 64; i++) {
            if (D_00450E08_5AA8[i].b1) {
                D_00450E08_5AA8[i].b0 = (val != 0);
            }
        }
    } else {
        D_00450E08_5AA8[idx].b0 = (val != 0);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5C78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_5C78 { int a; unsigned int b0 : 1; unsigned int rest : 31; };
struct H_5C78 { int a; void (*fn)(int, char *); unsigned int b0 : 1; unsigned int rest : 31; };
extern int D_00451008_5C78[] __asm__("D_00451008");
extern struct E_5C78 D_00450E08_5C78[] __asm__("D_00450E08");
extern struct H_5C78 D_00451250_5C78[] __asm__("D_00451250");

void func_003E5C78_5C78(int, char *, char *) __asm__("func_003E5C78");
void func_003E5C78_5C78(int idx, char *fmt, char *ap) {
    char buf[0x2000];
    int h;
    if (D_00451008_5C78[0] == 0) {
        func_003E5B60();
    }
    if (D_00450E08_5C78[idx].b0) {
        func_004186C8(buf, fmt, ap);
        h = (int)D_00451250_5C78;
        do {
            if (((struct H_5C78 *)h)->b0) {
                if (((struct H_5C78 *)h)->fn) {
                    ((struct H_5C78 *)h)->fn(idx, buf);
                }
            }
            h += 12;
        } while (h < (int)D_00451250_5C78 + 0x60);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5DC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5DC8 { int a; int b; int c; };
extern void func_003E5A78(int, int);
extern void func_003E5B60(void);
extern int D_00451008_5DC8[] __asm__("D_00451008");
extern struct S_5DC8 D_00451250_5DC8[] __asm__("D_00451250");

void func_003E5DC8(int a0, int a1) {
    if (D_00451250_5DC8[a0].b != 0) {
        if (D_00451008_5DC8[0] == 0) {
            func_003E5B60();
        }
        func_003E5A78(a0, a1);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E5E88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5E88 { int a; int b; int c; };
extern void func_003E5B60(void);
extern struct S_5E88 D_00451250_5E88[] __asm__("D_00451250");
extern int D_00451008_5E88[] __asm__("D_00451008");

void func_003E5E88(int a0, int a1, int a2) {
    if (D_00451008_5E88[0] == 0) {
        func_003E5B60();
    }
    D_00451250_5E88[a0].a = a1;
    D_00451250_5E88[a0].b = a2;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", REAL_abortmessage);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define REAL_abortmessage_va_start(ap)                                   \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern int func_004186C8(char *dst, const char *fmt, char *ap);
extern void (*D_00450DD8_6070[])(char *, char *) __asm__("D_00450DD8");
extern char D_00495EE8_6070[] __asm__("D_00495EE8");

void REAL_abortmessage(const char *fmt, ...) {
    char buf[0x200];
    if (fmt) {
        char *ap;
        REAL_abortmessage_va_start(ap);
        func_004186C8(buf, fmt, ap);
    } else {
        buf[0] = 0;
    }
    D_00450DD8_6070[0](D_00495EE8_6070, buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SYSTEM_abortmessage);
#ifdef SKIP_ASM
extern void func_003E4A10(void);
extern void func_003E5D30_60E8(int, char *, ...) __asm__("func_003E5D30");
extern int func_004186C8_60E8(char *, char *, char *) __asm__("func_004186C8");
extern char D_00495EF0[];
extern char D_00495F00[];
extern char *D_0045100C_100C[] __asm__("D_0045100C");
extern int D_00451010_1010[] __asm__("D_00451010");

void SYSTEM_abortmessage(char *fmt, ...) {
    char buf[512];
    // PORT: hand-rolled EE EABI va_start
    if (fmt != 0) {
        func_004186C8_60E8(buf, fmt, (char *)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0));
    } else {
        buf[0] = 0;
    }
    func_003E5D30_60E8(2, D_00495EF0, buf);
    if (D_0045100C_100C[0] != 0) {
        func_003E5D30_60E8(2, D_00495F00, D_0045100C_100C[0], D_00451010_1010[0]);
    }
    func_003E4A10();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003E6574_6188(unsigned char *, unsigned char *, int) __asm__("func_003E6574");
void func_003E6188_6188(unsigned char *dst, unsigned char *src, int n) __asm__("func_003E6188");

void func_003E6188_6188(unsigned char *dst, unsigned char *src, int n) {
    unsigned char *e;
    if (src >= dst || dst >= (e = src + n)) {
        func_003E6574_6188(dst, src, n);
    } else {
        dst += n;
        n--;
        src = e;
        while (n != -1) {
            src--;
            dst--;
            *dst = *src;
            n--;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6240);
#ifdef SKIP_ASM
int func_003E6240(unsigned int a) {
    int r = 0;
    if (a - 0x100000 <= 0x7EFFFFF || a - 0x10000000 <= 0x4000000 ||
        a - 0x20100000 <= 0x7EFFFFF || a - 0x30100000 <= 0x7EFFFFF ||
        a - 0x70000000 < 0x4000) {
        r = 1;
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E67F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0051EE98_67F8[] __asm__("D_0051EE98");
extern void func_00256BD8(int *);
extern void func_003F4C68(int *, int, int, int, int, int, int, int, int, int);
extern void func_003F4C98(int *, int, int);

void func_003E67F8(int *arg0) {
    func_003F4C98(D_0051EE98_67F8, 0x676D6C6B, 0x64657374);
    func_003F4C68(D_0051EE98_67F8, 0, *arg0, 0, 0, 0, 0, 0, 1, 0);
    func_00256BD8(arg0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E6878);

INCLUDE_ASM("ealib/seg_2B4578", func_003E6958);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6A70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char b[4]; } U4_6A70;
extern void func_0041605C_6A70(short *, int, int) __asm__("func_0041605C");

int func_003E6A70(char *arg0, short **arg1) {
    if (arg1 != 0) {
        int t = *(int *)(arg0 + 0x160);
        if (t < *(int *)(arg0 + 0x164)) {
            *(U4_6A70 *)*arg1 = *(U4_6A70 *)(((*(int *)(arg0 + 0x16C) + t) & 0x0FFFFFFF) | 0x20000000);
            func_0041605C_6A70(*arg1, ((*(int *)(arg0 + 0x16C) + *(int *)(arg0 + 0x160)) & 0x0FFFFFFF) | 0x20000000, **arg1);
        } else {
            *arg1 = 0;
        }
    }
    return *(int *)(arg0 + 0x164) - *(int *)(arg0 + 0x160);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E6FC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003F4C68_6FC8(void *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
extern void func_003F4C98_6FC8(void *, int, int) __asm__("func_003F4C98");
struct S_6FC8 { int a[4]; int out; };

int func_003E6FC8(int arg0) {
    struct S_6FC8 s;

    func_003F4C98_6FC8(&s, 0x676D7574, 0x636F6D70);
    func_003F4C68_6FC8(&s, 1, arg0, 0, 0, 0, 0, 0, 1, &s.out);
    return s.out;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7120);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003F4C68_7120(int *, int, int, int, char *, int, int, int, int, int) __asm__("func_003F4C68");
extern int func_003F4C98_7120(int *, int, int) __asm__("func_003F4C98");
extern int sprintf_7120(char *, int *, int, int) __asm__("sprintf");
extern int D_004A4860_7120[] __asm__("D_004A4860");

void func_003E7120(int arg0, int arg1, int arg2) {
    char sp10[256];
    int sp110[4];

    sprintf_7120(sp10, D_004A4860_7120, arg1, arg2);
    func_003F4C98_7120(sp110, 0x676D7574, 0x61647674);
    func_003F4C68_7120(sp110, 1, arg0, 0, sp10, -1, 0, 0, 1, 0);
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E73E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00256BD8_73E0(void *) __asm__("func_00256BD8");
extern int func_003F4C68_73E0(void *, int, int, int, int, int, int, int, int, int) __asm__("func_003F4C68");
extern void func_003F4C98_73E0(void *, int, int) __asm__("func_003F4C98");
struct S_73E0 { int a[4]; };

void func_003E73E0(void *arg0) {
    struct S_73E0 s;

    func_003F4C98_73E0(&s, 0x70696E67, 0x64657374);
    func_003F4C68_73E0(&s, 0, *(int *)((char*)arg0 + 0xC), 0, 0, 0, 0, 0, 1, 0);
    func_00256BD8_73E0(arg0);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E7450);

INCLUDE_ASM("ealib/seg_2B4578", func_003E74E8);

INCLUDE_ASM("ealib/seg_2B4578", func_003E7500);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E76A8);
#ifdef SKIP_ASM
extern void func_004186C8_76A8(char *, const char *, char *) __asm__("func_004186C8");

// PORT: SN-specific va_start
void func_003E76A8(char *self, const char *fmt, ...) {
    char buf[0x1000];
    char *ap;
    void (*cb)(int, char *);
    ap = (char*)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    func_004186C8_76A8(buf, fmt, ap);
    cb = *(void (**)(int, char *))(self + 0x548);
    if (cb != 0) {
        cb(*(int *)(self + 0x544), buf);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7718);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *func_002591E8_7718(int) __asm__("func_002591E8");

void *func_003E7718(char *arg0, int arg1, int arg2, int arg3) {
    char **pp;
    char *n;
    char *cur;
    char *nx;
    char *h;

    n = func_002591E8_7718(0x44);
    if (n != 0) {
        *(int*)(n + 4) = arg1;
        *(int*)(n + 0x38) = arg2;
        *(int*)(n + 0x3C) = arg3;
        pp = (char**)(arg0 + 0x53C);
        *(char**)(n + 0x40) = 0;
        *(int*)n = *(int*)(arg0 + 0x54C);
        h = *(char**)(arg0 + 0x53C);
        *(int*)(arg0 + 0x54C) = (*(int*)(arg0 + 0x54C) + 1) & 0x7FFFFFFF;
        if (h != 0 && *(int*)(h + 4) != 0x70696E67) {
            do {
                cur = *pp;
                nx = *(char**)(cur + 0x40);
                pp = (char**)(cur + 0x40);
            } while (nx != 0 && *(int*)(nx + 4) != 0x70696E67);
        }
        cur = *pp;
        *pp = n;
        *(char**)(n + 0x40) = cur;
    }
    return n;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003E77F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7918);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct N_7918 { int f0; int tag; int id; int fC; int f10; int f14; char name[0x28]; struct N_7918 *next; };
struct L_7918 { char pad[0x53C]; struct N_7918 *head; int cnt; };
extern int func_004165A8_7918(char *, char *) __asm__("func_004165A8");
extern void func_00259210_7918(void *) __asm__("func_00259210");

int func_003E7918(struct L_7918 *l, struct N_7918 *out, char *name, int id, int t) {
    struct N_7918 **pp = &l->head;
    struct N_7918 *n;

    while (*pp != 0) {
        n = *pp;
        if (n->tag == 0x70696E67 &&
            ((n->id == id && func_004165A8_7918(name, n->name) == 0) || (id == -1 && n->fC < t))) {
            *pp = n->next;
            *out = *n;
            func_00259210_7918(n);
            l->cnt--;
            return 1;
        }
        pp = &(*pp)->next;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7B00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003EE3E0(int, int, void *);
extern void *func_003EE468(int, int);
extern int func_003EEEE8(int);
extern void sprintf(void *, int *, int);
extern int D_004A4870_7B00[] __asm__("D_004A4870");

void func_003E7B00(void *arg0, int arg1, int arg2) {
    int temp_2;
    int temp_4;
    void *temp_2_2;

    temp_4 = (*(int *)((char*)(arg0) + (0x310)));
    if (temp_4 != 0) {
        temp_2 = func_003EEEE8(temp_4);
        if (temp_2 != 0) {
            temp_2_2 = func_003EE468((*(int *)((char*)(arg0) + (0x314))), temp_2);
            if ((temp_2_2 != 0) && (arg2 != (*(int *)((char*)(temp_2_2) + (0x10))))) {
                (*(int *)((char*)(temp_2_2) + (0x10))) = arg2;
                sprintf(temp_2_2 + 0x14, D_004A4870_7B00, arg2);
                func_003EE3E0((*(int *)((char*)(arg0) + (0x314))), temp_2, temp_2_2);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E7F70);
#ifdef SKIP_ASM
void func_003E7F70(char *arg0, int arg1, int arg2) {
    int sp[4];
    int (*fn)(char *, int *, int);

    *(int *)(arg0 + 0xC) = 0x7465726D;
    *(int *)(arg0 + 8) |= arg2;
    func_003E7F58(arg0);
    fn = *(int (**)(char *, int *, int))(arg0 + 0x52C);
    if (fn != 0) {
        sp[0] = 3; sp[1] = 0x64697363; sp[2] = arg1; sp[3] = 0;
        fn(arg0, sp, *(int *)(arg0 + 0x530));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003E7FF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_004A48A8_7FF0[] __asm__("D_004A48A8");
extern void *func_002591E8_7FF0(int) __asm__("func_002591E8");
extern void func_00416210_7FF0(void *, int, int) __asm__("func_00416210");
extern char *strchr_7FF0(char *, int) __asm__("strchr");
extern void cDirtysock_tag_TagFieldDupl_7FF0(void *, int, char *) __asm__("cDirtysock_tag_TagFieldDupl");
extern void cDirtysock_tag_TagFieldSetString_7FF0(void *, int, char *, char *) __asm__("cDirtysock_tag_TagFieldSetString");
extern int func_003F5478_7FF0(int) __asm__("func_003F5478");
extern int func_003E7340_7FF0(void) __asm__("func_003E7340");
extern void func_003E80C0_7FF0(void *) __asm__("func_003E80C0");

void *func_003E7FF0(char *arg0, int arg1, int arg2) {
    char *r;
    int t;

    r = func_002591E8_7FF0(0x550);
    if (r != 0) {
        func_00416210_7FF0(r, 0, 0x550);
        if (arg0 != 0) {
            if (strchr_7FF0(arg0, 0x3D) != 0) {
                cDirtysock_tag_TagFieldDupl_7FF0(r + 0x34, 0x100, arg0);
            } else {
                cDirtysock_tag_TagFieldSetString_7FF0(r + 0x34, 0x100, D_004A48A8_7FF0, arg0);
            }
        }
        *(int*)r = func_003F5478_7FF0(0x8000);
        t = func_003E7340_7FF0();
        *(int*)(r + 0x544) = arg1;
        *(int*)(r + 0x548) = arg2;
        *(int*)(r + 4) = t;
        func_003E80C0_7FF0(r);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E80C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00259210_80C0(void *) __asm__("func_00259210");
extern void func_003E7F58_80C0(void *) __asm__("func_003E7F58");
extern void *func_003EE480_80C0(int, int) __asm__("func_003EE480");
extern int func_003EE9A0_80C0(int) __asm__("func_003EE9A0");
extern void func_003F5738_80C0(int) __asm__("func_003F5738");

struct E_80C0 { int a, b, c, d, e; };
struct S_80C0 { int f0, f4, f8, fC; char pad[0x310 - 0x10]; struct E_80C0 e[8]; char pad2[0x53C - 0x3B0]; char *f53C; int f540; char pad3[0x54C - 0x544]; int f54C; };

void func_003E80C0(struct S_80C0 *s) {
    int i, t;
    char *p;
    s->fC = 0x6F66666C;
    func_003F5738_80C0(s->f0);
    for (i = 0; i < 8; i++) {
        if (s->e[i].a != 0) {
            while ((t = func_003EE9A0_80C0(s->e[i].a)) != 0) {
                func_00259210_80C0(func_003EE480_80C0(s->e[i].b, t));
            }
        }
    }
    while (s->f53C != 0) {
        p = s->f53C;
        s->f53C = *(char **)(p + 0x40);
        func_00259210_80C0(p);
    }
    func_003E7F58_80C0(s);
    s->f540 = 0;
    s->f54C = 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8260);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cDirtysock_tag_TagFieldDupl_8260(void *, int, void *) __asm__("cDirtysock_tag_TagFieldDupl");
extern void func_003E76A8_8260(void *, int *, int, int) __asm__("func_003E76A8");
extern int func_003E7718_8260(void *, int, int, int) __asm__("func_003E7718");
extern void func_003E7AC0_8260() __asm__("func_003E7AC0");
extern void func_003E7F58_8260(void *) __asm__("func_003E7F58");
extern void func_003F5638_8260(int, int, int) __asm__("func_003F5638");
extern void func_003F5738_8260(int) __asm__("func_003F5738");
extern int func_003F5910_8260(int, int, int, void *, int) __asm__("func_003F5910");
extern int func_003F5D88_8260(int) __asm__("func_003F5D88");
extern int D_00495F30_8260[] __asm__("D_00495F30");

struct S_8260 { int f0, f4, f8, fC, f10, f14; char pad[0x34 - 0x18]; char f34[0x3B0 - 0x34]; int f3B0, f3B4; char f3B8; char pad2[0x50C - 0x3B9]; int f50C; };

int func_003E8260(struct S_8260 *s, int a1, int a2, int a3, int a4) {
    char buf[0x100];
    s->f3B4 = a1;
    s->f3B0 = a2;
    func_003E7AC0_8260();
    s->f50C = 0;
    func_003E7F58_8260(s);
    if (func_003E7718_8260(s, 0x736B6579, a3, a4) == 0) {
        return -1;
    }
    func_003F5738_8260(s->f0);
    func_003F5638_8260(s->f0, s->f3B4, s->f3B0);
    func_003E76A8_8260(s, D_00495F30_8260, s->f3B4, s->f3B0);
    cDirtysock_tag_TagFieldDupl_8260(buf, 0x100, s->f34);
    func_003F5910_8260(s->f0, 0x40646972, 0, buf, -1);
    s->fC = 0x72646972;
    s->f10 = func_003F5D88_8260(s->f0) + 0x1D4C0;
    s->f14 = func_003F5D88_8260(s->f0);
    s->f3B8 = 0;
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8A70);
#ifdef SKIP_ASM
char *func_003E8A70(char *p, int k) {
    if (k == 0x75736572 || k == 0x61636374) return p + 0x134;
    if (k == 0x70657273) return p + 0x174;
    if (k == 0x726F6F6D) return p + 0x1C0;
    if (k == 0x706C6179) return (*(int *)(p + 0x2CC) == 0) ? 0 : p + 0x200;
    if (k == 0x646F776E) return (*(char *)(p + 0x3B8) == 0) ? 0 : p + 0x3B8;
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E8F08);
#ifdef SKIP_ASM
int func_003E8F08(char *arg0, unsigned arg1) {
    int a, b;
    if (arg1 == 0x73746174) return *(int *)(arg0 + 0xC);
    if (arg1 == 0x72666C67) return *(int *)(arg0 + 0x1BC);
    if (arg1 == 0x6578746E) return *(int *)(arg0 + 0x4FC);
    if (arg1 == 0x6C6F636C) return *(int *)(arg0 + 0x500);
    if (arg1 == 0x696E6174) {
        a = *(int *)(arg0 + 0x4FC);
        if (a != 0) {
            b = *(int *)(arg0 + 0x500);
            if (b != 0)
                return a != b;
        }
        return 0;
    }
    if (arg1 == 0x6D6F7265) return *(int *)(arg0 + 0x504);
    if (arg1 == 0x736C6F74) return *(int *)(arg0 + 0x510);
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003E94F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cDirtysock_tag_TagFieldFind_94F0(int, char *) __asm__("cDirtysock_tag_TagFieldFind");
extern int cDirtysock_tag_TagFieldGetNumber_94F0(int, int) __asm__("cDirtysock_tag_TagFieldGetNumber");
extern void func_003E76A8_94F0(void *, char *) __asm__("func_003E76A8");
extern int func_003F4330_94F0(void) __asm__("func_003F4330");
extern char D_00495F98_94F0[] __asm__("D_00495F98");
extern char D_004A4898_94F0[] __asm__("D_004A4898");

void func_003E94F0(void *arg0, void *arg1) {
    int n;
    if (*(int *)((char *)arg1 + 4) == 0x70696E67) {
        n = cDirtysock_tag_TagFieldGetNumber_94F0(cDirtysock_tag_TagFieldFind_94F0(*(int *)((char *)arg1 + 0xC), D_004A4898_94F0), 0);
        if (n == 0) {
            func_003E76A8_94F0(arg0, D_00495F98_94F0);
            *(int *)((char *)arg0 + 0x20) += 1;
            if (*(int *)((char *)arg0 + 0x20) == 3) {
                *(int *)((char *)arg0 + 0x20) = 0;
            } else {
                *(int *)((char *)arg0 + 0x18) = func_003F4330_94F0();
            }
        } else {
            *(int *)((char *)arg0 + 0x1C) = n;
            *(int *)((char *)arg0 + 0x18) = func_003F4330_94F0() + 0x7530;
            *(int *)((char *)arg0 + 0x20) = 0;
        }
    }
}
#endif

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
