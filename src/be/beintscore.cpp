#include "common.h"
#include "ai/ridermetrix.h"
#include "be/bedbprofile.h"
#include "vtable.h"
#include "be/beinterface.h"
#include "be/beintscore.h"
#include "be/beintnewrace.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cBEScoreInterface_cBEScoreInterface(void* self);
extern const char str_BEScoreInt[];


//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getThis__Fv);
#ifdef SKIP_ASM
// PORT: the unit declares cBEScoreInterface_cBEScoreInterface as returning void; the body returns self.
void* cBEScoreInterface_cBEScoreInterface_impl(void* self) __asm__("cBEScoreInterface_cBEScoreInterface");

/**
 * Returns the back end's score interface singleton, creating it on first use.
 *
 * The first call allocates 0x230 bytes tagged "BEScoreInt", runs the constructor, clears bIsActive (+0x8) and
 * the words at +0x10 and +0x14, and sets +0x18 and +0x1C to -1.
 *
 * @return the score interface (cBEScoreInterface_instance)
 */
void* cBEScoreInterface_getThis()
{
    if (cBEScoreInterface_instance == 0) {
        void* mem = cMemMan_alloc(560, str_BEScoreInt, 0, 0);
        cBEScoreInterface* scoreIface = (cBEScoreInterface*)cBEScoreInterface_cBEScoreInterface_impl(mem);
        cBEScoreInterface_instance = (cBEScoreInterface*)(scoreIface);
        scoreIface->bIsActive = 0;
        scoreIface->bNewHighScore[0] = 0;
        scoreIface->bNewHighScore[1] = 0;
        scoreIface->mNewTrackHighScoreIndex[0] = -1;
        scoreIface->mNewTrackHighScoreIndex[1] = -1;
    }
    return cBEScoreInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_cBEScoreInterface);
#ifdef SKIP_ASM
extern "C" void cBEScoreInterface_discard(cBEScoreInterface* self);
extern void* cBEScoreInterface_vtbl[];

// PORT: the unit declares cBEScoreInterface_cBEScoreInterface as returning void (used by getThis);
// the body returns self, so it is bound to the symbol with an asm label.
void* cBEScoreInterface_cBEScoreInterface_impl(void* self) __asm__("cBEScoreInterface_cBEScoreInterface");

/**
 * Constructs the back end's score interface: stores its vtable at +0xC and resets its score state.
 *
 * The reset, cBEScoreInterface_discard(), zeroes the words at +0x10 and +0x14, sets +0x18 and +0x1C to -1, and clears both
 * 0xB0-byte slots at +0x20, setting each slot's last word to -1.
 *
 * @return @p self
 */
