#ifndef AI_RIDERMETRIX_H
#define AI_RIDERMETRIX_H

// cRiderMetrix: one rider's trick and score tracking (the current trick's power, air, spin, grabs and bonuses, the
// combo chain, the trick history and the run totals) plus the rider's HUD effects. cRider_addRiderMetrix allocates
// it (cMemMan_alloc(460, "RiderMetrix")), stores it in cRider::mMetrix and links it with cRiderMetrix_linkToRider.
// Methods: ai/ridermetrix (the ev* trick events, earnName / earnNameAndPoints, the points getters) and seg/seg_14B10
// (constructor, destructor). No vtable, no base class.
//
// Size 0x1CC [R] (the allocation; On Tour 0x1F0). A FLAT layout, like main/game.h. Member names are EA's, from SSX On
// Tour's PSP DWARF: its cRiderMetrix derives from cRiderMetrixState (0x000-0x1AB here) and adds Rider onwards.
// Placed by:
//   [R] retail's constructor cRiderMetrix_cRiderMetrix and cRiderMetrix_linkToRider;
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [R] or [U].
// Retail differs from On Tour in two ways: On Tour's three leading bools are words here, and a trick definition
// (tTrickNameDef) is 8 bytes, not 0xC, in TrickHistory and in tRiderStats::BestTrick. Everything else is in On Tour's
// order and sizes.

#include "layout.h"

struct cRider;
struct cHUDFX;

// One step of a hold-bonus table (On Tour struct tHoldBonusDB, 0x8; [O] the globals kBigAirBonusDB, kLongGrabBonusDB,
// kLongPressBonusDB, kLongPlantBonusDB, kLongRailBonusDB are `const tHoldBonusDB[]` in On Tour, and retail has the same
// five tables). cRiderMetrix_updateBonusLevelAndCalcPoints (On Tour updateBonusLevelAndCalcPoints(int& curBonusLevel,
// float curMeasure, const tHoldBonusDB* pBonusDB, eHUDFXID)) reads entry [curBonusLevel] (view sGradeStep_119210).
// [Staged by views group A.]
struct tHoldBonusDB {
    float minTarget;                // 0x0 [O]; [U] a negative value ends the table; reached when the measure is >= it;
                                    //      shown with the HUD effect
    float points;                   // 0x4 [O]; [U] the step's points, times mPointsMultiplier, rounded
};

CHECK_OFFSET(tHoldBonusDB, minTarget, 0x0);
CHECK_OFFSET(tHoldBonusDB, points, 0x4);
CHECK_SIZE(tHoldBonusDB, 0x8);

