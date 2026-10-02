#include "common.h"

extern int D_005305B0[];

struct sPlayerCharEntry {
    char pad_0x00[0xc];
    int field_0xc; // 0xc
    char pad_0x10[1];
    signed char mCharID; // 0x11
    signed char field_0x12; // 0x12
    char pad_0x13[0x1C - 0x12 - 1];
};
extern sPlayerCharEntry D_00534FE0[];

extern "C" void func_00145B20(void* self);
extern "C" void cBENewPlayerInterface_defaultCtrl(void* self);
extern void* D_0045AE28[16];

struct cBENewPlayerInterfaceCtor {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_cBENewPlayerInterface__FP25cBENewPlayerInterfaceCtor);
#ifdef SKIP_ASM
cBENewPlayerInterfaceCtor* cBENewPlayerInterface_cBENewPlayerInterface(cBENewPlayerInterfaceCtor* self)
{
    self->field_0x8 = 0;
    self->vtable = D_0045AE28;
    func_00145B20(self);
    cBENewPlayerInterface_defaultCtrl(self);
    return self;
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A1D8[];
extern void* D_004A11C0;

//99.24%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBENewPlayerInterface_getThis()
{
    if (D_004A11C0 == 0) {
        void* mem = cMemMan_alloc(0x10, D_0045A1D8, 0, 0);
        D_004A11C0 = cBENewPlayerInterface_cBENewPlayerInterface((cBENewPlayerInterfaceCtor*)mem);
    }
    return D_004A11C0;
}
#endif

INCLUDE_ASM("be/beintplayer", func_001459B8);

INCLUDE_ASM("be/beintplayer", func_00145A98);

//100%
INCLUDE_ASM("be/beintplayer", func_00145B20);
#ifdef SKIP_ASM
struct sPlayerTable_00145B20
{
    sPlayerCharEntry entries[6];
};

extern sPlayerTable_00145B20 D_00535B20;

extern "C" void func_00145B20(void* self)
{
    *(sPlayerTable_00145B20*)D_00534FE0 = D_00535B20;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_defaultCtrl);
#ifdef SKIP_ASM
struct sVEntry00145BD8 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sCtrlEntry00145BD8 {
    char pad_0x00[0x10];
    unsigned int flag0 : 1;
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int rest : 29;
    char pad_0x14[0x8];
};

extern "C" void cBENewPlayerInterface_defaultCtrl(void* self)
{
    int i;
    sCtrlEntry00145BD8* e = (sCtrlEntry00145BD8*)&D_00535B20;
    for (i = 0; i < 6; i++)
    {
        ((char*)&e[i])[0x13] = 0;
        e[i].flag1 = 1;
    }
    sVEntry00145BD8* vt = *(sVEntry00145BD8**)((char*)self + 0xC);
    vt[2].fn((char*)self + vt[2].delta);
}
#endif

INCLUDE_ASM("be/beintplayer", func_00145C38);

INCLUDE_ASM("be/beintplayer", func_00145CB0);

INCLUDE_ASM("be/beintplayer", func_00145D38);

INCLUDE_ASM("be/beintplayer", func_00145DD0);

INCLUDE_ASM("be/beintplayer", func_00145E68);

INCLUDE_ASM("be/beintplayer", func_00145EF0);

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isPeakLocked);
#ifdef SKIP_ASM
extern int D_004A6CA8[];

// PORT: 64-bit ulong flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
extern "C" int cBENewPlayerInterface_isPeakLocked(void* self, int a, int b, int peak)
{
    if (peak != 0)
    {
        if (peak == 1)
        {
            int bOff = b * 0xF88;
            int aOff = a * 0x9B50;
            int off = bOff + aOff;
            char* p = (char*)D_004A6CA8 + off;
            return (int)(*(ulong*)(p + 0x278) >> 12) & 1;
        }
        else
        {
            int bOff = b * 0xF88;
            int aOff = a * 0x9B50;
            int off = bOff + aOff;
            char* p = (char*)D_004A6CA8 + off;
            return (int)(*(ulong*)(p + 0x278) >> 13) & 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isPeakLocked1);

INCLUDE_ASM("be/beintplayer", func_00146150);

INCLUDE_ASM("be/beintplayer", func_00146320);

INCLUDE_ASM("be/beintplayer", func_001464D0);

INCLUDE_ASM("be/beintplayer", func_00146A70);

INCLUDE_ASM("be/beintplayer", func_00146D98);

INCLUDE_ASM("be/beintplayer", func_00146E10);

INCLUDE_ASM("be/beintplayer", func_00146E98);

INCLUDE_ASM("be/beintplayer", func_00146F88);

//100%
INCLUDE_ASM("be/beintplayer", func_00147138);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);

extern "C" void func_00147138(void* self, int a1, const char* name)
{
    strncpy(D_00534FE0[a1].pad_0x00, name, 0xC);
}
#endif

INCLUDE_ASM("be/beintplayer", func_00147170);

INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCtrlID);