void* cBEScoreInterface_cBEScoreInterface_impl(void* vself)
{
    cBEScoreInterface* self = (cBEScoreInterface*)vself;
    VPTR_OF(cBEInterface, self) = cBEScoreInterface_vtbl;
    cBEScoreInterface_discard((cBEScoreInterface*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Submits the score interface's pending changes: nothing to do in this build.
 *
 * The first virtual of the back-end interfaces (submit, discard, writeToReplayFrame, readFromReplayFrame). On Tour's
 * version only asserts that the score interface is active (instance->bIsActive); without asserts the body is empty.
 */
void cBEScoreInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_discard);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Clears all of the score interface's recorded results.
 *
 * bNewHighScore[0..1] become 0 and mNewTrackHighScoreIndex[0..1] -1; the two human profiles' 0xB0-byte
 * LastEventStats slots are zeroed and each slot's rank set to -1.
 *
 * @param scoreIface  the score interface (this)
 */
extern "C" void cBEScoreInterface_discard(cBEScoreInterface* scoreIface)
{
    cBEScoreInterface* self = scoreIface;
    self->bNewHighScore[0] = 0;
    self->bNewHighScore[1] = 0;
    self->mNewTrackHighScoreIndex[0] = -1;
    self->mNewTrackHighScoreIndex[1] = -1;
    MEM_fill(self->LastEventStats, 0, 2 * sizeof(tLastEventRiderStats)); // the two human profiles' slots
    for (int i = 1; i >= 0; i--)
    {
        self->LastEventStats[i].rank = -1;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_processRiderStats);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void cBEScoreInterface_processRiderBest(int rider, int a, int b, int c);
extern signed char D_005305F9[];

/**
 * Records a rider's race statistics in its character's best records, for the two human profiles only.
 *
 * Unless raceDB byte +0x49 (D_005305F9) is set, calls cBEScoreInterface_processRiderBest(@p rider, @p riderStats, -1, -1) when the rider's profile
 * (cBELibrary_getProfileIndex) is 0 or 1. The AI code passes each rider's metrix stats block (metrix + 0xFC);
 * cBEScoreInterface_processEventRiderStats passes the event stats.
 *
 * @param rider  race rider slot
 * @param riderStats  the rider stats block, passed as an int
 */
extern "C" void cBEScoreInterface_processRiderStats(int rider, int riderStats)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (D_005305F9[0] == 0 && profile < 2)
    {
        cBEScoreInterface_processRiderBest(rider, riderStats, -1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_processEventRiderStats);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cBEScoreInterface_isEligibleForHighScore(cBEScoreInterface* self, int rider);
extern "C" char* cBENewPlayerInterface_getPlayerName(void* self, int idx);
extern "C" char* strcpy(char* dst, const char* src);

extern "C" int cBECharProfileDB_updateScoreStats(cBECharProfileDB* self, int event, int mode, unsigned int value);
extern cBECharProfileDB cBELibrary_profileLib[][10];

tBEHighScore* cBELibrary_getHighScoreTable_154AB8(int event, int mode) __asm__("cBELibrary_getHighScoreTable");

extern cBERaceDB cBELibrary_raceDB_154AB8 __asm__("cBELibrary_raceDB");
struct sEvent_154AB8 {
    int id;
    char pad_0x4[0x60];
};
extern sEvent_154AB8 cBELibrary_locationDB_154AB8[] __asm__("cBELibrary_locationDB");

/**
 * Records a human rider's finished-event stats and score: keeps the stats, enters the score in the event's top-5 table,
 * and updates the character's best records.
 *
 * Only profiles 0 and 1. Copies @p stats into the profile's LastEventStats slot with rank -1, passes them to
 * cBEScoreInterface_processRiderStats, and resets the profile's mNewTrackHighScoreIndex to -1 and bNewHighScore to 0.
 * The location comes from cBELibrary_locationDB[raceDB +0x40].id (22 becomes 0) and the event from
 * cBENewRaceInterface_newRace.cGameEvent. Race kinds 0 and 5 (raceDB +0x48) are timed: @p score is converted from ticks
 * to seconds.
 *
 * When cBEScoreInterface_isEligibleForHighScore allows it, the score goes into the 5-entry table from cBELibrary_getHighScoreTable (lower is better when timed):
 * later entries shift down, the new one gets the character, score and player name
 * (cBENewPlayerInterface_getPlayerName), and with two humans the other profile's rank moves down (dropping off at 5).
 * Then cBEScoreInterface_processRiderBest gets the score as the time (timed) or the points, and unless raceDB +0x49 is set, the best flag is
 * cBECharProfileDB_updateScoreStats's result for the character record.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param stats  the rider's event stats, 0xAC bytes (On Tour pRiderStats)
 * @param score  the event score or time in ticks (On Tour uiPoints)
 */
void cBEScoreInterface::processEventRiderStats(int rider, tRiderStats* stats, unsigned int score)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (profile >= 2)
        return;
    LastEventStats[profile].ridingStats = *stats;
    int mode;
    cBECharProfileDB* pc;
    int timed = 0;
    LastEventStats[profile].rank = -1;
    cBEScoreInterface_processRiderStats(rider, (int)stats);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int charID = cBELibrary_getCharacterID(rider);
    mode = cBENewRaceInterface_newRace.cGameEvent;
    int event = cBELibrary_locationDB_154AB8[cBELibrary_raceDB_154AB8.iLocationIdx].id;
    pc = &cBELibrary_profileLib[profile][charID];
    bNewHighScore[profile] = 0;
    mNewTrackHighScoreIndex[profile] = -1;
    if (event == 22)
        event = 0;
    if (cBELibrary_raceDB_154AB8.cGameMode == 0 || cBELibrary_raceDB_154AB8.cGameMode == 5) {
        timed = 1;
        score = (int)(score * (1.0f / 60.0f));
    }
    if (isEligibleForHighScore(profile)) {
        tBEHighScore* highScores = cBELibrary_getHighScoreTable_154AB8(event, mode);
        if (highScores != 0) {
            for (int i = 0; i < 5; i++) {
                int better;
                if (timed)
                    better = highScores[i].uiHighScore < score;
                else
                    better = score < highScores[i].uiHighScore;
                if (!better) {
                    for (int j = 4; j > i; j--)
                        highScores[j] = highScores[j - 1];
                    highScores[i].cCharacter = charID;
                    highScores[i].uiHighScore = score;
                    strcpy(highScores[i].strUserName, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), rider));
                    mNewTrackHighScoreIndex[profile] = i;
                    if (cBENewRaceInterface_newRace.iNumberHumans >= 2) {
                        if (mNewTrackHighScoreIndex[1 - profile] >= i) {
                            mNewTrackHighScoreIndex[1 - profile]++;
                            if (mNewTrackHighScoreIndex[1 - profile] >= 5)
                                mNewTrackHighScoreIndex[1 - profile] = -1;
                        }
                    }
                    break;
                }
            }
        }
    }
    if (timed)
        cBEScoreInterface_processRiderBest(rider, (int)stats, -1, score);
    else
        cBEScoreInterface_processRiderBest(rider, (int)stats, score, -1);
    if (cBELibrary_raceDB_154AB8.cGamePath == 0)
        bNewHighScore[profile] = cBECharProfileDB_updateScoreStats(pc, event, mode, score);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_processRiderRanking);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
extern "C" int cBEScoreInterface_convertRankToMedal(cBEScoreInterface* self, int a);
extern "C" void func_001E25D8(int mode, int event, int result);
extern "C" void func_001E1EB8(void);
extern "C" void func_001E1550(void);
extern "C" int cBENewRaceInterface_getPlatinumGoal(void* self, int idx, int mode);
extern "C" void cBERewardInterface_calculateUserTrackMedalRewards(void* iface, int profile, int charID, int mode, int event, int medal);

extern cBERaceDB cBELibrary_raceDB;

struct sEvent_154EE8 {
    int id;
    char pad_0x4[0x60];
};
extern sEvent_154EE8 cBELibrary_locationDB[];

/**
 * Handles a human rider's finishing rank: stores it, converts it to a medal, and passes the medal to the reward
 * interface.
 *
 * Only profiles 0 and 1. Stores @p result at +0xCC + profile * 0xB0 (the last-event slot's rank). The medal is
 * cBEScoreInterface_convertRankToMedal(@p result); events 4 and 5 (newRace.cGameEvent) give 1 for first place, else none.
 * Unless raceDB +0x49 is set it updates the event popups (func_001E25D8, then func_001E1EB8 and, outside events 4 and
 * 5, func_001E1550).
 *
 * With a medal, unless raceDB +0x49 is set or raceDB +0x48 is 4: when @p score meets or beats the event's target from
 * cBENewRaceInterface_getPlatinumGoal (time target * 60 for events 0 and 4, lower is better; a score target otherwise) the medal becomes 0
 * (the best). Then calls cBERewardInterface_calculateUserTrackMedalRewards on the reward interface.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param result  the finishing rank, 0 first (On Tour iRanking)
 * @param score  the event score or time in ticks (On Tour uiPoints)
 */
void cBEScoreInterface::processRiderRanking(int rider, int result, int score)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (profile >= 2)
        return;
    LastEventStats[profile].rank = result;
    void* be = cBE_getInterface_Fv(cBE_getBE(), 0);
    int charID = cBELibrary_getCharacterID(rider);
    int mode = cBENewRaceInterface_newRace.cGameEvent;
    int event = cBELibrary_locationDB[cBELibrary_raceDB.iLocationIdx].id;
    if (event == 22)
        event = 0;
    int medal = convertRankToMedal(result);
    if (mode == 4 || mode == 5)
        medal = result == 0 ? 1 : -1;
    if (cBELibrary_raceDB.cGamePath == 0)
        func_001E25D8(mode, event, result);
    if (medal == -1)
        return;
    if (cBELibrary_raceDB.cGamePath == 0) {
        func_001E1EB8();
        if (cBELibrary_raceDB.cGamePath == 0 && mode != 4 && mode != 5)
            func_001E1550();
    }
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 13);
    if (cBELibrary_raceDB.cGamePath != 0)
        return;
    if (cBELibrary_raceDB.cGameMode == 4)
        return;
    int best = cBENewRaceInterface_getPlatinumGoal(be, event, mode);
    if (best != -1) {
        if (mode == 0 || mode == 4) {
            if (best * 60 >= score)
                medal = 0;
        } else {
            if (score >= best)
                medal = 0;
        }
    }
    cBERewardInterface_calculateUserTrackMedalRewards(reward, profile, charID, mode, event, medal);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_001550E8);
