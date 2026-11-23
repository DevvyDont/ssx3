#ifndef CAMERA_RACE_RACEALGORITHMS_H
#define CAMERA_RACE_RACEALGORITHMS_H

// The race chase-camera algorithms that add members to cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390):
//   cMatrixChaseAlgorithm   (0x3A0)  "Matrix Chase Camera Algorithm"   camera type 41
//   cRelativeChaseAlgorithm (0x3C0)  "Relative Chase Camera Algorithm" camera types 33..40 (angle = (type-33)*pi/4)
//   cSplineCamAlgorithm     (0x3D0)  "Spline Camera Algorithm"         camera type 92
// Sizes [R]: cChaseCameraController_createChaseAlgorithmBlend (camera/cameraalgorithms.cpp) allocates them with
// cMemMan_alloc(size, tag) and calls their constructors (cMatrixChaseAlgorithm_cMatrixChaseAlgorithm, cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(mem, angle),
// cSplineCamAlgorithm_cSplineCamAlgorithm(mem, p1..p4, time, leadDistance, heightOffset, fov)).
// Class names: the July 2003 preview's vtable slots (builds/x/v2/name_map_v2.tsv: cSplineCamAlgorithm::<virtual slot 1>
// = cSplineCamAlgorithm__cSplineCamAlgorithm ...) and SSX On Tour's DWARF, whose functions.tsv lists ctor, dtor, snap(int), update(),
// getRestDrop(), getRaceCamFinalPhaseData(cRaceCamFinalPhaseData*) for each (z:/main/game/source/camera/
// all_in_one_camera.cpp). EA's PS2 source file is not known: the preview names camera/race/racealgorithms.cpp,
// allspinchasealgo.cpp and directionchasealgo.cpp; these classes sit in retail between camera/cameraalgorithms' free
// cameras (0x15F938) and race/racecontroller's cChaseCameraController (0x161950), so racealgorithms.h is a guess.
// Field-less subclasses of cLookAtChaseAlgorithm in the same retail range (all 0x390; at the end of this file, each
// deriving from the base): cManualChaseAlgorithm, cIdleChaseAlgorithm, cLazyChaseAlgorithm, cLookBackChaseAlgorithm,
// cMidRightChaseAlgorithm, cMidLeftChaseAlgorithm. The DEFAULT_2/3/4 and Direction chase algorithms are in
// camera/race/directionchasealgo.h, SPOKE/SUPERPIPE/POST_RACE_1 in camera/race/allspinchasealgo.h.
//
// Retail vtables (13 slots: dtor, getPose, snap, snapCurrent, update, getTargetPos, getRaceCamFinalPhaseData,
// doRaceCamFinalPhase, updateAlgorithm, <pure>, shouldSnapOnUpdate, setTarget, getRestDrop):
//   cMatrixChaseAlgorithm cMatrixChaseAlgorithm_vtbl, cRelativeChaseAlgorithm cRelativeChaseAlgorithm_vtbl, cSplineCamAlgorithm cSplineCamAlgorithm_vtbl.
//
// FLAT layouts, like main/game.h: every field at its retail offset; the 0x390-byte cLookAtChaseAlgorithm base is
// copied in front of each (its evidence tags are cameraalgorithms.h's); no C++ inheritance. Own members: On Tour's
// names and order (On Tour's base is 0x380, so each sits 0x10 later in retail), placed by [R] the constructors and
// [U] the update functions.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"    // tPose via bx/math3d.h, cPIDFilter

struct cRiderCameraTarget;

// cMatrixChaseAlgorithm (0x3A0; On Tour 0x390). Ctor cMatrixChaseAlgorithm_cMatrixChaseAlgorithm (stores vtable cMatrixChaseAlgorithm_vtbl, AlgorithmType),
// dtor cMatrixChaseAlgorithm__cMatrixChaseAlgorithm, snap cMatrixChaseAlgorithm_snap, update cMatrixChaseAlgorithm_update, getRaceCamFinalPhaseData cMatrixChaseAlgorithm_getRaceCamFinalPhaseData,
// getRestDrop cMatrixChaseAlgorithm_getRestDrop.
struct cMatrixChaseAlgorithm : public cLookAtChaseAlgorithm {
    // cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390): the vptr at 0x10:
    //   [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm
    //       ...)
    // cMatrixChaseAlgorithm's notes on cLookAtChaseAlgorithm's fields:
    //   View: [R] ctor: pi/4
    //   AlgorithmType: [R] 0x54; subclass ctors store their own
    //   CamTargetPos: [U] sq/lqc2 in the update functions
    //   Target: [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    // --- cMatrixChaseAlgorithm
    virtual float getRestDrop();                                                    // 13 ([R] cMatrixChaseAlgorithm_vtbl: cMatrixChaseAlgorithm_getRestDrop__FPv)
    float LastAngle;                // 0x390 [O] (On Tour 0x380); [R] = 0 (sw zero); [U] cMatrixChaseAlgorithm_update update (lwc1/swc1)
    char unk_394[0xC];              // 0x394 padding to the 16-byte class alignment (alloc 0x3A0)
};

