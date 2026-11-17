#include "common.h"
#include "online/lobbyman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnetwork.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_127trophyroom[];

/**
 * Opens the trophy-room screen ("127trophyroom"), keeps it at 0x40 and plays its frame 0.
 */
extern "C" void cFEStateTrophyRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_127trophyroom), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

extern "C" void* cUIState_onGainFocus(void* self);

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the trophy-room state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateTrophyRoom_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" char* cBERewardInterface_getTrophy(void* self, int a1, int a2);
extern "C" int func_001CE6F0(int a, int b);
extern "C" void cFEStateTrophyRoom_updateBigArt(void* self, int index);
extern "C" void setChallengeName(void* text, int a1, int a2, int idx);
extern char str_bitmap[];
extern char str_stats[];
extern char str_screentitle[];
extern char str_kT_TITLEEarningsTrophy[];
extern char str_kT_TITLEExploreTrophy[];
extern char str_kT_TITLERaceTrophy[];
extern char str_kT_TITLEFSTrophy[];
extern char* D_00441600[];
extern char* D_00441618[];

struct sVEntry_43F0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sTrophy_43F0 {
    char pad0[0x48];
    void* f48;          // 0x48
    void* f4C;          // 0x4C
    char pad50[4];
    int f54;            // 0x54
    int f58;            // 0x58
    char pad5C[0x40];
    int f9C;            // 0x9C
};

static inline void vcall_43F0(void* o, int slot, int arg)
{
    sVEntry_43F0* vt = *(sVEntry_43F0**)((char*)o + 8);
    vt[slot].fn((char*)o + vt[slot].delta, arg);
}

/**
 * Sets up the trophy room's widgets: the big icon, the stats text, the title and the trophy and event rows.
 *
 * "bitmap" is kept at 0x48 (On Tour mpBigIcon) and shows the selection (cFEStateTrophyRoom_updateBigArt, probably
 * updateBigArt); "stats" is kept at 0x4C and hidden. "screentitle" names the trophy type (0x58): 0 race, 1
 * freestyle, 2 explore, 3 earnings. Rows "trophy" and "event_0" to "event_4" (misc data 0-5) and "0text
 * blocks" to "5text blocks" are hidden and made static past the peak's event count (func_001CE6F0); "trophy"
 * shows the name from the reward interface and each event row its challenge name.
 *
 * @param w  the widget just created (its hash name is at +0x38)
 */
