#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "fe/backgroundman.h"
#include "ealib/shape.h"
#include "online/onlineevent.h"
#include "render/font.h"
#include "bx/math3d.h"
#include "fe/debuglobbymenus.h"
#include "fe/debugfe.h"
#include "fe/screenkeyboard.h"
#include "fe/heartbeat.h"
#include "fe/feinput.h"
#include "online/lobbyman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "online/commsystem.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/debugfe", cDebugMenuScreen_draw);
#ifdef SKIP_ASM
// FAKEMATCH: the first row reads self->cursor through an integer temp, `__SIZE_TYPE__ hi = (__SIZE_TYPE__)self +
// 0x8000;
// ... *(int*)(hi + 0xc1c)` (same address, same single load; the unflagged non-pointer base makes sched1's alias check
// keep the
// load after the 12 colour stores); plain form 97.65%. Byte-identical and behaves like the plain form; landed marked
// under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int cBENewRaceInterface_bIsBamLocation;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" char* cBENewRaceInterface_getEventName(void* iface);
extern "C" char* cBENewRaceInterface_getPathName(void* iface);
extern "C" char* cBENewRaceInterface_getModeName(void* iface);
extern "C" int cBENewPlayerInterface_isFirstTimeWC(void* iface, int i);
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int i);
extern "C" char* cBECharacterInterface_getName(void* iface, int i);
extern "C" void cBackgroundMan_renderImage(void* self);
extern "C" void cFont_textf(void* self, float x, float y, const char* fmt, ...);
extern "C" void cFont_textf1(void* self, float x, float y, float sx, float sy, const char* str, int n);
extern "C" void cFont_centertextf(void* self, float x, float y, const char* fmt, ...);
extern "C" void cFont_righttextf(void* self, float x, float y, const char* fmt, ...);
extern "C" void cCommSystem_netConnLocalAddr(void* comm, int* info);
extern "C" char* cAudioInterface_bxAudio();
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" void cFont_centertext(void* self, float x, float y, const char* str);
extern "C" void cFont_righttext(void* self, const char* str, float x, float y);
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


struct sDebugMenuScreen {
    char pad0[0x8C10];
    cBackgroundMan popup;            // 0x8C10
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
extern "C" void cDebugMenuScreen_draw(cDebugMenuScreen* self)
{
    cBackgroundMan* popup = &(*(&self->BackgroundMan));
    if (popup->miActiveImage >= 0) {
        cBackgroundMan_renderImage(popup);
        return;
    }
    cFont_Dbg* font = (cFont_Dbg*)SSXApp->DebugFE->Font;
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
    cFont_righttext(font, cBENewRaceInterface_getPathName(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 1 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Game_Mode);
    cFont_righttext(font, cBENewRaceInterface_getModeName(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 2 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Game_Event);
    cFont_righttext(font, cBENewRaceInterface_getEventName(newRace), 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 3 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_1st_Time_WC_Flag);
    cFont_righttextf(font, 580.0f, y, str_pct_d_b, cBENewPlayerInterface_isFirstTimeWC(intPlayer, 0));
    y += lineheight;
    if (cBENewRaceInterface_bIsBamLocation != 0)
        sprintf(buf, str_pct_s_pct_s_pct_s, cBENewRaceInterface_getLocation(newRace) + 4, cBENewRaceInterface_getLocation(newRace) + 0x44, cBENewRaceInterface_getLocation(newRace) + 0x34);
    else
        sprintf(buf, str_pct_s_pct_s_b, cBENewRaceInterface_getLocation(newRace) + 0x34, cBENewRaceInterface_getLocation(newRace) + 0x34);
    font->setColor(self->SelectedItem == 4 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Track);
    cFont_righttext(font, buf, 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 5 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Character);
    cFont_righttext(font, cBECharacterInterface_getName(character, 0), 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 6 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Control_Map);
    cFont_righttextf(font, 580.0f, y, str_pct_d_b, cBENewPlayerInterface_getControllerMapping(intPlayer, 0));
    y += lineheight;
    font->setColor(self->SelectedItem == 7 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Board);
    cFont_righttext(font, D_004A12E0[self->CurrentBoard], 580.0f, y);
    y += lineheight;
    font->setColor(self->SelectedItem == 8 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Number_of_AI_riders);
    cFont_righttextf(font, 580.0f, y, str_pct_d_b, cBENewRaceInterface_newRace.iNumberAI);
    y += lineheight;
    font->setColor(self->SelectedItem == 9 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Screen_Calibration);
    cFont_righttextf(font, 580.0f, y, str_pct_d_b, self->CurrentBack);
    y += lineheight;
    font->setColor(self->SelectedItem == 10 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_HDTV);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        cFont_righttextf(font, 580.0f, y, str_pct_s, ps2GraphicsMan->GetDTV() ? str_On_480p : str_Off_NTSC);
    }
    y += lineheight;
    font->setColor(self->SelectedItem == 11 ? selectColor : normalColor);
    cFont_text(font, 60.0f, y, str_Playlist);
    cFont_righttextf(font, 580.0f, y, str_pct_s, cAudioInterface_bxAudio() + (self->CurrentPlaylist * 20 + 24740));
    y += lineheight;
    unsigned int ip = 0;
    if (cCommSystem_mCommSystem != 0)
        cCommSystem_netConnLocalAddr(cCommSystem_mCommSystem, (int*)&ip);
    char ipbuf[0x20];
    sprintf(ipbuf, str_pct_d_pct_d_pct_d_pct_d, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    sModeNamesDbg modes = D_0045CFB8;
    if (cCommSystem_mCommSystem != 0)
        font->setColor(self->SelectedItem == 12 ? selectColor : normalColor);
    else
        font->setColor(self->SelectedItem == 12 ? selectColor : disabledColor);
    cFont_textf(font, 60.0f, y, str_Network_pct_s, ipbuf);
    cFont_righttextf(font, 580.0f, y, str_pct_s, modes.name[self->NetworkMode]);
    y += lineheight;
    y += 5.0f;
    font->setColor(self->SelectedItem == 13 ? selectColor : disabledColor);
    cFont_centertextf(font, 320.0f, y, str_Lobby);
    font->setColor(self->SelectedItem == 14 ? selectColor : normalColor);
    cFont_centertextf(font, 320.0f, y + lineheight, str_Real_Front_End);
    if ((int)SSXApp->LoadingModule == 0) {
        tARGB promptColour;
        promptColour.A = 1.0f;
        promptColour.R = 0.9f;
        promptColour.G = 0.9f;
        promptColour.B = 0.0f;
        font->setColor(promptColour);
        cFont_centertext(font, 320.0f, 420.0f, str_press_start_to_play);
    }
    tARGB animCountColour;
    animCountColour.A = 1.0f;
    animCountColour.R = 0.5f;
    animCountColour.G = 0.5f;
    animCountColour.B = 0.5f;
    font->setColor(animCountColour);
    cFont_textf1(font, 60.0f, 420.0f, 0.5f, 0.5f, str_Anims_pct_d, 7);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugMenuScreen_setGamePath);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct sDbgRaceSel17D008 {
    char pad_0x00[0x48];
    signed char mode;  // 0x48
    signed char slot;  // 0x49
};
class cBENewRaceIfaceK17D008 {
public:
    char pad_0x00[0xC];
    virtual int submit();
};
extern "C" void cBENewRaceInterface_setGamePath(void* iface, int idx);
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);

/**
 * Debug menu: switches the new race to another game path, keeping a remembered game mode per path.
 *
 * Saves cBENewRaceInterface_newRace's current mode in the screen's mode table at the current path's index, sets
 * the new path with cBENewRaceInterface_setGamePath, selects the mode remembered for it with cBENewRaceInterface_setGameMode and
 * calls the interface's virtual slot 1 (apply).
 *
 * @param idx  the game path to switch to (On Tour eBEGamePath)
 */
extern "C" void cDebugMenuScreen_setGamePath(cDebugMenuScreen* self, int idx)
{
    cBENewRaceIfaceK17D008* iface = (cBENewRaceIfaceK17D008*)cBE_getInterface_Fv(SSXApp->BE, 0);
    (self)->LastGamePathGameMode[cBENewRaceInterface_newRace.cGamePath] = cBENewRaceInterface_newRace.cGameMode;
    cBENewRaceInterface_setGamePath(iface, idx);
    cBENewRaceInterface_setGameMode(iface, (self)->LastGamePathGameMode[idx]);
    iface->submit();
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D0A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cCommSystem_openDSockChannel(void* comm, const char* addr, int port, int host);
extern "C" void cCommSystem_netConnLocalAddr(void* comm, int* info);
extern "C" int cCommSystem_isChannelConnected(void* comm, int ch);
extern "C" void cCommSystem_update(void* comm);
extern "C" void SYNCTASK_run(int a);
extern "C" void THREAD_yieldticks(int n);
extern "C" void cGameComm_construct(int ch, int host);
extern "C" void func_00266D58(void* p, int ip);
extern "C" void func_00266E30(void* p);
extern void* VoipMan;
extern char str_10_10_53_35[] __attribute__((aligned(8)));
extern char str_10_10_53_25[] __attribute__((aligned(8)));

struct sNetCfg_D0A8 {
    int active;             // 0x0
    int f4;
    int host;               // 0x8
    unsigned short port;    // 0xC
};
extern sNetCfg_D0A8 cBENetworkInterface_networkOpt_D0A8 __asm__("cBENetworkInterface_networkOpt");

/**
 * Opens the debug direct link to a fixed partner console when a network game is set, then starts the game comm on it.
 *
 * Fetches the network interface (cBE_getInterface__Fv kind 7; the result is unused) and returns at once unless
 * cBENetworkInterface_networkOpt.bIsNetworkGame is set. Reads this console's address with cCommSystem_netConnLocalAddr: a console at
 * 10.10.53.25 (0x0A0A3519) dials "10.10.53.35" (str_10_10_53_35), any other dials "10.10.53.25" (str_10_10_53_25). Opens a
 * DirtySock channel with cCommSystem_openDSockChannel on networkOpt.iPort, as host or client by bIsHost, and spins
 * until the channel reports connected (cCommSystem_isChannelConnected), running the comm update cCommSystem_update, SYNCTASK_run(0) and
 * THREAD_yieldticks(1) on each pass. Then creates the game comm (cGameComm_construct with the channel and bIsHost) and
 * connects and enables voice chat on VoipMan (func_00266D58, func_00266E30). cDebugMenuScreen_update calls it when
 * the debug start menu starts a game.
 *
 * Likely bug: voice chat is given this console's own address (from cCommSystem_netConnLocalAddr), not the partner's; func_002639E0
 * passes the opponent's. The wait loop has no timeout.
 */
extern "C" void func_0017D0A8(void)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt_D0A8.active == 0) {
        return;
    }
    char addr[32];
    int info[4];
    cCommSystem_netConnLocalAddr(cCommSystem_mCommSystem, info);
    if (info[0] == 0x0A0A3519) {
        __builtin_memcpy(addr, str_10_10_53_35, 12);
    } else {
        __builtin_memcpy(addr, str_10_10_53_25, 12);
    }
    int ch;
    if (cBENetworkInterface_networkOpt_D0A8.host) {
        ch = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, addr, cBENetworkInterface_networkOpt_D0A8.port, 1);
    } else {
        ch = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, addr, cBENetworkInterface_networkOpt_D0A8.port, 0);
    }
    while (cCommSystem_isChannelConnected(cCommSystem_mCommSystem, ch) == 0) {
        cCommSystem_update(cCommSystem_mCommSystem);
        SYNCTASK_run(0);
        THREAD_yieldticks(1);
    }
    cGameComm_construct(ch, cBENetworkInterface_networkOpt_D0A8.host);
    func_00266D58(VoipMan, info[0]);
    func_00266E30(VoipMan);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugMenuScreen_getMaxAIRiders);
