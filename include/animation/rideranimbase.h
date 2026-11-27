#ifndef ANIMATION_RIDERANIMBASE_H
#define ANIMATION_RIDERANIMBASE_H

// cRiderAnimBase: a rider's animation player. It owns 6 animation sequencers (one per AI-anim sequencer slot), the
// current AI-anim id of each, and the state copied into every sequence it starts (flip, frame-rate factor, grafting
// mask, orientation offset). Its methods live in animation/rideranimbase.cpp (EA's cRiderAnimBase::play, reset,
// changeHeadingOffset, changeOrientationOffset, getAIAnimFrame ...).
//
// Derived class: cRiderAnim (`cMemMan_alloc(0x70, "RiderAnim")` in cFERider_compileModel; its ctor cRiderAnim_cRiderAnim calls
// cRiderAnimBase_cRiderAnimBase, then stores its own vtable cRiderAnim_vtbl at 0x58 and its fields at 0x60 and 0x64).
// cRider holds one at mRiderAnim (0x784).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [P] the July 2003 preview's inline accessors (isFlipped lw 0x18, getFrameRateFactor lwc1 0x1C,
// getGraftingMask ld 0x20, getOrientationOffset addiu 0x30; preview and retail agree on every offset of this class;
// tools/layout_xlate.py's "0x18 -> 0x4" pair is a misalignment); [R] retail's constructor
// (cRiderAnimBase_cRiderAnimBase) and cRiderAnimBase_reset; [U] how matched retail code uses the field;
// [O] SSX On Tour's DWARF. On Tour's 0x70-byte cRiderAnimBase has the per-sequencer state in its first 0x60 bytes
// (the same members as its class cRiderAnimState, plus a BlendTimeScale at 0x50 that retail lacks), then Sequencer
// at 0x60 and AnimModel at 0x64; retail has them at 0x50 and 0x54 (On Tour's cRiderAnim is 0x10 bigger too: 0x80).

#include "layout.h"
#include "bx/math3d.h"

struct cAnimSequencer;
struct cAnimModel;
struct cAnimSequence;

// One sequence to expire (On Tour struct tExpireInfo, 0x8: cAnimSequence* seq, cAnimSequencer* seqr).
// cRiderAnimBase_expireAnimation collects up to 32 on the stack (each sequence whose end event, flag 0x3F, is set, with
// its sequencer), then hands each pair to vtable slot 3 (view sSeqRef_00312490). [Staged by views group A.]
struct tExpireInfo {
    cAnimSequence* seq;             // 0x0 [O]; [U] expireAnimation stores the sequence, passes it as slot 3's 3rd argument
    cAnimSequencer* seqr;           // 0x4 [O]; [U] expireAnimation stores &Sequencer[i], passes it as slot 3's 2nd argument
};

CHECK_OFFSET(tExpireInfo, seq, 0x0);
CHECK_OFFSET(tExpireInfo, seqr, 0x4);
CHECK_SIZE(tExpireInfo, 0x8);

