#ifndef AI_RIDER_H
#define AI_RIDER_H

// cRider: one snowboarder (player, CPU or mission rider). EA's header was ../source/ai/rider.h
// (preview build assert path).
//
// This is a FLAT layout: every field at its retail offset, no C++ inheritance or virtuals yet.
// Matching code uses it through `cRider* self` and `self->field`, and CHECK_OFFSET pins each field.
//
// What the real class looked like (evidence, not yet modelled):
//   - Bases: cBeing (primary; its vptr is at 0x6C0, after cBeing's data) and cScriptObjRider
//     (-> cScriptEntity, cScriptObject), whose vptrs are at 0x6D0 and 0x6E8. Source: the three
//     vtables the destructor (cRider__cRider) installs, _vt$6cRider / $15cScriptObjRider / $13cScriptObject.
//   - cRider is a *virtual* base of cPlayer (at cPlayer+0xE10, see cPlayer_cPlayer) and of about 30
//     rider components (cGroundMotion, cRailslideMotion, cWipeoutMotion, cHandplantMotion, cTrickBoostFX,
//     cAirStreamerFX, ...): their preview vtables are named _vt$<Component>$6cRider.
//
// Field names come from, in order of strength:
//   [R] a retail accessor body (e.g. cRider_getAIIndex = cRider::getAIIndex: lw v0,0x86C(a0));
//   [P] a preview-build accessor, carried to retail by tools/layout_xlate.py ("seen" = the offset pair
//       was observed in aligned preview/retail code; "bracketed" = between two observed pairs with the
//       same shift) and confirmed by how retail code uses the field;
//   [U] how the matched code uses it (comment says how);
//   [X] andersfischernielsen/ssx3-decomp (MIT), docs/investigation/structs/rider.md: a field map of the
//       same class built from Ghidra, live PCSX2 reads and RAM dumps, matched to ours by offset (their
//       Rider is our cRider: every named offset agrees). Only their high/med-confidence names are used.
// Everything else is unk_XXX. Rename only with evidence, and say which kind.

#include "layout.h"
#include "bx/math3d.h"
#include "animation/animmodel.h"

class cRiderAnim;
struct cAirPredictor;
class cRiderMetrix;
class cRiderHandler;

// Virtual slots of cRider's primary vtable (_vt$6cRider, retail cRider_vtbl), as used by
// `vt[k].fn(sub + vt[k].delta)` with sub = &self->mVtbl. Names from the preview vtable.
enum {
    kRiderVt_delete = 1,          // deleting destructor (cRider__cRider)
    kRiderVt_getVelocity = 2,
    kRiderVt_isIntersecting = 3,
    kRiderVt_applyBoost = 4,      // (const t4Vector* impulse): cRider_applyBoost
    kRiderVt_getPosition = 5,
    kRiderVt_getTransform = 6,
    kRiderVt_getAIIndex = 7,
    kRiderVt_isHuman = 8,
    kRiderVt_isComputer = 9,
    kRiderVt_transport = 10,
    kRiderVt_resetToPath = 11,
    kRiderVt_vibrationReset = 16,
    kRiderVt_vibrationBump = 17,
    kRiderVt_vibrationRumble = 18,
    kRiderVt_vibrationUpdate = 19
};

// One of cRider's 19 smoothed channels at 0x1F0 (cRider_OnStartObjectUse initialises {cur, rate, target}).
struct sRiderChannel {
    float cur;
    float rate;
    float target;
};

// One per rider in the race, indexed by the *other* rider's mAIIndex (cRider_checkOpponentCollisions: rider-vs-rider contact and shove
// timing). 6 of them fill cRider 0x000-0x0D8 exactly: a fixed 6-rider table (notes/subsystems/riders-cap.md).
// It is On Tour's struct tOpponentInfo (0x20: IsValid, IsHuman, XYDist, Theta, CollideTick, OverlapTick, PunchTick,
// ShouldTaunt, LastTauntTick; its two leading bools are words in retail, so retail is 0x24 with the same order).
// [Views group A: IsHuman added at 0x4; the other On Tour names are noted per field, not applied.]
struct sRiderPair {
    int mValid;         // [X] tested as "active" (cRider_checkOpponentCollisions); On Tour IsValid
    int IsHuman;        // 0x04 [O] tOpponentInfo::IsHuman (bool, a word in retail); [U] cAI_RefreshGroupAI fills it with
                        //      that rider's isHuman(); cComputer_myOpponentIndex tests it with mValid (view
                        //      sComputer_0010D8F8::b)
    float mDistance;       // [X] distance to that rider, compared < 1000 (cRider_checkForProximityAction); On Tour XYDist
    float mAngle;       // [X] bearing to that rider; minus the heading, wrapped to +-pi (cRider_checkForProximityAction); On Tour Theta
    int CollideTick;         // a tick (cRider_checkOpponentCollisions); On Tour CollideTick
    int OverlapTick;         // a tick (cRider_checkOpponentCollisions); On Tour OverlapTick
    int PunchTick;         // a tick (cRider_checkOpponentCollisions); On Tour PunchTick
    int mAggressive;         // [X] tested nonzero (cRider_checkForProximityAction); On Tour ShouldTaunt
    int unk_20;         // tick of the last glance, compared against now - 600 (cRider_checkForProximityAction); On Tour LastTauntTick
};

CHECK_OFFSET(sRiderPair, mValid, 0x00);
CHECK_OFFSET(sRiderPair, IsHuman, 0x04);
CHECK_OFFSET(sRiderPair, mDistance, 0x08);
CHECK_OFFSET(sRiderPair, mAngle, 0x0C);
CHECK_OFFSET(sRiderPair, CollideTick, 0x10);
CHECK_OFFSET(sRiderPair, OverlapTick, 0x14);
CHECK_OFFSET(sRiderPair, PunchTick, 0x18);
CHECK_OFFSET(sRiderPair, mAggressive, 0x1C);
CHECK_OFFSET(sRiderPair, unk_20, 0x20);
CHECK_SIZE(sRiderPair, 0x24);

struct cTrackPath;
struct cAIPath;
struct tInstanceRecord;
struct tPatchRecord;
struct tSplineRecord;
struct bxSphereTree;
struct cRiderSphereTree;      // intersect/riderspheretree.h
struct tPathIntersectInfo;    // path/basepath.h
struct cOverlapList;          // intersect/overlaplist.h

// One path event in range (On Tour struct tPathEventNode, size 0x10; path/basepath.h only forward-declares it). cRider
// keeps 12 of them at 0x4DC (PathEventsCur). Layout [O]; [U] the path's getEventsInRange fills 16-byte entries
// {int, int, float min, float max} (sBXRange in bx/bxstringctor.cpp), and cRider_updateTrackPath copies count << 4 bytes.
struct tPathEventNode {
    int Type;           // 0x0 [O] tPathEventType
    int Arg;            // 0x4 [O]
    float ParamStart;   // 0x8 [O]; [U] sBXRange.min
    float ParamEnd;     // 0xC [O]; [U] sBXRange.max
};

// cRider::tLegInfo (On Tour, nested in cRider; size 0x30). [P] the preview asserts "RightLeg.ThighSkelIndex >= 0",
// "RightLeg.CalfSkelIndex >= 0", "RightLeg.FootSkelIndex >= 0" (and the LeftLeg ones) name the members.
struct tLegInfo {
    int ThighSkelIndex;     // 0x00 [O] [P]
    int CalfSkelIndex;      // 0x04 [O] [P]
    int FootSkelIndex;      // 0x08 [O] [P]; [U] cRider_RenderShadow reads both legs' (unk_8E8 / unk_918) as bone indices
    tPose BoardRelativePose; // 0x10 [O]; [U] cRider_updateSkeleton combines it with the board bone for cRider_doLegIK
};

