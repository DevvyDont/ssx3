#include "common.h"
#include "vtable.h"
#include "be/beintbagt.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEBAGTInt[];

struct sVTableBAGT {
    char pad_0x00[0x10];
    short field_0x10;
    char pad_0x12[2];
    void (*fn)(void*);
};
extern sVTableBAGT cBEBAGTInterface_vtbl;


//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's BAGT interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEBAGTInt", stores the vtable cBEBAGTInterface_vtbl, calls its virtual
 * slot 2 (discard) and clears bIsActive (+0x8).
 *
 * @return the BAGT interface (cBEBAGTInterface_instance)
 */
void* cBEBAGTInterface_getThis()
{
    if (cBEBAGTInterface_instance == 0) {
        cBEBAGTInterface* mem = (cBEBAGTInterface*)cMemMan_alloc(0x10, str_BEBAGTInt, 0, 0);
        cBEBAGTInterface_instance = (cBEBAGTInterface*)(mem);
        sVTableBAGT* vt = &cBEBAGTInterface_vtbl;
        short delta = vt->field_0x10;
        void (*fn)(void*) = vt->fn;
        (*(void**)&VPTR_OF(cBEInterface, mem)) = vt;
        fn((char*)mem + delta);
        cBEBAGTInterface_instance->bIsActive = 0;
    }
    return cBEBAGTInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_submit);
#ifdef SKIP_ASM
struct sBagtRec {
    short data[0xFF];
};

struct sBagtSlot {
    sBagtRec rec;
    char pad[0xF88 - 0x1FE];
};

extern sBagtSlot D_004A788E[3][10];
extern sBagtRec D_00530EC0[3][10];

/**
 * Commits the staged trick choices: copies each 0x1FE-byte record of D_00530EC0 (3 profiles x 10 characters) into the
 * matching character profile (D_004A788E, +0xBE6 of each 0xF88-byte entry of cBELibrary_profileLib).
 *
 * A record is 85 six-byte entries, one per grab: two chosen-variant bytes and two 16-bit variant masks (see
 * cBEBAGTInterface_chooseUberVar, func_0014FFB8, func_00150048).
 */
extern "C" void cBEBAGTInterface_submit(void)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            D_004A788E[i][j].rec = D_00530EC0[i][j];
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_discard);
#ifdef SKIP_ASM
extern sBagtSlot D_004A788E[3][10];
extern sBagtRec D_00530EC0[3][10];

/**
 * Drops staged trick changes: copies each character profile's 0x1FE-byte trick record (D_004A788E, +0xBE6 of each
 * 0xF88-byte profile entry) back into D_00530EC0 (3 profiles x 10 characters).
 */
extern "C" void cBEBAGTInterface_discard(void)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            D_00530EC0[i][j] = D_004A788E[i][j].rec;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_0014FC40);
#ifdef SKIP_ASM
extern sBagtSlot D_004A788E[3][10];
extern sBagtRec D_00530EC0[3][10];

/**
 * Reloads the staged trick records of one profile slot: copies the 10 characters' 0x1FE-byte records of profile @p i
 * (D_004A788E) into D_00530EC0[@p i].
 *
 * cBE_newProfile calls it with the profile index.
 *
 * @param i  the profile slot, 0-2
 */