// An AI animation id (On Tour enum tAIAnimID; cRiderAnimBase::CurrentAnimID, cRiderAnimBase_play's anim). [O] names.
// Retail's ids are On Tour's for 0-70 and On Tour's + 68 for 213-410 (retail has more grab, tweak and uber anims in
// between: 71-212, not named here). [R] cAIAnimEventMap_AnimPlayAttr (437 records, ids 0-436) agrees record by record:
// each block's category, update and expire rules fit the On Tour name (kAID_GateRock 0 updates with ASYNC_GATEROCK,
// the handplant balances 40 / 45 with ASYNC_HANDPLANT, the grind balances 218 / 226 / 234 / 242 with ASYNC_UBERGRIND,
// the 34 wipeout hits 328-361 are kANCAT_WHit but for the lose-board 360 and no-board 361 ...). [U] the wipeout code
// maps each hit to the tumble or slide of the same pose (cWipeoutControl_playWipeoutHitTransitionAnim) and plays the
// no-board ones once the board is off (playWipeoutAirAnim).
enum tAIAnimID {
    // 0-70: On Tour's ids and values (cruise, board press, handplant, rail spin, bump, land)
    kAID_GateRock = 0, kAID_SitCycle = 1, kAID_SitToCruiseFwd = 2, kAID_FinishStand = 3, kAID_FinishCruiseToStand = 4,
    kAID_CruiseFwd = 5, kAID_CruiseCrouch = 6, kAID_CruiseCrouchSlow = 7, kAID_CruiseFast = 8, kAID_CruiseAirFwd = 9,
    kAID_CruiseAirCrouch = 10, kAID_CruiseBrake = 11, kAID_CruiseBrakeStopTS = 12, kAID_CruiseBrakeStopHS = 13,
    kAID_CruiseIce = 14, kAID_CruiseCrouchIce = 15, kAID_CruiseBobbleHS = 16, kAID_CruiseBobbleTS = 17,
    kAID_RSBalance = 18, kAID_RSFSBalance = 19, kAID_RSBSBalance = 20, kAID_ChangeFakie = 21, kAID_GetGoing = 22,
    kAID_NoseChangeFakie = 23, kAID_NoseInto = 24, kAID_NoseOutof = 25, kAID_NoseLand = 26, kAID_NoseOllie = 27,
    kAID_NoseBalance = 28, kAID_NosePivotL = 29, kAID_NosePivotR = 30, kAID_TailChangeFakie = 31, kAID_TailInto = 32,
    kAID_TailOutof = 33, kAID_TailLand = 34, kAID_TailOllie = 35, kAID_TailBalance = 36, kAID_TailPivotL = 37,
    kAID_TailPivotR = 38, kAID_HPTS_Into = 39, kAID_HPTS_Balance = 40, kAID_HPTS_TransReflect = 41,
    kAID_HPTS_ExitBalanceThrough = 42, kAID_HPTS_ExitBalanceReflect = 43, kAID_HPHS_Into = 44, kAID_HPHS_Balance = 45,
    kAID_HPHS_TransReflect = 46, kAID_HPHS_ExitBalanceThrough = 47, kAID_HPHS_ExitBalanceReflect = 48,
    kAID_RSReg2FS = 49, kAID_RSReg2BS = 50, kAID_RSFS2Reg = 51, kAID_RSFakie2FS = 52, kAID_RSFakie2BS = 53,
    kAID_RSBS2Reg = 54, kAID_BumpFromBehind = 55, kAID_BumpFromFront = 56, kAID_BumpFromTS = 57,
    kAID_BumpHardFromTS = 58, kAID_BumpFromHS = 59, kAID_BumpHardFromHS = 60, kAID_LandNormal = 61,
    kAID_LandMedium = 62, kAID_LandHard = 63, kAID_LandChangeFakie = 64, kAID_LandChangeFakieHard = 65,
    kAID_LandSpin180CCW = 66, kAID_LandSpin180CW = 67, kAID_RSLandNormal = 68, kAID_RSBSLandNormal = 69,
    kAID_RSFSLandNormal = 70,
    // 213-410: On Tour's 145-342 + 68 (rail grinds, prewinds, into air, air cycles and adjusts, out of air,
    // reactions, taunts, block / punch, wipeout hits, tumbles, slides, impacts, recovers and stuck)
    kAID_RSBS2FSGrind1 = 213, kAID_RSFS2FSGrind1 = 214, kAID_RSFakie2FSGrind1 = 215, kAID_RSReg2FSGrind1 = 216,
    kAID_RSFSGrind1Cycle = 217, kAID_RSFSGrind1Bal = 218, kAID_RSFSGrind1Land = 219, kAID_RSFSOutofGrind1 = 220,
    kAID_RSBS2FSGrind2 = 221, kAID_RSFS2FSGrind2 = 222, kAID_RSFakie2FSGrind2 = 223, kAID_RSReg2FSGrind2 = 224,
    kAID_RSFSGrind2Cycle = 225, kAID_RSFSGrind2Bal = 226, kAID_RSFSGrind2Land = 227, kAID_RSFSOutofGrind2 = 228,
    kAID_RSBS2FSGrind3 = 229, kAID_RSFS2FSGrind3 = 230, kAID_RSFakie2FSGrind3 = 231, kAID_RSReg2FSGrind3 = 232,
    kAID_RSFSGrind3Cycle = 233, kAID_RSFSGrind3Bal = 234, kAID_RSFSGrind3Land = 235, kAID_RSFSOutofGrind3 = 236,
    kAID_RSBS2FSGrind4 = 237, kAID_RSFS2FSGrind4 = 238, kAID_RSFakie2FSGrind4 = 239, kAID_RSReg2FSGrind4 = 240,
    kAID_RSFSGrind4Cycle = 241, kAID_RSFSGrind4Bal = 242, kAID_RSFSGrind4Land = 243, kAID_RSFSOutofGrind4 = 244,
    kAID_PrewindJump = 245, kAID_PrewindSpinBS = 246, kAID_PrewindSpinFS = 247, kAID_PrewindFlipBwd = 248,
    kAID_PrewindFlipFwd = 249, kAID_PrewindMistyBS = 250, kAID_PrewindMistyFS = 251, kAID_PrewindRodeoBS = 252,
    kAID_PrewindRodeoFS = 253, kAID_RSPrewindSpinBS = 254, kAID_RSPrewindSpinFS = 255, kAID_RSPrewindFlipBwd = 256,
    kAID_RSPrewindFlipFwd = 257, kAID_RSBSPrewindJump = 258, kAID_RSBSPrewindSpinBS = 259,
    kAID_RSBSPrewindSpinFS = 260, kAID_RSBSPrewindFlipBwd = 261, kAID_RSBSPrewindFlipFwd = 262,
    kAID_RSFSPrewindJump = 263, kAID_RSFSPrewindSpinBS = 264, kAID_RSFSPrewindSpinFS = 265,
    kAID_RSFSPrewindFlipBwd = 266, kAID_RSFSPrewindFlipFwd = 267, kAID_IntoAir = 268, kAID_IntoSpinBS = 269,
    kAID_IntoSpinFS = 270, kAID_IntoFlipBwd = 271, kAID_IntoFlipFwd = 272, kAID_IntoMistyBS = 273,
    kAID_IntoMistyFS = 274, kAID_IntoRodeoBS = 275, kAID_IntoRodeoFS = 276, kAID_RSBSIntoAir = 277,
    kAID_RSBSIntoSpinBS = 278, kAID_RSBSIntoSpinFS = 279, kAID_RSBSIntoFlipBwd = 280, kAID_RSBSIntoFlipFwd = 281,
    kAID_RSFSIntoAir = 282, kAID_RSFSIntoSpinBS = 283, kAID_RSFSIntoSpinFS = 284, kAID_RSFSIntoFlipBwd = 285,
    kAID_RSFSIntoFlipFwd = 286, kAID_AirCycle = 287, kAID_ChangeFakieAir = 288, kAID_SpinBSCycle = 289,
    kAID_SpinFSCycle = 290, kAID_FlipBwdCycle = 291, kAID_FlipFwdCycle = 292, kAID_MistyBSCycle = 293,
    kAID_MistyFSCycle = 294, kAID_RodeoBSCycle = 295, kAID_RodeoFSCycle = 296, kAID_IntoAirAdjL = 297,
    kAID_IntoAirAdjFwdL = 298, kAID_IntoAirAdjFwd = 299, kAID_IntoAirAdjFwdR = 300, kAID_IntoAirAdjR = 301,
    kAID_IntoAirAdjBwdR = 302, kAID_IntoAirAdjBwd = 303, kAID_IntoAirAdjBwdL = 304, kAID_OutOfAir = 305,
    kAID_OutOfSpinBS = 306, kAID_OutOfSpinFS = 307, kAID_OutOfFlipBwd = 308, kAID_OutOfFlipFwd = 309,
    kAID_OutOfMistyBS = 310, kAID_OutOfMistyFS = 311, kAID_OutOfRodeoBS = 312, kAID_OutOfRodeoFS = 313,
    kAID_ReactNegative = 314, kAID_ReactPositive = 315, kAID_HeadCheckTS = 316, kAID_HeadCheckHS = 317,
    kAID_CruiseBuckleBindings = 318, kAID_TauntBwd = 319, kAID_TauntTS = 320, kAID_TauntHS = 321, kAID_Block = 322,
    kAID_PunchTS = 323, kAID_PunchHS = 324, kAID_BlockCycle = 325, kAID_PunchTSCycle = 326, kAID_PunchHSCycle = 327,
    kAID_WH_GrndFromLeft2RollRight = 328, kAID_WH_GrndFromRight2RollLeft = 329, kAID_WH_GrndFromBack2RollFwd = 330,
    kAID_WH_GrndFromBack2FwdFaceDown = 331, kAID_WH_GrndFromFrontLow2CartFwd = 332,
    kAID_WH_GrndFromFront2CartBwd = 333, kAID_WH_GrndFromFront2FwdFaceUp = 334, kAID_WH_GrndFromFront2BwdFaceUp = 335,
    kAID_WH_GrndFromFront2RollBwd = 336, kAID_WH_GrndFromFront2BwdFaceDown = 337, kAID_WH_AirFromLeft2RollRight = 338,
    kAID_WH_AirFromRight2RollLeft = 339, kAID_WH_AirFromBack2RollFwd = 340, kAID_WH_AirFromBack2FwdFaceDown = 341,
    kAID_WH_AirFromFrontLow2CartFwd = 342, kAID_WH_AirFromFront2BwdFaceUp = 343,
    kAID_WH_AirFromFront2BwdFaceDown = 344, kAID_WH_AirFromFront2RollBwd = 345, kAID_WH_AirFromFront2CartBwd = 346,
    kAID_WH_AirFromFront2FwdFaceUp = 347, kAID_WH_LandTiltFwd2FwdFaceDown = 348, kAID_WH_LandTiltBwd2BwdFaceDown = 349,
    kAID_WH_LandOnHead2RollFwd = 350, kAID_WH_LandOnFeet2CartFwd = 351, kAID_WH_LandOnFeet2BwdFaceUp = 352,
    kAID_WH_LandOnFeet2FwdFaceDown = 353, kAID_WH_LandOnFace2FwdFaceDown = 354, kAID_WH_LandOnBack2FwdFaceUp = 355,
    kAID_WH_LandOnLeft2RollLeft = 356, kAID_WH_LandOnRight2RollRight = 357, kAID_WH_NoseFwd2FwdFaceDown = 358,
    kAID_WH_TailBwd2FwdFaceUp = 359, kAID_WH_LB_FromFront2FwdFaceDown = 360, kAID_WH_NB_LandOnFace2FwdFaceDown = 361,
    kAID_WG_RollRight = 362, kAID_WG_RollLeft = 363, kAID_WG_RollFwd = 364, kAID_WG_RollBwd = 365,
    kAID_WG_CartFwd = 366, kAID_WG_CartBwd = 367, kAID_WG_FwdFaceDown = 368, kAID_WG_BwdFaceDown = 369,
    kAID_WG_FwdFaceUp = 370, kAID_WG_BwdFaceUp = 371, kAID_WG_NB_FwdFaceDown = 372, kAID_WA_RollRight = 373,
    kAID_WA_RollLeft = 374, kAID_WA_RollFwd = 375, kAID_WA_RollBwd = 376, kAID_WA_CartFwd = 377, kAID_WA_CartBwd = 378,
    kAID_WA_FwdFaceDown = 379, kAID_WA_BwdFaceDown = 380, kAID_WA_FwdFaceUp = 381, kAID_WA_BwdFaceUp = 382,
    kAID_WA_NB_FwdFaceDown = 383, kAID_WGI_FwdFaceDown = 384, kAID_WGI_BwdFaceDown = 385, kAID_WGI_FwdFaceUp = 386,
    kAID_WGI_BwdFaceUp = 387, kAID_WGI_NB_FwdFaceDown = 388, kAID_WGR_FromRollRight = 389, kAID_WGR_FromRollLeft = 390,
    kAID_WGR_FromRollFwd = 391, kAID_WGR_FromRollBwd = 392, kAID_WGR_FromCartFwd = 393, kAID_WGR_FromCartBwd = 394,
    kAID_WGR_FromFwdFaceDown = 395, kAID_WGR_FromBwdFaceDown = 396, kAID_WGR_FromFwdFaceUp = 397,
    kAID_WGR_FromBwdFaceUp = 398, kAID_WAR_FromRollRight = 399, kAID_WAR_FromRollLeft = 400,
    kAID_WAR_FromRollFwd = 401, kAID_WAR_FromRollBwd = 402, kAID_WAR_FromCartFwd = 403, kAID_WAR_FromCartBwd = 404,
    kAID_WAR_FromFwdFaceDown = 405, kAID_WAR_FromBwdFaceDown = 406, kAID_WAR_FromFwdFaceUp = 407,
    kAID_WAR_FromBwdFaceUp = 408, kAID_WS_NB_FwdFaceDown = 409, kAID_WS_FwdFaceDown = 410,
    // [R] the attribute table ends at 437; cRiderAnimBase_reset stores 438 in every CurrentAnimID (no anim), and
    // the grab-table lookups return it for an empty slot (cBEBAGTInterface_getUberAnim). On Tour: Num, Num + 1.
    kAID_Num = 437, kAID_Invalid = 438
};

