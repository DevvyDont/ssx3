#include "vtable.h"
#include "common.h"
#include "render/2drender.h"
#include "camera/camera.h"
#include "main/gameviewman.h"
#include "camera/cameracontroller.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "world/world.h"
#include "world/worldcache.h"

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_cGameViewMan);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cCamera_cCamera(void* self, int idx);
extern "C" void cCamera_setCurrentController(void* self, int mode);
extern char str_GameCamera[];

struct sVECam22E3B8 {
    short delta;
    short index;
    void (*fn)(void*, int, float, float, float);
};


class cDisp22E3B8 {
public:
    char pad[0x10D8];
    // vptr at 0x10D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual unsigned int GetScreenWidth();
    virtual unsigned int GetScreenHeight();
};

struct sCamRef22E3B8 {
    cCamera* p;
    sCamRef22E3B8() {}
    void* operator new[](unsigned int, void* where) { return where; }
};

struct cGameViewMan22E3B8 {
    unsigned int m_uiNumPlayers;
    sCamRef22E3B8 cams[2];
    int m_GFState;
    int m_uiNumView;
    int m_uiCurrentView;
    int f18;
    int f1C;
    float width;
    float height;
};

/**
 * Constructs the game view manager with one camera per player and a single full-screen view.
 *
 * For each of the two slots below @p count it creates a cCamera ("GameCamera", 0x4B0 bytes), calls its init
 * (slot 2) with the slot index, a 45-degree field of view, near 30 and far 30000 (20000 with two players),
 * and selects controller 2; other slots get 0. One view (+0x10) is active and view 0's rectangle is the whole
 * display (GrMan slots 10 and 11).
 *
 * @param count  number of players, hence cameras (On Tour uiNumPlayers)
 * @return @p self
 */
extern "C" cGameViewMan22E3B8* cGameViewMan_cGameViewMan(cGameViewMan22E3B8* self, unsigned int count)
{
    new ((char*)self + 4) sCamRef22E3B8[2];
    self->m_uiNumPlayers = count;
    float farClip = 30000.0f;
    if (count >= 2) {
        farClip = 20000.0f;
    }
    unsigned int i;
    for (i = 0; i < 2; i++) {
        if (i < self->m_uiNumPlayers) {
            self->cams[i].p = (cCamera*)cCamera_cCamera(cMemMan_alloc(1200, str_GameCamera, 0x20000000, 0), i);
            cCamera* camera = self->cams[i].p;
            ((sVECam22E3B8*)camera->mVtbl)[2].fn(vthis(camera, ((sVECam22E3B8*)camera->mVtbl)[2]), i, 0.7853981852531433f, 30.0f, farClip);
            cCamera_setCurrentController(self->cams[i].p, 2);
        } else {
            self->cams[i].p = 0;
        }
    }
    self->m_GFState = 0;
    self->m_uiNumView = 1;
    self->m_uiCurrentView = 0;
    float w = (float)((cDisp22E3B8*)GrMan)->GetScreenWidth();
    float screenHeight = (float)((cDisp22E3B8*)GrMan)->GetScreenHeight();
    self->width = w;
    self->height = screenHeight;
    self->f18 = 0;
    self->f1C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_updateViewState);
#ifdef SKIP_ASM

/**
 * Splits or joins the screen when the game's state changes: one full-screen view, or two side-by-side halves for states
 * 3-5.
 *
 * Does nothing when the stored state (m_GFState) already equals the game's mCurrentStateID, while a replay is
 * playing (Replay state 1-9), or with one player (m_uiNumPlayers). Otherwise stores the state and, from GrMan's width
 * and height (vtable slots 10 and 11), sets m_uiNumView, m_uiCurrentView and m_ViewRect: states 3-5 give two
 * half-width views with far clip 20000 on both cameras, any other state one full view with far clip 30000. Then
 * clears the world view's stream-focus active flags for the unused view (one view) or for both (two views).
 * cGame_updateState calls it.
 */