#ifdef SKIP_ASM
extern "C" tBEHighScore* cBELibrary_getHighScoreTable(int a, int b);

/**
 * Returns the score of one entry in an event's high-score table.
 *
 * The table is cBELibrary_getHighScoreTable(@p event, @p mode); entries are 0x14 bytes with the score first.
 *
 * @param i  entry index, 0 (best) to 4
 * @param event  the location
 * @param mode  the game event
 * @return the score, or 0 when the event has no table
 */
extern "C" int func_001550E8(cBEScoreInterface* self, int i, int event, int mode)
{
    tBEHighScore* highScores = cBELibrary_getHighScoreTable(event, mode);
    if (highScores != 0) {
        return ((int)highScores[i].uiHighScore);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155130);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int D_005305F0[];

/**
 * Returns the score of one entry in the current race's high-score table.
 *
 * The table is cBELibrary_getHighScoreTable(raceDB +0x40, newRace.cGameEvent), the race's location index and game event. The pause and lodge
 * screens show entry 0 as the record.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the score, or 0 when the event has no table
 */
extern "C" int func_00155130(cBEScoreInterface* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    tBEHighScore* highScores = cBELibrary_getHighScoreTable(D_005305F0[0], cBENewRaceInterface_newRace.cGameEvent);
    if (highScores != 0)
    {
        return ((int)highScores[i].uiHighScore);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155190);
#ifdef SKIP_ASM
struct sScoreSlot_00155190
{
    char data[0x88];
};
extern sScoreSlot_00155190 D_00530990[];
extern void* D_004A1244;

/**
 * Returns the character record (D_00530990, 0x88 bytes each) of the rider who set one entry of the current race's
 * high-score table.
 *
 * The table is cBELibrary_getHighScoreTable(raceDB +0x40, newRace.cGameEvent); the entry's second word is the character ID. The lodge shows
 * the result as text next to the entry.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the character record, or D_004A1244 when the event has no table
 */
extern "C" void* func_00155190(cBEScoreInterface* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    tBEHighScore* highScores = cBELibrary_getHighScoreTable(D_005305F0[0], cBENewRaceInterface_newRace.cGameEvent);
    if (highScores == 0)
        return D_004A1244;
    return &D_00530990[highScores[i].cCharacter];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155208);
#ifdef SKIP_ASM
extern void* D_004A1244;

/**
 * Returns the player name stored in one entry of the current race's high-score table.
 *
 * The table is cBELibrary_getHighScoreTable(raceDB +0x40, newRace.cGameEvent); the 12-byte name is at +0x8 of the entry.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the name, or D_004A1244 when the event has no table
 */
extern "C" void* func_00155208(cBEScoreInterface* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    tBEHighScore* highScores = cBELibrary_getHighScoreTable(D_005305F0[0], cBENewRaceInterface_newRace.cGameEvent);
    if (highScores != 0)
        return highScores[i].strUserName;
    return D_004A1244;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getPlayerLastCompletedEventStats);
#ifdef SKIP_ASM
/**
 * Returns a profile's last-event block: the rider stats and rank stored at +0x20 + profile * 0xB0.
 *
 * @param profileIndex  profile index, 0 or 1 (On Tour iProfileIndex)
 * @return the stats block (0xAC bytes of stats, then the rank)
 */
void* cBEScoreInterface::getPlayerLastCompletedEventStats(int profileIndex)
{
    return (char*)&LastEventStats[profileIndex];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getPlayerBestStats);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns a character's best-stats block: +0xDE4 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88).
 *
 * Ten (value, location) pairs and the longest combo, as cBEScoreInterface_processRiderBest writes them.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iChar)
 * @return the best-stats block
 */
void* cBEScoreInterface::getPlayerBestStats(int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + (profileIndex * 39760 + characterId * 3976);
    return charRecord + 0xde4;
}
#endif

signed char cBELibrary_getCharacterID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getCurrentHighlightLevel__FPvi);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Returns the level reached in one career highlight by the current character of player 0's profile.
 *
 * Reads byte @p highlight of the highlight levels mCareerHighlights in cBELibrary_profileLib[0] for rider slot 0's
 * character (cBELibrary_getCharacterID(0)). Callers sum the 8 highlights and compare the total with 24.
 *
 * @param highlight  which highlight (0-7; On Tour: eBEChallengeType)
 * @return the highlight's level
 */
