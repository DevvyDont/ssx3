#include "common.h"

INCLUDE_ASM("seg/seg_1C9348", HUFF_about);

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C8468);
#ifdef SKIP_ASM
extern "C" unsigned func_002C8468(unsigned char *p, int n) {
    unsigned r = 0;
    while (n--) {
        r = (r << 8) + *p++;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C84A0);
#ifdef SKIP_ASM
extern "C" unsigned func_002C8468(unsigned char *p, int n);

extern "C" int func_002C84A0(unsigned char *p) {
    int r = 0;
    if (func_002C8468(p, 2) == 0x30FB
        || func_002C8468(p, 2) == 0x31FB
        || func_002C8468(p, 2) == 0x32FB
        || func_002C8468(p, 2) == 0x33FB
        || func_002C8468(p, 2) == 0x34FB
        || func_002C8468(p, 2) == 0x35FB) {
        r = 100;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C8550);
#ifdef SKIP_ASM
extern "C" unsigned func_002C8468(unsigned char *p, int n);

extern "C" void func_002C8550(unsigned char *p) {
    if (func_002C8468(p, 2) & 0x100) {
        func_002C8468(p + 5, 3);
    } else {
        func_002C8468(p + 2, 3);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C85A0);
#ifdef SKIP_ASM
extern "C" int func_002C76C0(int, int);

extern "C" int func_002C85A0(int a, int b, int c) {
    return func_002C76C0(a, c);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C85C0);
#ifdef SKIP_ASM
extern "C" void func_002C85C0(unsigned char *src, unsigned char *dst, int n) {
    unsigned char *end = src + n;
    int prev = 0;
    if (src < end) {
        do {
            int c = *src++;
            *dst++ = c - prev;
            prev = c;
        } while (src < end);
    }
}
#endif

INCLUDE_ASM("seg/seg_1C9348", func_002C8600);

//100%
INCLUDE_ASM("seg/seg_1C9348", func_002C86F8);
#ifdef SKIP_ASM
struct HS_86F8 { char pad[0x1524]; int l[0x208]; int r[0x208]; int a[256]; };
extern "C" void func_002C86F8(HS_86F8 *self, unsigned i, int d) {
    if (i < 0x100) {
        self->a[i] = d;
        return;
    }
    func_002C86F8(self, self->l[i], d + 1);
    func_002C86F8(self, self->r[i], d + 1);
}
#endif

INCLUDE_ASM("seg/seg_1C9348", func_002C8770);

INCLUDE_ASM("seg/seg_1C9348", func_002C8970);

INCLUDE_ASM("seg/seg_1C9348", func_002C8A78);

INCLUDE_ASM("seg/seg_1C9348", func_002C8C30);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1C9348", func_002C8CA8);
#ifdef SKIP_ASM
struct HS_8CA8 { char pad[0x2564]; int a[256]; int b[0x11A]; int cur; };
extern "C" void func_002C8600(HS_8CA8 *, int, int, int);
extern "C" void func_002C8C30(HS_8CA8 *, int, int);

extern "C" void func_002C8CA8(HS_8CA8 *self, int x, int idx) {
    if (idx == self->cur) {
        func_002C8C30(self, x, idx);
        return;
    }
    func_002C8600(self, x, self->b[idx], self->a[idx]);
}
#endif

INCLUDE_ASM("seg/seg_1C9348", func_002C8CF8);

INCLUDE_ASM("seg/seg_1C9348", func_002C8E48);

INCLUDE_ASM("seg/seg_1C9348", func_002C9A48);

INCLUDE_ASM("seg/seg_1C9348", func_002C9F60);

INCLUDE_ASM("seg/seg_1C9348", HUFF_encode);
