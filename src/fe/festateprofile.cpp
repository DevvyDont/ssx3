#include "common.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_05sel_prof[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the profile select screen ("05sel_prof") and shows its first frame.
 */
extern "C" void cFEStateProfileSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_05sel_prof), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* self, void* state);

struct sColorK18EB40 {
    float r, g, b, a;
    sColorK18EB40(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};
class cUIObjK18EB40 {
public:
    int f0, f4;
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
    virtual void setColor(const sColorK18EB40& c);
};
struct sVEK18EB40b {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};


/**
 * Handles select, back and highlight events on the profile select screen.
 *
 * Event 5 recolours the entry and pushes the next state for its misc data (custom interface slot 4); event 6
 * pushes the previous state (slot 5); events 1 and 2 set the highlight and normal colours.
 *
 * @param menu  the widget the event is for (null: nothing happens)
 * @param msg  UI event code (1, 2, 5 or 6)
 */
extern "C" void cFEStateProfileSelect_onWidgetEvent(void* self, char* menu, unsigned int msg)
{
    if (menu == 0)
        return;
    switch (msg) {
    case 5: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 1.0f, 0.0f, 0.0f));
        char* custom = **(char***)((char*)self + 0x10);
        sVEK18EB40b* vt = *(sVEK18EB40b**)(custom + 4);
        void* nextState = vt[4].fn(custom + vt[4].delta, self, *(int*)(menu + 0x18));
        if (nextState != 0)
            cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, nextState);
        break;
    }
    case 6: {
        char* custom = **(char***)((char*)self + 0x10);
        sVEK18EB40b* vt = *(sVEK18EB40b**)(custom + 4);
        void* nextState = vt[5].fn(custom + vt[5].delta, self, *(int*)(menu + 0x18));
        if (nextState != 0)
            cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, nextState);
        break;
    }
    case 1: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 0.0f, 1.0f, 0.0f));
        break;
    }
    case 2: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 1.0f, 1.0f, 1.0f));
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018ECA0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147318(void* self, int a1);
extern "C" void* func_001D53B0(void* self, int a1, int a2);
extern void* cFEStateProfileLoad_vtbl[];
extern int cFEStateProfileLoad_mScreenAction;

extern "C" void* func_0018ECA0(void* self, int a1, int a2, unsigned char flags)
{
    func_001D53B0(self, a1, 0);
    *(int*)((char*)self + 0xC) = 0x23;
    *(void***)((char*)self + 0x8) = cFEStateProfileLoad_vtbl;
    cFEStateProfileLoad_mScreenAction = a2;
    *(int*)((char*)self + 0x1CC) = 0;
    *(int*)((char*)self + 0x230) = 0;
    *(int*)((char*)self + 0x234) = 0;
    *(int*)((char*)self + 0x27C) = 0;
    *(unsigned char*)((char*)self + 0x15) = flags;
    *(int*)((char*)self + 0x22C) = 0;
    for (int i = 0; i < 3; i++) {
        if (flags & 1) {
            break;
        }
        flags >>= 1;
        *(int*)((char*)self + 0x22C) += 1;
    }
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* g = (signed char*)((char*)&cBENewRaceInterface_newRace);
    int n = 1;
    if (g[0x49] == 2) {
        n = 2;
    }
    for (int j = 0; j < n; j++) {
        if (func_00147318(pi, j) == *(int*)((char*)self + 0x22C)) {
            *(int*)((char*)self + 0x1CC) = j;
            break;
        }
    }
    int idx = *(signed char*)((char*)self + 0x1CC);
    *(signed char*)((char*)self + 0x44) = idx;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_93profile_load[];
extern int cFEStateProfileLoad_mScreenAction;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateProfileLoad_initGeneral(void* self);
extern "C" void cFEStateProfileLoad_initCreateScreen(void* self);
extern "C" void func_00190670(void* self);
extern "C" void func_00190768(void* self);

/**
 * Creates the profile load/save screen ("93profile_load") and sets it up for the current mode.
 *
 * After initGeneral, mode cFEStateProfileLoad_mScreenAction picks initCreateScreen (1), func_00190670 (2) or func_00190768 (3),
 * probably On Tour's initLoadScreen and initSaveScreen.
 */
extern "C" void cFEStateProfileLoad_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_93profile_load), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateProfileLoad_initGeneral(self);
    switch (cFEStateProfileLoad_mScreenAction) {
    case 1:
        cFEStateProfileLoad_initCreateScreen(self);
        break;
    case 2:
        func_00190670(self);
        break;
    case 3:
        func_00190768(self);
        break;
    }
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EE98__FPv);
#ifdef SKIP_ASM
void* func_0018EE98(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EEB8);
#ifdef SKIP_ASM
struct cList0018EEB8 {
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
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual int v54(int a);
};
extern "C" void* cSSXApp_getMCManager(void* app);
extern int cFEStateProfileLoad_mScreenAction;

