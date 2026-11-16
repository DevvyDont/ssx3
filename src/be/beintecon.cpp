#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEEconInt[];
extern void* D_0045AC78[16];
extern void* cBEEconInterface_instance;

struct cBEEconInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's economy interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEEconInt", clears the word at +0x8 and stores the vtable.
 *
 * @return the economy interface
 */
void* cBEEconInterface_getThis()
{
    if (cBEEconInterface_instance == 0) {
        cBEEconInterface* mem = (cBEEconInterface*)cMemMan_alloc(0x10, str_BEEconInt, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AC78;
        cBEEconInterface_instance = mem;
    }
    return cBEEconInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMoney);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Returns a character's cash (+0xAC4 of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the cash, or 0
 */
extern "C" int cBEEconInterface_getMoney(void* self, int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    int bOff = charType * 0xF88;
    int aOff = iHuman * 0x9B50;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC4);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150960);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern "C" int func_00150960(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int b = cBELibrary_getCharacterID(rider);
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)cBELibrary_profileLib + off;
    return *(int*)(p + 0xAC4);
}
#endif

extern int cBELibrary_profileLib[];

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned__FPvii);
#ifdef SKIP_ASM
/**
 * Returns the total money a character has earned (+0xAC8 of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the total earned, or 0
 */
int cBEEconInterface_getTotalMoneyEarned(void* self, int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    int bOff = charType * 0xF88;
    int aOff = iHuman * 0x9B50;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC8);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Returns the total money earned by a rider's character, found from the rider's profile and character.
 *
 * @param rider  the rider index (On Tour: iRider)
 * @return the total earned (+0xAC8 of the profile record)
 */
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int bOff = characterId * 0xF88;
    int aOff = profileIndex * 0x9B50;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC8);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150A58);
#ifdef SKIP_ASM
extern "C" void func_00150A58(void* self, int profileIndex, int characterId, int value)
{
    if (profileIndex < 2) {
        int bOff = characterId * 0xF88;
        int aOff = profileIndex * 0x9B50;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        *(int*)(profile + 0xAC4) = value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150A90);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

extern "C" int func_00150A90(void* self, int a, int b, int amount)
{
    if (a >= 2) {
        return 0;
    }
    int bOff = b * 0xF88;
    int aOff = a * 0x9B50;
    int off = bOff + aOff;
    char* p = (char*)cBELibrary_profileLib + off;
    *(int*)(p + 0xAC4) += amount;
    *(int*)(p + 0xAC8) += amount;
    return *(int*)(p + 0xAC4);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150AE0);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern "C" int func_00150AE0(void* self, int rider, int amount)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return func_00150A90(self, profile, cBELibrary_getCharacterID(rider), amount);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150B48);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

extern "C" int func_00150B48(void* self, int profileIndex, int characterId, int amount)
{
    if (profileIndex >= 2) {
        return 0;
    }
    int bOff = characterId * 0xF88;
    int aOff = profileIndex * 0x9B50;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC4) -= amount;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150B88);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

// PORT: 64-bit unsigned long flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
struct sPeakBits_0B88 {
    unsigned long low : 12;
    unsigned long peak1 : 1;
    unsigned long peak2 : 1;
    unsigned long rest : 50;
};