//100%
INCLUDE_ASM("be/beintplayer", func_00147290);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index);

extern "C" int func_00147290(void* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return D_00534FE0[id].field_0xc;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001472C8);
#ifdef SKIP_ASM
struct sPlayerCharFlags_001472C8 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int rest : 29;
};

extern "C" void func_001472C8(void* self, int a1, int a2)
{
    D_00534FE0[a1].field_0xc = a2;
    if (a2 >= 0) {
        char* p = (char*)&D_00534FE0[0] + a1 * 0x1c;
        sPlayerCharFlags_001472C8* f = (sPlayerCharFlags_001472C8*)(p + 0x10);
        f->bit0 = a1;
        f->bit2 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147318);
#ifdef SKIP_ASM
extern "C" int func_00147318(void* self, int a1)
{
    return D_00534FE0[a1].field_0xc;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCharID);
#ifdef SKIP_ASM
static inline int func_00147338_isLocked(sPlayerCharEntry* e)
{
    return (*(unsigned int*)((char*)e + 0x10) >> 2) & 1;
}

extern "C" void cBENewPlayerInterface_setRiderCharID(void* self, int index, int charID)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* e = &D_00534FE0[id];
    if (func_00147338_isLocked(e) == 1)
    {
        e->mCharID = D_00534FE0[0].mCharID;
    }
    else
    {
        e->mCharID = charID;
    }
}
#endif

int cBENewPlayerInterface_getPlayerID(int index);

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderCharID__FPvi);
#ifdef SKIP_ASM
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex)
{
    int playerID = cBENewPlayerInterface_getPlayerID(riderIndex);
    sPlayerCharEntry* entry = &D_00534FE0[playerID];
    return entry->mCharID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001473D0);
#ifdef SKIP_ASM
extern "C" void func_001473D0(void* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    D_00534FE0[id].field_0x12 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147410);
#ifdef SKIP_ASM
extern "C" signed char func_00147410(void* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return D_00534FE0[id].field_0x12;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147448);
#ifdef SKIP_ASM
struct sPlayerCharFlags_00147448 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int rest : 29;
};

static inline int func_00147448_getBit2(char* p)
{
    return ((sPlayerCharFlags_00147448*)(p + 0x10))->bit2;
}

extern "C" void func_00147448(void* self, int a1, int charID)
{
    char* p = (char*)&D_00534FE0[0] + a1 * 0x1c;
    if (func_00147448_getBit2(p) == 1) {
        *(signed char*)(p + 0x11) = D_00534FE0[0].mCharID;
    } else {
        *(signed char*)(p + 0x11) = charID;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCharID__FPvi);
#ifdef SKIP_ASM
signed char cBENewPlayerInterface_getPlayerCharID(void* self, int index)
{
    index *= sizeof(sPlayerCharEntry);
    return ((sPlayerCharEntry*)((char*)D_00534FE0 + index))->mCharID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001474A8);
#ifdef SKIP_ASM
extern "C" void func_001474A8(void* self, int a1, int a2)
{
    char* p = (char*)&D_00534FE0[0] + a1 * 0x1c;
    *(char*)(p + 0x12) = a2;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001474C8);
#ifdef SKIP_ASM
extern "C" signed char func_001474C8(void* self, int a1)
{
    char* p = (char*)&D_00534FE0[0] + a1 * 0x1c;
    return *(signed char*)(p + 0x12);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001474E8);
#ifdef SKIP_ASM
struct sPlayerCharFlags_0x10 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

extern "C" int func_001474E8(void* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    char* p = (char*)&D_00534FE0[0] + id * 0x1c;
    return ((sPlayerCharFlags_0x10*)(p + 0x10))->bit1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147528);
#ifdef SKIP_ASM
struct sPlayerCharFlags_00147528 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

extern "C" void func_00147528(void* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* e = &D_00534FE0[id];
    ((sPlayerCharFlags_00147528*)((char*)e + 0x10))->bit1 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147580);
#ifdef SKIP_ASM
extern "C" int func_00147580(void* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    char* p = (char*)&D_00534FE0[0] + id * 0x1c;
    return ((sPlayerCharFlags_0x10*)(p + 0x10))->bit3;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001475C0);
#ifdef SKIP_ASM
struct sPlayerCharFlags_001475C0 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

extern "C" void func_001475C0(void* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* e = &D_00534FE0[id];
    ((sPlayerCharFlags_001475C0*)((char*)e + 0x10))->bit3 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147618);
#ifdef SKIP_ASM
extern "C" signed char func_00147618(void* self, int index)
{
    return D_00534FE0[cBENewPlayerInterface_getPlayerID(index)].pad_0x13[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147658);
#ifdef SKIP_ASM
extern "C" void func_00147658(void* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    D_00534FE0[id].pad_0x13[0] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerID__Fi);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index)
{
    return D_005305B0[index];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isMissionMan__Fi);
#ifdef SKIP_ASM
struct sPlayerFlags_001477E8
{
    char pad_0x00[0x10];
    unsigned int flag0 : 1;       // 0x10 bit 0
    unsigned int flag1 : 1;       // 0x10 bit 1
    unsigned int missionMan : 1;  // 0x10 bit 2
    unsigned int rest : 29;
    char pad_0x14[0x1C - 0x14];
};

int cBENewPlayerInterface_isMissionMan(int index)
{
    int playerID = cBENewPlayerInterface_getPlayerID(index);
    return ((sPlayerFlags_001477E8*)D_00534FE0)[playerID].missionMan;
}
#endif

INCLUDE_ASM("be/beintplayer", func_00147828);

INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_resetFromMissionMan);