#ifdef SKIP_ASM
/**
 * Debug menu: the most AI riders a race may have for a game path and mode.
 *
 * Five by default (a six-rider race), four for game path 2; modes 0, 4, 5 and 6 allow none and modes 1, 2 and 3
 * allow one.
 *
 * @param path  the game path (On Tour eBEGamePath)
 * @param mode  the game mode (On Tour eBEGameMode)
 * @return the maximum number of AI riders
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cDebugMenuScreen_getMaxAIRiders(cDebugMenuScreen* self, int path, int mode)
{
    int maxAIRiders = (path == 2) ? 4 : 5;
    switch (mode) {
    case 0: case 4: case 5: case 6:
        maxAIRiders = 0;
        break;
    case 1: case 2: case 3:
        maxAIRiders = 1;
        break;
    }
    return maxAIRiders;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", ContinueMenu_ContinueMenu);
#ifdef SKIP_ASM
extern void* ContinueMenu_vtbl[];
extern char str_Continue[];
void* cMenu_cMenu(void* self);
extern "C" void* cPopMenuItem_cPopMenuItem(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);

/**
 * Constructs the debug lobby's "Continue" menu: a cMenu with a single "Continue" command item.
 *
 * Runs cMenu_cMenu, stores the menu's vtable ContinueMenu_vtbl at +0x12C (its destructor is ContinueMenu__ContinueMenu), builds the item at
 * +0x130 with cPopMenuItem_cPopMenuItem and str_Continue and appends it (cMenu_addItem, index -1). The debug lobby screen
 * (cDebugLobbyScreen_cDebugLobbyScreen) keeps it at +0x604 and pushes it under every error and "declined" message.
 *
 * @return @p self
 */
extern "C" void* ContinueMenu_ContinueMenu(ContinueMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = ContinueMenu_vtbl;
    cPopMenuItem_cPopMenuItem(&self->unk_130, str_Continue);
    cMenu_addItem(self, &self->unk_130, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D260);
#ifdef SKIP_ASM
extern void* D_004A4E90;
extern "C" void func_0017D6F0(cDebugLobbyScreen* self, online_Event* ev);
extern "C" void func_00259230(void* ev);

/**
 * Passes one lobby event to the debug lobby screen, when one exists, and then to the real front end.
 *
 * The lobby manager's update (func_0025AC50) calls it for every event it pops from mEventList, with itself as @p self
 * (unused here). When the debug lobby screen pointer D_004A4E90 is set (cDebugLobbyScreen_cDebugLobbyScreen sets it, cDebugLobbyScreen__cDebugLobbyScreen clears it)
 * the event goes to func_0017D6F0; func_00259230 then hands it to the real front end's top UI state.
 *
 * @param ev  the event (its type at +0x0)
 */
extern "C" void func_0017D260(LobbyMan* self, void* ev)
{
    if (D_004A4E90 != 0)
        func_0017D6F0((cDebugLobbyScreen*)D_004A4E90, (online_Event*)ev);
    func_00259230(ev);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugLobbyScreen_cDebugLobbyScreen);
#ifdef SKIP_ASM
extern void* cDebugLobbyScreen_vtbl[];
extern int cBXString_EmptyString;
extern void* D_004A4E90;
extern "C" void* cMenuSys_cMenuSys(void* self);
extern "C" void* AccountMenu_AccountMenu(void* self, void* a, void* b);
extern "C" void* PersonaMenu_PersonaMenu(void* self, void* a, void* b, void* c);
extern "C" void* LobbyMenu_LobbyMenu(void* self, void* a);
extern "C" void* RoomMenu_RoomMenu(void* self, void* a, void* b);
extern "C" void* ChatMenu_ChatMenu(void* self, void* a);
extern "C" void* ChallengeResponseMenu_ChallengeResponseMenu(void* self, void* a, void* b);
extern "C" void* AbortMenu_AbortMenu(void* self, void* a);


/**
 * Constructs the debug front end's lobby screen (cDebugLobbyScreen in the July 2003 preview) with its eight menus.
 *
 * Stores the vtable cDebugLobbyScreen_vtbl (slots: destructor cDebugLobbyScreen__cDebugLobbyScreen, update cDebugLobbyScreen_update, draw cDebugLobbyScreen_draw), zeroes the
 * state at +0x4 and builds the menu system at +0x8 (cMenuSys_cMenuSys). Each menu gets a one-pointer functor holding
 * this screen, which its command handler calls back: the account menu "Login Method" at +0x70 (AccountMenu_AccountMenu, Create
 * Account and Login), the "Continue" menu at +0x604 (ContinueMenu_ContinueMenu), "Select Persona" at +0x74C (PersonaMenu_PersonaMenu, with
 * Create Persona and Delete Persona), "Select Room" at +0xD2C (LobbyMenu_LobbyMenu), "Room Occupants" at +0x1044
 * (RoomMenu_RoomMenu, occupants and Chat), the chat menu at +0x1534 (ChatMenu_ChatMenu), the "You've been Challenged!" menu at
 * +0x18C0 (ChallengeResponseMenu_ChallengeResponseMenu, Accept and Decline) and the "Abort" menu at +0x1AAC (AbortMenu_AbortMenu). The three
 * cBXStrings at +0x1C04 (typed user name), +0x1C08 (typed password) and +0x1C0C (challenger's name) start empty. The
 * menu system's font, input and box texture (its +0x58, +0x5C, +0x60, here +0x60, +0x64, +0x68) are taken from the
 * debug front end's font (+0x18), SSXApp->Inputs[0] and its box texture (+0x20). Finally the screen registers itself in
 * D_004A4E90, which func_0017D260 uses to deliver lobby events. cDebugFE_load allocates it (7192 bytes, "LobbyMenu") as
 * its second screen.
 *
 * @return @p self
 */
extern "C" void* cDebugLobbyScreen_cDebugLobbyScreen(cDebugLobbyScreen* self)
{
    *(void***)self = cDebugLobbyScreen_vtbl;
    self->mState = 0;
    cMenuSys_cMenuSys(&self->mMenuSys);
    cDebugLobbyScreen::OnCreate onCreate;
    (*(void**)&onCreate.unk_0) = ((char*)self);
    cDebugLobbyScreen::OnCreate onLogin;
    (*(void**)&onLogin.unk_0) = ((char*)self);
    AccountMenu_AccountMenu(&self->mLoginMenu, &onCreate, &onLogin);
    ContinueMenu_ContinueMenu(&self->mContinueMenu);
    cDebugLobbyScreen::OnCreate onSelectPersona;
    (*(void**)&onSelectPersona.unk_0) = ((char*)self);
    cDebugLobbyScreen::OnCreate onCreatePersona;
    (*(void**)&onCreatePersona.unk_0) = ((char*)self);
    cDebugLobbyScreen::OnCreate onDeletePersona;
    (*(void**)&onDeletePersona.unk_0) = ((char*)self);
    PersonaMenu_PersonaMenu(&self->mPersonaMenu, &onSelectPersona, &onCreatePersona, &onDeletePersona);
    cDebugLobbyScreen::OnCreate onSelectRoom;
    (*(void**)&onSelectRoom.unk_0) = ((char*)self);
    LobbyMenu_LobbyMenu(&self->mRoomMenu, &onSelectRoom);
    cDebugLobbyScreen::OnCreate onSelectUser;
    (*(void**)&onSelectUser.unk_0) = ((char*)self);
    cDebugLobbyScreen::OnCreate onChat;
    (*(void**)&onChat.unk_0) = ((char*)self);
    RoomMenu_RoomMenu(&self->mOccupantsMenu, &onSelectUser, &onChat);
    cDebugLobbyScreen::OnCreate onMessage;
    (*(void**)&onMessage.unk_0) = ((char*)self);
    ChatMenu_ChatMenu(&self->mChatMenu, &onMessage);
    cDebugLobbyScreen::OnCreate onAcceptChallenge;
    (*(void**)&onAcceptChallenge.unk_0) = ((char*)self);
    cDebugLobbyScreen::OnCreate onDeclineChallenge;
    (*(void**)&onDeclineChallenge.unk_0) = ((char*)self);
    ChallengeResponseMenu_ChallengeResponseMenu(&self->mChallengedMenu, &onAcceptChallenge, &onDeclineChallenge);
    cDebugLobbyScreen::OnCreate onAbortChallenge;
    (*(void**)&onAbortChallenge.unk_0) = ((char*)self);
    AbortMenu_AbortMenu(&self->mAbortMenu, &onAbortChallenge);
    int emptyText = cBXString_EmptyString;
    cSSXApp* app = SSXApp;
    self->mUserName = (char*)emptyText;
    self->mPassword = (char*)emptyText;
    self->mChallenger = (char*)emptyText;
    self->mMenuSys.Font = (cFont*)((int)app->DebugFE->Font);
    self->mMenuSys.Input = (cInput*)((int)app->Inputs[0]);
    self->mMenuSys.BoxTexture = app->DebugFE->BoxTID;
    D_004A4E90 = ((char*)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugLobbyScreen__cDebugLobbyScreen);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cMenuItem__cMenuItem(void* self, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);
extern "C" void PersonaMenu__PersonaMenu(void* self, int flags);
extern "C" void AccountMenu__AccountMenu(void* self, int flags);
extern "C" void cMenuSys__cMenuSys(void* self, int flags);
void operator_delete(int* ptr);
extern void* cDebugLobbyScreen_vtbl[];
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

/**
 * Destroys the debug lobby screen: unregisters it, destroys its strings, menus and menu system, and frees it when
 * asked.
 *
 * Stores the vtable cDebugLobbyScreen_vtbl again, clears the screen pointer D_004A4E90, destroys the cBXStrings at +0x1C0C, +0x1C08
 * and +0x1C04, then the menus in reverse order: Abort (+0x1AAC), You've been Challenged (+0x18C0), the chat menu
 * (+0x1534, its 14 line items through their virtual destructors), Room Occupants (+0x1044, 24 items), Select Room
 * (+0xD2C, 12 items), Select Persona (+0x74C, PersonaMenu__PersonaMenu), Continue (+0x604) and Login Method (+0x70,
 * AccountMenu__AccountMenu), and the menu system (+0x8, cMenuSys__cMenuSys). Ends with cDebugScreen's vtable cDebugScreen_vtbl and
 * operator_delete when bit 0 of @p flags is set. The item and menu destructors run with flags 2 (no free).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the screen
 */
extern "C" void cDebugLobbyScreen__cDebugLobbyScreen(cDebugLobbyScreen* self, int flags)
{
    *(void***)self = cDebugLobbyScreen_vtbl;
    D_004A4E90 = 0;
    cBXString__cBXString(&self->mChallenger, 2);
    cBXString__cBXString(&self->mPassword, 2);
    cBXString__cBXString(&self->mUserName, 2);
    dtorA_17D3B0((char*)&self->mAbortMenu);
    dtorB_17D3B0((char*)&self->mChallengedMenu);
    dtorC_17D3B0((char*)&self->mChatMenu);
    dtorD_17D3B0((char*)&self->mOccupantsMenu);
    dtorE_17D3B0((char*)&self->mRoomMenu);
    PersonaMenu__PersonaMenu(&self->mPersonaMenu, 2);
    dtorF_17D3B0((char*)&self->mContinueMenu);
    AccountMenu__AccountMenu(&self->mLoginMenu, 2);
    cMenuSys__cMenuSys(&self->mMenuSys, 2);
    *(void***)self = cDebugScreen_vtbl;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugLobbyScreen_update);
#ifdef SKIP_ASM
extern "C" void cCommSystem_update(void*);
extern "C" void func_0025AC50(void*);
extern "C" void cMenuSys_input(void* stack);
extern "C" void func_0017DB18(cDebugLobbyScreen* self);

struct sMenuStack17D688 {
    int count;      // 0x0
    void* items[8]; // 0x4
};

static inline void* sMenuStack17D688_top(sMenuStack17D688* self)
{
    if (self->count > 0)
        return self->items[self->count - 1];
    return 0;
}

/**
 * Runs one frame of the debug lobby screen: the network and lobby updates, menu input and the screen's state machine.
 *
 * Calls the comm system update cCommSystem_update (cCommSystem_mCommSystem) and the lobby manager update func_0025AC50
 * (LobbyMan_mLobbyMan), which delivers queued lobby events to func_0017D6F0 through func_0017D260. When the menu system
 * at +0x8 has a menu open it runs its input (cMenuSys_input), then func_0017DB18 advances the state. cDebugFE_update
 * calls it through the active screen's virtual slot 2.
 */
extern "C" void cDebugLobbyScreen_update(cDebugLobbyScreen* self)
{
    cCommSystem_update(cCommSystem_mCommSystem);
    func_0025AC50(LobbyMan_mLobbyMan);
    if (sMenuStack17D688_top(&(*(sMenuStack17D688*)&self->mMenuSys)) != 0)
        cMenuSys_input(&(*(sMenuStack17D688*)&self->mMenuSys));
    func_0017DB18((cDebugLobbyScreen*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017D6F0);
#ifdef SKIP_ASM
extern char str_A_West[];
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cMenuSys_pushMenu(void* menu, void* items);
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void func_0025C610(void* net);
extern "C" void func_0017DA30(cDebugLobbyScreen* self, int a1);
extern "C" void func_0017DE70(cDebugLobbyScreen* self);
extern "C" void PersonaMenu_setpersonas(void* items, void* net);
extern "C" int LobbyMan_getnumrooms(void* net);
extern "C" char* func_0025F150(void* net, int i);
extern "C" int func_0025F430(void* net);
extern "C" int strcmp(char* a, char* b);
extern "C" void func_0025D800(void* a, int b, int c);
void func_0025CD28(void* a, int b);
void func_0025FC28(void* a, int b);



static inline void ClearMenu0017D6F0(int* list)
{
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
}

static inline void SetMenu0017D6F0(int* menu, void* items)
{
    ClearMenu0017D6F0(menu);
    cMenuSys_pushMenu(menu, items);
}

/**
 * Handles one lobby event for the debug lobby screen: moves its state (+0x4) on and opens the menu for the next step.
 *
 * The event type is at +0x0 of @p ev and its argument at +0x8. SSXApp->unk_74 set (automatic lobby login) skips the
 * menus. By type:
 * - 277 (disconnected): closes all menus (cMenuSys_popMenu until the stack is empty); automatic mode restarts at state 0,
 *   otherwise state 19 with the Continue menu (+0x604).
 * - 258 (error): func_0017DA30 with the error code.
 * - 201 (connected to the lobby server): automatic mode copies SSXApp->UserName and Password into LobbyMan's mUserName
 *   and mPassword, logs in (func_0025C610) and sets state 5; otherwise state 3 and the Login Method menu (+0x70).
 * - 203 (account created): state 5 and a login with the typed user name and password (+0x1C04, +0x1C08).
 * - 212 (logged in): automatic mode selects persona 0 (func_0025CD28, state 7); otherwise fills Select Persona (+0x74C)
 *   from the lobby (PersonaMenu_setpersonas), opens it and sets state 6.
 * - 218 (persona selected): automatic mode joins every room named "A.West" (LobbyMan_getnumrooms, func_0025F150,
 *   strcmp, then func_0025D800, state 11); otherwise state 10 with Select Room (+0xD2C).
 * - 214, 216 (persona created or deleted): refills and opens Select Persona, state 6.
 * - 220 (room joined): Room Occupants (+0x1044) and state 12 when func_0025F430 reports a current room, else Select
 *   Room and state 10.
 * - 237, 240, 241 (challenge declined, ignored, ...): state 17, closes all menus and opens Continue.
 * - 239 (game ready): func_0017DE70 sets up the race and opens the game channel; state 16.
 * - 236 (challenge received): in state 12 it closes all menus, sets state 15, copies the challenger's name (the event's
 *   cBXString at +0x8) into +0x1C0C, shows it as the third line of the challenge menu (+0x1A44) and opens that menu
 *   (+0x18C0); in any other state it declines at once (func_0025FC28 with the name).
 *
 * Likely bug: in automatic mode a lobby without an "A.West" room leaves the screen in state 7 ("Selecting Persona") for
 * good, and the room search does not stop at the first match.
 *
 * @param vself  the debug lobby screen
 * @param ev  the lobby event
 */
extern "C" void func_0017D6F0(cDebugLobbyScreen* vself, online_Event* ev)
{
    cDebugLobbyScreen* self = vself;
    switch (*(int*)ev) {
    case 277: {
        int* list = ((int*)&self->mMenuSys);
        while (*list != 0) {
            cMenuSys_popMenu(list);
        }
        if (SSXApp->AutoLobby != 0) {
            self->mState = 0;
        } else {
            self->mState = 19;
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mContinueMenu));
        }
        break;
    }
    case 258:
        func_0017DA30((cDebugLobbyScreen*)self, *(int*)((char*)ev + 8));
        break;
    case 201:
        if (SSXApp->AutoLobby != 0) {
            cBXString_operatorE1(&LobbyMan_mLobbyMan->mUserName, SSXApp->UserName);
            cBXString_operatorE1(&LobbyMan_mLobbyMan->mPassword, SSXApp->Password);
            func_0025C610(LobbyMan_mLobbyMan);
            self->mState = 5;
        } else {
            self->mState = 3;
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mLoginMenu));
        }
        break;
    case 203:
        self->mState = 5;
        cBXString_operatorE1(&LobbyMan_mLobbyMan->mUserName, self->mUserName);
        cBXString_operatorE1(&LobbyMan_mLobbyMan->mPassword, self->mPassword);
        func_0025C610(LobbyMan_mLobbyMan);
        break;
    case 212:
        if (SSXApp->AutoLobby != 0) {
            self->mState = 7;
            func_0025CD28(LobbyMan_mLobbyMan, 0);
        } else {
            PersonaMenu_setpersonas(((char*)&self->mPersonaMenu), LobbyMan_mLobbyMan);
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mPersonaMenu));
            self->mState = 6;
        }
        break;
    case 218:
        if (SSXApp->AutoLobby != 0) {
            int i;
            for (i = 0; i < LobbyMan_getnumrooms(LobbyMan_mLobbyMan); i++) {
                if (strcmp(func_0025F150(LobbyMan_mLobbyMan, i), str_A_West) == 0) {
                    func_0025D800(LobbyMan_mLobbyMan, i, 0);
                    self->mState = 11;
                }
            }
        } else {
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mRoomMenu));
            self->mState = 10;
        }
        break;
    case 214:
    case 216:
        PersonaMenu_setpersonas(((char*)&self->mPersonaMenu), LobbyMan_mLobbyMan);
        cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mPersonaMenu));
        self->mState = 6;
        break;
    case 220:
        if (func_0025F430(LobbyMan_mLobbyMan) != 0) {
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mOccupantsMenu));
            self->mState = 0xC;
        } else {
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mRoomMenu));
            self->mState = 10;
        }
        break;
    case 237:
    case 0xF0:
    case 241: {
        self->mState = 17;
        int* list = ((int*)&self->mMenuSys);
        while (*list != 0) {
            cMenuSys_popMenu(list);
        }
        cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mContinueMenu));
        break;
    }
    case 239:
        func_0017DE70((cDebugLobbyScreen*)self);
        self->mState = 0x10;
        break;
    case 236:
        if (self->mState == 0xC) {
            int* list = ((int*)&self->mMenuSys);
            while (*list != 0) {
                cMenuSys_popMenu(list);
            }
            self->mState = 0xF;
            cBXString_operatorE(&(*(int*)&self->mChallenger), (char*)ev + 8);
            (*(int*)&self->mChallengedMenu.unk_170.Caption) = ((int)self->mChallenger);
            cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mChallengedMenu));
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
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void cMenuSys_pushMenu(void* menu, void* items);
extern "C" void func_0025C610(void* net);
static inline void SetMode0017DA30(void* vself, int mode)
{
    cDebugLobbyScreen* self = (cDebugLobbyScreen*)vself;
    self->mState = mode;
    cMenuSys_pushMenu(&self->mMenuSys, &self->mContinueMenu);
}
/**
 * Shows the error screen that fits the debug lobby screen's current step after a lobby error event.
 *
 * Stores @p errorCode at +0x1C14 (cDebugLobbyScreen_draw prints its text through func_00260E50) and, by the state at +0x4, moves to the
 * matching error state and opens the Continue menu (+0x604): 2 (connecting) goes to 18, 4 (creating account) to 20, 7,
 * 8 and 9 (persona steps) to 22, 11 (joining a room) to 23 and 13 (waiting on a challenge) to 24. In state 5
 * (authenticating) it goes to 21 unless SSXApp->unk_74 (automatic login) is set; then it copies SSXApp->UserName and
 * Password into LobbyMan again and resends the login (func_0025C610), staying in state 5. Other states only keep the
 * code. func_0017D6F0 calls it for event 258.
 *
 * Likely bug: with automatic login a rejected login is retried at once with the same credentials, with no limit and no
 * error shown.
 *
 * @param errorCode  the lobby error code (a four-character tag)
 */
