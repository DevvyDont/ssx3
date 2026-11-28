#include "vtable.h"
#include "common.h"
#include "be/be.h"
#include "be/bestate.h"
#include "be/bedbprofile.h"
#include "be/beintnewrace.h"
#include "be/beintaggression.h"
#include "replay/frameptr.h"
#include "replay/bxreplayframe.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEAggressionInt[];
extern void* cBEAggressionInterface_vtbl[16];


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
        mem->bIsActive = 0;
        (*(void**)&VPTR_OF(cBEInterface, mem)) = cBEAggressionInterface_vtbl;
        cBEAggressionInterface_instance = (cBEAggressionInterface*)(mem);
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
void cBEAggressionInterface_submit(void* vself)
{
    cBEAggressionInterface* self = (cBEAggressionInterface*)vself;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the aggression interface's discard (slot 2 of its vtable cBEAggressionInterface_vtbl).
 */
void cBEAggressionInterface::discard()
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

extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns how @p rider feels about @p targetRider: the state byte of @p rider's relation record for the target's
 * character, or 2 when the target is the rival of the current peak.
 *
 * The record is cBELibrary_profileLib[profile of @p rider][character of @p rider].mRelations[character of
 * @p targetRider] (3 bytes: state, level, ticks; see cBEAggressionInterface_applyAggression). The rival comes from
 * cBENewRaceInterface_getRival on the new-race interface.
 *
 * @param targetRider  the rider being looked at
 * @param rider  the rider whose feelings are read
 * @return the relation state (0-3; On Tour friend, neutral, rival, enemy)
 */
int cBEAggressionInterface::getState(int targetRider, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int ca = cBELibrary_getCharacterID(targetRider);
    int cb = cBELibrary_getCharacterID(rider);
    if (cBENewRaceInterface_getRival(cBENewRaceInterface_getThis()) == ca)
        return 2;
    return cBELibrary_profileLib[profile][cb].mRelations[ca].aggState;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_getAggression);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
void* cBENewRaceInterface_getThis();
extern "C" int cBENewRaceInterface_getRival(void* race);

extern cBECharProfileDB cBELibrary_profileLib[][10];

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
int cBEAggressionInterface::getAggression(int targetRider, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int ca = cBELibrary_getCharacterID(targetRider);
    int cb = cBELibrary_getCharacterID(rider);
    if (cBENewRaceInterface_getRival(cBENewRaceInterface_getThis()) == ca)
        return 3;
    return cBELibrary_profileLib[profile][cb].mRelations[ca].aggLevel;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_applyAggression);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void queueAggressionMessage(int charID, int level);
extern signed char D_005305F9[];

/**
 * Raises one rider's aggression toward another after a bump or punch.
 *
 * The record is cBELibrary_profileLib[profile of @p iRider1][character of @p iRider1].mRelations[character of @p iRider2]: {state, level,
 * ticks}. @p event adds 1, 2, 4 or 6 ticks and the level becomes ticks / 5. When the level rises, rider 0 notifies
 * queueAggressionMessage(other character, ticks) unless D_005305F9 is set, and the state clamps it: state 0 caps at level 2,
 * state 1 drops back to level 2 and moves to state 3 on reaching 4, state 2 stays within 1-3 and state 3 within 1-4
 * (ticks reset to 5 x level). The lower-bound checks can only fire for a negative old level.
 *
 * @param iRider1  the rider who is angered (On Tour iRider1)
 * @param iRider2  the rider they are angry at (On Tour iRider2)
 * @param event  the contact (eBEAggBehaviors): 0 mild, 1 moderate, 2 hard, 3 Bertuzzi
 */
void cBEAggressionInterface::applyAggression(int iRider1, int iRider2, int event)
{
    int profile = cBELibrary_getProfileIndex(iRider1);
    int ca = cBELibrary_getCharacterID(iRider1);
    int cb = cBELibrary_getCharacterID(iRider2);
    int add = 0;
    int oldLvl = cBELibrary_profileLib[profile][ca].mRelations[cb].aggLevel;
    int pts = cBELibrary_profileLib[profile][ca].mRelations[cb].tick;
    int st = cBELibrary_profileLib[profile][ca].mRelations[cb].aggState;
    switch (event) {
    case kAGG_MILDCONTACT:
        add = 1;
        break;
    case kAGG_MODCONTACT:
        add = 2;
        break;
    case kAGG_HARDCONTACT:
        add = 4;
        break;
    case kAGG_BERTUZZI:
        add = 6;
        break;
    }
    pts += add;
    int lv = pts / 5;
    int lvl = lv;
    if (oldLvl < lvl) {
        if (iRider1 == 0 && D_005305F9[0] == 0) {
            queueAggressionMessage(cb, pts);
        }
        switch (st) {
        case kAGG_FRIEND:
            if (lv >= 3) {
                lvl = 2;
                pts = 10;
            }
            break;
        case kAGG_NEUTRAL:
            if (lv >= 4) {
                lvl = 2;
                pts = 10;
                st = kAGG_ENEMY;
            }
            break;
        case kAGG_RIVAL:
            if (lv <= 0) {
                lvl = 1;
                pts = 5;
            } else if (lv >= 4) {
                lvl = 3;
                pts = 15;
            }
            break;
        case kAGG_ENEMY:
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
    cBELibrary_profileLib[profile][ca].mRelations[cb].tick = pts;
    cBELibrary_profileLib[profile][ca].mRelations[cb].aggLevel = lvl;
    cBELibrary_profileLib[profile][ca].mRelations[cb].aggState = st;
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_regressAggression);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void queueAggressionMessage(int charID, int level);
extern cBECharProfileDB cBELibrary_profileLib[][10];

extern cBERaceDB cBELibrary_raceDB;

/**
 * Lets every rider's aggression cool down: each relation loses 3 ticks (not below 0) and its level is recomputed.
 *
 * For each of the race's riders (cBELibrary_raceDB +0x30) and each of the 10 characters, the relation record's ticks
 * become ticks - 3 and the level ticks / 5, then the state clamps them as in cBEAggressionInterface_applyAggression
 * (state 0 caps at level 2, state 1 moves to state 3 at level 4, state 2 stays within 1-3, state 3 drops to state 1 at
 * level 0 and caps at 4). Every relation is reported to queueAggressionMessage(character, ticks) unless the game-path byte
 * (cBELibrary_raceDB +0x49) is set.
 */
// Re-clamps every player's aggression level towards the other riders to the limits of its current state.
extern "C" void cBEAggressionInterface_regressAggression(void)
{
    for (int i = 0; i < cBELibrary_raceDB.iNumberRiders; i++) {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++) {
            int ca = cBELibrary_getCharacterID(i);
            int pts = cBELibrary_profileLib[profile][ca].mRelations[j].tick - 3;
            if (pts < 0)
                pts = 0;
            int st = cBELibrary_profileLib[profile][ca].mRelations[j].aggState;
            int lvl = pts / 5;
            if (cBELibrary_raceDB.cGamePath == 0) {
                queueAggressionMessage(j, pts);
            }
            switch (st) {
            case kAGG_FRIEND:
                if (lvl >= 3) {
                    lvl = 2;
                    pts = 10;
                }
                break;
            case kAGG_NEUTRAL:
                if (lvl >= 4) {
                    lvl = 2;
                    pts = 10;
                    st = kAGG_ENEMY;
                }
                break;
            case kAGG_RIVAL:
                if (lvl <= 0) {
                    lvl = 1;
                } else if (lvl >= 4) {
                    lvl = 3;
                    pts = 15;
                }
                break;
            case kAGG_ENEMY:
                if (lvl == 0) {
                    st = kAGG_NEUTRAL;
                    lvl = 3;
                    pts = 15;
                } else if (lvl >= 5) {
                    lvl = 4;
                    pts = 20;
                }
                break;
            }
            cBELibrary_profileLib[profile][ca].mRelations[j].aggState = st;
            cBELibrary_profileLib[profile][ca].mRelations[j].tick = pts;
            cBELibrary_profileLib[profile][ca].mRelations[j].aggLevel = lvl;
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
void cBEAggressionInterface::writeToReplayFrame(cBxReplayFrame* stream)
{
    int n = D_005305E0[0];
    stream->write((const char*)&n, 4);
    for (int i = 0; i < n; i++)
    {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++)
        {
            int characterId = cBELibrary_getCharacterID(i);
            stream->write((const char*)&cBELibrary_profileLib[profile][characterId].mRelations[j], 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintaggression", cBEAggressionInterface_readFromReplayFrame);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);


/**
 * Reads the aggression state back from a replay stream: a rider count (4 bytes), then for each of that many riders
 * its profile's ten 3-byte relation records.
 *
 * Each read goes through vtable slot 2 of @p stream (the read method). The count read is not stored anywhere.
 *
 * @param stream  the replay frame stream
 */
void cBEAggressionInterface::readFromReplayFrame(cBxReplayFrame* stream)
{
    int n;
    stream->read((char*)&n, 4);
    for (int i = 0; i < n; i++)
    {
        int profile = cBELibrary_getProfileIndex(i);
        for (int j = 0; j < 10; j++)
        {
            int charId = cBELibrary_getCharacterID(i);
            stream->read((char*)&cBELibrary_profileLib[profile][charId].mRelations[j], 3);
        }
    }
}
#endif


//100%
INCLUDE_ASM("be/beintaggression", cBEState_gotoNextState);
#ifdef SKIP_ASM

/**
 * Moves the back end to the next state: calls the current state's onExit, makes this state's nextState current
 * and calls the new state's onEnter.
 *
 * The states are cBE_aBEState[cBE_currentState]; onExit and onEnter are vtable slots 3 and 4 (vtable pointer at +0x4
 * of each state).
 */
void cBEState::gotoNextState()
{
    cBEState* from = cBE_aBEState[cBE_currentState];
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)from->mVtbl;
    vt[kBEStateVt_onExit].fn(vthis(from, vt[kBEStateVt_onExit]));
    int next = nextState;
    cBE_currentState = next;
    cBEState* to = cBE_aBEState[next];
    sVEntry<void(void*)>* vt2 = (sVEntry<void(void*)>*)to->mVtbl;
    vt2[kBEStateVt_onEnter].fn(vthis(to, vt2[kBEStateVt_onEnter]));
}
#endif
