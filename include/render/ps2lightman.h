#ifndef RENDER_PS2LIGHTMAN_H
#define RENDER_PS2LIGHTMAN_H

// cPS2LightMan: the light manager (cLightMan::TheLightMan): an array of light instances, the first n static (from the
// world's light records) followed by 50 dynamic slots handed out per frame. Size 0x14: [R] cLightMan_construct__Fv
// (render/lightman) does cMemMan_alloc(0x14, "LightMan") + cLightMan_cLightMan__FP12cPSPLightMan (vtable
// cLightMan_vtbl, stores TheLightMan), then stores cPS2LightMan_vtbl. Destructors cPS2LightMan__cPS2LightMan__FPv (slot 0) ->
// cLightMan__cLightMan (restores cLightMan_vtbl, clears TheLightMan).
// Bases: cLightMan (On Tour 0x4: vptr only) <- cPSPLightMan (retail class name kept from On Tour, render/ps2lightman)
// <- cPS2LightMan. The vptr is at 0x0 because cLightMan has no data.
// Methods (cPS2LightMan_vtbl, On Tour's cPSPLightMan order): 0 ~cPS2LightMan, 1 GetLightObj(i) cPS2LightMan_GetLightObj__FPvi,
// 2 Init(n) cPS2LightMan_Init, 3 Purge cPS2LightMan_Purge, 4 CreateLights cPS2LightMan_CreateLights, 5 ResetDynamicLights,
// 6 GetDynamicLight cPS2LightMan_GetDynamicLight, 7 SetAmbient (empty), 8-11 CreateAmbient/Directional/Point/Spot cPS2LightMan_CreateAmbient,
// cPS2LightMan_CreateDirectional, cPS2LightMan_CreatePoint, cPS2LightMan_CreateSpot (render/lightman), 12-14 cPS2LightMan_LoadLight/40/48 (empty).
// Instance global: cLightMan_TheLightMan (gp+0x2A9C) = cLightMan::TheLightMan, returned by cLightMan_getTheLightMan.
//
// FLAT layout. Members: [O] On Tour's cPSPLightMan (0x18: m_iStartDynLight 4, m_iDynLight 8, m_iMaxLights 0xC,
// m_pLights 0x10, m_iHighWaterLightCount 0x14); retail has no high-water count and a different order, so each member is
// placed by [R] Init (cPS2LightMan_Init) and [U] the accessors.

#include "layout.h"

struct cLightInst;

struct tLightRecord;

// How a light falls off with distance (tLightRecord::m_cDecayRate, the decay argument of cLightInst_CreatePoint /
// CreateSpot). [O] On Tour's enum eLightDecay (cLightInst::m_decayType; getIntensityInv(float, int decayRate)), names
// and values; [U] retail fits: getFalloffIntensity falls off as 1/d, 1/d^2, 1/d^3 and getIntensityInv ramps as x, x^2,
// x^3 for DECAY_LINEAR, DECAY_EXPONENTIAL2, DECAY_EXPONENTIAL3.
enum eLightDecay {
    DECAY_NONE = 0, DECAY_LINEAR = 1, DECAY_EXPONENTIAL2 = 2, DECAY_EXPONENTIAL3 = 3, NUMDECAYTYPES = 4
};

struct cPS2LightMan {
    // --- cLightMan
    void** mVtbl;                    // 0x00 [R] cLightMan_vtbl (cPSPLightMan ctor), cPS2LightMan_vtbl (cLightMan_construct)
    // --- cPSPLightMan
    cLightInst** m_pLights;          // 0x04 [O]; [R] Init: = new((n + 50) * 4); [U] GetLightObj returns &m_pLights[i];
                                     //      Purge frees it; CreateLights / Create* index it
    int m_iStartDynLight;            // 0x08 [O]; [R] Init: = n (the static light count); [U] GetDynamicLight adds it
    int m_iDynLight;                 // 0x0C [O]; [R] Init: = 0; [U] ResetDynamicLights = 0, GetDynamicLight post-increments it
    int m_iMaxLights;                // 0x10 [O]; [R] Init: = n + 50

    // Methods bound to their retail symbols by asm labels.
    void* GetLightObj(int index) __asm__("cPS2LightMan_GetLightObj__FPvi");
    void Init(int numLights) __asm__("cPS2LightMan_Init");
    void Purge() __asm__("cPS2LightMan_Purge");
    void ResetDynamicLights() __asm__("cPS2LightMan_ResetDynamicLights__FPv");
    int GetDynamicLight() __asm__("cPS2LightMan_GetDynamicLight");
    void CreateLights(tLightRecord* lights, int n, int base) __asm__("cPS2LightMan_CreateLights");
    void CreateAmbient(int light, int colour, float intensity) __asm__("cPS2LightMan_CreateAmbient");
    void CreateDirectional(int light, int colour, int dir, float intensity) __asm__("cPS2LightMan_CreateDirectional");
    void CreatePoint(int light, int colour, int pos, float intensity, int decay) __asm__("cPS2LightMan_CreatePoint");
    void CreateSpot(int light, int colour, int pos, int dir, float x, float y, int decay, float z)
        __asm__("cPS2LightMan_CreateSpot");
    void SetAmbient() __asm__("cPS2LightMan_SetAmbient__FPv");
    void LoadLight() __asm__("cPS2LightMan_LoadLight__FPv");
    void SetChannel() __asm__("cPS2LightMan_SetChannel__FPv");
    void ClearChannel() __asm__("cPS2LightMan_ClearChannel__FPv");
};

CHECK_OFFSET(cPS2LightMan, mVtbl, 0x00);
CHECK_OFFSET(cPS2LightMan, m_pLights, 0x04);
CHECK_OFFSET(cPS2LightMan, m_iStartDynLight, 0x08);
CHECK_OFFSET(cPS2LightMan, m_iDynLight, 0x0C);
CHECK_OFFSET(cPS2LightMan, m_iMaxLights, 0x10);
CHECK_SIZE(cPS2LightMan, 0x14);

#endif /* RENDER_PS2LIGHTMAN_H */