extern "C" void cGameViewMan_updateViewState(cGameViewMan* self)
{
    cGame* game = SSXApp->Game;
    int mode = game->mCurrentStateID;
    if (self->m_GFState == mode) {
        return;
    }
    int n = *(int*)game->Replay;
    int busy = n && n < 10;
    if (busy) {
        return;
    }
    self->m_GFState = mode;
    if (self->m_uiNumPlayers == 1) {
        return;
    }
    float w = (float)(unsigned int)GrMan->GetScreenWidth();
    float screenHeight = (float)(unsigned int)GrMan->GetScreenHeight();
    switch (self->m_GFState) {
    case 3:
    case 4:
    case 5: {
        float hw = w * 0.5f;
        self->m_uiNumView = 2;
        self->m_uiCurrentView = 0;
        ((&self->m_ViewRect[0]))[0].X = 0.0f;
        ((&self->m_ViewRect[0]))[0].Y = 0.0f;
        ((&self->m_ViewRect[0]))[0].W = hw;
        ((&self->m_ViewRect[0]))[0].H = screenHeight;
        ((&self->m_ViewRect[0]))[1].X = hw;
        ((&self->m_ViewRect[0]))[1].Y = 0.0f;
        ((&self->m_ViewRect[0]))[1].W = hw;
        ((&self->m_ViewRect[0]))[1].H = screenHeight;
        self->PlayerCamera[0]->MaxFar = 20000.0f;
        self->PlayerCamera[1]->MaxFar = 20000.0f;
        break;
    }
    default:
        self->m_uiNumView = 1;
        ((&self->m_ViewRect[0]))[0].W = w;
        ((&self->m_ViewRect[0]))[0].H = screenHeight;
        self->m_uiCurrentView = 0;
        ((&self->m_ViewRect[0]))[0].X = 0.0f;
        ((&self->m_ViewRect[0]))[0].Y = 0.0f;
        self->PlayerCamera[0]->MaxFar = 30000.0f;
        break;
    }
    if (((int)self->m_uiNumView) == 1) {
        int idx = self->PlayerCamera[1]->StreamFocusID;
        SSXApp->Game->World->m_pWorldCache->m_WorldView.m_StreamFocus[idx].m_bActive = 0;
    } else {
        SSXApp->Game->World->m_pWorldCache->m_WorldView.m_StreamFocus[0].m_bActive = 0;
        SSXApp->Game->World->m_pWorldCache->m_WorldView.m_StreamFocus[1].m_bActive = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan__cGameViewMan);
#ifdef SKIP_ASM
void operator_delete(int* p);

/**
 * Destroys the game view manager and every view it holds.
 *
 * Deletes each non-null view through its virtual destructor (slot 1, flags 3), clears m_uiNumView and frees this
 * object when bit 0 of @p flags is set.
 *
 * @param flags  bit 0: free the object's memory
 */
extern "C" void cGameViewMan__cGameViewMan(cGameViewMan* self, int flags)
{
    unsigned int i;
    for (i = 0; i < self->m_uiNumPlayers; i++) {
        cCamera* it = self->PlayerCamera[i];
        if (it != 0) {
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)it->mVtbl;
            vt[1].fn(vthis(it, vt[1]), 3);
        }
    }
    self->m_uiNumView = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_snapAll);
#ifdef SKIP_ASM

/**
 * Snaps the camera of every active view through its current controller's virtual slot 4 (probably snap).
 *
 * Loops over the active view count (m_uiNumView), not the camera count (m_uiNumPlayers); the controller is the
 * camera's Controller.
 */
extern "C" void cGameViewMan_snapAll(cGameViewMan* self)
{
    unsigned int i;
    for (i = 0; i < self->m_uiNumView; i++) {
        cCamera* camera = self->PlayerCamera[i];
        cCameraController* controller = camera->Controller;
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)controller->mVtbl;
        vt[4].fn(vthis(controller, vt[4]));
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_updateAll);
#ifdef SKIP_ASM

/**
 * Updates the camera of every active view through its virtual update (cCamera vtable slot 3).
 *
 * Loops over the active view count (+0x10), not the camera count (+0x0).
 */
extern "C" void cGameViewMan_updateAll(cGameViewMan* self)
{
    unsigned int i;
    for (i = 0; i < self->m_uiNumView; i++) {
        cCamera* camera = self->PlayerCamera[i];
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)camera->mVtbl;
        vt[3].fn(vthis(camera, vt[3]));
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_updateStreamFocus);
#ifdef SKIP_ASM
extern "C" void cCamera_updateStreamFocus(void* p);

/**
 * Updates the streaming focus of each view's camera: cCamera_updateStreamFocus on the cameras at +0x4 for the view
 * count at +0x10.
 *
 * cGame_update and cGame_render call it.
 */