extern "C" void func_0017DA30(cDebugLobbyScreen* self, int errorCode)
{
    self->mErrorCode = errorCode;
    switch (self->mState) {
    case 2:
        SetMode0017DA30(self, 18);
        break;
    case 4:
        SetMode0017DA30(self, 20);
        break;
    case 5:
        if (SSXApp->AutoLobby != 0) {
            cBXString_operatorE1(&LobbyMan_mLobbyMan->mUserName, SSXApp->UserName);
            cBXString_operatorE1(&LobbyMan_mLobbyMan->mPassword, SSXApp->Password);
            func_0025C610(LobbyMan_mLobbyMan);
        } else {
            SetMode0017DA30(self, 21);
        }
        break;
    case 7:
        SetMode0017DA30(self, 22);
        break;
    case 8:
        SetMode0017DA30(self, 22);
        break;
    case 9:
        SetMode0017DA30(self, 22);
        break;
    case 11:
        SetMode0017DA30(self, 23);
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
extern void* VoipMan;
extern char str_cPreGameLoadScreen[];
struct cAppMan;
extern "C" void cMenuSys_pushMenu(void* menu, void* items);
extern "C" int cMenuSys_isActive(void* menu);
extern "C" void cCommSystem_netConnConnect(void* comm, int a1);
extern "C" void func_0025B6D8(void* net, int a1);
extern "C" void PersonaMenu_setpersonas(void* items, void* net);
extern "C" void LobbyMenu_updaterooms(void* items, void* net);
extern "C" void RoomMenu_updateusers(void* items, void* net);
extern "C" void func_0025D800(void* a, int b, int c);
extern "C" int cCommSystem_isChannelConnected(void* comm, int ch);
extern "C" void cGameComm_construct(int ch, int host);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* mem);
void cAppMan_setNextModule(cAppMan* app, unsigned int module);
extern "C" void cCommSystem_netConnLocalAddr(void* comm, int* info);
extern "C" void func_00266D58(void* p, int ip);
extern "C" void func_00266E30(void* p);


/**
 * Advances the debug lobby screen's state machine, re-running it until the state (+0x4) stops changing.
 *
 * - 0: starts the network connection with config 0 (cCommSystem_netConnConnect) when the comm system is idle (mNetConnState 1), then
 *   state 1.
 * - 1: once mNetConnState is neither 3 nor 4, starts connecting to the lobby server (func_0025B6D8) and sets state 2.
 * - 3, 6, 10, 15: reopens Login Method (+0x70), Select Persona (+0x74C), Select Room (+0xD2C) or the challenge menu
 *   (+0x18C0) when no menu is open (cMenuSys_pushMenu); state 10 also refreshes the room list (LobbyMenu_updaterooms) every pass.
 * - 12: with no menu open it joins the empty-named room (func_0025D800 with index -1) and sets state 11; otherwise it
 *   refreshes the occupant list (RoomMenu_updateusers).
 * - 13: opens the Abort menu (+0x1AAC) when the menu system is inactive (cMenuSys_isActive).
 * - 14: with no menu open, state 12 and Room Occupants (+0x1044).
 * - After the Continue menu closes: 17 and 24 return to Room Occupants (state 12), 20 and 21 to Login Method (state 3),
 *   22 to a refilled Select Persona (state 6), 23 to Select Room (state 10), 18 and 19 to state 0.
 * - 16: once the game channel (+0x1C10) reports connected (cCommSystem_isChannelConnected) it creates the game comm (cGameComm_construct
 *   with LobbyMan's mIsHost), queues a new cPreGameLoadScreen as the next module and connects voice chat
 *   (func_00266D58, func_00266E30 on VoipMan).
 *
 * Likely bugs: in state 1 the loop runs again at once after state 0, when mNetConnState is still 2 (connect requested),
 * so the lobby connect starts before the network is up (a failed connection, 6, does not stop it either); voice chat in
 * state 16 gets this console's own address (cCommSystem_netConnLocalAddr), not the opponent's.
 *
 * @param vself  the debug lobby screen
 */
extern "C" void func_0017DB18(cDebugLobbyScreen* vself)
{
    cDebugLobbyScreen* self = vself;
    unsigned int state;
    do {
        state = ((unsigned int)self->mState);
        switch (state) {
        case 0: {
            cCommSystem* comm = cCommSystem_mCommSystem;
            bool busy = comm->mNetConnState != 1;
            if (!busy) {
                cCommSystem_netConnConnect((char*)comm, 0);
            }
            self->mState = 1;
            break;
        }
        case 1:
            if (cCommSystem_mCommSystem->mNetConnState != 3 && cCommSystem_mCommSystem->mNetConnState != 4) {
                func_0025B6D8(LobbyMan_mLobbyMan, 0);
                self->mState = 2;
            }
            break;
        case 13:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mAbortMenu));
            }
            break;
        case 17:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                self->mState = 0xC;
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mOccupantsMenu));
            }
            break;
        case 24:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mOccupantsMenu));
                self->mState = 0xC;
            }
            break;
        case 21:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                self->mState = 3;
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mLoginMenu));
            }
            break;
        case 20:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                self->mState = 3;
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mLoginMenu));
            }
            break;
        case 18:
        case 19:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                self->mState = 0;
            }
            break;
        case 22:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                PersonaMenu_setpersonas(((char*)&self->mPersonaMenu), LobbyMan_mLobbyMan);
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mPersonaMenu));
                self->mState = 6;
            }
            break;
        case 23:
            if (cMenuSys_isActive(((int*)&self->mMenuSys)) == 0) {
                cMenuSys_pushMenu(((int*)&self->mMenuSys), ((char*)&self->mRoomMenu));
                self->mState = 10;
            }
            break;
        case 15: {
            int* menu = ((int*)&self->mMenuSys);
            if (*menu == 0) {
                cMenuSys_pushMenu(menu, ((char*)&self->mChallengedMenu));
            }
            break;
        }
        case 6: {
            int* menu = ((int*)&self->mMenuSys);
            if (*menu == 0) {
                cMenuSys_pushMenu(menu, ((char*)&self->mPersonaMenu));
            }
            break;
        }
        case 10: {
            int* menu = ((int*)&self->mMenuSys);
            char* items = ((char*)&self->mRoomMenu);
            if (*menu == 0) {
                cMenuSys_pushMenu(menu, items);
            }
            LobbyMenu_updaterooms(items, LobbyMan_mLobbyMan);
            break;
        }
        case 12:
            if (self->mMenuSys.MenuDepth == 0) {
                func_0025D800(LobbyMan_mLobbyMan, -1, 0);
                self->mState = 11;
            } else {
                RoomMenu_updateusers(((char*)&self->mOccupantsMenu), LobbyMan_mLobbyMan);
            }
            break;
        case 3: {
            int* menu = ((int*)&self->mMenuSys);
            if (*menu == 0) {
                cMenuSys_pushMenu(menu, ((char*)&self->mLoginMenu));
            }
            break;
        }
        case 14: {
            int* menu = ((int*)&self->mMenuSys);
            if (*menu == 0) {
                self->mState = 0xC;
                cMenuSys_pushMenu(menu, ((char*)&self->mOccupantsMenu));
            }
            break;
        }
        case 16:
            if (cCommSystem_isChannelConnected(cCommSystem_mCommSystem, self->mGameChannel) != 0) {
                int info[4];
                cGameComm_construct(self->mGameChannel, LobbyMan_mLobbyMan->mIsHost);
                cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
                cCommSystem_netConnLocalAddr(cCommSystem_mCommSystem, info);
                func_00266D58(VoipMan, info[0]);
                func_00266E30(VoipMan);
            }
            break;
        }
    } while (((unsigned int)self->mState) != state);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017DE70);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBENewRaceInterface_setGamePath(void* iface, int idx);
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);
extern "C" int cBENewRaceInterface_setNumberAI(void* iface, int n);
extern "C" void cBENewPlayerInterface_setRiderCtrlID(void* iface, int rider, int ctrl);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* iface, int rider, int id);
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(void* iface, int a1, int a2);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int cCommSystem_openDSockChannel(void* comm, const char* addr, int port, int host);
extern int cBENewRaceInterface_bIsBamLocation;
extern char str_pct_d_pct_d_pct_d_pct_d[];

