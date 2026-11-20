#include "common.h"
#include "fe/festatetrophyroom.h"
#include "ui/uigroup.h"
#include "ui/uimenu.h"
#include "ui/uiinput.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festatepeakroom.h"
#include "fe/festatemountainroom.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_cFEStateMountainRoom);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cUIState_cUIState_Room(void *, int) __asm__("cUIState_cUIState");

extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

int cBENewPlayerInterface_getPlayerCharID(void* self, int index);

extern "C" int cUITextureBank_getElementByHashname(void* self, int id);

int GetHashValue32(char* str);

extern void* cFEStateMountainRoom_vtbl[];

extern "C" int cBENewPlayerInterface_isPeakLocked(void* self, int a, int b, int peak);
int cBELibrary_getRiderIndex(int rider) __asm__("cBELibrary_getRiderIndex__Fi");
extern "C" int cBEStatInterface_getStatLevel(void* iface, int rider);
extern "C" char* cBERewardInterface_getPeakPass(void* self, int a1, int a2);
extern "C" void cFEStateMountainRoom_loadPeakPass_Room(void* self) __asm__("cFEStateMountainRoom_loadPeakPass");
extern char str_dot_loc_arrow[];
extern char str_dot_visit_arrow[];

struct sMountainRoomK2678 {
    char pad_0x00[0x44];
    signed char player;      // 0x44
    char pad_0x45[0x3];
    int f48;
    int f4C;
    int f50;
    int f54;
    int a[5];                // 0x58
    int b[5][3];             // 0x6C
    int fA8;
    int fAC;
    int charID;              // 0xB0
    int fB4;
    int fB8;
    int fBC;
    char* fC0;
    int fC4;
};

/**
 * Constructs the lodge's mountain (trophy) room state for a player and starts loading the character's peak
 * pass.
 *
 * Records the player's character, the highest unlocked peak (+0xB4), the rider's level (+0xB8) and that
 * peak's pass
 * item (+0xC0), loads the "dot_loc arrow" and "dot_visit arrow" textures, then starts the pass image load
 * (cFEStateMountainRoom_loadPeakPass).
 *
 * @param ui  the UI engine (On Tour cUIEngine* pEngine)
 * @param idx  the player number (On Tour userNum)
 * @return @p self
 */
extern "C" void* cFEStateMountainRoom_cFEStateMountainRoom(cFEStateMountainRoom* self, cUIEngine* ui, int idx)
{
    signed char pl = idx;
    cUIState_cUIState_Room(self, (int)ui);
    self->mHashName = 43;
    self->mVtbl = cFEStateMountainRoom_vtbl;
    sMountainRoomK2678* room = (sMountainRoomK2678*)self;
    room->player = pl;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, pl);
    room->f48 = 0;
    room->f50 = 0;
    room->f54 = 0;
    for (int i = 4; i >= 0; i--) {
        room->a[i] = 0;
    }
    int* row1 = room->b[1];
    int* row2 = room->b[2];
    int* row3 = room->b[3];
    int* row4 = room->b[4];
    for (int i = 2; i >= 0; i--) {
        room->b[0][i] = 0;
    }
    for (int i = 2; i >= 0; i--) {
        row1[i] = 0;
    }
    for (int i = 2; i >= 0; i--) {
        row2[i] = 0;
    }
    for (int i = 2; i >= 0; i--) {
        row3[i] = 0;
    }
    for (int i = 2; i >= 0; i--) {
        row4[i] = 0;
    }
    room->fA8 = 0;
    room->fAC = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    room->charID = cBENewPlayerInterface_getPlayerCharID(iface, room->player);
    for (int i = 0; i < 3; i++) {
        if (cBENewPlayerInterface_isPeakLocked(iface, room->player, room->charID, i) == 0) {
            room->fB4 = i;
        }
    }
    void* iface3 = cBE_getInterface_Fv(cBE_getBE(), 3);
    room->fB8 = cBEStatInterface_getStatLevel(iface3, cBELibrary_getRiderIndex(room->player));
    void* iface13 = cBE_getInterface_Fv(cBE_getBE(), 13);
    char* item = cBERewardInterface_getPeakPass(iface13, room->fB4, room->charID);
    room->fBC = -1;
    room->fC4 = -1;
    room->fC0 = item;
    room->fAC = cUITextureBank_getElementByHashname((char*)&ui->mTextureBank, GetHashValue32(str_dot_loc_arrow));
    room->fA8 = cUITextureBank_getElementByHashname((char*)&ui->mTextureBank, GetHashValue32(str_dot_visit_arrow));
    cFEStateMountainRoom_loadPeakPass_Room(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom__cFEStateMountainRoom);
#ifdef SKIP_ASM
struct sVEntry001D28F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern void* cFEStateMountainRoom_vtbl[];
extern void* cFEState_vtbl[];
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void cUIState__cUIState(void* self, int flags);

/**
 * Destroys the mountain room: releases a pending pass-image load and frees the pass texture, then the base
 * state.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateMountainRoom__cFEStateMountainRoom(cFEStateMountainRoom* self, int flags)
{
    self->mVtbl = cFEStateMountainRoom_vtbl;
    int slot = self->mTextureFileIndex;
    if (slot >= 0) {
        func_0019DC20((char*)SSXApp->RealFE + 0x1A70, self->mCharID, slot);
        self->mTextureFileIndex = -1;
    }
    int id = self->mFrontTextureID;
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntry001D28F8* vt = (sVEntry001D28F8*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, id);
    }
    self->mVtbl = cFEState_vtbl;
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern char str_125mountainroom[];
extern char str_Numb_1[];
extern char str_Numb_2[];
extern char str_Numb_3[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];

/**
 * Creates the mountain room screen ("125mountainroom"), labels the peak numbers 1-3, and calls
 * func_0028F140(.., 7) on the audio system.
 */
