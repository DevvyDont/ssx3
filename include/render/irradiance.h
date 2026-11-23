#ifndef RENDER_IRRADIANCE_H
#define RENDER_IRRADIANCE_H

// cIrradianceDataBase: the irradiance data base: the loaded data, the item count, the cIrradiance array and their
// names. Methods in render/particle and render/irradiance.
//
// Size 0x10 (On Tour's; retail's methods reach mIrradianceNames at size - 4). Member names, order and offsets are EA's,
// from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"
#include "bx/math3d.h"

// cIrradiance: one irradiance set, ten 16-byte vectors (0xA0) in retail (On Tour's PSP class is 0x44: a name, an
// ambient colour and two cLights - a different platform layout). Evidence: [R] cIrradianceDataBase_Get returns
// mIrradianceArray + i * 0xA0 ([O] cIrradianceDataBase::Get returns cIrradiance&, mIrradianceArray is cIrradiance*);
// [O] cWorldLightMan::IntersectLights(const t4Vector&, cOverlapList*, cIrradiance&, int) = retail
// cWorldLightMan_IntersectLights1(self, &entry->pos, list, &entry->RTlightInfo, 4); [U] the set is copied as 10
// quadwords (getRealTimeLights, func_00389730 adds two sets vector by vector, cIrradiance_Scale / func_00389660 scale
// and tint each of the 10 vectors, cIrradiance_Clear zeroes it). The preview's SetIrradiance__15cPS2GraphicsManRC15
// tIrradianceInfob takes the same 0xA0 block (cPS2GraphicsMan::mIrradianceInfo); whether tIrradianceInfo is a base
// or another name of this class is not known. What each vector holds is not known yet.
struct cIrradiance {
    t4Vector unk_000[10];           // 0x000 [U] ten quadwords, scaled / tinted / added component-wise (x, y, z, w)
};

CHECK_OFFSET(cIrradiance, unk_000, 0x000);
CHECK_SIZE(cIrradiance, 0xA0);

// tIrradianceInfo: the irradiance block the graphics manager is handed. [P] SetIrradiance__15cPS2GraphicsManRC15
// tIrradianceInfob takes one; [O] class tIrradianceInfo : cIrradiance adds no members; [R] cPS2GraphicsMan_SetIrradiance
// copies the 0xA0 block into mIrradianceInfo, and cRider passes its unk_7C0 (one 0xA0 block).
struct tIrradianceInfo : public cIrradiance {
};

CHECK_SIZE(tIrradianceInfo, 0xA0);

struct cIrradianceDataBase {
    char* mData;                    // 0x000 [O]
    int mNumItem;                   // 0x004 [O]
    cIrradiance* mIrradianceArray;  // 0x008 [O]
    char* mIrradianceNames;         // 0x00C [O]
};

CHECK_OFFSET(cIrradianceDataBase, mData, 0x000);
CHECK_OFFSET(cIrradianceDataBase, mNumItem, 0x004);
CHECK_OFFSET(cIrradianceDataBase, mIrradianceArray, 0x008);
CHECK_OFFSET(cIrradianceDataBase, mIrradianceNames, 0x00C);
CHECK_SIZE(cIrradianceDataBase, 0x10);

#endif
