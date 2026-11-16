#include "common.h"
#include "online/lobbyman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "online/commsystem.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/debugfe", cDebugMenuScreen_draw);
#ifdef SKIP_ASM
// FAKEMATCH: the first row reads self->cursor through an integer temp, `__SIZE_TYPE__ hi = (__SIZE_TYPE__)self + 0x8000;
// ... *(int*)(hi + 0xc1c)` (same address, same single load; the unflagged non-pointer base makes sched1's alias check keep the
// load after the 12 colour stores); plain form 97.65%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int D_004A11B8;
extern int D_00535C04[];
extern char str_pct_d_b[];
extern char str_pct_s_pct_s_b[];
extern char str_Track[];
extern char str_Board[];
extern char* D_004A12E0[2];
extern char str_HDTV[];
extern char str_pct_s[];
extern char str_Lobby[];
extern char str_Game_Path[];
extern char str_Game_Mode[];
extern char str_Game_Event[];
extern char str_1st_Time_WC_Flag[];
extern char str_pct_s_pct_s_pct_s[];
extern char str_Character[];
extern char str_Control_Map[];
extern char str_Number_of_AI_riders[];
extern char str_Screen_Calibration[];
extern char str_On_480p[];
extern char str_Off_NTSC[];
extern char str_Playlist[];
extern char str_pct_d_pct_d_pct_d_pct_d[];
// The local mode-name table's initializer (five string pointers).
struct sModeNamesDbg { char* name[5]; };
extern sModeNamesDbg D_0045CFB8;
extern char str_Network_pct_s[];
extern char str_Real_Front_End[];
extern char str_press_start_to_play[];
extern char str_Anims_pct_d[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" char* func_00145340(void* iface);
extern "C" char* func_00145378(void* iface);
extern "C" char* func_00145398(void* iface);
extern "C" int func_00145C38(void* iface, int i);
extern "C" int func_00147618(void* iface, int i);
extern "C" char* func_0014EE28(void* iface, int i);
extern "C" void func_0017AE00(void* self);
extern "C" void func_001DC990(void* self, float x, float y, const char* fmt, ...);
extern "C" void func_001DC9D0(void* self, float x, float y, float sx, float sy, const char* str, int n);
extern "C" void func_001DCA40(void* self, float x, float y, const char* fmt, ...);
extern "C" void func_001DCA80(void* self, float x, float y, const char* fmt, ...);
extern "C" void func_00256038(void* comm, int* info);
extern "C" char* func_0028B180();
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" void func_00392908(void* self, float x, float y, const char* str);
extern "C" void func_00392A60(void* self, const char* str, float x, float y);
extern "C" int sprintf(char* buf, const char* fmt, ...);

// tARGB (On Tour layout: A, R, G, B floats).
struct tARGB {
    float A, R, G, B;
    tARGB() {}
    float& operator[](int i) { return (&A)[i]; }
    tARGB(float ia, float ir, float ig, float ib) { (*this)[0] = ia; (*this)[1] = ir; (*this)[2] = ig; (*this)[3] = ib; }
};

struct t2Vector_Dbg { float x, y; };
// cFont (On Tour layout).
struct cFont_Dbg {
    char pad0[0x14];
    int MaxCharHeight;          // 0x14
    char pad18[0x30 - 0x18];
    float BaseScaleX, BaseScaleY;   // 0x30
    float ScaleX, ScaleY;           // 0x38
    tARGB Color;                // 0x40
    void clearScale() { ScaleX = BaseScaleX; ScaleY = BaseScaleY; }
    float getMaxCharHeight() const { return (float)MaxCharHeight * BaseScaleY; }
    void setScale(float sx, float sy) { ScaleX = BaseScaleX * sx; ScaleY = BaseScaleY * sy; }
    void setColor(const tARGB& clr) { Color = clr; }
};

struct sDbgVEntry {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sDbgPopup {
    char pad0[0x8];
    int active;                 // 0x8
};

struct sDebugMenuScreen {
    char pad0[0x8C10];
    sDbgPopup popup;            // 0x8C10
    int cursor;                 // 0x8C1C
    int pad8C20;
    int optFlag;                // 0x8C24
    int pad8C28[2];
    int optVal;                 // 0x8C30
    int optTrack;               // 0x8C34
    int pad8C38[3];
    int optMode;                // 0x8C44
};

/**
 * Draws the debug front end's race setup screen, or its open popup instead.
 *
 * Lists game path, mode and event, the first-time flag, track, character, control map, board, AI rider count,
 * screen calibration, HDTV, playlist and network (with the IP address), then the "Lobby" and "Real Front End"
 * entries, highlighting the cursor row. Also prints "press <start> to play" and an anims count.
 */
extern "C" void cDebugMenuScreen_draw(sDebugMenuScreen* self)
{
    sDbgPopup* popup = &self->popup;
    if (popup->active >= 0) {
        func_0017AE00(popup);
        return;
    }
    cFont_Dbg* font = *(cFont_Dbg**)((char*)SSXApp->DebugFE + 0x18);
    void* newRace = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* character = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* intPlayer = cBE_getInterface_Fv(SSXApp->BE, 1);
    const float left = 60.0f;
    const float right = 580.0f;
    const float lineheight = font->getMaxCharHeight();
    font->setScale(1.0f, 1.0f);
    tARGB selectColor(1.0f, 0.7843137979507446f, 0.0f, 0.0f);
    tARGB normalColor(1.0f, 0.7843137979507446f, 0.7843137979507446f, 0.7843137979507446f);
    tARGB disabledColor(1.0f, 0.3921568989753723f, 0.3921568989753723f, 0.3921568989753723f);
    float y = 20.0f;
    char buf[0x80];

    __SIZE_TYPE__ hi = (__SIZE_TYPE__)self + 0x8000;
    font->setColor(*(int*)(hi + 0xc1c) == 0 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Game_Path);
    func_00392A60(font, func_00145378(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 1 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Game_Mode);
    func_00392A60(font, func_00145398(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 2 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Game_Event);
    func_00392A60(font, func_00145340(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 3 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_1st_Time_WC_Flag);
    func_001DCA80(font, 580.0f, y, str_pct_d_b, func_00145C38(intPlayer, 0));
    y += lineheight;
    if (D_004A11B8 != 0)
        sprintf(buf, str_pct_s_pct_s_pct_s, cBENewRaceInterface_getLocation(newRace) + 4, cBENewRaceInterface_getLocation(newRace) + 0x44, cBENewRaceInterface_getLocation(newRace) + 0x34);
    else
        sprintf(buf, str_pct_s_pct_s_b, cBENewRaceInterface_getLocation(newRace) + 0x34, cBENewRaceInterface_getLocation(newRace) + 0x34);
    font->setColor(self->cursor == 4 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Track);
    func_00392A60(font, buf, 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 5 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Character);
    func_00392A60(font, func_0014EE28(character, 0), 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 6 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Control_Map);
    func_001DCA80(font, 580.0f, y, str_pct_d_b, func_00147618(intPlayer, 0));
    y += lineheight;
    font->setColor(self->cursor == 7 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Board);
    func_00392A60(font, D_004A12E0[self->optFlag], 580.0f, y);
    y += lineheight;
    font->setColor(self->cursor == 8 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Number_of_AI_riders);
    func_001DCA80(font, 580.0f, y, str_pct_d_b, D_00535C04[0]);
    y += lineheight;
    font->setColor(self->cursor == 9 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Screen_Calibration);
    func_001DCA80(font, 580.0f, y, str_pct_d_b, self->optVal);
    y += lineheight;
    font->setColor(self->cursor == 10 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_HDTV);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sDbgVEntry* vt = (sDbgVEntry*)ps2GraphicsMan->mVtbl;
        func_001DCA80(font, 580.0f, y, str_pct_s, vt[9].fn((char*)ps2GraphicsMan + vt[9].delta) ? str_On_480p : str_Off_NTSC);
    }
    y += lineheight;
    font->setColor(self->cursor == 11 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Playlist);
    func_001DCA80(font, 580.0f, y, str_pct_s, func_0028B180() + (self->optTrack * 0x14 + 0x60A4));
    y += lineheight;
    unsigned int ip = 0;
    if (cCommSystem_mCommSystem != 0)
        func_00256038(cCommSystem_mCommSystem, (int*)&ip);
    char ipbuf[0x20];
    sprintf(ipbuf, str_pct_d_pct_d_pct_d_pct_d, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    sModeNamesDbg modes = D_0045CFB8;
    if (cCommSystem_mCommSystem != 0)
        font->setColor(self->cursor == 12 ? selectColor : normalColor);
    else
        font->setColor(self->cursor == 12 ? selectColor : disabledColor);
    func_001DC990(font, 60.0f, y, str_Network_pct_s, ipbuf);
    func_001DCA80(font, 580.0f, y, str_pct_s, modes.name[self->optMode]);
    y += lineheight;
    y += 5.0f;
    font->setColor(self->cursor == 13 ? selectColor : disabledColor);
    func_001DCA40(font, 320.0f, y, str_Lobby);
    font->setColor(self->cursor == 14 ? selectColor : normalColor);
    func_001DCA40(font, 320.0f, y + lineheight, str_Real_Front_End);
    if (*(int*)((char*)SSXApp + 0x4) == 0) {
        tARGB promptColour;
        promptColour.A = 1.0f;
        promptColour.R = 0.9f;
        promptColour.G = 0.9f;
        promptColour.B = 0.0f;
        font->setColor(promptColour);
        func_00392908(font, 320.0f, 420.0f, str_press_start_to_play);
    }
    tARGB animCountColour;
    animCountColour.A = 1.0f;
    animCountColour.R = 0.5f;
    animCountColour.G = 0.5f;
    animCountColour.B = 0.5f;
    font->setColor(animCountColour);
    func_001DC9D0(font, 60.0f, 420.0f, 0.5f, 0.5f, str_Anims_pct_d, 7);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D008);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct sDbgRaceSel17D008 {
    char pad_0x00[0x48];
    signed char mode;  // 0x48
    signed char slot;  // 0x49
};
class cBENewRaceIfaceK17D008 {
public:
    char pad_0x00[0xC];
    virtual int v01();
};
struct sDbgFE17D008 {
    char pad_0x0000[0x8C38];
    int modes[8]; // 0x8C38
};
extern "C" void func_00145108(void* iface, int idx);
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);

extern "C" void func_0017D008(void* self, int idx)
{
    cBENewRaceIfaceK17D008* iface = (cBENewRaceIfaceK17D008*)cBE_getInterface_Fv(SSXApp->BE, 0);
    ((sDbgFE17D008*)self)->modes[cBENewRaceInterface_newRace.cGamePath] = cBENewRaceInterface_newRace.cGameMode;
    func_00145108(iface, idx);
    cBENewRaceInterface_setGameMode(iface, ((sDbgFE17D008*)self)->modes[idx]);
    iface->v01();
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D0A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cCommSystem_openDSockChannel(void* comm, const char* addr, int port, int host);
extern "C" void func_00256038(void* comm, int* info);
extern "C" int func_00255D50(void* comm, int ch);
extern "C" void func_00255A20(void* comm);
extern "C" void SYNCTASK_run(int a);
extern "C" void THREAD_yieldticks(int n);
extern "C" void cGameComm_construct(int ch, int host);
extern "C" void func_00266D58(void* p, int ip);
extern "C" void func_00266E30(void* p);
extern void* D_004A33F4;
extern char D_0045D018[] __attribute__((aligned(8)));
extern char D_0045D028[] __attribute__((aligned(8)));

struct sNetCfg_D0A8 {
    int active;             // 0x0
    int f4;
    int host;               // 0x8
    unsigned short port;    // 0xC
};
extern sNetCfg_D0A8 D_00534B30_D0A8 __asm__("cBENetworkInterface_networkOpt");

// Debug menu: opens the direct-socket link to the paired test console and starts the game comm.
extern "C" void func_0017D0A8(void)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (D_00534B30_D0A8.active == 0) {
        return;
    }
    char addr[32];
    int info[4];
    func_00256038(cCommSystem_mCommSystem, info);
    if (info[0] == 0x0A0A3519) {
        __builtin_memcpy(addr, D_0045D018, 12);
    } else {
        __builtin_memcpy(addr, D_0045D028, 12);
    }
    int ch;
    if (D_00534B30_D0A8.host) {
        ch = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, addr, D_00534B30_D0A8.port, 1);
    } else {
        ch = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, addr, D_00534B30_D0A8.port, 0);
    }
    while (func_00255D50(cCommSystem_mCommSystem, ch) == 0) {
        func_00255A20(cCommSystem_mCommSystem);
        SYNCTASK_run(0);
        THREAD_yieldticks(1);
    }
    cGameComm_construct(ch, D_00534B30_D0A8.host);
    func_00266D58(D_004A33F4, info[0]);
    func_00266E30(D_004A33F4);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D1B8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_0017D1B8(void* self, int a, int b)
{
    int r = (a == 2) ? 4 : 5;
    switch (b) {
    case 0: case 4: case 5: case 6:
        r = 0;
        break;
    case 1: case 2: case 3:
        r = 1;
        break;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D200);
#ifdef SKIP_ASM
extern void* D_0046D740[];
extern char str_Continue[];
void* cMenu_cMenu(void* self);
extern "C" void* func_002CCDF0(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);

extern "C" void* func_0017D200(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D740;
    func_002CCDF0((char*)self + 0x130, str_Continue);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D260);
#ifdef SKIP_ASM
extern void* D_004A4E90;
extern "C" void func_0017D6F0(void* self, void* ev);
extern "C" void func_00259230(void* ev);

extern "C" void func_0017D260(void* self, void* ev)
{
    if (D_004A4E90 != 0)
        func_0017D6F0(D_004A4E90, ev);
    func_00259230(ev);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D298);
#ifdef SKIP_ASM
extern void* D_0046D350[];
extern int cBXString_EmptyString;
extern void* D_004A4E90;
extern "C" void* func_002CC018(void* self);
extern "C" void* func_001DA988(void* self, void* a, void* b);
extern "C" void* func_001DACB8(void* self, void* a, void* b, void* c);
extern "C" void* func_001DAE20(void* self, void* a);
extern "C" void* func_001DAF08(void* self, void* a, void* b);
extern "C" void* func_001DB028(void* self, void* a);
extern "C" void* __sti__all_in_one_main_cpp(void* self, void* a, void* b);
extern "C" void* func_001DB2B0(void* self, void* a);

// Owner reference handed to each debug sub-menu (a one-pointer functor).
struct sDbgOwner17D298 {
    void* owner;
};

extern "C" void* func_0017D298(char* self)
{
    *(void***)self = D_0046D350;
    *(int*)(self + 0x4) = 0;
    func_002CC018(self + 0x8);
    sDbgOwner17D298 o0;
    o0.owner = self;
    sDbgOwner17D298 o1;
    o1.owner = self;
    func_001DA988(self + 0x70, &o0, &o1);
    func_0017D200(self + 0x604);
    sDbgOwner17D298 o2;
    o2.owner = self;
    sDbgOwner17D298 o3;
    o3.owner = self;
    sDbgOwner17D298 o4;
    o4.owner = self;
    func_001DACB8(self + 0x74C, &o2, &o3, &o4);
    sDbgOwner17D298 o5;
    o5.owner = self;
    func_001DAE20(self + 0xD2C, &o5);
    sDbgOwner17D298 o6;
    o6.owner = self;
    sDbgOwner17D298 o7;
    o7.owner = self;
    func_001DAF08(self + 0x1044, &o6, &o7);
    sDbgOwner17D298 o8;
    o8.owner = self;
    func_001DB028(self + 0x1534, &o8);
    sDbgOwner17D298 o9;
    o9.owner = self;
    sDbgOwner17D298 o10;
    o10.owner = self;
    __sti__all_in_one_main_cpp(self + 0x18C0, &o9, &o10);
    sDbgOwner17D298 o11;
    o11.owner = self;
    func_001DB2B0(self + 0x1AAC, &o11);
    int g = cBXString_EmptyString;
    char* app = (char*)SSXApp;
    *(int*)(self + 0x1C04) = g;
    *(int*)(self + 0x1C08) = g;
    *(int*)(self + 0x1C0C) = g;
    *(int*)(self + 0x60) = *(int*)(*(char**)(app + 0x80) + 0x18);
    *(int*)(self + 0x64) = *(int*)(app + 0xB0);
    *(int*)(self + 0x68) = *(int*)(*(char**)(app + 0x80) + 0x20);
    D_004A4E90 = self;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D3B0);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cMenuItem__cMenuItem(void* self, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);
extern "C" void func_001DD0C8(void* self, int flags);
extern "C" void func_001DCEA0(void* self, int flags);
extern "C" void func_002CC048(void* self, int flags);
void operator_delete(int* ptr);
extern void* D_0046D350[];
extern void* cDebugScreen_vtbl[];
extern void* D_004A4E90;

struct sVE_17D3B0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

#define DTOR_ARR_17D3B0(base, n)                                  \
    if ((base) != 0) {                                            \
        char* p = (base) + (n) * 0x24;                            \
        while ((base) != p) {                                     \
            p -= 0x24;                                            \
            sVE_17D3B0* vt = *(sVE_17D3B0**)(p + 0x10);           \
            vt[1].fn(p + vt[1].delta, 0);                         \
        }                                                         \
    }

static inline void dtorA_17D3B0(char* sub)
{
    cMenuItem__cMenuItem(sub + 0x134, 2);
    cMenu__cMenu(sub, 2);
}

static inline void dtorB_17D3B0(char* sub)
{
    cMenuItem__cMenuItem(sub + 0x1C8, 2);
    cMenuItem__cMenuItem(sub + 0x1A4, 2);
    cMenuItem__cMenuItem(sub + 0x18C, 2);
    cMenuItem__cMenuItem(sub + 0x170, 2);
    cMenuItem__cMenuItem(sub + 0x154, 2);
    cMenuItem__cMenuItem(sub + 0x138, 2);
    cMenu__cMenu(sub, 2);
}

static inline void dtorC_17D3B0(char* sub)
{
    DTOR_ARR_17D3B0(sub + 0x194, 14)
    cMenuItem__cMenuItem(sub + 0x168, 2);
    cMenuItem__cMenuItem(sub + 0x150, 2);
    cMenuItem__cMenuItem(sub + 0x134, 2);
    cMenu__cMenu(sub, 2);
}

static inline void dtorD_17D3B0(char* sub)
{
    cMenuItem__cMenuItem(sub + 0x4CC, 2);
    DTOR_ARR_17D3B0(sub + 0x16C, 24)
    cMenuItem__cMenuItem(sub + 0x154, 2);
    cMenuItem__cMenuItem(sub + 0x138, 2);
    cMenu__cMenu(sub, 2);
}

static inline void dtorE_17D3B0(char* sub)
{
    DTOR_ARR_17D3B0(sub + 0x168, 12)
    cMenuItem__cMenuItem(sub + 0x150, 2);
    cMenuItem__cMenuItem(sub + 0x134, 2);
    cMenu__cMenu(sub, 2);
}

static inline void dtorF_17D3B0(char* sub)
{
    cMenuItem__cMenuItem(sub + 0x130, 2);
    cMenu__cMenu(sub, 2);
}

extern "C" void func_0017D3B0(char* self, int flags)
{
    *(void***)self = D_0046D350;
    D_004A4E90 = 0;
    cBXString__cBXString(self + 0x1C0C, 2);
    cBXString__cBXString(self + 0x1C08, 2);
    cBXString__cBXString(self + 0x1C04, 2);
    dtorA_17D3B0(self + 0x1AAC);
    dtorB_17D3B0(self + 0x18C0);
    dtorC_17D3B0(self + 0x1534);
    dtorD_17D3B0(self + 0x1044);
    dtorE_17D3B0(self + 0xD2C);
    func_001DD0C8(self + 0x74C, 2);
    dtorF_17D3B0(self + 0x604);
    func_001DCEA0(self + 0x70, 2);
    func_002CC048(self + 0x8, 2);
    *(void***)self = cDebugScreen_vtbl;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D688);
#ifdef SKIP_ASM
extern "C" void func_00255A20(void*);
extern "C" void func_0025AC50(void*);
extern "C" void func_002CC0C0(void* stack);
extern "C" void func_0017DB18(void* self);

struct sMenuStack17D688 {
    int count;      // 0x0
    void* items[8]; // 0x4
};
struct sDebugFE17D688 {
    int f0;
    int f4;
    sMenuStack17D688 stack; // 0x8
};

static inline void* sMenuStack17D688_top(sMenuStack17D688* self)
{
    if (self->count > 0)
        return self->items[self->count - 1];
    return 0;
}

extern "C" void func_0017D688(sDebugFE17D688* self)
{
    func_00255A20(cCommSystem_mCommSystem);
    func_0025AC50(LobbyMan_mLobbyMan);
    if (sMenuStack17D688_top(&self->stack) != 0)
        func_002CC0C0(&self->stack);
    func_0017DB18(self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D6F0);
#ifdef SKIP_ASM
extern char str_A_West[];
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void func_002CC3B8(void* menu, void* items);
extern "C" void func_002CC460(void* list);
extern "C" void func_0025C610(void* net);
extern "C" void func_0017DA30(void* self, int a1);
extern "C" void func_0017DE70(char* self);
extern "C" void func_001DB418(void* items, void* net);
extern "C" int func_0025F120(void* net);
extern "C" char* func_0025F150(void* net, int i);
extern "C" int func_0025F430(void* net);
extern "C" int func_004165A8(char* a, char* b);
extern "C" void func_0025D800(void* a, int b, int c);
void func_0025CD28(void* a, int b);
void func_0025FC28(void* a, int b);


struct sDebugFE17D6F0 {
    int f0;                       // 0x0
    int state;                    // 0x4
    int menu[0x1A];               // 0x8: menu stack (count first)
    char items70[0x604 - 0x70];   // 0x70
    char items604[0x74C - 0x604]; // 0x604
    char items74C[0xD2C - 0x74C]; // 0x74C
    char itemsD2C[0x1044 - 0xD2C];   // 0xD2C
    char items1044[0x18C0 - 0x1044]; // 0x1044
    char items18C0[0x1A44 - 0x18C0]; // 0x18C0
    int f1A44;                    // 0x1A44
    char pad1A48[0x1C04 - 0x1A48];
    char* user;                   // 0x1C04
    char* pass;                   // 0x1C08
    int text;                     // 0x1C0C (cBXString)
};

static inline void ClearMenu0017D6F0(int* list)
{
    while (*list != 0) {
        func_002CC460(list);
    }
}

static inline void SetMenu0017D6F0(int* menu, void* items)
{
    ClearMenu0017D6F0(menu);
    func_002CC3B8(menu, items);
}

// Debug front end: handles a menu event (id at ev+0, argument at ev+8).
extern "C" void func_0017D6F0(void* vself, void* ev)
{
    sDebugFE17D6F0* self = (sDebugFE17D6F0*)vself;
    switch (*(int*)ev) {
    case 0x115: {
        int* list = self->menu;
        while (*list != 0) {
            func_002CC460(list);
        }
        if (SSXApp->unk_74 != 0) {
            self->state = 0;
        } else {
            self->state = 0x13;
            func_002CC3B8(self->menu, self->items604);
        }
        break;
    }
    case 0x102:
        func_0017DA30(self, *(int*)((char*)ev + 8));
        break;
    case 0xC9:
        if (SSXApp->unk_74 != 0) {
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, SSXApp->UserName);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, SSXApp->Password);
            func_0025C610(LobbyMan_mLobbyMan);
            self->state = 5;
        } else {
            self->state = 3;
            func_002CC3B8(self->menu, self->items70);
        }
        break;
    case 0xCB:
        self->state = 5;
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, self->user);
        cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, self->pass);
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    case 0xD4:
        if (SSXApp->unk_74 != 0) {
            self->state = 7;
            func_0025CD28(LobbyMan_mLobbyMan, 0);
        } else {
            func_001DB418(self->items74C, LobbyMan_mLobbyMan);
            func_002CC3B8(self->menu, self->items74C);
            self->state = 6;
        }
        break;
    case 0xDA:
        if (SSXApp->unk_74 != 0) {
            int i;
            for (i = 0; i < func_0025F120(LobbyMan_mLobbyMan); i++) {
                if (func_004165A8(func_0025F150(LobbyMan_mLobbyMan, i), str_A_West) == 0) {
                    func_0025D800(LobbyMan_mLobbyMan, i, 0);
                    self->state = 0xB;
                }
            }
        } else {
            func_002CC3B8(self->menu, self->itemsD2C);
            self->state = 0xA;
        }
        break;
    case 0xD6:
    case 0xD8:
        func_001DB418(self->items74C, LobbyMan_mLobbyMan);
        func_002CC3B8(self->menu, self->items74C);
        self->state = 6;
        break;
    case 0xDC:
        if (func_0025F430(LobbyMan_mLobbyMan) != 0) {
            func_002CC3B8(self->menu, self->items1044);
            self->state = 0xC;
        } else {
            func_002CC3B8(self->menu, self->itemsD2C);
            self->state = 0xA;
        }
        break;
    case 0xED:
    case 0xF0:
    case 0xF1: {
        self->state = 0x11;
        int* list = self->menu;
        while (*list != 0) {
            func_002CC460(list);
        }
        func_002CC3B8(self->menu, self->items604);
        break;
    }
    case 0xEF:
        func_0017DE70((char*)self);
        self->state = 0x10;
        break;
    case 0xEC:
        if (self->state == 0xC) {
            int* list = self->menu;
            while (*list != 0) {
                func_002CC460(list);
            }
            self->state = 0xF;
            cBXString_operatorE(&self->text, (char*)ev + 8);
            self->f1A44 = self->text;
            func_002CC3B8(self->menu, self->items18C0);
        } else {
            func_0025FC28(LobbyMan_mLobbyMan, *(int*)((char*)ev + 8));
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017DA30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_002CC3B8(void* menu, void* items);
extern "C" void func_0025C610(void* net);
static inline void SetMode0017DA30(void* self, int mode)
{
    *(int*)((char*)self + 0x4) = mode;
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x604);
}
// Advances the debug menu to the follow-up screen for the current mode.
extern "C" void func_0017DA30(void* self, int a1)
{
    *(int*)((char*)self + 0x1C14) = a1;
    switch (*(int*)((char*)self + 0x4)) {
    case 2:
        SetMode0017DA30(self, 0x12);
        break;
    case 4:
        SetMode0017DA30(self, 0x14);
        break;
    case 5:
        if (SSXApp->unk_74 != 0) {
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, SSXApp->UserName);
            cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, SSXApp->Password);
            func_0025C610(LobbyMan_mLobbyMan);
        } else {
            SetMode0017DA30(self, 0x15);
        }
        break;
    case 7:
        SetMode0017DA30(self, 0x16);
        break;
    case 8:
        SetMode0017DA30(self, 0x16);
        break;
    case 9:
        SetMode0017DA30(self, 0x16);
        break;
    case 11:
        SetMode0017DA30(self, 0x17);
        break;
    case 13:
        SetMode0017DA30(self, 0x18);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017DB18);
#ifdef SKIP_ASM
extern void* D_004A33F4;
extern char str_cPreGameLoadScreen[];
struct cAppMan;
extern "C" void func_002CC3B8(void* menu, void* items);
extern "C" int func_002CC098(void* menu);
extern "C" void func_00255F50(void* comm, int a1);
extern "C" void func_0025B6D8(void* net, int a1);
extern "C" void func_001DB418(void* items, void* net);
extern "C" void func_001DB528(void* items, void* net);
extern "C" void func_001DB620(void* items, void* net);
extern "C" void func_0025D800(void* a, int b, int c);
extern "C" int func_00255D50(void* comm, int ch);
extern "C" void cGameComm_construct(int ch, int host);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* mem);
void cAppMan_setNextModule(cAppMan* app, unsigned int module);
extern "C" void func_00256038(void* comm, int* info);
extern "C" void func_00266D58(void* p, int ip);
extern "C" void func_00266E30(void* p);

struct sDebugFE17DB18 {
    int f0;                          // 0x0
    unsigned int state;              // 0x4
    int menu[0x1A];                  // 0x8: menu stack (count first)
    char items70[0x74C - 0x70];      // 0x70
    char items74C[0xD2C - 0x74C];    // 0x74C
    char itemsD2C[0x1044 - 0xD2C];   // 0xD2C
    char items1044[0x18C0 - 0x1044]; // 0x1044
    char items18C0[0x1AAC - 0x18C0]; // 0x18C0
    char items1AAC[0x1C10 - 0x1AAC]; // 0x1AAC
    int channel;                     // 0x1C10
};

// Debug front end: runs the menu state machine until the state settles.
extern "C" void func_0017DB18(void* vself)
{
    sDebugFE17DB18* self = (sDebugFE17DB18*)vself;
    unsigned int state;
    do {
        state = self->state;
        switch (state) {
        case 0: {
            cCommSystem* comm = cCommSystem_mCommSystem;
            bool busy = comm->mNetConnState != 1;
            if (!busy) {
                func_00255F50((char*)comm, 0);
            }
            self->state = 1;
            break;
        }
        case 1:
            if (cCommSystem_mCommSystem->mNetConnState != 3 && cCommSystem_mCommSystem->mNetConnState != 4) {
                func_0025B6D8(LobbyMan_mLobbyMan, 0);
                self->state = 2;
            }
            break;
        case 13:
            if (func_002CC098(self->menu) == 0) {
                func_002CC3B8(self->menu, self->items1AAC);
            }
            break;
        case 17:
            if (func_002CC098(self->menu) == 0) {
                self->state = 0xC;
                func_002CC3B8(self->menu, self->items1044);
            }
            break;
        case 24:
            if (func_002CC098(self->menu) == 0) {
                func_002CC3B8(self->menu, self->items1044);
                self->state = 0xC;
            }
            break;
        case 21:
            if (func_002CC098(self->menu) == 0) {
                self->state = 3;
                func_002CC3B8(self->menu, self->items70);
            }
            break;
        case 20:
            if (func_002CC098(self->menu) == 0) {
                self->state = 3;
                func_002CC3B8(self->menu, self->items70);
            }
            break;
        case 18:
        case 19:
            if (func_002CC098(self->menu) == 0) {
                self->state = 0;
            }
            break;
        case 22:
            if (func_002CC098(self->menu) == 0) {
                func_001DB418(self->items74C, LobbyMan_mLobbyMan);
                func_002CC3B8(self->menu, self->items74C);
                self->state = 6;
            }
            break;
        case 23:
            if (func_002CC098(self->menu) == 0) {
                func_002CC3B8(self->menu, self->itemsD2C);
                self->state = 0xA;
            }
            break;
        case 15: {
            int* menu = self->menu;
            if (*menu == 0) {
                func_002CC3B8(menu, self->items18C0);
            }
            break;
        }
        case 6: {
            int* menu = self->menu;
            if (*menu == 0) {
                func_002CC3B8(menu, self->items74C);
            }
            break;
        }
        case 10: {
            int* menu = self->menu;
            char* items = self->itemsD2C;
            if (*menu == 0) {
                func_002CC3B8(menu, items);
            }
            func_001DB528(items, LobbyMan_mLobbyMan);
            break;
        }
        case 12:
            if (self->menu[0] == 0) {
                func_0025D800(LobbyMan_mLobbyMan, -1, 0);
                self->state = 0xB;
            } else {
                func_001DB620(self->items1044, LobbyMan_mLobbyMan);
            }
            break;
        case 3: {
            int* menu = self->menu;
            if (*menu == 0) {
                func_002CC3B8(menu, self->items70);
            }
            break;
        }
        case 14: {
            int* menu = self->menu;
            if (*menu == 0) {
                self->state = 0xC;
                func_002CC3B8(menu, self->items1044);
            }
            break;
        }
        case 16:
            if (func_00255D50(cCommSystem_mCommSystem, self->channel) != 0) {
                int info[4];
                cGameComm_construct(self->channel, LobbyMan_mLobbyMan->mIsHost);
                cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
                func_00256038(cCommSystem_mCommSystem, info);
                func_00266D58(D_004A33F4, info[0]);
                func_00266E30(D_004A33F4);
            }
            break;
        }
    } while (self->state != state);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017DE70);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void func_00145108(void* iface, int idx);
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);
extern "C" int cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void cBENewPlayerInterface_setRiderCtrlID(void* iface, int rider, int ctrl);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* iface, int rider, int id);
extern "C" void func_001472C8(void* iface, int a1, int a2);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int cCommSystem_openDSockChannel(void* comm, const char* addr, int port, int host);
extern int D_004A11B8;
extern int D_00535C08[];
extern char str_pct_d_pct_d_pct_d_pct_d[];

