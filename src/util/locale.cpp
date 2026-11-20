#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("util/locale", cFELocale_addFile);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is probably the game's overloaded operator new(size, tag, flags, align). Calling it as
// operator new gives gcc's malloc-like (REG_NOALIAS) aliasing that the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cLocaleFile_loadFile(void* file, const char* name, signed char lang);
extern char str_LocaleFile[];
extern const char* D_00441138[];
extern signed char D_00441188[];

struct cFELocale_sFile {
    signed char lang;
    char pad1[3];
    int f4;
    int f8;
    int fC;
    int f10;
};

struct cFELocale_sFiles {
    cFELocale_sFile* items[4];
};

/**
 * Loads the common string file for a language unless it is already loaded.
 *
 * For lang 0-3 this is data/locale/cmnamer, cmnfren, cmngrmn or cmnspan.loc, loaded by cLocaleFile_loadFile() into a
 * new 0x14-byte entry ("LocaleFile") of the table at +0x8. A negative lang does nothing.
 *
 * @param lang  language index (0 English, 1 French, 2 German, 3 Spanish)
 */
extern "C" void cFELocale_addFile(void* self, int lang)
{
    signed char i = lang;
    if (i >= 0) {
        cFELocale_sFiles* arr = (cFELocale_sFiles*)((char*)self + 8);
        if (arr->items[i] == 0) {
            cFELocale_sFile* file = (cFELocale_sFile*)operator new(20, str_LocaleFile, 0, 0);
            arr->items[i] = file;
            file->lang = -1;
            file->f4 = 0;
            file->fC = 0;
            cLocaleFile_loadFile(file, D_00441138[i], D_00441188[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", cFELocale_deleteFile);
#ifdef SKIP_ASM
extern "C" void cLocaleFile__cLocaleFile(void*, int);

struct s195A50Arr {
    void* items[4];
};

/**
 * Deletes the locale file of entry @p i: when the entry (the pointer table at +0x8) is set, destroys and frees it with
 * cLocaleFile__cLocaleFile(file, 3) and clears the entry.
 *
 * @param i  the file entry index (0 .. 19)
 */
extern "C" void cFELocale_deleteFile(void* self, signed char i)
{
    s195A50Arr* arr = (s195A50Arr*)((char*)self + 8);
    if (arr->items[i] != 0) {
        cLocaleFile__cLocaleFile(arr->items[i], 3);
        arr->items[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195A98);
#ifdef SKIP_ASM
/**
 * Does nothing. Nothing in retail references it.
 */
extern "C" void func_00195A98(void) {

}
#endif

//100%
INCLUDE_ASM("util/locale", cFELocale_setLanguage);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteAllFiles(void* self);
extern "C" void cFELocale_loadFE(void* self);
extern "C" void cFELocale_loadOV(void* self);
extern "C" void cFELocale_addFile(void* self, int lang);

/**
 * Switches the front-end locale to another language and reloads its string files.
 *
 * Does nothing when @p lang is already the language (+0x1). Otherwise it stores it and, unless bit 0 of the locale
 * flags (+0x3, On Tour's tAttrLocale mLocaleTest) is set, deletes every file (cFELocale_deleteAllFiles), loads the
 * common file for languages 0-3 (cFELocale_addFile; -1, which loads nothing, for any other value), then the front-end
 * set (cFELocale_loadFE) when the module byte (+0x58) is 1, else the in-game set (cFELocale_loadOV). Locale vtable slot 3.
 *
 * @param lang  the language index (0 English, 1 French, 2 German, 3 Spanish)
 */
extern "C" void cFELocale_setLanguage(void* self, signed char lang)
{
    if (*(signed char*)((char*)self + 0x1) != lang) {
        *(signed char*)((char*)self + 0x1) = lang;
        int flags = *(signed char*)((char*)self + 0x3);
        if ((flags & 1) == 0) {
            cFELocale_deleteAllFiles(self);
            int langIndex = -1;
            switch (lang) {
            case 0:
                langIndex = 0;
                break;
            case 1:
                langIndex = 1;
                break;
            case 2:
                langIndex = 2;
                break;
            case 3:
                langIndex = 3;
                break;
            }
            cFELocale_addFile(self, langIndex);
            if (*(signed char*)((char*)self + 0x58) == 1) {
                cFELocale_loadFE(self);
            } else {
                cFELocale_loadOV(self);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", cFELocale_loadCommon);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

/**
 * Loads the string file shared by every front-end screen for the current language.
 *
 * Maps the language at +0x1 (0 to 3) to the matching file entry, or -1 for any other value, and passes it to
 * cFELocale_addFile.
 */
extern "C" void cFELocale_loadCommon(void* self)
{
    int lang = -1;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        lang = 0;
        break;
    case 1:
        lang = 1;
        break;
    case 2:
        lang = 2;
        break;
    case 3:
        lang = 3;
        break;
    }
    cFELocale_addFile(self, lang);
}
#endif

//100%
INCLUDE_ASM("util/locale", cFELocale_loadFE);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

/**
 * Loads the front-end string files for the current language: sets the module byte (+0x58) to 1 and adds file entries
 * 4-7, 8-11 and 16-19, the one of each group that matches the language (+0x1, 0-3) with cFELocale_addFile. Any other
 * language adds nothing.
 *
 * @param locale  the cFELocale
 */
extern "C" void cFELocale_loadFE(void* locale)
{
    char* self = (char*)locale;
    int id = -1;
    *(char*)(self + 0x58) = 1;
    switch (*(signed char*)(self + 1)) {
    case 0:
        id = 4;
        break;
    case 1:
        id = 5;
        break;
    case 2:
        id = 6;
        break;
    case 3:
        id = 7;
        break;
    }
    cFELocale_addFile(self, id);
    switch (*(signed char*)(self + 1)) {
    case 0:
        id = 8;
        break;
    case 1:
        id = 9;
        break;
    case 2:
        id = 10;
        break;
    case 3:
        id = 11;
        break;
    }
    cFELocale_addFile(self, id);
    id = -1;
    switch (*(signed char*)(self + 1)) {
    case 0:
        id = 0x10;
        break;
    case 1:
        id = 17;
        break;
    case 2:
        id = 18;
        break;
    case 3:
        id = 19;
        break;
    }
    cFELocale_addFile(self, id);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", cFELocale_unloadFE);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteFile(void* self, signed char i);

/**
 * Unloads the front-end string files: unless bit 0 of the locale flags (+0x3, mLocaleTest) is set, deletes file entries
 * 4-7 and 16-19 with cFELocale_deleteFile. Entries 8-11, which cFELocale_loadFE also loads, stay loaded, and the module byte is
 * not changed.
 */
extern "C" void cFELocale_unloadFE(void* self)
{
    int flags = *(signed char*)((char*)self + 0x3);
    if ((flags & 1) == 0) {
        cFELocale_deleteFile(self, 4);
        cFELocale_deleteFile(self, 5);
        cFELocale_deleteFile(self, 6);
        cFELocale_deleteFile(self, 7);
        cFELocale_deleteFile(self, 0x10);
        cFELocale_deleteFile(self, 17);
        cFELocale_deleteFile(self, 18);
        cFELocale_deleteFile(self, 19);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", cFELocale_loadOV);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

/**
 * Loads the in-game string file for the current language: sets the module byte (+0x58) to 2 and adds file entry 12-15,
 * the one matching the language (+0x1, 0-3), with cFELocale_addFile (-1, nothing, for any other language).
 */
extern "C" void cFELocale_loadOV(void* self)
{
    int file = -1;
    *(signed char*)((char*)self + 0x58) = 2;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        file = 0xC;
        break;
    case 1:
        file = 13;
        break;
    case 2:
        file = 0xE;
        break;
    case 3:
        file = 0xF;
        break;
    }
    cFELocale_addFile(self, file);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", cFELocale_unloadOV);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteFile(void* self, signed char i);

/**
 * Unloads the in-game string files: unless bit 0 of the locale flags (+0x3, mLocaleTest) is set, deletes file entries
 * 12-15 with cFELocale_deleteFile.
 */
extern "C" void cFELocale_unloadOV(void* self)
{
    int flags = *(signed char*)((char*)self + 3);
    if ((flags & 1) == 0) {
        cFELocale_deleteFile(self, 0xC);
        cFELocale_deleteFile(self, 13);
        cFELocale_deleteFile(self, 0xE);
        cFELocale_deleteFile(self, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", cFELocale_getStringByHash);
#ifdef SKIP_ASM
extern "C" const char* cLocaleFile_getStringByHash(void* file, int key);
extern "C" void sprintf2(char* dst, const char* src);
extern char str_sp[];

struct cFELocale_getStringByHash_sLocale {
    signed char useDefault;
    signed char lang;
    char pad2[6];
    signed char* files[20];
};

/**
 * Returns the front-end string with a given hash.
 *
 * When the display mode (+0x0) is 0, searches the 20 file entries (+0x8) for loaded files whose language byte equals
 * the current language (+0x1) and returns the first cLocaleFile_getStringByHash hit. Otherwise, or when no file has it, it writes
 * str_sp (a single space) into the 16-bit buffer at +0x5A with sprintf2 and returns that buffer. Locale vtable slot
 * 4.
 *
 * @param key  the string's hash (GetHashValue32 of its id)
 * @return the string, or the buffer at +0x5A
 */
extern "C" const char* cFELocale_getStringByHash(cFELocale_getStringByHash_sLocale* self, int key)
{
    if (self->useDefault == 0) {
        for (signed char i = 0; i < 20; i++) {
            signed char* localeFile = self->files[i];
            if (localeFile != 0 && *localeFile == self->lang) {
                const char* foundString = cLocaleFile_getStringByHash(localeFile, key);
                if (foundString != 0) {
                    return foundString;
                }
            }
        }
    }
    char* buf = (char*)self + 0x5A;
    sprintf2(buf, str_sp);
    return buf;
}
#endif

//100%
INCLUDE_ASM("util/locale", cFELocale_getStringByIndex);
#ifdef SKIP_ASM
// PORT: cLocaleFile_getStringByIndex__FPv really takes a second argument (it passes $5 through to
// LOCALE_getstr); bind the 2-arg view to the 1-arg mangled symbol.
void* cLocaleFile_getStringByIndex_2(void* self, void* arg) __asm__("cLocaleFile_getStringByIndex__FPv");

struct cFELocale_getStringByIndex_sArr {
    void* items[20];
};

/**
 * Returns the string at an index from the first loaded locale file that has one: calls cLocaleFile_getStringByIndex__FPv on each of
 * the 20 file entries (+0x8) and returns the first nonzero result, or 0.
 *
 * Unlike cFELocale_getStringByHash it does not check the files' language. Locale vtable slot 5.
 *
 * @param stringIndex  the string index
 * @return the string, or 0
 */
extern "C" void* cFELocale_getStringByIndex(void* self, void* stringIndex)
{
    cFELocale_getStringByIndex_sArr* arr = (cFELocale_getStringByIndex_sArr*)((char*)self + 8);
    signed char i;
    for (i = 0; i < 20; i++) {
        if (arr->items[i] != 0) {
            void* found = cLocaleFile_getStringByIndex_2(arr->items[i], stringIndex);
            if (found != 0) {
                return found;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/locale", cFEStateAudioOptions_cFEStateAudioOptions);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern "C" int cBERewardInterface_getRadioMode(void* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumSongsInPlayList(void* self, int a1, int a2);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int GetHashValue32(char*);
extern void* cFEStateAudioOptions_vtbl[];
extern char str_checkbox[];
extern char str_empty_box[];

/**
 * Constructs the audio options state for one player.
 *
 * Clears the widget pointers, looks up the "checkbox" and "empty box" entries (cUITextureBank_getElementByHashname on the engine's
 * +0x58 table) shown in the mode boxes, and reads the player's character, the selected music mode
 * (cBERewardInterface_getRadioMode; going by the menu items' ids, probably 0 radio big, 1 request line, 2 big mountain, 3
 * playlist) and whether the playlist has any songs.
 *
 * @param engine   the UI engine
 * @param idx      player (user) number (On Tour userNum)
 * @param inStore  nonzero when opened from the store (+0x74)
 * @return self
 */
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* self, void* engine, signed char idx, int inStore)
{
    char* selfBytes = (char*)self;
    cUIState_cUIState(self, engine);
    *(int*)(selfBytes + 0xC) = 0x10;
    *(void***)(selfBytes + 0x8) = cFEStateAudioOptions_vtbl;
    *(signed char*)(selfBytes + 0x44) = idx;
    void* feCustom = **(void***)(selfBytes + 0x10);
    if (feCustom != 0) {
        *(unsigned char*)(selfBytes + 0x15) = cFECustom_getPlayerCtrlMask(feCustom, idx);
    }
    *(int*)(selfBytes + 0x48) = 0;
    *(int*)(selfBytes + 0x64) = 0;
    *(int*)(selfBytes + 0x68) = 0;
    *(int*)(selfBytes + 0x6C) = 0;
    for (int i = 3; i >= 0; i--) {
        ((int*)(selfBytes + 0x4C))[i] = 0;
    }
    *(int*)(selfBytes + 0x74) = inStore;
    char* engineObj = *(char**)(selfBytes + 0x10);
    int hash = GetHashValue32(str_checkbox);
    *(void**)(selfBytes + 0x5C) = cUITextureBank_getElementByHashname(engineObj + 0x58, hash);
    char* engineObj2 = *(char**)(selfBytes + 0x10);
    int emptyBoxHash = GetHashValue32(str_empty_box);
    *(void**)(selfBytes + 0x60) = cUITextureBank_getElementByHashname(engineObj2 + 0x58, emptyBoxHash);
    *(int*)(selfBytes + 0x70) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)(selfBytes + 0x44));
    void* ri = cBE_getInterface_Fv(cBE_getBE(), 13);
    *(int*)(selfBytes + 0x78) = cBERewardInterface_getRadioMode(ri, *(signed char*)(selfBytes + 0x44), *(int*)(selfBytes + 0x70));
    *(int*)(selfBytes + 0x7C) = cBERewardInterface_getNumSongsInPlayList(ri, *(signed char*)(selfBytes + 0x44), *(int*)(selfBytes + 0x70)) > 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/locale", cFEStateAudioOptions_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern char str_hud[];
extern char str_142audio_pda[];
extern char str_140audio[];
class cFEStateAudioOptions_onDestroyScreen_cObj;
extern "C" cFEStateAudioOptions_onDestroyScreen_cObj* cUIStateStack_findState(void* list, int hash);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);

class cFEStateAudioOptions_onCreateScreen_cObj {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual int v24(int a, int b);
};

/**
 * Opens the audio options screen: the PDA version ("142audio_pda") in game, otherwise "140audio".
 *
 * In game it tells the "hud" state v24(3, 0); in the front end it calls func_0028F140(cAudioInterface_bxAudio(), 4).
 * The new screen is kept at +0x40 and starts at frame 0.
 */
extern "C" void cFEStateAudioOptions_onCreateScreen(void* self)
{
    char* name;
    if ((void*)SSXApp->Game != 0) {
        name = str_142audio_pda;
        cFEStateAudioOptions_onCreateScreen_cObj* obj = (cFEStateAudioOptions_onCreateScreen_cObj*)cUIStateStack_findState(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(str_hud));
        if (obj != 0) {
            obj->v24(3, 0);
        }
    } else {
        name = str_140audio;
        func_0028F140(cAudioInterface_bxAudio(), 4);
    }
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(name), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", cFEStateAudioOptions_onDestroyScreen);
#ifdef SKIP_ASM
int GetHashValue32(char*);
void* cUIState_onDestroyScreen(void* self);
extern char str_hud[];

class cFEStateAudioOptions_onDestroyScreen_cObj {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual int v24(int a, int b);
};

extern "C" cFEStateAudioOptions_onDestroyScreen_cObj* cUIStateStack_findState(void* list, int hash);

/**
 * Closes the audio options screen; in game it first tells the "hud" state v24(4, 0).
 */
extern "C" void cFEStateAudioOptions_onDestroyScreen(void* self)
{
    if ((void*)SSXApp->Game != 0) {
        cFEStateAudioOptions_onDestroyScreen_cObj* obj = cUIStateStack_findState(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(str_hud));
        if (obj != 0) {
            obj->v24(4, 0);
        }
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("util/locale", cFEStateAudioOptions_onGainTransition);
#ifdef SKIP_ASM
struct cFEStateAudioOptions_onGainTransition_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void cFEStateAudioOptions_onGainTransition_show(char* o, int on)
{
    cFEStateAudioOptions_onGainTransition_sVEntry* vt = *(cFEStateAudioOptions_onGainTransition_sVEntry**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

/**
 * On the first transition with an empty playlist, hides the options that need songs.
 *
 * Hides "request line", "playlist", the "2box" and "3box" ticks and, unless opened from the store, "edit
 * req".
 *
 * @param bFirstTime  nonzero on the state's first transition
 * @return always 0
 */
extern "C" int cFEStateAudioOptions_onGainTransition(void* self, int bFirstTime)
{
    if (bFirstTime != 0 && *(int*)((char*)self + 0x7C) == 0) {
        cFEStateAudioOptions_onGainTransition_show(*(char**)((char*)self + 0x68), 0);
        cFEStateAudioOptions_onGainTransition_show(*(char**)((char*)self + 0x6C), 0);
        cFEStateAudioOptions_onGainTransition_show(*(char**)((char*)self + 0x50), 0);
        cFEStateAudioOptions_onGainTransition_show(*(char**)((char*)self + 0x58), 0);
        if (*(int*)((char*)self + 0x74) == 0) {
            cFEStateAudioOptions_onGainTransition_show(*(char**)((char*)self + 0x64), 0);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/locale", cFEStateAudioOptions_onGainFocus);
#ifdef SKIP_ASM
struct cFEStateAudioOptions_onGainFocus_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct cFEStateAudioOptions_onGainFocus_sVEntryP {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern char gDisableColour[];
extern "C" void cFEState_onGainFocus(void* self, int a1);

static inline void cFEStateAudioOptions_onGainFocus_call(char* o, int slot, int a)
{
    cFEStateAudioOptions_onGainFocus_sVEntry* vt = *(cFEStateAudioOptions_onGainFocus_sVEntry**)(o + 8);
    vt[slot].fn(o + vt[slot].delta, a);
}

static inline void cFEStateAudioOptions_onGainFocus_callp(char* o, int slot, void* a)
{
    cFEStateAudioOptions_onGainFocus_sVEntryP* vt = *(cFEStateAudioOptions_onGainFocus_sVEntryP**)(o + 8);
    vt[slot].fn(o + vt[slot].delta, a);
}

/**
 * When the playlist is empty, shows the song options but greyed out and disabled.
 *
 * "request line", "playlist" and (unless in the store) "edit req" are made visible, switched to the disabled
 * colour and disabled; the "2box" and "3box" ticks are made visible and coloured with gDisableColour. Then
 * runs cFEState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before (On Tour cUIState*)
 */
extern "C" void cFEStateAudioOptions_onGainFocus(void* self, int pPrevState)
{
    char* selfBytes = (char*)self;
    if (*(int*)(selfBytes + 0x7C) == 0) {
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x68), 9, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x50), 9, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x64), 9, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x6C), 9, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x58), 9, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x68), 26, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x68), 6, 0);
        cFEStateAudioOptions_onGainFocus_callp(*(char**)(selfBytes + 0x50), 11, gDisableColour);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x6C), 26, 1);
        cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x6C), 6, 0);
        cFEStateAudioOptions_onGainFocus_callp(*(char**)(selfBytes + 0x58), 11, gDisableColour);
        if (*(int*)(selfBytes + 0x74) == 0) {
            cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x64), 26, 1);
            cFEStateAudioOptions_onGainFocus_call(*(char**)(selfBytes + 0x64), 6, 0);
        }
    }
    cFEState_onGainFocus(self, pPrevState);
}
#endif