extern "C" int func_0018EEB8(void* self, void* widget, unsigned int msg)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (msg) {
    case 6:
        if ((*(cList0018EEB8**)(app + 0x434))->v54(*(int*)(app + 0x428)) == 0) {
            return 0x100;
        }
        break;
    case 8:
        if (cFEStateProfileLoad_mScreenAction == 2 || *(int*)((char*)self + 0x220) == 0) {
            return 0x100;
        }
        break;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018EF70);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x[];
extern int cFEStateProfileLoad_mScreenAction;
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_ps2sq[];
struct cUIObj_EF70 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

extern "C" void func_0018EF70(void* self, int on)
{
    if (*(int*)((char*)self + 0x234) == on)
        return;
    int off = on ^ 1;
    ((cUIObj_EF70*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x)))->setVisible(off);
    if (cFEStateProfileLoad_mScreenAction != 2) {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq)))->setVisible(off);
    } else {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3a)))->setVisible(off);
    }
    *(int*)((char*)self + 0x234) = on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F0B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext5[];
extern char str_ps2sq[];
extern int cFEStateProfileLoad_mScreenAction;
struct cUIObj_F0B8 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

extern "C" void func_0018F0B8(void* self, bool on)
{
    if (cFEStateProfileLoad_mScreenAction != 2) {
        int off = !on;
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setActive(off);
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq)))->setActive(off);
    }
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F168);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext3[];
extern char str_btext3a[];
extern char str_ps2x[];
extern int cFEStateProfileLoad_mScreenAction;

struct cUIObj_F168 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

extern "C" void func_0018F168(void* self, bool a, bool b)
{
    cUIObj_F168* o = (cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x));
    if (cFEStateProfileLoad_mScreenAction != 2) {
        int v = !b;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setVisible(v);
        *(int*)((char*)self + 0x224) = v;
        o->setVisible(v);
    } else {
        int v = !a;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3a)))->setVisible(v);
        *(int*)((char*)self + 0x224) = v;
        o->setVisible(v);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F278);
#ifdef SKIP_ASM
extern "C" void cFEStateProfileLoad_displayOn(void* self);
extern "C" void func_0018F9D8(void* self);

extern "C" void func_0018F278(void* self)
{
    if (*(int*)((char*)self + 0x214) == 3) {
        cFEStateProfileLoad_displayOn(self);
    } else {
        func_0018F9D8(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayOn);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_0018F9D8(void* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001D8240();
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* mp, int dev, int a3);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mp, int dev, unsigned short* buf, int len);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern int cFEStateProfileLoad_mScreenAction;
extern char str_1percentage[];
extern char str_Memory_card_text[];
extern char str_playerpct_d[];
extern char str_arrw_left[];
extern char str_arrw_right[];
extern char str_datepct_d[];
extern char str_timepct_d[];

struct sColor_F2B8 {
    int c[4];
};
extern sColor_F2B8 D_004C66C8;

#define V_F2B8(n) virtual void v##n();
class cMCSub_F2B8 {
public:
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07) V_F2B8(08) V_F2B8(09) V_F2B8(10)
    V_F2B8(11) V_F2B8(12) V_F2B8(13)
    virtual void v14(int, void*);
    virtual void v15(int, void*);
    virtual void v16(int, void*);
    V_F2B8(17) V_F2B8(18) V_F2B8(19) V_F2B8(20)
    V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24) V_F2B8(25) V_F2B8(26) V_F2B8(27) V_F2B8(28) V_F2B8(29) V_F2B8(30)
    V_F2B8(31) V_F2B8(32) V_F2B8(33) V_F2B8(34) V_F2B8(35) V_F2B8(36) V_F2B8(37) V_F2B8(38) V_F2B8(39) V_F2B8(40)
    V_F2B8(41) V_F2B8(42) V_F2B8(43) V_F2B8(44) V_F2B8(45) V_F2B8(46) V_F2B8(47) V_F2B8(48) V_F2B8(49) V_F2B8(50)
    V_F2B8(51) V_F2B8(52)
    virtual int v53();
    virtual int v54(int);
    virtual int v55();
    V_F2B8(56) V_F2B8(57) V_F2B8(58) V_F2B8(59) V_F2B8(60)
    V_F2B8(61) V_F2B8(62) V_F2B8(63) V_F2B8(64) V_F2B8(65)
    virtual int v66(int);
    virtual int v67(int);
};