extern "C" void cGameViewMan_updateStreamFocus(cGameViewMan* self)
{
    unsigned int i;
    for (i = 0; i < self->m_uiNumView; i++) {
        cCamera_updateStreamFocus(self->PlayerCamera[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_isStreamReady);
#ifdef SKIP_ASM
struct sRenderList {
    int field_0x0;
    void* items[3];
    unsigned int count;
};

/**
 * Tells whether streaming is ready for every view: each view camera's word at +0xB4 is non-zero.
 *
 * Checks the cameras at +0x4 for the view count at +0x10. cGame_update and cGame_render test it.
 *
 * @return 1 when every view is ready, otherwise 0
 */
extern "C" int cGameViewMan_isStreamReady(cGameViewMan* self)
{
    unsigned int i;
    for (i = 0; i < self->m_uiNumView; i++) {
        if (self->PlayerCamera[i]->bStreamIsReady == 0) {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGame_cGame);
#ifdef SKIP_ASM
extern char cGame_vtbl[];
extern char cGFReplayStartState_vtbl[];
extern char cGFPodiumState_vtbl[];
extern char cGFPostEventState_vtbl[];
extern char cGFPreEventState_vtbl[];
extern char cGFSessionTeleportState_vtbl[];
extern char cGFNextHeatState_vtbl[];
extern char cGFRidingState_vtbl[];
extern char cGFGateState_vtbl[];
extern char cGFTransportWaitState_vtbl[];
extern char cGFEndWaitState_vtbl[];
extern char cGFEndState_vtbl[];
extern char cGameState_vtbl[];
extern char* cGameState_mpGame;
extern int cPauseState_StackIdx;
extern "C" void cFadeScreen_cFadeScreen(void* self);
extern "C" void* cGFFinishlineState_cGFFinishlineState(void* self);
extern "C" void cGFEventStartState_cGFEventStartState(void* self);
extern "C" void* cGFTrackStartState_cGFTrackStartState(void* self);
extern "C" void cGFUserSelectState_cGFUserSelectState(void* self);
extern "C" void cGFSectionLoadState_cGFSectionLoadState(void* self);
extern "C" void cPauseState_set(int a);

static inline void Base_22E968(char* s, int off)
{
    *(void**)(s + off + 0xC) = cGameState_vtbl;
    *(int*)(s + off + 0x0) = 0;
    cGameState_mpGame = 0;
}

static inline void Init_22E968(char* s, int off, void* vt, int id)
{
    Base_22E968(s, off);
    *(void**)(s + off + 0xC) = vt;
    *(int*)(s + off + 0x4) = 0;
    *(int*)(s + off + 0x0) = id;
}

/**
 * Constructs the in-game module: clears its key subsystem pointers and builds its game-flow states in place.
 *
 * Runs the module base constructor (cFadeScreen_cFadeScreen), stores cGame's vtable, clears AI, DebugFont, UI,
 * InputSync and ViewMan, clears cPauseState_StackIdx and calls cPauseState_set(0). It then constructs the cGF* state
 * objects in mStates (gate, podium, next heat, session teleport, replay start and others); each base state
 * constructor also clears cGameState_mpGame.
 *
 * Finally PauseOwner = -1, ControllerUnplugged = 0, and both TransitionMan and SplashManager entries are
 * cleared; the other subsystems are left for cGame_load.
 *
 * @return @p self
 */
extern "C" char* cGame_cGame(cGame* self)
{
    cFadeScreen_cFadeScreen(((char*)self));
    *(int*)&self->AI = 0;
    *(int*)&self->DebugFont = 0;
    *(void**)&self->mVtbl = cGame_vtbl;
    *(int*)&self->UI = 0;
    *(int*)&self->InputSync = 0;
    cPauseState_StackIdx = 0;
    cPauseState_set(0);
    *(int*)&self->ViewMan = 0;
    cGFEventStartState_cGFEventStartState(&self->mEventStartState);
    Init_22E968(((char*)self), 208, cGFPreEventState_vtbl, 3);
    Init_22E968(((char*)self), 0xE0, cGFGateState_vtbl, 4);
    Init_22E968(((char*)self), 0xF0, cGFRidingState_vtbl, 5);
    cGFFinishlineState_cGFFinishlineState(&self->mFinishlineState);
    Init_22E968(((char*)self), 280, cGFEndState_vtbl, 8);
    Init_22E968(((char*)self), 296, cGFPostEventState_vtbl, 8);
    Init_22E968(((char*)self), 312, cGFEndWaitState_vtbl, 11);
    Init_22E968(((char*)self), 328, cGFTransportWaitState_vtbl, 10);
    cGFTrackStartState_cGFTrackStartState(&self->mTrackStartState);
    cGFSectionLoadState_cGFSectionLoadState(&self->mSectionLoadState);
    Init_22E968(((char*)self), 396, cGFPodiumState_vtbl, 7);
    Base_22E968(((char*)self), 412);
    self->mNextHeatState.mVtbl = (void**)cGFNextHeatState_vtbl;
    self->mNextHeatState.mStartupFlag = 3;
    self->mNextHeatState.WaitTicks = 0;
    self->mNextHeatState.PlayTransport = 0;
    self->mNextHeatState.mNextState = 1;
    cGFUserSelectState_cGFUserSelectState(&self->mUserSelectState);
    Init_22E968(((char*)self), 476, cGFSessionTeleportState_vtbl, 10);
    Init_22E968(((char*)self), 492, cGFReplayStartState_vtbl, 4);
    self->PauseOwner = -1;
    self->ControllerUnplugged = 0;
    for (int i = 0; i < 2; i++) {
        self->TransitionMan[i] = 0;
        self->SplashManager[i] = 0;
    }
    return ((char*)self);
}
#endif

