#include "common.h"
#include "vtable.h"
#include "be/beintoption.h"
#include "main/ssxapp.h"
#include "util/consoleconfig.h"
#include "util/locale.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cBEOptionInterface_revert(cBEOptionInterface* self);
extern const char str_BEOptionInt[];
extern void* cBEOptionInterface_vtbl[16];


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
        (*(void**)&VPTR_OF(cBEInterface, mem)) = cBEOptionInterface_vtbl;
        cBEOptionInterface_instance = (cBEOptionInterface*)(mem);
        mem->bIsActive = 0;
        mem->revert();
    }
    return cBEOptionInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_submit);
#ifdef SKIP_ASM
struct sOptions_0014F2A8
{
    int data[0x288 / 4];
};

extern sOptions_0014F2A8 cBEOptionInterface_mOption;
extern sOptions_0014F2A8 cBELibrary_optionDB;

/**
 * Commits the options: copies cBEOptionInterface_mOption (0x288 bytes) into the saved copy cBELibrary_optionDB.
 */
extern "C" void cBEOptionInterface_submit(void)
{
    cBELibrary_optionDB = cBEOptionInterface_mOption;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_discard);
#ifdef SKIP_ASM
extern sOptions_0014F2A8 cBEOptionInterface_mOption;
extern sOptions_0014F2A8 cBELibrary_optionDB;

/**
 * Drops option changes: copies the saved copy cBELibrary_optionDB back into cBEOptionInterface_mOption (0x288 bytes).
 */
extern "C" void cBEOptionInterface_discard(void)
{
    cBEOptionInterface_mOption = cBELibrary_optionDB;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_revert);
#ifdef SKIP_ASM
extern "C" void cBEOptionInterface_defaultCtrl(cBEOptionInterface* self);
extern "C" void cBEOptionInterface_defaultHUD(cBEOptionInterface* self);
extern "C" void cBEOptionInterface_defaultGame(cBEOptionInterface* self);
extern "C" void cBEOptionInterface_defaultSound(cBEOptionInterface* self);

/**
 * Resets all game options to their defaults.
 *
 * Calls cBEOptionInterface_defaultCtrl, cBEOptionInterface_defaultHUD, cBEOptionInterface_defaultGame and cBEOptionInterface_defaultSound (On Tour defaultCtrl, defaultHUD, defaultGame,
 * defaultSound).
 */
