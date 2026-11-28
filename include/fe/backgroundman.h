#ifndef FE_BACKGROUNDMAN_H
#define FE_BACKGROUNDMAN_H

// cBackgroundMan: a set of background images (texture IDs) of which one is active. Methods in
// camera/trigger/cameratriggerfactory and main/loadscreens.
//
// Size 0xC (On Tour's; retail's methods reach miActiveImage at size - 4). Member names, order and offsets are EA's,
// from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"


void operator_delete(int* p);

struct cBackgroundMan {
    int* mpiImageTID;               // 0x000 [O]
    int miNumImage;                 // 0x004 [O]
    // No active image (name ours): [R] the constructor stores -1; [U] setActiveImage stores it for an index out of
    // range, and the debug menu draws the image instead of itself only while miActiveImage >= 0.
    enum { kNoImage = -1 };

    int miActiveImage;              // 0x008 [O]; kNoImage or an index below miNumImage

    // Methods bound to their retail symbols by asm labels.
    int setActiveImage(int i) __asm__("cBackgroundMan_setActiveImage");
    bool loadImages() __asm__("cBackgroundMan_loadImages");
    void renderImage() __asm__("cBackgroundMan_renderImage");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    ~cBackgroundMan();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cBackgroundMan, mpiImageTID, 0x000);
CHECK_OFFSET(cBackgroundMan, miNumImage, 0x004);
CHECK_OFFSET(cBackgroundMan, miActiveImage, 0x008);
CHECK_SIZE(cBackgroundMan, 0xC);

#endif
