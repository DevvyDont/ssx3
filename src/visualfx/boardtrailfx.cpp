#include "common.h"

INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_initialize);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E83F0);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002E8560);
#ifdef SKIP_ASM
struct func_002E8560_sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct func_002E8560_sVtx {
    float x, y, z;
    char pad[0x24];
};

struct func_002E8560_sTrail {
    int f0;
    float f4;
    float f8;
    int fC;
    float* v10;
    float* v14;
    float* v18;
    float* v1C;
    float* v20;
    float* v24;
    char pad28[0x8C - 0x28];
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
    int fA0;
    int fA4;
    char padA8[8];
    func_002E8560_sVec4 colB0;
    func_002E8560_sVec4 colC0;
    func_002E8560_sVec4 colD0;
    int fE0;
};

extern func_002E8560_sVec4 D_004FF130;
extern func_002E8560_sVec4 D_004FF120;
extern float D_004A45B0;
extern float D_004A45B4;
extern float D_004A45B8;

extern "C" void func_002E8560(func_002E8560_sTrail* self)
{
    self->f94 = 2;
    self->f90 = 0;
    self->f8C = 0;
    self->f98 = 0;
    self->f9C = 0;
    self->fA0 = 0;
    self->fA4 = 0;
    self->colB0 = D_004FF130;
    self->colC0 = D_004FF120;
    self->colD0 = D_004FF120;
    self->f4 = 1.0f;
    self->f8 = 1.0f;
    self->fE0 = 0;
    for (int i = 0; i < 54; i++) {
        float t = (float)(i & 1);
        self->v18[i * 12] = D_004A45B8;
        self->v10[i * 12] = D_004A45B4;
        self->v1C[i * 12] = D_004A45B0;
        self->v20[i * 12] = 1.0f - D_004A45B0;
        self->v14[i * 12] = 1.0f - D_004A45B4;
        self->v24[i * 12] = 1.0f - D_004A45B8;
        self->v18[i * 12 + 1] = self->v10[i * 12 + 1] = self->v1C[i * 12 + 1] = self->v20[i * 12 + 1] = self->v14[i * 12 + 1] = self->v24[i * 12 + 1] = t;
        self->v18[i * 12 + 2] = self->v10[i * 12 + 2] = self->v1C[i * 12 + 2] = self->v20[i * 12 + 2] = self->v14[i * 12 + 2] = self->v24[i * 12 + 2] = 1.0f;
    }
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002E86F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E87E8);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E8938);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA480);
#ifdef SKIP_ASM
struct sMtxEntry_A480 {
    int v[5];
};
extern int D_004A4518;
extern int D_004A451C;
struct sRenderCtx_A480 {
    char pad[0xE84];
    sMtxEntry_A480* top;
};
extern sRenderCtx_A480* D_004A289C;

class func_002EA480_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};