struct sNetCfg_17DE70 {
    int active;
    int f4;
    int host;
    unsigned short port;
};
extern sNetCfg_17DE70 D_00534B30_cfg __asm__("cBENetworkInterface_networkOpt");

struct sVEnt_17DE70 { short delta; short index; void (*fn)(void*); };

static inline void vcall1_17DE70(void* obj)
{
    sVEnt_17DE70* vt = *(sVEnt_17DE70**)((char*)obj + 0xC);
    vt[1].fn((char*)obj + vt[1].delta);
}

extern "C" void func_0017DE70(char* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    func_00145108(race, 1);
    cBENewRaceInterface_setGameMode(race, 4);
    cBENewRaceInterface_setNumberAI(race, 0);
    D_00535C08[0] = 0;
    D_004A11B8 = 1;
    vcall1_17DE70(race);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    for (int i = 2; i < 6; i++) {
        cBENewPlayerInterface_setRiderCtrlID(player, i, -1);
        cBENewPlayerInterface_setRiderCharID(player, i, 0);
    }
    func_001472C8(player, 0, 0);
    func_001472C8(player, 1, -1);
    vcall1_17DE70(player);
    void* net = cBE_getInterface_Fv(SSXApp->BE, 7);
    sNetCfg_17DE70* cfg = &D_00534B30_cfg;
    D_00534B30_cfg.active = 1;
    cfg->host = LobbyMan_mLobbyMan->mIsHost;
    vcall1_17DE70(net);
    unsigned int ip;
    if (cfg->host) {
        ip = (unsigned int)LobbyMan_mLobbyMan->mOppoAddr;
    } else {
        ip = (unsigned int)LobbyMan_mLobbyMan->mHostAddr;
    }
    char buf[256];
    sprintf(buf, str_pct_d_pct_d_pct_d_pct_d, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    *(int*)(self + 0x1C10) = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, buf, D_00534B30_cfg.port, D_00534B30_cfg.host);
    *(int*)(self + 4) = 0x10;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E030);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025B9C8(void* mgr, void* req);
