#include "common.h"
#include "be/beprofilelib.h"
#include "be/bestate.h"
#include "be/beintsavefile.h"
#include "be/beintaggression.h"
#include "be/beintnewrace.h"
#include "be/beintcharacter.h"
#include "be/beintbagt.h"
#include "be/beintoption.h"
#include "be/beintecon.h"
#include "be/beintstat.h"
#include "be/beintplayer.h"
#include "be/beintnetwork.h"
#include "be/beintreward.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);


extern const char str_BEPreRace[];
extern void* cBEPostRaceState_vtbl[16];
extern void* cBEPostRaceState_instance;

//99.86%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_getState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the cBEPostRaceState singleton, creating it on first use.
 *
 * The first call allocates 8 bytes tagged "BEPreRace" (the post-race state reuses the pre-race tag), stores
 * @p self in the first word and the cBEPostRaceState vtable in the second, and keeps the object in cBEPostRaceState_instance.
 * cBE_getBE() passes the state index (2) as @p self.
 *
 * @return the cBEPostRaceState object
 */
void* cBEPostRaceState_getState(void* self)
{
    if (cBEPostRaceState_instance == 0) {
        cBEState* mem = (cBEState*)cMemMan_alloc(8, str_BEPreRace, 0, 0);
        (*(void**)&mem->nextState) = self;
        (*(void**)&mem->mVtbl) = cBEPostRaceState_vtbl;
        cBEPostRaceState_instance = mem;
    }
    return cBEPostRaceState_instance;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_getInterface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void* cBENewRaceInterface_getThis();
void* cBENewPlayerInterface_getThis();
extern "C" void* cBECharacterInterface_getThis();
extern "C" void* cBEStatInterface_getThis();
void* cBEOptionInterface_getThis();
void* cBESaveInterface_getThis();
void* cBENetworkInterface_getThis();
void* cBEScoreInterface_getThis();
extern "C" void* cBEBoltInterface_getThis();
void* cBEBAGTInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();
/**
 * Returns the back-end interface singleton for an interface kind while in the post-race state.
 *
 * Kinds: 0 new race, 1 new player, 2 character, 3 stat, 4 option, 5 save, 6 BAGT, 7 network, 8 score, 9
 * cBEBoltInterface_getThis's singleton (cBEBoltInterface_instance), 10 mission, 11 econ, 12 aggression, 13 reward. Retail's 9..13 are one off
 * from On Tour's eBE_Interface (kInt_Mission = 9 .. kInt_Reward = 12).
 *
 * @param kind  the interface kind (eBE_Interface; On Tour interfaceType)
 * @return the interface's getThis() singleton, or 0 for an unknown kind
 */
// Returns the backend interface singleton for the given kind.
void* cBEPostRaceState::getInterface(int kind)
{
    switch (kind) {
    case kInt_Newrace:
        return cBENewRaceInterface_getThis();
    case kInt_Newplayer:
        return cBENewPlayerInterface_getThis();
    case kInt_Character:
        return cBECharacterInterface_getThis();
    case kInt_Stat:
        return cBEStatInterface_getThis();
    case kInt_Option:
        return cBEOptionInterface_getThis();
    case kInt_Save:
        return cBESaveInterface_getThis();
    case kInt_Network:
        return cBENetworkInterface_getThis();
    case kInt_Score:
        return cBEScoreInterface_getThis();
    case kInt_Bolt:
        return cBEBoltInterface_getThis();
    case kInt_BAGT:
        return cBEBAGTInterface_getThis();
    case kInt_Economy:
        return cBEEconInterface_getThis();
    case kInt_Mission:
        return cBEMissionInterface_getThis();
    case kInt_Aggression:
        return cBEAggressionInterface_getThis();
    case kInt_Reward:
        return cBERewardInterface_getThis();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_onExit);
#ifdef SKIP_ASM
extern void* cBEBoltInterface_instance;


/**
 * Leaves the post-race back-end state: marks its interfaces inactive.
 *
 * Clears the active flag of the new player, character, stat, option, save, network, cBEBoltInterface_instance (cBEBoltInterface_getThis's
 * singleton), BAGT, econ, aggression and reward interfaces; new race, score and mission are not touched.
 */
extern "C" void cBEPostRaceState_onExit(void)
{
    cBENewPlayerInterface_instance->bIsActive = 0;
    cBECharacterInterface_instance->bIsActive = 0;
    cBEStatInterface_instance->bIsActive = 0;
    cBEOptionInterface_instance->bIsActive = 0;
    cBESaveInterface_instance->bIsActive = 0;
    cBENetworkInterface_instance->bIsActive = 0;
    ((cBERewardInterface*)cBEBoltInterface_instance)->bIsActive = 0;
    cBEBAGTInterface_instance->bIsActive = 0;
    cBEEconInterface_instance->bIsActive = 0;
    cBEAggressionInterface_instance->bIsActive = 0;
    (cBERewardInterface_instance)->bIsActive = 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_onEnter);
#ifdef SKIP_ASM
extern void* cBEBoltInterface_instance;


/**
 * Enters the post-race back-end state: marks its interfaces active.
 *
 * Sets the active flag of the new player, character, stat, option, save, network, cBEBoltInterface_instance (cBEBoltInterface_getThis's
 * singleton), BAGT, econ, aggression and reward interfaces (the same set cBEPostRaceState_onExit clears).
 */
extern "C" void cBEPostRaceState_onEnter(void)
{
    cBENewPlayerInterface_instance->bIsActive = 1;
    cBECharacterInterface_instance->bIsActive = 1;
    cBEStatInterface_instance->bIsActive = 1;
    cBEOptionInterface_instance->bIsActive = 1;
    cBESaveInterface_instance->bIsActive = 1;
    cBENetworkInterface_instance->bIsActive = 1;
    ((cBERewardInterface*)cBEBoltInterface_instance)->bIsActive = 1;
    cBEBAGTInterface_instance->bIsActive = 1;
    cBEEconInterface_instance->bIsActive = 1;
    cBEAggressionInterface_instance->bIsActive = 1;
    (cBERewardInterface_instance)->bIsActive = 1;
}
#endif

extern void* cBEPreRaceState_vtbl[16];
extern void* cBEPreRaceState_instance;

//99.86%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_getState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the cBEPreRaceState singleton, creating it on first use.
 *
 * The first call allocates 8 bytes tagged "BEPreRace", stores @p self in the first word and the cBEPreRaceState
 * vtable in the second, and keeps the object in cBEPreRaceState_instance. cBE_getBE() passes the state index (0) as @p self.
 *
 * @return the cBEPreRaceState object
 */
void* cBEPreRaceState_getState(void* self)
{
    if (cBEPreRaceState_instance == 0) {
        cBEState* mem = (cBEState*)cMemMan_alloc(8, str_BEPreRace, 0, 0);
        (*(void**)&mem->nextState) = self;
        (*(void**)&mem->mVtbl) = cBEPreRaceState_vtbl;
        cBEPreRaceState_instance = mem;
    }
    return cBEPreRaceState_instance;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_getInterface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void* cBENewRaceInterface_getThis();
void* cBENewPlayerInterface_getThis();
extern "C" void* cBECharacterInterface_getThis();
extern "C" void* cBEStatInterface_getThis();
void* cBEOptionInterface_getThis();
void* cBESaveInterface_getThis();
void* cBENetworkInterface_getThis();
void* cBEScoreInterface_getThis();
extern "C" void* cBEBoltInterface_getThis();
void* cBEBAGTInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();
/**
 * Returns the back-end interface singleton for an interface kind while in the pre-race state.
 *
 * Same table as cBEPostRaceState_getInterface: 0 new race, 1 new player, 2 character, 3 stat, 4 option, 5 save, 6
 * BAGT, 7 network, 8 score, 9 cBEBoltInterface_getThis's singleton, 10 mission, 11 econ, 12 aggression, 13 reward.
 *
 * @param kind  the interface kind (eBE_Interface; On Tour interfaceType)
 * @return the interface's getThis() singleton, or 0 for an unknown kind
 */
// Returns the backend interface singleton for the given kind.
void* cBEPreRaceState::getInterface(int kind)
{
    switch (kind) {
    case kInt_Newrace:
        return cBENewRaceInterface_getThis();
    case kInt_Newplayer:
        return cBENewPlayerInterface_getThis();
    case kInt_Character:
        return cBECharacterInterface_getThis();
    case kInt_Stat:
        return cBEStatInterface_getThis();
    case kInt_Option:
        return cBEOptionInterface_getThis();
    case kInt_Save:
        return cBESaveInterface_getThis();
    case kInt_Network:
        return cBENetworkInterface_getThis();
    case kInt_Score:
        return cBEScoreInterface_getThis();
    case kInt_Bolt:
        return cBEBoltInterface_getThis();
    case kInt_BAGT:
        return cBEBAGTInterface_getThis();
    case kInt_Economy:
        return cBEEconInterface_getThis();
    case kInt_Mission:
        return cBEMissionInterface_getThis();
    case kInt_Aggression:
        return cBEAggressionInterface_getThis();
    case kInt_Reward:
        return cBERewardInterface_getThis();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_onExit);
#ifdef SKIP_ASM
extern void* cBEBoltInterface_instance;


/**
 * Leaves the pre-race back-end state: marks its interfaces inactive.
 *
 * Clears the active flag of the new race, new player, character, stat, option, save, network, cBEBoltInterface_instance
 * (cBEBoltInterface_getThis's singleton), BAGT, econ, aggression and reward interfaces; score and mission are not touched.
 */
extern "C" void cBEPreRaceState_onExit(void)
{
    cBENewRaceInterface_instance->bIsActive = 0;
    cBENewPlayerInterface_instance->bIsActive = 0;
    cBECharacterInterface_instance->bIsActive = 0;
    cBEStatInterface_instance->bIsActive = 0;
    cBEOptionInterface_instance->bIsActive = 0;
    cBESaveInterface_instance->bIsActive = 0;
    cBENetworkInterface_instance->bIsActive = 0;
    ((cBERewardInterface*)cBEBoltInterface_instance)->bIsActive = 0;
    cBEBAGTInterface_instance->bIsActive = 0;
    cBEEconInterface_instance->bIsActive = 0;
    cBEAggressionInterface_instance->bIsActive = 0;
    (cBERewardInterface_instance)->bIsActive = 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_onEnter);
#ifdef SKIP_ASM
extern void* cBEBoltInterface_instance;


/**
 * Enters the pre-race back-end state: marks its interfaces active.
 *
 * Sets the active flag of the new race, new player, character, stat, option, save, network, cBEBoltInterface_instance
 * (cBEBoltInterface_getThis's singleton), BAGT, econ, aggression and reward interfaces (the set cBEPreRaceState_onExit
 * clears).
 */
extern "C" void cBEPreRaceState_onEnter(void)
{
    cBENewRaceInterface_instance->bIsActive = 1;
    cBENewPlayerInterface_instance->bIsActive = 1;
    cBECharacterInterface_instance->bIsActive = 1;
    cBEStatInterface_instance->bIsActive = 1;
    cBEOptionInterface_instance->bIsActive = 1;
    cBESaveInterface_instance->bIsActive = 1;
    cBENetworkInterface_instance->bIsActive = 1;
    ((cBERewardInterface*)cBEBoltInterface_instance)->bIsActive = 1;
    cBEBAGTInterface_instance->bIsActive = 1;
    cBEEconInterface_instance->bIsActive = 1;
    cBEAggressionInterface_instance->bIsActive = 1;
    (cBERewardInterface_instance)->bIsActive = 1;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEProfileLib_init);
#ifdef SKIP_ASM
extern "C" void cBECharProfileDB_init(void* p, int i, int arg);

/**
 * Resets all ten character profiles of a player profile.
 *
 * Calls cBECharProfileDB_init (cBECharProfileDB::init) for characters 0-9 with the character index and @p reload.
 *
 * @param reload  nonzero to reload each character's item data first
 */
void cBEProfileLib::init(int reload)
{
    int i;
    for (i = 0; i < 10; i++) {
        cBECharProfileDB_init(&charProfile[i], i, reload);
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_00156810);
#ifdef SKIP_ASM
extern "C" void func_001519E0(void* p, int i);

/**
 * Runs func_001519E0 on each of a profile's ten character records (0xF88 bytes apart), passing the character index.
 *
 * func_001519E0 is the part of cBECharProfileDB_init that sets a character kind's defaults. The network interface calls
 * this when it resets a player's profile.
 */
extern "C" void func_00156810(cBEProfileLib* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        func_001519E0(&self->charProfile[i], i);
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEProfileLib_copy);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern int kBolt_Num;

struct sEnt_156858
{
    short id;
    unsigned short flags;
};

struct sProfile_156858
{
    char pad[0x288];
    short* index;       // 0x288
};

/**
 * Copies a profile's ten character records over another profile's, keeping the destination's bolt lookup tables.
 *
 * For each character, copies the 0xF88-byte record but puts back the destination's lookup pointer (+0x288), then
 * rebuilds that lookup: all kBolt_Num ids are set to -1, and each of the 525 bolt entries (+0x290) with an id of 0 or
 * more maps its id to its index.
 *
 * @param dst  the destination profile (10 character records)
 * @param src  the source profile (On Tour pSrcProfileLib)
 */
extern "C" void cBEProfileLib_copy(char* dst, char* src)
{
    int i;
    for (i = 0; i < 10; i++) {
        char* charRecord = dst + i * 3976;
        short* index = ((sProfile_156858*)charRecord)->index;
        MEM_copy(charRecord, src + i * 3976, 3976);
        ((sProfile_156858*)charRecord)->index = index;
        int j;
        for (j = 0; j < kBolt_Num; j++)
            index[j] = -1;
        sEnt_156858* boltEntry = (sEnt_156858*)(dst + i * 0xF88 + 0x290);
        int n;
        for (n = 0; n < 525; n++, boltEntry++) {
            short k = boltEntry->id;
            if (k >= 0)
                index[k] = n;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEProfileLib_backupCharProfile);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern signed char D_004A125C;
extern char cBEProfileLib_BackupCharProfile[];

/**
 * Saves one character record of a profile into the backup buffer cBEProfileLib_BackupCharProfile and remembers its character index in
 * D_004A125C.
 *
 * @param idx  the character whose record is saved
 */
void cBEProfileLib::backupCharProfile(int idx)
{
    D_004A125C = idx;
    MEM_copy(cBEProfileLib_BackupCharProfile, &charProfile[idx], 3976);
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEProfileLib_restoreCharProfile);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern signed char D_004A125C;
extern char cBEProfileLib_BackupCharProfile[];

/**
 * Copies the backup buffer cBEProfileLib_BackupCharProfile back into the profile's character record that cBEProfileLib_backupCharProfile saved (index
 * D_004A125C).
 *
 */
void cBEProfileLib::restoreCharProfile()
{
    MEM_copy(&charProfile[D_004A125C], cBEProfileLib_BackupCharProfile, 3976);
}
#endif