extern "C" void func_0014FC40(int i)
{
    for (int j = 0; j < 10; j++) {
        D_00530EC0[i][j] = D_004A788E[i][j].rec;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_0014FD80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern char D_00530EC0_raw[] __asm__("D_00530EC0");

/**
 * Stores @p value as the second chosen-variant byte (+1) of grab @p idx in the staged trick record of @p rider's
 * profile and character (D_00530EC0).
 *
 * This is the uber variant cBEBAGTInterface_chooseUberVar returns with its last argument set; the uber-trick store calls this to equip
 * an owned trick and then submits (vtable slot 1).
 *
 * @param rider  the rider (selects profile and character)
 * @param idx  the grab, 0-84
 * @param value  the variant index
 * @return 1
 */
extern "C" int func_0014FD80(cBEBAGTInterface* self, int rider, int idx, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int off = idx * 6 + characterId * 0x1FE + profile * 5100;
    char* entry = D_00530EC0_raw + off;
    entry[1] = value;
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_0014FE08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sBagtEntry {
    unsigned char b0;
    unsigned char b1;
    unsigned short flags;  // 0x2
    unsigned short flags2; // 0x4
};

struct sBagtEntries {
    sBagtEntry e[85];
};

extern sBagtEntries D_00530EC0_e[3][10] __asm__("D_00530EC0");

/**
 * Clears bit @p bit of the first variant mask (+2) of grab @p idx in @p rider's staged trick record (D_00530EC0).
 *
 * A set bit marks a variant still for sale (func_0014FFB8); the uber-trick store clears it when the trick is bought.
 *
 * @param rider  the rider (selects profile and character)
 * @param idx  the grab
 * @param bit  the variant
 * @return 1
 */
extern "C" int func_0014FE08(cBEBAGTInterface* self, int rider, int idx, int bit)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    D_00530EC0_e[profile][characterId].e[idx].flags &= ~(1 << bit);
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_chooseUberVar);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern char D_00530EC0_raw[] __asm__("D_00530EC0");

/**
 * Returns the uber variant chosen for grab @p idx by @p rider: the second variant byte of the staged record when
 * @p second is set, else the first; -1 when the grab has no uber variants.
 *
 * The grab's record in kGrabDB (tBEGrabDB) holds the variant count in numUbers. The staged record is D_00530EC0 for the
 * rider's profile and character. The uber anim, cycle anim and name getters (cBEBAGTInterface_getUberAnim, cBEBAGTInterface_getUberCycleAnim,
 * cBEBAGTInterface_getUberName) pass @p second = uber level >= 5.
 *
 * @param rider  the rider (selects profile and character)
 * @param idx  the grab
 * @param second  nonzero to read the second (customisable) choice
 * @return the variant index, or -1
 */
int cBEBAGTInterface::chooseUberVar(int rider, int idx, int second)
{
    if (kGrabDB[idx].numUbers == 0)
        return -1;
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int variant;
    if (second)
    {
        char* entry = D_00530EC0_raw + (idx * 6 + characterId * 0x1FE + profile * 5100);
        variant = (unsigned char)entry[1];
    }
    else
    {
        char* entry = D_00530EC0_raw + (idx * 6 + characterId * 0x1FE + profile * 5100);
        variant = (unsigned char)entry[0];
    }
    return variant;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_0014FF90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBagInfo_FF90 {
    char pad[0xC];
    unsigned short count;  // 0xC
    short pad2;
    int value;             // 0x10
};
extern sBagInfo_FF90 kGrabDB_FF90[] __asm__("kGrabDB");

// Release-build assert: expands to an empty do { } while (0); its loop notes keep the target's register choice.
#define ASSERT_FF90(cond) do { } while (0)

/**
 * Returns grab @p idx's uber variant table and stores its length in @p count.
 *
 * Reads the grab's 0x14-byte record in kGrabDB: the table pointer at +0x10 and the count at +0xC. Each table
 * entry is {anim, cycle anim, name/trick id, pad} (4 halfwords); the uber-trick store lists them. @p rider is unused.
 *
 * @param rider  unused
 * @param idx  the grab
 * @param count  receives the number of variants
 * @return the variant table
 */
extern "C" int func_0014FF90(cBEBAGTInterface* self, int rider, int idx, int* count)
{
    int uberTable = kGrabDB_FF90[idx].value;
    unsigned short uberCount;
    uberCount = kGrabDB_FF90[idx].count;
    *count = uberCount;
    ASSERT_FF90(count != 0);
    return uberTable;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_0014FFB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sBagtEntries D_00530EC0_e[3][10] __asm__("D_00530EC0");

/**
 * Tests bit @p bit of the first variant mask (+2) of grab @p idx in @p rider's staged trick record: whether uber
 * variant @p bit is still for sale.
 *
 * func_0014FE08 clears the bit when the trick is bought.
 *
 * @param rider  the rider (selects profile and character)
 * @param idx  the grab
 * @param bit  the variant
 * @return 1 if the bit is set, else 0
 */
extern "C" int func_0014FFB8(cBEBAGTInterface* self, int rider, int idx, int bit)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    return (D_00530EC0_e[profile][characterId].e[idx].flags >> bit) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_00150048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sBagtEntries D_00530EC0_e[3][10] __asm__("D_00530EC0");

/**
 * Tests bit @p bit of the second variant mask (+4) of grab @p idx in @p rider's staged trick record: whether uber
 * variant @p bit is offered to this character at all.
 *
 * func_00150558 sets every variant from 2 up in this mask; the uber-trick store lists only variants with the bit set.
 *
 * @param rider  the rider (selects profile and character)
 * @param idx  the grab
 * @param bit  the variant
 * @return 1 if the bit is set, else 0
 */
extern "C" int func_00150048(cBEBAGTInterface* self, int rider, int idx, int bit)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    return (D_00530EC0_e[profile][characterId].e[idx].flags2 >> bit) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getGrabAnim);
#ifdef SKIP_ASM
/**
 * Returns the start animation of grab @p grabIndex: grabAnim of its record in kGrabDB.
 *
 * cSpinControl_isGrabPlaying plays it when a grab begins. @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the animation id
 */
unsigned short cBEBAGTInterface::getGrabAnim(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].grabAnim;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getGrabCycleAnim);
#ifdef SKIP_ASM
/**
 * Returns the held (looping) animation of grab @p grabIndex: grabCycleAnim of its record in kGrabDB.
 *
 * 438 means none. @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the animation id
 */
unsigned short cBEBAGTInterface::getGrabCycleAnim(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].grabCycleAnim;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getGrabName);
#ifdef SKIP_ASM
/**
 * Returns the trick id of grab @p grabIndex: grabID of its record in kGrabDB, scored when the grab is held
 * and released. @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the trick id
 */
unsigned short cBEBAGTInterface::getGrabName(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].grabID;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getTweakAnim);
#ifdef SKIP_ASM
/**
 * Returns the tweak animation of grab @p grabIndex: tweakAnim of its record in kGrabDB. @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the animation id
 */
