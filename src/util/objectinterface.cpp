#include "common.h"

struct cObjectInterface {
    char pad_0x00[0x84];
    struct cObjectInterface_Impl* mImpl; // offset 0x84
};

struct cObjectInterface_Impl {
    char pad_0x00[0x88];
    void* mInstanceMan; // offset 0x88
};

extern cObjectInterface* D_004A28A8;

//100%
INCLUDE_ASM("util/objectinterface", cObjectInterface_getInstanceMan__Fv);
#ifdef SKIP_ASM
void* cObjectInterface_getInstanceMan()
{
    return D_004A28A8->mImpl->mInstanceMan;
}
#endif

extern "C" void* func_002D9C00(void* self);

//100%
INCLUDE_ASM("util/objectinterface", func_002D1CF0__FPv);
#ifdef SKIP_ASM
void* func_002D1CF0(void* self)
{
    return func_002D9C00(self);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D1D10);
#ifdef SKIP_ASM
struct sRState_1D10 {
    int f0;                     // 0x00
    int flagsA;                 // 0x04
    int flagsB;                 // 0x08
    int fC;                     // 0x0C
    short f10;                  // 0x10
    short f12;                  // 0x12
};

static inline void setA_1D10(sRState_1D10* r, int v, int mask, int shift)
{
    r->flagsA = (r->flagsA & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void setB_1D10(sRState_1D10* r, int v, int mask, int shift)
{
    r->flagsB = (r->flagsB & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

// 2D vector (8 bytes, BLKmode: its temporaries share 16-byte frame slots that combine when freed).
// The ctor takes const float&: a by-value float ctor folds the 1.0f into the inlined
// stores, which then fail to validate and leave the uv1 temp addressed through a register.
struct sVec2_1D10 {
    float x, y;
    sVec2_1D10(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sCol_1D10 {
    float a, r, g, b;
};

struct sVE_1D10 {
    short delta;
    short index;
    void* fn;
};

typedef int (*IsHiddenFn_1D10)(void*, void*);
typedef void (*DrawFn_1D10)(void*, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*);
typedef void (*DrawRotFn_1D10)(void*, float, void*, const sVec2_1D10&, const sVec2_1D10&, const sVec2_1D10&, const sCol_1D10*, int);

struct sRCtx_1D10 {
    char pad[0xE84];
    sRState_1D10* top;          // 0xE84
    char padE88[0xF50 - 0xE88];
    int texs[0x40];             // 0xF50
    char pad1050[0x10D8 - 0x1050];
    sVE_1D10* vt;               // 0x10D8
};

static inline int tex_1D10(sRCtx_1D10* c, int i)
{
    return c->texs[i];
}

extern sRCtx_1D10* D_004A289C;
extern sRCtx_1D10* D_004A5B80;
extern sRState_1D10 D_00501420;

extern "C" void func_002D1D10(void* obj, float* src, int kind, float size, float angle)
{
    sRCtx_1D10* g = D_004A289C;
    if (((IsHiddenFn_1D10)g->vt[91].fn)((char*)g + g->vt[91].delta, obj) != 0) {
        return;
    }
    sCol_1D10 col;
    col.a = src[3];
    col.r = src[0];
    col.g = src[1];
    col.b = src[2];
    sRCtx_1D10* ctx = D_004A5B80;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = D_00501420;
    setA_1D10(ctx->top, 1, 1, 22);
    setA_1D10(ctx->top, 0, 3, 0);
    setB_1D10(ctx->top, 0, 0x7FFFF, 10);
    setA_1D10(ctx->top, 7, 0x1F, 2);
    setB_1D10(ctx->top, 8, 0x1F, 5);
    setA_1D10(ctx->top, 1, 3, 23);
    setA_1D10(ctx->top, 0, 3, 20);
    setA_1D10(ctx->top, 0x14, 0xFF, 12);
    ctx->top->f10 = tex_1D10(ctx, kind + 0x25);
    if (angle != 0.0f) {
        ((DrawRotFn_1D10)ctx->vt[79].fn)((char*)ctx + ctx->vt[79].delta, angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        if (kind == 1 || kind == 4) {
            ((DrawRotFn_1D10)ctx->vt[79].fn)((char*)ctx + ctx->vt[79].delta, -angle * 0.01745329424738884f, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col, 1);
        }
    } else {
        ((DrawFn_1D10)ctx->vt[78].fn)((char*)ctx + ctx->vt[78].delta, obj, sVec2_1D10(size, size), sVec2_1D10(0.0f, 0.0f), sVec2_1D10(1.0f, 1.0f), &col);
    }
    ctx->top--;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2058);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x828)
extern char D_828[];

struct sQ128 { float v[4]; } __attribute__((aligned(16)));

// PORT: PS2-only VU0 microprogram call (ctc2/vcallmsr/cfc2); the PC port needs a C version.
extern "C" int func_002D2058(sQ128* v)
{
    char* inst = (char*)*(void**)((char*)D_004A28A8->mImpl + 0x8C);
    int mask = *(int*)(inst + 0x110);
    for (int i = 0; i < *(int*)(inst + 0x10C); i++) {
        int bit = 1 << i;
        if (mask & bit) {
            sQ128* m = (sQ128*)(*(char**)(inst + 8 + i * 8) + 0x50);
            int r;
            __asm__ __volatile__(
                ".set push\n"
                ".set noreorder\n"
                "lqc2      $vf1, %1\n"
                "lqc2      $vf2, %2\n"
                "lqc2      $vf3, %3\n"
                "lqc2      $vf4, %4\n"
                "lqc2      $vf5, %5\n"
                "lqc2      $vf6, %6\n"
                "ctc2.ni   %7, $vi27\n"
                "vnop\n"
                "vnop\n"
                "vcallmsr  $vi27\n"
                "cfc2.i    %0, $vi1\n"
                ".set pop\n"
                : "=&r"(r)
                : "m"(m[0]), "m"(m[1]), "m"(m[2]), "m"(m[3]), "m"(m[4]), "m"(*v), "r"((int)D_828 >> 3)
                : "memory");
            int ok = r == 1;
            if (ok) return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D20F8);
#ifdef SKIP_ASM
extern void* D_004A3A00;

extern "C" int func_002D20F8(void) {
    return (*(int*)&D_004A3A00);
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2100);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00486758[];
extern "C" void* func_002D21C0(void* p);

extern "C" void* func_002D2100(void)
{
    return func_002D21C0(cMemMan_alloc(1, D_00486758, 0, 0));
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D2138);
#ifdef SKIP_ASM
extern "C" void func_002D2138(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2140);
#ifdef SKIP_ASM
extern "C" void func_002D2140(void) {

}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2148);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long D_004FF120;

extern "C" long long *func_002D2148(long long *arg0) {
    *arg0 = D_004FF120;
    return arg0;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2160);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long D_004FF120;

extern "C" long long *func_002D2160(long long *arg0) {
    *arg0 = D_004FF120;
    return arg0;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2178);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern long long D_004FF120;

extern "C" long long *func_002D2178(long long *arg0) {
    *arg0 = D_004FF120;
    return arg0;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2190);
#ifdef SKIP_ASM
extern "C" float func_002D2190(void) {
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D21A0);
#ifdef SKIP_ASM
extern "C" float func_002D21A0(void) {
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D21B0__FPv);
#ifdef SKIP_ASM
void func_002D21B0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D21B8__FPv);
#ifdef SKIP_ASM
void func_002D21B8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/objectinterface", func_002D21C0);
#ifdef SKIP_ASM
extern void* D_004A3A00;

extern "C" void* func_002D21C0(void* p)
{
    D_004A3A00 = p;
    return p;
}
#endif


//100%
INCLUDE_ASM("util/objectinterface", func_002D21D0);
#ifdef SKIP_ASM
// PORT: later callers in the unit declare func_002D21D0 as void; bound by asm label with its real int return.
int func_002D21D0_impl(signed char **stream, float v) __asm__("func_002D21D0");

union uFloatBits_002D21D0 {
    float f;
    unsigned int u;
};

int func_002D21D0_impl(signed char **stream, float v)
{
    uFloatBits_002D21D0 b;
    b.f = v;
    unsigned int bits = b.u;
    signed char *p = *stream;
    p[0] = bits;
    p[1] = bits >> 8;
    p[2] = bits >> 16;
    p[3] = bits >> 24;
    *stream += 4;
    return 4;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D2210);
#ifdef SKIP_ASM
extern "C" int func_002D2210(signed char **stream, unsigned int v)
{
    signed char *p = *stream;
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
    *stream += 4;
    return 4;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2248);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2248(void* self, float* v)
{
    int i;
    for (i = 0; i < 3; i++) {
        func_002D21D0(self, v[i]);
    }
    return 0xC;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D22A8);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D22A8(void* self, float* v)
{
    int i;
    for (i = 0; i < 4; i++) {
        func_002D21D0(self, v[i]);
    }
    return 0x10;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2308);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2308(void* self, float* v)
{
    int i;
    for (i = 0; i < 16; i++) {
        func_002D21D0(self, v[i]);
    }
    return 0x40;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2368);
#ifdef SKIP_ASM
extern "C" void func_002D21D0(void* self, float v);

extern "C" int func_002D2368(void* self, float* q)
{
    func_002D21D0(self, q[3]);
    func_002D21D0(self, q[0]);
    func_002D21D0(self, q[1]);
    func_002D21D0(self, q[2]);
    return 0x10;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D23C8);
#ifdef SKIP_ASM
extern "C" int func_002D23C8(signed char **arg0, signed char arg1) {
    **arg0 = arg1;
    *arg0 += 1;
    return 1;
}
#endif
//100%
INCLUDE_ASM("util/objectinterface", func_002D23E8);
#ifdef SKIP_ASM
extern "C" int func_002D23C8(signed char **arg0, signed char arg1);
extern "C" unsigned int strlen(const char* s);

extern "C" int func_002D23E8(signed char** out, const char* s)
{
    unsigned int i;
    int n = 0;
    for (i = 0; i < strlen(s); i++) {
        n += func_002D23C8(out, s[i]);
    }
    func_002D23C8(out, 0);
    return n + 1;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/objectinterface", func_002D2478);
#ifdef SKIP_ASM
extern "C" int func_002D2210(signed char** out, unsigned int len);

struct sStr_2478 {
    unsigned int len;
    signed char* data;
};

extern "C" int func_002D2478(signed char** out, sStr_2478** pp)
{
    unsigned int i;
    int n = func_002D2210(out, (*pp)->len);
    for (i = 0; i < (*pp)->len; i++) {
        n += func_002D23C8(out, (*pp)->data[i]);
    }
    return n;
}
#endif
