#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cFEStateCharEquip);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, signed char a1);
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int cBELibrary_getRiderIndex(int index);
int GetHashValue32(char* str);
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern "C" int cUITextureBank_getElementByHashname(void* list, int hash);
extern "C" void cFERiderManager_setHumanCharacter(void* mgr, int idx, int ch, int a3);
extern "C" int cBEEconInterface_getMoney(void* iface, int idx, int ch);
extern "C" void func_0014AEA8(void* self, int a1, int a2);
extern "C" int func_0014B478(void* self, int a1, int a2);
extern "C" char* func_0014B700(void* iface, int rider);
extern "C" int cBENewPlayerInterface_getLastHub(void* iface, int a1);
extern "C" void func_0019B180(void* self, int id);
extern "C" int func_0019DA10(void* self, int bank, void* name, int a3, int a4, int a5);
extern const int D_004A11D0_c __asm__("D_004A11D0");
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

static inline int Find_8DB8(char* mgr, int hash)
{
    return cUITextureBank_getElementByHashname(mgr + 0x58, hash);
}

/**
 * Constructs the equip-gear state for a user in store (buy) or inventory (equip) mode.
 *
 * Sets state id 0xE, the two preview positions (0xAC0, 0xAD0), the user's controller bits, the character
 * (0xBC), the mode (0xC8: 1 in the store), money (0xC0) and equip points (0xC4, func_0014B478); clears the
 * widget pointers and the category stack (root -1), finds the dollar and checkbox texture elements and sets
 * the user's FE rider to the character (cFERiderManager_setHumanCharacter).
 *
 * With a store or a mode switch, 0xCC (On Tour mStoreNum) is the last hub's peak (2 for hubs 19-20, 3 for 21,
 * else 1), otherwise -1. Builds the root item list (func_0019B180) and loads the character's icon file into
 * the FE file cache (slot kept at 0xA64).
 *
 * @param mem  the state being constructed (this)
 * @param owner  the UI engine (On Tour pEngine)
 * @param team  the user number (On Tour userNum)
 * @param inStore  nonzero to open in store mode (buying), zero for the inventory (equipping)
 * @param canSwitchMode  nonzero when the option button may switch between the two modes (kept at 0xAE4)
 * @return the constructed state
 */
extern "C" void* cFEStateCharEquip_cFEStateCharEquip(void* mem, void* owner, signed char team, int inStore, int canSwitchMode)
{
    sCharEquip_8DB8* self = (sCharEquip_8DB8*)mem;
    func_0039E2A0(self);
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
    MEM_fill(self->bufD0, D_004A11D0_c, 80);
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
        char* mgr = *(char**)&self->mgr;
        self->fA90 = Find_8DB8(mgr, GetHashValue32(str_dollargreen));
    }
    {
        char* mgr = *(char**)&self->mgr;
        self->fA94 = Find_8DB8(mgr, GetHashValue32(str_dollarred));
    }
    {
        char* mgr = *(char**)&self->mgr;
        self->fA98 = Find_8DB8(mgr, GetHashValue32(str_checkbox));
    }
    {
        char* mgr = *(char**)&self->mgr;
        self->fA9C = Find_8DB8(mgr, GetHashValue32(str_empty_box));
    }
    self->fAA0 = 0;
    cFERiderManager_setHumanCharacter((char*)SSXApp->RealFE + 0xB0, self->team, self->slot, 1);
    self->fC0 = cBEEconInterface_getMoney(cBE_getInterface(cBE_getBE(), 11), self->team, self->slot);
    void* shop = cBE_getInterface(cBE_getBE(), 9);
    func_0014AEA8(shop, self->team, self->slot);
    self->fC4 = func_0014B478(shop, self->team, self->slot);
    self->fAE4 = canSwitchMode;
    if (inStore != 0 || canSwitchMode != 0) {
        switch (cBENewPlayerInterface_getLastHub(player, cBELibrary_getRiderIndex(self->team))) {
        case 17:
            self->fCC = 1;
            break;
        case 18:
            self->fCC = 1;
            break;
        case 19:
            self->fCC = 2;
            break;
        case 20:
            self->fCC = 2;
            break;
        case 21:
            self->fCC = 3;
            break;
        default:
            self->fCC = 1;
            break;
        }
    } else {
        self->fCC = -1;
    }
    func_0019B180(self, D_004A11D0_c);
    char* name = func_0014B700(shop, self->slot);
    self->fA64 = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, self->slot, name, 9, 1, 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip__cFEStateCharEquip);
#ifdef SKIP_ASM
struct sEquipTable_DC20;
extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i);
extern "C" void cFEStateCharEquip_cancelLargePreviewLoad(void* self);
extern "C" void func_0039E390(void* self, int flags);
extern void* cFEStateCharEquip_vtbl[];
extern void* D_0046D0D0[];

struct sVEntryK199148 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void releaseK199148(int id)
{
    if (id >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryK199148* vt = (sVEntryK199148*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, id);
    }
}

/**
 * Destroys the equip-gear state: frees its icon file and preview textures, then runs the base destructor.
 *
 * Releases the icon file (0xA64) from the FE file cache and the texture ids at 0xA68, 0xA70 and 0xA74 through
 * the graphics manager (vtable slot 50), cancels a pending large-preview load (cFEStateCharEquip_cancelLargePreviewLoad), restores the
 * base vtable D_0046D0D0 and calls the cUIState destructor func_0039E390().
 *
 * @param flags  g++ destructor flags, passed on to func_0039E390(); bit 0 frees the object
 */
extern "C" void cFEStateCharEquip__cFEStateCharEquip(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cFEStateCharEquip_vtbl;
    if (*(int*)((char*)self + 0xA64) >= 0) {
        func_0019DC20((sEquipTable_DC20*)((char*)SSXApp->RealFE + 0x1A70), *(int*)((char*)self + 0xBC),
                      *(int*)((char*)self + 0xA64));
        *(int*)((char*)self + 0xA64) = -1;
    }
    releaseK199148(*(int*)((char*)self + 0xA68));
    *(int*)((char*)self + 0xA68) = -1;
    cFEStateCharEquip_cancelLargePreviewLoad(self);
    releaseK199148(*(int*)((char*)self + 0xA70));
    releaseK199148(*(int*)((char*)self + 0xA74));
    *(int*)((char*)self + 0xA70) = -1;
    *(int*)((char*)self + 0xA74) = -1;
    *(void***)((char*)self + 0x8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cRiderAnimBase_reset(void* self);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cFEStateCharEquip_update3DPositions(void* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern char str_12equ_char[];

/**
 * Opens the equip-gear screen ("12equ_char") and poses the user's FE rider for it.
 *
 * Puts the rider's draw layer (+0xC2C) at 0x11 and clears its +0xCCC; when its character id is below 10 and
 * its model is ready (+0xCB4, +0xCB8), restarts its animation with animation 0x1B2. Resets the zoom (0xAA4)
 * and turn (0xAAC) to 0 and the pan (0xAA8) to -1, places the camera and models (cFEStateCharEquip_update3DPositions) and switches
 * the front-end sound-bank mode to 2.
 */
extern "C" void cFEStateCharEquip_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_12equ_char), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    char* feRider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44));
    *(int*)(feRider + 0xC2C) = 17;
    *(int*)(feRider + 0xCCC) = 0;
    int ok = *(unsigned int*)feRider < 10 && *(int*)(feRider + 0xCB8) != 0 && *(int*)(feRider + 0xCB4) != 0;
    if (ok) {
        void* anim = *(void**)(feRider + 0xC);
        if (anim != 0) {
            cRiderAnimBase_reset(anim);
            cRiderAnimBase_play(anim, 434, 0, -1.0f);
        }
    }
    *(int*)((char*)self + 0xAA4) = 0;
    *(int*)((char*)self + 0xAAC) = 0;
    *(float*)((char*)self + 0xAA8) = -1.0f;
    cFEStateCharEquip_update3DPositions(self);
    func_0028F140(cAudioInterface_bxAudio(), 2);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
void* cUIState_onDestroyScreen(void* self);

/**
 * Takes the user's FE rider off the draw layers (+0xC2C = -1), then destroys the screens like the base state.
 *
 * The rider is left alone when SSXApp->RealFE is null; cUIState_onDestroyScreen() does the rest.
 */
