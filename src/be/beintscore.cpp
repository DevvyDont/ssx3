#include "common.h"
#include "be/beintnewrace.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cBEScoreInterface_cBEScoreInterface(void* self);
extern const char str_BEScoreInt[];
extern void* cBEScoreInterface_instance;

struct cBEScoreInterface {
    char pad_0x00[8];
    int field_0x8;
    char pad_0xC[4];
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1C;
};

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
        cBEScoreInterface_instance = scoreIface;
        scoreIface->field_0x8 = 0;
        scoreIface->field_0x10 = 0;
        scoreIface->field_0x14 = 0;
        scoreIface->field_0x18 = -1;
        scoreIface->field_0x1C = -1;
    }
    return cBEScoreInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_cBEScoreInterface);
#ifdef SKIP_ASM
extern "C" void cBEScoreInterface_discard(void* self);
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
void* cBEScoreInterface_cBEScoreInterface_impl(void* self)
{
    *(void***)((char*)self + 0xC) = cBEScoreInterface_vtbl;
    cBEScoreInterface_discard(self);
    return self;
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

struct sScoreSlot_001549E8 {
    char pad_0x00[0xAC];
    int last; // 0xAC
};

struct sScore_001549E8 {
    char pad_0x00[0x10];
    int a;    // 0x10
    int b;    // 0x14
    int c;    // 0x18
    int d;    // 0x1C
    sScoreSlot_001549E8 slots[2]; // 0x20
};

/**
 * Clears all of the score interface's recorded results.
 *
 * +0x10 and +0x14 become 0, +0x18 and +0x1C -1; the two 0xB0-byte slots at +0x20 are zeroed and each slot's +0xAC set
 * to -1.
 *
 * @param scoreIface  the score interface (this)
 */
extern "C" void cBEScoreInterface_discard(void* scoreIface)
{
    sScore_001549E8* self = (sScore_001549E8*)scoreIface;
    self->a = 0;
    self->b = 0;
    self->c = -1;
    self->d = -1;
    MEM_fill(self->slots, 0, sizeof(self->slots));
    for (int i = 1; i >= 0; i--)
    {
        self->slots[i].last = -1;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00154A58);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void func_00155420(int rider, int a, int b, int c);
extern signed char D_005305F9[];

/**
 * Records a rider's race statistics in its character's best records, for the two human profiles only.
 *
 * Unless raceDB byte +0x49 (D_005305F9) is set, calls func_00155420(@p rider, @p a, -1, -1) when the rider's profile
 * (cBELibrary_getProfileIndex) is 0 or 1. The AI code passes each rider's metrix stats block (metrix + 0xFC);
 * func_00154AB8 passes the event stats.
 *
 * @param rider  race rider slot
 * @param a  the rider stats block, passed as an int
 */
extern "C" void func_00154A58(int rider, int a)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (D_005305F9[0] == 0 && profile < 2)
    {
        func_00155420(rider, a, -1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00154AB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool func_001557E0(void* self, int rider);
extern "C" char* cBENewPlayerInterface_getPlayerName(void* self, int idx);
extern "C" char* strcpy(char* dst, const char* src);

struct sProfileChar_154AB8 { char data[0xF88]; };
extern "C" int func_00152460(sProfileChar_154AB8* self, int event, int mode, unsigned int value);
extern sProfileChar_154AB8 D_004A6CA8_154AB8[][10] __asm__("cBELibrary_profileLib");

struct sRaceStats_154AB8 { int w[0xAC / 4]; };
struct sScoreSlot_154AB8 {
    sRaceStats_154AB8 stats;
    int result;     // 0xAC
};
struct sScore_154AB8 {
    char pad_0x00[0x10];
    int best[2];    // 0x10
    int rank[2];    // 0x18
    sScoreSlot_154AB8 slots[2]; // 0x20
};

struct sHighScore_154AB8 {
    unsigned int value;
    int charID;
    char name[0xC];
};
sHighScore_154AB8* func_0014AB20_154AB8(int event, int mode) __asm__("func_0014AB20");

struct sRaceSettings_154AB8 {
    char pad_0x00[0x40];
    int event;          // 0x40
    char pad_0x44[0x4];
    signed char b48;    // 0x48
    signed char b49;    // 0x49
};
extern sRaceSettings_154AB8 D_005305B0_154AB8 __asm__("cBELibrary_raceDB");
struct sRaceGlobal_154AB8 {
    char pad_0x00[0x34];
    int numHumans;      // 0x34
    char pad_0x38[0x12];
    signed char mode;   // 0x4A
};
struct sEvent_154AB8 {
    int id;
    char pad_0x4[0x60];
};
extern sEvent_154AB8 D_0043D950_154AB8[] __asm__("cBELibrary_locationDB");

/**
 * Records a human rider's finished-event stats and score: keeps the stats, enters the score in the event's top-5 table,
 * and updates the character's best records.
 *
 * Only profiles 0 and 1. Copies @p stats into the profile's last-event slot (+0x20 + profile * 0xB0) with result -1,
 * passes them to func_00154A58, and resets the profile's high-score rank (+0x18) to -1 and best flag (+0x10) to 0. The
 * location comes from cBELibrary_locationDB[raceDB +0x40].id (22 becomes 0) and the event from
 * cBENewRaceInterface_newRace.cGameEvent. Race kinds 0 and 5 (raceDB +0x48) are timed: @p score is converted from ticks
 * to seconds.
 *
 * When func_001557E0 allows it, the score goes into the 5-entry table from func_0014AB20 (lower is better when timed):
 * later entries shift down, the new one gets the character, score and player name
 * (cBENewPlayerInterface_getPlayerName), and with two humans the other profile's rank moves down (dropping off at 5).
 * Then func_00155420 gets the score as the time (timed) or the points, and unless raceDB +0x49 is set, the best flag is
 * func_00152460's result for the character record.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param stats  the rider's event stats, 0xAC bytes (On Tour pRiderStats)
 * @param score  the event score or time in ticks (On Tour uiPoints)
 */
extern "C" void func_00154AB8(sScore_154AB8* self, int rider, sRaceStats_154AB8* stats, unsigned int score)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (profile >= 2)
        return;
    self->slots[profile].stats = *stats;
    int mode;
    sProfileChar_154AB8* pc;
    int timed = 0;
    self->slots[profile].result = -1;
    func_00154A58(rider, (int)stats);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int charID = cBELibrary_getCharacterID(rider);
    mode = cBENewRaceInterface_newRace.cGameEvent;
    int event = D_0043D950_154AB8[D_005305B0_154AB8.event].id;
    pc = &D_004A6CA8_154AB8[profile][charID];
    self->best[profile] = 0;
    self->rank[profile] = -1;
    if (event == 22)
        event = 0;
    if (D_005305B0_154AB8.b48 == 0 || D_005305B0_154AB8.b48 == 5) {
        timed = 1;
        score = (int)(score * (1.0f / 60.0f));
    }
    if (func_001557E0(self, profile)) {
        sHighScore_154AB8* t = func_0014AB20_154AB8(event, mode);
        if (t != 0) {
            for (int i = 0; i < 5; i++) {
                int better;
                if (timed)
                    better = t[i].value < score;
                else
                    better = score < t[i].value;
                if (!better) {
                    for (int j = 4; j > i; j--)
                        t[j] = t[j - 1];
                    t[i].charID = charID;
                    t[i].value = score;
                    strcpy(t[i].name, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), rider));
                    self->rank[profile] = i;
                    if (cBENewRaceInterface_newRace.iNumberHumans >= 2) {
                        if (self->rank[1 - profile] >= i) {
                            self->rank[1 - profile]++;
                            if (self->rank[1 - profile] >= 5)
                                self->rank[1 - profile] = -1;
                        }
                    }
                    break;
                }
            }
        }
    }
    if (timed)
        func_00155420(rider, (int)stats, -1, score);
    else
        func_00155420(rider, (int)stats, score, -1);
    if (D_005305B0_154AB8.b49 == 0)
        self->best[profile] = func_00152460(pc, event, mode, score);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00154EE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
extern "C" int cBEScoreInterface_convertRankToMedal(void* self, int a);
extern "C" void func_001E25D8(int mode, int event, int result);
extern "C" void func_001E1EB8(void);
extern "C" void func_001E1550(void);
extern "C" int func_001456A0(void* self, int idx, int mode);
extern "C" void func_001591E8(void* iface, int profile, int charID, int mode, int event, int medal);
extern signed char D_00535C12[];

struct sRaceSettings_154EE8 {
    char pad_0x00[0x40];
    int event;          // 0x40
    char pad_0x44[0x4];
    signed char b48;    // 0x48
    signed char b49;    // 0x49
};
extern sRaceSettings_154EE8 cBELibrary_raceDB;

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
 * cBEScoreInterface_convertRankToMedal(@p result); events 4 and 5 (D_00535C12) give 1 for first place, else none.
 * Unless raceDB +0x49 is set it updates the event popups (func_001E25D8, then func_001E1EB8 and, outside events 4 and
 * 5, func_001E1550).
 *
 * With a medal, unless raceDB +0x49 is set or raceDB +0x48 is 4: when @p score meets or beats the event's target from
 * func_001456A0 (time target * 60 for events 0 and 4, lower is better; a score target otherwise) the medal becomes 0
 * (the best). Then calls func_001591E8 on the reward interface.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param result  the finishing rank, 0 first (On Tour iRanking)
 * @param score  the event score or time in ticks (On Tour uiPoints)
 */
extern "C" void func_00154EE8(char* self, int rider, int result, int score)
{
    int profile = cBELibrary_getProfileIndex(rider);
    if (profile >= 2)
        return;
    *(int*)(self + profile * 0xB0 + 0xCC) = result;
    void* be = cBE_getInterface_Fv(cBE_getBE(), 0);
    int charID = cBELibrary_getCharacterID(rider);
    int mode = D_00535C12[0];
    int event = cBELibrary_locationDB[cBELibrary_raceDB.event].id;
    if (event == 22)
        event = 0;
    int medal = cBEScoreInterface_convertRankToMedal(self, result);
    if (mode == 4 || mode == 5)
        medal = result == 0 ? 1 : -1;
    if (cBELibrary_raceDB.b49 == 0)
        func_001E25D8(mode, event, result);
    if (medal == -1)
        return;
    if (cBELibrary_raceDB.b49 == 0) {
        func_001E1EB8();
        if (cBELibrary_raceDB.b49 == 0 && mode != 4 && mode != 5)
            func_001E1550();
    }
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 13);
    if (cBELibrary_raceDB.b49 != 0)
        return;
    if (cBELibrary_raceDB.b48 == 4)
        return;
    int best = func_001456A0(be, event, mode);
    if (best != -1) {
        if (mode == 0 || mode == 4) {
            if (best * 60 >= score)
                medal = 0;
        } else {
            if (score >= best)
                medal = 0;
        }
    }
    func_001591E8(reward, profile, charID, mode, event, medal);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_001550E8);
#ifdef SKIP_ASM
struct sScoreEntry_001550E8 {
    int value;
    char pad[0x10];
};
extern "C" sScoreEntry_001550E8* func_0014AB20(int a, int b);

/**
 * Returns the score of one entry in an event's high-score table.
 *
 * The table is func_0014AB20(@p event, @p mode); entries are 0x14 bytes with the score first.
 *
 * @param i  entry index, 0 (best) to 4
 * @param event  the location
 * @param mode  the game event
 * @return the score, or 0 when the event has no table
 */
extern "C" int func_001550E8(void* self, int i, int event, int mode)
{
    sScoreEntry_001550E8* highScores = func_0014AB20(event, mode);
    if (highScores != 0) {
        return highScores[i].value;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155130);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int D_005305F0[];
extern signed char D_00535C12[];

/**
 * Returns the score of one entry in the current race's high-score table.
 *
 * The table is func_0014AB20(raceDB +0x40, D_00535C12), the race's location index and game event. The pause and lodge
 * screens show entry 0 as the record.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the score, or 0 when the event has no table
 */
extern "C" int func_00155130(void* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sScoreEntry_001550E8* t = func_0014AB20(D_005305F0[0], D_00535C12[0]);
    if (t != 0)
    {
        return t[i].value;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155190);
#ifdef SKIP_ASM
struct sScoreEntryB_00155190
{
    int value;
    int idx;
    char pad[0xC];
};
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
 * The table is func_0014AB20(raceDB +0x40, D_00535C12); the entry's second word is the character ID. The lodge shows
 * the result as text next to the entry.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the character record, or D_004A1244 when the event has no table
 */
extern "C" void* func_00155190(void* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sScoreEntryB_00155190* t = (sScoreEntryB_00155190*)func_0014AB20(D_005305F0[0], D_00535C12[0]);
    if (t == 0)
        return D_004A1244;
    return &D_00530990[t[i].idx];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155208);
#ifdef SKIP_ASM
struct sScoreEntryB_00155208
{
    int value;
    int idx;
    char f8[0xC];
};
extern void* D_004A1244;

/**
 * Returns the player name stored in one entry of the current race's high-score table.
 *
 * The table is func_0014AB20(raceDB +0x40, D_00535C12); the 12-byte name is at +0x8 of the entry.
 *
 * @param i  entry index, 0 (best) to 4
 * @return the name, or D_004A1244 when the event has no table
 */
extern "C" void* func_00155208(void* self, int i)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sScoreEntryB_00155208* t = (sScoreEntryB_00155208*)func_0014AB20(D_005305F0[0], D_00535C12[0]);
    if (t != 0)
        return t[i].f8;
    return D_004A1244;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155270);
#ifdef SKIP_ASM
/**
 * Returns a profile's last-event block: the rider stats and rank stored at +0x20 + profile * 0xB0.
 *
 * @param profileIndex  profile index, 0 or 1 (On Tour iProfileIndex)
 * @return the stats block (0xAC bytes of stats, then the rank)
 */
extern "C" void* func_00155270(void* self, int profileIndex)
{
    return (char*)self + (profileIndex * 0xb0 + 0x20);
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155288);
#ifdef SKIP_ASM
extern char cBELibrary_profileLib[];

/**
 * Returns a character's best-stats block: +0xDE4 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88).
 *
 * Ten (value, location) pairs and the longest combo, as func_00155420 writes them.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iChar)
 * @return the best-stats block
 */
extern "C" void* func_00155288(void* self, int profileIndex, int characterId)
{
    char* charRecord = cBELibrary_profileLib + (profileIndex * 39760 + characterId * 3976);
    return charRecord + 0xde4;
}
#endif

signed char cBELibrary_getCharacterID(int index);
extern char cBELibrary_profileLib[];

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_getCurrentHighlightLevel__FPvi);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sHL { char pad[0xBB8]; signed char lv[1]; };
/**
 * Returns the level reached in one career highlight by the current character of player 0's profile.
 *
 * Reads byte @p riderIndex of the highlight levels at +0xBB8 in cBELibrary_profileLib[0] for rider slot 0's
 * character (cBELibrary_getCharacterID(0)). Callers sum the 8 highlights and compare the total with 24.
 *
 * @param riderIndex  which highlight (0-7; On Tour: eBEChallengeType); despite the name, not a rider index
 * @return the highlight's level
 */
signed char cBEScoreInterface_getCurrentHighlightLevel(void* self, int riderIndex)
{
    int charID = cBELibrary_getCharacterID(0);
    int offset = riderIndex + charID * 3976;
    return ((sHL*)cBELibrary_profileLib)->lv[offset];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", cBEScoreInterface_isBetterMedal);
#ifdef SKIP_ASM
extern "C" int cBEScoreInterface_isBetterMedal(void* self, int currentMedal, int newMedal)
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
extern "C" int cBEScoreInterface_convertRankToMedal(void* self, int rank)
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
INCLUDE_ASM("be/beintscore", func_00155390);
#ifdef SKIP_ASM
extern short D_00440ED0[][3];

/**
 * Raises a character's level in one career highlight to the level a value reaches.
 *
 * The level is 1, 2 or 3 for the highest of the highlight's three thresholds D_00440ED0[@p idx][0..2] that @p score
 * reaches (0 for none). The stored level, the signed byte at +0xBB8 + @p idx of the character record (what
 * cBEScoreInterface_getCurrentHighlightLevel reads), only goes up. func_00155420 calls it for highlights 0 to 7.
 *
 * @param idx  which highlight, 0 to 7
 * @param score  the value to rate
 */
extern "C" void func_00155390(char* self, int idx, int score)
{
    signed char cur = *(signed char*)(self + idx + 0xBB8);
    int level = 0;
    int i;
    for (i = 2; i >= 0; i--) {
        if (score >= D_00440ED0[idx][i]) {
            level = i + 1;
            break;
        }
    }
    if (cur < level) {
        *(signed char*)(self + idx + 0xBB8) = level;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155420);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void func_00155390(char* self, int idx, int score);

struct sRaceSettings_155420 {
    char pad_0x00[0x40];
    int event;          // 0x40
    char pad_0x44[0x4];
    signed char b48;    // 0x48
    signed char b49;    // 0x49
    signed char mode;   // 0x4A
};
extern sRaceSettings_155420 D_005305B0_r155420 __asm__("cBELibrary_raceDB");

struct sEvent_155420 {
    int id;
    char pad_0x4[0x60];
};
extern sEvent_155420 D_0043D950_r155420[] __asm__("cBELibrary_locationDB");

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
 * highlight with func_00155390 (pair 1 excepted).
 *
 * Pair 8 keeps the shortest positive @p a3 in race event 0 (raceDB +0x4A); pair 9 keeps the largest positive @p a2 in
 * events 1-3 or race kind 6 (raceDB +0x48) and raises highlight 6 with @p a2 / 100. Without @p statsArg, or in event
 * 12, only pairs 8 and 9 are looked at. Finally the word after the ten pairs keeps the largest stats +0x88.
 *
 * @param rider  race rider slot
 * @param statsArg  the rider stats block, passed as an int; 0 for none
 * @param a2  the event points, or -1
 * @param a3  the event time in seconds, or -1
 */
// PORT: the unit declares this as (int, int, int, int) (func_00154A58 passes the stats pointer as int).
extern "C" void func_00155420(int rider, int statsArg, int a2, int a3)
{
    sRaceStats_155420* stats = (sRaceStats_155420*)statsArg;
    sRaceSettings_155420* rs = &D_005305B0_r155420;
    if (rs->b49 != 0)
        return;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile > 2)
        return;
    int charID = cBELibrary_getCharacterID(rider);
    char* rec = (char*)cBELibrary_profileLib + profile * 0x9B50 + charID * 3976;
    int event = D_0043D950_r155420[rs->event].id;
    int kind = rs->b48;
    int mode = rs->mode;
    if (event == 22)
        event = 0;
    sRecord_155420* best = (sRecord_155420*)(rec + 0xDE4);
    int start = 0;
    if (stats == 0 || mode == 12)
        start = 8;
    for (int i = start; i < 10; i++) {
        sRecord_155420* slot = &best[i];
        int v;
        switch (i) {
        case 0:
            v = stats->f2C;
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 3, v);
            }
            break;
        case 1:
            v = stats->f18;
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
            }
            break;
        case 2:
            v = stats->f1C;
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 5, v);
            }
            break;
        case 3:
            v = stats->f20;
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 4, v);
            }
            break;
        case 4:
            v = stats->f88;
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 7, v);
            }
            break;
        case 5:
            v = (int)(stats->f5C + 0.5f);
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 1, v);
            }
            break;
        case 6:
            v = (int)(stats->f58 + 0.5f);
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 0, v);
            }
            break;
        case 7:
            v = (int)(stats->f50 + 0.5f);
            if (v >= slot->score) {
                slot->score = v;
                slot->event = event;
                func_00155390(rec, 2, v);
            }
            break;
        case 8:
            if (a3 > 0 && mode == 0) {
                if (slot->score == 0 || a3 <= slot->score) {
                    slot->score = a3;
                    slot->event = event;
                }
            }
            break;
        case 9:
            if (a2 > 0) {
                if ((mode >= 1 && mode <= 3) || kind == 6) {
                    if (a2 >= slot->score) {
                        slot->score = a2;
                        slot->event = event;
                        func_00155390(rec, 6, a2 / 100);
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
INCLUDE_ASM("be/beintscore", func_001557E0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool func_0014F810(int i);
extern int D_00535BFC[];

/**
 * Returns whether a profile may enter the high-score tables.
 *
 * False for profile 2 and up, and while func_0014F810(0) reports any of player 0's option flags in D_005308B8 set.
 * Likely bug: with two or more humans (cBENewRaceInterface_newRace +0x34) it tests func_0014F810(0) again instead of
 * player 1, so that branch always returns true.
 *
 * @param rider  the profile index (On Tour iProfile)
 * @return true when the profile is eligible
 */
extern "C" bool func_001557E0(void* self, int rider)
{
    if (rider >= 2 || func_0014F810(0))
        return false;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535BFC[0] >= 2)
        return !func_0014F810(0);
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_00155848);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern signed char D_005305F9[];

/**
 * Returns how many times a race rider's character has won an event at a location: the short at +0x2 of its score
 * record.
 *
 * Returns 0 when raceDB +0x49 (D_005305F9) is set, the rider's profile is 3 or more, or there is no record. The record
 * is cBECharProfileDB_getScoreStats(character record, @p a, @p b). Nothing calls it.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param a  the location (On Tour iLocation)
 * @param b  the game event (On Tour iEvent)
 * @return the win count
 */
extern "C" int func_00155848(void* self, int rider, int a, int b)
{
    if (D_005305F9[0] != 0)
        return 0;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return 0;
    int c = cBELibrary_getCharacterID(rider);
    unsigned short* stats = (unsigned short*)cBECharProfileDB_getScoreStats(cBELibrary_profileLib + profile * 39760 + c * 3976, a, b);
    if (stats == 0)
        return 0;
    return stats[1];
}
#endif

//100%
INCLUDE_ASM("be/beintscore", func_001558F8);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
signed char cBELibrary_getCharacterID(int index);
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern signed char D_005305F9[];
extern char cBELibrary_profileLib[];

/**
 * Returns the best medal a race rider's character has for an event at a location: byte 1 of its score record.
 *
 * Returns -1 when raceDB +0x49 (D_005305F9) is set, the rider's profile is 3 or more, or there is no record. The record
 * is cBECharProfileDB_getScoreStats(character record, @p a, @p b).
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param a  the location (On Tour iLocation)
 * @param b  the game event (On Tour iEvent)
 * @return the medal, 0 best to 3, or -1
 */
extern "C" int func_001558F8(void* self, int rider, int a, int b)
{
    if (D_005305F9[0] != 0)
        return -1;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return -1;
    int c = cBELibrary_getCharacterID(rider);
    signed char* stats = (signed char*)cBECharProfileDB_getScoreStats(cBELibrary_profileLib + profile * 39760 + c * 3976, a, b);
    if (stats == 0)
        return -1;
    return stats[1];
}
#endif


//100%
INCLUDE_ASM("be/beintscore", func_001559A8);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern "C" void func_00152528(void* iface, int a, int b, int c);
extern signed char D_005305F9[];

/**
 * Stores a medal in a race rider's character's score record for an event at a location (func_00152528).
 *
 * Does nothing when raceDB +0x49 (D_005305F9) is set or the rider's profile is 3 or more. Nothing calls it.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param a  the location (On Tour iLocation)
 * @param b  the game event (On Tour iEvent)
 * @param c  the medal (On Tour iMedal)
 */
extern "C" void func_001559A8(void* self, int rider, int a, int b, int c)
{
    if (D_005305F9[0] != 0)
        return;
    int profile = cBELibrary_getProfileIndex(rider);
    if ((unsigned int)profile >= 3)
        return;
    int ch = cBELibrary_getCharacterID(rider);
    func_00152528(cBELibrary_profileLib + profile * 39760 + ch * 3976, a, b, c);
}
#endif
