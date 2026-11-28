#include "common.h"
#include "be/beinterface.h"
#include "be/beintnewrace.h"
#include "vtable.h"
#include "be/bedbprofile.h"
#include "be/bedbbolt.h"
#include "fe/feboltfilemanager.h"
#include "ealib/hlafile.h"
#include "ui/uiicon.h"
#include "ealib/shape.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "ui/uimenu.h"
#include "fe/uistatebuypopup.h"
#include "fe/ferider.h"
#include "fe/festatecharequip.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"
#include "ui/uiinput.h"

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cFEStateCharEquip);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int cBELibrary_getRiderIndex(int index);
int GetHashValue32(char* str);
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern "C" int cUITextureBank_getElementByHashname(void* list, int hash);
extern "C" void cFERiderManager_setHumanCharacter(void* mgr, int idx, int ch, int a3);
extern "C" int cBEEconInterface_getMoney(void* iface, int idx, int ch);
extern "C" void cBEBoltInterface_storeVisibleFlags(void* self, int a1, int a2);
extern "C" int cBEBoltInterface_getUsedEquipPoints(void* self, int a1, int a2);
extern "C" char* cBEBoltInterface_getBigIconFileName(void* iface, int rider);
extern "C" int cBENewPlayerInterface_getLastHub(void* iface, int a1);
extern "C" void cFEStateCharEquip_buildItemList(void* self, int id);
extern "C" int cFEBoltFileManager_requestFile(void* self, int bank, void* name, int a3, int a4, int a5);
extern const int kBolt_None_c __asm__("kBolt_None");
extern void* cFEStateCharEquip_vtbl[];
extern char str_dollargreen[];
extern char str_dollarred[];
extern char str_checkbox[];
extern char str_empty_box[];

struct sVec4_8DB8 {
    float x, y, z, w;
};

struct sCharEquip_8DB8 {
    char pad0[0x8];
    void** vtbl;             // 0x8
    int fC;                  // 0xC
    char** mgr;              // 0x10
    char pad14;
    unsigned char f15;       // 0x15
    char pad16[0x44 - 0x16];
    signed char team;        // 0x44
    char pad45[3];
    int f48[11];             // 0x48..0x70
    int f74;                 // 0x74
    int f78;                 // 0x78
    int f7C;                 // 0x7C
    int f80;                 // 0x80
    int f84;                 // 0x84
    int f88;                 // 0x88
    int a8C[6];              // 0x8C
    int aA4[6];              // 0xA4
    int slot;                // 0xBC
    int fC0;                 // 0xC0
    int fC4;                 // 0xC4
    int fC8;                 // 0xC8
    int fCC;                 // 0xCC
    char bufD0[0x50];        // 0xD0
    int f120;                // 0x120
    char pad124[0x958 - 0x124];
    int f958;                // 0x958
    int f95C;                // 0x95C
    char f960;               // 0x960
    char pad961[0xA60 - 0x961];
    int fA60;                // 0xA60
    int fA64;                // 0xA64
    int fA68;                // 0xA68
    int fA6C;                // 0xA6C
    int fA70;                // 0xA70
    int fA74;                // 0xA74
    int aA78[6];             // 0xA78
    int fA90;                // 0xA90
    int fA94;                // 0xA94
    int fA98;                // 0xA98
    int fA9C;                // 0xA9C
    int fAA0;                // 0xAA0
    char padAA4[0xAB0 - 0xAA4];
    int fAB0;                // 0xAB0
    char padAB4[0xAC0 - 0xAB4];
    sVec4_8DB8 camPos;       // 0xAC0
    sVec4_8DB8 camTarget;    // 0xAD0
    int fAE0;                // 0xAE0
    int fAE4;                // 0xAE4
};

static inline int BoardClass_8DB8(int kind)
{
    switch (kind) {
    case 17:
        return 1;
    case 18:
        return 1;
    case 19:
        return 2;
    case 20:
        return 2;
    case 21:
        return 3;
    }
    return 1;
}

static inline int Find_8DB8(cUIEngine* mgr, int hash)
{
    return cUITextureBank_getElementByHashname(&mgr->mTextureBank, hash);
}

/**
 * Constructs the equip-gear state for a user in store (buy) or inventory (equip) mode.
 *
 * Sets state id 0xE, the two preview positions (0xAC0, 0xAD0), the user's controller bits, the character
 * (0xBC), the mode (0xC8: 1 in the store), money (0xC0) and equip points (0xC4, cBEBoltInterface_getUsedEquipPoints); clears the
 * widget pointers and the category stack (root -1), finds the dollar and checkbox texture elements and sets
 * the user's FE rider to the character (cFERiderManager_setHumanCharacter).
 *
 * With a store or a mode switch, 0xCC (On Tour mStoreNum) is the last hub's peak (2 for kLOCATION_HUB_C / _D, 3
 * for kLOCATION_HUB_E, else 1), otherwise -1. Builds the root item list (cFEStateCharEquip_buildItemList) and loads the character's icon
 * file into the FE file cache (slot kept at 0xA64).
 *
 * @param mem  the state being constructed (this)
 * @param owner  the UI engine (On Tour pEngine)
 * @param team  the user number (On Tour userNum)
 * @param inStore  nonzero to open in store mode (buying), zero for the inventory (equipping)
 * @param canSwitchMode  nonzero when the option button may switch between the two modes (kept at 0xAE4)
 * @return the constructed state
 */
extern "C" void* cFEStateCharEquip_cFEStateCharEquip(cFEStateCharEquip* mem, cUIEngine* owner, signed char team, int inStore, int canSwitchMode)
{
    sCharEquip_8DB8* self = (sCharEquip_8DB8*)mem;
    cUIState_cUIState(self);
    self->vtbl = cFEStateCharEquip_vtbl;
    self->camPos.x = -105.0f;
    self->camPos.y = -100.0f;
    self->camPos.z = -80.0f;
    self->camPos.w = 1.0f;
    self->camTarget.x = 265.75f;
    self->camTarget.y = -1073.5f;
    self->camTarget.z = -232.0f;
    self->camTarget.w = 1.0f;
    self->fC = 0xE;
    self->team = team;
    self->f15 = cFECustom_getPlayerCtrlMask(*self->mgr, team);
    void* player = cBE_getInterface(cBE_getBE(), 1);
    self->slot = cBENewPlayerInterface_getPlayerCharID(player, self->team);
    if (inStore != 0) {
        self->fC8 = 1;
    } else {
        self->fC8 = 0;
    }
    self->f958 = 0;
    self->f960 = 0;
    self->fA60 = 0;
    self->f48[0] = 0;
    self->f48[1] = 0;
    self->f48[2] = 0;
    self->f48[3] = 0;
    self->f48[4] = 0;
    self->f48[5] = 0;
    self->f48[6] = 0;
    self->f48[7] = 0;
    self->f48[8] = 0;
    self->f48[9] = 0;
    self->f48[10] = 0;
    self->f78 = 0;
    self->f7C = 0;
    self->fAB0 = 0;
    self->f95C = -1;
    self->fA64 = -1;
    self->fA68 = -1;
    self->fA6C = -1;
    self->fA70 = -1;
    self->fA74 = -1;
    self->fAE0 = 1;
    MEM_fill(self->bufD0, kBolt_None_c, 80);
    self->f120 = 0;
    for (int i = 0; i < 6; i++) {
        self->a8C[i] = 0;
        self->aA4[i] = 0;
        self->aA78[i] = -99999;
    }
    self->f80 = 0;
    self->f84 = 0;
    self->f88 = 0;
    {
        cUIEngine* mgr = (cUIEngine*)self->mgr;
        self->fA90 = Find_8DB8(mgr, GetHashValue32(str_dollargreen));
    }
    {
        cUIEngine* mgr = (cUIEngine*)self->mgr;
        self->fA94 = Find_8DB8(mgr, GetHashValue32(str_dollarred));
    }
    {
        cUIEngine* mgr = (cUIEngine*)self->mgr;
        self->fA98 = Find_8DB8(mgr, GetHashValue32(str_checkbox));
    }
    {
        cUIEngine* mgr = (cUIEngine*)self->mgr;
        self->fA9C = Find_8DB8(mgr, GetHashValue32(str_empty_box));
    }
    self->fAA0 = 0;
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, self->team, self->slot, 1);
    self->fC0 = cBEEconInterface_getMoney(cBE_getInterface(cBE_getBE(), 11), self->team, self->slot);
    void* shop = cBE_getInterface(cBE_getBE(), 9);
    cBEBoltInterface_storeVisibleFlags(shop, self->team, self->slot);
    self->fC4 = cBEBoltInterface_getUsedEquipPoints(shop, self->team, self->slot);
    self->fAE4 = canSwitchMode;
    if (inStore != 0 || canSwitchMode != 0) {
        switch (cBENewPlayerInterface_getLastHub(player, cBELibrary_getRiderIndex(self->team))) {
        case kLOCATION_HUB_A:
            self->fCC = 1;
            break;
        case kLOCATION_HUB_B:
            self->fCC = 1;
            break;
        case kLOCATION_HUB_C:
            self->fCC = 2;
            break;
        case kLOCATION_HUB_D:
            self->fCC = 2;
            break;
        case kLOCATION_HUB_E:
            self->fCC = 3;
            break;
        default:
            self->fCC = 1;
            break;
        }
    } else {
        self->fCC = -1;
    }
    cFEStateCharEquip_buildItemList(self, kBolt_None_c);
    char* name = cBEBoltInterface_getBigIconFileName(shop, self->slot);
    self->fA64 = cFEBoltFileManager_requestFile(SSXApp->RealFE->mBoltFileManager, self->slot, name, 9, 1, 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip__cFEStateCharEquip);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_releaseFile(cFEBoltFileManager* self, int bank, int i);
extern "C" void cFEStateCharEquip_cancelLargePreviewLoad(cFEStateCharEquip* self);
extern "C" void cUIState__cUIState(void* self, int flags) __asm__("_$_8cUIState");
extern void* cFEStateCharEquip_vtbl[];
extern void* cFEState_vtbl[];

struct sVEntryK199148 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void releaseK199148(int id)
{
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryK199148* vt = (sVEntryK199148*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, id);
    }
}

/**
 * Destroys the equip-gear state: frees its icon file and preview textures, then runs the base destructor.
 *
 * Releases the icon file (0xA64) from the FE file cache and the texture ids at 0xA68, 0xA70 and 0xA74 through
 * the graphics manager (vtable slot 50), cancels a pending large-preview load (cFEStateCharEquip_cancelLargePreviewLoad), restores the
 * base vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState().
 *
 * @param flags  g++ destructor flags, passed on to cUIState__cUIState(); bit 0 frees the object
 */
