#include "common.h"
#include "render/ps2graphicsman.h"
#include "movie/mcoverlaymanager.h"
#include "main/ssxapp.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cMCOverlayManager_cMCOverlayManager(cMCOverlayManager* self);
extern const char str_MCManager[];
extern void* cMCOverlayManager_instance;

//99.53%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getManager__Fv);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager, creating it on first use.
 *
 * The single instance (0x74C bytes, allocated as "MCManager") is kept in cMCOverlayManager_instance.
 *
 * @return the cMCOverlayManager instance
 */
void* cMCOverlayManager_getManager()
{
    if (cMCOverlayManager_instance == 0) {
        void* mem = cMemMan_alloc(1868, str_MCManager, 0, 0);
        cMCOverlayManager_instance = cMCOverlayManager_cMCOverlayManager((cMCOverlayManager*)mem);
    }
    return cMCOverlayManager_instance;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C850);
#ifdef SKIP_ASM
extern "C" int func_0023C850(void) {
    return cMCOverlayManager_instance != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_purge);
#ifdef SKIP_ASM
struct sMovieObj_23C860 {
    char pad[0x748];
    virtual void v01();
    virtual ~sMovieObj_23C860();
};

/**
 * Destroys the memory card overlay manager singleton.
 *
 * Deletes cMCOverlayManager_instance and sets it to null.
 */
extern "C" void cMCOverlayManager_purge()
{
    delete (sMovieObj_23C860*)cMCOverlayManager_instance;
    cMCOverlayManager_instance = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isManagerDone);
#ifdef SKIP_ASM
/**
 * Tells whether the memory card manager has finished its operation: mState (+0x130) is 1 (On Tour kMCOverlay_Done).
 *
 * @return 1 when done, otherwise 0
 */
extern "C" int cMCOverlayManager_isManagerDone(void* self)
{
    return *(int*)((char*)self + 0x130) == 1;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8A8);
#ifdef SKIP_ASM
extern "C" int func_0023C8A8(void *manager) {
    return (*(int *)((char*)(manager) + (0x130))) == 35;
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8B8);
#ifdef SKIP_ASM
extern "C" int func_0023C8B8(void *manager) {
    return (*(int *)((char*)(manager) + (0xDC)));
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8C0);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;

extern "C" int func_0023C8C0(void) {
    return cMCOverlayManager_mSaveInProgress != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_hasCardStatusCriticalError);
#ifdef SKIP_ASM
/**
 * Tells whether the last card state has an error other than the non-critical ones.
 *
 * Masks bit 0x4 (card full) and bits 0x1E0 out of mCurrDeviceStateLast (+0x43C) and tests what is left.
 *
 * @return 1 when another bit is set, otherwise 0
 */
extern "C" int cMCOverlayManager_hasCardStatusCriticalError(void* self)
{
    // masks applied as separate statements; folding them into one
    // expression collapses the two `and` instructions into one
    int stateTest = *(int*)((char*)self + 0x43c);
    stateTest &= -5;
    stateTest &= -481;
    return stateTest != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023C8F0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_pct_s_pct_s_e[];

struct sMovieVEntry_23C8F0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023C8F0(void* self, int mode)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    char* manager = (char*)self;
    *(int*)(manager + 0xB4) = mode;
    cMCOverlayManager_SetAction((cMCOverlayManager*)self, 1);
    *(int*)(manager + 0xFC) = -1;
    *(int*)(manager + 0xBC) = 0;
    int fileType = *(int*)(manager + 0xB4);
    if (fileType == 0) {
        *(int*)(manager + 0xEC) = 4;
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)memCard;
        vt[19].fn(memCard + vt[19].delta, 0x80000);
    } else if (fileType == 1) {
        *(int*)(manager + 0xEC) = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveProfileSize();
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)memCard;
        vt[19].fn(memCard + vt[19].delta, fileSize);
    } else {
        *(int*)(manager + 0xEC) = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveGameSize_r();
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23C8F0* vt = *(sMovieVEntry_23C8F0**)memCard;
        vt[19].fn(memCard + vt[19].delta, fileSize);
    }
    func_002C2540(fmt, str_pct_s_pct_s_e);
    func_00241AA0_r(desc, 0x200);
    func_00241D40_r(self, *(int*)(manager + 0xB4), name, 0x100);
    sprintf1_v(manager + 0x1B4, fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_readFile);
#ifdef SKIP_ASM
struct sMovieVEntryCA28 {
    short delta;
    short index;
    void (*fn)(void*, int, int, int);
};

/**
 * Starts reading a save file from the memory card.
 *
 * Calls virtual slot 29 of the memory card object (+0x434) with the three arguments and stores @p size at +0xE8.
 *
 * @param file  which file
 * @param data  destination buffer
 * @param size  bytes to read
 */
extern "C" void cMCOverlayManager_readFile(void* self, int file, int data, int size)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCA28* vt = *(sMovieVEntryCA28**)obj;
    vt[29].fn((char*)obj + vt[29].delta, file, data, size);
    *(int*)((char*)self + 0xE8) = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CA70);
#ifdef SKIP_ASM
struct sMovieVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023CA70(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[32].fn((char*)obj + vt[32].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CAA0);
#ifdef SKIP_ASM
struct sMovieVEntryCAA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// PORT: the unit declares func_0023CAA0(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.
void func_0023CAA0_impl(void* self, int a) __asm__("func_0023CAA0");

void func_0023CAA0_impl(void* self, int size)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntryCAA0* vt = *(sMovieVEntryCAA0**)obj;
    vt[19].fn((char*)obj + vt[19].delta, size);
    *(int*)((char*)self + 0xF0) = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CAE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_pct_s_pct_s_e[];

struct sMovieVEntry_23CAE8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0023CAE8(void* self, int mode, int profileId)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    char* manager = (char*)self;
    *(int*)(manager + 0xB4) = mode;
    cMCOverlayManager_SetAction((cMCOverlayManager*)self, 1);
    *(int*)(manager + 0xFC) = profileId;
    *(int*)(manager + 0xBC) = 1;
    int fileType = *(int*)(manager + 0xB4);
    if (fileType == 0) {
        *(int*)(manager + 0xEC) = 4;
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)memCard;
        vt[19].fn(memCard + vt[19].delta, 0x80000);
    } else if (fileType == 1) {
        *(int*)(manager + 0xEC) = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveProfileSize();
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)memCard;
        vt[19].fn(memCard + vt[19].delta, fileSize);
    } else {
        *(int*)(manager + 0xEC) = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveGameSize_r();
        char* memCard = *(char**)(manager + 0x434);
        sMovieVEntry_23CAE8* vt = *(sMovieVEntry_23CAE8**)memCard;
        vt[19].fn(memCard + vt[19].delta, fileSize);
    }
    func_002C2540(fmt, str_pct_s_pct_s_e);
    func_00241AA0_r(desc, 0x200);
    func_00241D40_r(self, *(int*)(manager + 0xB4), name, 0x100);
    sprintf1_v(manager + 0x1B4, fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CC20);
#ifdef SKIP_ASM
extern "C" void func_0023CC20(void* self)
{
    if (*(int*)((char*)self + 0x118) != 0) {
        void* obj = *(void**)((char*)self + 0x434);
        sMovieVEntry* vt = *(sMovieVEntry**)obj;
        vt[43].fn((char*)obj + vt[43].delta);
    }
}
#endif

