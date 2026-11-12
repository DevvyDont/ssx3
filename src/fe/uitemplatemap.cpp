#include "common.h"

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cUITemplate_MAP_setShowInfo(void* self, int show);
extern "C" int func_001A3BF0(void* mgr);
extern "C" void cUITemplate_MAP_setupPeakUnlock(void* self);
struct cUITemplate_MAP;
void cUITemplate_MAP_setupEventUnlock(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupLayout(void* self);
extern "C" void cUITemplate_MAP_setupMapPic(void* self);
extern "C" void cUITemplate_MAP_setupPopup(char* self);
extern "C" void cUITemplate_MAP_setupPeakInfo(void* self);
extern unsigned int D_004A2594;
extern int D_004A2598;
extern int D_004A259C;
extern void* D_004A28A8;
extern char D_004A25A8[];
extern char D_004A25B0[];
extern char D_004A25B8[];
extern char D_004A25C0[];
extern char D_004A25C8[];
extern char D_004A25D0[];
extern char D_004A25D8[];
extern char D_004704C8[];
extern char D_004704D8[];
extern char D_004704E8[];
extern char D_004704F8[];
extern char D_00470508[];
extern char D_00470518[];
extern char D_00470528[];
extern char D_00470538[];
extern char D_00470548[];
extern char D_00470558[];
extern char D_00470568[];
extern char D_00470578[];
extern char D_00470588[];
extern char D_00470598[];
extern char D_004705A8[];
extern char D_004705B8[];
extern char D_004705C8[];
extern char D_004705D8[];
extern char D_004705E8[];

class cUIObj_200700 {
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
    virtual void v09(int on);
};

struct sMapTmpl_200700 {
    char pad0[0x2CC];
    int popup;                  // 0x2CC
    int mode;                   // 0x2D0
    int pic;                    // 0x2D4
    int file;                   // 0x2D8
    int f2DC;
    int f2E0;
    cUIObj_200700* map;         // 0x2E4
    void* screen;               // 0x2E8
    void* owner;                // 0x2EC
    int f2F0;                   // 0x2F0
};

extern "C" void cUITemplate_MAP_onCreateScreen(void* tmpl, void* screen, void* owner)
{
    sMapTmpl_200700* self = (sMapTmpl_200700*)tmpl;
    self->owner = owner;
    self->screen = screen;
    if (D_004A2594 == 1 || D_004A2594 == 3 || D_004A2594 == 4)
        cOVStateManager_addPDATemplate();
    if (D_004A2594 != 0 || D_004A2598 == 0)
        D_004A259C = 0;
    cUITemplate_MAP_setShowInfo(self, 0);
    self->popup = 0;
    self->mode = -1;
    self->file = func_001A3BF0(*(void**)((char*)D_004A28A8 + 0x11C));
    self->pic = 0;
    cUIObj_200700* o = (cUIObj_200700*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A25A8));
    self->map = o;
    if (o != 0)
        o->v09(0);
    self->f2F0 = 0;
    cUITemplate_MAP_setupPeakUnlock(self);
    cUITemplate_MAP_setupEventUnlock((cUITemplate_MAP*)self);
    cUITemplate_MAP_setupLayout(self);
    cUITemplate_MAP_setupMapPic(self);
    cUITemplate_MAP_setupPopup((char*)self);
    if (D_004A2594 >= 2)
        cUITemplate_MAP_setupPeakInfo(self);
    if (D_004A2594 < 2) {
        cUIState_hideObjSafe(self->owner, D_004704C8);
        cUIState_hideObjSafe(self->owner, D_004A25B0);
        if (D_004A2594 == 0) {
            cUIState_hideObjSafe(self->owner, D_004A25B8);
        } else {
            cUIState_hideObjSafe(self->owner, D_004704D8);
            cUIState_hideObjSafe(self->owner, D_004704E8);
            cUIState_hideObjSafe(self->owner, D_004704F8);
            cUIState_hideObjSafe(self->owner, D_00470508);
        }
    } else {
        cUIState_hideObjSafe(self->owner, D_004A25C0);
        cUIState_showObjSafe(self->owner, D_00470518);
    }
    if (D_004A2594 == 4) {
        cUIState_hideObjSafe(self->owner, D_00470528);
        cUIState_hideObjSafe(self->owner, D_00470538);
        cUIState_hideObjSafe(self->owner, D_004A25C8);
        cUIState_hideObjSafe(self->owner, D_004A25D0);
    }
    cUIState_hideObjSafe(self->owner, D_004A25D8);
    cUIState_hideObjSafe(self->owner, D_00470548);
    cUIState_hideObjSafe(self->owner, D_00470558);
    cUIState_hideObjSafe(self->owner, D_00470568);
    cUIState_hideObjSafe(self->owner, D_00470578);
    cUIState_hideObjSafe(self->owner, D_00470588);
    cUIState_hideObjSafe(self->owner, D_00470598);
    cUIState_hideObjSafe(self->owner, D_004705A8);
    cUIState_hideObjSafe(self->owner, D_004705B8);
    cUIState_hideObjSafe(self->owner, D_004705C8);
    cUIState_hideObjSafe(self->owner, D_004705D8);
    cUIState_hideObjSafe(self->owner, D_004705E8);
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_002009D0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
int GetHashValue32(char* str);
extern char D_00470600[];
extern unsigned int D_004A2594;
static inline int isLabel2009D0(int id, char* s)
{
    return id == GetHashValue32(s);
}
// PORT: the unit declares func_002009D0(void*); the body also takes the menu in $5.
extern "C" void func_002009D0_r(void* self, void* menu) __asm__("func_002009D0");

extern "C" void func_002009D0_r(void* self, void* menu)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (isLabel2009D0(*(int*)((char*)menu + 0x38), D_00470600)) {
        if (D_004A2594 < 2) {
            cUIMenu_setSelectedByIndex(menu, 2);
        } else {
            cUIMenu_setSelectedByIndex(menu, 2 - *(unsigned char*)((char*)cBENewRaceInterface_getLocation(iface) + 0x54));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(void* self, int focus);
extern "C" void cUITemplate_MAP_setupMenus(void* self);
extern "C" void func_00207430(void* self);
extern "C" void cUITemplate_MAP_setShowInfo(void* self, int show);
extern int D_004A259C;
// PORT: the unit declares cUITemplate_MAP_onGainTransition(void*); the body also takes a flag in $5.
extern "C" int cUITemplate_MAP_onGainTransition_r(void* self, int on) __asm__("cUITemplate_MAP_onGainTransition");

extern "C" int cUITemplate_MAP_onGainTransition_r(void* self, int on)
{
    if (on != 0) {
        cUITemplate_MAP_setupMenuFocus(self, D_004A259C);
        cUITemplate_MAP_setupMenus(self);
        func_00207430(self);
        cUITemplate_MAP_setShowInfo(self, *(int*)self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(void* self, int focus);
extern "C" void* func_0028B180(void);
extern "C" void func_0028F5B8(void* mgr);
extern int D_004A259C;

extern "C" void cUITemplate_MAP_onGainFocus(void* self)
{
    cUITemplate_MAP_setupMenuFocus(self, D_004A259C);
    func_0028F5B8(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200AF0);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void* func_0028B180(void);
extern "C" void func_0028F678(void* mgr, int a);
extern void* D_004A28A8;
extern unsigned int D_004A2594;

extern "C" void func_00200AF0(void* self)
{
    cFEAsyncManager_UnloadFEAsyncFile(*(void**)((char*)D_004A28A8 + 0x11C), *(int*)((char*)self + 0x2D8));
    if (D_004A2594 == 1 || D_004A2594 == 3 || D_004A2594 == 4) {
        cOVStateManager_removePDATemplate();
    }
    func_0028F678(func_0028B180(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200B50);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
int cBERewardInterface_getTrackMedal(void* self, int player, int charID, int mode, int track);
// PORT: the unit declares func_00146A70 as returning int; its result is negated as a bool here.
bool func_00146A70_b(void* iface, int player, int charID, int x) __asm__("func_00146A70");

// The unit's sPeakGoal (0x6C bytes), defined further down; only the leading fields are read here.
struct sMapEvent_0B50 { int track; int f4; int kind; int rest[0x6C / 4 - 3]; };

// PORT: the unit declares func_00200B50(void*, sPeakGoal*, int) further down; the goal is really taken by value
// (the callee copies it), so this definition binds the by-value form to the symbol. Each kind returns through its
// own call; g++ merges the two identical tails.
extern "C" int func_00200B50_byval(void* self, sMapEvent_0B50 ev, int peak) __asm__("func_00200B50");
extern "C" int func_00200B50_byval(void* self, sMapEvent_0B50 ev, int peak)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* rw = cBE_getInterface_Fv(cBE_getBE(), 13);
    int cid = cBELibrary_getCharacterID(0);
    if (ev.track == 0x17)
        return 0;
    if (ev.kind == 4) {
        int x = 6;
        if (peak == 1)
            x = 7;
        if (peak == 2)
            x = 8;
        return !func_00146A70_b(np, 0, cBELibrary_getCharacterID(0), x);
    } else if (ev.kind == 5) {
        int x = 9;
        if (peak == 1)
            x = 10;
        if (peak == 2)
            x = 11;
        return !func_00146A70_b(np, 0, cBELibrary_getCharacterID(0), x);
    }
    return cBERewardInterface_getTrackMedal(rw, 0, cid, ev.kind, ev.track) != -1;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00200D00);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int func_001464D0(void* iface, int player, int charID, int x, int flag);
extern "C" int func_00146A70(void* iface, int player, int charID, int x);

extern "C" int func_00200D00(void* self, int kind, int x)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (D_004A2594 < 2) {
        return 0;
    }
    if (kind == 4) {
        return func_001464D0(iface, 0, cBELibrary_getCharacterID(0), x, 1);
    }
    if (kind == 5) {
        return func_001464D0(iface, 0, cBELibrary_getCharacterID(0), x, 0);
    }
    if (kind >= 6 && kind <= 11) {
        return func_00146A70(iface, 0, cBELibrary_getCharacterID(0), kind);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenus);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.
class cUIThing {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void setEnabled(int on);
    virtual void setFocus(int on);
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

struct cUIText;

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int D_004A2594;

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern char D_004706C8[];

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).
struct sPeakGoal {
    int track;          // 0x00
    int f04;
    int kind;           // 0x08
    int f0C;
    int useTrackName;   // 0x10
    char name[0x1E];    // 0x14
    char help[0x23];    // 0x32 help-text key (func_00207430)
    char path[0x17];    // 0x55
};

extern sPeakGoal D_004781D0_enus[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_enus[3][5] __asm__("D_004786E0");

extern sPeakGoal D_00478D38_enus[3][8] __asm__("D_00478D38");

extern "C" void cUITemplate_MAP_setWidgetUnHiliteColor_enus(void* self, void* obj, int on) __asm__("cUITemplate_MAP_setWidgetUnHiliteColor");
extern "C" int func_00200B50(void* self, sPeakGoal* loc, int peak);
extern "C" int func_00200D00_enus(void* self, int kind, int x) __asm__("func_00200D00");
extern "C" int func_004165A8(const char*, const char*);
extern "C" const char* cBENewRaceInterface_getLocationName_enus(void* iface, int id) __asm__("cBENewRaceInterface_getLocationName");
extern "C" unsigned short* func_002C2540_enus(unsigned short* dst, const char* s) __asm__("func_002C2540");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" unsigned short* func_001FD190(int kind);
extern "C" void cUIText_setUnicodeString_enus(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" char* strcpy(char* d, const char* s);
extern char D_004A25E0[];
extern char D_004A25E8[];
extern char D_004A25F0[];
extern char D_00470610[];
extern char D_00470620[];
extern char D_00470630[];
extern char D_00470648[];
extern char D_00470660[];
extern char D_004A2560[];
extern char D_004A2590[];
extern char D_004706B8[];
extern char D_004C8B58[];

// A menu entry widget: cUIThing plus its menu index at 0x18; slot 11 sets the text colour.
class cUIObj_Menus : public cUIThing {
public:
    int f0C;
    int f10;
    int f14;
    int idx;            // 0x18
    void setIndex(int i) { idx = i; }
    virtual void v10();
    virtual void setColor(char* col);
};

// The MAP template as this function sees it.
struct sMapTmpl_200DE8 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int available[3][3][8];     // 0x20  [peak][mode][event]
    int f140[3][3][8];          // 0x140
    char pad260[0x2E8 - 0x260];
    void* screen;               // 0x2E8
};

// Location entries with track 0x17 have no event: no availability colour or text, the widget is made static.
static inline int hasEvent(int track) { return track != 0x17; }

extern "C" void cUITemplate_MAP_setupMenus(void* tmpl)
{
    sMapTmpl_200DE8* self = (sMapTmpl_200DE8*)tmpl;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cUIObj_Menus* o;
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A25E0));
    if (o != 0) {
        o->setIndex(2);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->fC);
    }
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A25E8));
    if (o != 0) {
        o->setIndex(1);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f8);
    }
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A25F0));
    if (o != 0) {
        o->setIndex(0);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f4);
    }
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470610));
    if (o != 0) {
        o->setIndex(3);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, 0);
    }
    char buf[32];
    for (int i = 0; i < 3; i++) {
        sprintf(buf, D_00470620, i + 1);
        o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(buf));
        o->setIndex(0);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f10);
        sprintf(buf, D_00470630, i + 1);
        o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(buf));
        o->setIndex(1);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f14);
        sprintf(buf, D_00470648, i + 1);
        o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(buf));
        o->setIndex(2);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f18);
        if (D_004A2594 < 2) {
            o->setStatic(1);
            o->setVisible(0);
        }
        sprintf(buf, D_00470660, i + 1);
        o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(buf));
        o->setIndex(3);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->f1C);
        if (D_004A2594 < 2) {
            o->setStatic(1);
            o->setVisible(0);
        }
    }
    for (int peak = 0; peak < 3; peak++) {
        for (int mode = 0; mode < 3; mode++) {
            int count;
            char fmt[24];
            if (mode == 0) {
                count = 4;
                strcpy(fmt, "Peak%dRaceLoc%d");
            } else if (mode == 1) {
                count = 5;
                strcpy(fmt, "Peak%dFreestyleLoc%d");
            } else {
                count = 8;
                strcpy(fmt, "Peak%dFreerideLoc%d");
            }
            for (int j = 0; j < count; j++) {
                sPeakGoal g = D_004781D0_enus[peak][j];
                if (mode == 1)
                    g = D_004786E0_enus[peak][j];
                if (mode == 2)
                    g = D_00478D38_enus[peak][j];
                if (mode == 0 || mode == 1)
                    self->f140[peak][mode][j] = func_00200B50(self, &g, peak);
                sprintf(buf, fmt, peak + 1, j);
                o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(buf));
                if (D_004A2594 < 2 && g.f0C != 0) {
                    self->available[peak][mode][j] = 0;
                    if (o != 0) {
                        o->setVisible(0);
                        o->setStatic(1);
                    }
                } else {
                    self->available[peak][mode][j] = func_00200D00_enus(self, g.kind, peak) ^ 1;
                }
                if (hasEvent(g.track))
                    cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, o, self->available[peak][mode][j]);
                if (hasEvent(g.track)) {
                    if (g.useTrackName == 1) {
                        cUIText_setAsciiString((cUIText*)o, cBENewRaceInterface_getLocationName_enus(iface, g.track));
                    } else if (func_004165A8(g.name, D_004A2560)) {
                        cUIText_setAsciiString((cUIText*)o, g.name);
                    } else if (g.kind == 4) {
                        unsigned short text[200];
                        unsigned short wfmt[24];
                        func_002C2540_enus(wfmt, D_004A2590);
                        sprintf1(text, wfmt, cBENewRaceInterface_getLocationName_enus(iface, g.track));
                        cUIText_setUnicodeString_enus(o, text);
                    } else {
                        cUIText_setUnicodeString_enus(o, func_001FD190(g.kind));
                    }
                    o->setIndex(j);
                } else if (o != 0) {
                    o->setStatic(1);
                }
            }
        }
    }
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004706B8));
    if (o != 0) {
        o->setIndex(0);
        o->setColor(D_004C8B58);
    }
    o = (cUIObj_Menus*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004706C8));
    if (o != 0) {
        o->setIndex(1);
        o->setColor(D_004C8B58);
    }
}
#endif

