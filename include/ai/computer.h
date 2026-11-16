#ifndef AI_COMPUTER_H
#define AI_COMPUTER_H

// cComputer: a computer-controlled rider: the race's AI riders (cAI::Computers[5], cAI_initComputerRiders) and the
// podium/lodge actors (cAI::Actors[5], cAI_initComputerActors, constructed with isActor = 1). Both are
// `cComputer_cComputer(cMemMan_alloc(0x1A90, "ComputerRider" / "ComputerActorRider", 0x80000000, 0), 1, isActor)`.
// EA's source is d:/ssx/code/source/ai/computer.cpp (preview assert path); the header name is the unit's.
//
// Size 0x1A90: [R] the allocations above; = 0xF50 + sizeof(cRider) 0xB40.
// Bases (On Tour: class cComputer : cRiderHandler, cComputerState, with cRider a *virtual* base):
//   0x000-0xDF0  cRiderHandler, exactly as in cPlayer (ai/player.h): 31 rider parts deriving virtually from cRider
//                (raw bytes here; ai/computer_classes.h models them), then MotionState, ControlState and the vptr.
//   0xDF0-0xF40  cComputerState (On Tour: size 0xD0, no vptr). Retail's is 0x150: its bools are words and the three
//                history tables have 15 entries (On Tour 7). Placed as a block: [R] cComputer::Init (func_00108E88)
//                stores mGrabID = -1 at 0xE0C, mStartJumpPos/mEndJumpPos (t4Vector, sq) at 0xE50/0xE60, mTargetID = -1
//                at 0xE70, clears the three 0x3C tables at 0xE7C/0xEB8/0xEF4 and mCruiseLogicID (sh) at 0xF38 - every
//                one at On Tour's offset re-computed with 4-byte bools and 15-entry tables from 0xDF0.
//   0xF40-0xF50  cComputer's own fields (Input, cruiseLogic).
//   0xF50-0x1A90 cRider, the virtual base (ai/rider.h); its vtables there are _vt$9cComputer$6cRider (D_00458660,
//                deltas -0xF50/-0x1610) and _vt$9cComputer$13cScriptObject (D_00458618).
//
// Member names are EA's, from SSX On Tour's PSP DWARF (cComputer, cComputerState, cRiderHandler), placed by:
//   [P] the July 2003 preview's inline accessors (getCurrentTarget lw 0xE20, setInput sw 0xEF0, setCruiseLogic
//       sh 0xEE8 + PMF 0xEF4, isAboutToRail lw ...), +0x50 in retail for cComputer's block;
//   [R] retail's constructor/Init and the out-of-line accessors in ai/control/handplantcontrol: func_00144000
//       setInput (sw 0xF40), func_00144008 getCurrentTarget (lw 0xE70), func_00144010 isAboutToRail (lw 0xE20),
//       func_00144040 setCruiseLogic (PMF D_0043CF20[i] -> 0xF44, i -> 0xF38);
//   [U] how retail code uses the field (setIndividualRiderDifficulty, updateRiderDifficulty, the four
//       shouldRider* tests, chooseRandom*Grab, scorePath, checkForBlock ...);
//   [O] On Tour's member order alone, where the fields on both sides are placed by [P], [R] or [U].
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "bx/math3d.h"
#include "ai/rider.h"

struct cInput;

// g++ 2.95 pointer to member function (8 bytes): {delta, index, pfn | delta2}. The static cComputer::mCruiseLogic[4]
// ([O]; retail D_0043CF20) holds four non-virtual ones (index -1): func_001009E0, func_00100680, func_00100F88
// ([O] avoidTargetCruise) and func_00100B90, each of which also switches the logic (writes cruiseLogic/mCruiseLogicID).
struct tComputerCruiseLogic {
    short delta;      // 0x0 [U] func_0010A960 (the call through it)
    short index;      // 0x2 [U] func_0010A960 tests it (-1 in every D_0043CF20 entry: non-virtual)
    union {
        void* pfn;    // 0x4 [U] func_0010A960 (non-virtual)
        short delta2; // 0x4 [U] func_0010A960 (virtual: offset of the vptr)
    } u;
};

CHECK_OFFSET(tComputerCruiseLogic, delta, 0x0);
CHECK_OFFSET(tComputerCruiseLogic, index, 0x2);
CHECK_OFFSET(tComputerCruiseLogic, u, 0x4);
CHECK_SIZE(tComputerCruiseLogic, 0x8);