class cState_F2B8 {
public:
    int pad[2];
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07) V_F2B8(08) V_F2B8(09) V_F2B8(10)
    V_F2B8(11) V_F2B8(12) V_F2B8(13) V_F2B8(14) V_F2B8(15) V_F2B8(16) V_F2B8(17) V_F2B8(18) V_F2B8(19) V_F2B8(20)
    V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24) V_F2B8(25) V_F2B8(26) V_F2B8(27) V_F2B8(28) V_F2B8(29) V_F2B8(30)
    V_F2B8(31) V_F2B8(32) V_F2B8(33) V_F2B8(34)
    virtual void v35(int);
    V_F2B8(36) V_F2B8(37) V_F2B8(38) V_F2B8(39) V_F2B8(40) V_F2B8(41)
    virtual int v42();
    virtual void v43();
};

class cUIObj_F2B8 {
public:
    int pad[2];
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07)
    virtual void v08(int);
    virtual void v09(int);
    V_F2B8(10) V_F2B8(11) V_F2B8(12) V_F2B8(13) V_F2B8(14) V_F2B8(15) V_F2B8(16) V_F2B8(17) V_F2B8(18) V_F2B8(19)
    V_F2B8(20) V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24)
    virtual void v25(int);
};
#undef V_F2B8

static inline cUIObj_F2B8* getObj_F2B8(cState_F2B8* self, char* name)
{
    return (cUIObj_F2B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

/**
 * Refreshes the memory card panel and the six profile slot rows.
 *
 * Does nothing unless this state's virtual slot 42 returns true (probably: a card is ready). If the memory
 * card manager's profile object (+0x434) then fails its slot 53, calls func_001D8240 and func_0018F9D8 and
 * returns; otherwise it moves to state 3 (slot 35) if needed, shows the device name, fills
 * "player1".."player6" with the slot names (plus the "date%d"/"time%d" lines for used slots) and shows the
 * left/right device arrows when another device is available.
 *
 * @param vself  the cFEStateProfileLoad
 */
// Memory-card profile slots: refreshes the device name and the six slot rows (name, the two detail lines).
extern "C" void cFEStateProfileLoad_displayOn(void* vself)
{
    cState_F2B8* self = (cState_F2B8*)vself;
    unsigned short buf[0x320];
    char name[0x40];
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    if (self->v42()) {
        if (!(*(cMCSub_F2B8**)(mp + 0x434))->v53()) {
            func_001D8240();
            func_0018F9D8(self);
            return;
        } else if (*(int*)((char*)self + 0x214) != 3) {
            self->v35(3);
        }
    }
    if (self->v42()) {
        int sel = 0;
        cMCOverlayManager_GetDeviceDisplayString(mp, *(int*)(mp + 0x428), buf, 0x320);
        cUIText_setUnicodeStringWrap(getObj_F2B8(self, str_Memory_card_text), buf);
        *(int*)((char*)self + 0x21C) = *(int*)(mp + 0x428);
        self->v43();
        cUIObj_F2B8* widget = getObj_F2B8(self, str_1percentage);
        widget->v09(0);
        for (int i = 1; i < 7; i++) {
            sprintf(name, str_playerpct_d, i);
            widget = getObj_F2B8(self, name);
            *(sColor_F2B8*)((char*)widget + 0x88) = D_004C66C8;
            if (!(*(cMCSub_F2B8**)(mp + 0x434))->v67(i - 1) && !(*(cMCSub_F2B8**)(mp + 0x434))->v66(i - 1)
                && (*(cMCSub_F2B8**)(mp + 0x434))->v55()) {
                (*(cMCSub_F2B8**)(mp + 0x434))->v14(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                if (cFEStateProfileLoad_mScreenAction != 2 && sel == 0) {
                    sel = 1;
                    widget->v08(0);
                    widget->v25(0);
                } else {
                    widget->v25(1);
                    widget->v08(1);
                }
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->v09(0);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->v09(0);
            } else if ((*(cMCSub_F2B8**)(mp + 0x434))->v54(*(int*)(mp + 0x428))) {
                (*(cMCSub_F2B8**)(mp + 0x434))->v14(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                if ((*(cMCSub_F2B8**)(mp + 0x434))->v66(i - 1)) {
                    widget->v08(1);
                    widget->v25(1);
                } else {
                    widget->v08(0);
                    widget->v25(0);
                }
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                (*(cMCSub_F2B8**)(mp + 0x434))->v15(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->v09(1);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                (*(cMCSub_F2B8**)(mp + 0x434))->v16(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->v09(1);
            } else {
                (*(cMCSub_F2B8**)(mp + 0x434))->v14(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                widget->v25(1);
                widget->v08(1);
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->v09(0);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->v09(0);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 1) == -1)
            getObj_F2B8(self, str_arrw_left)->v09(0);
        else
            getObj_F2B8(self, str_arrw_left)->v09(1);
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 0) == -1)
            getObj_F2B8(self, str_arrw_right)->v09(0);
        else
            getObj_F2B8(self, str_arrw_right)->v09(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018F9D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateProfileLoad_displayOn(void* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_playerpct_d[];

struct cSelf_F9D8 {
    int f0;
    int f4;
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
    virtual void setState(int s);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int isReady();
};
struct cCard_F9D8 {
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
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual int isPresent();
};
struct cUIObj_F9D8 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
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
    virtual void setSelected(int v);
};

extern "C" void func_0018F9D8(void* p)
{
    cSelf_F9D8* self = (cSelf_F9D8*)p;
    void* app = cSSXApp_getMCManager(SSXApp);
    if (self->isReady() != 0) {
        if ((*(cCard_F9D8**)((char*)app + 0x434))->isPresent() != 0) {
            self->setState(3);
            cFEStateProfileLoad_displayOn(self);
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, str_playerpct_d, i);
        cUIObj_F9D8* o = (cUIObj_F9D8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        o->setSelected(0);
        o->setEnabled(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FAF0);
#ifdef SKIP_ASM
struct cAppMan;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* iface, int ev);
extern "C" int cBENewPlayerInterface_getLastHub(void* iface, int a1);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* func_00194738(void* self, void* engine);
extern "C" void cUIStateStack_push(void* self, void* state);
extern "C" void func_0039F4C0(void* self, void* state);
extern int D_004A11B8;
extern int D_004A19D8;
extern int D_00535C08[];
extern char str_cPreGameLoadScreen[];
extern char str_cFEStateTitle[];

struct sVEK18FAF0a {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
struct sVEK18FAF0b {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0018FAF0(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        int sel = *(int*)((char*)self + 0x27C);
        if (sel == 0) {
            char* g = **(char***)((char*)self + 0x10);
            sVEK18FAF0a* vt = *(sVEK18FAF0a**)(g + 4);
            void* r = vt[5].fn(g + vt[5].delta, self, *(int*)((char*)self + 0x1BC));
            if (r != 0)
                func_0039F4C0(*(char**)((char*)self + 0x10) + 0x18, r);
        } else if (sel == 1) {
            char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
            void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBENewRaceInterface_setGameMode(race, 4);
            cBENewRaceInterface_setGameEvent(race, 0xC);
            int x = cBENewPlayerInterface_getLastHub(pl, 0);
            D_004A11B8 = 1;
            D_00535C08[0] = x;
            sVEK18FAF0b* vt = *(sVEK18FAF0b**)(race + 0xC);
            vt[1].fn(race + vt[1].delta);
            cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        } else if (sel == 2) {
            D_004A19D8 = 0;
            void* st = func_00194738(cMemMan_alloc(0x4C, str_cFEStateTitle, 0, 0), *(void**)((char*)self + 0x10));
            cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, st);
        }
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

extern "C" void* func_001D58B8(void*);

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the profile-load screen: runs the shared profile screen teardown, then clears the word at +0x1A4.
 *
 * The teardown, func_001D58B8(), calls helpers of back-end interface 5, runs three memory card manager calls
 * when its word at +0x434 is set, and frees the buffer at +0x1D4 when one is held.
 */
void cFEStateProfileLoad_onDestroyScreen(void* self)
{
    func_001D58B8(self);
    *(int*)((char*)self + 0x1a4) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FC98);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void func_0023CAE8(void* app, int a1, int a2);
extern "C" void func_00241E18(void*, unsigned short*, int);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_Temp_String[];

struct cSelf_FC98 {
    int f0;
    int f4;
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
    virtual void showMessage(unsigned short* text, int a2, int a3, int a4, int a5, int a6);
};

extern "C" void func_0018FC98(void* p)
{
    cSelf_FC98* self = (cSelf_FC98*)p;
    if (*(int*)((char*)self + 0x224) != 0) {
        func_0023CAE8(cSSXApp_getMCManager(SSXApp), 1, *(int*)((char*)self + 0x1CC));
        *(int*)((char*)self + 0x1C0) = 3;
        *(int*)((char*)self + 0x1A8) = 1;
        *(int*)((char*)self + 0x1DC) = 1;
        if (*(int*)((char*)self + 0x210) == 0 && *(int*)((char*)self + 0x204) == 0) {
            *(int*)((char*)self + 0x19C) = 0x16;
            unsigned short* buf = (unsigned short*)operator_new_tag(0x640, str_Temp_String, 0x100, 0);
            func_00241E18(cSSXApp_getMCManager(SSXApp), buf, 0x320);
            self->showMessage(buf, 1, 1, 0, 0, 1);
            *(int*)((char*)self + 0x210) = 1;
            if (buf != 0) {
                cMemMan_free(buf);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_0018FD90);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strlen(const char* s);
extern "C" void func_00147138(void* player, int idx, char* name);
extern "C" void* cBENewPlayerInterface_getPlayerName(void* player, int idx);
extern "C" void func_0018FC98(void* p);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* kb, void* name);
extern "C" void cKeyboardPopup_setMaxCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setMinCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setIncludeKey(void* kb, int key, int a2, int a3);
extern "C" void func_001CE408(void* kb, int a1);
extern "C" void cKeyboardPopup_setStatic(void* kb, void* text);
extern "C" void func_0023C8F0(void* mp, int a1);
extern "C" void cMCOverlayManager_SetAction(void* mp, int a1);
extern "C" void func_002C2540(void* dst, char* src);
extern "C" void strcpy2(char* dst, void* src);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern int cFEStateProfileLoad_mScreenAction;
extern char str_kT_MEMPlayerName_b[];
extern char str_Keyboard[];
extern char str_kT_KEYUserName[];

struct sVEntryK18FD90 {
    short delta;
    short index;
    void* (*fn)(void*, ...);
};

struct sVEntryVK18FD90 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

#define VCALLV_K18FD90(obj, off, idx) (*(sVEntryVK18FD90**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryVK18FD90**)((char*)(obj) + (off)))[idx].delta

#define VCALL_K18FD90(obj, off, idx) (*(sVEntryK18FD90**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryK18FD90**)((char*)(obj) + (off)))[idx].delta


struct sProfileK18FD90 {
    int f0;                  // 0x0
    int f4;                  // 0x4
    void* vtbl;              // 0x8
    int fC;                  // 0xC
    void* engine;            // 0x10
    unsigned char f14;       // 0x14
    unsigned char f15;       // 0x15
    char pad16[0x40 - 0x16];
    char* screen;            // 0x40
    char pad44[0x19C - 0x44];
    int f19C;                // 0x19C
    int f1A0;                // 0x1A0
    char* keyboard;          // 0x1A4
    int f1A8;                // 0x1A8
    char pad1AC[0x1C0 - 0x1AC];
    int state;               // 0x1C0
    int f1C4;                // 0x1C4
    int f1C8;                // 0x1C8
    int slot;                // 0x1CC
    char pad1D0[0x1E4 - 0x1D0];
    int f1E4;                // 0x1E4
    char pad1E8[0x214 - 0x1E8];
    int f214;                // 0x214
    char pad218[0x220 - 0x218];
    int f220;                // 0x220
    int f224;                // 0x224
};

struct sMgrK18FD90 {
    char pad0[0xF8];
    int fF8;                 // 0xF8
};

static inline void* GetText_K18FD90(char* key)
{
    char* db = (char*)SSXApp->FELocale;
    sVEntryK18FD90* vt = *(sVEntryK18FD90**)(db + 4);
    return vt[4].fn(db + vt[4].delta, GetHashValue32(key));
}

// Profile screen: handles menu events (5 = name picked, 6 = back, 7 = slot chosen, 13 = name entered).
extern "C" void func_0018FD90(void* vself, char* item, int event)
{
    sProfileK18FD90* self = (sProfileK18FD90*)vself;
    switch (event) {
    case 13: {
        char buf[0x70];
        char fmt[0x70];
        char* name = self->keyboard + 0x74;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        if (strlen(name) != 0) {
            func_00147138(player, self->slot, name);
            func_002C2540(mp + 0x276, name);
        } else {
            strcpy2(fmt, GetText_K18FD90(str_kT_MEMPlayerName_b));
            sprintf(buf, fmt, self->slot + 1);
            func_00147138(player, self->slot, buf);
            func_002C2540(mp + 0x276, buf);
        }
        VCALLV_K18FD90(player, 0xC, 1));
        func_0018FC98(vself);
        break;
    }
    case 5: {
        if (item == 0) break;
        if (self->f1A8 != 0) break;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->f214 != 3) break;
        ((sMgrK18FD90*)mp)->fF8 = self->f1C4 = *(int*)(item + 0x18);
        switch (cFEStateProfileLoad_mScreenAction) {
        case 1:
        case 3:
            if (self->state == 1 && self->f224 != 0) {
                if (self->keyboard == 0) {
                    char* kb = (char*)cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(0x444, str_Keyboard, 0x100, 0), self->engine, vself, 0, self->f15);
                    self->keyboard = kb;
                    *(int*)(kb + 0x140) = 0x37;
                    cKeyboardPopup_setMaxCharacters(self->keyboard, 8);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x4B, 1, 2);
                    *(int*)(self->keyboard + 0x58) = 1;
                    func_001CE408(self->keyboard, 1);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0xE, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x27, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x28, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x34, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x1A, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x19, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x35, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x40, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x41, 0, 2);
                    cKeyboardPopup_setIncludeKey(self->keyboard, 0x42, 0, 2);
                    cKeyboardPopup_setMinCharacters(self->keyboard, 1);
                }
                cKeyboardPopup_setStatic(self->keyboard, GetText_K18FD90(str_kT_KEYUserName));
                *(int*)(self->keyboard + 0x54) = 1;
                cKeyboardPopup_setInputText(self->keyboard, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->slot));
                cUIStateStack_pushPopup(*(char**)(*(char**)(self->screen + 0xD0) + 0x10) + 0x18, self->keyboard);
                self->f1A0 = 1;
                self->f19C = 0;
            }
            break;
        case 2:
            if (self->f224 != 0) {
                func_0023C8F0(mp, 1);
                VCALLV_K18FD90(self, 8, 44));
            }
            break;
        }
        break;
    }
    case 6: {
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->f1A8 != 1) {
            int st = self->state;
            if (st != 6) {
                self->f1E4 = 1;
                if (st == 0 || st == 2 || st == 1) {
                    VCALLV_K18FD90(mp, 0x748, 1), 0);
                }
                self->state = 6;
            }
        }
        break;
    }
    case 7:
        if (self->f1A8 != 1 && self->state != 6 && cFEStateProfileLoad_mScreenAction != 2 && self->f220 != 0) {
            char* mp = (char*)cSSXApp_getMCManager(SSXApp);
            if (VCALL_K18FD90(*(char**)(mp + 0x434), 0, 54), *(int*)(mp + 0x428)) != 0) {
                ((sMgrK18FD90*)mp)->fF8 = self->f1C4 = *(int*)(item + 0x18);
                if (self->state == 1) {
                    if (VCALL_K18FD90(*(char**)(mp + 0x434), 0, 67), self->f1C4) != 0) {
                        self->f1A8 = 1;
                        self->state = 4;
                        cMCOverlayManager_SetAction(mp, 0);
                        VCALLV_K18FD90(mp, 0x748, 1), 0x18);
                    }
                }
            }
        }
        break;
    case 14:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initGeneral);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Title_SaveProfile[];
extern char str_Title_LoadProfile[];
extern char str_helptext_save[];
extern char str_helptext_load[];
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_pct_d_b[];
extern char str_playerpct_d[];
extern char str_onmenu[];
extern char str_offmenu[];

struct cUIObj_02C0 {
    int pad[2];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void setEnabled(int on);
    virtual void v8();
    virtual void setVisible(int on);
};

static inline cUIObj_02C0* getObj_02C0(char* self, char* name)
{
    return (cUIObj_02C0*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(name));
}

/**
 * Hides the profile panels, numbers the six slot rows and sets up the menus.
 *
 * Hides the save/load titles and help texts and the buttons "btext3", "btext3a", "btext5"; numbers "1".."6",
 * gives "player1".."player6" misc data 0..5, disables "onmenu" and enables "offmenu".
 *
 * @param selfp  the cFEStateProfileLoad
 */
// Hides the profile panels, numbers the six slot rows and enables the confirm buttons.
extern "C" void cFEStateProfileLoad_initGeneral(void* selfp)
{
    char* self = (char*)selfp;
    cUIObj_02C0* widget;
    widget = getObj_02C0(self, str_Title_SaveProfile);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_Title_LoadProfile);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_helptext_save);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_helptext_load);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_btext3);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_btext3a);
    widget->setVisible(0);
    widget = getObj_02C0(self, str_btext5);
    widget->setVisible(0);
    char buf[0x10];
    for (int i = 1; i < 7; i++) {
        sprintf(buf, str_pct_d_b, i);
        widget = getObj_02C0(self, buf);
        cUIText_setAsciiString((cUIText*)widget, buf);
        sprintf(buf, str_playerpct_d, i);
        widget = getObj_02C0(self, buf);
        *(int*)((char*)widget + 0x18) = i - 1;
    }
    getObj_02C0(self, str_onmenu)->setEnabled(0);
    getObj_02C0(self, str_offmenu)->setEnabled(1);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_kT_FEProfileNew[];