// A trick's name, packed (On Tour struct tTrickNameDef: 0xC bytes of unsigned bit-fields; retail's is 8 bytes, see
// the header comment). Built by cRiderMetrix_buildTrickName (On Tour buildTrickName(tTrickNameDef& tn, ...)), kept in
// TrickHistory / tRiderStats::BestTrick. Only the fields cRiderMetrix_checkForMonsterTrick compares are placed
// (view sTrickBits_11B1A8): it tests them against tMonsterTrick's req* bytes in On Tour's member order, and on a match
// clears the name and stores the entry's id in monster. Their widths equal On Tour's except orient (On Tour 3 bits).
// The unnamed bits hold the rest of EA's fields (On Tour: rail, railSpin, takeoff before orient; special (6 bits)
// before postSpin; uberGrind before grab; toLate (1 bit) between grab and lateGrab; handplant, press, land after
// lateGrab), not yet placed by a retail use. [Staged by views group A.]
// lstruct group L2 placed the rest: cWScriptMan_getCurrentTrick(cmd) (On Tour cWScriptMan::getCurrentTrick(int being,
// int cmd), whose switch returns field `cmd` of tTrickNameDef in member order, 12 = lateGrab asserting) has the same
// case set in retail (12 missing) and reads cWScriptMan::TrickDef (tTrickNameDef*) through the view sScriptFlags_AF08,
// whose bit positions agree with every field placed above (orient 10-11 ... postSpin 28-31, grab, lateGrab as its
// "pad", monster after land). Retail widths differ from On Tour for takeoff, uberGrind and press (noted per field).
struct tTrickNameDef {
    unsigned int rail : 3;          // w0 bits 0-2 [O]; [U] cWScriptMan_getCurrentTrick case 0 (view sScriptFlags_AF08 f0)
    unsigned int railSpin : 4;      // w0 bits 3-6 [O]; [U] getCurrentTrick case 1 (f1)
    unsigned int takeoff : 3;       // w0 bits 7-9 [O] (4 bits in On Tour); [U] getCurrentTrick case 2 (f2, 3 bits)
    unsigned int orient : 2;        // w0 bits 10-11 [O]; [U] checkForMonsterTrick vs tMonsterTrick::reqOrient
    unsigned int spin : 4;          // w0 bits 12-15 [O]; [U] vs reqSpin
    unsigned int reps : 3;          // w0 bits 16-18 [O]; [U] vs reqReps
    unsigned int flip : 3;          // w0 bits 19-21 [O]; [U] vs reqFlip
    unsigned int special : 6;       // w0 bits 22-27 [O] (6 bits, between flip and postSpin as in On Tour); [U]
                                    //      cRiderMetrix_addToHistory tests it against 1 [views2 group A]
    unsigned int postSpin : 4;      // w0 bits 28-31 [O]; [U] vs reqPostSpin
    unsigned int uberGrind : 3;     // w1 bits 0-2 [O] (4 bits in On Tour); [U] getCurrentTrick case 9 (f9, 3 bits)
    unsigned int grab : 7;          // w1 bits 3-9 [O]; [U] vs reqGrab
    unsigned int toLate : 1;        // w1 bit 10 [O]; [U] getCurrentTrick case 11 (f11)
    unsigned int lateGrab : 7;      // w1 bits 11-17 [O]; [U] vs reqLateGrab
    unsigned int handplant : 2;     // w1 bits 18-19 [O]; [U] getCurrentTrick case 13 (f13)
    unsigned int press : 4;         // w1 bits 20-23 [O] (5 bits in On Tour); [U] getCurrentTrick case 15 (f15, 4 bits)
    unsigned int land : 3;          // w1 bits 24-26 [O] (in a third word there); [U] getCurrentTrick case 14 (f14)
    unsigned int monster : 5;       // w1 bits 27-31 [O] (eTrickNameMonster, 5 bits as in On Tour); [U] checkForMonsterTrick
                                    //      stores the matching entry's id after MEM_fill(tn, 0, 8)
};

CHECK_SIZE(tTrickNameDef, 0x8);

// A named monster trick (On Tour struct tMonsterTrick, 0x10; [O] the global gMonsterComboTable is tMonsterTrick[48]
// in On Tour; retail's has 24 entries, the loop bound of cRiderMetrix_checkForMonsterTrick; view sTrickBonus_11B1A8).
// [Staged by views group A.]
struct tMonsterTrick {
    int id;                         // 0x0 [O] eTrickNameMonster; [U] stored into tTrickNameDef::monster on a match
    int points;                     // 0x4 [O]; [U] checkForMonsterTrick returns it (the bonus)
    unsigned char reqOrient;        // 0x8 [O]; [U] compared with tTrickNameDef::orient
    unsigned char reqSpin;          // 0x9 [O]; [U] vs spin
    unsigned char reqReps;          // 0xA [O]; [U] vs reps
    unsigned char reqFlip;          // 0xB [O]; [U] vs flip
    unsigned char reqPostSpin;      // 0xC [O]; [U] vs postSpin
    unsigned char reqGrab;          // 0xD [O]; [U] vs grab
    unsigned char reqLateGrab;      // 0xE [O]; [U] vs lateGrab
    char unk_0F[0x1];               // 0xF padding
};

CHECK_OFFSET(tMonsterTrick, id, 0x0);
CHECK_OFFSET(tMonsterTrick, points, 0x4);
CHECK_OFFSET(tMonsterTrick, reqOrient, 0x8);
CHECK_OFFSET(tMonsterTrick, reqSpin, 0x9);
CHECK_OFFSET(tMonsterTrick, reqReps, 0xA);
CHECK_OFFSET(tMonsterTrick, reqFlip, 0xB);
CHECK_OFFSET(tMonsterTrick, reqPostSpin, 0xC);
CHECK_OFFSET(tMonsterTrick, reqGrab, 0xD);
CHECK_OFFSET(tMonsterTrick, reqLateGrab, 0xE);
CHECK_SIZE(tMonsterTrick, 0x10);

