#include "common.h"
#include "fe/festatenet.h"
#include "fe/festatelogin.h"
#include "fe/festatenetcreateroompopup.h"
#include "fe/festateauthdnas.h"
#include "fe/festatenetquickmessages.h"
#include "fe/festatenetquickplaypopup.h"
#include "fe/festateonlinecharacterselect.h"
#include "fe/festatecreateduplicate.h"
#include "fe/festatenetchallenge.h"
#include "fe/festatenetaccount.h"
#include "fe/festatenetlobby.h"
#include "fe/festatenetcreate.h"
#include "fe/festatenetconfig.h"
#include "fe/messagecenter.h"
#include "fe/festatenetmessenger.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_156thx[];
extern char* D_004A1540;
extern char str_Text0000[];
extern char str_kT_THXBrightness[];

class cUIObjK1DEB90 {
public:
    char pad_0x000[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int);
};

/**
 * Opens the THX video-calibration popup ("156thx") on its brightness page.
 *
 * Shows the "brightps2" image and sets Text0000 to kT_THXBrightness.
 */
extern "C" void cFEPopupVideoCalibration_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_156thx), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIObjK1DEB90* obj = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1540));
    if (obj != 0) {
        obj->v09(1);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Text0000));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_THXBrightness));
    }
    *(char*)((char*)self + 0x48) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onInputBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// D_004A1540 is an 8-byte string table; this reads its second entry.
extern char* D_004A1540_DEC68[2] __asm__("D_004A1540");
extern char str_kT_THXBars[];
extern char str_Text0001[];

struct sVEi_EC68 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVEm_EC68 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDefObj_EC68 {
    sVEm_EC68* vt;
};
extern sDefObj_EC68 D_004A5A58_EC68 __asm__("gUISoundNull");

struct sPopup_EC68 {
    char pad0[0x10];
    char* owner;                // 0x10
    char pad14[0x8];            // 0x14
    int flags;                  // 0x1C
    char pad20[0x20];           // 0x20
    void* screen;               // 0x40
    char pad44[0x4];            // 0x44
    signed char shown;          // 0x48
};

/**
 * Steps the video-calibration popup on input slot 5 (probably accept): first to the colour-bars page, then
 * marks it done.
 *
 * The bars page shows "barsps2" in place of "brightps2", sets Text0000 to kT_THXBars and hides Text0001. Each
 * press plays UI sound 6.
 *
 * @param in  the input (On Tour cUIInput* pInput)
 * @return 1 (the popup takes every input)
 */
extern "C" int cFEPopupVideoCalibration_onInputBegin(sPopup_EC68* self, char* in)
{
    sVEi_EC68* vt = *(sVEi_EC68**)(in + 8);
    if (vt[5].fn(in + vt[5].delta) != 0) {
        if (self->shown == 0) {
            cUIObjK1DEB90* obj = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A1540));
            if (obj != 0) {
                obj->v09(0);
            }
            obj = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A1540_DEC68[1]));
            if (obj != 0) {
                obj->v09(1);
            }
            cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Text0000));
            if (text != 0) {
                cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_THXBars));
            }
            obj = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Text0001));
            if (obj != 0) {
                obj->v09(0);
            }
            self->shown = 1;
        } else {
            self->flags = ((self->flags | 0x80) & ~0x3F00) | 0x700;
        }
        sDefObj_EC68* uiSound = *(sDefObj_EC68**)(self->owner + 0x14);
        if (uiSound == 0)
            uiSound = &D_004A5A58_EC68;
        if (uiSound != 0)
            uiSound->vt[2].fn((char*)uiSound + uiSound->vt[2].delta, 6);
    }
    return 1;
}

extern void* cFEPopupScreenPos_vtbl[];
extern "C" void* func_0039E390(void*);
#endif

extern void* cFEPopupScreenPos_vtbl[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos__cFEPopupScreenPos__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the screen-position popup through the UI-state destructor func_0039E390().
 *
 * Restores cFEPopupScreenPos_vtbl first. Only @p self is declared: the destructor flags the caller passed in the
 * second argument register reach func_0039E390() unchanged.
 *
 * @return no meaningful value (func_0039E390() returns nothing)
 */
void* cFEPopupScreenPos__cFEPopupScreenPos(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEPopupScreenPos_vtbl;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_24screen_position[];

struct sOptions_EE10 {
    unsigned int flags;
    int pad_4 : 8;
    int b5 : 8;
    int b6 : 8;
    int pad_7 : 8;
    int data[(0x288 - 8) / 4];
};
extern sOptions_EE10 D_00535610_EE10 __asm__("cBEOptionInterface_mOption");

static inline int optB6_EE10(sOptions_EE10 o)
{
    return o.b6;
}

static inline int optB5_EE10(sOptions_EE10 o)
{
    return o.b5;
}

/**
 * Opens the screen-position popup ("24screen_position") and loads the current screen offset from the options.
 */
extern "C" void cFEPopupScreenPos_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_24screen_position), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        *(int*)((char*)self + 0xC) = GetHashValue32(str_24screen_position);
    }
    void* be = cBE_getBE();
    cBE_getInterface_Fv(be, 4);
    int* offsetX = (int*)((char*)self + 0x48);
    *offsetX = optB6_EE10(D_00535610_EE10);
    int* p4C = (int*)((char*)self + 0x4C);
    *p4C = optB5_EE10(D_00535610_EE10);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_getData);
#ifdef SKIP_ASM
/**
 * Returns one of the screen-position popup's results.
 *
 * @param i  0 for the horizontal offset, 1 for the vertical offset (both clamped to -20..20 by the popup's
 *           update), anything else for the accepted flag
 * @return the x offset, the y offset, or 1 when the popup was accepted and 0 when it was cancelled
 */
extern "C" int cFEPopupScreenPos_getData(void* self, int i)
{
    switch (i) {
    case 0:
        return *(int*)((char*)self + 0x48);
    case 1:
        return *(int*)((char*)self + 0x4c);
    }
    return *(signed char*)((char*)self + 0x50);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onUpdate);
#ifdef SKIP_ASM
struct cUIScreen;
// PORT: the unit declares cUIScreen_getFrameByLabel as returning int; the frame id is an unsigned short.
unsigned short cUIScreen_getFrameByLabel_us(cUIScreen* screen, int hash) __asm__("cUIScreen_getFrameByLabel__FP9cUIScreeni");
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039F190(void* self, int a1);
extern char str_hl_left[];
extern char str_hl_up[];
extern char str_hl_down[];
extern char str_hl_right[];

struct sVEm_F050 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDefObj_F050 {
    sVEm_F050* vt;
};
extern sDefObj_F050 gUISoundNull;

struct sVEi_F050 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVEs_F050 {
    short delta;
    short index;
    void (*fn)(void*, unsigned char);
};

struct sVEp_F050 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

struct sPopup_F050 {
    char pad0[0x10];
    char* owner;                // 0x10
    char pad14[0x8];            // 0x14
    int flags;                  // 0x1C
    char pad20[0x20];           // 0x20
    void* screen;               // 0x40
    char pad44[0x4];            // 0x44
    int x;                      // 0x48
    int y;                      // 0x4C
    signed char accepted;       // 0x50
};

static inline int padCall_F050(char* in, int slot)
{
    sVEi_F050* vt = *(sVEi_F050**)(in + 8);
    return vt[slot].fn(in + vt[slot].delta);
}

static inline void sound_F050(sPopup_F050* s, int msg)
{
    sDefObj_F050* p = *(sDefObj_F050**)(s->owner + 0x14);
    if (p == 0)
        p = &gUISoundNull;
    if (p != 0)
        p->vt[2].fn((char*)p + p->vt[2].delta, msg);
}

static inline void playFrame_F050(sPopup_F050* s, char* name)
{
    unsigned short frame = cUIScreen_getFrameByLabel_us((cUIScreen*)s->screen, GetHashValue32(name));
    if (frame != 0xFFFF)
        cUIScreen_playFrame(s->screen, frame, 1);
}

static inline int clamp_F050(int v, int lo, int hi)
{
    if (v >= lo) {
        if (v > hi) {
            v = hi;
        }
    } else {
        v = lo;
    }
    return v;
}

/**
 * Moves the screen offset with the first pad that has input, or closes the popup on accept (keep) or back
 * (cancel).
 *
 * The offset stays within -20..20 on each axis (UI sound 4 when pushed past it) and is applied at once
 * through
 * the renderer (GrMan slot 12). Accept plays sound 6 and back sound 3.
 */
extern "C" void cFEPopupScreenPos_onUpdate(sPopup_F050* self)
{
    char* in = *(char**)(self->owner + 0xC);
    for (int i = 0; i < 2; i++) {
        sVEs_F050* vt = *(sVEs_F050**)(in + 8);
        vt[44].fn(in + vt[44].delta, i);
        if (padCall_F050(in, 40) == 0) {
            continue;
        }
        if (padCall_F050(in, 5)) {
            sound_F050(self, 6);
            self->accepted = 1;
            self->flags |= 0x80;
            func_0039F190(self->owner + 0x18, 1);
        } else if (padCall_F050(in, 6)) {
            sound_F050(self, 3);
            self->accepted = 0;
            self->flags |= 0x80;
            func_0039F190(self->owner + 0x18, 1);
        } else if (padCall_F050(in, 19)) {
            self->x--;
            playFrame_F050(self, str_hl_left);
        } else if (padCall_F050(in, 20)) {
            self->x++;
            playFrame_F050(self, str_hl_right);
        } else if (padCall_F050(in, 17)) {
            self->y++;
            playFrame_F050(self, str_hl_up);
        } else if (padCall_F050(in, 18)) {
            self->y--;
            playFrame_F050(self, str_hl_down);
        }
        if (self->x < -20 || self->x > 20 || self->y < -20 || self->y > 20) {
            sound_F050(self, 4);
        }
        self->x = clamp_F050(self->x, -20, 20);
        self->y = clamp_F050(self->y, -20, 20);
        sVEp_F050* rv = (sVEp_F050*)GrMan->mVtbl;
        rv[12].fn((char*)GrMan + rv[12].delta, self->x, self->y);
        return;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the screen-position popup's lose transition at once.
 *
 * The state stack update func_0039ECB0() ends a state's lose transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cFEPopupScreenPos_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF380);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateRiderBio_vtbl[];

extern "C" void* func_001DF380(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateRiderBio_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRiderBio__cFEStateRiderBio__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rider-bio front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRiderBio__cFEStateRiderBio(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF3E0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateRuleSelect_vtbl[];

extern "C" void* func_001DF3E0(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateRuleSelect_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRuleSelect__cFEStateRuleSelect__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rule-select front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRuleSelect__cFEStateRuleSelect(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF440);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046B078[];

extern "C" void func_001DF440(void* self, int flags)
{
    *(void***)((char*)self + 0x30) = D_0046B078;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF470);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new[](unsigned int, void* p) { return p; }
extern "C" void* func_001A8500(void*, int, int);
extern "C" void func_001926F0(void* self);
extern void* cFEStateBraggingRights_vtbl[];

// Element type of an inline-constructed array (0x34 bytes, constructed by func_001926F0).
struct sElem_F470 {
    char pad[0x34];
    sElem_F470() { func_001926F0(this); }
};

extern "C" void* func_001DF470(void* self)
{
    func_001A8500(self, 0, 0);
    *(void***)((char*)self + 0x8) = cFEStateBraggingRights_vtbl;
    new ((char*)self + 0x6D0) sElem_F470[2];
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateBraggingRights__cFEStateBraggingRights);
#ifdef SKIP_ASM
extern void* cFEStateBraggingRights_vtbl[];
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

struct sVEntry001DF4E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys the bragging-rights front-end state: its two embedded 0x34-byte members (+0x6D0), then the base
 * state.
 *
 * The base destructor func_001A85D0 frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateBraggingRights__cFEStateBraggingRights(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cFEStateBraggingRights_vtbl;
    if ((char*)self + 0x6D0 != 0) {
        char* panel = (char*)self + 0x6D0 + 0x68;
        while ((char*)self + 0x6D0 != panel) {
            panel -= 0x34;
            sVEntry001DF4E8* vt = *(sVEntry001DF4E8**)(panel + 0x30);
            vt[1].fn(panel + vt[1].delta, 0);
        }
    }
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateBraggingRights_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
void cFEStateBraggingRights_onMenuGainFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF580);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateChatRoom_vtbl[];

extern "C" void* func_001DF580(void* self)
{
    func_001A8500(self, 0, 0);
    *(int*)((char*)self + 0x6F8) = 0;
    *(void***)((char*)self + 0x8) = cFEStateChatRoom_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChatRoom_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
void cFEStateChatRoom_onMenuGainFocus(void* self)
{
}
#endif

extern void* cFEStateOnlineMainMenu_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineMainMenu_cFEStateOnlineMainMenu1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateOnlineMainMenu_cFEStateOnlineMainMenu1(cFEStateNet* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateOnlineMainMenu_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineMainMenu__cFEStateOnlineMainMenu__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the online main-menu state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateOnlineMainMenu__cFEStateOnlineMainMenu(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateOnlineMainMenu_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineAgreement_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the terms-of-service state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateOnlineAgreement_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateNetQuickPlayPopup_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1(cFEStateNetQuickPlayPopup* self) {
    func_001A8500(self, 0, 0);
    (self->mbSearching) = 0;
    (self->mVtbl) = cFEStateNetQuickPlayPopup_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the quick-match popup state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateNetQuickPlayPopup_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the quick-match popup: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateNetQuickPlayPopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateNetSSXWorld_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetSSXWorld_cFEStateNetSSXWorld1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateNetSSXWorld_cFEStateNetSSXWorld1(cFEStateNet* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateNetSSXWorld_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetSSXWorld__cFEStateNetSSXWorld__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the SSX World hub state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateNetSSXWorld__cFEStateNetSSXWorld(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateNetSSXWorld_vtbl;
    return func_001A85D0(self);
}
#endif

extern void* cFEStateNetQuickMessages_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickMessages_cFEStateNetQuickMessages1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateNetQuickMessages_cFEStateNetQuickMessages1(cFEStateNetQuickMessages* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateNetQuickMessages_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickMessages__cFEStateNetQuickMessages__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the quick-messages editor state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateNetQuickMessages__cFEStateNetQuickMessages(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateNetQuickMessages_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF778);
#ifdef SKIP_ASM
extern "C" int func_001DF778(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6D0)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLobbyMenu_cFEStateLobbyMenu1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateLobbyMenu_vtbl[];
extern int cBXString_EmptyString;

extern "C" void* cFEStateLobbyMenu_cFEStateLobbyMenu1(cFEStateLobbyMenu* self)
{
    func_001A8500(self, 0, 0);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEStateLobbyMenu_vtbl;
    self->mszChannelName = (char*)emptyStr;
    self->mszRoomName = (char*)emptyStr;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLobbyMenu__cFEStateLobbyMenu);
#ifdef SKIP_ASM
extern void* cFEStateLobbyMenu_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the lobby screen state: frees its room and channel name strings, then runs the cFEStateNet destructor.
 *
 * Restores cFEStateLobbyMenu_vtbl, destroys the cBXStrings mszRoomName (+0xB80) and mszChannelName (+0x76C) and
 * calls func_001A85D0 with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateLobbyMenu__cFEStateLobbyMenu(cFEStateLobbyMenu* self, int flags)
{
    self->mVtbl = cFEStateLobbyMenu_vtbl;
    cBXString__cBXString((char*)self + 0xB80, 2);
    cBXString__cBXString((char*)self + 0x76C, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateNetCreateRoomPopup_vtbl[];
extern int cBXString_EmptyString;

extern "C" void* cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1(cFEStateNetCreateRoomPopup* self)
{
    func_001A8500(self, 0, 0);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEStateNetCreateRoomPopup_vtbl;
    self->mszRoomName = (char*)emptyStr;
    self->mszChannelName = (char*)emptyStr;
    self->mszPassword = (char*)emptyStr;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup);
#ifdef SKIP_ASM
extern void* cFEStateNetCreateRoomPopup_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the create-room popup: frees its three strings (mszPassword, mszChannelName, mszRoomName), then runs the
 * cFEStateNet destructor (func_001A85D0).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup(cFEStateNetCreateRoomPopup* self, int flags)
{
    self->mVtbl = cFEStateNetCreateRoomPopup_vtbl;
    cBXString__cBXString((char*)self + 0x6E4, 2);
    cBXString__cBXString((char*)self + 0x6E0, 2);
    cBXString__cBXString((char*)self + 0x6DC, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the create-room popup: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateNetCreateRoomPopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateNetHelpPopup_vtbl[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getRoomName);
#ifdef SKIP_ASM
extern "C" int cFEStateNetCreateRoomPopup_getRoomName(void *roomPopup) {
    return (*(int *)((char*)(roomPopup) + (0x6DC)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getChannelName);
#ifdef SKIP_ASM
extern "C" int cFEStateNetCreateRoomPopup_getChannelName(void *roomPopup) {
    return (*(int *)((char*)(roomPopup) + (0x6E0)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getPassword);
#ifdef SKIP_ASM
extern "C" int cFEStateNetCreateRoomPopup_getPassword(void *roomPopup) {
    return (*(int *)((char*)(roomPopup) + (0x6E4)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF900);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateNetHelpPopup_vtbl[];

// Element type of an inline-constructed array; its empty constructor leaves only the loop counters.
struct sSlot_F900 {
    int v;
    sSlot_F900() {}
};

extern "C" void* func_001DF900(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateNetHelpPopup_vtbl;
    new ((char*)self + 0x20) sSlot_F900[7];
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup__cFEStateNetHelpPopup__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the network help popup through the UI-state destructor func_0039E390().
 *
 * Restores cFEStateNetHelpPopup_vtbl first. Only @p self is declared: the destructor flags the caller passed in
 * the second argument register reach func_0039E390() unchanged.
 *
 * @return no meaningful value (func_0039E390() returns nothing)
 */
void* cFEStateNetHelpPopup__cFEStateNetHelpPopup(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateNetHelpPopup_vtbl;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the network help popup's lose transition at once.
 *
 * The state stack update func_0039ECB0() ends a state's lose transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cFEStateNetHelpPopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF990);
#ifdef SKIP_ASM
extern "C" void func_001DF990(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x54))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF998);
#ifdef SKIP_ASM
extern "C" void func_001DF998(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x64))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9A0);
#ifdef SKIP_ASM
extern "C" void func_001DF9A0(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x50))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9A8);
#ifdef SKIP_ASM
extern "C" int func_001DF9A8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x50)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9B0);
#ifdef SKIP_ASM
extern "C" int func_001DF9B0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xC8))) == 1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9C0);
#ifdef SKIP_ASM
extern "C" int func_001DF9C0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xA60)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9C8);
#ifdef SKIP_ASM
extern "C" void func_001DF9C8(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0xAE4))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9D0);
#ifdef SKIP_ASM
extern "C" int func_001DF9D0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xAE4)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DF9D8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_0046A3C8[];

extern "C" void* func_001DF9D8(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA10);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFA10(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA70);
#ifdef SKIP_ASM
extern "C" int func_001DFA70(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8F0)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA78);
#ifdef SKIP_ASM
extern "C" unsigned int func_001DFA78(void *arg0) {
    return (unsigned int) ~(*(int *)((char*)(arg0) + (0x8F0))) >> 0x1F;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA88__FPv);
#ifdef SKIP_ASM
int func_001DFA88(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFA90);
#ifdef SKIP_ASM
extern "C" int func_001DFA90(void *arg0) {
    return ((int) (*(int *)((char*)(arg0) + (0x48))) >> 3) & 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAA0__FPv);
#ifdef SKIP_ASM
int func_001DFAA0(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAA8__FPv);
#ifdef SKIP_ASM
int func_001DFAA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAB0);
#ifdef SKIP_ASM
extern "C" int func_001DFAB0(void *arg0) {
    return ((int) (*(int *)((char*)(arg0) + (0x48))) >> 1) & 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAC0__FPv);
#ifdef SKIP_ASM
void func_001DFAC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAC8__FPv);
#ifdef SKIP_ASM
void func_001DFAC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAD0__FPv);
#ifdef SKIP_ASM
int func_001DFAD0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAD8__FPv);
#ifdef SKIP_ASM
int func_001DFAD8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAE0__FPv);
#ifdef SKIP_ASM
int func_001DFAE0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAE8__FPv);
#ifdef SKIP_ASM
int func_001DFAE8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFAF0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_0046A248[];

extern "C" void* func_001DFAF0(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_0046A248;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB28);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFB28(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB88__FPvi);
#ifdef SKIP_ASM
int func_001DFB88(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFB98);
#ifdef SKIP_ASM
extern "C" int func_001DFB98(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBB0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFBB0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBB8__FPvT0);
#ifdef SKIP_ASM
int func_001DFBB8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBC0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_0046A0C8[];

extern "C" void* func_001DFBC0(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_0046A0C8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFBF8);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFBF8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC58__FPvi);
#ifdef SKIP_ASM
int func_001DFC58(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC68);
#ifdef SKIP_ASM
extern "C" int func_001DFC68(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC80__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFC80(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC88__FPvT0);
#ifdef SKIP_ASM
int func_001DFC88(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFC90);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_00469F48[];

extern "C" void* func_001DFC90(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_00469F48;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFCC8);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFCC8(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD28__FPvi);
#ifdef SKIP_ASM
int func_001DFD28(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD38);
#ifdef SKIP_ASM
extern "C" int func_001DFD38(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD50__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFD50(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD58__FPvT0);
#ifdef SKIP_ASM
int func_001DFD58(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD60);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_00469DC8[];

extern "C" void* func_001DFD60(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_00469DC8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFD98);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFD98(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFDF8__FPvi);
#ifdef SKIP_ASM
int func_001DFDF8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE08);
#ifdef SKIP_ASM
extern "C" int func_001DFE08(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE20__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFE20(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE28__FPvT0);
#ifdef SKIP_ASM
int func_001DFE28(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE30);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_00469C48[];

extern "C" void* func_001DFE30(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_00469C48;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFE68);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFE68(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEC8__FPvi);
#ifdef SKIP_ASM
int func_001DFEC8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFED8);
#ifdef SKIP_ASM
extern "C" int func_001DFED8(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEF0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFEF0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFEF8__FPvT0);
#ifdef SKIP_ASM
int func_001DFEF8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFF00);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* D_00469AC8[];