signed char cBEScoreInterface_getCurrentHighlightLevel(void* self, int highlight)
{
    int charID = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][charID].mCareerHighlights[highlight];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_isBetterMedal);
#ifdef SKIP_ASM
/**
 * Tells whether @p newMedal beats @p currentMedal: never when the new one is -1 (none), always when the current one is
 * -1, otherwise when its id is lower (lower ids are better medals).
 *
 * @param currentMedal  the medal held so far, or -1
 * @param newMedal  the medal just won, or -1
 * @return 1 when the new medal is better, else 0
 */
int cBEScoreInterface::isBetterMedal(int currentMedal, int newMedal)
{
    if (newMedal == -1) {
        return 0;
    }
    if (currentMedal == -1) {
        return 1;
    }
    return newMedal < currentMedal;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_convertRankToMedal);
#ifdef SKIP_ASM
/**
 * Converts a finishing rank to a medal id: rank 0, 1, 2 (first to third) give medals 1, 2, 3; any other rank gives -1
 * (no medal).
 *
 * @param rank  the finishing position, 0 for first
 * @return the medal id, or -1
 */
int cBEScoreInterface::convertRankToMedal(int rank)
{
    switch (rank) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the score interface's state to a replay frame: an empty stub, nothing is recorded.
 */
void cBEScoreInterface_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reads the score interface's state from a replay frame: an empty stub, nothing is restored.
 */
void cBEScoreInterface_readFromReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_processHighlight);
#ifdef SKIP_ASM
extern short cBELibrary_highlightLevel[][3];

