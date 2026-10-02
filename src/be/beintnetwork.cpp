#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A648[];
extern void* D_0045AD98[16];
extern void* D_004A1204;

struct cBENetworkInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBENetworkInterface_getThis()
{
    if (D_004A1204 == 0) {
        cBENetworkInterface* mem = (cBENetworkInterface*)cMemMan_alloc(0x10, D_0045A648, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AD98;
        D_004A1204 = mem;
    }
    return D_004A1204;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E130);
#ifdef SKIP_ASM
struct sNetState_0014E130
{
    int data[0x4B0 / 4];
};

extern sNetState_0014E130 D_00534B30;
extern sNetState_0014E130 D_00535088;

extern "C" void func_0014E130(void)
{
    D_00535088 = D_00534B30;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E1F8);
#ifdef SKIP_ASM
extern sNetState_0014E130 D_00534B30;
extern sNetState_0014E130 D_00535088;

extern "C" void func_0014E1F8(void)
{
    D_00534B30 = D_00535088;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E2C0__FPv);
#ifdef SKIP_ASM
int func_0014E2C0(void* self)
{
    return 0xC44;
}
#endif

INCLUDE_ASM("be/beintnetwork", func_0014E2C8);

INCLUDE_ASM("be/beintnetwork", func_0014E5C8);

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E9C0);
#ifdef SKIP_ASM
struct sBEPlayerData {
    char data[0x9B50];
};
extern sBEPlayerData D_004A6CA8[];

extern "C" void func_00156858(sBEPlayerData* dst, sBEPlayerData* src);

extern "C" void func_0014E9C0(void* self, int a1, int a2)
{
    sBEPlayerData* src = &D_004A6CA8[a1];
    func_00156858(&D_004A6CA8[a2], src);
}
#endif

INCLUDE_ASM("be/beintnetwork", func_0014E9F8);

INCLUDE_ASM("be/beintnetwork", func_0014EA90);

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EB08);
#ifdef SKIP_ASM
struct sNetSlot_0014EB08
{
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 1;
    unsigned int rest : 28;
    int x;
    int y;
};

extern sNetSlot_0014EB08 D_005308B8[];

extern "C" void func_0014EB08(void)
{
    sNetSlot_0014EB08 tmp = D_005308B8[0];
    D_005308B8[0].y = D_005308B8[1].y;
    D_005308B8[0].c = D_005308B8[1].c;
    D_005308B8[0].a = D_005308B8[1].a;
    D_005308B8[0].x = D_005308B8[1].x;
    D_005308B8[1].y = tmp.y;
    D_005308B8[1].c = tmp.c;
    D_005308B8[1].a = tmp.a;
    D_005308B8[1].x = tmp.x;
}
#endif

extern "C" char* strcpy(char*, const char*);
extern char D_00534FC8[16];

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EBB8);
#ifdef SKIP_ASM
extern "C" void func_0014EBB8(void* self, const char* src)
{
    strcpy(D_00534FC8, src);
}
#endif

extern char D_00534FB8[16];

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EBD8);
#ifdef SKIP_ASM
extern "C" void func_0014EBD8(void* self, const char* src)
{
    strcpy(D_00534FB8, src);
}
#endif

INCLUDE_ASM("be/beintnetwork", func_0014EBF8);

INCLUDE_ASM("be/beintnetwork", func_0014ECA0);

INCLUDE_ASM("be/beintnetwork", func_0014EDD8);

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EE28);
#ifdef SKIP_ASM
signed char cBELibrary_getCharacterID(int index);

struct sNetChar_0014EE28
{
    char data[0x88];
};

extern sNetChar_0014EE28 D_00530970[];

extern "C" void* func_0014EE28(void* self, int rider)
{
    return &D_00530970[cBELibrary_getCharacterID(rider)];
}
#endif

INCLUDE_ASM("be/beintnetwork", func_0014EE58);

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EEC8);
#ifdef SKIP_ASM
struct sNetEntry8_0043FA38
{
    char data[8];
};

struct sNetEntry88_00530990
{
    char data[0x88];
};

extern sNetEntry8_0043FA38 D_0043FA38[];
extern sNetEntry88_00530990 D_00530990[];

extern "C" void* func_0014EEC8(void* self, int player, int index)
{
    if (index >= 10 && index < 30) {
        return &D_0043FA38[index];
    }
    return &D_00530990[player];
}
#endif