// An AI animation's category (tAIAnimPlayAttr::AnimCategory; On Tour enum tAIAnimCategoryID). [O] names. Retail lacks
// On Tour's kANCAT_ChangeFakie (7), kANCAT_GetGoing (9) and kANCAT_Idle (11): retail's change-fakie and get-going
// anims (21, 22) are kANCAT_Cruise, so the values from kANCAT_Cruise on are On Tour's minus 1, 2 or 3. [R]
// cAIAnimEventMap_AnimPlayAttr: the cruise anims 0-15 are 7, the handplants 39-48 8, the grabs 18, tweaks 19, ubers 20,
// the grinds 213-244 21, the wipeout hits 27, tumbles 28, slides 29, recovers 30, kAID_WS_FwdFaceDown 31, the no-board
// hit, slide and stuck 22, 25 and 26 and the lose-board hit 23. Retail's 32 (anims 411-431, sequencers 3-5) has no On
// Tour counterpart and is not named.
enum tAIAnimCategoryID {
    kANCAT_None = 0, kANCAT_AirAdj = 1, kANCAT_AirCycle = 2, kANCAT_Block = 3, kANCAT_BoardPress = 4,
    kANCAT_Bobble = 5, kANCAT_Bump = 6, kANCAT_Cruise = 7, kANCAT_Handplant = 8, kANCAT_IntoAir = 9, kANCAT_Land = 10,
    kANCAT_OutOfAir = 11, kANCAT_Prewind = 12, kANCAT_Punch = 13, kANCAT_RailSpin = 14, kANCAT_Railslide = 15,
    kANCAT_Reaction = 16, kANCAT_Taunt = 17, kANCAT_Grab = 18, kANCAT_Tweak = 19, kANCAT_Uber = 20,
    kANCAT_UberGrind = 21, kANCAT_WHitNoBoard = 22, kANCAT_WHitLoseBoard = 23, kANCAT_WTumbleNoBoard = 24,
    kANCAT_WSlideNoBoard = 25, kANCAT_WStuckNoBoard = 26, kANCAT_WHit = 27, kANCAT_WTumble = 28, kANCAT_WSlide = 29,
    kANCAT_WRecover = 30, kANCAT_WStuck = 31
};

