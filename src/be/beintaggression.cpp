#include "common.h"
#include "replay/frameptr.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEAggressionInt[];
extern void* cBEAggressionInterface_vtbl[16];
extern void* cBEAggressionInterface_instance;

struct cBEAggressionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's aggression interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEAggressionInt", clears the word at +0x8 and stores the vtable.
 *
 * @return the aggression interface
 */
void* cBEAggressionInterface_getThis()
{
    if (cBEAggressionInterface_instance == 0) {
        cBEAggressionInterface* mem = (cBEAggressionInterface*)cMemMan_alloc(0x10, str_BEAggressionInt, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = cBEAggressionInterface_vtbl;
        cBEAggressionInterface_instance = mem;
    }
    return cBEAggressionInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the aggression interface's submit (slot 1 of its vtable cBEAggressionInterface_vtbl).
 */
void cBEAggressionInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the aggression interface's discard (slot 2 of its vtable cBEAggressionInterface_vtbl).
 */
void cBEAggressionInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_getState);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
void* cBENewRaceInterface_getThis();
extern "C" int cBENewRaceInterface_getRival(void* race);

struct sAggrTriple {
    signed char v[3];
};

struct sAggrCharProfile {
    char pad_0x000[0xBC1];
    sAggrTriple aggression[16]; // 0xBC1
    char pad_0xBF1[0xF88 - 0xBC1 - 16 * 3];
};

extern sAggrCharProfile cBELibrary_profileLib[][10];

/**
 * Returns how @p rider feels about @p targetRider: the state byte of @p rider's relation record for the target's
 * character, or 2 when the target is the rival of the current peak.
 *
 * The record is cBELibrary_profileLib[profile of @p rider][character of @p rider].aggression[character of
 * @p targetRider] (3 bytes: state, level, ticks; see cBEAggressionInterface_applyAggression). The rival comes from
 * cBENewRaceInterface_getRival on the new-race interface.
 *
 * @param targetRider  the rider being looked at
 * @param rider  the rider whose feelings are read
 * @return the relation state (0-3; On Tour friend, neutral, rival, enemy)
 */
extern "C" int cBEAggressionInterface_getState(void* self, int targetRider, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int ca = cBELibrary_getCharacterID(targetRider);
    int cb = cBELibrary_getCharacterID(rider);
    if (cBENewRaceInterface_getRival(cBENewRaceInterface_getThis()) == ca)
        return 2;
    return cBELibrary_profileLib[profile][cb].aggression[ca].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_getAggression);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
void* cBENewRaceInterface_getThis();
extern "C" int cBENewRaceInterface_getRival(void* race);

extern sAggrCharProfile cBELibrary_profileLib[][10];

/**
 * Returns how aggressive @p rider is toward @p targetRider: the level byte of @p rider's relation record for the
 * target's character, or 3 when the target is the rival of the current peak (cBENewRaceInterface_getRival).
 *
 * Same record as cBEAggressionInterface_getState; the level is the record's tick count / 5, clamped by its state
 * (cBEAggressionInterface_applyAggression).
 *
 * @param targetRider  the rider being looked at
 * @param rider  the rider whose aggression is read
 * @return the aggression level (0-4; On Tour none, mild, moderate, hard, chase)
 */
extern "C" int cBEAggressionInterface_getAggression(void* self, int targetRider, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int ca = cBELibrary_getCharacterID(targetRider);
    int cb = cBELibrary_getCharacterID(rider);
    if (cBENewRaceInterface_getRival(cBENewRaceInterface_getThis()) == ca)
        return 3;
    return cBELibrary_profileLib[profile][cb].aggression[ca].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_applyAggression);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void func_001E2A08(int charID, int level);
extern signed char D_005305F9[];

/**
 * Raises one rider's aggression toward another after a bump or punch.
 *
 * The record is cBELibrary_profileLib[profile of @p iRider1][character of @p iRider1].aggression[character of @p iRider2]: {state, level,
 * ticks}. @p event adds 1, 2, 4 or 6 ticks and the level becomes ticks / 5. When the level rises, rider 0 notifies
 * func_001E2A08(other character, ticks) unless D_005305F9 is set, and the state clamps it: state 0 caps at level 2,
 * state 1 drops back to level 2 and moves to state 3 on reaching 4, state 2 stays within 1-3 and state 3 within 1-4
 * (ticks reset to 5 x level). The lower-bound checks can only fire for a negative old level.
 *
 * @param iRider1  the rider who is angered (On Tour iRider1)
 * @param iRider2  the rider they are angry at (On Tour iRider2)
 * @param event  the contact (On Tour eBEAggBehaviors): 0 mild, 1 moderate, 2 hard, 3 Bertuzzi
 */
extern "C" void cBEAggressionInterface_applyAggression(void* self, int iRider1, int iRider2, int event)
{
    int profile = cBELibrary_getProfileIndex(iRider1);
    int ca = cBELibrary_getCharacterID(iRider1);
    int cb = cBELibrary_getCharacterID(iRider2);
    int add = 0;
    int oldLvl = cBELibrary_profileLib[profile][ca].aggression[cb].v[1];
    int pts = cBELibrary_profileLib[profile][ca].aggression[cb].v[2];
    int st = cBELibrary_profileLib[profile][ca].aggression[cb].v[0];
    switch (event) {
    case 0:
        add = 1;
        break;
    case 1:
        add = 2;
        break;
    case 2:
        add = 4;
        break;
    case 3:
        add = 6;
        break;
    }
    pts += add;
    int lv = pts / 5;
    int lvl = lv;
    if (oldLvl < lvl) {
        if (iRider1 == 0 && D_005305F9[0] == 0) {
            func_001E2A08(cb, pts);
        }
        switch (st) {
        case 0:
            if (lv >= 3) {
                lvl = 2;
                pts = 10;
            }
            break;
        case 1:
            if (lv >= 4) {
                lvl = 2;
                pts = 10;
                st = 3;
            }
            break;
        case 2:
            if (lv <= 0) {
                lvl = 1;
                pts = 5;
            } else if (lv >= 4) {
                lvl = 3;
                pts = 15;
            }
            break;
        case 3:
            if (lv <= 0) {
                lvl = 1;
                pts = 5;
            } else if (lv >= 5) {
                lvl = 4;
                pts = 20;
            }
            break;
        }
    }
    cBELibrary_profileLib[profile][ca].aggression[cb].v[2] = pts;
    cBELibrary_profileLib[profile][ca].aggression[cb].v[1] = lvl;
    cBELibrary_profileLib[profile][ca].aggression[cb].v[0] = st;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_regressAggression);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void func_001E2A08(int charID, int level);
extern sAggrCharProfile cBELibrary_profileLib[][10];

struct sRaceSettings_155E58 {
    char pad_0x00[0x30];
    int numPlayers;     // 0x30
    char pad_0x34[0x15];
    signed char b49;    // 0x49
};
extern sRaceSettings_155E58 cBELibrary_raceDB;

/**
 * Lets every rider's aggression cool down: each relation loses 3 ticks (not below 0) and its level is recomputed.
 *
 * For each of the race's riders (cBELibrary_raceDB +0x30) and each of the 10 characters, the relation record's ticks
 * become ticks - 3 and the level ticks / 5, then the state clamps them as in cBEAggressionInterface_applyAggression
 * (state 0 caps at level 2, state 1 moves to state 3 at level 4, state 2 stays within 1-3, state 3 drops to state 1 at
 * level 0 and caps at 4). Every relation is reported to func_001E2A08(character, ticks) unless the game-path byte
 * (cBELibrary_raceDB +0x49) is set.
 */
// Re-clamps every player's aggression level towards the other riders to the limits of its current state.
extern "C" void cBEAggressionInterface_regressAggression(void)
{
    for (int i = 0; i < cBELibrary_raceDB.numPlayers; i++) {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++) {
            int ca = cBELibrary_getCharacterID(i);
            int pts = cBELibrary_profileLib[profile][ca].aggression[j].v[2] - 3;
            if (pts < 0)
                pts = 0;
            int st = cBELibrary_profileLib[profile][ca].aggression[j].v[0];
            int lvl = pts / 5;
            if (cBELibrary_raceDB.b49 == 0) {
                func_001E2A08(j, pts);
            }
            switch (st) {
            case 0:
                if (lvl >= 3) {
                    lvl = 2;
                    pts = 10;
                }
                break;
            case 1:
                if (lvl >= 4) {
                    lvl = 2;
                    pts = 10;
                    st = 3;
                }
                break;
            case 2:
                if (lvl <= 0) {
                    lvl = 1;
                } else if (lvl >= 4) {
                    lvl = 3;
                    pts = 15;
                }
                break;
            case 3:
                if (lvl == 0) {
                    st = 1;
                    lvl = 3;
                    pts = 15;
                } else if (lvl >= 5) {
                    lvl = 4;
                    pts = 20;
                }
                break;
            }
            cBELibrary_profileLib[profile][ca].aggression[j].v[0] = st;
            cBELibrary_profileLib[profile][ca].aggression[j].v[2] = pts;
            cBELibrary_profileLib[profile][ca].aggression[j].v[1] = lvl;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_writeToReplayFrame);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305E0[];

struct sStreamVE_1560A0 { short delta; short index; void (*fn)(void*, void*, int); };

/**
 * Writes the aggression state to a replay stream: the rider count (D_005305E0, 4 bytes), then for each rider its
 * profile's ten 3-byte relation records (one per character).
 *
 * Each write goes through vtable slot 1 of @p stream (the write method, as in cBEInterface_writeToReplayFrame).
 * cBEAggressionInterface_readFromReplayFrame reads the same layout.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cBEAggressionInterface_writeToReplayFrame(void* self, cReplayFramePtr* stream)
{
    int n = D_005305E0[0];
    sStreamVE_1560A0* vt = *(sStreamVE_1560A0**)stream;
    vt[1].fn((char*)stream + vt[1].delta, &n, 4);
    for (int i = 0; i < n; i++)
    {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++)
        {
            int characterId = cBELibrary_getCharacterID(i);
            sStreamVE_1560A0* vt2 = *(sStreamVE_1560A0**)stream;
            vt2[1].fn((char*)stream + vt2[1].delta, &cBELibrary_profileLib[profile][characterId].aggression[j], 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_readFromReplayFrame);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sStreamVE_1561B0 { short delta; short index; void (*fn)(void*, void*, int); };

/**
 * Reads the aggression state back from a replay stream: a rider count (4 bytes), then for each of that many riders
 * its profile's ten 3-byte relation records.
 *
 * Each read goes through vtable slot 2 of @p stream (the read method). The count read is not stored anywhere.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cBEAggressionInterface_readFromReplayFrame(void* self, cReplayFramePtr* stream)
{
    int n;
    sStreamVE_1561B0* vt = *(sStreamVE_1561B0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, &n, 4);
    for (int i = 0; i < n; i++)
    {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++)
        {
            int charId = cBELibrary_getCharacterID(i);
            sStreamVE_1561B0* vt2 = *(sStreamVE_1561B0**)stream;
            vt2[2].fn((char*)stream + vt2[2].delta, &cBELibrary_profileLib[profile][charId].aggression[j], 3);
        }
    }
}
#endif


//100%
INCLUDE_ASM("be/beintaggression", cBEState_gotoNextState);
#ifdef SKIP_ASM
extern int cBE_currentState;
extern char* cBE_aBEState[];

/**
 * Moves the back end to the next state: calls the current state's onExit, makes @p self's nextState current and calls
 * the new state's onEnter.
 *
 * The states are cBE_aBEState[cBE_currentState]; onExit and onEnter are vtable slots 3 and 4 (vtable pointer at +0x4
 * of each state).
 */
extern "C" void cBEState_gotoNextState(int *self) {
    char* obj;
    char* vtbl;
    char* obj2;
    char* vtbl2;
    int next;

    obj = cBE_aBEState[cBE_currentState];
    vtbl = *(char**)(obj + 4);
    (*(void (**)(void*))(vtbl + 0x1C))(obj + *(short*)(vtbl + 0x18));
    next = *self;
    cBE_currentState = next;
    obj2 = cBE_aBEState[next];
    vtbl2 = *(char**)(obj2 + 4);
    (*(void (**)(void*))(vtbl2 + 0x24))(obj2 + *(short*)(vtbl2 + 0x20));
}
#endif
