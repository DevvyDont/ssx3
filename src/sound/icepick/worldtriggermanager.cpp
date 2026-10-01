#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldTriggerManager_cWorldTriggerManager(void* self);
extern const char D_00483588[];
extern void* D_004A52D4;

//99.88%
INCLUDE_ASM("sound/icepick/worldtriggermanager", WORLDTRIGGERMANAGER_Init__Fv);
#ifdef SKIP_ASM
void WORLDTRIGGERMANAGER_Init()
{
    if (D_004A52D4 == 0) {
        void* mem = cMemMan_alloc(0x344, D_00483588, 0, 0);
        D_004A52D4 = cWorldTriggerManager_cWorldTriggerManager(mem);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4B48);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4B70);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4BE0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4C08);
#ifdef SKIP_ASM
extern "C" int func_002B4C08(void* self, int a1, int a2, int a3)
{
    if (a1 == *(int*)((char*)self + 0x4)
        && a2 == *(int*)((char*)self + 0x8)
        && a3 == *(int*)((char*)self + 0x3c)) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B4C38);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5758);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5818);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5838);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5878);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5898);
#ifdef SKIP_ASM
extern unsigned char D_004A35A8[];

extern "C" void func_002B5898(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x30) = -1;
    *(int*)((char*)self + 0x34) = 0;
    *(unsigned char*)((char*)self + 0x40) = D_004A35A8[0];
    *(float*)((char*)self + 0x68) = -1.0f;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x6c) = 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_cWorldTriggerManager);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5988);

INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_LoadTriggerInfo);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5B90);
#ifdef SKIP_ASM
struct sWorldTrigger70 {
    int active;
    char pad[0x30];
    int unk34;
    char pad2[0x38];
};

extern "C" sWorldTrigger70* func_002B5B90(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 0) {
            return &(*self)[i];
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5BC0);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5C68);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5D78);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5E68);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B5F60);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B63D0);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6550);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B65B0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6628__FPv);
#ifdef SKIP_ASM
int func_002B6628(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6630);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B66E8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6740);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B67D8);
#ifdef SKIP_ASM
struct sTriggerRef {
    int id;
    void* obj;
};

extern "C" int func_002B67D8(void* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = *(sTriggerRef**)((char*)self + 0x33c);
    return *(int*)((char*)refs[i].obj + 0x4) == 1;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6808);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6868);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B68D0);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6900__FPv);
#ifdef SKIP_ASM
void func_002B6900(void* self)
{
    *(int*)((char*)self + 0x330) = 0;
    *(int*)((char*)self + 0x324) = 0;
    *(int*)((char*)self + 0x328) = -1;
    *(int*)((char*)self + 0x32c) = -1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6988);
#ifdef SKIP_ASM
extern "C" int func_002B6988(void* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = *(sTriggerRef**)((char*)self + 0x33c);
    return *(int*)((char*)refs[i].obj + 0x8) == 1;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B69B8);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6A68);
#ifdef SKIP_ASM
extern "C" void func_002B6A68(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1) {
            (*self)[i].unk34 = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6AA0);
#ifdef SKIP_ASM
struct sQuad6AA0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sQuad6AA0 D_004FF120;

struct sTriggerSlot120 {
    int unk0;
    char pad[0x10c];
    sQuad6AA0 q;        // 0x110
};

struct sTriggerSlots {
    sTriggerSlot120 slots[2];
};

extern "C" sTriggerSlots* func_002B6AA0(sTriggerSlots* self)
{
    sTriggerSlot120* p = self->slots;
    for (int i = 1; i != -1; i--, p++) {
        p->unk0 = 0;
        p->q = D_004FF120;
    }
    return self;
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6AE0);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B08);

extern "C" void func_002B6C20(void*);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B30);
#ifdef SKIP_ASM
extern "C" void func_002B6B30(void* self, int a1)
{
    if (a1 != 0) {
        func_002B6C20(self);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B50);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6C20);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6C90);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6CF8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6F40);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7318);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7410);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B75D0);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B77C0);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7848);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B7908);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B8190);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B82B8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B83F8);

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B8818);