// An AI animation's update rule (tAIAnimPlayAttr::UpdateRule): which cRiderAnim::onUpdateXXX runs it each frame. [O] On
// Tour's enum tAIAnimUpdateRuleID, names and values. [U] cRiderAnim_updateRule's switch calls onUpdateFWD for 0 ..
// onUpdateASYNC_BP_BAL for 15, in this order.
enum tAIAnimUpdateRuleID {
    kAAU_FWD = 0, kAAU_HOLD_LAST = 1, kAAU_CYCLE = 2, kAAU_ASYNC_CRUISE3 = 3, kAAU_ASYNC_CRUISE5 = 4,
    kAAU_ASYNC_RAIL = 5, kAAU_ASYNC_BRAKE = 6, kAAU_ASYNC_PREWIND = 7, kAAU_ASYNC_GATEROCK = 8,
    kAAU_ASYNC_HANDPLANT = 9, kAAU_ASYNC_UBERGRIND = 10, kAAU_ASYNC_AIR_ADJ = 11, kAAU_ASYNC_AIR_CYC = 12,
    kAAU_ASYNC_BP_PIVOT = 13, kAAU_ASYNC_BP_INTO = 14, kAAU_ASYNC_BP_BAL = 15
};

// An AI animation's expire rule (tAIAnimPlayAttr::ExpireRule): which cRiderAnim::onExpireXXX picks the next anim. [O]
// On Tour's enum tAIAnimExpireRuleID, names and values (On Tour adds the skier rules kAAE_SK_*, 11-19). [U]
// cRiderAnim_expireRule's switch calls onExpireNOP for 0 .. onExpireBLENDTO_GEAR_CYC for 10, in this order.
enum tAIAnimExpireRuleID {
    kAAE_NOP = 0, kAAE_INTO_AIR = 1, kAAE_INTO_SPIN_CYC = 2, kAAE_INTO_CRUISE = 3, kAAE_BLENDTO_CRUISE = 4,
    kAAE_BLENDTO_AIR = 5, kAAE_BLENDTO_RAIL = 6, kAAE_INTO_FL_STAND = 7, kAAE_INTO_RSFS = 8, kAAE_INTO_PRESS = 9,
    kAAE_BLENDTO_GEAR_CYC = 10
};

