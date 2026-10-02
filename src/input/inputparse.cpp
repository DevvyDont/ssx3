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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", func_00323098);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(void* self);
extern "C" void cInputMapParser_parseStatement(void* self);

extern "C" void func_00323098(void* self)
{
    cInputMapParser_readToken(self);
    while (*(int*)((char*)self + 0x98) != 0) {
        cInputMapParser_parseStatement(self);
    }
}
#endif

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

//100%
INCLUDE_ASM("input/inputparse", func_00326308);
#ifdef SKIP_ASM
extern "C" void func_00326360(void* self, char c);

extern "C" void func_00326308(void* self, char* s)
{
    while (*s != 0) {
        func_00326360(self, *s++);
    }
}
#endif

INCLUDE_ASM("input/inputparse", func_00326360);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", func_00326430);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_00326308(void* self, char* msg);
extern char D_0048E3C8[];

extern "C" void func_00326430(void* self, int a1, int a2)
{
    char buf[0xB0];
    sprintf(buf, D_0048E3C8, a1, a2);
    func_00326308(self, buf);
}
#endif

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

//100%
INCLUDE_ASM("input/inputparse", func_00326678);
#ifdef SKIP_ASM
extern "C" void func_00326678(void* self, sParseStream* s)
{
    while (*(int*)((char*)self + 0xC) >= 0) {
        func_00326478(self, s);
        if (*(int*)((char*)self + 0xC) == 10) {
            func_00326478(self, s);
            *(int*)((char*)s + 0xC) += 1;
            break;
        }
    }
}
#endif

INCLUDE_ASM("input/inputparse", cInputPreProcessor_readCmdLine);

//100%
INCLUDE_ASM("input/inputparse", func_00326A20__FPv);
#ifdef SKIP_ASM
void func_00326A20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326A28);
#ifdef SKIP_ASM
extern "C" int func_00326D60(void* p, int n);

extern "C" void func_00326A28(void* self)
{
    *(int*)((char*)self + 0x2EE8) = func_00326D60((char*)self + 0x2EEC, 4);
    *(int*)((char*)self + 0x2EE0) = 0x1D;
    *(int*)((char*)self + 0x2EE4) = 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326A68);
#ifdef SKIP_ASM
struct sVEntry00326A68 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00326A68 {
    sVEntry00326A68* vt;
};

struct sOwner00326A68 {
    char pad[0x2EE8];
    int count;
    sObj00326A68* objs[1];
};

extern "C" void func_00326A68(sOwner00326A68* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        sObj00326A68* o = self->objs[i];
        if (o != 0) {
            o->vt[1].fn((char*)o + o->vt[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326AE0);
#ifdef SKIP_ASM
extern "C" void func_00326CF0(void* p);

extern "C" void func_00326AE0(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x2EE8); i++) {
        func_00326CF0(((void**)((char*)self + 0x2EEC))[i]);
    }
}
#endif

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

//100%
INCLUDE_ASM("input/inputparse", func_00326CF0);
#ifdef SKIP_ASM
struct sVEntry00326CF0 {
    short delta;
    short index;
    void* fn;
};

typedef int (*CountFn00326CF0)(void*);
typedef void (*SetFn00326CF0)(void*, int, float);

extern "C" void func_00326CF0(void* p)
{
    sVEntry00326CF0** self = (sVEntry00326CF0**)p;
    int i;
    for (i = 0; i < ((CountFn00326CF0)(*self)[3].fn)((char*)self + (*self)[3].delta); i++) {
        ((SetFn00326CF0)(*self)[4].fn)((char*)self + (*self)[4].delta, i, 0.0f);
    }
}
#endif

INCLUDE_ASM("input/inputparse", func_00326D60);

//100%
INCLUDE_ASM("input/inputparse", func_00326DF0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern void* D_0048E488[];

extern "C" void* func_00326DF0(void* self, int a1, int a2)
{
    *(void***)self = D_0048E488;
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a2;
    func_00416210((char*)self + 0xC, 0, 0x24);
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326E50);
#ifdef SKIP_ASM
extern "C" int func_003FF8F0(int a, int b);
void operator_delete(int* ptr);
extern void* D_0048E488[];
extern char D_0048E4B8[];

extern "C" void func_00326E50(void* self, int flags)
{
    *(void***)self = D_0048E488;
    func_003FF8F0(*(int*)((char*)self + 0x4), *(int*)((char*)self + 0x8));
    *(void**)self = D_0048E4B8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("input/inputparse", func_00326EB0);

//100%
INCLUDE_ASM("input/inputparse", func_00327738__FPv);
#ifdef SKIP_ASM
int func_00327738(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327740);
#ifdef SKIP_ASM
static inline int clamp_327740(int v, int lo, int hi)
{
    if (v >= lo) {
        if (v > hi) {
            v = hi;
        }
    } else {
        v = lo;
    }
    return v;
}

extern "C" void func_00327740(void* self, int mode, float v)
{
    if (mode == 1) {
        *(int*)((char*)self + 0x30) = v != 0.0f;
        return;
    }
    int* p = (int*)((char*)self + 0x38);
    int r;
    if (v != 0.0f) {
        int t = (int)(v * 205.0f + 50.0f);
        r = clamp_327740(t, 50, 255);
    } else {
        r = 0;
    }
    *p = r;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_003277D0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0048E4B8[];

extern "C" void func_003277D0(void* self, int flags)
{
    *(void**)self = D_0048E4B8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("input/inputparse", func_00327828);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
void operator_delete(int* ptr);

extern "C" void func_00327828(void* self, int flags)
{
    if (*(void**)self != 0) {
        cMemMan_free(*(void**)self);
    }
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

