#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "ui/uistate.h"
#include "movie/mcoverlaymanager.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "fe/festatelogin.h"
#include "fe/festatetrophyroom.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "ui/uiicon.h"
#include "ealib/shape.h"
#include "be/berewarddb.h"
#include "fe/fememcard.h"
#include "ui/uiengine.h"
#include "fe/festaterewardsroom.h"
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
extern "C" void cFEStateTrophyRoom_onCreateScreen(cFEStateTrophyRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_127trophyroom), 0);
    self->mpScreen = (cUIScreen*)screen;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" const tTrophyInfo* cBERewardInterface_getTrophy(void* self, int a1, int a2);
extern "C" int func_001CE6F0(int a, int b);
extern "C" void cFEStateTrophyRoom_updateBigArt(cFEStateTrophyRoom* self, int index);
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
    vt[slot].fn(vthis(o, vt[slot]), arg);
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
extern "C" void cFEStateTrophyRoom_onWidgetCreate(cFEStateTrophyRoom* self, cUIThing* w)
{
    int hashName = w->mHashName;
    if (hashName == GetHashValue32(str_bitmap)) {
        self->mpBigIcon = (cUIIcon*)((void*)w);
        self->updateBigArt(self->mCurSelection);
        return;
    }
    if (hashName == GetHashValue32(str_stats)) {
        self->mpStats = (cUIText*)((void*)w);
        vcall_43F0(w, 9, 0);
        return;
    }
    if (hashName == GetHashValue32(str_screentitle)) {
        int id = 0;
        switch (self->mTrophyType) {
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
            int n = func_001CE6F0(self->mPeakNum, self->mTrophyType);
            w->mMiscData = i;
            if (n < i) {
                vcall_43F0(w, 9, 0);
                vcall_43F0(w, 8, 1);
            } else if (i == 0) {
                cUIText_setAsciiString((cUIText*)w, cBERewardInterface_getTrophy(cBE_getInterface_Fv(cBE_getBE(), 13),
                                       self->mPeakNum, self->mTrophyType)->mNameStringID);
            } else {
                setChallengeName(w, self->mPeakNum, self->mTrophyType, i - 1);
            }
        } else if (hashName == GetHashValue32(D_00441618[i])) {
            if (func_001CE6F0(self->mPeakNum, self->mTrophyType) < i) {
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
extern "C" int cFEStateTrophyRoom_onWidgetMenu(cFEStateTrophyRoom* self, int pMenu, unsigned int Event)
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
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void cFEStateTrophyRoom_updateBigArt(cFEStateTrophyRoom* self, int index);
extern "C" void cFEStateTrophyRoom_updateStats(cFEStateTrophyRoom* self, int index);

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
extern "C" void cFEStateTrophyRoom_onWidgetEvent(cFEStateTrophyRoom* self, cUIThing* item, int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case 6: {
        cUIIcon* w = self->mpBigIcon;
        if (w != 0) {
            w->setVisible(0);
        }
        cUICustomInterface* obj = self->mpEngine->mpCustom;
        void* nextState = obj->getPrevState((cUIState*)self, self->mPeakNum);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 1:
        self->mCurSelection = item->mMiscData;
        self->updateBigArt(item->mMiscData);
        self->updateStats(item->mMiscData);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStateTrophyRoom_updateTexturesLoad(cFEStateTrophyRoom* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the trophy room, first finishing any of its pending trophy picture loads.
 *
 * While the pending count at 0x5C is positive, cFEStateTrophyRoom_updateTexturesLoad() hands each of the five slots that has finished
 * loading to GrMan under the name "trph", releases it and decrements the count. Then runs the base UI-state
 * update cUIState_onUpdate().
 */
extern "C" void cFEStateTrophyRoom_onUpdate(cFEStateTrophyRoom* self)
{
    if (self->mNumLoadingFiles > 0) {
        self->updateTexturesLoad();
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_updateBigArt);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" int getChallengeMedal(int rider, int a, int b, int c, int index);

/**
 * Shows the trophy room's big picture (+0x48, a cUIIcon) for a list row, or hides it.
 *
 * Row 0 is the trophy: its texture (+0x60[0]) is used when the character owns it (cBERewardInterface_isTrophyOwned on
 * reward interface 13 with mUserNum, character +0x50, peak +0x54 and trophy +0x58). Rows 1-4 are challenges:
 * getChallengeMedal gives the medal earned in challenge row - 1 (-1 for none) and the texture is +0x60[medal + 1]. With a
 * texture the icon is shown (setVisible, vtable slot 9) and gets mTextureID = it and mpTextureElem = 0; a negative @p
 * index or no texture hides it.
 *
 * @param index  the selected row (0 trophy, 1-4 challenges), or negative to hide
 */
void cFEStateTrophyRoom::updateBigArt(int index)
{
    int textureId;
    if (index < 0) {
        goto hide;
    }
    textureId = -1;
    {
        if (index == 0) {
            if (cBERewardInterface_isTrophyOwned(cBE_getInterface_Fv(cBE_getBE(), 13), mUserNum,
                              mCharID, mPeakNum,
                              mTrophyType) != 0) {
                textureId = mTextureIDs[0];
            }
        } else {
            int medal = getChallengeMedal(mUserNum,
                                  mCharID, mPeakNum,
                                  mTrophyType, index - 1);
            if (medal != -1) {
                textureId = mTextureIDs[medal + 1];
            }
        }
    }
    if (textureId < 0) {
    hide:
        mpBigIcon->setVisible(0);
    } else {
        mpBigIcon->setVisible(1);
        cUIIcon* icon = mpBigIcon;
        icon->mTextureID = textureId;
        icon->mpTextureElem = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_updateStats);
#ifdef SKIP_ASM
extern "C" void setChallengeStats(void* widget, signed char rider, int a2, int a3, int a4, int index);

/**
 * Shows the trophy room's stats text (+0x4C) for a challenge row, or hides it for row 0 (the trophy).
 *
 * Rows 1 and up fill it with setChallengeStats (mUserNum, character +0x50, peak +0x54, trophy +0x58, row - 1). Nothing
 * happens without the text.
 *
 * @param index  the selected row
 */
void cFEStateTrophyRoom::updateStats(int index)
{
    cUIText* widget = mpStats;
    if (widget != 0) {
        if (index == 0) {
            widget->setVisible(0);
        } else {
            widget->setVisible(1);
            setChallengeStats(mpStats, mUserNum,
                              mCharID, mPeakNum,
                              mTrophyType, index - 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_loadTextures);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" const tTrophyInfo* cBERewardInterface_getTrophy(void* self, int a1, int a2);
extern "C" const tMedalInfo* cBERewardInterface_getMedal(void* self, int a1, int a2);
extern "C" int func_0019DA10(void* self, int bank, const char* name, int a3, int a4, int a5);

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
 * (mpTextureFileName of cBERewardInterface_getTrophy for peak +0x54 and trophy +0x58) into slot index +0x74[0], and the
 * image of each of the trophy's four medals that has a name (cBERewardInterface_getMedal) into +0x74[1-4]. +0x5C counts
 * the requests. Called by the trophy room's constructor cFEStateTrophyRoom_cFEStateTrophyRoom.
 */
void cFEStateTrophyRoom::loadTextures()
{
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* snd = SSXApp->RealFE->mBoltFileManager;
    mTextureFileIndex[0] = func_0019DA10(snd, mCharID,
        cBERewardInterface_getTrophy(pi, mPeakNum, mTrophyType)->mpTextureFileName, 10, 1, 0);
    mNumLoadingFiles = 1;
    for (int i = 0; i < 4; i++) {
        const char* name = cBERewardInterface_getMedal(pi, mTrophyType, i)->mpTextureFileName;
        if (name != 0) {
            mTextureFileIndex[i + 1] = func_0019DA10(snd, mCharID, name, 10, 1, 0);
            mNumLoadingFiles++;
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
extern "C" void cFEStateTrophyRoom_updateBigArt(cFEStateTrophyRoom* self, int index);
extern char str_trph[];


/**
 * Turns each finished trophy-room image load into a texture and redraws the big picture.
 *
 * For each of the five cache slots (+0x74) still pending whose file has loaded (func_0019E238 on RealFE + 0x1A70, bank
 * +0x50) it creates the texture "trph" from the first shape of the shape file func_0019E2B0 returns (tbl[0].offset)
 * through GrMan's vtable slot 46 (cPS2GraphicsMan_AddTexture in cPS2GraphicsMan_vtbl) into +0x60[i], releases the file
 * (func_0019DC20), sets the slot to -1, decrements the pending count (+0x5C) and redraws the selected row's picture
 * (cFEStateTrophyRoom_updateBigArt with +0x9C). Called from cFEStateTrophyRoom_onUpdate.
 *
 */
void cFEStateTrophyRoom::updateTexturesLoad()
{
    cFEStateTrophyRoom* self = this;
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->mTextureFileIndex[i] >= 0 && func_0019E238(mgr, self->mCharID, self->mTextureFileIndex[i]) != 0) {
            SHAPEFILE* fileData = (SHAPEFILE*)func_0019E2B0(mgr, self->mCharID, self->mTextureFileIndex[i]);
            // ORIGINAL-PTR: a shape file's table holds each shape's byte offset from the start of the file
            // (a file blob)
            const SHAPE* shape = (const SHAPE*)((char*)fileData + fileData->tbl[0].offset);
            self->mTextureIDs[i] = GrMan->AddTexture(shape, str_trph, 0, 1, -1);
            func_0019DC20(mgr, self->mCharID, self->mTextureFileIndex[i]);
            self->mTextureFileIndex[i] = -1;
            self->mNumLoadingFiles--;
            self->updateBigArt(self->mCurSelection);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateTrophyRoom_purgeTextures);
#ifdef SKIP_ASM
extern "C" void func_0019DC20(void* self, int bank, int i);
struct sSelf001D4B20 {
    char pad[0x50];
    int bank;
    char pad54[0xC];
    int textureId[5];   // 0x60: GrMan texture ids
    int fileSlot[5];    // 0x74: pending file-cache slots (-1 once loaded)
};

/**
 * Releases the trophy room's five images.
 *
 * Each texture (+0x60) is removed with GrMan's vtable slot 50 (cPS2GraphicsMan_RemoveTexture) and each cached file
 * (+0x74) released with func_0019DC20 (RealFE + 0x1A70, bank +0x50); both are set to -1. Called by
 * cFEStateTrophyRoom__cFEStateTrophyRoom.
 */
void cFEStateTrophyRoom::purgeTextures()
{
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    int i;
    for (i = 0; i < 5; i++) {
        if (mTextureIDs[i] >= 0) {
            GrMan->RemoveTexture(mTextureIDs[i]);
        }
        mTextureIDs[i] = -1;
        if (mTextureFileIndex[i] >= 0) {
            func_0019DC20(mgr, mCharID, mTextureFileIndex[i]);
        }
        mTextureFileIndex[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_cFEStateRewardsRoom);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern void* cFEStateRewardsRoom_vtbl[];

/**
 * Constructs the rewards-room state for a user (allocated as "cFEStateRewardsRoom", 80 bytes).
 *
 * Runs cUIState's constructor (cUIState_cUIState; @p engine reaches it untouched in the second argument register), stores
 * mHashName 46 and cFEStateRewardsRoom_vtbl, sets mUserNum to @p idx and mCtrlBits to that user's controller bits
 * (cFECustom_getPlayerCtrlMask on the engine's front end), +0x4C to @p inStore (On Tour mInStore) and +0x48 to the user's character
 * (cBENewPlayerInterface_getPlayerCharID; On Tour mCharID).
 *
 * @param engine  the UI engine (only passed on to cUIState's constructor)
 * @param idx  the user number
 * @param inStore  the in-store flag; the front end's factories pass 1
 * @return @p self
 */
extern "C" void* cFEStateRewardsRoom_cFEStateRewardsRoom(cFEStateRewardsRoom* self, int engine, signed char idx, int inStore)
{
    cUIState_cUIState(self);
    self->mHashName = 46;
    VPTR_OF(cListNode, self) = cFEStateRewardsRoom_vtbl;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, idx);
    self->mInStore = inStore;
    self->mCharID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern char str_128rewardsroom[];

/**
 * Opens the rewards-room screen ("128rewardsroom") and switches the front-end sound-bank mode to 6.
 */
extern "C" void cFEStateRewardsRoom_onCreateScreen(cFEStateRewardsRoom* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_128rewardsroom), 0);
    self->mpScreen = (cUIScreen*)screen;
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
extern "C" void cFEStateRewardsRoom_setRewardsWidgetText(cFEStateRewardsRoom* self, int index, cUIText* text);
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
extern "C" void cFEStateRewardsRoom_onWidgetCreate(cFEStateRewardsRoom* self, cUIThing* w)
{
    int id = w->mHashName;
    if (id == GetHashValue32(str_art)) {
        w->mMiscData = 5;
    } else if (id == GetHashValue32(str_video)) {
        w->mMiscData = 6;
    } else if (id == GetHashValue32(str_poster)) {
        w->mMiscData = 3;
    } else if (id == GetHashValue32(str_toys)) {
        w->mMiscData = 7;
    } else if (id == GetHashValue32(str_trading_cards)) {
        w->mMiscData = 4;
    } else if (id == GetHashValue32(str_cheat_character)) {
        w->mMiscData = 8;
    } else if (id == GetHashValue32(str_txtmdl_0)) {
        self->setRewardsWidgetText(5, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_1)) {
        self->setRewardsWidgetText(3, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_2)) {
        self->setRewardsWidgetText(7, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_3)) {
        self->setRewardsWidgetText(4, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_4)) {
        self->setRewardsWidgetText(8, (cUIText*)w);
    } else if (id == GetHashValue32(str_txtmdl_5)) {
        self->setRewardsWidgetText(6, (cUIText*)w);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRewardsRoom_onWidgetEvent);
#ifdef SKIP_ASM
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
extern "C" void cFEStateRewardsRoom_onWidgetEvent(cFEStateRewardsRoom* self, cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case 0:
        break;
    case 5: {
        cUICustomInterface* obj = (cUICustomInterface*)((void*)self->mpEngine->mpCustom);
        void* nextState = obj->getNextState((cUIState*)self, item->mMiscData);
        cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        break;
    }
    case 6: {
        cUICustomInterface* obj = (cUICustomInterface*)((void*)self->mpEngine->mpCustom);
        void* nextState = obj->getPrevState((cUIState*)self, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
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
extern "C" int cFEStateRewardsRoom_onWidgetMenu(cFEStateRewardsRoom* self, int pMenu, unsigned int key)
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int cBERewardInterface_getTotalNumRewards(void* iface, int index);
extern "C" int cBERewardInterface_getNumOwnedRewards(void* iface, int rider, int a, int index);
extern char str_pct_d_pct_d[];

/**
 * Sets a rewards-room text to "<got> / <total>" for one kind of reward.
 *
 * The total comes from cBERewardInterface_getTotalNumRewards and the count that the user's character (+0x48) has from cBERewardInterface_getNumOwnedRewards, both on
 * reward interface 13.
 *
 * @param index  the reward kind
 * @param text  the text to set
 */
void cFEStateRewardsRoom::setRewardsWidgetText(int index, cUIText* text)
{
    char buf[256];
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    int total = cBERewardInterface_getTotalNumRewards(iface, index);
    int got = cBERewardInterface_getNumOwnedRewards(iface, mUserNum, mCharID, index);
    sprintf(buf, str_pct_d_pct_d, got, total);
    cUIText_setAsciiString(text, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_cFEStateRelogin);
#ifdef SKIP_ASM
extern "C" void* cFEStateLogin_cFEStateLogin(void* self);
extern void* cFEStateRelogin_vtbl[];

/**
 * Constructs the relogin state: runs cFEStateLogin_cFEStateLogin, installs cFEStateRelogin_vtbl (+0x8) and sets
 * mHashName (+0xC) to 52.
 *
 * The C call passes only @p self, so the caller's engine argument (cRealFE passes its UI) reaches the login constructor
 * unchanged.
 *
 * @return @p self
 */
extern "C" void* cFEStateRelogin_cFEStateRelogin(cFEStateLogin* self)
{
    cFEStateLogin_cFEStateLogin(self);
    VPTR_OF(cListNode, self) = cFEStateRelogin_vtbl;
    self->mHashName = 52;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEStateRelogin_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_248lobby_reconnect[];

/**
 * Creates the relogin screen: adds the "248lobby_reconnect" screen to the engine (+0x10) with
 * cUIEngine_addScreenByHashName, stores it in the screen pointer (+0x40) and plays it from frame 0.
 */
extern "C" void cFEStateRelogin_onCreateScreen(cFEStateLogin* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_248lobby_reconnect), 0);
    self->mpScreen = (cUIScreen*)screen;
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

/**
 * Fills in the relogin screen's text: the widget named "TextReconnecting" (hash at +0x38) gets the localised
 * "kT_POPUPCMNReconnecting" string (cUIText_setUnicodeStringByID).
 *
 * @param item  the widget being created
 */
extern "C" void cFEStateRelogin_onWidgetCreate(cFEStateLogin* self, cUIText* item)
{
    int id = item->mHashName;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" int func_001D5138(cFEStateLogin* self, int msg)
{
    switch (msg) {
    case 201:
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    case 212:
        func_0025CD50(LobbyMan_mLobbyMan, ((int)LobbyMan_mLobbyMan->mPersona));
        break;
    case 257:
        func_001A8A40(self, cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine));
        break;
    case 273: {
        cBE_getInterface_Fv(SSXApp->BE, 7);
        tBENetworkDB* netOpt = &cBENetworkInterface_networkOpt;
        func_0025FD50(LobbyMan_mLobbyMan, netOpt->bIsRankedGame, netOpt->bIsHost == 0, netOpt->bIsHost != 0, (char*)&netOpt->MyStats, (char*)&netOpt->OpponentStats);
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

/**
 * Handles a popup event: popup id 258 (+0x18) goes to func_001A9668 (close the popups and push a new login), every
 * other popup to the login state's handler (cFEStateLogin_onPopupEvent).
 *
 * @param popup  the popup state
 * @param event  the popup event
 * @return the handler's result
 */
extern "C" int cFEStateRelogin_onPopupEvent(cFEStateLogin* self, cUIState* popup, int event)
{
    int result;
    if (popup->miMiscData == 258) {
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
extern "C" int cUIStateStack_popToState(void* p);
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
 * Every event goes to cFEStateNet_onPopupEvent first. On 0xF (first option) or 20 (back) it then runs cUIStateStack_popToState on
 * the state stack (mpEngine + 0x18), sets bit 0x80 in the current state's mAttrState (+0x1C,
 * cUIStateStack_getCurrentState), logs out of the online services (func_002636F0 on func_002591B8(), no error) and
 * pushes a new cFEStateNetConfig (2332 bytes) through func_001A8A40. Popup handler slot (+0x10C) of
 * cFEStateRelogin_vtbl.
 *
 * @param item  the popup that sent the event
 * @param msg  the popup event
 */
extern "C" void func_001D5280(cFEStateLogin* self, void* item, int msg)
{
    if (msg == 0xF || msg == 20) {
        cFEStateNet_onPopupEvent(self, item, msg);
        cUIStateStack_popToState(&self->mpEngine->mStateStack);
        cUIState* state = (cUIState*)cUIStateStack_getCurrentState(&self->mpEngine->mStateStack);
        state->mAttrState |= 0x80;
        func_002636F0(func_002591B8(), 0);
        func_001A8A40(self, cFEStateNetConfig_cFEStateNetConfig(cMemMan_alloc(2332, str_cFEStateNetConfig, 0, 0), self->mpEngine));
    } else {
        cFEStateNet_onPopupEvent(self, item, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_cFEMemCard);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" void cFEMemCard_initFEMemCard(cFEMemCard* self, int a1);
extern void* cOVState_vtbl[];
extern void* cFEMemCard_vtbl[];
extern unsigned short str_sp[];

/**
 * Constructs a memory-card overlay state with an overlay id: cFEMemCard_cFEMemCard2's work, but with mOVID (+0x98) = @p
 * a3.
 *
 * Runs cUIState's constructor (cUIState_cUIState; @p engine reaches it untouched in the second argument register as the
 * engine), the inlined cOVState part (vtable cOVState_vtbl; mDoneTransition, mAllowPauseMenu and mRemovePDA 0; mszChatBuff
 * " " from str_sp), stores cFEMemCard's vtable cFEMemCard_vtbl and runs cFEMemCard_initFEMemCard with @p memCardType. cOVState_SAVEREPLAY_cOVState_SAVEREPLAY and
 * cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS (fe/ovstatepause) call it with types 1 and 2.
 *
 * @param engine  the UI engine (only passed on to cUIState's constructor)
 * @param memCardType  the memory-card screen type, passed to cFEMemCard_initFEMemCard
 * @param overlayId  the overlay id stored in mOVID
 * @return @p self
 */
extern "C" void* cFEMemCard_cFEMemCard(cFEMemCard* self, int engine, int memCardType, int overlayId)
{
    cUIState_cUIState(self);
    self->mOVID = overlayId;
    VPTR_OF(cListNode, self) = cOVState_vtbl;
    self->mDoneTransition = 0;
    self->mAllowPauseMenu = 0;
    self->mRemovePDA = 0;
    *(unsigned short*)self->mszChatBuff = str_sp[0];
    VPTR_OF(cListNode, self) = cFEMemCard_vtbl;
    self->initFEMemCard(memCardType);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_cFEMemCard2);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern "C" void cFEMemCard_initFEMemCard(cFEMemCard* self, int a1);
extern void* cOVState_vtbl[];
extern void* cFEMemCard_vtbl[];
extern unsigned short str_sp[];

/**
 * Constructs the memory card screen base used by the profile, autosave and save screens.
 *
 * Runs the cUIState constructor, installs the cOVState vtable (cOVState_vtbl) and then this class's (cFEMemCard_vtbl),
 * clears +0x48, +0x4C, +0x54 and +0x98, copies the empty string str_sp into +0x58, and runs cFEMemCard_initFEMemCard
 * (On Tour initFEMemCard) with @p iType. The engine argument is not used.
 *
 * @param pEngine  the UI engine (unused)
 * @param iType  what the screen is for (On Tour iType)
 * @return @p self
 */
void* cFEMemCard::cFEMemCard2(int pEngine, int iType)
{
    cUIState_cUIState(this);
    mOVID = 0;
    VPTR_OF(cListNode, this) = cOVState_vtbl;
    mDoneTransition = 0;
    mAllowPauseMenu = 0;
    mRemovePDA = 0;
    *(unsigned short*)mszChatBuff = str_sp[0];
    VPTR_OF(cListNode, this) = cFEMemCard_vtbl;
    initFEMemCard(iType);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard__cFEMemCard);
#ifdef SKIP_ASM
extern void* cFEMemCard_vtbl[];
extern void* cOVState_vtbl[];
extern "C" void cUIState__cUIState(void* self, int flags);

/**
 * Destroys a memory-card state.
 *
 * Stores cFEMemCard's vtable cFEMemCard_vtbl, clears cFEMemCard_mInstance, stores cOVState's vtable cOVState_vtbl and runs
 * cUIState's destructor (cUIState__cUIState) with @p flags. Destructor slot (+0xC) of cFEMemCard_vtbl; the destructors of
 * cFEStateMemCard, cOVState_PROFILE, cOVState_AUTOSAVE, cFEStateProfileLoad and cFEStateOptionsDeviceSelect end in it.
 *
 * @param flags  the g++ delete flags, passed on to cUIState__cUIState
 */
extern "C" void cFEMemCard__cFEMemCard(cFEMemCard* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEMemCard_vtbl;
    cFEMemCard_mInstance = 0;
    VPTR_OF(cListNode, self) = cOVState_vtbl;
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the memory-card screens' menu events: 0x100 for events 8 and 9, otherwise 0x101.
 *
 * onWidgetMenu slot (+0xA4) of cFEMemCard's vtable cFEMemCard_vtbl, also used by cOVState_AUTOSAVE_vtbl, cOVState_SAVEOPTIONS_vtbl and
 * cOVState_SAVEREPLAY_vtbl.
 *
 * @param menu  the menu that asks (not used)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
extern "C" int cFEMemCard_onWidgetMenu(cFEMemCard* self, int menu, unsigned int key)
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
// PORT: cMCOverlayManager_writeSetSize__FPv really takes a second argument.
void cMCOverlayManager_writeSetSize_2(void* app, int n) __asm__("cMCOverlayManager_writeSetSize__FPv");
extern "C" void cMCOverlayManager_readSetSize(void* app, int n);
extern "C" void cMCOverlayManager_readSetFileType(void* app, int mode);
extern "C" void cMCOverlayManager_ManagerGainState(void* app);
extern "C" void cMCOverlayManager_getFileName(void* app, int a, char* buf, int n);
extern "C" void cMCOverlayManager_getProductCode(char* buf, int n);
extern "C" void func_002C2540(char* buf, char* s);
extern "C" void sprintf1(void* dst, char* a, char* b, char* c);
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
 * and calls cMCOverlayManager_ManagerGainState (On Tour cMCOverlayManager::ManagerGainState). By mode it sets the manager's write size, read
 * size and file type (cMCOverlayManager_writeSetSize, cMCOverlayManager_readSetSize, cMCOverlayManager_readSetFileType): 2 = settings, cBESaveInterface_getSaveGameSize__FPv
 * bytes, file type 2 ("SET"); 1 = replay, 0x80000 bytes, file type 0 ("REP"); anything else = profile,
 * cBESaveInterface_getSaveProfileSize bytes, file type 1 ("GAM"). Last it builds the 16-bit file-name wildcard
 * "<product code>-<type>*" ("%s-%s*" with cMCOverlayManager_getProductCode and cMCOverlayManager_getFileName for mFileType) at +0x11C.
 *
 * @param mode  the save kind: 0 profile, 1 replay, 2 settings (On Tour iType)
 */
void cFEMemCard::initFEMemCard(int mode)
{
    void* gapp = SSXApp;
    cFEMemCard_mInstance = this;
    mState = 0;
    mScreenStatus = 0;
    cMCOverlayManager* app = (cMCOverlayManager*)cSSXApp_getMCManager(gapp);
    app->callbackWriteStart = (void (*)())cFEMemCard_writeStart_a5488;
    app->callbackReadStart = (void (*)())cFEMemCard_readStart_a5488;
    app->callbackWriteEnd = (void (*)())cFEMemCard_writeDone_a5488;
    app->callbackReadEnd = (int (*)())cFEMemCard_readDone_a5488;
    app->callbackIsCancelPressed = (int (*)())cFEMemCard_isCancelPressed_a5488;
    app->callbackIsDeletePressed = (int (*)())cFEMemCard_isDeletePressed_a5488;
    app->callbackIsSavePressed = (int (*)())cFEMemCard_isSavePressed_a5488;
    app->callbackIsOverwritePressed = (int (*)())cFEMemCard_isOWPressed_a5488;
    app->callbackIsDeviceSelected = (int (*)())cFEMemCard_isDeviceSelected_a5488;
    app->callbackIsLoadPressed = (int (*)())cFEMemCard_isLoadPressed_a5488;
    app->callbackIsNextDevicePressed = (int (*)())cFEMemCard_isNextMCPressed_a5488;
    app->callbackIsPrevDevicePressed = (int (*)())cFEMemCard_isPrevMCPressed_a5488;
    app->callbackIsRetryPressed = (int (*)())cFEMemCard_callbackIsRetryPressed_a5488;
    app->callbackWriteUpdate = (int (*)(int))cFEMemCard_callbackWriteUpdate_a5488;
    app->callbackConfirmOverwrite = (void (*)())cFEMemCard_callbackConfirmOverwrite_a5488;
    app->callbackFileCorrupt = (void (*)())cFEMemCard_callbackFileCorrupt_a5488;
    app->callbackReadUpdate = (int (*)(int))cFEMemCard_callbackReadUpdate_a5488;
    app->callbackIsFormatPressed = (int (*)())cFEMemCard_isFormatPressed_a5488;
    app->callbackConfirmFormat = (void (*)())cFEMemCard_callbackConfirmFormat_a5488;
    app->callbackConfirmFormatDone = (void (*)())cFEMemCard_callbackConfirmFormatDone_a5488;
    app->callbackConfirmFormatFail = (void (*)())cFEMemCard_callbackConfirmFormatFail_a5488;
    app->callbackConfirmDelete = (void (*)())cFEMemCard_callbackConfirmDelete_a5488;
    app->callbackFileNotFound = (void (*)())cFEMemCard_callbackFileNotFound_a5488;
    app->callbackReadSuccess = (void (*)())cFEMemCard_callbackReadSuccess_a5488;
    app->callbackConfirmLoadFailed = (void (*)())cFEMemCard_callbackConfirmLoadFailed_a5488;
    app->callbackConfirmNoCards = (void (*)())cFEMemCard_callbackConfirmNoCards_a5488;
    app->callbackConfirmInvalidCard = (void (*)())cFEMemCard_callbackConfirmInvalidCard_a5488;
    app->callbackConfirmSaveFailed = (void (*)())cFEMemCard_callbackConfirmSaveFailed_a5488;
    app->callbackSaveSuccess = (void (*)())cFEMemCard_callbackSaveSuccess_a5488;
    app->callbackConfirmSaveTooBig = (void (*)())cFEMemCard_callbackConfirmSaveTooBig_a5488;
    app->callbackDeleteDone = (int (*)())cFEMemCard_callbackDeleteDone_a5488;
    app->callbackDeleteFailed = (int (*)())cFEMemCard_callbackDeleteFailed_a5488;
    app->callbackSaveFileCorrupt = (void (*)())cFEMemCard_callbackSaveFileCorrupt_a5488;
    app->callbackCardsChecked = (void (*)())cFEMemCard_callbackCardsChecked_a5488;
    app->callbackFileChecked = (void (*)())cFEMemCard_callbackFileChecked_a5488;
    app->callbackDisplayNoFiles = (void (*)())cFEMemCard_callbackDisplayNoFiles_a5488;
    app->callbackDisplayNoCards = (void (*)())cFEMemCard_callbackDisplayNoCards_a5488;
    app->callbackConfirmNoFreeBlocks = (void (*)())cFEMemCard_callbackConfirmNoFreeBlocks_a5488;
    app->callbackFileExists = (void (*)())cFEMemCard_callbackFileExists_a5488;
    app->callbackCardChanged = (void (*)())cFEMemCard_callbackCardChanged_a5488;
    mPopupType = 25;
    mType = mode;
    mIsProcessing = 0;
    mIsDone = 0;
    mIsLoadWait = 0;
    mIsLoadDoneOK = 0;
    mIsInvalidConfirmed = 0;
    mSlot = 0;
    mPlayerIdx = 0;
    mIsManagerStarted = 0;
    mIsSavePressed = 0;
    mIsDeletePressed = 0;
    mIsCancelPressed = 0;
    mIsFormatPressed = 0;
    mIsRetryPressed = 0;
    mIsOverwriteOK = 0;
    mIsLoadPressed = 0;
    mDoNextMemCard = 0;
    mDoPrevMemCard = 0;
    mLoadBEData = 0;
    mAutoLoadOn = 0;
    mAutoSaveOn = 0;
    mFirstCheck = 0;
    mIsStatusPopupOn = 0;
    mStatusPopup = 0;
    mLastSelectedDevice = 0;
    mProcessingKeyboard = 0;
    mpKeyboard = 0;
    mDeleteButtonActive = 0;
    mMainButtonActive = 0;
    mBufferState = 0;
    mpStartReadBuffer = 0;
    mpReadBuffer = 0;
    mIsReplayAlloc = 0;
    *(short*)&mLastLoadFileName[0] = 0;
    cMCOverlayManager_ManagerGainState(app);
    cBE_getInterface_Fv(cBE_getBE(), 5);
    if (mType == 2)
    {
        int n = cBESaveInterface_getSaveGameSize_r();
        cMCOverlayManager_writeSetSize_2(app, n);
        cMCOverlayManager_readSetSize(app, n);
        cMCOverlayManager_readSetFileType(app, 2);
    }
    else if (mType == 1)
    {
        cMCOverlayManager_writeSetSize_2(app, 0x80000);
        cMCOverlayManager_readSetSize(app, 0x80000);
        cMCOverlayManager_readSetFileType(app, 0);
    }
    else
    {
        int n = cBESaveInterface_getSaveProfileSize();
        cMCOverlayManager_writeSetSize_2(app, n);
        cMCOverlayManager_readSetSize(app, n);
        cMCOverlayManager_readSetFileType(app, 1);
    }
    char saveType[0x200];
    char productCode[0x200];
    char format[0x200];
    cMCOverlayManager_getFileName(app, app->mFileType, saveType, 0x100);
    func_002C2540(format, str_pct_s_pct_s_c);
    cMCOverlayManager_getProductCode(productCode, 0x100);
    sprintf1(&mFileName[0], format, productCode, saveType);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" void cFEMemCard_onDestroyScreen(cFEMemCard* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    if (self->mType == 1 && self->mBufferState == 2 && self->mIsLoadDoneOK == 0) {
        cBESaveInterface_releaseReplayBuffer(iface);
        self->mpStartReadBuffer = 0;
        self->mpReadBuffer = 0;
        self->mBufferState = 0;
    }
    cBESaveInterface_releaseBuffer(iface);
    cMCOverlayManager* app = (cMCOverlayManager*)((char*)cSSXApp_getMCManager(SSXApp));
    if ((int)app->mpMemCard != 0) {
        cMCOverlayManager_cleanUp(app);
        cMCOverlayManager_ManagerLoseState(app);
        cMCOverlayManager_purge(app);
    }
    if (self->mBufferState == 1) {
        self->mBufferState = 0;
        if (self->mpStartReadBuffer != 0) {
            cMemMan_free(self->mpStartReadBuffer);
        }
        self->mpStartReadBuffer = 0;
        self->mpReadBuffer = 0;
    }
    cOVState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_writeStart);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cMCOverlayManager_SettingsWriteStart(void* app);
extern "C" void cReplay_initSave(void* p);
extern "C" int cBE_getDataSize(void* be);
extern "C" void cFEMemCard_createReadBuffer_59A0(void* self, int size) __asm__("cFEMemCard_createReadBuffer");
extern "C" void cBE_writeToData(void* be, int a);
extern "C" int cBESaveInterface_generateSaveProfile(void* iface, int a);
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void cMCOverlayManager_getFileName(void* app, int a, char* buf, int n);
extern "C" void cMCOverlayManager_getProductCode(char* buf, int n);
// PORT: the unit declares func_002C2540 as void; it returns dst (bound by asm label so the call is a call_value).
extern "C" unsigned short* func_002C2540_59A0(char* buf, const char* s) __asm__("func_002C2540");
extern "C" void sprintf1_59A0(char* dst, char* fmt, char* a, char* b, int n) __asm__("sprintf1");
extern "C" void cMCOverlayManager_WriteFile(void* app, char* name, int a, int b);
extern "C" void cMCOverlayManager_GetMessage(void* app, char* buf, int n);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern char str_pct_s_pct_spct_04d[];
extern char str_Temp_String[];


/**
 * Memory card callback: starts writing the current save (settings, replay or profile).
 *
 * Mode 2 (cFEMemCard_mInstance +0x1BC) writes settings (cMCOverlayManager_SettingsWriteStart). Mode 1 saves the replay: cReplay_initSave,
 * a read buffer of the back end's data size filled by cBE_writeToData, then cMCOverlayManager_WriteFile under a title built from
 * the file name and "%s %s%04d". Mode 0 saves the profile from the save interface (cBESaveInterface_generateSaveProfile,
 * cBESaveInterface_getSaveProfileSize). The first time (+0x210 and +0x208 clear), also shows the manager's current message through
 * virtual slot 36 and sets +0x210.
 */
extern "C" void cFEMemCard_writeStart(void)
{
    char name[144];
    char title[144];
    char slot[0x200];
    char fmt1[0x200];
    char fmt2[0x200];
    cMCOverlayManager* app = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    int mode = cFEMemCard_mInstance->mType;
    if (mode == 2) {
        cMCOverlayManager_SettingsWriteStart(app);
    } else if (mode == 1) {
        void* replay = SSXApp->Game->Replay;
        cFEMemCard_mInstance->mIsReplayAlloc = mode;
        cReplay_initSave(replay);
        int size = cBE_getDataSize(cBE_getBE());
        cFEMemCard_createReadBuffer_59A0(cFEMemCard_mInstance, size);
        cBE_writeToData(cBE_getBE(), (int)cFEMemCard_mInstance->mpStartReadBuffer);
        cMCOverlayManager_getFileName(app, 0, name, 65);
        func_002C2540_59A0(fmt1, str_pct_s_pct_spct_04d);
        cMCOverlayManager_getProductCode(slot, 0x100);
        sprintf1_59A0(title, fmt1, slot, name, app->mFileCursor + 1);
        cMCOverlayManager_WriteFile(app, title, (int)cFEMemCard_mInstance->mpStartReadBuffer, size);
    } else {
        cMCOverlayManager* app2 = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
        int profileData = cBESaveInterface_generateSaveProfile(iface, cFEMemCard_mInstance->mPlayerIdx);
        int profileSize = cBESaveInterface_getSaveProfileSize();
        cMCOverlayManager_getFileName(app2, 1, name, 65);
        func_002C2540_59A0(fmt2, str_pct_s_pct_spct_04d);
        cMCOverlayManager_getProductCode(slot, 0x100);
        sprintf1_59A0(title, fmt2, slot, name, app2->mFileCursor + 1);
        cMCOverlayManager_WriteFile(app2, title, profileData, profileSize);
    }
    cFEMemCard* fe = cFEMemCard_mInstance;
    if (fe->mIsStatusPopupOn == 0 && fe->mAutoSaveOn == 0) {
        fe->mPopupType = 22;
        char* buf = (char*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
        cMCOverlayManager_GetMessage(app, buf, 800);
        cFEMemCard_mInstance->getPopup((unsigned short*)buf, 1, 1, 0, 0, 1);
        cFEMemCard_mInstance->mIsStatusPopupOn = 1;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBESaveInterface_releaseReplayBuffer(void* iface);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cBE_getDataSize(void* be);
void* cBE_readFromData(void* self, int a1);
extern "C" void cMCOverlayManager_ReadData(void* self, char* a, int n);
extern "C" int MEM_crc16(void* buf, int size, int seed);
extern "C" void* memcpy(void*, const void*, unsigned int);

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
    cFEMemCard* memCard = cFEMemCard_mInstance;
    int mode = memCard->mType;
    if (mode != 1) {
        return 1;
    }
    if (memCard->mLoadBEData != 0) {
        memCard->mLoadBEData = 0;
        char* base = memCard->mpReadBuffer;
        int crc = MEM_crc16(memCard->mpReadBuffer, cBE_getDataSize(cBE_getBE()) - 4, 64490);
        cFEMemCard_mInstance->mpReadBuffer += cBE_getDataSize(cBE_getBE()) - 4;
        int stored = 0;
        memcpy(&stored, cFEMemCard_mInstance->mpReadBuffer, 4);
        cFEMemCard_mInstance->mpReadBuffer += 4;
        if (crc != stored) {
            cBESaveInterface_releaseReplayBuffer(cBE_getInterface_Fv(cBE_getBE(), 5));
            cFEMemCard_mInstance->mpStartReadBuffer = 0;
            cFEMemCard_mInstance->mpReadBuffer = 0;
            cFEMemCard_mInstance->mBufferState = 0;
            return 0;
        }
        gWeHaveToClearTheBackEnd = mode;
        cBE_readFromData(cBE_getBE(), (int)base);
    }
    if (size > 0) {
        if (size > 0x4000) {
            size = 0x4000;
        }
        cMCOverlayManager_ReadData(cSSXApp_getMCManager(SSXApp), cFEMemCard_mInstance->mpReadBuffer, size);
        cFEMemCard_mInstance->mpReadBuffer += size;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_callbackWriteUpdate);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cReplay_getSaveData(void* self, int* a, int* b);
extern "C" void cMCOverlayManager_WriteData(void* self, int a, int b);


/**
 * Memory-card write-progress callback: feeds the manager the next block of a replay being saved.
 *
 * Only replay saves (mode +0x1BC of cFEMemCard_mInstance == 1) are handled. cReplay_getSaveData on SSXApp->Game->Replay
 * produces the next block; when it does, cMCOverlayManager_WriteData writes it through the manager (cSSXApp_getMCManager) and adds
 * its size to mSavedSize.
 *
 * @param key  the size the manager asks for; cReplay_getSaveData overwrites it with the size of the produced block
 * @return 1 when a block was handed over, 0 when the replay is complete or the screen is not saving a replay
 */
extern "C" int cFEMemCard_callbackWriteUpdate(int key)
{
    int out;
    if ((cFEMemCard_mInstance)->mType == 1) {
        void* mcManager = cSSXApp_getMCManager(SSXApp);
        if (cReplay_getSaveData(SSXApp->Game->Replay, &out, &key) != 0) {
            cMCOverlayManager_WriteData(mcManager, out, key);
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cBE_getDataSize(void* be);
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void cMCOverlayManager_readFile(void* self, int a, int b, int c);


// PORT: cFEMemCard_mInstance holds the trophy room state pointer.
extern cFEMemCard* cFEMemCard_mInstance_tr5 __asm__("cFEMemCard_mInstance");
extern "C" int cBESaveInterface_getReplayBuffer(void* iface);
extern "C" void cFEMemCard_createReadBuffer(cFEMemCard* self, int size);

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
    cMCOverlayManager* app = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cFEMemCard* memCard = cFEMemCard_mInstance_tr5;
    memCard->mIsLoadWait = 1;
    memCard->mIsLoadDoneOK = 0;
    app->mFileCursor = memCard->mSlot;
    int mode = memCard->mType;
    switch (mode) {
    case 2: {
        int n = cBESaveInterface_getSaveGameSize_r();
        cFEMemCard_createReadBuffer(cFEMemCard_mInstance_tr5, n);
        cMCOverlayManager_readFile(app, cFEMemCard_mInstance_tr5->mSlot, ((int)cFEMemCard_mInstance_tr5->mpReadBuffer), n);
        break;
    }
    case 1: {
        cFEMemCard_mInstance_tr5->mBufferState = 2;
        int replayBuffer = cBESaveInterface_getReplayBuffer(iface);
        cFEMemCard* memCardInst = cFEMemCard_mInstance_tr5;
        (*(int*)&memCardInst->mpStartReadBuffer) = (*(int*)&memCardInst->mpReadBuffer) = replayBuffer;
        cMCOverlayManager_readFile(app, memCardInst->mSlot, ((int)memCardInst->mpReadBuffer), cBE_getDataSize(cBE_getBE()));
        cFEMemCard_mInstance_tr5->mLoadBEData = mode;
        break;
    }
    case 0: {
        int n = cBESaveInterface_getSaveProfileSize();
        cFEMemCard_createReadBuffer(cFEMemCard_mInstance_tr5, n);
        cMCOverlayManager_readFile(app, cFEMemCard_mInstance_tr5->mSlot, ((int)cFEMemCard_mInstance_tr5->mpReadBuffer), n);
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
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cBESaveInterface_loadSaveGame(void* self, char* buf);
extern "C" int cBESaveInterface_loadSaveProfile(void* self, char* buf, int idx);
extern "C" void cFEMemCard_setSoundMode(unsigned int mode);
extern "C" void func_00287410(void* opt, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* opt, int v);
extern "C" void func_00287520(void* opt, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* opt, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* opt, int v);
void cMemMan_free(void*);

struct sSndCfg_1D5F40 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int iUseArcadeSFX : 1;          // bit 16
    int iUseDJSpeech : 1;           // bit 17
    int pad18 : 14;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sSndCfg_1D5F40 cBEOptionInterface_mOption_1D5F40 __asm__("cBEOptionInterface_mOption");

static inline sSndCfg_1D5F40 Cfg_1D5F40()
{
    return cBEOptionInterface_mOption_1D5F40;
}


/**
 * Memory-card read-finished callback: loads what was read into the back end and frees the read buffer.
 *
 * With the save interface (back-end interface 5) it handles the read by the screen's mode (+0x1BC of
 * cFEMemCard_mInstance). Mode 2 (settings) loads the buffer at +0x1D0 with cBESaveInterface_loadSaveGame (On Tour
 * cBESaveInterface::loadSaveGame) and, when the audio object exists (cAudioInterface_bxAudio), applies the loaded sound options
 * from cBEOptionInterface_mOption: the output mode (bits 12-13) through cFEMemCard_setSoundMode, the SFX, speech and music
 * volumes (4-bit fields) through func_00287410, cSSXAudio_SetSFXVolume and func_00287520, bit 17 through cSSXAudio_SetDJSpeech
 * and bit 16 through cSSXAudio_SetArcadeSFX. Mode 0 (profile) loads it with cBESaveInterface_loadSaveProfile (On Tour loadSaveProfile) into slot
 * +0x1CC. Mode 1 (replay) does nothing here. A failed load returns 0 at once, keeping the buffer.
 *
 * Afterwards, when the buffer state (+0x228) is 1 (allocated by this screen), frees the allocation at +0x1D4 and clears
 * +0x1D0, +0x1D4 and +0x228.
 *
 * @return 0 when the save interface rejected the data, else 1
 */
extern "C" int cFEMemCard_readDone()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 5);
    switch ((cFEMemCard_mInstance)->mType) {
    case 2:
        if (!cBESaveInterface_loadSaveGame(iface, (cFEMemCard_mInstance)->mpReadBuffer))
            return 0;
        if (cAudioInterface_bxAudio() != 0) {
            cBE_getInterface_Fv(cBE_getBE(), 4);
            cFEMemCard_setSoundMode(Cfg_1D5F40().output);
            func_00287410(cAudioInterface_bxAudio(), Cfg_1D5F40().sfxVol);
            cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_1D5F40().speechVol);
            func_00287520(cAudioInterface_bxAudio(), Cfg_1D5F40().musicVol);
            cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_1D5F40().iUseDJSpeech != 0);
            cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_1D5F40().iUseArcadeSFX != 0);
        }
        break;
    case 0:
        if (!cBESaveInterface_loadSaveProfile(iface, (cFEMemCard_mInstance)->mpReadBuffer, (cFEMemCard_mInstance)->mPlayerIdx))
            return 0;
        break;
    case 1:
        break;
    }
    if ((cFEMemCard_mInstance)->mBufferState == 1) {
        if ((cFEMemCard_mInstance)->mpStartReadBuffer != 0)
            cMemMan_free((cFEMemCard_mInstance)->mpStartReadBuffer);
        (cFEMemCard_mInstance)->mpReadBuffer = 0;
        (cFEMemCard_mInstance)->mpStartReadBuffer = 0;
        (cFEMemCard_mInstance)->mBufferState = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatetrophyroom", cFEMemCard_setSoundMode);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int func_00287670(void* opt);
extern "C" int cSSXAudio_GetSFXVolume(void* opt);
extern "C" int func_002876D0(void* opt);
extern "C" void cSSXAudio_FrontEndPurge(void* opt);
extern "C" void SSXAUDIO_DeInit();
extern "C" void SSXAUDIO_Init(int lang);
extern "C" void func_00287410(void* opt, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* opt, int v);
extern "C" void func_00287520(void* opt, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* opt, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* opt, int v);
extern "C" void cSSXAudio_FrontEndLoad(void* opt);

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
 * (cAudioInterface_bxAudio) is asked to switch through method slot 2 of the object at +0x1D8 of its +0x118 member's
 * first pointer; only when that returns 1 does the restart run: it saves the SFX, speech and music volumes
 * (func_00287670, cSSXAudio_GetSFXVolume, func_002876D0), the DJ-speech flag (mDJSpeech), the arcade-SFX flag (+0x62B4)
 * and the +0x534 word, then calls cSSXAudio_FrontEndPurge, SSXAUDIO_DeInit and SSXAUDIO_Init(new mode), puts the
 * volumes and flags back (func_00287410, cSSXAudio_SetSFXVolume, func_00287520, cSSXAudio_SetDJSpeech,
 * cSSXAudio_SetArcadeSFX), reloads with cSSXAudio_FrontEndLoad and hands the saved +0x534 word back through virtual
 * slot 4 of the audio object's second base.
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
        float speechVolume = cSSXAudio_GetSFXVolume(cAudioInterface_bxAudio());
        float musicVolume = func_002876D0(cAudioInterface_bxAudio());
        int djSpeech = cAudioInterface_bxAudio()->mDJSpeech;
        int arcadeSfx = cAudioInterface_bxAudio()->mArcadeSFX;
        int saved534 = cAudioInterface_bxAudio()->MusicSys.unk_41C;
        cSSXAudio_FrontEndPurge(cAudioInterface_bxAudio());
        SSXAUDIO_DeInit();
        SSXAUDIO_Init(lang);
        func_00287410(cAudioInterface_bxAudio(), (int)sfxVolume);
        cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), (int)speechVolume);
        func_00287520(cAudioInterface_bxAudio(), (int)musicVolume);
        cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), djSpeech);
        cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), arcadeSfx);
        cSSXAudio_FrontEndLoad(cAudioInterface_bxAudio());
        sOpt_001D64C0* audio = (sOpt_001D64C0*)cAudioInterface_bxAudio();
        audio->v04(saved534);
    }
}
#endif

