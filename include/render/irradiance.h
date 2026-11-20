#ifndef RENDER_IRRADIANCE_H
#define RENDER_IRRADIANCE_H

// cIrradianceDataBase: the irradiance data base: the loaded data, the item count, the cIrradiance array and their
// names. Methods in render/particle and render/irradiance.
//
// Size 0x10 (On Tour's; retail's methods reach mIrradianceNames at size - 4). Member names, order and offsets are EA's,
// from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

struct cIrradiance;

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
