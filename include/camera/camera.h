#ifndef CAMERA_CAMERA_H
#define CAMERA_CAMERA_H

// cSimpleCamera and cCamera (EA: d:/ssx/code/source/camera/camera.h and camera.cpp, from the July 2003 preview's
// assert strings; camera.h asserts "maxFar>MinNear" / "minNear<MaxFar" are cSimpleCamera's setters).
//
// cSimpleCamera (0xA0): a view (FOV, near/far planes and their limits), a pose and the camera matrix. Retail vtable
// cSimpleCamera_vtbl (D_0045B8B0), stored by its ctor func_0015D698 and dtor cSimpleCamera__cSimpleCamera. It is also
// the base of the front end's cFECamera (fe/festatecharequip.cpp constructs one at +0x10 of its state).
//
// cCamera (0x4B0): one rider's game camera: cSimpleCamera + three controllers (manual, chase, script; the current
// one drives it), the stream focus it owns, the result algorithm the controllers blend into, camera shake and ground
// avoidance. Created by cGameViewMan_cGameViewMan (main/gamerender.cpp) with cMemMan_alloc(0x4B0, "GameCamera") +
// cCamera_cCamera(cam, viewIndex), one per view, stored at viewman + 4 + 4 * i. Retail vtable cCamera_vtbl
// (D_0045B888; slots: deleting dtor func_0015DD88, init = cCamera_init, update = func_0015DF98). Methods in
// camera/camera.cpp 0x15DC18-0x15F568 and accessors 0x168110-0x168138.
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors cCamera_cCamera, func_0015D698 (cSimpleCamera ctor), cCamera_init, and the allocation;
//   [P] the preview's inline accessors via tools/layout_xlate.py (cSimpleCamera getFOV/getNear/getFar/setMaxFOV at
//       0x0/0x4/0x8/0xC; cCamera getCurrentController/get*Controller/isStreamReady, preview 0xC0..0xD4 = retail
//       0xA0..0xB4: the preview's cSimpleCamera was 0x20 bigger);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cSimpleCamera 0xA0, cCamera 0x490): same members in the same order. Retail's
//       ResultAlgorithm is 0x10 bigger (0x390) and its bools are words, so cCamera's members after 0xC0 sit
//       0x10..0x20 later than On Tour's.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"    // cLookAtChaseAlgorithm

struct cCameraController;
struct cManualCameraController;
struct cChaseCameraController;
struct cScriptCameraController;
struct cRider;

// cRiderCameraTarget (0x8): what the chase algorithms follow: a cCameraTarget (On Tour: 0x4, only the vptr) bound to a
// rider. Created by cCamera_init: operator new(8, "RiderCameraTarget") + vtable D_0045B7A8 (26 slots, all in
// camera/camera.cpp 0x15F568-0x15F938: getPosition, getRotation, getVelocity ... each reads Rider), then handed to
// the chase controller (func_00161A60) and the result algorithm (func_00162310).
// NOTE: its t4Vector-returning slots (func_0015F568 .. func_0015F668, except func_0015F640) take the hidden return
// slot in a0 and `this` in a1.
struct cRiderCameraTarget {
    void** mVtbl;               // 0x0 [R] D_0045B7A8 (cCamera_init); cCameraTarget declares the virtuals
    cRider* Rider;              // 0x4 [O]; [P] getRider (lw) / setRider (sw); [R] cCamera_init stores the view's rider;
                                //     [U] all 26 slots lw 0x4; func_00168140 setRider, func_00168148 getRider
};

CHECK_OFFSET(cRiderCameraTarget, mVtbl, 0x0);
CHECK_OFFSET(cRiderCameraTarget, Rider, 0x4);
CHECK_SIZE(cRiderCameraTarget, 0x8);

