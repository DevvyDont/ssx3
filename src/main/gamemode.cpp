#include "common.h"
#include "be/beintnewrace.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_getGM);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_00536668[];
extern char str_GameModeMan[];
extern char str_GM_Handler[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00416210(void* dst, int c, int n);
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern void* D_0047CC60[];
extern void* D_0047CCC8[];
extern void* D_0047CD30[];
extern void* D_0047CD98[];
extern void* D_0047CE00[];
extern void* D_0047CE68[];
extern void* D_0047CED0[];
extern void* D_0047CF38[];
extern void* D_0047CFA0[];

struct sGMMode_4 {
    void** vt;
    sGMMode_4(void** v) { vt = v; }
};

struct sGMMode_8 {
    void** vt;
    char pad[0x4];
    sGMMode_8(void** v) { vt = v; }
};

struct sGMMode_C {
    void** vt;
    char pad[0x8];
    sGMMode_C(void** v) { vt = v; }
};

struct sGMMode_2C {
    void** vt;
    char pad[0x28];
    sGMMode_2C(void** v) { vt = v; }
};

struct sGMMode_7C {
    void** vt;
    char pad[0x78];
    sGMMode_7C(void** v) { vt = v; }
};

struct sGMMode_D0 {
    void** vt;
    char pad[0xCC];
    sGMMode_D0(void** v) { vt = v; }
};

/**
 * Returns the game-mode manager, creating it and its game-mode handlers on first use.
 *
 * The manager is "GameModeMan" (0xA0 bytes); the handlers ("GM Handler") fill table D_00536668, all but slot
 * 3. Some creations are retried up to three times while the result is 0, and slots 5 and 6 are checked twice.
 * A new manager starts with no event (-1), heat 0 and handler 0.
 *
 * @return the manager (cGameModeMan_instance)
 */
extern "C" int* cGameModeMan_getGM(void)
{
    if (cGameModeMan_instance == 0) {
        cGameModeMan_instance = (cGameModeMan*)((int*)cMemMan_alloc(160, str_GameModeMan, 0, 0));
        func_00416210(D_00536668, 0, 40);
        if (D_00536668[0] == 0) {
            D_00536668[0] = new (str_GM_Handler, 0, 0) sGMMode_7C(D_0047CFA0);
            if (D_00536668[0] == 0) {
                D_00536668[0] = new (str_GM_Handler, 0, 0) sGMMode_7C(D_0047CFA0);
                if (D_00536668[0] == 0) {
                    D_00536668[0] = new (str_GM_Handler, 0, 0) sGMMode_7C(D_0047CFA0);
                }
            }
        }
        if (D_00536668[1] == 0) {
            D_00536668[1] = new (str_GM_Handler, 0, 0) sGMMode_D0(D_0047CF38);
        }
        if (D_00536668[2] == 0) {
            D_00536668[2] = new (str_GM_Handler, 0, 0) sGMMode_4(D_0047CE00);
        }
        if (D_00536668[5] == 0) {
            D_00536668[5] = new (str_GM_Handler, 0, 0) sGMMode_4(D_0047CD30);
        }
        if (D_00536668[4] == 0) {
            D_00536668[4] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CD98);
            if (D_00536668[4] == 0) {
                D_00536668[4] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CD98);
                if (D_00536668[4] == 0) {
                    D_00536668[4] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CD98);
                }
            }
        }
        if (D_00536668[6] == 0) {
            D_00536668[6] = new (str_GM_Handler, 0, 0) sGMMode_4(D_0047CCC8);
        }
        if (D_00536668[7] == 0) {
            D_00536668[7] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CC60);
            if (D_00536668[7] == 0) {
                D_00536668[7] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CC60);
                if (D_00536668[7] == 0) {
                    D_00536668[7] = new (str_GM_Handler, 0, 0) sGMMode_C(D_0047CC60);
                }
            }
        }
        if (D_00536668[8] == 0) {
            D_00536668[8] = new (str_GM_Handler, 0, 0) sGMMode_2C(D_0047CED0);
        }
        if (D_00536668[9] == 0) {
            D_00536668[9] = new (str_GM_Handler, 0, 0) sGMMode_8(D_0047CE68);
            if (D_00536668[9] == 0) {
                D_00536668[9] = new (str_GM_Handler, 0, 0) sGMMode_8(D_0047CE68);
                if (D_00536668[9] == 0) {
                    D_00536668[9] = new (str_GM_Handler, 0, 0) sGMMode_8(D_0047CE68);
                }
            }
        }
        if (D_00536668[5] == 0) {
            D_00536668[5] = new (str_GM_Handler, 0, 0) sGMMode_4(D_0047CD30);
        }
        if (D_00536668[6] == 0) {
            D_00536668[6] = new (str_GM_Handler, 0, 0) sGMMode_4(D_0047CCC8);
        }
        cGameModeMan* gm = cGameModeMan_instance;
        cGameModeHandler_pManager = (cGameModeMan*)((char*)gm);
        gm->iCurrentGameEvent = -1;
        gm->iHeatNum = 0;
        gm->iCurrModeHandler = 0;
        gm->unk_80 = 0;
    }
    return (int*)cGameModeMan_instance;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_purgeGM);
#ifdef SKIP_ASM
class cMode2380E8 {
public:
    virtual ~cMode2380E8();
};
void operator_delete(int*);
extern void* D_00536668[];

/**
 * Destroys the game mode manager: deletes the ten mode handlers and the singleton.
 *
 * Deletes each handler in D_00536668 (On Tour aHandler), frees cGameModeMan_instance and sets it to null.
 */
extern "C" void cGameModeMan_purgeGM(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        delete (cMode2380E8*)D_00536668[i];
    }
    if (cGameModeMan_instance != 0) {
        operator_delete((int*)cGameModeMan_instance);
    }
    cGameModeMan_instance = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_initGameMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C11[];
extern "C" void cGameModeMan_restartHeat(cGameModeMan* self);

struct sVE_initGameMode { short delta; short index; void (*fn)(void*); };

struct sGameModeMan {
    int f0;
    int mode;      // 0x4
    unsigned int type;  // 0x8
    int net;       // 0xC
    char pad[0x84 - 0x10];
    int f84;       // 0x84
};

/**
 * Switches the game-mode manager to a new event type and picks the matching mode handler.
 *
 * Purges the current handler (its slot 3, func_00238C80), stores @p type in iCurrentGameEvent and the new race's
 * cGamePath in iCurrentGamePath. Online (path 2): events 0 -> handler 8, 1-3 -> 9, 4 -> 5, 5 -> 6, others 8.
 * Offline: 1-3 -> 0, 0 -> 1, 12 -> 2, 6-8 -> 4, 4 -> 5, 5 -> 6, 9-11 -> 7; other types keep the purged
 * handler index. Then sets bIsEventDone and calls cGameModeMan_restartHeat().
 *
 * @param type  the BE game event (newRace.cGameEvent)
 */
extern "C" void cGameModeMan_initGameMode(cGameModeMan* self, unsigned int type)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* gm = (char*)D_00536668[self->iCurrModeHandler];
    sVE_initGameMode* vt = *(sVE_initGameMode**)gm;
    vt[3].fn(gm + vt[3].delta);
    self->iCurrentGameEvent = (int)type;
    int net = D_00535C11[0];
    self->iCurrentGamePath = net;
    if (net == 2) {
        switch (type) {
        case 0: self->iCurrModeHandler = 8; break;
        case 1: self->iCurrModeHandler = 9; break;
        case 3: self->iCurrModeHandler = 9; break;
        case 2: self->iCurrModeHandler = 9; break;
        case 4: self->iCurrModeHandler = 5; break;
        case 5: self->iCurrModeHandler = 6; break;
        default: self->iCurrModeHandler = 8; break;
        }
    } else {
        switch (type) {
        case 1:
        case 2:
        case 3:
            self->iCurrModeHandler = 0;
            break;
        case 0: self->iCurrModeHandler = 1; break;
        case 12: self->iCurrModeHandler = 2; break;
        case 4: self->iCurrModeHandler = 5; break;
        case 6: self->iCurrModeHandler = 4; break;
        case 7: self->iCurrModeHandler = 4; break;
        case 8: self->iCurrModeHandler = 4; break;
        case 5: self->iCurrModeHandler = 6; break;
        case 9:
        case 10:
        case 11:
            self->iCurrModeHandler = 7;
            break;
        }
    }
    self->bIsEventDone = 1;
    cGameModeMan_restartHeat(self);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_002382B8);
#ifdef SKIP_ASM
extern int D_00536730[];

extern "C" void func_002382B8(cGameModeMan* self, int i)
{
    D_00536730[i] = -1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_restartHeat);
#ifdef SKIP_ASM
struct sVEntry_002382D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_00536668[];
extern int cGameModeMan_bGiveUp[];

/**
 * Restarts the current heat: calls the current handler's virtual slot 2 and clears the six riders' cGameModeMan_bGiveUp
 * values.
 */