// A ray / sphere-tree hit (On Tour struct tIntersectInfo, size 0x80; wscript/wscriptman.h only forward-declares it).
// cRider keeps its last object hit at 0x9E0 (CollideIntersectInfo). Retail confirms Ipos / Inorm / Ivel / Inst / Mesh
// (sContact_5398 pos / normal / vel / surf / idx in cRider_checkObjectCollisions) and u / v (copied to cRider 0xAAC /
// 0xAB0 by cRider_SetNISGroundVectors); the rest is [O] only.
struct tIntersectInfo {
    t4Vector Ipos;              // 0x00 [O] [U]
    t4Vector Inorm;             // 0x10 [O] [U]
    t4Vector Ivel;              // 0x20 [O] [U]
    t4Vector Iangvel;           // 0x30 [O]
    float Iparam;               // 0x40 [O]
    int HitType;                // 0x44 [O] tHitType
    bxSphereTree* SphTree;      // 0x48 [O]
    int Stype;                  // 0x4C [O] eSurfaceType
    tInstanceRecord* Inst;      // 0x50 [O]; [P] assert "CollideIntersectInfo.Inst == 0"; [U] cRider_updateSplit10 (unk_A30)
    tPatchRecord* Patch;        // 0x54 [O]
    tSplineRecord* Spline;      // 0x58 [O]
    int Mesh;                   // 0x5C [O]; [U] the triangle index (sContact_5398.idx, TriFlags_5398)
    int Face;                   // 0x60 [O]
    int Seg;                    // 0x64 [O]
    float T;                    // 0x68 [O]
    float u;                    // 0x6C [O]; [U] cRider_SetNISGroundVectors (hit f6C -> PatchUV.x)
    float v;                    // 0x70 [O]; [U] cRider_SetNISGroundVectors (hit f70 -> PatchUV.y)
};

// A collision response (On Tour struct tCollideInfo, size 0x40). [U] sInfo_5398 {pos, dir, normal, speed}
// (cRider_checkObjectCollisions builds it: contact pos, normalised velocity, contact normal, closing speed).
struct tCollideInfo {
    t4Vector Pos;               // 0x00 [O] [U]
    t4Vector Direct;            // 0x10 [O] [U]
    t4Vector Reflect;           // 0x20 [O] [U]
    float Vel;                  // 0x30 [O] [U]
};

// A ray-cast cache: the patch and triangle the last line / wall test hit (On Tour struct tIntHint, size 0x10, with an
// inline Clear()). cRider owns two (mLineHint, mWallHint). [R] cRider's ctor allocates each with
// cMemMan_alloc(0x10, "Line Intersect Hint" / "Wall Intersect Hint"); [U] cRider_putAt's inline clear (ClearCache_D660
// in ai/rider.cpp, On Tour tIntHint::Clear) zeroes exactly these widths: word 0x0, halves 0x4 / 0x6, words 0x8 / 0xC.
struct tIntHint {
    tPatchRecord* mPatch;       // 0x0 [O]; [U] the clear zeroes it; cRiderHandler_writeToReplayFrame reads the patch's
                                //      id (+0x150) through it, cRiderHandler_readFromReplayFrame stores the patch back
    unsigned short mU;          // 0x4 [O]; [U] the clear (sh 0x4)
    unsigned short mV;          // 0x6 [O]; [U] the clear (sh 0x6)
    int mFirstTri;              // 0x8 [O] bool (a word in retail); [U] the clear (sw 0x8)
    int mPrecision;             // 0xC [O] tIntPrecision; [U] the clear (sw 0xC)
};

CHECK_OFFSET(tIntHint, mPatch, 0x0);
CHECK_OFFSET(tIntHint, mU, 0x4);
CHECK_OFFSET(tIntHint, mV, 0x6);
CHECK_OFFSET(tIntHint, mFirstTri, 0x8);
CHECK_OFFSET(tIntHint, mPrecision, 0xC);
CHECK_SIZE(tIntHint, 0x10);

// One snow surface's physics and effects settings (On Tour struct tSurfData, 0xB0). [Staged by views group A.]
// cGroundMotion_getSlipForce (On Tour getSlipForce(const tSurfData* snow, float v, float gforce)) reads SlipFriction at
// 0x90: four (x, y) points of a piecewise-linear curve (view sHpCfg_13CCF0 / sCurvePt_13CCF0 in
// src/ai/control/handplantcontrol.cpp). Retail 0x90 is On Tour's 0x90 (On Tour's one bool, IsResetSurface, is padded
// to a word there too), so the other members are placed at On Tour's offsets, [O] only until a retail use confirms them.
struct tSurfData {
    float Gravity;                  // 0x00 [O]
    float TerminalVelocity;         // 0x04 [O]
    float Dig;                      // 0x08 [O]
    float MaxLeanAngle;             // 0x0C [O]
    float AirHeight;                // 0x10 [O]
    float PowderDepth1G;            // 0x14 [O]
    float PowderDepth3G;            // 0x18 [O]
    float PowderDamping;            // 0x1C [O]
    float AutoBoostSpeed;           // 0x20 [O]
    float AutoBoostFactor;          // 0x24 [O]
    float SlopeFactorHeadMult;      // 0x28 [O]
    float BackTurnHeadMult;         // 0x2C [O]
    float TurnKickFlare;            // 0x30 [O]
    float TurnHeadAdjustRate;       // 0x34 [O]
    float TurnAdjustRate;           // 0x38 [O]
    float LandingHardness;          // 0x3C [O]
    float MaxLandingPenetrationVel; // 0x40 [O]
    int IsResetSurface;             // 0x44 [O] bool
    float WakeAlpha;                // 0x48 [O]
    float WakeSpeed;                // 0x4C [O]
    int CloudySprayActive;          // 0x50 [O]
    float CloudySprayMaxHeight;     // 0x54 [O]
    int ParticleSnowTrailActive;    // 0x58 [O]
    float SnowBuildupAmount;        // 0x5C [O]
    int ImpactActive;               // 0x60 [O]
    int ImpactLarge;                // 0x64 [O]
    float ImpactMultiplier;         // 0x68 [O]
    float RockSprayChance;          // 0x6C [O]
    float SmallChunkySprayChanceScale; // 0x70 [O]
    float LargeChunkySprayChanceScale; // 0x74 [O]
    int ChunksUseWakeVelocity;      // 0x78 [O]
    float MinChunkWakeVelocityMult; // 0x7C [O]
    float MaxChunkWakeVelocityMult; // 0x80 [O]
    int HasVeryTightTurnWake;       // 0x84 [O]
    int GrindingParticles;          // 0x88 [O]
    int GrindingSparks;             // 0x8C [O]
    float SlipFriction[4][2];       // 0x90 [O] t2Vector SlipFriction[4] (float[2] each, like fe/heartbeat.h, until a
                                    //      shared header has t2Vector); [U] cGroundMotion_getSlipForce: [i][0] = the slip
                                    //      input, [i][1] = the force; below [0] -> [0][1], above [3] -> [3][1], linear
                                    //      in between
};