extern char str_kT_HELPCreateSaveNewFile[];
extern char str_btext3[];
extern char str_btext5[];

struct cUIObj_0540 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

/**
 * Sets the profile screen up for creating a new profile save.
 *
 * Activates "Title_SaveProfile" (kT_FEProfileNew), "helptext_save" (kT_HELPCreateSaveNewFile) and the buttons
 * "btext3" and "btext5", and sets the words at +0x220 and +0x224 to 1.
 */
extern "C" void cFEStateProfileLoad_initCreateScreen(void* self)
{
    cUIObj_0540* titleText = (cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_SaveProfile));
    titleText->setActive(1);
    cUIText_setUnicodeStringByID((cUIText*)titleText, GetHashValue32(str_kT_FEProfileNew));
    cUIObj_0540* helpText = (cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_save));
    helpText->setActive(1);
    cUIText_setUnicodeStringByID((cUIText*)helpText, GetHashValue32(str_kT_HELPCreateSaveNewFile));
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setActive(1);
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setActive(1);
    *(int*)((char*)self + 0x220) = 1;
    *(int*)((char*)self + 0x224) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190670);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern char str_Title_LoadProfile[];
extern char str_helptext_load[];
extern char str_btext3a[];
extern char str_ps2sq[];

struct cUIObj_0670 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

struct sProfile_0670 {
    char pad[0x220];
    int f220;
    int f224;
};

extern "C" void func_00190670(void* self)
{
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_LoadProfile)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_load)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3a)))->setActive(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq)))->setActive(0);
    ((sProfile_0670*)self)->f220 = 0;
    ((sProfile_0670*)self)->f224 = 1;
    *(int*)((char*)cSSXApp_getMCManager(SSXApp) + 0x11C) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190768);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_btext3[];
extern char str_btext5[];
struct cUIObj_0768 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setActive(int on);
};

extern "C" void func_00190768(void* self)
{
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_SaveProfile)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_save)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setActive(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setActive(1);
    *(int*)((char*)self + 0x220) = 1;
    *(int*)((char*)self + 0x224) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", func_00190858);
#ifdef SKIP_ASM
extern void* cFEStateRiderBio_vtbl[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_00190858(void* self, int a1, int a2)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateRiderBio_vtbl;
    *(int*)((char*)self + 0xC) = 0xF;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    return self;
}
#endif