extern "C" void cFEStateMountainRoom_onCreateScreen(cFEStateMountainRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_125mountainroom), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* textObj1 = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Numb_1));
    if (textObj1 != 0) {
        cUIText_setAsciiString(textObj1, D_004A14F8);
    }
    cUIText* textObj2 = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Numb_2));
    if (textObj2 != 0) {
        cUIText_setAsciiString(textObj2, D_004A1500);
    }
    cUIText* textObj3 = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Numb_3));
    if (textObj3 != 0) {
        cUIText_setAsciiString(textObj3, D_004A1508);
    }
    func_0028F140(cAudioInterface_bxAudio(), 7);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cFEStateMountainRoom_updateMarkers(cFEStateMountainRoom* self);
extern "C" void cFEState_onGainFocus(void* self, int a1);

/**
 * Refreshes the mountain room's event widgets, then gives the state focus.
 *
 * cFEStateMountainRoom_updateMarkers() shows one widget per event of the first two peaks on each of the three mountains, coloured by
 * the event's state for the room's player and character; cFEState_onGainFocus() is the base focus handling.
 *
 * @param pPrevState  the state that had focus before (On Tour: pPrevState), passed on to cFEState_onGainFocus()
 */
extern "C" void cFEStateMountainRoom_onGainFocus(cFEStateMountainRoom* self, int pPrevState)
{
    cFEStateMountainRoom_updateMarkers(self);
    cFEState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" int cUITextureBank_getElementByHashname(void* self, int id);
// The array-initializer clear (memset) is a void libcall here.
extern "C" void func_00416210(void* dst, int c, int n);

struct sName5_2AD0 { char c[5]; };
extern sName5_2AD0 D_00467298[];
extern char str_screentitle[];
extern char str_kT_TITLETrophyRoom[];
extern char str_Group_peak_pass[];
extern char str_Front[];
extern char str_peak_pass[];
extern char str_level_numb_back[];
extern char str_level_numb_front[];
extern char str_mrk_race_0[];
extern char str_mrk_race_1[];
extern char str_mrk_race_2[];
extern char str_mrk_race_3[];
extern char str_mrk_race_4[];
extern char str_mrk_slopestyle_0[];
extern char str_mrk_slopestyle_1[];
extern char str_mrk_slopestyle_2[];
extern char str_mrk_superpipe_0[];
extern char str_mrk_superpipe_1[];
extern char str_mrk_superpipe_2[];
extern char str_mrk_racebackcountry_0[];
extern char str_mrk_racebackcountry_1[];
extern char str_mrk_racebackcountry_2[];
extern char str_mrk_freestylebackcountry_0[];
extern char str_mrk_freestylebackcountry_1[];
extern char str_mrk_freestylebackcountry_2[];
extern char str_mrk_bigair_0[];
extern char str_mrk_bigair_1[];
extern char str_mrk_bigair_2[];
extern char str_Peak_pass_text[];
extern char str_peak1[];
extern char str_peak2[];
extern char str_peak3[];
extern char str_helptext[];

struct sWVEnt_2AD0 { short delta; short index; void (*fn)(void*, int); };

struct sWidget_2AD0 {
    char pad0[8];
    sWVEnt_2AD0* vt;    // 0x08
    char padC[0x18 - 0xC];
    int index;          // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;           // 0x38
    char pad3C[0x78 - 0x3C];
    int f78;            // 0x78
    int f7C;            // 0x7C
};

static inline void wShow_2AD0(sWidget_2AD0* w, int on)
{
    w->vt[9].fn((char*)w + w->vt[9].delta, on);
}

static inline void setW_2AD0(char* s, int off, sWidget_2AD0* w)
{
    *(sWidget_2AD0**)(s + off) = w;
    wShow_2AD0(w, 0);
}

/**
 * Binds the mountain room's widgets by name as the screen creates them.
 *
 * Sets the title (kT_TITLETrophyRoom), stores the peak-pass group, its front and the help text, the event
 * markers
 * ("mrk_race_0" and the others), sets the level-number icon ("nm<level>w") and numbers the peak-pass and peak
 * entries
 * 0-3.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateMountainRoom_onWidgetCreate(cFEStateMountainRoom* self, sWidget_2AD0* w)
{
    int widgetHash = w->hash;
    if (widgetHash == GetHashValue32(str_screentitle)) {
        cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLETrophyRoom));
        return;
    }
    if (widgetHash == GetHashValue32(str_Group_peak_pass)) {
        setW_2AD0(((char*)self), 72, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_Front)) {
        setW_2AD0(((char*)self), 76, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_peak_pass)) {
        self->mpPassIcon = (cUIIcon*)w;
        return;
    }
    if (widgetHash == GetHashValue32(str_level_numb_back) || widgetHash == GetHashValue32(str_level_numb_front)) {
        char buf[20];
        *(sName5_2AD0*)buf = D_00467298[0];
        func_00416210(buf + 5, 0, 15);
        char* eng = (char*)self->mpEngine;
        buf[2] = *(unsigned char*)&self->mLevel + '0';
        int hash = GetHashValue32(buf);
        int texElem = cUITextureBank_getElementByHashname(eng + 0x58, hash);
        w->f78 = -1;
        w->f7C = texElem;
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_0)) {
        setW_2AD0(((char*)self), 88, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_1)) {
        setW_2AD0(((char*)self), 92, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_2)) {
        setW_2AD0(((char*)self), 0x60, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_3)) {
        setW_2AD0(((char*)self), 100, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_4)) {
        setW_2AD0(((char*)self), 104, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_0)) {
        setW_2AD0(((char*)self), 108, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_1)) {
        setW_2AD0(((char*)self), 0x70, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_2)) {
        setW_2AD0(((char*)self), 116, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_0)) {
        setW_2AD0(((char*)self), 144, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_1)) {
        setW_2AD0(((char*)self), 148, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_2)) {
        setW_2AD0(((char*)self), 152, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_0)) {
        setW_2AD0(((char*)self), 0x78, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_1)) {
        setW_2AD0(((char*)self), 0x7C, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_2)) {
        setW_2AD0(((char*)self), 0x80, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_0)) {
        setW_2AD0(((char*)self), 132, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_1)) {
        setW_2AD0(((char*)self), 136, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_2)) {
        setW_2AD0(((char*)self), 140, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_0)) {
        setW_2AD0(((char*)self), 156, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_1)) {
        setW_2AD0(((char*)self), 160, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_2)) {
        setW_2AD0(((char*)self), 164, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_Peak_pass_text)) {
        w->index = 0;
        return;
    }
    if (widgetHash == GetHashValue32(str_peak1)) {
        w->index = 1;
        return;
    }
    if (widgetHash == GetHashValue32(str_peak2)) {
        w->index = 2;
        return;
    }
    if (widgetHash == GetHashValue32(str_peak3)) {
        w->index = 3;
        return;
    }
    if (widgetHash == GetHashValue32(str_helptext)) {
        setW_2AD0(((char*)self), 84, w);
        return;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onInputBegin);
#ifdef SKIP_ASM
struct sVEi1D2EA0 { short delta; short index; int (*fn)(void*); };
struct sVEv1D2EA0 { short delta; short index; void (*fn)(void*, int); };
extern char gUISoundNull;

static inline int IsShown1D2EA0(char* self)
{
    char* w = *(char**)(self + 0x48);
    return w != 0 && ((*(int*)(w + 0x14) >> 6) & 1);
}

/**
 * Closes the peak-pass view when the input passes its vtable slot-6 test: plays UI sound 3 and hides the pass
 * group.
 *
 * @param widget  the input (On Tour cUIInput* pInput)
 * @return 1 when the peak pass was shown and is closed, otherwise 0
 */
// Lodge menu: when the detail panel is shown and the widget accepts, plays the select sound and closes the panel.
extern "C" int cFEStateMountainRoom_onInputBegin(cFEStateMountainRoom* self, cUIInput* widget)
{
    if (IsShown1D2EA0(((char*)self))) {
    sVEi1D2EA0* wvt = (sVEi1D2EA0*)widget->mVtbl;
    if (wvt[6].fn(((char*)widget) + wvt[6].delta)) {
        char* snd = *(char**)&self->mpEngine->mpSound;
        if (snd == 0) {
            snd = &gUISoundNull;
        }
        sVEv1D2EA0* svt = *(sVEv1D2EA0**)snd;
        svt[2].fn(snd + svt[2].delta, 3);
        char* w = (char*)self->mpPassGroup;
        sVEv1D2EA0* vt = *(sVEv1D2EA0**)(w + 8);
        vt[9].fn(w + vt[9].delta, 0);
    }
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu events 8 and 9 in the mountain room.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param key  menu event number (On Tour Event)
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateMountainRoom_onWidgetMenu(cFEStateMountainRoom* self, int menu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void cFEStateMountainRoom_showPassFront_MR(void* self) __asm__("cFEStateMountainRoom_showPassFront");
extern "C" void cUIStateStack_push_MR(void* list, void* item) __asm__("cUIStateStack_push");
extern char str_kT_HELPViewPass[];
extern char str_kT_HELPViewTrophies[];
extern char str_kT_MAPHELPLockedPeak2[];
extern char str_kT_MAPHELPLockedPeak3[];

struct sVE_MR {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sFactory_MR {
    int pad0;
    sVE_MR* vt;
};

struct sTVE_MR {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sText_MR {
    int pad[2];
    sTVE_MR* vt;
};

/**
 * Handles the mountain room menu: entry 0 shows the peak pass, entries 1-3 open that peak's trophy room.
 *
 * Event 5 (select) opens the entry; event 6 pushes the state the engine's factory gives for the entry (slot
 * 5),
 * if any; event 1 (highlight) shows the entry's help: view pass, view trophies, or peak 2/3 locked.
 *
 * @param w  the menu entry (its index at +0x18), or 0
 * @param msg  widget event number (On Tour Event)
 */
// Widget events of the mountain-room menu: opens sub-screens and updates the description text.
extern "C" void cFEStateMountainRoom_onWidgetEvent(cFEStateMountainRoom* self, cUIThing* w, unsigned int msg)
{
    if (((char*)w) == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        int idx = w->mMiscData;
        if (idx == 0) {
            cFEStateMountainRoom_showPassFront_MR(((char*)self));
            break;
        }
        sFactory_MR* factory = *(sFactory_MR**)self->mpEngine;
        void* nextState = factory->vt[4].fn((char*)factory + factory->vt[4].delta, ((char*)self), idx - 1);
        cUIStateStack_push_MR(self->mpEngine->mStateStack, nextState);
        break;
    }
    case 6: {
        sFactory_MR* factory = *(sFactory_MR**)self->mpEngine;
        void* nextState = factory->vt[5].fn((char*)factory + factory->vt[5].delta, ((char*)self), w->mMiscData);
        if (nextState == 0) {
            break;
        }
        cUIStateStack_push_MR(self->mpEngine->mStateStack, nextState);
        break;
    }
    case 1: {
        if (self->mpHelpText == 0) {
            break;
        }
        int stringId = 0;
        switch (w->mMiscData) {
        case 0:
            stringId = GetHashValue32(str_kT_HELPViewPass);
            break;
        case 1:
            stringId = GetHashValue32(str_kT_HELPViewTrophies);
            break;
        case 2:
            if (self->mPeakNum > 0) {
                stringId = GetHashValue32(str_kT_HELPViewTrophies);
            } else {
                stringId = GetHashValue32(str_kT_MAPHELPLockedPeak2);
            }
            break;
        case 3:
            if (self->mPeakNum >= 2) {
                stringId = GetHashValue32(str_kT_HELPViewTrophies);
            } else {
                stringId = GetHashValue32(str_kT_MAPHELPLockedPeak3);
            }
            break;
        }
        sText_MR* helpText = (sText_MR*)self->mpHelpText;
        helpText->vt[9].fn((char*)helpText + helpText->vt[9].delta, 1);
        cUIText_setUnicodeStringByID(self->mpHelpText, stringId);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStateMountainRoom_updateTextureLoad(cFEStateMountainRoom* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the mountain room, first finishing its picture load while one is pending.
 *
 * While the async slot at 0xBC is not -1, cFEStateMountainRoom_updateTextureLoad() checks the load; once it has loaded, the data is
 * handed to GrMan under the name "pass" and attached to the widget at 0x50, and the slot is released. Then runs
 * the base UI-state update cUIState_onUpdate().
 */
extern "C" void cFEStateMountainRoom_onUpdate(cFEStateMountainRoom* self)
{
    if (~self->mTextureFileIndex != 0) {
        cFEStateMountainRoom_updateTextureLoad(self);
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_loadPeakPass);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);

/**
 * Starts loading the mountain room's peak-pass picture, unless a load is already pending or there is no pass to show.
 *
 * Only when the load slot (+0xBC) is negative and the pass item (+0xC0) is set: asks the front end's file loader
 * (SSXApp->RealFE + 0x1A70, func_0019DA10) for the item's file (its word at +4) under the character id (+0xB0), with
 * arguments 10, 1, 0, and keeps the returned slot at +0xBC. cFEStateMountainRoom_onUpdate finishes the load through
 * cFEStateMountainRoom_updateTextureLoad.
 */
extern "C" void cFEStateMountainRoom_loadPeakPass(cFEStateMountainRoom* self)
{
    if (self->mTextureFileIndex < 0) {
        void* item = self->mpPeakPassInfo;
        if (item != 0) {
            self->mTextureFileIndex = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, self->mCharID, *(int*)((char*)item + 4), 10, 1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_updateTextureLoad);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* mgr, int bank, int i);
extern "C" void* func_0019E2B0(void* mgr, int bank, int i);
extern "C" void cFEStateMountainRoom_showPassFront(cFEStateMountainRoom* self);
extern char str_pass[];

struct sVE_31C0 {
    short delta;
    short index;
    int (*fn)(void*, void*, void*, int, int, int);
};

struct sWidget_31C0 {
    char pad0[0x14];
    int flags;      // 0x14
};

static inline int isActive_31C0(sWidget_31C0* w)
{
    return w != 0 && ((w->flags >> 6) & 1);
}

/**
 * Finishes the mountain room's peak-pass picture load: once the file is in, turns it into the "pass" texture, shows the
 * pass front when appropriate and releases the file.
 *
 * Does nothing without the pass icon (+0x50) or while the loader (SSXApp->RealFE + 0x1A70) reports slot +0xBC of
 * character +0xB0 as not loaded (func_0019E238). Otherwise takes the file data (func_0019E2B0), creates the texture
 * "pass" from the data at its own offset (+0x14) through GrMan's virtual slot 46, and stores the texture id at +0xC4
 * and in the icon (+0x78, with +0x7C cleared). When the pass group (+0x48) is visible (widget flag bit 6) and the
 * pass-front group (+0x4C) is not, shows them (cFEStateMountainRoom_showPassFront). Finally frees the loader slot (func_0019DC20) and sets
 * +0xBC to -1.
 *
 * @param vself  the mountain-room state (this)
 */
extern "C" void cFEStateMountainRoom_updateTextureLoad(cFEStateMountainRoom* vself)
{
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    if (vself->mpPassIcon == 0) {
        return;
    }
    if (func_0019E238(mgr, vself->mCharID, vself->mTextureFileIndex) == 0) {
        return;
    }
    char* fileData = (char*)func_0019E2B0(mgr, vself->mCharID, vself->mTextureFileIndex);
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_31C0* vt = (sVE_31C0*)ps2GraphicsMan->mVtbl;
    int id = vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, fileData + *(int*)(fileData + 0x14), str_pass, 0, 1, -1);
    char* snd = (char*)vself->mpPassIcon;
    vself->mFrontTextureID = id;
    *(int*)(snd + 0x78) = id;
    *(int*)(snd + 0x7C) = 0;
    sWidget_31C0* passGroup = (sWidget_31C0*)vself->mpPassGroup;
    if (isActive_31C0(passGroup)) {
        int both = 0;
        if (isActive_31C0(passGroup)) {
            sWidget_31C0* passFrontGroup = (sWidget_31C0*)vself->mpPassFrontGroup;
            if (passFrontGroup != 0) {
                both = (passFrontGroup->flags >> 6) & 1;
            }
        }
        if (!both) {
            cFEStateMountainRoom_showPassFront(vself);
        }
    }
    func_0019DC20(mgr, vself->mCharID, vself->mTextureFileIndex);
    vself->mTextureFileIndex = -1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_showPassFront);
#ifdef SKIP_ASM
struct sVEntry_cFEStateMountainRoom_showPassFront {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows the mountain room's peak pass: the pass group (+0x48) always, the pass-front group (+0x4C) only when the front
 * texture exists (+0xC4 >= 0).
 *
 * Both through virtual slot 9 (show) of the widget's vtable at +0x8 with 1.
 */
extern "C" void cFEStateMountainRoom_showPassFront(cFEStateMountainRoom* self)
{
    {
        cUIGroup* obj = self->mpPassGroup;
        sVEntry_cFEStateMountainRoom_showPassFront* vt = *(sVEntry_cFEStateMountainRoom_showPassFront**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
    if (self->mFrontTextureID >= 0) {
        cUIGroup* obj = self->mpPassFrontGroup;
        sVEntry_cFEStateMountainRoom_showPassFront* vt = *(sVEntry_cFEStateMountainRoom_showPassFront**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_updateMarkers);
#ifdef SKIP_ASM
extern "C" int getChallengeMedal(int player, int charId, int mountain, int peak, int i);

struct sPeakEvent_3340 {
    signed char track;          // widget list selector; < 0 ends the peak's list
    signed char kind;
};
extern sPeakEvent_3340 D_0045AAD8[][4][5];

class cWidget_3340 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sLodge_3340 {
    char pad0[0x44];
    signed char player;         // 0x44
    char pad45[0x58 - 0x45];
    cWidget_3340* a[5];         // 0x58
    cWidget_3340* b[3];         // 0x6C
    cWidget_3340* c[3];         // 0x78
    cWidget_3340* d[3];         // 0x84
    cWidget_3340* e[3];         // 0x90
    cWidget_3340* f[3];         // 0x9C
    int colOn;                  // 0xA8
    int colOff;                 // 0xAC
    int charId;                 // 0xB0
};

static inline void wColour_3340(cWidget_3340* w, int col)
{
    *(int*)((char*)w + 0x7C) = col;
    *(int*)((char*)w + 0x78) = -1;
}

// Colour widget n of list `arr` by the event state, show it, advance n.
#define WSET_3340(arr, n)                                   \
    {                                                       \
        if (st < 4)                                         \
            wColour_3340(self->arr[n], self->colOn);        \
        else                                                \
            wColour_3340(self->arr[n], self->colOff);       \
        self->arr[n++]->show(1);                            \
    }

/**
 * Fills the mountain room's event markers: one marker per event of the first two challenge groups of every peak,
 * showing a done or not-done picture.
 *
 * For peaks 0-2 and challenge groups 0 and 1 (groups 2 and 3, collectibles / big challenges and earnings, are skipped)
 * it walks the peak's challenge table D_0045AAD8 until an entry's list selector is negative. getChallengeMedal gives the
 * entry's medal for the player (+0x44) and character (+0xB0); a value below 4 picks the done element (+0xA8), anything
 * else the not-done element (+0xAC). The selector picks the next free marker of one list: 0 -> +0x58 (5 markers), 1 ->
 * +0x6C, 2 -> +0x90, 3 -> +0x9C, 4 -> +0x78, 5 -> +0x84 (3 each). The marker gets the element (+0x7C, texture id +0x78
 * = -1) and is shown (virtual slot 8).
 *
 * @param mountainRoom  the mountain-room state (this)
 */
extern "C" void cFEStateMountainRoom_updateMarkers(cFEStateMountainRoom* mountainRoom)
{
    sLodge_3340* self = (sLodge_3340*)mountainRoom;
    int n0 = 0;
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    int n4 = 0;
    int n5 = 0;
    for (int mountain = 0; mountain < 3; mountain++) {
        for (int peak = 0; peak < 4; peak++) {
            if (peak == 2 || peak == 3)
                continue;
            for (int i = 0; i < 5; i++) {
                int track = D_0045AAD8[mountain][peak][i].track;
                if (track < 0)
                    break;
                unsigned int st = getChallengeMedal(self->player, self->charId, mountain, peak, i);
                if (track == 0) {
                    WSET_3340(a, n0);
                } else if (track == 1) {
                    WSET_3340(b, n1);
                } else if (track == 2) {
                    WSET_3340(e, n2);
                } else if (track == 3) {
                    WSET_3340(f, n3);
                } else if (track == 4) {
                    WSET_3340(c, n4);
                } else if (track == 5) {
                    WSET_3340(d, n5);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_cFEStatePeakRoom);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern void* cFEStatePeakRoom_vtbl[];

struct sLodge_3780 {
    char pad_0x0[0x48];
    int slots[4];       // 0x48
    int grid[6][5];     // 0x58
    int fD0;            // 0xD0
    char pad_0xD4[0x8];
    int charID;         // 0xDC
    int fE0;            // 0xE0
};

/**
 * Constructs the lodge's peak room state (the list of one peak's trophies) for a player.
 *
 * Runs the cUIState constructor (cUIState_cUIState), sets mHashName to 44 and the vtable cFEStatePeakRoom_vtbl, stores the
 * player number (low byte of @p userNum) in mUserNum and that player's controller mask (cFECustom_getPlayerCtrlMask on the engine's first
 * word) in mCtrlBits. The character id (+0xDC) comes from the player interface (back-end interface 1,
 * cBENewPlayerInterface_getPlayerCharID), the peak (+0xE0) is @p peakNum; the widget pointers from +0x48 to +0xD0 are
 * cleared.
 *
 * @param engine  the UI engine (passed through to the cUIState constructor)
 * @param userNum  the player number (On Tour userNum)
 * @param peakNum  the peak (On Tour peakNum)
 * @return @p self
 */
extern "C" void* cFEStatePeakRoom_cFEStatePeakRoom(cFEStatePeakRoom* self, int engine, int userNum, int peakNum)
{
    signed char idx = userNum;
    cUIState_cUIState(self);
    self->mHashName = 44;
    self->mVtbl = cFEStatePeakRoom_vtbl;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, idx);
    sLodge_3780* peakRoom = (sLodge_3780*)self;
    peakRoom->charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
    peakRoom->fE0 = peakNum;
    for (int i = 3; i >= 0; i--) {
        peakRoom->slots[i] = 0;
    }
    for (int j = 0; j < 5; j++) {
        peakRoom->grid[0][j] = 0;
        peakRoom->grid[1][j] = 0;
        peakRoom->grid[2][j] = 0;
        peakRoom->grid[3][j] = 0;
        peakRoom->grid[4][j] = 0;
        peakRoom->grid[5][j] = 0;
    }
    peakRoom->fD0 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_126peakroom[];

/**
 * Creates the peak (trophy) room screen ("126peakroom") and plays its first frame.
 */
extern "C" void cFEStatePeakRoom_onCreateScreen(cFEStatePeakRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_126peakroom), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBERewardInterface_getTrophy(void* self, int a1, int a2);
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" int cUITextureBank_getElementByHashname(void* self, int id);
extern char str_screentitle[];
extern char str_kT_Peak3name[];
extern char str_kT_Peak2name[];
extern char str_kT_Peak1name[];
extern char str_check[];
extern char str_nocheck[];
extern char str_helptext[];
extern char* D_004415E0[];
extern char* D_004415F0[];
extern char* D_00441540[];
extern char* D_00441558[];
extern char* D_00441570[];
extern char* str_HuF[];
extern char* D_004415A0[];
extern char* D_004415B8[];

struct sWVEnt_38F8 { short delta; short index; void (*fn)(void*, int); };

struct sWidget_38F8 {
    char pad0[8];
    sWVEnt_38F8* vt;    // 0x08
    char padC[0x18 - 0xC];
    int index;          // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;           // 0x38
    char pad3C[0x78 - 0x3C];
    int f78;            // 0x78
    int f7C;            // 0x7C
};

struct sPeak_38F8 {
    char pad0[0x10];
    char* engine;               // 0x10
    char pad14[0x44 - 0x14];
    signed char player;         // 0x44
    char pad45[0x48 - 0x45];
    sWidget_38F8* slots[4];     // 0x48
    sWidget_38F8* grid[6][5];   // 0x58
    sWidget_38F8* fD0;          // 0xD0
    int fD4;                    // 0xD4
    int fD8;                    // 0xD8
    int charID;                 // 0xDC
    int mode;                   // 0xE0
};

static inline void wShow_38F8(sWidget_38F8* w, int on)
{
    w->vt[9].fn((char*)w + w->vt[9].delta, on);
}

/**
 * Binds the peak room's widgets by name: the peak title, check marks, help text, trophies and event grid.
 *
 * Each of the four trophy slots shows the trophy's icon when the player's character owns it and is hidden
 * otherwise; the six rows of event widgets start hidden.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStatePeakRoom_onWidgetCreate(sPeak_38F8* self, sWidget_38F8* w)
{
    int widgetHash = w->hash;
    if (widgetHash == GetHashValue32(str_screentitle)) {
        int roomMode = self->mode;
        if (roomMode == 2)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_Peak3name));
        else if (roomMode == 1)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_Peak2name));
        else
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_Peak1name));
    }
    if (widgetHash == GetHashValue32(str_check)) {
        self->fD4 = w->f7C;
        return;
    }
    if (widgetHash == GetHashValue32(str_nocheck)) {
        self->fD8 = w->f7C;
        return;
    }
    if (widgetHash == GetHashValue32(str_helptext)) {
        self->fD0 = w;
        wShow_38F8(w, 0);
        return;
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    for (int i = 0; i < 4; i++) {
        if (widgetHash == GetHashValue32(D_004415F0[i])) {
            self->slots[i] = w;
            if (cBERewardInterface_isTrophyOwned(iface, self->player, self->charID, self->mode, i)) {
                char* info = cBERewardInterface_getTrophy(iface, self->mode, i);
                char* eng = self->engine;
                int hash = GetHashValue32(*(char**)(info + 8));
                int texElem = cUITextureBank_getElementByHashname(eng + 0x58, hash);
                sWidget_38F8* sw = self->slots[i];
                sw->f78 = -1;
                sw->f7C = texElem;
            } else {
                wShow_38F8(self->slots[i], 0);
            }
            return;
        }
        if (widgetHash == GetHashValue32(D_004415E0[i])) {
            w->index = i;
            return;
        }
    }
    for (int j = 0; j < 5; j++) {
        if (widgetHash == GetHashValue32(str_HuF[j])) {
            self->grid[1][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
        if (widgetHash == GetHashValue32(D_00441570[j])) {
            self->grid[0][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
        if (widgetHash == GetHashValue32(D_00441540[j])) {
            self->grid[2][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
        if (widgetHash == GetHashValue32(D_00441558[j])) {
            self->grid[3][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
        if (widgetHash == GetHashValue32(D_004415A0[j])) {
            self->grid[4][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
        if (widgetHash == GetHashValue32(D_004415B8[j])) {
            self->grid[5][j] = w;
            wShow_38F8(w, 0);
            w->index = j;
            return;
        }
    }
}
#endif

extern "C" void* cUIState_onGainFocus(void* self);

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the peak-room state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStatePeakRoom_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" void cFEStatePeakRoom_updateHelpText(cFEStatePeakRoom* self, int id);
extern "C" void cFEStatePeakRoom_updateMedalsList(cFEStatePeakRoom* self, int id);
extern "C" void cUIStateStack_push(void* list, void* item);

struct sVEntA_001D3C80 { short delta; short index; void* (*fn)(void*, void*, int); };
struct sVEntB_001D3C80 { short delta; short index; void (*fn)(void*, int); };
struct sItem_001D3C80 { int pad[2]; sVEntB_001D3C80* vt; };

/**
 * Handles the peak room menu: select opens an owned trophy, event 6 opens the factory's state for it,
 * highlight updates help.
 *
 * Both open through the engine's state factory with (peak << 16 | trophy); event 6 first hides the four
 * trophy
 * slots. Event 1 updates the help text and calls cFEStatePeakRoom_updateMedalsList().
 *
 * @param item  the menu entry (its trophy index at +0x18), or 0
 * @param key  widget event number (On Tour Event)
 */
extern "C" void cFEStatePeakRoom_onWidgetEvent(cFEStatePeakRoom* self, cUIThing* item, unsigned int key)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (key) {
    case 5:
        if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum,
                          self->mCharID, self->mPeakNum, item->mMiscData) != 0) {
            void* obj = *(void**)self->mpEngine;
            sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
            void* nextState = vt[4].fn((char*)obj + vt[4].delta, self, (self->mPeakNum << 16) | item->mMiscData);
            cUIStateStack_push(self->mpEngine->mStateStack, nextState);
        }
        break;
    case 6: {
        int id = (self->mPeakNum << 16) | item->mMiscData;
        for (int i = 0; i < 4; i++) {
            sItem_001D3C80* listItem = ((sItem_001D3C80**)&self->mpTrophyIcons)[i];
            if (listItem != 0) {
                listItem->vt[9].fn((char*)listItem + listItem->vt[9].delta, 0);
            }
        }
        void* obj = *(void**)self->mpEngine;
        sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, id);
        if (nextState != 0) {
            cUIStateStack_push(self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 1:
        cFEStatePeakRoom_updateHelpText(self, item->mMiscData);
        cFEStatePeakRoom_updateMedalsList(self, item->mMiscData);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" void* cList_getByIndex(void* list, int id);

/**
 * Filters the peak room's menu events: swallows 8 and 9 and refuses event 6 on a trophy not yet owned.
 *
 * @param item  the menu (On Tour cUIMenu* pMenu)
 * @param key  menu event number (On Tour Event)
 * @param id  the menu item index (On Tour index)
 * @return 0x100 for events 8 and 9, 0x10 for event 6 on an unowned trophy, otherwise 0x101
 */
extern "C" int cFEStatePeakRoom_onWidgetMenu(cFEStatePeakRoom* self, cUIMenu* item, unsigned int key, int id)
{
    switch (key) {
    default:
        break;
    case 8:
    case 9:
        return 0x100;
    case 6: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* entry = cList_getByIndex((char*)&item->mList, id);
        if (cBERewardInterface_isTrophyOwned(iface, self->mUserNum, self->mCharID,
                          self->mPeakNum, *(int*)((char*)entry + 0x18)) == 0) {
            return 0x10;
        }
        break;
    }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_kT_HELPSelectGoal[];
extern char str_kT_HELP_LockedGoal[];

class cPeakWidget_3EC8 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

/**
 * Sets the peak room's help line to select goal (trophy owned) or locked goal, and shows it.
 *
 * @param index  the trophy (On Tour trophyType)
 */
extern "C" void cFEStatePeakRoom_updateHelpText(cFEStatePeakRoom* self, int index)
{
    if (self->mpHelpText != 0) {
        if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum,
                          self->mCharID, self->mPeakNum, index) != 0) {
            cUIText_setUnicodeStringByID(self->mpHelpText, GetHashValue32(str_kT_HELPSelectGoal));
        } else {
            cUIText_setUnicodeStringByID(self->mpHelpText, GetHashValue32(str_kT_HELP_LockedGoal));
        }
        ((cPeakWidget_3EC8*)self->mpHelpText)->show(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStatePeakRoom_updateMedalsList);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv_3F80(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001CE6F0(int mountain);
extern "C" int getChallengeMedal(int player, int charId, int mountain, int peak, int i);
extern "C" void setChallengeName(void* text, int mountain, int peak, int i);
extern "C" char* cBERewardInterface_getMedal(void* iface, int peak, int ch);
extern "C" int cUITextureBank_getElementByHashname(void* self, int id);

class cWidget_3F80 {
public:
    int pad0, pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void show(int on);
};

struct sRoom_3F80 {
    char pad_0x0[0x10];
    char* engine;                   // 0x10
    char pad_0x14[0x30];
    signed char player;             // 0x44
    char pad_0x45[0x13];
    cWidget_3F80* icon[5];          // 0x58
    cWidget_3F80* box[5];           // 0x6C
    cWidget_3F80* name[5];          // 0x80
    cWidget_3F80* bar[5];           // 0x94
    cWidget_3F80* medal[5];         // 0xA8
    cWidget_3F80* lock[5];          // 0xBC
    char pad_0xD0[0xC];
    int charId;                     // 0xDC
    int mountain;                   // 0xE0
};

/**
 * Fills the peak room's medal list for one trophy group of the current peak: a line per challenge with its name and,
 * when earned, the medal and a check mark, else a cross.
 *
 * First hides all six widget lists (+0x58 medal icons, +0x6C backgrounds, +0x80 names, +0x94 name backgrounds, +0xA8
 * check marks, +0xBC cross marks). func_001CE6F0 counts the group's entries in the challenge table for the peak
 * (+0xE0); it is declared here with one argument and gets the group from $a1, which still holds @p peak. For each
 * entry: getChallengeMedal gives the medal for the player (+0x44) and character (+0xDC), setChallengeName labels the line,
 * and the name, its background and the medal background are shown. With a medal (not -1) the icon takes the
 * texture-bank element (mpEngine + 0x58) named by the medal record's string (cBERewardInterface_getMedal on back-end
 * interface 13, record +0x8) and is shown with the check mark; without one the cross mark is shown.
 *
 * @param vself  the peak-room state (this)
 * @param peak  the trophy group, 0-3 (On Tour trophyType): despite its name not a peak; the peak is +0xE0
 */
extern "C" void cFEStatePeakRoom_updateMedalsList(cFEStatePeakRoom* vself, int peak)
{
    sRoom_3F80* self = (sRoom_3F80*)vself;
    int n;
    void* iface;
    int i;
    cWidget_3F80** icon = self->icon;
    cWidget_3F80** box = self->box;
    cWidget_3F80** name = self->name;
    cWidget_3F80** bar = self->bar;
    cWidget_3F80** medal = self->medal;
    cWidget_3F80** lock = self->lock;
    n = func_001CE6F0(self->mountain);
    for (int j = 0; j < 5; j++) {
        self->icon[j]->show(0);
        self->box[j]->show(0);
        self->name[j]->show(0);
        self->bar[j]->show(0);
        self->medal[j]->show(0);
        self->lock[j]->show(0);
    }
    iface = cBE_getInterface_Fv_3F80(cBE_getBE(), 13);
    for (i = 0; i < n; i++) {
        int ch = getChallengeMedal(self->player, self->charId, self->mountain, peak, i);
        setChallengeName(name[i], self->mountain, peak, i);
        name[i]->show(1);
        bar[i]->show(1);
        box[i]->show(1);
        if (ch != -1) {
            char* trophyRec = cBERewardInterface_getMedal(iface, peak, ch);
            char* eng = self->engine;
            int hash = GetHashValue32(*(char**)(trophyRec + 8));
            int textureElem = cUITextureBank_getElementByHashname(eng + 0x58, hash);
            cWidget_3F80* w = icon[i];
            *(int*)((char*)w + 0x7C) = textureElem;
            *(int*)((char*)w + 0x78) = -1;
            icon[i]->show(1);
            medal[i]->show(1);
        } else {
            lock[i]->show(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateTrophyRoom_cFEStateTrophyRoom);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void cFEStateTrophyRoom_loadTextures(void* self);
extern void* cFEStateTrophyRoom_vtbl[];

struct sVE_4268 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sTrophy_4268 {
    char pad_0x0[0x48];
    int f48;            // 0x48
    char pad_0x4C[0x4];
    int charID;         // 0x50
    int f54;            // 0x54
    int bank;           // 0x58
    int count;          // 0x5C
    int snd[5];         // 0x60
    int tex[5];         // 0x74
    char pad_0x88[0x14];
    int f9C;            // 0x9C
};

/**
 * Constructs the lodge's trophy room state (one trophy and its medals) and starts loading their pictures.
 *
 * Runs the cUIState constructor (cUIState_cUIState), sets mHashName to 45 and the vtable cFEStateTrophyRoom_vtbl, stores
 * the player number (low byte of @p userNum) in mUserNum and that player's controller mask (cFECustom_getPlayerCtrlMask) in mCtrlBits, and
 * clears +0x48. The character id (+0x50) comes from the player interface (back-end interface 1,
 * cBENewPlayerInterface_getPlayerCharID), followed by a call of that interface's virtual slot 2. @p peakNum goes to +0x54
 * and @p trophyType to +0x58; the load count (+0x5C) is zeroed, the five texture ids (+0x60) and five loader slots (+0x74) are
 * set to -1 and so is +0x9C. Ends with cFEStateTrophyRoom_loadTextures, which starts the loads of the trophy picture and its up to four
 * medal pictures. The prelude's name `bank` for +0x58 is misleading: it is the trophy, passed to
 * cBERewardInterface_getTrophy / getMedal.
 *
 * @param engine  the UI engine (passed through to the cUIState constructor)
 * @param userNum  the player number (On Tour userNum)
 * @param peakNum  the peak (On Tour peakNum)
 * @param trophyType  the trophy within the peak (On Tour trophyType)
 * @return @p self
 */
extern "C" void* cFEStateTrophyRoom_cFEStateTrophyRoom(cFEStateTrophyRoom* self, int engine, int userNum, int peakNum, int trophyType)
{
    signed char idx = userNum;
    cUIState_cUIState(self);
    self->mHashName = 45;
    self->mVtbl = cFEStateTrophyRoom_vtbl;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, idx);
    sTrophy_4268* trophyRoom = (sTrophy_4268*)self;
    trophyRoom->f48 = 0;
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    trophyRoom->charID = cBENewPlayerInterface_getPlayerCharID(pi, self->mUserNum);
    sVE_4268* vt = *(sVE_4268**)((char*)pi + 0xC);
    vt[2].fn((char*)pi + vt[2].delta);
    trophyRoom->f54 = peakNum;
    trophyRoom->bank = trophyType;
    trophyRoom->count = 0;
    for (int i = 0; i < 5; i++) {
        trophyRoom->snd[i] = -1;
        trophyRoom->tex[i] = -1;
    }
    trophyRoom->f9C = -1;
    cFEStateTrophyRoom_loadTextures(self);
    return ((void*)self);
}
#endif