// A rider's run totals (On Tour tRiderStats, 0xB0 there: BestTrick is 8 bytes here).
struct tRiderStats {
    int SpinDegs;                    // 0x00 [O]; [U] earnNameAndPoints adds the trick's spin
    int FlipDegs;                    // 0x04 [O]
    int NumGrabs;                    // 0x08 [O]
    int NumHandplants;               // 0x0C [O]
    int NumRails;                    // 0x10 [O]
    int NumTricksLanded;             // 0x14 [O]
    int NumUberTricks;               // 0x18 [O]
    int NumSuperUberTricks;          // 0x1C [O]
    int NumUberGrindTricks;          // 0x20 [O]
    int NumResets;                   // 0x24 [O]; [U] evReset counts
    int NumWipeouts;                 // 0x28 [O]; [U] evWipeout counts
    int NumFrags;                    // 0x2C [O]
    int NumDeaths;                   // 0x30 [O]; [U] evWipeout
    int NumMultipliers;              // 0x34 [O]
    int NumComboChainLinks;          // 0x38 [O]
    int NumPerfectLandingBonus;      // 0x3C [O]
    int NumLastSecondLandingBonus;   // 0x40 [O]
    float AirTime;                   // 0x44 [O]
    float GrabTime;                  // 0x48 [O]
    float RailDist;                  // 0x4C [O]
    float BiggestAirTime;            // 0x50 [O]
    float BiggestGrabTime;           // 0x54 [O]
    float BiggestRailDist;           // 0x58 [O]
    float BiggestHandplantTime;      // 0x5C [O]
    float MaxSpeedCPS;               // 0x60 [O]
    float MaxAvgSpeedCPS;            // 0x64 [O]
    float SumSpeedCPS;               // 0x68 [O]
    float SumTick;                   // 0x6C [O]
    int TricksAtLevel[6];            // 0x70 [O]
    int LongestCombo;                // 0x88 [O]
    int BestComboPoints;             // 0x8C [O]
    int BestTrickPoints;             // 0x90 [O]; [U] earnNameAndPoints keeps the best
    unsigned int BestTrick[2];       // 0x94 [O] tTrickNameDef (8 bytes in retail); [U] stored with BestTrickPoints
    int TotalPoints;                 // 0x9C [O]; [U] evAddPoints
    int TotalStylePoints;            // 0xA0 [O]
    int TotalPointsLostToWipeouts;   // 0xA4 [O]; [U] evWipeout
    int TotalPointsLostToResets;     // 0xA8 [O]; [U] evReset
};

CHECK_OFFSET(tRiderStats, TricksAtLevel, 0x70);
CHECK_OFFSET(tRiderStats, BestTrick, 0x94);
CHECK_OFFSET(tRiderStats, TotalPointsLostToResets, 0xA8);
CHECK_SIZE(tRiderStats, 0xAC);

struct cBxReplayFrame;

// Where awarded cash comes from (cRiderMetrix_evAddCash's kind). [O] On Tour's enum eRiderMetrixCashSource (evAddCash's
// source), names and values; [U] evAddCash shows kHFX_EarnTrickPointCash, kHFX_EarnBonusCash, kHFX_EarnComboCash and
// kHFX_EarnCollectible for 0-3 and no popup for kRMCS_Mission.
enum eRiderMetrixCashSource {
    kRMCS_TrickSuccess = 0,
    kRMCS_BonusCash = 1,
    kRMCS_ComboBonus = 2,
    kRMCS_Collectible = 3,
    kRMCS_Mission = 4
};

