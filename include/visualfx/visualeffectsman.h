#ifndef VISUALFX_VISUALEFFECTSMAN_H
#define VISUALFX_VISUALEFFECTSMAN_H
#pragma interface

// cVisualEffectsMan: the manager of the screen / world visual effects. It holds one small cVisualEffect object per
// effect system (render toggles, light glow, plants, depth and height fog, detail textures, screen tint, shadow, snow
// surface, board trail, flags, power-up / snowfall / splash toggles, tube, path arrow); most of their state is static.
// An abstract base (update and render are pure virtuals, func_00411530) whose ctor clears the 17 slots, sets default
// effect texture ids on the graphics manager and makes the object the current manager (gVisualEffectsMan, returned by
// visualEffectsMan()); commonUpdate copies one camera's render-state values into the effect statics and the graphics
// manager's per-view arrays. cVisualEffectsMan_construct builds the platform class cPS2VisualEffectsMan, whose ctor
// allocates the 17 objects (4 bytes each) and whose update / render call each one's slots 2 / 3 (then commonUpdate per
// camera). cGame_load wraps the result in a cVisualEffectsObjNode (main/gameobjnodes.h).
// EA's source: d:/ssx/code/source/visualfx/visualeffectsman.cpp (July 2003 preview assert strings). Retail methods:
// unit visualfx/renderstateman (0x002EFF90 visualEffectsMan .. 0x002F00A0 commonUpdate, 0x002F0510 construct),
// visualfx/worldlightman (0x002F8310, the base dtor) and, for cPS2VisualEffectsMan, seg/seg_1F1548 (0x002F0548 ctor ..
// 0x002F0CD0 render).
//
// Sizes: cVisualEffectsMan 0x48 and cPS2VisualEffectsMan 0x48 (it adds no members): [R] cVisualEffectsMan_construct
// does cMemMan_alloc(72, "PS2VisualEffectsMan") + cPS2VisualEffectsMan_cPS2VisualEffectsMan; both ctors write
// 0x0-0x44. On Tour: cVisualEffectsMan 0x48 (the same 17 pointers at 0x0-0x40), cPSPVisualEffectsMan 0x48 (no members).
// No base class; g++ 2.95 puts the vptr after the class's data: 0x44. Vtables (3 slots: 1 dtor, 2 update, 3 render):
// cVisualEffectsMan_vtbl 0x00487DA0 (slots 2 / 3 pure), cPS2VisualEffectsMan_vtbl 0x00487D28.
//
// A FLAT layout, like main/game.h; cPS2VisualEffectsMan repeats the base's members. Evidence tags:
//   [R] retail ctors cVisualEffectsMan_cVisualEffectsMan (clears 0x0-0x40) and cPS2VisualEffectsMan_cPS2VisualEffectsMan
//       (cMemMan_alloc(4, "<name>") per slot: the tags are On Tour's member names without "m_"), the dtors and the
//       72-byte allocation;
//   [P] tools/layout_xlate.py pairs three cPS2VisualEffectsMan methods: 0x0-0x40 preview == retail; no inline
//       accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cVisualEffectsMan 0x48: m_RenderToggles .. m_PathArrow; cPSPVisualEffectsMan).

#include "layout.h"
#include "bx/memman.h"

// The base of the 17 effect objects: [O] On Tour class cVisualEffect (size 0x4, the vptr only; ~cVisualEffect,
// update, render). [R] every effect's destructor stores cVisualEffect_vtbl before freeing (render/font.cpp) and the
// effects are 4-byte allocations holding their own vtable (the cPS2VisualEffectsMan ctor); the manager's dtor, update
// and render call their slots 1, 2 and 3. The destructor is inline (every effect's destructor inlines it); retail's
// out-of-line copies cVisualEffect__cVisualEffect and cVisualEffect__cVisualEffect_2 stay C functions.
struct cVisualEffect {
    virtual ~cVisualEffect() {}   // 1 [O]; [U] the manager's dtor deletes each effect through it
    virtual void update() __asm__("cVisualEffect_update__FPv");      // 2 [O]; [U] cPS2VisualEffectsMan_update
    virtual void render() __asm__("cVisualEffect_render__FPv");      // 3 [O]; [U] cPS2VisualEffectsMan_render
    void deferredRender() __asm__("cVisualEffect_deferredRender__FPv");
};

CHECK_SIZE(cVisualEffect, 0x4);
__asm__("_vt$13cVisualEffect = cVisualEffect_vtbl");   // g++'s name for the retail vtable