CHECK_OFFSET(tSurfData, Gravity, 0x00);
CHECK_OFFSET(tSurfData, TerminalVelocity, 0x04);
CHECK_OFFSET(tSurfData, Dig, 0x08);
CHECK_OFFSET(tSurfData, MaxLeanAngle, 0x0C);
CHECK_OFFSET(tSurfData, AirHeight, 0x10);
CHECK_OFFSET(tSurfData, PowderDepth1G, 0x14);
CHECK_OFFSET(tSurfData, PowderDepth3G, 0x18);
CHECK_OFFSET(tSurfData, PowderDamping, 0x1C);
CHECK_OFFSET(tSurfData, AutoBoostSpeed, 0x20);
CHECK_OFFSET(tSurfData, AutoBoostFactor, 0x24);
CHECK_OFFSET(tSurfData, SlopeFactorHeadMult, 0x28);
CHECK_OFFSET(tSurfData, BackTurnHeadMult, 0x2C);
CHECK_OFFSET(tSurfData, TurnKickFlare, 0x30);
CHECK_OFFSET(tSurfData, TurnHeadAdjustRate, 0x34);
CHECK_OFFSET(tSurfData, TurnAdjustRate, 0x38);
CHECK_OFFSET(tSurfData, LandingHardness, 0x3C);
CHECK_OFFSET(tSurfData, MaxLandingPenetrationVel, 0x40);
CHECK_OFFSET(tSurfData, IsResetSurface, 0x44);
CHECK_OFFSET(tSurfData, WakeAlpha, 0x48);
CHECK_OFFSET(tSurfData, WakeSpeed, 0x4C);
CHECK_OFFSET(tSurfData, CloudySprayActive, 0x50);
CHECK_OFFSET(tSurfData, CloudySprayMaxHeight, 0x54);
CHECK_OFFSET(tSurfData, ParticleSnowTrailActive, 0x58);
CHECK_OFFSET(tSurfData, SnowBuildupAmount, 0x5C);
CHECK_OFFSET(tSurfData, ImpactActive, 0x60);
CHECK_OFFSET(tSurfData, ImpactLarge, 0x64);
CHECK_OFFSET(tSurfData, ImpactMultiplier, 0x68);
CHECK_OFFSET(tSurfData, RockSprayChance, 0x6C);
CHECK_OFFSET(tSurfData, SmallChunkySprayChanceScale, 0x70);
CHECK_OFFSET(tSurfData, LargeChunkySprayChanceScale, 0x74);
CHECK_OFFSET(tSurfData, ChunksUseWakeVelocity, 0x78);
CHECK_OFFSET(tSurfData, MinChunkWakeVelocityMult, 0x7C);
CHECK_OFFSET(tSurfData, MaxChunkWakeVelocityMult, 0x80);
CHECK_OFFSET(tSurfData, HasVeryTightTurnWake, 0x84);
CHECK_OFFSET(tSurfData, GrindingParticles, 0x88);
CHECK_OFFSET(tSurfData, GrindingSparks, 0x8C);
CHECK_OFFSET(tSurfData, SlipFriction, 0x90);
CHECK_SIZE(tSurfData, 0xB0);

// One file of the rider's model / texture load list (On Tour struct tRiderLoader, 0x20C: origFileName[256],
// FileName[256], FileOp, DataPtr, processingState). Retail's is 0x108, one name and the two words: [U] func_0011BD60 /
// func_0011BE88 strcpy the name, store -1 and 0 and qsort the entries with a 264-byte stride; cRider_loadRider polls
// +0x100 and reads +0x104. [Staged by views2 group A.]
struct tRiderLoader {
    char FileName[256];             // 0x000 [O] (On Tour's second name; retail keeps one); [U] the file to load
    int FileOp;                     // 0x100 [O]; [U] -1 until cRider_loadRider starts the async load
    void* DataPtr;                  // 0x104 [O]; [U] 0, then the loaded data (cRider_loadRider)
};

CHECK_OFFSET(tRiderLoader, FileName, 0x000);
CHECK_OFFSET(tRiderLoader, FileOp, 0x100);
CHECK_OFFSET(tRiderLoader, DataPtr, 0x104);
CHECK_SIZE(tRiderLoader, 0x108);

// The rider's load list (On Tour struct tRiderLoadList: tRiderLoader LoadList[40]; int MdlLoadNum; int TexLoadNum).
// [R] cRider_loadRider: cMemMan_alloc(0x2948, "LoadList") into cRider::mLoadList, zeroes 0x2940 / 0x2944, adds the
// model files (func_0011BD60, counting MdlLoadNum) and then the texture files after them (func_0011BE88, counting
// TexLoadNum). [Staged by views2 group A.]
struct tRiderLoadList {
    tRiderLoader LoadList[40];      // 0x0000 [O]; [U] models first, then textures
    int MdlLoadNum;                 // 0x2940 [O]; [R] 0; [U] func_0011BD60 counts the model files
    int TexLoadNum;                 // 0x2944 [O]; [R] 0; [U] func_0011BE88 counts the texture files (after the models)
};

CHECK_OFFSET(tRiderLoadList, LoadList, 0x0000);
CHECK_OFFSET(tRiderLoadList, MdlLoadNum, 0x2940);
CHECK_OFFSET(tRiderLoadList, TexLoadNum, 0x2944);
CHECK_SIZE(tRiderLoadList, 0x2948);