// An AI animation's play attributes (On Tour struct tAIAnimPlayAttr, 0x1C): cAIAnimEventMap::getAnimPlayAttr(tAIAnimID)
// returns one ([O] `tAIAnimPlayAttr& getAnimPlayAttr(tAIAnimID aiID)`). [R] cAIAnimEventMap_getAnimPlayAttr returns
// cAIAnimEventMap_AnimPlayAttr + id * 0x1C, so the seven word-sized members sit at On Tour's offsets. The enum members are words
// (tAIAnimCategoryID / tAIAnimUpdateRuleID / tAIAnimExpireRuleID / tAIAnimSequencerID in On Tour).
struct tAIAnimPlayAttr {
    int AnimCategory;               // 0x00 [O]; [U] cRiderAnimBase reads it (*getAnimPlayAttr(anim))
    int UpdateRule;                 // 0x04 [O]; [U] cRiderAnim_updateRule switches on it (16 update handlers)
    int ExpireRule;                 // 0x08 [O]; [U] cRiderAnim_expireRule switches on it
    int SequencerID;                // 0x0C [O]; [U] cRiderAnimBase reads it ([3]) to pick the sequencer
    float BlendInTime;              // 0x10 [O]
    float EmptySeqrBlendInTime;     // 0x14 [O]
    float BlendOutTime;             // 0x18 [O]
};

