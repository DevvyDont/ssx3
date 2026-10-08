#include "common.h"

INCLUDE_ASM("seg/seg_1FBE38", checkActiveNode);

INCLUDE_ASM("seg/seg_1FBE38", func_002FB060);

INCLUDE_ASM("seg/seg_1FBE38", func_002FB270);

INCLUDE_ASM("seg/seg_1FBE38", func_002FB498);

INCLUDE_ASM("seg/seg_1FBE38", func_002FB6B8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FB8D0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FBAB0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FBCB8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FBEC8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FC0D0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FC2C0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void func_003506D8(void*, void*);
extern "C" void func_00350F60(void*, void*);
extern char D_004896B8[];
extern char D_004896C8[];

typedef void (*DtorFn)(void*, int);

// virtual-destructor dispatch (delete obj): vtable entry {short delta @+8, fn @+0xC}
static inline void del_obj(char* o) {
    char* vt = *(char**)(o + 0xC);
    ((DtorFn) * (void**)(vt + 0xC))(o + *(short*)(vt + 8), 3);
}

extern "C" void func_002FC2C0(char* self, char* ev) {
    switch (*(int*)(ev + 4)) {
    case 1: {
        char* o = *(char**)(self + 0xC);
        if (o) {
            if (*(short*)(o + 0x10) == 6)
                break;
            del_obj(o);
        }
        int t = *(int*)(self + 8) & 0xFFFF0300;
        *(int*)(self + 8) = t | (t >> 16) | 2;
        break;
    }
    case 0: {
        char* o = *(char**)(self + 0xC);
        if (o) {
            if (*(short*)(o + 0x10) == 6)
                break;
            del_obj(o);
        }
        func_003506D8(cMemMan_alloc(0x1C, D_004896B8, 0x20000000, 0), self);
        break;
    }
    case 3: {
        char* o = *(char**)(self + 0xC);
        if (o) {
            short ty = *(short*)(o + 0x10);
            if (ty == 6 || ty == 0x13)
                break;
            del_obj(o);
        }
        func_00350F60(cMemMan_alloc(0x1C, D_004896C8, 0x20000000, 0), self);
        break;
    }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1FBE38", func_002FC420);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_C420 __asm__("D_004A3DD8");
extern char **D_004A47B8_C420 __asm__("D_004A47B8");
extern char *D_004A28A8_C420 __asm__("D_004A28A8");
static inline char *refToPtr_C420(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C420(unsigned i) { if (i == 0) return 0; return refToPtr_C420(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void func_002FC2C0_C420(void*, void*) __asm__("func_002FC2C0");
struct V3_C420 { float x, y, z; };
extern V3_C420 D_004C9098_C420 __asm__("D_004C9098");
struct Tag_C420 { char c; Tag_C420() {} Tag_C420(const Tag_C420 &o) : c(o.c) {} };
extern unsigned D_C420[] __asm__("D_004FB500");
extern int D_004A55E4;
extern int D_004A3C00;
extern "C" void* func_003E6574_C420(void*, void*, int) __asm__("func_003E6574");
struct Ent_C420 { int idx; int val; int pad; int type; };
extern "C" V3_C420 func_002FC420(int n, Ent_C420 *e) {
    if (D_004A55E4 == 0) {
        D_C420[0] = 0xFFFFFFFF;
        ((int*)D_C420)[1] = 0;
        D_004A55E4 = 1;
        (*(Tag_C420*)((char*)D_C420 + 8)) = Tag_C420((*(Tag_C420*)((char*)D_C420 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_C420(buf, D_C420, 12);
    Ent_C420 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C00)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_C420 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_C420 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_C420(i);
            r = t;
        }
        obj = r;
    }
    if (obj == 0)
        return D_004C9098_C420;
    void *p = *(void**)(obj + 0xC);
    if (!p || !checkActiveNode(p, 6, buf))
        func_002FC2C0_C420(obj, buf);
    return D_004C9098_C420;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_002FC5D8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FC7D0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FC9C8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FCC20);

INCLUDE_ASM("seg/seg_1FBE38", func_002FCDC8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FCFF0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FD250);

INCLUDE_ASM("seg/seg_1FBE38", func_002FD420);

INCLUDE_ASM("seg/seg_1FBE38", func_002FD758);

INCLUDE_ASM("seg/seg_1FBE38", func_002FD9F8);

INCLUDE_ASM("seg/seg_1FBE38", func_002FDC60);

INCLUDE_ASM("seg/seg_1FBE38", func_002FDED0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FE0C0);

INCLUDE_ASM("seg/seg_1FBE38", func_002FE2C0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FE4A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_E4A8 __asm__("D_004A3DD8");
extern char **D_004A47B8_E4A8 __asm__("D_004A47B8");
extern char *D_004A28A8_E4A8 __asm__("D_004A28A8");
extern "C" void func_00355BF0_E4A8(void*, void*) __asm__("func_00355BF0");
struct O_E4A8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_E4A8 { float x, y, z; };
extern V3_E4A8 D_004C9098_E4A8 __asm__("D_004C9098");
struct Tag_E4A8 { char c; Tag_E4A8() {} Tag_E4A8(const Tag_E4A8 &o) : c(o.c) {} };
extern unsigned D_E4A8[] __asm__("D_004FB818");
extern int D_004A5620;
extern const int D_00446268[];
extern "C" void* func_003E6574_E4A8(void*, void*, int) __asm__("func_003E6574");
struct Ent_E4A8 { int idx; int val; int pad; int type; };
extern "C" V3_E4A8 func_002FE4A8(int n, Ent_E4A8 *e) {
    if (D_004A5620 == 0) {
        D_E4A8[0] = 0xFFFFFFFF;
        ((int*)D_E4A8)[1] = 0;
        ((int*)D_E4A8)[2] = 0;
        ((int*)D_E4A8)[3] = 0;
        *(float*)&D_E4A8[4] = 2.0f;
        D_004A5620 = 1;
        (*(Tag_E4A8*)((char*)D_E4A8 + 20)) = Tag_E4A8((*(Tag_E4A8*)((char*)D_E4A8 + 20)));
    }
    unsigned buf64[6];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_E4A8(buf, D_E4A8, 24);
    Ent_E4A8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446268[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_E4A8 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_E4A8 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_E4A8 *p = *(O_E4A8**)(obj + 0xC);
    if (p) {
        if (p->v16())
            func_00355BF0_E4A8(p, buf);
    }
    return D_004C9098_E4A8;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_002FE668);

INCLUDE_ASM("seg/seg_1FBE38", cViewer_addParticle);

INCLUDE_ASM("seg/seg_1FBE38", func_002FEE98);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FF1C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_F1C8 __asm__("D_004A3DD8");
extern char **D_004A47B8_F1C8 __asm__("D_004A47B8");
extern char *D_004A28A8_F1C8 __asm__("D_004A28A8");
extern "C" void func_00355E38_F1C8(void*, void*) __asm__("func_00355E38");
struct O_F1C8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_F1C8 { float x, y, z; };
extern V3_F1C8 D_004C9098_F1C8 __asm__("D_004C9098");
struct Tag_F1C8 { char c; Tag_F1C8() {} Tag_F1C8(const Tag_F1C8 &o) : c(o.c) {} };
extern unsigned D_F1C8[] __asm__("D_004FBA28");
extern int D_004A5630;
extern const int D_00446470[];
extern "C" void* func_003E6574_F1C8(void*, void*, int) __asm__("func_003E6574");
struct Ent_F1C8 { int idx; int val; int pad; int type; };
extern "C" V3_F1C8 func_002FF1C8(int n, Ent_F1C8 *e) {
    if (D_004A5630 == 0) {
        D_F1C8[0] = 0xFFFFFFFF;
        D_F1C8[1] = 0xFFFFFFFF;
        ((int*)D_F1C8)[2] = -1;
        D_004A5630 = 1;
        (*(Tag_F1C8*)((char*)D_F1C8 + 12)) = Tag_F1C8((*(Tag_F1C8*)((char*)D_F1C8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    func_003E6574_F1C8(buf, D_F1C8, 16);
    Ent_F1C8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446470[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_F1C8 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_F1C8 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            r = 0;
        else
            r = (char*)(i << 2);
        obj = r;
    }
    if (~buf[1] != 0) {
        O_F1C8 *p = *(O_F1C8**)(obj + 0xC);
        if (p) {
            if (p->v16())
                func_00355E38_F1C8(p, buf);
        }
    }
    return D_004C9098_F1C8;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_002FF390);

INCLUDE_ASM("seg/seg_1FBE38", func_002FF5E8);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FF850);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_F850 { char c; Flag_F850() {} Flag_F850(const Flag_F850& o) : c(o.c) {} };
struct Ent_F850 { int idx; int val; int pad; int kind; };
struct R12_F850 { int a; int b; int c; };
extern R12_F850 D_004C9098_F850 __asm__("D_004C9098");

extern int D_00446530_F850[] __asm__("D_00446530");
extern "C" void func_0010F1C0(int, int, float);
extern int* D_004A3DD8_F850 __asm__("D_004A3DD8");
extern char* D_004A28A8_F850 __asm__("D_004A28A8");
extern int D_004A563C_F850 __asm__("D_004A563C");
struct T12_F850 { int a; int b; int e; Flag_F850 f; };
extern T12_F850 D_004FBAE8_F850 __asm__("D_004FBAE8");
extern "C" R12_F850 func_002FF850(int n, Ent_F850* e) {
    if (D_004A563C_F850 == 0) {
        D_004FBAE8_F850.a = -1;
        D_004A563C_F850 = 1;
        D_004FBAE8_F850.b = 0;
        D_004FBAE8_F850.e = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBAE8_F850.f = Flag_F850(D_004FBAE8_F850.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBAE8_F850, 16);
    Ent_F850* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_00446530_F850[k] && D_00446530_F850[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    if (vp[0] == -1)
        vp[0] = *D_004A3DD8_F850;
    char* o = *(char**)(D_004A28A8_F850 + 0x84);
    char* base = *(char**)(o + 0xC);
    func_0010F1C0(*(int*)(base + (vp[0] << 2) + 0x28), vp[1], *(float*)&vp[2]);
    return D_004C9098_F850;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FF9A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_F9A8 __asm__("D_004A3DD8");
extern char **D_004A47B8_F9A8 __asm__("D_004A47B8");
extern char *D_004A28A8_F9A8 __asm__("D_004A28A8");
static inline char *look_F9A8(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_F9A8 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_F9A8 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

struct O_F9A8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(int, float); };
struct V3_F9A8 { float x, y, z; };
extern V3_F9A8 D_004C9098_F9A8 __asm__("D_004C9098");
struct Tag_F9A8 { char c; Tag_F9A8() {} Tag_F9A8(const Tag_F9A8 &o) : c(o.c) {} };
extern unsigned D_F9A8[] __asm__("D_004FBAF8");
extern int D_004A5640;
extern const int D_00446540[];
extern "C" void* func_003E6574_F9A8(void*, void*, int) __asm__("func_003E6574");
struct Ent_F9A8 { int idx; int val; int pad; int type; };
extern "C" V3_F9A8 func_002FF9A8(int n, Ent_F9A8 *e) {
    if (D_004A5640 == 0) {
        D_F9A8[0] = 0xFFFFFFFF;
        ((int*)D_F9A8)[1] = -1;
        ((int*)D_F9A8)[2] = 0;
        D_004A5640 = 1;
        (*(Tag_F9A8*)((char*)D_F9A8 + 12)) = Tag_F9A8((*(Tag_F9A8*)((char*)D_F9A8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_F9A8(buf, D_F9A8, 16);
    Ent_F9A8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446540[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = look_F9A8(buf[0]);
    O_F9A8 *p = *(O_F9A8**)(obj + 0xC);
    if (p)
        p->v36(buf[1], *(float*)&buf[2]);
    return D_004C9098_F9A8;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_002FFB50);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FFD58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_FD58 __asm__("D_004A3DD8");
extern char **D_004A47B8_FD58 __asm__("D_004A47B8");
extern char *D_004A28A8_FD58 __asm__("D_004A28A8");
static inline char *look_FD58(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_FD58 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_FD58 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

struct O_FD58 { int p0, p1, p2; virtual void v01(int); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); };
struct V3_FD58 { float x, y, z; };
extern V3_FD58 D_004C9098_FD58 __asm__("D_004C9098");
struct Tag_FD58 { char c; Tag_FD58() {} Tag_FD58(const Tag_FD58 &o) : c(o.c) {} };
extern unsigned D_FD58 __asm__("D_004A5658");
extern Tag_FD58 D_004A565C;
extern int D_004A5660;
extern int D_004A3C20;
extern "C" void* func_003E6574_FD58(void*, void*, int) __asm__("func_003E6574");
struct Ent_FD58 { int idx; int val; int pad; int type; };
extern "C" V3_FD58 func_002FFD58(int n, Ent_FD58 *e) {
    if (D_004A5660 == 0) {
        D_FD58 = 0xFFFFFFFF;
        D_004A5660 = 1;
        D_004A565C = Tag_FD58(D_004A565C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_FD58(buf, &D_FD58, 8);
    Ent_FD58 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C20)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = look_FD58(buf[0]);
    if (obj) {
        O_FD58 *p = *(O_FD58**)(obj + 0xC);
        if (p) {
            if (p->v10(0x10))
                p->v01(3);
        }
    }
    return D_004C9098_FD58;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_002FFF00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_FF00 __asm__("D_004A3DD8");
extern char **D_004A47B8_FF00 __asm__("D_004A47B8");
extern char *D_004A28A8_FF00 __asm__("D_004A28A8");
static inline char *refToPtr_FF00(unsigned i) { return (char*)(i << 2); }
static inline char *conv_FF00(unsigned i) { if (i == 0) return 0; return refToPtr_FF00(i); }
extern "C" void *func_0028B180_FF00(void) __asm__("func_0028B180");
extern "C" void func_002974A0_FF00(void*, void*, int) __asm__("func_002974A0");
struct V3_FF00 { float x, y, z; };
extern V3_FF00 D_004C9098_FF00 __asm__("D_004C9098");
struct Tag_FF00 { char c; Tag_FF00() {} Tag_FF00(const Tag_FF00 &o) : c(o.c) {} };
extern unsigned D_FF00[] __asm__("D_004FBB08");
extern int D_004A5664;
extern int D_004A3C28;
extern "C" void* func_003E6574_FF00(void*, void*, int) __asm__("func_003E6574");
struct Ent_FF00 { int idx; int val; int pad; int type; };
extern "C" V3_FF00 func_002FFF00(int n, Ent_FF00 *e) {
    if (D_004A5664 == 0) {
        D_FF00[0] = 0xFFFFFFFF;
        D_FF00[1] = 0xFFFFFFFF;
        D_004A5664 = 1;
        (*(Tag_FF00*)((char*)D_FF00 + 8)) = Tag_FF00((*(Tag_FF00*)((char*)D_FF00 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_FF00(buf, D_FF00, 12);
    Ent_FF00 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C28)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_FF00 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_FF00 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_FF00(i);
            r = t;
        }
        obj = r;
    }
    if (obj) {
        int b1 = buf[1];
        func_002974A0_FF00(func_0028B180_FF00(), obj, b1);
    }
    return D_004C9098_FF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003000A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_00A8 __asm__("D_004A3DD8");
extern char **D_004A47B8_00A8 __asm__("D_004A47B8");
extern char *D_004A28A8_00A8 __asm__("D_004A28A8");
static inline char *refToPtr_00A8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_00A8(unsigned i) { if (i == 0) return 0; return refToPtr_00A8(i); }
extern "C" void *func_0028B180_00A8(void) __asm__("func_0028B180");
extern "C" void func_00297950_00A8(void*, void*, int, int) __asm__("func_00297950");
struct V3_00A8 { float x, y, z; };
extern V3_00A8 D_004C9098_00A8 __asm__("D_004C9098");
struct Tag_00A8 { char c; Tag_00A8() {} Tag_00A8(const Tag_00A8 &o) : c(o.c) {} };
extern unsigned D_00A8[] __asm__("D_004FBB18");
extern int D_004A5668;
extern const int D_00446550[];
extern "C" void* func_003E6574_00A8(void*, void*, int) __asm__("func_003E6574");
struct Ent_00A8 { int idx; int val; int pad; int type; };
extern "C" V3_00A8 func_003000A8(int n, Ent_00A8 *e) {
    if (D_004A5668 == 0) {
        D_00A8[0] = 0xFFFFFFFF;
        D_00A8[1] = 0xFFFFFFFF;
        ((int*)D_00A8)[2] = -1;
        D_004A5668 = 1;
        (*(Tag_00A8*)((char*)D_00A8 + 12)) = Tag_00A8((*(Tag_00A8*)((char*)D_00A8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_00A8(buf, D_00A8, 16);
    Ent_00A8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446550[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_00A8 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_00A8 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_00A8(i);
            r = t;
        }
        obj = r;
    }
    if (obj) {
        int b1 = buf[1];
        int b2 = buf[2];
        func_00297950_00A8(func_0028B180_00A8(), obj, b1, b2);
    }
    return D_004C9098_00A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00300260);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_0260 __asm__("D_004A3DD8");
extern char **D_004A47B8_0260 __asm__("D_004A47B8");
extern char *D_004A28A8_0260 __asm__("D_004A28A8");
static inline char *refToPtr_0260(unsigned i) { return (char*)(i << 2); }
static inline char *conv_0260(unsigned i) { if (i == 0) return 0; return refToPtr_0260(i); }
extern "C" void *func_0028B180_0260(void) __asm__("func_0028B180");
extern "C" void func_00297EB8_0260(void*, void*, int, int) __asm__("func_00297EB8");
struct V3_0260 { float x, y, z; };
extern V3_0260 D_004C9098_0260 __asm__("D_004C9098");
struct Tag_0260 { char c; Tag_0260() {} Tag_0260(const Tag_0260 &o) : c(o.c) {} };
extern unsigned D_0260[] __asm__("D_004FBB28");
extern int D_004A566C;
extern const int D_00446560[];
extern "C" void* func_003E6574_0260(void*, void*, int) __asm__("func_003E6574");
struct Ent_0260 { int idx; int val; int pad; int type; };
extern "C" V3_0260 func_00300260(int n, Ent_0260 *e) {
    if (D_004A566C == 0) {
        D_0260[0] = 0xFFFFFFFF;
        D_0260[1] = 0xFFFFFFFF;
        ((int*)D_0260)[2] = -1;
        D_004A566C = 1;
        (*(Tag_0260*)((char*)D_0260 + 12)) = Tag_0260((*(Tag_0260*)((char*)D_0260 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_0260(buf, D_0260, 16);
    Ent_0260 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446560[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_0260 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_0260 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_0260(i);
            r = t;
        }
        obj = r;
    }
    if (obj) {
        int b1 = buf[1];
        int b2 = buf[2];
        func_00297EB8_0260(func_0028B180_0260(), obj, b1, b2);
    }
    return D_004C9098_0260;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00300418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_0418 __asm__("D_004A3DD8");
extern char **D_004A47B8_0418 __asm__("D_004A47B8");
extern char *D_004A28A8_0418 __asm__("D_004A28A8");
struct O_0418 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(int); };
struct V3_0418 { float x, y, z; };
extern V3_0418 D_004C9098_0418 __asm__("D_004C9098");
struct Tag_0418 { char c; Tag_0418() {} Tag_0418(const Tag_0418 &o) : c(o.c) {} };
extern unsigned D_0418[] __asm__("D_004FBB38");
extern int D_004A5670;
extern int D_004A3C30;
extern "C" void* func_003E6574_0418(void*, void*, int) __asm__("func_003E6574");
struct Ent_0418 { int idx; int val; int pad; int type; };
extern "C" V3_0418 func_00300418(int n, Ent_0418 *e) {
    if (D_004A5670 == 0) {
        D_0418[0] = 0xFFFFFFFF;
        ((int*)D_0418)[1] = 0;
        D_004A5670 = 1;
        (*(Tag_0418*)((char*)D_0418 + 8)) = Tag_0418((*(Tag_0418*)((char*)D_0418 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_0418(buf, D_0418, 12);
    Ent_0418 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C30)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_0418 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_0418 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_0418 *p = *(O_0418**)(obj + 0xC);
    if (p) {
        int b1 = buf[1];
        if (b1 == 0)
            p->v34(0);
        else if (b1 == 1)
            p->v34(1);
    }
    return D_004C9098_0418;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003005E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3DD8_05E8 __asm__("D_004A3DD8");
extern char *D_004A47B8_05E8 __asm__("D_004A47B8");
struct O_05E8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); };
struct V3_05E8 { float x, y, z; };
extern V3_05E8 D_004C9098_05E8 __asm__("D_004C9098");
struct Tag_05E8 { char c; Tag_05E8() {} Tag_05E8(const Tag_05E8 &o) : c(o.c) {} };
extern unsigned D_05E8 __asm__("D_004A5678");
extern Tag_05E8 D_004A567C;
extern int D_004A5680;
extern int D_004A3C38;
extern "C" void* func_003E6574_05E8(void*, void*, int) __asm__("func_003E6574");
struct Ent_05E8 { int idx; int val; int pad; int type; };
extern "C" V3_05E8 func_003005E8(int n, Ent_05E8 *e) {
    if (D_004A5680 == 0) {
        D_05E8 = 0xFFFFFFFF;
        D_004A5680 = 1;
        D_004A567C = Tag_05E8(D_004A567C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_05E8(buf, &D_05E8, 8);
    Ent_05E8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C38)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(*(char**)&D_004A3DD8_05E8 + 0x290);
    } else {
        char *tb = *(char**)(*(char**)D_004A47B8_05E8 + 8);
        char *s = *(char**)(tb + (id & 0xFF) * 4);
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_05E8 *o = *(O_05E8**)(obj + 0xC);
    if (o)
        o->v35();
    return D_004C9098_05E8;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00300770);

INCLUDE_ASM("seg/seg_1FBE38", func_00300948);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00300B20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_0B20 { char c; Flag_0B20() {} Flag_0B20(const Flag_0B20& o) : c(o.c) {} };
struct Ent_0B20 { int idx; int val; int pad; int kind; };
struct R12_0B20 { int a; int b; int c; };
extern R12_0B20 D_004C9098_0B20 __asm__("D_004C9098");

extern int D_004A3C50_0B20[2] __asm__("D_004A3C50");
extern "C" void func_00358B28(unsigned, int);
extern int D_004A56A4_0B20 __asm__("D_004A56A4");
struct T12_0B20 { unsigned a; int b; Flag_0B20 f; };
extern T12_0B20 D_004FBB48_0B20 __asm__("D_004FBB48");
extern "C" R12_0B20 func_00300B20(int n, Ent_0B20* e) {
    if (D_004A56A4_0B20 == 0) {
        D_004FBB48_0B20.a = -1;
        D_004FBB48_0B20.b = 1;
        D_004A56A4_0B20 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBB48_0B20.f = Flag_0B20(D_004FBB48_0B20.f);
    }
    unsigned v[4] __attribute__((aligned(16)));
    unsigned* vp = v;
    vp[0] = 0xFFFFFFFFu;
    func_003E6574(vp, &D_004FBB48_0B20, 12);
    Ent_0B20* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3C50_0B20[k] && D_004A3C50_0B20[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    if (~vp[0] != 0) {
        int y = vp[1];
        if (y == 1)
            func_00358B28(vp[0], 1);
        else if (y == 0)
            func_00358B28(vp[0], 0);
    }
    return D_004C9098_0B20;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00300C78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A28A4_0C78 __asm__("D_004A28A4");
extern "C" void func_00278E50_0C78(int, int, int, int, int, int, int, int) __asm__("func_00278E50");
extern "C" void func_00278ED0_0C78(int, int, int, int, int, int, int) __asm__("func_00278ED0");
extern "C" void func_00278F38_0C78(int, int) __asm__("func_00278F38");
extern "C" void func_00278F68_0C78(int, int, int, int) __asm__("func_00278F68");
struct V3_0C78 { float x, y, z; };
extern V3_0C78 D_004C9098_0C78 __asm__("D_004C9098");
struct Tag_0C78 { char c; Tag_0C78() {} Tag_0C78(const Tag_0C78 &o) : c(o.c) {} };
extern unsigned D_0C78[] __asm__("D_004FBB58");
extern int D_004A56A8;
extern int D_004A3C58;
extern "C" void* func_003E6574_0C78(void*, void*, int) __asm__("func_003E6574");
struct Ent_0C78 { int idx; int val; int pad; int type; };
extern "C" V3_0C78 func_00300C78(int n, Ent_0C78 *e) {
    if (D_004A56A8 == 0) {
        D_0C78[0] = 0xFFFFFFFF;
        ((int*)D_0C78)[1] = 0;
        D_004A56A8 = 1;
        (*(Tag_0C78*)((char*)D_0C78 + 8)) = Tag_0C78((*(Tag_0C78*)((char*)D_0C78 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    func_003E6574_0C78(buf, D_0C78, 12);
    Ent_0C78 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C58)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    switch ((int)buf[1]) {
    case 0:
        func_00278ED0_0C78(D_004A28A4_0C78, 0, id, 3, -1, -1, -1);
        func_00278F38_0C78(D_004A28A4_0C78, 0);
        break;
    case 1:
        func_00278E50_0C78(D_004A28A4_0C78, 0, id, 3, 0, -1, -1, -1);
        func_00278F38_0C78(D_004A28A4_0C78, 0);
        break;
    case 2:
        func_00278F68_0C78(D_004A28A4_0C78, 0, 1, 0);
        break;
    }
    return D_004C9098_0C78;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00300E28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
struct Ent_0E28 { int idx; int val; int pad; int kind; };
struct R12_0E28 { int a; int b; int c; };
extern R12_0E28 D_004C9098_0E28 __asm__("D_004C9098");
struct Flag_0E28 { char c; Flag_0E28() {} Flag_0E28(const Flag_0E28& o) : c(o.c) {} };

extern int D_004A3C60_0E28[2] __asm__("D_004A3C60");
extern "C" void func_0030B928(int, unsigned, int);
extern int D_004A3DD8_0E28 __asm__("D_004A3DD8");
extern int D_004A56AC_0E28 __asm__("D_004A56AC");
struct T12u_0E28 { unsigned a; int b; Flag_0E28 f; };
extern T12u_0E28 D_004FBB68_0E28 __asm__("D_004FBB68");
extern "C" R12_0E28 func_00300E28(int n, Ent_0E28* e) {
    if (D_004A56AC_0E28 == 0) {
        D_004FBB68_0E28.a = 0xFFFFFFFFu;
        D_004FBB68_0E28.b = 1;
        D_004A56AC_0E28 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBB68_0E28.f = Flag_0E28(D_004FBB68_0E28.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBB68_0E28, 12);
    Ent_0E28* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3C60_0E28[k] && D_004A3C60_0E28[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu)
        func_0030B928(D_004A3DD8_0E28, x, vp[1]);
    return D_004C9098_0E28;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00300F50);

INCLUDE_ASM("seg/seg_1FBE38", func_00301120);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003012F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_12F0 { char c; Flag_12F0() {} Flag_12F0(const Flag_12F0& o) : c(o.c) {} };
struct Ent_12F0 { int idx; int val; int pad; int kind; };
struct R12_12F0 { int a; int b; int c; };
extern R12_12F0 D_004C9098_12F0 __asm__("D_004C9098");

extern int D_004A3C70_12F0[2] __asm__("D_004A3C70");
extern "C" int func_0030A868_12F0(int, unsigned, int, int) __asm__("func_0030A868");
extern int D_004A3DD8_12F0 __asm__("D_004A3DD8");
extern int D_004A56B8_12F0 __asm__("D_004A56B8");
struct T12_12F0 { unsigned a; int b; Flag_12F0 f; };
extern T12_12F0 D_004FBBE0_12F0 __asm__("D_004FBBE0");
extern "C" void* func_003012F0(void* self, int n, Ent_12F0* e) {
    if (D_004A56B8_12F0 == 0) {
        D_004FBBE0_12F0.a = -1;
        D_004FBBE0_12F0.b = -1;
        D_004A56B8_12F0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBBE0_12F0.f = Flag_12F0(D_004FBBE0_12F0.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBBE0_12F0, 12);
    Ent_12F0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3C70_12F0[k] && D_004A3C70_12F0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu) {
        int r = func_0030A868_12F0(D_004A3DD8_12F0, x, vp[1], -1);
        *(int*)((char*)self + 8) = 1;
        *func_00226610(self) = r;
    } else {
        *(R12_12F0*)self = D_004C9098_12F0;
    }
    return self;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00301440);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301560);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_1560 { char c; Flag_1560() {} Flag_1560(const Flag_1560& o) : c(o.c) {} };
struct Ent_1560 { int idx; int val; int pad; int kind; };
struct R12_1560 { int a; int b; int c; };
extern R12_1560 D_004C9098_1560 __asm__("D_004C9098");

extern int D_004A3C80_1560[2] __asm__("D_004A3C80");
extern "C" int func_0030BA80(int, unsigned);
extern int D_004A3DD8_1560 __asm__("D_004A3DD8");
extern int D_004A56C8_1560 __asm__("D_004A56C8");
extern unsigned D_004A56C0_1560 __asm__("D_004A56C0");
extern Flag_1560 D_004A56C4_1560 __asm__("D_004A56C4");
extern "C" void* func_00301560(void* self, int n, Ent_1560* e) {
    if (D_004A56C8_1560 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A56C4_1560 = Flag_1560(D_004A56C4_1560);
        D_004A56C0_1560 = -1;
        D_004A56C8_1560 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A56C0_1560, 8);
    Ent_1560* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3C80_1560[k] && D_004A3C80_1560[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu) {
        int r = func_0030BA80(D_004A3DD8_1560, x);
        *(int*)((char*)self + 8) = 1;
        *func_00226610(self) = r;
    } else {
        *(int*)((char*)self + 8) = 1;
        *func_00226610(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301680);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_1680 __asm__("D_004A3DD8");
extern char **D_004A47B8_1680 __asm__("D_004A47B8");
extern char *D_004A28A8_1680 __asm__("D_004A28A8");
static inline char *look_1680(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_1680 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_1680 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

float *func_00226618_1680(void*) __asm__("func_00226618__FPv");
struct O_1680 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual float v30(); };
struct V3_1680 { int w0, w1, w2; };
struct Tag_1680 { char c; Tag_1680() {} Tag_1680(const Tag_1680 &o) : c(o.c) {} };
extern unsigned D_1680 __asm__("D_004A56D0");
extern Tag_1680 D_004A56D4;
extern int D_004A56D8;
extern int D_004A3C88;
extern "C" void* func_003E6574_1680(void*, void*, int) __asm__("func_003E6574");
struct Ent_1680 { int idx; int val; int pad; int type; };
extern "C" V3_1680* func_00301680(V3_1680 *ret, int n, Ent_1680 *e) {
    if (D_004A56D8 == 0) {
        D_1680 = 0xFFFFFFFF;
        D_004A56D8 = 1;
        D_004A56D4 = Tag_1680(D_004A56D4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_1680(buf, &D_1680, 8);
    Ent_1680 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C88)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = look_1680(buf[0]);
    float v;
    if (obj) {
        O_1680 *p = *(O_1680**)(obj + 0xC);
        if (p)
            v = p->v30();
        else
            v = -1.0f;
        ((int*)ret)[2] = 2;
        *func_00226618_1680(ret) = v;
    } else {
        v = -1.0f;
        ((int*)ret)[2] = 2;
        *func_00226618_1680(ret) = v;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301830);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_1830 __asm__("D_004A3DD8");
extern char **D_004A47B8_1830 __asm__("D_004A47B8");
extern char *D_004A28A8_1830 __asm__("D_004A28A8");
static inline char *look_1830(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_1830 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_1830 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

int *func_00226610(void*);
struct O_1830 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual int v31(); };
struct V3_1830 { int w0, w1, w2; };
struct Tag_1830 { char c; Tag_1830() {} Tag_1830(const Tag_1830 &o) : c(o.c) {} };
extern unsigned D_1830 __asm__("D_004A56E0");
extern Tag_1830 D_004A56E4;
extern int D_004A56E8;
extern int D_004A3C90;
extern "C" void* func_003E6574_1830(void*, void*, int) __asm__("func_003E6574");
struct Ent_1830 { int idx; int val; int pad; int type; };
extern "C" V3_1830* func_00301830(V3_1830 *ret, int n, Ent_1830 *e) {
    if (D_004A56E8 == 0) {
        D_1830 = 0xFFFFFFFF;
        D_004A56E8 = 1;
        D_004A56E4 = Tag_1830(D_004A56E4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_1830(buf, &D_1830, 8);
    Ent_1830 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C90)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_1830 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_1830 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            r = 0;
        else
            r = (char*)(i << 2);
        obj = r;
    }
    if (obj) {
        O_1830 *p = *(O_1830**)(obj + 0xC);
        int v;
        if (p)
            v = p->v31();
        else
            v = 0;
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = v;
    } else {
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003019C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_19C8 __asm__("D_004A3DD8");
extern char **D_004A47B8_19C8 __asm__("D_004A47B8");
extern char *D_004A28A8_19C8 __asm__("D_004A28A8");
int *func_00226610(void*);
struct O_19C8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual int v32(float); };
struct V3_19C8 { int w0, w1, w2; };
struct Tag_19C8 { char c; Tag_19C8() {} Tag_19C8(const Tag_19C8 &o) : c(o.c) {} };
extern unsigned D_19C8[] __asm__("D_004FBC00");
extern int D_004A56EC;
extern int D_004A3C98;
extern "C" void* func_003E6574_19C8(void*, void*, int) __asm__("func_003E6574");
struct Ent_19C8 { int idx; int val; int pad; int type; };
extern "C" V3_19C8* func_003019C8(V3_19C8 *ret, int n, Ent_19C8 *e) {
    if (D_004A56EC == 0) {
        D_19C8[0] = 0xFFFFFFFF;
        *(float*)&D_19C8[1] = -1.0f;
        D_004A56EC = 1;
        (*(Tag_19C8*)((char*)D_19C8 + 8)) = Tag_19C8((*(Tag_19C8*)((char*)D_19C8 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_19C8(buf, D_19C8, 12);
    Ent_19C8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C98)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_19C8 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_19C8 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        O_19C8 *p = *(O_19C8**)(obj + 0xC);
        float f = *(float*)&buf[1];
        int v;
        if (p)
            v = p->v32(f * 0.03333333507180214f);
        else
            v = 0;
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = v;
    } else {
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1B88 { char c; Flag_1B88() {} Flag_1B88(const Flag_1B88& o) : c(o.c) {} };
extern unsigned D_004A56F0_1B88 __asm__("D_004A56F0");
extern Flag_1B88 D_004A56F4_1B88 __asm__("D_004A56F4");
extern int D_004A56F8_1B88 __asm__("D_004A56F8");
extern int D_004A3CA0_1B88[2] __asm__("D_004A3CA0");
extern int D_004A3DD8_1B88 __asm__("D_004A3DD8");
extern "C" void func_003E6574(void*, void*, int);
extern "C" void func_0030A6D8(int, int);
int* func_00226610(void*);

struct Ent_1B88 { int idx; int val; int pad; int kind; };

extern "C" void* func_00301B88(void* self, int n, Ent_1B88* e) {
    if (D_004A56F8_1B88 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A56F4_1B88 = Flag_1B88(D_004A56F4_1B88);
        D_004A56F0_1B88 = -1;
        D_004A56F8_1B88 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A56F0_1B88, 8);
    Ent_1B88* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CA0_1B88[k] && D_004A3CA0_1B88[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A6D8(D_004A3DD8_1B88, vp[0]);
    *(int*)((char*)self + 8) = 1;
    *func_00226610(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301C80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1C80 { char c; Flag_1C80() {} Flag_1C80(const Flag_1C80& o) : c(o.c) {} };
extern unsigned D_004A5700_1C80 __asm__("D_004A5700");
extern Flag_1C80 D_004A5704_1C80 __asm__("D_004A5704");
extern int D_004A5708_1C80 __asm__("D_004A5708");
extern int D_004A3CA8_1C80[2] __asm__("D_004A3CA8");
extern int D_004A3DD8_1C80 __asm__("D_004A3DD8");
extern "C" void func_003E6574(void*, void*, int);
extern "C" void func_0030A700(int, int);
int* func_00226610(void*);

struct Ent_1C80 { int idx; int val; int pad; int kind; };

extern "C" void* func_00301C80(void* self, int n, Ent_1C80* e) {
    if (D_004A5708_1C80 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A5704_1C80 = Flag_1C80(D_004A5704_1C80);
        D_004A5700_1C80 = -1;
        D_004A5708_1C80 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5700_1C80, 8);
    Ent_1C80* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CA8_1C80[k] && D_004A3CA8_1C80[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A700(D_004A3DD8_1C80, vp[0]);
    *(int*)((char*)self + 8) = 1;
    *func_00226610(self) = 0;
    return self;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00301D78);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00301F48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1F48 { char c; Flag_1F48() {} Flag_1F48(const Flag_1F48& o) : c(o.c) {} };
extern int D_004A5720_1F48 __asm__("D_004A5720");
extern Flag_1F48 D_004A5724_1F48 __asm__("D_004A5724");
extern int D_004A5728_1F48 __asm__("D_004A5728");
extern int D_004A3CB8_1F48[2] __asm__("D_004A3CB8");
extern "C" void func_003E6574(void*, void*, int);
extern "C" void func_00303D88_1F48(int) __asm__("func_00303D88");

struct Ent_1F48 { int idx; int val; int pad; int kind; };
struct R12_1F48 { int a; int b; int c; };
extern R12_1F48 D_004C9098_1F48 __asm__("D_004C9098");

extern "C" R12_1F48 func_00301F48(int n, Ent_1F48* e) {
    if (D_004A5728_1F48 == 0) {
        D_004A5720_1F48 = -1;
        D_004A5728_1F48 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A5724_1F48 = Flag_1F48(D_004A5724_1F48);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5720_1F48, 8);
    Ent_1F48* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CB8_1F48[k] && D_004A3CB8_1F48[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_00303D88_1F48(vp[0]);
    return D_004C9098_1F48;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A3DD8_2048 __asm__("D_004A3DD8");
extern char *D_004A28A8_2048 __asm__("D_004A28A8");
extern int D_00445E40_2048[] __asm__("D_00445E40");
extern "C" void func_0030B7F8_2048(int*, int) __asm__("func_0030B7F8");
extern "C" void func_0022D6C8_2048(int, int) __asm__("func_0022D6C8");
struct Sh_2048 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual int v09(); };
struct V3_2048 { float x, y, z; };
extern V3_2048 D_004C9098_2048 __asm__("D_004C9098");
struct Tag_2048 { char c; Tag_2048() {} Tag_2048(const Tag_2048 &o) : c(o.c) {} };
extern unsigned D_2048[] __asm__("D_004FBC10");
extern int D_004A572C;
extern int D_004A3CC0;
extern "C" void* func_003E6574_2048(void*, void*, int) __asm__("func_003E6574");
struct Ent_2048 { int idx; int val; int pad; int type; };
extern "C" V3_2048 func_00302048(int n, Ent_2048 *e) {
    if (D_004A572C == 0) {
        ((int*)D_2048)[0] = -1;
        ((int*)D_2048)[1] = -1;
        D_004A572C = 1;
        (*(Tag_2048*)((char*)D_2048 + 8)) = Tag_2048((*(Tag_2048*)((char*)D_2048 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    func_003E6574_2048(buf, D_2048, 12);
    Ent_2048 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3CC0)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    int r;
    int k = *D_004A3DD8_2048;
    if (k != -1) {
        int k4 = k * 4;
        char *lvl = *(char**)(*(char**)(D_004A28A8_2048 + 0x84) + 0xC);
        char *q = *(char**)(lvl + k4 + 0x28);
        char *a = q ? q + 0x6C0 : 0;
        char *b = a ? a - 0x6C0 : 0;
        Sh_2048 *sh = (Sh_2048*)(b + 0x6C0);
        r = sh->v09() ^ 1;
    } else {
        r = 0;
    }
    if (r) {
        int v16 = 0;
        unsigned id = buf[0];
        int b1 = buf[1];
        int w = *(int*)(*(char**)(D_004A28A8_2048 + 0x84) + 0x78);
        if (id < 8)
            v16 = D_00445E40_2048[id];
        if (b1 == 0) {
            if (v16 != 1)
                func_0030B7F8_2048(D_004A3DD8_2048, id);
            func_0022D6C8_2048(w, v16);
        }
    }
    return D_004C9098_2048;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00302210);

INCLUDE_ASM("seg/seg_1FBE38", func_00302490);

INCLUDE_ASM("seg/seg_1FBE38", func_00302680);

INCLUDE_ASM("seg/seg_1FBE38", func_00302778);

INCLUDE_ASM("seg/seg_1FBE38", func_00302870);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302968);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_2968 { char c; Flag_2968() {} Flag_2968(const Flag_2968& o) : c(o.c) {} };
struct Ent_2968 { int idx; int val; int pad; int kind; };
struct R12_2968 { int a; int b; int c; };
extern R12_2968 D_004C9098_2968 __asm__("D_004C9098");

extern int D_004A3CE8_2968[2] __asm__("D_004A3CE8");
extern char** D_004A47B8_2968 __asm__("D_004A47B8");
extern char* D_004A3DD8_2968 __asm__("D_004A3DD8");
extern int D_004A5770_2968 __asm__("D_004A5770");
extern unsigned D_004A5768_2968 __asm__("D_004A5768");
extern Flag_2968 D_004A576C_2968 __asm__("D_004A576C");
extern "C" R12_2968 func_00302968(int n, Ent_2968* e) {
    if (D_004A5770_2968 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A576C_2968 = Flag_2968(D_004A576C_2968);
        D_004A5768_2968 = -1;
        D_004A5770_2968 = 1;
    }
    unsigned v[4] __attribute__((aligned(16)));
    unsigned* vp = v;
    vp[0] = 0xFFFFFFFFu;
    func_003E6574(vp, &D_004A5768_2968, 8);
    Ent_2968* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CE8_2968[k] && D_004A3CE8_2968[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    char* g = D_004A3DD8_2968;
    unsigned x = vp[0];
    char** tbl = *(char***)(*D_004A47B8_2968 + 8);
    char* e0 = tbl[x & 0xFF];
    unsigned w;
    int r;
    if (e0 == 0 || (w = (*(unsigned**)(e0 + 0x1C))[x >> 8] >> 8) == 0)
        r = 0;
    else
        r = w << 2;
    *(int*)(g + 0x290) = r;
    return D_004C9098_2968;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302AC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_2AC0 { char c; Flag_2AC0() {} Flag_2AC0(const Flag_2AC0& o) : c(o.c) {} };
struct Ent_2AC0 { int idx; int val; int pad; int kind; };
struct R12_2AC0 { int a; int b; int c; };
extern R12_2AC0 D_004C9098_2AC0 __asm__("D_004C9098");

extern int D_004A3CF0_2AC0[2] __asm__("D_004A3CF0");
extern "C" void func_0030A728(int, int, int);
extern int D_004A3DD8_2AC0 __asm__("D_004A3DD8");
extern int D_004A5774_2AC0 __asm__("D_004A5774");
struct T12_2AC0 { unsigned a; int b; Flag_2AC0 f; };
extern T12_2AC0 D_004FBC40_2AC0 __asm__("D_004FBC40");
extern "C" R12_2AC0 func_00302AC0(int n, Ent_2AC0* e) {
    if (D_004A5774_2AC0 == 0) {
        D_004FBC40_2AC0.a = -1;
        D_004FBC40_2AC0.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBC40_2AC0.f = Flag_2AC0(D_004FBC40_2AC0.f);
        D_004A5774_2AC0 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBC40_2AC0, 12);
    Ent_2AC0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CF0_2AC0[k] && D_004A3CF0_2AC0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A728(D_004A3DD8_2AC0, vp[0], vp[1]);
    return D_004C9098_2AC0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302BD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_2BD8 { char c; Flag_2BD8() {} Flag_2BD8(const Flag_2BD8& o) : c(o.c) {} };
struct Ent_2BD8 { int idx; int val; int pad; int kind; };
struct R12_2BD8 { int a; int b; int c; };
extern R12_2BD8 D_004C9098_2BD8 __asm__("D_004C9098");

extern int D_004A3CF8_2BD8[2] __asm__("D_004A3CF8");
extern "C" void func_0030A868(int, int, int, int);
extern int D_004A3DD8_2BD8 __asm__("D_004A3DD8");
extern int D_004A5780_2BD8 __asm__("D_004A5780");
extern unsigned D_004A5778_2BD8 __asm__("D_004A5778");
extern Flag_2BD8 D_004A577C_2BD8 __asm__("D_004A577C");
extern "C" R12_2BD8 func_00302BD8(int n, Ent_2BD8* e) {
    if (D_004A5780_2BD8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A577C_2BD8 = Flag_2BD8(D_004A577C_2BD8);
        D_004A5778_2BD8 = -1;
        D_004A5780_2BD8 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5778_2BD8, 8);
    Ent_2BD8* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3CF8_2BD8[k] && D_004A3CF8_2BD8[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A868(D_004A3DD8_2BD8, vp[0], 0, -1);
    return D_004C9098_2BD8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302CE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_2CE8 { char c; Flag_2CE8() {} Flag_2CE8(const Flag_2CE8& o) : c(o.c) {} };
struct Ent_2CE8 { int idx; int val; int pad; int kind; };
struct R12_2CE8 { int a; int b; int c; };
extern R12_2CE8 D_004C9098_2CE8 __asm__("D_004C9098");

extern int D_004A3D00_2CE8[2] __asm__("D_004A3D00");
extern "C" void func_0030A728(int, int, int);
extern int D_004A3DD8_2CE8 __asm__("D_004A3DD8");
extern int D_004A5790_2CE8 __asm__("D_004A5790");
extern unsigned D_004A5788_2CE8 __asm__("D_004A5788");
extern Flag_2CE8 D_004A578C_2CE8 __asm__("D_004A578C");
extern "C" R12_2CE8 func_00302CE8(int n, Ent_2CE8* e) {
    if (D_004A5790_2CE8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A578C_2CE8 = Flag_2CE8(D_004A578C_2CE8);
        D_004A5788_2CE8 = -1;
        D_004A5790_2CE8 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5788_2CE8, 8);
    Ent_2CE8* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D00_2CE8[k] && D_004A3D00_2CE8[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A728(D_004A3DD8_2CE8, vp[0], 0);
    return D_004C9098_2CE8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00302DF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_2DF0 { char c; Flag_2DF0() {} Flag_2DF0(const Flag_2DF0& o) : c(o.c) {} };
struct Ent_2DF0 { int idx; int val; int pad; int kind; };
struct R12_2DF0 { int a; int b; int c; };
extern R12_2DF0 D_004C9098_2DF0 __asm__("D_004C9098");

extern int D_004A3D08_2DF0[2] __asm__("D_004A3D08");
extern "C" void func_0030A728(int, int, int);
extern int D_004A3DD8_2DF0 __asm__("D_004A3DD8");
extern int D_004A57A0_2DF0 __asm__("D_004A57A0");
extern unsigned D_004A5798_2DF0 __asm__("D_004A5798");
extern Flag_2DF0 D_004A579C_2DF0 __asm__("D_004A579C");
extern "C" R12_2DF0 func_00302DF0(int n, Ent_2DF0* e) {
    if (D_004A57A0_2DF0 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A579C_2DF0 = Flag_2DF0(D_004A579C_2DF0);
        D_004A5798_2DF0 = -1;
        D_004A57A0_2DF0 = 1;
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5798_2DF0, 8);
    Ent_2DF0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D08_2DF0[k] && D_004A3D08_2DF0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030A728(D_004A3DD8_2DF0, vp[0], 2);
    return D_004C9098_2DF0;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00302EF8);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00303130);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_3130 __asm__("D_004A3DD8");
extern char **D_004A47B8_3130 __asm__("D_004A47B8");
extern char *D_004A28A8_3130 __asm__("D_004A28A8");
static inline char *look_3130(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_3130 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_3130 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

int *func_00226610(void*);
struct V3_3130 { int w0, w1, w2; };
struct Tag_3130 { char c; Tag_3130() {} Tag_3130(const Tag_3130 &o) : c(o.c) {} };
extern unsigned D_3130[] __asm__("D_004FBC50");
extern int D_004A57B4;
extern int D_004A3D18;
extern "C" void* func_003E6574_3130(void*, void*, int) __asm__("func_003E6574");
struct Ent_3130 { int idx; int val; int pad; int type; };
extern "C" V3_3130* func_00303130(V3_3130 *ret, int n, Ent_3130 *e) {
    if (D_004A57B4 == 0) {
        D_3130[0] = 0xFFFFFFFF;
        D_3130[1] = 1;
        D_004A57B4 = 1;
        (*(Tag_3130*)((char*)D_3130 + 8)) = Tag_3130((*(Tag_3130*)((char*)D_3130 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_3130(buf, D_3130, 12);
    Ent_3130 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3D18)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = look_3130(buf[0]);
    if (obj && *(int*)(obj + 0xC)) {
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = 1;
    } else {
        ((int*)ret)[2] = 1;
        *func_00226610(ret) = 0;
    }
    return ret;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_003032C0);

INCLUDE_ASM("seg/seg_1FBE38", func_00303380);

INCLUDE_ASM("seg/seg_1FBE38", func_00303430);

INCLUDE_ASM("seg/seg_1FBE38", func_00303490);

INCLUDE_ASM("seg/seg_1FBE38", func_003034F8);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00303598);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_3598 { char c; Flag_3598() {} Flag_3598(const Flag_3598& o) : c(o.c) {} };
struct Ent_3598 { int idx; int val; int pad; int kind; };
struct R12_3598 { int a; int b; int c; };
extern R12_3598 D_004C9098_3598 __asm__("D_004C9098");

extern int D_004A3D20_3598[2] __asm__("D_004A3D20");
float AIrandf(float, float);
void* func_00226618(void*);
extern int D_004A57B8_3598 __asm__("D_004A57B8");
struct T12_3598 { float a; float b; Flag_3598 f; };
extern T12_3598 D_004FBC60_3598 __asm__("D_004FBC60");
extern "C" void* func_00303598(void* self, int n, Ent_3598* e) {
    if (D_004A57B8_3598 == 0) {
        D_004FBC60_3598.a = 0;
        D_004FBC60_3598.b = 1.0f;
        D_004A57B8_3598 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBC60_3598.f = Flag_3598(D_004FBC60_3598.f);
    }
    float v[4] __attribute__((aligned(16)));
    float* vp = v;
    func_003E6574(vp, &D_004FBC60_3598, 12);
    Ent_3598* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D20_3598[k] && D_004A3D20_3598[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    float r = AIrandf(vp[0], vp[1]);
    *(int*)((char*)self + 8) = 2;
    *(float*)func_00226618(self) = r;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003036B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_36B0 { char c; Flag_36B0() {} Flag_36B0(const Flag_36B0& o) : c(o.c) {} };
extern int D_004A57C0_36B0 __asm__("D_004A57C0");
extern Flag_36B0 D_004A57C4_36B0 __asm__("D_004A57C4");
extern int D_004A57C8_36B0 __asm__("D_004A57C8");
extern int D_004A3D28_36B0[2] __asm__("D_004A3D28");
extern "C" void func_003E6574(void*, void*, int);
extern "C" int func_0030AF08(int*, int, int);
extern int* D_004A3DD8_36B0 __asm__("D_004A3DD8");
int* func_00226610(void*);

struct Ent_36B0 { int idx; int val; int pad; int kind; };

extern "C" void* func_003036B0(void* self, int n, Ent_36B0* e) {
    if (D_004A57C8_36B0 == 0) {
        D_004A57C0_36B0 = -1;
        D_004A57C8_36B0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A57C4_36B0 = Flag_36B0(D_004A57C4_36B0);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A57C0_36B0, 8);
    Ent_36B0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D28_36B0[k] && D_004A3D28_36B0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    int r = func_0030AF08(D_004A3DD8_36B0, *D_004A3DD8_36B0, vp[0]);
    *(int*)((char*)self + 8) = 1;
    *func_00226610(self) = r;
    return self;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_003037B0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003039F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_39F0 __asm__("D_004A3DD8");
extern char **D_004A47B8_39F0 __asm__("D_004A47B8");
extern char *D_004A28A8_39F0 __asm__("D_004A28A8");
extern int D_004A3FF0_39F0 __asm__("D_004A3FF0");
extern "C" void func_00343C60_39F0(int, int) __asm__("func_00343C60");
extern "C" void func_00343F38_39F0(int, int) __asm__("func_00343F38");
struct V3_39F0 { float x, y, z; };
extern V3_39F0 D_004C9098_39F0 __asm__("D_004C9098");
struct Tag_39F0 { char c; Tag_39F0() {} Tag_39F0(const Tag_39F0 &o) : c(o.c) {} };
extern unsigned D_39F0[] __asm__("D_004FBC70");
extern int D_004A57DC;
extern int D_004A3D38;
extern "C" void* func_003E6574_39F0(void*, void*, int) __asm__("func_003E6574");
struct Ent_39F0 { int idx; int val; int pad; int type; };
extern "C" V3_39F0 func_003039F0(int n, Ent_39F0 *e) {
    if (D_004A57DC == 0) {
        D_39F0[0] = 0xFFFFFFFF;
        ((int*)D_39F0)[1] = -1;
        D_004A57DC = 1;
        (*(Tag_39F0*)((char*)D_39F0 + 8)) = Tag_39F0((*(Tag_39F0*)((char*)D_39F0 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_39F0(buf, D_39F0, 12);
    Ent_39F0 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3D38)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_39F0 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_39F0 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        int b1 = buf[1];
        int w = *(int*)(obj + 0x78);
        if (b1 == 0)
            func_00343C60_39F0(D_004A3FF0_39F0, w);
        else if (b1 == 1)
            func_00343F38_39F0(D_004A3FF0_39F0, w);
    }
    return D_004C9098_39F0;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00303BA0);

extern "C" void func_00303D88(void) {
}

INCLUDE_ASM("seg/seg_1FBE38", func_00303D90);

INCLUDE_ASM("seg/seg_1FBE38", func_00303DF8);

INCLUDE_ASM("seg/seg_1FBE38", func_00303E60);

INCLUDE_ASM("seg/seg_1FBE38", func_00303EC8);

INCLUDE_ASM("seg/seg_1FBE38", func_00303F80);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00303FF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_3FF0 { char c; Flag_3FF0() {} Flag_3FF0(const Flag_3FF0& o) : c(o.c) {} };
struct Ent_3FF0 { int idx; int val; int pad; int kind; };
struct R12_3FF0 { int a; int b; int c; };
extern R12_3FF0 D_004C9098_3FF0 __asm__("D_004C9098");

extern int D_004A3D40_3FF0[2] __asm__("D_004A3D40");
void func_0030B4C0(void*, int, int);
extern void* D_004A3DD8_3FF0 __asm__("D_004A3DD8");
extern int D_004A57E4_3FF0 __asm__("D_004A57E4");
struct T12_3FF0 { int a; int b; Flag_3FF0 f; };
extern T12_3FF0 D_004FBC98_3FF0 __asm__("D_004FBC98");
extern "C" R12_3FF0 func_00303FF0(int n, Ent_3FF0* e) {
    if (D_004A57E4_3FF0 == 0) {
        D_004FBC98_3FF0.a = 0;
        D_004FBC98_3FF0.b = -1;
        D_004A57E4_3FF0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBC98_3FF0.f = Flag_3FF0(D_004FBC98_3FF0.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBC98_3FF0, 12);
    Ent_3FF0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D40_3FF0[k] && D_004A3D40_3FF0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030B4C0(D_004A3DD8_3FF0, vp[0], vp[1]);
    return D_004C9098_3FF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304100);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_4100 { char c; Flag_4100() {} Flag_4100(const Flag_4100& o) : c(o.c) {} };
struct Ent_4100 { int idx; int val; int pad; int kind; };
struct R12_4100 { int a; int b; int c; };
extern R12_4100 D_004C9098_4100 __asm__("D_004C9098");

extern int D_004A3D48_4100[2] __asm__("D_004A3D48");
void func_0030B4D8(void*, int, int);
extern void* D_004A3DD8_4100 __asm__("D_004A3DD8");
extern int D_004A57E8_4100 __asm__("D_004A57E8");
struct T12_4100 { int a; int b; Flag_4100 f; };
extern T12_4100 D_004FBCA8_4100 __asm__("D_004FBCA8");
extern "C" R12_4100 func_00304100(int n, Ent_4100* e) {
    if (D_004A57E8_4100 == 0) {
        D_004FBCA8_4100.a = 0;
        D_004A57E8_4100 = 1;
        D_004FBCA8_4100.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBCA8_4100.f = Flag_4100(D_004FBCA8_4100.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBCA8_4100, 12);
    Ent_4100* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D48_4100[k] && D_004A3D48_4100[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030B4D8(D_004A3DD8_4100, vp[0], vp[1]);
    return D_004C9098_4100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304210);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_4210 { char c; Flag_4210() {} Flag_4210(const Flag_4210& o) : c(o.c) {} };
struct Ent_4210 { int idx; int val; int pad; int kind; };
struct R12_4210 { int a; int b; int c; };
extern R12_4210 D_004C9098_4210 __asm__("D_004C9098");

extern int D_004A3D50_4210[2] __asm__("D_004A3D50");
void func_0030B4F0(void*, int, int);
extern void* D_004A3DD8_4210 __asm__("D_004A3DD8");
extern int D_004A57EC_4210 __asm__("D_004A57EC");
struct T12_4210 { int a; int b; Flag_4210 f; };
extern T12_4210 D_004FBCB8_4210 __asm__("D_004FBCB8");
extern "C" R12_4210 func_00304210(int n, Ent_4210* e) {
    if (D_004A57EC_4210 == 0) {
        D_004FBCB8_4210.a = 0;
        D_004FBCB8_4210.b = -1;
        D_004A57EC_4210 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBCB8_4210.f = Flag_4210(D_004FBCB8_4210.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBCB8_4210, 12);
    Ent_4210* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D50_4210[k] && D_004A3D50_4210[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_0030B4F0(D_004A3DD8_4210, vp[0], vp[1]);
    return D_004C9098_4210;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00304320);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003044A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_44A8 { char c; Flag_44A8() {} Flag_44A8(const Flag_44A8& o) : c(o.c) {} };
struct Ent_44A8 { int idx; int val; int pad; int kind; };
struct R12_44A8 { int a; int b; int c; };
extern R12_44A8 D_004C9098_44A8 __asm__("D_004C9098");

extern int D_004A3D60_44A8[2] __asm__("D_004A3D60");
extern "C" void func_00238550(int, int, int);
extern int* D_004A3DD8_44A8 __asm__("D_004A3DD8");
extern char* D_004A28A8_44A8 __asm__("D_004A28A8");
extern int D_004A5808_44A8 __asm__("D_004A5808");
extern int D_004A5800_44A8 __asm__("D_004A5800");
extern Flag_44A8 D_004A5804_44A8 __asm__("D_004A5804");
extern "C" R12_44A8 func_003044A8(int n, Ent_44A8* e) {
    if (D_004A5808_44A8 == 0) {
        D_004A5800_44A8 = -1;
        D_004A5808_44A8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A5804_44A8 = Flag_44A8(D_004A5804_44A8);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5800_44A8, 8);
    Ent_44A8* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D60_44A8[k] && D_004A3D60_44A8[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    int b = *D_004A3DD8_44A8;
    func_00238550(*(int*)(D_004A28A8_44A8 + 0xC0), b, vp[0]);
    return D_004C9098_44A8;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_003045B8);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304980);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_4980 __asm__("D_004A3DD8");
extern char **D_004A47B8_4980 __asm__("D_004A47B8");
extern char *D_004A28A8_4980 __asm__("D_004A28A8");
void func_00355878(void*, void*);
struct O_4980 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_4980 { float x, y, z; };
extern V3_4980 D_004C9098_4980 __asm__("D_004C9098");
struct Tag_4980 { char c; Tag_4980() {} Tag_4980(const Tag_4980 &o) : c(o.c) {} };
extern unsigned D_4980[] __asm__("D_004FBD10");
extern int D_004A5810;
extern int D_004A3D68;
extern "C" void* func_003E6574_4980(void*, void*, int) __asm__("func_003E6574");
struct Ent_4980 { int idx; int val; int pad; int type; };
extern "C" V3_4980 func_00304980(int n, Ent_4980 *e) {
    if (D_004A5810 == 0) {
        D_4980[0] = 0xFFFFFFFF;
        *(float*)&D_4980[1] = 1.0f;
        D_004A5810 = 1;
        (*(Tag_4980*)((char*)D_4980 + 8)) = Tag_4980((*(Tag_4980*)((char*)D_4980 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_4980(buf, D_4980, 12);
    Ent_4980 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3D68)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_4980 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_4980 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_4980 *p = *(O_4980**)(obj + 0xC);
    if (p == 0)
        return D_004C9098_4980;
    if (p->v16())
        func_00355878(p, buf);
    return D_004C9098_4980;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304B38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_4B38 { char c; Flag_4B38() {} Flag_4B38(const Flag_4B38& o) : c(o.c) {} };
struct Ent_4B38 { int idx; int val; int pad; int kind; };
struct R12_4B38 { int a; int b; int c; };
extern R12_4B38 D_004C9098_4B38 __asm__("D_004C9098");

extern int D_004A3D70_4B38[2] __asm__("D_004A3D70");
extern "C" void func_002709D8(int, int, int);
extern int* D_004A3DD8_4B38 __asm__("D_004A3DD8");
extern char* D_004A28A8_4B38 __asm__("D_004A28A8");
extern int D_004A5820_4B38 __asm__("D_004A5820");
extern int D_004A5818_4B38 __asm__("D_004A5818");
extern Flag_4B38 D_004A581C_4B38 __asm__("D_004A581C");
extern "C" R12_4B38 func_00304B38(int n, Ent_4B38* e) {
    if (D_004A5820_4B38 == 0) {
        D_004A5818_4B38 = -1;
        D_004A5820_4B38 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A581C_4B38 = Flag_4B38(D_004A581C_4B38);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5818_4B38, 8);
    Ent_4B38* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D70_4B38[k] && D_004A3D70_4B38[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    func_002709D8(*(int*)(*(char**)(D_004A28A8_4B38 + 0x84) + 0x28), *D_004A3DD8_4B38, vp[0]);
    return D_004C9098_4B38;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00304C50);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304E38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_4E38 __asm__("D_004A3DD8");
extern char **D_004A47B8_4E38 __asm__("D_004A47B8");
extern char *D_004A28A8_4E38 __asm__("D_004A28A8");
extern "C" void func_00355888_4E38(void*, void*) __asm__("func_00355888");
struct O_4E38 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_4E38 { float x, y, z; };
extern V3_4E38 D_004C9098_4E38 __asm__("D_004C9098");
struct Tag_4E38 { char c; Tag_4E38() {} Tag_4E38(const Tag_4E38 &o) : c(o.c) {} };
extern unsigned D_4E38[] __asm__("D_004FBD20");
extern int D_004A5834;
extern int D_004A3D80;
extern "C" void* func_003E6574_4E38(void*, void*, int) __asm__("func_003E6574");
struct Ent_4E38 { int idx; int val; int pad; int type; };
extern "C" V3_4E38 func_00304E38(int n, Ent_4E38 *e) {
    if (D_004A5834 == 0) {
        D_4E38[0] = 0xFFFFFFFF;
        *(float*)&D_4E38[1] = 1.0f;
        D_004A5834 = 1;
        (*(Tag_4E38*)((char*)D_4E38 + 8)) = Tag_4E38((*(Tag_4E38*)((char*)D_4E38 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    func_003E6574_4E38(buf, D_4E38, 12);
    Ent_4E38 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3D80)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_4E38 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_4E38 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_4E38 *p = *(O_4E38**)(obj + 0xC);
    if (p == 0)
        return D_004C9098_4E38;
    if (p->v16())
        func_00355888_4E38(p, buf);
    return D_004C9098_4E38;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00304FF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A5838_4FF0 __asm__("D_004A5838");
extern int D_004A3D88_4FF0[2] __asm__("D_004A3D88");
extern "C" void func_003E6574(void*, void*, int);

struct Flag_4FF0 { char c; Flag_4FF0() {} Flag_4FF0(const Flag_4FF0& o) : c(o.c) {} };
struct Ent_4FF0 { int idx; int val; int pad; int kind; };
struct R12_4FF0 { int a; int b; int c; };
struct T12_4FF0 { int a; int b; Flag_4FF0 f; };
extern T12_4FF0 D_004FBD30_4FF0 __asm__("D_004FBD30");
extern R12_4FF0 D_004C9098_4FF0 __asm__("D_004C9098");

extern "C" R12_4FF0 func_00304FF0(int n, Ent_4FF0* e) {
    if (D_004A5838_4FF0 == 0) {
        D_004FBD30_4FF0.a = 0;
        D_004A5838_4FF0 = 1;
        D_004FBD30_4FF0.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBD30_4FF0.f = Flag_4FF0(D_004FBD30_4FF0.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBD30_4FF0, 12);
    Ent_4FF0* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D88_4FF0[k] && D_004A3D88_4FF0[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    return D_004C9098_4FF0;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_003050F0);

INCLUDE_ASM("seg/seg_1FBE38", func_00305478);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00305660);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *D_004A3DD8_5660 __asm__("D_004A3DD8");
extern char *D_004A28A8_5660 __asm__("D_004A28A8");
struct Sh_5660 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual int v08(); };
extern "C" void func_00390EC8_5660(float) __asm__("func_00390EC8");
struct V3_5660 { float x, y, z; };
extern V3_5660 D_004C9098_5660 __asm__("D_004C9098");
struct Tag_5660 { char c; Tag_5660() {} Tag_5660(const Tag_5660 &o) : c(o.c) {} };
extern unsigned D_5660 __asm__("D_004A5848");
extern Tag_5660 D_004A584C;
extern int D_004A5850;
extern int D_004A3D90;
extern "C" void* func_003E6574_5660(void*, void*, int) __asm__("func_003E6574");
struct Ent_5660 { int idx; int val; int pad; int type; };
extern "C" V3_5660 func_00305660(int n, Ent_5660 *e) {
    if (D_004A5850 == 0) {
        D_5660 = 0xFFFFFFFF;
        D_004A5850 = 1;
        D_004A584C = Tag_5660(D_004A584C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_5660(buf, &D_5660, 8);
    Ent_5660 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3D90)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    int k = *D_004A3DD8_5660;
    if (k != -1) {
        int k4 = k * 4;
        char *lvl = *(char**)(*(char**)(D_004A28A8_5660 + 0x84) + 0xC);
        char *q = *(char**)(lvl + k4 + 0x28);
        Sh_5660 *sh = q ? (Sh_5660*)(q + 0x6C0) : 0;
        if (sh->v08())
            func_00390EC8_5660(0.0f);
    }
    return D_004C9098_5660;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_003057C0);

INCLUDE_ASM("seg/seg_1FBE38", func_003059A0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00305C88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_5C88 { char c; Flag_5C88() {} Flag_5C88(const Flag_5C88& o) : c(o.c) {} };
struct Ent_5C88 { int idx; int val; int pad; int kind; };
struct R12_5C88 { int a; int b; int c; };
extern R12_5C88 D_004C9098_5C88 __asm__("D_004C9098");

extern int D_004A3D98_5C88[2] __asm__("D_004A3D98");
extern "C" int cAvalanche_triggerAvalanche(int);
extern int D_004A3AC4_5C88 __asm__("D_004A3AC4");
extern int D_004A5868_5C88 __asm__("D_004A5868");
extern int D_004A5860_5C88 __asm__("D_004A5860");
extern Flag_5C88 D_004A5864_5C88 __asm__("D_004A5864");
extern "C" R12_5C88 func_00305C88(int n, Ent_5C88* e) {
    if (D_004A5868_5C88 == 0) {
        D_004A5868_5C88 = 1;
        D_004A5860_5C88 = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004A5864_5C88 = Flag_5C88(D_004A5864_5C88);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004A5860_5C88, 8);
    Ent_5C88* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3D98_5C88[k] && D_004A3D98_5C88[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    int a = vp[0];
    if (D_004A3AC4_5C88 == 0)
        cAvalanche_triggerAvalanche(a);
    return D_004C9098_5C88;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00305D90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_5D90 __asm__("D_004A3DD8");
extern char **D_004A47B8_5D90 __asm__("D_004A47B8");
extern char *D_004A28A8_5D90 __asm__("D_004A28A8");
static inline char *refToPtr_5D90(unsigned i) { return (char*)(i << 2); }
static inline char *conv_5D90(unsigned i) { if (i == 0) return 0; return refToPtr_5D90(i); }
extern "C" void func_00355A78_5D90(void*, void*) __asm__("func_00355A78");
struct O_5D90 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_5D90 { float x, y, z; };
extern V3_5D90 D_004C9098_5D90 __asm__("D_004C9098");
struct Tag_5D90 { char c; Tag_5D90() {} Tag_5D90(const Tag_5D90 &o) : c(o.c) {} };
extern unsigned D_5D90 __asm__("D_004A5870");
extern Tag_5D90 D_004A5874;
extern int D_004A5878;
extern int D_004A3DA0;
extern "C" void* func_003E6574_5D90(void*, void*, int) __asm__("func_003E6574");
struct Ent_5D90 { int idx; int val; int pad; int type; };
extern "C" V3_5D90 func_00305D90(int n, Ent_5D90 *e) {
    if (D_004A5878 == 0) {
        D_5D90 = 0xFFFFFFFF;
        D_004A5878 = 1;
        D_004A5874 = Tag_5D90(D_004A5874);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_5D90(buf, &D_5D90, 8);
    Ent_5D90 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3DA0)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_5D90 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_5D90 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_5D90(i);
            r = t;
        }
        obj = r;
    }
    if (obj) {
        O_5D90 *p = *(O_5D90**)(obj + 0xC);
        if (p) {
            if (p->v16())
                func_00355A78_5D90(p, obj);
        }
    }
    return D_004C9098_5D90;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00305F40);

INCLUDE_ASM("seg/seg_1FBE38", func_003061B0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306300);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_6300 { char c; Flag_6300() {} Flag_6300(const Flag_6300& o) : c(o.c) {} };
struct Ent_6300 { int idx; int val; int pad; int kind; };
struct R12_6300 { int a; int b; int c; };
extern R12_6300 D_004C9098_6300 __asm__("D_004C9098");

extern int D_004A3DB0_6300[2] __asm__("D_004A3DB0");
extern "C" void func_001E3510(int, int);
extern int D_00534B30_6300[] __asm__("D_00534B30");
extern "C" void* cBE_getInterface_6300(int, int) __asm__("cBE_getInterface__Fv");
extern char* D_004A28A8_6300 __asm__("D_004A28A8");
extern int D_004A588C_6300 __asm__("D_004A588C");
struct T12_6300 { int a; int b; Flag_6300 f; };
extern T12_6300 D_004FBF38_6300 __asm__("D_004FBF38");
extern "C" R12_6300 func_00306300(int n, Ent_6300* e) {
    if (D_004A588C_6300 == 0) {
        D_004FBF38_6300.a = 0;
        D_004A588C_6300 = 1;
        D_004FBF38_6300.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FBF38_6300.f = Flag_6300(D_004FBF38_6300.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FBF38_6300, 12);
    Ent_6300* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3DB0_6300[k] && D_004A3DB0_6300[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    int x = vp[0];
    int y = vp[1];
    cBE_getInterface_6300(*(int*)(D_004A28A8_6300 + 0x78), 7);
    if (D_00534B30_6300[0] == 0)
        func_001E3510(x, y);
    return D_004C9098_6300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306438);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_6438 __asm__("D_004A3DD8");
extern char **D_004A47B8_6438 __asm__("D_004A47B8");
extern char *D_004A28A8_6438 __asm__("D_004A28A8");
extern "C" void func_00101728_6438(void*, void*, float, float) __asm__("func_00101728");
struct V3_6438 { float x, y, z; };
extern V3_6438 D_004C9098_6438 __asm__("D_004C9098");
struct Tag_6438 { char c; Tag_6438() {} Tag_6438(const Tag_6438 &o) : c(o.c) {} };
extern unsigned D_6438[] __asm__("D_004FBF48");
extern int D_004A5890;
extern const int D_00446880[];
extern "C" void* func_003E6574_6438(void*, void*, int) __asm__("func_003E6574");
struct Ent_6438 { int idx; int val; int pad; int type; };
extern "C" V3_6438 func_00306438(int n, Ent_6438 *e) {
    if (D_004A5890 == 0) {
        D_6438[0] = 0xFFFFFFFF;
        *(float*)&D_6438[1] = 1000.0f;
        ((int*)D_6438)[2] = 0;
        D_004A5890 = 1;
        (*(Tag_6438*)((char*)D_6438 + 12)) = Tag_6438((*(Tag_6438*)((char*)D_6438 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_6438(buf, D_6438, 16);
    Ent_6438 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446880[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_6438 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_6438 + 8))[id & 0xFF];
        unsigned i;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        float f1 = *(float*)&buf[1];
        float f2 = *(float*)&buf[2];
        int tmp[1];
        tmp[0] = *(int*)(obj + 0x78);
        func_00101728_6438(*(void**)(*(char**)(*(char**)(D_004A28A8_6438 + 0x84) + 0xC) + 0xA8), tmp, f1, f2);
    }
    return D_004C9098_6438;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_003065E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_65E8 __asm__("D_004A3DD8");
extern char **D_004A47B8_65E8 __asm__("D_004A47B8");
extern char *D_004A28A8_65E8 __asm__("D_004A28A8");
static inline char *look_65E8(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_65E8 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_65E8 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

extern "C" void func_00101888_65E8(void*, void*) __asm__("func_00101888");
struct V3_65E8 { float x, y, z; };
extern V3_65E8 D_004C9098_65E8 __asm__("D_004C9098");
struct Tag_65E8 { char c; Tag_65E8() {} Tag_65E8(const Tag_65E8 &o) : c(o.c) {} };
extern unsigned D_65E8 __asm__("D_004A5898");
extern Tag_65E8 D_004A589C;
extern int D_004A58A0;
extern int D_004A3DB8;
extern "C" void* func_003E6574_65E8(void*, void*, int) __asm__("func_003E6574");
struct Ent_65E8 { int idx; int val; int pad; int type; };
extern "C" V3_65E8 func_003065E8(int n, Ent_65E8 *e) {
    if (D_004A58A0 == 0) {
        D_65E8 = 0xFFFFFFFF;
        D_004A58A0 = 1;
        D_004A589C = Tag_65E8(D_004A589C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_65E8(buf, &D_65E8, 8);
    Ent_65E8 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3DB8)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = look_65E8(buf[0]);
    if (obj) {
        int tmp[1]; tmp[0] = *(int*)(obj + 0x78);
        func_00101888_65E8(*(void**)(*(char**)(*(char**)(D_004A28A8_65E8 + 0x84) + 0xC) + 0xA8), tmp);
    }
    return D_004C9098_65E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306770);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_6770 __asm__("D_004A3DD8");
extern char **D_004A47B8_6770 __asm__("D_004A47B8");
extern char *D_004A28A8_6770 __asm__("D_004A28A8");
static inline char *look_6770(unsigned id) {
    if (~id == 0)
        return *(char**)(D_004A3DD8_6770 + 0x290);
    char *s = (*(char***)((char*)*D_004A47B8_6770 + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

extern "C" void func_00101900_6770(void*, void*, int) __asm__("func_00101900");
struct V3_6770 { float x, y, z; };
extern V3_6770 D_004C9098_6770 __asm__("D_004C9098");
struct Tag_6770 { char c; Tag_6770() {} Tag_6770(const Tag_6770 &o) : c(o.c) {} };
extern unsigned D_6770[] __asm__("D_004FBF58");
extern int D_004A58A4;
extern int D_004A3DC0;
extern "C" void* func_003E6574_6770(void*, void*, int) __asm__("func_003E6574");
struct Ent_6770 { int idx; int val; int pad; int type; };
extern "C" V3_6770 func_00306770(int n, Ent_6770 *e) {
    if (D_004A58A4 == 0) {
        D_6770[0] = 0xFFFFFFFF;
        D_6770[1] = 0;
        D_004A58A4 = 1;
        (*(Tag_6770*)((char*)D_6770 + 8)) = Tag_6770((*(Tag_6770*)((char*)D_6770 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_6770(buf, D_6770, 12);
    Ent_6770 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3DC0)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_6770 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_6770 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            r = 0;
        else
            r = (char*)(i << 2);
        obj = r;
    }

    if (obj) {
        int b1 = buf[1];
        func_00101900_6770(*(void**)(*(char**)(*(char**)(D_004A28A8_6770 + 0x84) + 0xC) + 0xA8), obj + 0x78, b1);
    }
    return D_004C9098_6770;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306908);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_6908 __asm__("D_004A3DD8");
extern char **D_004A47B8_6908 __asm__("D_004A47B8");
extern char *D_004A28A8_6908 __asm__("D_004A28A8");
extern "C" void func_00101970_6908(void*, void*) __asm__("func_00101970");
struct V3_6908 { float x, y, z; };
extern V3_6908 D_004C9098_6908 __asm__("D_004C9098");
struct Tag_6908 { char c; Tag_6908() {} Tag_6908(const Tag_6908 &o) : c(o.c) {} };
extern unsigned D_6908 __asm__("D_004A58A8");
extern Tag_6908 D_004A58AC;
extern int D_004A58B0;
extern int D_004A3DC8;
extern "C" void* func_003E6574_6908(void*, void*, int) __asm__("func_003E6574");
struct Ent_6908 { int idx; int val; int pad; int type; };
extern "C" V3_6908 func_00306908(int n, Ent_6908 *e) {
    if (D_004A58B0 == 0) {
        D_6908 = 0xFFFFFFFF;
        D_004A58B0 = 1;
        D_004A58AC = Tag_6908(D_004A58AC);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    func_003E6574_6908(buf, &D_6908, 8);
    Ent_6908 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3DC8)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_6908 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_6908 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            r = 0;
        else
            r = (char*)(i << 2);
        obj = r;
    }
    if (obj)
        func_00101970_6908(*(void**)(*(char**)(*(char**)(D_004A28A8_6908 + 0x84) + 0xC) + 0xA8), obj + 0x78);
    return D_004C9098_6908;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00306A90);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306CB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_004A3DD8_6CB0 __asm__("D_004A3DD8");
extern char **D_004A47B8_6CB0 __asm__("D_004A47B8");
extern char *D_004A28A8_6CB0 __asm__("D_004A28A8");
extern void *D_004A4028_6CB0 __asm__("D_004A4028");
void func_00357AE8(void*, int, int);
void func_00357B10(void*, int, int);
struct V3_6CB0 { float x, y, z; };
extern V3_6CB0 D_004C9098_6CB0 __asm__("D_004C9098");
struct Tag_6CB0 { char c; Tag_6CB0() {} Tag_6CB0(const Tag_6CB0 &o) : c(o.c) {} };
extern unsigned D_6CB0[] __asm__("D_004FC050");
extern int D_004A58B8;
extern const int D_00446970[];
extern "C" void* func_003E6574_6CB0(void*, void*, int) __asm__("func_003E6574");
struct Ent_6CB0 { int idx; int val; int pad; int type; };
extern "C" V3_6CB0 func_00306CB0(int n, Ent_6CB0 *e) {
    if (D_004A58B8 == 0) {
        ((int*)D_6CB0)[0] = 0;
        D_6CB0[1] = 0xFFFFFFFF;
        ((int*)D_6CB0)[2] = 0;
        D_004A58B8 = 1;
        (*(Tag_6CB0*)((char*)D_6CB0 + 12)) = Tag_6CB0((*(Tag_6CB0*)((char*)D_6CB0 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[1] = 0xFFFFFFFF;
    func_003E6574_6CB0(buf, D_6CB0, 16);
    Ent_6CB0 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446970[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[1];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(D_004A3DD8_6CB0 + 0x290);
    } else {
        char *s = (*(char***)((char*)*D_004A47B8_6CB0 + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
            r = 0;
        else
            r = (char*)(i << 2);
        obj = r;
    }
    if (obj) {
        if (buf[2] == 0)
            func_00357AE8(D_004A4028_6CB0, buf[0], *(int*)(obj + 0x78));
        else
            func_00357B10(D_004A4028_6CB0, buf[0], *(int*)(obj + 0x78));
    }
    return D_004C9098_6CB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00306E68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3DD8_6E68 __asm__("D_004A3DD8");
extern char **D_004A47B8_6E68 __asm__("D_004A47B8");
extern "C" int func_0030BFC0_6E68(int, int, int, int) __asm__("func_0030BFC0");
extern "C" void *func_0028B180_6E68() __asm__("func_0028B180");
extern "C" void func_002974A0_6E68(void*, void*, int) __asm__("func_002974A0");
int *func_00226610(void*);
static inline char *refToPtr_6E68(unsigned i) { return (char*)(i << 2); }
static inline char *conv_6E68(unsigned i) { if (i == 0) return 0; return refToPtr_6E68(i); }
struct V3_6E68 { int w0, w1, w2; };
struct Tag_6E68 { char c; Tag_6E68() {} Tag_6E68(const Tag_6E68 &o) : c(o.c) {} };
extern unsigned D_6E68[] __asm__("D_004FC060");
extern int D_004A58BC;
extern const int D_00446980[];
extern "C" void* func_003E6574_6E68(void*, void*, int) __asm__("func_003E6574");
struct Ent_6E68 { int idx; int val; int pad; int type; };
extern "C" V3_6E68* func_00306E68(V3_6E68 *ret, int n, Ent_6E68 *e) {
    if (D_004A58BC == 0) {
        D_6E68[0] = 0xFFFFFFFF;
        D_6E68[1] = 0xFFFFFFFF;
        D_6E68[2] = 0xFFFFFFFF;
        D_004A58BC = 1;
        (*(Tag_6E68*)((char*)D_6E68 + 12)) = Tag_6E68((*(Tag_6E68*)((char*)D_6E68 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    func_003E6574_6E68(buf, D_6E68, 16);
    Ent_6E68 *x = e;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446980[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    int b1 = buf[1];
    int b2 = buf[2];
    int r = func_0030BFC0_6E68(D_004A3DD8_6E68, buf[0], b1, b2);
    if (r == 1) {
        unsigned id = buf[1];
        char *obj;
        char *s = (*(char***)((char*)*D_004A47B8_6E68 + 8))[id & 0xFF];
        if (!s) {
            obj = 0;
        } else {
            unsigned i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8;
            char *t = conv_6E68(i);
            obj = t;
        }
        if (obj)
            func_002974A0_6E68(func_0028B180_6E68(), obj, 0x5C);
    }
    ((int*)ret)[2] = 1;
    *func_00226610(ret) = r;
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00307020);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003E6574(void*, void*, int);
int* func_00226610(void*);
struct Flag_7020 { char c; Flag_7020() {} Flag_7020(const Flag_7020& o) : c(o.c) {} };
struct Ent_7020 { int idx; int val; int pad; int kind; };
struct R12_7020 { int a; int b; int c; };
extern R12_7020 D_004C9098_7020 __asm__("D_004C9098");

extern int D_004A3DD0_7020[2] __asm__("D_004A3DD0");
extern "C" int func_001E3570(int, int);
extern int D_004A58C0_7020 __asm__("D_004A58C0");
struct T12_7020 { int a; int b; Flag_7020 f; };
extern T12_7020 D_004FC070_7020 __asm__("D_004FC070");
extern "C" void* func_00307020(void* self, int n, Ent_7020* e) {
    if (D_004A58C0_7020 == 0) {
        D_004FC070_7020.a = 0;
        D_004A58C0_7020 = 1;
        D_004FC070_7020.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb 0(sp)).
        D_004FC070_7020.f = Flag_7020(D_004FC070_7020.f);
    }
    int v[4] __attribute__((aligned(16)));
    int* vp = v;
    func_003E6574(vp, &D_004FC070_7020, 12);
    Ent_7020* p = e;
    for (int i = 0; i < n; i++, p++) {
        int k = p->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (p->kind != D_004A3DD0_7020[k] && D_004A3DD0_7020[k] == 2)
            *(float*)d = (float)p->val;
        else
            *d = p->val;
    }
    bool r = func_001E3570(vp[0], vp[1]) != 0;
    *(int*)((char*)self + 8) = 1;
    *func_00226610(self) = r;
    return self;
}
#endif

INCLUDE_ASM("seg/seg_1FBE38", func_00307128);

INCLUDE_ASM("seg/seg_1FBE38", func_003071C8);

INCLUDE_ASM("seg/seg_1FBE38", func_003071F0);

INCLUDE_ASM("seg/seg_1FBE38", func_00307240);

INCLUDE_ASM("seg/seg_1FBE38", func_003072B0);

INCLUDE_ASM("seg/seg_1FBE38", func_00307308);

INCLUDE_ASM("seg/seg_1FBE38", func_003074C0);

//100%
INCLUDE_ASM("seg/seg_1FBE38", func_00307568);
#ifdef SKIP_ASM
extern "C" void func_0030AC98(int, void *, int);

extern "C" void func_00307568(void *arg0) {
    int temp_6;

    temp_6 = (*(int *)((char*)(arg0) + (0x74)));
    if (temp_6 != 0) {
        func_0030AC98((*(int *)((char*)(arg0) + (0x10))), arg0, temp_6);
    }
}
#endif
