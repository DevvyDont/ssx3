#ifndef AI_AIRPREDICTOR_H
#define AI_AIRPREDICTOR_H

// cAirPredictor: a rider's jump simulator. When the rider launches into the air (cAirPredictor_startLaunchIntoAir), it
// simulates the flight ahead in 1/60 s ticks against the world (cAirPredictor_simStep / cAirPredictor_simOneTick) to
// predict where, when and on what surface the rider lands and where the apex is; the AI, the camera
// (cRiderCameraTarget), sound and the trick code ask it for the air time left (TotalAirTime - CurAirTime) once SimState
// is 1 or 3 (On Tour isSimDone).
// EA's source is d:/ssx/code/source/ai/airpredictor.cpp (July 2003 preview assert strings); retail's methods are in
// ai/airpredictor.cpp (0x113198-0x113AD8), the ctor/dtor in ai/computer.cpp (0x113138, 0x113170).
// Size 0xB0 [R]: cRider_loadRider allocates it with cMemMan_alloc(0xB0, "AirPredictor") + cAirPredictor_cAirPredictor and
// stores it in cRider::mAirPredictor (ai/rider.h, +0x788); cAirPredictor_writeToReplayFrame / readFromReplayFrame copy
// all 0xB0 bytes. No vtable (On Tour: no virtuals; the replay calls are on the frame object).
// Base class: On Tour's cAirPredictor (0xB0, "2 variants") has no members of its own; its data is the struct
// cAirPredictorState (0xB0), written out here as the whole class.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail ctor cAirPredictor_cAirPredictor (+0x30 = -1, then reset), cAirPredictor_reset, the allocation size;
//   [U] how retail code uses the field (the methods; ai/ai.cpp, ai/rider.cpp, ai/control/handplantcontrol.cpp,
//       seg/seg_1218.cpp, seg/seg_14B10.cpp, sound/soundsys.cpp, camera/camera.cpp read SimState/TotalAirTime/CurAirTime);
//   [O] SSX On Tour's DWARF (cAirPredictorState, 0xB0): every member at the same offset as retail's accesses.
// No preview evidence: tools/layout_xlate.py pairs no cAirPredictor method.

#include "layout.h"
#include "bx/math3d.h"

struct cBxReplayFrame;

struct t4Vector;

struct cAirPredictor {
    t4Vector GuessLandPosition;     // 0x00 [O]; [U] simStep stores the hit point (sq); updateFlightPath stores t4vect_Origin
                                    //      when it gives up (state 3); reset leaves it
    t4Vector GuessLandVelocity;     // 0x10 [O]; [R] reset: t4vect_Zero; [U] simStep (sq); handplantcontrol reads it
    t4Vector GuessLandNormal;       // 0x20 [O]; [R] reset: t4vect_ZAxis; [U] simStep (sq); handplantcontrol aligns the rider to it
    unsigned int GuessLandPatchRID; // 0x30 [O] tResourceID; [R] ctor and reset store -1; [U] simStep stores the hit patch
    float GuessLandPatchUV[2];      // 0x34 [O] t2Vector; [R] reset copies 8 bytes from t2vect_Zero (sdl/sdr); [U] simStep
                                    //      stores the hit u/v (swc1 0x34, 0x38)
    char unk_3C[0x4];               // 0x3C alignment gap before the 16-byte-aligned t4Vector
    t4Vector GuessApexPos;          // 0x40 [O]; [U] simStep stores the sim position while the height keeps rising
    t4Vector CurSimPos;             // 0x50 [O]; [U] initLaunch = launchPos; simStep steps it (&+0x50 to simOneTick, lq/sq, lwc1 .z)
    t4Vector CurSimVel;             // 0x60 [O]; [U] initLaunch = launchVel; simStep steps it
    t4Vector PrevTickPos;           // 0x70 [O]; [U] initLaunch = launchPos; updateFlightPath steps it in 1/60 s ticks
    t4Vector PrevTickVel;           // 0x80 [O]; [U] initLaunch = launchVel; updateFlightPath steps it
    int GuessLandSurface;           // 0x90 [O] eSurfaceType; [R] reset -1; [U] simStep stores the hit surface;
                                    //      updateFlightPath 13 on give-up; cRider_isChangeFakieAir indexes the surface cache by it
    short SurfaceFlags;             // 0x94 [O]; [U] simStep (sh)
    char unk_96[0x2];               // 0x96 padding
    float TotalAirTime;             // 0x98 [O]; [R] reset 0; [U] simStep advances it, initLaunch = CurAirTime, land zeroes it;
                                    //      the AI/sound/camera read TotalAirTime - CurAirTime
    float ApexTime;                 // 0x9C [O]; [R] reset 0; [U] simStep records the apex time; camera/camera.cpp reads
                                    //      ApexTime - CurAirTime
    float CurAirTime;               // 0xA0 [O]; [R] reset 0; [U] updateFlightPath advances it by dt
    float PrevTickTime;             // 0xA4 [O]; [R] reset 0; [U] updateFlightPath counts the 1/60 s ticks done
    float TerminalVel;              // 0xA8 [O]; [U] startLaunchIntoAir stores its terminalVel argument; simStep / simOneTick read it
    int SimState;                   // 0xAC [O] eSimState (0 simulating, 1 landed, 2 no landing yet, 3 gave up); [R] reset 0;
                                    //      [U] initLaunch 0, simStep / updateFlightPath set 1/2/3; read all over (== 1 || == 3)