unsigned short cBEBAGTInterface::getTweakAnim(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].tweakAnim;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getTweakCycleAnim);
#ifdef SKIP_ASM
/**
 * Returns the held tweak animation of grab @p grabIndex: tweakCycleAnim of its record in kGrabDB (438 = none).
 * @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the animation id
 */
unsigned short cBEBAGTInterface::getTweakCycleAnim(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].tweakCycleAnim;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getTweakName);
#ifdef SKIP_ASM
/**
 * Returns the trick id of grab @p grabIndex's tweak: tweakID of its record in kGrabDB. @p rider is unused.
 *
 * @param rider  the rider (unused)
 * @param grabIndex  the grab
 * @return the trick id
 */
unsigned short cBEBAGTInterface::getTweakName(int rider, int grabIndex)
{
    return kGrabDB[grabIndex].tweakID;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getUberAnim);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBELibrary_getCheatCharacterID(int rider);
extern "C" int cBEBAGTInterface_chooseUberVar(cBEBAGTInterface* self, int rider, int idx, int second);

struct sBagtAnim_150198
{
    unsigned short anim;
    unsigned short anim2;
    unsigned short anim3;
    unsigned short pad;
};

struct sBagtCat_150198
{
    char pad0[0x10];
    sBagtAnim_150198* anims;    // 0x10
};
// Typed view of kGrabDB (tBEGrabDB, include/be/beintbagt.h).
extern sBagtCat_150198 kGrabDB_150198[] __asm__("kGrabDB");

/**
 * Returns the uber animation of grab @p idx for @p rider: the anim of the chosen variant (cBEBAGTInterface_chooseUberVar) in the grab's
 * variant table (kGrabDB record +0x10), or 438 when the grab has none.
 *
 * Some cheat characters (cBELibrary_getCheatCharacterID 20, 24, 25, 28) get a fixed anim (159-162) for one grab.
 *
 * @param rider  the rider
 * @param idx  the grab
 * @param second  passed to cBEBAGTInterface_chooseUberVar (the callers pass uber level >= 5)
 * @return the animation id
 */
int cBEBAGTInterface::getUberAnim(int rider, int idx, int second)
{
    int ch = cBELibrary_getCheatCharacterID(rider);
    if (ch != 0)
    switch (ch)
    {
    case 20:
        if (idx == 4) return 160;
        break;
    case 0x18:
        if (idx == 4) return 159;
        break;
    case 25:
        if (idx == 9) return 162;
        break;
    case 0x1C:
        if (idx == 4) return 161;
        break;
    }
    int k = chooseUberVar(rider, idx, second);
    if (k < 0) return 438;
    return kGrabDB_150198[idx].anims[k].anim;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getUberCycleAnim);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBELibrary_getCheatCharacterID(int rider);
extern "C" int cBEBAGTInterface_chooseUberVar(cBEBAGTInterface* self, int rider, int idx, int second);

struct sBagtAnim_1502C8
{
    unsigned short anim;
    unsigned short anim2;
    unsigned short anim3;
    unsigned short pad;
};

struct sBagtCat_1502C8
{
    char pad0[0x10];
    sBagtAnim_1502C8* anims;    // 0x10
};
// Typed view of kGrabDB (tBEGrabDB, include/be/beintbagt.h).
extern sBagtCat_1502C8 kGrabDB_1502C8[] __asm__("kGrabDB");

