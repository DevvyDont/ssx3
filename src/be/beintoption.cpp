#include "common.h"
#include "main/ssxapp.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cBEOptionInterface_revert(void* self);
extern const char str_BEOptionInt[];
extern void* D_0045ADC8[16];
extern void* cBEOptionInterface_instance;

struct cBEOptionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's option interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEOptionInt", stores the vtable, clears the word at +0x8 and runs
 * cBEOptionInterface_revert(), which calls four setup helpers on it (probably resetting the options to their defaults).
 *
 * @return the option interface
 */
void* cBEOptionInterface_getThis()
{
    if (cBEOptionInterface_instance == 0) {
        cBEOptionInterface* mem = (cBEOptionInterface*)cMemMan_alloc(0x10, str_BEOptionInt, 0, 0);
        mem->vtable = D_0045ADC8;
        cBEOptionInterface_instance = mem;
        mem->field_0x8 = 0;
        cBEOptionInterface_revert(mem);
    }
    return cBEOptionInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F2A8);
#ifdef SKIP_ASM
struct sOptions_0014F2A8
{
    int data[0x288 / 4];
};

extern sOptions_0014F2A8 cBEOptionInterface_mOption;
extern sOptions_0014F2A8 D_00535898;

/**
 * Commits the options: copies cBEOptionInterface_mOption (0x288 bytes) into the saved copy D_00535898.
 */
extern "C" void func_0014F2A8(void)
{
    D_00535898 = cBEOptionInterface_mOption;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F360);
#ifdef SKIP_ASM
extern sOptions_0014F2A8 cBEOptionInterface_mOption;
extern sOptions_0014F2A8 D_00535898;

/**
 * Drops option changes: copies the saved copy D_00535898 back into cBEOptionInterface_mOption (0x288 bytes).
 */
extern "C" void func_0014F360(void)
{
    cBEOptionInterface_mOption = D_00535898;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_revert);
#ifdef SKIP_ASM
extern "C" void func_0014F648(void* self);
extern "C" void func_0014F600(void* self);
extern "C" void func_0014F4F8(void* self);
extern "C" void func_0014F458(void* self);

/**
 * Resets all game options to their defaults.
 *
 * Calls func_0014F648, func_0014F600, func_0014F4F8 and func_0014F458 (On Tour defaultCtrl, defaultHUD, defaultGame,
 * defaultSound).
 */
extern "C" void cBEOptionInterface_revert(void* self)
{
    func_0014F648(self);
    func_0014F600(self);
    func_0014F4F8(self);
    func_0014F458(self);
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F458);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sOptBits_0014F458
{
    unsigned int a : 4;
    unsigned int b : 4;
    unsigned int c : 4;
    unsigned int d : 2;
    unsigned int e : 2;
    unsigned int f : 1;
    unsigned int g : 1;
    unsigned int h : 1;
    unsigned int rest0 : 13;
    unsigned int w1a : 2;
    unsigned int rest1 : 30;
};
extern sOptBits_0014F458 D_00535610_v0014F458[] __asm__("cBEOptionInterface_mOption");

class cBEOptionInterfaceV_0014F458
{
public:
    char pad_0x00[0xC];
    virtual void apply();
};

/**
 * Resets the sound options and submits them.
 *
 * In the first option word: music, speech and effects volumes 10, sound mode 0, audio mix 0, arcade effects, DJ speech
 * and voice chat on; the low two bits of the second word become 1. Then calls the option interface's vtable slot 1
 * (func_0014F2A8).
 */
