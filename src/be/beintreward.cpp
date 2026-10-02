#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A938[];
extern void* D_0045AE88[16];
extern void* D_004A1264;

struct cBERewardInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.84%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBERewardInterface_getThis()
{
    if (D_004A1264 == 0) {
        cBERewardInterface* mem = (cBERewardInterface*)cMemMan_alloc(0x18, D_0045A938, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AE88;
        D_004A1264 = mem;
    }
    return D_004A1264;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156A10);
#ifdef SKIP_ASM
extern "C" void cBERewardDB_init(void* db, int flag);
extern void* D_004C3E98[];

extern "C" void func_00156A10()
{
    cBERewardDB_init(D_004C3E98, 1);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156A38);
#ifdef SKIP_ASM
// PORT: compares a pointer cast to int against -1; not 64-bit safe.
extern void* D_004C3E98[];

extern "C" int func_00156A38(void)
{
    int r = 0;
    if ((int)D_004C3E98[0] == -1) {
        r = D_004C3E98[1] != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isBetterMedal__FPvii);
#ifdef SKIP_ASM
int cBERewardInterface_isBetterMedal(void* self, int a, int b)
{
    if (b == -1) {
        return 0;
    }
    if (a == -1) {
        return 1;
    }
    return b < a;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156A90);
#ifdef SKIP_ASM
extern "C" void* func_0015A478(void* self);

extern "C" char* func_00156A90(void* self, int a1, int a2)
{
    return *(char**)((char*)func_0015A478(self) + 0x8) + (a2 * 3 + a1) * 0xC;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156AE0);
#ifdef SKIP_ASM
extern "C" void* func_0015A478(void* self);

extern "C" char* func_00156AE0(void* self, int a1, int a2)
{
    return *(char**)((char*)func_0015A478(self) + 0xC) + (a1 * 4 + a2) * 0xC;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156B28);
#ifdef SKIP_ASM
extern "C" void* func_0015A478(void* self);

extern "C" char* func_00156B28(void* self, int a1, int a2)
{
    return *(char**)((char*)func_0015A478(self) + 0x10) + (a2 * 3 + a1) * 8;
}
#endif

INCLUDE_ASM("be/beintreward", func_00156C70);

INCLUDE_ASM("be/beintreward", func_00156EE0);

INCLUDE_ASM("be/beintreward", func_00157080);

INCLUDE_ASM("be/beintreward", func_00157210);

extern "C" void* func_0015A478(void* self);

//100%
INCLUDE_ASM("be/beintreward", func_001572B0__FPv);
#ifdef SKIP_ASM
int func_001572B0(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x3c);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_001572D0__FPv);
#ifdef SKIP_ASM
int func_001572D0(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x40);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_001572F0__FPv);
#ifdef SKIP_ASM
int func_001572F0(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x44);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157310__FPv);
#ifdef SKIP_ASM
int func_00157310(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x48);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157330__FPv);
#ifdef SKIP_ASM
int func_00157330(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x4c);
}
#endif

extern "C" void* func_0028B180(void* self);

//100%
INCLUDE_ASM("be/beintreward", func_00157350__FPv);
#ifdef SKIP_ASM
int func_00157350(void* self)
{
    return *(int*)((char*)func_0028B180(self) + 0x504);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157370__FPv);
#ifdef SKIP_ASM
int func_00157370(void* self)
{
    return *(int*)((char*)func_0015A478(self) + 0x50);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157390);
