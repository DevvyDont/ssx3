#include "common.h"

//100%
INCLUDE_ASM("wscript/wscriptmission", cWScriptMan_checkGate);
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
extern "C" int func_00398380(void* self, int id);
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
    vt[slot].fn(o + vt[slot].delta, a);
}

static inline int lookup_3DE0(char* self, char* name)
{
    char* mgr = *(char**)(self + 0x10);
    int h = GetHashValue32(name);
    return func_00398380(mgr + 0x58, h);
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
extern "C" void cWScriptMan_checkGate(void* vself)
{
    char* self = (char*)vself;
    void* screen = cList_first((cList*)(self + 0x24));
    char* labels[9];
    char* marks[9];
    for (int i = 0; i < 9; i++) {
        char name[0x10];
        sprintf(name, str_Trickpct_02d, i);
        char* uiObj = (char*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        char** lp = &labels[i];
        if (uiObj != 0) {
            *lp = uiObj;
            uiCall_3DE0(uiObj, 8, 1);
            uiCall_3DE0(uiObj, 9, 0);
            *(int*)(uiObj + 0x18) = -1;
        } else {
            *lp = 0;
        }
        sprintf(name, str_ckpct_02d, i);
        uiObj = (char*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
        char** mp = &marks[i];
        if (uiObj != 0) {
            *mp = uiObj;
            uiCall_3DE0(uiObj, 9, 0);
            *(int*)(uiObj + 0x18) = -1;
        } else {
            *mp = 0;
        }
    }
    sGateVec_3DE0 textSize0, textSize1, textSize2, s3, o0, o1, o2, p0, p1, p2, e0, e1, e2;
    int count;
    int cur;
    int slot = 1;
    char* rider = *(char**)(*(char**)(self + 0x4C) + 0xA0);
    if (rider != 0)
        slot = *(signed char*)(rider + 0x18);
    int shown = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    sGateEntry_3DE0* tbl = (sGateEntry_3DE0*)func_0014FF90(iface, *(signed char*)(self + 0x44), slot, &count);
    cur = func_0014FEA8(iface, *(signed char*)(self + 0x44), slot, 1);
    for (int j = 0; j < count; j++) {
        if (func_00150048(iface, *(signed char*)(self + 0x44), slot, j)) {
            char* uiObj = labels[shown];
            if (uiObj != 0) {
                cUIText_setAsciiString((cUIText*)uiObj, func_00116938(tbl[j].name));
                uiCall_3DE0(uiObj, 9, 1);
                uiCall_3DE0(uiObj, 8, 0);
                *(int*)(uiObj + 0x18) = j;
            }
            uiObj = marks[shown];
            if (uiObj != 0) {
                uiCall_3DE0(uiObj, 9, 1);
                if (func_0014FFB8(iface, *(signed char*)(self + 0x44), slot, j))
                    setHighlight_3DE0(uiObj, *(int*)(self + 0x5C));
                else if (cur == j)
                    setHighlight_3DE0(uiObj, *(int*)(self + 0x54));
                else
                    setHighlight_3DE0(uiObj, *(int*)(self + 0x58));
            }
            shown++;
        }
    }
    int ca = 0;
    int cb = 0;
    int c0 = lookup_3DE0(self, str_bsl2);
    int c1 = lookup_3DE0(self, str_bsl1);
    int c2 = lookup_3DE0(self, str_bsr1);
    int c3 = lookup_3DE0(self, str_bsr2);
    int c4 = lookup_3DE0(self, str_square_NML);
    switch (slot) {
    case 1:
        ca = c1;
        break;
    case 3:
        ca = c3;
        break;
    case 0:
        ca = c0;
        break;
    case 4:
        ca = c0;
        cb = c1;
        break;
    case 9:
        cb = c3;
        ca = c2;
        break;
    case 2:
        ca = c2;
        break;
    }
    float gap = 10.0f;
    char* t0 = getObj_3DE0(screen, str_InstBtn0);
    setTab_3DE0(t0, ca);
    char* t1 = getObj_3DE0(screen, str_InstBtn1);
    setTab_3DE0(t1, cb);
    char* t2 = getObj_3DE0(screen, str_InstBtn2);
    setTab_3DE0(t2, c4);
    char* w0 = getObj_3DE0(screen, str_inst0);
    char* w1 = getObj_3DE0(screen, str_inst1);
    char* w2 = getObj_3DE0(screen, str_inst2);
    func_003A1030(w0, &textSize0);
    textSize0.x += gap;
    func_003A1030(w1, &textSize1);
    func_003A1030(w2, &textSize2);
    textSize2.x += gap;
    func_003A0290(w0, &o0);
    func_003A0290(w2, &o2);
    p0 = *(sGateVec_3DE0*)(t0 + 0x44);
    p1 = *(sGateVec_3DE0*)(t1 + 0x44);
    p2 = *(sGateVec_3DE0*)(t2 + 0x44);
    sUIVtP_3DE0* vt0 = *(sUIVtP_3DE0**)(t0 + 8);
    vt0[20].fn(t0 + vt0[20].delta, &e0);
    sUIVtP_3DE0* vt1 = *(sUIVtP_3DE0**)(t1 + 8);
    vt1[20].fn(t1 + vt1[20].delta, &e1);
    sUIVtP_3DE0* vt2 = *(sUIVtP_3DE0**)(t2 + 8);
    vt2[20].fn(t2 + vt2[20].delta, &e2);
    float x = o0.x + textSize0.x;
    p0.x = x;
    p1.x = x;
    if (*(int*)(t1 + 0x7C) != 0) {
        e1.x += gap;
        p1.x = x + (e0.x + gap);
    } else {
        e1 = t3vect_Zero;
    }
    p2.x = o2.x + textSize2.x;
    *(sGateVec_3DE0*)(t0 + 0x44) = p0;
    *(sGateVec_3DE0*)(t1 + 0x44) = p1;
    *(sGateVec_3DE0*)(t2 + 0x44) = p2;
    o0 = *(sGateVec_3DE0*)(w0 + 0x44);
    o1 = *(sGateVec_3DE0*)(w1 + 0x44);
    o1.x = o0.x + textSize0.x + 10.0f + e0.x + e1.x;
    *(sGateVec_3DE0*)(w1 + 0x44) = o1;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptmission", func_00184520);
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

extern "C" void func_00184520(sWSMission4520* self)
{
    signed char b = self->p4C->rider->slot;
    signed char a = self->p50->rider->slot;
    char buf[16];
    sWSMEntry4520* tbl = func_0014FF90(cBE_getInterface_Fv(cBE_getBE(), 6), self->player, b, buf);
    void* screen = cList_first((cList*)self->list);
    cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_num));
    if (t != 0) {
        cUIText_setAsciiString(t, getDollarString(tbl[a].id));
    }
    cUIText* t2 = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_num));
    if (t2 != 0) {
        signed char cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->player);
        cUIText_setAsciiString(t2, getDollarString(cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 0xB), self->player, cid)));
    }
}
#endif