INCLUDE_ASM("be/beintplayer", func_00147908);

INCLUDE_ASM("be/beintplayer", func_00147980);

INCLUDE_ASM("be/beintplayer", func_00147A30);

//100%
INCLUDE_ASM("be/beintplayer", func_00147CB8);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);

extern "C" short func_00147CB8(void* self, int which)
{
    int c = cBELibrary_getCharacterID(0);
    if (which != 0)
    {
        char* p = (char*)D_004A6CA8 + c * 0xF88;
        return *(short*)(p + 0x280);
    }
    else
    {
        char* p = (char*)D_004A6CA8 + c * 0xF88;
        return *(short*)(p + 0x284);
    }
}
#endif

INCLUDE_ASM("be/beintplayer", func_00147D20);

INCLUDE_ASM("be/beintplayer", func_00147E18);

INCLUDE_ASM("be/beintplayer", func_00147F78);

//100%
INCLUDE_ASM("be/beintplayer", func_00147FD8);
#ifdef SKIP_ASM
struct sCtrl7_00147FD8
{
    signed char b[7];
};

struct sPlayerSlot_00147FD8
{
    sCtrl7_00147FD8 ctrl;
    char pad[0xF88 - 7];
};

struct sPlayerStatRow;
extern sPlayerStatRow D_00535538[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

extern "C" void func_00147FD8(void)
{
    for (int i = 0; i < 10; i++)
    {
        D_004A7887[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[0][i];
        D_004B13D7[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[1][i];
        D_004BAF27[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[2][i];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148098);
#ifdef SKIP_ASM
struct sPlayerStatRow;
extern sPlayerStatRow D_00535538[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

extern "C" void func_00148098(void)
{
    for (int i = 0; i < 10; i++)
    {
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[0][i] = D_004A7887[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[1][i] = D_004B13D7[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[2][i] = D_004BAF27[i].ctrl;
    }
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148158);

INCLUDE_ASM("be/beintplayer", func_001483A0);

//100%
INCLUDE_ASM("be/beintplayer", func_00148410);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sPlayerStat7View_00148410 {
    signed char v[7];
};
struct sPlayerStatRowView_00148410 {
    sPlayerStat7View_00148410 e[10];
};
extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_00148410(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[0];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148470);

//100%
INCLUDE_ASM("be/beintplayer", func_001484E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_001484E0(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[1];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148540);

//100%
INCLUDE_ASM("be/beintplayer", func_001485B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_001485B0(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[2];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148610);

//100%
INCLUDE_ASM("be/beintplayer", func_00148680);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_00148680(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[3];
}
#endif

INCLUDE_ASM("be/beintplayer", func_001486E0);

//100%
INCLUDE_ASM("be/beintplayer", func_00148750);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_00148750(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[4];
}
#endif

INCLUDE_ASM("be/beintplayer", func_001487B0);

//100%
INCLUDE_ASM("be/beintplayer", func_00148820);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_00148820(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[5];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148880);

//100%
INCLUDE_ASM("be/beintplayer", func_001488F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

extern "C" signed char func_001488F0(void* self, int rider)
{
    int a = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    return D_00535538_view[a].e[c].v[6];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148950);

INCLUDE_ASM("be/beintplayer", func_00148AA8);

//100%
INCLUDE_ASM("be/beintplayer", func_00148B78);
#ifdef SKIP_ASM
struct sPlayerStat7 {
    signed char v[7];
};
struct sPlayerStatRow {
    sPlayerStat7 e[10]; // 0x46 bytes
};
extern sPlayerStatRow D_00535538[];

extern "C" signed char func_00148B78(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148BC8);
#ifdef SKIP_ASM
extern "C" signed char func_00148BC8(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148C18);
#ifdef SKIP_ASM
extern "C" signed char func_00148C18(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[2];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148C68);
#ifdef SKIP_ASM
extern "C" signed char func_00148C68(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[3];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148CB8);
#ifdef SKIP_ASM
extern "C" signed char func_00148CB8(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[4];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148D08);
#ifdef SKIP_ASM
extern "C" signed char func_00148D08(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[5];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00148D58);
#ifdef SKIP_ASM
extern "C" signed char func_00148D58(void* self, int a1, int a2)
{
    return D_00535538[a1].e[a2].v[6];
}
#endif

INCLUDE_ASM("be/beintplayer", func_00148D80);

INCLUDE_ASM("be/beintplayer", func_00148E68);

