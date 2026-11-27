#ifndef CAMERA_RACE_ALLSPINCHASEALGO_H
#define CAMERA_RACE_ALLSPINCHASEALGO_H

// The SPOKE, SUPERPIPE and POST_RACE_1 chase-camera algorithms: subclasses of cLookAtChaseAlgorithm
// (camera/cameraalgorithms.h, 0x390). EA file [P]: d:/ssx/code/source/camera/race/allspinchasealgo.cpp, whose assert
// (line 644) sits in the preview function 0x185E78 between cSPOKEChaseAlgorithm's snap and cSUPERPIPEChaseAlgorithm's
// dtor (it reads the SUPERPIPE points); in retail the three classes share one compile unit (0x177E50-0x178F58 inside
// the project's camera/trigger/cameratriggerfactory segment, closed by stub func_00178F18 and static init
// func_00178F58).
//   cSPOKEChaseAlgorithm        (0x3A0)  "SPOKE Chase Camera Algorithm"        camera type 66
//   cSUPERPIPEChaseAlgorithm    (0x3B0)  "SUPERPIPE Chase Camera Algorithm"    the superpipe case of the factory
//   cPOST_RACE_1ChaseAlgorithm  (0x3B0)  "POST_RACE_1 Chase Camera Algorithm"  camera type 68
// Sizes and tags [R]: cChaseCameraController_createChaseAlgorithmBlend (camera/cameraalgorithms.cpp). Each class
// overrides vtable slots 1, 3, 5, 7, 13 (dtor, snap, update, getRaceCamFinalPhaseData, getRestDrop), as On Tour's do.
//
// Each derives from cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390, its fields' evidence there): with no
// virtual functions declared in C the layout is the base's 0x390 bytes, then the class's own members. Own members: On Tour's
// names and order (On Tour's base is 0x380, so each sits 0x10 later in retail), placed by [R] the constructors and
// [U] the methods.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"    // tPose via bx/math3d.h, cPIDFilter

struct cRiderCameraTarget;

// cSPOKEChaseAlgorithm (0x3A0; On Tour 0x390). [R] cMemMan_alloc(0x3A0, "SPOKE Chase Camera Algorithm") then the ctor
// cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm (type 0x42 = CAMERA_ALGORITHM_CHASE_SPOKE = 66, vtable cSPOKEChaseAlgorithm_vtbl D_0045CCC8).
// [P] the preview's _vt$20cSPOKEChaseAlgorithm matches D_0045CCC8. Methods: dtor
// cSPOKEChaseAlgorithm__cSPOKEChaseAlgorithm__FPv (1), snap cSPOKEChaseAlgorithm_snap (3), update
// cSPOKEChaseAlgorithm_update (5), getRaceCamFinalPhaseData cSPOKEChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cSPOKEChaseAlgorithm_getRestDrop (13);
// non-virtual getRiderPosition cSPOKEChaseAlgorithm_getRiderPosition and getDesiredPosition(const t4Vector&) cSPOKEChaseAlgorithm_getDesiredPosition (both return a
// t4Vector through a hidden pointer: a0 = result, a1 = this).
struct cSPOKEChaseAlgorithm : public cLookAtChaseAlgorithm {
    virtual float getRestDrop();                                                    // 13 ([R] cSPOKEChaseAlgorithm_vtbl: cSPOKEChaseAlgorithm_getRestDrop)
    t4Vector LastLookDir;           // 0x390 [O] (On Tour 0x380); [U] snap cSPOKEChaseAlgorithm_snap (sq), getDesiredPosition
                                    //       cSPOKEChaseAlgorithm_getDesiredPosition (lqc2, sq)

    // Methods bound to their retail symbols by asm labels.
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cSPOKEChaseAlgorithm_getRaceCamFinalPhaseData");

    // Methods bound to their retail symbols by asm labels.
    virtual void snap(int id) __asm__("cSPOKEChaseAlgorithm_snap");
    virtual void update() __asm__("cSPOKEChaseAlgorithm_update");
};

CHECK_OFFSET(cSPOKEChaseAlgorithm, LastLookDir, 0x390);
CHECK_SIZE(cSPOKEChaseAlgorithm, 0x3A0);

