#include "common.h"

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushExplicit);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);

class cUIStateStack_pushExplicit_cState {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

extern "C" void cUIStateStack_pushExplicit(void* self, cUIStateStack_pushExplicit_cState* state)
{
    *(int*)((char*)state + 0x1C) |= 0x60;
    state->v04();
    cList_addToEnd((cList*)((char*)self + 0x1C), (cListNode*)state);
}
#endif

INCLUDE_ASM("ui/uistatestack", func_0039F290);

INCLUDE_ASM("ui/uistatestack", cUIStateStack_pushSpecial);

INCLUDE_ASM("ui/uistatestack", func_0039F400);

INCLUDE_ASM("ui/uistatestack", func_0039F4C0);

INCLUDE_ASM("ui/uistatestack", func_0039F600);

INCLUDE_ASM("ui/uistatestack", func_0039F698);

INCLUDE_ASM("ui/uistatestack", func_0039F718);

INCLUDE_ASM("ui/uistatestack", func_0039F7B8);

INCLUDE_ASM("ui/uistatestack", func_0039F840);

INCLUDE_ASM("ui/uistatestack", func_0039F8C8);

INCLUDE_ASM("ui/uistatestack", func_0039F9D8);

//100%
INCLUDE_ASM("ui/uistatestack", cUIStateStack_getCurrentState);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);
struct cListNode;
int cListNode_isSentinel(cListNode*);

extern "C" void* cUIStateStack_getCurrentState(void* self)
{
    void* n = cList_first((cList*)self);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            if ((*(int*)((char*)n + 0x1C) >> 5) & 1) {
                return n;
            }
            n = *(void**)((char*)n + 4);
        }
    }
    return 0;
}
#endif

struct cList;
void* cList_first(cList*);

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FAE8);
#ifdef SKIP_ASM
extern "C" void* func_0039FAE8(void* self)
{
    return cList_first((cList*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FB30);
#ifdef SKIP_ASM
struct func_0039FB30_sVec3 {
    float x;
    float y;
    float z;
};

extern func_0039FB30_sVec3 D_004FF0D8;
extern char D_00494BD8[];

struct func_0039FB30_sFlags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 5;
    unsigned int align : 6;
};

struct func_0039FB30_sUIObj {
    void* prev;
    void* next;
    void* vtbl;
    int unkC;
    char unk10;
    func_0039FB30_sFlags flags;
    int unk18;
    float color[4];
    func_0039FB30_sVec3 pos;
    int unk38;
    int unk3C;
    int unk40;
    func_0039FB30_sVec3 rot;
    func_0039FB30_sVec3 scale;
    int unk5C;
    func_0039FB30_sVec3 offs;
    int unk6C;
    int unk70;
};

extern "C" func_0039FB30_sUIObj* func_0039FB30(func_0039FB30_sUIObj* self, int a1, int a2)
{
    self->vtbl = D_00494BD8;
    self->flags.b0 = 1;
    self->flags.b7 = 0;
    self->flags.b8 = 0;
    self->flags.b5 = 0;
    self->flags.b4 = 0;
    self->flags.b2 = 0;
    self->flags.b1 = 1;
    self->flags.b3 = 1;
    self->flags.b6 = 1;
    self->flags.align = 9;
    self->next = self;
    self->prev = self;
    self->unkC = 0;
    self->unk10 = 0;
    self->unk18 = 0;
    self->color[0] = 1.0f;
    self->color[1] = 1.0f;
    self->color[2] = 1.0f;
    self->color[3] = 1.0f;
    self->pos = D_004FF0D8;
    self->unk40 = a2;
    self->unk38 = 0;
    self->unk3C = 0;
    self->rot = D_004FF0D8;
    self->scale.x = 1.0f;
    self->scale.y = 1.0f;
    self->scale.z = 1.0f;
    self->unk5C = a1;
    self->offs = D_004FF0D8;
    self->unk6C = 4;
    self->unk70 = 6;
    return self;
}
#endif

INCLUDE_ASM("ui/uistatestack", func_0039FC48);

INCLUDE_ASM("ui/uistatestack", func_0039FCC8);

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FD38);
#ifdef SKIP_ASM
struct sVEntry39FD38 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0039FD38(void* self)
{
    void* obj = *(void**)((char*)self + 0xC);
    if (obj != 0) {
        sVEntry39FD38* vt = *(sVEntry39FD38**)((char*)obj + 0x10);
        vt[1].fn((char*)obj + vt[1].delta, 3);
        *(void**)((char*)self + 0xC) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uistatestack", func_0039FE00);

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FEB8);
#ifdef SKIP_ASM
struct sUIAlignFlags14 {
    unsigned int lo : 13;
    unsigned int align : 6;
};

struct sUIVec2 {
    float x;
    float y;
};

static inline unsigned int getUIAlign(void* self)
{
    return ((sUIAlignFlags14*)((char*)self + 0x14))->align;
}

extern "C" void func_0039FEB8(void* self, sUIVec2* pos, sUIVec2* size)
{
    unsigned int align = getUIAlign(self);
    if ((align & 1) == 0) {
        if (align & 4) {
            float d = size->y;
            pos->y -= d;
        } else {
            pos->y -= size->y * 0.5f;
        }
    }
    if ((align & 8) == 0) {
        if (align & 0x20) {
            pos->x -= size->x;
        } else {
            pos->x -= size->x * 0.5f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FF50__FPv);
#ifdef SKIP_ASM
int func_0039FF50(void* self)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
}
#endif

//100%
INCLUDE_ASM("ui/uistatestack", func_0039FF60);
#ifdef SKIP_ASM
extern "C" void* func_0039FF60(void* self, int a1)
{
    return *(int*)((char*)self + 0x38) == a1 ? self : 0;
}
#endif