// The effects ([O] On Tour's classes of these names, 0x4 each: their tweak values are statics; [R] their own vtables
// cRenderToggles_vtbl .. cTube_vtbl, cDetailSystem_cDetailSystem / cPathArrow_cPathArrow; dtors in render/font.cpp).
struct cRenderToggles : cVisualEffect { virtual ~cRenderToggles(); };
__asm__("_vt$14cRenderToggles = cRenderToggles_vtbl");   // g++'s name for the retail vtable
struct cVisualEffectsTest : cVisualEffect { virtual ~cVisualEffectsTest(); };
__asm__("_vt$18cVisualEffectsTest = cVisualEffectsTest_vtbl");   // g++'s name for the retail vtable
struct cLightGlow : cVisualEffect { virtual ~cLightGlow(); };
__asm__("_vt$10cLightGlow = cLightGlow_vtbl");   // g++'s name for the retail vtable
struct cPlants : cVisualEffect { virtual ~cPlants(); };
__asm__("_vt$7cPlants = cPlants_vtbl");   // g++'s name for the retail vtable
struct cDepthFog : cVisualEffect { virtual ~cDepthFog(); };
__asm__("_vt$9cDepthFog = cDepthFog_vtbl");   // g++'s name for the retail vtable
struct cHeightFog : cVisualEffect { virtual ~cHeightFog(); };
__asm__("_vt$10cHeightFog = cHeightFog_vtbl");   // g++'s name for the retail vtable
struct cDetailSystem : cVisualEffect {
    cDetailSystem() __asm__("cDetailSystem_cDetailSystem__FPv");
    virtual ~cDetailSystem();
};
__asm__("_vt$13cDetailSystem = cDetailSystem_vtbl");   // g++'s name for the retail vtable
struct cScreenTint : cVisualEffect { virtual ~cScreenTint(); };
__asm__("_vt$11cScreenTint = cScreenTint_vtbl");   // g++'s name for the retail vtable
struct cShadow : cVisualEffect { virtual ~cShadow(); };
__asm__("_vt$7cShadow = cShadow_vtbl");   // g++'s name for the retail vtable
struct cSnowSurface : cVisualEffect { virtual ~cSnowSurface(); };
__asm__("_vt$12cSnowSurface = cSnowSurface_vtbl");   // g++'s name for the retail vtable
struct cBoardTrail : cVisualEffect { virtual ~cBoardTrail(); };
__asm__("_vt$11cBoardTrail = cBoardTrail_vtbl");   // g++'s name for the retail vtable
struct cFlags : cVisualEffect { virtual ~cFlags(); };
__asm__("_vt$6cFlags = cFlags_vtbl");   // g++'s name for the retail vtable
struct cPowerUpFXToggles : cVisualEffect { virtual ~cPowerUpFXToggles(); };
__asm__("_vt$17cPowerUpFXToggles = cPowerUpFXToggles_vtbl");   // g++'s name for the retail vtable
struct cSnowfallToggles : cVisualEffect { virtual ~cSnowfallToggles(); };
__asm__("_vt$16cSnowfallToggles = cSnowfallToggles_vtbl");   // g++'s name for the retail vtable
struct cSplashToggles : cVisualEffect { virtual ~cSplashToggles(); };
__asm__("_vt$14cSplashToggles = cSplashToggles_vtbl");   // g++'s name for the retail vtable
struct cTube : cVisualEffect {
    cTube() __asm__("cTube_cTube");
    virtual ~cTube();
    virtual void render() __asm__("cTube_render__FPv");
};
__asm__("_vt$5cTube = cTube_vtbl");   // g++'s name for the retail vtable
struct cPathArrow : cVisualEffect {
    cPathArrow() __asm__("cPathArrow_cPathArrow");
    virtual ~cPathArrow();
    virtual void update() __asm__("cPathArrow_update__FPv");
    virtual void render() __asm__("cPathArrow_render__FPv");
};
__asm__("_vt$10cPathArrow = cPathArrow_vtbl");   // g++'s name for the retail vtable

