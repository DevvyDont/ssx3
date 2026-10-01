#include "common.h"

INCLUDE_ASM("object/debouncenode", cDebounceNode_cDebounceNode);

INCLUDE_ASM("object/debouncenode", func_00342D88);

INCLUDE_ASM("object/debouncenode", func_00342DD8);

extern "C" void* func_00356B08(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00342E78__FPv);
#ifdef SKIP_ASM
void* func_00342E78(void* self)
{
    return func_00356B08(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00342E98);

INCLUDE_ASM("object/debouncenode", func_00342FA8);

INCLUDE_ASM("object/debouncenode", func_00343010);

INCLUDE_ASM("object/debouncenode", func_003430D0);

INCLUDE_ASM("object/debouncenode", func_00343130);

//100%
INCLUDE_ASM("object/debouncenode", func_003434E8);
#ifdef SKIP_ASM
struct sDebounceScroll {
    char pad_0x0[0x38];
    float du; // 0x38
    float dv; // 0x3c
    float u;  // 0x40
    float v;  // 0x44
};

extern "C" void func_003434E8(sDebounceScroll* self)
{
    self->u += self->du;
    self->v += self->dv;
    if (self->u > 1.0f) {
        self->u -= 1.0f;
    } else if (self->u < -1.0f) {
        self->u += 1.0f;
    }
    if (self->v > 1.0f) {
        self->v -= 1.0f;
    } else if (self->v < -1.0f) {
        self->v += 1.0f;
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343588);

INCLUDE_ASM("object/debouncenode", func_00343718);

INCLUDE_ASM("object/debouncenode", func_00343768);

INCLUDE_ASM("object/debouncenode", func_003437C0);

INCLUDE_ASM("object/debouncenode", func_00343820);

INCLUDE_ASM("object/debouncenode", func_00343868);

INCLUDE_ASM("object/debouncenode", func_00343A18);

INCLUDE_ASM("object/debouncenode", func_00343A68);

INCLUDE_ASM("object/debouncenode", func_00343B28);

INCLUDE_ASM("object/debouncenode", func_00343BC0);

INCLUDE_ASM("object/debouncenode", func_00343C08);

INCLUDE_ASM("object/debouncenode", func_00343C60);

INCLUDE_ASM("object/debouncenode", func_00343F38);

INCLUDE_ASM("object/debouncenode", func_003440C8);

INCLUDE_ASM("object/debouncenode", func_00344138);

INCLUDE_ASM("object/debouncenode", func_003441A8);

INCLUDE_ASM("object/debouncenode", func_00344240);

//100%
INCLUDE_ASM("object/debouncenode", func_003442E0);
#ifdef SKIP_ASM
struct sDebounceSlot {
    unsigned int id; // 0x0
    int value;       // 0x4
};

struct sDebounceState {
    int field_0x0;            // 0x000
    unsigned int field_0x4;   // 0x004
    unsigned int field_0x8;   // 0x008
    sDebounceSlot slots[32];  // 0x00c
    int field_0x10c;          // 0x10c
    int field_0x110;          // 0x110
    int field_0x114;          // 0x114
    char pad_0x118[0xc4];     // 0x118
    int field_0x1dc;          // 0x1dc
    int field_0x1e0;          // 0x1e0
    int field_0x1e4;          // 0x1e4
    int field_0x1e8;          // 0x1e8
    int field_0x1ec;          // 0x1ec
    int field_0x1f0;          // 0x1f0
};

extern "C" void func_003442E0(sDebounceState* self)
{
    int i;
    self->field_0x0 = 0;
    self->field_0x1dc = 0;
    self->field_0x8 = 0xFFFFFFFF;
    self->field_0x4 = 0xFFFFFFFF;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f0 = 0;
    self->field_0x114 = 0;
    self->field_0x110 = 0;
    for (i = 0; i < 32; i++) {
        self->slots[i].id = 0xFFFFFFFF;
        self->slots[i].value = 0;
    }
}
#endif

extern "C" void* func_00344800(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00344348__FPv);
#ifdef SKIP_ASM
void* func_00344348(void* self)
{
    return func_00344800(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344368);

INCLUDE_ASM("object/debouncenode", func_003443A8);

INCLUDE_ASM("object/debouncenode", func_00344730);

INCLUDE_ASM("object/debouncenode", func_00344800);

INCLUDE_ASM("object/debouncenode", func_00344898);

INCLUDE_ASM("object/debouncenode", func_003449F0);

INCLUDE_ASM("object/debouncenode", func_00344AA0);

INCLUDE_ASM("object/debouncenode", func_00344E18);

INCLUDE_ASM("object/debouncenode", func_00344FC0);

INCLUDE_ASM("object/debouncenode", func_00344FF8);

INCLUDE_ASM("object/debouncenode", func_00345048);

INCLUDE_ASM("object/debouncenode", func_003451C0);

INCLUDE_ASM("object/debouncenode", func_00345248);

INCLUDE_ASM("object/debouncenode", func_00345430);

