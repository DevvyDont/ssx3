#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_cFEStateMountainRoom);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_0039E2A0_Room(void *, int) __asm__("func_0039E2A0");

extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

int cBENewPlayerInterface_getPlayerCharID(void* self, int index);

extern "C" int cUITextureBank_getElementByHashname(void* self, int id);

int GetHashValue32(char* str);

extern void* cFEStateMountainRoom_vtbl[];

extern "C" int cBENewPlayerInterface_isPeakLocked(void* self, int a, int b, int peak);
int cBELibrary_getRiderIndex(int rider) __asm__("cBELibrary_getRiderIndex__Fi");
extern "C" int func_00148950(void* iface, int rider);
extern "C" char* func_00156B28(void* self, int a1, int a2);
extern "C" void func_001D3160_Room(void* self) __asm__("func_001D3160");
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
 * (func_001D3160).
 *
 * @param ui  the UI engine (On Tour cUIEngine* pEngine)
 * @param idx  the player number (On Tour userNum)
 * @return @p self
 */
extern "C" void* cFEStateMountainRoom_cFEStateMountainRoom(void* self, void* ui, int idx)
{
    signed char pl = idx;
    func_0039E2A0_Room(self, (int)ui);
    *(int*)((char*)self + 0xC) = 43;
    *(void***)((char*)self + 0x8) = cFEStateMountainRoom_vtbl;
    sMountainRoomK2678* room = (sMountainRoomK2678*)self;
    room->player = pl;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), pl);
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
    room->fB8 = func_00148950(iface3, cBELibrary_getRiderIndex(room->player));
    void* iface13 = cBE_getInterface_Fv(cBE_getBE(), 13);
    char* item = func_00156B28(iface13, room->fB4, room->charID);
    room->fBC = -1;
    room->fC4 = -1;
    room->fC0 = item;
    room->fAC = cUITextureBank_getElementByHashname((char*)ui + 0x58, GetHashValue32(str_dot_loc_arrow));
    room->fA8 = cUITextureBank_getElementByHashname((char*)ui + 0x58, GetHashValue32(str_dot_visit_arrow));
    func_001D3160_Room(self);
    return self;
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
extern void* D_0046D0D0[];
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void func_0039E390(void* self, int flags);

/**
 * Destroys the mountain room: releases a pending pass-image load and frees the pass texture, then the base
 * state.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateMountainRoom__cFEStateMountainRoom(void* self, int flags)
{
    *(void***)((char*)self + 8) = cFEStateMountainRoom_vtbl;
    int slot = *(int*)((char*)self + 0xBC);
    if (slot >= 0) {
        func_0019DC20((char*)SSXApp->RealFE + 0x1A70, *(int*)((char*)self + 0xB0), slot);
        *(int*)((char*)self + 0xBC) = -1;
    }
    int id = *(int*)((char*)self + 0xC4);
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntry001D28F8* vt = (sVEntry001D28F8*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, id);
    }
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
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
extern "C" void* func_0028B180();
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
extern "C" void cFEStateMountainRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_125mountainroom), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* textObj1 = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Numb_1));
    if (textObj1 != 0) {
        cUIText_setAsciiString(textObj1, D_004A14F8);
    }
    cUIText* textObj2 = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Numb_2));
    if (textObj2 != 0) {
        cUIText_setAsciiString(textObj2, D_004A1500);
    }
    cUIText* textObj3 = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Numb_3));
    if (textObj3 != 0) {
        cUIText_setAsciiString(textObj3, D_004A1508);
    }
    func_0028F140(func_0028B180(), 7);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onGainFocus);
#ifdef SKIP_ASM
extern "C" void func_001D3340(void* self);
extern "C" void cFEState_onGainFocus(void* self, int a1);

/**
 * Refreshes the mountain room's event widgets, then gives the state focus.
 *
 * func_001D3340() shows one widget per event of the first two peaks on each of the three mountains, coloured by
 * the event's state for the room's player and character; cFEState_onGainFocus() is the base focus handling.
 *
 * @param pPrevState  the state that had focus before (On Tour: pPrevState), passed on to cFEState_onGainFocus()
 */