#ifdef SKIP_ASM
// Counts the set bits among the first `count` bits of `bits` (LSB first).
extern "C" int func_00157390(void* self, int count, signed char* bits)
{
    int i = 0;
    int total = 0;
    while (i < count)
    {
        signed char b = *bits++;
        for (int k = 0; k < 8 && i < count; k++)
        {
            i++;
            total += (b >> k) & 1;
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_001573F0);
#ifdef SKIP_ASM
extern "C" void func_001573F0(void* self, int bit, char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157430);
#ifdef SKIP_ASM
extern "C" int func_00157430(void* self, int bit, char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

INCLUDE_ASM("be/beintreward", func_00157468);

INCLUDE_ASM("be/beintreward", func_00157518);

INCLUDE_ASM("be/beintreward", func_001575C0);

INCLUDE_ASM("be/beintreward", func_00157620);

INCLUDE_ASM("be/beintreward", func_00157680);

INCLUDE_ASM("be/beintreward", func_001576E0);

INCLUDE_ASM("be/beintreward", func_00157740);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_001577A0);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157390(void* self, int count, signed char* bits);

extern "C" int func_001577A0(void* self, int a1, int a2)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157390(self, 0x14, (signed char*)(p + 0xF57));
}
#endif

INCLUDE_ASM("be/beintreward", func_001577E0);

//100%
INCLUDE_ASM("be/beintreward", func_001578A0);
#ifdef SKIP_ASM
extern "C" int func_00157A78(void* self, int a, int b, int i);

extern "C" int func_001578A0(void* self, int a, int b)
{
    for (int i = 0; i < 3; i++)
    {
        if (func_00157A78(self, a, b, i) == 0)
            return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("be/beintreward", func_00157920);

INCLUDE_ASM("be/beintreward", func_00157A78);

//100%
INCLUDE_ASM("be/beintreward", func_00157B08);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" int func_00157B70(void* self, int a, int b, int peak);

extern "C" int func_00157B08(void* self, int rider, int peak)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return func_00157B70(self, profile, cBELibrary_getCharacterID(rider), peak);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157B70);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

// PORT: 64-bit ulong flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
extern "C" int func_00157B70(void* self, int a, int b, int peak)
{
    if (peak == 1)
    {
        int bOff = b * 0xF88;
        int aOff = a * 0x9B50;
        int off = bOff + aOff;
        char* p = (char*)D_004A6CA8 + off;
        return (int)(*(ulong*)(p + 0x278) >> 12) & 1;
    }
    if (peak == 2)
    {
        int bOff = b * 0xF88;
        int aOff = a * 0x9B50;
        int off = bOff + aOff;
        char* p = (char*)D_004A6CA8 + off;
        return (int)(*(ulong*)(p + 0x278) >> 13) & 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("be/beintreward", func_00157BF0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157D60);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157D60(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit, p + 0xF30);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157DA0);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157DA0(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit, p + 0xF36);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157DE0);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157DE0(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit, p + 0xF45);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157E20);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157E20(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit, p + 0xF53);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157E60);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157E60(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit, p + 0xF52);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00157EE0);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157430(void* self, int bit, char* bits);

extern "C" int func_00157EE0(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157430(self, bit-10, p + 0xF57);
}
#endif

INCLUDE_ASM("be/beintreward", func_00157F20);

INCLUDE_ASM("be/beintreward", func_00157FD0);

INCLUDE_ASM("be/beintreward", func_001580F8);

INCLUDE_ASM("be/beintreward", func_00158220);

INCLUDE_ASM("be/beintreward", func_00158348);

INCLUDE_ASM("be/beintreward", func_00158470);

INCLUDE_ASM("be/beintreward", func_00158558);

INCLUDE_ASM("be/beintreward", func_00158618);

//100%
INCLUDE_ASM("be/beintreward", func_00158700);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" int func_00158700(void* self, int a1, int a2)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return *(int*)(p + 0xf80);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00158728);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int D_004A6CA8[];

extern "C" long func_00158728(void* self, int a1, int a2)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return *(long*)(p + 0xf70);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00158750);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int D_004A6CA8[];

extern "C" long func_00158750(void* self, int a1, int a2)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return *(long*)(p + 0xf78);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_001587B8);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_00157390(void* self, int count, signed char* bits);

extern "C" int func_001587B8(void* self, int a1, int a2)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_00157390(self, 0x40, (signed char*)(p + 0xF78));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_001587F8);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

extern "C" void func_001587F8(void* self, int a1, int a2, int value)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    *(int*)(p + 0xf80) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00158820);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int D_004A6CA8[];

extern "C" void func_00158820(void* self, int a1, int a2, long value)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    *(long*)(p + 0xf70) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00158848);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int D_004A6CA8[];

extern "C" void func_00158848(void* self, int a1, int a2, long value)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    *(long*)(p + 0xf78) = value;
}
#endif

extern int D_0045AFE8[];

//100%
INCLUDE_ASM("be/beintreward", func_00158870);
#ifdef SKIP_ASM
extern "C" int func_00158870(void* self, int a1, int a2)
{
    return *(int*)((char*)D_0045AFE8 + ((a2 << 2) + (a1 << 4)));
}
#endif

extern int D_0045B018[];

//100%
INCLUDE_ASM("be/beintreward", func_00158890);
#ifdef SKIP_ASM
extern "C" int func_00158890(void* self, int a1, int a2)
{
    return *(int*)((char*)D_0045B018 + ((a2 << 2) + (a1 << 4)));
}
#endif

