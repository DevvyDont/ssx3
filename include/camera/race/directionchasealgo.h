#ifndef CAMERA_RACE_DIRECTIONCHASEALGO_H
#define CAMERA_RACE_DIRECTIONCHASEALGO_H

// The DEFAULT_2/3/4 and Direction chase-camera algorithms: subclasses of cLookAtChaseAlgorithm
// (camera/cameraalgorithms.h, 0x390). EA file [P]: d:/ssx/code/source/camera/race/directionchasealgo.cpp, whose assert
// sits inside the preview's cDirectionChaseAlgorithm::update (0x184654); in retail all four classes share one
// compile unit (retail 0x176A68-0x177E30 inside the project's camera/trigger/cameratriggerfactory segment: one
// debug-print stub func_00177610 and one static initializer func_00177650 close it).
//   cDEFAULT_2ChaseAlgorithm  (0x390)  "DEFAULT_2 Chase Camera Algorithm"  camera types 2 (NEAR) and 60
//   cDEFAULT_3ChaseAlgorithm  (0x390)  "DEFAULT_3 Chase Camera Algorithm"  the default case; cCamera embeds one at 0xC0
//   cDEFAULT_4ChaseAlgorithm  (0x390)  "DEFAULT_4 Chase Camera Algorithm"  camera types 1 (FAR) and 62
//   cDirectionChaseAlgorithm  (0x3A0)  "Direction <N..NE> Chase Camera Algorithm"  camera types 18..25
// Sizes and tags [R]: cChaseCameraController_createChaseAlgorithmBlend (camera/cameraalgorithms.cpp) allocates them
// with cMemMan_alloc(size, tag) and calls their ctors. Retail vtables (13 slots: dtor, getPose, snap, snapCurrent,
// update, getTargetPos, getRaceCamFinalPhaseData, doRaceCamFinalPhase, updateAlgorithm, notifyMoveZ,
// shouldSnapOnUpdate, setTarget, getRestDrop; each class overrides slots 1, 3, 5, 7, 13, as On Tour's do).
//
// Each derives from cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390, its fields' evidence there): with no
// virtual functions declared in C the layout is the base's 0x390 bytes, then the class's own members.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"    // tPose via bx/math3d.h, cPIDFilter

struct cRiderCameraTarget;

// cDEFAULT_2ChaseAlgorithm (0x390: no fields of its own) - a tuned default chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "DEFAULT_2 Chase Camera Algorithm") then the ctor cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0x3C and vtable cDEFAULT_2ChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cDEFAULT_2ChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_DEFAULT_2 = 60.
// Methods (vtable slot): dtor cDEFAULT_2ChaseAlgorithm__cDEFAULT_2ChaseAlgorithm__FPv (1), snap cDEFAULT_2ChaseAlgorithm_snap (3), update cDEFAULT_2ChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cDEFAULT_2ChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cDEFAULT_2ChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// [P] the preview's _vt$24cDEFAULT_2ChaseAlgorithm matches retail cDEFAULT_2ChaseAlgorithm_vtbl.
struct cDEFAULT_2ChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cDEFAULT_2ChaseAlgorithm_vtbl: cDEFAULT_2ChaseAlgorithm_getRestDrop)

    // Methods bound to their retail symbols by asm labels.
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cDEFAULT_2ChaseAlgorithm_getRaceCamFinalPhaseData");
    virtual void update() __asm__("cDEFAULT_2ChaseAlgorithm_update");
    virtual void snap(int id) __asm__("cDEFAULT_2ChaseAlgorithm_snap");
};

CHECK_SIZE(cDEFAULT_2ChaseAlgorithm, 0x390);