CHECK_OFFSET(cMatrixChaseAlgorithm, LastAngle, 0x390);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_394, 0x394);
CHECK_SIZE(cMatrixChaseAlgorithm, 0x3A0);

// cRelativeChaseAlgorithm (0x3C0; On Tour 0x3B0). Ctor cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(this, angle) (vtable cRelativeChaseAlgorithm_vtbl; the factory
// then stores the camera type into AlgorithmType), dtor cRelativeChaseAlgorithm__cRelativeChaseAlgorithm, snap cRelativeChaseAlgorithm_snap, update cRelativeChaseAlgorithm_update,
// getRaceCamFinalPhaseData cRelativeChaseAlgorithm_getRaceCamFinalPhaseData, getRestDrop cRelativeChaseAlgorithm_getRestDrop.
struct cRelativeChaseAlgorithm : public cLookAtChaseAlgorithm {
    // cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390): the vptr at 0x10:
    //   [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm
    //       ...)
    // cRelativeChaseAlgorithm's notes on cLookAtChaseAlgorithm's fields:
    //   View: [R] ctor: pi/4
    //   AlgorithmType: [R] 0x54; subclass ctors store their own
    //   CamTargetPos: [U] sq/lqc2 in the update functions
    //   Target: [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    // --- cRelativeChaseAlgorithm
    virtual float getRestDrop();                                                    // 13 ([R] cRelativeChaseAlgorithm_vtbl: cRelativeChaseAlgorithm_getRestDrop__FPv)
    float Angle;                    // 0x390 [O] (On Tour 0x380); [R] = angle (swc1); [U] cRelativeChaseAlgorithm_update (lwc1)
    int Initialized;                // 0x394 [O] bool (a word in retail); [R] = 0; [U] cRelativeChaseAlgorithm_update tests and sets it
    char unk_398[0x8];              // 0x398 alignment gap before the 16-byte-aligned t4Vector
    t4Vector LastLook;              // 0x3A0 [O] (On Tour 0x390); [U] cRelativeChaseAlgorithm_update (sq/lqc2)
    t4Vector LastCamTargetPos;      // 0x3B0 [O] (On Tour 0x3A0); [U] cRelativeChaseAlgorithm_update (sq/lqc2)
};

CHECK_OFFSET(cRelativeChaseAlgorithm, Angle, 0x390);
CHECK_OFFSET(cRelativeChaseAlgorithm, Initialized, 0x394);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_398, 0x398);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastLook, 0x3A0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastCamTargetPos, 0x3B0);
CHECK_SIZE(cRelativeChaseAlgorithm, 0x3C0);

// cSplineCamAlgorithm (0x3D0; On Tour 0x3C0). Ctor cSplineCamAlgorithm_cSplineCamAlgorithm(this, point1..point4, time, leadDistance,
// heightOffset, fov) (vtable cSplineCamAlgorithm_vtbl; builds the three interpolants with cCubicSplineInterpolant_cCubicSplineInterpolant / cCubicSplineInterpolant_initAsNatural /
// cCubicSplineInterpolant_addPoint / cCubicSplineInterpolant_evaluate), dtor cSplineCamAlgorithm__cSplineCamAlgorithm (cCubicSplineInterpolant__cCubicSplineInterpolant on each interpolant), snap cSplineCamAlgorithm_snap,
// update cSplineCamAlgorithm_update, getRaceCamFinalPhaseData cSplineCamAlgorithm_getRaceCamFinalPhaseData, getRestDrop cSplineCamAlgorithm_getRestDrop.
struct cSplineCamAlgorithm : public cLookAtChaseAlgorithm {
    // cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390): the vptr at 0x10:
    //   [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm
    //       ...)
    // cSplineCamAlgorithm's notes on cLookAtChaseAlgorithm's fields:
    //   View: [R] ctor: pi/4
    //   AlgorithmType: [R] 0x54; subclass ctors store their own
    //   CamTargetPos: [U] sq/lqc2 in the update functions
    //   Target: [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    // --- cSplineCamAlgorithm
    virtual float getRestDrop();                                                    // 13 ([R] cSplineCamAlgorithm_vtbl: cSplineCamAlgorithm_getRestDrop__FPv)
    char XOfT[0xC];                 // 0x390 [O] cCubicSplineInterpolant (On Tour 0x380); [R] ctor passes this+0x390
                                    //       to cCubicSplineInterpolant_cCubicSplineInterpolant/cCubicSplineInterpolant_evaluate, dtor to cCubicSplineInterpolant__cCubicSplineInterpolant
    char YOfT[0xC];                 // 0x39C [O] cCubicSplineInterpolant; [R] ditto (this+0x39C)
    char ZOfT[0xC];                 // 0x3A8 [O] cCubicSplineInterpolant; [R] ditto (this+0x3A8)
    float TotalTime;                // 0x3B4 [O]; [R] swc1; [U] cSplineCamAlgorithm_update (lwc1)
    float LastEvaluatedTime;        // 0x3B8 [O]; [R] swc1; [U] cSplineCamAlgorithm_update (lwc1/swc1)
    float Speed;                    // 0x3BC [O]; [R] swc1; [U] cSplineCamAlgorithm_update
    float LeadDistance;             // 0x3C0 [O]; [R] = leadDistance; [U] cSplineCamAlgorithm_update
    float HeightOffset;             // 0x3C4 [O]; [R] = heightOffset; [U] cSplineCamAlgorithm_update
    char unk_3C8[0x8];              // 0x3C8 padding to the 16-byte class alignment (alloc 0x3D0)
};