extern void* D_0045B048[];

//100%
INCLUDE_ASM("be/beintreward", func_001588B0__FPvi);
#ifdef SKIP_ASM
int func_001588B0(void* self, int a1)
{
    return *(int*)((char*)(void*)D_0045B048 + a1 * 4);
}
#endif

void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern int D_004A6CA8[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackMedal__FPviiii);
#ifdef SKIP_ASM
struct sProfileSlot_00158910
{
    char data[0xF88];
};

signed char cBERewardInterface_getTrackMedal(void* self, int b, int c, int d, int e)
{
    void* result = cBECharProfileDB_getScoreStats(&((sProfileSlot_00158910 (*)[10])D_004A6CA8)[b][c], e, d);
    if (result == 0) {
        return -1;
    }
    return ((signed char*)result)[1];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00158960);
#ifdef SKIP_ASM
extern "C" int func_00158960(void* self, int b, int c, int d, int e)
{
    void* result = cBECharProfileDB_getScoreStats(&((sProfileSlot_00158910 (*)[10])D_004A6CA8)[b][c], e, d);
    if (result == 0) {
        return 0;
    }
    return ((int*)result)[1];
}
#endif

INCLUDE_ASM("be/beintreward", func_001589B0);

INCLUDE_ASM("be/beintreward", func_00158A50);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getEarningsMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEEconInterface_getTotalMoneyEarned(void* self, int a, int b);

extern "C" int cBERewardInterface_getEarningsMedal(void* self, int a, int b, int medal)
{
    unsigned int earned = cBEEconInterface_getTotalMoneyEarned(cBE_getInterface_Fv(cBE_getBE(), 0xB), a, b);
    if (earned >= *(unsigned int*)((char*)(void*)D_0045B048 + medal * 4))
        return 1;
    return -1;
}
#endif

INCLUDE_ASM("be/beintreward", func_00158BE0);

INCLUDE_ASM("be/beintreward", func_00158C80);

INCLUDE_ASM("be/beintreward", func_00158D58);

INCLUDE_ASM("be/beintreward", func_00158E30);

extern void* D_004C3EF0[];

//100%
INCLUDE_ASM("be/beintreward", func_00158F20__FPv);
#ifdef SKIP_ASM
void* func_00158F20(void* self)
{
    return (void*)D_004C3EF0;
}
#endif

extern "C" void* func_0015A6F0(void*);

//100%
INCLUDE_ASM("be/beintreward", func_00158F30__FPv);
#ifdef SKIP_ASM
void* func_00158F30(void* self)
{
    return func_0015A6F0((void*)D_004C3EF0);
}
#endif

INCLUDE_ASM("be/beintreward", func_00158F60);