struct cSimpleCamera {
    float View;                 // 0x00 [O]; [P] getFOV (lwc1); [U] func_00168080 getFOV, func_001680F0 setFOV
    float Near;                 // 0x04 [O]; [P] getNear; [U] func_00168088
    float Far;                  // 0x08 [O]; [P] getFar; [U] func_00168090
    float MaxView;              // 0x0C [O]; [P] setMaxFOV (swc1); [U] func_00168108; cCamera_init stores its arg
    float MinNear;              // 0x10 [O]; [U] func_00168100 (swc1); cCamera_init
    float MaxFar;               // 0x14 [O]; [U] func_001680F8 (swc1); cCamera_init
    int ViewIndex;              // 0x18 [O]; [U] cCamera_init stores the view index; func_0015E668 reads it
    char unk_1C[0x4];           // 0x1C alignment gap before the 16-byte-aligned tPose
    tPose Pose;                 // 0x20 [O]; [R] ctor: P = 0, Q = (0, 1, 0, 1) on the stack, sq 0x20/0x30;
                                //      [U] func_001680A0/func_001680A8 return this + 0x20, func_001680B8 (sq 0x20),
                                //      func_001680C8 (sq 0x30), func_001680D8 (both)
    t4Matrix CamMat;            // 0x40 [O]; [U] func_00168098 returns this + 0x40; func_0015E050 writes its rows
    t4Vector Direction;         // 0x80 [O]; [R] = D_004FF140; [U] func_001680B0 returns this + 0x80
    void** mVtbl;               // 0x90 [R] cSimpleCamera_vtbl (D_0045B8B0)
    char unk_94[0xC];           // 0x94 padding to the 16-byte class alignment
};

CHECK_OFFSET(cSimpleCamera, View, 0x00);
CHECK_OFFSET(cSimpleCamera, Near, 0x04);
CHECK_OFFSET(cSimpleCamera, Far, 0x08);
CHECK_OFFSET(cSimpleCamera, MaxView, 0x0C);
CHECK_OFFSET(cSimpleCamera, MinNear, 0x10);
CHECK_OFFSET(cSimpleCamera, MaxFar, 0x14);
CHECK_OFFSET(cSimpleCamera, ViewIndex, 0x18);
CHECK_OFFSET(cSimpleCamera, Pose, 0x20);
CHECK_OFFSET(cSimpleCamera, CamMat, 0x40);
CHECK_OFFSET(cSimpleCamera, Direction, 0x80);
CHECK_OFFSET(cSimpleCamera, mVtbl, 0x90);
CHECK_SIZE(cSimpleCamera, 0xA0);

struct cCamera {
    // --- cSimpleCamera
    float View;                     // 0x000 [O]; [U] func_0015E668
    float Near;                     // 0x004 [O]; [U] func_0015E668
    float Far;                      // 0x008 [O]; [U] func_0015E668, func_0015EC98
    float MaxView;                  // 0x00C [O]; [U] cCamera_init stores its arg
    float MinNear;                  // 0x010 [O]; [U] cCamera_init
    float MaxFar;                   // 0x014 [O]; [U] cCamera_init
    int ViewIndex;                  // 0x018 [O]; [U] cCamera_init stores the view index
    char unk_01C[0x4];              // 0x01C alignment gap
    tPose Pose;                     // 0x020 [O]; [U] func_0015E050, func_0015E668 (sq, .x/.y/.z by lwc1)
    t4Matrix CamMat;                // 0x040 [O]; [U] func_0015E050 (updateCamMat: sqc2 rows)
    t4Vector Direction;             // 0x080 [O]; [U] func_0015E050, func_0015E668 (sq)
    void** mVtbl;                   // 0x090 [R] cCamera_vtbl (D_0045B888); the dtor restores cSimpleCamera_vtbl
    char unk_094[0xC];              // 0x094 padding
    // --- cCamera
    cCameraController* Controller;  // 0x0A0 [O]; [P] getCurrentController; [U] func_0015DFD8 (setCurrentController)
                                    //       copies one of the three below, func_0015DF98 (update) calls its slot 2
    cManualCameraController* ManualController; // 0x0A4 [O]; [P] getManualController; [R] "Manual Cam Controller" (0x20)
    cChaseCameraController* ChaseController;   // 0x0A8 [O]; [P] getChaseController; [R] "Chase Cam Controller" (0x44)
    cScriptCameraController* ScriptController; // 0x0AC [O]; [P] getScriptController; [R] "Script Cam Controller" (0x20)
    int StreamFocusID;              // 0x0B0 [O] eStreamFocusID; [R] -1, then cCamera_init: 0 for view 0, 1 for view 1;
                                    //       [U] func_0015EC98 (updateStreamFocus), func_00168138 getStreamFocusID
    int bStreamIsReady;             // 0x0B4 [O] bool (a word in retail); [P] isStreamReady (lw); [R] 0;
                                    //       [U] func_0015EC98 sets it, func_00168130 returns it
    char unk_0B8[0x8];              // 0x0B8 alignment gap
    cLookAtChaseAlgorithm ResultAlgorithm; // 0x0C0 [O] a cDEFAULT_3ChaseAlgorithm (no fields of its own); [R] constructed
                                    //       by func_00176D68, destroyed by func_00176DA8__FPv; [U] cCamera_init sets its
                                    //       Target (func_00162310), func_0015E030 snaps it (func_00166F28)
    int DominantCamShakeType;       // 0x450 [O] eCamShakeType; [R] 0; [U] func_0015E360 (shakeCamera) sw,
                                    //       func_0015E460 (applyCameraShake) lw
    float MaxShakeVariance;         // 0x454 [O]; [R] 0; [U] func_0015E360 lwc1/swc1, func_0015E460 lwc1
    int DoingCamShake;              // 0x458 [O] bool; [R] 0; [U] func_0015E360 sw, func_0015E460 lw/sw
    int DoingCamShakeLastFrame;     // 0x45C [O] bool; [R] 0; [U] func_0015E460 lw/sw
    float LastGroundMove;           // 0x460 [O]; [R] 0; [U] func_0015EE00 (avoidGround) lwc1/swc1
    char unk_464[0xC];              // 0x464 alignment gap
    t4Vector LastGroundNormal;      // 0x470 [O]; [R] = D_004FF160; [U] func_0015EE00
    char IntersectionHint[0x10];    // 0x480 [O] tIntHint (intersect: mPatch, mU/mV, mFirstTri, mPrecision); only its
                                    //       address is used (func_0015EE00)
    t4Vector LastSafePosition;      // 0x490 [O]; [R] = D_004FF130; [U] func_0015EE00
    int UsingSafePositionLastFrame; // 0x4A0 [O] bool; [R] 0; [U] func_0015EE00
    int LastSafePositionInvalid;    // 0x4A4 [O] bool; [R] 1; [U] func_0015EE00
    char unk_4A8[0x8];              // 0x4A8 padding to the 16-byte class alignment
};