CHECK_OFFSET(tAIAnimPlayAttr, AnimCategory, 0x00);
CHECK_OFFSET(tAIAnimPlayAttr, UpdateRule, 0x04);
CHECK_OFFSET(tAIAnimPlayAttr, ExpireRule, 0x08);
CHECK_OFFSET(tAIAnimPlayAttr, SequencerID, 0x0C);
CHECK_OFFSET(tAIAnimPlayAttr, BlendInTime, 0x10);
CHECK_OFFSET(tAIAnimPlayAttr, EmptySeqrBlendInTime, 0x14);
CHECK_OFFSET(tAIAnimPlayAttr, BlendOutTime, 0x18);
CHECK_SIZE(tAIAnimPlayAttr, 0x1C);

struct tPose;

struct cRiderAnimBase {
    int CurrentAnimID[6];           // 0x000 [O] (cRiderAnimState::CurrentAnimID[6]); [R] reset stores 0x1B6 (no anim)
                                    //       in each; [U] cRiderAnimBase_play stores the started id at [slot],
                                    //       cRiderAnimBase_getCurrentAnimCategory / cRiderAnimBase_getCurrentAnimID read [i]
    int IsFlipped;                  // 0x018 [P] isFlipped/setIsFlipped (lw/sw: a 4-byte bool); [O]; [R] reset: 0;
                                    //       [U] cRiderAnimBase_onInput copies it to the new sequence (+0x80), as
                                    //       cRiderAnimBase_changeIsFlipped does for every sequence
    float FrameRateFactor;          // 0x01C [P] get/setFrameRateFactor (lwc1/swc1); [O]; [R] reset: 1.0f;
                                    //       [U] cRiderAnimBase_onInput copies it to the sequence (+0x90)
    unsigned long GraftMask;        // 0x020 [P] get/setGraftingMask (ld/sd, `Ul`), clearGraftingMask stores -1; [O];
                                    //       [R] reset: -1; [U] cRiderAnimBase_onInput copies it to the sequence (+0x88); cRider
                                    //       copies mUpperBodyBoneMask here before play. PORT: 64-bit long on the EE
    char unk_028[0x8];              // 0x028 alignment gap before the 16-byte-aligned tPose (On Tour has it too)
    tPose OrientationOffset;        // 0x030 [P] getOrientationOffset (this + 0x30), setOrientationOffset (sq 0x30,
                                    //       0x40); [O]; [R] reset copies tPose_Identity (the identity pose) into it;
                                    //       [U] cRiderAnimBase_onInput copies it to the sequence (+0x60)
    cAnimSequencer* Sequencer;      // 0x050 [O] Sequencer; [R] the ctor allocates 0x30 bytes tagged "Sequencers"
                                    //       (6 x 8-byte cAnimSequencer, each cAnimSequencer_init'ed); [P] the preview
                                    //       destructor reads 0x50 too; [U] every method indexes it by slot
    cAnimModel* AnimModel;          // 0x054 [O] AnimModel (follows Sequencer in On Tour); [R] the ctor stores 0;
                                    //       [U] cRiderAnimBase_setupAnimation passes it to func_0030F2B0 (animation/animmodel.cpp)
                                    //       with the states gathered from the 6 sequencers
    void** mVtbl;                   // 0x058 [R] the ctor and destructor (cRiderAnimBase__cRiderAnimBase) store cRiderAnimBase_vtbl (slot 1 =
                                    //       cRiderAnimBase__cRiderAnimBase; slot 2 = __pure_virtual, a stub in 5 vtables, likely
                                    //       __pure_virtual); cRiderAnim's ctor overwrites it with
                                    //       cRiderAnim_vtbl; [P] preview vptr at 0x58; [U] slots 2 and 4 called
                                    //       (cRiderAnimBase_updateAnimation, cRiderAnimBase_play, cRiderAnimBase_getAIAnimFrame)
    char unk_05C[0x4];              // 0x05C tail padding (the class is 16-byte aligned by tPose)

