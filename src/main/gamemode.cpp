#include "vtable.h"
#include "common.h"
#include "main/gamemodehandler.h"
#include "be/beinterface.h"
#include "ai/ridermetrix.h"
#include "ai/player.h"
#include "be/beintnewrace.h"
#include "be/beintreward.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"
#include "replay/bxreplayframe.h"
#include "replay/frameptr.h"

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_getGM);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern cGameModeHandler* cGameModeMan_aHandler[];
extern char str_GameModeMan[];
extern char str_GM_Handler[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* memset(void* dst, int c, int n);
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern void* cGMPointHandler_vtbl[];
extern void* cGMRivalPointHandler_vtbl[];
extern void* cGMRivalTimeHandler_vtbl[];
extern void* cGMTimeHandler_vtbl[];
extern void* cGMFreerideHandler_vtbl[];
extern void* cGMMPSlopeHandler_vtbl[];
extern void* cGMMPRaceHandler_vtbl[];
extern void* cGMRaceHandler_vtbl[];
extern void* cGMBigAirHandler_vtbl[];

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
 * The manager is "GameModeMan" (0xA0 bytes); the handlers ("GM Handler") fill table cGameModeMan_aHandler, all but slot
 * 3. Some creations are retried up to three times while the result is 0, and slots 5 and 6 are checked twice.
 * A new manager starts with no event (-1), heat 0 and handler 0.
 *
 * @return the manager (cGameModeMan_instance)
 */
extern "C" int* cGameModeMan_getGM(void)
{
    if (cGameModeMan_instance == 0) {
        cGameModeMan_instance = (cGameModeMan*)((int*)cMemMan_alloc(160, str_GameModeMan, 0, 0));
        memset(cGameModeMan_aHandler, 0, 40);
        if (cGameModeMan_aHandler[0] == 0) {
            cGameModeMan_aHandler[0] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_7C(cGMBigAirHandler_vtbl);
            if (cGameModeMan_aHandler[0] == 0) {
                cGameModeMan_aHandler[0] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_7C(cGMBigAirHandler_vtbl);
                if (cGameModeMan_aHandler[0] == 0) {
                    cGameModeMan_aHandler[0] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_7C(cGMBigAirHandler_vtbl);
                }
            }
        }
        if (cGameModeMan_aHandler[1] == 0) {
            cGameModeMan_aHandler[1] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_D0(cGMRaceHandler_vtbl);
        }
        if (cGameModeMan_aHandler[2] == 0) {
            cGameModeMan_aHandler[2] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_4(cGMFreerideHandler_vtbl);
        }
        if (cGameModeMan_aHandler[5] == 0) {
            cGameModeMan_aHandler[5] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_4(cGMRivalTimeHandler_vtbl);
        }
        if (cGameModeMan_aHandler[4] == 0) {
            cGameModeMan_aHandler[4] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMTimeHandler_vtbl);
            if (cGameModeMan_aHandler[4] == 0) {
                cGameModeMan_aHandler[4] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMTimeHandler_vtbl);
                if (cGameModeMan_aHandler[4] == 0) {
                    cGameModeMan_aHandler[4] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMTimeHandler_vtbl);
                }
            }
        }
        if (cGameModeMan_aHandler[6] == 0) {
            cGameModeMan_aHandler[6] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_4(cGMRivalPointHandler_vtbl);
        }
        if (cGameModeMan_aHandler[7] == 0) {
            cGameModeMan_aHandler[7] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMPointHandler_vtbl);
            if (cGameModeMan_aHandler[7] == 0) {
                cGameModeMan_aHandler[7] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMPointHandler_vtbl);
                if (cGameModeMan_aHandler[7] == 0) {
                    cGameModeMan_aHandler[7] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_C(cGMPointHandler_vtbl);
                }
            }
        }
        if (cGameModeMan_aHandler[8] == 0) {
            cGameModeMan_aHandler[8] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_2C(cGMMPRaceHandler_vtbl);
        }
        if (cGameModeMan_aHandler[9] == 0) {
            cGameModeMan_aHandler[9] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_8(cGMMPSlopeHandler_vtbl);
            if (cGameModeMan_aHandler[9] == 0) {
                cGameModeMan_aHandler[9] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_8(cGMMPSlopeHandler_vtbl);
                if (cGameModeMan_aHandler[9] == 0) {
                    cGameModeMan_aHandler[9] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_8(cGMMPSlopeHandler_vtbl);
                }
            }
        }
        if (cGameModeMan_aHandler[5] == 0) {
            cGameModeMan_aHandler[5] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_4(cGMRivalTimeHandler_vtbl);
        }
        if (cGameModeMan_aHandler[6] == 0) {
            cGameModeMan_aHandler[6] = (cGameModeHandler*)new (str_GM_Handler, 0, 0) sGMMode_4(cGMRivalPointHandler_vtbl);
        }
        cGameModeMan* gm = cGameModeMan_instance;
        cGameModeHandler_pManager = (cGameModeMan*)gm;
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
void operator_delete(int*);
extern cGameModeHandler* cGameModeMan_aHandler[];

/**
 * Destroys the game mode manager: deletes the ten mode handlers and the singleton.
 *
 * Deletes each handler in cGameModeMan_aHandler (On Tour aHandler), frees cGameModeMan_instance and sets it to null.
 */
extern "C" void cGameModeMan_purgeGM(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        delete cGameModeMan_aHandler[i];
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
 * Purges the current handler (its slot 3, cGameModeHandler_purge), stores @p type in iCurrentGameEvent and the new race's
 * cGamePath in iCurrentGamePath. Multiplayer (kGAMEPATH_MULTIPLAYER): the freestyle events (kGAMEEVENT_SLOPESTYLE,
 * SUPERPIPE, BIGAIR) -> kHANDLER_MPSLOPE, the rival events -> kHANDLER_RIVALTIME / RIVALPOINT, others kHANDLER_MPRACE.
 * Offline: freestyle events -> kHANDLER_BIGAIR, kGAMEEVENT_RACE -> kHANDLER_RACE, FREERIDE -> kHANDLER_FREERIDE, the
 * TIME_PEAK events -> kHANDLER_TIME, the rival events as online, the POINTS_PEAK events -> kHANDLER_POINT; other types
 * keep the purged handler index. Then sets bIsEventDone and calls cGameModeMan_restartHeat().
 *
 * @param type  the BE game event (newRace.cGameEvent)
 */
void cGameModeMan::initGameMode(unsigned int type)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cGameModeHandler* gm = cGameModeMan_aHandler[iCurrModeHandler];
    gm->purge();
    iCurrentGameEvent = (int)type;
    int net = cBENewRaceInterface_newRace.cGamePath;
    iCurrentGamePath = net;
    if (net == kGAMEPATH_MULTIPLAYER) {
        switch (type) {
        case kGAMEEVENT_RACE: iCurrModeHandler = kHANDLER_MPRACE; break;
        case kGAMEEVENT_SLOPESTYLE: iCurrModeHandler = kHANDLER_MPSLOPE; break;
        case kGAMEEVENT_BIGAIR: iCurrModeHandler = kHANDLER_MPSLOPE; break;
        case kGAMEEVENT_SUPERPIPE: iCurrModeHandler = kHANDLER_MPSLOPE; break;
        case kGAMEEVENT_RIVALTIME: iCurrModeHandler = kHANDLER_RIVALTIME; break;
        case kGAMEEVENT_RIVALPOINTS: iCurrModeHandler = kHANDLER_RIVALPOINT; break;
        default: iCurrModeHandler = kHANDLER_MPRACE; break;
        }
    } else {
        switch (type) {
        case kGAMEEVENT_SLOPESTYLE:
        case kGAMEEVENT_SUPERPIPE:
        case kGAMEEVENT_BIGAIR:
            iCurrModeHandler = kHANDLER_BIGAIR;
            break;
        case kGAMEEVENT_RACE: iCurrModeHandler = kHANDLER_RACE; break;
        case kGAMEEVENT_FREERIDE: iCurrModeHandler = kHANDLER_FREERIDE; break;
        case kGAMEEVENT_RIVALTIME: iCurrModeHandler = kHANDLER_RIVALTIME; break;
        case kGAMEEVENT_TIME_PEAK1: iCurrModeHandler = kHANDLER_TIME; break;
        case kGAMEEVENT_TIME_PEAK2: iCurrModeHandler = kHANDLER_TIME; break;
        case kGAMEEVENT_TIME_PEAK3: iCurrModeHandler = kHANDLER_TIME; break;
        case kGAMEEVENT_RIVALPOINTS: iCurrModeHandler = kHANDLER_RIVALPOINT; break;
        case kGAMEEVENT_POINTS_PEAK1:
        case kGAMEEVENT_POINTS_PEAK2:
        case kGAMEEVENT_POINTS_PEAK3:
            iCurrModeHandler = kHANDLER_POINT;
            break;
        }
    }
    bIsEventDone = true;
    restartHeat();
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_002382B8);
#ifdef SKIP_ASM
extern int cGameModeMan_iRanking[];

/**
 * Sets entry @p i of the rank table cGameModeMan_iRanking to -1.
 *
 * @param i  the rider index
 */
extern "C" void func_002382B8(cGameModeMan* self, int i)
{
    cGameModeMan_iRanking[i] = -1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_restartHeat);
#ifdef SKIP_ASM

extern cGameModeHandler* cGameModeMan_aHandler[];
extern int cGameModeMan_bGiveUp[];

/**
 * Restarts the current heat: calls the current handler's virtual slot 2 and clears the six riders' cGameModeMan_bGiveUp
 * values.
 */
