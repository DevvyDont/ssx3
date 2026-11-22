#include "vtable.h"
#include "common.h"
#include "online/usbkeyboard.h"
#include "util/locale.h"
#include "render/font.h"
#include "render/2drender.h"
#include "bx/math3d.h"
#include "fe/popupinfo.h"
#include "ui/uiinput.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "fe/buypopupinfo.h"
#include "fe/uistatebuypopup.h"
#include "online/gamecomm.h"
#include "fe/keyboardpopup.h"
#include "fe/screenpopup.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_cScreenPopup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cPopupInfo_cPopupInfo(void* self);
extern "C" void memset(void* dst, int c, int n);
extern void* cScreenPopup_vtbl[];
extern int cBXString_EmptyString;
struct sVec3_58E8 {
    float x, y, z;
    sVec3_58E8(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sQuad_58E8 { int v[4]; };
extern sQuad_58E8 gPopupWhite;
extern sQuad_58E8 gPopupBlack;

/**
 * Constructs a message/confirmation popup state with empty widgets and default layout settings.
 *
 * Runs the cUIState constructor (cUIState_cUIState1), stores cScreenPopup_vtbl, constructs the embedded cPopupInfo
 * (cPopupInfo_cPopupInfo at +0xBC) and points mszInputText and mszStaticText at the empty string cBXString_EmptyString. Every
 * widget pointer, mpKeyboard, the five options and the five confirmations are cleared; miDefaultOption is -1,
 * mbReceivePrev 1, mvCurrentScale (1, 1, 1), the highlight colours come from gPopupWhite / gPopupBlack and
 * miMaxChars is 0x3F.
 *
 * @param engine  the UI engine that owns the state
 * @param owner  the parent state that receives the popup's events (mpParentState)
 * @return @p self
 */
extern "C" void* cScreenPopup_cScreenPopup(cScreenPopup* self, cUIEngine* engine, cUIState* owner)
{
    cUIState_cUIState1(self, engine, owner);
    self->mVtbl = cScreenPopup_vtbl;
    cPopupInfo_cPopupInfo((char*)&self->info);
    int defaultStr = cBXString_EmptyString;
    self->mszInputText = (char*)defaultStr;
    self->mszStaticText = (char*)defaultStr;
    self->miDefaultOption = -1;
    self->mpKeyboard = 0;
    self->mbShowing = 0;
    self->mbFirstGainTran = 0;
    self->meType = 0;
    self->mbPassUpdate = 0;
    self->miLayerOffset = 0;
    for (int j = 0; j < 5; j++)
        memset((char*)&self->mtConfirmation + j * 12, 0, 12);
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
        ((int*)&self->mpOptions)[i] = 0;
    self->meBackgroundType = 0;
    *(sVec3_58E8*)&self->mvCurrentScale[0] = sVec3_58E8(1.0f, 1.0f, 1.0f);
    self->mbCenterOptions = 0;
    self->mbCenterText = 0;
    *(sQuad_58E8*)&self->mtHighlightedColor[0] = gPopupWhite;
    *(sQuad_58E8*)&self->mtUnhighlightColor[0] = gPopupBlack;
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
extern "C" void cUIState__cUIState(void* self, int flags);

/**
 * Destroys a screen popup: its two strings and popup info, then the cUIState base.
 *
 * Restores cScreenPopup_vtbl, destroys mszStaticText and mszInputText (cBXString destructor, flags 2) and the
 * embedded cPopupInfo (cPopupInfo__cPopupInfo, flags 2), then calls the cUIState destructor cUIState__cUIState(self, flags).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScreenPopup__cScreenPopup(cScreenPopup* self, int flags)
{
    self->mVtbl = cScreenPopup_vtbl;
    cBXString__cBXString((char*)&self->mszStaticText, 2);
    cBXString__cBXString((char*)&self->mszInputText, 2);
    cPopupInfo__cPopupInfo((char*)&self->info, 2);
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_popup[];
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
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_popup), 0);
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
void* cScreenPopup_onDestroyScreen(void* vself)
{
    cScreenPopup* self = (cScreenPopup*)vself;
    self->mbShowing = 0;
    return cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onGainFocus);
#ifdef SKIP_ASM
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
            char* menu = (char*)self->mpMenu;
            sVEntry<void>* vt = *(sVEntry<void>**)(menu + 8);
            ((void (*)(void*, int))vt[8].fn)(vthis(menu, vt[8]), 0);
        }
        cScreenPopup_setPrivateDefaultOption(self, self->mpMenu, self->miDefaultOption, self->meType);
        {
            char* menu = (char*)self->mpMenu;
            sVEntry<void>* vt = *(sVEntry<void>**)(menu + 8);
            ((void (*)(void*, int))vt[7].fn)(vthis(menu, vt[7]), 1);
        }
        {
            sVEntry<void>* vt = (sVEntry<void>*)self->mVtbl;
            ((int (*)(void*, void*, int, int))vt[20].fn)(vthis(self, vt[20]), self->mpMenu, 5, 0);
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
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, cPopupInfo* p);
extern "C" void cScreenPopup_scaleBackground(cScreenPopup* self, sColor_7738* color);
extern char str_Start[];

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
        sColF_5C30 bgScale;
        bgScale.r = 0.2f;
        bgScale.g = 0.2f;
        bgScale.b = 1.0f;
        self->mbFirstGainTran = 1;
        cScreenPopup_initScreenPopup(self, (cPopupInfo*)&self->info);
        cScreenPopup_scaleBackground(self, (sColor_7738*)&bgScale);
        if (self->meBackgroundType == 1)
            cScreenPopup_SetTextObjectsAlpha(self, 0.0f);
        int startFrame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_Start));
        if (startFrame != 0xFFFF)
            self->miStartFrame = startFrame;
        else
            self->miStartFrame = 25;
        if (self->miStartFrame == 1)
            self->miStartFrame = 2;
        self->miGainTransistionCount = 1;
        self->mvAnimateIncrement[0] = (self->mvCurrentScale[0] - 0.2f) / (float)(self->miStartFrame - 1);
        self->mvAnimateIncrement[1] = (self->mvCurrentScale[1] - 0.2f) / (float)(self->miStartFrame - 1);
    }
    if (self->meBackgroundType == 1) {
        float transitionStep = (float)self->miGainTransistionCount;
        sVec3_5C30 pos = *(sVec3_5C30*)((char*)self->mpBackGroundSmall + 0x44);
        sColF_5C30 bgScale;
        bgScale.r = transitionStep * self->mvAnimateIncrement[0] + 0.2f;
        bgScale.g = transitionStep * self->mvAnimateIncrement[1] + 0.2f;
        cScreenPopup_scaleBackground(self, (sColor_7738*)&bgScale);
        if (self->miGainTransistionCount < self->miStartFrame) {
            self->miGainTransistionCount += 1;
            return 0;
        }
    } else {
        sColF_5C30 bgScale = *(sColF_5C30*)&self->mvCurrentScale[0];
        cScreenPopup_scaleBackground(self, (sColor_7738*)&bgScale);
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
extern "C" void cScreenPopup_restoreElements(cScreenPopup* self);
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, cPopupInfo* p);
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
        cScreenPopup_initScreenPopup(self, (cPopupInfo*)&self->info);
        sColor5DD8 bgScale = *(sColor5DD8*)&self->mvCurrentScale[0];
        cScreenPopup_scaleBackground(self, (sColor_7738*)&bgScale);
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
        vt[6].fn(vthis(self, vt[6]), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_SetTextObjectsVisibility);
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
        vt[9].fn(vthis(o, vt[9]), a);
    }
}

/**
 * Shows or hides the popup's text objects.
 *
 * Calls setVisible (cUIThing vtable slot 9) with @p visible on mpButtonGroup, mpMessageText, mpPostText, mpTitleText
 * and mpTextBoxPair, skipping any that is null.
 *
 * @param visible  nonzero to show, 0 to hide
 */
extern "C" void cScreenPopup_SetTextObjectsVisibility(cScreenPopup* self, int visible)
{
    Call9_5E58((char*)self->mpButtonGroup, visible);
    Call9_5E58((char*)self->mpMessageText, visible);
    Call9_5E58((char*)self->mpPostText, visible);
    Call9_5E58((char*)self->mpTitleText, visible);
    Call9_5E58((char*)self->mpTextBoxPair, visible);
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
extern char str_ngca[];
extern char str_xba[];
extern char str_ps2x[];

static inline void SetAlpha_5F20(char* o, float t)
{
    if (o != 0) {
        sColor_5F20 c = *(sColor_5F20*)(o + 0x1C);
        c.a = t;
        sVEntry_5F20* vt = *(sVEntry_5F20**)(o + 8);
        vt[11].fn(vthis(o, vt[11]), &c);
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
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ngca)), fAlpha);
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_xba)), fAlpha);
    SetAlpha_5F20((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x)), fAlpha);
    for (int i = 0; i < 5; i++) {
        SetAlpha_5F20(((char**)(&self->mpOptions[0]))[i], fAlpha);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_FillObjectPointers);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_Menu0000[];
extern char str_grey_background[];
extern char str_Big_outline[];
extern char str_Big_shape[];
extern char str_Small_outline[];
extern char str_Small_shape[];
extern char str_black_bg[];
extern char str_popupText[];
extern char str_popupTitle[];
extern char str_popupPost[];
extern char str_EnterTextBox[];
extern char str_TextBoxPair[];
extern char str_StaticTextBoxLabel[];
extern char str_EnterBackground[];
extern char str_whitefg[];
extern char str_redbg[];
extern char str_Shadow[];
extern char str_buttons[];
extern char str_Optionpct_d[];

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
    if (sObj_6260* uiObj = find_6260(self, str_whitefg)) {
        POS_6260(392) = uiObj->pos;
        uiObj->GetRect((char*)&self->mvSizeBackWhite);
        OBJ_6260(72) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_redbg)) {
        POS_6260(404) = uiObj->pos;
        OBJ_6260(76) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_grey_background))
        OBJ_6260(100) = uiObj;
    if (sObj_6260* uiObj = find_6260(self, str_Big_outline)) {
        POS_6260(560) = uiObj->pos;
        OBJ_6260(84) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_Big_shape)) {
        POS_6260(548) = uiObj->pos;
        OBJ_6260(80) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_Small_outline)) {
        POS_6260(584) = uiObj->pos;
        OBJ_6260(92) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_Small_shape)) {
        POS_6260(572) = uiObj->pos;
        uiObj->GetRect((char*)&self->mvSizeBackSmall);
        OBJ_6260(88) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_Shadow)) {
        POS_6260(596) = uiObj->pos;
        OBJ_6260(0x60) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_black_bg))
        OBJ_6260(104) = uiObj;
    if (sObj_6260* uiObj = find_6260(self, str_Menu0000)) {
        POS_6260(416) = uiObj->pos;
        OBJ_6260(108) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_buttons)) {
        POS_6260(428) = uiObj->pos;
        OBJ_6260(0x70) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_popupText)) {
        POS_6260(452) = uiObj->pos;
        OBJ_6260(116) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_popupTitle)) {
        POS_6260(440) = uiObj->pos;
        OBJ_6260(0x7C) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_popupPost)) {
        POS_6260(464) = uiObj->pos;
        OBJ_6260(0x78) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_EnterTextBox)) {
        POS_6260(476) = uiObj->pos;
        OBJ_6260(0x80) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_TextBoxPair)) {
        POS_6260(488) = uiObj->pos;
        OBJ_6260(136) = uiObj;
    }
    if (sObj_6260* uiObj = find_6260(self, str_StaticTextBoxLabel))
        OBJ_6260(132) = uiObj;
    if (sObj_6260* uiObj = find_6260(self, str_EnterBackground)) {
        POS_6260(0x200) = uiObj->pos;
        uiObj->GetRect((char*)&self->mvSizeBackEnterBox);
        OBJ_6260(140) = uiObj;
    }
    for (int i = 0; i < 5; i++) {
        char name[32];
        sprintf(name, str_Optionpct_d, i);
        sObj_6260* uiObj = find_6260(self, name);
        if (uiObj != 0)
            ((sObj_6260**)&self->mpOptions)[i] = uiObj;
    }
}
#undef POS_6260
#undef OBJ_6260
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setupBackgroundType);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScreenPopup_hideWidgetSafe(cScreenPopup* self, void* obj);
extern "C" void cScreenPopup_showWidgetSafe(cScreenPopup* self, void* obj);
extern sQuad_58E8 gPopupBlue;


static inline void SetColor_C66E8(char* o, sQuad_58E8* c)
{
    if (o != 0) {
        sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(o + 8);
        vt[11].fn(vthis(o, vt[11]), c);
    }
}

extern "C" void cScreenPopup_setupBackgroundType_impl(char* self) __asm__("cScreenPopup_setupBackgroundType");
/**
 * Shows the widgets of the popup's background style and hides the others.
 *
 * Switches on the background type (meBackgroundType, +0x31C, which cScreenPopup_initScreenPopup sets just before
 * the call): each style shows its frame pieces with cScreenPopup_showWidgetSafe, hides the rest with cScreenPopup_hideWidgetSafe, colours the
 * frame widgets (+0x74..+0x80) through virtual slot 11 and sets the popup's text and back colours (+0x32C, +0x33C)
 * from gPopupWhite/gPopupBlack.
 */
extern "C" void cScreenPopup_setupBackgroundType_impl(char* vself)
{
    cScreenPopup* self = (cScreenPopup*)vself;
    switch (self->meBackgroundType) {
    case 0:
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundGreyFade);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundWhite);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundRed);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundLarge);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundLargeOutline);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmall);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmallOutline);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundShadow);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundBlack);
        {
            char* frameWidget = (char*)self->mpMessageText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpPostText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpTitleText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpEnterBox;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        *(sQuad_58E8*)&self->mtHighlightedColor[0] = gPopupWhite;
        *(sQuad_58E8*)&self->mtUnhighlightColor[0] = gPopupBlack;
        break;
    case 1:
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundGreyFade);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundLarge);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundLargeOutline);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmall);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmallOutline);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundShadow);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundWhite);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundRed);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundBlack);
        {
            char* frameWidget = (char*)self->mpMessageText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpPostText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpTitleText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        {
            char* frameWidget = (char*)self->mpEnterBox;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupBlack);
            }
        }
        *(sQuad_58E8*)&self->mtHighlightedColor[0] = gPopupWhite;
        *(sQuad_58E8*)&self->mtUnhighlightColor[0] = gPopupBlack;
        break;
    case 2:
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundWhite);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundRed);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundLarge);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundLargeOutline);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmall);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundSmallOutline);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundShadow);
        cScreenPopup_hideWidgetSafe((cScreenPopup*)self, self->mpBackGroundGreyFade);
        cScreenPopup_showWidgetSafe((cScreenPopup*)self, self->mpBackGroundBlack);
        {
            char* frameWidget = (char*)self->mpMessageText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupWhite);
            }
        }
        {
            char* frameWidget = (char*)self->mpPostText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupWhite);
            }
        }
        {
            char* frameWidget = (char*)self->mpTitleText;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupWhite);
            }
        }
        {
            char* frameWidget = (char*)self->mpEnterBox;
            if (frameWidget != 0) {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(frameWidget + 8);
                vt[11].fn(vthis(frameWidget, vt[11]), &gPopupWhite);
            }
        }
        *(sQuad_58E8*)&self->mtHighlightedColor[0] = gPopupBlue;
        *(sQuad_58E8*)&self->mtUnhighlightColor[0] = gPopupWhite;
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
extern "C" sVec3_6B08* cPopupInfo_getPopupConfirmation(void* info, int i);
extern "C" void cScreenPopup_createTitleBar(cScreenPopup* self, cPopupInfo* a1);
extern "C" void cScreenPopup_setPopupMessage(cScreenPopup* self, cPopupInfo* a1);
extern "C" void cScreenPopup_FillOptions(cScreenPopup* self, cPopupInfo* a1);
extern "C" void cScreenPopup_adjustMenuPosition(cScreenPopup* self);
extern "C" void cScreenPopup_adjustButtonPosition(cScreenPopup* self);
extern "C" void cScreenPopup_handlePostPending(cScreenPopup* self, cPopupInfo* a1);
extern "C" void cScreenPopup_adjustLayer(void* self, int v);
extern "C" void cScreenPopup_setupBackgroundType(void* self, int v);
extern "C" void cScreenPopup_centerPopup(cScreenPopup* self);
extern "C" void cScreenPopup_FillTextEntryBox(cScreenPopup* self);
extern "C" void cScreenPopup_checkAndFixTextEntry(cScreenPopup* self, cUIText* text, const char* str);

/**
 * Builds the popup from a popup description: copies its settings and lays out the title, text and options.
 *
 * Copies the misc data, pass-update, receive-previous, type, centring flags, minimum width, confirmations,
 * offset, layer offset, background type, default option and maximum characters from the info. The start width
 * is mfMinSize, or 472 (300 when the text is centred). Then it places the title (cScreenPopup_createTitleBar), message,
 * options, buttons (cScreenPopup_adjustMenuPosition, cScreenPopup_adjustButtonPosition) and post text (cScreenPopup_handlePostPending), shifts the layers, sets the
 * background (cScreenPopup_setupBackgroundType) and its layout (cScreenPopup_centerPopup); a text-entry popup (type -1) also sizes the
 * entry box (cScreenPopup_FillTextEntryBox) and fits its initial text with cScreenPopup_checkAndFixTextEntry().
 *
 * @param info  the popup description (On Tour: cPopupInfo *pInfo); the popup's own copy at +0xBC in practice
 */
