#include "common.h"
#include "be/beintnetwork.h"
#include "be/beintreward.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sStateSingleton {
    void* field_0x0;
    void* vtable;
};

extern const char str_BEPreRace[];
extern void* cBEPostRaceState_vtbl[16];
extern void* D_004A1254;

//99.86%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_getState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the cBEPostRaceState singleton, creating it on first use.
 *
 * The first call allocates 8 bytes tagged "BEPreRace" (the post-race state reuses the pre-race tag), stores
 * @p self in the first word and the cBEPostRaceState vtable in the second, and keeps the object in D_004A1254.
 * cBE_getBE() passes the state index (2) as @p self.
 *
 * @return the cBEPostRaceState object
 */
void* cBEPostRaceState_getState(void* self)
{
    if (D_004A1254 == 0) {
        sStateSingleton* mem = (sStateSingleton*)cMemMan_alloc(8, str_BEPreRace, 0, 0);
        mem->field_0x0 = self;
        mem->vtable = cBEPostRaceState_vtbl;
        D_004A1254 = mem;
    }
    return D_004A1254;
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
extern "C" void* func_0014ABE0();
void* cBEBAGTInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();
/**
 * Returns the back-end interface singleton for an interface kind while in the post-race state.
 *
 * Kinds: 0 new race, 1 new player, 2 character, 3 stat, 4 option, 5 save, 6 BAGT, 7 network, 8 score, 9
 * func_0014ABE0's singleton (D_004A11C8), 10 mission, 11 econ, 12 aggression, 13 reward. Retail's 9..13 are one off
 * from On Tour's eBE_Interface (kInt_Mission = 9 .. kInt_Reward = 12).
 *
 * @param kind  the interface kind (On Tour eBE_Interface interfaceType)
 * @return the interface's getThis() singleton, or 0 for an unknown kind
 */
// Returns the backend interface singleton for the given kind.
extern "C" void* cBEPostRaceState_getInterface(void* self, int kind)
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
    case 5:
        return cBESaveInterface_getThis();
    case 7:
        return cBENetworkInterface_getThis();
    case 8:
        return cBEScoreInterface_getThis();
    case 9:
        return func_0014ABE0();
    case 6:
        return cBEBAGTInterface_getThis();
    case 11:
        return cBEEconInterface_getThis();
    case 10:
        return cBEMissionInterface_getThis();
    case 12:
        return cBEAggressionInterface_getThis();
    case 13:
        return cBERewardInterface_getThis();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_onExit);
#ifdef SKIP_ASM
extern void* D_004A11C0;
extern void* D_004A1208;
extern void* D_004A11C4;
extern void* D_004A120C;
extern void* D_004A121C;
extern void* D_004A11C8;
extern void* D_004A1210;
extern void* D_004A1214;
extern void* D_004A124C;

struct sBEIface_001564B0
{
    char pad_0x00[8];
    int active;
};

/**
 * Leaves the post-race back-end state: marks its interfaces inactive.
 *
 * Clears the active flag of the new player, character, stat, option, save, network, D_004A11C8 (func_0014ABE0's
 * singleton), BAGT, econ, aggression and reward interfaces; new race, score and mission are not touched.
 */
extern "C" void cBEPostRaceState_onExit(void)
{
    ((sBEIface_001564B0*)D_004A11C0)->active = 0;
    ((sBEIface_001564B0*)D_004A1208)->active = 0;
    ((sBEIface_001564B0*)D_004A11C4)->active = 0;
    ((sBEIface_001564B0*)D_004A120C)->active = 0;
    ((sBEIface_001564B0*)D_004A121C)->active = 0;
    cBENetworkInterface_instance->bIsActive = 0;
    ((sBEIface_001564B0*)D_004A11C8)->active = 0;
    ((sBEIface_001564B0*)D_004A1210)->active = 0;
    ((sBEIface_001564B0*)D_004A1214)->active = 0;
    ((sBEIface_001564B0*)D_004A124C)->active = 0;
    ((sBEIface_001564B0*)cBERewardInterface_instance)->active = 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPostRaceState_onEnter);
#ifdef SKIP_ASM
extern void* D_004A11C0;
extern void* D_004A1208;
extern void* D_004A11C4;
extern void* D_004A120C;
extern void* D_004A121C;
extern void* D_004A11C8;
extern void* D_004A1210;
extern void* D_004A1214;
extern void* D_004A124C;

