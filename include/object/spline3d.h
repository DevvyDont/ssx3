#ifndef OBJECT_SPLINE3D_H
#define OBJECT_SPLINE3D_H

// cSpline: a follower on a world spline resource (current segment and arc length), embedded by value in
// cSplineModifier (+0xD8) and cMultiSplineModifier (+0x48). Size 0x10: On Tour 0x10; retail ctors initialise 0x0..0xC and
// the next field / end of the owner follows. EA's source: d:/ssx/code/source/object/spline3d.cpp (preview assert).
// Methods (src/object/spline3d.cpp): cSpline_calcLength (0x3454E8), cSpline_writeToReplayFrame (0x345538),
// cSpline_readFromReplayFrame (0x345570; reads 8 bytes = SplineId + SegIndex, then walks SegIndex segments).
// On Tour names only Seg and Length (0x0/0x4 come from an unnamed base or were dropped).

#include "layout.h"

struct tSplineSegmentRecord;  // world/worldrecords.h

struct cSpline {
    unsigned int SplineId;          // 0x00 [R] owners' ctors store -1; [U] readFromReplayFrame resolves it (low 8 bits:
                                    //      resource set, >> 8: index) to the spline whose +0x24 is the first segment
    int SegIndex;                   // 0x04 [R] ctors store 0; [U] readFromReplayFrame steps Seg = Seg->next (+0x64) this
                                    //      many times
    tSplineSegmentRecord* Seg;      // 0x08 [O] Seg (tSplineSegmentRecord*); [U] calcLength walks it; cSpline_draw reads Seg->m_Parent
    float Length;                   // 0x0C [O]; [U] readFromReplayFrame stores calcLength()
};

CHECK_OFFSET(cSpline, SplineId, 0x00);
CHECK_OFFSET(cSpline, SegIndex, 0x04);
CHECK_OFFSET(cSpline, Seg, 0x08);
CHECK_OFFSET(cSpline, Length, 0x0C);
CHECK_SIZE(cSpline, 0x10);

#endif /* OBJECT_SPLINE3D_H */