extern char str_pleaseat_get_lost[];

struct sDebugFEState17E030 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};

struct sDsReq17E030 {
    char pad_0x000[0x260];
    char* user;     // 0x260
    char* pass;     // 0x264
    char* pass2;    // 0x268
    char* host;     // 0x26C
    char pad_0x270[0x288 - 0x270];
    int f288;
    int f28C;
    int port;       // 0x290
    int f294;
    int f298;
    int f29C;
};

extern "C" void func_0017E030(sDebugFEState17E030* self, const char* a, const char* b)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    cBXString_cBXString4((char*)self + 0x1C04, a);
    cBXString_cBXString4((char*)self + 0x1C08, b);
    self->state = 0x4;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    cBXString_cBXString4(&lobbyMan->mNewAccount.mName, a);
    cBXString_cBXString4(&lobbyMan->mNewAccount.mPassword, b);
    cBXString_cBXString4(&lobbyMan->mNewAccount.unk_08, b);
    cBXString_cBXString4(&lobbyMan->mNewAccount.mMail, str_pleaseat_get_lost);
    lobbyMan->mNewAccount.mBornYear = 0x79E;
    lobbyMan->mNewAccount.mGender = 1;
    lobbyMan->mNewAccount.mBornMonth = 1;
    lobbyMan->mNewAccount.mSpam[1] = 0;
    lobbyMan->mNewAccount.mSpam[0] = 0;
    lobbyMan->mNewAccount.mBornDay = 1;
    func_0025B9C8(LobbyMan_mLobbyMan, &lobbyMan->mNewAccount.mName);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E118);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);