extern "C" void* func_001DFF00(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = D_00469AC8;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFF38);
#ifdef SKIP_ASM
extern void* D_0046A3C8[];
extern void* D_0046D0D0[];
void func_001DFAC8(void* self);
extern "C" void func_001D00C0(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

extern "C" void func_001DFF38(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_0046A3C8;
    func_001DFAC8(self);
    func_001D00C0(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFF98__FPvi);
#ifdef SKIP_ASM
int func_001DFF98(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0xA28);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFA8);
#ifdef SKIP_ASM
extern "C" int func_001DFFA8(void* self, void* a1)
{
    int value = *(short*)((char*)a1 + 0xc);
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFC0__FPvT0);
#ifdef SKIP_ASM
signed char func_001DFFC0(void* self, void* other)
{
    return *(signed char*)((char*)other + 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFC8__FPvT0);
#ifdef SKIP_ASM
int func_001DFFC8(void* self, void* other)
{
    return *(int*)((char*)other + 0x0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001DFFD0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStatePreviewReward_vtbl[];

extern "C" void* func_001DFFD0(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStatePreviewReward_vtbl;
    return self;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0008);
#ifdef SKIP_ASM
extern "C" int func_001E0008(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0010);
#ifdef SKIP_ASM
extern "C" int func_001E0010(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0018);
#ifdef SKIP_ASM
extern "C" int func_001E0018(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0020__FPv);
#ifdef SKIP_ASM
int func_001E0020(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0028__FPv);
#ifdef SKIP_ASM
int func_001E0028(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0030);
#ifdef SKIP_ASM
extern "C" void func_001E0030(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x270))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0038);
#ifdef SKIP_ASM
extern "C" void func_001E0038(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x258))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0040);
#ifdef SKIP_ASM
extern "C" void func_001E0040(void *arg0, int value) {
    (*(int *)((char*)(arg0) + (0x254))) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0048);
#ifdef SKIP_ASM
struct sBlock24 {
    int w[9];
};

extern "C" void func_001E0048(void* self, void* src)
{
    *(sBlock24*)((char*)self + 0x24C) = *(sBlock24*)src;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0098);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateMountainRoom_vtbl[];

extern "C" void* func_001E0098(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateMountainRoom_vtbl;
    return self;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E00D0);
#ifdef SKIP_ASM
extern "C" int func_001E00D0(void *arg0) {
    return ~(*(int *)((char*)(arg0) + (0xBC))) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E00E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile).
class cUIThing {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void setEnabled(int on);
    virtual void setFocus(int on);
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

extern "C" void func_001E00E0(void* self)
{
    (*(cUIThing**)((char*)self + 0x48))->setVisible(0);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0110);
#ifdef SKIP_ASM
extern "C" int func_001E0110(void *arg0) {
    int var_2;
    void *temp_3;

    temp_3 = (*(void **)((char*)(arg0) + (0x48)));
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = ((int) (*(int *)((char*)(temp_3) + (0x14))) >> 6) & 1;
    }
    return var_2;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0130);
#ifdef SKIP_ASM
extern "C" int func_001E0130(void *arg0) {
    int var_2;
    int var_5;
    void *temp_3;
    void *temp_4;

    temp_3 = (*(void **)((char*)(arg0) + (0x48)));
    var_5 = 0;
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = ((int) (*(int *)((char*)(temp_3) + (0x14))) >> 6) & 1;
    }
    if (var_2 != 0) {
        temp_4 = (*(void **)((char*)(arg0) + (0x4C)));
        if (temp_4 != 0) {
            var_5 = ((int) (*(int *)((char*)(temp_4) + (0x14))) >> 6) & 1;
        }
    }
    return var_5;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0178);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStatePeakRoom_vtbl[];