CHECK_OFFSET(cCamera, View, 0x000);
CHECK_OFFSET(cCamera, Near, 0x004);
CHECK_OFFSET(cCamera, Far, 0x008);
CHECK_OFFSET(cCamera, MaxView, 0x00C);
CHECK_OFFSET(cCamera, MinNear, 0x010);
CHECK_OFFSET(cCamera, MaxFar, 0x014);
CHECK_OFFSET(cCamera, ViewIndex, 0x018);
CHECK_OFFSET(cCamera, Pose, 0x020);
CHECK_OFFSET(cCamera, CamMat, 0x040);
CHECK_OFFSET(cCamera, Direction, 0x080);
CHECK_OFFSET(cCamera, mVtbl, 0x090);
CHECK_OFFSET(cCamera, Controller, 0x0A0);
CHECK_OFFSET(cCamera, ManualController, 0x0A4);
CHECK_OFFSET(cCamera, ChaseController, 0x0A8);
CHECK_OFFSET(cCamera, ScriptController, 0x0AC);
CHECK_OFFSET(cCamera, StreamFocusID, 0x0B0);
CHECK_OFFSET(cCamera, bStreamIsReady, 0x0B4);
CHECK_OFFSET(cCamera, ResultAlgorithm, 0x0C0);
CHECK_OFFSET(cCamera, DominantCamShakeType, 0x450);
CHECK_OFFSET(cCamera, MaxShakeVariance, 0x454);
CHECK_OFFSET(cCamera, DoingCamShake, 0x458);
CHECK_OFFSET(cCamera, DoingCamShakeLastFrame, 0x45C);
CHECK_OFFSET(cCamera, LastGroundMove, 0x460);
CHECK_OFFSET(cCamera, LastGroundNormal, 0x470);
CHECK_OFFSET(cCamera, IntersectionHint, 0x480);
CHECK_OFFSET(cCamera, LastSafePosition, 0x490);
CHECK_OFFSET(cCamera, UsingSafePositionLastFrame, 0x4A0);
CHECK_OFFSET(cCamera, LastSafePositionInvalid, 0x4A4);
CHECK_SIZE(cCamera, 0x4B0);

#endif /* CAMERA_CAMERA_H */
