#ifndef FE_FEREAL_H
#define FE_FEREAL_H
#pragma interface

// cRealFE: the front-end module (SSXApp's front end): a cFE (fade screen, UI engine, camera, rider manager, bolt file
// manager, FX loader) plus the online connection managers and the attract / intro movie player.
// EA's source is d:/ssx/code/source/fe/fereal.cpp (the July 2003 preview's assert strings); retail code: fe/fereal
// (cRealFE_cRealFE 0x001A1CE8 ... cRealFE_loadStartState 0x001A2C40) and the out-of-line inlines in main/all_in_one_main
// (0x001DCCB8 ... 0x001DCCE8). The cFE base's methods are in fe/festatecharequip (cFE_cFE, cFE__cFE) and
// main/all_in_one_main (the getters).
//
// Size 0xB5AE0:
//   [R] cSSXApp's front-end loader (main/ssxapp) allocates cMemMan_alloc(0xB5AE0, "FEModule") + cRealFE_cRealFE(mem); the
//       ctor runs cFE_cFE, stores cRealFE_vtbl, builds cConnMan at 0xB5AB0 and cGameConnMan at 0xB5ABC and zeroes 0xB5AD4,
//       0xB5AD8, 0xB5ADC (the last word).
//   [O] On Tour's cRealFE (0x33D0) is cFE (0x33C0) + mpMoviePlayer, mMovieSkippable, mWaitRenderNum, the same order as
//       retail's last three words; On Tour has no connection managers.
// Base: cFE (On Tour 0x33C0; retail 0xB5AB0: cFE_cFE writes up to mFXLoader at 0xB5AA0 and the class is 16-byte aligned
// by cSimpleCamera's t4Matrix, so 0xB5AA4 ... 0xB5AAF is tail padding; nothing reads it) : cFadeScreen (On Tour 0xC;
// [R] cFadeScreen_cFadeScreen stores cFadeScreen_vtbl at 0x0, 0.2f at 0x4 and 0 at 0x8: the vptr is at 0 in retail).
// Vtables: cRealFE_vtbl (0x0046D7C0, 8 slots: 1 dtor, 2 load, 3 purge, 4 loseFocus, 5 gainFocus, 6 update, 7 render,
// 8 loadStartState), cFE_vtbl in cFE_cFE / cFE__cFE.
//
// Real C++ classes (`#pragma interface`: no unit emits their vtables; the equates bind g++'s names to the retail
// tables): cRealFE : cFE : cFadeScreen (main/fadescreen.h), cFE first below; cRealFE's overrides are virtual in
// cAppModule's slot order, plus slot 8 loadStartState. Their constructors and destructors build and destroy the
// embedded objects (flat classes or raw regions) by hand. bools are ints. cFECamera (On Tour: a cSimpleCamera with its
// own
// vtable and no data, 0xA0) is embedded as its base cSimpleCamera (camera/camera.h) and cFERiderManager by value
// (fe/feridermanager.h); the other embedded objects (the bolt file manager, cConnMan, cGameConnMan) are raw regions
// sized from the ctors' offsets.
// Evidence tags:
//   [R] retail's constructors cRealFE_cRealFE / cFE_cFE / cFadeScreen_cFadeScreen, destructors and the allocation size;
//   [P] the July 2003 preview's inline accessors (getUI__3cFE, getCamera__3cFE, getRiderManager__3cFE,
//       getBoltFileManager__3cFE, getConnMan__7cRealFE, getGameConnMan__7cRealFE): names, and retail's copies return
//       this + 0x10 / 0xB0 / 0x1A70 / 0xB5AB0 / 0xB5ABC and lw 0xC (tools/layout_xlate.py: getUI preview 0x18 -> 0xC);
//   [U] how retail code uses the field (C bodies in src/fe/fereal.cpp, src/main/all_in_one_main.cpp, and the asm);
//   [O] SSX On Tour's DWARF (class cRealFE, cFE, cFadeScreen): names, types and order.

#include "layout.h"
#include "main/fadescreen.h"
#include "camera/camera.h"        // cSimpleCamera (cFE::mCamera)
#include "fe/feridermanager.h"     // cFERiderManager (cFE::mRiderManager)
#include "online/connman.h"        // cConnMan, cGameConnMan (cRealFE::mConnMan / mGameConnMan)

struct cUIEngine;
struct cFXLoader;
struct cMoviePlayer;