struct cUITemplate_MAP {
    char pad_0x00[0x10];
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1C;
};
extern unsigned int D_004A2594;

//99.9%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupEventUnlock__FP15cUITemplate_MAP);
#ifdef SKIP_ASM
void cUITemplate_MAP_setupEventUnlock(cUITemplate_MAP* self)
{
    self->field_0x1C = 0;
    self->field_0x10 = 1;
    self->field_0x14 = 1;
    if (D_004A2594 < 2) {
        self->field_0x18 = 0;
    } else {
        self->field_0x18 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakUnlock);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" int cBENewPlayerInterface_isPeakLocked(void* self, int a, int b, int peak);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern char D_00535BC8[];

struct sPeakUnlock_MAP {
    int pad0;
    int unlocked[3];    // 0x4, indexed by peak
};

// Marks each peak as unlocked unless it is still locked for the active rider(s).
extern "C" void cUITemplate_MAP_setupPeakUnlock(void* p)
{
    sPeakUnlock_MAP* self = (sPeakUnlock_MAP*)p;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    self->unlocked[0] = 1;
    self->unlocked[1] = 1;
    self->unlocked[2] = 1;
    for (int peak = 1; peak < 3; peak++) {
        int locked;
        if (D_00535BC8[0x49] == 2) {
            locked = cBENewPlayerInterface_isPeakLocked(player, 0, cBENewPlayerInterface_getPlayerCharID(player, 0), peak);
            if (locked) {
                locked = cBENewPlayerInterface_isPeakLocked(player, 1, cBENewPlayerInterface_getPlayerCharID(player, 1), peak);
            }
        } else {
            locked = cBENewPlayerInterface_isPeakLocked1(player, 0, peak);
        }
        // The store sits in its own do { } while (0) block (macro-style); the extra loop level is what
        // gives the slot pointer priority over the peak counter in register allocation.
        do {
            if (locked) {
                self->unlocked[peak] = 0;
            }
        } while (0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00201868);
#ifdef SKIP_ASM
struct sVEntry00201868 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00201868(void* self)
{
    *(int*)((char*)self + 0x2F0) = 1;
    *(int*)((char*)self + 0x2DC) = 0;
    void* obj = *(void**)((char*)self + 0x2E4);
    if (obj != 0) {
        sVEntry00201868* vt = *(sVEntry00201868**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onWidgetEvent);

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00202738);
#ifdef SKIP_ASM
extern "C" int func_00202738(void* self, int a1, int a2)
{
    if (a2 == 6 || a2 == 9 || a2 == 8) {
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00202768__FPv);
#ifdef SKIP_ASM
int func_00202768(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onInputAlways);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupPeakInfo(void* self);
extern "C" void cUITemplate_MAP_setupPeakGoals(void* self);
extern "C" void cUITemplate_MAP_setupLayout(void* self);
extern "C" void func_00207430(void* self);
extern int D_004A259C;

struct sVEntry00202770 {
    short delta;
    short index;
    int (*fn)(void*);
};

// PORT: the unit declares cUITemplate_MAP_onInputAlways(void*); the body also takes the menu object in $5.
extern "C" void cUITemplate_MAP_onInputAlways_r(void* self, void* menu) __asm__("cUITemplate_MAP_onInputAlways");

extern "C" void cUITemplate_MAP_onInputAlways_r(void* self, void* menu)
{
    sVEntry00202770* e = &(*(sVEntry00202770**)((char*)menu + 8))[17];
    if (e->fn((char*)menu + e->delta) == 0) {
        sVEntry00202770* e2 = &(*(sVEntry00202770**)((char*)menu + 8))[18];
        if (e2->fn((char*)menu + e2->delta) == 0) {
            return;
        }
    }
    if (D_004A259C == 0) {
        cUITemplate_MAP_setupPeakInfo(self);
        cUITemplate_MAP_setupLayout(self);
    }
    if (D_004A259C == 1) {
        cUITemplate_MAP_setupPeakGoals(self);
        cUITemplate_MAP_setupLayout(self);
    }
    if (D_004A259C == 2) {
        func_00207430(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cFEAsyncManager;
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
extern char D_00470588[];
extern char D_00470598[];
extern char D_004705A8[];
extern char D_004707A0[];
extern char D_004707B0[];
extern int D_004A259C;
extern unsigned int D_004A2594;
extern void* D_004A28A8;

struct sColor_202828 {
    float a, r, g, b;
};

class cUIPic_202828 {
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
    virtual void v09(int on);
    virtual void v10();
    virtual void v11(sColor_202828* c);
};

extern "C" void cUITemplate_MAP_onUpdate(void* vself)
{
    char* self = (char*)vself;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (*(int*)(self + 0x2DC) != 0) {
        if (*(float*)(self + 0x2E0) < 1.0f)
            *(float*)(self + 0x2E0) += 0.1f;
        cUIPic_202828* pic = *(cUIPic_202828**)(self + 0x2E4);
        sColor_202828 c = *(sColor_202828*)((char*)pic + 0x1C);
        c.a = *(float*)(self + 0x2E0);
        pic->v11(&c);
        (*(cUIPic_202828**)(self + 0x2E4))->v09(1);
        if (D_004A259C == 0) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470588);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470598);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004705A8);
        } else {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707A0);
        }
    } else {
        *(float*)(self + 0x2E0) = 0.0f;
        (*(cUIPic_202828**)(self + 0x2E4))->v09(0);
        if (D_004A259C == 0) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470588);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470598);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004705A8);
        } else {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707A0);
        }
    }
    if (*(int*)(self + 0x2F0) != 0)
        return;
    for (int i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, D_004707B0, i + 1);
        if (*(int*)(self + 0x2DC) != 0 && D_004A259C == 0 && i == *(int*)((char*)cBENewRaceInterface_getLocation(iface) + 0x54) &&
            *(int*)self == 0 && D_004A2594 >= 2)
            cUIState_showObjSafe(*(void**)(self + 0x2EC), buf);
        else
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), buf);
    }
    char* mgr = *(char**)((char*)D_004A28A8 + 0x11C);
    if (cFEAsyncManager_GetFileStatus((cFEAsyncManager*)mgr, *(int*)(self + 0x2D8)) == 3 && *(int*)(self + 0x2DC) == 0) {
        char* pic = *(char**)(self + 0x2E4);
        if (pic != 0) {
            int tex = *(int*)(mgr + *(int*)(self + 0x2D8) * 0x11C + 0x110);
            *(int*)(pic + 0x7C) = 0;
            *(int*)(pic + 0x78) = tex;
            *(int*)(self + 0x2DC) = 1;
            *(float*)(self + 0x2E0) = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupLayout);
#ifdef SKIP_ASM
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_00207430(void* self);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern int D_004A259C;
extern int D_004A25A0;
extern int D_004A25A4;
extern unsigned int D_004A2594;
extern char D_0046F9E0[];
extern char D_00470600[];
extern char D_004706D8[];
extern char D_004707A0[];
extern char D_004707C0[];
extern char D_004707D8[];
extern char D_004707E8[];
extern char D_004707F8[];
extern char D_00470808[];
extern char D_00470818[];
extern char D_00470828[];
extern char D_00470840[];
extern char D_00470850[];
extern char D_00470860[];
extern char D_00470870[];
extern char D_00470888[];
extern char D_004708A0[];
extern char D_004708B8[];
extern char D_004708D0[];
extern char D_004708E0[];
extern char D_004708F8[];
extern char D_00470908[];
extern char D_004A25B0[];
extern char D_004A2608[];
extern char D_004A2610[];
extern char D_004A2618[];
extern char D_004A2620[];
extern char D_004A2628[];
extern char D_004A2630[];

extern "C" void cUITemplate_MAP_setupLayout(void* vself)
{
    char* self = (char*)vself;
    char name[32];
    char next[32];
    char again[32];
    cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_0046F9E0));
    switch (D_004A259C) {
    case 0: {
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_004707C0));
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707D8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2608);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707E8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707F8);
        if (*(int*)(self + 0x2CC) == 0)
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2610);
        int state = 0;
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707A0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2608);
        char* o = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470600));
        if (o != 0)
            state = *(unsigned char*)(o + 0x95);
        if (*(int*)self != 0 && state != 3)
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2608);
        if (state == 3) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470808);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470818);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2618);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2620);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2628);
        } else if (D_004A2594 >= 2) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470808);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470818);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2618);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2620);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2628);
        }
        break;
    }
    case 1: {
        int state = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470828));
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707D8);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707E8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707F8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2610);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707A0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470840);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470850);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470860);
        sprintf(name, D_00470870, D_004A25A4);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), name);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470888);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004708A0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004708B8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004708D0);
        sprintf(next, D_004706D8, D_004A25A4 + 1);
        char* o = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(next));
        if (o != 0)
            state = *(unsigned char*)(o + 0x95);
        if (*(int*)self != 0 || state == 3) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004708D0);
            switch (state) {
            case 0:
            case 1:
                cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470888);
                break;
            case 2:
                cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004708A0);
                break;
            case 3:
                cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004708B8);
                break;
            }
        }
        if (state == 3) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470808);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470818);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2618);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2620);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2628);
        } else if (D_004A2594 >= 2) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470808);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470818);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2618);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2620);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2628);
        }
        break;
    }
    case 2:
        if (D_004A25A0 == 0) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2610);
            cUIText_setUnicodeStringByID(title, GetHashValue32(D_004708E0));
        } else if (D_004A25A0 == 1) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2610);
            cUIText_setUnicodeStringByID(title, GetHashValue32(D_004708F8));
        } else {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2610);
            cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470908));
        }
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707D8);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004707E8);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707F8);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004707A0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470840);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470850);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470860);
        sprintf(name, D_00470870, D_004A25A4);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), name);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2630);
        if (*(int*)self != 0)
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2630);
        func_00207430(self);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470840);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470850);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470860);
        sprintf(again, D_00470870, D_004A25A4);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), again);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenuFocus);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupPeakGoals(void* self);
