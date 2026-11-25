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
//                history tables have 15 entries (On Tour 7). Placed as a block: [R] cComputer::Init (cComputer_Init)
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
//   [R] retail's constructor/Init and the out-of-line accessors in ai/control/handplantcontrol: cComputer_setInput
//       setInput (sw 0xF40), cComputer_getCurrentTarget getCurrentTarget (lw 0xE70), cComputer_isAboutToRail isAboutToRail (lw 0xE20),
//       cComputer_setCruiseLogic setCruiseLogic (PMF cComputer_mCruiseLogic[i] -> 0xF44, i -> 0xF38);
//   [U] how retail code uses the field (setIndividualRiderDifficulty, updateRiderDifficulty, the four
//       shouldRider* tests, chooseRandom*Grab, scorePath, checkForBlock ...);
//   [O] On Tour's member order alone, where the fields on both sides are placed by [P], [R] or [U].
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "bx/math3d.h"
#include "ai/rider.h"
#include "ai/riderhandler.h"

struct cInput;

// g++ 2.95 pointer to member function (8 bytes): {delta, index, pfn | delta2}. The static cComputer::mCruiseLogic[4]
// ([O]; retail cComputer_mCruiseLogic) holds four non-virtual ones (index -1): cComputer_jumpSetupCruise, cComputer_followPathCruise, cComputer_avoidTargetCruise
// ([O] avoidTargetCruise) and cComputer_attackTargetCruise, each of which also switches the logic (writes cruiseLogic/mCruiseLogicID).
struct tComputerCruiseLogic {
    short delta;      // 0x0 [U] cComputer_getCruiseCmd (the call through it)
    short index;      // 0x2 [U] cComputer_getCruiseCmd tests it (-1 in every cComputer_mCruiseLogic entry: non-virtual)
    union {
        void* pfn;    // 0x4 [U] cComputer_getCruiseCmd (non-virtual)
        short delta2; // 0x4 [U] cComputer_getCruiseCmd (virtual: offset of the vptr)
    } u;
};

CHECK_OFFSET(tComputerCruiseLogic, delta, 0x0);
CHECK_OFFSET(tComputerCruiseLogic, index, 0x2);
CHECK_OFFSET(tComputerCruiseLogic, u, 0x4);
CHECK_SIZE(tComputerCruiseLogic, 0x8);

struct cAIPath;

struct cBxReplayFrame;
struct t4Vector;
struct tBoardPressCmd;
struct tCruiseCmd;
struct tGateCmd;
struct tNaturalAirCmd;
struct tPrewindCmd;
struct tRailslideCmd;
struct tSpinCmd;
struct tWipeoutCmd;
union tControlCmd;