struct cRider {
    /* 0x000 */ sRiderPair mRelations[6]; // [X] [U] per other rider, by its mAIIndex (cRider_checkOpponentCollisions)
    /* 0x0D8 */ int unk_0D8; // set to -1, then passed to cRiderMetrix_reset(mMetrix, v) (cRider_initAlways)
    /* 0x0DC */ float mRubberBandBehind; // [X] cleared on reset (cRider_initAlways); rubber-band threshold, c.lt.s in cComputer_isFarBehind, cComputer_setupWarp
    /* 0x0E0 */ float mRubberBandAhead; // [X] cleared on reset (cRider_initAlways); rubber-band threshold, c.lt.s in cComputer_isFarAhead
    /* 0x0E4 */ int mGlueStrengthLevel; // [O] cRiderState::mGlueStrengthLevel (+0x8C there); [U] cComputer_setupWarp: 0 = no warp, 2/3 = catch-up, 1/3 = slow-down, as On Tour's setupWarp tests rider+0x8C
    /* 0x0E8 */ int PrevRanking; // [O] cRiderState::PrevRanking (+0x90); [R] cRider_setPreviousRanking / getPreviousRanking (On Tour setPreviousRanking: sw a1,0x90); cRider_flagEventPathCheckpoint copies RaceRanking (On Tour RaceRanking, +0x94) into it
    /* 0x0EC */ int RaceRanking; // [O] cRiderState::RaceRanking (+0x94, after PrevRanking); [U] copied into PrevRanking; the place 0..5: func_002A34D0 switches it to 1,2,4,8,0x10,0x200; func_002A1E68 compares it 0..4
    /* 0x0F0 */ int PissedOff; // [O] cRiderState::PissedOff (bool at +0x98 there; a word here: cleared with sw on reset, cRider_initAlways); [U] cComputer_checkForAttack tests it before comparing PissedAIIndex
    /* 0x0F4 */ int LastPunchTick; // [O] cRiderState::LastPunchTick (+0x9C there); [U] cleared on reset; the shove check stores the AI tick here and waits 120 ticks
    /* 0x0F8 */ int PissedAIIndex; // [O] cRiderState::PissedAIIndex (+0xA0 there); [U] set to -1 on reset; indexes the AI's riders (the opponent this rider is after)
    /* 0x0FC */ int PissedOffType; // [O] cRiderState::PissedOffType (eBEAggLevel; +0xA4 there, right after PissedAIIndex); [U] cAI_UpdateAggression stores cBEAggressionInterface_getAggression's level here
    /* 0x100 */ int unk_100; // cleared when the rider quits (cRider_doFinishEvent)
    /* 0x104 */ char unk_104[0xC];
    /* 0x110 */ t4Vector mPosition; // [R] getPosition (cRider_getPosition)
    /* 0x120 */ t4Vector mRotation; // [P] getRotation (unshifted); quaternion, normalised by updateOrientationImplicit
    /* 0x130 */ t4Vector mRootPoseOverridePos; // [X] moved by AddEq by the wall-hit displacement (func_00137138)
    /* 0x140 */ char unk_140[0x10];
    /* 0x150 */ int mUseRootPoseOverride; // [X]
    /* 0x154 */ char unk_154[0xC];
    /* 0x160 */ t4Matrix RiderRoot; // [O] cRiderState::RiderRoot (+0x100, between BoardOrientation/UseBoardOrientation and Transform, as here);
                // [U] cRider_updateSkeleton builds it from the pose (QuatToMat at +0x160; On Tour: tPose::matrix(this+0x100));
                // rows side/front/up/position read by cRider_reactToCollision and cRider_doOpponentCollision
                // row[1] (front): dotted with mVelocity (rejected above 277.78) and the launch dir (cRider_isChangeFakieAir);
                // row[2] (up): dotted with the wall / contact normal (cRider_doObjectCollision); its dot with the launch dir
                // must be >= 0 (cRider_isChangeFakieAir)
    /* 0x1A0 */ t4Matrix mMatrix; // [P] rows getSide/getFront/getUp (unshifted) + position; built from mRotation/mPosition
    /* 0x1E0 */ t4Vector mVelocity; // [R] getVelocity (cRider_getVelocity); [P] setVelocity
    /* 0x1F0 */ sRiderChannel mSlewed[19]; // [X] [U] 19 channels, init by cRider_OnStartObjectUse; .cur of [0],[2],[3],[8] read as lean input
    /* 0x2D4 */ unsigned short SurfaceFlags; // [O] SurfaceFlags (placed by its neighbours); [U] = hit object +0xA (cRider_SetNISGroundVectors)
    /* 0x2D6 */ char unk_2D6[0x2];
    /* 0x2D8 */ int mWasFlipped; // = 0x320 on reset (cRider_OnStartObjectUse)
    /* 0x2DC */ float mSpinRate; // [X] cleared on transport (cRider_putAt)
    /* 0x2E0 */ float mFlipRate; // [X] cleared on transport (cRider_putAt)
    /* 0x2E4 */ float TerminalVelocity; // 4th arg of cAirPredictor_startLaunchIntoAir after a bounce (cRider_doObjectCollision)
    /* 0x2E8 */ float mPowerUpBoostTimer; // [X] decays each frame (cRider_updateBoost)
    /* 0x2EC */ float mPowerUpTrickTimer; // [X]
    /* 0x2F0 */ float mUberTimer; // [X]
    /* 0x2F4 */ int mUberLevel; // [X] a mode: 5, 10, >= 11 tested
    /* 0x2F8 */ float BoostPowerBar;
    /* 0x2FC */ float mBoostStrength; // [X] scales the mass (cRider_getMass)
    /* 0x300 */ float mTimeScale; // [X] decay rate for 0x2E8-0x2F8
    /* 0x304 */ int mBoostMode; // [X] 0..3
    /* 0x308 */ int mAllowNaturalAir; // = 1 (ctor)
    /* 0x30C */ float mBoostMaxSpeed; // [X] = 4000.0 (ctor)
    /* 0x310 */ float mCheatSteerAlign; // [X] = 0.18 (ctor)
    /* 0x314 */ float mCheatSpeedLerp; // [X] = 0.015 (ctor)
    /* 0x318 */ float LegIK; // [O] LegIK (+0xF38); [U] the blend cRider_updateSkeleton passes to cRider_doLegIK
    /* 0x31C */ float ChatterMag; // cleared on transport (cRider_putAt); lwc1, compared > 0.0f, scales mGroundNormal (cRider_updateSkeleton)
    /* 0x320 */ int IsRidingFakie; // copied into mWasFlipped (cRider_OnStartObjectUse)
    /* 0x324 */ int IsGoofyStance; // [X] = cBECharacterInterface_getIsGoofyFoot(interface 2, mAIIndex) (cRider_initAlways)
    /* 0x328 */ int RailslideMode; // [X] landing state 1..4 from the board-vs-ground angle (cRider_checkRailBoardCollisions); 3/4 rotate the rider +-90 deg about mMatrix.row[2] (cRider_flagEventWipeout)
    /* 0x32C */ int HandplantMode; // cleared on transport (cRider_putAt)
    /* 0x330 */ int PressMode; // [X] landing kind: 0 handler path, 1 anim 0x1A, else anim 0x22 (cRider_checkRailBoardCollisions)
    /* 0x334 */ char unk_334[0xC];
    /* 0x340 */ t4Vector PunchVect; // [X] zeroed on transport; dot with the direction to another rider must be > 0 to shove (cRider_checkOpponentCollisions)
    /* 0x350 */ float PunchStrength; // [X] shove-strength factor (cRider_checkOpponentCollisions)
    /* 0x354 */ int ReactionEventTick; // [X] = -1 on transport (cRider_putAt)
    /* 0x358 */ int ReactionEventType; // [X] cleared on transport (cRider_putAt)
    /* 0x35C */ float ProximityActionDelay; // cleared on transport (cRider_putAt); += 1/60 per frame, compared < 1.0f (cRider_checkForProximityAction)
    /* 0x360 */ float JumpHeldSec; // [O] cRiderState::JumpHeldSec (+0xF70, after ProximityActionDelay); [U] cRider_checkForPrewind tests it == 0 and sets it to 1.0 as On Tour's checkForPrewind does at +0xF70; the gainFocus of cBoardPressControl / cRailslideControl / cCruiseControl clear it
    /* 0x364 */ char unk_364[0xC];
    /* 0x370 */ t4Vector mGroundNormal; // [P] getGroundNormal, preview 0x350 (+0x20 assumed); slope = atan2(|xy|, z) in cRider_updateSplit12
    /* 0x380 */ t4Vector LastGroundNormal; // [O] LastGroundNormal; [U] copy of mGroundNormal taken before the ground probe (cRider_SetNISGroundVectors)
    /* 0x390 */ t4Vector DampGroundNormal; // [O] DampGroundNormal; [U] reference axis for lean rotation: Cross/Dot with the bone side vector (cRider_updateSkeleton)
    /* 0x3A0 */ t4Vector mGroundForward; // [X] Normalize(front minus its mGroundNormal component) (cRider_SetNISGroundVectors)
    /* 0x3B0 */ t4Vector mGroundSide; // [X] Cross(mGroundNormal, unk_3A0); scaled by lean (doLeanPoseAdjust, cRider_SetNISGroundVectors)
    /* 0x3C0 */ t4Vector HalfPipeNormal; // [O] cRiderState::HalfPipeNormal (+0xFD0, between DigDir = mGroundSide and GroundVelocity); [R] cRiderCameraTarget_getHalfPipeNormal returns &rider+0x3C0 (On Tour: cRiderState::getHalfPipeNormal)
    /* 0x3D0 */ t4Vector GroundVelocity; // [O] GroundVelocity (after HalfPipeNormal); [U] hit.v20, or t4vect_Zero on a miss (cRider_SetNISGroundVectors)
    /* 0x3E0 */ t4Vector mImpulse; // [X] damped by 0.98 per frame (cRider_updateSplit3)
    /* 0x3F0 */ float mImpactAccum; // [X]
    /* 0x3F4 */ float mImpactAccum2; // [X]
    /* 0x3F8 */ char unk_3F8[0x4];
    /* 0x3FC */ int InTunnel; // [O] (On Tour bool, +0x100C there) [U] set/isInTunnel; RiderInTunnelFunc sets it
    /* 0x400 */ t4Vector mBoundsMin; // [X] with 0x410: box tested against the view (cRider_renderClip)
    /* 0x410 */ t4Vector mBoundsMax; // [X]
    /* 0x420 */ t4Vector mBoundSphere; // [X] tested by the VU0 frustum program
    /* 0x430 */ unsigned int mGroundPolyId; // [X] built as 0xFFFFFFFF with lui/ori (unsigned) in the ctor and cRider_SetNISGroundVectors
    /* 0x434 */ int mGroundMaterial; // [X]
    /* 0x438 */ int mSurfaceType; // [X] ground material from the hit, -1 mapped to 0 (cRider_SetNISGroundVectors)
    /* 0x43C */ char unk_43C[0x14];
    /* 0x450 */ int unk_450; // = -1 (ctor)
    /* 0x454 */ float SnowOffset; // Dot(probe - hit.pos, normal): height above ground (cRider_SetNISGroundVectors)
    /* 0x458 */ char unk_458[0x8];
    /* 0x460 */ t4Vector mGroundContactPoint; // [X] [U] ground hit position (cRider_SetNISGroundVectors)
    /* 0x470 */ float mFinishTime; // [P] preview 0x440 (+0x30): <0 racing, >=0 seconds past the line (isPastFinishLine)
    /* 0x474 */ int mStartTick; // [P] getStartTick, preview 0x444 (+0x30)
    /* 0x478 */ int mFinishTick; // [P] getFinishTick, preview 0x448 (+0x30); = now + mPenaltyTicks (cRider_doFinishEvent)
    /* 0x47C */ int mPenaltyTicks; // [P] getPenaltyTicks, preview 0x44C (+0x30)
    /* 0x480 */ int mDidNotFinish; // [P] getDidNotFinish, preview 0x450 (+0x30); set by cRider_quitEvent
    /* 0x484 */ int mEndOfTheWorldTimer; // [O] cRiderState::mEndOfTheWorldTimer (+0x1094, after isDidNotFinish); [U] cComputer_getSpinCmd counts frames with > 15 s of air time in it and sets cmd bit 12 at 31, as On Tour's getSpinCmd does with rider+0x1094
    /* 0x488 */ char unk_488[0x8];
    /* 0x490 */ t4Vector PathPos; // [O] cRiderState::PathPos (+0x10A0); [U] the nearest AI-path point: cPath_intersectPath1's output in cRider_initPath / updateAIPath / checkAIPathSwitch / switchToResetPath (On Tour updateAIPath: intersectPath(.., this+0x10A0, this+0x10D8, ..)); the reset position (cResetControl_update)
    /* 0x4A0 */ t4Vector IntersectPos; // [O] cRiderState::IntersectPos (+0x10B0); [U] the look-ahead path point cPath_getPathPos(AIPath, PathParam + ahead) (On Tour updateAIPath stores getPathPos at +0x10B0); cRider_setAimTarget copies it to AimTargetPos
    /* 0x4B0 */ t4Vector AimTargetPos; // [O] cRiderState::AimTargetPos (+0x10C0); [U] cRider_setAimTarget writes it (On Tour setAimTarget)
    /* 0x4C0 */ float PrevPathParam; // [O] (+0x10D0); [U] cRider_updateAIPath: PrevPathParam = PathParam first (On Tour: lwc1 0x10D4 / swc1 0x10D0); the low end of the path-event queries (cComputer_checkForJumpEvent etc.)
    /* 0x4C4 */ float PathParam; // [O] (+0x10D4); [U] the AI-path position returned by cPath_intersectPath1 (cRider_updateAIPath, initPath, switchToResetPath)
    /* 0x4C8 */ float PathDistXY; // [O] (+0x10D8); [U] cPath_intersectPath1's distance output; > 500 starts a path switch (cRider_updateAIPath, On Tour kPathStrayDist vs +0x10D8)
    /* 0x4CC */ float PathHeading; // [O] (+0x10DC); [U] atan2 of IntersectPos - PathPos (cRider_updateAIPath; On Tour fATan2 -> swc1 0x10DC); read by cComputer_attackTargetCruise / avoidTargetCruise
    /* 0x4D0 */ float mDistToFinish; // [X]
    /* 0x4D4 */ float MinDistanceLeft; // [O] cRiderState::MinDistanceLeft (+0x10E4, after DistanceLeft = mDistToFinish); [U] the lowest track distance so far: cRider_checkTrackPathEvents fires checkpoints only below it and then lowers it; cRider_initPath sets it with mDistToFinish; > 0 gates the path switches (cRider_updateAIPath / updateTrackPath; On Tour updateAIPath: lwc1 0x10E4)
    /* 0x4D8 */ float TotalPathDistance; // [O] (+0x10E8); [R] cRider_getTotalPathDistance / cRider_setTotalPathDistance (On Tour getTotalPathDistance: lwc1 0x10E8)
    /* 0x4DC */ tPathEventNode PathEventsCur[12]; // [O] (+0x10EC, tPathEventNode[12]); [U] cRider_checkTrackPathEvents has the path's getEventsInRange fill up to 0xC entries here and stores the count at 0x59C (NumPathEventsCur = On Tour NumPathEventsCur); cRider_updateTrackPath copies count << 4 bytes
    /* 0x59C */ int NumPathEventsCur; // [O] NumPathEventsCur, PathEventsCur's count (max 12); [U] set by cRider_checkTrackPathEvents, cleared on reset (cRider_initAlways)
    /* 0x5A0 */ int ConsecutiveAirFrames; // cleared on reset (cRider_initAlways)
    /* 0x5A4 */ float VerticalJump; // cleared on reset (cRider_initAlways); [U] read with lwc1 (cRider_getVerticalJump)
    /* 0x5A8 */ int DidPop; // cleared on reset (cRider_initAlways)
    /* 0x5AC */ int RidingFlexRail; // cleared on reset (cRider_initAlways)
    /* 0x5B0 */ int DangerLevel;
    /* 0x5B4 */ float mSmoothedHeading; // [X] smoothed slope angle (cRider_updateSplit12)
    /* 0x5B8 */ unsigned int mVisitedIds; // [X]
    /* 0x5BC */ char unk_5BC[0x104];
    /* 0x6C0 */ void* mVtbl; // cRider vtable (_vt$6cRider); cBeing's vptr slot
    /* 0x6C4 */ char unk_6C4[0xC];
    /* 0x6D0 */ void* mScriptObjRiderVtbl; // _vt$6cRider$15cScriptObjRider; start of that base
    /* 0x6D4 */ char unk_6D4[0x4];
    /* 0x6D8 */ void* mpEntity;
    /* 0x6DC */ char unk_6DC[0x8]; // cScriptObject data; its vptr follows
    /* 0x6E4 */ int mInUse; // count or gate in the cScriptObject data: <= 0 makes cRider_GetNISEndPosition return mPosition unchanged
    /* 0x6E8 */ void* mScriptObjectVtbl; // _vt$6cRider$13cScriptObject
    /* 0x6EC */ char unk_6EC[0x14];
    /* 0x700 */ t4Matrix mScriptMatrix; // [X] start transform used by a VU0 matrix*vector (cRider_OnStartObjectUse)
    /* 0x740 */ float mBaseRotX; // [O] cScriptObjRider::mBaseRotX (the cScriptObjRider base at 0x6D0); [U] added to mScriptCurrRotX (cRider_OnStartObjectUse)
    /* 0x744 */ float mBaseRotY; // [O] cScriptObjRider::mBaseRotY; [U] added to mScriptCurrRotY (cRider_OnStartObjectUse)
    /* 0x748 */ float mBaseRotZ; // [O] cScriptObjRider::mBaseRotZ; [U] added to mScriptCurrRotZ (cRider_OnStartObjectUse)
    /* 0x74C */ char unk_74C[0x4];
    /* 0x750 */ float mAudioBraking; // [P] getAudioBraking_0_to_1 (bracketed +0x40); cRider_SetNISGroundVectors also writes it as int 0
    /* 0x754 */ float mAudioEdging; // [P] getAudioEdging_0_to_1
    /* 0x758 */ float mAudioPressure; // [P] getAudioPressure_0_to_1
    /* 0x75C */ float mAudioSpeed; // [P] getAudioSpeed_0_to_1
    /* 0x760 */ int LastMotionType[2]; // [O] (On Tour cRider, the same order); [U] [0] = cRider_getMotionState(rider) when it
                                    //   changes (cSSXAudio_HandleAIGlide / HandleGlide), [1] written with it (cSSXAudio_BeginRiderSound)
    /* 0x768 */ int LastSnowType[2]; // [O]; [U] [0] the surface sound id from cSSXAudio_Surface (HandleAIGlide / HandleGlide),
                                    //   [1] HandleCarve's (9 on one cSSXAudio_GetSurfaceProgramType result)
    /* 0x770 */ float LastLandSpeed; // fed to curve D_00482D30 for the sound volume (cSSXAudio_Land)
    /* 0x774 */ int mWipeoutSlideHandle; // [P] GetWipeoutSlideHandle (bracketed +0x40)
    /* 0x778 */ int mWipeoutTumbleHandle; // [P] GetWipeoutTumbleHandle
    /* 0x77C */ cRiderHandler* mRiderHandler; // [P] getRiderHandler (bracketed +0x40)
    /* 0x780 */ cAnimModel* mAnimModel; // [P] getAnimModel (seen); passed to cAnimModel_addModelPartLOD
    /* 0x784 */ cRiderAnim* mRiderAnim; // [P] getRiderAnim (seen)
    /* 0x788 */ cAirPredictor* mAirPredictor; // [P] getAirPredictor (seen); passed to cAirPredictor_startLaunchIntoAir
    /* 0x78C */ void* mFocusBox; // [P] getFocusBox (seen); set by cRider_addFocusBox
    /* 0x790 */ cRiderMetrix* mMetrix; // [P] getMetrix (seen); set by cRider_addRiderMetrix
    /* 0x794 */ void* ActiveLights[8]; // cleared as 0x20 bytes (cRider_flushActiveLights)
    /* 0x7B4 */ char unk_7B4[0xC];
    /* 0x7C0 */ char unk_7C0[0xA0] __attribute__((aligned(16))); // copied as one 0xA0-byte block (cRider_SetRiderLights)
    /* 0x860 */ cOverlapList* mOverlapList; // [O] cRider::OverlapList (cOverlapList*); [R] ctor: new ("OverlapList")
                                            //      cOverlapList; [U] cRider_updateSplit1 reads its deletecount (+0x4)
    /* 0x864 */ tIntHint* mLineHint; // [O] cRider::LineHint (tIntHint*); [R] ctor: cMemMan_alloc(0x10, "Line Intersect Hint")
    /* 0x868 */ tIntHint* mWallHint; // [O] cRider::WallHint (tIntHint*); [R] ctor: cMemMan_alloc(0x10, "Wall Intersect Hint")
    /* 0x86C */ int mAIIndex; // [R] getAIIndex (cRider_getAIIndex); also the character/stat lookup index
    /* 0x870 */ int mViewIndex; // [P] getViewIndex (bracketed +0x114); < 2 = has a split-screen view
    /* 0x874 */ int mIsHuman; // [R] isHuman (cRider_isHuman); isComputer = !mIsHuman
    /* 0x878 */ int mIsActor; // [P] isActor (bracketed +0x114)
    /* 0x87C */ int mIsLocal; // [P] isLocal (bracketed +0x114)
    /* 0x880 */ int mLoadStep; // [X] init state; 7 = initialised (cRider_initOnce)
    /* 0x884 */ int mRenderFlag; // [P] getRenderFlag (bracketed +0x114)
    /* 0x888 */ int mSafeRenderTick; // [P] getSafeRenderTick (bracketed +0x114); -1 = unset (cRider_isSafeToDelete)
    /* 0x88C */ void* mAttachedEmitterFX; // [X] object with its vptr at +0xA4
    /* 0x890 */ void* mBoltOnFX; // [X] object with its vptr at +0xC4
    /* 0x894 */ void* mLoadList; // [X] anim name table; two different views in func_0011BD60 / func_0011BE88
    /* 0x898 */ int mLOD; // [U] 0-2 from camera distance (func_00121F30)
    /* 0x89C */ int mBoneHips; // [X] index into mAnimModel's 0x20-byte transform array
    /* 0x8A0 */ int mBoneBoardRoot; // [X] same array
    /* 0x8A4 */ int mBoneBoardChild; // [X] second bone index into mAnimModel localPos/localRot/bones, like unk_8A0 (cRider_updateSkeleton)
    /* 0x8A8 */ int mBoneHead; // [X] bone index into mAnimModel+0x30 (0x40-byte matrices, row 3 = position) (cPlayerIndicatorFX_render, func_002D5048)
    /* 0x8AC */ char unk_8AC[0x14];
    /* 0x8C0 */ long mUpperBodyBoneMask; // [X] 64-bit (PORT: long is 64-bit on the EE); copied to mRiderAnim+0x20 before each cRiderAnimBase_play, then reset to -1 (cRider_checkForPunch, cRider_checkForProximityAction)
    /* 0x8C8 */ long GraftFromUpperSpine; // [O] cRider::GraftFromUpperSpine (unsigned long long, +0x1478, between GraftFromLowerSpine = mUpperBodyBoneMask and GraftFromClavicles = mHeadBoneMask); [U] 64-bit mask copied to mRiderAnim+0x20 before cRiderAnimBase_play in cRider_checkForReaction, as On Tour's checkForReaction does with +0x1478 (PORT: long is 64-bit on the EE)
    /* 0x8D0 */ long mHeadBoneMask; // [X] 64-bit; same use for the wide-glance anims (cRider_checkForProximityAction)
    /* 0x8D8 */ char unk_8D8[0x8];
    /* 0x8E0 */ tLegInfo RightLeg; // [O] cRider::RightLeg (+0x1490); [P] assert "RightLeg.FootSkelIndex >= 0"; [U] cRider_updateSkeleton: cRider_doLegIK(self, &RightLeg, board * RightLeg.BoardRelativePose (+0x8F0), LegIK), as On Tour's updateSkeleton (+0x1490 / +0x14A0, LegIK +0xF38)
                // RightLeg.FootSkelIndex: a bone index (cRider_RenderShadow)
    /* 0x910 */ tLegInfo LeftLeg; // [O] cRider::LeftLeg (+0x14C0); [P] assert "LeftLeg.FootSkelIndex >= 0"; [U] cRider_updateSkeleton: the second cRider_doLegIK (+0x910 / +0x920), as On Tour's (+0x14C0 / +0x14D0)
                // LeftLeg.FootSkelIndex: a bone index (cRider_RenderShadow)
    /* 0x940 */ char unk_940[0x90];
    /* 0x9D0 */ t4Vector mModelOffset; // [X] reset to t4vect_Zero each frame
    /* 0x9E0 */ tIntersectInfo CollideIntersectInfo; // [O] cRider::CollideIntersectInfo (+0x1500, after ResolveOffset = mModelOffset); [P] assert "CollideIntersectInfo.Inst == 0";
                // [U] cRider_checkObjectCollisions stores the object hit here (On Tour: assert, then this+0x1500); cRider_updateSplit10 passes it to doOnCollideScript
    /* 0xA60 */ tCollideInfo CollideInfo; // [O] cRider::CollideInfo (+0x1580, before SphereTree = mSphereTree); [U] cRider_checkObjectCollisions stores the collision response here with the hit (On Tour: this+0x1580); cRider_updateSplit10 passes it to doOnCollideScript
    /* 0xAA0 */ cRiderSphereTree* mSphereTree; // [P] getSphereTree (seen); [O] cRider::SphereTree (cRiderSphereTree*); [U] the motions set its sphMask (+0x28, kInAirSphereMask)
    /* 0xAA4 */ int unk_AA4; // shadow drawn this frame (cRider_RenderShadow)
    /* 0xAA8 */ void* mShadow; // [P] getShadow (seen); drawn by cRider_RenderShadow
    /* 0xAAC */ t2Vector PatchUV; // [O]; [U] cRider_getPatchUV returns its address; the hit's u / v (cRider_SetNISGroundVectors,
                                 //   the wipeout motion); the ctor copies t2vect_Zero
    /* 0xAB4 */ cTrackPath* TrackPath; // [R] cRider_getTrackPath returns it; [O] cRider::TrackPath (On Tour: TrackPath,
                                     //     AIPath, then the two intersect infos, as here); [U] the lowest track path wins
    /* 0xAB8 */ cAIPath* AIPath;       // [R] cRider_getAIPath returns it; [O] cRider::AIPath (getAIPath -> cAIPath*);
                                     //     [U] the AI intersects it with mAIPathHint; replay frames store its index
    /* 0xABC */ tPathIntersectInfo* mAIPathHint; // [O] cRider::AIPathIntersect (tPathIntersectInfo*, after AIPath); [R] ctor: cMemMan_alloc(0x20, "AI path intersect info"); [U] NodeID (+0x14) = -1 before cPath_intersectPath
    /* 0xAC0 */ tPathIntersectInfo* mTrackPathHint; // [O] cRider::TrackPathIntersect (tPathIntersectInfo*); [R] ctor: cMemMan_alloc(0x20, "Track path intersect info"); [U] NodeID (+0x14) = -1
    /* 0xAC4 */ int mIsInNIS; // [P] IsInNIS (seen); most updates are skipped while set
    /* 0xAC8 */ int mHasTriggeredSpeechEvents; // set when an anim event fires
    /* 0xACC */ char unk_ACC[0x4];
    /* 0xAD0 */ int mHiddenNIS;
    /* 0xAD4 */ char unk_AD4[0xC];
    /* 0xAE0 */ t4Vector mScriptCurrPosOffset; // start position (w = 1), zeroed in the ctor (cRider_OnStartObjectUse)
    /* 0xAF0 */ float mScriptCurrRotX; // start angle, degrees * pi/180 (cRider_OnStartObjectUse)
    /* 0xAF4 */ float mScriptCurrRotY; // start angle, degrees * pi/180 (cRider_OnStartObjectUse)
    /* 0xAF8 */ float mScriptCurrRotZ; // start angle, degrees * pi/180 (cRider_OnStartObjectUse)
    /* 0xAFC */ int mNISShowBoardVisualFX;
    /* 0xB00 */ int mOnGroundNIS;
    /* 0xB04 */ float mLastSpeed; // speed (cRider_SetNISGroundVectors), 0.0f (cRider_OnStartObjectUse)
    /* 0xB08 */ int mSoundHandles[4]; // sound ids, -1 = none (cRider_OnEndObjectUse)
    /* 0xB18 */ int mIsOnScreen; // [P] IsRiderOnScreen, preview 0x9F8 (+0x120 assumed); [U] frustum test result (cRider_renderClip)
    /* 0xB1C */ int mUpdateMinAnimOnly;
    /* 0xB20 */ int mWipeoutRecoveryMode; // = 1 (ctor)
    /* 0xB24 */ float mBoostMeterStart; // [X]
    /* 0xB28 */ int mBoostSourceMask; // [X] = 31 (ctor)
    /* 0xB2C */ int mUberFlags; // [X]
    /* 0xB30 */ int mStartHandicap; // [X] = 0 (ctor)
    /* 0xB34 */ int mStatsIndex; // [X] stat column for cBEStatInterface_getCollisionAttrib
};