static inline void push_A480(sRenderCtx_A480* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void pop_A480(sRenderCtx_A480* ctx)
{
    ctx->top--;
}

extern "C" void func_002EA480(void* self)
{
    if (D_004A4518 != 0) {
        return;
    }
    if (D_004A451C != 0) {
        func_002EA480_cObj* o = (func_002EA480_cObj*)(*(char**)self + 0x6C0);
        if (o->v09() != 0) {
            return;
        }
    }
    if (*(int*)((char*)self + 0x90) < 2) {
        return;
    }
    push_A480(D_004A289C);
    pop_A480(D_004A289C);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA538);
#ifdef SKIP_ASM
extern int D_004A45C8;

struct sTrailStripA538 {
    char* pts;                  // 0x0
    int start;                  // 0x4
    int count;                  // 0x8
    int fC;                     // 0xC
    float scale;                // 0x10
};
struct sRctxVtA538 { short delta; short index; void (*fn)(void*, sTrailStripA538*, int); };

extern "C" void func_002EA538(char** trails, int n)
{
    sTrailStripA538 strips[16];
    int count = 0;
    push_A480(D_004A289C);
    if (n > 0) {
    do {
        char* t = *trails;
        int len = *(int*)(t + 0x90);
        if (len < 2) continue;
        int k = len - 0x39;
        int start = *(int*)(t + 0x8C) - k;
        int cnt = len - 2;
        int over = len - 0x32;
        if (over > 0) {
            cnt -= over;
            start += over;
        }
        if (cnt < 2) continue;
        int st = start % 0x36;
        float sc = 1.0f / (float)D_004A45C8;
        if (count < 16) {
            strips[count].pts = t + 0x10;
            strips[count].start = st;
            strips[count].count = cnt;
            strips[count].fC = 0;
            strips[count].scale = sc;
            count++;
        }
    } while (trails++, --n > 0);
    }
    {
        char* ctx = (char*)D_004A289C;
        sRctxVtA538* vt = *(sRctxVtA538**)(ctx + 0x10D8);
        vt[89].fn(ctx + vt[89].delta, strips, count);
    }
    pop_A480(D_004A289C);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA670);
#ifdef SKIP_ASM
extern void* D_00487F30[];
extern "C" void* func_00354648(void* self);
extern "C" void func_002EAA28(void* self);

extern "C" void* func_002EA670(void* self)
{
    func_00354648(self);
    *(void***)((char*)self + 0xC) = D_00487F30;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    func_002EAA28(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA6B8);
#ifdef SKIP_ASM
extern "C" void func_002EAA28(void* self);

struct func_002EA6B8_sFade {
    char pad[0x10];
    int state;
    float t;
    float dur;
};

extern "C" void func_002EA6B8(func_002EA6B8_sFade* self, float dur)
{
    switch (self->state) {
    case 0:
        func_002EAA28(self);
        self->dur = dur;
        self->t = 0.0f;
        break;
    case 1:
        return;
    case 2: {
        float r = self->t / self->dur;
        self->dur = dur;
        self->t = (1.0f - r) * dur;
        break;
    }
    case 3: {
        float r = self->t / self->dur;
        self->dur = dur;
        self->t = r * dur;
        break;
    }
    }
    self->state = 3;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA780);
#ifdef SKIP_ASM
struct sTrailFade {
    char pad[0x10];
    int state;
    float value;
    float time;
};

extern "C" void func_002EA780(sTrailFade* self, float t)
{
    switch (self->state) {
    case 0:
        return;
    case 1:
        self->time = t;
        self->value = 0.0f;
        break;
    case 2:
        self->value = (self->value / self->time) * t;
        self->time = t;
        break;
    case 3:
        self->value = (1.0f - self->value / self->time) * t;
        self->time = t;
        break;
    }
    self->state = 2;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA820);
#ifdef SKIP_ASM
struct sBTVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern char* D_004A5B80;

extern "C" void func_002EA820(void* self)
{
    if (*(int*)((char*)self + 0x10) != 0) {
        *(int*)((char*)self + 0x10) = 0;
        char* obj = D_004A5B80;
        sBTVEntry* e = &(*(sBTVEntry**)(obj + 0x10D8))[41];
        e->fn(obj + e->delta, (char*)self + 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA860);
#ifdef SKIP_ASM
extern char* D_004A5B80;

extern "C" void func_002EA860(void* self)
{
    if (*(int*)((char*)self + 0x10) == 0) {
        func_002EAA28(self);
    }
    char* obj = D_004A5B80;
    *(int*)((char*)self + 0x10) = 1;
    sBTVEntry* e = &(*(sBTVEntry**)(obj + 0x10D8))[41];
    e->fn(obj + e->delta, (char*)self + 0x34);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA8B8);
#ifdef SKIP_ASM
extern "C" void func_002EA8B8(void* self, int up)
{
    int s = *(int*)((char*)self + 0x10);
    if (s == 2 || s == 3) {
        *(int*)((char*)self + 0x1C) = 0;
        return;
    }
    if (up) {
        *(int*)((char*)self + 0x1C) += 1;
        return;
    }
    if (*(int*)((char*)self + 0x1C) > 0) {
        *(int*)((char*)self + 0x1C) -= 1;
    }
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA900);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EAA28);
#ifdef SKIP_ASM
struct sBTState {
    int mode;
    float a;
    float b;
    float c;
    float d;
};

struct sBTFx {
    char pad[0x20];
    sBTState saved;
    sBTState cur;
};

struct sBTVEntryGet {
    short delta;
    short index;
    sBTState* (*fn)(void*);
};

extern char* D_004A5B80;

extern "C" void func_002EAA28(void* self)
{
    sBTFx* s = (sBTFx*)self;
    char* obj = D_004A5B80;
    sBTVEntryGet* e = &(*(sBTVEntryGet**)(obj + 0x10D8))[42];
    s->saved = *e->fn(obj + e->delta);
    float c = 0.75f;
    float d = 0.6299999952316284f;
    if (s->saved.mode == 2) {
        s->cur.d = d;
        s->cur.mode = 2;
        s->cur.b = 1.0f;
        s->cur.c = c;
        s->cur.a = 0.0f;
    } else {
        s->cur.mode = 1;
        s->cur.a = 0.125f;
        s->cur.b = c;
        s->cur.c = c;
        s->cur.d = d;
    }
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAAE0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAC60);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EADC0__FPv);
#ifdef SKIP_ASM
void func_002EADC0(void* self)
{
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EADD0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB198);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EB8F0);
#ifdef SKIP_ASM
struct sBTVEntryR {
    short delta;
    short index;
    float* (*fn)(void*);
};

extern char* D_004A5B80;
extern float D_004A3B3C;
extern float D_004A3B40;
extern float D_004A3B44;

extern "C" void func_002EB8F0(void)
{
    char* obj = D_004A5B80;
    sBTVEntryR* e = &(*(sBTVEntryR**)(obj + 0x10D8))[42];
    float* r = e->fn(obj + e->delta);
    float a = r[1];
    float w = r[2];
    D_004A3B3C = a;
    float b = w + a;
    D_004A3B40 = b;
    D_004A3B44 = b - a;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB938);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardtrailfx", func_002EBB10);
#ifdef SKIP_ASM
struct sBTRect4 {
    float x, y, z, w;
};

extern float D_004A3B30;
extern float D_004A3B38;
extern float D_004A3B3C;
extern float D_004A3B44;

extern "C" void func_002EB938(sBTRect4* pos, sBTRect4* rect, sBTRect4* color, float f);

extern "C" void func_002EBB10(void* self, float s)
{
    sBTRect4 color = *(sBTRect4*)((char*)self + 4);
    color.x = ((sBTRect4*)((char*)self + 4))->x * s;
    sBTRect4 pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 640.0f;
    pos.w = 480.0f;
    sBTRect4 rect;
    rect.x = D_004A3B30;
    rect.y = D_004A3B3C;
    rect.z = D_004A3B38;
    rect.w = D_004A3B44;
    func_002EB938(&pos, &rect, &color, pos.x);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EBBA8);
#ifdef SKIP_ASM
void func_002E4D70(void* self);
extern void* D_004880A0[];

struct func_002EBBA8_sVec4 {
    float x, y, z, w;
};

struct func_002EBBA8_sObj {
    void** vtable;               // 0x0
    func_002EBBA8_sVec4 v;       // 0x4
    int a;                       // 0x14
    int b;                       // 0x18
};

extern "C" func_002EBBA8_sObj* func_002EBBA8(func_002EBBA8_sObj* self, const func_002EBBA8_sVec4& v, int a, int b)
{
    func_002EBBA8_sVec4 t = v;
    func_002E4D70(self);
    self->vtable = D_004880A0;
    self->v = t;
    self->a = a;
    self->b = b;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardtrailfx", func_002EBC40);
#ifdef SKIP_ASM
extern float D_004A3B30;
extern float D_004A3B34;
extern float D_004A3B38;
extern float D_004A3B3C;
extern float D_004A3B40;
extern float D_004A3B44;
extern "C" void func_002EB938(sBTRect4* pos, sBTRect4* rect, sBTRect4* color, float f);

extern "C" void func_002EBC40(char* self, float t)
{
    float k = 1.0f - t;
    float dx = (float)*(int*)(self + 0x14) * k * 640.0f;
    float dy = (float)*(int*)(self + 0x18) * k * -480.0f;
    float w = 640.0f;
    float u0, u1;
    if (dx < 0.0f) {
        w = dx + w;
        u0 = D_004A3B30 + D_004A3B38 * 0.0015625000232830644f * dx;
        u1 = D_004A3B34;
        dx = 0.0f;
    } else {
        w = w - dx;
        u0 = D_004A3B30;
        u1 = u0 + D_004A3B38 * 0.0015625000232830644f * w;
    }
    float h;
    float v0, v1;
    if (dy < 0.0f) {
        h = dy + 480.0f;
        v0 = D_004A3B3C + D_004A3B44 * 0.0020833334419876337f * dy;
        v1 = D_004A3B40;
        dy = 0.0f;
    } else {
        h = 480.0f - dy;
        v0 = D_004A3B3C;
        v1 = v0 + D_004A3B44 * 0.0020833334419876337f * h;
    }
    float uw = u1 - u0;
    float vh = v1 - v0;
    sBTRect4 pos;
    pos.x = dx;
    pos.y = dy;
    pos.z = w;
    pos.w = h;
    sBTRect4 rect;
    rect.x = u0;
    rect.y = v0;
    rect.z = uw;
    rect.w = vh;
    func_002EB938(&pos, &rect, (sBTRect4*)(self + 4), 0.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EBE20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
unsigned int BXrand();
// base ctor returns this; the unit declares it void
void* func_002E4D70_r(void* self) __asm__("func_002E4D70__FPv");
extern void* D_00488080[];

struct sBtRectBE20 { float x, y, w, h; };

extern "C" void* func_002EBE20(void* self, sBtRectBE20 rect)
{
    func_002E4D70_r(self);
    *(void***)self = D_00488080;
    *(sBtRectBE20*)((char*)self + 0x4) = rect;
    unsigned int r1 = BXrand() % 1000;
    float* p = (float*)((char*)self + 0x14);
    *p = (float)r1 * 0.64000004529953f;
    unsigned int r2 = BXrand() % 1000;
    float* q = (float*)((char*)self + 0x18);
    *q = (float)r2 * 0.48000001907348633f;
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBF48);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EC060);