struct cComputer {
    // ---- base cRiderHandler (0x000-0xDF0), as in cPlayer ----
    cRiderHandler RiderHandler;    // 0x000 cRiderHandler base (ai/riderhandler.h): [O] class cComputer : cRiderHandler;
                                   //       [R] cComputer_cComputer stores &this->Rider in all 31 parts' virtual-base
                                   //       pointers (RiderHandler.<part>.mRider: 0x018, 0x024, 0x070 ... 0xC70, 0xD20, the
                                   //       offsets of ai/riderhandler.h) and then runs cRiderHandler_cRiderHandler(this, 0);
                                   //       every user reads the rider through the first one, RiderHandler.GroundMotion.mRider
                                   //       (cAI_initComputerRiders, cComputer_followPathCruise ...); RiderHandler.mVtbl is
                                   //       _vt$9cComputer (D_004585F0: cComputer_getControlCmd, cComputer_writeToReplayFrame)
    // ---- base cComputerState (0xDF0-0xF40) ----
    float mDesiredSpeed;       // 0xDF0 [O]; [R] Init = 0; [U] float (cComputer_getPrewindCmd, cComputer_getGateCmd)
    float mAccuracy;           // 0xDF4 [O]; [R] Init = 1.0f
    float mRiderDifficulty;    // 0xDF8 [O]; [U] set by setIndividualRiderDifficulty / updateRiderDifficulty
    float mTrickDifficulty;    // 0xDFC [O]; [U] set by the same; read by the four shouldRider* tests
                               //       (cComputer_shouldRiderWipeout, cComputer_shouldRiderUber, cComputer_shouldRiderTweak, cComputer_shouldRiderGrab)
    // mPathDifficulty's levels (names ours): [U] calculateDifficultyScore accepts a path for 0, 1 and 2 only when its
    // IsEasy, IsMedium and IsHard flag (On Tour's parameter names) is set.
    enum ePathDifficulty { kPD_Easy = 0, kPD_Medium = 1, kPD_Hard = 2 };
    short mPathDifficulty;     // 0xE00 [O] short; [U] sh in updateRiderDifficulty, lh in calculateDifficultyScore
    char pad_E02[0x2];
    int mShouldTakeShortcuts;  // 0xE04 [O] (bool); [U] set by updateRiderDifficulty, read by scorePath
    int mShouldBeRandom;       // 0xE08 [O] (bool); [U] set by updateRiderDifficulty
    // mGrabID's "no grab" (name ours): [R] Init stores -1; [U] cComputer_getSpinCmd picks a grab only while it is -1
    // and drops one by storing -1; cComputer_getNaturalAirCmd stores it.
    enum { kNoGrab = -1 };
    int mGrabID;               // 0xE0C [O]; [R] Init = kNoGrab; [U] a grab 0..14 (AIrand() % 15) or kNoGrab
    int mIsSpinTrick;          // 0xE10 [O] (bool); [R] Init = 0
    int mIsGonnaMessUp;        // 0xE14 [O] (bool); [R] Init = 0
    int mIsNaturalAir;         // 0xE18 [O] (bool); [R] Init = 0
    int mToPipe;               // 0xE1C [O] (bool); [R] Init = 0
    int mToRail;               // 0xE20 [O] (bool); [R] Init = 0; isAboutToRail (cComputer_isAboutToRail) tests it
    int mIsUber;               // 0xE24 [O] (bool); [U] set by the chooseRandom*Grab functions
    int mIsTweak;              // 0xE28 [O] (bool); [U] same
    float mHoldToLandTime;     // 0xE2C [O]; [U] swc1 in chooseRandom*Grab
    float mGrabStartTime;      // 0xE30 [O]; [U] same
    float mGrabEndTime;        // 0xE34 [O]; [U] same
    float mSpinAxis;           // 0xE38 [O]; [U] cComputer_getNaturalAirCmd, cComputer_setupPrewindState
    float mFlipAxis;           // 0xE3C [O]; [U] same
    int mBoardPressTimer;      // 0xE40 [O]; [R] Init = 0
    char pad_E44[0xC];         // 0xE44 t4Vector alignment
    t4Vector mStartJumpPos;    // 0xE50 [O]; [R] Init (sq); [U] checkForJumpEvent (cComputer_checkForJumpEvent) stores it
    t4Vector mEndJumpPos;      // 0xE60 [O]; [R] Init (sq); [U] same
    // mTargetID's "no target" (name ours): [R] Init stores -1; [U] cComputer_checkForTargets drops a target by
    // storing -1 and treats any negative id as none.
    enum { kNoTarget = -1 };
    int mTargetID;             // 0xE70 [P] getCurrentTarget (preview 0xE20); [R] Init = kNoTarget,
                               //       cComputer_getCurrentTarget
    int mWanderTime;           // 0xE74 [O]; [R] Init = 0; [U] cComputer_checkForReset
    int mBadHeadingTime;       // 0xE78 [O]; [R] Init = 0; [U] cComputer_checkForReset
    int GrabHistoryCounter[15]; // 0xE7C [O] (On Tour [7]); [R] Init/resetCounters clear 0x3C bytes; [U] cComputer_chooseRandomGrab
    int UberHistoryCounter[15]; // 0xEB8 [O]; [U] cComputer_chooseRandomUberGrab (chooseRandomUberGrab: sets mIsUber)
    int TweakHistoryCounter[15]; // 0xEF4 [O]; [U] cComputer_chooseRandomTweakGrab
    float mBlockTimer;         // 0xF30 [O]; [R] Init = 0; [U] cComputer_checkForBlock (float)
    int mShouldIBlock;         // 0xF34 [O] (bool); [R] Init = 0; [U] cComputer_checkForBlock
    short mCruiseLogicID;      // 0xF38 [P] setCruiseLogic (preview sh 0xEE8); [R] Init = 1, cComputer_setCruiseLogic
    char pad_F3A[0x6];         // 0xF3A cComputerState is 16-byte aligned (t4Vector members)
    // ---- cComputer ----
    cInput* Input;             // 0xF40 [P] setInput (preview 0xEF0); [R] cComputer_setInput; [O]
    tComputerCruiseLogic cruiseLogic; // 0xF44 [P] setCruiseLogic (preview 0xEF4); [R] Init = mCruiseLogic[1],
                               //       cComputer_setCruiseLogic; [U] cComputer_getCruiseCmd calls through it; [O] "void ::* cruiseLogic"
    char pad_F4C[0x4];         // 0xF4C the cRider virtual base is 16-byte aligned
    // ---- virtual base cRider (0xF50-0x1A90) ----
    cRider Rider;              // 0xF50 [R] cComputer_cComputer: cRider_cRider(this + 0xF50) when in charge

