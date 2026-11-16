#ifndef RENDER_PS2LIGHTMAN_H
#define RENDER_PS2LIGHTMAN_H

// cPS2LightMan: the light manager (cLightMan::TheLightMan): an array of light instances, the first n static (from the
// world's light records) followed by 50 dynamic slots handed out per frame. Size 0x14: [R] cLightMan_construct__Fv
// (render/lightman) does cMemMan_alloc(0x14, "LightMan") + cPSPLightMan_cPSPLightMan__FP12cPSPLightMan (vtable
// D_004936D8, stores TheLightMan), then stores D_00493650. Destructors cPS2LightMan__cPS2LightMan__FPv (slot 0) ->
// cLightMan__cLightMan (restores D_004936D8, clears TheLightMan).
// Bases: cLightMan (On Tour 0x4: vptr only) <- cPSPLightMan (retail class name kept from On Tour, render/ps2lightman)
// <- cPS2LightMan. The vptr is at 0x0 because cLightMan has no data.
// Methods (D_00493650, On Tour's cPSPLightMan order): 0 ~cPS2LightMan, 1 GetLightObj(i) func_00394C88__FPvi,
// 2 Init(n) func_00394C98, 3 Purge cPS2LightMan_Purge, 4 CreateLights cPS2LightMan_CreateLights, 5 ResetDynamicLights,
// 6 GetDynamicLight func_00394D18, 7 SetAmbient (empty), 8-11 CreateAmbient/Directional/Point/Spot cPS2LightMan_CreateAmbient,
// cPS2LightMan_CreateDirectional, cPS2LightMan_CreatePoint, cPS2LightMan_CreateSpot (render/lightman), 12-14 func_00394D38/40/48 (empty).
// Instance global: cLightMan_TheLightMan (gp+0x2A9C) = cLightMan::TheLightMan, returned by func_00394C40 (getTheLightMan).
//
// FLAT layout. Members: [O] On Tour's cPSPLightMan (0x18: m_iStartDynLight 4, m_iDynLight 8, m_iMaxLights 0xC,
// m_pLights 0x10, m_iHighWaterLightCount 0x14); retail has no high-water count and a different order, so each member is
// placed by [R] Init (func_00394C98) and [U] the accessors.

#include "layout.h"

struct cLightInst;

struct cPS2LightMan {
    // --- cLightMan
    void** mVtbl;                    // 0x00 [R] D_004936D8 (cPSPLightMan ctor), D_00493650 (cLightMan_construct)
    // --- cPSPLightMan
    cLightInst** m_pLights;          // 0x04 [O]; [R] Init: = new((n + 50) * 4); [U] GetLightObj returns &m_pLights[i];
                                     //      Purge frees it; CreateLights / Create* index it
    int m_iStartDynLight;            // 0x08 [O]; [R] Init: = n (the static light count); [U] GetDynamicLight adds it
    int m_iDynLight;                 // 0x0C [O]; [R] Init: = 0; [U] ResetDynamicLights = 0, GetDynamicLight post-increments it
    int m_iMaxLights;                // 0x10 [O]; [R] Init: = n + 50
};

CHECK_OFFSET(cPS2LightMan, mVtbl, 0x00);
CHECK_OFFSET(cPS2LightMan, m_pLights, 0x04);
CHECK_OFFSET(cPS2LightMan, m_iStartDynLight, 0x08);
CHECK_OFFSET(cPS2LightMan, m_iDynLight, 0x0C);
CHECK_OFFSET(cPS2LightMan, m_iMaxLights, 0x10);
CHECK_SIZE(cPS2LightMan, 0x14);

#endif /* RENDER_PS2LIGHTMAN_H */
