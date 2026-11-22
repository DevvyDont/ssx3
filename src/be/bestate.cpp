#include "common.h"
#include "be/bestate.h"
#include "be/beintsavefile.h"
#include "be/beintaggression.h"
#include "be/beintscore.h"
#include "be/beintnewrace.h"
#include "be/beintmission.h"
#include "be/beintoption.h"
#include "be/beintecon.h"
#include "be/beintplayer.h"
#include "be/beintreward.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);


extern const char str_BERace[];
extern void* cBERaceState_vtbl[16];
extern void* cBERaceState_instance;

//99.86%
INCLUDE_ASM("be/bestate", cBERaceState_getState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the cBERaceState singleton, creating it on first use.
 *
 * The first call allocates 8 bytes tagged "BERace", stores @p self in the first word and the cBERaceState vtable
 * in the second, and keeps the object in cBERaceState_instance. cBE_getBE() passes the state index (1) as @p self.
 *
 * @return the cBERaceState object
 */
void* cBERaceState_getState(void* self)
{
    if (cBERaceState_instance == 0) {
        cBERaceState* mem = (cBERaceState*)cMemMan_alloc(8, str_BERace, 0, 0);
        (*(void**)&mem->nextState) = self;
        (*(void**)&mem->mVtbl) = cBERaceState_vtbl;
        cBERaceState_instance = mem;
    }
    return cBERaceState_instance;
}
#endif

//100%
INCLUDE_ASM("be/bestate", cBERaceState_getInterface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void* cBENewRaceInterface_getThis();
void* cBENewPlayerInterface_getThis();
extern "C" void* cBECharacterInterface_getThis();
extern "C" void* cBEStatInterface_getThis();
void* cBEOptionInterface_getThis();
void* cBEBAGTInterface_getThis();
void* cBENetworkInterface_getThis();
void* cBEScoreInterface_getThis();
extern "C" void* cBEBoltInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();
void* cBESaveInterface_getThis();

/**
 * Returns the back-end interface singleton for an interface kind while racing.
 *
 * Same table as the pre/post-race states (case order differs only): 0 new race, 1 new player, 2 character, 3 stat,
 * 4 option, 5 save, 6 BAGT, 7 network, 8 score, 9 cBEBoltInterface_getThis's singleton, 10 mission, 11 econ, 12 aggression, 13
 * reward.
 *
 * @param kind  the interface kind (On Tour eBE_Interface interfaceType)
 * @return the interface's getThis() singleton, or 0 for an unknown kind
 */
// Returns the backend interface singleton for the given kind.
extern "C" void* cBERaceState_getInterface(cBERaceState* self, int kind)
{
    switch (kind) {
    case 0:
        return cBENewRaceInterface_getThis();
    case 1:
        return cBENewPlayerInterface_getThis();
    case 2:
        return cBECharacterInterface_getThis();
    case 3:
        return cBEStatInterface_getThis();
    case 4:
        return cBEOptionInterface_getThis();
    case 6:
        return cBEBAGTInterface_getThis();
    case 7:
        return cBENetworkInterface_getThis();
    case 8:
        return cBEScoreInterface_getThis();
    case 9:
        return cBEBoltInterface_getThis();
    case 10:
        return cBEMissionInterface_getThis();
    case 11:
        return cBEEconInterface_getThis();
    case 12:
        return cBEAggressionInterface_getThis();
    case 13:
        return cBERewardInterface_getThis();
    case 5:
        return cBESaveInterface_getThis();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/bestate", cBERaceState_onExit);
#ifdef SKIP_ASM
extern cBEInterface* cBEBoltInterface_instance;

/**
 * Leaves the race back-end state: marks its interfaces inactive.
 *
 * Clears the active word (+0x8) of the option, score, cBEBoltInterface_instance (cBEBoltInterface_getThis's singleton), new player, new race,
 * mission, econ, aggression, reward and save interfaces. New player and new race are cleared here although
 * cBERaceState_onEnter never sets them.
 */
extern "C" void cBERaceState_onExit(void) {
    cBEOptionInterface_instance->bIsActive = 0;
    cBEScoreInterface_instance->bIsActive = 0;
    cBEBoltInterface_instance->bIsActive = 0;
    cBENewPlayerInterface_instance->bIsActive = 0;
    cBENewRaceInterface_instance->bIsActive = 0;
    cBEMissionInterface_instance->bIsActive = 0;
    cBEEconInterface_instance->bIsActive = 0;
    cBEAggressionInterface_instance->bIsActive = 0;
    cBERewardInterface_instance->bIsActive = 0;
    cBESaveInterface_instance->bIsActive = 0;
}
#endif

//100%
INCLUDE_ASM("be/bestate", cBERaceState_onEnter);
#ifdef SKIP_ASM
extern cBEInterface* cBEBoltInterface_instance;

/**
 * Enters the race back-end state: marks its interfaces active.
 *
 * Sets the active word (+0x8) of the option, score, cBEBoltInterface_instance (cBEBoltInterface_getThis's singleton), mission, econ,
 * aggression, reward and save interfaces; the network, player and character interfaces stay as they were.
 */
extern "C" void cBERaceState_onEnter(void) {
    cBEOptionInterface_instance->bIsActive = 1;
    cBEScoreInterface_instance->bIsActive = 1;
    cBEBoltInterface_instance->bIsActive = 1;
    cBEMissionInterface_instance->bIsActive = 1;
    cBEEconInterface_instance->bIsActive = 1;
    cBEAggressionInterface_instance->bIsActive = 1;
    cBERewardInterface_instance->bIsActive = 1;
    cBESaveInterface_instance->bIsActive = 1;
}
#endif

