#ifndef CAMERA_CAMERAJITTERER_H
#define CAMERA_CAMERAJITTERER_H

// cCameraJitterer: random positional and roll jitter for the script cameras (hand-held look). Size 0x30: SSX On Tour's
// DWARF (class cCameraJitterer, 0x30, no base, no virtuals) and retail's constructor func_0015FFF0, which stores every
// word of 0x0..0x2C. Embedded by value at +0x74 of the script camera algorithms in camera/script/scriptcontroller.cpp
// (func_0015FFF0 / func_00160028 / func_00160130 / func_00160228 / func_001603F0 are called on `p + 0x74`).
// Methods (camera/camera.cpp, between cEditorCameraAlgorithm and the race chase algorithms, 0x15FFF0-0x160438):
//   func_0015FFF0 cCameraJitterer(), func_00160028 updatePositionalJitter(float posJitter),
//   func_00160130 updateRollJitter(float rollJitter), func_00160228 getPositionalJitter(float heading, float pitch)
//   (returns a t4Vector: the hidden return slot is a0, `this` a1), func_001603F0 getRollJitter().
// EA's source file is not known (On Tour: all_in_one_camera.cpp); camera/camerajitterer.h is our name.
//
// A FLAT layout, like main/game.h. Member names, types and order are On Tour's; every offset matches retail:
//   [R] the constructor (0x0..0x14 = 0, 0x20/0x24 = 1.05f, 0x28/0x2C = 0.05f);
//   [U] the update / get functions.

#include "layout.h"

struct cCameraJitterer {
    float OldRandomSideJitter;      // 0x00 [O]; [R] = 0 (sw); [U] updatePositionalJitter copies RandomSideJitter here;
                                    //      getPositionalJitter blends from it
    float OldRandomUpJitter;        // 0x04 [O]; [R] = 0; [U] ditto (RandomUpJitter)
    float OldRandomRollJitter;      // 0x08 [O]; [R] = 0; [U] updateRollJitter / getRollJitter
    float RandomSideJitter;         // 0x0C [O]; [R] = 0; [U] updatePositionalJitter picks it (BXrand)
    float RandomUpJitter;           // 0x10 [O]; [R] = 0; [U] ditto
    float RandomRollJitter;         // 0x14 [O]; [R] = 0; [U] updateRollJitter picks it; getRollJitter
    float FocusJitterFactor;        // 0x18 [O]; [U] updatePositionalJitter stores its argument; getPositionalJitter
    float RollJitterFactor;         // 0x1C [O]; [U] updateRollJitter stores its argument; getRollJitter
    float CurrFocusJitterTime;      // 0x20 [O]; [R] 1.05f; [U] updatePositionalJitter advances / resets it
    float CurrRollJitterTime;       // 0x24 [O]; [R] 1.05f; [U] updateRollJitter (one sw zero); getRollJitter
    float FocusJitterTime;          // 0x28 [O]; [R] 0.05f; [U] updatePositionalJitter, getPositionalJitter
    float RollJitterTime;           // 0x2C [O]; [R] 0.05f; [U] updateRollJitter, getRollJitter
};

CHECK_OFFSET(cCameraJitterer, OldRandomSideJitter, 0x00);
CHECK_OFFSET(cCameraJitterer, OldRandomUpJitter, 0x04);
CHECK_OFFSET(cCameraJitterer, OldRandomRollJitter, 0x08);
CHECK_OFFSET(cCameraJitterer, RandomSideJitter, 0x0C);
CHECK_OFFSET(cCameraJitterer, RandomUpJitter, 0x10);
CHECK_OFFSET(cCameraJitterer, RandomRollJitter, 0x14);
CHECK_OFFSET(cCameraJitterer, FocusJitterFactor, 0x18);
CHECK_OFFSET(cCameraJitterer, RollJitterFactor, 0x1C);
CHECK_OFFSET(cCameraJitterer, CurrFocusJitterTime, 0x20);
CHECK_OFFSET(cCameraJitterer, CurrRollJitterTime, 0x24);
CHECK_OFFSET(cCameraJitterer, FocusJitterTime, 0x28);
CHECK_OFFSET(cCameraJitterer, RollJitterTime, 0x2C);
CHECK_SIZE(cCameraJitterer, 0x30);

#endif /* CAMERA_CAMERAJITTERER_H */
