#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

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

struct sCam22E3B8 {
    char pad[0x90];
    sVECam22E3B8* vt;
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
    virtual unsigned int v10();
    virtual unsigned int v11();
};

struct sCamRef22E3B8 {
    sCam22E3B8* p;
    sCamRef22E3B8() {}
    void* operator new[](unsigned int, void* where) { return where; }
};

struct cGameViewMan22E3B8 {
    unsigned int count;
    sCamRef22E3B8 cams[2];
    int fC;
    int f10;
    int f14;
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
    self->count = count;
    float farClip = 30000.0f;
    if (count >= 2) {
        farClip = 20000.0f;
    }
    unsigned int i;
    for (i = 0; i < 2; i++) {
        if (i < self->count) {
            self->cams[i].p = (sCam22E3B8*)cCamera_cCamera(cMemMan_alloc(1200, str_GameCamera, 0x20000000, 0), i);
            sCam22E3B8* camera = self->cams[i].p;
            camera->vt[2].fn((char*)camera + camera->vt[2].delta, i, 0.7853981852531433f, 30.0f, farClip);
            cCamera_setCurrentController(self->cams[i].p, 2);
        } else {
            self->cams[i].p = 0;
        }
    }
    self->fC = 0;
    self->f10 = 1;
    self->f14 = 0;
    float w = (float)((cDisp22E3B8*)GrMan)->v10();
    float screenHeight = (float)((cDisp22E3B8*)GrMan)->v11();
    self->width = w;
    self->height = screenHeight;
    self->f18 = 0;
    self->f1C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E550);
#ifdef SKIP_ASM

class cDisp22E550 {
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
    virtual unsigned int v10();
    virtual unsigned int v11();
};

struct sCam22E550 {
    char pad[0x14];
    float farClip;      // 0x14
    char pad18[0xB0 - 0x18];
    int index;          // 0xB0
};

struct sSlot22E550 {
    int active;
    char pad[0x4C];
};

struct sSlots22E550 {
    char pad[0x250];
    sSlot22E550 e[2];
};

struct sRect22E550 {
    float x, y, w, h;
};

struct cGameViewMan22E550 {
    unsigned int count;      // 0x0
    sCam22E550* cams[2];     // 0x4
    int mode;                // 0xC
    int views;               // 0x10
    int f14;                 // 0x14
    sRect22E550 rects[2];    // 0x18
};

extern "C" void func_0022E550(cGameViewMan22E550* self)
{
    char* game = (char*)SSXApp->Game;
    int mode = *(int*)(game + 0x214);
    if (self->mode == mode) {
        return;
    }
    int n = **(int**)(game + 0x28);
    int busy = n && n < 10;
    if (busy) {
        return;
    }
    self->mode = mode;
    if (self->count == 1) {
        return;
    }
    float w = (float)((cDisp22E550*)GrMan)->v10();
    float h = (float)((cDisp22E550*)GrMan)->v11();
    switch (self->mode) {
    case 3:
    case 4:
    case 5: {
        float hw = w * 0.5f;
        self->views = 2;
        self->f14 = 0;
        self->rects[0].x = 0.0f;
        self->rects[0].y = 0.0f;
        self->rects[0].w = hw;
        self->rects[0].h = h;
        self->rects[1].x = hw;
        self->rects[1].y = 0.0f;
        self->rects[1].w = hw;
        self->rects[1].h = h;
        self->cams[0]->farClip = 20000.0f;
        self->cams[1]->farClip = 20000.0f;
        break;
    }
    default:
        self->views = 1;
        self->rects[0].w = w;
        self->rects[0].h = h;
        self->f14 = 0;
        self->rects[0].x = 0.0f;
        self->rects[0].y = 0.0f;
        self->cams[0]->farClip = 30000.0f;
        break;
    }
    if (self->views == 1) {
        int idx = self->cams[1]->index;
        (*(sSlots22E550**)SSXApp->Game->World)->e[idx].active = 0;
    } else {
        (*(sSlots22E550**)SSXApp->Game->World)->e[0].active = 0;
        (*(sSlots22E550**)SSXApp->Game->World)->e[1].active = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan__cGameViewMan);
#ifdef SKIP_ASM
struct sVEntry0022E730 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sItem0022E730 {
    char pad[0x90];
    sVEntry0022E730* vt;
};
struct sList0022E730 {
    unsigned int count;
    sItem0022E730* items[3];
    int field_0x10;
};
void operator_delete(int* p);

/**
 * Destroys the game view manager and every view it holds.
 *
 * Deletes each non-null view through its virtual destructor (slot 1, flags 3), clears +0x10 and frees this
 * object when bit 0 of @p flags is set.
 *
 * @param flags  bit 0: free the object's memory
 */
extern "C" void cGameViewMan__cGameViewMan(sList0022E730* self, int flags)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        sItem0022E730* it = self->items[i];
        if (it != 0) {
            it->vt[1].fn((char*)it + it->vt[1].delta, 3);
        }
    }
    self->field_0x10 = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_snapAll);