void cGameModeMan::restartHeat()
{
    int i;
    cGameModeHandler* obj = cGameModeMan_aHandler[iCurrModeHandler];
    obj->init();
    for (i = 5; i >= 0; i--) {
        cGameModeMan_bGiveUp[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_restorePreviousHeat__FPv);
#ifdef SKIP_ASM
/**
 * Takes back the last heat decision: copies iRestoreHeat (+0x74) into iNextHeat (+0x70).
 *
 * The pause dialog's onWidgetEvent (cOVTemplate_Dialog_onWidgetEvent) calls it.
 *
 * @return the restored heat number
 */
int cGameModeMan::restorePreviousHeat()
{
    int restoreHeat = iRestoreHeat;
    iNextHeat = restoreHeat;
    return restoreHeat;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_raceFinish);
#ifdef SKIP_ASM
extern cGameModeHandler* cGameModeMan_aHandler[];
extern unsigned int D_00536640[];
extern int cGameModeMan_iRanking[];
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cBEScoreInterface_processEventRiderStats(void* self, int idx, int a2, unsigned int v);
extern "C" void cBEScoreInterface_processRiderRanking(void* self, int idx, int a2, unsigned int v);
extern "C" int cBERewardInterface_isMountainConquered(void* self, int a, int b);
extern "C" int cBERewardInterface_isAwardReceived(void* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_awardUser(void* self, int a1, int a2, int bit);
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);


/**
 * Handles a rider finishing the race: tells the current mode handler, then records the result in the back
 * end.
 *
 * The result (with the rider's time or score, D_00536640) is only recorded at locations below 17 and when the
 * replay state is not 1-9. For rider 0, a conquered mountain leads to cBERewardInterface_awardUser() when
 * cBERewardInterface_isAwardReceived returns 0 (probably: not yet awarded). Locked results also record the rider's place
 * (cGameModeMan_iRanking).
 *
 * @param idx  rider index (On Tour iRider)
 * @param pRiderStats   the rider's stats (On Tour pRiderStats), passed to the score interface
 */
void cGameModeMan::raceFinish(int idx, int pRiderStats)
{
    cGameModeHandler* obj = cGameModeMan_aHandler[iCurrModeHandler];
    obj->raceFinish(idx);
    if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) < 17) {
        int n = *(int*)SSXApp->Game->Replay;
        int ok = n != 0 && n < 10;
        if (!ok) {
            void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
            unsigned int riderScore = D_00536640[idx];
            cBEScoreInterface_processEventRiderStats(score, idx, pRiderStats, riderScore);
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
            if (bAreResultsLocked) {
                cBEScoreInterface_processRiderRanking(score, idx, cGameModeMan_iRanking[idx], riderScore);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_riderCheckpointHit);
#ifdef SKIP_ASM

extern cGameModeHandler* cGameModeMan_aHandler[];

/**
 * Tells the current game mode that a rider passed a checkpoint.
 *
 * Calls virtual slot 6 of the current handler (cGameModeMan_aHandler[+0x4]) with both arguments.
 *
 * @param iRider  the rider
 * @param iSecsAdded  the seconds the checkpoint adds
 */
void cGameModeMan::riderCheckpointHit(int iRider, int iSecsAdded)
{
    cGameModeHandler* obj = cGameModeMan_aHandler[iCurrModeHandler];
    obj->riderCheckpointHit(iRider, iSecsAdded);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_riderGateHit);
#ifdef SKIP_ASM

extern cGameModeHandler* cGameModeMan_aHandler[];

/**
 * Tells the current game-mode handler that a rider passed a gate (its virtual slot 7).
 *
 * @param iRider  rider index (On Tour iRider)
 * @param iGateID  gate id (On Tour iGateID)
 */
void cGameModeMan::riderGateHit(int iRider, int iGateID)
{
    cGameModeHandler* obj = cGameModeMan_aHandler[iCurrModeHandler];
    obj->riderGateHit(iRider, iGateID);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_getHeatScore);
#ifdef SKIP_ASM

extern cGameModeHandler* cGameModeMan_aHandler[];

/**
 * Asks the current game-mode handler (aHandler[iCurrModeHandler], slot 9) for a rider's score in one heat.
 *
 * Declared void here, but the handler's result is left in $v0 (On Tour returns unsigned int).
 *
 * @param iRider  the rider index (On Tour iRider)
 * @param iHeat  the heat (On Tour iHeat)
 */
void cGameModeMan::getHeatScore(int iRider, int iHeat)
{
    cGameModeHandler* obj = cGameModeMan_aHandler[iCurrModeHandler];
    obj->getHeatScore(iRider, iHeat);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_writeToReplayFrame);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int cGameModeMan_iCharRank[];

/**
 * Saves the game-mode manager to a replay frame, then lets the current handler save itself (writeToReplayFrame).
 *
 * Writes iCurrModeHandler, iHeatNum, iCurrentGameEvent, iNumberAI, iSimAI, iCharacters, iLocation, iNextLocation,
 * iNextHeat, the score tables cGameModeMan_iRanking and cGameModeMan_iCharRank, uiTimeLimit, uiPointTarget, unk_80, the table D_00536640,
 * then bIsEventDone through bAreResultsLocked. iCurrentGamePath, iCheatCharacters and iRestoreHeat are not saved.
 *
 * @param data  the replay frame written to (On Tour cReplayFramePtr* data)
 */
void cGameModeMan::writeToReplayFrame(cBxReplayFrame* data)
{
    data->write((const char*)&iCurrModeHandler, 4);
    data->write((const char*)this, 4);
    data->write((const char*)&iCurrentGameEvent, 4);
    data->write((const char*)&iNumberAI, 4);
    data->write((const char*)&iSimAI, 4);
    data->write((const char*)&iCharacters[0], 40);
    data->write((const char*)&iLocation, 4);
    data->write((const char*)&iNextLocation, 4);
    data->write((const char*)&iNextHeat, 4);
    data->write((const char*)cGameModeMan_iRanking, 40);
    data->write((const char*)cGameModeMan_iCharRank, 40);
    data->write((const char*)&uiTimeLimit, 4);
    data->write((const char*)&uiPointTarget, 4);
    data->write((const char*)&unk_80, 4);
    data->write((const char*)D_00536640, 40);
    data->write((const char*)&bIsEventDone, 4);
    data->write((const char*)&bIsTimedEvent, 4);
    data->write((const char*)&bHasPointTarget, 4);
    data->write((const char*)&unk_90, 4);
    data->write((const char*)&unk_94, 4);
    data->write((const char*)&bIsLastHeat, 4);
    data->write((const char*)&bAreResultsLocked, 4);
    cGameModeMan_aHandler[iCurrModeHandler]->writeToReplayFrame(static_cast<cReplayFramePtr*>(data));
}
#endif
//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_readFromReplayFrame);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
/**
 * Restores the game-mode manager from a replay frame in cGameModeMan_writeToReplayFrame's order, then the handler.
 *
 * The handler is aHandler[iCurrModeHandler] after iCurrModeHandler has been read back (readFromReplayFrame).
 *
 * @param data  the replay frame read from (On Tour cReplayFramePtr* data)
 */
void cGameModeMan::readFromReplayFrame(cBxReplayFrame* data)
{
    data->read((char*)&iCurrModeHandler, 4);
    data->read((char*)this, 4);
    data->read((char*)&iCurrentGameEvent, 4);
    data->read((char*)&iNumberAI, 4);
    data->read((char*)&iSimAI, 4);
    data->read((char*)&iCharacters[0], 40);
    data->read((char*)&iLocation, 4);
    data->read((char*)&iNextLocation, 4);
    data->read((char*)&iNextHeat, 4);
    data->read((char*)cGameModeMan_iRanking, 40);
    data->read((char*)cGameModeMan_iCharRank, 40);
    data->read((char*)&uiTimeLimit, 4);
    data->read((char*)&uiPointTarget, 4);
    data->read((char*)&unk_80, 4);
    data->read((char*)D_00536640, 40);
    data->read((char*)&bIsEventDone, 4);
    data->read((char*)&bIsTimedEvent, 4);
    data->read((char*)&bHasPointTarget, 4);
    data->read((char*)&unk_90, 4);
    data->read((char*)&unk_94, 4);
    data->read((char*)&bIsLastHeat, 4);
    data->read((char*)&bAreResultsLocked, 4);
    cGameModeMan_aHandler[iCurrModeHandler]->readFromReplayFrame(static_cast<cReplayFramePtr*>(data));
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_sortScores);
#ifdef SKIP_ASM
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern "C" int cGameModeMan_cmpScores(const void* a, const void* b);
extern "C" int cGameModeMan_cmpRanks(const void* a, const void* b);
extern "C" void qsort(void* base, int n, int size, int (*cmp)(const void*, const void*));

/**
 * Ranks the first @p n riders by score, highest first, into the rank tables.
 *
 * Resets cGameModeMan_iRanking and cGameModeMan_iCharRank to 0..9, then sorts cGameModeMan_iCharRank with qsort (qsort) and cGameModeMan_cmpScores, and
 * cGameModeMan_iRanking with cGameModeMan_cmpRanks, both over the scores in D_00536640. The game-mode handlers call it after every heat.
 *
 * @param n  number of riders to rank
 */
void cGameModeMan::sortScores(int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        cGameModeMan_iRanking[i] = i;
        cGameModeMan_iCharRank[i] = i;
    }
    qsort(cGameModeMan_iCharRank, n, 4, cGameModeMan_cmpScores);
    qsort(cGameModeMan_iRanking, n, 4, cGameModeMan_cmpRanks);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_sortTimes);
#ifdef SKIP_ASM
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern "C" int cGameModeMan_cmpTimes(const void* a, const void* b);
extern "C" int cGameModeMan_cmpRanks(const void* a, const void* b);
extern "C" void qsort(void* base, int n, int size, int (*cmp)(const void*, const void*));

/**
 * Ranks the first @p n riders by time, lowest first, into the rank tables.
 *
 * Same as cGameModeMan_sortScores except that cGameModeMan_iCharRank is sorted with cGameModeMan_cmpTimes (ascending), so the smallest values in
 * D_00536640 come first.
 *
 * @param n  number of riders to rank
 */
void cGameModeMan::sortTimes(int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        cGameModeMan_iRanking[i] = i;
        cGameModeMan_iCharRank[i] = i;
    }
    qsort(cGameModeMan_iCharRank, n, 4, cGameModeMan_cmpTimes);
    qsort(cGameModeMan_iRanking, n, 4, cGameModeMan_cmpRanks);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeHandler_purge);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

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


/**
 * Purges a game-mode handler: resets the game-mode manager to "no event" and clears its tables.
 *
 * Through cGameModeHandler_pManager it sets bIsEventDone to 1, iCurrModeHandler, iLocation, iNextLocation and
 * iCurrentGameEvent to -1, clears the heat numbers, AI counts, time limit, point target and flags, zeroes iCharacters
 * and iCheatCharacters, then calls the handler's own slot +0x2C (cGameModeHandler_cleanRiderRanksTime by default). It is slot +0x1C of every
 * handler vtable; cGameModeMan_initGameMode calls it on the old handler.
 */
void cGameModeHandler::purge()
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
    memset(gm->iCharacters, 0, 40);
    memset(cGameModeHandler_pManager->iCheatCharacters, 0, 40);
    cleanRiderRanksTime();
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeHandler_cleanRiderRanksPoints);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_bGiveUp[];

/**
 * Clears the rank and score tables for a scored event.
 *
 * Fills cGameModeMan_iRanking with the byte 9, and cGameModeMan_iCharRank, the score table D_00536640, D_005366A8 and cGameModeMan_bGiveUp
 * with 0. It is slot +0x24 of every handler vtable. Quirk: memset with 9 makes each rank word 0x09090909, not 9.
 */