// cSUPERPIPEChaseAlgorithm (0x3B0; On Tour 0x3A0). [R] cMemMan_alloc(0x3B0, "SUPERPIPE Chase Camera Algorithm") then
// the ctor cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(this, point1, point2) (type 0x43 = CAMERA_ALGORITHM_CHASE_SUPERPIPE = 67, vtable cSUPERPIPEChaseAlgorithm_vtbl);
// the factory builds both points on the stack (cCTBoundObjLine_worldPoint1, cCTBoundObjLine_worldPoint2) from the superpipe it is given.
// [P] the preview's _vt$24cSUPERPIPEChaseAlgorithm matches cSUPERPIPEChaseAlgorithm_vtbl. Methods: dtor cSUPERPIPEChaseAlgorithm__cSUPERPIPEChaseAlgorithm__FPv (1), snap
// cSUPERPIPEChaseAlgorithm_snap (3), update cSUPERPIPEChaseAlgorithm_update (5), getRaceCamFinalPhaseData cSUPERPIPEChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cSUPERPIPEChaseAlgorithm_getRestDrop
// (13); non-virtual getIdealPosition cSUPERPIPEChaseAlgorithm_getIdealPosition and getIdealTargetPos cSUPERPIPEChaseAlgorithm_getIdealTargetPos (a0 = result, a1 = this).
struct cSUPERPIPEChaseAlgorithm : public cLookAtChaseAlgorithm {
    virtual float getRestDrop();                                                    // 13 ([R] cSUPERPIPEChaseAlgorithm_vtbl: cSUPERPIPEChaseAlgorithm_getRestDrop)
    t4Vector Point1;                // 0x390 [O] (On Tour 0x380); [R] ctor = *point1 (lq/sq); [U] getIdealPosition
                                    //       cSUPERPIPEChaseAlgorithm_getIdealPosition (lqc2)
    t4Vector Point2;                // 0x3A0 [O] (On Tour 0x390); [R] ctor = *point2 (lq/sq); [U] getIdealPosition (lqc2)

    // Methods bound to their retail symbols by asm labels.
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* out)
        __asm__("cSUPERPIPEChaseAlgorithm_getRaceCamFinalPhaseData");
    virtual void snap(int id) __asm__("cSUPERPIPEChaseAlgorithm_snap");
    virtual void update() __asm__("cSUPERPIPEChaseAlgorithm_update");
};

CHECK_OFFSET(cSUPERPIPEChaseAlgorithm, Point1, 0x390);
CHECK_OFFSET(cSUPERPIPEChaseAlgorithm, Point2, 0x3A0);
CHECK_SIZE(cSUPERPIPEChaseAlgorithm, 0x3B0);

// cPOST_RACE_1ChaseAlgorithm (0x3B0; On Tour 0x3A0). [R] cMemMan_alloc(0x3B0, "POST_RACE_1 Chase Camera Algorithm")
// then the ctor cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(this, target) (type 0x44 = CAMERA_ALGORITHM_CHASE_POST_RACE_1 = 68, vtable cPOST_RACE_1ChaseAlgorithm_vtbl).
// [O] On Tour: cPOST_RACE_1ChaseAlgorithm(cRiderCameraTarget* target). Methods: dtor cPOST_RACE_1ChaseAlgorithm__cPOST_RACE_1ChaseAlgorithm__FPv (1), snap
// cPOST_RACE_1ChaseAlgorithm_snap (3), update cPOST_RACE_1ChaseAlgorithm_update (5), getRaceCamFinalPhaseData cPOST_RACE_1ChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop
// cPOST_RACE_1ChaseAlgorithm_getRestDrop__FPv (13).
struct cPOST_RACE_1ChaseAlgorithm : public cLookAtChaseAlgorithm {
    virtual float getRestDrop();                                                    // 13 ([R] cPOST_RACE_1ChaseAlgorithm_vtbl: cPOST_RACE_1ChaseAlgorithm_getRestDrop__FPv)
    t4Vector StartDir;              // 0x390 [O] (On Tour 0x380); [R] ctor: the target's direction, normalised (sq),
                                    //       .z = 0 (sw), renormalised (lqc2/sqc2); [U] update cPOST_RACE_1ChaseAlgorithm_update (lqc2)
    int AgeInFrames;                // 0x3A0 [O] (On Tour 0x390); [R] ctor = 0 (sw); [U] update cPOST_RACE_1ChaseAlgorithm_update (lw/sw)
    char unk_3A4[0xC];              // 0x3A4 padding to the 16-byte class alignment (alloc 0x3B0)

    // Methods bound to their retail symbols by asm labels.
    virtual void update() __asm__("cPOST_RACE_1ChaseAlgorithm_update");
    virtual void snap(int id) __asm__("cPOST_RACE_1ChaseAlgorithm_snap");
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* other)
        __asm__("cPOST_RACE_1ChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0");
};

CHECK_OFFSET(cPOST_RACE_1ChaseAlgorithm, StartDir, 0x390);
CHECK_OFFSET(cPOST_RACE_1ChaseAlgorithm, AgeInFrames, 0x3A0);
CHECK_OFFSET(cPOST_RACE_1ChaseAlgorithm, unk_3A4, 0x3A4);
CHECK_SIZE(cPOST_RACE_1ChaseAlgorithm, 0x3B0);

#endif /* CAMERA_RACE_ALLSPINCHASEALGO_H */