extern "C" void cScreenPopup_initScreenPopup(cScreenPopup* self, cPopupInfo* info)
{
    cScreenPopup_FillObjectPointers(self);
    self->miMiscData = info->miMiscData;
    self->mbPassUpdate = info->mbPassUpdate;
    self->mbReceivePrev = info->mbReceivePrev;
    self->meType = info->meType;
    self->mbCenterOptions = info->mbCenterOptions;
    self->mbCenterText = info->mbCenterText;
    float minSize = info->mfMinSize;
    self->mfMinSize = minSize;
    if (minSize != 0.0f)
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(minSize, 0.0f, 0.0f);
    else if (self->mbCenterText == 0)
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(472.0f, 0.0f, 0.0f);
    else
        *(sVec3_6B08*)&self->mvCurSize[0] = sVec3_6B08(300.0f, 0.0f, 0.0f);
    for (int i = 0; i < 5; i++)
        ((sVec3_6B08*)&self->mtConfirmation)[i] = *cPopupInfo_getPopupConfirmation(info, i);
    sVec3_6B08 offset = *(sVec3_6B08*)&info->mvOffset[0];
    *(sVec3_6B08*)&self->mvOffset[0] = offset;
    cScreenPopup_createTitleBar(self, info);
    cScreenPopup_setPopupMessage(self, info);
    cScreenPopup_FillOptions(self, info);
    cScreenPopup_adjustMenuPosition(self);
    cScreenPopup_adjustButtonPosition(self);
    cScreenPopup_handlePostPending(self, info);
    int layerOffset = info->miLayerOffset;
    self->miLayerOffset = layerOffset;
    if (layerOffset != 0)
        cScreenPopup_adjustLayer(self, layerOffset);
    int bgType = info->meBackgroundType;
    self->meBackgroundType = bgType;
    cScreenPopup_setupBackgroundType(self, bgType);
    self->miDefaultOption = info->miDefaultOption;
    self->miMaxChars = info->miMaxChars;
    cScreenPopup_centerPopup(self);
    if (info->meType == -1) {
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
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};
extern sVec3_6D30 t3vect_Zero_6D30 __asm__("t3vect_Zero");
extern "C" void cScreenPopup_setTextString1(cScreenPopup* self, void* dst, int* info);
extern "C" void cScreenPopup_setTextString2(cScreenPopup* self, cUIText* text, int* info);
extern "C" void cScreenPopup_getStringSize3(cScreenPopup* self, void* obj, tRect* out);
extern "C" void cScreenPopup_getStringSize(cScreenPopup* self, cUIText* text, int* info, float* out);
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item);

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
extern "C" int cScreenPopup_handleEntryBox(cScreenPopup* self, cPopupInfo* infop)
{
    int* info = (int*)infop;
    cScreenPopup* screenPopup = self;
    float size[4];
    if (info[0] == -1) {
        sVec3_6D30 pos = t3vect_Zero_6D30;
        float w = 0.0f;
        if (((cUIObj_6D30*)screenPopup->mpStaticBox) != 0) {
            float item[4];
            int* it = info + 5;
            cScreenPopup_setTextString2(self, (cUIText*)((cUIObj_6D30*)screenPopup->mpStaticBox), it);
            cScreenPopup_getStringSize3(self, ((cUIObj_6D30*)screenPopup->mpStaticBox), (tRect*)size);
            cScreenPopup_getStringSize(self, (cUIText*)((cUIObj_6D30*)screenPopup->mpStaticBox), it, item);
            w = item[2];
            pos = *(sVec3_6D30*)((char*)((cUIObj_6D30*)screenPopup->mpStaticBox) + 0x44);
            ((cUIObj_6D30*)screenPopup->mpStaticBox)->setStatic(0);
            ((cUIObj_6D30*)screenPopup->mpStaticBox)->setVisible(1);
        }
        if (((cUIObj_6D30*)screenPopup->mpEnterBox) != 0) {
            cScreenPopup_setTextString1(self, (char*)&self->mszInputText, info + 7);
            sVec3_6D30 boxPos = pos;
            boxPos.x = boxPos.x + w + 10.0f;
            *(sVec3_6D30*)((char*)((cUIObj_6D30*)screenPopup->mpEnterBox) + 0x44) = boxPos;
            cScreenPopup_increaseBoxSize(self, (float*)&self->mvCurSize, size);
            ((cUIObj_6D30*)screenPopup->mpEnterBox)->setStatic(0);
            ((cUIObj_6D30*)screenPopup->mpEnterBox)->setVisible(1);
        }
        if (((cUIObj_6D30*)screenPopup->mpTextBoxPair) != 0) {
            ((cUIObj_6D30*)screenPopup->mpTextBoxPair)->setStatic(0);
            ((cUIObj_6D30*)screenPopup->mpTextBoxPair)->setVisible(1);
        }
        if (((cUIObj_6D30*)screenPopup->mpEnterBoxBackground) != 0) {
            ((cUIObj_6D30*)screenPopup->mpEnterBoxBackground)->setVisible(1);
        }
        info[0] = 1;
        return 1;
    }
    if (((cUIObj_6D30*)screenPopup->mpTextBoxPair) != 0) {
        ((cUIObj_6D30*)screenPopup->mpTextBoxPair)->setStatic(1);
        ((cUIObj_6D30*)screenPopup->mpTextBoxPair)->setVisible(0);
    }
    if (((cUIObj_6D30*)screenPopup->mpStaticBox) != 0) {
        ((cUIObj_6D30*)screenPopup->mpStaticBox)->setStatic(1);
        ((cUIObj_6D30*)screenPopup->mpStaticBox)->setVisible(0);
    }
    if (((cUIObj_6D30*)screenPopup->mpEnterBox) != 0) {
        ((cUIObj_6D30*)screenPopup->mpEnterBox)->setStatic(1);
        ((cUIObj_6D30*)screenPopup->mpEnterBox)->setVisible(0);
    }
    if (((cUIObj_6D30*)screenPopup->mpEnterBoxBackground) != 0) {
        ((cUIObj_6D30*)screenPopup->mpEnterBoxBackground)->setVisible(0);
    }
    cScreenPopup_getStringSize3(self, ((cUIObj_6D30*)screenPopup->mpEnterBox), (tRect*)size);
    cScreenPopup_increaseBoxSize(self, (float*)&self->mvCurSize, size);
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_handlePostPending);
#ifdef SKIP_ASM
struct sVec3_7040 { float x, y, z; };
extern "C" void cScreenPopup_getTextBoxWrapSize(cScreenPopup* self, void* a, void* b, int which, float t);
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float t);

/**
 * Places and sizes the popup's post-pending text below the title when the popup description has one.
 *
 * With mpPostText set and the description's mtPostPending given (eType at +0x4C nonzero), moves the text to
 * mvPosTitle + (0, mvCurSize.y), grows mvCurSize by its wrapped size with a 10.0 gap
 * (cScreenPopup_getTextBoxWrapSize on the post text) and, when mbCenterText is set, re-pivots it (cScreenPopup_changePivot, On
 * Tour changePivot, pivot 0x11).
 *
 * @param pInfo  the popup description (On Tour cPopupInfo* pInfo)
 */