struct cVisualEffectsMan {
    cRenderToggles* m_RenderToggles;        // 0x00 [O]; [R] ctor 0; PS2 ctor "RenderToggles" (cRenderToggles_vtbl)
    cVisualEffectsTest* m_VisualEffectsTest; // 0x04 [O]; [R] ctor 0; PS2 ctor "VisualEffectsTest" (cVisualEffectsTest_vtbl)
    cLightGlow* m_LightGlow;                // 0x08 [O]; [R] ctor 0; PS2 ctor "LightGlow" (cLightGlow_vtbl)
    cPlants* m_Plants;                      // 0x0C [O]; [R] ctor 0; PS2 ctor "Plants" (cPlants_vtbl)
    cDepthFog* m_DepthFog;                  // 0x10 [O]; [R] ctor 0; PS2 ctor "DepthFog" (cDepthFog_vtbl)
    cHeightFog* m_HeightFog;                // 0x14 [O]; [R] ctor 0; PS2 ctor "HeightFog" (cHeightFog_vtbl)
    cDetailSystem* m_DetailSystem;          // 0x18 [O]; [R] ctor 0; PS2 ctor "DetailSystem" (cDetailSystem_cDetailSystem)
    cScreenTint* m_ScreenTint;              // 0x1C [O]; [R] ctor 0; PS2 ctor "ScreenTint" (cScreenTint_vtbl)
    cShadow* m_Shadow;                      // 0x20 [O]; [R] ctor 0; PS2 ctor "Shadow" (cShadow_vtbl)
    cSnowSurface* m_SnowSurface;            // 0x24 [O]; [R] ctor 0; PS2 ctor "SnowSurface" (cSnowSurface_vtbl)
    cBoardTrail* m_BoardTrail;              // 0x28 [O]; [R] ctor 0; PS2 ctor "BoardTrail" (cBoardTrail_vtbl)
    cFlags* m_Flags;                        // 0x2C [O]; [R] ctor 0; PS2 ctor "Flags" (cFlags_vtbl)
    cPowerUpFXToggles* m_PowerUpFXToggles;  // 0x30 [O]; [R] ctor 0; PS2 ctor "PowerUpFXToggles" (cPowerUpFXToggles_vtbl)
    cSnowfallToggles* m_SnowfallToggles;    // 0x34 [O]; [R] ctor 0; PS2 ctor "SnowfallToggles" (cSnowfallToggles_vtbl)
    cSplashToggles* m_SplashToggles;        // 0x38 [O]; [R] ctor 0; PS2 ctor "SplashToggles" (cSplashToggles_vtbl)
    cTube* m_Tube;                          // 0x3C [O]; [R] ctor 0; PS2 ctor "Tube" (cTube_vtbl)
    cPathArrow* m_PathArrow;                // 0x40 [O]; [R] ctor 0; PS2 ctor "PathArrow" (cPathArrow_cPathArrow)
    void** mVtbl;                           // 0x44 [R] cVisualEffectsMan_vtbl (ctor and dtor); [U] cVisualEffectsObjNode
                                            //      calls slots 1-3 through it

    // Methods bound to their retail symbols by asm labels.
    void commonUpdate(int i) __asm__("cVisualEffectsMan_commonUpdate");
};

CHECK_OFFSET(cVisualEffectsMan, m_RenderToggles, 0x00);
CHECK_OFFSET(cVisualEffectsMan, m_VisualEffectsTest, 0x04);
CHECK_OFFSET(cVisualEffectsMan, m_LightGlow, 0x08);
CHECK_OFFSET(cVisualEffectsMan, m_Plants, 0x0C);
CHECK_OFFSET(cVisualEffectsMan, m_DepthFog, 0x10);
CHECK_OFFSET(cVisualEffectsMan, m_HeightFog, 0x14);
CHECK_OFFSET(cVisualEffectsMan, m_DetailSystem, 0x18);
CHECK_OFFSET(cVisualEffectsMan, m_ScreenTint, 0x1C);
CHECK_OFFSET(cVisualEffectsMan, m_Shadow, 0x20);
CHECK_OFFSET(cVisualEffectsMan, m_SnowSurface, 0x24);
CHECK_OFFSET(cVisualEffectsMan, m_BoardTrail, 0x28);
CHECK_OFFSET(cVisualEffectsMan, m_Flags, 0x2C);
CHECK_OFFSET(cVisualEffectsMan, m_PowerUpFXToggles, 0x30);
CHECK_OFFSET(cVisualEffectsMan, m_SnowfallToggles, 0x34);
CHECK_OFFSET(cVisualEffectsMan, m_SplashToggles, 0x38);
CHECK_OFFSET(cVisualEffectsMan, m_Tube, 0x3C);
CHECK_OFFSET(cVisualEffectsMan, m_PathArrow, 0x40);
CHECK_OFFSET(cVisualEffectsMan, mVtbl, 0x44);
CHECK_SIZE(cVisualEffectsMan, 0x48);