extern "C" void* func_0023CAA0(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", func_0023CC58__FPv);
#ifdef SKIP_ASM
void* func_0023CC58(void* self)
{
    return func_0023CAA0(self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023CC78);
#ifdef SKIP_ASM
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
int GetHashValue32(char* s);
extern char str_pct_spct_d_b[];
extern char str_kT_OVGameOptions[];

struct sLocVEntry_23CC78 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void func_0023CC78(void* self, int n, char* dst)
{
    char name[0x100];
    char fmt[0x200];
    switch (*(int*)((char*)self + 0xB4)) {
    case 0:
        func_002C2540(fmt, str_pct_spct_d_b);
        func_00241D40_r(self, *(int*)((char*)self + 0xB4), name, 0x80);
        sprintf1_v(dst, fmt, name, n + 1);
        break;
    case 2: {
        char* loc = (char*)SSXApp->FELocale;
        sLocVEntry_23CC78* vt = *(sLocVEntry_23CC78**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        strcpy1(dst, vt[4].fn(thisp, GetHashValue32(str_kT_OVGameOptions)));
        break;
    }
    case 1:
        func_002C2540(fmt, str_pct_spct_d_b);
        func_00241D40_r(self, *(int*)((char*)self + 0xB4), name, 0x80);
        sprintf1_v(dst, fmt, name, n + 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setTitleString);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
extern "C" void func_0023CC78(void* self, int n, char* dst);
extern "C" void* cBENewPlayerInterface_getPlayerName(void* iface, int id);
int GetHashValue32(char* s);
extern char str_kT_OVGameOptions[];
extern char str_SSX_3_pct_s[];
extern char str_kT_MEMFileNameReplay[];

struct sLocVEntry_23CDB0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

struct sMovieVEntry_23CDB0 {
    short delta;
    short index;
    void (*fn)(void*, char*);
};

/**
 * Builds the save-file title for a file slot and hands it to the memory card.
 *
 * The title depends on mFileType: type 0 formats the localized kT_MEMFileNameReplay text with @p n, type 1
 * uses
 * the profile name of mSaveProfileID, others use kT_OVGameOptions, each inside "SSX 3 %s". The result goes to
 * the
 * card object (vtable slot 45) and into mActiveTitleString.
 *
 * @param n  file slot number (On Tour calls it file)
 */
extern "C" void cMCOverlayManager_setTitleString(cMCOverlayManager* self, int n)
{
    char out[0x200];
    char title[0x200];
    char fmt[0x200];
    char name[0x200];
    func_0023CC78(self, n, title);
    func_002C2540(fmt, str_SSX_3_pct_s);
    switch (self->mFileType) {
    case 0: {
        char* loc = (char*)SSXApp->FELocale;
        sLocVEntry_23CDB0* vt = *(sLocVEntry_23CDB0**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        sprintf1_v(title, fmt, vt[4].fn(thisp, GetHashValue32(str_kT_MEMFileNameReplay)));
        sprintf1_v(out, title, n);
        break;
    }
    case 1: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        func_002C2540(name, cBENewPlayerInterface_getPlayerName(iface, self->mSaveProfileID));
        sprintf1_v(out, fmt, name);
        break;
    }
    default: {
        char* loc = (char*)SSXApp->FELocale;
        sLocVEntry_23CDB0* vt = *(sLocVEntry_23CDB0**)(loc + 4);
        char* thisp = loc + vt[4].delta;
        strcpy1(title, vt[4].fn(thisp, GetHashValue32(str_kT_OVGameOptions)));
        sprintf1_v(out, fmt, title);
        break;
    }
    }
    char* memCard = (char*)self->mpMemCard;
    sMovieVEntry_23CDB0* mvt = *(sMovieVEntry_23CDB0**)memCard;
    mvt[45].fn(memCard + mvt[45].delta, out);
    strcpy1((char*)self + 0x234, out);
}
#endif

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_SetState);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_SetAction);
#ifdef SKIP_ASM
/**
 * Switches the memory-card manager to another action, starting the display timers for action 1.
 *
 * Stores @p mode in mAction, which cMCOverlayManager_update() switches on. Action 1 also sets mFlash to 1.0,
 * mFlashStep to 1/60 and mDisplayWaitTicks to 60; any other value only stores mAction.
 *
 * @param mode  new action: 0 runs the load/save state machine, 1 the initial card and file checks
 * @return nothing meaningful; the body has no return statement
 */
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode)
{
    self->mAction = mode;
    switch (mode) {
    case 0:
        break;
    case 1:
        self->mFlash = 1.0f;
        self->mFlashStep = 0.016669999808073044f;
        self->mDisplayWaitTicks = 0x3C;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D5A8);
#ifdef SKIP_ASM
extern "C" void* cMemCard_CreateInstance(void);

extern "C" void func_0023D5A8(void* self)
{
    char* manager = (char*)self;
    void* memCard = cMemCard_CreateInstance();
//START
    *(void**)(manager + 0x434) = memCard;
    *(int*)(manager + 0x104) = 0;
    *(float*)(manager + 0x110) = 1.0f;
    *(float*)(manager + 0x114) = 0.016669999808073044f;
//END
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerLoseState);
#ifdef SKIP_ASM
extern "C" void cMemCard_DeleteInstance(void*);

/**
 * Releases the memory card object when the memory card screen goes away.
 *
 * Deletes +0x434 with cMemCard_DeleteInstance (On Tour cMemCard::DeleteInstance) and clears it.
 */
extern "C" void cMCOverlayManager_ManagerLoseState(void* self)
{
    cMemCard_DeleteInstance(*(void**)((char*)self + 0x434));
    *(void**)((char*)self + 0x434) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_update);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_ManagerLoadSaveUpdate(cMCOverlayManager* self);
extern "C" void func_0023D660(void* self);

/**
 * Runs one update of the memory-card manager for its current action.
 *
 * Action 0 calls cMCOverlayManager_ManagerLoadSaveUpdate(); action 1 calls func_0023D660(), which drives the
 * initial card and file checks (UpdateInitCardsCheck, UpdateInitFileCheck, UpdateInitFileCheckDevice) and can
 * switch back to action 0. Other actions do nothing.
 */
extern "C" void cMCOverlayManager_update(cMCOverlayManager* self)
{
    switch (self->mAction) {
    case 0:
        cMCOverlayManager_ManagerLoadSaveUpdate(self);
        break;
    case 1:
        func_0023D660(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D660);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_getMaxDevicesSupported(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitCardsCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFileCheckDevice(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFileCheck(cMCOverlayManager* self);

struct sVoidVEntry_23D660 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sIntVEntry_23D660 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sArgVEntry_23D660 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline bool isState_23D660(char* s, int k)
{
    return *(int*)(s + 0x130) == k;
}

extern "C" void func_0023D660(void* self)
{
    char* manager = (char*)self;
    char* memCard = *(char**)(manager + 0x434);
    sVoidVEntry_23D660* vt = *(sVoidVEntry_23D660**)memCard;
    vt[3].fn(memCard + vt[3].delta);
    if (*(int*)(manager + 0x338) != 0) {
        memCard = *(char**)(manager + 0x434);
        sIntVEntry_23D660* ivt = *(sIntVEntry_23D660**)memCard;
        if (ivt[11].fn(memCard + ivt[11].delta) != 0) {
            return;
        }
        *(int*)(manager + 0x338) = 0;
    }
    if (*(int*)(manager + 0x424) == -1) {
        *(int*)(manager + 0x424) = cMCOverlayManager_getMaxDevicesSupported((cMCOverlayManager*)self);
    }
    memCard = *(char**)(manager + 0x434);
    sIntVEntry_23D660* ivt = *(sIntVEntry_23D660**)memCard;
    if (ivt[11].fn(memCard + ivt[11].delta) == 0) {
        cMCOverlayManager_SetAction((cMCOverlayManager*)self, 0);
        int ok = !(isState_23D660(manager, 49) || isState_23D660(manager, 50) || isState_23D660(manager, 51) || isState_23D660(manager, 52) || isState_23D660(manager, 54));
        if (ok) {
            sArgVEntry_23D660* avt = *(sArgVEntry_23D660**)(manager + 0x748);
            avt[1].fn(manager + avt[1].delta, 3);
        }
    }
    int st = *(int*)(manager + 0x130);
    if (st == 49) {
        cMCOverlayManager_UpdateInitCardsCheck((cMCOverlayManager*)self);
    } else if (st == 51) {
        cMCOverlayManager_UpdateInitFileCheck((cMCOverlayManager*)self);
    } else if (st == 54) {
        cMCOverlayManager_UpdateInitFileCheckDevice((cMCOverlayManager*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023D7A8);
#ifdef SKIP_ASM
extern "C" int func_0023D7A8(void* self)
{
    int deviceState = *(int*)((char*)self + 0x43C);
    deviceState &= ~4;
    deviceState &= ~0x10;
    return *(int*)((char*)self + 0x440) == 0 && deviceState != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerLoadSaveUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`do { } while (0)`) in the isState inline, perhaps EA's compiled-out assert but unconfirmed; plain form 99.28%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).
#include "vtable.h"



// PORT: the unit declares cMCOverlayManager_GatherDeviceInfo1 as returning void*, but it returns nothing; bind the void body by asm label.
extern "C" void cMCOverlayManager_GatherDeviceInfo1_v(void* self, int i) __asm__("cMCOverlayManager_GatherDeviceInfo1");
extern "C" void MEM_fill(void* p, int c, int n);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_UpdateInitFileCheckWait(cMCOverlayManager* self);
void cMCOverlayManager_UpdateInitCardsCheckDone(void* self);
void cMCOverlayManager_UpdateInitFileCheckDone(void* self);
extern "C" void cMCOverlayManager_UpdateCheckCardsWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCheckCards(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoCardsError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCardInvalid(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateReadingCard(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveTooBig(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoFreeBlocks(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdatePreSaveFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdatePreSaveFileDelete(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCreateFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSave(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveClosing(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadDoneWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadStart(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoad(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadClosing(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadFileNotFound(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteDoneWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateOverwriteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteDone(cMCOverlayManager* self);
void* cMCOverlayManager_UpdateCardUnusable(void* self);
extern "C" void cMCOverlayManager_UpdateCardDamaged(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFileExists(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCardChanged(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_OnHighlightNextDevice(char* self);
extern "C" void cMCOverlayManager_OnHighlightPrevDevice(cMCOverlayManager* self);

// The player's decoder at 0x434, called through its vtable slots by hand (re-read for every call).
#define DEC_7D8(s) (*(char**)((s) + 0x434))

static inline int call0_7D8(char* s, int n)
{
    char* o = DEC_7D8(s);
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)o;
    return vt[n].fn(o + vt[n].delta);
}
static inline int callI_7D8(char* s, int n, int a)
{
    char* o = DEC_7D8(s);
    sVEntry<int(void*, int)>* vt = *(sVEntry<int(void*, int)>**)o;
    return vt[n].fn(o + vt[n].delta, a);
}
static inline void callV_7D8(char* s, int n, int a)
{
    char* o = DEC_7D8(s);
    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)o;
    vt[n].fn(o + vt[n].delta, a);
}
static inline void callN_7D8(char* s, int n)
{
    char* o = DEC_7D8(s);
    sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)o;
    vt[n].fn(o + vt[n].delta);
}
static inline int isBusy_7D8(char* s) { return call0_7D8(s, 11); }
// Release-build assert: compiles to an empty do { } while (0) (its loop notes still shape the code).
#define ASSERT_7D8(cond) do { } while (0)

static inline bool isState_7D8(char* s, int k)
{
    ASSERT_7D8(k > 0);
    return *(int*)(s + 0x130) == k;
}
static inline bool isActive_7D8(char* s)
{
    return !(isState_7D8(s, 0x14) || isState_7D8(s, 0x24) || isState_7D8(s, 0x25) || isState_7D8(s, 0x1C) || isState_7D8(s, 0xC));
}
static inline bool canLoad_7D8(char* s)
{
    return !(isState_7D8(s, 0x14) || isState_7D8(s, 0x24) || isState_7D8(s, 0x25) || isState_7D8(s, 0x1C)
        || isState_7D8(s, 0xC) || isState_7D8(s, 0x31) || isState_7D8(s, 0x32) || isState_7D8(s, 0x33)
        || isState_7D8(s, 0x34) || isState_7D8(s, 0x35) || isState_7D8(s, 0x36) || *(int*)(s + 0x430) < 0);
}
// Decoder capability tests, written to the player's flag words.
static inline void canPlay_7D8(char* s, int& out)
{
    out = call0_7D8(s, 53) && !callI_7D8(s, 65, *(int*)(s + 0xF8)) && !callI_7D8(s, 66, *(int*)(s + 0xF8));
}
static inline void canSkip_7D8(char* s, int& out)
{
    out = call0_7D8(s, 55) && call0_7D8(s, 58) && !call0_7D8(s, 51) && !call0_7D8(s, 59)
        && callI_7D8(s, 67, *(int*)(s + 0xF8)) && !callI_7D8(s, 65, *(int*)(s + 0xF8)) && !callI_7D8(s, 66, *(int*)(s + 0xF8));
}

/**
 * Runs one frame of the memory-card manager: polls the devices, then runs the handler for the current state.
 *
 * Recomputes the save/load/delete/cancel permissions, notices card changes, regathers device info for the
 * device
 * being polled, handles next/previous-device input in the Save (7) and Load (29) states, and every few frames
 * picks
 * the next device to check. The state numbers dispatched here match On Tour's tMCOverlayManagerState names.
 */
extern "C" void cMCOverlayManager_ManagerLoadSaveUpdate(cMCOverlayManager* self)
{
    if (self->mState == 1)
        return;
    self->mSaveOk = 0;
    self->mLoadOk = 0;
    self->mDeleteOk = 0;
    self->mCancelOk = 0;
    callN_7D8(((char*)self), 3);
    if (isBusy_7D8(((char*)self)) || isActive_7D8(((char*)self))) {
        int st = self->mState;
        if (st == 7 || st == 29) {
            self->mCancelOk = 1;
            canPlay_7D8(((char*)self), self->mSaveOk);
            canSkip_7D8(((char*)self), self->mLoadOk);
            if (self->mState == 29)
                self->mDeleteOk = 0;
            else
                self->mDeleteOk = callI_7D8(((char*)self), 67, self->mFileCursor);
        }
        if (call0_7D8(((char*)self), 63)) {
            self->mCardChanged = 1;
            callN_7D8(((char*)self), 47);
        }
    }
    if (!isBusy_7D8(((char*)self))) {
        if (canLoad_7D8(((char*)self))) {
            cMCOverlayManager* mgr = self;
            int idx = self->mLastCheckDevice;
            if (((&mgr->mDevices[0]))[idx].bPluggedIn == 0) {
                if (callI_7D8(((char*)self), 52, idx)) {
                    if (self->mLastCheckDevice == self->mSelectedDevice) {
                        sVEntry<void(void*, int)>* avt = (sVEntry<void (void*, int)>*)self->mVtbl;
                        avt[1].fn(((char*)self) + avt[1].delta, 49);
                    } else {
                        cMCOverlayManager_GatherDeviceInfo1_v(self, self->mLastCheckDevice);
                    }
                } else {
                    MEM_fill(&((&mgr->mDevices[0]))[self->mLastCheckDevice], 0, 0xE0);
                    if (self->mSelectedDevice == self->mLastCheckDevice && !(self->callbackIsDeviceSelected)()
                        && !(self->callbackIsSavePressed)() && !(self->callbackIsLoadPressed)() && !(self->callbackIsDeletePressed)()
                        && !(self->callbackIsFormatPressed)() && self->mSave == 0 && self->mDelete == 0
                        && !(self->callbackIsLoadPressed)())
                        cMCOverlayManager_OnHighlightNextDevice(((char*)self));
                }
            } else {
                if (!callI_7D8(((char*)self), 52, idx))
                    MEM_fill(&((&mgr->mDevices[0]))[self->mLastCheckDevice], 0, 0xE0);
                else
                    cMCOverlayManager_GatherDeviceInfo1_v(self, self->mLastCheckDevice);
            }
        }
    }
    if (!isBusy_7D8(((char*)self))) {
        if (self->mCheckCard != 0) {
            self->mCheckCard = 0;
            self->mCheckDeviceTicks = 5;
        }
        int st = self->mState;
        if (st == 7 || st == 29) {
            if ((self->callbackIsNextDevicePressed)()) {
                int old = self->mSelectedDevice;
                cMCOverlayManager_OnHighlightNextDevice(((char*)self));
                if (old != self->mSelectedDevice)
                    cMCOverlayManager_InitiateFileCheck(self);
            } else if ((self->callbackIsPrevDevicePressed)()) {
                int old = self->mSelectedDevice;
                cMCOverlayManager_OnHighlightPrevDevice(self);
                if (old != self->mSelectedDevice)
                    cMCOverlayManager_InitiateFileCheck(self);
            }
        }
    }
    int go = 0;
    int st = self->mState;
    if (st == 7 || st == 22 || st == 20 || st == 29 || st == 4 || st == 23 || st == 0x18 || st == 9 || st == 10) {
        (self->mCheckDeviceTicks)--;
        if (self->mCheckCard == 0 && self->mCheckDeviceTicks <= 0)
            go = !isBusy_7D8(((char*)self));
    }
    if (go) {
        bool changed = false;
        for (int i = 0; i < cMCOverlayManager_getMaxDevicesSupported(self); i++) {
            if (callI_7D8(((char*)self), 26, i)) {
                if (self->mLastCheckDevice < i || self->mLastCheckDevice == cMCOverlayManager_getMaxDevicesSupported(self) - 1) {
                    self->mCurrCheckDevice = i;
                    self->mLastCheckDevice = i;
                    if (i != self->mSelectedDevice) changed = true; else changed = false;
                    callV_7D8(((char*)self), 5, i);
                    self->mCheckCard = 1;
                    callV_7D8(((char*)self), 27, self->mCurrCheckDevice);
                    break;
                }
            }
        }
        if (changed)
            cMCOverlayManager_checkDevice(self, self->mSelectedDevice);
    }
    switch (self->mState) {
    case 49:
        cMCOverlayManager_UpdateInitCardsCheck(self);
        break;
    case 50:
        cMCOverlayManager_UpdateInitCardsCheckDone(self);
        break;
    case 53:
        cMCOverlayManager_UpdateInitFileCheckWait(self);
        break;
    case 54:
        cMCOverlayManager_UpdateInitFileCheckDevice(self);
        break;
    case 51:
        cMCOverlayManager_UpdateInitFileCheck(self);
        break;
    case 52:
        cMCOverlayManager_UpdateInitFileCheckDone(self);
        break;
    case 2:
        cMCOverlayManager_UpdateCheckCardsWait(self);
        break;
    case 3:
        cMCOverlayManager_UpdateCheckCards(self);
        break;
    case 4:
        cMCOverlayManager_UpdateNoCardsError(self);
        break;
    case 5:
        cMCOverlayManager_UpdateCardInvalid(self);
        break;
    case 6:
        cMCOverlayManager_UpdateReadingCard(self);
        break;
    case 10:
        cMCOverlayManager_UpdateFormatConfirm(self);
        break;
    case 11:
        cMCOverlayManager_UpdateFormatProgress(self);
        break;
    case 12:
        cMCOverlayManager_UpdateFormatFailed(self);
        break;
    case 22:
        cMCOverlayManager_UpdateSaveCorrupt(self);
        break;
    case 19:
        cMCOverlayManager_UpdateSaveDone(self);
        break;
    case 20:
        cMCOverlayManager_UpdateSaveFailed(self);
        break;
    case 21:
        cMCOverlayManager_UpdateSaveTooBig(self);
        break;
    case 23:
        cMCOverlayManager_UpdateNoFreeBlocks(self);
        break;
    case 13:
        cMCOverlayManager_UpdatePreSaveFile(self);
        break;
    case 14:
        cMCOverlayManager_UpdatePreSaveFileDelete(self);
        break;
    case 15:
        cMCOverlayManager_UpdateCreateFile(self);
        break;
    case 16:
        cMCOverlayManager_UpdateSaveFile(self);
        break;
    case 17:
        cMCOverlayManager_UpdateSaveProgress(self);
        break;
    case 7:
        cMCOverlayManager_UpdateSave(self);
        break;
    case 18:
        cMCOverlayManager_UpdateSaveClosing(self);
        break;
    case 34:
        cMCOverlayManager_UpdateLoadDoneWait(self);
        break;
    case 35:
        cMCOverlayManager_UpdateLoadDone(self);
        break;
    case 36:
        cMCOverlayManager_UpdateLoadFailed(self);
        break;
    case 37:
        cMCOverlayManager_UpdateLoadCorrupt(self);
        break;
    case 30:
        cMCOverlayManager_UpdateLoadConfirm(self);
        break;
    case 31:
        cMCOverlayManager_UpdateLoadStart(self);
        break;
    case 32:
        cMCOverlayManager_UpdateLoadProgress(self);
        break;
    case 29:
        cMCOverlayManager_UpdateLoad(self);
        break;
    case 33:
        cMCOverlayManager_UpdateLoadClosing(self);
        break;
    case 38:
        cMCOverlayManager_UpdateLoadFileNotFound(self);
        break;
    case 24:
        cMCOverlayManager_UpdateDeleteConfirm(self);
        break;
    case 25:
        cMCOverlayManager_UpdateDeleteProgress(self);
        break;
    case 26:
        cMCOverlayManager_UpdateDeleteDoneWait(self);
        break;
    case 27:
        cMCOverlayManager_UpdateDeleteDone(self);
        break;
    case 28:
        cMCOverlayManager_UpdateDeleteFailed(self);
        break;
    case 9:
        cMCOverlayManager_UpdateOverwriteConfirm(self);
        break;
    case 39:
        cMCOverlayManager_UpdateInitFormatConfirm(self);
        break;
    case 40:
        cMCOverlayManager_UpdateInitFormatProgress(self);
        break;
    case 41:
        cMCOverlayManager_UpdateInitFormatFailed(self);
        break;
    case 42:
        cMCOverlayManager_UpdateInitFormatDone(self);
        break;
    case 43:
        cMCOverlayManager_UpdateInitDeleteConfirm(self);
        break;
    case 44:
        cMCOverlayManager_UpdateInitDeleteProgress(self);
        break;
    case 45:
        cMCOverlayManager_UpdateInitDeleteFailed(self);
        break;
    case 46:
        cMCOverlayManager_UpdateInitDeleteDone(self);
        break;
    case 47:
        cMCOverlayManager_UpdateCardUnusable(self);
        break;
    case 48:
        cMCOverlayManager_UpdateCardDamaged(self);
        break;
    case 55:
        cMCOverlayManager_UpdateFileExists(self);
        break;
    case 56:
        cMCOverlayManager_UpdateCardChanged(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitCardsCheck);
#ifdef SKIP_ASM
struct sMovieVEntryE268a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE268b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits for the initial card check to finish, then moves to InitCardsCheckDone (state 50).
 *
 * The card is idle once its vtable slot 11 (busy) returns 0; the state change goes through the virtual
 * SetState
 * (vtable slot 1).
 */
extern "C" void cMCOverlayManager_UpdateInitCardsCheck(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE268a* vt = *(sMovieVEntryE268a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        sMovieVEntryE268b* vt2 = (sMovieVEntryE268b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 50);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitCardsCheckDone__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the memory-card overlay's init-cards-check-done update has an empty body.
 */
void cMCOverlayManager_UpdateInitCardsCheckDone(void* self)
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckWait);
#ifdef SKIP_ASM
struct sMovieVEntryE2C8a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE2C8b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits for the card to go idle, then moves to InitFileCheck (state 51).
 */
extern "C" void cMCOverlayManager_UpdateInitFileCheckWait(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE2C8a* vt = *(sMovieVEntryE2C8a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        sMovieVEntryE2C8b* vt2 = (sMovieVEntryE2C8b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 51);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckDevice);
#ifdef SKIP_ASM
struct sMovieVEntryE320a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE320b {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sMovieVEntryE320c {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);

/**
 * Once the card is idle, starts the file check on the selected device or reports that no card is present.
 *
 * If the card says the selected device is not present (card slot 52), moves to CheckCardsWait (2) and calls
 * callbackDisplayNoCards; otherwise starts InitiateFileCheck() and calls callbackDisplayNoFiles.
 */
extern "C" void cMCOverlayManager_UpdateInitFileCheckDevice(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE320a* vt = *(sMovieVEntryE320a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        void* card = self->mpMemCard;
        sMovieVEntryE320b* vtb = *(sMovieVEntryE320b**)card;
        if (vtb[52].fn((char*)card + vtb[52].delta, self->mSelectedDevice) == 0) {
            sMovieVEntryE320c* vt2 = (sMovieVEntryE320c*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 2);
            (self->callbackDisplayNoCards)();
        } else {
            cMCOverlayManager_InitiateFileCheck(self);
            (self->callbackDisplayNoFiles)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFileExists);
#ifdef SKIP_ASM
struct sMovieVEntryE3C0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE3C0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits on the "file exists" message: once the card is idle, cancelling returns to CheckCards (state 3).
 */
extern "C" void cMCOverlayManager_UpdateFileExists(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE3C0a* vt = *(sMovieVEntryE3C0a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            sMovieVEntryE3C0b* vt2 = (sMovieVEntryE3C0b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardChanged);
#ifdef SKIP_ASM
struct sMovieVEntryE428 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);

/**
 * Waits on the "card changed" message: once the card is idle, cancelling aborts the operation and rechecks
 * the card.
 *
 * Calls CancelCardOp(0), clears mCardChanged and calls RecheckCard().
 */
extern "C" void cMCOverlayManager_UpdateCardChanged(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE428* vt = *(sMovieVEntryE428**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
            self->mCardChanged = 0;
            cMCOverlayManager_RecheckCard(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheck);
#ifdef SKIP_ASM
struct sMovieVEntryE498a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE498b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits for the initial file check to finish, then moves to InitFileCheckDone (state 52).
 */
extern "C" void cMCOverlayManager_UpdateInitFileCheck(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE498a* vt = *(sMovieVEntryE498a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        sMovieVEntryE498b* vt2 = (sMovieVEntryE498b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 52);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckDone__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the memory-card overlay's init-file-check-done update has an empty body.
 */
void cMCOverlayManager_UpdateInitFileCheckDone(void* self)
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCheckCardsWait);
#ifdef SKIP_ASM
struct sMovieVEntryE4F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits out the memory-card check display delay, then moves on with state code 3.
 *
 * Decrements mDisplayWaitTicks (SetAction(1) sets it to 60); once it was already 0 or less, passes code 3 to
 * the manager's virtual state handler (vtable slot 1, cMCOverlayManager_SetState).
 */
extern "C" void cMCOverlayManager_UpdateCheckCardsWait(cMCOverlayManager* self)
{
    if ((self->mDisplayWaitTicks)-- <= 0) {
        sMovieVEntryE4F8* vt = (sMovieVEntryE4F8*)self->mVtbl;
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCheckCards);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);

struct sMovieVEntryE540a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryE540b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int Pad_E540(void* self, int slot)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntryE540a* vt = *(sMovieVEntryE540a**)o;
    return vt[slot].fn((char*)o + vt[slot].delta);
}

static inline void Post_E540(void* self, int code)
{
    sMovieVEntryE540b* vt = *(sMovieVEntryE540b**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, code);
}

static inline int Ready_E540(void* self)
{
    return Pad_E540(self, 55) && Pad_E540(self, 58) && !Pad_E540(self, 51) && !Pad_E540(self, 59);
}

#define S_E540 ((char*)self)

/**
 * Polls the memory cards while checking them and passes the next state code to the virtual state handler.
 *
 * Waits while the card object is busy (its virtual slot 11). A pending save, delete or load press means the
 * user wants to leave: a changed card with mShowCardChangeNotify set gives 0x38. On a device state change it
 * raises the no-card error (TriggerPollNoCardError) when polling for it, posts 5 (state bit 1) or 0x27
 * (unformatted, when mPollForUnformatted), or starts a file check when a card arrives or its state clears.
 * Without a pending action it rechecks a changed card or posts 0x31 for no card. Otherwise it posts 6 when the
 * card object's slots 55 and 58 report true and 51 and 59 false, else 7 when saving or 0x1D when not.
 */
extern "C" void cMCOverlayManager_UpdateCheckCards(cMCOverlayManager* self)
{
    if (Pad_E540(self, 11))
        return;
    int quit = 0;
    if (self->mSave || self->mDelete || (self->callbackIsLoadPressed)())
        quit = 1;
    if (self->mShowCardChangeNotify && self->mCardChanged && quit) {
        Post_E540(self, 0x38);
        return;
    }
    int held = self->mCurrDeviceStateLast;
    if (cMCOverlayManager_updateDeviceStateMonitor(self)) {
        if ((self->mPollForNoCard || quit) && (self->mCurrDeviceStateLast & 8)) {
            cMCOverlayManager_TriggerPollNoCardError(self);
            return;
        }
        if (!(self->mCurrDeviceStateLast & 8)) {
            if (self->mCurrDeviceStateLast & 1) {
                Post_E540(self, 5);
                return;
            }
            if (self->mPollForUnformatted && (self->mCurrDeviceStateLast & 0x10)) {
                Post_E540(self, 39);
                return;
            }
        }
        int rel;
        if ((!(self->mCurrDeviceStateLast & 8) && (held & 8)) || (rel = held & ~4, self->mCurrDeviceStateLast == 0 && rel)) {
            cMCOverlayManager_InitiateFileCheck(self);
            return;
        }
    }
    if (!quit) {
        if (self->mCardChanged) {
            self->mCardChanged = 0;
            cMCOverlayManager_RecheckCard(self);
            return;
        }
        if (self->mCurrDeviceStateLast & 8) {
            Post_E540(self, 49);
            return;
        }
    }
    if (self->mSave) {
        if (Pad_E540(self, 55) && Pad_E540(self, 58) && !Pad_E540(self, 51) && !Pad_E540(self, 59))
            Post_E540(self, 6);
        else
            Post_E540(self, 7);
    } else {
        if (Pad_E540(self, 55) && Pad_E540(self, 58) && !Pad_E540(self, 51) && !Pad_E540(self, 59))
            Post_E540(self, 6);
        else
            Post_E540(self, 29);
    }
}
#undef S_E540
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateNoCardsError);
#ifdef SKIP_ASM
struct sMovieVEntryE820 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "no memory card" error: once the card is idle, cancel aborts the operation and retry retries
 * it.
 */
extern "C" void cMCOverlayManager_UpdateNoCardsError(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryE820* vt = *(sMovieVEntryE820**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023E8A0);
#ifdef SKIP_ASM
struct sMovieVEntry_E8A0 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMovieDelegate_E8A0 {
    char pad[8];
    short delta;
    short pad2;
    void (*fn)(void*, int);
};

static inline int vq_E8A0(char* s, int k)
{
    char* o = *(char**)(s + 0x434);
    sMovieVEntry_E8A0* vt = *(sMovieVEntry_E8A0**)o;
    return vt[k].fn(o + vt[k].delta);
}

static inline void notify_E8A0(char* s, int v)
{
    sMovieDelegate_E8A0* d = *(sMovieDelegate_E8A0**)(s + 0x748);
    d->fn(s + d->delta, v);
}

extern "C" void func_0023E8A0(void* self, int flag)
{
    char* manager = (char*)self;

    if (vq_E8A0(manager, 53) == 0) {
        if (vq_E8A0(manager, 49) != 0 || vq_E8A0(manager, 51) != 0 || vq_E8A0(manager, 59) != 0) {
            notify_E8A0(manager, 5);
        } else {
            int ok = *(int*)(manager + 0x130) != 4;
            if (ok) {
                notify_E8A0(manager, 4);
            }
        }
    } else {
        if (vq_E8A0(manager, 55) != 0 && vq_E8A0(manager, 58) != 0 && vq_E8A0(manager, 51) == 0 && vq_E8A0(manager, 59) == 0) {
            if (flag != 0) {
                notify_E8A0(manager, 6);
            }
        } else if (*(int*)(manager + 0xBC) != 0) {
            notify_E8A0(manager, 7);
        } else {
            notify_E8A0(manager, 29);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_CancelCardOp);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

struct sMovieVEntryEA30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Abandons the current memory-card operation and returns the manager to Done (state 1).
 *
 * A save in progress is reported as failed through WriteFailed(); also clears the global save-in-progress
 * flag,
 * mDelete and the buffered format press, and marks the device state as acknowledged.
 *
 * @param showFailMessage  passed to WriteFailed() when a save is being abandoned: show the write-failed popup
 */
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int showFailMessage)
{
    int on = self->mSave;
    cMCOverlayManager_mSaveInProgress = 0;
    if (on != 0) {
        self->mSave = 0;
        cMCOverlayManager_WriteFailed(self, showFailMessage);
    }
    self->mDelete = 0;
    self->mCurrDeviceStateAck = 1;
    self->mFormatPressedBuffered = 0;
    sMovieVEntryEA30* vt = (sMovieVEntryEA30*)self->mVtbl;
    vt[1].fn(((char*)self) + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_RetryCardOp);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_hasCardStatusCriticalError(void* self);
extern "C" void cMCOverlayManager_setSelectedDevice(void* self);
void func_0023FB18(void* self, int val);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);
extern int cMCOverlayManager_mSaveInProgress;

struct sMovieVEntryEA90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sMovieVEntryEA90b {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Retries the operation that failed: a save restarts from CheckCardsWait, anything else rechecks the card.
 *
 * For a save, an unfinished card write is first stopped (card vtable slot 44). Otherwise, with mRetryToInit
 * set it
 * reselects device 0 and returns to InitCardsCheck (49); else it restarts the file check when the device
 * state
 * shows no problem besides a full card (cMCOverlayManager_hasCardStatusCriticalError()), or rechecks the card.
 */
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self)
{
    if (self->mSave != 0) {
        int playing = self->mWriteInProgress;
        cMCOverlayManager_mSaveInProgress = 0;
        if (playing != 0) {
            void* memCard = self->mpMemCard;
            sMovieVEntryEA90b* vt2 = *(sMovieVEntryEA90b**)memCard;
            vt2[44].fn((char*)memCard + vt2[44].delta);
        }
        sMovieVEntryEA90* vt = (sMovieVEntryEA90*)self->mVtbl;
        vt[1].fn(((char*)self) + vt[1].delta, 2);
    } else {
        self->mCurrDeviceStateAck = 0;
        if (self->mRetryToInit != 0) {
            func_0023FB18(self, 0);
            cMCOverlayManager_setSelectedDevice(self);
            sMovieVEntryEA90* vt = (sMovieVEntryEA90*)self->mVtbl;
            vt[1].fn(((char*)self) + vt[1].delta, 49);
        } else if (cMCOverlayManager_hasCardStatusCriticalError(self) == 0) {
            cMCOverlayManager_InitiateFileCheck(self);
        } else {
            cMCOverlayManager_RecheckCard(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_TriggerPollNoCardError);
#ifdef SKIP_ASM
struct sMovieVEntry2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Reports a missing memory card by passing code 4 to the manager's virtual state handler (vtable slot 1).
 */
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 4);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardInvalid);
#ifdef SKIP_ASM
struct sMovieVEntryEB80 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "invalid card" error: once the card is idle, cancel aborts (with the fail message) and retry
 * retries.
 */
extern "C" void cMCOverlayManager_UpdateCardInvalid(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryEB80* vt = *(sMovieVEntryEB80**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 1);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateReadingCard);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
// PORT: the target calls cMCOverlayManager_GatherDeviceInfo as a void function here (the unit defines it returning void*).
void cMCOverlayManager_GatherDeviceInfo_v(void* self) __asm__("cMCOverlayManager_GatherDeviceInfo__FPv");

struct sMovieVEntryEC00 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMovieVEntryEC00b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int ovCall_EC00(char* s, int slot)
{
    void* o = *(void**)(s + 0x434);
    sMovieVEntryEC00* vt = *(sMovieVEntryEC00**)o;
    return vt[slot].fn((char*)o + vt[slot].delta);
}

static inline void post_EC00(char* s, int code)
{
    sMovieVEntryEC00b* vt = *(sMovieVEntryEC00b**)(s + 0x748);
    vt[1].fn(s + vt[1].delta, code);
}

/**
 * Waits while the card is being read, then picks the next state from the device status and the pending
 * operation.
 *
 * After the display delay and once the card is idle: a card change goes to CardChanged (56) or back to
 * ReadingCard; a save goes to SaveFailed (20), SaveTooBig (21) or Save (7); a delete or load goes to the
 * no-card
 * error, LoadFailed (36), CardInvalid (5) or Load (29). Device status bits come from mCurrDeviceStateLast
 * (1 error, 8 no card).
 */
extern "C" void cMCOverlayManager_UpdateReadingCard(cMCOverlayManager* self)
{
    int ready = cMCOverlayManager_updateDeviceStateMonitor(self);
    if ((self->mDisplayWaitTicks)-- > 0)
        return;
    if (ovCall_EC00(((char*)self), 11))
        return;
    if (self->mCardChanged) {
        int busy = 0;
        if (self->mSave || self->mDelete || (self->callbackIsLoadPressed)())
            busy = 1;
        if (self->mShowCardChangeNotify && busy) {
            post_EC00(((char*)self), 0x38);
            return;
        }
        if (ovCall_EC00(((char*)self), 55) && ovCall_EC00(((char*)self), 58) && !ovCall_EC00(((char*)self), 51) && !ovCall_EC00(((char*)self), 59)) {
            post_EC00(((char*)self), 6);
            return;
        }
    }
    if (self->mSave) {
        int deviceState = self->mCurrDeviceStateLast;
        if (!(deviceState & 8)) {
            if (deviceState & 1) {
                post_EC00(((char*)self), 20);
                return;
            }
            if (ovCall_EC00(((char*)self), 55) && ovCall_EC00(((char*)self), 58) && !ovCall_EC00(((char*)self), 51) && !ovCall_EC00(((char*)self), 59) && !ovCall_EC00(((char*)self), 13)) {
                int neededSpace = ovCall_EC00(((char*)self), 23);
                if (ovCall_EC00(((char*)self), 20) < neededSpace || !ovCall_EC00(((char*)self), 25)) {
                    post_EC00(((char*)self), 21);
                    return;
                }
            }
        } else if (ready) {
            cMCOverlayManager_TriggerPollNoCardError(self);
            return;
        }
        cMCOverlayManager_GatherDeviceInfo_v(self);
        post_EC00(((char*)self), 7);
        return;
    }
    {
        int busy = 0;
        if (self->mDelete || (self->callbackIsLoadPressed)())
            busy = 1;
        if (self->mPollForNoCard || busy) {
            if (self->mCurrDeviceStateLast & 8) {
                cMCOverlayManager_TriggerPollNoCardError(self);
                return;
            }
        } else if (self->mCurrDeviceStateLast & 8) {
            goto fail;
        }
        if (self->mCurrDeviceStateLast & 1) {
            if ((self->callbackIsLoadPressed)())
                post_EC00(((char*)self), 36);
            else
                post_EC00(((char*)self), 5);
            return;
        }
    fail:
        cMCOverlayManager_GatherDeviceInfo_v(self);
        post_EC00(((char*)self), 29);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatConfirm);
#ifdef SKIP_ASM
class cMovieSub434 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual void v05(int);
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual int v70();
    virtual int v71();
    virtual int v72();
    virtual int v73();
    virtual int v74();
    virtual int v75();
    virtual int v76();
    virtual int v77();
    virtual int v78();
    virtual int v79();
    virtual int v80();
};
class cMoviePlayerVt {
public:
    char pad[0x748];
    virtual void v01(int);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the format-card confirmation: starts the format once confirmed, or reports a missing or failed
 * card.
 *
 * When no recheck is pending: a missing card goes to NoCardsError (4), a card error to FormatFailed (12), and
 * a
 * buffered format press to FormatProgress (11). Cancel aborts; the format button is buffered in
 * mFormatPressedBuffered for the next frame.
 */
extern "C" void cMCOverlayManager_UpdateFormatConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (self->mCheckCard == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if (((cMovieSub434*)self->mpMemCard)->v09() != 0) {
            me->v01(0xC);
            return;
        }
        if (self->mFormatPressedBuffered != 0) {
            self->mFormatPressedBuffered = 0;
            me->v01(11);
            return;
        }
    }
    if ((self->callbackIsCancelPressed)() != 0) {
        cMCOverlayManager_CancelCardOp(self, 0);
        return;
    }
    if ((self->callbackIsFormatPressed)() != 0) {
        self->mFormatPressedBuffered = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatProgress);
#ifdef SKIP_ASM
/**
 * Waits for a card format to finish, then moves to FormatFailed (12) on error or PreSaveFile (13) on success.
 */
extern "C" void cMCOverlayManager_UpdateFormatProgress(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int waitTicks = (self->mDisplayWaitTicks)--;
    if (waitTicks <= 0) {
        if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
            if (((cMovieSub434*)self->mpMemCard)->v09() != 0) {
                me->v01(0xC);
            } else {
                me->v01(13);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatFailed);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Handles the format-failed card state: once cancel is pressed, abandons the operation and rechecks the card.
 *
 * When callbackIsCancelPressed() returns nonzero: cMCOverlayManager_CancelCardOp(self, 0),
 * cMCOverlayManager_checkDevice() on mSelectedDevice, and mCheckCard = 1. Otherwise it keeps waiting.
 */
extern "C" void cMCOverlayManager_UpdateFormatFailed(cMCOverlayManager* self)
{
    if ((self->callbackIsCancelPressed)() != 0) {
        cMCOverlayManager_CancelCardOp(self, 0);
        cMCOverlayManager_checkDevice(self, self->mSelectedDevice);
        self->mCheckCard = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveFailed);
#ifdef SKIP_ASM
struct sMovieVEntryF150 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "save failed" message: once the card is idle, cancel aborts and retry restarts the save.
 */
extern "C" void cMCOverlayManager_UpdateSaveFailed(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryF150* vt = *(sMovieVEntryF150**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            self->mSave = 1;
            cMCOverlayManager_RetryCardOp(self);
            self->mCurrDeviceStateAck = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveTooBig);
#ifdef SKIP_ASM
struct sMovieVEntryF1D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "not enough space" message: once the card is idle, cancel aborts and retry retries the
 * operation.
 */
extern "C" void cMCOverlayManager_UpdateSaveTooBig(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryF1D8* vt = *(sMovieVEntryF1D8**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateNoFreeBlocks);
#ifdef SKIP_ASM
struct sMovieVEntryF258 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "no free blocks" message: once the card is idle, retry retries and cancel aborts.
 */
extern "C" void cMCOverlayManager_UpdateNoFreeBlocks(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryF258* vt = *(sMovieVEntryF258**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        } else if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveDone);
#ifdef SKIP_ASM
/**
 * Finishes a completed save by passing code 1 to the manager's virtual state handler (vtable slot 1).
 */
extern "C" void cMCOverlayManager_UpdateSaveDone(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdatePreSaveFile);
#ifdef SKIP_ASM
struct sMovieVEntryF308a {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sMovieVEntryF308b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Decides whether a save must first delete the existing file in the chosen slot.
 *
 * If the card reports the slot in use (card slots 67 or 65 for mFileCursor), moves to PreSaveFileDelete (14),
 * otherwise straight to CreateFile (15).
 */
extern "C" void cMCOverlayManager_UpdatePreSaveFile(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryF308a* vt = *(sMovieVEntryF308a**)memCard;
    void* card;
    sMovieVEntryF308a* vtb;
    if (vt[67].fn((char*)memCard + vt[67].delta, self->mFileCursor) != 0
        || (card = (void*)self->mpMemCard, vtb = *(sMovieVEntryF308a**)card,
            vtb[65].fn((char*)card + vtb[65].delta, self->mFileCursor) != 0)) {
        sMovieVEntryF308b* vt2 = (sMovieVEntryF308b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 0xE);
    } else {
        sMovieVEntryF308b* vt2 = (sMovieVEntryF308b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdatePreSaveFileDelete);
#ifdef SKIP_ASM
struct sMovieVEntryF3A0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryF3A0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits for the old save file to be deleted, then moves to CreateFile (15), or SaveFailed (20) on a card
 * error.
 */
extern "C" void cMCOverlayManager_UpdatePreSaveFileDelete(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryF3A0a* vt = *(sMovieVEntryF3A0a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        void* card = self->mpMemCard;
        sMovieVEntryF3A0a* vtb = *(sMovieVEntryF3A0a**)card;
        if (vtb[73].fn((char*)card + vtb[73].delta) != 0) {
            sMovieVEntryF3A0b* vt2 = (sMovieVEntryF3A0b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 20);
        } else {
            sMovieVEntryF3A0b* vt2 = (sMovieVEntryF3A0b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 0xF);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCreateFile);
#ifdef SKIP_ASM
/**
 * Waits for the save file to be created, then moves to SaveFile (16), or reports the error.
 *
 * A card error goes to FileExists (55) when card slot 74 says so, otherwise to SaveFailed (20).
 */
extern "C" void cMCOverlayManager_UpdateCreateFile(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            if (((cMovieSub434*)self->mpMemCard)->v74() != 0) {
                me->v01(55);
            } else {
                me->v01(20);
            }
        } else {
            me->v01(0x10);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveFile);
#ifdef SKIP_ASM
struct sMovieVEntryF4F8a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryF4F8b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

/**
 * Updates the save progress and moves to SaveProgress (17), or fails the write when the card reports an
 * error.
 */
extern "C" void cMCOverlayManager_UpdateSaveFile(cMCOverlayManager* self)
{
    void* memCard;
    sMovieVEntryF4F8a* vt;
    self->mProgress = (float)self->mSavedSize / (float)self->mSize;
    memCard = (void*)self->mpMemCard;
    vt = *(sMovieVEntryF4F8a**)memCard;
    if (vt[73].fn((char*)memCard + vt[73].delta) != 0) {
        cMCOverlayManager_WriteFailed(self, 1);
    } else {
        sMovieVEntryF4F8b* vt2 = (sMovieVEntryF4F8b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 17);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveProgress);
#ifdef SKIP_ASM
extern "C" void func_0023CC20(void* self);
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

class cMovieDecoder_23F578 {
public:
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
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int isPlaying();          // slot 42 (0x150)
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();         // slot 73 (0x248)
};
struct sMovieVEntry1_23F578 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Feeds the save data to the card until it is all written, then finishes the write and moves to SaveClosing
 * (18).
 *
 * Updates mProgress = mSavedSize / mSize; while data remains, callbackWriteUpdate gets the remaining byte
 * count
 * and a 0 result fails the write, as does a card error.
 */
extern "C" void cMCOverlayManager_UpdateSaveProgress(cMCOverlayManager* self)
{
    self->mDisplayWaitTicks -= 1;
    self->mProgress = (float)self->mSavedSize / (float)self->mSize;
    if (((cMovieDecoder_23F578*)self->mpMemCard)->isFinished() != 0) {
        cMCOverlayManager_WriteFailed(self, 1);
        return;
    }
    if (((cMovieDecoder_23F578*)self->mpMemCard)->isPlaying() == 1) {
        int savedSize = self->mSavedSize;
        int totalSize = self->mSize;
        if (savedSize < totalSize) {
            if ((self->callbackWriteUpdate)(totalSize - savedSize) == 0) {
                cMCOverlayManager_WriteFailed(self, 1);
            }
        } else {
            func_0023CC20(((char*)self));
            (self->callbackWriteEnd)();
            sMovieVEntry1_23F578* vt1 = (sMovieVEntry1_23F578*)self->mVtbl;
            vt1[1].fn(((char*)self) + vt1[1].delta, 18);
        }
    } else {
        if (((cMovieDecoder_23F578*)self->mpMemCard)->isFinished() != 0) {
            cMCOverlayManager_WriteFailed(self, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSave);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);
extern "C" int func_003DBDB0(void* a, void* b);

struct sMovieVEntryF698 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMovieVEntryF698i {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sMovieVEntryF698ip {
    short delta;
    short index;
    void (*fn)(void*, int, void*);
};
struct sMovieVEntryF698b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int ovCall_F698(char* s, int slot)
{
    void* o = *(void**)(s + 0x434);
    sMovieVEntryF698* vt = *(sMovieVEntryF698**)o;
    return vt[slot].fn((char*)o + vt[slot].delta);
}

static inline int ovCallI_F698(char* s, int slot, int a)
{
    void* o = *(void**)(s + 0x434);
    sMovieVEntryF698i* vt = *(sMovieVEntryF698i**)o;
    return vt[slot].fn((char*)o + vt[slot].delta, a);
}

static inline void ovCallIP_F698(char* s, int slot, int a, void* p)
{
    void* o = *(void**)(s + 0x434);
    sMovieVEntryF698ip* vt = *(sMovieVEntryF698ip**)o;
    vt[slot].fn((char*)o + vt[slot].delta, a, p);
}

static inline void post_F698(char* s, int code)
{
    sMovieVEntryF698b* vt = *(sMovieVEntryF698b**)(s + 0x748);
    vt[1].fn(s + vt[1].delta, code);
}

/**
 * Runs the save screen (state Save, 7): reacts to card status changes and starts a save, overwrite or delete.
 *
 * Card problems found by updateDeviceStateMonitor() send it to the no-card error, InitFormatConfirm (39),
 * CardDamaged (48), CardInvalid (5) or NoFreeBlocks (23). Pressing save checks the card is usable and has
 * room,
 * refuses a name already used by another slot (FileExists, 55), and goes to OverwriteConfirm (9) for a used
 * slot
 * or PreSaveFile (13) for an empty one; delete goes to DeleteConfirm (24) and cancel to Done (1).
 */
extern "C" void cMCOverlayManager_UpdateSave(cMCOverlayManager* self)
{
    if (self->mCheckCard)
        return;
    if (self->mShowCardChangeNotify && self->mCardChanged) {
        post_F698(((char*)self), 0x38);
        return;
    }
    int ready = cMCOverlayManager_updateDeviceStateMonitor(self);
    if (!(self->callbackIsSavePressed)() && ready) {
        int deviceState = self->mCurrDeviceStateLast;
        if (deviceState & 8) {
            cMCOverlayManager_TriggerPollNoCardError(self);
            return;
        }
        if (deviceState & 0x10) {
            post_F698(((char*)self), 39);
            return;
        }
        if (deviceState & 2) {
            post_F698(((char*)self), 0x30);
            return;
        }
        if (deviceState & 1) {
            post_F698(((char*)self), 5);
            return;
        }
        if (deviceState & 4) {
            post_F698(((char*)self), 23);
            return;
        }
    }
    if (self->mCardChanged) {
        self->mCardChanged = 0;
        cMCOverlayManager_RecheckCard(self);
        return;
    }
    if ((self->callbackIsCancelPressed)()) {
        post_F698(((char*)self), 1);
        return;
    }
    if ((self->callbackIsSavePressed)()) {
        self->mFlash = 1.0f;
        self->mFlashStep = 0.016669999808073044f;
        if (self->mSaveOk) {
            if (ovCall_F698(((char*)self), 55) && ovCall_F698(((char*)self), 58) && !ovCall_F698(((char*)self), 51) && !ovCall_F698(((char*)self), 59)) {
                if (!ovCallI_F698(((char*)self), 67, self->mFileCursor)) {
                    int neededSpace = ovCall_F698(((char*)self), 23);
                    if (ovCall_F698(((char*)self), 20) < neededSpace || !ovCall_F698(((char*)self), 25)) {
                        post_F698(((char*)self), 21);
                        return;
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (ovCallI_F698(((char*)self), 67, i)) {
                        char buf[0x200];
                        ovCallIP_F698(((char*)self), 14, i, buf);
                        if (func_003DBDB0(((char*)self) + 0x276, buf) == 0 && i != self->mFileCursor) {
                            post_F698(((char*)self), 55);
                            return;
                        }
                    }
                }
                if (ovCallI_F698(((char*)self), 67, self->mFileCursor)) {
                    ovCallIP_F698(((char*)self), 14, self->mFileCursor, ((char*)self) + 0x234);
                    post_F698(((char*)self), 9);
                    return;
                }
                post_F698(((char*)self), 13);
            } else {
                post_F698(((char*)self), 10);
                return;
            }
        } else if (ovCallI_F698(((char*)self), 65, self->mFileCursor)) {
            post_F698(((char*)self), 22);
        } else if (ovCallI_F698(((char*)self), 66, self->mFileCursor)) {
            post_F698(((char*)self), 23);
        } else if (!ovCall_F698(((char*)self), 53)) {
            post_F698(((char*)self), 20);
        }
    }
    if ((self->callbackIsDeletePressed)() && self->mDeleteOk) {
        ovCallIP_F698(((char*)self), 14, self->mFileCursor, ((char*)self) + 0x234);
        post_F698(((char*)self), 0x18);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setSelectedDevice);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Makes the highlighted memory card device the selected one and checks it.
 *
 * When the device index (+0x428) differs from the last selected one (+0x444), stores it there and clears the
 * card status words (+0x43C, +0x440); then calls cMCOverlayManager_checkDevice for it.
 */
extern "C" void cMCOverlayManager_setSelectedDevice(void* self)
{
    int cur = *(int*)((char*)self + 0x428);
    if (*(int*)((char*)self + 0x444) != cur) {
        *(int*)((char*)self + 0x444) = cur;
        *(int*)((char*)self + 0x43C) = 0;
        *(int*)((char*)self + 0x440) = 0;
    }
    cMCOverlayManager_checkDevice((cMCOverlayManager*)self, *(int*)((char*)self + 0x428));
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_0023FB18__FPvi);
#ifdef SKIP_ASM
void func_0023FB18(void* self, int val)
{
    *(int*)((char*)self + 0x428) = val;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_checkDevice);
#ifdef SKIP_ASM
/**
 * Starts a status check of one memory-card device: records it in mCurrCheckDevice and asks the card to check
 * it.
 *
 * @param iSlot  device (card slot) to check
 */
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int iSlot)
{
    self->mCurrCheckDevice = iSlot;
    void* obj = self->mpMemCard;
    sMovieVEntry2* vt = *(sMovieVEntry2**)obj;
    vt[5].fn((char*)obj + vt[5].delta, iSlot);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ReturnToDevSelectScreen);
#ifdef SKIP_ASM
struct sMovieVEntryFB58 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int v);

/**
 * Returns the memory-card screens to device selection, dropping any pending save, delete or format.
 *
 * Resets mLastCheckDevice to -1 and mCurrCheckDevice to 0, clears mSave, mDelete and mFormatPressedBuffered,
 * passes code 0 to the virtual state handler (vtable slot 1), then calls cMCOverlayManager_SetAction(self, 1).
 *
 * @return no meaningful value (cMCOverlayManager_SetAction returns nothing)
 */
extern "C" void* cMCOverlayManager_ReturnToDevSelectScreen(cMCOverlayManager* self)
{
    self->mLastCheckDevice = -1;
    self->mCurrCheckDevice = 0;
    self->mSave = 0;
    self->mDelete = 0;
    self->mFormatPressedBuffered = 0;
    sMovieVEntryFB58* vt = (sMovieVEntryFB58*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 0);
    return cMCOverlayManager_SetAction(self, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveClosing);
#ifdef SKIP_ASM
/**
 * Waits for the save file to close: on success calls callbackSaveSuccess and moves to SaveDone (19).
 *
 * A card error goes to SaveFailed (20) instead; the display delay (mDisplayWaitTicks) must also have run out.
 */
extern "C" void cMCOverlayManager_UpdateSaveClosing(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    (self->mDisplayWaitTicks)--;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            me->v01(20);
            return;
        }
        if (self->mDisplayWaitTicks <= 0) {
            (self->callbackSaveSuccess)();
            me->v01(19);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadFailed);
#ifdef SKIP_ASM
struct sMovieVEntryFC70a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntryFC70b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Handles the "load failed" message: once the card is idle, cancel goes to CheckCards (3), retry to
 * ReadingCard (6).
 */
extern "C" void cMCOverlayManager_UpdateLoadFailed(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryFC70a* vt = *(sMovieVEntryFC70a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            sMovieVEntryFC70b* vt2 = (sMovieVEntryFC70b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 3);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            sMovieVEntryFC70b* vt2 = (sMovieVEntryFC70b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadFileNotFound);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the load-file-not-found card state: cancels the card operation and notifies the front end.
 *
 * Calls cMCOverlayManager_CancelCardOp(self, 1), then the callbackFileNotFound handler.
 */
extern "C" void cMCOverlayManager_UpdateLoadFileNotFound(cMCOverlayManager* self)
{
    cMCOverlayManager_CancelCardOp(self, 1);
    (self->callbackFileNotFound)();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveCorrupt);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

class cMovieDecoder_23FD40 {
public:
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
    virtual int isBusy();                      // slot 11 (0x58)
    virtual void v12();
    virtual void v13();
    virtual void play(int a, void* b);          // slot 14 (0x70)
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual int isReady();                     // slot 53 (0x1A8)
};

struct sMovieVEntry_23FD40 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_23FD40(char* self, int s)
{
    sMovieVEntry_23FD40* vt = *(sMovieVEntry_23FD40**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

/**
 * Handles a corrupt save file found while saving: offers cancel, retry, delete of the file, or overwrite.
 *
 * Once the card is idle: a missing card goes to NoCardsError (4); delete asks the card to delete the slot's
 * file
 * (mFileCursor) and moves to DeleteProgress (25); overwrite goes to PreSaveFile (13).
 */
extern "C" void cMCOverlayManager_UpdateSaveCorrupt(cMCOverlayManager* self)
{
    if (((cMovieDecoder_23FD40*)self->mpMemCard)->isBusy() != 0) {
        return;
    }
    if (self->mCheckCard == 0 && ((cMovieDecoder_23FD40*)self->mpMemCard)->isReady() == 0) {
        setState_23FD40(((char*)self), 4);
        return;
    }
    if ((self->callbackIsCancelPressed)() != 0) {
        cMCOverlayManager_CancelCardOp(self, 0);
        return;
    }
    if ((self->callbackIsRetryPressed)() != 0) {
        cMCOverlayManager_RetryCardOp(self);
        return;
    }
    if ((self->callbackIsDeletePressed)() != 0) {
        ((cMovieDecoder_23FD40*)self->mpMemCard)->play(self->mFileCursor, ((char*)self) + 0x234);
        setState_23FD40(((char*)self), 25);
        return;
    }
    if ((self->callbackIsOverwritePressed)() != 0) {
        setState_23FD40(((char*)self), 13);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadCorrupt);
#ifdef SKIP_ASM
struct sMovieVEntryFE70 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles a corrupt save file found while loading: cancel aborts with the fail message, retry retries.
 *
 * Otherwise it calls callbackIsDeletePressed and ignores the result.
 */
extern "C" void cMCOverlayManager_UpdateLoadCorrupt(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntryFE70* vt = *(sMovieVEntryFE70**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 1);
        } else if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        } else {
            ((void (*)())self->callbackIsDeletePressed)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadDoneWait);
#ifdef SKIP_ASM
/**
 * Waits out the display delay after a load, then reports the result to the front end.
 *
 * A card error goes to LoadFailed (36) and calls callbackReadFailed; otherwise moves to LoadDone (35) and
 * calls
 * callbackReadSuccess.
 */
extern "C" void cMCOverlayManager_UpdateLoadDoneWait(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int n = self->mDisplayWaitTicks;
    if (n > 0) {
        self->mDisplayWaitTicks = n - 1;
        return;
    }
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            me->v01(36);
            (self->callbackReadFailed)();
        } else {
            me->v01(35);
            (self->callbackReadSuccess)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadDone);
#ifdef SKIP_ASM
/**
 * Finishes a completed load by passing code 1 to the manager's virtual state handler (vtable slot 1).
 */
extern "C" void cMCOverlayManager_UpdateLoadDone(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadConfirm);
#ifdef SKIP_ASM
/**
 * Handles the load confirmation: load starts the read (LoadStart, 31) and cancel returns to Load (29).
 *
 * A missing card goes to NoCardsError (4).
 */
extern "C" void cMCOverlayManager_UpdateLoadConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((self->callbackIsLoadPressed)() != 0) {
            me->v01(0x1F);
            return;
        }
        if ((self->callbackIsCancelPressed)() != 0) {
            me->v01(29);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadStart);
#ifdef SKIP_ASM
struct sMovieVEntry00B0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry00B0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Starts the load: moves to LoadProgress (32), or to LoadFailed (36) with callbackReadFailed on a card error.
 */
extern "C" void cMCOverlayManager_UpdateLoadStart(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntry00B0a* vt = *(sMovieVEntry00B0a**)memCard;
    if (vt[73].fn((char*)memCard + vt[73].delta) != 0) {
        sMovieVEntry00B0b* vt2 = (sMovieVEntry00B0b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 36);
        (self->callbackReadFailed)();
    } else {
        sMovieVEntry00B0b* vt2 = (sMovieVEntry00B0b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadProgress);
#ifdef SKIP_ASM
extern "C" void func_0023CA70(void* self);
extern "C" void func_00241240(void* self);

class cMovieSub434_0130 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual void v40(int);
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual int v70();
    virtual int v71();
    virtual int v72();
    virtual int v73();
};

/**
 * Tracks a save-file read and passes the data to the front end until the whole file is loaded.
 *
 * mProgress is the card's byte count (card slot 18) over mSize. When the card's read completes, the front end
 * gets
 * callbackReadUpdate with the remaining size, then callbackReadEnd; a 0 from either fails the load through
 * func_00241240(), otherwise the manager moves to LoadClosing (33). A card error goes to LoadFailed (36).
 */
extern "C" void cMCOverlayManager_UpdateLoadProgress(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    self->mDisplayWaitTicks -= 1;
    int n = ((cMovieSub434_0130*)self->mpMemCard)->v18();
    self->mProgress = (float)n / (float)self->mSize;
    if (((cMovieSub434_0130*)self->mpMemCard)->v73() != 0) {
        me->v01(36);
        (self->callbackReadFailed)();
        return;
    }
    if (((cMovieSub434_0130*)self->mpMemCard)->v31() == 1) {
        int loadedSize = self->mLoadedSize;
        int totalSize = self->mSize;
        if (loadedSize < totalSize) {
            if ((self->callbackReadUpdate)(totalSize - loadedSize) == 0)
                func_00241240(self);
        } else if ((self->callbackReadEnd)() == 0) {
            ((cMovieSub434_0130*)self->mpMemCard)->v40(self->mFileCursor);
            func_00241240(self);
        } else {
            func_0023CA70(self);
            me->v01(33);
        }
        return;
    }
    if (((cMovieSub434_0130*)self->mpMemCard)->v73() != 0) {
        (self->callbackReadFailed)();
        me->v01(36);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoad);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);

struct sMovieVEntry02D0a {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sMovieVEntry02D0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline sMovieVEntry02D0a* Pad_02D0(void* self, char** po)
{
    *po = *(char**)((char*)self + 0x434);
    return *(sMovieVEntry02D0a**)*po;
}

static inline int Pad0_02D0(void* self, int slot)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry02D0a* vt = *(sMovieVEntry02D0a**)o;
    return vt[slot].fn((char*)o + vt[slot].delta);
}

static inline int Pad1_02D0(void* self, int slot, int a)
{
    void* o = *(void**)((char*)self + 0x434);
    sMovieVEntry02D0a* vt = *(sMovieVEntry02D0a**)o;
    return vt[slot].fn((char*)o + vt[slot].delta, a);
}

static inline void Post_02D0(void* self, int code)
{
    sMovieVEntry02D0b* vt = *(sMovieVEntry02D0b**)((char*)self + 0x748);
    vt[1].fn((char*)self + vt[1].delta, code);
}

#define S_02D0 ((char*)self)
#define CB_02D0(off) (*(int (**)())(S_02D0 + (off)))()

/**
 * Runs the load screen (state Load, 29): reacts to card status changes and starts a load or a delete.
 *
 * Card problems go to the no-card error, InitFormatConfirm (39), CardInvalid (5) or NoFreeBlocks (23) as far
 * as
 * the mPollFor* flags allow; a card inserted or recovered restarts the file check. After callbackLoadReady,
 * load
 * goes to LoadStart (31), LoadFailed (36) or LoadCorrupt (37), delete to DeleteConfirm (24), cancel to Done
 * (1).
 */
extern "C" void cMCOverlayManager_UpdateLoad(cMCOverlayManager* self)
{
    if (self->mCheckCard)
        return;
    int quit = 0;
    if (self->mSave || self->mDelete || CB_02D0(0x78))
        quit = 1;
    int held = self->mCurrDeviceStateLast;
    int ready = cMCOverlayManager_updateDeviceStateMonitor(self);
    if (!CB_02D0(0x78) && ready) {

        if (self->mCurrDeviceStateLast & 8) {
            if (self->mPollForNoCard || CB_02D0(0x78) || CB_02D0(104) || quit) {
                cMCOverlayManager_TriggerPollNoCardError(self);
                return;
            }
        } else {
            if (self->mPollForUnformatted && (self->mCurrDeviceStateLast & 0x10)) {
                Post_02D0(self, 39);
                return;
            }
            if (self->mCurrDeviceStateLast & 1) {
                Post_02D0(self, 5);
                return;
            }
            if (self->mPollForFullCard && (self->mCurrDeviceStateLast & 4)) {
                Post_02D0(self, 23);
                return;
            }
        }
        if (!(self->mCurrDeviceStateLast & 8) && (held & 8)) {
            cMCOverlayManager_InitiateFileCheck(self);
            return;
        }
        int rel = held & ~4;
        if (self->mCurrDeviceStateLast == 0 && rel) {
            cMCOverlayManager_InitiateFileCheck(self);
            return;
        }
    }
    if (!quit) {
        if (self->mCardChanged) {
            self->mCardChanged = 0;
            cMCOverlayManager_RecheckCard(self);
        }
        if (self->mCurrDeviceStateLast & 8) {
            Post_02D0(self, 49);
            return;
        }
    }
    (self->callbackLoadReady)();
    if ((self->mState ^ 0x1D) != 0)
        return;
    if (CB_02D0(0x78)) {
        if (self->mLoadOk) {
            Post_02D0(self, 0x1F);
            return;
        }
        if (!Pad0_02D0(self, 53)) {
            Post_02D0(self, 36);
            return;
        }
        if (!Pad1_02D0(self, 67, self->mFileCursor)) {
            Post_02D0(self, 37);
            return;
        }
        if (!Pad1_02D0(self, 65, self->mFileCursor))
            return;
        Post_02D0(self, 37);
        return;
    }
    if (CB_02D0(104)) {
        if (!self->mDeleteOk)
            return;
        char* memCard;
        sMovieVEntry02D0a* vt = Pad_02D0(self, &memCard);
        ((void (*)(void*, int, char*))vt[14].fn)(memCard + vt[14].delta, self->mFileCursor, S_02D0 + 0x234);
        Post_02D0(self, 0x18);
        return;
    }
    if (CB_02D0(108))
        Post_02D0(self, 1);
}
#undef CB_02D0
#undef S_02D0
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadClosing);
#ifdef SKIP_ASM
/**
 * Waits for the loaded file to close, then moves to LoadDoneWait (34), or to LoadFailed (36) on a card error.
 */
extern "C" void cMCOverlayManager_UpdateLoadClosing(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    (self->mDisplayWaitTicks)--;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            me->v01(36);
            (self->callbackReadFailed)();
            return;
        }
        if (self->mDisplayWaitTicks <= 0) {
            me->v01(34);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteConfirm);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the delete confirmation: delete moves to DeleteProgress (25) and cancel aborts with the fail
 * message.
 *
 * Once the card is idle, a missing card goes to NoCardsError (4) and a card error to DeleteFailed (28).
 */
extern "C" void cMCOverlayManager_UpdateDeleteConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            me->v01(0x1C);
            return;
        }
        if ((self->callbackIsDeletePressed)() != 0) {
            me->v01(25);
            return;
        }
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteProgress);
#ifdef SKIP_ASM
struct sMovieVEntry0768a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry0768b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Waits for a file delete to finish, then moves to DeleteDoneWait (26), or DeleteFailed (28) on a card error.
 */
extern "C" void cMCOverlayManager_UpdateDeleteProgress(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntry0768a* vt = *(sMovieVEntry0768a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        void* card = self->mpMemCard;
        sMovieVEntry0768a* vtb = *(sMovieVEntry0768a**)card;
        if (vtb[73].fn((char*)card + vtb[73].delta) != 0) {
            sMovieVEntry0768b* vt2 = (sMovieVEntry0768b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 0x1C);
        } else {
            sMovieVEntry0768b* vt2 = (sMovieVEntry0768b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 26);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteFailed);
#ifdef SKIP_ASM
struct sMovieVEntry0800 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the "delete failed" message: once the card is idle, cancel aborts the operation.
 */
extern "C" void cMCOverlayManager_UpdateDeleteFailed(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntry0800* vt = *(sMovieVEntry0800**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteDoneWait);
#ifdef SKIP_ASM
class cMovieDecoder_240860 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void setVolume(int v);  // slot 5 (0x28)
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual int isBusy();  // slot 11 (0x58)
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
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();  // slot 73 (0x248)
};

struct sMovieVEntry_240860 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_240860(char* self, int s)
{
    sMovieVEntry_240860* vt = *(sMovieVEntry_240860**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

/**
 * Waits out the display delay after a delete, then moves to DeleteDone (27), or DeleteFailed (28) on a card
 * error.
 *
 * While the delay runs, each idle frame asks the card to recheck the selected device.
 */
extern "C" void cMCOverlayManager_UpdateDeleteDoneWait(cMCOverlayManager* self)
{
    cMovieDecoder_240860* memCard = (cMovieDecoder_240860*)self->mpMemCard;
    if (self->mDisplayWaitTicks > 0) {
        if (memCard->isBusy() == 0) {
            if (((cMovieDecoder_240860*)self->mpMemCard)->isFinished() != 0) {
                setState_240860(((char*)self), 0x1C);
                return;
            }
            ((cMovieDecoder_240860*)self->mpMemCard)->setVolume(self->mSelectedDevice);
        }
        self->mDisplayWaitTicks -= 1;
    } else {
        if (memCard->isBusy() != 0) {
            return;
        }
        if (((cMovieDecoder_240860*)self->mpMemCard)->isFinished() != 0) {
            setState_240860(((char*)self), 0x1C);
        } else {
            setState_240860(((char*)self), 27);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteDone);
#ifdef SKIP_ASM
struct sMovieVEntry0960a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry0960b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Finishes a delete: once the card is idle and callbackDeleteDone accepts, rereads the card (ReadingCard, 6).
 */
extern "C" void cMCOverlayManager_UpdateDeleteDone(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntry0960a* vt = *(sMovieVEntry0960a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackDeleteDone)() != 0) {
            sMovieVEntry0960b* vt2 = (sMovieVEntry0960b*)self->mVtbl;
            vt2[1].fn((char*)self + vt2[1].delta, 6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateOverwriteConfirm);
#ifdef SKIP_ASM
/**
 * Handles the overwrite confirmation: overwrite goes to PreSaveFile (13) and cancel to Done (1).
 *
 * Once the card is idle, a missing card goes to NoCardsError (4) and a card error to SaveFailed (20).
 */
extern "C" void cMCOverlayManager_UpdateOverwriteConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if (((cMovieSub434*)self->mpMemCard)->v73() != 0) {
            me->v01(20);
            return;
        }
        if ((self->callbackIsOverwritePressed)() != 0) {
            me->v01(13);
            return;
        }
        if ((self->callbackIsCancelPressed)() != 0) {
            me->v01(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatConfirm);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the format prompt for an unformatted card: format starts it (InitFormatProgress, 40).
 *
 * Once the card is idle: a missing card goes to NoCardsError (4), cancel aborts with the fail message, retry
 * retries.
 */
extern "C" void cMCOverlayManager_UpdateInitFormatConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((self->callbackIsFormatPressed)() != 0) {
            me->v01(40);
            return;
        }
        if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 1);
            return;
        }
        if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatProgress);
#ifdef SKIP_ASM
/**
 * Waits for the card format to finish, then moves to InitFormatFailed (41) or InitFormatDone (42).
 */
extern "C" void cMCOverlayManager_UpdateInitFormatProgress(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    int waitTicks = (self->mDisplayWaitTicks)--;
    if (waitTicks <= 0) {
        if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
            if (((cMovieSub434*)self->mpMemCard)->v09() != 0) {
                me->v01(41);
            } else {
                me->v01(42);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatFailed);
#ifdef SKIP_ASM
struct sMovieVEntry0C30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Handles a failed format during the initial card check: once cancel is pressed, rechecks the selected card.
 *
 * When callbackIsCancelPressed() returns nonzero: passes code 1 to the virtual state handler (vtable slot 1),
 * calls cMCOverlayManager_checkDevice() on mSelectedDevice and sets mCheckCard. Otherwise it keeps waiting.
 */
extern "C" void cMCOverlayManager_UpdateInitFormatFailed(cMCOverlayManager* self)
{
    if ((self->callbackIsCancelPressed)() != 0) {
        sMovieVEntry0C30* vt = (sMovieVEntry0C30*)self->mVtbl;
        vt[1].fn((char*)self + vt[1].delta, 1);
        cMCOverlayManager_checkDevice(self, self->mSelectedDevice);
        self->mCheckCard = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatDone);
#ifdef SKIP_ASM
/**
 * Finishes a format during the initial card check by passing code 6 to the virtual state handler (vtable slot
 * 1).
 */
extern "C" void cMCOverlayManager_UpdateInitFormatDone(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteConfirm);
#ifdef SKIP_ASM
/**
 * Handles the prompt to delete the card's files: delete starts it (InitDeleteProgress, 44), cancel rereads
 * the card.
 *
 * Once the card is idle a missing card goes to NoCardsError (4); while waiting it keeps asking the card to
 * recheck
 * the selected device.
 */
extern "C" void cMCOverlayManager_UpdateInitDeleteConfirm(cMCOverlayManager* self)
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)self;
    if (((cMovieSub434*)self->mpMemCard)->v11() == 0) {
        if (((cMovieSub434*)self->mpMemCard)->v53() == 0) {
            me->v01(4);
            return;
        }
        if ((self->callbackIsDeletePressed)() != 0) {
            me->v01(44);
            return;
        }
        if ((self->callbackIsCancelPressed)() != 0) {
            me->v01(6);
            return;
        }
        ((cMovieSub434*)self->mpMemCard)->v05(self->mSelectedDevice);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteProgress);
#ifdef SKIP_ASM
class cMovieDecoder_240D90 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void seek(int i);  // slot 10 (0x50)
    virtual int isBusy();  // slot 11 (0x58)
    virtual void v12();
    virtual int getCount();  // slot 13 (0x68)
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
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual int canSeek(int i);  // slot 65 (0x208)
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual int isFinished();  // slot 73 (0x248)
};

struct sMovieVEntry_240D90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setState_240D90(char* self, int s)
{
    sMovieVEntry_240D90* vt = *(sMovieVEntry_240D90**)(self + 0x748);
    vt[1].fn(self + vt[1].delta, s);
}

/**
 * Deletes the card's files one per frame, then moves to InitDeleteDone (46), or InitDeleteFailed (45) on an
 * error.
 *
 * mCurrDeleteFile walks the card's file list; each entry the card says can be deleted is deleted. The local
 * view
 * calls these card methods getCount, canSeek and seek (names copied from the movie-decoder views).
 */
extern "C" void cMCOverlayManager_UpdateInitDeleteProgress(cMCOverlayManager* self)
{
    self->mDisplayWaitTicks -= 1;
    if (((cMovieDecoder_240D90*)self->mpMemCard)->isBusy() != 0) {
        return;
    }
    if (self->mDisplayWaitTicks > 0) {
        return;
    }
    if (((cMovieDecoder_240D90*)self->mpMemCard)->isFinished() != 0) {
        setState_240D90(((char*)self), 45);
        return;
    }
    if (self->mCurrDeleteFile < ((cMovieDecoder_240D90*)self->mpMemCard)->getCount()) {
        if (((cMovieDecoder_240D90*)self->mpMemCard)->canSeek(self->mCurrDeleteFile) != 0) {
            ((cMovieDecoder_240D90*)self->mpMemCard)->seek(self->mCurrDeleteFile);
        }
        self->mCurrDeleteFile += 1;
    } else {
        setState_240D90(((char*)self), 46);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteFailed);
#ifdef SKIP_ASM
/**
 * Handles a failed delete during the initial check by passing code 1 to the virtual state handler (vtable slot
 * 1).
 */
extern "C" void cMCOverlayManager_UpdateInitDeleteFailed(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteDone);
#ifdef SKIP_ASM
/**
 * Finishes a delete during the initial check by passing code 6 to the virtual state handler (vtable slot 1).
 */
extern "C" void cMCOverlayManager_UpdateInitDeleteDone(cMCOverlayManager* self)
{
    sMovieVEntry2* vt = (sMovieVEntry2*)self->mVtbl;
    vt[1].fn((char*)self + vt[1].delta, 6);
}
#endif

extern "C" void* cMCOverlayManager_ReturnToDevSelectScreen(cMCOverlayManager* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardUnusable__FPv);
#ifdef SKIP_ASM
/**
 * Handles an unusable memory card by returning to the device-select screen.
 *
 * @return the result of cMCOverlayManager_ReturnToDevSelectScreen()
 */
void* cMCOverlayManager_UpdateCardUnusable(void* self)
{
    return cMCOverlayManager_ReturnToDevSelectScreen((cMCOverlayManager*)self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardDamaged);
#ifdef SKIP_ASM
struct sMovieVEntry0F30 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "damaged card" message: once the card is idle, retry retries and cancel aborts with the fail
 * message.
 */
extern "C" void cMCOverlayManager_UpdateCardDamaged(cMCOverlayManager* self)
{
    void* memCard = self->mpMemCard;
    sMovieVEntry0F30* vt = *(sMovieVEntry0F30**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        if ((self->callbackIsRetryPressed)() != 0) {
            cMCOverlayManager_RetryCardOp(self);
        } else if ((self->callbackIsCancelPressed)() != 0) {
            cMCOverlayManager_CancelCardOp(self, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00240FB0);
#ifdef SKIP_ASM
struct sMovieVEntry0FB0 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

extern "C" void func_00240FB0(void* self, int a, int data, int size)
{
    void* memCard = *(void**)((char*)self + 0x434);
    cMCOverlayManager_mSaveInProgress = 1;
    sMovieVEntry0FB0* vt = *(sMovieVEntry0FB0**)memCard;
    vt[36].fn((char*)memCard + vt[36].delta, a, data, size);
    *(int*)((char*)self + 0xE4) = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241000);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;

struct sMovieVEntry1000 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

extern "C" void func_00241000(void* self, int fileName, int data, int size)
{
    void* memCard = *(void**)((char*)self + 0x434);
    cMCOverlayManager_mSaveInProgress = 1;
    sMovieVEntry1000* vt = *(sMovieVEntry1000**)memCard;
    vt[37].fn((char*)memCard + vt[37].delta, fileName, data, size);
    *(int*)((char*)self + 0xE4) = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241050);
#ifdef SKIP_ASM
struct sMovieVEntry1050 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

extern "C" void func_00241050(void* self, int a, int data, int size)
{
    void* memCard = *(void**)((char*)self + 0x434);
    cMCOverlayManager_mSaveInProgress = 1;
    sMovieVEntry1050* vt = *(sMovieVEntry1050**)memCard;
    vt[38].fn((char*)memCard + vt[38].delta, a, data, size);
    *(int*)((char*)self + 0xE4) = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_cleanUp);
#ifdef SKIP_ASM
/**
 * Tells the memory card object (+0x434) to clean up, through its virtual slot 12.
 */
extern "C" void cMCOverlayManager_cleanUp(void* self)
{
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry* vt = *(sMovieVEntry**)obj;
    vt[12].fn((char*)obj + vt[12].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002410D0);
#ifdef SKIP_ASM
struct sMovieVEntry_2410D0 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_002410D0(void* self)
{
    char* manager = (char*)self;
    char* memCard = *(char**)(manager + 0x434);
    sMovieVEntry_2410D0* vt = *(sMovieVEntry_2410D0**)memCard;
    if (*(int*)(manager + 0xEC) < vt[68].fn(memCard + vt[68].delta)) {
        return *(int*)(manager + 0xEC);
    }
    char* memCardAgain = *(char**)(manager + 0x434);
    sMovieVEntry_2410D0* vt2 = *(sMovieVEntry_2410D0**)memCardAgain;
    return vt2[68].fn(memCardAgain + vt2[68].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241138);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;

struct sMovieVEntry1138 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern "C" void func_00241138(void* self, int data, int size)
{
    void* memCard = *(void**)((char*)self + 0x434);
    int one = 1;
    *(int*)((char*)self + 0xE4) += size;
    cMCOverlayManager_mSaveInProgress = one;
    sMovieVEntry1138* vt = *(sMovieVEntry1138**)memCard;
    vt[41].fn((char*)memCard + vt[41].delta, data, size);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteFailed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cMCOverlayManager_mSaveInProgress;
extern "C" void cMCOverlayManager_SetAction_v(void* self, int v) __asm__("cMCOverlayManager_SetAction");

struct sMovieVEntry1180a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMovieVEntry1180b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Ends a failed save: stops the card write if one is in progress and optionally shows the failure.
 *
 * Clears the global save-in-progress flag; with @p showWriteFailPopup it also calls SetAction(0) and moves to
 * SaveFailed (20).
 *
 * @param showWriteFailPopup  nonzero to show the save-failed state
 */
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int showWriteFailPopup)
{
    int on = self->mWriteInProgress;
    cMCOverlayManager_mSaveInProgress = 0;
    if (on != 0) {
        void* memCard = self->mpMemCard;
        sMovieVEntry1180a* vt = *(sMovieVEntry1180a**)memCard;
        vt[44].fn((char*)memCard + vt[44].delta);
    }
    if (showWriteFailPopup != 0) {
        cMCOverlayManager_SetAction_v(self, 0);
        sMovieVEntry1180b* vt2 = (sMovieVEntry1180b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 20);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ReadData);
#ifdef SKIP_ASM
struct sMovieVEntry3 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

/**
 * Reads the next chunk of the file being loaded.
 *
 * Adds @p n to the running byte count (+0xE8) and calls virtual slot 30 of the memory card object (+0x434).
 *
 * @param data  destination buffer
 * @param n  bytes to read
 */
extern "C" void cMCOverlayManager_ReadData(void* self, int data, int n)
{
    *(int*)((char*)self + 0xE8) += n;
    void* obj = *(void**)((char*)self + 0x434);
    sMovieVEntry3* vt = *(sMovieVEntry3**)obj;
    vt[30].fn((char*)obj + vt[30].delta, data, n);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241240);
#ifdef SKIP_ASM
struct sMovieVEntry1240a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMovieVEntry1240b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00241240(void* self)
{
    void* memCard = *(void**)((char*)self + 0x434);
    sMovieVEntry1240a* vt = *(sMovieVEntry1240a**)memCard;
    vt[33].fn((char*)memCard + vt[33].delta);
    sMovieVEntry1240b* vt2 = *(sMovieVEntry1240b**)((char*)self + 0x748);
    vt2[1].fn((char*)self + vt2[1].delta, 36);
    (*(void (**)())((char*)self + 0x2C))();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002412A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00152758(void* iface);
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void func_00241D40_r(void* self, int mode, void* buf, int size) __asm__("func_00241D40");
extern "C" void func_00241AA0_r(void* buf, int size) __asm__("func_00241AA0");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void func_00241000_r(void* self, char* text, int a, int b) __asm__("func_00241000");
extern int cMCOverlayManager_mSaveInProgress;
extern char str_pct_s_pct_spct_04d_b[];

extern "C" void func_002412A0(void* self)
{
    char text[130];
    char name[130];
    char desc[0x200];
    char fmt[0x200];
    cMCOverlayManager_mSaveInProgress = 1;
    int saveData = func_00152758(cBE_getInterface_Fv(cBE_getBE(), 5));
    int saveSize = cBESaveInterface_getSaveGameSize_r();
    func_002C2540(fmt, str_pct_s_pct_spct_04d_b);
    func_00241D40_r(self, 2, name, 130);
    func_00241AA0_r(desc, 0x200);
    sprintf1_v(text, fmt, desc, name, *(int*)((char*)self + 0xF8) + 1);
    func_00241000_r(self, text, saveData, saveSize);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241380);
#ifdef SKIP_ASM
struct sMovieVEntry1380 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00241380(void* self)
{
    int one = 1;
    void* memCard;
    sMovieVEntry1380* vt;
    *(int*)((char*)self + 0x340) = one;
    *(int*)((char*)self + 0xDC) = 0;
    *(int*)((char*)self + 0x42C) = 0;
    *(int*)((char*)self + 0xF8) = 0;
    *(int*)((char*)self + 0x430) = -1;
    *(int*)((char*)self + 0xFC) = -1;
    *(int*)((char*)self + 0x444) = -1;
    func_0023FB18(self, 0);
    cMCOverlayManager_setSelectedDevice(self);
    *(int*)((char*)self + 0x338) = one;
    *(int*)((char*)self + 0x130) = 2;
    memCard = *(void**)((char*)self + 0x434);
    vt = *(sMovieVEntry1380**)memCard;
    vt[3].fn((char*)memCard + vt[3].delta);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightNextDevice);
#ifdef SKIP_ASM
class cMovieDecoder_241400 {
public:
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
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual int hasChapter(int i);  // slot 52 (0x1A0)
};

extern "C" void cMCOverlayManager_setSelectedDevice(void* self);
void func_0023FB18(void* self, int val);

static inline int nextChapter_241400(char* self, int i)
{
    if (i < *(int*)(self + 0x424) - 1) {
        return i + 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return 0;
    }
    return i;
}

/**
 * Moves the memory card device highlight to the next device that has a card.
 *
 * Does nothing when no device count is known (+0x424 == -1). Steps from the current device (+0x428) to the next
 * one the memory card object (+0x434) reports present, stopping at the last device unless wrapping is allowed
 * (+0x100); falls back to the current device or device 0. A new device is selected with func_0023FB18 and
 * cMCOverlayManager_setSelectedDevice; +0xE0 is cleared. (The C's helper type names, cMovieDecoder_241400/hasChapter, are leftovers:
 * this is the memory card device list.)
 */
extern "C" void cMCOverlayManager_OnHighlightNextDevice(char* self)
{
    int i = *(int*)(self + 0x428);
    if (*(int*)(self + 0x424) == -1) {
        return;
    }
    while (i = nextChapter_241400(self, i), (*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
        if (i == *(int*)(self + 0x424) - 1 || *(int*)(self + 0x100) != 0) {
            if (i == *(int*)(self + 0x428)) {
                break;
            }
            if (*(int*)(self + 0x100) != 1) {
                break;
            }
        }
    }
    if (*(int*)(self + 0x100) == 0 && i == *(int*)(self + 0x424) - 1 && (*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
        i = *(int*)(self + 0x428);
        if ((*(cMovieDecoder_241400**)(self + 0x434))->hasChapter(i) == 0) {
            i = 0;
        }
    }
    if (*(int*)(self + 0x428) != i) {
        func_0023FB18(self, i);
        cMCOverlayManager_setSelectedDevice(self);
    }
    *(int*)(self + 0xE0) = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightPrevDevice);
#ifdef SKIP_ASM
class cMovieDecoder_241540 {
public:
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
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual int hasChapter(int i);  // slot 52 (0x1A0)
};

extern "C" void cMCOverlayManager_setSelectedDevice(void* self);
void func_0023FB18(void* self, int val);

static inline int prevChapter_241540(char* self, int i)
{
    if (i > 0) {
        return i - 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return *(int*)(self + 0x424) - 1;
    }
    return i;
}

/**
 * Moves the device highlight to the previous memory-card device that the card object reports as available.
 *
 * Does nothing while mMaxDevices is -1. Steps mSelectedDevice down, wrapping to mMaxDevices - 1 when mWrapItems
 * is set, until the card object's virtual slot 52 accepts the device or the search gives up. Without wrapping,
 * an unavailable device 0 keeps the current device (or 0 if that one is unavailable too). A new selection goes
 * through func_0023FB18() and cMCOverlayManager_setSelectedDevice(), which resets the device state monitor. Always clears mGetDir.
 */
extern "C" void cMCOverlayManager_OnHighlightPrevDevice(cMCOverlayManager* self)
{
    int i = self->mSelectedDevice;
    if (self->mMaxDevices == -1) {
        return;
    }
    while (i = prevChapter_241540(((char*)self), i), ((cMovieDecoder_241540*)self->mpMemCard)->hasChapter(i) == 0) {
        if (i == 0 || self->mWrapItems != 0) {
            if (i == self->mSelectedDevice) {
                break;
            }
            if (self->mWrapItems != 1) {
                break;
            }
        }
    }
    if (self->mWrapItems == 0 && i == 0 && ((cMovieDecoder_241540*)self->mpMemCard)->hasChapter(0) == 0) {
        i = self->mSelectedDevice;
        if (((cMovieDecoder_241540*)self->mpMemCard)->hasChapter(i) == 0) {
            i = 0;
        }
    }
    if (self->mSelectedDevice != i) {
        func_0023FB18(((char*)self), i);
        cMCOverlayManager_setSelectedDevice(((char*)self));
    }
    self->mGetDir = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241678);
#ifdef SKIP_ASM
extern "C" void func_00241678(void* self)
{
    int n = *(int*)((char*)self + 0xF8);
    if (n > 0) {
        *(int*)((char*)self + 0xF8) = n - 1;
    } else if (*(int*)((char*)self + 0x100) != 0) {
        *(int*)((char*)self + 0xF8) = func_002410D0(self) - 1;
    }
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", func_002416C0);
#ifdef SKIP_ASM
extern "C" void func_002416C0(void* self)
{
    int n = func_002410D0(self) - 1;
    int cur = *(int*)((char*)self + 0xF8);
    if (cur < n) {
        *(int*)((char*)self + 0xF8) = cur + 1;
    } else if (*(int*)((char*)self + 0x100) != 0) {
        *(int*)((char*)self + 0xF8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceDisplayString);
#ifdef SKIP_ASM
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
extern "C" int strlen1(void* s);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* self, void* dst, int n);
int GetHashValue32(char* s);
extern char str_kT_MEMOVNotDetectedPS2_b[];
extern char str_kT_MEMOVNotFormattedPS2_b[];
extern char str_kT_MEMMemDevicePS2[];

class cMovieSub434_1710 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48(int);
    virtual int v49();
    virtual int v50(int);
    virtual int v51();
    virtual int v52(int);
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56(int);
    virtual int v57(int);
    virtual int v58();
    virtual int v59();
    virtual int v60(int);
};

struct sLocVEntry_241710 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};



static inline void* Loc_241710(char* key)
{
    char* loc = (char*)SSXApp->FELocale;
    sLocVEntry_241710* vt = *(sLocVEntry_241710**)(loc + 4);
    char* thisp = loc + vt[4].delta;
    return vt[4].fn(thisp, GetHashValue32(key));
}

/**
 * Builds the on-screen label for a memory-card device.
 *
 * A missing or failing device gets the localized kT_MEMOVNotDetectedPS2 text and an unformatted one
 * kT_MEMOVNotFormattedPS2, both formatted with the device number (idx + 1). A usable device shows its card
 * name, or
 * kT_MEMMemDevicePS2 when the name is empty. A formatted card with the wrong encoding leaves @p dst
 * untouched.
 *
 * @param idx  device index
 * @param dst  receives the label (16-bit string)
 */
extern "C" void cMCOverlayManager_GetDeviceDisplayString(cMCOverlayManager* self, int idx, char* dst)
{
    char buf[0x200];
    if (((cMovieSub434_1710*)self->mpMemCard)->v52(idx))
    {
        if (((cMovieSub434_1710*)self->mpMemCard)->v48(idx) || ((cMovieSub434_1710*)self->mpMemCard)->v50(idx) || ((cMovieSub434_1710*)self->mpMemCard)->v60(idx))
        {
            cMCOverlayManager_getDeviceNumberString(self, buf, idx);
            sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotDetectedPS2_b), buf);
        }
        else if (!((cMovieSub434_1710*)self->mpMemCard)->v56(idx))
        {
            cMCOverlayManager_getDeviceNumberString(self, buf, idx);
            sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotFormattedPS2_b), buf);
        }
        else if (((cMovieSub434_1710*)self->mpMemCard)->v57(idx))
        {
            cMCOverlayManager_getDeviceNumberString(self, buf, idx);
            char* name = ((char*)&((&(self)->mDevices[0]))[idx].szName[0]);
            if (strlen1(name) == 0)
                sprintf1_v(dst, Loc_241710(str_kT_MEMMemDevicePS2), buf);
            else
                strcpy1(dst, name);
        }
    }
    else
    {
        cMCOverlayManager_getDeviceNumberString(self, buf, idx);
        sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotDetectedPS2_b), buf);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241928);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" unsigned short* func_002C2540_r(void*, void*) __asm__("func_002C2540");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_Free[];
extern char str_pct_d_pct_s[];

struct sMPSlot1928 {
    int a;      // 0x344
    int b;      // 0x348
    int c;      // 0x34C
    int d;      // 0x350
    int id;     // 0x354
    char pad[0xE0 - 0x14];
};

struct sMP1928 {
    char pad[0x344];
    sMPSlot1928 slots[1];
};

extern "C" void func_00241928(sMP1928* self, int i, void* out)
{
    if (self->slots[i].d != 0 && self->slots[i].a != 0 && self->slots[i].b != 0 && self->slots[i].c != 0) {
        char name[0x200];
        char path[0x200];
        func_002C2540_r(name, str_Free);
        func_002C2540_r(path, str_pct_d_pct_s);
        sprintf1_v(out, path, self->slots[i].id, name);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002419D8);
#ifdef SKIP_ASM
extern "C" int func_002419D8(void* self, int device)
{
    char* deviceBase = (char*)self + device * 0xe0;
    return *(int*)(deviceBase + 0x354);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceTotalString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned short* func_002C2540_r(void*, void*) __asm__("func_002C2540");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_Total[];
extern char str_pct_d_pct_s[];

struct sMCDev {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    char pad[0xE0 - 0x18];
};
struct sMCOverlayMgr {
    char pad[0x344];
    sMCDev dev[1];
};

/**
 * Writes a device's total space as "<total> Total" into a 16-bit string, when the device is usable.
 *
 * Writes nothing unless the device is plugged in, valid, formatted and correctly encoded; the format "%d %s"
 * and "Total" are converted to 16-bit strings first.
 *
 * @param idx  index of the device record (cMCOverlayManager_sdevice)
 * @param dst  destination 16-bit string
 */
extern "C" void cMCOverlayManager_GetDeviceTotalString(sMCOverlayMgr* self, int idx, void* dst)
{
    unsigned short str[256];
    unsigned short fmt[256];
    if (self->dev[idx].fC == 0) return;
    if (self->dev[idx].f0 && self->dev[idx].f4 && self->dev[idx].f8) {
        func_002C2540_r(str, str_Total);
        func_002C2540_r(fmt, str_pct_d_pct_s);
        sprintf1_v(dst, fmt, self->dev[idx].f14, str);
    }
}
#endif

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0047C3A8;
extern "C" void func_002C2540(void*, void*);

//100%
INCLUDE_ASM("movie/movieplayer", func_00241AA0);
#ifdef SKIP_ASM
extern "C" void func_00241AA0(void* self)
{
    func_002C2540(self, &D_0047C3A8);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getMaxDevicesSupported);
#ifdef SKIP_ASM
struct sMovieVEntry1AC0 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Returns how many memory-card devices the manager uses, asking the card once and caching the answer.
 *
 * The card's count (card vtable slot 69) is clamped to 1, so only the first device is ever used.
 *
 * @return the number of devices (cached in mMaxDevices)
 */
extern "C" int cMCOverlayManager_getMaxDevicesSupported(cMCOverlayManager* self)
{
    if (self->mMaxDevices < 0) {
        void* memCard = self->mpMemCard;
        sMovieVEntry1AC0* vt = *(sMovieVEntry1AC0**)memCard;
        int n = vt[69].fn((char*)memCard + vt[69].delta);
        self->mMaxDevices = n;
        if (n >= 2) {
            self->mMaxDevices = 1;
        }
    }
    return self->mMaxDevices;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getDeviceState);
#ifdef SKIP_ASM
class cMovieSub434_1B20 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual bool v53();
    virtual int v54();
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
};

struct sMovieFlags_1B20
{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int e : 1;
    unsigned int f : 4;
    unsigned int rest : 23;
};

// PORT: the unit declares cMCOverlayManager_getDeviceState as returning int; it really returns this 4-byte flags struct in $v0.
extern "C" sMovieFlags_1B20 cMCOverlayManager_getDeviceState_s(void* self) __asm__("cMCOverlayManager_getDeviceState");

/**
 * Returns the status flags of the selected memory-card device.
 *
 * While another device is being checked it returns the last stored state (mCurrDeviceStateLast). Otherwise it
 * asks
 * the card and builds the flags: error, damaged, full (free space below what the save needs), no card,
 * unformatted.
 *
 * @return the device state (bits 1 error, 2 damaged, 4 full, 8 no card, 0x10 unformatted in
 *         mCurrDeviceStateLast)
 */
extern "C" sMovieFlags_1B20 cMCOverlayManager_getDeviceState_s(void* self)
{
    if (*(int*)((char*)self + 0x42C) != *(int*)((char*)self + 0x428))
        return *(sMovieFlags_1B20*)((char*)self + 0x43C);
    sMovieFlags_1B20 state = {0, 0, 0, 0, 0, 0, 0};
    state.a = (*(cMovieSub434_1B20**)((char*)self + 0x434))->v49();
    int isDamaged = (*(cMovieSub434_1B20**)((char*)self + 0x434))->v51() || (*(cMovieSub434_1B20**)((char*)self + 0x434))->v59();
    state.b = isDamaged;
    int isFull = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v13() && (*(cMovieSub434_1B20**)((char*)self + 0x434))->v20() < (*(cMovieSub434_1B20**)((char*)self + 0x434))->v23();
    state.c = isFull;
    state.d = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v53();
    int isUnformatted = !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v55() || !(*(cMovieSub434_1B20**)((char*)self + 0x434))->v58();
    state.e = isUnformatted;
    state.f = 0;
    return state;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_updateDeviceStateMonitor);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_getDeviceState(void* self);

/**
 * Polls the selected device's state and tells whether an unacknowledged state change is waiting.
 *
 * Stores the state from cMCOverlayManager_getDeviceState() in mCurrDeviceStateLast. On a change, the front
 * end's callbackAcknowledgeCardStatusChange() decides whether it is acknowledged (mCurrDeviceStateAck).
 *
 * @return 1 when the current state is not acknowledged, otherwise 0
 */
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self)
{
    int newState = cMCOverlayManager_getDeviceState(self);
    int changed = self->mCurrDeviceStateLast != newState;
    if (self->mCurrDeviceStateAck == 0 || changed) {
        self->mCurrDeviceStateLast = newState;
        if (changed) {
            self->mCurrDeviceStateAck = (self->callbackAcknowledgeCardStatusChange)(newState);
        }
        return self->mCurrDeviceStateAck ^ 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241D40);
#ifdef SKIP_ASM
extern "C" void func_002C2540(void*, void*);
extern char str_REP[];
extern char str_GAM[];
extern char str_SET[];

extern "C" void func_00241D40(void* self, int mode, void* buf)
{
    switch (mode) {
    case 0:
        func_002C2540(buf, str_REP);
        break;
    case 1:
        func_002C2540(buf, str_GAM);
        break;
    case 2:
        func_002C2540(buf, str_SET);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241DC0);
#ifdef SKIP_ASM
extern "C" int func_00241DC0(void) {
    return 0x30;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getDeviceNumberString);
#ifdef SKIP_ASM
extern "C" void sprintf1(void* dst, void* src, int n);
extern char str_pct_d_g[];

/**
 * Writes a device's number for display ("1", "2", ...) as a wide string.
 *
 * @param dst  the destination wide-string buffer
 * @param n    the zero-based device index; the text shows @p n + 1
 */
extern "C" void cMCOverlayManager_getDeviceNumberString(void* self, void* dst, int n)
{
    unsigned short buf[256];
    func_002C2540(buf, str_pct_d_g);
    sprintf1(dst, buf, n + 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetMessage);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* self, void* dst, int n);
extern "C" void* strcpy1(void* dst, void* src);
extern char D_004A29E8[];
extern char str_kT_MEMFECheckingCardPS2_b[];
extern char str_kT_MEMOVNotDetectedPS2_b[];
extern char str_kT_MEMFormattingPS2_b[];
extern char str_kT_MEMOVSaveSavingPS2NGC_b[];
extern char str_kT_MEMOVLoadingPS2NGC[];
extern char str_kT_MEMDeletingPS2_b[];

struct sVEntry_00241E18 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_00241E18(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_00241E18* vt = *(sVEntry_00241E18**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(key));
}

static inline void SetText_00241E18(void* out, char* key, void* arg)
{
    int str = Loc_00241E18(key);
    sprintf1_v(out, (void*)str, arg);
}

/**
 * Fills a wide-string buffer with the status message for the memory card manager's current state.
 *
 * Starts from an empty string, gets the device number text (cMCOverlayManager_getDeviceNumberString) and, by state (+0x130), formats
 * kT_MEMFECheckingCardPS2, kT_MEMOVNotDetectedPS2, kT_MEMFormattingPS2, kT_MEMOVSaveSavingPS2NGC or
 * kT_MEMOVLoadingPS2NGC with it, or copies kT_MEMDeletingPS2; state 55 and others leave it empty.
 *
 * @param out  the destination wide-string buffer
 */
extern "C" void cMCOverlayManager_GetMessage(char* self, void* out)
{
    unsigned short num[256];
    unsigned short num2[256];
    func_002C2540(out, D_004A29E8);
    cMCOverlayManager_getDeviceNumberString(self, num, *(int*)(self + 0x428));
    switch (*(int*)(self + 0x130)) {
    case 0: case 2: case 3: case 6: case 7: case 13: case 29:
    case 49: case 50: case 51: case 52: case 53: case 54:
        SetText_00241E18(out, str_kT_MEMFECheckingCardPS2_b, num);
        break;
    case 4:
        cMCOverlayManager_getDeviceNumberString(self, num2, *(int*)(self + 0x428));
        SetText_00241E18(out, str_kT_MEMOVNotDetectedPS2_b, num2);
        break;
    case 10: case 11: case 39: case 40:
        SetText_00241E18(out, str_kT_MEMFormattingPS2_b, num);
        break;
    case 15: case 16: case 17: case 18:
        SetText_00241E18(out, str_kT_MEMOVSaveSavingPS2NGC_b, num);
        break;
    case 31: case 32: case 33: case 34:
        SetText_00241E18(out, str_kT_MEMOVLoadingPS2NGC, num);
        break;
    case 25: case 26: case 44:
        strcpy1(out, (void*)Loc_00241E18(str_kT_MEMDeletingPS2_b));
        break;
    case 55:
        break;
    }
}
#endif

extern "C" void* cMCOverlayManager_GatherDeviceInfo1(void*, int);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GatherDeviceInfo__FPv);
#ifdef SKIP_ASM
/**
 * Refreshes the info record of the device being checked (mCurrCheckDevice) through GatherDeviceInfo1().
 *
 * @return no meaningful value (cMCOverlayManager_GatherDeviceInfo1 returns nothing)
 */
void* cMCOverlayManager_GatherDeviceInfo(void* self)
{
    return cMCOverlayManager_GatherDeviceInfo1(self, *(int*)((char*)self + 0x42c));
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_InitiateFileCheck);
#ifdef SKIP_ASM
struct sMovieVEntry1FD0a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry1FD0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Starts looking for the game's files on the card: calls callbackSysRecheckFile and moves to a file-check
 * state.
 *
 * Goes to InitFileCheck (51) when the card is idle, otherwise InitFileCheckWait (53).
 */
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self)
{
    void* memCard;
    sMovieVEntry1FD0a* vt;
    (self->callbackSysRecheckFile)();
    memCard = (void*)self->mpMemCard;
    vt = *(sMovieVEntry1FD0a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        sMovieVEntry1FD0b* vt2 = (sMovieVEntry1FD0b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 51);
    } else {
        sMovieVEntry1FD0b* vt2 = (sMovieVEntry1FD0b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 53);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_RecheckCard);
#ifdef SKIP_ASM
struct sMovieVEntry2050a {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMovieVEntry2050b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Starts a fresh card check: clears mCheckCard and moves to CheckCardsWait (2), or CheckCards (3) if the card
 * is busy.
 */
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self)
{
    void* memCard;
    sMovieVEntry2050a* vt;
    self->mCheckCard = 0;
    memCard = (void*)self->mpMemCard;
    vt = *(sMovieVEntry2050a**)memCard;
    if (vt[11].fn((char*)memCard + vt[11].delta) == 0) {
        sMovieVEntry2050b* vt2 = (sMovieVEntry2050b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 2);
    } else {
        sMovieVEntry2050b* vt2 = (sMovieVEntry2050b*)self->mVtbl;
        vt2[1].fn((char*)self + vt2[1].delta, 3);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GatherDeviceInfo1);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* p, int c, int n);

class cMovieSub434_20C8 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48(int);
    virtual int v49();
    virtual int v50(int);
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54();
    virtual int v55();
    virtual int v56(int);
    virtual int v57(int);
    virtual int v58();
    virtual int v59();
    virtual int v60(int);
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67();
    virtual int v68();
    virtual int v69();
    virtual void v70(int, char*, int);
};

struct sSlot_20C8
{
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    char name[200];
};

struct sMP_20C8
{
    char pad0[0x344];
    sSlot_20C8 slots[1];
    int f424;
    int f428;
    int cur;
};

// PORT: the unit declares cMCOverlayManager_GatherDeviceInfo1 as returning void*, but it returns nothing; bind the void body by asm label.
extern "C" void cMCOverlayManager_GatherDeviceInfo1_v(void* self, int i) __asm__("cMCOverlayManager_GatherDeviceInfo1");

/**
 * Refreshes the record for one memory-card device from the card.
 *
 * Marks the device valid, formatted, correctly encoded and plugged in, then clears whichever flag the card
 * contradicts and stops there. For a usable card it reads the card's name (100 characters), and for the
 * device
 * being checked (mCurrCheckDevice) also its free space.
 *
 * @param i  device index
 */
extern "C" void cMCOverlayManager_GatherDeviceInfo1_v(void* self, int i)
{
    sMP_20C8* me = (sMP_20C8*)self;
    me->slots[i].a = 1;
    char* bb = (char*)self + 0x348;
    int* pb = (int*)(bb + i * 0xE0);
    char* bc = (char*)self + 0x34C;
    int* pc = (int*)(bc + i * 0xE0);
    *pb = 1;
    *pc = 1;
    char* name = me->slots[i].name;
    me->slots[i].d = 1;
    MEM_fill(name, 0, 200);
    if ((*(cMovieSub434_20C8**)((char*)self + 0x434))->v48(i) || (*(cMovieSub434_20C8**)((char*)self + 0x434))->v50(i) || (*(cMovieSub434_20C8**)((char*)self + 0x434))->v60(i))
    {
        me->slots[i].a = 0;
        return;
    }
    if (!(*(cMovieSub434_20C8**)((char*)self + 0x434))->v56(i))
    {
        *pb = 0;
        return;
    }
    if (!(*(cMovieSub434_20C8**)((char*)self + 0x434))->v57(i))
    {
        *pc = 0;
        return;
    }
    if (i == me->cur)
        me->slots[me->cur].e = (*(cMovieSub434_20C8**)((char*)self + 0x434))->v20();
    (*(cMovieSub434_20C8**)((char*)self + 0x434))->v70(i, name, 100);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_cMCOverlayManager);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void func_00244860(void*);
void func_00244868(void*);
void func_00244870(void*);
void func_00244878(void*);
extern "C" void MEM_fill(void* p, int c, int n);
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern void* cMCOverlayManager_vtbl[];
extern int cMCOverlayManager_mSaveInProgress;

/**
 * Constructs the memory-card overlay manager: resets every state field and installs the default callbacks.
 *
 * Sets mFileType 2 and SetAction(1), and fills the 45 front-end callback slots with stubs that do nothing or
 * return 0 or 1, as fitting each slot. The device records are zeroed and mMaxDevices is -1 (not yet asked).
 *
 * @return @p self
 */
// cMCOverlayManager constructor body: resets the state, the 45 state handlers (0x00-0xB0) and the device records.
extern "C" void* cMCOverlayManager_cMCOverlayManager(cMCOverlayManager* self)
{
    int* w = (int*)self;
    void** callbacks = (void**)self;
    sMCDev* dev = ((sMCOverlayMgr*)self)->dev;
    w[45] = 2;
    w[47] = 0;
    w[0x30] = 0;
    w[49] = 0;
    w[466] = (int)cMCOverlayManager_vtbl;
    cMCOverlayManager_SetAction(self, 1);
    w[76] = 0;
    w[0x3E] = 0;
    w[0x3F] = -1;
    w[67] = 0;
    w[65] = 0;
    w[66] = 0;
    w[50] = 0;
    w[51] = 0;
    w[52] = 0;
    w[53] = 0;
    w[54] = 0;
    w[0x38] = 0;
    w[0x3C] = 0;
    w[58] = 0;
    w[57] = 0;
    w[61] = 0;
    w[55] = 0;
    w[59] = 0;
    w[265] = -1;
    w[0x40] = 0;
    w[75] = 0;
    w[70] = 0;
    w[71] = 0;
    w[72] = 0;
    w[73] = 0;
    w[74] = 1;
    ((float*)self)[0x44] = 1.0f;
    ((float*)self)[0x45] = 0.016669999808073044f;
    MEM_fill(dev, 0, 0xE0);
    w[273] = -1;
    w[266] = 0;
    w[267] = 0;
    w[271] = 0;
    w[272] = 0;
    MEM_fill(w + 0x8D, 0, 66);
    w[269] = 0;
    w[207] = 5;
    w[208] = 0;
    w[206] = 0;
    cMCOverlayManager_mSaveInProgress = 0;
    w[270] = 0;
    callbacks[0] = (void*)func_00244860;
    callbacks[1] = (void*)func_00244878;
    callbacks[2] = (void*)func_00244860;
    callbacks[3] = (void*)func_00244860;
    callbacks[4] = (void*)func_00244860;
    callbacks[5] = (void*)func_00244860;
    callbacks[6] = (void*)func_00244860;
    callbacks[7] = (void*)func_00244878;
    callbacks[8] = (void*)func_00244868;
    callbacks[9] = (void*)func_00244860;
    callbacks[10] = (void*)func_00244860;
    callbacks[11] = (void*)func_00244860;
    callbacks[12] = (void*)func_00244870;
    callbacks[13] = (void*)func_00244870;
    callbacks[14] = (void*)func_00244860;
    callbacks[15] = (void*)func_00244860;
    callbacks[16] = (void*)func_00244860;
    callbacks[17] = (void*)func_00244860;
    callbacks[18] = (void*)func_00244860;
    callbacks[19] = (void*)func_00244860;
    callbacks[20] = (void*)func_00244860;
    callbacks[21] = (void*)func_00244860;
    callbacks[22] = (void*)func_00244860;
    callbacks[23] = (void*)func_00244860;
    callbacks[24] = (void*)func_00244860;
    callbacks[25] = (void*)func_00244868;
    callbacks[26] = (void*)func_00244868;
    callbacks[27] = (void*)func_00244868;
    callbacks[28] = (void*)func_00244868;
    callbacks[29] = (void*)func_00244868;
    callbacks[30] = (void*)func_00244868;
    callbacks[31] = (void*)func_00244868;
    callbacks[32] = (void*)func_00244868;
    callbacks[33] = (void*)func_00244868;
    callbacks[34] = (void*)func_00244868;
    callbacks[35] = (void*)func_00244860;
    callbacks[36] = (void*)func_00244860;
    callbacks[37] = (void*)func_00244860;
    callbacks[38] = (void*)func_00244860;
    callbacks[39] = (void*)func_00244860;
    callbacks[40] = (void*)func_00244860;
    callbacks[41] = (void*)func_00244860;
    callbacks[42] = (void*)func_00244860;
    callbacks[43] = (void*)func_00244860;
    callbacks[44] = (void*)func_00244868;
    for (int i = 0; i < 1; i++) {   // every device record (sMCOverlayMgr::dev has one)
        MEM_fill(&dev[i], 0, 0xE0);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager__cMCOverlayManager);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;
extern void* cMCOverlayManager_vtbl[];
void operator_delete(int*);

/**
 * Destroys the memory-card overlay manager: restores its vtable, clears the save-in-progress flag, frees on
 * request.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMCOverlayManager__cMCOverlayManager(cMCOverlayManager* self, int flags)
{
    self->mVtbl = cMCOverlayManager_vtbl;
    cMCOverlayManager_mSaveInProgress = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_cOcclusionMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the occlusion manager by zeroing its words at +0x0, +0x4, +0x108 and +0x10C.
 *
 * @return @p self
 */
void* cOcclusionMan_cOcclusionMan(void* self)
{
    int zeroValue = 0;
    *(int*)((char*)self + 0x4) = zeroValue;
    *(int*)((char*)self + 0x108) = zeroValue;
    *(int*)((char*)self + 0x10c) = zeroValue;
    *(int*)self = zeroValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242518);
#ifdef SKIP_ASM
void operator_delete(int*);

extern "C" void func_00242518(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242540);
#ifdef SKIP_ASM
struct sMovieNode {
    char pad_0x0[0xbc];
    sMovieNode* next; // 0xbc
    sMovieNode* prev; // 0xc0
};

struct sMovieList {
    sMovieNode* head; // 0x0
    int count;        // 0x4
};

extern "C" void func_00242540(sMovieList* list, sMovieNode* node)
{
    node->next = list->head;
    node->prev = 0;
    if (list->head != 0) {
        list->head->prev = node;
    }
    list->head = node;
    list->count++;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242570);
#ifdef SKIP_ASM
extern "C" void func_00242570(sMovieList* list, sMovieNode* node)
{
    if (node->prev != 0) {
        node->prev->next = node->next;
    }
    if (node->next != 0) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    list->count--;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002425C0);
#ifdef SKIP_ASM
extern "C" void func_002429B0(void* self, const void* eye, void* frustum);
extern "C" void func_00229F88(int index, void* v, void* m);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));
extern "C" int func_00242978(const void* a, const void* b);

struct sVec4_25C0 {
    float x, y, z, w;
    sVec4_25C0() {}
    sVec4_25C0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sMtx_25C0 {
    sVec4_25C0 r[4];
};

struct sBox_25C0 {
    sVec4_25C0 min;
    sVec4_25C0 max;
};

struct sBounds_25C0 {
    float minx, miny, minz, pad0C;
    float maxx, maxy, maxz;
};

struct sView_25C0 {
    sVec4_25C0 pos;         // 0x00
    char pad10[0x50 - 0x10];
    sVec4_25C0 normal;      // 0x50
    char pad60[0xA0 - 0x60];
    float minx, miny, minz; // 0xA0
    float maxx, maxy, maxz; // 0xAC
    char padB8[4];
    sView_25C0* next;       // 0xBC
};

struct sVisEnt_25C0 {
    sView_25C0* view;
    float dist;
};

struct sVisMgr_25C0 {
    sView_25C0* head;       // 0x00
    int enabled;            // 0x04
    sVisEnt_25C0 ents[32];  // 0x08
    int count;              // 0x108
    int nvis;               // 0x10C
    int mask;               // 0x110
};

struct sCamVt_25C0 {
    short delta;
    short index;
    void* fn;
};

struct sCam_25C0 {
    char pad0[0x10D8];
    sCamVt_25C0* vt;        // 0x10D8
};


typedef sMtx_25C0 (*tCamMtx_25C0)(void*);
typedef sMtx_25C0* (*tCamMtxP_25C0)(void*);
typedef bool (*tCamCull_25C0)(void*, sVec4_25C0*, sVec4_25C0*, sMtx_25C0*);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_25C0 vu0Scale_25C0(const sVec4_25C0& v, float s)
{
    sVec4_25C0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_25C0(const sVec4_25C0& a, const sVec4_25C0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (squared 3D distance).
static inline float vu0DistSq_25C0(const sVec4_25C0& a, const sVec4_25C0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf4, %3\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf7, $vf5, $vf5\n"
        "vadday.x  ACC, $vf7, $vf7y\n"
        "vmaddz.x  $vf7, $vf6, $vf7z\n"
        "qmfc2.ni  %1, $vf7\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline sVec4_25C0 rowPos_25C0(const sMtx_25C0* m)
{
    return m->r[3];
}

static inline sVec4_25C0 mulT_25C0(const sMtx_25C0* m, const sVec4_25C0& v)
{
    return sVec4_25C0(v.x * m->r[0].x + v.y * m->r[0].y + v.z * m->r[0].z + v.w * m->r[0].w,
                      v.x * m->r[1].x + v.y * m->r[1].y + v.z * m->r[1].z + v.w * m->r[1].w,
                      v.x * m->r[2].x + v.y * m->r[2].y + v.z * m->r[2].z + v.w * m->r[2].w,
                      v.x * m->r[3].x + v.y * m->r[3].y + v.z * m->r[3].z + v.w * m->r[3].w);
}

static inline sMtx_25C0* camMtx_25C0()
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    return ((tCamMtxP_25C0)((sCamVt_25C0*)ps2GraphicsMan->mVtbl)[0x23].fn)((char*)ps2GraphicsMan + ((sCamVt_25C0*)ps2GraphicsMan->mVtbl)[0x23].delta);
}

extern "C" void func_002425C0(sVisMgr_25C0* self, sBounds_25C0* bb)
{
    self->nvis = 0;
    self->mask = 0;
    if (self->enabled == 0)
        return;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sMtx_25C0 mtx = ((tCamMtx_25C0)((sCamVt_25C0*)ps2GraphicsMan->mVtbl)[0x2B].fn)((char*)ps2GraphicsMan + ((sCamVt_25C0*)ps2GraphicsMan->mVtbl)[0x2B].delta);
    sVec4_25C0 eye = vu0Scale_25C0(rowPos_25C0(camMtx_25C0()), -1.0f);
    eye = mulT_25C0(camMtx_25C0(), eye);
    self->count = 0;
    eye.w = 1.0f;
    sView_25C0* it = self->head;
    while (it != 0) {
        sView_25C0* candidate = it;
        it = it->next;
        sBox_25C0 box;
        box.min.x = candidate->minx;
        box.min.y = candidate->miny;
        box.min.z = candidate->minz;
        box.min.w = 1.0f;
        box.max.x = candidate->maxx;
        box.max.y = candidate->maxy;
        box.max.z = candidate->maxz;
        box.max.w = 1.0f;
        int ov = bb->minx <= box.max.x && box.min.x <= bb->maxx
              && bb->miny <= box.max.y && box.min.y <= bb->maxy
              && bb->minz <= box.max.z && box.min.z <= bb->maxz;
        if (ov) {
            cPS2GraphicsMan* graphics = GrMan;
            bool vis = !((tCamCull_25C0)((sCamVt_25C0*)graphics->mVtbl)[0x5D].fn)((char*)graphics + ((sCamVt_25C0*)graphics->mVtbl)[0x5D].delta, &box.min, &box.max, &mtx);
            if (vis) {
                if (!(0.0f < vu0Dot_25C0(eye, candidate->normal))) {
                    float distSq = vu0DistSq_25C0(eye, candidate->pos);
                    self->ents[self->count].view = candidate;
                    self->ents[self->count].dist = distSq;
                    self->count++;
                }
            }
        }
    }
    if (self->count == 0)
        return;
    func_00418EF8(self->ents, self->count, 8, func_00242978);
    int n = self->count;
    if (n > 2)
        n = 2;
    self->nvis = n;
    for (int i = 0; i < self->nvis; i++) {
        self->mask |= 1 << i;
        func_002429B0(self, &eye, self->ents[i].view);
        func_00229F88(i, &self->ents[i].view->normal, (char*)self->ents[i].view + 0x60);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00242978);
#ifdef SKIP_ASM
// qsort-style comparator: orders by the float at +0x4, ascending.
extern "C" int func_00242978(const void* lhs, const void* rhs)
{
    float fa = *(float*)((char*)lhs + 0x4);
    float fb = *(float*)((char*)rhs + 0x4);
    if (fa < fb) {
        return -1;
    }
    if (fb < fa) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_002429B0);
#ifdef SKIP_ASM
struct sVec4_29B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPlane_29B0 {
    float x, y, z, d;
};

struct sFrustum_29B0 {
    sVec4_29B0 pad0;
    sVec4_29B0 corner[4];   // 0x10
    sVec4_29B0 pad50;
    sPlane_29B0 plane[4];   // 0x60
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_29B0 vu0Sub_29B0(const sVec4_29B0& a, const sVec4_29B0& b)
{
    sVec4_29B0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec4_29B0 vu0Cross_29B0(const sVec4_29B0& a, const sVec4_29B0& b)
{
    sVec4_29B0 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_29B0 vu0Normalize_29B0(const sVec4_29B0& v)
{
    sVec4_29B0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_29B0(const sVec4_29B0& a, const sVec4_29B0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline void makePlane_29B0(sFrustum_29B0* f, int i, const sVec4_29B0& eye, const sVec4_29B0& c0, const sVec4_29B0& c1)
{
    sVec4_29B0 a;
    sVec4_29B0 b;
    sVec4_29B0 n;
    a = vu0Sub_29B0(c1, eye);
    b = vu0Sub_29B0(c0, eye);
    n = vu0Normalize_29B0(vu0Cross_29B0(a, b));
    n.w = -vu0Dot_29B0(eye, n);
    f->plane[i].x = n.x;
    f->plane[i].y = n.y;
    f->plane[i].z = n.z;
    f->plane[i].d = n.w;
}

extern "C" void func_002429B0(void* self, const void* eyep, void* frustumPtr)
{
    const sVec4_29B0& eye = *(const sVec4_29B0*)eyep;
    sFrustum_29B0* frustum = (sFrustum_29B0*)frustumPtr;
    makePlane_29B0(frustum, 0, eye, frustum->corner[0], frustum->corner[1]);
    makePlane_29B0(frustum, 1, eye, frustum->corner[1], frustum->corner[2]);
    makePlane_29B0(frustum, 2, eye, frustum->corner[2], frustum->corner[3]);
    makePlane_29B0(frustum, 3, eye, frustum->corner[3], frustum->corner[0]);
}
#endif