void cBEOptionInterface::revert()
{
    defaultCtrl();
    defaultHUD();
    defaultGame();
    defaultSound();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_defaultSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The option block as the static cBEOptionInterface::mOption (this unit's submit / discard copy it as raw words).
extern tBEOptionDB cBEOptionInterface_mOptionDB[] __asm__("cBEOptionInterface_mOption");

/**
 * Resets the sound options and submits them.
 *
 * In the first option word: music, speech and effects volumes 10, sound mode 0, audio mix 0, arcade effects, DJ speech
 * and voice chat on; the low two bits of the second word become 1. Then calls the option interface's vtable slot 1
 * (cBEOptionInterface_submit).
 */
void cBEOptionInterface::defaultSound()
{
    tBEOptionDB* options = cBEOptionInterface_mOptionDB;
    options->iRadioBigVolume = 10;
    options->iCharVolume = 10;
    options->iSFXVolume = 10;
    options->iSoundMode = 0;
    options->iAudioMix = 0;
    options->iUseArcadeSFX = 1;
    options->iUseDJSpeech = 1;
    options->iUseVOIP = 1;
    options->iExtraSoundOption = 1;
    cBEOptionInterface_instance->submit();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_defaultGame);
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
extern sOptBits_0014F4F8 cBEOptionInterface_mOption_v0014F4F8[] __asm__("cBEOptionInterface_mOption");

class cBEOptionInterfaceV_0014F4F8
{
public:
    char pad_0x00[0xC];
    virtual void apply();
};

/**
 * Resets the game options and submits them.
 *
 * Widescreen mode follows the console configuration (SSXApp->ConsoleConfig->mTVAspect: anamorphic for an anamorphic
 * TV, 16:9 for a 16:9 one, else off);
 * language, metric units and the replay option become 0, brightness 9, and the screen offsets (mOption +5, +6) 0. Then
 * calls the option interface's vtable slot 1 (cBEOptionInterface_submit).
 */
void cBEOptionInterface::defaultGame()
{
    switch (SSXApp->ConsoleConfig->mTVAspect)
    {
    case cConsoleConfig::kCTV_AspectAnimorphic:
        cBEOptionInterface_mOption_v0014F4F8[0].j = kWidescreen_Anamorphic;
        break;
    case cConsoleConfig::kCTV_Aspect169:
        cBEOptionInterface_mOption_v0014F4F8[0].j = kWidescreen_16x9;
        break;
    case cConsoleConfig::kCTV_Aspect43:
    default:
        cBEOptionInterface_mOption_v0014F4F8[0].j = kWidescreen_Off;
        break;
    }
    cBEOptionInterface_mOption_v0014F4F8[0].k = 0;
    cBEOptionInterface_mOption_v0014F4F8[0].i = 0;
    cBEOptionInterface_mOption_v0014F4F8[0].m = 0;
    cBEOptionInterface_mOption_v0014F4F8[0].l = 9;
    cBEOptionInterface_mOption_v0014F4F8[0].b5 = 0;
    cBEOptionInterface_mOption_v0014F4F8[0].b6 = 0;
    ((cBEOptionInterfaceV_0014F4F8*)cBEOptionInterface_instance)->apply();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_defaultHUD);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/**
 * Resets the HUD option (bits 30-31 of the first option word) to 0 and submits (vtable slot 1, cBEOptionInterface_submit).
 */
void cBEOptionInterface::defaultHUD()
{
    cBEOptionInterface_mOptionDB[0].iHUDDetail = 0;
    cBEOptionInterface_instance->submit();
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_defaultCtrl);
#ifdef SKIP_ASM
/**
 * Submits the options (the option interface's vtable slot 1, cBEOptionInterface_submit) without changing any.
 */
void cBEOptionInterface::defaultCtrl()
{
    cBEOptionInterface_instance->submit();
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
int cBEOptionInterface::getDefaultQuickKeyMessageHashValue(int value)
{
    char buf[24];
    sprintf(buf, str_kT_206QuickMessagepct_d, value);
    return GetHashValue32(buf);
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_getQuickKeyMessage);
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
char* cBEOptionInterface::getQuickKeyMessage(int i)
{
    char* message = D_00535617[i];
    if (strlen(message) == 0) {
        return 0;
    }
    return message;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBEOptionInterface_resetQuickKey);
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
void cBEOptionInterface::resetQuickKey(int i)
{
    cFELocale* obj = SSXApp->FELocale;
    sVEntry_0014F6E8* vt = (sVEntry_0014F6E8*)VPTR_OF(cUILocaleInterface, obj);
    void* localeThis = vthis(obj, vt[4]);
    int hashValue = this->getDefaultQuickKeyMessageHashValue(i);
    void* text = vt[4].fn(localeThis, hashValue);
    USTR_strcpyW(D_00535617[i], text);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintoption", cBEOptionInterface_resetAllQuickKeys);
#ifdef SKIP_ASM
extern "C" void cBEOptionInterface_resetQuickKey(cBEOptionInterface* self, int i);

/**
 * Restores all ten quick-chat messages to their defaults (cBEOptionInterface_resetQuickKey for slots 0-9).
 */
void cBEOptionInterface::resetAllQuickKeys()
{
    for (int i = 0; i < 10; i++)
    {
        resetQuickKey(i);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBERules_getPlayerStartDelaySecs);
#ifdef SKIP_ASM
extern "C" int cBERules_getMaxDelaySecs();

/**
 * Returns how long player @p i is held at the start: the largest head start (cBERules_getMaxDelaySecs) minus the player's own
 * (word +8 of record @p i of the rules block cBERules_mRules).
 *
 * @param i  the player, 0 or 1
 * @return the delay in seconds
 */
extern "C" int cBERules_getPlayerStartDelaySecs(int i)
{
    tBERuleOptionDB* rules = &cBERules_mRules;  // through a pointer: the code adds HeadStartSecs' 8 at the load
    int headStart = rules->Bragging[i].HeadStartSecs;
    return cBERules_getMaxDelaySecs() - headStart;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBERules_getAIStartDelaySecs);
#ifdef SKIP_ASM
extern "C" int cBERules_getMaxDelaySecs();

/**
 * Returns how long the AI riders are held at the start: the largest head start (cBERules_getMaxDelaySecs) minus the AI head start
 * (D_005308D4, the rules block's word at +0x1C).
 *
 * @return the delay in seconds
 */
extern "C" int cBERules_getAIStartDelaySecs(void* self)
{
    return cBERules_getMaxDelaySecs() - cBERules_mRules.MyRules.AIHeadStartSecs;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBERules_hasModifiedRules);
#ifdef SKIP_ASM
/**
 * Tells whether any rule is set for player @p i: a nonzero word in record @p i of the rules block cBERules_mRules or in the
 * "my rules" pair (+0x18, +0x1C).
 *
 * @param i  the player, 0 or 1
 * @return true when any is set
 */
extern "C" bool cBERules_hasModifiedRules(int i)
{
    tBERuleOptionDB* rules = &cBERules_mRules;
    if (rules->Bragging[i].Flags != 0 || rules->Bragging[i].StartingPoints != 0 || rules->Bragging[i].HeadStartSecs != 0
        || rules->MyRules.Flags != 0 || rules->MyRules.AIHeadStartSecs != 0)
    {
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBERules_getMaxDelaySecs);
#ifdef SKIP_ASM
/**
 * Returns the largest head start in the rules block cBERules_mRules: the maximum of the AI head start (+0x1C) and the two
 * records' head starts (+0x8, +0x14).
 *
 * @return the largest head start, in seconds
 */
extern "C" int cBERules_getMaxDelaySecs()
{
    int maxHeadStart = cBERules_mRules.MyRules.AIHeadStartSecs;
    if (maxHeadStart < cBERules_mRules.Bragging[0].HeadStartSecs)
        maxHeadStart = cBERules_mRules.Bragging[0].HeadStartSecs;
    if (maxHeadStart < cBERules_mRules.Bragging[1].HeadStartSecs)
        maxHeadStart = cBERules_mRules.Bragging[1].HeadStartSecs;
    return maxHeadStart;
}
#endif

//100%
INCLUDE_ASM("be/beintoption", cBERules_swap);
#ifdef SKIP_ASM

/**
 * Swaps the two 12-byte bragging-rights records at the start of the rules block cBERules_mRules (all three words of each).
 */
extern "C" void cBERules_swap()
{
    tBEBraggingRights* records = cBERules_mRules.Bragging;
    tBEBraggingRights firstRecord = records[0];
    records[0] = records[1];
    records[1] = firstRecord;
}
#endif