// cPS2VisualEffectsMan: the PS2 platform class (On Tour's PSP twin is cPSPVisualEffectsMan); adds no members. [U] its
// dtor deletes all 17 objects through their slot 1, update calls each one's slot 2 then commonUpdate per camera, render
// each one's slot 3 (all 17 read; preview == retail per tools/layout_xlate.py).
struct cPS2VisualEffectsMan {
    // --- cVisualEffectsMan (0x48)
    cRenderToggles* m_RenderToggles;        // 0x00 [O] cVisualEffectsMan::m_RenderToggles; [R] "RenderToggles"
    cVisualEffectsTest* m_VisualEffectsTest; // 0x04 [O]; [R] "VisualEffectsTest"
    cLightGlow* m_LightGlow;                // 0x08 [O]; [R] "LightGlow"
    cPlants* m_Plants;                      // 0x0C [O]; [R] "Plants"
    cDepthFog* m_DepthFog;                  // 0x10 [O]; [R] "DepthFog"
    cHeightFog* m_HeightFog;                // 0x14 [O]; [R] "HeightFog"
    cDetailSystem* m_DetailSystem;          // 0x18 [O]; [R] "DetailSystem"
    cScreenTint* m_ScreenTint;              // 0x1C [O]; [R] "ScreenTint"
    cShadow* m_Shadow;                      // 0x20 [O]; [R] "Shadow"
    cSnowSurface* m_SnowSurface;            // 0x24 [O]; [R] "SnowSurface"
    cBoardTrail* m_BoardTrail;              // 0x28 [O]; [R] "BoardTrail"
    cFlags* m_Flags;                        // 0x2C [O]; [R] "Flags"
    cPowerUpFXToggles* m_PowerUpFXToggles;  // 0x30 [O]; [R] "PowerUpFXToggles"
    cSnowfallToggles* m_SnowfallToggles;    // 0x34 [O]; [R] "SnowfallToggles"
    cSplashToggles* m_SplashToggles;        // 0x38 [O]; [R] "SplashToggles"
    cTube* m_Tube;                          // 0x3C [O]; [R] "Tube"
    cPathArrow* m_PathArrow;                // 0x40 [O]; [R] "PathArrow"
    void** mVtbl;                           // 0x44 [R] cPS2VisualEffectsMan_vtbl (ctor; dtor, which then stores
                                            //      cVisualEffectsMan_vtbl)

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cPS2VisualEffectsMan_update");
    void render() __asm__("cPS2VisualEffectsMan_render");
};

CHECK_OFFSET(cPS2VisualEffectsMan, m_RenderToggles, 0x00);
CHECK_OFFSET(cPS2VisualEffectsMan, m_VisualEffectsTest, 0x04);
CHECK_OFFSET(cPS2VisualEffectsMan, m_LightGlow, 0x08);
CHECK_OFFSET(cPS2VisualEffectsMan, m_Plants, 0x0C);
CHECK_OFFSET(cPS2VisualEffectsMan, m_DepthFog, 0x10);
CHECK_OFFSET(cPS2VisualEffectsMan, m_HeightFog, 0x14);
CHECK_OFFSET(cPS2VisualEffectsMan, m_DetailSystem, 0x18);
CHECK_OFFSET(cPS2VisualEffectsMan, m_ScreenTint, 0x1C);
CHECK_OFFSET(cPS2VisualEffectsMan, m_Shadow, 0x20);
CHECK_OFFSET(cPS2VisualEffectsMan, m_SnowSurface, 0x24);
CHECK_OFFSET(cPS2VisualEffectsMan, m_BoardTrail, 0x28);
CHECK_OFFSET(cPS2VisualEffectsMan, m_Flags, 0x2C);
CHECK_OFFSET(cPS2VisualEffectsMan, m_PowerUpFXToggles, 0x30);
CHECK_OFFSET(cPS2VisualEffectsMan, m_SnowfallToggles, 0x34);
CHECK_OFFSET(cPS2VisualEffectsMan, m_SplashToggles, 0x38);
CHECK_OFFSET(cPS2VisualEffectsMan, m_Tube, 0x3C);
CHECK_OFFSET(cPS2VisualEffectsMan, m_PathArrow, 0x40);
CHECK_OFFSET(cPS2VisualEffectsMan, mVtbl, 0x44);
CHECK_SIZE(cPS2VisualEffectsMan, 0x48);

#endif /* VISUALFX_VISUALEFFECTSMAN_H */
