#include "common.h"
#include "animation/animmodel.h"
#include "ai/computer.h"
#include "ai/player.h"
#include "replay/frameptr.h"
#include "bx/math3d.h"
#include "ai/riderhandler.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "ai/ai.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("ai/ai", cAI_cAI);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char cFreerideHandler_vtbl[];
extern char cPreRaceHandler_vtbl[];
extern char cGameInitHandler_vtbl[];
extern char cCountdownHandler_vtbl[];
extern char cAI_vtbl[];
extern char cAIStateHandler_vtbl[];
extern char cEndRaceHandler_vtbl[];
extern char cShutdownHandler_vtbl[];
extern char str_FocusMan[];
extern char str_DangerMan[];

struct sAIEntry_1286A0 {
    char pad[0x20];
    sAIEntry_1286A0() {}
};

struct sAIState_1286A0 {
    int f0;                     // 0x00
    char pad4[0xC];
    sAIEntry_1286A0 e[6];       // 0x10
    int d0;                     // 0xD0
    int d4;                     // 0xD4
    int d8;                     // 0xD8
    char padDC[0x8E0 - 0xDC];
    sAIState_1286A0() : f0(0) { d0 = -1; d4 = 1; d8 = 0; }
};

struct sAIObj_1286A0 {
    char pad[0xB60];
    sAIObj_1286A0(void* owner) __asm__("cDangerMan_cDangerMan");
};

/**
 * Constructs the AI manager: its vtables, its focus and danger managers and a cleared race rank type.
 *
 * Stores _vt$3cAI and the seven state handlers' vtables (GameInit .. Shutdown). The "FocusMan" block (0x8E0)
 * is built inline (+0x00 = 0, +0xD0 = -1, +0xD4 = 1, +0xD8 = 0); DangerMan is cDangerMan_cDangerMan() on a
 * new "DangerMan" block (0xB60) with this AI. The extern named cAIStateHandler_vtbl (0x00458528) is
 * cRaceHandler's vtable.
 *
 * @return the constructed object (self)
 */
extern "C" void* cAI_cAI(cAI* self)
{
    self->mVtbl = (void**)cAI_vtbl;
    *(void**)&self->mGameInitHandler = cGameInitHandler_vtbl;
    *(void**)&self->mFreerideHandler = cFreerideHandler_vtbl;
    *(void**)&self->mPreRaceHandler = cPreRaceHandler_vtbl;
    *(void**)&self->mCountdownHandler = cCountdownHandler_vtbl;
    *(void**)&self->mRaceHandler = cAIStateHandler_vtbl;
    *(void**)&self->mEndRaceHandler = cEndRaceHandler_vtbl;
    *(void**)&self->mShutdownHandler = cShutdownHandler_vtbl;
    sAIState_1286A0** slot = (sAIState_1286A0**)&self->FocusMan;
    *slot = new (str_FocusMan, 0, 0) sAIState_1286A0;
    self->DangerMan = (cDangerMan*)(new (str_DangerMan, 0, 0) sAIObj_1286A0(self));
    self->RaceRankType = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI__cAI);
#ifdef SKIP_ASM
extern "C" void cAI_shutdown(cAI*);
void operator_delete(int*);
extern char cAI_vtbl[];

/**
 * Destroys the AI manager: shuts it down, frees its focus and danger managers and, if asked, itself.
 *
 * Restores _vt$3cAI and calls cAI_shutdown(); FocusMan and DangerMan are released with operator_delete only
 * (their destructors are not run).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAI__cAI(cAI* self, int flags)
{
    self->mVtbl = (void**)cAI_vtbl;
    cAI_shutdown(self);
    operator_delete((int*)self->FocusMan);
    operator_delete((int*)self->DangerMan);
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_init);
#ifdef SKIP_ASM
extern "C" void cFocusMan_reset(void*);
extern "C" void cDangerMan_Reset(void*);
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cAI_commonInit(cAI*, int);
// PORT: cAI_loadAnims is defined (void) in this unit, but this caller passes self in $a0; bound by asm label.
void cAI_loadAnims_self(void* self) __asm__("cAI_loadAnims");

struct sVEntry128818 { short delta; short index; void (*fn)(void*); };

struct sAiObj128818
{
    int f0;             // 0x00
    char pad4[0xC];
    int f10;            // 0x10
    int f14;            // 0x14
    int f18;            // 0x18
    int f1C;            // 0x1C
    int a20[2];         // 0x20
    int a28[6];         // 0x28
    char pad40[0x8];
    int a48[5];         // 0x48
    char pad5C[0x14];
    int a70[1];         // 0x70
    int f74;            // 0x74
    int f78;            // 0x78
    int f7C;            // 0x7C
    int f80;            // 0x80
    int f84;            // 0x84
    int f88;            // 0x88
    int f8C;            // 0x8C
    int f90;            // 0x90
    int f94;            // 0x94
    int f98;            // 0x98
    char pad9C[0x8];
    void* fA4;          // 0xA4
    void* fA8;          // 0xA8
    char padAC[0x20];
    sVEntry128818* vt;  // 0xCC
};

/**
 * Initialises the AI manager for a new event: clears its state, counts and rider tables, then sets up the players.
 *
 * Resets FocusMan (cFocusMan_reset) and DangerMan (cDangerMan_Reset); zeroes IsAIReadyToRace, ComputerRidersActive,
 * MissionRidersActive, the rider / player / computer / actor / mission-rider counts, CountdownTicksLeft,
 * CurrentAIStateHandler and CurrentAIState; sets the AI state to 1 (GameInit) and clears Riders[6],
 * MissionRiders[1] and Computers[5]. Then loads the AI anims, calls the InitPlayers virtual (slot 6),
 * cAI_commonInit(self, 1) and clears PodiumActorList[2].
 */
extern "C" void cAI_init(sAiObj128818* self)
{
    int i;
    cFocusMan_reset(self->fA4);
    cDangerMan_Reset(self->fA8);
    self->f10 = 0;
    self->f14 = 0;
    self->f90 = 0;
    self->f78 = 0;
    self->f7C = 0;
    self->f80 = 0;
    self->f84 = 0;
    self->f88 = 0;
    self->f1C = 0;
    self->f98 = 0;
    self->f0 = 0;
    cAI_setAIState(self, 1);
    for (i = 0; i < 6; i++) self->a28[i] = 0;
    for (i = 0; i < 1; i++) self->a70[i] = 0;
    for (i = 0; i < 5; i++) self->a48[i] = 0;
    cAI_loadAnims_self(self);
    self->vt[6].fn((char*)self + self->vt[6].delta);
    cAI_commonInit((cAI*)self, 1);
    for (i = 0; i < 2; i++) self->a20[i] = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initAIRiders);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(cAI*);
extern "C" void cAI_initComputerRiders(cAI*);

/**
 * Rebuilds the AI rider set: clears the required actor count, purges loaded mission riders and creates the
 * computer riders.
 *
 * Sets NumberOfActorsRequired to 0, calls cAI_purgeMissionRiders() only when NumberOfMissionRidersLoaded is
 * positive, then cAI_initComputerRiders(), which also sets IsAIReadyToRace.
 */
extern "C" void cAI_initAIRiders(cAI* self)
{
    self->NumberOfActorsRequired = 0;
    if (self->NumberOfMissionRidersLoaded > 0)
    {
        cAI_purgeMissionRiders(self);
    }
    cAI_initComputerRiders(self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initAIActors);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cAI_initComputerActors(cAI*);

/**
 * Sets how many AI actors are needed and creates them, unless the race is on game path 1 or 2.
 *
 * Fetches the BE interface (result unused). Only when the new race's cGamePath (newRace.cGamePath) is 0 does it store
 * @p n in NumberOfActorsRequired and call cAI_initComputerActors(); path 2 is online play.
 *
 * @param n  number of AI actors to create
 */
extern "C" void cAI_initAIActors(cAI* self, int n)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0)
    {
        self->NumberOfActorsRequired = n;
        cAI_initComputerActors(self);
    }
}
#endif

extern "C" void* cAI_commonInit(cAI*, int);

//100%
INCLUDE_ASM("ai/ai", cAI_resolveRace__FPv);
#ifdef SKIP_ASM
/**
 * Re-runs the AI's shared race initialisation when an event or the next heat restarts.
 *
 * Just cAI_commonInit(self, 0): the computer and mission riders are reset, the human riders and EventTimerTick are
 * kept. On Tour's resolveRace first waits for isAILoadedAndReady(); retail has no such wait.
 *
 * @return cAI_commonInit's value, which has no meaning (On Tour's resolveRace returns void; the callers ignore it)
 */
void* cAI_resolveRace(void* self)
{
    return cAI_commonInit((cAI*)self, 0);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initFreeride);
#ifdef SKIP_ASM
struct sAiObj128A48;
extern "C" void cAI_setRaceRankType(cAI* self, int mode);

/**
 * Prepares the AI for free ride: the shared init without the human riders, then no race ranking.
 *
 * Calls cAI_commonInit(self, 0) (resets the computer riders, keeps EventTimerTick) and
 * cAI_setRaceRankType(self, 0).
 */
extern "C" void cAI_initFreeride(cAI* self)
{
    cAI_commonInit(self, 0);
    cAI_setRaceRankType(self, 0);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_setRaceRankType);
#ifdef SKIP_ASM
struct sAiObj128A48
{
    char pad0[0x28];
    char* mRiders[19];
    int mMode;
    int mCount;
};

/**
 * Sets the race rank type and seeds every rider's place to match it.
 *
 * Stores @p mode in RaceRankType. Type 0 sets each rider's place word (+0xEC, which the ranking pass writes)
 * to 0; any other type sets it to the rider's slot index.
 *
 * @param mode  the rank type (On Tour tRaceRankType type); 0 puts every rider at place 0
 */
extern "C" void cAI_setRaceRankType(cAI* self, int mode)
{
    self->RaceRankType = mode;
    if (mode == 0)
    {
        for (int i = 0; i < self->NumberOfRidersAllocated; i++)
        {
            *(int*)(((char**)&self->Riders[0])[i] + 0xEC) = 0;
        }
    }
    else
    {
        for (int i = 0; i < self->NumberOfRidersAllocated; i++)
        {
            *(int*)(((char**)&self->Riders[0])[i] + 0xEC) = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_reinitMissionRiders);
#ifdef SKIP_ASM
extern "C" void cAI_initMissionRiders(cAI*);

/**
 * Re-runs the mission rider setup followed by the shared AI initialisation.
 *
 * Calls cAI_initMissionRiders() and then cAI_commonInit(self, 0).
 */
extern "C" void cAI_reinitMissionRiders(cAI* self)
{
    cAI_initMissionRiders(self);
    cAI_commonInit(self, 0);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_update);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int cPauseState_Flags;
// PORT: cAI_onlineAICheck is defined below with its own self type; bound by asm label.
extern "C" void cAI_onlineAICheck_AF0(void* self) __asm__("cAI_onlineAICheck");
extern "C" void cAI_AIStateUpdate(void* self);
extern "C" void cAI_UpdateGroupAI(void* self);
extern "C" void cRider_updateFocus(void* rider);
extern "C" void cRider_updateSplitPaused(void* rider);
extern "C" void cRider_updateSplit1(void* rider);
extern "C" void cRider_updateSplit2(void* rider);
extern "C" void cRider_updateSplit3(void* rider);
extern "C" void cRider_updateSplit4(void* rider);
void* cRider_updateSplit5(void* self);
extern "C" void cRider_updateSplit6(void* rider);
extern "C" void cRider_updateSplit7(void* rider);
extern "C" void cRider_updateSplit8(void* rider);
void* cRider_updateSplit9(void* self);
extern "C" void cRider_updateSplit10(void* rider);
extern "C" void cRider_updateSplit11(void* rider);
extern "C" void cRider_updateSplit12(void* rider);
extern "C" void cBoardWakeFX_update(void* p);
extern "C" void cBoardSparkFX_update(void* p);
extern "C" void cBoardTrailFX_update(void* p);
extern "C" void cSpeedBoostFX_update(void* p);
extern "C" void cTrickBoostFX_update(void* p);
extern "C" void cAirStreamerFX_update(void* p);
extern "C" void func_002D4C08(void* p);
extern "C" void cPlayerIndicatorFX_update(void* p);
extern "C" void func_002DF920(void* p);
extern "C" void cPunchFX_update(void* p);
extern "C" void cCheatFX_update(void* p);
extern "C" void cRider_updateSplitMiscFX(void* rider);
extern "C" void cFocusMan_update(void* p);
extern "C" void cDangerMan_Update(void* p);


// Derived rider object -> its cRider virtual base (stored at +0x18).
static inline char* Rider_AF0(char* obj)
{
    return obj ? *(char**)(obj + 0x18) : 0;
}

/**
 * Runs one AI tick: the online check, the state machine, the group AI and every active rider's update passes.
 *
 * Calls cAI_onlineAICheck, cAI_AIStateUpdate() and cAI_UpdateGroupAI (group AI), then gathers the
 * riders: with a mission rider loaded, player 0 and, unless PlaybackMan's mode (+0x70) is 0, the mission
 * rider; otherwise the players, the computers (if ComputerRidersActive) and the actors (if
 * ComputerActorsActive). Each pass (cRider_updateSplit1..12, then the rider handler's FX parts) runs over all
 * of them before the next; while bit 0 of cPauseState_Flags (pause flags) is set only cRider_updateSplitPaused runs.
 * Ends with the FocusMan (cFocusMan_update) and DangerMan (cDangerMan_Update) updates and AITick++.
 */
// Updates every active rider: players, computers and others, in fixed passes.
extern "C" void cAI_update(cAI* self)
{
    char* riders[8];
    char* handlers[8];
    int n = 0;
    int i;
    cAI_onlineAICheck_AF0((self));
    cAI_AIStateUpdate((self));
    cAI_UpdateGroupAI((self));
    if (self->NumberOfMissionRidersLoaded > 0 && self->NumberOfPlayersAllocated > 0) {
        char* x = (char*)SSXApp->Game->PlaybackMan;
        if (x != 0 && *(int*)(x + 0x70) == 0) {
            n = 1;
            riders[0] = Rider_AF0(((char**)&self->Players[0])[0]);
            cRider_updateFocus(Rider_AF0((char*)self->MissionRiders[0]));
        } else {
            char** riderSlot = &riders[n];
            *riderSlot = Rider_AF0(((char**)&self->Players[0])[0]);
            n = 2;
            riders[1] = Rider_AF0((char*)self->MissionRiders[0]);
            cRider_updateFocus(Rider_AF0(((char**)&self->Players[0])[0]));
        }
    } else {
        for (int i = 0; i < self->NumberOfPlayersAllocated; i++) {
            n++;
            riders[n - 1] = Rider_AF0(((char**)&self->Players[0])[i]);
        }
        if (self->ComputerRidersActive)
            for (int i = 0; i < self->NumberOfComputersAllocated; i++) {
                n++;
                riders[n - 1] = Rider_AF0(((char**)&self->Computers[0])[i]);
            }
        if (self->ComputerActorsActive)
            for (int i = 0; i < self->NumberOfActorsAllocated; i++) {
                n++;
                riders[n - 1] = Rider_AF0(((char**)&self->Actors[0])[i]);
            }
    }
    if (cPauseState_Flags & 1) {
        for (i = 0; i < n; i++) cRider_updateSplitPaused(riders[i]);
    } else {
        for (i = 0; i < n; i++) handlers[i] = *(char**)(riders[i] + 0x77C);
        for (i = 0; i < n; i++) cRider_updateSplit1(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit2(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit3(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit4(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit5(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit6(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit7(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit8(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit9(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit10(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit11(riders[i]);
        for (i = 0; i < n; i++) cRider_updateSplit12(riders[i]);
        for (i = 0; i < n; i++) cBoardWakeFX_update(handlers[i] + 0x3B0);
        for (i = 0; i < n; i++) cBoardSparkFX_update(handlers[i] + 0x470);
        for (i = 0; i < n; i++) cBoardTrailFX_update(handlers[i] + 0x520);
        for (i = 0; i < n; i++) cSpeedBoostFX_update(handlers[i] + 0x610);
        for (i = 0; i < n; i++) cTrickBoostFX_update(handlers[i] + 0x9C0);
        for (i = 0; i < n; i++) cAirStreamerFX_update(handlers[i] + 0xAD0);
        for (i = 0; i < n; i++) func_002D4C08(handlers[i] + 0xAF0);
        for (i = 0; i < n; i++) cPlayerIndicatorFX_update(handlers[i] + 0xB00);
        for (i = 0; i < n; i++) func_002DF920(handlers[i] + 0xB40);
        for (i = 0; i < n; i++) cPunchFX_update(handlers[i] + 0xC70);
        for (i = 0; i < n; i++) cCheatFX_update(handlers[i] + 0xD20);
        for (i = 0; i < n; i++) cRider_updateSplitMiscFX(riders[i]);
    }
    cFocusMan_update(self->FocusMan);
    cDangerMan_Update(self->DangerMan);
    self->AITick++;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_updateAnimation);
#ifdef SKIP_ASM
extern "C" void cRider_updateAnimation(void* self, float value);
extern "C" void cRider_updateSkeleton(void* self);
extern "C" void cAnimModel_buildSkelMatrices(void*);

/**
 * Advances every rider's animation by one frame and rebuilds their skeletons.
 *
 * For each of the NumberOfRidersAllocated riders (Riders[]): cRider_updateAnimation(rider, 1.0f), cRider_updateSkeleton (On
 * Tour cRider::updateSkeleton) and cAnimModel_buildSkelMatrices on the rider's anim model (+0x780). Used when an event
 * start loses focus, so the riders are posed before the first rendered frame.
 */
extern "C" void cAI_updateAnimation(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        cRider_updateAnimation(self->mRiders[i], 1.0f);
        cRider_updateSkeleton(self->mRiders[i]);
        cAnimModel_buildSkelMatrices(*(void**)(self->mRiders[i] + 0x780));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_render);
#ifdef SKIP_ASM
extern "C" int cRider_renderClip(void*);
extern "C" void cRider_renderBody(void*);
extern "C" void cRider_renderLights(void*);
extern "C" void cRider_renderShadow(void*);
void func_00122638(void*);
extern "C" void func_002D5048(void*);
extern "C" void cBoardSparkFX_render(void*);
extern "C" void cBoardWakeFX_render(void*);
extern "C" void func_002E24D0(void*);
extern "C" void cPlayerIndicatorFX_render(void*);
extern "C" void cSpeedBoostFX_render(void*);
extern "C" void func_002EA538(void*, int);
extern "C" void cTrickBoostFX_render(void*);
extern "C" void cAirStreamerFX_render(void*);
extern "C" void cPunchFX_render(void*);
void cCheatFX_render(void*);

struct sVEntry_91E0 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sHolder_91E0 {
    char pad[0x18];
    char* rider;
};

struct sAI_91E0 {
    char pad0[0x14];
    int f14;
    char pad18[0x28 - 0x18];
    char* list28[6];
    sHolder_91E0* a40[2];
    sHolder_91E0* a48[5];
    sHolder_91E0* a5C[5];
    sHolder_91E0* h70;
    char pad74[0x78 - 0x74];
    int n78;
    int n7C;
    int n80;
    int n84;
    int f88;
    int f8C;
};

/**
 * Draws every active rider and its per-rider effects, pass by pass across all riders.
 *
 * First gives each allocated rider whose mRenderFlag is 0 and mSafeRenderTick is unset (-1) a safe render tick of
 * GrMan's slot 114 value + 4. Then gathers the riders as cAI_update does: with a mission rider loaded (and a
 * player), only player 0 when PlaybackMan exists with mode (+0x70) 0, else the mission rider; otherwise the
 * players, the computers (if ComputerRidersActive) and the actors (if ComputerActorsActive). Each pass runs over
 * all of them: cRider_renderClip (visibility), then lights, body and shadow for visible riders, the board trails
 * (func_002EA538 on each handler's cBoardTrailFX, +0x520), the player indicator, cheat, board wake, air streamer,
 * speed boost and trick boost FX, then for visible riders board sparks, cAngerFX (func_002D5048), cDynamicSprayFX
 * (func_002E24D0) and punch FX, and finally func_00122638 per rider.
 */
extern "C" void cAI_render(sAI_91E0* self)
{
    char* riders[8];
    char* sub[8];
    int active[8];
    char* ptrs[16];
    int n;
    int i;

    for (n = 0; n < self->n78; n++) {
        char* curRider = self->list28[n];
        if (*(int*)(curRider + 0x884) == 0 && *(int*)(curRider + 0x888) == -1) {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sVEntry_91E0* vt = (sVEntry_91E0*)ps2GraphicsMan->mVtbl;
            int frameTick = vt[114].fn((char*)ps2GraphicsMan + vt[114].delta);
            *(int*)(self->list28[n] + 0x888) = frameTick + 4;
        }
    }
    n = 0;
    if (self->f88 > 0 && self->n7C > 0) {
        char* x = (char*)SSXApp->Game->PlaybackMan;
        if (x != 0 && *(int*)(x + 0x70) == 0) {
            riders[n++] = self->a40[0] ? self->a40[0]->rider : 0;
        } else {
            riders[n++] = self->h70 ? self->h70->rider : 0;
        }
    } else {
        for (i = 0; i < self->n7C; i++) {
            riders[n++] = self->a40[i] ? self->a40[i]->rider : 0;
        }
        if (self->f14 != 0) {
            for (i = 0; i < self->n80; i++) {
                riders[n++] = self->a48[i] ? self->a48[i]->rider : 0;
            }
        }
        if (self->f8C != 0) {
            for (i = 0; i < self->n84; i++) {
                riders[n++] = self->a5C[i] ? self->a5C[i]->rider : 0;
            }
        }
    }
    for (i = 0; i < n; i++) {
        active[i] = cRider_renderClip(riders[i]);
        sub[i] = *(char**)(riders[i] + 0x77C);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) cRider_renderLights(riders[i]);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) cRider_renderBody(riders[i]);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) cRider_renderShadow(riders[i]);
    }
    for (i = 0; i < n; i++) {
        ptrs[i] = sub[i] ? sub[i] + 0x520 : 0;
    }
    func_002EA538(ptrs, n);
    for (i = 0; i < n; i++) cPlayerIndicatorFX_render(sub[i] + 0xB00);
    for (i = 0; i < n; i++) cCheatFX_render(sub[i] + 0xD20);
    for (i = 0; i < n; i++) cBoardWakeFX_render(sub[i] + 0x3B0);
    for (i = 0; i < n; i++) cAirStreamerFX_render(sub[i] + 0xAD0);
    for (i = 0; i < n; i++) cSpeedBoostFX_render(sub[i] + 0x610);
    for (i = 0; i < n; i++) cTrickBoostFX_render(sub[i] + 0x9C0);
    for (i = 0; i < n; i++) {
        if (active[i]) cBoardSparkFX_render(sub[i] + 0x470);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) func_002D5048(sub[i] + 0xAF0);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) func_002E24D0(sub[i] + 0xB40);
    }
    for (i = 0; i < n; i++) {
        if (active[i]) cPunchFX_render(sub[i] + 0xC70);
    }
    for (i = 0; i < n; i++) func_00122638(riders[i]);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_loadRiders);
#ifdef SKIP_ASM
extern "C" int cRider_isLoaded(void* self);
extern "C" void cRider_loadRider(void* self);

/**
 * Loads the next rider that is not loaded yet; at most one rider per call.
 *
 * Walks Riders[0 .. NumberOfRidersAllocated) and calls cRider_loadRider() on the first one for which
 * cRider_isLoaded() is 0, then stops.
 */
extern "C" void cAI_loadRiders(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        if (cRider_isLoaded(self->mRiders[i]) == 0)
        {
            cRider_loadRider(self->mRiders[i]);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_restart);
#ifdef SKIP_ASM
extern "C" void cFocusMan_reset(void*);
extern "C" void cDangerMan_Reset(void*);
extern "C" void cAI_setAIState(void*, int);

/**
 * Restarts the AI in a given state: resets the focus and danger managers, the AI state and the riders.
 *
 * Clears CurrentAIState and its handler first, so cAI_setAIState() installs @p state with no previous state;
 * then cAI_commonInit(self, 1) resets every rider, humans included, and the event timer.
 *
 * @param state  the AI state to start in (tAIState: 1 GameInit .. 5 Race, 6 EndRace, 7 Shutdown)
 */
extern "C" void cAI_restart(cAI* self, int state)
{
    cFocusMan_reset(self->FocusMan);
    cDangerMan_Reset(self->DangerMan);
    self->CurrentAIStateHandler = 0;
    self->CurrentAIState = 0;
    cAI_setAIState(self, state);
    cAI_commonInit(self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_commonInit);
#ifdef SKIP_ASM
void* cAI_InitGroupAI(void* self);
extern "C" void func_00154A58(int rider, int a);
extern "C" void cRider_initAlways(void* rider);

struct sVEntry1297C8 { short delta; short index; int (*fn)(void*); };

struct sAiObj1297C8
{
    char pad0[0x8];
    int f8;             // 0x08
    int fC;             // 0x0C
    char pad10[0x8];
    int f18;            // 0x18
    int f1C;            // 0x1C
    char pad20[0x8];
    char* mRiders[18];  // 0x28
    char* mList[2];     // 0x70
    int mCount;         // 0x78
    char pad7C[0xC];
    int mCount2;        // 0x88
};

/**
 * Shared (re)initialisation for a new run: resets the group AI records, the tick and timers, and the riders.
 *
 * Calls cAI_InitGroupAI (cAI_RefreshGroupAI), zeroes AITick and CountdownTicksLeft, sets EndOfRaceTimer to -1
 * and zeroes EventTimerTick only when @p flag is set. Each computer rider (cRider vtable slot 9), or every
 * rider when @p flag is set, has its stats (metrix + 0xFC) recorded by func_00154A58 and is reset
 * (cRider_initAlways); the mission riders are always reset.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 * @param flag  On Tour's shouldInitHuman: also reset the human riders and the event timer
 * @return nothing meaningful: the body has no return statement (On Tour's commonInit returns void)
 */
extern "C" void* cAI_commonInit(cAI* aiManager, int flag)
{
    sAiObj1297C8* self = (sAiObj1297C8*)aiManager;
    cAI_InitGroupAI(self);
    self->f8 = 0;
    if (flag != 0)
    {
        self->fC = 0;
    }
    self->f18 = -1;
    self->f1C = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* obj = self->mRiders[i] + 0x6C0;
        sVEntry1297C8* vt = *(sVEntry1297C8**)obj;
        if (vt[9].fn(obj + vt[9].delta) != 0 || flag != 0)
        {
            func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
            cRider_initAlways(self->mRiders[i]);
        }
    }
    for (int i = 0; i < self->mCount2; i++)
    {
        cRider_initAlways(*(void**)(self->mList[i] + 0x18));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", AITick);
#ifdef SKIP_ASM

/**
 * Returns the AI's frame counter, the game-wide tick the riders time their events with.
 *
 * Reads SSXApp->Game->AI->AITick (cAI +0x08), which cAI_commonInit zeroes at the start of every run.
 *
 * @return the current AITick
 */
extern "C" int AITick()
{
    return SSXApp->Game->AI->AITick;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_InitPlayers);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* PadCache;
extern char str_Player[];
extern "C" int cBENewPlayerInterface_getRiderCtrlID(void* iface, int i);
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int i);
extern "C" int cInputSync_createlocalinput(void* a, void* b);
extern "C" int cInputSync_createremoteinput(void* a, void* b);
extern "C" void cAI_applyPlayerRulesCustom(cAI* self, cPlayer* obj, int i);
// PORT: cPlayer_cPlayer returns self here (declared void elsewhere in this unit); bound by asm label.
extern "C" char* cPlayer_cPlayer_IP(void* self, int inChrg) __asm__("cPlayer_cPlayer");
extern "C" void cRider_addFocusBox(void* self);
extern "C" void cRider_initOnce(void* self, int immediate);
struct cRider;
void cRider_addRiderMetrix(cRider* rider);
void cBENewRaceInterface_assignPlayer(void*, int, int);

struct sRiderIP { char pad[0x86C]; int mIndex; int mCtrl; char pad874[8]; int f87C; };
class cBEIfaceIP {
public:
    int pad[3];
    virtual void update();
};

struct sAiObjIP
{
    char pad0[0x28];
    sRiderIP* mRiders[6];
    char* mPlayers[2];
    char pad48[0x30];
    int mCount;
    int mNumPlayers;
};

static inline void SetHud_IP(char* obj, int a, int b)
{
    *(int*)(obj + 0xDF0) = a;
    *(int*)(obj + 0xDF4) = b;
}

static inline int HudTab_IP(int ch)
{
    int* h = (int*)PadCache;
    h += ch;
    return h[0xBBB];
}

static inline char* Cfg_IP() { return ((char*)&cBENewRaceInterface_newRace); }

// PORT: cMemMan_alloc with unsigned size / int flags (the argument order retail sets up); bound by asm label.
extern "C" void* cMemMan_alloc_IP(unsigned int size, const char* tag, int flags, int d) __asm__("cMemMan_alloc");

/**
 * Creates the human players' riders from slot 0 and connects their input, pad, focus box and rules.
 *
 * Per human (newRace.iNumberHumans) allocates a "Player" (0x1950): Players[i] = it, Riders[i] = its cRider
 * with mAIIndex = mViewIndex = i. Its Input is SSXApp->Inputs[c] (c = the player's controller ID), bound to
 * the input map that cBENewPlayerInterface_getControllerMapping names, and its Pad the PadCache pad c. Online the remote player (host: 1,
 * guest: 0) gets a remote InputSync input (cInputSync_createremoteinput), no pad, mIsLocal = 0 and finally mViewIndex -2;
 * the local one a synced input. Also applies the custom rules, cRider_initOnce() and assignPlayer(i, i); both
 * counts become the player count.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
// Creates the human players' riders and hooks them up to their input and HUD slots.
extern "C" void cAI_InitPlayers(cAI* aiManager)
{
    sAiObjIP* self = (sAiObjIP*)aiManager;
    for (int k = 1; k >= 0; k--)
        self->mPlayers[k] = 0;
    int idx = 0;
    char* rm = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    int i = 0;
    int nPlayers = 0;
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    int n = *(int*)(Cfg_IP() + 0x34);
    for (; i < n; i++) {
        int ch = cBENewPlayerInterface_getRiderCtrlID(iface, i);
        cPlayer* obj = (cPlayer*)cPlayer_cPlayer_IP(cMemMan_alloc_IP(6480, str_Player, 0x80000000, 0), 1);
        self->mRiders[i] = ((char*)obj) ? (sRiderIP*)obj->mRider : 0;
        self->mRiders[i]->mIndex = idx;
        self->mRiders[i]->mCtrl = idx;
        int ctrl = cBENewPlayerInterface_getControllerMapping(iface, idx);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
            if ((cBENetworkInterface_networkOpt.bIsHost != 0 && i != 1) || (cBENetworkInterface_networkOpt.bIsHost == 0 && i != 0)) {
                cSSXApp* app = SSXApp;
                char** box = (char**)(&app->Inputs[0]);
                box += ch;
                int* inputBase = (int*)app;
                inputBase += ctrl;
                *(int*)(*box + 4) = inputBase[46];
                SetHud_IP(((char*)obj), cInputSync_createlocalinput(app->Game->InputSync, *box), HudTab_IP(ch));
            } else {
                cSSXApp* app = SSXApp;
                ((sRiderIP*)obj->mRider)->f87C = 0;
                int* inputBase = (int*)app;
                inputBase += ctrl;
                SetHud_IP(((char*)obj), cInputSync_createremoteinput(app->Game->InputSync, (void*)inputBase[46]), 0);
            }
        } else {
            cSSXApp* app = SSXApp;
            char** box = (char**)(&app->Inputs[0]);
            int* hud = (int*)PadCache;
            int* inputBase = (int*)app;
            box += ch;
            inputBase += ctrl;
            hud += ch;
            *(int*)(*box + 4) = inputBase[46];
            SetHud_IP(((char*)obj), (int)*box, hud[3003]);
            self->mRiders[i]->mCtrl = idx;
        }
        cRider_addFocusBox(obj->mRider);
        nPlayers++;
        cRider_addRiderMetrix(obj->mRider);
        cAI_applyPlayerRulesCustom((cAI*)self, (cPlayer*)obj, i);
        *(int*)&obj->mRider->unk_2F8 = 0;
        cRider_initOnce(self->mRiders[i], 0);
        cBENewRaceInterface_assignPlayer(rm, idx, idx);
        idx++;
        self->mPlayers[i] = ((char*)obj);
    }
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        if (cBENetworkInterface_networkOpt.bIsHost != 0) {
            (*(sRiderIP**)(self->mPlayers[0] + 0x18))->mCtrl = 0;
            (*(sRiderIP**)(self->mPlayers[1] + 0x18))->mCtrl = -2;
        } else {
            (*(sRiderIP**)(self->mPlayers[0] + 0x18))->mCtrl = -2;
            (*(sRiderIP**)(self->mPlayers[1] + 0x18))->mCtrl = 0;
        }
    }
    ((cBEIfaceIP*)rm)->update();
    self->mCount = idx;
    self->mNumPlayers = nPlayers;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initMissionRiders);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cRider_cRider(void* self);
extern "C" void cPlayer_cPlayer(void* self, int inChrg);
extern "C" void cAnimModel_setScale(void* self, float a, float b);
extern "C" void cBEStatInterface_setStatLevel(void* self, int rider, int value);
extern "C" void cRider_addFocusBox(void* self);
extern "C" void cRider_initOnce(void* self, int immediate);
struct cRider;
void cRider_addRiderMetrix(cRider* rider);
void cBENewRaceInterface_assignPlayer(void*, int, int);
extern char str_MissionRider[];

struct sVEntry_MR { short delta; short index; void* fn; };
struct sVtbl9_MR { sVEntry_MR e[9]; } __attribute__((aligned(8)));
struct sVtbl22_MR { sVEntry_MR e[22]; } __attribute__((aligned(8)));

extern const sVtbl9_MR D_00458240;
extern char D_00459B90[];
extern const sVtbl22_MR D_00458288;
extern char cPlayer_vtbl[];

// PORT: g++ 2.95 most-derived constructor of a class deriving virtually from cRider (vbase at +0xE10,
// vbase pointers in every subobject), inlined here; the not-in-charge vtable fix-up is written out by hand.
static inline char* cMissionRider_ctor(char* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = self + 0xE10;
        *(char**)(self + 0xD20) = vb;
        *(char**)(self + 0xC70) = vb;
        *(char**)(self + 0xB40) = vb;
        *(char**)(self + 0xB00) = vb;
        *(char**)(self + 0xAF0) = vb;
        *(char**)(self + 0xAD0) = vb;
        *(char**)(self + 0x9C0) = vb;
        *(char**)(self + 0x610) = vb;
        *(char**)(self + 0x520) = vb;
        *(char**)(self + 0x470) = vb;
        *(char**)(self + 0x3B0) = vb;
        *(char**)(self + 0x3A0) = vb;
        *(char**)(self + 0x398) = vb;
        *(char**)(self + 0x384) = vb;
        *(char**)(self + 0x364) = vb;
        *(char**)(self + 0x358) = vb;
        *(char**)(self + 0x340) = vb;
        *(char**)(self + 0x2B8) = vb;
        *(char**)(self + 0x2A4) = vb;
        *(char**)(self + 0x288) = vb;
        *(char**)(self + 0x224) = vb;
        *(char**)(self + 0x200) = vb;
        *(char**)(self + 0x1F0) = vb;
        *(char**)(self + 0x1E4) = vb;
        *(char**)(self + 0x1C0) = vb;
        *(char**)(self + 0x1B0) = vb;
        *(char**)(self + 0x100) = vb;
        *(char**)(self + 0xA0) = vb;
        *(char**)(self + 0x70) = vb;
        *(char**)(self + 0x24) = vb;
        *(char**)(self + 0x18) = vb;
        cRider_cRider(vb);
    }
    cPlayer_cPlayer(self, 0);
    *(const void**)(*(char**)(self + 0x18) + 0x6E8) = &D_00458240;
    *(void**)(*(char**)(self + 0x18) + 0x6D0) = D_00459B90;
    *(const void**)(*(char**)(self + 0x18) + 0x6C0) = &D_00458288;
    *(void**)(self + 0xDE8) = cPlayer_vtbl;
    if (!inChrg)
    {
        sVtbl9_MR t1 = D_00458240;
        *(void**)(*(char**)(self + 0x18) + 0x6E8) = &t1;
        char* base = *(char**)(self + 0x18) - 0xE10;
        int d = self - base;
        t1.e[1].delta = D_00458240.e[1].delta + d;
        sVtbl22_MR t2 = D_00458288;
        *(void**)(*(char**)(self + 0x18) + 0x6C0) = &t2;
        t2.e[1].delta = D_00458288.e[1].delta + d;
    }
    return self;
}

struct sRiderMR { char pad[0x86C]; int mIndex; int mCtrl; };

struct sAiObjMR
{
    char pad0[0x28];
    sRiderMR* mRiders[18];
    char* mMission[2];
    int mCount;
    char pad7C[0xC];
    int mNumMission;
};

/**
 * Creates the mission rider (at most one) after the riders already allocated, driven by the playback input.
 *
 * Does nothing when a mission rider is loaded. Builds a cMissionRider ("MissionRider", 0x1950, constructor
 * inlined) with Input = PlaybackMan +0x6C and no pad, scales its anim model by 0.5, sets its seven stats to
 * 25 (cBEStatInterface_setStatLevel(.., 5)), gives it the next slot (mAIIndex; mViewIndex 0), a focus box and metrics, and
 * adds one to NumberOfRidersAllocated and NumberOfMissionRidersLoaded.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_initMissionRiders(cAI* aiManager)
{
    sAiObjMR* self = (sAiObjMR*)aiManager;
    if (self->mNumMission > 0)
        return;
    char* rm = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    char* playbackMan = (char*)SSXApp->Game->PlaybackMan;
    int idx = self->mCount;
    char* obj = cMissionRider_ctor((char*)cMemMan_alloc(6480, str_MissionRider, 0x80000000, 0), 1);
    *(int*)(obj + 0xDF0) = *(int*)(playbackMan + 0x6C);
    *(int*)(obj + 0xDF4) = 0;
    cAnimModel_setScale(*(void**)(*(char**)(obj + 0x18) + 0x780), 0.5f, 0.5f);
    cBEStatInterface_setStatLevel(cBE_getInterface_Fv(cBE_getBE(), 3), idx, 5);
    self->mRiders[idx] = obj ? *(sRiderMR**)(obj + 0x18) : 0;
    self->mRiders[idx]->mIndex = idx;
    self->mRiders[idx]->mCtrl = 0;
    cRider_addFocusBox(*(void**)(obj + 0x18));
    cRider_addRiderMetrix(*(cRider**)(obj + 0x18));
    cRider_initOnce(self->mRiders[idx], 0);
    cBENewRaceInterface_assignPlayer(rm, idx, idx);
    idx++;
    self->mMission[self->mNumMission] = obj;
    self->mCount = idx;
    self->mNumMission++;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initComputerRiders);
#ifdef SKIP_ASM
extern "C" void* cComputer_cComputer(void* mem, int a, int b);
extern "C" void cBENewPlayerInterface_setRiderCtrlID(void* iface, int rider, int ctrl);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* iface, int rider, int ch);
int cBENewPlayerInterface_getRiderCharID(void* iface, int rider);
struct cRider;
void cRider_addRiderMetrix(cRider* rider);
extern "C" void cComputer_setAttributeLevels(void*);
extern "C" void cComputer_updateRiderDifficulty(void*);
extern "C" void cAI_applyComputerRulesCustom(cAI* self, cComputer* rider);
void cBENewRaceInterface_assignPlayer(void*, int, int);
extern char str_ComputerRider[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sRiderCR { char pad[0x86C]; int mIndex; int mCtrl; };
struct sVEntryCR { short delta; short index; void (*fn)(void*); };
class cBEIfaceCR {
public:
    int pad[3];
    virtual void update();
};

struct sAiObjCR
{
    char pad0[0x10];
    int f10;
    char pad14[0x14];
    sRiderCR* mRiders[8];
    char* mObjs[12];
    int mCount;
    char pad7C[4];
    int mNumComputers;
};

/**
 * Creates the computer riders still missing up to newRace.iNumberAI and marks the AI ready to race.
 *
 * Each takes the next rider slot: a "ComputerRider" cComputer (0x1A90, not an actor) whose cRider gets
 * mAIIndex = slot and mViewIndex -1; the slot keeps its character with controller -1. Then metrics, a stat
 * level (cComputer_setAttributeLevels), its difficulty, the custom computer rules and assignPlayer(slot, slot). Finally
 * updates NumberOfRidersAllocated and sets IsAIReadyToRace to 1.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_initComputerRiders(cAI* aiManager)
{
    sAiObjCR* self = (sAiObjCR*)aiManager;
    char* rm = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    int n = cBENewRaceInterface_newRace.iNumberAI - self->mNumComputers;
    int idx = self->mCount;
    for (int i = 0; i < n; i++)
    {
        int ch = cBENewPlayerInterface_getRiderCharID(iface, idx);
        cComputer* obj = (cComputer*)((char*)cComputer_cComputer(cMemMan_alloc(6800, str_ComputerRider, 0x80000000, 0), 1, 0));
        sRiderCR** slot = &self->mRiders[idx];
        sRiderCR* rider = ((char*)obj) ? (sRiderCR*)obj->RiderHandler.GroundMotion.mRider : 0;
        *slot = rider;
        rider->mIndex = idx;
        cBENewPlayerInterface_setRiderCtrlID(iface, idx, -1);
        cBENewPlayerInterface_setRiderCharID(iface, idx, ch);
        ((cBEIfaceCR*)iface)->update();
        sRiderCR* slotRider = *slot;
        int noController = -1;
        slotRider->mCtrl = noController;
        cRider_addRiderMetrix(obj->RiderHandler.GroundMotion.mRider);
        cComputer_setAttributeLevels(obj);
        cComputer_updateRiderDifficulty(obj);
        cAI_applyComputerRulesCustom((cAI*)self, (cComputer*)obj);
        cBENewRaceInterface_assignPlayer(rm, idx, idx);
        self->mObjs[self->mNumComputers] = ((char*)obj);
        self->mNumComputers++;
        idx++;
    }
    ((cBEIfaceCR*)rm)->update();
    ((cBEIfaceCR*)iface)->update();
    self->mCount = idx;
    self->f10 = 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_initComputerActors);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cComputer_cComputer(void* mem, int a, int b);
extern "C" void cBENewPlayerInterface_setRiderCtrlID(void* iface, int rider, int ctrl);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* iface, int rider, int ch);
extern "C" void cBENewPlayerInterface_setRiderCheatCharID(void* iface, int rider, int v);
struct cRider;
void cRider_addRiderMetrix(cRider* rider);
extern char str_ComputerActorRider[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sRiderCA { char pad[0x86C]; int mIndex; int mCtrl; };
struct sVEntryCA { short delta; short index; void (*fn)(void*); };

struct sAiObj129FF8
{
    char pad0[0x20];
    int mCharIds[2];
    sRiderCA* mRiders[13];
    char* mObjs[7];
    int mCount;
    char pad7C[8];
    int mNumActors;
    char pad88[0xC];
    int mMaxActors;
};

/**
 * Creates the computer actors (podium riders) still missing up to NumberOfActorsRequired.
 *
 * Actor k uses the character PodiumActorList[k] and the next rider slot: a "ComputerActorRider" cComputer
 * (0x1A90, actor) with mAIIndex = slot, mViewIndex -1, controller -1, player-record byte 0x12 = 0
 * (cBENewPlayerInterface_setRiderCheatCharID) and metrics. PodiumActorList has 2 entries, so a third actor would read Riders[0]. Updates
 * NumberOfRidersAllocated.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_initComputerActors(cAI* aiManager)
{
    sAiObj129FF8* self = (sAiObj129FF8*)aiManager;
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    int n = self->mMaxActors - self->mNumActors;
    int idx = self->mCount;
    for (int i = 0; i < n; i++)
    {
        int ch = self->mCharIds[self->mNumActors];
        char* obj = (char*)cComputer_cComputer(cMemMan_alloc(6800, str_ComputerActorRider, 0x80000000, 0), 1, 1);
        sRiderCA* rider = obj ? *(sRiderCA**)(obj + 0x18) : 0;
        self->mRiders[idx] = rider;
        rider->mIndex = idx;
        cBENewPlayerInterface_setRiderCtrlID(iface, idx, -1);
        cBENewPlayerInterface_setRiderCharID(iface, idx, ch);
        cBENewPlayerInterface_setRiderCheatCharID(iface, idx, 0);
        sVEntryCA* vt = *(sVEntryCA**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        sRiderCA* slotRider = self->mRiders[idx];
        int noController = -1;
        slotRider->mCtrl = noController;
        cRider_addRiderMetrix(*(cRider**)(obj + 0x18));
        self->mObjs[self->mNumActors] = obj;
        self->mNumActors++;
        idx++;
    }
    sVEntryCA* vt = *(sVEntryCA**)(iface + 0xC);
    vt[1].fn(iface + vt[1].delta);
    self->mCount = idx;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_isAILoadedAndReady);
#ifdef SKIP_ASM
extern "C" int cRider_isLoaded(void* self);

/**
 * Tells whether every allocated rider has finished loading.
 *
 * Checks cRider_isLoaded() on all of Riders[0 .. NumberOfRidersAllocated) without stopping early.
 *
 * @return 1 when all riders are loaded (or there are none), otherwise 0
 */
extern "C" int cAI_isAILoadedAndReady(cAI* self)
{
    int result = 1;
    for (int i = 0; i < self->NumberOfRidersAllocated; i++)
    {
        if (cRider_isLoaded(((char**)&self->Riders[0])[i]) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012A1F0);
#ifdef SKIP_ASM
extern int cGameModeMan_iCharRank[];

/**
 * Returns whether rider @p idx's character is one of the first six entries of cGameModeMan_iCharRank.
 *
 * Reads SSXApp->GameModeMan->iCharacters[idx] and compares it with cGameModeMan_iCharRank[0..5] (one of cGameModeMan's sorted score
 * / rank tables, fe/ovstatepause uses it as a list of rider indices). Used by func_0012A998 and func_0012AA78 with each
 * computer rider's getAIIndex.
 *
 * @param idx  the rider's AI index
 * @return 1 when found, else 0
 */
extern "C" int func_0012A1F0(cAI* self, int idx)
{
    char* tbl = (char*)SSXApp->GameModeMan;
    int characterId = *(int*)((char*)tbl + (idx << 2) + 0x18);
    int found = 0;
    for (int i = 0; i < 6; i++) {
        if (characterId == cGameModeMan_iCharRank[i]) found = 1;
    }
    return found;
}
#endif
//100%
INCLUDE_ASM("ai/ai", cAI_hasRaceStarted);
#ifdef SKIP_ASM
/**
 * Returns whether the AI's CurrentAIState (the first word of @p aiMan) is 5, On Tour's AIS_Race.
 *
 * Nothing in the source calls it.
 *
 * @param aiMan  the cAI
 * @return 1 while racing, else 0
 */
extern "C" int cAI_hasRaceStarted(int *aiMan) {
    return *aiMan == 5;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_isRaceFinished);
#ifdef SKIP_ASM
struct sAiObj12A250
{
    char pad0[0x40];
    char* mRiders[15];
    int mCount;
};

static inline bool aiRiderOk12A250(char* rider)
{
    return *(float*)(*(char**)(rider + 0x18) + 0x470) >= 0.0f;
}

/**
 * Tells whether every human player has crossed the finish line.
 *
 * A player counts as finished once its rider's mFinishTime is >= 0; computer riders are not checked.
 *
 * @return 1 when all players have finished (or there are none), otherwise 0
 */
extern "C" int cAI_isRaceFinished(cAI* self)
{
    for (int i = 0; i < self->NumberOfPlayersAllocated; i++)
    {
        bool ok = aiRiderOk12A250(((char**)&self->Players[0])[i]);
        if (!ok)
        {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_swaploadPathFile);
#ifdef SKIP_ASM
extern "C" void cRider_initPath(void* rider, int v);
extern "C" void cPathSys_purgePaths(void* self);
extern "C" void cPathSys_loadPaths(void*, int);
extern char gPaths[];
/**
 * Swaps in a different AI path file: purges the loaded paths, loads the new file and re-initialises every rider's
 * path.
 *
 * Calls cPathSys_purgePaths (On Tour cPathSys::purgePaths) and cPathSys_loadPaths on PathSys (gPaths) with the file, then
 * cRider_initPath (cRider::initPath(false)) on each of the NumberOfRidersAllocated riders (+0x28 table).
 *
 * @param filename  the path file name, passed as an int (On Tour const char* filename)
 */
extern "C" void cAI_swaploadPathFile(cAI* self, int filename) {
    int i;
    cPathSys_purgePaths(gPaths);
    cPathSys_loadPaths(gPaths, filename);
    for (i = 0; i < self->NumberOfRidersAllocated; i++) {
        cRider_initPath(*(void**)((char*)&self->Riders[0] + (i << 2)), 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_swaploadInjectedPathFile);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self);
extern char gPaths[];
extern "C" void cPathSys_purgePaths(void* self);
extern "C" int cPathSys_loadInjectedPaths(void* self, int v);
extern "C" void cRider_initPath(void* rider, int v);

struct sAiObj12A340
{
    char pad0[0x28];
    char* mRiders[19];
    int mMode;
    int mCount;
};

/**
 * Accepts or refuses an injected AI path file and, when accepted, swaps it in as the course path data.
 *
 * In BE sections 0x11-0x15, game mode 5 or game event 0xB takes only UID 1 and game mode 6 only UID 2; every
 * other case refuses UIDs 1 and 2, and sections 0x16 and up refuse all files. On acceptance frees the old
 * data of the path object gPaths, loads the file into it and re-attaches each rider's path state
 * (cRider_initPath) if there is an AI.
 *
 * @param uiSectionID  the section ID (On Tour uiSectionID); not read, the section comes from Game->SectionMan
 * @param mode  the file's UID (On Tour uiUID); 1 and 2 are the game-mode-specific variants
 * @param data  the path file data (On Tour data)
 * @return 1 when the file was loaded, 0 when it was refused
 */
extern "C" int cAI_swaploadInjectedPathFile(cAI* self, int uiSectionID, int mode, int data)
{
    int lvl = cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if ((unsigned)(lvl - 17) < 5)
    {
        signed char* newRace = (signed char*)((char*)&cBENewRaceInterface_newRace);
        if (newRace[0x48] == 5 || newRace[0x4A] == 11)
        {
            if (mode != 1)
                return 0;
            goto ok;
        }
        if (newRace[0x48] == 6)
        {
            if (mode != 2)
                return 0;
            goto ok;
        }
    }
    if (mode == 1 || mode == 2)
        return 0;
ok:
    if (lvl < 22)
    {
        cPathSys_purgePaths(gPaths);
        cPathSys_loadInjectedPaths(gPaths, data);
        if ((void*)SSXApp->Game->AI == 0)
            return 1;
        for (int i = 0; i < self->NumberOfRidersAllocated; i++)
        {
            cRider_initPath(((char**)&self->Riders[0])[i], 0);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_removeInjectedFile);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self);
void cPathSys_removeInjectedPathFile(void* self);
extern char gPaths[];

/**
 * Drops the AI path data injected with a world chunk when that chunk is unloaded.
 *
 * Callers pass (AI, resourceId); the section id reaches cSectionMan_getBESectionIDFromWorldSectionID untouched in a1
 * (the C declaration has no parameters). When the BE section id is below 22 it calls cPathSys_removeInjectedPathFile on gPaths, which has
 * an empty body in retail, so the call does nothing. Called by cPS2ChunkResolve_unresolve for resource type 14; the
 * load side is cAI_swaploadInjectedPathFile.
 */
extern "C" void cAI_removeInjectedFile(void)
{
    if (cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan) < 22)
    {
        cPathSys_removeInjectedPathFile(gPaths);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012A4D0);
#ifdef SKIP_ASM
extern "C" void cPathSys_isPathfileLoaded(int *);

/**
 * Returns whether the AI path file is loaded: gPaths' loaded flag (+0x4C, via cPathSys_isPathfileLoaded).
 *
 * The C body discards the value, but cPathSys_isPathfileLoaded's result is left in v0, so callers get the flag. The flag is set to
 * 1 by cPathSys_loadPaths when the path pack loads. Nothing in the source calls it.
 */
extern "C" void func_0012A4D0(void) {
    cPathSys_isPathfileLoaded((int*)&gPaths);
}
#endif
//100%
INCLUDE_ASM("ai/ai", func_0012A4F0);
#ifdef SKIP_ASM
/**
 * Re-initialises every allocated rider's path: cRider_initPath(Riders[i], 0) for i below NumberOfRidersAllocated.
 *
 * Nothing in the source calls it.
 */
extern "C" void func_0012A4F0(cAI* self) {
    int i;
    for (i = 0; i < self->NumberOfRidersAllocated; i++) {
        cRider_initPath(*(void**)((char*)&self->Riders[0] + (i << 2)), 0);
    }
}
#endif
//100%
INCLUDE_ASM("ai/ai", func_0012A558);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty body). Nothing in the source calls it.
 */
extern "C" void func_0012A558(void) {

}
#endif
//100%
INCLUDE_ASM("ai/ai", func_0012A560);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getTrackPath(void* tree, int id);
extern "C" float cTrackPath_getFinishLineParam(void* node);
t4Vector cPath_getPathPos_A560(void* path, int* inRange, float t) __asm__("cPath_getPathPos");
void cRider_putAt_A560(cRider* self, t4Vector* pos, t4Vector* dir, int n, float t) __asm__("cRider_putAt");
// PORT: prototype mismatch. cRider_setControlState/cRider_setMotionState are defined with one param, but callers pass a
// mode in $a1.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_A560(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_A560(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void vDivEq_A560(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

static inline int NodeCount_A560(char* tree)
{
    return *(int*)(tree + 0x10);
}

static inline t4Vector Front_A560(cRider* rider)
{
    return rider->mMatrix.row[1];
}

/**
 * Puts all riders, or one, back at the start of the first AI path that has a marker distance, facing along the path.
 *
 * Walks gPaths' nodes (count at gPaths + 0x10, cPathSys_getTrackPath) to the first whose cTrackPath_getFinishLineParam distance is not -1. The
 * position is the path point 1000 units before that distance (at least 0), the facing is the normalised vector from
 * there to the point at the distance (the rider's current front row when that is shorter than 1). Each chosen rider
 * (all when @p which is -1) gets cRider_setControlState and cRider_setMotionState with 0 and is placed with
 * cRider_putAt (mode 5). Only the first such node is used. Nothing in the source calls it.
 *
 * @param which  the rider index to move, or -1 for all riders
 */
// Puts the riders (all, or only rider `which`) back on the first path node that has a distance, facing along the path.
extern "C" void func_0012A560(sAiObj12A340* self, int which)
{
    int n = NodeCount_A560(gPaths);
    for (int i = 0; i < n; i++) {
        void* node = cPathSys_getTrackPath(gPaths, i);
        float markerDist = cTrackPath_getFinishLineParam(node);
        if (markerDist != -1.0f) {
            float back = markerDist - 1000.0f;
            if (back < 0.0f)
                back = 0.0f;
            t4Vector ahead = cPath_getPathPos_A560(node, 0, markerDist);
            t4Vector pos = cPath_getPathPos_A560(node, 0, back);
            for (int j = 0; j < self->mCount; j++) {
                if (which == -1 || which == j) {
                    cRider_setControlState_impl(self->mRiders[j], 0);
                    cRider_setMotionState_impl(self->mRiders[j], 0);
                    t4Vector dir = vSub_A560(ahead, pos);
                    float len = vLen_A560(dir);
                    if (len > 1.0f)
                        vDivEq_A560(dir, len);
                    else
                        dir = Front_A560((cRider*)self->mRiders[j]);
                    cRider_putAt_A560((cRider*)self->mRiders[j], &pos, &dir, 5, 0.0f);
                }
            }
            return;
        }
    }
}
#endif
//100%
INCLUDE_ASM("ai/ai", func_0012A768);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getTrackPath(void* tree, int id);
extern "C" float cTrackPath_getFinishLineParam(void* node);
// NOTE: float before the flags pointer. EABI numbers int and float args separately, so this is the same ABI as
// the (self, int*, float) definition; this caller's arg-load order needs it (every other landed user is order-neutral).
t4Vector cPath_getPathPos_A768(void* path, float t, int* inRange) __asm__("cPath_getPathPos");
void cRider_putAt_A768(cRider* self, t4Vector* pos, t4Vector* dir, int n, float t) __asm__("cRider_putAt");
// PORT: prototype mismatch. cRider_setControlState/cRider_setMotionState are defined with one param, but callers pass a
// mode in $a1.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_A768(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_A768(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void vDivEq_A768(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

static inline int NodeCount_A768(char* tree)
{
    return *(int*)(tree + 0x10);
}

static inline t4Vector Front_A768(cRider* rider)
{
    return rider->mMatrix.row[1];
}

/**
 * Lines the first @p n riders up behind the first AI path node that has a marker distance, 1000 units apart, with
 * staggered time penalties.
 *
 * Finds the node like func_0012A560. Rider j (while riders remain) is placed with mode 5, after cRider_setControlState
 * / cRider_setMotionState with 0, at markerDist - k * 1000 (at least 0) facing along the path, with mPenaltyTicks = k *
 * 3600 (one minute per place at 60 ticks/s), where k counts down from @p n. Rider 0 is placed furthest back and gets
 * the largest penalty. Nothing in the source calls it.
 *
 * @param n  how many riders to place
 */
// Puts the first `n` riders back on the first path node that has a distance, staggered 1000 units apart,
// each with a penalty of 3600 ticks per place.
extern "C" void func_0012A768(sAiObj12A340* self, int n)
{
    int count = NodeCount_A768(gPaths);
    for (int i = 0; i < count; i++) {
        void* node = cPathSys_getTrackPath(gPaths, i);
        float markerDist = cTrackPath_getFinishLineParam(node);
        if (markerDist != -1.0f) {
            for (int j = 0; j < self->mCount; j++) {
                if (n == 0)
                    return;
                float back = markerDist - (float)n-- * 1000.0f;
                if (back < 0.0f)
                    back = 0.0f;
                t4Vector ahead = cPath_getPathPos_A768(node, markerDist, 0);
                t4Vector pos = cPath_getPathPos_A768(node, back, 0);
                cRider_setControlState_impl(self->mRiders[j], 0);
                cRider_setMotionState_impl(self->mRiders[j], 0);
                ((cRider*)self->mRiders[j])->mPenaltyTicks = n * 3600 + 3600;
                t4Vector dir = vSub_A768(ahead, pos);
                float len = vLen_A768(dir);
                if (len > 1.0f)
                    vDivEq_A768(dir, len);
                else
                    dir = Front_A768((cRider*)self->mRiders[j]);
                cRider_putAt_A768((cRider*)self->mRiders[j], &pos, &dir, 5, 0.0f);
            }
            return;
        }
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012A998);
#ifdef SKIP_ASM
// self is a cAI (no header yet): +0x48 is a list of rider objects (count at +0x80), each holding
// its cRider virtual-base pointer at +0x18.
struct sVEntry0012A998 { short delta; short index; int (*fn)(void*); };

extern "C" int func_0012A1F0(cAI*, int);

/**
 * Hides the computer riders that func_0012A1F0 picks so they can be deleted safely later.
 *
 * For each allocated computer (Computers[i], its cRider at +0x18) whose mRenderFlag is set and whose getAIIndex passes
 * func_0012A1F0: clears mRenderFlag and sets mSafeRenderTick to GrMan's virtual slot 114 result + 4 (a frame count a
 * few frames ahead). Nothing in the source calls it; it mirrors cAI_prepareToPurgeComputerRiders.
 */
extern "C" void func_0012A998(cAI* self)
{
    for (int i = 0; i < self->NumberOfComputersAllocated; i++)
    {
        char** list = (char**)((char*)&self->Computers);
        cRider* rider = *(cRider**)(list[i] + 0x18);
        if (rider->mRenderFlag != 0)
        {
            char* sub = (char*)&rider->mVtbl;
            sVEntry0012A998* vt = *(sVEntry0012A998**)sub;
            if (func_0012A1F0(self, vt[kRiderVt_getAIIndex].fn(sub + vt[kRiderVt_getAIIndex].delta)) != 0)
            {
                cRider* hiddenRider = *(cRider**)(list[i] + 0x18);
                hiddenRider->mRenderFlag = 0;
                hiddenRider->mSafeRenderTick = -1;
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                char* obj = list[i];
                sVEntry0012A998* gvt = (sVEntry0012A998*)ps2GraphicsMan->mVtbl;
                (*(cRider**)(obj + 0x18))->mSafeRenderTick = gvt[114].fn((char*)ps2GraphicsMan + gvt[114].delta) + 4;
            }
        }
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012AA78);
#ifdef SKIP_ASM
extern "C" int cRider_isSafeToDelete(void*);
struct sVE_0012AA78 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sEnt_0012AA78 {
    char pad[0x18];
    cRider* rider;
};
struct sOwner_0012AA78 {
    char pad[0x48];
    sEnt_0012AA78* ent[8];
    char pad2[0x80 - 0x68];
    int count;
};

extern "C" int func_0012A1F0(cAI*, int);

/**
 * Returns whether the computer riders that func_0012A1F0 picks can all be deleted now.
 *
 * Returns 0 when any allocated computer that is not yet safe to delete (cRider_isSafeToDelete) passes func_0012A1F0,
 * else 1. Nothing in the source calls it; it mirrors cAI_isReadyToPurgeComputerRiders.
 *
 * @return 1 when every selected rider is safe to delete, else 0
 */
extern "C" int func_0012AA78(sOwner_0012AA78* self) {
    int i;
    int ok = 1;
    for (i = 0; i < self->count; i++) {
        if (cRider_isSafeToDelete(self->ent[i]->rider) == 0) {
            char* obj = (char*)&self->ent[i]->rider->mVtbl;
            sVE_0012AA78* vt = *(sVE_0012AA78**)obj;
            if (func_0012A1F0((cAI*)self, vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta)) != 0)
                ok = 0;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_prepareToPurgeComputerRiders);
#ifdef SKIP_ASM
struct sAiObj12AB20
{
    char pad0[0x14];
    int m14;
    char pad18[0x30];
    char* mRiders[14];
    int mCount;
};

struct sVEntry0012AB20 { short delta; short index; int (*fn)(void*); };


/**
 * Stops drawing the computer riders so they can be purged once the renderer no longer uses them.
 *
 * Clears ComputerRidersActive (+0x14). For each of the NumberOfComputersAllocated computers (Computers[] at +0x48)
 * whose rider (+0x18) still has its render flag (+0x884) set, it clears the flag and stores a safe-render tick (+0x888)
 * of the graphics manager's current frame count (GrMan vtable slot 114) + 4; cAI_isReadyToPurgeComputerRiders then
 * waits for that frame.
 */
extern "C" void cAI_prepareToPurgeComputerRiders(sAiObj12AB20* self)
{
    self->m14 = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* computerRider = *(char**)(self->mRiders[i] + 0x18);
        if (*(int*)(computerRider + 0x884) != 0)
        {
            *(int*)(computerRider + 0x884) = 0;
            *(int*)(computerRider + 0x888) = -1;
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            char* rider = self->mRiders[i];
            sVEntry0012AB20* vt = (sVEntry0012AB20*)ps2GraphicsMan->mVtbl;
            *(int*)(*(char**)(rider + 0x18) + 0x888) = vt[114].fn((char*)ps2GraphicsMan + vt[114].delta) + 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_isReadyToPurgeComputerRiders);
#ifdef SKIP_ASM
struct sAiObj12ABD0
{
    char pad0[0x48];
    char* mRiders[14];
    int mCount;
};

extern "C" int cRider_isSafeToDelete(void*);

/**
 * Tells whether every computer rider can be deleted safely.
 *
 * Checks cRider_isSafeToDelete() on the cRider of each of Computers[0 .. NumberOfComputersAllocated), without
 * stopping early.
 *
 * @return 1 when all computer riders are safe to delete (or there are none), otherwise 0
 */
extern "C" int cAI_isReadyToPurgeComputerRiders(cAI* self)
{
    int result = 1;
    for (int i = 0; i < self->NumberOfComputersAllocated; i++)
    {
        if (cRider_isSafeToDelete(*(void**)(((char**)&self->Computers[0])[i] + 0x18)) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_prepareToPurgeComputerActors);
#ifdef SKIP_ASM
struct sAiObj12AC48
{
    char pad0[0x5C];
    char* mRiders[10];
    int mCount;
    int m88;
    int m8C;
};

struct sVEntry0012AC48 { short delta; short index; int (*fn)(void*); };


/**
 * Stops drawing the computer actors so they can be purged once the renderer no longer uses them.
 *
 * Clears ComputerActorsActive (+0x8C). For each of the NumberOfActorsAllocated actors (Actors[] at +0x5C) whose rider
 * (+0x18) still has its render flag (+0x884) set, it clears the flag and stores a safe-render tick (+0x888) of the
 * graphics manager's current frame count (GrMan vtable slot 114) + 4, which cAI_isReadyToPurgeComputerActors waits for.
 */
extern "C" void cAI_prepareToPurgeComputerActors(sAiObj12AC48* self)
{
    self->m8C = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* actorRider = *(char**)(self->mRiders[i] + 0x18);
        if (*(int*)(actorRider + 0x884) != 0)
        {
            *(int*)(actorRider + 0x884) = 0;
            *(int*)(actorRider + 0x888) = -1;
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            char* rider = self->mRiders[i];
            sVEntry0012AC48* vt = (sVEntry0012AC48*)ps2GraphicsMan->mVtbl;
            *(int*)(*(char**)(rider + 0x18) + 0x888) = vt[114].fn((char*)ps2GraphicsMan + vt[114].delta) + 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_isReadyToPurgeComputerActors);
#ifdef SKIP_ASM
struct sAiObj12ACF8
{
    char pad0[0x5C];
    char* mRiders[10];
    int mCount;
};

extern "C" int cRider_isSafeToDelete(void*);

/**
 * Tells whether every computer actor can be deleted safely.
 *
 * Checks cRider_isSafeToDelete() on the cRider of each of Actors[0 .. NumberOfActorsAllocated), without
 * stopping early.
 *
 * @return 1 when all actors are safe to delete (or there are none), otherwise 0
 */
extern "C" int cAI_isReadyToPurgeComputerActors(cAI* self)
{
    int result = 1;
    for (int i = 0; i < self->NumberOfActorsAllocated; i++)
    {
        if (cRider_isSafeToDelete(*(void**)(((char**)&self->Actors[0])[i] + 0x18)) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_prepareToPurgeMissionRiders);
#ifdef SKIP_ASM
/**
 * Stops drawing the loaded mission riders so they can be deleted safely.
 *
 * For each of the NumberOfMissionRidersLoaded entries (+0x70 table), takes the cRider virtual-base pointer (+0x18)
 * and, when its mRenderFlag is set, clears it and resets mSafeRenderTick to -1.
 */
// self is a cAI (no header yet); +0x70 is its list of rider objects (count at +0x88),
// each holding its cRider virtual-base pointer at +0x18.
extern "C" void cAI_prepareToPurgeMissionRiders(cAI* self)
{
    for (int i = 0; i < self->NumberOfMissionRidersLoaded; i++)
    {
        char* obj = ((char**)((char*)&self->MissionRiders))[i];
        cRider* rider = *(cRider**)(obj + 0x18);
        if (rider->mRenderFlag != 0)
        {
            rider->mRenderFlag = 0;
            rider->mSafeRenderTick = -1;
        }
    }
}
#endif
//100%
INCLUDE_ASM("ai/ai", cAI_isReadyToPurgeMissionRiders);
#ifdef SKIP_ASM
/**
 * Tells whether every loaded mission rider may now be deleted.
 *
 * Asks cRider_isSafeToDelete for each of the NumberOfMissionRidersLoaded entries (+0x70 table, cRider at +0x18) and
 * does not stop early at the first refusal.
 *
 * @return 1 when all mission riders are safe to delete, else 0
 */
extern "C" int cAI_isReadyToPurgeMissionRiders(cAI* self) {
    int i;
    int ok = 1;
    for (i = 0; i < self->NumberOfMissionRidersLoaded; i++) {
        void* missionRider = *(void**)((char*)&self->MissionRiders[0] + (i << 2));
        if (cRider_isSafeToDelete(*(void**)((char*)missionRider + 0x18)) == 0)
            ok = 0;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_purgeComputerRiders);
#ifdef SKIP_ASM
struct sAiMissionVEntry { short delta; short index; void (*fn)(void*, int); };

struct sAiMission
{
    char pad0[0x14];
    int m14;
    char pad18[0x30];
    void* mListA[5];
    void* mListB[5];
    void* mListC[2];
    int mTotal;
    int pad7C;
    int mCountA;
    int mCountB;
    int mCountC;
    int m8C;
    int m90;
};

/**
 * Deletes the computer riders and drops them from the rider counts.
 *
 * Runs each non-null computer's virtual deleting destructor (cRider vtable slot 1, flags 3) and clears its
 * slot, subtracts NumberOfComputersAllocated from NumberOfRidersAllocated, then zeroes it and
 * ComputerRidersActive. Riders[] is not compacted: it keeps the stale pointers and only its count shrinks.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_purgeComputerRiders(cAI* aiManager)
{
    sAiMission* self = (sAiMission*)aiManager;
    for (int i = 0; i < self->mCountA; i++)
    {
        void* computer = self->mListA[i];
        if (computer)
        {
            char* obj = *(char**)((char*)computer + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListA[i] = 0;
    }
    self->mTotal -= self->mCountA;
    self->mCountA = 0;
    self->m14 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_purgeComputerActors);
#ifdef SKIP_ASM
/**
 * Deletes the computer actors and drops them from the rider counts.
 *
 * Runs each non-null actor's virtual deleting destructor (cRider vtable slot 1, flags 3) and clears its slot,
 * subtracts NumberOfActorsAllocated from NumberOfRidersAllocated, then zeroes it and ComputerActorsActive.
 * Riders[] is not compacted: it keeps the stale pointers and only its count shrinks.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_purgeComputerActors(cAI* aiManager)
{
    sAiMission* self = (sAiMission*)aiManager;
    for (int i = 0; i < self->mCountB; i++)
    {
        void* actor = self->mListB[i];
        if (actor)
        {
            char* obj = *(char**)((char*)actor + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListB[i] = 0;
    }
    self->mTotal -= self->mCountB;
    self->mCountB = 0;
    self->m8C = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_purgeMissionRiders);
#ifdef SKIP_ASM
/**
 * Deletes the mission riders and drops them from the rider counts.
 *
 * Runs each non-null mission rider's virtual deleting destructor (cRider vtable slot 1, flags 3) and clears
 * its slot, subtracts NumberOfMissionRidersLoaded from NumberOfRidersAllocated, then zeroes it and
 * MissionRidersActive. Riders[] is not compacted: it keeps the stale pointers and only its count shrinks.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_purgeMissionRiders(cAI* aiManager)
{
    sAiMission* self = (sAiMission*)aiManager;
    for (int i = 0; i < self->mCountC; i++)
    {
        void* missionRider = self->mListC[i];
        if (missionRider)
        {
            char* obj = *(char**)((char*)missionRider + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListC[i] = 0;
    }
    self->mTotal -= self->mCountC;
    self->mCountC = 0;
    self->m90 = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", cAI_cleanupActors);
#ifdef SKIP_ASM
extern "C" void cAI_purgeComputerActors(cAI*);
extern "C" void cAI_RefreshGroupAI(void*);

/**
 * Removes the computer-controlled actors and refreshes the group AI.
 *
 * Calls cAI_purgeComputerActors() and then cAI_RefreshGroupAI().
 */
extern "C" void cAI_cleanupActors(cAI* self)
{
    cAI_purgeComputerActors(self);
    cAI_RefreshGroupAI(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", cAI_cleanupAfterRace);
#ifdef SKIP_ASM
extern "C" void cAI_purgeComputerRiders(cAI*);
extern "C" void cAI_RefreshGroupAI(void*);

/**
 * Removes the computer riders after a race and marks the AI as not ready to race.
 *
 * Calls cAI_purgeComputerRiders() and cAI_RefreshGroupAI(), then clears IsAIReadyToRace.
 */
extern "C" void cAI_cleanupAfterRace(cAI* self)
{
    cAI_purgeComputerRiders(self);
    cAI_RefreshGroupAI(self);
    self->IsAIReadyToRace = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_cleanupAfterFreeride);
#ifdef SKIP_ASM
/**
 * Cleans up the AI manager after free ride: purges the mission riders, refreshes the group AI and clears
 * IsAIReadyToRace.
 */
extern "C" void cAI_cleanupAfterFreeride(cAI* self)
{
    cAI_purgeMissionRiders(self);
    cAI_RefreshGroupAI(self);
    self->IsAIReadyToRace = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", cAI_shutdown);
#ifdef SKIP_ASM
extern "C" void func_00154A58(int rider, int a);
extern "C" void cAI_purgeAnims(cAI*);

struct sAiObj12B090
{
    char pad0[0x28];
    char* mRiders[6];
    void* mList[13];
    int m74;
    int mCount;
    int mCount2;
};

/**
 * Shuts the AI down: records every rider's stats, deletes all riders and frees the character animations.
 *
 * Passes each rider's stats (metrix + 0xFC) to func_00154A58, deletes the players (virtual destructor, flags
 * 3), purges the mission riders, computers and actors, zeroes NumberOfPlayersAllocated and
 * NumberOfRidersAllocated, calls cAI_purgeAnims() and sets RaceRankType to 0.
 *
 * @param aiManager  the AI manager (cAI), viewed through a local layout
 */
extern "C" void cAI_shutdown(cAI* aiManager)
{
    sAiObj12B090* self = (sAiObj12B090*)aiManager;
    for (int i = 0; i < self->mCount; i++)
    {
        func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
    }
    for (int i = 0; i < self->mCount2; i++)
    {
        void* player = self->mList[i];
        if (player)
        {
            char* obj = *(char**)((char*)player + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mList[i] = 0;
    }
    cAI_purgeMissionRiders((cAI*)self);
    cAI_purgeComputerRiders((cAI*)self);
    cAI_purgeComputerActors((cAI*)self);
    self->mCount2 = 0;
    self->mCount = 0;
    cAI_purgeAnims((cAI*)self);
    self->m74 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_resetRidersStatTotals);
#ifdef SKIP_ASM
extern "C" void func_00154A58(int rider, int a);
extern "C" void cRiderMetrix_resetTotals(char* self);

/**
 * Records each rider's race stats with the BE score code, then clears the rider's metrix totals.
 *
 * For every allocated rider, passes the stats block at offset 0xFC of its mMetrix to func_00154A58() (which hands
 * them to func_00155420() for profile indexes 0 and 1 only), then calls cRiderMetrix_resetTotals() on the metrix.
 */
extern "C" void cAI_resetRidersStatTotals(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
        cRiderMetrix_resetTotals(*(char**)(self->mRiders[i] + 0x790));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_purgeAnims);
#ifdef SKIP_ASM
extern void** cAnimDataBase_TheAnimDataBase;
extern void* cGameAnimMap_TheGameAnimMap;
extern "C" void cGameAnimMap_remSource(void*, void*);
extern "C" void cAnimDataBase_purgeChanCache(void*);
extern "C" void cAdfDataSource__cAdfDataSource(void*, int);

/**
 * Unloads the character animation file that cAI_loadAnims() loaded.
 *
 * For the one entry of the anim table cAnimDataBase_TheAnimDataBase: removes its animations from the game anim map cGameAnimMap_TheGameAnimMap,
 * clears the entry, frees the animation banks (cAnimDataBase_purgeChanCache) and deletes the data source (cAdfDataSource__cAdfDataSource,
 * flags 3).
 */
extern "C" void cAI_purgeAnims(cAI* self)
{
    int i;
    for (i = 0; i < 1; i++)
    {
        void* bank = *(void**)((char*)cAnimDataBase_TheAnimDataBase + ((unsigned char)i << 2));
        if (bank != 0)
        {
            cGameAnimMap_remSource(cGameAnimMap_TheGameAnimMap, bank);
            void** animTable = cAnimDataBase_TheAnimDataBase;
            *(void**)((char*)animTable + ((unsigned char)i << 2)) = 0;
            cAnimDataBase_purgeChanCache(animTable);
            cAdfDataSource__cAdfDataSource(bank, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_loadAnims);
#ifdef SKIP_ASM
extern "C" void FILESYS_addbigsync(char* name, int a1, int a2, void* out);
extern "C" void FILESYS_delbigsync(void* handle, int arg);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cAdfDataSource_cAdfDataSource(void* mem, char* name);
extern "C" void cGameAnimMap_addSource(void* map, int i, void* anim);
// PORT: the original passes the map to cGameAnimMap_testResolve(), whose symbol is declared with no params.
void cGameAnimMap_testResolve_impl(void* map) __asm__("cGameAnimMap_testResolve__Fv");
struct sAnimTable { void* anims[1]; };
extern char str_data_char_anm_big[];
extern char str_AdfDataSource[];
extern char* kAnimBankName[1];

/**
 * Loads the riders' basic animation file and registers its animations in the game anim map.
 *
 * Opens "data/char/anm.big" (FILESYS_addbigsync), reads "data/char/anm.big|data/char/basic.afl" into a new
 * "AdfDataSource" kept in the anim table cAnimDataBase_TheAnimDataBase, adds its animations to the map cGameAnimMap_TheGameAnimMap as kind 0,
 * resolves the map (cGameAnimMap_testResolve) and releases the archive handle (FILESYS_delbigsync).
 */
extern "C" void cAI_loadAnims(void)
{
    void* handle;
    int i;
    FILESYS_addbigsync(str_data_char_anm_big, 0, 100, &handle);
    for (i = 0; i < 1; i++)
    {
        void* bank = cAdfDataSource_cAdfDataSource(cMemMan_alloc(0x18, str_AdfDataSource, 0, 0), kAnimBankName[i]);
        ((sAnimTable*)cAnimDataBase_TheAnimDataBase)->anims[(unsigned char)i] = bank;
        cGameAnimMap_addSource(cGameAnimMap_TheGameAnimMap, i, bank);
    }
    cGameAnimMap_testResolve_impl(cGameAnimMap_TheGameAnimMap);
    FILESYS_delbigsync(handle, 100);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_finishRemotePlayer);
#ifdef SKIP_ASM
extern "C" void cGameComm_setFinishRaceAlone(void* self);
extern "C" void cReplay_endReplay(void* self);
extern "C" void cRider_doFinishEvent(void* self);

static inline bool aiRiderFar12B340(char* r)
{
    return *(float*)(r + 0x470) >= 10.0f;
}

/**
 * Ends an online race for the remote player: the link races alone and an unfinished remote rider gets a DNF.
 *
 * Only in the Race state: sets the game link's mAlone (cGameComm_setFinishRaceAlone), then for each non-local player
 * (mIsLocal 0) returns at once if its mFinishTime is >= 10; otherwise ends the replay (cReplay_endReplay), sets
 * mDidNotFinish and calls cRider_doFinishEvent().
 */
extern "C" void cAI_finishRemotePlayer(cAI* self)
{
    if (self->CurrentAIState == 5)
    {
        cGameComm_setFinishRaceAlone((char*)cGameComm_mGameComm);
        for (int i = 0; i < self->NumberOfPlayersAllocated; i++)
        {
            char* rider = ((char**)&self->Players[0])[i];
            char* playerRider = *(char**)(rider + 0x18);
            if (*(int*)(playerRider + 0x87C) == 0)
            {
                bool far = aiRiderFar12B340(playerRider);
                if (far)
                {
                    return;
                }
                cReplay_endReplay(SSXApp->Game->Replay);
                *(int*)(*(char**)(rider + 0x18) + 0x480) = 1;
                cRider_doFinishEvent(*(void**)(rider + 0x18));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_syncCheckSum);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" int cRider_syncCheckSum(void* self);

/**
 * Builds the online sync checksum: one AIrand() draw plus every rider's cRider_syncCheckSum().
 *
 * The AIrand() call advances the shared AI random generator. cAI_onlineAICheck calls this every
 * 60 ticks of an online race.
 *
 * @return the sum (wrapping)
 */
extern "C" int cAI_syncCheckSum(cAI* self)
{
    int sum = AIrand();
    for (int i = 0; i < self->NumberOfRidersAllocated; i++)
    {
        sum += cRider_syncCheckSum(((char**)&self->Riders[0])[i]);
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_applyPlayerRulesCustom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_initBoost(void* self);
extern "C" void cRider_setUbersAllowed(void* self, int a1);
extern "C" void cRiderMetrix_setInitialPointsTotal(void* self, int a1);
extern "C" int cBERules_getPlayerStartDelaySecs(int i);


struct sOptEntry_12B498
{
    unsigned int f0;
    int x;
    int y;
};

struct sOptView_12B498
{
    sOptEntry_12B498 e[2];
    int f18;
};

// The unit declares cBERules_mRules later with its own type; bind this view by asm label.
extern sOptView_12B498 D_005308B8_12B498 __asm__("cBERules_mRules");

static inline int OptBit_12B498(sOptView_12B498* o, int b)
{
    return (o->f18 >> b) & 1;
}

/**
 * Applies the custom game rules (cBERules_mRules) to a newly created human player's rider.
 *
 * mBoostMode is 3 with global bit 3, 0 with global bit 0, 2 with the player's bit 5, else 1; then
 * cRider_initBoost(). Player bits: 3 full boost meter, 4 no Ubers (else global bit 1 allows them), 0 unk_B20
 * = 2 (faster wipeout recovery), 2 metrix +0x1C4 = 0.5, 1 mStatsIndex 1 offline (ranked online games always
 * get 8); global bit 4 sets mBoostSourceMask 2. mStartHandicap = cBERules_getPlayerStartDelaySecs(i); the player's second rule
 * word goes to metrix +0x1C8.
 *
 * @param obj  the new cPlayer (On Tour pPlayer)
 * @param i  the player index, 0 or 1, selecting the per-player rule entry (On Tour playerIndex)
 */
extern "C" void cAI_applyPlayerRulesCustom(cAI* self, cPlayer* obj, int i)
{
    int boostMode = 1;
    if (OptBit_12B498(&D_005308B8_12B498, 3))
        boostMode = 3;
    else if (OptBit_12B498(&D_005308B8_12B498, 0))
        boostMode = 0;
    else
        if ((D_005308B8_12B498.e[i].f0 >> 5) & 1) boostMode = 2;
    cRider* playerRider = obj->mRider;
    playerRider->mBoostMode = boostMode;
    cRider_initBoost(playerRider);
    if ((D_005308B8_12B498.e[i].f0 >> 3) & 1)
        obj->mRider->mBoostMeterStart = 1.0f;
    if (OptBit_12B498(&D_005308B8_12B498, 4))
        obj->mRider->mBoostSourceMask = 2;
    if ((D_005308B8_12B498.e[i].f0 >> 4) & 1)
        cRider_setUbersAllowed(obj->mRider, 0);
    else if (OptBit_12B498(&D_005308B8_12B498, 1))
        cRider_setUbersAllowed(obj->mRider, 1);
    if (D_005308B8_12B498.e[i].f0 & 1)
        obj->mRider->unk_B20 = 2;
    (obj->mRider)->mStartHandicap = cBERules_getPlayerStartDelaySecs(i);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0)
    {
        if (cBENetworkInterface_networkOpt.bIsRankedGame != 0)
            obj->mRider->mStatsIndex = 8;
    }
    else if ((D_005308B8_12B498.e[i].f0 >> 1) & 1)
        obj->mRider->mStatsIndex = 1;
    char* riderMetrix = (char*)obj->mRider->mMetrix;
    if ((D_005308B8_12B498.e[i].f0 >> 2) & 1)
        *(float*)(riderMetrix + 0x1C4) = 0.5f;
    cRiderMetrix_setInitialPointsTotal(riderMetrix, D_005308B8_12B498.e[i].x);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_applyComputerRulesCustom);
#ifdef SKIP_ASM
extern "C" void cRider_initBoost(void* self);
extern "C" void cRider_setUbersAllowed(void* self, int a1);
extern "C" int cBERules_getAIStartDelaySecs();

struct sOptFlags_0012B698
{
    unsigned int f0;
    char pad4[0x14];
    int f18;
};
extern sOptFlags_0012B698 cBERules_mRules;

static inline int OptBit_0012B698(sOptFlags_0012B698* o, int b)
{
    return (o->f18 >> b) & 1;
}

/**
 * Applies the custom game rules (cBERules_mRules) to a newly created computer rider.
 *
 * Same boost-mode choice as for players, then cRider_initBoost(); global bit 4 sets mBoostSourceMask 2,
 * global bit 1 allows Ubers, and mStartHandicap = cBERules_getAIStartDelaySecs(). The per-player bits it reads (5: boost
 * mode 2, 3: full boost meter) are player 0's entry.
 *
 * @param rider  the new cComputer (On Tour pComputer)
 */
extern "C" void cAI_applyComputerRulesCustom(cAI* self, cComputer* rider)
{
    int boostMode = 1;
    if (OptBit_0012B698(&cBERules_mRules, 3))
        boostMode = 3;
    else if (OptBit_0012B698(&cBERules_mRules, 0))
        boostMode = 0;
    else
        if ((cBERules_mRules.f0 >> 5) & 1) boostMode = 2;
    cRider* computerRider = rider->mRider;
    computerRider->mBoostMode = boostMode;
    cRider_initBoost(computerRider);
    if ((cBERules_mRules.f0 >> 3) & 1)
        rider->mRider->mBoostMeterStart = 1.0f;
    if (OptBit_0012B698(&cBERules_mRules, 4))
        rider->mRider->mBoostSourceMask = 2;
    if (OptBit_0012B698(&cBERules_mRules, 1))
        cRider_setUbersAllowed(rider->mRider, 1);
    rider->mRider->mStartHandicap = cBERules_getAIStartDelaySecs();
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_updateOverlapList);
#ifdef SKIP_ASM
extern "C" void cRider_updateOverlapList(void*);

struct sAi12B788
{
    char pad0[0x28];
    void* items[20];
    int count;
};

/**
 * Refreshes every allocated rider's overlap list with cRider_updateOverlapList().
 */
extern "C" void cAI_updateOverlapList(cAI* self)
{
    int i;
    for (i = 0; i < self->NumberOfRidersAllocated; i++)
    {
        cRider_updateOverlapList(((void**)&self->Riders[0])[i]);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_writeToReplayFrame(void* frame);

struct sVEntryWF { short delta; short index; void (*fn)(void*, void*, int); };
struct sVEntryWF2 { short delta; short index; void (*fn)(void*, void*); };


static inline void replayWriteObj(char* obj, void* frame)
{
    char* x = *(char**)(*(char**)(obj + 0x18) + 0x77C);
    sVEntryWF2* vt = *(sVEntryWF2**)(x + 0xDE8);
    vt[2].fn(x + vt[2].delta, frame);
}

/**
 * Writes the AI's part of a replay frame: the 0x28-byte cAIState block, the riders and the render state.
 *
 * The block (states, ticks, timers, podium list) goes through the frame's virtual write (slot 1); then each
 * player's, computer's and actor's rider handler writes itself (handler vtable slot 2; mission riders are
 * skipped), and cRenderStateMan_writeToReplayFrame, probably the render-state manager's counterpart of
 * cRenderStateMan_readFromReplayFrame.
 *
 * @param frame  the replay frame (On Tour cReplayFramePtr)
 */
extern "C" void cAI_writeToReplayFrame(cAI* self, cReplayFramePtr* frame)
{
    sVEntryWF* vt = *(sVEntryWF**)frame;
    vt[1].fn((char*)frame + vt[1].delta, (self), 40);
    for (int i = 0; i < self->NumberOfPlayersAllocated; i++)
    {
        replayWriteObj(((char**)&self->Players[0])[i], frame);
    }
    for (int i = 0; i < self->NumberOfComputersAllocated; i++)
    {
        replayWriteObj(((char**)&self->Computers[0])[i], frame);
    }
    for (int i = 0; i < self->NumberOfActorsAllocated; i++)
    {
        replayWriteObj(((char**)&self->Actors[0])[i], frame);
    }
    cRenderStateMan_writeToReplayFrame(frame);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAI_forceAIState(void* self, int state);
extern "C" void cRenderStateMan_readFromReplayFrame(void* frame);

struct sVEntryRF { short delta; short index; void (*fn)(void*, void*, int); };
struct sVEntryRF2 { short delta; short index; void (*fn)(void*, void*); };


static inline void replayReadObj(char* obj, void* frame)
{
    char* x = *(char**)(*(char**)(obj + 0x18) + 0x77C);
    sVEntryRF2* vt = *(sVEntryRF2**)(x + 0xDE8);
    vt[3].fn(x + vt[3].delta, frame);
}

/**
 * Reads the AI's part of a replay frame back: the cAIState block, its handlers, the riders, the render state.
 *
 * After reading the 0x28-byte block (frame slot 2) it rebuilds the handler pointers from the restored ids: it
 * forces PreviousAIState to get PreviousAIStateHandler, then forces the current state back. Then each
 * player's, computer's and actor's rider handler reads itself (slot 3), followed by
 * cRenderStateMan_readFromReplayFrame().
 *
 * @param frame  the replay frame (On Tour cReplayFramePtr)
 */
extern "C" void cAI_readFromReplayFrame(cAI* self, cReplayFramePtr* frame)
{
    sVEntryRF* vt = *(sVEntryRF**)frame;
    vt[2].fn((char*)frame + vt[2].delta, (self), 40);
    int state = self->CurrentAIState;
    cAI_forceAIState((self), self->PreviousAIState);
    self->PreviousAIStateHandler = (cAIStateHandler*)(int)self->CurrentAIStateHandler;
    cAI_forceAIState((self), state);
    for (int i = 0; i < self->NumberOfPlayersAllocated; i++)
    {
        replayReadObj(((char**)&self->Players[0])[i], frame);
    }
    for (int i = 0; i < self->NumberOfComputersAllocated; i++)
    {
        replayReadObj(((char**)&self->Computers[0])[i], frame);
    }
    for (int i = 0; i < self->NumberOfActorsAllocated; i++)
    {
        replayReadObj(((char**)&self->Actors[0])[i], frame);
    }
    cRenderStateMan_readFromReplayFrame(frame);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_setPodiumActorList);
#ifdef SKIP_ASM
/**
 * Copies two character IDs into PodiumActorList.
 *
 * @param src  the two characters (eBECharacters) for the podium actors
 */
extern "C" void cAI_setPodiumActorList(cAI* self, int* src)
{
    int* dst = (int*)&self->PodiumActorList;
    for (int i = 0; i < 2; i++)
    {
        dst[i] = src[i];
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_getPodiumActorList);
#ifdef SKIP_ASM
/**
 * Copies PodiumActorList's two character IDs out.
 *
 * @param dst  receives the two podium actor characters (eBECharacters)
 */
extern "C" void cAI_getPodiumActorList(cAI* self, int* dst)
{
    int* src = (int*)&self->PodiumActorList;
    for (int i = 0; i < 2; i++)
    {
        dst[i] = src[i];
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_onlineAICheck);
#ifdef SKIP_ASM
extern "C" void cGameComm_updateSyncPacket(void* self, int t, int seed);

struct sAi_12BB20
{
    int state;          // 0x00
    int f4;
    int timer;          // 0x08
    int fC[3];
    int countdown;      // 0x18
    char pad1C[0x24];
    char* riders[15];   // 0x40
    int count;          // 0x7C
};

static inline bool riderOk_12BB20(char* rider)
{
    return *(float*)(*(char**)(rider + 0x18) + 0x470) >= 0.0f;
}

/**
 * Keeps an online race in step: checksums the AI, ends the race on a timer and sends the periodic sync packet.
 *
 * Does nothing unless cBENetworkInterface_networkOpt.bIsNetworkGame. Every 60 AITicks it takes cAI_syncCheckSum. In the
 * race state (5), or state 3 while the game is in state 3, and only for game event 0 or 4 (newRace.cGameEvent,
 * cBENewRaceInterface::cGameEvent): while EndOfRaceTimer (+0x18) is positive it counts down, and at 0 every player
 * whose rider has not passed the finish line (rider +0x470 < 0) is marked did-not-finish (+0x480 = 1) and finished with
 * cRider_doFinishEvent, the replay is ended and the timer goes back to -1.
 *
 * With no timer running, once a player has finished (and the game is neither ended remotely nor alone) before tick
 * 0xD32B, the timer starts at min(0xD3A4 - AITick, 7200); past tick 0xD3A3 the unfinished players are force-finished at
 * once (0xD3A4 ticks is about 15 minutes at 60 Hz). Finally, unless a replay is playing, every 60 ticks cGameComm_updateSyncPacket
 * (On Tour cGameComm::updateSyncPacket) sends the tick and checksum.
 */
extern "C" void cAI_onlineAICheck(sAi_12BB20* self)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0)
        return;
    int seed = 0;
    if (self->timer % 60 == 0)
        seed = cAI_syncCheckSum((cAI*)((sAiObj128A48*)self));
    int st = self->state;
    if (st != 5) {
        if (st != 3)
            return;
        if (SSXApp->Game->mCurrentStateID != st)
            return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char gameEvent = cBENewRaceInterface_newRace.cGameEvent;
    if (gameEvent == 0 || gameEvent == 4) {
        int endTimer = self->countdown;
        if (endTimer >= 0) {
            if (endTimer > 0) {
                endTimer--;
                self->countdown = endTimer;
                if (endTimer <= 0) {
                    for (int i = 0; i < self->count; i++) {
                        bool ok = riderOk_12BB20(self->riders[i]);
                        if (!ok) {
                            *(int*)(*(char**)(self->riders[i] + 0x18) + 0x480) = 1;
                            cRider_doFinishEvent(*(void**)(self->riders[i] + 0x18));
                        }
                    }
                    cReplay_endReplay(SSXApp->Game->Replay);
                    self->countdown = -1;
                }
            }
        } else {
            if (self->timer <= 54059 && cAI_isRaceFinished((cAI*)((sAiObj12A250*)self)) == 0) {
                int n = self->count;
                for (int j = 0; j < n; j++) {
                    bool ok = riderOk_12BB20(self->riders[j]);
                    if (ok && cGameComm_mGameComm->mRemoteEndGame == 0 && cGameComm_mGameComm->mAlone == 0) {
                        int timeLeft = 54180 - self->timer;
                        if (timeLeft > 7200) timeLeft = 7200;
                        self->countdown = timeLeft;
                    }
                }
            }
            if (self->timer > 54179 && cAI_isRaceFinished((cAI*)((sAiObj12A250*)self)) == 0) {
                for (int i = 0; i < self->count; i++) {
                    bool ok = riderOk_12BB20(self->riders[i]);
                    if (!ok) {
                        *(int*)(*(char**)(self->riders[i] + 0x18) + 0x480) = 1;
                        cRider_doFinishEvent(*(void**)(self->riders[i] + 0x18));
                    }
                }
                cReplay_endReplay(SSXApp->Game->Replay);
                self->countdown = -1;
            }
        }
    }
    int replayState = *(int*)SSXApp->Game->Replay;
    int busy = replayState != 0 && replayState < 10;
    if (busy)
        return;
    if (self->timer % 60 == 0)
        cGameComm_updateSyncPacket((char*)cGameComm_mGameComm, self->timer, seed);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_gainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

struct sQuat_12BE20
{
    float x, y, z, w;
    sQuat_12BE20() {}
    sQuat_12BE20(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_12BE20 D_004FF130_12BE20 __asm__("t4vect_Origin");
extern sQuat_12BE20 D_004FF160_12BE20 __asm__("t4vect_ZAxis");

static inline sQuat_12BE20 AxisAngle_12BE20(const sQuat_12BE20& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_12BE20(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Enters the start-gate control: faces the rider by its stance and starts the gate rock (or sit) animation.
 *
 * Copies IsGoofyStance into the rider's stance word (+0x320) and the anim's (+0x18), resets mRiderAnim with
 * its root at the origin, turned half a circle about Z when the stance is nonzero. GateState 2 (sit) plays
 * anim 1; anything else plays anim 0 and becomes 0 (rock). Clears GateAnticipate, HeldAtExtremeSec and both
 * thrusts.
 */
extern "C" void cGateControl_gainFocus(cGateControl* self)
{
    cRider* rider0 = self->mRider;
    rider0->IsRidingFakie = rider0->IsGoofyStance;
    cRiderAnimBase_reset(self->mRider->mRiderAnim);
    cRider* rider1 = self->mRider;
    *(int*)((char*)rider1->mRiderAnim + 0x18) = rider1->IsRidingFakie;
    cRider* rider = self->mRider;
    float fakieAngle = 0.0f;
    if (rider->IsRidingFakie != 0) fakieAngle = 3.1415927410125732f;
    char* anim = (char*)rider->mRiderAnim;
    *(sQuat_12BE20*)(anim + 0x30) = D_004FF130_12BE20;
    *(sQuat_12BE20*)(anim + 0x40) = AxisAngle_12BE20(D_004FF160_12BE20, -fakieAngle);
    if (*(int*)self == 2)
    {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 1, 0, -1.0f);
    }
    else
    {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 0, 0, -1.0f);
        *(int*)self = 0;
    }
    *(int*)&self->GateAnticipate = 0;
    *(int*)&self->HeldAtExtremeSec = 0;
    *(int*)&self->CurThrustMin = 0;
    *(int*)&self->CurThrustMax = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_update);
#ifdef SKIP_ASM
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cGateControl_doSit(cGateControl* self, void* a1);
extern "C" void cGateControl_doGetupFromSit(cGateControl* self, void* a1);
extern "C" void cGateControl_doGateRock(cGateControl* self, void* a1);
extern "C" void cGateControl_doGateLaunch(cGateControl* self, void* a1);

/**
 * Runs the start gate for one tick: stops any boost, then runs the handler of the current gate state.
 *
 * Calls cRider_setupBoostWarp(rider, 0, 0), then by GateState: 0 doGateRock, 1 doGateLaunch, 2 doSit, 3
 * doGetupFromSit.
 *
 * @param cmd  the gate command (On Tour tGateCmd), passed on to the state handler
 */
extern "C" void cGateControl_update(cGateControl* self, void* cmd)
{
    cRider_setupBoostWarp(self->mRider, 0, 0);
    switch (*(int*)self)
    {
    case 0:
        cGateControl_doGateRock(self, cmd);
        break;
    case 1:
        cGateControl_doGateLaunch(self, cmd);
        break;
    case 2:
        cGateControl_doSit(self, cmd);
        break;
    case 3:
        cGateControl_doGetupFromSit(self, cmd);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_isLaunchAllowed);
#ifdef SKIP_ASM

/**
 * Tells whether the rider may leave the gate: the race is running and its start handicap has elapsed.
 *
 * True in AI state Race (5) once the race time in whole seconds (EventTimerTick / 60) reaches mStartHandicap.
 *
 * @return 1 when the launch is allowed, otherwise 0
 */
extern "C" int cGateControl_isLaunchAllowed(cGateControl* self)
{
    int allowed = 0;
    cAI* gameAi = SSXApp->Game->AI;
    if (*(int*)gameAi == 5)
    {
        allowed = self->mRider->mStartHandicap <= (int)(gameAi->EventTimerTick * 0.01666666753590107f);
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_isForceLaunch);
#ifdef SKIP_ASM

/**
 * Tells whether the rider has to launch: the AI is in the Race state or the rider has already finished.
 *
 * doGateRock calls it only after isLaunchAllowed(), which already requires the Race state, so there it is
 * always 1.
 *
 * @return 1 when the AI state is Race (5) or mFinishTime >= 0, otherwise 0
 */
extern "C" int cGateControl_isForceLaunch(cGateControl* self)
{
    if (SSXApp->Game->AI->CurrentAIState == 5)
        return 1;
    if (self->mRider->mFinishTime >= 0.0f)
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_doSit);
#ifdef SKIP_ASM
// PORT: cRiderAnimBase_play returns a value here (the target uses v1 after the call); the unit declares it void.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
struct sAiBits0012C0C0
{
    int pad : 12;
    int v : 6;
};

/**
 * Sit gate state: stands the rider up once the command pushes forward.
 *
 * When the command's signed 6-bit axis (bits 12-17, scaled by 1/31) is positive, plays anim 2 (get up) and
 * sets GateState to 3 (getup from sit).
 *
 * @param cmd  the gate command (On Tour tGateCmd)
 */
extern "C" void cGateControl_doSit(cGateControl* self, void* cmd)
{
    if (((sAiBits0012C0C0*)cmd)->v * 0.032258063554763794f > 0.0f)
    {
        cRiderAnimBase_play_i(self->mRider->mRiderAnim, 2, 0, -1.0f);
        *(int*)self = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_doGetupFromSit);
#ifdef SKIP_ASM
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(void* self, int i);
int cRider_getMotionState(void* self);
// PORT: prototype mismatch. cRider_setControlState/cRider_setMotionState are defined with one param, but callers pass a
// mode in $a1.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

struct sVec4_12C130
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_12C130 vu0Scale_12C130(const sVec4_12C130& in, float s)
{
    sVec4_12C130 v = in;
    sVec4_12C130 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

/**
 * Getup-from-sit gate state: launches the rider at a fixed speed once the get-up animation is half done.
 *
 * Holds cRider_setupCrouchBrake(rider, 1, 0). While anim slot 2 still plays anim 2: if Static (motion state 3) and past
 * 50%, sets mVelocity = the matrix's front row * 555.56 and switches to motion state Ground. Once that anim
 * is over: control state NaturalAir (4) if airborne, else Cruise (0), and GateState 0.
 *
 * @param cmd  the gate command (On Tour tGateCmd); not read
 */
extern "C" void cGateControl_doGetupFromSit(cGateControl* self, void* cmd)
{
    cRider_setupCrouchBrake(self->mRider, 1.0f, 0.0f);
    if (cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2) == 2)
    {
        if (cRider_getMotionState(self->mRider) == 3)
        {
            if (cRiderAnimBase_getCurrentAnimFramePercent(self->mRider->mRiderAnim, 2) > 0.5f)
            {
                cRider* rider = self->mRider;
                *(sVec4_12C130*)&rider->mVelocity = vu0Scale_12C130(*(sVec4_12C130*)&rider->mMatrix.row[1], 555.5555419921875f);
                cRider_setMotionState_impl(self->mRider, 0);
            }
        }
    }
    else
    {
        if (cRider_getMotionState(self->mRider) == 1)
        {
            cRider_setControlState_impl(self->mRider, 4);
        }
        else
        {
            cRider_setControlState_impl(self->mRider, 0);
        }
        *(int*)self = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_doGateRock);
#ifdef SKIP_ASM
extern "C" float cRider_getStartGateKneeOffset(void* rider);
extern "C" int cGateControl_isLaunchAllowed(cGateControl* self);
extern "C" int cGateControl_isForceLaunch(cGateControl* self);

struct sAiBits0012C230
{
    int pad : 12;
    int v : 6;
};

/**
 * Gate rock state: rocks the rider toward the command's lean and launches it once the race allows.
 *
 * Moves GateAnticipate toward 0.62 (axis forward), 0 (back) or 0.31 (centre), faster for big gaps; each
 * change restarts HeldAtExtremeSec and stores the value in CurThrustMax (rising) or CurThrustMin (falling).
 * On launch the speed is 555.56, or (Max - Min) * 2027.78 * (1 - GateAnticipate) / (Held + 1) if higher when
 * pushing forward within 0.4 s of a change; it goes into CurThrustMax and GateState becomes 1. Also pins the
 * knee channel.
 *
 * @param cmd  the gate command (On Tour tGateCmd): bits 12-17 are the signed forward/back axis
 */
extern "C" void cGateControl_doGateRock(cGateControl* self, void* cmd)
{
    float fy = ((sAiBits0012C230*)cmd)->v * 0.032258063554763794f;
    float old = self->GateAnticipate;
    float target;
    if (fy > 0.0f)
        target = 0.6200000047683716f;
    else if (fy < 0.0f)
        target = 0.0f;
    else
        target = 0.3100000023841858f;
    float kneeOffset = cRider_getStartGateKneeOffset(self->mRider);
    cRider* rider = self->mRider;
    rider->mSlewed[18].rate = 0.0f;
    rider->mSlewed[18].target = kneeOffset;
    rider->mSlewed[18].cur = kneeOffset;
    float targetDist = __builtin_fabsf(target - self->GateAnticipate);
    float rate;
    if (targetDist >= 0.20000000298023224f)
        rate = targetDist * 4.5f;
    else
        rate = 0.9000000357627869f;
    float step = rate * 0.01666666753590107f;
    float anticipate = self->GateAnticipate;
    float n;
    if (anticipate > target + step)
    {
        n = anticipate - step;
    }
    else
    {
        n = target;
        if (anticipate < target - step)
            n = anticipate + step;
    }
    self->GateAnticipate = n;
    self->HeldAtExtremeSec = self->HeldAtExtremeSec + 0.01666666753590107f;
    if (old != n)
    {
        *(int*)&self->HeldAtExtremeSec = 0;
        if (old < n)
            self->CurThrustMax = n;
        else
            self->CurThrustMin = n;
    }
    if (cGateControl_isLaunchAllowed(self) == 0) return;
    if (cGateControl_isForceLaunch(self) == 0) return;
    float z = 0.0f;
    if (target > 0.5f && self->HeldAtExtremeSec < 0.4000000059604645f)
    {
        float extremeSec = self->HeldAtExtremeSec;
        z = (self->CurThrustMax - self->CurThrustMin) * 2027.77783203125f * (1.0f - self->GateAnticipate) / (extremeSec + 1.0f);
    }
    if (z < 555.5555419921875f)
        z = 555.5555419921875f;
    self->CurThrustMax = z;
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_doGateLaunch);
#ifdef SKIP_ASM
extern "C" float cRider_getStartGateKneeOffset(void* rider);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Gate launch state: pushes the rider out of the gate at the launch speed chosen by doGateRock.
 *
 * Moves GateAnticipate toward 1.0 by 0.015 per tick and holds cRider_setupCrouchBrake(rider, 1, 0). While Static
 * (motion state 3) with GateAnticipate >= 0.62, sets mVelocity = the front row * CurThrustMax and switches to
 * Ground. At 1.0 plays anim 6 and leaves the gate: control state NaturalAir (4) if airborne, else Cruise (0).
 *
 * @param cmd  the gate command (On Tour tGateCmd); not read
 */
extern "C" void cGateControl_doGateLaunch(cGateControl* self, void* cmd)
{
    float kneeOffset = cRider_getStartGateKneeOffset(self->mRider);
    cRider* riderObj = self->mRider;
    riderObj->mSlewed[18].rate = 0.0f;
    riderObj->mSlewed[18].target = kneeOffset;
    riderObj->mSlewed[18].cur = kneeOffset;
    float anticipateStep = 0.015000001527369022f;
    float anticipate = self->GateAnticipate;
    float n;
    if (anticipate > 1.0149999856948853f)
    {
        n = anticipate - anticipateStep;
    }
    else
    {
        n = 1.0f;
        if (anticipate < 0.9850000143051147f)
            n = anticipate + anticipateStep;
    }
    self->GateAnticipate = n;
    cRider_setupCrouchBrake(self->mRider, 1.0f, 0.0f);
    if (cRider_getMotionState(self->mRider) == 3 && self->GateAnticipate >= 0.6200000047683716f)
    {
        cRider* rider = self->mRider;
        *(sVec4_12C130*)&rider->mVelocity = vu0Scale_12C130(*(sVec4_12C130*)&rider->mMatrix.row[1], self->CurThrustMax);
        cRider_setMotionState_impl(self->mRider, 0);
    }
    if (self->GateAnticipate == 1.0f)
    {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 6, 0, -1.0f);
        if (cRider_getMotionState(self->mRider) == 1)
        {
            cRider_setControlState_impl(self->mRider, 4);
        }
        else
        {
            cRider_setControlState_impl(self->mRider, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sAiVEntry12C558 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the gate control's state (its first 0x14 bytes, GateState to GateAnticipate) to a replay frame.
 *
 * @param obj  the replay frame (On Tour cReplayFramePtr); its virtual write (slot 1) does the copy
 */
extern "C" void cGateControl_writeToReplayFrame(cGateControl* self, cReplayFramePtr* obj)
{
    sAiVEntry12C558* vt = *(sAiVEntry12C558**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cGateControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sAiVEntry12C590 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the gate control's state (its first 0x14 bytes, GateState to GateAnticipate) from a replay frame.
 *
 * @param obj  the replay frame (On Tour cReplayFramePtr); its virtual read (slot 2) does the copy
 */
extern "C" void cGateControl_readFromReplayFrame(cGateControl* self, cReplayFramePtr* obj)
{
    sAiVEntry12C590* vt = *(sAiVEntry12C590**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cFinishLineControl_gainFocus);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_evReset(void* self, int a1);
extern float nFinishLineControl_kFinishStopThreshCPS;

struct sAiVec4_12C5C8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float aiLength_12C5C8(const sAiVec4_12C5C8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

struct sAiObj12C5C8
{
    int m0;
    char* mRider;
};

/**
 * Enters the finish-line control: closes the rider's trick scoring and picks the first finish state.
 *
 * Ends the current trick and combo (cRiderMetrix_evReset(metrix, 1)). FinishState is 0 (Brake) when the rider moves
 * faster than nFinishLineControl_kFinishStopThreshCPS (277.78, set by a static initializer), is spinning (mSpinRate) or stands on a slope
 * (mGroundNormal.z < 0.8); otherwise 1 (IntoStance).
 */
extern "C" void cFinishLineControl_gainFocus(cFinishLineControl* self)
{
    cRiderMetrix_evReset(self->mRider->mMetrix, 1);
    cRider* rider = self->mRider;
    if (nFinishLineControl_kFinishStopThreshCPS < aiLength_12C5C8(*(sAiVec4_12C5C8*)&rider->mVelocity))
    {
        self->FinishState = 0;
        return;
    }
    if (rider->mSpinRate != 0.0f)
    {
        self->FinishState = 0;
        return;
    }
    if (rider->mGroundNormal.z < 0.800000011920929f)
    {
        self->FinishState = 0;
        return;
    }
    self->FinishState = 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cFinishLineControl_update);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_flagEventReaction(void* self, int mode, float t);
extern "C" void cRider_setupLean(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" int cRider_isChangeFakieGround(void* rider);
extern "C" void cRider_checkForReaction(void*);
int cRider_getMotionState(void* self);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern float nFinishLineControl_kFinishStopThreshCPS;
extern char t4vect_Zero[];

struct sVec4_12C678
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector *= scalar).
static inline void vu0ScaleEq_12C678(sVec4_12C678& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float aiLength_12C678(const sVec4_12C678& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_12C678(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

struct sAiObj12C678
{
    int mState;
    char* mRider;
};

/**
 * Runs the finish-line control for one tick: brakes the rider, turns it into stance, plays its reaction.
 *
 * Brake (0): plays the reverse turn (anim 0x15) when it runs or can start; else, on the ground, anim 0xB with
 * a small lean and no boost or spin, and on flat ground velocity * 0.97 until the speed drops under 277.78 or
 * channel 3 (+0x214) reaches +-1 (then 1). IntoStance (1): velocity * 0.93 until lean and channel 3 settle,
 * then 2 with anim 4 and a queued reaction (kind 1 when rider +0x100 is 1, else 4). Reaction (2): holds the
 * rider still and plays it (cRider_checkForReaction) at anim speed 0.75; once cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) is 0
 * (probably the reaction is over), mFinishTime = max(mFinishTime, 10) and state 3 (Done).
 */
extern "C" void cFinishLineControl_update(cFinishLineControl* self)
{
    int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int prev = anim;
    cRider* rider = self->mRider;
    rider->mSlewed[1].rate = 0.03333333507180214f;
    rider->mSlewed[1].target = rider->mSlewed[0].cur;
    switch (self->FinishState)
    {
    case 0:
        if (anim == 21 || (self->mRider->mSpinRate == 0.0f && cRider_isChangeFakieGround((char*)self->mRider)))
        {
            anim = 21;
            cRider_setupCrouchBrake((char*)self->mRider, 0.0f, 0.0f);
        }
        else
        {
            if (cRider_getMotionState((char*)self->mRider) != 0)
                break;
            float floatZero = 0.0f;
            float turn = -0.10000000149011612f;
            if (floatZero < self->mRider->mSlewed[0].cur)
                turn = 0.10000000149011612f;
            anim = 11;
            cRider_setupLean((char*)self->mRider, turn);
            cRider_setupBoostWarp((char*)self->mRider, 0, 0);
            self->mRider->mSpinRate = floatZero;
            if (self->mRider->mGroundNormal.z < 0.800000011920929f)
            {
                cRider_setupCrouchBrake((char*)self->mRider, floatZero, floatZero);
            }
            else
            {
                vu0ScaleEq_12C678(*(sVec4_12C678*)&self->mRider->mVelocity, 0.9700000286102295f);
                if (aiAbs_12C678(self->mRider->mSlewed[3].cur) == 1.0f ||
                    aiLength_12C678(*(sVec4_12C678*)&self->mRider->mVelocity) < nFinishLineControl_kFinishStopThreshCPS)
                {
                    self->FinishState = 1;
                }
                else
                {
                    cRider_setupCrouchBrake((char*)self->mRider, floatZero, 1.0f);
                }
            }
        }
        break;
    case 1:
    {
        float floatZero = 0.0f;
        cRider_setupCrouchBrake((char*)self->mRider, floatZero, floatZero);
        cRider_setupLean((char*)self->mRider, floatZero);
        vu0ScaleEq_12C678(*(sVec4_12C678*)&self->mRider->mVelocity, 0.9300000071525574f);
        if (aiAbs_12C678(self->mRider->mSlewed[3].cur) < 0.20000000298023224f &&
            self->mRider->mSlewed[0].cur == floatZero)
        {
            self->FinishState = 2;
            anim = 4;
            if (self->mRider->unk_100 == 1)
                cRider_flagEventReaction((char*)self->mRider, 1, floatZero);
            else
                cRider_flagEventReaction((char*)self->mRider, 4, floatZero);
        }
        break;
    }
    case 2:
        *(sVec4_12C678*)&self->mRider->mVelocity = *(sVec4_12C678*)t4vect_Zero;
        *(float*)((char*)self->mRider->mRiderAnim + 0x1C) = 0.75f;
        cRider_checkForReaction((char*)self->mRider);
        *(float*)((char*)self->mRider->mRiderAnim + 0x1C) = 1.0f;
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 1) == 0)
        {
            self->mRider->mFinishTime = self->mRider->mFinishTime >? 10.0f;
            self->FinishState = 3;
        }
        break;
    case 3:
        *(sVec4_12C678*)&self->mRider->mVelocity = *(sVec4_12C678*)t4vect_Zero;
        break;
    }
    if (anim != prev)
        cRiderAnimBase_play(self->mRider->mRiderAnim, anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cFinishLineControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012C9C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the finish-line control's state (FinishState, 4 bytes) to a replay frame.
 *
 * @param obj  the replay frame (On Tour cReplayFramePtr); its virtual write (slot 1) does the copy
 */
extern "C" void cFinishLineControl_writeToReplayFrame(cFinishLineControl* self, cReplayFramePtr* obj)
{
    sVEntry0012C9C0* vt = *(sVEntry0012C9C0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cFinishLineControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012C9F8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the finish-line control's state (FinishState, 4 bytes) back from a replay frame.
 *
 * @param obj  the replay frame (On Tour cReplayFramePtr); its virtual read (slot 2) does the copy
 */
extern "C" void cFinishLineControl_readFromReplayFrame(cFinishLineControl* self, cReplayFramePtr* obj)
{
    sVEntry0012C9F8* vt = *(sVEntry0012C9F8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_ResetWipeoutRecoverHit(void* self, void* a1);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cReplay_eventWipeout(void* self, int key, int id);
extern "C" void cWScriptMan_eventWipeout(void* self);
extern void* cWScriptMan_TheWScriptMan;
extern char t4vect_Zero[];

struct sVec4_12CA30
{
    float x, y, z, w;
} __attribute__((aligned(16)));


extern "C" void cWipeoutMotion_throwItem(char* self, sVec4_12CA30* a, sVec4_12CA30* b, tPose* c);

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_12CA30(sVec4_12CA30& dst, const sVec4_12CA30& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

struct sAiObj12CA30
{
    int f0;                 // 0x00
    char pad4[0xC];
    tPose m10;    // 0x10
    tPose m30;    // 0x30
    char pad50[0x4];
    int f54;                // 0x54
    char pad58[0x18];
    int f70;                // 0x70
    char pad74[0xC];
    char* mRider;           // 0x80
};

/**
 * Enters the wipeout control: clears the power-up timers and recovery, and snapshots the hip and board poses.
 *
 * Zeroes mPowerUpBoostTimer, mPowerUpTrickTimer, IsNewImpact and RecoverPercent, clears the audio word of the
 * rider's view (cSSXAudio_ResetWipeoutRecoverHit), copies the hip and board-child bone poses (plus mModelOffset) into
 * RiderPrevPose and BoardPrevPose, and sets RiderState 0 (Launch). When cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) is 0x16
 * it calls cWipeoutMotion_throwItem (rider velocity, no spin, BoardPrevPose) and clears LegIK. Then
 * notifies the replay (cReplay_eventWipeout) and queues world-script op 0x70004 (cWScriptMan_eventWipeout).
 */
extern "C" void cWipeoutControl_gainFocus(cWipeoutControl* self)
{
    tPose* boardPrevPose = &(*(&self->BoardPrevPose));
    *(int*)&self->mRider->mPowerUpTrickTimer = 0;
    *(int*)&self->mRider->mPowerUpBoostTimer = 0;
    self->IsNewImpact = 0;
    *(int*)&self->RecoverPercent = 0;
    cSSXAudio_ResetWipeoutRecoverHit(cAudioInterface_bxAudio(), (char*)self->mRider);
    cRider* rider = self->mRider;
    (*(&self->RiderPrevPose)) = ((tPose*)rider->mAnimModel->bones)[rider->mBoneHips];
    (*(&self->BoardPrevPose)) = ((tPose*)rider->mAnimModel->bones)[rider->mBoneBoardChild];
    vu0AddEq_12CA30(*(sVec4_12CA30*)&self->RiderPrevPose, *(sVec4_12CA30*)&self->mRider->mModelOffset);
    vu0AddEq_12CA30(*(sVec4_12CA30*)&self->BoardPrevPose, *(sVec4_12CA30*)&self->mRider->mModelOffset);
    self->RiderState = 0;
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 22)
    {
        cWipeoutMotion_throwItem((char*)&self->mRider->mRiderHandler->WipeoutMotion, (sVec4_12CA30*)(&self->mRider->mVelocity), (sVec4_12CA30*)t4vect_Zero, boardPrevPose);
        *(int*)&self->mRider->LegIK = 0;
    }
    cReplay_eventWipeout(SSXApp->Game->Replay, self->mRider->mAIIndex, (int)self->mRider->mMetrix + 0xFC);
    cWScriptMan_eventWipeout(cWScriptMan_TheWScriptMan);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" int cRider_checkForReset_CB68(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayWipeoutRecoverFull(void* snd, void* rider);
extern "C" void cSSXAudio_PlayWipeoutRecoverHit(void* snd, void* rider, float t);
extern "C" void cWipeoutControl_updateLaunch_CB68(void* self) __asm__("cWipeoutControl_updateLaunch");
extern "C" void cWipeoutControl_updateAir(cWipeoutControl* self);
extern "C" void cWipeoutControl_updateGround(cWipeoutControl* self);
extern "C" void cWipeoutControl_updateRecover(cWipeoutControl* self);
extern "C" void cWipeoutControl_updateStuck(cWipeoutControl* self);


/**
 * Runs the wipeout control for one tick: reset check, the recovery button meter, then the wipeout state.
 *
 * Stops any boost; returns early when cRider_checkForReset() (command bit 12 = reset input) starts a reset.
 * Unless recovering (RiderState 3), RecoverPercent decays toward 0 by 0.00417 per tick and each tick with bit
 * 13 set adds 17/60 (8/60 when rider unk_B20 is 0, 34/60 when 2) with a recover-hit or, at 1.0, recover-full
 * sound. Then by RiderState: updateLaunch, updateAir, updateGround, updateRecover or updateStuck.
 *
 * @param input  the wipeout command word (On Tour tWipeoutCmd)
 */
// Decays or charges the AI's 0x70 meter (updating its sound), then runs the current state's handler.
extern "C" void cWipeoutControl_update(cWipeoutControl* self, int* input)
{
    cRider_setupBoostWarp((char*)self->mRider, 0, 0);
    if (cRider_checkForReset_CB68((char*)self->mRider, (*input >> 12) & 1, 0)) {
        return;
    }
    bool isRecovering = self->RiderState == 3;
    if (!isRecovering) {
        float prevRecover = self->RecoverPercent;
        float newRecover;
        if (prevRecover > 0.00416986271739006f) {
            newRecover = prevRecover - 0.00416986271739006f;
        } else {
            newRecover = 0.0f;
            if (prevRecover < -0.00416986271739006f) {
                newRecover = prevRecover + 0.00416986271739006f;
            }
        }
        self->RecoverPercent = newRecover;
        if (*input & 0x2000) {
            float rate = 17.00261688232422f;
            switch (self->mRider->unk_B20) {
            case 0:
                rate = 8.001495361328125f;
                break;
            case 2:
                rate = 34.00276565551758f;
                break;
            }
            self->RecoverPercent += rate * 0.01666666753590107f;
            if (self->RecoverPercent >= 1.0f) {
                cSSXAudio_PlayWipeoutRecoverFull(cAudioInterface_bxAudio(), (char*)self->mRider);
            } else {
                cSSXAudio_PlayWipeoutRecoverHit(cAudioInterface_bxAudio(), (char*)self->mRider, self->RecoverPercent);
            }
        }
    }
    switch (self->RiderState) {
    case 0:
        cWipeoutControl_updateLaunch_CB68((self));
        break;
    case 1:
        cWipeoutControl_updateAir(self);
        break;
    case 2:
        cWipeoutControl_updateGround(self);
        break;
    case 3:
        cWipeoutControl_updateRecover(self);
        break;
    case 4:
        cWipeoutControl_updateStuck(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_updateLaunch);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(void* self, int i);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(void* self, int i);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cWipeoutControl_playWipeoutHitTransitionAnim(cWipeoutControl* self);
extern "C" void cWipeoutMotion_throwRider(char* self, void* vel, void* angvel);
extern "C" void cSSXAudio_WipeoutSlideStart(void* mgr, void* self);
extern "C" void cSSXAudio_WipeoutTumbleStart(void* mgr, void* self);

struct sV_12CD20
{
    float x, y, z, w;
    sV_12CD20() {}
    sV_12CD20(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));



// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_12CD20 Sub_12CD20(const sV_12CD20& a, const sV_12CD20& b)
{
    sV_12CD20 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV_12CD20 Scale_12CD20(const sV_12CD20& v, float s)
{
    sV_12CD20 r;
    int t;
    __asm__(
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

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sV_12CD20 QuatMul_12CD20(const sV_12CD20& a, const sV_12CD20& b)
{
    sV_12CD20 r;
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

static inline sV_12CD20 Conj_12CD20(const tPose& p)
{
    sV_12CD20 r;
    r.w = (*(sV_12CD20*)&p.Q).w;
    r.x = (*(sV_12CD20*)&p.Q).x;
    r.y = (*(sV_12CD20*)&p.Q).y;
    r.z = (*(sV_12CD20*)&p.Q).z;
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

/**
 * Runs the launch phase of a wipeout (RS_Launch): when the crash clip ends, throws the body and switches to
 * the air or ground tumble.
 *
 * When the main-channel (2) clip completes, an angular velocity from the last two poses of the hips bone
 * (mBoneHips) goes to cWipeoutMotion_throwRider() with the rider's unchanged mVelocity; a class-0x17 clip
 * also throws the board from the mBoneBoardChild poses (cWipeoutMotion_throwItem). RiderState then becomes
 * RS_Air or RS_Ground from the wipeout motion's state, the hit-transition clip plays, and on the ground the
 * slide sound (cSSXAudio_WipeoutSlideStart, classes 0x1D/0x19) or the tumble sound (cSSXAudio_WipeoutTumbleStart) starts. Before that, the
 * two poses and the clip progress are recorded each tick while the progress is below 0.95. A linear hip
 * velocity is also computed but never used (On Tour's calcVelFromAnim, inlined).
 */
extern "C" void cWipeoutControl_updateLaunch(cWipeoutControl* self)
{
    cRider* rider = self->mRider;
    tPose* nodes = (tPose*)rider->mAnimModel->bones;
    tPose* pose = &nodes[rider->mBoneHips];
    tPose* boardPose = &nodes[rider->mBoneBoardChild];
    float framePercent = cRiderAnimBase_getCurrentAnimFramePercent(rider->mRiderAnim, 2);
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2))
    {
        char* an = cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2);
        float rate = *(float*)(an + 0x90);
        float invDeltaTime = rate / ((framePercent - self->PrevFramePercent) * *(float*)(an + 0x10));
        sV_12CD20 vel = Scale_12CD20(Scale_12CD20(Sub_12CD20(*(sV_12CD20*)&pose->P, *(sV_12CD20*)&self->RiderPrevPose), *(float*)((char*)self->mRider->mRiderAnim + 0x1C)), invDeltaTime);
        float doubleInvDeltaTime = invDeltaTime + invDeltaTime;
        sV_12CD20 deltaQuat = Sub_12CD20(*(sV_12CD20*)&pose->Q, *(sV_12CD20*)&self->RiderPrevPose.Q);
        sV_12CD20 angVelQuat = QuatMul_12CD20(deltaQuat, Conj_12CD20(*pose));
        sV_12CD20 angvel = Scale_12CD20(sV_12CD20(angVelQuat.x, angVelQuat.y, angVelQuat.z, 0.0f), doubleInvDeltaTime);
        cWipeoutMotion_throwRider((char*)&self->mRider->mRiderHandler->WipeoutMotion, &self->mRider->mVelocity, &angvel);
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 23)
        {
            vel = Scale_12CD20(Scale_12CD20(Sub_12CD20(*(sV_12CD20*)&boardPose->P, *(sV_12CD20*)&self->BoardPrevPose), *(float*)((char*)self->mRider->mRiderAnim + 0x1C)), invDeltaTime);
            deltaQuat = Sub_12CD20(*(sV_12CD20*)&boardPose->Q, *(sV_12CD20*)&self->BoardPrevPose.Q);
            angVelQuat = QuatMul_12CD20(deltaQuat, Conj_12CD20(*boardPose));
            angvel = Scale_12CD20(sV_12CD20(angVelQuat.x, angVelQuat.y, angVelQuat.z, 0.0f), doubleInvDeltaTime);
            cWipeoutMotion_throwItem((char*)&self->mRider->mRiderHandler->WipeoutMotion, (sVec4_12CA30*)&vel, (sVec4_12CA30*)&angvel, boardPose);
        }
        if (self->mRider->mRiderHandler->WipeoutMotion.WipeoutMotionState == 1)
        {
            self->RiderState = 1;
            cWipeoutControl_playWipeoutHitTransitionAnim(self);
        }
        else
        {
            self->RiderState = 2;
            cWipeoutControl_playWipeoutHitTransitionAnim(self);
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 29 ||
                cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 25)
                cSSXAudio_WipeoutSlideStart(cAudioInterface_bxAudio(), (self));
            else
                cSSXAudio_WipeoutTumbleStart(cAudioInterface_bxAudio(), (self));
        }
        return;
    }
    if (framePercent < 0.949999988079071f)
    {
        self->PrevFramePercent = framePercent;
        (*(&self->RiderPrevPose)) = *pose;
        (*(&self->BoardPrevPose)) = *boardPose;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_updateGround);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventWipeoutRecover(void* rider, int flag);
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cWipeoutControl_playWipeoutAirAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutGroundAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutGroundRecoverAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutStuckAnim(cWipeoutControl* self);
extern "C" float cWipeoutControl_chooseTumbleAnimRate(cWipeoutControl* self);
extern "C" void cCamera_shakeCamera(void* obj, int mode, float a, float b);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_WipeoutSlideEnd(void* mgr, void* rider);
extern "C" void cSSXAudio_WipeoutTumbleEnd(void* mgr, void* rider);
extern "C" void cSSXAudio_PlayGrunt(void* mgr, void* rider, int a, int b);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);

struct sVEntry12D160 { short delta; short index; void (*fn)(void*, float); };

static inline bool aiOk_12D160(void* self)
{
    return *(float*)(*(char**)((char*)self + 0x80) + 0x470) >= 0.0f;
}

static inline int aiIsOne_12D160(char* r)
{
    return *(int*)(r + 0x150) == 1;
}

static inline float aiRatio_12D160(float v, float hi)
{
    float t = 0.0f;
    if (v >= 0.0f)
    {
        t = 1.0f;
        if (v <= hi)
            t = v / hi;
    }
    return t;
}

static inline void aiTail_12D160(void* self)
{
    cSSXAudio_WipeoutSlideEnd(cAudioInterface_bxAudio(), *(char**)((char*)self + 0x80));
    cSSXAudio_WipeoutTumbleEnd(cAudioInterface_bxAudio(), *(char**)((char*)self + 0x80));
}

/**
 * Runs the ground phase of a wipeout (RS_Ground): slides or tumbles until the rider can get up, sticks, or is
 * airborne again.
 *
 * Every tick: camera shake for a local rider with a view (eCST_Wipeout, km/h / 100 capped at 1), rumble at
 * half the tumble rate, and a new impact becomes a vibration bump that ends the tick. A detached board
 * (mUseRootPoseOverride == 1) under 1 km/h goes to RS_Stuck. After the clip ends (at once for classes 0x1D
 * and 0x19), a full recovery meter or finished race, or an attached board under 25 km/h (60 km/h for class
 * 0x1C), gets the rider up (RS_Recover, the recover event cRider_flagEventWipeoutRecover, a long grunt for 0x1C); on
 * eST_Rock_WipeOut it instead resets (eAR_UberWipeout) when recovery is allowed or goes to RS_Stuck under 10
 * km/h, and a detached board resets once recovery is allowed. An airborne wipeout motion goes to RS_Air;
 * otherwise a finished clip re-runs playWipeoutGroundAnim(). These changes stop the slide and tumble sounds,
 * which also stop on rock between 10 km/h and the get-up limit although the state stays RS_Ground.
 */
extern "C" void cWipeoutControl_updateGround(cWipeoutControl* self)
{
    cRider* rider = self->mRider;
    float speed = aiLength_12C5C8(*(sAiVec4_12C5C8*)&rider->mVelocity);
    if (rider->mViewIndex >= 0)
    {
        float speedScalar = speed * 0.035999998450279236f;
        float shakeRatio = aiRatio_12D160(speedScalar, 100.0f);
        cRider* rider2 = self->mRider;
        if (rider2->mIsLocal != 0)
        {
            cCamera_shakeCamera(*(void**)((char*)SSXApp->Game->ViewMan + (rider2->mViewIndex << 2) + 4), 4, shakeRatio, 0.0f);
        }
    }
    float k = cWipeoutControl_chooseTumbleAnimRate(self);
    {
        cRider* rider3 = self->mRider;
        sVEntry12D160* vt = (sVEntry12D160*)rider3->mVtbl;
        vt[18].fn(((char*)rider3) + vt[18].delta, k * 0.5f);
    }
    if (self->IsNewImpact != 0)
    {
        self->IsNewImpact = 0;
        cRider* rider4 = self->mRider;
        sVEntry12D160* vt = (sVEntry12D160*)rider4->mVtbl;
        vt[17].fn(((char*)rider4) + vt[17].delta, aiLength_12C5C8(*(sAiVec4_12C5C8*)&self->LatestImpact));
        return;
    }
    if (aiIsOne_12D160((char*)self->mRider) && speed < 27.77777862548828f)
    {
        cWipeoutControl_playWipeoutStuckAnim(self);
        *(int*)self = 4;
        aiTail_12D160(self);
        return;
    }
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) == 0 &&
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) != 29 &&
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) != 25)
        return;
    float lim = 694.4444580078125f;
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 0x1C)
        lim = 1666.666748046875f;
    int fast = self->RecoverPercent >= 1.0f || aiOk_12D160(self);
    if ((!aiIsOne_12D160((char*)self->mRider) && speed < lim) || fast)
    {
        if (self->mRider->mSurfaceType == 18 || aiIsOne_12D160((char*)self->mRider))
        {
            if (fast)
            {
                cRider_checkForReset((char*)self->mRider, 0, 3);
                cRider_flagEventWipeoutRecover((char*)self->mRider, 1);
            }
            else if (speed < 277.77777099609375f)
            {
                cWipeoutControl_playWipeoutStuckAnim(self);
                *(int*)self = 4;
            }
        }
        else
        {
            cRider_flagEventWipeoutRecover((char*)self->mRider, fast);
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 0x1C)
                cSSXAudio_PlayGrunt(cAudioInterface_bxAudio(), (char*)self->mRider, 1, 0);
            cWipeoutControl_playWipeoutGroundRecoverAnim(self);
            *(int*)self = 3;
        }
        aiTail_12D160(self);
    }
    else if (self->mRider->mRiderHandler->WipeoutMotion.WipeoutMotionState == 1)
    {
        cWipeoutControl_playWipeoutAirAnim(self);
        *(int*)self = 1;
        aiTail_12D160(self);
    }
    else
    {
        if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) != 0)
            cWipeoutControl_playWipeoutGroundAnim(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_updateAir);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventWipeoutRecover(void* rider, int flag);
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cWipeoutControl_playWipeoutAirAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutGroundAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutAirRecoverAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutStuckAnim(cWipeoutControl* self);
extern "C" void cWipeoutControl_playWipeoutGroundImpactAnim(cWipeoutControl* self);
extern "C" float cWipeoutControl_chooseTumbleAnimRate(cWipeoutControl* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_WipeoutHit(void* mgr, void* rider);
extern "C" void cSSXAudio_WipeoutSlideStart(void* mgr, void* self);
extern "C" void cSSXAudio_WipeoutTumbleStart(void* mgr, void* self);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(void* self, int i);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" void cAnimSequence_setSubAnimTime(void* self, int idx, float v);

struct sAiVec4_12D4E8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float aiDot_12D4E8(const sAiVec4_12D4E8& a, const sAiVec4_12D4E8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sAiVec4_12D4E8 aiScale_12D4E8(const sAiVec4_12D4E8& in, float s)
{
    sAiVec4_12D4E8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(in), "f"(s));
    return r;
}

struct sVEntry12D4E8 { short delta; short index; void (*fn)(void*, float); };

static inline bool aiOk_12D4E8(void* self)
{
    return *(float*)(*(char**)((char*)self + 0x80) + 0x470) >= 0.0f;
}

static inline int aiIsOne_12D4E8(char* r)
{
    return *(int*)(r + 0x150) == 1;
}

/**
 * Runs the airborne phase of a wipeout (RS_Air): handles the body's landing impacts and starts the recovery
 * once it is allowed.
 *
 * Updates the tumble rate first (result unused). A new impact becomes a vibration bump; if the wipeout motion
 * is on the ground, a head-on hit on flat ground (mGroundNormal z > 0.9, impact within about 25 degrees of
 * straight down into it) goes to RS_Stuck and keeps only the normal part of mVelocity, else RS_Ground starts
 * with the ground-impact clip and slide sound (classes 0x1D/0x19) or the ground tumble at the same progress
 * and the tumble sound; both play the wipeout-hit sound (cSSXAudio_WipeoutHit). Without an impact, once the clip
 * ends (at once for 0x1D/0x19): a full meter or finished race recovers (RS_Recover) or, with the board
 * detached, resets (eAR_UberWipeout); else a grounded wipeout motion goes to RS_Ground and a finished air
 * clip is re-picked.
 */
extern "C" void cWipeoutControl_updateAir(cWipeoutControl* self)
{
    cWipeoutControl_chooseTumbleAnimRate(self);
    if (self->IsNewImpact != 0)
    {
        if (self->mRider->mRiderHandler->WipeoutMotion.WipeoutMotionState == 0)
        {
            if (self->mRider->mGroundNormal.z > 0.8999999761581421f &&
                aiDot_12D4E8(*(sAiVec4_12D4E8*)&self->LatestImpact, *(sAiVec4_12D4E8*)&self->mRider->mGroundNormal) <
                    aiLength_12C5C8(*(sAiVec4_12C5C8*)&self->LatestImpact) * -0.8999999761581421f)
            {
                cWipeoutControl_playWipeoutStuckAnim(self);
                cRider* rider2 = self->mRider;
                sAiVec4_12D4E8 normalVel = aiScale_12D4E8(*(sAiVec4_12D4E8*)&rider2->mGroundNormal,
                    aiDot_12D4E8(*(sAiVec4_12D4E8*)&rider2->mGroundNormal, *(sAiVec4_12D4E8*)&rider2->mVelocity));
                *(sAiVec4_12D4E8*)&rider2->mVelocity = normalVel;
                *(int*)self = 4;
            }
            else
            {
                if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 29 ||
                    cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 25)
                {
                    cWipeoutControl_playWipeoutGroundImpactAnim(self);
                    cSSXAudio_WipeoutSlideStart(cAudioInterface_bxAudio(), self);
                }
                else
                {
                    float framePercent = cRiderAnimBase_getCurrentAnimFramePercent(self->mRider->mRiderAnim, 2);
                    cWipeoutControl_playWipeoutGroundAnim(self);
                    float animDuration = *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x10);
                    cAnimSequence_setSubAnimTime(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2), 0, animDuration * framePercent);
                    cSSXAudio_WipeoutTumbleStart(cAudioInterface_bxAudio(), self);
                }
                *(int*)self = 2;
            }
            cSSXAudio_WipeoutHit(cAudioInterface_bxAudio(), self->mRider);
        }
        self->IsNewImpact = 0;
        cRider* rider3 = self->mRider;
        sVEntry12D4E8* vt = (sVEntry12D4E8*)rider3->mVtbl;
        vt[17].fn(((char*)rider3) + vt[17].delta, aiLength_12C5C8(*(sAiVec4_12C5C8*)&self->LatestImpact));
        return;
    }
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) != 0 ||
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 29 ||
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 25)
    {
        int fast = self->RecoverPercent >= 1.0f || aiOk_12D4E8(self);
        if (fast)
        {
            if (!aiIsOne_12D4E8((char*)self->mRider))
            {
                cRider_flagEventWipeoutRecover((char*)self->mRider, fast);
                cWipeoutControl_playWipeoutAirRecoverAnim(self);
                *(int*)self = 3;
            }
            else
            {
                cRider_checkForReset((char*)self->mRider, 0, 3);
                cRider_flagEventWipeoutRecover(self->mRider, 1);
            }
        }
        else if (self->mRider->mRiderHandler->WipeoutMotion.WipeoutMotionState == 0)
        {
            cWipeoutControl_playWipeoutGroundAnim(self);
            *(int*)self = 2;
        }
        else if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) != 0)
        {
            cWipeoutControl_playWipeoutAirAnim(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_updateRecover);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" void cSSXAudio_Land(void* mgr, void* rider, float v);
extern "C" void cRiderMetrix_evTakeoff(void* p, int flip, int c);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

struct sVEntry12D848 { short delta; short index; void (*fn)(void*, float); };

/**
 * Runs the get-up phase of a wipeout (RS_Recover) and hands the rider back to normal control when the
 * recovery clip ends.
 *
 * A new impact gives a vibration bump and the landing sound (cSSXAudio_Land, 0 boost). While the wipeout
 * motion is on the ground the rumble is 2 * (1 - clip progress). When the main clip completes, on the ground
 * it plays semantic 5 and requests CS_Cruise and MS_Ground; in the air it starts a new takeoff in the trick
 * score (cRiderMetrix_evTakeoff, flag: IsRidingFakie differs from IsGoofyStance), plays the air cycle 0x11F and requests
 * CS_Spin and MS_Air.
 */
extern "C" void cWipeoutControl_updateRecover(cWipeoutControl* self)
{
    if (self->IsNewImpact != 0)
    {
        self->IsNewImpact = 0;
        cRider* rider = self->mRider;
        sVEntry12D848* vt = (sVEntry12D848*)rider->mVtbl;
        vt[17].fn(((char*)rider) + vt[17].delta, aiLength_12C5C8(*(sAiVec4_12C5C8*)&self->LatestImpact));
        cSSXAudio_Land(cAudioInterface_bxAudio(), self->mRider, 0.0f);
    }
    cRider* rider = self->mRider;
    if (rider->mRiderHandler->WipeoutMotion.WipeoutMotionState == 0)
    {
        sVEntry12D848* vt = (sVEntry12D848*)rider->mVtbl;
        char* thisp = ((char*)rider) + vt[18].delta;
        float percentLeft = 1.0f - cRiderAnimBase_getCurrentAnimFramePercent(rider->mRiderAnim, 2);
        vt[18].fn(thisp, percentLeft + percentLeft);
    }
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) != 0)
    {
        cRider* rider2 = self->mRider;
        if (rider2->mRiderHandler->WipeoutMotion.WipeoutMotionState == 0)
        {
            cRiderAnimBase_play(rider2->mRiderAnim, 5, 0, -1.0f);
            cRider_setControlState_impl(self->mRider, 0);
            cRider_setMotionState_impl(self->mRider, 0);
        }
        else
        {
            cRiderMetrix_evTakeoff(rider2->mMetrix, rider2->IsRidingFakie != rider2->IsGoofyStance, 0);
            cRiderAnimBase_play(self->mRider->mRiderAnim, 287, 0, -1.0f);
            cRider_setControlState_impl(self->mRider, 5);
            cRider_setMotionState_impl(self->mRider, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_updateStuck);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventWipeoutRecover(void* rider, int flag);
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);

static inline int AiCheck_0012D9D8(char* rider)
{
    return *(float*)(rider + 0x470) >= 0.0f;
}

/**
 * Runs the stuck phase of a wipeout (RS_Stuck): ends it with a reset when the stuck clip finishes or recovery
 * is allowed.
 *
 * Recovery is allowed when RecoverPercent >= 1 or the race is finished (mFinishTime >= 0). Then cRider_flagEventWipeoutRecover
 * (probably On Tour's flagEventWipeoutRecover; the RECOVERED! bonus only when recovery was allowed) runs,
 * followed by cRider_checkForReset() with eAR_Stuck (2).
 */
extern "C" void cWipeoutControl_updateStuck(cWipeoutControl* self)
{
    int flag = 0;
    if (self->RecoverPercent >= 1.0f || AiCheck_0012D9D8((char*)self->mRider))
    {
        flag = 1;
    }
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) || flag)
    {
        cRider_flagEventWipeoutRecover(self->mRider, flag);
        cRider_checkForReset(self->mRider, 0, 2);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutHitTransitionAnim);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
/**
 * Swaps the crash clip on the main channel for the matching tumble clip, the air or the ground variant.
 *
 * Crash semantics 0x148-0x169 map to air tumbles 0x175-0x17F when RiderState is RS_Air, else to ground
 * tumbles 0x16A-0x174 (several crash clips share a tumble). A changed clip is played (blend -1) and its rate
 * set with cWipeoutControl_chooseTumbleAnimRate().
 */
// Maps the rider's layer-2 animation to its base (or alternate) variant and replays it.
extern "C" void cWipeoutControl_playWipeoutHitTransitionAnim(cWipeoutControl* self)
{
    bool isAir = *(int*)self == 1;
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int next = cur;
    switch (cur) {
    case 328:
        next = isAir ? 373 : 362;
        break;
    case 329:
        next = isAir ? 374 : 363;
        break;
    case 330:
        next = isAir ? 375 : 364;
        break;
    case 331:
        next = isAir ? 379 : 368;
        break;
    case 332:
        next = isAir ? 377 : 366;
        break;
    case 333:
        next = isAir ? 378 : 367;
        break;
    case 334:
        next = isAir ? 381 : 370;
        break;
    case 335:
        next = isAir ? 382 : 371;
        break;
    case 336:
        next = isAir ? 376 : 365;
        break;
    case 337:
        next = isAir ? 380 : 369;
        break;
    case 338:
        next = isAir ? 373 : 362;
        break;
    case 339:
        next = isAir ? 374 : 363;
        break;
    case 340:
        next = isAir ? 375 : 364;
        break;
    case 341:
        next = isAir ? 379 : 368;
        break;
    case 342:
        next = isAir ? 377 : 366;
        break;
    case 343:
        next = isAir ? 382 : 371;
        break;
    case 344:
        next = isAir ? 380 : 369;
        break;
    case 345:
        next = isAir ? 376 : 365;
        break;
    case 346:
        next = isAir ? 378 : 367;
        break;
    case 347:
        next = isAir ? 381 : 370;
        break;
    case 348:
        next = isAir ? 379 : 368;
        break;
    case 349:
        next = isAir ? 380 : 369;
        break;
    case 350:
        next = isAir ? 375 : 364;
        break;
    case 351:
        next = isAir ? 377 : 366;
        break;
    case 352:
        next = isAir ? 382 : 371;
        break;
    case 353:
        next = isAir ? 379 : 368;
        break;
    case 354:
        next = isAir ? 379 : 368;
        break;
    case 355:
        next = isAir ? 381 : 370;
        break;
    case 356:
        next = isAir ? 374 : 363;
        break;
    case 357:
        next = isAir ? 373 : 362;
        break;
    case 358:
        next = isAir ? 379 : 368;
        break;
    case 359:
        next = isAir ? 381 : 370;
        break;
    case 360: case 361:
        next = isAir ? 383 : 372;
        break;
    }
    if (next != cur) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, next, 0, -1.0f);
        cWipeoutControl_chooseTumbleAnimRate(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutAirAnim);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
/**
 * Switches the main channel to the airborne tumble clip that matches the current ground clip.
 *
 * With the board detached (mUseRootPoseOverride == 1) the clip is always 0x17F. Otherwise ground tumbles
 * 0x16A-0x173 and ground impacts 0x180-0x183 map to air tumbles 0x175-0x17E; other clips stay. A changed clip
 * is played (blend -1) and its rate set with cWipeoutControl_chooseTumbleAnimRate().
 */
// Picks the rider's layer-2 follow-up animation and replays it.
extern "C" void cWipeoutControl_playWipeoutAirAnim(cWipeoutControl* self)
{
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int next = cur;
    bool useRootPose = self->mRider->mUseRootPoseOverride == 1;
    if (!useRootPose) {
        switch (cur) {
        case 362: next = 373; break;
        case 363: next = 374; break;
        case 364: next = 375; break;
        case 365: next = 376; break;
        case 366: next = 377; break;
        case 367: next = 378; break;
        case 368: case 0x180: next = 379; break;
        case 369: case 385: next = 380; break;
        case 370: case 386: next = 381; break;
        case 371: case 387: next = 382; break;
        case 372: case 373: case 374: case 375: case 376: case 377:
        case 378: case 379: case 380: case 381: case 382: case 383:
            break;
        }
    } else {
        next = 383;
    }
    if (next != cur) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, next, 0, -1.0f);
        cWipeoutControl_chooseTumbleAnimRate(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutGroundAnim);
#ifdef SKIP_ASM
/**
 * Switches the main channel to the ground tumble clip that matches the current air clip.
 *
 * The reverse of cWipeoutControl_playWipeoutAirAnim(): a detached board gives 0x174; otherwise air tumbles
 * 0x175-0x17E and ground impacts 0x180-0x183 map to ground tumbles 0x16A-0x173. A changed clip is played
 * (blend -1) and its rate set with cWipeoutControl_chooseTumbleAnimRate().
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// Reverse of cWipeoutControl_playWipeoutAirAnim: maps follow-up animations back to their base.
extern "C" void cWipeoutControl_playWipeoutGroundAnim(cWipeoutControl* self)
{
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int next = cur;
    bool useRootPose = self->mRider->mUseRootPoseOverride == 1;
    if (!useRootPose) {
        switch (cur) {
        case 373: next = 362; break;
        case 374: next = 363; break;
        case 375: next = 364; break;
        case 376: next = 365; break;
        case 377: next = 366; break;
        case 378: next = 367; break;
        case 379: case 0x180: next = 368; break;
        case 380: case 385: next = 369; break;
        case 381: case 386: next = 370; break;
        case 382: case 387: next = 371; break;
        case 383:
            break;
        }
    } else {
        next = 372;
    }
    if (next != cur) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, next, 0, -1.0f);
        cWipeoutControl_chooseTumbleAnimRate(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutAirRecoverAnim);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
/**
 * Plays the in-air recovery clip that matches the current air tumble.
 *
 * Maps air tumbles 0x175-0x17E to recovery clips 0x18F-0x198 on the main channel (blend -1); any other clip
 * is left playing.
 */
// Remaps the rider's current layer-2 animation to its follow-up and plays it.
extern "C" void cWipeoutControl_playWipeoutAirRecoverAnim(cWipeoutControl* self)
{
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int next = cur;
    switch (cur) {
    case 373:
        next = 399;
        break;
    case 374:
        next = 400;
        break;
    case 375:
        next = 401;
        break;
    case 376:
        next = 402;
        break;
    case 377:
        next = 403;
        break;
    case 378:
        next = 404;
        break;
    case 379:
        next = 405;
        break;
    case 380:
        next = 406;
        break;
    case 381:
        next = 407;
        break;
    case 382:
        next = 408;
        break;
    }
    if (next != cur) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, next, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutGroundRecoverAnim);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
/**
 * Plays the get-up clip that matches the current ground tumble.
 *
 * Maps ground tumbles 0x16A-0x173 and ground impacts 0x180-0x183 to get-up clips 0x185-0x18E on the main
 * channel (blend -1); other clips, including the detached-board tumbles 0x174 and 0x17F, are left playing.
 */
// Remaps the rider's current layer-2 animation to its follow-up and plays it.
extern "C" void cWipeoutControl_playWipeoutGroundRecoverAnim(cWipeoutControl* self)
{
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int next = cur;
    switch (cur) {
    case 362:
        next = 389;
        break;
    case 363:
        next = 390;
        break;
    case 364:
        next = 391;
        break;
    case 365:
        next = 392;
        break;
    case 366:
        next = 393;
        break;
    case 367:
        next = 394;
        break;
    case 368: case 0x180:
        next = 395;
        break;
    case 369: case 385:
        next = 396;
        break;
    case 370: case 386:
        next = 397;
        break;
    case 371: case 387:
        next = 398;
        break;
    case 372: case 373: case 374: case 375: case 376: case 377: case 378: case 379: case 380: case 381: case 382: case 383:
        break;
    }
    if (next != cur) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, next, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutStuckAnim);
#ifdef SKIP_ASM
// D_004A4C08 is its own 8-byte config struct (config/symbol_addrs.txt), not D_004A4C00 + 8.
struct sCfg_E010 { int a; int b; };
extern sCfg_E010 D_004A4C08_E010 __asm__("D_004A4C08");
extern "C" void cRiderAnimBase_getAIAnimFrame(void* buf, void* anim, int id, int a3, float t);
extern "C" void func_0030ECD8(void* xf, void* model, void* buf, int a3);
extern "C" void cRiderAnimBase_changeOrientationOffset(void* self, void* xf);
extern int D_004A4C00;
extern "C" void cRider_updateOrientationImplicit(void*);

struct sXf_12E010
{
    sQuat_12BE20 pos;
    sQuat_12BE20 rot;
};

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_12BE20 QuatMul_12E010(const sQuat_12BE20& a, const sQuat_12BE20& b)
{
    sQuat_12BE20 r;
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sQuat_12BE20 Sub_12E010(const sQuat_12BE20& a, const sQuat_12BE20& b)
{
    sQuat_12BE20 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sQuat_12BE20 Add_12E010(const sQuat_12BE20& a, const sQuat_12BE20& b)
{
    sQuat_12BE20 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sQuat_12BE20 Rot_12E010(const sQuat_12BE20& q, const sQuat_12BE20& v)
{
    sQuat_12BE20 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sQuat_12BE20 Scale_12E010(const sQuat_12BE20& v, float s)
{
    sQuat_12BE20 r;
    int t;
    __asm__(
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

static inline sQuat_12BE20 Conj_12E010(const sXf_12E010& p)
{
    sQuat_12BE20 r;
    r.w = p.rot.w;
    r.x = p.rot.x;
    r.y = p.rot.y;
    r.z = p.rot.z;
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

static inline sXf_12E010 Invert_12E010(const sXf_12E010& a)
{
    sXf_12E010 r;
    r.rot = Conj_12E010(a);
    r.pos = Add_12E010(Scale_12E010(Rot_12E010(r.rot, Sub_12E010(a.pos, D_004FF130_12BE20)), -1.0f), D_004FF130_12BE20);
    return r;
}

static inline sXf_12E010 Combine_12E010(const sXf_12E010& a, const sXf_12E010& b)
{
    sXf_12E010 r;
    r.rot = QuatMul_12E010(a.rot, b.rot);
    r.pos = Add_12E010(Rot_12E010(a.rot, Sub_12E010(b.pos, D_004FF130_12BE20)), a.pos);
    return r;
}

/**
 * Plays the stuck clip (0x199 with the board detached, else 0x19A) with its root re-based so the body does
 * not jump.
 *
 * Samples the clip's first frame (cRiderAnimBase_getAIAnimFrame, func_0030ECD8), composes the hips bone's
 * current scaled local root with the inverse of that frame's root, applies the result to the rider's
 * mPosition/mRotation (then cRider_updateOrientationImplicit) and passes its inverse to
 * cRiderAnimBase_changeOrientationOffset() before playing the clip (blend -1).
 */
extern "C" void cWipeoutControl_playWipeoutStuckAnim(cWipeoutControl* self)
{
    cRider* rider = self->mRider;
    int anim = 409;
    if (rider->mUseRootPoseOverride != 1)
        anim = 410;
    char buf[80];
    cRiderAnimBase_getAIAnimFrame(buf, rider->mRiderAnim, anim, D_004A4C08_E010.a, 0.0f);
    sXf_12E010 newPose;
    func_0030ECD8(&newPose, self->mRider->mAnimModel, buf, 0);
    cRider* rd = self->mRider;
    cAnimModel* trk = rd->mAnimModel;
    sQuat_12BE20* n = (sQuat_12BE20*)((rd->mBoneHips << 4) + (int)trk->localPos);
    sXf_12E010 node;
    node.pos = sQuat_12BE20(n->x * trk->unk_140.x, n->y * trk->unk_140.y,
                            n->z * trk->unk_140.z, n->w * trk->unk_140.w);
    node.rot = *(sQuat_12BE20*)((char*)rd->mAnimModel->localRot + (rd->mBoneHips << 4));
    sXf_12E010 y = Combine_12E010(node, Invert_12E010(newPose));
    sXf_12E010* rx = (sXf_12E010*)(&self->mRider->mPosition);
    *rx = Combine_12E010(*rx, y);
    cRider_updateOrientationImplicit(self->mRider);
    cRider* rd3 = self->mRider;
    sXf_12E010 w = Invert_12E010(y);
    cRiderAnimBase_changeOrientationOffset(rd3->mRiderAnim, &w);
    cRiderAnimBase_play(self->mRider->mRiderAnim, anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_playWipeoutGroundImpactAnim);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Plays the ground-impact clip that follows the current air tumble.
 *
 * Maps air tumbles 0x17B-0x17F to impact clips 0x180-0x184 on the main channel (blend -1); for any other clip
 * nothing is played.
 */
extern "C" void cWipeoutControl_playWipeoutGroundImpactAnim(cWipeoutControl* self)
{
    switch (cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2)) {
    case 379:
        cRiderAnimBase_play(self->mRider->mRiderAnim, 0x180, 0, -1.0f);
        break;
    case 380:
        cRiderAnimBase_play(self->mRider->mRiderAnim, 385, 0, -1.0f);
        break;
    case 381:
        cRiderAnimBase_play(self->mRider->mRiderAnim, 386, 0, -1.0f);
        break;
    case 382:
        cRiderAnimBase_play(self->mRider->mRiderAnim, 387, 0, -1.0f);
        break;
    case 383:
        cRiderAnimBase_play(self->mRider->mRiderAnim, 388, 0, -1.0f);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_chooseTumbleAnimRate);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);

static inline bool aiOk_12E528(void* self)
{
    return *(float*)(*(char**)((char*)self + 0x80) + 0x470) >= 0.0f;
}

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_12E528(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

/**
 * Sets the tumble clip's playback rate from how fast the body spins, and returns that rate.
 *
 * rate = |RiderAngVelocity| (the wipeout motion's, rider handler + 0x60) * clip duration / 2pi, the turns per
 * clip length, clamped to 0.5..2. The main sequence's rate (+0x90) becomes 1 for classes 0x1D/0x19, max(rate,
 * 2) when recovery is allowed (meter full or race finished), else rate; as rate is at most 2, the recovery
 * case is always 2.
 *
 * @return the clamped rate (0.5..2), before the class and recovery adjustments
 */
extern "C" float cWipeoutControl_chooseTumbleAnimRate(cWipeoutControl* self)
{
    cRider* rider = self->mRider;
    cRiderHandler* handler = rider->mRiderHandler;
    char* animSeq = cRiderAnimBase_getCurrentAnimSequence(rider->mRiderAnim, 2);
    float rawRate = aiLength_12C5C8(*(sAiVec4_12C5C8*)&handler->WipeoutMotion.RiderAngVelocity) * *(float*)(animSeq + 0x10) * 0.15915493667125702f;
    float res = aiClamp_12E528(rawRate, 0.5f, 2.0f);
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 29 ||
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 25)
    {
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 1.0f;
    }
    else if (self->RecoverPercent >= 1.0f || aiOk_12E528(self))
    {
        // PORT: g++ `>?` (max) operator.
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = res >? 2.0f;
    }
    else
    {
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = res;
    }
    return res;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", cWipeoutControl_loseFocus);
#ifdef SKIP_ASM
struct sVEntry0012E690 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int AITick();
extern "C" void cRider_flagEventReaction(void* self, int mode, float t);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_002A02D8(void*, void*);

/**
 * Leaves the wipeout control: on even logic ticks, asks for a negative rider reaction and the wipeout speech.
 *
 * Only when the logic tick (AITick) is even: a human rider (rider vtable slot 8, isHuman) gets
 * reaction kREACT_Neg (cRider_flagEventReaction, kind 4, no delay), then func_002A02D8 requests the Wipeout_* speech. On
 * Tour's loseFocus also takes the next control state; this body has no such parameter.
 */
extern "C" void cWipeoutControl_loseFocus(cWipeoutControl* self)
{
    if ((AITick() & 1) == 0)
    {
        char* sub = (char*)&self->mRider->mVtbl;
        sVEntry0012E690* vt = *(sVEntry0012E690**)sub;
        if (vt[8].fn(sub + vt[8].delta))
        {
            cRider_flagEventReaction(self->mRider, 4, 0.0f);
        }
        func_002A02D8(cAudioInterface_bxAudio(), self->mRider);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012E708 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the wipeout control's 0x80-byte state to a replay frame.
 *
 * Passes the cWipeoutControlState part (RiderState through RecoverPercent, 0x00-0x7F) to the frame's write
 * virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr)
 */
extern "C" void cWipeoutControl_writeToReplayFrame(cWipeoutControl* self, cReplayFramePtr* obj)
{
    sVEntry0012E708* vt = *(sVEntry0012E708**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x80);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012E740 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the wipeout control's 0x80-byte state back from a replay frame.
 *
 * Fills the cWipeoutControlState part (RiderState through RecoverPercent, 0x00-0x7F) through the frame's read
 * virtual (slot 2); the counterpart of cWipeoutControl_writeToReplayFrame().
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr)
 */
extern "C" void cWipeoutControl_readFromReplayFrame(cWipeoutControl* self, cReplayFramePtr* obj)
{
    sVEntry0012E740* vt = *(sVEntry0012E740**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x80);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBumpControl_update);
#ifdef SKIP_ASM
extern "C" int cRider_checkForFinishLine(void*);
// The unit declares cRider_checkForReset as void, but this caller tests its result: bind an int view by asm label.
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkRailBoardCollisions(void*);
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cRider_setupLean(void*, float);
extern "C" void cRider_setupRailBalance(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" void cRailslideControl_playGainFocusAnim(cRailslideControl*, int);
extern "C" void cRider_undoRailslideModeOrientation(void* rider);

struct sAiVec4_12E778
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAiBitsA_12E778
{
    int pad : 21;
    int v : 6;
};

struct sAiBitsB_12E778
{
    int pad : 15;
    int v : 6;
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float aiDot_12E778(const sAiVec4_12E778& a, const sAiVec4_12E778& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Updates the soft-collision reaction control (CS_Bump): steers while the bump clip plays, then returns to
 * cruise, natural air or the rail.
 *
 * The finish check (cRider_checkForFinishLine), the reset check (ResetPath, bit 12) and the rail attach
 * (cRider_checkRailBoardCollisions) may each end the tick. In the air it stops boosting and zeroes the turn
 * target; otherwise it passes BoostHeld/BoostPressed (bits 14/13) to cRider_setupBoostWarp and sets the rail balance
 * (bits 21-26) in rail motion or the turn target from CruiseTurn (bits 15-20, negated when moving against
 * mGroundForward). Crouch, brake and animation-turn targets go to 0. When the main clip completes, on a rail
 * it plays the rail cycle (cRailslideControl_playGainFocusAnim) and requests CS_Railslide; else it restores
 * the stance (cRider_undoRailslideModeOrientation) and requests CS_NaturalAir in the air or CS_Cruise.
 *
 * @param in  the command words (On Tour tBumpCmd)
 */
extern "C" void cBumpControl_update(cBumpControl* self, void* in)
{
    if (cRider_checkForFinishLine(*(void**)self))
        return;
    if (cRider_checkForReset_i(*(void**)self, (*(int*)in >> 12) & 1, 0))
        return;
    if (cRider_checkRailBoardCollisions(*(void**)self))
        return;
    if (cRider_getMotionState(*(void**)self) == 1)
    {
        cRider_setupBoostWarp(*(void**)self, 0, 0);
        cRider_setupLean(*(void**)self, 0.0f);
    }
    else
    {
        int w = *(int*)in;
        cRider_setupBoostWarp(*(void**)self, (w >> 14) & 1, (w >> 13) & 1);
        if (cRider_getMotionState(*(void**)self) == 4)
        {
            cRider_setupRailBalance(*(void**)self, ((sAiBitsA_12E778*)in)->v * 0.032258063554763794f);
        }
        else
        {
            char* rider = *(char**)self;
            float turn = ((sAiBitsB_12E778*)in)->v * 0.032258063554763794f;
            if (aiDot_12E778(*(sAiVec4_12E778*)(rider + 0x1E0), *(sAiVec4_12E778*)(rider + 0x3A0)) < 0.0f)
                turn = -turn;
            cRider_setupLean(rider, turn);
        }
    }
    float z = 0.0f;
    cRider_setupCrouchBrake(*(void**)self, z, z);
    char* rider2 = *(char**)self;
    *(float*)(rider2 + 0x204) = z;
    *(float*)(rider2 + 0x200) = 0.03333333507180214f;
    if (cRiderAnimBase_isCurrentAnimEndFlag(*(void**)(*(char**)self + 0x784), 2) == 0)
        return;
    if (cRider_getMotionState(*(void**)self) == 4)
    {
        cRailslideControl_playGainFocusAnim((cRailslideControl*)(*(char**)(*(char**)self + 0x77C) + 0x2B0), 0);
        cRider_setControlState_impl(*(void**)self, 7);
    }
    else
    {
        cRider_undoRailslideModeOrientation(*(void**)self);
        if (cRider_getMotionState(*(void**)self) == 1)
            cRider_setControlState_impl(*(void**)self, 4);
        else
            cRider_setControlState_impl(*(void**)self, 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cPrewindControl_gainFocus);
#ifdef SKIP_ASM
extern "C" void cPrewindControl_playFromTable(cPrewindControl*, void*);
extern char kAnimPrewindTable[];

/**
 * Enters the prewind control: resets the animation-turn target and plays the prewind clip for the current
 * input.
 *
 * Sets the animation-turn channel (rider + 0x1FC) to target 0 at rate 1/30, then calls
 * cPrewindControl_playFromTable() with the prewind table kAnimPrewindTable (semantics 245-267).
 */
extern "C" void cPrewindControl_gainFocus(cPrewindControl* self)
{
    char* rider = *(char**)self;
    *(float*)(rider + 0x200) = 0.03333333507180214f;
    *(int*)(rider + 0x204) = 0;
    cPrewindControl_playFromTable(self, kAnimPrewindTable);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cPrewindControl_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when the prewind control loses focus.
 */
void cPrewindControl_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/ai", cPrewindControl_update);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" void cRider_setupLean(void*, float);
extern "C" void cRider_setupRailBalance(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cRider_applyJumpPop(void* rider, float v);
extern "C" int cRider_isChangeFakieGround(void* rider);
extern "C" void cRider_snapAxisIncrement(void* rider, void* a, void* b, float c, float d);
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkForFinishLine(void*);
extern "C" void cRider_verifyRailslideModeDEBUG(void* rider);
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void cPrewindControl_playFromTable(cPrewindControl*, void*);
extern "C" void cSSXAudio_Lift(void* self, void* rider);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern char kAnimIntoAirTable[];
extern char kAnimPrewindTable[];

struct sAiBits0_12E9B8 { int v : 6; };
struct sAiBits6_12E9B8 { int pad : 6; int v : 6; };
struct sAiBits15_12E9B8 { int pad : 15; int v : 6; };
struct sAiBits21_12E9B8 { int pad : 21; int v : 6; };

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_12E9B8(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

static inline float aiStep_12E9B8(float v)
{
    if (v < -0.20000000298023224f) return -1.0f;
    if (v > 0.20000000298023224f) return 1.0f;
    return 0.0f;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_12E9B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Updates the prewind (charged jump) control: winds up while jump is held and takes off into the air control
 * on release.
 *
 * The finish, reset (ResetPath) and rail-attach checks may end the tick. On release (JumpHeld, bit 13, clear)
 * it snaps the prewind pair to pi/4 steps (cRider_snapAxisIncrement), resets the orientation offset for rail styles 3/4,
 * plays the release clip from kAnimIntoAirTable, clears the rail style, takes off unless airborne (cRider_applyJumpPop
 * with the crouch value, MS_Air, cSSXAudio_Lift), zeroes the turn, crouch and brake targets and requests
 * CS_Spin.
 *
 * While held: boost from BoostHeld and a full crouch target. A class-10 (landing) clip sets the turn
 * (PrewindTurn) or rail balance (word 1, clamped to +-0.5) and zeroes the prewind targets; semantic 0x15
 * zeroes the turn and prewind targets. Else, with no rail style and no spin, cRider_isChangeFakieGround may start a
 * reverse turn (semantic 0x15). Otherwise PrewindSpin and PrewindFlip (bits 15-20, 21-26), snapped to -1, 0
 * or 1, become the targets of the prewind channels (rider + 0x2A4, + 0x2B0), the turn or rail balance is set,
 * and the prewind clip is picked from kAnimPrewindTable.
 *
 * @param in  the command words (On Tour tPrewindCmd)
 */
extern "C" void cPrewindControl_update(cPrewindControl* self, void* in)
{
    if (cRider_checkForFinishLine(*(void**)self))
        return;
    if (cRider_checkForReset_i(*(void**)self, (*(int*)in >> 12) & 1, 0))
        return;
    if (cRider_checkRailBoardCollisions(*(void**)self))
        return;
    int w = *(int*)in;
    if ((w & 0x2000) == 0)
    {
        char* rider0 = *(char**)self;
        cRider_snapAxisIncrement(rider0, rider0 + 0x2B0, rider0 + 0x2A4, 0.7853982448577881f, 0.20000000298023224f);
        char* rider = *(char**)self;
        if (*(int*)(rider + 0x328) == 3 || *(int*)(rider + 0x328) == 4)
        {
            float fakieAngle = 0.0f;
            char* anim = *(char**)(rider + 0x784);
            *(sQuat_12BE20*)(anim + 0x30) = D_004FF130_12BE20;
            *(sQuat_12BE20*)(anim + 0x40) = AxisAngle_12BE20(D_004FF160_12BE20, -fakieAngle);
        }
        cPrewindControl_playFromTable(self, kAnimIntoAirTable);
        *(int*)(*(char**)self + 0x328) = 0;
        cRider_verifyRailslideModeDEBUG(*(void**)self);
        if (cRider_getMotionState(*(void**)self) != 1)
        {
            cRider_applyJumpPop(*(void**)self, *(float*)(*(char**)self + 0x220));
            cRider_setMotionState_impl(*(void**)self, 1);
            cSSXAudio_Lift(gSSXAudio, *(void**)self);
        }
        float z = 0.0f;
        cRider_setupLean(*(void**)self, z);
        cRider_setupCrouchBrake(*(void**)self, z, z);
        cRider_setControlState_impl(*(void**)self, 5);
        return;
    }
    cRider_setupBoostWarp(*(void**)self, (w >> 14) & 1, 0);
    float zero = 0.0f;
    cRider_setupCrouchBrake(*(void**)self, 1.0f, zero);
    float sx = aiClamp_12E9B8(((sAiBits0_12E9B8*)((char*)in + 4))->v * 0.032258063554763794f, -0.5f, 0.5f);
    float sy = aiClamp_12E9B8(((sAiBits6_12E9B8*)((char*)in + 4))->v * 0.032258063554763794f, -0.5f, 0.5f);
    if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(*(char**)self + 0x784), 2) == 10)
    {
        char* rider = *(char**)self;
        if (*(int*)(rider + 0x328) == 0)
            cRider_setupLean(rider, sx);
        else
            cRider_setupRailBalance(rider, sy);
        char* rider1 = *(char**)self;
        *(float*)(rider1 + 0x2AC) = 0.0f;
        *(float*)(rider1 + 0x2A8) = 0.08333379030227661f;
        char* rider2 = *(char**)self;
        *(float*)(rider2 + 0x2B4) = 0.08333379030227661f;
        *(float*)(rider2 + 0x2B8) = 0.0f;
        return;
    }
    if (cRiderAnimBase_getCurrentAnimID(*(void**)(*(char**)self + 0x784), 2) == 21)
    {
        cRider_setupLean(*(void**)self, zero);
        char* rider1 = *(char**)self;
        *(float*)(rider1 + 0x2AC) = zero;
        *(float*)(rider1 + 0x2A8) = 0.08333379030227661f;
        char* rider2 = *(char**)self;
        *(float*)(rider2 + 0x2B8) = zero;
        *(float*)(rider2 + 0x2B4) = 0.08333379030227661f;
        return;
    }
    char* rider = *(char**)self;
    if (*(int*)(rider + 0x328) == 0 && *(float*)(rider + 0x2DC) == zero && cRider_isChangeFakieGround(rider))
    {
        cRiderAnimBase_play(*(void**)(*(char**)self + 0x784), 21, 0, -1.0f);
        return;
    }
    float rate = 5.0000901222229f;
    if (*(int*)(*(char**)self + 0x328) == 0)
        rate = 5.000027179718018f;
    float tx = ((sAiBits15_12E9B8*)in)->v * 0.032258063554763794f;
    float ty = ((sAiBits21_12E9B8*)in)->v * 0.032258063554763794f;
    if (tx < -0.20000000298023224f)
        tx = -1.0f;
    else if (tx > 0.20000000298023224f)
        tx = 1.0f;
    else
        tx = 0.0f;
    if (ty < -0.20000000298023224f)
        ty = -1.0f;
    else if (ty > 0.20000000298023224f)
        ty = 1.0f;
    else
        ty = 0.0f;
    float ux = tx;
    float uy = ty;
    char* rider3 = *(char**)self;
    float spinDelta = ux - *(float*)(rider3 + 0x2A4);
    *(float*)(rider3 + 0x2AC) = ux;
    *(float*)(rider3 + 0x2A8) = rate * aiAbs_12E9B8(spinDelta) * 0.01666666753590107f;
    char* rider4 = *(char**)self;
    *(float*)(rider4 + 0x2B8) = uy;
    *(float*)(rider4 + 0x2B4) = rate * aiAbs_12E9B8(uy - *(float*)(rider4 + 0x2B0)) * 0.01666666753590107f;
    char* rider5 = *(char**)self;
    if (*(int*)(rider5 + 0x328) == 0)
        cRider_setupLean(rider5, sx);
    else
        cRider_setupRailBalance(rider5, sy);
    cPrewindControl_playFromTable(self, kAnimPrewindTable);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cPrewindControl_playFromTable);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

static inline float aiAtan2_12EE30(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12EE30(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Plays the prewind or release clip for the prewind input's direction from a table of semantics.
 *
 * The row comes from the prewind spin and flip values (rider + 0x2A4, + 0x2B0): row 0 when both are 0, else
 * rows 1-8 by atan2(-flip, spin) in pi/4 sectors (spin negated in switch stance, rider + 0x320); the column
 * is the rail style (rider + 0x328). The clip plays on the main channel (blend -1) only if it is not already
 * playing.
 *
 * @param tbl_  semantic table of 9 rows of 5 (kAnimPrewindTable prewinds or kAnimIntoAirTable releases)
 */
extern "C" void cPrewindControl_playFromTable(cPrewindControl* self, void* tbl_)
{
    char* obj = *(char**)self;
    float x = *(float*)(obj + 0x2A4);
    float y = *(float*)(obj + 0x2B0);
    float ax = aiAbs_12EE30(x);
    float ay = aiAbs_12EE30(y);
    float mag = (ay < ax) ? ax : ay;
    float angle;
    if (*(int*)(obj + 0x320))
        angle = aiAtan2_12EE30(-y, -x);
    else
        angle = aiAtan2_12EE30(-y, x);
    int cur = cRiderAnimBase_getCurrentAnimID(*(void**)(*(char**)self + 0x784), 2);
    int anim;
    if (mag == 0.0f)
        anim = ((int (*)[5])tbl_)[0][*(int*)(*(char**)self + 0x328)];
    else if (aiAbs_12EE30(angle) > 2.7488937377929688f)
        anim = ((int (*)[5])tbl_)[1][*(int*)(*(char**)self + 0x328)];
    else if (angle > 1.9634956121444702f)
        anim = ((int (*)[5])tbl_)[2][*(int*)(*(char**)self + 0x328)];
    else if (angle > 1.1780973672866821f)
        anim = ((int (*)[5])tbl_)[3][*(int*)(*(char**)self + 0x328)];
    else if (angle > 0.39269912242889404f)
        anim = ((int (*)[5])tbl_)[4][*(int*)(*(char**)self + 0x328)];
    else if (angle < -1.9634956121444702f)
        anim = ((int (*)[5])tbl_)[5][*(int*)(*(char**)self + 0x328)];
    else if (angle < -1.1780973672866821f)
        anim = ((int (*)[5])tbl_)[6][*(int*)(*(char**)self + 0x328)];
    else if (angle < -0.39269912242889404f)
        anim = ((int (*)[5])tbl_)[7][*(int*)(*(char**)self + 0x328)];
    else
        anim = ((int (*)[5])tbl_)[8][*(int*)(*(char**)self + 0x328)];
    if (anim != cur)
        cRiderAnimBase_play(*(void**)(*(char**)self + 0x784), anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cPrewindControl_getFlipSpinInputAng);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12F118(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_12F118(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

/**
 * Returns the direction of the prewind input, the angle of (spin, flip), or reports that there is none.
 *
 * Reads the prewind spin and flip values (rider + 0x2A4, + 0x2B0) with a 0.1 dead zone. Called by the trick
 * scoring (cRiderMetrix_updateStaticHUD).
 *
 * @param out  set to 1 when both values are inside the dead zone, else 0 (On Tour bool& isNull)
 * @return atan2(flip, spin) in radians, or 0 when there is no input
 */
extern "C" float cPrewindControl_getFlipSpinInputAng(cPrewindControl* self, int* out)
{
    char* obj = *(char**)self;
    float y = *(float*)(obj + 0x2B0);
    float x = *(float*)(obj + 0x2A4);
    if (aiAbs_12F118(y) < 0.1f) y = 0.0f;
    if (aiAbs_12F118(x) < 0.1f) x = 0.0f;
    int isNull = 0;
    if (y == 0.0f && x == 0.0f) isNull = 1;
    *out = isNull;
    if (isNull) return 0.0f;
    return aiAtan2_12F118(y, x);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cResetControl_gainFocus);
#ifdef SKIP_ASM
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int cResetControl_shouldTransition(cResetControl* self);
void* cGame_getTransitionMan(void* self, int id);
extern "C" void cTransitionMan_EndTransition(void* obj);
void* cTransitionObj_cTransitionObj(void* self);
extern "C" void cTransitionMan_StartTransition(void* self, int a1, void* a2, void* a3, float f0, float f1, float f2, int a4, void* a5);
extern void* cFade_TObj_vtbl[];

struct sColor_12F230
{
    float r, g, b, a;
    sColor_12F230() {}
    sColor_12F230(float ar, float ag, float ab, float aa) { r = ar; g = ag; b = ab; a = aa; }
};

struct sFx_12F230
{
    void** vt;
    sColor_12F230 c;
};

// PORT: the wake-fx pool allocator ignores its argument; callers pass the object size.
extern "C" void* cTransitionObj_operator_new_sz(unsigned size) __asm__("cTransitionObj_operator_new");

static inline sFx_12F230* initFx_12F230(sFx_12F230* p, const sColor_12F230& col)
{
    cTransitionObj_cTransitionObj(p);
    p->vt = cFade_TObj_vtbl;
    p->c = col;
    return p;
}

/**
 * Enters the reset control: zeroes the fade progress, freezes the main animation and starts the white reset
 * fade on the rider's view.
 *
 * FadePercent and the main-channel sequence rate (+0x90) are cleared (stored as integer 0). When
 * cResetControl_shouldTransition() allows it, a running effect on the rider's view (cGame_getTransitionMan with
 * mViewIndex) is stopped (cTransitionMan_EndTransition), then two white cFade_TObj fades go to cTransitionMan_StartTransition (times 0.5, 0,
 * 0.5) with a pointer to FadePercent, which drives the fade.
 */
extern "C" void cResetControl_gainFocus(cResetControl* self)
{
    int* flag = (int*)&self->FadePercent;
    *flag = 0;
    *(int*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 0;
    if (cResetControl_shouldTransition(self))
    {
        char* obj = (char*)cGame_getTransitionMan(SSXApp->Game, self->mRider->mViewIndex);
        if (*(int*)(obj + 0x44))
            cTransitionMan_EndTransition(obj);
        sFx_12F230* firstFade = (sFx_12F230*)cTransitionObj_operator_new_sz(20);
        sColor_12F230 ca;
        ca.r = 1.0f;
        ca.g = 1.0f;
        ca.b = 1.0f;
        ca.a = 1.0f;
        initFx_12F230(firstFade, ca);
        sFx_12F230* secondFade = (sFx_12F230*)cTransitionObj_operator_new_sz(20);
        sColor_12F230 cb;
        cb.r = 1.0f;
        cb.g = 1.0f;
        cb.b = 1.0f;
        cb.a = 1.0f;
        initFx_12F230(secondFade, cb);
        cTransitionMan_StartTransition(obj, 1, firstFade, secondFade, 0.5f, 0.0f, 0.5f, 0, flag);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cResetControl_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int cResetControl_shouldTransition(cResetControl* self);
void* cGame_getTransitionMan(void* self, int id);
extern "C" void cTransitionMan_EndTransition(void* obj);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" float cRiderMetrix_evReset(void* self, int a1);
extern "C" void cRiderMetrix_evTakeoff(void* p, int flip, int c);
extern "C" void cRider_switchToResetPath(void*);
extern "C" void cRider_cleanupAfterPutAt(void* self, int notify);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

struct sVec4_12F398
{
    float x, y, z, w;
};

extern "C" sVec4_12F398 cPath_getPathHeading(void* self, float t);
extern "C" void cRider_putAt_12F398(void* self, void* a, sVec4_12F398* b, int n, float t) __asm__("cRider_putAt");

/**
 * Advances the reset fade, moves the rider back onto the course halfway through, and hands over to natural
 * air at the end.
 *
 * FadePercent approaches 1 by mTimeScale * 0.025 per tick. On crossing 0.5 it picks a reset route
 * (cRider_switchToResetPath), places the rider along the path direction (cPath_getPathHeading, cRider_putAt with semantic
 * 0x11F and 200 cm clearance, 1000 when cGameEvent (newRace.cGameEvent) is 2), calls cRider_cleanupAfterPutAt() and
 * takes boost through cRiderMetrix_evReset (which also counts the reset): 10% of the boost meter (rider + 0x2F8) for
 * an auto reset (IsAutoReset), 70% otherwise. At 1 it stops the view fade (when shouldTransition), starts a
 * new score takeoff (cRiderMetrix_evTakeoff), restores the main sequence rate to 1 and requests CS_NaturalAir and
 * MS_Air.
 */
extern "C" void cResetControl_update(cResetControl* self)
{
    float prevFadePercent = self->FadePercent;
    float step = self->mRider->mTimeScale * 0.02500000223517418f;
    float n;
    if (prevFadePercent > step + 1.0f)
        n = prevFadePercent - step;
    else if (prevFadePercent < 1.0f - step)
        n = prevFadePercent + step;
    else
        n = 1.0f;
    self->FadePercent = n;
    if (prevFadePercent <= 0.5f && n > 0.5f)
    {
        float range = 200.0f;
        cRider_switchToResetPath(self->mRider);
        cRider* rider = self->mRider;
        const sVec4_12F398& dir = cPath_getPathHeading(rider->AIPath, rider->PathParam);
        if (cBE_getInterface_Fv(cBE_getBE(), 0) != 0 && cBENewRaceInterface_newRace.cGameEvent == 2)
            range = 1000.0f;
        cRider_putAt_12F398(self->mRider, &self->mRider->PathPos, (sVec4_12F398*)&dir, 287, range);
        cRider_cleanupAfterPutAt(self->mRider, 1);
        if (*(int*)self != 0)
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReset(self->mRider->mMetrix, 0));
        else
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReset(self->mRider->mMetrix, 1));
    }
    if (self->FadePercent == 1.0f)
    {
        if (cResetControl_shouldTransition(self))
            cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, self->mRider->mViewIndex));
        cRiderMetrix_evTakeoff(self->mRider->mMetrix, 0, 0);
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 1.0f;
        cRider_setControlState_impl(self->mRider, 4);
        cRider_setMotionState_impl(self->mRider, 1);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cResetControl_shouldTransition);
#ifdef SKIP_ASM
/**
 * Tells whether the reset shows a screen transition, which is only for a local rider with a view.
 *
 * @return 1 when mIsLocal is set and mViewIndex >= 0, otherwise 0
 */
extern "C" int cResetControl_shouldTransition(cResetControl* self)
{
    cRider* rider = self->mRider;
    return rider->mIsLocal != 0 && rider->mViewIndex >= 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cResetControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012F5B0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the reset control's 8-byte state (IsAutoReset, FadePercent) to a replay frame.
 *
 * Uses the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr)
 */
extern "C" void cResetControl_writeToReplayFrame(cResetControl* self, cReplayFramePtr* obj)
{
    sVEntry0012F5B0* vt = *(sVEntry0012F5B0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cResetControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012F5E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the reset control's 8-byte state (IsAutoReset, FadePercent) back from a replay frame.
 *
 * Uses the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr)
 */
extern "C" void cResetControl_readFromReplayFrame(cResetControl* self, cReplayFramePtr* obj)
{
    sVEntry0012F5E8* vt = *(sVEntry0012F5E8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cNaturalAirControl_gainFocus);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12F620(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_12F620(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

/**
 * Enters the natural (passive) air control: remembers the stick input held at take-off and clears the prewind
 * targets.
 *
 * LimitMag = max(|turn|, crouch) and LimitAng = atan2(crouch, turn) from the current turn and crouch values
 * (rider + 0x1F0, + 0x220); IsPunchHeld and IsGrabHeld are set and InitGrabHeld = -1 (none yet). Both prewind
 * channels (rider + 0x2A4, + 0x2B0) get target 0 at rate 1/30.
 */
extern "C" void cNaturalAirControl_gainFocus(cNaturalAirControl* self)
{
    cRider* rider = self->mRider;
    float curCrouch = rider->mSlewed[4].cur;
    float curMag = aiAbs_12F620(rider->mSlewed[0].cur);
    if (curMag < curCrouch) curMag = curCrouch;
    self->LimitMag = curMag;
    float ang = aiAtan2_12F620(rider->mSlewed[4].cur, rider->mSlewed[0].cur);
    self->LimitAng = ang;
    self->IsPunchHeld = 1;
    self->IsGrabHeld = 1;
    self->InitGrabHeld = -1;
    cRider* rider1 = self->mRider;
    *(int*)&rider1->mSlewed[15].target = 0;
    rider1->mSlewed[15].rate = 0.03333333507180214f;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[16].rate = 0.03333333507180214f;
    *(int*)&rider2->mSlewed[16].target = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cNaturalAirControl_update);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" int cRider_checkHandplantCollisions(void* rider, int on);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkForPunch(void* rider, int a, int b);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" float func_0031C228(float x);

struct sAiBitsX_12F730
{
    int pad : 24;
    int v : 6;
};

struct sAiBitsY_12F730
{
    int v : 6;
};

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_12F730(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_12F730(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float aiFloor_12F730(float x)
{
    float t;
    __asm__("cvt.w.s %0, %1\n\tcvt.s.w %0, %0" : "=f"(t) : "f"(x));
    if (x < t)
    {
        t -= 1.0f;
    }
    return t;
}

static inline float aiWrap_12F730(float x)
{
    return x - aiFloor_12F730(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

struct sAiObj12F730
{
    float mHeading;  // 0x00
    float mTimer;    // 0x04
    int mFlag;       // 0x08
    int mHold;       // 0x0C
    int mId;         // 0x10
    char* mRider;    // 0x14
};

/**
 * Updates the natural air control (CS_NaturalAir): keeps the input held from take-off and hands over to the
 * air control once it is released.
 *
 * The reset check (bit 12) may end the tick. Attacks (bits 14/15) go to cRider_checkForPunch(), whose result
 * ends the tick only while IsPunchHeld (still held from entry); a handplant (bit 13) may end it or else
 * forces a crouch of 1; rail attach may end it; boost is turned off. A grab held at entry (bits 16-23) is
 * latched in InitGrabHeld; when it changes, IsGrabHeld drops and LimitMag becomes -1.
 *
 * CruiseTurn/CruiseCrouch is kept only while it matches the take-off input: when LimitMag is negative, the
 * input's size max(|turn|, crouch) is at most max(LimitMag - 0.5, 0), or its angle is over 90 degrees from
 * LimitAng, LimitMag becomes -1 and the input is zeroed. The result sets the turn and crouch targets (brake
 * 0, animation turn = current turn, all at rate 0.05). With turn, brake and crouch all 0 and no grab, it
 * plays the air cycle 0x11F (unless a class-9 clip runs) and requests CS_Spin; otherwise, unless a class-9
 * clip runs, it plays semantic 9 or 10 by crouch against (1 - |turn|) / 2.
 *
 * @param in  the command words (On Tour tNaturalAirCmd)
 */
extern "C" void cNaturalAirControl_update(cNaturalAirControl* self, void* in)
{
    if (cRider_checkForReset_i((char*)self->mRider, (*(int*)in >> 12) & 1, 0))
        return;
    if (self->IsPunchHeld != 0)
    {
        self->IsPunchHeld = (*(int*)in & 0xC000) != 0;
        if (cRider_checkForPunch((char*)self->mRider, (*(int*)in >> 14) & 1, (*(int*)in >> 15) & 1))
            return;
    }
    else
    {
        cRider_checkForPunch((char*)self->mRider, 0, 0);
    }
    int w = *(int*)in;
    float x = ((sAiBitsX_12F730*)&w)->v * 0.032258063554763794f;
    float y = ((sAiBitsY_12F730*)((char*)in + 4))->v * 0.032258063554763794f;
    if (w & 0x2000)
    {
        if (cRider_checkHandplantCollisions((char*)self->mRider, y > 0.5f))
            return;
        y = 1.0f;
    }
    if (cRider_checkRailBoardCollisions((char*)self->mRider))
        return;
    cRider_setupBoostWarp((char*)self->mRider, 0, 0);
    if (self->IsGrabHeld != 0 && self->InitGrabHeld == -1)
        self->InitGrabHeld = *((signed char*)in + 2);
    if (*((signed char*)in + 2) != self->InitGrabHeld || *((signed char*)in + 2) == -1)
        self->IsGrabHeld = 0;
    if (self->IsGrabHeld == 0 && self->InitGrabHeld != -1)
        self->LimitMag = -1.0f;
    float ax = aiAbs_12F730(x);
    float lim = y;
    if (y <= ax)
        lim = ax;
    float ang = aiAtan2_12F730(y, x);
    if (self->LimitMag < 0.0f || lim <= (self->LimitMag - 0.5f >? 0.0f) ||
        __builtin_fabsf(aiWrap_12F730(ang - self->LimitAng)) > 1.5707964897155762f)
    {
        self->LimitMag = -1.0f;
        y = 0.0f;
        x = y;
    }
    cRider* rider = self->mRider;
    rider->mSlewed[0].target = x;
    rider->mSlewed[0].rate = 0.05000000447034836f;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[1].rate = 0.05000000447034836f;
    rider2->mSlewed[1].target = rider2->mSlewed[0].cur;
    cRider* rider3 = self->mRider;
    rider3->mSlewed[4].rate = 0.05000000447034836f;
    rider3->mSlewed[4].target = y;
    cRider* rider4 = self->mRider;
    rider4->mSlewed[3].rate = 0.05000000447034836f;
    *(int*)&rider4->mSlewed[3].target = 0;
    int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    cRider* rider5 = self->mRider;
    if (rider5->mSlewed[4].cur == 0.0f && rider5->mSlewed[3].cur == 0.0f && rider5->mSlewed[0].cur == 0.0f && self->IsGrabHeld == 0)
    {
        if (anim != 287)
        {
            if (cRiderAnimBase_getCurrentAnimCategory(rider5->mRiderAnim, 2) != 9)
                cRiderAnimBase_play(self->mRider->mRiderAnim, 287, 0, -1.0f);
        }
        cRider_setControlState_impl((char*)self->mRider, 5);
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 9)
        return;
    int desAnim = 10;
    cRider* rider6 = self->mRider;
    if (rider6->mSlewed[4].cur <= (1.0f - aiAbs_12F730(rider6->mSlewed[0].cur)) * 0.5f)
        desAnim = 9;
    if (desAnim != anim)
        cRiderAnimBase_play(rider6->mRiderAnim, desAnim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cNaturalAirControl_loseFocus);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRiderAnimBase_blendOutAll(void*, int, float);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);

/**
 * Leaves the natural air control: when going anywhere but cruise, ends an attack still playing on the upper
 * body.
 *
 * If @p next is not CS_Cruise and the channel-1 clip is a block or punch (class 3 or 13), fades channel 0
 * (the attack's held cycle) out over 0.1 s (cRiderAnimBase_blendOutAll) and restores channel 1's sequence rate to 1, so an
 * attack survives a landing only.
 *
 * @param next  control state being entered (eControlState)
 */
extern "C" void cNaturalAirControl_loseFocus(cNaturalAirControl* self, int next)
{
    if (next)
    {
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 1) == 3 ||
            cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 1) == 13)
        {
            cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 0, 0.10000000149011612f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 1) + 0x90) = 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cNaturalAirControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012FBF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the natural air control's 0x14-byte state (LimitAng to InitGrabHeld) to a replay frame.
 *
 * Uses the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr)
 */
extern "C" void cNaturalAirControl_writeToReplayFrame(cNaturalAirControl* self, cReplayFramePtr* obj)
{
    sVEntry0012FBF0* vt = *(sVEntry0012FBF0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cNaturalAirControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry0012FC28 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the natural air control's 0x14-byte state (LimitAng to InitGrabHeld) back from a replay frame.
 *
 * Uses the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr)
 */
extern "C" void cNaturalAirControl_readFromReplayFrame(cNaturalAirControl* self, cReplayFramePtr* obj)
{
    sVEntry0012FC28* vt = *(sVEntry0012FC28**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_gainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Enters the board-press control: starts in BP_Into with all timers and the ollie latch cleared.
 *
 * Sets State to 0 (BP_Into), clears BalanceSec, OffBalanceSec, PressTimer and IsOllieHeld, and clears the
 * rider's board-press jump latch (rider + 0x360).
 */
void cBoardPressControl_gainFocus(void* vself)
{
    cBoardPressControl* self = (cBoardPressControl*)vself;
    *(int*)self = 0;
    *(int*)&self->mRider->JumpHeldSec = 0;
    *(int*)&self->BalanceSec = 0;
    *(int*)&self->OffBalanceSec = 0;
    *(int*)&self->PressTimer = 0;
    self->IsOllieHeld = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_update);
#ifdef SKIP_ASM
extern "C" int cRider_checkForFinishLine(void*);
// The unit declares cRider_checkForReset as void, but this caller tests its result: bind an int view by asm label.
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" int cBoardPressControl_checkAirMotion(cBoardPressControl*);
extern "C" int cRider_checkForPrewind(void*, int, int);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cRider_setupLean(void*, float);
extern "C" void cRider_setupRailBalance(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" void cRider_checkForReaction(void*);
extern "C" void cRider_checkForProximityAction(void*);
struct sPadIn_130228;
extern "C" void cBoardPressControl_handleIntoState(cBoardPressControl* self, sPadIn_130228* in);
extern "C" void cBoardPressControl_handleBalanceState(cBoardPressControl* self, void* a1);
struct sPadIn_1304E0;
extern "C" void cBoardPressControl_handleExitState(cBoardPressControl* self, sPadIn_1304E0* in);
// PORT: cBoardPressControl_handleIntoAirState is defined with one param, but this caller passes the input in $a1.
void cBoardPressControl_handleIntoAirState_2(void* self, void* in) __asm__("cBoardPressControl_handleIntoAirState__FPv");

struct sAiBitsC_12FC80
{
    int pad : 24;
    int v : 6;
};

struct sAiBitsD_12FC80
{
    int pad : 18;
    int v : 6;
};

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12FC80(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiClamp_12FC80(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

/**
 * Updates the board-press control (CS_BoardPress): steering, boost and the handler for the current press
 * phase.
 *
 * The finish, reset (bit 12), rail-attach, airborne (checkAirMotion) and jump (cRider_checkForPrewind with
 * JumpHeld/JumpPressed, bits 15/16) checks may each end the tick. Then boost from BoostHeld/BoostPressed
 * (bits 14/13), crouch and brake targets 0, and either the rail balance (bits 24-29, clamped to +-0.5) with a
 * rail style or the turn from CruiseTurn (bits 18-23) clamped to +-||pivot| - 0.5|. After the reaction and
 * proximity updates (cRider_checkForReaction, cRider_checkForProximityAction) it runs the State handler:
 * handleIntoState, handleBalanceState, cBoardPressControl_handleIntoAirState (BP_IntoAir, which only sets BP_Balance) or
 * handleExitState.
 *
 * @param in  the command (On Tour tBoardPressCmd)
 */
extern "C" void cBoardPressControl_update(cBoardPressControl* self, void* in)
{
    if (cRider_checkForFinishLine(self->mRider))
        return;
    if (cRider_checkForReset_i(self->mRider, (*(int*)in >> 12) & 1, 0))
        return;
    if (cRider_checkRailBoardCollisions(self->mRider))
        return;
    if (cBoardPressControl_checkAirMotion(self))
        return;
    int w = *(int*)in;
    if (cRider_checkForPrewind(self->mRider, (w >> 15) & 1, (w >> 16) & 1))
        return;
    int w2 = *(int*)in;
    cRider_setupBoostWarp(self->mRider, (w2 >> 14) & 1, (w2 >> 13) & 1);
    cRider_setupCrouchBrake(self->mRider, 0.0f, 0.0f);
    cRider* rider = self->mRider;
    if (rider->RailslideMode != 0)
    {
        cRider_setupRailBalance(rider, aiClamp_12FC80(((sAiBitsC_12FC80*)in)->v * 0.032258063554763794f, -0.5f, 0.5f));
    }
    else
    {
        float absPivot = aiAbs_12FC80(rider->mSlewed[12].cur);
        float maxTurn;
        if (absPivot > 0.5f)
            maxTurn = absPivot - 0.5f;
        else
            maxTurn = 0.5f - absPivot;
        cRider_setupLean(self->mRider, aiClamp_12FC80(((sAiBitsD_12FC80*)in)->v * 0.032258063554763794f, -maxTurn, maxTurn));
    }
    cRider_checkForReaction(self->mRider);
    cRider_checkForProximityAction(self->mRider);
    switch (*(int*)self)
    {
    case 0:
        cBoardPressControl_handleIntoState(self, (sPadIn_130228*)in);
        break;
    case 1:
        cBoardPressControl_handleBalanceState(self, in);
        break;
    case 2:
        cBoardPressControl_handleIntoAirState_2(self, in);
        break;
    case 3:
        cBoardPressControl_handleExitState(self, (sPadIn_1304E0*)in);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_loseFocus);
#ifdef SKIP_ASM
/**
 * Leaves the board-press control: clears the press mode and lets the press and board depth decay to 0.
 *
 * Sets PressMode (rider + 0x330, On Tour ePressMode) to 0, the board-depth channel (rider + 0x274) to target
 * 0 at rate 1/60 and the press-depth channel (rider + 0x268) to target 0 at rate 1/30.
 */
extern "C" void cBoardPressControl_loseFocus(cBoardPressControl* self)
{
    cRider* rider = self->mRider;
    rider->PressMode = 0;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[11].rate = 0.01666666753590107f;
    *(int*)&rider2->mSlewed[11].target = 0;
    cRider* rider3 = self->mRider;
    rider3->mSlewed[10].rate = 0.03333333507180214f;
    *(int*)&rider3->mSlewed[10].target = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_changeFakieBoardPress);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_verifyRailslideModeDEBUG(void* rider);

struct sQuat_12FEC8
{
    float x, y, z, w;
    sQuat_12FEC8() {}
    sQuat_12FEC8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_12FEC8 t4vect_Origin;
extern sQuat_12FEC8 t4vect_ZAxis;

static inline sQuat_12FEC8 AxisAngle_12FEC8(const sQuat_12FEC8& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_12FEC8(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Flips the rider's stance for a board pivot: toggles switch stance and swaps the nose and tail sides.
 *
 * Toggles the switch flag (rider + 0x320), sets the animation orientation offset to a turn of 0 or pi about Z
 * (t4vect_Origin, t4vect_ZAxis) and copies the flag to the animator's IsFlipped; swaps rail styles 1 and 2 and
 * negates the pivot value and target (rider + 0x280, + 0x288).
 */
extern "C" void cBoardPressControl_changeFakieBoardPress(cBoardPressControl* self)
{
    self->mRider->IsRidingFakie ^= 1;
    cRider* rider = self->mRider;
    float fakieAngle = 0.0f;
    if (rider->IsRidingFakie != 0) fakieAngle = 3.1415927410125732f;
    char* anim = (char*)rider->mRiderAnim;
    *(sQuat_12FEC8*)(anim + 0x30) = t4vect_Origin;
    *(sQuat_12FEC8*)(anim + 0x40) = AxisAngle_12FEC8(t4vect_ZAxis, -fakieAngle);
    cRider* rider2 = self->mRider;
    *(int*)((char*)rider2->mRiderAnim + 0x18) = rider2->IsRidingFakie;
    cRider* rider3 = self->mRider;
    if (rider3->RailslideMode == 1)
    {
        rider3->RailslideMode = 2;
    }
    else if (rider3->RailslideMode == 2)
    {
        rider3->RailslideMode = 1;
    }
    self->mRider->mSlewed[12].cur = -self->mRider->mSlewed[12].cur;
    self->mRider->mSlewed[12].target = -self->mRider->mSlewed[12].target;
    cRider_verifyRailslideModeDEBUG(self->mRider);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkAirMotion);
#ifdef SKIP_ASM
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void cRider_verifyRailslideModeDEBUG(void* rider);
extern "C" float cRiderMetrix_evPressEnd(void*);
extern "C" float cRiderMetrix_evPressPivot(void* p, int v);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12FFF8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Handles a board press that has left the ground: finishes a pivot, then switches to the air control.
 *
 * Acts only in MS_Air and clears any rail style. In BP_IntoAir it sends the pivot (rider + 0x280) to 0 (clips
 * 0x25/0x26) or to +-1 (clips 0x1D/0x1E) and waits until it is within 0.15 of that; then it scores the pivot
 * and the press (cRiderMetrix_evPressPivot, cRiderMetrix_evPressEnd), plays 0x11F and requests CS_Spin. Otherwise a playing pivot
 * clip first sets BP_IntoAir, and any other clip gives the ollie clip 0x1B (nose) or 0x23 and CS_Spin.
 *
 * @return 1 when the rider is airborne (the caller's tick ends), 0 on the ground
 */
extern "C" int cBoardPressControl_checkAirMotion(cBoardPressControl* self)
{
    int k = cRider_getMotionState(self->mRider);
    if (k != 1)
        return 0;
    cRider* rider = self->mRider;
    if (rider->RailslideMode != 0)
    {
        rider->RailslideMode = 0;
        cRider_verifyRailslideModeDEBUG(self->mRider);
    }
    int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    if (*(int*)self == 2)
    {
        int ok = 0;
        if (anim == 37 || anim == 38)
        {
            cRider* rider2 = self->mRider;
            rider2->mSlewed[12].target = 0.0f;
            rider2->mSlewed[12].rate = rider2->mTimeScale * 0.02500000223517418f;
            ok = aiAbs_12FFF8(self->mRider->mSlewed[12].cur) <= 0.15000000596046448f;
        }
        else if (anim == 29 || anim == 0x1E)
        {
            cRider* rider2 = self->mRider;
            if (rider2->mSlewed[12].cur < 0.0f)
            {
                rider2->mSlewed[12].target = -1.0f;
                rider2->mSlewed[12].rate = rider2->mTimeScale * 0.02500000223517418f;
            }
            else
            {
                rider2->mSlewed[12].target = 1.0f;
                rider2->mSlewed[12].rate = rider2->mTimeScale * 0.02500000223517418f;
            }
            ok = 0.8500000238418579f <= aiAbs_12FFF8(self->mRider->mSlewed[12].cur);
        }
        if (ok == 0)
            return 1;
        cRider* rider3 = self->mRider;
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressPivot(rider3->mMetrix, rider3->PressMode));
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressEnd(self->mRider->mMetrix));
        cRiderAnimBase_play(self->mRider->mRiderAnim, 287, 0, -1.0f);
        cRider_setControlState_impl(self->mRider, 5);
        return 1;
    }
    if (anim == 37 || anim == 38 || anim == 29 || anim == 0x1E)
    {
        *(int*)self = 2;
    }
    else
    {
        cRider* rider4 = self->mRider;
        if (rider4->PressMode == 1)
            cRiderAnimBase_play(rider4->mRiderAnim, 27, 0, -1.0f);
        else
            cRiderAnimBase_play(rider4->mRiderAnim, 35, 0, -1.0f);
        cRider_setControlState_impl(self->mRider, 5);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_handleIntoState);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" float cRiderMetrix_evPressStart(void*, int);

// PORT: abs.s via inline asm (use fabsf off-PS2).
static inline float Abs_130228(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sPadIn_130228
{
    int f0;
    int x : 6;
    int y : 6;
};

/**
 * Runs the press-in phase (BP_Into): pushes the press and board depth in, and enters the balance phase at
 * full depth.
 *
 * Letting go (BoardPress exactly 0 and |BoardPivot| < 0.5, word 1 bits 0-5 and 6-11) sends the press depth
 * (rider + 0x268) to 0 at 2 * mTimeScale / 60, plays the release clip 0x21 (tail) or 0x19 and enters BP_Exit.
 * Otherwise the board depth (rider + 0x274) approaches 0.5 at mTimeScale / 60 and the press depth 1 at twice
 * that; at exactly 1 it scores the press (cRiderMetrix_evPressStart), enters BP_Balance and plays 0x24 with the pivot at
 * 1 (tail) or 0x1C with it at 0.
 *
 * @param in  the command (On Tour tBoardPressCmd)
 */
extern "C" void cBoardPressControl_handleIntoState(cBoardPressControl* self, sPadIn_130228* in)
{
    float fx = in->x * 0.032258063554763794f;
    float fy = in->y * 0.032258063554763794f;
    if (fx == 0.0f && Abs_130228(fy) < 0.5f)
    {
        cRider* rider = self->mRider;
        rider->mSlewed[10].target = 0.0f;
        rider->mSlewed[10].rate = rider->mTimeScale * 2.0f * 0.01666666753590107f;
        *(int*)self = 3;
        cRider* rider2 = self->mRider;
        if (rider2->PressMode == 2)
        {
            cRiderAnimBase_play_i(rider2->mRiderAnim, 33, 0, -1.0f);
        }
        else
        {
            cRiderAnimBase_play_i(rider2->mRiderAnim, 25, 0, -1.0f);
        }
        return;
    }
    cRider* rider = self->mRider;
    rider->mSlewed[11].rate = rider->mTimeScale * 0.01666666753590107f;
    rider->mSlewed[11].target = 0.5f;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[10].target = 1.0f;
    rider2->mSlewed[10].rate = rider2->mTimeScale * 2.0f * 0.01666666753590107f;
    cRider* rider3 = self->mRider;
    if (rider3->mSlewed[10].cur == 1.0f)
    {
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressStart(rider3->mMetrix, rider3->PressMode));
        *(int*)self = 1;
        cRider* rider4 = self->mRider;
        if (rider4->PressMode == 2)
        {
            cRiderAnimBase_play_i(rider4->mRiderAnim, 36, 0, -1.0f);
            cRider* rider5 = self->mRider;
            rider5->mSlewed[12].rate = 0.0f;
            rider5->mSlewed[12].target = 1.0f;
            rider5->mSlewed[12].cur = 1.0f;
        }
        else
        {
            cRiderAnimBase_play_i(rider4->mRiderAnim, 0x1C, 0, -1.0f);
            cRider* rider5 = self->mRider;
            rider5->mSlewed[12].target = 0.0f;
            rider5->mSlewed[12].cur = 0.0f;
            rider5->mSlewed[12].rate = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_handleBalanceState);
#ifdef SKIP_ASM
extern "C" int cBoardPressControl_checkForFakie(cBoardPressControl* self);
extern "C" int cBoardPressControl_checkForOllie(cBoardPressControl* self, void* a1);
extern "C" int cBoardPressControl_checkForPivot(cBoardPressControl* self, void* a1);
extern "C" int cBoardPressControl_checkForWipeout(cBoardPressControl* self, void* a1);
extern "C" int cBoardPressControl_checkForExit(cBoardPressControl* self, void* a1);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Runs the balance phase (BP_Balance): tries a reverse, an ollie, a pivot, the over-press crash and the exit,
 * in that order.
 *
 * The first of checkForFakie, checkForOllie, checkForPivot, checkForWipeout and checkForExit that returns
 * nonzero ends the tick. When it is checkForExit, the press depth (rider + 0x268) goes to 0 at 2 * mTimeScale
 * / 60, the release clip 0x21 (tail) or 0x19 plays and State becomes BP_Exit.
 *
 * @param cmd  the command (On Tour tBoardPressCmd)
 */
extern "C" void cBoardPressControl_handleBalanceState(cBoardPressControl* self, void* cmd)
{
    if (cBoardPressControl_checkForFakie(self) != 0) return;
    if (cBoardPressControl_checkForOllie(self, cmd) != 0) return;
    if (cBoardPressControl_checkForPivot(self, cmd) != 0) return;
    if (cBoardPressControl_checkForWipeout(self, cmd) != 0) return;
    if (cBoardPressControl_checkForExit(self, cmd) != 0)
    {
        cRider* rider = self->mRider;
        *(int*)&rider->mSlewed[10].target = 0;
        rider->mSlewed[10].rate = rider->mTimeScale * 2.0f * 0.01666666753590107f;
        cRider* rider2 = self->mRider;
        if (rider2->PressMode == 2)
        {
            cRiderAnimBase_play(rider2->mRiderAnim, 33, 0, -1.0f);
        }
        else
        {
            cRiderAnimBase_play(rider2->mRiderAnim, 25, 0, -1.0f);
        }
        *(int*)self = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_handleIntoAirState__FPv);
#ifdef SKIP_ASM
/**
 * cBoardPressControl's handler for the BP_IntoAir state: sets the state word (+0) to 1 (BP_Balance).
 *
 * cBoardPressControl_update calls it for state 2 and passes the input in a1, which it ignores.
 */
void cBoardPressControl_handleIntoAirState(void* self)
{
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_handleExitState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void cRider_undoRailslideModeOrientation(void* rider);
extern "C" float cRiderMetrix_evPressEnd(void*);
extern "C" void cRailslideControl_playGainFocusAnim(cRailslideControl*, int);

struct sPadIn_1304E0
{
    int f0;
    int x : 6;
    int y : 6;
};

/**
 * Runs the release phase (BP_Exit): allows a re-press while the press depth drains, then leaves the board
 * press.
 *
 * While a release clip (0x21 or 0x19) plays and the press depth (rider + 0x268) is above 0, pushing
 * BoardPress past 0.5 the same way again re-presses: clip 0x20 (tail) or 0x18 (nose), PressMode to match,
 * depth target 1 and BP_Into. Otherwise it scores the end of a nose or tail press (cRiderMetrix_evPressEnd) and goes
 * back to the rail (rail cycle clip, CS_Railslide) in rail motion, else to CS_Cruise after restoring the
 * stance (cRider_undoRailslideModeOrientation).
 *
 * @param in  the command (On Tour tBoardPressCmd)
 */
extern "C" void cBoardPressControl_handleExitState(cBoardPressControl* self, sPadIn_1304E0* in)
{
    int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    char* rider;
    if ((anim == 33 || anim == 25) && 0.0f < *(float*)((rider = (char*)self->mRider) + 0x268))
    {
        float fx = in->x * 0.032258063554763794f;
        if ((fx < -0.5f && anim == 33) || (0.5f < fx && anim == 25))
        {
            if (fx < 0.0f)
            {
                cRiderAnimBase_play(self->mRider->mRiderAnim, 0x20, 0, -1.0f);
                self->mRider->PressMode = 2;
            }
            else
            {
                cRiderAnimBase_play(self->mRider->mRiderAnim, 0x18, 0, -1.0f);
                self->mRider->PressMode = 1;
            }
            cRider* rider2 = self->mRider;
            rider2->mSlewed[10].target = 1.0f;
            rider2->mSlewed[10].rate = rider2->mTimeScale * 2.0f * 0.01666666753590107f;
            *(int*)self = 0;
        }
        return;
    }
    rider = (char*)self->mRider;
    if (*(int*)(rider + 0x330) == 1 || *(int*)(rider + 0x330) == 2)
    {
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressEnd(*(void**)(rider + 0x790)));
    }
    if (cRider_getMotionState(self->mRider) == 4)
    {
        cRailslideControl_playGainFocusAnim((cRailslideControl*)(&self->mRider->mRiderHandler->RailslideControl), 0);
        cRider_setControlState_impl(self->mRider, 7);
    }
    else
    {
        cRider_undoRailslideModeOrientation(self->mRider);
        cRider_setControlState_impl(self->mRider, 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkForFakie);
#ifdef SKIP_ASM
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
int cRider_getMotionState(void* self);
extern "C" int cRider_isChangeFakieGround(void* rider);
// PORT: cRiderAnimBase_play returns a value (the unit declares it void); int-returning view bound by asm label.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");

/**
 * Tries a reverse turn during a balanced board press, which swaps the press between nose and tail.
 *
 * Returns 1 at once while a reverse clip (0x17 or 0x1F) plays. Nothing is tried while a pivot clip plays, off
 * the ground (motion not MS_Ground) or with a spin rate (mSpinRate) set; otherwise, when cRider_isChangeFakieGround starts
 * a reverse turn, it plays 0x17 and makes a nose press a tail press, or plays 0x1F and makes it a nose press.
 *
 * @return 1 when a reverse is playing or was started, otherwise 0
 */
extern "C" int cBoardPressControl_checkForFakie(cBoardPressControl* self)
{
    int curAnimId = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    if (curAnimId == 23 || curAnimId == 0x1F) return 1;
    if (curAnimId == 37 || curAnimId == 38 || curAnimId == 29 || curAnimId == 0x1E) return 0;
    if (cRider_getMotionState(self->mRider) != 0) return 0;
    cRider* rider = self->mRider;
    if (rider->mSpinRate != 0.0f) return 0;
    if (cRider_isChangeFakieGround(rider) != 0)
    {
        cRider* rider2 = self->mRider;
        if (rider2->PressMode == 1)
        {
            cRiderAnimBase_play_i(rider2->mRiderAnim, 23, 0, -1.0f);
            self->mRider->PressMode = 2;
        }
        else
        {
            cRiderAnimBase_play_i(rider2->mRiderAnim, 0x1F, 0, -1.0f);
            self->mRider->PressMode = 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkForOllie);
#ifdef SKIP_ASM
extern "C" void cBoardPressControl_blendOutOfPivot(cBoardPressControl*);
extern "C" void cRider_verifyRailslideModeDEBUG(void* rider);
extern "C" void cRider_applyJumpPop(void* rider, float v);
extern "C" void cSSXAudio_Lift(void* self, void* rider);
// PORT: prototype mismatch. cRider_setControlState/cRider_setMotionState are defined with one param, but callers pass a
// mode in $a1.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_1307B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Pops an ollie out of the board press when the ollie button (R3) is let go after being held.
 *
 * OllieHeld (bit 17) sets IsOllieHeld; once the bit is clear again, the pivot is finalised
 * (cBoardPressControl_blendOutOfPivot), clip 0x23 (|pivot| > 0.5) or 0x1B plays, any rail style is cleared,
 * and the rider takes off (cRider_applyJumpPop with 1.0, MS_Air, CS_Spin, cSSXAudio_Lift).
 *
 * @param cmd  the command (On Tour tBoardPressCmd)
 * @return 1 when the ollie was started, otherwise 0
 */
extern "C" int cBoardPressControl_checkForOllie(cBoardPressControl* self, void* cmd)
{
    int mask = 0x20000;
    if (*(int*)cmd & mask) self->IsOllieHeld = 1;
    int ok = self->IsOllieHeld != 0 && (*(int*)cmd & mask) == 0;
    float speed = aiAbs_1307B8(self->mRider->mSlewed[12].cur);
    if (!ok) return 0;
    cBoardPressControl_blendOutOfPivot(self);
    if (speed > 0.5f)
    {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 35, 0, -1.0f);
    }
    else
    {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 27, 0, -1.0f);
    }
    cRider* rider = self->mRider;
    if (rider->RailslideMode != 0)
    {
        rider->RailslideMode = 0;
        cRider_verifyRailslideModeDEBUG(self->mRider);
    }
    cRider_applyJumpPop(self->mRider, 1.0f);
    cRider_setMotionState_impl(self->mRider, 1);
    cRider_setControlState_impl(self->mRider, 5);
    cSSXAudio_Lift(gSSXAudio, self->mRider);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkForPivot);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cBoardPressControl_changeFakieBoardPress(cBoardPressControl* self);
extern "C" void cBoardPressControl_autoCompletePivot(cBoardPressControl*);
extern "C" void cBoardPressControl_blendOutOfPivot(cBoardPressControl*);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" float func_0031C228(float x);

struct sAiVec4_08D8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAiBits_08D8
{
    int lo : 6;
    int hi : 6;
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float aiDot_08D8(const sAiVec4_08D8& a, const sAiVec4_08D8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_08D8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_08D8(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float aiFloor_08D8(float x)
{
    float t;
    __asm__("cvt.w.s %0, %1\n\tcvt.s.w %0, %0" : "=f"(t) : "f"(x));
    if (x < t)
    {
        t -= 1.0f;
    }
    return t;
}

static inline float aiWrap_08D8(float x)
{
    return x - aiFloor_08D8(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

static inline void aiSetLean_08D8(char* r, float v)
{
    *(float*)(r + 0x284) = 0.0f;
    *(float*)(r + 0x280) = *(float*)(r + 0x288) = v;
}


/**
 * Drives the board pivot from the stick: turns the pivot toward the stick angle and plays the pivot clips.
 *
 * With both sticks under 0.1, when moving against mGroundForward, or with IsOllieHeld, the pivot springs back
 * (autoCompletePivot); once |pivot| is within 0.2 of 0 or 1 it is finalised (blendOutOfPivot; PressTimer = 1
 * when idle) and 0 is returned. Otherwise atan2(BoardPivot, BoardPress) (pivot negated in switch stance) is
 * compared with pivot * pi: outside a pivot clip a gap of 27 degrees or more kicks the pivot to +-0.15 or
 * +-0.85, else it turns by gap * 2 * mTimeScale / 60. Between 0.15 and 0.85 the board depth targets 0.5 and
 * the pivot clip plays (0x1D/0x1E nose, 0x25/0x26 tail, by sign), flipping the stance (changeFakieBoardPress)
 * as a pivot starts.
 *
 * @param vself  the board-press control
 * @param in  the command (On Tour tBoardPressCmd)
 * @return 1 while a pivot is in progress, 0 when there is none
 */
extern "C" int cBoardPressControl_checkForPivot(cBoardPressControl* vself, void* in)
{
    cBoardPressControl* self = vself;
    sAiBits_08D8* bits = (sAiBits_08D8*)((char*)in + 4);
    float stickX = bits->lo * 0.032258063554763794f;
    float stickY = bits->hi * 0.032258063554763794f;
    float aa = aiAbs_08D8(stickY);
    float ab = aiAbs_08D8(stickX);
    float stickMag = aa < ab ? ab : aa;
    char* rider0;
    if (stickMag < 0.1f || aiDot_08D8(*(sAiVec4_08D8*)((rider0 = ((char*)self->mRider)) + 0x1E0), *(sAiVec4_08D8*)(rider0 + 0x3A0)) < 0.0f || self->IsOllieHeld != 0)
    {
        cBoardPressControl_autoCompletePivot((cBoardPressControl*)self);
        float absPivot = aiAbs_08D8(self->mRider->mSlewed[12].cur);
        if (absPivot <= 0.2f || absPivot >= 0.8f)
        {
            if (stickMag < 0.1f)
                self->PressTimer = 1.0f;
            cBoardPressControl_blendOutOfPivot((cBoardPressControl*)self);
            return 0;
        }
        return 1;
    }
    if (*(int*)(rider0 + 0x320) != 0)
        stickY = -stickY;
    float ang = aiAtan2_08D8(stickY, stickX);
    float curAngle = self->mRider->mSlewed[12].cur * 3.1415927410125732f;
    float deltaAngle = aiWrap_08D8(ang - curAngle);
    int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    int special = anim == 37 || anim == 38 || anim == 29 || anim == 0x1E;
    if (!special && aiAbs_08D8(deltaAngle) >= 0.4712389409542084f)
    {
        cRider* rider = self->mRider;
        if (aiAbs_08D8(rider->mSlewed[12].cur) > 0.5f)
        {
            if (deltaAngle > 0.0f)
                aiSetLean_08D8(((char*)rider), -0.8500000238418579f);
            else
                aiSetLean_08D8(((char*)rider), 0.8500000238418579f);
        }
        else
        {
            if (deltaAngle < 0.0f)
                aiSetLean_08D8(((char*)rider), -0.15000000596046448f);
            else
                aiSetLean_08D8(((char*)rider), 0.15000000596046448f);
        }
    }
    else
    {
        cRider* rider = self->mRider;
        float step = deltaAngle * 0.01666666753590107f * (rider->mTimeScale * 2.0f);
        float nh;
        if (curAngle < 0.0f && step > 0.0f)
            nh = aiWrap_08D8(curAngle + step - 6.2831854820251465f);
        else if (curAngle > 0.0f && step < 0.0f)
            nh = aiWrap_08D8(curAngle + step + 6.2831854820251465f);
        else
            nh = aiWrap_08D8(curAngle + step);
        aiSetLean_08D8(((char*)self->mRider), nh * 0.31830987334251404f);
    }
    cRider* rider3 = self->mRider;
    float absPivot = aiAbs_08D8(rider3->mSlewed[12].cur);
    if (absPivot >= 0.15000000596046448f && absPivot <= 0.8500000238418579f)
    {
        rider3->mSlewed[11].target = 0.5f;
        rider3->mSlewed[11].rate = rider3->mTimeScale * 0.01666666753590107f;
        cRider* rider2 = self->mRider;
        int na;
        if (rider2->PressMode == 1)
            na = rider2->mSlewed[12].cur > 0.0f ? 29 : 0x1E;
        else
            na = rider2->mSlewed[12].cur > 0.0f ? 37 : 38;
        if (anim == na)
            return 1;
        if (!special)
            cBoardPressControl_changeFakieBoardPress((cBoardPressControl*)self);
        cRiderAnimBase_play(self->mRider->mRiderAnim, na, 0, -1.0f);
        return 1;
    }
    cBoardPressControl_blendOutOfPivot((cBoardPressControl*)self);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkForWipeout);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_002A1560(void* mgr, void* rider);
extern "C" void cRider_flagEventWipeout(void* rider, int id, int a, int b, void* info);

struct sAiBitsA_130DD0
{
    int v : 6;
};

struct sAiBitsB_130DD0
{
    int pad : 6;
    int v : 6;
};

struct sCurvePt_130DD0
{
    float x, y;
};

struct sCurve_130DD0
{
    sCurvePt_130DD0 p[4];
};

extern sCurve_130DD0* kBPEasyBalanceRate;
extern sCurve_130DD0* kBPHardBalanceRate;

struct sInfo_130DD0
{
    sQuat_12FEC8 pos;
    sQuat_12FEC8 dir;
    sQuat_12FEC8 up;
    float speed;
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sQuat_12FEC8 Scale_130DD0(const sQuat_12FEC8& v, float s)
{
    sQuat_12FEC8 r;
    int t;
    __asm__(
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

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sQuat_12FEC8 Div_130DD0(const sQuat_12FEC8& v, float s)
{
    sQuat_12FEC8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_130DD0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Lerp_130DD0(float x0, float y0, float x1, float y1, float x)
{
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

static inline float Curve_130DD0(const sCurve_130DD0* c, float x)
{
    if (c->p[1].x < x)
    {
        if (c->p[2].x < x)
        {
            if (c->p[3].x < x)
                return c->p[3].y;
            return Lerp_130DD0(c->p[2].x, c->p[2].y, c->p[3].x, c->p[3].y, x);
        }
        return Lerp_130DD0(c->p[1].x, c->p[1].y, c->p[2].x, c->p[2].y, x);
    }
    if (x < c->p[0].x)
        return c->p[0].y;
    return Lerp_130DD0(c->p[0].x, c->p[0].y, c->p[1].x, c->p[1].y, x);
}

/**
 * Sets the board-depth target from the press input, and crashes the rider when the board stays at full depth
 * for over a second.
 *
 * BalanceSec counts the time in the press and OffBalanceSec the time at full depth (rider + 0x274 == 1). The
 * target is the press (word 1 bits 0-5, negated for a tail press, at least 0), capped at 0.5 + BalanceSec / 2
 * in the first second and then snapped to 0 or 1 (1 when it or |BoardPivot| exceeds 0.5). The rate is 1 when
 * crossing 0.5, else a blend over 5 s of the curves kBPEasyBalanceRate and kBPHardBalanceRate, with the Whooh speech
 * (func_002A1560) above depth 0.8. After 1 s at full depth it calls the hard crash cRider_flagEventWipeout with
 * semantic 0x167 (tail) or 0x166 (nose).
 *
 * @param in  the command (On Tour tBoardPressCmd)
 * @return 1 when the crash was started, otherwise 0
 */
extern "C" int cBoardPressControl_checkForWipeout(cBoardPressControl* self, void* in)
{
    cRider* rider = self->mRider;
    float k = rider->mTimeScale * 0.01666666753590107f;
    self->BalanceSec += k;
    if (rider->mSlewed[11].cur == 1.0f)
        self->OffBalanceSec += k;
    else
        self->OffBalanceSec = 0.0f;
    float desPress = ((sAiBitsA_130DD0*)((char*)in + 4))->v * 0.032258063554763794f;
    if (self->mRider->PressMode == 2)
        desPress = -desPress >? 0.0f;
    else
        desPress = desPress >? 0.0f;
    if (self->BalanceSec <= 1.0f)
    {
        desPress = desPress <? self->BalanceSec * 0.5f + 0.5f;
    }
    else
    {
        float absStickY = aiAbs_130DD0(((sAiBitsB_130DD0*)((char*)in + 4))->v * 0.032258063554763794f);
        float stickMag = (desPress <= absStickY) ? absStickY : desPress;
        desPress = 0.0f;
        if (stickMag > 0.5f)
            desPress = 1.0f;
    }
    float desRate;
    if ((desPress <= 0.5f && self->mRider->mSlewed[11].cur > 0.5f) || (desPress >= 0.5f && self->mRider->mSlewed[11].cur < 0.5f))
    {
        desRate = 1.0f;
    }
    else
    {
        float easyRate = Curve_130DD0(kBPEasyBalanceRate, self->mRider->mSlewed[11].cur);
        float hardRate = Curve_130DD0(kBPHardBalanceRate, self->mRider->mSlewed[11].cur);
        float mix = self->BalanceSec * 0.20000895857810974f <? 1.0f;
        desRate = easyRate * (1.0f - mix) + hardRate * mix;
        if (self->mRider->mSlewed[11].cur > 0.800000011920929f)
            func_002A1560(cAudioInterface_bxAudio(), self->mRider);
    }
    cRider* rider3 = self->mRider;
    rider3->mSlewed[11].target = desPress;
    rider3->mSlewed[11].rate = desRate * k;
    if (self->OffBalanceSec > 1.0f)
    {
        sInfo_130DD0 info;
        cRider* rider4 = self->mRider;
        float speed = aiLength_12C5C8(*(sAiVec4_12C5C8*)&rider4->mVelocity);
        info.pos = *(sQuat_12FEC8*)&rider4->mPosition;
        info.up = Scale_130DD0(t4vect_ZAxis, -1.0f);
        info.speed = speed;
        if (speed > 0.0010000000474974513f)
            info.dir = Div_130DD0(*(sQuat_12FEC8*)&self->mRider->mVelocity, speed);
        else
            info.dir = t4vect_ZAxis;
        cRider* rider5 = self->mRider;
        if (rider5->PressMode == 2)
            cRider_flagEventWipeout(rider5, 359, 0, rider5->mSurfaceType, &info);
        else
            cRider_flagEventWipeout(rider5, 358, 0, rider5->mSurfaceType, &info);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_checkForExit);
#ifdef SKIP_ASM
struct sAiStick_131200
{
    int pad0;
    int x : 6;
    int y : 6;
};

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_131200(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Decides when a balanced board press ends: after 0.5 s without input, or when the stick is pushed the other
 * way.
 *
 * PressTimer counts the time with BoardPress at 0 and |BoardPivot| < 0.5. Past 0.5 s, or with BoardPress
 * beyond 0.8 against the press (above 0.8 for tail, below -0.8 for nose), the board-depth target (rider +
 * 0x27C) becomes 0 at 2 * mTimeScale / 60.
 *
 * @param cmd  the command (On Tour tBoardPressCmd)
 * @return 1 when the board depth (rider + 0x274) is 0, or under 0.5 on a tick that asks to exit; else 0
 */
extern "C" int cBoardPressControl_checkForExit(cBoardPressControl* self, void* cmd)
{
    float x = ((sAiStick_131200*)cmd)->x * 0.032258063554763794f;
    float y = ((sAiStick_131200*)cmd)->y * 0.032258063554763794f;
    if (x == 0.0f && aiAbs_131200(y) < 0.5f)
    {
        self->PressTimer += self->mRider->mTimeScale * 0.01666666753590107f;
    }
    else
    {
        self->PressTimer = 0.0f;
    }
    cRider* rider = self->mRider;
    if (self->PressTimer > 0.5f ||
        (rider->PressMode == 2 && x > 0.8f) ||
        (rider->PressMode == 1 && x < -0.8f))
    {
        *(int*)&rider->mSlewed[11].target = 0;
        rider->mSlewed[11].rate = rider->mTimeScale * 2.0f * 0.01666666753590107f;
        if (self->mRider->mSlewed[11].cur < 0.5f) return 1;
    }
    if (self->mRider->mSlewed[11].cur == 0.0f) return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_forceExit);
#ifdef SKIP_ASM
extern "C" void cBoardPressControl_autoCompletePivot(cBoardPressControl*);
extern "C" void cBoardPressControl_blendOutOfPivot(cBoardPressControl*);
extern "C" float cRiderMetrix_evPressEnd(void*);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);

/**
 * Ends a board press early: settles the pivot and scores the press.
 *
 * Runs cBoardPressControl_autoCompletePivot() and cBoardPressControl_blendOutOfPivot(), then scores the end
 * of a nose or tail press (cRiderMetrix_evPressEnd through cRider_flagEventBoostPowerUp). Called by the jump request
 * cRider_checkForPrewind and the soft-collision handler cRider_doBump.
 */
extern "C" void cBoardPressControl_forceExit(cBoardPressControl* self)
{
    cBoardPressControl_autoCompletePivot(self);
    cBoardPressControl_blendOutOfPivot(self);
    cRider* rider = self->mRider;
    int pressType = rider->PressMode;
    if (pressType >= 1 && pressType <= 2)
    {
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressEnd(rider->mMetrix));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_autoCompletePivot);
#ifdef SKIP_ASM
/**
 * Springs the board pivot toward its nearest rest position (-1, 0 or 1).
 *
 * Sets the pivot target (rider + 0x288) to -1 below -0.5, 1 above 0.5, else 0, with rate (rider + 0x284)
 * mTimeScale * 0.025.
 */
extern "C" void cBoardPressControl_autoCompletePivot(cBoardPressControl* self)
{
    cRider* rider = self->mRider;
    if (rider->mSlewed[12].cur < -0.5f)
    {
        rider->mSlewed[12].target = -1.0f;
        rider->mSlewed[12].rate = rider->mTimeScale * 0.02500000223517418f;
    }
    else if (rider->mSlewed[12].cur > 0.5f)
    {
        rider->mSlewed[12].target = 1.0f;
        rider->mSlewed[12].rate = rider->mTimeScale * 0.02500000223517418f;
    }
    else
    {
        rider->mSlewed[12].target = 0.0f;
        rider->mSlewed[12].rate = rider->mTimeScale * 0.02500000223517418f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_blendOutOfPivot);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" float cRiderMetrix_evPressPivot(void* p, int v);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Ends a board-press pivot: completes it into the other press or turns back to the press it started from.
 *
 * For pivot anims 0x25/0x26 a pivot (mSlewed[12]) of at most 0.5 scores a half turn (cRiderMetrix_evPressPivot), plays
 * press anim 0x1C and sets PressMode 1; otherwise it calls changeFakieBoardPress() (probably undoing the
 * stance swap checkForPivot made) and plays 0x24. Pivots 0x1D/0x1E mirror this but complete at 0.5 or more.
 */
extern "C" void cBoardPressControl_blendOutOfPivot(cBoardPressControl* self)
{
    int curAnimId = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    if (curAnimId >= 37 && curAnimId <= 38)
    {
        cRider* rider = self->mRider;
        if (aiAbs_12F118(rider->mSlewed[12].cur) <= 0.5f)
        {
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressPivot(rider->mMetrix, rider->PressMode));
            cRiderAnimBase_play(self->mRider->mRiderAnim, 0x1C, 0, -1.0f);
            self->mRider->PressMode = 1;
        }
        else
        {
            cBoardPressControl_changeFakieBoardPress(self);
            cRiderAnimBase_play(self->mRider->mRiderAnim, 36, 0, -1.0f);
        }
    }
    else if (curAnimId >= 29 && curAnimId <= 0x1E)
    {
        cRider* rider = self->mRider;
        if (aiAbs_12F118(rider->mSlewed[12].cur) >= 0.5f)
        {
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evPressPivot(rider->mMetrix, rider->PressMode));
            cRiderAnimBase_play(self->mRider->mRiderAnim, 36, 0, -1.0f);
            self->mRider->PressMode = 2;
        }
        else
        {
            cBoardPressControl_changeFakieBoardPress(self);
            cRiderAnimBase_play(self->mRider->mRiderAnim, 0x1C, 0, -1.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00131598 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the board press state to a replay frame.
 *
 * Writes the 0x14 bytes at offset 0 (State .. IsOllieHeld) through the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cBoardPressControl_writeToReplayFrame(cBoardPressControl* self, cReplayFramePtr* obj)
{
    sVEntry00131598* vt = *(sVEntry00131598**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cBoardPressControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001315D0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the board press state from a replay frame.
 *
 * Reads the 0x14 bytes at offset 0 (State .. IsOllieHeld) through the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cBoardPressControl_readFromReplayFrame(cBoardPressControl* self, cReplayFramePtr* obj)
{
    sVEntry001315D0* vt = *(sVEntry001315D0**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cCruiseControl_gainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Prepares cruising: clears the rider's proximity timer and its prewind gate.
 *
 * Zeroes unk_35C (counted up by cRider_checkForProximityAction) and the word at +0x360, which cRider_checkForPrewind
 * reads as a float: while it is 0 a held prewind button starts a prewind straight away.
 */
void cCruiseControl_gainFocus(void* self)
{
    *(int*)((char*)*(void**)self + 0x35c) = 0;
    *(int*)((char*)*(void**)self + 0x360) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cCruiseControl_update);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int cRider_checkForFinishLine(void*);
extern "C" int cRider_checkForReset_1620(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cCruiseControl_checkAirMotion(cCruiseControl* self);
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" int cRider_checkHandplantCollisions(void* rider, int on);
extern "C" int cRider_checkForPrewind(void*, int, int);
extern "C" int cRider_checkForBoardPress(void* rider, float v);
extern "C" int cRider_checkForPunch(void* rider, int a, int b);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cRider_setupLean(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" void cRider_checkForReaction(void*);
extern "C" void cRider_checkForProximityAction(void*);
extern "C" int cRider_isChangeFakieGround(void* rider);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
unsigned int AIrand();

struct sAiVec4_1620
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAiBitsX_1620
{
    int pad : 20;
    int v : 6;
};

struct sAiBitsY_1620
{
    int pad : 26;
    int v : 6;
};

struct sAiBitsC_1620
{
    int v : 6;
};

struct sAiBitsD_1620
{
    int pad : 6;
    int v : 6;
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float aiDot_1620(const sAiVec4_1620& a, const sAiVec4_1620& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float aiLength_1620(const sAiVec4_1620& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_1620(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_1620(float v, float lo, float hi)
{
    float r = lo;
    if (v >= lo)
        r = v <? hi;
    return r;
}

/**
 * Runs one frame of cruising (riding on the ground): checks the ways out of cruise, applies the stick and
 * buttons, and keeps the riding anim in step with speed, lean and braking.
 *
 * Leaves early at the finish line (cRider_checkForFinishLine), on a reset (bit 12), in the air, on a handplant (bit 13),
 * a rail or board collision, a prewind (bits 14-15) or a board press (word 1 bits 6-11). It then sets boost,
 * steering, crouch and brake (mSlewed[0], [4], [3]) and picks push-off 0x16, turnaround 0x15, carves 5-8 and
 * 0xE-0x11 or brake slides 0xB-0xD.
 *
 * @param in  the cruise command (On Tour tCruiseCmd): button bits and 6-bit stick values scaled by 1/31
 */
extern "C" void cCruiseControl_update(cCruiseControl* self, void* in)
{
    float floatZero = 0.0f;
    float x = ((sAiBitsX_1620*)in)->v * 0.032258063554763794f;
    float y = ((sAiBitsY_1620*)in)->v * 0.032258063554763794f;
    float desBrake = ((sAiBitsC_1620*)((char*)in + 4))->v * 0.032258063554763794f;
    char* rider = *(char**)self;
    if (aiDot_1620(*(sAiVec4_1620*)(rider + 0x1E0), *(sAiVec4_1620*)(rider + 0x3A0)) < floatZero)
        desBrake = floatZero;
    if (cRider_checkForFinishLine(rider))
        return;
    if (cRider_checkForReset_1620(*(void**)self, (*(int*)in >> 12) & 1, 0))
        return;
    if (cCruiseControl_checkAirMotion(self))
        return;
    if (*(int*)in & 0x2000)
    {
        if (cRider_checkHandplantCollisions(*(void**)self, y > 0.5f))
            return;
        x = aiClamp_1620(x, -0.5f, 0.5f);
        y = 1.0f;
        desBrake = floatZero;
    }
    else
    {
        if (cRider_checkRailBoardCollisions(*(void**)self))
            return;
    }
    int w = *(int*)in;
    if (cRider_checkForPrewind(*(void**)self, (w >> 15) & 1, (w >> 14) & 1))
        return;
    if (*(int*)in & 0x8000)
    {
        y = 1.0f;
        desBrake = 0.0f;
    }
    if (cRider_checkForBoardPress(*(void**)self, ((sAiBitsD_1620*)((char*)in + 4))->v * 0.032258063554763794f))
        return;
    int w2 = *(int*)in;
    if (cRider_checkForPunch(*(void**)self, (w2 >> 18) & 1, (w2 >> 19) & 1))
        x = aiClamp_1620(x, -0.5f, 0.5f);
    int w3 = *(int*)in;
    cRider_setupBoostWarp(*(void**)self, (w3 >> 17) & 1, (w3 >> 16) & 1);
    cRider_setupLean(*(void**)self, x);
    cRider_setupCrouchBrake(*(void**)self, y, desBrake);
    cRider_checkForReaction(*(void**)self);
    cRider_checkForProximityAction(*(void**)self);
    int cur = cRiderAnimBase_getCurrentAnimID(*(void**)(*(char**)self + 0x784), 2);
    int anim = cur;
    if (cur == 22)
    {
        char* rider1 = *(char**)self;
        *(int*)(rider1 + 0x204) = 0;
        *(float*)(rider1 + 0x200) = 0.03333333507180214f;
        char* rider2 = *(char**)self;
        *(int*)(rider2 + 0x21C) = 0;
        *(float*)(rider2 + 0x218) = 0.03333333507180214f;
        char* rider3 = *(char**)self;
        *(float*)(rider3 + 0x224) = 0.03333333507180214f;
        *(int*)(rider3 + 0x228) = 0;
    }
    else if (cur == 21 || (*(float*)(*(char**)self + 0x2DC) == 0.0f && cRider_isChangeFakieGround(*(void**)self)))
    {
        anim = 21;
        char* rider1 = *(char**)self;
        *(int*)(rider1 + 0x204) = 0;
        *(float*)(rider1 + 0x200) = 0.03333333507180214f;
        char* rider2 = *(char**)self;
        *(float*)(rider2 + 0x218) = 0.03333333507180214f;
        *(int*)(rider2 + 0x21C) = 0;
    }
    else if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(*(char**)self + 0x784), 2) == 10)
    {
        char* rider1 = *(char**)self;
        *(int*)(rider1 + 0x204) = 0;
        *(float*)(rider1 + 0x200) = 0.03333333507180214f;
    }
    else if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(*(char**)self + 0x784), 2) == 5)
    {
        char* rider1 = *(char**)self;
        *(float*)(rider1 + 0x200) = 0.03333333507180214f;
        *(float*)(rider1 + 0x204) = *(float*)(rider1 + 0x1F0);
        char* rider2 = *(char**)self;
        *(float*)(rider2 + 0x218) = 0.03333333507180214f;
        *(int*)(rider2 + 0x21C) = 0;
    }
    else
    {
        char* rider1 = *(char**)self;
        float spd = aiLength_1620(*(sAiVec4_1620*)(rider1 + 0x1E0));
        if (*(float*)(rider1 + 0x214) == 0.0f)
        {
            *(float*)(rider1 + 0x204) = *(float*)(rider1 + 0x1F0);
            *(float*)(rider1 + 0x200) = 0.03333333507180214f;
            if (spd < 833.3333740234375f && (y > 0.0f || aiAbs_1620(x) > 0.2f))
            {
                anim = 22;
            }
            else
            {
                char* rider2 = *(char**)self;
                if (*(int*)(rider2 + 0x438) == 4)
                {
                    float absTurn = aiAbs_1620(*(float*)(rider2 + 0x1F0));
                    if (*(float*)(rider2 + 0x220) <= (1.0f - absTurn) * 0.5f)
                        anim = 0xE;
                    else if (*(float*)(rider2 + 0x2FC) > 0.0f && absTurn < 0.30000001192092896f)
                        anim = 8;
                    else
                        anim = 0xF;
                }
                else if (aiAbs_1620(*(float*)(rider2 + 0x1FC)) > 0.6000000238418579f &&
                         aiAbs_1620(*(float*)(rider2 + 0x3B8)) > 0.699999988079071f &&
                         (AIrand() & 1) == 0)
                {
                    char* rider3 = *(char**)self;
                    float animTurn = *(float*)(rider3 + 0x1FC);
                    if (*(int*)(rider3 + 0x320) != 0)
                        animTurn = -animTurn;
                    anim = animTurn < 0.0f ? 0x10 : 17;
                }
                else
                {
                    char* rider3 = *(char**)self;
                    if (*(float*)(rider3 + 0x220) <= (1.0f - aiAbs_1620(*(float*)(rider3 + 0x1F0))) * 0.5f)
                        anim = 5;
                    else if (*(float*)(rider3 + 0x2FC) > 0.0f)
                        anim = 8;
                    else if (spd < 1666.666748046875f || (unsigned int)(*(int*)(rider3 + 0x438) - 2) < 2)
                        anim = 7;
                    else
                        anim = 6;
                }
            }
        }
        else
        {
            *(float*)(rider1 + 0x204) = 0.0f;
            *(float*)(rider1 + 0x200) = 0.03333333507180214f;
            float lim = 694.4444580078125f;
            if (cur == 0xC || cur == 13)
                lim = 972.2222290039062f;
            if (lim < spd)
            {
                anim = 11;
            }
            else
            {
                char* rider2 = *(char**)self;
                float brake = *(float*)(rider2 + 0x214);
                if (aiAbs_1620(brake) < 0.800000011920929f)
                    anim = 11;
                else if (brake > 0.0f)
                    anim = *(int*)(rider2 + 0x320) ? 0xC : 13;
                else
                    anim = *(int*)(rider2 + 0x320) ? 13 : 0xC;
            }
        }
    }
    if (anim != cur)
        cRiderAnimBase_play(*(void**)(*(char**)self + 0x784), anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cCruiseControl_loseFocus);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRiderAnimBase_blendOutAll(void*, int, float);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);

/**
 * Leaves cruising; unless natural air follows, it fades out anim layer 0 while a punch plays on layer 1.
 *
 * A punch is a layer-1 anim of type 3 or 0xD (see cRider_checkForPunch); layer 0 fades over 0.1 s
 * (cRiderAnimBase_blendOutAll) and layer 1's playback rate (+0x90) is set back to 1.
 *
 * @param next  the control state being entered (eControlState; CS_NaturalAir, 4, skips this)
 */
extern "C" void cCruiseControl_loseFocus(cCruiseControl* self, int next)
{
    if (next != 4)
    {
        if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(*(char**)self + 0x784), 1) == 3 ||
            cRiderAnimBase_getCurrentAnimCategory(*(void**)(*(char**)self + 0x784), 1) == 13)
        {
            cRiderAnimBase_blendOutAll(*(void**)(*(char**)self + 0x784), 0, 0.10000000149011612f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(*(void**)(*(char**)self + 0x784), 1) + 0x90) = 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cCruiseControl_checkAirMotion);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");

/**
 * Switches to natural-air control once the rider has left the ground.
 *
 * @return 1 when the motion state is MS_Air and the control state was set to CS_NaturalAir, otherwise 0
 */
extern "C" int cCruiseControl_checkAirMotion(cCruiseControl* self)
{
    if (cRider_getMotionState(*(void**)self) == 1)
    {
        cRider_setControlState_impl(*(void**)self, 4);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_gainFocus);
#ifdef SKIP_ASM
/**
 * Prepares rail sliding: clears the rail spin angle, the prewind gate and the remembered grind button.
 *
 * Sets RSpinAngle 0, the rider word at +0x360 0 (see cCruiseControl_gainFocus) and PrevGrindHeld -1, and
 * lets the lean channel mSlewed[1] return to 0 (rate 1/15).
 */
extern "C" void cRailslideControl_gainFocus(cRailslideControl* self)
{
    *(int*)&self->RSpinAngle = 0;
    *(int*)&self->mRider->JumpHeldSec = 0;
    self->PrevGrindHeld = -1;
    cRider* rider = self->mRider;
    rider->mSlewed[1].rate = 0.06666667014360428f;
    *(int*)&rider->mSlewed[1].target = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_update);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool tEventFlags_isSet1(void* self, int bit);
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkForBoardPress(void* rider, float v);
extern "C" void cRailslideControl_chooseRailSpin(cRailslideControl* self, int neg);
extern "C" int cRailslideControl_checkForUberGrind(cRailslideControl* self, int id);
extern "C" int cRailslideControl_checkAirMotion(cRailslideControl* self, void* a1);

struct sPadA_131D30
{
    int pad : 17;
    int id : 8;
};

struct sPadB_131D30
{
    int pad : 25;
    int v : 6;
};

struct sPadC_131D30
{
    int x : 6;
    int y : 6;
};

static inline float clamp_131D30(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

/**
 * Runs one frame of rail sliding: checks the ways off the rail, then handles board presses, rail spins and
 * the balance lean.
 *
 * Leaves early on a reset (bit 12), in the air, on a prewind (bits 13-14) or an uber grind (grind id in bits
 * 17-24); sets boost (bits 15-16) and rail balance (bits 25-30). Word 1 bits 6-11 start a board press and
 * bits 0-5 a rail spin (chooseRailSpin; during a spin anim, type 0xE, only after its event 0). Otherwise the
 * lean mSlewed[6] follows RailOffCenter and the slide anim 0x12-0x14 matches RailslideMode.
 *
 * @param in  the rail slide command (On Tour tRailslideCmd): button bits and 6-bit stick values
 */
extern "C" void cRailslideControl_update(cRailslideControl* self, void* in)
{
    if (cRider_checkForReset_i(self->mRider, (*(int*)in >> 12) & 1, 0))
        return;
    if (cRailslideControl_checkAirMotion(self, in))
        return;
    int w = *(int*)in;
    if (cRider_checkForPrewind(self->mRider, (w >> 14) & 1, (w >> 13) & 1))
        return;
    if (cRailslideControl_checkForUberGrind(self, ((sPadA_131D30*)in)->id))
        return;
    int w2 = *(int*)in;
    float k = 0.032258063554763794f;
    float floatZero = 0.0f;
    cRider_setupBoostWarp(self->mRider, (w2 >> 16) & 1, (w2 >> 15) & 1);
    cRider_setupRailBalance(self->mRider, ((sPadB_131D30*)in)->v * k);
    cRider_setupCrouchBrake(self->mRider, floatZero, floatZero);
    cRider_checkForReaction(self->mRider);
    cRider_checkForProximityAction(self->mRider);
    sPadC_131D30* in2 = (sPadC_131D30*)((char*)in + 4);
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 0xE) {
        if (!tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 0))
            return;
        if (cRider_checkForBoardPress(self->mRider, in2->y * k))
            return;
        float fy = in2->x * k;
        if (fy == floatZero)
            return;
        cRailslideControl_chooseRailSpin(self, fy < floatZero);
        return;
    }
    if (cRider_checkForBoardPress(self->mRider, in2->y * k))
        return;
    float fx = in2->y * k;
    float fy = in2->x * k;
    float desRailSpin = fy;
    if (fx != floatZero && fy == floatZero)
        desRailSpin = fx;
    if (desRailSpin != 0.0f) {
        cRider* rider = self->mRider;
        rider->mSlewed[6].target = 0.0f;
        rider->mSlewed[6].rate = 0.06666667014360428f;
        cRailslideControl_chooseRailSpin(self, desRailSpin < 0.0f);
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 10) {
        cRider* rider = self->mRider;
        rider->mSlewed[6].target = 0.0f;
        rider->mSlewed[6].rate = 0.06666667014360428f;
        return;
    }
    cRider* rider = self->mRider;
    rider->mSlewed[6].target = clamp_131D30(rider->mRiderHandler->RailslideMotion.RailOffCenter * 1.2000000476837158f, -1.0f, 1.0f);
    rider->mSlewed[6].rate = 0.06666667014360428f;
    int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
    cRider* rider2 = self->mRider;
    int railStyle = rider2->RailslideMode;
    int anim;
    if (railStyle == 4)
        anim = 20;
    else {
        anim = 18;
        if (railStyle == 3) anim = 19;
    }
    if (cur != anim)
        cRiderAnimBase_play(rider2->mRiderAnim, anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_loseFocus);
#ifdef SKIP_ASM
/**
 * Leaves rail sliding: lets the rail lean channel mSlewed[6] return to 0 (rate 1/15).
 */
extern "C" void cRailslideControl_loseFocus(cRailslideControl* self)
{
    cRider* rider = self->mRider;
    rider->mSlewed[6].rate = 0.06666667014360428f;
    *(int*)&rider->mSlewed[6].target = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_chooseRailSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" int cRiderAnimBase_play_2060(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void cRider_verifyRailslideModeDEBUG(void* rider);
float cRiderMetrix_evRailSpin_2060(void* p, int st, float h) __asm__("cRiderMetrix_evRailSpin__FPvi");

struct sQuat_2060
{
    float x, y, z, w;
    sQuat_2060() {}
    sQuat_2060(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_2060 D_004FF130_2060 __asm__("t4vect_Origin");
extern sQuat_2060 D_004FF160_2060 __asm__("t4vect_ZAxis");

static inline sQuat_2060 AxisAngle_2060(const sQuat_2060& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_2060(s * axis.x, s * axis.y, s * axis.z, c);
}

#define RIDER_2060 (*(char**)((char*)self + 8))

static inline void setRot_2060(char* anim, float a)
{
    *(sQuat_2060*)(anim + 0x30) = D_004FF130_2060;
    *(sQuat_2060*)(anim + 0x40) = AxisAngle_2060(D_004FF160_2060, -a);
}

static inline void flip_2060(void* self, int f)
{
    *(int*)(RIDER_2060 + 0x320) = f;
    *(int*)(*(char**)(RIDER_2060 + 0x784) + 0x18) = f;
    *(float*)(RIDER_2060 + 0x238) = -*(float*)(RIDER_2060 + 0x238);
    *(float*)(RIDER_2060 + 0x240) = -*(float*)(RIDER_2060 + 0x240);
}
/**
 * Turns the rider a quarter turn on the rail, switching the rail stance (RailslideMode) and playing its anim.
 *
 * With @p dir set RSpinAngle drops by pi/2 and the stance goes 1->3->2->4->1, else it grows by pi/2 and goes
 * 1->4->2->3->1. Entering stance 2 sets the switch flag IsRidingFakie and the anim mirror, leaving it clears them;
 * the anim heading offset follows the stance (anims 0x31-0x36). The spin is scored with cRiderMetrix_evRailSpin().
 *
 * @param dir  nonzero for one direction (On Tour isCCW), zero for the other
 */
extern "C" void cRailslideControl_chooseRailSpin(cRailslideControl* self, int dir)
{
    if (dir != 0) {
        *(float*)self -= 1.5707964897155762f;
        switch (*(int*)(RIDER_2060 + 0x328)) {
    case 1:
        setRot_2060(*(char**)(RIDER_2060 + 0x784), -1.5707963705062866f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 49, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 3;
        break;
    case 2:
        flip_2060(self, 0);
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 1.5707963705062866f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 53, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 4;
        break;
    case 3:
        flip_2060(self, 1);
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 3.1415927410125732f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 51, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 2;
        break;
    case 4:
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 0.0f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 54, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 1;
        break;
        }
    } else {
        *(float*)self += 1.5707964897155762f;
        switch (*(int*)(RIDER_2060 + 0x328)) {
    case 1:
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 1.5707963705062866f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 50, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 4;
        break;
    case 2:
        flip_2060(self, 0);
        setRot_2060(*(char**)(RIDER_2060 + 0x784), -1.5707963705062866f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 52, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 3;
        break;
    case 3:
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 0.0f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 51, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 1;
        break;
    case 4:
        flip_2060(self, 1);
        setRot_2060(*(char**)(RIDER_2060 + 0x784), 3.1415927410125732f);
        cRiderAnimBase_play_2060(*(void**)(RIDER_2060 + 0x784), 54, 0, -1.0f);
        *(int*)(RIDER_2060 + 0x328) = 2;
        break;
        }
    }
    cRider_flagEventBoostPowerUp(RIDER_2060, 1, cRiderMetrix_evRailSpin_2060(*(void**)(RIDER_2060 + 0x790), *(int*)(RIDER_2060 + 0x328), *(float*)self));
    cRider_verifyRailslideModeDEBUG(RIDER_2060);
}
#undef RIDER_2060
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_checkForUberGrind);
#ifdef SKIP_ASM
// PORT: prototype mismatch. cRider_setControlState is defined with one param, but its body
// forwards $5 to cRiderHandler__setControlState.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_UberRailDeny(void* snd, void* rider);

/**
 * Starts an uber grind when a grind button is held and an uber is available, else plays the deny sound.
 *
 * With mUberTimer > 0 and mUberFlags bit 1 set it stores @p id as the uber grind control's CurrentGrind
 * (handler + 0x394) and switches to CS_UberGrind. A refused id plays cSSXAudio_UberRailDeny() once per new
 * press; PrevGrindHeld remembers the id.
 *
 * @param id  the grind held this frame (-1 for none; On Tour grindHeld)
 * @return 1 when the uber grind started, otherwise 0
 */
extern "C" int cRailslideControl_checkForUberGrind(cRailslideControl* self, int id)
{
    if (id != -1)
    {
        cRider* rider = self->mRider;
        if (rider->mUberTimer > 0.0f && ((rider->mUberFlags >> 1) & 1))
        {
            rider->mRiderHandler->UberGrindControl.CurrentGrind = id;
            cRider_setControlState_impl(self->mRider, 0xC);
            return 1;
        }
        if (id != self->PrevGrindHeld)
        {
            cSSXAudio_UberRailDeny(cAudioInterface_bxAudio(), self->mRider);
        }
    }
    self->PrevGrindHeld = id;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_playGainFocusAnim);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

/**
 * Plays the anim for getting onto a rail in the current rail stance.
 *
 * Stance 4 plays 0x14, stance 3 0x13, any other 0x12; with @p alt set the variants 0x45, 0x46 and 0x44.
 * cRider_checkRailBoardCollisions passes 1 when the rider lands on the rail from natural air or spin control.
 *
 * @param alt  nonzero for the landing variants (On Tour isLanding)
 */
extern "C" void cRailslideControl_playGainFocusAnim(cRailslideControl* self, int alt)
{
    cRider* rider = self->mRider;
    int railStyle = rider->RailslideMode;
    if (railStyle == 4)
    {
        if (alt)
            cRiderAnimBase_play(rider->mRiderAnim, 69, 0, -1.0f);
        else
            cRiderAnimBase_play(rider->mRiderAnim, 20, 0, -1.0f);
    }
    else if (railStyle == 3)
    {
        if (alt)
            cRiderAnimBase_play(rider->mRiderAnim, 70, 0, -1.0f);
        else
            cRiderAnimBase_play(rider->mRiderAnim, 19, 0, -1.0f);
    }
    else
    {
        if (alt)
            cRiderAnimBase_play(rider->mRiderAnim, 68, 0, -1.0f);
        else
            cRiderAnimBase_play(rider->mRiderAnim, 18, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_checkAirMotion);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
// PORT: prototype mismatch. cRider_setControlState is defined with one param, but its body
// forwards $5 to cRiderHandler__setControlState.
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" bool tEventFlags_isSet1(void* self, int bit);
extern "C" void cRider_undoRailslideModeOrientation(void* rider);

struct sAiBits00132770
{
    int v : 6;
};

/**
 * Hands control to the air states when the rider has come off the rail into the air.
 *
 * Waits while a rail spin anim (type 0xE) has not reached its event 0, then leaves the rail stance
 * (cRider_undoRailslideModeOrientation) and switches to CS_Spin when the rail spin input (word 1 bits 0-5) is nonzero, else to
 * CS_NaturalAir.
 *
 * @param cmd  the rail slide command (On Tour tRailslideCmd)
 * @return 1 when the control state was changed, 0 when not in the air or still waiting
 */
extern "C" int cRailslideControl_checkAirMotion(cRailslideControl* self, void* cmd)
{
    if (cRider_getMotionState(self->mRider) != 1)
    {
        return 0;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 0xE)
    {
        if (!tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 0))
        {
            return 0;
        }
    }
    cRider_undoRailslideModeOrientation(self->mRider);
    if (((sAiBits00132770*)((char*)cmd + 4))->v * 0.032258063554763794f != 0.0f)
    {
        cRider_setControlState_impl(self->mRider, 5);
    }
    else
    {
        cRider_setControlState_impl(self->mRider, 4);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00132840 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the rail slide state to a replay frame.
 *
 * Writes the 8 bytes at offset 0 (RSpinAngle, PrevGrindHeld) through the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cRailslideControl_writeToReplayFrame(cRailslideControl* self, cReplayFramePtr* obj)
{
    sVEntry00132840* vt = *(sVEntry00132840**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cRailslideControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00132878 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the rail slide state from a replay frame.
 *
 * Reads the 8 bytes at offset 0 (RSpinAngle, PrevGrindHeld) through the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cRailslideControl_readFromReplayFrame(cRailslideControl* self, cReplayFramePtr* obj)
{
    sVEntry00132878* vt = *(sVEntry00132878**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_HandPlantSound(void* self, void* rider);
extern "C" float cRiderAnimBase_getAIAnimDuration(char* self, int a1);
extern "C" void cHandplantControl_playHandplantAnim(cHandplantControl* self, int a1, int a2);
extern "C" void cRiderAnimBase_blendOutAll(void*, int, float);
extern char kHandplantAnim[];

/**
 * Starts a handplant: resets the steering channels, plays the sound and an entry anim timed to EnterSec.
 *
 * Sets the rider's handplant mode (HandplantMode) to 1 and scales the anim speed (+0x1C) so that mode's anim from
 * kHandplantAnim[mode][HandplantType] lasts EnterSec seconds while playHandplantAnim() starts it. Then clears
 * CurBalance and fades out anim layers 1 and 0 over 0.33 s.
 */
extern "C" void cHandplantControl_gainFocus(cHandplantControl* self)
{
    float k = 0.05000000447034836f;
    float blend = 0.33000001311302185f;
    cRider* rider = self->mRider;
    rider->mSlewed[1].rate = k;
    *(int*)&rider->mSlewed[1].target = 0;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[0].rate = k;
    *(int*)&rider2->mSlewed[0].target = 0;
    self->mRider->HandplantMode = 1;
    cSSXAudio_HandPlantSound(cAudioInterface_bxAudio(), self->mRider);
    cRider* rider3 = self->mRider;
    float enterAnimSec = cRiderAnimBase_getAIAnimDuration((char*)rider3->mRiderAnim, *(int*)(kHandplantAnim + self->HandplantType * 8 + rider3->HandplantMode * 16));
    *(float*)((char*)self->mRider->mRiderAnim + 0x1C) = enterAnimSec / *(float*)self;
    cHandplantControl_playHandplantAnim(self, self->mRider->HandplantMode, self->HandplantType);
    *(float*)((char*)self->mRider->mRiderAnim + 0x1C) = 1.0f;
    *(int*)&self->CurBalance = 0;
    cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, blend);
    cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 0, blend);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_init);
#ifdef SKIP_ASM
struct sV4_29B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Length_29B0(const sV4_29B0& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

struct sRider_29B0 {
    char pad[0x1E0];
    sV4_29B0 vel;
};

struct sFlags_29B0 {
    char pad[0x1C];
    int flags;
};

struct sOwner_29B0 {
    char pad[0x58];
    sFlags_29B0* ctl;
};

struct sState_29B0 {
    float t;            // 0x0
    int f4;             // 0x4
    int fast;           // 0x8
    int ctl;            // 0xC
    int flag;           // 0x10
    sRider_29B0* rider; // 0x14
};

/**
 * Sets up a handplant from the intersection that started it: entry time, toeside, speed and a surface flag.
 *
 * Stores @p enterSec (+0x0) and @p flag as a 0/1 bool (+0x10), sets +0x8 when the rider's velocity (+0x1E0) is longer than
 * 2222.2222 (80 kph in cm/s), and sets +0xC from bit 0 of the flags word (+0x1C) of the object at +0x58 of the
 * intersect info, or 0 when there is none.
 *
 * @param self  the handplant control (this)
 * @param intersectInfo  the intersect info the handplant started from (On Tour tIntersectInfo)
 * @param flag  On Tour's isToeside: the hand goes down on the toe side
 * @param enterSec  On Tour's enterSec: the handplant's entry time
 */
extern "C" void cHandplantControl_init(sState_29B0* self, sOwner_29B0* intersectInfo, int flag, float enterSec)
{
    int on = 1;
    self->t = enterSec;
    self->flag = flag ? on : 0;
    self->fast = Length_29B0(self->rider->vel) > 2222.2222f;
    bool surfaceFlag = false;
    sFlags_29B0* hitObj = intersectInfo->ctl;
    if (hitObj != 0)
        surfaceFlag = (hitObj->flags & 1) ? on : 0;
    self->ctl = surfaceFlag;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_update);
#ifdef SKIP_ASM
extern "C" int cRider_checkForReset_2A30(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" float cRiderMetrix_evHandplantStart(void* self, int a1);
extern "C" float cRiderMetrix_evHandplantEnd(void* self);
extern "C" void cHandplantControl_playHandplantAnim(cHandplantControl* self, int a1, int a2);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* p);
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" bool tEventFlags_isSet1(void* self, int bit);
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

struct sPad_2A30 {
    int flags;
};

// PORT: PS2-only inline asm (float absolute value)
static inline float fabs_2A30(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float clamp_2A30(float x, float lo, float hi)
{
    if (x >= lo) {
        return x <? hi; // PORT: g++ min operator
    }
    return lo;
}

struct sAi_2A30 {
    float t;      // 0x0
    float pos;    // 0x4
    int f8;
    int fC;
    int dir;      // 0x10
    cRider* r;    // 0x14
};

/**
 * Runs one frame of a handplant: the entry, the balanced hold on the wall and the exit to riding or air.
 *
 * Mode 1 (HandplantMode) waits for the entry anim, then holds (mode 2) if bit 13 is down and the plant was not
 * missed, else drops out (mode 3). Holding moves CurBalance toward the balance input (bits 14-19) and tilts
 * mSlewed[8]; on release it picks exit mode 4, 5 or 6 (rail spline). Exits switch to MS_Air and, when the
 * anim ends or reaches event 1, to CS_NaturalAir (anim 0x11F) or CS_Cruise (0x3D).
 *
 * @param in  the handplant command (On Tour tHandplantCmd): button bits and the 6-bit balance value
 */
extern "C" void cHandplantControl_update(cHandplantControl* self, sPad_2A30* in)
{
    if (cRider_checkForReset_2A30(self->mRider, (in->flags >> 12) & 1, 0) != 0) {
        return;
    }
    cRider* rider = self->mRider;
    int st = rider->HandplantMode;
    if (st == 1) {
        if (cRiderAnimBase_isCurrentAnimEndFlag(rider->mRiderAnim, 2) == 0) {
            return;
        }
        if ((in->flags & 0x2000) && self->IsMissedHandplant == 0) {
            self->mRider->HandplantMode = 2;
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evHandplantStart(self->mRider->mMetrix, 1));
        } else {
            self->mRider->HandplantMode = 3;
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evHandplantStart(self->mRider->mMetrix, 2));
        }
        cHandplantControl_playHandplantAnim(self, self->mRider->HandplantMode, self->HandplantType);
        self->EnterSec = 0.0f;
        return;
    }
    if (st == 2) {
        float dt = rider->mTimeScale * 0.01666666753590107f;
        self->EnterSec += dt;
        if ((in->flags & 0x2000) && __builtin_fabsf(rider->mSlewed[8].cur) < 1.0f) {
            float desBal = ((in->flags << 12) >> 26) * 0.032258063554763794f;
            if (rider->IsRidingFakie != 0) desBal = -desBal;
            if (self->HandplantType == 1) desBal = -desBal;
            rider->mSlewed[7].target = desBal;
            rider->mSlewed[7].rate = dt * 5.0f;
            float k = dt * 3.0f;
            float prevBalance = self->CurBalance;
            float np;
            if (desBal + k < prevBalance) {
                np = prevBalance - k;
            } else if (prevBalance < desBal - k) {
                np = prevBalance + k;
            } else {
                np = desBal;
            }
            self->CurBalance = np;
            float desTilt = np;
            if (np < 0.0f) {
                desTilt = -1.0f;
            } else if (np > 0.0f) {
                desTilt = 1.0f;
            } else if (self->EnterSec > 0.5013384222984314f || self->mRider->mSlewed[8].cur != 0.0f) {
                desTilt = 1.0f;
                if (self->mRider->mSlewed[8].cur < 0.0f) desTilt = -1.0f;
            }
            rider = self->mRider;
            float weight = self->EnterSec * 0.19999131560325623f <? 1.0f; // PORT: g++ min operator
            float invWeight = 1.0f - weight;
            float absTilt = fabs_2A30(rider->mSlewed[8].cur);
            rider->mSlewed[8].target = desTilt;
            float cenRate = weight * 4.0f + invWeight;
            float edgeRate = weight * 5.5f + invWeight * 1.5f;
            rider->mSlewed[8].rate = (edgeRate * absTilt + cenRate * (1.0f - absTilt)) * dt;
            return;
        }
        if (self->IsRailSpline != 0 && __builtin_fabsf(self->mRider->mSlewed[8].cur) < 0.25f) {
            self->mRider->HandplantMode = 6;
        } else if (self->mRider->mSlewed[8].cur < -0.05000000074505806f) {
            self->mRider->HandplantMode = 4;
        } else {
            self->mRider->HandplantMode = 5;
        }
        cRider_doLeanPoseAdjust(self->mRider, &self->mRider->mPosition);
        cRider_updateOrientationImplicit(self->mRider);
        cRider* rider2 = self->mRider;
        rider2->mSlewed[0].target = 0.0f;
        rider2->mSlewed[0].cur = 0.0f;
        rider2->mSlewed[0].rate = 0.0f;
        cRider* rider3 = self->mRider;
        rider3->mSlewed[2].target = 0.0f;
        rider3->mSlewed[2].cur = 0.0f;
        rider3->mSlewed[2].rate = 0.0f;
        cRider* rider4 = self->mRider;
        rider4->mSlewed[8].target = 0.0f;
        rider4->mSlewed[8].cur = 0.0f;
        rider4->mSlewed[8].rate = 0.0f;
        cHandplantControl_playHandplantAnim(self, self->mRider->HandplantMode, self->HandplantType);
        return;
    }
    if (cRider_getMotionState(rider) == 5) {
        cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evHandplantEnd(self->mRider->mMetrix));
        cRider_setMotionState_impl(self->mRider, 1);
    }
    if (tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 1)) {
        if (cRider_checkRailBoardCollisions(self->mRider)) {
            return;
        }
        if (cRider_getMotionState(self->mRider) == 0) {
            cRiderAnimBase_play(self->mRider->mRiderAnim, 61, 0, -1.0f);
            cRider_setControlState_impl(self->mRider, 0);
            return;
        }
    } else {
        int motionState = cRider_getMotionState(self->mRider);
        if (motionState == 1) {
            int ok = *(int*)((char*)self->mRider->mAirPredictor + 0xAC) == 1 || *(int*)((char*)self->mRider->mAirPredictor + 0xAC) == 3;
            if (ok) {
                char* airPredictor = (char*)self->mRider->mAirPredictor;
                float airTimeLeft = *(float*)(airPredictor + 0x98) - *(float*)(airPredictor + 0xA0);
                if (airTimeLeft > 0.0f) {
                    char* seq = cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2);
                    float animRate = (*(float*)(seq + 0x10) - *(float*)(seq + 0x8)) / airTimeLeft;
                    *(float*)(seq + 0x90) = clamp_2A30(animRate, 0.5f, 2.0f);
                }
            }
        }
    }
    if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) == 0) {
        return;
    }
    if (cRider_getMotionState(self->mRider) == 1) {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 287, 0, -1.0f);
        cRider_setControlState_impl(self->mRider, 4);
    } else {
        cRiderAnimBase_play(self->mRider->mRiderAnim, 61, 0, -1.0f);
        cRider_setControlState_impl(self->mRider, 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_loseFocus);
#ifdef SKIP_ASM
/**
 * Leaves a handplant: clears the rider's handplant mode (HandplantMode) and releases the balance channel
 * mSlewed[7] (target 0, rate 0.05).
 */
extern "C" void cHandplantControl_loseFocus(cHandplantControl* self)
{
    cRider* rider = self->mRider;
    rider->HandplantMode = 0;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[7].rate = 0.05000000447034836f;
    *(int*)&rider2->mSlewed[7].target = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_playHandplantAnim);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);

struct sAnimEntry_132FB8
{
    int anim;
    int flip;
};
// Typed view of kHandplantAnim (cHandplantControl_gainFocus reads it as raw bytes); bound by asm label.
extern sAnimEntry_132FB8 D_00459FC8_tbl[][2] __asm__("kHandplantAnim");

struct sVec4_132FB8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVec3_132FB8
{
    float x, y, z;
    sVec3_132FB8(sVec4_132FB8 v) { x = v.x; y = v.y; z = v.z; }
};

struct sQuat_132FB8
{
    float x, y, z, w;
    sQuat_132FB8() {}
    sQuat_132FB8(const sVec3_132FB8& v, float aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_132FB8 QuatMul_132FB8(const sQuat_132FB8& a, const sQuat_132FB8& b)
{
    sQuat_132FB8 r;
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

/**
 * Plays the handplant anim for a mode and type from kHandplantAnim, first turning the rider round if asked.
 *
 * When the entry's flip word is set it rotates the rider 180 degrees about its up axis (mMatrix row 2),
 * refreshes the orientation and adds pi to the anim heading offset.
 *
 * @param mode  handplant mode (On Tour eHandplantMode; the rider's HandplantMode)
 * @param type  handplant type (eHandplantType: HT_HS 0, HT_TS 1)
 */
extern "C" void cHandplantControl_playHandplantAnim(cHandplantControl* self, int mode, int type)
{
    if (D_00459FC8_tbl[mode][type].flip != 0)
    {
        cRider* rider = self->mRider;
        sQuat_132FB8 newRotation = QuatMul_132FB8(sQuat_132FB8(sVec3_132FB8(*(sVec4_132FB8*)&rider->mMatrix.row[2]), 0.0f), *(sQuat_132FB8*)&rider->mRotation);
        *(sQuat_132FB8*)&self->mRider->mRotation = newRotation;
        cRider_updateOrientationImplicit(self->mRider);
        cRiderAnimBase_changeHeadingOffset(self->mRider->mRiderAnim, 3.1415927410125732f);
    }
    cRiderAnimBase_play(self->mRider->mRiderAnim, D_00459FC8_tbl[mode][type].anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001330B8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the handplant state to a replay frame.
 *
 * Writes the 0x14 bytes at offset 0 (EnterSec .. HandplantType) through the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cHandplantControl_writeToReplayFrame(cHandplantControl* self, cReplayFramePtr* obj)
{
    sVEntry001330B8* vt = *(sVEntry001330B8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cHandplantControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001330F0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the handplant state from a replay frame.
 *
 * Reads the 0x14 bytes at offset 0 (EnterSec .. HandplantType) through the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cHandplantControl_readFromReplayFrame(cHandplantControl* self, cReplayFramePtr* obj)
{
    sVEntry001330F0* vt = *(sVEntry001330F0**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 20);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float cBEStatInterface_getSpinAttrib(void* self, int rider, int value);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" float cSpinControl_calcFlipAxisWeight(cSpinControl* self);
extern "C" void cSpinControl_initDesSpin(cSpinControl* self, float a, float b);

/**
 * Prepares spin control on takeoff: clears the grab, spin and flip state and starts any spin wound up before.
 *
 * Resets the angles, MaxSpinAngVel/MaxFlipAngVel to about 4.5 rad/s and FlipSpinInputAng to 1e30 (none). If
 * the prewind channels mSlewed[15]/[16] (spin, flip) are nonzero it scales the rates up from the larger one
 * and runs initDesSpin() (AirAdjState 1, SpinState 0 = into); else AirAdjState 0 and SpinState 3 (none).
 */
extern "C" void cSpinControl_gainFocus(cSpinControl* self)
{
    float lo = 4.499661445617676f;
    self->CurrentGrab = -1;
    self->GrabState = 0;
    *(int*)&self->CurFlip = 0;
    *(int*)&self->CurSpin = 0;
    *(int*)&self->DesFlip = 0;
    *(int*)&self->DesSpin = 0;
    *(int*)&self->TrickSpin = 0;
    *(int*)&self->TrickFlip = 0;
    *(int*)&self->AirAdjSpin = 0;
    *(int*)&self->AirAdjFlip = 0;
    *(int*)&self->TotalSpinAng = 0;
    *(int*)&self->TotalFlipAng = 0;
    self->MaxSpinAngVel = lo;
    self->MaxFlipAngVel = lo;
    self->FlipAxisWeight = 0.0f;
    *(float*)&self->IsPartialLateSpin = 0.0f;
    self->mRider->mSpinRate = 0.0f;
    self->mRider->mFlipRate = 0.0f;
    self->DesSpinInputSec = 0.0f;
    self->FlipSpinInputAng = 1.0000000150474662e+30f;
    char* rider = (char*)self->mRider;
    self->DesFlipInputSec = 0.0f;
    self->NullFlipSpinInputSec = 0.0f;
    if (*(float*)(rider + 0x2A4) != 0.0f || *(float*)(rider + 0x2B0) != 0.0f)
    {
        self->SpinState = 0;
        float x = *(float*)(rider + 0x2A4);
        float absSpin = __builtin_fabsf(x);
        float absFlip = __builtin_fabsf(*(float*)(rider + 0x2B0));
        float power;
        if (absFlip < absSpin)
            power = absSpin;
        else
            power = absFlip;
        if (x != 0.0f)
        {
            float angVel = power * 7.00038480758667f >? lo;
            self->MaxSpinAngVel = angVel;
            *(float*)(rider + 0x2DC) = angVel;
        }
        rider = (char*)self->mRider;
        if (*(float*)(rider + 0x2B0) != 0.0f)
        {
            float angVel = (power * 7.00038480758667f >? lo) * 0.6666666865348816f;
            self->MaxFlipAngVel = angVel;
            *(float*)(rider + 0x2E0) = angVel;
        }
        self->FlipAxisWeight = cSpinControl_calcFlipAxisWeight(self);
        cRider* rider2 = self->mRider;
        cSpinControl_initDesSpin(self, rider2->mSlewed[16].cur, rider2->mSlewed[15].cur);
        float spinAttrib = cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->mRider->mAIIndex, self->mRider->mStatsIndex);
        cRider* rider3 = self->mRider;
        float animRate = (spinAttrib * 0.5002591609954834f + 1.0f) * (rider3->mFlipRate >? rider3->mSpinRate) * 0.14284929633140564f;
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(rider3->mRiderAnim, 2) + 0x90) = animRate;
        *(int*)self = 1;
    }
    else
    {
        *(int*)self = 0;
        self->SpinState = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares cRider_checkForReset earlier with a void return; same symbol.
extern "C" int cRider_checkForReset_3308(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" int cRider_checkHandplantCollisions(void* rider, int on);
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" int cSpinControl_isGrabPlaying(cSpinControl* self, int c, int flag);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" int cSpinControl_checkChangeFakieAir(void* self);
extern "C" void cRider_snapAxisIncrement(void* rider, void* a, void* b, float c, float d);
extern "C" void cSpinControl_initDesSpin(cSpinControl* self, float a, float b);
extern "C" float cRiderMetrix_evLateSpin(void* self, int v);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float cBEStatInterface_getSpinAttrib(void* self, int rider, int value);
extern "C" float func_0031C228(float x);
extern "C" float cRiderMetrix_evSpin(void* self, float x);
extern "C" float cRiderMetrix_evFlip(void* self, float x);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);
extern "C" float cRiderAnimBase_getAIAnimDuration(char* self, int a1);
extern float kSpinOutOfAng;
extern float kIntoSpinAbortAng;
// Max spin/flip rates: computed once by the static initialiser (__sti__all_in_one_ai_cpp) and read as
// unchanging memory here (the reciprocal loads float above the channel stores).
extern const float kMaxAdjSpinAng;
extern const float kMaxAdjFlipAng;
extern float kMaxPressFlipAng;
extern float kSpinNoDecayThresh;
extern float kFlipNoDecayThresh;
extern float kAirAdjFwdMaxAng;

struct sSpinCtl_133308
{
    int mode;           // 0x00
    int f04;            // 0x04
    int f08;            // 0x08
    int state;          // 0x0C
    float desFlip;      // 0x10
    float desSpin;      // 0x14
    float curFlip;      // 0x18
    float curSpin;      // 0x1C
    float totSpin;      // 0x20
    float totFlip;      // 0x24
    float spinVel;      // 0x28
    float flipVel;      // 0x2C
    float spinAcc;      // 0x30
    float flipAcc;      // 0x34
    float spinSpeed;    // 0x38
    float flipSpeed;    // 0x3C
    float f40;          // 0x40
    int f44;            // 0x44
    float spinHold;     // 0x48
    float flipHold;     // 0x4C
    float stickAngle;   // 0x50
    float idle;         // 0x54
    cRider* rider;      // 0x58
};

#define SPINRATE_133308(r) (*(float*)&(r)->mSpinRate)
#define FLIPRATE_133308(r) (*(float*)&(r)->mFlipRate)

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_133308(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_133308(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// How far the current angle may trail the target turn before a new turn can be queued.
static inline float aiTurnLimit_133308(float turn)
{
    return turn - kIntoSpinAbortAng;
}

static inline float aiTarget_133308(float in, const float& max, const float& cur)
{
    if (in < 0.0f) return -max;
    if (in <= 0.0f) return cur;
    return max;
}

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_133308(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

static inline float aiApproach_133308(float cur, float target, float step)
{
    if (target + step < cur) return cur - step;
    if (cur < target - step) return cur + step;
    return target;
}

// Fraction of the full turn rate to use: ramps down over the last tenth of the way to the target speed.
static inline float aiTurnScale_133308(float dist, float max)
{
    return aiClamp_133308(dist * 10.0f / max, 0.10000000149011612f, 1.0f);
}

static inline bool aiIsGrinding_133308(cRider* rider)
{
    int st = *(int*)((char*)rider->mAirPredictor + 0xAC);
    return st == 1 || st == 3;
}

// PORT: g++ `>?` (max) operator.
static inline float aiSpeed_133308(float dist, float base)
{
    float t = dist * 0.15915492177009583f;
    float s = t * 4.999992370605469f + (1.0f - t) * 7.500268936157227f;
    return s >? base;
}

static inline float aiSign_133308(float a, float b)
{
    if (a >= b)
    {
        if (a > b) return 1.0f;
        return 0.0f;
    }
    return -1.0f;
}

static inline int aiDir8_133308(float ang, int back, int r3, int r2, int r1, int l3, int l2, int l1, int fwd)
{
    if (aiAbs_133308(ang) > 2.7488937377929688f) return back;
    if (ang > 1.9634956121444702f) return r3;
    if (ang > 1.1780973672866821f) return r2;
    if (ang > 0.39269912242889404f) return r1;
    if (ang < -1.9634956121444702f) return l3;
    if (ang < -1.1780973672866821f) return l2;
    if (ang < -0.39269912242889404f) return l1;
    return fwd;
}

static inline void aiSetFalloff_133308(float& rate, float r, float sp)
{
    float v;
    if (r <= 1.0f)
        v = sp * (r * 0.800000011920929f + (1.0f - 0.800000011920929f));
    else
        v = sp;
    rate = v;
}

static inline void aiSetChannel_133308(cRider* rider, int i, float rate, float target)
{
    rider->mSlewed[i].rate = rate;
    rider->mSlewed[i].target = target;
}

/**
 * Runs one frame of in-air spin and grab control: turns the stick into target spins and flips, steps the
 * rotation toward them, scores it and picks the air anim.
 *
 * Leaves early on a reset (bit 12), a handplant (bit 13 while not spinning) or a rail or board collision.
 * SpinState goes none -> into (new input, snapped to 45 degrees) -> cycle (holding adds half spins and full
 * flips) -> out of; AirAdjState handles air adjust and late spin (bit 15), isGrabPlaying() the grab (byte 2,
 * tweak bit 14), and word 1 bits 18-19 the landing press (PressMode).
 *
 * @param input  the spin command (On Tour tSpinCmd): button bits, grab id and 6-bit spin and flip values
 */
extern "C" void cSpinControl_update(sSpinCtl_133308* self, int* input)
{
    if (cRider_checkForReset_3308(self->rider, (*input >> 12) & 1, 0))
        return;
    if ((*input & 0x2000) && self->state == 3 &&
        cRider_checkHandplantCollisions(self->rider, ((input[1] << 20) >> 26) * 0.032258063554763794f > 0.5f))
        return;
    if (cRider_checkRailBoardCollisions(self->rider))
        return;

    float flip = ((input[1] << 26) >> 26) * 0.032258063554763794f;
    float spin = ((input[0] << 2) >> 26) * 0.032258063554763794f;
    int trick = ((signed char*)input)[2];
    float flipAdj = ((input[1] << 20) >> 26) * 0.032258063554763794f;
    float spinAdj = ((input[1] << 14) >> 26) * 0.032258063554763794f;

    if (self->mode == 0)
    {
        if (*input & 0x8000)
        {
            if (spinAdj != 0.0f || flipAdj != 0.0f)
                self->mode = 2;
        }
    }
    else if (spin != 0.0f || flip != 0.0f)
    {
        self->mode = 0;
    }
    if (self->mode == 2)
    {
        if (*input & 0x8000)
            trick = -1;
        else
            self->mode = 1;
    }
    if (self->mode == 1 || self->mode == 2)
    {
        if (spinAdj == 0.0f && flipAdj == 0.0f && self->idle > 0.30000001192092896f)
            self->mode = 0;
        if (spin == 0.0f)
            spin = spinAdj;
        if (flip == 0.0f)
            flip = flipAdj;
        spinAdj = flipAdj = 0.0f;
    }

    int busy = cSpinControl_isGrabPlaying((cSpinControl*)self, trick, (*input >> 14) & 1);
    if (!busy && self->mode == 3)
        self->mode = 0;

    self->rider->PressMode = 0;
    float maxFlip = kMaxAdjFlipAng;
    int dir = (input[1] << 12) >> 30;
    float flipInput;
    switch (dir)
    {
    case 1:
        flipInput = 1.0f;
        break;
    default:
        flipInput = 0.0f;
        break;
    case -1:
        flipInput = -1.0f;
        break;
    }
    if (flipInput != 0.0f)
    {
        flipAdj = flipInput;
        maxFlip = kMaxPressFlipAng;
        if (self->flipVel < 0.0f || (self->flipVel == 0.0f && flipAdj < 0.0f))
            self->rider->PressMode = 2;
        else
            self->rider->PressMode = 1;
    }
    cRider_setupBoostWarp(self->rider, 0, 0);

    int state0 = self->state;
    if (busy)
    {
        spinAdj = flipAdj = 0.0f;
        if (self->mode == 0)
            self->mode = 3;
    }
    else if (state0 == 3)
    {
        if (cRiderAnimBase_getCurrentAnimID(self->rider->mRiderAnim, 2) != 288 && !cSpinControl_checkChangeFakieAir(self) &&
            (spin != 0.0f || flip != 0.0f))
        {
            cRider_snapAxisIncrement(self->rider, &flip, &spin, 0.7853982448577881f, 0.0f);
            cSpinControl_initDesSpin((cSpinControl*)self, flip, spin);
            self->spinAcc = 0.0f;
            self->flipAcc = 0.0f;
            self->spinSpeed = 4.499661445617676f;
            SPINRATE_133308(self->rider) = 4.499661445617676f;
            self->flipSpeed = 2.99977445602417f;
            FLIPRATE_133308(self->rider) = 2.99977445602417f;
            self->state = 0;
            cRider_flagEventBoostPowerUp(self->rider, 1,
                          cRiderMetrix_evLateSpin(self->rider->mMetrix, self->rider->IsRidingFakie != self->rider->IsGoofyStance));
        }
    }

    if (self->state == 0)
    {
        if (self->desSpin == 0.0f && self->desFlip == 0.0f)
        {
            spin = 0.0f;
            flip = 0.0f;
            if (self->curSpin == 0.0f && self->curFlip == 0.0f)
                self->state = 3;
        }
        else if (aiAbs_133308(self->curSpin - self->desSpin) < aiTurnLimit_133308(3.1415929794311523f) &&
                 aiAbs_133308(self->curFlip - self->desFlip) < aiTurnLimit_133308(6.283185958862305f))
        {
            self->state = 1;
        }
        else if (spin == 0.0f && flip == 0.0f)
        {
            self->desSpin = 0.0f;
            self->desFlip = 0.0f;
        }
    }
    else if (self->state == 1)
    {
        if (spin == 0.0f && flip == 0.0f)
        {
            self->idle += 0.01666666753590107f;
            float ds = self->desSpin;
            if (aiAbs_133308(ds - self->curSpin) > 3.1415929794311523f)
            {
                if (ds < 0.0f)
                    ds += 3.1415929794311523f;
                else
                    ds -= 3.1415929794311523f;
            }
            float df = self->desFlip;
            if (aiAbs_133308(df - self->curFlip) > 6.283185958862305f)
            {
                if (df < 0.0f)
                    df += 6.283185958862305f;
                else
                    df -= 6.283185958862305f;
            }
            float tol = 0.0f;
            if (self->idle > 0.30000001192092896f)
            {
                self->desSpin = ds;
                self->desFlip = df;
                tol = kSpinOutOfAng;
            }
            if (aiAbs_133308(ds - self->curSpin) <= tol && aiAbs_133308(df - self->curFlip) <= tol)
                self->state = 2;
        }
        else
        {
            self->idle = 0.0f;
            if (spin == 0.0f)
                self->spinHold = 0.0f;
            else
                self->spinHold += 0.01666666753590107f;
            if (flip == 0.0f)
                self->flipHold = 0.0f;
            else
                self->flipHold += 0.01666666753590107f;

            {
                float& des = self->desSpin;
                if (aiAbs_133308(self->curSpin - des) <= kSpinOutOfAng)
                {
                    float th = (des == 0.0f) ? 0.20000000298023224f : 0.0f;
                    if (self->spinHold > th)
                    {
                        float n;
                        if (des == 0.0f)
                        {
                            self->f44 = 1;
                            n = (spin < 0.0f) ? -3.1415929794311523f : 3.1415929794311523f;
                        }
                        else
                            n = (des < 0.0f) ? des - 3.1415929794311523f : des + 3.1415929794311523f;
                        des = n;
                        if (aiAbs_133308(self->desSpin) > kSpinNoDecayThresh)
                            self->spinSpeed = (self->spinSpeed * 0.7999517321586609f) >? 2.9995594024658203f;
                    }
                }
            }
            {
                float& des = self->desFlip;
                if (aiAbs_133308(self->curFlip - des) <= kSpinOutOfAng)
                {
                    float th = (des == 0.0f) ? 0.20000000298023224f : 0.0f;
                    if (self->flipHold > th)
                    {
                        float n;
                        if (des == 0.0f)
                        {
                            self->f44 = 1;
                            n = (flip < 0.0f) ? -6.283185958862305f : 6.283185958862305f;
                        }
                        else
                            n = (des < 0.0f) ? des - 6.283185958862305f : des + 6.283185958862305f;
                        des = n;
                        if (aiAbs_133308(self->desFlip) > kFlipNoDecayThresh)
                            self->flipSpeed = (self->flipSpeed * 0.6399227976799011f) >? 2.9995594024658203f;
                    }
                }
            }
            if (busy)
                self->f44 = 1;
        }
    }
    else if (self->state == 2)
    {
        self->mode = 0;
        spin = 0.0f;
        flip = 0.0f;
        if (self->curSpin == self->desSpin && self->curFlip == self->desFlip)
        {
            self->curSpin = 0.0f;
            self->state = 3;
            self->desSpin = 0.0f;
            self->curFlip = 0.0f;
            self->desFlip = 0.0f;
            self->f44 = 0;
        }
        else if (!busy && cRiderAnimBase_getCurrentAnimID(self->rider->mRiderAnim, 2) != 288)
        {
            cSpinControl_checkChangeFakieAir(self);
        }
        else if (self->desFlip != self->curFlip)
        {
            if ((self->curFlip < 0.0f && flipAdj > 0.0f) || (self->curFlip > 0.0f && flipAdj < 0.0f))
                flipAdj = -flipAdj;
        }
    }

    float invSpinAttribScale = 1.0f / (cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->rider->mAIIndex,
                                     self->rider->mStatsIndex) * 0.349495530128479f + 1.0f);
    float spinAttribScale = cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->rider->mAIIndex,
                             self->rider->mStatsIndex) * 0.5002591609954834f + 1.0f;
    if (flip == 0.0f && spin == 0.0f)
        self->stickAngle = 1.0000000150474662e+30f;
    else
        self->stickAngle = aiAtan2_133308(flip, spin);

    if (self->desSpin == self->curSpin)
    {
        SPINRATE_133308(self->rider) = 0.0f;
    }
    else if (spin * self->desSpin < 0.0f)
    {
        SPINRATE_133308(self->rider) = self->spinSpeed <? invSpinAttribScale * 2.000131845474243f;
    }
    else
    {
        if (spin != 0.0f)
        {
            SPINRATE_133308(self->rider) = self->spinSpeed;
        }
        else
        {
            float diff = self->curSpin - self->desSpin;
            float sp = self->spinSpeed;
            if (flip == 0.0f)
                sp = aiSpeed_133308(aiAbs_133308(diff), sp);
            float ratio = aiAbs_133308(diff) / kSpinOutOfAng;
            aiSetFalloff_133308(SPINRATE_133308(self->rider), ratio, sp);
        }
        SPINRATE_133308(self->rider) *= spinAttribScale;
        if (self->rider->mPowerUpTrickTimer > 0.0f)
            SPINRATE_133308(self->rider) *= 1.9997502565383911f;
    }

    if (self->desFlip == self->curFlip)
    {
        FLIPRATE_133308(self->rider) = 0.0f;
    }
    else if (flip * self->desFlip < 0.0f)
    {
        FLIPRATE_133308(self->rider) = self->flipSpeed <? invSpinAttribScale * 2.000131845474243f;
    }
    else
    {
        if (flip != 0.0f)
        {
            FLIPRATE_133308(self->rider) = self->flipSpeed;
        }
        else
        {
            float diff = self->curFlip - self->desFlip;
            float sp = self->flipSpeed;
            if (spin == 0.0f)
                sp = aiSpeed_133308(aiAbs_133308(diff), sp);
            float ratio = aiAbs_133308(diff) / kSpinOutOfAng;
            aiSetFalloff_133308(FLIPRATE_133308(self->rider), ratio, sp);
        }
        FLIPRATE_133308(self->rider) *= spinAttribScale;
        if (self->rider->mPowerUpTrickTimer > 0.0f)
            FLIPRATE_133308(self->rider) *= 1.9997502565383911f;
    }

    if (aiAbs_133308(spinAdj) < 0.20000000298023224f)
        spinAdj = 0.0f;
    if (aiAbs_133308(flipAdj) < 0.20000000298023224f)
        flipAdj = 0.0f;

    float spin0 = self->curSpin;
    float flip0 = self->curFlip;

    {
        float tgt = aiTarget_133308(spinAdj, kMaxAdjSpinAng, self->spinVel);
        float vel = self->spinVel;
        self->spinVel = aiApproach_133308(vel, tgt, (aiAbs_133308(spinAdj) * 0.5f + 0.5f) *
                                                    (aiTurnScale_133308(aiAbs_133308(tgt - vel), kMaxAdjSpinAng) * 0.04999711737036705f));
        float cs = self->curSpin;
        float ds = self->desSpin;
        if (cs < ds)
            self->curSpin = ds <? cs + self->spinVel - vel;
        else if (ds < cs)
            self->curSpin = ds >? cs + self->spinVel - vel;
        float old = self->curSpin;
        float n = aiApproach_133308(old, self->desSpin, SPINRATE_133308(self->rider) * 0.01666666753590107f);
        self->curSpin = n;
        self->totSpin += n - old;
    }
    {
        float tgt = aiTarget_133308(flipAdj, maxFlip, self->flipVel);
        if (tgt > 0.0f)
            tgt = kAirAdjFwdMaxAng <? tgt;
        float vel = self->flipVel;
        self->flipVel = aiApproach_133308(vel, tgt, (aiAbs_133308(flipAdj) * 0.5f + 0.5f) *
                                                    (aiTurnScale_133308(aiAbs_133308(tgt - vel), kMaxAdjFlipAng) * 0.05000609531998634f));
        float cf = self->curFlip;
        float df = self->desFlip;
        if (cf < df)
            self->curFlip = df <? cf + self->flipVel - vel;
        else if (df < cf)
            self->curFlip = df >? cf + self->flipVel - vel;
        float old = self->curFlip;
        float n = aiApproach_133308(old, self->desFlip, FLIPRATE_133308(self->rider) * 0.01666666753590107f);
        self->curFlip = n;
        self->totFlip += n - old;
    }
    self->spinAcc += self->curSpin - spin0;
    self->flipAcc += self->curFlip - flip0;

    float stanceSpin = self->spinAcc;
    if (self->rider->IsGoofyStance)
        stanceSpin = -stanceSpin;
    cRider_flagEventBoostPowerUp(self->rider, 1, cRiderMetrix_evSpin(self->rider->mMetrix, stanceSpin));
    cRider_flagEventBoostPowerUp(self->rider, 1, cRiderMetrix_evFlip(self->rider->mMetrix, self->flipAcc));

    int cur = cRiderAnimBase_getCurrentAnimID(self->rider->mRiderAnim, 2);
    int anim = cur;
    aiSetChannel_133308(self->rider, 14, 0.1666666716337204f, self->spinVel * (1.0f / kMaxAdjSpinAng));
    aiSetChannel_133308(self->rider, 13, 0.1666666716337204f, self->flipVel * (1.0f / kMaxAdjFlipAng));

    if (!busy && cur != 288)
    {
        if (self->spinVel != 0.0f || self->flipVel != 0.0f)
        {
            if (cRiderAnimBase_getCurrentAnimCategory(self->rider->mRiderAnim, 2) != 9)
            {
                cRider* rd = self->rider;
                float x = rd->mSlewed[14].cur;
                if (rd->IsRidingFakie)
                    x = -x;
                float ang = aiAtan2_133308(rd->mSlewed[13].cur, x);
                anim = aiDir8_133308(ang, 297, 298, 299, 300, 304, 303, 302, 301);
            }
        }
        else if (state0 == 3 && (self->state == 0 || self->state == 1))
        {
            float ang;
            if (self->rider->IsRidingFakie)
                ang = aiAtan2_133308(flip, -spin);
            else
                ang = aiAtan2_133308(flip, spin);
            anim = aiDir8_133308(ang, 270, 274, 272, 273, 276, 271, 275, 269);
            float spinAttrib = cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->rider->mAIIndex,
                                    self->rider->mStatsIndex);
            cRider* rd = self->rider;
            float t = (spinAttrib * 0.5002591609954834f + 1.0f) * (FLIPRATE_133308(rd) >? SPINRATE_133308(rd)) *
                      0.14284929633140564f;
            *(float*)((char*)rd->mRiderAnim + 0x1C) = t;
        }
        else if (self->state == 0 && self->desSpin == 0.0f && self->desFlip == 0.0f)
        {
            anim = 287;
        }
        else if (self->state == 1)
        {
            if (self->f44 && cRiderAnimBase_getCurrentAnimCategory(self->rider->mRiderAnim, 2) == 2 &&
                !(self->curSpin == self->desSpin && self->curFlip == self->desFlip) &&
                (cRiderAnimBase_isCurrentAnimEndFlag(self->rider->mRiderAnim, 2) || anim == 287))
            {
                float y = aiSign_133308(self->desFlip, self->curFlip);
                float x = aiSign_133308(self->desSpin, self->curSpin);
                float ang;
                if (self->rider->IsRidingFakie)
                    ang = aiAtan2_133308(y, -x);
                else
                    ang = aiAtan2_133308(y, x);
                anim = aiDir8_133308(ang, 294, 294, 293, 293, 296, 296, 295, 295);
            }
        }
        else if (state0 != 2 && self->state == 2)
        {
            switch (anim)
            {
            case 269:
            case 278:
            case 283:
            case 289:
                anim = 306;
                break;
            case 270:
            case 279:
            case 284:
            case 290:
                anim = 307;
                break;
            case 271:
            case 285:
            case 291:
                anim = 308;
                break;
            case 272:
            case 280:
            case 281:
            case 286:
            case 292:
                anim = 309;
                break;
            case 273:
            case 293:
                anim = 310;
                break;
            case 274:
            case 294:
                anim = 311;
                break;
            case 275:
            case 295:
                anim = 312;
                break;
            case 276:
            case 296:
                anim = 313;
                break;
            default:
                anim = 287;
                break;
            }
            if (anim == cur)
                return;
            float len = cRiderAnimBase_getAIAnimDuration((char*)self->rider->mRiderAnim, anim);
            float dS = aiAbs_133308(self->curSpin - self->desSpin);
            float dF = aiAbs_133308(self->curFlip - self->desFlip);
            float t = dS / aiSpeed_133308(dS, self->spinSpeed);
            float tF = dF / aiSpeed_133308(dF, self->flipSpeed);
            if (t < tF)
                t = tF;
            if (self->rider->mPowerUpTrickTimer > 0.0f)
                t *= 0.5000624656677246f;
            t *= 1.0f / (cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->rider->mAIIndex,
                                      self->rider->mStatsIndex) * 0.5002591609954834f + 1.0f);
            float rate = 2.0f;
            if (t > 0.0010000000474974513f)
                rate = len / t;
            *(float*)((char*)self->rider->mRiderAnim + 0x1C) = rate <? 2.0f;
        }
        else
        {
            if (aiIsGrinding_133308(self->rider))
            {
                float dur = *(float*)((char*)self->rider->mAirPredictor + 0x98) - *(float*)((char*)self->rider->mAirPredictor + 0xA0);
                float len = cRiderAnimBase_getAIAnimDuration((char*)self->rider->mRiderAnim, 305);
                if (anim == 287 && len >= dur)
                {
                    float ratio = 1.0f;
                    if (dur > 0.0010000000474974513f)
                        ratio = len / dur;
                    anim = 305;
                    *(float*)((char*)self->rider->mRiderAnim + 0x1C) = ratio <? 2.0f;
                }
                else if (anim == 305 && len < dur)
                {
                    anim = 287;
                }
            }
        }
    }
    if (anim != cur)
    {
        cRiderAnimBase_play(self->rider->mRiderAnim, anim, 0, -1.0f);
        *(float*)((char*)self->rider->mRiderAnim + 0x1C) = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_loseFocus);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_blendOutAll(void*, int, float);
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cSpinControl_applyFlipSpinAngle(void* self, void* pos);

/**
 * Ends spin control: bakes the spin and flip into the rider's pose and leaves a bucketed spin rate behind.
 *
 * Fades out anim layer 1, clears the prewind channels mSlewed[15]/[16], relaxes [13]/[14], applies the
 * rotation to mPosition/mRotation (cSpinControl_applyFlipSpinAngle), then sets mSpinRate to 3/4 of itself bucketed to pi/2,
 * 3pi/2 or 5pi/2 (0 if not positive), negated when CurSpin > DesSpin, and clears mFlipRate.
 */
extern "C" void cSpinControl_loseFocus(cSpinControl* self)
{
    cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, 0.33000001311302185f);
    cRider* rider = self->mRider;
    *(int*)&rider->mSlewed[15].target = 0;
    *(int*)&rider->mSlewed[15] = 0;
    *(int*)&rider->mSlewed[15].rate = 0;
    cRider* rider2 = self->mRider;
    *(int*)&rider2->mSlewed[16].target = 0;
    *(int*)&rider2->mSlewed[16] = 0;
    *(int*)&rider2->mSlewed[16].rate = 0;
    cRider* rider3 = self->mRider;
    rider3->mSlewed[13].rate = 0.01666666753590107f;
    *(int*)&rider3->mSlewed[13].target = 0;
    cRider* rider4 = self->mRider;
    rider4->mSlewed[14].rate = 0.01666666753590107f;
    *(int*)&rider4->mSlewed[14].target = 0;
    cSpinControl_applyFlipSpinAngle(self, &self->mRider->mPosition);
    cRider_updateOrientationImplicit(self->mRider);
    self->mRider->mSpinRate *= 0.7500380277633667f;
    cRider* rider5 = self->mRider;
    float spinRate = rider5->mSpinRate;
    if (spinRate > 4.7123894691467285f)
        rider5->mSpinRate = 7.853982448577881f;
    else if (spinRate > 3.1415929794311523f)
        rider5->mSpinRate = 4.7123894691467285f;
    else if (spinRate > 0.0f)
        rider5->mSpinRate = 1.5707964897155762f;
    else
        rider5->mSpinRate = 0.0f;
    *(int*)&self->mRider->mFlipRate = 0;
    if (self->CurSpin > self->DesSpin)
    {
        self->mRider->mSpinRate = -self->mRider->mSpinRate;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_applyFlipSpinAngle);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 16-byte vector with inline constructors (a non-POD sibling of t4Vector); stores x, y, z, w.
struct sVec4 : t4Vector
{
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// A bone transform read through sVec4 (cAnimModel::bones entries, 0x20 bytes): position, rotation quaternion.
struct sXf
{
    sVec4 pos;
    sVec4 rot;
};
extern t4Vector D_004FF130_4DD0 __asm__("t4vect_Origin");
extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;
extern t4Vector D_004FF160_4DD0 __asm__("t4vect_ZAxis");
// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4 QuatMul_12E010_4DD0(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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
// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4 Sub_12E010_4DD0(const sVec4& a, const t4Vector& b)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}
// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sVec4 Rot_12E010_4DD0(const sVec4& q, const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}
// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4 Scale_12E010_4DD0(const sVec4& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
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
extern "C" float cSpinControl_calcFlipAxisWeight(cSpinControl* self);
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (vector times scalar, constant vector through a register).
static inline sVec4 ScaleO_134DD0(const t4Vector& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b, constant b through a register).
static inline sVec4 SubO_134DD0(const sVec4& a, const t4Vector& b)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dst = a - b).
static inline void SubTo_134DD0(sVec4& dst, const sVec4& a, const sVec4& b)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    dst = r;
}

// PORT: PS2-only VU0 inline asm (dst = normalize via rsqrt).
static inline void NormTo_134DD0(sVec4& dst, const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    dst = r;
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_134DD0(sVec4& dst, const sVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (dst += b, label before a call).
static inline void AddToL_134DD0(sVec4& dst, const sVec4& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4 Norm_134DD0(const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

static inline sVec4 AxisAngleB_134DD0(const sVec4& axis, float angle) return r
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    r.w = c;
    r.x = s * axis.x;
    r.y = s * axis.y;
    r.z = s * axis.z;
}

static inline void SetAA_134DD0(sVec4& q, const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    q.x = s * axis.x;
    q.y = s * axis.y;
    q.z = s * axis.z;
    q.w = c;
}

static inline sVec4 AxisAngle2_134DD0(const t4Vector& axis, float angle)
{
    sVec4 q;
    SetAA_134DD0(q, axis, angle);
    return q;
}

// rot *= q (quaternion product). Declared to return the quaternion by value but has no return statement (the
// classic forgotten return); the callers ignore the result, and that ignored by-value result is the unwritten
// 16-byte temp the retail frame keeps during both rotations.
static inline sVec4 operator*=(sVec4& a, const sVec4& b)
{
    a = QuatMul_12E010_4DD0(a, b);
}

extern "C" void cSpinControl_applyFlipSpinAngle_x(void* self, sXf* xf) __asm__("cSpinControl_applyFlipSpinAngle");
/**
 * Rotates the rider's root pose by the current spin and flip angles, pivoting about a body bone.
 *
 * The pivot is the rider's bone offset (anim model bone table +0x24, indexed by rider +0x89C) scaled by the model's
 * scale (+0x140). The pose is moved to the pivot, turned by -(+0x20 + +0x28) about t4vect_ZAxis (spin), then by +0x24 +
 * +0x2C about an axis between t4vect_YAxis and t4vect_XAxis (flip), and moved back. The axis weight (+0x40) follows
 * cSpinControl_calcFlipAxisWeight, moving by at most |difference| / 30 per call.
 *
 * The C definition is cSpinControl_applyFlipSpinAngle with an asm label; the local operator*= declares a by-value result it never
 * returns (matching quirk, noted in the body).
 *
 * @param xf  the root pose to rotate (On Tour tPose &root): position, then rotation quaternion
 */
extern "C" void cSpinControl_applyFlipSpinAngle_x(void* self, sXf* xf)
{
    char* rd = *(char**)((char*)self + 0x58);
    char* trk = *(char**)(rd + 0x780);
    sVec4* n = (sVec4*)((*(int*)(rd + 0x89C) << 4) + *(int*)(trk + 0x24));
    float spinAngle = -(*(float*)((char*)self + 0x20) + *(float*)((char*)self + 0x28));
    float flipAngle = *(float*)((char*)self + 0x24) + *(float*)((char*)self + 0x2C);
    sVec4 pivot = SubO_134DD0(sVec4(n->x * *(float*)(trk + 0x140), n->y * *(float*)(trk + 0x144),
                                              n->z * *(float*)(trk + 0x148), n->w * *(float*)(trk + 0x14C)),
                                 D_004FF130_4DD0);
    {
        sVec4 rotPivot = Rot_12E010_4DD0(xf->rot, pivot);
        AddToL_134DD0(xf->pos, rotPivot);
        xf->rot *= AxisAngle2_134DD0(D_004FF160_4DD0, spinAngle);
    }
    float cur = cSpinControl_calcFlipAxisWeight((cSpinControl*)self);
    float old = *(float*)((char*)self + 0x40);
    float delta = __builtin_fabsf(old - cur) * 0.03333333507180214f;
    float nv;
    if (cur + delta < old)
        nv = old - delta;
    else if (old < cur - delta)
        nv = old + delta;
    else
        nv = cur;
    *(float*)((char*)self + 0x40) = nv;
    sVec4 ax = Norm_134DD0(Sub_12E010_4DD0(ScaleO_134DD0(t4vect_YAxis, nv),
                                             ScaleO_134DD0(t4vect_XAxis, 1.0f - *(float*)((char*)self + 0x40))));
    {
        sVec4 flipQuat;
        flipQuat = AxisAngleB_134DD0(ax, flipAngle);
        xf->rot *= flipQuat;
    }
    sVec4 w = Scale_12E010_4DD0(pivot, -1.0f);
    sVec4 backOffset = Rot_12E010_4DD0(xf->rot, w);
    AddTo_134DD0(xf->pos, backOffset);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_calcFlipAxisWeight);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

static inline float aiAtan2_135180(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135180(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float aiSqrt_135180(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_135180(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

/**
 * Computes the flip-axis weight (FlipAxisWeight, which tilts the flip axis in cSpinControl_applyFlipSpinAngle) from the rates.
 *
 * The weight grows with the combined rate sqrt(mSpinRate^2 + mFlipRate^2) / 7 beyond 0.3 (full at 1.0) and
 * peaks when the two rates are equal, falling to 0 for a pure spin or a pure flip.
 *
 * @return the weight, 0 to 1
 */
extern "C" float cSpinControl_calcFlipAxisWeight(cSpinControl* self)
{
    cRider* rider = self->mRider;
    float x = rider->mSpinRate;
    float y = rider->mFlipRate;
    float ax = aiAbs_135180(x);
    float ay = aiAbs_135180(y);
    float mag = aiSqrt_135180(x * x + y * y) * 0.14284929633140564f;
    float ang = aiAtan2_135180(ay, ax);
    float magScale = aiClamp_135180(mag - 0.30000001192092896f, 0.0f, 0.699999988079071f);
    return magScale * 1.4285714626312256f * (1.0f - aiAbs_135180(ang - 0.7853982448577881f) / 0.7853982448577881f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_isGrabPlaying);
#ifdef SKIP_ASM
extern "C" float cRider_getGrabAnimSpeedScale(void* rider);
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" unsigned short cBEBAGTInterface_getGrabCycleAnim(void* self, int a1, int a2);
extern "C" unsigned short cBEBAGTInterface_getGrabName(void* self, int a1, int a2);
extern "C" unsigned short cBEBAGTInterface_getTweakAnim(void* self, int a1, int a2);
extern "C" unsigned short cBEBAGTInterface_getTweakCycleAnim(void* self, int a1, int a2);
extern "C" unsigned short cBEBAGTInterface_getTweakName(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getUberAnim(void* self, int rider, int idx, int second);
extern "C" int cBEBAGTInterface_getUberCycleAnim(void* self, int rider, int idx, int second);
extern "C" int cBEBAGTInterface_getUberName(void* self, int rider, int idx, int second);
extern "C" float cRiderMetrix_evGrabHold(void* metrix, int id);
extern "C" float cRiderMetrix_evReleaseGrab(void* metrix, int id);
extern "C" void cSSXAudio_Tweak(void* self, void* obj);


/**
 * Advances the grab state machine (GrabState) for this frame and tells whether a grab anim is on layer 2.
 *
 * Anims and trick ids come from BE interface 6 (cBEBAGTInterface_getGrabAnim .. cBEBAGTInterface_getUberName). A new @p trick starts its
 * grab; anim events 0 and 1 reach the held pose (scored by cRiderMetrix_evGrabHold); @p held plays the tweak, or the
 * uber variant while the uber timer runs; letting go ends the trick (cRiderMetrix_evReleaseGrab) and returns to GS_Start.
 *
 * @param vself  the spin control
 * @param trick  the grab held this frame (-1 for none; On Tour grabHeld)
 * @param held  nonzero while the tweak button is held (On Tour isTweaked)
 * @return 1 while layer 2 plays a grab, tweak or uber anim (anim type 0x12-0x14), otherwise 0
 */
extern "C" int cSpinControl_isGrabPlaying(cSpinControl* vself, int trick, int held)
{
    cSpinControl* self = vself;
    float speed = cRider_getGrabAnimSpeedScale(self->mRider);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    int hi = self->mRider->mUberLevel >= 5;
    int anim;

    if (self->GrabState == 0) {
        if (trick == -1) {
            if (self->CurrentGrab != trick) {
                *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = -1.0f;
                cRiderAnimBase_play(self->mRider->mRiderAnim, 287, 0, -1.0f);
                self->CurrentGrab = trick;
            }
        } else if (self->CurrentGrab != trick) {
            self->CurrentGrab = trick;
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 18) {
                cRider* rider = self->mRider;
                cRiderAnimBase_play(rider->mRiderAnim, cBEBAGTInterface_getGrabAnim(iface, rider->mAIIndex, self->CurrentGrab), 1, -1.0f);
            } else {
                cRider* rider = self->mRider;
                cRiderAnimBase_play(rider->mRiderAnim, cBEBAGTInterface_getGrabAnim(iface, rider->mAIIndex, self->CurrentGrab), 0, -1.0f);
            }
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = speed;
        } else if (tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 0)) {
            self->GrabState = 1;
        }
    }
    if (self->GrabState == 1) {
        if (tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 1)) {
            cRider* rider = self->mRider;
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evGrabHold(rider->mMetrix, cBEBAGTInterface_getGrabName(iface, rider->mAIIndex, self->CurrentGrab)));
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 0.0f;
            if (cBEBAGTInterface_getGrabCycleAnim(iface, self->mRider->mAIIndex, self->CurrentGrab) != 438) {
                cRider* rider2 = self->mRider;
                cRiderAnimBase_play_i(rider2->mRiderAnim, cBEBAGTInterface_getGrabCycleAnim(iface, rider2->mAIIndex, self->CurrentGrab), 0, -1.0f);
                *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 1) + 0x90) = speed;
            }
            self->GrabState = 2;
        }
    }
    if (self->GrabState == 2) {
        if (trick != self->CurrentGrab) {
            cRider* rider = self->mRider;
            cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReleaseGrab(rider->mMetrix, cBEBAGTInterface_getGrabName(iface, rider->mAIIndex, self->CurrentGrab)));
            cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, 0.33000001311302185f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = speed;
            self->GrabState = 5;
        } else if (held != 0) {
            cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, 0.10000000149011612f);
            cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 2, 0.10000000149011612f);
            if (self->mRider->mUberTimer == 0.0f || (self->mRider->mUberFlags & 1) == 0
                || cBEBAGTInterface_getUberAnim(iface, self->mRider->mAIIndex, self->CurrentGrab, hi) == 438) {
                cRider* rider = self->mRider;
                cRiderAnimBase_play(rider->mRiderAnim, cBEBAGTInterface_getTweakAnim(iface, rider->mAIIndex, self->CurrentGrab), 0, -1.0f);
            } else {
                cRider* rider = self->mRider;
                cRiderAnimBase_play(rider->mRiderAnim, cBEBAGTInterface_getUberAnim(iface, rider->mAIIndex, self->CurrentGrab, hi), 0, -1.0f);
            }
            cSSXAudio_Tweak(cAudioInterface_bxAudio(), self->mRider);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = speed;
            self->GrabState = 3;
        }
    }
    if (self->GrabState == 3) {
        if (trick == self->CurrentGrab && held != 0
            && tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 1)) {
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 0.0f;
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 19) {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evGrabHold(rider->mMetrix, cBEBAGTInterface_getTweakName(iface, rider->mAIIndex, self->CurrentGrab)));
                if (cBEBAGTInterface_getTweakCycleAnim(iface, self->mRider->mAIIndex, self->CurrentGrab) != 438) {
                    cRider* rider2 = self->mRider;
                    cRiderAnimBase_play_i(rider2->mRiderAnim, cBEBAGTInterface_getTweakCycleAnim(iface, rider2->mAIIndex, self->CurrentGrab), 0, -1.0f);
                    *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 1) + 0x90) = speed;
                }
            } else {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evGrabHold(rider->mMetrix, cBEBAGTInterface_getUberName(iface, rider->mAIIndex, self->CurrentGrab, hi)));
                if (cBEBAGTInterface_getUberCycleAnim(iface, self->mRider->mAIIndex, self->CurrentGrab, hi) != 438) {
                    cRider* rider2 = self->mRider;
                    cRiderAnimBase_play_i(rider2->mRiderAnim, cBEBAGTInterface_getUberCycleAnim(iface, rider2->mAIIndex, self->CurrentGrab, hi), 0, -1.0f);
                    *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 1) + 0x90) = speed;
                }
            }
            self->GrabState = 4;
        } else if (tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 1)
                   || tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 3)
                   || tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 2)) {
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 19) {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evGrabHold(rider->mMetrix, cBEBAGTInterface_getTweakName(iface, rider->mAIIndex, self->CurrentGrab)));
                cRider* rider2 = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReleaseGrab(rider2->mMetrix, cBEBAGTInterface_getTweakName(iface, rider2->mAIIndex, self->CurrentGrab)));
            } else {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evGrabHold(rider->mMetrix, cBEBAGTInterface_getUberName(iface, rider->mAIIndex, self->CurrentGrab, hi)));
                cRider* rider2 = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReleaseGrab(rider2->mMetrix, cBEBAGTInterface_getUberName(iface, rider2->mAIIndex, self->CurrentGrab, hi)));
            }
            self->GrabState = 5;
        }
    }
    if (self->GrabState == 4) {
        if (trick != self->CurrentGrab || held == 0) {
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 19) {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReleaseGrab(rider->mMetrix, cBEBAGTInterface_getTweakName(iface, rider->mAIIndex, self->CurrentGrab)));
            } else {
                cRider* rider = self->mRider;
                cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evReleaseGrab(rider->mMetrix, cBEBAGTInterface_getUberName(iface, rider->mAIIndex, self->CurrentGrab, hi)));
            }
            cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, 0.33000001311302185f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = speed;
            self->GrabState = 5;
        }
    }
    anim = cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2);
    if (self->GrabState == 5) {
        if (anim == 2 || tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 3)) {
            if (anim == 20 && held != 0 && trick != -1
                && cBEBAGTInterface_getUberAnim(iface, self->mRider->mAIIndex, trick, hi) != 438) {
                self->CurrentGrab = trick;
                cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, 0.33000001311302185f);
                cRider* rider = self->mRider;
                cRiderAnimBase_play_i(rider->mRiderAnim, cBEBAGTInterface_getUberAnim(iface, rider->mAIIndex, self->CurrentGrab, hi), 1, -1.0f);
                *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = speed;
                self->GrabState = 3;
            } else {
                self->GrabState = 0;
                self->CurrentGrab = -1;
            }
        }
    }
    return anim >= 18 && anim <= 20;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_initDesSpin);
#ifdef SKIP_ASM
/**
 * Sets the target spin and flip from the input signs and folds the pending air-adjust angles into the trick.
 *
 * DesSpin becomes +-pi (half a turn) by the sign of @p spin and DesFlip +-2pi by @p flip (0 for no input);
 * AirAdjSpin/AirAdjFlip are added to CurSpin/CurFlip and TrickSpin/TrickFlip, then cleared.
 *
 * @param flip  flip input (only its sign is used)
 * @param spin  spin input (only its sign is used)
 */
extern "C" void cSpinControl_initDesSpin(cSpinControl* self, float flip, float spin)
{
    if (spin > 0.0f)
        self->DesSpin = 3.1415929794311523f;
    else if (spin < 0.0f)
        self->DesSpin = -3.1415929794311523f;
    else
        self->DesSpin = 0.0f;
    if (flip > 0.0f)
        self->DesFlip = 6.283185958862305f;
    else if (flip < 0.0f)
        self->DesFlip = -6.283185958862305f;
    else
        self->DesFlip = 0.0f;
    self->CurSpin += self->AirAdjSpin;
    self->CurFlip += self->AirAdjFlip;
    self->TrickSpin += self->AirAdjSpin;
    self->TrickFlip += self->AirAdjFlip;
    self->AirAdjSpin = 0.0f;
    self->AirAdjFlip = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_checkChangeFakieAir);
#ifdef SKIP_ASM
extern "C" int cRider_isChangeFakieAir(void* rider);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float aiFloor_135BE0(float x)
{
    float t;
    __asm__("cvt.w.s %0, %1\n\tcvt.s.w %0, %0" : "=f"(t) : "f"(x));
    if (x < t)
    {
        t -= 1.0f;
    }
    return t;
}

static inline float aiWrap_135BE0(float x)
{
    return x - aiFloor_135BE0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

/**
 * Switches the spin state to fakie when the rider changes stance in the air.
 *
 * If cRider_isChangeFakieAir (On Tour cRider::isChangeFakieAir) says the rider (+0x58) switched, the angle at +0x20 gets half a
 * turn added (wrapped to -pi..pi), the values at +0x24, +0x2C, +0x18 and +0x10 are negated, and the rider's animation
 * base plays AI anim 0x120 with blend -1.
 *
 * @return 1 when the rider changed to fakie, else 0
 */
extern "C" int cSpinControl_checkChangeFakieAir(void* self)
{
    char* spinCtl = (char*)self;
    if (cRider_isChangeFakieAir(*(void**)(spinCtl + 0x58)) != 0)
    {
    *(float*)(spinCtl + 0x20) = aiWrap_135BE0(*(float*)(spinCtl + 0x20) + 3.1415927410125732f);
    *(float*)(spinCtl + 0x24) = -*(float*)(spinCtl + 0x24);
    *(float*)(spinCtl + 0x2C) = -*(float*)(spinCtl + 0x2C);
    *(float*)(spinCtl + 0x18) = -*(float*)(spinCtl + 0x18);
    *(float*)(spinCtl + 0x10) = -*(float*)(spinCtl + 0x10);
    cRiderAnimBase_play(*(void**)(*(char**)(spinCtl + 0x58) + 0x784), 288, 0, -1.0f);
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_predictSpinSec);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float cBEStatInterface_getSpinAttrib(void* self, int rider, int value);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135CB0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Predicts the time left in the current spin and the time one more half turn would take.
 *
 * Divides |CurSpin - DesSpin| (less pi when above pi) and pi by MaxSpinAngVel; both are halved while
 * mPowerUpTrickTimer runs and divided by 1 + 0.5 * a rider stat (cBEStatInterface_getSpinAttrib, BE interface 3).
 *
 * @param cur  receives the seconds left in the current spin
 * @param next  receives the seconds a further half turn would take
 */
extern "C" void cSpinControl_predictSpinSec(cSpinControl* self, float* cur, float* next)
{
    float pi = 3.1415929794311523f;
    float delSpin = aiAbs_135CB0(self->CurSpin - self->DesSpin);
    if (delSpin > pi) delSpin -= pi;
    *cur = delSpin / self->MaxSpinAngVel;
    *next = pi / self->MaxSpinAngVel;
    if (self->mRider->mPowerUpTrickTimer > 0.0f)
    {
        *next *= 0.5000624656677246f;
        *cur *= 0.5000624656677246f;
    }
    float airFactor = cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->mRider->mAIIndex, self->mRider->mStatsIndex);
    float invAirFactor = 1.0f / (airFactor * 0.5002591609954834f + 1.0f);
    *next *= invAirFactor;
    *cur *= invAirFactor;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_predictFlipSec);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float cBEStatInterface_getSpinAttrib(void* self, int rider, int value);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135DB0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Predicts the time left in the current flip and the time one more full flip would take.
 *
 * Divides |CurFlip - DesFlip| (less 2pi when above 2pi) and 2pi by MaxFlipAngVel; both are halved while
 * mPowerUpTrickTimer runs and divided by 1 + 0.5 * a rider stat (cBEStatInterface_getSpinAttrib, BE interface 3).
 *
 * @param cur  receives the seconds left in the current flip
 * @param next  receives the seconds a further full flip would take
 */
extern "C" void cSpinControl_predictFlipSec(cSpinControl* self, float* cur, float* next)
{
    float twopi = 6.283185958862305f;
    float delFlip = aiAbs_135DB0(self->CurFlip - self->DesFlip);
    if (delFlip > twopi) delFlip -= twopi;
    *cur = delFlip / self->MaxFlipAngVel;
    *next = twopi / self->MaxFlipAngVel;
    if (self->mRider->mPowerUpTrickTimer > 0.0f)
    {
        *next *= 0.5000624656677246f;
        *cur *= 0.5000624656677246f;
    }
    float airFactor = cBEStatInterface_getSpinAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->mRider->mAIIndex, self->mRider->mStatsIndex);
    float invAirFactor = 1.0f / (airFactor * 0.5002591609954834f + 1.0f);
    *next *= invAirFactor;
    *cur *= invAirFactor;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_isGrabHeld);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);

/**
 * Tells whether the rider is holding a grab: GrabState 2 with a grab anim (type 0x12) on layer 2.
 *
 * @return 1 while the grab is held, otherwise 0
 */
extern "C" int cSpinControl_isGrabHeld(cSpinControl* self)
{
    int held = 0;
    if (self->GrabState == 2)
    {
        held = cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 18;
    }
    return held;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_isTweakHeld);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);

/**
 * Tells whether the rider is holding a tweak: GrabState 4 with a tweak anim (type 0x13) on layer 2.
 *
 * @return 1 while the tweak is held, otherwise 0
 */
extern "C" int cSpinControl_isTweakHeld(cSpinControl* self)
{
    int held = 0;
    if (self->GrabState == 4)
    {
        held = cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 19;
    }
    return held;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_isUberHeld);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);

/**
 * Tells whether the rider is holding an uber trick: GrabState 4 with an uber anim (type 0x14) on layer 2.
 *
 * @return 1 while the uber is held, otherwise 0
 */
extern "C" int cSpinControl_isUberHeld(cSpinControl* self)
{
    int held = 0;
    if (self->GrabState == 4)
    {
        held = cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 20;
    }
    return held;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_getFlipSpinAng);
#ifdef SKIP_ASM
static inline float aiSign_135F70(float v)
{
    if (v >= 0.0f)
    {
        if (v > 0.0f) return 1.0f;
        return 0.0f;
    }
    return -1.0f;
}

/**
 * Returns the direction of the current rotation as an angle from the signs of CurFlip and CurSpin.
 *
 * @param out  set to 1 when both are 0 (no rotation), else 0 (On Tour isNull)
 * @return atan2(sign(CurFlip), sign(CurSpin)), one of eight 45-degree directions, or 0 with no rotation
 */
extern "C" float cSpinControl_getFlipSpinAng(cSpinControl* self, int* out)
{
    float y = aiSign_135F70(self->CurFlip);
    float x = aiSign_135F70(self->CurSpin);
    int isNull = 0;
    if (y == 0.0f && x == 0.0f) isNull = 1;
    *out = isNull;
    if (isNull) return 0.0f;
    return aiAtan2_135180(y, x);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_getFlipSpinInputAng);
#ifdef SKIP_ASM
/**
 * Returns the angle of the spin/flip stick input kept in FlipSpinInputAng.
 *
 * @param out  set to 1 when there is no input (the 1e30 marker), else 0 (On Tour isNull)
 * @return FlipSpinInputAng, or 0 with no input
 */
extern "C" float cSpinControl_getFlipSpinInputAng(cSpinControl* self, int* out)
{
    int isNull = self->FlipSpinInputAng == 1.0000000150474662e+30f;
    *out = isNull;
    if (isNull)
        return 0.0f;
    return self->FlipSpinInputAng;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_getInvertScale);
#ifdef SKIP_ASM
/**
 * Tells how upside down the flip has the rider: 0 within 90 degrees of upright, rising to 1 at 180.
 *
 * Wraps CurFlip to +-pi and returns max(|flip| / pi - 0.5, 0) * 2.
 *
 * @return the invert scale, 0 to 1
 */
// PORT: g++ `>?` (max) operator. aiWrap_135BE0 uses the FPU-only cvt.w.s/cvt.s.w asm helper.
extern "C" float cSpinControl_getInvertScale(cSpinControl* self)
{
    float scale = __builtin_fabsf(aiWrap_135BE0(self->CurFlip)) * 0.31830987334251404f - 0.5f;
    scale = scale >? 0.0f;
    return scale * 2.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_getOffAxisScale);
#ifdef SKIP_ASM
/**
 * Tells how sideways the spin has the rider: 0 within 22.5 degrees of forward or backward, 1 at 90.
 *
 * Wraps CurSpin to +-pi, folds it to quarter turns (|spin| * 2 / pi, mirrored above 1) and returns
 * max(q - 0.25, 0) * 4 / 3.
 *
 * @return the off-axis scale, 0 to 1
 */
// PORT: g++ `>?` (max) operator. aiWrap_135BE0 uses the FPU-only cvt.w.s/cvt.s.w asm helper.
extern "C" float cSpinControl_getOffAxisScale(cSpinControl* self)
{
    float scale = __builtin_fabsf(aiWrap_135BE0(self->CurSpin)) * 0.6366197466850281f;
    if (scale > 1.0f)
        scale = 2.0f - scale;
    scale = scale - 0.25f;
    scale = scale >? 0.0f;
    return scale * 1.3333333730697632f;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001361F8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the spin control state to a replay frame.
 *
 * Writes the 0x58 bytes at offset 0 (all of cSpinControlState) through the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cSpinControl_writeToReplayFrame(cSpinControl* self, cReplayFramePtr* obj)
{
    sVEntry001361F8* vt = *(sVEntry001361F8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 88);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cSpinControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00136230 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the spin control state from a replay frame.
 *
 * Reads the 0x58 bytes at offset 0 (all of cSpinControlState) through the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cSpinControl_readFromReplayFrame(cSpinControl* self, cReplayFramePtr* obj)
{
    sVEntry00136230* vt = *(sVEntry00136230**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 88);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cUberGrindControl_gainFocus);
#ifdef SKIP_ASM
float cRiderMetrix_evUberGrindStart(void* self, int a1, int a2);

struct sAnimSet_00136268 {
    int anims[4];       // 0x00
    char pad10[0x10];
    int x20;            // 0x20
};
extern sAnimSet_00136268 kUberGrindAnim[];

/**
 * Starts an uber grind: plays the entry anim of grind CurrentGrind for the rail stance and scores the trick.
 *
 * Picks the entry anim from kUberGrindAnim[CurrentGrind] (0x24 bytes each) by RailslideMode (4, 3, 2, other), turns
 * the anim heading offset a quarter turn (not from stance 3; stance 2 also clears the switch flag), reports
 * the grind (cRiderMetrix_evUberGrindStart) for boost, then sets RailslideMode 3, GrindState 0 and fades anim layers 1 and 0.
 */
extern "C" void cUberGrindControl_gainFocus(cUberGrindControl* self)
{
    sAnimSet_00136268* grind = &kUberGrindAnim[self->CurrentGrind];
    cRider* rider = self->mRider;
    int railStyle = rider->RailslideMode;
    if (railStyle == 4) {
        char* anim = (char*)rider->mRiderAnim;
        *(sQuat_12FEC8*)(anim + 0x30) = t4vect_Origin;
        *(sQuat_12FEC8*)(anim + 0x40) = AxisAngle_12FEC8(t4vect_ZAxis, 1.5707963705062866f);
        cRiderAnimBase_play(self->mRider->mRiderAnim, grind->anims[0], 0, -1.0f);
    } else if (railStyle == 3) {
        cRiderAnimBase_play(rider->mRiderAnim, grind->anims[1], 0, -1.0f);
    } else if (railStyle == 2) {
        rider->IsRidingFakie = 0;
        *(int*)((char*)self->mRider->mRiderAnim + 0x18) = 0;
        char* anim = (char*)self->mRider->mRiderAnim;
        *(sQuat_12FEC8*)(anim + 0x30) = t4vect_Origin;
        *(sQuat_12FEC8*)(anim + 0x40) = AxisAngle_12FEC8(t4vect_ZAxis, 1.5707963705062866f);
        cRiderAnimBase_play(self->mRider->mRiderAnim, grind->anims[2], 0, -1.0f);
    } else {
        char* anim = (char*)rider->mRiderAnim;
        *(sQuat_12FEC8*)(anim + 0x30) = t4vect_Origin;
        *(sQuat_12FEC8*)(anim + 0x40) = AxisAngle_12FEC8(t4vect_ZAxis, 1.5707963705062866f);
        cRiderAnimBase_play(self->mRider->mRiderAnim, grind->anims[3], 0, -1.0f);
    }
    float k = 0.33000001311302185f;
    cRider_flagEventBoostPowerUp(self->mRider, 1, cRiderMetrix_evUberGrindStart(self->mRider->mMetrix, grind->x20, self->mRider->RailslideMode));
    self->mRider->RailslideMode = 3;
    *(int*)self = 0;
    cRider_verifyRailslideModeDEBUG(self->mRider);
    cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 1, k);
    cRiderAnimBase_blendOutAll(self->mRider->mRiderAnim, 0, k);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cUberGrindControl_update);
#ifdef SKIP_ASM
// PORT: the unit declares cRiderAnimBase_play as void, but it returns the new anim index.
extern "C" int cRiderAnimBase_play_i136508(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
extern "C" int cRider_checkRailBoardCollisions(void*);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void cRider_setupRailBalance(void*, float);
extern "C" void cRider_setupCrouchBrake(void* rider, float a, float b);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);
extern "C" void cRider_undoRailslideModeOrientation(void* rider);
extern "C" int cRider_checkForReset_i(void* rider, int a, int b) __asm__("cRider_checkForReset");
extern "C" float cRiderMetrix_evUberGrindEnd(void* p, int v);
int cRider_getMotionState(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
extern "C" void cRailslideControl_playGainFocusAnim(cRailslideControl*, int);
extern "C" bool tEventFlags_isSet1(void* self, int bit);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
int cRiderAnimBase_getCurrentAnimID(void* self, int i);
extern "C" int cRiderAnimBase_isCurrentAnimEndFlag(void* self, int i);


struct sAiBitsA_136508
{
    int pad : 23;
    int v : 6;
};

struct sAiBitsB_136508
{
    int pad : 15;
    int v : 8;
};

// PORT: PS2-only inline asm (float absolute value).
static inline float aiAbs_136508(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

struct sAiObj136508
{
    int mState;
    int mIndex;
    char* mRider;
};

/**
 * Runs one frame of an uber grind: the locked-in entry, the hold while the grind button stays down, the exit.
 *
 * GrindState 0 waits for the entry anim, then plays the hold anim (state 1). State 1 ends when the held grind
 * id (bits 15-22) changes or the rider is on the ground, else leans with RailOffCenter and swaps hold anims.
 * State 2 waits out the exit anim, returns to CS_Railslide, CS_Spin or CS_Cruise, scores the grind and counts
 * mUberLevel up to 10 (reaching 10 sets mUberTimer to 60).
 *
 * @param in  the uber grind command (On Tour tUberGrindCmd): button bits, grind id and the rail balance value
 */
extern "C" void cUberGrindControl_update(cUberGrindControl* self, void* in)
{
    if (cRider_checkForReset_i((char*)self->mRider, (*(int*)in >> 12) & 1, 0))
        return;
    char* tbl = (char*)kUberGrindAnim + self->CurrentGrind * 0x24;
    if (cRider_checkRailBoardCollisions((char*)self->mRider))
    {
        if (self->GrindState == 1)
            cRiderAnimBase_play_i136508(self->mRider->mRiderAnim, *(int*)(tbl + 0x18), 0, -1.0f);
        return;
    }
    int w = *(int*)in;
    float floatZero = 0.0f;
    cRider_setupBoostWarp((char*)self->mRider, (w >> 14) & 1, (w >> 13) & 1);
    cRider_setupRailBalance((char*)self->mRider, ((sAiBitsA_136508*)in)->v * 0.032258063554763794f);
    cRider_setupCrouchBrake((char*)self->mRider, floatZero, floatZero);
    if (self->GrindState == 0)
    {
        cRider* rider0 = self->mRider;
        rider0->mSlewed[6].target = floatZero;
        rider0->mSlewed[6].rate = 0.05833333730697632f;
        if (cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2))
        {
            cRiderAnimBase_play_i136508(self->mRider->mRiderAnim, *(int*)(tbl + 0x10), 0, -1.0f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 1.0f;
            self->GrindState = 1;
        }
    }
    if (self->GrindState == 1)
    {
        if (((sAiBitsB_136508*)in)->v != self->CurrentGrind || cRider_getMotionState((char*)self->mRider) == 0)
        {
            cRiderAnimBase_play_i136508(self->mRider->mRiderAnim, *(int*)(tbl + 0x1C), 0, -1.0f);
            *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0x90) = 1.0f;
            self->GrindState = 2;
        }
        else if (cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2) != *(int*)(tbl + 0x18) ||
                 cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2))
        {
            if (cRider_getMotionState((char*)self->mRider) != 4)
            {
                cRider* rider = self->mRider;
                rider->mSlewed[6].target = 0.0f;
                rider->mSlewed[6].rate = 0.05833333730697632f;
            }
            else
            {
                cRider* rider = self->mRider;
                rider->mSlewed[6].target = rider->mRiderHandler->RailslideMotion.RailOffCenter;
                rider->mSlewed[6].rate = 0.05833333730697632f;
            }
            int cur = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
            int desAnim;
            if (aiAbs_136508(self->mRider->mSlewed[6].cur) < 0.10000000149011612f)
                desAnim = *(int*)(tbl + 0x10);
            else
                desAnim = *(int*)(tbl + 0x14);
            if (desAnim != cur)
                cRiderAnimBase_play_i136508(self->mRider->mRiderAnim, desAnim, 0, -1.0f);
        }
    }
    if (self->GrindState == 2)
    {
        cRider* rider2 = self->mRider;
        rider2->mSlewed[6].target = 0.0f;
        rider2->mSlewed[6].rate = 0.05833333730697632f;
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 21 &&
            !cRiderAnimBase_isCurrentAnimEndFlag(self->mRider->mRiderAnim, 2) &&
            !tEventFlags_isSet1(cRiderAnimBase_getCurrentAnimSequence(self->mRider->mRiderAnim, 2) + 0xB0, 0))
            return;
        if (cRider_getMotionState((char*)self->mRider) == 4)
        {
            cRailslideControl_playGainFocusAnim((cRailslideControl*)(&self->mRider->mRiderHandler->RailslideControl), 0);
            cRider_setControlState_impl((char*)self->mRider, 7);
        }
        else
        {
            cRider_undoRailslideModeOrientation((char*)self->mRider);
            if (cRider_getMotionState((char*)self->mRider) == 1)
                cRider_setControlState_impl((char*)self->mRider, 5);
            else
                cRider_setControlState_impl((char*)self->mRider, 0);
        }
        cRider_flagEventBoostPowerUp((char*)self->mRider, 1, cRiderMetrix_evUberGrindEnd(self->mRider->mMetrix, self->mRider->RailslideMode));
        cRider* rider = self->mRider;
        if (rider->mUberLevel < 10)
        {
            rider->mUberLevel = rider->mUberLevel + 1;
            if (self->mRider->mUberLevel == 10)
                self->mRider->mUberTimer = 60.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cUberGrindControl_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001368E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the uber grind state to a replay frame.
 *
 * Writes the 8 bytes at offset 0 (GrindState, CurrentGrind) through the frame's write virtual (slot 1).
 *
 * @param obj  replay frame to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cUberGrindControl_writeToReplayFrame(cUberGrindControl* self, cReplayFramePtr* obj)
{
    sVEntry001368E8* vt = *(sVEntry001368E8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cUberGrindControl_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00136920 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the uber grind state from a replay frame.
 *
 * Reads the 8 bytes at offset 0 (GrindState, CurrentGrind) through the frame's read virtual (slot 2).
 *
 * @param obj  replay frame to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cUberGrindControl_readFromReplayFrame(cUberGrindControl* self, cReplayFramePtr* obj)
{
    sVEntry00136920* vt = *(sVEntry00136920**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

extern "C" void cRider_updateOrientationImplicit(void*);

//100%
INCLUDE_ASM("ai/ai", cStaticMotion_step);
#ifdef SKIP_ASM
/**
 * Steps the static motion: only refreshes the rider's orientation with cRider_updateOrientationImplicit().
 */
extern "C" void cStaticMotion_step(cStaticMotion* self)
{
    cRider_updateOrientationImplicit(*(void**)self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cStaticMotion_resolve__FPv);
#ifdef SKIP_ASM
/**
 * Resolves the static motion: does nothing (empty body).
 */
void cStaticMotion_resolve(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutMotion_cWipeoutMotion);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_136980 { short delta; short index; void* fn; };
struct sVtbl9_136980 { sVEntry_136980 e[9]; } __attribute__((aligned(8)));
struct sVtbl22_136980 { sVEntry_136980 e[22]; } __attribute__((aligned(8)));

extern const sVtbl9_136980 D_004597B0;
extern char D_00459B90[];
extern const sVtbl22_136980 D_004597F8;

/**
 * Constructs the wipeout motion part, building the shared cRider first when it is the complete object.
 *
 * In charge it points mRider at this + 0x70 and runs cRider_cRider() there; it then installs the
 * cWipeoutMotion-in-cRider vtables at rider + 0x6E8, 0x6D0 and 0x6C0 (stack copies with the this-delta fixed
 * up when not in charge, as g++ 2.95 does for virtual bases) and clears BoardSphereTree.
 *
 * @param inChrg  nonzero when this is the most-derived object and must construct the cRider virtual base
 * @return self
 */
// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0x40, vbase at +0x70); when not most-derived, g++ copies cRider's overridden vtables to the stack and
// fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* cWipeoutMotion_cWipeoutMotion(cWipeoutMotion* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0x70;
        self->mRider = (cRider*)vb;
        cRider_cRider(vb);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_004597B0;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    self->mRider->mVtbl = (void*)&D_004597F8;
    if (!inChrg)
    {
        sVtbl9_136980 scriptVtbl = D_004597B0;
        self->mRider->mScriptObjectVtbl = &scriptVtbl;
        char* base = (char*)self->mRider - 0x70;
        int thisDelta = (char*)self - base;
        scriptVtbl.e[1].delta = D_004597B0.e[1].delta + thisDelta;
        sVtbl22_136980 riderVtbl = D_004597F8;
        self->mRider->mVtbl = &riderVtbl;
        riderVtbl.e[1].delta = D_004597F8.e[1].delta + thisDelta;
    }
    self->BoardSphereTree = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", cWipeoutMotion__cWipeoutMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/ai_classes.h"

/**
 * Destroys the wipeout motion part: deletes its board sphere tree and, as the flags ask, the cRider and self.
 *
 * Written as the prototype class cVbDtor00136AD0, whose m44 is BoardSphereTree (destroyed with
 * cRiderSphereTree__cRiderSphereTree). g++ 2.95 adds the virtual-base vtable fix-ups, cRider__cRider() when
 * in charge (flag bit 1) and operator delete when deleting (flag bit 0).
 */
cVbDtor00136AD0::~cVbDtor00136AD0()
{
    delete m44;
}

// PORT: the compiler names this dtor _$_15cVbDtor00136AD0; expose it under the address-derived name.
__asm__(".globl cWipeoutMotion__cWipeoutMotion\n.type cWipeoutMotion__cWipeoutMotion,@function\ncWipeoutMotion__cWipeoutMotion = _$_15cVbDtor00136AD0\n.size cWipeoutMotion__cWipeoutMotion,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00136AD0$12cRiderVbC_ai = D_004597B0\n"
        "_vt$15cVbDtor00136AD0$13cRiderVbBC_ai = D_00459B90\n"
        "_vt$15cVbDtor00136AD0$11cRiderVb_ai = D_004597F8\n"
        "_$_17cVbMember00136AD0 = cRiderSphereTree__cRiderSphereTree\n"
        "_$_11cRiderVb_ai = cRider__cRider\n");
#endif