extern char D_00470518[];
extern char D_004706C8[];
extern char D_004707C0[];
extern char D_00470918[];
extern char D_00470928[];
extern char D_00470938[];
extern char D_00470948[];
extern char D_00470960[];
extern char D_00470978[];
extern char D_00470990[];
extern char D_004709A8[];
extern char D_004709C0[];
extern char D_004709D8[];
extern char D_004709F0[];
extern char D_00470A08[];
extern char D_00470A20[];
extern char D_00470A30[];
extern char D_00470A48[];

class cUIObj_2030A0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int on);
    virtual void v08(int on);
    virtual void v09(int on);
};

static inline cUIObj_2030A0* getObj_2030A0(char* self, char* name)
{
    return (cUIObj_2030A0*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(name));
}

static inline cUIObj_2030A0* hideObj_2030A0(char* self, char* name)
{
    cUIObj_2030A0* o = getObj_2030A0(self, name);
    if (o != 0) {
        o->v07(0);
        o->v09(0);
    }
    return o;
}

extern "C" void cUITemplate_MAP_setupMenuFocus(void* vself, int focus)
{
    char* self = (char*)vself;
    cUITemplate_MAP_setShowInfo(self, 0);
    D_004A259C = focus;
    cUIObj_2030A0* peak = hideObj_2030A0(self, D_00470600);
    cUIObj_2030A0* goal0 = hideObj_2030A0(self, D_00470918);
    cUIObj_2030A0* goal1 = hideObj_2030A0(self, D_00470928);
    cUIObj_2030A0* goal2 = hideObj_2030A0(self, D_00470938);
    cUIObj_2030A0* ev00 = hideObj_2030A0(self, D_00470948);
    cUIObj_2030A0* ev01 = hideObj_2030A0(self, D_00470960);
    cUIObj_2030A0* ev02 = hideObj_2030A0(self, D_00470978);
    cUIObj_2030A0* ev10 = hideObj_2030A0(self, D_00470990);
    cUIObj_2030A0* ev11 = hideObj_2030A0(self, D_004709A8);
    cUIObj_2030A0* ev12 = hideObj_2030A0(self, D_004709C0);
    cUIObj_2030A0* ev20 = hideObj_2030A0(self, D_004709D8);
    cUIObj_2030A0* ev21 = hideObj_2030A0(self, D_004709F0);
    cUIObj_2030A0* ev22 = hideObj_2030A0(self, D_00470A08);
    cUIObj_2030A0* popup = hideObj_2030A0(self, D_00470A20);
    if (*(int*)(self + 0x2CC) != 0) {
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2610);
        popup->v07(1);
        popup->v09(1);
        if (*(int*)(self + 0x2D0) == 0) {
            cUIObj_2030A0* o = getObj_2030A0(self, D_004706C8);
            o->v09(1);
            o->v08(0);
        } else {
            cUIObj_2030A0* o = getObj_2030A0(self, D_004706C8);
            o->v09(0);
            o->v08(1);
        }
        return;
    }
    switch (D_004A259C) {
    case 0:
        cUITemplate_MAP_setupPeakInfo(self);
        if (peak != 0) {
            *(int*)((char*)peak + 0x14) |= 1;
            peak->v07(1);
            peak->v09(1);
        }
        if (D_004A2594 < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(self, D_00470518), GetHashValue32(D_004707C0));
        }
        break;
    case 1:
        cUITemplate_MAP_setupPeakGoals(self);
        if (D_004A25A4 == 0 && goal0 != 0) {
            *(int*)((char*)goal0 + 0x14) |= 1;
            goal0->v09(1);
            goal0->v07(1);
        } else if (D_004A25A4 == 1 && goal1 != 0) {
            *(int*)((char*)goal1 + 0x14) |= 1;
            goal1->v09(1);
            goal1->v07(1);
        } else if (goal2 != 0) {
            *(int*)((char*)goal2 + 0x14) |= 1;
            goal2->v09(1);
            goal2->v07(1);
        }
        if (D_004A2594 < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(self, D_00470518), GetHashValue32(D_00470A30));
        }
        break;
    case 2: {
        cUIObj_2030A0* o = 0;
        if (D_004A25A4 == 0) {
            if (D_004A25A0 == 0)
                o = ev00;
            else if (D_004A25A0 == 1)
                o = ev01;
            else if (D_004A25A0 == 2)
                o = ev02;
        } else if (D_004A25A4 == 1) {
            if (D_004A25A0 == 0)
                o = ev10;
            else if (D_004A25A0 == 1)
                o = ev11;
            else if (D_004A25A0 == 2)
                o = ev12;
        } else if (D_004A25A4 == 2) {
            if (D_004A25A0 == 0)
                o = ev20;
            else if (D_004A25A0 == 1)
                o = ev21;
            else if (D_004A25A0 == 2)
                o = ev22;
        }
        if (o != 0) {
            *(int*)((char*)o + 0x14) |= 1;
            o->v09(1);
            o->v07(1);
        }
        if (D_004A2594 < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(self, D_00470518), GetHashValue32(D_00470A48));
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMapPic);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_Load3PeakPic(void* mgr, int file, int a);
extern "C" void cFEAsyncManager_Load1PeakPic(void* mgr, int peak, int file, int a);
extern int D_004A25A4;

