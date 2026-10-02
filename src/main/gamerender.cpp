#include "common.h"

INCLUDE_ASM("main/gamerender", cGameViewMan_cGameViewMan);

INCLUDE_ASM("main/gamerender", func_0022E550);

//100%
INCLUDE_ASM("main/gamerender", func_0022E730);
#ifdef SKIP_ASM
struct sVEntry0022E730 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sItem0022E730 {
    char pad[0x90];
    sVEntry0022E730* vt;
};
struct sList0022E730 {
    unsigned int count;
    sItem0022E730* items[3];
    int field_0x10;
};
void operator_delete(int* p);

extern "C" void func_0022E730(sList0022E730* self, int flags)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        sItem0022E730* it = self->items[i];
        if (it != 0) {
            it->vt[1].fn((char*)it + it->vt[1].delta, 3);
        }
    }
    self->field_0x10 = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E7C8);
#ifdef SKIP_ASM
struct sVEntry_0022E7C8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0022E7C8(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        void* v = ((void**)((char*)self + 0x4))[i];
        void* o = *(void**)((char*)v + 0xA0);
        sVEntry_0022E7C8* vt = *(sVEntry_0022E7C8**)((char*)o + 0x14);
        vt[4].fn((char*)o + vt[4].delta);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_updateAll);
#ifdef SKIP_ASM
struct sVEntry_0022E840 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void cGameViewMan_updateAll(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        void* v = ((void**)((char*)self + 0x4))[i];
        sVEntry_0022E840* vt = *(sVEntry_0022E840**)((char*)v + 0x90);
        vt[3].fn((char*)v + vt[3].delta);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E8B8);
#ifdef SKIP_ASM
extern "C" void func_0015EC98(void* p);

extern "C" void func_0022E8B8(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        func_0015EC98(((void**)((char*)self + 0x4))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E920);
#ifdef SKIP_ASM
struct sRenderList {
    int field_0x0;
    void* items[3];
    unsigned int count;
};

extern "C" int func_0022E920(sRenderList* self)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        if (*(int*)((char*)self->items[i] + 0xb4) == 0) {
            return 0;
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("main/gamerender", func_0022E968);

