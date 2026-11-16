#include "common.h"
#include "be/beintnetwork.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A648[];
extern void* D_0045AD98[16];

//99.58%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's network interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BENetworkInt", clears bIsActive and stores the vtable.
 *
 * @return the network interface
 */
void* cBENetworkInterface_getThis()
{
    if (cBENetworkInterface_instance == 0) {
        cBENetworkInterface* mem = (cBENetworkInterface*)cMemMan_alloc(0x10, D_0045A648, 0, 0);
        mem->bIsActive = 0;
        mem->mVtbl = D_0045AD98;
        cBENetworkInterface_instance = mem;
    }
    return cBENetworkInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E130);
#ifdef SKIP_ASM
// The submitted copy of the network options (discard restores it).
extern tBENetworkDB D_00535088;

extern "C" void func_0014E130(void)
{
    D_00535088 = cBENetworkInterface_networkOpt;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", func_0014E1F8);
#ifdef SKIP_ASM
extern tBENetworkDB D_00535088;

extern "C" void func_0014E1F8(void)
{
    cBENetworkInterface_networkOpt = D_00535088;
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

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getSyncData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sNetHdr_E2C8 {
    int w[0x4C / 4];
};

struct sNetPair_E2C8 {
    int w[2];
};

struct sNetTrip_E2C8 {
    int w[3];
};

struct sNetHW_E2C8 {
    short h[2];
};

struct sNetDW_E2C8 {
    int w[2];
};

struct sNetTag_E2C8 {
    char c[7];
};

struct sNetKey_E2C8 {
    short h[3];
};

struct sCharE_E2C8 {
    char pad_0x00[0x11];
    signed char charID;     // 0x11
    char pad_0x12[0x2];
    int w[2];
};

struct sNetState_E2C8 {
    sNetHdr_E2C8 hdr;                   // 0x000
    sCharE_E2C8 me;             // 0x04C
    sCharE_E2C8 chars[6];       // 0x068
    signed char charID;                 // 0x110
    sNetTag_E2C8 tag;                   // 0x111
    sNetKey_E2C8 keys[0x55];            // 0x118
    sNetHW_E2C8 blob[0x20D];            // 0x316
    char pad_0xB4A[2];
    sNetDW_E2C8 moves[0x1A];            // 0xB4C
    sNetTrip_E2C8 t0;                   // 0xC1C
    sNetTrip_E2C8 t1;                   // 0xC28
    sNetPair_E2C8 pair;                 // 0xC34
    int a;                              // 0xC3C
    int b;                              // 0xC40
};

extern sCharE_E2C8 D_00535B20_E2C8[] __asm__("cBELibrary_playerDB");
extern int D_004A11B8;
extern int D_004A11BC;
extern sNetHdr_E2C8 D_005305B0;
extern char D_005308D0[];
extern char D_004A6F38[];
extern char D_004A7778[];
extern char D_004A7887[];

extern "C" void cBENetworkInterface_getSyncData(cBENetworkInterface* self, int p, sNetState_E2C8* out)
{
    out->a = D_004A11B8;
    out->b = D_004A11BC;
    out->hdr = D_005305B0;
    for (int i = 0; i < 6; i++) {
        out->chars[i] = D_00535B20_E2C8[i];
    }
    out->me = D_00535B20_E2C8[p];
    signed char c = D_00535B20_E2C8[p].charID;
    out->charID = c;
    out->pair = *(sNetPair_E2C8*)D_005308D0;
    out->t0 = *(sNetTrip_E2C8*)(D_005308D0 - 0x18);
    out->t1 = *(sNetTrip_E2C8*)(D_005308D0 - 0xC);
    for (int k = 0; k < 0x20D; k++) {
        out->blob[k] = ((sNetHW_E2C8*)(D_004A6F38 + (c * 0xF88 + p * 0x9B50)))[k];
    }
    for (int k = 0; k < 0x1A; k++) {
        out->moves[k] = ((sNetDW_E2C8*)(D_004A7778 + (c * 0xF88 + p * 0x9B50)))[k];
    }
    out->tag = *(sNetTag_E2C8*)(D_004A7887 + (c * 0xF88 + p * 0x9B50));
    for (int k = 0; k < 0x55; k++) {
        out->keys[k] = ((sNetKey_E2C8*)(D_004A7887 + 7 + (c * 0xF88 + p * 0x9B50)))[k];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_setSyncData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00151600(void* p, int i, int arg);
extern "C" void func_00156810(void* self);
extern sNetTrip_E2C8 D_005308B8_E5C8[] __asm__("D_005308B8");

struct sProfChar_E5C8 {
    char d[0xF88];
};

extern sProfChar_E5C8 D_004A6CA8_E5C8[][10] __asm__("cBELibrary_profileLib");

class cBEIface_E5C8 {
public:
    int pad[3];
    virtual void v01();
    virtual void v02();
};

extern "C" void cBENetworkInterface_setSyncData(cBENetworkInterface* self, int player, sNetState_E2C8* in)
{
    D_004A11B8 = in->a;
    D_004A11BC = in->b;
    D_005305B0 = in->hdr;
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 0))->v02();
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
    {
        for (int i = 2; i < 6; i++)
            D_00535B20_E2C8[i] = in->chars[i];
    }
    D_00535B20_E2C8[player] = in->me;
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 1))->v02();
    signed char c = in->charID;
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
        *(sNetPair_E2C8*)D_005308D0 = in->pair;
    D_005308B8_E5C8[player] = (&in->t0)[player];
    func_00151600(&D_004A6CA8_E5C8[player][c], c, 1);
    func_00156810(D_004A6CA8_E5C8[player]);
    for (int k = 0; k < 0x20D; k++)
        ((sNetHW_E2C8*)((char*)D_004A6CA8_E5C8 + 0x290 + (c * 0xF88 + player * 0x9B50)))[k] = in->blob[k];
    for (int k = 0; k < 0x1A; k++)
        ((sNetDW_E2C8*)(D_004A7778 + (c * 0xF88 + player * 0x9B50)))[k] = in->moves[k];
    *(sNetTag_E2C8*)(D_004A7887 + (c * 0xF88 + player * 0x9B50)) = in->tag;
    for (int k = 0; k < 0x55; k++)
        ((sNetKey_E2C8*)(D_004A7887 + 7 + (c * 0xF88 + player * 0x9B50)))[k] = in->keys[k];
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 6))->v02();
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_copyProfile);
#ifdef SKIP_ASM
struct sBEPlayerData {
    char data[0x9B50];
};
extern sBEPlayerData cBELibrary_profileLib[];