extern "C" void func_0014F458(void* self)
{
    sOptBits_0014F458* o = D_00535610_v0014F458;
    o->a = 10;
    o->b = 10;
    o->c = 10;
    o->d = 0;
    o->e = 0;
    o->f = 1;
    o->g = 1;
    o->h = 1;
    o->w1a = 1;
    ((cBEOptionInterfaceV_0014F458*)cBEOptionInterface_instance)->apply();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F4F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

struct sOptBits_0014F4F8
{
    unsigned int pad0 : 19;
    unsigned int i : 1;     // 19
    unsigned int j : 2;     // 20-21
    unsigned int k : 3;     // 22-24
    unsigned int l : 4;     // 25-28
    unsigned int m : 1;     // 29
    unsigned int top : 2;   // 30-31
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
};
extern sOptBits_0014F4F8 D_00535610_v0014F4F8[] __asm__("cBEOptionInterface_mOption");

class cBEOptionInterfaceV_0014F4F8
{
public:
    char pad_0x00[0xC];
    virtual void apply();
};

/**
 * Resets the game options and submits them.
 *
 * Widescreen mode follows the console configuration (SSXApp->ConsoleConfig +0x24: 1 gives 2, 2 gives 1, else 0);
 * language, metric units and the replay option become 0, brightness 9, and the screen offsets (mOption +5, +6) 0. Then
 * calls the option interface's vtable slot 1 (func_0014F2A8).
 */
extern "C" void func_0014F4F8(void* self)
{
    switch (*(int*)((char*)SSXApp->ConsoleConfig + 0x24))
    {
    case 1:
        D_00535610_v0014F4F8[0].j = 2;
        break;
    case 2:
        D_00535610_v0014F4F8[0].j = 1;
        break;
    case 0:
    default:
        D_00535610_v0014F4F8[0].j = 0;
        break;
    }
    D_00535610_v0014F4F8[0].k = 0;
    D_00535610_v0014F4F8[0].i = 0;
    D_00535610_v0014F4F8[0].m = 0;
    D_00535610_v0014F4F8[0].l = 9;
    D_00535610_v0014F4F8[0].b5 = 0;
    D_00535610_v0014F4F8[0].b6 = 0;
    ((cBEOptionInterfaceV_0014F4F8*)cBEOptionInterface_instance)->apply();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F600);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sOptBits_0014F600
{
    unsigned int pad : 30;
    unsigned int top : 2;
};
extern sOptBits_0014F600 D_00535610_v0014F600[] __asm__("cBEOptionInterface_mOption");

class cBEOptionInterfaceV_0014F600
{
public:
    char pad_0x00[0xC];
    virtual void apply();
};

/**
 * Resets the HUD option (bits 30-31 of the first option word) to 0 and submits (vtable slot 1, func_0014F2A8).
 */
extern "C" void func_0014F600(void* self)
{
    D_00535610_v0014F600[0].top = 0;
    ((cBEOptionInterfaceV_0014F600*)cBEOptionInterface_instance)->apply();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F648);
#ifdef SKIP_ASM
class cBEOptionInterfaceV_0014F648
{
public:
    char pad_0x00[0xC];
    virtual void apply();
};

/**
 * Submits the options (the option interface's vtable slot 1, func_0014F2A8) without changing any.
 */
extern "C" void func_0014F648(void* self)
{
    ((cBEOptionInterfaceV_0014F648*)cBEOptionInterface_instance)->apply();
}
#endif

extern "C" int sprintf(char* buf, const char* fmt, ...);
int GetHashValue32(char* str);
extern const char str_kT_206QuickMessagepct_d[];

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_getDefaultQuickKeyMessageHashValue__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the hash of a quick message's default text key, "kT_206QuickMessage<n>".
 *
 * @param value  the quick message number put into the key
 * @return GetHashValue32() of the key
 */
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int value)
{
    char buf[24];
    sprintf(buf, str_kT_206QuickMessagepct_d, value);
    return GetHashValue32(buf);
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F6A8);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);
extern char D_00535617[][0x40];

/**
 * Returns the player's quick-chat message @p i (the 64-byte entry D_00535617[@p i], cBEOptionInterface_mOption +7), or
 * null when it is empty.
 *
 * @param i  the message slot, 0-9
 * @return the message, or 0
 */
extern "C" char* func_0014F6A8(void* self, int i)
{
    char* s = D_00535617[i];
    if (strlen(s) == 0) {
        return 0;
    }
    return s;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F6E8);
#ifdef SKIP_ASM
extern "C" void USTR_strcpyW(char* dst, void* src);

struct sVEntry_0014F6E8
{
    short delta;
    short index;
    void* (*fn)(void*, int);
};

/**
 * Restores quick-chat message @p i to its default: looks up cBEOptionInterface_getDefaultQuickKeyMessageHashValue(@p i)
 * in the front-end locale (SSXApp->FELocale's vtable slot 4) and copies the text into D_00535617[@p i] with
 * USTR_strcpyW.
 *
 * @param i  the message slot, 0-9
 */
extern "C" void func_0014F6E8(void* self, int i)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_0014F6E8* vt = *(sVEntry_0014F6E8**)(obj + 0x4);
    char* o2 = obj + vt[4].delta;
    int h = cBEOptionInterface_getDefaultQuickKeyMessageHashValue(self, i);
    void* text = vt[4].fn(o2, h);
    USTR_strcpyW(D_00535617[i], text);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintoption", func_0014F758);