extern "C" void* cBXString_cBXString4(void* self, const char* str);
extern "C" void func_0025C610(void* mgr);

struct sDebugFEState17E118 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
    char pad_0x0C[0x1C04 - 0xC];
    char* user;  // 0x1C04
    char* pass;  // 0x1C08
};

extern "C" void func_0017E118(sDebugFEState17E118* self, const char* userName, const char* password)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    cBXString_cBXString4(&self->user, userName);
    cBXString_cBXString4(&self->pass, password);
    self->state = 0x5;
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x48, self->user);
    cBXString_cBXString4((char*)LobbyMan_mLobbyMan + 0x4C, self->pass);
    func_0025C610(LobbyMan_mLobbyMan);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E1C0);
#ifdef SKIP_ASM
struct sDebugFEState17E1C0 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D1B8(void* a, int b);

extern "C" void func_0017E1C0(sDebugFEState17E1C0* self, int persona)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x8;
    func_0025D1B8(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E228);
#ifdef SKIP_ASM
struct sDebugFEState17E228 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D428(void* a, int b);

extern "C" void func_0017E228(sDebugFEState17E228* self, int persona)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x9;
    func_0025D428(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E290);
#ifdef SKIP_ASM
struct sDebugFEState17E290 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
void func_0025CD28(void* a, int b);

extern "C" void func_0017E290(sDebugFEState17E290* self, int persona)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0x7;
    func_0025CD28(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E2F8);
#ifdef SKIP_ASM
struct sDebugFEState17E2F8 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025D800(void* a, int b, int c);

extern "C" void func_0017E2F8(sDebugFEState17E2F8* self, int room)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0xB;
    func_0025D800(LobbyMan_mLobbyMan, room, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E368);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);

extern "C" void func_0017E368(void* self)
{
    int* list = (int*)((char*)self + 0x8);
    while (*list != 0) {
        func_002CC460(list);
    }
    *(int*)((char*)self + 0x4) = 0xD;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E3B8);
#ifdef SKIP_ASM
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);

extern "C" void func_0017E3B8(void* self)
{
    int* list = (int*)((char*)self + 0x8);
    while (*list != 0) {
        func_002CC460(list);
    }
    *(int*)((char*)self + 0x4) = 0xE;
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1534);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E418);
#ifdef SKIP_ASM
struct sDebugFEState17E418 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
extern "C" void func_0025AAE0(void* a, int b);

extern "C" void func_0017E418(sDebugFEState17E418* self, int arg)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025AAE0(LobbyMan_mLobbyMan, arg);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E490);
#ifdef SKIP_ASM
struct sDebugFEState17E490 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_0025FB58(void* a, int b, int c);