CHECK_OFFSET(cSplineCamAlgorithm, XOfT, 0x390);
CHECK_OFFSET(cSplineCamAlgorithm, YOfT, 0x39C);
CHECK_OFFSET(cSplineCamAlgorithm, ZOfT, 0x3A8);
CHECK_OFFSET(cSplineCamAlgorithm, TotalTime, 0x3B4);
CHECK_OFFSET(cSplineCamAlgorithm, LastEvaluatedTime, 0x3B8);
CHECK_OFFSET(cSplineCamAlgorithm, Speed, 0x3BC);
CHECK_OFFSET(cSplineCamAlgorithm, LeadDistance, 0x3C0);
CHECK_OFFSET(cSplineCamAlgorithm, HeightOffset, 0x3C4);
CHECK_OFFSET(cSplineCamAlgorithm, unk_3C8, 0x3C8);
CHECK_SIZE(cSplineCamAlgorithm, 0x3D0);

// ---------------------------------------------------------------------------------------------------------------
// The field-less subclasses. All six live in retail camera/camera (0x160438-0x168F60) among cMatrix/cRelative-
// ChaseAlgorithm, cChaseCameraController and cLookAtChaseAlgorithm's own methods; in the preview their methods span
// 0x15F8F0-0x1718F0, where the only camera/ assert strings are camera/race/racealgorithms.cpp's (0x1675A8-0x16CC80).
// Each overrides exactly vtable slots 1, 3, 5, 7, 13 (On Tour declares exactly ctor, dtor, snap,
// update, getRestDrop, getRaceCamFinalPhaseData for each), so On Tour's 13-slot vtable maps slot for slot.

// cManualChaseAlgorithm (0x390: no fields of its own) - the rider-steered chase camera ("Manual Chase Camera Algorithm").
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Manual Chase Camera Algorithm") then the ctor cManualChaseAlgorithm_cManualChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0xB and vtable cManualChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cManualChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MANUAL = 11.
// Methods (vtable slot): dtor cManualChaseAlgorithm__cManualChaseAlgorithm__FPv (1), snap cManualChaseAlgorithm_snap (3), update cManualChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cManualChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cManualChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
struct cManualChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cManualChaseAlgorithm_vtbl: cManualChaseAlgorithm_getRestDrop__FPv)
};

CHECK_SIZE(cManualChaseAlgorithm, 0x390);

// cIdleChaseAlgorithm (0x390: no fields of its own) - the idle chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Idle Chase Camera Algorithm") then the ctor cIdleChaseAlgorithm_cIdleChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 7 and vtable cIdleChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cIdleChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_IDLE = 7.
// Methods (vtable slot): dtor cIdleChaseAlgorithm__cIdleChaseAlgorithm__FPv (1), snap cIdleChaseAlgorithm_snap (3), update cIdleChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cIdleChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cIdleChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$19cIdleChaseAlgorithm matches retail cIdleChaseAlgorithm_vtbl (builds/x/v2/vtables_v2.tsv).
struct cIdleChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cIdleChaseAlgorithm_vtbl: cIdleChaseAlgorithm_getRestDrop__FPv)
};

