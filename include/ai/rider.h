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
//     vtables the destructor (func_0011B978) installs, _vt$6cRider / $15cScriptObjRider / $13cScriptObject.
//   - cRider is a *virtual* base of cPlayer (at cPlayer+0xE10, see func_00125C70) and of about 30
//     rider components (cGroundMotion, cRailslideMotion, cWipeoutMotion, cHandplantMotion, cTrickBoostFX,
//     cAirStreamerFX, ...): their preview vtables are named _vt$<Component>$6cRider.
//
// Field names come from, in order of strength:
//   [R] a retail accessor body (e.g. func_00140B80 = cRider::getAIIndex: lw v0,0x86C(a0));
//   [P] a preview-build accessor, carried to retail by tools/layout_xlate.py ("seen" = the offset pair
//       was observed in aligned preview/retail code; "bracketed" = between two observed pairs with the
//       same shift) and confirmed by how retail code uses the field;
//   [U] how the matched code uses it (comment says how).
// Everything else is unk_XXX. Rename only with evidence, and say which kind.

#include "layout.h"
#include "bx/math3d.h"

class cAnimModel;
class cRiderAnim;
class cAirPredictor;
class cRiderMetrix;
class cRiderHandler;

// Virtual slots of cRider's primary vtable (_vt$6cRider, retail D_00459C18), as used by
// `vt[k].fn(sub + vt[k].delta)` with sub = &self->mVtbl. Names from the preview vtable.
enum {
    kRiderVt_delete = 1,          // deleting destructor (func_0011B978)
    kRiderVt_getVelocity = 2,
    kRiderVt_isIntersecting = 3,
    kRiderVt_applyBoost = 4,      // (const t4Vector* impulse): func_001250A8
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

struct cRider {
    /* 0x000 */ char unk_000[0x100];
    /* 0x100 */ int unk_100; // cleared when the rider quits (func_00125108)
    /* 0x104 */ char unk_104[0xC];
    /* 0x110 */ t4Vector mPosition; // [R] getPosition (func_001408F0)
    /* 0x120 */ t4Vector mRotation; // [P] getRotation (unshifted); quaternion, normalised by updateOrientationImplicit
    /* 0x130 */ char unk_130[0x20];
    /* 0x150 */ int unk_150;
    /* 0x154 */ char unk_154[0x4C];
    /* 0x1A0 */ t4Matrix mMatrix; // [P] rows getSide/getFront/getUp (unshifted) + position; built from mRotation/mPosition
    /* 0x1E0 */ t4Vector mVelocity; // [R] getVelocity (func_00140910); [P] setVelocity
    /* 0x1F0 */ char unk_1F0[0xF8];
    /* 0x2E8 */ float unk_2E8; // decays each frame (func_001200D0)
    /* 0x2EC */ float unk_2EC;
    /* 0x2F0 */ float unk_2F0;
    /* 0x2F4 */ int unk_2F4; // a mode: 5, 10, >= 11 tested
    /* 0x2F8 */ float unk_2F8;
    /* 0x2FC */ float unk_2FC; // scales the mass (cRider_getMass)
    /* 0x300 */ float unk_300; // decay rate for 0x2E8-0x2F8
    /* 0x304 */ int unk_304; // 0..3
    /* 0x308 */ char unk_308[0x10];
    /* 0x318 */ float unk_318;
    /* 0x31C */ char unk_31C[0x54];
    /* 0x370 */ t4Vector mGroundNormal; // [P] getGroundNormal, preview 0x350 (+0x20 assumed); slope = atan2(|xy|, z) in func_00121950
    /* 0x380 */ char unk_380[0x60];
    /* 0x3E0 */ t4Vector unk_3E0; // damped by 0.98 per frame (func_001210B0)
    /* 0x3F0 */ float unk_3F0;
    /* 0x3F4 */ float unk_3F4;
    /* 0x3F8 */ char unk_3F8[0x4];
    /* 0x3FC */ int unk_3FC;
    /* 0x400 */ t4Vector unk_400; // with 0x410: box tested against the view (func_00122278)
    /* 0x410 */ t4Vector unk_410;
    /* 0x420 */ t4Vector unk_420; // tested by the VU0 frustum program
    /* 0x430 */ int unk_430;
    /* 0x434 */ int unk_434;
    /* 0x438 */ char unk_438[0x28];
    /* 0x460 */ float unk_460;
    /* 0x464 */ float unk_464;
    /* 0x468 */ char unk_468[0x8];
    /* 0x470 */ float mFinishTime; // [P] preview 0x440 (+0x30): <0 racing, >=0 seconds past the line (isPastFinishLine)
    /* 0x474 */ int mStartTick; // [P] getStartTick, preview 0x444 (+0x30)
    /* 0x478 */ int mFinishTick; // [P] getFinishTick, preview 0x448 (+0x30); = now + mPenaltyTicks (func_00125108)
    /* 0x47C */ int mPenaltyTicks; // [P] getPenaltyTicks, preview 0x44C (+0x30)
    /* 0x480 */ int mDidNotFinish; // [P] getDidNotFinish, preview 0x450 (+0x30); set by cRider_quitEvent
    /* 0x484 */ char unk_484[0x4C];
    /* 0x4D0 */ float unk_4D0;
    /* 0x4D4 */ char unk_4D4[0xDC];
    /* 0x5B0 */ int unk_5B0;
    /* 0x5B4 */ float unk_5B4; // smoothed slope angle (func_00121950)
    /* 0x5B8 */ unsigned int unk_5B8;
    /* 0x5BC */ char unk_5BC[0x104];
    /* 0x6C0 */ void* mVtbl; // cRider vtable (_vt$6cRider); cBeing's vptr slot
    /* 0x6C4 */ char unk_6C4[0xC];
    /* 0x6D0 */ void* mScriptObjRiderVtbl; // _vt$6cRider$15cScriptObjRider; start of that base
    /* 0x6D4 */ char unk_6D4[0x4];
    /* 0x6D8 */ void* unk_6D8;
    /* 0x6DC */ char unk_6DC[0xC]; // cScriptObject data; its vptr follows
    /* 0x6E8 */ void* mScriptObjectVtbl; // _vt$6cRider$13cScriptObject
    /* 0x6EC */ char unk_6EC[0x64];
    /* 0x750 */ float mAudioBraking; // [P] getAudioBraking_0_to_1 (bracketed +0x40)
    /* 0x754 */ float mAudioEdging; // [P] getAudioEdging_0_to_1
    /* 0x758 */ float mAudioPressure; // [P] getAudioPressure_0_to_1
    /* 0x75C */ float mAudioSpeed; // [P] getAudioSpeed_0_to_1
    /* 0x760 */ char unk_760[0x14];
    /* 0x774 */ int mWipeoutSlideHandle; // [P] GetWipeoutSlideHandle (bracketed +0x40)
    /* 0x778 */ int mWipeoutTumbleHandle; // [P] GetWipeoutTumbleHandle
    /* 0x77C */ cRiderHandler* mRiderHandler; // [P] getRiderHandler (bracketed +0x40)
    /* 0x780 */ cAnimModel* mAnimModel; // [P] getAnimModel (seen); passed to cAnimModel_addModelPartLOD
    /* 0x784 */ cRiderAnim* mRiderAnim; // [P] getRiderAnim (seen)
    /* 0x788 */ cAirPredictor* mAirPredictor; // [P] getAirPredictor (seen); passed to cAirPredictor_startLaunchIntoAir
    /* 0x78C */ void* mFocusBox; // [P] getFocusBox (seen); set by cRider_addFocusBox
    /* 0x790 */ cRiderMetrix* mMetrix; // [P] getMetrix (seen); set by cRider_addRiderMetrix
    /* 0x794 */ void* unk_794[8]; // cleared as 0x20 bytes (func_00122658)
    /* 0x7B4 */ char unk_7B4[0xC];
    /* 0x7C0 */ char unk_7C0[0xA0] __attribute__((aligned(16))); // copied as one 0xA0-byte block (func_001220D8)
    /* 0x860 */ void* unk_860;
    /* 0x864 */ void* unk_864;
    /* 0x868 */ void* unk_868;
    /* 0x86C */ int mAIIndex; // [R] getAIIndex (func_00140B80); also the character/stat lookup index
    /* 0x870 */ int mViewIndex; // [P] getViewIndex (bracketed +0x114); < 2 = has a split-screen view
    /* 0x874 */ int mIsHuman; // [R] isHuman (func_00140BC0); isComputer = !mIsHuman
    /* 0x878 */ int mIsActor; // [P] isActor (bracketed +0x114)
    /* 0x87C */ int mIsLocal; // [P] isLocal (bracketed +0x114)
    /* 0x880 */ int unk_880; // init state; 7 = initialised (cRider_initOnce)
    /* 0x884 */ int mRenderFlag; // [P] getRenderFlag (bracketed +0x114)
    /* 0x888 */ int mSafeRenderTick; // [P] getSafeRenderTick (bracketed +0x114); -1 = unset (func_0011C0E0)
    /* 0x88C */ void* unk_88C; // object with its vptr at +0xA4
    /* 0x890 */ void* unk_890; // object with its vptr at +0xC4
    /* 0x894 */ void* unk_894; // anim name table; two different views in func_0011BD60 / func_0011BE88
    /* 0x898 */ int mLOD; // [U] 0-2 from camera distance (func_00121F30)
    /* 0x89C */ int unk_89C; // index into mAnimModel's 0x20-byte transform array
    /* 0x8A0 */ int unk_8A0; // same array
    /* 0x8A4 */ char unk_8A4[0x44];
    /* 0x8E8 */ int unk_8E8; // bone index (func_00122898)
    /* 0x8EC */ char unk_8EC[0x2C];
    /* 0x918 */ int unk_918; // bone index (func_00122898)
    /* 0x91C */ char unk_91C[0xB4];
    /* 0x9D0 */ t4Vector unk_9D0; // reset to D_004FF120 each frame
    /* 0x9E0 */ char unk_9E0[0x50];
    /* 0xA30 */ void* unk_A30;
    /* 0xA34 */ char unk_A34[0x2C];
    /* 0xA60 */ char unk_A60[0x40];
    /* 0xAA0 */ void* mSphereTree; // [P] getSphereTree (seen)
    /* 0xAA4 */ int unk_AA4; // shadow drawn this frame (func_00122898)
    /* 0xAA8 */ void* mShadow; // [P] getShadow (seen); drawn by func_00122898
    /* 0xAAC */ char unk_AAC[0x10];
    /* 0xABC */ int* unk_ABC;
    /* 0xAC0 */ int* unk_AC0;
    /* 0xAC4 */ int mIsInNIS; // [P] IsInNIS (seen); most updates are skipped while set
    /* 0xAC8 */ int unk_AC8; // set when an anim event fires
    /* 0xACC */ char unk_ACC[0x4];
    /* 0xAD0 */ int unk_AD0;
    /* 0xAD4 */ char unk_AD4[0x28];
    /* 0xAFC */ int unk_AFC;
    /* 0xB00 */ int unk_B00;
    /* 0xB04 */ int unk_B04;
    /* 0xB08 */ int unk_B08[4]; // sound ids, -1 = none (func_00123B48)
    /* 0xB18 */ int mIsOnScreen; // [P] IsRiderOnScreen, preview 0x9F8 (+0x120 assumed); [U] frustum test result (func_00122278)
    /* 0xB1C */ int unk_B1C;
    /* 0xB20 */ char unk_B20[0x4];
    /* 0xB24 */ float unk_B24;
    /* 0xB28 */ char unk_B28[0x4];
    /* 0xB2C */ int unk_B2C;
    /* 0xB30 */ char unk_B30[0x4];
    /* 0xB34 */ int unk_B34; // stat column for cBEStatInterface_getCollisionAttrib
};

CHECK_OFFSET(cRider, unk_100, 0x100);
CHECK_OFFSET(cRider, mPosition, 0x110);
CHECK_OFFSET(cRider, mRotation, 0x120);
CHECK_OFFSET(cRider, unk_150, 0x150);
CHECK_OFFSET(cRider, mMatrix, 0x1A0);
CHECK_OFFSET(cRider, mVelocity, 0x1E0);
CHECK_OFFSET(cRider, unk_2E8, 0x2E8);
CHECK_OFFSET(cRider, unk_2EC, 0x2EC);
CHECK_OFFSET(cRider, unk_2F0, 0x2F0);
CHECK_OFFSET(cRider, unk_2F4, 0x2F4);
CHECK_OFFSET(cRider, unk_2F8, 0x2F8);
CHECK_OFFSET(cRider, unk_2FC, 0x2FC);
CHECK_OFFSET(cRider, unk_300, 0x300);
CHECK_OFFSET(cRider, unk_304, 0x304);
CHECK_OFFSET(cRider, unk_318, 0x318);
CHECK_OFFSET(cRider, mGroundNormal, 0x370);
CHECK_OFFSET(cRider, unk_3E0, 0x3E0);
CHECK_OFFSET(cRider, unk_3F0, 0x3F0);
CHECK_OFFSET(cRider, unk_3F4, 0x3F4);
CHECK_OFFSET(cRider, unk_3FC, 0x3FC);
CHECK_OFFSET(cRider, unk_400, 0x400);
CHECK_OFFSET(cRider, unk_410, 0x410);
CHECK_OFFSET(cRider, unk_420, 0x420);
CHECK_OFFSET(cRider, unk_430, 0x430);
CHECK_OFFSET(cRider, unk_434, 0x434);
CHECK_OFFSET(cRider, unk_460, 0x460);
CHECK_OFFSET(cRider, unk_464, 0x464);
CHECK_OFFSET(cRider, mFinishTime, 0x470);
CHECK_OFFSET(cRider, mStartTick, 0x474);
CHECK_OFFSET(cRider, mFinishTick, 0x478);
CHECK_OFFSET(cRider, mPenaltyTicks, 0x47C);
CHECK_OFFSET(cRider, mDidNotFinish, 0x480);
CHECK_OFFSET(cRider, unk_4D0, 0x4D0);
CHECK_OFFSET(cRider, unk_5B0, 0x5B0);
CHECK_OFFSET(cRider, unk_5B4, 0x5B4);
CHECK_OFFSET(cRider, unk_5B8, 0x5B8);
CHECK_OFFSET(cRider, mVtbl, 0x6C0);
CHECK_OFFSET(cRider, mScriptObjRiderVtbl, 0x6D0);
CHECK_OFFSET(cRider, unk_6D8, 0x6D8);
CHECK_OFFSET(cRider, unk_6DC, 0x6DC);
CHECK_OFFSET(cRider, mScriptObjectVtbl, 0x6E8);
CHECK_OFFSET(cRider, mAudioBraking, 0x750);
CHECK_OFFSET(cRider, mAudioEdging, 0x754);
CHECK_OFFSET(cRider, mAudioPressure, 0x758);
CHECK_OFFSET(cRider, mAudioSpeed, 0x75C);
CHECK_OFFSET(cRider, mWipeoutSlideHandle, 0x774);
CHECK_OFFSET(cRider, mWipeoutTumbleHandle, 0x778);
CHECK_OFFSET(cRider, mRiderHandler, 0x77C);
CHECK_OFFSET(cRider, mAnimModel, 0x780);
CHECK_OFFSET(cRider, mRiderAnim, 0x784);
CHECK_OFFSET(cRider, mAirPredictor, 0x788);
CHECK_OFFSET(cRider, mFocusBox, 0x78C);
CHECK_OFFSET(cRider, mMetrix, 0x790);
CHECK_OFFSET(cRider, unk_794, 0x794);
CHECK_OFFSET(cRider, unk_7C0, 0x7C0);
CHECK_OFFSET(cRider, unk_860, 0x860);
CHECK_OFFSET(cRider, unk_864, 0x864);
CHECK_OFFSET(cRider, unk_868, 0x868);
CHECK_OFFSET(cRider, mAIIndex, 0x86C);
CHECK_OFFSET(cRider, mViewIndex, 0x870);
CHECK_OFFSET(cRider, mIsHuman, 0x874);
CHECK_OFFSET(cRider, mIsActor, 0x878);
CHECK_OFFSET(cRider, mIsLocal, 0x87C);
CHECK_OFFSET(cRider, unk_880, 0x880);
CHECK_OFFSET(cRider, mRenderFlag, 0x884);
CHECK_OFFSET(cRider, mSafeRenderTick, 0x888);
CHECK_OFFSET(cRider, unk_88C, 0x88C);
CHECK_OFFSET(cRider, unk_890, 0x890);
CHECK_OFFSET(cRider, unk_894, 0x894);
CHECK_OFFSET(cRider, mLOD, 0x898);
CHECK_OFFSET(cRider, unk_89C, 0x89C);
CHECK_OFFSET(cRider, unk_8A0, 0x8A0);
CHECK_OFFSET(cRider, unk_8E8, 0x8E8);
CHECK_OFFSET(cRider, unk_918, 0x918);
CHECK_OFFSET(cRider, unk_9D0, 0x9D0);
CHECK_OFFSET(cRider, unk_9E0, 0x9E0);
CHECK_OFFSET(cRider, unk_A30, 0xA30);
CHECK_OFFSET(cRider, unk_A60, 0xA60);
CHECK_OFFSET(cRider, mSphereTree, 0xAA0);
CHECK_OFFSET(cRider, unk_AA4, 0xAA4);
CHECK_OFFSET(cRider, mShadow, 0xAA8);
CHECK_OFFSET(cRider, unk_ABC, 0xABC);
CHECK_OFFSET(cRider, unk_AC0, 0xAC0);
CHECK_OFFSET(cRider, mIsInNIS, 0xAC4);
CHECK_OFFSET(cRider, unk_AC8, 0xAC8);
CHECK_OFFSET(cRider, unk_AD0, 0xAD0);
CHECK_OFFSET(cRider, unk_AFC, 0xAFC);
CHECK_OFFSET(cRider, unk_B00, 0xB00);
CHECK_OFFSET(cRider, unk_B04, 0xB04);
CHECK_OFFSET(cRider, unk_B08, 0xB08);
CHECK_OFFSET(cRider, mIsOnScreen, 0xB18);
CHECK_OFFSET(cRider, unk_B1C, 0xB1C);
CHECK_OFFSET(cRider, unk_B24, 0xB24);
CHECK_OFFSET(cRider, unk_B2C, 0xB2C);
CHECK_OFFSET(cRider, unk_B34, 0xB34);

#endif /* AI_RIDER_H */

