#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F31F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *D_004A4B98;
extern void f2220(void *) __asm__("func_003F2220");
typedef struct { unsigned count; unsigned st[4]; unsigned char buf[64]; } sMd5;
void func_003F31F8(sMd5 *ctx, unsigned char *out, int n) {
    unsigned char c = 0x80;
    unsigned w = 0;
    int i;
    i = ctx->count & 0x3F;
    do {
        ctx->buf[i] = c;
        i++;
        c = 0;
        if (i == 0x40) {
            f2220(ctx);
            i = 0;
        }
    } while (i != 0x38);
    ctx->buf[0x3D] = 0;
    ctx->buf[0x38] = (unsigned char)ctx->count << 3;
    ctx->buf[0x3E] = 0;
    ctx->buf[0x3F] = 0;
    ctx->buf[0x39] = ctx->count >> 5;
    ctx->buf[0x3A] = ctx->count >> 13;
    ctx->buf[0x3B] = ctx->count >> 21;
    ctx->buf[0x3C] = ctx->count >> 29;
    f2220(ctx);
    for (i = 0; i < 16; i++) {
        if ((i & 3) == 0) w = ctx->st[i >> 2];
        if (n == 16) {
            *out = w;
            out++;
        } else if (n > 32) {
            *out = D_004A4B98[(w >> 4) & 0xF];
            out++;
            *out = D_004A4B98[w & 0xF];
            out++;
        }
        w >>= 8;
    }
    if (n > 32) {
        *out = 0;
    } else if (n != 16) {
        while (n > 0) {
            *out = 0;
            out++;
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F33B0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F3530);

INCLUDE_ASM("seg/seg_2F41F8", func_003F35D0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F3748);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void f33B0(void *, int, int, int) __asm__("func_003F33B0");
extern void f3530(void *, unsigned char *, int) __asm__("func_003F3530");
int func_003F3748(char *out, int n, unsigned char *in, int a, int b, int c) {
    unsigned char ctx[0x110];
    unsigned char k;
    int i;
    int ch;
    int cur;
    unsigned t;
    k = 0;
    f33B0(ctx, a, b, c);
    i = 0;
    while (i < n - 1) {
        cur = in[i];
        i++;
        if (cur == 0) break;
        f3530(ctx, &k, 1);
        t = k % 96U - 0x40;
        ch = (int)(cur - t) % 96 + 0x20;
        *out = ch;
        if ((char)ch == 0x7F) break;
        out++;
    }
    if (i < n) *out = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3860);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051F000[];
extern int func_003F4C98(char *, int, int);
extern int f4C68g(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
int func_003F3860(int *self) {
    int id = self[0x58 / 4];
    func_003F4C98(D_0051F000, 'name', 'done');
    f4C68g(D_0051F000, 1, id, 0, 0, 0, 0, 0, 1, self);
    if (self[0] > 0) {
        func_003F4C98(D_0051F000, 'name', 'addr');
        f4C68g(D_0051F000, 1, id, 0, 0, 0, 0, 0, 1, self + 1);
    }
    return self[0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3940);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051F010[];
extern int func_003F4C98(char *, int, int);
extern void func_00256BD8(int *);
void func_003F3940(int *self) {
    int v = self[0x58 / 4];
    func_003F4C98(D_0051F010, 'name', 'free');
    func_003F4C68(D_0051F010, 1, v, 0, 0, 0, 0, 0, 0, 0);
    func_00256BD8(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F39C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_0051F020[];
extern int *RpcAlloc(int);
extern void f6210k(int *, int, int) __asm__("func_00416210");
extern int func_003F4C98(char *, int, int);
extern int f4C68k(char *, int, int, int, int, int, int, int, int, int *) __asm__("func_003F4C68");
extern int func_003F3860(int *);
extern void func_003F3940(int *);
int *f39C8impl(int a, int b) __asm__("func_003F39C8");
int *f39C8impl(int a, int b) {
    int f;
    int *p = RpcAlloc(0x5C);
    if (p) {
        f6210k(p, 0, 0x5C);
        f = (int)func_003F3860;
        p[3] = (int)func_003F3940;
        p[2] = f;
        func_003F4C98(D_0051F020, 'name', 'look');
        f = 1;
        f4C68k(D_0051F020, f, 0, b, a, -f, 0, 0, f, p + 0x58 / 4);
    }
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3A98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F39C8(void);
int func_003F3A98(void* p, int a) { return func_003F39C8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F3AB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F39C8(void);
int func_003F3AB8(void) {
    int *p;
    int r = 0;
    p = (int *)func_003F39C8();
    if (p) {
        while (((int (*)(int *))p[2])(p) == 0)
            ;
        r = p[1];
        ((void (*)(int *))p[3])(p);
    }
    return r;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F3B18);

INCLUDE_ASM("seg/seg_2F41F8", func_003F3BC8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F3C50);

INCLUDE_ASM("seg/seg_2F41F8", func_003F3CC0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F3F78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004512E8[];
extern const char D_00497028[];
extern void f3CC0(int, unsigned char *, int) __asm__("func_003F3CC0");
char *func_003F3F78(void) {
    unsigned char b[16];
    if (D_004512E8[0] == 0) {
        f3CC0(0x6d616378, b, 0x10);
        D_004512E8[1] = D_00497028[b[0] >> 4];
        D_004512E8[2] = D_00497028[b[0] & 0xF];
        D_004512E8[3] = D_00497028[b[1] >> 4];
        D_004512E8[4] = D_00497028[b[1] & 0xF];
        D_004512E8[5] = D_00497028[b[2] >> 4];
        D_004512E8[6] = D_00497028[b[2] & 0xF];
        D_004512E8[7] = D_00497028[b[3] >> 4];
        D_004512E8[8] = D_00497028[b[3] & 0xF];
        D_004512E8[9] = D_00497028[b[4] >> 4];
        D_004512E8[10] = D_00497028[b[4] & 0xF];
        D_004512E8[11] = D_00497028[b[5] >> 4];
        D_004512E8[12] = D_00497028[b[5] & 0xF];
        D_004512E8[0] = '$';
        D_004512E8[13] = 0;
    }
    return D_004512E8;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F40B8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4118);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4198);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F4DE8(void);
int func_003F4198(void) { return func_003F4DE8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F41B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int func_003F4DE8u(void) __asm__("func_003F4DE8");
void func_003F41B8(int n) {
    if (n == 0) n = 5;
    if (n > 0) {
        unsigned int end = func_003F4DE8u() + n;
        while (func_003F4DE8u() < end) func_003F4DE8u();
    }
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F4210);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4250);

INCLUDE_ASM("seg/seg_2F41F8", func_003F42A0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4330);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F4DE8(void);
int func_003F4330(void) { return func_003F4DE8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4350);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F4350(unsigned char *a, unsigned char *b, int n) {
    int d;
    if (n < 0) {
        while (*a) {
            if (*a != *b)
                return *a - *b;
            a++;
            b++;
        }
        return 0;
    }
    while (n > 0) { d = *a++ - *b; if (d) return d; n--; b++; }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F43D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F43D0(int a, ...) { return 1; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F4410);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4448);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F44F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F4A78(void);
extern int func_00423CC0(void);
extern volatile int D_004A5B04;
void func_003F44F8(void) {
    while (1) {
        func_003F4A78();
        D_004A5B04 = 1;
        func_00423CC0();
        D_004A5B04 = 0;
    }
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F4528);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4A78);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4C68);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4C98);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4D68);

INCLUDE_ASM("seg/seg_2F41F8", func_003F4DE8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4E80);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_00424698(int, int);
int func_003F4E80(int a0, int a1) { return func_00424698(a0, a0 + a1); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F4EA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F4EA0(unsigned a, int *out) {
    unsigned *p = (unsigned *)((a & 0xFFFFFFF) | 0x20000000);
    unsigned x = p[0];
    int sz;
    int n;
    unsigned *q;
    unsigned h;
    if (x == 0 || (sz = p[3]) < 0 || sz > 0x8000) return -1;
    n = ((sz + 0xF) & 0x7FFFFFF0) + 0x10;
    q = (unsigned *)((x & 0xFFFFFFF) | 0x20000000);
    h = p[2];
    if (q != (unsigned *)((char *)p + n)) return -2;
    if ((((q[0]) & 0xFFFFFFF) | 0x20000000) != (unsigned)p) return -2;
    if (h != q[2]) return -3;
    if (out) *out = sz;
    return h;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F4F40);

INCLUDE_ASM("seg/seg_2F41F8", func_003F5000);

INCLUDE_ASM("seg/seg_2F41F8", func_003F53F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F5470);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5478);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F00[];
extern int *RpcAlloc(int);
extern void *f6210h(void *, int, int) __asm__("func_00416210");
extern int func_00424698(int, int);
extern void func_00424020(int);
extern void func_00256BD8(int *);
extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
int *func_003F5478(int size) {
    int *p;

    if (size < 0x1000) size = 0x1000;
    if (size > 0x2000) size = 0x2000;

    p = RpcAlloc(size + 0x9C);
    if (p == 0) return 0;
    f6210h(p, 0, 0x9C);
    p[1] = ((int)p + 0x5B) & ~0x3F;
    p[5] = size;
    p[6] = p[1] + 0x40;
    f6210h((void *)p[1], 0, 0x20);
    f6210h((void *)p[6], 0, p[5]);
    func_00424698(p[1], p[1] + 0x20);
    func_00424698(p[6], p[6] + p[5]);
    func_00424020(0);
    func_003F4C98(D_00525F00, 'sess', 'crea');
    func_003F4C68(D_00525F00, 1, 0, size, 0, 0, 0, 0, 1, (int)p);
    if (p[0] == 0) {
        func_00256BD8(p);
        p = 0;
    }
    return p;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F55B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F10[];
extern int func_003F4C98(char *, int, int);
extern void func_00256BD8(int *);
void func_003F55B8(int *p) {
    func_003F4C98(D_00525F10, 'sess', 'dest');
    func_003F4C68(D_00525F10, 1, *p, 0, 0, 0, 0, 0, 1, 0);
    func_00256BD8(p);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5638);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F20[];
extern int func_003F4C98(char *, int, int);
extern int func_003F4C68(char *, int, int, int, int, int, int, int, int, int);
extern int func_00424698(int, int);
typedef struct { unsigned short family; unsigned short port; int addr; int z8; int zC; } SA5638;
int func_003F5638(int *self, unsigned addr, int port) {
    unsigned v;
    if (((SA5638 *)self[1])->family != 0) return -1;
    ((SA5638 *)self[1])->family = 2;
    ((SA5638 *)self[1])->port = 0;
    ((SA5638 *)self[1])->addr = 0;
    ((SA5638 *)self[1])->z8 = 0;
    ((SA5638 *)self[1])->zC = 0;
    ((unsigned char *)self[1])[2] = port >> 8;
    ((unsigned char *)self[1])[3] = port;
    v = addr;
    ((unsigned char *)self[1])[7] = v;
    v >>= 8;
    ((unsigned char *)self[1])[6] = v;
    v >>= 8;
    ((unsigned char *)self[1])[5] = v;
    v >>= 8;
    ((unsigned char *)self[1])[4] = v;
    func_00424698(self[1], self[1] + 0x20);
    func_003F4C98(D_00525F20, 'sess', 'conn');
    func_003F4C68(D_00525F20, 0, self[0], 0, self[1], 0x10, 0, 0, 0, 0);
    self[4] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5738);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00525F30[];
extern int func_003F4C98(char *, int, int);
extern void func_00416210b(int, int, int) __asm__("func_00416210");
int func_003F5738(int *self) {
    func_003F4C98(D_00525F30, 'sess', 'disc');
    func_003F4C68(D_00525F30, 0, self[0], 0, 0, 0, 0, 0, 0, 0);
    func_00416210b(self[1], 0, 0x20);
    self[4] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F57C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned char b[0x18]; } B57C8;
typedef struct { int f0; B57C8 *f4; int f8; int fC; int f10; } S57C8;
extern char D_00525F40[];
extern int func_003F4C98(char *, int, int);
extern int c57_4C68(char *, int, int, int, int, int, B57C8 *, int, int, int) __asm__("func_003F4C68");
int func_003F57C8_impl(S57C8 *self, int key) __asm__("func_003F57C8");
int func_003F57C8_impl(S57C8 *self, int key) {
    func_003F4C98(D_00525F40, 'sess', 'stat');
    c57_4C68(D_00525F40, 0, self->f0, 0, 0, 0, self->f4, 0x20, 1, 0);
    if (key == 'port') {
        return (self->f4->b[2] << 8) | self->f4->b[3];
    }
    if (key == 'addr') {
        return (((self->f4->b[4] << 8 | self->f4->b[5]) << 8 | self->f4->b[6]) << 8) | self->f4->b[7];
    }
    if (key == 'ladr') {
        return (((self->f4->b[0x14] << 8 | self->f4->b[0x15]) << 8 | self->f4->b[0x16]) << 8) | self->f4->b[0x17];
    }
    if (key == 'lprt') {
        return (self->f4->b[0x12] << 8) | self->f4->b[0x13];
    }
    if (key == 'ibuf') {
        return self->f10;
    }
    return 0;
}
#endif

void func_003F5908(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5910);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int n0; unsigned char *p; int n; char *s; int z; } IO5910;
extern char D_00525F50[];
extern int func_003F4C98(char *, int, int);
extern int c59_4C68(char *, int, int, int, IO5910 *, int, int, int, int, int) __asm__("func_003F4C68");
extern void c59_5908(int *) __asm__("func_003F5908");
int func_003F5910(int *self, int a, int b, char *str, int len) {
    unsigned char hdr[12];
    IO5910 io;
    c59_5908(self);
    if (len < 0) {
        len = strlen(str) + 1;
    }
    len += 0xC;
    hdr[8] = len >> 24;
    hdr[9] = len >> 16;
    hdr[10] = len >> 8;
    hdr[11] = len;
    hdr[0] = a >> 24;
    hdr[1] = a >> 16;
    hdr[2] = a >> 8;
    hdr[3] = a;
    hdr[4] = b >> 24;
    hdr[5] = b >> 16;
    hdr[6] = b >> 8;
    hdr[7] = b;
    io.n0 = 0xC;
    io.p = hdr;
    io.n = len - 0xC;
    io.s = str;
    io.z = 0;
    func_003F4C98(D_00525F50, 'sess', 'send');
    c59_4C68(D_00525F50, 2, self[0], 0, &io, 0, 0, 0, 0, 0);
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F5A50);

INCLUDE_ASM("seg/seg_2F41F8", func_003F5A70);

INCLUDE_ASM("seg/seg_2F41F8", func_003F5C38);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5D88);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F4DE8(void);
int func_003F5D88(void) { return func_003F4DE8(); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F5DA8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F5E90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_0042B438(int, int, int, int *);
extern int func_0042B640(int);
int func_003F5E90(int a, int b) {
    int buf[4];
    int r = 0;
    if (a >= 0) {
        if (func_0042B438(a, 0, 0, buf) < 0) r = b;
        if (func_0042B640(a) < 0) r = b;
    }
    return r;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F5F08);

INCLUDE_ASM("seg/seg_2F41F8", func_003F5F88);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6040);

INCLUDE_ASM("seg/seg_2F41F8", func_003F60B8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6138);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F61C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F61C8(void) { return 0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F61D0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F62F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6348);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6358);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int sceUsbKbSetLEDStatus(int, int);
extern int sceUsbKbSetLEDMode(int, int);
extern void func_0040C810(int, int *);
extern void func_0040D4A0(int);
extern void func_0040C728(int, int, int);
extern void func_0040C770(int, int);
void func_003F6358(int h, int on) {
    int res;
    if (on) {
        res = sceUsbKbSetLEDStatus(h, 1);
        if (res == 0) func_0040C810(0, &res);
        res = sceUsbKbSetLEDMode(h, 1);
        if (res == 0) func_0040C810(0, &res);
        func_0040D4A0(h);
        func_0040C728(h, 0x1E, 2);
        func_0040C770(h, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F63E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00526038[];
void func_003F63E0(int a0) { D_00526038[0] = a0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F63F0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F64E0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6520);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F6538);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003F63F0(void);
extern int func_003F6520(int);
extern int func_003F6348(void);
int func_003F6538(void) {
    int i;
    func_003F63F0();
    for (i = 0; i < func_003F6348(); i++) {
        if (func_003F6520(i) == 1) return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F65A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern short D_004513DC[], D_0045133A[], D_00451350[];
short *func_003F65A0(unsigned int c) {
    if (c - 0x2E < 8) return &D_004513DC[c];
    if (c - 0x87 >= 5) return &D_00451350[c];
    return &D_0045133A[c];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F65F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern short D_00451420[], D_00451350[];
extern short *func_003F65A0(unsigned int);
void func_003F65F8(char *out, char *flags, int mode, int c) {
    unsigned char *p;
    unsigned int t;
    if ((*(int *)flags & 1) && (t = c - 0x59, t < 0xB)) {
        p = (unsigned char *)&D_00451420[t];
    } else if (mode == 0) {
        p = (unsigned char *)&D_00451350[c];
    } else {
        p = (unsigned char *)func_003F65A0(c);
    }
    *out = p[1];
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6670);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_00451458[];
extern int f268(int, int, int, int) __asm__("func_0040D268");
void func_003F6670_impl(unsigned char *self, int *fl, int kb, unsigned code) __asm__("func_003F6670");
void func_003F6670_impl(unsigned char *self, int *fl, int kb, unsigned code) {
    unsigned t = code - 0x54;
    if (t < 0x10) {
        if (code < 0x59 || (fl[0] & 1)) self[1] = D_00451458[t];
        else self[1] = 0;
    } else {
        int r = f268(kb, fl[1], fl[0], code & 0xFFFF);
        if (r < 0x100) self[1] = r;
        else self[1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6700);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F6700_impl(unsigned flags, unsigned key, unsigned *out) __asm__("func_003F6700");
int func_003F6700_impl(unsigned flags, unsigned key, unsigned *out) {
    int ok = 1;
    unsigned v = 0;
    switch (key) {
    case 0x5B: v = flags & 0x8; break;
    case 0xA3: v = flags & 0x10; break;
    case 0xA1: v = flags & 0x20; break;
    case 0xA0:
    case 0x10: v = flags & 0x2; break;
    case 0x12:
    case 0xA4: v = flags & 0x4; break;
    case 0x11: v = flags & 0x11; break;
    case 0x5C: v = flags & 0x80; break;
    case 0xA2: v = flags & 0x1; break;
    case 0xA5: v = flags & 0x40; break;
    default: ok = 0; break;
    }
    if (ok) *out = v;
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F67E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F67E0(int a, unsigned int mode, int *out) {
    int r = 1;
    int v = 0;
    switch (mode) {
    case 0x90: v = a & 1; break;
    case 0x14: v = a & 2; break;
    case 0x91: v = a & 4; break;
    default: r = 0; break;
    }
    if (r) *out = v;
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6838);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int a; int b; int pad; unsigned short t[1]; } S6838;
extern int f6700(int, int, int *) __asm__("func_003F6700");
extern int f67E0(int, int, int *) __asm__("func_003F67E0");
extern void f65F8(char *, S6838 *, int, int) __asm__("func_003F65F8");
int func_003F6838(S6838 *s, int b, int idx, int d) {
    int res = 0;
    char buf[8];
    int key = s->t[idx];
    if (f6700(s->b, d, &res) == 0) {
        if (f67E0(s->a, d, &res) == 0) {
            f65F8(buf, s, b, key);
            if ((unsigned char)buf[0] == d) res = 1;
        }
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F68F0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_004A5B10;
extern void func_003F65F8(char *, char *, int, int);
extern void func_003F6670(char *, char *, int, int);
char *func_003F68F0(char *a, int b, int c) {
    unsigned int v = *(unsigned short *)(a + (c << 1) + 12);
    func_003F65F8(&D_004A5B10, a, b, v);
    func_003F6670(&D_004A5B10, a, b, v);
    return &D_004A5B10;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F6960);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6A40);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6AB0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6AB8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6AF0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6B20);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6B50);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6B80);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6C40(void);
int func_003F6B80(void) { return func_003F6C40() & 1; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F6BA0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6C10);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6C40);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6C50);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6C60);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6C70);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6D98);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6DC8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F6E70);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FC260(void);
extern int func_003F8178(int);
int func_003F6E70(void) { return func_003F8178(func_003FC260()); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F6E98);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6EE0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F6F68);

