#include "common.h"
#include "ui/uithing.h"
#include "vtable.h"
#include "fe/festateubertrick.h"

//100%
INCLUDE_ASM("wscript/wscriptmission", cFEStateUberTrick_updateTrickMenu);
#ifdef SKIP_ASM
struct cList;
struct cUIText;
struct sWSMEntry4520;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void* cList_first(cList* list);
int GetHashValue32(char*);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" sWSMEntry4520* func_0014FF90(void* iface, int player, int slot, void* out);
extern "C" int func_0014FEA8(void* iface, int player, int slot, int a3);
extern "C" int func_00150048(void* iface, int player, int slot, int idx);
extern "C" int func_0014FFB8(void* iface, int player, int slot, int idx);
extern "C" const char* func_00116938(unsigned short id);
extern "C" int cUITextureBank_getElementByHashname(void* self, int id);
extern "C" void func_00183D08(void* obj, int colour, float a);
extern "C" void func_003A1030(void* obj, void* out);
extern "C" void func_003A0290(void* obj, void* out);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_Trickpct_02d[];
extern char str_ckpct_02d[];
extern char str_bsl2[];
extern char str_bsl1[];
extern char str_bsr1[];
extern char str_bsr2[];
extern char str_square_NML[];
extern char str_InstBtn0[];
extern char str_InstBtn1[];
extern char str_InstBtn2[];
extern char str_inst0[];
extern char str_inst1[];
extern char str_inst2[];

struct sGateVec_3DE0 {
    float x, y, z;
};
extern sGateVec_3DE0 t3vect_Zero;

struct sGateEntry_3DE0 {
    int f0;
    unsigned short name;    // 0x4
    unsigned short id;      // 0x6
};

struct sUIVt_3DE0 { short delta; short index; void (*fn)(void*, int); };
struct sUIVtP_3DE0 { short delta; short index; void (*fn)(void*, void*); };

static inline void uiCall_3DE0(char* o, int slot, int a)
{
    sUIVt_3DE0* vt = *(sUIVt_3DE0**)(o + 8);
    vt[slot].fn(vthis(o, vt[slot]), a);
}

static inline int lookup_3DE0(char* self, char* name)
{
    char* mgr = *(char**)(self + 0x10);
    int h = GetHashValue32(name);
    return cUITextureBank_getElementByHashname(mgr + 0x58, h);
}