extern "C" void func_0017E490(sDebugFEState17E490* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    self->state = 0xD;
    func_0025FB58(LobbyMan_mLobbyMan, *(int*)((char*)self + 0x1C0C), 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E4F0);
#ifdef SKIP_ASM
struct sDebugFEState17E4F0 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
void func_0025FC28(void* a, int b);

extern "C" void func_0017E4F0(sDebugFEState17E4F0* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025FC28(LobbyMan_mLobbyMan, *(int*)((char*)self + 0x1C0C));
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E558);
#ifdef SKIP_ASM
struct sDebugFEState17E558 {
    int f0;
    int state; // 0x4
    int list;  // 0x8
};
extern "C" void func_002CC460(void* list);
extern "C" void func_002CC3B8(void* list, void* item);
extern "C" void func_0025F628(void* a, int b, int c);

extern "C" void func_0017E558(sDebugFEState17E558* self)
{
    int* list = &self->list;
    while (*list != 0) {
        func_002CC460(list);
    }
    func_002CC3B8((char*)self + 0x8, (char*)self + 0x1044);
    self->state = 0xC;
    func_0025F628(LobbyMan_mLobbyMan, 1, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E5C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char str_Connecting_to_network[];
extern char str_Connecting_to_lobby[];
extern char str_Creating_account[];
extern char str_Authenticating_account[];
extern char str_Creating_Persona[];
extern char str_Deleting_Persona[];
extern char str_Selecting_Persona[];
extern char str_Selecting_Room[];
extern char str_Waiting_for_reponse[];
extern char str_Challenge_Declined[];
extern char str_Error_Connecting[];
extern char str_Disconnected_from_server[];
extern char str_Error_Creating_Account[];
extern char str_Persona_Error[];
extern char str_Error_Authenticating_Account[];
extern char str_Error_Entering_room[];
extern char str_Error_Issueing_Challenge[];
extern char str_Play_Game[];
extern char str_Opponent[];
extern char str_Host_Client[];
extern char str_Host_Address[];
extern char str_Client_Address[];
extern char str_pct_d_pct_d_pct_d_pct_d[];
extern char str_Game[];
extern char D_004A12F8[];
extern char D_004A1330[];
extern char str_Seed[];
extern char str_pct_d_b[];
extern "C" void func_00392908(void* self, float x, float y, const char* str);
extern "C" void func_00392A60(void* self, const char* str, float x, float y);
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" void func_001DCA80(void* self, float x, float y, const char* fmt, ...);
extern "C" void func_001DCA40(void* self, float x, float y, const char* fmt, ...);
extern "C" void func_002CC350(void* self);
extern "C" void func_00256038(void* comm, int* out);
extern "C" void cBXString__cBXString(void* self, int flags);

struct sColor17E5C8 {
    float r, g, b, a;
    sColor17E5C8(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct sFont17E5C8 {
    char pad0[0x30];
    float bx, by;
    float sx, sy;
    sColor17E5C8 color;
};

// Returned by value through a hidden pointer and destroyed at the end of the
// full expression, so it shares its stack slot with the colour temporaries.
struct sBXStr17E5C8 {
    char* p;
    sBXStr17E5C8(const sBXStr17E5C8&);
    ~sBXStr17E5C8() { cBXString__cBXString(this, 2); }
};
sBXStr17E5C8 getStr17E5C8(void* self, unsigned int tag) __asm__("func_00260E50");

struct sDebugFE17E5C8 {
    int f0;
    int state;
    int count;
    void* items[0x1C14 / 4 - 3];
    unsigned int tag;
};

static inline void scale17E5C8(sFont17E5C8* f, float s)
{
    f->sx = f->bx * s;
    f->sy = f->by * s;
}

static inline void* top17E5C8(sDebugFE17E5C8* self)
{
    if (self->count > 0)
        return self->items[self->count - 1];
    return 0;
}

// Kept as an inline helper: its end label splits the block, so the 530.0f
// constants after it are loaded again instead of being shared.
static inline void onOff17E5C8(void* font, int on, float x, float y)
{
    func_00392A60(font, on ? D_004A12F8 : D_004A1330, x, y);
}

static inline int isEq17E5C8(int a, int b) { return a == b; }

extern "C" void func_0017E5C8(sDebugFE17E5C8* self)
{
    sFont17E5C8* font = *(sFont17E5C8**)((char*)SSXApp->DebugFE + 0x18);
    switch (self->state) {
    case 0:
    case 1:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Connecting_to_network);
        break;
    case 2:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Connecting_to_lobby);
        break;
    case 4:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Creating_account);
        break;
    case 5:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Authenticating_account);
        break;
    case 8:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Creating_Persona);
        break;
    case 9:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Deleting_Persona);
        break;
    case 7:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Selecting_Persona);
        break;
    case 11:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 200.0f, str_Selecting_Room);
        break;
    case 13:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Waiting_for_reponse);
        break;
    case 17:
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Challenge_Declined);
        break;
    case 18: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Error_Connecting);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 19: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Disconnected_from_server);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 20: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Error_Creating_Account);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 22: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Persona_Error);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 21: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Error_Authenticating_Account);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 23: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Error_Entering_room);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 24: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Error_Issueing_Challenge);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        func_00392908(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 16: {
        scale17E5C8(font, 1.5f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        func_00392908(font, 320.0f, 100.0f, str_Play_Game);
        scale17E5C8(font, 0.7f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.6f);
        float y = 200.0f;
        cFont_text(font, 110.0f, y, str_Game);
        func_00392A60(font, LobbyMan_mLobbyMan->mGameName, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Opponent);
        func_00392A60(font, LobbyMan_mLobbyMan->mOpponent, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Host_Client);
        onOff17E5C8(font, LobbyMan_mLobbyMan->mIsHost, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Host_Address);
        unsigned int ip = (unsigned int)LobbyMan_mLobbyMan->mHostAddr;
        func_001DCA80(font, 530.0f, y, str_pct_d_pct_d_pct_d_pct_d, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Client_Address);
        unsigned int ip2 = (unsigned int)LobbyMan_mLobbyMan->mOppoAddr;
        func_001DCA80(font, 530.0f, y, str_pct_d_pct_d_pct_d_pct_d, ip2 >> 24, (ip2 >> 16) & 0xFF, (ip2 >> 8) & 0xFF, ip2 & 0xFF);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Seed);
        func_001DCA80(font, 530.0f, y, str_pct_d_b, LobbyMan_mLobbyMan->mSeed);
        break;
    }
    }
    scale17E5C8(font, 0.9f);
    if (top17E5C8(self) != 0)
        func_002CC350(&self->count);
    if (isEq17E5C8(cCommSystem_mCommSystem->mNetConnState, 5)) {
        unsigned int ip;
        func_00256038(cCommSystem_mCommSystem, (int*)&ip);
        scale17E5C8(font, 0.8f);
        font->color = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.2f);
        unsigned int localAddr = ip;
        func_001DCA40(font, 320.0f, 420.0f, str_pct_d_pct_d_pct_d_pct_d, localAddr >> 24, (localAddr >> 16) & 0xFF, (localAddr >> 8) & 0xFF, localAddr & 0xFF);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017F2D0);
#ifdef SKIP_ASM
extern "C" float func_0031BF60(float x);

struct sVec2_17F2D0 {
    float x, y;
    sVec2_17F2D0() {}
    sVec2_17F2D0(float ax, float ay) : x(ax), y(ay) {}
    sVec2_17F2D0& operator+=(const sVec2_17F2D0& o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }
    sVec2_17F2D0& operator+=(float s)
    {
        x += s;
        y += s;
        return *this;
    }
    sVec2_17F2D0& operator*=(float s)
    {
        x *= s;
        y *= s;
        return *this;
    }
};