extern "C" void cFEStateCharEquip__cFEStateCharEquip(cFEStateCharEquip* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateCharEquip_vtbl;
    if (self->mIconsFileIndex >= 0) {
        ((cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager)->releaseFile(self->mCharID,
                      self->mIconsFileIndex);
        self->mIconsFileIndex = -1;
    }
    releaseK199148(self->mIconTextureID);
    self->mIconTextureID = -1;
    self->cancelLargePreviewLoad();
    releaseK199148(self->mPreviewTextureID);
    releaseK199148(self->mOldPreviewTextureID);
    self->mPreviewTextureID = -1;
    self->mOldPreviewTextureID = -1;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cRiderAnimBase_reset(void* self);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cFEStateCharEquip_update3DPositions(cFEStateCharEquip* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_SetFrontEndMusicMode(void* self, int a1);
extern char str_12equ_char[];

/**
 * Opens the equip-gear screen ("12equ_char") and poses the user's FE rider for it.
 *
 * Puts the rider's draw layer (+0xC2C) at 0x11 and clears its +0xCCC; when its character id is below 10 and
 * its model is ready (+0xCB4, +0xCB8), restarts its animation with animation 0x1B2. Resets the zoom (0xAA4)
 * and turn (0xAAC) to 0 and the pan (0xAA8) to -1, places the camera and models (cFEStateCharEquip_update3DPositions) and switches
 * the front-end sound-bank mode to 2.
 */
void cFEStateCharEquip::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_12equ_char), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFERider* feRider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
    feRider->mDrawLayer = 17;
    feRider->mAnimateBoard = 0;
    int ok = *(unsigned int*)feRider < 10 && feRider->mDoneLoadingTextures != 0 && feRider->mCompiledModel != 0;
    if (ok) {
        void* anim = feRider->mpRiderAnim;
        if (anim != 0) {
            cRiderAnimBase_reset(anim);
            cRiderAnimBase_play(anim, 434, 0, -1.0f);
        }
    }
    mZoom = 0.0f;
    mPlayerRotation = 0.0f;
    mPan = -1.0f;
    update3DPositions();
    cSSXAudio_SetFrontEndMusicMode(cAudioInterface_bxAudio(), 2);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
void* cUIState_onDestroyScreen(void* self);

/**
 * Takes the user's FE rider off the draw layers (mDrawLayer = -1), then destroys the screens like the base state.
 *
 * The rider is left alone when SSXApp->RealFE is null; cUIState_onDestroyScreen() does the rest.
 */
void cFEStateCharEquip::onDestroyScreen()
{
    cFE* mgr = SSXApp->RealFE;
    if (((char*)mgr) != 0) {
        cFERider* feRider = cFERiderManager_getHuman1(&mgr->mRiderManager, mUserNum);
        *(unsigned int*)&feRider->mDrawLayer = 0xFFFFFFFF;
    }
    cUIState_onDestroyScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_initStorePreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_initInventoryPreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updatePolyBar(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(cFEStateCharEquip* self, int a1);
extern "C" void cFEStateCharEquip_redrawRows(cFEStateCharEquip* self, int a1, int a2);

/**
 * Gives the equip-gear state focus: sets up its mode's widgets, the points bars and the item rows.
 *
 * Runs cFEStateCharEquip_initStorePreview() in store mode (0xC8 set; probably On Tour initStorePreview) or cFEStateCharEquip_initInventoryPreview()
 * otherwise (initInventoryPreview), then the points bar (cFEStateCharEquip_updatePolyBar) and the equip preview bar with no
 * extra cost (cFEStateCharEquip_updatePolyEquipPreviewBar), redraws the rows with a scroll reset (cFEStateCharEquip_redrawRows) and runs
 * cFEState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before, passed on to cFEState_onGainFocus()
 */
void cFEStateCharEquip::onGainFocus(cUIState* pPrevState)
{
    bool inStore = mEquipMode == cFEStateCharEquip::kEquipModeStore;
    if (inStore) {
        initStorePreview();
    } else {
        initInventoryPreview();
    }
    updatePolyBar();
    updatePolyEquipPreviewBar(0);
    redrawRows(1, 1);
    cFEState::onGainFocus(pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onLoseFocus);
#ifdef SKIP_ASM
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(cFERider* rider, int a1);

/**
 * Clears two flags of the front-end rider record this equip screen works on when the screen loses focus.
 *
 * The record is entry +0x44 (a signed char rider index) of the 0xCE0-byte records at SSXApp->RealFE +
 * 0xB0 (cFERiderManager_getHuman1); cFERider_setVisible(record, 0) zeroes its words at +0xCC4 and +0xCC8.
 */
void cFEStateCharEquip::onLoseFocus(cUIState*)
{
    cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum)->setVisible(0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

int GetHashValue32(char* str);

extern const int kBolt_None_eate __asm__("kBolt_None");

extern "C" void cFEStateCharEquip_updateHeading_eate(void* self, int id) __asm__("cFEStateCharEquip_updateHeading");

extern "C" void cFEStateCharEquip_drawInventoryRow_eate(void* self, int idx) __asm__("cFEStateCharEquip_drawInventoryRow");

extern "C" void cFEStateCharEquip_drawStoreRow_eate(void* self, int idx) __asm__("cFEStateCharEquip_drawStoreRow");

extern "C" void cFEStateCharEquip_updateBankWidget_eate(void* self) __asm__("cFEStateCharEquip_updateBankWidget");

extern char str_kT_CMNBuyGear[];

extern char str_kT_CMNEquipGear[];

extern "C" void cUIText_setAsciiString_eate(void* t, const char* s) __asm__("cUIText_setAsciiString__FP7cUITextPCc");
extern "C" void cUIText_setUnicodeStringByID_eate(void* t, int id) __asm__("cUIText_setUnicodeStringByID__FP7cUITexti");
extern "C" void cUIMenu_setScrollableMenu_FE(void* w, unsigned char v) __asm__("cUIMenu_setScrollableMenu");
extern char* D_00441218[];
extern char* D_00441230[];
extern char str_helptext[];
extern char str_title_equip_character[];
extern char str_MnuGroups[];
extern char str_buytotals[];
extern char str_barandpoints[];
extern char str_overbar2[];
extern char str_equip_points[];
extern char str_equip_btm_left[];
extern char str_largeshape[];
extern char str_empty_d[];
extern char str_btext5[];
extern char str_ps2sq[];
extern char str_xby[];
extern char str_ngcy[];
extern char str_heading[];
extern char str_1right[];
extern char str_2right[];
extern char str_bolt01[];
extern char str_overbar[];
extern char str_points[];

static inline void hide_FE(void* w)
{
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)VPTR_OF(cListNode, w);
    vt[9].fn(vthis(w, vt[9]), 0);
}

struct sCharEquip_FE {
    char pad0[0x48];
    void* mpTitleText;
    void* mpHelpText;
    void* mpButtonHelpText;
    void* mpButtonHelpIcon;
    void* mpHeading;
    void* mpMenu;
    void* f60;
    void* f64;
    void* f68;
    void* f6C;
    void* f70;
    void* f74;
    void* f78;
    void* f7C;
    void* f80;
    void* f84;
    void* f88;
    void* f8C[6];
    void* fA4[6];
    char padBC[0xC8 - 0xBC];
    int mEquipMode;
    char padCC[0x958 - 0xCC];
    int mNumItems;
    char pad95C[0xAE4 - 0x95C];
    int mCanSwitchMode;
};

static inline bool isC8_FE(sCharEquip_FE* s)
{
    return s->mEquipMode == 1;
}

/**
 * Keeps the equip-gear screen's widgets as they are created and sets their first text and visibility.
 *
 * Each named widget goes to its On Tour member slot (0x48 title to 0x88 large preview). The title says
 * kT_CMNBuyGear in store mode, else kT_CMNEquipGear; the switch-mode button text and icon ("btext5", "ps2sq",
 * "xby", "ngcy") name the other mode, or hide when the mode cannot switch (0xAE4). "heading" shows the root
 * (updateHeading(-1)), "MnuGroups" becomes a scrollable menu of the item count, the store totals show only in
 * store mode, and the points and preview widgets start hidden. Rows "1"-"6" (misc data 0-5) and
 * "check1"-"check6" are filled from the item list or hidden past its end.
 *
 * @param w  the widget just created (its hash name is at +0x38)
 */
extern "C" void cFEStateCharEquip_onWidgetCreate(sCharEquip_FE* self, cUIThing* w)
{
    int id = w->mHashName;
    if (id == GetHashValue32(str_title_equip_character)) {
        self->mpTitleText = ((void*)w);
        if (isC8_FE(self)) {
            cUIText_setUnicodeStringByID_eate(self->mpTitleText, GetHashValue32(str_kT_CMNBuyGear));
        } else {
            cUIText_setUnicodeStringByID_eate(self->mpTitleText, GetHashValue32(str_kT_CMNEquipGear));
        }
    } else if (id == GetHashValue32(str_helptext)) {
        self->mpHelpText = ((void*)w);
        hide_FE(self->mpHelpText);
    } else if (id == GetHashValue32(str_btext5)) {
        self->mpButtonHelpText = ((void*)w);
        if (self->mCanSwitchMode == 0) {
            hide_FE(self->mpButtonHelpText);
        } else {
            if (isC8_FE(self)) {
                cUIText_setUnicodeStringByID_eate(self->mpButtonHelpText, GetHashValue32(str_kT_CMNEquipGear));
            } else {
                cUIText_setUnicodeStringByID_eate(self->mpButtonHelpText, GetHashValue32(str_kT_CMNBuyGear));
            }
        }
    } else if (id == GetHashValue32(str_ps2sq) || id == GetHashValue32(str_xby) || id == GetHashValue32(str_ngcy)) {
        self->mpButtonHelpIcon = ((void*)w);
        if (self->mCanSwitchMode == 0) {
            hide_FE(self->mpButtonHelpIcon);
        }
    } else if (id == GetHashValue32(str_heading)) {
        self->mpHeading = ((void*)w);
        cFEStateCharEquip_updateHeading_eate(self, kBolt_None_eate);
    } else if (id == GetHashValue32(str_MnuGroups)) {
        self->mpMenu = ((void*)w);
        cUIMenu_setScrollableMenu_FE(self->mpMenu, self->mNumItems);
    } else if (id == GetHashValue32(str_buytotals)) {
        self->f60 = ((void*)w);
        if (!isC8_FE(self)) {
            hide_FE(self->f60);
        }
    } else if (id == GetHashValue32(str_1right)) {
        self->f64 = ((void*)w);
        cFEStateCharEquip_updateBankWidget_eate(self);
    } else if (id == GetHashValue32(str_2right)) {
        self->f68 = ((void*)w);
        cUIText_setAsciiString_eate(self->f68, str_empty_d);
    } else if (id == GetHashValue32(str_bolt01)) {
        self->f84 = ((void*)w);
    } else if (id == GetHashValue32(str_barandpoints)) {
        self->f6C = ((void*)w);
        hide_FE(self->f6C);
    } else if (id == GetHashValue32(str_overbar)) {
        self->f70 = ((void*)w);
        hide_FE(self->f70);
    } else if (id == GetHashValue32(str_overbar2)) {
        self->f74 = ((void*)w);
        hide_FE(self->f74);
    } else if (id == GetHashValue32(str_equip_points)) {
        self->f78 = ((void*)w);
    } else if (id == GetHashValue32(str_equip_btm_left)) {
        self->f80 = ((void*)w);
        hide_FE(self->f80);
    } else if (id == GetHashValue32(str_points)) {
        self->f7C = ((void*)w);
        cUIText_setAsciiString_eate(self->f7C, str_empty_d);
    } else if (id == GetHashValue32(str_largeshape)) {
        self->f88 = ((void*)w);
        hide_FE(self->f88);
    } else {
        for (int i = 0; i < 6; i++) {
            if (id == GetHashValue32(D_00441218[i])) {
                self->f8C[i] = ((void*)w);
                w->mMiscData = i;
                if (i < self->mNumItems) {
                    if (isC8_FE(self)) {
                        cFEStateCharEquip_drawStoreRow_eate(self, i);
                    } else {
                        cFEStateCharEquip_drawInventoryRow_eate(self, i);
                    }
                } else {
                    hide_FE(self->f8C[i]);
                }
                return;
            }
            if (id == GetHashValue32(D_00441230[i])) {
                self->fA4[i] = ((void*)w);
                if (i < self->mNumItems) {
                    if (isC8_FE(self)) {
                        cFEStateCharEquip_drawStoreRow_eate(self, i);
                    } else {
                        cFEStateCharEquip_drawInventoryRow_eate(self, i);
                    }
                } else {
                    hide_FE(self->fA4[i]);
                }
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_buySelectedItem(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updateBankWidget(cFEStateCharEquip* self);
struct sEquipDetail;
extern "C" bool cFEStateCharEquip_removeItem(sEquipDetail* self, int idx);
extern "C" void cFEStateCharEquip_redrawRows(cFEStateCharEquip* self, int a1, int a2);

/**
 * Hides the equip-gear state behind popups and buys the selected item when the buy popup closes with a yes.
 *
 * kONPOPUPCREATE (a popup opened) clears this state's visible bit (mAttrState bit 3) unless the popup is the buy
 * popup (state id cUIStateBuyPopup::kStateID). kONPOPUPDESTROY (popped) sets it again and, for the buy popup with
 * its result (+0x6C) set,
 * buys the item (cFEStateCharEquip_buySelectedItem), shows the new bank and redraws the rows when the item leaves the list
 * (cFEStateCharEquip_removeItem).
 *
 * @param widget  the popup state that sent the event
 * @param msg  the popup event (kONPOPUPCREATE opened, kONPOPUPDESTROY closed)
 */
void cFEStateCharEquip::onPopupEvent(cUIState* widget, unsigned int msg)
{
    switch (msg) {
    case cUIState::kONPOPUPCREATE:
        if (widget->mHashName != cUIStateBuyPopup::kStateID) {
            mAttrState &= ~8;
        }
        break;
    case cUIState::kONPOPUPDESTROY:
        if (((mAttrState >> 3) & 1) == 0) {
            mAttrState |= 8;
        }
        if (widget->mHashName == cUIStateBuyPopup::kStateID && ((cUIStateBuyPopup*)widget)->mbAccept != 0) {
            this->buySelectedItem();
            updateBankWidget();
            if (cFEStateCharEquip_removeItem((sEquipDetail*)this, mpMenu->mScrollItemCurrent)) {
                this->redrawRows(0, 1);
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onUpdate);
#ifdef SKIP_ASM
struct cList;
int GetHashValue32(char* str);
void* cList_first(cList* list);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_onUpdate(void* self);
extern "C" bool cFEBoltFileManager_isFileLoaded(cFEBoltFileManager* self, int a1, int a2);
extern "C" void cFEStateCharEquip_updatePreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updatePreviewLoad(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_update3DPositions(cFEStateCharEquip* self);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern char str_loading_text[];


/**
 * Updates the equip-gear state each frame: the icon file's arrival, the "loading text" and the preview loads.
 *
 * Runs the base update cUIState_onUpdate(); once the icon file (0xA64) is ready in the FE file cache, marks it
 * (0xA60) and refreshes the selected item's preview (cFEStateCharEquip_updatePreview). "loading text" shows in store mode while
 * a large preview is pending (0xA6C, 0x95C), otherwise until the FE rider's model is shown (+0xCC8). Then
 * cFEStateCharEquip_updatePreviewLoad() advances the preview load and cFEStateCharEquip_update3DPositions() places the camera and models.
 */
void cFEStateCharEquip::onUpdate()
{
    cUIState_onUpdate(this);
    if (mIconsLoaded == 0
        && ((cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager)->isFileLoaded(mCharID,
                         mIconsFileIndex)) {
        mIconsLoaded = 1;
        this->updatePreview();
    }
    void* screen = cList_first((cList*)(&mScreens));
    cUIThing* obj = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_loading_text));
    if (obj != 0) {
        cFERider* feRider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
        int on = 0;
        int mode1 = mEquipMode == cFEStateCharEquip::kEquipModeStore;
        if (mode1) {
            if (mPreviewTextureFileIndex >= 0 || mLargePreviewDelay > 0)
                on = 1;
        } else {
            on = feRider->mVisible;
            on ^= 1;
        }
        obj->setVisible(on);
    }
    this->updatePreviewLoad();
    update3DPositions();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onInputBegin);
#ifdef SKIP_ASM
static inline int isMode1_9A38(cFEStateCharEquip* self)
{
    return self->mEquipMode == cFEStateCharEquip::kEquipModeStore;
}

static inline float clamp_9A38(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

/**
 * Turns, zooms and pans the rider preview with the camera controls in inventory mode.
 *
 * Only outside store mode and in camera state kCameraRider or kCameraToRider (0xAE0): camera rotate adds 3 x
 * getCamRotate() degrees to the turn (0xAAC, kept in 0-360), zoom subtracts 0.1 x getCamZoom() from 0xAA4 (clamped to
 * 0..1) and pan 0.1 x getCamPan() from 0xAA8 (clamped to -1..1); these are cUIInput vtable slots 33-38.
 *
 * @param in  the controller input (cUIInput)
 * @return 1 when any camera control was used, otherwise 0
 */
bool cFEStateCharEquip::onInputBegin(cUIInput* in)
{
    int handled = 0;
    if (!isMode1_9A38(this)) {
    int mode = mCameraState;
    if (mode == cFEStateCharEquip::kCameraRider || mode == cFEStateCharEquip::kCameraToRider) {
    if (in->isCamRotate()) {
        mPlayerRotation += in->getCamRotate() * 3.0f;
        while (mPlayerRotation >= 360.0f)
            mPlayerRotation -= 360.0f;
        while (mPlayerRotation < 0.0f)
            mPlayerRotation += 360.0f;
        handled = 1;
    }
    if (in->isCamZoom()) {
        // PORT: <? (GNU min operator)
        mZoom = clamp_9A38(mZoom + in->getCamZoom() * -0.10000000149011612f, 0.0f, 1.0f);
        handled = 1;
    }
    if (in->isCamPan()) {
        mPan = clamp_9A38(mPan + in->getCamPan() * -0.10000000149011612f, -1.0f, 1.0f);
        handled = 1;
    }
    }
    }
    return handled;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cFEStateCharEquip_updateHeading(cFEStateCharEquip* self, int id);
extern "C" void cFEStateCharEquip_buildItemList(void* self, int id);
extern "C" void cFEStateCharEquip_redrawRows(cFEStateCharEquip* self, int a1, int a2);
extern "C" void cFEStateCharEquip_startLargePreviewLoad(cFEStateCharEquip* self, int a1);
extern "C" void cFEStateCharEquip_updatePreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updateCharacterModel(cFEStateCharEquip* self);
extern "C" bool cFEStateCharEquip_toggleSelectedItem(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_openBuyPopup(cFEStateCharEquip* self, tBEBoltDef* item);
extern "C" void cFEStateCharEquip_updatePolyBar(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(cFEStateCharEquip* self, int a1);
extern "C" int cBEBoltInterface_getUsedEquipPoints(void* self, int a1, int a2);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern int kBolt_boardGroup;
extern int cFEStateCharEquip_polyLimit;

static inline int isMode1_9C28(cFEStateCharEquip* self)
{
    return self->mEquipMode == cFEStateCharEquip::kEquipModeStore;
}

/**
 * Handles the item rows: opens a category, buys or equips an item, goes back up a category, or leaves.
 *
 * kONNEXT (accept) on row top + misc data: a category (flag 0x20) is pushed on the stack (0xD0, depth 0x120)
 * and listed, the boards category (3) easing the camera to the board (0xAE0 = kCameraToBoard); an item (flag 4) opens
 * the buy popup in store mode when the money covers its price x 10, or in the inventory is equipped or taken off
 * (cFEStateCharEquip_toggleSelectedItem) when the row's points fit the 3392 budget, then the bars and rider model are refreshed.
 *
 * kONPREV (back) lists the parent category (leaving boards eases back, 0xAE0 = kCameraToRider) or, at the root, sets
 * the FE rider's Customize speech request (mCustomized) and pushes getPrevState's state. kONGAINFOCUS (row focused)
 * starts a delayed large-preview load in store mode and refreshes the item preview.
 *
 * @param w  the row widget that sent the event; nothing happens when null
 * @param ev  the widget event (kONNEXT accept, kONPREV back, kONGAINFOCUS focused)
 */
void cFEStateCharEquip::onWidgetEvent(cUIThing* w, unsigned int ev)
{
    if (w == 0)
        return;
    switch (ev) {
    case cUIState::kONNEXT: {
        if (mpMenu == 0)
            return;
        int idx = mpMenu->mScrollItemOffset + w->mMiscData;
        if (idx >= mNumItems)
            return;
        tBEBoltDef* item = mpItems[idx];
        int id = item->id;
        if (item->flags & 0x20) {
            mCurrentStackPos++;
            updateHeading(id);
            mCategoryStack[mCurrentStackPos] = id;
            cFEStateCharEquip_buildItemList(this, id);
            this->redrawRows(1, 1);
            if (id == kBolt_boardGroup)
                mCameraState = kCameraToBoard;
            return;
        }
        if (!(item->flags & 4))
            return;
        if (isMode1_9C28(this)) {
            int cost = item->price;
            if (cost > 0)
                cost *= 10;
            if (mMoneyAvailable < cost)
                return;
            this->openBuyPopup(item);
            return;
        }
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        if (cFEStateCharEquip_polyLimit < mRowPoints[w->mMiscData] + mCurrentPolyCount)
            return;
        if (!this->toggleSelectedItem())
            return;
        mCurrentPolyCount = cBEBoltInterface_getUsedEquipPoints(iface, mUserNum, mCharID);
        this->redrawRows(0, 1);
        updatePolyBar();
        updatePolyEquipPreviewBar(mRowPoints[w->mMiscData]);
        updateCharacterModel();
        return;
    }
    case cUIState::kONPREV: {
        if (mCurrentStackPos > 0) {
            if (mCategoryStack[mCurrentStackPos] == kBolt_boardGroup)
                mCameraState = kCameraToRider;
            updateHeading(mCategoryStack[mCurrentStackPos - 1]);
            cFEStateCharEquip_buildItemList(this, mCategoryStack[mCurrentStackPos - 1]);
            this->redrawRows(1, 1);
        } else {
            cFERider* feRider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
            feRider->mSelected = 0;
            feRider->mCustomized = 1;
            cUIState* prevState = mpEngine->mpCustom->getPrevState(this, 0);
            if (prevState)
                cUIStateStack_push(&mpEngine->mStateStack, prevState);
        }
        break;
    }
    case cUIState::kONGAINFOCUS:
        if (isMode1_9C28(this))
            startLargePreviewLoad(1);
        this->updatePreview();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cFEStateCharEquip_switchEquipMode(cFEStateCharEquip* self);
extern int cFEStateCharEquip_polyLimit;

static inline bool isMode_9F20(int m, int v)
{
    return m == v;
}

static inline bool hasFlag_9F20(tBEBoltDef* item, int f)
{
    return (item->flags & f) != 0;
}

/**
 * Answers the item menu's queries and buttons: row checks, scrolling, accept, back and mode switch.
 *
 * kISSELECTABLE is true for an item with flag 4 and kISHIGHLIGHTABLE for an existing row; kSCROLLUP and
 * kSCROLLDOWN redraw the rows and give 1. kONNEXT gives 0x101 for a category, any store item (its price test gives
 * 0x101 either way) or an inventory item whose points fit (row cost at 0xA78 plus 0xC4 at most 3392), else 0x110 (the
 * item's mSound[0]). kONPREVIOUS (back) steps the category depth (0x120) down and gives 1; kONOPTION switches the
 * mode (cFEStateCharEquip_switchEquipMode) when allowed (0xAE4) and gives 0x101, else 0x100; kONMISC gives 0x100.
 *
 * @param pMenu  the menu that sent the event (unused)
 * @param kind  the menu event (tUIMenuEvent)
 * @param idx  the row index for kISSELECTABLE and kISHIGHLIGHTABLE
 * @return a flag for kISSELECTABLE and kISHIGHLIGHTABLE, otherwise feedback bits (0x100 frame, 1 mSound[1], 0x10
 *           mSound[0])
 */
int cFEStateCharEquip::onWidgetMenu(cUIMenu* pMenu, unsigned int kind, unsigned int idx)
{
    switch (kind) {
    case cUIMenu::kSCROLLUP:
    case cUIMenu::kSCROLLDOWN:
        redrawRows(0, 0);
        return 1;
    case cUIMenu::kISSELECTABLE:
        if ((int)idx < mNumItems) {
            return hasFlag_9F20(mpItems[idx], 4);
        }
        return 0;
    case cUIMenu::kISHIGHLIGHTABLE:
        if (idx == 0 || mNumItems == 0 || (int)idx < mNumItems) return 1;
        return 0;
    case cUIMenu::kONNEXT: {
        cUIMenu* selected = mpMenu;
        int selIndex = selected->mScrollItemCurrent;
        if (selIndex < 0) return 272;
        if (selIndex < mNumItems) {
            tBEBoltDef* item = mpItems[selIndex];
            if (item->flags & 0x20) return 257;
            if (isMode_9F20(mEquipMode, cFEStateCharEquip::kEquipModeStore)) {
                int cost = item->price;
                if (cost > 0) cost *= 10;
                if (cost >= mMoneyAvailable) break;
                return 257;
            } else {
                if (cFEStateCharEquip_polyLimit >= mRowPoints[selIndex - selected->mScrollItemOffset] + mCurrentPolyCount)
                    break;
            }
        }
        return 272;
    }
    case cUIMenu::kONPREVIOUS:
        if (mCurrentStackPos > 0) {
            mCurrentStackPos = mCurrentStackPos - 1;
            return 1;
        }
        break;
    case cUIMenu::kONOPTION:
        if (mCanSwitchMode != 0) {
            switchEquipMode();
            return 257;
        }
        return 0x100;
    case cUIMenu::kONMISC:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_getIconID);
#ifdef SKIP_ASM
extern "C" void* cFEBoltFileManager_getFileDataValue(cFEBoltFileManager* self, int bank, int slotIndex);
extern "C" void SHAPE_name(void* pkg, int i, char* name);
extern "C" int strcmp(const void* a, const void* b);
extern char str_icon[];

/**
 * Finds an icon in the character's loaded icon package and returns its texture.
 *
 * Needs the icon package loaded (+0xA60) and present in the file manager (cFEBoltFileManager_getFileDataValue with the character's bank +0xBC
 * and file +0xA64). Compares each image's name, cut to 4 characters, with @p name; on a match creates the icon texture
 * (graphics manager vtable entry 46, kind "icon") into +0xA68 the first time, or reloads that texture with the image
 * (entry 48). All icons share that one texture.
 *
 * @param name  the icon name
 * @return the texture id (+0xA68), or -1 when the package is not loaded or has no such image
 */
int cFEStateCharEquip::getIconID(const char* name)
{
    if (mIconsLoaded == 0)
        return -1;
    SHAPEFILE* pkg = (SHAPEFILE*)((cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager)->getFileDataValue(
            mCharID, mIconsFileIndex);
    if (pkg == 0)
        return -1;
    int n = pkg->num;
    int i;
    for (i = 0; i < n; i++) {
        char nm[16];
        SHAPE_name(pkg, i, nm);
        nm[4] = 0;
        if (strcmp(nm, name) == 0) {
            // ORIGINAL-PTR: a shape file's table holds each shape's byte offset from the start of the file (a file
            // blob)
            const SHAPE* shape = (const SHAPE*)((char*)pkg + pkg->tbl[i].offset);
            int id = mIconTextureID;
            if (id == kNoTextureID) {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                mIconTextureID = ps2GraphicsMan->AddTexture(shape, str_icon, 0, 1, -1);
            } else {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                ps2GraphicsMan->SetNewTextureData(id, shape, 1);
            }
            return mIconTextureID;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateIconTexturesLoad);
#ifdef SKIP_ASM
extern "C" bool cFEBoltFileManager_isFileLoaded(cFEBoltFileManager* self, int a1, int a2);
extern "C" void cFEStateCharEquip_updatePreview(cFEStateCharEquip* self);

/**
 * Marks the icon package loaded once its file is ready and refreshes the item preview.
 *
 * When +0xA60 is clear and the file manager reports the icon file ready (cFEBoltFileManager_isFileLoaded on +0xBC, +0xA64), sets +0xA60
 * and calls cFEStateCharEquip_updatePreview. No call site was found in the asm.
 */
void cFEStateCharEquip::updateIconTexturesLoad()
{
    if (mIconsLoaded == 0
        && ((cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager)->isFileLoaded(mCharID,
                         mIconsFileIndex)) {
        mIconsLoaded = 1;
        this->updatePreview();
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_initInventoryPreview);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_kT_CMNEquipGear[];


/**
 * Sets the equip screen up for the inventory.
 *
 * Titles it kT_CMNEquipGear (+0x48), hides the money group (+0x60) and the large preview (+0x88), shows the small icon
 * preview group (+0x80), shows the user's front-end rider (cFERider_setVisible with 1) and sets the camera state (+0xAE0) to
 * 1.
 */
void cFEStateCharEquip::initInventoryPreview()
{
    if (mpTitleText != 0) {
        cUIText_setUnicodeStringByID(mpTitleText, GetHashValue32(str_kT_CMNEquipGear));
    }
    cUIThing* widget = mpMoneyGroup;
    if (((char*)widget) != 0) {
        widget->setVisible(0);
    }
    widget = (cUIThing*)mpLargePreview;
    if (((char*)widget) != 0) {
        widget->setVisible(0);
    }
    widget = (cUIThing*)mpSmallIconPreviewGroup;
    if (((char*)widget) != 0) {
        widget->setVisible(1);
    }
    cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum)->setVisible(1);
    mCameraState = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_initStorePreview);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_kT_CMNBuyGear[];


/**
 * Sets the equip screen up for the store.
 *
 * Titles it kT_CMNBuyGear (+0x48), hides the small icon preview group (+0x80) and the points group (+0x6C), hides the
 * user's front-end rider and shows the money group (+0x60).
 */
void cFEStateCharEquip::initStorePreview()
{
    if (mpTitleText != 0) {
        cUIText_setUnicodeStringByID(mpTitleText, GetHashValue32(str_kT_CMNBuyGear));
    }
    cUIThing* widget = mpSmallIconPreviewGroup;
    if (((char*)widget) != 0) {
        widget->setVisible(0);
    }
    widget = (cUIThing*)mpPointsGroup;
    if (((char*)widget) != 0) {
        widget->setVisible(0);
    }
    cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum)->setVisible(0);
    widget = (cUIThing*)mpMoneyGroup;
    if (((char*)widget) != 0) {
        widget->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_switchEquipMode);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern "C" void cFEStateCharEquip_updateHeading(cFEStateCharEquip* self, int id);
extern "C" void cFEStateCharEquip_buildItemList(void* self, int id);
extern int kBolt_None;
extern char str_kT_CMNBuyGear[];
extern char str_kT_CMNEquipGear[];


static inline int isDetailK19A3D0(cFEStateCharEquip* self)
{
    return self->mEquipMode == 1;
}

/**
 * Switches the equip screen between the inventory (mode 0) and the store (mode 1) and lists the root category again.
 *
 * Flips +0xC8, sets the screen up for the new mode (cFEStateCharEquip_initInventoryPreview or cFEStateCharEquip_initStorePreview) and labels the button help (+0x50)
 * with the other mode (kT_CMNBuyGear in the inventory, kT_CMNEquipGear in the store). Then empties the category stack
 * (depth +0x120 = 0, the 80 bytes at +0xD0 filled with kBolt_None), updates the heading and rebuilds and redraws the
 * list (cFEStateCharEquip_buildItemList, cFEStateCharEquip_redrawRows with 1, 1).
 */
void cFEStateCharEquip::switchEquipMode()
{
    if (isDetailK19A3D0(this)) {
        mEquipMode = 0;
        initInventoryPreview();
        if (mpButtonHelpText != 0) {
            cUIText_setUnicodeStringByID(mpButtonHelpText, GetHashValue32(str_kT_CMNBuyGear));
        }
    } else {
        mEquipMode = 1;
        initStorePreview();
        if (mpButtonHelpText != 0) {
            cUIText_setUnicodeStringByID(mpButtonHelpText, GetHashValue32(str_kT_CMNEquipGear));
        }
    }
    mCurrentStackPos = 0;
    MEM_fill(&mCategoryStack[0], kBolt_None, 80);
    updateHeading(kBolt_None);
    cFEStateCharEquip_buildItemList(this, kBolt_None);
    this->redrawRows(1, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cancelLargePreviewLoad);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_releaseFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Cancels the pending large-preview load.
 *
 * Releases the queued preview file (+0xA6C, cFEBoltFileManager_releaseFile) if any, clears the file name (+0x960) and sets +0xA6C and
 * the delay (+0x95C) to -1.
 */
void cFEStateCharEquip::cancelLargePreviewLoad()
{
    int idx = mPreviewTextureFileIndex;
    if (idx >= 0) {
        ((cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager)->releaseFile(mCharID, idx);
    }
    mLargePreviewFileName[0] = 0;
    mPreviewTextureFileIndex = -1;
    mLargePreviewDelay = -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_startLargePreviewLoad);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBEBoltInterface_getBigIconFileName(void* iface, int rider);
extern "C" char* strrchr(const char* s, int c);
extern "C" char* strncpy(char* dst, const char* src, int n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFEStateCharEquip_cancelLargePreviewLoad(cFEStateCharEquip* self);
extern char str_pct_s_pct_spct_s[];

static inline int isDigitK19A4E8(unsigned char c)
{
    return c >= '0' && c <= '9';
}

/**
 * Starts loading the large preview of the selected item after a delay.
 *
 * Cancels the previous load (cFEStateCharEquip_cancelLargePreviewLoad), releases the old preview texture (+0xA74) and moves the current one
 * (+0xA70) there. For a selected row with a name (+0x30) sets the delay (+0x95C: 20 frames when @p flag is set, else 1)
 * and builds the file name at +0x960 as "%s_%s%s": the character's file name (cBEBoltInterface_getBigIconFileName) without the character
 * before its '.', the item name from its first digit, and the extension. The name buffer is both the destination and
 * the first source of that sprintf.
 *
 * @param flag  nonzero for the longer delay
 */
void cFEStateCharEquip::startLargePreviewLoad(int flag)
{
    if (mpMenu == 0)
        return;
    cancelLargePreviewLoad();
    releaseK199148(mOldPreviewTextureID);
    mOldPreviewTextureID = mPreviewTextureID;
    mPreviewTextureID = -1;
    int idx = mpMenu->mScrollItemCurrent;
    if (idx < 0)
        return;
    if (idx >= mNumItems)
        return;
    const char* namePart = mpItems[idx]->pictureName;
    if (namePart == 0)
        return;
    if (flag)
        mLargePreviewDelay = 20;
    else
        mLargePreviewDelay = 1;
    char* buf = mLargePreviewFileName;
    char* fileName = cBEBoltInterface_getBigIconFileName(cBE_getInterface_Fv(cBE_getBE(), 9), mCharID);
    char* dot = strrchr(fileName, '.');
    int len = dot - fileName - 1;
    strncpy(buf, fileName, len);
    buf[len] = 0;
    while (*namePart != 0) {
        if (*namePart >= '0' && *namePart <= '9')
            break;
        namePart++;
    }
    sprintf(mLargePreviewFileName, str_pct_s_pct_spct_s, mLargePreviewFileName, namePart, dot);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePreviewLoad);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_releaseFile(cFEBoltFileManager* self, int bank, int i);
extern "C" bool cFEBoltFileManager_isFileLoaded(cFEBoltFileManager* self, int a1, int a2);
extern "C" void* cFEBoltFileManager_getFileDataValue(cFEBoltFileManager* self, int bank, int slotIndex);
extern "C" int cFEBoltFileManager_requestFile(void* self, int bank, void* name, int a3, int a4, int a5);
extern "C" void cFEStateCharEquip_updatePreview(cFEStateCharEquip* self);
extern char str_preview[];


/**
 * Advances the store's large-preview load (store mode only).
 *
 * Counts the delay (+0x95C) down and, when it reaches 0, queues the preview file (cFEBoltFileManager_requestFile kind 9 into +0xA6C).
 * Once the queued file is ready, makes its texture (graphics manager vtable entry 46, kind "preview") into +0xA70, or
 * -1 when the file is empty, releases the file and refreshes the item preview (cFEStateCharEquip_updatePreview).
 */
void cFEStateCharEquip::updatePreviewLoad()
{
    int mode1 = mEquipMode == cFEStateCharEquip::kEquipModeStore;
    if (!mode1)
        return;
    if (mLargePreviewDelay > 0) {
        if (--mLargePreviewDelay == 0) {
            mPreviewTextureFileIndex = cFEBoltFileManager_requestFile(SSXApp->RealFE->mBoltFileManager,
                                                         mCharID, mLargePreviewFileName, 9, 1, 0);
            mLargePreviewDelay = -1;
        }
    } else if (mPreviewTextureFileIndex >= 0) {
        cFEBoltFileManager* tbl = (cFEBoltFileManager*)SSXApp->RealFE->mBoltFileManager;
        if (tbl->isFileLoaded(mCharID, mPreviewTextureFileIndex)) {
            SHAPEFILE* file = (SHAPEFILE*)tbl->getFileDataValue(mCharID, mPreviewTextureFileIndex);
            if (file != 0) {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                // ORIGINAL-PTR: a shape file's table holds each shape's byte offset from the start of the file (a file
                // blob)
                const SHAPE* shape = (const SHAPE*)((char*)file + file->tbl[0].offset);
                mPreviewTextureID = ps2GraphicsMan->AddTexture(shape, str_preview, 0, 1, -1);
            } else {
                mPreviewTextureID = -1;
            }
            tbl->releaseFile(mCharID, mPreviewTextureFileIndex);
            mPreviewTextureFileIndex = -1;
            this->updatePreview();
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cancelPreviewLoad);
#ifdef SKIP_ASM
extern "C" void cFERider_releasePartTextures(void* a0, int a1);

/**
 * Cancels the user's front-end rider's texture loads (cFERider_releasePartTextures with no item list) when the front end exists.
 *
 * No call site was found in the asm.
 */
void cFEStateCharEquip::cancelPreviewLoad()
{
    cFE* mgr = SSXApp->RealFE;
    if (((char*)mgr) != 0) {
        cFERider_releasePartTextures(cFERiderManager_getHuman1(&mgr->mRiderManager, mUserNum), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateHelpText);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBEBoltInterface_getCharProfile(void* self, int a1, int a2);
extern int cFEStateCharEquip_polyLimit;
extern char str_kT_HELPNoItemsCTM[];
extern char str_kT_HELPBuyItemInCTM[];
extern char str_kT_HELPBoughtMaxItems[];
extern char str_kT_FEHELPChooseCatContinue[];
extern char str_kT_FEHELPItemAvailBuy[];
extern char str_kT_129HELPSaveCashItem[];
extern char str_kT_FEYourCurrentOutfit[];
extern char str_kT_FEHELPRemoveItems[];

static inline int isMode1_A798(cFEStateCharEquip* self)
{
    return self->mEquipMode == 1;
}

static inline tBEProfileBolt* getEntry_A798(cBECharProfileDB* db, int id)
{
    short idx = db->mpBoltsMap[id];
    if (idx >= 0) {
        return &((&db->mBolts[0]))[idx];
    }
    return 0;
}

static inline void setText_A798(cFEStateCharEquip* self, char* s)
{
    cUIText_setUnicodeStringByID(self->mpHelpText, GetHashValue32(s));
}

/**
 * Sets the equip screen's help text for the selected row.
 *
 * Hides the help text (mpHelpText), then: with no item selected, kT_HELPBoughtMaxItems in the store, else
 * kT_HELPNoItemsCTM (when +0xAE4 is set) or kT_HELPBuyItemInCTM; on a category (flag 0x20) kT_FEHELPChooseCatContinue;
 * in the store kT_FEHELPItemAvailBuy when the money (+0xC0) covers the price x 10, else kT_129HELPSaveCashItem; in the
 * inventory kT_FEHELPRemoveItems when the row's points would pass the limit cFEStateCharEquip_polyLimit, else kT_FEYourCurrentOutfit.
 * Shows the help text again. Likely bug: both outcomes of the item's equipped test (entry flag 4) choose
 * kT_FEYourCurrentOutfit.
 */
void cFEStateCharEquip::updateHelpText()
{
    if ((cUIThing*)mpHelpText == 0)
        return;
    ((cUIThing*)mpHelpText)->setVisible(0);
    int sel = mpMenu->mScrollItemCurrent;
    if (sel < 0 || sel >= mNumItems) {
        ((cUIThing*)mpHelpText)->setVisible(1);
        if (!isMode1_A798(this)) {
            if (mCanSwitchMode != 0)
                setText_A798(this, str_kT_HELPNoItemsCTM);
            else
                setText_A798(this, str_kT_HELPBuyItemInCTM);
        } else {
            setText_A798(this, str_kT_HELPBoughtMaxItems);
        }
        return;
    }
    tBEBoltDef* item = mpItems[sel];
    if (item->flags & 0x20) {
        setText_A798(this, str_kT_FEHELPChooseCatContinue);
    } else if (isMode1_A798(this)) {
        int cost = item->price;
        if (cost > 0)
            cost *= 10;
        if (mMoneyAvailable >= cost)
            setText_A798(this, str_kT_FEHELPItemAvailBuy);
        else
            setText_A798(this, str_kT_129HELPSaveCashItem);
    } else {
        cBECharProfileDB* db = (cBECharProfileDB*)cBEBoltInterface_getCharProfile(cBE_getInterface(cBE_getBE(), 9), mUserNum, mCharID);
        tBEProfileBolt* equipEntry = getEntry_A798(db, item->id);
        int idx = sel - mpMenu->mScrollItemOffset;
        if (cFEStateCharEquip_polyLimit >= mCurrentPolyCount + mRowPoints[idx]) {
            if (equipEntry->flags & 4)
                setText_A798(this, str_kT_FEYourCurrentOutfit);
            else
                setText_A798(this, str_kT_FEYourCurrentOutfit);
        } else {
            setText_A798(this, str_kT_FEHELPRemoveItems);
        }
    }
    ((cUIThing*)mpHelpText)->setVisible(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePreview);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_updateStorePreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updateInventoryPreview(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_updateHelpText(cFEStateCharEquip* self);

/**
 * Refreshes the equip screen's item preview for the current mode (cFEStateCharEquip_updateStorePreview in the store, cFEStateCharEquip_updateInventoryPreview in the
 * inventory) and then its help text (cFEStateCharEquip_updateHelpText).
 */
void cFEStateCharEquip::updatePreview()
{
    bool buyMode = mEquipMode == cFEStateCharEquip::kEquipModeStore;
    if (buyMode) {
        updateStorePreview();
    } else {
        updateInventoryPreview();
    }
    updateHelpText();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateInventoryPreview);
#ifdef SKIP_ASM
extern "C" int cFEStateCharEquip_getIconID(cFEStateCharEquip* self, const char* name);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(cFEStateCharEquip* self, int a1);

/**
 * Refreshes the inventory preview for the selected row.
 *
 * Hides the points group (+0x6C), the small bolt icon (+0x84) and the points cost (+0x7C). For an item (flag 4, not a
 * category) shows the points group, takes the row's points (+0xA78) and, when the icons are loaded and the item has a
 * name, shows the icon with its texture (cFEStateCharEquip_getIconID). Finally updates the points preview bar (cFEStateCharEquip_updatePolyEquipPreviewBar) with
 * the row's points, or 0.
 */
void cFEStateCharEquip::updateInventoryPreview()
{
    if (mpMenu == 0)
        return;
    cUIThing* widget = mpPointsGroup;
    if (widget != 0)
        widget->setVisible(0);
    widget = mpSmallBoltIcon;
    if (widget != 0)
        widget->setVisible(0);
    widget = mpPointsCost;
    if (widget != 0)
        widget->setVisible(0);
    int idx = mpMenu->mScrollItemCurrent;
    int rowValue = 0;
    if (idx < mNumItems) {
        tBEBoltDef* item = mpItems[idx];
        unsigned int flags = item->flags;
        if (flags & 4) {
            if (!(flags & 0x20)) {
                mpPointsGroup->setVisible(1);
                int topIndex = mpMenu->mScrollItemOffset;
                cUIThing* iconWidget = mpSmallBoltIcon;
                rowValue = mRowPoints[idx - topIndex];
                if (iconWidget != 0 && item->pictureName != 0 && mIconsLoaded != 0) {
                    iconWidget->setVisible(1);
                    int iconId = getIconID(item->pictureName);
                    cUIIcon* iconWidget2 = mpSmallBoltIcon;
                    iconWidget2->mTextureID = iconId;
                    iconWidget2->mpTextureElem = 0;
                }
            }
        }
    }
    updatePolyEquipPreviewBar(rowValue);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateStorePreview);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
// getDollarString formats a dollar amount (its definition in fe/messagecenter takes the amount as a void*).
extern "C" const char* getDollarString(int dollars);

/**
 * Refreshes the store preview for the selected row.
 *
 * Shows the large preview (+0x88) with the loaded preview texture (+0xA70) when an item is selected and the texture is
 * ready, and the cost (+0x68) as the item's price x 10 (getDollarString) for an item that is not a category; otherwise
 * hides them.
 */
void cFEStateCharEquip::updateStorePreview()
{
    cUIMenu* menu = mpMenu;
    if (menu == 0)
        return;
    int sel = menu->mScrollItemCurrent;
    tBEBoltDef* item = 0;
    if (sel >= 0 && sel < mNumItems)
        item = mpItems[sel];
    if (mpLargePreview != 0) {
        mpLargePreview->setVisible(0);
        if (item != 0 && mPreviewTextureID >= 0) {
            cUIIcon* widget = mpLargePreview;
            widget->mTextureID = mPreviewTextureID;
            widget->mpTextureElem = 0;
            mpLargePreview->setVisible(1);
        }
    }
    if (mpCost != 0) {
        mpCost->setVisible(0);
        if (item != 0 && !(item->flags & 0x20)) {
            int price = item->price;
            if (price > 0)
                price *= 10;
            cUIText_setAsciiString(mpCost, getDollarString(price));
            mpCost->setVisible(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateCharacterModel);
#ifdef SKIP_ASM
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBEBoltInterface_getCharProfile(void* self, int a1, int a2);
extern "C" void cFERider_loadTextures(void* rider, void* item, int a2, int a3);

/**
 * Requeues the user's front-end rider's textures for the character's current items (cFERider_loadTextures with 0, 1).
 *
 * cFEStateCharEquip_onWidgetEvent calls it after equipping or removing an item.
 */
void cFEStateCharEquip::updateCharacterModel()
{
    void* rider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
    cFERider_loadTextures(rider, cBEBoltInterface_getCharProfile(cBE_getInterface_Fv(cBE_getBE(), 9), mUserNum, mCharID), 0, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_scrollRows);
#ifdef SKIP_ASM
extern "C" int cFEStateCharEquip_drawInventoryRows(cFEStateCharEquip* self);
extern "C" int cFEStateCharEquip_drawStoreRows(cFEStateCharEquip* self);
extern "C" void cFEStateCharEquip_drawInventoryRow(cFEStateCharEquip* self, int idx);
extern "C" void cFEStateCharEquip_drawStoreRow(cFEStateCharEquip* self, int idx);
extern "C" void* cUIText_getUnicodeString(void* item);
extern "C" void cUIText_setUnicodeString(void* item, void* value);

static inline int isMode1_19AD18(cFEStateCharEquip* self)
{
    return self->mEquipMode == 1;
}

static inline void copyRow_19AD18(cFEStateCharEquip* self, int dst, int src)
{
    cUIThing* o = self->mpBoltNameWidgets[dst];
    o->setColour(&self->mpBoltNameWidgets[src]->mColour);
    cUIText_setUnicodeString(self->mpBoltNameWidgets[dst], cUIText_getUnicodeString(self->mpBoltNameWidgets[src]));
    o = self->mpBoltCheckWidgets[dst];
    o->setVisible((self->mpBoltCheckWidgets[src]->mAttr >> 6) & 1);
}

/**
 * Scrolls the equip screen's six item rows by @p n.
 *
 * Beyond six rows either way redraws all rows (cFEStateCharEquip_drawStoreRows in the store, cFEStateCharEquip_drawInventoryRows in the inventory). Otherwise
 * copies each kept row from the row @p n away (the source text's mColour passed to setColour, its string, and the
 * icon's visibility from bit 6 (mVisible) of the source icon's mAttr) and redraws the rows that came into view. No
 * call site was found in the asm.
 *
 * @param n  rows to scroll (negative moves the rows down)
 */
void cFEStateCharEquip::scrollRows(int n)
{
    if (mpMenu == 0) {
        return;
    }
    if (n < -6 || n > 6) {
        if (isMode1_19AD18(this)) {
            drawStoreRows();
        } else {
            drawInventoryRows();
        }
        return;
    }
    if (n < 0) {
        int i;
        for (i = 5; i >= -n; i--) {
            copyRow_19AD18(this, i, i + n);
        }
        if (isMode1_19AD18(this)) {
            int j;
            for (j = 0; j < -n; j++) {
                drawStoreRow(j);
            }
        } else {
            int j;
            for (j = 0; j < -n; j++) {
                drawInventoryRow(j);
            }
        }
    } else if (n > 0) {
        int i;
        for (i = n; i < 6; i++) {
            copyRow_19AD18(this, i - n, i);
        }
        if (isMode1_19AD18(this)) {
            int j;
            for (j = 6 - n; j < 6; j++) {
                drawStoreRow(j);
            }
        } else {
            int j;
            for (j = 6 - n; j < 6; j++) {
                drawInventoryRow(j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_hasInventoryItemInCategory);
#ifdef SKIP_ASM
struct sEquipEntry_AFD0 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipItem_AFD0 {
    int f0;
    short id;       // 0x4
    char pad_0x6[0x34 - 0x6];
    int flags;      // 0x34
};
extern "C" int cBEBoltDB_collectCategoryBolts(int a0, int a1, int a2, void* out, int a4, int a5);

static inline sEquipEntry_AFD0* getEntry_AFD0(cBECharProfileDB* db, int id)
{
    short idx = db->mpBoltsMap[id];
    if (idx >= 0) {
        return &((sEquipEntry_AFD0*)&db->mBolts[0])[idx];
    }
    return 0;
}

extern "C" bool cFEStateCharEquip_hasInventoryItemInCategory(cFEStateCharEquip* self, int library,
                                                             cBECharProfileDB* db, int itemId);

/**
 * Reports whether a category holds an item of the character that is in the inventory.
 *
 * Lists the category's items (cBEBoltDB_collectCategoryBolts with the character +0xBC) and looks for one with flag 4, not a category,
 * whose character entry has flag 2 (the flag the inventory lists by).
 *
 * @param library  the item library
 * @param db       the character's item entries
 * @param itemId   the category
 * @return true when there is one, else false
 */
bool cFEStateCharEquip::hasInventoryItemInCategory(int library, cBECharProfileDB* db, int itemId)
{
    sEquipItem_AFD0* items[528];
    int n = cBEBoltDB_collectCategoryBolts(library, mCharID, itemId, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        sEquipItem_AFD0* it = items[i];
        if (it->flags & 4) {
            if (getEntry_AFD0(db, it->id)->flags & 2) {
                if (!(it->flags & 0x20)) {
                    return true;
                }
            }
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_isStoreCategoryEmpty);
#ifdef SKIP_ASM
extern "C" int cBEBoltDB_collectCategoryBolts(int a0, int a1, int a2, void* out, int a4, int a5);

static inline tBEProfileBolt* getEntry_B098(cBECharProfileDB* db, int id)
{
    short idx = db->mpBoltsMap[id];
    if (idx >= 0) {
        return &((&db->mBolts[0]))[idx];
    }
    return 0;
}

extern "C" bool cFEStateCharEquip_isStoreCategoryEmpty(
        cFEStateCharEquip* self, int library, cBECharProfileDB* db, int itemId);

/**
 * Reports whether a category has nothing to buy in the current store.
 *
 * Lists the category's items and returns false at the first one with flag 4, not a category, whose character entry
 * lacks flag 2 and whose store number equals +0xCC. Unlike cFEStateCharEquip_buildItemList's item test it does not accept +0xCC == -1.
 *
 * @param library  the item library
 * @param db       the character's item entries
 * @param itemId   the category
 * @return false when the store sells something in it, else true
 */
bool cFEStateCharEquip::isStoreCategoryEmpty(int library, cBECharProfileDB* db, int itemId)
{
    tBEBoltDef* items[528];
    int n = cBEBoltDB_collectCategoryBolts(library, mCharID, itemId, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        tBEBoltDef* it = items[i];
        if (it->flags & 4) {
            if (!(getEntry_B098(db, it->id)->flags & 2)) {
                if (it->storeNum == mStoreNum) {
                    if (!(it->flags & 0x20)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_buildItemList);
#ifdef SKIP_ASM
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_noarg() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" void qsort_B180(void* base, int n, int size, int (*cmp)(const void*, const void*)) __asm__("qsort");
extern "C" int compareEquipItems(const void* a, const void* b);

struct sEquipItem_B180 {
    short f0;
    signed char slot;   // 0x2
    char f3;
    short id;           // 0x4
    char pad_0x6[0x14 - 0x6];
    char* name;         // 0x14
    char pad_0x18[0x34 - 0x18];
    int flags;          // 0x34
};

static inline tBEProfileBolt* getEntry_B180(cBECharProfileDB* db, int id)
{
    short idx = db->mpBoltsMap[id];
    if (idx >= 0) {
        return &((&db->mBolts[0]))[idx];
    }
    return 0;
}

static inline int isMode1_B180(cFEStateCharEquip* self)
{
    return self->mEquipMode == 1;
}

struct sCharEquip_B180 {
    char pad0[0x44];
    signed char team;               // 0x44
    char pad45[0xBC - 0x45];
    int slot;                       // 0xBC
    char padC0[0x124 - 0xC0];
    sEquipItem_B180* items[(0x958 - 0x124) / 4]; // 0x124
    int count;                      // 0x958
};

// PORT: the unit declares cFEStateCharEquip_buildItemList as void; the body returns the item count.
extern "C" int cFEStateCharEquip_buildItemList_impl(void* selfv, int cat) __asm__("cFEStateCharEquip_buildItemList");
/**
 * Builds the equip screen's item list for a category.
 *
 * Lists the category's items (cBEBoltDB_collectCategoryBolts) into the list at +0x124 (count +0x958, cleared first), keeping only named
 * items and categories. In the store (mode 1): items whose character entry lacks flag 2 and whose store number matches
 * +0xCC (or +0xCC is -1), and categories with something to buy (cFEStateCharEquip_isStoreCategoryEmpty). In the inventory: items whose entry has
 * flag 2 and categories holding one (cFEStateCharEquip_hasInventoryItemInCategory). Sorts the list (qsort with compareEquipItems).
 *
 * @param selfv  the equip state
 * @param cat  the category
 * @return the number of entries
 */
// FAKEMATCH: identical arms `if (self) { if (it->name == 0) continue; } else { <same> }` in the second
// loop (cross-jumped away after reload; before that they lengthen the live range of the self->items
// copy, so global-alloc gives the loop-1 counter s5 and the copy s6 as in retail); plain form 99.01%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int cFEStateCharEquip_buildItemList_impl(void* selfv, int cat)
{
    sCharEquip_B180* self = (sCharEquip_B180*)selfv;
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int lib = (int)cBEBoltInterface_getBoltDB_noarg();
    cBECharProfileDB* db = (cBECharProfileDB*)cBEBoltInterface_getCharProfile(iface, self->team, self->slot);
    sEquipItem_B180* list[528];
    int n = cBEBoltDB_collectCategoryBolts(lib, self->slot, cat, list, -1, 0);
    self->count = 0;
    MEM_fill(self->items, 0, 2100);
    if (isMode1_B180((cFEStateCharEquip*)self)) {
        for (int i = 0; i < n; i++) {
            sEquipItem_B180* it = list[i];
            if (it->name == 0)
                continue;
            if (!(it->flags & 4)) {
                if (!(it->flags & 0x20))
                    continue;
            }
            tBEProfileBolt* entry = getEntry_B180(db, it->id);
            if ((it->flags & 0x20) && cFEStateCharEquip_isStoreCategoryEmpty((cFEStateCharEquip*)self, lib, (cBECharProfileDB*)db, it->id) == 0) {
                self->items[self->count] = it;
                self->count++;
            } else if (!(it->flags & 0x20) && !(entry->flags & 2)
                       && (*(int*)((char*)self + 0xCC) == -1 || *(int*)((char*)self + 0xCC) == it->slot)) {
                self->items[self->count] = it;
                self->count++;
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            sEquipItem_B180* it = list[i];
            if (self) {
                if (it->name == 0)
                    continue;
            } else {
                if (it->name == 0)
                    continue;
            }
            if (!(it->flags & 4)) {
                if (!(it->flags & 0x20))
                    continue;
            }
            tBEProfileBolt* entry = getEntry_B180(db, it->id);
            if ((!(it->flags & 0x20) && (entry->flags & 2))
                || ((it->flags & 0x20) && cFEStateCharEquip_hasInventoryItemInCategory((cFEStateCharEquip*)self, lib, db, it->id) != 0)) {
                self->items[self->count] = it;
                self->count++;
            }
        }
    }
    qsort_B180(self->items, self->count, 4, compareEquipItems);
    return self->count;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_removeItem);
#ifdef SKIP_ASM
struct sEquipListWidget {
    char pad_0x0[0x95];
    unsigned char sel;   // 0x95
    char pad_0x96[2];
    unsigned char top;   // 0x98
};

struct sEquipDetail {
    char pad_0x0[0x5C];
    sEquipListWidget* list;   // 0x5C
    char pad_0x60[0x124 - 0x60];
    int items[(0x958 - 0x124) / 4];  // 0x124
    int count;   // 0x958
};

/**
 * Removes entry @p idx from the equip screen's item list and keeps the menu's top row and selection in range.
 *
 * cFEStateCharEquip_onPopupEvent calls it after a purchase.
 *
 * @param idx  the list position
 * @return true, or false when @p idx is past the end
 */
extern "C" bool cFEStateCharEquip_removeItem(sEquipDetail* self, int idx)
{
    int top, sel, n;
    if (idx >= self->count) {
        return false;
    }
    for (; idx < self->count - 1; idx++) {
        self->items[idx] = self->items[idx + 1];
    }
    self->count--;
    self->items[self->count] = 0;
    n = self->count;
    top = self->list->top;
    sel = self->list->sel;
    if (n < top + 6) {
        top = n - 6;
    }
    if (sel >= n) {
        sel = n - 1;
    }
    if (top < 0) {
        top = 0;
    }
    self->list->top = top;
    if (sel < 0) {
        sel = 0;
    }
    self->list->sel = sel;
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_drawInventoryRows);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_drawInventoryRow(cFEStateCharEquip* self, int idx);

/**
 * Draws the inventory rows from the menu's top row (cFEStateCharEquip_drawInventoryRow), up to six or the end of the list.
 *
 * @return the number of rows drawn
 */
int cFEStateCharEquip::drawInventoryRows()
{
    int j = mpMenu->mScrollItemOffset;
    int i = 0;
    while (i < 6 && j < mNumItems) {
        drawInventoryRow(i);
        i++;
        j++;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_drawStoreRows);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_drawStoreRow(cFEStateCharEquip* self, int idx);

/**
 * Draws the store rows from the menu's top row (cFEStateCharEquip_drawStoreRow), up to six or the end of the list.
 *
 * @return the number of rows drawn
 */
int cFEStateCharEquip::drawStoreRows()
{
    int j = mpMenu->mScrollItemOffset;
    int i = 0;
    while (i < 6 && j < mNumItems) {
        drawStoreRow(i);
        i++;
        j++;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_redrawRows);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int cFEStateCharEquip_drawInventoryRows(cFEStateCharEquip* self);
extern "C" int cFEStateCharEquip_drawStoreRows(cFEStateCharEquip* self);
extern "C" void cUIMenu_setScrollableMenu(void* menu, int n);
extern char kColourEnabled[];
extern char str_kT_FEEmpty[];

static inline int isMode1_B618(cFEStateCharEquip* self)
{
    return self->mEquipMode == 1;
}

/**
 * Redraws the equip screen's item rows.
 *
 * Hides the six name (+0x8C) and check (+0xA4) widgets, sets the menu's item count when @p reset is set (the count, or
 * 1 for an empty list) and draws the rows for the mode. With nothing drawn shows kT_FEEmpty in the first row (colour
 * kColourEnabled) and resets the menu to one row. The visible row is clamped (0 when @p reset is set) and selected when @p
 * select is set.
 *
 * @param reset   nonzero to reset the menu to the top of a new list
 * @param select  nonzero to select the visible row
 */
void cFEStateCharEquip::redrawRows(int reset, int select)
{
    if (mpMenu == 0)
        return;
    cUIText** nameWidgets = mpBoltNameWidgets;
    cUIIcon** checkWidgets = mpBoltCheckWidgets;
    for (int i = 0; i < 6; i++) {
        nameWidgets[i]->setVisible(0);
        checkWidgets[i]->setVisible(0);
    }
    if (reset) {
        if (mNumItems == 0)
            cUIMenu_setScrollableMenu(mpMenu, 1);
        else
            cUIMenu_setScrollableMenu(mpMenu, (unsigned char)mNumItems);
    }
    int n;
    if (isMode1_B618(this))
        n = drawStoreRows();
    else
        n = drawInventoryRows();
    int visibleRow;
    if (n == 0) {
        mpBoltNameWidgets[0]->setVisible(1);
        mpBoltNameWidgets[0]->setColour((const tARGB*)kColourEnabled);
        cUIText_setUnicodeStringByID(mpBoltNameWidgets[0], GetHashValue32(str_kT_FEEmpty));
        mpMenu->mScrollItemNum = 1;
        mpMenu->mScrollItemOffset = 0;
        visibleRow = 0;
        n = 1;
    } else {
        cUIMenu* scrollList = mpMenu;
        visibleRow = scrollList->mScrollItemCurrent - scrollList->mScrollItemOffset;
    }
    if (visibleRow >= n)
        visibleRow = n - 1;
    if (visibleRow < 0)
        visibleRow = 0;
    if (reset)
        visibleRow = 0;
    if (select) {
        cUIMenu* scrollList = mpMenu;
        cUIMenu_setSelectedByIndex(scrollList, scrollList->mScrollItemOffset + visibleRow);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_drawInventoryRow);
#ifdef SKIP_ASM
extern "C" int cBEBoltInterface_trySetBoltVisible(void* self, int a1, int a2, int id, int locked);
extern "C" void* MEM_copy(void* dst, void* src, int n);
extern "C" void cBEBoltInterface_restoreBolts(void* self, int a1, int a2, void* src);
extern "C" int cBEBoltInterface_getUsedEquipPoints(void* self, int a1, int a2);

static inline tBEProfileBolt* getEntry_19B7E0(cBECharProfileDB* p, int idx)
{
    short k = p->mpBoltsMap[idx];
    if (k >= 0)
        return &p->mBolts[k];
    return 0;
}

static inline int IsSet_19B7E0(tBEProfileBolt* e)
{
    return e->flags & 4;
}

/**
 * Draws inventory row @p i.
 *
 * Writes the item's name (a string id for categories). For an item, tries toggling its equipped flag (cBEBoltInterface_trySetBoltVisible) on
 * a saved copy of the character's entries, stores the change in equip points (cBEBoltInterface_getUsedEquipPoints) in the row's points
 * (+0xA78, else -99999) and shows the checkbox (+0xA98) when the item is equipped or the empty box (+0xA9C) when it is
 * not; a failed toggle shows no box. The entries are restored afterwards (cBEBoltInterface_restoreBolts). A category gets the texture
 * at +0xAA0 (0, so no box).
 *
 * @param i  the row (0-5)
 */
void cFEStateCharEquip::drawInventoryRow(int i)
{
    cUIMenu* scrollList = mpMenu;
    int top = scrollList ? scrollList->mScrollItemOffset : 0;
    tBEBoltDef* item = mpItems[top + i];
    cUIText* text = mpBoltNameWidgets[i];
    cUIIcon* icon = mpBoltCheckWidgets[i];
    mRowPoints[i] = -99999;
    const cTextureElement* col = 0;
    if (item->flags & 0x20) {
        col = mpPlusElem;
    } else {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        cBECharProfileDB* prof = (cBECharProfileDB*)cBEBoltInterface_getCharProfile(iface, mUserNum, mCharID);
        tBEProfileBolt* flagEntry = getEntry_19B7E0(prof, item->id);
        tBEProfileBolt save[525];
        MEM_copy(save, &prof->mBolts[0], 2100);
        int before = cBEBoltInterface_getUsedEquipPoints(iface, mUserNum, mCharID);
        if (cBEBoltInterface_trySetBoltVisible(iface, mUserNum, mCharID, item->id, !IsSet_19B7E0(flagEntry))) {
            mRowPoints[i] = cBEBoltInterface_getUsedEquipPoints(iface, mUserNum, mCharID) - before;
            if (IsSet_19B7E0(flagEntry))
                col = mpCheckMarkElem;
            else
                col = mpEmptyBoxElem;
        }
        cBEBoltInterface_restoreBolts(iface, mUserNum, mCharID, save);
    }
    if (text) {
        if (item->flags & 0x20)
            cUIText_setUnicodeStringByID(text, GetHashValue32(item->name));
        else
            cUIText_setAsciiString(text, item->name);
        text->setVisible(1);
    }
    if (icon) {
        if (col) {
            icon->setVisible(1);
            icon->mpTextureElem = col;
            icon->mTextureID = -1;
        } else {
            icon->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_drawStoreRow);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);

/**
 * Draws store row @p i: the item's name and a green (mpGreenDollarSignElem) or red (mpRedDollarSignElem) dollar sign
 * by whether the money (+0xC0) covers its price x 10.
 *
 * A category shows its name as a string id and mpPlusElem (0, so no icon).
 *
 * @param i  the row (0-5)
 */
void cFEStateCharEquip::drawStoreRow(int i)
{
    cUIMenu* menu = mpMenu;
    int top = 0;
    if (menu != 0)
        top = menu->mScrollItemOffset;
    tBEBoltDef* item = mpItems[top + i];
    cUIText* text = mpBoltNameWidgets[i];
    cUIIcon* icon = mpBoltCheckWidgets[i];
    const cTextureElement* tex;
    if (item->flags & 0x20) {
        tex = mpPlusElem;
    } else {
        int price = item->price;
        if (price > 0)
            price *= 10;
        if (mMoneyAvailable < price)
            tex = mpRedDollarSignElem;
        else
            tex = mpGreenDollarSignElem;
    }
    if (text != 0) {
        if (item->flags & 0x20)
            cUIText_setUnicodeStringByID(text, GetHashValue32(item->name));
        else
            cUIText_setAsciiString(text, item->name);
        text->setVisible(1);
    }
    if (icon != 0) {
        if (tex != 0) {
            icon->setVisible(1);
            icon->mpTextureElem = tex;
            icon->mTextureID = -1;
        } else {
            icon->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateBankWidget);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);

/**
 * Shows the money (+0xC0) in the bank text (+0x64) through getDollarString.
 */
void cFEStateCharEquip::updateBankWidget()
{
    if (mpBank != 0) {
        cUIText_setAsciiString(mpBank, getDollarString(mMoneyAvailable));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateHeading);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_noarg() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" int cBEBoltDB_findBoltIndex(void* self, int g, int key);
extern int kBolt_None;
extern char str_kT_FECategories[];


static inline tBEBoltDef* findEntryK19BBE0(cBEBoltDB* t, int g, int key)
{
    int idx = t->findBoltIndex(g, key);
    if (idx < 0) {
        return 0;
    }
    return &t->mpCharBolts[idx];
}

/**
 * Shows the current category in the heading text: kT_FECategories at the root, else the category's name.
 *
 * For an @p id other than -1 (kBolt_None), looks the category up for the character (0xBC) with
 * cBEBoltDB_findBoltIndex() in the equip table from cBEBoltInterface_getBoltDB() and shows the string whose ID is the hash of its
 * name (+0x14); a missing entry is not checked. Nothing happens without a heading widget (0x58).
 *
 * @param id  the category id, or -1 for the root
 */
void cFEStateCharEquip::updateHeading(int id)
{
    if (mpHeading != 0) {
        if (id == kBolt_None) {
            cUIText_setUnicodeStringByID(mpHeading, GetHashValue32(str_kT_FECategories));
        } else {
            tBEBoltDef* entry = findEntryK19BBE0((cBEBoltDB*)cBEBoltInterface_getBoltDB_noarg(), mCharID, id);
            cUIText_setUnicodeStringByID(mpHeading, GetHashValue32(entry->name));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePolyBar);
#ifdef SKIP_ASM
extern int cFEStateCharEquip_polyLimit;



/**
 * Shows the equip points bar (+0x70) scaled to the current points (+0xC4).
 *
 * The bar's scale (+0x50) becomes (1, -t, 1) with t = (points - 1500) / (cFEStateCharEquip_polyLimit - 1500), not below 0 (there is no
 * upper clamp).
 */
void cFEStateCharEquip::updatePolyBar()
{
    cUIThing* pointsBar = mpPointsBar;
    if (((char*)pointsBar) != 0) {
        float fillRatio = (float)(mCurrentPolyCount - 1500) / (float)(cFEStateCharEquip_polyLimit - 1500);
        if (fillRatio < 0.0f) {
            fillRatio = 0.0f;
        }
        pointsBar->setVisible(1);
        t3Vector scale;
        scale.x = 1.0f;
        scale.y = -fillRatio;
        scale.z = 1.0f;
        mpPointsBar->mScale = scale;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePolyEquipPreviewBar);
#ifdef SKIP_ASM
extern int cFEStateCharEquip_polyLimit;
extern char kInvalidColour[];
extern char kOriginalColour[];




/**
 * Shows the equip points preview bar (+0x74) for the current points plus @p delta.
 *
 * Scales it (+0x50) to (1, -t, 1) with t = (points + delta - 1500) / (cFEStateCharEquip_polyLimit - 1500) clamped to 0-1, and colours it
 * (vtable entry 11) kInvalidColour when the total would pass cFEStateCharEquip_polyLimit, else kOriginalColour.
 *
 * @param delta  the selected item's points
 */
void cFEStateCharEquip::updatePolyEquipPreviewBar(int delta)
{
    if (mpPointsEquipBar == 0)
        return;
    float fillRatio = (float)(mCurrentPolyCount + delta - 1500) / (float)(cFEStateCharEquip_polyLimit - 1500);
    if (fillRatio > 1.0f)
        fillRatio = 1.0f;
    else if (fillRatio < 0.0f)
        fillRatio = 0.0f;
    if (cFEStateCharEquip_polyLimit < mCurrentPolyCount + delta) {
        cUIThing* equipBar = mpPointsEquipBar;
        equipBar->setColour((const tARGB*)kInvalidColour);
    } else {
        cUIThing* equipBar = mpPointsEquipBar;
        equipBar->setColour((const tARGB*)kOriginalColour);
    }
    {
        cUIThing* equipBar = mpPointsEquipBar;
        equipBar->setVisible(1);
    }
    t3Vector scale;
    scale.x = 1.0f;
    scale.y = -fillRatio;
    scale.z = 1.0f;
    mpPointsEquipBar->mScale = scale;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_buySelectedItem);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEBoltInterface_buyBolt(void* iface, int a1, int a2, int a3, int a4);

/**
 * Buys the selected item: stores the result of cBEBoltInterface_buyBolt (BE interface 9, the user, the character, the item's id,
 * 1) as the money (+0xC0).
 *
 * cFEStateCharEquip_onPopupEvent calls it when the buy popup closes with a yes.
 */
void cFEStateCharEquip::buySelectedItem()
{
    int idx = mpMenu->mScrollItemCurrent;
    tBEBoltDef* item = mpItems[idx];
    mMoneyAvailable = cBEBoltInterface_buyBolt(cBE_getInterface_Fv(cBE_getBE(), 9),
        mUserNum, mCharID, item->id, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_toggleSelectedItem);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBEBoltInterface_getCharProfile(void* self, int a1, int a2);
extern "C" int cBEBoltInterface_trySetBoltVisible(void* self, int a1, int a2, int id, int locked);
extern "C" void cBEBoltInterface_storeVisibleFlags(void* self, int a1, int a2);


static inline tBEProfileBolt* getItemK19BEE8(cBECharProfileDB* l, int id)
{
    int idx = l->mpBoltsMap[id];
    if (idx >= 0)
        return &((&l->mBolts[0]))[idx];
    return 0;
}

/**
 * Equips or takes off the selected inventory item.
 *
 * For a selected item (flag 4) toggles its equipped flag (entry flag 4) with cBEBoltInterface_trySetBoltVisible and, on success, calls
 * cBEBoltInterface_storeVisibleFlags for the character.
 *
 * @return true when the item changed, else false
 */
bool cFEStateCharEquip::toggleSelectedItem()
{
    if (mNumItems <= 0)
        return false;
    tBEBoltDef* item = mpItems[mpMenu->mScrollItemCurrent];
    if (!(item->flags & 4))
        return false;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    cBECharProfileDB* list = (cBECharProfileDB*)cBEBoltInterface_getCharProfile(iface, mUserNum,
                                                              mCharID);
    tBEProfileBolt* entry = getItemK19BEE8(list, item->id);
    int mask = 4;
    if (cBEBoltInterface_trySetBoltVisible(iface, mUserNum, mCharID, item->id,
                      (entry->flags & mask) == 0)) {
        cBEBoltInterface_storeVisibleFlags(iface, mUserNum, mCharID);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BFE0);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty function that returns at once. Nothing in the build references it.
 */
extern "C" void func_0019BFE0(void) {

}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_update3DPositions);
#ifdef SKIP_ASM
// FAKEMATCH: identical arms `if (self) { state = 2; } else { state = 2; }` in the state-3 arrival test (cross-jumping merges them after
// reload; before that they lengthen yaw's live range from 166 to 169 insns, so global-alloc ranks the 0.0174533 constant above yaw and
// they take retail's fs3/fs4); plain form 99.95%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void fSinCos(float angle, float* s, float* c);

struct sQuat_BFE8
{
    float x, y, z, w;
    sQuat_BFE8() {}
    sQuat_BFE8(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct sXform_BFE8
{
    sQuat_BFE8 pos;
    sQuat_BFE8 rot;
};

extern sQuat_BFE8 t4vect_XAxis;
extern sQuat_BFE8 t4vect_YAxis;
extern sQuat_BFE8 t4vect_ZAxis;
extern "C" void cFERider_setPose(void* rider, sXform_BFE8* xf);
extern "C" void cFERider_setBoardPose(void* rider, sXform_BFE8* xf);
extern "C" void cSimpleCamera_setupCamWithPosAndTarget(void* cam, sQuat_BFE8* a, sQuat_BFE8* b);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_BFE8 QuatMul_BFE8(const sQuat_BFE8& a, const sQuat_BFE8& b)
{
    sQuat_BFE8 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sQuat_BFE8 AxisAngle_BFE8(const sQuat_BFE8& axis, float angle)
{
    float s, c;
    fSinCos(angle * 0.5f, &s, &c);
    return sQuat_BFE8(s * axis.x, s * axis.y, s * axis.z, c);
}

// PORT: PS2-only VU0 macro-mode asm (vector difference a - b).
static inline sQuat_BFE8 VecSub_BFE8(const sQuat_BFE8& a, const sQuat_BFE8& b)
{
    sQuat_BFE8 r;
    __asm__ (
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (vector times scalar).
static inline sQuat_BFE8 VecScale_BFE8(const sQuat_BFE8& v, float s)
{
    sQuat_BFE8 r;
    int t;
    __asm__ (
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

// PORT: PS2-only VU0 macro-mode asm (dst = a + b).
static inline void VecAdd_BFE8(sQuat_BFE8& dst, const sQuat_BFE8& a, const sQuat_BFE8& b)
{
    __asm__ (
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(a), "m"(b));
}

// PORT: PS2-only VU0 macro-mode asm (4-component dot product).
static inline float VecDot_BFE8(const sQuat_BFE8& a, const sQuat_BFE8& b)
{
    float r;
    __asm__ (
        "lqc2      $vf3, %1\n"
        "lqc2      $vf5, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        : "=r"(r)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Poses the user's front-end rider and board for the equip screen and sets the front-end camera.
 *
 * Once the rider is loaded (character below 10, +0xCB8 and +0xCB4 set): camera state (+0xAE0) 1 puts the rider at
 * (-105, -100 + zoom x 320, -80 + pan x zoom x 65) (zoom +0xAA4, pan +0xAA8) and the board at (265.75, -1073.5, -232);
 * state 2 puts the rider at (-250, -650, -215) and the board at (-100, 45, 15); states 3 and 4 ease (0.2 of the gap per
 * call) toward the state-2 and state-1 positions and switch to that state when close. The rider (+0xAC0) is turned
 * +0xAAC + 80 degrees about Z (cFERider_setPose); the board (+0xAD0) is turned 245 degrees about Z, 320 about Y and its
 * spin angle (+0xAB0, one degree more per call) + 90 about X (cFERider_setBoardPose). Always places the camera (RealFE + 0x10,
 * cSimpleCamera_setupCamWithPosAndTarget) at (-110, 394, 0) looking at (-35, 0, 0) and stores 0.43633 (25 degrees) in its first word.
 *
 * @param vself  the equip state
 */
extern "C" void cFEStateCharEquip_update3DPositions(cFEStateCharEquip* vself)
{
    cFERider* rider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, vself->mUserNum);
    int ok = 0;
    if (*(unsigned int*)rider < 10 && rider->mDoneLoadingTextures != 0) {
        ok = rider->mCompiledModel != 0;
    }
    if (ok) {
        sXform_BFE8 xf;
        float zero;
        float zoom = vself->mZoom;
        float zOff = vself->mPan * zoom * 65.0f;
        float yOff = zoom * 320.0f;
        float yaw = vself->mPlayerRotation;
        int state = vself->mCameraState;
        if (state == 1) {
            sQuat_BFE8 point;
            point.w = 1.0f;
            point.x = -105.0f;
            point.y = yOff + -100.0f;
            point.z = zOff + -80.0f;
            *(sQuat_BFE8*)&vself->mCharPos = point;
            point.x = 265.75f;
            point.y = -1073.5f;
            point.z = -232.0f;
            *(sQuat_BFE8*)&vself->mBoardPos = point;
        } else if (state == 2) {
            sQuat_BFE8 point;
            point.x = -250.0f;
            point.y = -650.0f;
            point.z = -215.0f;
            point.w = 1.0f;
            *(sQuat_BFE8*)&vself->mCharPos = point;
            point.x = -100.0f;
            point.y = 45.0f;
            point.z = 15.0f;
            *(sQuat_BFE8*)&vself->mBoardPos = point;
        } else if (state == 3) {
            sQuat_BFE8 pos;
            sQuat_BFE8 lookAt;
            pos.x = -250.0f;
            pos.y = -650.0f;
            pos.z = -215.0f;
            pos.w = 1.0f;
            lookAt.x = -100.0f;
            lookAt.y = 45.0f;
            lookAt.z = 15.0f;
            lookAt.w = 1.0f;
            sQuat_BFE8 dPos = VecSub_BFE8(pos, *(sQuat_BFE8*)&vself->mCharPos);
            sQuat_BFE8 dAt = VecSub_BFE8(lookAt, *(sQuat_BFE8*)&vself->mBoardPos);
            VecAdd_BFE8(*(sQuat_BFE8*)&vself->mCharPos, *(sQuat_BFE8*)&vself->mCharPos, VecScale_BFE8(dPos, 0.2f));
            VecAdd_BFE8(*(sQuat_BFE8*)&vself->mBoardPos, *(sQuat_BFE8*)&vself->mBoardPos, VecScale_BFE8(dAt, 0.2f));
            if (VecDot_BFE8(dAt, dAt) < 0.2f) {
                if (vself) { // FAKEMATCH: identical arms, see header
                    vself->mCameraState = 2;
                } else {
                    vself->mCameraState = 2;
                }
            }
        } else if (state == 4) {
            sQuat_BFE8 pos;
            sQuat_BFE8 lookAt;
            pos.x = -105.0f;
            pos.y = yOff + -100.0f;
            pos.z = zOff + -80.0f;
            pos.w = 1.0f;
            lookAt.x = 265.75f;
            lookAt.y = -1073.5f;
            lookAt.z = -232.0f;
            lookAt.w = 1.0f;
            sQuat_BFE8 dPos = VecSub_BFE8(pos, *(sQuat_BFE8*)&vself->mCharPos);
            sQuat_BFE8 dAt = VecSub_BFE8(lookAt, *(sQuat_BFE8*)&vself->mBoardPos);
            VecAdd_BFE8(*(sQuat_BFE8*)&vself->mCharPos, *(sQuat_BFE8*)&vself->mCharPos, VecScale_BFE8(dPos, 0.2f));
            VecAdd_BFE8(*(sQuat_BFE8*)&vself->mBoardPos, *(sQuat_BFE8*)&vself->mBoardPos, VecScale_BFE8(dAt, 0.2f));
            if (VecDot_BFE8(dPos, dPos) < 0.2f) {
                vself->mCameraState = 1;
            }
        }
        xf.rot.x = 0.0f;
        xf.rot.w = 1.0f;
        xf.rot.y = 0.0f;
        xf.rot.z = 0.0f;
        xf.pos = *(sQuat_BFE8*)&vself->mCharPos;
        zero = 0.0f;

        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_XAxis, zero));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_YAxis, zero));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_ZAxis, (yaw + 80.0f) * 0.01745329424738884f));
        cFERider_setPose(rider, &xf);
        xf.pos = *(sQuat_BFE8*)&vself->mBoardPos;
        xf.rot.w = 1.0f;
        xf.rot.x = zero;
        xf.rot.y = zero;
        xf.rot.z = zero;
        vself->mBoardRotation += 1.0f;
        if (vself->mBoardRotation >= 360.0f) {
            vself->mBoardRotation -= 360.0f;
        }
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_ZAxis, 245.0f * 0.01745329424738884f));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_YAxis, 320.0f * 0.01745329424738884f));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_XAxis, (vself->mBoardRotation + 90.0f) * 0.01745329424738884f));
        cFERider_setBoardPose(rider, &xf);
    }
    cRealFE* cam = (cRealFE*)SSXApp->RealFE;
    sQuat_BFE8 pos;
    sQuat_BFE8 lookAt;
    pos.x = -110.0f;
    pos.y = 394.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
    lookAt.x = -35.0f;
    lookAt.y = 0.0f;
    lookAt.z = 0.0f;
    lookAt.w = 1.0f;
    cSimpleCamera_setupCamWithPosAndTarget(&cam->mCamera, &pos, &lookAt);
    cam->mCamera.View = 0.4363323450088501f;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_openBuyPopup);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* self, void* engine, void* owner, signed char idx);
extern "C" void cBuyPopupInfo_initBuyBolt(cBuyPopupInfo* self, const char* boltName, int cost, int money);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cUIStateBuyPopup[];

/**
 * Opens the buy popup for an item.
 *
 * Constructs a cUIStateBuyPopup (0x70 bytes) for the user, sets it up for the item (cBuyPopupInfo_initBuyBolt with the
 * item's name, its price x 10 and the money mMoneyAvailable) and pushes it on the state stack.
 *
 * @param item  the item
 */
void cFEStateCharEquip::openBuyPopup(tBEBoltDef* item)
{
    cUIStateBuyPopup* popup = (cUIStateBuyPopup*)cUIStateBuyPopup_cUIStateBuyPopup(
        cMemMan_alloc(0x70, str_cUIStateBuyPopup, 0, 0), mpEngine, this, mUserNum);
    int price = item->price;
    const char* name = item->name;
    if (price > 0) {
        price *= 10;
    }
    cBuyPopupInfo_initBuyBolt(&popup->mInfo, name, price, mMoneyAvailable);
    cUIStateStack_pushPopup(&mpEngine->mStateStack, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_allocNode);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_getNextNode(cBoltFileIndexManager* self, tBoltFileIndexNode* item);

/**
 * Takes the first node off the node pool's free list (+0x1F40) and returns it.
 *
 * The next node (cBoltFileIndexManager_getNextNode) becomes the head with its previous index (+0xC) cleared; the taken node's next index
 * (+0xE) is set to -1. Likely bug: an empty free list is not checked, so the null head is written through.
 *
 * @return the node
 */
tBoltFileIndexNode* cBoltFileIndexManager::allocNode()
{
    tBoltFileIndexNode* item = mpFreeList;
    mpFreeList = getNextNode(item);
    item->next = -1;
    tBoltFileIndexNode* head = mpFreeList;
    if (head != 0) {
        head->prev = -1;
    }
    return item;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_getPrevNode);
#ifdef SKIP_ASM
/**
 * Returns the node before @p item: the pool's 16-byte node at index +0xC of @p item.
 *
 * @param item  the node, or null
 * @return the previous node, or null when there is none
 */
tBoltFileIndexNode* cBoltFileIndexManager::getPrevNode(tBoltFileIndexNode* item)
{
    if (item != 0) {
        short idx = item->prev;
        if (idx >= 0) {
            return &mNodes[idx];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_getNextNode);
#ifdef SKIP_ASM
/**
 * Returns the node after @p item: the pool's 16-byte node at index +0xE of @p item.
 *
 * @param item  the node, or null
 * @return the next node, or null when there is none
 */
tBoltFileIndexNode* cBoltFileIndexManager::getNextNode(tBoltFileIndexNode* item)
{
    if (item != 0) {
        short idx = item->next;
        if (idx >= 0) {
            return &mNodes[idx];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_getNodeIndex);
#ifdef SKIP_ASM
/**
 * Returns the pool index of the node at @p node ((address - pool) / 16).
 *
 * @param node  the node, or 0
 * @return the index, or -1 for 0
 */
int cBoltFileIndexManager::getNodeIndex(tBoltFileIndexNode* node)
{
    if (node != 0) return node - mNodes;
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_findNode);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_allocNode(cBoltFileIndexManager* self);
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_getNextNode(cBoltFileIndexManager* self, tBoltFileIndexNode* item);
extern "C" int cBoltFileIndexManager_getNodeIndex(cBoltFileIndexManager* self, tBoltFileIndexNode* node);

/**
 * Finds the node for a two-part key in the file-index hash table, optionally creating it.
 *
 * The table (allocated as "BoltFileIndexManager", 8004 bytes; cBoltFileIndexManager_instance) holds 400 16-byte nodes at +0x0, 400 bucket
 * head pointers at +0x1900 and the free-list head at +0x1F40. The key is (@p hi << 16) | @p lo and its bucket is key %
 * 400. Walks the bucket's chain (next index at node +0xE, via cBoltFileIndexManager_getNextNode) for a node with that key. When the chain
 * ends and @p create is set, takes a node from the free list (cBoltFileIndexManager_allocNode) and appends it: its prev index (+0xC) is
 * the old tail's index (cBoltFileIndexManager_getNodeIndex) or -1 when it becomes the bucket head, and its next index is -1. Whatever node
 * results gets the key stored at +0x0.
 *
 * The search also stops when the free list is empty and the chain is exhausted, so a missing key with no free node
 * returns 0 even with @p create set.
 *
 * @param hi  high half of the key (the rider manager passes the rider's +0x4, its profile index, also used as the mask
 *            bit)
 * @param lo  low half of the key (the callers pass an item id)
 * @param create  nonzero to add a node when the key is missing
 * @return the node, or 0 when the key is missing and no node was created
 */
tBoltFileIndexNode* cBoltFileIndexManager::findNode(int hi, int lo, int create)
{
    int key = (hi << 16) | lo;
    int bucket = key % 400;
    tBoltFileIndexNode* prev = 0;
    tBoltFileIndexNode* n = mBuckets[bucket];
    while (mpFreeList != 0 || n != 0) {
        if (n == 0) {
            if (create) {
                n = allocNode();
                n->prev = getNodeIndex(prev);
                if (prev != 0) {
                    n->prev = getNodeIndex(prev);
                    prev->next = getNodeIndex(n);
                } else {
                    n->prev = -1;
                    mBuckets[bucket] = n;
                }
                n->next = -1;
            }
            break;
        }
        if (n->key == key)
            break;
        prev = n;
        n = getNextNode(prev);
    }
    if (n != 0)
        n->key = key;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_freeNode);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_getPrevNode(cBoltFileIndexManager* self, tBoltFileIndexNode* item);
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_getNextNode(cBoltFileIndexManager* self, tBoltFileIndexNode* item);
extern "C" int cBoltFileIndexManager_getNodeIndex(cBoltFileIndexManager* self, tBoltFileIndexNode* node);

/**
 * Unlinks a node from its hash bucket in the file-index table and returns it to the free list.
 *
 * Does nothing for a null @p node. Otherwise the bucket (key % 400) head at +0x1900 or the previous node's next index
 * takes the node's next, the next node's prev index takes the node's prev, the node's key and both values (+0x8, +0xA)
 * and prev index become -1, and the node is pushed onto the free list at +0x1F40 (its next index is the old head's
 * index, cBoltFileIndexManager_getNodeIndex).
 *
 * @param node  the node to release (from cBoltFileIndexManager_findNode), or 0
 */
void cBoltFileIndexManager::freeNode(tBoltFileIndexNode* node)
{
    if (node != 0) {
        int bucketIndex = node->key % 400;
        if (node->prev < 0) {
            mBuckets[bucketIndex] = getNextNode(node);
        } else {
            getPrevNode(node)->next = node->next;
        }
        if (node->next >= 0) {
            getNextNode(node)->prev = node->prev;
        }
        node->key = -1;
        node->modelPartFile = -1;
        node->partTextureFile = -1;
        node->prev = -1;
        node->next = getNodeIndex(mpFreeList);
        mpFreeList = node;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", __21cBoltFileIndexManager);
#ifdef SKIP_ASM
extern "C" void cBoltFileIndexManager_reset(cBoltFileIndexManager* self);

extern "C" cBoltFileIndexManager* __21cBoltFileIndexManager(cBoltFileIndexManager* self);

/**
 * Constructs the file-index hash table by resetting it with cBoltFileIndexManager_reset.
 */
cBoltFileIndexManager::cBoltFileIndexManager()
{
    reset();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_reset);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, unsigned int value, int size);

/**
 * Resets the file-index hash table to 400 free nodes and empty buckets.
 *
 * Fills the 400 16-byte nodes (6400 bytes) with 0xFF, clears the 400 bucket heads at +0x1900, points the free-list head
 * (+0x1F40) at node 0 and chains every node to its neighbours (prev i - 1, next i + 1, the last next -1) with both mask
 * words (+0x4, +0x6) cleared.
 */
void cBoltFileIndexManager::reset()
{
    MEM_fill(mNodes, 0xFFFFFFFF, sizeof(mNodes));
    MEM_fill(mBuckets, 0, sizeof(mBuckets));
    mpFreeList = mNodes;
    tBoltFileIndexNode* n = mNodes;
    for (int i = 0; i < 400; i++) {
        n[i].next = i + 1;
        n[i].prev = i - 1;
        n[i].partTextureMask = 0;
        n[i].modelPartMask = 0;
    }
    n[399].next = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_getModelPartFile);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_findNode(cBoltFileIndexManager* self, int hi, int lo, int create);

/**
 * Returns the second value stored for a key in the file-index hash table, or -1 when it is not set.
 *
 * Looks the node up without creating it (cBoltFileIndexManager_findNode) and returns its value at +0xA when bit @p bit of its mask at
 * +0x6 is set. The rider manager stores, per profile index and item id, the FE file cache slot of the item's type-8
 * file here (a cFEBoltFileManager_requestFile result).
 *
 * @param bit  high half of the key and the mask bit to test (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @return the stored value, or -1
 */
int cBoltFileIndexManager::getModelPartFile(int bit, int lowKey)
{
    tBoltFileIndexNode* node = findNode(bit, lowKey, 0);
    if (node != 0) {
        if ((node->modelPartMask >> bit) & 1) {
            return node->modelPartFile;
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_setModelPartFile);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_findNode(cBoltFileIndexManager* self, int hi, int lo, int create);
extern "C" void cBoltFileIndexManager_freeNode(cBoltFileIndexManager* self, tBoltFileIndexNode* node);

/**
 * Stores or clears the second value for a key in the file-index hash table.
 *
 * A @p val of kNoBoltFile (-1) looks the node up without creating it; any other value creates it if needed
 * (cBoltFileIndexManager_findNode). A negative @p val clears bit @p bit of the mask at +0x6 and, once that mask is 0 and the first value
 * (+0x8) is negative, frees the node (cBoltFileIndexManager_freeNode). Otherwise the value goes to +0xA and the mask bit is set.
 *
 * Quirk: the free test checks the first value rather than the first mask, and +0x8 is only reset to -1 when a node is
 * freed, so a node whose first value was ever set stays allocated after both masks clear.
 *
 * @param bit  high half of the key and the mask bit (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @param val  value to store, or a negative value to clear it
 */
void cBoltFileIndexManager::setModelPartFile(int bit, int lowKey, int val)
{
    tBoltFileIndexNode* node = findNode(bit, lowKey, val != kNoBoltFile);
    if (node != 0) {
        if (val < 0) {
            if ((node->modelPartMask >> bit) & 1) {
                node->modelPartMask &= ~(1 << bit);
                if (node->modelPartMask == 0 && node->partTextureFile < 0) {
                    freeNode(node);
                }
            }
        } else {
            node->modelPartFile = val;
            node->modelPartMask |= 1 << bit;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_getPartTextureFile);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_findNode(cBoltFileIndexManager* self, int hi, int lo, int create);

/**
 * Returns the first value stored for a key in the file-index hash table, or -1 when it is not set.
 *
 * Looks the node up without creating it (cBoltFileIndexManager_findNode) and returns its value at +0x8 when bit @p bit of its mask at
 * +0x4 is set. The rider manager stores, per profile index and item id, the FE file cache slot of the item's type-11
 * file here (a cFEBoltFileManager_requestFile result).
 *
 * @param bit  high half of the key and the mask bit to test (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @return the stored value, or -1
 */
int cBoltFileIndexManager::getPartTextureFile(int bit, int lowKey)
{
    tBoltFileIndexNode* node = findNode(bit, lowKey, 0);
    if (node != 0) {
        if ((node->partTextureMask >> bit) & 1) {
            return node->partTextureFile;
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cBoltFileIndexManager_setPartTextureFile);
#ifdef SKIP_ASM
extern "C" tBoltFileIndexNode* cBoltFileIndexManager_findNode(cBoltFileIndexManager* self, int hi, int lo, int create);
extern "C" void cBoltFileIndexManager_freeNode(cBoltFileIndexManager* self, tBoltFileIndexNode* node);

/**
 * Stores or clears the first value for a key in the file-index hash table.
 *
 * A @p val of kNoBoltFile (-1) looks the node up without creating it; any other value creates it if needed
 * (cBoltFileIndexManager_findNode). A negative @p val clears bit @p bit of the mask at +0x4 and, once that mask is 0 and the second
 * value (+0xA) is negative, frees the node (cBoltFileIndexManager_freeNode). Otherwise the value goes to +0x8 and the mask bit is set.
 * Same quirk as cBoltFileIndexManager_setModelPartFile: the free test checks the other value, not the other mask.
 *
 * @param bit  high half of the key and the mask bit (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @param val  value to store, or a negative value to clear it
 */
void cBoltFileIndexManager::setPartTextureFile(int bit, int lowKey, int val)
{
    tBoltFileIndexNode* node = findNode(bit, lowKey, val != kNoBoltFile);
    if (node != 0) {
        if (val < 0) {
            if ((node->partTextureMask >> bit) & 1) {
                node->partTextureMask &= ~(1 << bit);
                if (node->partTextureMask == 0 && node->modelPartFile < 0) {
                    freeNode(node);
                }
            }
        } else {
            node->partTextureFile = val;
            node->partTextureMask |= 1 << bit;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", __18cFEBoltFileManager);
#ifdef SKIP_ASM
extern cFEBoltFileManager* cFEBoltFileManager_instance;
extern "C" void cFEBoltFileManager_init(cFEBoltFileManager* self);

/**
 * Constructs the FE file cache: records it in cFEBoltFileManager_instance and initialises it with cFEBoltFileManager_init.
 *
 * cFE_cFE builds it at the front end's +0x1A70. cFEBoltFileManager_instance is what the sort comparators compareDeferredModelPartFiles and compareDeferredPartTextureFiles
 * read.
 */
cFEBoltFileManager::cFEBoltFileManager()
{
    cFEBoltFileManager_instance = this;
    init();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", _$_18cFEBoltFileManager);
#ifdef SKIP_ASM
extern cFEBoltFileManager* cFEBoltFileManager_instance;
extern "C" void cFEBoltFileManager_purge(cFEBoltFileManager* self);
void operator_delete(int* p);

/**
 * Destroys the FE file cache: clears cFEBoltFileManager_instance, purges every slot (cFEBoltFileManager_purge) and frees the object when bit 0 of
 * g++'s delete flag is set.
 */
cFEBoltFileManager::~cFEBoltFileManager()
{
    cFEBoltFileManager_instance = 0;
    purge();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_init);
#ifdef SKIP_ASM
extern cBoltFileIndexManager* cBoltFileIndexManager_instance;
extern char str_BoltFileIndexManager[];

/**
 * Initialises the FE file cache and its file-index hash table.
 *
 * For each of the 10 banks it clears all 256 slots (state, name, data, handle, size and use count `timer` to 0, type
 * -1) and sets the bank's stamp counter (counts) to 1; then clears used and nPending. The file-index table cBoltFileIndexManager_instance
 * is allocated on first use (cMemMan_alloc, 8004 bytes, tag "BoltFileIndexManager", built by cBoltFileIndexManager::cBoltFileIndexManager), otherwise
 * reset with cBoltFileIndexManager_reset. cRealFE_load calls it on the front end's +0x1A70.
 */
void cFEBoltFileManager::init()
{
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 256; j++) {
            tBoltFile* slot = &mFiles[i][j];
            slot->flags = 0;
            slot->fileName[0] = 0;
            slot->dataPtr = 0;
            slot->fileSize = 0;
            slot->asyncFileHandle = 0;
            slot->refCount = 0;
            slot->fileType = -1;
        }
        unk_B4008[i] = 1;
    }
    mNumLoadThreads = 0;
    mMemUsed = 0;
    if (cBoltFileIndexManager_instance == 0) {
        cBoltFileIndexManager_instance = new (str_BoltFileIndexManager, 0, 0) cBoltFileIndexManager;
    } else {
        cBoltFileIndexManager_instance->reset();
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_purge);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_freeFile(cFEBoltFileManager* self, int bank, int i);
void operator_delete(int* p);
extern cBoltFileIndexManager* cBoltFileIndexManager_instance;


/**
 * Empties the FE file cache and frees its file-index hash table.
 *
 * Releases every slot of the 10 banks with cFEBoltFileManager_freeFile and resets each bank's stamp counter to 1, then deletes
 * cBoltFileIndexManager_instance (operator_delete, no destructor call) and clears it. nPending is not reset here. Called by the cache's
 * destructor and by cRealFE_purge.
 */
void cFEBoltFileManager::purge()
{
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 256; j++) {
            freeFile(i, j);
        }
        unk_B4008[i] = 1;
    }
    if (cBoltFileIndexManager_instance != 0) {
        operator_delete((int*)cBoltFileIndexManager_instance);
    }
    cBoltFileIndexManager_instance = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_unpackFile);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* ptr);
extern "C" int unpacksizez(void* data);
extern "C" void unpack(void* src, void* dst);
extern "C" void cFEBoltFileManager_makeRoom(cFEBoltFileManager* self, int bank, int size);
extern "C" void cFEBoltFileManager_evictBytes(cFEBoltFileManager* self, int bank, int size);
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern char str_UncompressedData[];
extern char cMemMan_m_mutex[];


/**
 * Unpacks a slot's compressed file data in place in the FE file cache.
 *
 * Does nothing when @p rawSlot or its data is null or unpacksizez() reports no unpacked size. Otherwise it makes room
 * for the unpacked size (cFEBoltFileManager_makeRoom), allocates it (operator new, tag "UncompressedData", flags 0x60000000), and
 * while that fails it locks and unlocks cMemMan_m_mutex, evicts that many bytes (cFEBoltFileManager_evictBytes) and retries. It then
 * unpacks into the new block, frees the packed data, stores the new block as the slot's data and adjusts used by the
 * size difference. Called when an asynchronous load completes (cFEBoltFileManager_update, cFEBoltFileManager_releaseFile).
 *
 * @param bank  the slot's bank (0-9), passed to the eviction helpers
 * @param rawSlot  the slot
 */
void cFEBoltFileManager::unpackFile(int bank, tBoltFile* rawSlot)
{
    tBoltFile* slot = (tBoltFile*)rawSlot;
    if (slot == 0)
        return;
    if (slot->dataPtr == 0)
        return;
    int size = unpacksizez(slot->dataPtr);
    if (size <= 0)
        return;
    makeRoom(bank, size);
    void* buf = operator_new_tag(size, str_UncompressedData, 0x60000000, 0);
    while (buf == 0) {
        MUTEX_lock(cMemMan_m_mutex);
        MUTEX_unlock(cMemMan_m_mutex);
        evictBytes(bank, size);
        buf = operator_new_tag(size, str_UncompressedData, 0x60000000, 0);
    }
    unpack(slot->dataPtr, buf);
    if (slot->dataPtr != 0)
        cMemMan_free(slot->dataPtr);
    slot->dataPtr = buf;
    mMemUsed += size - slot->fileSize;
    slot->fileSize = size;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_startNextQueuedLoad);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cFEBoltFileManager_loadFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Starts the oldest queued load, preferring the given bank of the FE file cache.
 *
 * Picks the queued (kBoltFileFlag_Queued) slot with the lowest stamp in @p bank; when the bank has none, it searches
 * the other banks in order and takes the lowest-stamp queued slot of the first bank that has one. The chosen slot is
 * loaded with cFEBoltFileManager_loadFile.
 *
 * Quirk: a best stamp of 0 counts as "nothing chosen yet", so a queued slot with stamp 0 loses to any later queued
 * slot.
 *
 * @param bank  the bank to try first
 */
void cFEBoltFileManager::startNextQueuedLoad(int bank)
{
    unsigned int best = 0;
    int bestIdx = kNoBoltFile;
    int found;
    int i;
    tBoltFile* slot = mFiles[bank];
    found = bank;
    for (i = 0; i < 256; i++, slot++) {
        if (slot->flags == kBoltFileFlag_Queued && (best == 0 || slot->stamp < best)) {
            best = slot->stamp;
            bestIdx = i;
        }
    }
    if (bestIdx == kNoBoltFile) {
        int otherBank;
        for (otherBank = 0; otherBank < 10; otherBank++) {
            if (otherBank == bank) {
                continue;
            }
            slot = mFiles[otherBank];
            for (i = 0; i < 256; i++, slot++) {
                if (slot->flags == kBoltFileFlag_Queued && (best == 0 || slot->stamp < best)) {
                    best = slot->stamp;
                    found = otherBank;
                    bestIdx = i;
                }
            }
            if (bestIdx >= 0) {
                break;
            }
        }
    }
    if (bestIdx >= 0) {
        loadFile(found, bestIdx);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_update);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" void cFEBoltFileManager_unpackFile(cFEBoltFileManager* self, int bank, tBoltFile* slot);
extern "C" void cFEBoltFileManager_startNextQueuedLoad(cFEBoltFileManager* self, int bank);
extern "C" void cFEBoltFileManager_loadFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Polls the FE file cache's asynchronous loads once per frame (cRealFE_update).
 *
 * For every slot of the 10 banks: an asynchronous load in flight (pending set, kBoltFileFlag_Loading) whose
 * ASYNCFILE_getstatus() reports kASYNCFILE_Done is released (ASYNCFILE_release), unpacked (cFEBoltFileManager_unpackFile) and becomes
 * loaded (kBoltFileFlag_Done) or idle (kBoltFileFlag_Idle)
 * when its use count is 0; pending is cleared and nPending decremented, and when no loads remain in flight the next
 * queued load starts (cFEBoltFileManager_startNextQueuedLoad for that bank). A queued slot (kBoltFileFlag_Queued) starts loading (cFEBoltFileManager_loadFile)
 * whenever no loads are in flight.
 *
 */
void cFEBoltFileManager::update()
{
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        tBoltFile* slot = mFiles[i];
        for (j = 0; j < 256; j++, slot++) {
            if (slot->async != 0 && slot->flags == kBoltFileFlag_Loading) {
                int asyncStatus = ASYNCFILE_getstatus(slot->asyncFileHandle);
                if (asyncStatus == kASYNCFILE_Done) {
                    int status[4];
                    ASYNCFILE_release(slot->asyncFileHandle, 0, status);
                    unpackFile(i, slot);
                    if (slot->refCount == 0)
                        slot->flags = 0;
                    else
                        slot->flags = asyncStatus;
                    slot->async = 0;
                    if (--mNumLoadThreads <= 0)
                        startNextQueuedLoad(i);
                }
            } else if (mNumLoadThreads <= 0 && slot->flags == kBoltFileFlag_Queued) {
                loadFile(i, j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_findFreeSlot);
#ifdef SKIP_ASM
/**
 * Picks the slot to reuse for a new file in a bank of the FE file cache.
 *
 * Returns the first idle slot (state 0) that holds no data; otherwise the idle slot with the oldest stamp (least
 * recently used).
 *
 * @param bank  the bank (0-9)
 * @return the slot index, or -1 when no slot of the bank is idle
 */
int cFEBoltFileManager::findFreeSlot(int bank)
{
    unsigned int best = 0;
    int bestIdx = -1;
    tBoltFile* slot = mFiles[bank];
    for (int i = 0; i < 256; i++, slot++) {
        if (slot->flags == kBoltFileFlag_Idle) {
            if (slot->dataPtr == 0) {
                return i;
            }
            if (bestIdx < 0 || slot->stamp < best) {
                best = slot->stamp;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_findFile);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);

/**
 * Finds the slot holding a file name in a bank of the FE file cache.
 *
 * Compares @p name with every non-empty slot name using strcmp (a strcmp routine).
 *
 * @param bank  the bank (0-9)
 * @param name  the file name
 * @return the slot index, or -1 when the name is not cached
 */
int cFEBoltFileManager::findFile(int bank, const char* name)
{
    for (int i = 0; i < 256; i++) {
        tBoltFile* slot = &mFiles[bank][i];
        if (slot->fileName[0] != 0 && strcmp(mFiles[bank][i].fileName, name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_freeFile);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
void cMemMan_free(void* ptr);
/**
 * Frees one slot of the FE file cache.
 *
 * When an asynchronous load is in flight (kBoltFileFlag_Loading, pending) it releases the request (ASYNCFILE_release).
 * Subtracts the slot's size from used, frees its data and clears data, size, state, name, handle, pending and use
 * count; the type becomes -1. The stamp is kept and nPending is not adjusted.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 */
void cFEBoltFileManager::freeFile(int bank, int i)
{
    int status;
    tBoltFile* slot = &mFiles[bank][i];
    if (slot->flags == kBoltFileFlag_Loading) {
        if (slot->async != 0) {
            ASYNCFILE_release(slot->asyncFileHandle, 0, &status);
        }
    }
    mMemUsed -= slot->fileSize;
    if (slot->dataPtr != 0) {
        cMemMan_free(slot->dataPtr);
    }
    slot->dataPtr = 0;
    slot->fileSize = 0;
    slot->flags = kBoltFileFlag_Idle;
    slot->fileName[0] = 0;
    slot->asyncFileHandle = 0;
    slot->async = 0;
    slot->refCount = 0;
    slot->fileType = -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_makeRoom);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_freeFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Evicts idle cached files until a new file fits in the FE file cache's 9,999,999-byte budget.
 *
 * Returns at once when used plus @p size fits. Otherwise frees idle slots (data present, use count 0, state 0) of the
 * other banks in bank order, stopping as soon as it fits, then frees this bank's idle slots oldest stamp first until it
 * fits or none remain (cFEBoltFileManager_freeFile).
 *
 * @param bank  the bank being loaded into; its own files go last
 * @param size  bytes about to be added
 */
void cFEBoltFileManager::makeRoom(int bank, int size)
{
    if (mMemUsed + size <= 9999999)
        return;
    int otherBank;
    int j;
    for (otherBank = 0; otherBank < 10; otherBank++) {
        if (otherBank == bank)
            continue;
        for (j = 0; j < 256; j++) {
            tBoltFile* slot = &mFiles[otherBank][j];
            if (slot->dataPtr != 0 && slot->refCount == 0 && slot->flags == 0) {
                freeFile(otherBank, j);
                if (mMemUsed + size <= 9999999)
                    return;
            }
        }
    }
    do {
        int best = -1;
        unsigned int stamp = 0;
        for (int k = 0; k < 256; k++) {
            tBoltFile* slot = &mFiles[bank][k];
            if (slot->dataPtr != 0 && slot->flags == 0 && slot->refCount == 0) {
                if (best < 0 || slot->stamp < stamp) {
                    stamp = slot->stamp;
                    best = k;
                }
            }
        }
        if (best < 0)
            return;
        freeFile(bank, best);
    } while (mMemUsed + size > 9999999);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_evictBytes);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_freeFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Evicts at least a given number of bytes of idle cached files from the FE file cache.
 *
 * Used after an allocation fails, regardless of the budget. Frees idle slots (data present, use count 0, state 0) of
 * the other banks in bank order until the freed sizes reach @p size, then this bank's idle slots oldest stamp first
 * until they do or none remain (cFEBoltFileManager_freeFile).
 *
 * @param bank  the bank being loaded into; its own files go last
 * @param size  bytes to free
 */
void cFEBoltFileManager::evictBytes(int bank, int size)
{
    int otherBank;
    int j;
    for (otherBank = 0; otherBank < 10; otherBank++) {
        if (otherBank == bank)
            continue;
        for (j = 0; j < 256; j++) {
            tBoltFile* slot = &mFiles[otherBank][j];
            if (slot->dataPtr != 0 && slot->refCount == 0 && slot->flags == 0) {
                size -= slot->fileSize;
                freeFile(otherBank, j);
                if (size <= 0)
                    return;
            }
        }
    }
    do {
        int best = -1;
        unsigned int stamp = 0;
        for (int k = 0; k < 256; k++) {
            tBoltFile* slot = &mFiles[bank][k];
            if (slot->dataPtr != 0 && slot->flags == 0 && slot->refCount == 0) {
                if (best < 0 || slot->stamp < stamp) {
                    stamp = slot->stamp;
                    best = k;
                }
            }
        }
        if (best < 0)
            return;
        size -= mFiles[bank][best].fileSize;
        freeFile(bank, best);
    } while (size > 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_loadFile);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int BXFILE_sizez(const char* name);
extern "C" int ASYNCFILE_loadat(const char* name, void* data, int size);
extern "C" int FILE_loadatz(const char* name, void* data, int size);
extern "C" void cFEBoltFileManager_makeRoom(cFEBoltFileManager* self, int bank, int size);
extern "C" void cFEBoltFileManager_evictBytes(cFEBoltFileManager* self, int bank, int size);
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern char str_Bolt_FileData[];
extern char cMemMan_m_mutex[];


/**
 * Starts loading a slot's file into the FE file cache.
 *
 * Reads the file size (BXFILE_sizez), makes room (cFEBoltFileManager_makeRoom) and allocates the data (operator new, tag "Bolt
 * FileData", flags 0x60000000), and while that fails it locks and unlocks cMemMan_m_mutex, evicts that many bytes
 * (cFEBoltFileManager_evictBytes) and retries; used grows by the size. With pending set the load is asynchronous: state 2, handle from
 * ASYNCFILE_loadat (an async load request) and nPending incremented. Otherwise FILE_loadatz() loads it at once and the
 * state becomes 1 (loaded).
 *
 * Quirk: a synchronous load is not unpacked; only completed asynchronous loads go through cFEBoltFileManager_unpackFile.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 */
void cFEBoltFileManager::loadFile(int bank, int i)
{
    tBoltFile* slot = &mFiles[bank][i];
    int size = BXFILE_sizez(slot->fileName);
    makeRoom(bank, size);
    slot->dataPtr = operator_new_tag(size, str_Bolt_FileData, 0x60000000, 0);
    while (slot->dataPtr == 0) {
        MUTEX_lock(cMemMan_m_mutex);
        MUTEX_unlock(cMemMan_m_mutex);
        evictBytes(bank, size);
        slot->dataPtr = operator_new_tag(size, str_Bolt_FileData, 0x60000000, 0);
    }
    mMemUsed += size;
    if (slot->async != 0) {
        slot->fileSize = size;
        slot->flags = 2;
        slot->asyncFileHandle = ASYNCFILE_loadat(slot->fileName, slot->dataPtr, size);
        mNumLoadThreads += 1;
    } else {
        slot->fileSize = size;
        FILE_loadatz(slot->fileName, slot->dataPtr, size);
        slot->flags = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_requestFile);
#ifdef SKIP_ASM
extern "C" void cFEBoltFileManager_freeFile(cFEBoltFileManager* self, int bank, int i);
extern "C" void cFEBoltFileManager_loadFile(cFEBoltFileManager* self, int bank, int i);
extern "C" int cFEBoltFileManager_findFreeSlot(cFEBoltFileManager* self, int bank);
extern "C" int cFEBoltFileManager_findFile(cFEBoltFileManager* self, int bank, const char* name);
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Requests a file from the FE file cache and returns its slot, adding one use.
 *
 * When the name is already cached (cFEBoltFileManager_findFile): the stamp becomes 0 with @p deferred or the bank's next stamp otherwise;
 * with @p async an idle slot becomes loaded (kBoltFileFlag_Done); without it, pending is cleared, an in-flight load is
 * waited on (ASYNCFILE_release), a queued one is loaded at once (cFEBoltFileManager_loadFile), and the slot is marked loaded with
 * handle -1. Otherwise it reuses a slot (cFEBoltFileManager_findFreeSlot, emptied with cFEBoltFileManager_freeFile), stamps it the same way, copies the
 * name and stores @p fileType as the type and @p async as pending; it loads at once (cFEBoltFileManager_loadFile) unless @p deferred
 * is set or an asynchronous request meets loads already in flight, in which case it is queued (kBoltFileFlag_Queued).
 * Either way the use count (timer) goes up by 1; cFEBoltFileManager_releaseFile gives the use back.
 *
 * Likely bug: the cached path that waits on an in-flight load neither unpacks it (cFEBoltFileManager_unpackFile) nor decrements
 * nPending, unlike cFEBoltFileManager_releaseFile. Likely bug: a -1 from cFEBoltFileManager_findFreeSlot (no idle slot) is used as a slot index.
 *
 * @param cache  the FE file cache
 * @param bank  the bank (0-9)
 * @param namep  the file name
 * @param fileType  the file type stored in the slot (the rider manager uses 8 and 11)
 * @param async  nonzero to load asynchronously
 * @param deferred  nonzero to defer the load: stamp 0 and queued, to be ordered later by cFEBoltFileManager_startDeferredModelPartLoads or cFEBoltFileManager_startDeferredPartTextureLoads
 * @return the slot index
 */
extern "C" int cFEBoltFileManager_requestFile(void* cache, int bank, void* namep, int fileType, int async, int deferred)
{
    cFEBoltFileManager* self = (cFEBoltFileManager*)cache;
    const char* name = (const char*)namep;
    int i = self->findFile(bank, name);
    if (i >= 0) {
        tBoltFile* slot = &self->mFiles[bank][i];
        if (deferred)
            slot->stamp = 0;
        else
            slot->stamp = self->unk_B4008[bank]++;
        if (async) {
            if (slot->flags == 0)
                slot->flags = 1;
        } else {
            slot->async = 0;
            if (slot->flags == kBoltFileFlag_Loading) {
                int status;
                ASYNCFILE_release(slot->asyncFileHandle, 0, &status);
            } else if (slot->flags == kBoltFileFlag_Queued) {
                self->loadFile(bank, i);
            }
            slot->flags = 1;
            slot->asyncFileHandle = -1;
        }
        slot->refCount++;
    } else {
    i = self->findFreeSlot(bank);
    tBoltFile* slot = &self->mFiles[bank][i];
    self->freeFile(bank, i);
    if (deferred)
        slot->stamp = 0;
    else
        slot->stamp = self->unk_B4008[bank]++;
    strcpy(slot->fileName, name);
    slot->refCount++;
    slot->fileType = fileType;
    slot->async = async;
    if (deferred == 0 && (async == 0 || self->mNumLoadThreads <= 0))
        self->loadFile(bank, i);
    else
        slot->flags = 3;
    }
    return i;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_releaseFile);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" void cFEBoltFileManager_unpackFile(cFEBoltFileManager* self, int bank, tBoltFile* slot);
extern "C" void cFEBoltFileManager_freeFile(cFEBoltFileManager* self, int bank, int i);

/**
 * Drops one use of a slot in the FE file cache, keeping its data cached for reuse.
 *
 * Decrements the use count (timer) and returns while it is still positive. An asynchronous load still in flight
 * (ASYNCFILE_getstatus() not kASYNCFILE_Done) is left to finish; a finished one is released, unpacked (cFEBoltFileManager_unpackFile)
 * and counted out of nPending. A slot without data is then freed (cFEBoltFileManager_freeFile); one with data becomes idle (state 0),
 * so a later request can reuse it until it is evicted.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index (from cFEBoltFileManager_requestFile)
 */
void cFEBoltFileManager::releaseFile(int bank, int i)
{
    int status;
    tBoltFile* slot = &mFiles[bank][i];
    if (--slot->refCount > 0) {
        return;
    }
    if (slot->flags == kBoltFileFlag_Loading) {
        if (ASYNCFILE_getstatus(slot->asyncFileHandle) != kASYNCFILE_Done) {
            return;
        }
        ASYNCFILE_release(slot->asyncFileHandle, 0, &status);
        unpackFile(bank, slot);
        slot->async = 0;
        slot->flags = 0;
        mNumLoadThreads -= 1;
    }
    if (slot->dataPtr == 0) {
        freeFile(bank, i);
    } else {
        slot->flags = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", compareDeferredModelPartFiles);
#ifdef SKIP_ASM
extern cFEBoltFileManager* cFEBoltFileManager_instance;
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Compares two FE file cache slots to sort the deferred kBoltFileType_ModelPart (8) loads by name (qsort comparator).
 *
 * Each argument is a packed key (bank << 16 | slot) into the cache cFEBoltFileManager_instance; both use the first key's bank. A slot
 * counts when it is queued (kBoltFileFlag_Queued), has type kBoltFileType_ModelPart and stamp 0. Two counted slots
 * compare by name with strcasecmp; a counted slot sorts before any other; two others compare equal.
 *
 * @param firstKey  first packed key
 * @param secondKey  second packed key
 * @return negative, 0 or positive in qsort fashion
 */
extern "C" int compareDeferredModelPartFiles(const int* firstKey, const int* secondKey)
{
    int ka = *firstKey;
    tBoltFile* bank = cFEBoltFileManager_instance->mFiles[ka >> 16];
    tBoltFile* sa = &bank[ka & 0xFFFF];
    tBoltFile* sb = &bank[*secondKey & 0xFFFF];
    if (sa->flags == kBoltFileFlag_Queued && sa->fileType == kBoltFileType_ModelPart && sa->stamp == 0) {
        if (sb->flags == kBoltFileFlag_Queued && sb->fileType == kBoltFileType_ModelPart && sb->stamp == 0)
            return strcasecmp(sa->fileName, sb->fileName);
        return -1;
    }
    if (sb->flags == kBoltFileFlag_Queued && sb->fileType == kBoltFileType_ModelPart && sb->stamp == 0) {
        if (sa->flags == kBoltFileFlag_Queued && sa->fileType == kBoltFileType_ModelPart && sa->stamp == 0)
            return strcasecmp(sa->fileName, sb->fileName);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", compareDeferredPartTextureFiles);
#ifdef SKIP_ASM
extern cFEBoltFileManager* cFEBoltFileManager_instance;
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Compares two FE file cache slots to sort the deferred kBoltFileType_PartTexture (11) loads by name (qsort
 * comparator).
 *
 * Same as compareDeferredModelPartFiles for kBoltFileType_PartTexture: queued, type 11, stamp 0 slots sort first, by name with
 * strcasecmp; two others compare equal.
 *
 * @param firstKey  first packed key
 * @param secondKey  second packed key
 * @return negative, 0 or positive in qsort fashion
 */
extern "C" int compareDeferredPartTextureFiles(const int* firstKey, const int* secondKey)
{
    int ka = *firstKey;
    tBoltFile* bank = cFEBoltFileManager_instance->mFiles[ka >> 16];
    tBoltFile* sa = &bank[ka & 0xFFFF];
    tBoltFile* sb = &bank[*secondKey & 0xFFFF];
    if (sa->flags == kBoltFileFlag_Queued && sa->fileType == kBoltFileType_PartTexture && sa->stamp == 0) {
        if (sb->flags == kBoltFileFlag_Queued && sb->fileType == kBoltFileType_PartTexture && sb->stamp == 0)
            return strcasecmp(sa->fileName, sb->fileName);
        return -1;
    }
    if (sb->flags == kBoltFileFlag_Queued && sb->fileType == kBoltFileType_PartTexture && sb->stamp == 0) {
        if (sa->flags == kBoltFileFlag_Queued && sa->fileType == kBoltFileType_PartTexture && sa->stamp == 0)
            return strcasecmp(sa->fileName, sb->fileName);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_startDeferredModelPartLoads);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One file slot of the FE file cache (0x120 bytes; the cache sits at game + 0x7C -> + 0x1A70, cFEBoltFileManager_instance points at
// it). state: 0 idle, 1 loaded, 2 async load in flight, 3 queued.
struct sFileSlot {
    int state;              // 0x0
    char name[0x100];       // 0x4
    void* data;             // 0x104
    int handle;             // 0x108: async file handle
    int pending;            // 0x10C: load asynchronously
    int type;               // 0x110
    int timer;              // 0x114: use count
    int size;               // 0x118
    unsigned int stamp;     // 0x11C: least-recently-used stamp
};

// 256 slots per bank (0x12000 bytes).
struct sFileBank {
    sFileSlot slots[256];
};

// The FE file cache: ten banks, the loaded byte count, the number of loads in flight and per-bank stamp counters.
struct sFileCache {
    sFileBank banks[10];    // 0x0
    int used;               // 0xB4000
    int nPending;           // 0xB4004
    int counts[10];         // 0xB4008
};

extern "C" void cFEBoltFileManager_loadFile_DF20(sFileCache* self, int bank, int i) __asm__("cFEBoltFileManager_loadFile");

// (prototype from their definition at line 2766)
extern "C" int compareDeferredModelPartFiles_DF20(const int* a, const int* b) __asm__("compareDeferredModelPartFiles");

extern "C" void qsort(void* base, int n, int size, int (*cmp)(const int*, const int*));

/**
 * Stamps a bank's deferred kBoltFileType_ModelPart (8) loads in name order and starts the first one.
 *
 * Sorts the bank's 256 packed keys with qsort (qsort) and compareDeferredModelPartFiles, then walks them while the slot has
 * type 8 and stamp 0: each gets the bank's next stamp (counts) and, when no loads are in flight and it is queued,
 * starts loading (cFEBoltFileManager_loadFile). The rider manager (cFERider_loadModels) calls it after requesting deferred files.
 *
 * @param bank  the bank (0-9)
 */
extern "C" void cFEBoltFileManager_startDeferredModelPartLoads(sFileCache* self, int bank)
{
    int keys[256];
    for (int i = 0; i < 256; i++)
        keys[i] = (bank << 16) | i;
    qsort(keys, 256, 4, compareDeferredModelPartFiles_DF20);
    for (int i = 0; i < 256; i++) {
        unsigned short idx = keys[i] & 0xFFFF;
        sFileSlot* slot = &self->banks[bank].slots[idx];
        if (slot->stamp != 0 || slot->type != kBoltFileType_ModelPart)
            break;
        slot->stamp = self->counts[bank]++;
        if (self->nPending <= 0 && slot->state == kBoltFileFlag_Queued)
            cFEBoltFileManager_loadFile_DF20(self, bank, idx);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_startDeferredPartTextureLoads);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One file slot of the FE file cache (0x120 bytes; the cache sits at game + 0x7C -> + 0x1A70, cFEBoltFileManager_instance points at
// it). state: 0 idle, 1 loaded, 2 async load in flight, 3 queued.
// 256 slots per bank (0x12000 bytes).
// The FE file cache: ten banks, the loaded byte count, the number of loads in flight and per-bank stamp counters.
extern "C" void cFEBoltFileManager_loadFile_E070(sFileCache* self, int bank, int i) __asm__("cFEBoltFileManager_loadFile");

// (prototype from their definition at line 2789)
extern "C" int compareDeferredPartTextureFiles_E070(const int* a, const int* b) __asm__("compareDeferredPartTextureFiles");

extern "C" void qsort(void* base, int n, int size, int (*cmp)(const int*, const int*));

/**
 * Stamps a bank's deferred kBoltFileType_PartTexture (11) loads in name order and starts the first one.
 *
 * Same as cFEBoltFileManager_startDeferredModelPartLoads with compareDeferredPartTextureFiles and kBoltFileType_PartTexture. The rider manager (cFERider_loadTextures)
 * calls it after requesting deferred files.
 *
 * @param bank  the bank (0-9)
 */
extern "C" void cFEBoltFileManager_startDeferredPartTextureLoads(sFileCache* self, int bank)
{
    int keys[256];
    for (int i = 0; i < 256; i++)
        keys[i] = (bank << 16) | i;
    qsort(keys, 256, 4, compareDeferredPartTextureFiles_E070);
    for (int i = 0; i < 256; i++) {
        unsigned short idx = keys[i] & 0xFFFF;
        sFileSlot* slot = &self->banks[bank].slots[idx];
        if (slot->stamp != 0 || slot->type != kBoltFileType_PartTexture)
            break;
        slot->stamp = self->counts[bank]++;
        if (self->nPending <= 0 && slot->state == kBoltFileFlag_Queued)
            cFEBoltFileManager_loadFile_E070(self, bank, idx);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_isBankIdle);
#ifdef SKIP_ASM
/**
 * Returns whether a bank of the FE file cache has no load in flight or queued.
 *
 * @param bank  the bank (0-9)
 * @return false when any slot is loading or queued (state kBoltFileFlag_Loading / kBoltFileFlag_Queued, or a
 *         negative state), else true
 */
bool cFEBoltFileManager::isBankIdle(int bank)
{
    for (int i = 0; i < 0x100; i++) {
        if ((unsigned int)mFiles[bank][i].flags >= 2) return false;
    }
    return true;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_isFileLoading);
#ifdef SKIP_ASM
/**
 * Returns whether a slot of the FE file cache is loading (state kBoltFileFlag_Loading).
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return true while the load is in flight, else false
 */
bool cFEBoltFileManager::isFileLoading(int bank, int i)
{
    return mFiles[bank][i].flags == kBoltFileFlag_Loading;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_isFileLoaded);
#ifdef SKIP_ASM
/**
 * Returns whether a slot of the FE file cache is loaded (state kBoltFileFlag_Done).
 *
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return true when loaded, else false
 */
bool cFEBoltFileManager::isFileLoaded(int bank, int slotIndex)
{
    return mFiles[bank][slotIndex].flags == kBoltFileFlag_Done;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_getFileName);
#ifdef SKIP_ASM
/**
 * Returns a slot's file name in the FE file cache.
 *
 * Returns 0 only for state kBoltFileFlag_Invalid (-1), which the cache's own functions never set.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the name buffer, or 0
 */
char* cFEBoltFileManager::getFileName(int bank, int i)
{
    if (mFiles[bank][i].flags == kBoltFileFlag_Invalid) return 0;
    return mFiles[bank][i].fileName;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_getFileDataValue);
#ifdef SKIP_ASM
/**
 * Returns a slot's file data in the FE file cache once it is loaded.
 *
 * The view calls the data pointer at +0x104 `value`; cFEBoltFileManager_getFileData is the same lookup typed as a pointer.
 *
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return the data pointer as an int when the state is kBoltFileFlag_Done, else 0
 */
void* cFEBoltFileManager::getFileDataValue(int bank, int slotIndex)
{
    if (mFiles[bank][slotIndex].flags != kBoltFileFlag_Done) {
        return 0;
    }
    return mFiles[bank][slotIndex].dataPtr;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_getFileData);
#ifdef SKIP_ASM
/**
 * Returns a slot's file data in the FE file cache once it is loaded (state kBoltFileFlag_Done), else 0.
 *
 * Same as cFEBoltFileManager_getFileDataValue, typed as a pointer.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the data, or 0
 */
void* cFEBoltFileManager::getFileData(int bank, int i)
{
    if (mFiles[bank][i].flags != kBoltFileFlag_Done) return 0;
    return mFiles[bank][i].dataPtr;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_detachFileData);
#ifdef SKIP_ASM
/**
 * Detaches a slot's file data from the FE file cache and returns it.
 *
 * Clears the slot's data and size and subtracts the size from mMemUsed; state, name and use count stay. The
 * reward database (cBERewardDB_update) takes the block this way and then drops its use with cFEBoltFileManager_releaseFile, which frees the
 * now empty slot.
 *
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return the data pointer as an int (the caller now owns it)
 */
void* cFEBoltFileManager::detachFileData(int bank, int slotIndex)
{
    void* data = mFiles[bank][slotIndex].dataPtr;
    mFiles[bank][slotIndex].dataPtr = 0;
    mMemUsed -= mFiles[bank][slotIndex].fileSize;
    mFiles[bank][slotIndex].fileSize = 0;
    return data;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_getFileType);
#ifdef SKIP_ASM
/**
 * Returns a slot's file type (+0x110, as passed to cFEBoltFileManager_requestFile) in the FE file cache.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the type
 */
int cFEBoltFileManager::getFileType(int bank, int i)
{
    return mFiles[bank][i].fileType;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEBoltFileManager_getUseCount);
#ifdef SKIP_ASM
/**
 * Returns a slot's use count (+0x114, `timer` in the views) in the FE file cache.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the use count
 */
int cFEBoltFileManager::getUseCount(int bank, int i)
{
    return mFiles[bank][i].refCount;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider_cFERider);
#ifdef SKIP_ASM
// FAKEMATCH: stand-in local `int none = -1;` (declared after the -1 header stores, before `e`) used for the loop's `e->id` store instead
// of a literal -1 that loop.c hoists after `e`; plain form 99.56%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sQuadPairCE { int x[8]; } __attribute__((aligned(16)));
extern sQuadPairCE tPose_Identity;

struct sSlotEntryCE {
    int id;
    unsigned char flag;
    char pad[7];
};


/**
 * Constructs a front-end rider (0xCE0 bytes; the rider manager holds two).
 *
 * Sets mCharID, mProfileIndex, mNewCharID, mSafeRenderCount and the 256 mTextureRefs ids to -1 (their flags to 0);
 * mDrawLayer to 0xFFFFFFFF; mPose and mBoardPose to tPose_Identity; and clears mpAnimModel to mNumTextureRefs,
 * mCustomized, mSelected, mNewLoadAll, the FX pointers, mBoltPoints, the load-stage flags (mCompiledModel to
 * mDoneUpdatingModelParts), mSetVisible, mVisible and mAnimateBoard.
 *
 * @return @p self
 */
extern "C" void* cFERider_cFERider(cFERider* self)
{
    int i;
    sSlotEntryCE* textureEntry;
    self->mpAttachedEmitterFX = 0;
    self->mpBoltOnFX = 0;
    self->mCharID = -1;
    self->mProfileIndex = -1;
    int none = -1;
    textureEntry = ((sSlotEntryCE*)&self->mTextureRefs[0]);
    (*(int*)&self->mpAnimModel) = 0;
    (*(int*)&self->mpRiderAnim) = 0;
    self->mLoadAll = 0;
    self->mRiderIsPosed = 0;
    self->mNumTextureRefs = 0;
    for (i = 0; i < 256; i++) {
        textureEntry->flag = 0;
        textureEntry->id = none;
        textureEntry++;
    }
    self->mCustomized = 0;
    *(unsigned int*)&self->mDrawLayer = 0xFFFFFFFF;
    self->mSelected = 0;
    self->mNewCharID = -1;
    self->mNewLoadAll = 0;
    self->mAnimateBoard = 0;
    *(sQuadPairCE*)&self->mPose = tPose_Identity;
    *(sQuadPairCE*)&self->mBoardPose = tPose_Identity;
    self->mSafeRenderCount = -1;
    self->mBoltPoints = 0;
    self->mSetVisible = 0;
    self->mVisible = 0;
    self->mCompiledModel = 0;
    self->mDoneLoadingTextures = 0;
    self->mDoneLoadingModels = 0;
    self->mDoneUpdatingModelParts = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider__cFERider);
#ifdef SKIP_ASM

extern "C" void cFERider_purge(void* self);
void operator_delete(int* p);

/**
 * Destroys a front-end rider: unloads it and releases its textures.
 *
 * Calls cFERider_purge (deletes the animation model at +0x8 and the objects at +0xC, +0xC70 and +0xC74), then for each
 * of the 256 texture entries at +0x1C with an id >= 0 calls GrMan's virtual slot 50 with the id; every entry's id
 * becomes -1 and its flag 0. Frees the rider when bit 0 of @p flags is set.
 *
 * @param flags  deleting-destructor flags; bit 0 frees the object
 */
extern "C" void cFERider__cFERider(cFERider* self, int flags)
{
    tTextureRef* slot = (tTextureRef*)&self->mTextureRefs;
    cFERider_purge(self);
    int i;
    int none = -1;
    for (i = 0xFF; i >= 0; i--, slot++) {
        if (slot->TextureID >= 0) {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            ps2GraphicsMan->RemoveTexture(slot->TextureID);
        }
        slot->TextureID = none;
        slot->Name[0] = 0;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider_setVisible);
#ifdef SKIP_ASM
/**
 * Shows or hides a front-end rider, deferring the show until its model is loaded.
 *
 * With @p enable set: when the load stages +0xCC0, +0xCB8 and +0xCB4 are all done and no reload is pending (+0xCD4 <
 * 0), sets visible (+0xCC8); otherwise sets +0xCC4 so the update shows it once loaded. With @p enable 0 both flags are
 * cleared.
 *
 * @param enable  nonzero to show, 0 to hide
 */
void cFERider::setVisible(int enable)
{
    if (enable != 0) {
        if (mDoneUpdatingModelParts != 0 && mDoneLoadingTextures != 0 &&
            mCompiledModel != 0 && mSafeRenderCount < 0) {
            mVisible = 1;
        } else {
            mSetVisible = 1;
        }
    } else {
        mVisible = 0;
        mSetVisible = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider_setCharID);
#ifdef SKIP_ASM


struct sRider_E588 {
    unsigned int item;  // 0x0
    char pad4[0xC];
    int f10;            // 0x10
    char pad14[0xC24 - 0x14];
    int reqItem;        // 0xC24
    int reqFlag;        // 0xC28
    char padC2C[0xCB4 - 0xC2C];
    int fCB4;           // 0xCB4
    int fCB8;           // 0xCB8
    char padCBC[0xCC4 - 0xCBC];
    int fCC4;           // 0xCC4
    int fCC8;           // 0xCC8
    char padCCC[0xCD4 - 0xCCC];
    int timer;          // 0xCD4
};

/**
 * Requests a character for a front-end rider, reloading it a few frames later when it changes.
 *
 * Stores @p item as the requested character (+0xC24) and @p flag at +0xC28. When the rider already has that character
 * (+0x0) and either +0x10 is set or @p flag is 0: if a valid character (below 10) is loaded (+0xCB8 and +0xCB4 set),
 * cancels any pending reload (+0xCD4 = -1) and makes the rider visible when it was visible or waiting to be (+0xCC8 |=
 * +0xCC4, then +0xCC4 = 0); without a loaded character nothing changes. Otherwise, unless a reload is already pending,
 * arms one for GrMan's frame counter (virtual slot 114) + 4, moving visible into +0xCC4 and hiding the rider; the
 * rider's update then calls cFERider_init with the requested values.
 *
 * @param item  the character id
 * @param flag  stored at +0xC28 and passed to cFERider_init as its last argument
 */
void cFERider::setCharID(unsigned int item, int flag)
{
    mNewCharID = item;
    mNewLoadAll = flag;
    // The three-way flag test (equivalent to f10 != 0 || flag == 0) is kept as written for codegen.
    if ((unsigned int)mCharID == item
        && ((mLoadAll != 0 && flag != 0) || (mLoadAll == 0 && flag == 0) || !(mLoadAll == 0 || flag != 0))) {
        int ok = 0;
        if ((unsigned int)mCharID < 10 && mDoneLoadingTextures != 0) {
            ok = mCompiledModel != 0;
        }
        if (ok) {
            mSafeRenderCount = -1;
            int* visibleField = &mVisible;
            int showNow = 0;
            if (*visibleField != 0 || mSetVisible != 0) {
                showNow = 1;
            }
            *visibleField = showNow;
            mSetVisible = 0;
        }
        return;
    }
    if (mSafeRenderCount < 0) {
        cPS2GraphicsMan* w = GrMan;
        mSafeRenderCount = w->getCurrentFrameCount() + 4;
        mSetVisible = mVisible;
        mVisible = 0;
    }
}
#endif