static inline char* getObj_3DE0(void* screen, char* name)
{
    return (char*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
}

// colours a gate tab and gives it its colour as the highlight
static inline void setHighlight_3DE0(char* o, int c)
{
    *(int*)(o + 0x78) = -1;
    *(int*)(o + 0x7C) = c;
}

static inline void setTab_3DE0(char* t, int c)
{
    func_00183D08(t, c, 0.8f);
    setHighlight_3DE0(t, c);
}

/**
 * Fills the store's uber-trick screen with the tricks of the selected category; the name does not fit the
 * body.
 *
 * Shows each unlocked trick of the category (the selected menu item's misc data, default 1) in
 * "Trick00".."Trick08" with its check mark "ck00".., coloured as owned (+0x5C), current pick (+0x54) or
 * neither (+0x58). Then sets the button icons "InstBtn0".."InstBtn2" for the category (bsl1/bsl2/bsr1/bsr2,
 * "square NML") and lays out the instruction texts "inst0".."inst2" around them. Called by the store's
 * uber-trick states (fe/festatestore).
 *
 * @param vself  the store state
 */
extern "C" void cFEStateUberTrick_updateTrickMenu(cFEStateUberTrick* vself)
{
    void* screen = cList_first((cList*)(((char*)vself) + 0x24));
    char* labels[9];
    char* marks[9];
    for (int i = 0; i < 9; i++) {
        char name[0x10];
        sprintf(name, str_Trickpct_02d, i);
        cUIThing* uiObj = (cUIThing*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        char** lp = &labels[i];
        if (((char*)uiObj) != 0) {
            *lp = ((char*)uiObj);
            uiCall_3DE0(((char*)uiObj), 8, 1);
            uiCall_3DE0(((char*)uiObj), 9, 0);
            uiObj->mMiscData = -1;
        } else {
            *lp = 0;
        }
        sprintf(name, str_ckpct_02d, i);
        uiObj = (cUIThing*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        char** mp = &marks[i];
        if (((char*)uiObj) != 0) {
            *mp = ((char*)uiObj);
            uiCall_3DE0(((char*)uiObj), 9, 0);
            uiObj->mMiscData = -1;
        } else {
            *mp = 0;
        }
    }
    sGateVec_3DE0 textSize0, textSize1, textSize2, spareVec, instPos0, instPos1, instPos2, btnPos0, btnPos1, btnPos2, btnSize0, btnSize1, btnSize2;
    int count;
    int cur;
    int slot = 1;
    char* rider = *(char**)((char*)vself->mpMainMenu + 0xA0);
    if (rider != 0)
        slot = *(signed char*)(rider + 0x18);
    int shown = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    sGateEntry_3DE0* tbl = (sGateEntry_3DE0*)func_0014FF90(iface, vself->mUserNum, slot, &count);
    cur = func_0014FEA8(iface, vself->mUserNum, slot, 1);
    for (int j = 0; j < count; j++) {
        if (func_00150048(iface, vself->mUserNum, slot, j)) {
            cUIThing* uiObj = (cUIThing*)labels[shown];
            if (((char*)uiObj) != 0) {
                cUIText_setAsciiString((cUIText*)uiObj, func_00116938(tbl[j].name));
                uiCall_3DE0(((char*)uiObj), 9, 1);
                uiCall_3DE0(((char*)uiObj), 8, 0);
                uiObj->mMiscData = j;
            }
            uiObj = (cUIThing*)marks[shown];
            if (((char*)uiObj) != 0) {
                uiCall_3DE0(((char*)uiObj), 9, 1);
                if (func_0014FFB8(iface, vself->mUserNum, slot, j))
                    setHighlight_3DE0(((char*)uiObj), (int)vself->mpDollarSign);
                else if (cur == j)
                    setHighlight_3DE0(((char*)uiObj), (int)vself->mpCheckmark);
                else
                    setHighlight_3DE0(((char*)uiObj), (int)vself->mpCheckbox);
            }
            shown++;
        }
    }
    int ca = 0;
    int cb = 0;
    int iconL2 = lookup_3DE0(((char*)vself), str_bsl2);
    int iconL1 = lookup_3DE0(((char*)vself), str_bsl1);
    int iconR1 = lookup_3DE0(((char*)vself), str_bsr1);
    int iconR2 = lookup_3DE0(((char*)vself), str_bsr2);
    int iconSquare = lookup_3DE0(((char*)vself), str_square_NML);
    switch (slot) {
    case 1:
        ca = iconL1;
        break;
    case 3:
        ca = iconR2;
        break;
    case 0:
        ca = iconL2;
        break;
    case 4:
        ca = iconL2;
        cb = iconL1;
        break;
    case 9:
        cb = iconR2;
        ca = iconR1;
        break;
    case 2:
        ca = iconR1;
        break;
    }
    float gap = 10.0f;
    cUIThing* instBtn0 = (cUIThing*)getObj_3DE0(screen, str_InstBtn0);
    setTab_3DE0(((char*)instBtn0), ca);
    char* instBtn1 = getObj_3DE0(screen, str_InstBtn1);
    setTab_3DE0(instBtn1, cb);
    cUIThing* instBtn2 = (cUIThing*)getObj_3DE0(screen, str_InstBtn2);
    setTab_3DE0(((char*)instBtn2), iconSquare);
    char* w0 = getObj_3DE0(screen, str_inst0);
    cUIThing* w1 = (cUIThing*)getObj_3DE0(screen, str_inst1);
    char* w2 = getObj_3DE0(screen, str_inst2);
    func_003A1030(w0, &textSize0);
    textSize0.x += gap;
    func_003A1030(w1, &textSize1);
    func_003A1030(w2, &textSize2);
    textSize2.x += gap;
    func_003A0290(w0, &instPos0);
    func_003A0290(w2, &instPos2);
    btnPos0 = *(sGateVec_3DE0*)&instBtn0->mPosition;
    btnPos1 = *(sGateVec_3DE0*)(instBtn1 + 0x44);
    btnPos2 = *(sGateVec_3DE0*)&instBtn2->mPosition;
    sUIVtP_3DE0* vt0 = (sUIVtP_3DE0*)instBtn0->mVtbl;
    vt0[20].fn(vthis(instBtn0, vt0[20]), &btnSize0);
    sUIVtP_3DE0* vt1 = *(sUIVtP_3DE0**)(instBtn1 + 8);
    vt1[20].fn(vthis(instBtn1, vt1[20]), &btnSize1);
    sUIVtP_3DE0* vt2 = (sUIVtP_3DE0*)instBtn2->mVtbl;
    vt2[20].fn(vthis(instBtn2, vt2[20]), &btnSize2);
    float x = instPos0.x + textSize0.x;
    btnPos0.x = x;
    btnPos1.x = x;
    if (*(int*)(instBtn1 + 0x7C) != 0) {
        btnSize1.x += gap;
        btnPos1.x = x + (btnSize0.x + gap);
    } else {
        btnSize1 = t3vect_Zero;
    }
    btnPos2.x = instPos2.x + textSize2.x;
    *(sGateVec_3DE0*)&instBtn0->mPosition = btnPos0;
    *(sGateVec_3DE0*)(instBtn1 + 0x44) = btnPos1;
    *(sGateVec_3DE0*)&instBtn2->mPosition = btnPos2;
    instPos0 = *(sGateVec_3DE0*)(w0 + 0x44);
    instPos1 = *(sGateVec_3DE0*)&w1->mPosition;
    instPos1.x = instPos0.x + textSize0.x + 10.0f + btnSize0.x + btnSize1.x;
    *(sGateVec_3DE0*)&w1->mPosition = instPos1;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptmission", cFEStateUberTrick_updateTrickMenuEntry);
#ifdef SKIP_ASM
struct cList;
struct cUIText;

struct sWSMEntry4520 {
    int f0;
    short f4;
    unsigned short id;          // 0x6
};

struct sWSMRider4520 {
    char pad0[0x18];
    signed char slot;           // 0x18
};

struct sWSMPlayer4520 {
    char pad0[0xA0];
    sWSMRider4520* rider;       // 0xA0
};

struct sWSMission4520 {
    char pad0[0x24];
    char list[0x20];            // 0x24 (cList)
    signed char player;         // 0x44
    char pad45[0x7];
    sWSMPlayer4520* p4C;        // 0x4C
    sWSMPlayer4520* p50;        // 0x50
};

extern char str_cost_num[];
extern char str_have_num[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
void* cList_first(cList* list);
int GetHashValue32(char*);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" sWSMEntry4520* func_0014FF90(void* iface, int player, int slot, void* out);
extern "C" int cBEEconInterface_getMoney(void* iface, int player, int charID);
extern "C" const char* getDollarString(int id);

/**
 * Refreshes the uber-trick screen's price readouts: "cost_num" shows the selected trick's cost and "have_num" the
 * player's money.
 *
 * Takes the selected items' mMiscData of the category menu (+0x4C) and trick menu (+0x50), gets the category's trick
 * table with func_0014FF90 on cBE interface 6 for the player (+0x44), and on the first screen of the state's list
 * (+0x24) sets "cost_num" to getDollarString of the trick entry's cost (halfword at +0x6 of its 8-byte entry) and
 * "have_num" to getDollarString of cBEEconInterface_getMoney (interface 11) for the player and their character
 * (cBENewPlayerInterface_getPlayerCharID on interface 1). A missing text object is skipped.
 * cFEStateUberTrick_onPopupEvent calls it after a trick is bought, cFEStateUberTrick_onWidgetEvent when a trick is
 * highlighted.
 */
extern "C" void cFEStateUberTrick_updateTrickMenuEntry(sWSMission4520* self)
{
    signed char categoryIndex = self->p4C->rider->slot;
    signed char trickIndex = self->p50->rider->slot;
    char buf[16];
    sWSMEntry4520* tbl = func_0014FF90(cBE_getInterface_Fv(cBE_getBE(), 6), self->player, categoryIndex, buf);
    void* screen = cList_first((cList*)self->list);
    cUIText* costText = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_num));
    if (costText != 0) {
        cUIText_setAsciiString(costText, getDollarString(tbl[trickIndex].id));
    }
    cUIText* moneyText = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_num));
    if (moneyText != 0) {
        signed char cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->player);
        cUIText_setAsciiString(moneyText, getDollarString(cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->player, cid)));
    }
}
#endif