struct sNetCfg_17DE70 {
    int active;
    int f4;
    int host;
    unsigned short port;
};
extern sNetCfg_17DE70 cBENetworkInterface_networkOpt_cfg __asm__("cBENetworkInterface_networkOpt");

struct sVEnt_17DE70 { short delta; short index; void (*fn)(void*); };

static inline void vcall1_17DE70(void* obj)
{
    sVEnt_17DE70* vt = *(sVEnt_17DE70**)((char*)obj + 0xC);
    vt[1].fn(vthis(obj, vt[1]));
}

/**
 * Sets up the debug lobby's two-player online race and opens the game channel to the opponent.
 *
 * The new-race interface (cBE_getInterface__Fv kind 0) gets game path 1, game mode 4 and no AI riders, sets
 * cBENewRaceInterface_newRace.iLocationIdx (newRace.iLocationIdx) to 0 and cBENewRaceInterface_bIsBamLocation to 1, then applies them through the
 * interface's virtual slot 1. The player interface (kind 1) gets controller -1 and character 0 for riders 2..5,
 * controller 0 for player 0 and -1 for player 1, and is applied the same way. cBENetworkInterface_networkOpt gets
 * bIsNetworkGame 1 and bIsHost from LobbyMan's mIsHost (network interface, kind 7, applied). The address to dial is
 * mOppoAddr when hosting and mHostAddr otherwise; it is formatted as dotted decimal and passed to
 * cCommSystem_openDSockChannel with networkOpt.iPort and bIsHost. Stores the channel at +0x1C10 and sets state 16,
 * where func_0017DB18 waits for the connection. func_0017D6F0 calls it for event 239 (game ready).
 */
extern "C" void func_0017DE70(cDebugLobbyScreen* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBENewRaceInterface_setGamePath(race, 1);
    cBENewRaceInterface_setGameMode(race, 4);
    cBENewRaceInterface_setNumberAI(race, 0);
    cBENewRaceInterface_newRace.iLocationIdx = 0;
    cBENewRaceInterface_bIsBamLocation = 1;
    vcall1_17DE70(race);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    for (int i = 2; i < 6; i++) {
        cBENewPlayerInterface_setRiderCtrlID(player, i, -1);
        cBENewPlayerInterface_setRiderCharID(player, i, 0);
    }
    cBENewPlayerInterface_setPlayerCtrlID(player, 0, 0);
    cBENewPlayerInterface_setPlayerCtrlID(player, 1, -1);
    vcall1_17DE70(player);
    void* net = cBE_getInterface_Fv(SSXApp->BE, 7);
    sNetCfg_17DE70* cfg = &cBENetworkInterface_networkOpt_cfg;
    cBENetworkInterface_networkOpt_cfg.active = 1;
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
    self->mGameChannel = cCommSystem_openDSockChannel(cCommSystem_mCommSystem, buf, cBENetworkInterface_networkOpt_cfg.port, cBENetworkInterface_networkOpt_cfg.host);
    self->mState = 0x10;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E030);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void func_0025B9C8(void* mgr, void* req);
extern char str_pleaseat_get_lost[];


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

/**
 * Creates a lobby account from the debug "Create Account" menu: keeps the typed name and password and sends the
 * request.
 *
 * Closes all menus (cMenuSys_popMenu until the stack is empty), copies @p userName and @p password into the screen's strings at +0x1C04
 * and +0x1C08 and sets state 4 ("Creating account"). Fills LobbyMan's mNewAccount form: mName @p userName, mPassword and
 * unk_08 @p password, mMail "please@get.lost", born 1950-01-01 (mBornYear, mBornMonth, mBornDay), mGender 1 (sent as 'F'),
 * both mSpam flags 0, and sends it with func_0025B9C8. On event 203 func_0017D6F0 then logs in with the same name and
 * password. Called by the Create Account menu's command handler AccountDetailsMenu_onCommand.
 *
 * @param userName  account name
 * @param password  password
 */
extern "C" void func_0017E030(cDebugLobbyScreen* self, const char* userName, const char* password)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    cBXString_operatorE1(&self->mUserName, userName);
    cBXString_operatorE1(&self->mPassword, password);
    self->mState = 0x4;
    LobbyMan* lobbyMan = LobbyMan_mLobbyMan;
    cBXString_operatorE1(&lobbyMan->mNewAccount.mName, userName);
    cBXString_operatorE1(&lobbyMan->mNewAccount.mPassword, password);
    cBXString_operatorE1(&lobbyMan->mNewAccount.unk_08, password);
    cBXString_operatorE1(&lobbyMan->mNewAccount.mMail, str_pleaseat_get_lost);
    lobbyMan->mNewAccount.mBornYear = 1950;
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
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" void func_0025C610(void* mgr);


/**
 * Logs in from the debug "Login" menu: keeps the typed name and password and sends the lobby login.
 *
 * Closes all menus, copies @p userName and @p password into the screen's strings at +0x1C04 and +0x1C08, sets state 5
 * ("Authenticating account"), copies both into LobbyMan's mUserName and mPassword and sends the login (func_0025C610).
 * Called by the Login menu's command handler AccountDetailsMenu_onCommand1.
 *
 * @param userName  account name
 * @param password  password
 */
extern "C" void func_0017E118(cDebugLobbyScreen* self, const char* userName, const char* password)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    cBXString_operatorE1(&self->mUserName, userName);
    cBXString_operatorE1(&self->mPassword, password);
    self->mState = 0x5;
    cBXString_operatorE1(&LobbyMan_mLobbyMan->mUserName, self->mUserName);
    cBXString_operatorE1(&LobbyMan_mLobbyMan->mPassword, self->mPassword);
    func_0025C610(LobbyMan_mLobbyMan);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E1C0);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void func_0025D1B8(void* a, int b);

