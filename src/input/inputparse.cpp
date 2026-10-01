#include "common.h"

INCLUDE_ASM("input/inputparse", cInputMapParser_lookupModifierName);

INCLUDE_ASM("input/inputparse", cInputMapParser_lookupConfigName);

INCLUDE_ASM("input/inputparse", func_00321A40);

//100%
INCLUDE_ASM("input/inputparse", func_00321AD0);
#ifdef SKIP_ASM
extern "C" void* func_00321AD0(void* self, void* a1)
{
    return (char*)a1 + 0x40;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00321AD8);
#ifdef SKIP_ASM
extern "C" int func_00321AD8(void* self, int a1, int a2)
{
    return a1 * 6 + a2 + 0x60;
}
#endif

INCLUDE_ASM("input/inputparse", cInputMapParser_readToken);

INCLUDE_ASM("input/inputparse", func_00323098);

INCLUDE_ASM("input/inputparse", cInputMapParser_parseStatement);

INCLUDE_ASM("input/inputparse", cInputMapParser_parseExpression);

INCLUDE_ASM("input/inputparse", func_003233B8);

INCLUDE_ASM("input/inputparse", func_00323540);

INCLUDE_ASM("input/inputparse", func_003236D8);

INCLUDE_ASM("input/inputparse", func_00323900);

INCLUDE_ASM("input/inputparse", func_00323A68);

INCLUDE_ASM("input/inputparse", cInputMapParser_parseTerm);

INCLUDE_ASM("input/inputparse", cInputMapParser_parseBinaryFunc);

INCLUDE_ASM("input/inputparse", cInputMapParser_parseUnaryFunc);

INCLUDE_ASM("input/inputparse", cInputMapParser_compileStatement);

INCLUDE_ASM("input/inputparse", func_00324678);

INCLUDE_ASM("input/inputparse", func_00324EC8);

INCLUDE_ASM("input/inputparse", func_003250D8);

//100%
INCLUDE_ASM("input/inputparse", func_00325250__FPv);
#ifdef SKIP_ASM
void* func_00325250(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

INCLUDE_ASM("input/inputparse", func_00325260);

//100%
INCLUDE_ASM("input/inputparse", func_003252E8);
#ifdef SKIP_ASM
struct sInputParse_003252E8 {
    int pad[2];
    int a[1];
};

extern "C" void* func_003252E8(void* self, int i)
{
    return &((sInputParse_003252E8*)self)->a[i];
}
#endif

INCLUDE_ASM("input/inputparse", func_003252F8);

//100%
INCLUDE_ASM("input/inputparse", func_00325430);
#ifdef SKIP_ASM
extern "C" float func_00325430(void* self, int a1)
{
    if (a1 == 0) {
        return 0.0f;
    }
    char* p = (char*)self + a1 * 4;
    return *(float*)(p + 0x108);
}
#endif

INCLUDE_ASM("input/inputparse", func_00325450);

INCLUDE_ASM("input/inputparse", func_00325F48);

INCLUDE_ASM("input/inputparse", func_00326078);

INCLUDE_ASM("input/inputparse", func_00326150);

INCLUDE_ASM("input/inputparse", func_00326308);

INCLUDE_ASM("input/inputparse", func_00326360);

INCLUDE_ASM("input/inputparse", func_00326430);

//100%
INCLUDE_ASM("input/inputparse", func_00326478);
#ifdef SKIP_ASM
struct sParseStream {
    unsigned char* base;
    int size;
    unsigned char* cur;
};

extern "C" void func_00326478(void* self, sParseStream* s)
{
    if (s->cur == s->base + s->size) {
        *(int*)((char*)self + 0xc) = -1;
    } else {
        *(int*)((char*)self + 0xc) = *s->cur++;
    }
}
#endif

INCLUDE_ASM("input/inputparse", func_003264B0);

INCLUDE_ASM("input/inputparse", func_003265C0);

INCLUDE_ASM("input/inputparse", func_00326678);

INCLUDE_ASM("input/inputparse", cInputPreProcessor_readCmdLine);

//100%
INCLUDE_ASM("input/inputparse", func_00326A20__FPv);
#ifdef SKIP_ASM
void func_00326A20(void* self)
{
}
#endif

INCLUDE_ASM("input/inputparse", func_00326A28);

INCLUDE_ASM("input/inputparse", func_00326A68);

INCLUDE_ASM("input/inputparse", func_00326AE0);

//100%
INCLUDE_ASM("input/inputparse", func_00326B48);
#ifdef SKIP_ASM
struct sInputSlot_00326B48 {
    int value;
    char data[0x60];
};

struct sInputParse_00326B48 {
    sInputSlot_00326B48 slots[30][4];
    unsigned int head;
    unsigned int tail;
};

extern "C" int func_00326B48(sInputParse_00326B48* s)
{
    unsigned int next = (s->head + 1) % 30;
    if (next == s->tail) {
        return 0;
    }
    s->head = next;
    return 1;
}
#endif

INCLUDE_ASM("input/inputparse", func_00326B88);

//100%
INCLUDE_ASM("input/inputparse", func_00326C60);
#ifdef SKIP_ASM
struct sInputSlot_00326C60 {
    int value;
    char data[0x60];
};

struct sInputParse_00326C60 {
    sInputSlot_00326C60 slots[30][4];
    int head;
    int tail;
};

extern "C" int func_00326C60(sInputParse_00326C60* s)
{
    int tail = s->tail;
    int head = s->head;
    s->head = (tail + 29) % 30;
    return (tail - head + 30) % 30;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326CA0);
#ifdef SKIP_ASM
struct sInputSlot_00326CA0 {
    int value;
    char pad[0x60];
};

struct sInputParse_00326CA0 {
    sInputSlot_00326CA0 slots[30][4];
    int bank;
};

extern "C" int func_00326CA0(void* self, int i)
{
    sInputParse_00326CA0* s = (sInputParse_00326CA0*)self;
    return s->slots[s->bank][i].value;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326CC8);
#ifdef SKIP_ASM
struct sInputSlot_00326CC8 {
    int value;
    char data[0x60];
};

struct sInputParse_00326CC8 {
    sInputSlot_00326CC8 slots[30][4];
    int bank;
};

extern "C" void* func_00326CC8(void* self, int i)
{
    sInputParse_00326CC8* s = (sInputParse_00326CC8*)self;
    return s->slots[s->bank][i].data;
}
#endif

INCLUDE_ASM("input/inputparse", func_00326CF0);

INCLUDE_ASM("input/inputparse", func_00326D60);

INCLUDE_ASM("input/inputparse", func_00326DF0);

INCLUDE_ASM("input/inputparse", func_00326E50);

INCLUDE_ASM("input/inputparse", func_00326EB0);

//100%
INCLUDE_ASM("input/inputparse", func_00327738__FPv);
#ifdef SKIP_ASM
int func_00327738(void* self)
{
    return 0x2;
}
#endif

INCLUDE_ASM("input/inputparse", func_00327740);

INCLUDE_ASM("input/inputparse", func_003277D0);

//100%
INCLUDE_ASM("input/inputparse", func_00327800__FPv);
#ifdef SKIP_ASM
int func_00327800(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327808__FPv);
#ifdef SKIP_ASM
void func_00327808(void* self)
{
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327810__FPv);
#ifdef SKIP_ASM
void* func_00327810(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x8) = t0;
    *(int*)((char*)self + 0xc) = t0;
    return self;
}
#endif

INCLUDE_ASM("input/inputparse", func_00327828);