extern "C" void cGameModeHandler_cleanRiderRanksPoints(void)
{
    memset(cGameModeMan_iRanking, 9, 40);
    memset(cGameModeMan_iCharRank, 0, 40);
    memset(D_00536640, 0, 40);
    memset(D_005366A8, 0, 40);
    memset(cGameModeMan_bGiveUp, 0, 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGameModeHandler_cleanRiderRanksTime);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_bGiveUp[];

/**
 * Clears the rank and score tables for a timed event.
 *
 * Fills cGameModeMan_iRanking and the time table D_00536640 with the byte 9 (0x09090909 per word, a large time), and cGameModeMan_iCharRank,
 * D_005366A8 and cGameModeMan_bGiveUp with 0. It is slot +0x2C of every handler vtable; cGameModeHandler_purge calls it.
 */
extern "C" void cGameModeHandler_cleanRiderRanksTime(void)
{
    memset(cGameModeMan_iRanking, 9, 40);
    memset(cGameModeMan_iCharRank, 0, 40);
    memset(D_00536640, 9, 40);
    memset(D_005366A8, 0, 40);
    memset(cGameModeMan_bGiveUp, 0, 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cBENewRaceInterface_getTimeLimit(void* iface, int a1);
extern "C" void cGameModeMan_sortScores(cGameModeMan* self, int n);
// PORT: func_00239938 is defined as (void); this caller passes self.
extern "C" void func_00239938_self(void* self) __asm__("func_00239938");
extern "C" void cGMBigAirHandler_fillScoreTable(cGMBigAirHandler* self);
extern "C" void* memset(void* dst, int c, int n);
extern unsigned int D_00536640[];
extern int cGameModeMan_iCharRank[];

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


static inline cAI* race_238E20()
{
    return SSXApp->Game->AI;
}

/**
 * Starts a heat of the scored three-heat event (kHANDLER_BIGAIR, vtable cGMBigAirHandler_vtbl; the offline freestyle
 * events kGAMEEVENT_SLOPESTYLE, SUPERPIPE and BIGAIR).
 *
 * Sets the time limit from cBENewRaceInterface_getTimeLimit(race, heat) * 60 and iHeatNum from iNextHeat. Heat 1 (or 0) sets five timed AI
 * riders and, when the event was done, clears the score table (+0x4, 3 heats x 10 riders), runs slot +0x24, picks the
 * line-up (func_00239938) and the AI scores (cGMBigAirHandler_fillScoreTable); it then ranks twice with sortScores (cGameModeMan_sortScores) and
 * puts the two leaders' characters in the race (AI + 0x20, + 0x24). Heat 2 ranks on heats 1 + 2; heat 3 is the last
 * heat and ranks on heat 3. When the game path (newRace.cGamePath) is not 0 it first sets up a single heat-3 event with fresh
 * line-up. It fills the init slot (+0x14), which cGameModeMan_restartHeat calls.
 */
void cGMBigAirHandler::init()
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int i;
    cGameModeMan* gm = cGameModeHandler_pManager;
    int quick = cBENewRaceInterface_newRace.cGamePath;
    gm->bHasPointTarget = 0;
    gm->uiPointTarget = 0;
    if (quick != 0) {
        gm->iNumberAI = 5;
        if (gm->iCurrentGameEvent == kGAMEEVENT_SLOPESTYLE) {
            gm->iSimAI = 4;
        } else {
            gm->iSimAI = 5;
        }
        cGameModeMan* gameModeMan = cGameModeHandler_pManager;
        gameModeMan->iNextLocation = -1;
        gameModeMan->unk_90 = 0;
        if (gameModeMan->iCurrentGameEvent == kGAMEEVENT_SLOPESTYLE) {
            gameModeMan->unk_94 = gameModeMan->iCurrentGameEvent;
        } else {
            gameModeMan->unk_94 = 0;
        }
        memset(uiScore, 0, 0x78);
        cleanRiderRanksPoints();
        if (cGameModeHandler_pManager->bIsEventDone == true) {
            func_00239938_self(this);
        }
        cGameModeMan* modeMan = cGameModeHandler_pManager;
        modeMan->iNextHeat = 3;
        modeMan->iRestoreHeat = 3;
        this->fillScoreTable();
    }
    cGameModeHandler_pManager->uiTimeLimit = cBENewRaceInterface_getTimeLimit(race, cGameModeHandler_pManager->iHeatNum) * 60;
    cGameModeMan* modeManager = cGameModeHandler_pManager;
    if (modeManager->iCurrentGameEvent == kGAMEEVENT_SUPERPIPE) {
        modeManager->unk_94 = 0;
    } else {
        modeManager->unk_94 = 1;
    }
    cGameModeMan* manager = cGameModeHandler_pManager;
    int nextHeat = manager->iNextHeat;
    manager->iHeatNum = nextHeat;
    switch (nextHeat) {
    case 0:
    case cGameModeMan::kHeat1: {
        manager->iHeatNum = 1;
        manager->iNextHeat = 1;
        manager->iRestoreHeat = 1;
        manager->iNumberAI = 5;
        manager->bIsTimedEvent = 1;
        manager->unk_90 = 0;
        manager->bAreResultsLocked = 0;
        manager->bIsLastHeat = 0;
        manager->unk_80 = 0;
        manager->iSimAI = 5;
        manager->iNextLocation = -1;
        uiScore[0][0] = 0;
        uiScore[1][0] = 0;
        uiScore[2][0] = 0;
        unsigned int* firstHeatScores = uiScore[0];
        if (manager->bIsEventDone == true) {
            memset(firstHeatScores, 0, 0x78);
            cleanRiderRanksPoints();
            func_00239938_self(this);
            this->fillScoreTable();
        }
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = uiScore[2][i];
        }
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        {
            cGameModeMan* pManager = cGameModeHandler_pManager;
            race_238E20()->PodiumActorList[0] = pManager->iCharacters[cGameModeMan_iCharRank[0]];
            race_238E20()->PodiumActorList[1] = pManager->iCharacters[cGameModeMan_iCharRank[1]];
            for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
                D_00536640[i] = firstHeatScores[i];
            }
        }
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        cGameModeHandler_pManager->bIsEventDone = 0;
        break;
    }
    case cGameModeMan::kHeat2:
        manager->bAreResultsLocked = 0;
        manager->iNextHeat = 1;
        manager->iRestoreHeat = 1;
        manager->bIsLastHeat = 0;
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = uiScore[1][i] + uiScore[0][i];
        }
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        break;
    case cGameModeMan::kFinalHeat:
        manager->bIsLastHeat = 1;
        manager->iNextHeat = 3;
        manager->iRestoreHeat = 3;
        manager->bIsTimedEvent = 1;
        manager->bAreResultsLocked = 0;
        manager->bIsEventDone = 0;
        D_00536640[0] = 0;
        for (i = 1; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            D_00536640[i] = uiScore[2][i];
        }
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_raceFinish);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishScore(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_00147D20(void* iface, int a1);
extern "C" void func_00147E18(void* iface, int a1);
extern "C" void cGameModeMan_sortScores(cGameModeMan* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];




static inline cAI* Race_239230()
{
    return SSXApp->Game->AI;
}

static inline cRider* Rider_239230(int i)
{
    return Race_239230()->Riders[i];
}

/**
 * Records a rider's heat score in the scored three-heat event and, when the race is over, decides the next heat.
 *
 * Stores the rider's score (its metrics block + 0xFC + 0x9C) or 0 when it did not finish (D_005366A8 marks that) in the
 * heat's column. Once cAI_isRaceFinished: heat 1 ranks on heats 1 + 2 and sends a top-3 rider to heat 3; heat 2 also
 * reports the result to the player interface (func_00147D20 for a win, func_00147E18 for a loss, career only); heat 3
 * estimates unfinished riders' scores (cRider_estimateFinishScore), locks the results and ranks. Finally sets the human
 * rider's +0x2F8 to 0.7, 0.35 or 0 for first, second or other place. Slot +0x44 (raceFinish) of cGMBigAirHandler_vtbl.
 *
 * @param idx  the rider index
 */
void cGMBigAirHandler::raceFinish(int idx)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    int quick = cBENewRaceInterface_newRace.cGamePath;
    int n = cBENewPlayerInterface_getHumanRider(player, 0);
    tRiderStats st = *(&Rider_239230(idx)->mMetrix->Totals);
    if (Rider_239230(idx)->mDidNotFinish != 0) {
        uiScore[cGameModeHandler_pManager->iHeatNum - 1][idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        uiScore[cGameModeHandler_pManager->iHeatNum - 1][idx] = st.TotalPoints;
        D_005366A8[idx] = 0;
    }
    if (cAI_isRaceFinished(Race_239230()) == 0)
        return;
    cGameModeMan* gm = cGameModeHandler_pManager;
    switch (gm->iHeatNum) {
    case cGameModeMan::kHeat1:
        gm->iRestoreHeat = gm->iNextHeat;
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = uiScore[0][i] + uiScore[1][i];
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        if (cGameModeMan_iRanking[idx] < 3)
            cGameModeHandler_pManager->iNextHeat = 3;
        else
            cGameModeHandler_pManager->iNextHeat = cGameModeHandler_pManager->iHeatNum + 1;
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = uiScore[0][i];
        if (cGameModeMan_iRanking[idx] >= 3) {
            cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
            if (cGameModeMan_iRanking[idx] >= 3) {
                Rider_239230(idx)->ShouldCelebrateAtFinish = 0;
                break;
            }
        }
        Rider_239230(idx)->ShouldCelebrateAtFinish = 1;
        break;
    case cGameModeMan::kHeat2:
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = uiScore[0][i] + uiScore[1][i];
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = uiScore[1][i];
        gm = cGameModeHandler_pManager;
        gm->iRestoreHeat = gm->iNextHeat;
        if (cGameModeMan_iRanking[idx] < 3) {
            gm->iNextHeat = 3;
            Rider_239230(idx)->ShouldCelebrateAtFinish = 1;
            if (quick == 0 && cGameModeMan_iRanking[idx] == 0)
                func_00147D20(player, 0);
        } else {
            gm->iNextHeat = 1;
            Rider_239230(idx)->ShouldCelebrateAtFinish = 0;
            if (quick == 0)
                func_00147E18(player, 0);
        }
        break;
    case cGameModeMan::kFinalHeat: {
        gm->bAreResultsLocked = 1;
        int last = Race_239230()->NumberOfComputersAllocated;
        for (int i = 0; i < last + 1; i++) {
            cRider* rider = Rider_239230(i);
            int done = rider->mFinishTime >= 0.0f;
            if (!done)
                uiScore[2][i] = cRider_estimateFinishScore(rider);
        }
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++)
            D_00536640[i] = uiScore[2][i];
        cGameModeHandler_pManager->sortScores(cGameModeHandler_pManager->iNumberAI + 1);
        gm = cGameModeHandler_pManager;
        gm->iRestoreHeat = gm->iNextHeat;
        int lost = !(cGameModeMan_iRanking[idx] < 3);
        if (quick == 0 && !lost) {
            gm->iNextHeat = 0;
            gm->iRestoreHeat = 0;
        } else {
            gm->iNextHeat = 3;
        }
        if (lost) {
            Rider_239230(idx)->ShouldCelebrateAtFinish = 0;
            if (quick == 0)
                func_00147E18(player, 0);
        } else {
            Rider_239230(idx)->ShouldCelebrateAtFinish = 1;
            if (quick == 0)
                func_00147D20(player, 0);
        }
        break;
    }
    }
    int place = cGameModeMan_iRanking[n];
    if (place == cGameModeMan::kFirstPlace)
        ((&Race_239230()->Players[0])[0]->mRider)->BoostPowerBar = 0.699999988079071f;
    else if (place == cGameModeMan::kSecondPlace)
        ((&Race_239230()->Players[0])[0]->mRider)->BoostPowerBar = 0.3499999940395355f;
    else
        ((&Race_239230()->Players[0])[0]->mRider)->BoostPowerBar = 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_riderCheckpointHit);
#ifdef SKIP_ASM

/**
 * Handles a checkpoint in the scored three-heat event: in kGAMEEVENT_SLOPESTYLE adds @p secs seconds to the time
 * limit.
 *
 * Returns true except in kGAMEEVENT_SLOPESTYLE when the new limit is still below the race clock (AI + 0x8). Slot +0x34
 * (riderCheckpointHit) of cGMBigAirHandler_vtbl.
 *
 * @param player  the rider index (unused)
 * @param secs  seconds to add
 * @return true, or false when the extended limit has already run out
 */