INCLUDE_ASM("seg/seg_2F41F8", func_003F71B8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F71F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7BD0(void);
int func_003F71F8(void) { return func_003F7BD0(); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F7218);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F72C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7BD0(void);
void func_003F72C8(int *a) { func_003F7BD0(); *a = 2; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F72F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7328);

INCLUDE_ASM("seg/seg_2F41F8", func_003F73A8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F73F0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7438);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7478);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7528);

INCLUDE_ASM("seg/seg_2F41F8", func_003F75D8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7658);

INCLUDE_ASM("seg/seg_2F41F8", func_003F76D8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7708);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F77F0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F77F0(void) { return 0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F77F8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7820);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F77F8(void);
int func_003F7820(void) { return func_003F77F8(); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F7840);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7890);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F78E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7920(int);
int func_003F78E0(void) { return func_003F7920(0); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7900);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F7958(int);
int func_003F7900(void) { return func_003F7958(0); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F7920);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7958);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7990);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7A30);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7A60);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7BD0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7C00);

INCLUDE_ASM("seg/seg_2F41F8", func_003F7D98);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7DE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int **p; } W7DE0;
typedef struct { char pad[0xC]; int (*f0)(void); int (*f1)(int, int); int (*f2)(W7DE0 **); } V7DE0;
extern V7DE0 *fB5E8(void) __asm__("func_003FB5E8");
int func_003F7DE0(int a) {
    int err = 0;
    V7DE0 *o = fB5E8();
    int r;
    W7DE0 *w;
    r = o->f0();
    if (r == 0) {
        r = o->f1(a, 0x296E);
        if (r == 0) {
            r = o->f2(&w);
            if (r == 0) {
                err = 0x15;
                if (**(unsigned **)((char *)w + 4) == 0x836C71D) err = 0;
            }
        }
    }
    if (r != 0) return 0x18;
    return err;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F7E90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003F7E90(char *dst, char *src, int n) {
    char *d;
    unsigned char *s = (unsigned char*)src;
    if (s && dst && n && s != (unsigned char*)dst) {
        d = dst;
        do {
            if (--n == 0) { *d = 0; break; }
        } while ((*d++ = *s++) != 0);
    }
    return dst;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F7EE0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6C50(void);
extern int func_003F6C70(int);
extern int func_003F6DC8(void);
void func_003F7EE0(int a) {
    func_003F6C50();
    func_003F6C70(a);
    func_003F6DC8();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F7F18);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6E70(void);
extern int func_003F6D98(void);
extern int func_003F6C60(void);
int func_003F7F18(void) {
    func_003F6E70();
    func_003F6D98();
    return func_003F6C60();
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F7F48);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8000);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8048);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003F80C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F6E98(int);
int func_003F80C0(void) { return func_003F6E98(3); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F80E0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8178);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8178(int a);
int func_003F8178(int a) {
    switch (a) {
    case 0: return 0;
    case 104: return 1;
    case 103: return 2;
    case 114: return 3;
    case 108: return 4;
    case 3:
    case 7:
    case 110: return 5;
    case 9: return 6;
    case 1: return 0x13;
    case 2: return 0x14;
    case 4: return 7;
    case 6:
    case 107: return 0xB;
    }
    return 0x18;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F8200);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8218);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8230);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FCF98(int, int);
