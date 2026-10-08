#include "common.h"

INCLUDE_ASM("seg/seg_1DCD10", func_002DBD10);

INCLUDE_ASM("seg/seg_1DCD10", func_002DBEE0);

INCLUDE_ASM("seg/seg_1DCD10", func_002DBF80);

//100%
INCLUDE_ASM("seg/seg_1DCD10", func_002DBF98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00418EF8_BF98(void* base, int n, int size, int (*cmp)(void*, void*)) __asm__("func_00418EF8");
extern "C" int func_002DC168(void* arg0, void* arg1);
struct RS_BF98 {
    unsigned int w0_lo : 2;
    unsigned int a : 2;
    unsigned int w0_hi : 28;
    unsigned int b : 2;
    unsigned int c : 5;
    unsigned int w1_p7 : 5;
    unsigned int d : 8;
    unsigned int e : 2;
    unsigned int f : 1;
    unsigned int g : 2;
    unsigned int w1_hi : 7;
    unsigned int w2_lo : 5;
    unsigned int h : 5;
    unsigned int i : 19;
    unsigned int w2_hi : 3;
    int w3;
    int w4;
};
struct Ctx_BF98 { char pad[0xE84]; RS_BF98* top; char pad2[0xF60 - 0xE88]; int f60; };
extern Ctx_BF98* D_004A289C;
extern char D_00537AC0[];
struct VEnt_BF98 { short delta; short idx; void (*fn)(void*); };
struct Obj_BF98 { int n; char* vt; };

static inline void Push_BF98() {
    Ctx_BF98* ctx = D_004A289C;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}
static inline void Pop_BF98() {
    D_004A289C->top--;
}
static inline void G3_BF98(int c, int f, int g, int e, int d) {
    Ctx_BF98* ctx = D_004A289C;
    ctx->top->c = c;
    ctx->top->f = f;
    ctx->top->g = g;
    ctx->top->e = e;
    ctx->top->d = d;
}

extern "C" void func_002DBF98(Obj_BF98* self) {
    if (self->n) {
        func_00418EF8_BF98(D_00537AC0, self->n, 8, func_002DC168);
        Push_BF98();
        {
            Ctx_BF98* ctx = D_004A289C;
            int v = ctx->f60;
            ctx->top->h = 7;
            ctx->top->b = 2;
            ctx->top->i = 0x3FF;
            *(short*)((char*)ctx->top + 0x10) = v;
        }
        {
            Ctx_BF98* ctx = D_004A289C;
            ctx->top->a = 3;
            *(short*)((char*)ctx->top + 0x12) = -1;
        }
        G3_BF98(5, 1, 0, 3, 0);
        {
            VEnt_BF98* e = (VEnt_BF98*)(self->vt + 0x18);
            e->fn((char*)self + e->delta);
        }
        Pop_BF98();
        self->n = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1DCD10", func_002DC168);
#ifdef SKIP_ASM
extern "C" int func_002DC168(void *arg0, void *arg1) {
    int temp_3;
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (4)));
    temp_3 = (*(int *)((char*)(arg1) + (4)));
    if (temp_4 >= temp_3) {
        return (temp_3 >= temp_4) ? 0 : -1;
    }
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_1DCD10", func_002DC190);

INCLUDE_ASM("seg/seg_1DCD10", func_002DC7B0);