/**
 * Creates a persona from the debug "Create Persona" menu.
 *
 * Closes all menus, sets state 8 ("Creating Persona") and sends the create-persona request (func_0025D1B8). The menu's
 * command handler CreatePersonaMenu_onCommand passes the typed name buffer, although the parameter is declared int.
 *
 * @param persona  the new persona's name (a char pointer)
 */
extern "C" void func_0017E1C0(cDebugLobbyScreen* self, int persona)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 0x8;
    func_0025D1B8(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E228);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void func_0025D428(void* a, int b);

/**
 * Deletes a persona chosen in the debug "Delete Persona" menu.
 *
 * Closes all menus, sets state 9 ("Deleting Persona") and sends the delete-persona request for that index
 * (func_0025D428). Called by the menu's command handler DeletePersonaMenu_onCommand.
 *
 * @param persona  index of the persona in LobbyMan's list
 */
extern "C" void func_0017E228(cDebugLobbyScreen* self, int persona)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 0x9;
    func_0025D428(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E290);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
void func_0025CD28(void* a, int b);

/**
 * Selects the persona chosen in the debug "Select Persona" menu.
 *
 * Closes all menus, sets state 7 ("Selecting Persona") and selects the persona with func_0025CD28. Called by the menu's
 * command handler PersonaMenu_onCommand.
 *
 * @param persona  index of the persona in LobbyMan's list
 */
extern "C" void func_0017E290(cDebugLobbyScreen* self, int persona)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 0x7;
    func_0025CD28(LobbyMan_mLobbyMan, persona);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E2F8);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void func_0025D800(void* a, int b, int c);

/**
 * Joins the room chosen in the debug "Select Room" menu.
 *
 * Closes all menus, sets state 11 ("Selecting Room") and joins the room without a password (func_0025D800). Called by
 * the menu's command handler LobbyMenu_onCommand.
 *
 * @param room  index in the lobby's room list
 */
extern "C" void func_0017E2F8(cDebugLobbyScreen* self, int room)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 11;
    func_0025D800(LobbyMan_mLobbyMan, room, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E368);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);

/**
 * Handles picking a user in the debug "Room Occupants" menu: closes all menus and sets state 13 ("Waiting for
 * reponse").
 *
 * Likely bug: the menu's command handler RoomMenu_onCommand passes the chosen occupant's index as a second argument, but
 * this function ignores it and sends no challenge, so the screen waits for a reply that is never requested (the Abort
 * menu, func_0017E558, gets it out).
 */
extern "C" void func_0017E368(cDebugLobbyScreen* self)
{
    int* list = (int*)(&self->mMenuSys);
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 13;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E3B8);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void cMenuSys_pushMenu(void* list, void* item);

/**
 * Opens the debug chat menu from "Room Occupants": closes all menus, sets state 14 and pushes the chat menu (+0x1534).
 */
extern "C" void func_0017E3B8(cDebugLobbyScreen* self)
{
    int* list = (int*)(&self->mMenuSys);
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 0xE;
    cMenuSys_pushMenu(&self->mMenuSys, &self->mChatMenu);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E418);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void cMenuSys_pushMenu(void* list, void* item);
extern "C" void func_0025AAE0(void* a, int b);

/**
 * Sends a chat line chosen in the debug chat menu and returns to "Room Occupants".
 *
 * Closes all menus, pushes Room Occupants (+0x1044), sets state 12 and sends @p messageText as a lobby message with
 * func_0025AAE0 ('mesg'). The chat menu's command handler ChatMenu_onCommand passes the item's caption.
 *
 * @param messageText  the line to send (a char pointer, declared int)
 */
extern "C" void func_0017E418(cDebugLobbyScreen* self, int messageText)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    cMenuSys_pushMenu(&self->mMenuSys, &self->mOccupantsMenu);
    self->mState = 0xC;
    func_0025AAE0(LobbyMan_mLobbyMan, messageText);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E490);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void func_0025FB58(void* a, int b, int c);

/**
 * Accepts a challenge from the debug "You've been Challenged!" menu.
 *
 * Closes all menus, sets state 13 ("Waiting for reponse") and locks the challenge with the challenger named in +0x1C0C
 * (func_0025FB58, match type 0). func_0017DB18 then shows the Abort menu. Called by the challenge menu's command
 * handler ChallengeResponseMenu_onCommand.
 */
extern "C" void func_0017E490(cDebugLobbyScreen* self)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    self->mState = 13;
    func_0025FB58(LobbyMan_mLobbyMan, (int)self->mChallenger, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E4F0);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void cMenuSys_pushMenu(void* list, void* item);
void func_0025FC28(void* a, int b);

/**
 * Declines a challenge from the debug "You've been Challenged!" menu and returns to "Room Occupants".
 *
 * Closes all menus, pushes Room Occupants (+0x1044), sets state 12 and sends "decline" to the challenger named in
 * +0x1C0C (func_0025FC28). Called by the challenge menu's command handler ChallengeResponseMenu_onCommand.
 */
extern "C" void func_0017E4F0(cDebugLobbyScreen* self)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    cMenuSys_pushMenu(&self->mMenuSys, &self->mOccupantsMenu);
    self->mState = 0xC;
    func_0025FC28(LobbyMan_mLobbyMan, (int)self->mChallenger);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_0017E558);
#ifdef SKIP_ASM
extern "C" void cMenuSys_popMenu(void* list);
extern "C" void cMenuSys_pushMenu(void* list, void* item);
extern "C" void func_0025F628(void* a, int b, int c);

/**
 * Aborts the pending challenge from the debug "Abort" menu and returns to "Room Occupants".
 *
 * Closes all menus, pushes Room Occupants (+0x1044), sets state 12 and cancels the challenge with func_0025F628(..., 1,
 * 0): a type-238 event is posted and no "abortChal" is sent to the opponent. Called by the Abort menu's command handler
 * AbortMenu_onCommand.
 */
extern "C" void func_0017E558(cDebugLobbyScreen* self)
{
    int* list = &self->mMenuSys.MenuDepth;
    while (*list != 0) {
        cMenuSys_popMenu(list);
    }
    cMenuSys_pushMenu(&self->mMenuSys, &self->mOccupantsMenu);
    self->mState = 0xC;
    func_0025F628(LobbyMan_mLobbyMan, 1, 0);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugLobbyScreen_draw);
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
extern "C" void cFont_centertext(void* self, float x, float y, const char* str);
extern "C" void cFont_righttext(void* self, const char* str, float x, float y);
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" void cFont_righttextf(void* self, float x, float y, const char* fmt, ...);
extern "C" void cFont_centertextf(void* self, float x, float y, const char* fmt, ...);
extern "C" void cMenuSys_render(void* self);
extern "C" void cCommSystem_netConnLocalAddr(void* comm, int* out);
extern "C" void cBXString__cBXString(void* self, int flags);

struct sColor17E5C8 {
    float r, g, b, a;
    sColor17E5C8(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
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

static inline void scale17E5C8(cFont* f, float s)
{
    f->Scale[0] = f->BaseScale[0] * s;
    f->Scale[1] = f->BaseScale[1] * s;
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
    cFont_righttext(font, on ? D_004A12F8 : D_004A1330, x, y);
}

static inline int isEq17E5C8(int a, int b) { return a == b; }

/**
 * Draws the debug lobby screen: a status line for the current state, the error text or game details, the menus and this
 * console's address.
 *
 * Uses the debug front end's font (SSXApp->DebugFE +0x18), setting its Scale to BaseScale times a factor and its Color
 * (tARGB) before each part. By the state at +0x4 it centres a red (0.9, 0.2, 0.2) line at scale 1.5 (cFont_centertext):
 * "Connecting to network" (0, 1), "Connecting to lobby" (2), "Creating account" (4), "Authenticating account" (5),
 * "Selecting Persona" (7), "Creating Persona" (8), "Deleting Persona" (9) and "Selecting Room" (11) at (320, 200);
 * "Waiting for reponse" (13) and "Challenge Declined!" (17) at (320, 100). The error states 18 "Error Connecting!", 19
 * "Disconnected from server!", 20 "Error Creating Account!", 21 "Error Authenticating Account!", 22 "Persona Error!",
 * 23 "Error Entering room!" and 24 "Error Issueing Challenge!" add a light grey line at scale 0.8 and y 150: the text
 * func_00260E50 builds for the error code at +0x1C14. State 16 shows "Play Game!" and, at scale 0.7 in pale yellow,
 * rows 20 apart from y 200 with labels at x 110 and values right-aligned at x 530: Game (LobbyMan mGameName), Opponent
 * (mOpponent), Host/Client ("Host" or "Client" by mIsHost), Host Address (mHostAddr) and Client Address (mOppoAddr) in
 * dotted form, and Seed (mSeed). Then, at scale 0.9, the menu system renders its top menu (cMenuSys_render) when one is
 * open, and while the comm system is connected (mNetConnState 5) this console's address (cCommSystem_netConnLocalAddr) is centred in
 * yellow at (320, 420), scale 0.8. cDebugFE_render calls it through the active screen's virtual slot 3.
 */
extern "C" void cDebugLobbyScreen_draw(sDebugFE17E5C8* self)
{
    cFont* font = SSXApp->DebugFE->Font;
    switch (self->state) {
    case 0:
    case 1:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Connecting_to_network);
        break;
    case 2:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Connecting_to_lobby);
        break;
    case 4:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Creating_account);
        break;
    case 5:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Authenticating_account);
        break;
    case 8:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Creating_Persona);
        break;
    case 9:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Deleting_Persona);
        break;
    case 7:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Selecting_Persona);
        break;
    case 11:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 200.0f, str_Selecting_Room);
        break;
    case 13:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Waiting_for_reponse);
        break;
    case 17:
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Challenge_Declined);
        break;
    case 18: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Error_Connecting);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 19: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Disconnected_from_server);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 20: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Error_Creating_Account);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 22: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Persona_Error);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 21: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Error_Authenticating_Account);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 23: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Error_Entering_room);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 24: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Error_Issueing_Challenge);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.9f);
        cFont_centertext(font, 320.0f, 150.0f, getStr17E5C8(LobbyMan_mLobbyMan, self->tag).p);
        break;
    }
    case 16: {
        scale17E5C8(font, 1.5f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.2f, 0.2f);
        cFont_centertext(font, 320.0f, 100.0f, str_Play_Game);
        scale17E5C8(font, 0.7f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.6f);
        float y = 200.0f;
        cFont_text(font, 110.0f, y, str_Game);
        cFont_righttext(font, LobbyMan_mLobbyMan->mGameName, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Opponent);
        cFont_righttext(font, LobbyMan_mLobbyMan->mOpponent, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Host_Client);
        onOff17E5C8(font, LobbyMan_mLobbyMan->mIsHost, 530.0f, y);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Host_Address);
        unsigned int ip = (unsigned int)LobbyMan_mLobbyMan->mHostAddr;
        cFont_righttextf(font, 530.0f, y, str_pct_d_pct_d_pct_d_pct_d, ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Client_Address);
        unsigned int ip2 = (unsigned int)LobbyMan_mLobbyMan->mOppoAddr;
        cFont_righttextf(font, 530.0f, y, str_pct_d_pct_d_pct_d_pct_d, ip2 >> 24, (ip2 >> 16) & 0xFF, (ip2 >> 8) & 0xFF, ip2 & 0xFF);
        y += 20.0f;
        cFont_text(font, 110.0f, y, str_Seed);
        cFont_righttextf(font, 530.0f, y, str_pct_d_b, LobbyMan_mLobbyMan->mSeed);
        break;
    }
    }
    scale17E5C8(font, 0.9f);
    if (top17E5C8(self) != 0)
        cMenuSys_render(&self->count);
    if (isEq17E5C8(cCommSystem_mCommSystem->mNetConnState, 5)) {
        unsigned int ip;
        cCommSystem_netConnLocalAddr(cCommSystem_mCommSystem, (int*)&ip);
        scale17E5C8(font, 0.8f);
        (*(sColor17E5C8*)&font->Color[0]) = sColor17E5C8(1.0f, 0.9f, 0.9f, 0.2f);
        unsigned int localAddr = ip;
        cFont_centertextf(font, 320.0f, 420.0f, str_pct_d_pct_d_pct_d_pct_d, localAddr >> 24, (localAddr >> 16) & 0xFF, (localAddr >> 8) & 0xFF, localAddr & 0xFF);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cHeartBeat_update);
