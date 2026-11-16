#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("util/locale", cFELocale_addFile);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is probably the game's overloaded operator new(size, tag, flags, align). Calling it as
// operator new gives gcc's malloc-like (REG_NOALIAS) aliasing that the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_002C6E98(void* file, const char* name, signed char lang);
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
 * For lang 0-3 this is data/locale/cmnamer, cmnfren, cmngrmn or cmnspan.loc, loaded by func_002C6E98() into a
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
            func_002C6E98(file, D_00441138[i], D_00441188[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195A50);
#ifdef SKIP_ASM
extern "C" void func_002C6D70(void*, int);

struct s195A50Arr {
    void* items[4];
};

extern "C" void func_00195A50(void* self, signed char i)
{
    s195A50Arr* arr = (s195A50Arr*)((char*)self + 8);
    if (arr->items[i] != 0) {
        func_002C6D70(arr->items[i], 3);
        arr->items[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195A98);
#ifdef SKIP_ASM
extern "C" void func_00195A98(void) {

}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195AA0);
#ifdef SKIP_ASM
extern "C" void func_00195948(void* self);
extern "C" void func_00195BE0(void* self);
extern "C" void func_00195DE0(void* self);
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195AA0(void* self, signed char lang)
{
    if (*(signed char*)((char*)self + 0x1) != lang) {
        *(signed char*)((char*)self + 0x1) = lang;
        int flags = *(signed char*)((char*)self + 0x3);
        if ((flags & 1) == 0) {
            func_00195948(self);
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
                func_00195BE0(self);
            } else {
                func_00195DE0(self);
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
INCLUDE_ASM("util/locale", func_00195BE0);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195BE0(void* locale)
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
INCLUDE_ASM("util/locale", func_00195D50);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, signed char i);

extern "C" void func_00195D50(void* self)
{
    int flags = *(signed char*)((char*)self + 0x3);
    if ((flags & 1) == 0) {
        func_00195A50(self, 4);
        func_00195A50(self, 5);
        func_00195A50(self, 6);
        func_00195A50(self, 7);
        func_00195A50(self, 0x10);
        func_00195A50(self, 17);
        func_00195A50(self, 18);
        func_00195A50(self, 19);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195DE0);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195DE0(void* self)
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
INCLUDE_ASM("util/locale", func_00195E58);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, signed char i);

extern "C" void func_00195E58(void* self)
{
    int flags = *(signed char*)((char*)self + 3);
    if ((flags & 1) == 0) {
        func_00195A50(self, 0xC);
        func_00195A50(self, 13);
        func_00195A50(self, 0xE);
        func_00195A50(self, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195EB8);
#ifdef SKIP_ASM
extern "C" const char* func_002C6DE0(void* file, int key);
extern "C" void sprintf2(char* dst, const char* src);
extern char D_004A1390[];

struct func_00195EB8_sLocale {
    signed char useDefault;
    signed char lang;
    char pad2[6];
    signed char* files[20];
};

extern "C" const char* func_00195EB8(func_00195EB8_sLocale* self, int key)
{
    if (self->useDefault == 0) {
        for (signed char i = 0; i < 20; i++) {
            signed char* f = self->files[i];
            if (f != 0 && *f == self->lang) {
                const char* s = func_002C6DE0(f, key);
                if (s != 0) {
                    return s;
                }
            }
        }
    }
    char* buf = (char*)self + 0x5A;
    sprintf2(buf, D_004A1390);
    return buf;
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195F70);
#ifdef SKIP_ASM
// PORT: func_002C6DC0__FPv really takes a second argument (it passes $5 through to
// func_003DBB68); bind the 2-arg view to the 1-arg mangled symbol.
void* func_002C6DC0_2(void* self, void* arg) __asm__("func_002C6DC0__FPv");

struct func_00195F70_sArr {
    void* items[20];
};

extern "C" void* func_00195F70(void* self, void* arg)
{
    func_00195F70_sArr* arr = (func_00195F70_sArr*)((char*)self + 8);
    signed char i;
    for (i = 0; i < 20; i++) {
        if (arr->items[i] != 0) {
            void* found = func_002C6DC0_2(arr->items[i], arg);
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
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern "C" int func_00158700(void* self, int a1, int a2);
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
 * (func_00158700; going by the menu items' ids, probably 0 radio big, 1 request line, 2 big mountain, 3
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
    func_0039E2A0(self, engine);
    *(int*)(selfBytes + 0xC) = 0x10;
    *(void***)(selfBytes + 0x8) = cFEStateAudioOptions_vtbl;
    *(signed char*)(selfBytes + 0x44) = idx;
    void* feCustom = **(void***)(selfBytes + 0x10);
    if (feCustom != 0) {
        *(unsigned char*)(selfBytes + 0x15) = func_001A1CD0(feCustom, idx);
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
    *(int*)(selfBytes + 0x78) = func_00158700(ri, *(signed char*)(selfBytes + 0x44), *(int*)(selfBytes + 0x70));
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
extern "C" void* func_0028B180();
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
 * In game it tells the "hud" state v24(3, 0); in the front end it calls func_0028F140(func_0028B180(), 4).
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
        func_0028F140(func_0028B180(), 4);
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