extern "C" void cGameModeMan_restartHeat(cGameModeMan* self)
{
    int i;
    void* obj = D_00536668[self->iCurrModeHandler];
    sVEntry_002382D8* vt = *(sVEntry_002382D8**)obj;
    vt[2].fn((char*)obj + vt[2].delta);
    for (i = 5; i >= 0; i--) {
        cGameModeMan_bGiveUp[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238348__FPv);
#ifdef SKIP_ASM
int func_00238348(void* self)
{
    int restoreHeat = *(int*)((char*)self + 0x74);
    *(int*)((char*)self + 0x70) = restoreHeat;
    return restoreHeat;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_raceFinish);
#ifdef SKIP_ASM
extern void* D_00536668[];
extern unsigned int D_00536640[];
extern int D_00536730[];
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void func_00154AB8(void* self, int idx, int a2, unsigned int v);
extern "C" void func_00154EE8(void* self, int idx, int a2, unsigned int v);
extern "C" int cBERewardInterface_isMountainConquered(void* self, int a, int b);
extern "C" int cBERewardInterface_isAwardReceived(void* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_awardUser(void* self, int a1, int a2, int bit);
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);

struct sVEntry_00238358 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Handles a rider finishing the race: tells the current mode handler, then records the result in the back
 * end.
 *
 * The result (with the rider's time or score, D_00536640) is only recorded at locations below 17 and when the
 * replay state is not 1-9. For rider 0, a conquered mountain leads to cBERewardInterface_awardUser() when
 * cBERewardInterface_isAwardReceived returns 0 (probably: not yet awarded). Locked results also record the rider's place
 * (D_00536730).
 *
 * @param idx  rider index (On Tour iRider)
 * @param pRiderStats   the rider's stats (On Tour pRiderStats), passed to the score interface
 */
extern "C" void cGameModeMan_raceFinish(cGameModeMan* self, int idx, int pRiderStats)
{
    void* obj = D_00536668[self->iCurrModeHandler];
    sVEntry_00238358* vt = *(sVEntry_00238358**)obj;
    vt[8].fn((char*)obj + vt[8].delta, idx);
    if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) < 17) {
        int n = *(int*)SSXApp->Game->Replay;
        int ok = n != 0 && n < 10;
        if (!ok) {
            void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
            unsigned int riderScore = D_00536640[idx];
            func_00154AB8(score, idx, pRiderStats, riderScore);
            if (idx == 0) {
                int prof = cBELibrary_getProfileIndex(0);
                int ch = cBELibrary_getCharacterID(0);
                void* rewardInt = cBE_getInterface_Fv(cBE_getBE(), 13);
                if (cBERewardInterface_isMountainConquered(rewardInt, prof, ch)) {
                    if (cBERewardInterface_isAwardReceived(rewardInt, prof, prof, 0) == 0) {
                        cBERewardInterface_awardUser(rewardInt, prof, ch, 0);
                    }
                }
            }
            if (self->bAreResultsLocked) {
                func_00154EE8(score, idx, D_00536730[idx], riderScore);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_riderCheckpointHit);
#ifdef SKIP_ASM
struct sVEntry_00238510 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

/**
 * Tells the current game mode that a rider passed a checkpoint.
 *
 * Calls virtual slot 6 of the current handler (D_00536668[+0x4]) with both arguments.
 *
 * @param iRider  the rider
 * @param iSecsAdded  the seconds the checkpoint adds
 */
extern "C" void cGameModeMan_riderCheckpointHit(void* self, int iRider, int iSecsAdded)
{
    void* obj = D_00536668[*(int*)((char*)self + 0x4)];
    sVEntry_00238510* vt = *(sVEntry_00238510**)obj;
    vt[6].fn((char*)obj + vt[6].delta, iRider, iSecsAdded);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_riderGateHit);
#ifdef SKIP_ASM
struct sVEntry_00238550 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

/**
 * Tells the current game-mode handler that a rider passed a gate (its virtual slot 7).
 *
 * @param iRider  rider index (On Tour iRider)
 * @param iGateID  gate id (On Tour iGateID)
 */
extern "C" void cGameModeMan_riderGateHit(cGameModeMan* self, int iRider, int iGateID)
{
    void* obj = D_00536668[self->iCurrModeHandler];
    sVEntry_00238550* vt = *(sVEntry_00238550**)obj;
    vt[7].fn((char*)obj + vt[7].delta, iRider, iGateID);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_getHeatScore);
#ifdef SKIP_ASM
struct sVEntry_00238590 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

/**
 * Asks the current game-mode handler (aHandler[iCurrModeHandler], slot 9) for a rider's score in one heat.
 *
 * Declared void here, but the handler's result is left in $v0 (On Tour returns unsigned int).
 *
 * @param iRider  the rider index (On Tour iRider)
 * @param iHeat  the heat (On Tour iHeat)
 */
extern "C" void cGameModeMan_getHeatScore(cGameModeMan* self, int iRider, int iHeat)
{
    void* obj = D_00536668[self->iCurrModeHandler];
    sVEntry_00238590* vt = *(sVEntry_00238590**)obj;
    vt[9].fn((char*)obj + vt[9].delta, iRider, iHeat);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_writeToReplayFrame);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
class cGmStream_385D0 {
public:
    virtual void write(void* p, int n);
    virtual void read(void* p, int n);
};

class cGameModeObj_2385D0 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void save(cGmStream_385D0* s);
    virtual void load(cGmStream_385D0* s);
};

extern int D_00536708[];

/**
 * Saves the game-mode manager to a replay frame, then lets the current handler save itself (its save virtual).
 *
 * Writes iCurrModeHandler, iHeatNum, iCurrentGameEvent, iNumberAI, iSimAI, iCharacters, iLocation, iNextLocation,
 * iNextHeat, the score tables D_00536730 and D_00536708, uiTimeLimit, uiPointTarget, unk_80, the table D_00536640,
 * then bIsEventDone through bAreResultsLocked. iCurrentGamePath, iCheatCharacters and iRestoreHeat are not saved.
 *
 * @param data  the replay frame written to (On Tour cReplayFramePtr* data)
 */
extern "C" void cGameModeMan_writeToReplayFrame(cGameModeMan* self, cGmStream_385D0* data)
{
    data->write(((char*)self) + 4, 4);
    data->write(((char*)self), 4);
    data->write(((char*)self) + 8, 4);
    data->write(((char*)self) + 0x10, 4);
    data->write(((char*)self) + 0x14, 4);
    data->write(((char*)self) + 0x18, 40);
    data->write(((char*)self) + 0x68, 4);
    data->write(((char*)self) + 0x6C, 4);
    data->write(((char*)self) + 0x70, 4);
    data->write(D_00536730, 40);
    data->write(D_00536708, 40);
    data->write(((char*)self) + 0x78, 4);
    data->write(((char*)self) + 0x7C, 4);
    data->write(((char*)self) + 0x80, 4);
    data->write(D_00536640, 40);
    data->write(((char*)self) + 0x84, 4);
    data->write(((char*)self) + 0x88, 4);
    data->write(((char*)self) + 0x8C, 4);
    data->write(((char*)self) + 0x90, 4);
    data->write(((char*)self) + 0x94, 4);
    data->write(((char*)self) + 0x98, 4);
    data->write(((char*)self) + 0x9C, 4);
    ((cGameModeObj_2385D0*)D_00536668[self->iCurrModeHandler])->save(data);
}
#endif
//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_readFromReplayFrame);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
class cGameModeObj_2388A0 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void save(cGmStream_385D0* s);
    virtual void load(cGmStream_385D0* s);
};

/**
 * Restores the game-mode manager from a replay frame in cGameModeMan_writeToReplayFrame's order, then the handler.
 *
 * The handler is aHandler[iCurrModeHandler] after iCurrModeHandler has been read back (its load virtual).
 *
 * @param data  the replay frame read from (On Tour cReplayFramePtr* data)
 */
extern "C" void cGameModeMan_readFromReplayFrame(cGameModeMan* self, cGmStream_385D0* data)
{
    data->read(((char*)self) + 4, 4);
    data->read(((char*)self), 4);
    data->read(((char*)self) + 8, 4);
    data->read(((char*)self) + 0x10, 4);
    data->read(((char*)self) + 0x14, 4);
    data->read(((char*)self) + 0x18, 40);
    data->read(((char*)self) + 0x68, 4);
    data->read(((char*)self) + 0x6C, 4);
    data->read(((char*)self) + 0x70, 4);
    data->read(D_00536730, 40);
    data->read(D_00536708, 40);
    data->read(((char*)self) + 0x78, 4);
    data->read(((char*)self) + 0x7C, 4);
    data->read(((char*)self) + 0x80, 4);
    data->read(D_00536640, 40);
    data->read(((char*)self) + 0x84, 4);
    data->read(((char*)self) + 0x88, 4);
    data->read(((char*)self) + 0x8C, 4);
    data->read(((char*)self) + 0x90, 4);
    data->read(((char*)self) + 0x94, 4);
    data->read(((char*)self) + 0x98, 4);
    data->read(((char*)self) + 0x9C, 4);
    ((cGameModeObj_2388A0*)D_00536668[self->iCurrModeHandler])->load(data);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238B70);
#ifdef SKIP_ASM
extern int D_00536730[];
extern int D_00536708[];
extern "C" int func_00245E30(const void* a, const void* b);
extern "C" int func_00245EC0(const void* a, const void* b);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));

extern "C" void func_00238B70(void* self, int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        D_00536730[i] = i;
        D_00536708[i] = i;
    }
    func_00418EF8(D_00536708, n, 4, func_00245E30);
    func_00418EF8(D_00536730, n, 4, func_00245EC0);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238BF8);
#ifdef SKIP_ASM
extern int D_00536730[];
extern int D_00536708[];
extern "C" int func_00245E78(const void* a, const void* b);
extern "C" int func_00245EC0(const void* a, const void* b);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));

extern "C" void func_00238BF8(void* self, int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        D_00536730[i] = i;
        D_00536708[i] = i;
    }
    func_00418EF8(D_00536708, n, 4, func_00245E78);
    func_00418EF8(D_00536730, n, 4, func_00245EC0);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238C80);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sGM00238C80 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
};