int func_003F8230(int a) { func_003FCF98(0x22, a); return 0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F8258);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8550);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8550(unsigned v) {
    int i = 0;
    v &= 0xFFFF;
    if (!(v & 1)) {
        do {
            i++;
        } while (i < 8 && !(v & (1 << i)));
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8588);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8588(unsigned v) {
    int i = 7;
    v &= 0xFFFF;
    if (!(v & 0x80)) {
        do {
            i--;
        } while (i >= 0 && !(v & (1 << i)));
    }
    return i;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F85C0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F86B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F86B8(unsigned short v) {
    int r = 0; int i = 0;
    for (i = 0; i < 8; i++) { int bit = 1 << i; if (v & bit) r++; }
    return r;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F86F0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8710);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8720);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8768);

INCLUDE_ASM("seg/seg_2F41F8", func_003F87B0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F87F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F87F8(int a0) { return a0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F8800);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8838);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8880);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8908);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8968);

INCLUDE_ASM("seg/seg_2F41F8", func_003F89C8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8A08);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8A58);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8AC0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8B28);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8B78);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8BB0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8BE8);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003F8C30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003F8C30(int a0) { return a0; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003F8C38);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8EF8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F8FF0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9130);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9208);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9218);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9530);

INCLUDE_ASM("seg/seg_2F41F8", func_003F95E8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9698);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9818);

