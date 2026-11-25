#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "ui/uimenu.h"
#include "ealib/shape.h"
#include "be/berewarddb.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "fe/uistatebuypopup.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "fe/festatevideoplayer.h"
#include "ui/uiicon.h"
#include "ui/uiinput.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "fe/festatepreviewreward.h"
#include "fe/festaterewards.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"
#include "ui/uiscreen.h"
#include "util/locale.h"

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_129_rewardgallery[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateRewardGalleryBase_loadRewards(cFEStateRewardGalleryBase* self);
extern "C" void cFEStateRewardGalleryBase_loadIcons(cFEStateRewardGalleryBase* self);
struct sRewardEntry_F0B0 {
    char name[8];
    int id;
};
struct sRewardGallery_F0B0 {
    char pad_0x0[0x7C];
    sRewardEntry_F0B0 entries[180];  // 0x7C
};

/**
 * Creates the reward gallery screen ("129 rewardgallery"), clears the 180 reward entries and starts loading.
 *
 * Calls cFEStateRewardGalleryBase_loadRewards() (checks whether the rewards are loaded, else starts the load) and, unless flag bit 3
 * of
 * +0x48 is set, cFEStateRewardGalleryBase_loadIcons().
 */
extern "C" void cFEStateRewardGalleryBase_onCreateScreen(cFEStateRewardGalleryBase* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_129_rewardgallery), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    int i;
    for (i = 0; i < 180; i++) {
        ((sRewardGallery_F0B0*)self)->entries[i].name[0] = 0;
        ((sRewardGallery_F0B0*)self)->entries[i].id = -1;
    }
    self->mNumIcons = 0;
    self->loadRewards();
    if (((self->mLoadingState >> 3) & 1) == 0) {
        self->loadIcons();
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUIThing_getAbsPosition(void* thing, t3Vector* out);

/**
 * Prepares the reward gallery when it gains focus: measures the select bar's offset from the first grid cell and, once
 * the rewards and icons are loaded, highlights a cell.
 *
 * Vtable slot 6 of all six gallery vtables (the slot cFEStateCharEquip_onGainFocus fills in cFEStateCharEquip_vtbl).
 * With the first cell (+0x924) and the select bar (+0x8F8, "orange_select_bar") both bound, stores the bar's position
 * minus the cell's (cUIThing_getAbsPosition) in +0x70 (x) and +0x74 (y); cFEStateRewardGalleryBase_updateHighlither adds this offset back onto the highlighted
 * cell. Resets the first visible row (+0x64) to 0. When flags +0x48 have bit 3 (icons loaded) and bit 1 (rewards
 * loaded), the first cell becomes the current cell (+0x8FC) if none is set, then vtable slot 40 (cFEStateRewardGalleryBase_updateList)
 * refreshes the grid and slot 33 (cFEStateRewardGalleryBase_updateHelpText) shows the help text for reward 0.
 */
extern "C" void cFEStateRewardGalleryBase_onGainFocus(cFEStateRewardGalleryBase* self)
{
    t3Vector firstCellPos;
    t3Vector widgetPos;
    void* widget = *(void**)&self->mpIcons[0];
    if (widget != 0) {
        cUIThing_getAbsPosition(widget, &firstCellPos);
        widget = (void*)self->mpHilighter;
        if (widget != 0) {
            cUIThing_getAbsPosition(widget, &widgetPos);
            self->mVertHilighterOffset = widgetPos.y - firstCellPos.y;
            self->mHorizHilighterOffset = widgetPos.x - firstCellPos.x;
        }
    }
    self->mFirstVisibleRow = 0;
    int fl = self->mLoadingState;
    if (((fl >> 3) & 1) && ((fl >> 1) & 1)) {
        if ((int)self->mpSelected == 0) {
            self->mpSelected = (cUIIcon*)(*(int*)&self->mpIcons[0]);
        }
        self->updateList();
        self->updateHelpText(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStateRewardGalleryBase_updateRewardsLoad(cFEStateRewardGalleryBase* self);
extern "C" void cFEStateRewardGalleryBase_updateIconsLoad(cFEStateRewardGalleryBase* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Runs the reward gallery's per-frame update: finishes the reward-list load, then the icon-file load, then the base
 * cUIState_onUpdate().
 *
 * Vtable slot 12 of the gallery vtables (cFEStateCharEquip_onUpdate's slot). Calls cFEStateRewardGalleryBase_updateRewardsLoad() and cFEStateRewardGalleryBase_updateIconsLoad().
 */
extern "C" void cFEStateRewardGalleryBase_onUpdate(cFEStateRewardGalleryBase* self)
{
    self->updateRewardsLoad();
    self->updateIconsLoad();
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onInputBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIStateStack_push(void* self, void* a1);
extern char str_TransitionOut[];

// g++ 2.95 vtable entries: this-adjust, index, function.
struct sVEi_F270 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVEii_F270 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
// $gp-relative default sound player (the unit declares it later as sDefObj_1FF0).
extern cUISoundInterface gUISoundNull_F270 __asm__("gUISoundNull");

static inline int Test_F270(char* o, int slot)
{
    sVEi_F270* vt = *(sVEi_F270**)(o + 8);
    return vt[slot].fn(vthis(o, vt[slot]));
}

static inline int Call_F270(char* o, int slot, int a)
{
    sVEii_F270* vt = *(sVEii_F270**)(o + 8);
    return vt[slot].fn(vthis(o, vt[slot]), a);
}

static inline void CallV_F270(char* o, int slot, int a)
{
    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(o + 8);
    vt[slot].fn(vthis(o, vt[slot]), a);
}

static inline void Play_F270(cUISoundInterface* snd, int id)
{
    snd->playSound(id);
}

/**
 * Handles input on the reward gallery: back leaves it, accept views or buys the highlighted reward, and the direction
 * and page inputs move the highlight around the grid.
 *
 * Vtable slot 8 of the gallery vtables (cFEStateCharEquip_onInputBegin's slot). "Sound n" below is vtable slot 2 of the
 * engine's sound object (the word at mpEngine + 0x14), or of gUISoundNull when there is none; "test n" is vtable slot n
 * of @p in.
 * - Back (test 6): sound 3, hides all 32 grid cells (mpIcons, setVisible 0), plays the screen's "TransitionOut" frame,
 *   asks the engine's cFECustom (*mpEngine, its vtable slot 5: cFECustom_getPrevState) for the state to return to, passing 1
 *   when a store number is set (+0x8F0 >= 0), and pushes that with cUIStateStack_push().
 * - Accept (test 5): the reward index is first visible row (+0x64) * columns (+0x5C) + the current cell's mMiscData. An
 *   empty gallery (reward count +0x68 = 0) gets sound 4. An owned reward (slot 38, cFEStateRewardGalleryBase_isRewardOwned) opens with sound 6
 *   and slot 31 (cFEStateRewardGalleryBase_showReward), except cheat characters (type +0x4C = 8), which get sound 4. An
 *   unowned one opens the buy popup (slot 32, cFEStateRewardGalleryBase_showBuyPopup) with sound 6 only in a store (+0x8F0 >= 0) whose number
 *   equals the reward's store number (slot 36) when its cost (slot 35) is at most the cash (+0x54); otherwise sound 4.
 * - Tests 19/20 move the column left/right with wrap-around (right also wraps past the last reward); 17/18 move the row
 *   within the visible rows (+0x58), down wrapping once the row passes the total row count (+0x60); 27 and 25 move the
 *   first visible row back or forward by a page of rows, wrapping at either end.
 *
 * After a move the index is clamped to the last reward. If the cell or the page changed, stores the page, makes the
 * matching cell current (mpSelected), refreshes the grid (updateList) and the help text (slot 33) for the new index,
 * and plays sound 2 for up/down or sound 1 otherwise; if nothing changed it plays sound 4.
 *
 * @param in  the input object; its vtable slots are the button tests
 * @return 1 when the input was handled (every back, accept and move input), otherwise 0
 */
extern "C" int cFEStateRewardGalleryBase_onInputBegin(cFEStateRewardGalleryBase* self, cUIInput* in)
{
    cUISoundInterface* snd = &gUISoundNull_F270;
    if (*(cUISoundInterface**)&self->mpEngine->mpSound != 0) {
        snd = *(cUISoundInterface**)&self->mpEngine->mpSound;
    }
    if (Test_F270(((char*)in), 6)) {
        Play_F270(snd, 3);
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 8; j++) {
                cUIIcon* w = self->mpIcons[i][j];
                if (w != 0) {
                    w->setVisible(0);
                }
            }
        }
        cUIScreen_playFrame(self->mpScreen, cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut)), 1);
        cUICustomInterface* mgr = self->mpEngine->mpCustom;
        void* backState = mgr->getPrevState((cUIState*)self, self->mStoreNum >= 0);
        cUIStateStack_push(&self->mpEngine->mStateStack, backState);
        return 1;
    }
    cUIIcon* cur = self->mpSelected;
    int curIdx = 0;
    if (((char*)cur) != 0) {
        curIdx = cur->mMiscData;
    }
    int changed = 0;
    int row = curIdx / self->mMaxRewardColumns;
    int col = curIdx % self->mMaxRewardColumns;
    int scroll = self->mFirstVisibleRow;
    if (Test_F270(((char*)in), 5)) {
        int idx = self->mFirstVisibleRow * self->mMaxRewardColumns + curIdx;
        if (self->mNumTotalRewards > 0) {
            if (Call_F270(((char*)self), 38, idx)) {
                if (self->mRewardType != 8) {
                    Play_F270(snd, 6);
                    CallV_F270(((char*)self), 31, idx);
                } else {
                    Play_F270(snd, 4);
                }
            } else {
                if (self->mStoreNum >= 0) {
                    int item = Call_F270(((char*)self), 34, idx);
                    int kind = self->mStoreNum;
                    if (kind == Call_F270(((char*)self), 36, item) && Call_F270(((char*)self), 35, item) <= self->mMoney) {
                        Play_F270(snd, 6);
                        CallV_F270(((char*)self), 32, idx);
                    } else {
                        Play_F270(snd, 4);
                    }
                } else {
                    Play_F270(snd, 4);
                }
            }
            return 1;
        }
        Play_F270(snd, 4);
        return 1;
    } else {
        if (Test_F270(((char*)in), 19)) {
            col--;
            if (col < 0) col = self->mMaxRewardColumns - 1;
            changed = 1;
        } else if (Test_F270(((char*)in), 20)) {
            col++;
            if (col >= self->mMaxRewardColumns || (self->mFirstVisibleRow + row) * self->mMaxRewardColumns + col >= self->mNumTotalRewards) col = 0;
            changed = 1;
        } else if (Test_F270(((char*)in), 17)) {
            row--;
            if (row < 0) row = self->mMaxRewardRows - 1;
            changed = 1;
        } else if (Test_F270(((char*)in), 18)) {
            row++;
            if (row >= self->mMaxRewardRows || self->mNumRows < self->mFirstVisibleRow + row) row = 0;
            changed = 1;
        } else if (Test_F270(((char*)in), 27)) {
            scroll -= self->mMaxRewardRows;
            if (scroll < 0) scroll = (self->mNumRows - 1) / self->mMaxRewardRows * self->mMaxRewardRows;
            changed = 1;
        } else if (Test_F270(((char*)in), 25)) {
            changed = 1;
            scroll += self->mMaxRewardRows;
            if (scroll >= self->mNumRows) scroll = 0;
        }
        if (changed) {
        int cols = self->mMaxRewardColumns;
        int local = row * cols + col;
        int idx = local + scroll * cols;
        if (idx >= self->mNumTotalRewards) idx = self->mNumTotalRewards - 1;
        local = idx - scroll * cols;
        if (curIdx != local || scroll != self->mFirstVisibleRow) {
            self->mFirstVisibleRow = scroll;
            self->mpSelected = self->mpIcons[local / cols][local % cols];
            self->updateList();
            Call_F270(((char*)self), 33, idx);
            if (Test_F270(((char*)in), 17) || Test_F270(((char*)in), 18)) {
                Play_F270(snd, 2);
            } else {
                Play_F270(snd, 1);
            }
            return 1;
        }
        Play_F270(snd, 4);
        return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_helptext[];
extern char str_arrow_up[];
extern char str_arrow_down[];
extern char str_orange_select_bar[];
extern char str_max_rewards[];
extern char str_num_rewards[];
extern char str_slash[];
extern char str_slash_d[];
extern char str_Menu0000[];
extern char str_Poster_icon[];
extern char str_poster_icon[];
extern char str_trading_card_icon[];
extern char str_art_icon[];
extern char str_video_icon[];
extern char str_toys_icon[];
extern char str_screentitle[];
extern char str_kT_OVRCMNPosters[];
extern char str_kT_OVRCMNTradeCard[];
extern char str_kT_OVRCMNArt[];
extern char str_kT_OVRCMNVideos[];
extern char str_kT_OVRCMNToys[];
extern char str_kT_TITLECheatCharacter[];
extern char str_Page_of_text[];
extern char str_reward_name_colon[];
extern char str_kT_OVRCMNPosterColon[];
extern char str_kT_OVRCMNCardColon[];
extern char str_kT_OVRCMNArtColon[];
extern char str_kT_OVRCMNVideoColon[];
extern char str_kT_OVRCMNToyColon[];
extern char str_kT_OVRCMNCheatCharColon[];
extern char str_reward_name[];
extern char str_reward_cost[];
extern char str_you_have_amount[];
extern char str_pct_d_b[];
extern char str_empty_d[];
extern char* D_00441440[][8];
extern char* D_004414C0[][8];

struct sVEv_F820 {
    short delta;
    short index;
    void (*fn)(void*);
};


struct sGallery_F820 {
    char pad0[0x8];
    sVEv_F820* vt;              // 0x8
    char padC[0x10 - 0xC];
    char* ui;                   // 0x10
    char pad14[0x4C - 0x14];
    int type;                   // 0x4C
    char pad50[0x58 - 0x50];
    int rows;                   // 0x58
    int cols;                   // 0x5C
    char pad60[0x68 - 0x60];
    int count68;                // 0x68
    int count6C;                // 0x6C
    char pad70[0x8F4 - 0x70];
    cUIIcon* w8F4;         // 0x8F4
    cUIIcon* w8F8;         // 0x8F8
    char pad8FC[0x900 - 0x8FC];
    cUIIcon* w900;         // 0x900
    cUIIcon* w904;         // 0x904
    cUIIcon* w908;         // 0x908
    cUIIcon* w90C;         // 0x90C
    cUIIcon* w910;         // 0x910
    cUIIcon* w914;         // 0x914
    cUIIcon* w918;         // 0x918
    cUIIcon* w91C;         // 0x91C
    cUIIcon* w920;         // 0x920
    cUIIcon* cells[4][8];  // 0x924
    cUIIcon* frames[4][8]; // 0x9A4
};

static inline void show_F820(cUIIcon* w, int on)
{
    w->setVisible(on);
}

/**
 * Binds the reward gallery's widgets by name as the screen creates them, and fills the fixed ones.
 *
 * Stores the help text, arrows, select bar, counters ("max_rewards", "num_rewards" as %d), page text, reward
 * name,
 * cost and cash widgets; sets the title, the "reward name colon" label and the poster icon for the gallery's
 * reward
 * type (3 posters, 4 trading cards, 5 art, 6 videos, 7 toys, 8 cheat characters); and records the grid cells.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateRewardGalleryBase_onWidgetCreate(sGallery_F820* self, cUIIcon* w)
{
    char buf[16];
    int hash = w->mHashName;

    if (hash == GetHashValue32(str_helptext)) {
        self->w900 = w;
        show_F820(w, 0);
    } else if (hash == GetHashValue32(str_arrow_up)) {
        self->w91C = w;
    } else if (hash == GetHashValue32(str_arrow_down)) {
        self->w920 = w;
    } else if (hash == GetHashValue32(str_orange_select_bar)) {
        self->w8F8 = w;
        show_F820(w, 0);
    } else if (hash == GetHashValue32(str_max_rewards)) {
        self->w918 = w;
        sprintf(buf, str_pct_d_b, self->count68);
        cUIText_setAsciiString((cUIText*)self->w918, buf);
    } else if (hash == GetHashValue32(str_num_rewards)) {
        self->w914 = w;
        sprintf(buf, str_pct_d_b, self->count6C);
        cUIText_setAsciiString((cUIText*)self->w914, buf);
    } else if (hash == GetHashValue32(str_slash)) {
        cUIText_setAsciiString((cUIText*)w, str_slash_d);
    } else if (hash == GetHashValue32(str_Menu0000)) {
        self->w8F4 = w;
    } else if (hash == GetHashValue32(str_Poster_icon)) {
        switch (self->type) {
        case 3:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int texHash = GetHashValue32(str_poster_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, texHash);
            }
            w->mTextureID = -1;
            break;
        case 4:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int texHash = GetHashValue32(str_trading_card_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, texHash);
            }
            w->mTextureID = -1;
            break;
        case 5:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int texHash = GetHashValue32(str_art_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, texHash);
            }
            w->mTextureID = -1;
            break;
        case 6:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int texHash = GetHashValue32(str_video_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, texHash);
            }
            w->mTextureID = -1;
            break;
        case 7:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int texHash = GetHashValue32(str_toys_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, texHash);
            }
            w->mTextureID = -1;
            break;
        }
    } else if (hash == GetHashValue32(str_screentitle)) {
        switch (self->type) {
        case 3:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosters));
            break;
        case 4:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNTradeCard));
            break;
        case 5:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArt));
            break;
        case 6:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideos));
            break;
        case 7:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToys));
            break;
        case 8:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLECheatCharacter));
            break;
        }
    } else if (hash == GetHashValue32(str_Page_of_text)) {
        self->w904 = w;
        cUIText_setAsciiString((cUIText*)w, str_empty_d);
    } else if (hash == GetHashValue32(str_reward_name_colon)) {
        int type = self->type;
        if (type == 3)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosterColon));
        else if (type == 4)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNCardColon));
        else if (type == 5)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArtColon));
        else if (type == 6)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideoColon));
        else if (type == 7)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToyColon));
        else if (type == 8)
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNCheatCharColon));
    } else if (hash == GetHashValue32(str_reward_name)) {
        self->w908 = w;
        cUIText_setAsciiString((cUIText*)w, str_empty_d);
    } else if (hash == GetHashValue32(str_reward_cost)) {
        self->w90C = w;
        cUIText_setAsciiString((cUIText*)w, str_empty_d);
    } else if (hash == GetHashValue32(str_you_have_amount)) {
        self->w910 = w;
        self->vt[43].fn(vthis(self, self->vt[43]));
    } else {
        for (int row = 0; row < self->rows; row++) {
            for (int col = 0; col < self->cols; col++) {
                if (hash == GetHashValue32(D_00441440[row][col])) {
                    w->mMiscData = row * self->cols + col;
                    show_F820(w, 0);
                    self->cells[row][col] = w;
                } else if (hash == GetHashValue32(D_004414C0[row][col])) {
                    self->frames[row][col] = w;
                    show_F820(w, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onPopupEvent);
#ifdef SKIP_ASM
struct sVE_FD18 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Tracks popups over the reward gallery and buys the highlighted reward when the buy popup is accepted.
 *
 * Vtable slot 17 of the gallery vtables (cFEStateCharEquip_onPopupEvent's slot). Event 21: unless the popup's mHashName
 * is 7 (cUIStateBuyPopup's id), clears the mVisible bit (bit 3) of the gallery's mAttrState. Event 22: sets mVisible
 * again and, when the popup is the buy popup (mHashName 7) with mbAccept set, calls vtable slot 30 (cFEStateRewardGalleryBase_buyReward) for
 * the highlighted reward (first visible row +0x64 * columns +0x5C + the current cell's mMiscData). Other events are
 * ignored.
 *
 * @param widget  the popup state that sent the event (a cUIStateBuyPopup when its mHashName is 7)
 * @param msg     the popup event; 21 and 22 are handled
 */
extern "C" void cFEStateRewardGalleryBase_onPopupEvent(cFEStateRewardGalleryBase* self, cUIState* widget, int msg)
{
    switch (msg) {
    case 21:
        if (widget->mHashName != 7) {
            self->mAttrState &= ~8;
        }
        break;
    case 22:
        if (((self->mAttrState >> 3) & 1) == 0) {
            self->mAttrState |= 8;
        }
        if (widget->mHashName == 7 && ((cUIStateBuyPopup*)widget)->mbAccept != 0) {
            self->buyReward(self->mFirstVisibleRow * self->mMaxRewardColumns + self->mpSelected->mMiscData);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_getIconID);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);

struct sRewardName_FDD0 {
    char name[8];
    int id;
};

struct sRewardTable_FDD0 {
    char pad_0x0[0x78];
    int count;
    sRewardName_FDD0 entries[1];
};

// PORT: the unit declares cFEStateRewardGalleryBase_getIconID as `void (void*, char*)`, but the body returns the id.
int cFEStateRewardGalleryBase_getIconID_find(sRewardTable_FDD0* self, char* name) __asm__("cFEStateRewardGalleryBase_getIconID");

/**
 * Returns the GrMan handle of the gallery icon with the given name, or -1.
 *
 * Searches the icon table built by cFEStateRewardGalleryBase_allocIcons (count +0x78; 12-byte entries from +0x7C holding the name and the
 * handle) with strcmp (strcmp). The per-gallery icon lookups (cFEStateRewardGalleryPosters_getIconID and its copies) call it with the
 * first 4 characters of a reward's icon name.
 *
 * @param name  the icon name (4 characters, NUL-terminated)
 * @return the icon's handle, or -1 when no entry has that name
 */
int cFEStateRewardGalleryBase_getIconID_find(sRewardTable_FDD0* self, char* name)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (strcmp(self->entries[i].name, name) == 0) {
            return self->entries[i].id;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_loadIcons);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, const char* name, int a3, int a4, int a5);
struct sVE_FE60 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Starts loading the gallery's icon file, or marks the icons loaded when the gallery has none.
 *
 * The file name comes from vtable slot 25 (the per-gallery icon file, e.g. cFEStateRewardGalleryPosters_getIconsFileName's
 * "data/char/rwrdps2.big|poster_icon.ssh"; the cheat-character gallery has none). With a name, func_0019DA10() requests
 * it from the FE file table (SSXApp->RealFE + 0x1A70) in the bank of the player's character (+0x50) with arguments 9, 1
 * and 0, the slot goes into +0x8EC, and flags +0x48 get bit 2 (icons loading) set and bit 3 (icons loaded) cleared;
 * otherwise bit 3 is set and bit 2 cleared. Slot 25 is called twice, once to test and once for the name. Called by
 * cFEStateRewardGalleryBase_onCreateScreen while bit 3 is clear.
 */
void cFEStateRewardGalleryBase::loadIcons()
{
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    if (getIconsFileName() != 0) {
        mIconsFileIndex = func_0019DA10(mgr, mCharID, getIconsFileName(), 9, 1, 0);
        mLoadingState = (mLoadingState & ~8) | 4;
    } else {
        mLoadingState = (mLoadingState | 8) & ~4;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateIconsLoad);
#ifdef SKIP_ASM
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" int func_0019E238(void* self, int bank, int i);
struct sSelf001CFFF8;
struct sData001CFFF8;
extern "C" SHAPEFILE* func_0019E2B0(void* self, int bank, int i);
extern "C" void cFEStateRewardGalleryBase_allocIcons(cFEStateRewardGalleryBase* self, SHAPEFILE* data);
struct sVE_FF10 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_FF10i {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Finishes the icon-file load: once the file is in, builds the icon table from it and releases the file.
 *
 * Only while bit 3 (icons loaded) of flags +0x48 is clear, and once func_0019E238() reports the slot in +0x8EC (bank
 * +0x50 of the FE file table, SSXApp->RealFE + 0x1A70) ready: passes the file from func_0019E2B0() to cFEStateRewardGalleryBase_allocIcons(),
 * releases the slot (func_0019DC20), sets +0x8EC to -1, clears bit 2 and sets bit 3. If the rewards are loaded too (bit
 * 1), the first cell (+0x924) becomes the current cell (+0x8FC) and vtable slots 40 (cFEStateRewardGalleryBase_updateList, grid refresh) and
 * 33 (cFEStateRewardGalleryBase_updateHelpText, reward 0) run.
 */
void cFEStateRewardGalleryBase::updateIconsLoad()
{
    if (((mLoadingState >> 3) & 1) == 0) {
        char* mgr = SSXApp->RealFE->mBoltFileManager;
        if (func_0019E238(mgr, mCharID, mIconsFileIndex) != 0) {
            allocIcons(func_0019E2B0(mgr, mCharID, mIconsFileIndex));
            func_0019DC20(mgr, mCharID, mIconsFileIndex);
            mIconsFileIndex = -1;
            mLoadingState = (mLoadingState & ~4) | 8;
            if ((mLoadingState >> 1) & 1) {
                mpSelected = (cUIIcon*)(*(int*)&mpIcons[0]);
                updateList();
                updateHelpText(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_allocIcons);
#ifdef SKIP_ASM
extern "C" void SHAPE_name(void* data, int i, void* entry);
struct sDataEnt001CFFF8 {
    int off;
    int f4;
};
struct sSelf001CFFF8 {
    char pad[0x78];
    int count;
    tIconInfo entries[1];
};

/**
 * Builds the gallery's icon table from a loaded icon file (an SSH shape file), registering each shape with GrMan.
 *
 * With @p data 0 the table is emptied (count +0x78 = 0). Otherwise the count is the file's shape count (its word at
 * +8); for each shape SHAPE_name() copies the 4-character name into the 12-byte entry at +0x7C (terminated at the fifth
 * byte) and GrMan's virtual v46 (as the view declares it) registers the shape header (data + the shape's offset) with
 * the entry, 0, 1 and -1; its result becomes the entry's handle. There is no check against the 180 entries the table
 * holds.
 *
 * @param data  the loaded icon file, or 0
 */
void cFEStateRewardGalleryBase::allocIcons(SHAPEFILE* data)
{
    if (data == 0) {
        mNumIcons = 0;
        return;
    }
    mNumIcons = data->num;
    int i;
    for (i = 0; i < mNumIcons; i++) {
        SHAPE_name(data, i, &mIcons[i]);
        mIcons[i].mName[4] = 0;
        // ORIGINAL-PTR: a shape file's table holds each shape's byte offset from the start of the file (a file blob)
        const SHAPE* shape = (const SHAPE*)((char*)data + data->tbl[i].offset);
        mIcons[i].mID = GrMan->AddTexture(shape, mIcons[i].mName, 0, 1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_purgeIcons);
#ifdef SKIP_ASM
extern "C" void func_0019DC20(void* self, int bank, int i);
/**
 * Empties the gallery's icon table, releasing every registered icon and any icon-file slot still held.
 *
 * Each entry with a handle >= 0 is released through GrMan's virtual v50 (as the view declares it); every entry's handle
 * becomes -1 and its name empty. A file slot in +0x8EC that is >= 0 is released in the FE file table (func_0019DC20,
 * bank +0x50) and set to -1. Clears bit 3 (icons loaded) of flags +0x48. The callers are in fe/fepopupmisc.cpp, among
 * them the gallery vtables' slot-1 destructors (cFEStateRewardGalleryConceptArts__cFEStateRewardGalleryConceptArts, cFEStateRewardGalleryVideos__cFEStateRewardGalleryVideos, cFEStateRewardGalleryToys__cFEStateRewardGalleryToys, cFEStateRewardGalleryCheatChars__cFEStateRewardGalleryCheatChars).
 */
void cFEStateRewardGalleryBase::purgeIcons()
{
    int i;
    for (i = 0; i < mNumIcons; i++) {
        if (mIcons[i].mID >= 0) {
            GrMan->RemoveTexture(mIcons[i].mID);
        }
        mIcons[i].mID = -1;
        mIcons[i].mName[0] = 0;
    }
    int iconSlot = mIconsFileIndex;
    if (iconSlot >= 0) {
        func_0019DC20(SSXApp->RealFE->mBoltFileManager, mCharID, iconSlot);
        mIconsFileIndex = -1;
    }
    mLoadingState &= ~8;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_loadRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isRewardsLoaded(void* iface);
extern "C" void cBERewardInterface_loadRewards(void* iface);

/**
 * Starts loading the reward list, or builds it at once when the reward database is ready.
 *
 * Uses the reward interface (cBE_getInterface__Fv kind 13). When cBERewardInterface_isRewardsLoaded() reports the database not ready, sets
 * bit 0 (rewards loading) of flags +0x48, clears bit 1 and starts it with cBERewardInterface_loadRewards() (cBERewardDB_init). Otherwise
 * sets bit 1 (rewards loaded), clears bit 0, fills the gallery's list through vtable slot 28 (the per-gallery list
 * builder, e.g. cFEStateRewardGalleryPosters_allocRewards) and sets the total row count +0x60 to the reward count (+0x68) divided by the columns
 * (+0x5C), rounded up. Called by cFEStateRewardGalleryBase_onCreateScreen.
 */
void cFEStateRewardGalleryBase::loadRewards()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    if (cBERewardInterface_isRewardsLoaded(iface) == 0) {
        mLoadingState = (mLoadingState & ~2) | 1;
        cBERewardInterface_loadRewards(iface);
    } else {
        mLoadingState = (mLoadingState & ~1) | 2;
        sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)VPTR_OF(cListNode, this);
        vt[28].fn(vthis(this, vt[28]));
        mNumRows = (mNumTotalRewards + mMaxRewardColumns - 1) / mMaxRewardColumns;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRewardsLoad);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBERewardInterface_isRewardsLoaded(void* iface);

/**
 * Finishes the reward-list load once the reward database is ready, and highlights the first cell when the icons are in
 * too.
 *
 * Only while bit 1 (rewards loaded) of flags +0x48 is clear and cBERewardInterface_isRewardsLoaded() reports the database (reward interface,
 * kind 13) ready: sets bit 1, clears bit 0, builds the list (vtable slot 28) and sets the total row count +0x60 to the
 * reward count (+0x68) divided by the columns (+0x5C), rounded up. When the icons are loaded (bit 3), the first cell
 * (+0x924) becomes the current cell (+0x8FC) and slots 40 (cFEStateRewardGalleryBase_updateList) and 33
 * (cFEStateRewardGalleryBase_updateHelpText, reward 0) run.
 */
void cFEStateRewardGalleryBase::updateRewardsLoad()
{
    if (((mLoadingState >> 1) & 1) == 0) {
        if (cBERewardInterface_isRewardsLoaded(cBE_getInterface_Fv(cBE_getBE(), 13)) != 0) {
            mLoadingState = (mLoadingState & ~1) | 2;
            sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)VPTR_OF(cListNode, this);
            vt[28].fn(vthis(this, vt[28]));
            mNumRows = (mNumTotalRewards + mMaxRewardColumns - 1) / mMaxRewardColumns;
            if ((mLoadingState >> 3) & 1) {
                mpSelected = (cUIIcon*)(*(int*)&mpIcons[0]);
                sVEntry<void(void*)>* vt2 = (sVEntry<void(void*)>*)VPTR_OF(cListNode, this);
                vt2[40].fn(vthis(this, vt2[40]));
                sVEntry<void(void*, int)>* vt3 = (sVEntry<void(void*, int)>*)VPTR_OF(cListNode, this);
                vt3[33].fn(vthis(this, vt3[33]), 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_buyReward);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_buyReward(void* iface, int player, int a2, int a3, int a4, int a5);
extern "C" int cBEEconInterface_getMoney(void* iface, int a1, int charID);

/**
 * Gives the player a reward bought in the gallery, then refreshes the cash and the reward display.
 *
 * Vtable slot 30 of the gallery vtables; cFEStateRewardGalleryBase_onPopupEvent calls it when the buy popup is accepted. Calls cBERewardInterface_buyReward()
 * on the reward interface (kind 13) with mUserNum, the character (+0x50), the gallery's reward type (+0x4C), @p
 * rewardIndex and 1; reads the cash back into +0x54 with cBEEconInterface_getMoney() (interface kind 11); then runs
 * vtable slots 42 (cFEStateRewardGalleryBase_updateRewardInfo, name and cost), 33 (cFEStateRewardGalleryBase_updateHelpText for @p rewardIndex), 43
 * (cFEStateRewardGalleryBase_updateAvailableCash, cash) and 45 (cFEStateRewardGalleryBase_updateHighlither, highlight).
 *
 * @param rewardIndex  index of the reward in the gallery's list
 */
extern "C" void cFEStateRewardGalleryBase_buyReward(cFEStateRewardGalleryBase* self, int rewardIndex)
{
    cBERewardInterface_buyReward(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum,
                  self->mCharID, self->mRewardType, rewardIndex, 1);
    self->mMoney = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum,
                                                self->mCharID);
    self->updateRewardInfo();
    self->updateHelpText(rewardIndex);
    self->updateAvailableCash();
    self->updateHighlither();
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_showReward);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStatePreviewReward_cFEStatePreviewReward(cFEStatePreviewReward* self, cUIEngine* engine, cUIState* owner, int player);
extern "C" void cFEStatePreviewReward_setReward(cFEStatePreviewReward* self, int kind, void* data);
extern "C" void* cFEStateVideoPlayer_cFEStateVideoPlayer(cFEStateVideoPlayer* self, cUIEngine* engine, cUIState* owner, signed char idx);
extern "C" void cFEStateVideoPlayer_setVideoSourceFiles(cFEStateVideoPlayer* self, const char* a, const char* b);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern char str_cFEStatePreviewReward[];
extern char str_cFEStateVideoPlayer[];


/**
 * Opens the viewer for the selected reward as a popup: a preview for posters, cards, art and toys, a video
 * player for videos.
 *
 * The reward comes from vtable slot 34 (probably getReward). Types 3, 4, 5 and 7 push a
 * cFEStatePreviewReward; type
 * 6 pushes a cFEStateVideoPlayer playing the reward's file. Other types show nothing.
 */
extern "C" void cFEStateRewardGalleryBase_showReward(cFEStateRewardGalleryBase* self)
{
    void* nextState = 0;
    sVEntry<void*(void*)>* vt = (sVEntry<void*(void*)>*)VPTR_OF(cListNode, self);
    tVideoInfo* data = (tVideoInfo*)vt[34].fn(vthis(self, vt[34]));
    switch (self->mRewardType) {
    case 3:
    case 4:
    case 5:
    case 7:
        nextState = cFEStatePreviewReward_cFEStatePreviewReward((cFEStatePreviewReward*)cMemMan_alloc(0x78, str_cFEStatePreviewReward, 0, 0), self->mpEngine, (cUIState*)self,
                          self->mUserNum);
        ((cFEStatePreviewReward*)nextState)->setReward(self->mRewardType, data);
        break;
    case 6:
        nextState = cFEStateVideoPlayer_cFEStateVideoPlayer((cFEStateVideoPlayer*)cMemMan_alloc(640, str_cFEStateVideoPlayer, 0, 0), self->mpEngine, (cUIState*)self,
                          self->mUserNum);
        ((cFEStateVideoPlayer*)nextState)->setVideoSourceFiles(((const tVideoInfo*)data)->mpVideoFileName, 0);
        break;
    }
    if (nextState != 0) {
        cUIStateStack_pushPopup(&self->mpEngine->mStateStack, nextState);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_showBuyPopup);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* self, void* engine, void* owner, signed char idx);
extern "C" void cBuyPopupInfo_initBuyReward(void* self, int a1, int a2, int a3, int a4);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern char str_cUIStateBuyPopup[];

/**
 * Opens the buy popup for a reward: a 0x70-byte cUIStateBuyPopup holding the reward's name and cost and the player's
 * cash.
 *
 * Vtable slot 32 of the gallery vtables; cFEStateRewardGalleryBase_onInputBegin calls it with the reward index, which this C view does not
 * declare: the index stays in the second argument register and reaches vtable slot 34 (the per-gallery reward getter)
 * unchanged. The popup is built by cUIStateBuyPopup_cUIStateBuyPopup() (the engine, this gallery as parent, mUserNum);
 * cBuyPopupInfo_initBuyReward() fills its mInfo (+0x48) with the reward type (+0x4C), the reward's name (slot 37), its cost (slot 35)
 * and the cash (+0x54); cUIStateStack_pushPopup() pushes it on the engine's state stack (mpEngine + 0x18).
 */
extern "C" void cFEStateRewardGalleryBase_showBuyPopup(cFEStateRewardGalleryBase* self)
{
    sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)VPTR_OF(cListNode, self);
    int sel = vt[34].fn(vthis(self, vt[34]));
    char* popup = (char*)cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_cUIStateBuyPopup, 0, 0), self->mpEngine, self,
                                       self->mUserNum);
    sVEntry<int(void*, int)>* vt2 = (sVEntry<int(void*, int)>*)VPTR_OF(cListNode, self);
    int rewardName = vt2[37].fn(vthis(self, vt2[37]), sel);
    sVEntry<int(void*, int)>* vt3 = (sVEntry<int(void*, int)>*)VPTR_OF(cListNode, self);
    int cost = vt3[35].fn(vthis(self, vt3[35]), sel);
    cBuyPopupInfo_initBuyReward(popup + 0x48, self->mRewardType, rewardName, cost, self->mMoney);
    cUIStateStack_pushPopup(&self->mpEngine->mStateStack, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_isRewardOwned);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_isRewardOwned1(void* iface, int a, int b, int c, int d);

/**
 * Tests whether the player owns a reward of the gallery.
 *
 * Vtable slot 38 of the gallery vtables. Calls cBERewardInterface_isRewardOwned1() on the reward interface (kind 13) with mUserNum, the
 * character (+0x50), the reward type (+0x4C) and @p rewardIndex. The C definition is void, but the callee's result
 * stays in the return register, and the slot's callers (cFEStateRewardGalleryBase_onInputBegin, cFEStateRewardGalleryBase_updateRewardInfo, cFEStateRewardGalleryBase_updateHighlither) test it.
 *
 * @param rewardIndex  index of the reward in the gallery's list
 */
extern "C" void cFEStateRewardGalleryBase_isRewardOwned(cFEStateRewardGalleryBase* self, int rewardIndex)
{
    cBERewardInterface_isRewardOwned1(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum,
                  self->mCharID, self->mRewardType, rewardIndex);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateHelpText);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_kT_129HELPSelectCheatChar[];
extern char str_kT_129HELPBuyInPeak1[];
extern char str_kT_129HELPBuyInPeak2[];
extern char str_kT_129HELPBuyInPeak3[];
extern char str_kT_129HELPBuyItem[];
extern char str_kT_129HELPBuyInCTM[];
extern char str_kT_129HELPViewItem[];
extern char str_kT_129HELPSaveCashItem[];

static inline void setHelp_0648(char* self, char* key)
{
    cUIText_setUnicodeStringByID(*(cUIText**)(self + 0x900), GetHashValue32(key));
}

/**
 * Shows the help line for a reward: view or select it if owned, else where it can be bought and whether cash
 * suffices.
 *
 * Owned rewards show kT_129HELPViewItem (cheat characters: kT_129HELPSelectCheatChar). Otherwise, outside a
 * store
 * (+0x8F0 < 0) it says buy in CTM; in another peak's store, buy in peak 1-3; in its own store, buy or save
 * cash
 * (cash at +0x54 against the cost). A cheat character with no store number shows the reward's own text.
 *
 * @param idx  reward index (On Tour rIndex)
 */
// PORT: item pointers passed through int vtable parameters.
extern "C" void cFEStateRewardGalleryBase_updateHelpText(cFEStateRewardGalleryBase* self, int idx)
{
    cUIThing* txt = self->mpHelpText;
    if (txt == 0)
        return;
    txt->setVisible(0);
    tCheatCharInfo* item = (tCheatCharInfo*)self->getReward(idx);
    if (((char*)item) == 0)
        return;
    int state = self->getRewardStoreNum((const void*)item);
    self->mpHelpText->setVisible(1);
    if (self->mRewardType == 8) {
        if (self->isRewardOwned(idx)) {
            setHelp_0648(((char*)self), str_kT_129HELPSelectCheatChar);
        } else if (state < 0) {
            setHelp_0648(((char*)self), ((const tCheatCharInfo*)item)->mpHelpStringID);
        } else {
            int selected = self->mStoreNum;
            if (selected >= 0) {
                if (state != selected) {
                    if (state == 1)
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak1);
                    else if (state == 2)
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak2);
                    else
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak3);
                } else {
                    if (self->mMoney < self->getRewardCost((const void*)item))
                        setHelp_0648(((char*)self), str_kT_129HELPSaveCashItem);
                    else
                        setHelp_0648(((char*)self), str_kT_129HELPBuyItem);
                }
            } else {
                setHelp_0648(((char*)self), str_kT_129HELPBuyInCTM);
            }
        }
    } else {
        if (self->isRewardOwned(idx)) {
            setHelp_0648(((char*)self), str_kT_129HELPViewItem);
        } else {
            int selected = self->mStoreNum;
            if (selected >= 0) {
                if (state != selected) {
                    if (state == 1)
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak1);
                    else if (state == 2)
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak2);
                    else
                        setHelp_0648(((char*)self), str_kT_129HELPBuyInPeak3);
                } else {
                    if (self->mMoney < self->getRewardCost((const void*)item))
                        setHelp_0648(((char*)self), str_kT_129HELPSaveCashItem);
                    else
                        setHelp_0648(((char*)self), str_kT_129HELPBuyItem);
                }
            } else {
                setHelp_0648(((char*)self), str_kT_129HELPBuyInCTM);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRow);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern char str_dollar_sign[];
extern char str_questmark[];
extern char gFadedColour[];



static inline void show_08A0(cUIIcon* c, int on)
{
    c->setVisible(on);
}

/**
 * Refreshes one row of the reward grid: owned rewards show their icon, others a dollar sign or a question
 * mark.
 *
 * The dollar sign marks rewards sold in the current store. Cells past the last reward are hidden.
 *
 * @param row  grid row on screen (On Tour rowNum)
 */
extern "C" void cFEStateRewardGalleryBase_updateRow(cFEStateRewardGalleryBase* self, int row)
{
    int idx = (self->mFirstVisibleRow + row) * self->mMaxRewardColumns;
    for (int col = 0; col < self->mMaxRewardColumns; col++, idx++) {
        if (idx < self->mNumTotalRewards) {
            show_08A0(self->mpIcons[row][col], 1);
            self->mpIconBacks[row][col]->setVisible(1);
            if (self->isRewardOwned(idx) != 0) {
                int rewardIndex = self->getIconID(idx);
                if (rewardIndex >= 0) {
                    cUIIcon* cell = self->mpIcons[row][col];
                    cell->mTextureID = rewardIndex;
                    (*(void**)&cell->mpTextureElem) = 0;
                } else {
                    int texture = (int)self->getTextureElement(idx);
                    if (texture != 0) {
                        cUIIcon* cell = self->mpIcons[row][col];
                        (*(void**)&cell->mpTextureElem) = (void*)texture;
                        cell->mTextureID = -1;
                    }
                }
            } else {
                int rewardItem = (int)self->getReward(idx);
                void* icon;
                if (self->mStoreNum >= 0 && self->getRewardStoreNum((const void*)rewardItem) == self->mStoreNum) {
                    cUIEngine* ui = self->mpEngine;
                    int iconHash = GetHashValue32(str_dollar_sign);
                    icon = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
                } else {
                    cUIEngine* ui = self->mpEngine;
                    int iconHash = GetHashValue32(str_questmark);
                    icon = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
                }
                cUIIcon* cell = self->mpIcons[row][col];
                (*(void**)&cell->mpTextureElem) = icon;
                cell->mTextureID = -1;
            }
            cUIIcon* cell = self->mpIcons[row][col];
            cell->setColour((const tARGB*)gFadedColour);
        } else {
            show_08A0(self->mpIcons[row][col], 0);
            self->mpIconBacks[row][col]->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateList);
#ifdef SKIP_ASM

/**
 * Refreshes the whole reward grid: every visible row, then the arrows, highlight, counters, page number and reward
 * info.
 *
 * Calls vtable slot 39 (cFEStateRewardGalleryBase_updateRow) for each of the +0x58 rows, then slots 46 (cFEStateRewardGalleryBase_updateArrows),
 * 45 (cFEStateRewardGalleryBase_updateHighlither), 44 (cFEStateRewardGalleryBase_updateCounter), 41 (cFEStateRewardGalleryBase_updatePageNumber) and 42 (cFEStateRewardGalleryBase_updateRewardInfo).
 */
extern "C" void cFEStateRewardGalleryBase_updateList(cFEStateRewardGalleryBase* self)
{
    int i;
    for (i = 0; i < self->mMaxRewardRows; i++) {
        self->updateRow(i);
    }
    self->updateArrows();
    self->updateHighlither();
    self->updateCounter();
    self->updatePageNumber();
    self->updateRewardInfo();
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updatePageNumber);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNPageOf[];
extern char str_R1_ps2[];
extern char str_L1_ps2[];
extern char str_empty_d[];


/**
 * Updates the "page X of Y" text and shows the L1/R1 page buttons only when there is more than one page.
 *
 * Clears the text when the localized format kT_OVRCMNPageOf is missing.
 */
extern "C" void cFEStateRewardGalleryBase_updatePageNumber(cFEStateRewardGalleryBase* self)
{
    if (self->mpPageNumberText == 0)
        return;
    cUILocaleInterface* locale = self->mpScreen->mpState->mpEngine->mpLocaleInterface;
    if (locale != 0) {
        // getStringByHash (slot 4), called by hand
        sVEntry<unsigned short*(void*, int)>* vt =
            (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, locale);
        void* thisp = vthis(locale, vt[4]);
        unsigned short* fmt = vt[4].fn(thisp, GetHashValue32(str_kT_OVRCMNPageOf));
        if (fmt != 0) {
            unsigned short buf[100];
            int per = self->mMaxRewardRows;
            int pages = (self->mNumRows + per - 1) / per;
            sprintf1(buf, fmt, (self->mFirstVisibleRow + per - 1) / per + 1, pages);
            cUIText_setUnicodeString(self->mpPageNumberText, buf);
            cUIThing* buttonPrompt = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_R1_ps2));
            if (buttonPrompt != 0) {
                buttonPrompt->setVisible(pages > 1);
            }
            buttonPrompt = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_L1_ps2));
            if (buttonPrompt != 0) {
                buttonPrompt->setVisible(pages > 1);
            }
            return;
        }
    }
    cUIText_setAsciiString(self->mpPageNumberText, str_empty_d);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRewardInfo);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);
// PORT: getDollarString really takes the int value to format; bound by asm label
extern "C" const char* getDollarString_i(int v) __asm__("getDollarString");
extern char str_empty_d[];


/**
 * Shows the highlighted reward's name and, while it is unowned and has a price, its cost.
 *
 * With no current cell (+0x8FC) both texts, "reward_name" (+0x908) and "reward_cost" (+0x90C), are emptied. Otherwise
 * the reward at first visible row (+0x64) * columns (+0x5C) + the cell's mMiscData comes from vtable slot 34; its name
 * (slot 37) goes into reward_name, and reward_cost shows getDollarString() of its cost (slot 35) when that is above 0
 * and the reward is not owned (slot 38), else an empty string. Missing text widgets are skipped.
 */
extern "C" void cFEStateRewardGalleryBase_updateRewardInfo(cFEStateRewardGalleryBase* self)
{
    cUIIcon* grid = self->mpSelected;
    if (((char*)grid) == 0) {
        if (self->mpRewardNameText != 0)
            cUIText_setAsciiString(self->mpRewardNameText, str_empty_d);
        if (self->mpRewardCostText != 0)
            cUIText_setAsciiString(self->mpRewardCostText, str_empty_d);
        return;
    }
    int idx = self->mFirstVisibleRow * self->mMaxRewardColumns + grid->mMiscData;
    void* item = (void*)self->getReward(idx);
    if (self->mpRewardNameText != 0) {
        cUIText_setAsciiString(self->mpRewardNameText, self->getRewardName(item));
    }
    if (self->mpRewardCostText != 0) {
        int price = self->getRewardCost(item);
        if (price > 0) {
            if (self->isRewardOwned(idx) == 0) {
                cUIText_setAsciiString(self->mpRewardCostText, getDollarString_i(price));
                return;
            }
        }
        cUIText_setAsciiString(self->mpRewardCostText, str_empty_d);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateAvailableCash);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);

/**
 * Shows the player's cash (+0x54) as a dollar string in the "you_have_amount" text (+0x910), when that widget exists.
 */
extern "C" void cFEStateRewardGalleryBase_updateAvailableCash(cFEStateRewardGalleryBase* self)
{
    if (self->mpAvailableCashText != 0) {
        cUIText_setAsciiString(self->mpAvailableCashText, getDollarString((void*)self->mMoney));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateCounter);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_d_b[];

/**
 * Writes the two reward counters as "%d": "max_rewards" (+0x918) gets the word at +0x6C and "num_rewards" (+0x914) the
 * reward count +0x68.
 *
 * A missing widget is skipped. Likely bug: cFEStateRewardGalleryBase_onWidgetCreate fills the same widgets the other
 * way round (max_rewards from +0x68, num_rewards from +0x6C), so one of the two has the pair swapped.
 */
extern "C" void cFEStateRewardGalleryBase_updateCounter(cFEStateRewardGalleryBase* self)
{
    char buf[32];
    if (self->mpTotalRewards != 0) {
        sprintf(buf, str_pct_d_b, self->mNumUnlockedRewards);
        cUIText_setAsciiString(self->mpTotalRewards, buf);
    }
    if (self->mpNumRewards != 0) {
        sprintf(buf, str_pct_d_b, self->mNumTotalRewards);
        cUIText_setAsciiString(self->mpNumRewards, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateHighlither);
#ifdef SKIP_ASM
extern char gSolidColour[];


/**
 * Moves the select bar onto the highlighted cell and gives that cell the reward's icon when the reward is owned.
 *
 * With a select bar (+0x8F8): when there is a current cell (+0x8FC), the bar's mPosition becomes the cell's position
 * plus the offset in +0x70/+0x74 (keeping the bar's own z) and the bar is shown; otherwise it is hidden. Then, with a
 * current cell, for the reward at first visible row (+0x64) * columns (+0x5C) + the cell's mMiscData: when it is owned
 * (vtable slot 38), the cell's mTextureID becomes the icon id from slot 26 (the per-gallery icon lookup) with
 * mpTextureElem 0, or, when that id is negative, mTextureID -1 and mpTextureElem from slot 27 (the texture-element
 * lookup the cheat-character gallery fills in). Finally sets the cell's colour (setColour) to gSolidColour.
 */
extern "C" void cFEStateRewardGalleryBase_updateHighlither(cFEStateRewardGalleryBase* self)
{
    cUIThing* icon = self->mpHilighter;
    if (((char*)icon) != 0) {
        if ((char*)self->mpSelected != 0) {
            t3Vector cellPos;
            t3Vector barPos;
            cUIThing_getAbsPosition((char*)self->mpSelected, &cellPos);
            cUIThing_getAbsPosition((char*)self->mpHilighter, &barPos);
            barPos.y = cellPos.y + self->mVertHilighterOffset;
            barPos.x = cellPos.x + self->mHorizHilighterOffset;
            *(&self->mpHilighter->mPosition) = barPos;
            cUIThing* selectBar = self->mpHilighter;
            selectBar->setVisible(1);
        } else {
            icon->setVisible(0);
        }
    }
    cUIIcon* grid = self->mpSelected;
    if (((char*)grid) == 0)
        return;
    int idx = self->mFirstVisibleRow * self->mMaxRewardColumns + grid->mMiscData;
    if (self->isRewardOwned(idx) != 0) {
        int iconId = self->getIconID(idx);
        if (iconId < 0) {
            int textureElement = (int)self->getTextureElement(idx);
            cUIIcon* curCell = self->mpSelected;
            curCell->mTextureID = -1;
            curCell->mpTextureElem = (const cTextureElement*)textureElement;
        } else {
            cUIIcon* curCell = self->mpSelected;
            curCell->mTextureID = iconId;
            curCell->mpTextureElem = 0;
        }
    }
    cUIIcon* curCell = self->mpSelected;
    curCell->setColour((const tARGB*)gSolidColour);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateArrows);
#ifdef SKIP_ASM

/**
 * Shows the up arrow (+0x91C) when the first visible row (+0x64) is past 0, and the down arrow (+0x920) when more rows
 * (+0x60) follow the visible ones (+0x58).
 */
extern "C" void cFEStateRewardGalleryBase_updateArrows(cFEStateRewardGalleryBase* self)
{
    cUIIcon* obj = self->mpArrowUp;
    if (obj != 0) {
        obj->setVisible(self->mFirstVisibleRow > 0);
    }
    cUIIcon* obj2 = self->mpArrowDown;
    if (obj2 != 0) {
        obj2->setVisible(self->mFirstVisibleRow + self->mMaxRewardRows < self->mNumRows);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryPosters_vtbl[];

/**
 * Constructs the posters reward gallery (reward type 3), which cFECustom_getNextState allocates (2772 bytes) under the
 * tag "cFEStateRewardGalleryPosters".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 3, installs vtable cFEStateRewardGalleryPosters_vtbl and clears the reward count
 * (+0x68), the list base (+0xA24) and the 43 list entries from +0xA28.
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters(cFEStateRewardGalleryPosters* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 3);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryPosters_vtbl;
    self->mNumTotalRewards = 0;
    self->mpDBPosters = 0;
    for (int i = 42; i >= 0; i--) {
        self->mpPosters[i] = 0;
    }
    return ((void*)self);
}
#endif

extern void* D_00467070[];

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryPosters_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns the posters gallery's icon file name, "data/char/rwrdps2.big|poster_icon.ssh" (D_00467070).
 *
 * Vtable slot 25 of the posters vtable cFEStateRewardGalleryPosters_vtbl.
 *
 * @return D_00467070
 */
void* cFEStateRewardGalleryPosters_getIconsFileName(void* self)
{
    return (void*)D_00467070;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryPosters_getIconID);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void cFEStateRewardGalleryBase_getIconID(void* self, char* name);

/**
 * Looks up the icon of a poster in the gallery's icon table by the first 4 characters of the reward's icon name.
 *
 * Vtable slot 26 of the posters vtable cFEStateRewardGalleryPosters_vtbl. Takes the reward record in list entry @p i (+0xA28), copies 4
 * characters of the name its word at +8 points to into a terminated buffer and calls cFEStateRewardGalleryBase_getIconID(). The C definition
 * is void, but cFEStateRewardGalleryBase_getIconID()'s handle (or -1) stays in the return register, and cFEStateRewardGalleryBase_updateHighlither reads it.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryPosters_getIconID(cFEStateRewardGalleryPosters* self, int i)
{
    char buf[5];
    tPosterInfo* reward = (tPosterInfo*)((void*)self->mpPosters[i]);
    strncpy(buf, (const char*)reward->mpIconName, 4);
    buf[4] = 0;
    cFEStateRewardGalleryBase_getIconID(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryPosters_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumPosters(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the posters gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the posters vtable cFEStateRewardGalleryPosters_vtbl. The count comes from
 * cBERewardInterface_getTotalNumPosters() on the reward interface (kind 13); mpDBPosters is the database's mpPosters
 * (cBERewardInterface_getDB()); entry k of mpPosters gets &mpDBPosters[k] (16-byte reward records) and mNumTotalRewards
 * counts them. There is no check against the 43 entries the object has room for.
 */
extern "C" void cFEStateRewardGalleryPosters_allocRewards(cFEStateRewardGalleryPosters* self)
{
    int n = cBERewardInterface_getTotalNumPosters(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBPosters = cBERewardInterface_getDB()->mpPosters;
    for (int i = 0; i < n; i++) {
        self->mpPosters[self->mNumTotalRewards++] = &self->mpDBPosters[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryPosters_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the posters gallery's reward list: mNumTotalRewards, mpDBPosters and the 43 entries of mpPosters.
 *
 * Vtable slot 29 of the posters vtable cFEStateRewardGalleryPosters_vtbl.
 */
extern "C" void cFEStateRewardGalleryPosters_purgeRewards(cFEStateRewardGalleryPosters* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBPosters = 0;
    for (i = 42; i >= 0; i--) {
        self->mpPosters[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryTradingCards_vtbl[];

/**
 * Constructs the trading-card reward gallery (reward type 4), which cFECustom_getNextState allocates (3064 bytes) under
 * the tag "cFEStateRewardGalleryTradingCards".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 4, installs vtable cFEStateRewardGalleryTradingCards_vtbl and clears the list base
 * (+0xA24) and the 116 list entries from +0xA28.
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards(cFEStateRewardGalleryTradingCards* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 4);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryTradingCards_vtbl;
    self->mpDBTradingCards = 0;
    for (int i = 115; i >= 0; i--) {
        self->mpTradingCards[i] = 0;
    }
    return ((void*)self);
}
#endif

extern void* D_00467098[];

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryTradingCards_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns the trading-card gallery's icon file name, "data/char/rwrdps2.big|tradingcard_icon.ssh" (D_00467098).
 *
 * Vtable slot 25 of the trading-card vtable cFEStateRewardGalleryTradingCards_vtbl.
 *
 * @return D_00467098
 */
void* cFEStateRewardGalleryTradingCards_getIconsFileName(void* self)
{
    return (void*)D_00467098;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryTradingCards_getIconID);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void cFEStateRewardGalleryBase_getIconID(void* self, char* name);

/**
 * Looks up the icon of a trading card in the gallery's icon table by the first 4 characters of the reward's icon name.
 *
 * Vtable slot 26 of the trading-card vtable cFEStateRewardGalleryTradingCards_vtbl. Takes the reward record in list entry @p i (+0xA28), copies 4
 * characters of the name its word at +8 points to into a terminated buffer and calls cFEStateRewardGalleryBase_getIconID(). The C definition
 * is void, but cFEStateRewardGalleryBase_getIconID()'s handle (or -1) stays in the return register, and cFEStateRewardGalleryBase_updateHighlither reads it.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryTradingCards_getIconID(cFEStateRewardGalleryTradingCards* self, int i)
{
    char buf[5];
    tTradingCardInfo* reward = (tTradingCardInfo*)((void*)self->mpTradingCards[i]);
    strncpy(buf, (const char*)reward->mpIconName, 4);
    buf[4] = 0;
    cFEStateRewardGalleryBase_getIconID(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryTradingCards_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumTradingCards(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the trading-card gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the trading-card vtable cFEStateRewardGalleryTradingCards_vtbl. The count comes from
 * cBERewardInterface_getTotalNumTradingCards() on the reward interface (kind 13); mpDBTradingCards is the database's
 * mpTradingCards (cBERewardInterface_getDB()); entry k of mpTradingCards gets &mpDBTradingCards[k] (16-byte reward
 * records) and mNumTotalRewards counts them. There is no check against the 116 entries the object has room for.
 */
extern "C" void cFEStateRewardGalleryTradingCards_allocRewards(cFEStateRewardGalleryTradingCards* self)
{
    int n = cBERewardInterface_getTotalNumTradingCards(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBTradingCards = cBERewardInterface_getDB()->mpTradingCards;
    for (int i = 0; i < n; i++) {
        self->mpTradingCards[self->mNumTotalRewards++] = &self->mpDBTradingCards[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryTradingCards_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the trading-card gallery's reward list: mNumTotalRewards, mpDBTradingCards and the 116 entries of
 * mpTradingCards.
 *
 * Vtable slot 29 of the trading-card vtable cFEStateRewardGalleryTradingCards_vtbl.
 */
extern "C" void cFEStateRewardGalleryTradingCards_purgeRewards(cFEStateRewardGalleryTradingCards* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBTradingCards = 0;
    for (i = 115; i >= 0; i--) {
        self->mpTradingCards[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryConceptArts_vtbl[];

/**
 * Constructs the art reward gallery (reward type 5), which cFECustom_getNextState allocates (3000 bytes) under the tag
 * "cFEStateRewardGalleryConceptArts".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 5, installs vtable cFEStateRewardGalleryConceptArts_vtbl and clears the list base
 * (+0xA24) and the 100 list entries from +0xA28.
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts(cFEStateRewardGalleryConceptArts* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 5);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryConceptArts_vtbl;
    self->mpDBConceptArts = 0;
    for (int i = 99; i >= 0; i--) {
        self->mpConceptArts[i] = 0;
    }
    return ((void*)self);
}
#endif

extern void* D_004670C8[];

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryConceptArts_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns the art gallery's icon file name, "data/char/rwrdps2.big|gallery_icon.ssh" (D_004670C8).
 *
 * Vtable slot 25 of the art vtable cFEStateRewardGalleryConceptArts_vtbl.
 *
 * @return D_004670C8
 */
void* cFEStateRewardGalleryConceptArts_getIconsFileName(void* self)
{
    return (void*)D_004670C8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryConceptArts_getIconID);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void cFEStateRewardGalleryBase_getIconID(void* self, char* name);

/**
 * Looks up the icon of an art reward in the gallery's icon table by the first 4 characters of the reward's icon name.
 *
 * Vtable slot 26 of the art vtable cFEStateRewardGalleryConceptArts_vtbl. Takes the reward record in list entry @p i (+0xA28), copies 4 characters
 * of the name its word at +8 points to into a terminated buffer and calls cFEStateRewardGalleryBase_getIconID(). The C definition is void,
 * but cFEStateRewardGalleryBase_getIconID()'s handle (or -1) stays in the return register, and cFEStateRewardGalleryBase_updateHighlither reads it.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryConceptArts_getIconID(cFEStateRewardGalleryConceptArts* self, int i)
{
    char buf[5];
    tConceptArtInfo* reward = (tConceptArtInfo*)((void*)self->mpConceptArts[i]);
    strncpy(buf, (const char*)reward->mpIconName, 4);
    buf[4] = 0;
    cFEStateRewardGalleryBase_getIconID(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryConceptArts_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumConceptArts(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the art gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the art vtable cFEStateRewardGalleryConceptArts_vtbl. The count comes from
 * cBERewardInterface_getTotalNumConceptArts() on the reward interface (kind 13); mpDBConceptArts is the database's
 * mpConceptArts (cBERewardInterface_getDB()); entry k of mpConceptArts gets &mpDBConceptArts[k] (16-byte reward
 * records) and mNumTotalRewards counts them. There is no check against the 100 entries the object has room for.
 */
extern "C" void cFEStateRewardGalleryConceptArts_allocRewards(cFEStateRewardGalleryConceptArts* self)
{
    int n = cBERewardInterface_getTotalNumConceptArts(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBConceptArts = cBERewardInterface_getDB()->mpConceptArts;
    for (int i = 0; i < n; i++) {
        self->mpConceptArts[self->mNumTotalRewards++] = &self->mpDBConceptArts[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryConceptArts_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the art gallery's reward list: mNumTotalRewards, mpDBConceptArts and the 100 entries of mpConceptArts.
 *
 * Vtable slot 29 of the art vtable cFEStateRewardGalleryConceptArts_vtbl.
 */
extern "C" void cFEStateRewardGalleryConceptArts_purgeRewards(cFEStateRewardGalleryConceptArts* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBConceptArts = 0;
    for (i = 99; i >= 0; i--) {
        self->mpConceptArts[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryVideos_vtbl[];

/**
 * Constructs the video reward gallery (reward type 6), which cFECustom_getNextState allocates (2608 bytes) under the
 * tag "cFEStateRewardGalleryVideos".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 6, installs vtable cFEStateRewardGalleryVideos_vtbl and clears the list base
 * (+0xA24) and the 2 list entries from +0xA28 (the object has room for only 2).
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos(cFEStateRewardGalleryVideos* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 6);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryVideos_vtbl;
    self->mpDBVideos = 0;
    for (int i = 1; i >= 0; i--) {
        self->mpVideos[i] = 0;
    }
    return ((void*)self);
}
#endif

extern void* D_004670F0[];

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryVideos_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns the video gallery's icon file name, "data/char/rwrdps2.big|video_icon.ssh" (D_004670F0).
 *
 * Vtable slot 25 of the video vtable cFEStateRewardGalleryVideos_vtbl.
 *
 * @return D_004670F0
 */
void* cFEStateRewardGalleryVideos_getIconsFileName(void* self)
{
    return (void*)D_004670F0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryVideos_getIconID);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void cFEStateRewardGalleryBase_getIconID(void* self, char* name);

/**
 * Looks up the icon of a video reward in the gallery's icon table by the first 4 characters of the reward's icon name.
 *
 * Vtable slot 26 of the video vtable cFEStateRewardGalleryVideos_vtbl. Takes the reward record in list entry @p i (+0xA28), copies 4
 * characters of the name its word at +8 points to into a terminated buffer and calls cFEStateRewardGalleryBase_getIconID(). The C definition
 * is void, but cFEStateRewardGalleryBase_getIconID()'s handle (or -1) stays in the return register, and cFEStateRewardGalleryBase_updateHighlither reads it.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryVideos_getIconID(cFEStateRewardGalleryVideos* self, int i)
{
    char buf[5];
    tVideoInfo* reward = (tVideoInfo*)self->mpVideos[i];
    strncpy(buf, (const char*)reward->mpIconName, 4);
    buf[4] = 0;
    cFEStateRewardGalleryBase_getIconID(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryVideos_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumVideos(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the video gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the video vtable cFEStateRewardGalleryVideos_vtbl. The count comes from
 * cBERewardInterface_getTotalNumVideos() on the reward interface (kind 13); mpDBVideos is the database's mpVideos
 * (cBERewardInterface_getDB()); entry k of mpVideos gets &mpDBVideos[k] (16-byte reward records) and mNumTotalRewards
 * counts them. There is no check against the 2 entries the object has room for, so more than 2 videos in the database
 * would overrun it.
 */
extern "C" void cFEStateRewardGalleryVideos_allocRewards(cFEStateRewardGalleryVideos* self)
{
    int n = cBERewardInterface_getTotalNumVideos(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBVideos = cBERewardInterface_getDB()->mpVideos;
    for (int i = 0; i < n; i++) {
        self->mpVideos[self->mNumTotalRewards++] = &self->mpDBVideos[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryVideos_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the video gallery's reward list: mNumTotalRewards, mpDBVideos and the 2 entries of mpVideos.
 *
 * Vtable slot 29 of the video vtable cFEStateRewardGalleryVideos_vtbl.
 */
extern "C" void cFEStateRewardGalleryVideos_purgeRewards(cFEStateRewardGalleryVideos* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBVideos = 0;
    for (i = 1; i >= 0; i--) {
        self->mpVideos[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryToys_cFEStateRewardGalleryToys);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryToys_vtbl[];

/**
 * Constructs the toy reward gallery (reward type 7), which cFECustom_getNextState allocates (2712 bytes) under the tag
 * "cFEStateRewardGalleryToys".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 7, installs vtable cFEStateRewardGalleryToys_vtbl and clears the list base
 * (+0xA24) and the 28 list entries from +0xA28.
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryToys_cFEStateRewardGalleryToys(cFEStateRewardGalleryToys* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 7);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryToys_vtbl;
    self->mpDBToys = 0;
    for (int i = 27; i >= 0; i--) {
        self->mpToys[i] = 0;
    }
    return ((void*)self);
}
#endif

extern void* D_00467118[];

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryToys_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns the toy gallery's icon file name, "data/char/rwrdps2.big|toy_icon.ssh" (D_00467118).
 *
 * Vtable slot 25 of the toy vtable cFEStateRewardGalleryToys_vtbl.
 *
 * @return D_00467118
 */
void* cFEStateRewardGalleryToys_getIconsFileName(void* self)
{
    return (void*)D_00467118;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryToys_getIconID);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void cFEStateRewardGalleryBase_getIconID(void* self, char* name);

/**
 * Looks up the icon of a toy in the gallery's icon table by the first 4 characters of the reward's icon name.
 *
 * Vtable slot 26 of the toy vtable cFEStateRewardGalleryToys_vtbl. Takes the reward record in list entry @p i (+0xA28), copies 4 characters
 * of the name its word at +8 points to into a terminated buffer and calls cFEStateRewardGalleryBase_getIconID(). The C definition is void,
 * but cFEStateRewardGalleryBase_getIconID()'s handle (or -1) stays in the return register, and cFEStateRewardGalleryBase_updateHighlither reads it.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryToys_getIconID(cFEStateRewardGalleryToys* self, int i)
{
    char buf[5];
    tToyInfo* reward = (tToyInfo*)((void*)self->mpToys[i]);
    strncpy(buf, (const char*)reward->mpIconName, 4);
    buf[4] = 0;
    cFEStateRewardGalleryBase_getIconID(self, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryToys_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumToys(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the toy gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the toy vtable cFEStateRewardGalleryToys_vtbl. The count comes from
 * cBERewardInterface_getTotalNumToys() on the reward interface (kind 13); mpDBToys is the database's mpToys
 * (cBERewardInterface_getDB()); entry k of mpToys gets &mpDBToys[k] (16-byte reward records) and mNumTotalRewards
 * counts them. There is no check against the 28 entries the object has room for.
 */
extern "C" void cFEStateRewardGalleryToys_allocRewards(cFEStateRewardGalleryToys* self)
{
    int n = cBERewardInterface_getTotalNumToys(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBToys = cBERewardInterface_getDB()->mpToys;
    for (int i = 0; i < n; i++) {
        self->mpToys[self->mNumTotalRewards++] = &self->mpDBToys[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryToys_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the toy gallery's reward list: mNumTotalRewards, mpDBToys and the 28 entries of mpToys.
 *
 * Vtable slot 29 of the toy vtable cFEStateRewardGalleryToys_vtbl.
 */
extern "C" void cFEStateRewardGalleryToys_purgeRewards(cFEStateRewardGalleryToys* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBToys = 0;
    for (i = 27; i >= 0; i--) {
        self->mpToys[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars);
#ifdef SKIP_ASM
extern "C" void* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(void* self, void* engine, signed char idx, int a3, int kind);
extern void* cFEStateRewardGalleryCheatChars_vtbl[];

/**
 * Constructs the cheat-character reward gallery (reward type 8), which cFECustom_getNextState allocates (2712 bytes)
 * under the tag "cFEStateRewardGalleryCheatChars".
 *
 * Runs the gallery base constructor cFEStateRewardGalleryBase_cFEStateRewardGalleryBase() with type 8, installs vtable cFEStateRewardGalleryCheatChars_vtbl and clears the list base
 * (+0xA24) and the 28 list entries from +0xA28. This gallery keeps the base icon-file and icon-id slots (no icon file)
 * and supplies texture elements instead (slot 27, cFEStateRewardGalleryCheatChars_getTextureElement).
 *
 * @param engine  the UI engine
 * @param idx     user number
 * @param inStore      nonzero when opened from the lodge (cFECustom_getNextState passes gInLodge); cFEStateRewardGalleryBase_cFEStateRewardGalleryBase then sets
 *                the store number (+0x8F0) from the player's last hub
 * @return @p self
 */
extern "C" void* cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars(cFEStateRewardGalleryCheatChars* self, cUIEngine* engine, signed char idx, int inStore)
{
    cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(self, engine, idx, inStore, 8);
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryCheatChars_vtbl;
    self->mpDBCheatChars = 0;
    for (int i = 27; i >= 0; i--) {
        self->mpCheatChars[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryCheatChars_allocRewards);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBERewardInterface_getTotalNumCheatChars(void* self);
extern "C" cBERewardDB* cBERewardInterface_getDB();

/**
 * Builds the cheat-character gallery's reward list from the reward database.
 *
 * Vtable slot 28 of the cheat-character vtable cFEStateRewardGalleryCheatChars_vtbl. The count comes from
 * cBERewardInterface_getTotalNumCheatChars() on the reward interface (kind 13); mpDBCheatChars is the database's
 * mpCheatChars (cBERewardInterface_getDB()); entry k of mpCheatChars gets &mpDBCheatChars[k] (16-byte reward records)
 * and mNumTotalRewards counts them. There is no check against the 28 entries the object has room for.
 */
extern "C" void cFEStateRewardGalleryCheatChars_allocRewards(cFEStateRewardGalleryCheatChars* self)
{
    int n = cBERewardInterface_getTotalNumCheatChars(cBE_getInterface_Fv(cBE_getBE(), 13));
    self->mNumTotalRewards = 0;
    self->mpDBCheatChars = cBERewardInterface_getDB()->mpCheatChars;
    for (int i = 0; i < n; i++) {
        self->mpCheatChars[self->mNumTotalRewards++] = &self->mpDBCheatChars[i];
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryCheatChars_purgeRewards);
#ifdef SKIP_ASM
/**
 * Clears the cheat-character gallery's reward list: mNumTotalRewards, mpDBCheatChars and the 28 entries of
 * mpCheatChars.
 *
 * Vtable slot 29 of the cheat-character vtable cFEStateRewardGalleryCheatChars_vtbl.
 */
extern "C" void cFEStateRewardGalleryCheatChars_purgeRewards(cFEStateRewardGalleryCheatChars* self)
{
    int i;
    self->mNumTotalRewards = 0;
    self->mpDBCheatChars = 0;
    for (i = 27; i >= 0; i--) {
        self->mpCheatChars[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryCheatChars_getTextureElement);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);

/**
 * Looks up the texture element shown for a cheat character: the reward's name hashed and found in the UI engine's
 * texture bank.
 *
 * Vtable slot 27 of the cheat-character vtable cFEStateRewardGalleryCheatChars_vtbl (the other galleries keep the base slot). Hashes the string
 * at +4 of the reward record in list entry @p i (+0xA28) with GetHashValue32() and looks it up with
 * cUITextureBank_getElementByHashname() in mpEngine + 0x58. The C definition is void, but the element stays in the
 * return register, and cFEStateRewardGalleryBase_updateHighlither stores it in the highlighted cell's mpTextureElem.
 *
 * @param i  index into the gallery's reward list
 */
extern "C" void cFEStateRewardGalleryCheatChars_getTextureElement(cFEStateRewardGalleryCheatChars* self, int i)
{
    cUIEngine* obj = self->mpEngine;
    tCheatCharInfo* reward = (tCheatCharInfo*)((void*)self->mpCheatChars[i]);
    int textureHash = GetHashValue32(reward->mpIconName);
    cUITextureBank_getElementByHashname(&obj->mTextureBank, textureHash);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_cFEStatePreviewReward);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
extern void* cFEStatePreviewReward_vtbl[];
struct sRewardState_1AA0 {
    char pad_0x0[0x48];
    int a[3];   // 0x48
    int b[3];   // 0x54
};

/**
 * Constructs the reward preview state (vtable cFEStatePreviewReward_vtbl, state id 0x30) for a user.
 *
 * Runs the cUIState constructor cUIState_cUIState1(), sets mHashName to 0x30, the vtable, mUserNum and mCtrlBits
 * (cFECustom_getPlayerCtrlMask on *mpEngine); clears the three bitmap pointers (+0x48) and the three shape-frame pointers (+0x54) and
 * the reward (+0x60); stores the player's character id (cBENewPlayerInterface_getPlayerCharID, interface kind 1) in
 * +0x64; and sets the reward type (+0x68), the file slot (+0x6C), the texture handle (+0x70) and the shape index
 * (+0x74) to -1. Called by cFEStateRewardGalleryBase_showReward.
 *
 * @param engine  the UI engine
 * @param owner   the parent state (the reward gallery)
 * @param player  user number
 * @return @p self
 */
extern "C" void* cFEStatePreviewReward_cFEStatePreviewReward(cFEStatePreviewReward* self, cUIEngine* engine, cUIState* owner, int player)
{
    signed char idx = player;
    int i;
    cUIState_cUIState1(self, engine, owner);
    self->mHashName = 0x30;
    VPTR_OF(cListNode, self) = cFEStatePreviewReward_vtbl;
    self->mUserNum = idx;
    self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, idx);
    for (i = 0; i < 3; i++) {
        ((sRewardState_1AA0*)self)->a[i] = 0;
        ((sRewardState_1AA0*)self)->b[i] = 0;
    }
    self->mpReward = 0;
    self->mCharID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1),
                                                                         self->mUserNum);
    self->mRewardType = -1;
    self->mTextureFileIndex = -1;
    self->mTextureID = -1;
    self->mOrientation = -1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward__cFEStatePreviewReward);
#ifdef SKIP_ASM

extern void* cFEStatePreviewReward_vtbl[];
extern void* cFEState_vtbl[];
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void cUIState__cUIState(void* self, int flags);

/**
 * Destroys a reward preview: frees its texture and releases its file slot (func_0019DC20), then the base
 * state.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStatePreviewReward__cFEStatePreviewReward(cFEStatePreviewReward* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStatePreviewReward_vtbl;
    int id = self->mTextureID;
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->RemoveTexture(id);
    }
    int slot = self->mTextureFileIndex;
    if (slot >= 0) {
        func_0019DC20(SSXApp->RealFE->mBoltFileManager, self->mCharID, slot);
    }
    self->mTextureFileIndex = -1;
    self->mTextureID = -1;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_130rewardposter[];
extern char str_Menu[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

/**
 * Creates the reward preview screen ("130rewardposter") and sets flag 8 on its "Menu" object.
 */
extern "C" void cFEStatePreviewReward_onCreateScreen(cFEStatePreviewReward* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_130rewardposter), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu));
    if (((void*)obj) != 0) {
        *(int*)&obj->mAttrMenu |= 8;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetCreate);
#ifdef SKIP_ASM
extern char str_Poster_icon[];
extern char str_poster_icon[];
extern char str_trading_card_icon[];
extern char str_art_icon[];
extern char str_toys_icon[];
extern char str_screentitle[];
extern char str_kT_OVRCMNPosters[];
extern char str_kT_OVRCMNTradeCard[];
extern char str_kT_OVRCMNArt[];
extern char str_kT_OVRCMNVideo[];
extern char str_kT_OVRCMNToys[];
extern char str_reward_name[];
extern char str_Square_shape[];
extern char str_Horizontal_shape[];
extern char str_Vertical_shape[];
extern char str_Square_bitmap[];
extern char str_Horizontal_bitmap[];
extern char str_Vertical_bitmap[];


struct sPreviewReward_1C98 {
    char pad_0x0[0x10];
    char* ui;                   // 0x10
    char pad_0x14[0x48 - 0x14];
    cUIIcon* items[3];     // 0x48
    cUIIcon* frames[3];    // 0x54
    char** reward;              // 0x60
    char pad_0x64[0x68 - 0x64];
    int type;                   // 0x68
    char pad_0x6C[0x70 - 0x6C];
    int f70;                    // 0x70
    int sel;                    // 0x74
};

/**
 * Binds the preview screen's widgets as they are created: icon and title by reward type, the name, and the
 * frames.
 *
 * The reward picture goes into the square, horizontal or vertical bitmap chosen by the shape index (+0x70
 * texture),
 * and that frame is shown.
 *
 * @param w  the widget being created, or 0
 */
extern "C" void cFEStatePreviewReward_onWidgetCreate(sPreviewReward_1C98* self, cUIIcon* w)
{
    if (w == 0)
        return;

    int hash = w->mHashName;
    if (hash == GetHashValue32(str_Poster_icon)) {
        switch (self->type) {
        case 3:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int iconHash = GetHashValue32(str_poster_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
            }
            w->mTextureID = -1;
            break;
        case 4:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int iconHash = GetHashValue32(str_trading_card_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
            }
            w->mTextureID = -1;
            break;
        case 5:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int iconHash = GetHashValue32(str_art_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
            }
            w->mTextureID = -1;
            break;
        case 7:
            {
                cUIEngine* ui = (cUIEngine*)self->ui;
                int iconHash = GetHashValue32(str_toys_icon);
                (*(void**)&w->mpTextureElem) = cUITextureBank_getElementByHashname(&ui->mTextureBank, iconHash);
            }
            w->mTextureID = -1;
            break;
        }
    } else if (hash == GetHashValue32(str_screentitle)) {
        switch (self->type) {
        case 3:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNPosters));
            break;
        case 4:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNTradeCard));
            break;
        case 5:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNArt));
            break;
        case 6:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNVideo));
            break;
        case 7:
            cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_OVRCMNToys));
            break;
        }
    } else if (hash == GetHashValue32(str_reward_name)) {
        switch (self->type) {
        case 4:
            w->setVisible(0);
            break;
        case 3:
        case 5:
        case 7:
            cUIText_setAsciiString((cUIText*)w, *self->reward);
            break;
        case 6:
            break;
        }
    } else if (hash == GetHashValue32(str_Square_shape)) {
        self->frames[0] = w;
        w->setVisible(0);
    } else if (hash == GetHashValue32(str_Horizontal_shape)) {
        self->frames[1] = w;
        w->setVisible(0);
    } else if (hash == GetHashValue32(str_Vertical_shape)) {
        self->frames[2] = w;
        w->setVisible(0);
    } else if (hash == GetHashValue32(str_Square_bitmap)) {
        self->items[0] = w;
    } else if (hash == GetHashValue32(str_Horizontal_bitmap)) {
        self->items[1] = w;
    } else if (hash == GetHashValue32(str_Vertical_bitmap)) {
        self->items[2] = w;
    }
    int sel = self->sel;
    if (sel >= 0) {
        cUIIcon* item = self->items[sel];
        if (item != 0 && self->frames[sel] != 0) {
            item->mTextureID = self->f70;
            (*(void**)&item->mpTextureElem) = 0;
            cUIIcon* frame = self->frames[self->sel];
            frame->setVisible(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu events 6, 8 and 9 on the preview screen.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param event  menu event number (On Tour Event)
 * @return 0x100 for events 6, 8 and 9, otherwise 0x101
 */
extern "C" int cFEStatePreviewReward_onWidgetMenu(cFEStatePreviewReward* self, int menu, unsigned int event)
{
    switch (event) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStatePreviewReward_updateTextureLoad(cFEStatePreviewReward* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the reward preview, first finishing its preview picture load while one is pending.
 *
 * While the async slot at 0x6C is not negative, cFEStatePreviewReward_updateTextureLoad() checks the load; once it has loaded, the shape
 * is handed to GrMan, the slot is released and set to -1 and the preview widget shows it. Then runs the base
 * UI-state update cUIState_onUpdate().
 */
extern "C" void cFEStatePreviewReward_onUpdate(cFEStatePreviewReward* self)
{
    if (self->mTextureFileIndex >= 0) {
        self->updateTextureLoad();
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onInputBegin);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern char str_TransitionOut[];
struct sVE_1FF0a {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVE_1FF0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sDefObj_1FF0 {
    sVE_1FF0b* vt;
};
// $gp-relative default object (target has raw `addiu $3,$28,0x2968`, no symbol)
extern sDefObj_1FF0 gUISoundNull;

/**
 * Closes the preview when the input passes its vtable slot-6 test: plays UI sound 3, hides the frames,
 * transitions out.
 *
 * @param obj  the input (On Tour cUIInput* pInput)
 * @return 1 when the preview closes, otherwise 0
 */
extern "C" int cFEStatePreviewReward_onInputBegin(cFEStatePreviewReward* self, cUIInput* obj)
{
    if (obj->isPrevious() != 0) {
    sDefObj_1FF0* sound = *(sDefObj_1FF0**)&self->mpEngine->mpSound;
    if (sound == 0) {
        sound = &gUISoundNull;
    }
    if (sound != 0) {
        sound->vt[2].fn(vthis(sound, sound->vt[2]), 3);
    }
    int i;
    for (i = 0; i < 3; i++) {
        cUIIcon* bitmap = self->mpIcon[i];
        if (bitmap != 0) {
            bitmap->setVisible(0);
        }
    }
    cUIScreen_playFrame(self->mpScreen, cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut)), 1);
    cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_setReward);
#ifdef SKIP_ASM
extern "C" void cFEStatePreviewReward_loadTexture(cFEStatePreviewReward* self, const char* fileName);

/**
 * Sets the reward the preview shows and starts loading its picture.
 *
 * Stores @p kind in mRewardType and @p data in mpReward; for types 3, 4, 5 and 7 (posters, trading cards, art, toys)
 * loads the record's mpTextureFileName with cFEStatePreviewReward_loadTexture(). Videos (type 6) and other types load
 * nothing. Called by cFEStateRewardGalleryBase_showReward.
 *
 * @param kind  the reward type (3 posters, 4 trading cards, 5 art, 6 videos, 7 toys)
 * @param data  the 16-byte reward record
 */
void cFEStatePreviewReward::setReward(int kind, void* data)
{
    mRewardType = kind;
    mpReward = data;
    switch (kind) {
    case 3:
        loadTexture(((tPosterInfo*)data)->mpTextureFileName);
        break;
    case 4:
        loadTexture(((tTradingCardInfo*)data)->mpTextureFileName);
        break;
    case 5:
        loadTexture(((tConceptArtInfo*)data)->mpTextureFileName);
        break;
    case 7:
        loadTexture(((tToyInfo*)data)->mpTextureFileName);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_loadTexture);
#ifdef SKIP_ASM
extern "C" int func_0019DA10(void* self, int bank, const char* name, int a3, int a4, int a5);

/**
 * Requests the preview's picture file from the FE file table and keeps the slot in +0x6C.
 *
 * Calls func_0019DA10() on SSXApp->RealFE + 0x1A70 in the bank of the player's character (+0x64) with the name and
 * arguments 9, 1 and 0.
 *
 * @param fileName  the picture file name (the reward record's mpTextureFileName)
 */
void cFEStatePreviewReward::loadTexture(const char* fileName)
{
    mTextureFileIndex = func_0019DA10(SSXApp->RealFE->mBoltFileManager, mCharID, fileName, 9, 1, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_updateTextureLoad);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* self, int bank, int i);
extern "C" void func_0019DC20(void* self, int bank, int i);

struct sSelfK1D2198 {
    char pad_0x0[0x48];
    char* a48[3];   // 0x48
    char* a54[3];   // 0x54
    int f60;
    int bank;       // 0x64
    int f68;
    int slot;       // 0x6C
    int handle;     // 0x70
    int kind;       // 0x74
};
struct sVEK1D2198 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Finishes the preview picture load: registers the picture with GrMan, picks the square, horizontal or vertical frame
 * by its shape and shows the picture there.
 *
 * Returns at once until func_0019E238() reports the slot in +0x6C (bank +0x64 of the FE file table) ready. Then takes
 * the first shape of the loaded file (func_0019E2B0): its width (+4) and height (+6) set the shape index +0x74 (1 when
 * wider, 2 when taller, 0 when square); SHAPE_name() copies its name and GrMan's virtual v46 (as the view declares it)
 * registers it, the handle going into +0x70. The file slot is released (func_0019DC20) and set to -1. When the bitmap
 * (+0x48) and the frame (+0x54) for that shape both exist, the bitmap's mTextureID becomes the handle with
 * mpTextureElem 0 and the frame is shown (setVisible 1). Called by cFEStatePreviewReward_onUpdate while the slot is >=
 * 0.
 */
void cFEStatePreviewReward::updateTextureLoad()
{
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    if (func_0019E238(mgr, mCharID, mTextureFileIndex) == 0)
        return;
    SHAPEFILE* data = func_0019E2B0(mgr, mCharID, mTextureFileIndex);
    char* shapeHeader = (char*)data + ((sDataEnt001CFFF8*)&data->tbl[0].offset)[0].off;
    short shapeWidth = *(short*)(shapeHeader + 4);
    short shapeHeight = *(short*)(shapeHeader + 6);
    if (shapeHeight < shapeWidth)
        mOrientation = 1;
    else if (shapeWidth < shapeHeight)
        mOrientation = 2;
    else
        mOrientation = 0;
    tIconInfo ent;
    SHAPE_name(data, 0, &ent);
    ent.mName[4] = 0;
    mTextureID = GrMan->AddTexture((const SHAPE*)shapeHeader, (const char*)&ent, 0, 1, -1);
    func_0019DC20(mgr, mCharID, mTextureFileIndex);
    mTextureFileIndex = -1;
    sSelfK1D2198* previewState = (sSelfK1D2198*)this;
    char* bitmapWidget = previewState->a48[previewState->kind];
    if (bitmapWidget != 0 && previewState->a54[previewState->kind] != 0) {
        *(int*)(bitmapWidget + 0x78) = previewState->handle;
        *(int*)(bitmapWidget + 0x7C) = 0;
        char* frameWidget = previewState->a54[previewState->kind];
        sVEK1D2198* vt = *(sVEK1D2198**)(frameWidget + 8);
        vt[9].fn(vthis(frameWidget, vt[9]), 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_cFEStateVideoPlayer);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void tMoviePlayOptions_SetDefaults(void* p);
extern void* cFEStateVideoPlayer_vtbl[];

/**
 * Constructs the full-screen video player state (state id 6, vtable cFEStateVideoPlayer_vtbl) used for reward videos and bonus
 * material.
 *
 * Runs the cUIState constructor cUIState_cUIState1(), sets mUserNum, empties the video (+0x48) and audio (+0x148) file
 * names, clears the movie player (+0x248) and the updated, rendered and stop-request flags (+0x274, +0x278, +0x27C),
 * enables input (+0x270 = 1), sets mHashName to 6, initialises the movie settings block at +0x24C (tMoviePlayOptions_SetDefaults) and
 * points its first two words at the two file-name buffers. cFEStateRewardGalleryBase_showReward and
 * cFEStateBonusMaterial_onWidgetEvent allocate it (640 bytes) under the tag "cFEStateVideoPlayer".
 *
 * @param vself   the memory for the state
 * @param engine  the UI engine
 * @param owner   the parent state
 * @param player  user number
 * @return @p vself
 */
extern "C" void* cFEStateVideoPlayer_cFEStateVideoPlayer(cFEStateVideoPlayer* vself, cUIEngine* engine, cUIState* owner, signed char player)
{
    cUIState_cUIState1(((char*)vself), engine, owner);
    char* title = vself->mVideoFileName;
    char* body = vself->mAudioFileName;
    vself->mUserNum = player;
    title[0] = 0;
    body[0] = 0;
    vself->mUpdatedOnce = 0;
    vself->mRenderOnce = 0;
    vself->mStopPending = 0;
    vself->mpMoviePlayer = 0;
    VPTR_OF(cListNode, vself) = cFEStateVideoPlayer_vtbl;
    vself->mHashName = 6;
    vself->mInterruptible = 1;
    tMoviePlayOptions_SetDefaults(&vself->mOptions);
    *(char**)&vself->mOptions = title;
    vself->mOptions.sAudioFileName = (const char*)body;
    return ((char*)vself);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer__cFEStateVideoPlayer);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int a1);
extern "C" void cUIState__cUIState(void* self, int flags);
extern void* cFEStateVideoPlayer_vtbl[];
extern void* cFEState_vtbl[];

/**
 * Destroys the video player state: deletes its movie player, then runs the cUIState destructor.
 *
 * Vtable slot 1 of cFEStateVideoPlayer_vtbl. Reinstalls cFEStateVideoPlayer_vtbl, deletes the movie player (+0x248) with cMoviePlayer__cMoviePlayer(player, 3)
 * when there is one, installs cFEState's vtable cFEState_vtbl and calls cUIState__cUIState() with @p flags.
 *
 * @param flags  g++ destructor flags, passed on to cUIState__cUIState (bit 0 frees the object's memory)
 */
extern "C" void cFEStateVideoPlayer__cFEStateVideoPlayer(cFEStateVideoPlayer* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateVideoPlayer_vtbl;
    if (self->mpMoviePlayer != 0) {
        cMoviePlayer__cMoviePlayer(self->mpMoviePlayer, 3);
    }
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cMoviePlayer_cMoviePlayer(void* mem);
extern "C" void cFEStateVideoPlayer_Start(cFEStateVideoPlayer* self);
extern char str_MoviePlayer[];

/**
 * Creates the video player's movie player and starts playback.
 *
 * Vtable slot 4 of cFEStateVideoPlayer_vtbl (the create-screen slot; no screen is created). Allocates 68 bytes under the tag
 * "MoviePlayer", constructs them with cMoviePlayer_cMoviePlayer() into +0x248 and calls cFEStateVideoPlayer_Start().
 */
extern "C" void cFEStateVideoPlayer_onCreateScreen(cFEStateVideoPlayer* self)
{
    self->mpMoviePlayer = (cMoviePlayer*)cMoviePlayer_cMoviePlayer(cMemMan_alloc(68, str_MoviePlayer, 0, 0));
    self->Start();
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int a1);

/**
 * Deletes the video player's movie player (+0x248) with cMoviePlayer__cMoviePlayer(player, 3), when there is one, and clears the
 * pointer.
 *
 * Vtable slot 5 of cFEStateVideoPlayer_vtbl.
 */
extern "C" void cFEStateVideoPlayer_onDestroyScreen(cFEStateVideoPlayer* self)
{
    void* moviePlayer = self->mpMoviePlayer;
    if (moviePlayer != 0) {
        cMoviePlayer__cMoviePlayer(moviePlayer, 3);
    }
    self->mpMoviePlayer = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_onRender2D);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncRender(void* p, int a1);
extern "C" void* cUIState_onRender2D(void* self);

/**
 * Draws the playing movie: with a movie player (+0x248), sets the rendered flag (+0x278) and calls
 * cMoviePlayer_asyncRender(player, 0); then runs cUIState_onRender2D().
 *
 * Vtable slot 15 of cFEStateVideoPlayer_vtbl.
 */
extern "C" void cFEStateVideoPlayer_onRender2D(cFEStateVideoPlayer* self)
{
    void* moviePlayer = self->mpMoviePlayer;
    if (moviePlayer != 0) {
        self->mRenderOnce = 1;
        cMoviePlayer_asyncRender(moviePlayer, 0);
    }
    cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_onUpdate);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncUpdate(void* p);
extern "C" void cFEStateVideoPlayer_Stop(cFEStateVideoPlayer* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Steps the movie each frame and stops the player once the movie ends or a requested stop has been drawn.
 *
 * Vtable slot 12 of cFEStateVideoPlayer_vtbl. With a movie player (+0x248): sets the updated flag (+0x274), runs
 * cMoviePlayer_asyncUpdate(), and calls cFEStateVideoPlayer_Stop() when a stop was requested (+0x27C) and a frame has been drawn
 * since playback started (+0x278), or when the player's first word (its status) is nonzero. Then runs
 * cUIState_onUpdate().
 */
extern "C" void cFEStateVideoPlayer_onUpdate(cFEStateVideoPlayer* self)
{
    void* moviePlayer = self->mpMoviePlayer;
    if (moviePlayer != 0) {
        self->mUpdatedOnce = 1;
        cMoviePlayer_asyncUpdate(moviePlayer);
        if ((self->mStopPending != 0 && self->mRenderOnce != 0)
            || *(int*)self->mpMoviePlayer != 0) {
            self->Stop();
        }
    }
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_onInputBegin);
#ifdef SKIP_ASM
struct sVEntry001D2518 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int vcall001D2518(void* obj, int slot)
{
    sVEntry001D2518* vt = *(sVEntry001D2518**)((char*)obj + 8);
    return vt[slot].fn(vthis(obj, vt[slot]));
}

/**
 * Requests a stop of the video when the accept or back input (tests 5 and 6, vtable slots of @p obj) fires while input
 * is enabled (+0x270).
 *
 * Vtable slot 8 of cFEStateVideoPlayer_vtbl. Sets the stop request (+0x27C); cFEStateVideoPlayer_onUpdate acts on it.
 *
 * @param obj  the input object
 * @return 1 when a stop was requested, otherwise 0
 */
extern "C" int cFEStateVideoPlayer_onInputBegin(cFEStateVideoPlayer* self, cUIInput* obj)
{
    if (self->mInterruptible != 0) {
        if (vcall001D2518(obj, 5) != 0 || vcall001D2518(obj, 6) != 0) {
            self->mStopPending = 1;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_setVideoSourceFiles);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Sets the files the video player plays: copies @p videoFile into +0x48 and @p audioFile into +0x148, or empties the
 * latter when it is 0.
 *
 * strcpy() without a length check into 256-byte buffers. Called by cFEStateRewardGalleryBase_showReward and
 * cFEStateBonusMaterial_onWidgetEvent right after the constructor cFEStateVideoPlayer_cFEStateVideoPlayer.
 *
 * @param videoFile  the video file name
 * @param audioFile  the audio file name, or 0 for none
 */
void cFEStateVideoPlayer::setVideoSourceFiles(const char* videoFile, const char* audioFile)
{
    strcpy((char*)&mVideoFileName, videoFile);
    if (audioFile != 0) {
        strcpy((char*)&mAudioFileName, audioFile);
    } else {
        *((char*)&mAudioFileName) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_Start);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002B3A70(void* p);
extern "C" int cMoviePlayer_init(void* p, void* q);
extern "C" void cMoviePlayer_asyncPlay(void* p);

/**
 * Starts the movie: turns the current song down, then opens and starts the movie player with the source files.
 *
 * Calls func_002B3A70() on cAudioInterface_bxAudio() + 0x118 (it sets the current song's level to 0), sets the word at +0x254 of
 * the settings block to 1, opens the movie with cMoviePlayer_init() and the settings block (+0x24C), starts it with
 * cMoviePlayer_asyncPlay(), and clears the rendered (+0x278) and updated (+0x274) flags. Called by cFEStateVideoPlayer_onCreateScreen.
 */
void cFEStateVideoPlayer::Start()
{
    func_002B3A70(&cAudioInterface_bxAudio()->MusicSys);
    mOptions.bSoundOn = 1;
    cMoviePlayer_init(mpMoviePlayer, (char*)&mOptions);
    cMoviePlayer_asyncPlay(mpMoviePlayer);
    mRenderOnce = 0;
    mUpdatedOnce = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", cFEStateVideoPlayer_Stop);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002B3A98(void* self);
extern "C" void cUIStateStack_pop(void* self, int a1);

/**
 * Stops the video player: restores the current song and closes the front state.
 *
 * Calls func_002B3A98() on cAudioInterface_bxAudio() + 0x118 (it sets the current song's level back to 0x1000), clears the stop
 * request (+0x27C), and marks the front state on the engine's state stack (mpEngine + 0x18) pending delete in mode 6
 * with cUIStateStack_pop(stack, 1). Called by cFEStateVideoPlayer_onUpdate.
 */
void cFEStateVideoPlayer::Stop()
{
    func_002B3A98(&cAudioInterface_bxAudio()->MusicSys);
    mStopPending = 0;
    cUIStateStack_pop(&mpEngine->mStateStack, 1);
}
#endif

