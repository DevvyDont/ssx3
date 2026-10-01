#include "common.h"

extern int D_005305B0[];

struct sCharEntry {
    char pad_0x00[0x11];
    signed char mCharID; // 0x11
    char pad_0x12[0x1C - 0x11 - 1];
};
extern sCharEntry D_00535B20[];

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getCharacterID__Fi);
#ifdef SKIP_ASM
signed char cBELibrary_getCharacterID(int index)
{
    int charID = D_005305B0[index];
    sCharEntry* entry = &D_00535B20[charID];
    return entry->mCharID;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014A0B0);
#ifdef SKIP_ASM
extern "C" signed char func_0014A0B0(int index)
{
    int charID = D_005305B0[index];
    signed char* entry = (signed char*)((char*)D_00535B20 + charID * 0x1C);
    return entry[0x12];
}
#endif

struct sCharEntry2 {
    char pad_0x00[0xC];
    int field_0xC;
    int field_0x10;
};

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getProfileIndex__Fi);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int index)
{
    int charID = D_005305B0[index];
    sCharEntry2* entry = (sCharEntry2*)((char*)D_00535B20 + charID * 0x1C);
    if (entry->field_0xC == -1) {
        return 2;
    }
    return entry->field_0x10 & 1;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getRiderIndex__Fi);
#ifdef SKIP_ASM
int cBELibrary_getRiderIndex(int flag)
{
    int* base = D_005305B0;
    int count = 0;
    while (1) {
        int charID = *base;
        sCharEntry2* entry = (sCharEntry2*)((char*)D_00535B20 + charID * 0x1C);
        int bit = entry->field_0x10 & 1;
        if (bit == flag) {
            return count;
        }
        count++;
        if (count < 6) {
            base++;
            continue;
        }
        return -1;
    }
}
#endif

INCLUDE_ASM("be/belibrary", func_0014A188);

INCLUDE_ASM("be/belibrary", func_0014A5E0);

INCLUDE_ASM("be/belibrary", func_0014AB20);

INCLUDE_ASM("be/belibrary", cBELibrary_getScoreType);

INCLUDE_ASM("be/belibrary", func_0014ABE0);

//100%
INCLUDE_ASM("be/belibrary", func_0014AC30);
#ifdef SKIP_ASM
extern "C" signed char func_0014AC30(int a0)
{
    return cBELibrary_getCharacterID(a0);
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("be/belibrary", func_0014AC50);
#ifdef SKIP_ASM
extern "C" int func_0014AC50(int a0)
{
    return cBELibrary_getProfileIndex(a0);
}
#endif

INCLUDE_ASM("be/belibrary", func_0014ACB0);

//100%
INCLUDE_ASM("be/belibrary", func_0014AD28);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" void* func_0014AD28(void* self, int a1, int a2)
{
    return (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014AD50);

INCLUDE_ASM("be/belibrary", func_0014AEA8);

INCLUDE_ASM("be/belibrary", func_0014AF10);

INCLUDE_ASM("be/belibrary", func_0014AFB0);

INCLUDE_ASM("be/belibrary", func_0014B478);

INCLUDE_ASM("be/belibrary", func_0014B560);

INCLUDE_ASM("be/belibrary", func_0014B700);

INCLUDE_ASM("be/belibrary", func_0014B988);

extern void* D_004A6750[];
extern "C" void* func_0014C6A0(void*);

//100%
INCLUDE_ASM("be/belibrary", func_0014BD78__FPv);
#ifdef SKIP_ASM
void* func_0014BD78(void* self)
{
    return func_0014C6A0((void*)D_004A6750);
}
#endif

extern "C" void* func_0014D068(void*);

//100%
INCLUDE_ASM("be/belibrary", func_0014BD98__FPv);
#ifdef SKIP_ASM
void* func_0014BD98(void* self)
{
    return func_0014D068((void*)D_004A6750);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014BDB8__FPv);
#ifdef SKIP_ASM
void* func_0014BDB8(void* self)
{
    return (void*)D_004A6750;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014BE70);

INCLUDE_ASM("be/belibrary", func_0014C2B0);

INCLUDE_ASM("be/belibrary", func_0014C320);

INCLUDE_ASM("be/belibrary", func_0014C3C8);

INCLUDE_ASM("be/belibrary", func_0014C488);

//100%
INCLUDE_ASM("be/belibrary", func_0014C620);
#ifdef SKIP_ASM
extern "C" void* func_0014C620(void* self)
{
    int* p = (int*)self;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    p[8] = 0;
    p[9] = 0;
    p[10] = 0;
    return self;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014C658);

INCLUDE_ASM("be/belibrary", func_0014C6A0);

INCLUDE_ASM("be/belibrary", func_0014C7A0);

INCLUDE_ASM("be/belibrary", func_0014D068);

INCLUDE_ASM("be/belibrary", func_0014D240);

INCLUDE_ASM("be/belibrary", func_0014D448);

INCLUDE_ASM("be/belibrary", func_0014D4C8);

INCLUDE_ASM("be/belibrary", func_0014D558);

INCLUDE_ASM("be/belibrary", func_0014D608);

INCLUDE_ASM("be/belibrary", func_0014D7E8);

INCLUDE_ASM("be/belibrary", func_0014D908);

//100%
INCLUDE_ASM("be/belibrary", func_0014D988__FPvi);
#ifdef SKIP_ASM
int func_0014D988(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x2C);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014D998);
#ifdef SKIP_ASM
struct sBELibTables {
    char pad_0x00[4];
    char* field_0x4;
    char pad_0x8[4];
    char* field_0xC;
    char* field_0x10;
    char pad_0x14[0x2C - 0x14];
    int count0[30]; // 0x2C
    int index0[30]; // 0xA4
    char pad_0x11C[0x2FC - 0x11C];
    int count1[30]; // 0x2FC
    int index1[30]; // 0x374
    int count2[30]; // 0x3EC
    int index2[30]; // 0x464
};

extern "C" void* func_0014D998(sBELibTables* self, int i)
{
    if (self->count0[i] <= 0) {
        return 0;
    }
    return self->field_0x4 + self->index0[i] * 0x38;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014D9D0);
#ifdef SKIP_ASM
extern "C" void* func_0014D9D0(sBELibTables* self, int i)
{
    if (self->count0[i] <= 0) {
        return 0;
    }
    return self->field_0x4 + self->index0[i] * 0x38;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014DB40);

//100%
INCLUDE_ASM("be/belibrary", func_0014DC00__FPvi);
#ifdef SKIP_ASM
int func_0014DC00(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x2FC);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014DC10);
#ifdef SKIP_ASM
extern "C" void* func_0014DC10(sBELibTables* self, int i)
{
    if (self->count1[i] > 0) {
        return self->field_0xC + self->index1[i] * 8;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014DC40__FPvi);
#ifdef SKIP_ASM
int func_0014DC40(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x3EC);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", func_0014DC50);
#ifdef SKIP_ASM
extern "C" void* func_0014DC50(sBELibTables* self, int i)
{
    if (self->count2[i] > 0) {
        return self->field_0x10 + self->index2[i] * 8;
    }
    return 0;
}
#endif