    // Methods bound to their retail symbols by asm labels.
    void reset() __asm__("cAirPredictor_reset");
    void land() __asm__("cAirPredictor_land__FPv");
    void readFromReplayFrame(cBxReplayFrame* obj) __asm__("cAirPredictor_readFromReplayFrame");
    void simStep() __asm__("cAirPredictor_simStep");
    float simOneTick(t4Vector* pos, t4Vector* vel, t4Vector* outPos, t4Vector* outVel)
        __asm__("cAirPredictor_simOneTick");
    void updateFlightPath(t4Vector* airPos, t4Vector* airVel, float dt) __asm__("cAirPredictor_updateFlightPath");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cAirPredictor_writeToReplayFrame");
};

CHECK_OFFSET(cAirPredictor, GuessLandPosition, 0x00);
CHECK_OFFSET(cAirPredictor, GuessLandVelocity, 0x10);
CHECK_OFFSET(cAirPredictor, GuessLandNormal, 0x20);
CHECK_OFFSET(cAirPredictor, GuessLandPatchRID, 0x30);
CHECK_OFFSET(cAirPredictor, GuessLandPatchUV, 0x34);
CHECK_OFFSET(cAirPredictor, unk_3C, 0x3C);
CHECK_OFFSET(cAirPredictor, GuessApexPos, 0x40);
CHECK_OFFSET(cAirPredictor, CurSimPos, 0x50);
CHECK_OFFSET(cAirPredictor, CurSimVel, 0x60);
CHECK_OFFSET(cAirPredictor, PrevTickPos, 0x70);
CHECK_OFFSET(cAirPredictor, PrevTickVel, 0x80);
CHECK_OFFSET(cAirPredictor, GuessLandSurface, 0x90);
CHECK_OFFSET(cAirPredictor, SurfaceFlags, 0x94);
CHECK_OFFSET(cAirPredictor, unk_96, 0x96);
CHECK_OFFSET(cAirPredictor, TotalAirTime, 0x98);
CHECK_OFFSET(cAirPredictor, ApexTime, 0x9C);
CHECK_OFFSET(cAirPredictor, CurAirTime, 0xA0);
CHECK_OFFSET(cAirPredictor, PrevTickTime, 0xA4);
CHECK_OFFSET(cAirPredictor, TerminalVel, 0xA8);
CHECK_OFFSET(cAirPredictor, SimState, 0xAC);
CHECK_SIZE(cAirPredictor, 0xB0);

#endif /* AI_AIRPREDICTOR_H */