#ifdef SKIP_ASM
extern "C" void func_0014F6E8(void* self, int i);

/**
 * Restores all ten quick-chat messages to their defaults (func_0014F6E8 for slots 0-9).
 */
extern "C" void func_0014F758(void* self)
{
    for (int i = 0; i < 10; i++)
    {
        func_0014F6E8(self, i);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F7A8);
#ifdef SKIP_ASM
struct sOptionTripleView_0014F7A8 {
    int a;
    int b;
    int c;
};

struct sOptionTriplesView_0014F7A8 {
    sOptionTripleView_0014F7A8 triples[2]; // 0x0
};

struct sOptionGlobal_005308B8;
extern sOptionGlobal_005308B8 D_005308B8;
extern "C" int func_0014F870();

/**
 * Returns how long player @p i is held at the start: the largest head start (func_0014F870) minus the player's own
 * (word +8 of record @p i of the rules block D_005308B8).
 *
 * @param i  the player, 0 or 1
 * @return the delay in seconds
 */
extern "C" int func_0014F7A8(int i)
{
    int c = ((sOptionTriplesView_0014F7A8*)&D_005308B8)->triples[i].c;
    return func_0014F870() - c;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F7E8);
#ifdef SKIP_ASM
extern "C" int func_0014F870();
extern int D_005308D4[]; // D_005308B8 + 0x1C, addressed through its own symbol

/**
 * Returns how long the AI riders are held at the start: the largest head start (func_0014F870) minus the AI head start
 * (D_005308D4, the rules block's word at +0x1C).
 *
 * @return the delay in seconds
 */
extern "C" int func_0014F7E8(void* self)
{
    return func_0014F870() - D_005308D4[0];
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F810);
#ifdef SKIP_ASM
// View of D_005308B8 (the unit defines sOptionGlobal_005308B8 further down).
struct sOptionTriple_005308B8 {
    int a;
    int b;
    int c;
};

struct sOptionView_005308B8 {
    sOptionTriple_005308B8 triples[2]; // 0x0
    int field_0x18;
    int field_0x1C;
};

struct sOptionGlobal_005308B8;
extern sOptionGlobal_005308B8 D_005308B8;

/**
 * Tells whether any rule is set for player @p i: a nonzero word in record @p i of the rules block D_005308B8 or in the
 * "my rules" pair (+0x18, +0x1C).
 *
 * @param i  the player, 0 or 1
 * @return 1 when any is set, else 0
 */
extern "C" int func_0014F810(int i)
{
    sOptionView_005308B8* g = (sOptionView_005308B8*)&D_005308B8;
    if (g->triples[i].a != 0 || g->triples[i].b != 0 || g->triples[i].c != 0 || g->field_0x18 != 0 || g->field_0x1C != 0)
    {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F870);
#ifdef SKIP_ASM
struct sOptionGlobal_005308B8 {
    char pad_0x00[0x8];
    int field_0x8;
    char pad_0xC[0x8];
    int field_0x14;
    char pad_0x18[0x4];
    int field_0x1C;
};
extern sOptionGlobal_005308B8 D_005308B8;

/**
 * Returns the largest head start in the rules block D_005308B8: the maximum of the AI head start (+0x1C) and the two
 * records' head starts (+0x8, +0x14).
 *
 * @return the largest head start, in seconds
 */
extern "C" int func_0014F870()
{
    int m = D_005308B8.field_0x1C;
    if (m < D_005308B8.field_0x8)
        m = D_005308B8.field_0x8;
    if (m < D_005308B8.field_0x14)
        m = D_005308B8.field_0x14;
    return m;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", func_0014F898);
#ifdef SKIP_ASM
struct sOptionTriple_0014F898 {
    int a;
    int b;
    int c;
};

/**
 * Swaps the two 12-byte bragging-rights records at the start of the rules block D_005308B8 (all three words of each).
 */
// Swaps the two 12-byte triples at the start of D_005308B8.
extern "C" void func_0014F898()
{
    sOptionTriple_0014F898* t = (sOptionTriple_0014F898*)&D_005308B8;
    sOptionTriple_0014F898 tmp = t[0];
    t[0] = t[1];
    t[1] = tmp;
}
#endif