struct cComputer {
    // ---- base cRiderHandler (0x000-0xDF0), as in cPlayer ----
    char unk_000[0x18];        // 0x000 the first part's own data
    cRider* mRider;            // 0x018 [R] the first part's virtual-base pointer: cComputer_cComputer stores
                               //       &this->Rider here (and in 30 other part pointers); [U] every user reads the
                               //       rider through it (cAI_initComputerRiders, func_00100680 ...)
    char unk_01C[0xDC4];       // 0x01C the other parts of cRiderHandler (virtual-base pointers at the same offsets
                               //       as in cPlayer: 0x024, 0x070, 0x0A0, 0x100 ... 0xC70, 0xD20)
    int MotionState;           // 0xDE0 [P] cRiderHandler::_getMotionState; [R] cRiderHandler__getMotionState; [O]
    int ControlState;          // 0xDE4 [P] cRiderHandler::_getControlState; [R] cRiderHandler__getControlState; [O]
    void** mVtbl;              // 0xDE8 [R] _vt$9cComputer (D_004585F0: func_0010A768, func_0010A898)
    char pad_DEC[0x4];         // 0xDEC cRiderHandler is 16-byte aligned
    // ---- base cComputerState (0xDF0-0xF40) ----
    float mDesiredSpeed;       // 0xDF0 [O]; [R] Init = 0; [U] float (func_0010AA70, func_0010B790)
    float mAccuracy;           // 0xDF4 [O]; [R] Init = 1.0f
    float mRiderDifficulty;    // 0xDF8 [O]; [U] set by setIndividualRiderDifficulty / updateRiderDifficulty
    float mTrickDifficulty;    // 0xDFC [O]; [U] set by the same; read by the four shouldRider* tests
                               //       (func_0010C1D0, func_0010C258, func_0010C320, func_0010C3B8)
    short mPathDifficulty;     // 0xE00 [O] short; [U] sh in updateRiderDifficulty, lh in calculateDifficultyScore
    char pad_E02[0x2];
    int mShouldTakeShortcuts;  // 0xE04 [O] (bool); [U] set by updateRiderDifficulty, read by scorePath
    int mShouldBeRandom;       // 0xE08 [O] (bool); [U] set by updateRiderDifficulty
    int mGrabID;               // 0xE0C [O]; [R] Init = -1; [U] a grab 0..14 (AIrand() % 15) or -1
    int mIsSpinTrick;          // 0xE10 [O] (bool); [R] Init = 0
    int mIsGonnaMessUp;        // 0xE14 [O] (bool); [R] Init = 0
    int mIsNaturalAir;         // 0xE18 [O] (bool); [R] Init = 0
    int mToPipe;               // 0xE1C [O] (bool); [R] Init = 0
    int mToRail;               // 0xE20 [O] (bool); [R] Init = 0; isAboutToRail (func_00144010) tests it
    int mIsUber;               // 0xE24 [O] (bool); [U] set by the chooseRandom*Grab functions
    int mIsTweak;              // 0xE28 [O] (bool); [U] same
    float mHoldToLandTime;     // 0xE2C [O]; [U] swc1 in chooseRandom*Grab
    float mGrabStartTime;      // 0xE30 [O]; [U] same
    float mGrabEndTime;        // 0xE34 [O]; [U] same
    float mSpinAxis;           // 0xE38 [O]; [U] func_0010B590, func_0010BFA8
    float mFlipAxis;           // 0xE3C [O]; [U] same
    int mBoardPressTimer;      // 0xE40 [O]; [R] Init = 0
    char pad_E44[0xC];         // 0xE44 t4Vector alignment
    t4Vector mStartJumpPos;    // 0xE50 [O]; [R] Init (sq); [U] checkForJumpEvent (func_0010B980) stores it
    t4Vector mEndJumpPos;      // 0xE60 [O]; [R] Init (sq); [U] same
    int mTargetID;             // 0xE70 [P] getCurrentTarget (preview 0xE20); [R] Init = -1, func_00144008
    int mWanderTime;           // 0xE74 [O]; [R] Init = 0; [U] func_0010D1A0
    int mBadHeadingTime;       // 0xE78 [O]; [R] Init = 0; [U] func_0010D1A0
    int GrabHistoryCounter[15]; // 0xE7C [O] (On Tour [7]); [R] Init/resetCounters clear 0x3C bytes; [U] func_0010CBE0
    int UberHistoryCounter[15]; // 0xEB8 [O]; [U] func_0010CF68 (chooseRandomUberGrab: sets mIsUber)
    int TweakHistoryCounter[15]; // 0xEF4 [O]; [U] func_0010CD20 (chooseRandomTweakGrab)
    float mBlockTimer;         // 0xF30 [O]; [R] Init = 0; [U] func_0010DA10 (float)
    int mShouldIBlock;         // 0xF34 [O] (bool); [R] Init = 0; [U] func_0010DA10
    short mCruiseLogicID;      // 0xF38 [P] setCruiseLogic (preview sh 0xEE8); [R] Init = 1, func_00144040
    char pad_F3A[0x6];         // 0xF3A cComputerState is 16-byte aligned (t4Vector members)
    // ---- cComputer ----
    cInput* Input;             // 0xF40 [P] setInput (preview 0xEF0); [R] func_00144000; [O]
    tComputerCruiseLogic cruiseLogic; // 0xF44 [P] setCruiseLogic (preview 0xEF4); [R] Init = mCruiseLogic[1],
                               //       func_00144040; [U] func_0010A960 calls through it; [O] "void ::* cruiseLogic"
    char pad_F4C[0x4];         // 0xF4C the cRider virtual base is 16-byte aligned
    // ---- virtual base cRider (0xF50-0x1A90) ----
    cRider Rider;              // 0xF50 [R] cComputer_cComputer: cRider_cRider(this + 0xF50) when in charge
};