extern "C" void* func_001E0178(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStatePeakRoom_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePeakRoom__cFEStatePeakRoom__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the peak-room front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStatePeakRoom__cFEStatePeakRoom(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E01D8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateTrophyRoom_vtbl[];

extern "C" void* func_001E01D8(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateTrophyRoom_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateTrophyRoom__cFEStateTrophyRoom);
#ifdef SKIP_ASM
extern void* cFEStateTrophyRoom_vtbl[];
extern void* D_0046D0D0[];
extern "C" void func_001D4B20(void* self);
// PORT: unit declares func_0039E390 with one arg; the body takes (self, flags)
extern "C" void func_0039E390_dtor(void* self, int flags) __asm__("func_0039E390");

/**
 * Destroys the trophy-room front-end state: releases its five loaded images (func_001D4B20), then the base
 * states.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateTrophyRoom__cFEStateTrophyRoom(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cFEStateTrophyRoom_vtbl;
    func_001D4B20(self);
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0268);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateRewardsRoom_vtbl[];

extern "C" void* func_001E0268(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateRewardsRoom_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardsRoom__cFEStateRewardsRoom__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rewards-room front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRewardsRoom__cFEStateRewardsRoom(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

extern void* cFEPopupSelectMultiplayerMode_vtbl[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E02C8);
#ifdef SKIP_ASM
extern "C" int func_001E02C8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x4C)));
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode__cFEPopupSelectMultiplayerMode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the multiplayer-mode selection popup through the UI-state destructor func_0039E390().
 *
 * Restores cFEPopupSelectMultiplayerMode_vtbl first. Only @p self is declared: the destructor flags the caller
 * passed in the second argument register reach func_0039E390() unchanged.
 *
 * @return no meaningful value (func_0039E390() returns nothing)
 */
void* cFEPopupSelectMultiplayerMode__cFEPopupSelectMultiplayerMode(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEPopupSelectMultiplayerMode_vtbl;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_86multiplayermode[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Opens the multiplayer-mode popup ("86multiplayermode").
 */
extern "C" void cFEPopupSelectMultiplayerMode_onCreateScreen(void* self)
{
    int hash = GetHashValue32(str_86multiplayermode);
    *(int*)((char*)self + 0xC) = hash;
    void* screen = cUIEngine_addScreenByHashName(*(void**)((char*)self + 0x10), self, hash, 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIMenu_setScrollableMenu(void* self, int a1);
extern char str_menu0000[];
extern char D_004A13A0[];
extern char D_004A14F8[];

/**
 * Sets up the multiplayer-mode popup's widgets: menu0000 becomes a wrapping two-item menu, items "0" and "1"
 * get indices 0 and 1.
 *
 * @param item  the widget being created (On Tour cUIThing* pWidget)
 */
extern "C" void cFEPopupSelectMultiplayerMode_onWidgetCreate(void* self, void* item)
{
    int itemHash = *(int*)((char*)item + 0x38);
    if (itemHash == GetHashValue32(str_menu0000)) {
        *(int*)((char*)item + 0x14) |= 1;
        cUIMenu_setScrollableMenu(item, 2);
        *(int*)((char*)item + 0x14) |= 0x80;
        return;
    }
    int itemHash2 = *(int*)((char*)item + 0x38);
    if (itemHash2 == GetHashValue32(D_004A13A0)) {
        *(int*)((char*)item + 0x18) = 0;
        return;
    }
    int itemHash3 = *(int*)((char*)item + 0x38);
    if (itemHash3 == GetHashValue32(D_004A14F8)) {
        *(int*)((char*)item + 0x18) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetEvent);
#ifdef SKIP_ASM
extern int D_004A19D4;
extern "C" void func_0039F190(void* self, int a1);

/**
 * Closes the multiplayer-mode popup: select (5) records the chosen mode, back (6) cancels.
 *
 * Select stores whether item 1 was chosen in D_004A19D4, which the front end checks when it starts the next
 * multiplayer state.
 *
 * @param pWidget  the widget that sent the event
 * @param Event  the widget event
 */
extern "C" void cFEPopupSelectMultiplayerMode_onWidgetEvent(void* self, void* pWidget, int Event)
{
    switch (Event) {
    case 5: {
        char* engine = *(char**)((char*)self + 0x10);
        *(char*)((char*)self + 0x48) = 1;
        D_004A19D4 = *(int*)((char*)pWidget + 0x18) != 0;
        func_0039F190(engine + 0x18, 1);
        break;
    }
    case 6: {
        char* engine = *(char**)((char*)self + 0x10);
        *(char*)((char*)self + 0x48) = 0;
        func_0039F190(engine + 0x18, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

/**
 * Handles the multiplayer-mode popup's menu events by moving the selection on scroll events.
 *
 * cUIMenu_onInputChangeItem() sends event 2 when the menu scrolls up and 3 when it scrolls down. Event 3 selects
 * item @p idx + 1; event 2 selects @p idx - 1 and then, falling through into event 3's case, @p idx + 1.
 *
 * @param menu  the menu that sent the event
 * @param key   menu event number (On Tour: Event)
 * @param idx   the event's index argument (the menu's scroll offset for events 2 and 3)
 * @return always 0x101, the base handler's result
 */
extern "C" int cFEPopupSelectMultiplayerMode_onWidgetMenu(void* self, void* menu, unsigned int key, int idx)
{
    switch (key) {
    default:
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (key < 6) {
            return 257;
        }
        break;
    case 2:
        cUIMenu_setSelectedByIndex(menu, idx - 1);
    case 3:
        cUIMenu_setSelectedByIndex(menu, idx + 1);
        break;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_getData__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether a multiplayer mode was chosen in the popup.
 *
 * @return 1 after an entry was chosen (widget event 5), 0 after widget event 6 (probably cancel)
 */
signed char cFEPopupSelectMultiplayerMode_getData(void* self)
{
    return *(signed char*)((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the multiplayer-mode selection popup's gain transition at once.
 *
 * The state stack update func_0039ECB0() ends a state's gain transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cFEPopupSelectMultiplayerMode_onGainTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the multiplayer-mode selection popup's lose transition at once.
 *
 * The state stack update func_0039ECB0() ends a state's lose transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cFEPopupSelectMultiplayerMode_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0510);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateAudioOptions_vtbl[];

extern "C" void* func_001E0510(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateAudioOptions_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAudioOptions__cFEStateAudioOptions__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the audio-options front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateAudioOptions__cFEStateAudioOptions(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0570);
#ifdef SKIP_ASM
extern "C" int func_001E0570(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x74)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0578);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateRequestLine_vtbl[];

extern "C" void* func_001E0578(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateRequestLine_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine__cFEStateRequestLine__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the request-line front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRequestLine__cFEStateRequestLine(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E05D8);
#ifdef SKIP_ASM
extern "C" int func_001E05D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x16C)));
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E05E0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E05E0(void* unused, unsigned long bits) {
    int count;
    int i;
    int bit;

    count = 0;
    i = 0;
    do {
        bit = (bits >> i) & 1;
        i += 1;
        count += bit;
    } while (i < 0x40);
    return count;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0610);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001E0610(void* self, int bit)
{
    *(long*)((char*)self + 0x158) |= 1L << bit;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0628);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001E0628(void* self, int bit)
{
    *(long*)((char*)self + 0x158) &= ~(1L << bit);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0648);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E0648(void *arg0) {
    return (*(long *)((char*)(arg0) + (0x158))) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0658);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E0658(void* self, int bit)
{
    return (*(long*)((char*)self + 0x158) & ~(1L << bit)) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0678);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E0678(void* self, int bit)
{
    return (*(unsigned long*)((char*)self + 0x158) >> bit) & 1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0698);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E0698(void *arg0) {
    int temp_2;
    int var_4;
    int var_5;

    var_5 = 0;
    var_4 = 0;
    do {
        temp_2 = ((unsigned long) (*(unsigned long *)((char*)(arg0) + (0x158))) >> var_4) & 1;
        var_4 += 1;
        var_5 += temp_2;
    } while (var_4 < 0x40);
    return var_5;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E06D0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E06D0(void* self, int bit)
{
    return (*(unsigned long*)((char*)self + 0x160) >> bit) & 1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E06F0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_001E06F0(void* self, int bit)
{
    *(long*)((char*)self + 0x160) |= 1L << bit;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0708);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" int func_001E0708(void *arg0) {
    int temp_2;
    int var_4;
    int var_5;

    var_5 = 0;
    var_4 = 0;
    do {
        temp_2 = ((unsigned long) (*(unsigned long *)((char*)(arg0) + (0x160))) >> var_4) & 1;
        var_4 += 1;
        var_5 += temp_2;
    } while (var_4 < 0x40);
    return var_5;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0740);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

extern "C" void func_001E0740(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0770__FPv);
#ifdef SKIP_ASM
void func_001E0770(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0778__FPv);
#ifdef SKIP_ASM
void func_001E0778(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound__cOVSound);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

/**
 * Destroys the overlay sound interface, freeing it when asked.
 *
 * Puts back D_0046DD60, the base sound-interface vtable (On Tour cUISoundInterface; its own destructor func_001E1048
 * stores the same table), and calls operator_delete when bit 0 of flags is set. cOVSound owns nothing to release
 * (On Tour: only mDummyVar at +0x4).
 *
 * @param flags  g++ 2.95 destructor flags; bit 0 = free the object
 */
extern "C" void cOVSound__cOVSound(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound_soundInitialize__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the overlay sound interface needs no set-up.
 *
 * Empty, like On Tour's cOVSound::soundInitialize. Vtable slot 3 of cOVSound_vtbl, after cOVSound_playSound (slot 2, which
 * plays the menu sounds) and before cOVSound_soundShutdown.
 */
void cOVSound_soundInitialize(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound_soundShutdown__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the overlay sound interface has nothing to shut down.
 *
 * Empty, like On Tour's cOVSound::soundShutdown. Vtable slot 4 of cOVSound_vtbl, the last one.
 */
void cOVSound_soundShutdown(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E07C0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateSelectMultiplayerMode_vtbl[];

extern "C" void* func_001E07C0(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateSelectMultiplayerMode_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectMultiplayerMode__cFEStateSelectMultiplayerMode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the select-multiplayer-mode front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateSelectMultiplayerMode__cFEStateSelectMultiplayerMode(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0820);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern void* cFEStateMPCircuit_vtbl[];

extern "C" void* func_001E0820(void* self)
{
    func_0039E2A0(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateMPCircuit_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMPCircuit__cFEStateMPCircuit__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the multiplayer-circuit front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateMPCircuit__cFEStateMPCircuit(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEUseExistingAccount_cFEUseExistingAccount1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEUseExistingAccount_vtbl[];
extern int cBXString_EmptyString;

extern "C" void* cFEUseExistingAccount_cFEUseExistingAccount1(cFEUseExistingAccount* self)
{
    func_001A8500(self, 0, 0);
    int g = cBXString_EmptyString;
    self->mVtbl = cFEUseExistingAccount_vtbl;
    self->mszEmail = (char*)g;
    self->unk_6D0 = 0;
    self->mpTextForgotPassword = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E08D0__FPv);
#ifdef SKIP_ASM
int func_001E08D0(void* self)
{
    return 0;
}
#endif

extern void* cFEStateSelectPersona_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona_cFEStateSelectPersona1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateSelectPersona_cFEStateSelectPersona1(cFEStateSelectPersona* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateSelectPersona_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona__cFEStateSelectPersona__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the persona-selection state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateSelectPersona__cFEStateSelectPersona(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateSelectPersona_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0940__FPv);
#ifdef SKIP_ASM
int func_001E0940(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0948__FPv);
#ifdef SKIP_ASM
void func_001E0948(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup_cFEStateChallengePopup1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateChallengePopup_vtbl[];
extern int D_00441268[];

extern "C" void* cFEStateChallengePopup_cFEStateChallengePopup1(cFEStateChallengePopup* self)
{
    func_001A8500(self, 0, 0);
    self->mVtbl = cFEStateChallengePopup_vtbl;
    self->mpEventOptions = D_00441268;
    self->mbChallengeSent = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup__cFEStateChallengePopup);
#ifdef SKIP_ASM
struct sVEntry001E09A0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* cFEStateChallengePopup_vtbl[];
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys the challenge popup: calls its parent state's slot 32, then runs the cFEStateNet destructor.
 *
 * Slot 32 is the same hook the popup's own onGainTransition calls on itself, so the parent refreshes as the popup
 * goes away.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateChallengePopup__cFEStateChallengePopup(cFEStateChallengePopup* self, int flags)
{
    self->mVtbl = cFEStateChallengePopup_vtbl;
    void* obj = self->mpParentState;
    sVEntry001E09A0* vt = *(sVEntry001E09A0**)((char*)obj + 8);
    vt[32].fn((char*)obj + vt[32].delta);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the challenge popup: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateChallengePopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateChallengeReceivedPopup_vtbl[];
extern int cBXString_EmptyString;

extern "C" void* cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1(cFEStateChallengeReceivedPopup* self)
{
    func_001A8500(self, 0, 0);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEStateChallengeReceivedPopup_vtbl;
    self->mszChallenger = (char*)emptyStr;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup);
#ifdef SKIP_ASM
extern void* cFEStateChallengeReceivedPopup_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");
struct sVEntry001E0A50 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Destroys the challenge-received popup: calls its parent state's slot 32, frees the string at +0x6D0, then runs the
 * cFEStateNet destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup(cFEStateChallengeReceivedPopup* self, int flags)
{
    self->mVtbl = cFEStateChallengeReceivedPopup_vtbl;
    void* obj = self->mpParentState;
    sVEntry001E0A50* vt = *(sVEntry001E0A50**)((char*)obj + 8);
    vt[32].fn((char*)obj + vt[32].delta);
    cBXString__cBXString((char*)self + 0x6D0, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the challenge-received popup: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateChallengeReceivedPopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateOnlineCharacterSelect_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1(cFEStateOnlineCharacterSelect* self) {
    func_001A8500(self, 0, 0);
    (self->mbConfirmed) = 0;
    (self->mVtbl) = cFEStateOnlineCharacterSelect_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the online character-select state: restores its vtable and runs the cFEStateNet destructor
 * (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateOnlineCharacterSelect_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetConfig_cFEStateNetConfig1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateNetConfig_vtbl[];

extern "C" void* cFEStateNetConfig_cFEStateNetConfig1(cFEStateNetConfig* self)
{
    func_001A8500(self, 0, 0);
    self->miSelectedSetting = 0;
    self->mVtbl = cFEStateNetConfig_vtbl;
    self->miNumSettings = 0;
    self->unk_6D8 = 0;
    self->unk_6DC = 0;
    self->mpLBAdapters = 0;
    self->mpLBSettings = 0;
    self->unk_6F0 = 0;
    self->mpMenuConfig = 0;
    self->mpConfigRectangle = 0;
    self->mpNoConfiguration = 0;
    self->unk_700 = 0;
    self->mpustrPopupText = 0;
    self->mpustrAuthError[0] = 0;
    self->mpustrAuthError[1] = 0;
    self->mpustrAuthError[2] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0BA0__FPv);
#ifdef SKIP_ASM
void func_001E0BA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0BA8__FPv);
#ifdef SKIP_ASM
int func_001E0BA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_cFEStateAuthDNAS1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateAuthDNAS_vtbl[];

extern "C" void* cFEStateAuthDNAS_cFEStateAuthDNAS1(cFEStateAuthDNAS* self)
{
    func_001A8500(self, 0, 0);
    self->mVtbl = cFEStateAuthDNAS_vtbl;
    self->mpAuth = 0;
    self->mpTextAuth = 0;
    self->miState = 0;
    self->unk_6F4 = 2;
    self->unk_6F8 = 0x1E;
    self->mpustrText = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Opening transition of the DNAS authentication state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateAuthDNAS_onGainTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the DNAS authentication state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateAuthDNAS_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C20__FPv);
#ifdef SKIP_ASM
void func_001E0C20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C28__FPv);
#ifdef SKIP_ASM
int func_001E0C28(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_cFEStateAuthDNASError1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateAuthDNASError_vtbl[];

extern "C" void* cFEStateAuthDNASError_cFEStateAuthDNASError1(cFEStateAuthDNASError* self)
{
    func_001A8500(self, 0, 0);
    self->mpustrBody = 0;
    self->mVtbl = cFEStateAuthDNASError_vtbl;
    self->mpustrFooter = 0;
    self->mpustrTitle = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the DNAS error state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateAuthDNASError_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C80__FPv);
#ifdef SKIP_ASM
void func_001E0C80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0C88__FPv);
#ifdef SKIP_ASM
int func_001E0C88(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetAccountMem_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the account memory-card popup's gain-focus transition at once.
 *
 * @return always 1 (the transition is done)
 */
int cFEStateNetAccountMem_onGainTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetAccountMem_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the account memory-card popup's lose-focus transition at once.
 *
 * @return always 1 (the transition is done)
 */
int cFEStateNetAccountMem_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0CA0);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateCreateStep1_vtbl[];
extern int cBXString_EmptyString;

extern "C" void* func_001E0CA0(cFEStateCreateStep1* self)
{
    func_001A8500(self, 0, 0);
    int emptyStr = cBXString_EmptyString;
    self->mVtbl = cFEStateCreateStep1_vtbl;
    self->mszSavedPassword = (char*)emptyStr;
    self->mszSavedConfirm = (char*)emptyStr;
    self->mszSavedMail = (char*)emptyStr;
    self->mbEditAccount = 1;
    self->mpStaticContinue = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep1__cFEStateCreateStep1);
#ifdef SKIP_ASM
extern void* cFEStateCreateStep1_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares func_001A85D0 with one arg; the body takes (self, flags)
extern "C" void func_001A85D0_dtor(void* self, int flags) __asm__("func_001A85D0");

/**
 * Destroys account creation step 1: frees its saved mail, confirmation and password strings, then the cFEStateNet
 * part.
 *
 * Restores cFEStateCreateStep1_vtbl, destroys the cBXStrings mszSavedMail (+0x6DC), mszSavedConfirm (+0x6D8) and
 * mszSavedPassword (+0x6D4) and calls func_001A85D0 with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateCreateStep1__cFEStateCreateStep1(cFEStateCreateStep1* self, int flags)
{
    self->mVtbl = cFEStateCreateStep1_vtbl;
    cBXString__cBXString((char*)self + 0x6DC, 2);
    cBXString__cBXString((char*)self + 0x6D8, 2);
    cBXString__cBXString((char*)self + 0x6D4, 2);
    func_001A85D0_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0D60__FPv);
#ifdef SKIP_ASM
int func_001E0D60(void* self)
{
    return 0;
}
#endif

extern void* cFEStateCreateStep2_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0D68);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *func_001E0D68(cFEStateCreateStep2* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateCreateStep2_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep2__cFEStateCreateStep2__FPv);
#ifdef SKIP_ASM
/**
 * Destroys account creation step 2: restores cFEStateCreateStep2_vtbl and runs the cFEStateNet destructor.
 *
 * The C passes only @p self to func_001A85D0; the destructor flags reach it untouched in $a1 (a port must pass
 * them).
 *
 * @return func_001A85D0()'s result (not meaningful; declared void* by the unit)
 */
void* cFEStateCreateStep2__cFEStateCreateStep2(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateCreateStep2_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0DD0__FPv);
#ifdef SKIP_ASM
int func_001E0DD0(void* self)
{
    return 0;
}
#endif

extern void* cFEStateCreateDuplicate_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateDuplicate_cFEStateCreateDuplicate1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);

extern "C" void *cFEStateCreateDuplicate_cFEStateCreateDuplicate1(cFEStateCreateDuplicate* self) {
    func_001A8500(self, 0, 0);
    (self->mVtbl) = cFEStateCreateDuplicate_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateDuplicate__cFEStateCreateDuplicate__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the duplicate-account state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateCreateDuplicate__cFEStateCreateDuplicate(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateCreateDuplicate_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E40__FPv);
#ifdef SKIP_ASM
int func_001E0E40(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLogin_cFEStateLogin1);
#ifdef SKIP_ASM
extern "C" void* func_001A8500(void*, int, int);
extern void* cFEStateLogin_vtbl[];

extern "C" void* cFEStateLogin_cFEStateLogin1(cFEStateLogin* self)
{
    func_001A8500(self, 0, 0);
    self->mpustrDisconnect = 0;
    self->mVtbl = cFEStateLogin_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E88__FPv);
#ifdef SKIP_ASM
void func_001E0E88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0E90__FPv);
#ifdef SKIP_ASM
int func_001E0E90(void* self)
{
    return 0;
}
#endif

extern void* cFEStateStats_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateStats__cFEStateStats__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the stats screen state: restores cFEStateStats_vtbl and runs the cFEStateNet destructor.
 *
 * The C passes only @p self to func_001A85D0; the destructor flags reach it untouched in $a1 (a port must pass
 * them).
 *
 * @return func_001A85D0()'s result (not meaningful; declared void* by the unit)
 */
void* cFEStateStats__cFEStateStats(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateStats_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateStats_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
void cFEStateStats_onMenuGainFocus(void* self)
{
}
#endif

extern void* cFEStateNetNews_vtbl[];
extern "C" void* func_001A85D0(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetNews__cFEStateNetNews__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the server-news state: restores its vtable and runs the cFEStateNet destructor (func_001A85D0).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach func_001A85D0 unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return func_001A85D0()'s result (declared void* here)
 */
void* cFEStateNetNews__cFEStateNetNews(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateNetNews_vtbl;
    return func_001A85D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetNews_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the server-news state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateNetNews_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateRelogin_vtbl[];
extern "C" void* cFEStateLogin__cFEStateLogin(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin_cFEStateRelogin1);
#ifdef SKIP_ASM
extern "C" void* cFEStateLogin_cFEStateLogin(void* self, int);
extern void* cFEStateRelogin_vtbl[];

extern "C" void* cFEStateRelogin_cFEStateRelogin1(void* self)
{
    cFEStateLogin_cFEStateLogin(self, 0);
    *(void***)((char*)self + 0x8) = cFEStateRelogin_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin__cFEStateRelogin__FPv);
#ifdef SKIP_ASM
void* cFEStateRelogin__cFEStateRelogin(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)cFEStateRelogin_vtbl;
    return cFEStateLogin__cFEStateLogin(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin_onLoseTransition__FPv);
#ifdef SKIP_ASM
int cFEStateRelogin_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_cFEStateNetMessenger1);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* func_001A8500(void* self, int a1, int a2);
extern void* D_00467F28[];
extern int cBXString_EmptyString;

struct sAlloc_E0F60 {
    sAlloc_E0F60() {}
    sAlloc_E0F60(const sAlloc_E0F60&) {}
};

// The zeroed triple at 0x6E8 looks like an STL vector; its constructor takes an
// allocator temporary, which is what leaves the unused stack slot.
static inline void InitVec_E0F60(void* self, const sAlloc_E0F60& a = sAlloc_E0F60())
{
    *(int*)((char*)self + 0x6E8) = 0;
    *(int*)((char*)self + 0x6EC) = 0;
    *(int*)((char*)self + 0x6F0) = 0;
}

extern "C" void* cFEStateNetMessenger_cFEStateNetMessenger1(cFEStateNetMessenger* self)
{
    func_001A8500(self, 0, 0);
    int d = cBXString_EmptyString;
    self->mVtbl = D_00467F28;
    self->mszFoundName = (char*)d;
    self->mszSearchName = (char*)d;
    self->unk_6E0 = (char*)d;
    InitVec_E0F60(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_onLoseTransition__FPv);
#ifdef SKIP_ASM
int cFEStateNetMessenger_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_onGainTransition);
#ifdef SKIP_ASM
class cFEStateNetMessenger_onGainTransition_cObj {
public:
    char pad[0x8];
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
};

extern "C" int cFEStateNetMessenger_onGainTransition(cFEStateNetMessenger* self)
{
    ((cFEStateNetMessenger_onGainTransition_cObj*)self)->v32();
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E0FF0);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* func_001A8500(void* self, int a1, int a2);
extern void* cFEStateReadMessagePopup_vtbl[];
extern int cBXString_EmptyString;

struct sAlloc_E0FF0 {
    sAlloc_E0FF0() {}
    sAlloc_E0FF0(const sAlloc_E0FF0&) {}
};

// The zeroed triple at 0x6DC looks like an STL vector; its constructor takes an
// allocator temporary, which is what leaves the unused stack slot.
static inline void InitVec_E0FF0(void* self, const sAlloc_E0FF0& a = sAlloc_E0FF0())
{
    *(int*)((char*)self + 0x6DC) = 0;
    *(int*)((char*)self + 0x6E0) = 0;
    *(int*)((char*)self + 0x6E4) = 0;
}

extern "C" void* func_001E0FF0(void* self)
{
    func_001A8500(self, 0, 0);
    int emptyStr = cBXString_EmptyString;
    *(void***)((char*)self + 0x8) = cFEStateReadMessagePopup_vtbl;
    *(int*)((char*)self + 0x6D0) = emptyStr;
    InitVec_E0FF0(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateReadMessagePopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the read-message popup: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateReadMessagePopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1048);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD60[];

extern "C" void func_001E1048(void* self, int flags)
{
    *(void***)self = D_0046DD60;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1078__FPv);
#ifdef SKIP_ASM
void func_001E1078(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1080__FPv);
#ifdef SKIP_ASM
void func_001E1080(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1088__FPv);
#ifdef SKIP_ASM
void func_001E1088(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1090);
#ifdef SKIP_ASM
struct sFlags_001E1090 {
    unsigned int pad : 27;
    unsigned int flag : 1;
};

extern "C" void func_001E1090(void* self, int enabled)
{
    ((sFlags_001E1090*)self)->flag = (enabled != 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10B8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046DD28[];

extern "C" void func_001E10B8(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_0046DD28;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10E8__FPv);
#ifdef SKIP_ASM
void func_001E10E8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10F0__FPv);
#ifdef SKIP_ASM
int func_001E10F0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E10F8__FPv);
#ifdef SKIP_ASM
int func_001E10F8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1100__FPv);
#ifdef SKIP_ASM
int func_001E1100(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput__cUIInput);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUIInput_vtbl[];

/**
 * Destroys a UI input object: restores cUIInput's vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIInput__cUIInput(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cUIInput_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getState__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's getState: always 0 (On Tour getState(char state); retail's stub reads no state).
 *
 * @return always 0 (not active)
 */
int cUIInput_getState(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isStart__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's start test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isStart(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isSelect__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's select test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isSelect(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNext__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isNext(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPrevious__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isPrevious(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOption__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isOption(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMisc__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isMisc(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNextHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isNextHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPreviousHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isPreviousHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOptionHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isOptionHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMiscHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isMiscHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNextReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next-released test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isNextReleased(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPreviousReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous-released test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isPreviousReleased(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOptionReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option-released test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isOptionReleased(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMiscReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc-released test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isMiscReleased(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isUp__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's up test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isUp(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isDown__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's down test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isDown(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLeft__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's left test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLeft(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRight__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's right test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRight(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isUpHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's up-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isUpHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isDownHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's down-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isDownHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLeftHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's left-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLeftHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRightHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's right-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRightHeld(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift1__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R1 test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRShift1(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift2__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R2 test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRShift2(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift1__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L1 test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLShift1(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift2__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L2 test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLShift2(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R1-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRShift1Held(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R2-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isRShift2Held(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L1-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLShift1Held(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L2-held test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isLShift2Held(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-zoom test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isCamZoom(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-pan test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isCamPan(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-rotate test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isCamRotate(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-zoom amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput_getCamZoom(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-pan amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput_getCamPan(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-rotate amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput_getCamRotate(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_keyPressed__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's key-pressed test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_keyPressed(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getKeyReady__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's key-ready test (a typed key waiting): always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_getKeyReady(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getVKey__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's virtual key code: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_getVKey(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getAsciiKey__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's typed ASCII key: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_getAsciiKey(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_setPadNum__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the controller pad number of a UI input object.
 *
 * @param val  pad number, stored as a byte at offset 4
 */
void cUIInput_setPadNum(void* self, int val)
{
    *(char*)((char*)self + 0x4) = (char)val;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isActive__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's active test: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput_isActive(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_update__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's per-frame update: an empty stub; device classes override it.
 */
void cUIInput_update(void* self)
{
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12B0__FPv);
#ifdef SKIP_ASM
void* func_001E12B0(void* self)
{
    return func_001DBC98(1, 0xffff);
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12D0__FPv);
#ifdef SKIP_ASM
void* func_001E12D0(void* self)
{
    return func_001DBC98(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12F0);
#ifdef SKIP_ASM
#include "fakematch.h"
// sPopupEntry14 (0x14 bytes: +0x4 start, +0x8 count) is defined later in the unit.
struct sPopupEntry14;
extern sPopupEntry14 D_00441630[];

struct sCount_12F0 {
    int f0;
    int start;
    int count;
    int fC;
    int f10;
};

struct sRun_12F0 {
    int start;
    int count;
    int f8;
    int fC;
    int f10;
};

// FAKEMATCH: statement order (each run of equal counts is written with its first entry last, and the
// zero and one entries of the tail interleaved, so sched1 reproduces retail's store order) plus five
// FM_OPAQUE(n) after n = 1 (delays the ones by five cycles so d[52] issues after d[46]); plain form 99.82%
// (natural index order: 99.02%).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void func_001E12F0(void)
{
    sCount_12F0* d = (sCount_12F0*)D_00441630;
    int i;
    int n;

    n = 6;
    d[2].count = n;
    d[3].count = n;
    d[4].count = n;
    d[5].count = n;
    d[6].count = n;
    d[1].count = n;
    d[0].count = 0;
    n = 11;
    d[7].count = n;
    n = 17;
    d[8].count = n;
    n = 7;
    d[9].count = n;
    n = 6;
    d[10].count = n;
    n = 8;
    d[11].count = n;
    n = 3;
    d[13].count = n;
    d[14].count = n;
    d[15].count = n;
    d[16].count = n;
    d[17].count = n;
    d[18].count = n;
    d[19].count = n;
    d[20].count = n;
    d[21].count = n;
    d[12].count = n;
    n = 4;
    d[23].count = n;
    d[24].count = n;
    d[25].count = n;
    d[26].count = n;
    d[27].count = n;
    d[22].count = n;
    n = 16;
    d[28].count = n;
    d[29].count = n;
    d[30].count = n;
    d[31].count = n;
    d[32].count = n;
    d[33].count = n;
    d[40].count = 0;
    n = 2;
    d[35].count = n;
    d[36].count = n;
    d[37].count = n;
    d[38].count = n;
    d[39].count = n;
    d[34].count = n;
    d[41].count = 0;
    d[42].count = 0;
    d[43].count = 0;
    d[44].count = 0;
    d[45].count = 0;
    d[46].count = 0;
    d[47].count = 0;
    n = 1;
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    d[53].count = n;
    d[48].count = 0;
    d[54].count = n;
    d[49].count = 0;
    d[55].count = n;
    d[50].count = 0;
    d[56].count = n;
    d[51].count = 0;
    d[57].count = n;
    d[0].start = 0;
    d[58].count = n;
    d[59].count = n;
    d[60].count = n;
    d[52].count = n;
    for (i = 1; i <= 60; i++) {
        sRun_12F0* e = (sRun_12F0*)&d->start;
        e[i].start = e[i - 1].start + e[i - 1].count;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1450);
#ifdef SKIP_ASM
extern "C" void func_001E1450(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E1458);
#ifdef SKIP_ASM
extern int D_004A20C0;
extern int D_004A20C8;
extern int D_004A20BC;
extern void* cAppMan_TheAppMan;
extern "C" void func_001E12F0(void);
extern "C" void func_001E14C0(int type);

extern "C" void func_001E1458(void)
{
    if (D_004A20C0 == 0) {
        if (D_004A20C8 == 0) {
            D_004A20C8 = 1;
            func_001E12F0();
        }
        int t = *(int*)((char*)cAppMan_TheAppMan + 0x1C);
        if (t - D_004A20BC > 600) {
            func_001E14C0(1);
            D_004A20BC = t;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E14C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);
void func_001E1548(void* self);

struct sEntry14_14C0 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_14C0 D_00441630_14C0[] __asm__("D_00441630");

extern "C" void func_001E14C0(int type)
{
    int i;
    for (i = 0; i < 61; i++) {
        if (func_001E3A30((void*)i) == 0) {
            int t = D_00441630_14C0[i].type;
            if (t == type || t == 5) {
                func_001E1548((void*)i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1548__FPv);
#ifdef SKIP_ASM
void func_001E1548(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1550);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E2FE0(int a, unsigned char b);
extern "C" void func_001E3A78(int a, int b);
extern "C" int func_001464D0(void* iface, int a, int ch, int b, int c);
extern "C" int func_00145EF0(void* iface, int a, int b, int c);
unsigned int BXrand();

extern "C" void func_001E1550(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        if (func_001E3A30((void*)1) == 0) {
            if (func_001464D0(iface, 0, ch, 0, 1) == 0 && func_00145EF0(iface, 0, 0xE, 4) == 0) {
                func_001E3A78(1, 1);
                if (ch != 3) {
                    func_001E2FE0(BXrand() % 3, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 3, 0);
                }
            }
        } else if (func_001E3A30((void*)2) == 0) {
            if (func_001464D0(iface, 0, ch, 1, 1) == 0 && func_00145EF0(iface, 0, 0xF, 4) == 0) {
                func_001E3A78(2, 1);
                if (ch != 7) {
                    func_001E2FE0(BXrand() % 3 + 6, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 9, 0);
                }
            }
        } else if (func_001E3A30((void*)3) == 0) {
            if (func_001464D0(iface, 0, ch, 2, 1) == 0 && func_00145EF0(iface, 0, 0x10, 4) == 0) {
                func_001E3A78(3, 1);
                if (ch != 8) {
                    func_001E2FE0(BXrand() % 3 + 0xC, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 0xF, 0);
                }
            }
        }
        if (func_001E3A30((void*)4) == 0) {
            if (func_001464D0(iface, 0, ch, 0, 0) == 0 && func_00145EF0(iface, 0, 0xE, 5) == 0) {
                func_001E3A78(4, 1);
                if (ch != 3) {
                    func_001E2FE0(BXrand() % 3 + 18, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 21, 0);
                }
            }
        } else if (func_001E3A30((void*)5) == 0) {
            if (func_001464D0(iface, 0, ch, 1, 0) == 0 && func_00145EF0(iface, 0, 0xF, 5) == 0) {
                func_001E3A78(5, 1);
                if (ch != 7) {
                    func_001E2FE0(BXrand() % 3 + 0x18, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 27, 0);
                }
            }
        } else if (func_001E3A30((void*)6) == 0) {
            if (func_001464D0(iface, 0, ch, 2, 0) == 0 && func_00145EF0(iface, 0, 0x10, 5) == 0) {
                func_001E3A78(6, 1);
                if (ch != 8) {
                    func_001E2FE0(BXrand() % 3 + 0x1E, 0);
                } else {
                    func_001E2FE0(BXrand() % 3 + 33, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1A30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_001E1A30(int x, int y)
{
    switch (x) {
    case 0: return y == 0;
    case 1: return y == 1;
    case 2: return y == 2;
    case 3: return y == 3;
    case 4: return y == 4;
    case 5: return y == 5;
    case 6: return y == 6;
    case 7: return y == 7;
    case 8: return y == 8;
    case 9: return y == 9;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E1AD0);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
unsigned int BXrand();
extern "C" int func_001E1A30(int kind, int arg);
extern "C" int func_001E3388(int id);
extern char str_AddForeshadowMessage[];

struct sPopupEntry18_1AD0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};
extern sPopupEntry18_1AD0 D_004C6C08_1AD0[] __asm__("D_004C6C08");

struct sRangeK1E1AD0 {
    int f0;
    int start;   // 0x4
    int count;   // 0x8
};

extern "C" int func_001E1AD0(sRangeK1E1AD0* r, int arg)
{
    int* list = (int*)operator_new_tag(r->count * 4, str_AddForeshadowMessage, 0x100, 0);
    int n = 0;
    int i;
    for (i = r->start; i < r->count + r->start; i++) {
        if (func_001E1A30(D_004C6C08_1AD0[i].fC, arg) == 0 && func_001E3388(i) == -1) {
            list[n++] = i;
        }
    }
    if (n == 0)
        return 0x100;
    int id = list[BXrand() % n];
    if (list != 0)
        cMemMan_free(list);
    return id;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1C10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E2FE0(int a, unsigned char b);

struct sEntry14_1C10 {
    int type;
    int f4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_1C10 D_00441630_1C10[] __asm__("D_00441630");



extern "C" void func_001E1C10(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        int id;
        switch (which) {
        case 0: {
            if (func_001E3A30((void*)0x1C) != 0) {
                break;
            }
            if (func_001E3A30((void*)29) != 0) {
                break;
            }
            sEntry14_1C10* r;
            if (ch != 3) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[0x1C];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[29];
            }
            id = func_001E1AD0((sRangeK1E1AD0*)r, ch);
            if (id != 0x100) {
                func_001E2FE0(id, 0);
            }
            break;
        }
        case 1: {
            if (func_001E3A30((void*)0x1E) != 0) {
                break;
            }
            if (func_001E3A30((void*)0x1F) != 0) {
                break;
            }
            sEntry14_1C10* r;
            if (ch != 7) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[0x1E];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[0x1F];
            }
            id = func_001E1AD0((sRangeK1E1AD0*)r, ch);
            if (id != 0x100) {
                func_001E2FE0(id, 0);
            }
            break;
        }
        case 2: {
            if (func_001E3A30((void*)0x20) != 0) {
                break;
            }
            if (func_001E3A30((void*)33) != 0) {
                break;
            }
            sEntry14_1C10* r;
            if (ch != 8) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[0x20];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                r = &tbl[33];
            }
            id = func_001E1AD0((sRangeK1E1AD0*)r, ch);
            if (id != 0x100) {
                func_001E2FE0(id, 0);
            }
            break;
        }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1DD0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int id, int a1);
static inline int IsBusy001E1DD0()
{
    int n = *(int*)SSXApp->Game->Replay;
    return n != 0 && n < 10;
}
// Queues the follow-up popup for an event kind unless one is already pending.
extern "C" void func_001E1DD0(void* self, int kind, int alt)
{
    if (IsBusy001E1DD0()) return;
    int id = 0;
    switch (kind) {
    case 6: id = func_001E3A30((void*)34) == 0 ? 40 : 0; break;
    case 7: id = func_001E3A30((void*)36) == 0 ? 44 : 0; break;
    case 8: id = func_001E3A30((void*)38) == 0 ? 0x30 : 0; break;
    case 9: id = func_001E3A30((void*)35) == 0 ? 42 : 0; break;
    case 10: id = func_001E3A30((void*)37) == 0 ? 46 : 0; break;
    case 11: id = func_001E3A30((void*)39) == 0 ? 50 : 0; break;
    }
    if (id != 0) {
        if (alt != 0) id++;
        func_001E3A78(id, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1EB8);
#ifdef SKIP_ASM
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E20D0(int which, int flag);
extern "C" void func_001E2220(int which, int flag);

extern "C" void func_001E1EB8(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        if (func_001E3A30((void*)40) != 0) {
            func_001E2220(0, 0);
            func_001E3A78(40, 0);
        } else if (func_001E3A30((void*)41) != 0) {
            func_001E2220(0, 1);
            func_001E3A78(41, 0);
        }
        if (func_001E3A30((void*)44) != 0) {
            func_001E2220(1, 0);
            func_001E3A78(44, 0);
        } else if (func_001E3A30((void*)45) != 0) {
            func_001E2220(1, 1);
            func_001E3A78(45, 0);
        }
        if (func_001E3A30((void*)0x30) != 0) {
            func_001E2220(2, 0);
            func_001E3A78(0x30, 0);
        } else if (func_001E3A30((void*)49) != 0) {
            func_001E2220(2, 1);
            func_001E3A78(49, 0);
        }
        if (func_001E3A30((void*)42) != 0) {
            func_001E20D0(0, 0);
            func_001E3A78(42, 0);
        } else if (func_001E3A30((void*)43) != 0) {
            func_001E20D0(0, 1);
            func_001E3A78(43, 0);
        }
        if (func_001E3A30((void*)46) != 0) {
            func_001E20D0(1, 0);
            func_001E3A78(46, 0);
        } else if (func_001E3A30((void*)47) != 0) {
            func_001E20D0(1, 1);
            func_001E3A78(47, 0);
        }
        if (func_001E3A30((void*)50) != 0) {
            func_001E20D0(2, 0);
            func_001E3A78(50, 0);
        } else if (func_001E3A30((void*)51) != 0) {
            func_001E20D0(2, 1);
            func_001E3A78(51, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E20D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

struct sEntry14_20D0 {
    int type;
    int f4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_20D0 D_00441630_20D0[] __asm__("D_00441630");

extern "C" void func_001E20D0(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (func_001E3A30((void*)35) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    func_001E3A78(35, 1);
                    func_001E2FE0(tbl[35].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[35].f4, 0);
                }
            }
            break;
        case 1:
            if (func_001E3A30((void*)37) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    func_001E3A78(37, 1);
                    func_001E2FE0(tbl[37].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[37].f4, 0);
                }
            }
            break;
        case 2:
            if (func_001E3A30((void*)39) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    func_001E3A78(39, 1);
                    func_001E2FE0(tbl[39].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[39].f4, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2220);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

struct sEntry14_2220 {
    int type;
    int f4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_2220 D_00441630_2220[] __asm__("D_00441630");

extern "C" void func_001E2220(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (func_001E3A30((void*)34) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    func_001E3A78(34, 1);
                    func_001E2FE0(tbl[34].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[34].f4, 0);
                }
            }
            break;
        case 1:
            if (func_001E3A30((void*)36) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    func_001E3A78(36, 1);
                    func_001E2FE0(tbl[36].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[36].f4, 0);
                }
            }
            break;
        case 2:
            if (func_001E3A30((void*)38) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    func_001E3A78(38, 1);
                    func_001E2FE0(tbl[38].f4 + 1, 0);
                } else {
                    func_001E2FE0(tbl[38].f4, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2370);
#ifdef SKIP_ASM
extern "C" void func_001E3A78(int a, int b);

extern "C" void func_001E2370(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            func_001E3A78(0x1C, 1);
            func_001E3A78(29, 1);
            break;
        case 1:
            func_001E3A78(0x1E, 1);
            func_001E3A78(0x1F, 1);
            break;
        case 2:
            func_001E3A78(0x20, 1);
            func_001E3A78(33, 1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2428);
#ifdef SKIP_ASM
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2428(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (func_001E3A30((void*)57) == 0) {
                func_001E3A78(57, 1);
                func_001E2FE0(0xFC, 0);
            }
            break;
        case 1:
            if (func_001E3A30((void*)58) == 0) {
                func_001E3A78(57, 1);
                func_001E2FE0(253, 0);
            }
            break;
        case 2:
            if (func_001E3A30((void*)59) == 0) {
                func_001E3A78(59, 1);
                func_001E2FE0(0xFE, 0);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2518);
#ifdef SKIP_ASM
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2518(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0 && func_001E3A30((void*)0x3C) == 0) {
        func_001E3A78(0x3C, 1);
        func_001E2FE0(0xFF, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2578);
#ifdef SKIP_ASM
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E2578(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0 && func_001E3A30((void*)0x38) == 0) {
        func_001E3A78(0x38, 1);
        func_001E2FE0(251, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E25D8);
#ifdef SKIP_ASM
extern "C" void func_001E2648(void* a, int b);
extern "C" void func_001E2828(void* a, int b);

extern "C" void func_001E25D8(int key, void* a1, int a2)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (key) {
        case 4:
            func_001E2648(a1, a2);
            break;
        case 5:
            func_001E2828(a1, a2);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2648);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);
extern "C" void func_001E32C8(int type);

// PORT: the unit declares func_001E2648(void*, int); the first arg is really an int id.
extern "C" void func_001E2648_impl(int which, int flag) __asm__("func_001E2648");
extern "C" void func_001E2648_impl(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        switch (which) {
        case 0xE:
            if (func_001E3A30((void*)22) != 0) {
                break;
            }
            func_001E32C8(1);
            func_001E32C8(22);
            if (flag == 0) {
                func_001E3A78(22, 1);
                if (ch != 3) {
                    func_001E2FE0(116, 0);
                } else {
                    func_001E2FE0(118, 0);
                }
            } else {
                if (ch != 3) {
                    func_001E2FE0(115, 0);
                } else {
                    func_001E2FE0(117, 0);
                }
            }
            break;
        case 0xF:
            if (func_001E3A30((void*)23) != 0) {
                break;
            }
            func_001E32C8(2);
            func_001E32C8(23);
            if (flag == 0) {
                func_001E3A78(23, 1);
                if (ch != 7) {
                    func_001E2FE0(0x78, 0);
                } else {
                    func_001E2FE0(122, 0);
                }
            } else {
                if (ch != 7) {
                    func_001E2FE0(119, 0);
                } else {
                    func_001E2FE0(121, 0);
                }
            }
            break;
        case 0x10:
            if (func_001E3A30((void*)0x18) != 0) {
                break;
            }
            func_001E32C8(3);
            func_001E32C8(0x18);
            if (flag == 0) {
                func_001E3A78(0x18, 1);
                if (ch != 8) {
                    func_001E2FE0(0x7C, 0);
                } else {
                    func_001E2FE0(0x7E, 0);
                }
            } else {
                if (ch != 8) {
                    func_001E2FE0(123, 0);
                } else {
                    func_001E2FE0(125, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2828);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);
extern "C" void func_001E32C8(int type);

// PORT: the unit declares func_001E2828(void*, int); the first arg is really an int id.
extern "C" void func_001E2828_impl(int which, int flag) __asm__("func_001E2828");
extern "C" void func_001E2828_impl(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        switch (which) {
        case 0xE:
            if (func_001E3A30((void*)25) != 0) {
                break;
            }
            func_001E32C8(4);
            func_001E32C8(25);
            if (flag == 0) {
                func_001E3A78(25, 1);
                if (ch != 3) {
                    func_001E2FE0(0x80, 0);
                } else {
                    func_001E2FE0(130, 0);
                }
            } else {
                if (ch != 3) {
                    func_001E2FE0(0x7F, 0);
                } else {
                    func_001E2FE0(129, 0);
                }
            }
            break;
        case 0xF:
            if (func_001E3A30((void*)26) != 0) {
                break;
            }
            func_001E32C8(5);
            func_001E32C8(26);
            if (flag == 0) {
                func_001E3A78(26, 1);
                if (ch != 7) {
                    func_001E2FE0(132, 0);
                } else {
                    func_001E2FE0(134, 0);
                }
            } else {
                if (ch != 7) {
                    func_001E2FE0(131, 0);
                } else {
                    func_001E2FE0(133, 0);
                }
            }
            break;
        case 0x10:
            if (func_001E3A30((void*)27) != 0) {
                break;
            }
            func_001E32C8(6);
            func_001E32C8(27);
            if (flag == 0) {
                func_001E3A78(27, 1);
                if (ch != 8) {
                    func_001E2FE0(136, 0);
                } else {
                    func_001E2FE0(138, 0);
                }
            } else {
                if (ch != 8) {
                    func_001E2FE0(135, 0);
                } else {
                    func_001E2FE0(137, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2A08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int func_001E2B38(int charID);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);
unsigned int BXrand();

struct sEntry14_2A08 {
    int type;
    int start;   // 0x4
    int count;   // 0x8
    char pad_0xC[0x14 - 0xC];
};
extern sEntry14_2A08 D_00441630_2A08[] __asm__("D_00441630");

extern "C" void func_001E2A08(int charID, int level)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy)
        return;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0)) == charID)
        return;
    int id = func_001E2B38(charID);
    sEntry14_2A08* e = &D_00441630_2A08[id];
    if (func_001E3A30((void*)id)) {
        if (level < 5)
            func_001E3A78(id, 0);
    } else if ((int)((BXrand() & 7) + 0xF) < level) {
        func_001E2FE0(e->start + BXrand() % e->count, 0);
        func_001E3A78(id, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2B38);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_001E2B38(int x)
{
    switch (x) {
    case 0: return 0xC;
    case 1: return 13;
    case 2: return 0xE;
    case 3: return 0xF;
    case 4: return 0x10;
    case 5: return 17;
    case 6: return 18;
    case 7: return 19;
    case 8: return 20;
    case 9: return 21;
    case 10:
    default:
        return 61;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2BB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);

struct sEntry14_2BB8 {
    int type;
    int f4;
    int count;
    int fC;
    int f10;
};
extern sEntry14_2BB8 D_00441630_2BB8[] __asm__("D_00441630");

extern "C" int func_001E2BB8(int type, int all)
{
    int total = 0;
    int i;
    for (i = 0; i < 61; i++) {
        sEntry14_2BB8* e = &D_00441630_2BB8[i];
        if (e->type == type) {
            if (all != 0 || (e->f10 != 0 && func_001E3A30((void*)i) != 0)) {
                total += e->count;
            } else {
                total += 1;
            }
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sEntry14_2C60 {
    int type;
    int start;   // 0x4
    int count;   // 0x8
    int fC;
    int f10;
};
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

extern "C" int func_001E2C60(int i)
{
    return D_00441630_2C60[i].count;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2C80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001E3A30(void* self);

struct sEntry14_2C80 {
    int type;
    int first;
    int count;
    int fC;
    int f10;
};
extern sEntry14_2C80 D_00441630_2C80[] __asm__("D_00441630");

extern "C" int func_001E2C80(int type, int idx, int mode)
{
    int base = 0;
    int result = -1;
    int i;
    for (i = 0; i < 61; i++) {
        sEntry14_2C80* e = &D_00441630_2C80[i];
        if (e->type == type) {
            int n;
            if (mode == 0 || e->f10 == 0 || func_001E3A30((void*)i) != 0) {
                n = e->count;
            } else {
                n = 1;
            }
            n += base;
            if (idx < n) {
                result = e->first + (idx - base);
                goto done;
            }
            base = n;
        }
    }
done:
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2D58);
#ifdef SKIP_ASM
struct sPopupEntry18 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18 D_004C6C08[];

struct sPopupEntry14 {
    char pad_0x0[0xC];
    int field_0xC;
    int field_0x10;
};
extern sPopupEntry14 D_00441630[];

extern "C" int func_001E2D58(int v, int i)
{
    sPopupEntry18* a = &D_004C6C08[i];
    sPopupEntry14* b = &D_00441630[a->field_0x4];
    return b->field_0xC == v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2D98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

extern "C" int func_001E2D98(int v, int i)
{
    int* p = (int*)&D_00441630_2C60[0] + i * 5;
    return p[3] == v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2DC0);
#ifdef SKIP_ASM
extern "C" int func_001E2DC0(int idx)
{
    sPopupEntry18* ta = &D_004C6C08[idx];
    sPopupEntry14* target = &D_00441630[ta->field_0x4];
    int r = 0;
    int i = 0;
    while (i <= idx) {
        sPopupEntry18* a = &D_004C6C08[i];
        sPopupEntry14* e = &D_00441630[a->field_0x4];
        int start = *(int*)((char*)e + 4);
        int count = *(int*)((char*)e + 8);
        if (start + count - 1 < idx) {
            if (e->field_0xC == target->field_0xC) r += count;
            i += count;
        } else {
            if (e->field_0xC == target->field_0xC) r += idx - start + 1;
            break;
        }
    }
    return r - 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2E80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

extern "C" int func_001E2E80(int i, int n)
{
    return D_00441630_2C60[i].start + n;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2EA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern char str_kT_CHARMoby[];
extern char str_kT_CHARKaori[];
extern char str_kT_CHARAllegra[];
extern char str_kT_CHARMac[];
extern char str_kT_CHARZoe[];
extern char str_kT_CHARGriff[];
extern char str_kT_CHARElise[];
extern char str_kT_CHARNate[];
extern char str_kT_CHARPsymon[];
extern char str_kT_CHARViggo[];
extern char str_kT_CMNGameTitle[];
extern char str_kT_CMNDJAtomica[];
extern char str_kT_FAQFolder[];

// Returns the string hash of the title for popup message kind (0-12), or 0.
extern "C" int func_001E2EA0(int kind)
{
    switch ((unsigned int)kind) {
    case 0:
        return GetHashValue32(str_kT_CHARMoby);
    case 1:
        return GetHashValue32(str_kT_CHARKaori);
    case 2:
        return GetHashValue32(str_kT_CHARAllegra);
    case 3:
        return GetHashValue32(str_kT_CHARMac);
    case 4:
        return GetHashValue32(str_kT_CHARZoe);
    case 5:
        return GetHashValue32(str_kT_CHARGriff);
    case 6:
        return GetHashValue32(str_kT_CHARElise);
    case 7:
        return GetHashValue32(str_kT_CHARNate);
    case 8:
        return GetHashValue32(str_kT_CHARPsymon);
    case 9:
        return GetHashValue32(str_kT_CHARViggo);
    case 10:
        return GetHashValue32(str_kT_CMNGameTitle);
    case 11:
        return GetHashValue32(str_kT_CMNDJAtomica);
    case 12:
        return GetHashValue32(str_kT_FAQFolder);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E2FE0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_hud_b[];
extern "C" void func_001E31B8(int idx);
extern "C" unsigned char func_001E3100(int i);
extern "C" void* cUIStateStack_findState(void* list, int hash);

struct sPopupListEntry_2FE0 {
    int id;
    unsigned char flag;
    char pad_0x5[3];
};
struct sPopupList_2FE0 {
    sPopupListEntry_2FE0 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_2FE0* func_001E39F8_list2FE0() __asm__("func_001E39F8");

struct sPopupEntry18_2FE0 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_2FE0 D_004C6C08_2FE0[] __asm__("D_004C6C08");

struct sVEntryK1E2FE0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" void func_001E2FE0(int id, unsigned char flag)
{
    sPopupList_2FE0* t = func_001E39F8_list2FE0();
    if (t->count == 25)
        func_001E31B8(0);
    if (D_004C6C08_2FE0[id].field_0x8 == 1)
        flag = func_001E3100(id);
    t->entries[t->count].id = id;
    t->entries[t->count].flag = flag;
    t->mask &= ~(1 << t->count);
    char* o = (char*)SSXApp->Game->UI;
    if (o != 0) {
        char* obj = (char*)cUIStateStack_findState(o + 0x18, GetHashValue32(str_hud_b));
        if (obj != 0) {
            sVEntryK1E2FE0* vt = *(sVEntryK1E2FE0**)(obj + 8);
            vt[24].fn(obj + vt[24].delta, 8, 0);
        }
    }
    t->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E3100);
#ifdef SKIP_ASM
extern "C" int func_001E2D58(int v, int i);
unsigned int BXrand();

extern "C" unsigned char func_001E3100(int i)
{
    unsigned char pick = 0xFF;
    if (func_001E2D58(3, i)) {
        pick = BXrand() % 10;
    } else if (func_001E2D58(5, i)) {
        pick = BXrand() % 11;
    } else if (func_001E2D58(8, i)) {
        pick = BXrand() % 4;
    } else if (func_001E2D58(4, i)) {
        pick = BXrand() % 10;
    }
    return pick;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E31B8);
#ifdef SKIP_ASM
struct sPopupListEntry_31B8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_31B8 {
    sPopupListEntry_31B8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_31B8* func_001E39F8_list31B8() __asm__("func_001E39F8");

extern "C" void func_001E31B8(int idx)
{
    sPopupList_31B8* msgList = func_001E39F8_list31B8();
    int i;
    for (i = idx; i < msgList->count - 1; i++) {
        msgList->entries[i] = msgList->entries[i + 1];
        int bit = 1 << (i + 1);
        if (msgList->mask & bit) {
            msgList->mask |= 1 << i;
        } else {
            msgList->mask &= ~(1 << i);
        }
    }
    msgList->count--;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3268);
#ifdef SKIP_ASM
struct sPopupListEntry {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList {
    sPopupListEntry entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");
extern "C" void func_001E31B8(int idx);

extern "C" void func_001E3268(int id)
{
    sPopupList* msgList = func_001E39F8_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        if (id == msgList->entries[i].id) {
            func_001E31B8(i);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E32C8);
#ifdef SKIP_ASM
struct sPopupEntry18_32C8 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_32C8 D_004C6C08_32C8[] __asm__("D_004C6C08");
struct sPopupListEntry_32C8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_32C8 {
    sPopupListEntry_32C8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList_32C8* func_001E39F8_list32C8() __asm__("func_001E39F8");
extern "C" void func_001E31B8(int idx);

extern "C" void func_001E32C8(int type)
{
    int n = 0;
    int j = 0;
    sPopupList_32C8* t = func_001E39F8_list32C8();
    for (; n < t->count; n++) {
        sPopupEntry18_32C8* a = &D_004C6C08_32C8[t->entries[j].id];
        if (type == a->field_0x4) {
            func_001E31B8(j);
        } else {
            j++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3388);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");

extern "C" int func_001E3388(int id)
{
    sPopupList* msgList = func_001E39F8_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        if (id == msgList->entries[i].id) {
            return i;
        }
    }
    return -1;
}
#endif

extern "C" void* func_001E39F8(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E33E8__FPv);
#ifdef SKIP_ASM
unsigned char func_001E33E8(void* self)
{
    return *(unsigned char*)((char*)func_001E39F8(self) + 0xcc);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3408);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");

extern "C" int func_001E3408()
{
    sPopupList* msgList = func_001E39F8_list();
    int n = 0;
    int i;
    for (i = 0; i < msgList->count; i++) {
        int bit = 1 << i;
        if (!(msgList->mask & bit)) {
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3460);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupList* func_001E39F8_list() __asm__("func_001E39F8");
extern "C" int func_001E3620(int idx);

extern "C" int func_001E3460(int* out)
{
    sPopupList* msgList = func_001E39F8_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        int bit = 1 << i;
        if (!(msgList->mask & bit)) {
            *out = i;
            return func_001E3620(i);
        }
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E34D8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int func_001E34D8(int i)
{
    sPopupEntry18* a = &D_004C6C08[i];
    int n = i + 1;
    sPopupEntry18* b = &D_004C6C08[n];
    if (b->field_0x4 == a->field_0x4)
        return n;
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3510);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001475C0(void* iface, int a1, int a2);
extern int D_004A20CC;
extern signed char D_00535C11[];

extern "C" void func_001E3510(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 0) {
        D_004A20CC = 0;
        func_001475C0(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3570);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147580(void* iface, int a1);

extern "C" int func_001E3570(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath != 0) {
        return 0;
    }
    if (cBENewRaceInterface_newRace.cGameEvent == 6 || cBENewRaceInterface_newRace.cGameEvent == 9) {
        return 0;
    }
    return func_00147580(cBE_getInterface_Fv(cBE_getBE(), 1), 0) == 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E35F0);
#ifdef SKIP_ASM
extern "C" void* func_001E39F8(void* self);

extern "C" int func_001E35F0(void* self)
{
    return (int)func_001E39F8(self) + ((int)self << 3);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3620);
#ifdef SKIP_ASM
struct sPopupEntry8 {
    int field_0x0;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupTable8 {
    sPopupEntry8 entries[1];
};
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupTable8* func_001E39F8_get() __asm__("func_001E39F8");

extern "C" int func_001E3620(int idx)
{
    return func_001E39F8_get()->entries[idx].field_0x0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3650);
#ifdef SKIP_ASM
// PORT: unit declares func_001E39F8(void*), but its body takes no arguments
extern "C" sPopupTable8* func_001E39F8_get() __asm__("func_001E39F8");

extern "C" unsigned char func_001E3650(int idx)
{
    return func_001E39F8_get()->entries[idx].field_0x4;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3680);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern int D_004A20C4;

extern "C" unsigned char func_001E3680(int id)
{
    if (D_004A20C4 != 0) {
        return id;
    }
    int n1 = func_001E2BB8(4, 0);
    int n2 = func_001E33E8_noarg();
    int i;
    for (i = 0; i < n1 + n2; i++) {
        int k = i + 1;
        int idx = n2 - k;
        if (idx >= 0) {
            if (func_001E3620(idx) == id) {
                return i;
            }
        } else if (idx + n1 >= 0) {
            if (func_001E2C80(4, ~idx, 1) == id) {
                return i;
            }
        }
    }
    return 0xFF;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3760);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
int cBERewardInterface_getEarningsTarget(void* self, int a1);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E3760(int rider, int amt)
{
    if (rider == 0) {
        void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
        void* rew = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
        if (cBENewPlayerInterface_isPeakLocked1(pl, 0, 1)) {
            if ((unsigned)(cBEEconInterface_getTotalMoneyEarned1(econ, 0) + amt) >= (unsigned)cBERewardInterface_getEarningsTarget(rew, 0) && func_001E3A30((void*)52) == 0) {
                func_001E3A78(52, 1);
                func_001E2FE0(247, 0);
            }
        }
        if (cBENewPlayerInterface_isPeakLocked1(pl, rider, 2)) {
            if ((unsigned)(cBEEconInterface_getTotalMoneyEarned1(econ, rider) + amt) >= (unsigned)cBERewardInterface_getEarningsTarget(rew, 1) && func_001E3A30((void*)53) == 0) {
                func_001E3A78(53, 1);
                func_001E2FE0(0xF8, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E38B8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E2FE0(int a, unsigned char b);

extern "C" void func_001E38B8(int player)
{
    if (player == 0) {
        void* stats = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
        int charID = cBENewPlayerInterface_getRiderCharID(np, cBENewPlayerInterface_getHumanRider(np, 0));
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 1)) {
            if (cBERewardInterface_isTrophyOwned(stats, player, charID, 0, 2) && func_001E3A30((void*)54) == 0) {
                func_001E3A78(54, 1);
                func_001E2FE0(249, 0);
            }
        }
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 2)) {
            if (cBERewardInterface_isTrophyOwned(stats, player, charID, 1, 2) && func_001E3A30((void*)55) == 0) {
                func_001E3A78(55, 1);
                func_001E2FE0(250, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E39F8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_00147908(void* iface, int a1);

extern "C" void* func_001E39F8(void* self)
{
    return func_00147908(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3A30);
#ifdef SKIP_ASM
extern "C" int func_00147980(void* iface, int a1, void* a2);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_001E3A30(void* self)
{
    return func_00147980(cBE_getInterface_Fv(cBE_getBE(), 1), 0, self) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3A78);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00147A30(void* iface, int a1, int a2, int a3);

extern "C" void func_001E3A78(int a, int b)
{
    func_00147A30(cBE_getInterface_Fv(cBE_getBE(), 1), 0, a, b);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3AC8);
#ifdef SKIP_ASM
extern char* PadCache;

class cSnd_3AC8 {
public:
    virtual void v01();
    virtual void v02();
    virtual int getState();
    virtual void setVolume(int ch, float v);
};

// PORT: uses g++'s >? (max) operator.
extern "C" void func_001E3AC8(int idx, float x, float y)
{
    signed char i = idx;
    float adjusted = x * 0.9133333563804626f - 18.518518447875977f;
    float clamped = 0.0f;
    if (adjusted >= 0.0f) {
        clamped = adjusted;
    }
    y = y * 0.9916666746139526f;
    cSnd_3AC8* s = 0;
    if (PadCache != 0) {
        s = *(cSnd_3AC8**)(PadCache + (i << 2) + 0x2EEC);
    }
    if (s->getState() == 2) {
        s->setVolume(0, ((y - 0.5f) * 0.009999999776482582f >? (clamped - 2.0f) / 972.2222290039062f) >? 0.0f);
        s->setVolume(1, ((y - 70.0f) * 0.0062500000931322575f >? (clamped - 100.0f) / 5000.0f) >? 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_cOVState_MESSAGECENTER);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" int cUITextureBank_getElementByHashname(void* self, int hash);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E3A78(int a, int b);
int GetHashValue32(char* str);
extern void* D_00474E08[];
extern void* D_00474318[];
extern unsigned short D_004A2088[];
extern char str_folder[];
extern char str_minus_folder[];
extern char str_questmark_b[];
extern int D_004A20CC;

struct sObj_3C00 {
    char pad_0x0[0x9C];
    int a[8];   // 0x9C
    int b[8];   // 0xBC
    int c[8];   // 0xDC
    int d[8];   // 0xFC
    int e[8];   // 0x11C
    char pad_0x13C[0x164 - 0x13C];
    int h164;   // 0x164
    int h168;   // 0x168
    int h16C;   // 0x16C
};

/**
 * Constructs the in-game message center overlay: clears its widget tables and looks up its folder icons.
 *
 * Runs the cUIState constructor (func_0039E2A0), then the inlined cOVState part (mOVID = @p id, vtable D_00474E08,
 * mDoneTransition, mAllowPauseMenu and mRemovePDA 0, chat buffer " "), stores D_00474318, zeroes the eight-row
 * widget tables (unread mail, FAQ icon, from, subject, number) and the labels, menu, buttons and arrows (not
 * mpNoMessage), and looks up the "folder", "minus folder" and "questmark" textures in the engine's texture bank
 * (+0x58, cUITextureBank_getElementByHashname). miMiscData becomes the opening mode: 2 when D_004A20CC is -1, else 0 or 1 by whether
 * new-player hint 7 is set (func_001E3A30), which it then sets (func_001E3A78(7, 1)).
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param id  the overlay's id (On Tour cOVState ctor tOVStateEnum id; the overlay factory passes 0x22)
 * @return @p self
 */
extern "C" void* cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(cOVState_MESSAGECENTER* self, void* engine, int id)
{
    sObj_3C00* stateView = (sObj_3C00*)self;
    func_0039E2A0(self, engine);
    self->mOVID = id;
    self->mVtbl = D_00474E08;
    self->mDoneTransition = 0;
    self->mAllowPauseMenu = 0;
    self->mRemovePDA = 0;
    *(unsigned short*)((char*)self + 0x58) = D_004A2088[0];
    self->mVtbl = D_00474318;
    int i;
    for (i = 0; i < 8; i++) {
        stateView->a[i] = 0;
        stateView->b[i] = 0;
        stateView->c[i] = 0;
        stateView->d[i] = 0;
        stateView->e[i] = 0;
    }
    self->mpTotalMessagesLabel = 0;
    self->mpUnreadMessagesLabel = 0;
    self->mpMenu = 0;
    self->mpButtonSquare = 0;
    self->mpButtonText3 = 0;
    self->mpButtonCross = 0;
    self->mpButtonText1 = 0;
    self->mpArrowUp = 0;
    self->mpArrowDown = 0;
    {
        char* eng = (char*)self->mpEngine;
        int nameHash = GetHashValue32(str_folder);
        stateView->h164 = cUITextureBank_getElementByHashname(eng + 0x58, nameHash);
    }
    {
        char* eng = (char*)self->mpEngine;
        int nameHash = GetHashValue32(str_minus_folder);
        stateView->h168 = cUITextureBank_getElementByHashname(eng + 0x58, nameHash);
    }
    {
        char* eng = (char*)self->mpEngine;
        int nameHash = GetHashValue32(str_questmark_b);
        stateView->h16C = cUITextureBank_getElementByHashname(eng + 0x58, nameHash);
    }
    if (~D_004A20CC != 0) {
        if (func_001E3A30((void*)7) != 0)
            self->miMiscData = 0;
        else
            self->miMiscData = 1;
        func_001E3A78(7, 1);
    } else {
        self->miMiscData = 2;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3D58);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void func_001E48B0(cOVState_MESSAGECENTER*);
extern "C" void func_001E4758(cOVState_MESSAGECENTER*);
extern "C" void func_001E4458(cOVState_MESSAGECENTER*);
extern "C" void func_001E4658(cOVState_MESSAGECENTER*);
extern char str_112messagecenter[];

extern "C" void func_001E3D58(cOVState_MESSAGECENTER* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_112messagecenter), 0);
    self->mpMessageScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    int mode = self->miMiscData;
    if (mode == 0) {
        goto add;
    }
    if (mode == 1) {
    add:
        cOVStateManager_addPDATemplate();
    }
    self->mScrollOffset = 0;
    int n = func_001E33E8_noarg();
    n += func_001E2BB8(4, 1);
    cUIMenu_setScrollableMenu(self->mpMenu, (unsigned char)n);
    func_001E48B0(self);
    func_001E4758(self);
    func_001E4458(self);
    func_001E4658(self);
    D_004A20C0 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3E30);
#ifdef SKIP_ASM
extern "C" void func_001E3A78(int a, int b);
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void cOVState_onDestroyScreen(void* self);
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_001E3E30(cOVState_MESSAGECENTER* self)
{
    int s = self->miMiscData;
    if (s == 1) {
        func_001E3A78(7, 0);
        cOVStateManager_removePDATemplate();
        cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
    } else if (s == 0) {
        func_001E3A78(7, 1);
        cOVStateManager_removePDATemplate();
        cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
    }
    cOVState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E3EC0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_004A20A8;
extern char* D_004A2098;
extern char str_btext3_b[];
extern char str_btext1[];
extern char str_TotalMessagesLabel[];
extern char str_UnreadMessagesLabel[];
extern char str_MessageView[];
extern char str_MenuFroms[];
extern char str_arr_down_b[];
extern char str_arr_up_b[];
extern char str_NoMessage[];
extern char str_UnreadMailpct_d[];
extern char str_FAQIconpct_d[];
extern char str_Frompct_d[];
extern char str_Subjectpct_d[];
extern char str_Numberpct_d[];

struct sVEntry_1E3EC0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sPopup_1E3EC0 {
    char pad0[0x9C];
    void* a9C[8];       // 0x9C
    void* aBC[8];       // 0xBC
    void* aDC[8];       // 0xDC
    void* aFC[8];       // 0xFC
    void* a11C[8];      // 0x11C
    void* f13C;         // 0x13C
    void* f140;         // 0x140
    void* f144;         // 0x144
    void* f148;         // 0x148
    void* f14C;         // 0x14C
    void* f150;         // 0x150
    void* f154;         // 0x154
    void* f158;         // 0x158
    void* f15C;         // 0x15C
    void* f160;         // 0x160
};

extern "C" void func_001E3EC0(sPopup_1E3EC0* self, void* w)
{
    char buf[32];
    int h = *(int*)((char*)w + 0x38);
    if (h == GetHashValue32(D_004A20A8))
        self->f14C = w;
    else if (h == GetHashValue32(str_btext3_b))
        self->f150 = w;
    if (h == GetHashValue32(D_004A2098)) {
        self->f154 = w;
        return;
    }
    if (h == GetHashValue32(str_btext1)) {
        self->f158 = w;
        return;
    }
    if (h == GetHashValue32(str_TotalMessagesLabel)) {
        self->f13C = w;
        return;
    }
    if (h == GetHashValue32(str_UnreadMessagesLabel)) {
        self->f140 = w;
        return;
    }
    if (h == GetHashValue32(str_MessageView)) {
        sVEntry_1E3EC0* vt = *(sVEntry_1E3EC0**)((char*)w + 8);
        vt[9].fn((char*)w + vt[9].delta, 0);
        return;
    }
    if (h == GetHashValue32(str_MenuFroms)) {
        self->f144 = w;
        return;
    }
    if (h == GetHashValue32(str_arr_down_b)) {
        self->f160 = w;
        return;
    }
    if (h == GetHashValue32(str_arr_up_b)) {
        self->f15C = w;
        return;
    }
    if (h == GetHashValue32(str_NoMessage)) {
        self->f148 = w;
        sVEntry_1E3EC0* vt = *(sVEntry_1E3EC0**)((char*)w + 8);
        vt[9].fn((char*)w + vt[9].delta, 0);
        return;
    }
    for (int i = 0; i < 8; i++) {
        sprintf(buf, str_UnreadMailpct_d, i);
        if (h == GetHashValue32(buf)) {
            self->a9C[i] = w;
            break;
        }
        sprintf(buf, str_FAQIconpct_d, i);
        if (h == GetHashValue32(buf)) {
            self->aBC[i] = w;
            return;
        }
        sprintf(buf, str_Frompct_d, i);
        if (h == GetHashValue32(buf)) {
            self->aDC[i] = w;
            return;
        }
        sprintf(buf, str_Subjectpct_d, i);
        if (h == GetHashValue32(buf)) {
            self->aFC[i] = w;
            sVEntry_1E3EC0* vt = *(sVEntry_1E3EC0**)((char*)w + 8);
            vt[27].fn((char*)w + vt[27].delta, 1);
            return;
        }
        sprintf(buf, str_Numberpct_d, i);
        if (h == GetHashValue32(buf)) {
            self->a11C[i] = w;
            return;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E4168);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" unsigned char func_001E3680(int id);
extern "C" void func_001E50C8(cOVState_MESSAGECENTER* self, int a1, int a2);

struct sVEntry001E4168 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4168(cOVState_MESSAGECENTER* self)
{
    int state = self->miMiscData;
    if (state == 0) {
        goto reset;
    }
    if (state == 1) {
    reset:
        cUIMenu_setSelectedByIndex(self->mpMenu, func_001E3680(37));
        void* menu = self->mpMenu;
        sVEntry001E4168* vt = *(sVEntry001E4168**)((char*)menu + 8);
        vt[7].fn((char*)menu + vt[7].delta, 1);
        func_001E50C8(self, 37, -1);
    } else {
        void* menu = self->mpMenu;
        sVEntry001E4168* vt = *(sVEntry001E4168**)((char*)menu + 8);
        vt[7].fn((char*)menu + vt[7].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4200);
#ifdef SKIP_ASM
struct sPopupEntry18_4200 {
    int field_0x0;
    int id;                         // 0x4
    char pad_0x8[0x10];
};
extern sPopupEntry18_4200 D_004C6C08_4200[] __asm__("D_004C6C08");

struct sPopupEntry14_4200 {
    int type;
    int start;                      // 0x4
    int count;                      // 0x8
    int field_0xC;
    int enabled;                    // 0x10
};
extern sPopupEntry14_4200 D_00441630_4200[] __asm__("D_00441630");

struct sVEntry001E4200 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern char str_TransitionOut_b[];
extern int D_004A20C0;

// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
// PORT: func_001E3A30 is declared int; its result is used as a bool here
extern "C" bool func_001E3A30_b(int id) __asm__("func_001E3A30");
extern "C" int func_001E3620(int idx);
extern "C" void func_001E3A78(int a, int b);
extern "C" void func_001E48B0(cOVState_MESSAGECENTER*);
extern "C" void func_001E4458(cOVState_MESSAGECENTER*);
extern "C" void func_001E4658(cOVState_MESSAGECENTER*);
extern "C" void func_001E50C8(cOVState_MESSAGECENTER* self, int a1, int a2);
extern "C" void func_001E5148(cOVState_MESSAGECENTER* self, int idx);

extern "C" void func_001E4200(cOVState_MESSAGECENTER* self, void* widget, unsigned int event)
{
    if (widget == 0) {
        return;
    }
    sPopupList* list = func_001E39F8_list();
    int n1 = func_001E2BB8(4, 1);
    int n2 = func_001E33E8_noarg();
    switch (event) {
    case 5:
        if (n1 + n2 > 0) {
            int k = *(unsigned char*)((char*)self->mpMenu + 0x95);
            int idx = n2 - k - 1;
            if (idx >= 0) {
                int id = func_001E3620(idx);
                list->mask |= 1 << idx;
                func_001E50C8(self, id, k + 1);
            } else if (idx + n1 >= 0) {
                int w = *(int*)((char*)widget + 0x18);
                sPopupEntry18_4200* a = &D_004C6C08_4200[w];
                sPopupEntry14_4200* e = &D_00441630_4200[a->id];
                if (e->enabled != 0 && w == e->start) {
                    func_001E3A78(a->id, !func_001E3A30_b(a->id));
                    func_001E48B0(self);
                    func_001E4658(self);
                    func_001E4458(self);
                } else {
                    func_001E50C8(self, w, k + 1);
                }
            }
        }
        break;
    case 7:
        self->mbMessageDeleted = 0;
        if (n2 > 0) {
            int idx = n2 - *(unsigned char*)((char*)self->mpMenu + 0x95) - 1;
            if (idx >= 0) {
                func_001E5148(self, idx);
                self->mbMessageDeleted = 1;
            }
            func_001E4458(self);
        }
        break;
    case 6: {
        int f = cUIScreen_getFrameByLabel(self->mpMessageScreen, GetHashValue32(str_TransitionOut_b));
        if (f != 0xFFFF) {
            cUIScreen_playFrame(self->mpMessageScreen, f, 1);
        }
        func_0039F190((char*)self->mpEngine + 0x18, 1);
        D_004A20C0 = 0;
        break;
    }
    case 1: {
        int i;
        for (i = 0; i < 8; i++) {
            void* o = ((void**)((char*)self + 0xFC))[i];
            sVEntry001E4200* vt = *(sVEntry001E4200**)((char*)o + 8);
            vt[5].fn((char*)o + vt[5].delta, 0);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4458);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" int func_001E2C80(int type, int idx, int mode);
extern "C" int func_001E3A30(void* self);
extern "C" void func_001E4840(cOVState_MESSAGECENTER* self, void* obj);
extern "C" void func_001E4878(cOVState_MESSAGECENTER* self, void* obj);
extern char str_kT_BTNView[];
extern char str_kT_BTNCollapse[];
extern char str_kT_BTNExpand[];

struct sEntry18_4458 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sEntry18_4458 D_004C6C08_4458[] __asm__("D_004C6C08");
struct sEntry14_4458 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_4458 D_00441630_4458[] __asm__("D_00441630");

extern "C" void func_001E4458(cOVState_MESSAGECENTER* self)
{
    int base = *(unsigned char*)((char*)self->mpMenu + 0x95);
    int diff = func_001E33E8_noarg() - base;
    if (diff - 1 >= 0) {
        func_001E4878(self, self->mpButtonSquare);
        func_001E4878(self, self->mpButtonText3);
        if (self->mpButtonText1 != 0)
            cUIText_setUnicodeStringByID(self->mpButtonText1, GetHashValue32(str_kT_BTNView));
    } else {
        func_001E4840(self, self->mpButtonSquare);
        func_001E4840(self, self->mpButtonText3);
        if (self->mpButtonText1 != 0) {
            int id = func_001E2C80(4, -diff, 1);
            sEntry18_4458* a = &D_004C6C08_4458[id];
            int t = a->field_0x4;
            sEntry14_4458* b = &D_00441630_4458[t];
            if (id == b->field_0x4) {
                if (func_001E3A30((void*)t))
                    cUIText_setUnicodeStringByID(self->mpButtonText1, GetHashValue32(str_kT_BTNCollapse));
                else
                    cUIText_setUnicodeStringByID(self->mpButtonText1, GetHashValue32(str_kT_BTNExpand));
            } else {
                cUIText_setUnicodeStringByID(self->mpButtonText1, GetHashValue32(str_kT_BTNView));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4578);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void func_001E4658(cOVState_MESSAGECENTER* self);
extern "C" void func_001E48B0(cOVState_MESSAGECENTER* self);
extern "C" void func_001E4458(cOVState_MESSAGECENTER* self);

extern "C" int func_001E4578(cOVState_MESSAGECENTER* self, void* widget, int event, int value)
{
    switch (event) {
    case 4:
        self->mScrollOffset = value;
        func_001E4658(self);
        func_001E48B0(self);
        break;
    case 5:
        func_001E4458(self);
        return 257;
    case 9:
        return 0;
    case 8:
        return self->mbMessageDeleted != 0;
    case 6:
    case 7:
        return 1;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4610);
#ifdef SKIP_ASM
extern "C" void func_001E48B0(cOVState_MESSAGECENTER*);
extern "C" void func_001E4758(cOVState_MESSAGECENTER*);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001E4610(cOVState_MESSAGECENTER* self, int popup, int event)
{
    if (event == 20) {
        func_001E48B0(self);
        func_001E4758(self);
        func_0039F190((char*)self->mpEngine + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4658);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" int func_001E2BB8(int type, int all);
extern int D_004A20C4;

struct sVEntryK1E4658 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4658(cOVState_MESSAGECENTER* self)
{
    if (self->mScrollOffset == 0) {
        char* o = (char*)self->mpArrowUp;
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 0);
    } else {
        char* o = (char*)self->mpArrowUp;
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 1);
    }
    int n;
    if (D_004A20C4 != 0) {
        n = func_001E33E8_noarg();
        n += func_001E2BB8(4, 1);
    } else {
        n = func_001E33E8_noarg();
        n += func_001E2BB8(4, 0);
    }
    if (self->mScrollOffset + 8 >= n) {
        char* o = (char*)self->mpArrowDown;
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 0);
    } else {
        char* o = (char*)self->mpArrowDown;
        sVEntryK1E4658* vt = *(sVEntryK1E4658**)(o + 8);
        vt[9].fn(o + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4758);
#ifdef SKIP_ASM
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNTotalMsg[];
extern char str_kT_OVRCMNUnread[];

struct sVEntry001E4758 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern "C" void func_001E4758(cOVState_MESSAGECENTER* self)
{
    unsigned short buf[0x38];
    if (self->mpTotalMessagesLabel != 0) {
        char* locale = (char*)SSXApp->FELocale;
        sVEntry001E4758* vt = *(sVEntry001E4758**)(locale + 4);
        unsigned short* fmt = vt[4].fn(locale + vt[4].delta, GetHashValue32(str_kT_OVRCMNTotalMsg));
        sprintf1(buf, fmt, func_001E33E8_noarg());
        cUIText_setUnicodeString(self->mpTotalMessagesLabel, buf);
    }
    if (self->mpUnreadMessagesLabel != 0) {
        char* locale = (char*)SSXApp->FELocale;
        sVEntry001E4758* vt = *(sVEntry001E4758**)(locale + 4);
        unsigned short* fmt = vt[4].fn(locale + vt[4].delta, GetHashValue32(str_kT_OVRCMNUnread));
        sprintf1(buf, fmt, func_001E3408());
        cUIText_setUnicodeString(self->mpUnreadMessagesLabel, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4840);
#ifdef SKIP_ASM
struct sVEntry_func_001E4840 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4840(cOVState_MESSAGECENTER* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001E4840* vt = *(sVEntry_func_001E4840**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4878);
#ifdef SKIP_ASM
struct sVEntry_func_001E4878 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E4878(cOVState_MESSAGECENTER* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001E4878* vt = *(sVEntry_func_001E4878**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E48B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_showObjSafe_48B0(void* self, char* name) __asm__("cUIState_showObjSafe");
extern "C" void cUIState_hideObjSafe_48B0(void* self, char* name) __asm__("cUIState_hideObjSafe");
extern "C" int func_001E2BB8_48B0(int kind, int online) __asm__("func_001E2BB8");
extern "C" int func_001E33E8_48B0() __asm__("func_001E33E8__FPv");
extern "C" void func_001E4840_48B0(void* self, int obj) __asm__("func_001E4840");
extern "C" void func_001E4878_48B0(void* self, int obj) __asm__("func_001E4878");
extern "C" int func_001E3620_48B0(int idx) __asm__("func_001E3620");
extern "C" int func_001E3650_48B0(int idx) __asm__("func_001E3650");
extern "C" void func_001E4C78_48B0(void* self, int idx, int row, int a3, int a4) __asm__("func_001E4C78");
extern "C" int func_001E2C80_48B0(int kind, int idx, int local) __asm__("func_001E2C80");
extern "C" int func_001E4B80_48B0(void* self, int entry) __asm__("func_001E4B80");
extern "C" void cUIMenu_setScrollableMenu_48B0(void* menu, unsigned char n) __asm__("cUIMenu_setScrollableMenu");
extern int D_004A20C4;
extern char str_MessageNumbers[];
extern char str_MessageFroms[];
extern char str_MessageSubjects[];
extern char str_UnreadMailBitmaps[];
extern char str_MessageHighlights[];
extern char str_NoMessages[];

struct sList_48B0 {
    char pad0[0x9C];
    int a[8];       // 0x9C
    int b[8];       // 0xBC
    int c[8];       // 0xDC
    int d[8];       // 0xFC
    int e[8];       // 0x11C
    char pad13C[0x144 - 0x13C];
    char* menu;     // 0x144
    char pad148[0x14C - 0x148];
    int f14C;       // 0x14C
    int f150;       // 0x150
    int f154;       // 0x154
    int f158;       // 0x158
    char pad15C[0x174 - 0x15C];
    int scroll;     // 0x174
};

// Rebuilds the eight rows of the friends/buddies list from the local and online entries.
extern "C" void func_001E48B0(cOVState_MESSAGECENTER* selfp)
{
    sList_48B0* self = (sList_48B0*)selfp;
    int online;
    if (D_004A20C4) {
        online = func_001E2BB8_48B0(4, 1);
    } else {
        online = func_001E2BB8_48B0(4, 0);
    }
    int local = func_001E33E8_48B0();
    int total = local + online;
    int sel;
    if (total == 0) {
        sel = 0;
        cUIState_hideObjSafe_48B0(self, str_MessageNumbers);
        cUIState_hideObjSafe_48B0(self, str_MessageFroms);
        cUIState_hideObjSafe_48B0(self, str_MessageSubjects);
        cUIState_hideObjSafe_48B0(self, str_UnreadMailBitmaps);
        cUIState_hideObjSafe_48B0(self, str_MessageHighlights);
        cUIState_showObjSafe_48B0(self, str_NoMessages);
        func_001E4840_48B0(self, self->f154);
        func_001E4840_48B0(self, self->f158);
        func_001E4840_48B0(self, self->f14C);
        func_001E4840_48B0(self, self->f150);
    } else {
        cUIState_showObjSafe_48B0(self, str_MessageNumbers);
        sel = total;
        cUIState_showObjSafe_48B0(self, str_MessageFroms);
        cUIState_showObjSafe_48B0(self, str_MessageSubjects);
        cUIState_showObjSafe_48B0(self, str_UnreadMailBitmaps);
        cUIState_hideObjSafe_48B0(self, str_NoMessages);
        func_001E4878_48B0(self, self->f154);
        func_001E4878_48B0(self, self->f158);
        for (int i = 0; i < 8; i++) {
            int n = i + 1;
            int k = local - self->scroll - n;
            if (k >= 0) {
                int r1 = func_001E3620_48B0(k);
                func_001E4C78_48B0(self, k, i, r1, func_001E3650_48B0(k));
            } else if (k + online >= 0) {
                int nk = ~k;
                int r;
                if (D_004A20C4) {
                    r = func_001E2C80_48B0(4, nk, 0);
                } else {
                    r = func_001E2C80_48B0(4, nk, 1);
                }
                func_001E4C78_48B0(self, -1, i, r, (unsigned char)func_001E4B80_48B0(self, r));
            } else {
                func_001E4840_48B0(self, self->a[i]);
                func_001E4840_48B0(self, self->b[i]);
                func_001E4840_48B0(self, self->c[i]);
                func_001E4840_48B0(self, self->e[i]);
                func_001E4840_48B0(self, self->d[i]);
            }
        }
    }
    unsigned char keep = self->menu[0x95];
    cUIMenu_setScrollableMenu_48B0(self->menu, sel);
    self->menu[0x95] = keep;
    unsigned char s = self->scroll;
    self->menu[0x98] = s;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A20C4;
extern "C" int func_001E2DC0(int idx);

struct sPopupEntry18_4B80 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_4B80 D_004C6C08_4B80[] __asm__("D_004C6C08");

extern "C" unsigned char func_001E4B80(cOVState_MESSAGECENTER* self, int idx)
{
    unsigned char r = 0xFF;
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    sPopupEntry18_4B80* a = &D_004C6C08_4B80[idx];
    if (a->field_0x8 == 1) {
        int v = func_001E2DC0(idx);
        int div = 1;
        sPopupEntry14* b = &D_00441630[a->field_0x4];
        switch (b->field_0xC) {
        case 3:
            div = 10;
            v = v / 2;
            break;
        case 5:
            div = 11;
            break;
        case 4:
            div = 10;
            break;
        case 8:
            div = 4;
            break;
        }
        r = v % div;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E4C78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int func_001E2EA0(int id);
extern "C" void func_001E4F70(cOVState_MESSAGECENTER* self, int slot, int id, unsigned char sub);
extern char D_004A2100[];
extern char D_004A2108[];
extern char D_004A2110[];
extern char str_pct_d_d[];

struct sEnt18_4C78 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};
extern sEnt18_4C78 D_004C6C08_4C78[] __asm__("D_004C6C08");

struct sEnt14_4C78 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
};
extern sEnt14_4C78 D_00441630_4C78[] __asm__("D_00441630");

struct sVEntry_4C78 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj_4C78 {
    int f0;
    int f4;
    sVEntry_4C78* vt;   // 0x08
    char padC[0xC];
    int f18;            // 0x18
    char pad1C[0x5C];
    int f78;            // 0x78
    int f7C;            // 0x7C
};

struct sPopup_4C78 {
    char pad0[0x9C];
    sObj_4C78* a9C[8];  // 0x9C
    sObj_4C78* aBC[8];  // 0xBC
    sObj_4C78* aDC[8];  // 0xDC
    sObj_4C78* aFC[8];  // 0xFC
    sObj_4C78* a11C[8]; // 0x11C
    char pad13C[0x28];
    int f164;           // 0x164
    int f168;           // 0x168
    int f16C;           // 0x16C
    char pad170[4];
    int f174;           // 0x174
};

static inline void setTint_4C78(sObj_4C78* o, int v, int idx)
{
    o->f78 = idx;
    o->f7C = v;
}

static inline void show_4C78(sObj_4C78* o)
{
    o->vt[9].fn((char*)o + o->vt[9].delta, 1);
}

extern "C" void func_001E4C78(cOVState_MESSAGECENTER* self_, int idx, int slot, int id, unsigned char sub)
{
    sPopup_4C78* self = (sPopup_4C78*)self_;
    char buf[16];
    sEnt18_4C78* a = &D_004C6C08_4C78[id];
    sEnt14_4C78* b = &D_00441630_4C78[a->f4];
    char* p = (char*)func_001E39F8(self);
    if (idx == -1) {
        if (b->f10 != 0) {
            if (id == b->f4) {
                if (func_001E3A30((void*)a->f4) == 0)
                    setTint_4C78(self->aBC[slot], self->f164, idx);
                else
                    setTint_4C78(self->aBC[slot], self->f168, idx);
            } else {
                setTint_4C78(self->aBC[slot], self->f16C, idx);
            }
        }
        func_001E4840((cOVState_MESSAGECENTER*)self, self->a9C[slot]);
        func_001E4878((cOVState_MESSAGECENTER*)self, self->aBC[slot]);
    } else {
        int mask = 1 << idx;
        if (*(int*)(p + 0xC8) & mask) {
            func_001E4840((cOVState_MESSAGECENTER*)self, self->a9C[slot]);
            func_001E4840((cOVState_MESSAGECENTER*)self, self->aBC[slot]);
        } else {
            func_001E4878((cOVState_MESSAGECENTER*)self, self->a9C[slot]);
            func_001E4840((cOVState_MESSAGECENTER*)self, self->aBC[slot]);
        }
    }
    if (self->a11C[slot] != 0) {
        if (b->f10 != 0) {
            if (id == b->f4) {
                if (func_001E3A30((void*)a->f4) == 0)
                    sprintf(buf, D_004A2100);
                else
                    sprintf(buf, D_004A2108);
            } else {
                sprintf(buf, D_004A2110);
            }
        } else {
            sprintf(buf, str_pct_d_d, self->f174 + slot + 1);
        }
        cUIText_setAsciiString((cUIText*)self->a11C[slot], buf);
        show_4C78(self->a11C[slot]);
    }
    if (self->aDC[slot] != 0) {
        cUIText_setUnicodeStringByID((cUIText*)self->aDC[slot], func_001E2EA0(a->fC));
        show_4C78(self->aDC[slot]);
        self->aDC[slot]->f18 = id;
    }
    if (a->f8 == 1) {
        func_001E4F70((cOVState_MESSAGECENTER*)self, slot, id, sub);
    } else if (self->aFC[slot] != 0) {
        cUIText_setUnicodeStringByID((cUIText*)self->aFC[slot], a->f10);
        show_4C78(self->aFC[slot]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E4F70);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" unsigned char func_001E5AA0(cOVState_ViewPopup* self, int idx);
extern "C" int func_001E3388(int id);
extern "C" unsigned char func_001E3650(int idx);
extern "C" int func_001E2D58(int v, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A20C4;
extern char str_kT_MSGSubjectBCFinishLosspct_d[];
extern char str_kT_MSGSubjectBCFinishWinpct_d[];
extern char str_kT_MSGSubjectAggressionpct_d[];
extern char str_kT_MSGSubjectForeshadowpct_d[];
extern char str_kT_MSGSubjectBeatThePeakpct_d[];

struct sPopup1E4F70 {
    char pad0[0xFC];
    cUIText* texts[4];  // 0xFC
};

struct sVE1E4F70 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// Fills text slot `slot` with the "level N" string for the reward id and shows it.
extern "C" void func_001E4F70(cOVState_MESSAGECENTER* self, int slot, int id, unsigned char lvl)
{
    char buf[48];
    if (func_001E2D58(3, id)) {
        switch (id) {
        case 115: case 117: case 119: case 121: case 123: case 125: case 0x7F: case 129: case 131: case 133: case 135: case 137:
            sprintf(buf, str_kT_MSGSubjectBCFinishLosspct_d, lvl);
            break;
        case 116: case 118: case 0x78: case 122: case 0x7C: case 0x7E: case 0x80: case 130: case 132: case 134: case 136: case 138:
            sprintf(buf, str_kT_MSGSubjectBCFinishWinpct_d, lvl);
            break;
        }
    } else if (func_001E2D58(5, id)) {
        sprintf(buf, str_kT_MSGSubjectAggressionpct_d, lvl);
    } else if (func_001E2D58(4, id)) {
        sprintf(buf, str_kT_MSGSubjectForeshadowpct_d, lvl);
    } else if (func_001E2D58(8, id)) {
        sprintf(buf, str_kT_MSGSubjectBeatThePeakpct_d, lvl);
    }
    sPopup1E4F70* view = (sPopup1E4F70*)self;
    if (view->texts[slot] != 0) {
        cUIText_setUnicodeStringByID(view->texts[slot], GetHashValue32(buf));
        char* textObj = (char*)view->texts[slot];
        sVE1E4F70* vt = *(sVE1E4F70**)(textObj + 8);
        vt[9].fn(textObj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E50C8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cOVState_ViewPopup_cOVState_ViewPopup(cOVState_ViewPopup* self, void* a1, void* owner, int a3, int a4);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cOVState_ViewPopup[];

extern "C" void func_001E50C8(cOVState_MESSAGECENTER* self, int a1, int a2)
{
    void* item = cOVState_ViewPopup_cOVState_ViewPopup((cOVState_ViewPopup*)cMemMan_alloc(0x70, str_cOVState_ViewPopup, 0, 0), self->mpEngine, self, a1, a2);
    void* screen = self->mpMessageScreen;
    void* b = *(void**)((char*)screen + 0xD0);
    void* c = *(void**)((char*)b + 0x10);
    cUIStateStack_pushPopup((char*)c + 0x18, item);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E5148);
#ifdef SKIP_ASM
extern "C" void func_001E31B8(int idx);
extern "C" int func_001E2BB8(int a0, int a1);
// PORT: func_001E33E8 is declared (void*) but never reads its argument; called here with none
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");
extern "C" void func_001E48B0(cOVState_MESSAGECENTER*);
extern "C" void func_001E4758(cOVState_MESSAGECENTER*);
extern "C" void func_001E4658(cOVState_MESSAGECENTER*);

extern "C" void func_001E5148(cOVState_MESSAGECENTER* self, int idx)
{
    func_001E31B8(idx);
    int cur = *(unsigned char*)((char*)self->mpMenu + 0x95);
    int n = func_001E2BB8(4, 1);
    n += func_001E33E8_noarg();
    if (cur >= n) {
        cur--;
    }
    *(unsigned char*)((char*)self->mpMenu + 0x95) = cur;
    func_001E48B0(self);
    func_001E4758(self);
    func_001E4658(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_cOVState_ViewPopup);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern void* cOVState_ViewPopup_vtbl[];

/**
 * Constructs the popup that shows one saved message (screen "113ViewMessage").
 *
 * Runs the cUIState constructor (func_0039E318), stores the message index and number, cOVState_ViewPopup_vtbl, and
 * zeroes every widget pointer.
 *
 * @param engine  the UI engine (the message center's mpEngine)
 * @param owner  the parent state (the cOVState_MESSAGECENTER)
 * @param message  the message's index in the saved list D_004C6C08 (miMessage)
 * @param number  the number shown for it (miNumber)
 * @return @p self
 */
extern "C" void* cOVState_ViewPopup_cOVState_ViewPopup(cOVState_ViewPopup* self, void* engine, void* owner, int message, int number)
{
    func_0039E318(self, engine, owner);
    self->miMessage = message;
    self->miNumber = number;
    self->mVtbl = cOVState_ViewPopup_vtbl;
    self->mpNumberLabel = 0;
    self->mpNumber = 0;
    self->mpFrom = 0;
    self->mpSubject = 0;
    self->mpButtonSquare = 0;
    self->mpDeleteMessage = 0;
    self->mpKeepMessage = 0;
    self->mpMessageLine0 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_113ViewMessage[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Opens the message view popup's "113ViewMessage" screen into mpScreen and plays its frame 0.
 */
extern "C" void cOVState_ViewPopup_onCreateScreen(cOVState_ViewPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_113ViewMessage), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

extern "C" void* cUIState_onUpdate(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of the message view popup: only the cUIState update (cUIState_onUpdate).
 *
 * @return cUIState_onUpdate()'s result (not meaningful; declared void* by the unit)
 */
void* cOVState_ViewPopup_onUpdate(void* self)
{
    return cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onGainTransition);
#ifdef SKIP_ASM
extern "C" void func_001E5428(cOVState_ViewPopup* self);

/**
 * Fills the message view the first time it gains focus (func_001E5428: number, sender, subject, body and buttons).
 *
 * @param on  nonzero on the first transition (On Tour bFirstTime)
 * @return always 0
 */
extern "C" int cOVState_ViewPopup_onGainTransition(cOVState_ViewPopup* self, int on)
{
    if (on != 0) {
        func_001E5428(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_From[];
extern char str_Subject[];
extern char str_NumberLabel[];
extern char str_Number[];
extern char str_MV_KeepMessage[];
extern char* D_004A20A8;
extern char str_MV_DeleteMessage[];
extern char str_MessageLine0[];
extern char str_MenuDummy[];

struct sVEntryK1E52F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows each of the message view's widgets as it is created and binds the ones it fills.
 *
 * Every widget is shown (slot 9). "From", "Subject", "NumberLabel", "Number", "MV_KeepMessage", the square button
 * (D_004A20A8), "MV_DeleteMessage" and "MessageLine0" are stored in their members; "MenuDummy" gets bit 3 of its
 * +0x90 word.
 *
 * @param widget  the widget being created (On Tour pWidget)
 */
extern "C" void cOVState_ViewPopup_onWidgetCreate(cOVState_ViewPopup* self, void* widget)
{
    int nameHash = *(int*)((char*)widget + 0x38);
    sVEntryK1E52F8* vt = *(sVEntryK1E52F8**)((char*)widget + 8);
    vt[9].fn((char*)widget + vt[9].delta, 1);
    if (nameHash == GetHashValue32(str_From)) {
        self->mpFrom = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_Subject)) {
        self->mpSubject = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_NumberLabel)) {
        self->mpNumberLabel = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_Number)) {
        self->mpNumber = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_MV_KeepMessage)) {
        self->mpKeepMessage = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(D_004A20A8)) {
        self->mpButtonSquare = (cUIThing*)widget;
    } else if (nameHash == GetHashValue32(str_MV_DeleteMessage)) {
        self->mpDeleteMessage = (cUIThing*)widget;
    } else if (nameHash == GetHashValue32(str_MessageLine0)) {
        self->mpMessageLine0 = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_MenuDummy)) {
        *(int*)((char*)widget + 0x90) |= 8;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", func_001E5428);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPopupEntry18_5428 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x4];
    int field_0xC;
    char pad_0x10[0x8];
};
extern sPopupEntry18_5428 D_004C6C08_5428[] __asm__("D_004C6C08");

struct sPopupEntry14_5428 {
    int type;
    char pad_0x4[0x10];
};
extern sPopupEntry14_5428 D_00441630_5428[] __asm__("D_00441630");

struct sVEntryK1E5428 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVEntryS1E5428 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern char str_kT_BTNKeepMsg[];
extern char str_kT_MSGFAQNumber[];
extern char D_004A2138[];

void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int func_001E2EA0(int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001E55D0(cOVState_ViewPopup* self, int idx);
extern "C" void func_001E5780(cOVState_ViewPopup* self, int idx);

extern "C" void func_001E5428(cOVState_ViewPopup* self)
{
    sPopupEntry18_5428* a = &D_004C6C08_5428[self->miMessage];
    sPopupEntry14_5428* e = &D_00441630_5428[a->field_0x4];
    if (e->type == 4) {
        void* o = self->mpButtonSquare;
        if (o != 0) {
            sVEntryK1E5428* vt = *(sVEntryK1E5428**)((char*)o + 8);
            vt[9].fn((char*)o + vt[9].delta, 0);
        }
        o = (void*)self->mpDeleteMessage;
        if (o != 0) {
            sVEntryK1E5428* vt = *(sVEntryK1E5428**)((char*)o + 8);
            vt[9].fn((char*)o + vt[9].delta, 0);
        }
        if (self->mpKeepMessage != 0) {
            cUIText_setUnicodeStringByID(self->mpKeepMessage, GetHashValue32(str_kT_BTNKeepMsg));
        }
    }
    if (self->mpNumber != 0) {
        if (e->type == 4) {
            unsigned short buf[0x18];
            char* mgr = (char*)SSXApp->FELocale;
            sVEntryS1E5428* vt = *(sVEntryS1E5428**)(mgr + 4);
            unsigned short* fmt = vt[4].fn(mgr + vt[4].delta, GetHashValue32(str_kT_MSGFAQNumber));
            sprintf1(buf, fmt, func_001E2DC0(self->miMessage));
            cUIText_setUnicodeString(self->mpNumber, buf);
        } else {
            char buf[0x30];
            sprintf(buf, D_004A2138, self->miNumber);
            cUIText_setAsciiString(self->mpNumber, buf);
        }
    }
    if (self->mpFrom != 0) {
        cUIText_setUnicodeStringByID(self->mpFrom, func_001E2EA0(a->field_0xC));
    }
    func_001E55D0(self, self->miMessage);
    func_001E5780(self, self->miMessage);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E55D0);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void func_001E5638(cOVState_ViewPopup* self, int idx);

extern "C" void func_001E55D0(cOVState_ViewPopup* self, int idx)
{
    char* e = (char*)&D_004C6C08[idx];
    if (*(int*)(e + 0x8) == 1) {
        func_001E5638(self, idx);
    } else {
        cUIText* uiText = self->mpSubject;
        if (uiText != 0) {
            cUIText_setUnicodeStringByID(uiText, *(int*)(e + 0x10));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5638);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" unsigned char func_001E5AA0(cOVState_ViewPopup* self, int idx);
extern "C" int func_001E3388(int id);
extern "C" unsigned char func_001E3650(int idx);
extern "C" int func_001E2D58(int v, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A20C4;
extern char str_kT_MSGSubjectBCFinishLosspct_d[];
extern char str_kT_MSGSubjectBCFinishWinpct_d[];
extern char str_kT_MSGSubjectAggressionpct_d[];
extern char str_kT_MSGSubjectForeshadowpct_d[];
extern char str_kT_MSGSubjectBeatThePeakpct_d[];

// Sets the popup text to the "level N" string matching the reward id's category.
extern "C" void func_001E5638(cOVState_ViewPopup* self, int id)
{
    char buf[32];
    unsigned char lvl;
    if (D_004A20C4 != 0) {
        lvl = func_001E5AA0(self, id);
    } else {
        lvl = func_001E3650(func_001E3388(id));
    }
    if (func_001E2D58(3, id)) {
        switch (id) {
        case 115: case 117: case 119: case 121: case 123: case 125: case 0x7F: case 129: case 131: case 133: case 135: case 137:
            sprintf(buf, str_kT_MSGSubjectBCFinishLosspct_d, lvl);
            break;
        case 116: case 118: case 0x78: case 122: case 0x7C: case 0x7E: case 0x80: case 130: case 132: case 134: case 136: case 138:
            sprintf(buf, str_kT_MSGSubjectBCFinishWinpct_d, lvl);
            break;
        }
    } else if (func_001E2D58(5, id)) {
        sprintf(buf, str_kT_MSGSubjectAggressionpct_d, lvl);
    } else if (func_001E2D58(4, id)) {
        sprintf(buf, str_kT_MSGSubjectForeshadowpct_d, lvl);
    } else if (func_001E2D58(8, id)) {
        sprintf(buf, str_kT_MSGSubjectBeatThePeakpct_d, lvl);
    }
    if (self->mpSubject != 0) {
        cUIText_setUnicodeStringByID(self->mpSubject, GetHashValue32(buf));
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5780);
#ifdef SKIP_ASM
struct sVEntry001E5780 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001E5780(cOVState_ViewPopup* self, int idx)
{
    char* e = (char*)&D_004C6C08[idx];
    int kind = *(int*)(e + 0x8);
    if (kind < 2) {
        if (kind >= 0) {
            char* t = (char*)self->mpMessageLine0;
            *(int*)(t + 0x14) |= 0x80;
            cUIText_setUnicodeStringByID(self->mpMessageLine0, *(int*)(e + 0x14));
            void* obj = self->mpMessageLine0;
            sVEntry001E5780* vt = *(sVEntry001E5780**)((char*)obj + 8);
            vt[9].fn((char*)obj + vt[9].delta, 1);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPopupEntry18_5800 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_5800 D_004C6C08_5800[] __asm__("D_004C6C08");
struct sEntry14_5800 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_5800 D_00441630_5800[] __asm__("D_00441630");
extern "C" void func_001E3268(int id);
struct sVE5800 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Handles the message view's widget events: delete the message, or close the popup.
 *
 * Event 7 deletes the shown message from the saved list (func_001E3268) unless its type (D_00441630[entry +0x4]) is
 * 4, in which case nothing happens; it then falls through to close. Events 5 and 6 (and a delete) tell the parent
 * state's onPopupEvent (slot 17) event 0x14 (back) from this popup.
 *
 * @param pWidget  the widget (On Tour pWidget); not used
 * @param msg  the widget event (On Tour Event)
 */
extern "C" void cOVState_ViewPopup_onWidgetEvent(cOVState_ViewPopup* self, void* pWidget, unsigned int msg)
{
    switch (msg) {
    case 7: {
        int id = self->miMessage;
        sPopupEntry18_5800* popupEntry = &D_004C6C08_5800[id];
        sEntry14_5800* typeEntry = &D_00441630_5800[popupEntry->field_0x4];
        if (typeEntry->type == 4) {
            return;
        }
        func_001E3268(id);
    }
    case 5: {
        char* parentState = (char*)self->mpParentState;
        sVE5800* vt = *(sVE5800**)(parentState + 8);
        vt[17].fn(parentState + vt[17].delta, self, 20);
        break;
    }
    case 6: {
        char* parentState = (char*)self->mpParentState;
        sVE5800* vt = *(sVE5800**)(parentState + 8);
        vt[17].fn(parentState + vt[17].delta, self, 20);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E58E0);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int func_001E34D8(int i);
extern "C" int func_001E3408();
extern "C" int func_001E3460(int* out);
extern "C" void func_001E5428(cOVState_ViewPopup* self);
// PORT: func_001E33E8 is declared (void*) but called with no argument here
unsigned char func_001E33E8_noarg() __asm__("func_001E33E8__FPv");

extern "C" void func_001E58E0(cOVState_ViewPopup* self)
{
    int id = self->miMessage;
    sPopupEntry18_5800* a = &D_004C6C08_5800[id];
    sEntry14_5800* b = &D_00441630_5800[a->field_0x4];
    if (b->type == 4) {
        int n = func_001E34D8(id);
        if (n == -1) {
            char* o = (char*)self->mpParentState;
            sVE5800* vt = *(sVE5800**)(o + 8);
            vt[17].fn(o + vt[17].delta, self, 20);
            return;
        }
        self->miMessage = n;
        self->miNumber = self->miNumber + 1;
        func_001E5428(self);
    } else if (func_001E3408() > 0) {
        sPopupList* t = func_001E39F8_list();
        int idx;
        self->miMessage = func_001E3460(&idx);
        t->mask |= 1 << idx;
        self->miNumber = func_001E33E8_noarg() - idx;
        func_001E5428(self);
    } else {
        char* o = (char*)self->mpParentState;
        sVE5800* vt = *(sVE5800**)(o + 8);
        vt[17].fn(o + vt[17].delta, self, 20);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the message view's menu events; event 8 asks whether the shown message can be deleted.
 *
 * @param pMenu  the menu (On Tour pMenu); not used
 * @param Event  the menu event (On Tour Event)
 * @return 1 for events 6 and 7, 0 for 9, for 8 whether the message's type is not 4, otherwise 0x101
 */
extern "C" int cOVState_ViewPopup_onWidgetMenu(cOVState_ViewPopup* self, int pMenu, unsigned int Event)
{
    switch (Event) {
    case 6:
    case 7:
        return 1;
    case 9:
        return 0;
    case 8: {
        sPopupEntry18* popupEntry = &D_004C6C08[self->miMessage];
        sPopupEntry14* typeEntry = &D_00441630[popupEntry->field_0x4];
        return *(int*)typeEntry->pad_0x0 != 4;
    }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5AA0);
#ifdef SKIP_ASM
extern int D_004A20C4;

extern "C" unsigned char func_001E5AA0(cOVState_ViewPopup* self, int idx)
{
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    int v = func_001E2DC0(idx);
    int div = 1;
    sPopupEntry18* a = &D_004C6C08[idx];
    sPopupEntry14* b = &D_00441630[a->field_0x4];
    switch (b->field_0xC) {
    case 3:
        div = 10;
        v = v / 2;
        break;
    case 5:
        div = 11;
        break;
    case 4:
        div = 10;
        break;
    case 8:
        div = 4;
        break;
    }
    return v % div;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E5B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sInfo5B80 {
    int kind;
    int f04;
    int f08;
    int f0C;
    int count;
    int f14;
    int a18[10];
    int a40[12];
    int f70;
};

extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern void* func_00144BF0_5B80(void* iface) __asm__("func_00144BF0");
extern void func_002C2540_5B80(unsigned short* dst, const char* src) __asm__("func_002C2540");
extern void sprintf2_5B80(unsigned short* dst, const unsigned short* fmt, ...) __asm__("sprintf2");
extern "C" char* func_0014EEC8(void* iface, int a1, int a2);
extern "C" int cGameModeMan_getHeatScore(sInfo5B80* info, int idx, int which);
extern int D_00536730[];
extern int cGameModeMan_bGiveUp[];
extern int D_00536708[];
extern char* D_00441D10[];
extern unsigned short D_004A2140_5B80[] __asm__("D_004A2140");
extern char str_42freestyle_standings[];
extern char str_OV_darkblue[];
extern char str_track_event[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_title_b[];
extern char str_kT_OVRCMNQFHeat1Stand[];
extern char str_total[];
extern char str_kT_OVRCMNQFHeat2Stand[];
extern char str_heat1[];
extern char str_kT_OVRCMNScoreColon[];
extern char str_rider_pct_d[];
extern char str_run_1_pct_d[];
extern char str_DNF[];
extern char str_run_2_pct_d[];
extern char str_total_pct_d[];
extern char D_004A2168[];
extern char str_help_current0pct_d[];
extern char str_kT_CMNHELPEarnedEnoughPoints[];
extern char str_help_advance[];
extern char str_help_sorry[];
extern char str_overlayOptpct_d[];
extern char str_kT_OVRCMNFinalRound[];


struct sIdx5_5B80 {
    int v[5];
};
extern sIdx5_5B80 D_0046E488;

struct sVEntry5B80 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cUIObj_5B80 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void setVisible(int on);
};

extern "C" void func_001E5B80(void* self)
{
    unsigned short title[0x38];
    unsigned short line[104];
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_42freestyle_standings), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    signed char event = cBENewRaceInterface_newRace.cGameMode;
    sInfo5B80* info = (sInfo5B80*)SSXApp->GameModeMan;
    int kind = info->kind;
    cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_track_event));
    func_002C2540_5B80(title, (char*)func_00144BF0_5B80(iface0));
    unsigned short* fmt = 0;
    switch (event) {
    case 1: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry5B80* vt = *(sVEntry5B80**)(o + 4);
        fmt = vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_cmnSlopeCaps));
        break;
    }
    case 2: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry5B80* vt = *(sVEntry5B80**)(o + 4);
        fmt = vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_cmnBigAirCaps));
        break;
    }
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry5B80* vt = *(sVEntry5B80**)(o + 4);
        fmt = vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_cmnPipeCaps));
        break;
    }
    }
    sprintf2_5B80(line, D_004A2140_5B80, title, fmt);
    cUIText_setUnicodeString(uiText, line);
    uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_title_b));
    switch (kind) {
    case 1:
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQFHeat1Stand));
        cUIState_hideObjSafe(self, str_total);
        break;
    case 2:
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQFHeat2Stand));
        break;
    }
    uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_heat1));
    if (cBENewRaceInterface_newRace.cGamePath != 0) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNScoreColon));
    }
    int flag = 0;
    if (kind == 1 && info->f70 == 3) {
        flag = 1;
    }
    int pl0 = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int pl1;
    int any = 0;
    pl1 = cBENewPlayerInterface_getHumanRider(iface1, 1);
    int last;
    {
        char name[32];
        char num[16];
        char num2[32];
        int i;
        for (i = 1; i < info->count + 2; i++) {
            int idx = D_00536708[i - 1];
            sprintf(name, str_rider_pct_d, i);
            int hl = 0;
            uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            if (i - 1 == D_00536730[pl0]) {
                if (cGameModeMan_bGiveUp[pl0]) hl = 1; else hl = 0;
            } else if (cBENewRaceInterface_newRace.cGamePath == 2 && i - 1 == D_00536730[pl1]) {
                if (cGameModeMan_bGiveUp[pl1]) hl = 1; else hl = 0;
            }
            cUIText_setAsciiString(uiText, func_0014EEC8(iface2, info->a18[idx], info->a40[idx]));
            sprintf(name, str_run_1_pct_d, i);
            uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            if (hl && kind == 1) {
                cUIText_setAsciiString(uiText, str_DNF);
                any = 1;
            } else {
                sprintf(num, D_004A2138, cGameModeMan_getHeatScore(info, idx, 0));
                cUIText_setAsciiString(uiText, num);
            }
            sprintf(name, str_run_2_pct_d, i);
            uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            sprintf(name, str_total_pct_d, i);
            cUIObj_5B80* b = (cUIObj_5B80*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            if (kind == 2 || flag) {
                if (hl) {
                    cUIText_setAsciiString(uiText, str_DNF);
                    any = 1;
                    b->setVisible(0);
                } else {
                    sprintf(num, "%d", cGameModeMan_getHeatScore(info, idx, 1));
                    cUIText_setAsciiString(uiText, num);
                    int total = cGameModeMan_getHeatScore(info, idx, 0) + cGameModeMan_getHeatScore(info, idx, 1);
                    sprintf(num2, "%d", total);
                    cUIText_setAsciiString((cUIText*)b, num2);
                    if (i - 1 == D_00536730[pl0] && flag) {
                        cUIText_setAsciiString(uiText, D_004A2168);
                    }
                }
            } else {
                cUIText_setAsciiString(uiText, D_004A2168);
                b->setVisible(0);
            }
        }
        last = -1;
        int k;
        for (k = 1; k < 7; k++) {
            sprintf(name, str_help_current0pct_d, k);
            int s = D_00536730[cBENewPlayerInterface_getHumanRider(iface1, 0)];
            int m = s < 6 ? s : 5;
            if (kind == 1 && m + 1 == k && !any) {
                last = k;
                cUIState_showObjSafe(self, name);
            } else {
                cUIState_hideObjSafe(self, name);
            }
        }
        if (flag) {
            sprintf(name, str_help_current0pct_d, last);
            cUIText* o = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            cUIText_setUnicodeStringByID(o, GetHashValue32(str_kT_CMNHELPEarnedEnoughPoints));
        }
    }
    {
        sIdx5_5B80 ids = D_0046E488;
        char name[32];
        int i;
        cUIState_hideObjSafe(self, str_help_advance);
        cUIState_hideObjSafe(self, str_help_sorry);
        if (any) {
            ids.v[0] = 2;
        } else if (kind == 2) {
            if (D_00536730[cBENewPlayerInterface_getHumanRider(iface1, 0)] < 3) {
                cUIState_showObjSafe(self, str_help_advance);
            } else {
                cUIState_showObjSafe(self, str_help_sorry);
                ids.v[0] = kind;
            }
        }
        for (i = 0; i < 5; i++) {
            sprintf(name, str_overlayOptpct_d, i);
            cUIObj_5B80* o = (cUIObj_5B80*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            if (o != 0) {
                if (i < 5) {
                    cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_00441D10[ids.v[i]]));
                    if (i == 0 && ids.v[0] == 1 && info->f70 == 3) {
                        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNFinalRound));
                    }
                    *(int*)((char*)o + 0x18) = ids.v[i];
                } else {
                    o->setVisible(0);
                    o->v08(1);
                }
            }
        }
    }
}
#endif

extern "C" void* cOVState_onUpdate(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E64A8__FPv);
#ifdef SKIP_ASM
void* func_001E64A8(void* self)
{
    return cOVState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E64C8);
#ifdef SKIP_ASM
// cUIState_onGainFocus returns nothing (ends in a void vcall); the unit's later void* declaration is a guess.
void cUIState_onGainFocus_v(void* self) __asm__("cUIState_onGainFocus");
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_00536730[];
extern char str_rider_pct_d[];
extern char str_at_TextPulseWhiteOrange[];
extern char str_run_pct_d_pct_d[];

extern "C" void func_001E64C8(void* self)
{
    char buf[32];
    cUIState_onGainFocus_v(self);
    int* p = (int*)SSXApp->GameModeMan;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int x = *p;
    int n = D_00536730[cBENewPlayerInterface_getHumanRider(iface, 0)] + 1;
    if (n >= 7) {
        n = 6;
    }
    sprintf(buf, str_rider_pct_d, n);
    cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    sprintf(buf, str_run_pct_d_pct_d, x, n);
    cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E65B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_overlayMenu[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void func_0026F898(void* self, int val);

extern "C" int func_001E65B8(void* self, void* a1)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_overlayMenu));
    if (obj != 0) {
        if (*(unsigned char*)((char*)obj + 0x95) == 2) {
            func_0026F898(SSXApp->Game->Replay, *(unsigned char*)((char*)a1 + 4));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6630);
#ifdef SKIP_ASM
extern "C" int func_001E6630(void* self, int widget, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6640);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

extern "C" void func_001E6640(void* self, void* popup, int event)
{
    if (popup != 0 && event == 5) {
        int popupId = *(int*)((char*)popup + 0x18);
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6668);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_43final_standings[];
extern char str_OV_darkblue[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onCreateScreen(void* self);

extern "C" void func_001E6668(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_43final_standings), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    *(int*)((char*)self + 0x9C) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E6718);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile).
// Declarations shared by the functions of this unit.
struct cUIText;

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");


extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);


extern "C" int sprintf(char* buf, const char* fmt, ...);

extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);

extern "C" void cUIText_setUnicodeString(void* text, void* p);

void cUIText_setAsciiString(cUIText* text, const char* str);

extern char D_004A2138[];

extern int D_00536730[];


extern char str_helptext_b[];

extern "C" int func_004139F8(float f);

extern "C" char* func_0020A1A8(int v);

extern unsigned int D_00536640[];

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" unsigned short* func_002C2540(void* dst, const char* s);
extern "C" void sprintf2(void* dst, const char* fmt, ...);
extern "C" const char* func_00144BF0(void* iface);
extern "C" char* func_00146F88(void* iface, int idx);
extern "C" char* func_0014EEC8(void* iface, int id, int a2);
extern "C" void cUIText_setUnicodeStringWrap(void* text, unsigned short* str);
extern "C" void cUIIcon_setTextureByHashValue(void* obj, int hash);
extern int D_00536708[];
extern int D_005366A8[];
extern char* D_00441D10[];
extern int D_004A213C;
struct sIdx5_6718 {
    int v[5];
};
extern sIdx5_6718 D_0046E648;
extern char str_screenData[];
extern char str_track_event[];
extern char str_kT_cmnRaceCaps_b[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char D_0046E558[];
extern char D_004A2140[];
extern char str_title_b[];
extern char str_kT_OVRCMNMultiPlayResults[];
extern char str_kT_OVRCMNQuickPlayResults[];
extern char str_kT_OVRQualifierResults[];
extern char str_kT_OVRSemiFinalResults[];
extern char str_kT_OVRFinalResults[];
extern char str_kT_TITLERivalChallenge[];
extern char str_finalScoreTime[];
extern char str_kT_OVRCMNTime_b[];
extern char str_kT_OVRCMNScore[];
extern char str_riderpct_d[];
extern char str_timePointspct_d[];
extern char str_DNF[];
extern char str_rankpct_d[];
extern char str_kT_HELPCongratWonChal[];
extern char str_kT_OVRHELPSorryLose[];
extern char str_kT_OVRCongratGoToSemi[];
extern char str_kT_OVRRaceObjectives7[];
extern char str_kT_OVRCMNSorryGetThirdSemi[];
extern char str_kT_OVRCMNSorryGetThird[];
extern char str_kT_OVRCongratulationsPlatinum[];
extern char str_kT_OVRCongratulationsGold[];
extern char str_kT_OVRCongratulationsSilver[];
extern char str_kT_OVRCongratulationsBronze[];
extern char str_kT_OVRCMNCongratP1[];
extern char str_gold[];
extern char str_silver[];
extern char str_bronze[];
extern char str_platinum[];
extern char str_overlayOptpct_d[];
extern char str_kT_OVRCMNFinalRound[];

// EA's 3-float vector (prototype name, as in fe/fepopup).
struct t3Vector {
    float x, y, z;
};

// The race setup at SSXApp + 0xC0 (mode, then per-slot rider and outfit ids).
struct sRaceSetup_6718 {
    int mode;
    char pad_0x4[0x14];
    int rider[10];              // 0x18
    int outfit[10];             // 0x40
    char pad_0x68[0x8];
    int f70;                    // 0x70
};

static inline unsigned short* Loc1E6718(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = *(sVEntry<unsigned short*(void*, int)>**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

extern "C" int func_001E6718(void* self)
{
    if (*(int*)((char*)self + 0x9C) != 0) {
        *(int*)((char*)self + 0x9C) = 0;
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
        void* net = cBE_getInterface_Fv(cBE_getBE(), 2);
        sRaceSetup_6718* p = (sRaceSetup_6718*)SSXApp->GameModeMan;
        int mode = p->mode;
        int track = cBENewRaceInterface_newRace.cGameMode;
        int players = cBENewRaceInterface_newRace.cGamePath;
        if ((track != 0 && players == 2) || (track == 5 || track == 6)) {
            cUIThing* o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_screenData));
            o->setVisible(1);
            t3Vector pos = *(t3Vector*)((char*)o + 0x44);
            t3Vector v;
            v.x = pos.x;
            v.y = 200.0f;
            v.z = pos.z;
            *(t3Vector*)((char*)o + 0x44) = v;
        }
        cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_track_event));
        unsigned short name[0x38];
        func_002C2540(name, func_00144BF0(iface0));
        unsigned short* str = 0;
        switch (cBENewRaceInterface_newRace.cGameMode) {
        case 0:
        case 5:
            str = Loc1E6718(str_kT_cmnRaceCaps_b);
            break;
        case 1:
            str = Loc1E6718(str_kT_cmnSlopeCaps);
            break;
        case 2:
            str = Loc1E6718(str_kT_cmnBigAirCaps);
            break;
        case 3:
            str = Loc1E6718(str_kT_cmnPipeCaps);
            break;
        }
        unsigned short title[104];
        // titleFmt ("%s - %s") is a variable: the target loads its %hi before the track test, which a literal
        // argument (loaded in the else arm only) does not.
        char* titleFmt = D_004A2140;
        if (track == 6) {
            sprintf2(title, D_0046E558, name);
        } else {
            sprintf2(title, titleFmt, name, str);
        }
        cUIText_setUnicodeString(uiText, title);
        uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_title_b));
        if (players == 2) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNMultiPlayResults));
        } else if (players == 1) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQuickPlayResults));
        } else if (track == 0) {
            switch (mode) {
            case 1:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRQualifierResults));
                break;
            case 2:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRSemiFinalResults));
                break;
            case 3:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRFinalResults));
                break;
            }
        } else if (track == 5 || track == 6) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_TITLERivalChallenge));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRFinalResults));
        }
        uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_finalScoreTime));
        if (track == 0 || track == 5) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNTime_b));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNScore));
        }
        int count = 6;
        if (track == 5 || track == 6) {
            count = 2;
        } else if (track == 0) {
            count = *(int*)((char*)SSXApp->Game->AI + 0x78);
        } else if (players == 2) {
            count = 2;
        }
        int pl0 = cBENewPlayerInterface_getHumanRider(player, 0);
        int pl1;
        int done = 0;
        pl1 = cBENewPlayerInterface_getHumanRider(player, 1);
        char buf[48];
        char text[0x70];
        for (int i = 0; i < 6; i++) {
            int idx = D_00536708[i];
            if (i < count) {
                sprintf(buf, str_riderpct_d, i + 1);
                int finished = 0;
                uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
                if (i == D_00536730[pl0]) {
                    if (D_005366A8[pl0] != 0) {
                        finished = 1;
                    }
                } else if (cBENewRaceInterface_newRace.cGamePath == 2 && i == D_00536730[pl1]) {
                    if (D_005366A8[pl1] != 0) {
                        finished = 1;
                    }
                }
                if (cBENewRaceInterface_newRace.cGamePath == 2 && i == D_00536730[pl0]) {
                    cUIText_setAsciiString(uiText, func_00146F88(player, pl0));
                } else if (cBENewRaceInterface_newRace.cGamePath == 2 && i == D_00536730[pl1]) {
                    cUIText_setAsciiString(uiText, func_00146F88(player, pl1));
                } else {
                    int rider = p->rider[idx];
                    int outfit = p->outfit[idx];
                    cUIText_setAsciiString(uiText, func_0014EEC8(net, rider, outfit));
                }
                sprintf(buf, str_timePointspct_d, i + 1);
                uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
                if (finished) {
                    cUIText_setAsciiString(uiText, str_DNF);
                    done = 1;
                } else if (track == 0 || track == 5) {
                    cUIText_setAsciiString(uiText, func_0020A1A8(func_004139F8(D_00536640[idx] * 0.01666666753590107f)));
                } else {
                    unsigned int time = D_00536640[idx];
                    sprintf(text, D_004A2138, time);
                    cUIText_setAsciiString(uiText, text);
                }
            } else {
                sprintf(buf, str_riderpct_d, i + 1);
                cUIState_hideObjSafe(self, buf);
                sprintf(buf, str_timePointspct_d, i + 1);
                cUIState_hideObjSafe(self, buf);
                sprintf(buf, str_rankpct_d, i + 1);
                cUIState_hideObjSafe(self, buf);
            }
        }
        sIdx5_6718 ids = D_0046E648;
        int same = 0;
        if (players == 2 && track != 0 && track != 5) {
            same = D_00536640[pl0] == D_00536640[pl1];
        }
        uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_b));
        if (players != 2) {
            if (track == 5 || track == 6) {
                if (D_00536730[pl0] == 0) {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_HELPCongratWonChal));
                } else {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRHELPSorryLose));
                }
            } else if (track == 0 && mode < 3) {
                if (D_00536730[pl0] < 3) {
                    ids.v[0] = 1;
                    if (mode == 1) {
                        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratGoToSemi));
                    } else {
                        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRRaceObjectives7));
                    }
                } else if (mode == 1) {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNSorryGetThirdSemi));
                } else {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNSorryGetThird));
                }
            } else if (D_004A213C != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsPlatinum));
            } else if (D_00536730[pl0] == 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsGold));
            } else if (D_00536730[pl0] == 1) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsSilver));
            } else if (D_00536730[pl0] == 2) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsBronze));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRHELPSorryLose));
            }
            if (players == 1) {
                ids.v[0] = 3;
            }
        } else {
            ids.v[0] = 3;
            if (*(int*)((char*)SSXApp->Game->AI + 0x78) != players || same) {
                ((cUIThing*)uiText)->setVisible(0);
            } else {
                if (D_00536730[pl0] < D_00536730[pl1]) {
                    func_002C2540(text, func_00146F88(player, pl0));
                } else {
                    func_002C2540(text, func_00146F88(player, pl1));
                }
                unsigned short msg[200];
                unsigned short* msgFmt = Loc1E6718(str_kT_OVRCMNCongratP1);
                sprintf1(msg, msgFmt, (unsigned short*)text);
                cUIText_setUnicodeStringWrap(uiText, msg);
            }
        }
        if (done && players != 2 && players != 1) {
            ids.v[0] = 2;
        }
        int show2 = 0;
        cUIState_hideObjSafe(self, str_gold);
        cUIState_hideObjSafe(self, str_silver);
        cUIState_hideObjSafe(self, str_bronze);
        int show1 = 1;
        if (cBENewRaceInterface_newRace.cGamePath != 2 && (track == 5 || track == 6) && D_00536730[pl0] != 0) {
            show1 = 0;
        }
        if (pl1 != -1 && cBENewRaceInterface_newRace.cGamePath == 2) {
            show2 = 1;
            if ((track >= 1 && track <= 3) || track == 5 || track == 6) {
                if (D_00536730[pl0] < D_00536730[pl1]) {
                    show2 = 0;
                } else {
                    show1 = 0;
                }
            }
        }
        if (same) {
            show1 = 0;
            show2 = 0;
        }
        if (mode == 3 || track == 5 || track == 6) {
            if (show1) {
                if (D_00536730[pl0] == 0) {
                    cUIState_showObjSafe(self, str_gold);
                } else if (D_00536730[pl0] == 1) {
                    cUIState_showObjSafe(self, str_silver);
                } else if (D_00536730[pl0] == 2) {
                    cUIState_showObjSafe(self, str_bronze);
                }
                if (cBENewRaceInterface_newRace.cGamePath == 0 && D_004A213C != 0) {
                    void* m = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_gold));
                    cUIIcon_setTextureByHashValue(m, GetHashValue32(str_platinum));
                }
            }
            if (show2) {
                if (D_00536730[pl1] == 0) {
                    cUIState_showObjSafe(self, str_gold);
                } else if (D_00536730[pl1] == 1) {
                    cUIState_showObjSafe(self, str_silver);
                } else if (D_00536730[pl1] == 2) {
                    cUIState_showObjSafe(self, str_bronze);
                }
            }
        }
        int netIds[1] = { 8 };
        for (int i = 0; i < 5; i++) {
            sprintf(text, str_overlayOptpct_d, i);
            cUIThing* button = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(text));
            if (button != 0) {
                cBE_getInterface_Fv(SSXApp->BE, 7);
                if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                    if (i < 1) {
                        cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(D_00441D10[netIds[i]]));
                        *(int*)((char*)button + 0x18) = netIds[i];
                    } else {
                        button->setVisible(0);
                        button->setStatic(1);
                    }
                } else {
                    if (i < 5) {
                        cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(D_00441D10[ids.v[i]]));
                        if (i == 0 && ids.v[0] == 1 && p->f70 == 3) {
                            cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(str_kT_OVRCMNFinalRound));
                        }
                        *(int*)((char*)button + 0x18) = ids.v[i];
                    } else {
                        button->setVisible(0);
                        button->setStatic(1);
                    }
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7558);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_overlayMenu[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern signed char D_00535C11[];

extern "C" void func_001E7558(void* self, void* menu)
{
    int hash = *(int*)((char*)menu + 0x38);
    if (hash == GetHashValue32(str_overlayMenu)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 1) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E75D0);
#ifdef SKIP_ASM
// cUIState_onGainFocus returns nothing (ends in a void vcall); the unit's later void* declaration is a guess.
void cUIState_onGainFocus_v(void* self) __asm__("cUIState_onGainFocus");
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern int D_00536730[];
extern char str_riderpct_d[];
extern char str_at_TextPulseWhiteOrange[];
extern char str_timePointspct_d[];
extern char str_ChatGroup[];
extern char str_chatText[];
extern char str_kT_POPUP235Chat_b[];
extern char str_helptexts[];
extern char str_textScrollChat_b[];

struct sRect_75D0 {
    int a, b, c, d;
};
extern sRect_75D0 D_004C8408;

extern "C" void func_001E75D0(void* self)
{
    char buf[32];
    int second = 0;
    cUIState_onGainFocus_v(self);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGamePath;
    int track = cBENewRaceInterface_newRace.cGameMode;
    int first = 1;
    if (mode == 2) {
        second = 1;
        if ((track >= 1 && track <= 3) || track == 5 || track == 6) {
            first = 0;
            second = 0;
        }
    }
    if (first) {
        sprintf(buf, str_riderpct_d, D_00536730[cBENewPlayerInterface_getHumanRider(player, 0)] + 1);
        cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
        sprintf(buf, str_timePointspct_d, D_00536730[cBENewPlayerInterface_getHumanRider(player, 0)] + 1);
        cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    }
    if (second) {
        sprintf(buf, str_riderpct_d, D_00536730[cBENewPlayerInterface_getHumanRider(player, 1)] + 1);
        cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
        sprintf(buf, str_timePointspct_d, D_00536730[cBENewPlayerInterface_getHumanRider(player, 1)] + 1);
        cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cGameComm_mGameComm != 0 && cGameComm_mGameComm->mConnected != 0) {
        cUIObjK1DEB90* o = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ChatGroup));
        if (o != 0) {
            o->v09(1);
        }
        cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_chatText));
        if (uiText != 0) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_POPUP235Chat_b));
        }
        o = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptexts));
        if (o != 0) {
            o->v09(1);
        }
        char* p = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_textScrollChat_b));
        *(int*)(p + 0x74) |= 1;
        *(sRect_75D0*)(p + 0xA8) = D_004C8408;
        cUITextScroll_setInputMap(p, 1);
        ((cUIObjK1DEB90*)p)->v09(0);
    }
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7920__FPv);
#ifdef SKIP_ASM
void func_001E7920(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7928);
#ifdef SKIP_ASM
extern void* cAppMan_TheAppMan;
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;
extern "C" int cOVState_onUpdate_HandleIncomingChat(void* self);
extern char str_helptext_b[];
extern char str_helptexts[];
extern char str_ChatGroup[];
extern char str_chatText[];
extern char str_kT_POPUP235Chat_b[];

extern "C" void func_001E7928(void* self)
{
    cOVState_onUpdate(self);
    if (cGameComm_mGameComm != 0) {
        if (cGameComm_mGameComm->mConnected != 0) {
            if (cOVState_onUpdate_HandleIncomingChat(self) != 0) {
                cUIObjK1DEB90* o = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_b));
                if (o != 0) {
                    o->v09(0);
                }
                o = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptexts));
                if (o != 0) {
                    o->v09(0);
                }
            }
        }
        if (*(int*)((char*)cAppMan_TheAppMan + 0x18) % 30 == 0 && cGameComm_mGameComm->mConnected == 0) {
            cUIObjK1DEB90* o = (cUIObjK1DEB90*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ChatGroup));
            if (o != 0) {
                o->v09(0);
            }
            cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_chatText));
            if (text != 0) {
                cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUP235Chat_b));
            }
        }
        if (cGameComm_mGameComm->mOverlayTimeout == 0) {
            cGameComm_mGameComm->mOverlayTimeout = -1;
            cOVStateManager_mMiscData = 8;
            cOVStateManager_mStatus = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7AA8);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_triggerOverlay(int state);

struct sVEntry001E7AA8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_001E7AA8(void* self, void* a1)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_overlayMenu));
    if (obj != 0) {
        if (*(unsigned char*)((char*)obj + 0x95) == 2) {
            func_0026F898(SSXApp->Game->Replay, *(unsigned char*)((char*)a1 + 4));
        }
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cGameComm_mGameComm != 0 && cGameComm_mGameComm->mConnected != 0) {
        sVEntry001E7AA8* vt = *(sVEntry001E7AA8**)((char*)a1 + 8);
        if (vt[27].fn((char*)a1 + vt[27].delta) != 0) {
            cOVStateManager_triggerOverlay(40);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7B78);
#ifdef SKIP_ASM
extern "C" int func_001E7B78(void* self, int widget, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7B88);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

extern "C" void func_001E7B88(void* self, void* popup, int event)
{
    if (popup != 0 && event == 5) {
        int popupId = *(int*)((char*)popup + 0x18);
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E7BB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int func_004139F8(float f);
extern "C" char* func_0020A1A8(int v);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern int D_004A213C;
extern int D_00536730[];
extern unsigned int D_00536640[];
extern char str_finishov[];
extern char str_Menu0000_b[];
extern char str_firstrun[];
extern char str_secondrun[];
extern char str_thirdrun[];
extern char str_finalrun[];
extern char str_pointstotal[];
extern char str_new_record[];
extern char str_screendata[];
extern char str_platinum[];
extern char str_gold[];
extern char str_silver[];
extern char str_bronze[];
extern char str_placepct_d[];
extern char str_3D_Ov[];

class cUIObj_7BB0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

extern "C" void func_001E7BB0(void* self)
{
    char buf[32];
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_finishov), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    *(int*)((char*)self + 0x9C) = 0;
    char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu0000_b));
    if (obj != 0) {
        *(int*)(obj + 0x90) |= 8;
    }
    int* p = (int*)SSXApp->GameModeMan;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    char* iface8 = (char*)cBE_getInterface_Fv(cBE_getBE(), 8);
    char* ifaceD = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int mode = *p;
    int sel = D_00536730[pl];
    cUIState_hideObjSafe(self, str_firstrun);
    cUIState_hideObjSafe(self, str_secondrun);
    cUIState_hideObjSafe(self, str_thirdrun);
    cUIState_hideObjSafe(self, str_finalrun);
    cUIObj_7BB0* o = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_pointstotal));
    cUIText_setAsciiString((cUIText*)o, func_0020A1A8(func_004139F8(D_00536640[pl] * 0.01666666753590107f)));
    int off = pl * 4;
    int i;
    for (i = 1; i < 7; i++) {
        sprintf(buf, str_placepct_d, i);
        o = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        o->setVisible(0);
        if (i == sel + 1) {
            o->setVisible(1);
        }
    }
    if (*(int*)(ifaceD + 0x14) == 0 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 4)) {
        D_004A213C = 1;
        cUIState_showObjSafe(self, str_platinum);
    } else {
        D_004A213C = 0;
        cUIState_hideObjSafe(self, str_platinum);
    }
    if (*(int*)(ifaceD + 0x14) == 1 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 4)) {
        cUIState_showObjSafe(self, str_gold);
    } else {
        cUIState_hideObjSafe(self, str_gold);
    }
    if (*(int*)(ifaceD + 0x14) == 2 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 4)) {
        cUIState_showObjSafe(self, str_silver);
    } else {
        cUIState_hideObjSafe(self, str_silver);
    }
    if (*(int*)(ifaceD + 0x14) == 3 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 4)) {
        cUIState_showObjSafe(self, str_bronze);
    } else {
        cUIState_hideObjSafe(self, str_bronze);
    }
    o = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_new_record));
    int vis = 0;
    if (*(int*)(iface8 + 0x18) >= 0 || *(int*)(iface8 + 0x1C) >= 0) {
        vis = 1;
    }
    if (vis) {
        o->setVisible(1);
    } else {
        o->setVisible(0);
    }
    char* r = *(char**)((char*)SSXApp->Game->AI + off + 0x28);
    if (cBENewRaceInterface_newRace.cGamePath == 2
        || *(int*)(r + 0x480) != 0
        || cBENewRaceInterface_newRace.cGameEvent == 6 || cBENewRaceInterface_newRace.cGameEvent == 7 || cBENewRaceInterface_newRace.cGameEvent == 8) {
        cUIState_hideObjSafe(self, str_screendata);
        cUIState_hideObjSafe(self, str_3D_Ov);
    } else {
        cSSXAudio_MenuSound(func_0028B180(), 0xE);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8098);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8098(void* self)
{
    float limit = 3.0f;
    int idx = cBENewPlayerInterface_getHumanRider(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    char* riders = (char*)SSXApp->Game->AI;
    char* rider = *(char**)(riders + (idx << 2) + 0x28);
    if (*(int*)(rider + 0x480) != 0) {
        limit = 1.0f;
    }
    cOVState_onUpdate(self);
    *(float*)((char*)self + 0x9C) += 0.01666666753590107f;
    if (*(int*)((char*)self + 0xA0) != 0 && *(float*)((char*)self + 0x9C) >= limit) {
        cUIStateStack_purgeNormalStates((char*)SSXApp->Game->UI + 0x18);
        func_00231320(SSXApp->Game);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8160);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8160(void* self, void* popup, int event)
{
    if (popup != 0) {
        if (event == 5 && *(int*)((char*)self + 0xA0) != 0) {
            char* game = (char*)SSXApp->Game;
            cUIStateStack_purgeNormalStates(*(char**)(game + 0x48) + 0x18);
            func_00231320(game);
        }
    }
}
#endif

//88.13% - logic verified correct; GCC allocates v0 to the comparison
// constant and v1 to the result, the target does the reverse. No source
// shape tried (temp local, early return, nested ternary) flips it.
//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E81B0);
#ifdef SKIP_ASM
extern "C" int func_001E81B0(void* self, int widget, int key)
{
    int result = 0;
    if (key == 6) {
        if (*(int*)((char*)self + 0xa0) == 0) {
            result = 0;
        } else {
            return 257;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E81D0);
#ifdef SKIP_ASM
extern "C" int func_001E81D0(void* self, int a1)
{
    switch (a1) {
    case 0:
        *(int*)((char*)self + 0xA0) = 0;
        break;
    case 1:
        *(int*)((char*)self + 0xA0) = 1;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8200);
#ifdef SKIP_ASM
// Wide-string copy; returns dst.
extern "C" unsigned short* strcpy1(unsigned short* dst, const unsigned short* src);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNPointstotal[];
extern char str_place6[];

class cUIObj_8200 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

struct sVEntry8200 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern "C" void func_001E8200(void* self)
{
    char buf[32];
    unsigned short fmt[104];
    unsigned short text[104];
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_finishov), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    *(int*)((char*)self + 0x9C) = 0;
    char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu0000_b));
    if (obj != 0) {
        *(int*)(obj + 0x90) |= 8;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    char* iface8 = (char*)cBE_getInterface_Fv(cBE_getBE(), 8);
    char* ifaceD = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
    int* p = (int*)SSXApp->GameModeMan;
    signed char event = cBENewRaceInterface_newRace.cGameMode;
    cUIState_hideObjSafe(self, str_firstrun);
    cUIState_hideObjSafe(self, str_secondrun);
    cUIState_hideObjSafe(self, str_thirdrun);
    cUIState_hideObjSafe(self, str_finalrun);
    int mode = *p;
    if (event != 6) {
        switch (mode) {
        case 0:
        case 1:
            cUIState_showObjSafe(self, str_firstrun);
            break;
        case 2:
            cUIState_showObjSafe(self, str_secondrun);
            break;
        case 3:
            cUIState_showObjSafe(self, str_finalrun);
            break;
        }
    }
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int sel = D_00536730[pl];
    if (*(int*)(ifaceD + 0x14) == 0 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 4 || cBENewRaceInterface_newRace.cGameEvent == 5)) {
        D_004A213C = 1;
        cUIState_showObjSafe(self, str_platinum);
    } else {
        D_004A213C = 0;
        cUIState_hideObjSafe(self, str_platinum);
    }
    if (*(int*)(ifaceD + 0x14) == 1 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 5)) {
        cUIState_showObjSafe(self, str_gold);
    } else {
        cUIState_hideObjSafe(self, str_gold);
    }
    if (*(int*)(ifaceD + 0x14) == 2 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 5)) {
        cUIState_showObjSafe(self, str_silver);
    } else {
        cUIState_hideObjSafe(self, str_silver);
    }
    if (*(int*)(ifaceD + 0x14) == 3 && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == 5)) {
        cUIState_showObjSafe(self, str_bronze);
    } else {
        cUIState_hideObjSafe(self, str_bronze);
    }
    int off = pl * 4;
    int i;
    cUIObj_8200* o;
    for (i = 1; i < 7; i++) {
        sprintf(buf, str_placepct_d, i);
        o = (cUIObj_8200*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        o->setVisible(0);
        if (i == sel + 1) {
            o->setVisible(1);
        }
    }
    if ((unsigned int)sel >= 6) {
        cUIState_showObjSafe(self, str_place6);
    }
    o = (cUIObj_8200*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_pointstotal));
    char* loc = (char*)SSXApp->FELocale;
    sVEntry8200* vt = *(sVEntry8200**)(loc + 4);
    strcpy1(fmt, vt[4].fn(loc + vt[4].delta, GetHashValue32(str_kT_OVRCMNPointstotal)));
    sprintf1(text, fmt, *(unsigned int*)((char*)D_00536640 + off));
    cUIText_setUnicodeString(o, text);
    o = (cUIObj_8200*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_new_record));
    int vis = 0;
    if (*(int*)(iface8 + 0x18) >= 0 || *(int*)(iface8 + 0x1C) >= 0) {
        vis = 1;
    }
    if (vis) {
        o->setVisible(1);
    } else {
        o->setVisible(0);
    }
    char* r = *(char**)((char*)SSXApp->Game->AI + off + 0x28);
    if (cBENewRaceInterface_newRace.cGamePath == 2
        || *(int*)(r + 0x480) != 0
        || cBENewRaceInterface_newRace.cGameEvent == 9 || cBENewRaceInterface_newRace.cGameEvent == 10 || cBENewRaceInterface_newRace.cGameEvent == 11
        || (mode == 1 && p[0x70 / 4] == 3)) {
        cUIState_hideObjSafe(self, str_screendata);
        cUIState_hideObjSafe(self, str_3D_Ov);
    }
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E87B8);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E87B8(void* self)
{
    float limit = 3.0f;
    int idx = cBENewPlayerInterface_getHumanRider(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    char* riders = (char*)SSXApp->Game->AI;
    char* rider = *(char**)(riders + (idx << 2) + 0x28);
    if (*(int*)(rider + 0x480) != 0) {
        limit = 1.0f;
    }
    cOVState_onUpdate(self);
    *(float*)((char*)self + 0x9C) += 0.01666666753590107f;
    if (*(int*)((char*)self + 0xA0) != 0 && *(float*)((char*)self + 0x9C) >= limit) {
        cUIStateStack_purgeNormalStates((char*)SSXApp->Game->UI + 0x18);
        func_00231320(SSXApp->Game);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8880);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void func_00231320(void* self);

extern "C" void func_001E8880(void* self, void* popup, int event)
{
    if (popup != 0) {
        if (event == 5 && *(int*)((char*)self + 0xA0) != 0) {
            char* game = (char*)SSXApp->Game;
            cUIStateStack_purgeNormalStates(*(char**)(game + 0x48) + 0x18);
            func_00231320(game);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E88D0);
#ifdef SKIP_ASM
extern "C" int func_001E88D0(void* self, int widget, int key)
{
    int result = 0;
    if (key == 6) {
        if (*(int*)((char*)self + 0xa0) == 0) {
            result = 0;
        } else {
            return 257;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E88F0);
#ifdef SKIP_ASM
extern "C" int func_001E88F0(void* self, int a1)
{
    switch (a1) {
    case 0:
        *(int*)((char*)self + 0xA0) = 0;
        break;
    case 1:
        *(int*)((char*)self + 0xA0) = 1;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8920);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" int func_004139F8(float f);
extern "C" char* func_0020A1A8(int v);
extern "C" void* func_001FD190(int v);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern signed char D_00535C12[];
extern int D_005366A8[];
extern int D_00536730[];
extern int cGameModeMan_bGiveUp[];
extern unsigned int D_00536640[];
extern char* D_00441D10[];
struct sIdx3_8920 {
    int v[3];
};
extern sIdx3_8920 D_0046E9B0;
extern char str_70peakchal_results[];
extern char str_OV_darkblue[];
extern char str_title1[];
extern char str_title2[];
extern char str_kT_TITLERivalChallenge[];
extern char str_label_target_timescore[];
extern char str_target_timescore[];
extern char str_label_timescore[];
extern char str_timescore[];
extern char str_kT_OVRCMNTimeToBeatColon[];
extern char str_kT_OVRCMNYourTimeColon[];
extern char str_kT_OVRCMNScoreToBeatColon[];
extern char str_kT_CMNYourScore[];
extern char D_004A2138[];
extern char str_DNF[];
extern char str_helptext_b[];
extern char str_kT_HELPCongratWonChal[];
extern char str_kT_HELPSorryBeatTime[];
extern char str_kT_HELPSorryBeatScore[];
extern char str_overlayOptpct_d[];

class cUIObj_8920 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void setVisible(int on);
};

extern "C" void func_001E8920(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_70peakchal_results), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    char* p = (char*)SSXApp->GameModeMan;
    int trk = D_00535C12[0];
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_title1));
    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_TITLERivalChallenge));
    uiText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_title2));
    if (trk < 12) {
        if (trk >= 6) {
            cUIText_setUnicodeString(uiText, func_001FD190(trk));
        }
    }
    cUIText* a = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_label_target_timescore));
    cUIText* b = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_target_timescore));
    cUIText* c = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_label_timescore));
    cUIText* d = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_timescore));
    if (trk >= 6 && trk <= 8) {
        cUIText_setUnicodeStringByID(a, GetHashValue32(str_kT_OVRCMNTimeToBeatColon));
        cUIText_setUnicodeStringByID(c, GetHashValue32(str_kT_OVRCMNYourTimeColon));
        cUIText_setAsciiString(b, func_0020A1A8(func_004139F8(*(unsigned int*)(p + 0x78) * 0.01666666753590107f)));
        if (D_005366A8[0] != 0) {
            cUIText_setAsciiString(d, str_DNF);
        } else {
            cUIText_setAsciiString(d, func_0020A1A8(func_004139F8(D_00536640[0] * 0.01666666753590107f)));
        }
    } else {
        char buf[32];
        cUIText_setUnicodeStringByID(a, GetHashValue32(str_kT_OVRCMNScoreToBeatColon));
        cUIText_setUnicodeStringByID(c, GetHashValue32(str_kT_CMNYourScore));
        sprintf(buf, D_004A2138, *(int*)(p + 0x7C));
        cUIText_setAsciiString(b, buf);
        if (D_005366A8[0] != 0) {
            cUIText_setAsciiString(d, str_DNF);
        } else {
            sprintf(buf, D_004A2138, D_00536640[0]);
            cUIText_setAsciiString(d, buf);
        }
    }
    cUIObj_8920* o = (cUIObj_8920*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_b));
    int s = D_00536730[pl];
    if (s == 0) goto yes;
    if (s == 1) goto yes;
    if (s == 2) goto yes;
    if (s != 3) goto no;
yes:
    cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_HELPCongratWonChal));
    goto done;
no:
    if (trk >= 6 && trk <= 8) {
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_HELPSorryBeatTime));
    } else {
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_HELPSorryBeatScore));
    }
done:
    if (cGameModeMan_bGiveUp[pl] != 0) {
        o->setVisible(0);
    }
    {
        sIdx3_8920 ids = D_0046E9B0;
        char name[32];
        int i;
        for (i = 0; i < 5; i++) {
            sprintf(name, str_overlayOptpct_d, i);
            o = (cUIObj_8920*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
            if (o != 0) {
                if (i < 3) {
                    cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(D_00441D10[ids.v[i]]));
                    *(int*)((char*)o + 0x18) = ids.v[i];
                } else {
                    o->setVisible(0);
                    o->v08(1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E70__FPv);
#ifdef SKIP_ASM
void* func_001E8E70(void* self)
{
    return cOVState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E90__FPv);
#ifdef SKIP_ASM
int func_001E8E90(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8E98);
#ifdef SKIP_ASM
extern "C" int func_001E8E98(void* self, int widget, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8EA8);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

extern "C" void func_001E8EA8(void* self, void* popup, int event)
{
    if (popup != 0 && event == 5) {
        int popupId = *(int*)((char*)popup + 0x18);
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8ED0);
#ifdef SKIP_ASM
extern "C" void* cUIState_onGainFocus(void* self);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);

extern "C" void func_001E8ED0(void* self)
{
    cUIState_onGainFocus(self);
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8F00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_109MultiplayerStandings[];
extern char str_OV_darkblue[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onCreateScreen(void* self);
void func_001E8FC0(void* self);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);

extern "C" void func_001E8F00(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_109MultiplayerStandings), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001E8FC0(self);
    cOVState_onCreateScreen(self);
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC0__FPv);
#ifdef SKIP_ASM
void func_001E8FC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC8);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

extern "C" void func_001E8FC8(void* self, void* popup, int event)
{
    if (popup != 0 && event == 5) {
        int popupId = *(int*)((char*)popup + 0x18);
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