struct cRiderMetrix {
    // cRiderMetrixState
    int InitTrickIsFakie;            // 0x000 [O] (bool there)
    int InitTrickIsPipe;             // 0x004 [O] (bool there)
    int InitTrickIsLateSpin;         // 0x008 [O] (bool there); [U] evLateSpin
    int InitTrickRailslideMode;      // 0x00C [O]
    int InitTrickPressMode;          // 0x010 [O]
    float CurTrickPower;             // 0x014 [O]; [U] earnName / earnNameAndPoints
    float CurTrickPointMult;         // 0x018 [O]; [U] getCurTrickPointsWithMult
    float CurTrickStylePower;        // 0x01C [O]; [U] getCurTrickStyleSubPoints
    int CurTrickRailslideMode;       // 0x020 [O]
    float CurTrickRailslideDist;     // 0x024 [O]
    int CurTrickPressMode;           // 0x028 [O]
    float CurTrickPressSec;          // 0x02C [O]
    float CurTrickAirSec;            // 0x030 [O]
    float CurTrickSpinAng;           // 0x034 [O]; [U] evLateSpin
    float CurTrickFlipAng;           // 0x038 [O]; [U] evFlip
    float CurGrabPowerPerTick;       // 0x03C [O]
    float CurGrabHoldSec;            // 0x040 [O]
    float CurTrickTotalGrabHoldSec;  // 0x044 [O]
    float CurTrickMaxGrabHoldSec;    // 0x048 [O]
    int CurTrickNumGrabs;            // 0x04C [O]
    int CurTrickNumTweaks;           // 0x050 [O]
    int CurTrickNumUbers;            // 0x054 [O]; [U] evUberGrindEnd
    int CurTrickNumSuperUbers;       // 0x058 [O]
    int CurTrickIsUber;              // 0x05C [O] (bool there)
    int CurTrickGrabList[3];         // 0x060 [O] eTrickNameGrab[3]
    float CurTrickUberGrindHoldSec;  // 0x06C [O]
    int CurTrickUberGrind;           // 0x070 [O] eTrickNameUberGrind; [U] evTakeoff clears it
    int CurTrickNumUberGrinds;       // 0x074 [O]; [U] evUberGrindEnd
    float CurTrickHandplantHoldSec;  // 0x078 [O]; [U] evHandplantStart
    int CurTrickHandplant;           // 0x07C [O] eTrickNameHandplant; [U] evHandplantStart
    int CurTrickNumHandplants;       // 0x080 [O]; [U] evHandplantEnd
    int CurTrickBonusPoints;         // 0x084 [O]
    int CurTrickBigAirBonusLevel;    // 0x088 [O]
    int CurTrickLongGrabBonusLevel;  // 0x08C [O]
    int CurTrickLongPressBonusLevel; // 0x090 [O]
    int CurTrickLongPlantBonusLevel; // 0x094 [O]
    int CurTrickLongRailBonusLevel;  // 0x098 [O]
    int NumCombos;                   // 0x09C [O]; [U] resetComboChain
    int ComboPoints;                 // 0x0A0 [O]; [U] resetComboChain, losePoints
    float ComboResetDelay;           // 0x0A4 [O]; [U] delayComboChainReset, cancelComboChainReset
    unsigned int TrickHistory[10][2]; // 0x0A8 [O] tTrickNameDef[10] (8 bytes each in retail)
    int TrickHistoryPtr;             // 0x0F8 [O]
    tRiderStats Totals;              // 0x0FC [O]
    float CurAvgSpeedCPS;            // 0x1A8 [O]
    // cRiderMetrix
    cRider* Rider;                   // 0x1AC [O]; [R] ctor = 0, linkToRider stores the rider
    cHUDFX* HUDfx;                   // 0x1B0 [O]; [R] ctor = 0; linkToRider allocates 44 (0x9C each, "cHUDFX")
    int NumHUDfx;                    // 0x1B4 [O]; [R] ctor = 0, linkToRider = 44
    unsigned int ActiveFXListCache[2]; // 0x1B8 [O]
    int IsActiveFXListCacheInvalid;  // 0x1C0 [O] (bool there)
    float mPointsMultiplier;         // 0x1C4 [O]; [R] ctor = 1.0; [U] evAddPoints, getCurTrickPoints
    int mPointsTotalInitial;         // 0x1C8 [O]; [R] ctor = 0; [U] setInitialPointsTotal