// cDEFAULT_3ChaseAlgorithm (0x390: no fields of its own) - the default chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "DEFAULT_3 Chase Camera Algorithm") then the ctor cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0x3D and vtable cDEFAULT_3ChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cDEFAULT_3ChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_DEFAULT_3 = 61.
// Methods (vtable slot): dtor cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv (1), snap cDEFAULT_3ChaseAlgorithm_snap (3), update cDEFAULT_3ChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cDEFAULT_3ChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cDEFAULT_3ChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// Also [R] cCamera_cCamera and cSimpleCamera_setupCamWithPosAndTarget construct one in place at cCamera+0xC0 (On Tour: cCamera::
// ResultAlgorithm is a cDEFAULT_3ChaseAlgorithm at 0xC0). builds/x/v2 pairs cDEFAULT_3ChaseAlgorithm_vtbl with the preview's
// _vt$18cMidChaseAlgorithm by body similarity only; the retail tag, type 61 and On Tour say DEFAULT_3.
struct cDEFAULT_3ChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cDEFAULT_3ChaseAlgorithm_vtbl: cDEFAULT_3ChaseAlgorithm_getRestDrop)

    // Methods bound to their retail symbols by asm labels.
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cDEFAULT_3ChaseAlgorithm_getRaceCamFinalPhaseData");
    virtual void update() __asm__("cDEFAULT_3ChaseAlgorithm_update");
    virtual void snap(int id) __asm__("cDEFAULT_3ChaseAlgorithm_snap");
};

CHECK_SIZE(cDEFAULT_3ChaseAlgorithm, 0x390);

// cDEFAULT_4ChaseAlgorithm (0x390: no fields of its own) - a tuned default chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "DEFAULT_4 Chase Camera Algorithm") then the ctor cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0x3E and vtable cDEFAULT_4ChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cDEFAULT_4ChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_DEFAULT_4 = 62.
// Methods (vtable slot): dtor cDEFAULT_4ChaseAlgorithm__cDEFAULT_4ChaseAlgorithm__FPv (1), snap cDEFAULT_4ChaseAlgorithm_snap (3), update cDEFAULT_4ChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cDEFAULT_4ChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cDEFAULT_4ChaseAlgorithm_getRestDrop (13); the other slots are the base's.
struct cDEFAULT_4ChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
    virtual float getRestDrop();                                                    // 13 ([R] cDEFAULT_4ChaseAlgorithm_vtbl: cDEFAULT_4ChaseAlgorithm_getRestDrop)

    // Methods bound to their retail symbols by asm labels.
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cDEFAULT_4ChaseAlgorithm_getRaceCamFinalPhaseData");
    virtual void update() __asm__("cDEFAULT_4ChaseAlgorithm_update");
    virtual void snap(int id) __asm__("cDEFAULT_4ChaseAlgorithm_snap");
};

CHECK_SIZE(cDEFAULT_4ChaseAlgorithm, 0x390);

// cDirectionChaseAlgorithm (0x3A0; On Tour 0x390) - a chase camera looking along a fixed compass direction.
// [R] createChaseAlgorithmBlend cases 18..25: cMemMan_alloc(0x3A0, "Direction N/NW/W/SW/S/SE/E/NE Chase Camera
// Algorithm") then the ctor cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(this, dir), which stores AlgorithmType = dir + 18 and Angle = dir * pi/4.
// [O] On Tour: cDirectionChaseAlgorithm(eCamDirection dir), member Angle at 0x380 (retail 0x390).
// Methods (vtable slot): dtor cDirectionChaseAlgorithm__cDirectionChaseAlgorithm__FPv (1), snap cDirectionChaseAlgorithm_snap (3), update cDirectionChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cDirectionChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cDirectionChaseAlgorithm_getRestDrop__FPv (13).
struct cDirectionChaseAlgorithm : public cLookAtChaseAlgorithm {
    virtual float getRestDrop();                                                    // 13 ([R] cDirectionChaseAlgorithm_vtbl: cDirectionChaseAlgorithm_getRestDrop__FPv)
    float Angle;                    // 0x390 [O] (On Tour 0x380); [R] ctor = dir * pi/4 (swc1); [U] update
                                    //       cDirectionChaseAlgorithm_update passes it to cLookAtChaseAlgorithm_updateRotation (lwc1)
    char unk_394[0xC];              // 0x394 padding to the 16-byte class alignment (alloc 0x3A0)

    // Methods bound to their retail symbols by asm labels.
    virtual void update() __asm__("cDirectionChaseAlgorithm_update");
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cDirectionChaseAlgorithm_getRaceCamFinalPhaseData");
    virtual void snap(int id) __asm__("cDirectionChaseAlgorithm_snap");
};

CHECK_OFFSET(cDirectionChaseAlgorithm, Angle, 0x390);
CHECK_OFFSET(cDirectionChaseAlgorithm, unk_394, 0x394);
CHECK_SIZE(cDirectionChaseAlgorithm, 0x3A0);

#endif /* CAMERA_RACE_DIRECTIONCHASEALGO_H */