INCLUDE_ASM("seg/seg_2F41F8", func_003F99B8);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9B38);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9C48);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9DC0);

INCLUDE_ASM("seg/seg_2F41F8", func_003F9F00);

INCLUDE_ASM("seg/seg_2F41F8", func_003FA040);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FA1D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003F9208(void);
extern int func_003FA708(unsigned int, int);
int func_003FA1D0(unsigned int a, int b) {
    if (b == 0) return 12;
    if (func_003F9208() == 0) return 2;
    if (a >= 8) return 9;
    func_003FA708(a, b);
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FA240);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA360);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB278(void);
int func_003FA360(char* name) { return func_003FB278(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA380);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned char b[33]; } T_A380;
extern T_A380 D_004973B0;
void func_003FA380(char *src, unsigned short *dst) {
    T_A380 tbl = D_004973B0;
    unsigned char i = 0;
    unsigned short c = *src++;
    while (c != 0) {
        if ((unsigned short)(c - 0x81) < 0x18) {
            c = (c << 8) + *src++;
        } else if (c >= 0x7B) {
            c = tbl.b[c - 0x5E] + 0x8100;
        } else if (c >= 0x61) {
            c = c + 0x8220;
        } else if (c >= 0x5B) {
            c = tbl.b[c - 0x44] + 0x8100;
        } else if (c >= 0x41) {
            c = c + 0x821F;
        } else if (c >= 0x3A) {
            c = tbl.b[c - 0x2A] + 0x8100;
        } else if (c >= 0x30) {
            c = c + 0x821F;
        } else {
            c = tbl.b[c - 0x20] + 0x8100;
        }
        c = (c >> 8) | ((c & 0xFF) << 8);
        dst[i++] = c;
        c = *src++;
    }
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FA4F0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB280(void);
int func_003FA6A8(void) { return func_003FB280(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB2C8(void);
int func_003FA6C8(void) { return func_003FB2C8(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA6E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_3FB300(void);
int func_003FA6E8(void) { return func_003FB300(); }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA708);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003FA708_impl(unsigned a, unsigned char *p) __asm__("func_003FA708");
void func_003FA708_impl(unsigned a, unsigned char *p) {
    switch (a) {
    case 0: p[0] = 0; p[1] = 0; break;
    case 1: p[0] = 0; p[1] = 1; break;
    case 2: p[0] = 0; p[1] = 2; break;
    case 3: p[0] = 0; p[1] = 3; break;
    case 4: p[0] = 1; p[1] = 0; break;
    case 5: p[0] = 1; p[1] = 1; break;
    case 6: p[0] = 1; p[1] = 2; break;
    case 7: p[0] = 1; p[1] = 3; break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA7A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_004A4BE0[];
extern void func_00416210(char *, int, int);
extern void func_0041605C(char *, char *, int);
char *func_003FA7A0(char *dst, int unused, char *src) {
    char *p = dst + 1;
    func_00416210(dst, 0, 0x20);
    *dst = D_004A4BE0[0];
    func_0041605C(p, src, strlen(src));
    return p + strlen(src);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FA818);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[9]; } K9;
extern K9 D_00497400;
extern void f6210(char *, int, int) __asm__("func_00416210");
extern void f605C(char *, char *, int) __asm__("func_0041605C");
void func_003FA818_impl(char *a, char *b, int unused, char *str) __asm__("func_003FA818");
void func_003FA818_impl(char *a, char *b, int unused, char *str) {
    char *p;
    f6210(a, 0, 0x20);
    *(K9 *)a = D_00497400;
    p = a + 9;
    f605C(p, str, strlen(str));
    p = b + 9;
    strlen(str);
    f6210(b, 0, 0x20);
    *(K9 *)b = D_00497400;
    f605C(p, str, strlen(str));
    strlen(str);
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FA8F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FAE50);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB278);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB278(void) { return 1; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB280);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
static inline unsigned roundup_kb(unsigned v) { if (v & 0x3FF) return ((v >> 10) + 1) << 10; return (v >> 10) << 10; }
void func_003FB280(char *p, int *out, int *kind) {
    unsigned a = roundup_kb(*(unsigned *)(p + 0x3CC));
    unsigned b = roundup_kb(*(unsigned *)(p + 4));
    *out = a + b + 0x1400;
    *kind = 3;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB2C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char *func_003FB5E8(void);
void func_003FB2C8(char **a) {
    char *r = func_003FB5E8();
    if (a) *a = r;
}
#endif

void func_003FB2F8(void) {
}

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB300);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB300(void) { return 0; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB308);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int func_003FB308(int a0) { return a0 ? 0 : 12; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB318);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
unsigned int func_003FB318(unsigned int c) {
    unsigned char i = 8;
    do {
        if (c & 1) {
            c = (c >> 1) ^ 0xEDB88320;
        } else {
            c >>= 1;
        }
    } while (--i != 0);
    return c;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB358);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int func_003FB318(unsigned int);
void func_003FB358(unsigned int *t) {
    unsigned int i;
    for (i = 0; i < 256; i++) {
        *t++ = func_003FB318(i);
    }
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FB3A0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB430);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB480);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB490);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB4C0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB558);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB5B0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB5E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00497508[];
char *func_003FB5E8(void) { return D_00497508; }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FB5F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB620);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB640);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB650);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB6A0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB730);