    // Methods bound to their retail symbols by asm labels.
    void addToComboChain(int points) __asm__("cRiderMetrix_addToComboChain");
    void cancelComboChainReset() __asm__("cRiderMetrix_cancelComboChainReset");
    void delayComboChainReset(float seconds) __asm__("cRiderMetrix_delayComboChainReset");
    void earnComboChain() __asm__("cRiderMetrix_earnComboChain");
    int allocHUDfx(int type) __asm__("cRiderMetrix_allocHUDfx");
    void addGrab(int value) __asm__("cRiderMetrix_addGrab");
    int calcTrickLevel(int points) __asm__("cRiderMetrix_calcTrickLevel");
    float evFinishRace() __asm__("cRiderMetrix_evFinishRace");
    float evFrag() __asm__("cRiderMetrix_evFrag");
    float evCollectMultiplier(float value) __asm__("cRiderMetrix_evCollectMultiplier");
    float evCheckpoint(int timeBonusSec) __asm__("cRiderMetrix_evCheckpoint");
    float evAddPoints(int pts) __asm__("cRiderMetrix_evAddPoints");
    float evGrabHold(int id) __asm__("cRiderMetrix_evGrabHold");
    float evReleaseGrab(int grabID) __asm__("cRiderMetrix_evReleaseGrab");
    float evFlip(float x) __asm__("cRiderMetrix_evFlip");
    void* evRailSpin(int rsm) __asm__("cRiderMetrix_evRailSpin__FPvi");
    float evPressStart(int pressMode) __asm__("cRiderMetrix_evPressStart");
    float evPressEnd() __asm__("cRiderMetrix_evPressEnd");
    float evPressPivot(int pressMode) __asm__("cRiderMetrix_evPressPivot");
    float evHandplantStart(int plant) __asm__("cRiderMetrix_evHandplantStart");
    float evHandplantEnd() __asm__("cRiderMetrix_evHandplantEnd");
    float evLateSpin(int isFakieInit) __asm__("cRiderMetrix_evLateSpin");
    float evLand(int stance, int alternate, int style, int flag) __asm__("cRiderMetrix_evLand");
    float evMissionStart() __asm__("cRiderMetrix_evMissionStart");
    float evMissionSuccess(int amount) __asm__("cRiderMetrix_evMissionSuccess");
    void earnName(int stance, int alternate, int style, int flag) __asm__("cRiderMetrix_earnName");
    float earnNameAndPoints(int stance, int alternate, int style, int flag, int takeoff)
        __asm__("cRiderMetrix_earnNameAndPoints");
    void linkToRider(cRider* rider) __asm__("cRiderMetrix_linkToRider");
    int hasActiveHUDfx(int type) __asm__("cRiderMetrix_hasActiveHUDfx");
    float getCurTrickPower() __asm__("cRiderMetrix_getCurTrickPower__FPv");
    int getCurTrickStyleSubPoints() __asm__("cRiderMetrix_getCurTrickStyleSubPoints");
    int getCurTrickPoints() __asm__("cRiderMetrix_getCurTrickPoints");
    int getCurTrickPointsWithMult() __asm__("cRiderMetrix_getCurTrickPointsWithMult");
    float evReset(int isUserReset) __asm__("cRiderMetrix_evReset");
    float evSplitTime(int value) __asm__("cRiderMetrix_evSplitTime");
    float evSplitPoints(int value) __asm__("cRiderMetrix_evSplitPoints");
    float evUberGrindStart(int grindID, int rsm) __asm__("cRiderMetrix_evUberGrindStart__FPvii");
    float evUberGrindEnd(int rsm) __asm__("cRiderMetrix_evUberGrindEnd");
    float evWipeout(int flag) __asm__("cRiderMetrix_evWipeout");
    float evWipeoutRecover(int isUserRecover) __asm__("cRiderMetrix_evWipeoutRecover");
    float evTakeoff(int isFakieTakeoff, int isPipeTakeoff) __asm__("cRiderMetrix_evTakeoff");
    void resetHUDfx() __asm__("cRiderMetrix_resetHUDfx");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cRiderMetrix_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* obj) __asm__("cRiderMetrix_readFromReplayFrame");
    void setInitialPointsTotal(int pointsTotal) __asm__("cRiderMetrix_setInitialPointsTotal");
    void reset() __asm__("cRiderMetrix_reset");
    void resetTotals() __asm__("cRiderMetrix_resetTotals");
    void resetComboChain() __asm__("cRiderMetrix_resetComboChain");
    void resetCurTrick() __asm__("cRiderMetrix_resetCurTrick");
    void removeHUDfx(int type) __asm__("cRiderMetrix_removeHUDfx");
    void update() __asm__("cRiderMetrix_update");
    void updateStaticHUD() __asm__("cRiderMetrix_updateStaticHUD");
    int updateBonusLevelAndCalcPoints(int* idx, tHoldBonusDB* tbl, int type, float x)
        __asm__("cRiderMetrix_updateBonusLevelAndCalcPoints");
    int losePoints() __asm__("cRiderMetrix_losePoints");
};

CHECK_OFFSET(cRiderMetrix, CurTrickPower, 0x014);
CHECK_OFFSET(cRiderMetrix, CurTrickGrabList, 0x060);
CHECK_OFFSET(cRiderMetrix, ComboResetDelay, 0x0A4);
CHECK_OFFSET(cRiderMetrix, TrickHistory, 0x0A8);
CHECK_OFFSET(cRiderMetrix, Totals, 0x0FC);
CHECK_OFFSET(cRiderMetrix, CurAvgSpeedCPS, 0x1A8);
CHECK_OFFSET(cRiderMetrix, Rider, 0x1AC);
CHECK_OFFSET(cRiderMetrix, mPointsMultiplier, 0x1C4);
CHECK_SIZE(cRiderMetrix, 0x1CC);

#endif
