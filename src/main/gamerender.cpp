#include "common.h"

INCLUDE_ASM("main/gamerender", cGameViewMan_cGameViewMan);

INCLUDE_ASM("main/gamerender", func_0022E550);

INCLUDE_ASM("main/gamerender", func_0022E730);

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