INCLUDE_ASM("seg/seg_2F41F8", func_003FB7C0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB800);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_00497528[];
char *func_003FB800(void) { return D_00497528; }
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB810);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB810_impl(char *p) __asm__("func_003FB810");
char *func_003FB810_impl(char *p) {
    if (p == 0) return 0;
    return p + 100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB828);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB828_impl(char *p) __asm__("func_003FB828");
char *func_003FB828_impl(char *p) {
    if (p == 0) return 0;
    return p + 200;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB840);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
char *func_003FB840_impl(char *p) __asm__("func_003FB840");
char *func_003FB840_impl(char *p) {
    if (p == 0) return 0;
    return p + 300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB858);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003FB858(unsigned a, int *x, int *y) {
    int sx = 4, sy = 1;
    switch (a) {
    case 0: sx = 4; sy = 1; break;
    case 1: sx = 4; sy = 1; break;
    case 2: sx = 4; sy = 1; break;
    case 3: sx = 4; sy = 1; break;
    case 4: sx = 4; sy = 1; break;
    }
    *x = sx;
    *y = sy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB898);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003FB858b(int, int *, int *) __asm__("func_003FB858");
int func_003FB898(int a, int b);
int func_003FB898(int a, int b) {
    int x = 4;
    unsigned y = 1;
    unsigned r;
    unsigned pad;
    func_003FB858b(b, &x, (int *)&y);
    r = a + 0xC;
    if (r < y) return y;
    pad = (y - r % y) % y;
    return a + 0xC + pad;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FB910);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB958);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[4]; } W3FB958;
