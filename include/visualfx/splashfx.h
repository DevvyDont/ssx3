#ifndef VISUALFX_SPLASHFX_H
#define VISUALFX_SPLASHFX_H
#pragma interface

// cSplashManager: the camera snow-splash effect of one view - snow drops (tDrop) and ice crystals (tCrystal) drawn on
// the screen when the camera moves through falling snow or a rider's impact spray reaches it ("Camera Snow Splash
// Effects"; tuned by the static cSplashToggles, the "SplashToggles" debug menu). A scene node (cObjNode) in a bucket:
// cGame_load builds one per view; cGame_getSplashManager(i) returns it. EA's source:
// d:/ssx/code/source/visualfx/splashfx.cpp (the preview keeps that path next to the "SplashToggles" tag and the
// _vt$14cSplashManager / _vt$14cSplashToggles vtables); retail: splat unit seg/seg_1F1548 (tDrop_tDrop 0x002F2588 ..
// cSplashManager_ReportSnowfall 0x002F4330).
//
// Size 0x1030: [R] cGame_load: cMemMan_alloc(0x1030, "cSplashManager") + cSplashManager_cSplashManager(p, bucket 2,
// view); the ctor builds 30 tDrop of 0x3C from +0x1C and 30 tCrystal of 0x4C from +0x724 (= 0x1C + 30 * 0x3C) and
// zeroes 0x100C..0x1024 (0x724 + 30 * 0x4C = 0x100C). [O] On Tour's is 0x14F0 (16-aligned t4Vector/tARGB in its drops
// and crystals, a 0xC-byte cObjNode); member order is the same.
// Base: cObjNode (object/objnode.h, 0x10). Its vptr at 0xC is the only one: cSplashManager_vtbl
// (D_00488230, 14 slots: 1 dtor, 2 update, 4 render, 14 reset; the rest are cObjNode's).
//
// Derives from cObjNode; its own members FLAT, every field at its retail offset. Tags: [R] retail
// constructors (cSplashManager_cSplashManager, tDrop_tDrop, tCrystal::tCrystal) and the allocation; [U] how retail code
// uses the field; [O] SSX On Tour's DWARF (cSplashManager, tDrop, tCrystal); [P] layout_xlate.py pairs 4 methods
// (dtor, update, render, reset): preview == retail at 0xC, 0x10, 0x14, 0x18, 0x100C, 0x1010, 0x1020.
// On Tour's t2Vector / tARGB members are written as float arrays: retail copies them with ldl/ldr pairs (4-byte
// aligned) and several units define their own t2Vector.

#include "layout.h"
#include "object/objnode.h"
#include "bx/math3d.h"

struct cObjNode;

// A snow drop on the screen (On Tour struct tDrop, 0x50; retail 0x3C: its tARGB is only 4-aligned).
struct tDrop {
    int mActive;                    // 0x000 [O] bool (a word in retail); [R] tDrop_tDrop zeroes it; [U] init / update
    float mPosition[2];             // 0x004 [O] t2Vector; [U] init (ldl/ldr copy), update, render
    float mSize[2];                 // 0x00C [O] t2Vector; [U] init, update, render
    float mVelocity[2];             // 0x014 [O] t2Vector; [U] init, update
    float mAge;                     // 0x01C [O]; [U] init zeroes it, update, render
    float mDeath;                   // 0x020 [O]; [U] init, update, render
    float mColour[4];               // 0x024 [O] tARGB {A, R, G, B}; [U] init, render (ldl/ldr)
    float mRotation;                // 0x034 [O]; [U] init, render
    float mRotationChangeOverLifetime; // 0x038 [O]; [U] init, render
};

CHECK_OFFSET(tDrop, mActive, 0x000);
CHECK_OFFSET(tDrop, mPosition, 0x004);
CHECK_OFFSET(tDrop, mSize, 0x00C);
CHECK_OFFSET(tDrop, mVelocity, 0x014);
CHECK_OFFSET(tDrop, mAge, 0x01C);
CHECK_OFFSET(tDrop, mDeath, 0x020);
CHECK_OFFSET(tDrop, mColour, 0x024);
CHECK_OFFSET(tDrop, mRotation, 0x034);
CHECK_OFFSET(tDrop, mRotationChangeOverLifetime, 0x038);
CHECK_SIZE(tDrop, 0x03C);

// An ice crystal on the screen; it sheds drops (On Tour struct tCrystal, 0x60; retail 0x4C).
struct tCrystal {
    int mNumSpawned;                // 0x000 [O]; [U] init zeroes it; cSplashManager_update counts the drops it sheds
    int mActive;                    // 0x004 [O] bool (a word); [R] tCrystal::tCrystal zeroes it; [U] init, update
    float mPosition[2];             // 0x008 [O] t2Vector; [U] init, update, render
    float mSize[2];                 // 0x010 [O] t2Vector; [U] init, render
    float mVelocity[2];             // 0x018 [O] t2Vector; [U] init, update
    float mAge;                     // 0x020 [O]; [U] init, update, render
    float mDeath;                   // 0x024 [O]; [U] init, update, render
    float mColour[4];               // 0x028 [O] tARGB; [U] init, render (ldl/ldr)
    float mRotation;                // 0x038 [O]; [U] init, render
    float mRotationChangeOverLifetime; // 0x03C [O]; [U] init, render
    float mSmallMovementTimeLeft;   // 0x040 [O]; [U] init, update
    float mSmallMovementX;          // 0x044 [O]; [U] init, update
    float mSmallMovementY;          // 0x048 [O]; [U] init, update

