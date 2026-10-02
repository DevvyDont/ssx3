#include "common.h"

INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_initialize);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E83F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E8560);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E86F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E87E8);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E8938);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA480);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA538);

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

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAA28);

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

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBC40);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBE20);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBF48);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EC060);