typedef struct { W3FB958 x, y; } V3FB958;
void func_003FB958(W3FB958 *dst, int a, int b, W3FB958 *c) {
    V3FB958 t;
    *(int *)&t.x = a;
    *(int *)&t.y = b;
    dst[0] = t.x;
    dst[1] = t.y;
    dst[2] = *c;
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FB9A0);

//100%
INCLUDE_ASM("seg/seg_2F41F8", func_003FB9D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003FB9D0(char *src, char *d1, char *d2, char *d3) {
    if (d1) __builtin_memcpy(d1, src, 4);
    if (d2) __builtin_memcpy(d2, src + 4, 4);
    if (d3) __builtin_memcpy(d3, src + 8, 4);
}
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FBA20);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBA78);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBAD0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBB50);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBBE0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBC40);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBCD8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBD70);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBDB8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBDF0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBE40);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBED0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBF80);

INCLUDE_ASM("seg/seg_2F41F8", func_003FBFE0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC0D8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC0E8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC260);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC390);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC3C0);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC3F0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2F41F8", func_003FC4C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int func_003FBD70(int, int, int, int);
int func_003FC4C8(int a, int b, int c) { return func_003FBD70(a, b, c, 4); }
#endif

INCLUDE_ASM("seg/seg_2F41F8", func_003FC4E8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC548);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC708);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC850);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC8F8);

INCLUDE_ASM("seg/seg_2F41F8", func_003FC990);

INCLUDE_ASM("seg/seg_2F41F8", func_003FCA40);