struct sBEIface_00156510
{
    char pad_0x00[8];
    int active;
};

/**
 * Enters the post-race back-end state: marks its interfaces active.
 *
 * Sets the active flag of the new player, character, stat, option, save, network, D_004A11C8 (func_0014ABE0's
 * singleton), BAGT, econ, aggression and reward interfaces (the same set cBEPostRaceState_onExit clears).
 */
extern "C" void cBEPostRaceState_onEnter(void)
{
    ((sBEIface_00156510*)D_004A11C0)->active = 1;
    ((sBEIface_00156510*)D_004A1208)->active = 1;
    ((sBEIface_00156510*)D_004A11C4)->active = 1;
    ((sBEIface_00156510*)D_004A120C)->active = 1;
    ((sBEIface_00156510*)D_004A121C)->active = 1;
    cBENetworkInterface_instance->bIsActive = 1;
    ((sBEIface_00156510*)D_004A11C8)->active = 1;
    ((sBEIface_00156510*)D_004A1210)->active = 1;
    ((sBEIface_00156510*)D_004A1214)->active = 1;
    ((sBEIface_00156510*)D_004A124C)->active = 1;
    ((sBEIface_00156510*)cBERewardInterface_instance)->active = 1;
}
#endif

extern void* cBEPreRaceState_vtbl[16];
extern void* D_004A1258;

//99.86%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_getState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the cBEPreRaceState singleton, creating it on first use.
 *
 * The first call allocates 8 bytes tagged "BEPreRace", stores @p self in the first word and the cBEPreRaceState
 * vtable in the second, and keeps the object in D_004A1258. cBE_getBE() passes the state index (0) as @p self.
 *
 * @return the cBEPreRaceState object
 */
void* cBEPreRaceState_getState(void* self)
{
    if (D_004A1258 == 0) {
        sStateSingleton* mem = (sStateSingleton*)cMemMan_alloc(8, str_BEPreRace, 0, 0);
        mem->field_0x0 = self;
        mem->vtable = cBEPreRaceState_vtbl;
        D_004A1258 = mem;
    }
    return D_004A1258;
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
extern "C" void* func_0014ABE0();
void* cBEBAGTInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();
/**
 * Returns the back-end interface singleton for an interface kind while in the pre-race state.
 *
 * Same table as cBEPostRaceState_getInterface: 0 new race, 1 new player, 2 character, 3 stat, 4 option, 5 save, 6
 * BAGT, 7 network, 8 score, 9 func_0014ABE0's singleton, 10 mission, 11 econ, 12 aggression, 13 reward.
 *
 * @param kind  the interface kind (On Tour eBE_Interface interfaceType)
 * @return the interface's getThis() singleton, or 0 for an unknown kind
 */
// Returns the backend interface singleton for the given kind.
extern "C" void* cBEPreRaceState_getInterface(void* self, int kind)
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
    case 5:
        return cBESaveInterface_getThis();
    case 7:
        return cBENetworkInterface_getThis();
    case 8:
        return cBEScoreInterface_getThis();
    case 9:
        return func_0014ABE0();
    case 6:
        return cBEBAGTInterface_getThis();
    case 11:
        return cBEEconInterface_getThis();
    case 10:
        return cBEMissionInterface_getThis();
    case 12:
        return cBEAggressionInterface_getThis();
    case 13:
        return cBERewardInterface_getThis();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_onExit);
#ifdef SKIP_ASM
extern void* D_004A11B4;
extern void* D_004A11C0;
extern void* D_004A1208;
extern void* D_004A11C4;
extern void* D_004A120C;
extern void* D_004A121C;
extern void* D_004A11C8;
extern void* D_004A1210;
extern void* D_004A1214;
extern void* D_004A124C;

struct sBEIface_001566E8
{
    char pad_0x00[8];
    int active;
};

/**
 * Leaves the pre-race back-end state: marks its interfaces inactive.
 *
 * Clears the active flag of the new race, new player, character, stat, option, save, network, D_004A11C8
 * (func_0014ABE0's singleton), BAGT, econ, aggression and reward interfaces; score and mission are not touched.
 */