/**
 * Raises a character's level in one career highlight to the level a value reaches.
 *
 * The level is 1, 2 or 3 for the highest of the highlight's three thresholds cBELibrary_highlightLevel[@p idx][0..2] that @p score
 * reaches (0 for none). The stored level, mCareerHighlights[@p idx] of the character record (what
 * cBEScoreInterface_getCurrentHighlightLevel reads), only goes up. cBEScoreInterface_processRiderBest calls it for
 * highlights 0 to 7, passing the character record where a method would pass `this`.
 *
 * @param rec  the character record (cBELibrary_profileLib[profile][character])
 * @param idx  which highlight, 0 to 7
 * @param score  the value to rate
 */
extern "C" void cBEScoreInterface_processHighlight(cBECharProfileDB* rec, int idx, int score)
{
    signed char cur = rec->mCareerHighlights[idx];
    int level = 0;
    int i;
    for (i = 2; i >= 0; i--) {
        if (score >= cBELibrary_highlightLevel[idx][i]) {
            level = i + 1;
            break;
        }
    }
    if (cur < level) {
        rec->mCareerHighlights[idx] = level;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_processRiderBest);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void cBEScoreInterface_processHighlight(cBECharProfileDB* rec, int idx, int score);

extern cBERaceDB cBELibrary_raceDB_r155420 __asm__("cBELibrary_raceDB");

struct sEvent_155420 {
    int id;
    char pad_0x4[0x60];
};
extern sEvent_155420 cBELibrary_locationDB_r155420[] __asm__("cBELibrary_locationDB");

struct sRaceStats_155420 {
    char pad_0x00[0x18];
    int f18;            // 0x18
    int f1C;            // 0x1C
    int f20;            // 0x20
    char pad_0x24[0x2C - 0x24];
    int f2C;            // 0x2C
    char pad_0x30[0x50 - 0x30];
    float f50;          // 0x50
    char pad_0x54[0x58 - 0x54];
    float f58;          // 0x58
    float f5C;          // 0x5C
    char pad_0x60[0x88 - 0x60];
    int f88;            // 0x88
};

struct sRecord_155420 {
    int score;
    int event;
};

/**
 * Updates a character's best stats from one event's results and raises the matching career highlights.
 *
 * Does nothing when raceDB +0x49 is set or the rider's profile is above 2. The best stats are the ten (value, location)
 * pairs at +0xDE4 of the character record; the location is cBELibrary_locationDB[raceDB +0x40].id (22 becomes 0). Pairs
 * 0-7 take the larger of the stored value and the stats words at +0x2C, +0x18, +0x1C, +0x20, +0x88 and the rounded
 * floats at +0x5C, +0x58, +0x50 (On Tour tRiderStats has NumFrags, NumUberTricks, NumSuperUberTricks,
 * NumUberGrindTricks, LongestCombo, BiggestHandplantTime, BiggestRailDist, BiggestAirTime there), each also raising a
 * highlight with cBEScoreInterface_processHighlight (pair 1 excepted).
 *
 * Pair 8 keeps the shortest positive @p eventTime in race event 0 (raceDB +0x4A); pair 9 keeps the largest positive @p eventPoints in
 * events 1-3 or race kind 6 (raceDB +0x48) and raises highlight 6 with @p eventPoints / 100. Without @p statsArg, or in event
 * 12, only pairs 8 and 9 are looked at. Finally the word after the ten pairs keeps the largest stats +0x88.
 *
 * @param rider  race rider slot
 * @param statsArg  the rider stats block, passed as an int; 0 for none
 * @param eventPoints  the event points, or -1
 * @param eventTime  the event time in seconds, or -1
 */
// PORT: the unit declares this as (int, int, int, int) (cBEScoreInterface_processRiderStats passes the stats pointer as int).
extern "C" void cBEScoreInterface_processRiderBest(int rider, int statsArg, int eventPoints, int eventTime)
{
    sRaceStats_155420* stats = (sRaceStats_155420*)statsArg;
    cBERaceDB* rs = &cBELibrary_raceDB_r155420;
    if (rs->cGamePath != 0)
        return;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile > 2)
        return;
    int charID = cBELibrary_getCharacterID(rider);
    cBECharProfileDB* rec = &cBELibrary_profileLib[profile][charID];
    int event = cBELibrary_locationDB_r155420[rs->iLocationIdx].id;
    int kind = rs->cGameMode;
    int mode = rs->cGameEvent;
    if (event == 22)
        event = 0;
    sRecord_155420* best = (sRecord_155420*)(&rec->mRiderBest);
    int start = 0;
    if (stats == 0 || mode == 12)
        start = 8;
    for (int i = start; i < 10; i++) {
        sRecord_155420* slot = &best[i];
        int statValue;
        switch (i) {
        case 0:
            statValue = stats->f2C;
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 3, statValue);
            }
            break;
        case 1:
            statValue = stats->f18;
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
            }
            break;
        case 2:
            statValue = stats->f1C;
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 5, statValue);
            }
            break;
        case 3:
            statValue = stats->f20;
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 4, statValue);
            }
            break;
        case 4:
            statValue = stats->f88;
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 7, statValue);
            }
            break;
        case 5:
            statValue = (int)(stats->f5C + 0.5f);
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 1, statValue);
            }
            break;
        case 6:
            statValue = (int)(stats->f58 + 0.5f);
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 0, statValue);
            }
            break;
        case 7:
            statValue = (int)(stats->f50 + 0.5f);
            if (statValue >= slot->score) {
                slot->score = statValue;
                slot->event = event;
                cBEScoreInterface_processHighlight(rec, 2, statValue);
            }
            break;
        case 8:
            if (eventTime > 0 && mode == 0) {
                if (slot->score == 0 || eventTime <= slot->score) {
                    slot->score = eventTime;
                    slot->event = event;
                }
            }
            break;
        case 9:
            if (eventPoints > 0) {
                if ((mode >= 1 && mode <= 3) || kind == 6) {
                    if (eventPoints >= slot->score) {
                        slot->score = eventPoints;
                        slot->event = event;
                        cBEScoreInterface_processHighlight(rec, 6, eventPoints / 100);
                    }
                }
            }
            break;
        }
    }
    if (stats != 0) {
        if (stats->f88 >= best[10].score)
            best[10].score = stats->f88;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_isEligibleForHighScore);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cBERules_hasModifiedRules(int i);