    // Methods bound to their retail symbols by asm labels.
    float getFrameRateFactor() __asm__("cRiderAnimBase_getFrameRateFactor");
    long getGraftingMask() __asm__("cRiderAnimBase_getGraftingMask");
    int isFlipped() __asm__("cRiderAnimBase_isFlipped");
    void clearGraftingMask() __asm__("cRiderAnimBase_clearGraftingMask");
    int getAIAnimSequencerName(int anim) __asm__("cRiderAnimBase_getAIAnimSequencerName");
    const char* getAIAnimName(int anim) __asm__("cRiderAnimBase_getAIAnimName");
    void changeHeadingOffset(float angle) __asm__("cRiderAnimBase_changeHeadingOffset");
    void changeOrientationOffset(tPose* xf) __asm__("cRiderAnimBase_changeOrientationOffset");
    void changeIsFlipped(int value) __asm__("cRiderAnimBase_changeIsFlipped");
    void blendOutAll(int i, float blendOut) __asm__("cRiderAnimBase_blendOutAll");
    void expireAnimation() __asm__("cRiderAnimBase_expireAnimation");
    float getAIAnimDuration(int aid) __asm__("cRiderAnimBase_getAIAnimDuration");
    float getAIAnimEventTime(int anim, int frame) __asm__("cRiderAnimBase_getAIAnimEventTime");
    void setFrameRateFactor(float fparg0) __asm__("cRiderAnimBase_setFrameRateFactor");
    void setGraftingMask(long graftMask) __asm__("cRiderAnimBase_setGraftingMask");
    int getSequencer(int slot) __asm__("cRiderAnimBase_getSequencer");
    int getCurrentAnimCategory(int i) __asm__("cRiderAnimBase_getCurrentAnimCategory");
    cAnimSequence* getCurrentAnimSequence(int i) __asm__("cRiderAnimBase_getCurrentAnimSequence");
    int getCurrentAnimID(int i) __asm__("cRiderAnimBase_getCurrentAnimID__FPvi");
    float getCurrentAnimFramePercent(int i) __asm__("cRiderAnimBase_getCurrentAnimFramePercent");
    int isCurrentAnimEndFlag(int i) __asm__("cRiderAnimBase_isCurrentAnimEndFlag");
    void setIsFlipped(int isFlipped) __asm__("cRiderAnimBase_setIsFlipped");
    void setHeadingOffset(float angle) __asm__("cRiderAnimBase_setHeadingOffset");
    void setOrientationOffset(tPose* offset) __asm__("cRiderAnimBase_setOrientationOffset");
    void reset() __asm__("cRiderAnimBase_reset");
    void removeAll(int i) __asm__("cRiderAnimBase_removeAll");
    int onInput(cAnimSequencer* vseq, int anim, int animid, int isDupSeqAllowed, int priority, float blend)
        __asm__("cRiderAnimBase_onInput");
    void updateAnimation(int animMinParts, float timescale) __asm__("cRiderAnimBase_updateAnimation");
    void setupAnimation(int animMinParts) __asm__("cRiderAnimBase_setupAnimation");
    void playFromAnimID(int anim, int animid, float blend) __asm__("cRiderAnimBase_playFromAnimID");
    int playInto(cAnimSequencer* seq, cAnimSequence* node, int anim) __asm__("cRiderAnimBase_playInto");
    void playBlendIn(void* sequencer, cAnimSequence* sequence, int anim) __asm__("cRiderAnimBase_playBlendIn");
    int play(int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
};

CHECK_OFFSET(cRiderAnimBase, CurrentAnimID, 0x00);
CHECK_OFFSET(cRiderAnimBase, IsFlipped, 0x18);
CHECK_OFFSET(cRiderAnimBase, FrameRateFactor, 0x1C);
CHECK_OFFSET(cRiderAnimBase, GraftMask, 0x20);
CHECK_OFFSET(cRiderAnimBase, unk_028, 0x28);
CHECK_OFFSET(cRiderAnimBase, OrientationOffset, 0x30);
CHECK_OFFSET(cRiderAnimBase, Sequencer, 0x50);
CHECK_OFFSET(cRiderAnimBase, AnimModel, 0x54);
CHECK_OFFSET(cRiderAnimBase, mVtbl, 0x58);
CHECK_OFFSET(cRiderAnimBase, unk_05C, 0x5C);
CHECK_SIZE(cRiderAnimBase, 0x60);

#endif /* ANIMATION_RIDERANIMBASE_H */