    // Methods bound to their retail symbols by asm labels.
    void setIndividualRiderDifficulty(int level, int mode) __asm__("cComputer_setIndividualRiderDifficulty");
    void setAttributeLevels() __asm__("cComputer_setAttributeLevels");
    void initRiderDifficulty() __asm__("cComputer_initRiderDifficulty");
    int chooseGrabMain(float airTime) __asm__("cComputer_chooseGrabMain");
    int chooseRandomGrab(float time) __asm__("cComputer_chooseRandomGrab");
    int chooseRandomTweakGrab(float time) __asm__("cComputer_chooseRandomTweakGrab");
    int chooseRandomUberGrab(float time) __asm__("cComputer_chooseRandomUberGrab");
    void resetCounters() __asm__("cComputer_resetCounters");
    int checkForReset() __asm__("cComputer_checkForReset");
    float scorePath(cAIPath* tgt) __asm__("cComputer_scorePath");
    float calculateDifficultyScore(int IsEasy, int IsMedium, int IsHard) __asm__("cComputer_calculateDifficultyScore");
    int myOpponentIndex() __asm__("cComputer_myOpponentIndex");
    bool isFarBehind() __asm__("cComputer_isFarBehind");
    bool isFarAhead() __asm__("cComputer_isFarAhead");
    int checkForAttack(int oppID) __asm__("cComputer_checkForAttack");
    int checkForBlock() __asm__("cComputer_checkForBlock");
    bool checkForPunching(int* side) __asm__("cComputer_checkForPunching");
    int getRival() __asm__("cComputer_getRival");
    float setupWarp() __asm__("cComputer_setupWarp");
    void setInput(int input) __asm__("cComputer_setInput");
    int getCurrentTarget() __asm__("cComputer_getCurrentTarget");
    int isAboutToRail(float fparg0) __asm__("cComputer_isAboutToRail");
    void setCruiseLogic(int idx) __asm__("cComputer_setCruiseLogic");
    float calcTilt(const t4Vector* direction) __asm__("cComputer_calcTilt");
    void jumpSetupCruise(tCruiseCmd* cmd) __asm__("cComputer_jumpSetupCruise");
    void getControlCmd(tControlCmd* _cmd) __asm__("cComputer_getControlCmd");
    void writeToReplayFrame(cBxReplayFrame* data) __asm__("cComputer_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* data) __asm__("cComputer_readFromReplayFrame");
    void getPrewindCmd(tPrewindCmd* cmd) __asm__("cComputer_getPrewindCmd");
    void getBoardPressCmd(tBoardPressCmd* cmd) __asm__("cComputer_getBoardPressCmd");
    void getRailslideCmd(tRailslideCmd* cmd) __asm__("cComputer_getRailslideCmd");
    void setupEndSpin(float airTime) __asm__("cComputer_setupEndSpin");
    int getSpinCmd(tSpinCmd* cmd) __asm__("cComputer_getSpinCmd");
    void getNaturalAirCmd(tNaturalAirCmd* cmd) __asm__("cComputer_getNaturalAirCmd");
    void getWipeoutCmd(tWipeoutCmd* controlCmd) __asm__("cComputer_getWipeoutCmd");
    void getGateCmd(tGateCmd* cmd) __asm__("cComputer_getGateCmd");
    bool checkForJumpEvent(float* approachSpeed, int* below, int* toPipe, int* toRail, int* isSpin, int* isFlip)
        __asm__("cComputer_checkForJumpEvent");
    bool checkForSpeedZone(float* out, int* ok) __asm__("cComputer_checkForSpeedZone");
    bool checkForBoardPress(int* out) __asm__("cComputer_checkForBoardPress");
    bool checkForRail() __asm__("cComputer_checkForRail");
    bool checkForTargets(int* isAttack) __asm__("cComputer_checkForTargets");
    void setupPrewindState(int isSpin, int isFlip, int toRail, int toPipe) __asm__("cComputer_setupPrewindState");
    void setupCrouch(void* cmd) __asm__("cComputer_setupCrouch");
    void setupRecklessCrouch(void* cmd) __asm__("cComputer_setupRecklessCrouch");
    bool shouldRiderWipeout() __asm__("cComputer_shouldRiderWipeout");
    bool shouldRiderUber() __asm__("cComputer_shouldRiderUber");
    bool shouldRiderTweak() __asm__("cComputer_shouldRiderTweak");
    bool shouldRiderGrab() __asm__("cComputer_shouldRiderGrab");
    int updateRiderPathDifficulty(int RiderID, int Heat) __asm__("cComputer_updateRiderPathDifficulty");
};

// Virtual slots of _vt$9cComputer (D_004585F0). cRiderHandler's slot 1 is pure virtual (cRiderHandler_vtbl: __pure_virtual);
// cPlayer and cComputer each provide one.
enum {
    kComputerVt_getControlCmd = 1,  // cComputer_getControlCmd ([O] getControlCmd; writes mGrabEndTime)
    kComputerVt_slot2 = 2           // cComputer_writeToReplayFrame
};

CHECK_OFFSET(cComputer, RiderHandler, 0x000);
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
