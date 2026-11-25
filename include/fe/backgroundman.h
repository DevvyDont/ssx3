#ifndef FE_BACKGROUNDMAN_H
#define FE_BACKGROUNDMAN_H

// cBackgroundMan: a set of background images (texture IDs) of which one is active. Methods in
// camera/trigger/cameratriggerfactory and main/loadscreens.
//
// Size 0xC (On Tour's; retail's methods reach miActiveImage at size - 4). Member names, order and offsets are EA's,
// from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"


struct cBackgroundMan {
    int* mpiImageTID;               // 0x000 [O]
    int miNumImage;                 // 0x004 [O]
    int miActiveImage;              // 0x008 [O]

    // Methods bound to their retail symbols by asm labels.
    int setActiveImage(int i) __asm__("cBackgroundMan_setActiveImage");
    int loadImages() __asm__("cBackgroundMan_loadImages");
    void renderImage() __asm__("cBackgroundMan_renderImage");
};

CHECK_OFFSET(cBackgroundMan, mpiImageTID, 0x000);
CHECK_OFFSET(cBackgroundMan, miNumImage, 0x004);
CHECK_OFFSET(cBackgroundMan, miActiveImage, 0x008);
CHECK_SIZE(cBackgroundMan, 0xC);

#endif