extern "C" void cUITemplate_MAP_setupMapPic(void* self)
{
    void* mgr = *(void**)((char*)D_004A28A8 + 0x11C);
    if (D_004A259C == 0) {
        if (*(int*)((char*)self + 0x2D4) != 1) {
            cFEAsyncManager_UnloadFEAsyncFile(mgr, *(int*)((char*)self + 0x2D8));
            cFEAsyncManager_Load3PeakPic(mgr, *(int*)((char*)self + 0x2D8), 1);
            *(int*)((char*)self + 0x2D4) = 1;
            *(int*)((char*)self + 0x2DC) = 0;
        }
    } else if (D_004A259C >= 0) {
        if (D_004A259C < 3) {
            if (*(int*)((char*)self + 0x2D4) != D_004A25A4 + 2) {
                cFEAsyncManager_UnloadFEAsyncFile(mgr, *(int*)((char*)self + 0x2D8));
                cFEAsyncManager_Load1PeakPic(mgr, D_004A25A4, *(int*)((char*)self + 0x2D8), 1);
                int v = D_004A25A4 + 2;
                *(int*)((char*)self + 0x2DC) = 0;
                *(int*)((char*)self + 0x2D4) = v;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPopup);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046FAA0[];
extern char D_004706B8[];
extern char D_004706C8[];
extern char D_00470700[];
extern char D_00470A20[];
extern char D_00470A60[];
extern char D_00470A70[];
extern char D_00470A80[];
extern char D_00470A98[];
extern char D_00470AA8[];
extern char D_004A25B0[];
extern char D_004A2638[];
extern char D_004A2640[];
extern int D_004A259C;

class cUIObj_203A58 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
};

extern "C" void cUITemplate_MAP_setupPopup(char* self)
{
    if (*(int*)(self + 0x2CC) != 0) {
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470A60);
        void* menu = cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470A20));
        if (menu != 0)
            cUIMenu_setSelectedByIndex(menu, 0);
        if (*(int*)(self + 0x2CC) != 0) {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A2638);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A2640);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
            cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470A70));
            if (D_004A259C == 0) {
                if (t != 0)
                    cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470A80));
            } else {
                if (t != 0)
                    cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470A98));
            }
        }
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470700);
        cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_004706B8));
        char* o = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_004706C8));
        if (*(int*)(self + 0x2D0) == 1) {
            if (title != 0)
                cUIText_setUnicodeStringByID(title, GetHashValue32(D_0046FAA0));
            if (o != 0) {
                ((cUIObj_203A58*)o)->v09(0);
                ((cUIObj_203A58*)o)->v08(1);
            }
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004706C8);
        } else {
            if (title != 0)
                cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470AA8));
            if (o != 0) {
                ((cUIObj_203A58*)o)->v09(1);
                ((cUIObj_203A58*)o)->v08(0);
            }
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004706C8);
        }
    } else {
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470A60);
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A25B0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setShowInfo);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_00470AB8[];
extern char D_00470AC8[];
extern char D_00470AD8[];

extern "C" void cUITemplate_MAP_setShowInfo(void* self, int show)
{
    *(int*)self = show;
    cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x2E8), GetHashValue32(D_00470AB8));
    if (*(int*)self != 0) {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470AC8));
    } else {
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470AD8));
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakInfo);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.
template <class F> struct sVEntry {
    short delta;
    short index;
    F* fn;
};

// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.
struct cUIText;

int GetHashValue32(char* str);

extern "C" void cUIState_showObjSafe(void* self, char* name);

extern "C" void cUIState_hideObjSafe(void* self, char* name);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int D_004A2594;

extern int D_004A259C;

extern void* D_004A28A8;

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).
extern sPeakGoal D_004781D0_Info[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_Info[3][5] __asm__("D_004786E0");

// The text database (D_004A28A8 + 0x8C): localised strings by hash through vtable slot 4 (as in cUITemplate_MAP_getMedalStr).
struct cTextDB_5098 {
    int pad;
    sVEntry<unsigned short*(void*, int)>* vt;
};

#define TextDB_5098 (*(cTextDB_5098**)((char*)D_004A28A8 + 0x8C))

// A macro, not an inline helper: an inline's key parameter is a pseudo that loop.c hoists out of case 0's loop.
#define Loc_5098(key) (TextDB_5098->vt[4].fn((char*)TextDB_5098 + TextDB_5098->vt[4].delta, GetHashValue32(key)))

extern char D_004A25D8[];

extern char D_00470548[];

extern char D_00470558[];

extern char D_00470568[];

extern char D_00470578[];

int cBELibrary_getCharacterID(int);

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern char D_0046F9E0_Info[] __asm__("D_0046F9E0");
extern char D_00470600_Info[] __asm__("D_00470600");
extern char D_004706D8_Info[] __asm__("D_004706D8");
extern char D_004707A0_Info[] __asm__("D_004707A0");
extern char D_004707C0_Info[] __asm__("D_004707C0");
extern char D_004707D8_Info[] __asm__("D_004707D8");
extern char D_004707E8_Info[] __asm__("D_004707E8");
extern char D_004707F8_Info[] __asm__("D_004707F8");

extern char D_004A25B0_Info[] __asm__("D_004A25B0");
extern char D_004A2608_Info[] __asm__("D_004A2608");
extern char D_004A2610_Info[] __asm__("D_004A2610");
extern char D_004A2618_Info[] __asm__("D_004A2618");
extern char D_004A2620_Info[] __asm__("D_004A2620");
extern char D_004A2628_Info[] __asm__("D_004A2628");
extern char D_004A2630_Info[] __asm__("D_004A2630");

extern char D_004A2610_Info[] __asm__("D_004A2610");

extern "C" int cBERewardInterface_isTrophyOwned(void* self, int a, int b, int c, int i);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int kind, int track);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString_Info(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int peak);
extern "C" int cBERewardInterface_getChallengesMedalTarget(void* self, int peak, int tier);
extern "C" int cBERewardInterface_getCollectibleMedalTarget(void* self, int peak, int tier);
int cBERewardInterface_getEarningsTarget(void* self, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
int cBEScoreInterface_getCurrentHighlightLevel(void* self, int i);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern char D_00470AE8[], D_004A2648[], D_00470AF8[], D_00470B08[], D_004A2650[], D_00470B20[], D_00470610[];
extern char D_004A2658[], D_00470B38[], D_00470B50[], D_00470B68[], D_00470B80[], D_00470B98[], D_00470BB0[];
extern char D_00470BC8[], D_00470BE0[], D_00470BF8[], D_00470C10[], D_00470C20[], D_00470C30[], D_00470C48[];
extern char D_00470C60[], D_00470C70[], D_00470C88[], D_00470C98[], D_00470CB0[], D_00470CC8[], D_00470CD8[];
extern char D_00470CE8[], D_00470D00[], D_00470D10[], D_00470D28[], D_00470D40[], D_00470D58[], D_00470D68[];
extern char D_00470D80[], D_00470D90[], D_00470DA8[], D_00470DC0[], D_00470DD8[], D_00470DE8[], D_00470E00[];
extern char D_004A2398_Info[] __asm__("D_004A2398");
extern char D_004A2660[];
extern char D_004A2668[];
extern char D_004A2670[];
extern char D_004A2678[];
extern char D_004A2680[];
extern char D_004A2688[];
extern char D_004A2690[];

// The peak list entry selected in the map menu (at widget + 0xA0).
struct sPeakSel_MAP {
    char pad[0x18];
    int peak;           // 0x18
};

struct sPeakInfo_MAP {
    int pad0;
    int unlocked[3];            // 0x4, indexed by peak
    char pad10[0x2CC - 0x10];
    int popup;                  // 0x2CC
    char pad2D0[0x2E8 - 0x2D0];
    void* screen;               // 0x2E8
    void* owner;                // 0x2EC
};



// Goal kind test; the kind constant is taken by const reference.
static inline int isKind(int kind, const int& k) { return kind == k; }
extern "C" void cUITemplate_MAP_setupPeakInfo(void* tmpl)
{
    sPeakInfo_MAP* self = (sPeakInfo_MAP*)tmpl;
    void* rew = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    void* mis = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    int ch = cBELibrary_getCharacterID(0);
    if (self->popup != 0) {
        cUIState_hideObjSafe(self->owner, D_00470AE8);
        cUIState_hideObjSafe(self->owner, D_004A2648);
        cUIState_hideObjSafe(self->owner, D_004A2610_Info);
    } else {
        cUIState_showObjSafe(self->owner, D_00470AE8);
        cUIState_showObjSafe(self->owner, D_004A2648);
        cUIState_showObjSafe(self->owner, D_004A2610_Info);
    }
    cUIState_hideObjSafe(self->owner, D_00470AF8);
    cUIState_hideObjSafe(self->owner, D_00470B08);
    cUIState_hideObjSafe(self->owner, D_004A2650);
    char name1[32];
    char name2[32];
    for (int i = 0; i < 3; i++) {
        sprintf(name1, D_00470B20, i + 1);
        if (cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 0) && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 1)
            && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 2) && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 3)) {
            cUIState_showObjSafe(self->owner, name1);
        } else {
            cUIState_hideObjSafe(self->owner, name1);
        }
    }
    if (D_004A2594 < 2) {
        cUIThing* o = (cUIThing*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470610));
        o->setStatic(1);
        o->setVisible(0);
    }
    cUIState_hideObjSafe(self->owner, D_004A25D8);
    cUIState_hideObjSafe(self->owner, D_00470548);
    cUIState_hideObjSafe(self->owner, D_00470558);
    for (int i = 0; i < 3; i++) {
        sprintf(name1, D_004A2658, i + 1);
        sprintf(name2, D_00470B38, i + 1);
        if (D_004A259C == 0 && self->unlocked[i] == 0) {
            cUIState_hideObjSafe(self->owner, name2);
        } else {
            cUIState_hideObjSafe(self->owner, name1);
            if (D_004A2594 < 2)
                cUIState_hideObjSafe(self->owner, name2);
        }
    }
    cUIThing* title = (cUIThing*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470578));
    title->setVisible(0);
    char* list = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470600_Info));
    if (list == 0)
        return;
    sPeakSel_MAP* sel = *(sPeakSel_MAP**)(list + 0xA0);
    if (sel == 0)
        return;
    if (sel->peak < 3) {
        cUIState_hideObjSafe(self->owner, D_00470568);
        int peak = sel->peak;
        title->setVisible(1);
        if (self->unlocked[peak] != 0) {
            if (D_004A2594 < 2)
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B50));
            else
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B68));
        } else if (peak == 1) {
            if (D_004A2594 < 2)
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B80));
            else
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B98));
        } else if (D_004A2594 < 2) {
            cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470BB0));
        } else {
            cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470BC8));
        }
        if (D_004A2594 < 2)
            return;
        unsigned short wbuf[200];
        int cnt = 4;
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470BE0));
        int found = 0;
        unsigned short* fmt = Loc_5098(D_00470BF8);
        sprintf1(wbuf, fmt, peak + 1);
        cUIText_setUnicodeString_Info(t, wbuf);
        if (peak == 2)
            cnt = 3;
        for (int k = 0; k < cnt; k++) {
            sPeakGoal g = D_004781D0_Info[peak][k];
            if (cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track) != -1)
                found++;
        }
        char buf[32];
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C10));
        sprintf(buf, D_004A2398_Info, found, cnt);
        cUIText_setAsciiString(t, buf);
        if (found >= cnt)
            cUIState_showObjSafe(self->owner, D_00470C20);
        else
            cUIState_hideObjSafe(self->owner, D_00470C20);
        int found2 = 0;
        for (int k = 0; k < 5; k++) {
            sPeakGoal g = D_004786E0_Info[peak][k];
            if (cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track) != -1)
                found2++;
        }
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C30));
        sprintf(buf, D_004A2660, found2);
        cUIText_setAsciiString(t, buf);
        if (found2 >= 5)
            cUIState_showObjSafe(self->owner, D_00470C48);
        else
            cUIState_hideObjSafe(self->owner, D_00470C48);
        int medals = 0;
        if (cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, peak) >= cBERewardInterface_getChallengesMedalTarget(rew, peak, 3))
            medals++;
        if (cBEMissionInterface_getCurrentCollectForPeak(mis, 0, peak) >= cBERewardInterface_getCollectibleMedalTarget(rew, peak, 3))
            medals++;
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C60));
        sprintf(buf, D_004A2668, medals);
        cUIText_setAsciiString(t, buf);
        if (medals >= 2)
            cUIState_showObjSafe(self->owner, D_00470C70);
        else
            cUIState_hideObjSafe(self->owner, D_00470C70);
        unsigned int money = cBEEconInterface_getTotalMoneyEarned1(econ, 0);
        int ok = cBERewardInterface_getEarningsTarget(rew, peak) < money;
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C88));
        sprintf(buf, D_004A2670, ok);
        cUIText_setAsciiString(t, buf);
        if (ok > 0)
            cUIState_showObjSafe(self->owner, D_00470C98);
        else
            cUIState_hideObjSafe(self->owner, D_00470C98);
    } else {
        int done = 0;
        cUIState_showObjSafe(self->owner, D_00470568);
        cUIState_hideObjSafe(self->owner, D_004A2608_Info);
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470CB0));
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470CC8));
        for (int p = 0; p < 3; p++) {
            int cnt = 4;
            if (p == 2)
                cnt = 3;
            for (int k = 0; k < cnt; k++) {
                sPeakGoal g = D_004781D0_Info[p][k];
                if (!isKind(g.kind, 4) && !isKind(g.kind, 6) && !isKind(g.kind, 7) && !isKind(g.kind, 8)) {
                    if ((unsigned)cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track) < 2)
                        done++;
                }
            }
            for (int k = 0; k < 5; k++) {
                sPeakGoal g = D_004786E0_Info[p][k];
                if (!isKind(g.kind, 5) && !isKind(g.kind, 9) && !isKind(g.kind, 10) && !isKind(g.kind, 11)) {
                    if ((unsigned)cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track) < 2)
                        done++;
                }
            }
        }
        char buf[32];
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470CD8));
        sprintf(buf, D_004A2678, done);
        cUIText_setAsciiString(t, buf);
        if (done >= 14)
            cUIState_showObjSafe(self->owner, D_00470CE8);
        else
            cUIState_hideObjSafe(self->owner, D_00470CE8);
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D00));
        int gold = 0;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 6, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 9, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 7, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 10, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 8, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 11, 0x10) != -1)
            gold++;
        sprintf(buf, D_004A2680, gold);
        cUIText_setAsciiString(t, buf);
        if (gold >= 12)
            cUIState_showObjSafe(self->owner, D_00470D10);
        else
            cUIState_hideObjSafe(self->owner, D_00470D10);
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D28));
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470D40));
        int cur = 0;
        int tot = 0;
        for (int p = 0; p < 3; p++) {
            cur += cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, p);
            tot += cBEMissionInterface_getNumberMissionsOnPeak(mis, p);
        }
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D58));
        sprintf(buf, D_004A2398_Info, cur, tot);
        cUIText_setAsciiString(t, buf);
        if (cur >= tot)
            cUIState_showObjSafe(self->owner, D_00470D68);
        else
            cUIState_hideObjSafe(self->owner, D_00470D68);
        int col = 0;
        int colTot = 0;
        for (int p = 0; p < 3; p++) {
            col += cBEMissionInterface_getCurrentCollectForPeak(mis, 0, p);
            colTot += cBEMissionInterface_getTotalCollectForPeak(mis, p);
        }
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D80));
        sprintf(buf, D_004A2398_Info, col, colTot);
        cUIText_setAsciiString(t, buf);
        if (col >= colTot)
            cUIState_showObjSafe(self->owner, D_00470D90);
        else
            cUIState_hideObjSafe(self->owner, D_00470D90);
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470DA8));
        cUIText_setUnicodeStringByID(t, GetHashValue32(D_00470DC0));
        int hl = 0;
        for (int p = 0; p < 8; p++)
            hl += cBEScoreInterface_getCurrentHighlightLevel(score, p);
        sprintf(buf, D_004A2688, hl);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470DD8)), buf);
        if (hl >= 24)
            cUIState_showObjSafe(self->owner, D_00470DE8);
        else
            cUIState_hideObjSafe(self->owner, D_00470DE8);
        float pct = ((float)done * (1.0f / 14.0f) + (float)gold * (1.0f / 12.0f) + (float)cur / (float)tot + (float)col / (float)colTot + (float)hl * (1.0f / 24.0f)) * 20.0f;
        int pctInt = 100;
        if (pct != 100.0f) {
            pctInt = (int)pct;
            if (pct >= 99.0f)
                pctInt = 99;
        }
        t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470E00));
        sprintf(buf, D_004A2690, pctInt);
        cUIText_setAsciiString(t, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_getMedalStr);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern void* D_004A28A8;