//100%
INCLUDE_ASM("be/beintreward", func_00159080);
#ifdef SKIP_ASM
extern "C" int func_00159080(void* self, int a1, int a2, int kind, int tier)
{
    int r = -1;
    switch (tier)
    {
    case 0:
        if (kind == 0)
            r = 5;
        else if (kind == 1)
            r = 6;
        else if (kind == 2)
            r = 7;
        break;
    case 1:
        if (kind == 0)
            r = 8;
        else if (kind == 1)
            r = 9;
        else if (kind == 2)
            r = 10;
        break;
    case 2:
        if (kind == 0)
            r = 11;
        else if (kind == 1)
            r = 12;
        else if (kind == 2)
            r = 13;
        break;
    case 3:
        if (kind == 0)
            r = 14;
        else if (kind == 1)
            r = 15;
        else if (kind == 2)
            r = 16;
        break;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", func_00159170);
#ifdef SKIP_ASM
extern "C" int func_0015A2E0(void* self, int a1, int a2, int bit);
extern "C" void func_00159CD0(void* self, int a1, int a2, int bit);

extern "C" void func_00159170(void* self, int a1, int a2, int kind, int tier)
{
    int bit = func_00159080(self, a1, a2, kind, tier);
    if (func_0015A2E0(self, a1, a2, bit) == 0)
    {
        func_00159CD0(self, a1, a2, bit);
    }
}
#endif

INCLUDE_ASM("be/beintreward", func_001591E8);

//100%
INCLUDE_ASM("be/beintreward", func_001597B0);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void func_00159818(void* self, int a, int b, int c);

extern "C" void func_001597B0(void* self, int rider, int c)
{
    int profile = cBELibrary_getProfileIndex(rider);
    func_00159818(self, profile, cBELibrary_getCharacterID(rider), c);
}
#endif

INCLUDE_ASM("be/beintreward", func_00159818);

INCLUDE_ASM("be/beintreward", func_001599A0);

INCLUDE_ASM("be/beintreward", func_00159B08);

INCLUDE_ASM("be/beintreward", func_00159CD0);

//100%
INCLUDE_ASM("be/beintreward", func_0015A2E0);
#ifdef SKIP_ASM
extern int D_004A6CA8[];
extern "C" int func_0015A320(void* self, int bit, unsigned char* bits);

extern "C" int func_0015A2E0(void* self, int a1, int a2, int bit)
{
    char* p = (char*)D_004A6CA8 + a1 * 0x9b50 + a2 * 0xf88;
    return func_0015A320(self, bit, (unsigned char*)(p + 0xF28));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A320);
#ifdef SKIP_ASM
extern "C" int func_0015A320(void* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A358);
#ifdef SKIP_ASM
extern "C" void func_0015A358(void* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

// 0x64-byte array elements; arr[i].field indexing reproduces the target's
// base-first addu
struct sBERewardEntry {
    char pad_0x00[0x54];
    int field_0x54;
    char pad_0x58[0xc];
};
extern sBERewardEntry D_0043D950[];

//100%
INCLUDE_ASM("be/beintreward", func_0015A398);
#ifdef SKIP_ASM
extern "C" int func_0015A398(void* self, int a1)
{
    return D_0043D950[a1].field_0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A3B8);
#ifdef SKIP_ASM
struct sRewardPair_0015A3B8
{
    signed char a;
    signed char b;
};

extern sRewardPair_0015A3B8 D_0045AAD8[3][4][5];

extern "C" int func_0015A3B8(void* self, int a, int b)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if (D_0045AAD8[i][j][k].a == a)
                {
                    if (D_0045AAD8[i][j][k].b == b || a == 6 || a == 7 || a == 8 || a == 9 || a == 10 || a == 11)
                        return j;
                }
            }
        }
    }
    return -1;
}
#endif

extern void* D_004C3E98[];

INCLUDE_ASM("be/beintreward", func_0015A478);
#ifdef SKIP_ASM
void* func_0015A478(void* self)
{
    return (void*)D_004C3E98;
}
#endif

INCLUDE_ASM("be/beintreward", func_0015A488);

//100%
INCLUDE_ASM("be/beintreward", func_0015A510);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int c, int n);

extern "C" void func_0015A510(void* self)
{
    func_003E6448(self, 0, 0x174);
    *(short*)((char*)self + 0x4) = -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A5B0);
#ifdef SKIP_ASM
extern "C" void func_0015A5B0(void* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A5F0);
#ifdef SKIP_ASM
extern "C" int func_0015A5F0(void* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

INCLUDE_ASM("be/beintreward", func_0015A628);

//100%
INCLUDE_ASM("be/beintreward", func_0015A6F0);
#ifdef SKIP_ASM
struct sRewardGrid_0015A6F0
{
    char pad_0x00[0x6];
    signed char vals[32][10]; // 0x6 (func_0015A750 reads vals[a1][a2])
};

// PORT: the unit declares func_0015A6F0 as returning void* (line ~378, used by
// func_00158F30); the body returns an int sum, so it is bound by asm label.
// The lander should change that declaration to int instead.
int func_0015A6F0_impl(sRewardGrid_0015A6F0* self) __asm__("func_0015A6F0");

int func_0015A6F0_impl(sRewardGrid_0015A6F0* self)
{
    int sum = 0;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            sum += self->vals[i][j];
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A750);
#ifdef SKIP_ASM
extern "C" signed char func_0015A750(void* self, int a1, int a2)
{
    return *(signed char*)((char*)self + (a1 * 0xa + a2) + 0x6);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A768__FPvi);
#ifdef SKIP_ASM
int func_0015A768(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x174);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A778);
#ifdef SKIP_ASM
extern "C" void* func_0015A778(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x0) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x34) = 0;
    *(int*)((char*)self + 0x38) = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_0015A7D0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_0015AE00(void* self);

extern "C" void func_0015A7D0(int* self, int flags)
{
    func_0015AE00(self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("be/beintreward", func_0015A818);