struct sVEntry00238C80 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00238C80(void* self)
{
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->bIsEventDone = 1;
    gm->iCurrModeHandler = -1;
    gm->iLocation = -1;
    gm->iNextLocation = -1;
    gm->iCurrentGameEvent = -1;
    gm->bIsLastHeat = 0;
    gm->iHeatNum = 0;
    gm->iNextHeat = 0;
    gm->iRestoreHeat = 0;
    gm->iNumberAI = 0;
    gm->iSimAI = 0;
    gm->uiTimeLimit = 0;
    gm->uiPointTarget = 0;
    gm->unk_80 = 0;
    gm->bIsTimedEvent = 0;
    gm->bHasPointTarget = 0;
    gm->unk_90 = 0;
    gm->unk_94 = 0;
    func_00416210(gm->iCharacters, 0, 40);
    func_00416210(cGameModeHandler_pManager->iCheatCharacters, 0, 40);
    sVEntry00238C80* e = &(*(sVEntry00238C80**)self)[5];
    e->fn((char*)self + e->delta);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238D30);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern int D_00536730[];
extern int D_00536708[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_bGiveUp[];

extern "C" void func_00238D30(void)
{
    func_00416210(D_00536730, 9, 40);
    func_00416210(D_00536708, 0, 40);
    func_00416210(D_00536640, 0, 40);
    func_00416210(D_005366A8, 0, 40);
    func_00416210(cGameModeMan_bGiveUp, 0, 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238DA8);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern int D_00536730[];
extern int D_00536708[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_bGiveUp[];

extern "C" void func_00238DA8(void)
{
    func_00416210(D_00536730, 9, 40);
    func_00416210(D_00536708, 0, 40);
    func_00416210(D_00536640, 9, 40);
    func_00416210(D_005366A8, 0, 40);
    func_00416210(cGameModeMan_bGiveUp, 0, 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238E20);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int func_001454F8(void* iface, int a1);
extern "C" void func_00238B70(void* self, int n);
// PORT: func_00239938 is defined as (void); this caller passes self.
extern "C" void func_00239938_self(void* self) __asm__("func_00239938");
struct sScore_239AA0;
extern "C" void func_00239AA0(sScore_239AA0* self);
extern "C" void* func_00416210(void* dst, int c, int n);
extern signed char D_00535C11[];
extern unsigned int D_00536640[];
extern int D_00536708[];

struct sGM_238E20 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_238E20 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sScore_238E20 {
    sVEntry_238E20* vt;
    int t[3][10];
};

struct sRace_238E20 {
    char pad00[0x20];
    int f20;
    int f24;
};

static inline sRace_238E20* race_238E20()
{
    return (sRace_238E20*)SSXApp->Game->AI;
}

extern "C" void func_00238E20(sScore_238E20* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int i;
    cGameModeMan* gm = cGameModeHandler_pManager;
    int quick = D_00535C11[0];
    gm->bHasPointTarget = 0;
    gm->uiPointTarget = 0;
    if (quick != 0) {
        gm->iNumberAI = 5;
        if (gm->iCurrentGameEvent == 1) {
            gm->iSimAI = 4;
        } else {
            gm->iSimAI = 5;
        }
        cGameModeMan* gameModeMan = cGameModeHandler_pManager;
        gameModeMan->iNextLocation = -1;
        gameModeMan->unk_90 = 0;
        if (gameModeMan->iCurrentGameEvent == 1) {
            gameModeMan->unk_94 = gameModeMan->iCurrentGameEvent;
        } else {
            gameModeMan->unk_94 = 0;
        }
        func_00416210(self->t, 0, 0x78);
        self->vt[4].fn((char*)self + self->vt[4].delta);
        if (cGameModeHandler_pManager->bIsEventDone == 1) {
            func_00239938_self(self);
        }
        cGameModeMan* g2 = cGameModeHandler_pManager;
        g2->iNextHeat = 3;
        g2->iRestoreHeat = 3;
        func_00239AA0((sScore_239AA0*)self);
    }
    cGameModeHandler_pManager->uiTimeLimit = func_001454F8(race, cGameModeHandler_pManager->iHeatNum) * 60;
    cGameModeMan* g3 = cGameModeHandler_pManager;
    if (g3->iCurrentGameEvent == 2) {
        g3->unk_94 = 0;
    } else {
        g3->unk_94 = 1;
    }
    cGameModeMan* g = cGameModeHandler_pManager;
    int s = g->iNextHeat;
    g->iHeatNum = s;
    switch (s) {
    case 0:
    case 1: {
        g->iHeatNum = 1;
        g->iNextHeat = 1;
        g->iRestoreHeat = 1;
        g->iNumberAI = 5;
        g->bIsTimedEvent = 1;
        g->unk_90 = 0;
        g->bAreResultsLocked = 0;
        g->bIsLastHeat = 0;
        g->unk_80 = 0;
        g->iSimAI = 5;
        g->iNextLocation = -1;
        self->t[0][0] = 0;
        self->t[1][0] = 0;
        self->t[2][0] = 0;
        int* t0 = self->t[0];
        if (g->bIsEventDone == 1) {
            func_00416210(t0, 0, 0x78);
            self->vt[4].fn((char*)self + self->vt[4].delta);
            func_00239938_self(self);
            func_00239AA0((sScore_239AA0*)self);
        }
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = self->t[2][i];
        }
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        {
            cGameModeMan* h = cGameModeHandler_pManager;
            race_238E20()->f20 = h->iCharacters[D_00536708[0]];
            race_238E20()->f24 = h->iCharacters[D_00536708[1]];
            for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
                D_00536640[i] = t0[i];
            }
        }
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        cGameModeHandler_pManager->bIsEventDone = 0;
        break;
    }
    case 2:
        g->bAreResultsLocked = 0;
        g->iNextHeat = 1;
        g->iRestoreHeat = 1;
        g->bIsLastHeat = 0;
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = self->t[1][i] + self->t[0][i];
        }
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        break;
    case 3:
        g->bIsLastHeat = 1;
        g->iNextHeat = 3;
        g->iRestoreHeat = 3;
        g->bIsTimedEvent = 1;
        g->bAreResultsLocked = 0;
        g->bIsEventDone = 0;
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = self->t[2][i];
        }
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239230);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishScore(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_00147D20(void* iface, int a1);
extern "C" void func_00147E18(void* iface, int a1);
extern "C" void func_00238B70(void* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_00536730[];
extern signed char D_00535C11[];


struct sScore_239230 {
    void* vt;
    int t[3][10];
};

// cRiderMetrix block copied out of the rider (0xAC bytes; finish time at 0x9C).
struct sStats_239230 {
    int v[0x9C / 4];
    int time;           // 0x9C
    int w[3];
};

// The race (*(SSXApp + 0x84) + 0xC): rider list at 0x28, player objects at 0x40, last index at 0x80.
struct sRace_239230 {
    char pad_0x0[0x28];
    cRider* riders[6];  // 0x28
    char* players[16];  // 0x40: each holds its cRider virtual-base pointer at +0x18
    int f80;            // 0x80
};

static inline sRace_239230* Race_239230()
{
    return (sRace_239230*)SSXApp->Game->AI;
}

static inline cRider* Rider_239230(int i)
{
    return Race_239230()->riders[i];
}

extern "C" void func_00239230(sScore_239230* self, int idx)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    int quick = D_00535C11[0];
    int n = cBENewPlayerInterface_getHumanRider(player, 0);
    sStats_239230 st = *(sStats_239230*)((char*)Rider_239230(idx)->mMetrix + 0xFC);
    if (Rider_239230(idx)->mDidNotFinish != 0) {
        self->t[cGameModeHandler_pManager->iHeatNum - 1][idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        self->t[cGameModeHandler_pManager->iHeatNum - 1][idx] = st.time;
        D_005366A8[idx] = 0;
    }
    if (cAI_isRaceFinished(Race_239230()) == 0)
        return;
    cGameModeMan* gm = cGameModeHandler_pManager;
    switch (gm->iHeatNum) {
    case 1:
        gm->iRestoreHeat = gm->iNextHeat;
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = self->t[0][i] + self->t[1][i];
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        if (D_00536730[idx] < 3)
            cGameModeHandler_pManager->iNextHeat = 3;
        else
            cGameModeHandler_pManager->iNextHeat = cGameModeHandler_pManager->iHeatNum + 1;
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = self->t[0][i];
        if (D_00536730[idx] >= 3) {
            func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
            if (D_00536730[idx] >= 3) {
                Rider_239230(idx)->unk_100 = 0;
                break;
            }
        }
        Rider_239230(idx)->unk_100 = 1;
        break;
    case 2:
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = self->t[0][i] + self->t[1][i];
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = self->t[1][i];
        gm = cGameModeHandler_pManager;
        gm->iRestoreHeat = gm->iNextHeat;
        if (D_00536730[idx] < 3) {
            gm->iNextHeat = 3;
            Rider_239230(idx)->unk_100 = 1;
            if (quick == 0 && D_00536730[idx] == 0)
                func_00147D20(player, 0);
        } else {
            gm->iNextHeat = 1;
            Rider_239230(idx)->unk_100 = 0;
            if (quick == 0)
                func_00147E18(player, 0);
        }
        break;
    case 3: {
        gm->bAreResultsLocked = 1;
        int last = Race_239230()->f80;
        for (int i = 0; i < last + 1; i++) {
            cRider* rider = Rider_239230(i);
            int done = rider->mFinishTime >= 0.0f;
            if (!done)
                self->t[2][i] = cRider_estimateFinishScore(rider);
        }
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = self->t[2][i];
        func_00238B70((char*)cGameModeHandler_pManager, cGameModeHandler_pManager->iNumberAI + 1);
        gm = cGameModeHandler_pManager;
        gm->iRestoreHeat = gm->iNextHeat;
        int lost = !(D_00536730[idx] < 3);
        if (quick == 0 && !lost) {
            gm->iNextHeat = 0;
            gm->iRestoreHeat = 0;
        } else {
            gm->iNextHeat = 3;
        }
        if (lost) {
            Rider_239230(idx)->unk_100 = 0;
            if (quick == 0)
                func_00147E18(player, 0);
        } else {
            Rider_239230(idx)->unk_100 = 1;
            if (quick == 0)
                func_00147D20(player, 0);
        }
        break;
    }
    }
    int place = D_00536730[n];
    if (place == 0)
        (*(cRider**)(Race_239230()->players[0] + 0x18))->unk_2F8 = 0.699999988079071f;
    else if (place == 1)
        (*(cRider**)(Race_239230()->players[0] + 0x18))->unk_2F8 = 0.3499999940395355f;
    else
        (*(cRider**)(Race_239230()->players[0] + 0x18))->unk_2F8 = 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_002398E8);
#ifdef SKIP_ASM

extern "C" int func_002398E8(void* self, int player, int secs)
{
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    if (gameModeMan->iCurrentGameEvent == 1) {
        unsigned int limit = *(unsigned int*)((char*)SSXApp->Game->AI + 8);
        unsigned int newTimeLimit = gameModeMan->uiTimeLimit + secs * 60;
        gameModeMan->uiTimeLimit = newTimeLimit;
        if (newTimeLimit < limit) {
            return 0;
        }
        return 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239938);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_00145750(void* iface);
extern "C" int func_00147410(void* iface, int player);
extern "C" void func_0023C770(int* arr, unsigned int n);

struct sGM_239938 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
};

extern "C" void func_00239938(void)
{
    int n = 0;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
    int charA = cBENewPlayerInterface_getRiderCharID(player, humanRider);
    int charB = func_00145750(race);
    int list[10];

    for (int i = 0; i < 10; i++) {
        if (i != charA && i != charB) {
            list[n] = i;
            n++;
        }
    }
    func_0023C770(list, n);
    cGameModeHandler_pManager->iCharacters[0] = charA;
    cGameModeHandler_pManager->iCheatCharacters[0] = func_00147410(player, humanRider);
    cGameModeHandler_pManager->iCharacters[1] = charB;
    cGameModeHandler_pManager->iCheatCharacters[1] = 0;
    for (int j = 0; j < n && j + 2 < 10; j++) {
        cGameModeHandler_pManager->iCharacters[j + 2] = list[j];
        cGameModeHandler_pManager->iCheatCharacters[j + 2] = 0;
    }
    if (cBENewRaceInterface_newRace.cGamePath != 0) {
        cGameModeHandler_pManager->iCharacters[1] = list[n - 1];
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239AA0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" short func_00147CB8(void* self, int which);
extern "C" int func_001453D0(void* self, int kind, int place, int mode);
extern "C" void func_00238B70(void* self, int n);
extern unsigned int D_00536640[];
extern int D_00536730[];

struct sOpts_239AA0 {
    char pad00[0x49];
    signed char f49;
};

struct sScore_239AA0 {
    int f0;
    int t[3][10];
};

static inline int opt49_239AA0(sOpts_239AA0* o)
{
    return o->f49;
}

extern "C" void func_00239AA0(sScore_239AA0* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    int nB = cGameModeHandler_pManager->iSimAI;
    int d = cGameModeHandler_pManager->iNumberAI - nB;
    int k = 0;
    for (int i = 0; i < d + 1; i++) {
        D_00536730[i] = 5 - i;
        for (int j = 0; j < 3; j++) {
            self->t[j][i] = 0;
        }
        D_00536640[i] = self->t[0][i];
        k++;
    }
    int nA = d + 1;
    for (int r = 0; r < nB; r++) {
        for (int c = 0; c < 3; c++) {
            int b = opt49_239AA0((sOpts_239AA0*)((signed char*)&cBENewRaceInterface_newRace));
            int same = 0;
            if (c != 0) same = 1;
            if (b == 0) same = 0;
            if (same) {
                self->t[c][k] = self->t[0][k];
            } else {
                self->t[c][k] = func_001453D0(race, c + 1, r, func_00147CB8(player, 0));
            }
        }
        D_00536640[k] = self->t[0][k];
        D_00536730[k] = r;
        k++;
    }
    func_00238B70((char*)cGameModeHandler_pManager, nA + nB);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239CE0);
#ifdef SKIP_ASM
struct sScoreTable239CE0 {
    int f0;
    int tbl[8][10];
};

extern "C" int func_00239CE0(sScoreTable239CE0* self, int col, int row)
{
    if (cGameModeHandler_pManager->iHeatNum < row) {
        return 0;
    }
    return self->tbl[row][col];
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239D18);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cGameModeStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00239D18(void* self, cGameModeStream* s)
{
    s->v01((char*)self + 0x4, 160);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239D50);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_00239D50(void* self, cGameModeStream* s)
{
    s->v02((char*)self + 0x4, 160);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239D88);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* iface, int player);
extern "C" int func_001454F8(void* iface, int a1);
extern "C" int func_001474C8(void* iface, int player);

struct sGM_239D88 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_239D88 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00239D88(void* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->unk_80 = 0;
    gm->bIsEventDone = 0;
    gm->unk_94 = 0;
    gm->unk_90 = 0;
    gm->bAreResultsLocked = 0;
    gm->iNumberAI = 0;
    gm->iSimAI = 0;
    gm->bIsLastHeat = 1;
    gm->iLocation = -1;
    gm->iNextLocation = -1;
    gm->iHeatNum = 3;
    gm->iNextHeat = 3;
    gm->iRestoreHeat = 3;
    gm->bIsTimedEvent = 1;
    int t = func_001454F8(race, 3);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->uiPointTarget = 0;
    gm2->bHasPointTarget = 0;
    gm2->uiTimeLimit = t * 60;
    func_00416210(gm2->iCharacters, -1, 40);
    sVEntry_239D88* e = &(*(sVEntry_239D88**)self)[4];
    e->fn((char*)self + e->delta);
    cGameModeHandler_pManager->iCharacters[0] = cBENewPlayerInterface_getPlayerCharID(player, 0);
    cGameModeHandler_pManager->iCharacters[1] = cBENewPlayerInterface_getPlayerCharID(player, 1);
    cGameModeHandler_pManager->iCheatCharacters[0] = func_001474C8(player, 0);
    cGameModeHandler_pManager->iCheatCharacters[1] = func_001474C8(player, 1);
    *(short*)((char*)self + 0x4) = 0;
    *(short*)((char*)self + 0x6) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239EC8);
#ifdef SKIP_ASM
extern "C" void func_00239EC8(void* self, int idx)
{
    sRace_239230* race = Race_239230();
    sStats_239230 st = *(sStats_239230*)((char*)race->riders[idx]->mMetrix + 0xFC);
    bool dnf = Rider_239230(idx)->mDidNotFinish != 0;
    if (dnf) {
        D_00536640[idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        D_00536640[idx] = st.time;
        D_005366A8[idx] = 0;
    }
    bool over = cAI_isRaceFinished(race) != 0;
    if (over) {
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bIsEventDone = 1;
        gm->bAreResultsLocked = 1;
        func_00238B70(gm, 2);
        if (D_00536730[idx] > 0) {
            Rider_239230(idx)->unk_100 = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A070);
#ifdef SKIP_ASM

extern "C" int func_0023A070(char* self, int player, int secs)
{
    short* score = (short*)(self + 4);
    short* mine = (short*)((char*)score + (player << 1));
    short newScore = ++*mine;
    int other = 0;
    if (player == 0) {
        other = 1;
    }
    if (*(short*)((char*)score + (other << 1)) >= newScore) {
        return 0;
    }
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    if (gameModeMan->iCurrentGameEvent == 1) {
        unsigned int timeLimit = gameModeMan->uiTimeLimit;
        if (timeLimit < *(unsigned int*)((char*)SSXApp->Game->AI + 8)) {
            return 0;
        }
        gameModeMan->uiTimeLimit = secs * 60 + timeLimit;
        return 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A108);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_00145750(void* iface);
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void func_0023A4F0(void* self);
extern "C" void func_0023A668(void* self);
extern signed char D_00535C11[];
extern unsigned int D_00536640[];
extern int D_00536730[];
extern int D_00536708[];

struct sGM_23A108 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23A108 {
    short delta;
    short index;
    void (*fn)(void*);
};

class cGameModeBase_23A108 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

struct sMode_23A108 : public cGameModeBase_23A108 {
    int f4;                     // 0x04
    int f8;                     // 0x08
    int fC;                     // 0x0C
    char pad10[0x18];           // 0x10
    int charA;                  // 0x28
    int othersA[5];             // 0x2C
    int charB;                  // 0x40
    int charC;                  // 0x44
    int othersB[4];             // 0x48
    int tbl[3][10];             // 0x58
};

static inline void fillOthers_23A108(int* dst, int p)
{
    for (int i = 0; i < 3; i++) {
        int v = D_00536708[i];
        if (v != p) {
            cGameModeMan* gm = cGameModeHandler_pManager;
            int x = gm->iCheatCharacters[v];
            if (x == 0) {
                *dst = gm->iCharacters[v];
            } else {
                *dst = x;
            }
            dst++;
        }
    }
}

extern "C" void func_0023A108(sMode_23A108* self)
{
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    gameModeMan->bAreResultsLocked = 0;
    gameModeMan->iHeatNum = gameModeMan->iNextHeat;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int quick = D_00535C11[0];
    if (quick != 0 && cGameModeHandler_pManager->iHeatNum != 3) {
        cGameModeHandler_pManager->iHeatNum = 3;
        self->fC = 0;
    }
    cGameModeMan* gm = cGameModeHandler_pManager;
    int state = gm->iHeatNum;
    gm->uiPointTarget = 0;
    gm->bHasPointTarget = 0;
    switch (state) {
    case 0:
    case 1:
        gm->bIsLastHeat = 0;
        gm->iHeatNum = 1;
        gm->iSimAI = 0;
        gm->iNumberAI = 5;
        if (gm->bIsEventDone == 1) {
            func_0023A4F0(self);
            func_0023A668(self);
        }
        func_00416210(self->tbl, 9, 0x78);
        self->v05();
        {
            cGameModeMan* g = cGameModeHandler_pManager;
            g->iNextLocation = -1;
            g->iNextHeat = 1;
            g->iRestoreHeat = 1;
            g->uiTimeLimit = 0;
            g->unk_80 = 0;
            g->bIsEventDone = 0;
            g->bIsTimedEvent = 0;
            g->unk_90 = 1;
            g->unk_94 = 0;
        }
        self->f8 = 0;
        self->fC = 0;
        break;
    case 2:
        gm->bIsLastHeat = 0;
        gm->iNumberAI = 5;
        gm->iSimAI = 0;
        if (self->f8 == 0) {
            int p = cBENewPlayerInterface_getHumanRider(player, 0);
            self->charA = cBENewPlayerInterface_getRiderCharID(player, p);
            fillOthers_23A108(self->othersA, p);
            func_0023A668(self);
        }
        {
            cGameModeMan* g = cGameModeHandler_pManager;
            g->iNextLocation = -1;
            g->iNextHeat = 2;
            g->iRestoreHeat = 2;
        }
        func_00416210(D_00536640, 9, 40);
        func_00416210(D_00536730, 0, 40);
        func_00416210(self->tbl[1], 9, 40);
        {
            cGameModeMan* g = cGameModeHandler_pManager;
            g->uiTimeLimit = 0;
            g->unk_80 = 0;
            g->bIsEventDone = 0;
            g->bIsTimedEvent = 0;
            g->unk_90 = 0;
            g->unk_94 = 0;
        }
        self->f8 = 1;
        self->fC = 0;
        break;
    case 3:
        gm->iNumberAI = 5;
        gm->iSimAI = 0;
        gm->bIsLastHeat = 1;
        if (self->fC == 0) {
            if (quick != 0) {
                func_0023A4F0(self);
                cGameModeHandler_pManager->iHeatNum = 1;
                func_0023A668(self);
                cGameModeHandler_pManager->iHeatNum = state;
                func_00416210(self->tbl, 9, 0x78);
                self->v05();
            } else {
                int p = cBENewPlayerInterface_getHumanRider(player, 0);
                self->charB = cBENewPlayerInterface_getRiderCharID(player, p);
                self->charC = func_00145750(race);
                fillOthers_23A108(self->othersB, p);
                func_0023A668(self);
            }
        }
        {
            cGameModeMan* g = cGameModeHandler_pManager;
            g->iNextLocation = -1;
            g->iNextHeat = 3;
            g->iRestoreHeat = 3;
        }
        func_00416210(D_00536640, 9, 40);
        func_00416210(D_00536730, 9, 40);
        func_00416210(self->tbl[2], 9, 40);
        {
            cGameModeMan* g = cGameModeHandler_pManager;
            g->uiTimeLimit = 0;
            g->unk_80 = 0;
            g->bIsEventDone = 0;
            g->bIsTimedEvent = 0;
            g->unk_90 = 0;
            g->unk_94 = 0;
        }
        self->fC = 1;
        self->f8 = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A4F0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_00145750(void* iface);
extern "C" unsigned int GMrand(void);
extern "C" void func_0023C770(int* arr, unsigned int n);

extern "C" void func_0023A4F0(void* self)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int charA = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    int charB = func_00145750(race);
    int arr[10];
    arr[0] = GMrand() % 7 + 10;
    unsigned int cheatRoll = GMrand() % 6;
    arr[1] = cheatRoll + 10;
    if (arr[1] >= arr[0]) {
        arr[1] = cheatRoll + 11;
    }
    int n = 0;
    for (int i = 0; i < 10; i++) {
        if (i != charA && i != charB) {
            arr[n + 2] = i;
            n++;
        }
        if (n + 2 >= 10) {
            break;
        }
    }
    func_0023C770(arr, 10);
    *(int*)((char*)self + 0x10) = charA;
    *(int*)((char*)self + 0x14) = arr[0];
    *(int*)((char*)self + 0x18) = arr[1];
    *(int*)((char*)self + 0x1C) = arr[2];
    *(int*)((char*)self + 0x20) = arr[3];
    *(int*)((char*)self + 0x24) = arr[4];
    *(int*)((char*)self + 0x34) = arr[5];
    *(int*)((char*)self + 0x38) = arr[6];
    *(int*)((char*)self + 0x3C) = arr[7];
    *(int*)((char*)self + 0x50) = arr[8];
    *(int*)((char*)self + 0x54) = arr[9];
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A668);
#ifdef SKIP_ASM


// Game-mode handler base (cGameModeHandler): the handlers in D_00536668 are its subclasses.
class cGameModeHandler {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

// The race handler (cGMRaceHandler, vtable D_0047CF38, 0xD0 bytes).
struct cGMRaceHandler : public cGameModeHandler {
    int f4;                     // 0x04
    int f8;                     // 0x08
    int fC;                     // 0x0C
    int rows[3][6];             // 0x10 line-up per heat: the player's character, then the others
    int tbl[3][10];             // 0x58 times per heat and rider
};

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

int cBENewPlayerInterface_getRiderCharID(void* iface, int player);

extern "C" int func_00147410(void* iface, int player);

extern "C" void func_0023A668_A668(cGMRaceHandler* self) __asm__("func_0023A668");
extern "C" void func_0023A668_A668(cGMRaceHandler* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int id = cBENewPlayerInterface_getHumanRider(iface, 0);
    int ch = cBENewPlayerInterface_getRiderCharID(iface, id);
    int cheatChar = func_00147410(iface, id);
    int k = cGameModeHandler_pManager->iHeatNum - 1;
    if (k < 0) k = 0;
    for (int i = 0; i < 6; i++) {
        int charId = self->rows[k][i];
        if (charId >= 10) {
            cGameModeHandler_pManager->iCheatCharacters[i] = charId;
            charId = ch;
        } else {
            cGameModeHandler_pManager->iCheatCharacters[i] = 0;
        }
        cGameModeHandler_pManager->iCharacters[i] = charId;
        if (i == id) cGameModeHandler_pManager->iCheatCharacters[i] = cheatChar;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A760);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).
// Virtual calls in this unit go through these by hand.
#include "vtable.h"

// The game-mode manager (cGameModeMan, 0xA0 bytes; cGameModeMan_getGM creates it). cGameModeMan_instance owns it and
// D_004A2C70_A760 is the pointer the handlers read it through. iNumberAI is the prototype's name (assert
// `iRider < (pManager->iNumberAI+1)` in func_0023A760); mode indexes the handler table D_00536668.

// The race (seg/seg_1FBE38 layout): riders by being index at 0x28; each player holds its rider at +0x18.
struct sRacePlayer {
    char pad_0x0[0x18];
    cRider* rider;              // 0x18
};

struct sRace {
    char pad_0x0[0x8];
    unsigned int f8;            // 0x08
    int fC;                     // 0x0C race clock (ticks)
    char pad_0x10[0x10];
    int f20;                    // 0x20
    int f24;                    // 0x24
    cRider* riders[6];          // 0x28
    sRacePlayer* players[12];   // 0x40
    sRacePlayer* players2[3];   // 0x70
    int numPlayers;             // 0x7C
    int f80;                    // 0x80 last rider index
};

struct sGame {
    char pad_0x0[0xC];
    sRace* race;                // 0xC
    char pad_0x10[0x18];
    int* f28;                   // 0x28
};

// The game singleton (D_004A28A8_A760).
struct sApp {
    char pad_0x0[0x78];
    void* f78;                  // 0x78
    char pad_0x7C[0x8];
    sGame* game;                // 0x84
};

extern sApp* D_004A28A8_A760 __asm__("SSXApp");

// Race settings (D_00535BC8_A760; ai/ai's sRaceSettings). D_00535C04 / D_00535C11 / D_00535C12 are splat
// symbols for its fields at 0x3C / 0x49 / 0x4A.
struct sRaceSettings {
    char pad[0x49];
    signed char f49;            // 0x49
};


// Game-mode handler base (cGameModeHandler): the handlers in D_00536668 are its subclasses.
// The race handler (cGMRaceHandler, vtable D_0047CF38, 0xD0 bytes).
static inline sRace* GetRace()
{
    return D_004A28A8_A760->game->race;
}

static inline int Finished(cRider* rider)
{
    return rider->mFinishTime >= 0.0f;
}

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern int D_00536730[];

extern unsigned int D_00536640[];

extern int D_005366A8[];

extern "C" int cAI_isRaceFinished(void* race);

extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

extern "C" void func_00147D20(void* iface, int a1);

extern "C" void func_00147E18(void* iface, int a1);

extern "C" int cRider_estimateFinishTick(void* self);

extern "C" void func_00238BF8_A760(void* self, int n) __asm__("func_00238BF8");

static inline int vcall_23A760(char* obj, int slot)
{
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    return vt[slot].fn(obj + vt[slot].delta);
}

// cGMRaceHandler slot 7: records rider idx's heat time, ranks the heat and moves the heat state on.
extern "C" void func_0023A760(cGMRaceHandler* self, int idx)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int n = cBENewPlayerInterface_getHumanRider(player, 0);
    int heat = cGameModeHandler_pManager->iHeatNum - 1;
    sRace* rc = GetRace();
    if (cAI_isRaceFinished(rc) == 0 || vcall_23A760((char*)GetRace()->riders[idx] + 0x6C0, 9) == 0) {
        int tick = rc->riders[idx]->mFinishTick;
        if (GetRace()->riders[idx]->mDidNotFinish) {
            self->tbl[heat][idx] = 360000;
            D_005366A8[idx] = 1;
        } else {
            self->tbl[heat][idx] = tick;
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(rc) == 0) {
        return;
    }
    if (vcall_23A760((char*)GetRace()->riders[idx] + 0x6C0, 8) == 0) {
        return;
    }
    for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
        if (!Finished(rc->riders[i])) {
            self->tbl[heat][i] = cRider_estimateFinishTick(rc->riders[i]);
        }
        D_00536640[i] = self->tbl[heat][i];
    }
    func_00238BF8_A760(cGameModeHandler_pManager, 10);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iRestoreHeat = gm->iNextHeat;
    if (D_00536730[n] < 3) {
        gm->iNextHeat = gm->iHeatNum + 1;
        GetRace()->riders[idx]->unk_100 = 1;
        if (gm->iHeatNum == 3) {
            gm->bIsEventDone = 1;
            gm->iNextHeat = 3;
        }
        if (cBENewRaceInterface_newRace.cGamePath == 0) {
            if (D_00536730[n] == 0) {
                if ((cGameModeHandler_pManager->iHeatNum < 2 && D_00536640[1] - D_00536640[0] > 600) ||
                    (cGameModeHandler_pManager->iHeatNum == 2 && D_00536640[1] - D_00536640[0] > 300) ||
                    (cGameModeHandler_pManager->iHeatNum == 3 && D_00536640[1] - D_00536640[0] > 180)) {
                    func_00147D20(player, 1);
                }
            }
            if (cGameModeHandler_pManager->iHeatNum == 3) {
                cGameModeHandler_pManager->iNextHeat = 0;
                cGameModeHandler_pManager->iRestoreHeat = 0;
            }
        }
    } else {
        gm->iNextHeat = gm->iHeatNum;
        self->f8 = 1;
        self->fC = 1;
        GetRace()->riders[idx]->unk_100 = 0;
        if (cBENewRaceInterface_newRace.cGamePath == 0) {
            if (cGameModeHandler_pManager->iHeatNum < 2 ||
                (cGameModeHandler_pManager->iHeatNum == 2 && D_00536730[n] >= 4) ||
                (cGameModeHandler_pManager->iHeatNum == 3 && D_00536730[n] == 5)) {
                func_00147E18(player, 1);
            }
        }
    }
    if (cGameModeHandler_pManager->iHeatNum == 3) {
        cGameModeHandler_pManager->bAreResultsLocked = 1;
    }
    int place = D_00536730[n];
    if (place == 0)
        GetRace()->players[0]->rider->unk_2F8 = 0.699999988079071f;
    else if (place == 1)
        GetRace()->players[0]->rider->unk_2F8 = 0.3499999940395355f;
    else
        GetRace()->players[0]->rider->unk_2F8 = 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC10__FPv);
#ifdef SKIP_ASM
void func_0023AC10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC18);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023AC18(void* self, cGameModeStream* s)
{
    s->v01(self, 208);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC50);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023AC50(void* self, cGameModeStream* s)
{
    s->v02(self, 208);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC88);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* iface, int player);
extern "C" int func_001474C8(void* iface, int player);
extern "C" void func_0023B0A8(void* self);
extern int D_00535C04[];

struct sGM_23AC88 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23AC88 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023AC88(void* self)
{
    int one = 1;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iCurrentGamePath = 2;
    gm->unk_94 = 0;
    gm->unk_90 = 0;
    gm->bAreResultsLocked = 0;
    gm->bIsLastHeat = one;
    gm->bIsTimedEvent = 0;
    gm->iCurrentGameEvent = 0;
    gm->iHeatNum = 3;
    gm->iNextHeat = 3;
    gm->iRestoreHeat = 3;
    gm->iNextLocation = -1;
    gm->iNumberAI = 4;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        cGameModeHandler_pManager->iNumberAI = D_00535C04[0];
    }
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->iSimAI = 0;
    gm2->unk_80 = 0;
    gm2->uiTimeLimit = 0;
    gm2->uiPointTarget = 0;
    gm2->bHasPointTarget = 0;
    func_00416210((char*)self + 4, 9, 40);
    sVEntry_23AC88* e = &(*(sVEntry_23AC88**)self)[5];
    e->fn((char*)self + e->delta);
    cGameModeHandler_pManager->iCharacters[0] = cBENewPlayerInterface_getPlayerCharID(player, 0);
    cGameModeHandler_pManager->iCharacters[1] = cBENewPlayerInterface_getPlayerCharID(player, 1);
    cGameModeHandler_pManager->iCheatCharacters[0] = func_001474C8(player, 0);
    cGameModeHandler_pManager->iCheatCharacters[1] = func_001474C8(player, 1);
    if (cGameModeHandler_pManager->bIsEventDone == one) {
        func_0023B0A8(self);
    }
    cGameModeHandler_pManager->bIsEventDone = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AE00);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_00536730[];
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cRider_estimateFinishTick(void* self);
extern "C" void func_00238BF8(void* self, int n);

struct sVEnt_23AE00 { short delta; short index; int (*fn)(void*); };

struct sRider_23AE00 {
    char pad000[0x100];
    int f100;
    char pad104[0x36C];
    float f470;
    int pad474;
    int f478;
    int pad47C;
    int f480;
    char pad484[0x23C];
    sVEnt_23AE00* subvt;
};

struct sRace_23AE00 {
    char pad00[0x28];
    sRider_23AE00* riders[8];
};

struct sSelf_23AE00 {
    int f0;
    int times[8];
};

static inline sRace_23AE00* race_23AE00()
{
    return (sRace_23AE00*)SSXApp->Game->AI;
}

static inline int subActive_23AE00(char* obj)
{
    sVEnt_23AE00* vt = *(sVEnt_23AE00**)obj;
    return vt[9].fn(obj + vt[9].delta);
}

static inline int finished_23AE00(sRider_23AE00* r)
{
    return r->f470 >= 0.0f;
}

extern "C" void func_0023AE00(sSelf_23AE00* self, int idx)
{
    sRace_23AE00* rc = race_23AE00();
    if (cAI_isRaceFinished(rc) == 0 || subActive_23AE00((char*)rc->riders[idx] + 0x6C0) == 0) {
        if (race_23AE00()->riders[idx]->f480) {
            D_00536640[idx] = self->times[idx] = 360000;
            D_005366A8[idx] = 1;
        } else {
            int v = rc->riders[idx]->f478;
            self->times[idx] = v;
            D_00536640[idx] = v;
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(rc)) {
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 2; i++) {
            if (!finished_23AE00(rc->riders[i]) && i != idx) {
                self->times[i] = cRider_estimateFinishTick(rc->riders[i]);
            }
            D_00536640[i] = self->times[i];
        }
        func_00238BF8((char*)cGameModeHandler_pManager, 10);
        if (D_00536730[idx] >= 3) {
            race_23AE00()->riders[idx]->f100 = 0;
        }
        cGameModeHandler_pManager->bAreResultsLocked = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B038);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023B038(void* self, cGameModeStream* s)
{
    s->v01(self, 44);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B070);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023B070(void* self, cGameModeStream* s)
{
    s->v02(self, 44);
}
#endif

INCLUDE_ASM("main/gamemode", func_0023B0A8);

//100%
INCLUDE_ASM("main/gamemode", func_0023B170);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

struct sGM_23B170 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23B170 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023B170(void* self)
{
    int one = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = one;
    gm->bIsLastHeat = one;
    gm->iNumberAI = 0;
    gm->iSimAI = 0;
    func_00416210(gm->iCharacters, -1, 40);
    int p = cBENewPlayerInterface_getHumanRider(iface, 0);
    cGameModeHandler_pManager->iCharacters[p] = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
    sVEntry_23B170* e = &(*(sVEntry_23B170**)self)[4];
    e->fn((char*)self + e->delta);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->iNextHeat = one;
    gm2->iRestoreHeat = one;
    gm2->bAreResultsLocked = one;
    gm2->uiTimeLimit = 0;
    gm2->uiPointTarget = 0;
    gm2->unk_80 = 0;
    gm2->bIsEventDone = 0;
    gm2->bIsTimedEvent = 0;
    gm2->bHasPointTarget = 0;
    gm2->unk_90 = 0;
    gm2->unk_94 = 0;
    gm2->iNextLocation = -1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B268);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_001455D0(void* iface, int mode, int a2);
extern "C" int func_001558F8(void* iface, int a1, int a2, int a3);
extern "C" void* func_00416210(void* dst, int c, int n);
extern signed char D_00535C12[];

struct sGM_23B268 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23B268 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMode_23B268 {
    sVEntry_23B268* vt;
    int f4;
    short f8;
    short fA;
};

extern "C" void func_0023B268(void* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    int mode = D_00535C12[0];
    gm->iNumberAI = 0;
    gm->iCurrentGameEvent = mode;
    gm->iSimAI = 0;
    gm->bHasPointTarget = 0;
    switch (mode) {
    case 6:
        gm->iNextLocation = 0;
        gm->iLocation = 14;
        break;
    case 7:
        gm->iLocation = 15;
        gm->iNextLocation = 3;
        break;
    case 8:
        gm->iLocation = 16;
        gm->iNextLocation = 4;
        break;
    }
    int r = func_001558F8(score, 0, cGameModeHandler_pManager->iLocation, cGameModeHandler_pManager->iCurrentGameEvent);
    *(short*)((char*)self + 0xA) = 0;
    if (r == 1 || r == 2) {
        *(short*)((char*)self + 0xA) = 2;
    } else if (r == 3) {
        *(short*)((char*)self + 0xA) = 1;
    }
    int t = func_001455D0(race, cGameModeHandler_pManager->iCurrentGameEvent, *(short*)((char*)self + 0xA));
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    *(int*)((char*)self + 4) = t * 60;
    gm2->bIsTimedEvent = 1;
    gm2->uiTimeLimit = *(int*)((char*)self + 4);
    gm2->uiPointTarget = *(int*)((char*)self + 4);
    gm2->iNextHeat = 2;
    gm2->iRestoreHeat = 2;
    sVEntry_23B268* vt = *(sVEntry_23B268**)self;
    vt[5].fn((char*)self + vt[5].delta);
    func_00416210(cGameModeHandler_pManager->iCharacters, -1, 40);
    int p = cBENewPlayerInterface_getHumanRider(player, 0);
    cGameModeHandler_pManager->iCharacters[p] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    cGameModeMan* gm3 = cGameModeHandler_pManager;
    gm3->unk_80 = 0;
    gm3->bIsEventDone = 0;
    gm3->unk_90 = 0;
    gm3->unk_94 = 0;
    gm3->bIsLastHeat = 0;
    gm3->bAreResultsLocked = 0;
    *(short*)((char*)self + 8) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B468);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int func_001455D0(void* iface, int mode, int a2);
extern int D_00536730[];
extern unsigned int D_00536640[];
extern int D_005366A8[];

struct sGM_23B468 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

static inline char* rider_23B468(char* g, int off)
{
    return *(char**)(*(char**)(*(char**)(g + 0x84) + 0xC) + off + 0x28);
}

extern "C" void func_0023B468(void* self, int p)
{
    int one = 1;
    int off = p << 2;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    char* g = (char*)SSXApp;
    cGameModeMan* gm = cGameModeHandler_pManager;
    unsigned int* t = &D_00536640[p];
    *t = *(unsigned int*)(rider_23B468(g, off) + 0x478);
    gm->bAreResultsLocked = one;
    char* r = rider_23B468(g, off);
    if (*(int*)(r + 0x480) != 0) {
        *t = 0x57E40;
        *(int*)(rider_23B468(g, off) + 0x100) = 0;
        gm->iNextHeat = one;
        D_005366A8[p] = one;
    } else if (*(unsigned int*)((char*)self + 4) < *t) {
        *(int*)(r + 0x100) = 0;
        gm->iNextHeat = one;
        D_005366A8[p] = 0;
    } else {
        *(int*)(r + 0x100) = one;
        gm->bIsEventDone = one;
        unsigned int a = func_001455D0(race, gm->iCurrentGameEvent, 2) * 60;
        unsigned int b = func_001455D0(race, cGameModeHandler_pManager->iCurrentGameEvent, 1) * 60;
        if (a >= *t) {
            D_00536730[p] = 0;
        } else if (b >= *t) {
            D_00536730[p] = one;
        } else {
            D_00536730[p] = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B5F8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int func_00145668(void* iface, int a1, int a2, int a3);
extern "C" float cRiderMetrix_evSplitTime(void* self, int value);
extern signed char D_00535C12[];

extern "C" int func_0023B5F8(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int state = cBENewRaceInterface_getLocationID(iface);
    int ok = 0;
    int t = state >= 17;
    if (t) {
        ok = state;
        ok = ok < 22;
    }
    if (ok) {
        int d = func_00145668(iface, D_00535C12[0], *(short*)((char*)self + 0xA), *(unsigned short*)((char*)self + 8));
        void* clk = SSXApp->Game->AI;
        int secs = (int)((float)*(int*)((char*)clk + 0xC) * 0.01666666753590107f);
        if (d != 0) {
            cRiderMetrix_evSplitTime(*(void**)(*(char**)((char*)clk + 0x28) + 0x790), secs - d);
        }
        (*(unsigned short*)((char*)self + 8))++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B6C0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_00145750(void* iface);
extern "C" int func_00147410(void* iface, int player);
extern "C" void* func_00416210(void* dst, int c, int n);
extern signed char D_00535C11[];

struct sGM_23B6C0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23B6C0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023B6C0(void* self)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int one = 1;
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = one;
    gm->iNumberAI = one;
    gm->iSimAI = 0;
    gm->iLocation = -1;
    gm->iNextHeat = one;
    gm->iRestoreHeat = one;
    gm->uiTimeLimit = 0;
    gm->uiPointTarget = 0;
    gm->unk_80 = 0;
    gm->bIsEventDone = 0;
    gm->bIsTimedEvent = 0;
    gm->bHasPointTarget = 0;
    gm->unk_90 = 0;
    gm->unk_94 = 0;
    gm->iNextLocation = -1;
    gm->bIsLastHeat = one;
    gm->bAreResultsLocked = 0;
    sVEntry_23B6C0* vt = *(sVEntry_23B6C0**)self;
    vt[5].fn((char*)self + vt[5].delta);
    if (D_00535C11[0] == 2) {
        cGameModeHandler_pManager->iNumberAI = 0;
        int p0 = cBENewPlayerInterface_getHumanRider(player, 0);
        int p1 = cBENewPlayerInterface_getHumanRider(player, 1);
        cGameModeHandler_pManager->iCharacters[p0] = cBENewPlayerInterface_getRiderCharID(player, p0);
        cGameModeHandler_pManager->iCheatCharacters[p0] = func_00147410(player, p0);
        cGameModeHandler_pManager->iCharacters[p1] = cBENewPlayerInterface_getRiderCharID(player, p1);
        cGameModeHandler_pManager->iCheatCharacters[p1] = func_00147410(player, p1);
    } else {
        func_00416210(cGameModeHandler_pManager->iCharacters, -1, 40);
        int p = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeHandler_pManager->iCharacters[p] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
        cGameModeHandler_pManager->iCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = func_00145750(race);
        cGameModeHandler_pManager->iCheatCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B8C8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishTick(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_00238BF8(void* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_00536730[];
extern signed char D_00535C11[];


class cRiderState_23B8C8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual int isActive();
    virtual int isFinished();
};

struct sClk_23B8C8 {
    char pad_0x0[0x28];
    char* riders[1];    // 0x28
};

static inline char* Rider_23B8C8(int i)
{
    return ((sClk_23B8C8*)SSXApp->Game->AI)->riders[i];
}

extern "C" void func_0023B8C8(void* self, int idx)
{
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0 ||
        ((cRiderState_23B8C8*)(Rider_23B8C8(idx) + 0x6C0))->isFinished() == 0) {
        char* r = Rider_23B8C8(idx);
        if (*(int*)(r + 0x480) != 0) {
            D_00536640[idx] = 0x57E40;
            D_005366A8[idx] = 1;
        } else {
            D_00536640[idx] = *(int*)(r + 0x478);
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0) {
        return;
    }
    if (((cRiderState_23B8C8*)(Rider_23B8C8(idx) + 0x6C0))->isActive() == 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        func_00238BF8((char*)cGameModeHandler_pManager, 10);
        cGameModeHandler_pManager->bAreResultsLocked = 1;
        int w = 1;
        if (D_00536640[0] < D_00536640[1]) {
            w = 0;
        }
        if (idx == w) {
            return;
        }
    } else {
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            char* r = Rider_23B8C8(i);
            int done = *(float*)(r + 0x470) >= 0.0f;
            if (!done) {
                D_00536640[i] = cRider_estimateFinishTick(r);
            }
        }
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        int n = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bAreResultsLocked = 1;
        func_00238BF8(gm, 10);
        if (D_00536730[n] == 0 && *(int*)(Rider_23B8C8(idx) + 0x480) == 0) {
            *(int*)(Rider_23B8C8(idx) + 0x100) = 1;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            gm2->bIsEventDone = 1;
            gm2->iRestoreHeat = gm2->iNextHeat;
            gm2->iNextHeat = 0;
            return;
        }
    }
    *(int*)(Rider_23B8C8(idx) + 0x100) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023BB98);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_00145750(void* iface);
extern "C" int func_00147410(void* iface, int player);
extern "C" int func_001454F8(void* iface, int a1);
extern "C" void* func_00416210(void* dst, int c, int n);
extern signed char D_00535C11[];

struct sGM_23BB98 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23BB98 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023BB98(void* self)
{
    int one = 1;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iLocation = -1;
    gm->iRestoreHeat = one;
    gm->iHeatNum = one;
    gm->iNumberAI = one;
    gm->iSimAI = 0;
    gm->iNextHeat = one;
    gm->iNextLocation = -1;
    int t = func_001454F8(race, 1);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->bAreResultsLocked = 0;
    gm2->uiPointTarget = 0;
    gm2->uiTimeLimit = t * 60;
    gm2->unk_80 = 0;
    gm2->bIsEventDone = 0;
    gm2->bIsTimedEvent = one;
    gm2->bHasPointTarget = 0;
    gm2->unk_90 = 0;
    gm2->unk_94 = 0;
    gm2->bIsLastHeat = one;
    sVEntry_23BB98* vt = *(sVEntry_23BB98**)self;
    vt[4].fn((char*)self + vt[4].delta);
    if (D_00535C11[0] == 2) {
        cGameModeHandler_pManager->iNumberAI = 0;
        int p0 = cBENewPlayerInterface_getHumanRider(player, 0);
        int p1 = cBENewPlayerInterface_getHumanRider(player, 1);
        cGameModeHandler_pManager->iCharacters[p0] = cBENewPlayerInterface_getRiderCharID(player, p0);
        cGameModeHandler_pManager->iCheatCharacters[p0] = func_00147410(player, p0);
        cGameModeHandler_pManager->iCharacters[p1] = cBENewPlayerInterface_getRiderCharID(player, p1);
        cGameModeHandler_pManager->iCheatCharacters[p1] = func_00147410(player, p1);
    } else {
        func_00416210(cGameModeHandler_pManager->iCharacters, -1, 40);
        int p = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeHandler_pManager->iCharacters[p] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
        cGameModeHandler_pManager->iCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = func_00145750(race);
        cGameModeHandler_pManager->iCheatCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023BDB8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishScore(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_00238B70(void* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_00536730[];
extern signed char D_00535C11[];


struct sStats_23BDB8 {
    int v[0x9C / 4];
    int time;           // 0x9C
    int w[3];
};

struct sClk_23BDB8 {
    char pad_0x0[0x28];
    char* riders[1];    // 0x28
};

static inline char* Rider_23BDB8(int i)
{
    return ((sClk_23BDB8*)SSXApp->Game->AI)->riders[i];
}

extern "C" void func_0023BDB8(void* self, int idx)
{
    char* r = Rider_23BDB8(idx);
    if (*(int*)(r + 0x480) != 0) {
        D_00536640[idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        sStats_23BDB8 st = *(sStats_23BDB8*)(*(char**)(r + 0x790) + 0xFC);
        D_005366A8[idx] = 0;
        D_00536640[idx] = st.time;
    }
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        func_00238B70((char*)cGameModeHandler_pManager, 2);
        cGameModeHandler_pManager->bAreResultsLocked = 1;
        int w = 1;
        if (D_00536640[0] > D_00536640[1]) {
            w = 0;
        }
        if (idx == w) {
            return;
        }
        *(int*)(Rider_23BDB8(idx) + 0x100) = 0;
    } else {
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            char* r = Rider_23BDB8(i);
            int done = *(float*)(r + 0x470) >= 0.0f;
            if (!done) {
                D_00536640[i] = cRider_estimateFinishScore(r);
            }
        }
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        int n = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bAreResultsLocked = 1;
        func_00238B70(gm, 2);
        if (D_00536730[n] == 0 && *(int*)(Rider_23BDB8(idx) + 0x480) == 0) {
            *(int*)(Rider_23BDB8(idx) + 0x100) = 1;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            gm2->bIsEventDone = 1;
            gm2->iRestoreHeat = gm2->iNextHeat;
            gm2->iNextHeat = 0;
            return;
        }
        *(int*)(Rider_23BDB8(idx) + 0x100) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023C0D0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int func_001455D0(void* iface, int mode, int a2);
extern "C" int func_00145600(void* iface, int mode, int a2);
extern "C" int func_001558F8(void* iface, int a1, int a2, int a3);
extern "C" void* func_00416210(void* dst, int c, int n);
extern signed char D_00535C12[];

struct sGM_23C0D0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23C0D0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023C0D0(void* self)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    int mode = D_00535C12[0];
    gm->iNumberAI = 0;
    gm->iCurrentGameEvent = mode;
    gm->iSimAI = 0;
    switch (mode) {
    case 9:
        gm->iLocation = 14;
        gm->iNextLocation = 5;
        break;
    case 10:
        gm->iLocation = 15;
        gm->iNextLocation = 6;
        break;
    case 11:
        gm->iLocation = 16;
        gm->iNextLocation = 4;
        break;
    }
    unsigned int r = (unsigned int)func_001558F8(score, 0, cGameModeHandler_pManager->iLocation, cGameModeHandler_pManager->iCurrentGameEvent);
    *(short*)((char*)self + 0xA) = 0;
    if (r < 3) {
        *(short*)((char*)self + 0xA) = 2;
    } else if (r == 3) {
        *(short*)((char*)self + 0xA) = 1;
    }
    int t = func_001455D0(race, cGameModeHandler_pManager->iCurrentGameEvent, *(short*)((char*)self + 0xA)) * 100;
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    *(int*)((char*)self + 4) = t;
    gm2->uiPointTarget = t;
    int u = func_00145600(race, gm2->iCurrentGameEvent, *(short*)((char*)self + 0xA));
    cGameModeMan* gm3 = cGameModeHandler_pManager;
    gm3->iNextHeat = 2;
    gm3->iRestoreHeat = 2;
    gm3->uiTimeLimit = u * 60;
    sVEntry_23C0D0* vt = *(sVEntry_23C0D0**)self;
    vt[4].fn((char*)self + vt[4].delta);
    func_00416210(cGameModeHandler_pManager->iCharacters, -1, 40);
    int p = cBENewPlayerInterface_getHumanRider(player, 0);
    cGameModeHandler_pManager->iCharacters[p] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    cGameModeMan* gm4 = cGameModeHandler_pManager;
    gm4->unk_80 = 0;
    gm4->bIsEventDone = 0;
    gm4->bIsTimedEvent = 1;
    gm4->bHasPointTarget = 1;
    gm4->unk_90 = 0;
    gm4->unk_94 = 0;
    gm4->bIsLastHeat = 0;
    gm4->bAreResultsLocked = 0;
    *(short*)((char*)self + 8) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023C2D8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int func_001455D0(void* iface, int mode, int a2);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_00536730[];

struct sGM_23C2D8 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sStats_23C2D8 {
    int v[0x9C / 4];
    int time;           // 0x9C
    int w[3];
};

struct sClk_23C2D8 {
    char pad_0x0[0x28];
    char* riders[1];    // 0x28
};

extern "C" void func_0023C2D8(void* self, int idx)
{
    sClk_23C2D8* clk = (sClk_23C2D8*)SSXApp->Game->AI;
    sStats_23C2D8 st = *(sStats_23C2D8*)(*(char**)(clk->riders[idx] + 0x790) + 0xFC);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    char* r = ((sClk_23C2D8*)SSXApp->Game->AI)->riders[idx];
    if (*(int*)(r + 0x480) != 0) {
        D_00536640[idx] = 0;
        cGameModeHandler_pManager->iNextHeat = 1;
        *(int*)(((sClk_23C2D8*)SSXApp->Game->AI)->riders[idx] + 0x100) = 0;
        D_005366A8[idx] = 1;
    } else {
        cGameModeMan* gm = cGameModeHandler_pManager;
        if (st.time < (int)gm->uiPointTarget) {
            *(int*)(r + 0x100) = 0;
            D_00536640[idx] = st.time;
            gm->iNextHeat = 1;
            D_005366A8[idx] = 0;
        } else {
            int a = func_001455D0(race, gm->iCurrentGameEvent, 2) * 100;
            int b = func_001455D0(race, cGameModeHandler_pManager->iCurrentGameEvent, 1) * 100;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            *(int*)(((sClk_23C2D8*)SSXApp->Game->AI)->riders[idx] + 0x100) = 1;
            D_00536640[idx] = st.time;
            gm2->bIsEventDone = 1;
            if (st.time >= a) {
                D_00536730[idx] = 0;
            } else if (st.time >= b) {
                D_00536730[idx] = 1;
            } else {
                D_00536730[idx] = 2;
            }
        }
    }
    cGameModeHandler_pManager->bAreResultsLocked = 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023C560);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int func_00145668(void* iface, int a1, int a2, int a3);
extern "C" float cRiderMetrix_evSplitPoints(void* self, int value);
extern signed char D_00535C12[];

extern "C" int func_0023C560(void* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int state = cBENewRaceInterface_getLocationID(iface);
    int ok = 0;
    int t = state >= 17;
    if (t) {
        ok = state;
        ok = ok < 22;
    }
    if (ok) {
        int d = func_00145668(iface, D_00535C12[0], *(short*)((char*)self + 0xA), *(unsigned short*)((char*)self + 8)) * 100;
        void* m = *(void**)(*(char**)((char*)SSXApp->Game->AI + 0x28) + 0x790);
        int v = *(int*)((char*)m + 0x198);
        if (d != 0) {
            cRiderMetrix_evSplitPoints(m, v - d);
        }
        (*(unsigned short*)((char*)self + 8))++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023C618);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_0023C770(int* arr, unsigned int n);
extern int D_00535C04[];

struct sGM_23C618 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int a18[10];
    int a40[10];
    int f68;
    int f6C;
    int f70;
    int f74;
    int f78;
    int f7C;
    int f80;
    int f84;
    int f88;
    int f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
};

struct sVEntry_23C618 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0023C618(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    gm->bIsLastHeat = 1;
    gm->iNumberAI = D_00535C04[0];
    gm->iSimAI = 0;
    int p = cBENewPlayerInterface_getHumanRider(player, 0);
    int ch = cBENewPlayerInterface_getRiderCharID(player, p);
    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    func_0023C770(arr, 10);
    int j = 0;
    int* src = arr;
    for (; j < 10; j++) {
        if (j != p) {
            int v = *src;
            if (v == ch) {
                src++;
                v = *src;
            }
            cGameModeHandler_pManager->iCharacters[j] = v;
            src++;
        } else {
            cGameModeHandler_pManager->iCharacters[j] = ch;
        }
    }
    sVEntry_23C618* e = &(*(sVEntry_23C618**)self)[4];
    e->fn((char*)self + e->delta);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->iNextHeat = 1;
    gm2->uiTimeLimit = 0;
    gm2->uiPointTarget = 0;
    gm2->unk_80 = 0;
    gm2->bIsEventDone = 0;
    gm2->bIsTimedEvent = 0;
    gm2->bHasPointTarget = 0;
    gm2->unk_90 = 0;
    gm2->iNextLocation = -1;
    gm2->iRestoreHeat = 1;
    gm2->unk_94 = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023C770);
#ifdef SKIP_ASM
extern "C" unsigned int GMrand(void);

extern "C" void func_0023C770(int* arr, unsigned int n)
{
    int i;
    for (i = 0; i < 25; i++) {
        unsigned int firstIndex = GMrand() % n;
        unsigned int secondIndex = GMrand() % n;
        int swapValue = arr[firstIndex];
        arr[firstIndex] = arr[secondIndex];
        arr[secondIndex] = swapValue;
    }
}
#endif