struct sFirefly_17F2D0 {
    sVec2_17F2D0 pos;        // 0x0
    sVec2_17F2D0 phase;      // 0x8
    sVec2_17F2D0 trail[64];  // 0x10
    int head;                // 0x210
};

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_17F2D0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_17F2D0(float x)
{
    return x - ffloor_17F2D0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

extern "C" void func_0017F2D0(void* p)
{
    sFirefly_17F2D0* self = (sFirefly_17F2D0*)p;
    for (int i = 0; i < 2; i++) {
        sVec2_17F2D0 v(func_0031BF60(self->phase.x), func_0031BF60(self->phase.y));
        v += 1.899999976158142f;
        v *= 0.03200000151991844f;
        {
            sVec2_17F2D0 d;
            d.x = 0.004999999888241291f;
            d.y = 0.008999999612569809f;
            self->phase.x += d.x;
            self->phase.y += d.y;
        }
        self->pos += v;
        self->phase.x = wrap_17F2D0(self->phase.x);
        self->phase.y = wrap_17F2D0(self->phase.y);
        self->pos.x = wrap_17F2D0(self->pos.x);
        self->pos.y = wrap_17F2D0(self->pos.y);
        self->head = (self->head + 1) % 64;
        sVec2_17F2D0 pt(func_0031BF60(self->pos.x) * 20.0f + 550.0f,
                        func_0031BF60(self->pos.y) * 20.0f + 420.0f);
        self->trail[self->head] = pt;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017F500);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28AC;
extern "C" void func_0038EC40(void* self, float* p0, float* p1, float* col, float z);


struct sV2_17F500 {
    float x, y;
    sV2_17F500() {}
    sV2_17F500(float ax, float ay) : x(ax), y(ay) {}
};

static inline sV2_17F500 operator*(const sV2_17F500& a, float s) { return sV2_17F500(a.x * s, a.y * s); }
static inline sV2_17F500 operator-(const sVec2_17F2D0& a, const sV2_17F500& b);
static inline sV2_17F500 operator+(const sVec2_17F2D0& a, const sV2_17F500& b);
static inline sV2_17F500 operator-(const sVec2_17F2D0& a, const sV2_17F500& b) { return sV2_17F500(a.x - b.x, a.y - b.y); }
static inline sV2_17F500 operator+(const sVec2_17F2D0& a, const sV2_17F500& b) { return sV2_17F500(a.x + b.x, a.y + b.y); }

struct sCol_17F500 {
    float a, r, g, b;
    sCol_17F500(float aa, const float& ar, float ag, float ab) : a(aa), r(ar), g(ag), b(ab) {}
};

// The quad routine takes its corners and colour by reference (pointers on the EE).
void func_0038EC40_r(void* self, const sV2_17F500& p0, const sV2_17F500& p1, const sCol_17F500& c, float z) __asm__("func_0038EC40");

static inline void set1_17F500(char* m, unsigned int mask, int shift, unsigned int v)
{
    sDrawState* top = *(sDrawState**)(m + 0xE84);
    top->unk_00[1] = (top->unk_00[1] & ~mask) | ((v << shift) & mask);
}


extern "C" void func_0017F500(void* p_, int tex)
{
    sFirefly_17F2D0* self = (sFirefly_17F2D0*)p_;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    set1_17F500((char*)ps2GraphicsMan, 0x1800000, 23, 2);
    set1_17F500((char*)ps2GraphicsMan, 0x300000, 20, 3);
    set1_17F500((char*)ps2GraphicsMan, 0xFF000, 12, 0xD);
    set1_17F500((char*)ps2GraphicsMan, 0x7C, 2, 5);
    set1_17F500((char*)ps2GraphicsMan, 0x400000, 22, 1);
    sV2_17F500 size(6.0f, 6.0f);
    (ps2GraphicsMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    for (int i = 1; i < 64; i++) {
        sVec2_17F2D0& pt = self->trail[(self->head + i) % 64];
        float t = (float)i * 0.015625f;
        func_0038EC40_r(D_004A28AC, pt - size * 0.5f, pt + size * 0.5f, sCol_17F500(t * t * 0.15f, 0.5f, t * 0.2f + 0.3f, t * (t * 0.5f)), 0.0f);
    }
    // Head marker colour; named locals (not literals) so the inlined ctor stores sp-relative (CHEATSHEET).
    float hi = 0.5f;
    float lo = 0.25f;
    func_0038EC40_r(D_004A28AC, self->trail[self->head] - size, self->trail[self->head] + size, sCol_17F500(hi, lo, lo, hi), 0.0f);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017F7B8);
#ifdef SKIP_ASM
extern "C" void func_00231CD0(void* self);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char cDebugFE_vtbl[];

struct sDbgEntry17F7B8 {
    int a;
    int b;
    sDbgEntry17F7B8() {}
};

inline void* operator new[](unsigned int, void* p) { return p; }

struct sDbgList17F7B8 {
    int a;                      // 0x00
    int b;                      // 0x04
    int c;                      // 0x08
    int d;                      // 0x0C
    sDbgEntry17F7B8 e[64];      // 0x10
    int n;                      // 0x210
};

extern "C" void* func_0017F7B8(void* self)
{
    func_00231CD0(self);
    *(void**)self = cDebugFE_vtbl;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x30) = 0;
    sDbgList17F7B8* list = (sDbgList17F7B8*)((char*)self + 0x24);
    new (list->e) sDbgEntry17F7B8[64];
    list->n = 0;
    MEM_fill(list->e, 0, 0x200);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE__cDebugFE);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys the debug front end: stores the base vtable D_0046D970 at +0x0 and, if asked, frees the object.
 *
 * It does not free the screens, font or textures; cDebugFE_purge does that.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDebugFE__cDebugFE(void* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE_load);
#ifdef SKIP_ASM
extern char str_DebugFont[];
extern char str_data_fonts_debugfnt_sfn[];
extern char str_data_textures_dot_ssh[];
extern char str_data_fonts_menu_ssh[];
extern char str_DebugMenu[];
extern char str_LobbyMenu[];
extern char str_part[];
extern char str_menu[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cMemMan_free(void* ptr);
extern "C" void* cFont_cFont(void* self, const char* name, int a2);
extern "C" int* FILE_loadz(const char* name, int flags);
extern "C" int* FILE_load(const char* name, int flags);
extern "C" void* cDebugMenuScreen_cDebugMenuScreen(void* self);
extern "C" void* func_0017D298(char* self);

struct sVEnt_17F868 { short delta; short index; void* fn; };
typedef void (*Fn1_17F868)(void*, int);
typedef int (*Load_17F868)(void*, void*, const char*, int, int, int);

struct sVec2_17F868 {
    float x, y;
    sVec2_17F868(float ax, float ay) : x(ax), y(ay) {}
};

struct sDebugFE_17F868 {
    char pad_0x00[0xC];
    void* objs[2];      // 0xC
    void* cur;          // 0x14
    char* font;         // 0x18
    int snd1C;          // 0x1C
    int snd20;          // 0x20
};

/**
 * Loads the debug front end: its font, the dot and menu textures, and the debug and lobby menu screens.
 *
 * Starts on the lobby menu when SSXApp->unk_74 is set, otherwise on the debug menu screen.
 */
extern "C" void cDebugFE_load(sDebugFE_17F868* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVEnt_17F868* vt = (sVEnt_17F868*)ps2GraphicsMan->mVtbl;
    ((Fn1_17F868)vt[44].fn)((char*)ps2GraphicsMan + vt[44].delta, 0);
    self->font = (char*)cFont_cFont(cMemMan_alloc(0x84, str_DebugFont, 0, 0), str_data_fonts_debugfnt_sfn, 0);
    sVec2_17F868 sc(1.600000023841858f, 1.600000023841858f);
    *(sVec2_17F868*)(self->font + 0x30) = sc;
    *(sVec2_17F868*)(self->font + 0x38) = *(sVec2_17F868*)(self->font + 0x30);
    self->snd1C = -1;
    int* data = FILE_loadz(str_data_textures_dot_ssh, 0x100);
    if (data) {
        char* payload = (char*)data + data[5];
        if (payload) {
            cPS2GraphicsMan* theGraphicsMan = cGraphicsMan_TheGraphicsMan;
            sVEnt_17F868* gvt = (sVEnt_17F868*)theGraphicsMan->mVtbl;
            self->snd1C = ((Load_17F868)gvt[46].fn)((char*)theGraphicsMan + gvt[46].delta, payload, str_part, 0, 1, -1);
        }
        cMemMan_free(data);
    }
    data = FILE_load(str_data_fonts_menu_ssh, 0x100);
    cPS2GraphicsMan* graphicsMan = GrMan;
    sVEnt_17F868* vt2 = (sVEnt_17F868*)graphicsMan->mVtbl;
    self->snd20 = ((Load_17F868)vt2[46].fn)((char*)graphicsMan + vt2[46].delta, (char*)data + data[5], str_menu, 0, 1, -1);
    if (data) {
        cMemMan_free(data);
    }
    for (int i = 0; i < 2; i++) {
        self->objs[i] = 0;
    }
    self->cur = self->objs[0] = cDebugMenuScreen_cDebugMenuScreen(cMemMan_alloc(0x8C48, str_DebugMenu, 0, 0));
    self->objs[1] = func_0017D298((char*)cMemMan_alloc(0x1C18, str_LobbyMenu, 0, 0));
    if (SSXApp->unk_74) {
        self->cur = self->objs[1];
    } else {
        self->cur = self->objs[0];
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE_gainFocus);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cGraphicsMan_UploadTextures(void* p);
extern int cDepthFog_m_Enable;
struct sVec3K17FA60 {
    int x, y, z;
    sVec3K17FA60() { x = 0; y = 0; z = 0; }
};
typedef void (*tFn_17FA60)(void*, const sVec3K17FA60&);

/**
 * Gives the debug front end focus: uploads the textures, clears cDepthFog_m_Enable and sets a black clear colour.
 *
 * Calls cGraphicsMan_UploadTextures (On Tour cGraphicsMan::UploadTextures), then GrMan's slot 15 (SetClearColor) with (0, 0, 0).
 */
extern "C" void cDebugFE_gainFocus(void* self)
{
    char* graphicsMan;
    char* vt;
    cGraphicsMan_UploadTextures((char*)GrMan);
    cDepthFog_m_Enable = 0;
    graphicsMan = (char*)GrMan;
    vt = *(char**)(graphicsMan + 0x10D8);
    ((tFn_17FA60)*(void**)(vt + 0x7C))(graphicsMan + *(short*)(vt + 0x78), sVec3K17FA60());
}
#endif

extern "C" void* cFadeScreen_loseFocus(void* self);

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus from the debug front end: the fade screen's loseFocus result as a bool.
 *
 * @return 1 when cFadeScreen_loseFocus() reports the fade done, otherwise 0
 */
int cDebugFE_loseFocus(void* self)
{
    return (cFadeScreen_loseFocus(self) != 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE_purge);
#ifdef SKIP_ASM
extern "C" void func_003916C0(void* p, int flags);

struct sVEi_0017FAD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDebugFE_0017FAD0 {
    char pad_0x00[0xC];
    char* objs[2];      // 0xC
    char pad_0x14[4];
    void* font;         // 0x18
    int snd1C;          // 0x1C
    int snd20;          // 0x20
};

/**
 * Releases the debug front end's screens, textures and font, and unregisters both front ends from the app.
 *
 * Deletes the two screens (+0xC, +0x10: debug menu and lobby menu) through their destructor virtual (slot 1, flags
 * 3), zeroes SSXApp->RealFE and SSXApp->DebugFE, frees the two texture ids loaded by cDebugFE_load (+0x20, +0x1C,
 * when >= 0) through GrMan slot 50, and deletes the font (+0x18) with func_003916C0(font, 3). The local view calls
 * the texture ids snd1C/snd20; they are On Tour's ParticleTID and BoxTID.
 */
extern "C" void cDebugFE_purge(sDebugFE_0017FAD0* self)
{
    for (int i = 0; i < 2; i++)
    {
        char* screen = self->objs[i];
        if (screen != 0)
        {
            sVEi_0017FAD0* vt = *(sVEi_0017FAD0**)screen;
            vt[1].fn(screen + vt[1].delta, 3);
        }
    }
    char* app = (char*)SSXApp;
    *(int*)(app + 0x7C) = 0;
    *(int*)(app + 0x80) = 0;
    if (self->snd20 >= 0)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEi_0017FAD0* vt = (sVEi_0017FAD0*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, self->snd20);
    }
    if (self->snd1C >= 0)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEi_0017FAD0* vt = (sVEi_0017FAD0*)ps2GraphicsMan->mVtbl;
        vt[50].fn((char*)ps2GraphicsMan + vt[50].delta, self->snd1C);
    }
    if (self->font != 0)
        func_003916C0(self->font, 3);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/debugfe", cDebugFE_update);
#ifdef SKIP_ASM
class cFEObjK17FB98 {
public:
    virtual void v01();
    virtual void v02();
};
extern "C" void func_00231D18(void* self);
extern "C" void func_0017F2D0(void* p);

/**
 * Runs one frame of the debug front end: the fade, the active screen and the heartbeat.
 *
 * Calls func_00231D18 (On Tour cFadeScreen::update); while the app's word +0x4 is 0 it updates the active screen
 * (+0x14, virtual slot 2); then func_0017F2D0 (On Tour cHeartBeat::update) on the heartbeat at +0x24.
 */
extern "C" void cDebugFE_update(void* self)
{
    func_00231D18(self);
    if (*(int*)((char*)SSXApp + 4) == 0)
        (*(cFEObjK17FB98**)((char*)self + 0x14))->v02();
    func_0017F2D0((char*)self + 0x24);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/debugfe", cDebugFE_render);
#ifdef SKIP_ASM
extern char t4mat_Identity[];
extern "C" int func_00231D60(void* self);
extern "C" void func_0017F500(void* self, int a1);

struct sVEnt_17FBF0 { short delta; short index; void* fn; };
typedef int (*FnI_17FBF0)(void*);
typedef void (*FnV_17FBF0)(void*);
typedef void (*FnP_17FBF0)(void*, void*);
typedef void (*FnView_17FBF0)(void*, int, int, float, float, float, float, float, float);

struct sVec4_17FBF0 {
    float x, y, z, w;
    sVec4_17FBF0() {}
    sVec4_17FBF0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

struct sRS_17FBF0 {
    int f0;
    unsigned int lo : 2;
    unsigned int b2 : 5;
    unsigned int b7 : 5;
    unsigned int b12 : 8;
    unsigned int b20 : 2;
    unsigned int b22 : 1;
    unsigned int b23 : 2;
    unsigned int hi : 7;
    unsigned int c0 : 5;
    unsigned int c5 : 5;
    unsigned int chi : 22;
};

static inline sRS_17FBF0* rs_17FBF0(char* m) { return *(sRS_17FBF0**)(m + 0xE84); }
static inline void setB23_17FBF0(sRS_17FBF0* r, int v) { r->b23 = v; }
static inline void setB20_17FBF0(sRS_17FBF0* r, int v) { r->b20 = v; }
static inline void setB12_17FBF0(sRS_17FBF0* r, int v) { r->b12 = v; }
static inline void setB2_17FBF0(sRS_17FBF0* r, int v) { r->b2 = v; }
static inline void setB22_17FBF0(sRS_17FBF0* r, int v) { r->b22 = v; }
static inline void setC5_17FBF0(sRS_17FBF0* r, int v) { r->c5 = v; }

/**
 * Draws the debug front end: a full-screen 640x480 view, the heartbeat and the active screen.
 *
 * Returns 0 at once when GrMan's slot 17 returns 0. Draws the fade overlay with func_00231D60 (cFadeScreen's
 * render); unless that reports the screen fully faded it sets a black clear colour (slot 15), the 640x480 viewport
 * (slot 26), the identity matrix (slot 34) and the render-state bits, draws the heartbeat (func_0017F500 on +0x24
 * with the texture at +0x1C) and calls the active screen's render virtual (+0x14, slot 3). Always ends with GrMan
 * slot 20.
 *
 * @return 0 when GrMan's slot 17 returns 0, otherwise 1
 */
extern "C" int cDebugFE_render(char* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVEnt_17FBF0* vt = (sVEnt_17FBF0*)ps2GraphicsMan->mVtbl;
    if (!((FnI_17FBF0)vt[17].fn)((char*)ps2GraphicsMan + vt[17].delta)) {
        return 0;
    }
    if (func_00231D60(self) == 0) {
        cPS2GraphicsMan* graphicsMan1 = GrMan;
        sVEnt_17FBF0* vt1 = (sVEnt_17FBF0*)graphicsMan1->mVtbl;
        char* this1 = (char*)graphicsMan1 + vt1[15].delta;
        sVec4_17FBF0 pos;
        float one = 1.0f;
        sVec4_17FBF0 col(one, 0.0f, 0.0f, 0.0f);
        float zeroF = 0.0f;
        pos.x = zeroF;
        pos.y = zeroF;
        pos.z = zeroF;
        ((FnP_17FBF0)vt1[15].fn)(this1, &pos);
        cPS2GraphicsMan* graphicsMan2 = GrMan;
        sVEnt_17FBF0* vt2 = (sVEnt_17FBF0*)graphicsMan2->mVtbl;
        ((FnView_17FBF0)vt2[26].fn)((char*)graphicsMan2 + vt2[26].delta, 0, 0, zeroF, zeroF, 640.0f, 480.0f, -1.0f, one);
        cPS2GraphicsMan* graphicsMan3 = GrMan;
        sVEnt_17FBF0* vt3 = (sVEnt_17FBF0*)graphicsMan3->mVtbl;
        ((FnP_17FBF0)vt3[34].fn)((char*)graphicsMan3 + vt3[34].delta, t4mat_Identity);
        cPS2GraphicsMan* graphicsMan4 = GrMan;
        setB23_17FBF0(rs_17FBF0((char*)graphicsMan4), 2);
        setB20_17FBF0(rs_17FBF0((char*)graphicsMan4), 3);
        setB12_17FBF0(rs_17FBF0((char*)graphicsMan4), 0xD);
        setB2_17FBF0(rs_17FBF0((char*)graphicsMan4), 5);
        setB22_17FBF0(rs_17FBF0((char*)graphicsMan4), 1);
        func_0017F500(self + 0x24, *(int*)(self + 0x1C));
        setC5_17FBF0(rs_17FBF0((char*)GrMan), 9);
        void* menu = *(void**)(self + 0x14);
        sVEnt_17FBF0* mvt = *(sVEnt_17FBF0**)menu;
        ((FnV_17FBF0)mvt[3].fn)((char*)menu + mvt[3].delta);
    }
    cPS2GraphicsMan* graphicsMan5 = GrMan;
    sVEnt_17FBF0* vt5 = (sVEnt_17FBF0*)graphicsMan5->mVtbl;
    ((FnV_17FBF0)vt5[20].fn)((char*)graphicsMan5 + vt5[20].delta);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_getState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void* cInput_getState(int, int) __asm__("cInput_getState__FPvi");

/**
 * Reads one of the eight front-end input states: maps @p k 0-7 to input ids 0x72-0x79 for cInput_getState(mpInput).
 *
 * Any other @p k reads id 0. The result is left in the return register although the function is declared void here
 * (On Tour's getState returns bool).
 *
 * @param k  the state index, 0-7 (On Tour char state)
 */
extern "C" void cFEInput_getState(void* self, signed char k)
{
    int id = 0;
    switch (k) {
    case 0: id = 0x72; break;
    case 1: id = 0x73; break;
    case 2: id = 0x74; break;
    case 3: id = 0x75; break;
    case 4: id = 0x76; break;
    case 5: id = 0x77; break;
    case 6: id = 0x78; break;
    case 7: id = 0x79; break;
    }
    cInput_getState(*(int*)((char*)self + 0x18), id);
}
#endif

// PORT: the real function is C++ void* cInput_getState(void* self, int) (bx/execman); these thunks pass an int handle.
void* cInput_getState(int, int) __asm__("cInput_getState__FPvi");

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isStart__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's start state is active: cInput_getState(mpInput, 0x70).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isStart(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x70);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isSelect__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's select state is active: cInput_getState(mpInput, 0x71).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isSelect(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x71);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isNext__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's next state is active: cInput_getState(mpInput, 0x7A).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isNext(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7a);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isPrevious__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's previous state is active: cInput_getState(mpInput, 0x7B).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isPrevious(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7b);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isOption__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's option state is active: cInput_getState(mpInput, 0x7C).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isOption(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7c);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isMisc__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's misc state is active: cInput_getState(mpInput, 0x7D).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isMisc(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7d);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isNextHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "next" button is held on the front-end input: cInput_getState(mpInput, 0x97).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isNextHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x97);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isPreviousHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "previous" button is held on the front-end input: cInput_getState(mpInput, 0x98).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isPreviousHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x98);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isOptionHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "option" button is held on the front-end input: cInput_getState(mpInput, 0x99).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isOptionHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x99);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isMiscHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "misc" button is held on the front-end input: cInput_getState(mpInput, 0x9A).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isMiscHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x9a);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isNextReleased__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "next" button was just released on the front-end input: cInput_getState(mpInput, 0x9B).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isNextReleased(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x9b);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isPreviousReleased__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "previous" button was just released on the front-end input: cInput_getState(mpInput, 0x9C).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isPreviousReleased(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x9c);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isOptionReleased__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "option" button was just released on the front-end input: cInput_getState(mpInput, 0x9D).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isOptionReleased(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x9d);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isMiscReleased__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the "misc" button was just released on the front-end input: cInput_getState(mpInput, 0x9E).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isMiscReleased(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x9e);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isUp__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's up state is active: cInput_getState(mpInput, 0x72).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isUp(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x72);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isDown__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's down state is active: cInput_getState(mpInput, 0x73).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isDown(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x73);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLeft__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's left state is active: cInput_getState(mpInput, 0x74).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLeft(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x74);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRight__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether right was pressed on the front-end input: cInput_getState(mpInput, 0x75), getState's index 3.
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRight(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x75);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isUpHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether up is held on the front-end input: cInput_getState(mpInput, 0x93).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isUpHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x93);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isDownHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether down is held on the front-end input: cInput_getState(mpInput, 0x94).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isDownHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x94);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLeftHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether left is held on the front-end input: cInput_getState(mpInput, 0x95).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLeftHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x95);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRightHeld__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether right is held on the front-end input: cInput_getState(mpInput, 0x96).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRightHeld(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x96);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRShift1__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's R1 state is active: cInput_getState(mpInput, 0x7E).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRShift1(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7e);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRShift2__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's R2 state is active: cInput_getState(mpInput, 0x7F).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRShift2(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x7f);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLShift1__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's L1 state is active: cInput_getState(mpInput, 0x80).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLShift1(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x80);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLShift2__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input's L2 state is active: cInput_getState(mpInput, 0x81).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLShift2(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x81);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether R1 is held on the front-end input: cInput_getState(mpInput, 0x82).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRShift1Held(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x82);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isRShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether R2 is held on the front-end input: cInput_getState(mpInput, 0x83).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isRShift2Held(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x83);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether L1 is held on the front-end input: cInput_getState(mpInput, 0x84).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLShift1Held(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x84);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isLShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether L2 is held on the front-end input: cInput_getState(mpInput, 0x85).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isLShift2Held(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x85);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the camera-zoom control is active on the front-end input: cInput_getState(mpInput, 0x90).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isCamZoom(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x90);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the camera-pan control is active on the front-end input: cInput_getState(mpInput, 0x92).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isCamPan(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x92);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the camera-rotate control is active on the front-end input: cInput_getState(mpInput, 0x91).
 *
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cFEInput_isCamRotate(void* self)
{
    return cInput_getState(*(int*)((char*)self + 0x18), 0x91);
}
#endif

