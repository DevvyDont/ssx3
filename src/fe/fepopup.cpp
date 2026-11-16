#include "common.h"
#include "fe/buypopupinfo.h"
#include "fe/uistatebuypopup.h"
#include "online/gamecomm.h"
#include "fe/keyboardpopup.h"
#include "fe/screenpopup.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_cScreenPopup);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void cPopupInfo_cPopupInfo(void* self);
extern "C" void func_00416210(void* dst, int c, int n);
extern void* cScreenPopup_vtbl[];
extern int D_004A3E90;
struct sVec3_58E8 {
    float x, y, z;
    sVec3_58E8(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sQuad_58E8 { int v[4]; };
extern sQuad_58E8 D_004C6788;
extern sQuad_58E8 D_004C6798;

/**
 * Constructs a message/confirmation popup state with empty widgets and default layout settings.
 *
 * Runs the cUIState constructor (func_0039E318), stores cScreenPopup_vtbl, constructs the embedded cPopupInfo
 * (cPopupInfo_cPopupInfo at +0xBC) and points mszInputText and mszStaticText at the empty string D_004A3E90. Every
 * widget pointer, mpKeyboard, the five options and the five confirmations are cleared; miDefaultOption is -1,
 * mbReceivePrev 1, mvCurrentScale (1, 1, 1), the highlight colours come from D_004C6788 / D_004C6798 and
 * miMaxChars is 0x3F.
 *
 * @param engine  the UI engine that owns the state
 * @param owner  the parent state that receives the popup's events (mpParentState)
 * @return @p self
 */
extern "C" void* cScreenPopup_cScreenPopup(cScreenPopup* self, void* engine, void* owner)
{
    func_0039E318(self, engine, owner);
    self->mVtbl = cScreenPopup_vtbl;
    cPopupInfo_cPopupInfo((char*)self + 0xBC);
    int d = D_004A3E90;
    self->mszInputText = (char*)d;
    self->mszStaticText = (char*)d;
    self->miDefaultOption = -1;
    self->mpKeyboard = 0;
    self->mbShowing = 0;
    self->mbFirstGainTran = 0;
    self->meType = 0;
    self->mbPassUpdate = 0;
    self->miLayerOffset = 0;
    for (int j = 0; j < 5; j++)
        func_00416210((char*)self + 0x2D0 + j * 12, 0, 12);
    self->mbReceivePrev = 1;
    self->mpBackGroundWhite = 0;
    self->mpBackGroundRed = 0;
    self->mpMenu = 0;
    self->mpButtonGroup = 0;
    self->mpMessageText = 0;
    self->mpPostText = 0;
    self->mpTitleText = 0;
    self->mpEnterBox = 0;
    self->mpTextBoxPair = 0;
    self->mpStaticBox = 0;
    self->mpEnterBoxBackground = 0;
    self->mpBackGroundGreyFade = 0;
    self->mpBackGroundLarge = 0;
    self->mpBackGroundLargeOutline = 0;
    self->mpBackGroundSmall = 0;
    self->mpBackGroundSmallOutline = 0;
    self->mpBackGroundShadow = 0;
    self->mpBackGroundBlack = 0;
    for (int i = 0; i < 5; i++)
        ((int*)((char*)self + 0x90))[i] = 0;
    self->meBackgroundType = 0;
    *(sVec3_58E8*)&self->mvCurrentScale[0] = sVec3_58E8(1.0f, 1.0f, 1.0f);
    self->mbCenterOptions = 0;
    self->mbCenterText = 0;
    *(sQuad_58E8*)&self->mtHighlightedColor[0] = D_004C6788;
    *(sQuad_58E8*)&self->mtUnhighlightColor[0] = D_004C6798;
    self->miMaxChars = 0x3F;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup__cScreenPopup);
#ifdef SKIP_ASM
extern void* cScreenPopup_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cPopupInfo__cPopupInfo(void* self, int flags);
extern "C" void func_0039E390(void* self, int flags);

/**
 * Destroys a screen popup: its two strings and popup info, then the cUIState base.
 *
 * Restores cScreenPopup_vtbl, destroys mszStaticText and mszInputText (cBXString destructor, flags 2) and the
 * embedded cPopupInfo (cPopupInfo__cPopupInfo, flags 2), then calls the cUIState destructor func_0039E390(self, flags).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScreenPopup__cScreenPopup(cScreenPopup* self, int flags)
{
    self->mVtbl = cScreenPopup_vtbl;
    cBXString__cBXString((char*)self + 0x2B4, 2);
    cBXString__cBXString((char*)self + 0x2B0, 2);
    cPopupInfo__cPopupInfo((char*)self + 0xBC, 2);
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A1C40[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the popup's "popup" screen and starts it at frame 0.
 *
 * Adds the screen through cUIEngine_addScreenByHashName() and stores it in mpScreen; when that succeeds, sets
 * mbShowing and plays frame 0.
 */
extern "C" void cScreenPopup_onCreateScreen(cScreenPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A1C40), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        self->mbShowing = 1;
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

void* cUIState_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Clears the popup's showing flag, then destroys its screens with the base cUIState_onDestroyScreen().
 *
 * @return the result of cUIState_onDestroyScreen()
 */
void* cScreenPopup_onDestroyScreen(void* self)
{
    *(int*)((char*)self + 0x2b8) = 0;
    return cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onGainFocus);
#ifdef SKIP_ASM
struct sVE5B88 {
    short delta;
    short index;
    void* fn;
};
extern "C" void cScreenPopup_SetTextObjectsAlpha(cScreenPopup* self, float t);
extern "C" void cScreenPopup_setPrivateDefaultOption(cScreenPopup* self, void* menu, int idx, int max);

/**
 * Re-activates the popup's menu when the popup gets the focus back.
 *
 * Only when mpMenu exists: sets the text alpha to 1 (cScreenPopup_SetTextObjectsAlpha()), calls the menu's
 * slot 8 with 0, selects the default option with cScreenPopup_setPrivateDefaultOption(), calls the menu's
 * slot 7 with 1, then recolours the entries through its own onWidgetMenu (slot 20) with message 5.
 */
extern "C" void cScreenPopup_onGainFocus(cScreenPopup* self)
{
    if (self->mpMenu != 0) {
        cScreenPopup_SetTextObjectsAlpha(self, 1.0f);
        {
            char* o = (char*)self->mpMenu;
            sVE5B88* vt = *(sVE5B88**)(o + 8);
            ((void (*)(void*, int))vt[8].fn)(o + vt[8].delta, 0);
        }
        cScreenPopup_setPrivateDefaultOption(self, self->mpMenu, self->miDefaultOption, self->meType);
        {
            char* o = (char*)self->mpMenu;
            sVE5B88* vt = *(sVE5B88**)(o + 8);
            ((void (*)(void*, int))vt[7].fn)(o + vt[7].delta, 1);
        }
        {
            sVE5B88* vt = (sVE5B88*)self->mVtbl;
            ((int (*)(void*, void*, int, int))vt[20].fn)((char*)self + vt[20].delta, self->mpMenu, 5, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onGainTransition);
#ifdef SKIP_ASM
struct cUIScreen;
struct sColor_7738;
struct sColF_5C30 { float r, g, b; };
struct sVec3_5C30 { float x, y, z; };
int GetHashValue32(char* str);
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern "C" void cScreenPopup_SetTextObjectsAlpha(cScreenPopup* self, float t);
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, void* p);
extern "C" void cScreenPopup_scaleBackground(cScreenPopup* self, sColor_7738* color);
extern char D_004A1C48[];

/**
 * Runs the popup's opening transition: lays the popup out, then grows the blue background frame by frame.
 *
 * On the first call it sets mbFirstGainTran, builds the popup from its info (cScreenPopup_initScreenPopup())
 * and scales the background to (0.2, 0.2, 1). For the blue background (meBackgroundType 1) the text starts
 * transparent and the scale grows linearly to mvCurrentScale over miStartFrame frames (the frame of the
 * screen label "Start", else 25); other backgrounds are set to mvCurrentScale at once. In the animated frames
 * only x and y of the scale are written, so z is whatever is on the stack.
 *
 * @param first  nonzero on the first call of the transition (On Tour: bFirstTime)
 * @return 0 while the background is still growing, 1 when the transition is done
 */
extern "C" int cScreenPopup_onGainTransition(cScreenPopup* self, int first)
{
    if (first) {
        sColF_5C30 c;
        c.r = 0.2f;
        c.g = 0.2f;
        c.b = 1.0f;
        self->mbFirstGainTran = 1;
        cScreenPopup_initScreenPopup(self, (char*)self + 0xBC);
        cScreenPopup_scaleBackground(self, (sColor_7738*)&c);
        if (self->meBackgroundType == 1)
            cScreenPopup_SetTextObjectsAlpha(self, 0.0f);
        int f = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(D_004A1C48));
        if (f != 0xFFFF)
            self->miStartFrame = f;
        else
            self->miStartFrame = 25;
        if (self->miStartFrame == 1)
            self->miStartFrame = 2;
        self->miGainTransistionCount = 1;
        self->mvAnimateIncrement[0] = (self->mvCurrentScale[0] - 0.2f) / (float)(self->miStartFrame - 1);
        self->mvAnimateIncrement[1] = (self->mvCurrentScale[1] - 0.2f) / (float)(self->miStartFrame - 1);
    }
    if (self->meBackgroundType == 1) {
        float t = (float)self->miGainTransistionCount;
        sVec3_5C30 pos = *(sVec3_5C30*)((char*)self->mpBackGroundSmall + 0x44);
        sColF_5C30 c;
        c.r = t * self->mvAnimateIncrement[0] + 0.2f;
        c.g = t * self->mvAnimateIncrement[1] + 0.2f;
        cScreenPopup_scaleBackground(self, (sColor_7738*)&c);
        if (self->miGainTransistionCount < self->miStartFrame) {
            self->miGainTransistionCount += 1;
            return 0;
        }
    } else {
        sColF_5C30 c = *(sColF_5C30*)&self->mvCurrentScale[0];
        cScreenPopup_scaleBackground(self, (sColor_7738*)&c);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Handles the popup's lose transition by reporting it finished immediately.
 *
 * @return always 1
 */
int cScreenPopup_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_redrawPopup);
#ifdef SKIP_ASM
struct sColor_7738;
struct sColor5DD8 { int r, g, b; };
struct sVEntry001C5DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern "C" void cScreenPopup_restoreElements(cScreenPopup* self);
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, void* p);
extern "C" void cScreenPopup_scaleBackground(cScreenPopup* self, sColor_7738* color);

/**
 * Rebuilds the popup's layout from its info, once it has been shown at least once.
 *
 * Does nothing before the first gain transition (mbFirstGainTran). Otherwise it puts the widgets back
 * (cScreenPopup_restoreElements()), lays them out again from the embedded info, rescales the background to
 * mvCurrentScale and calls its own slot 6 (onGainFocus) with 0.
 */
extern "C" void cScreenPopup_redrawPopup(cScreenPopup* self)
{
    if (self->mbFirstGainTran != 0) {
        cScreenPopup_restoreElements(self);
        cScreenPopup_initScreenPopup(self, (char*)self + 0xBC);
        sColor5DD8 c = *(sColor5DD8*)&self->mvCurrentScale[0];
        cScreenPopup_scaleBackground(self, (sColor_7738*)&c);
        sVEntry001C5DD8* vt = (sVEntry001C5DD8*)self->mVtbl;
        vt[6].fn((char*)self + vt[6].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C5E58);
#ifdef SKIP_ASM
struct sVEntry_5E58 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Call9_5E58(char* o, int a)
{
    if (o != 0) {
        sVEntry_5E58* vt = *(sVEntry_5E58**)(o + 8);
        vt[9].fn(o + vt[9].delta, a);
    }
}

extern "C" void func_001C5E58(void* self, int a)
{
    char* s = (char*)self;
    Call9_5E58(*(char**)(s + 0x70), a);
    Call9_5E58(*(char**)(s + 0x74), a);
    Call9_5E58(*(char**)(s + 0x78), a);
    Call9_5E58(*(char**)(s + 0x7C), a);
    Call9_5E58(*(char**)(s + 0x88), a);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_SetTextObjectsAlpha);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct sColor_5F20 { float a, r, g, b; };
struct sVEntry_5F20 {
    short delta;
    short index;
    void (*fn)(void*, sColor_5F20*);
};
extern char D_004A1C50[];
extern char D_004A1C58[];
extern char D_004A15E0[];

static inline void SetAlpha_5F20(char* o, float t)
{
    if (o != 0) {
        sColor_5F20 c = *(sColor_5F20*)(o + 0x1C);
        c.a = t;
        sVEntry_5F20* vt = *(sVEntry_5F20**)(o + 8);
        vt[11].fn(o + vt[11].delta, &c);
    }
}

/**
 * Sets the alpha of every text widget of the popup.
 *
 * Covers the button group, the message, post and title texts, the text-box pair, the screen objects "ngca",
 * "xba" and "ps2x" and the five options; each existing widget keeps its colour (+0x1C) with only the alpha
 * replaced (slot 11).
 *
 * @param fAlpha  the new alpha, 0 to 1
 */
extern "C" void cScreenPopup_SetTextObjectsAlpha(cScreenPopup* self, float fAlpha)
{
    SetAlpha_5F20((char*)self->mpButtonGroup, fAlpha);
    SetAlpha_5F20((char*)self->mpMessageText, fAlpha);
    SetAlpha_5F20((char*)self->mpPostText, fAlpha);
    SetAlpha_5F20((char*)self->mpTitleText, fAlpha);
    SetAlpha_5F20((char*)self->mpTextBoxPair, fAlpha);
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1C50)), fAlpha);
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1C58)), fAlpha);
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A15E0)), fAlpha);
    for (int i = 0; i < 5; i++) {
        SetAlpha_5F20(((char**)(((char*)self) + 0x90))[i], fAlpha);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_FillObjectPointers);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_0045DB90[];
extern char D_00465FA8[];
extern char D_00465FB8[];
extern char D_00465FC8[];
extern char D_00465FD8[];
extern char D_00465FE8[];
extern char D_00465FF8[];
extern char D_00466008[];
extern char D_00466018[];
extern char D_00466028[];
extern char D_00466038[];
extern char D_00466048[];
extern char D_00466058[];
extern char D_00466070[];
extern char D_004A1C60[];
extern char D_004A1C68[];
extern char D_004A1C70[];
extern char D_004A1C78[];
extern char D_00466080[];

struct sV3_6260 {
    float x, y, z;
};

struct sVE_6260 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sObj_6260 {
    char pad0[0x8];
    sVE_6260* vt;                   // 0x8
    char padC[0x44 - 0xC];
    sV3_6260 pos;                   // 0x44
    void GetRect(void* out) { vt[20].fn((char*)this + vt[20].delta, out); }
};

static inline sObj_6260* find_6260(void* self, char* name)
{
    return (sObj_6260*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

#define POS_6260(o) (*(sV3_6260*)((char*)self + (o)))
#define OBJ_6260(o) (*(sObj_6260**)((char*)self + (o)))

/**
 * Looks up the popup screen's widgets by name and caches them with their home positions and sizes.
 *
 * Fills mpBackGroundWhite ("whitefg"), mpBackGroundRed ("redbg"), the large/small shapes and outlines, the
 * shadow, grey and black backgrounds, mpMenu, mpButtonGroup, the title, message and post texts, the entry
 * box, static label, text-box pair and entry background, and mpOptions[0-4] ("Option0" to "Option4"). The
 * positions go to the mvPos* fields; "whitefg", "Small shape" and "EnterBackground" also store their sizes
 * (slot 20). Widgets that are not found leave their fields unchanged.
 */
// Looks up the popup's widgets by name and caches them (and their positions) in the screen object.
extern "C" void cScreenPopup_FillObjectPointers(cScreenPopup* self)
{
    if (sObj_6260* o = find_6260(self, D_004A1C60)) {
        POS_6260(0x188) = o->pos;
        o->GetRect((char*)self + 0x1F4);
        OBJ_6260(0x48) = o;
    }
    if (sObj_6260* o = find_6260(self, D_004A1C68)) {
        POS_6260(0x194) = o->pos;
        OBJ_6260(0x4C) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00465FA8))
        OBJ_6260(0x64) = o;
    if (sObj_6260* o = find_6260(self, D_00465FB8)) {
        POS_6260(0x230) = o->pos;
        OBJ_6260(0x54) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00465FC8)) {
        POS_6260(0x224) = o->pos;
        OBJ_6260(0x50) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00465FD8)) {
        POS_6260(0x248) = o->pos;
        OBJ_6260(0x5C) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00465FE8)) {
        POS_6260(0x23C) = o->pos;
        o->GetRect((char*)self + 0x218);
        OBJ_6260(0x58) = o;
    }
    if (sObj_6260* o = find_6260(self, D_004A1C70)) {
        POS_6260(0x254) = o->pos;
        OBJ_6260(0x60) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00465FF8))
        OBJ_6260(0x68) = o;
    if (sObj_6260* o = find_6260(self, D_0045DB90)) {
        POS_6260(0x1A0) = o->pos;
        OBJ_6260(0x6C) = o;
    }
    if (sObj_6260* o = find_6260(self, D_004A1C78)) {
        POS_6260(0x1AC) = o->pos;
        OBJ_6260(0x70) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466008)) {
        POS_6260(0x1C4) = o->pos;
        OBJ_6260(0x74) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466018)) {
        POS_6260(0x1B8) = o->pos;
        OBJ_6260(0x7C) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466028)) {
        POS_6260(0x1D0) = o->pos;
        OBJ_6260(0x78) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466038)) {
        POS_6260(0x1DC) = o->pos;
        OBJ_6260(0x80) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466048)) {
        POS_6260(0x1E8) = o->pos;
        OBJ_6260(0x88) = o;
    }
    if (sObj_6260* o = find_6260(self, D_00466058))
        OBJ_6260(0x84) = o;
    if (sObj_6260* o = find_6260(self, D_00466070)) {
        POS_6260(0x200) = o->pos;
        o->GetRect((char*)self + 0x20C);
        OBJ_6260(0x8C) = o;
    }
    for (int i = 0; i < 5; i++) {
        char name[32];
        sprintf(name, D_00466080, i);
        sObj_6260* o = find_6260(self, name);
        if (o != 0)
            ((sObj_6260**)((char*)self + 0x90))[i] = o;
    }
}
#undef POS_6260
#undef OBJ_6260
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C66E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_001CA488(void* self, void* obj);
extern "C" void func_001CA4C0(void* self, void* obj);
extern sQuad_58E8 D_004C67A8;

struct sVEp_C66E8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

static inline void SetColor_C66E8(char* o, sQuad_58E8* c)
{
    if (o != 0) {
        sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
        vt[11].fn(o + vt[11].delta, c);
    }
}

extern "C" void func_001C66E8_impl(char* self) __asm__("func_001C66E8");
extern "C" void func_001C66E8_impl(char* self)
{
    switch (*(int*)(self + 0x31C)) {
    case 0:
        func_001CA4C0(self, *(void**)(self + 0x64));
        func_001CA4C0(self, *(void**)(self + 0x48));
        func_001CA4C0(self, *(void**)(self + 0x4C));
        func_001CA488(self, *(void**)(self + 0x50));
        func_001CA488(self, *(void**)(self + 0x54));
        func_001CA488(self, *(void**)(self + 0x58));
        func_001CA488(self, *(void**)(self + 0x5C));
        func_001CA488(self, *(void**)(self + 0x60));
        func_001CA488(self, *(void**)(self + 0x68));
        {
            char* o = *(char**)(self + 0x74);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x78);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x7C);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x80);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        *(sQuad_58E8*)(self + 0x32C) = D_004C6788;
        *(sQuad_58E8*)(self + 0x33C) = D_004C6798;
        break;
    case 1:
        func_001CA4C0(self, *(void**)(self + 0x64));
        func_001CA4C0(self, *(void**)(self + 0x50));
        func_001CA4C0(self, *(void**)(self + 0x54));
        func_001CA4C0(self, *(void**)(self + 0x58));
        func_001CA4C0(self, *(void**)(self + 0x5C));
        func_001CA4C0(self, *(void**)(self + 0x60));
        func_001CA488(self, *(void**)(self + 0x48));
        func_001CA488(self, *(void**)(self + 0x4C));
        func_001CA488(self, *(void**)(self + 0x68));
        {
            char* o = *(char**)(self + 0x74);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x78);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x7C);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        {
            char* o = *(char**)(self + 0x80);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6798);
            }
        }
        *(sQuad_58E8*)(self + 0x32C) = D_004C6788;
        *(sQuad_58E8*)(self + 0x33C) = D_004C6798;
        break;
    case 2:
        func_001CA488(self, *(void**)(self + 0x48));
        func_001CA488(self, *(void**)(self + 0x4C));
        func_001CA488(self, *(void**)(self + 0x50));
        func_001CA488(self, *(void**)(self + 0x54));
        func_001CA488(self, *(void**)(self + 0x58));
        func_001CA488(self, *(void**)(self + 0x5C));
        func_001CA488(self, *(void**)(self + 0x60));
        func_001CA488(self, *(void**)(self + 0x64));
        func_001CA4C0(self, *(void**)(self + 0x68));
        {
            char* o = *(char**)(self + 0x74);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6788);
            }
        }
        {
            char* o = *(char**)(self + 0x78);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6788);
            }
        }
        {
            char* o = *(char**)(self + 0x7C);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6788);
            }
        }
        {
            char* o = *(char**)(self + 0x80);
            if (o != 0) {
                sVEp_C66E8* vt = *(sVEp_C66E8**)(o + 8);
                vt[11].fn(o + vt[11].delta, &D_004C6788);
            }
        }
        *(sQuad_58E8*)(self + 0x32C) = D_004C67A8;
        *(sQuad_58E8*)(self + 0x33C) = D_004C6788;
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_initScreenPopup);
#ifdef SKIP_ASM
struct cUIText;
struct sVec3_6B08 {
    float x, y, z;
    sVec3_6B08() {}
    sVec3_6B08(float a, float b, float c) : x(a), y(b), z(c) {}
};
extern "C" void cScreenPopup_FillObjectPointers(cScreenPopup* self);
extern "C" sVec3_6B08* func_001C58D0(void* info, int i);
extern "C" void cScreenPopup_createTitleBar(cScreenPopup* self, void* a1);
extern "C" void cScreenPopup_setPopupMessage(cScreenPopup* self, void* a1);
extern "C" void cScreenPopup_FillOptions(cScreenPopup* self, void* a1);
extern "C" void cScreenPopup_adjustMenuPosition(cScreenPopup* self);
extern "C" void cScreenPopup_adjustButtonPosition(cScreenPopup* self);
extern "C" void cScreenPopup_handlePostPending(cScreenPopup* self, void* a1);
extern "C" void func_001C9CD0(void* self, int v);
extern "C" void func_001C66E8(void* self, int v);
extern "C" void func_001C7C20(void* self);
extern "C" void cScreenPopup_FillTextEntryBox(cScreenPopup* self);
extern "C" void cScreenPopup_checkAndFixTextEntry(cScreenPopup* self, cUIText* text, const char* str);