// Virtual slots of _vt$9cComputer (D_004585F0). cRiderHandler's slot 1 is pure virtual (D_00458710: func_00411530);
// cPlayer and cComputer each provide one.
enum {
    kComputerVt_getControlCmd = 1,  // func_0010A768 ([O] getControlCmd; writes mGrabEndTime)
    kComputerVt_slot2 = 2           // func_0010A898
};

CHECK_OFFSET(cComputer, unk_000, 0x000);
CHECK_OFFSET(cComputer, mRider, 0x018);
CHECK_OFFSET(cComputer, unk_01C, 0x01C);
CHECK_OFFSET(cComputer, MotionState, 0xDE0);
CHECK_OFFSET(cComputer, ControlState, 0xDE4);
CHECK_OFFSET(cComputer, mVtbl, 0xDE8);
CHECK_OFFSET(cComputer, pad_DEC, 0xDEC);
CHECK_OFFSET(cComputer, mDesiredSpeed, 0xDF0);
CHECK_OFFSET(cComputer, mAccuracy, 0xDF4);
CHECK_OFFSET(cComputer, mRiderDifficulty, 0xDF8);
CHECK_OFFSET(cComputer, mTrickDifficulty, 0xDFC);
CHECK_OFFSET(cComputer, mPathDifficulty, 0xE00);
CHECK_OFFSET(cComputer, pad_E02, 0xE02);
CHECK_OFFSET(cComputer, mShouldTakeShortcuts, 0xE04);
CHECK_OFFSET(cComputer, mShouldBeRandom, 0xE08);
CHECK_OFFSET(cComputer, mGrabID, 0xE0C);
CHECK_OFFSET(cComputer, mIsSpinTrick, 0xE10);
CHECK_OFFSET(cComputer, mIsGonnaMessUp, 0xE14);
CHECK_OFFSET(cComputer, mIsNaturalAir, 0xE18);
CHECK_OFFSET(cComputer, mToPipe, 0xE1C);
CHECK_OFFSET(cComputer, mToRail, 0xE20);
CHECK_OFFSET(cComputer, mIsUber, 0xE24);
CHECK_OFFSET(cComputer, mIsTweak, 0xE28);
CHECK_OFFSET(cComputer, mHoldToLandTime, 0xE2C);
CHECK_OFFSET(cComputer, mGrabStartTime, 0xE30);
CHECK_OFFSET(cComputer, mGrabEndTime, 0xE34);
CHECK_OFFSET(cComputer, mSpinAxis, 0xE38);
CHECK_OFFSET(cComputer, mFlipAxis, 0xE3C);
CHECK_OFFSET(cComputer, mBoardPressTimer, 0xE40);
CHECK_OFFSET(cComputer, pad_E44, 0xE44);
CHECK_OFFSET(cComputer, mStartJumpPos, 0xE50);
CHECK_OFFSET(cComputer, mEndJumpPos, 0xE60);
CHECK_OFFSET(cComputer, mTargetID, 0xE70);
CHECK_OFFSET(cComputer, mWanderTime, 0xE74);
CHECK_OFFSET(cComputer, mBadHeadingTime, 0xE78);
CHECK_OFFSET(cComputer, GrabHistoryCounter, 0xE7C);
CHECK_OFFSET(cComputer, UberHistoryCounter, 0xEB8);
CHECK_OFFSET(cComputer, TweakHistoryCounter, 0xEF4);
CHECK_OFFSET(cComputer, mBlockTimer, 0xF30);
CHECK_OFFSET(cComputer, mShouldIBlock, 0xF34);
CHECK_OFFSET(cComputer, mCruiseLogicID, 0xF38);
CHECK_OFFSET(cComputer, pad_F3A, 0xF3A);
CHECK_OFFSET(cComputer, Input, 0xF40);
CHECK_OFFSET(cComputer, cruiseLogic, 0xF44);
CHECK_OFFSET(cComputer, pad_F4C, 0xF4C);
CHECK_OFFSET(cComputer, Rider, 0xF50);
CHECK_SIZE(cComputer, 0x1A90);

#endif /* AI_COMPUTER_H */