#ifdef SKIP_ASM
extern "C" float fSin(float x);

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

/**
 * Advances the debug front end's heartbeat doodle: a point wandering on sine curves, leaving a trail.
 *
 * Runs two steps: each adds (sin(phase) + 1.9) * 0.032 to the position, advances the phases by (0.005, 0.009),
 * wraps phases and position with the unit's wrap helper, and stores (sin(pos) * 20 + (550, 420)) as the next of
 * 64 trail points (head index mod 64).
 *
 * @param heartBeat  the heartbeat (this)
 */
extern "C" void cHeartBeat_update(cHeartBeat* heartBeat)
{
    cHeartBeat* self = heartBeat;
    for (int i = 0; i < 2; i++) {
        sVec2_17F2D0 posStep(fSin((*(sVec2_17F2D0*)&self->Phase[0]).x), fSin((*(sVec2_17F2D0*)&self->Phase[0]).y));
        posStep += 1.899999976158142f;
        posStep *= 0.03200000151991844f;
        {
            sVec2_17F2D0 phaseStep;
            phaseStep.x = 0.004999999888241291f;
            phaseStep.y = 0.008999999612569809f;
            (*(sVec2_17F2D0*)&self->Phase[0]).x += phaseStep.x;
            (*(sVec2_17F2D0*)&self->Phase[0]).y += phaseStep.y;
        }
        (*(sVec2_17F2D0*)&self->Angle[0]) += posStep;
        (*(sVec2_17F2D0*)&self->Phase[0]).x = wrap_17F2D0((*(sVec2_17F2D0*)&self->Phase[0]).x);
        (*(sVec2_17F2D0*)&self->Phase[0]).y = wrap_17F2D0((*(sVec2_17F2D0*)&self->Phase[0]).y);
        (*(sVec2_17F2D0*)&self->Angle[0]).x = wrap_17F2D0((*(sVec2_17F2D0*)&self->Angle[0]).x);
        (*(sVec2_17F2D0*)&self->Angle[0]).y = wrap_17F2D0((*(sVec2_17F2D0*)&self->Angle[0]).y);
        self->HistoryIdx = (self->HistoryIdx + 1) % 64;
        sVec2_17F2D0 pt(fSin((*(sVec2_17F2D0*)&self->Angle[0]).x) * 20.0f + 550.0f,
                        fSin((*(sVec2_17F2D0*)&self->Angle[0]).y) * 20.0f + 420.0f);
        ((sVec2_17F2D0*)&self->History[0][0])[self->HistoryIdx] = pt;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cHeartBeat_draw);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* G2D;
extern "C" void c2DRender_drawRect4(void* self, float* p0, float* p1, float* col, float z);



static inline t2Vector operator*(const t2Vector& a, float s) { return t2Vector(a.x * s, a.y * s); }
static inline t2Vector operator-(const sVec2_17F2D0& a, const t2Vector& b);
static inline t2Vector operator+(const sVec2_17F2D0& a, const t2Vector& b);
static inline t2Vector operator-(const sVec2_17F2D0& a, const t2Vector& b) { return t2Vector(a.x - b.x, a.y - b.y); }
static inline t2Vector operator+(const sVec2_17F2D0& a, const t2Vector& b) { return t2Vector(a.x + b.x, a.y + b.y); }

struct sCol_17F500 {
    float a, r, g, b;
    sCol_17F500(float aa, const float& ar, float ag, float ab) : a(aa), r(ar), g(ag), b(ab) {}
};

// The quad routine takes its corners and colour by reference (pointers on the EE).
void c2DRender_drawRect4_r(void* self, const t2Vector& p0, const t2Vector& p1, const sCol_17F500& c, float z) __asm__("c2DRender_drawRect4");

static inline void set1_17F500(char* m, unsigned int mask, int shift, unsigned int v)
{
    sDrawState* top = *(sDrawState**)(m + 0xE84);
    top->m_bitGroups[1] = (top->m_bitGroups[1] & ~mask) | ((v << shift) & mask);
}


/**
 * Draws the debug front end's heartbeat doodle: its trail of 64 points as small quads, then a larger quad on the newest
 * point.
 *
 * Sets the same draw-state bits on GrMan's current draw state (+0xE84) as cDebugFE_render does and makes @p tex its
 * texture 0. For i = 1..63 it takes trail point (head + i) mod 64 (the History array at +0x10, head index at +0x210),
 * so the oldest point comes first, and with f = i/64 draws a 6x6 quad centred on it through the 2D renderer G2D
 * (c2DRender_drawRect4, z 0) in colour (0.15 f^2, 0.5, 0.3 + 0.2 f, 0.5 f^2) in the code's a, r, g, b order. The head point
 * gets a 12x12 quad in (0.5, 0.25, 0.25, 0.5). cDebugFE_render calls it on the heartbeat at +0x24 with the particle
 * texture (+0x1C).
 *
 * @param p_  the heartbeat (cHeartBeat_update advances it)
 * @param tex  texture id to draw with
 */
extern "C" void cHeartBeat_draw(cHeartBeat* p_, int tex)
{
    cHeartBeat* self = p_;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    set1_17F500((char*)ps2GraphicsMan, 0x1800000, 23, 2);
    set1_17F500((char*)ps2GraphicsMan, 0x300000, 20, 3);
    set1_17F500((char*)ps2GraphicsMan, 0xFF000, 12, 13);
    set1_17F500((char*)ps2GraphicsMan, 0x7C, 2, 5);
    set1_17F500((char*)ps2GraphicsMan, 0x400000, 22, 1);
    t2Vector size(6.0f, 6.0f);
    (ps2GraphicsMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    for (int i = 1; i < 64; i++) {
        sVec2_17F2D0& pt = ((sVec2_17F2D0*)&self->History[0][0])[(self->HistoryIdx + i) % 64];
        float recency = (float)i * 0.015625f;
        c2DRender_drawRect4_r(G2D, pt - size * 0.5f, pt + size * 0.5f, sCol_17F500(recency * recency * 0.15f, 0.5f, recency * 0.2f + 0.3f, recency * (recency * 0.5f)), 0.0f);
    }
    // Head marker colour; named locals (not literals) so the inlined ctor stores sp-relative (CHEATSHEET).
    float hi = 0.5f;
    float lo = 0.25f;
    c2DRender_drawRect4_r(G2D, ((sVec2_17F2D0*)&self->History[0][0])[self->HistoryIdx] - size, ((sVec2_17F2D0*)&self->History[0][0])[self->HistoryIdx] + size, sCol_17F500(hi, lo, lo, hi), 0.0f);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE_cDebugFE);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_cFadeScreen(void* self);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char cDebugFE_vtbl[];


inline void* operator new[](unsigned int, void* p) { return p; }


/**
 * Constructs the debug front end: the cFadeScreen base, cDebugFE's vtable and a zeroed heartbeat.
 *
 * Runs cFadeScreen_cFadeScreen, stores cDebugFE_vtbl, zeroes the heartbeat at +0x24 (its two vectors at +0x24..+0x30,
 * the 64-point trail at +0x34 through MEM_fill and the head index at +0x234). The screens, font and textures are set
 * later by cDebugFE_load. cSSXApp_startFELoad builds the 0x238-byte DebugFE with it.
 *
 * @return @p self
 */
extern "C" void* cDebugFE_cDebugFE(cDebugFE* self)
{
    cFadeScreen_cFadeScreen(self);
    *(void**)self = cDebugFE_vtbl;
    *(int*)&self->HeartBeat = 0;
    *(int*)&self->HeartBeat.Angle[1] = 0;
    *(int*)&self->HeartBeat.Phase[0] = 0;
    *(int*)&self->HeartBeat.Phase[1] = 0;
    cHeartBeat* list = ((&self->HeartBeat));
    new (((t2Vector*)&list->History[0][0])) t2Vector[64];
    list->HistoryIdx = 0;
    MEM_fill(((t2Vector*)&list->History[0][0]), 0, 0x200);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cDebugFE__cDebugFE);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cAppModule_vtbl[];

/**
 * Destroys the debug front end: stores the base vtable cAppModule_vtbl at +0x0 and, if asked, frees the object.
 *
 * It does not free the screens, font or textures; cDebugFE_purge does that.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDebugFE__cDebugFE(cDebugFE* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
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
extern "C" void* cDebugLobbyScreen_cDebugLobbyScreen(cDebugLobbyScreen* self);

typedef void (*Fn1_17F868)(void*, int);
typedef int (*Load_17F868)(void*, void*, const char*, int, int, int);


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
    ps2GraphicsMan->InitTextures(0);
    self->font = (char*)cFont_cFont(cMemMan_alloc(132, str_DebugFont, 0, 0), str_data_fonts_debugfnt_sfn, 0);
    t2Vector sc(1.600000023841858f, 1.600000023841858f);
    *(t2Vector*)(self->font + 0x30) = sc;
    *(t2Vector*)(self->font + 0x38) = *(t2Vector*)(self->font + 0x30);
    self->snd1C = -1;
    int* data = FILE_loadz(str_data_textures_dot_ssh, 0x100);
    if (data) {
        char* payload = (char*)data + data[5];
        if (payload) {
            cPS2GraphicsMan* theGraphicsMan = cGraphicsMan_TheGraphicsMan;
            self->snd1C = theGraphicsMan->AddTexture((const SHAPE*)payload, str_part, 0, 1, -1);
        }
        cMemMan_free(data);
    }
    data = FILE_load(str_data_fonts_menu_ssh, 0x100);
    cPS2GraphicsMan* graphicsMan = GrMan;
    self->snd20 = graphicsMan->AddTexture((const SHAPE*)((char*)data + data[5]), str_menu, 0, 1, -1);
    if (data) {
        cMemMan_free(data);
    }
    for (int i = 0; i < 2; i++) {
        self->objs[i] = 0;
    }
    self->cur = self->objs[0] = cDebugMenuScreen_cDebugMenuScreen(cMemMan_alloc(35912, str_DebugMenu, 0, 0));
    self->objs[1] = cDebugLobbyScreen_cDebugLobbyScreen((cDebugLobbyScreen*)cMemMan_alloc(7192, str_LobbyMenu, 0, 0));
    if (SSXApp->AutoLobby) {
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
extern "C" void cDebugFE_gainFocus(cDebugFE* self)
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
extern "C" void cFont__cFont(void* p, int flags);


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
 * when >= 0) through GrMan slot 50, and deletes the font (+0x18) with cFont__cFont(font, 3). The local view calls
 * the texture ids snd1C/snd20; they are On Tour's ParticleTID and BoxTID.
 */
extern "C" void cDebugFE_purge(cDebugFE* self)
{
    for (int i = 0; i < 2; i++)
    {
        char* screen = ((char*)self->Screens[i]);
        if (screen != 0)
        {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)screen;
            vt[1].fn(vthis(screen, vt[1]), 3);
        }
    }
    cSSXApp* app = SSXApp;
    app->RealFE = 0;
    app->DebugFE = 0;
    if (self->BoxTID >= 0)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->RemoveTexture(self->BoxTID);
    }
    if (self->ParticleTID >= 0)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->RemoveTexture(self->ParticleTID);
    }
    if (self->Font != 0)
        cFont__cFont(self->Font, 3);
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
extern "C" void cFadeScreen_update(void* self);
extern "C" void cHeartBeat_update(cHeartBeat* p);