    // Methods bound to their retail symbols by asm labels.
    void render(const t2Vector* off) __asm__("tCrystal_render");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    tCrystal();
};

CHECK_OFFSET(tCrystal, mNumSpawned, 0x000);
CHECK_OFFSET(tCrystal, mActive, 0x004);
CHECK_OFFSET(tCrystal, mPosition, 0x008);
CHECK_OFFSET(tCrystal, mSize, 0x010);
CHECK_OFFSET(tCrystal, mVelocity, 0x018);
CHECK_OFFSET(tCrystal, mAge, 0x020);
CHECK_OFFSET(tCrystal, mDeath, 0x024);
CHECK_OFFSET(tCrystal, mColour, 0x028);
CHECK_OFFSET(tCrystal, mRotation, 0x038);
CHECK_OFFSET(tCrystal, mRotationChangeOverLifetime, 0x03C);
CHECK_OFFSET(tCrystal, mSmallMovementTimeLeft, 0x040);
CHECK_OFFSET(tCrystal, mSmallMovementX, 0x044);
CHECK_OFFSET(tCrystal, mSmallMovementY, 0x048);
CHECK_SIZE(tCrystal, 0x04C);

struct t2Vector;
struct t4Vector;

struct cSplashManager : public cObjNode {
    // cObjNode (object/objnode.h, 0x10): the vptr at 0x0C:
    //   [R] cSplashManager_cSplashManager / __cSplashManager store cSplashManager_vtbl; [P] 0xC; [U]
    //       cSplashManager_CameraSnapped calls slot 14 (reset)
    virtual void reset();           // 14 [O] cSplashManager::reset; [R] cSplashManager_vtbl slot 14 is
                                    //      cSplashManager_reset; [U] cSplashManager_CameraSnapped
    // --- cSplashManager
    unsigned int mViewIndex;        // 0x010 [O]; [R] the ctor's viewIndex; [P] 0x10; [U] update / render pick the view
    int mNumActiveDrops;            // 0x014 [O]; [R] zeroed; [P]; [U] AddDrop, reset, update, render
    int mNumActiveCrystals;         // 0x018 [O]; [R] zeroed; [P]; [U] AddCrystal, reset, update, render
    tDrop mDropList[30];            // 0x01C [O]; [R] 30 x tDrop_tDrop, stride 0x3C; [U] update / render walk them
    tCrystal mCrystalList[30];      // 0x724 [O]; [R] 30 x tCrystal::tCrystal, stride 0x4C; [U] update / render
    int mLastCameraPositionValid;   // 0x100C [O] bool (a word); [R] zeroed; [P]; [U] reset clears, update sets / tests
    t4Vector mLastCameraPosition;   // 0x1010 [O]; [R] (0, 0, 0, 1.0); [P]; [U] update (lq/sq, lqc2), ReportImpactLocal (lqc2)
    float mLastCameraSpeed;         // 0x1020 [O]; [R] zeroed; [P]; [U] update stores it, ReportSnowfall reads it
    float mNumSplashesToAdd;        // 0x1024 [O]; [R] zeroed; [U] reset, AddSplashes, ReportImpactLocal, ReportSnowfall
    float mLastSnowfallIntensity;   // 0x1028 [O]; [U] reset zeroes it; ReportSnowfall / ReportImpactLocal
    char unk_102C[0x4];             // 0x102C tail padding to the 16-byte-aligned size 0x1030

    // Methods bound to their retail symbols by asm labels.
    virtual ~cSplashManager();   // g++'s own symbol _$_14cSplashManager (the retail label renamed)
    void AddCrystal(t2Vector* basePos, int n) __asm__("cSplashManager_AddCrystal");
    void AddSplashes() __asm__("cSplashManager_AddSplashes");
    void ReportImpactLocal(t4Vector* impactPos, float speed) __asm__("cSplashManager_ReportImpactLocal");
    void ReportSnowfall(float intensity) __asm__("cSplashManager_ReportSnowfall");
    virtual void render() __asm__("cSplashManager_render");
};

CHECK_OFFSET(cSplashManager, mViewIndex, 0x010);
CHECK_OFFSET(cSplashManager, mNumActiveDrops, 0x014);
CHECK_OFFSET(cSplashManager, mNumActiveCrystals, 0x018);
CHECK_OFFSET(cSplashManager, mDropList, 0x01C);
CHECK_OFFSET(cSplashManager, mCrystalList, 0x724);
CHECK_OFFSET(cSplashManager, mLastCameraPositionValid, 0x100C);
CHECK_OFFSET(cSplashManager, mLastCameraPosition, 0x1010);
CHECK_OFFSET(cSplashManager, mLastCameraSpeed, 0x1020);
CHECK_OFFSET(cSplashManager, mNumSplashesToAdd, 0x1024);
CHECK_OFFSET(cSplashManager, mLastSnowfallIntensity, 0x1028);
CHECK_OFFSET(cSplashManager, unk_102C, 0x102C);
CHECK_SIZE(cSplashManager, 0x1030);
__asm__("_vt$14cSplashManager = cSplashManager_vtbl");   // g++'s name for the retail vtable

#endif /* VISUALFX_SPLASHFX_H */
