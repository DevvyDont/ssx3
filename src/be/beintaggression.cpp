#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A898[];
extern void* D_0045AC48[16];
extern void* D_004A124C;

struct cBEAggressionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis()
{
    if (D_004A124C == 0) {
        cBEAggressionInterface* mem = (cBEAggressionInterface*)cMemMan_alloc(0x10, D_0045A898, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AC48;
        D_004A124C = mem;
    }
    return D_004A124C;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", func_00155AA0__FPv);
#ifdef SKIP_ASM
void func_00155AA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", func_00155AA8__FPv);
#ifdef SKIP_ASM
void func_00155AA8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", func_00155AB0);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
void* cBENewRaceInterface_getThis();
extern "C" int func_00145750(void* race);

struct sAggrTriple {
    signed char v[3];
};

struct sAggrCharProfile {
    char pad_0x000[0xBC1];
    sAggrTriple aggression[16]; // 0xBC1
    char pad_0xBF1[0xF88 - 0xBC1 - 16 * 3];
};

extern sAggrCharProfile D_004A6CA8[][10];

extern "C" int func_00155AB0(void* self, int a, int b)
{
    int profile = cBELibrary_getProfileIndex(b);
    int ca = cBELibrary_getCharacterID(a);
    int cb = cBELibrary_getCharacterID(b);
    if (func_00145750(cBENewRaceInterface_getThis()) == ca)
        return 2;
    return D_004A6CA8[profile][cb].aggression[ca].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", func_00155B50);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
void* cBENewRaceInterface_getThis();
extern "C" int func_00145750(void* race);

extern sAggrCharProfile D_004A6CA8[][10];

extern "C" int func_00155B50(void* self, int a, int b)
{
    int profile = cBELibrary_getProfileIndex(b);
    int ca = cBELibrary_getCharacterID(a);
    int cb = cBELibrary_getCharacterID(b);
    if (func_00145750(cBENewRaceInterface_getThis()) == ca)
        return 3;
    return D_004A6CA8[profile][cb].aggression[ca].v[1];
}
#endif

INCLUDE_ASM("be/beintaggression", func_00155BF0);

INCLUDE_ASM("be/beintaggression", func_00155E58);

INCLUDE_ASM("be/beintaggression", func_001560A0);

INCLUDE_ASM("be/beintaggression", func_001561B0);