#ifdef SKIP_ASM
struct sVEntry_0022E7C8 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Snaps the camera of every active view through its current controller's virtual slot 4 (probably snap).
 *
 * Loops over the active view count (+0x10), not the camera count (+0x0); the controller is the camera's
 * Controller (+0xA0).
 */
extern "C" void cGameViewMan_snapAll(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        void* camera = ((void**)((char*)self + 0x4))[i];
        void* controller = *(void**)((char*)camera + 0xA0);
        sVEntry_0022E7C8* vt = *(sVEntry_0022E7C8**)((char*)controller + 0x14);
        vt[4].fn((char*)controller + vt[4].delta);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", cGameViewMan_updateAll);
#ifdef SKIP_ASM
struct sVEntry_0022E840 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Updates the camera of every active view through its virtual update (cCamera vtable slot 3).
 *
 * Loops over the active view count (+0x10), not the camera count (+0x0).
 */
extern "C" void cGameViewMan_updateAll(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        void* camera = ((void**)((char*)self + 0x4))[i];
        sVEntry_0022E840* vt = *(sVEntry_0022E840**)((char*)camera + 0x90);
        vt[3].fn((char*)camera + vt[3].delta);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E8B8);
#ifdef SKIP_ASM
extern "C" void cCamera_updateStreamFocus(void* p);

extern "C" void func_0022E8B8(void* self)
{
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x10); i++) {
        cCamera_updateStreamFocus(((void**)((char*)self + 0x4))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamerender", func_0022E920);
#ifdef SKIP_ASM
struct sRenderList {
    int field_0x0;
    void* items[3];
    unsigned int count;
};

extern "C" int func_0022E920(sRenderList* self)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        if (*(int*)((char*)self->items[i] + 0xb4) == 0) {
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
extern char D_0047D218[];
extern char D_0047D270[];
extern char cGFSessionTeleportState_vtbl[];
extern char cGFNextHeatState_vtbl[];
extern char D_0047D480[];
extern char cGFGateState_vtbl[];
extern char D_0047D588[];
extern char D_0047D638[];
extern char D_0047D690[];
extern char D_0047D6E8[];
extern char* cGameState_mpGame;
extern int D_004A2A54;
extern "C" void func_00231CD0(void* self);
extern "C" void* func_00233C10(void* self);
extern "C" void func_00234188(void* self);
extern "C" void* func_00234EF8(void* self);
extern "C" void func_00236208(void* self);
extern "C" void func_00236868(void* self);
extern "C" void func_00244240(int a);

struct sArr_22E968 {
    char pad[0x5C];
    int a[2];
    int pad64;
    int b[2];
};

static inline void Base_22E968(char* s, int off)
{
    *(void**)(s + off + 0xC) = D_0047D6E8;
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
 * Runs the module base constructor (func_00231CD0), stores cGame's vtable, clears AI, DebugFont, UI,
 * InputSync and ViewMan, clears D_004A2A54 and calls func_00244240(0). It then constructs the cGF* state
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
    func_00231CD0(((char*)self));
    *(int*)&self->AI = 0;
    *(int*)&self->DebugFont = 0;
    *(void**)&self->mVtbl = cGame_vtbl;
    *(int*)&self->UI = 0;
    *(int*)&self->InputSync = 0;
    D_004A2A54 = 0;
    func_00244240(0);
    *(int*)&self->ViewMan = 0;
    func_00234188(((char*)self) + 0xB0);
    Init_22E968(((char*)self), 208, D_0047D270, 3);
    Init_22E968(((char*)self), 0xE0, cGFGateState_vtbl, 4);
    Init_22E968(((char*)self), 0xF0, D_0047D480, 5);
    func_00233C10(((char*)self) + 0x100);
    Init_22E968(((char*)self), 280, D_0047D690, 8);
    Init_22E968(((char*)self), 296, D_0047D218, 8);
    Init_22E968(((char*)self), 312, D_0047D638, 11);
    Init_22E968(((char*)self), 328, D_0047D588, 10);
    func_00234EF8(((char*)self) + 0x158);
    func_00236868(((char*)self) + 0x178);
    Init_22E968(((char*)self), 396, cGFPodiumState_vtbl, 7);
    Base_22E968(((char*)self), 412);
    *(void**)(((char*)self) + 0x1A8) = cGFNextHeatState_vtbl;
    *(int*)(((char*)self) + 0x1A0) = 3;
    *(int*)(((char*)self) + 0x1B0) = 0;
    *(int*)(((char*)self) + 0x1B4) = 0;
    *(int*)(((char*)self) + 0x19C) = 1;
    func_00236208(((char*)self) + 0x1B8);
    Init_22E968(((char*)self), 476, cGFSessionTeleportState_vtbl, 10);
    Init_22E968(((char*)self), 492, cGFReplayStartState_vtbl, 4);
    self->PauseOwner = -1;
    self->ControllerUnplugged = 0;
    for (int i = 0; i < 2; i++) {
        ((sArr_22E968*)self)->a[i] = 0;
        ((sArr_22E968*)self)->b[i] = 0;
    }
    return ((char*)self);
}
#endif