CHECK_OFFSET(cRider, unk_0D8, 0xD8);
CHECK_OFFSET(cRider, mRubberBandBehind, 0xDC);
CHECK_OFFSET(cRider, mRubberBandAhead, 0xE0);
CHECK_OFFSET(cRider, mGlueStrengthLevel, 0xE4);
CHECK_OFFSET(cRider, PrevRanking, 0xE8);
CHECK_OFFSET(cRider, RaceRanking, 0xEC);
CHECK_OFFSET(cRider, PissedOff, 0xF0);
CHECK_OFFSET(cRider, LastPunchTick, 0xF4);
CHECK_OFFSET(cRider, PissedAIIndex, 0xF8);
CHECK_OFFSET(cRider, PissedOffType, 0xFC);
CHECK_OFFSET(cRider, unk_100, 0x100);
CHECK_OFFSET(cRider, mPosition, 0x110);
CHECK_OFFSET(cRider, mRotation, 0x120);
CHECK_OFFSET(cRider, mUseRootPoseOverride, 0x150);
CHECK_OFFSET(cRider, RiderRoot, 0x160);
CHECK_OFFSET(cRider, mMatrix, 0x1A0);
CHECK_OFFSET(cRider, mVelocity, 0x1E0);
CHECK_OFFSET(cRider, mSlewed, 0x1F0);
CHECK_OFFSET(cRider, SurfaceFlags, 0x2D4);
CHECK_OFFSET(cRider, mWasFlipped, 0x2D8);
CHECK_OFFSET(cRider, mSpinRate, 0x2DC);
CHECK_OFFSET(cRider, mFlipRate, 0x2E0);
CHECK_OFFSET(cRider, TerminalVelocity, 0x2E4);
CHECK_OFFSET(cRider, mPowerUpBoostTimer, 0x2E8);
CHECK_OFFSET(cRider, mPowerUpTrickTimer, 0x2EC);
CHECK_OFFSET(cRider, mUberTimer, 0x2F0);
CHECK_OFFSET(cRider, mUberLevel, 0x2F4);
CHECK_OFFSET(cRider, BoostPowerBar, 0x2F8);
CHECK_OFFSET(cRider, mBoostStrength, 0x2FC);
CHECK_OFFSET(cRider, mTimeScale, 0x300);
CHECK_OFFSET(cRider, mBoostMode, 0x304);
CHECK_OFFSET(cRider, mAllowNaturalAir, 0x308);
CHECK_OFFSET(cRider, mBoostMaxSpeed, 0x30C);
CHECK_OFFSET(cRider, mCheatSteerAlign, 0x310);
CHECK_OFFSET(cRider, mCheatSpeedLerp, 0x314);
CHECK_OFFSET(cRider, LegIK, 0x318);
CHECK_OFFSET(cRider, ChatterMag, 0x31C);
CHECK_OFFSET(cRider, IsRidingFakie, 0x320);
CHECK_OFFSET(cRider, IsGoofyStance, 0x324);
CHECK_OFFSET(cRider, RailslideMode, 0x328);
CHECK_OFFSET(cRider, HandplantMode, 0x32C);
CHECK_OFFSET(cRider, PressMode, 0x330);
CHECK_OFFSET(cRider, PunchVect, 0x340);
CHECK_OFFSET(cRider, PunchStrength, 0x350);
CHECK_OFFSET(cRider, ReactionEventTick, 0x354);
CHECK_OFFSET(cRider, ReactionEventType, 0x358);
CHECK_OFFSET(cRider, ProximityActionDelay, 0x35C);
CHECK_OFFSET(cRider, JumpHeldSec, 0x360);
CHECK_OFFSET(cRider, mGroundNormal, 0x370);
CHECK_OFFSET(cRider, LastGroundNormal, 0x380);
CHECK_OFFSET(cRider, mGroundForward, 0x3A0);
CHECK_OFFSET(cRider, mGroundSide, 0x3B0);
CHECK_OFFSET(cRider, HalfPipeNormal, 0x3C0);
CHECK_OFFSET(cRider, GroundVelocity, 0x3D0);
CHECK_OFFSET(cRider, mImpulse, 0x3E0);
CHECK_OFFSET(cRider, mImpactAccum, 0x3F0);
CHECK_OFFSET(cRider, mImpactAccum2, 0x3F4);
CHECK_OFFSET(cRider, InTunnel, 0x3FC);
CHECK_OFFSET(cRider, mBoundsMin, 0x400);
CHECK_OFFSET(cRider, mBoundsMax, 0x410);
CHECK_OFFSET(cRider, mBoundSphere, 0x420);
CHECK_OFFSET(cRider, mGroundPolyId, 0x430);
CHECK_OFFSET(cRider, mGroundMaterial, 0x434);
CHECK_OFFSET(cRider, mSurfaceType, 0x438);
CHECK_OFFSET(cRider, unk_450, 0x450);
CHECK_OFFSET(cRider, SnowOffset, 0x454);
CHECK_OFFSET(cRider, mGroundContactPoint, 0x460);
CHECK_OFFSET(cRider, mFinishTime, 0x470);
CHECK_OFFSET(cRider, mStartTick, 0x474);
CHECK_OFFSET(cRider, mFinishTick, 0x478);
CHECK_OFFSET(cRider, mPenaltyTicks, 0x47C);
CHECK_OFFSET(cRider, mDidNotFinish, 0x480);
CHECK_OFFSET(cRider, mEndOfTheWorldTimer, 0x484);
CHECK_OFFSET(cRider, PathPos, 0x490);
CHECK_OFFSET(cRider, IntersectPos, 0x4A0);
CHECK_OFFSET(cRider, AimTargetPos, 0x4B0);
CHECK_OFFSET(cRider, PrevPathParam, 0x4C0);
CHECK_OFFSET(cRider, PathParam, 0x4C4);
CHECK_OFFSET(cRider, PathDistXY, 0x4C8);
CHECK_OFFSET(cRider, PathHeading, 0x4CC);
CHECK_OFFSET(cRider, mDistToFinish, 0x4D0);
CHECK_OFFSET(cRider, MinDistanceLeft, 0x4D4);
CHECK_OFFSET(cRider, TotalPathDistance, 0x4D8);
CHECK_OFFSET(cRider, PathEventsCur, 0x4DC);
CHECK_OFFSET(cRider, NumPathEventsCur, 0x59C);
CHECK_OFFSET(cRider, ConsecutiveAirFrames, 0x5A0);
CHECK_OFFSET(cRider, VerticalJump, 0x5A4);
CHECK_OFFSET(cRider, DidPop, 0x5A8);
CHECK_OFFSET(cRider, RidingFlexRail, 0x5AC);
CHECK_OFFSET(cRider, DangerLevel, 0x5B0);
CHECK_OFFSET(cRider, mSmoothedHeading, 0x5B4);
CHECK_OFFSET(cRider, mVisitedIds, 0x5B8);
CHECK_OFFSET(cRider, mVtbl, 0x6C0);
CHECK_OFFSET(cRider, mScriptObjRiderVtbl, 0x6D0);
CHECK_OFFSET(cRider, mpEntity, 0x6D8);
CHECK_OFFSET(cRider, unk_6DC, 0x6DC);
CHECK_OFFSET(cRider, mInUse, 0x6E4);
CHECK_OFFSET(cRider, mScriptObjectVtbl, 0x6E8);
CHECK_OFFSET(cRider, mScriptMatrix, 0x700);
CHECK_OFFSET(cRider, mBaseRotX, 0x740);
CHECK_OFFSET(cRider, mBaseRotY, 0x744);
CHECK_OFFSET(cRider, mBaseRotZ, 0x748);
CHECK_OFFSET(cRider, mAudioBraking, 0x750);
CHECK_OFFSET(cRider, mAudioEdging, 0x754);
CHECK_OFFSET(cRider, mAudioPressure, 0x758);
CHECK_OFFSET(cRider, mAudioSpeed, 0x75C);
CHECK_OFFSET(cRider, LastMotionType, 0x760);
CHECK_OFFSET(cRider, LastSnowType, 0x768);
CHECK_OFFSET(cRider, LastLandSpeed, 0x770);
CHECK_OFFSET(cRider, mWipeoutSlideHandle, 0x774);
CHECK_OFFSET(cRider, mWipeoutTumbleHandle, 0x778);
CHECK_OFFSET(cRider, mRiderHandler, 0x77C);
CHECK_OFFSET(cRider, mAnimModel, 0x780);
CHECK_OFFSET(cRider, mRiderAnim, 0x784);
CHECK_OFFSET(cRider, mAirPredictor, 0x788);
CHECK_OFFSET(cRider, mFocusBox, 0x78C);
CHECK_OFFSET(cRider, mMetrix, 0x790);
CHECK_OFFSET(cRider, ActiveLights, 0x794);
CHECK_OFFSET(cRider, unk_7C0, 0x7C0);
CHECK_OFFSET(cRider, mOverlapList, 0x860);
CHECK_OFFSET(cRider, mLineHint, 0x864);
CHECK_OFFSET(cRider, mWallHint, 0x868);
CHECK_OFFSET(cRider, mAIIndex, 0x86C);
CHECK_OFFSET(cRider, mViewIndex, 0x870);
CHECK_OFFSET(cRider, mIsHuman, 0x874);
CHECK_OFFSET(cRider, mIsActor, 0x878);
CHECK_OFFSET(cRider, mIsLocal, 0x87C);
CHECK_OFFSET(cRider, mLoadStep, 0x880);
CHECK_OFFSET(cRider, mRenderFlag, 0x884);
CHECK_OFFSET(cRider, mSafeRenderTick, 0x888);
CHECK_OFFSET(cRider, mAttachedEmitterFX, 0x88C);
CHECK_OFFSET(cRider, mBoltOnFX, 0x890);
CHECK_OFFSET(cRider, mLoadList, 0x894);
CHECK_OFFSET(cRider, mLOD, 0x898);
CHECK_OFFSET(cRider, mBoneHips, 0x89C);
CHECK_OFFSET(cRider, mBoneBoardRoot, 0x8A0);
CHECK_OFFSET(cRider, mBoneHead, 0x8A8);
CHECK_OFFSET(cRider, mUpperBodyBoneMask, 0x8C0);
CHECK_OFFSET(cRider, GraftFromUpperSpine, 0x8C8);
CHECK_OFFSET(cRider, mHeadBoneMask, 0x8D0);
CHECK_OFFSET(cRider, RightLeg, 0x8E0);
CHECK_OFFSET(cRider, LeftLeg, 0x910);
CHECK_OFFSET(cRider, mModelOffset, 0x9D0);
CHECK_OFFSET(cRider, CollideIntersectInfo, 0x9E0);
CHECK_OFFSET(cRider, CollideInfo, 0xA60);
CHECK_OFFSET(cRider, mSphereTree, 0xAA0);
CHECK_OFFSET(cRider, unk_AA4, 0xAA4);
CHECK_OFFSET(cRider, mShadow, 0xAA8);
CHECK_OFFSET(cRider, PatchUV, 0xAAC);
CHECK_OFFSET(cRider, TrackPath, 0xAB4);
CHECK_OFFSET(cRider, AIPath, 0xAB8);
CHECK_OFFSET(cRider, mAIPathHint, 0xABC);
CHECK_OFFSET(cRider, mTrackPathHint, 0xAC0);
CHECK_OFFSET(cRider, mIsInNIS, 0xAC4);
CHECK_OFFSET(cRider, mHasTriggeredSpeechEvents, 0xAC8);
CHECK_OFFSET(cRider, mHiddenNIS, 0xAD0);
CHECK_OFFSET(cRider, mScriptCurrPosOffset, 0xAE0);
CHECK_OFFSET(cRider, mScriptCurrRotX, 0xAF0);
CHECK_OFFSET(cRider, mScriptCurrRotY, 0xAF4);
CHECK_OFFSET(cRider, mScriptCurrRotZ, 0xAF8);
CHECK_OFFSET(cRider, mNISShowBoardVisualFX, 0xAFC);
CHECK_OFFSET(cRider, mOnGroundNIS, 0xB00);
CHECK_OFFSET(cRider, mLastSpeed, 0xB04);
CHECK_OFFSET(cRider, mSoundHandles, 0xB08);
CHECK_OFFSET(cRider, mIsOnScreen, 0xB18);
CHECK_OFFSET(cRider, mUpdateMinAnimOnly, 0xB1C);
CHECK_OFFSET(cRider, mWipeoutRecoveryMode, 0xB20);
CHECK_OFFSET(cRider, mBoostMeterStart, 0xB24);
CHECK_OFFSET(cRider, mBoostSourceMask, 0xB28);
CHECK_OFFSET(cRider, mUberFlags, 0xB2C);
CHECK_OFFSET(cRider, mStartHandicap, 0xB30);
CHECK_OFFSET(cRider, mStatsIndex, 0xB34);

#endif /* AI_RIDER_H */