extern "C" void* cInput_getValue(int, int);

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_getCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the camera-zoom control's analogue value: cInput_getValue(mpInput, 0x90).
 *
 * @return cInput_getValue's result (On Tour returns float; declared void* here)
 */
void* cFEInput_getCamZoom(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x18), 0x90);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_getCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Returns the camera-pan control's analogue value: cInput_getValue(mpInput, 0x92).
 *
 * @return cInput_getValue's result (On Tour returns float; declared void* here)
 */
void* cFEInput_getCamPan(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x18), 0x92);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_getCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Returns the camera-rotate control's analogue value: cInput_getValue(mpInput, 0x91).
 *
 * @return cInput_getValue's result (On Tour returns float; declared void* here)
 */
void* cFEInput_getCamRotate(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x18), 0x91);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180300);
#ifdef SKIP_ASM
extern "C" int func_00180300(void* self)
{
    int r = 0;
    if (cInput_getState(*(int*)((char*)self + 0x18), 0x72) || cInput_getState(*(int*)((char*)self + 0x18), 0x73) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x74) || cInput_getState(*(int*)((char*)self + 0x18), 0x75) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x76) || cInput_getState(*(int*)((char*)self + 0x18), 0x77) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x78) || cInput_getState(*(int*)((char*)self + 0x18), 0x79) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x7A) || cInput_getState(*(int*)((char*)self + 0x18), 0x7B) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x7D) || cInput_getState(*(int*)((char*)self + 0x18), 0x7C) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x90) || cInput_getState(*(int*)((char*)self + 0x18), 0x92) ||
        cInput_getState(*(int*)((char*)self + 0x18), 0x91) || cInput_getState(*(int*)((char*)self + 0x18), 0x71))
        r = 1;
    return r;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180478);