bool cGMBigAirHandler::riderCheckpointHit(int player, int secs)
{
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    if (gameModeMan->iCurrentGameEvent == kGAMEEVENT_SLOPESTYLE) {
        unsigned int limit = (unsigned int)SSXApp->Game->AI->AITick;
        unsigned int newTimeLimit = gameModeMan->uiTimeLimit + secs * 60;
        gameModeMan->uiTimeLimit = newTimeLimit;
        if (newTimeLimit < limit) {
            return false;
        }
        return true;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239938);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int player);
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

/**
 * Builds the AI line-up: the player's character, the rival, then the other characters in random order.
 *
 * Takes the human rider's character (cBENewPlayerInterface_getRiderCharID) and the rival (cBENewRaceInterface_getRival), shuffles the
 * remaining characters 0-9 with func_0023C770 and stores them in iCharacters (cheat characters 0, the player's from
 * cBENewPlayerInterface_getRiderCheatCharID). Off the career path (cGamePath != 0) the rival slot gets the last shuffled character instead.
 */
extern "C" void func_00239938(void)
{
    int n = 0;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
    int charA = cBENewPlayerInterface_getRiderCharID(player, humanRider);
    int charB = cBENewRaceInterface_getRival(race);
    int list[10];

    for (int i = 0; i < 10; i++) {
        if (i != charA && i != charB) {
            list[n] = i;
            n++;
        }
    }
    func_0023C770(list, n);
    cGameModeHandler_pManager->iCharacters[0] = charA;
    cGameModeHandler_pManager->iCheatCharacters[0] = cBENewPlayerInterface_getRiderCheatCharID(player, humanRider);
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
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_fillScoreTable);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" short cBENewPlayerInterface_getDifficultyLevel(void* self, int which);
extern "C" int cBENewRaceInterface_getSimThreshold(void* self, int kind, int place, int mode);
extern "C" void cGameModeMan_sortScores(cGameModeMan* self, int n);
extern unsigned int D_00536640[];
extern int cGameModeMan_iRanking[];


static inline int opt49_239AA0(cBERaceDB* o)
{
    return o->cGamePath;
}

/**
 * Fills the AI riders' scores for all three heats of the scored event.
 *
 * For the non-simulated riders it clears all three heats and ranks them 5, 4, ...; for each simulated AI rider it asks
 * the race interface (cBENewRaceInterface_getSimThreshold) for a score per heat (or repeats heat 1's when the race setting at newRace + 0x49
 * is set), then ranks everyone with sortScores (cGameModeMan_sortScores).
 */
void cGMBigAirHandler::fillScoreTable()
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    int nB = cGameModeHandler_pManager->iSimAI;
    int numNonSimulated = cGameModeHandler_pManager->iNumberAI - nB;
    int k = 0;
    for (int i = 0; i < numNonSimulated + 1; i++) {
        cGameModeMan_iRanking[i] = 5 - i;
        for (int j = 0; j < 3; j++) {
            uiScore[j][i] = 0;
        }
        D_00536640[i] = uiScore[0][i];
        k++;
    }
    int nA = numNonSimulated + 1;
    for (int simRider = 0; simRider < nB; simRider++) {
        for (int heat = 0; heat < 3; heat++) {
            int repeatFirstHeat = opt49_239AA0(&cBENewRaceInterface_newRace);
            int same = 0;
            if (heat != 0) same = 1;
            if (repeatFirstHeat == 0) same = 0;
            if (same) {
                uiScore[heat][k] = uiScore[0][k];
            } else {
                uiScore[heat][k] = cBENewRaceInterface_getSimThreshold(race, heat + 1, simRider,
                        cBENewPlayerInterface_getDifficultyLevel(player, 0));
            }
        }
        D_00536640[k] = uiScore[0][k];
        cGameModeMan_iRanking[k] = simRider;
        k++;
    }
    cGameModeHandler_pManager->sortScores(nA + nB);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_getHeatScore);
#ifdef SKIP_ASM
/**
 * Returns a rider's score in one heat of the scored event, or 0 for a heat not run yet.
 *
 * Slot +0x4C (getHeatScore) of cGMBigAirHandler_vtbl; reads uiScore (heat x 10 riders).
 *
 * @param col  the rider index
 * @param row  the heat
 * @return the score, or 0 when @p row is past iHeatNum
 */
unsigned int cGMBigAirHandler::getHeatScore(int col, int row)
{
    if (cGameModeHandler_pManager->iHeatNum < row) {
        return 0;
    }
    return uiScore[row][col];
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_writeToReplayFrame);
#ifdef SKIP_ASM
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
/**
 * Saves the scored event handler's 160-byte table (+0x4) to a replay frame with the stream's write (its first virtual).
 *
 * Slot +0x54 (writeToReplayFrame) of cGMBigAirHandler_vtbl.
 *
 * @param stream  the replay stream
 */
extern "C" void cGMBigAirHandler_writeToReplayFrame(cGMBigAirHandler* self, cBxReplayFrame* stream)
{
    stream->write((char*)&self->uiScore[0][0], 160);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMBigAirHandler_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores the scored event handler's 160-byte table (+0x4) from a replay frame with the stream's read (its second
 * virtual).
 *
 * Slot +0x5C (readFromReplayFrame) of cGMBigAirHandler_vtbl.
 *
 * @param stream  the replay stream
 */
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
extern "C" void cGMBigAirHandler_readFromReplayFrame(cGMBigAirHandler* self, cBxReplayFrame* stream)
{
    stream->read((char*)&self->uiScore[0][0], 160);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPSlopeHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* iface, int player);
extern "C" int cBENewRaceInterface_getTimeLimit(void* iface, int a1);
extern "C" int cBENewPlayerInterface_getPlayerCheatCharID(void* iface, int player);

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


/**
 * Starts the two-player online event of handler 9 (vtable cGMMPSlopeHandler_vtbl; online events 1-3).
 *
 * Sets a single last heat (3) with no AI, a time limit of cBENewRaceInterface_getTimeLimit(race, 3) * 60, runs slot +0x24, puts both
 * players' characters and cheat characters in the tables and clears the two per-player counters (+0x4, +0x6).
 */
void cGMMPSlopeHandler::init()
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
    int heatTimeLimit = cBENewRaceInterface_getTimeLimit(race, 3);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->uiPointTarget = 0;
    gm2->bHasPointTarget = 0;
    gm2->uiTimeLimit = heatTimeLimit * 60;
    memset(gm2->iCharacters, -1, 40);
    cleanRiderRanksPoints();
    cGameModeHandler_pManager->iCharacters[0] = cBENewPlayerInterface_getPlayerCharID(player, 0);
    cGameModeHandler_pManager->iCharacters[1] = cBENewPlayerInterface_getPlayerCharID(player, 1);
    cGameModeHandler_pManager->iCheatCharacters[0] = cBENewPlayerInterface_getPlayerCheatCharID(player, 0);
    cGameModeHandler_pManager->iCheatCharacters[1] = cBENewPlayerInterface_getPlayerCheatCharID(player, 1);
    LastCheckpointHit[0] = 0;
    LastCheckpointHit[1] = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPSlopeHandler_raceFinish);
#ifdef SKIP_ASM
/**
 * Records a rider's finish in the two-player online event of handler 9 and locks the results when the race is over.
 *
 * Stores the rider's score (or 0 and a did-not-finish mark in D_005366A8); when cAI_isRaceFinished, sets the event
 * done, locks the results, ranks the two riders with sortScores and clears +0x100 of a rider who did not win. Slot
 * +0x44 (raceFinish) of cGMMPSlopeHandler_vtbl.
 *
 * @param idx  the rider index
 */