extern "C" void cScreenPopup_handlePostPending(cScreenPopup* self, cPopupInfo* pInfo)
{
    char* postText = (char*)self->mpPostText;
    if (postText != 0) {
        if (pInfo->mtPostPending.eType != 0) {
            sVec3_7040 textPos = *(sVec3_7040*)&self->mvPosTitle[0];
            textPos.y += self->mvCurSize[1];
            *(sVec3_7040*)(postText + 0x44) = textPos;
            cScreenPopup_getTextBoxWrapSize(self, (char*)&pInfo->mtPostPending, (char*)&self->mvCurSize, 0, 10.0f);
            if (self->mbCenterText != 0) {
                cScreenPopup_changePivot(self, self->mpPostText, 17, 0.0f);
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

/**
 * Sets the size of the popup's message, post and title texts, keeping each text's own width or height where @p vp
 * is 0.
 *
 * For each existing text it reads the current size and replaces the components that are nonzero in @p vp, but only
 * the post text gets that merged size: the message and title texts are set to @p vp itself, so a 0 component zeroes
 * their size. Likely an original bug (copy-paste of the post-text branch).
 *
 * @param vp  the new size (On Tour t3Vector& vSize); 0 components mean "keep"
 */
extern "C" void cScreenPopup_setTextObjectSize(cScreenPopup* self, sVec3_7258* vp)
{
    sVec3_70F0* size = (sVec3_70F0*)vp;
    cW_70F0* textObj = (cW_70F0*)self->mpMessageText;
    if (textObj != 0) {
        sVec3_70F0 curSize;
        textObj->get(&curSize);
        if (size->x != 0.0f) curSize.x = size->x;
        if (size->y != 0.0f) curSize.y = size->y;
        ((cW_70F0*)self->mpMessageText)->set(size);
    }
    textObj = (cW_70F0*)self->mpPostText;
    if (textObj != 0) {
        sVec3_70F0 curSize;
        textObj->get(&curSize);
        if (size->x != 0.0f) curSize.x = size->x;
        if (size->y != 0.0f) curSize.y = size->y;
        ((cW_70F0*)self->mpPostText)->set(&curSize);
    }
    textObj = (cW_70F0*)self->mpTitleText;
    if (textObj != 0) {
        sVec3_70F0 curSize;
        textObj->get(&curSize);
        if (size->x != 0.0f) curSize.x = size->x;
        if (size->y != 0.0f) curSize.y = size->y;
        ((cW_70F0*)self->mpTitleText)->set(size);
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
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float t);

/**
 * Places and fills the popup's message text below the title and records the size reached.
 *
 * The message goes at mvPosTitle shifted down by the height so far. With centred text it first measures the
 * message's first wrapped line (cScreenPopup_getMessageLineWrapSize1()) and widens the texts (cScreenPopup_setTextObjectSize)
 * when it is wider than the popup. cScreenPopup_getTextBoxWrapSize fills the text from the info's mtMessage and grows
 * mvCurSize; centred text is then realigned (cScreenPopup_changePivot). mvMenuSize gets the size reached.
 *
 * @param pInfo  the popup description (cPopupInfo)
 */
extern "C" void cScreenPopup_setPopupMessage(cScreenPopup* self, cPopupInfo* pInfo)
{
    char* messageText = (char*)self->mpMessageText;
    if (messageText != 0) {
        sVec3_7258 textPos = *(sVec3_7258*)&self->mvPosTitle[0];
        textPos.y += self->mvCurSize[1];
        *(sVec3_7258*)(messageText + 0x44) = textPos;
        char* message = (char*)&pInfo->mtMessage;
        if (self->mbCenterText != 0) {
            sVec3_7258 wrapSize = t3vect_Zero;
            cScreenPopup_getMessageLineWrapSize1(self, (int*)message, &wrapSize);
            if (wrapSize.x > self->mvCurSize[0])
                cScreenPopup_setTextObjectSize(self, &wrapSize);
        }
        cScreenPopup_getTextBoxWrapSize(self, message, (char*)&self->mvCurSize, 1, 10.0f);
        if (self->mbCenterText != 0)
            cScreenPopup_changePivot(self, self->mpMessageText, 17, 0.0f);
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
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};
struct sPopup_7388 {
    char pad[0x90];
    cUIObj_7388* items[5];
};
extern "C" int cScreenPopup_handleEntryBox(cScreenPopup* self, cPopupInfo* info);
extern "C" int* cPopupInfo_getPopupOptionNames(void* info, int i);
extern "C" void cScreenPopup_setTextString2(cScreenPopup* self, cUIText* text, int* info);
extern "C" void cScreenPopup_getStringSize3(cScreenPopup* self, void* obj, tRect* out);
extern "C" void cScreenPopup_getStringSize(cScreenPopup* self, cUIText* text, int* info, float* out);
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item);

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
extern "C" void cScreenPopup_FillOptions(cScreenPopup* self, cPopupInfo* infop)
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
    int hasEntryBox = cScreenPopup_handleEntryBox(self, (cPopupInfo*)info);
    if (info[0] == 0)
        ((cUIObj_7388*)self->mpButtonGroup)->setVisible(0);
    else
        ((cUIObj_7388*)self->mpButtonGroup)->setVisible(1);
    for (int i = info[0]; i < 5; i++) {
        if (((sPopup_7388*)self)->items[i] != 0) {
            ((sPopup_7388*)self)->items[i]->setStatic(1);
            ((sPopup_7388*)self)->items[i]->setVisible(0);
        }
    }
    for (int j = 0; j < info[0]; j++) {
        cUIObj_7388* itemObj = ((sPopup_7388*)self)->items[j];
        if (itemObj != 0) {
            float size[4];
            float item[4];
            int* it = cPopupInfo_getPopupOptionNames(info, j);
            cScreenPopup_setTextString2(self, (cUIText*)itemObj, it);
            cScreenPopup_getStringSize3(self, itemObj, (tRect*)size);
            cScreenPopup_getStringSize(self, (cUIText*)itemObj, it, item);
            if (self->mvLargestOption[0] < item[2])
                self->mvLargestOption[0] = item[2];
            cScreenPopup_increaseBoxSize(self, (float*)&self->mvCurSize, size);
            itemObj->setStatic(0);
            itemObj->setVisible(1);
        }
    }
    self->mvCurSize[1] += 10.0f;
    if (hasEntryBox != 0)
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
INCLUDE_ASM("fe/fepopup", cScreenPopup_determineAndSetScaling);
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

/**
 * Works out how much to stretch the popup background to fit content of a given size.
 *
 * For the current background type (+0x31C), divides the content size plus that style's margin by the style's
 * base size and stores the x and y scales at +0x278 and +0x27C (one style also widens x by 5%).
 *
 * @param pos  the content size (On Tour vSize)
 */
// Converts a screen position into the popup's normalised scroll position for the current layout.
extern "C" void cScreenPopup_determineAndSetScaling(cScreenPopup* self, t3Vector pos)
{
    switch (self->meBackgroundType) {
    case 0:
        self->mvCurrentScale[0] = (pos.x + 15.0f) / self->mvSizeBackWhite[0];
        self->mvCurrentScale[1] = (pos.y + 10.0f) / self->mvSizeBackWhite[1];
        break;
    case 1:
        self->mvCurrentScale[0] = dptofp(dpmul(fptodp((pos.x + 20.0f) / self->mvSizeBackSmall[0]), 1.05));
        self->mvCurrentScale[1] = (pos.y + 15.0f) / self->mvSizeBackSmall[1];
        break;
    case 2:
        self->mvCurrentScale[0] = 1.0f;
        self->mvCurrentScale[1] = 1.0f;
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
    sColor_7738 scaleColor = *color;
    switch (self->meBackgroundType) {
    case 0:
        {
            void* background = self->mpBackGroundWhite;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        {
            void* background = self->mpBackGroundRed;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        break;
    case 1:
        {
            void* background = self->mpBackGroundSmall;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        {
            void* background = self->mpBackGroundSmallOutline;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        {
            void* background = self->mpBackGroundLarge;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        {
            void* background = self->mpBackGroundLargeOutline;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        {
            void* background = self->mpBackGroundShadow;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        break;
    case 2:
        {
            void* background = self->mpBackGroundBlack;
            if (background != 0) {
                *(sColor_7738*)((char*)background + 0x50) = scaleColor;
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_resizeBackground);
#ifdef SKIP_ASM
struct sVec3_78C0 { float x, y, z; };
// PORT: SN soft-float helpers called by name (fptodp / dpsub / dptofp); on PC: y = (float)((double)(...) - 7.5)
extern "C" double fptodp(float f);
extern "C" double dpsub(double a, double b);
extern "C" float dptofp(double d);

/**
 * Moves the background frame pieces to wrap content of a given size at an offset.
 *
 * For the current background type (+0x31C), places each existing frame widget (+0x48, +0x4C, ...) from its base
 * position (+0x188...) shifted by @p fXOffset and positioned vertically from @p fYOffset and half the content height.
 *
 * @param vSize  the content size (On Tour vSize)
 * @param fXOffset  horizontal offset
 * @param fYOffset  vertical offset
 */
extern "C" void cScreenPopup_resizeBackground(cScreenPopup* self, sVec3_78C0 vSize, float fXOffset, float fYOffset)
{
    float y = 0.0f;
    switch (self->meBackgroundType) {
    case 0:
        if ((char*)self->mpBackGroundWhite != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackWhite[0];
            y = self->mvPosTitle[1] + fYOffset + vSize.y * 0.5f - 5.0f;
            piecePos.x += fXOffset;
            piecePos.y = y;
            *(sVec3_78C0*)((char*)self->mpBackGroundWhite + 0x44) = piecePos;
        }
        if ((char*)self->mpBackGroundRed != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackRed[0];
            piecePos.y = y;
            piecePos.x += fXOffset;
            *(sVec3_78C0*)((char*)self->mpBackGroundRed + 0x44) = piecePos;
        }
        break;
    case 1:
        if ((char*)self->mpBackGroundSmall != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackSmall[0];
            y = dptofp(dpsub(fptodp(self->mvPosTitle[1] + fYOffset + vSize.y * 0.5f), 7.5));
            piecePos.x += fXOffset;
            piecePos.y = y;
            *(sVec3_78C0*)((char*)self->mpBackGroundSmall + 0x44) = piecePos;
        }
        if ((char*)self->mpBackGroundSmallOutline != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackSmallOutline[0];
            piecePos.y = y;
            piecePos.x += fXOffset;
            *(sVec3_78C0*)((char*)self->mpBackGroundSmallOutline + 0x44) = piecePos;
        }
        if ((char*)self->mpBackGroundLarge != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackLarge[0];
            piecePos.y = y;
            piecePos.x += fXOffset;
            *(sVec3_78C0*)((char*)self->mpBackGroundLarge + 0x44) = piecePos;
        }
        if ((char*)self->mpBackGroundLargeOutline != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackLargeOutline[0];
            piecePos.y = y;
            piecePos.x += fXOffset;
            *(sVec3_78C0*)((char*)self->mpBackGroundLargeOutline + 0x44) = piecePos;
        }
        if ((char*)self->mpBackGroundShadow != 0) {
            sVec3_78C0 piecePos = *(sVec3_78C0*)&self->mvPosBackShadow[0];
            piecePos.y = y;
            piecePos.x += fXOffset;
            *(sVec3_78C0*)((char*)self->mpBackGroundShadow + 0x44) = piecePos;
        }
        break;
    case 2:
        self->mvCurrentScale[1] = self->mvCurrentScale[0] = 1.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_centerOffset);
#ifdef SKIP_ASM
/**
 * Gives the x offset that centres the popup box on the 640-pixel screen.
 *
 * @return 0 when mbCenterText is set, otherwise (640 - mvCurSize.x) / 2 - mvPosTitle.x
 */
extern "C" float cScreenPopup_centerOffset(cScreenPopup* self)
{
    if (self->mbCenterText != 0) {
        return 0.0f;
    }
    return (640.0f - self->mvCurSize[0]) * 0.5f - self->mvPosTitle[0];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_centerPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec3_7C20 { float x, y, z; };
extern "C" float cScreenPopup_centerOffset(cScreenPopup* self);
void cScreenPopup_determineAndSetScaling_7C20(void* self, sVec3_7C20 v) __asm__("cScreenPopup_determineAndSetScaling");
void cScreenPopup_resizeBackground_7C20(void* self, sVec3_7C20 v, float a, float b) __asm__("cScreenPopup_resizeBackground");

/**
 * Lays the popup out centred on screen.
 *
 * Takes the horizontal offset from cScreenPopup_centerOffset plus +0x26C and the vertical one from +0x270,
 * scales (cScreenPopup_determineAndSetScaling) and places (cScreenPopup_resizeBackground) the background for the content size (+0x260), then moves
 * the title, message, option and button widgets by the same offsets.
 */
extern "C" void cScreenPopup_centerPopup(cScreenPopup* self)
{
    float x = cScreenPopup_centerOffset((cScreenPopup*)self) + self->mvOffset[0];
    float y = self->mvOffset[1];
    cScreenPopup_determineAndSetScaling_7C20(self, *(sVec3_7C20*)&self->mvCurSize[0]);
    cScreenPopup_resizeBackground_7C20(self, *(sVec3_7C20*)&self->mvCurSize[0], self->mvOffset[0], y);
    char* widget = (char*)self->mpMenu;
    if (widget != 0) {
        sVec3_7C20 widgetPos = *(sVec3_7C20*)(widget + 0x44);
        widgetPos.x += self->mvOffset[0];
        widgetPos.y += y;
        *(sVec3_7C20*)(widget + 0x44) = widgetPos;
    }
    widget = (char*)self->mpButtonGroup;
    if (widget != 0) {
        sVec3_7C20 widgetPos = *(sVec3_7C20*)(widget + 0x44);
        widgetPos.x += self->mvOffset[0];
        widgetPos.y += y;
        *(sVec3_7C20*)(widget + 0x44) = widgetPos;
    }
    if ((char*)self->mpMessageText != 0) {
        sVec3_7C20 widgetPos = *(sVec3_7C20*)((char*)self->mpMessageText + 0x44);
        if (self->mbCenterText != 0) {
            widgetPos.x = x + 320.0f;
        } else {
            widgetPos.x += x;
        }
        widgetPos.y += y;
        *(sVec3_7C20*)((char*)self->mpMessageText + 0x44) = widgetPos;
    }
    if ((char*)self->mpTitleText != 0) {
        sVec3_7C20 widgetPos = *(sVec3_7C20*)((char*)self->mpTitleText + 0x44);
        if (self->mbCenterText != 0) {
            widgetPos.x = x + 320.0f;
        } else {
            widgetPos.x += x;
        }
        widgetPos.y += y;
        *(sVec3_7C20*)((char*)self->mpTitleText + 0x44) = widgetPos;
    }
    if ((char*)self->mpPostText != 0) {
        sVec3_7C20 widgetPos = *(sVec3_7C20*)((char*)self->mpPostText + 0x44);
        if (self->mbCenterText != 0) {
            widgetPos.x = x + 320.0f;
        } else {
            widgetPos.x += x;
        }
        widgetPos.y += y;
        *(sVec3_7C20*)((char*)self->mpPostText + 0x44) = widgetPos;
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

/**
 * Stretches the popup's text-entry box and its background to the popup's current width.
 *
 * The entry box (mpEnterBox) gets width mvCurSize.x - (its x - mpStaticBox's x) - 10 (size get / set virtuals,
 * slots 20 and 21). mpEnterBoxBackground, when present, is centred on it (x + (width + 6) / 2, y + half the
 * background height + the menu's y - 3) and scaled in x by (width + 6) / mvSizeBackEnterBox.x.
 */
extern "C" void cScreenPopup_FillTextEntryBox(cScreenPopup* self)
{
    char* enterBox = (char*)self->mpEnterBox;
    sVec3_7EB0 enterPos = *(sVec3_7EB0*)(enterBox + 0x44);
    sVec3_7EB0 staticPos;
    sVec3_7EB0 menuPos = *(sVec3_7EB0*)((char*)self->mpMenu + 0x44);
    staticPos = *(sVec3_7EB0*)((char*)self->mpStaticBox + 0x44);
    sVec2_7EB0 size;
    {
        sVE_7EB0* vt = *(sVE_7EB0**)(enterBox + 8);
        vt[20].fn(vthis(enterBox, vt[20]), &size);
    }
    size.x = self->mvCurSize[0] - (enterPos.x - staticPos.x) - 10.0f;
    {
        char* box = (char*)self->mpEnterBox;
        sVE_7EB0* vt = *(sVE_7EB0**)(box + 8);
        vt[21].fn(vthis(box, vt[21]), &size);
    }
    sVec3_7EB0 scale;
    scale.x = (size.x + 6.0f) / self->mvSizeBackEnterBox[0];
    scale.y = 1.0f;
    scale.z = 1.0f;
    char* background = (char*)self->mpEnterBoxBackground;
    if (background != 0) {
        sVec3_7EB0 backPos = enterPos;
        backPos.y = backPos.y + self->mvSizeBackEnterBox[1] * 0.5f + menuPos.y - 3.0f;
        backPos.x = backPos.x + (size.x + 6.0f) * 0.5f;
        *(sVec3_7EB0*)(background + 0x44) = backPos;
        *(sVec3_7EB0*)((char*)self->mpEnterBoxBackground + 0x50) = scale;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextString2);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern char D_004A1408[];

/**
 * Sets a text widget's string from a popup string element.
 *
 * By @p info's eType (tPopupElement): 1 sets the unicode string (cUIText_setUnicodeString), 2 the ascii string
 * (cUIText_setAsciiString), 3 the localised string by id (cUIText_setUnicodeStringByID); 0 sets the empty string
 * D_004A1408. Other types leave the text unchanged.
 *
 * @param text  the text widget to fill
 * @param info  the string element: eType at [0], the string or id at [1]
 */
extern "C" void cScreenPopup_setTextString2(cScreenPopup* self, cUIText* text, int* info)
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
extern "C" void* cBXString_operatorE1(void* self, const char* str);
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_setTextString[];
extern char str_String_pct_08x[];
extern char D_004A1408[];


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
        unsigned short* unicode = (unsigned short*)info[1];
        char* buf = (char*)operator_new_tag(USTR_length(unicode) + 1, str_setTextString, 0x100, 0);
        strcpy2(buf, (unsigned short*)info[1]);
        cBXString_operatorE1(dst, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
        break;
    }
    case 2:
        cBXString_operatorE1(dst, (const char*)info[1]);
        break;
    case 3: {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)locale->mVtbl;
        unsigned short* str = vt[4].fn(vthis(locale, vt[4]), info[1]);
        if (str) {
            char* buf = (char*)operator_new_tag(USTR_length(str) + 1, str_setTextString, 0x100, 0);
            strcpy2(buf, str);
            cBXString_operatorE1(dst, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
        } else {
            char idText[0x40];
            sprintf(idText, str_String_pct_08x, info[1]);
            cBXString_operatorE1(dst, idText);
        }
        break;
    }
    case 0:
        cBXString_operatorE1(dst, D_004A1408);
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
extern "C" void* cBXString_operatorE1(void* self, const char* str);
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_ScreenPopuponWidgetEvent[];
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
    char* ascii = cUIText_getAsciiString(text);
    if (ascii != 0) {
        cBXString_operatorE1(dst, ascii);
        return;
    }
    unsigned short* unicode = cUIText_getUnicodeString(text);
    if (unicode != 0) {
        char* buf = (char*)operator_new_tag(USTR_length(unicode) + 1, str_ScreenPopuponWidgetEvent, 0x100, 0);
        strcpy2(buf, unicode);
        cBXString_operatorE1(dst, buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else {
        cBXString_operatorE1(dst, D_004A1408);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getStringSize);
#ifdef SKIP_ASM
struct cUIText;
struct sVec3_8320 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" float cFont_textExtent1(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_String_pct_08x[];

struct sVEntry_8320 {
    short delta;
    short index;
    char* (*fn)(void*, int);
};

/**
 * Measures a popup string element as a text widget would draw it.
 *
 * Scales the widget's font by the widget's mScale (font +0x38/+0x3C = font +0x30/+0x34 times mScale.x/.y), then by @p
 * info's eType: 1 measures the unicode string (cFont_textExtent1), 2 the ascii string (cFont_textExtent); 3 looks the id up
 * through SSXApp->FELocale's virtual slot 4 and measures that unicode string, or, when it is missing, the text "String
 * %08x" of the id (64-byte stack buffer) as ascii. Type 0 sets @p out[2] and @p out[3] (width and height) to 0.
 *
 * @param text  the text widget whose font and scale are used
 * @param info  the string element: eType at [0], the string or id at [1]
 * @param out   the result rectangle (four floats, written by the font's extent function)
 */
extern "C" void cScreenPopup_getStringSize(cScreenPopup* self, cUIText* text, int* info, float* out)
{
    sVec3_8320 textScale = *(sVec3_8320*)((char*)text + 0x50);
    cFont* scaledFont = (cFont*)cUIText_getFont(text);
    float sx = scaledFont->BaseScale[0] * textScale.x;
    float sy = scaledFont->BaseScale[1] * textScale.y;
    scaledFont->Scale[0] = sx;
    scaledFont->Scale[1] = sy;
    char* measureFont;
    switch (info[0]) {
    case 1:
        measureFont = (char*)cUIText_getFont(text);
        cFont_textExtent1(measureFont, (const char*)info[1], out, 0, *(float*)(measureFont + 0x38), *(float*)(measureFont + 0x3C));
        break;
    case 2:
        measureFont = (char*)cUIText_getFont(text);
        cFont_textExtent(measureFont, (const char*)info[1], out, 0, *(float*)(measureFont + 0x38), *(float*)(measureFont + 0x3C));
        break;
    case 3: {
        char* feLocale = (char*)SSXApp->FELocale;
        sVEntry_8320* vt = *(sVEntry_8320**)(feLocale + 4);
        char* str = vt[4].fn(vthis(feLocale, vt[4]), info[1]);
        if (str) {
            measureFont = (char*)cUIText_getFont(text);
            cFont_textExtent1(measureFont, str, out, 0, *(float*)(measureFont + 0x38), *(float*)(measureFont + 0x3C));
        } else {
            char buf[0x40];
            sprintf(buf, str_String_pct_08x, info[1]);
            measureFont = (char*)cUIText_getFont(text);
            cFont_textExtent(measureFont, buf, out, 0, *(float*)(measureFont + 0x38), *(float*)(measureFont + 0x3C));
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
INCLUDE_ASM("fe/fepopup", cScreenPopup_getStringSize1);
#ifdef SKIP_ASM
struct sVec3_84A8 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

/**
 * Measures an ascii string as a text widget would draw it, including the line height.
 *
 * Scales the widget's font by its mScale (as cScreenPopup_getStringSize does), measures @p str with cFont_textExtent into @p out,
 * and sets @p out's height (+0xC) to mScale.y times the font's line height (font +0x14, as a float) times its y scale
 * (font +0x34).
 *
 * @param text  the text widget whose font and scale are used
 * @param str   the ascii string
 * @param out   the result rectangle (four floats)
 */
extern "C" void cScreenPopup_getStringSize1(cScreenPopup* self, void* text, const char* str, void* out)
{
    sVec3_84A8 scale = *(sVec3_84A8*)((char*)text + 0x50);
    cFont* font = (cFont*)cUIText_getFont(text);
    float sx = font->BaseScale[0] * scale.x;
    float sy = font->BaseScale[1] * scale.y;
    font->Scale[0] = sx;
    font->Scale[1] = sy;
    font = (cFont*)cUIText_getFont(text);
    cFont_textExtent(font, str, out, 0, font->Scale[0], font->Scale[1]);
    font = (cFont*)cUIText_getFont(text);
    *(float*)((char*)out + 0xC) = scale.y * ((float)font->MaxCharHeight * font->BaseScale[1]);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getStringSize2);
#ifdef SKIP_ASM
struct sVec3_8568 { float x, y, z; };
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent1(void* self, const char* str, void* out, int n, float sx, float sy);

/**
 * Measures a unicode string as a text widget would draw it, including the line height.
 *
 * Same as cScreenPopup_getStringSize1() but measures with the unicode extent function cFont_textExtent1 (@p str is passed through as a
 * char pointer).
 *
 * @param text  the text widget whose font and scale are used
 * @param str   the unicode string
 * @param out   the result rectangle (four floats)
 */
extern "C" void cScreenPopup_getStringSize2(cScreenPopup* self, void* text, const char* str, void* out)
{
    sVec3_8568 scale = *(sVec3_8568*)((char*)text + 0x50);
    cFont* font = (cFont*)cUIText_getFont(text);
    float sx = font->BaseScale[0] * scale.x;
    float sy = font->BaseScale[1] * scale.y;
    font->Scale[0] = sx;
    font->Scale[1] = sy;
    font = (cFont*)cUIText_getFont(text);
    cFont_textExtent1(font, str, out, 0, font->Scale[0], font->Scale[1]);
    font = (cFont*)cUIText_getFont(text);
    *(float*)((char*)out + 0xC) = scale.y * ((float)font->MaxCharHeight * font->BaseScale[1]);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getStringSize3);
#ifdef SKIP_ASM

struct sVEntry001C8628 {
    short delta;
    short index;
    void (*fn)(void*, t2Vector*);
};

/**
 * Copies a widget's size into a rectangle's width and height.
 *
 * Calls the widget's getSize (cUIThing vtable slot 20) and stores its x and y at @p out + 0x8 and + 0xC.
 *
 * @param obj  the widget
 * @param out  the result rectangle (four floats)
 */
extern "C" void cScreenPopup_getStringSize3(cScreenPopup* self, void* obj, tRect* out)
{
    t2Vector widgetSize;
    sVEntry001C8628* vt = *(sVEntry001C8628**)((char*)obj + 8);
    vt[20].fn(vthis(obj, vt[20]), &widgetSize);
    out->W = widgetSize.x;
    out->H = widgetSize.y;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_createTitleBar);
#ifdef SKIP_ASM
struct cUIText;
extern "C" void cScreenPopup_setTextString2(cScreenPopup* self, cUIText* text, int* info);
extern "C" void cScreenPopup_getStringSize(cScreenPopup* self, cUIText* text, int* info, float* out);
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item);
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float t);

/**
 * Shows and sizes the popup's title bar from the popup description, or hides it when there is no title.
 *
 * With mpTitleText set: when the description's mtTitle is given (eType at +0xC nonzero) it sets the text
 * (cScreenPopup_setTextString2, On Tour setTextString), shows it (slot 9), adds its size (getStringSize, increaseBoxSize) to
 * mvCurSize plus 5 in y and re-pivots it when mbCenterText is set; otherwise it hides it.
 *
 * @param pInfo  the popup description (On Tour cPopupInfo* pInfo)
 */
extern "C" void cScreenPopup_createTitleBar(cScreenPopup* self, cPopupInfo* pInfo)
{
    cUIText* text = self->mpTitleText;
    if (text != 0) {
        int* info = (int*)&pInfo->mtTitle;
        if (pInfo->mtTitle.eType != 0) {
            float item[4];
            cScreenPopup_setTextString2(self, text, info);
            {
                char* titleText = (char*)self->mpTitleText;
                sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(titleText + 8);
                vt[9].fn(vthis(titleText, vt[9]), 1);
            }
            cScreenPopup_getStringSize(self, self->mpTitleText, info, item);
            cScreenPopup_increaseBoxSize(self, (float*)&self->mvCurSize, item);
            self->mvCurSize[1] += 5.0f;
            if (self->mbCenterText != 0) {
                cScreenPopup_changePivot(self, self->mpTitleText, 17, 0.0f);
            }
        } else {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)text + 8);
            vt[9].fn(vthis(text, vt[9]), 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_adjustMenuPosition);
#ifdef SKIP_ASM
struct sVec3_8758 { float x, y, z; };
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float t);

/**
 * Centres the popup's option menu below the text and remembers the shift for the buttons.
 *
 * With mpMenu set: x = (640 - mvLargestOption.x) / 2 - the first option's x; the menu goes to mvPosMenu + (x,
 * mvMenuSize.y), mpTextBoxPair moves by -x + 5, the options are re-pivoted around half the widest option when
 * mbCenterOptions is set, and mvButtonOffset becomes (x, mvMenuSize.y).
 */
extern "C" void cScreenPopup_adjustMenuPosition(cScreenPopup* self)
{
    char* menu = (char*)self->mpMenu;
    if (menu != 0) {
        sVec3_8758 firstOptionPos = *(sVec3_8758*)((char*)self->mpOptions[0] + 0x44);
        float x = (640.0f - self->mvLargestOption[0]) * 0.5f - firstOptionPos.x;
        sVec3_8758 menuPos = *(sVec3_8758*)&self->mvPosMenu[0];
        menuPos.y += self->mvMenuSize[1];
        menuPos.x += x;
        *(sVec3_8758*)(menu + 0x44) = menuPos;
        char* textBoxPair = (char*)self->mpTextBoxPair;
        sVec3_8758 pairPos = *(sVec3_8758*)(textBoxPair + 0x44);
        pairPos.x = pairPos.x - x + 5.0f;
        *(sVec3_8758*)(textBoxPair + 0x44) = pairPos;
        if (self->mbCenterOptions != 0) {
            int i;
            for (i = 0; i < 5; i++) {
                void* option = *(void**)((char*)&self->mpOptions[0] + i * 4);
                if (option != 0)
                    cScreenPopup_changePivot(self, option, 18, self->mvLargestOption[0] * 0.5f);
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

/**
 * Moves the popup's button group to mvPosButtons plus the shift cScreenPopup_adjustMenuPosition stored in
 * mvButtonOffset.
 */
extern "C" void cScreenPopup_adjustButtonPosition(cScreenPopup* self)
{
    void* buttonGroup = self->mpButtonGroup;
    if (buttonGroup != 0) {
        sVec3_88C8 buttonPos = *(sVec3_88C8*)&self->mvPosButtons[0];
        buttonPos.y += self->mvButtonOffset[1];
        buttonPos.x += self->mvButtonOffset[0];
        *(sVec3_88C8*)((char*)buttonGroup + 0x44) = buttonPos;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_changePivot);
#ifdef SKIP_ASM
struct sObjBitsK8930 {
    unsigned lo : 13;
    unsigned state : 6;
    unsigned hi : 13;
};
struct sVec3K8930 { float x, y, z; };

struct sVEntryK8930 {
    short delta;
    short index;
    void (*fn)(void*, t2Vector*);
};

/**
 * Changes a text widget's pivot (anchor) while keeping it visually in place.
 *
 * Does nothing when the pivot bits at +0x14 already equal @p state. Otherwise stores the new pivot and shifts the
 * position (+0x44) by @p fOffset (half the text width when 0) according to the old and new anchors.
 *
 * @param obj    the text widget
 * @param state  the new pivot (On Tour ucPivot)
 * @param fOffset      the horizontal offset to apply, or 0 for half the text width
 */
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float fOffset)
{
    sObjBitsK8930* pivotBits = (sObjBitsK8930*)((char*)obj + 0x14);
    if (pivotBits->state == state)
        return;
    t2Vector size;
    sVEntryK8930* vt = *(sVEntryK8930**)((char*)obj + 8);
    vt[20].fn(vthis(obj, vt[20]), &size);
    sVec3K8930 pos = *(sVec3K8930*)((char*)obj + 0x44);
    pivotBits->state = state;
    if (fOffset == 0.0f)
        fOffset = size.x * 0.5f;
    switch (state) {
    case 17:
    case 18:
        pos.x += fOffset;
        break;
    case 9:
    case 10:
        pos.x -= fOffset;
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
extern sVec3_8A38 t3vect_Zero_8A38 __asm__("t3vect_Zero");
extern "C" void cScreenPopup_setTextObjectSize(cScreenPopup* self, sVec3_7258* v);
extern "C" void cScreenPopup_changePivot(cScreenPopup* self, void* obj, unsigned char state, float t);
extern "C" void cScreenPopup_adjustLayer(void* self, int v);

#define OBJ_8A38(off) (*(char**)((char*)self + (off)))
#define VEC_8A38(p, off) (*(sVec3_8A38*)((char*)(p) + (off)))

/**
 * Puts the popup's widgets back to their unit scale and home positions before a new layout.
 *
 * Resets the background shapes' scale and position, moves the menu, buttons, texts, entry box and text-box
 * pair back to the positions cached by cScreenPopup_FillObjectPointers(), realigns the options
 * (cScreenPopup_changePivot), resets the text widths to 472 (cScreenPopup_setTextObjectSize) and mvCurrentScale to 1, and undoes the
 * layer offset (cScreenPopup_adjustLayer with -miLayerOffset).
 *
 * The title and post-text branches realign mpMessageText instead of their own widgets (EA's code passes +0x74
 * three times), so only the message is realigned.
 */
// Resets the popup's widgets: unit scale and home positions, then re-anchors the side items.
extern "C" void cScreenPopup_restoreElements(cScreenPopup* self)
{
    if (OBJ_8A38(72) != 0) {
        VEC_8A38(OBJ_8A38(72), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(72), 68) = VEC_8A38(self, 392);
    }
    if (OBJ_8A38(76) != 0) {
        VEC_8A38(OBJ_8A38(76), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(76), 68) = VEC_8A38(self, 404);
    }
    if (OBJ_8A38(80) != 0) {
        VEC_8A38(OBJ_8A38(80), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(80), 68) = VEC_8A38(self, 548);
    }
    if (OBJ_8A38(84) != 0) {
        VEC_8A38(OBJ_8A38(84), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(84), 68) = VEC_8A38(self, 560);
    }
    if (OBJ_8A38(88) != 0) {
        VEC_8A38(OBJ_8A38(88), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(88), 68) = VEC_8A38(self, 572);
    }
    if (OBJ_8A38(92) != 0) {
        VEC_8A38(OBJ_8A38(92), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(92), 68) = VEC_8A38(self, 584);
    }
    if (OBJ_8A38(0x60) != 0) {
        VEC_8A38(OBJ_8A38(0x60), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(0x60), 68) = VEC_8A38(self, 596);
    }
    if (OBJ_8A38(140) != 0) {
        VEC_8A38(OBJ_8A38(140), 80) = sVec3_8A38(1.0f, 1.0f, 1.0f);
        VEC_8A38(OBJ_8A38(140), 68) = VEC_8A38(self, 0x200);
    }
    if (OBJ_8A38(108) != 0) {
        VEC_8A38(OBJ_8A38(108), 68) = VEC_8A38(self, 416);
    }
    if (OBJ_8A38(0x70) != 0) {
        VEC_8A38(OBJ_8A38(0x70), 68) = VEC_8A38(self, 428);
    }
    if (OBJ_8A38(116) != 0) {
        VEC_8A38(OBJ_8A38(116), 68) = VEC_8A38(self, 452);
        cScreenPopup_changePivot(self, OBJ_8A38(116), 9, 0.0f);
    }
    if (OBJ_8A38(0x7C) != 0) {
        VEC_8A38(OBJ_8A38(0x7C), 68) = VEC_8A38(self, 440);
        cScreenPopup_changePivot(self, OBJ_8A38(116), 9, 0.0f);
    }
    if (OBJ_8A38(0x78) != 0) {
        VEC_8A38(OBJ_8A38(0x78), 68) = VEC_8A38(self, 464);
        cScreenPopup_changePivot(self, OBJ_8A38(116), 9, 0.0f);
    }
    if (OBJ_8A38(0x80) != 0) {
        VEC_8A38(OBJ_8A38(0x80), 68) = VEC_8A38(self, 476);
    }
    if (OBJ_8A38(136) != 0) {
        VEC_8A38(OBJ_8A38(136), 68) = VEC_8A38(self, 488);
    }
    int i;
    for (i = 0; i < 5; i++) {
        void* option = *(void**)((char*)&self->mpOptions[0] + i * 4);
        if (option != 0) {
            cScreenPopup_changePivot(self, option, 10, self->mvLargestOption[0] * 0.5f);
        }
    }
    sVec3_8A38 textSize = t3vect_Zero_8A38;
    textSize.x = 472.0f;
    cScreenPopup_setTextObjectSize(self, (sVec3_7258*)&textSize);
    sVec3_8A38 one;
    one.x = 1.0f;
    one.y = 1.0f;
    one.z = 1.0f;
    VEC_8A38(self, 632) = one;
    cScreenPopup_adjustLayer(self, -self->miLayerOffset);
}

#undef OBJ_8A38
#undef VEC_8A38
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxSize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScreenPopup_getTextBoxSize1(cScreenPopup* self, void* text, unsigned short* str, float* size);
extern "C" void cScreenPopup_getTextBoxSize2_s(void* self, void* text, const char* str, float* size) __asm__("cScreenPopup_getTextBoxSize2");
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_String_pct_08x[];

struct sVEntry_8EF0 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

/**
 * Grows a text-box size to fit a popup string element, plus a gap.
 *
 * Picks mpMessageText when @p flag is nonzero, else mpPostText, and returns when it is null. By @p item's eType: 1
 * measures the unicode string with cScreenPopup_getTextBoxSize1(), 2 the ascii string with cScreenPopup_getTextBoxSize2(), 3 the localised string
 * (SSXApp->FELocale virtual slot 4) with cScreenPopup_getTextBoxSize1(), or the text "String %08x" with cScreenPopup_getTextBoxSize2() when the id
 * has no string. Afterwards adds @p extra to @p size[1], except for type 0 and a missing id.
 *
 * @param vself  the popup (cScreenPopup)
 * @param item   the string element: eType at [0], the string or id at [1]
 * @param size   the box size (t3Vector) to grow
 * @param flag   nonzero to measure with the message text, 0 with the post text
 * @param extra  gap added to the height
 */
extern "C" void cScreenPopup_getTextBoxSize(cScreenPopup* vself, int* item, float* size, int flag, float extra)
{
    void* text;
    if (flag != 0)
        text = (void*)vself->mpMessageText;
    else
        text = (void*)vself->mpPostText;
    if (text == 0)
        return;
    switch (item[0]) {
    case 0:
        break;
    case 2:
        cScreenPopup_getTextBoxSize2_s(((char*)vself), text, (const char*)item[1], size);
        goto add;
    case 1:
        cScreenPopup_getTextBoxSize1(vself, text, (unsigned short*)item[1], size);
        goto add;
    case 3: {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry_8EF0* vt = (sVEntry_8EF0*)locale->mVtbl;
        unsigned short* str = vt[4].fn(vthis(locale, vt[4]), item[1]);
        if (str) {
            cScreenPopup_getTextBoxSize1(vself, text, str, size);
            goto add;
        } else {
            char buf[0x40];
            sprintf(buf, str_String_pct_08x, item[1]);
            cScreenPopup_getTextBoxSize2_s(((char*)vself), text, buf, size);
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
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxSize1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cScreenPopup_getStringSize2(cScreenPopup* self, void* text, const char* str, void* out);
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item);
extern char str_getTextBoxSize[];
extern char gUSTRBackSlash1;

/**
 * Grows a text-box size to fit a unicode string, line by line.
 *
 * Splits a copy of @p str (allocated "getTextBoxSize") at each pair of backslashes (gUSTRBackSlash1 is a backslash) and each
 * '\r'; each line is measured with cScreenPopup_getStringSize2(), added to @p size with cScreenPopup_increaseBoxSize() and followed
 * by 3 units of height. The copy is freed at the end.
 *
 * @param text  the text widget whose font is used
 * @param str   the unicode string
 * @param size  the box size (t3Vector) to grow
 */
extern "C" void cScreenPopup_getTextBoxSize1(cScreenPopup* self, void* text, unsigned short* str, float* size)
{
    int j = 0;
    int i = 0;
    float item[4];
    unsigned short* buf = new (str_getTextBoxSize, 0x100, 0) unsigned short[USTR_length(str) + 1];
    buf[0] = 0;
    int n = USTR_length(str);
    while (i < n) {
        unsigned short curChar = str[i];
        unsigned short next = str[i + 1];
        if (curChar == gUSTRBackSlash1 && next == gUSTRBackSlash1) {
            buf[j] = 0;
            cScreenPopup_getStringSize2(self, text, (const char*)buf, item);
            i += 2;
            j = 0;
            cScreenPopup_increaseBoxSize(self, size, item);
            size[1] += 3.0f;
        } else if (curChar == '\r') {
            buf[j] = 0;
            cScreenPopup_getStringSize2(self, text, (const char*)buf, item);
            i += 1;
            j = 0;
            cScreenPopup_increaseBoxSize(self, size, item);
            size[1] += 3.0f;
        } else {
            buf[j] = curChar;
            i++;
            j++;
        }
    }
    if (j != 0) {
        buf[j] = 0;
        cScreenPopup_getStringSize2(self, text, (const char*)buf, item);
        cScreenPopup_increaseBoxSize(self, size, item);
        size[1] += 3.0f;
    }
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxSize2);
#ifdef SKIP_ASM
struct cUIText;
extern "C" int strlen(const char* s);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cScreenPopup_getStringSize1(cScreenPopup* self, void* text, const char* str, void* out);
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item);
extern char str_getTextBoxSize[];

/**
 * Grows a text-box size to fit an ascii string, line by line.
 *
 * Splits a copy of @p str (allocated "getTextBoxSize") at each pair of backslashes and each '\r'; each line is measured
 * with cScreenPopup_getStringSize1() and added to the size with cScreenPopup_increaseBoxSize(). Lines ended by a separator add 3
 * units of height; unlike cScreenPopup_getTextBoxSize1() the last line does not. The copy is freed at the end.
 *
 * @param text     the text widget whose font is used
 * @param str      the ascii string
 * @param sizePtr  the box size (a float* to a t3Vector, declared int)
 */
// PORT: a3 is the float* size out pointer (the unit declares it int)
extern "C" void cScreenPopup_getTextBoxSize2(cScreenPopup* self, cUIText* text, const char* str, int sizePtr)
{
    float* size = (float*)sizePtr;
    int j = 0;
    int i = 0;
    float item[4];
    char* buf = new (str_getTextBoxSize, 0x100, 0) char[strlen(str) + 1];
    buf[0] = 0;
    int n = strlen(str);
    while (i < n) {
        char chr = str[i];
        char next = str[i + 1];
        if (chr == '\\' && next == '\\') {
            buf[j] = 0;
            cScreenPopup_getStringSize1(self, text, buf, item);
            i += 2;
            cScreenPopup_increaseBoxSize(self, size, item);
            j = 0;
            size[1] += 3.0f;
        } else if (chr == '\r') {
            buf[j] = 0;
            cScreenPopup_getStringSize1(self, text, buf, item);
            i += 1;
            cScreenPopup_increaseBoxSize(self, size, item);
            j = 0;
            size[1] += 3.0f;
        } else {
            buf[j] = chr;
            i++;
            j++;
        }
    }
    if (j != 0) {
        buf[j] = 0;
        cScreenPopup_getStringSize1(self, text, buf, item);
        cScreenPopup_increaseBoxSize(self, size, item);
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
extern "C" void cScreenPopup_getMessageLineWrapSize2(cScreenPopup* self, void* text, void* str, void* out);
extern char str_String_pct_08x[];


/**
 * Measures the first wrapped line of a popup message with the popup's message text object.
 *
 * Does nothing without mpMessageText. ASCII text goes to cScreenPopup_getMessageLineWrapSize(), unicode to
 * cScreenPopup_getMessageLineWrapSize2; a text id is looked up in the front-end locale first, or measured as "String %08x" when it
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
        cScreenPopup_getMessageLineWrapSize2(self, self->mpMessageText, (void*)msg[1], out);
        break;
    case 3: {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)locale->mVtbl;
        void* str = vt[4].fn(vthis(locale, vt[4]), msg[1]);
        if (str) {
            cScreenPopup_getMessageLineWrapSize2(self, self->mpMessageText, str, out);
        } else {
            char buf[0x40];
            sprintf(buf, str_String_pct_08x, msg[1]);
            cScreenPopup_getMessageLineWrapSize(self, self->mpMessageText, buf, out);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getMessageLineWrapSize2);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec3_94B0 { float x, y, z; };
struct sVEntry_94B0 {
    short delta;
    short index;
    void (*fn)(void*, t2Vector*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_charAdvance1(void* font, int c);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UITEXTTEMP[];
extern char gUSTRBackSlash1;

/**
 * Measures how wide the first line of a unicode string gets before it would overflow the text widget.
 *
 * Sets bit 7 of the widget's mAttr, then walks a copy of @p strp (allocated "UITEXTTEMP") adding each character's width
 * (cFont_charAdvance1 times mScale.x); a pair of backslashes (gUSTRBackSlash1) restarts the width at 0. Stops at the first
 * character that makes the width exceed the widget's width (getSize, vtable slot 20) and stores the width before it in
 * @p outp; otherwise stores the full width. Returns early, without writing, when the widget has no font.
 *
 * @param text  the text widget
 * @param strp  the unicode string
 * @param outp  receives the width (float)
 */
extern "C" void cScreenPopup_getMessageLineWrapSize2(cScreenPopup* self, void* text, void* strp, void* outp)
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
    unsigned short* buf = new (str_UITEXTTEMP, 0x100, 0) unsigned short[n + 1];
    USTR_copy(buf, str);
    sVec3_94B0 textScale;
    t2Vector size;
    sVEntry_94B0* vt = *(sVEntry_94B0**)((char*)text + 8);
    vt[20].fn(vthis(text, vt[20]), &size);
    textScale = *(sVec3_94B0*)((char*)text + 0x50);
    while (i < n) {
        if (buf[i] == gUSTRBackSlash1 && buf[i + 1] == gUSTRBackSlash1) {
            w = 0.0f;
            i += 2;
        } else {
            float cw = cFont_charAdvance1(font, buf[i]) * textScale.x;
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
extern "C" float cFont_charAdvance(void* font, int c);  // cFont::charAdvance(char) const
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_getMessageLineWrapSize[];

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
    char* pTempString = new (str_getMessageLineWrapSize, 0x100, 0) char[length + 1];
    strcpy(pTempString, pString);
    int j = 0;
    float Pixels = 0.0f;
    bool bFound = false;
    t3Vector_9648 vScale;
    t3Vector_9648 vTextSize;
    sVEntry_9648* vt = *(sVEntry_9648**)((char*)pText + 8);
    vt[20].fn(vthis(pText, vt[20]), &vTextSize);
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
            float advance = cFont_charAdvance(pFont, pTempString[j]) * vScale.x;
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
extern "C" void cScreenPopup_setTextString2(cScreenPopup* self, cUIText* text, int* info);
extern "C" void cScreenPopup_getTextBoxWrapSize1(cScreenPopup* self, cUIText* text, void* str, float* out);
// PORT: cScreenPopup_getTextBoxWrapSize2's 4th parameter is the out pointer (declared int in the unit)
extern "C" void cScreenPopup_getTextBoxWrapSize2(cScreenPopup* self, cUIText* text, const char* str, int a3);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_String_pct_08x[];


/**
 * Sets the popup's message or post text from a popup string and grows a size by the wrapped text plus a gap.
 *
 * Picks mpMessageText when @p which is nonzero, else mpPostText (nothing without one). By the string's type: 2
 * ascii (cScreenPopup_getTextBoxWrapSize2) and 1 unicode (cScreenPopup_getTextBoxWrapSize1) wrap the text into @p vSize; 3 looks the id up in the FE locale
 * (slot 4) and wraps that string, or, when missing, the ascii text "String %08x" and returns without the gap; 0
 * only sets the text (cScreenPopup_setTextString2) and returns. Otherwise adds @p bMessage to the size's y.
 *
 * @param ptElement  the popup string (On Tour tPopupElement* ptElement): type word, then the value
 * @param vSize  the size to grow (On Tour t3Vector& vSize)
 * @param which  nonzero for the message text, 0 for the post text (On Tour bMessage)
 * @param bMessage  the gap added below the text (On Tour fGapDistance)
 */
extern "C" void cScreenPopup_getTextBoxWrapSize(cScreenPopup* self, void* ptElement, void* vSize, int which, float bMessage)
{
    int* info = (int*)ptElement;
    float* out = (float*)vSize;
    cUIText* text;
    if (which != 0)
        text = self->mpMessageText;
    else
        text = self->mpPostText;
    if (text == 0)
        return;
    switch (info[0]) {
    case 2:
        cScreenPopup_getTextBoxWrapSize2(self, text, (const char*)info[1], (int)out);
        break;
    case 1:
        cScreenPopup_getTextBoxWrapSize1(self, text, (void*)info[1], out);
        break;
    case 3: {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)locale->mVtbl;
        void* str = vt[4].fn(vthis(locale, vt[4]), info[1]);
        if (str) {
            cScreenPopup_getTextBoxWrapSize1(self, text, str, out);
        } else {
            char buf[0x40];
            sprintf(buf, str_String_pct_08x, info[1]);
            cScreenPopup_getTextBoxWrapSize2(self, text, buf, (int)out);
            return;
        }
        break;
    }
    case 0:
        cScreenPopup_setTextString2(self, text, info);
        return;
    }
    out[1] += bMessage;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxWrapSize1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
struct sVec3_9938 { float x, y, z; };
struct sVEntry_9938 {
    short delta;
    short index;
    void (*fn)(void*, t2Vector*);
};
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_charAdvance1(void* font, int c);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern "C" void cScreenPopup_getTextBoxSize1(cScreenPopup* self, void* text, unsigned short* str, float* size);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UITEXTTEMP[];
extern char gUSTRBackSlash1;

/**
 * Word-wraps a unicode string to a text widget's width, grows a box size to fit it, and puts it in the widget.
 *
 * Sets bit 7 of the widget's mAttr; returns when the widget has no font. On a copy of @p strp (allocated "UITEXTTEMP")
 * it adds character widths (cFont_charAdvance1 times mScale.x), restarting at backslash pairs and '\r'; when the width
 * passes the widget's width (getSize) after a space was seen, that space becomes '\r' and scanning resumes after it.
 * The wrapped copy is measured into @p out with cScreenPopup_getTextBoxSize1() and set with cUIText_setUnicodeString(), then freed.
 *
 * @param text  the text widget
 * @param strp  the unicode string
 * @param out   the box size (t3Vector) to grow
 */
extern "C" void cScreenPopup_getTextBoxWrapSize1(cScreenPopup* self, cUIText* text, void* strp, float* out)
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
    unsigned short* buf = new (str_UITEXTTEMP, 0x100, 0) unsigned short[n + 1];
    USTR_copy(buf, str);
    sVec3_9938 textScale;
    t2Vector size;
    sVEntry_9938* vt = *(sVEntry_9938**)((char*)text + 8);
    vt[20].fn(vthis(text, vt[20]), &size);
    textScale = *(sVec3_9938*)((char*)text + 0x50);
    while (i < n) {
        if (buf[i] == gUSTRBackSlash1 && buf[i + 1] == gUSTRBackSlash1) {
            last = -1;
            w = 0.0f;
            i += 2;
        } else {
            if (buf[i] == '\r') {
                w = 0.0f;
                last = -1;
            } else {
                float cw = cFont_charAdvance1(font, buf[i]) * textScale.x;
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
    cScreenPopup_getTextBoxSize1(self, text, buf, out);
    cUIText_setUnicodeString(text, buf);
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cScreenPopup_getTextBoxWrapSize2);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void cScreenPopup_getTextBoxSize2(cScreenPopup* self, cUIText* text, const char* str, int a3);

/**
 * Grows a box size to fit an ascii string and puts the string in the text widget.
 *
 * Runs cScreenPopup_getTextBoxSize2() and then cUIText_setAsciiString(); the ascii string is not wrapped.
 *
 * @param text     the text widget
 * @param str      the ascii string
 * @param sizePtr  the box size (a float* to a t3Vector, declared int)
 */
extern "C" void cScreenPopup_getTextBoxWrapSize2(cScreenPopup* self, cUIText* text, const char* str, int sizePtr)
{
    cScreenPopup_getTextBoxSize2(self, text, str, sizePtr);
    cUIText_setAsciiString(text, str);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_increaseBoxSize);
#ifdef SKIP_ASM
/**
 * Grows a box so a line of text fits: as wide as the widest line, as tall as all lines together.
 *
 * Sets @p size x to the rectangle's width (+0x8) when that is larger, and adds the rectangle's height (+0xC) to
 * @p size y.
 *
 * @param size  the box size being accumulated (On Tour vBox)
 * @param item  the line's rectangle (On Tour rectString)
 */
extern "C" void cScreenPopup_increaseBoxSize(cScreenPopup* self, float* size, float* item)
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
extern "C" void* cScreenPopup_cScreenPopup(cScreenPopup* mem, cUIEngine* engine, cUIState* owner);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_Confirmation_Popup[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];

struct sVec3K1C9B98 {
    float x, y, z;
    sVec3K1C9B98(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sEntryK1C9B98 { tPopupElement p; int c; };
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
    cScreenPopup* popup = (cScreenPopup*)cScreenPopup_cScreenPopup((cScreenPopup*)cMemMan_alloc(864, str_Confirmation_Popup, 0x100, 0), self->mpEngine, (cUIState*)self);
    self->mpConfirmPopup = (cScreenPopup*)popup;
    char* slots = (char*)&popup->info;
    popup->info.meType = 2;
    func_001C57C0(slots, 0, GetHashValue32(str_kT_CMNYes));
    func_001C57C0(slots, 1, GetHashValue32(str_kT_CMNNo));
    sVec3K1C9B98 popupOffset(30.0f, 40.0f, 0.0f);
    *(sVec3K1C9B98*)&popup->info.mvOffset[0] = popupOffset;
    popup->info.miLayerOffset = self->miLayerOffset + 3;
    popup->info.miDefaultOption = ((sSelfK1C9B98*)self)->entries[idx].c;
    *(&popup->info.mtMessage) = ((sSelfK1C9B98*)self)->entries[idx].p;
    self->miActiveConfirmation = idx;
    cUIStateStack_pushPopup((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18,
                  self->mpConfirmPopup);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_adjustLayer);
#ifdef SKIP_ASM
extern char str_ngca[];
extern char str_xba[];
extern char str_ps2x[];

struct sUIObj_C9CD0 {
    char pad_0x0[0x14];
    int flags; // 0x14: bits 8..12 = layer (out-of-line copies: cUIThing_getDrawLayer get, cUIThing_setDrawLayer set)
    int getLayer() { return ((unsigned int)flags >> 8) & 0x1F; }
    void setLayer(int l) { flags = (flags & ~0x1F00) | ((l & 0x1F) << 8); }
};

/**
 * Moves every widget of the popup up or down by a number of draw layers.
 *
 * Adds @p v to the layer of each popup widget pointer (+0x48 onwards) that is set, and of the "ngca", "xba" and
 * "ps2x" button objects on the screen.
 *
 * @param v  the layer offset
 */
extern "C" void cScreenPopup_adjustLayer(void* self, int v)
{
    char* s = (char*)self;
    sUIObj_C9CD0* o;
    // NOTE: one `layer` local shared by every block (hand-repeated code); a per-block temp
    // gives different registers.
    int layer;
#define ADD_LAYER(e) o = (e); if (o) { layer = o->getLayer(); o->setLayer(layer + v); }
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 72));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 76));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 80));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 84));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 88));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 92));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x60));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 100));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 104));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 116));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x7C));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x78));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 0x80));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 132));
    ADD_LAYER(*(sUIObj_C9CD0**)(s + 140));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(str_ngca)));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(str_xba)));
    ADD_LAYER((sUIObj_C9CD0*)cUIScreen_getObjectByHashName(*(void**)(s + 0x40), GetHashValue32(str_ps2x)));
    for (int i = 0; i < 5; i++) {
        ADD_LAYER(((sUIObj_C9CD0**)(s + 144))[i]);
    }
#undef ADD_LAYER
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_setStatic);
#ifdef SKIP_ASM
struct sVEntry_A0F0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Greys out or re-enables one popup option: setStatic(@p bStatic) and setUseStaticColour(1) on mpOptions[idx].
 *
 * Does nothing when that option widget does not exist.
 *
 * @param idx  the option index (On Tour iEntryNum)
 * @param bStatic  nonzero to make the option static (unselectable)
 */
extern "C" void cScreenPopup_setStatic(cScreenPopup* self, int idx, int bStatic)
{
    char* option = (char*)self->mpOptions[idx];
    if (option != 0) {
        sVEntry_A0F0* vt = *(sVEntry_A0F0**)(option + 8);
        vt[8].fn(vthis(option, vt[8]), bStatic);
        sVEntry_A0F0* vt2 = *(sVEntry_A0F0**)(option + 8);
        vt2[25].fn(vthis(option, vt2[25]), 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_getCurrentlySelected);
#ifdef SKIP_ASM
/**
 * Returns the popup's selected option.
 *
 * Without a menu, returns info.miDefaultOption. Otherwise returns the menu's mScrollItemCurrent, less 1 unless the
 * popup is a text-entry popup (meType -1).
 *
 * @return the selected option index
 */
extern "C" int cScreenPopup_getCurrentlySelected(cScreenPopup* self)
{
    void* menu = self->mpMenu;
    if (menu == 0) {
        return self->info.miDefaultOption;
    }
    int n = *(unsigned char*)((char*)menu + 0x95);
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
extern "C" char* cList_getByIndex(void* list, int idx);

struct sVE1CA180 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

static inline void SetColour1CA180(char* obj, void* colour)
{
    sVE1CA180* vt = *(sVE1CA180**)(obj + 8);
    vt[11].fn(vthis(obj, vt[11]), colour);
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
            char* obj = cList_getByIndex(menu + 0x74, i);
            if (obj == (char*)self->mpTextBoxPair) {
                obj = (char*)self->mpStaticBox;
            }
            if (i == sel) {
                SetColour1CA180(obj, &self->mtHighlightedColor[0]);
            } else {
                SetColour1CA180(obj, &self->mtUnhighlightColor[0]);
            }
        }
        break;
    }
    case 8: case 9:
        return 0;
    case 7:
        return self->mbReceivePrev ? 257 : 0;
    case 6:
        return self->mbNoEntryOnNext ? 0 : 257;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cScreenPopup_createConfirmationPopup(cScreenPopup* self, int idx);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(cKeyboardPopup* self, cUIEngine* engine, cUIState* owner, int a3, unsigned char a4);
extern "C" void cKeyboardPopup_setMaxCharacters(cKeyboardPopup* popup, int a1);
extern "C" void cUIStateStack_pushPopup(void* stack, void* item);
extern char str_cKeyboardPopup[];

struct sPopupVE_1CA298 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};



static inline void Notify_1CA298(cUIState* o, int vi, void* self, int v)
{
    ((sPopupVE_1CA298*)o->mVtbl)[vi].fn(vthis(o, ((sPopupVE_1CA298*)o->mVtbl)[vi]), self, v);
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
                if (id == ((int*)(&self->mpOptions[0]))[i]) {
                    if ((((&self->mtConfirmation[0])))[i].eType != 0) {
                        cScreenPopup_createConfirmationPopup(self, i);
                    } else {
                        Notify_1CA298(self->mpParentState, 17, ((char*)self), i + 15);
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
            void* mem = cMemMan_alloc(1092, str_cKeyboardPopup, 0x100, 0);
            self->mpKeyboard = (cKeyboardPopup*)cKeyboardPopup_cKeyboardPopup((cKeyboardPopup*)mem, self->mpEngine, (cUIState*)(((char*)self)), 0, 0xF);
            cKeyboardPopup_setMaxCharacters(self->mpKeyboard, self->miMaxChars);
            cUIStateStack_pushPopup(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, self->mpKeyboard);
            if (self->mpStaticBox != 0) {
                cScreenPopup_setTextString(self, &self->mszStaticText, self->mpStaticBox);
            }
        } else {
            self->mbNoEntryOnNext = 1;
        }
        break;
    case 6:
        if (self->mbReceivePrev != 0) {
            Notify_1CA298(self->mpParentState, 17, ((char*)self), 20);
        }
        break;
    case 7:
    case 8:
        break;
    case 13:
        cBXString_operatorE1(&self->mszInputText, self->mpKeyboard->mszEditString);
        if (self->mpEnterBox != 0 && *(int*)(self->mszInputText - 8) != 0) {
            cScreenPopup_checkAndFixTextEntry(self, self->mpEnterBox, (const char*)self->mszInputText);
        }
        break;
    default:
        {
            cUIState* parentState = self->mpParentState;
            ((void (*)(void*, int, int))((sPopupVE_1CA298*)parentState->mVtbl)[19].fn)(vthis(parentState, ((sPopupVE_1CA298*)parentState->mVtbl)[19]), id, ev);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_hideWidgetSafe);
#ifdef SKIP_ASM
struct sVEntry_cScreenPopup_hideWidgetSafe {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Hides a widget if there is one.
 *
 * Calls virtual slot 9 (set visible) with 0 when @p obj is not null.
 *
 * @param obj  the widget, or null
 */
extern "C" void cScreenPopup_hideWidgetSafe(cScreenPopup* self, void* obj)
{
    if (obj != 0) {
        sVEntry_cScreenPopup_hideWidgetSafe* vt = *(sVEntry_cScreenPopup_hideWidgetSafe**)((char*)obj + 0x8);
        vt[9].fn(vthis(obj, vt[9]), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_showWidgetSafe);
#ifdef SKIP_ASM
struct sVEntry_cScreenPopup_showWidgetSafe {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows a widget if there is one.
 *
 * Calls virtual slot 9 (set visible) with 1 when @p obj is not null.
 *
 * @param obj  the widget, or null
 */
extern "C" void cScreenPopup_showWidgetSafe(cScreenPopup* self, void* obj)
{
    if (obj != 0) {
        sVEntry_cScreenPopup_showWidgetSafe* vt = *(sVEntry_cScreenPopup_showWidgetSafe**)((char*)obj + 0x8);
        vt[9].fn(vthis(obj, vt[9]), 1);
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
    virtual int handleEvent(int, int);
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
    return ((cScreenPopup_handleEvent_cObj*)self->mpParentState)->handleEvent(msg, pParam);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onUpdate);
#ifdef SKIP_ASM

extern "C" void cUIState_onUpdate(void*);

/**
 * Updates the popup, first updating its parent state when the popup passes updates through.
 *
 * When mbPassUpdate is set, calls the parent state's onUpdate (vtable slot 12); then runs the base UI-state
 * update cUIState_onUpdate().
 */
extern "C" void cScreenPopup_onUpdate(cScreenPopup* self)
{
    if (self->mbPassUpdate != 0) {
        cUIState* obj = self->mpParentState;
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)obj->mVtbl;
        vt[12].fn(vthis(obj, vt[12]));
    }
    cUIState_onUpdate(self);
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the popup's 2D layer with the shared UI-screen render routine cUIState_onRender2D().
 *
 * @return no meaningful value (cUIState_onRender2D() returns nothing)
 */
void* cScreenPopup_onRender2D(void* self)
{
    return cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cScreenPopup_onPopupEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cKeyboardPopup_setInputText(cKeyboardPopup* popup, const char* a1);
extern "C" void cKeyboardPopup_setStatic2(cKeyboardPopup* popup, const char* a1);
// PORT: the unit declares cUIStateStack_pop as void; it returns a value this caller ignores.
extern "C" int cUIStateStack_pop_i(void* self, int a1) __asm__("cUIStateStack_pop");


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
extern "C" void cScreenPopup_onPopupEvent(cScreenPopup* self, cUIState* child, int msg)
{
    switch (msg) {
    case 21:
        if (((void*)child) == (void*)self->mpKeyboard) {
            cKeyboardPopup_setInputText((cKeyboardPopup*)child, (const char*)self->mszInputText);
            cKeyboardPopup_setStatic2(self->mpKeyboard, (const char*)self->mszStaticText);
        }
        break;
    case 0xF:
        cUIStateStack_pop_i(&self->mpEngine->mStateStack, 1);
        self->miConfimationReturnCode = self->miActiveConfirmation + 0xF;
        break;
    case 0x10: case 20:
        self->miConfimationReturnCode = -1;
        cUIStateStack_pop_i(&self->mpEngine->mStateStack, 1);
        break;
    case 17: case 18: case 19:
        cUIStateStack_pop_i(&self->mpEngine->mStateStack, 1);
        break;
    case 22:
        if (((void*)child) == (void*)self->mpKeyboard) {
            self->mpKeyboard = 0;
        }
        if (((void*)child) == (void*)self->mpConfirmPopup) {
            self->mpConfirmPopup = 0;
            if (self->miConfimationReturnCode != -1) {
                cUIState* obj = self->mpParentState;
                sVEntry<void(void*, void*, int)>* vt = (sVEntry<void(void*, void*, int)>*)obj->mVtbl;
                vt[17].fn(vthis(obj, vt[17]), ((char*)self), self->miConfimationReturnCode);
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
extern "C" char* strcat(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cMemMan_free(void* ptr);
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_checkAndFixTextEntry[];
extern char str_dot_dot_dot[];

static inline void measure_A6A0(cUIText* text, const char* s, float* out)
{
    cFont* f = (cFont*)cUIText_getFont(text);
    cFont_textExtent(f, s, out, 0, f->Scale[0], f->Scale[1]);
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
    vt[20].fn(vthis(text, vt[20]), &size);
    cFont* font = (cFont*)cUIText_getFont(text);
    float sx = font->BaseScale[0] * scale.x;
    float sy = font->BaseScale[1] * scale.y;
    font->Scale[0] = sx;
    font->Scale[1] = sy;
    char* buf = new (str_checkAndFixTextEntry, 0x100, 0) char[self->miMaxChars + 4];
    int len = strlen(str) - 1;
    strcpy(buf, str);
    const char* dotstr = str_dot_dot_dot;
    measure_A6A0(text, dotstr, dots);
    measure_A6A0(text, buf, ext);
    while (size.x - dots[2] - 1.0f < ext[2]) {
        fixed = 1;
        buf[len] = 0;
        len--;
        measure_A6A0(text, buf, ext);
    }
    if (fixed) {
        strcat(buf, dotstr);
        cUIText_setAsciiString(text, buf);
    } else {
        cUIText_setAsciiString(text, str);
    }
    if (buf != 0)
        cMemMan_free(buf);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_clear);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Clears a buy popup's description: the item-name, item-type and question elements, mCost, mMoney and mCreditsLeft.
 */
extern "C" void cBuyPopupInfo_clear(cBuyPopupInfo* self)
{
    MEM_fill((char*)&self->mItemType, 0, 8);
    MEM_fill((char*)&self->mItemName, 0, 8);
    MEM_fill((char*)&self->mQuestion, 0, 8);
    self->mCost = 0;
    self->mMoney = 0;
    self->mCreditsLeft = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySongByCredit);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_OVRSong[];
extern char str_kT_OVRCMNBuySongCredit[];

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
    int hash = GetHashValue32(str_kT_OVRSong);
    self->mItemName.ui = song;
    self->mItemType.eType = three;
    self->mItemName.eType = 2;
    self->mItemType.ui = hash;
    hash = GetHashValue32(str_kT_OVRCMNBuySongCredit);
    self->mCreditsLeft = credit;
    self->mQuestion.eType = three;
    self->mQuestion.ui = hash;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySong);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_OVRSong[];
extern char str_kT_16BuyMusicTrack[];

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
    int hash = GetHashValue32(str_kT_OVRSong);
    self[4] = songName;
    self[5] = three;
    self[3] = 2;
    self[6] = hash;
    hash = GetHashValue32(str_kT_16BuyMusicTrack);
    self[1] = money;
    self[7] = three;
    self[0] = cost;
    self[2] = -1;
    self[8] = hash;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuyReward);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern char str_kT_OVRCMNBuyItem[];
extern char str_kT_OVRCMNPosterColon[];
extern char str_kT_OVRCMNCardColon[];
extern char str_kT_OVRCMNArtColon[];
extern char str_kT_OVRCMNVideoColon[];
extern char str_kT_OVRCMNToyColon[];
extern char str_kT_OVRCMNCheatCharColon[];

/**
 * Fills a buy popup for a reward: price, cash, the reward's name and type, and the "buy item" question.
 *
 * Sets mCost to @p cost, mMoney to @p money and mCreditsLeft to -1; mItemName is the ascii string @p rewardName, mQuestion the text
 * id of "kT_OVRCMNBuyItem". mItemType gets the text id for @p kind: 3 "kT_OVRCMNPosterColon", 4 "kT_OVRCMNCardColon", 5
 * "kT_OVRCMNArtColon", 6 "kT_OVRCMNVideoColon", 7 "kT_OVRCMNToyColon", 8 "kT_OVRCMNCheatCharColon"; any other kind
 * leaves it unchanged.
 *
 * @param desc  the cBuyPopupInfo to fill
 * @param kind  the reward type (3..8)
 * @param rewardName    the reward's name (ascii string, passed as int)
 * @param cost    the price
 * @param money    the player's cash
 */
extern "C" void cBuyPopupInfo_initBuyReward(int* desc, int kind, int rewardName, int cost, int money)
{
    desc[4] = rewardName;
    desc[3] = 2;
    desc[8] = GetHashValue32(str_kT_OVRCMNBuyItem);
    desc[7] = 3;
    desc[2] = -1;
    desc[0] = cost;
    desc[1] = money;
    switch (kind) {
    case 3:
        desc[6] = GetHashValue32(str_kT_OVRCMNPosterColon);
        desc[5] = 3;
        break;
    case 4:
        desc[6] = GetHashValue32(str_kT_OVRCMNCardColon);
        desc[5] = 3;
        break;
    case 5:
        desc[6] = GetHashValue32(str_kT_OVRCMNArtColon);
        desc[5] = 3;
        break;
    case 6:
        desc[6] = GetHashValue32(str_kT_OVRCMNVideoColon);
        desc[5] = 3;
        break;
    case 7:
        desc[6] = GetHashValue32(str_kT_OVRCMNToyColon);
        desc[5] = 3;
        break;
    case 8:
        desc[6] = GetHashValue32(str_kT_OVRCMNCheatCharColon);
        desc[5] = 3;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuyBolt);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_CMNItemColon[];
extern char str_kT_OVRCMNBuyItem[];

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
    int hash = GetHashValue32(str_kT_CMNItemColon);
    self[4] = boltName;
    self[5] = three;
    self[3] = 2;
    self[6] = hash;
    hash = GetHashValue32(str_kT_OVRCMNBuyItem);
    self[1] = money;
    self[7] = three;
    self[0] = cost;
    self[2] = -1;
    self[8] = hash;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_cUIStateBuyPopup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cBuyPopupInfo_clear(cBuyPopupInfo* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern void* cUIStateBuyPopup_vtbl[];

/**
 * Constructs the buy-confirmation popup state for one user.
 *
 * Runs the cUIState constructor (cUIState_cUIState1), stores cUIStateBuyPopup_vtbl, clears the embedded cBuyPopupInfo
 * (+0x48, cBuyPopupInfo_clear) twice (before and after the other stores), stores @p idx as mUserNum and mHashName 7, and
 * when the engine's first word is set takes mCtrlBits from cFECustom_getPlayerCtrlMask(that word, idx).
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param owner  the parent state (On Tour pParentState)
 * @param idx  the user the popup belongs to (On Tour userNum)
 * @return @p self
 */
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(cUIStateBuyPopup* self, cUIEngine* engine, cUIState* owner, signed char idx)
{
    cUIState_cUIState1(self, engine, owner);
    self->mVtbl = cUIStateBuyPopup_vtbl;
    cBuyPopupInfo_clear((cBuyPopupInfo*)&self->mInfo);
    self->mUserNum = idx;
    self->mHashName = 7;
    void* custom = *(void**)self->mpEngine;
    if (custom != 0) {
        *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(custom, idx);
    }
    cBuyPopupInfo_clear((cBuyPopupInfo*)&self->mInfo);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_139buy_popup[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the buy popup's screen "139buy_popup" and plays its first frame.
 */
extern "C" void cUIStateBuyPopup_onCreateScreen(cUIStateBuyPopup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_139buy_popup), 0);
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
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIText_setUnicodeString(cUIText* text, void* p);
extern "C" char* getDollarString(void* p);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_item_type[];
extern char str_item_name[];
extern char str_question[];
extern char str_cost_answer[];
extern char str_you_have_answer[];
extern char str_credits_remain[];
extern char str_group_credit[];
extern char str_group_cash[];
extern char str_pct_d_b[];

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
    if (id == GetHashValue32(str_item_type)) {
        int type = *(int*)((char*)self + 0x5C);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x60));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x60));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x60));
    } else if (id == GetHashValue32(str_item_name)) {
        int type = *(int*)((char*)self + 0x54);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x58));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x58));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x58));
    } else if (id == GetHashValue32(str_question)) {
        int type = *(int*)((char*)self + 0x64);
        if (type == 2)
            cUIText_setAsciiString(text, *(const char**)((char*)self + 0x68));
        else if (type == 1)
            cUIText_setUnicodeString(text, *(void**)((char*)self + 0x68));
        else if (type == 3)
            cUIText_setUnicodeStringByID(text, *(int*)((char*)self + 0x68));
    } else if (id == GetHashValue32(str_cost_answer)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            cUIText_setAsciiString(text, getDollarString(*(void**)((char*)self + 0x48)));
    } else if (id == GetHashValue32(str_you_have_answer)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            cUIText_setAsciiString(text, getDollarString(*(void**)((char*)self + 0x4C)));
    } else if (id == GetHashValue32(str_credits_remain)) {
        int n = *(int*)((char*)self + 0x50);
        if (n > 0) {
            char buf[16];
            sprintf(buf, str_pct_d_b, n);
            cUIText_setAsciiString(text, buf);
        }
    } else if (id == GetHashValue32(str_group_credit)) {
        if (*(int*)((char*)self + 0x50) <= 0)
            ((cUIObj_AC98*)widget)->setVisible(0);
    } else if (id == GetHashValue32(str_group_cash)) {
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
    int result = 257;
    if (key < 10) {
        if (key < 7) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes[];

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
            int widgetHash = *(int*)((char*)item + 0x38);
            if (widgetHash == GetHashValue32(str_yes)) {
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
extern char str_kT_OVRCMNUbertrick[];
extern char str_kT_OVRCMNBuyThisUber[];

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
    self->mInfo.mItemName.ui = trick;
    self->mInfo.mItemName.eType = 2;
    int hash = GetHashValue32(str_kT_OVRCMNUbertrick);
    self->mInfo.mItemType.eType = three;
    self->mInfo.mItemType.ui = hash;
    hash = GetHashValue32(str_kT_OVRCMNBuyThisUber);
    self->mInfo.mQuestion.eType = three;
    self->mInfo.mQuestion.ui = hash;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_initBuyAttributes);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_OVRCMNBuyAttributesColon[];
extern char D_004A1408[];

/**
 * Sets up the buy popup for buying attribute points.
 *
 * Stores @p cost and @p money, the item type as the text id "kT_OVRCMNBuyAttributesColon", and both the item name and
 * the question as the empty ascii string. @p pAttrName is never read.
 *
 * @param pAttrName  the attribute's name (unused)
 * @param cost  the price
 * @param money  the player's money
 */
extern "C" void cUIStateBuyPopup_initBuyAttributes(cUIStateBuyPopup* self, int pAttrName, int cost, int money)
{
    char* defaultStr = D_004A1408;
    int two = 2;
    self->mInfo.mItemName.pustr = (const unsigned short*)defaultStr;
    self->mInfo.mItemName.eType = two;
    self->mInfo.mCost = cost;
    self->mInfo.mMoney = money;
    int hash = GetHashValue32(str_kT_OVRCMNBuyAttributesColon);
    self->mInfo.mQuestion.eType = two;
    self->mInfo.mQuestion.pustr = (const unsigned short*)defaultStr;
    self->mInfo.mItemType.eType = 3;
    self->mInfo.mItemType.ui = hash;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_cKeyboardPopup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cBXString_Reset(void* self);
extern "C" void cKeyboardPopup_fillSelectableKeys(cKeyboardPopup* self);
extern int cBXString_EmptyString;
extern void* Keyboard;
extern void* cKeyboardPopup_vtbl[];

/**
 * Constructs the on-screen keyboard popup: caps on, cursor at key 29, a 63-character limit and the default key set.
 *
 * Runs the cUIState constructor (cUIState_cUIState1), stores @p CtrlBits in mCtrlBits and @p Password in mbPassword, clears the edit
 * string (+0x74), cursor, view range and flags (mbPopOnEnd 1, mbCapsOn 1), stores cKeyboardPopup_vtbl, miCurrentKey
 * 0x1D, mszStatic = the empty string, miMaxCharacters 63, meSkipState 2 (both), miLastKey 0x2B and meType 1
 * (compact); resets the cBXString at +0x424 and clears the static texts and the three backgrounds. When Keyboard
 * is set, mbCapsOn comes from bit 1 of its word +0x88. Ends with cKeyboardPopup_fillSelectableKeys().
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param owner  the parent state (On Tour pParentState)
 * @param Password  nonzero for a password entry (On Tour Password; stored in mbPassword)
 * @param CtrlBits  the controller bits (On Tour CtrlBits; stored in mCtrlBits)
 * @return @p self
 */
extern "C" void* cKeyboardPopup_cKeyboardPopup(cKeyboardPopup* self, cUIEngine* engine, cUIState* owner, int Password, unsigned char CtrlBits)
{
    cUIState_cUIState1(self, engine, owner);
    int defaultStr = cBXString_EmptyString;
    self->mCtrlBits = CtrlBits;
    self->mbPassword = Password;
    self->mbSetInputText = 0;
    self->mbTransistionDone = 0;
    self->mbPopOnEnd = 1;
    self->miMinCharacters = 0;
    self->mbShiftOn = 0;
    self->mbCapsOn = 1;
    self->mszEditString[0] = 0;
    self->miCursorPos = 0;
    self->miViewableStartIndex = 0;
    self->miViewableEndIndex = 0;
    self->mbClearOnFirst = 0;
    self->mbDisableForFirst = 0;
    self->mVtbl = cKeyboardPopup_vtbl;
    self->miCurrentKey = 29;
    self->mszStatic = (char*)defaultStr;
    self->miMaxCharacters = 0x3F;
    cBXString_Reset((char*)&self->mszStatic);
    cUsbKeyboard* bx = (cUsbKeyboard*)Keyboard;
    self->meSkipState = 2;
    self->miLastKey = 43;
    self->meType = 1;
    self->mpustrStatic = 0;
    self->muiStatic = (unsigned int)0;
    self->meLastDirection = 0;
    self->mbReferEventsToParent = 0;
    self->mpBackCompact = 0;
    self->mpBackRegular = 0;
    self->mpBackExpand = 0;
    if (((void*)bx) != 0)
        self->mbCapsOn = (bx->mSlots[0].mFlags >> 1) & 1;
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
extern "C" void cUIState__cUIState(void* self, int flags);

/**
 * Destroys the on-screen keyboard: frees its pending unicode static text and its static string.
 *
 * Stores the class vtable, frees mpustrStatic when set, destroys the mszStatic cBXString and runs the
 * cUIState destructor (cUIState__cUIState), which also frees the object when the flags ask for it.
 *
 * @param flags  destructor flags, passed on to cUIState__cUIState
 */
extern "C" void cKeyboardPopup__cKeyboardPopup(cKeyboardPopup* self, int flags)
{
    self->mVtbl = cKeyboardPopup_vtbl;
    void* ustrStatic = self->mpustrStatic;
    if (ustrStatic != 0) {
        cMemMan_free(ustrStatic);
        self->mpustrStatic = 0;
    }
    cBXString__cBXString((char*)&self->mszStatic, 2);
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_Fullkeyboard[];
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
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_Fullkeyboard), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onUpdate);
#ifdef SKIP_ASM

extern "C" void cUIState_onUpdate(void*);
extern "C" void cUIStateStack_pop(void* self, int a1);

/**
 * Updates the on-screen keyboard and closes it when the online overlay timeout runs out in a game.
 *
 * Runs the base UI-state update cUIState_onUpdate(). When cGameComm exists, its mOverlayTimeout is 0 and a game is
 * running, pops the top state off the UI state stack (cUIStateStack_pop(stack, 1)). When mbCallParentsUpdate is set,
 * also calls the parent state's onUpdate (vtable slot 12).
 */
extern "C" void cKeyboardPopup_onUpdate(cKeyboardPopup* self)
{
    cUIState_onUpdate(self);
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mOverlayTimeout == 0 && (int)SSXApp->Game != 0) {
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
    }
    if (self->mbCallParentsUpdate != 0) {
        cUIState* obj = self->mpParentState;
        if (((void*)obj) != 0) {
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)obj->mVtbl;
            vt[12].fn(vthis(obj, vt[12]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_pop(void* self, int a1);

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
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern int D_004A1A70;
extern char str_HelpGroup[];
extern char str_back_com[];
extern char str_back_exp[];
extern char str_back_reg[];


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
    if (IsK1CB2C8(*(int*)((char*)widget + 0x38), str_HelpGroup)) {
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)widget + 8);
        vt[9].fn(vthis(widget, vt[9]), 0);
        if (isOnK1CB2C8()) {
            sVEntry<void(void*, int)>* vt2 = *(sVEntry<void(void*, int)>**)((char*)widget + 8);
            vt2[9].fn(vthis(widget, vt2[9]), 1);
        }
        if ((int)SSXApp->Game != 0) {
            sVEntry<void(void*, int)>* vt3 = *(sVEntry<void(void*, int)>**)((char*)widget + 8);
            vt3[9].fn(vthis(widget, vt3[9]), 0);
        }
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), str_back_com)) {
        self->mpBackCompact = (cUIVector*)widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), str_back_exp)) {
        self->mpBackExpand = (cUIVector*)widget;
    } else if (IsK1CB2C8(*(int*)((char*)widget + 0x38), str_back_reg)) {
        self->mpBackRegular = (cUIVector*)widget;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleEvent);
#ifdef SKIP_ASM

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
        cUIState* obj = self->mpParentState;
        if (((void*)obj) != 0) {
            sVEntry<int(void*, int, int)>* vt = (sVEntry<int(void*, int, int)>*)obj->mVtbl;
            return vt[24].fn(vthis(obj, vt[24]), msg, pParam);
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
    char* buf = (char*)&self->mszEditString;
    self->mbSetInputText = one;
    strcpy(buf, str);
    self->miCursorPos = strlen(buf);
    self->miViewableStartIndex = 0;
    self->miViewableEndIndex = strlen(buf);
    if (self->mbDisableForFirst != 0) {
        cKeyboardPopup_setIncludeKey(self, 75, strlen(buf) ? one : 0, 2);
    }
    if (strlen(buf) < self->miMinCharacters) {
        cKeyboardPopup_setIncludeKey(self, 55, 0, 2);
    } else {
        cKeyboardPopup_setIncludeKey(self, 55, 1, 2);
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
extern signed char cKeyboardPopup_m_KeyValues[];

/**
 * Moves the keyboard cursor one key left, wrapping within the 14-key row and skipping blank and disabled keys.
 *
 * The case (caps XOR shift; keys 15-26 use !shift) decides which key set a refused key falls back to. With @p bControllerInput
 * set it saves miCurrentKey in miLastKey and sets meLastDirection to left (2); if the previous move was right (3)
 * it simply returns to miLastKey's old value. Otherwise column 0 wraps to 13, '*' (blank) keys of cKeyboardPopup_m_KeyValues are
 * skipped leftwards, and a key cKeyboardPopup_allowSelection refuses is passed over by moving again (with @p bControllerInput 0),
 * setting meSkipState to the other case when the key exists in it.
 *
 * @param bControllerInput  nonzero for a move from the controller (On Tour bControllerInput); 0 for the internal repeat
 */
extern "C" void cKeyboardPopup_kbMoveLeft(cKeyboardPopup* self, int bControllerInput)
{
    bool capsOn = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = capsOn;
        flag = !flag;
    } else {
        flag = capsOn != 0;
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
        self->meLastDirection = 2;
        if (st == 3) {
            self->miCurrentKey = old;
            return;
        }
    }
    int curKey = self->miCurrentKey;
    if (curKey == 0 || curKey == 14 || curKey == 28 || curKey == 42 || curKey == 56 || curKey == 70)
        self->miCurrentKey += 13;
    else
        self->miCurrentKey = curKey - 1;
    while (cKeyboardPopup_m_KeyValues[self->miCurrentKey] == 42)
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
extern signed char cKeyboardPopup_m_KeyValues[];

/**
 * Moves the keyboard cursor one key right, wrapping within the 14-key row and skipping blank and disabled keys.
 *
 * Mirrors cKeyboardPopup_kbMoveLeft (direction right, 3; column 13 wraps to 0), but the undo after a left move
 * happens only when meSkipState is 2 or 0 and the old key is selectable. The test reads (flag && state == 0) ||
 * (!flag && state == 0): both arms check 0 (Reg), so the case flag has no effect (probably one arm was meant to
 * test 1, Alt).
 *
 * @param bControllerInput  nonzero for a move from the controller (On Tour bControllerInput); 0 for the internal repeat
 */
extern "C" void cKeyboardPopup_kbMoveRight(cKeyboardPopup* self, int bControllerInput)
{
    bool capsOn = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = capsOn;
        flag = !flag;
    } else {
        flag = capsOn != 0;
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
        self->meLastDirection = 3;
        if (st == 2) {
            int skipState = self->meSkipState;
            if (skipState == 2 || (flag && skipState == 0) || (!flag && skipState == 0)) {
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
    int curKey = self->miCurrentKey;
    if (curKey == 13 || curKey == 27 || curKey == 41 || curKey == 55 || curKey == 69 || curKey == 83)
        self->miCurrentKey -= 13;
    else
        self->miCurrentKey = curKey + 1;
    while (cKeyboardPopup_m_KeyValues[self->miCurrentKey] == 42)
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
extern signed char cKeyboardPopup_m_KeyValues[];
extern "C" int cKeyboardPopup_handleMovementUpFromQuickKeyRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementUpFromNumberRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementUpFromLast(cKeyboardPopup* self, int a1);

/**
 * Moves the keyboard cursor one row up, with special targets from the bottom, number and quick-key rows.
 *
 * The undo after a down move (meLastDirection 1) has the same always-0 skip-state test as
 * cKeyboardPopup_kbMoveRight. Keys 70-83, 14-27 and 0-13 first try cKeyboardPopup_handleMovementUpFromLast,
 * ..FromNumberRow and ..FromQuickKeyRow; otherwise the cursor moves up 14 keys (row 0 wraps to row 5, +70), skips
 * '*' keys leftwards and repeats when the key is refused. With @p bControllerInput set it records miLastKey and direction up (0).
 *
 * @param bControllerInput  nonzero for a move from the controller (On Tour bControllerInput); 0 for the internal repeat
 */
extern "C" void cKeyboardPopup_kbMoveUp(cKeyboardPopup* self, int bControllerInput)
{
    bool capsOn = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = capsOn;
        flag = !flag;
    } else {
        flag = capsOn != 0;
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
        self->meLastDirection = 0;
        if (st == 1) {
            int skipState = self->meSkipState;
            if (skipState == 2 || (flag && skipState == 0) || (!flag && skipState == 0)) {
                if (cKeyboardPopup_allowSelection_out(self, old, 0)) {
                    self->miCurrentKey = old;
                    return;
                }
            }
        }
    }
    unsigned int curKey = (unsigned int)self->miCurrentKey;
    if (curKey >= 70 && curKey <= 83) {
        if (cKeyboardPopup_handleMovementUpFromLast(self, flag))
            return;
        curKey = (unsigned int)self->miCurrentKey;
    } else if (curKey >= 14 && curKey <= 27) {
        if (cKeyboardPopup_handleMovementUpFromNumberRow(self, flag))
            return;
        curKey = (unsigned int)self->miCurrentKey;
    } else if (curKey < 14) {
        if (cKeyboardPopup_handleMovementUpFromQuickKeyRow(self, flag))
            return;
        curKey = (unsigned int)self->miCurrentKey;
    }
    if (curKey < 14)
        self->miCurrentKey = curKey + 70;
    else
        self->miCurrentKey -= 14;
    while (cKeyboardPopup_m_KeyValues[self->miCurrentKey] == 42)
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
extern signed char cKeyboardPopup_m_KeyValues[];
extern "C" int cKeyboardPopup_handleMovementDownFromQuickKeyRow(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementDownFromLast(cKeyboardPopup* self, int a1);
extern "C" int cKeyboardPopup_handleMovementDownToLast(cKeyboardPopup* self, int a1);

/**
 * Moves the keyboard's selection one row down, skipping gaps and keys that cannot be selected.
 *
 * With controller input it first remembers the key and direction, and returns to the key it came from when
 * the previous direction was 0 (probably up), meSkipState allows it and that key is selectable. The quick-key
 * row, the shift row (56-69) and the last row (70-83) have their own handlers; other keys move 14 down (the
 * last row wraps to the top) past gaps in cKeyboardPopup_m_KeyValues. When the new key is not selectable it moves on
 * recursively and updates meSkipState.
 *
 * @param bControllerInput  nonzero for a move from the pad (not a recursive skip)
 */
extern "C" void cKeyboardPopup_kbMoveDown(cKeyboardPopup* self, int bControllerInput)
{
    bool capsOn = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = capsOn;
        flag = !flag;
    } else {
        flag = capsOn != 0;
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
            int skipState = self->meSkipState;
            if (skipState == 2 || (flag && skipState == 0) || (!flag && skipState == 0)) {
                if (cKeyboardPopup_allowSelection_out(self, old, 0)) {
                    self->miCurrentKey = old;
                    return;
                }
            }
        }
    }
    unsigned int curKey = (unsigned int)self->miCurrentKey;
    if (curKey >= 56 && curKey <= 69) {
        if (cKeyboardPopup_handleMovementDownToLast(self, flag))
            return;
    } else if (curKey >= 70 && curKey <= 83) {
        if (cKeyboardPopup_handleMovementDownFromLast(self, flag))
            return;
    } else if (curKey < 14) {
        if (cKeyboardPopup_handleMovementDownFromQuickKeyRow(self, flag))
            return;
    }
    unsigned int updatedKey = (unsigned int)self->miCurrentKey;
    if (updatedKey >= 70 && updatedKey <= 83)
        self->miCurrentKey = updatedKey - 70;
    else
        self->miCurrentKey = updatedKey + 14;
    while (cKeyboardPopup_m_KeyValues[self->miCurrentKey] == 42)
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

/**
 * Moves up from the quick-key row (keys 0-13) onto the bottom row's wide keys.
 *
 * Keys 5-8 go to key 0x4B (space), 9-10 to 0x4C and 11-12 to 0x4F when selectable; else meSkipState becomes the
 * other case (if the key exists there) and cKeyboardPopup_skipRowToBottom(-1) picks the lowest selectable key of
 * the column.
 *
 * @param bUseReg  the case flag of the caller (On Tour bUseReg): the skip state becomes Alt when it is 0
 * @return 1 when the key was one of 5-12 and the cursor moved, otherwise 0
 */
extern "C" int cKeyboardPopup_handleMovementUpFromQuickKeyRow(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int state = self->miCurrentKey;
    if (state >= 5 && state <= 8) {
        if (cKeyboardPopup_allowSelection_out(self, 75, &val)) {
            self->miCurrentKey = 75;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 9 && state <= 10) {
        if (cKeyboardPopup_allowSelection_out(self, 76, &val)) {
            self->miCurrentKey = 76;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 11 && state <= 12) {
        if (cKeyboardPopup_allowSelection_out(self, 79, &val)) {
            self->miCurrentKey = 79;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
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
        if (cKeyboardPopup_allowSelection_out(self, 18, &val)) {
            self->miCurrentKey = 18;
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
    int otherCaseAllowed;
    int state = self->miCurrentKey;
    if (state == 76) {
        if (cKeyboardPopup_allowSelection_out(self, 0, &val)) {
            self->miCurrentKey = 9;
        } else if (cKeyboardPopup_allowSelection_out(self, 23, &val)) {
            self->miCurrentKey = 23;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state == 75) {
        if (cKeyboardPopup_allowSelection_out(self, 19, &otherCaseAllowed)) {
            self->miCurrentKey = 19;
        } else {
            if (otherCaseAllowed) {
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

/**
 * Moves up from the number row (keys 14-27) to the quick keys or onto the bottom row's wide keys.
 *
 * When key 0 is selectable (a quick-key row is shown) keys 16-17 go to 4 and 18 to 5. Otherwise 15-22 go to 0x4B
 * (space), 23 to 0x4C, 26 to 0x53 and 24-25 to 0x4F when selectable, else the skip state is updated and
 * cKeyboardPopup_skipRowToBottom picks the column's lowest selectable key (from key 0x3E for key 0x14).
 *
 * @param bUseReg  the case flag of the caller (On Tour bUseReg): the skip state becomes Alt when it is 0
 * @return 1 when the cursor moved, otherwise 0
 */
extern "C" int cKeyboardPopup_handleMovementUpFromNumberRow(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int allowed = cKeyboardPopup_allowSelection_out(self, 0, 0);
    int state = self->miCurrentKey;
    if (allowed) {
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
        if (cKeyboardPopup_allowSelection_out(self, 75, &val)) {
            self->miCurrentKey = 75;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            if (self->miCurrentKey == 20) {
                cKeyboardPopup_skipRowToBottom(self, 0x3E);
            } else {
                cKeyboardPopup_skipRowToBottom(self, -1);
            }
        }
        return 1;
    } else if (state == 23) {
        if (cKeyboardPopup_allowSelection_out(self, 76, &val)) {
            self->miCurrentKey = 76;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state == 26) {
        if (cKeyboardPopup_allowSelection_out(self, 83, &val)) {
            self->miCurrentKey = 83;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToBottom(self, -1);
        }
        return 1;
    } else if (state >= 24 && state <= 25) {
        if (cKeyboardPopup_allowSelection_out(self, 79, &val)) {
            self->miCurrentKey = 79;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
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

/**
 * Moves up from the bottom row's keys 0x4C and 0x4F to key 0x42, or up the rows to the first selectable key.
 *
 * @param bUseReg  the case flag of the caller (On Tour bUseReg): the skip state becomes Alt when it is 0
 * @return 1 when the cursor was on 0x4C or 0x4F (and moved), otherwise 0
 */
extern "C" int cKeyboardPopup_handleMovementUpFromLast(cKeyboardPopup* self, int bUseReg)
{
    int val;
    int state = self->miCurrentKey;
    if (state == 76) {
        if (cKeyboardPopup_allowSelection_out(self, 66, &val)) {
            self->miCurrentKey = 66;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_moveUpRows(self);
        }
        return 1;
    } else if (state == 79) {
        if (cKeyboardPopup_allowSelection_out(self, 66, &val)) {
            self->miCurrentKey = 66;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
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
    if (state == 57) {
        if (cKeyboardPopup_allowSelection_out(self, 75, &val)) {
            self->miCurrentKey = 75;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state >= 58 && state <= 65) {
        if (cKeyboardPopup_allowSelection_out(self, 75, &val)) {
            self->miCurrentKey = 75;
        } else {
            if (val) {
                self->meSkipState = bUseReg == 0;
            }
            cKeyboardPopup_skipRowToTop(self);
        }
        return 1;
    } else if (state == 66) {
        if (cKeyboardPopup_allowSelection_out(self, 76, &val)) {
            self->miCurrentKey = 76;
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

/**
 * Puts the cursor on the lowest selectable key of a column, searching upwards one 14-key row at a time.
 *
 * @param start  the key to start from (On Tour iStartingLook); -1 starts four rows below the cursor (+0x38)
 */
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

/**
 * Moves the cursor up 14 keys at a time until cKeyboardPopup_allowSelection accepts the key.
 */
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
extern "C" void memset(void* dst, int c, int n);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self);
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int key, char* out, int a3);
extern "C" void cKeyboardPopup_updateEdit(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_AddCharacterEdit(cKeyboardPopup* self);
extern "C" void cKeyboardPopup_handleQuickKeyPress(cKeyboardPopup* self, int digit);
extern "C" void cKeyboardPopup_pressKey(cKeyboardPopup* self, int a1);
// PORT: cKeyboardPopup_allowSelection's third parameter is an int* out-param; the unit declares it (void*, int, int).
int cKeyboardPopup_allowSelection_out(void* self, int idx, int* out) __asm__("cKeyboardPopup_allowSelection");
// cUIStateStack_pop returns its last list node in $v0; the unit declares it void.
void* cUIStateStack_pop_r(void* self, int a1) __asm__("cUIStateStack_pop");

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
 * quick keys 4-13 run cKeyboardPopup_handleQuickKeyPress and other keys add their character (cKeyboardPopup_AddCharacterEdit). Shift is released
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
        if (key != 55 && key != 70) {
            self->pendingAccept = 0;
            self->key = 70;
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
            memset(self->text, 0, 0x40);
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
                cUIStateStack_pop_r(self->owner + 0x18, 1);
            self->done = 1;
            self->parent->vt[19].fn(vthis(self->parent, self->parent->vt[19]), 0, 13);
            break;
        case 75:
            if (cKeyboardPopup_allowSelection_out(self, 75, 0) != 0)
                cKeyboardPopup_AddCharacterEdit((cKeyboardPopup*)self);
            break;
        case 0:
            cUIStateStack_pop_r(self->owner + 0x18, 1);
            self->parent->vt[19].fn(vthis(self->parent, self->parent->vt[19]), 0, 0xE);
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
            cKeyboardPopup_handleQuickKeyPress((cKeyboardPopup*)self, self->key - 4);
            break;
        default:
            cKeyboardPopup_AddCharacterEdit((cKeyboardPopup*)self);
            break;
        }
    }
    if (self->key != 0x38 && self->shift != 0) {
        self->shift ^= 1;
        cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
    }
    cKeyboardPopup_updateEdit((cKeyboardPopup*)self, moved);
    if (self->needText != 0)
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 75, strlen(self->text) != 0, 2);
    if (strlen(self->text) < self->minLen)
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 55, 0, 2);
    else
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, 55, 1, 2);
    cKeyboardPopup_redrawScreen((cKeyboardPopup*)self);
    if (redo) {
        char name[32];
        cKeyboardPopup_getObjName((cKeyboardPopup*)self, 70, name, 1);
        sUIObj_C1E0* keyObj = (sUIObj_C1E0*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (keyObj != 0) {
            // PORT: takes the address of a temporary (accepted by g++ 2.95); use a named local elsewhere.
            keyObj->vt[11].fn(vthis(keyObj, keyObj->vt[11]), &sColor_C1E0(0.3f, 0, 0, 0));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_AddAsciiEdit);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

/**
 * Types one character into the keyboard's edit string at the cursor.
 *
 * Overwrites mszEditString[miCursorPos]. At the end of the string, the string grows: on the last allowed position
 * (miMaxCharacters - 1) it is cut at miMaxCharacters and miViewableEndIndex becomes cursor + 1; otherwise the cursor
 * advances, the string is terminated after it and miViewableEndIndex grows by 1. Inside the string the character is
 * overwritten and the cursor advances (not past miMaxCharacters - 1).
 *
 * @param value  the character (On Tour char c)
 */
extern "C" void cKeyboardPopup_AddAsciiEdit(cKeyboardPopup* self, int value)
{
    char asciiChar = value;
    char* buf = (char*)&self->mszEditString;
    int len = strlen(buf);
    int cur = self->miCursorPos;
    buf[cur] = asciiChar;
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
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_updateToAscii);
#ifdef SKIP_ASM
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);
extern signed char cKeyboardPopup_m_KeyValues[];
extern signed char cKeyboardPopup_m_AltValues[];

/**
 * Moves the on-screen keyboard's cursor to the key that types a given character.
 *
 * '`' selects key 0xE. Otherwise the first of the 0x54 keys whose upper (cKeyboardPopup_m_KeyValues) or lower (cKeyboardPopup_m_AltValues) character
 * matches is taken, skipping '*' keys and keys that are '@' in both tables; it becomes miCurrentKey when
 * cKeyboardPopup_allowSelection allows it.
 *
 * @param value  the character (On Tour char c)
 * @return 1 when a key was selected, else 0
 */
extern "C" int cKeyboardPopup_updateToAscii(cKeyboardPopup* self, int value)
{
    char asciiChar = value;
    int i;
    if (asciiChar == 0x60) {
        self->miCurrentKey = 0xE;
        return 1;
    }
    for (i = 0; i < 84; i++) {
        if ((cKeyboardPopup_m_KeyValues[i] == asciiChar || cKeyboardPopup_m_AltValues[i] == asciiChar) && cKeyboardPopup_m_KeyValues[i] != '*'
            && (cKeyboardPopup_m_KeyValues[i] != '@' || cKeyboardPopup_m_AltValues[i] != cKeyboardPopup_m_KeyValues[i])) {
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
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_AddCharacterEdit);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_AddAsciiEdit(cKeyboardPopup* self, int value);
extern signed char cKeyboardPopup_m_KeyValues[];
extern signed char cKeyboardPopup_m_AltValues[];

/**
 * Types the current key's character into the edit string (cKeyboardPopup_AddAsciiEdit).
 *
 * Uses the first table (cKeyboardPopup_m_KeyValues: digits, upper case) when shift and caps lock differ, else the second (cKeyboardPopup_m_AltValues:
 * symbols, lower case); keys 15-26 (the number row) follow shift alone.
 */
extern "C" void cKeyboardPopup_AddCharacterEdit(cKeyboardPopup* self)
{
    bool capsOn = self->mbCapsOn != 0;
    int flag = self->mbShiftOn;
    if (flag) {
        flag = !capsOn;
    } else {
        flag = capsOn != 0;
    }
    int idx = self->miCurrentKey;
    if (idx >= 15 && idx <= 26) {
        // PORT: reads the int field at 0x6C as a bool (bool is 4 bytes in SN gcc 2.95); use `*(int*)... == 0` off-PS2.
        flag = !*(bool*)&self->mbShiftOn;
    }
    if (flag) {
        cKeyboardPopup_AddAsciiEdit(self, cKeyboardPopup_m_KeyValues[idx]);
    } else {
        cKeyboardPopup_AddAsciiEdit(self, cKeyboardPopup_m_AltValues[idx]);
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
extern char str_EnterValue[];

/**
 * Refreshes the keyboard's edit field: the visible part of the text, stars for passwords, and the cursor.
 *
 * @param bDropFromRight  passed to cKeyboardPopup_updateViewable(): trim the visible part from the right
 */
extern "C" void cKeyboardPopup_updateEdit(cKeyboardPopup* self, int bDropFromRight)
{
    cKeyboardPopup_updateViewable(self, bDropFromRight);
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterValue));
    if (text != 0) {
        if (self->mbPassword != 0) {
            cKeyboardPopup_setPasswordText((sPwd_CCE98*)self, (char*)&self->mszViewableString);
            cUIText_setAsciiString(text, (char*)&self->mszPasswordString);
        } else {
            cUIText_setAsciiString(text, (char*)&self->mszViewableString);
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
extern char str_EnterValue[];  // "EnterValue"
extern char str_ExtendCursorLeft[];  // "ExtendCursorLeft"
extern char str_ExtendCursorRight[];  // "ExtendCursorRight"

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
        void* pText = cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(str_EnterValue));
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
    pCursor = (cUIThing_CC8D8*)cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(str_ExtendCursorLeft));
    pCursor->setVisible((self)->miViewableStartIndex != 0);
    pCursor = (cUIThing_CC8D8*)cUIScreen_getObjectByHashName(((void*)(self)->mpScreen), GetHashValue32(str_ExtendCursorRight));
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
extern char str_CharacterCursor[];

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

    void* text = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterValue));
    char* uiObj = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterValue));
    if (uiObj != 0) {
        sVEntry_CC30* vt = *(sVEntry_CC30**)(uiObj + 8);
        vt[20].fn(vthis(uiObj, vt[20]), (char*)&self->mvSizeEnter[0]);
        *(sVec3_CC30*)&self->mvPosEnter[0] = *(sVec3_CC30*)(uiObj + 0x44);
    }
    self->mvPosCursor[1] = self->mvSizeEnter[1] + self->mvPosEnter[1] - self->mvSizeCursor[1] - 2.0f;
    unsigned char cursorChar = *(((char*)self) + self->miCursorPos + 0x74);
    char tc = *(((char*)self) + self->miCursorPos + 0x74);
    pos = *(sVec3_CC30*)&self->mvSizeCursor[0];
    if (tc != 0) {
        sRect_CC30 size;
        char ch[2];
        ch[1] = 0;
        if (self->mbPassword != 0) {
            ch[0] = '*';
        } else {
            ch[0] = cursorChar;
        }
        cKeyboardPopup_setScale(self, text);
        char* font = (char*)cUIText_getFont(text);
        size.Measure(font, ch, ch[0] == ' ');
        pos.x = size.w;
    }
    uiObj = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_CharacterCursor));
    sVEntry_CC30* vt = *(sVEntry_CC30**)(uiObj + 8);
    vt[21].fn(vthis(uiObj, vt[21]), &pos);
    char buf[0x40];
    if (self->mbPassword != 0) {
        cKeyboardPopup_setPasswordText((sPwd_CCE98*)self, self->mszViewableString);
        strncpy(buf, self->mszPasswordString, self->miCursorPos - self->miViewableStartIndex);
    } else {
        strncpy(buf, self->mszViewableString, self->miCursorPos - self->miViewableStartIndex);
    }
    buf[self->miCursorPos - self->miViewableStartIndex] = 0;
    cKeyboardPopup_setScale(self, text);
    char* font = (char*)cUIText_getFont(text);
    sRect_CC30 size;
    size.Measure(font, buf, 1);
    sVec3_CC30 cursorPos = *(sVec3_CC30*)&self->mvPosCursor[0];
    cursorPos.x = size.w + self->mvPosEnter[0];
    *(sVec3_CC30*)(uiObj + 0x44) = cursorPos;
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
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setStatic2);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_StaticValue[];
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBXString_operatorE1(void* self, const char* str);

/**
 * Sets the keyboard popup's static label from an ascii string.
 *
 * Before the screen exists (mpScreen null) the string is kept in mszStatic (cBXString at +0x424); otherwise it is put
 * straight into the "StaticValue" text widget.
 *
 * @param str  the label text
 */
extern "C" void cKeyboardPopup_setStatic2(cKeyboardPopup* self, const char* str)
{
    if (self->mpScreen == 0) {
        cBXString_operatorE1((char*)&self->mszStatic, str);
        return;
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_StaticValue));
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
extern char str_StaticValue[];
extern char str_cKeyboardPopup_setStatic[];

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
        void* oldStatic = self->mpustrStatic;
        if (oldStatic != 0) {
            cMemMan_free(oldStatic);
            self->mpustrStatic = 0;
        }
        unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_cKeyboardPopup_setStatic, 0x100, 0);
        self->mpustrStatic = copy;
        USTR_copy(copy, str);
    } else {
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_StaticValue));
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
extern char str_StaticValue[];

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
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_StaticValue));
        if (text != 0) {
            cUIText_setUnicodeStringByID(text, id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setMaxCharacters);
#ifdef SKIP_ASM
/**
 * Sets the keyboard's text length limit, clamped to 0..63.
 *
 * @param iMaxCharacters  the requested limit (On Tour iMaxCharacters)
 */
extern "C" void cKeyboardPopup_setMaxCharacters(cKeyboardPopup* self, int iMaxCharacters)
{
    if (iMaxCharacters < 0) {
        self->miMaxCharacters = 0;
        return;
    }
    if (iMaxCharacters >= 0x40) {
        self->miMaxCharacters = 0x3f;
        return;
    }
    self->miMaxCharacters = iMaxCharacters;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setMinCharacters);
#ifdef SKIP_ASM
/**
 * Sets miMinCharacters, clamped to 0-63.
 *
 * @param iMinCharacters  the minimum length the entry must have
 */
extern "C" void cKeyboardPopup_setMinCharacters(cKeyboardPopup* self, int iMinCharacters)
{
    if (iMinCharacters < 0) {
        self->miMinCharacters = 0;
        return;
    }
    if (iMinCharacters >= 0x40) {
        self->miMinCharacters = 0x3f;
        return;
    }
    self->miMinCharacters = iMinCharacters;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_getObjName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sName8_1CD0E8 { char s[8]; };
struct sName10_1CD0E8 { char s[10]; };
extern char str_cursor[];
extern sName10_1CD0E8 D_0045D550_1CD0E8 __asm__("D_0045D550");
extern char str_back[];
extern char str_caps[];
extern char str_Lshift[];
extern char str_clear[];
extern char str_arrLeft[];
extern char str_arrRight[];
extern char str_tab[];
extern char str_enter[];
extern char str_arrUp[];
extern char str_arrDown[];
extern char str_space[];
extern char str_ESC[];
extern char str_Qpct_d_b[];

/**
 * Builds the UI object name of a keyboard key or of its cursor highlight.
 *
 * The name is "keyboard_" or "cursor_" plus the key's name: "back", "caps", "Lshift", "clear", "arrLeft",
 * "arrRight", "tab", "enter", "arrUp", "arrDown", "space" and "ESC" for the special keys, "Q1" to "Q10" for
 * keys 4-13, otherwise the key's character from cKeyboardPopup_m_KeyValues.
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
        *(sName8_1CD0E8*)out = *(sName8_1CD0E8*)str_cursor;
    } else {
        *(sName10_1CD0E8*)out = D_0045D550_1CD0E8;
    }
    switch (key) {
    case 27: strcat(out, str_back); break;
    case 42: strcat(out, str_caps); break;
    case 56: strcat(out, str_Lshift); break;
    case 70: strcat(out, str_clear); break;
    case 76: strcat(out, str_arrLeft); break;
    case 83: strcat(out, str_arrRight); break;
    case 28: strcat(out, str_tab); break;
    case 55: strcat(out, str_enter); break;
    case 69: strcat(out, str_arrUp); break;
    case 79: strcat(out, str_arrDown); break;
    case 75: strcat(out, str_space); break;
    case 0: strcat(out, str_ESC); break;
    case 4: case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12: case 13:
        sprintf(num, str_Qpct_d_b, key - 3);
        strcat(out, num);
        break;
    default:
        if (cKeyboardPopup_m_KeyValues[key] == '*') {
            return 0;
        }
        ch[0] = cKeyboardPopup_m_KeyValues[key];
        ch[1] = 0;
        strcat(out, ch);
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
extern "C" void cKeyboardPopup_setStatic2(cKeyboardPopup* self, const char* str);
extern "C" void cKeyboardPopup_setStatic1(cKeyboardPopup* self, int a1);
extern "C" void cKeyboardPopup_updateCursor(cKeyboardPopup* self);
extern "C" void cBXString_Reset(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cMemMan_free(void* ptr);
extern void* Keyboard;
extern char str_regular[];
extern char str_compact[];
extern char str_expanded[];
extern char str_CharacterCursor[];
extern char str_EnterValue[];
extern char str_Name_bar[];
extern char str_ExtendCursorLeft[];
extern char str_ExtendCursorRight[];
extern char D_004A1408[];

struct sVec3_D348 { float x, y, z; };

struct sVEi_D348 {
    short delta;
    short index;
    void (*fn)(void*, int);
};



static inline void SetVis_D348(char* o, int on)
{
    sVEi_D348* vt = *(sVEi_D348**)(o + 8);
    vt[9].fn(vthis(o, vt[9]), on);
}

/**
 * Prepares the keyboard the first time it is shown: layout, measurements and the initial texts.
 *
 * Sets mbTransistionDone, turns caps on (or takes bit 1 of Keyboard + 0x88 when that exists) and shift off,
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
        if (Keyboard != 0) {
            (self)->mbCapsOn = (*(unsigned int*)((char*)Keyboard + 0x88) >> 1) & 1;
        }
        self->mbShiftOn = 0;
        if (self->mbSetInputText == 0) {
            ((char*)self)[0x74] = 0;
        }
        self->miViewableStartIndex = 0;
        self->miViewableEndIndex = 0;
        self->miCursorPos = 0;
        int showCompact = 0;
        int showRegular = 0;
        int showExpand = 0;
        cKeyboardPopup_redrawScreen(self);
        switch (self->meType) {
        case 0:
            showRegular = 1;
            sprintf(buf, str_regular);
            break;
        case 2:
            showExpand = 1;
            sprintf(buf, str_expanded);
            break;
        case 1:
        default:
            showCompact = 1;
            sprintf(buf, str_compact);
            break;
        }
        char* oa = (char*)self->mpBackCompact;
        if (oa != 0) {
            SetVis_D348(oa, showCompact);
        }
        char* ob = (char*)self->mpBackExpand;
        if (ob != 0) {
            SetVis_D348(ob, showExpand);
        }
        char* oc = (char*)self->mpBackRegular;
        if (oc != 0) {
            SetVis_D348(oc, showRegular);
        }
        int frameNum = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(buf));
        if (frameNum != 0xFFFF) {
            cUIScreen_playFrame(self->mpScreen, frameNum, 1);
        }
        char* charCursor = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_CharacterCursor));
        if (charCursor != 0) {
            sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(charCursor + 8);
            vt[20].fn(vthis(charCursor, vt[20]), (char*)&self->mvSizeCursor[0]);
            *(sVec3_D348*)&self->mvPosCursor[0] = *(sVec3_D348*)(charCursor + 0x44);
        }
        char* enterValue = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_EnterValue));
        if (enterValue != 0) {
            sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(enterValue + 8);
            vt[20].fn(vthis(enterValue, vt[20]), (char*)&self->mvSizeEnter[0]);
            *(sVec3_D348*)&self->mvPosEnter[0] = *(sVec3_D348*)(enterValue + 0x44);
            self->mvPosCursor[1] = self->mvSizeEnter[1] + self->mvPosEnter[1];
        }
        char* nameBar = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Name_bar));
        if (nameBar != 0) {
            sVec3_D348 nameBarPos = *(sVec3_D348*)(nameBar + 0x44);
            self->mvPosEnter[0] += nameBarPos.x;
            self->mvPosEnter[1] += nameBarPos.y;
            self->mvPosEnter[2] += nameBarPos.z;
        }
        char* extendCursorLeft = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ExtendCursorLeft));
        if (extendCursorLeft != 0) {
            SetVis_D348(extendCursorLeft, 0);
        }
        char* extendCursorRight = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ExtendCursorRight));
        if (extendCursorRight != 0) {
            SetVis_D348(extendCursorRight, 0);
        }
        if (self->mbSetInputText != 0) {
            cKeyboardPopup_setInputText(self, self->mszEditString);
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
            char** str = (char**)(&self->mszStatic);
            if (*(int*)(*str - 8) != 0) {
                cKeyboardPopup_setStatic2((cKeyboardPopup*)(((char*)self)), *str);
                cBXString_Reset(str);
            } else {
                cKeyboardPopup_setStatic2((cKeyboardPopup*)(((char*)self)), D_004A1408);
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
extern "C" int cKeyboardPopup_getObjName(cKeyboardPopup* self, int i, char* out, int mode);
extern "C" void cKeyboardPopup_SetNameForKey(cKeyboardPopup* self, cUIText* text, int i);
extern "C" void cKeyboardPopup_checkAndFixConflict(cKeyboardPopup* self);
extern signed char cKeyboardPopup_m_KeyValues[];
extern signed char cKeyboardPopup_m_AltValues[];
extern char gtStaticColor[];
extern char gtNotStatic[];
extern char gtSelectedKey[];
extern char gtNonSelectedKey[];

/**
 * Redraws every key of the keyboard: labels, availability colours, cursor objects and the highlight.
 *
 * Special keys get their labels from cKeyboardPopup_SetNameForKey(); character keys show the upper or lower
 * character (caps xor shift; keys 15-26 follow shift alone) and are coloured by
 * cKeyboardPopup_allowSelection(). Cursor objects of keys 0-13 are hidden in the compact layout (meType 1).
 * The current key's cursor gets the highlight colour gtSelectedKey. Does nothing without a screen.
 */
extern "C" void cKeyboardPopup_redrawScreen(cKeyboardPopup* self)
{
    char name[0x20];
    char ch[2];
    if (self->mpScreen == 0) {
        return;
    }
    ch[1] = 0;
    for (int i = 0; i < 84; i++) {
        if (cKeyboardPopup_getObjName(self, i, name, 0) == 0) {
            continue;
        }
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
        if (cKeyboardPopup_m_KeyValues[i] == '@' || cKeyboardPopup_m_KeyValues[i] == ' ') {
            cKeyboardPopup_SetNameForKey(self, text, i);
        } else {
            int caps = self->mbCapsOn != 0;
            bool upper;
            if (Shift_D708(((char*)self))) {
                upper = Neg_D708(caps);
            } else {
                upper = caps;
            }
            if (i >= 0xF && i < 27) {
                upper = !Shift_D708(((char*)self));
            }
            cKeyboardPopup_checkAndFixConflict(self);
            if (upper) {
                ch[0] = cKeyboardPopup_m_KeyValues[i];
            } else {
                ch[0] = cKeyboardPopup_m_AltValues[i];
            }
            if (text != 0) {
                cUIText_setAsciiString(text, ch);
                if (cKeyboardPopup_allowSelection(self, i, 0) == 0) {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)((char*)text + 8);
                    vt[11].fn(vthis(text, vt[11]), gtStaticColor);
                } else {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)((char*)text + 8);
                    vt[11].fn(vthis(text, vt[11]), gtNotStatic);
                }
            }
        }
        cKeyboardPopup_getObjName(self, i, name, 1);
        char* uiObj = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
        if (uiObj != 0) {
            if (self->meType == 1) {
                if (i >= 0 && i < 0xE) {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(uiObj + 8);
                    vt[9].fn(vthis(uiObj, vt[9]), (void*)0);
                } else {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(uiObj + 8);
                    vt[9].fn(vthis(uiObj, vt[9]), (void*)1);
                }
            } else {
                {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(uiObj + 8);
                    vt[11].fn(vthis(uiObj, vt[11]), gtNonSelectedKey);
                }
                {
                    sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(uiObj + 8);
                    vt[9].fn(vthis(uiObj, vt[9]), (void*)1);
                }
            }
        }
    }
    cKeyboardPopup_getObjName(self, self->miCurrentKey, name, 1);
    char* uiObj = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
    if (uiObj != 0) {
        sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(uiObj + 8);
        vt[11].fn(vthis(uiObj, vt[11]), gtSelectedKey);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_SetNameForKey);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup* self, int key, int a2);
extern char gtStaticColor[];
extern char gtNotStatic[];
extern char str_Qpct_d_b[];
extern char str_kT_POPUPKeyboardBackspace[];
extern char str_kT_POPUPKeyboardCapsLock[];
extern char str_kT_POPUPKeyboardShift[];
extern char str_kT_POPUPKeyboardClear[];
extern char str_kT_POPUPKeyboardTab[];
extern char str_kT_POPUPKeyboardEnter[];
extern char str_kT_POPUPKeyboardSpace[];
extern char str_kT_POPUPKeyboardESC[];

struct sTextVE_1CD9B8 {
    short delta;
    short index;
    void* fn;
};

static inline void SetColor_1CD9B8(cUIText* text, void* col)
{
    sTextVE_1CD9B8* vt = *(sTextVE_1CD9B8**)((char*)text + 8);
    ((int (*)(void*, void*))vt[11].fn)(vthis(text, vt[11]), col);
}

static inline void SetVisible_1CD9B8(cUIText* text, int on)
{
    sTextVE_1CD9B8* vt = *(sTextVE_1CD9B8**)((char*)text + 8);
    ((void (*)(void*, int))vt[9].fn)(vthis(text, vt[9]), on);
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
        SetColor_1CD9B8(text, gtStaticColor);
    } else {
        SetColor_1CD9B8(text, gtNotStatic);
    }
    switch (key) {
    case 27:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardBackspace));
        break;
    case 42:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardCapsLock));
        SetColor_1CD9B8(text, gtNotStatic);
        break;
    case 56:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardShift));
        SetColor_1CD9B8(text, gtNotStatic);
        break;
    case 70:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardClear));
        break;
    case 28:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardTab));
        break;
    case 55:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardEnter));
        break;
    case 75:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardSpace));
        break;
    case 0:
        if (vself->meType == 1) {
            SetVisible_1CD9B8(text, 0);
        } else {
            SetVisible_1CD9B8(text, 1);
        }
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUPKeyboardESC));
        break;
    case 4: case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12: case 13:
        if (vself->meType == 1) {
            SetVisible_1CD9B8(text, 0);
        } else {
            SetVisible_1CD9B8(text, 1);
        }
        sprintf(buf, str_Qpct_d_b, key - 3);
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
extern "C" void cKeyboardPopup_AddAsciiEdit(cKeyboardPopup* self, int value);
extern "C" bool cKeyboardPopup_updateToAscii_b(void* self, int value) __asm__("cKeyboardPopup_updateToAscii");
extern "C" int cKeyboardPopup_allowSelection(cKeyboardPopup*, int, int);
extern "C" void* cFEStateNetHelpPopup_cFEStateNetHelpPopup(void* mem, void* engine, void* owner);
void cFEStateNetHelpPopup_setOption(void* self, int i, int a, int b);
void cFEStateNetHelpPopup_setNumHelpOptions(void* self, int val);
void cFEStateNetHelpPopup_setDescription(void* self, int i, int value);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern int D_004A1A70;
extern char str_cFEStateNetHelpPopup[];
extern char str_kT_HELPKeyboard[];
extern char str_kT_HELPKeyboardMessage[];
extern char str_kT_HELPKeyboardNavigateKeyboard[];
extern char str_kT_HELPKeyboardCapsLock[];
extern char str_kT_HELPKeyboardShift[];
extern char str_kT_HELPKeyboardSelectCharacter[];
extern char str_kT_HELPKeyboardCancel[];

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
    sVEntry<void>* vt;
};
extern sSndK1CDB98 gUISoundNull;

typedef int (*FnIK1CDB98)(void*);

static inline int InputK1CDB98(char* ev, int slot)
{
    sVEntry<void>* vt = *(sVEntry<void>**)(ev + 8);
    return ((FnIK1CDB98)vt[slot].fn)(vthis(ev, vt[slot]));
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
extern "C" int cKeyboardPopup_onInputBegin(cKeyboardPopup* self, cUIInput* ev)
{
    if (InputK1CDB98(((char*)ev), 8) != 0)
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
        if (InputK1CDB98(((char*)ev), 19)) {
            changed = 1;
            cKeyboardPopup_kbMoveLeft(self, 1);
        } else if (InputK1CDB98(((char*)ev), 20)) {
            changed = 1;
            cKeyboardPopup_kbMoveRight(self, 1);
        } else if (InputK1CDB98(((char*)ev), 17)) {
            changed = 1;
            cKeyboardPopup_kbMoveUp(self, 1);
        } else if (InputK1CDB98(((char*)ev), 18)) {
            changed = 1;
            cKeyboardPopup_kbMoveDown(self, 1);
        } else if (InputK1CDB98(((char*)ev), 5)) {
            changed = 1;
            cKeyboardPopup_pressKey(self, 0);
        } else if (InputK1CDB98(((char*)ev), 7)) {
            if (isOnK1CDB98()) {
                if ((int)SSXApp->Game == 0) {
                    changed = 1;
                    void* popup = cFEStateNetHelpPopup_cFEStateNetHelpPopup(cMemMan_alloc(320, str_cFEStateNetHelpPopup, 0x100, 0), self->mpEngine, self);
                    cFEStateNetHelpPopup_setNumHelpOptions(popup, 6);
                    *(int*)((char*)popup + 0x54) = GetHashValue32(str_kT_HELPKeyboard);
                    cFEStateNetHelpPopup_setDescription(popup, 0, GetHashValue32(str_kT_HELPKeyboardMessage));
                    cFEStateNetHelpPopup_setOption(popup, 0, GetHashValue32(str_kT_HELPKeyboardNavigateKeyboard), 4);
                    cFEStateNetHelpPopup_setOption(popup, 1, GetHashValue32(str_kT_HELPKeyboardNavigateKeyboard), 3);
                    cFEStateNetHelpPopup_setOption(popup, 2, GetHashValue32(str_kT_HELPKeyboardCapsLock), 5);
                    cFEStateNetHelpPopup_setOption(popup, 3, GetHashValue32(str_kT_HELPKeyboardShift), 6);
                    cFEStateNetHelpPopup_setOption(popup, 4, GetHashValue32(str_kT_HELPKeyboardSelectCharacter), 0);
                    cFEStateNetHelpPopup_setOption(popup, 5, GetHashValue32(str_kT_HELPKeyboardCancel), 1);
                    cUIStateStack_pushPopup(&self->mpEngine->mStateStack, popup);
                }
                sound = 5;
            }
        } else if (InputK1CDB98(((char*)ev), 6)) {
            changed = 1;
            sound = 6;
            cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
            char* o = (char*)self->mpParentState;
            sVEntry<void>* vt = *(sVEntry<void>**)(o + 8);
            ((void (*)(void*, int, int))vt[19].fn)(vthis(o, vt[19]), 0, 0xE);
        } else if (InputK1CDB98(((char*)ev), 25)) {
            changed = 1;
            self->mbCapsOn ^= 1;
            cKeyboardPopup_redrawScreen(self);
        } else if (InputK1CDB98(((char*)ev), 27)) {
            changed = 1;
            self->mbShiftOn ^= 1;
            cKeyboardPopup_redrawScreen(self);
        } else {
            int fail = 0;
            if (InputK1CDB98(((char*)ev), 41)) {
                int typed = 0;
                switch (InputK1CDB98(((char*)ev), 42)) {
                case 8:
                    if (cKeyboardPopup_allowSelection(self, 27, 0)) self->miCurrentKey = 27;
                    else fail = 1;
                    break;
                case 13:
                    self->miCurrentKey = 55;
                    break;
                case 0x10:
                    if (cKeyboardPopup_allowSelection(self, 0x38, 0)) self->miCurrentKey = 0x38;
                    else fail = 1;
                    break;
                case 20:
                    if (cKeyboardPopup_allowSelection(self, 42, 0)) self->miCurrentKey = 42;
                    else fail = 1;
                    break;
                case 27:
                    if (cKeyboardPopup_allowSelection(self, 0, 0)) self->miCurrentKey = 0;
                    else fail = 1;
                    break;
                case 37:
                    if (cKeyboardPopup_allowSelection(self, 76, 0)) self->miCurrentKey = 76;
                    else fail = 1;
                    break;
                case 39:
                    if (cKeyboardPopup_allowSelection(self, 83, 0)) self->miCurrentKey = 83;
                    else fail = 1;
                    break;
                case 38:
                    if (cKeyboardPopup_allowSelection(self, 69, 0)) self->miCurrentKey = 69;
                    else fail = 1;
                    break;
                case 40:
                    if (cKeyboardPopup_allowSelection(self, 79, 0)) self->miCurrentKey = 79;
                    else fail = 1;
                    break;
                case 9:
                    if (cKeyboardPopup_allowSelection(self, 0x1C, 0)) self->miCurrentKey = 0x1C;
                    else fail = 1;
                    break;
                case 0x70: case 113: case 114: case 115: case 116:
                case 117: case 118: case 119: case 0x78: case 121: {
                    int key = InputK1CDB98(((char*)ev), 42) - 108;
                    if (cKeyboardPopup_allowSelection(self, key, 0)) self->miCurrentKey = key;
                    else fail = 1;
                    break;
                }
                default:
                    if (InputK1CDB98(((char*)ev), 43) >= 0x20) {
                        char asciiChar = InputK1CDB98(((char*)ev), 43);
                        fail = !cKeyboardPopup_updateToAscii_b(self, asciiChar);
                        if (!fail) {
                            if (cKeyboardPopup_allowSelection(self, self->miCurrentKey, 0)) {
                                if (asciiChar == 0x60) cKeyboardPopup_AddAsciiEdit(self, 0x7E);
                                else cKeyboardPopup_AddAsciiEdit(self, asciiChar);
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
        sSndK1CDB98* snd = *(sSndK1CDB98**)&self->mpEngine->mpSound;
        if (snd == 0)
            snd = &gUISoundNull;
        if (changed && snd != 0)
            ((void (*)(void*, int))snd->vt[2].fn)(vthis(snd, snd->vt[2]), sound);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_fillSelectableKeys);
#ifdef SKIP_ASM
extern "C" void cKeyboardPopup_setIncludeKey(cKeyboardPopup* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_setIncludeQuickKeyRow(cKeyboardPopup* self, int a1);
extern "C" int strlen(const char* s);
extern signed char cKeyboardPopup_m_KeyValues[];

/**
 * Sets which of the 84 keyboard keys can be selected in both key sets, then refreshes the key display.
 *
 * Excludes the blank keys ('*' in cKeyboardPopup_m_KeyValues) and keys 0x1C, 0x29, 0x45 and 0x4F; key 0x4B (space) is included only
 * when the edit string is not empty and mbDisableForFirst is set; every other key is included
 * (cKeyboardPopup_setIncludeKey(key, on, 2)). Ends with cKeyboardPopup_setIncludeQuickKeyRow(self, 0).
 */
extern "C" void cKeyboardPopup_fillSelectableKeys(cKeyboardPopup* self)
{
    int i;
    for (i = 0; i < 84; i++) {
        if (cKeyboardPopup_m_KeyValues[i] == '*' || i == 0x1C) {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else if (i == 69 || i == 79 || i == 41) {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else if (i == 75) {
            if (strlen((char*)&self->mszEditString) != 0 && self->mbDisableForFirst != 0)
                cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 1, 2);
            else
                cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 0, 2);
        } else {
            cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, 1, 2);
        }
    }
    cKeyboardPopup_setIncludeQuickKeyRow(self, 0);
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
        self->mbRegular[i] = bIncludeKey;
    }
    if (mode == 1 || mode == 2) {
        self->mbAlternate[i] = bIncludeKey;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setDisableShiftNumbers);
#ifdef SKIP_ASM
/**
 * Enables or disables the shifted number keys of the keyboard popup.
 *
 * For keys 15..24 (the number row) calls cKeyboardPopup_setIncludeKey(key, !on, 1), changing only the alternate key set
 * (mbAlternate).
 *
 * @param on  nonzero to disable the keys in the alternate set, 0 to enable them
 */
extern "C" void cKeyboardPopup_setDisableShiftNumbers(cKeyboardPopup* self, bool on)
{
    int i;
    for (i = 0xF; i < 25; i++) {
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, !on, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setIncludeQuickKeyRow);
#ifdef SKIP_ASM
/**
 * Includes or excludes the keyboard popup's quick-key row.
 *
 * For keys 0..13 calls cKeyboardPopup_setIncludeKey(key, @p included, 2), in both key sets.
 * cKeyboardPopup_fillSelectableKeys ends with a call that excludes them.
 *
 * @param included  nonzero to make the keys selectable
 */
extern "C" void cKeyboardPopup_setIncludeQuickKeyRow(cKeyboardPopup* self, int included)
{
    int i;
    for (i = 0; i < 0xE; i++) {
        cKeyboardPopup_setIncludeKey((cKeyboardPopup*)self, i, included, 2);
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
    sVec3_CE4C8 objScale = *(sVec3_CE4C8*)((char*)obj + 0x50);
    cFont* font = (cFont*)cUIText_getFont(obj);
    float x = font->BaseScale[0] * objScale.x;
    float y = font->BaseScale[1] * objScale.y;
    font->Scale[0] = x;
    font->Scale[1] = y;
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
    bool capsOn = self->mbCapsOn != 0;
    int flag;
    if (self->mbShiftOn) {
        flag = capsOn;
        flag = !flag;
    } else {
        flag = capsOn != 0;
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
INCLUDE_ASM("fe/fepopup", cKeyboardPopup_handleQuickKeyPress);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBEOptionInterface_getQuickKeyMessage(void* self, int i);
int cBEOptionInterface_getDefaultQuickKeyMessageHashValue(void* self, int value);
extern "C" void USTR_strcpyW(char* dst, void* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);


/**
 * Appends a quick message to the edit string, cut to the room left, and moves the cursor to its end.
 *
 * The text is the player's message @p key from the option interface (cBEOptionInterface_getQuickKeyMessage) or, when unset, its localised
 * default (cBEOptionInterface_getDefaultQuickKeyMessageHashValue). The cursor ends on the last character when the
 * string is full, else just after it.
 *
 * @param key  the quick-message index (On Tour iKeyIndex)
 */
extern "C" void cKeyboardPopup_handleQuickKeyPress(cKeyboardPopup* self, int key)
{
    char buf[0x40];
    int room = self->miMaxCharacters - strlen((char*)&self->mszEditString);
    void* opt = cBE_getInterface_Fv(cBE_getBE(), 4);
    char* msg = cBEOptionInterface_getQuickKeyMessage(opt, key);
    if (msg == 0) {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)locale->mVtbl;
        void* str = vt[4].fn(vthis(locale, vt[4]), cBEOptionInterface_getDefaultQuickKeyMessageHashValue(opt, key));
        if (str)
            USTR_strcpyW(buf, str);
        else
            buf[0] = 0;
    } else {
        strcpy(buf, msg);
    }
    buf[room] = 0;
    strcat((char*)&self->mszEditString, buf);
    int len = strlen((char*)&self->mszEditString);
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

extern sPopupSlot_E6F0 gMedalsInfo[][4][5];

/**
 * Counts the events listed for one peak of one mountain in the event table gMedalsInfo.
 *
 * Looks at the five entries gMedalsInfo[@p mountain][@p peak][0..4] and counts them up to the first whose first byte (the event's
 * track/kind selector) is negative. Used by the trophy room (cFEStateTrophyRoom_onWidgetCreate) and the lodge
 * (cFEStatePeakRoom_updateMedalsList) to size their event rows.
 *
 * @param mountain  the mountain (0..2)
 * @param peak  the peak (0..3)
 * @return the number of events before the first terminator (0..5)
 */
extern "C" int func_001CE6F0(int mountain, int peak)
{
    int n = 0;
    for (int i = 0; i < 5; i++) {
        if (gMedalsInfo[mountain][peak][i].id >= 0) {
            n++;
        } else {
            break;
        }
    }
    return n;
}
#endif

