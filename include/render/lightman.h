#ifndef RENDER_LIGHTMAN_H
#define RENDER_LIGHTMAN_H

// cLightInst: one light of the light manager (cPS2LightMan::m_pLights, render/ps2lightman.h) - on the PS2 only a
// handle on the world light record it was created from. The manager allocates them as one array and hands them out
// by index (GetLightObj); cPS2LightMan_CreateLights points each at its tLightRecord (visualfx/worldlightman.h) and then
// calls the cLightInst creator for the record's type, but all four creators are empty on the PS2.
// EA's source: render/lightman.cpp (the unit that holds the creators; no preview assert path).
//
// Size 0x4: [R] cPS2LightMan_Init allocates the array as operator new((n + 50) * 4, "LightInst") and
// cPS2LightMan_GetLightObj returns m_pLights + index * 4 (the July 2003 preview's out-of-line Init__12cPS2LightMani /
// GetLightObj__12cPS2LightMani do the same, tag "LightInst"). [O] On Tour's cPSPLightMan has `cLightInst* m_pLights`
// and `cLightInst* GetLightObj(int)`, so the array holds the objects themselves (4 bytes each), not pointers.
// [O] On Tour's cLightInst is 0x40 (m_dir, m_type, m_pos, m_intensity, m_colour, m_decayType, m_angle, m_dropoff,
// _pad), the state its PSP creators fill; retail's creators are empty, so none of those members exist here.
// No vtable, no base class. A FLAT layout, like main/game.h.
//
// cLightMan: the abstract light manager - the base of cPS2LightMan (render/ps2lightman.h; retail keeps On Tour's
// intermediate name cPSPLightMan in the ctor's mangled symbol). It holds no data, only the vptr; the light array and
// counters are the derived class's. cLightMan::TheLightMan (retail cLightMan_TheLightMan, gp+0x2A9C) is the singleton:
// the ctor sets it, the dtor clears it, cLightMan_getTheLightMan returns it. cLightMan_construct__Fv (On Tour
// cLightMan::construct, render/lightman) creates the PS2 manager: cMemMan_alloc(0x14, "LightMan"), this ctor, then
// cPS2LightMan_vtbl; cSSXApp_init calls it once.
// EA's source path is not in the preview's strings for this file (only ../source/render/ps2lightman.h); the header
// name follows the unit, render/lightman.cpp.
//
// Size 0x4: [O] On Tour's cLightMan is 0x4 (no data members: the vptr); [R] cLightMan_cLightMan__FP12cPSPLightMan /
// cLightMan__cLightMan store cLightMan_vtbl at 0x0 and nothing else; cPS2LightMan's own members start at 0x4 (its Init,
// GetLightObj). The vptr is at 0x0 because cLightMan has no data before it (g++ 2.95 puts the vptr after the own data
// of the class that first declares virtuals).
// No base class. A FLAT layout, like main/game.h.
//
// Vtable cLightMan_vtbl (0x004936D8; 8-byte {delta, fn} entries, entry 0 = RTTI), slot names and order [O] On Tour's
// cLightMan / cPSPLightMan, [P] the preview's _vt$9cLightMan (0x461680) and its out-of-line inlines
// (CreateAmbient__9cLightManiRC5tARGBf, LoadLight__9cLightManii, SetChannel__9cLightManii, ClearChannel__9cLightMani):
//   1 ~cLightMan (cLightMan__cLightMan)   2 GetLightObj(int)        3 Init(int)          4 Purge()
//   5 CreateLights(tLightRecord*, int, int) 6 ResetDynamicLights()  7 GetDynamicLight()  8 SetAmbient(const tARGB&)
//   9 CreateAmbient(int, const tARGB&, float) (cLightMan_CreateAmbient__FPv, empty)
//   10 CreateDirectional  11 CreatePoint  12 CreateSpot
//   13 LoadLight(int, int) (cLightMan_LoadLight__FPv, empty)
//   14 SetChannel(int, int) (cLightMan_SetChannel__FPv, empty)
//   15 ClearChannel(int) (cLightMan_ClearChannel__FPv, empty)
// Slots 2-8 and 10-12 are __pure_virtual in cLightMan_vtbl; cPS2LightMan_vtbl (0x00493650) fills all 15.

#include "layout.h"

struct tLightRecord;                 // visualfx/worldlightman.h (0x70 bytes)

struct cLightInst {
    tLightRecord* mpRecord;          // 0x0 (ours) [U] cPS2LightMan_CreateLights stores the light's record (`obj[0] =
                                     //     &lights[i]`) before calling the creator; no reader found (cSSXApp_init drops
                                     //     cLightMan_construct's result, and cLightMan_TheLightMan is only touched by
                                     //     the ctor / dtor and cLightMan_getTheLightMan, which nothing calls)

    // Methods bound to their retail symbols by asm labels.
    void CreateAmbient() __asm__("cLightInst_CreateAmbient__FPv");
    void CreateDirectional() __asm__("cLightInst_CreateDirectional__FPv");
    void CreatePoint() __asm__("cLightInst_CreatePoint__FPv");
    void CreateSpot() __asm__("cLightInst_CreateSpot__FPv");
};

CHECK_OFFSET(cLightInst, mpRecord, 0x0);
CHECK_SIZE(cLightInst, 0x4);

// The vtable slots above, as indexes into the {delta, fn} entries (include/vtable.h sVEntry<F>).
enum {
    kLightManVt_dtor = 1, kLightManVt_GetLightObj = 2, kLightManVt_Init = 3, kLightManVt_Purge = 4,
    kLightManVt_CreateLights = 5, kLightManVt_ResetDynamicLights = 6, kLightManVt_GetDynamicLight = 7,
    kLightManVt_SetAmbient = 8, kLightManVt_CreateAmbient = 9, kLightManVt_CreateDirectional = 10,
    kLightManVt_CreatePoint = 11, kLightManVt_CreateSpot = 12, kLightManVt_LoadLight = 13,
    kLightManVt_SetChannel = 14, kLightManVt_ClearChannel = 15
};

struct cLightMan {
    void** mVtbl;                    // 0x0 [R] cLightMan_cLightMan__FP12cPSPLightMan / cLightMan__cLightMan store
                                     //     cLightMan_vtbl; cLightMan_construct__Fv then stores cPS2LightMan_vtbl;
                                     //     [U] cPS2LightMan_CreateLights / CreateAmbient / ... call slot 2 through it

    // Methods bound to their retail symbols by asm labels.
    void CreateAmbient() __asm__("cLightMan_CreateAmbient__FPv");
    void LoadLight() __asm__("cLightMan_LoadLight__FPv");
    void SetChannel() __asm__("cLightMan_SetChannel__FPv");
    void ClearChannel() __asm__("cLightMan_ClearChannel__FPv");
};

CHECK_OFFSET(cLightMan, mVtbl, 0x0);
CHECK_SIZE(cLightMan, 0x4);

// The light manager singleton ([O] static cLightMan* TheLightMan).
extern cLightMan* cLightMan_TheLightMan;

#endif /* RENDER_LIGHTMAN_H */