extern "C" void func_00156858(sBEPlayerData* dst, sBEPlayerData* src);

extern "C" void cBENetworkInterface_copyProfile(cBENetworkInterface* self, int a1, int a2)
{
    sBEPlayerData* src = &cBELibrary_profileLib[a1];
    func_00156858(&cBELibrary_profileLib[a2], src);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_preInitBE);
#ifdef SKIP_ASM
extern "C" void func_001567B8(void* self, int arg);
// PORT: func_0014EB08 is defined (void) in this unit but called with self here.
void func_0014EB08_self(void* self) __asm__("func_0014EB08");

struct sRiderEntry_0014E9F8 {
    int w[7];
};

extern char D_004BA348[];
extern int D_00534B38[];
extern sRiderEntry_0014E9F8 D_00535B3C;

extern "C" void cBENetworkInterface_preInitBE(cBENetworkInterface* self)
{
    func_001567B8(D_004BA348, 0);
    if (D_00534B38[0] == 0)
    {
        sRiderEntry_0014E9F8* e = &D_00535B3C;
        e[0] = e[-1];
        cBENetworkInterface_copyProfile(self, 0, 1);
    }
    func_0014EB08_self(self);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_postInitBE);
#ifdef SKIP_ASM
struct sVEntry0014EA90 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sBEIface0014EA90 {
    char pad_0x00[0xC];
    sVEntry0014EA90* vtable; // 0xC
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014EB08 is defined (void) in this unit but called with self here.
void func_0014EB08_self(void* self) __asm__("func_0014EB08");

extern "C" void cBENetworkInterface_postInitBE(cBENetworkInterface* self)
{
    sBEIface0014EA90* a = (sBEIface0014EA90*)cBE_getInterface_Fv(cBE_getBE(), 3);
    a->vtable[2].fn((char*)a + a->vtable[2].delta);
    sBEIface0014EA90* b = (sBEIface0014EA90*)cBE_getInterface_Fv(cBE_getBE(), 0xC);
    b->vtable[2].fn((char*)b + b->vtable[2].delta);
    func_0014EB08_self(self);
}
#endif

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
extern "C" void func_0014EBB8(cBENetworkInterface* self, const char* src)
{
    strcpy(D_00534FC8, src);
}
#endif

extern char D_00534FB8[16];

//100%
INCLUDE_ASM("be/beintnetwork", func_0014EBD8);
#ifdef SKIP_ASM
extern "C" void func_0014EBD8(cBENetworkInterface* self, const char* src)
{
    strcpy(D_00534FB8, src);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_backupCharProfile);
#ifdef SKIP_ASM
struct sCharEntry_0014EBF8
{
    char pad_0x00[0x11];
    signed char charID; // 0x11
    char pad_0x12[0x2];
    int w[2];
};

extern sCharEntry_0014EBF8 cBELibrary_playerDB[];
extern sCharEntry_0014EBF8 D_00534A88[];
extern "C" void func_00156950(sBEPlayerData* data, int charID);

extern "C" void cBENetworkInterface_backupCharProfile(cBENetworkInterface* self, int idx)
{
    func_00156950(&cBELibrary_profileLib[idx], cBELibrary_playerDB[idx].charID);
    for (int i = 0; i < 6; i++)
    {
        D_00534A88[i] = cBELibrary_playerDB[i];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_restoreCharProfile);
#ifdef SKIP_ASM
extern "C" void func_00156988(sBEPlayerData* data);

extern "C" void cBENetworkInterface_restoreCharProfile(cBENetworkInterface* self, int idx)
{
    func_00156988(&cBELibrary_profileLib[idx]);
    if (D_00534B38[0] == 0)
    {
        cBELibrary_playerDB[0] = cBELibrary_playerDB[1];
    }
    for (int i = 1; i < 6; i++)
    {
        cBELibrary_playerDB[i] = D_00534A88[i];
    }
    sBEIface0014EA90* a = (sBEIface0014EA90*)cBE_getInterface_Fv(cBE_getBE(), 1);
    a->vtable[2].fn((char*)a + a->vtable[2].delta);
    sBEIface0014EA90* b = (sBEIface0014EA90*)cBE_getInterface_Fv(cBE_getBE(), 6);
    b->vtable[2].fn((char*)b + b->vtable[2].delta);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getThis);
#ifdef SKIP_ASM
extern const char D_0045A280[];
extern void* D_0045AD38[16];
extern void* D_004A1208;

// cBECharacterInterface is another 0x10-byte cBEInterface (three 4-byte bools, then the vptr; see be/beintreward.h).
struct cBECharacterInterface {
    int bCanSubmit;
    int bDataChanged;
    int bIsActive;
    void** mVtbl;
};

/**
 * Returns the back end's character interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BECharInt", clears bIsActive and stores the vtable.
 *
 * @return the character interface
 */
extern "C" void* cBECharacterInterface_getThis(void)
{
    if (D_004A1208 == 0) {
        cBECharacterInterface* mem = (cBECharacterInterface*)cMemMan_alloc(0x10, D_0045A280, 0, 0);
        mem->bIsActive = 0;
        mem->mVtbl = D_0045AD38;
        D_004A1208 = mem;
    }
    return D_004A1208;
}
#endif

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

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getShortName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBELibrary_getCharacterID(int index);
extern "C" signed char cBELibrary_getCheatCharacterID(int index);

struct sNetEntry8_0014EE58
{
    char data[8];
};

struct sNetEntry88_0014EE58
{
    char data[0x88];
};

extern sNetEntry8_0014EE58 D_0043FA38_0014EE58[] __asm__("D_0043FA38");
extern sNetEntry88_0014EE58 D_00530990_0014EE58[] __asm__("D_00530990");

/**
 * Returns a rider's short character name.
 *
 * A cheat character (cheat index 10 to 29) gets its 8-byte name from the table based at D_0043FA38 (index 10 is
 * "Brodi"); any other rider gets the address of its character's 0x88-byte record in D_00530990 (probably On
 * Tour's cBELibrary::characterDB), which presumably starts with the name.
 *
 * @param rider  the rider whose character is used
 * @return the name
 */
extern "C" void* cBECharacterInterface_getShortName(void* self, int rider)
{
    int c = cBELibrary_getCharacterID(rider);
    int index = cBELibrary_getCheatCharacterID(rider);
    if (index >= 10 && index < 30) {
        return &D_0043FA38_0014EE58[index];
    }
    return &D_00530990_0014EE58[c];
}
#endif

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


//100%
INCLUDE_ASM("be/beintnetwork", func_0014EF00);
#ifdef SKIP_ASM
signed char cBELibrary_getCharacterID(int index);

struct sNetChar_0014EF00
{
    char data[0x88];
};

extern sNetChar_0014EF00 D_005309A0[];

extern "C" void* func_0014EF00(void* self, int rider)
{
    return &D_005309A0[cBELibrary_getCharacterID(rider)];
}
#endif
