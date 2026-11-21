#ifndef VISUALFX_VISUALEFFECTSMAN_H
#define VISUALFX_VISUALEFFECTSMAN_H

// cVisualEffectsMan: the manager of the screen / world visual effects. It holds one small cVisualEffect object per
// effect system (render toggles, light glow, plants, depth and height fog, detail textures, screen tint, shadow, snow
// surface, board trail, flags, power-up / snowfall / splash toggles, tube, path arrow); most of their state is static.
// An abstract base (update and render are pure virtuals, func_00411530) whose ctor clears the 17 slots, sets default
// effect texture ids on the graphics manager and makes the object the current manager (D_004A3B78, returned by
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

struct cRenderToggles;
struct cVisualEffectsTest;
struct cLightGlow;
struct cPlants;
struct cDepthFog;
struct cHeightFog;
struct cDetailSystem;
struct cScreenTint;
struct cShadow;
struct cSnowSurface;
struct cBoardTrail;
struct cFlags;
struct cPowerUpFXToggles;
struct cSnowfallToggles;
struct cSplashToggles;
struct cTube;
struct cPathArrow;

struct cVisualEffectsMan {
    cRenderToggles* m_RenderToggles;        // 0x00 [O]; [R] ctor 0; PS2 ctor "RenderToggles" (cRenderToggles_vtbl)
    cVisualEffectsTest* m_VisualEffectsTest; // 0x04 [O]; [R] ctor 0; PS2 ctor "VisualEffectsTest" (cVisualEffectsTest_vtbl)
    cLightGlow* m_LightGlow;                // 0x08 [O]; [R] ctor 0; PS2 ctor "LightGlow" (D_00487EB8)
    cPlants* m_Plants;                      // 0x0C [O]; [R] ctor 0; PS2 ctor "Plants" (cPlants_vtbl)
    cDepthFog* m_DepthFog;                  // 0x10 [O]; [R] ctor 0; PS2 ctor "DepthFog" (D_00487E88)
    cHeightFog* m_HeightFog;                // 0x14 [O]; [R] ctor 0; PS2 ctor "HeightFog" (D_00487E58)
    cDetailSystem* m_DetailSystem;          // 0x18 [O]; [R] ctor 0; PS2 ctor "DetailSystem" (cDetailSystem_cDetailSystem)
    cScreenTint* m_ScreenTint;              // 0x1C [O]; [R] ctor 0; PS2 ctor "ScreenTint" (cScreenTint_vtbl)
    cShadow* m_Shadow;                      // 0x20 [O]; [R] ctor 0; PS2 ctor "Shadow" (D_00487E28)
    cSnowSurface* m_SnowSurface;            // 0x24 [O]; [R] ctor 0; PS2 ctor "SnowSurface" (D_00487DF8)
    cBoardTrail* m_BoardTrail;              // 0x28 [O]; [R] ctor 0; PS2 ctor "BoardTrail" (D_00487FA8)
    cFlags* m_Flags;                        // 0x2C [O]; [R] ctor 0; PS2 ctor "Flags" (D_00487DC8)
    cPowerUpFXToggles* m_PowerUpFXToggles;  // 0x30 [O]; [R] ctor 0; PS2 ctor "PowerUpFXToggles" (D_00488330)
    cSnowfallToggles* m_SnowfallToggles;    // 0x34 [O]; [R] ctor 0; PS2 ctor "SnowfallToggles" (D_00488050)
    cSplashToggles* m_SplashToggles;        // 0x38 [O]; [R] ctor 0; PS2 ctor "SplashToggles" (D_004882B0)
    cTube* m_Tube;                          // 0x3C [O]; [R] ctor 0; PS2 ctor "Tube" (cTube_vtbl)
    cPathArrow* m_PathArrow;                // 0x40 [O]; [R] ctor 0; PS2 ctor "PathArrow" (cPathArrow_cPathArrow)
    void** mVtbl;                           // 0x44 [R] cVisualEffectsMan_vtbl (ctor and dtor); [U] cVisualEffectsObjNode
                                            //      calls slots 1-3 through it
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