#ifdef SKIP_ASM

extern "C" void func_00180478(void* self, unsigned char idx)
{
    int v = *(int*)((char*)SSXApp + (idx << 2) + 0xB0);
    *(unsigned char*)((char*)self + 0x4) = idx;
    *(int*)((char*)self + 0x18) = v;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_isActive);
#ifdef SKIP_ASM
/**
 * Tells whether the front-end input has a controller that is connected.
 *
 * @return 1 when the controller pointer (+0x18) and its device are set, otherwise 0
 */
extern "C" int cFEInput_isActive(void* self)
{
    void* input = *(void**)((char*)self + 0x18);
    if (input != 0) {
        return **(int**)input != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001804B8);
#ifdef SKIP_ASM
extern void* D_004A33F0;
extern "C" int func_00266BA8(void* mgr, int a1, int* out0, int* out1);

struct sDebugFE1804B8 {
    char pad_0x00[0xC];
    int found; // 0xC
    int a;     // 0x10
    int b;     // 0x14
};

extern "C" void func_001804B8(sDebugFE1804B8* self)
{
    self->found = 0;
    if (D_004A33F0 != 0) {
        self->b = 0;
        self->a = 0;
        if (func_00266BA8(D_004A33F0, 0, &self->a, &self->b) != 0)
            self->found = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180508);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned short* func_001DD580(unsigned short* p);
extern "C" int func_003977E8(void* p);
extern "C" int func_0039FB30(void* p, int a1, int a2);
extern int D_0046D1A0[];
extern int cUIScreen_vtbl[];
extern int cUIGroup_vtbl[];

extern "C" void* func_00180508(char* self, int a1)
{
    *(char**)(self + 4) = self;
    *(char**)(self + 0) = self;
    *(int*)(self + 0xC) = 0;
    *(int*)(self + 0x10) = 0;
    *(int**)(self + 8) = cUIScreen_vtbl;
    func_001DD580((unsigned short*)(self + 0x14));
    func_003977E8(self + 0x18);
    *(int*)(self + 0x34) = 0;
    *(int*)(self + 0x38) = 0;
    *(int*)(self + 0x3C) = 0;
    func_0039FB30(self + 0x40, 0, 0);
    *(int**)(self + 0x48) = cUIGroup_vtbl;
    func_003977E8(self + 0xB4);
    *(int*)(self + 0xD0) = a1;
    *(int**)(self + 8) = D_0046D1A0;
    *(int*)(self + 0xD8) = 0;
    *(int*)(self + 0xDC) = 0;
    *(int*)(self + 0xE0) = 0;
    *(int*)(self + 0xE4) = 1;
    *(int*)(self + 0xE8) = 1;
    *(int*)(self + 0x170) = 10;
    *(int*)(self + 0xD4) = 0x2D;
    *(char*)(self + 0xEC) = 0;
    *(int*)(self + 0x16C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001805D8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);

extern "C" void func_001805D8(void* self, const char* text)
{
    char* buf = (char*)self + 0xEC;
    strcpy(buf, text);
    *(int*)((char*)self + 0x16C) = strlen(buf);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180618);
#ifdef SKIP_ASM
extern "C" void func_00180618(void* self)
{
    int keyIndex = *(int*)((char*)self + 0x170);
    if (keyIndex == 0 || keyIndex == 10 || keyIndex == 20 || keyIndex == 30) {
        *(int*)((char*)self + 0x170) += 9;
    } else if (keyIndex == 39 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 37;
    } else {
        *(int*)((char*)self + 0x170) -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180678);
#ifdef SKIP_ASM
extern "C" void func_00180678(void* self)
{
    int keyIndex = *(int*)((char*)self + 0x170);
    if (keyIndex == 9 || keyIndex == 19 || keyIndex == 29 || keyIndex == 39) {
        *(int*)((char*)self + 0x170) -= 9;
    } else if (keyIndex == 37 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 39;
    } else {
        *(int*)((char*)self + 0x170) += 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001806D8);
#ifdef SKIP_ASM
extern "C" void func_001806D8(void* self)
{
    int keyIndex = *(int*)((char*)self + 0x170);
    if (keyIndex >= 0 && keyIndex < 10) {
        if (keyIndex == 8 && *(int*)((char*)self + 0xE0) == 0) {
            *(int*)((char*)self + 0x170) = 39;
        } else {
            *(int*)((char*)self + 0x170) += 30;
        }
    } else if (keyIndex >= 10 && keyIndex < 20 && *(int*)((char*)self + 0xD8) == 0) {
        if (keyIndex == 18 && *(int*)((char*)self + 0xE0) == 0) {
            *(int*)((char*)self + 0x170) = 39;
        } else {
            *(int*)((char*)self + 0x170) += 20;
        }
    } else {
        *(int*)((char*)self + 0x170) -= 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180768);
#ifdef SKIP_ASM
extern "C" void func_00180768(void* self)
{
    int keyIndex = *(int*)((char*)self + 0x170);
    if (keyIndex >= 30 && keyIndex < 40) {
        if (*(int*)((char*)self + 0xD8) != 0) {
            *(int*)((char*)self + 0x170) = keyIndex - 30;
        } else {
            *(int*)((char*)self + 0x170) = keyIndex - 20;
        }
    } else if (keyIndex == 28 && *(int*)((char*)self + 0xE0) == 0) {
        *(int*)((char*)self + 0x170) = 39;
    } else {
        *(int*)((char*)self + 0x170) += 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001807C8);
#ifdef SKIP_ASM
extern "C" int func_001807C8(void* self)
{
    int toggled = *(int*)((char*)self + 0xe4) ^ 1;
    *(int*)((char*)self + 0xe4) = toggled;
    return toggled;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001807D8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sStarsK1807D8 {
    char pad_0x000[0xEC];
    char name[0x40];    // 0xEC
    char stars[0x40];   // 0x12C
};

extern "C" char* func_001807D8(sStarsK1807D8* self)
{
    int n = strlen(self->name);
    int i;
    for (i = 0; i < n; i++) {
        self->stars[i] = '*';
    }
    self->stars[n] = 0;
    return self->stars;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180840);
#ifdef SKIP_ASM
struct cDbgKbdTarget {
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
    virtual void v19(int, int);
};

struct sDbgKbd {
    char pad0[0xD0];
    cDbgKbdTarget* obj;
    int maxLen;
    char padD8[0xC];
    int caps;
    char padE8[4];
    char buf[0x80];
    int cur;
    int key;
};

extern "C" void func_00180840(sDbgKbd* self)
{
    int k = self->key;
    if (k >= 0 && k < 10) {
        self->buf[self->cur] = self->key + '0';
        if (self->cur + 1 < self->maxLen)
            self->cur = self->cur + 1;
    } else if (k >= 10 && k < 36) {
        int c = !self->caps ? 'a' : 'A';
        self->buf[self->cur] = c + (self->key - 10);
        if (self->cur + 1 < self->maxLen)
            self->cur = self->cur + 1;
        self->buf[self->cur] = 0;
    } else if (k == 36) {
        if (self->cur > 0)
            self->cur = self->cur - 1;
        self->buf[self->cur] = 0;
    } else if (k == 37) {
        if (self->cur + 1 < self->maxLen)
            self->cur = self->cur + 1;
        self->buf[self->cur] = 0;
    } else if (k == 38) {
        self->buf[self->cur] = ' ';
        if (self->cur + 1 < self->maxLen)
            self->cur = self->cur + 1;
    }
    if (self->key == 39)
        self->obj->v19(0, 13);
    else
        self->obj->v19(0, 12);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_001809B0);
#ifdef SKIP_ASM
struct sStr10 { char c[10]; };
extern sStr10 D_0045D550;
extern char str_arrLeft[];
extern char str_arrRight[];
extern char str_spc[];
extern char str_end[];
extern "C" char* func_004162D0(char*, const char*);

extern "C" void func_001809B0(void* self, int c, char* dst)
{
    char buf[2];
    *(sStr10*)dst = D_0045D550;
    if (c < 10) {
        buf[0] = c + '0';
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c < 36) {
        buf[0] = c + 'A' - 10;
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (c == 36) {
        func_004162D0(dst, str_arrLeft);
    } else if (c == 37) {
        func_004162D0(dst, str_arrRight);
    } else if (c == 38) {
        func_004162D0(dst, str_spc);
    } else if (c == 39) {
        func_004162D0(dst, str_end);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180A90);
#ifdef SKIP_ASM
struct sStr8 { char c[8]; };
extern char str_cursor[];
extern char str_arrLeft[];
extern char str_arrRight[];
extern char str_spc[];
extern char str_end[];
extern "C" char* func_004162D0(char*, const char*);

extern "C" void func_00180A90(void* self, int keyIndex, char* dst)
{
    char buf[2];
    *(sStr8*)dst = *(sStr8*)str_cursor;
    if (keyIndex < 10) {
        buf[0] = keyIndex + '0';
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (keyIndex < 36) {
        buf[0] = keyIndex + 'A' - 10;
        buf[1] = 0;
        func_004162D0(dst, buf);
    } else if (keyIndex == 36) {
        func_004162D0(dst, str_arrLeft);
    } else if (keyIndex == 37) {
        func_004162D0(dst, str_arrRight);
    } else if (keyIndex == 38) {
        func_004162D0(dst, str_spc);
    } else if (keyIndex == 39) {
        func_004162D0(dst, str_end);
    }
}
#endif