// ---------------------------------------------------------------------------------------------------------------------
// cFE: the front-end app module's base (cRealFE below derives from it; cDebugFE does not): a cFadeScreen with the UI
// engine, the front-end camera, the rider manager, the bolt file manager and the FX loader. Never allocated on its own
// (its vtable's load and render, slots 2 / 7, are the pure-virtual stub func_00411530).
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: cFE_cFE and cFE__cFE in fe/festatecharequip
// (0x001956F8, 0x001957D0); the out-of-line getters getCamera / getHuman / getRiderManager / getBoltFileManager (two
// copies each) and getUI in main/all_in_one_main (0x001DCB60 ... 0x001DCBD0).
//
// Size 0xB5AB0:
//   [R] cRealFE_cRealFE builds its own cConnMan at 0xB5AB0 after cFE_cFE; cFE_cFE's last member is mFXLoader at 0xB5AA0
//       and the class is 16-byte aligned (cSimpleCamera's t4Matrix), so 0xB5AA4 ... 0xB5AAF is tail padding (see
//       cRealFE above);
//   [O] On Tour's class is 0x33C0 (a smaller cFERiderManager and no bolt file manager).
// Base: cFadeScreen (main/fadescreen.h, 0xC) : cAppModule (the vptr). cFE_cFE runs cFadeScreen_cFadeScreen and stores
// cFE_vtbl (0x0046D878, 7 slots: 1 dtor cFE__cFE, 2 / 7 func_00411530, 3 cAppModule_purge, 4 cFadeScreen_loseFocus,
// 5 cAppModule_gainFocus, 6 cFadeScreen_update); cFE__cFE stores it again and ends with cAppModule's cAppModule_vtbl.
// A real C++ class (see cRealFE below); the embedded objects are raw regions (as in cRealFE). Evidence tags: [R]
// cFE_cFE / cFE__cFE; [P] the July 2003 preview's inline getters (see cRealFE); [U] how retail code uses the field; [O]
// SSX On Tour's DWARF (class cFE, cFadeScreen): names, types and order.

struct cFE : public cFadeScreen {
    //   the vptr 0x000 [R] cFadeScreen_vtbl, then cFE_vtbl (cFE_cFE); cFE_vtbl, then cAppModule_vtbl (cFE__cFE)
    //   FadeTimer 0x004 [O]; [R] cFadeScreen_cFadeScreen 0.2
    //   IsEnding 0x008 [O] (bool, a word); [R] 0
    // --- cFE
    cUIEngine* mpUI;                // 0x00C [O][P] getUI; [R] cFE_cFE: cMemMan_alloc(0x70, "UIEngine") +
                                    //       cUIEngine_cUIEngine; cFE__cFE deletes it (cUIEngine__cUIEngine(.., 3)) when
                                    //       set; [U] cFE_getUI returns it
    cSimpleCamera mCamera;          // 0x010 [O] cFECamera mCamera (= cSimpleCamera, camera/camera.h, with cFECamera's
                                    //       vtable in mCamera.mVtbl at 0xA0); [P] getCamera returns this + 0x10; [R] cFE_cFE:
                                    //       cSimpleCamera_cSimpleCamera(&mCamera), cFECamera_vtbl in mCamera.mVtbl,
                                    //       cSimpleCamera_init(&mCamera, 0, pi/4, 5, 5000); _$_3cFE stores cSimpleCamera_vtbl
                                    //       back; [U] cRealFE_load sets mCamera.Pose (P 0x30, Q 0x40) and updates it
    cFERiderManager mRiderManager;  // 0x0B0 [O] cFERiderManager (fe/feridermanager.h, 0x19C0); [P] getRiderManager /
                                    //       getHuman use this + 0xB0; [R] cFERiderManager::cFERiderManager /
                                    //       __cFERiderManager(&mRiderManager)
    char mBoltFileManager[0xB4030]; // 0x1A70 [P] getBoltFileManager returns this + 0x1A70 (no On Tour member); [R]
                                    //       cFEBoltFileManager::cFEBoltFileManager(this + 0x1A70) (its ctor), cFEBoltFileManager::~cFEBoltFileManager(.., 2) in _$_3cFE;
                                    //       extent: up to mFXLoader
    cFXLoader* mFXLoader;           // 0xB5AA0 [O]; [R] cFE_cFE: cMemMan_alloc(1, "FXLoader_FE") + cFXLoader_cFXLoader;
                                    //       cFE__cFE destroys it when set
    char unk_B5AA4[0xC];            // 0xB5AA4 tail padding (16-byte alignment; no access)

    // Methods bound to their retail symbols by asm labels (the destructor is _$_3cFE; the constructor's body is the C
    // function cFE_cFE in fe/festatecharequip).
    cFE() __asm__("cFE_cFE");
    ~cFE();
    cUIEngine* getUI() __asm__("cFE_getUI");
};

CHECK_OFFSET(cFE, mpUI, 0x00C);
CHECK_OFFSET(cFE, mCamera, 0x010);
CHECK_OFFSET(cFE, mRiderManager, 0x0B0);
CHECK_OFFSET(cFE, mBoltFileManager, 0x1A70);
CHECK_OFFSET(cFE, mFXLoader, 0xB5AA0);
CHECK_OFFSET(cFE, unk_B5AA4, 0xB5AA4);
CHECK_SIZE(cFE, 0xB5AB0);
__asm__("_vt$3cFE = cFE_vtbl");   // g++'s name for the retail vtable