extern char D_00470E10[];
extern char D_00470E20[];
extern char D_00470E30[];
extern char D_00470E40[];

struct sVEntry00204FF0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int setPic00204FF0(char* name)
{
    void* obj = *(void**)((char*)D_004A28A8 + 0x8C);
    sVEntry00204FF0* vt = *(sVEntry00204FF0**)((char*)obj + 4);
    return vt[4].fn((char*)obj + vt[4].delta, GetHashValue32(name));
}

extern "C" int cUITemplate_MAP_getMedalStr(void* self, int peak)
{
    switch (peak) {
    case 0:
    default:
        return setPic00204FF0(D_00470E10);
    case 1:
        return setPic00204FF0(D_00470E20);
    case 2:
        return setPic00204FF0(D_00470E30);
    case 3:
        return setPic00204FF0(D_00470E40);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakGoals);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.

struct cUIText;

int GetHashValue32(char* str);

extern "C" void cUIState_showObjSafe(void* self, char* name);

extern "C" void cUIState_hideObjSafe(void* self, char* name);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int D_004A2594;

extern int D_004A259C;

extern void* D_004A28A8;

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern int D_004A25A4;

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).

extern sPeakGoal D_004781D0_oals[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_oals[3][5] __asm__("D_004786E0");

// The text database (D_004A28A8 + 0x8C): localised strings by hash through vtable slot 4 (as in cUITemplate_MAP_getMedalStr_oals).

#define TextDB_5098 (*(cTextDB_5098**)((char*)D_004A28A8 + 0x8C))

// A macro, not an inline helper: an inline's key parameter is a pseudo that loop.c hoists out of case 0's loop.
#define Loc_5098(key) (TextDB_5098->vt[4].fn((char*)TextDB_5098 + TextDB_5098->vt[4].delta, GetHashValue32(key)))

extern char D_004A25D8[];

extern char D_00470548[];

extern char D_00470558[];

extern char D_00470578[];

int cBELibrary_getCharacterID(int);

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern char D_0046F9E0_oals[] __asm__("D_0046F9E0");
extern char D_00470600_oals[] __asm__("D_00470600");
extern char D_004706D8_oals[] __asm__("D_004706D8");
extern char D_004707A0_oals[] __asm__("D_004707A0");
extern char D_004707C0_oals[] __asm__("D_004707C0");
extern char D_004707D8_oals[] __asm__("D_004707D8");
extern char D_004707E8_oals[] __asm__("D_004707E8");
extern char D_004707F8_oals[] __asm__("D_004707F8");

// (prototype from their definition at line 1392)
extern "C" int cUITemplate_MAP_getMedalStr_oals(void* self, int peak) __asm__("cUITemplate_MAP_getMedalStr");

extern "C" void cUITemplate_MAP_showEventPath_oals(void* tmpl, char* name) __asm__("cUITemplate_MAP_showEventPath");
extern "C" int func_004165A8(const char*, const char*);
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int a, int b, int c, int i);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int kind, int track);
extern "C" const char* cBENewRaceInterface_getLocationName_oals(void* iface, int id) __asm__("cBENewRaceInterface_getLocationName");
extern "C" unsigned short* func_002C2540_oals(unsigned short* dst, const char* s) __asm__("func_002C2540");
extern "C" unsigned short* strcpy1_oals(unsigned short* dst, const unsigned short* src) __asm__("strcpy1");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void sprintf2(unsigned short* dst, const char* fmt, ...);
extern "C" unsigned short* func_001FD190(int kind);
extern "C" void cUIText_setUnicodeString_oals(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int peak);
extern "C" int cBERewardInterface_getChallengesMedalTarget(void* self, int peak, int tier);
extern "C" int cBERewardInterface_getCollectibleMedalTarget(void* self, int peak, int tier);
int cBERewardInterface_getEarningsTarget(void* self, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
extern "C" const char* getDollarString(int value);
extern char D_004A2550[];
extern char D_004A2560_oals[] __asm__("D_004A2560");
extern char D_004A23C0[];
extern char D_004A2698[];
extern char D_004A26A0_oals[] __asm__("D_004A26A0");
extern char D_0046E548_oals[] __asm__("D_0046E548");
extern char D_00470E50[], D_00470E68[], D_00470E80[], D_00470E98[], D_00470EB0[], D_00470EC0[], D_00470ED0[];
extern char D_00470EE0[], D_00470EF0[], D_00470F00[], D_00470F18[], D_00470F28[], D_00470F38[], D_00470F48[];
extern char D_00470F58[], D_00470F70[], D_00470F90[], D_00470FA8[], D_00470FC0[], D_00470FD0[], D_00470FE0[];
extern char D_00470FF0[], D_00471000[], D_00471010[], D_00471020[], D_00471030[], D_00471040[], D_00471058[];
extern char D_00471068[], D_00471078[], D_00471088[], D_004710A0[], D_004710B8[], D_004710D0[], D_004710F0[];
extern char D_00471108[], D_00471128[], D_00471140[], D_00471158[], D_00471170[], D_00471188[], D_004711A0[];
extern char D_004711B8[], D_004711C8[], D_004711D8[], D_004711E8[], D_00471200[], D_00471218[], D_00471230[];
extern char D_00471248[], D_00471260[], D_00471278[], D_00471298[], D_004712B0[], D_004712C8[], D_004712E0[];
extern char D_004712F0[], D_00471300[], D_00471318[], D_00471330[];

extern "C" char* strcpy(char* d, const char* s);

extern "C" void cUITemplate_MAP_setupPeakGoals(void* tmpl)
{
    char* self = (char*)tmpl;
    unsigned short wbuf[200];
    char buf[32];
    cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470578));
    if (D_004A2594 < 2) {
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470E50);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470E68);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470E80);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470E98);
    }
    cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004A25D8);
    if (D_004A2594 == 0) {
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470558);
    } else {
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470548);
    }
    cUIText* name = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_004A25D8));
    if (D_004A25A4 == 0) {
        cUIText_setUnicodeStringByID(name, GetHashValue32(D_00470EB0));
    } else if (D_004A25A4 == 1) {
        cUIText_setUnicodeStringByID(name, GetHashValue32(D_00470EC0));
    } else {
        cUIText_setUnicodeStringByID(name, GetHashValue32(D_00470ED0));
    }
    cUIThing* o;
    unsigned char b = 0;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* rew = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    void* mis = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    int ch = cBELibrary_getCharacterID(0);
    sprintf(buf, D_004706D8_oals, D_004A25A4 + 1);
    char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(buf));
    if (obj) b = obj[0x95];
    if (D_004A259C == 1) {
        char path[24];
        if (b == 0) {
            sprintf(path, D_00470EE0, D_004A25A4 + 1);
            cUITemplate_MAP_showEventPath_oals(self, path);
        } else if (b == 1) {
            sprintf(path, D_00470EF0, D_004A25A4 + 1);
            cUITemplate_MAP_showEventPath_oals(self, path);
        } else {
            sprintf(path, D_00470F00, D_004A25A4 + 1);
            cUITemplate_MAP_showEventPath_oals(self, path);
        }
    }
    cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470F18);
    cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470F28);
    cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470F38);
    cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470F48);
    if (D_004A2594 < 2) {
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470F58));
        return;
    }
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, D_004A25A4, 0))
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470F18);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, D_004A25A4, 1))
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470F28);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, D_004A25A4, 2))
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470F38);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, D_004A25A4, 3))
        cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00470F48);
    switch (b) {
    case 0: {
        int cnt = 4;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470F70));
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470F90));
        sprintf1(wbuf, Loc_5098(D_00470FA8), D_004A25A4 + 1);
        cUIText_setUnicodeString_oals(o, wbuf);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470FC0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470FD0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470FE0);
        cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00470FF0);
        if (D_004A25A4 == 2) {
            cnt = 3;
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471000);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471010);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471020);
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471030);
        }
        int done = 0;
        for (int k = 0; k < cnt; k++) {
            char name1[24];
            unsigned short ev[50];
            unsigned short medal[50];
            sPeakGoal g = D_004781D0_oals[D_004A25A4][k];
            if (g.useTrackName) {
                sprintf2(ev, cBENewRaceInterface_getLocationName_oals(race, g.track));
            } else if (func_004165A8(g.name, D_004A2560_oals)) {
                func_002C2540_oals(ev, g.name);
            } else if (g.kind == 4) {
                unsigned short fmt[20];
                func_002C2540_oals(fmt, D_004A2698);
                sprintf1(ev, fmt, cBENewRaceInterface_getLocationName_oals(race, g.track), Loc_5098(D_0046E548_oals));
            } else {
                strcpy1_oals(ev, func_001FD190(g.kind));
            }
            sprintf(name1, D_00471040, k);
            char name2[24];
            sprintf(name2, D_00471058, k);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), name2);
            char name3[24];
            sprintf(name3, D_00471068, k);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), name3);
            func_002C2540_oals(medal, D_004A2560_oals);
            int m = cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track);
            if (m != -1) {
                unsigned short fmt[8];
                done++;
                func_002C2540_oals(fmt, D_004A23C0);
                sprintf1(medal, fmt, cUITemplate_MAP_getMedalStr_oals(self, m));
                cUIState_showObjSafe(*(void**)(self + 0x2EC), name1);
            } else {
                cUIState_hideObjSafe(*(void**)(self + 0x2EC), name1);
            }
            char name4[24];
            sprintf(name4, D_00471078, k);
            o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(name4));
            unsigned short fmt[8];
            func_002C2540_oals(fmt, D_004A2550);
            sprintf1(wbuf, fmt, ev, medal);
            cUIText_setUnicodeString_oals(o, wbuf);
        }
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471088));
        sprintf1(wbuf, Loc_5098(D_004710A0), done, cnt);
        cUIText_setUnicodeString_oals(o, wbuf);
        break;
    }
    case 1: {
        int done = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_004710B8));
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00470F90));
        sprintf1(wbuf, Loc_5098(D_004710D0), D_004A25A4 + 1);
        cUIText_setUnicodeString_oals(o, wbuf);
        for (int k = 0; k < 5; k++) {
            unsigned short ev[50];
            unsigned short medal[50];
            sPeakGoal g = D_004786E0_oals[D_004A25A4][k];
            if (g.useTrackName) {
                sprintf2(ev, cBENewRaceInterface_getLocationName_oals(race, g.track));
            } else if (func_004165A8(g.name, D_004A2560_oals)) {
                func_002C2540_oals(ev, g.name);
            } else if (g.kind == 5) {
                unsigned short fmt[20];
                func_002C2540_oals(fmt, D_004A26A0_oals);
                sprintf1(ev, fmt, cBENewRaceInterface_getLocationName_oals(race, g.track));
            } else {
                strcpy1_oals(ev, func_001FD190(g.kind));
            }
            char name1[24];
            sprintf(name1, D_00471040, k);
            char name2[24];
            sprintf(name2, D_00471058, k);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), name2);
            char name3[24];
            sprintf(name3, D_00471068, k);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), name3);
            func_002C2540_oals(medal, D_004A2560_oals);
            int m = cBERewardInterface_getTrackMedal(rew, 0, ch, g.kind, g.track);
            char medalFmt[16];
            if (m != -1) {
                strcpy(medalFmt, "[%s]");
                done++;
                sprintf2(medal, medalFmt, cUITemplate_MAP_getMedalStr_oals(self, m));
                cUIState_showObjSafe(*(void**)(self + 0x2EC), name1);
            } else {
                cUIState_hideObjSafe(*(void**)(self + 0x2EC), name1);
            }
            char name4[24];
            sprintf(name4, D_00471078, k);
            o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(name4));
            char fmt[16];
            strcpy(fmt, "%s %s");
            sprintf2(wbuf, fmt, ev, medal);
            cUIText_setUnicodeString_oals(o, wbuf);
            o->setVisible(1);
            char name5[24];
            sprintf(name5, D_004710F0, k);
            cUIState_showObjSafe(*(void**)(self + 0x2EC), name5);
        }
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471088));
        sprintf1(wbuf, Loc_5098(D_004710A0), done, 5);
        cUIText_setUnicodeString_oals(o, wbuf);
        break;
    }
    case 2: {
        int count = 0;
        int ok = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00471108));
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471128));
        sprintf1(wbuf, Loc_5098(D_00471140), D_004A25A4 + 1);
        cUIText_setUnicodeString_oals(o, wbuf);
        int cur = cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, D_004A25A4);
        int tot = cBEMissionInterface_getNumberMissionsOnPeak(mis, D_004A25A4);
        sprintf1(wbuf, Loc_5098(D_00471158), cur, tot);
        cUIText_setUnicodeString_oals(cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471170)), wbuf);
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471188));
        if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 0)) {
            o->setVisible(0);
            ok = 1;
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 1)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711A0), cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 0));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 2)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711B8), cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 1));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 3)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711C8), cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 2));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else {
            sprintf1(wbuf, Loc_5098(D_004711D8), cBERewardInterface_getChallengesMedalTarget(rew, D_004A25A4, 3));
            cUIText_setUnicodeString_oals(o, wbuf);
        }
        if (ok) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_004711E8);
            count++;
        } else {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_004711E8);
        }
        ok = 0;
        cur = cBEMissionInterface_getCurrentCollectForPeak(mis, 0, D_004A25A4);
        tot = cBEMissionInterface_getTotalCollectForPeak(mis, D_004A25A4);
        sprintf1(wbuf, Loc_5098(D_00471200), cur, tot);
        cUIText_setUnicodeString_oals(cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471218)), wbuf);
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471230));
        if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 0)) {
            o->setVisible(0);
            ok = 1;
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 1)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711A0), cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 0));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 2)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711B8), cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 1));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 3)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711C8), cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 2));
            cUIText_setUnicodeString_oals(o, wbuf);
        } else {
            sprintf1(wbuf, Loc_5098(D_004711D8), cBERewardInterface_getCollectibleMedalTarget(rew, D_004A25A4, 3));
            cUIText_setUnicodeString_oals(o, wbuf);
        }
        if (ok) {
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00471248);
            count++;
        } else {
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471248);
        }
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471260));
        sprintf1(wbuf, Loc_5098(D_004710A0), count, 2);
        cUIText_setUnicodeString_oals(o, wbuf);
        break;
    }
    case 3: {
        unsigned short num[16];
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00471278));
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471298));
        sprintf1(wbuf, Loc_5098(D_004712B0), D_004A25A4 + 1);
        cUIText_setUnicodeString_oals(o, wbuf);
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_004712C8));
        int cost = cBERewardInterface_getEarningsTarget(rew, D_004A25A4);
        unsigned short* fmt = Loc_5098(D_004712E0);
        func_002C2540_oals(num, getDollarString(cost));
        sprintf1(wbuf, fmt, num);
        cUIText_setUnicodeString_oals(o, wbuf);
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_004712F0));
        int money = cBEEconInterface_getTotalMoneyEarned1(econ, 0);
        fmt = Loc_5098(D_00471300);
        func_002C2540_oals(num, getDollarString(money));
        sprintf1(wbuf, fmt, num);
        cUIText_setUnicodeString_oals(o, wbuf);
        int ok;
        if (cost > money)
            ok = 0;
        else
            ok = 1;
        o = (cUIThing*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(D_00471318));
        sprintf1(wbuf, Loc_5098(D_004710A0), ok, 1);
        cUIText_setUnicodeString_oals(o, wbuf);
        if (ok > 0)
            cUIState_showObjSafe(*(void**)(self + 0x2EC), D_00471330);
        else
            cUIState_hideObjSafe(*(void**)(self + 0x2EC), D_00471330);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_showEventPath);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const char* a, const char* b);