extern "C" void cFEStateCharEquip_onDestroyScreen(void* self)
{
    char* mgr = (char*)SSXApp->RealFE;
    if (mgr != 0) {
        char* feRider = (char*)cFERiderManager_getHuman1(mgr + 0xB0, *(signed char*)((char*)self + 0x44));
        *(unsigned int*)(feRider + 0xC2C) = 0xFFFFFFFF;
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cFEStateCharEquip_initStorePreview(void* self);
extern "C" void cFEStateCharEquip_initInventoryPreview(void* self);
extern "C" void cFEStateCharEquip_updatePolyBar(void* self);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(void* self, int a1);
extern "C" void func_0019B618(void* self, int a1, int a2);
extern "C" void cFEState_onGainFocus(void* self, int a1);

/**
 * Gives the equip-gear state focus: sets up its mode's widgets, the points bars and the item rows.
 *
 * Runs cFEStateCharEquip_initStorePreview() in store mode (0xC8 set; probably On Tour initStorePreview) or cFEStateCharEquip_initInventoryPreview()
 * otherwise (initInventoryPreview), then the points bar (cFEStateCharEquip_updatePolyBar) and the equip preview bar with no
 * extra cost (cFEStateCharEquip_updatePolyEquipPreviewBar), redraws the rows with a scroll reset (func_0019B618) and runs
 * cFEState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before, passed on to cFEState_onGainFocus()
 */
extern "C" void cFEStateCharEquip_onGainFocus(void* self, int pPrevState)
{
    bool off = !*(bool*)((char*)self + 0xC8);
    if (!off) {
        cFEStateCharEquip_initStorePreview(self);
    } else {
        cFEStateCharEquip_initInventoryPreview(self);
    }
    cFEStateCharEquip_updatePolyBar(self);
    cFEStateCharEquip_updatePolyEquipPreviewBar(self, 0);
    func_0019B618(self, 1, 1);
    cFEState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onLoseFocus);
#ifdef SKIP_ASM
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(void* rider, int a1);

/**
 * Clears two flags of the front-end rider record this equip screen works on when the screen loses focus.
 *
 * The record is entry @p self+0x44 (a signed char rider index) of the 0xCE0-byte records at SSXApp->RealFE +
 * 0xB0 (cFERiderManager_getHuman1); cFERider_setVisible(record, 0) zeroes its words at +0xCC4 and +0xCC8.
 */
extern "C" void cFEStateCharEquip_onLoseFocus(void* self)
{
    cFERider_setVisible(cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

int GetHashValue32(char* str);

extern const int D_004A11D0_eate __asm__("D_004A11D0");

extern "C" void cFEStateCharEquip_updateHeading_eate(void* self, int id) __asm__("cFEStateCharEquip_updateHeading");

extern "C" void func_0019B7E0_eate(void* self, int idx) __asm__("func_0019B7E0");

extern "C" void func_0019BA60_eate(void* self, int idx) __asm__("func_0019BA60");

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
extern char D_004A1408[];
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
    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)w + 8);
    vt[9].fn((char*)w + vt[9].delta, 0);
}

struct sCharEquip_FE {
    char pad0[0x48];
    void* f48;
    void* f4C;
    void* f50;
    void* f54;
    void* f58;
    void* f5C;
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
    int fC8;
    char padCC[0x958 - 0xCC];
    int f958;
    char pad95C[0xAE4 - 0x95C];
    int fAE4;
};

static inline bool isC8_FE(sCharEquip_FE* s)
{
    return s->fC8 == 1;
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
extern "C" void cFEStateCharEquip_onWidgetCreate(sCharEquip_FE* self, void* w)
{
    int id = *(int*)((char*)w + 0x38);
    if (id == GetHashValue32(str_title_equip_character)) {
        self->f48 = w;
        if (isC8_FE(self)) {
            cUIText_setUnicodeStringByID_eate(self->f48, GetHashValue32(str_kT_CMNBuyGear));
        } else {
            cUIText_setUnicodeStringByID_eate(self->f48, GetHashValue32(str_kT_CMNEquipGear));
        }
    } else if (id == GetHashValue32(str_helptext)) {
        self->f4C = w;
        hide_FE(self->f4C);
    } else if (id == GetHashValue32(str_btext5)) {
        self->f50 = w;
        if (self->fAE4 == 0) {
            hide_FE(self->f50);
        } else {
            if (isC8_FE(self)) {
                cUIText_setUnicodeStringByID_eate(self->f50, GetHashValue32(str_kT_CMNEquipGear));
            } else {
                cUIText_setUnicodeStringByID_eate(self->f50, GetHashValue32(str_kT_CMNBuyGear));
            }
        }
    } else if (id == GetHashValue32(str_ps2sq) || id == GetHashValue32(str_xby) || id == GetHashValue32(str_ngcy)) {
        self->f54 = w;
        if (self->fAE4 == 0) {
            hide_FE(self->f54);
        }
    } else if (id == GetHashValue32(str_heading)) {
        self->f58 = w;
        cFEStateCharEquip_updateHeading_eate(self, D_004A11D0_eate);
    } else if (id == GetHashValue32(str_MnuGroups)) {
        self->f5C = w;
        cUIMenu_setScrollableMenu_FE(self->f5C, self->f958);
    } else if (id == GetHashValue32(str_buytotals)) {
        self->f60 = w;
        if (!isC8_FE(self)) {
            hide_FE(self->f60);
        }
    } else if (id == GetHashValue32(str_1right)) {
        self->f64 = w;
        cFEStateCharEquip_updateBankWidget_eate(self);
    } else if (id == GetHashValue32(str_2right)) {
        self->f68 = w;
        cUIText_setAsciiString_eate(self->f68, D_004A1408);
    } else if (id == GetHashValue32(str_bolt01)) {
        self->f84 = w;
    } else if (id == GetHashValue32(str_barandpoints)) {
        self->f6C = w;
        hide_FE(self->f6C);
    } else if (id == GetHashValue32(str_overbar)) {
        self->f70 = w;
        hide_FE(self->f70);
    } else if (id == GetHashValue32(str_overbar2)) {
        self->f74 = w;
        hide_FE(self->f74);
    } else if (id == GetHashValue32(str_equip_points)) {
        self->f78 = w;
    } else if (id == GetHashValue32(str_equip_btm_left)) {
        self->f80 = w;
        hide_FE(self->f80);
    } else if (id == GetHashValue32(str_points)) {
        self->f7C = w;
        cUIText_setAsciiString_eate(self->f7C, D_004A1408);
    } else if (id == GetHashValue32(str_largeshape)) {
        self->f88 = w;
        hide_FE(self->f88);
    } else {
        for (int i = 0; i < 6; i++) {
            if (id == GetHashValue32(D_00441218[i])) {
                self->f8C[i] = w;
                *(int*)((char*)w + 0x18) = i;
                if (i < self->f958) {
                    if (isC8_FE(self)) {
                        func_0019BA60_eate(self, i);
                    } else {
                        func_0019B7E0_eate(self, i);
                    }
                } else {
                    hide_FE(self->f8C[i]);
                }
                return;
            }
            if (id == GetHashValue32(D_00441230[i])) {
                self->fA4[i] = w;
                if (i < self->f958) {
                    if (isC8_FE(self)) {
                        func_0019BA60_eate(self, i);
                    } else {
                        func_0019B7E0_eate(self, i);
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
extern "C" void func_0019BE80(void* self);
extern "C" void cFEStateCharEquip_updateBankWidget(void* self);
struct sEquipDetail;
extern "C" int func_0019B458(sEquipDetail* self, int idx);
extern "C" void func_0019B618(void* self, int a1, int a2);

/**
 * Hides the equip-gear state behind popups and buys the selected item when the buy popup closes with a yes.
 *
 * Event 0x15 (a popup opened) clears this state's visible bit (mAttrState bit 3) unless the popup is the buy
 * popup (state id 7). Event 0x16 (popped) sets it again and, for the buy popup with its result (+0x6C) set,
 * buys the item (func_0019BE80), shows the new bank and redraws the rows when the item leaves the list
 * (func_0019B458).
 *
 * @param widget  the popup state that sent the event
 * @param msg  the popup event (0x15 opened, 0x16 closed)
 */
extern "C" void cFEStateCharEquip_onPopupEvent(void* self, void* widget, int msg)
{
    switch (msg) {
    case 21:
        if (*(int*)((char*)widget + 0xC) != 7) {
            *(int*)((char*)self + 0x1C) &= ~8;
        }
        break;
    case 22:
        if (((*(int*)((char*)self + 0x1C) >> 3) & 1) == 0) {
            *(int*)((char*)self + 0x1C) |= 8;
        }
        if (*(int*)((char*)widget + 0xC) == 7 && *(int*)((char*)widget + 0x6C) != 0) {
            func_0019BE80(self);
            cFEStateCharEquip_updateBankWidget(self);
            if (func_0019B458((sEquipDetail*)self, *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95))) {
                func_0019B618(self, 0, 1);
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
struct sEquipTable;
int GetHashValue32(char* str);
void* cList_first(cList* list);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_onUpdate(void* self);
extern "C" int func_0019E238(sEquipTable* self, int a1, int a2);
extern "C" void func_0019A9B8(void* self);
extern "C" void cFEStateCharEquip_updatePreviewLoad(void* self);
extern "C" void cFEStateCharEquip_update3DPositions(void* self);
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern char str_loading_text[];

struct sVEntryK199938 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Updates the equip-gear state each frame: the icon file's arrival, the "loading text" and the preview loads.
 *
 * Runs the base update cUIState_onUpdate(); once the icon file (0xA64) is ready in the FE file cache, marks it
 * (0xA60) and refreshes the selected item's preview (func_0019A9B8). "loading text" shows in store mode while
 * a large preview is pending (0xA6C, 0x95C), otherwise until the FE rider's model is shown (+0xCC8). Then
 * cFEStateCharEquip_updatePreviewLoad() advances the preview load and cFEStateCharEquip_update3DPositions() places the camera and models.
 */
extern "C" void cFEStateCharEquip_onUpdate(void* self)
{
    cUIState_onUpdate(self);
    if (*(int*)((char*)self + 0xA60) == 0
        && func_0019E238((sEquipTable*)((char*)SSXApp->RealFE + 0x1A70), *(int*)((char*)self + 0xBC),
                         *(int*)((char*)self + 0xA64))) {
        *(int*)((char*)self + 0xA60) = 1;
        func_0019A9B8(self);
    }
    void* screen = cList_first((cList*)((char*)self + 0x24));
    char* obj = (char*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_loading_text));
    if (obj != 0) {
        char* feRider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44));
        int on = 0;
        int mode1 = *(int*)((char*)self + 0xC8) == 1;
        if (mode1) {
            if (*(int*)((char*)self + 0xA6C) >= 0 || *(int*)((char*)self + 0x95C) > 0)
                on = 1;
        } else {
            on = *(int*)(feRider + 0xCC8);
            on ^= 1;
        }
        sVEntryK199938* vt = *(sVEntryK199938**)(obj + 8);
        vt[9].fn(obj + vt[9].delta, on);
    }
    cFEStateCharEquip_updatePreviewLoad(self);
    cFEStateCharEquip_update3DPositions(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onInputBegin);
#ifdef SKIP_ASM
class cPad_9A38 {
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
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual float v36();
    virtual float v37();
    virtual float v38();
};

static inline int isMode1_9A38(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
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
 * Only outside store mode and in view state 1 or 4 (0xAE0): camera rotate adds 3 x getCamRotate() degrees to
 * the turn (0xAAC, kept in 0-360), zoom subtracts 0.1 x getCamZoom() from 0xAA4 (clamped to 0..1) and pan 0.1
 * x getCamPan() from 0xAA8 (clamped to -1..1); these are cUIInput vtable slots 33-38.
 *
 * @param in  the controller input (cUIInput)
 * @return 1 when any camera control was used, otherwise 0
 */
extern "C" int cFEStateCharEquip_onInputBegin(void* self, cPad_9A38* in)
{
    int handled = 0;
    if (!isMode1_9A38(self)) {
    int mode = *(int*)((char*)self + 0xAE0);
    if (mode == 1 || mode == 4) {
    if (in->v35()) {
        *(float*)((char*)self + 0xAAC) += in->v38() * 3.0f;
        while (*(float*)((char*)self + 0xAAC) >= 360.0f)
            *(float*)((char*)self + 0xAAC) -= 360.0f;
        while (*(float*)((char*)self + 0xAAC) < 0.0f)
            *(float*)((char*)self + 0xAAC) += 360.0f;
        handled = 1;
    }
    if (in->v33()) {
        // PORT: <? (GNU min operator)
        *(float*)((char*)self + 0xAA4) = clamp_9A38(*(float*)((char*)self + 0xAA4) + in->v36() * -0.10000000149011612f, 0.0f, 1.0f);
        handled = 1;
    }
    if (in->v34()) {
        *(float*)((char*)self + 0xAA8) = clamp_9A38(*(float*)((char*)self + 0xAA8) + in->v37() * -0.10000000149011612f, -1.0f, 1.0f);
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
extern "C" void cFEStateCharEquip_updateHeading(void* self, int id);
extern "C" void func_0019B180(void* self, int id);
extern "C" void func_0019B618(void* self, int a1, int a2);
extern "C" void cFEStateCharEquip_startLargePreviewLoad(void* self, int a1);
extern "C" void func_0019A9B8(void* self);
extern "C" void func_0019ACA0(void* self);
extern "C" int func_0019BEE8(void* self);
extern "C" void func_0019C7E8(void* self, void* item);
extern "C" void cFEStateCharEquip_updatePolyBar(void* self);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(void* self, int a1);
extern "C" int func_0014B478(void* self, int a1, int a2);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern int D_004A11E0;
extern int D_004A18D8;

struct sList_9C28 {
    char pad_0x0[0x98];
    unsigned char top;   // 0x98
};

struct sItem_9C28 {
    int f0;
    short id;           // 0x4
    char pad6[0xE - 0x6];
    short cost;         // 0xE
    char pad10[0x34 - 0x10];
    int flags;          // 0x34
};

struct sVE_9C28 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sCharEquip_9C28 {
    char pad0[0x10];
    char* menu;                     // 0x10
    char pad14[0x44 - 0x14];
    signed char team;               // 0x44
    char pad45[0x5C - 0x45];
    sList_9C28* list;               // 0x5C
    char pad60[0xBC - 0x60];
    int slot;                       // 0xBC
    int money;                      // 0xC0
    int spent;                      // 0xC4
    int mode;                       // 0xC8
    char padCC[0xD0 - 0xCC];
    int stack[(0x120 - 0xD0) / 4];  // 0xD0
    int depth;                      // 0x120
    sItem_9C28* items[(0x958 - 0x124) / 4]; // 0x124
    int count;                      // 0x958
    char pad95C[0xA78 - 0x95C];
    int vals[6];                    // 0xA78
    char padA90[0xAE0 - 0xA90];
    int state;                      // 0xAE0
};

static inline int isMode1_9C28(sCharEquip_9C28* self)
{
    return self->mode == 1;
}

/**
 * Handles the item rows: opens a category, buys or equips an item, goes back up a category, or leaves.
 *
 * Event 5 (accept) on row top + misc data: a category (flag 0x20) is pushed on the stack (0xD0, depth 0x120)
 * and listed, the boards category (3) easing the camera to the board (0xAE0 = 3); an item (flag 4) opens the
 * buy popup in store mode when the money covers its price x 10, or in the inventory is equipped or taken off
 * (func_0019BEE8) when the row's points fit the 3392 budget, then the bars and rider model are refreshed.
 *
 * Event 6 (back) lists the parent category (leaving boards eases back, 0xAE0 = 4) or, at the root, sets the
 * FE rider's Customize speech request (+0xC1C) and pushes getPrevState's state. Event 1 (row focused) starts
 * a delayed large-preview load in store mode and refreshes the item preview.
 *
 * @param selfv  the equip-gear state (this)
 * @param w  the row widget that sent the event; nothing happens when null
 * @param ev  the widget event (5 accept, 6 back, 1 focused)
 */
extern "C" void cFEStateCharEquip_onWidgetEvent(void* selfv, char* w, unsigned int ev)
{
    sCharEquip_9C28* self = (sCharEquip_9C28*)selfv;
    if (w == 0)
        return;
    switch (ev) {
    case 5: {
        if (self->list == 0)
            return;
        int idx = self->list->top + *(int*)(w + 0x18);
        if (idx >= self->count)
            return;
        sItem_9C28* item = self->items[idx];
        int id = item->id;
        if (item->flags & 0x20) {
            self->depth++;
            cFEStateCharEquip_updateHeading(self, id);
            self->stack[self->depth] = id;
            func_0019B180(self, id);
            func_0019B618(self, 1, 1);
            if (id == D_004A11E0)
                self->state = 3;
            return;
        }
        if (!(item->flags & 4))
            return;
        if (isMode1_9C28(self)) {
            int cost = item->cost;
            if (cost > 0)
                cost *= 10;
            if (self->money < cost)
                return;
            func_0019C7E8(self, item);
            return;
        }
        int* vals = self->vals;
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        if (D_004A18D8 < *(int*)((char*)vals + (*(int*)(w + 0x18) << 2)) + self->spent)
            return;
        if (!func_0019BEE8(self))
            return;
        self->spent = func_0014B478(iface, self->team, self->slot);
        func_0019B618(self, 0, 1);
        cFEStateCharEquip_updatePolyBar(self);
        cFEStateCharEquip_updatePolyEquipPreviewBar(self, *(int*)((char*)vals + (*(int*)(w + 0x18) << 2)));
        func_0019ACA0(self);
        return;
    }
    case 6: {
        if (self->depth > 0) {
            int* stack = self->stack;
            if (*(int*)((char*)stack + (self->depth << 2)) == D_004A11E0)
                self->state = 4;
            cFEStateCharEquip_updateHeading(self, *(int*)((char*)stack + ((self->depth - 1) << 2)));
            func_0019B180(self, *(int*)((char*)stack + ((self->depth - 1) << 2)));
            func_0019B618(self, 1, 1);
        } else {
            char* feRider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, self->team);
            *(int*)(feRider + 0xC20) = 0;
            *(int*)(feRider + 0xC1C) = 1;
            char* obj = *(char**)self->menu;
            sVE_9C28* vt = *(sVE_9C28**)(obj + 4);
            void* x = vt[5].fn(obj + vt[5].delta, self, 0);
            if (x)
                cUIStateStack_push(self->menu + 0x18, x);
        }
        break;
    }
    case 1:
        if (isMode1_9C28(self))
            cFEStateCharEquip_startLargePreviewLoad(self, 1);
        func_0019A9B8(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_onWidgetMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cFEStateCharEquip_switchEquipMode(void* self);
extern int D_004A18D8;

struct sEquipItem_9F20 {
    char pad0[0xE];
    short cost;      // 0xE
    char pad10[0x24];
    int flags;       // 0x34
};

struct sEquipDetail_9F20 {
    char pad0[0x5C];
    char* sel;                    // 0x5C
    char pad60[0x60];
    int fC0;                      // 0xC0
    int fC4;                      // 0xC4
    int fC8;                      // 0xC8
    char padCC[0x54];
    int f120;                     // 0x120
    sEquipItem_9F20* items[0x20D]; // 0x124
    int count;                    // 0x958
    char pad95C[0x11C];
    int prices[27];               // 0xA78
    int fAE4;                     // 0xAE4
};

static inline bool isMode_9F20(int m, int v)
{
    return m == v;
}

static inline bool hasFlag_9F20(sEquipItem_9F20* item, int f)
{
    return (item->flags & f) != 0;
}

/**
 * Answers the item menu's queries and buttons: row checks, scrolling, accept, back and mode switch.
 *
 * Event 0 is true for an item with flag 4 and event 1 for an existing row; 2 and 3 (scrolled) redraw the rows
 * and give 1. Event 6 (next) gives 0x101 for a category, any store item (its price test gives 0x101 either
 * way) or an inventory item whose points fit (row cost at 0xA78 plus 0xC4 at most 3392), else 0x110 (the
 * item's mSound[0]). Event 7 (back) steps the category depth (0x120) down and gives 1; 8 switches the mode
 * (cFEStateCharEquip_switchEquipMode) when allowed (0xAE4) and gives 0x101, else 0x100; 9 gives 0x100.
 *
 * @param pMenu  the menu that sent the event (unused)
 * @param kind  the menu event
 * @param idx  the row index for events 0 and 1
 * @return a flag for events 0 and 1, otherwise feedback bits (0x100 frame, 1 mSound[1], 0x10 mSound[0])
 */
extern "C" int cFEStateCharEquip_onWidgetMenu(sEquipDetail_9F20* self, int pMenu, int kind, int idx)
{
    switch (kind) {
    case 2:
    case 3:
        func_0019B618(self, 0, 0);
        return 1;
    case 0:
        if (idx < self->count) {
            return hasFlag_9F20(self->items[idx], 4);
        }
        return 0;
    case 1:
        if (idx == 0 || self->count == 0 || idx < self->count) return 1;
        return 0;
    case 6: {
        char* selected = self->sel;
        int selIndex = *(unsigned char*)(selected + 0x95);
        if (selIndex < 0) return 272;
        if (selIndex < self->count) {
            sEquipItem_9F20* item = self->items[selIndex];
            if (item->flags & 0x20) return 257;
            if (isMode_9F20(self->fC8, 1)) {
                int cost = item->cost;
                if (cost > 0) cost *= 10;
                if (cost >= self->fC0) break;
                return 257;
            } else {
                if (D_004A18D8 >= self->prices[selIndex - *(unsigned char*)(selected + 0x98)] + self->fC4) break;
            }
        }
        return 272;
    }
    case 7:
        if (self->f120 > 0) {
            self->f120 = self->f120 - 1;
            return 1;
        }
        break;
    case 8:
        if (self->fAE4 != 0) {
            cFEStateCharEquip_switchEquipMode(self);
            return 257;
        }
        return 0x100;
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_getIconID);
#ifdef SKIP_ASM
struct sEquipTable;
// PORT: func_0019E2B0 returns a pointer through int (the unit defines it as int).
extern "C" int func_0019E2B0(sEquipTable* self, int a1, int a2);
extern "C" void SHAPE_name(void* pkg, int i, char* name);
extern "C" int func_004165A8(const void* a, const void* b);
extern char str_icon[];

struct sPkgEntK19A098 {
    int off;
    int f4;
};
struct sPkgK19A098 {
    int f0;
    int f4;
    int count;      // 0x8
    int fC;
    int f10;
    sPkgEntK19A098 ents[1];   // 0x14
};
struct sVEK19A098a {
    short delta;
    short index;
    int (*fn)(void*, char*, char*, int, int, int);
};
struct sVEK19A098b {
    short delta;
    short index;
    void (*fn)(void*, int, char*, int);
};

/**
 * Finds an icon in the character's loaded icon package and returns its texture.
 *
 * Needs the icon package loaded (+0xA60) and present in the file manager (func_0019E2B0 with the character's bank +0xBC
 * and file +0xA64). Compares each image's name, cut to 4 characters, with @p name; on a match creates the icon texture
 * (graphics manager vtable entry 46, kind "icon") into +0xA68 the first time, or reloads that texture with the image
 * (entry 48). All icons share that one texture.
 *
 * @param name  the icon name
 * @return the texture id (+0xA68), or -1 when the package is not loaded or has no such image
 */
extern "C" int cFEStateCharEquip_getIconID(void* self, const char* name)
{
    if (*(int*)((char*)self + 0xA60) == 0)
        return -1;
    sPkgK19A098* pkg = (sPkgK19A098*)func_0019E2B0((sEquipTable*)((char*)SSXApp->RealFE + 0x1A70),
                                                  *(int*)((char*)self + 0xBC), *(int*)((char*)self + 0xA64));
    if (pkg == 0)
        return -1;
    int n = pkg->count;
    int i;
    for (i = 0; i < n; i++) {
        char nm[16];
        SHAPE_name(pkg, i, nm);
        nm[4] = 0;
        if (func_004165A8(nm, name) == 0) {
            sPkgEntK19A098* entry = &pkg->ents[i];
            char* data = (char*)pkg + entry->off;
            int id = *(int*)((char*)self + 0xA68);
            if (id == -1) {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                sVEK19A098a* vt = (sVEK19A098a*)ps2GraphicsMan->mVtbl;
                *(int*)((char*)self + 0xA68) = vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, data, str_icon, 0, 1, -1);
            } else {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                sVEK19A098b* vt = (sVEK19A098b*)ps2GraphicsMan->mVtbl;
                vt[48].fn((char*)ps2GraphicsMan + vt[48].delta, id, data, 1);
            }
            return *(int*)((char*)self + 0xA68);
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateIconTexturesLoad);
#ifdef SKIP_ASM
struct sEquipTable;
extern "C" int func_0019E238(sEquipTable* self, int a1, int a2);
extern "C" void func_0019A9B8(void* self);

/**
 * Marks the icon package loaded once its file is ready and refreshes the item preview.
 *
 * When +0xA60 is clear and the file manager reports the icon file ready (func_0019E238 on +0xBC, +0xA64), sets +0xA60
 * and calls func_0019A9B8. No call site was found in the asm.
 */
extern "C" void cFEStateCharEquip_updateIconTexturesLoad(void* self)
{
    if (*(int*)((char*)self + 0xA60) == 0
        && func_0019E238((sEquipTable*)((char*)SSXApp->RealFE + 0x1A70), *(int*)((char*)self + 0xBC),
                         *(int*)((char*)self + 0xA64))) {
        *(int*)((char*)self + 0xA60) = 1;
        func_0019A9B8(self);
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

struct sVEntry0019A238 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Sets the equip screen up for the inventory.
 *
 * Titles it kT_CMNEquipGear (+0x48), hides the money group (+0x60) and the large preview (+0x88), shows the small icon
 * preview group (+0x80), shows the user's front-end rider (cFERider_setVisible with 1) and sets the camera state (+0xAE0) to
 * 1.
 */
extern "C" void cFEStateCharEquip_initInventoryPreview(void* self)
{
    if (*(cUIText**)((char*)self + 0x48) != 0) {
        cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x48), GetHashValue32(str_kT_CMNEquipGear));
    }
    char* widget = *(char**)((char*)self + 0x60);
    if (widget != 0) {
        sVEntry0019A238* vt = *(sVEntry0019A238**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 0);
    }
    widget = *(char**)((char*)self + 0x88);
    if (widget != 0) {
        sVEntry0019A238* vt = *(sVEntry0019A238**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 0);
    }
    widget = *(char**)((char*)self + 0x80);
    if (widget != 0) {
        sVEntry0019A238* vt = *(sVEntry0019A238**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 1);
    }
    cFERider_setVisible(cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44)), 1);
    *(int*)((char*)self + 0xAE0) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_initStorePreview);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_kT_CMNBuyGear[];

struct sVEntry0019A308 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Sets the equip screen up for the store.
 *
 * Titles it kT_CMNBuyGear (+0x48), hides the small icon preview group (+0x80) and the points group (+0x6C), hides the
 * user's front-end rider and shows the money group (+0x60).
 */
extern "C" void cFEStateCharEquip_initStorePreview(void* self)
{
    if (*(cUIText**)((char*)self + 0x48) != 0) {
        cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x48), GetHashValue32(str_kT_CMNBuyGear));
    }
    char* widget = *(char**)((char*)self + 0x80);
    if (widget != 0) {
        sVEntry0019A308* vt = *(sVEntry0019A308**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 0);
    }
    widget = *(char**)((char*)self + 0x6C);
    if (widget != 0) {
        sVEntry0019A308* vt = *(sVEntry0019A308**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 0);
    }
    cFERider_setVisible(cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    widget = *(char**)((char*)self + 0x60);
    if (widget != 0) {
        sVEntry0019A308* vt = *(sVEntry0019A308**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 1);
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
extern "C" void cFEStateCharEquip_updateHeading(void* self, int id);
extern "C" void func_0019B180(void* self, int id);
extern int D_004A11D0;
extern char str_kT_CMNBuyGear[];
extern char str_kT_CMNEquipGear[];

struct sEquipDetailK19A3D0 {
    char pad_0x000[0x120];
    int f120;      // 0x120
};

static inline int isDetailK19A3D0(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
}

/**
 * Switches the equip screen between the inventory (mode 0) and the store (mode 1) and lists the root category again.
 *
 * Flips +0xC8, sets the screen up for the new mode (cFEStateCharEquip_initInventoryPreview or cFEStateCharEquip_initStorePreview) and labels the button help (+0x50)
 * with the other mode (kT_CMNBuyGear in the inventory, kT_CMNEquipGear in the store). Then empties the category stack
 * (depth +0x120 = 0, the 80 bytes at +0xD0 filled with D_004A11D0), updates the heading and rebuilds and redraws the
 * list (func_0019B180, func_0019B618 with 1, 1).
 */
extern "C" void cFEStateCharEquip_switchEquipMode(void* self)
{
    if (isDetailK19A3D0(self)) {
        *(int*)((char*)self + 0xC8) = 0;
        cFEStateCharEquip_initInventoryPreview(self);
        if (*(cUIText**)((char*)self + 0x50) != 0) {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x50), GetHashValue32(str_kT_CMNBuyGear));
        }
    } else {
        *(int*)((char*)self + 0xC8) = 1;
        cFEStateCharEquip_initStorePreview(self);
        if (*(cUIText**)((char*)self + 0x50) != 0) {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x50), GetHashValue32(str_kT_CMNEquipGear));
        }
    }
    ((sEquipDetailK19A3D0*)self)->f120 = 0;
    MEM_fill((char*)self + 0xD0, D_004A11D0, 80);
    cFEStateCharEquip_updateHeading(self, D_004A11D0);
    func_0019B180(self, D_004A11D0);
    func_0019B618(self, 1, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cancelLargePreviewLoad);
#ifdef SKIP_ASM
struct sEquipTable_DC20;
extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i);

/**
 * Cancels the pending large-preview load.
 *
 * Releases the queued preview file (+0xA6C, func_0019DC20) if any, clears the file name (+0x960) and sets +0xA6C and
 * the delay (+0x95C) to -1.
 */
extern "C" void cFEStateCharEquip_cancelLargePreviewLoad(void* self)
{
    int idx = *(int*)((char*)self + 0xA6C);
    if (idx >= 0) {
        func_0019DC20((sEquipTable_DC20*)((char*)SSXApp->RealFE + 0x1A70), *(int*)((char*)self + 0xBC), idx);
    }
    *(char*)((char*)self + 0x960) = 0;
    *(int*)((char*)self + 0xA6C) = -1;
    *(int*)((char*)self + 0x95C) = -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_startLargePreviewLoad);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* func_0014B700(void* iface, int rider);
extern "C" char* func_0041ACC0(const char* s, int c);
extern "C" char* strncpy(char* dst, const char* src, int n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFEStateCharEquip_cancelLargePreviewLoad(void* self);
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
 * and builds the file name at +0x960 as "%s_%s%s": the character's file name (func_0014B700) without the character
 * before its '.', the item name from its first digit, and the extension. The name buffer is both the destination and
 * the first source of that sprintf.
 *
 * @param flag  nonzero for the longer delay
 */
extern "C" void cFEStateCharEquip_startLargePreviewLoad(void* self, int flag)
{
    if (*(void**)((char*)self + 0x5C) == 0)
        return;
    cFEStateCharEquip_cancelLargePreviewLoad(self);
    releaseK199148(*(int*)((char*)self + 0xA74));
    *(int*)((char*)self + 0xA74) = *(int*)((char*)self + 0xA70);
    *(int*)((char*)self + 0xA70) = -1;
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95);
    if (idx < 0)
        return;
    if (idx >= *(int*)((char*)self + 0x958))
        return;
    char* namePart = *(char**)(*(char**)((char*)self + (idx << 2) + 0x124) + 0x30);
    if (namePart == 0)
        return;
    if (flag)
        *(int*)((char*)self + 0x95C) = 20;
    else
        *(int*)((char*)self + 0x95C) = 1;
    char* buf = (char*)self + 0x960;
    char* fileName = func_0014B700(cBE_getInterface_Fv(cBE_getBE(), 9), *(int*)((char*)self + 0xBC));
    char* dot = func_0041ACC0(fileName, '.');
    int len = dot - fileName - 1;
    strncpy(buf, fileName, len);
    buf[len] = 0;
    while (*namePart != 0) {
        if (*namePart >= '0' && *namePart <= '9')
            break;
        namePart++;
    }
    sprintf((char*)self + 0x960, str_pct_s_pct_spct_s, (char*)self + 0x960, namePart, dot);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePreviewLoad);
#ifdef SKIP_ASM
struct sEquipTable;
struct sEquipTable_DC20;
extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i);
extern "C" int func_0019E238(sEquipTable* self, int a1, int a2);
// PORT: func_0019E2B0 returns a pointer through int (the unit defines it as int).
extern "C" int func_0019E2B0(sEquipTable* self, int a1, int a2);
extern "C" int func_0019DA10(void* self, int bank, void* name, int a3, int a4, int a5);
extern "C" void func_0019A9B8(void* self);
extern char str_preview[];

struct sVEntryK19A638 {
    short delta;
    short index;
    int (*fn)(void*, void*, char*, int, int, int);
};

/**
 * Advances the store's large-preview load (store mode only).
 *
 * Counts the delay (+0x95C) down and, when it reaches 0, queues the preview file (func_0019DA10 kind 9 into +0xA6C).
 * Once the queued file is ready, makes its texture (graphics manager vtable entry 46, kind "preview") into +0xA70, or
 * -1 when the file is empty, releases the file and refreshes the item preview (func_0019A9B8).
 */
extern "C" void cFEStateCharEquip_updatePreviewLoad(void* self)
{
    int mode1 = *(int*)((char*)self + 0xC8) == 1;
    if (!mode1)
        return;
    if (*(int*)((char*)self + 0x95C) > 0) {
        if (--*(int*)((char*)self + 0x95C) == 0) {
            *(int*)((char*)self + 0xA6C) = func_0019DA10((char*)SSXApp->RealFE + 0x1A70,
                                                         *(int*)((char*)self + 0xBC), (char*)self + 0x960, 9, 1, 0);
            *(int*)((char*)self + 0x95C) = -1;
        }
    } else if (*(int*)((char*)self + 0xA6C) >= 0) {
        char* tbl = (char*)SSXApp->RealFE + 0x1A70;
        if (func_0019E238((sEquipTable*)tbl, *(int*)((char*)self + 0xBC), *(int*)((char*)self + 0xA6C))) {
            char* entry = (char*)func_0019E2B0((sEquipTable*)tbl, *(int*)((char*)self + 0xBC), *(int*)((char*)self + 0xA6C));
            if (entry != 0) {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                sVEntryK19A638* vt = (sVEntryK19A638*)ps2GraphicsMan->mVtbl;
                *(int*)((char*)self + 0xA70) = vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, entry + *(int*)(entry + 0x14), str_preview, 0, 1, -1);
            } else {
                *(int*)((char*)self + 0xA70) = -1;
            }
            func_0019DC20((sEquipTable_DC20*)tbl, *(int*)((char*)self + 0xBC), *(int*)((char*)self + 0xA6C));
            *(int*)((char*)self + 0xA6C) = -1;
            func_0019A9B8(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_cancelPreviewLoad);
#ifdef SKIP_ASM
extern "C" void func_0019FA78(void* a0, int a1);

/**
 * Cancels the user's front-end rider's texture loads (func_0019FA78 with no item list) when the front end exists.
 *
 * No call site was found in the asm.
 */
extern "C" void cFEStateCharEquip_cancelPreviewLoad(void* self)
{
    char* mgr = (char*)SSXApp->RealFE;
    if (mgr != 0) {
        func_0019FA78(cFERiderManager_getHuman1(mgr + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateHelpText);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
class cUIObj_A798 {
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
struct sEquipEntry_A798 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_A798 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_A798 entries[1];  // 0x290
};
struct sEquipItem_A798 {
    short f0;
    signed char slot;   // 0x2
    char f3;
    short id;       // 0x4
    char pad_0x6[0xE - 0x6];
    short cost;     // 0xE
    char pad_0x10[0x34 - 0x10];
    int flags;      // 0x34
};
struct sList_A798 {
    char pad_0x0[0x95];
    unsigned char sel;   // 0x95
    unsigned char count; // 0x96
    char pad_0x97;
    unsigned char top;   // 0x98
};
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern int D_004A18D8;
extern char str_kT_HELPNoItemsCTM[];
extern char str_kT_HELPBuyItemInCTM[];
extern char str_kT_HELPBoughtMaxItems[];
extern char str_kT_FEHELPChooseCatContinue[];
extern char str_kT_FEHELPItemAvailBuy[];
extern char str_kT_129HELPSaveCashItem[];
extern char str_kT_FEYourCurrentOutfit[];
extern char str_kT_FEHELPRemoveItems[];

static inline int isMode1_A798(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
}

static inline sEquipEntry_A798* getEntry_A798(sEquipDb_A798* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

static inline void setText_A798(void* self, char* s)
{
    cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x4C), GetHashValue32(s));
}

/**
 * Sets the equip screen's help text for the selected row.
 *
 * Hides the help text (+0x4C), then: with no item selected, kT_HELPBoughtMaxItems in the store, else kT_HELPNoItemsCTM
 * (when +0xAE4 is set) or kT_HELPBuyItemInCTM; on a category (flag 0x20) kT_FEHELPChooseCatContinue; in the store
 * kT_FEHELPItemAvailBuy when the money (+0xC0) covers the price x 10, else kT_129HELPSaveCashItem; in the inventory
 * kT_FEHELPRemoveItems when the row's points would pass the limit D_004A18D8, else kT_FEYourCurrentOutfit. Shows the
 * help text again. Likely bug: both outcomes of the item's equipped test (entry flag 4) choose kT_FEYourCurrentOutfit.
 */
extern "C" void cFEStateCharEquip_updateHelpText(void* self)
{
    if (*(cUIObj_A798**)((char*)self + 0x4C) == 0)
        return;
    (*(cUIObj_A798**)((char*)self + 0x4C))->setVisible(0);
    int sel = (*(sList_A798**)((char*)self + 0x5C))->sel;
    if (sel < 0 || sel >= *(int*)((char*)self + 0x958)) {
        (*(cUIObj_A798**)((char*)self + 0x4C))->setVisible(1);
        if (!isMode1_A798(self)) {
            if (*(int*)((char*)self + 0xAE4) != 0)
                setText_A798(self, str_kT_HELPNoItemsCTM);
            else
                setText_A798(self, str_kT_HELPBuyItemInCTM);
        } else {
            setText_A798(self, str_kT_HELPBoughtMaxItems);
        }
        return;
    }
    sEquipItem_A798* item = *(sEquipItem_A798**)((char*)self + (sel << 2) + 0x124);
    if (item->flags & 0x20) {
        setText_A798(self, str_kT_FEHELPChooseCatContinue);
    } else if (isMode1_A798(self)) {
        int cost = item->cost;
        if (cost > 0)
            cost *= 10;
        if (*(int*)((char*)self + 0xC0) >= cost)
            setText_A798(self, str_kT_FEHELPItemAvailBuy);
        else
            setText_A798(self, str_kT_129HELPSaveCashItem);
    } else {
        sEquipDb_A798* db = (sEquipDb_A798*)func_0014AD28(cBE_getInterface(cBE_getBE(), 9), *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC));
        sEquipEntry_A798* equipEntry = getEntry_A798(db, item->id);
        int idx = sel - (*(sList_A798**)((char*)self + 0x5C))->top;
        if (D_004A18D8 >= *(int*)((char*)self + 0xC4) + *(int*)((char*)self + (idx << 2) + 0xA78)) {
            if (equipEntry->flags & 4)
                setText_A798(self, str_kT_FEYourCurrentOutfit);
            else
                setText_A798(self, str_kT_FEYourCurrentOutfit);
        } else {
            setText_A798(self, str_kT_FEHELPRemoveItems);
        }
    }
    (*(cUIObj_A798**)((char*)self + 0x4C))->setVisible(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019A9B8);
#ifdef SKIP_ASM
extern "C" void func_0019AB78(void* self);
extern "C" void func_0019AA08(void* self);
extern "C" void cFEStateCharEquip_updateHelpText(void* self);

/**
 * Refreshes the equip screen's item preview for the current mode (func_0019AB78 in the store, func_0019AA08 in the
 * inventory) and then its help text (cFEStateCharEquip_updateHelpText).
 */
extern "C" void func_0019A9B8(void* self)
{
    bool buyMode = *(int*)((char*)self + 0xC8) == 1;
    if (buyMode) {
        func_0019AB78(self);
    } else {
        func_0019AA08(self);
    }
    cFEStateCharEquip_updateHelpText(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019AA08);
#ifdef SKIP_ASM
extern "C" int cFEStateCharEquip_getIconID(void* self, const char* name);
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(void* self, int a1);

struct sVEK19AA08 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void showK19AA08(char* o, int on)
{
    sVEK19AA08* vt = *(sVEK19AA08**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

/**
 * Refreshes the inventory preview for the selected row.
 *
 * Hides the points group (+0x6C), the small bolt icon (+0x84) and the points cost (+0x7C). For an item (flag 4, not a
 * category) shows the points group, takes the row's points (+0xA78) and, when the icons are loaded and the item has a
 * name, shows the icon with its texture (cFEStateCharEquip_getIconID). Finally updates the points preview bar (cFEStateCharEquip_updatePolyEquipPreviewBar) with
 * the row's points, or 0.
 */
extern "C" void func_0019AA08(void* self)
{
    if (*(void**)((char*)self + 0x5C) == 0)
        return;
    char* widget = *(char**)((char*)self + 0x6C);
    if (widget != 0)
        showK19AA08(widget, 0);
    widget = *(char**)((char*)self + 0x84);
    if (widget != 0)
        showK19AA08(widget, 0);
    widget = *(char**)((char*)self + 0x7C);
    if (widget != 0)
        showK19AA08(widget, 0);
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95);
    int rowValue = 0;
    if (idx < *(int*)((char*)self + 0x958)) {
        char* item = *(char**)((char*)self + (idx << 2) + 0x124);
        unsigned int flags = *(unsigned int*)(item + 0x34);
        if (flags & 4) {
            if (!(flags & 0x20)) {
                showK19AA08(*(char**)((char*)self + 0x6C), 1);
                int topIndex = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x98);
                char* iconWidget = *(char**)((char*)self + 0x84);
                rowValue = *(int*)((char*)self + ((idx - topIndex) << 2) + 0xA78);
                if (iconWidget != 0 && *(char**)(item + 0x30) != 0 && *(int*)((char*)self + 0xA60) != 0) {
                    showK19AA08(iconWidget, 1);
                    int iconId = cFEStateCharEquip_getIconID(self, *(char**)(item + 0x30));
                    char* iconWidget2 = *(char**)((char*)self + 0x84);
                    *(int*)(iconWidget2 + 0x78) = iconId;
                    *(int*)(iconWidget2 + 0x7C) = 0;
                }
            }
        }
    }
    cFEStateCharEquip_updatePolyEquipPreviewBar(self, rowValue);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019AB78);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
// PORT: the unit declares getDollarString(void*), but it formats an int price; bind an int view.
extern "C" const char* getDollarString_price(int price) __asm__("getDollarString");

struct sVEntryK19AB78 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void showK19AB78(void* o, int on)
{
    sVEntryK19AB78* vt = *(sVEntryK19AB78**)((char*)o + 8);
    vt[9].fn((char*)o + vt[9].delta, on);
}

/**
 * Refreshes the store preview for the selected row.
 *
 * Shows the large preview (+0x88) with the loaded preview texture (+0xA70) when an item is selected and the texture is
 * ready, and the cost (+0x68) as the item's price x 10 (getDollarString) for an item that is not a category; otherwise
 * hides them.
 */
extern "C" void func_0019AB78(void* self)
{
    char* menu = *(char**)((char*)self + 0x5C);
    if (menu == 0)
        return;
    int sel = *(unsigned char*)(menu + 0x95);
    char* item = 0;
    if (sel >= 0 && sel < *(int*)((char*)self + 0x958))
        item = *(char**)((char*)self + (sel << 2) + 0x124);
    if (*(void**)((char*)self + 0x88) != 0) {
        showK19AB78(*(void**)((char*)self + 0x88), 0);
        if (item != 0 && *(int*)((char*)self + 0xA70) >= 0) {
            char* widget = *(char**)((char*)self + 0x88);
            *(int*)(widget + 0x78) = *(int*)((char*)self + 0xA70);
            *(int*)(widget + 0x7C) = 0;
            showK19AB78(*(void**)((char*)self + 0x88), 1);
        }
    }
    if (*(void**)((char*)self + 0x68) != 0) {
        showK19AB78(*(void**)((char*)self + 0x68), 0);
        if (item != 0 && !(*(int*)(item + 0x34) & 0x20)) {
            int price = *(short*)(item + 0xE);
            if (price > 0)
                price *= 10;
            cUIText_setAsciiString(*(cUIText**)((char*)self + 0x68), getDollarString_price(price));
            showK19AB78(*(void**)((char*)self + 0x68), 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019ACA0);
#ifdef SKIP_ASM
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void func_0019FBE0(void* rider, void* item, int a2, int a3);

/**
 * Requeues the user's front-end rider's textures for the character's current items (func_0019FBE0 with 0, 1).
 *
 * cFEStateCharEquip_onWidgetEvent calls it after equipping or removing an item.
 */
extern "C" void func_0019ACA0(void* self)
{
    void* rider = cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44));
    func_0019FBE0(rider, func_0014AD28(cBE_getInterface_Fv(cBE_getBE(), 9), *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC)), 0, 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019AD18);
#ifdef SKIP_ASM
extern "C" int func_0019B518(void* self);
extern "C" int func_0019B598(void* self);
extern "C" void func_0019B7E0(void* self, int idx);
extern "C" void func_0019BA60(void* self, int idx);
extern "C" void* cUIText_getUnicodeString(void* item);
extern "C" void cUIText_setUnicodeString(void* item, void* value);

struct sVEntry0019AD18a {
    short delta;
    short index;
    void (*fn)(void*, char*);
};

struct sVEntry0019AD18b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline int isMode1_19AD18(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
}

struct sCharEquipDetail_19AD18 {
    char pad0[0x5C];
    int f5C;                 // 0x5C
    char pad60[0x8C - 0x60];
    char* a8C[6];            // 0x8C
    char* aA4[6];            // 0xA4
    char padBC[0xC8 - 0xBC];
    int fC8;                 // 0xC8
};

static inline void copyRow_19AD18(sCharEquipDetail_19AD18* self, int dst, int src)
{
    char* o = self->a8C[dst];
    sVEntry0019AD18a* vt = *(sVEntry0019AD18a**)(o + 8);
    vt[11].fn(o + vt[11].delta, self->a8C[src] + 0x1C);
    cUIText_setUnicodeString(self->a8C[dst], cUIText_getUnicodeString(self->a8C[src]));
    o = self->aA4[dst];
    sVEntry0019AD18b* vt2 = *(sVEntry0019AD18b**)(o + 8);
    vt2[9].fn(o + vt2[9].delta, (*(int*)(self->aA4[src] + 0x14) >> 6) & 1);
}

/**
 * Scrolls the equip screen's six item rows by @p n.
 *
 * Beyond six rows either way redraws all rows (func_0019B598 in the store, func_0019B518 in the inventory). Otherwise
 * copies each kept row from the row @p n away (the source text's +0x1C passed to vtable entry 11, its string, and the
 * icon's visibility from bit 6 of the source icon's +0x14) and redraws the rows that came into view. No call site was
 * found in the asm.
 *
 * @param n  rows to scroll (negative moves the rows down)
 */
extern "C" void func_0019AD18(sCharEquipDetail_19AD18* self, int n)
{
    if (self->f5C == 0) {
        return;
    }
    if (n < -6 || n > 6) {
        if (isMode1_19AD18(self)) {
            func_0019B598(self);
        } else {
            func_0019B518(self);
        }
        return;
    }
    if (n < 0) {
        int i;
        for (i = 5; i >= -n; i--) {
            copyRow_19AD18(self, i, i + n);
        }
        if (isMode1_19AD18(self)) {
            int j;
            for (j = 0; j < -n; j++) {
                func_0019BA60(self, j);
            }
        } else {
            int j;
            for (j = 0; j < -n; j++) {
                func_0019B7E0(self, j);
            }
        }
    } else if (n > 0) {
        int i;
        for (i = n; i < 6; i++) {
            copyRow_19AD18(self, i - n, i);
        }
        if (isMode1_19AD18(self)) {
            int j;
            for (j = 6 - n; j < 6; j++) {
                func_0019BA60(self, j);
            }
        } else {
            int j;
            for (j = 6 - n; j < 6; j++) {
                func_0019B7E0(self, j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019AFD0);
#ifdef SKIP_ASM
struct sEquipEntry_AFD0 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_AFD0 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_AFD0 entries[1];  // 0x290
};
struct sEquipItem_AFD0 {
    int f0;
    short id;       // 0x4
    char pad_0x6[0x34 - 0x6];
    int flags;      // 0x34
};
extern "C" int func_0014D7E8(int a0, int a1, int a2, void* out, int a4, int a5);

static inline sEquipEntry_AFD0* getEntry_AFD0(sEquipDb_AFD0* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

/**
 * Reports whether a category holds an item of the character that is in the inventory.
 *
 * Lists the category's items (func_0014D7E8 with the character +0xBC) and looks for one with flag 4, not a category,
 * whose character entry has flag 2 (the flag the inventory lists by).
 *
 * @param library  the item library
 * @param db       the character's item entries
 * @param itemId   the category
 * @return 1 when there is one, else 0
 */
extern "C" int func_0019AFD0(void* self, int library, sEquipDb_AFD0* db, int itemId)
{
    sEquipItem_AFD0* items[528];
    int n = func_0014D7E8(library, *(int*)((char*)self + 0xBC), itemId, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        sEquipItem_AFD0* it = items[i];
        if (it->flags & 4) {
            if (getEntry_AFD0(db, it->id)->flags & 2) {
                if (!(it->flags & 0x20)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B098);
#ifdef SKIP_ASM
struct sEquipEntry_B098 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_B098 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_B098 entries[1];  // 0x290
};
struct sEquipItem_B098 {
    short f0;
    signed char slot;   // 0x2
    char f3;
    short id;       // 0x4
    char pad_0x6[0x34 - 0x6];
    int flags;      // 0x34
};
extern "C" int func_0014D7E8(int a0, int a1, int a2, void* out, int a4, int a5);

static inline sEquipEntry_B098* getEntry_B098(sEquipDb_B098* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

/**
 * Reports whether a category has nothing to buy in the current store.
 *
 * Lists the category's items and returns 0 at the first one with flag 4, not a category, whose character entry lacks
 * flag 2 and whose store number equals +0xCC. Unlike func_0019B180's item test it does not accept +0xCC == -1.
 *
 * @param library  the item library
 * @param db       the character's item entries
 * @param itemId   the category
 * @return 0 when the store sells something in it, else 1
 */
extern "C" int func_0019B098(void* self, int library, sEquipDb_B098* db, int itemId)
{
    sEquipItem_B098* items[528];
    int n = func_0014D7E8(library, *(int*)((char*)self + 0xBC), itemId, items, -1, 1);
    int i;
    for (i = 0; i < n; i++) {
        sEquipItem_B098* it = items[i];
        if (it->flags & 4) {
            if (!(getEntry_B098(db, it->id)->flags & 2)) {
                if (it->slot == *(int*)((char*)self + 0xCC)) {
                    if (!(it->flags & 0x20)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B180);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void func_00418EF8_B180(void* base, int n, int size, int (*cmp)(const void*, const void*)) __asm__("func_00418EF8");
extern "C" int func_00198DA0(const void* a, const void* b);

struct sEquipEntry_B180 {
    unsigned short f0;
    unsigned short flags;
};
struct sEquipDb_B180 {
    char pad_0x0[0x288];
    short* map;     // 0x288
    char pad_0x28c[4];
    sEquipEntry_B180 entries[1];  // 0x290
};
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

static inline sEquipEntry_B180* getEntry_B180(sEquipDb_B180* db, int id)
{
    short idx = db->map[id];
    if (idx >= 0) {
        return &db->entries[idx];
    }
    return 0;
}

static inline int isMode1_B180(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
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

// PORT: the unit declares func_0019B180 as void; the body returns the item count.
extern "C" int func_0019B180_impl(void* selfv, int cat) __asm__("func_0019B180");
/**
 * Builds the equip screen's item list for a category.
 *
 * Lists the category's items (func_0014D7E8) into the list at +0x124 (count +0x958, cleared first), keeping only named
 * items and categories. In the store (mode 1): items whose character entry lacks flag 2 and whose store number matches
 * +0xCC (or +0xCC is -1), and categories with something to buy (func_0019B098). In the inventory: items whose entry has
 * flag 2 and categories holding one (func_0019AFD0). Sorts the list (func_00418EF8 with func_00198DA0).
 *
 * @param selfv  the equip state
 * @param cat  the category
 * @return the number of entries
 */
// FAKEMATCH: identical arms `if (self) { if (it->name == 0) continue; } else { <same> }` in the second
// loop (cross-jumped away after reload; before that they lengthen the live range of the self->items
// copy, so global-alloc gives the loop-1 counter s5 and the copy s6 as in retail); plain form 99.01%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int func_0019B180_impl(void* selfv, int cat)
{
    sCharEquip_B180* self = (sCharEquip_B180*)selfv;
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int lib = (int)func_0014BDB8_noarg();
    sEquipDb_B180* db = (sEquipDb_B180*)func_0014AD28(iface, self->team, self->slot);
    sEquipItem_B180* list[528];
    int n = func_0014D7E8(lib, self->slot, cat, list, -1, 0);
    self->count = 0;
    MEM_fill(self->items, 0, 2100);
    if (isMode1_B180(self)) {
        for (int i = 0; i < n; i++) {
            sEquipItem_B180* it = list[i];
            if (it->name == 0)
                continue;
            if (!(it->flags & 4)) {
                if (!(it->flags & 0x20))
                    continue;
            }
            sEquipEntry_B180* entry = getEntry_B180(db, it->id);
            if ((it->flags & 0x20) && func_0019B098(self, lib, (sEquipDb_B098*)db, it->id) == 0) {
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
            sEquipEntry_B180* entry = getEntry_B180(db, it->id);
            if ((!(it->flags & 0x20) && (entry->flags & 2))
                || ((it->flags & 0x20) && func_0019AFD0(self, lib, (sEquipDb_AFD0*)db, it->id) != 0)) {
                self->items[self->count] = it;
                self->count++;
            }
        }
    }
    func_00418EF8_B180(self->items, self->count, 4, func_00198DA0);
    return self->count;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B458);
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
 * @return 1, or 0 when @p idx is past the end
 */
extern "C" int func_0019B458(sEquipDetail* self, int idx)
{
    int top, sel, n;
    if (idx >= self->count) {
        return 0;
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
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B518);
#ifdef SKIP_ASM
extern "C" void func_0019B7E0(void* self, int idx);

/**
 * Draws the inventory rows from the menu's top row (func_0019B7E0), up to six or the end of the list.
 *
 * @return the number of rows drawn
 */
extern "C" int func_0019B518(void* self)
{
    int j = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x98);
    int i = 0;
    while (i < 6 && j < *(int*)((char*)self + 0x958)) {
        func_0019B7E0(self, i);
        i++;
        j++;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B598);
#ifdef SKIP_ASM
extern "C" void func_0019BA60(void* self, int idx);

/**
 * Draws the store rows from the menu's top row (func_0019BA60), up to six or the end of the list.
 *
 * @return the number of rows drawn
 */
extern "C" int func_0019B598(void* self)
{
    int j = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x98);
    int i = 0;
    while (i < 6 && j < *(int*)((char*)self + 0x958)) {
        func_0019BA60(self, i);
        i++;
        j++;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B618);
#ifdef SKIP_ASM
struct cUIText;
class cUIObj_B618 {
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
    virtual void v10();
    virtual void setColor(void* c);
};
struct sList_B618 {
    char pad_0x0[0x95];
    unsigned char sel;   // 0x95
    unsigned char count; // 0x96
    char pad_0x97;
    unsigned char top;   // 0x98
};
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int func_0019B518(void* self);
extern "C" int func_0019B598(void* self);
extern "C" void cUIMenu_setScrollableMenu(void* menu, int n);
extern char D_004C6708[];
extern char str_kT_FEEmpty[];

static inline int isMode1_B618(void* self)
{
    return *(int*)((char*)self + 0xC8) == 1;
}

/**
 * Redraws the equip screen's item rows.
 *
 * Hides the six name (+0x8C) and check (+0xA4) widgets, sets the menu's item count when @p reset is set (the count, or
 * 1 for an empty list) and draws the rows for the mode. With nothing drawn shows kT_FEEmpty in the first row (colour
 * D_004C6708) and resets the menu to one row. The visible row is clamped (0 when @p reset is set) and selected when @p
 * select is set.
 *
 * @param reset   nonzero to reset the menu to the top of a new list
 * @param select  nonzero to select the visible row
 */
extern "C" void func_0019B618(void* self, int reset, int select)
{
    if (*(void**)((char*)self + 0x5C) == 0)
        return;
    cUIObj_B618** nameWidgets = (cUIObj_B618**)((char*)self + 0x8C);
    cUIObj_B618** checkWidgets = (cUIObj_B618**)((char*)self + 0xA4);
    for (int i = 0; i < 6; i++) {
        nameWidgets[i]->setVisible(0);
        checkWidgets[i]->setVisible(0);
    }
    if (reset) {
        if (*(int*)((char*)self + 0x958) == 0)
            cUIMenu_setScrollableMenu(*(void**)((char*)self + 0x5C), 1);
        else
            cUIMenu_setScrollableMenu(*(void**)((char*)self + 0x5C), *(unsigned char*)((char*)self + 0x958));
    }
    int n;
    if (isMode1_B618(self))
        n = func_0019B598(self);
    else
        n = func_0019B518(self);
    int visibleRow;
    if (n == 0) {
        (*(cUIObj_B618**)((char*)self + 0x8C))->setVisible(1);
        (*(cUIObj_B618**)((char*)self + 0x8C))->setColor(D_004C6708);
        cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x8C), GetHashValue32(str_kT_FEEmpty));
        (*(sList_B618**)((char*)self + 0x5C))->count = 1;
        (*(sList_B618**)((char*)self + 0x5C))->top = 0;
        visibleRow = 0;
        n = 1;
    } else {
        sList_B618* scrollList = *(sList_B618**)((char*)self + 0x5C);
        visibleRow = scrollList->sel - scrollList->top;
    }
    if (visibleRow >= n)
        visibleRow = n - 1;
    if (visibleRow < 0)
        visibleRow = 0;
    if (reset)
        visibleRow = 0;
    if (select) {
        sList_B618* scrollList = *(sList_B618**)((char*)self + 0x5C);
        cUIMenu_setSelectedByIndex(scrollList, scrollList->top + visibleRow);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019B7E0);
#ifdef SKIP_ASM
extern "C" int func_0014AFB0(void* self, int a1, int a2, int id, int locked);
extern "C" void* MEM_copy(void* dst, void* src, int n);
extern "C" void func_0014ACB0(void* self, int a1, int a2, void* src);
extern "C" int func_0014B478(void* self, int a1, int a2);

struct sFlagEntry_19B7E0
{
    short id;
    unsigned short flags;
    sFlagEntry_19B7E0() : id(-1), flags(0) {}
};

struct sProfile_19B7E0
{
    char pad[0x288];
    short* index;
    int count;
    sFlagEntry_19B7E0 entries[0x20D];
};

static inline sFlagEntry_19B7E0* getEntry_19B7E0(sProfile_19B7E0* p, int idx)
{
    short k = p->index[idx];
    if (k >= 0)
        return &p->entries[k];
    return 0;
}

static inline int IsSet_19B7E0(sFlagEntry_19B7E0* e)
{
    return e->flags & 4;
}

struct sItem_19B7E0 {
    int f0;
    short id;           // 0x4
    char pad6[0x14 - 0x6];
    char* name;         // 0x14
    char pad18[0x34 - 0x18];
    int flags;          // 0x34
};

struct sCharEquip_19B7E0 {
    char pad0[0x44];
    signed char team;               // 0x44
    char pad45[0x5C - 0x45];
    sList_B618* list;               // 0x5C
    char pad60[0x8C - 0x60];
    cUIObj_B618* texts[6];          // 0x8C
    cUIObj_B618* icons[6];          // 0xA4
    int slot;                       // 0xBC
    char padC0[0x124 - 0xC0];
    sItem_19B7E0* items[(0xA78 - 0x124) / 4]; // 0x124
    int vals[6];                    // 0xA78
    char padA90[0xA98 - 0xA90];
    int colLocked;                  // 0xA98
    int colFree;                    // 0xA9C
    int colOwned;                   // 0xAA0
};

/**
 * Draws inventory row @p i.
 *
 * Writes the item's name (a string id for categories). For an item, tries toggling its equipped flag (func_0014AFB0) on
 * a saved copy of the character's entries, stores the change in equip points (func_0014B478) in the row's points
 * (+0xA78, else -99999) and shows the checkbox (+0xA98) when the item is equipped or the empty box (+0xA9C) when it is
 * not; a failed toggle shows no box. The entries are restored afterwards (func_0014ACB0). A category gets the texture
 * at +0xAA0 (0, so no box).
 *
 * @param selfv  the equip state
 * @param i  the row (0-5)
 */
extern "C" void func_0019B7E0(void* selfv, int i)
{
    sCharEquip_19B7E0* self = (sCharEquip_19B7E0*)selfv;
    sList_B618* scrollList = self->list;
    int top = scrollList ? scrollList->top : 0;
    sItem_19B7E0* item = self->items[top + i];
    cUIObj_B618* text = self->texts[i];
    cUIObj_B618* icon = self->icons[i];
    self->vals[i] = 0xFFFE7961;
    int col = 0;
    if (item->flags & 0x20) {
        col = self->colOwned;
    } else {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        sProfile_19B7E0* prof = (sProfile_19B7E0*)func_0014AD28(iface, self->team, self->slot);
        sFlagEntry_19B7E0* flagEntry = getEntry_19B7E0(prof, item->id);
        sFlagEntry_19B7E0 save[525];
        MEM_copy(save, prof->entries, 2100);
        int before = func_0014B478(iface, self->team, self->slot);
        if (func_0014AFB0(iface, self->team, self->slot, item->id, !IsSet_19B7E0(flagEntry))) {
            self->vals[i] = func_0014B478(iface, self->team, self->slot) - before;
            if (IsSet_19B7E0(flagEntry))
                col = self->colLocked;
            else
                col = self->colFree;
        }
        func_0014ACB0(iface, self->team, self->slot, save);
    }
    if (text) {
        if (item->flags & 0x20)
            cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(item->name));
        else
            cUIText_setAsciiString((cUIText*)text, item->name);
        text->setVisible(1);
    }
    if (icon) {
        if (col) {
            icon->setVisible(1);
            *(int*)((char*)icon + 0x7C) = col;
            *(int*)((char*)icon + 0x78) = -1;
        } else {
            icon->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BA60);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);

struct sVEntryK19BA60 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void showK19BA60(void* o, int on)
{
    sVEntryK19BA60* vt = *(sVEntryK19BA60**)((char*)o + 8);
    vt[9].fn((char*)o + vt[9].delta, on);
}

struct sCharEquipK19BA60 {
    char pad_0x0[0x5C];
    char* menu;             // 0x5C
    char pad_0x60[0x8C - 0x60];
    cUIText* texts[6];      // 0x8C
    char* icons[6];         // 0xA4
    int bank;               // 0xBC
    int money;              // 0xC0
    char pad_0xC4[0x124 - 0xC4];
    char* items[1];         // 0x124
};

/**
 * Draws store row @p i: the item's name and a green (+0xA90) or red (+0xA94) dollar sign by whether the money (+0xC0)
 * covers its price x 10.
 *
 * A category shows its name as a string id and the texture at +0xAA0 (0, so no icon).
 *
 * @param equipState  the equip state
 * @param i  the row (0-5)
 */
extern "C" void func_0019BA60(void* equipState, int i)
{
    sCharEquipK19BA60* self = (sCharEquipK19BA60*)equipState;
    char* menu = self->menu;
    int top = 0;
    if (menu != 0)
        top = *(unsigned char*)(menu + 0x98);
    char* item = self->items[top + i];
    cUIText* text = self->texts[i];
    char* icon = self->icons[i];
    int tex;
    if (*(int*)(item + 0x34) & 0x20) {
        tex = *(int*)((char*)self + 0xAA0);
    } else {
        int price = *(short*)(item + 0xE);
        if (price > 0)
            price *= 10;
        if (self->money < price)
            tex = *(int*)((char*)self + 0xA94);
        else
            tex = *(int*)((char*)self + 0xA90);
    }
    if (text != 0) {
        if (*(int*)(item + 0x34) & 0x20)
            cUIText_setUnicodeStringByID(text, GetHashValue32(*(char**)(item + 0x14)));
        else
            cUIText_setAsciiString(text, *(char**)(item + 0x14));
        showK19BA60(text, 1);
    }
    if (icon != 0) {
        if (tex != 0) {
            showK19BA60(icon, 1);
            *(int*)(icon + 0x7C) = tex;
            *(int*)(icon + 0x78) = -1;
        } else {
            showK19BA60(icon, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateBankWidget);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* getDollarString(void* a0);

/**
 * Shows the money (+0xC0) in the bank text (+0x64) through getDollarString.
 */
extern "C" void cFEStateCharEquip_updateBankWidget(void* self)
{
    if (*(cUIText**)((char*)self + 0x64) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x64), getDollarString(*(void**)((char*)self + 0xC0)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updateHeading);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" int func_0014D448(void* self, int g, int key);
extern int D_004A11D0;
extern char str_kT_FECategories[];

struct sBEEntry38K19BBE0 {
    char pad_0x00[0x14];
    char* name;    // 0x14
    char pad_0x18[0x38 - 0x18];
};
struct sBETableK19BBE0 {
    char pad_0x00[4];
    sBEEntry38K19BBE0* entries; // 0x4
};

static inline sBEEntry38K19BBE0* findEntryK19BBE0(sBETableK19BBE0* t, int g, int key)
{
    int idx = func_0014D448(t, g, key);
    if (idx < 0) {
        return 0;
    }
    return &t->entries[idx];
}

/**
 * Shows the current category in the heading text: kT_FECategories at the root, else the category's name.
 *
 * For an @p id other than -1 (D_004A11D0), looks the category up for the character (0xBC) with
 * func_0014D448() in the equip table from func_0014BDB8() and shows the string whose ID is the hash of its
 * name (+0x14); a missing entry is not checked. Nothing happens without a heading widget (0x58).
 *
 * @param id  the category id, or -1 for the root
 */
extern "C" void cFEStateCharEquip_updateHeading(void* self, int id)
{
    if (*(cUIText**)((char*)self + 0x58) != 0) {
        if (id == D_004A11D0) {
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x58), GetHashValue32(str_kT_FECategories));
        } else {
            sBEEntry38K19BBE0* entry = findEntryK19BBE0((sBETableK19BBE0*)func_0014BDB8_noarg(), *(int*)((char*)self + 0xBC), id);
            cUIText_setUnicodeStringByID(*(cUIText**)((char*)self + 0x58), GetHashValue32(entry->name));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePolyBar);
#ifdef SKIP_ASM
extern int D_004A18D8;

struct sVEntry0019BC90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVec3K19BC90 {
    float x, y, z;
    sVec3K19BC90(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

/**
 * Shows the equip points bar (+0x70) scaled to the current points (+0xC4).
 *
 * The bar's scale (+0x50) becomes (1, -t, 1) with t = (points - 1500) / (D_004A18D8 - 1500), not below 0 (there is no
 * upper clamp).
 */
extern "C" void cFEStateCharEquip_updatePolyBar(void* self)
{
    char* pointsBar = *(char**)((char*)self + 0x70);
    if (pointsBar != 0) {
        float fillRatio = (float)(*(int*)((char*)self + 0xC4) - 1500) / (float)(D_004A18D8 - 1500);
        if (fillRatio < 0.0f) {
            fillRatio = 0.0f;
        }
        sVEntry0019BC90* vt = *(sVEntry0019BC90**)(pointsBar + 8);
        vt[9].fn(pointsBar + vt[9].delta, 1);
        *(sVec3K19BC90*)(*(char**)((char*)self + 0x70) + 0x50) = sVec3K19BC90(1.0f, -fillRatio, 1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFEStateCharEquip_updatePolyEquipPreviewBar);
#ifdef SKIP_ASM
extern int D_004A18D8;
extern char D_004C66E8[];
extern char D_004C66F8[];

struct sVec3K19BD48 {
    float x, y, z;
    sVec3K19BD48(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct sVEntryK19BD48 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sVEntryIK19BD48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows the equip points preview bar (+0x74) for the current points plus @p delta.
 *
 * Scales it (+0x50) to (1, -t, 1) with t = (points + delta - 1500) / (D_004A18D8 - 1500) clamped to 0-1, and colours it
 * (vtable entry 11) D_004C66E8 when the total would pass D_004A18D8, else D_004C66F8.
 *
 * @param delta  the selected item's points
 */
extern "C" void cFEStateCharEquip_updatePolyEquipPreviewBar(void* self, int delta)
{
    if (*(void**)((char*)self + 0x74) == 0)
        return;
    float fillRatio = (float)(*(int*)((char*)self + 0xC4) + delta - 1500) / (float)(D_004A18D8 - 1500);
    if (fillRatio > 1.0f)
        fillRatio = 1.0f;
    else if (fillRatio < 0.0f)
        fillRatio = 0.0f;
    if (D_004A18D8 < *(int*)((char*)self + 0xC4) + delta) {
        char* equipBar = *(char**)((char*)self + 0x74);
        sVEntryK19BD48* vt = *(sVEntryK19BD48**)(equipBar + 8);
        vt[11].fn(equipBar + vt[11].delta, D_004C66E8);
    } else {
        char* equipBar = *(char**)((char*)self + 0x74);
        sVEntryK19BD48* vt = *(sVEntryK19BD48**)(equipBar + 8);
        vt[11].fn(equipBar + vt[11].delta, D_004C66F8);
    }
    {
        char* equipBar = *(char**)((char*)self + 0x74);
        sVEntryIK19BD48* vt = *(sVEntryIK19BD48**)(equipBar + 8);
        vt[9].fn(equipBar + vt[9].delta, 1);
    }
    *(sVec3K19BD48*)(*(char**)((char*)self + 0x74) + 0x50) = sVec3K19BD48(1.0f, -fillRatio, 1.0f);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BE80);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014B560(void* iface, int a1, int a2, int a3, int a4);

/**
 * Buys the selected item: stores the result of func_0014B560 (BE interface 9, the user, the character, the item's id,
 * 1) as the money (+0xC0).
 *
 * cFEStateCharEquip_onPopupEvent calls it when the buy popup closes with a yes.
 */
extern "C" void func_0019BE80(void* self)
{
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95);
    char* item = *(char**)((char*)self + (idx << 2) + 0x124);
    *(void**)((char*)self + 0xC0) = func_0014B560(cBE_getInterface_Fv(cBE_getBE(), 9),
        *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC), *(short*)(item + 4), 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BEE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0014AFB0(void* self, int a1, int a2, int id, int locked);
extern "C" void func_0014AEA8(void* self, int a1, int a2);

struct sItemK19BEE8 {
    short id;
    unsigned short flags;
};
struct sItemListK19BEE8 {
    char pad_0x0[0x288];
    short* map;                 // 0x288
    int count;                  // 0x28C
    sItemK19BEE8 items[1];      // 0x290
};

static inline sItemK19BEE8* getItemK19BEE8(sItemListK19BEE8* l, int id)
{
    int idx = l->map[id];
    if (idx >= 0)
        return &l->items[idx];
    return 0;
}

/**
 * Equips or takes off the selected inventory item.
 *
 * For a selected item (flag 4) toggles its equipped flag (entry flag 4) with func_0014AFB0 and, on success, calls
 * func_0014AEA8 for the character.
 *
 * @return 1 when the item changed, else 0
 */
extern "C" int func_0019BEE8(void* self)
{
    if (*(int*)((char*)self + 0x958) <= 0)
        return 0;
    char* item = *(char**)((char*)self + (*(unsigned char*)(*(char**)((char*)self + 0x5C) + 0x95) << 2) + 0x124);
    if (!(*(int*)(item + 0x34) & 4))
        return 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    sItemListK19BEE8* list = (sItemListK19BEE8*)func_0014AD28(iface, *(signed char*)((char*)self + 0x44),
                                                              *(int*)((char*)self + 0xBC));
    sItemK19BEE8* entry = getItemK19BEE8(list, *(short*)(item + 4));
    int mask = 4;
    if (func_0014AFB0(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC), *(short*)(item + 4),
                      (entry->flags & mask) == 0)) {
        func_0014AEA8(iface, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0xBC));
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019BFE0);
#ifdef SKIP_ASM
/**
 * Does nothing.
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
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
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
extern "C" void cFEStateCharEquip_update3DPositions(void* vself)
{
    char* self = (char*)vself;
    char* rider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, *(signed char*)(self + 0x44));
    int ok = 0;
    if (*(unsigned int*)rider < 10 && *(int*)(rider + 0xCB8) != 0) {
        ok = *(int*)(rider + 0xCB4) != 0;
    }
    if (ok) {
        sXform_BFE8 xf;
        float zero;
        float zoom = *(float*)(self + 0xAA4);
        float zOff = *(float*)(self + 0xAA8) * zoom * 65.0f;
        float yOff = zoom * 320.0f;
        float yaw = *(float*)(self + 0xAAC);
        int state = *(int*)(self + 0xAE0);
        if (state == 1) {
            sQuat_BFE8 point;
            point.w = 1.0f;
            point.x = -105.0f;
            point.y = yOff + -100.0f;
            point.z = zOff + -80.0f;
            *(sQuat_BFE8*)(self + 0xAC0) = point;
            point.x = 265.75f;
            point.y = -1073.5f;
            point.z = -232.0f;
            *(sQuat_BFE8*)(self + 0xAD0) = point;
        } else if (state == 2) {
            sQuat_BFE8 point;
            point.x = -250.0f;
            point.y = -650.0f;
            point.z = -215.0f;
            point.w = 1.0f;
            *(sQuat_BFE8*)(self + 0xAC0) = point;
            point.x = -100.0f;
            point.y = 45.0f;
            point.z = 15.0f;
            *(sQuat_BFE8*)(self + 0xAD0) = point;
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
            sQuat_BFE8 dPos = VecSub_BFE8(pos, *(sQuat_BFE8*)(self + 0xAC0));
            sQuat_BFE8 dAt = VecSub_BFE8(lookAt, *(sQuat_BFE8*)(self + 0xAD0));
            VecAdd_BFE8(*(sQuat_BFE8*)(self + 0xAC0), *(sQuat_BFE8*)(self + 0xAC0), VecScale_BFE8(dPos, 0.2f));
            VecAdd_BFE8(*(sQuat_BFE8*)(self + 0xAD0), *(sQuat_BFE8*)(self + 0xAD0), VecScale_BFE8(dAt, 0.2f));
            if (VecDot_BFE8(dAt, dAt) < 0.2f) {
                if (self) { // FAKEMATCH: identical arms, see header
                    *(int*)(self + 0xAE0) = 2;
                } else {
                    *(int*)(self + 0xAE0) = 2;
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
            sQuat_BFE8 dPos = VecSub_BFE8(pos, *(sQuat_BFE8*)(self + 0xAC0));
            sQuat_BFE8 dAt = VecSub_BFE8(lookAt, *(sQuat_BFE8*)(self + 0xAD0));
            VecAdd_BFE8(*(sQuat_BFE8*)(self + 0xAC0), *(sQuat_BFE8*)(self + 0xAC0), VecScale_BFE8(dPos, 0.2f));
            VecAdd_BFE8(*(sQuat_BFE8*)(self + 0xAD0), *(sQuat_BFE8*)(self + 0xAD0), VecScale_BFE8(dAt, 0.2f));
            if (VecDot_BFE8(dPos, dPos) < 0.2f) {
                *(int*)(self + 0xAE0) = 1;
            }
        }
        xf.rot.x = 0.0f;
        xf.rot.w = 1.0f;
        xf.rot.y = 0.0f;
        xf.rot.z = 0.0f;
        xf.pos = *(sQuat_BFE8*)(self + 0xAC0);
        zero = 0.0f;

        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_XAxis, zero));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_YAxis, zero));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_ZAxis, (yaw + 80.0f) * 0.01745329424738884f));
        cFERider_setPose(rider, &xf);
        xf.pos = *(sQuat_BFE8*)(self + 0xAD0);
        xf.rot.w = 1.0f;
        xf.rot.x = zero;
        xf.rot.y = zero;
        xf.rot.z = zero;
        *(float*)(self + 0xAB0) += 1.0f;
        if (*(float*)(self + 0xAB0) >= 360.0f) {
            *(float*)(self + 0xAB0) -= 360.0f;
        }
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_ZAxis, 245.0f * 0.01745329424738884f));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_YAxis, 320.0f * 0.01745329424738884f));
        xf.rot = QuatMul_BFE8(xf.rot, AxisAngle_BFE8(t4vect_XAxis, (*(float*)(self + 0xAB0) + 90.0f) * 0.01745329424738884f));
        cFERider_setBoardPose(rider, &xf);
    }
    char* cam = (char*)SSXApp->RealFE;
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
    cSimpleCamera_setupCamWithPosAndTarget(cam + 0x10, &pos, &lookAt);
    *(float*)(cam + 0x10) = 0.4363323450088501f;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C7E8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* self, void* engine, void* owner, signed char idx);
extern "C" void cBuyPopupInfo_initBuyBolt(int* self, int a1, int a2, int a3);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cUIStateBuyPopup[];

/**
 * Opens the buy popup for an item.
 *
 * Constructs a cUIStateBuyPopup (0x70 bytes) for the user, sets it up for the item (cBuyPopupInfo_initBuyBolt with the
 * item's id +0x14, the price +0xE x 10 and the money +0xC0) and pushes it on the state stack.
 *
 * @param item  the item
 */
extern "C" void func_0019C7E8(void* self, void* item)
{
    void* popup = cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_cUIStateBuyPopup, 0, 0), *(void**)((char*)self + 0x10), self, *(signed char*)((char*)self + 0x44));
    int price = *(short*)((char*)item + 0xE);
    int id = *(int*)((char*)item + 0x14);
    if (price > 0) {
        price *= 10;
    }
    cBuyPopupInfo_initBuyBolt((int*)((char*)popup + 0x48), id, price, *(int*)((char*)self + 0xC0));
    cUIStateStack_pushPopup(*(char**)((char*)self + 0x10) + 0x18, popup);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C880);
#ifdef SKIP_ASM
extern "C" void* func_0019C8F8(void* self, void* item);

/**
 * Takes the first node off the node pool's free list (+0x1F40) and returns it.
 *
 * The next node (func_0019C8F8) becomes the head with its previous index (+0xC) cleared; the taken node's next index
 * (+0xE) is set to -1. Likely bug: an empty free list is not checked, so the null head is written through.
 *
 * @return the node
 */
extern "C" void* func_0019C880(void* self)
{
    char* item = *(char**)((char*)self + 0x1F40);
    *(void**)((char*)self + 0x1F40) = func_0019C8F8(self, item);
    *(short*)(item + 0xE) = -1;
    char* head = *(char**)((char*)self + 0x1F40);
    if (head != 0) {
        *(short*)(head + 0xC) = -1;
    }
    return item;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C8D0);
#ifdef SKIP_ASM
/**
 * Returns the node before @p item: the pool's 16-byte node at index +0xC of @p item.
 *
 * @param item  the node, or null
 * @return the previous node, or null when there is none
 */
extern "C" void* func_0019C8D0(void* self, void* item)
{
    if (item != 0) {
        short idx = *(short*)((char*)item + 0xc);
        if (idx >= 0) {
            return (char*)self + (idx << 4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C8F8);
#ifdef SKIP_ASM
/**
 * Returns the node after @p item: the pool's 16-byte node at index +0xE of @p item.
 *
 * @param item  the node, or null
 * @return the next node, or null when there is none
 */
extern "C" void* func_0019C8F8(void* self, void* item)
{
    if (item != 0) {
        short idx = *(short*)((char*)item + 0xe);
        if (idx >= 0) {
            return (char*)self + (idx << 4);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C920);
#ifdef SKIP_ASM
/**
 * Returns the pool index of the node at @p nodeAddr ((address - pool) / 16).
 *
 * @param nodeAddr  the node's address, or 0
 * @return the index, or -1 for 0
 */
extern "C" int func_0019C920(void* self, int nodeAddr)
{
    int diff = nodeAddr - (int)self;
    if (nodeAddr != 0) return diff >> 4;
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019C938);
#ifdef SKIP_ASM
struct sNodeK19C938 {
    int key;
    short f4;
    short f6;
    short f8;
    short fA;
    short prev;   // 0xC
    short next;   // 0xE
};

extern "C" void* func_0019C880(void* self);
extern "C" void* func_0019C8F8(void* self, void* item);
// PORT: func_0019C920 takes the node pointer as an int.
extern "C" int func_0019C920(void* self, int a1);

/**
 * Finds the node for a two-part key in the file-index hash table, optionally creating it.
 *
 * The table (allocated as "BoltFileIndexManager", 8004 bytes; D_004A1968) holds 400 16-byte nodes at +0x0, 400 bucket
 * head pointers at +0x1900 and the free-list head at +0x1F40. The key is (@p hi << 16) | @p lo and its bucket is key %
 * 400. Walks the bucket's chain (next index at node +0xE, via func_0019C8F8) for a node with that key. When the chain
 * ends and @p create is set, takes a node from the free list (func_0019C880) and appends it: its prev index (+0xC) is
 * the old tail's index (func_0019C920) or -1 when it becomes the bucket head, and its next index is -1. Whatever node
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
extern "C" void* func_0019C938(void* self, int hi, int lo, int create)
{
    int key = (hi << 16) | lo;
    int bucket = key % 400;
    sNodeK19C938* prev = 0;
    sNodeK19C938* n = *(sNodeK19C938**)((char*)self + (bucket << 2) + 0x1900);
    while (*(void**)((char*)self + 0x1F40) != 0 || n != 0) {
        if (n == 0) {
            if (create) {
                n = (sNodeK19C938*)func_0019C880(self);
                n->prev = func_0019C920(self, (int)prev);
                if (prev != 0) {
                    n->prev = func_0019C920(self, (int)prev);
                    prev->next = func_0019C920(self, (int)n);
                } else {
                    n->prev = -1;
                    *(sNodeK19C938**)((char*)self + (bucket << 2) + 0x1900) = n;
                }
                n->next = -1;
            }
            break;
        }
        if (n->key == key)
            break;
        prev = n;
        n = (sNodeK19C938*)func_0019C8F8(self, prev);
    }
    if (n != 0)
        n->key = key;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CA70);
#ifdef SKIP_ASM
struct sNode_CA70 {
    int key;
    short f4;
    short f6;
    short f8;
    short fA;
    short prev;
    short next;
};
extern "C" void* func_0019C8D0(void* self, void* item);
extern "C" void* func_0019C8F8(void* self, void* item);
extern "C" int func_0019C920(void* self, int a1);

/**
 * Unlinks a node from its hash bucket in the file-index table and returns it to the free list.
 *
 * Does nothing for a null @p nodep. Otherwise the bucket (key % 400) head at +0x1900 or the previous node's next index
 * takes the node's next, the next node's prev index takes the node's prev, the node's key and both values (+0x8, +0xA)
 * and prev index become -1, and the node is pushed onto the free list at +0x1F40 (its next index is the old head's
 * index, func_0019C920).
 *
 * @param nodep  the node to release (from func_0019C938), or 0
 */
extern "C" void func_0019CA70(void* self, void* nodep)
{
    sNode_CA70* node = (sNode_CA70*)nodep;
    if (node != 0) {
        int bucketIndex = node->key % 400;
        if (node->prev < 0) {
            *(void**)((char*)self + (bucketIndex << 2) + 0x1900) = func_0019C8F8(self, node);
        } else {
            ((sNode_CA70*)func_0019C8D0(self, node))->next = node->next;
        }
        if (node->next >= 0) {
            ((sNode_CA70*)func_0019C8F8(self, node))->prev = node->prev;
        }
        node->key = -1;
        node->fA = -1;
        node->f8 = -1;
        node->prev = -1;
        // PORT: free-list head pointer passed as int (unit declares func_0019C920(void*, int))
        node->next = func_0019C920(self, *(int*)((char*)self + 0x1F40));
        *(sNode_CA70**)((char*)self + 0x1F40) = node;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CB38);
#ifdef SKIP_ASM
extern "C" void func_0019CB60(void* self);

/**
 * Constructs the file-index hash table by resetting it with func_0019CB60.
 *
 * @return @p self
 */
extern "C" void* func_0019CB38(void* self)
{
    func_0019CB60(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CB60);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, unsigned int value, int size);

struct sNode0019CB60 {
    int field_0x0;
    short field_0x4;
    short field_0x6;
    int field_0x8;
    short prev;
    short next;
};

/**
 * Resets the file-index hash table to 400 free nodes and empty buckets.
 *
 * Fills the 400 16-byte nodes (6400 bytes) with 0xFF, clears the 400 bucket heads at +0x1900, points the free-list head
 * (+0x1F40) at node 0 and chains every node to its neighbours (prev i - 1, next i + 1, the last next -1) with both mask
 * words (+0x4, +0x6) cleared.
 */
extern "C" void func_0019CB60(void* self)
{
    MEM_fill(self, 0xFFFFFFFF, 6400);
    MEM_fill((char*)self + 0x1900, 0, 1600);
    *(void**)((char*)self + 0x1F40) = self;
    sNode0019CB60* n = (sNode0019CB60*)self;
    for (int i = 0; i < 400; i++) {
        n[i].next = i + 1;
        n[i].prev = i - 1;
        n[i].field_0x4 = 0;
        n[i].field_0x6 = 0;
    }
    n[399].next = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CBE0);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

/**
 * Returns the second value stored for a key in the file-index hash table, or -1 when it is not set.
 *
 * Looks the node up without creating it (func_0019C938) and returns its value at +0xA when bit @p bit of its mask at
 * +0x6 is set. The rider manager stores, per profile index and item id, the FE file cache slot of the item's type-8
 * file here (a func_0019DA10 result).
 *
 * @param bit  high half of the key and the mask bit to test (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @return the stored value, or -1
 */
extern "C" int func_0019CBE0(void* self, int bit, int lowKey)
{
    void* node = func_0019C938(self, bit, lowKey, 0);
    if (node != 0) {
        if ((*(short*)((char*)node + 6) >> bit) & 1) {
            return *(short*)((char*)node + 0xA);
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CC30);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

extern "C" void func_0019CA70(void* self, void* node);
struct sNode_CC30 {
    int key;
    short f4;
    short mask;
    short f8;
    short val;
    short prev;
    short next;
};

/**
 * Stores or clears the second value for a key in the file-index hash table.
 *
 * A @p val of -1 looks the node up without creating it; any other value creates it if needed (func_0019C938). A
 * negative @p val clears bit @p bit of the mask at +0x6 and, once that mask is 0 and the first value (+0x8) is
 * negative, frees the node (func_0019CA70). Otherwise the value goes to +0xA and the mask bit is set.
 *
 * Quirk: the free test checks the first value rather than the first mask, and +0x8 is only reset to -1 when a node is
 * freed, so a node whose first value was ever set stays allocated after both masks clear.
 *
 * @param bit  high half of the key and the mask bit (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @param val  value to store, or a negative value to clear it
 */
extern "C" void func_0019CC30(void* self, int bit, int lowKey, int val)
{
    sNode_CC30* node = (sNode_CC30*)func_0019C938(self, bit, lowKey, val != -1);
    if (node != 0) {
        if (val < 0) {
            if ((node->mask >> bit) & 1) {
                node->mask &= ~(1 << bit);
                if (node->mask == 0 && node->f8 < 0) {
                    func_0019CA70(self, node);
                }
            }
        } else {
            node->val = val;
            node->mask |= 1 << bit;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CCE8);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);

/**
 * Returns the first value stored for a key in the file-index hash table, or -1 when it is not set.
 *
 * Looks the node up without creating it (func_0019C938) and returns its value at +0x8 when bit @p bit of its mask at
 * +0x4 is set. The rider manager stores, per profile index and item id, the FE file cache slot of the item's type-11
 * file here (a func_0019DA10 result).
 *
 * @param bit  high half of the key and the mask bit to test (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @return the stored value, or -1
 */
extern "C" int func_0019CCE8(void* self, int bit, int lowKey)
{
    void* node = func_0019C938(self, bit, lowKey, 0);
    if (node != 0) {
        if ((*(short*)((char*)node + 4) >> bit) & 1) {
            return *(short*)((char*)node + 0x8);
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CD38);
#ifdef SKIP_ASM
extern "C" void* func_0019C938(void* self, int a1, int a2, int a3);
extern "C" void func_0019CA70(void* self, void* node);
struct sNode_CD38 {
    int key;
    short mask;
    short f6;
    short val;
    short fA;
    short prev;
    short next;
};

/**
 * Stores or clears the first value for a key in the file-index hash table.
 *
 * A @p val of -1 looks the node up without creating it; any other value creates it if needed (func_0019C938). A
 * negative @p val clears bit @p bit of the mask at +0x4 and, once that mask is 0 and the second value (+0xA) is
 * negative, frees the node (func_0019CA70). Otherwise the value goes to +0x8 and the mask bit is set. Same quirk as
 * func_0019CC30: the free test checks the other value, not the other mask.
 *
 * @param bit  high half of the key and the mask bit (the rider manager passes the rider's profile index, +0x4)
 * @param lowKey  low half of the key (an item id)
 * @param val  value to store, or a negative value to clear it
 */
extern "C" void func_0019CD38(void* self, int bit, int lowKey, int val)
{
    sNode_CD38* node = (sNode_CD38*)func_0019C938(self, bit, lowKey, val != -1);
    if (node != 0) {
        if (val < 0) {
            if ((node->mask >> bit) & 1) {
                node->mask &= ~(1 << bit);
                if (node->mask == 0 && node->fA < 0) {
                    func_0019CA70(self, node);
                }
            }
        } else {
            node->val = val;
            node->mask |= 1 << bit;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CDF0);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" void func_0019CE68(void* self);

/**
 * Constructs the FE file cache: records it in D_004A196C and initialises it with func_0019CE68.
 *
 * cFE_cFE builds it at the front end's +0x1A70. D_004A196C is what the sort comparators func_0019DD10 and func_0019DE18
 * read.
 *
 * @return @p self
 */
extern "C" void* func_0019CDF0(void* self)
{
    D_004A196C = self;
    func_0019CE68(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CE20);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" void func_0019CF40(void* self);
void operator_delete(int* p);

/**
 * Destroys the FE file cache: clears D_004A196C, purges every slot (func_0019CF40) and frees the object when bit 0 of
 * @p flags is set.
 *
 * @param flags  deleting-destructor flags; bit 0 frees the object
 */
extern "C" void func_0019CE20(void* self, int flags)
{
    D_004A196C = 0;
    func_0019CF40(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CE68);
#ifdef SKIP_ASM
extern void* D_004A1968;
extern char str_BoltFileIndexManager[];

struct sEquipEntryK19CE68 {
    int f0;                // 0x0
    char b4;               // 0x4
    char pad_0x005[0x104 - 0x5];
    int f104;              // 0x104
    int f108;              // 0x108
    int pad_0x10C;
    int f110;              // 0x110
    int f114;              // 0x114
    int f118;              // 0x118
    int pad_0x11C;
};
struct sEquipSlotK19CE68 {
    sEquipEntryK19CE68 e[256];
};
struct sEquipTableK19CE68 {
    sEquipSlotK19CE68 slots[10]; // 0x0
    int fB4000;                  // 0xB4000
    int fB4004;                  // 0xB4004
    int counts[10];              // 0xB4008
};

/**
 * Initialises the FE file cache and its file-index hash table.
 *
 * For each of the 10 banks it clears all 256 slots (state, name, data, handle, size and use count `timer` to 0, type
 * -1) and sets the bank's stamp counter (counts) to 1; then clears used and nPending. The file-index table D_004A1968
 * is allocated on first use (cMemMan_alloc, 8004 bytes, tag "BoltFileIndexManager", built by func_0019CB38), otherwise
 * reset with func_0019CB60. cRealFE_load calls it on the front end's +0x1A70.
 */
extern "C" void func_0019CE68(void* self)
{
    sEquipTableK19CE68* cache = (sEquipTableK19CE68*)self;
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 256; j++) {
            sEquipEntryK19CE68* slot = &cache->slots[i].e[j];
            slot->f0 = 0;
            slot->b4 = 0;
            slot->f104 = 0;
            slot->f118 = 0;
            slot->f108 = 0;
            slot->f114 = 0;
            slot->f110 = -1;
        }
        cache->counts[i] = 1;
    }
    cache->fB4004 = 0;
    cache->fB4000 = 0;
    if (D_004A1968 == 0) {
        D_004A1968 = func_0019CB38(cMemMan_alloc(8004, str_BoltFileIndexManager, 0, 0));
    } else {
        func_0019CB60(D_004A1968);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019CF40);
#ifdef SKIP_ASM
struct sEquipTable_D4B8;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);
void operator_delete(int* p);
extern void* D_004A1968;

struct sEquipTableK19CF40 {
    char slots[0xB4008];
    int counts[10];    // 0xB4008
};

/**
 * Empties the FE file cache and frees its file-index hash table.
 *
 * Releases every slot of the 10 banks with func_0019D4B8 and resets each bank's stamp counter to 1, then deletes
 * D_004A1968 (operator_delete, no destructor call) and clears it. nPending is not reset here. Called by the cache's
 * destructor and by cRealFE_purge.
 */
extern "C" void func_0019CF40(void* self)
{
    sEquipTableK19CF40* cache = (sEquipTableK19CF40*)self;
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 256; j++) {
            func_0019D4B8((sEquipTable_D4B8*)self, i, j);
        }
        cache->counts[i] = 1;
    }
    if (D_004A1968 != 0) {
        operator_delete((int*)D_004A1968);
    }
    D_004A1968 = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D000);
#ifdef SKIP_ASM
struct sEquipTable_DC20;
struct sEquipSlot_DC20;
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* ptr);
extern "C" int unpacksizez(void* data);
extern "C" void unpack(void* src, void* dst);
extern "C" void func_0019D578(sEquipTable_DC20* self, int bank, int size);
extern "C" void func_0019D738(sEquipTable_DC20* self, int bank, int size);
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern char str_UncompressedData[];
extern char cMemMan_m_mutex[];

struct sSlotK19D000 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
};

/**
 * Unpacks a slot's compressed file data in place in the FE file cache.
 *
 * Does nothing when @p rawSlot or its data is null or unpacksizez() reports no unpacked size. Otherwise it makes room
 * for the unpacked size (func_0019D578), allocates it (operator new, tag "UncompressedData", flags 0x60000000), and
 * while that fails it locks and unlocks cMemMan_m_mutex, evicts that many bytes (func_0019D738) and retries. It then
 * unpacks into the new block, frees the packed data, stores the new block as the slot's data and adjusts used by the
 * size difference. Called when an asynchronous load completes (func_0019D250, func_0019DC20).
 *
 * @param bank  the slot's bank (0-9), passed to the eviction helpers
 * @param rawSlot  the slot
 */
extern "C" void func_0019D000(sEquipTable_DC20* self, int bank, sEquipSlot_DC20* rawSlot)
{
    sSlotK19D000* slot = (sSlotK19D000*)rawSlot;
    if (slot == 0)
        return;
    if (slot->data == 0)
        return;
    int size = unpacksizez(slot->data);
    if (size <= 0)
        return;
    func_0019D578(self, bank, size);
    void* buf = operator_new_tag(size, str_UncompressedData, 0x60000000, 0);
    while (buf == 0) {
        MUTEX_lock(cMemMan_m_mutex);
        MUTEX_unlock(cMemMan_m_mutex);
        func_0019D738(self, bank, size);
        buf = operator_new_tag(size, str_UncompressedData, 0x60000000, 0);
    }
    unpack(slot->data, buf);
    if (slot->data != 0)
        cMemMan_free(slot->data);
    slot->data = buf;
    *(int*)((char*)self + 0xB4000) += size - slot->size;
    slot->size = size;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D140);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sEquipTable_DC20;
extern "C" void func_0019D8B8(sEquipTable_DC20* self, int bank, int i);
struct sEquipSlot_D140 {
    int state;              // 0x000
    char pad_0x4[0x118];
    unsigned int stamp;     // 0x11C
};

/**
 * Starts the oldest queued load, preferring the given bank of the FE file cache.
 *
 * Picks the queued (state 3) slot with the lowest stamp in @p bank; when the bank has none, it searches the other banks
 * in order and takes the lowest-stamp queued slot of the first bank that has one. The chosen slot is loaded with
 * func_0019D8B8.
 *
 * Quirk: a best stamp of 0 counts as "nothing chosen yet", so a queued slot with stamp 0 loses to any later queued
 * slot.
 *
 * @param bank  the bank to try first
 */
extern "C" void func_0019D140(void* self, int bank)
{
    unsigned int best = 0;
    int bestIdx = -1;
    int found;
    int i;
    sEquipSlot_D140* slot = (sEquipSlot_D140*)((char*)self + bank * 0x12000);
    found = bank;
    for (i = 0; i < 256; i++, slot++) {
        if (slot->state == 3 && (best == 0 || slot->stamp < best)) {
            best = slot->stamp;
            bestIdx = i;
        }
    }
    if (bestIdx == -1) {
        int otherBank;
        for (otherBank = 0; otherBank < 10; otherBank++) {
            if (otherBank == bank) {
                continue;
            }
            slot = (sEquipSlot_D140*)((char*)self + otherBank * 0x12000);
            for (i = 0; i < 256; i++, slot++) {
                if (slot->state == 3 && (best == 0 || slot->stamp < best)) {
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
        func_0019D8B8((sEquipTable_DC20*)self, found, bestIdx);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D250);
#ifdef SKIP_ASM
struct sEquipTable_DC20;
struct sEquipSlot_DC20;
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" void func_0019D000(sEquipTable_DC20* self, int bank, sEquipSlot_DC20* slot);
extern "C" void func_0019D140(void* self, int bank);
extern "C" void func_0019D8B8(sEquipTable_DC20* self, int bank, int i);

struct sSlotK19D250 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    unsigned int stamp; // 0x11C
};
struct sBankK19D250 {
    sSlotK19D250 e[256];
};
struct sTableK19D250 {
    sBankK19D250 banks[10];      // 0x0
    int fB4000;                  // 0xB4000
    int nPending;                // 0xB4004
};

/**
 * Polls the FE file cache's asynchronous loads once per frame (cRealFE_update).
 *
 * For every slot of the 10 banks: an asynchronous load in flight (pending set, state 2) whose ASYNCFILE_getstatus()
 * reports 1 is released (ASYNCFILE_release), unpacked (func_0019D000) and becomes loaded (state 1) or idle (state 0)
 * when its use count is 0; pending is cleared and nPending decremented, and when no loads remain in flight the next
 * queued load starts (func_0019D140 for that bank). A queued slot (state 3) starts loading (func_0019D8B8) whenever no
 * loads are in flight.
 *
 * @param table  the FE file cache
 */
extern "C" void func_0019D250(void* table)
{
    sTableK19D250* self = (sTableK19D250*)table;
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        sSlotK19D250* slot = (sSlotK19D250*)((char*)self + i * 0x12000);
        for (j = 0; j < 256; j++, slot++) {
            if (slot->pending != 0 && slot->state == 2) {
                int asyncStatus = ASYNCFILE_getstatus(slot->handle);
                if (asyncStatus == 1) {
                    int status[4];
                    ASYNCFILE_release(slot->handle, 0, status);
                    func_0019D000((sEquipTable_DC20*)self, i, (sEquipSlot_DC20*)slot);
                    if (slot->timer == 0)
                        slot->state = 0;
                    else
                        slot->state = asyncStatus;
                    slot->pending = 0;
                    if (--self->nPending <= 0)
                        func_0019D140(self, i);
                }
            } else if (self->nPending <= 0 && slot->state == 3) {
                func_0019D8B8((sEquipTable_DC20*)self, i, j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D3B0);
#ifdef SKIP_ASM
struct sEquipSlot_D3B0 {
    int used;               // 0x000
    char pad_0x4[0x100];
    int valid;              // 0x104
    char pad_0x108[0x14];
    unsigned int stamp;     // 0x11C
};

/**
 * Picks the slot to reuse for a new file in a bank of the FE file cache.
 *
 * Returns the first idle slot (state 0) that holds no data; otherwise the idle slot with the oldest stamp (least
 * recently used).
 *
 * @param bank  the bank (0-9)
 * @return the slot index, or -1 when no slot of the bank is idle
 */
extern "C" int func_0019D3B0(void* self, int bank)
{
    unsigned int best = 0;
    int bestIdx = -1;
    sEquipSlot_D3B0* slot = (sEquipSlot_D3B0*)((char*)self + bank * 0x12000);
    for (int i = 0; i < 256; i++, slot++) {
        if (slot->used == 0) {
            if (slot->valid == 0) {
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
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D428);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sEquipSlot_D428 {
    int state;
    char name[0x100];
    int value;
    char pad108[0x18];
};
struct sEquipRow_D428 {
    sEquipSlot_D428 slots[0x100];
};
struct sEquipTable_D428 {
    sEquipRow_D428 rows[1];
};

/**
 * Finds the slot holding a file name in a bank of the FE file cache.
 *
 * Compares @p name with every non-empty slot name using func_004165A8 (a strcmp routine).
 *
 * @param bank  the bank (0-9)
 * @param name  the file name
 * @return the slot index, or -1 when the name is not cached
 */
extern "C" int func_0019D428(sEquipTable_D428* self, int bank, const char* name)
{
    for (int i = 0; i < 256; i++) {
        sEquipSlot_D428* slot = &self->rows[bank].slots[i];
        if (slot->name[0] != 0 && func_004165A8(self->rows[bank].slots[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D4B8);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
void cMemMan_free(void* ptr);
struct sEquipSlot_D4B8 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int f114;
    int size;       // 0x118
    int f11C;
};
struct sEquipRow_D4B8 {
    sEquipSlot_D4B8 slots[0x100];
};
struct sEquipTable_D4B8 {
    sEquipRow_D4B8 rows[1];
};

/**
 * Frees one slot of the FE file cache.
 *
 * When an asynchronous load is in flight (state 2, pending) it releases the request (ASYNCFILE_release). Subtracts the
 * slot's size from used, frees its data and clears data, size, state, name, handle, pending and use count; the type
 * becomes -1. The stamp is kept and nPending is not adjusted.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 */
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i)
{
    int status;
    sEquipSlot_D4B8* slot = &self->rows[bank].slots[i];
    if (slot->state == 2) {
        if (slot->pending != 0) {
            ASYNCFILE_release(slot->handle, 0, &status);
        }
    }
    *(int*)((char*)self + 0xB4000) -= slot->size;
    if (slot->data != 0) {
        cMemMan_free(slot->data);
    }
    slot->data = 0;
    slot->size = 0;
    slot->state = 0;
    slot->name[0] = 0;
    slot->handle = 0;
    slot->pending = 0;
    slot->f114 = 0;
    slot->f110 = -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D578);
#ifdef SKIP_ASM
struct sEquipTable_D4B8;
struct sEquipTable_DC20;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);

struct sSlotK19D578 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    unsigned int stamp; // 0x11C
};
struct sBankK19D578 {
    sSlotK19D578 e[256];
};
struct sTableK19D578 {
    sBankK19D578 banks[10];      // 0x0
    int used;                    // 0xB4000
};

/**
 * Evicts idle cached files until a new file fits in the FE file cache's 9,999,999-byte budget.
 *
 * Returns at once when used plus @p size fits. Otherwise frees idle slots (data present, use count 0, state 0) of the
 * other banks in bank order, stopping as soon as it fits, then frees this bank's idle slots oldest stamp first until it
 * fits or none remain (func_0019D4B8).
 *
 * @param table  the FE file cache
 * @param bank  the bank being loaded into; its own files go last
 * @param size  bytes about to be added
 */
extern "C" void func_0019D578(sEquipTable_DC20* table, int bank, int size)
{
    sTableK19D578* self = (sTableK19D578*)table;
    if (self->used + size <= 9999999)
        return;
    int otherBank;
    int j;
    for (otherBank = 0; otherBank < 10; otherBank++) {
        if (otherBank == bank)
            continue;
        for (j = 0; j < 256; j++) {
            sSlotK19D578* slot = &self->banks[otherBank].e[j];
            if (slot->data != 0 && slot->timer == 0 && slot->state == 0) {
                func_0019D4B8((sEquipTable_D4B8*)self, otherBank, j);
                if (self->used + size <= 9999999)
                    return;
            }
        }
    }
    do {
        int best = -1;
        unsigned int stamp = 0;
        for (int k = 0; k < 256; k++) {
            sSlotK19D578* slot = &self->banks[bank].e[k];
            if (slot->data != 0 && slot->state == 0 && slot->timer == 0) {
                if (best < 0 || slot->stamp < stamp) {
                    stamp = slot->stamp;
                    best = k;
                }
            }
        }
        if (best < 0)
            return;
        func_0019D4B8((sEquipTable_D4B8*)self, bank, best);
    } while (self->used + size > 9999999);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D738);
#ifdef SKIP_ASM
struct sEquipTable_D4B8;
struct sEquipTable_DC20;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);

struct sSlotK19D738 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    unsigned int stamp; // 0x11C
};
struct sBankK19D738 {
    sSlotK19D738 e[256];
};
struct sTableK19D738 {
    sBankK19D738 banks[10];      // 0x0
};

/**
 * Evicts at least a given number of bytes of idle cached files from the FE file cache.
 *
 * Used after an allocation fails, regardless of the budget. Frees idle slots (data present, use count 0, state 0) of
 * the other banks in bank order until the freed sizes reach @p size, then this bank's idle slots oldest stamp first
 * until they do or none remain (func_0019D4B8).
 *
 * @param table  the FE file cache
 * @param bank  the bank being loaded into; its own files go last
 * @param size  bytes to free
 */
extern "C" void func_0019D738(sEquipTable_DC20* table, int bank, int size)
{
    sTableK19D738* self = (sTableK19D738*)table;
    int otherBank;
    int j;
    for (otherBank = 0; otherBank < 10; otherBank++) {
        if (otherBank == bank)
            continue;
        for (j = 0; j < 256; j++) {
            sSlotK19D738* slot = &self->banks[otherBank].e[j];
            if (slot->data != 0 && slot->timer == 0 && slot->state == 0) {
                size -= slot->size;
                func_0019D4B8((sEquipTable_D4B8*)self, otherBank, j);
                if (size <= 0)
                    return;
            }
        }
    }
    do {
        int best = -1;
        unsigned int stamp = 0;
        for (int k = 0; k < 256; k++) {
            sSlotK19D738* slot = &self->banks[bank].e[k];
            if (slot->data != 0 && slot->state == 0 && slot->timer == 0) {
                if (best < 0 || slot->stamp < stamp) {
                    stamp = slot->stamp;
                    best = k;
                }
            }
        }
        if (best < 0)
            return;
        size -= self->banks[bank].e[best].size;
        func_0019D4B8((sEquipTable_D4B8*)self, bank, best);
    } while (size > 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019D8B8);
#ifdef SKIP_ASM
struct sEquipTable_DC20;
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int BXFILE_sizez(const char* name);
extern "C" int func_003DF748(const char* name, void* data, int size);
extern "C" int FILE_loadatz(const char* name, void* data, int size);
extern "C" void func_0019D578(sEquipTable_DC20* self, int bank, int size);
extern "C" void func_0019D738(sEquipTable_DC20* self, int bank, int size);
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern char str_Bolt_FileData[];
extern char cMemMan_m_mutex[];

struct sSlotK19D8B8 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    unsigned int stamp; // 0x11C
};

/**
 * Starts loading a slot's file into the FE file cache.
 *
 * Reads the file size (BXFILE_sizez), makes room (func_0019D578) and allocates the data (operator new, tag "Bolt
 * FileData", flags 0x60000000), and while that fails it locks and unlocks cMemMan_m_mutex, evicts that many bytes
 * (func_0019D738) and retries; used grows by the size. With pending set the load is asynchronous: state 2, handle from
 * func_003DF748 (an async load request) and nPending incremented. Otherwise FILE_loadatz() loads it at once and the
 * state becomes 1 (loaded).
 *
 * Quirk: a synchronous load is not unpacked; only completed asynchronous loads go through func_0019D000.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 */
extern "C" void func_0019D8B8(sEquipTable_DC20* self, int bank, int i)
{
    sSlotK19D8B8* slot = &((sSlotK19D8B8*)((char*)self + bank * 0x12000))[i];
    int size = BXFILE_sizez(slot->name);
    func_0019D578(self, bank, size);
    slot->data = operator_new_tag(size, str_Bolt_FileData, 0x60000000, 0);
    while (slot->data == 0) {
        MUTEX_lock(cMemMan_m_mutex);
        MUTEX_unlock(cMemMan_m_mutex);
        func_0019D738(self, bank, size);
        slot->data = operator_new_tag(size, str_Bolt_FileData, 0x60000000, 0);
    }
    *(int*)((char*)self + 0xB4000) += size;
    if (slot->pending != 0) {
        slot->size = size;
        slot->state = 2;
        slot->handle = func_003DF748(slot->name, slot->data, size);
        *(int*)((char*)self + 0xB4004) += 1;
    } else {
        slot->size = size;
        FILE_loadatz(slot->name, slot->data, size);
        slot->state = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DA10);
#ifdef SKIP_ASM
struct sEquipTable_D4B8;
struct sEquipTable_DC20;
struct sEquipTable_D428;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);
extern "C" void func_0019D8B8(sEquipTable_DC20* self, int bank, int i);
extern "C" int func_0019D3B0(void* self, int bank);
extern "C" int func_0019D428(sEquipTable_D428* self, int bank, const char* name);
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" char* strcpy(char* dst, const char* src);

struct sSlotK19DA10 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    unsigned int stamp; // 0x11C
};
struct sBankK19DA10 {
    sSlotK19DA10 e[256];
};
struct sTableK19DA10 {
    sBankK19DA10 banks[10];      // 0x0
    int used;                    // 0xB4000
    int nPending;                // 0xB4004
    int counts[10];              // 0xB4008
};

/**
 * Requests a file from the FE file cache and returns its slot, adding one use.
 *
 * When the name is already cached (func_0019D428): the stamp becomes 0 with @p deferred or the bank's next stamp otherwise;
 * with @p async an idle slot becomes loaded (state 1); without it, pending is cleared, an in-flight load is waited on
 * (ASYNCFILE_release), a queued one is loaded at once (func_0019D8B8), and the slot is marked loaded with handle -1.
 * Otherwise it reuses a slot (func_0019D3B0, emptied with func_0019D4B8), stamps it the same way, copies the name and
 * stores @p fileType as the type and @p async as pending; it loads at once (func_0019D8B8) unless @p deferred is set or an
 * asynchronous request meets loads already in flight, in which case it is queued (state 3). Either way the use count
 * (timer) goes up by 1; func_0019DC20 gives the use back.
 *
 * Likely bug: the cached path that waits on an in-flight load neither unpacks it (func_0019D000) nor decrements
 * nPending, unlike func_0019DC20. Likely bug: a -1 from func_0019D3B0 (no idle slot) is used as a slot index.
 *
 * @param cache  the FE file cache
 * @param bank  the bank (0-9)
 * @param namep  the file name
 * @param fileType  the file type stored in the slot (the rider manager uses 8 and 11)
 * @param async  nonzero to load asynchronously
 * @param deferred  nonzero to defer the load: stamp 0 and queued, to be ordered later by func_0019DF20 or func_0019E070
 * @return the slot index
 */
extern "C" int func_0019DA10(void* cache, int bank, void* namep, int fileType, int async, int deferred)
{
    sTableK19DA10* self = (sTableK19DA10*)cache;
    const char* name = (const char*)namep;
    int i = func_0019D428((sEquipTable_D428*)self, bank, name);
    if (i >= 0) {
        sSlotK19DA10* slot = &self->banks[bank].e[i];
        if (deferred)
            slot->stamp = 0;
        else
            slot->stamp = self->counts[bank]++;
        if (async) {
            if (slot->state == 0)
                slot->state = 1;
        } else {
            slot->pending = 0;
            if (slot->state == 2) {
                int status;
                ASYNCFILE_release(slot->handle, 0, &status);
            } else if (slot->state == 3) {
                func_0019D8B8((sEquipTable_DC20*)self, bank, i);
            }
            slot->state = 1;
            slot->handle = -1;
        }
        slot->timer++;
    } else {
    i = func_0019D3B0(self, bank);
    sSlotK19DA10* slot = &self->banks[bank].e[i];
    func_0019D4B8((sEquipTable_D4B8*)self, bank, i);
    if (deferred)
        slot->stamp = 0;
    else
        slot->stamp = self->counts[bank]++;
    strcpy(slot->name, name);
    slot->timer++;
    slot->f110 = fileType;
    slot->pending = async;
    if (deferred == 0 && (async == 0 || self->nPending <= 0))
        func_0019D8B8((sEquipTable_DC20*)self, bank, i);
    else
        slot->state = 3;
    }
    return i;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DC20);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_release(int handle, int a1, void* status);
extern "C" int ASYNCFILE_getstatus(int handle);
struct sEquipSlot_DC20 {
    int state;
    char name[0x100];
    void* data;     // 0x104
    int handle;     // 0x108
    int pending;    // 0x10C
    int f110;
    int timer;      // 0x114
    int size;       // 0x118
    int f11C;
};
struct sEquipRow_DC20 {
    sEquipSlot_DC20 slots[0x100];
};
struct sEquipTable_DC20 {
    sEquipRow_DC20 rows[1];
};
extern "C" void func_0019D000(sEquipTable_DC20* self, int bank, sEquipSlot_DC20* slot);
struct sEquipTable_D4B8;
extern "C" void func_0019D4B8(sEquipTable_D4B8* self, int bank, int i);

/**
 * Drops one use of a slot in the FE file cache, keeping its data cached for reuse.
 *
 * Decrements the use count (timer) and returns while it is still positive. An asynchronous load still in flight
 * (ASYNCFILE_getstatus() not 1) is left to finish; a finished one is released, unpacked (func_0019D000) and counted out
 * of nPending. A slot without data is then freed (func_0019D4B8); one with data becomes idle (state 0), so a later
 * request can reuse it until it is evicted.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index (from func_0019DA10)
 */
extern "C" void func_0019DC20(sEquipTable_DC20* self, int bank, int i)
{
    int status;
    sEquipSlot_DC20* slot = &self->rows[bank].slots[i];
    if (--slot->timer > 0) {
        return;
    }
    if (slot->state == 2) {
        if (ASYNCFILE_getstatus(slot->handle) != 1) {
            return;
        }
        ASYNCFILE_release(slot->handle, 0, &status);
        func_0019D000(self, bank, slot);
        slot->pending = 0;
        slot->state = 0;
        *(int*)((char*)self + 0xB4004) -= 1;
    }
    if (slot->data == 0) {
        func_0019D4B8((sEquipTable_D4B8*)self, bank, i);
    } else {
        slot->state = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DD10);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" int strcasecmp(const char* a, const char* b);

struct sSlotK19DD10 {
    int state;
    char name[0x10C];
    int type;           // 0x110
    int f114;
    int f118;
    int f11C;           // 0x11C
};
struct sRowK19DD10 {
    sSlotK19DD10 slots[0x100];
};

/**
 * Compares two FE file cache slots to sort the deferred type-8 loads by name (qsort comparator).
 *
 * Each argument is a packed key (bank << 16 | slot) into the cache D_004A196C; both use the first key's bank. A slot
 * counts when it is queued (state 3), has type 8 and stamp 0. Two counted slots compare by name with strcasecmp; a
 * counted slot sorts before any other; two others compare equal.
 *
 * @param firstKey  first packed key
 * @param secondKey  second packed key
 * @return negative, 0 or positive in qsort fashion
 */
extern "C" int func_0019DD10(const int* firstKey, const int* secondKey)
{
    int ka = *firstKey;
    sRowK19DD10* row = (sRowK19DD10*)D_004A196C + (ka >> 16);
    sSlotK19DD10* sa = &row->slots[ka & 0xFFFF];
    sSlotK19DD10* sb = &row->slots[*secondKey & 0xFFFF];
    if (sa->state == 3 && sa->type == 8 && sa->f11C == 0) {
        if (sb->state == 3 && sb->type == 8 && sb->f11C == 0)
            return strcasecmp(sa->name, sb->name);
        return -1;
    }
    if (sb->state == 3 && sb->type == 8 && sb->f11C == 0) {
        if (sa->state == 3 && sa->type == 8 && sa->f11C == 0)
            return strcasecmp(sa->name, sb->name);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DE18);
#ifdef SKIP_ASM
extern void* D_004A196C;
extern "C" int strcasecmp(const char* a, const char* b);

struct sSlotK19DE18 {
    int state;
    char name[0x10C];
    int type;           // 0x110
    int f114;
    int f118;
    int f11C;           // 0x11C
};
struct sRowK19DE18 {
    sSlotK19DE18 slots[0x100];
};

/**
 * Compares two FE file cache slots to sort the deferred type-11 loads by name (qsort comparator).
 *
 * Same as func_0019DD10 for type 11: queued (state 3), type 11, stamp 0 slots sort first, by name with strcasecmp; two
 * others compare equal.
 *
 * @param firstKey  first packed key
 * @param secondKey  second packed key
 * @return negative, 0 or positive in qsort fashion
 */
extern "C" int func_0019DE18(const int* firstKey, const int* secondKey)
{
    int ka = *firstKey;
    sRowK19DE18* row = (sRowK19DE18*)D_004A196C + (ka >> 16);
    sSlotK19DE18* sa = &row->slots[ka & 0xFFFF];
    sSlotK19DE18* sb = &row->slots[*secondKey & 0xFFFF];
    if (sa->state == 3 && sa->type == 11 && sa->f11C == 0) {
        if (sb->state == 3 && sb->type == 11 && sb->f11C == 0)
            return strcasecmp(sa->name, sb->name);
        return -1;
    }
    if (sb->state == 3 && sb->type == 11 && sb->f11C == 0) {
        if (sa->state == 3 && sa->type == 11 && sa->f11C == 0)
            return strcasecmp(sa->name, sb->name);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019DF20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One file slot of the FE file cache (0x120 bytes; the cache sits at game + 0x7C -> + 0x1A70, D_004A196C points at
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

extern "C" void func_0019D8B8_DF20(sFileCache* self, int bank, int i) __asm__("func_0019D8B8");

// (prototype from their definition at line 2766)
extern "C" int func_0019DD10_DF20(const int* a, const int* b) __asm__("func_0019DD10");

extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const int*, const int*));

/**
 * Stamps a bank's deferred type-8 loads in name order and starts the first one.
 *
 * Sorts the bank's 256 packed keys with func_00418EF8 (qsort) and func_0019DD10, then walks them while the slot has
 * type 8 and stamp 0: each gets the bank's next stamp (counts) and, when no loads are in flight and it is queued,
 * starts loading (func_0019D8B8). The rider manager (func_0019FF00) calls it after requesting deferred files.
 *
 * @param bank  the bank (0-9)
 */
extern "C" void func_0019DF20(sFileCache* self, int bank)
{
    int keys[256];
    for (int i = 0; i < 256; i++)
        keys[i] = (bank << 16) | i;
    func_00418EF8(keys, 256, 4, func_0019DD10_DF20);
    for (int i = 0; i < 256; i++) {
        unsigned short idx = keys[i] & 0xFFFF;
        sFileSlot* slot = &self->banks[bank].slots[idx];
        if (slot->stamp != 0 || slot->type != 8)
            break;
        slot->stamp = self->counts[bank]++;
        if (self->nPending <= 0 && slot->state == 3)
            func_0019D8B8_DF20(self, bank, idx);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E070);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One file slot of the FE file cache (0x120 bytes; the cache sits at game + 0x7C -> + 0x1A70, D_004A196C points at
// it). state: 0 idle, 1 loaded, 2 async load in flight, 3 queued.
// 256 slots per bank (0x12000 bytes).
// The FE file cache: ten banks, the loaded byte count, the number of loads in flight and per-bank stamp counters.
extern "C" void func_0019D8B8_E070(sFileCache* self, int bank, int i) __asm__("func_0019D8B8");

// (prototype from their definition at line 2789)
extern "C" int func_0019DE18_E070(const int* a, const int* b) __asm__("func_0019DE18");

extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const int*, const int*));

/**
 * Stamps a bank's deferred type-11 loads in name order and starts the first one.
 *
 * Same as func_0019DF20 with func_0019DE18 and type 11. The rider manager (func_0019FBE0) calls it after requesting
 * deferred files.
 *
 * @param bank  the bank (0-9)
 */
extern "C" void func_0019E070(sFileCache* self, int bank)
{
    int keys[256];
    for (int i = 0; i < 256; i++)
        keys[i] = (bank << 16) | i;
    func_00418EF8(keys, 256, 4, func_0019DE18_E070);
    for (int i = 0; i < 256; i++) {
        unsigned short idx = keys[i] & 0xFFFF;
        sFileSlot* slot = &self->banks[bank].slots[idx];
        if (slot->stamp != 0 || slot->type != 11)
            break;
        slot->stamp = self->counts[bank]++;
        if (self->nPending <= 0 && slot->state == 3)
            func_0019D8B8_E070(self, bank, idx);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E1C0);
#ifdef SKIP_ASM
/**
 * Returns whether a bank of the FE file cache has no load in flight or queued.
 *
 * @param bank  the bank (0-9)
 * @return 0 when any slot's state is 2 or 3 (or negative), else 1
 */
extern "C" int func_0019E1C0(sTableK19D250* self, int bank) {
    for (int i = 0; i < 0x100; i++) {
        if ((unsigned int)self->banks[bank].e[i].state >= 2) return 0;
    }
    return 1;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E208);
#ifdef SKIP_ASM
/**
 * Returns whether a slot of the FE file cache is loading (state 2).
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return 1 while the load is in flight, else 0
 */
extern "C" int func_0019E208(sTableK19D250* self, int bank, int i) {
    return self->banks[bank].e[i].state == 2;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E238);
#ifdef SKIP_ASM
struct sEquipSlot120 {
    int state;
    char pad04[0x100];
    int value;
    char pad108[0x18];
};
struct sEquipRow12000 {
    sEquipSlot120 slots[0x100];
};
struct sEquipTable {
    sEquipRow12000 rows[1];
};

/**
 * Returns whether a slot of the FE file cache is loaded (state 1).
 *
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return 1 when loaded, else 0
 */
extern "C" int func_0019E238(sEquipTable* self, int bank, int slotIndex)
{
    return self->rows[bank].slots[slotIndex].state == 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E268);
#ifdef SKIP_ASM
/**
 * Returns a slot's file name in the FE file cache.
 *
 * Returns 0 only for state -1, which the cache's own functions never set.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the name buffer, or 0
 */
extern "C" char* func_0019E268(sTableK19D250* self, int bank, int i) {
    if (self->banks[bank].e[i].state == -1) return 0;
    return self->banks[bank].e[i].name;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E2B0);
#ifdef SKIP_ASM
/**
 * Returns a slot's file data in the FE file cache once it is loaded.
 *
 * The view calls the data pointer at +0x104 `value`; func_0019E2F0 is the same lookup typed as a pointer.
 *
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return the data pointer as an int when the state is 1, else 0
 */
extern "C" int func_0019E2B0(sEquipTable* self, int bank, int slotIndex)
{
    if (self->rows[bank].slots[slotIndex].state != 1) {
        return 0;
    }
    return self->rows[bank].slots[slotIndex].value;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E2F0);
#ifdef SKIP_ASM
/**
 * Returns a slot's file data in the FE file cache once it is loaded (state 1), else 0.
 *
 * Same as func_0019E2B0, typed as a pointer.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the data, or 0
 */
extern "C" void* func_0019E2F0(sTableK19D250* self, int bank, int i) {
    if (self->banks[bank].e[i].state != 1) return 0;
    return self->banks[bank].e[i].data;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E330);
#ifdef SKIP_ASM
/**
 * Detaches a slot's file data from the FE file cache and returns it.
 *
 * Clears the slot's data and size and subtracts the size from used (+0xB4000); state, name and use count stay. The
 * reward database (func_0015AEC0) takes the block this way and then drops its use with func_0019DC20, which frees the
 * now empty slot.
 *
 * @param base  the FE file cache
 * @param bank  the bank (0-9)
 * @param slotIndex  the slot index
 * @return the data pointer as an int (the caller now owns it)
 */
extern "C" int func_0019E330(char* base, int bank, int slotIndex) {
    char* pb = base + 0x104;
    char* qb = base + 0x118;
    int slotOffset = slotIndex * 288 + bank * 0x12000;
    int* dataField = (int*)(pb + slotOffset);
    int* sizeField = (int*)(qb + slotOffset);
    int data = *dataField;
    *dataField = 0;
    *(int*)(base + 0xB4000) -= *sizeField;
    *sizeField = 0;
    return data;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E380);
#ifdef SKIP_ASM
/**
 * Returns a slot's file type (+0x110, as passed to func_0019DA10) in the FE file cache.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the type
 */
extern "C" int func_0019E380(sTableK19D250* self, int bank, int i) {
    return self->banks[bank].e[i].f110;
}
#endif
//100%
INCLUDE_ASM("fe/festatecharequipdetail", func_0019E3A8);
#ifdef SKIP_ASM
/**
 * Returns a slot's use count (+0x114, `timer` in the views) in the FE file cache.
 *
 * @param bank  the bank (0-9)
 * @param i  the slot index
 * @return the use count
 */
extern "C" int func_0019E3A8(sTableK19D250* self, int bank, int i) {
    return self->banks[bank].e[i].timer;
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

struct sRiderSlotCE {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    sSlotEntryCE entries[256];  // 0x1C
};

/**
 * Constructs a front-end rider (0xCE0 bytes; the rider manager holds two).
 *
 * Sets the character id (+0x0), the profile index (+0x4), the requested character (+0xC24), the reload frame (+0xCD4)
 * and the 256 texture entries' ids (+0x1C, 8 bytes each) to -1 and their flags to 0; +0xC2C to 0xFFFFFFFF; both poses
 * (+0xC30, +0xC50) to tPose_Identity; and clears +0x8 to +0x18, +0xC1C, +0xC20, +0xC28, +0xC70 to +0xC78, the
 * load-stage flags +0xCB4 to +0xCC0, +0xCC4 (show once loaded), +0xCC8 (visible) and +0xCCC.
 *
 * @return @p self
 */
extern "C" void* cFERider_cFERider(sRiderSlotCE* self)
{
    int i;
    char* riderBytes = (char*)self;
    sSlotEntryCE* textureEntry;
    *(int*)(riderBytes + 0xC70) = 0;
    *(int*)(riderBytes + 0xC74) = 0;
    self->f0 = -1;
    self->f4 = -1;
    int none = -1;
    textureEntry = self->entries;
    self->f8 = 0;
    self->fC = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f18 = 0;
    for (i = 0; i < 256; i++) {
        textureEntry->flag = 0;
        textureEntry->id = none;
        textureEntry++;
    }
    *(int*)(riderBytes + 0xC1C) = 0;
    *(unsigned int*)(riderBytes + 0xC2C) = 0xFFFFFFFF;
    *(int*)(riderBytes + 0xC20) = 0;
    *(int*)(riderBytes + 0xC24) = -1;
    *(int*)(riderBytes + 0xC28) = 0;
    *(int*)(riderBytes + 0xCCC) = 0;
    *(sQuadPairCE*)(riderBytes + 0xC30) = tPose_Identity;
    *(sQuadPairCE*)(riderBytes + 0xC50) = tPose_Identity;
    *(int*)(riderBytes + 0xCD4) = -1;
    *(int*)(riderBytes + 0xC78) = 0;
    *(int*)(riderBytes + 0xCC4) = 0;
    *(int*)(riderBytes + 0xCC8) = 0;
    *(int*)(riderBytes + 0xCB4) = 0;
    *(int*)(riderBytes + 0xCB8) = 0;
    *(int*)(riderBytes + 0xCBC) = 0;
    *(int*)(riderBytes + 0xCC0) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider__cFERider);
#ifdef SKIP_ASM
struct sVEntry0019E498 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sSlot0019E498 {
    int id;
    char flag;
    char pad[7];
};

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
extern "C" void cFERider__cFERider(void* self, int flags)
{
    sSlot0019E498* slot = (sSlot0019E498*)((char*)self + 0x1C);
    cFERider_purge(self);
    int i;
    int none = -1;
    for (i = 0xFF; i >= 0; i--, slot++) {
        if (slot->id >= 0) {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sVEntry0019E498* vt = (sVEntry0019E498*)ps2GraphicsMan->mVtbl;
            vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, slot->id);
        }
        slot->id = none;
        slot->flag = 0;
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
extern "C" void cFERider_setVisible(void* self, int enable)
{
    if (enable != 0) {
        if (*(int*)((char*)self + 0xcc0) != 0 && *(int*)((char*)self + 0xcb8) != 0 &&
            *(int*)((char*)self + 0xcb4) != 0 && *(int*)((char*)self + 0xcd4) < 0) {
            *(int*)((char*)self + 0xcc8) = 1;
        } else {
            *(int*)((char*)self + 0xcc4) = 1;
        }
    } else {
        *(int*)((char*)self + 0xcc8) = 0;
        *(int*)((char*)self + 0xcc4) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequipdetail", cFERider_setCharID);
#ifdef SKIP_ASM

struct sVE_E588 {
    short delta;
    short index;
    int (*fn)(void*);
};

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
extern "C" void cFERider_setCharID(sRider_E588* self, unsigned int item, int flag)
{
    self->reqItem = item;
    self->reqFlag = flag;
    // The three-way flag test (equivalent to f10 != 0 || flag == 0) is kept as written for codegen.
    if (self->item == item
        && ((self->f10 != 0 && flag != 0) || (self->f10 == 0 && flag == 0) || !(self->f10 == 0 || flag != 0))) {
        int ok = 0;
        if (self->item < 10 && self->fCB8 != 0) {
            ok = self->fCB4 != 0;
        }
        if (ok) {
            self->timer = -1;
            int* visibleField = &self->fCC8;
            int showNow = 0;
            if (*visibleField != 0 || self->fCC4 != 0) {
                showNow = 1;
            }
            *visibleField = showNow;
            self->fCC4 = 0;
        }
        return;
    }
    if (self->timer < 0) {
        cPS2GraphicsMan* w = GrMan;
        sVE_E588* vt = (sVE_E588*)w->mVtbl;
        self->timer = vt[114].fn((char*)w + vt[114].delta) + 4;
        self->fCC4 = self->fCC8;
        self->fCC8 = 0;
    }
}
#endif