extern "C" void cFEStateMountainRoom_onGainFocus(void* self, int pPrevState)
{
    func_001D3340(self);
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
extern "C" void cFEStateMountainRoom_onWidgetCreate(void* self, sWidget_2AD0* w)
{
    char* state = (char*)self;
    int widgetHash = w->hash;
    if (widgetHash == GetHashValue32(str_screentitle)) {
        cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLETrophyRoom));
        return;
    }
    if (widgetHash == GetHashValue32(str_Group_peak_pass)) {
        setW_2AD0(state, 72, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_Front)) {
        setW_2AD0(state, 76, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_peak_pass)) {
        *(sWidget_2AD0**)(state + 0x50) = w;
        return;
    }
    if (widgetHash == GetHashValue32(str_level_numb_back) || widgetHash == GetHashValue32(str_level_numb_front)) {
        char buf[20];
        *(sName5_2AD0*)buf = D_00467298[0];
        func_00416210(buf + 5, 0, 15);
        char* eng = *(char**)(state + 0x10);
        buf[2] = *(unsigned char*)(state + 0xB8) + '0';
        int hash = GetHashValue32(buf);
        int texElem = cUITextureBank_getElementByHashname(eng + 0x58, hash);
        w->f78 = -1;
        w->f7C = texElem;
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_0)) {
        setW_2AD0(state, 88, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_1)) {
        setW_2AD0(state, 92, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_2)) {
        setW_2AD0(state, 0x60, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_3)) {
        setW_2AD0(state, 100, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_race_4)) {
        setW_2AD0(state, 104, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_0)) {
        setW_2AD0(state, 108, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_1)) {
        setW_2AD0(state, 0x70, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_slopestyle_2)) {
        setW_2AD0(state, 116, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_0)) {
        setW_2AD0(state, 144, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_1)) {
        setW_2AD0(state, 148, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_superpipe_2)) {
        setW_2AD0(state, 152, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_0)) {
        setW_2AD0(state, 0x78, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_1)) {
        setW_2AD0(state, 0x7C, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_racebackcountry_2)) {
        setW_2AD0(state, 0x80, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_0)) {
        setW_2AD0(state, 132, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_1)) {
        setW_2AD0(state, 136, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_freestylebackcountry_2)) {
        setW_2AD0(state, 140, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_0)) {
        setW_2AD0(state, 156, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_1)) {
        setW_2AD0(state, 160, w);
        return;
    }
    if (widgetHash == GetHashValue32(str_mrk_bigair_2)) {
        setW_2AD0(state, 164, w);
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
        setW_2AD0(state, 84, w);
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
extern "C" int cFEStateMountainRoom_onInputBegin(char* self, char* widget)
{
    if (IsShown1D2EA0(self)) {
    sVEi1D2EA0* wvt = *(sVEi1D2EA0**)(widget + 8);
    if (wvt[6].fn(widget + wvt[6].delta)) {
        char* snd = *(char**)(*(char**)(self + 0x10) + 0x14);
        if (snd == 0) {
            snd = &gUISoundNull;
        }
        sVEv1D2EA0* svt = *(sVEv1D2EA0**)snd;
        svt[2].fn(snd + svt[2].delta, 3);
        char* w = *(char**)(self + 0x48);
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
extern "C" int cFEStateMountainRoom_onWidgetMenu(void* self, int menu, unsigned int key)
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
extern "C" void func_001D32E0_MR(void* self) __asm__("func_001D32E0");
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
extern "C" void cFEStateMountainRoom_onWidgetEvent(char* self, char* w, unsigned int msg)
{
    if (w == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        int idx = *(int*)(w + 0x18);
        if (idx == 0) {
            func_001D32E0_MR(self);
            break;
        }
        sFactory_MR* factory = **(sFactory_MR***)(self + 0x10);
        void* nextState = factory->vt[4].fn((char*)factory + factory->vt[4].delta, self, idx - 1);
        cUIStateStack_push_MR(*(char**)(self + 0x10) + 0x18, nextState);
        break;
    }
    case 6: {
        sFactory_MR* factory = **(sFactory_MR***)(self + 0x10);
        void* nextState = factory->vt[5].fn((char*)factory + factory->vt[5].delta, self, *(int*)(w + 0x18));
        if (nextState == 0) {
            break;
        }
        cUIStateStack_push_MR(*(char**)(self + 0x10) + 0x18, nextState);
        break;
    }
    case 1: {
        if (*(void**)(self + 0x54) == 0) {
            break;
        }
        int stringId = 0;
        switch (*(int*)(w + 0x18)) {
        case 0:
            stringId = GetHashValue32(str_kT_HELPViewPass);
            break;
        case 1:
            stringId = GetHashValue32(str_kT_HELPViewTrophies);
            break;
        case 2:
            if (*(int*)(self + 0xB4) > 0) {
                stringId = GetHashValue32(str_kT_HELPViewTrophies);
            } else {
                stringId = GetHashValue32(str_kT_MAPHELPLockedPeak2);
            }
            break;
        case 3:
            if (*(int*)(self + 0xB4) >= 2) {
                stringId = GetHashValue32(str_kT_HELPViewTrophies);
            } else {
                stringId = GetHashValue32(str_kT_MAPHELPLockedPeak3);
            }
            break;
        }
        sText_MR* helpText = *(sText_MR**)(self + 0x54);
        helpText->vt[9].fn((char*)helpText + helpText->vt[9].delta, 1);
        cUIText_setUnicodeStringByID(*(cUIText**)(self + 0x54), stringId);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", cFEStateMountainRoom_onUpdate);
#ifdef SKIP_ASM
extern "C" void func_001D31C0(void* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the mountain room, first finishing its picture load while one is pending.
 *
 * While the async slot at 0xBC is not -1, func_001D31C0() checks the load; once it has loaded, the data is
 * handed to GrMan under the name "pass" and attached to the widget at 0x50, and the slot is released. Then runs
 * the base UI-state update cUIState_onUpdate().
 */
extern "C" void cFEStateMountainRoom_onUpdate(void* self)
{
    if (~*(int*)((char*)self + 0xBC) != 0) {
        func_001D31C0(self);
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3160);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);

extern "C" void func_001D3160(void* self)
{
    if (*(int*)((char*)self + 0xBC) < 0) {
        void* item = *(void**)((char*)self + 0xC0);
        if (item != 0) {
            *(int*)((char*)self + 0xBC) = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, *(int*)((char*)self + 0xB0), *(int*)((char*)item + 4), 10, 1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D31C0);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* mgr, int bank, int i);
extern "C" void* func_0019E2B0(void* mgr, int bank, int i);
extern "C" void func_001D32E0(void* self);
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

// Starts the preview sound of the selected lodge item once its bank entry has loaded.
extern "C" void func_001D31C0(void* vself)
{
    char* self = (char*)vself;
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    if (*(void**)(self + 0x50) == 0) {
        return;
    }
    if (func_0019E238(mgr, *(int*)(self + 0xB0), *(int*)(self + 0xBC)) == 0) {
        return;
    }
    char* fileData = (char*)func_0019E2B0(mgr, *(int*)(self + 0xB0), *(int*)(self + 0xBC));
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_31C0* vt = (sVE_31C0*)ps2GraphicsMan->mVtbl;
    int id = vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, fileData + *(int*)(fileData + 0x14), str_pass, 0, 1, -1);
    char* snd = *(char**)(self + 0x50);
    *(int*)(self + 0xC4) = id;
    *(int*)(snd + 0x78) = id;
    *(int*)(snd + 0x7C) = 0;
    sWidget_31C0* a = *(sWidget_31C0**)(self + 0x48);
    if (isActive_31C0(a)) {
        int both = 0;
        if (isActive_31C0(a)) {
            sWidget_31C0* b = *(sWidget_31C0**)(self + 0x4C);
            if (b != 0) {
                both = (b->flags >> 6) & 1;
            }
        }
        if (!both) {
            func_001D32E0(self);
        }
    }
    func_0019DC20(mgr, *(int*)(self + 0xB0), *(int*)(self + 0xBC));
    *(int*)(self + 0xBC) = -1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D32E0);
#ifdef SKIP_ASM
struct sVEntry_func_001D32E0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D32E0(void* self)
{
    {
        void* obj = *(void**)((char*)self + 0x48);
        sVEntry_func_001D32E0* vt = *(sVEntry_func_001D32E0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
    if (*(int*)((char*)self + 0xC4) >= 0) {
        void* obj = *(void**)((char*)self + 0x4C);
        sVEntry_func_001D32E0* vt = *(sVEntry_func_001D32E0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3340);
#ifdef SKIP_ASM
extern "C" int func_001CED90(int player, int charId, int mountain, int peak, int i);

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

extern "C" void func_001D3340(void* p)
{
    sLodge_3340* self = (sLodge_3340*)p;
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
                unsigned int st = func_001CED90(self->player, self->charId, mountain, peak, i);
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
INCLUDE_ASM("fe/ovstatelodge", func_001D3780);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
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

extern "C" void* func_001D3780(void* self, int a1, int a2, int a3)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 44;
    *(void***)((char*)self + 0x8) = cFEStatePeakRoom_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    sLodge_3780* s = (sLodge_3780*)self;
    s->charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    s->fE0 = a3;
    for (int i = 3; i >= 0; i--) {
        s->slots[i] = 0;
    }
    for (int j = 0; j < 5; j++) {
        s->grid[0][j] = 0;
        s->grid[1][j] = 0;
        s->grid[2][j] = 0;
        s->grid[3][j] = 0;
        s->grid[4][j] = 0;
        s->grid[5][j] = 0;
    }
    s->fD0 = 0;
    return self;
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
extern "C" void cFEStatePeakRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_126peakroom), 0);
    *(void**)((char*)self + 0x40) = screen;
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
extern "C" char* func_00156A90(void* self, int a1, int a2);
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
                char* info = func_00156A90(iface, self->mode, i);
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
extern "C" void cFEStatePeakRoom_updateHelpText(void* self, int id);
extern "C" void func_001D3F80(void* self, int id);
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
 * slots. Event 1 updates the help text and calls func_001D3F80().
 *
 * @param item  the menu entry (its trophy index at +0x18), or 0
 * @param key  widget event number (On Tour Event)
 */
extern "C" void cFEStatePeakRoom_onWidgetEvent(void* self, void* item, unsigned int key)
{
    if (item == 0) {
        return;
    }
    switch (key) {
    case 5:
        if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), *(signed char*)((char*)self + 0x44),
                          *(int*)((char*)self + 0xDC), *(int*)((char*)self + 0xE0), *(int*)((char*)item + 0x18)) != 0) {
            void* obj = **(void***)((char*)self + 0x10);
            sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
            void* nextState = vt[4].fn((char*)obj + vt[4].delta, self, (*(int*)((char*)self + 0xE0) << 16) | *(int*)((char*)item + 0x18));
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    case 6: {
        int id = (*(int*)((char*)self + 0xE0) << 16) | *(int*)((char*)item + 0x18);
        for (int i = 0; i < 4; i++) {
            sItem_001D3C80* listItem = ((sItem_001D3C80**)((char*)self + 0x48))[i];
            if (listItem != 0) {
                listItem->vt[9].fn((char*)listItem + listItem->vt[9].delta, 0);
            }
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntA_001D3C80* vt = *(sVEntA_001D3C80**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, id);
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    case 1:
        cFEStatePeakRoom_updateHelpText(self, *(int*)((char*)item + 0x18));
        func_001D3F80(self, *(int*)((char*)item + 0x18));
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
extern "C" int cFEStatePeakRoom_onWidgetMenu(void* self, void* item, unsigned int key, int id)
{
    switch (key) {
    default:
        break;
    case 8:
    case 9:
        return 0x100;
    case 6: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* entry = cList_getByIndex((char*)item + 0x74, id);
        if (cBERewardInterface_isTrophyOwned(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xDC),
                          *(int*)((char*)self + 0xE0), *(int*)((char*)entry + 0x18)) == 0) {
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
extern "C" void cFEStatePeakRoom_updateHelpText(void* self, int index)
{
    if (*(void**)((char*)self + 0xD0) != 0) {
        if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), *(signed char*)((char*)self + 0x44),
                          *(int*)((char*)self + 0xDC), *(int*)((char*)self + 0xE0), index) != 0) {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0xD0), GetHashValue32(str_kT_HELPSelectGoal));
        } else {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0xD0), GetHashValue32(str_kT_HELP_LockedGoal));
        }
        (*(cPeakWidget_3EC8**)((char*)self + 0xD0))->show(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatelodge", func_001D3F80);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv_3F80(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001CE6F0(int mountain);
extern "C" int func_001CED90(int player, int charId, int mountain, int peak, int i);
extern "C" void setChallengeName(void* text, int mountain, int peak, int i);
extern "C" char* func_00156AE0(void* iface, int peak, int ch);
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

extern "C" void func_001D3F80(void* vself, int peak)
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
        int ch = func_001CED90(self->player, self->charId, self->mountain, peak, i);
        setChallengeName(name[i], self->mountain, peak, i);
        name[i]->show(1);
        bar[i]->show(1);
        box[i]->show(1);
        if (ch != -1) {
            char* trophyRec = func_00156AE0(iface, peak, ch);
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
INCLUDE_ASM("fe/ovstatelodge", func_001D4268);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_001D4918(void* self);
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

extern "C" void* func_001D4268(void* self, int a1, int a2, int a3, int a4)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 45;
    *(void***)((char*)self + 0x8) = cFEStateTrophyRoom_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    sTrophy_4268* s = (sTrophy_4268*)self;
    s->f48 = 0;
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    s->charID = cBENewPlayerInterface_getPlayerCharID(pi, *(signed char*)((char*)self + 0x44));
    sVE_4268* vt = *(sVE_4268**)((char*)pi + 0xC);
    vt[2].fn((char*)pi + vt[2].delta);
    s->f54 = a3;
    s->bank = a4;
    s->count = 0;
    for (int i = 0; i < 5; i++) {
        s->snd[i] = -1;
        s->tex[i] = -1;
    }
    s->f9C = -1;
    func_001D4918(self);
    return self;
}
#endif