/**
 * Runs one frame of the debug front end: the fade, the active screen and the heartbeat.
 *
 * Calls cFadeScreen_update (On Tour cFadeScreen::update); while the app's word +0x4 is 0 it updates the active screen
 * (+0x14, virtual slot 2); then cHeartBeat_update (On Tour cHeartBeat::update) on the heartbeat at +0x24.
 */
extern "C" void cDebugFE_update(cDebugFE* self)
{
    cFadeScreen_update(self);
    if ((int)SSXApp->LoadingModule == 0)
        ((cFEObjK17FB98*)self->ActiveScreen)->v02();
    cHeartBeat_update((cHeartBeat*)(&self->HeartBeat));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/debugfe", cDebugFE_render);
#ifdef SKIP_ASM
extern t4Matrix t4mat_Identity;
extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void cHeartBeat_draw(cHeartBeat* self, int a1);

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
 * Returns 0 at once when GrMan's slot 17 returns 0. Draws the fade overlay with cFadeScreen_drawFade (cFadeScreen's
 * render); unless that reports the screen fully faded it sets a black clear colour (slot 15), the 640x480 viewport
 * (slot 26), the identity matrix (slot 34) and the render-state bits, draws the heartbeat (cHeartBeat_draw on +0x24
 * with the texture at +0x1C) and calls the active screen's render virtual (+0x14, slot 3). Always ends with GrMan
 * slot 20.
 *
 * @return 0 when GrMan's slot 17 returns 0, otherwise 1
 */
