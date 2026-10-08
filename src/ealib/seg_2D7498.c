#include "common.h"

INCLUDE_ASM("ealib/seg_2D7498", func_003D6498);

INCLUDE_ASM("ealib/seg_2D7498", func_003D6618);

INCLUDE_ASM("ealib/seg_2D7498", func_003D66C0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D66F0);
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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", func_003D6748);
#ifdef SKIP_ASM
extern void func_003D6498(int, int);

void func_003D6748(int a) {
    func_003D6498(a, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6768);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFA8_6768[4] __asm__("D_0044FFA8");

int *func_003D6768(void) {
    return D_0044FFA8_6768;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D6778);

INCLUDE_ASM("ealib/seg_2D7498", func_003D67F8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D6840);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6940);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6968);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6988);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D69A8);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D69F0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D6BC0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6C58);
#ifdef SKIP_ASM
int func_003D6C58(int arg0) {
    return arg0 * 4;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6C60);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6CA0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6CC0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6CF8);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D6D30);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

void func_003D6D30(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6D40);
#ifdef SKIP_ASM
extern int D_004A4824;
extern int D_004A4828;

void func_003D6D40(void) {
    D_004A4824 = 0;
    D_004A4828 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6D50);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D6DB8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D6E30);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D6EE8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D6F80);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D7038);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D70B0);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D7110);

INCLUDE_ASM("ealib/seg_2D7498", func_003D71D8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D72A0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7418);

INCLUDE_ASM("ealib/seg_2D7498", func_003D76F0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7760);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7A50);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7B98);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7C38);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7D58);

INCLUDE_ASM("ealib/seg_2D7498", func_003D7EC8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D8008);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D8028);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044FFF8_8028[128] __asm__("D_0044FFF8");

int func_003D8028(int arg0) {
    return D_0044FFF8_8028[arg0];
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003D8040);

INCLUDE_ASM("ealib/seg_2D7498", func_003D80B0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D8158);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004506A0_8158[128] __asm__("D_004506A0");

void func_003D8158(int arg0, int arg1) {
    D_004506A0_8158[arg1] = arg0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", func_003D8170);
#ifdef SKIP_ASM
extern void func_003D7EC8(int);

void func_003D8170(void) {
    func_003D7EC8(0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2D7498", func_003D8190);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D7EC8_8190(int) __asm__("func_003D7EC8");
extern void func_003D9BD8(int);
extern int func_003DA3F0(int);
extern int D_004A484C_484C __asm__("D_004A484C");

void func_003D8190(void) {
    if (D_004A484C_484C == 0) {
        D_004A484C_484C = 1;
        if (func_003DA3F0(0) != 0) {
            func_003D9BD8(0);
        } else if (func_003D7EC8_8190(0) != 0) {
            func_003D9BD8(0);
        }
    }
    D_004A484C_484C = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D81F0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D8278);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D82B0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D8330);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D83B8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D8460);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D84A0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D8500);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003D8530);
#ifdef SKIP_ASM
void func_003D8530(void *arg0) {
    *(int *)((char *)arg0 + 0) = 0;
    *(int *)((char *)arg0 + 4) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D8540);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D8598);

INCLUDE_ASM("ealib/seg_2D7498", func_003D85C8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D8780);

INCLUDE_ASM("ealib/seg_2D7498", func_003D88C8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D8CB0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D8E58);

INCLUDE_ASM("ealib/seg_2D7498", func_003D9088);

INCLUDE_ASM("ealib/seg_2D7498", func_003D93C0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D95B8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D9830);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003D9A40);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003D9AC8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D9BD8);

INCLUDE_ASM("ealib/seg_2D7498", func_003D9ED0);

INCLUDE_ASM("ealib/seg_2D7498", func_003D9FB0);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA3A8);
#ifdef SKIP_ASM
void func_003DA3A8(void *arg0, int *arg1, int *arg2) {
    *arg1 = (int) (*(unsigned char *)((char*)(arg0) + (8)));
    *arg2 = (int) (*(unsigned char *)((char*)(arg0) + (9)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA3C0);
#ifdef SKIP_ASM
extern int D_004A483C;

void func_003DA3C0(int arg0) {
    D_004A483C = arg0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA3C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_004506E0_A3C8[] __asm__("D_004506E0");
unsigned int func_003DA3C8(unsigned int a) {
    a &= 0xFF;
    return (a & 0x1F) * D_004506E0_A3C8[a >> 5];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA3F0);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DA418);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA4E0);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DA578);

INCLUDE_ASM("ealib/seg_2D7498", func_003DA5C8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA6B0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DA718);
#ifdef SKIP_ASM
int func_003DA718(unsigned char *arg0, int arg1) {
    int temp_3 = arg1 / 8;
    unsigned char m = 1 << (arg1 - temp_3 * 8);
    return arg0[temp_3] & m;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA750);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DA810);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DA848);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA8E0);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DA910);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DA9C8);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DAA10);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DAA50);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DAAB0);

INCLUDE_ASM("ealib/seg_2D7498", func_003DAB58);

INCLUDE_ASM("ealib/seg_2D7498", func_003DAC20);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DACA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003DB5C0_ACA8(unsigned char, short) __asm__("func_003DB5C0");

int func_003DACA8(void *arg0) {
    (*(signed char *)((char*)(arg0) + (6))) = (signed char) ((*(unsigned char *)((char*)(arg0) + (4))) + func_003DB5C0_ACA8((*(unsigned char *)((char*)(arg0) + (5))), (*(short *)((char*)(arg0) + (0)))));
    return 1;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003DACE8);

INCLUDE_ASM("ealib/seg_2D7498", func_003DAE48);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DAFD0);
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

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DB008);
#ifdef SKIP_ASM
extern int func_003DB5C0(int, int);

int func_003DB008(unsigned char *arg0) {
    return func_003DB5C0(0x64, -1) < (int) arg0[1];
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003DB040);

INCLUDE_ASM("ealib/seg_2D7498", func_003DB208);

INCLUDE_ASM("ealib/seg_2D7498", func_003DB3B8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DB3E0);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DB440);
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

INCLUDE_ASM("ealib/seg_2D7498", func_003DB4D0);

INCLUDE_ASM("ealib/seg_2D7498", func_003DB5C0);

INCLUDE_ASM("ealib/seg_2D7498", func_003DB790);

INCLUDE_ASM("ealib/seg_2D7498", func_003DB838);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DB978);
#ifdef SKIP_ASM
struct S_B978 { int a; int b; int c; unsigned char r, g, bl, al; };
void func_003DB978(struct S_B978 *s) {
    s->b = -1; s->a = 0; s->c = 0; s->r = 0xFF; s->g = 0xFF; s->bl = 0xFF; s->al = 0xFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DB9A0);
#ifdef SKIP_ASM
extern int (*D_0044FFB0[])();

int func_003DB9A0(void) {
    int r = 0;
    if (D_0044FFB0[0] != 0) {
        r = D_0044FFB0[0]();
    }
    return r;
}
#endif

INCLUDE_ASM("ealib/seg_2D7498", func_003DB9D8);

//100%
INCLUDE_ASM("ealib/seg_2D7498", func_003DBAA8);
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
INCLUDE_ASM("ealib/seg_2D7498", func_003DBAC8);
#ifdef SKIP_ASM
int func_003DBAC8(unsigned short *arg0, void *arg1) {
    return *arg0 - (((*(unsigned char *)((char*)(arg1) + (1))) << 8) | (*(unsigned char *)((char*)(arg1) + (0))));
}
#endif