/**
 * Returns whether a profile may enter the high-score tables.
 *
 * False for profile 2 and up, and while cBERules_hasModifiedRules(0) reports any of player 0's option flags in cBERules_mRules set.
 * Likely bug: with two or more humans (cBENewRaceInterface_newRace +0x34) it tests cBERules_hasModifiedRules(0) again instead of
 * player 1, so that branch always returns true.
 *
 * @param rider  the profile index (On Tour iProfile)
 * @return true when the profile is eligible
 */
bool cBEScoreInterface::isEligibleForHighScore(int rider)
{
    if (rider >= 2 || cBERules_hasModifiedRules(0))
        return false;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.iNumberHumans >= 2)
        return !cBERules_hasModifiedRules(0);
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getNumberOfWinsOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern signed char D_005305F9[];

/**
 * Returns how many times a race rider's character has won an event at a location: the short at +0x2 of its score
 * record.
 *
 * Returns 0 when raceDB +0x49 (D_005305F9) is set, the rider's profile is 3 or more, or there is no record. The record
 * is cBECharProfileDB_getScoreStats(character record, @p locationID, @p eventID). Nothing calls it.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param locationID  the location (On Tour iLocation)
 * @param eventID  the game event (On Tour iEvent)
 * @return the win count
 */
int cBEScoreInterface::getNumberOfWinsOnTrack(int rider, int locationID, int eventID)
{
    if (D_005305F9[0] != 0)
        return 0;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return 0;
    int charID = cBELibrary_getCharacterID(rider);
    unsigned short* stats = (unsigned short*)cBECharProfileDB_getScoreStats(&cBELibrary_profileLib[profile][charID], locationID, eventID);
    if (stats == 0)
        return 0;
    return stats[1];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getBestMedalOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern signed char D_005305F9[];
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the best medal a race rider's character has for an event at a location: byte 1 of its score record.
 *
 * Returns -1 when raceDB +0x49 (D_005305F9) is set, the rider's profile is 3 or more, or there is no record. The record
 * is cBECharProfileDB_getScoreStats(character record, @p locationID, @p eventID).
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param locationID  the location (On Tour iLocation)
 * @param eventID  the game event (On Tour iEvent)
 * @return the medal, 0 best to 3, or -1
 */
int cBEScoreInterface::getBestMedalOnTrack(int rider, int locationID, int eventID)
{
    if (D_005305F9[0] != 0)
        return -1;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return -1;
    int charID = cBELibrary_getCharacterID(rider);
    signed char* stats = (signed char*)cBECharProfileDB_getScoreStats(&cBELibrary_profileLib[profile][charID], locationID, eventID);
    if (stats == 0)
        return -1;
    return stats[1];
}
#endif


//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_setBestMedalOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void cBECharProfileDB_updateMedalStats(void* iface, int a, int b, int c);
extern signed char D_005305F9[];

/**
 * Stores a medal in a race rider's character's score record for an event at a location (cBECharProfileDB_updateMedalStats).
 *
 * Does nothing when raceDB +0x49 (D_005305F9) is set or the rider's profile is 3 or more. Nothing calls it.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param locationID  the location (On Tour iLocation)
 * @param eventID  the game event (On Tour iEvent)
 * @param medal  the medal (On Tour iMedal)
 */
void cBEScoreInterface::setBestMedalOnTrack(int rider, int locationID, int eventID, int medal)
{
    if (D_005305F9[0] != 0)
        return;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return;
    int ch = cBELibrary_getCharacterID(rider);
    cBECharProfileDB_updateMedalStats(&cBELibrary_profileLib[profile][ch], locationID, eventID, medal);
}
#endif