extern int D_004A25A4;
extern char D_004A2560[];
extern char D_004714B0[];
extern char D_004714C0[];
extern char D_004714D0[];
extern char D_004714E0[];
extern char D_004714F0[];
extern char D_00471500[];
extern char D_00471510[];
extern char D_00471520[];
extern char D_00471530[];
extern char D_00471548[];
extern char D_00471558[];
extern char D_00471568[];
extern char D_00471580[];
extern char D_00471590[];
extern char D_004715A0[];
extern char* D_004713A8[];
extern char* D_00471420[];
extern char* D_00471488[];
extern char* D_004714A0[];
extern char* D_004A26A8[];
extern char* D_004A26B0[];
extern char* D_004A26B8[];
extern char* D_004A26C0[];
extern char* D_004A26C8[];
extern char D_00478225[][0x6C];
extern char D_00478735[][0x6C];
extern char D_004783D5[][0x6C];
extern char D_00478951[][0x6C];
extern char D_00478585[][0x6C];
extern char D_00478B6D[][0x6C];

struct sNameList2_206690 {
    char* s[2];
};
struct sNameList3_206690 {
    char* s[3];
};
struct sNameList5_206690 {
    char* s[5];
};
struct sNameList6_206690 {
    char* s[6];
};

// Recolours the map's event-path objects for the event or peak called `name`: objects on the
// path get col0, the rest col1. The name lists are local copies of constant tables.
extern "C" void cUITemplate_MAP_showEventPath(void* tmpl, char* name)
{
    sMapTmpl_200700* self = (sMapTmpl_200700*)tmpl;
    sColor_202828 col0;
    col0.a = 1.0f;
    col0.r = 0.8901961445808411f;
    col0.g = 0.16470588743686676f;
    col0.b = 0.03529411926865578f;
    sColor_202828 col1;
    col1.a = 1.0f;
    col1.r = 0.8705883026123047f;
    col1.g = 0.5176470875740051f;
    col1.b = 0.13333334028720856f;
    sNameList6_206690 A = *(sNameList6_206690*)D_004713A8;
    sNameList6_206690 B = *(sNameList6_206690*)D_00471420;
    sNameList5_206690 C = *(sNameList5_206690*)D_00471488;
    sNameList3_206690 D = *(sNameList3_206690*)D_004714A0;
    sNameList2_206690 E = *(sNameList2_206690*)D_004A26A8;
    sNameList2_206690 F = *(sNameList2_206690*)D_004A26B0;
    sNameList2_206690 G = *(sNameList2_206690*)D_004A26B8;
    sNameList2_206690 H = *(sNameList2_206690*)D_004A26C0;
    sNameList2_206690 I = *(sNameList2_206690*)D_004A26C8;
    cUIPic_202828* obj;


    if (cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name)) != 0
        || func_004165A8(name, D_004A2560) == 0) {
        switch (D_004A25A4) {
        case 0:
            for (int i = 0; i < 6; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
                if (obj != 0) {
                    if (func_004165A8(A.s[i], name) == 0) {
                        obj->v11(&col0);
                    } else {
                        obj->v11(&col1);
                    }
                }
            }
            break;
        case 1:
            for (int i = 0; i < 6; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
                if (obj != 0) {
                    if (func_004165A8(B.s[i], name) == 0) {
                        obj->v11(&col0);
                    } else {
                        obj->v11(&col1);
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < 5; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
                if (obj != 0) {
                    if (func_004165A8(C.s[i], name) == 0) {
                        obj->v11(&col0);
                    } else {
                        obj->v11(&col1);
                    }
                }
            }
            break;
        }
    } else if (func_004165A8(name, D_004714B0) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 3; j++) {
                    if (func_004165A8(A.s[i], D.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_004714C0) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(A.s[i], E.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_004714D0) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(B.s[i], F.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_004714E0) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(B.s[i], G.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_004714F0) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(C.s[i], H.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_00471500) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(C.s[i], I.s[j]) == 0) {
                        obj->v11(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, D_00471510) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 3; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478225[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471520) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478735[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471530) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(A.s[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471548) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 3; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004783D5[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471558) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478951[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471568) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(B.s[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471580) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 2; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478585[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_00471590) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
            if (obj != 0) {
                obj->v11(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478B6D[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    } else if (func_004165A8(name, D_004715A0) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(C.s[i]));
            if (obj != 0) {
                obj->v11(&col0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_00207430);
#ifdef SKIP_ASM
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cUIText_setAsciiString(cUIText* text, const char* str);
int cBERewardInterface_getTrackMedal(void* iface, int player, int charID, int type, int event);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* cBENewRaceInterface_getLocationName(void* iface, int event);
extern "C" int func_00145870(void* iface, int event);
extern "C" int func_00153350(void* iface, int event);
extern "C" int func_00153520(void* iface, int a, int event);
extern "C" int cBEMissionInterface_getNumberMissionsOnTrack(void* iface, int event);
extern "C" int func_001542E0(void* iface, int event);
extern "C" int func_001550E8(void* iface, int a, int event, int type);
extern "C" char* func_0020A1A8(int t);
extern "C" void strcpy1(void* dst, void* src);
extern "C" void func_002C2540(void* dst, char* src);
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeStringWrap(void* text, void* wstr);
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern "C" void cUITemplate_MAP_showEventPath(void* tmpl, char* name);

struct sRewardDesc_0388 {
    int w[0x6C / 4];
};
extern sRewardDesc_0388 D_004781D0[][4];
extern sRewardDesc_0388 D_004786E0[][5];
extern sRewardDesc_0388 D_00478D38[][8];

struct sMapTmpl_207430 {
    int showInfo;                 // 0x0
    int peakUnlocked[3];          // 0x4
    int goalUnlocked[4];          // 0x10
    int eventUnlocked[3][3][8];   // 0x20 [peak][goal][event]
    int eventDone[3][3][8];       // 0x140
    sRewardDesc_0388 desc;        // 0x260
    int popup;                    // 0x2CC
    int mode;                     // 0x2D0
    int pic;                      // 0x2D4
    int file;                     // 0x2D8
    int f2DC;
    int f2E0;
    void* map;                    // 0x2E4
    void* screen;                 // 0x2E8
    void* owner;                  // 0x2EC
};

struct sLocVEntry_207430 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern int D_004A25A0;
extern int D_004A25A4;
extern char D_004715B8[];
extern char D_004A2600[];
extern char D_004715D0[];
extern char D_004715E8[];
extern char D_004715F8[];
extern char D_00471608[];
extern char D_00471628[];
extern char D_004A26D0[];
extern char D_00471640[];
extern char D_00471650[];
extern char D_00471668[];
extern char D_00471680[];
extern char D_00471698[];
extern char D_004A2590[];
extern char D_004716B0[];
extern char D_004A26A0[];
extern char D_0046E548[];
extern char D_0046E398[];
extern char D_0046E3A8[];
extern char D_004716C8[];
extern char D_0046E5F0[];
extern char D_004716D8[];
extern char D_004716E8[];
extern char D_004A26D8[];
extern char D_004716F8[];
extern char D_00471710[];
extern char D_00471720[];
extern char D_00471738[];
extern char D_004A2398[];
extern char D_00471748[];
extern char D_00471758[];
extern char D_00471770[];
extern char D_004A26E0[];
extern char D_00471780[];
extern char D_00471790[];
extern char D_004717A0[];
extern char D_004717B0[];
extern char D_004A2108[];
extern char D_004A2138[];

static inline void* getObj_207430(sMapTmpl_207430* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
}

// PORT: the unit declares func_00207430(void*).
extern "C" void func_00207430_r(sMapTmpl_207430* self) __asm__("func_00207430");

extern "C" void func_00207430_r(sMapTmpl_207430* self)
{
    char buf[200];
    sRewardDesc_0388 desc;
    char name[32];
    cUIObj_200700* mapObj = (cUIObj_200700*)getObj_207430(self, D_00470578);
    if (D_004A259C == 2)
        mapObj->v09(1);
    void* mission = cBE_getInterface_Fv(cBE_getBE(), 0xA);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    int charID = cBELibrary_getCharacterID(0);
    if (self->popup != 0) {
        cUIState_hideObjSafe(self->owner, D_004715B8);
        cUIState_hideObjSafe(self->owner, D_004A2600);
    } else {
        cUIState_showObjSafe(self->owner, D_004715B8);
        cUIState_showObjSafe(self->owner, D_004A2600);
    }
    if (D_004A259C == 2) {
        cUIState_showObjSafe(self->owner, D_004A25D8);
        if (D_004A2594 == 0)
            cUIState_showObjSafe(self->owner, D_00470558);
        else
            cUIState_showObjSafe(self->owner, D_00470548);
    }
    cUIText* title = (cUIText*)getObj_207430(self, D_004A25D8);
    desc = D_004781D0[0][0];
    int sel = 0;
    switch (D_004A25A0) {
    case 0: {
        sprintf(name, D_004715D0, D_004A25A4 + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_004781D0[D_004A25A4][sel];
        int n = 4;
        if (D_004A25A4 == 2)
            n = 3;
        for (int i = 0; i < 8; i++) {
            if (i < n && D_004A2594 >= 2) {
                sprintf(buf, D_004715E8, i);
                cUIState_showObjSafe(self->owner, buf);
                sprintf(buf, D_004715F8, i);
                if (self->eventDone[D_004A25A4][D_004A25A0][i] != 0)
                    cUIState_showObjSafe(self->owner, buf);
                else
                    cUIState_hideObjSafe(self->owner, buf);
            } else {
                sprintf(buf, D_004715E8, i);
                cUIState_hideObjSafe(self->owner, buf);
                sprintf(buf, D_004715F8, i);
                cUIState_hideObjSafe(self->owner, buf);
            }
        }
        break;
    }
    case 1: {
        sprintf(name, D_00471608, D_004A25A4 + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_004786E0[D_004A25A4][sel];
        for (int i = 0; i < 8; i++) {
            if (i < 5 && D_004A2594 >= 2) {
                sprintf(buf, D_004715E8, i);
                cUIState_showObjSafe(self->owner, buf);
                sprintf(buf, D_004715F8, i);
                if (self->eventDone[D_004A25A4][D_004A25A0][i] != 0)
                    cUIState_showObjSafe(self->owner, buf);
                else
                    cUIState_hideObjSafe(self->owner, buf);
            } else {
                sprintf(buf, D_004715E8, i);
                cUIState_hideObjSafe(self->owner, buf);
                sprintf(buf, D_004715F8, i);
                cUIState_hideObjSafe(self->owner, buf);
            }
        }
        break;
    }
    case 2: {
        char buf2[32];
        sprintf(name, D_00471628, D_004A25A4 + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_00478D38[D_004A25A4][sel];
        for (int i = 0; i < 8; i++) {
            sprintf(buf2, D_004715F8, i);
            cUIState_hideObjSafe(self->owner, buf2);
            sprintf(buf2, D_004715E8, i);
            cUIState_hideObjSafe(self->owner, buf2);
        }
        break;
    }
    }
    cUIText* text = (cUIText*)getObj_207430(self, D_004A26D0);
    cUIText_setAsciiString(text, cBENewRaceInterface_getLocationName(race, desc.w[0]));
    text = (cUIText*)getObj_207430(self, D_00471640);
    int type = desc.w[2];
    if ((unsigned int)(desc.w[0] - 0x11) < 5) {
        type = 0xC;
        if (D_004A259C == 2)
            cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471650));
    } else {
        if (func_004165A8((char*)&desc.w[0] + 0x32, D_004A2560) != 0) {
            if (D_004A259C == 2)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32((char*)&desc.w[0] + 0x32));
        } else if (desc.w[2] == 4) {
            if (self->eventUnlocked[D_004A25A4][D_004A25A0][sel] != 0)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471668));
            else
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471680));
        } else if (desc.w[2] == 5) {
            if (self->eventUnlocked[D_004A25A4][D_004A25A0][sel] != 0)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471668));
            else
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471698));
        } else if (desc.w[2] >= 6 && desc.w[2] <= 8) {
            if (self->eventUnlocked[D_004A25A4][D_004A25A0][sel] != 0) {
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471668));
            } else {
                short fmt[16];
                short part[0x130];
                short pre[0x130];
                short line[0x130];
                func_002C2540(fmt, D_004A2590);
                sprintf1_v(part, fmt, cBENewRaceInterface_getLocationName(race, desc.w[0]));
                char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
                sLocVEntry_207430* vt = *(sLocVEntry_207430**)(loc + 4);
                char* thisp = loc + vt[4].delta;
                strcpy1(pre, vt[4].fn(thisp, GetHashValue32(D_004716B0)));
                sprintf1_v(line, pre, part);
                cUIText_setUnicodeStringWrap(mapObj, line);
            }
        } else if (desc.w[2] >= 9 && desc.w[2] <= 11) {
            if (self->eventUnlocked[D_004A25A4][D_004A25A0][sel] != 0) {
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(D_00471668));
            } else {
                short part[0x130];
                short fmt[16];
                short pre[0x138];
                short line[0x130];
                func_002C2540(fmt, D_004A26A0);
                sprintf1_v(part, fmt, cBENewRaceInterface_getLocationName(race, desc.w[0]));
                char* loc = *(char**)((char*)D_004A28A8 + 0x8C);
                sLocVEntry_207430* vt = *(sLocVEntry_207430**)(loc + 4);
                char* thisp = loc + vt[4].delta;
                strcpy1(pre, vt[4].fn(thisp, GetHashValue32(D_004716B0)));
                sprintf1_v(line, pre, part);
                cUIText_setUnicodeStringWrap(mapObj, line);
            }
        }
        if (type == 0xC)
            type = func_00145870(race, desc.w[0]);
    }
    switch (type) {
    case 0:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_0046E548));
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_0046E548));
        break;
    case 1:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_0046E398));
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_0046E398));
        break;
    case 3:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_0046E3A8));
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_0046E3A8));
        break;
    case 2:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004716C8));
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_004716C8));
        break;
    case 4:
    case 5:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_0046E5F0));
        cUIText_setAsciiString(title, D_004A26D8);
        break;
    case 6:
    case 7:
    case 8:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004716D8));
        cUIText_setAsciiString(title, D_004A26D8);
        break;
    case 9:
    case 10:
    case 11:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004716E8));
        cUIText_setAsciiString(title, D_004A26D8);
        break;
    case 12:
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_004716F8));
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_004716F8));
        break;
    }
    if (D_004A259C == 2)
        cUITemplate_MAP_showEventPath(self, (char*)&desc.w[0] + 0x55);
    if (D_004A25A0 == 2) {
        text = (cUIText*)getObj_207430(self, D_00471710);
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00471720));
        text = (cUIText*)getObj_207430(self, D_00471738);
        sprintf(buf, D_004A2398, func_00153520(mission, 0, desc.w[0]), func_00153350(mission, desc.w[0]));
        cUIText_setAsciiString(text, buf);
        text = (cUIText*)getObj_207430(self, D_00471748);
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00471758));
        text = (cUIText*)getObj_207430(self, D_00471770);
        sprintf(buf, D_004A2398, func_001542E0(mission, desc.w[0]), cBEMissionInterface_getNumberMissionsOnTrack(mission, desc.w[0]));
        cUIText_setAsciiString(text, buf);
        if (cBEMissionInterface_getNumberMissionsOnTrack(mission, desc.w[0]) == 0)
            cUIText_setAsciiString(text, D_004A26E0);
    } else {
        cUIState_showObjSafe(self->owner, D_00471710);
        cUIState_showObjSafe(self->owner, D_00471738);
        text = (cUIText*)getObj_207430(self, D_00471710);
        cUIText_setUnicodeStringByID(text, GetHashValue32(D_00471780));
        int medal = cBERewardInterface_getTrackMedal(reward, 0, charID, desc.w[2], desc.w[0]);
        text = (cUIText*)getObj_207430(self, D_00471738);
        if (medal != -1)
            cUIText_setUnicodeString(text, (char*)cUITemplate_MAP_getMedalStr(self, medal));
        else
            cUIText_setUnicodeStringByID(text, GetHashValue32(D_00471790));
        cUIText* label = (cUIText*)getObj_207430(self, D_00471748);
        cUIText* value = (cUIText*)getObj_207430(self, D_00471770);
        if (D_004A25A0 == 0) {
            cUIText_setUnicodeStringByID(label, GetHashValue32(D_004717A0));
            int best = func_001550E8(score, 0, desc.w[0], desc.w[2]);
            cUIText_setAsciiString(value, func_0020A1A8(best));
            if (best == 0)
                cUIText_setAsciiString(value, D_004A2108);
        } else {
            cUIText_setUnicodeStringByID(label, GetHashValue32(D_004717B0));
            int best = func_001550E8(score, 0, desc.w[0], desc.w[2]);
            char num[32];
            sprintf(num, D_004A2138, best);
            cUIText_setAsciiString(value, num);
            if (best == 0)
                cUIText_setAsciiString(value, D_004A2108);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", func_002083D8);
#ifdef SKIP_ASM
struct sVEntry_func_002083D8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B38[];
extern char D_004C8B28[];

extern "C" void func_002083D8(void* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_func_002083D8* vt = *(sVEntry_func_002083D8**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B38);
        } else {
            sVEntry_func_002083D8* vt = *(sVEntry_func_002083D8**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B28);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setWidgetUnHiliteColor);
#ifdef SKIP_ASM
struct sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char D_004C8B58[];
extern char D_004C8B48[];

extern "C" void cUITemplate_MAP_setWidgetUnHiliteColor(void* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B58);
        } else {
            sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, D_004C8B48);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208498);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cUITemplate_MAP_onCreateScreen(void* tmpl, void* screen, void* owner);