/**
 * Builds the popup from a popup description: copies its settings and lays out the title, text and options.
 *
 * Copies the misc data, pass-update, receive-previous, type, centring flags, minimum width, confirmations,
 * offset, layer offset, background type, default option and maximum characters from the info. The start width
 * is mfMinSize, or 472 (300 when the text is centred). Then it places the title (cScreenPopup_createTitleBar), message,
 * options, buttons (cScreenPopup_adjustMenuPosition, cScreenPopup_adjustButtonPosition) and post text (cScreenPopup_handlePostPending), shifts the layers, sets the
 * background (func_001C66E8) and its layout (func_001C7C20); a text-entry popup (type -1) also sizes the
 * entry box (cScreenPopup_FillTextEntryBox) and fits its initial text with cScreenPopup_checkAndFixTextEntry().
 *
 * @param info  the popup description (On Tour: cPopupInfo *pInfo); the popup's own copy at +0xBC in practice
 */
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, void* info)
{
    cScreenPopup_FillObjectPointers(self);
    self->miMiscData = *(int*)((char*)info + 0x94);
    self->mbPassUpdate = *(int*)((char*)info + 0x98);
    self->mbReceivePrev = *(int*)((char*)info + 0xA8);
    self->meType = *(int*)((char*)info + 0x0);
    self->mbCenterOptions = *(int*)((char*)info + 0xB8);
    self->mbCenterText = *(int*)((char*)info + 0xBC);
    float t = *(float*)((char*)info + 0xB0);
    self->mfMinSize = t;
    if (t != 0.0f)
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(t, 0.0f, 0.0f);
    else if (self->mbCenterText == 0)
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(472.0f, 0.0f, 0.0f);
    else
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(300.0f, 0.0f, 0.0f);
    for (int i = 0; i < 5; i++)
        ((sVec3_6B08*)((char*)self + 0x2D0))[i] = *func_001C58D0(info, i);
    sVec3_6B08 c = *(sVec3_6B08*)((char*)info + 0x9C);
    *(sVec3_6B08*)&self->mvOffset[0] = c;
    cScreenPopup_createTitleBar(self, info);
    cScreenPopup_setPopupMessage(self, info);
    cScreenPopup_FillOptions(self, info);
    cScreenPopup_adjustMenuPosition(self);
    cScreenPopup_adjustButtonPosition(self);
    cScreenPopup_handlePostPending(self, info);
    int v = *(int*)((char*)info + 0x90);
    self->miLayerOffset = v;
    if (v != 0)
        func_001C9CD0(self, v);
    int m = *(int*)((char*)info + 0xC0);
    self->meBackgroundType = m;
    func_001C66E8(self, m);
    self->miDefaultOption = *(int*)((char*)info + 0xAC);
    self->miMaxChars = *(int*)((char*)info + 0xC4);
    func_001C7C20(self);
    if (*(int*)((char*)info + 0x0) == -1) {
        cScreenPopup_FillTextEntryBox(self);
        cScreenPopup_checkAndFixTextEntry(self, self->mpEnterBox, (const char*)self->mszInputText);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_handleEntryBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
struct sVec3_6D30 { float x, y, z; };
class cUIObj_6D30 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};
extern sVec3_6D30 D_004FF0D8_6D30 __asm__("t3vect_Zero");
extern "C" void cScreenPopup_setTextString1(cScreenPopup* self, void* dst, int* info);
extern "C" void func_001C8050(void* self, cUIText* text, int* info);
extern "C" void func_001C8628(void* self, void* obj, void* out);
extern "C" void func_001C8320(void* self, cUIText* text, int* info, float* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);

/**
 * Shows or hides the popup's text-entry widgets, depending on whether the popup is a text-entry popup.
 *
 * For a text-entry popup (type -1) it sets the static label from mtEntryBoxTitle, copies mtEntryBoxInitial
 * into mszInputText, places the entry box 10 units right of the label, grows mvCurSize, shows the label,
 * entry box, text-box pair and background, and changes the info's type to 1 (the text-box pair is the single
 * option). Otherwise it hides those four widgets, yet still adds the entry box's size to mvCurSize.
 *
 * @param infop  the popup description (cPopupInfo); its type is overwritten for a text-entry popup
 * @return 1 for a text-entry popup (the caller restores type -1 afterwards), else 0
 */
extern "C" int cScreenPopup_handleEntryBox(cScreenPopup* self, void* infop)
{
    int* info = (int*)infop;
    cScreenPopup* p = self;
    float size[4];
    if (info[0] == -1) {
        sVec3_6D30 pos = D_004FF0D8_6D30;
        float w = 0.0f;
        if (((cUIObj_6D30*)p->mpStaticBox) != 0) {
            float item[4];
            int* it = info + 5;
            func_001C8050(self, (cUIText*)((cUIObj_6D30*)p->mpStaticBox), it);
            func_001C8628(self, ((cUIObj_6D30*)p->mpStaticBox), size);
            func_001C8320(self, (cUIText*)((cUIObj_6D30*)p->mpStaticBox), it, item);
            w = item[2];
            pos = *(sVec3_6D30*)((char*)((cUIObj_6D30*)p->mpStaticBox) + 0x44);
            ((cUIObj_6D30*)p->mpStaticBox)->setEnabled(0);
            ((cUIObj_6D30*)p->mpStaticBox)->setVisible(1);
        }
        if (((cUIObj_6D30*)p->mpEnterBox) != 0) {
            cScreenPopup_setTextString1(self, (char*)self + 0x2B0, info + 7);
            sVec3_6D30 v = pos;
            v.x = v.x + w + 10.0f;
            *(sVec3_6D30*)((char*)((cUIObj_6D30*)p->mpEnterBox) + 0x44) = v;
            func_001C9B68(self, (float*)((char*)self + 0x260), size);
            ((cUIObj_6D30*)p->mpEnterBox)->setEnabled(0);
            ((cUIObj_6D30*)p->mpEnterBox)->setVisible(1);
        }
        if (((cUIObj_6D30*)p->mpTextBoxPair) != 0) {
            ((cUIObj_6D30*)p->mpTextBoxPair)->setEnabled(0);
            ((cUIObj_6D30*)p->mpTextBoxPair)->setVisible(1);
        }
        if (((cUIObj_6D30*)p->mpEnterBoxBackground) != 0) {
            ((cUIObj_6D30*)p->mpEnterBoxBackground)->setVisible(1);
        }
        info[0] = 1;
        return 1;
    }
    if (((cUIObj_6D30*)p->mpTextBoxPair) != 0) {
        ((cUIObj_6D30*)p->mpTextBoxPair)->setEnabled(1);
        ((cUIObj_6D30*)p->mpTextBoxPair)->setVisible(0);
    }
    if (((cUIObj_6D30*)p->mpStaticBox) != 0) {
        ((cUIObj_6D30*)p->mpStaticBox)->setEnabled(1);
        ((cUIObj_6D30*)p->mpStaticBox)->setVisible(0);
    }
    if (((cUIObj_6D30*)p->mpEnterBox) != 0) {
        ((cUIObj_6D30*)p->mpEnterBox)->setEnabled(1);
        ((cUIObj_6D30*)p->mpEnterBox)->setVisible(0);
    }
    if (((cUIObj_6D30*)p->mpEnterBoxBackground) != 0) {
        ((cUIObj_6D30*)p->mpEnterBoxBackground)->setVisible(0);
    }
    func_001C8628(self, ((cUIObj_6D30*)p->mpEnterBox), size);
    func_001C9B68(self, (float*)((char*)self + 0x260), size);
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_handlePostPending);
#ifdef SKIP_ASM
struct sVec3_7040 { float x, y, z; };
extern "C" void cScreenPopup_getTextBoxWrapSize(cScreenPopup* self, void* a, void* b, int which, float t);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void cScreenPopup_handlePostPending(cScreenPopup* self, void* a1)
{
    char* o = (char*)self->mpPostText;
    if (o != 0) {
        if (*(int*)((char*)a1 + 0x4C) != 0) {
            sVec3_7040 v = *(sVec3_7040*)&self->mvPosTitle[0];
            v.y += self->mvCurSize[1];
            *(sVec3_7040*)(o + 0x44) = v;
            cScreenPopup_getTextBoxWrapSize(self, (char*)a1 + 0x4C, (char*)self + 0x260, 0, 10.0f);
            if (self->mbCenterText != 0) {
                func_001C8930(self, self->mpPostText, 0x11, 0.0f);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextObjectSize);
#ifdef SKIP_ASM
struct cW_70F0 {
    int f0, f4;
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
    virtual void get(void* out);
    virtual void set(void* in);
};
struct sVec3_70F0 { float x, y, z; };
struct sVec3_7258;

extern "C" void cScreenPopup_setTextObjectSize(cScreenPopup* self, sVec3_7258* vp)
{
    sVec3_70F0* v = (sVec3_70F0*)vp;
    cW_70F0* o = (cW_70F0*)self->mpMessageText;
    if (o != 0) {
        sVec3_70F0 t;
        o->get(&t);
        if (v->x != 0.0f) t.x = v->x;
        if (v->y != 0.0f) t.y = v->y;
        ((cW_70F0*)self->mpMessageText)->set(v);
    }
    o = (cW_70F0*)self->mpPostText;
    if (o != 0) {
        sVec3_70F0 t;
        o->get(&t);
        if (v->x != 0.0f) t.x = v->x;
        if (v->y != 0.0f) t.y = v->y;
        ((cW_70F0*)self->mpPostText)->set(&t);
    }
    o = (cW_70F0*)self->mpTitleText;
    if (o != 0) {
        sVec3_70F0 t;
        o->get(&t);
        if (v->x != 0.0f) t.x = v->x;
        if (v->y != 0.0f) t.y = v->y;
        ((cW_70F0*)self->mpTitleText)->set(v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_setPopupMessage);
#ifdef SKIP_ASM
struct sVec3_7258 { float x, y, z; };
extern sVec3_7258 t3vect_Zero;
extern "C" void cScreenPopup_setTextObjectSize(cScreenPopup* self, sVec3_7258* v);
extern "C" void cScreenPopup_getMessageLineWrapSize1(cScreenPopup* self, int* msg, void* out);
extern "C" void cScreenPopup_getTextBoxWrapSize(cScreenPopup* self, void* a, void* b, int which, float t);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

/**
 * Places and fills the popup's message text below the title and records the size reached.
 *
 * The message goes at mvPosTitle shifted down by the height so far. With centred text it first measures the
 * message's first wrapped line (cScreenPopup_getMessageLineWrapSize1()) and widens the texts (cScreenPopup_setTextObjectSize)
 * when it is wider than the popup. cScreenPopup_getTextBoxWrapSize fills the text from the info's mtMessage and grows
 * mvCurSize; centred text is then realigned (func_001C8930). mvMenuSize gets the size reached.
 *
 * @param pInfo  the popup description (cPopupInfo)
 */
extern "C" void cScreenPopup_setPopupMessage(cScreenPopup* self, void* pInfo)
{
    char* o = (char*)self->mpMessageText;
    if (o != 0) {
        sVec3_7258 v = *(sVec3_7258*)&self->mvPosTitle[0];
        v.y += self->mvCurSize[1];
        *(sVec3_7258*)(o + 0x44) = v;
        char* p = (char*)pInfo + 4;
        if (self->mbCenterText != 0) {
            sVec3_7258 t = t3vect_Zero;
            cScreenPopup_getMessageLineWrapSize1(self, (int*)p, &t);
            if (t.x > self->mvCurSize[0])
                cScreenPopup_setTextObjectSize(self, &t);
        }
        cScreenPopup_getTextBoxWrapSize(self, p, (char*)self + 0x260, 1, 10.0f);
        if (self->mbCenterText != 0)
            func_001C8930(self, self->mpMessageText, 0x11, 0.0f);
    }
    *(sVec3_7258*)&self->mvMenuSize[0] = *(sVec3_7258*)&self->mvCurSize[0];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_FillOptions);
#ifdef SKIP_ASM
struct cUIText;
class cUIObj_7388 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};
struct sPopup_7388 {
    char pad[0x90];
    cUIObj_7388* items[5];
};
extern "C" int cScreenPopup_handleEntryBox(cScreenPopup* self, void* info);
extern "C" int* func_001C5800(void* info, int i);
extern "C" void func_001C8050(void* self, cUIText* text, int* info);
extern "C" void func_001C8628(void* self, void* obj, void* out);
extern "C" void func_001C8320(void* self, cUIText* text, int* info, float* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);

/**
 * Fills the popup's option texts from its description and hides the unused ones.
 *
 * Sets bit 3 of the menu's flags (+0x90) for types 0 and 1 and clears it otherwise, runs
 * cScreenPopup_handleEntryBox(), shows the button group unless the type is 0, hides options from index type
 * upward and fills the first type options from the info's option names, keeping the widest in mvLargestOption
 * and growing mvCurSize. Adds 10 to the height and restores type -1 for a text-entry popup.
 *
 * @param infop  the popup description (cPopupInfo); meType is the number of options, -1 for text entry
 */
extern "C" void cScreenPopup_FillOptions(cScreenPopup* self, void* infop)
{
    int* info = (int*)infop;
    switch (info[0]) {
    case 0:
        *(int*)((char*)self->mpMenu + 0x90) |= 8;
        break;
    case 1:
        *(int*)((char*)self->mpMenu + 0x90) |= 8;
        break;
    default:
        *(int*)((char*)self->mpMenu + 0x90) &= ~8;
        break;
    }
    *(int*)&self->mvButtonOffset[0] = 0;
    *(int*)&self->mvLargestOption[0] = 0;
    int r = cScreenPopup_handleEntryBox(self, info);
    if (info[0] == 0)
        ((cUIObj_7388*)self->mpButtonGroup)->setVisible(0);
    else
        ((cUIObj_7388*)self->mpButtonGroup)->setVisible(1);
    for (int i = info[0]; i < 5; i++) {
        if (((sPopup_7388*)self)->items[i] != 0) {
            ((sPopup_7388*)self)->items[i]->setEnabled(1);
            ((sPopup_7388*)self)->items[i]->setVisible(0);
        }
    }
    for (int j = 0; j < info[0]; j++) {
        cUIObj_7388* t = ((sPopup_7388*)self)->items[j];
        if (t != 0) {
            float size[4];
            float item[4];
            int* it = func_001C5800(info, j);
            func_001C8050(self, (cUIText*)t, it);
            func_001C8628(self, t, size);
            func_001C8320(self, (cUIText*)t, it, item);
            if (self->mvLargestOption[0] < item[2])
                self->mvLargestOption[0] = item[2];
            func_001C9B68(self, (float*)((char*)self + 0x260), size);
            t->setEnabled(0);
            t->setVisible(1);
        }
    }
    self->mvCurSize[1] += 10.0f;
    if (r != 0)
        info[0] = -1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setPrivateDefaultOption);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

/**
 * Selects the menu entry one past an option index, clamped to the valid range.
 *
 * Selects item @p idx + 1, clamped to 1..@p max; when @p max is -1 the range is 0..1 instead.
 *
 * @param menu  menu whose selection is set
 * @param idx   option index; the item selected is @p idx + 1
 * @param max   highest selectable item, or -1 for the range 0..1
 */
extern "C" void cScreenPopup_setPrivateDefaultOption(cScreenPopup* self, void* menu, int idx, int max)
{
    int lo;
    idx = idx + 1;
    if (max == -1) {
        lo = 0;
        max = 1;
    } else {
        lo = 1;
    }
    if (idx < lo) {
        idx = lo;
    } else if (idx > max) {
        idx = max;
    }
    cUIMenu_setSelectedByIndex(menu, idx);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C7620);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// EA's 3-float vector (name from the July 2003 prototype, e.g. setOffset__10cPopupInfoG8t3Vector).
struct t3Vector {
    float x, y, z;
};

extern "C" double fptodp(float f);

extern "C" float dptofp(double d);

// PORT: SN soft-float helpers called by name (fptodp / dpmul / dptofp); on PC: (float)((double)(...) * 1.05)
extern "C" double dpmul(double a, double b);

// Converts a screen position into the popup's normalised scroll position for the current layout.
extern "C" void func_001C7620(void* self, t3Vector pos)
{
    char* s = (char*)self;
    switch (*(int*)(s + 0x31C)) {
    case 0:
        *(float*)(s + 0x278) = (pos.x + 15.0f) / *(float*)(s + 0x1F4);
        *(float*)(s + 0x27C) = (pos.y + 10.0f) / *(float*)(s + 0x1F8);
        break;
    case 1:
        *(float*)(s + 0x278) = dptofp(dpmul(fptodp((pos.x + 20.0f) / *(float*)(s + 0x218)), 1.05));
        *(float*)(s + 0x27C) = (pos.y + 15.0f) / *(float*)(s + 0x21C);
        break;
    case 2:
        *(float*)(s + 0x278) = 1.0f;
        *(float*)(s + 0x27C) = 1.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_scaleBackground);
#ifdef SKIP_ASM
struct sColor_7738 { int r, g, b; };

/**
 * Applies a scale to the background shapes of the popup's current background type.
 *
 * Type 0 (online) scales the white and red backgrounds, type 1 (blue) the small and large shapes, their
 * outlines and the shadow, type 2 (black) the black background; the vector is stored at +0x50 (the widget's
 * scale) of each existing shape.
 *
 * @param color  the scale vector (On Tour: t3Vector vScale), despite the parameter's type name
 */
extern "C" void cScreenPopup_scaleBackground(cScreenPopup* self, sColor_7738* color)
{
    sColor_7738 c = *color;
    switch (self->meBackgroundType) {
    case 0:
        {
            void* p = self->mpBackGroundWhite;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = self->mpBackGroundRed;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 1:
        {
            void* p = self->mpBackGroundSmall;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = self->mpBackGroundSmallOutline;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = self->mpBackGroundLarge;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = self->mpBackGroundLargeOutline;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = self->mpBackGroundShadow;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 2:
        {
            void* p = self->mpBackGroundBlack;
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C78C0);
#ifdef SKIP_ASM
struct sVec3_78C0 { float x, y, z; };
// PORT: SN soft-float helpers called by name (fptodp / dpsub / dptofp); on PC: y = (float)((double)(...) - 7.5)
extern "C" double fptodp(float f);
extern "C" double dpsub(double a, double b);
extern "C" float dptofp(double d);

extern "C" void func_001C78C0(void* self, sVec3_78C0 v, float a, float b)
{
    char* s = (char*)self;
    float y = 0.0f;
    switch (*(int*)(s + 0x31C)) {
    case 0:
        if (*(char**)(s + 0x48) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x188);
            y = *(float*)(s + 0x1BC) + b + v.y * 0.5f - 5.0f;
            p.x += a;
            p.y = y;
            *(sVec3_78C0*)(*(char**)(s + 0x48) + 0x44) = p;
        }
        if (*(char**)(s + 0x4C) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x194);
            p.y = y;
            p.x += a;
            *(sVec3_78C0*)(*(char**)(s + 0x4C) + 0x44) = p;
        }
        break;
    case 1:
        if (*(char**)(s + 0x58) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x23C);
            y = dptofp(dpsub(fptodp(*(float*)(s + 0x1BC) + b + v.y * 0.5f), 7.5));
            p.x += a;
            p.y = y;
            *(sVec3_78C0*)(*(char**)(s + 0x58) + 0x44) = p;
        }
        if (*(char**)(s + 0x5C) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x248);
            p.y = y;
            p.x += a;
            *(sVec3_78C0*)(*(char**)(s + 0x5C) + 0x44) = p;
        }
        if (*(char**)(s + 0x50) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x224);
            p.y = y;
            p.x += a;
            *(sVec3_78C0*)(*(char**)(s + 0x50) + 0x44) = p;
        }
        if (*(char**)(s + 0x54) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x230);
            p.y = y;
            p.x += a;
            *(sVec3_78C0*)(*(char**)(s + 0x54) + 0x44) = p;
        }
        if (*(char**)(s + 0x60) != 0) {
            sVec3_78C0 p = *(sVec3_78C0*)(s + 0x254);
            p.y = y;
            p.x += a;
            *(sVec3_78C0*)(*(char**)(s + 0x60) + 0x44) = p;
        }
        break;
    case 2:
        *(float*)(s + 0x27C) = *(float*)(s + 0x278) = 1.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_centerOffset);
#ifdef SKIP_ASM
extern "C" float cScreenPopup_centerOffset(cScreenPopup* self)
{
    if (self->mbCenterText != 0) {
        return 0.0f;
    }
    return (640.0f - self->mvCurSize[0]) * 0.5f - self->mvPosTitle[0];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001C7C20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec3_7C20 { float x, y, z; };
extern "C" float cScreenPopup_centerOffset(cScreenPopup* self);
void func_001C7620_7C20(void* self, sVec3_7C20 v) __asm__("func_001C7620");
void func_001C78C0_7C20(void* self, sVec3_7C20 v, float a, float b) __asm__("func_001C78C0");

extern "C" void func_001C7C20(void* self)
{
    char* s = (char*)self;
    float x = cScreenPopup_centerOffset((cScreenPopup*)self) + *(float*)(s + 0x26C);
    float y = *(float*)(s + 0x270);
    func_001C7620_7C20(self, *(sVec3_7C20*)(s + 0x260));
    func_001C78C0_7C20(self, *(sVec3_7C20*)(s + 0x260), *(float*)(s + 0x26C), y);
    char* p = *(char**)(s + 0x6C);
    if (p != 0) {
        sVec3_7C20 v = *(sVec3_7C20*)(p + 0x44);
        v.x += *(float*)(s + 0x26C);
        v.y += y;
        *(sVec3_7C20*)(p + 0x44) = v;
    }
    p = *(char**)(s + 0x70);
    if (p != 0) {
        sVec3_7C20 v = *(sVec3_7C20*)(p + 0x44);
        v.x += *(float*)(s + 0x26C);
        v.y += y;
        *(sVec3_7C20*)(p + 0x44) = v;
    }
    if (*(char**)(s + 0x74) != 0) {
        sVec3_7C20 v = *(sVec3_7C20*)(*(char**)(s + 0x74) + 0x44);
        if (*(int*)(s + 0x2CC) != 0) {
            v.x = x + 320.0f;
        } else {
            v.x += x;
        }
        v.y += y;
        *(sVec3_7C20*)(*(char**)(s + 0x74) + 0x44) = v;
    }
    if (*(char**)(s + 0x7C) != 0) {
        sVec3_7C20 v = *(sVec3_7C20*)(*(char**)(s + 0x7C) + 0x44);
        if (*(int*)(s + 0x2CC) != 0) {
            v.x = x + 320.0f;
        } else {
            v.x += x;
        }
        v.y += y;
        *(sVec3_7C20*)(*(char**)(s + 0x7C) + 0x44) = v;
    }
    if (*(char**)(s + 0x78) != 0) {
        sVec3_7C20 v = *(sVec3_7C20*)(*(char**)(s + 0x78) + 0x44);
        if (*(int*)(s + 0x2CC) != 0) {
            v.x = x + 320.0f;
        } else {
            v.x += x;
        }
        v.y += y;
        *(sVec3_7C20*)(*(char**)(s + 0x78) + 0x44) = v;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_FillTextEntryBox);
#ifdef SKIP_ASM
struct sVec3_7EB0 { float x, y, z; };
struct sVec2_7EB0 { float x, y; };
struct sVE_7EB0 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_7EB0*);
};

extern "C" void cScreenPopup_FillTextEntryBox(cScreenPopup* self)
{
    char* o80 = (char*)self->mpEnterBox;
    sVec3_7EB0 a = *(sVec3_7EB0*)(o80 + 0x44);
    sVec3_7EB0 c;
    sVec3_7EB0 b = *(sVec3_7EB0*)((char*)self->mpMenu + 0x44);
    c = *(sVec3_7EB0*)((char*)self->mpStaticBox + 0x44);
    sVec2_7EB0 size;
    {
        sVE_7EB0* vt = *(sVE_7EB0**)(o80 + 8);
        vt[20].fn(o80 + vt[20].delta, &size);
    }
    size.x = self->mvCurSize[0] - (a.x - c.x) - 10.0f;
    {
        char* o = (char*)self->mpEnterBox;
        sVE_7EB0* vt = *(sVE_7EB0**)(o + 8);
        vt[21].fn(o + vt[21].delta, &size);
    }
    sVec3_7EB0 scale;
    scale.x = (size.x + 6.0f) / self->mvSizeBackEnterBox[0];
    scale.y = 1.0f;
    scale.z = 1.0f;
    char* p = (char*)self->mpEnterBoxBackground;
    if (p != 0) {
        sVec3_7EB0 d = a;
        d.y = d.y + self->mvSizeBackEnterBox[1] * 0.5f + b.y - 3.0f;
        d.x = d.x + (size.x + 6.0f) * 0.5f;
        *(sVec3_7EB0*)(p + 0x44) = d;
        *(sVec3_7EB0*)((char*)self->mpEnterBoxBackground + 0x50) = scale;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8050);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern char D_004A1408[];

extern "C" void func_001C8050(void* self, cUIText* text, int* info)
{
    switch (info[0]) {
    case 1:
        cUIText_setUnicodeString(text, (void*)info[1]);
        break;
    case 2:
        cUIText_setAsciiString(text, (const char*)info[1]);
        break;
    case 3:
        cUIText_setUnicodeStringByID(text, info[1]);
        break;
    case 0:
        cUIText_setAsciiString(text, D_004A1408);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextString1);
#ifdef SKIP_ASM
extern "C" int USTR_length(unsigned short* s);
extern "C" char* strcpy2(char* dst, unsigned short* src);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_00466098[];
extern char D_004660A8[];
extern char D_004A1408[];

struct sVEntry_80E8 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

/**
 * Sets a string object to the text of a popup element (unicode, ASCII or a localised text id).
 *
 * Unicode text is narrowed to ASCII through a temporary buffer (strcpy2); an id is looked up in the front-end
 * locale (slot 4) and narrowed the same way, or printed as "String %08x" when it has no text; an empty
 * element gives an empty string.
 *
 * @param dst  the cBXString to set (On Tour: cBXString &bxString)
 * @param info  the popup element (On Tour: const tPopupElement *ptElement): type, then the string or id
 */
extern "C" void cScreenPopup_setTextString1(cScreenPopup* self, void* dst, int* info)
{
    switch (info[0]) {
    case 1: {
        unsigned short* u = (unsigned short*)info[1];
        char* buf = (char*)operator_new_tag(USTR_length(u) + 1, D_00466098, 0x100, 0);
        strcpy2(buf, (unsigned short*)info[1]);
        cBXString_cBXString4(dst, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
        break;
    }
    case 2:
        cBXString_cBXString4(dst, (const char*)info[1]);
        break;
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry_80E8* vt = *(sVEntry_80E8**)(o + 4);
        unsigned short* str = vt[4].fn(o + vt[4].delta, info[1]);
        if (str) {
            char* buf = (char*)operator_new_tag(USTR_length(str) + 1, D_00466098, 0x100, 0);
            strcpy2(buf, str);
            cBXString_cBXString4(dst, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
        } else {
            char tmp[0x40];
            sprintf(tmp, D_004660A8, info[1]);
            cBXString_cBXString4(dst, tmp);
        }
        break;
    }
    case 0:
        cBXString_cBXString4(dst, D_004A1408);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextString);
#ifdef SKIP_ASM
struct cUIText;
extern "C" char* cUIText_getAsciiString(cUIText* text);
extern "C" unsigned short* cUIText_getUnicodeString(cUIText* text);
extern "C" int USTR_length(unsigned short* s);
extern "C" char* strcpy2(char* dst, unsigned short* src);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004660B8[];
extern char D_004A1408[];

/**
 * Copies a text widget's string into a cBXString as ASCII.
 *
 * Uses the widget's ASCII string when it has one. Otherwise narrows its unicode string with strcpy2() through a
 * temporary heap buffer, or stores the empty string when the widget has neither.
 *
 * @param dst   cBXString that receives the text
 * @param text  text widget to read
 */
extern "C" void cScreenPopup_setTextString(cScreenPopup* self, void* dst, cUIText* text)
{
    char* s = cUIText_getAsciiString(text);
    if (s != 0) {
        cBXString_cBXString4(dst, s);
        return;
    }
    unsigned short* u = cUIText_getUnicodeString(text);
    if (u != 0) {
        char* buf = (char*)operator_new_tag(USTR_length(u) + 1, D_004660B8, 0x100, 0);
        strcpy2(buf, u);
        cBXString_cBXString4(dst, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else {
        cBXString_cBXString4(dst, D_004A1408);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8320);
#ifdef SKIP_ASM
struct cUIText;
struct sVec3_8320 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" float func_003921F0(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004660A8[];

struct sVEntry_8320 {
    short delta;
    short index;
    char* (*fn)(void*, int);
};

extern "C" void func_001C8320(void* self, cUIText* text, int* info, float* out)
{
    sVec3_8320 v = *(sVec3_8320*)((char*)text + 0x50);
    char* g = (char*)cUIText_getFont(text);
    float sx = *(float*)(g + 0x30) * v.x;
    float sy = *(float*)(g + 0x34) * v.y;
    *(float*)(g + 0x38) = sx;
    *(float*)(g + 0x3C) = sy;
    char* f;
    switch (info[0]) {
    case 1:
        f = (char*)cUIText_getFont(text);
        func_003921F0(f, (const char*)info[1], out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        break;
    case 2:
        f = (char*)cUIText_getFont(text);
        cFont_textExtent(f, (const char*)info[1], out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        break;
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry_8320* vt = *(sVEntry_8320**)(o + 4);
        char* str = vt[4].fn(o + vt[4].delta, info[1]);
        if (str) {
            f = (char*)cUIText_getFont(text);
            func_003921F0(f, str, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        } else {
            char buf[0x40];
            sprintf(buf, D_004660A8, info[1]);
            f = (char*)cUIText_getFont(text);
            cFont_textExtent(f, buf, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        }
        break;
    }
    case 0:
        out[2] = 0.0f;
        out[3] = 0.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C84A8);
#ifdef SKIP_ASM
struct sVec3_84A8 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_001C84A8(void* self, void* text, const char* str, void* out)
{
    sVec3_84A8 v = *(sVec3_84A8*)((char*)text + 0x50);
    char* f = (char*)cUIText_getFont(text);
    float sx = *(float*)(f + 0x30) * v.x;
    float sy = *(float*)(f + 0x34) * v.y;
    *(float*)(f + 0x38) = sx;
    *(float*)(f + 0x3C) = sy;
    f = (char*)cUIText_getFont(text);
    cFont_textExtent(f, str, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
    f = (char*)cUIText_getFont(text);
    *(float*)((char*)out + 0xC) = v.y * ((float)*(int*)(f + 0x14) * *(float*)(f + 0x34));
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8568);
#ifdef SKIP_ASM
struct sVec3_8568 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_003921F0(void* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_001C8568(void* self, void* text, const char* str, void* out)
{
    sVec3_8568 v = *(sVec3_8568*)((char*)text + 0x50);
    char* f = (char*)cUIText_getFont(text);
    float sx = *(float*)(f + 0x30) * v.x;
    float sy = *(float*)(f + 0x34) * v.y;
    *(float*)(f + 0x38) = sx;
    *(float*)(f + 0x3C) = sy;
    f = (char*)cUIText_getFont(text);
    func_003921F0(f, str, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
    f = (char*)cUIText_getFont(text);
    *(float*)((char*)out + 0xC) = v.y * ((float)*(int*)(f + 0x14) * *(float*)(f + 0x34));
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8628);
#ifdef SKIP_ASM
struct sVec2_8628 { float x, y; };

struct sVEntry001C8628 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_8628*);
};

extern "C" void func_001C8628(void* self, void* obj, void* out)
{
    sVec2_8628 v;
    sVEntry001C8628* vt = *(sVEntry001C8628**)((char*)obj + 8);
    vt[20].fn((char*)obj + vt[20].delta, &v);
    *(float*)((char*)out + 0x8) = v.x;
    *(float*)((char*)out + 0xC) = v.y;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_createTitleBar);
#ifdef SKIP_ASM
struct cUIText;
struct sVE8678 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern "C" void func_001C8050(void* self, cUIText* text, int* info);
extern "C" void func_001C8320(void* self, cUIText* text, int* info, float* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void cScreenPopup_createTitleBar(cScreenPopup* self, void* a1)
{
    cUIText* text = self->mpTitleText;
    if (text != 0) {
        int* info = (int*)((char*)a1 + 0xC);
        if (*(int*)((char*)a1 + 0xC) != 0) {
            float item[4];
            func_001C8050(self, text, info);
            {
                char* o = (char*)self->mpTitleText;
                sVE8678* vt = *(sVE8678**)(o + 8);
                vt[9].fn(o + vt[9].delta, 1);
            }
            func_001C8320(self, self->mpTitleText, info, item);
            func_001C9B68(self, (float*)((char*)self + 0x260), item);
            self->mvCurSize[1] += 5.0f;
            if (self->mbCenterText != 0) {
                func_001C8930(self, self->mpTitleText, 0x11, 0.0f);
            }
        } else {
            sVE8678* vt = *(sVE8678**)((char*)text + 8);
            vt[9].fn((char*)text + vt[9].delta, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_adjustMenuPosition);
#ifdef SKIP_ASM
struct sVec3_8758 { float x, y, z; };
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);

extern "C" void cScreenPopup_adjustMenuPosition(cScreenPopup* self)
{
    char* p = (char*)self->mpMenu;
    if (p != 0) {
        sVec3_8758 a = *(sVec3_8758*)((char*)self->mpOptions[0] + 0x44);
        float x = (640.0f - self->mvLargestOption[0]) * 0.5f - a.x;
        sVec3_8758 b = *(sVec3_8758*)&self->mvPosMenu[0];
        b.y += self->mvMenuSize[1];
        b.x += x;
        *(sVec3_8758*)(p + 0x44) = b;
        char* q = (char*)self->mpTextBoxPair;
        sVec3_8758 c = *(sVec3_8758*)(q + 0x44);
        c.x = c.x - x + 5.0f;
        *(sVec3_8758*)(q + 0x44) = c;
        if (self->mbCenterOptions != 0) {
            int i;
            for (i = 0; i < 5; i++) {
                void* o = *(void**)((char*)self + 0x90 + i * 4);
                if (o != 0)
                    func_001C8930(self, o, 0x12, self->mvLargestOption[0] * 0.5f);
            }
        }
        self->mvButtonOffset[0] = x;
        self->mvButtonOffset[1] = self->mvMenuSize[1];
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_adjustButtonPosition);
#ifdef SKIP_ASM
struct sVec3_88C8 { float x, y, z; };

extern "C" void cScreenPopup_adjustButtonPosition(cScreenPopup* self)
{
    void* p = self->mpButtonGroup;
    if (p != 0) {
        sVec3_88C8 v = *(sVec3_88C8*)&self->mvPosButtons[0];
        v.y += self->mvButtonOffset[1];
        v.x += self->mvButtonOffset[0];
        *(sVec3_88C8*)((char*)p + 0x44) = v;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8930);
#ifdef SKIP_ASM
struct sObjBitsK8930 {
    unsigned lo : 13;
    unsigned state : 6;
    unsigned hi : 13;
};
struct sVec2K8930 { float x, y; };
struct sVec3K8930 { float x, y, z; };

struct sVEntryK8930 {
    short delta;
    short index;
    void (*fn)(void*, sVec2K8930*);
};

extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t)
{
    sObjBitsK8930* b = (sObjBitsK8930*)((char*)obj + 0x14);
    if (b->state == state)
        return;
    sVec2K8930 size;
    sVEntryK8930* vt = *(sVEntryK8930**)((char*)obj + 8);
    vt[20].fn((char*)obj + vt[20].delta, &size);
    sVec3K8930 pos = *(sVec3K8930*)((char*)obj + 0x44);
    b->state = state;
    if (t == 0.0f)
        t = size.x * 0.5f;
    switch (state) {
    case 17:
    case 18:
        pos.x += t;
        break;
    case 9:
    case 10:
        pos.x -= t;
        break;
    }
    *(sVec3K8930*)((char*)obj + 0x44) = pos;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_restoreElements);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec3_8A38 {
    float x, y, z;
    sVec3_8A38() {}
    sVec3_8A38(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sVec3_7258;
extern sVec3_8A38 D_004FF0D8_8A38 __asm__("t3vect_Zero");
extern "C" void cScreenPopup_setTextObjectSize(cScreenPopup* self, sVec3_7258* v);
extern "C" void func_001C8930(void* self, void* obj, unsigned char state, float t);
extern "C" void func_001C9CD0(void* self, int v);

#define OBJ_8A38(off) (*(char**)((char*)self + (off)))
#define VEC_8A38(p, off) (*(sVec3_8A38*)((char*)(p) + (off)))

/**
 * Puts the popup's widgets back to their unit scale and home positions before a new layout.
 *
 * Resets the background shapes' scale and position, moves the menu, buttons, texts, entry box and text-box
 * pair back to the positions cached by cScreenPopup_FillObjectPointers(), realigns the options
 * (func_001C8930), resets the text widths to 472 (cScreenPopup_setTextObjectSize) and mvCurrentScale to 1, and undoes the
 * layer offset (func_001C9CD0 with -miLayerOffset).
 *
 * The title and post-text branches realign mpMessageText instead of their own widgets (EA's code passes +0x74
 * three times), so only the message is realigned.
 */
// Resets the popup's widgets: unit scale and home positions, then re-anchors the side items.
extern "C" void cScreenPopup_restoreElements(cScreenPopup* self)
{
    if (OBJ_8A38(0x48) != 0) {
        VEC_8A38(OBJ_8A38(0x48), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x48), 0x44) = VEC_8A38(self, 0x188);
    }
    if (OBJ_8A38(0x4C) != 0) {
        VEC_8A38(OBJ_8A38(0x4C), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x4C), 0x44) = VEC_8A38(self, 0x194);
    }
    if (OBJ_8A38(0x50) != 0) {
        VEC_8A38(OBJ_8A38(0x50), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x50), 0x44) = VEC_8A38(self, 0x224);
    }
    if (OBJ_8A38(0x54) != 0) {
        VEC_8A38(OBJ_8A38(0x54), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x54), 0x44) = VEC_8A38(self, 0x230);
    }
    if (OBJ_8A38(0x58) != 0) {
        VEC_8A38(OBJ_8A38(0x58), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x58), 0x44) = VEC_8A38(self, 0x23C);
    }
    if (OBJ_8A38(0x5C) != 0) {
        VEC_8A38(OBJ_8A38(0x5C), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x5C), 0x44) = VEC_8A38(self, 0x248);
    }
    if (OBJ_8A38(0x60) != 0) {
        VEC_8A38(OBJ_8A38(0x60), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x60), 0x44) = VEC_8A38(self, 0x254);
    }
    if (OBJ_8A38(0x8C) != 0) {
        VEC_8A38(OBJ_8A38(0x8C), 0x50) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x8C), 0x44) = VEC_8A38(self, 0x200);
    }
    if (OBJ_8A38(0x6C) != 0) {
        VEC_8A38(OBJ_8A38(0x6C), 0x44) = VEC_8A38(self, 0x1A0);
    }
    if (OBJ_8A38(0x70) != 0) {
        VEC_8A38(OBJ_8A38(0x70), 0x44) = VEC_8A38(self, 0x1AC);
    }
    if (OBJ_8A38(0x74) != 0) {
        VEC_8A38(OBJ_8A38(0x74), 0x44) = VEC_8A38(self, 0x1C4);
        func_001C8930(self, OBJ_8A38(0x74), 9, 0.0f);
    }
    if (OBJ_8A38(0x7C) != 0) {
        VEC_8A38(OBJ_8A38(0x7C), 0x44) = VEC_8A38(self, 0x1B8);
        func_001C8930(self, OBJ_8A38(0x74), 9, 0.0f);
    }
    if (OBJ_8A38(0x78) != 0) {
        VEC_8A38(OBJ_8A38(0x78), 0x44) = VEC_8A38(self, 0x1D0);
        func_001C8930(self, OBJ_8A38(0x74), 9, 0.0f);
    }
    if (OBJ_8A38(0x80) != 0) {
        VEC_8A38(OBJ_8A38(0x80), 0x44) = VEC_8A38(self, 0x1DC);
    }
    if (OBJ_8A38(0x88) != 0) {
        VEC_8A38(OBJ_8A38(0x88), 0x44) = VEC_8A38(self, 0x1E8);
    }
    int i;
    for (i = 0; i < 5; i++) {
        void* o = *(void**)((char*)self + 0x90 + i * 4);
        if (o != 0) {
            func_001C8930(self, o, 10, self->mvLargestOption[0] * 0.5f);
        }
    }
    sVec3_8A38 t = D_004FF0D8_8A38;
    t.x = 472.0f;
    cScreenPopup_setTextObjectSize(self, (sVec3_7258*)&t);
    sVec3_8A38 one;
    one.x = 1.0f;
    one.y = 1.0f;
    one.z = 1.0f;
    VEC_8A38(self, 0x278) = one;
    func_001C9CD0(self, -self->miLayerOffset);
}

#undef OBJ_8A38
#undef VEC_8A38
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C8EF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_001C9038(void* self, void* text, unsigned short* str, float* size);
extern "C" void func_001C9210_s(void* self, void* text, const char* str, float* size) __asm__("func_001C9210");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004660A8[];

struct sVEntry_8EF0 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern "C" void func_001C8EF0(void* vself, int* item, float* size, int flag, float extra)
{
    char* self = (char*)vself;
    void* text;
    if (flag != 0)
        text = *(void**)(self + 0x74);
    else
        text = *(void**)(self + 0x78);
    if (text == 0)
        return;
    switch (item[0]) {
    case 0:
        break;
    case 2:
        func_001C9210_s(self, text, (const char*)item[1], size);
        goto add;
    case 1:
        func_001C9038(self, text, (unsigned short*)item[1], size);
        goto add;
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry_8EF0* vt = *(sVEntry_8EF0**)(o + 4);
        unsigned short* str = vt[4].fn(o + vt[4].delta, item[1]);
        if (str) {
            func_001C9038(self, text, str, size);
            goto add;
        } else {
            char buf[0x40];
            sprintf(buf, D_004660A8, item[1]);
            func_001C9210_s(self, text, buf, size);
        }
        break;
    }
    default:
    add:
        size[1] += extra;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9038);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_001C8568(void* self, void* text, const char* str, void* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);
extern char D_004660D8[];
extern char D_004A1C38;

extern "C" void func_001C9038(void* self, void* text, unsigned short* str, float* size)
{
    int j = 0;
    int i = 0;
    float item[4];
    unsigned short* buf = new (D_004660D8, 0x100, 0) unsigned short[USTR_length(str) + 1];
    buf[0] = 0;
    int n = USTR_length(str);
    while (i < n) {
        unsigned short c = str[i];
        unsigned short next = str[i + 1];
        if (c == D_004A1C38 && next == D_004A1C38) {
            buf[j] = 0;
            func_001C8568(self, text, (const char*)buf, item);
            i += 2;
            j = 0;
            func_001C9B68(self, size, item);
            size[1] += 3.0f;
        } else if (c == '\r') {
            buf[j] = 0;
            func_001C8568(self, text, (const char*)buf, item);
            i += 1;
            j = 0;
            func_001C9B68(self, size, item);
            size[1] += 3.0f;
        } else {
            buf[j] = c;
            i++;
            j++;
        }
    }
    if (j != 0) {
        buf[j] = 0;
        func_001C8568(self, text, (const char*)buf, item);
        func_001C9B68(self, size, item);
        size[1] += 3.0f;
    }
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9210);
#ifdef SKIP_ASM
struct cUIText;
extern "C" int strlen(const char* s);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_001C84A8(void* self, void* text, const char* str, void* out);
extern "C" void func_001C9B68(void* self, float* size, float* item);
extern char D_004660D8[];

// PORT: a3 is the float* size out pointer (the unit declares it int)
extern "C" void func_001C9210(void* self, cUIText* text, const char* str, int a3)
{
    float* size = (float*)a3;
    int j = 0;
    int i = 0;
    float item[4];
    char* buf = new (D_004660D8, 0x100, 0) char[strlen(str) + 1];
    buf[0] = 0;
    int n = strlen(str);
    while (i < n) {
        char c = str[i];
        char next = str[i + 1];
        if (c == '\\' && next == '\\') {
            buf[j] = 0;
            func_001C84A8(self, text, buf, item);
            i += 2;
            func_001C9B68(self, size, item);
            j = 0;
            size[1] += 3.0f;
        } else if (c == '\r') {
            buf[j] = 0;
            func_001C84A8(self, text, buf, item);
            i += 1;
            func_001C9B68(self, size, item);
            j = 0;
            size[1] += 3.0f;
        } else {
            buf[j] = c;
            i++;
            j++;
        }
    }
    if (j != 0) {
        buf[j] = 0;
        func_001C84A8(self, text, buf, item);
        func_001C9B68(self, size, item);
    }
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getMessageLineWrapSize1);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cScreenPopup_getMessageLineWrapSize(cScreenPopup* self, void* text, const char* str, void* out);
extern "C" void func_001C94B0(void* self, void* text, void* str, void* out);
extern char D_004660A8[];

struct sVEntryK93B0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

/**
 * Measures the first wrapped line of a popup message with the popup's message text object.
 *
 * Does nothing without mpMessageText. ASCII text goes to cScreenPopup_getMessageLineWrapSize(), unicode to
 * func_001C94B0; a text id is looked up in the front-end locale first, or measured as "String %08x" when it
 * has no text. An empty element leaves the size alone.
 *
 * @param msg  the message element (On Tour: tPopupElement *ptElement)
 * @param out  receives the width in x (On Tour: t3Vector &vSize)
 */
extern "C" void cScreenPopup_getMessageLineWrapSize1(cScreenPopup* self, int* msg, void* out)
{
    if (self->mpMessageText == 0)
        return;
    switch (msg[0]) {
    case 0:
        break;
    case 2:
        cScreenPopup_getMessageLineWrapSize(self, self->mpMessageText, (const char*)msg[1], out);
        break;
    case 1:
        func_001C94B0(self, self->mpMessageText, (void*)msg[1], out);
        break;
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntryK93B0* vt = *(sVEntryK93B0**)(o + 4);
        void* str = vt[4].fn(o + vt[4].delta, msg[1]);
        if (str) {
            func_001C94B0(self, self->mpMessageText, str, out);
        } else {
            char buf[0x40];
            sprintf(buf, D_004660A8, msg[1]);
            cScreenPopup_getMessageLineWrapSize(self, self->mpMessageText, buf, out);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C94B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec3_94B0 { float x, y, z; };
struct sVec2_94B0 { float x, y; };
struct sVEntry_94B0 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_94B0*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_00392C60(void* font, int c);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004660E8[];
extern char D_004A1C38;

extern "C" void func_001C94B0(void* self, void* text, void* strp, void* outp)
{
    unsigned short* str = (unsigned short*)strp;
    float* out = (float*)outp;
    *(int*)((char*)text + 0x14) |= 0x80;
    void* font = cUIText_getFont(text);
    if (font == 0)
        return;
    int i = 0;
    int n = USTR_length(str);
    float w = 0.0f;
    int done = 0;
    unsigned short* buf = new (D_004660E8, 0x100, 0) unsigned short[n + 1];
    USTR_copy(buf, str);
    sVec3_94B0 v;
    sVec2_94B0 size;
    sVEntry_94B0* vt = *(sVEntry_94B0**)((char*)text + 8);
    vt[20].fn((char*)text + vt[20].delta, &size);
    v = *(sVec3_94B0*)((char*)text + 0x50);
    while (i < n) {
        if (buf[i] == D_004A1C38 && buf[i + 1] == D_004A1C38) {
            w = 0.0f;
            i += 2;
        } else {
            float cw = func_00392C60(font, buf[i]) * v.x;
            w += cw;
            if (size.x < w) {
                done = 1;
                *out = w - cw;
                break;
            }
            i += 1;
        }
    }
    if (!done)
        *out = w;
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getMessageLineWrapSize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cScreenPopup::getMessageLineWrapSize(cUIText*, const char*, t3Vector&) (On Tour signature and locals).
struct t3Vector_9648 { float x, y, z; };
struct sVEntry_9648 {
    short delta;
    short index;
    void (*fn)(void*, t3Vector_9648*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_00392BA8(void* font, int c);  // cFont::charAdvance(char) const
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004660F8[];

/**
 * Measures the width of an ASCII string up to the point where it would wrap in a text object.
 *
 * Sets flag 0x80 on the text (+0x14), then adds up the font's character advances times the text's x scale on
 * a copy of the string. A double backslash or a carriage return starts a new line (width back to 0). When the
 * line gets wider than the text box (slot 20) the width before that character is stored; otherwise the last
 * line's width is. Returns early, without a size, when the text has no font.
 *
 * @param pText  the text object whose font, scale and box are used (cUIText)
 * @param pString  the ASCII string
 * @param pvSize  receives the width in x (a t3Vector)
 */
extern "C" void cScreenPopup_getMessageLineWrapSize(cScreenPopup* self, void* pText, const char* pString, void* pvSize)
{
    t3Vector_9648& vSize = *(t3Vector_9648*)pvSize;
    *(int*)((char*)pText + 0x14) |= 0x80;
    void* pFont = cUIText_getFont(pText);
    if (pFont == 0)
        return;
    int length = strlen(pString);
    char* pTempString = new (D_004660F8, 0x100, 0) char[length + 1];
    strcpy(pTempString, pString);
    int j = 0;
    float Pixels = 0.0f;
    bool bFound = false;
    t3Vector_9648 vScale;
    t3Vector_9648 vTextSize;
    sVEntry_9648* vt = *(sVEntry_9648**)((char*)pText + 8);
    vt[20].fn((char*)pText + vt[20].delta, &vTextSize);
    vScale = *(t3Vector_9648*)((char*)pText + 0x50);
    while (pTempString[j] != 0) {
        char char1 = pTempString[j];
        char char2 = pTempString[j + 1];
        if (char1 == '\\' && char2 == '\\') {
            Pixels = 0.0f;
            j += 2;
        } else if (char1 == '\r') {
            Pixels = 0.0f;
            j++;
        } else {
            float advance = func_00392BA8(pFont, pTempString[j]) * vScale.x;
            Pixels += advance;
            if (vTextSize.x < Pixels) {
                bFound = true;
                vSize.x = Pixels - advance;
                break;
            }
            j++;
        }
    }
    if (!bFound)
        vSize.x = Pixels;
    if (pTempString != 0)
        cMemMan_free(pTempString);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxWrapSize);
#ifdef SKIP_ASM
struct cUIText;
extern "C" void func_001C8050(void* self, cUIText* text, int* info);
extern "C" void func_001C9938(void* self, cUIText* text, void* str, float* out);
// PORT: func_001C9B28's 4th parameter is the out pointer (declared int in the unit)
extern "C" void func_001C9B28(void* self, cUIText* text, const char* str, int a3);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004660A8[];

struct sVEntry_97E0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void cScreenPopup_getTextBoxWrapSize(cScreenPopup* self, void* a, void* b, int which, float t)
{
    int* info = (int*)a;
    float* out = (float*)b;
    cUIText* text;
    if (which != 0)
        text = self->mpMessageText;
    else
        text = self->mpPostText;
    if (text == 0)
        return;
    switch (info[0]) {
    case 2:
        func_001C9B28(self, text, (const char*)info[1], (int)out);
        break;
    case 1:
        func_001C9938(self, text, (void*)info[1], out);
        break;
    case 3: {
        char* o = (char*)SSXApp->FELocale;
        sVEntry_97E0* vt = *(sVEntry_97E0**)(o + 4);
        void* str = vt[4].fn(o + vt[4].delta, info[1]);
        if (str) {
            func_001C9938(self, text, str, out);
        } else {
            char buf[0x40];
            sprintf(buf, D_004660A8, info[1]);
            func_001C9B28(self, text, buf, (int)out);
            return;
        }
        break;
    }
    case 0:
        func_001C8050(self, text, info);
        return;
    }
    out[1] += t;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9938);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
struct sVec3_9938 { float x, y, z; };
struct sVec2_9938 { float x, y; };
struct sVEntry_9938 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_9938*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_00392C60(void* font, int c);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern "C" void func_001C9038(void* self, void* text, unsigned short* str, float* size);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004660E8[];
extern char D_004A1C38;

extern "C" void func_001C9938(void* self, cUIText* text, void* strp, float* out)
{
    unsigned short* str = (unsigned short*)strp;
    *(int*)((char*)text + 0x14) |= 0x80;
    void* font = cUIText_getFont(text);
    if (font == 0)
        return;
    int last = -1;
    int n = USTR_length(str);
    float w = 0.0f;
    int i = 0;
    unsigned short* buf = new (D_004660E8, 0x100, 0) unsigned short[n + 1];
    USTR_copy(buf, str);
    sVec3_9938 v;
    sVec2_9938 size;
    sVEntry_9938* vt = *(sVEntry_9938**)((char*)text + 8);
    vt[20].fn((char*)text + vt[20].delta, &size);
    v = *(sVec3_9938*)((char*)text + 0x50);
    while (i < n) {
        if (buf[i] == D_004A1C38 && buf[i + 1] == D_004A1C38) {
            last = -1;
            w = 0.0f;
            i += 2;
        } else {
            if (buf[i] == '\r') {
                w = 0.0f;
                last = -1;
            } else {
                float cw = func_00392C60(font, buf[i]) * v.x;
                if (buf[i] == ' ')
                    last = i;
                w += cw;
                if (size.x < w && last >= 0) {
                    buf[last] = '\r';
                    i = last;
                    w = 0.0f;
                    last = -1;
                }
            }
            i++;
        }
    }
    func_001C9038(self, text, buf, out);
    cUIText_setUnicodeString(text, buf);
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001C9B28);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001C9210(void* self, cUIText* text, const char* str, int a3);

extern "C" void func_001C9B28(void* self, cUIText* text, const char* str, int a3)
{
    func_001C9210(self, text, str, a3);
    cUIText_setAsciiString(text, str);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9B68);
#ifdef SKIP_ASM
extern "C" void func_001C9B68(void* self, float* size, float* item)
{
    float w = item[2];
    if (size[0] < w) {
        size[0] = w;
    }
    size[1] += item[3];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_createConfirmationPopup);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cScreenPopup_cScreenPopup(cScreenPopup* mem, void* engine, void* owner);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char D_00466110[];
extern char D_0045DCC8[];
extern char D_0045FCD8[];

struct sVec3K1C9B98 {
    float x, y, z;
    sVec3K1C9B98(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sPairK1C9B98 { int a, b; };
struct sEntryK1C9B98 { sPairK1C9B98 p; int c; };
struct sSelfK1C9B98 { char pad[0x2D0]; sEntryK1C9B98 entries[4]; };

/**
 * Opens a yes/no confirmation popup for one of the popup's options.
 *
 * Allocates a "Confirmation Popup" cScreenPopup with this popup as parent and fills its info: two options
 * "kT_CMNYes" and "kT_CMNNo", offset (30, 40), layer offset miLayerOffset + 3, and the message and default
 * option of mtConfirmation[idx]. Stores it in mpConfirmPopup, remembers idx in miActiveConfirmation and
 * pushes it on the engine's state stack as a popup.
 *
 * @param idx  the option index, 0-4
 */
extern "C" void cScreenPopup_createConfirmationPopup(cScreenPopup* self, int idx)
{
    char* popup = (char*)cScreenPopup_cScreenPopup((cScreenPopup*)cMemMan_alloc(0x360, D_00466110, 0x100, 0), self->mpEngine, self);
    self->mpConfirmPopup = (cScreenPopup*)popup;
    char* slots = popup + 0xBC;
    *(int*)(popup + 0xBC) = 2;
    func_001C57C0(slots, 0, GetHashValue32(D_0045DCC8));
    func_001C57C0(slots, 1, GetHashValue32(D_0045FCD8));
    sVec3K1C9B98 v(30.0f, 40.0f, 0.0f);
    *(sVec3K1C9B98*)(popup + 0x158) = v;
    *(int*)(popup + 0x14C) = self->miLayerOffset + 3;
    *(int*)(popup + 0x168) = ((sSelfK1C9B98*)self)->entries[idx].c;
    *(sPairK1C9B98*)(popup + 0xC0) = ((sSelfK1C9B98*)self)->entries[idx].p;
    self->miActiveConfirmation = idx;
    cUIStateStack_pushPopup((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18,
                  self->mpConfirmPopup);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9CD0);
#ifdef SKIP_ASM
extern char D_004A1C50[];
extern char D_004A1C58[];
extern char D_004A15E0[];

struct sUIObj_C9CD0 {
    char pad_0x0[0x14];
    int flags; // 0x14: bits 8..12 = layer (out-of-line copies: func_003A4B60 get, func_003A4B70 set)
    int getLayer() { return ((unsigned int)flags >> 8) & 0x1F; }
    void setLayer(int l) { flags = (flags & ~0x1F00) | ((l & 0x1F) << 8); }
};

extern "C" void func_001C9CD0(void* self, int v)
{
    char* s = (char*)self;
    sUIObj_C9CD0* o;
    // NOTE: one `layer` local shared by every block (hand-repeated code); a per-block temp
    // gives different registers.
    int layer;
#define ADD_LAYER(e) o = (e); if (o) { layer = o->getLayer(); o->setLayer(layer + v); }
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x48));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x4C));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x50));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x54));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x58));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x5C));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x60));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x64));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x68));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x74));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x7C));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x78));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x80));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x84));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x8C));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(D_004A1C50)));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(D_004A1C58)));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(D_004A15E0)));
    for (int i = 0; i < 5; i++) {
        ADD_LAYER(((sUIObj_C9CD0**)(s + 0x90))[i]);
    }