void cGMMPSlopeHandler::raceFinish(int idx)
{
    cAI* race = Race_239230();
    tRiderStats st = *(&race->Riders[idx]->mMetrix->Totals);
    bool dnf = Rider_239230(idx)->mDidNotFinish != 0;
    if (dnf) {
        D_00536640[idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        D_00536640[idx] = st.TotalPoints;
        D_005366A8[idx] = 0;
    }
    bool over = cAI_isRaceFinished(race) != 0;
    if (over) {
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bIsEventDone = 1;
        gm->bAreResultsLocked = 1;
        gm->sortScores(2);
        if (cGameModeMan_iRanking[idx] > 0) {
            Rider_239230(idx)->ShouldCelebrateAtFinish = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPSlopeHandler_riderCheckpointHit);
#ifdef SKIP_ASM

/**
 * Counts a checkpoint for one player in the two-player event of kHANDLER_MPSLOPE.
 *
 * Increments the player's counter (LastCheckpointHit[@p player]); returns false unless it is now ahead of the other
 * player's. In kGAMEEVENT_SLOPESTYLE the leader also adds @p secs seconds to the time limit, unless the limit has
 * already passed (then false). Slot +0x34 (riderCheckpointHit) of cGMMPSlopeHandler_vtbl.
 *
 * @param player  the player (0 or 1)
 * @param secs  seconds to add
 * @return true when the player took the lead (and time was added)
 */
bool cGMMPSlopeHandler::riderCheckpointHit(int player, int secs)
{
    short* score = LastCheckpointHit;
    short* mine = &score[player];
    short newScore = ++*mine;
    int other = 0;
    if (player == 0) {
        other = 1;
    }
    short* theirs = &score[other];
    if (*theirs >= newScore) {
        return false;
    }
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    if (gameModeMan->iCurrentGameEvent == kGAMEEVENT_SLOPESTYLE) {
        unsigned int timeLimit = gameModeMan->uiTimeLimit;
        if (timeLimit < (unsigned int)SSXApp->Game->AI->AITick) {
            return false;
        }
        gameModeMan->uiTimeLimit = secs * 60 + timeLimit;
        return true;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRaceHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" void* memset(void* dst, int c, int n);
extern "C" void func_0023A4F0(cGMRaceHandler* self);
extern "C" void func_0023A668(void* self);
extern unsigned int D_00536640[];
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];

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

static inline void fillOthers_23A108(int* dst, int p)
{
    for (int i = 0; i < 3; i++) {
        int v = cGameModeMan_iCharRank[i];
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

/**
 * Starts a heat of the three-heat timed event (kHANDLER_RACE, vtable cGMRaceHandler_vtbl; offline kGAMEEVENT_RACE).
 *
 * Sets iHeatNum from iNextHeat (forced to kFinalHeat off the career path) and five AI riders. Heat 1 builds the heat
 * line-ups (func_0023A4F0) and applies one (func_0023A668) when the event was done, fills the time table (+0x58) with
 * 9s and runs slot +0x2C. Heat 2 and heat 3 record the player's character and the riders that survived the last heat
 * (cGameModeMan_iCharRank) in the next line-up and apply it, and reset the score and rank tables. +0x8 and +0xC
 * remember which heat set-up already ran.
 */
void cGMRaceHandler::init()
{
    cGameModeMan* gameModeMan = cGameModeHandler_pManager;
    gameModeMan->bAreResultsLocked = 0;
    gameModeMan->iHeatNum = gameModeMan->iNextHeat;
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int quick = cBENewRaceInterface_newRace.cGamePath;
    if (quick != 0 && cGameModeHandler_pManager->iHeatNum != cGameModeMan::kFinalHeat) {
        cGameModeHandler_pManager->iHeatNum = cGameModeMan::kFinalHeat;
        mHeat3Ready = 0;
    }
    cGameModeMan* gm = cGameModeHandler_pManager;
    int state = gm->iHeatNum;
    gm->uiPointTarget = 0;
    gm->bHasPointTarget = 0;
    switch (state) {
    case 0:
    case cGameModeMan::kHeat1:
        gm->bIsLastHeat = 0;
        gm->iHeatNum = 1;
        gm->iSimAI = 0;
        gm->iNumberAI = 5;
        if (gm->bIsEventDone == true) {
            func_0023A4F0(this);
            func_0023A668(this);
        }
        memset(uiTime, 9, 0x78);
        cleanRiderRanksTime();
        {
            cGameModeMan* manager = cGameModeHandler_pManager;
            manager->iNextLocation = -1;
            manager->iNextHeat = 1;
            manager->iRestoreHeat = 1;
            manager->uiTimeLimit = 0;
            manager->unk_80 = 0;
            manager->bIsEventDone = 0;
            manager->bIsTimedEvent = 0;
            manager->unk_90 = 1;
            manager->unk_94 = 0;
        }
        mHeat2Ready = 0;
        mHeat3Ready = 0;
        break;
    case cGameModeMan::kHeat2:
        gm->bIsLastHeat = 0;
        gm->iNumberAI = 5;
        gm->iSimAI = 0;
        if (mHeat2Ready == 0) {
            int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
            mLineup[1][0] = cBENewPlayerInterface_getRiderCharID(player, humanRider);
            fillOthers_23A108(&mLineup[1][1], humanRider);
            func_0023A668(this);
        }
        {
            cGameModeMan* manager = cGameModeHandler_pManager;
            manager->iNextLocation = -1;
            manager->iNextHeat = 2;
            manager->iRestoreHeat = 2;
        }
        memset(D_00536640, 9, 40);
        memset(cGameModeMan_iRanking, 0, 40);
        memset(uiTime[1], 9, 40);
        {
            cGameModeMan* manager = cGameModeHandler_pManager;
            manager->uiTimeLimit = 0;
            manager->unk_80 = 0;
            manager->bIsEventDone = 0;
            manager->bIsTimedEvent = 0;
            manager->unk_90 = 0;
            manager->unk_94 = 0;
        }
        mHeat2Ready = 1;
        mHeat3Ready = 0;
        break;
    case cGameModeMan::kFinalHeat:
        gm->iNumberAI = 5;
        gm->iSimAI = 0;
        gm->bIsLastHeat = 1;
        if (mHeat3Ready == 0) {
            if (quick != 0) {
                func_0023A4F0(this);
                cGameModeHandler_pManager->iHeatNum = 1;
                func_0023A668(this);
                cGameModeHandler_pManager->iHeatNum = state;
                memset(uiTime, 9, 0x78);
                cleanRiderRanksTime();
            } else {
                int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
                mLineup[2][0] = cBENewPlayerInterface_getRiderCharID(player, humanRider);
                mLineup[2][1] = cBENewRaceInterface_getRival(race);
                fillOthers_23A108(&mLineup[2][2], humanRider);
                func_0023A668(this);
            }
        }
        {
            cGameModeMan* manager = cGameModeHandler_pManager;
            manager->iNextLocation = -1;
            manager->iNextHeat = 3;
            manager->iRestoreHeat = 3;
        }
        memset(D_00536640, 9, 40);
        memset(cGameModeMan_iRanking, 9, 40);
        memset(uiTime[2], 9, 40);
        {
            cGameModeMan* manager = cGameModeHandler_pManager;
            manager->uiTimeLimit = 0;
            manager->unk_80 = 0;
            manager->bIsEventDone = 0;
            manager->bIsTimedEvent = 0;
            manager->unk_90 = 0;
            manager->unk_94 = 0;
        }
        mHeat3Ready = 1;
        mHeat2Ready = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A4F0);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" unsigned int GMrand(void);
extern "C" void func_0023C770(int* arr, unsigned int n);

/**
 * Builds the timed event's heat line-ups: the player, two random characters from 10-16, then the other characters.
 *
 * Draws two distinct values with GMrand (10 + rand % 7, and 10 + rand % 6 skipping the first), lists the characters 0-9
 * other than the player's and the rival's (cBENewRaceInterface_getRival), shuffles all ten with func_0023C770 and stores the player's
 * character and the shuffled values in the line-up rows at +0x10 .. +0x54.
 */
extern "C" void func_0023A4F0(cGMRaceHandler* self)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int charA = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    int charB = cBENewRaceInterface_getRival(race);
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
    self->mLineup[0][0] = charA;
    self->mLineup[0][1] = arr[0];
    self->mLineup[0][2] = arr[1];
    self->mLineup[0][3] = arr[2];
    self->mLineup[0][4] = arr[3];
    self->mLineup[0][5] = arr[4];
    self->mLineup[1][3] = arr[5];
    self->mLineup[1][4] = arr[6];
    self->mLineup[1][5] = arr[7];
    self->mLineup[2][4] = arr[8];
    self->mLineup[2][5] = arr[9];
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023A668);
#ifdef SKIP_ASM


extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

int cBENewPlayerInterface_getRiderCharID(void* iface, int player);

extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int player);

extern "C" void func_0023A668_A668(cGMRaceHandler* self) __asm__("func_0023A668");
/**
 * Applies the current heat's line-up row to the game-mode manager's character tables.
 *
 * For the row of heat iHeatNum - 1 (+0x10, six entries) it copies each character to iCharacters; values of 10 and above
 * go to iCheatCharacters with the player's character in iCharacters. The human rider's slot gets the player's cheat
 * character (cBENewPlayerInterface_getRiderCheatCharID).
 */
extern "C" void func_0023A668_A668(cGMRaceHandler* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int id = cBENewPlayerInterface_getHumanRider(iface, 0);
    int ch = cBENewPlayerInterface_getRiderCharID(iface, id);
    int cheatChar = cBENewPlayerInterface_getRiderCheatCharID(iface, id);
    int k = cGameModeHandler_pManager->iHeatNum - 1;
    if (k < 0) k = 0;
    for (int i = 0; i < 6; i++) {
        int charId = self->mLineup[k][i];
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
INCLUDE_ASM("main/gamemode", cGMRaceHandler_raceFinish);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).
// Virtual calls in this unit go through these by hand.

// The game-mode manager (cGameModeMan, 0xA0 bytes; cGameModeMan_getGM creates it). cGameModeMan_instance owns it and
// D_004A2C70_A760 is the pointer the handlers read it through. iNumberAI is the prototype's name (assert
// `iRider < (pManager->iNumberAI+1)` in cGMRaceHandler_raceFinish); mode indexes the handler table cGameModeMan_aHandler.

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

// The game singleton (SSXApp_A760).
struct sApp {
    char pad_0x0[0x78];
    void* f78;                  // 0x78
    char pad_0x7C[0x8];
    sGame* game;                // 0x84
};

extern sApp* SSXApp_A760 __asm__("SSXApp");

// Race settings (D_00535BC8_A760: cBENewRaceInterface_newRace, be/beintnewrace.h; ai/ai's sRaceSettings); 0x49 / 0x4A
// are its cGamePath / cGameEvent.
struct sRaceSettings {
    char pad[0x49];
    signed char f49;            // 0x49
};


// Game-mode handler base (cGameModeHandler): the handlers in cGameModeMan_aHandler are its subclasses.
// The race handler (cGMRaceHandler, vtable cGMRaceHandler_vtbl, 0xD0 bytes).
static inline sRace* GetRace()
{
    return SSXApp_A760->game->race;
}

static inline int Finished(cRider* rider)
{
    return rider->mFinishTime >= 0.0f;
}

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern int cGameModeMan_iRanking[];

extern unsigned int D_00536640[];

extern int D_005366A8[];

extern "C" int cAI_isRaceFinished(void* race);

extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

extern "C" void func_00147D20(void* iface, int a1);

extern "C" void func_00147E18(void* iface, int a1);

extern "C" int cRider_estimateFinishTick(void* self);

extern "C" void cGameModeMan_sortTimes_A760(void* self, int n) __asm__("cGameModeMan_sortTimes");

static inline int vcall_23A760(char* obj, int slot)
{
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    return vt[slot].fn(vthis(obj, vt[slot]));
}

/**
 * Records a rider's time in the timed three-heat event and, when the race is over, decides who goes on.
 *
 * Stores the finish tick (360000 for did-not-finish) in the heat's row of the table at +0x58. When the race is over and
 * the rider's state says so, estimates unfinished riders (cRider_estimateFinishTick), ranks with sortTimes
 * (cGameModeMan_sortTimes) and: a top-3 human goes to the next heat (heat 3 ends the event), otherwise repeats the heat. On the
 * career path it reports big wins and losses to the player interface (func_00147D20 / func_00147E18), and heat 3 locks
 * the results. Sets the human's +0x2F8 to 0.7, 0.35 or 0 by place. Slot +0x44 (raceFinish) of cGMRaceHandler_vtbl.
 *
 * @param idx  the rider index
 */
// cGMRaceHandler slot 7: records rider idx's heat time, ranks the heat and moves the heat state on.
void cGMRaceHandler::raceFinish(int idx)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int n = cBENewPlayerInterface_getHumanRider(player, 0);
    int heat = cGameModeHandler_pManager->iHeatNum - 1;
    sRace* rc = GetRace();
    if (cAI_isRaceFinished(rc) == 0 || vcall_23A760((char*)&VPTR_AT(GetRace()->riders[idx], sizeof(cRiderState)), 9) == 0) {
        int tick = rc->riders[idx]->mFinishTick;
        if (GetRace()->riders[idx]->mDidNotFinish) {
            uiTime[heat][idx] = 360000;
            D_005366A8[idx] = 1;
        } else {
            uiTime[heat][idx] = tick;
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(rc) == 0) {
        return;
    }
    if (vcall_23A760((char*)&VPTR_AT(GetRace()->riders[idx], sizeof(cRiderState)), 8) == 0) {
        return;
    }
    for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
        if (!Finished(rc->riders[i])) {
            uiTime[heat][i] = cRider_estimateFinishTick(rc->riders[i]);
        }
        D_00536640[i] = uiTime[heat][i];
    }
    cGameModeMan_sortTimes_A760(cGameModeHandler_pManager, 10);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iRestoreHeat = gm->iNextHeat;
    if (cGameModeMan_iRanking[n] < 3) {
        gm->iNextHeat = gm->iHeatNum + 1;
        GetRace()->riders[idx]->ShouldCelebrateAtFinish = 1;
        if (gm->iHeatNum == cGameModeMan::kFinalHeat) {
            gm->bIsEventDone = 1;
            gm->iNextHeat = 3;
        }
        if (cBENewRaceInterface_newRace.cGamePath == 0) {
            if (cGameModeMan_iRanking[n] == cGameModeMan::kFirstPlace) {
                if ((cGameModeHandler_pManager->iHeatNum < cGameModeMan::kHeat2
                     && D_00536640[1] - D_00536640[0] > 600) ||
                    (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kHeat2
                     && D_00536640[1] - D_00536640[0] > 300) ||
                    (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kFinalHeat
                     && D_00536640[1] - D_00536640[0] > 180)) {
                    func_00147D20(player, 1);
                }
            }
            if (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kFinalHeat) {
                cGameModeHandler_pManager->iNextHeat = 0;
                cGameModeHandler_pManager->iRestoreHeat = 0;
            }
        }
    } else {
        gm->iNextHeat = gm->iHeatNum;
        mHeat2Ready = 1;
        mHeat3Ready = 1;
        GetRace()->riders[idx]->ShouldCelebrateAtFinish = 0;
        if (cBENewRaceInterface_newRace.cGamePath == 0) {
            if (cGameModeHandler_pManager->iHeatNum < cGameModeMan::kHeat2 ||
                (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kHeat2 && cGameModeMan_iRanking[n] >= 4) ||
                (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kFinalHeat
                 && cGameModeMan_iRanking[n] == cGameModeMan::kLastPlace)) {
                func_00147E18(player, 1);
            }
        }
    }
    if (cGameModeHandler_pManager->iHeatNum == cGameModeMan::kFinalHeat) {
        cGameModeHandler_pManager->bAreResultsLocked = 1;
    }
    int place = cGameModeMan_iRanking[n];
    if (place == cGameModeMan::kFirstPlace)
        GetRace()->players[0]->rider->BoostPowerBar = 0.699999988079071f;
    else if (place == cGameModeMan::kSecondPlace)
        GetRace()->players[0]->rider->BoostPowerBar = 0.3499999940395355f;
    else
        GetRace()->players[0]->rider->BoostPowerBar = 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRaceHandler_riderGateHit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: slot +0x3C (riderGateHit) of the timed event handler's vtable cGMRaceHandler_vtbl.
 */
void cGMRaceHandler_riderGateHit(void* vself)
{
    cGMRaceHandler* self = (cGMRaceHandler*)vself;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRaceHandler_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves the whole 208-byte timed event handler to a replay frame with the stream's write (its first virtual).
 *
 * Slot +0x54 (writeToReplayFrame) of cGMRaceHandler_vtbl. Quirk: the 208 bytes start at the object itself, so its vtable pointer
 * is written too.
 *
 * @param stream  the replay stream
 */
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
extern "C" void cGMRaceHandler_writeToReplayFrame(cGMRaceHandler* self, cBxReplayFrame* stream)
{
    stream->write((const char*)self, 208);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRaceHandler_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores the whole 208-byte timed event handler from a replay frame with the stream's read (its second virtual).
 *
 * Slot +0x5C (readFromReplayFrame) of cGMRaceHandler_vtbl. Quirk: the read covers the vtable pointer at +0x0 as well.
 *
 * @param stream  the replay stream
 */
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
extern "C" void cGMRaceHandler_readFromReplayFrame(cGMRaceHandler* self, cBxReplayFrame* stream)
{
    stream->read((char*)self, 208);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPRaceHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getPlayerCheatCharID(void* iface, int player);
extern "C" void func_0023B0A8(void* self);

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


/**
 * Starts the online race of handler 8 (vtable cGMMPRaceHandler_vtbl; online event 0 and the online default).
 *
 * Sets path 2 and event 0, a single last heat (3) with four AI riders (newRace.iNumberAI when cBENetworkInterface_networkOpt
 * says it is a network game), no time limit, fills the time table (+0x4) with 9s, runs slot +0x2C and stores both
 * players' characters. When the event was done it also runs func_0023B0A8.
 */
void cGMMPRaceHandler::init()
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
        cGameModeHandler_pManager->iNumberAI = cBENewRaceInterface_newRace.iNumberAI;
    }
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->iSimAI = 0;
    gm2->unk_80 = 0;
    gm2->uiTimeLimit = 0;
    gm2->uiPointTarget = 0;
    gm2->bHasPointTarget = 0;
    memset(&uiTime[0], 9, 40);
    cleanRiderRanksTime();
    cGameModeHandler_pManager->iCharacters[0] = cBENewPlayerInterface_getPlayerCharID(player, 0);
    cGameModeHandler_pManager->iCharacters[1] = cBENewPlayerInterface_getPlayerCharID(player, 1);
    cGameModeHandler_pManager->iCheatCharacters[0] = cBENewPlayerInterface_getPlayerCheatCharID(player, 0);
    cGameModeHandler_pManager->iCheatCharacters[1] = cBENewPlayerInterface_getPlayerCheatCharID(player, 1);
    if (cGameModeHandler_pManager->bIsEventDone == one) {
        func_0023B0A8(this);
    }
    cGameModeHandler_pManager->bIsEventDone = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPRaceHandler_raceFinish);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cRider_estimateFinishTick(void* self);
extern "C" void cGameModeMan_sortTimes(cGameModeMan* self, int n);

/**
 * Records a rider's time in the online race of handler 8 and ranks the riders when the race is over.
 *
 * While the race runs or for a human rider (not isComputer()), stores the rider's time (360000 for did-not-finish) in
 * uiTime and D_00536640. Once cAI_isRaceFinished, estimates the unfinished riders (cRider_estimateFinishTick), ranks
 * with sortTimes (cGameModeMan_sortTimes), clears ShouldCelebrateAtFinish of a rider placed 4th or worse and locks the
 * results. Slot +0x44 (raceFinish) of cGMMPRaceHandler_vtbl.
 *
 * @param idx  the rider index
 */
void cGMMPRaceHandler::raceFinish(int idx)
{
    cAI* rc = SSXApp->Game->AI;
    if (cAI_isRaceFinished(rc) == 0 || !rc->Riders[idx]->isComputer()) {
        if (SSXApp->Game->AI->Riders[idx]->mDidNotFinish) {
            D_00536640[idx] = uiTime[idx] = 360000;
            D_005366A8[idx] = 1;
        } else {
            int finishTick = rc->Riders[idx]->mFinishTick;
            uiTime[idx] = finishTick;
            D_00536640[idx] = finishTick;
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(rc)) {
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 2; i++) {
            int finished = rc->Riders[i]->mFinishTime >= 0.0f;
            if (!finished && i != idx) {
                uiTime[i] = cRider_estimateFinishTick(rc->Riders[i]);
            }
            D_00536640[i] = uiTime[i];
        }
        cGameModeHandler_pManager->sortTimes(10);
        if (cGameModeMan_iRanking[idx] >= 3) {
            SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 0;
        }
        cGameModeHandler_pManager->bAreResultsLocked = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPRaceHandler_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves the online race handler's first 44 bytes (from +0x0, vtable pointer included) to a replay frame with the
 * stream's write (its first virtual).
 *
 * Slot +0x54 (writeToReplayFrame) of cGMMPRaceHandler_vtbl.
 *
 * @param stream  the replay stream
 */
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
extern "C" void cGMMPRaceHandler_writeToReplayFrame(cGMMPRaceHandler* self, cBxReplayFrame* stream)
{
    stream->write((const char*)self, 44);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMMPRaceHandler_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores the online race handler's first 44 bytes (from +0x0, vtable pointer included) from a replay frame with the
 * stream's read (its second virtual).
 *
 * Slot +0x5C (readFromReplayFrame) of cGMMPRaceHandler_vtbl.
 *
 * @param stream  the replay stream
 */
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
extern "C" void cGMMPRaceHandler_readFromReplayFrame(cGMMPRaceHandler* self, cBxReplayFrame* stream)
{
    stream->read((char*)self, 44);
}
#endif

INCLUDE_ASM("main/gamemode", func_0023B0A8);

//100%
INCLUDE_ASM("main/gamemode", cGMFreerideHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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


/**
 * Starts a freeride (handler 2, vtable cGMFreerideHandler_vtbl; event 12): one last heat, no AI, no time limit, results locked.
 *
 * Clears iCharacters to -1 and gives the human rider its character, runs slot +0x24, sets iNextHeat and iRestoreHeat to
 * 1 and clears the time limit, point target and flags. Slot +0x14 (init) of cGMFreerideHandler_vtbl.
 */
extern "C" void cGMFreerideHandler_init(cGameModeHandler* self)
{
    int one = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = one;
    gm->bIsLastHeat = one;
    gm->iNumberAI = 0;
    gm->iSimAI = 0;
    memset(gm->iCharacters, -1, 40);
    int humanRider = cBENewPlayerInterface_getHumanRider(iface, 0);
    cGameModeHandler_pManager->iCharacters[humanRider] = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
    self->cleanRiderRanksPoints();
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
INCLUDE_ASM("main/gamemode", cGMTimeHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getPeakGoal(void* iface, int mode, int a2);
extern "C" int cBEScoreInterface_getBestMedalOnTrack(void* iface, int a1, int a2, int a3);
extern "C" void* memset(void* dst, int c, int n);

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

/**
 * Starts the timed event of kHANDLER_TIME (vtable cGMTimeHandler_vtbl; kGAMEEVENT_TIME_PEAK1-3) at its location.
 *
 * kGAMEEVENT_TIME_PEAK1, 2 or 3 maps to location 14, 15 or 16 (next locations 0, 3, 4). The medal already won there
 * (cBEScoreInterface_getBestMedalOnTrack) picks the target tier (+0xA: 2 for kMEDAL_GOLD or kMEDAL_SILVER, 1 for
 * kMEDAL_BRONZE, else 0); the time limit (+0x4 and uiTimeLimit) is cBENewRaceInterface_getPeakGoal(race, event, tier) *
 * 60 and also becomes the point target. Runs slot +0x2C, gives the human its character and clears the checkpoint
 * counter (+0x8). Slot +0x14 (init) of cGMTimeHandler_vtbl.
 */
void cGMTimeHandler::init()
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    int mode = cBENewRaceInterface_newRace.cGameEvent;
    gm->iNumberAI = 0;
    gm->iCurrentGameEvent = mode;
    gm->iSimAI = 0;
    gm->bHasPointTarget = 0;
    switch (mode) {
    case kGAMEEVENT_TIME_PEAK1:
        gm->iNextLocation = 0;
        gm->iLocation = 14;
        break;
    case kGAMEEVENT_TIME_PEAK2:
        gm->iLocation = 15;
        gm->iNextLocation = 3;
        break;
    case kGAMEEVENT_TIME_PEAK3:
        gm->iLocation = 16;
        gm->iNextLocation = 4;
        break;
    }
    int medalResult = cBEScoreInterface_getBestMedalOnTrack(score, 0, cGameModeHandler_pManager->iLocation, cGameModeHandler_pManager->iCurrentGameEvent);
    mCurrentDifficulty = 0;
    if (medalResult == kMEDAL_GOLD || medalResult == kMEDAL_SILVER) {
        mCurrentDifficulty = 2;
    } else if (medalResult == kMEDAL_BRONZE) {
        mCurrentDifficulty = 1;
    }
    int tierTime = cBENewRaceInterface_getPeakGoal(race, cGameModeHandler_pManager->iCurrentGameEvent,
            mCurrentDifficulty);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    uiTimeToBeat = tierTime * 60;
    gm2->bIsTimedEvent = 1;
    gm2->uiTimeLimit = (int)uiTimeToBeat;
    gm2->uiPointTarget = (int)uiTimeToBeat;
    gm2->iNextHeat = 2;
    gm2->iRestoreHeat = 2;
    cleanRiderRanksTime();
    memset(cGameModeHandler_pManager->iCharacters, -1, 40);
    int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
    cGameModeHandler_pManager->iCharacters[humanRider] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    cGameModeMan* gm3 = cGameModeHandler_pManager;
    gm3->unk_80 = 0;
    gm3->bIsEventDone = 0;
    gm3->unk_90 = 0;
    gm3->unk_94 = 0;
    gm3->bIsLastHeat = 0;
    gm3->bAreResultsLocked = 0;
    uiTrackProgress = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMTimeHandler_raceFinish);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cBENewRaceInterface_getPeakGoal(void* iface, int mode, int a2);
extern int cGameModeMan_iRanking[];
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

/**
 * Records the rider's time in the timed event of handler 4 and awards a place by the medal times.
 *
 * Copies the rider's finish tick (+0x478) to D_00536640; did-not-finish stores 0x57E40 and retries the heat. A time
 * above the limit (+0x4) also retries; otherwise the event is done and the rank is 0, 1 or 2 against the tier-2 and
 * tier-1 times (cBENewRaceInterface_getPeakGoal * 60). Always locks the results. Slot +0x44 (raceFinish) of cGMTimeHandler_vtbl.
 *
 * @param riderIndex  the rider index
 */
void cGMTimeHandler::raceFinish(int riderIndex)
{
    int one = 1;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cSSXApp* ssxApp = SSXApp;
    cGameModeMan* gm = cGameModeHandler_pManager;
    unsigned int* timeSlot = &D_00536640[riderIndex];
    *timeSlot = ssxApp->Game->AI->Riders[riderIndex]->mFinishTick;
    gm->bAreResultsLocked = one;
    cRider* rider = ssxApp->Game->AI->Riders[riderIndex];
    if (rider->mDidNotFinish != 0) {
        *timeSlot = 0x57E40;
        ssxApp->Game->AI->Riders[riderIndex]->ShouldCelebrateAtFinish = 0;
        gm->iNextHeat = one;
        D_005366A8[riderIndex] = one;
    } else if (uiTimeToBeat < *timeSlot) {
        rider->ShouldCelebrateAtFinish = 0;
        gm->iNextHeat = one;
        D_005366A8[riderIndex] = 0;
    } else {
        rider->ShouldCelebrateAtFinish = one;
        gm->bIsEventDone = one;
        unsigned int tier2Time = cBENewRaceInterface_getPeakGoal(race, gm->iCurrentGameEvent, 2) * 60;
        unsigned int tier1Time = cBENewRaceInterface_getPeakGoal(race, cGameModeHandler_pManager->iCurrentGameEvent, 1) * 60;
        if (tier2Time >= *timeSlot) {
            cGameModeMan_iRanking[riderIndex] = 0;
        } else if (tier1Time >= *timeSlot) {
            cGameModeMan_iRanking[riderIndex] = one;
        } else {
            cGameModeMan_iRanking[riderIndex] = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMTimeHandler_riderCheckpointHit);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int cBENewRaceInterface_getPeakSplitTimePoints(void* iface, int a1, int a2, int a3);
extern "C" float cRiderMetrix_evSplitTime(void* self, int value);

/**
 * Handles a checkpoint in the timed event of handler 4: at locations 17-21 subtracts that checkpoint's bonus from the
 * race clock.
 *
 * Asks cBENewRaceInterface_getPeakSplitTimePoints for the bonus of checkpoint +0x8 (then advances it) at the target tier (+0xA) and, when non-zero,
 * calls cRiderMetrix_evSplitTime on the first rider's object (+0x790) with the clock in seconds minus the bonus. Slot
 * +0x34 (riderCheckpointHit) of cGMTimeHandler_vtbl.
 *
 * @return 1
 */
extern "C" int cGMTimeHandler_riderCheckpointHit(cGMTimeHandler* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int state = cBENewRaceInterface_getLocationID(iface);
    int ok = 0;
    int aboveMinLocation = state >= 17;
    if (aboveMinLocation) {
        ok = state;
        ok = ok < 22;
    }
    if (ok) {
        int bonus = cBENewRaceInterface_getPeakSplitTimePoints(iface, cBENewRaceInterface_newRace.cGameEvent, self->mCurrentDifficulty, self->uiTrackProgress);
        cAI* clk = SSXApp->Game->AI;
        int secs = (int)((float)clk->EventTimerTick * 0.01666666753590107f);
        if (bonus != 0) {
            cRiderMetrix_evSplitTime(clk->Riders[0]->mMetrix, secs - bonus);
        }
        (self->uiTrackProgress)++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRivalTimeHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int player);
extern "C" void* memset(void* dst, int c, int n);

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


/**
 * Starts the head-to-head event of kHANDLER_RIVALTIME (vtable cGMRivalTimeHandler_vtbl; kGAMEEVENT_RIVALTIME): one last
 * heat against one AI rider.
 *
 * Runs slot +0x2C. Online (kGAMEPATH_MULTIPLAYER) both players take their characters and no AI; offline the human gets
 * its character and the next slot gets the rival (cBENewRaceInterface_getRival). Slot +0x14 (init) of
 * cGMRivalTimeHandler_vtbl.
 */
extern "C" void cGMRivalTimeHandler_init(cGameModeHandler* self)
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
    self->cleanRiderRanksTime();
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        cGameModeHandler_pManager->iNumberAI = 0;
        int firstHuman = cBENewPlayerInterface_getHumanRider(player, 0);
        int secondHuman = cBENewPlayerInterface_getHumanRider(player, 1);
        cGameModeHandler_pManager->iCharacters[firstHuman] = cBENewPlayerInterface_getRiderCharID(player, firstHuman);
        cGameModeHandler_pManager->iCheatCharacters[firstHuman] = cBENewPlayerInterface_getRiderCheatCharID(player, firstHuman);
        cGameModeHandler_pManager->iCharacters[secondHuman] = cBENewPlayerInterface_getRiderCharID(player, secondHuman);
        cGameModeHandler_pManager->iCheatCharacters[secondHuman] = cBENewPlayerInterface_getRiderCheatCharID(player, secondHuman);
    } else {
        memset(cGameModeHandler_pManager->iCharacters, -1, 40);
        int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeHandler_pManager->iCharacters[humanRider] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
        cGameModeHandler_pManager->iCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = cBENewRaceInterface_getRival(race);
        cGameModeHandler_pManager->iCheatCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRivalTimeHandler_raceFinish);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishTick(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cGameModeMan_sortTimes(cGameModeMan* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];



/**
 * Records a rider's time in the head-to-head event of kHANDLER_RIVALTIME and decides the winner when the race is over.
 *
 * Stores the finish tick (0x57E40 for did-not-finish) while the race runs or for a human rider (not isComputer());
 * the rest runs only for a human (isHuman()) once the race is over. Online it ranks with sortTimes and returns early
 * for the faster player. Offline it estimates unfinished riders, ranks with sortTimes, and a human in first place that
 * finished ends the event (iNextHeat 0); otherwise the rider's ShouldCelebrateAtFinish is cleared. Slot +0x44
 * (raceFinish) of cGMRivalTimeHandler_vtbl.
 *
 * @param idx  the rider index
 */
extern "C" void cGMRivalTimeHandler_raceFinish(void* self, int idx)
{
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0 ||
        !SSXApp->Game->AI->Riders[idx]->isComputer()) {
        cRider* rider = SSXApp->Game->AI->Riders[idx];
        if (rider->mDidNotFinish != 0) {
            D_00536640[idx] = 0x57E40;
            D_005366A8[idx] = 1;
        } else {
            D_00536640[idx] = rider->mFinishTick;
            D_005366A8[idx] = 0;
        }
    }
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0) {
        return;
    }
    if (!SSXApp->Game->AI->Riders[idx]->isHuman()) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        cGameModeHandler_pManager->sortTimes(10);
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
            cRider* rider = SSXApp->Game->AI->Riders[i];
            int done = rider->mFinishTime >= 0.0f;
            if (!done) {
                D_00536640[i] = cRider_estimateFinishTick(rider);
            }
        }
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        int n = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bAreResultsLocked = 1;
        gm->sortTimes(10);
        if (cGameModeMan_iRanking[n] == 0 && SSXApp->Game->AI->Riders[idx]->mDidNotFinish == 0) {
            SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 1;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            gm2->bIsEventDone = 1;
            gm2->iRestoreHeat = gm2->iNextHeat;
            gm2->iNextHeat = 0;
            return;
        }
    }
    SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRivalPointHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int player);
extern "C" int cBENewRaceInterface_getTimeLimit(void* iface, int a1);
extern "C" void* memset(void* dst, int c, int n);

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


/**
 * Starts the head-to-head scored event of kHANDLER_RIVALPOINT (vtable cGMRivalPointHandler_vtbl;
 * kGAMEEVENT_RIVALPOINTS): one timed last heat against one AI rider.
 *
 * The time limit is cBENewRaceInterface_getTimeLimit(race, 1) * 60. Runs slot +0x24; online both players take their characters, offline
 * the human and the rival (cBENewRaceInterface_getRival). Slot +0x14 (init) of cGMRivalPointHandler_vtbl.
 */
extern "C" void cGMRivalPointHandler_init(cGameModeHandler* self)
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
    int heatTimeLimit = cBENewRaceInterface_getTimeLimit(race, 1);
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    gm2->bAreResultsLocked = 0;
    gm2->uiPointTarget = 0;
    gm2->uiTimeLimit = heatTimeLimit * 60;
    gm2->unk_80 = 0;
    gm2->bIsEventDone = 0;
    gm2->bIsTimedEvent = one;
    gm2->bHasPointTarget = 0;
    gm2->unk_90 = 0;
    gm2->unk_94 = 0;
    gm2->bIsLastHeat = one;
    self->cleanRiderRanksPoints();
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        cGameModeHandler_pManager->iNumberAI = 0;
        int firstHuman = cBENewPlayerInterface_getHumanRider(player, 0);
        int secondHuman = cBENewPlayerInterface_getHumanRider(player, 1);
        cGameModeHandler_pManager->iCharacters[firstHuman] = cBENewPlayerInterface_getRiderCharID(player, firstHuman);
        cGameModeHandler_pManager->iCheatCharacters[firstHuman] = cBENewPlayerInterface_getRiderCheatCharID(player, firstHuman);
        cGameModeHandler_pManager->iCharacters[secondHuman] = cBENewPlayerInterface_getRiderCharID(player, secondHuman);
        cGameModeHandler_pManager->iCheatCharacters[secondHuman] = cBENewPlayerInterface_getRiderCheatCharID(player, secondHuman);
    } else {
        memset(cGameModeHandler_pManager->iCharacters, -1, 40);
        int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeHandler_pManager->iCharacters[humanRider] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
        cGameModeHandler_pManager->iCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = cBENewRaceInterface_getRival(race);
        cGameModeHandler_pManager->iCheatCharacters[cBENewPlayerInterface_getHumanRider(player, 0) + 1] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMRivalPointHandler_raceFinish);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cRider_estimateFinishScore(void* self);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cGameModeMan_sortScores(cGameModeMan* self, int n);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];




/**
 * Records a rider's score in the head-to-head scored event of kHANDLER_RIVALPOINT and decides the winner when the race
 * is over.
 *
 * Stores the score (0 for did-not-finish). Online it ranks the two players with sortScores and clears
 * ShouldCelebrateAtFinish of the loser; offline it estimates unfinished riders' scores, ranks, and a human in first
 * place that finished ends the event (iNextHeat 0). Slot +0x44 (raceFinish) of cGMRivalPointHandler_vtbl.
 *
 * @param idx  the rider index
 */
extern "C" void cGMRivalPointHandler_raceFinish(void* self, int idx)
{
    cRider* rider = SSXApp->Game->AI->Riders[idx];
    if (rider->mDidNotFinish != 0) {
        D_00536640[idx] = 0;
        D_005366A8[idx] = 1;
    } else {
        tRiderStats st = *(&rider->mMetrix->Totals);
        D_005366A8[idx] = 0;
        D_00536640[idx] = st.TotalPoints;
    }
    if (cAI_isRaceFinished(SSXApp->Game->AI) == 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        cGameModeHandler_pManager->sortScores(2);
        cGameModeHandler_pManager->bAreResultsLocked = 1;
        int w = 1;
        if (D_00536640[0] > D_00536640[1]) {
            w = 0;
        }
        if (idx == w) {
            return;
        }
        SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 0;
    } else {
        for (int i = 0; i < cGameModeHandler_pManager->iNumberAI + 1; i++) {
            cRider* rider = SSXApp->Game->AI->Riders[i];
            int done = rider->mFinishTime >= 0.0f;
            if (!done) {
                D_00536640[i] = cRider_estimateFinishScore(rider);
            }
        }
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        int n = cBENewPlayerInterface_getHumanRider(player, 0);
        cGameModeMan* gm = cGameModeHandler_pManager;
        gm->bAreResultsLocked = 1;
        gm->sortScores(2);
        if (cGameModeMan_iRanking[n] == 0 && SSXApp->Game->AI->Riders[idx]->mDidNotFinish == 0) {
            SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 1;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            gm2->bIsEventDone = 1;
            gm2->iRestoreHeat = gm2->iNextHeat;
            gm2->iNextHeat = 0;
            return;
        }
        SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMPointHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" int cBENewRaceInterface_getPeakGoal(void* iface, int mode, int a2);
extern "C" int cBENewRaceInterface_getPeakTimeLimit(void* iface, int mode, int a2);
extern "C" int cBEScoreInterface_getBestMedalOnTrack(void* iface, int a1, int a2, int a3);
extern "C" void* memset(void* dst, int c, int n);

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

/**
 * Starts the point-target event of kHANDLER_POINT (vtable cGMPointHandler_vtbl; kGAMEEVENT_POINTS_PEAK1-3) at its
 * location.
 *
 * kGAMEEVENT_POINTS_PEAK1, 2 or 3 maps to location 14, 15 or 16 (next locations 5, 6, 4). The medal already won
 * (cBEScoreInterface_getBestMedalOnTrack) picks the tier (+0xA); the point target (+0x4) is
 * cBENewRaceInterface_getPeakGoal(race, event, tier) * 100 and the time limit cBENewRaceInterface_getPeakTimeLimit(..)
 * * 60. Runs slot +0x24, gives the human its character and clears the checkpoint counter (+0x8). Slot +0x14 (init) of
 * cGMPointHandler_vtbl.
 */
void cGMPointHandler::init()
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    int mode = cBENewRaceInterface_newRace.cGameEvent;
    gm->iNumberAI = 0;
    gm->iCurrentGameEvent = mode;
    gm->iSimAI = 0;
    switch (mode) {
    case kGAMEEVENT_POINTS_PEAK1:
        gm->iLocation = 14;
        gm->iNextLocation = 5;
        break;
    case kGAMEEVENT_POINTS_PEAK2:
        gm->iLocation = 15;
        gm->iNextLocation = 6;
        break;
    case kGAMEEVENT_POINTS_PEAK3:
        gm->iLocation = 16;
        gm->iNextLocation = 4;
        break;
    }
    unsigned int medalResult = (unsigned int)cBEScoreInterface_getBestMedalOnTrack(score, 0, cGameModeHandler_pManager->iLocation, cGameModeHandler_pManager->iCurrentGameEvent);
    mCurrentDifficulty = 0;
    if (medalResult < 3) {
        mCurrentDifficulty = 2;
    } else if (medalResult == kMEDAL_BRONZE) {
        mCurrentDifficulty = 1;
    }
    int pointTarget = cBENewRaceInterface_getPeakGoal(race, cGameModeHandler_pManager->iCurrentGameEvent,
            mCurrentDifficulty) * 100;
    cGameModeMan* gm2 = cGameModeHandler_pManager;
    uiScoreToBeat = pointTarget;
    gm2->uiPointTarget = pointTarget;
    int heatTimeLimit = cBENewRaceInterface_getPeakTimeLimit(race, gm2->iCurrentGameEvent, mCurrentDifficulty);
    cGameModeMan* gm3 = cGameModeHandler_pManager;
    gm3->iNextHeat = 2;
    gm3->iRestoreHeat = 2;
    gm3->uiTimeLimit = heatTimeLimit * 60;
    cleanRiderRanksPoints();
    memset(cGameModeHandler_pManager->iCharacters, -1, 40);
    int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
    cGameModeHandler_pManager->iCharacters[humanRider] = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    cGameModeMan* gm4 = cGameModeHandler_pManager;
    gm4->unk_80 = 0;
    gm4->bIsEventDone = 0;
    gm4->bIsTimedEvent = 1;
    gm4->bHasPointTarget = 1;
    gm4->unk_90 = 0;
    gm4->unk_94 = 0;
    gm4->bIsLastHeat = 0;
    gm4->bAreResultsLocked = 0;
    uiTrackProgress = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMPointHandler_raceFinish);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cBENewRaceInterface_getPeakGoal(void* iface, int mode, int a2);
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];

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


/**
 * Records the rider's score in the point-target event of handler 7 and awards a place by the medal scores.
 *
 * Did-not-finish or a score below uiPointTarget retries the heat; otherwise the event is done and the rank is 0, 1 or 2
 * against the tier-2 and tier-1 targets (cBENewRaceInterface_getPeakGoal * 100). Always locks the results. Slot +0x44 (raceFinish) of
 * cGMPointHandler_vtbl.
 *
 * @param idx  the rider index
 */
void cGMPointHandler::raceFinish(int idx)
{
    cAI* ai = SSXApp->Game->AI;
    tRiderStats st = ai->Riders[idx]->mMetrix->Totals;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cRider* rider = SSXApp->Game->AI->Riders[idx];
    if (rider->mDidNotFinish != 0) {
        D_00536640[idx] = 0;
        cGameModeHandler_pManager->iNextHeat = 1;
        SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 0;
        D_005366A8[idx] = 1;
    } else {
        cGameModeMan* gm = cGameModeHandler_pManager;
        if (st.TotalPoints < (int)gm->uiPointTarget) {
            rider->ShouldCelebrateAtFinish = 0;
            D_00536640[idx] = st.TotalPoints;
            gm->iNextHeat = 1;
            D_005366A8[idx] = 0;
        } else {
            int tier2Target = cBENewRaceInterface_getPeakGoal(race, gm->iCurrentGameEvent, 2) * 100;
            int tier1Target = cBENewRaceInterface_getPeakGoal(race, cGameModeHandler_pManager->iCurrentGameEvent, 1) * 100;
            cGameModeMan* gm2 = cGameModeHandler_pManager;
            SSXApp->Game->AI->Riders[idx]->ShouldCelebrateAtFinish = 1;
            D_00536640[idx] = st.TotalPoints;
            gm2->bIsEventDone = 1;
            if (st.TotalPoints >= tier2Target) {
                cGameModeMan_iRanking[idx] = 0;
            } else if (st.TotalPoints >= tier1Target) {
                cGameModeMan_iRanking[idx] = 1;
            } else {
                cGameModeMan_iRanking[idx] = 2;
            }
        }
    }
    cGameModeHandler_pManager->bAreResultsLocked = 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMPointHandler_riderCheckpointHit);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int cBENewRaceInterface_getPeakSplitTimePoints(void* iface, int a1, int a2, int a3);
extern "C" float cRiderMetrix_evSplitPoints(void* self, int value);

/**
 * Handles a checkpoint in the point-target event of handler 7: at locations 17-21 subtracts that checkpoint's bonus
 * points from the first rider's score.
 *
 * Asks cBENewRaceInterface_getPeakSplitTimePoints for the bonus of checkpoint +0x8 (then advances it) at the tier (+0xA), times 100, and calls
 * cRiderMetrix_evSplitPoints on the first rider's object (+0x790) with its score (+0x198) minus the bonus. Slot +0x34
 * (riderCheckpointHit) of cGMPointHandler_vtbl.
 *
 * @return 1
 */
extern "C" int cGMPointHandler_riderCheckpointHit(cGMPointHandler* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int state = cBENewRaceInterface_getLocationID(iface);
    int ok = 0;
    int aboveMinLocation = state >= 17;
    if (aboveMinLocation) {
        ok = state;
        ok = ok < 22;
    }
    if (ok) {
        int bonusPoints = cBENewRaceInterface_getPeakSplitTimePoints(iface, cBENewRaceInterface_newRace.cGameEvent, self->mCurrentDifficulty, self->uiTrackProgress) * 100;
        cRiderMetrix* riderMetrix = SSXApp->Game->AI->Riders[0]->mMetrix;
        int curScore = riderMetrix->Totals.TotalPoints;
        if (bonusPoints != 0) {
            cRiderMetrix_evSplitPoints(riderMetrix, curScore - bonusPoints);
        }
        (self->uiTrackProgress)++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamemode", cGMDebugHandler_init);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getRiderCharID(void* iface, int player);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void func_0023C770(int* arr, unsigned int n);

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


/**
 * Starts a single-heat event with newRace.iNumberAI AI riders in a random line-up.
 *
 * Puts the human rider's character in its slot and fills the others from a shuffled 0-9 list (func_0023C770) without
 * repeating the player's character, runs slot +0x24 and clears the time limit, point target and flags. Slot +0x14 of
 * the handler vtable at cMCOverlayManager_vtbl + 0x20.
 */
extern "C" void cGMDebugHandler_init(cGameModeHandler* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = cGameModeHandler_pManager;
    gm->iHeatNum = 1;
    gm->bIsLastHeat = 1;
    gm->iNumberAI = cBENewRaceInterface_newRace.iNumberAI;
    gm->iSimAI = 0;
    int humanRider = cBENewPlayerInterface_getHumanRider(player, 0);
    int ch = cBENewPlayerInterface_getRiderCharID(player, humanRider);
    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    func_0023C770(arr, 10);
    int j = 0;
    int* src = arr;
    for (; j < 10; j++) {
        if (j != humanRider) {
            int charId = *src;
            if (charId == ch) {
                src++;
                charId = *src;
            }
            cGameModeHandler_pManager->iCharacters[j] = charId;
            src++;
        } else {
            cGameModeHandler_pManager->iCharacters[j] = ch;
        }
    }
    self->cleanRiderRanksPoints();
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

/**
 * Shuffles an int array: 25 swaps of two random entries (GMrand() % @p n each).
 *
 * @param arr  the array
 * @param n  number of entries
 */
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