extern "C" void cFEStateTrophyRoom_onWidgetCreate(sTrophy_43F0* self, void* w)
{
    int hashName = *(int*)((char*)w + 0x38);
    if (hashName == GetHashValue32(str_bitmap)) {
        self->f48 = w;
        cFEStateTrophyRoom_updateBigArt(self, self->f9C);
        return;
    }
    if (hashName == GetHashValue32(str_stats)) {
        self->f4C = w;
        vcall_43F0(w, 9, 0);
        return;
    }
    if (hashName == GetHashValue32(str_screentitle)) {
        int id = 0;
        switch (self->f58) {
        case 3:
            id = GetHashValue32(str_kT_TITLEEarningsTrophy);
            break;
        case 2:
            id = GetHashValue32(str_kT_TITLEExploreTrophy);
            break;
        case 0:
            id = GetHashValue32(str_kT_TITLERaceTrophy);
            break;
        case 1:
            id = GetHashValue32(str_kT_TITLEFSTrophy);
            break;
        }
        cUIText_setUnicodeStringByID((cUIText*)w, id);
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (hashName == GetHashValue32(D_00441600[i])) {
            int n = func_001CE6F0(self->f54, self->f58);
            *(int*)((char*)w + 0x18) = i;
            if (n < i) {
                vcall_43F0(w, 9, 0);
                vcall_43F0(w, 8, 1);
            } else if (i == 0) {
                cUIText_setAsciiString((cUIText*)w, *(char**)cBERewardInterface_getTrophy(cBE_getInterface_Fv(cBE_getBE(), 13), self->f54, self->f58));
            } else {
                setChallengeName(w, self->f54, self->f58, i - 1);
            }
        } else if (hashName == GetHashValue32(D_00441618[i])) {
            if (func_001CE6F0(self->f54, self->f58) < i) {
                vcall_43F0(w, 9, 0);
                vcall_43F0(w, 8, 1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the trophy-room menu's events: 0x100 for next, option and misc (6, 8, 9), else 0x101.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param Event  the menu event
 * @return 0x100 for events 6, 8 and 9, otherwise 0x101
 */
extern "C" int cFEStateTrophyRoom_onWidgetMenu(void* self, int pMenu, unsigned int Event)
{
    switch (Event) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001D4698 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void cFEStateTrophyRoom_updateBigArt(void* self, int index);
extern "C" void cFEStateTrophyRoom_updateStats(void* self, int index);

class cTrophyWidget_4698 {
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
 * Updates the big art and stats when a trophy row gets focus, and leaves the trophy room on back.
 *
 * Event 1 (a row focused) stores the row's misc data as the selection (0x9C) and runs cFEStateTrophyRoom_updateBigArt() and
 * cFEStateTrophyRoom_updateStats() (probably On Tour updateBigArt and updateStats). Event 6 (back) hides the big icon and
 * pushes getPrevState's state for the peak (0x54).
 *
 * @param item  the widget that sent the event; nothing happens when null
 * @param msg  the widget event (1 focused, 6 back)
 */
extern "C" void cFEStateTrophyRoom_onWidgetEvent(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        cTrophyWidget_4698* w = *(cTrophyWidget_4698**)((char*)self + 0x48);
        if (w != 0) {
            w->show(0);
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4698* vt = *(sVEntry001D4698**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)self + 0x54));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    case 1:
        *(int*)((char*)self + 0x9C) = *(int*)((char*)item + 0x18);
        cFEStateTrophyRoom_updateBigArt(self, *(int*)((char*)item + 0x18));
        cFEStateTrophyRoom_updateStats(self, *(int*)((char*)item + 0x18));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStateTrophyRoom_updateTexturesLoad(void* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the trophy room, first finishing any of its pending trophy picture loads.
 *
 * While the pending count at 0x5C is positive, cFEStateTrophyRoom_updateTexturesLoad() hands each of the five slots that has finished
 * loading to GrMan under the name "trph", releases it and decrements the count. Then runs the base UI-state
 * update cUIState_onUpdate().
 */
extern "C" void cFEStateTrophyRoom_onUpdate(void* self)
{
    if (*(int*)((char*)self + 0x5C) > 0) {
        cFEStateTrophyRoom_updateTexturesLoad(self);
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_updateBigArt);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" int func_001CED90(int rider, int a, int b, int c, int index);

class cTrophyWidget_47A0 {
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
 * Shows the trophy room's big picture (+0x48, a cUIIcon) for a list row, or hides it.
 *
 * Row 0 is the trophy: its texture (+0x60[0]) is used when the character owns it (cBERewardInterface_isTrophyOwned on
 * reward interface 13 with mUserNum, character +0x50, peak +0x54 and trophy +0x58). Rows 1-4 are challenges:
 * func_001CED90 gives the medal earned in challenge row - 1 (-1 for none) and the texture is +0x60[medal + 1]. With a
 * texture the icon is shown (setVisible, vtable slot 9) and gets mTextureID = it and mpTextureElem = 0; a negative @p
 * index or no texture hides it.
 *
 * @param index  the selected row (0 trophy, 1-4 challenges), or negative to hide
 */
extern "C" void cFEStateTrophyRoom_updateBigArt(void* self, int index)
{
    int textureId;
    if (index < 0) {
        goto hide;
    }
    textureId = -1;
    {
        if (index == 0) {
            if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), *(signed char*)((char*)self + 0x44),
                              *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                              *(int*)((char*)self + 0x58)) != 0) {
                textureId = *(int*)((char*)self + 0x60);
            }
        } else {
            int medal = func_001CED90(*(signed char*)((char*)self + 0x44),
                                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                                  *(int*)((char*)self + 0x58), index - 1);
            if (medal != -1) {
                textureId = *(int*)((char*)self + ((medal + 1) << 2) + 0x60);
            }
        }
    }
    if (textureId < 0) {
    hide:
        (*(cTrophyWidget_47A0**)((char*)self + 0x48))->show(0);
    } else {
        (*(cTrophyWidget_47A0**)((char*)self + 0x48))->show(1);
        char* w = *(char**)((char*)self + 0x48);
        *(int*)(w + 0x78) = textureId;
        *(int*)(w + 0x7C) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_updateStats);
#ifdef SKIP_ASM
extern "C" void setChallengeStats(void* widget, signed char rider, int a2, int a3, int a4, int index);

// Real virtual class so g++ emits the vcall itself (vptr at +8, after 8 bytes of data).
class cTrophyWidget_4890 {
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
 * Shows the trophy room's stats text (+0x4C) for a challenge row, or hides it for row 0 (the trophy).
 *
 * Rows 1 and up fill it with setChallengeStats (mUserNum, character +0x50, peak +0x54, trophy +0x58, row - 1). Nothing
 * happens without the text.
 *
 * @param index  the selected row
 */
extern "C" void cFEStateTrophyRoom_updateStats(void* self, int index)
{
    cTrophyWidget_4890* widget = *(cTrophyWidget_4890**)((char*)self + 0x4C);
    if (widget != 0) {
        if (index == 0) {
            widget->show(0);
        } else {
            widget->show(1);
            setChallengeStats(*(void**)((char*)self + 0x4C), *(signed char*)((char*)self + 0x44),
                              *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x54),
                              *(int*)((char*)self + 0x58), index - 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_loadTextures);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBERewardInterface_getTrophy(void* self, int a1, int a2);
extern "C" char* cBERewardInterface_getMedal(void* self, int a1, int a2);
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);

struct sTrophyRoom_4918 {
    char pad[0x50];
    int bank;
    int f54;
    int f58;
    int count;
    char pad60[0x14];
    int ids[5];
};

/**
 * Requests the trophy room's five images from the FE file cache.
 *
 * On the cache at RealFE + 0x1A70, bank +0x50 (the character), func_0019DA10 (with 10, 1, 0) loads the trophy's image
 * (the name at +4 of cBERewardInterface_getTrophy for peak +0x54 and trophy +0x58) into slot index +0x74[0], and the
 * image of each of the trophy's four medals that has a name (cBERewardInterface_getMedal) into +0x74[1-4]. +0x5C counts
 * the requests. Called by the trophy room's constructor cFEStateTrophyRoom_cFEStateTrophyRoom.
 */
extern "C" void cFEStateTrophyRoom_loadTextures(sTrophyRoom_4918* self)
{
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* snd = (char*)SSXApp->RealFE + 0x1A70;
    self->ids[0] = func_0019DA10(snd, self->bank, *(int*)(cBERewardInterface_getTrophy(pi, self->f54, self->f58) + 4), 10, 1, 0);
    self->count = 1;
    for (int i = 0; i < 4; i++) {
        int id = *(int*)(cBERewardInterface_getMedal(pi, self->f58, i) + 4);
        if (id != 0) {
            self->ids[i + 1] = func_0019DA10(snd, self->bank, id, 10, 1, 0);
            self->count++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_updateTexturesLoad);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* self, int bank, int i);
extern "C" char* func_0019E2B0(void* self, int bank, int i);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void cFEStateTrophyRoom_updateBigArt(void* self, int index);
extern char str_trph[];

struct cGame_4A20 {
    char pad[0x10D8];
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
    virtual int playSound(void* data, char* name, int a3, int a4, int a5);
};

struct sSelf_4A20 {
    char pad[0x50];
    int bank;
    char pad54[0x8];
    int count;
    int snd[5];
    int tex[5];
    char pad88[0x14];
    int f9C;
};

/**
 * Turns each finished trophy-room image load into a texture and redraws the big picture.
 *
 * For each of the five cache slots (+0x74) still pending whose file has loaded (func_0019E238 on RealFE + 0x1A70, bank
 * +0x50) it creates the texture "trph" from the file data (func_0019E2B0's pointer plus the offset at its +0x14)
 * through GrMan's vtable slot 46 (func_0037C8C0 in cPS2GraphicsMan_vtbl) into +0x60[i], releases the file
 * (func_0019DC20), sets the slot to -1, decrements the pending count (+0x5C) and redraws the selected row's picture
 * (cFEStateTrophyRoom_updateBigArt with +0x9C). Called from cFEStateTrophyRoom_onUpdate.
 *
 * @param trophyRoom  the trophy room state
 */
extern "C" void cFEStateTrophyRoom_updateTexturesLoad(void* trophyRoom)
{
    sSelf_4A20* self = (sSelf_4A20*)trophyRoom;
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->tex[i] >= 0 && func_0019E238(mgr, self->bank, self->tex[i]) != 0) {
            char* fileData = func_0019E2B0(mgr, self->bank, self->tex[i]);
            self->snd[i] = ((cGame_4A20*)GrMan)->playSound(fileData + *(int*)(fileData + 0x14), str_trph, 0, 1, -1);
            func_0019DC20(mgr, self->bank, self->tex[i]);
            self->tex[i] = -1;
            self->count--;
            cFEStateTrophyRoom_updateBigArt(self, self->f9C);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_purgeTextures);
#ifdef SKIP_ASM
struct cGame001D4B20 {
    char pad[0x10D8];
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
    virtual void v50(int id);
};

extern "C" void func_0019DC20(void* self, int bank, int i);
struct sSelf001D4B20 {
    char pad[0x50];
    int bank;
    char pad54[0xC];
    int snd[5];
    int tex[5];
};

/**
 * Releases the trophy room's five images.
 *
 * Each texture (+0x60) is removed with GrMan's vtable slot 50 (cPS2GraphicsMan_RemoveTexture) and each cached file
 * (+0x74) released with func_0019DC20 (RealFE + 0x1A70, bank +0x50); both are set to -1. Called by
 * cFEStateTrophyRoom__cFEStateTrophyRoom.
 */
extern "C" void cFEStateTrophyRoom_purgeTextures(sSelf001D4B20* self)
{
    char* mgr = (char*)SSXApp->RealFE + 0x1A70;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->snd[i] >= 0) {
            ((cGame001D4B20*)GrMan)->v50(self->snd[i]);
        }
        self->snd[i] = -1;
        if (self->tex[i] >= 0) {
            func_0019DC20(mgr, self->bank, self->tex[i]);
        }
        self->tex[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_cFEStateRewardsRoom);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern void* cFEStateRewardsRoom_vtbl[];

/**
 * Constructs the rewards-room state for a user (allocated as "cFEStateRewardsRoom", 80 bytes).
 *
 * Runs cUIState's constructor (func_0039E2A0; @p engine reaches it untouched in the second argument register), stores
 * mHashName 46 and cFEStateRewardsRoom_vtbl, sets mUserNum to @p idx and mCtrlBits to that user's controller bits
 * (cFECustom_getPlayerCtrlMask on the engine's front end), +0x4C to @p inStore (On Tour mInStore) and +0x48 to the user's character
 * (cBENewPlayerInterface_getPlayerCharID; On Tour mCharID).
 *
 * @param engine  the UI engine (only passed on to cUIState's constructor)
 * @param idx  the user number
 * @param inStore  the in-store flag; the front end's factories pass 1
 * @return @p self
 */
extern "C" void* cFEStateRewardsRoom_cFEStateRewardsRoom(void* self, int engine, signed char idx, int inStore)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 46;
    *(void***)((char*)self + 0x8) = cFEStateRewardsRoom_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = cFECustom_getPlayerCtrlMask(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = inStore;
    *(int*)((char*)self + 0x48) = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern char str_128rewardsroom[];

/**
 * Opens the rewards-room screen ("128rewardsroom") and switches the front-end sound-bank mode to 6.
 */
extern "C" void cFEStateRewardsRoom_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_128rewardsroom), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0028F140(cAudioInterface_bxAudio(), 6);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
extern "C" void cFEStateRewardsRoom_setRewardsWidgetText(void* self, int index, cUIText* text);
extern char str_art[];
extern char str_video[];
extern char str_poster[];
extern char str_toys[];
extern char str_trading_cards[];
extern char str_cheat_character[];
extern char str_txtmdl_0[];
extern char str_txtmdl_1[];
extern char str_txtmdl_2[];
extern char str_txtmdl_3[];
extern char str_txtmdl_4[];
extern char str_txtmdl_5[];

/**
 * Tags the rewards room's category buttons with their reward type and fills their text blocks.
 *
 * "poster", "trading cards", "art", "video", "toys" and "cheat character" get misc data 3 to 8 in that order;
 * the texts "txtmdl_0" to "txtmdl_5" are filled by cFEStateRewardsRoom_setRewardsWidgetText() (probably On Tour setRewardsWidgetText)
 * for types 5, 3, 7, 4, 8 and 6.
 *
 * @param w  the widget just created (its hash name is at +0x38)
 */
extern "C" void cFEStateRewardsRoom_onWidgetCreate(void* self, void* w)
{
    int id = *(int*)((char*)w + 0x38);
    if (id == GetHashValue32(str_art)) {
        *(int*)((char*)w + 0x18) = 5;
    } else if (id == GetHashValue32(str_video)) {
        *(int*)((char*)w + 0x18) = 6;
    } else if (id == GetHashValue32(str_poster)) {
        *(int*)((char*)w + 0x18) = 3;
    } else if (id == GetHashValue32(str_toys)) {
        *(int*)((char*)w + 0x18) = 7;
    } else if (id == GetHashValue32(str_trading_cards)) {
        *(int*)((char*)w + 0x18) = 4;
    } else if (id == GetHashValue32(str_cheat_character)) {
        *(int*)((char*)w + 0x18) = 8;
    } else if (id == GetHashValue32(str_txtmdl_0)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 5, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_1)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 3, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_2)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 7, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_3)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 4, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_4)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 8, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_5)) {
        cFEStateRewardsRoom_setRewardsWidgetText(self, 6, (cUIText*)w);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001D4EA8 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * Opens the chosen reward category's screen on accept and goes back on back.
 *
 * Event 5 pushes the custom interface's getNextState for the widget's misc data (the reward type) without a
 * null check; event 6 pushes getPrevState's state when there is one.
 *
 * @param item  the widget that sent the event; nothing happens when null
 * @param msg  the widget event (5 accept, 6 back)
 */
extern "C" void cFEStateRewardsRoom_onWidgetEvent(void* self, void* item, unsigned int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 0:
        break;
    case 5: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4EA8* vt = *(sVEntry001D4EA8**)((char*)obj + 4);
        void* nextState = vt[4].fn((char*)obj + vt[4].delta, self, *(int*)((char*)item + 0x18));
        cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        break;
    }
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001D4EA8* vt = *(sVEntry001D4EA8**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the rewards-room menu's events: 0x100 for the option and misc buttons (8, 9), else 0x101.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateRewardsRoom_onWidgetMenu(void* self, int pMenu, unsigned int key)
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
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_setRewardsWidgetText);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_00157210(void* iface, int index);
extern "C" int func_00157518(void* iface, int rider, int a, int index);
extern char str_pct_d_pct_d[];

/**
 * Sets a rewards-room text to "<got> / <total>" for one kind of reward.
 *
 * The total comes from func_00157210 and the count that the user's character (+0x48) has from func_00157518, both on
 * reward interface 13.
 *
 * @param index  the reward kind
 * @param text  the text to set
 */
extern "C" void cFEStateRewardsRoom_setRewardsWidgetText(void* self, int index, cUIText* text)
{
    char buf[256];
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    int total = func_00157210(iface, index);
    int got = func_00157518(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x48), index);
    sprintf(buf, str_pct_d_pct_d, got, total);
    cUIText_setAsciiString(text, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_cFEStateRelogin);
#ifdef SKIP_ASM
extern "C" void* cFEStateLogin_cFEStateLogin(void* self);
extern void* cFEStateRelogin_vtbl[];

extern "C" void* cFEStateRelogin_cFEStateRelogin(void* self)
{
    cFEStateLogin_cFEStateLogin(self);
    *(void***)((char*)self + 0x8) = cFEStateRelogin_vtbl;
    *(int*)((char*)self + 0xC) = 52;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_248lobby_reconnect[];

extern "C" void cFEStateRelogin_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_248lobby_reconnect), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_TextReconnecting[];
extern char str_kT_POPUPCMNReconnecting[];

extern "C" void cFEStateRelogin_onWidgetCreate(void* self, cUIText* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(str_TextReconnecting)) {
        cUIText_setUnicodeStringByID(item, GetHashValue32(str_kT_POPUPCMNReconnecting));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5138);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001A8A40(void* self, void* obj);
extern "C" int func_001BF228(void* self, int msg);
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(void* mem, int a1);
extern "C" void func_0025C610(void* mgr);
extern "C" void* func_0025CD50(void*, int);
extern "C" void func_0025FD50(void* mgr, int a1, int a2, int a3, char* a4, char* a5);
extern char str_cFEStateOnlineMainMenu[];

/**
 * Handles lobby events on the reconnect screen: logs in again, re-selects the persona and returns to the online main
 * menu.
 *
 * 201 sends the login request (func_0025C610 on LobbyMan_mLobbyMan); 212 selects the current persona again
 * (func_0025CD50 with mPersona); 257 pushes a new cFEStateOnlineMainMenu (1744 bytes) through func_001A8A40; 273 calls
 * func_0025FD50 with fields of cBENetworkInterface_networkOpt (+0x4, whether +0x8 is 0, whether it is not, and the
 * strings at +0x330 and +0x3DC). Other events go to cFEStateLogin's handler func_001BF228. Lobby event slot (+0xFC) of
 * cFEStateRelogin_vtbl.
 *
 * @param msg  the lobby event id
 * @return 0 for the handled events, else func_001BF228's result
 */
extern "C" int func_001D5138(void* self, int msg)
{
    switch (msg) {
    case 201:
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    case 212:
        func_0025CD50(LobbyMan_mLobbyMan, ((int)LobbyMan_mLobbyMan->mPersona));
        break;
    case 257:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), *(int*)((char*)self + 0x10)));
        break;
    case 273: {
        cBE_getInterface_Fv(SSXApp->BE, 7);
        char* netOpt = ((char*)&cBENetworkInterface_networkOpt);
        func_0025FD50(LobbyMan_mLobbyMan, *(int*)(netOpt + 4), *(int*)(netOpt + 8) == 0, *(int*)(netOpt + 8) != 0, netOpt + 0x330, netOpt + 0x3DC);
        break;
    }
    default:
        return func_001BF228(self, msg);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_onPopupEvent);
#ifdef SKIP_ASM
extern "C" int func_001A9668(void* self, void* a1, int a2);
extern "C" int cFEStateLogin_onPopupEvent(void* self, void* a1, int a2);

extern "C" int cFEStateRelogin_onPopupEvent(void* self, void* popup, int event)
{
    int result;
    if (*(int*)((char*)popup + 0x18) == 258) {
        result = func_001A9668(self, popup, event);
    } else {
        result = cFEStateLogin_onPopupEvent(self, popup, event);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5280);
#ifdef SKIP_ASM
extern "C" int cFEStateNet_onPopupEvent(void* self, void* a1, int a2);
extern "C" int func_0039F698(void* p);
extern "C" void* cUIStateStack_getCurrentState(void* self);
extern "C" void* func_002591B8();
extern "C" void func_002636F0(void* p, int a1);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateNetConfig_cFEStateNetConfig(void* mem, void* owner);
extern "C" void func_001A8A40(void* self, void* obj);
extern char str_cFEStateNetConfig[];

/**
 * Handles the reconnect screen's popup: on its first option or back it gives up and opens the network configuration.
 *
 * Every event goes to cFEStateNet_onPopupEvent first. On 0xF (first option) or 20 (back) it then runs func_0039F698 on
 * the state stack (mpEngine + 0x18), sets bit 0x80 in the current state's mAttrState (+0x1C,
 * cUIStateStack_getCurrentState), logs out of the online services (func_002636F0 on func_002591B8(), no error) and
 * pushes a new cFEStateNetConfig (2332 bytes) through func_001A8A40. Popup handler slot (+0x10C) of
 * cFEStateRelogin_vtbl.
 *
 * @param item  the popup that sent the event
 * @param msg  the popup event
 */
extern "C" void func_001D5280(void* self, void* item, int msg)
{
    if (msg == 0xF || msg == 20) {
        cFEStateNet_onPopupEvent(self, item, msg);
        func_0039F698((char*)*(void**)((char*)self + 0x10) + 0x18);
        void* state = cUIStateStack_getCurrentState((char*)*(void**)((char*)self + 0x10) + 0x18);
        *(int*)((char*)state + 0x1C) |= 0x80;
        func_002636F0(func_002591B8(), 0);
        func_001A8A40(self, cFEStateNetConfig_cFEStateNetConfig(cMemMan_alloc(2332, str_cFEStateNetConfig, 0, 0), *(void**)((char*)self + 0x10)));
    } else {
        cFEStateNet_onPopupEvent(self, item, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", func_001D5330);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void cFEMemCard_initFEMemCard(void* self, int a1);
extern void* D_00474E08[];
extern void* D_0046C980[];
extern unsigned short D_004A1390[];

/**
 * Constructs a memory-card overlay state with an overlay id: cFEMemCard_cFEMemCard's work, but with mOVID (+0x98) = @p
 * a3.
 *
 * Runs cUIState's constructor (func_0039E2A0; @p engine reaches it untouched in the second argument register as the
 * engine), the inlined cOVState part (vtable D_00474E08; mDoneTransition, mAllowPauseMenu and mRemovePDA 0; mszChatBuff
 * " " from D_004A1390), stores cFEMemCard's vtable D_0046C980 and runs cFEMemCard_initFEMemCard with @p memCardType. cOVState_SAVEREPLAY_cOVState_SAVEREPLAY and
 * cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS (fe/ovstatepause) call it with types 1 and 2.
 *
 * @param engine  the UI engine (only passed on to cUIState's constructor)
 * @param memCardType  the memory-card screen type, passed to cFEMemCard_initFEMemCard
 * @param overlayId  the overlay id stored in mOVID
 * @return @p self
 */
extern "C" void* func_001D5330(void* self, int engine, int memCardType, int overlayId)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0x98) = overlayId;
    *(void***)((char*)self + 0x8) = D_00474E08;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(unsigned short*)((char*)self + 0x58) = D_004A1390[0];
    *(void***)((char*)self + 0x8) = D_0046C980;
    cFEMemCard_initFEMemCard(self, memCardType);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_cFEMemCard);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void cFEMemCard_initFEMemCard(void* self, int a1);
extern void* D_00474E08[];
extern void* D_0046C980[];
extern unsigned short D_004A1390[];

/**
 * Constructs the memory card screen base used by the profile, autosave and save screens.
 *
 * Runs the cUIState constructor, installs the cOVState vtable (D_00474E08) and then this class's (D_0046C980),
 * clears +0x48, +0x4C, +0x54 and +0x98, copies the empty string D_004A1390 into +0x58, and runs cFEMemCard_initFEMemCard
 * (On Tour initFEMemCard) with @p iType. The engine argument is not used.
 *
 * @param pEngine  the UI engine (unused)
 * @param iType  what the screen is for (On Tour iType)
 * @return @p self
 */
extern "C" void* cFEMemCard_cFEMemCard(void* self, int pEngine, int iType)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0x98) = 0;
    *(void***)((char*)self + 0x8) = D_00474E08;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(unsigned short*)((char*)self + 0x58) = D_004A1390[0];
    *(void***)((char*)self + 0x8) = D_0046C980;
    cFEMemCard_initFEMemCard(self, iType);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard__cFEMemCard);
#ifdef SKIP_ASM
extern void* D_0046C980[];
extern void* D_00474E08[];
extern void* cFEMemCard_mInstance;
extern "C" void func_0039E390(void* self, int flags);

/**
 * Destroys a memory-card state.
 *
 * Stores cFEMemCard's vtable D_0046C980, clears cFEMemCard_mInstance, stores cOVState's vtable D_00474E08 and runs
 * cUIState's destructor (func_0039E390) with @p flags. Destructor slot (+0xC) of D_0046C980; the destructors of
 * cFEStateMemCard, cOVState_PROFILE, cOVState_AUTOSAVE, cFEStateProfileLoad and cFEStateOptionsDeviceSelect end in it.
 *
 * @param flags  the g++ delete flags, passed on to func_0039E390
 */
extern "C" void cFEMemCard__cFEMemCard(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_0046C980;
    cFEMemCard_mInstance = 0;
    *(void***)((char*)self + 8) = D_00474E08;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the memory-card screens' menu events: 0x100 for events 8 and 9, otherwise 0x101.
 *
 * onWidgetMenu slot (+0xA4) of cFEMemCard's vtable D_0046C980, also used by cOVState_AUTOSAVE_vtbl, D_004722B8 and
 * D_00472440.
 *
 * @param menu  the menu that asks (not used)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
extern "C" int cFEMemCard_onWidgetMenu(void* self, int menu, unsigned int key)
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
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_initFEMemCard);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
// PORT: func_0023CC58__FPv really takes a second argument.
void func_0023CC58_2(void* app, int n) __asm__("func_0023CC58__FPv");
extern "C" void func_0023CAA0(void* app, int n);
extern "C" void func_0023C8F0(void* app, int mode);
extern "C" void func_0023D5A8(void* app);
extern "C" void func_00241D40(void* app, int a, char* buf, int n);
extern "C" void func_00241AA0(char* buf, int n);
extern "C" void func_002C2540(char* buf, char* s);
extern "C" void sprintf1(void* dst, char* a, char* b, char* c);
extern void* cFEMemCard_mInstance;
extern char str_pct_s_pct_s_c[];
// PORT: callback function addresses taken through asm-label views (their real prototypes vary).
extern char cFEMemCard_writeStart_a5488[] __asm__("cFEMemCard_writeStart");
extern char cFEMemCard_callbackWriteUpdate_a5488[] __asm__("cFEMemCard_callbackWriteUpdate");
extern char cFEMemCard_writeDone_a5488[] __asm__("cFEMemCard_writeDone__FPv");
extern char cFEMemCard_callbackSaveSuccess_a5488[] __asm__("cFEMemCard_callbackSaveSuccess");
extern char cFEMemCard_callbackConfirmSaveFailed_a5488[] __asm__("cFEMemCard_callbackConfirmSaveFailed");
extern char cFEMemCard_readStart_a5488[] __asm__("cFEMemCard_readStart");
extern char cFEMemCard_callbackReadUpdate_a5488[] __asm__("cFEMemCard_callbackReadUpdate");
extern char cFEMemCard_readDone_a5488[] __asm__("cFEMemCard_readDone");
extern char cFEMemCard_callbackConfirmLoadFailed_a5488[] __asm__("cFEMemCard_callbackConfirmLoadFailed");
extern char cFEMemCard_callbackReadSuccess_a5488[] __asm__("cFEMemCard_callbackReadSuccess");
extern char cFEMemCard_callbackDeleteDone_a5488[] __asm__("cFEMemCard_callbackDeleteDone");
extern char cFEMemCard_callbackDeleteFailed_a5488[] __asm__("cFEMemCard_callbackDeleteFailed");
extern char cFEMemCard_callbackFileNotFound_a5488[] __asm__("cFEMemCard_callbackFileNotFound");
extern char cFEMemCard_callbackConfirmNoCards_a5488[] __asm__("cFEMemCard_callbackConfirmNoCards");
extern char cFEMemCard_callbackFileCorrupt_a5488[] __asm__("cFEMemCard_callbackFileCorrupt");
extern char cFEMemCard_callbackConfirmOverwrite_a5488[] __asm__("cFEMemCard_callbackConfirmOverwrite");
extern char cFEMemCard_callbackConfirmFormat_a5488[] __asm__("cFEMemCard_callbackConfirmFormat");
extern char cFEMemCard_callbackConfirmFormatFail_a5488[] __asm__("cFEMemCard_callbackConfirmFormatFail");
extern char cFEMemCard_callbackConfirmFormatDone_a5488[] __asm__("cFEMemCard_callbackConfirmFormatDone");
extern char cFEMemCard_callbackConfirmDelete_a5488[] __asm__("cFEMemCard_callbackConfirmDelete");
extern char cFEMemCard_callbackConfirmInvalidCard_a5488[] __asm__("cFEMemCard_callbackConfirmInvalidCard");
extern char cFEMemCard_callbackConfirmNoFreeBlocks_a5488[] __asm__("cFEMemCard_callbackConfirmNoFreeBlocks");
extern char cFEMemCard_callbackConfirmSaveTooBig_a5488[] __asm__("cFEMemCard_callbackConfirmSaveTooBig");
extern char cFEMemCard_isSavePressed_a5488[] __asm__("cFEMemCard_isSavePressed");
extern char cFEMemCard_isDeletePressed_a5488[] __asm__("cFEMemCard_isDeletePressed");
extern char cFEMemCard_isCancelPressed_a5488[] __asm__("cFEMemCard_isCancelPressed");
extern char cFEMemCard_isOWPressed_a5488[] __asm__("cFEMemCard_isOWPressed");
extern char cFEMemCard_isDeviceSelected_a5488[] __asm__("cFEMemCard_isDeviceSelected");
extern char cFEMemCard_isLoadPressed_a5488[] __asm__("cFEMemCard_isLoadPressed");
extern char cFEMemCard_isNextMCPressed_a5488[] __asm__("cFEMemCard_isNextMCPressed");
extern char cFEMemCard_isPrevMCPressed_a5488[] __asm__("cFEMemCard_isPrevMCPressed");
extern char cFEMemCard_callbackIsRetryPressed_a5488[] __asm__("cFEMemCard_callbackIsRetryPressed");
extern char cFEMemCard_isFormatPressed_a5488[] __asm__("cFEMemCard_isFormatPressed");
extern char cFEMemCard_callbackSaveFileCorrupt_a5488[] __asm__("cFEMemCard_callbackSaveFileCorrupt");
extern char cFEMemCard_callbackCardsChecked_a5488[] __asm__("cFEMemCard_callbackCardsChecked");
extern char cFEMemCard_callbackFileChecked_a5488[] __asm__("cFEMemCard_callbackFileChecked");
extern char cFEMemCard_callbackDisplayNoFiles_a5488[] __asm__("cFEMemCard_callbackDisplayNoFiles");
extern char cFEMemCard_callbackDisplayNoCards_a5488[] __asm__("cFEMemCard_callbackDisplayNoCards");
extern char cFEMemCard_callbackFileExists_a5488[] __asm__("cFEMemCard_callbackFileExists");
extern char cFEMemCard_callbackCardChanged_a5488[] __asm__("cFEMemCard_callbackCardChanged");

/**
 * Sets up a memory-card screen: makes it the active instance, plugs the front end's handlers into the memory-card
 * manager and sizes the manager's file operations for the screen's save kind.
 *
 * Stores @p self_ in cFEMemCard_mInstance and fills 40 of the 45 cMCOverlayManager callback slots
 * (cSSXApp_getMCManager): callbackWriteStart = cFEMemCard_writeStart, callbackWriteUpdate = cFEMemCard_callbackWriteUpdate,
 * callbackWriteEnd = cFEMemCard_writeDone, callbackReadStart = cFEMemCard_readStart, callbackReadUpdate =
 * cFEMemCard_callbackReadUpdate, callbackReadEnd = cFEMemCard_readDone, the cFEMemCard_callback* handlers in the slots of the
 * same name, the button queries (cFEMemCard_isSavePressed, cFEMemCard_isCancelPressed,
 * cFEMemCard_callbackIsRetryPressed, ...) in the callbackIs* slots, and unnamed handlers (cFEMemCard_callbackConfirmOverwrite ...
 * cFEMemCard_callbackDisplayNoFiles) in the rest; callbackLoadStart, callbackReadFailed, callbackLoadReady, callbackSysRecheckFile and
 * callbackAcknowledgeCardStatusChange keep their defaults.
 *
 * Then stores @p mode at +0x1BC and 25 at +0x19C, zeroes the state words from +0x1A0 to +0x228 and the short at +0x9C,
 * and calls func_0023D5A8 (On Tour cMCOverlayManager::ManagerGainState). By mode it sets the manager's write size, read
 * size and file type (func_0023CC58, func_0023CAA0, func_0023C8F0): 2 = settings, cBESaveInterface_getSaveGameSize__FPv
 * bytes, file type 2 ("SET"); 1 = replay, 0x80000 bytes, file type 0 ("REP"); anything else = profile,
 * cBESaveInterface_getSaveProfileSize bytes, file type 1 ("GAM"). Last it builds the 16-bit file-name wildcard
 * "<product code>-<type>*" ("%s-%s*" with func_00241AA0 and func_00241D40 for mFileType) at +0x11C.
 *
 * @param self_  the memory-card screen (this)
 * @param mode  the save kind: 0 profile, 1 replay, 2 settings (On Tour iType)
 */
extern "C" void cFEMemCard_initFEMemCard(void* self_, int mode)
{
    char* self = (char*)self_;
    void* gapp = SSXApp;
    cFEMemCard_mInstance = self;
    *(int*)(self + 0x1C0) = 0;
    *(int*)(self + 0x214) = 0;
    char* app = (char*)cSSXApp_getMCManager(gapp);
    *(void**)(app + 0x0) = cFEMemCard_writeStart_a5488;
    *(void**)(app + 0x18) = cFEMemCard_readStart_a5488;
    *(void**)(app + 0x8) = cFEMemCard_writeDone_a5488;
    *(void**)(app + 0x20) = cFEMemCard_readDone_a5488;
    *(void**)(app + 0x6C) = cFEMemCard_isCancelPressed_a5488;
    *(void**)(app + 0x68) = cFEMemCard_isDeletePressed_a5488;
    *(void**)(app + 0x64) = cFEMemCard_isSavePressed_a5488;
    *(void**)(app + 0x70) = cFEMemCard_isOWPressed_a5488;
    *(void**)(app + 0x74) = cFEMemCard_isDeviceSelected_a5488;
    *(void**)(app + 0x78) = cFEMemCard_isLoadPressed_a5488;
    *(void**)(app + 0x7C) = cFEMemCard_isNextMCPressed_a5488;
    *(void**)(app + 0x80) = cFEMemCard_isPrevMCPressed_a5488;
    *(void**)(app + 0x84) = cFEMemCard_callbackIsRetryPressed_a5488;
    *(void**)(app + 0x4) = cFEMemCard_callbackWriteUpdate_a5488;
    *(void**)(app + 0x44) = cFEMemCard_callbackConfirmOverwrite_a5488;
    *(void**)(app + 0x40) = cFEMemCard_callbackFileCorrupt_a5488;
    *(void**)(app + 0x1C) = cFEMemCard_callbackReadUpdate_a5488;
    *(void**)(app + 0x88) = cFEMemCard_isFormatPressed_a5488;
    *(void**)(app + 0x48) = cFEMemCard_callbackConfirmFormat_a5488;
    *(void**)(app + 0x50) = cFEMemCard_callbackConfirmFormatDone_a5488;
    *(void**)(app + 0x4C) = cFEMemCard_callbackConfirmFormatFail_a5488;
    *(void**)(app + 0x54) = cFEMemCard_callbackConfirmDelete_a5488;
    *(void**)(app + 0x38) = cFEMemCard_callbackFileNotFound_a5488;
    *(void**)(app + 0x28) = cFEMemCard_callbackReadSuccess_a5488;
    *(void**)(app + 0x24) = cFEMemCard_callbackConfirmLoadFailed_a5488;
    *(void**)(app + 0x3C) = cFEMemCard_callbackConfirmNoCards_a5488;
    *(void**)(app + 0x58) = cFEMemCard_callbackConfirmInvalidCard_a5488;
    *(void**)(app + 0x10) = cFEMemCard_callbackConfirmSaveFailed_a5488;
    *(void**)(app + 0xC) = cFEMemCard_callbackSaveSuccess_a5488;
    *(void**)(app + 0x60) = cFEMemCard_callbackConfirmSaveTooBig_a5488;
    *(void**)(app + 0x30) = cFEMemCard_callbackDeleteDone_a5488;
    *(void**)(app + 0x34) = cFEMemCard_callbackDeleteFailed_a5488;
    *(void**)(app + 0x8C) = cFEMemCard_callbackSaveFileCorrupt_a5488;
    *(void**)(app + 0x90) = cFEMemCard_callbackCardsChecked_a5488;
    *(void**)(app + 0x94) = cFEMemCard_callbackFileChecked_a5488;
    *(void**)(app + 0x98) = cFEMemCard_callbackDisplayNoFiles_a5488;
    *(void**)(app + 0x9C) = cFEMemCard_callbackDisplayNoCards_a5488;
    *(void**)(app + 0x5C) = cFEMemCard_callbackConfirmNoFreeBlocks_a5488;
    *(void**)(app + 0xA0) = cFEMemCard_callbackFileExists_a5488;
    *(void**)(app + 0xA8) = cFEMemCard_callbackCardChanged_a5488;
    *(int*)(self + 0x19C) = 25;
    *(int*)(self + 0x1BC) = mode;
    *(int*)(self + 0x1A8) = 0;
    *(int*)(self + 0x1AC) = 0;
    *(int*)(self + 0x1B0) = 0;
    *(int*)(self + 0x1B4) = 0;
    *(int*)(self + 0x1B8) = 0;
    *(int*)(self + 0x1C4) = 0;
    *(int*)(self + 0x1CC) = 0;
    *(int*)(self + 0x1D8) = 0;
    *(int*)(self + 0x1DC) = 0;
    *(int*)(self + 0x1E0) = 0;
    *(int*)(self + 0x1E4) = 0;
    *(int*)(self + 0x1E8) = 0;
    *(int*)(self + 0x1EC) = 0;
    *(int*)(self + 0x1F0) = 0;
    *(int*)(self + 0x1F4) = 0;
    *(int*)(self + 0x1F8) = 0;
    *(int*)(self + 0x1FC) = 0;
    *(int*)(self + 0x200) = 0;
    *(int*)(self + 0x204) = 0;
    *(int*)(self + 0x208) = 0;
    *(int*)(self + 0x20C) = 0;
    *(int*)(self + 0x210) = 0;
    *(int*)(self + 0x218) = 0;
    *(int*)(self + 0x21C) = 0;
    *(int*)(self + 0x1A0) = 0;
    *(int*)(self + 0x1A4) = 0;
    *(int*)(self + 0x220) = 0;
    *(int*)(self + 0x224) = 0;
    *(int*)(self + 0x228) = 0;
    *(int*)(self + 0x1D4) = 0;
    *(int*)(self + 0x1D0) = 0;
    *(int*)(self + 0x1C8) = 0;
    *(short*)(self + 0x9C) = 0;
    func_0023D5A8(app);
    cBE_getInterface_Fv(cBE_getBE(), 5);
    if (*(int*)(self + 0x1BC) == 2)
    {
        int n = cBESaveInterface_getSaveGameSize_r();
        func_0023CC58_2(app, n);
        func_0023CAA0(app, n);
        func_0023C8F0(app, 2);
    }
    else if (*(int*)(self + 0x1BC) == 1)
    {
        func_0023CC58_2(app, 0x80000);
        func_0023CAA0(app, 0x80000);
        func_0023C8F0(app, 0);
    }
    else
    {
        int n = cBESaveInterface_getSaveProfileSize();
        func_0023CC58_2(app, n);
        func_0023CAA0(app, n);
        func_0023C8F0(app, 1);
    }
    char saveType[0x200];
    char productCode[0x200];
    char format[0x200];
    func_00241D40(app, *(int*)(app + 0xB4), saveType, 0x100);
    func_002C2540(format, str_pct_s_pct_s_c);
    func_00241AA0(productCode, 0x100);
    sprintf1(self + 0x11C, format, productCode, saveType);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cMemMan_free(void* p);
extern "C" void cBESaveInterface_releaseBuffer(void* iface);
extern "C" void cBESaveInterface_releaseReplayBuffer(void* iface);
extern "C" void cOVState_onDestroyScreen(void* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_purge(void* self);
extern "C" void cMCOverlayManager_ManagerLoseState(void* self);
extern "C" void cMCOverlayManager_cleanUp(void* self);

/**
 * Memory card screen teardown: releases the save buffers and shuts the memory card manager down.
 *
 * In replay mode (+0x1BC == 1) with an external replay buffer (+0x228 == 2) that was not kept (+0x1B4 == 0),
 * releases the save interface's replay buffer (cBESaveInterface_releaseReplayBuffer) and forgets it; always releases its general buffer
 * (cBESaveInterface_releaseBuffer). If the memory card manager is active (+0x434), cleans it up (cMCOverlayManager_cleanUp, cMCOverlayManager_ManagerLoseState,
 * cMCOverlayManager_purge). A buffer this screen allocated (+0x228 == 1) is freed. Ends with cOVState_onDestroyScreen.
 */
extern "C" void cFEMemCard_onDestroyScreen(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    if (*(int*)((char*)self + 0x1BC) == 1 && *(int*)((char*)self + 0x228) == 2 && *(int*)((char*)self + 0x1B4) == 0) {
        cBESaveInterface_releaseReplayBuffer(iface);
        *(int*)((char*)self + 0x1D4) = 0;
        *(int*)((char*)self + 0x1D0) = 0;
        *(int*)((char*)self + 0x228) = 0;
    }
    cBESaveInterface_releaseBuffer(iface);
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    if (*(int*)(app + 0x434) != 0) {
        cMCOverlayManager_cleanUp(app);
        cMCOverlayManager_ManagerLoseState(app);
        cMCOverlayManager_purge(app);
    }
    if (*(int*)((char*)self + 0x228) == 1) {
        *(int*)((char*)self + 0x228) = 0;
        if (*(void**)((char*)self + 0x1D4) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x1D4));
        }
        *(int*)((char*)self + 0x1D4) = 0;
        *(int*)((char*)self + 0x1D0) = 0;
    }
    cOVState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_writeStart);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_002412A0(void* app);
extern "C" void cReplay_initSave(void* p);
extern "C" int cBE_getDataSize(void* be);
extern "C" void cFEMemCard_createReadBuffer_59A0(void* self, int size) __asm__("cFEMemCard_createReadBuffer");
extern "C" void func_0014E050(void* be, int a);
extern "C" int cBESaveInterface_generateSaveProfile(void* iface, int a);
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void func_00241D40(void* app, int a, char* buf, int n);
extern "C" void func_00241AA0(char* buf, int n);
// PORT: the unit declares func_002C2540 as void; it returns dst (bound by asm label so the call is a call_value).
extern "C" unsigned short* func_002C2540_59A0(char* buf, const char* s) __asm__("func_002C2540");
extern "C" void sprintf1_59A0(char* dst, char* fmt, char* a, char* b, int n) __asm__("sprintf1");
extern "C" void func_00241000(void* app, char* name, int a, int b);
extern "C" void cMCOverlayManager_GetMessage(void* app, char* buf, int n);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern void* cFEMemCard_mInstance;
extern char str_pct_s_pct_spct_04d[];
extern char str_Temp_String[];

struct sVE_59A0 {
    short delta;
    short index;
    void (*fn)(void*, char*, int, int, int, int, int);
};

/**
 * Memory card callback: starts writing the current save (settings, replay or profile).
 *
 * Mode 2 (cFEMemCard_mInstance +0x1BC) writes settings (func_002412A0). Mode 1 saves the replay: cReplay_initSave,
 * a read buffer of the back end's data size filled by func_0014E050, then func_00241000 under a title built from
 * the file name and "%s %s%04d". Mode 0 saves the profile from the save interface (cBESaveInterface_generateSaveProfile,
 * cBESaveInterface_getSaveProfileSize). The first time (+0x210 and +0x208 clear), also shows the manager's current message through
 * virtual slot 36 and sets +0x210.
 */
// Memory-card "save" step: builds the save-file title for the current mode and starts the save; on first entry shows the saving dialog.
extern "C" void cFEMemCard_writeStart(void)
{
    char name[144];
    char title[144];
    char slot[0x200];
    char fmt1[0x200];
    char fmt2[0x200];
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    int mode = *(int*)((char*)cFEMemCard_mInstance + 0x1BC);
    if (mode == 2) {
        func_002412A0(app);
    } else if (mode == 1) {
        void* replay = SSXApp->Game->Replay;
        *(int*)((char*)cFEMemCard_mInstance + 0x1C8) = mode;
        cReplay_initSave(replay);
        int size = cBE_getDataSize(cBE_getBE());
        cFEMemCard_createReadBuffer_59A0(cFEMemCard_mInstance, size);
        func_0014E050(cBE_getBE(), *(int*)((char*)cFEMemCard_mInstance + 0x1D4));
        func_00241D40(app, 0, name, 65);
        func_002C2540_59A0(fmt1, str_pct_s_pct_spct_04d);
        func_00241AA0(slot, 0x100);
        sprintf1_59A0(title, fmt1, slot, name, *(int*)(app + 0xF8) + 1);
        func_00241000(app, title, *(int*)((char*)cFEMemCard_mInstance + 0x1D4), size);
    } else {
        char* app2 = (char*)cSSXApp_getMCManager(SSXApp);
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
        int profileData = cBESaveInterface_generateSaveProfile(iface, *(int*)((char*)cFEMemCard_mInstance + 0x1CC));
        int profileSize = cBESaveInterface_getSaveProfileSize();
        func_00241D40(app2, 1, name, 65);
        func_002C2540_59A0(fmt2, str_pct_s_pct_spct_04d);
        func_00241AA0(slot, 0x100);
        sprintf1_59A0(title, fmt2, slot, name, *(int*)(app2 + 0xF8) + 1);
        func_00241000(app2, title, profileData, profileSize);
    }
    char* fe = (char*)cFEMemCard_mInstance;
    if (*(int*)(fe + 0x210) == 0 && *(int*)(fe + 0x208) == 0) {
        *(int*)(fe + 0x19C) = 22;
        char* buf = (char*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
        cMCOverlayManager_GetMessage(app, buf, 800);
        sVE_59A0* vt = *(sVE_59A0**)((char*)cFEMemCard_mInstance + 8);
        vt[36].fn((char*)cFEMemCard_mInstance + vt[36].delta, buf, 1, 1, 0, 0, 1);
        *(int*)((char*)cFEMemCard_mInstance + 0x210) = 1;
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_callbackReadUpdate);
#ifdef SKIP_ASM
extern int gWeHaveToClearTheBackEnd;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBESaveInterface_releaseReplayBuffer(void* iface);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cBE_getDataSize(void* be);
void* cBE_readFromData(void* self, int a1);
extern "C" void cMCOverlayManager_ReadData(void* self, char* a, int n);
extern "C" int func_003E62D0(void* buf, int size, int seed);
extern "C" void* memcpy(void*, const void*, unsigned int);

struct sTrophyRoom_001D5C28 {
    char pad0[0x1BC];
    int mode;           // 0x1BC
    char pad1C0[0x10];
    char* cur;          // 0x1D0
    void* buf;          // 0x1D4
    char pad1D8[0x28];
    int pending;        // 0x200
    char pad204[0x24];
    int state;          // 0x228
};
// PORT: cFEMemCard_mInstance holds the trophy room state pointer.
extern sTrophyRoom_001D5C28* D_004A2028_tr __asm__("cFEMemCard_mInstance");

/**
 * Memory card read progress callback: checks and loads a replay as it arrives.
 *
 * Only replay reads (mode 1) are handled; others return 1. On the first call after the read started, CRCs the
 * data (seed 0xFBEA) and compares it with the 4 bytes after it: on a mismatch releases the replay buffer, clears
 * the buffer state and returns 0; otherwise sets gWeHaveToClearTheBackEnd and loads the back end from it
 * (cBE_readFromData__FPvi). Then asks the manager for the next chunk of up to 0x4000 bytes (cMCOverlayManager_ReadData).
 *
 * @param size  bytes to read next
 * @return 0 when the replay failed its CRC check, else 1
 */
extern "C" int cFEMemCard_callbackReadUpdate(int size)
{
    sTrophyRoom_001D5C28* memCard = D_004A2028_tr;
    int mode = memCard->mode;
    if (mode != 1) {
        return 1;
    }
    if (memCard->pending != 0) {
        memCard->pending = 0;
        char* base = memCard->cur;
        int crc = func_003E62D0(memCard->cur, cBE_getDataSize(cBE_getBE()) - 4, 64490);
        D_004A2028_tr->cur += cBE_getDataSize(cBE_getBE()) - 4;
        int stored = 0;
        memcpy(&stored, D_004A2028_tr->cur, 4);
        D_004A2028_tr->cur += 4;
        if (crc != stored) {
            cBESaveInterface_releaseReplayBuffer(cBE_getInterface_Fv(cBE_getBE(), 5));
            D_004A2028_tr->buf = 0;
            D_004A2028_tr->cur = 0;
            D_004A2028_tr->state = 0;
            return 0;
        }
        gWeHaveToClearTheBackEnd = mode;
        cBE_readFromData(cBE_getBE(), (int)base);
    }
    if (size > 0) {
        if (size > 0x4000) {
            size = 0x4000;
        }
        cMCOverlayManager_ReadData(cSSXApp_getMCManager(SSXApp), D_004A2028_tr->cur, size);
        D_004A2028_tr->cur += size;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_callbackWriteUpdate);
#ifdef SKIP_ASM
extern void* cFEMemCard_mInstance;
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cReplay_getSaveData(void* self, int* a, int* b);
extern "C" void func_00241138(void* self, int a, int b);

// cFEMemCard_mInstance: the active memory-card state object (passed to cFEMemCard_createReadBuffer in this unit).
struct sFEMemCard_5D78 {
    char pad[0x1BC];
    int mode;   // 0x1BC
};

/**
 * Memory-card write-progress callback: feeds the manager the next block of a replay being saved.
 *
 * Only replay saves (mode +0x1BC of cFEMemCard_mInstance == 1) are handled. cReplay_getSaveData on SSXApp->Game->Replay
 * produces the next block; when it does, func_00241138 writes it through the manager (cSSXApp_getMCManager) and adds
 * its size to mSavedSize.
 *
 * @param key  the size the manager asks for; cReplay_getSaveData overwrites it with the size of the produced block
 * @return 1 when a block was handed over, 0 when the replay is complete or the screen is not saving a replay
 */
extern "C" int cFEMemCard_callbackWriteUpdate(int key)
{
    int out;
    if (((sFEMemCard_5D78*)cFEMemCard_mInstance)->mode == 1) {
        void* mcManager = cSSXApp_getMCManager(SSXApp);
        if (cReplay_getSaveData(SSXApp->Game->Replay, &out, &key) != 0) {
            func_00241138(mcManager, out, key);
            return 1;
        }
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_readStart);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cBE_getDataSize(void* be);
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void cMCOverlayManager_readFile(void* self, int a, int b, int c);

struct sMoviePlayer_001D5DF0 {
    char pad0[0xF8];
    int fF8;            // 0xF8
};

struct sTrophyRoom_001D5DF0 {
    char pad0[0x1B0];
    int f1B0;           // 0x1B0
    int f1B4;           // 0x1B4
    char pad1B8[0x4];
    int mode;           // 0x1BC
    char pad1C0[0x4];
    int size;           // 0x1C4
    char pad1C8[0x8];
    int cur;            // 0x1D0
    int buf;            // 0x1D4
    char pad1D8[0x28];
    int pending;        // 0x200
    char pad204[0x24];
    int state;          // 0x228
};
// PORT: cFEMemCard_mInstance holds the trophy room state pointer.
extern sTrophyRoom_001D5DF0* D_004A2028_tr5 __asm__("cFEMemCard_mInstance");
extern "C" int cBESaveInterface_getReplayBuffer(void* iface);
extern "C" void cFEMemCard_createReadBuffer(sTrophyRoom_001D5DF0* self, int size);

/**
 * Memory card callback: starts reading a save (settings, replay or profile) into a buffer.
 *
 * Marks the read as started (+0x1B0 = 1, +0x1B4 = 0) and gives the manager the slot (+0x1D8). Mode 2 reads a
 * save game of cBESaveInterface_getSaveGameSize__FPv's size into a new read buffer; mode 1 reads a replay straight into the save
 * interface's replay buffer (cBESaveInterface_getReplayBuffer) and flags it for the CRC check in cFEMemCard_callbackReadUpdate; mode 0 reads a
 * profile of cBESaveInterface_getSaveProfileSize's size. Each read starts with cMCOverlayManager_readFile.
 */
extern "C" void cFEMemCard_readStart()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    sMoviePlayer_001D5DF0* app = (sMoviePlayer_001D5DF0*)cSSXApp_getMCManager(SSXApp);
    sTrophyRoom_001D5DF0* memCard = D_004A2028_tr5;
    memCard->f1B0 = 1;
    memCard->f1B4 = 0;
    app->fF8 = memCard->size;
    int mode = memCard->mode;
    switch (mode) {
    case 2: {
        int n = cBESaveInterface_getSaveGameSize_r();
        cFEMemCard_createReadBuffer(D_004A2028_tr5, n);
        cMCOverlayManager_readFile(app, D_004A2028_tr5->size, D_004A2028_tr5->cur, n);
        break;
    }
    case 1: {
        D_004A2028_tr5->state = 2;
        int replayBuffer = cBESaveInterface_getReplayBuffer(iface);
        sTrophyRoom_001D5DF0* memCardInst = D_004A2028_tr5;
        memCardInst->buf = memCardInst->cur = replayBuffer;
        cMCOverlayManager_readFile(app, memCardInst->size, memCardInst->cur, cBE_getDataSize(cBE_getBE()));
        D_004A2028_tr5->pending = mode;
        break;
    }
    case 0: {
        int n = cBESaveInterface_getSaveProfileSize();
        cFEMemCard_createReadBuffer(D_004A2028_tr5, n);
        cMCOverlayManager_readFile(app, D_004A2028_tr5->size, D_004A2028_tr5->cur, n);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_writeDone__FPv);
#ifdef SKIP_ASM
/**
 * Memory-card write-finished callback; does nothing.
 *
 * cFEMemCard_initFEMemCard installs it as the manager's callbackWriteEnd.
 */
void cFEMemCard_writeDone(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_readDone);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" int func_00152BB0(void* self, char* buf);
extern "C" int func_00152DE0(void* self, char* buf, int idx);
extern "C" void cFEMemCard_setSoundMode(unsigned int mode);
extern "C" void func_00287410(void* opt, int v);
extern "C" void func_00287488(void* opt, int v);
extern "C" void func_00287520(void* opt, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* opt, int v);
extern "C" void func_002875D0(void* opt, int v);
void cMemMan_free(void*);
extern void* cFEMemCard_mInstance;

struct sSndCfg_1D5F40 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int f16 : 1;                    // bit 16
    int f17 : 1;                    // bit 17
    int pad18 : 14;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sSndCfg_1D5F40 D_00535610_1D5F40 __asm__("cBEOptionInterface_mOption");

static inline sSndCfg_1D5F40 Cfg_1D5F40()
{
    return D_00535610_1D5F40;
}

struct sMemCard_1D5F40 {
    char pad0[0x1BC];
    int op;                         // 0x1BC
    char pad1C0[0x1CC - 0x1C0];
    int slot;                       // 0x1CC
    char* buf;                      // 0x1D0
    char* data;                     // 0x1D4
    char pad1D8[0x228 - 0x1D8];
    int busy;                       // 0x228
};

/**
 * Memory-card read-finished callback: loads what was read into the back end and frees the read buffer.
 *
 * With the save interface (back-end interface 5) it handles the read by the screen's mode (+0x1BC of
 * cFEMemCard_mInstance). Mode 2 (settings) loads the buffer at +0x1D0 with func_00152BB0 (On Tour
 * cBESaveInterface::loadSaveGame) and, when the audio object exists (cAudioInterface_bxAudio), applies the loaded sound options
 * from cBEOptionInterface_mOption: the output mode (bits 12-13) through cFEMemCard_setSoundMode, the SFX, speech and music
 * volumes (4-bit fields) through func_00287410, func_00287488 and func_00287520, bit 17 through cSSXAudio_SetArcadeSFX
 * and bit 16 through func_002875D0. Mode 0 (profile) loads it with func_00152DE0 (On Tour loadSaveProfile) into slot
 * +0x1CC. Mode 1 (replay) does nothing here. A failed load returns 0 at once, keeping the buffer.
 *
 * Afterwards, when the buffer state (+0x228) is 1 (allocated by this screen), frees the allocation at +0x1D4 and clears
 * +0x1D0, +0x1D4 and +0x228.
 *
 * @return 0 when the save interface rejected the data, else 1
 */
// Memory-card load/save step: applies the loaded sound options, then frees the transfer buffer.
extern "C" int cFEMemCard_readDone()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    switch (((sMemCard_1D5F40*)cFEMemCard_mInstance)->op) {
    case 2:
        if (!func_00152BB0(iface, ((sMemCard_1D5F40*)cFEMemCard_mInstance)->buf))
            return 0;
        if (cAudioInterface_bxAudio() != 0) {
            cBE_getInterface_Fv(cBE_getBE(), 4);
            cFEMemCard_setSoundMode(Cfg_1D5F40().output);
            func_00287410(cAudioInterface_bxAudio(), Cfg_1D5F40().sfxVol);
            func_00287488(cAudioInterface_bxAudio(), Cfg_1D5F40().speechVol);
            func_00287520(cAudioInterface_bxAudio(), Cfg_1D5F40().musicVol);
            cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_1D5F40().f17 != 0);
            func_002875D0(cAudioInterface_bxAudio(), Cfg_1D5F40().f16 != 0);
        }
        break;
    case 0:
        if (!func_00152DE0(iface, ((sMemCard_1D5F40*)cFEMemCard_mInstance)->buf, ((sMemCard_1D5F40*)cFEMemCard_mInstance)->slot))
            return 0;
        break;
    case 1:
        break;
    }
    if (((sMemCard_1D5F40*)cFEMemCard_mInstance)->busy == 1) {
        if (((sMemCard_1D5F40*)cFEMemCard_mInstance)->data != 0)
            cMemMan_free(((sMemCard_1D5F40*)cFEMemCard_mInstance)->data);
        ((sMemCard_1D5F40*)cFEMemCard_mInstance)->buf = 0;
        ((sMemCard_1D5F40*)cFEMemCard_mInstance)->data = 0;
        ((sMemCard_1D5F40*)cFEMemCard_mInstance)->busy = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_setSoundMode);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" int func_00287670(void* opt);
extern "C" int func_002876A0(void* opt);
extern "C" int func_002876D0(void* opt);
extern "C" void cSSXAudio_FrontEndPurge(void* opt);
extern "C" void SSXAUDIO_DeInit();
extern "C" void SSXAUDIO_Init(int lang);
extern "C" void func_00287410(void* opt, int v);
extern "C" void func_00287488(void* opt, int v);
extern "C" void func_00287520(void* opt, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* opt, int v);
extern "C" void func_002875D0(void* opt, int v);
extern "C" void func_00285FB0(void* opt);

struct sLang_001D64C0 {
    int f0;
    virtual void v01();
    virtual int v02(int lang);
};
struct sLangHolder_001D64C0 {
    char pad0[0x1D8];
    sLang_001D64C0* lang;   // 0x1D8
};
struct sOptSub_001D64C0 {
    sLangHolder_001D64C0** holder;  // 0x0
    char pad4[0x543C];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int v);
};
struct sOptBase_001D64C0 {
    char pad0[0x118];
};
struct sOpt_001D64C0 : sOptBase_001D64C0, sOptSub_001D64C0 {
};

/**
 * Switches the audio output mode, restarting the sound system when the mode changes.
 *
 * The output setting @p mode maps to an SSXAUDIO mode: 1 -> 2, 2 -> 0, 3 -> 3, anything else -> 1. The audio object
 * (cAudioInterface_bxAudio) is asked to switch through method slot 2 of the object at +0x1D8 of its +0x118 member's first
 * pointer; only when that returns 1 does the restart run: it saves the SFX, speech and music volumes (func_00287670,
 * func_002876A0, func_002876D0), the arcade-SFX flag (+0x62B8), the +0x62B4 flag and the +0x534 word, then calls
 * cSSXAudio_FrontEndPurge, SSXAUDIO_DeInit and SSXAUDIO_Init(new mode), puts the volumes and flags back (func_00287410,
 * func_00287488, func_00287520, cSSXAudio_SetArcadeSFX, func_002875D0), reloads with func_00285FB0 and hands the saved
 * +0x534 word back through virtual slot 4 of the audio object's second base.
 *
 * @param mode  the output setting, 0-3 (bits 12-13 of cBEOptionInterface_mOption)
 */
extern "C" void cFEMemCard_setSoundMode(unsigned int mode)
{
    int lang;
    switch (mode) {
    case 1:
        lang = 2;
        break;
    case 2:
        lang = 0;
        break;
    case 3:
        lang = 3;
        break;
    case 0:
    default:
        lang = 1;
        break;
    }
    if ((*((sOpt_001D64C0*)cAudioInterface_bxAudio())->holder)->lang->v02(lang) == 1) {
        float sfxVolume = func_00287670(cAudioInterface_bxAudio());
        float speechVolume = func_002876A0(cAudioInterface_bxAudio());
        float musicVolume = func_002876D0(cAudioInterface_bxAudio());
        int x = *(int*)((char*)cAudioInterface_bxAudio() + 0x62B8);
        int y = *(int*)((char*)cAudioInterface_bxAudio() + 0x62B4);
        int z = *(int*)((char*)cAudioInterface_bxAudio() + 0x534);
        cSSXAudio_FrontEndPurge(cAudioInterface_bxAudio());
        SSXAUDIO_DeInit();
        SSXAUDIO_Init(lang);
        func_00287410(cAudioInterface_bxAudio(), (int)sfxVolume);
        func_00287488(cAudioInterface_bxAudio(), (int)speechVolume);
        func_00287520(cAudioInterface_bxAudio(), (int)musicVolume);
        cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), x);
        func_002875D0(cAudioInterface_bxAudio(), y);
        func_00285FB0(cAudioInterface_bxAudio());
        sOpt_001D64C0* audio = (sOpt_001D64C0*)cAudioInterface_bxAudio();
        audio->v04(z);
    }
}
#endif