#undef ADD_LAYER
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA0F0);
#ifdef SKIP_ASM
struct sVEntry_A0F0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA0F0(cScreenPopup* self, int idx, int a)
{
    char* o = *(char**)((char*)self + (idx << 2) + 0x90);
    if (o != 0) {
        sVEntry_A0F0* vt = *(sVEntry_A0F0**)(o + 8);
        vt[8].fn(o + vt[8].delta, a);
        sVEntry_A0F0* vt2 = *(sVEntry_A0F0**)(o + 8);
        vt2[25].fn(o + vt2[25].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA150);
#ifdef SKIP_ASM
extern "C" int func_001CA150(cScreenPopup* self)
{
    void* p = self->mpMenu;
    if (p == 0) {
        return self->info.miDefaultOption;
    }
    int n = *(unsigned char*)((char*)p + 0x95);
    if (self->meType == -1) {
        return n;
    }
    return n - 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onWidgetMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" char* func_00397870(void* list, int idx);

struct sVE1CA180 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

static inline void SetColour1CA180(char* obj, void* colour)
{
    sVE1CA180* vt = *(sVE1CA180**)(obj + 8);
    vt[11].fn(obj + vt[11].delta, colour);
}

/**
 * Handles a menu event of the popup's menu: colours the selection and decides which events pass on.
 *
 * Message 5 colours the selected entry with mtHighlightedColor and the others with mtUnhighlightColor (the
 * text-box pair is coloured through its static label). Messages 8 and 9 return 0; 7 returns 0x101 only when
 * mbReceivePrev is set; 6 returns 0 while mbNoEntryOnNext is set.
 *
 * @param menu  the menu (On Tour: cUIMenu *pMenu)
 * @param msg  the menu event (On Tour: Event)
 * @return 0 or 0x101 as above; 0x101 for the other events
 */
// Popup menu event handler: on draw (5) colours the selected entry; 0x101 = not handled.
extern "C" int cScreenPopup_onWidgetMenu(cScreenPopup* self, char* menu, int msg)
{
    switch (msg) {
    case 5: {
        int sel = *(unsigned char*)(menu + 0x95);
        for (int i = 0; i < *(unsigned char*)(menu + 0x96); i++) {
            char* obj = func_00397870(menu + 0x74, i);
            if (obj == (char*)self->mpTextBoxPair) {
                obj = (char*)self->mpStaticBox;
            }
            if (i == sel) {
                SetColour1CA180(obj, ((char*)self) + 0x32C);
            } else {
                SetColour1CA180(obj, ((char*)self) + 0x33C);
            }
        }
        break;
    }
    case 8: case 9:
        return 0;
    case 7:
        return self->mbReceivePrev ? 0x101 : 0;
    case 6:
        return self->mbNoEntryOnNext ? 0 : 0x101;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cScreenPopup_createConfirmationPopup(cScreenPopup* self, int idx);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(cKeyboardPopup* self, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cKeyboardPopup_setMaxCharacters(cKeyboardPopup* popup, int a1);
extern "C" void cUIStateStack_pushPopup(void* stack, void* item);
extern char D_0045D8D8[];

struct sPopupVE_1CA298 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sPopupOwner_1CA298 {
    char pad0[8];
    sPopupVE_1CA298* vt;   // 0x8
};

struct sBtn_1CA298 {
    int confirm;
    int b;
    int c;
};

static inline void Notify_1CA298(sPopupOwner_1CA298* o, int vi, void* self, int v)
{
    o->vt[vi].fn((char*)o + o->vt[vi].delta, self, v);
}

/**
 * Handles a widget event of the popup: option picks, the text box, back and the keyboard's result.
 *
 * Event 5 on an option either opens its confirmation (cScreenPopup_createConfirmationPopup()) or reports
 * option + 15 to the parent's onPopupEvent (slot 17); on the text-box pair it opens a cKeyboardPopup limited
 * to miMaxChars. A popup without options (type 0) only sets mbNoEntryOnNext. Event 6 reports 0x14 to the
 * parent when mbReceivePrev is set. Event 13 copies the keyboard's text into mszInputText and refits the
 * entry box. Events 7 and 8 are ignored; anything else goes to the parent's onWidgetEvent (slot 19).
 *
 * @param id  the widget (On Tour: cUIThing *pWidget), compared as an int against the option pointers
 * @param ev  the widget event (On Tour: Event)
 */
extern "C" void cScreenPopup_onWidgetEvent(cScreenPopup* self, int id, int ev)
{
    switch (ev) {
    case 5:
        if (self->meType != 0) {
            int handled = 0;
            int i;
            self->mbNoEntryOnNext = 0;
            for (i = 0; i < 5; i++) {
                if (id == ((int*)(((char*)self) + 0x90))[i]) {
                    if (((sBtn_1CA298*)(((char*)self) + 0x2D0))[i].confirm != 0) {
                        cScreenPopup_createConfirmationPopup(self, i);
                    } else {
                        Notify_1CA298((sPopupOwner_1CA298*)self->mpParentState, 17, ((char*)self), i + 15);
                    }
                    handled = 1;
                    break;
                }
            }
            if (handled) {
                break;
            }
            if (id != (int)self->mpTextBoxPair) {
                break;
            }
            void* p = cMemMan_alloc(0x444, D_0045D8D8, 0x100, 0);
            self->mpKeyboard = (cKeyboardPopup*)cKeyboardPopup_cKeyboardPopup((cKeyboardPopup*)p, self->mpEngine, ((char*)self), 0, 0xF);
            cKeyboardPopup_setMaxCharacters(self->mpKeyboard, self->miMaxChars);
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, self->mpKeyboard);
            if (self->mpStaticBox != 0) {
                cScreenPopup_setTextString(self, ((char*)self) + 0x2B4, self->mpStaticBox);
            }
        } else {
            self->mbNoEntryOnNext = 1;
        }
        break;
    case 6:
        if (self->mbReceivePrev != 0) {
            Notify_1CA298((sPopupOwner_1CA298*)self->mpParentState, 17, ((char*)self), 0x14);
        }
        break;
    case 7:
    case 8:
        break;
    case 13:
        cBXString_cBXString4(((char*)self) + 0x2B0, (char*)self->mpKeyboard + 0x74);
        if (self->mpEnterBox != 0 && *(int*)(self->mszInputText - 8) != 0) {
            cScreenPopup_checkAndFixTextEntry(self, self->mpEnterBox, (const char*)self->mszInputText);
        }
        break;
    default:
        {
            sPopupOwner_1CA298* o = (sPopupOwner_1CA298*)self->mpParentState;
            ((void (*)(void*, int, int))o->vt[19].fn)((char*)o + o->vt[19].delta, id, ev);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA488);
#ifdef SKIP_ASM
struct sVEntry_func_001CA488 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA488(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA488* vt = *(sVEntry_func_001CA488**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA4C0);
#ifdef SKIP_ASM
struct sVEntry_func_001CA4C0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA4C0(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA4C0* vt = *(sVEntry_func_001CA4C0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_handleEvent);
#ifdef SKIP_ASM
class cScreenPopup_handleEvent_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

/**
 * Passes an event on to the popup's parent state.
 *
 * Calls handleEvent (vtable slot 24) on mpParentState with both arguments unchanged.
 *
 * @param msg  event message (On Tour: msg)
 * @param pParam  event parameter (On Tour: pParam)
 * @return the parent state's handleEvent result
 */
extern "C" int cScreenPopup_handleEvent(cScreenPopup* self, int msg, int pParam)
{
    return ((cScreenPopup_handleEvent_cObj*)self->mpParentState)->v24(msg, pParam);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onUpdate);
#ifdef SKIP_ASM
struct sVEntry001CA528 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039E510(void*);

/**
 * Updates the popup, first updating its parent state when the popup passes updates through.
 *
 * When mbPassUpdate is set, calls the parent state's onUpdate (vtable slot 12); then runs the base UI-state
 * update func_0039E510().
 */
extern "C" void cScreenPopup_onUpdate(cScreenPopup* self)
{
    if (self->mbPassUpdate != 0) {
        void* obj = self->mpParentState;
        sVEntry001CA528* vt = *(sVEntry001CA528**)((char*)obj + 8);
        vt[12].fn((char*)obj + vt[12].delta);
    }
    func_0039E510(self);
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the popup's 2D layer with the shared UI-screen render routine func_0039E6B8().
 *
 * @return no meaningful value (func_0039E6B8() returns nothing)
 */
void* cScreenPopup_onRender2D(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onPopupEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cKeyboardPopup_setInputText(cKeyboardPopup* popup, const char* a1);
extern "C" void func_001CCF00(cKeyboardPopup* popup, const char* a1);
// PORT: the unit declares func_0039F190 as void; it returns a value this caller ignores.
extern "C" int func_0039F190_i(void* self, int a1) __asm__("func_0039F190");

struct sVE1CA598 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Handles events from the popup's own child popups (the keyboard and the confirmation popup).
 *
 * 0x15 from the keyboard hands it the current input and static texts. 0xF (yes) pops the confirmation and
 * keeps miActiveConfirmation + 15 as the return code; 0x10 and 0x14 pop it with -1; 0x11 to 0x13 just pop it.
 * 0x16 (the child is gone) clears mpKeyboard or mpConfirmPopup; for the confirmation it then reports the
 * return code to the parent's onPopupEvent (slot 17) unless it is -1.
 *
 * @param child  the child popup that sent the event (On Tour: cUIState *pPopup)
 * @param msg  the event (On Tour: Event)
 */
// Child-popup message handler: forwards results, plays sounds and closes the popup when done.
extern "C" void cScreenPopup_onPopupEvent(cScreenPopup* self, void* child, int msg)
{
    switch (msg) {
    case 0x15:
        if (child == (void*)self->mpKeyboard) {
            cKeyboardPopup_setInputText((cKeyboardPopup*)child, (const char*)self->mszInputText);
            func_001CCF00(self->mpKeyboard, (const char*)self->mszStaticText);
        }
        break;
    case 0xF:
        func_0039F190_i((char*)self->mpEngine + 0x18, 1);
        self->miConfimationReturnCode = self->miActiveConfirmation + 0xF;
        break;
    case 0x10: case 0x14:
        self->miConfimationReturnCode = -1;
        func_0039F190_i((char*)self->mpEngine + 0x18, 1);
        break;
    case 0x11: case 0x12: case 0x13:
        func_0039F190_i((char*)self->mpEngine + 0x18, 1);
        break;
    case 0x16:
        if (child == (void*)self->mpKeyboard) {
            self->mpKeyboard = 0;
        }
        if (child == (void*)self->mpConfirmPopup) {
            self->mpConfirmPopup = 0;
            if (self->miConfimationReturnCode != -1) {
                char* obj = (char*)self->mpParentState;
                sVE1CA598* vt = *(sVE1CA598**)(obj + 8);
                vt[17].fn(obj + vt[17].delta, ((char*)self), self->miConfimationReturnCode);
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_checkAndFixTextEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
struct sVec3_A6A0 { float x, y, z; };
struct sVec2_A6A0 { float x, y; };
struct sVEntry_A6A0 {
    short delta;
    short index;
    void (*fn)(void*, sVec2_A6A0*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" char* func_004162D0(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004661A0[];
extern char D_004A1BD0[];

static inline void measure_A6A0(cUIText* text, const char* s, float* out)
{
    char* f = (char*)cUIText_getFont(text);
    cFont_textExtent(f, s, out, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
}

/**
 * Shows a string in a text object, cut short with "..." when it does not fit the box.
 *
 * Scales the font to the text's scale, copies the string into a buffer of miMaxChars + 4 bytes and drops
 * characters from the end until the text plus "..." fits the box width minus 1. The cut text gets "..."
 * appended; a string that fits is shown unchanged.
 *
 * @param text  the text object (the entry box)
 * @param str  the ASCII string to show
 */
extern "C" void cScreenPopup_checkAndFixTextEntry(cScreenPopup* self, cUIText* text, const char* str)
{
    int fixed = 0;
    sVec3_A6A0 scale = *(sVec3_A6A0*)((char*)text + 0x50);
    sVec2_A6A0 size;
    float ext[4];
    float dots[4];
    sVEntry_A6A0* vt = *(sVEntry_A6A0**)((char*)text + 8);
    vt[20].fn((char*)text + vt[20].delta, &size);
    char* f = (char*)cUIText_getFont(text);
    float sx = *(float*)(f + 0x30) * scale.x;
    float sy = *(float*)(f + 0x34) * scale.y;
    *(float*)(f + 0x38) = sx;
    *(float*)(f + 0x3C) = sy;
    char* buf = new (D_004661A0, 0x100, 0) char[self->miMaxChars + 4];
    int len = strlen(str) - 1;
    strcpy(buf, str);
    const char* dotstr = D_004A1BD0;
    measure_A6A0(text, dotstr, dots);
    measure_A6A0(text, buf, ext);
    while (size.x - dots[2] - 1.0f < ext[2]) {
        fixed = 1;
        buf[len] = 0;
        len--;
        measure_A6A0(text, buf, ext);
    }
    if (fixed) {
        func_004162D0(buf, dotstr);
        cUIText_setAsciiString(text, buf);
    } else {
        cUIText_setAsciiString(text, str);
    }
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA8A0);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

extern "C" void func_001CA8A0(cBuyPopupInfo* self)
{
    MEM_fill((char*)self + 0x14, 0, 8);
    MEM_fill((char*)self + 0xC, 0, 8);
    MEM_fill((char*)self + 0x1C, 0, 8);
    self->mCost = 0;
    self->mMoney = 0;
    self->mCreditsLeft = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySongByCredit);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004661B8[];
extern char D_004661C8[];

/**
 * Fills a buy popup's info for getting a song with a credit.
 *
 * Stores the song title as an ASCII element (mItemName), the text ids "kT_OVRSong" (mItemType) and
 * "kT_OVRCMNBuySongCredit" (mQuestion), and the credits left; the cost and cash are left as they are.
 *
 * @param song  the song title (an ASCII string pointer)
 * @param credit  the credits left (mCreditsLeft)
 */
extern "C" void cBuyPopupInfo_initBuySongByCredit(cBuyPopupInfo* self, int song, int credit)
{
    int three = 3;
    int h = GetHashValue32(D_004661B8);
    *(int*)((char*)self + 0x10) = song;
    self->mItemType.eType = three;
    self->mItemName.eType = 2;
    *(int*)((char*)self + 0x18) = h;
    h = GetHashValue32(D_004661C8);
    self->mCreditsLeft = credit;
    self->mQuestion.eType = three;
    *(int*)((char*)self + 0x20) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySong);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004661B8[];
extern char D_004661E0[];

/**
 * Fills a buy popup's info for buying a song with cash.
 *
 * Stores the price, the cash and -1 in the first three words (the third is the credit count that
 * cBuyPopupInfo_initBuySongByCredit() sets), the song title as an ASCII element, and the text ids "kT_OVRSong"
 * and "kT_16BuyMusicTrack" as two ID elements.
 *
 * @param songName  song title (an ASCII string pointer)
 * @param cost  price of the song
 * @param money  cash the player has (cFEStateRequestLine_showBuyPopup() passes its cash total)
 */
extern "C" void cBuyPopupInfo_initBuySong(int* self, int songName, int cost, int money)
{
    int three = 3;
    int h = GetHashValue32(D_004661B8);
    self[4] = songName;
    self[5] = three;
    self[3] = 2;
    self[6] = h;
    h = GetHashValue32(D_004661E0);
    self[1] = money;
    self[7] = three;
    self[0] = cost;
    self[2] = -1;
    self[8] = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CAA18);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern char D_004661F8[];
extern char D_00466210[];
extern char D_00466228[];
extern char D_00466240[];
extern char D_00466258[];
extern char D_00466270[];
extern char D_00466288[];

// Fills in a popup description: title, OK button and the message text for the given kind.
extern "C" void func_001CAA18(int* desc, int kind, int a2, int a3, int a4)
{
    desc[4] = a2;
    desc[3] = 2;
    desc[8] = GetHashValue32(D_004661F8);
    desc[7] = 3;
    desc[2] = -1;
    desc[0] = a3;
    desc[1] = a4;
    switch (kind) {
    case 3:
        desc[6] = GetHashValue32(D_00466210);
        desc[5] = 3;
        break;
    case 4:
        desc[6] = GetHashValue32(D_00466228);
        desc[5] = 3;
        break;
    case 5:
        desc[6] = GetHashValue32(D_00466240);
        desc[5] = 3;
        break;
    case 6:
        desc[6] = GetHashValue32(D_00466258);
        desc[5] = 3;
        break;
    case 7:
        desc[6] = GetHashValue32(D_00466270);
        desc[5] = 3;
        break;
    case 8:
        desc[6] = GetHashValue32(D_00466288);
        desc[5] = 3;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuyBolt);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004662B8[];
extern char D_004661F8[];

/**
 * Fills a buy popup's info for buying an equipment item.
 *
 * Same layout as cBuyPopupInfo_initBuySong(): the price, the cash and -1 (no credits), the item's name as an
 * ASCII element, and the text ids "kT_CMNItemColon" and "kT_OVRCMNBuyItem" as two ID elements.
 *
 * @param boltName  item name (an ASCII string pointer; the equipment-detail state passes the item's word at 0x14)
 * @param cost  price (the caller passes the item's price times 10)
 * @param money  cash the player has
 */
extern "C" void cBuyPopupInfo_initBuyBolt(int* self, int boltName, int cost, int money)
{
    int three = 3;
    int h = GetHashValue32(D_004662B8);
    self[4] = boltName;
    self[5] = three;
    self[3] = 2;
    self[6] = h;
    h = GetHashValue32(D_004661F8);
    self[1] = money;
    self[7] = three;
    self[0] = cost;
    self[2] = -1;
    self[8] = h;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_cUIStateBuyPopup);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void func_001CA8A0(cBuyPopupInfo* self);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern void* cUIStateBuyPopup_vtbl[];

extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(cUIStateBuyPopup* self, void* engine, void* owner, signed char idx)
{
    func_0039E318(self, engine, owner);
    self->mVtbl = cUIStateBuyPopup_vtbl;
    func_001CA8A0((cBuyPopupInfo*)((char*)self + 0x48));
    self->mUserNum = idx;
    self->mHashName = 7;
    void* p = *(void**)self->mpEngine;
    if (p != 0) {
        *(char*)&self->mCtrlBits = func_001A1CD0(p, idx);
    }
    func_001CA8A0((cBuyPopupInfo*)((char*)self + 0x48));
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004662C8[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the buy popup's screen "139buy_popup" and plays its first frame.
 */
extern "C" void cUIStateBuyPopup_onCreateScreen(cUIStateBuyPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004662C8), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
class cUIObj_AC98 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern "C" char* getDollarString(void* p);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004662D8[];
extern char D_004662E8[];
extern char D_004662F8[];
extern char D_004605F0[];
extern char D_00460610[];
extern char D_00466308[];
extern char D_00466318[];
extern char D_00466328[];
extern char D_004A12C0[];

/**
 * Fills the buy popup's widgets from its cBuyPopupInfo as they are created.
 *
 * "item type", "item name" and "question" get their elements (ASCII, unicode or text id). With no credits
 * left (mCreditsLeft <= 0) "cost_answer" and "you have_answer" show the price and cash as dollars and "group
 * credit" is hidden; otherwise "credits remain" shows the count and "group cash" is hidden.
 *
 * @param widget  the widget being created
 */
extern "C" void cUIStateBuyPopup_onWidgetCreate(void* self, void* widget)
{
    cUIText* text = (cUIText*)widget;
    int id = *(int*)((char*)widget + 0x38);
    if (id == GetHashValue32(D_004662D8)) {
        int type = *(int*)((char*)self + 0x5C);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x60));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x60));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x60));
    } else if (id == GetHashValue32(D_004662E8)) {
        int type = *(int*)((char*)self + 0x54);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x58));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x58));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x58));
    } else if (id == GetHashValue32(D_004662F8)) {
        int type = *(int*)((char*)self + 0x64);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x68));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x68));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x68));
    } else if (id == GetHashValue32(D_004605F0)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            cUIText_setAsciiString(text, getDollarString(*(void**)((char*)self + 0x48)));
    } else if (id == GetHashValue32(D_00460610)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            cUIText_setAsciiString(text, getDollarString(*(void**)((char*)self + 0x4C)));
    } else if (id == GetHashValue32(D_00466308)) {
        int n = *(int*)((char*)self + 0x50);
        if (n > 0) {
            char buf[16];
            sprintf(buf, D_004A12C0, n);
            cUIText_setAsciiString(text, buf);
        }
    } else if (id == GetHashValue32(D_00466318)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            ((cUIObj_AC98*)widget)->setVisible(0);
    } else if (id == GetHashValue32(D_00466328)) {
        if (*(int*)((char*)self + 0x50) > 0)
            ((cUIObj_AC98*)widget)->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters the buy popup's menu events: 7, 8 and 9 return 0x100, everything else 0x101.
 *
 * @param pMenu  the menu (On Tour: cUIMenu *pMenu); unused
 * @param key  the menu event (On Tour: Event)
 * @return 0x100 for events 7-9, else 0x101
 */
// Key filter: keys 7, 8 and 9 return 0x100, every other key 0x101.
extern "C" int cUIStateBuyPopup_onWidgetMenu(cUIStateBuyPopup* self, int pMenu, unsigned int key)
{
    int r = 0x101;
    if (key < 10) {
        if (key < 7) {
            r = 0x101;
        } else {
            return 0x100;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14E8[];

/**
 * Records the buy popup's answer: on event 5 mAccept (+0x6C) becomes 1 for the "yes" widget, 0 otherwise.
 *
 * @param item  the widget (ignored when 0)
 * @param event  the widget event; only 5 is handled
 */
extern "C" void cUIStateBuyPopup_onWidgetEvent(cUIStateBuyPopup* self, void* item, int event)
{
    if (item != 0) {
        if (event == 5) {
            int h = *(int*)((char*)item + 0x38);
            if (h == GetHashValue32(D_004A14E8)) {
                self->mbAccept = 1;
            } else {
                self->mbAccept = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_initBuyTrick);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466338[];
extern char D_00466350[];

/**
 * Fills the buy popup's info for buying an uber trick.
 *
 * Stores the price and the cash, the trick's name as an ASCII element, and the text ids "kT_OVRCMNUbertrick"
 * and "kT_OVRCMNBuyThisUber" as the item type and the question. mCreditsLeft is not touched.
 *
 * @param trick  the trick's name (an ASCII string pointer)
 * @param cost  the price
 * @param money  the cash the player has
 */
extern "C" void cUIStateBuyPopup_initBuyTrick(cUIStateBuyPopup* self, int trick, int cost, int money)
{
    int three = 3;
    self->mInfo.mCost = cost;
    self->mInfo.mMoney = money;
    *(int*)((char*)self + 0x58) = trick;
    self->mInfo.mItemName.eType = 2;
    int h = GetHashValue32(D_00466338);
    self->mInfo.mItemType.eType = three;
    *(int*)((char*)self + 0x60) = h;
    h = GetHashValue32(D_00466350);
    self->mInfo.mQuestion.eType = three;
    *(int*)((char*)self + 0x68) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CAFC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466368[];
extern char D_004A1408[];

extern "C" void func_001CAFC0(cUIStateBuyPopup* self, int a1, int a2, int a3)
{
    char* s = D_004A1408;
    int two = 2;
    *(char**)((char*)self + 0x58) = s;
    self->mInfo.mItemName.eType = two;
    self->mInfo.mCost = a2;
    self->mInfo.mMoney = a3;
    int h = GetHashValue32(D_00466368);
    self->mInfo.mQuestion.eType = two;
    *(char**)((char*)self + 0x68) = s;
    self->mInfo.mItemType.eType = 3;
    *(int*)((char*)self + 0x60) = h;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_cKeyboardPopup);
#ifdef SKIP_ASM
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" void cBXString_Reset(void* self);
extern "C" void cKeyboardPopup_fillSelectableKeys(cKeyboardPopup* self);
extern int D_004A3E90;
extern void* D_004A33F0;
extern void* cKeyboardPopup_vtbl[];

extern "C" void* cKeyboardPopup_cKeyboardPopup(cKeyboardPopup* self, void* engine, void* owner, int a3, unsigned char a4)
{
    func_0039E318(self, engine, owner);
    int d = D_004A3E90;
    self->mCtrlBits = a4;
    self->mbPassword = a3;
    self->mbSetInputText = 0;
    self->mbTransistionDone = 0;
    self->mbPopOnEnd = 1;
    self->miMinCharacters = 0;
    self->mbShiftOn = 0;
    self->mbCapsOn = 1;
    *(char*)((char*)self + 0x74) = 0;
    self->miCursorPos = 0;
    self->miViewableStartIndex = 0;
    self->miViewableEndIndex = 0;
    self->mbClearOnFirst = 0;
    self->mbDisableForFirst = 0;
    self->mVtbl = cKeyboardPopup_vtbl;
    self->miCurrentKey = 0x1D;
    self->mszStatic = (char*)d;
    self->miMaxCharacters = 0x3F;
    cBXString_Reset((char*)self + 0x424);
    void* bx = D_004A33F0;
    self->meSkipState = 2;
    self->miLastKey = 0x2B;
    self->meType = 1;
    self->mpustrStatic = 0;
    self->muiStatic = (unsigned int)0;
    self->meLastDirection = 0;
    self->mbReferEventsToParent = 0;
    self->mpBackCompact = 0;
    self->mpBackRegular = 0;
    self->mpBackExpand = 0;
    if (bx != 0)
        self->mbCapsOn = (*(unsigned int*)((char*)bx + 0x88) >> 1) & 1;
    cKeyboardPopup_fillSelectableKeys(self);
    self->mbCallParentsUpdate = 0;
    self->mbAcceptText = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup__cKeyboardPopup);
#ifdef SKIP_ASM
extern void* cKeyboardPopup_vtbl[];
void cMemMan_free(void* ptr);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void func_0039E390(void* self, int flags);

/**
 * Destroys the on-screen keyboard: frees its pending unicode static text and its static string.
 *
 * Stores the class vtable, frees mpustrStatic when set, destroys the mszStatic cBXString and runs the
 * cUIState destructor (func_0039E390), which also frees the object when the flags ask for it.
 *
 * @param flags  destructor flags, passed on to func_0039E390
 */
extern "C" void cKeyboardPopup__cKeyboardPopup(cKeyboardPopup* self, int flags)
{
    self->mVtbl = cKeyboardPopup_vtbl;
    void* p = self->mpustrStatic;
    if (p != 0) {
        cMemMan_free(p);
        self->mpustrStatic = 0;
    }
    cBXString__cBXString((char*)self + 0x424, 2);
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466388[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the on-screen keyboard's "Fullkeyboard" screen and starts it at frame 0.
 *
 * The screen goes into mpScreen; frame 0 is played only when the screen was created.
 */
extern "C" void cKeyboardPopup_onCreateScreen(cKeyboardPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_00466388), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onUpdate);
#ifdef SKIP_ASM
struct sVEntry001CB208 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039E510(void*);
extern "C" void func_0039F190(void* self, int a1);

/**
 * Updates the on-screen keyboard and closes it when the online overlay timeout runs out in a game.
 *
 * Runs the base UI-state update func_0039E510(). When cGameComm exists, its mOverlayTimeout is 0 and a game is
 * running, pops the top state off the UI state stack (func_0039F190(stack, 1)). When mbCallParentsUpdate is set,
 * also calls the parent state's onUpdate (vtable slot 12).
 */
extern "C" void cKeyboardPopup_onUpdate(cKeyboardPopup* self)
{
    func_0039E510(self);
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mOverlayTimeout == 0 && (int)SSXApp->Game != 0) {
        func_0039F190((char*)self->mpEngine + 0x18, 1);
    }
    if (self->mbCallParentsUpdate != 0) {
        void* obj = self->mpParentState;
        if (obj != 0) {
            sVEntry001CB208* vt = *(sVEntry001CB208**)((char*)obj + 8);
            vt[12].fn((char*)obj + vt[12].delta);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void func_0039F190(void* self, int a1);

/**
 * Closes the keyboard on widget events 5 and 6 by popping the top state of the engine's stack.
 *
 * @param pWidget  the widget (unused)
 * @param Event  the widget event
 */
extern "C" void cKeyboardPopup_onWidgetEvent(cKeyboardPopup* self, void* pWidget, unsigned int Event)
{
    switch (Event) {
    case 5:
    case 6:
        func_0039F190((char*)self->mpEngine + 0x18, 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern int D_004A1A70;
extern char D_00466398[];
extern char D_004663A8[];
extern char D_004663B8[];
extern char D_004663C8[];

struct sVEntryK1CB2C8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int isOnK1CB2C8()
{
    return D_004A1A70 == 1;
}

static inline int IsK1CB2C8(int id, char* s)
{
    return id == GetHashValue32(s);
}

/**
 * Sets up the keyboard's widgets as they are created: the help group and the three backgrounds.
 *
 * "HelpGroup" is shown only online (D_004A1A70 == 1) and outside a game (SSXApp->Game == 0); "back_com",
 * "back_exp" and "back_reg" are kept in mpBackCompact, mpBackExpand and mpBackRegular.
 *
 * @param widget  the widget being created
 */
extern "C" void cKeyboardPopup_onWidgetCreate(cKeyboardPopup* self, void* widget)
{
    if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_00466398)) {
        sVEntryK1CB2C8* vt = *(sVEntryK1CB2C8**)((char*)widget + 8);
        vt[9].fn((char*)widget + vt[9].delta, 0);
        if (isOnK1CB2C8()) {
            sVEntryK1CB2C8* vt2 = *(sVEntryK1CB2C8**)((char*)widget + 8);
            vt2[9].fn((char*)widget + vt2[9].delta, 1);
        }
        if ((int)SSXApp->Game != 0) {
            sVEntryK1CB2C8* vt3 = *(sVEntryK1CB2C8**)((char*)widget + 8);
            vt3[9].fn((char*)widget + vt3[9].delta, 0);
        }
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663A8)) {
        self->mpBackCompact = (cUIVector*)widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663B8)) {
        self->mpBackExpand = (cUIVector*)widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), D_004663C8)) {
        self->mpBackRegular = (cUIVector*)widget;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleEvent);
#ifdef SKIP_ASM
struct sVEntry001CB3D0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

/**
 * Passes an event on to the parent state when the keyboard refers its events to it.
 *
 * Calls the parent's handleEvent (slot 24) only when mbReferEventsToParent is set and there is a parent.
 *
 * @param msg  event message (On Tour: msg)
 * @param pParam  event parameter (On Tour: pParam)
 * @return the parent's result, or 0
 */
extern "C" int cKeyboardPopup_handleEvent(cKeyboardPopup* self, int msg, int pParam)
{
    if (self->mbReferEventsToParent != 0) {
        void* obj = self->mpParentState;
        if (obj != 0) {
            sVEntry001CB3D0* vt = *(sVEntry001CB3D0**)((char*)obj + 8);
            return vt[24].fn((char*)obj + vt[24].delta, msg, pParam);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setInputText);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self);
extern "C" void cKeyboardPopup_updateEdit(cKeyboardPopup* self, int a1);

/**
 * Puts a text into the keyboard's edit field with the cursor at its end.
 *
 * Sets mbSetInputText, copies the text into mszEditString, puts the cursor at its end and shows all of it.
 * With mbDisableForFirst the space key (75) is enabled only for a non-empty text; the enter key (55) is
 * enabled once the text has miMinCharacters. Redraws the keys, and the edit field once the transition is
 * done.
 *
 * @param str  the text (On Tour: szText); copied without a length check into the 64-byte field
 */
extern "C" void cKeyboardPopup_setInputText(cKeyboardPopup* self, const char* str)
{
    int one = 1;
    char* buf = (char*)self + 0x74;
    self->mbSetInputText = one;
    strcpy(buf, str);
    self->miCursorPos = strlen(buf);
    self->miViewableStartIndex = 0;
    self->miViewableEndIndex = strlen(buf);
    if (self->mbDisableForFirst != 0) {
        cKeyboardPopup_setIncludeKey(self, 0x4B, strlen(buf) ? one : 0, 2);
    }
    if (strlen(buf) < self->miMinCharacters) {
        cKeyboardPopup_setIncludeKey(self, 0x37, 0, 2);
    } else {
        cKeyboardPopup_setIncludeKey(self, 0x37, 1, 2);
    }
    cKeyboardPopup_redrawScreen(self);
    if (self->mbTransistionDone != 0) {
        cKeyboardPopup_updateEdit(self, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_kbMoveLeft);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern signed char D_00441390[];

extern "C" void cKeyboardPopup_kbMoveLeft(cKeyboardPopup* self, int a1)
{
    bool b = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = b;
        flag = !flag;
    } else {
        flag = b != 0;
    }
    int idx = self->miCurrentKey;
    int old = self->miLastKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    int st = self->meLastDirection;
    if (a1) {
        self->miLastKey = idx;
        self->meLastDirection = 2;
        if (st == 3) {
            self->miCurrentKey = old;
            return;
        }
    }
    int s = self->miCurrentKey;
    if (s == 0 || s == 14 || s == 28 || s == 42 || s == 56 || s == 70)
        self->miCurrentKey += 13;
    else
        self->miCurrentKey = s - 1;
    while (D_00441390[self->miCurrentKey] == 0x2A)
        self->miCurrentKey -= 1;
    int val;
    if (cKeyboardPopup_allowSelection_out(self, self->miCurrentKey, &val)) {
        self->meSkipState = 2;
    } else {
        cKeyboardPopup_kbMoveLeft(self, 0);
        if (val)
            self->meSkipState = flag == 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_kbMoveRight);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern signed char D_00441390[];

extern "C" void cKeyboardPopup_kbMoveRight(cKeyboardPopup* self, int a1)
{
    bool b = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = b;
        flag = !flag;
    } else {
        flag = b != 0;
    }
    int idx = self->miCurrentKey;
    int old = self->miLastKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    int st = self->meLastDirection;
    if (a1) {
        self->miLastKey = idx;
        self->meLastDirection = 3;
        if (st == 2) {
            int v = self->meSkipState;
            if (v == 2 || (flag && v == 0) || (!flag && v == 0)) {
                if (cKeyboardPopup_allowSelection_out(self, old, 0)) {
                    // the do/while(0) wrapper (a compiled-out macro block) is needed to match register allocation
                    do {
                        self->miCurrentKey = old;
                        return;
                    } while (0);
                }
            }
        }
    }
    int s = self->miCurrentKey;
    if (s == 13 || s == 27 || s == 41 || s == 55 || s == 69 || s == 83)
        self->miCurrentKey -= 13;
    else
        self->miCurrentKey = s + 1;
    while (D_00441390[self->miCurrentKey] == 0x2A)
        self->miCurrentKey += 1;
    int val;
    if (cKeyboardPopup_allowSelection_out(self, self->miCurrentKey, &val)) {
        self->meSkipState = 2;
    } else {
        cKeyboardPopup_kbMoveRight(self, 0);
        if (val)
            self->meSkipState = flag == 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_kbMoveUp);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern signed char D_00441390[];
extern "C" int cKeyboardPopup_handleMovementUpFromQuickKeyRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementUpFromNumberRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementUpFromLast(cKeyboardPopup* self, int a1);

extern "C" void cKeyboardPopup_kbMoveUp(cKeyboardPopup* self, int a1)
{
    bool b = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = b;
        flag = !flag;
    } else {
        flag = b != 0;
    }
    int idx = self->miCurrentKey;
    int old = self->miLastKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    int st = self->meLastDirection;
    if (a1) {
        self->miLastKey = idx;
        self->meLastDirection = 0;
        if (st == 1) {
            int v = self->meSkipState;
            if (v == 2 || (flag && v == 0) || (!flag && v == 0)) {
                if (cKeyboardPopup_allowSelection_out(self, old, 0)) {
                    self->miCurrentKey = old;
                    return;
                }
            }
        }
    }
    unsigned int s = (unsigned int)self->miCurrentKey;
    if (s >= 70 && s <= 83) {
        if (cKeyboardPopup_handleMovementUpFromLast(self, flag))
            return;
        s = (unsigned int)self->miCurrentKey;
    } else if (s >= 14 && s <= 27) {
        if (cKeyboardPopup_handleMovementUpFromNumberRow(self, flag))
            return;
        s = (unsigned int)self->miCurrentKey;
    } else if (s < 14) {
        if (cKeyboardPopup_handleMovementUpFromQuickKeyRow(self, flag))
            return;
        s = (unsigned int)self->miCurrentKey;
    }
    if (s < 14)
        self->miCurrentKey = s + 70;
    else
        self->miCurrentKey -= 14;
    while (D_00441390[self->miCurrentKey] == 0x2A)
        self->miCurrentKey -= 1;
    int val;
    if (cKeyboardPopup_allowSelection_out(self, self->miCurrentKey, &val)) {
        self->meSkipState = 2;
    } else {
        cKeyboardPopup_kbMoveUp(self, 0);
        if (val)
            self->meSkipState = flag == 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_kbMoveDown);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern signed char D_00441390[];
extern "C" int cKeyboardPopup_handleMovementDownFromQuickKeyRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementDownFromLast(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementDownToLast(cKeyboardPopup* self, int a1);

/**
 * Moves the keyboard's selection one row down, skipping gaps and keys that cannot be selected.
 *
 * With controller input it first remembers the key and direction, and returns to the key it came from when
 * the previous direction was 0 (probably up), meSkipState allows it and that key is selectable. The quick-key
 * row, the shift row (56-69) and the last row (70-83) have their own handlers; other keys move 14 down (the
 * last row wraps to the top) past gaps in D_00441390. When the new key is not selectable it moves on
 * recursively and updates meSkipState.
 *
 * @param bControllerInput  nonzero for a move from the pad (not a recursive skip)
 */
extern "C" void cKeyboardPopup_kbMoveDown(cKeyboardPopup* self, int bControllerInput)
{
    bool b = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = b;
        flag = !flag;
    } else {
        flag = b != 0;
    }
    int idx = self->miCurrentKey;
    int old = self->miLastKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    int st = self->meLastDirection;
    if (bControllerInput) {
        self->miLastKey = idx;
        self->meLastDirection = 1;
        if (st == 0) {
            int v = self->meSkipState;
            if (v == 2 || (flag && v == 0) || (!flag && v == 0)) {
                if (cKeyboardPopup_allowSelection_out(self, old, 0)) {
                    self->miCurrentKey = old;
                    return;
                }
            }
        }
    }
    unsigned int s = (unsigned int)self->miCurrentKey;
    if (s >= 56 && s <= 69) {
        if (cKeyboardPopup_handleMovementDownToLast(self, flag))
            return;
    } else if (s >= 70 && s <= 83) {
        if (cKeyboardPopup_handleMovementDownFromLast(self, flag))
            return;
    } else if (s < 14) {
        if (cKeyboardPopup_handleMovementDownFromQuickKeyRow(self, flag))
            return;
    }
    unsigned int s2 = (unsigned int)self->miCurrentKey;
    if (s2 >= 70 && s2 <= 83)
        self->miCurrentKey = s2 - 70;
    else
        self->miCurrentKey = s2 + 14;
    while (D_00441390[self->miCurrentKey] == 0x2A)
        self->miCurrentKey += 1;
    int val;
    if (cKeyboardPopup_allowSelection_out(self, self->miCurrentKey, &val)) {
        self->meSkipState = 2;
    } else {
        cKeyboardPopup_kbMoveDown(self, 0);
        if (val)
            self->meSkipState = flag == 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementUpFromQuickKeyRow);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_skipRowToBottom(cKeyboardPopup* self, int a1);

extern "C" int cKeyboardPopup_handleMovementUpFromQuickKeyRow(cKeyboardPopup* self, int a1)
{
    int val;
    int state = self->miCurrentKey;
    if (state >= 5 && state <= 8) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4B, &val)) {
            self->miCurrentKey = 0x4B;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 9 && state <= 10) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4C, &val)) {
            self->miCurrentKey = 0x4C;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 11 && state <= 12) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4F, &val)) {
            self->miCurrentKey = 0x4F;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementDownFromQuickKeyRow);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_moveDownRows(cKeyboardPopup* self);

/**
 * Moves down from quick keys 4 and 5 to keys 16 and 18, or past the rows when those are not selectable.
 *
 * @param bUseReg  which key set the move is for; meSkipState records its inverse when the target is skipped
 * @return 1 when the current key is 4 or 5 (handled), else 0
 */
extern "C" int cKeyboardPopup_handleMovementDownFromQuickKeyRow(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int state = self->miCurrentKey;
    if (state == 4) {
        if (cKeyboardPopup_allowSelection_out(self, 0x10, &val)) {
            self->miCurrentKey = 0x10;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_moveDownRows(self);
        }
        return 1;
    } else if (state == 5) {
        if (cKeyboardPopup_allowSelection_out(self, 0x12, &val)) {
            self->miCurrentKey = 0x12;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_moveDownRows(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementDownFromLast);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_skipRowToTop(cKeyboardPopup* self);

/**
 * Moves down from the last row's arrow-left (76) and space (75) keys to the top rows.
 *
 * From key 76 it selects key 9 when key 0 is selectable (EA's code tests 0 but moves to 9), else key 23; from
 * key 75 it selects key 19. When the target is not selectable it skips to the top
 * (cKeyboardPopup_skipRowToTop()).
 *
 * @param bUseReg  which key set the move is for; meSkipState records its inverse when the target is skipped
 * @return 1 when the current key is 75 or 76 (handled), else 0
 */
extern "C" int cKeyboardPopup_handleMovementDownFromLast(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int val2;
    int state = self->miCurrentKey;
    if (state == 0x4C) {
        if (cKeyboardPopup_allowSelection_out(self, 0, &val)) {
            self->miCurrentKey = 9;
        } else if (cKeyboardPopup_allowSelection_out(self, 0x17, &val)) {
            self->miCurrentKey = 0x17;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state == 0x4B) {
        if (cKeyboardPopup_allowSelection_out(self, 0x13, &val2)) {
            self->miCurrentKey = 0x13;
        } else {
            if (val2) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementUpFromNumberRow);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_skipRowToBottom(cKeyboardPopup* self, int a1);

extern "C" int cKeyboardPopup_handleMovementUpFromNumberRow(cKeyboardPopup* self, int a1)
{
    int val;
    int r = cKeyboardPopup_allowSelection_out(self, 0, 0);
    int state = self->miCurrentKey;
    if (r) {
        if (state >= 16 && state <= 17) {
            self->miCurrentKey = 4;
            return 1;
        } else if (state == 18) {
            self->miCurrentKey = 5;
            return 1;
        }
        return 0;
    }
    if (state >= 15 && state <= 22) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4B, &val)) {
            self->miCurrentKey = 0x4B;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            if (self->miCurrentKey == 0x14) {
                cKeyboardPopup_skipRowToBottom(self, 0x3E);
            } else {
                cKeyboardPopup_skipRowToBottom(self, -1);
            }
        }
        return 1;
    } else if (state == 23) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4C, &val)) {
            self->miCurrentKey = 0x4C;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state == 26) {
        if (cKeyboardPopup_allowSelection_out(self, 0x53, &val)) {
            self->miCurrentKey = 0x53;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 24 && state <= 25) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4F, &val)) {
            self->miCurrentKey = 0x4F;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementUpFromLast);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_moveUpRows(cKeyboardPopup* self);

extern "C" int cKeyboardPopup_handleMovementUpFromLast(cKeyboardPopup* self, int a1)
{
    int val;
    int state = self->miCurrentKey;
    if (state == 0x4C) {
        if (cKeyboardPopup_allowSelection_out(self, 0x42, &val)) {
            self->miCurrentKey = 0x42;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_moveUpRows(self);
        }
        return 1;
    } else if (state == 0x4F) {
        if (cKeyboardPopup_allowSelection_out(self, 0x42, &val)) {
            self->miCurrentKey = 0x42;
        } else {
            if (val) {
                self->meSkipState = a1 == 0;
            }
            cKeyboardPopup_moveUpRows(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleMovementDownToLast);
#ifdef SKIP_ASM
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
extern "C" void cKeyboardPopup_skipRowToTop(cKeyboardPopup* self);

/**
 * Moves down from the row above the last one onto the space (75) or arrow-left (76) key.
 *
 * Keys 57 to 65 go to the space key and key 66 to arrow-left; when that key is not selectable it skips to the
 * top instead (cKeyboardPopup_skipRowToTop()).
 *
 * @param bUseReg  which key set the move is for; meSkipState records its inverse when the target is skipped
 * @return 1 when the current key is 57-66 (handled), else 0
 */
extern "C" int cKeyboardPopup_handleMovementDownToLast(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int state = self->miCurrentKey;
    if (state == 0x39) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4B, &val)) {
            self->miCurrentKey = 0x4B;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state >= 0x3A && state <= 0x41) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4B, &val)) {
            self->miCurrentKey = 0x4B;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state == 0x42) {
        if (cKeyboardPopup_allowSelection_out(self, 0x4C, &val)) {
            self->miCurrentKey = 0x4C;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_skipRowToTop);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);

/**
 * Moves the on-screen keyboard cursor four rows up, then down a row at a time to the first selectable key.
 *
 * The keyboard is 6 rows of 14 keys: the search starts at miCurrentKey - 56 and adds 14 until
 * cKeyboardPopup_allowSelection() accepts the key, with no bound check. The move-down handlers call it when
 * moving down from the two bottom rows, so it probably wraps the cursor back to the top.
 */
extern "C" void cKeyboardPopup_skipRowToTop(cKeyboardPopup* self)
{
    int i = self->miCurrentKey - 0x38;
    while (!cKeyboardPopup_allowSelection(self, i, 0)) {
        i += 0xE;
    }
    self->miCurrentKey = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_skipRowToBottom);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);

extern "C" void cKeyboardPopup_skipRowToBottom(cKeyboardPopup* self, int start)
{
    int i;
    if (start == -1) {
        i = self->miCurrentKey + 0x38;
    } else {
        i = start;
    }
    while (!cKeyboardPopup_allowSelection((cKeyboardPopup*)self, i, 0)) {
        i -= 0xE;
    }
    self->miCurrentKey = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_moveUpRows);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);

extern "C" void cKeyboardPopup_moveUpRows(cKeyboardPopup* self)
{
    int i = self->miCurrentKey - 0xE;
    while (!cKeyboardPopup_allowSelection((cKeyboardPopup*)self, i, 0)) {
        i -= 0xE;
    }
    self->miCurrentKey = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_moveDownRows);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);

/**
 * Moves the on-screen keyboard cursor down to the next selectable key in the same column.
 *
 * Adds 14 (one row) to miCurrentKey until cKeyboardPopup_allowSelection() accepts the key, with no bound check.
 */
extern "C" void cKeyboardPopup_moveDownRows(cKeyboardPopup* self)
{
    int i = self->miCurrentKey + 0xE;
    while (!cKeyboardPopup_allowSelection(self, i, 0)) {
        i += 0xE;
    }
    self->miCurrentKey = i;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_pressKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" void func_00416210(void* dst, int c, int n);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self);
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int key, char* out, int a3);
extern "C" void cKeyboardPopup_updateEdit(cKeyboardPopup* self, int a1);
extern "C" void func_001CC7C8(cKeyboardPopup* self);
extern "C" void func_001CE5E0(cKeyboardPopup* self, int digit);
extern "C" void cKeyboardPopup_pressKey(cKeyboardPopup* self, int a1);
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
// func_0039F190 returns its last list node in $v0; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");

struct sVE_C1E0 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sUIObj_C1E0 {
    char pad_0x0[0x8];
    sVE_C1E0* vt;                   // 0x8
};

struct sColor_C1E0 {
    float r, g, b, a;
    sColor_C1E0(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};


struct sKeyboard_C1E0 {
    char pad_0x0[0x10];
    char* owner;                    // 0x10
    char pad_0x14[0xC];
    sUIObj_C1E0* parent;            // 0x20
    char pad_0x24[0x1C];
    void* screen;                   // 0x40
    char pad_0x44[0x10];
    int pendingAccept;              // 0x54
    int needText;                   // 0x58
    int done;                       // 0x5C
    int maxLen;                     // 0x60
    int minLen;                     // 0x64
    int caps;                       // 0x68
    int shift;                      // 0x6C
    char pad_0x70[0x4];
    char text[0xC0];                // 0x74
    int cursor;                     // 0x134
    int mark;                       // 0x138
    int end;                        // 0x13C
    int key;                        // 0x140
    char pad_0x144[0x2DC];
    int closeOnAccept;              // 0x420
};

/**
 * Presses the keyboard's selected key: edits the text or runs the key's action, then redraws.
 *
 * With mbClearOnFirst set, any key but enter and clear first presses "clear" (70). Then, unless only the
 * screen is updated or the key is not selectable, backspace (27) deletes before the cursor, caps (42) and
 * shift (56) toggle, clear (70) empties the text, the arrows (76, 83) move the cursor, enter (55) sets
 * mbAcceptText, pops the state when mbPopOnEnd and tells the parent (event 0xD), ESC (0) pops and sends 0xE,
 * quick keys 4-13 run func_001CE5E0 and other keys add their character (func_001CC7C8). Shift is released
 * after any other key; the edit field, the space and enter keys' availability and the key labels are
 * refreshed.
 *
 * @param self_  the keyboard (the body views it through a local struct)
 * @param flag  nonzero to only refresh the screen without pressing (On Tour: bScreenUpdateOnly)
 */
// Handles the currently selected key of the on-screen keyboard.
extern "C" void cKeyboardPopup_pressKey(cKeyboardPopup* self_, int flag)
{
    sKeyboard_C1E0* self = (sKeyboard_C1E0*)self_;
    int redo = 0;
    int moved;
    if (self->pendingAccept != 0) {
        int key = self->key;
        if (key != 0x37 && key != 0x46) {
            self->pendingAccept = 0;
            self->key = 0x46;
            cKeyboardPopup_pressKey((cKeyboardPopup*)self, 0);
            redo = 1;
            self->key = key;
        }
    }
    moved = 0;
    if (flag == 0 && cKeyboardPopup_allowSelection_out(self, self->key, 0) != 0) {
        switch (self->key) {
        case 27: {
            int len = strlen(self->text);
            if (self->cursor != len) {
                if (self->cursor == 0)
                    break;
                if (self->cursor > 0)
                    self->cursor--;
                strcpy(&self->text[self->cursor], &self->text[self->cursor + 1]);
                if (self->mark == self->cursor + 1) {
                    self->mark = self->cursor;
                    moved = 1;
                }
                if ((unsigned int)strlen(self->text) < (unsigned int)self->end)
                    self->end = strlen(self->text);
            } else if (self->mark == self->cursor) {
                self->mark = 0;
            } else {
                if (self->cursor > 0)
                    self->cursor--;
                self->text[self->cursor] = 0;
                self->end = self->cursor;
            }
            break;
        }
        case 28:
        case 69:
        case 79:
            break;
        case 42:
            self->caps ^= 1;
            cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
            break;
        case 56:
            self->shift ^= 1;
            cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
            break;
        case 70:
            func_00416210(self->text, 0, 0x40);
            self->text[0] = 0;
            self->cursor = 0;
            self->end = 0;
            self->mark = 0;
            break;
        case 76:
            if (self->cursor == self->mark) {
                if (self->cursor != 0)
                    self->mark = self->cursor - 1;
                moved = 1;
            }
            if (self->cursor > 0)
                self->cursor--;
            break;
        case 83: {
            int len = strlen(self->text);
            if (self->cursor != len) {
                if (self->cursor < self->maxLen - 1)
                    self->cursor++;
                if (self->end == self->cursor)
                    self->end++;
            }
            break;
        }
        case 55:
            if (self->closeOnAccept != 0)
                func_0039F190_r(self->owner + 0x18, 1);
            self->done = 1;
            self->parent->vt[19].fn((char*)self->parent + self->parent->vt[19].delta, 0, 0xD);
            break;
        case 75:
            if (cKeyboardPopup_allowSelection_out(self, 0x4B, 0) != 0)
                func_001CC7C8((cKeyboardPopup*)self);
            break;
        case 0:
            func_0039F190_r(self->owner + 0x18, 1);
            self->parent->vt[19].fn((char*)self->parent + self->parent->vt[19].delta, 0, 0xE);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            func_001CE5E0((cKeyboardPopup*)self, self->key - 4);
            break;
        default:
            func_001CC7C8((cKeyboardPopup*)self);
            break;
        }
    }
    if (self->key != 0x38 && self->shift != 0) {
        self->shift ^= 1;
        cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
    }
    cKeyboardPopup_updateEdit((cKeyboardPopup*)self, moved);
    if (self->needText != 0)
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 0x4B, strlen(self->text) != 0, 2);
    if (strlen(self->text) < self->minLen)
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 0x37, 0, 2);
    else
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 0x37, 1, 2);
    cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
    if (redo) {
        char name[32];
        cKeyboardPopup_getObjName((cKeyboardPopup*)self, 0x46, name, 1);
        sUIObj_C1E0* o = (sUIObj_C1E0*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (o != 0) {
            // PORT: takes the address of a temporary (accepted by g++ 2.95); use a named local elsewhere.
            o->vt[11].fn((char*)o + o->vt[11].delta, &sColor_C1E0(0.3f, 0, 0, 0));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC620);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

extern "C" void func_001CC620(cKeyboardPopup* self, int value)
{
    char c = value;
    char* buf = (char*)self + 0x74;
    int len = strlen(buf);
    int cur = self->miCursorPos;
    buf[cur] = c;
    if (cur == len) {
        if (self->miCursorPos == self->miMaxCharacters - 1) {
            buf[self->miMaxCharacters] = 0;
            self->miViewableEndIndex = self->miCursorPos + 1;
        } else {
            if (self->miCursorPos < self->miMaxCharacters - 1) {
                self->miCursorPos += 1;
            }
            buf[self->miCursorPos] = 0;
            self->miViewableEndIndex += 1;
        }
    } else {
        if (self->miCursorPos < self->miMaxCharacters - 1) {
            self->miCursorPos += 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CC6E8);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);
extern signed char D_00441390[];
extern signed char D_004413E8[];

extern "C" int func_001CC6E8(cKeyboardPopup* self, int value)
{
    char c = value;
    int i;
    if (c == 0x60) {
        self->miCurrentKey = 0xE;
        return 1;
    }
    for (i = 0; i < 0x54; i++) {
        if ((D_00441390[i] == c || D_004413E8[i] == c) && D_00441390[i] != '*'
            && (D_00441390[i] != '@' || D_004413E8[i] != D_00441390[i])) {
            if (cKeyboardPopup_allowSelection((cKeyboardPopup*)self, i, 0)) {
                self->miCurrentKey = i;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001CC7C8);
#ifdef SKIP_ASM
extern "C" void func_001CC620(cKeyboardPopup* self, int value);
extern signed char D_00441390[];
extern signed char D_004413E8[];

extern "C" void func_001CC7C8(cKeyboardPopup* self)
{
    bool b = self->mbCapsOn != 0;
    int flag = self->mbShiftOn;
    if (flag) {
        flag = !b;
    } else {
        flag = b != 0;
    }
    int idx = self->miCurrentKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    if (flag) {
        func_001CC620(self, D_00441390[idx]);
    } else {
        func_001CC620(self, D_004413E8[idx]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_updateEdit);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
struct sPwd_CCE98;
extern "C" void cKeyboardPopup_setPasswordText(sPwd_CCE98* self, char* str);
extern "C" void cKeyboardPopup_updateViewable(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_updateCursor(cKeyboardPopup* self);
extern char D_00466530[];

/**
 * Refreshes the keyboard's edit field: the visible part of the text, stars for passwords, and the cursor.
 *
 * @param bDropFromRight  passed to cKeyboardPopup_updateViewable(): trim the visible part from the right
 */
extern "C" void cKeyboardPopup_updateEdit(cKeyboardPopup* self, int bDropFromRight)
{
    cKeyboardPopup_updateViewable(self, bDropFromRight);
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466530));
    if (text != 0) {
        if (self->mbPassword != 0) {
            cKeyboardPopup_setPasswordText((sPwd_CCE98*)self, (char*)self + 0xB4);
            cUIText_setAsciiString(text, (char*)self + 0xF4);
        } else {
            cUIText_setAsciiString(text, (char*)self + 0xB4);
        }
    }
    cKeyboardPopup_updateCursor(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_updateViewable);
#ifdef SKIP_ASM
// cKeyboardPopup::updateViewable(bool bDropFromRight) (On Tour signature and locals).
extern "C" char* strncpy(char* dst, const char* src, int n);
extern "C" void cKeyboardPopup_setScale(cKeyboardPopup* self, void* obj);
extern char D_00466530[];  // "EnterValue"
extern char D_00466540[];  // "ExtendCursorLeft"
extern char D_00466558[];  // "ExtendCursorRight"

struct tRect_CC8D8 { float x, y, w, h; };

// cFont: Scale at 0x38; textExtent(str, rect, add) forwards the current scale.
struct cFont_CC8D8 {
    char pad0[0x38];
    float ScaleX, ScaleY;
    float textExtent(const char* str, tRect_CC8D8* rect, bool add)
    {
        return cFont_textExtent(this, str, rect, add, ScaleX, ScaleY);
    }
};

struct sVEntry_CC8D8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

class cUIThing_CC8D8 {
public:
    int f0;
    int f4;
    // vptr at 0x8; slot N at vtable offset N*8
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


/**
 * Works out which part of the edited text fits in the "EnterValue" box and copies it to mszViewableString.
 *
 * Trims characters from the right or the left until the measured width fits mvSizeEnter (less the cursor
 * width when the cursor is at the end), then extends the end while there is room and miMaxCharacters allows.
 * "ExtendCursorLeft" shows when the start is cut, "ExtendCursorRight" when text follows the visible part.
 *
 * @param bDropFromRight  nonzero to drop characters from the right end, 0 from the left
 */
// The body uses the incoming pointer directly (as `this` would be): a
// separate `self` local leaves a copy in the entry block that changes
// GCSE's expression numbering.
extern "C" void cKeyboardPopup_updateViewable(cKeyboardPopup* self, int bDropFromRight)
{
    cUIThing_CC8D8* pCursor;
    if (strlen((self)->mszEditString) != 0) {
        float fAllowableLength;
        void* pText = cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(D_00466530));
        tRect_CC8D8 rectString;
        int iCutLength = (self)->miViewableEndIndex - (self)->miViewableStartIndex;
        strncpy((self)->mszViewableString, (self)->mszEditString + (self)->miViewableStartIndex, iCutLength);
        (self)->mszViewableString[iCutLength] = 0;
        cKeyboardPopup_setScale(self, pText);
        ((cFont_CC8D8*)cUIText_getFont(pText))->textExtent((self)->mszViewableString, &rectString, false);
        fAllowableLength = (self)->mvSizeEnter[0];
        if ((self)->miCursorPos == strlen((self)->mszEditString)) {
            fAllowableLength -= (self)->mvSizeCursor[0];
            if (fAllowableLength < rectString.w) {
                if (bDropFromRight) {
                    fAllowableLength += (self)->mvSizeCursor[0];
                }
            }
        }
        while (fAllowableLength < rectString.w) {
            if (bDropFromRight) {
                (self)->miViewableEndIndex -= 1;
            } else {
                (self)->miViewableStartIndex += 1;
            }
            iCutLength = (self)->miViewableEndIndex - (self)->miViewableStartIndex;
            strncpy((self)->mszViewableString, (self)->mszEditString + (self)->miViewableStartIndex, iCutLength);
            (self)->mszViewableString[iCutLength] = 0;
            cKeyboardPopup_setScale(self, pText);
            ((cFont_CC8D8*)cUIText_getFont(pText))->textExtent((self)->mszViewableString, &rectString, false);
        }
        while (rectString.w < (self)->mvSizeEnter[0] - (self)->mvSizeCursor[0]) {
            if ((self)->miViewableEndIndex >= (self)->miMaxCharacters)
                break;
            iCutLength = ++(self)->miViewableEndIndex - (self)->miViewableStartIndex;
            strncpy((self)->mszViewableString, (self)->mszEditString + (self)->miViewableStartIndex, iCutLength);
            (self)->mszViewableString[iCutLength] = 0;
            cKeyboardPopup_setScale(self, pText);
            ((cFont_CC8D8*)cUIText_getFont(pText))->textExtent((self)->mszViewableString, &rectString, false);
        }
    } else {
        (self)->mszViewableString[0] = 0;
    }
    pCursor = (cUIThing_CC8D8*)cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(D_00466540));
    pCursor->setVisible((self)->miViewableStartIndex != 0);
    pCursor = (cUIThing_CC8D8*)cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(D_00466558));
    if (strlen((self)->mszEditString) != 0) {
        sVEntry_CC8D8* vt = *(sVEntry_CC8D8**)((char*)pCursor + 8);
        char* thisp = (char*)pCursor + vt[9].delta;
        vt[9].fn(thisp, (self)->miViewableEndIndex < (unsigned int)strlen((self)->mszEditString));
    } else {
        pCursor->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_updateCursor);
#ifdef SKIP_ASM
struct sVec3_CC30 { float x, y, z; };
struct sRect_CC30 {
    float x, y, w, h;
    void Measure(char* f, const char* str, int flag)
    {
        cFont_textExtent(f, str, this, flag, *(float*)(f + 0x38), *(float*)(f + 0x3C));
    }
};
struct sVEntry_CC30 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern "C" void cKeyboardPopup_setScale(cKeyboardPopup* self, void* obj);
extern "C" char* strncpy(char* dst, const char* src, int n);
extern char D_00466570[];

/**
 * Places and sizes the keyboard's "CharacterCursor" at the cursor position in the edit field.
 *
 * Refreshes the "EnterValue" size and position, sets the cursor's y so it ends 2 units above the box's
 * bottom, gives it the width of the character under it ('*' for a password; the default cursor size at the
 * end of the text) and moves it right by the width of the visible text before the cursor.
 */
extern "C" void cKeyboardPopup_updateCursor(cKeyboardPopup* self)
{
    sVec3_CC30 pos;

    void* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466530));
    char* t = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466530));
    if (t != 0) {
        sVEntry_CC30* vt = *(sVEntry_CC30**)(t + 8);
        vt[20].fn(t + vt[20].delta, ((char*)self) + 0x3F4);
        *(sVec3_CC30*)&self->mvPosEnter[0] = *(sVec3_CC30*)(t + 0x44);
    }
    self->mvPosCursor[1] = self->mvSizeEnter[1] + self->mvPosEnter[1] - self->mvSizeCursor[1] - 2.0f;
    unsigned char c = *(((char*)self) + self->miCursorPos + 0x74);
    char tc = *(((char*)self) + self->miCursorPos + 0x74);
    pos = *(sVec3_CC30*)&self->mvSizeCursor[0];
    if (tc != 0) {
        sRect_CC30 size;
        char ch[2];
        ch[1] = 0;
        if (self->mbPassword != 0) {
            ch[0] = '*';
        } else {
            ch[0] = c;
        }
        cKeyboardPopup_setScale(self, text);
        char* f = (char*)cUIText_getFont(text);
        size.Measure(f, ch, ch[0] == ' ');
        pos.x = size.w;
    }
    t = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466570));
    sVEntry_CC30* vt = *(sVEntry_CC30**)(t + 8);
    vt[21].fn(t + vt[21].delta, &pos);
    char buf[0x40];
    if (self->mbPassword != 0) {
        cKeyboardPopup_setPasswordText((sPwd_CCE98*)self, ((char*)self) + 0xB4);
        strncpy(buf, ((char*)self) + 0xF4, self->miCursorPos - self->miViewableStartIndex);
    } else {
        strncpy(buf, ((char*)self) + 0xB4, self->miCursorPos - self->miViewableStartIndex);
    }
    buf[self->miCursorPos - self->miViewableStartIndex] = 0;
    cKeyboardPopup_setScale(self, text);
    char* f = (char*)cUIText_getFont(text);
    sRect_CC30 size;
    size.Measure(f, buf, 1);
    sVec3_CC30 p = *(sVec3_CC30*)&self->mvPosCursor[0];
    p.x = size.w + self->mvPosEnter[0];
    *(sVec3_CC30*)(t + 0x44) = p;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setPasswordText);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

struct sPwd_CCE98 {
    char pad[0xF4];
    char buf[1];
};

/**
 * Fills the popup's text buffer with one '*' per character of the given password.
 *
 * Only the masked text is stored; @p str itself is not kept.
 *
 * @param str  password whose length sets the number of asterisks
 */
extern "C" void cKeyboardPopup_setPasswordText(sPwd_CCE98* self, char* str)
{
    int n = strlen(str);
    int i;
    for (i = 0; i < n; i++) {
        self->buf[i] = '*';
    }
    self->buf[n] = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CCF00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00466580[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBXString_cBXString4(void* self, const char* str);

extern "C" void func_001CCF00(cKeyboardPopup* self, const char* str)
{
    if (self->mpScreen == 0) {
        cBXString_cBXString4((char*)self + 0x424, str);
        return;
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466580));
    if (text != 0) {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setStatic);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
void cMemMan_free(void* ptr);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00466580[];
extern char D_00466590[];

/**
 * Sets the keyboard's static label ("StaticValue") from a unicode string.
 *
 * Before the screen exists, keeps a heap copy of @p str in mpustrStatic (freeing any earlier copy); afterwards
 * it sets the "StaticValue" text widget directly.
 *
 * @param str  NUL-terminated unicode label text
 */
extern "C" void cKeyboardPopup_setStatic(cKeyboardPopup* self, unsigned short* str)
{
    if (self->mpScreen == 0) {
        void* p = self->mpustrStatic;
        if (p != 0) {
            cMemMan_free(p);
            self->mpustrStatic = 0;
        }
        unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_00466590, 0x100, 0);
        self->mpustrStatic = copy;
        USTR_copy(copy, str);
    } else {
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466580));
        if (text != 0) {
            cUIText_setUnicodeString(text, str);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setStatic1);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_00466580[];

/**
 * Sets the keyboard's static label ("StaticValue") from a localised string id.
 *
 * Before the screen exists, stores @p id in muiStatic; afterwards it sets the "StaticValue" text widget by id.
 *
 * @param id  string id (hash of the text's key)
 */
extern "C" void cKeyboardPopup_setStatic1(cKeyboardPopup* self, int id)
{
    if (self->mpScreen == 0) {
        self->muiStatic = (unsigned int)id;
    } else {
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466580));
        if (text != 0) {
            cUIText_setUnicodeStringByID(text, id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setMaxCharacters);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_setMaxCharacters(cKeyboardPopup* self, int v)
{
    if (v < 0) {
        self->miMaxCharacters = 0;
        return;
    }
    if (v >= 0x40) {
        self->miMaxCharacters = 0x3f;
        return;
    }
    self->miMaxCharacters = v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CD0B0);
#ifdef SKIP_ASM
extern "C" void func_001CD0B0(cKeyboardPopup* self, int v)
{
    if (v < 0) {
        self->miMinCharacters = 0;
        return;
    }
    if (v >= 0x40) {
        self->miMinCharacters = 0x3f;
        return;
    }
    self->miMinCharacters = v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_getObjName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sName8_1CD0E8 { char s[8]; };
struct sName10_1CD0E8 { char s[10]; };
extern char D_004A1368[];
extern sName10_1CD0E8 D_0045D550_1CD0E8 __asm__("D_0045D550");
extern char D_004A12A8[];
extern char D_004A1C80[];
extern char D_004A1C88[];
extern char D_004A1C90[];
extern char D_004A1350[];
extern char D_0045D560[];
extern char D_004A1C98[];
extern char D_004A1CA0[];
extern char D_004A1CA8[];
extern char D_004A1CB0[];
extern char D_004A1CB8[];
extern char D_004A1CC0[];
extern char D_004A1CC8[];

/**
 * Builds the UI object name of a keyboard key or of its cursor highlight.
 *
 * The name is "keyboard_" or "cursor_" plus the key's name: "back", "caps", "Lshift", "clear", "arrLeft",
 * "arrRight", "tab", "enter", "arrUp", "arrDown", "space" and "ESC" for the special keys, "Q1" to "Q10" for
 * keys 4-13, otherwise the key's character from D_00441390.
 *
 * @param key  the key index, 0-83
 * @param out  receives the name
 * @param shifted  nonzero for the cursor object's name (On Tour: bCursor), 0 for the key's
 * @return 0 for a gap in the layout (character '*'), else 1
 */
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int key, char* out, int shifted)
{
    char num[16];
    char ch[2];
    if (shifted != 0) {
        *(sName8_1CD0E8*)out = *(sName8_1CD0E8*)D_004A1368;
    } else {
        *(sName10_1CD0E8*)out = D_0045D550_1CD0E8;
    }
    switch (key) {
    case 27: func_004162D0(out, D_004A12A8); break;
    case 42: func_004162D0(out, D_004A1C80); break;
    case 56: func_004162D0(out, D_004A1C88); break;
    case 70: func_004162D0(out, D_004A1C90); break;
    case 76: func_004162D0(out, D_004A1350); break;
    case 83: func_004162D0(out, D_0045D560); break;
    case 28: func_004162D0(out, D_004A1C98); break;
    case 55: func_004162D0(out, D_004A1CA0); break;
    case 69: func_004162D0(out, D_004A1CA8); break;
    case 79: func_004162D0(out, D_004A1CB0); break;
    case 75: func_004162D0(out, D_004A1CB8); break;
    case 0: func_004162D0(out, D_004A1CC0); break;
    case 4: case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12: case 13:
        sprintf(num, D_004A1CC8, key - 3);
        func_004162D0(out, num);
        break;
    default:
        if (D_00441390[key] == '*') {
            return 0;
        }
        ch[0] = D_00441390[key];
        ch[1] = 0;
        func_004162D0(out, ch);
        break;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_updateCursor(cKeyboardPopup* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);

/**
 * Keeps the keyboard's text cursor inside the text when the keyboard regains focus.
 *
 * Steps miCursorPos back by one when it sits at miMaxCharacters, redraws the cursor with
 * cKeyboardPopup_updateCursor(), then runs the base cUIState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before (On Tour: pPrevState), passed on to cUIState_onGainFocus()
 */
extern "C" void cKeyboardPopup_onGainFocus(cKeyboardPopup* self, int pPrevState)
{
    if (self->miCursorPos == self->miMaxCharacters) {
        self->miCursorPos = self->miCursorPos - 1;
    }
    cKeyboardPopup_updateCursor(self);
    cUIState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onGainTransition);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self);
extern "C" void cKeyboardPopup_setStatic(cKeyboardPopup* self, unsigned short* str);
extern "C" void cKeyboardPopup_setInputText(cKeyboardPopup* self, const char* str);
extern "C" void func_001CCF00(cKeyboardPopup* self, const char* str);
extern "C" void cKeyboardPopup_setStatic1(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_updateCursor(cKeyboardPopup* self);
extern "C" void cBXString_Reset(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cMemMan_free(void* ptr);
extern void* D_004A33F0;
extern char D_004A1CD0[];
extern char D_004A1CD8[];
extern char D_00466700[];
extern char D_00466570[];
extern char D_00466530[];
extern char D_00466710[];
extern char D_00466540[];
extern char D_00466558[];
extern char D_004A1408[];

struct sVec3_D348 { float x, y, z; };

struct sVEi_D348 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVEp_D348 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


static inline void SetVis_D348(char* o, int on)
{
    sVEi_D348* vt = *(sVEi_D348**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

/**
 * Prepares the keyboard the first time it is shown: layout, measurements and the initial texts.
 *
 * Sets mbTransistionDone, turns caps on (or takes bit 1 of D_004A33F0 + 0x88 when that exists) and shift off,
 * clears the text unless one was set, shows the background for meType (0 "regular", 2 "expanded", else
 * "compact") and plays that label's frame. It measures "CharacterCursor" and "EnterValue" (offset by "Name
 * bar"), hides the extend arrows, re-applies a pending input text and the pending static text (string id,
 * unicode copy or mszStatic), and places the cursor. Later calls do nothing.
 *
 * @param first  nonzero on the first call (On Tour: bFirstTime)
 * @return always 1 (the transition finishes at once)
 */
extern "C" int cKeyboardPopup_onGainTransition(cKeyboardPopup* self, int first)
{
    if (first) {
        char buf[16];
        (self)->mbTransistionDone = 1;
        (self)->mbCapsOn = 1;
        if (D_004A33F0 != 0) {
            (self)->mbCapsOn = (*(unsigned int*)((char*)D_004A33F0 + 0x88) >> 1) & 1;
        }
        self->mbShiftOn = 0;
        if (self->mbSetInputText == 0) {
            ((char*)self)[0x74] = 0;
        }
        self->miViewableStartIndex = 0;
        self->miViewableEndIndex = 0;
        self->miCursorPos = 0;
        int a = 0;
        int c = 0;
        int b = 0;
        cKeyboardPopup_redrawScreen(self);
        switch (self->meType) {
        case 0:
            c = 1;
            sprintf(buf, D_004A1CD0);
            break;
        case 2:
            b = 1;
            sprintf(buf, D_00466700);
            break;
        case 1:
        default:
            a = 1;
            sprintf(buf, D_004A1CD8);
            break;
        }
        char* oa = (char*)self->mpBackCompact;
        if (oa != 0) {
            SetVis_D348(oa, a);
        }
        char* ob = (char*)self->mpBackExpand;
        if (ob != 0) {
            SetVis_D348(ob, b);
        }
        char* oc = (char*)self->mpBackRegular;
        if (oc != 0) {
            SetVis_D348(oc, c);
        }
        int f = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(buf));
        if (f != 0xFFFF) {
            cUIScreen_playFrame(self->mpScreen, f, 1);
        }
        char* o1 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466570));
        if (o1 != 0) {
            sVEp_D348* vt = *(sVEp_D348**)(o1 + 8);
            vt[20].fn(o1 + vt[20].delta, ((char*)self) + 0x400);
            *(sVec3_D348*)&self->mvPosCursor[0] = *(sVec3_D348*)(o1 + 0x44);
        }
        char* o2 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466530));
        if (o2 != 0) {
            sVEp_D348* vt = *(sVEp_D348**)(o2 + 8);
            vt[20].fn(o2 + vt[20].delta, ((char*)self) + 0x3F4);
            *(sVec3_D348*)&self->mvPosEnter[0] = *(sVec3_D348*)(o2 + 0x44);
            self->mvPosCursor[1] = self->mvSizeEnter[1] + self->mvPosEnter[1];
        }
        char* o3 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466710));
        if (o3 != 0) {
            sVec3_D348 p = *(sVec3_D348*)(o3 + 0x44);
            self->mvPosEnter[0] += p.x;
            self->mvPosEnter[1] += p.y;
            self->mvPosEnter[2] += p.z;
        }
        char* o4 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466540));
        if (o4 != 0) {
            SetVis_D348(o4, 0);
        }
        char* o5 = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_00466558));
        if (o5 != 0) {
            SetVis_D348(o5, 0);
        }
        if (self->mbSetInputText != 0) {
            cKeyboardPopup_setInputText(self, ((char*)self) + 0x74);
        }
        if ((int)self->muiStatic != 0) {
            cKeyboardPopup_setStatic1(self, (int)self->muiStatic);
            self->muiStatic = (unsigned int)0;
        } else if ((int)self->mpustrStatic != 0) {
            cKeyboardPopup_setStatic(self, self->mpustrStatic);
            if (self->mpustrStatic != 0) {
                cMemMan_free(self->mpustrStatic);
            }
            self->mpustrStatic = 0;
        } else {
            char** str = (char**)(((char*)self) + 0x424);
            if (*(int*)(*str - 8) != 0) {
                func_001CCF00((cKeyboardPopup*)(((char*)self)), *str);
                cBXString_Reset(str);
            } else {
                func_001CCF00((cKeyboardPopup*)(((char*)self)), D_004A1408);
            }
        }
        cKeyboardPopup_updateCursor(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_redrawScreen);
#ifdef SKIP_ASM
static inline bool Neg_D708(bool b) { return !b; }
static inline bool Shift_D708(char* s) { return *(bool*)(s + 0x6C); }
struct cUIText;
struct sVEntry_D708 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int i, char* out, int mode);
extern "C" void cKeyboardPopup_SetNameForKey(cKeyboardPopup* self, cUIText* text, int i);
extern "C" void cKeyboardPopup_checkAndFixConflict(cKeyboardPopup* self);
extern signed char D_00441390[];
extern signed char D_004413E8[];
extern char D_004C67B8[];
extern char D_004C67C8[];
extern char D_004C67E8[];
extern char D_004C67F8[];

/**
 * Redraws every key of the keyboard: labels, availability colours, cursor objects and the highlight.
 *
 * Special keys get their labels from cKeyboardPopup_SetNameForKey(); character keys show the upper or lower
 * character (caps xor shift; keys 15-26 follow shift alone) and are coloured by
 * cKeyboardPopup_allowSelection(). Cursor objects of keys 0-13 are hidden in the compact layout (meType 1).
 * The current key's cursor gets the highlight colour D_004C67E8. Does nothing without a screen.
 */
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self)
{
    char name[0x20];
    char ch[2];
    if (self->mpScreen == 0) {
        return;
    }
    ch[1] = 0;
    for (int i = 0; i < 0x54; i++) {
        if (cKeyboardPopup_getObjName(self, i, name, 0) == 0) {
            continue;
        }
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
        if (D_00441390[i] == '@' || D_00441390[i] == ' ') {
            cKeyboardPopup_SetNameForKey(self, text, i);
        } else {
            int caps = self->mbCapsOn != 0;
            bool upper;
            if (Shift_D708(((char*)self))) {
                upper = Neg_D708(caps);
            } else {
                upper = caps;
            }
            if (i >= 0xF && i < 0x1B) {
                upper = !Shift_D708(((char*)self));
            }
            cKeyboardPopup_checkAndFixConflict(self);
            if (upper) {
                ch[0] = D_00441390[i];
            } else {
                ch[0] = D_004413E8[i];
            }
            if (text != 0) {
                cUIText_setAsciiString(text, ch);
                if (cKeyboardPopup_allowSelection(self, i, 0) == 0) {
                    sVEntry_D708* vt = *(sVEntry_D708**)((char*)text + 8);
                    vt[11].fn((char*)text + vt[11].delta, D_004C67B8);
                } else {
                    sVEntry_D708* vt = *(sVEntry_D708**)((char*)text + 8);
                    vt[11].fn((char*)text + vt[11].delta, D_004C67C8);
                }
            }
        }
        cKeyboardPopup_getObjName(self, i, name, 1);
        char* o = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
        if (o != 0) {
            if (self->meType == 1) {
                if (i >= 0 && i < 0xE) {
                    sVEntry_D708* vt = *(sVEntry_D708**)(o + 8);
                    vt[9].fn(o + vt[9].delta, (void*)0);
                } else {
                    sVEntry_D708* vt = *(sVEntry_D708**)(o + 8);
                    vt[9].fn(o + vt[9].delta, (void*)1);
                }
            } else {
                {
                    sVEntry_D708* vt = *(sVEntry_D708**)(o + 8);
                    vt[11].fn(o + vt[11].delta, D_004C67F8);
                }
                {
                    sVEntry_D708* vt = *(sVEntry_D708**)(o + 8);
                    vt[9].fn(o + vt[9].delta, (void*)1);
                }
            }
        }
    }
    cKeyboardPopup_getObjName(self, self->miCurrentKey, name, 1);
    char* o = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    if (o != 0) {
        sVEntry_D708* vt = *(sVEntry_D708**)(o + 8);
        vt[11].fn(o + vt[11].delta, D_004C67E8);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_SetNameForKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup* self, int key, int a2);
extern char D_004C67B8[];
extern char D_004C67C8[];
extern char D_004A1CC8[];
extern char D_00466720[];
extern char D_00466740[];
extern char D_00466760[];
extern char D_00466778[];
extern char D_00466790[];
extern char D_004667A8[];
extern char D_004667C0[];
extern char D_004667D8[];

struct sTextVE_1CD9B8 {
    short delta;
    short index;
    void* fn;
};

static inline void SetColor_1CD9B8(cUIText* text, void* col)
{
    sTextVE_1CD9B8* vt = *(sTextVE_1CD9B8**)((char*)text + 8);
    ((int (*)(void*, void*))vt[11].fn)((char*)text + vt[11].delta, col);
}

static inline void SetVisible_1CD9B8(cUIText* text, int on)
{
    sTextVE_1CD9B8* vt = *(sTextVE_1CD9B8**)((char*)text + 8);
    ((void (*)(void*, int))vt[9].fn)((char*)text + vt[9].delta, on);
}

/**
 * Sets the label text and colour of one on-screen keyboard key.
 *
 * Colours the label by whether cKeyboardPopup_allowSelection() accepts the key (caps lock and shift always use
 * the selectable colour). Special keys get localised names: 0 escape, 27 backspace, 28 tab, 42 caps lock, 55
 * enter, 56 shift, 70 clear, 75 space; keys 4-13 read "Q1" to "Q10". The escape key and keys 4-13 are hidden on
 * the compact keyboard (meType 1). Other keys only get the colour; a null @p text does nothing.
 *
 * @param vself  the keyboard popup
 * @param text   the key's text widget; may be 0
 * @param key    key index, 0-83
 */
extern "C" void cKeyboardPopup_SetNameForKey(cKeyboardPopup* vself, cUIText* text, int key)
{
    char buf[16];
    if (text == 0) {
        return;
    }
    if (cKeyboardPopup_allowSelection(vself, key, 0) == 0) {
        SetColor_1CD9B8(text, D_004C67B8);
    } else {
        SetColor_1CD9B8(text, D_004C67C8);
    }
    switch (key) {
    case 27:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00466720));
        break;
    case 42:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00466740));
        SetColor_1CD9B8(text, D_004C67C8);
        break;
    case 56:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00466760));
        SetColor_1CD9B8(text, D_004C67C8);
        break;
    case 70:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00466778));
        break;
    case 28:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00466790));
        break;
    case 55:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004667A8));
        break;
    case 75:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004667C0));
        break;
    case 0:
        if (vself->meType == 1) {
            SetVisible_1CD9B8(text, 0);
        } else {
            SetVisible_1CD9B8(text, 1);
        }
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004667D8));
        break;
    case 4: case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12: case 13:
        if (vself->meType == 1) {
            SetVisible_1CD9B8(text, 0);
        } else {
            SetVisible_1CD9B8(text, 1);
        }
        sprintf(buf, D_004A1CC8, key - 3);
        cUIText_setAsciiString(text, buf);
        break;
    case 83:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onInputBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int key, char* out, int a3);
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cKeyboardPopup_kbMoveLeft(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_kbMoveRight(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_kbMoveUp(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_kbMoveDown(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_pressKey(cKeyboardPopup* self, int a1);
extern "C" void func_001CC620(cKeyboardPopup* self, int value);
extern "C" bool func_001CC6E8_b(void* self, int value) __asm__("func_001CC6E8");
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);
extern "C" void* func_001D9D48(void* mem, void* engine, void* owner);
void func_001DA510(void* self, int i, int a, int b);
void func_001DA528(void* self, int val);
void func_001DA530(void* self, int i, int value);
extern "C" void func_0039F190(void* self, int a1);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern int D_004A1A70;
extern char D_00461A70[];
extern char D_00466940[];
extern char D_00466950[];
extern char D_00466968[];
extern char D_00466988[];
extern char D_004669A0[];
extern char D_004669B8[];
extern char D_004669D8[];

struct sVEntryK1CDB98 {
    short delta;
    short index;
    void* fn;
};
struct sColorK1CDB98 {
    float a, r, g, b;
    sColorK1CDB98(float a_, float r_, float g_, float b_) : a(a_), r(r_), g(g_), b(b_) {}
};
class cUIObjK1CDB98 {
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
    virtual void SetColor(const sColorK1CDB98& c);
};
struct sSndK1CDB98 {
    sVEntryK1CDB98* vt;
};
extern sSndK1CDB98 D_004A5A58;

typedef int (*FnIK1CDB98)(void*);

static inline int InputK1CDB98(char* ev, int slot)
{
    sVEntryK1CDB98* vt = *(sVEntryK1CDB98**)(ev + 8);
    return ((FnIK1CDB98)vt[slot].fn)(ev + vt[slot].delta);
}

static inline int isOnK1CDB98()
{
    return D_004A1A70 == 1;
}

/**
 * Handles the keyboard's input: moves, key presses, caps, shift, help, cancel and typed characters.
 *
 * Ignores input with slot 8 set. The four directions move the selection (cKeyboardPopup_kbMoveLeft, cKeyboardPopup_kbMoveRight,
 * cKeyboardPopup_kbMoveUp, cKeyboardPopup_kbMoveDown()), accept presses the key, back pops the keyboard and sends 0xE
 * to the parent, and online outside a game the help input opens a help popup. Keys from a typed-key event
 * (slots 41-43) select the matching special key, or add the typed character (` becomes ~) and refresh. The
 * old and new current keys' cursors are recoloured and a sound plays when something changed.
 *
 * @param ev  the input object (On Tour: cUIInput *pInput)
 * @return always 1 (input consumed)
 */
extern "C" int cKeyboardPopup_onInputBegin(cKeyboardPopup* self, char* ev)
{
    if (InputK1CDB98(ev, 8) != 0)
        return 1;
    {
        char name[32];
        int changed = 0;
        int sound = 1;
        cKeyboardPopup_getObjName(self, self->miCurrentKey, name, 1);
        {
            cUIObjK1CDB98* o = (cUIObjK1CDB98*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
            if (o != 0) {
                o->SetColor(sColorK1CDB98(0.3f, 0.0f, 0.0f, 0.0f));
            }
        }
        if (InputK1CDB98(ev, 19)) {
            changed = 1;
            cKeyboardPopup_kbMoveLeft(self, 1);
        } else if (InputK1CDB98(ev, 20)) {
            changed = 1;
            cKeyboardPopup_kbMoveRight(self, 1);
        } else if (InputK1CDB98(ev, 17)) {
            changed = 1;
            cKeyboardPopup_kbMoveUp(self, 1);
        } else if (InputK1CDB98(ev, 18)) {
            changed = 1;
            cKeyboardPopup_kbMoveDown(self, 1);
        } else if (InputK1CDB98(ev, 5)) {
            changed = 1;
            cKeyboardPopup_pressKey(self, 0);
        } else if (InputK1CDB98(ev, 7)) {
            if (isOnK1CDB98()) {
                if ((int)SSXApp->Game == 0) {
                    changed = 1;
                    void* p = func_001D9D48(cMemMan_alloc(0x140, D_00461A70, 0x100, 0), self->mpEngine, self);
                    func_001DA528(p, 6);
                    *(int*)((char*)p + 0x54) = GetHashValue32(D_00466940);
                    func_001DA530(p, 0, GetHashValue32(D_00466950));
                    func_001DA510(p, 0, GetHashValue32(D_00466968), 4);
                    func_001DA510(p, 1, GetHashValue32(D_00466968), 3);
                    func_001DA510(p, 2, GetHashValue32(D_00466988), 5);
                    func_001DA510(p, 3, GetHashValue32(D_004669A0), 6);
                    func_001DA510(p, 4, GetHashValue32(D_004669B8), 0);
                    func_001DA510(p, 5, GetHashValue32(D_004669D8), 1);
                    cUIStateStack_pushPopup((char*)self->mpEngine + 0x18, p);
                }
                sound = 5;
            }
        } else if (InputK1CDB98(ev, 6)) {
            changed = 1;
            sound = 6;
            func_0039F190((char*)self->mpEngine + 0x18, 1);
            char* o = (char*)self->mpParentState;
            sVEntryK1CDB98* vt = *(sVEntryK1CDB98**)(o + 8);
            ((void (*)(void*, int, int))vt[19].fn)(o + vt[19].delta, 0, 0xE);
        } else if (InputK1CDB98(ev, 25)) {
            changed = 1;
            self->mbCapsOn ^= 1;
            cKeyboardPopup_redrawScreen(self);
        } else if (InputK1CDB98(ev, 27)) {
            changed = 1;
            self->mbShiftOn ^= 1;
            cKeyboardPopup_redrawScreen(self);
        } else {
            int fail = 0;
            if (InputK1CDB98(ev, 41)) {
                int typed = 0;
                switch (InputK1CDB98(ev, 42)) {
                case 8:
                    if (cKeyboardPopup_allowSelection(self, 0x1B, 0)) self->miCurrentKey = 0x1B;
                    else fail = 1;
                    break;
                case 0xD:
                    self->miCurrentKey = 0x37;
                    break;
                case 0x10:
                    if (cKeyboardPopup_allowSelection(self, 0x38, 0)) self->miCurrentKey = 0x38;
                    else fail = 1;
                    break;
                case 0x14:
                    if (cKeyboardPopup_allowSelection(self, 0x2A, 0)) self->miCurrentKey = 0x2A;
                    else fail = 1;
                    break;
                case 0x1B:
                    if (cKeyboardPopup_allowSelection(self, 0, 0)) self->miCurrentKey = 0;
                    else fail = 1;
                    break;
                case 0x25:
                    if (cKeyboardPopup_allowSelection(self, 0x4C, 0)) self->miCurrentKey = 0x4C;
                    else fail = 1;
                    break;
                case 0x27:
                    if (cKeyboardPopup_allowSelection(self, 0x53, 0)) self->miCurrentKey = 0x53;
                    else fail = 1;
                    break;
                case 0x26:
                    if (cKeyboardPopup_allowSelection(self, 0x45, 0)) self->miCurrentKey = 0x45;
                    else fail = 1;
                    break;
                case 0x28:
                    if (cKeyboardPopup_allowSelection(self, 0x4F, 0)) self->miCurrentKey = 0x4F;
                    else fail = 1;
                    break;
                case 9:
                    if (cKeyboardPopup_allowSelection(self, 0x1C, 0)) self->miCurrentKey = 0x1C;
                    else fail = 1;
                    break;
                case 0x70: case 0x71: case 0x72: case 0x73: case 0x74:
                case 0x75: case 0x76: case 0x77: case 0x78: case 0x79: {
                    int key = InputK1CDB98(ev, 42) - 0x6C;
                    if (cKeyboardPopup_allowSelection(self, key, 0)) self->miCurrentKey = key;
                    else fail = 1;
                    break;
                }
                default:
                    if (InputK1CDB98(ev, 43) >= 0x20) {
                        char c = InputK1CDB98(ev, 43);
                        fail = !func_001CC6E8_b(self, c);
                        if (!fail) {
                            if (cKeyboardPopup_allowSelection(self, self->miCurrentKey, 0)) {
                                if (c == 0x60) func_001CC620(self, 0x7E);
                                else func_001CC620(self, c);
                            } else {
                                fail = 1;
                            }
                        }
                    }
                    typed = 1;
                    break;
                }
                if (!fail) {
                    changed = 1;
                    cKeyboardPopup_pressKey(self, typed);
                }
            }
        }
        cKeyboardPopup_getObjName(self, self->miCurrentKey, name, 1);
        {
            cUIObjK1CDB98* o = (cUIObjK1CDB98*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
            if (o != 0) {
                o->SetColor(sColorK1CDB98(1.0f, 0.0f, 0.0f, 0.0f));
            }
        }
        sSndK1CDB98* snd = *(sSndK1CDB98**)((char*)self->mpEngine + 0x14);
        if (snd == 0)
            snd = &D_004A5A58;
        if (changed && snd != 0)
            ((void (*)(void*, int))snd->vt[2].fn)((char*)snd + snd->vt[2].delta, sound);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_fillSelectableKeys);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int v, int mode);
extern "C" void func_001CE468(void* self, int a1);
extern "C" int strlen(const char* s);
extern signed char D_00441390[];

extern "C" void cKeyboardPopup_fillSelectableKeys(cKeyboardPopup* self)
{
    int i;
    for (i = 0; i < 0x54; i++) {
        if (D_00441390[i] == '*' || i == 0x1C) {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else if (i == 0x45 || i == 0x4F || i == 0x29) {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else if (i == 0x4B) {
            if (strlen((char*)self + 0x74) != 0 && self->mbDisableForFirst != 0)
                cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 1, 2);
            else
                cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 1, 2);
        }
    }
    func_001CE468(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setIncludeKey);
#ifdef SKIP_ASM
/**
 * Sets whether a key can be selected, in the regular set, the alternate set or both.
 *
 * @param i  the key index, 0-83 (On Tour: eScreenFullKeyboard eCharacter)
 * @param bIncludeKey  nonzero to allow the key
 * @param mode  0 regular (mbRegular), 1 alternate (mbAlternate), 2 both (On Tour: eKeyStateType)
 */
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int bIncludeKey, int mode)
{
    if (mode == 0 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x148) = bIncludeKey;
    }
    if (mode == 1 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x298) = bIncludeKey;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE408);
#ifdef SKIP_ASM
extern "C" void func_001CE408(void* self, bool on)
{
    int i;
    for (i = 0xF; i < 0x19; i++) {
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, !on, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE468);
#ifdef SKIP_ASM
extern "C" void func_001CE468(void* self, int v)
{
    int i;
    for (i = 0; i < 0xE; i++) {
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, v, 2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setScale);
#ifdef SKIP_ASM
struct sVec3_CE4C8 { float x, y, z; };

extern "C" void* cUIText_getFont(void* self);

/**
 * Sets a text object's font to its current scale: the font's base size times the text's scale.
 *
 * @param obj  the text object (On Tour: cUIText *pText)
 */
extern "C" void cKeyboardPopup_setScale(cKeyboardPopup* self, void* obj)
{
    sVec3_CE4C8 v = *(sVec3_CE4C8*)((char*)obj + 0x50);
    char* p = (char*)cUIText_getFont(obj);
    float x = *(float*)(p + 0x30) * v.x;
    float y = *(float*)(p + 0x34) * v.y;
    *(float*)(p + 0x38) = x;
    *(float*)(p + 0x3C) = y;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_checkAndFixConflict);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);
extern "C" void cKeyboardPopup_kbMoveDown(cKeyboardPopup*, int);

/**
 * Moves the keyboard cursor off its key when that key cannot be selected in the current caps/shift state.
 *
 * When cKeyboardPopup_allowSelection() rejects miCurrentKey, calls cKeyboardPopup_kbMoveDown(self, 0).
 * cKeyboardPopup_redrawScreen() calls it while redrawing the keys.
 */
extern "C" void cKeyboardPopup_checkAndFixConflict(cKeyboardPopup* self)
{
    if (cKeyboardPopup_allowSelection(self, self->miCurrentKey, 0) == 0) {
        cKeyboardPopup_kbMoveDown(self, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_allowSelection);
#ifdef SKIP_ASM
/**
 * Tells whether a keyboard key can be selected in the current caps lock and shift state.
 *
 * The regular key set applies when caps lock XOR shift is on (for keys 15-26, when shift is off); otherwise the
 * alternate set applies. The key's flag in the other set is written to @p out.
 *
 * @param idx  key index, 0-83
 * @param out  address of an int that receives the key's flag in the other set (On Tour: bOtherSet); may be 0
 * @return nonzero when the key is included in the set that applies
 */
// PORT: out is an int* out-param (the unit declares it int)
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup* self, int idx, int out)
{
    bool b = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = b;
        flag = !flag;
    } else {
        flag = b != 0;
    }
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    if (flag) {
        if (out)
            *(int*)out = (self)->mbAlternate[idx];
        return (self)->mbRegular[idx];
    } else {
        if (out)
            *(int*)out = (self)->mbRegular[idx];
        return (self)->mbAlternate[idx];
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE5E0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* func_0014F6A8(void* self, int i);
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int value);
extern "C" void USTR_strcpyW(char* dst, void* src);
extern "C" char* func_004162D0(char* dst, const char* src);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);

struct sVEntryK1CE5E0 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern "C" void func_001CE5E0(cKeyboardPopup* self, int key)
{
    char buf[0x40];
    int room = self->miMaxCharacters - strlen((char*)self + 0x74);
    void* opt = cBE_getInterface_Fv(cBE_getBE(), 4);
    char* msg = func_0014F6A8(opt, key);
    if (msg == 0) {
        char* o = (char*)SSXApp->FELocale;
        sVEntryK1CE5E0* vt = *(sVEntryK1CE5E0**)(o + 4);
        void* str = vt[4].fn(o + vt[4].delta, cBEOptionInterface_getDefaultQuickKeyMessageHashValue(opt, key));
        if (str)
            USTR_strcpyW(buf, str);
        else
            buf[0] = 0;
    } else {
        strcpy(buf, msg);
    }
    buf[room] = 0;
    func_004162D0((char*)self + 0x74, buf);
    int len = strlen((char*)self + 0x74);
    int n = len - 1;
    self->miCursorPos = n;
    if (n < self->miMaxCharacters - 1)
        self->miCursorPos = len;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CE6F0);
#ifdef SKIP_ASM
struct sPopupSlot_E6F0 {
    signed char id;
    signed char pad;
};

extern sPopupSlot_E6F0 D_0045AAD8[][4][5];

extern "C" int func_001CE6F0(int a, int b)
{
    int n = 0;
    for (int i = 0; i < 5; i++) {
        if (D_0045AAD8[a][b][i].id >= 0) {
            n++;
        } else {
            break;
        }
    }
    return n;
}
#endif

