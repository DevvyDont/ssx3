#ifndef BE_BESTATE_H
#define BE_BESTATE_H

// cBEPreRaceState / cBERaceState / cBEPostRaceState: the back end's three race states (before, during and after a race).
// cBE (cBE_getBE) keeps one singleton of each, created by the static getState (cBEPreRaceState_getState: the first call
// allocates 8 bytes "BEPreRace" / "BERace" / "BEPreRace" - the post-race state reuses the pre-race tag - with the cBEState
// ctor inlined: the argument goes to nextState, the vtable to 0x4) and freed by the virtual purgeState (operator delete,
// no destructor). Their virtuals hand out the BE interfaces by kind (getInterface) and run the state's enter/exit work.
// Instances: cBEPreRaceState_instance (pre-race), cBERaceState_instance (0x004A122C), cBEPostRaceState_instance (post-race) - On Tour's static
// `instance` members.
// EA's source: d:/ssx/code/source/be/bestate.cpp (preview assert path; its asserts name nextState and cBE's aBEState[]).
// Retail code: getState / getInterface / onExit / onEnter in units be/bestate (cBERaceState) and be/beprepoststate
// (cBEPreRaceState, cBEPostRaceState); the out-of-line ctors and purgeState in be/bedbreward.
//
// Size 0x8 (each): [R] the 8-byte allocations in getState; [O] On Tour's cBEState / cBEPreRaceState / cBERaceState /
// cBEPostRaceState are all 0x8 (nextState + vptr).
// Base: cBEState (On Tour: nextState, then the vptr; it declares the virtuals getInterface, purgeState, onExit, onEnter),
// written out inline; the derived classes add no members. cBEState declares the first virtual, so its vptr follows its
// data (g++ 2.95): 0x4. Vtables (4 slots: 1 getInterface, 2 purgeState, 3 onExit, 4 onEnter): cBEPreRaceState_vtbl
// 0x0045AC18, cBERaceState_vtbl 0x0045ABE8, cBEPostRaceState_vtbl 0x0045ABB8.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the inlined ctor in getState (word 0 = the argument, word 1 = the vtable) and the out-of-line ctors
//       cBE*State_cBE*State (vtable at 0x4);
//   [P] the preview's assert "nextState < kBESTATE_NULL" (be/bestate.cpp);
//   [O] SSX On Tour's DWARF (class cBEState: nextState at 0x0, size 0x8).

#include "layout.h"

struct cBEPreRaceState {
    // --- cBEState
    int nextState;                  // 0x0 [O] cBEState::nextState; [P] assert "nextState < kBESTATE_NULL"; [R] getState
                                    //     stores its argument (cBE_getBE passes a state index)
    void** mVtbl;                   // 0x4 [R] cBEPreRaceState_vtbl (getState, cBEPreRaceState_cBEPreRaceState)

    // Methods bound to their retail symbols by asm labels.
    void* getInterface(int kind) __asm__("cBEPreRaceState_getInterface");
};

CHECK_OFFSET(cBEPreRaceState, nextState, 0x0);
CHECK_OFFSET(cBEPreRaceState, mVtbl, 0x4);
CHECK_SIZE(cBEPreRaceState, 0x8);

struct cBERaceState {
    // --- cBEState
    int nextState;                  // 0x0 [O] cBEState::nextState; [P] assert; [R] getState stores its argument
    void** mVtbl;                   // 0x4 [R] cBERaceState_vtbl (getState, cBERaceState_cBERaceState)

    // Methods bound to their retail symbols by asm labels.
    void* getInterface(int kind) __asm__("cBERaceState_getInterface");
};

CHECK_OFFSET(cBERaceState, nextState, 0x0);
CHECK_OFFSET(cBERaceState, mVtbl, 0x4);
CHECK_SIZE(cBERaceState, 0x8);

struct cBEPostRaceState {
    // --- cBEState
    int nextState;                  // 0x0 [O] cBEState::nextState; [P] assert; [R] getState stores its argument
    void** mVtbl;                   // 0x4 [R] cBEPostRaceState_vtbl (getState, cBEPostRaceState_cBEPostRaceState)

    // Methods bound to their retail symbols by asm labels.
    void* getInterface(int kind) __asm__("cBEPostRaceState_getInterface");
};

CHECK_OFFSET(cBEPostRaceState, nextState, 0x0);
CHECK_OFFSET(cBEPostRaceState, mVtbl, 0x4);
CHECK_SIZE(cBEPostRaceState, 0x8);

// cBEState: the base of the three race states (views group D), for code that holds "some state" (cBE's aBEState[],
// [P] the preview's asserts). [O] On Tour class cBEState {int nextState; vptr} (0x8) declaring getInterface,
// purgeState, onExit, onEnter; [R] every derived state stores its vtable at 0x4 and the four slots below.
// Calls through mVtbl: the slot's {short delta; short index; fn} entry (include/vtable.h sVEntry<F>).
struct cBEState {
    int nextState;                  // 0x0 [O]; [P] assert "nextState < kBESTATE_NULL"
    void** mVtbl;                   // 0x4 [R] the derived state's vtable
};

CHECK_OFFSET(cBEState, nextState, 0x0);
CHECK_OFFSET(cBEState, mVtbl, 0x4);
CHECK_SIZE(cBEState, 0x8);

// cBEState's vtable slots ([O] On Tour's virtual order; [R] the three retail vtables above).
enum {
    kBEStateVt_getInterface = 1,    // cBEInterface* getInterface(eBE_Interface)
    kBEStateVt_purgeState = 2,      // operator delete of the singleton
    kBEStateVt_onExit = 3,
    kBEStateVt_onEnter = 4
};

#endif /* BE_BESTATE_H */