CHECK_SIZE(cIdleChaseAlgorithm, 0x390);

// cLazyChaseAlgorithm (0x390: no fields of its own) - the lazy chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Lazy Chase Camera Algorithm") then the ctor cLazyChaseAlgorithm_cLazyChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0xD and vtable cLazyChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cLazyChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_LAZY = 13.
// Methods (vtable slot): dtor cLazyChaseAlgorithm__cLazyChaseAlgorithm__FPv (1), snap cLazyChaseAlgorithm_snap (3), update cLazyChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cLazyChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cLazyChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$19cLazyChaseAlgorithm matches retail cLazyChaseAlgorithm_vtbl.
struct cLazyChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cLazyChaseAlgorithm_vtbl: cLazyChaseAlgorithm_getRestDrop__FPv)
};

CHECK_SIZE(cLazyChaseAlgorithm, 0x390);

// cLookBackChaseAlgorithm (0x390: no fields of its own) - the look-back chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Look Back Chase Camera Algorithm") then the ctor cLookBackChaseAlgorithm_cLookBackChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0x20 and vtable cLookBackChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cLookBackChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_LOOKBACK = 32.
// Methods (vtable slot): dtor cLookBackChaseAlgorithm__cLookBackChaseAlgorithm__FPv (1), snap cLookBackChaseAlgorithm_snap (3), update cLookBackChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cLookBackChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cLookBackChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$23cLookBackChaseAlgorithm matches retail cLookBackChaseAlgorithm_vtbl.
struct cLookBackChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cLookBackChaseAlgorithm_vtbl: cLookBackChaseAlgorithm_getRestDrop__FPv)
};

CHECK_SIZE(cLookBackChaseAlgorithm, 0x390);

// cMidRightChaseAlgorithm (0x390: no fields of its own) - the mid-distance chase camera shifted to the rider's right.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Mid Right Chase Camera Algorithm") then the ctor cMidRightChaseAlgorithm_cMidRightChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 9 and vtable cMidRightChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cMidRightChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MID_RIGHT = 9.
// Methods (vtable slot): dtor cMidRightChaseAlgorithm__cMidRightChaseAlgorithm__FPv (1), snap cMidRightChaseAlgorithm_snap (3), update cMidRightChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cMidRightChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cMidRightChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// Which mid vtable is Right: (1) [R] the "Mid Right" allocation (createChaseAlgorithmBlend case 9 =
// CAMERA_ALGORITHM_CHASE_MID_RIGHT) calls cMidRightChaseAlgorithm_cMidRightChaseAlgorithm, which stores cMidRightChaseAlgorithm_vtbl; (2) [P] the preview's
// _vt$23cMidRightChaseAlgorithm update passes -0.29444468 to updateLateralShift, as retail cMidRightChaseAlgorithm_update does;
// (3) [O] On Tour's cMidRightChaseAlgorithm::update passes -kLateralShift_MIDSIDE (= 0.29444468, set by
// __sti__all_in_one_camera_cpp). Both mid ctors store type 9 in retail AND in On Tour (EA copy-paste).
struct cMidRightChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cMidRightChaseAlgorithm_vtbl: cMidRightChaseAlgorithm_getRestDrop)
};

CHECK_SIZE(cMidRightChaseAlgorithm, 0x390);

// cMidLeftChaseAlgorithm (0x390: no fields of its own) - the mid-distance chase camera shifted to the rider's left.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Mid Left Chase Camera Algorithm") then the ctor cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 9 (sic: MidRight's; On Tour's ctor also stores 9) and vtable cMidLeftChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cMidLeftChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MID_RIGHT = 9.
// Methods (vtable slot): dtor cMidLeftChaseAlgorithm__cMidLeftChaseAlgorithm__FPv (1), snap cMidLeftChaseAlgorithm_snap (3), update cMidLeftChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cMidLeftChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cMidLeftChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// Left, not Right: the "Mid Left" allocation (case 10 = CAMERA_ALGORITHM_CHASE_MID_LEFT) calls cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm;
// its update cMidLeftChaseAlgorithm_update passes +0.29444468 to updateLateralShift, like the preview's
// _vt$22cMidLeftChaseAlgorithm update and On Tour's cMidLeftChaseAlgorithm::update (+kLateralShift_MIDSIDE).
struct cMidLeftChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cMidLeftChaseAlgorithm_vtbl: cMidLeftChaseAlgorithm_getRestDrop)
};

CHECK_SIZE(cMidLeftChaseAlgorithm, 0x390);


#endif /* CAMERA_RACE_RACEALGORITHMS_H */