/**
 * Returns the held uber animation of grab @p idx for @p rider: the cycle anim of the chosen variant
 * (cBEBAGTInterface_chooseUberVar), or 438 when the grab has none.
 *
 * The same cheat characters as in cBEBAGTInterface_getUberAnim get fixed anims (209-212).
 *
 * @param rider  the rider
 * @param idx  the grab
 * @param second  passed to cBEBAGTInterface_chooseUberVar (uber level >= 5)
 * @return the animation id
 */
int cBEBAGTInterface::getUberCycleAnim(int rider, int idx, int second)
{
    int ch = cBELibrary_getCheatCharacterID(rider);
    if (ch != 0)
    switch (ch)
    {
    case 20:
        if (idx == 4) return 210;
        break;
    case 0x18:
        if (idx == 4) return 209;
        break;
    case 25:
        if (idx == 9) return 212;
        break;
    case 0x1C:
        if (idx == 4) return 211;
        break;
    }
    int k = chooseUberVar(rider, idx, second);
    if (k < 0) return 438;
    return kGrabDB_1502C8[idx].anims[k].anim2;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getUberName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBELibrary_getCheatCharacterID(int rider);
extern "C" int cBEBAGTInterface_chooseUberVar(cBEBAGTInterface* self, int rider, int idx, int second);

struct sBagtAnim_1503F8
{
    unsigned short anim;
    unsigned short anim2;
    unsigned short anim3;
    unsigned short pad;
};

struct sBagtCat_1503F8
{
    char pad0[0x10];
    sBagtAnim_1503F8* anims;    // 0x10
};
// Typed view of kGrabDB (tBEGrabDB, include/be/beintbagt.h).
extern sBagtCat_1503F8 kGrabDB_1503F8[] __asm__("kGrabDB");

/**
 * Returns the trick id of grab @p idx's chosen uber variant for @p rider (cBEBAGTInterface_chooseUberVar), or 0 when the grab has none.
 *
 * The same cheat characters as in cBEBAGTInterface_getUberAnim get fixed ids (81-84).
 *
 * @param rider  the rider
 * @param idx  the grab
 * @param second  passed to cBEBAGTInterface_chooseUberVar (uber level >= 5)
 * @return the trick id
 */
int cBEBAGTInterface::getUberName(int rider, int idx, int second)
{
    int ch = cBELibrary_getCheatCharacterID(rider);
    if (ch != 0)
    switch (ch)
    {
    case 20:
        if (idx == 4) return 82;
        break;
    case 0x18:
        if (idx == 4) return 81;
        break;
    case 25:
        if (idx == 9) return 84;
        break;
    case 0x1C:
        if (idx == 4) return 83;
        break;
    }
    int k = chooseUberVar(rider, idx, second);
    if (k < 0) return 0;
    return kGrabDB_1503F8[idx].anims[k].anim3;
}
#endif

extern tBEGrabPointsDB cBELibrary_grabPointsDB[];

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getGrabPoints__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the base points a grab is worth.
 *
 * cBELibrary_grabPointsDB is an array of {points, pointsPerSec} pairs indexed by grab.
 *
 * @param grabEnum  the grab (On Tour grabEnum)
 * @return the grab's base points
 */
int cBEBAGTInterface_getGrabPoints(void* self, int grabEnum)
{
    return cBELibrary_grabPointsDB[grabEnum].pointsPerGrab;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", cBEBAGTInterface_getGrabPointsPerSec__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the points per second a held grab earns.
 *
 * Second word of the grab's {points, pointsPerSec} pair in cBELibrary_grabPointsDB.
 *
 * @param grabEnum  the grab (On Tour grabEnum)
 * @return the grab's points per second of hold
 */
int cBEBAGTInterface_getGrabPointsPerSec(void* self, int grabEnum)
{
    return cBELibrary_grabPointsDB[grabEnum].pointsPerSec;
}
#endif

//100%
INCLUDE_ASM("be/beintbagt", func_00150558);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sBagtSlot0558 {
    unsigned char a;
    unsigned char b;
    unsigned short c;
    unsigned short d;
};
struct sBagtDef0558 {
    int pad_0x00[3];
    unsigned short count;
    unsigned short pad_0x0E;
    int pad_0x10;
};
extern const sBagtDef0558 kGrabDB_bagt[] __asm__("kGrabDB");
extern "C" void MEM_fill(void*, int, int);

/**
 * Fills one character's trick record with its starting choices: chosen variants for grabs 0-4 and 9 by @p mode, and
 * the variant masks of the first 15 grabs.
 *
 * Clears 90 bytes, then sets each grab's two variant bytes from a per-character table (@p mode 0-8, else a default).
 * For grabs 0-14 both masks get one bit per variant in kGrabDB (+0xC count) minus bits 0-1; the first mask (for
 * sale) also drops the second chosen variant. cBECharProfileDB_init calls it.
 *
 * @param mode  the character (selects the starting choices)
 * @param entries  the record's first entries (6 bytes each)
 */
extern "C" void func_00150558(int mode, sBagtSlot0558* entries)
{
    int i;
    MEM_fill(entries, 0, 90);
    switch (mode) {
    case 0:
        entries[0].a = 1; entries[0].b = 3; entries[1].a = 0; entries[1].b = 2; entries[2].a = 0; entries[2].b = 2;
        entries[3].a = 1; entries[3].b = 2; entries[4].a = 0; entries[4].b = 7; entries[9].a = 1; entries[9].b = 3;
        break;
    case 1:
        entries[0].a = 0; entries[0].b = 2; entries[1].a = 0; entries[1].b = 2; entries[2].a = 1; entries[2].b = 3;
        entries[3].a = 0; entries[3].b = 3; entries[4].a = 1; entries[4].b = 2; entries[9].a = 0; entries[9].b = 9;
        break;
    case 2:
        entries[0].a = 1; entries[0].b = 3; entries[1].a = 1; entries[1].b = 3; entries[2].a = 0; entries[2].b = 2;
        entries[3].a = 0; entries[3].b = 2; entries[4].a = 0; entries[4].b = 8; entries[9].a = 1; entries[9].b = 3;
        break;
    case 3:
        entries[0].a = 1; entries[0].b = 2; entries[1].a = 0; entries[1].b = 3; entries[2].a = 0; entries[2].b = 3;
        entries[3].a = 1; entries[3].b = 2; entries[4].a = 0; entries[4].b = 2; entries[9].a = 1; entries[9].b = 6;
        break;
    case 4:
        entries[0].a = 1; entries[0].b = 3; entries[1].a = 1; entries[1].b = 2; entries[2].a = 0; entries[2].b = 2;
        entries[3].a = 1; entries[3].b = 2; entries[4].a = 1; entries[4].b = 10; entries[9].a = 1; entries[9].b = 3;
        break;
    case 5:
        entries[0].a = 0; entries[0].b = 2; entries[1].a = 0; entries[1].b = 3; entries[2].a = 0; entries[2].b = 2;
        entries[3].a = 0; entries[3].b = 2; entries[4].a = 0; entries[4].b = 3; entries[9].a = 1; entries[9].b = 7;
        break;
    case 6:
        entries[0].a = 0; entries[0].b = 2; entries[1].a = 0; entries[1].b = 2; entries[2].a = 1; entries[2].b = 3;
        entries[3].a = 0; entries[3].b = 3; entries[4].a = 1; entries[4].b = 2; entries[9].a = 0; entries[9].b = 8;
        break;
    case 7:
        entries[0].a = 1; entries[0].b = 3; entries[1].a = 1; entries[1].b = 2; entries[2].a = 0; entries[2].b = 2;
        entries[3].a = 1; entries[3].b = 3; entries[4].a = 0; entries[4].b = 9; entries[9].a = 0; entries[9].b = 2;
        break;
    case 8:
        entries[0].a = 1; entries[0].b = 3; entries[1].a = 1; entries[1].b = 2; entries[2].a = 0; entries[2].b = 3;
        entries[3].a = 1; entries[3].b = 2; entries[4].a = 0; entries[4].b = 3; entries[9].a = 0; entries[9].b = 10;
        break;
    default:
        entries[0].a = 0; entries[0].b = 2; entries[1].a = 1; entries[1].b = 3; entries[2].a = 0; entries[2].b = 3;
        entries[3].a = 1; entries[3].b = 3; entries[4].a = 1; entries[4].b = 6; entries[9].a = 1; entries[9].b = 2;
        break;
    }
    for (i = 0; i < 15; i++) {
        int j;
        entries[i].c = 0;
        entries[i].d = 0;
        for (j = 0; j < kGrabDB_bagt[i].count; j++) {
            entries[i].c |= 1 << j;
            entries[i].d |= 1 << j;
        }
        entries[i].d &= 0xFFFC;
        entries[i].c = (entries[i].c & 0xFFFC) & ~(1 << entries[i].b);
    }
}
#endif