extern char D_004A26E8[];

extern "C" void func_00208498(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_004A26E8), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cUITemplate_MAP_onCreateScreen((char*)self + 0x9C, *(void**)((char*)self + 0x40), self);
}
#endif

extern "C" void* func_002009D0(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208518__FPv);
#ifdef SKIP_ASM
void* func_00208518(void* self)
{
    return func_002009D0((char*)self + 0x9c);
}
#endif

extern "C" void* cUITemplate_MAP_onGainTransition(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208538__FPv);
#ifdef SKIP_ASM
void* func_00208538(void* self)
{
    return cUITemplate_MAP_onGainTransition((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208558);
#ifdef SKIP_ASM
extern "C" void cUIState_onGainFocus(void* self);
extern "C" void cUITemplate_MAP_onGainFocus(void* self);

extern "C" void func_00208558(void* self)
{
    cUIState_onGainFocus(self);
    cUITemplate_MAP_onGainFocus((char*)self + 0x9C);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208588);
#ifdef SKIP_ASM
extern "C" void func_00200AF0(void* self);
extern "C" void cOVState_onDestroyScreen(void* self);

extern "C" void func_00208588(void* self)
{
    func_00200AF0((char*)self + 0x9C);
    cOVState_onDestroyScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_002085B8);
#ifdef SKIP_ASM
extern "C" int cUITemplate_MAP_onWidgetEvent(void* p);
// PORT: returns int (value unused here); the int return type keeps the target's register choice after the call.
extern "C" int func_0039F190_85B8(void* self, int a1) __asm__("func_0039F190");

extern "C" void func_002085B8(char* self)
{
    if (cUITemplate_MAP_onWidgetEvent(self + 0x9C) == 1) {
        func_0039F190_85B8(*(char**)(self + 0x10) + 0x18, 1);
        *(int*)(self + 0x1C) = (*(int*)(self + 0x1C) & ~0x3F00) | 0x780;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208610);
#ifdef SKIP_ASM
extern "C" int func_00208610(void* self, int a1, int a2)
{
    if (func_00202738((char*)self + 0x9C, a1, a2) != 0) {
        return 0x101;
    }
    return 0;
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("fe/uitemplatemap", func_00208638);
#ifdef SKIP_ASM
extern "C" void func_00208638(void* self)
{
    func_00202768((char*)self + 0x9c);
}
#endif

extern "C" void* cUITemplate_MAP_onInputAlways(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", func_00208658__FPv);
#ifdef SKIP_ASM
void* func_00208658(void* self)
{
    return cUITemplate_MAP_onInputAlways((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", func_00208678);
#ifdef SKIP_ASM
extern "C" void cOVState_onUpdate(void* self);
extern "C" void cUITemplate_MAP_onUpdate(void* self);

extern "C" void func_00208678(void* self)
{
    cOVState_onUpdate(self);
    cUITemplate_MAP_onUpdate((char*)self + 0x9C);
}
#endif

