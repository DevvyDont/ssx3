#ifndef UTIL_SURFPROP_H
#define UTIL_SURFPROP_H

// cSurfPropCache: the snow-surface physics table, one tSurfData per surface type (cGame::SurfPropCache). EA's header:
// d:/ssx/code/source/util/surfprop.h ([P] the July 2003 preview's assert path, with "type >= 0 && type < eST_Num" from
// the inline getSurfData). Methods in worldpainter/quadtree (func_002C2F18 init, cSurfPropCache_purge).
// Size 0xD10 = 19 x 0xB0: [R] cGame_loadTrack: new (0xD10, "SurfPropCache") with the 19-row array ctor (view sTbl_F6B0 /
// sRow_F6B0 in main/gameload); [O] On Tour class cSurfPropCache (0xD10: tSurfData SurfData[19]; getSurfData(eSurfaceType)
// returns const tSurfData*). [U] the motions (cAirMotion_step / _resolve, cGroundMotion_resolve) index it by the hit's
// surface type x 0xB0 and read tSurfData members.
// Tags: [O] On Tour's DWARF; [P] preview assert; [R] retail allocation; [U] retail use.

#include "layout.h"
#include "ai/rider.h"   // tSurfData

struct cSurfPropCache {
    tSurfData SurfData[19];         // 0x000 [O]; [P] getSurfData's "type < eST_Num"; [R] 19 rows of 0xB0; [U] indexed by
                                    //       the surface type (eSurfaceType)
};

CHECK_OFFSET(cSurfPropCache, SurfData, 0x000);
CHECK_SIZE(cSurfPropCache, 0xD10);

#endif /* UTIL_SURFPROP_H */