// Sets the locked state of a peak for a profile/character: stores the inverse of `unlocked`.
extern "C" int func_00150B88(void* self, int profileIndex, int characterId, int peak, int unlocked)
{
    if (profileIndex >= 2) {
        return 0;
    }
    unlocked ^= 1;
    if (peak == 1) {
        int bOff = characterId * 0xF88;
        int aOff = profileIndex * 0x9B50;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        ((sPeakBits_0B88*)(profile + 0x278))->peak1 = unlocked;
    } else if (peak == 2) {
        int bOff = characterId * 0xF88;
        int aOff = profileIndex * 0x9B50;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        ((sPeakBits_0B88*)(profile + 0x278))->peak2 = unlocked;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150C20);
#ifdef SKIP_ASM
struct sEconRec_0C20 {
    char pad0[0xAC4];
    int unk_AC4;
    char pad1[0xBDF - 0xAC8];
    signed char f[7];
    char pad2[0xF88 - 0xBDF - 7];
};
class cBEIface_0C20 {
public:
    char pad[0xC];
    virtual void v1();
    virtual void v2();
};
extern "C" cBEIface_0C20* cBEStatInterface_getThis();
int func_00150E50(void* self, int a1);
extern "C" int func_00150C20(void* self, int arg1, int arg2, unsigned int arg3) {
    signed char *p;
    int cur;
    int cost;

    if (arg1 >= 2) {
        return 0;
    }
    {
        p = 0;
        cur = ((sEconRec_0C20*)((arg1 * 0x9B50 + arg2 * 0xF88) + (char*)cBELibrary_profileLib))->unk_AC4;
        switch (arg3) {
        case 0:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[0];
            break;
        case 1:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[1];
            break;
        case 2:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[2];
            break;
        case 3:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[3];
            break;
        case 4:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[4];
            break;
        case 5:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[5];
            break;
        case 6:
            p = &((sEconRec_0C20(*)[10])cBELibrary_profileLib)[arg1][arg2].f[6];
            break;
        }
        cost = func_00150E50(self, *p / 5 - 1);
        if (cur < cost || *p == 0x37) {
            return 0;
        }
        sEconRec_0C20* r = (sEconRec_0C20*)((arg2 * 0xF88 + arg1 * 0x9B50) + (char*)cBELibrary_profileLib);
        r->unk_AC4 = r->unk_AC4 - cost;
        *p = (unsigned char)*p + 1;
        cBEStatInterface_getThis()->v2();
        return 1;
    }
}
#endif

extern void* D_00440550[];

//100%
INCLUDE_ASM("be/beintecon", func_00150E50__FPvi);
#ifdef SKIP_ASM
struct sEconEntry {
    int key;
    int value;
};

struct sEconTable {
    sEconEntry entries[1];
};

int func_00150E50(void* self, int a1)
{
    return ((sEconTable*)D_00440550)->entries[a1].value;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00150E68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sProf0E68 { char s[0xF88]; };
struct sEcon0E68 { int a; int b; };
extern sEcon0E68 D_00440550_0E68[] __asm__("D_00440550");

extern "C" int func_00150E68(void* self, int profile, int c, unsigned int kind)
{
    char* p;
    if (profile >= 2) return 0;
    p = 0;
    switch (kind) {
    case 0: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBDF]; break;
    case 1: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE0]; break;
    case 2: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE1]; break;
    case 3: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE2]; break;
    case 4: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE3]; break;
    case 5: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE4]; break;
    case 6: p = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][c].s[0xBE5]; break;
    }
    if (*p == 5) return 0;
    {
        int v = D_00440550_0E68[(*p - 1) / 5].b;
        int off = c * 0xF88 + profile * 0x9B50;
        char* q = (char*)cBELibrary_profileLib + off;
        *(int*)(q + 0xAC4) += v;
    }
    (*p)++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151040);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00145630(void* race, int place, int kind);

struct sEconPrize_151040
{
    int key;
    int prize[4];
};
extern sEconPrize_151040 D_004405A0[];