extern "C" int cDebugFE_render(cDebugFE* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVEntry<void>* vt = (sVEntry<void>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    if (!((FnI_17FBF0)vt[17].fn)((char*)ps2GraphicsMan + vt[17].delta)) {
        return 0;
    }
    if (cFadeScreen_drawFade(((char*)self)) == 0) {
        cPS2GraphicsMan* graphicsMan1 = GrMan;
        sVEntry<void>* vt1 = (sVEntry<void>*)VPTR_OF(cGraphicsMan, graphicsMan1);
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
        sVEntry<void>* vt2 = (sVEntry<void>*)VPTR_OF(cGraphicsMan, graphicsMan2);
        ((FnView_17FBF0)vt2[26].fn)((char*)graphicsMan2 + vt2[26].delta, 0, 0, zeroF, zeroF, 640.0f, 480.0f, -1.0f, one);
        cPS2GraphicsMan* graphicsMan3 = GrMan;
        sVEntry<void>* vt3 = (sVEntry<void>*)VPTR_OF(cGraphicsMan, graphicsMan3);
        ((FnP_17FBF0)vt3[34].fn)((char*)graphicsMan3 + vt3[34].delta, &t4mat_Identity);
        cPS2GraphicsMan* graphicsMan4 = GrMan;
        setB23_17FBF0(rs_17FBF0((char*)graphicsMan4), 2);
        setB20_17FBF0(rs_17FBF0((char*)graphicsMan4), 3);
        setB12_17FBF0(rs_17FBF0((char*)graphicsMan4), 13);
        setB2_17FBF0(rs_17FBF0((char*)graphicsMan4), 5);
        setB22_17FBF0(rs_17FBF0((char*)graphicsMan4), 1);
        cHeartBeat_draw((cHeartBeat*)(&self->HeartBeat), self->ParticleTID);
        setC5_17FBF0(rs_17FBF0((char*)GrMan), 9);
        void* menu = self->ActiveScreen;
        sVEntry<void>* mvt = *(sVEntry<void>**)menu;
        ((FnV_17FBF0)mvt[3].fn)((char*)menu + mvt[3].delta);
    }
    cPS2GraphicsMan* graphicsMan5 = GrMan;
    sVEntry<void>* vt5 = (sVEntry<void>*)VPTR_OF(cGraphicsMan, graphicsMan5);
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
extern "C" void cFEInput_getState(cFEInput* self, signed char k)
{
    int id = 0;
    switch (k) {
    case 0: id = 114; break;
    case 1: id = 115; break;
    case 2: id = 116; break;
    case 3: id = 117; break;
    case 4: id = 118; break;
    case 5: id = 119; break;
    case 6: id = 0x78; break;
    case 7: id = 121; break;
    }
    cInput_getState((int)self->mpInput, id);
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
void* cFEInput_isStart(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 0x70);
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
void* cFEInput_isSelect(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 113);
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
void* cFEInput_isNext(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 122);
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
void* cFEInput_isPrevious(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 123);
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
void* cFEInput_isOption(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 0x7c);
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
void* cFEInput_isMisc(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 125);
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
void* cFEInput_isNextHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 151);
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
void* cFEInput_isPreviousHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 152);
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
void* cFEInput_isOptionHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 153);
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
void* cFEInput_isMiscHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 154);
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
void* cFEInput_isNextReleased(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 155);
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
void* cFEInput_isPreviousReleased(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 156);
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
void* cFEInput_isOptionReleased(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 157);
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
void* cFEInput_isMiscReleased(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 158);
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
void* cFEInput_isUp(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 114);
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
void* cFEInput_isDown(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 115);
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
void* cFEInput_isLeft(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 116);
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
void* cFEInput_isRight(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 117);
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
void* cFEInput_isUpHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 147);
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
void* cFEInput_isDownHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 148);
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
void* cFEInput_isLeftHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 149);
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
void* cFEInput_isRightHeld(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 150);
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
void* cFEInput_isRShift1(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 0x7e);
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
void* cFEInput_isRShift2(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 0x7f);
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
void* cFEInput_isLShift1(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 0x80);
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
void* cFEInput_isLShift2(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 129);
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
void* cFEInput_isRShift1Held(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 130);
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
void* cFEInput_isRShift2Held(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 131);
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
void* cFEInput_isLShift1Held(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 132);
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
void* cFEInput_isLShift2Held(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 133);
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
void* cFEInput_isCamZoom(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 144);
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
void* cFEInput_isCamPan(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 146);
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
void* cFEInput_isCamRotate(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getState((int)self->mpInput, 145);
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
void* cFEInput_getCamZoom(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getValue((int)self->mpInput, 144);
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
void* cFEInput_getCamPan(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getValue((int)self->mpInput, 146);
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
void* cFEInput_getCamRotate(void* vself)
{
    cFEInput* self = (cFEInput*)vself;
    return cInput_getValue((int)self->mpInput, 145);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_keyPressed);
#ifdef SKIP_ASM
/**
 * Tells whether any front-end button is down on this input's pad.
 *
 * Checks cInput_getState on the pad input (+0x18) for states 0x71..0x7D and 0x90..0x92.
 *
 * @return 1 when one of them is active, otherwise 0
 */
extern "C" int cFEInput_keyPressed(cFEInput* self)
{
    int pressed = 0;
    if (cInput_getState((int)self->mpInput, 114) || cInput_getState((int)self->mpInput, 115) ||
        cInput_getState((int)self->mpInput, 116) || cInput_getState((int)self->mpInput, 117) ||
        cInput_getState((int)self->mpInput, 118) || cInput_getState((int)self->mpInput, 119) ||
        cInput_getState((int)self->mpInput, 0x78) || cInput_getState((int)self->mpInput, 121) ||
        cInput_getState((int)self->mpInput, 122) || cInput_getState((int)self->mpInput, 123) ||
        cInput_getState((int)self->mpInput, 125) || cInput_getState((int)self->mpInput, 0x7C) ||
        cInput_getState((int)self->mpInput, 144) || cInput_getState((int)self->mpInput, 146) ||
        cInput_getState((int)self->mpInput, 145) || cInput_getState((int)self->mpInput, 113))
        pressed = 1;
    return pressed;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_setPadNum);
#ifdef SKIP_ASM

/**
 * Binds this front-end input to a controller pad.
 *
 * Stores @p idx (+0x4) and SSXApp's cInput for that pad (SSXApp +0xB0 + 4 * idx) in the input pointer (+0x18).
 *
 * @param idx  the pad number
 */
extern "C" void cFEInput_setPadNum(cFEInput* self, unsigned char idx)
{
    int controllerInput = (int)SSXApp->Inputs[idx];
    self->mPadNum = idx;
    self->mpInput = (cInput*)controllerInput;
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
extern "C" int cFEInput_isActive(cFEInput* self)
{
    void* input = self->mpInput;
    if (input != 0) {
        return **(int**)input != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cFEInput_update);
#ifdef SKIP_ASM
extern void* Keyboard;
extern "C" int func_00266BA8(void* mgr, int a1, int* out0, int* out1);

struct sDebugFE1804B8 {
    char pad_0x00[0xC];
    int found; // 0xC
    int a;     // 0x10
    int b;     // 0x14
};

/**
 * Polls the USB keyboard for the front-end input, once per update.
 *
 * Clears the key-ready word (+0xC, On Tour mbKeyReady). When a keyboard is connected (Keyboard), zeroes the virtual
 * key (+0x10, miVKey) and the ASCII key (+0x14, miAsciiKey) and asks func_00266BA8(keyboard, 0, &vkey, &ascii) for a
 * key press; when it returns one, key-ready becomes 1. cFEInput_getKeyReady, cFEInput_getVKey and cFEInput_getAsciiKey
 * return the results. The last virtual of cFEInput; On Tour's PSP version is empty.
 */
extern "C" void cFEInput_update(cFEInput* self)
{
    self->mbKeyReady = 0;
    if (Keyboard != 0) {
        self->miAsciiKey = 0;
        self->miVKey = 0;
        if (func_00266BA8(Keyboard, 0, &self->miVKey, &self->miAsciiKey) != 0)
            self->mbKeyReady = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", func_00180508);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned short* tAttrScreen_tAttrScreen(unsigned short* p);
extern "C" int cList_cList(void* p);
extern "C" int cUIThing_cUIThing(void* p, int a1, int a2);
extern int cScreenKeyboard_vtbl[];
extern int cUIScreen_vtbl[];
extern int cUIGroup_vtbl[];

/**
 * Constructs an on-screen keyboard (cScreenKeyboard) for a UI state, with its default options.
 *
 * Builds the cUIScreen part like cScreenKeyboard_cScreenKeyboard (list links, cUIScreen_vtbl, tAttrScreen_tAttrScreen
 * at +0x14, the lists at +0x18 and +0xB4, the root group cUIThing_cUIThing at +0x40 with cUIGroup_vtbl), stores @p parentState
 * at +0xD0 and the keyboard vtable cScreenKeyboard_vtbl, then sets the keyboard's fields (On Tour names): mIncludeNumbers
 * (+0xD8), mIncludeShift (+0xDC) and mIncludeSpace (+0xE0) 0, mShiftOn (+0xE4) and mFirstKey (+0xE8) 1, an empty input
 * text (+0xEC), mCursorPos (+0x16C) 0, mCurrentKey (+0x170) 10 (the "A" key) and mMaxChars (+0xD4) 45. No caller of it
 * is known (none in the C sources or the asm).
 *
 * @param parentState  the owning UI state; cScreenKeyboard_pressKey calls its virtual slot 19 after each key press
 * @return @p self
 */
extern "C" void* func_00180508(cScreenKeyboard* self, int parentState)
{
    self->mpNext = (cListNode*)self;
    self->mpPrev = (cListNode*)self;
    self->mpBuffer = 0;
    self->mpScreenHeader = 0;
    self->mVtbl = (void**)cUIScreen_vtbl;
    tAttrScreen_tAttrScreen((unsigned short*)(&self->mAttrScreen));
    cList_cList(self->mThreads);
    self->mpKeyframeBlock = 0;
    self->mpObjectBlock = 0;
    self->mppObjectList = 0;
    cUIThing_cUIThing(self->mRootGroup, 0, 0);
    *(int**)(((char*)self) + 0x48) = cUIGroup_vtbl;
    cList_cList(&self->mRootGroup[116]);
    self->mpState = (cUIState*)parentState;
    self->mVtbl = (void**)cScreenKeyboard_vtbl;
    self->mIncludeNumbers = 0;
    self->mIncludeShift = 0;
    self->mIncludeSpace = 0;
    self->mShiftOn = 1;
    self->mFirstKey = 1;
    self->mCurrentKey = 10;
    self->mMaxChars = 45;
    self->mHiScoreName[0] = 0;
    self->mCursorPos = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_setInputText);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);

/**
 * Sets the on-screen keyboard's input text and puts the cursor at its end.
 *
 * Copies @p text into the input buffer at +0xEC (strcpy, no length check) and stores its length in mCursorPos (+0x16C).
 * cScreenKeyboard_initScreenKeyboard calls it with "mypro1".
 *
 * @param text  the new text
 */
extern "C" void cScreenKeyboard_setInputText(cScreenKeyboard* self, const char* text)
{
    char* buf = self->mHiScoreName;
    strcpy(buf, text);
    self->mCursorPos = strlen(buf);
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_kbMoveLeft);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's selection one key left, wrapping within the row.
 *
 * Keys are 4 rows of 10 (0-9 digits, 10-35 A-Z, 36 arrLeft, 37 arrRight, 38 spc, 39 end) in mCurrentKey (+0x170). The
 * first key of a row (0, 10, 20, 30) wraps to the last (+9); from 39 it jumps to 37 when mIncludeSpace (+0xE0) is 0,
 * skipping the space key; otherwise it steps back one. Called by the keyboard's input handler cScreenKeyboard_onInput.
 */
extern "C" void cScreenKeyboard_kbMoveLeft(cScreenKeyboard* self)
{
    int keyIndex = self->mCurrentKey;
    if (keyIndex == 0 || keyIndex == 10 || keyIndex == 20 || keyIndex == 30) {
        self->mCurrentKey += 9;
    } else if (keyIndex == 39 && self->mIncludeSpace == 0) {
        self->mCurrentKey = 37;
    } else {
        self->mCurrentKey -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_kbMoveRight);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's selection one key right, wrapping within the row.
 *
 * The last key of a row (9, 19, 29, 39) wraps to the first (-9); from 37 it jumps to 39 when mIncludeSpace (+0xE0) is
 * 0, skipping the space key; otherwise mCurrentKey (+0x170) steps forward one. Called by the keyboard's input handler
 * cScreenKeyboard_onInput.
 */
extern "C" void cScreenKeyboard_kbMoveRight(cScreenKeyboard* self)
{
    int keyIndex = self->mCurrentKey;
    if (keyIndex == 9 || keyIndex == 19 || keyIndex == 29 || keyIndex == 39) {
        self->mCurrentKey -= 9;
    } else if (keyIndex == 37 && self->mIncludeSpace == 0) {
        self->mCurrentKey = 39;
    } else {
        self->mCurrentKey += 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_kbMoveUp);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's selection one row up, wrapping from the top row to the bottom one.
 *
 * From the digit row (0-9) it goes to the bottom row (+30), or to 39 from key 8 when mIncludeSpace (+0xE0) is 0 (key 38
 * is the space). From the A-J row (10-19) with mIncludeNumbers (+0xD8) 0 it skips the hidden digit row and goes to the
 * bottom row (+20), again 39 from key 18 without space. Otherwise mCurrentKey (+0x170) goes up one row (-10). Called by
 * the keyboard's input handler cScreenKeyboard_onInput.
 */
extern "C" void cScreenKeyboard_kbMoveUp(cScreenKeyboard* self)
{
    int keyIndex = self->mCurrentKey;
    if (keyIndex >= 0 && keyIndex < 10) {
        if (keyIndex == 8 && self->mIncludeSpace == 0) {
            self->mCurrentKey = 39;
        } else {
            self->mCurrentKey += 30;
        }
    } else if (keyIndex >= 10 && keyIndex < 20 && self->mIncludeNumbers == 0) {
        if (keyIndex == 18 && self->mIncludeSpace == 0) {
            self->mCurrentKey = 39;
        } else {
            self->mCurrentKey += 20;
        }
    } else {
        self->mCurrentKey -= 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_kbMoveDown);
#ifdef SKIP_ASM
/**
 * Moves the on-screen keyboard's selection one row down, wrapping from the bottom row to the top one.
 *
 * From the bottom row (30-39) it goes to the digit row (-30) when mIncludeNumbers (+0xD8) is set, else to the A-J row
 * (-20). From key 28 with mIncludeSpace (+0xE0) 0 it goes to 39 (skipping the space key 38). Otherwise mCurrentKey
 * (+0x170) goes down one row (+10). Called by the keyboard's input handler cScreenKeyboard_onInput.
 */
extern "C" void cScreenKeyboard_kbMoveDown(cScreenKeyboard* self)
{
    int keyIndex = self->mCurrentKey;
    if (keyIndex >= 30 && keyIndex < 40) {
        if (self->mIncludeNumbers != 0) {
            self->mCurrentKey = keyIndex - 30;
        } else {
            self->mCurrentKey = keyIndex - 20;
        }
    } else if (keyIndex == 28 && self->mIncludeSpace == 0) {
        self->mCurrentKey = 39;
    } else {
        self->mCurrentKey += 10;
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_toggleShift);
#ifdef SKIP_ASM
/**
 * Toggles the on-screen keyboard's shift (mShiftOn, +0xE4), which cScreenKeyboard_pressKey uses to choose upper case.
 *
 * @return the new mShiftOn value
 */
extern "C" int cScreenKeyboard_toggleShift(cScreenKeyboard* self)
{
    int toggled = self->mShiftOn ^ 1;
    self->mShiftOn = toggled;
    return toggled;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_getPasswordText);
#ifdef SKIP_ASM

/**
 * Returns the on-screen keyboard's input text masked as a password: one '*' per character, in the buffer at +0x12C.
 *
 * @return the masked text (+0x12C)
 */
extern "C" char* cScreenKeyboard_getPasswordText(cScreenKeyboard* self)
{
    int n = strlen(self->mHiScoreName);
    int i;
    for (i = 0; i < n; i++) {
        self->mPasswordText[i] = '*';
    }
    self->mPasswordText[n] = 0;
    return self->mPasswordText;
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_pressKey);
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

/**
 * Applies the on-screen keyboard's selected key (mCurrentKey, +0x170) to the input text at +0xEC, then notifies the
 * owning state.
 *
 * Keys 0-9 write a digit and 10-35 a letter (lower case unless mShiftOn, +0xE4, is set) at mCursorPos (+0x16C); 38
 * writes a space. 36 (arrLeft) moves the cursor back one and cuts the text there; 37 (arrRight) moves it forward one
 * and writes a terminator at the new place. Moving forward stops below mMaxChars (+0xD4). Finally the owner state at
 * +0xD0 gets its virtual slot 19 with (0, 13) for the end key (39) or (0, 12) for any other. Called by the keyboard's
 * input handler cScreenKeyboard_onInput.
 *
 * Likely bugs: the digit and space keys do not write a terminator after the new character (the letter keys do), so
 * characters cut off by arrLeft come back after typing a digit or a space; arrRight at the end of the text leaves the
 * old terminator in place, so what is typed after it is hidden.
 */
extern "C" void cScreenKeyboard_pressKey(sDbgKbd* self)
{
    int k = self->key;
    if (k >= 0 && k < 10) {
        self->buf[self->cur] = self->key + '0';
        if (self->cur + 1 < self->maxLen)
            self->cur = self->cur + 1;
    } else if (k >= 10 && k < 36) {
        int letterBase = !self->caps ? 'a' : 'A';
        self->buf[self->cur] = letterBase + (self->key - 10);
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
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_getKeyObjName);
#ifdef SKIP_ASM
struct sStr10 { char c[10]; };
extern sStr10 D_0045D550;
extern char str_arrLeft[];
extern char str_arrRight[];
extern char str_spc[];
extern char str_end[];
extern "C" char* strcat(char*, const char*);

/**
 * Builds the UI object name of a keyboard key: "keyboard_" plus the key's label.
 *
 * Starts @p dst with "keyboard_" (D_0045D550) and appends the digit for 0-9, the upper-case letter for 10-35, or
 * "arrLeft", "arrRight", "spc" or "end" for 36..39; other values leave just the prefix.
 * cScreenKeyboard_initScreenKeyboard uses it to find the key objects.
 *
 * @param keyIndex  key index (0-39)
 * @param dst  output buffer (at least 18 bytes)
 */
extern "C" void cScreenKeyboard_getKeyObjName(cScreenKeyboard* self, int keyIndex, char* dst)
{
    char buf[2];
    *(sStr10*)dst = D_0045D550;
    if (keyIndex < 10) {
        buf[0] = keyIndex + '0';
        buf[1] = 0;
        strcat(dst, buf);
    } else if (keyIndex < 36) {
        buf[0] = keyIndex + 'A' - 10;
        buf[1] = 0;
        strcat(dst, buf);
    } else if (keyIndex == 36) {
        strcat(dst, str_arrLeft);
    } else if (keyIndex == 37) {
        strcat(dst, str_arrRight);
    } else if (keyIndex == 38) {
        strcat(dst, str_spc);
    } else if (keyIndex == 39) {
        strcat(dst, str_end);
    }
}
#endif

//100%
INCLUDE_ASM("fe/debugfe", cScreenKeyboard_getCursorObjName);
#ifdef SKIP_ASM
struct sStr8 { char c[8]; };
extern char str_cursor[];
extern char str_arrLeft[];
extern char str_arrRight[];
extern char str_spc[];
extern char str_end[];
extern "C" char* strcat(char*, const char*);

/**
 * Builds the UI object name of a keyboard key's cursor: "cursor_" plus the key's label.
 *
 * Starts @p dst with "cursor_" (str_cursor) and appends the digit for 0-9, the upper-case letter for 10-35, or
 * "arrLeft", "arrRight", "spc" or "end" for 36..39. The keyboard's input handler cScreenKeyboard_onInput uses it to hide the old
 * cursor and show the new one; cScreenKeyboard_initScreenKeyboard to hide every cursor but the one on "A".
 *
 * @param keyIndex  key index (0-39)
 * @param dst  output buffer (at least 16 bytes)
 */
extern "C" void cScreenKeyboard_getCursorObjName(cScreenKeyboard* self, int keyIndex, char* dst)
{
    char buf[2];
    *(sStr8*)dst = *(sStr8*)str_cursor;
    if (keyIndex < 10) {
        buf[0] = keyIndex + '0';
        buf[1] = 0;
        strcat(dst, buf);
    } else if (keyIndex < 36) {
        buf[0] = keyIndex + 'A' - 10;
        buf[1] = 0;
        strcat(dst, buf);
    } else if (keyIndex == 36) {
        strcat(dst, str_arrLeft);
    } else if (keyIndex == 37) {
        strcat(dst, str_arrRight);
    } else if (keyIndex == 38) {
        strcat(dst, str_spc);
    } else if (keyIndex == 39) {
        strcat(dst, str_end);
    }
}
#endif