struct cRealFE : public cFE {
    //   the vptr 0x000 [R] cFadeScreen_vtbl, cFE_vtbl, then cRealFE_vtbl (ctors and dtors)
    //   FadeTimer 0x004 [O]; [R] cFadeScreen_cFadeScreen stores 0.2f
    //   IsEnding 0x008 [O] (bool); [R] 0; [U] cRealFE_update stores 1 instead of updating the UI while SSXApp + 0x4 is
    //       set
    // cFE's members (On Tour 0x33C0; retail 0xB5AB0), with cRealFE's notes on them:
    //   mpUI 0x00C [O][P] getUI (preview 0x18); [R] cFE_cFE: cMemMan_alloc(0x70, "UIEngine") +
    //       cUIEngine_cUIEngine; [U] load / loadStartState / render / update
    //   mCamera 0x010 [O] cFECamera mCamera (On Tour 0xA0: a cSimpleCamera, no data of its own); [P]
    //       getCamera returns this + 0x10; [R] cSimpleCamera_cSimpleCamera(&mCamera), cFECamera's
    //       vtable cFECamera_vtbl in mCamera.mVtbl (0xA0), cSimpleCamera_init; [U] render passes
    //       mCamera.View / Near / Far (0x10 / 0x14 / 0x18) and &mCamera.CamMat (0x50) to the
    //       graphics manager; update calls slot 3 of mCamera.mVtbl; load sets mCamera.Pose
    //   mRiderManager 0x0B0 [O] cFERiderManager (fe/feridermanager.h, 0x19C0); [P] getRiderManager returns
    //       this + 0xB0; [R] cFERiderManager::cFERiderManager(&mRiderManager); [U] load / purge /
    //       render / update pass its address
    //   mBoltFileManager 0x1A70 [P] getBoltFileManager returns this + 0x1A70 (name from the accessor; On
    //       Tour has no such member); [R] cFEBoltFileManager::cFEBoltFileManager(this + 0x1A70) (its ctor),
    //       cFEBoltFileManager::~cFEBoltFileManager(.., 2) in cFE__cFE; extent: up to mFXLoader; [U] load / purge /
    //       update (cFEBoltFileManager_init / cFEBoltFileManager_purge / cFEBoltFileManager_update) pass its address
    //   mFXLoader 0xB5AA0 [O]; [R] cFE_cFE: cMemMan_alloc(1, "FXLoader_FE") + cFXLoader_cFXLoader;
    //       cFE__cFE destroys it when set
    //   unk_B5AA4 0xB5AA4 tail padding (cFE is 16-byte aligned; no access)
    // --- cRealFE
    cConnMan mConnMan;              // 0xB5AB0 [P] getConnMan returns this + 0xB5AB0 (cConnMan); [R] cConnMan_cConnMan
                                    //       (state word 0, its vptr cConnMan_vtbl at +0x8: 0xC bytes); [U] update calls
                                    //       cConnMan_update on it when online
    cGameConnMan mGameConnMan;      // 0xB5ABC [P] getGameConnMan returns this + 0xB5ABC (cGameConnMan); [R]
                                    //       cGameConnMan_cGameConnMan (vptr cGameConnMan_vtbl at +0x14: 0x18 bytes); [U]
                                    //       update calls cGameConnMan_update on it when online
    cMoviePlayer* mpMoviePlayer;    // 0xB5AD4 [O]; [R] 0; [U] update creates it (cMemMan_alloc(68, "MoviePlayer")), plays
                                    //       and destroys it; render draws it; loseFocus destroys it
    int mMovieSkippable;            // 0xB5AD8 [O] (bool); [R] 0; [U] update sets it for the intro movie (bit 4) and ends
                                    //       the movie on a button press while set; gainFocus / loseFocus clear it
    int mWaitRenderNum;             // 0xB5ADC [O]; [R] 0; [U] update does nothing while it is set and sets 1 after a
                                    //       movie; render counts it down; gainFocus / loseFocus clear it

    // Methods bound to their retail symbols by asm labels; the virtuals in cRealFE_vtbl's slot order (slot 1 is the
    // destructor, _$_7cRealFE).
    cRealFE() __asm__("cRealFE_cRealFE");
    ~cRealFE();
    virtual void load() __asm__("cRealFE_load");                        // 2
    virtual void purge() __asm__("cRealFE_purge");                      // 3
    virtual bool loseFocus() __asm__("cRealFE_loseFocus");              // 4
    virtual void gainFocus() __asm__("cRealFE_gainFocus");              // 5
    virtual void update() __asm__("cRealFE_update");                    // 6
    virtual bool render() __asm__("cRealFE_render");                    // 7
    virtual void loadStartState() __asm__("cRealFE_loadStartState");    // 8
    void purgeCharAnimations() __asm__("cRealFE_purgeCharAnimations");
};

CHECK_OFFSET(cRealFE, mConnMan, 0xB5AB0);
CHECK_OFFSET(cRealFE, mGameConnMan, 0xB5ABC);
CHECK_OFFSET(cRealFE, mpMoviePlayer, 0xB5AD4);
CHECK_OFFSET(cRealFE, mMovieSkippable, 0xB5AD8);
CHECK_OFFSET(cRealFE, mWaitRenderNum, 0xB5ADC);
CHECK_SIZE(cRealFE, 0xB5AE0);
__asm__("_vt$7cRealFE = cRealFE_vtbl");   // g++'s name for the retail vtable

#endif /* FE_FEREAL_H */