extern "C" int func_00151040(void* self, int medal, int key, int place)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int r = 0;
    if ((unsigned int)(place - 6) < 6)
    {
        switch (medal)
        {
        case 0:
        case 1:
            r = func_00145630(race, place, 2);
            break;
        case 2:
            r = func_00145630(race, place, 1);
            break;
        case 3:
            r = func_00145630(race, place, 0);
            break;
        }
    }
    else
    {
        for (int i = 0; i < 17; i++)
        {
            if (D_004405A0[i].key != key) continue;
            switch (medal)
            {
            case 0:
                r = D_004405A0[i].prize[0] * 100;
                break;
            case 1:
                r = D_004405A0[i].prize[1] * 100;
                break;
            case 2:
                r = D_004405A0[i].prize[2] * 100;
                break;
            case 3:
                r = D_004405A0[i].prize[3] * 100;
                break;
            }
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151178);
#ifdef SKIP_ASM
extern "C" int func_00151178(void* self, int level)
{
    switch (level)
    {
    case 0:
    default:
        return 500;
    case 1:
        return 1000;
    case 2:
        return 2000;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001511B0);
#ifdef SKIP_ASM
extern "C" int func_001511B0(void* self, int level)
{
    switch (level)
    {
    case 1:
    default:
        return 2000;
    case 2:
        return 4000;
    case 3:
        return 6000;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001511E8);
#ifdef SKIP_ASM
struct sEconRec {
    char pad0[0xBDF];
    signed char f[7];
    char pad1[0xF88 - 0xBDF - 7];
};
extern "C" int func_001511E8(void* self, int arg1, int arg2, unsigned int arg3) {
    signed char *p;
    if (arg1 >= 2) {
        return 0;
    }
    {
        p = 0;
        switch (arg3) {
        case 0:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[0];
            break;
        case 1:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[1];
            break;
        case 2:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[2];
            break;
        case 3:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[3];
            break;
        case 4:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[4];
            break;
        case 5:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[5];
            break;
        case 6:
            p = &((sEconRec(*)[10])cBELibrary_profileLib)[arg1][arg2].f[6];
            break;
        }
        return ((sEconTable*)D_00440550)->entries[(*p + 1) / 5].value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151368);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_00151368(void* self, int flags)
{
    void* buffer = *(void**)((char*)self + 0x288);
    if (buffer != 0)
    {
        cMemMan_free(buffer);
    }
    *(void**)((char*)self + 0x288) = 0;
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001513B8);
#ifdef SKIP_ASM
int func_0014DC40(void* self, int kind);
extern "C" void* func_0014DC50(void* self, int kind);
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" int func_00151C90(void* self, int id, int on);
extern char D_004A6750[];

struct sEconItem_1513B8
{
    short id;
    short flags;
};

struct sEconItems_1513B8
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    sEconItem_1513B8 items[0x20D];  // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;               // 0xBC0
};

struct sEconGroup_1513B8
{
    int f0;
    short id;       // 0x4
    short prize;    // 0x6
};

struct sEconDef_1513B8
{
    int f0;
    short id;       // 0x4
    char pad6[0x8];
    short price;    // 0xE
    char pad10[0x28];
};

static inline sEconItem_1513B8* Find_1513B8(sEconItems_1513B8* self, int id)
{
    int idx = self->lookup[id];
    if (idx >= 0)
        return &self->items[idx];
    return 0;
}

static inline int Price_1513B8(sEconDef_1513B8* d)
{
    short v = d->price;
    int p = v;
    if (v > 0)
        p = v * 10;
    return p;
}

extern "C" void func_001513B8(void* vself)
{
    sEconItems_1513B8* self = (sEconItems_1513B8*)vself;
    void* db = D_004A6750;
    int n = func_0014DC40(db, self->kind);
    sEconGroup_1513B8* g = (sEconGroup_1513B8*)func_0014DC50(db, self->kind);
    for (int i = 0; i < n; i++, g++)
    {
        func_00151C90(self, g->prize, 1);
    }
    int j;
    for (j = 0; j < self->count; j++)
    {
        sEconItem_1513B8* it = &self->items[j];
        if (it->flags & 0x10)
            it->flags |= 0x26;
    }
    int m = func_0014D988(db, self->kind);
    sEconDef_1513B8* d = (sEconDef_1513B8*)func_0014D998(db, self->kind);
    int changed = 0;
    for (int i = 0; i < m; i++, d++)
    {
        if (Price_1513B8(d) == 0)
        {
            Find_1513B8(self, d->id)->flags |= 2;
        }
        else if (Price_1513B8(d) == -1)
        {
            Find_1513B8(self, d->id)->flags |= 2;
            func_00151C90(self, d->id, 1);
            changed = 1;
        }
    }
    if (changed)
    {
        int k;
        for (k = 0; k < self->count; k++)
        {
            sEconItem_1513B8* it = &self->items[k];
            if (it->flags & 0x10)
                it->flags |= 6;
            else
                it->flags &= ~4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001515B8);
#ifdef SKIP_ASM
struct sEconItem_001515B8
{
    short id;
    short flags;
};

struct sEconItems_001515B8
{
    char pad[0x28C];
    int count;
    sEconItem_001515B8 items[1];
};

extern "C" void func_001515B8(sEconItems_001515B8* self)
{
    int n = self->count;
    sEconItem_001515B8* item = self->items;
    for (int i = 0; i < n; i++, item++)
    {
        if (item->flags & 4)
            item->flags |= 0x10;
        else
            item->flags &= ~0x10;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151600);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sEconBits_1600 {
    unsigned long b0 : 1;
    unsigned long b1 : 1;
    unsigned long b2 : 1;
    unsigned long b3 : 1;
    unsigned long b4 : 1;
    unsigned long b5 : 1;
    unsigned long b6 : 1;
    unsigned long b7 : 1;
    unsigned long b8 : 1;
    unsigned long b9 : 1;
    unsigned long b10 : 1;
    unsigned long b11 : 1;
    unsigned long b12 : 1;
    unsigned long b13 : 1;
    unsigned long b14 : 1;
    unsigned long b15 : 1;
    unsigned long b16 : 1;
    unsigned long b17 : 1;
    unsigned long b18 : 1;
    unsigned long b19 : 1;
};

struct sEconSlot_1600 {
    char b[8];
};

struct sEconEnt_1600 {
    signed char flag;               // 0x0
    char pad1[3];
    int data[2];                    // 0x4
};

struct sEconFlags_1600 {
    unsigned int f0 : 1;
    unsigned int f1 : 1;
    unsigned int f2 : 1;
    unsigned int f3 : 1;
    unsigned int f4 : 1;
};

struct sEconDef_1600 {
    char pad0[0x1A];
    signed char a;                  // 0x1A
    signed char b;                  // 0x1B
    char pad1C[0x8];
};

struct sEconCol_1600 {
    char b[7];
};

struct sEconColRec_1600 {
    sEconCol_1600 col;
    char pad[8];
};

struct sEcon_1600 {
    int f0;                         // 0x0
    sEconEnt_1600 ents[23][1];      // 0x4
    sEconFlags_1600 flags[0x58];    // 0x118
    char pad_0x278[0x0];
    sEconBits_1600 bits;            // 0x278
    char pad_0x280[0x0];
    short h280;                     // 0x280
    short h282;                     // 0x282
    short h284;                     // 0x284
    short h286;                     // 0x286
    char pad_0x288[0xAC4 - 0x288];
    int fAC4;                       // 0xAC4
    int fAC8;                       // 0xAC8
    int fACC;                       // 0xACC
    sEconSlot_1600 slots[26];       // 0xAD0
    char fBA0[0x18];                // 0xBA0
    char fBB8[0x8];                 // 0xBB8
    signed char kind;               // 0xBC0
    char pad_0xBC1[0xBDF - 0xBC1];
    sEconCol_1600 col;              // 0xBDF
    char fBE6[0xDE4 - 0xBE6];       // 0xBE6
    char fDE4[0x54];                // 0xDE4
    int pad_0xE38_[0];
    struct {
        int v;
        int pad;
    } fE38[25];                    // 0xE38
    char pad_0xF00_[0xF00 - 0xE38 - 25 * 8];
    unsigned int fF00;              // 0xF00
    signed char fF04;               // 0xF04
    char pad_0xF05[3];
    unsigned int fF08[2];           // 0xF08
    char pad_0xF10[0xF28 - 0xF10];
    char fF28[4];                   // 0xF28
    char pad_0xF2C[4];
    char fF30[0x50];                // 0xF30
    int fF80;                       // 0xF80
};

extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void MEM_fill(void* dst, int c, int n);
struct sEconItems_151A88;
struct sEcon_001519E0;
extern "C" void func_00151A88(sEconItems_151A88* self, int k);
extern "C" void func_001519E0(sEcon_001519E0* self, int k);
extern "C" void func_00150558(int kind, void* dst);
extern sEconDef_1600 D_0043EE10_v1600[] __asm__("cBELibrary_missionInfo");
extern sEconColRec_1600 D_005308D9[];

extern "C" void func_00151600(sEcon_1600* self, int k, int reload)
{
    if (reload) {
        func_00151A88((sEconItems_151A88*)self, k);
    }
    self->bits.b0 = 0;
    self->bits.b1 = 0;
    self->bits.b2 = 0;
    self->bits.b3 = 0;
    self->bits.b4 = 0;
    self->bits.b5 = 0;
    self->bits.b12 = 1;
    self->bits.b13 = 1;
    self->bits.b6 = 1;
    self->bits.b7 = 1;
    self->bits.b8 = 1;
    self->bits.b9 = 1;
    self->bits.b10 = 1;
    self->bits.b11 = 1;
    self->bits.b14 = 1;
    self->bits.b15 = 1;
    self->bits.b16 = 1;
    self->bits.b17 = 1;
    self->bits.b18 = 1;
    self->bits.b19 = 1;
    self->f0 = 1;
    self->h280 = 1;
    self->h284 = 1;
    self->h282 = 0;
    self->h286 = 0;
    self->fACC = 0;
    func_00416210(self->fBA0, 0, 0x18);
    sEconSlot_1600 blank;
    func_00416210(&blank, 0, 8);
    blank.b[1] = -1;
    int i;
    for (i = 0; i < 17; i++) {
        for (int j = 0; j < 26; j++) {
            self->slots[j] = blank;
        }
    }
    for (i = 0; i < 1; i++) {
        for (int j = 0; j < 23; j++) {
            func_00416210(self->ents[j][i].data, 0, 8);
            self->ents[j][i].flag = 0;
        }
    }
    for (i = 0; i < 0x58; i++) {
        sEconDef_1600* d = &D_0043EE10_v1600[i];
        self->flags[i].f0 = d->a;
        self->flags[i].f1 = 1;
        self->flags[i].f2 = 0;
        self->flags[i].f3 = 0;
        self->flags[i].f4 = d->b;
    }
    func_00416210(&self->col, 1, 7);
    self->col = D_005308D9[k].col;
    self->fAC4 = 0;
    self->fAC8 = 0;
    func_001519E0((sEcon_001519E0*)self, self->kind);
    func_00150558(self->kind, self->fBE6);
    func_00416210(self->fBB8, 0, 8);
    func_00416210(self->fDE4, 0, 0x54);
    self->fF04 = 0;
    for (int i = 0; i < 25; i++) {
        *(int*)((char*)self + 0xE38 + i * 8) = 0;
        self->fF00 &= ~(1 << i);
    }
    for (int i = 0; i < 61; i++) {
        self->fF08[i / 32] &= ~(1 << (i % 32));
    }
    MEM_fill(self->fF28, 0, 4);
    MEM_fill(self->fF30, 0, 0x50);
    self->fF80 = 0;
    *(int*)((char*)self + 0x27C) = 0x11;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001519E0);
#ifdef SKIP_ASM
struct sEconColor_001519E0
{
    signed char a;
    signed char b;
    signed char c;
};

struct sEconRange_001519E0
{
    int lo;
    int hi;
    int pad;
};

extern sEconColor_001519E0 D_004A1218[];
extern sEconRange_001519E0 D_004406FC[];

struct sEcon_001519E0
{
    char pad0[0xBC1];
    sEconColor_001519E0 colors[10];  // 0xBC1
};

extern "C" void func_001519E0(sEcon_001519E0* self, int k)
{
    sEconColor_001519E0 def = D_004A1218[0];
    for (int i = 0; i < 10; i++)
    {
        self->colors[i] = def;
        if (i == D_004406FC[k].lo)
        {
            self->colors[i].a = 0;
            self->colors[i].b = 0;
        }
        else if (i == D_004406FC[k].hi)
        {
            self->colors[i].a = 3;
            self->colors[i].b = 2;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintecon", func_00151A88);
#ifdef SKIP_ASM
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" void func_001513B8(void* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern int D_004A11E8;
extern const char str_Profile_Bolts_Map[];
extern char D_004A6750[];

struct sEconDef_151A88
{
    int f0;
    short id;           // 0x4
    char pad[0x32];
};

struct sEconItem_151A88
{
    short id;
    short flags;
};

struct sEconItems_151A88
{
    char pad[0x288];
    short* lookup;              // 0x288
    int count;                  // 0x28C
    sEconItem_151A88 items[0x20D]; // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;           // 0xBC0
};

extern "C" void func_00151A88(sEconItems_151A88* self, int k)
{
    void* db = D_004A6750;
    sEconDef_151A88* d = (sEconDef_151A88*)func_0014D998(db, k);
    int n = func_0014D988(db, k);
    self->kind = k;
    self->count = n;
    if (self->lookup == 0)
        self->lookup = (short*)operator_new_tag(D_004A11E8 * 2, str_Profile_Bolts_Map, 0, 0);
    int i;
    for (i = 0; i < D_004A11E8; i++)
        self->lookup[i] = -1;
    for (i = 0; i < n; i++, d++)
    {
        sEconItem_151A88* p = &self->items[i];
        p->flags = 0;
        int id = d->id;
        p->id = id;
        self->lookup[d->id] = i;
    }
    for (i = n; i < 0x20D; i++)
    {
        sEconItem_151A88* q = &self->items[i];
        q->id = -1;
        q->flags = 0;
    }
    func_001513B8(self);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151BD0);
#ifdef SKIP_ASM
struct sEconItem_151BD0
{
    short id;
    unsigned short flags;
};

struct sEconItems_151BD0
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    sEconItem_151BD0 items[0x20D];  // 0x290
};

static inline unsigned int clearBits_151BD0(unsigned int f, unsigned int m)
{
    return f & ~m;
}

extern "C" void func_00151BD0(sEconItems_151BD0* self)
{
    for (int i = 0; i < self->count; i++) {
        sEconItem_151BD0* item = &self->items[i];
        unsigned int oldFlags = item->flags;
        item->flags = (oldFlags & 0x20) ? (oldFlags | 0x16) : clearBits_151BD0(clearBits_151BD0(oldFlags, 0x10), 4);
    }
}
#endif
//100%
INCLUDE_ASM("be/beintecon", func_00151C20);
#ifdef SKIP_ASM
extern "C" short func_00151C20(void* self, int i)
{
    if (i >= *(int*)((char*)self + 0x28C))
        return -1;
    return *(short*)((i << 2) + (int)self + 0x290);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151C48);
#ifdef SKIP_ASM
struct sEconItem_00151C48
{
    short id;
    short flags;
};

struct sEconItems_00151C48
{
    char pad[0x288];
    short* lookup;
    int count;
    sEconItem_00151C48 items[1];
};

extern "C" void func_00151C48(sEconItems_00151C48* self, int id, int on)
{
    int idx = self->lookup[id];
    sEconItem_00151C48* item;
    if (idx >= 0)
        item = &self->items[idx];
    else
        item = 0;
    if (on)
        item->flags |= 2;
    else
        item->flags &= ~2;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151C90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_1C90() __asm__("func_0014BDB8__FPv");
extern "C" int func_0014D448(void* db, int kind, int id);
extern "C" void* func_0014D558(void* db, int kind, int id);
extern "C" int func_0014D4C8(void* db, int kind, int id);
extern "C" int func_00151EF0(void* self, int id, int on);

struct sEconItem_151C90
{
    short id;
    unsigned short flags;
};

struct sEconItems_151C90
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    sEconItem_151C90 items[0x20D];  // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;               // 0xBC0
};

struct sEconRel_151C90
{
    char pad0[0x4];
    short other;        // 0x4
    short pad6;
    char pad8[0x2C];
    int flags;          // 0x34
};

struct sEconDef_151C90
{
    char pad0[0x6];
    short rel;          // 0x6
    char pad8[0x2C];
    int flags;          // 0x34
};

struct sEconDefs_151C90
{
    char pad0[0x4];
    sEconDef_151C90* defs;  // 0x4
};

static inline sEconItem_151C90* getItem_151C90(sEconItems_151C90* self, int id)
{
    int idx = self->lookup[id];
    if (idx >= 0)
        return &self->items[idx];
    return 0;
}

static inline int isOwned_151C90(sEconItem_151C90* p)
{
    return p->flags & 0x10;
}

static inline int hasFlag_151C90(sEconDef_151C90* d, int mask)
{
    return d->flags & mask;
}

static inline sEconDef_151C90* getDef_151C90(sEconDefs_151C90* db, int i)
{
    if (i < 0)
        return 0;
    return &db->defs[i];
}

// Sets item id owned/unowned; turning on cascades turn-offs to conflicting items (def flag 1), turning off
// cascades to dependent items (owned before or def flag 8). Then applies the item's links; 0 on failure.
extern "C" int func_00151C90(void* selfp, int id, int on)
{
    sEconItems_151C90* self = (sEconItems_151C90*)selfp;
    sEconDefs_151C90* db = (sEconDefs_151C90*)func_0014BDB8_1C90();
    sEconDef_151C90* def = getDef_151C90(db, func_0014D448(db, self->kind, id));
    sEconItem_151C90* item = getItem_151C90(self, id);
    int bit1 = hasFlag_151C90(def, 1) != 0;
    int bit8 = hasFlag_151C90(def, 8) != 0;
    int wasOn = isOwned_151C90(item) != 0;
    if (on)
        item->flags |= 0x10;
    else
        item->flags &= ~0x10;
    if (on && wasOn)
        return 1;
    if (on && bit1)
    {
        int rid = def->rel;
        sEconRel_151C90* rel = (sEconRel_151C90*)func_0014D558(db, self->kind, rid);
        int n = func_0014D4C8(db, self->kind, rid);
        for (int i = 0; i < n; i++, rel++)
        {
            int sid = rel->other;
            if (sid != id)
            {
                int j = self->lookup[sid];
                if (j >= 0 && (isOwned_151C90(&self->items[j]) || (rel->flags & 8)))
                {
                    if (func_00151C90(self, sid, 0) == 0)
                        return 0;
                }
            }
        }
    }
    else if (!on && (wasOn || bit8))
    {
        sEconRel_151C90* rel = (sEconRel_151C90*)func_0014D558(db, self->kind, id);
        int n = func_0014D4C8(db, self->kind, id);
        for (int i = 0; i < n; i++, rel++)
        {
            if (func_00151C90(self, rel->other, 0) == 0)
                return 0;
        }
    }
    if ((on && !wasOn) || (!on && wasOn))
    {
        if (func_00151EF0(self, id, on) == 0)
            return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151EF0);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_1EF0() __asm__("func_0014BDB8__FPv");
struct sEconLink_151EF0;
extern "C" sEconLink_151EF0* func_0014DB40(void* db, int kind, int id, int on, int* n);
extern "C" int func_0014D448(void* db, int kind, int id);
extern "C" int func_00151C90(void* self, int id, int on);

struct sEconItem_151EF0
{
    short id;
    unsigned short flags;
};

struct sEconItems_151EF0
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    sEconItem_151EF0 items[0x20D];  // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;               // 0xBC0
};

// One dependency: when condId is set, applies only if its owned state equals cond.
struct sEconLink_151EF0
{
    char pad0[0x5];
    signed char cond;   // 0x5
    short condId;       // 0x6
    char pad8;
    signed char on;     // 0x9
    short target;       // 0xA
};

struct sEconDef_151EF0
{
    char pad0[0x34];
    int flags;          // 0x34
};

struct sEconDefs_151EF0
{
    char pad0[0x4];
    sEconDef_151EF0* defs;  // 0x4
};

static inline sEconItem_151EF0* getItem_151EF0(sEconItems_151EF0* self, int id)
{
    int idx = self->lookup[id];
    if (idx >= 0)
        return &self->items[idx];
    return 0;
}

static inline int isOwned_151EF0(sEconItem_151EF0* p)
{
    return p->flags & 0x10;
}

static inline sEconDef_151EF0* getDef_151EF0(sEconDefs_151EF0* db, int i)
{
    if (i < 0)
        return 0;
    return &db->defs[i];
}

// Sets item id owned/unowned and applies its dependency links (cascading through func_00151C90); 0 on failure.
extern "C" int func_00151EF0(void* selfp, int id, int on)
{
    sEconItems_151EF0* self = (sEconItems_151EF0*)selfp;
    sEconDefs_151EF0* db = (sEconDefs_151EF0*)func_0014BDB8_1EF0();
    int n = 0;
    sEconLink_151EF0* link = func_0014DB40(db, self->kind, id, on, &n);
    sEconItem_151EF0* item = getItem_151EF0(self, id);
    for (int i = 0; i < n; i++, link++)
    {
        if (link->target == -1)
            continue;
        if (link->condId != -1)
        {
            if ((isOwned_151EF0(getItem_151EF0(self, link->condId)) != 0) != (link->cond != 0))
                continue;
        }
        sEconItem_151EF0* targetItem = &self->items[self->lookup[link->target]];
        int has = isOwned_151EF0(targetItem) != 0;
        if (has == (link->on != 0))
        {
            if (has)
                continue;
            if (!(getDef_151EF0(db, func_0014D448(db, self->kind, link->target))->flags & 8))
                continue;
        }
        if (targetItem->id == id)
        {
            if (link->on)
                item->flags |= 0x10;
            else
                item->flags &= ~0x10;
        }
        else if (func_00151C90(self, targetItem->id, link->on != 0) == 0)
        {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001520E8);
#ifdef SKIP_ASM
void* func_0014BDB8(void* self);
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" int func_00151EF0(void* self, int id, int on);

struct sEconItem_001520E8
{
    short id;
    unsigned short flags;
};

struct sEconItems_001520E8
{
    char pad[0x288];
    short* lookup;              // 0x288
    int count;                  // 0x28C
    sEconItem_001520E8 items[1];    // 0x290
};

struct sEconDef_001520E8
{
    char pad0[0x4];
    short id;           // 0x4
    char pad6[0x2E];
    int flags;          // 0x34
};

static inline sEconItem_001520E8* getItem_001520E8(sEconItems_001520E8* self, int id)
{
    int idx = self->lookup[id];
    if (idx >= 0)
        return &self->items[idx];
    return 0;
}

extern "C" int func_001520E8(sEconItems_001520E8* self, int* changed)
{
    void* db = func_0014BDB8(self);
    int n = func_0014D988(db, *((signed char*)self + 0xBC0));
    sEconDef_001520E8* def = (sEconDef_001520E8*)func_0014D998(db, *((signed char*)self + 0xBC0));
    *changed = 0;
    for (int i = 0; i < n; i++, def++)
    {
        if (def->flags & 0x1000)
        {
            if (getItem_001520E8(self, def->id)->flags & 0x10)
            {
                *changed = 1;
                if (func_00151EF0(self, def->id, 1) == 0)
                    return 0;
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001521F0);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014DC40(void* self, int kind);
extern "C" void* func_0014DC50(void* self, int kind);
extern "C" int func_0014D608(void* self, int kind, int id, void** out, int a, int b);
extern "C" int func_00151C90(void* self, int id, int on);

struct sEconItem_1521F0
{
    short id;
    unsigned short flags;
};

struct sEconItems_1521F0
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    sEconItem_1521F0 items[0x20D];  // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;               // 0xBC0
};

struct sEconGroup_1521F0
{
    int f0;
    short id;       // 0x4
    short prize;    // 0x6
};

struct sEconDef_1521F0
{
    int f0;
    short id;       // 0x4
    char pad[0x2E];
    int flags;      // 0x34
};

static inline sEconItem_1521F0* Find_1521F0(sEconItems_1521F0* self, int id)
{
    int idx = self->lookup[id];
    if (idx >= 0)
        return &self->items[idx];
    return 0;
}

extern "C" int func_001521F0(sEconItems_1521F0* self)
{
    int i = 0;
    int ok = 1;
    void* db = func_0014BDB8_noarg();
    sEconGroup_1521F0* group = (sEconGroup_1521F0*)func_0014DC50(db, self->kind);
    int n = func_0014DC40(db, self->kind);
    for (; i < n; i++, group++)
    {
        sEconDef_1521F0* list[528];
        int cnt = func_0014D608(db, self->kind, group->id, (void**)list, -1, 1);
        int j;
        for (j = 0; j < cnt; j++)
        {
            if (!(Find_1521F0(self, list[j]->id)->flags & 0x10))
                continue;
            if (list[j]->flags & 0x10)
                break;
        }
        if (j == cnt)
        {
            if (func_00151C90(self, group->prize, 1) == 0)
            {
                ok = 0;
                break;
            }
        }
    }
    return ok;
}
#endif


//100%
INCLUDE_ASM("be/beintecon", func_00152340);
#ifdef SKIP_ASM
static inline bool testFlag_152340(unsigned int flags, unsigned int mask)
{
    return (flags & mask) != 0;
}

extern "C" bool func_00152340(void* self, int id)
{
    int idx = (*(short**)((char*)self + 0x288))[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)self + 0x292);
    return testFlag_152340(flags, 0x10);
}
#endif
//100%
INCLUDE_ASM("be/beintecon", func_00152368);
#ifdef SKIP_ASM
extern "C" bool func_00152368(void* self, int id, unsigned int mask)
{
    int idx = (*(short**)((char*)self + 0x288))[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)self + 0x292);
    return (~flags & mask) == 0;
}
#endif