extern "C" void cBEPreRaceState_onExit(void)
{
    ((sBEIface_001566E8*)D_004A11B4)->active = 0;
    ((sBEIface_001566E8*)D_004A11C0)->active = 0;
    ((sBEIface_001566E8*)D_004A1208)->active = 0;
    ((sBEIface_001566E8*)D_004A11C4)->active = 0;
    ((sBEIface_001566E8*)D_004A120C)->active = 0;
    ((sBEIface_001566E8*)D_004A121C)->active = 0;
    cBENetworkInterface_instance->bIsActive = 0;
    ((sBEIface_001566E8*)D_004A11C8)->active = 0;
    ((sBEIface_001566E8*)D_004A1210)->active = 0;
    ((sBEIface_001566E8*)D_004A1214)->active = 0;
    ((sBEIface_001566E8*)D_004A124C)->active = 0;
    ((sBEIface_001566E8*)cBERewardInterface_instance)->active = 0;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", cBEPreRaceState_onEnter);
#ifdef SKIP_ASM
extern void* D_004A11B4;
extern void* D_004A11C0;
extern void* D_004A1208;
extern void* D_004A11C4;
extern void* D_004A120C;
extern void* D_004A121C;
extern void* D_004A11C8;
extern void* D_004A1210;
extern void* D_004A1214;
extern void* D_004A124C;

struct sBEIface_00156750
{
    char pad_0x00[8];
    int active;
};

/**
 * Enters the pre-race back-end state: marks its interfaces active.
 *
 * Sets the active flag of the new race, new player, character, stat, option, save, network, D_004A11C8
 * (func_0014ABE0's singleton), BAGT, econ, aggression and reward interfaces (the set cBEPreRaceState_onExit
 * clears).
 */
extern "C" void cBEPreRaceState_onEnter(void)
{
    ((sBEIface_00156750*)D_004A11B4)->active = 1;
    ((sBEIface_00156750*)D_004A11C0)->active = 1;
    ((sBEIface_00156750*)D_004A1208)->active = 1;
    ((sBEIface_00156750*)D_004A11C4)->active = 1;
    ((sBEIface_00156750*)D_004A120C)->active = 1;
    ((sBEIface_00156750*)D_004A121C)->active = 1;
    cBENetworkInterface_instance->bIsActive = 1;
    ((sBEIface_00156750*)D_004A11C8)->active = 1;
    ((sBEIface_00156750*)D_004A1210)->active = 1;
    ((sBEIface_00156750*)D_004A1214)->active = 1;
    ((sBEIface_00156750*)D_004A124C)->active = 1;
    ((sBEIface_00156750*)cBERewardInterface_instance)->active = 1;
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_001567B8);
#ifdef SKIP_ASM
extern "C" void func_00151600(void* p, int i, int arg);

extern "C" void func_001567B8(void* self, int arg)
{
    int i;
    for (i = 0; i < 10; i++) {
        func_00151600((char*)self + i * 0xF88, i, arg);
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_00156810);
#ifdef SKIP_ASM
extern "C" void func_001519E0(void* p, int i);

extern "C" void func_00156810(void* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        func_001519E0((char*)self + i * 0xF88, i);
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_00156858);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern int D_004A11E8;

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

extern "C" void func_00156858(char* dst, char* src)
{
    int i;
    for (i = 0; i < 10; i++) {
        char* d = dst + i * 0xF88;
        short* index = ((sProfile_156858*)d)->index;
        MEM_copy(d, src + i * 0xF88, 0xF88);
        ((sProfile_156858*)d)->index = index;
        int j;
        for (j = 0; j < D_004A11E8; j++)
            index[j] = -1;
        sEnt_156858* e = (sEnt_156858*)(dst + i * 0xF88 + 0x290);
        int n;
        for (n = 0; n < 0x20D; n++, e++) {
            short k = e->id;
            if (k >= 0)
                index[k] = n;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_00156950);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern signed char D_004A125C;
extern char D_004C4090[];

extern "C" void func_00156950(char* base, int idx)
{
    D_004A125C = idx;
    MEM_copy(D_004C4090, base + idx * 0xF88, 0xF88);
}
#endif

//100%
INCLUDE_ASM("be/beprepoststate", func_00156988);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int n);
extern signed char D_004A125C;
extern char D_004C4090[];

extern "C" void func_00156988(char* base)
{
    MEM_copy(base + D_004A125C * 0xF88, D_004C4090, 0xF88);
}
#endif

