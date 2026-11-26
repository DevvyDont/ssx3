#ifndef VISUALFX_SNOWFALL_H
#define VISUALFX_SNOWFALL_H

// cSnowFall: the falling snow. A scene node (cObjNode) that owns up to twelve cSnowBox flake boxes (visualfx/snowbox.h),
// six per view (four flake boxes and two "fluff" boxes; the second view's six only with two players). update feeds
// every box its view's snow settings (render-state queries 6/7, or the debug cSnowfallToggles); render moves the
// current view's boxes by the camera's movement since the last frame and draws them. cGame_load creates the only
// instance in bucket 2 (cGame::SnowFall, main/game.h +0x74; cGame_getSnowFall returns it).
// EA's source: d:/ssx/code/source/visualfx/snowfall.cpp (July 2003 preview assert strings). Retail methods: unit
// visualfx/boardwakefx (0x002E5920 ctor .. 0x002E62C8 readFromReplayFrame) and visualfx/worldlightman (0x002F8168,
// the empty writeToReplayFrame__FPv).
//
// Size 0x60: [R] cGame_load does operator new(0x60, "SnowFall") + cSnowFall_cSnowFall(p, 2) (src/main/gameload.cpp);
// the ctor writes the vptr, the twelve box pointers and both vectors (sq). On Tour: 0x60 as well (its cObjNode is
// 0xC, but t4Vector's 16-byte alignment puts mLastCameraPosition at 0x40 in both builds; mSnowBoxes is On Tour 0xC,
// retail 0x10).
// Base cObjNode (object/objnode.h; retail 0x10: unk_00, Next, Prev, vptr);
// the ctor runs cObjNode_cObjNode1(self, bucket) and stores cSnowFall_vtbl (0x00487FD8, 13 cObjNode
// slots: 1 dtor cSnowFall__cSnowFall, 2 update, 4 render, 12 writeToReplayFrame__FPv, 13 readFromReplayFrame; the rest
// are cObjNode's defaults). cGame's serialize / load call slots 12 / 13 through the vptr at 0xC.
//
// Derives from cObjNode; its own members FLAT, like main/game.h. Evidence tags:
//   [R] retail ctor cSnowFall_cSnowFall, dtor cSnowFall__cSnowFall and the 0x60 allocation;
//   [P] tools/layout_xlate.py pairs four preview methods with retail (only the dtor's vptr store at 0xC to measure:
//       preview == retail); no inline accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cSnowFall 0x60: mSnowBoxes[12] (kMaxNumSnowBoxes), mLastCameraPosition[2]).

#include "layout.h"
#include "object/objnode.h"
#include "bx/math3d.h"

struct cObjNode;
struct cSnowBox;

struct cSnowFall : public cObjNode {
    // cObjNode (object/objnode.h, 0x10): the vptr at 0x0C:
    //   [R] cSnowFall_vtbl (ctor and dtor)
    // --- cSnowFall
    cSnowBox* mSnowBoxes[12];           // 0x10 [O] (kMaxNumSnowBoxes = 12); [R] the ctor zeroes all twelve, then
                                        //      allocates [0..5] (view 0) and, with two players, [6..11] (view 1) as
                                        //      0xC0-byte "SnowBox"es; [U] the dtor deletes the non-null ones, update /
                                        //      render / readFromReplayFrame walk all twelve (a box's mViewNum picks its
                                        //      view)
    t4Vector mLastCameraPosition[2];    // 0x40 [O]; [R] ctor (0, 0, 0, 1) each; [U] render: indexed by the current view,
                                        //      the view's camera position (+0x20 of its cGame::ViewMan entry) minus it
                                        //      is the boxes' move this frame, then it is overwritten

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cSnowFall__cSnowFall stays a C function (g++ 2.95 takes no asm label on a destructor);
    // declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cSnowFall();
    virtual void update() __asm__("cSnowFall_update");
    virtual void render() __asm__("cSnowFall_render");
};

CHECK_OFFSET(cSnowFall, mSnowBoxes, 0x10);
CHECK_OFFSET(cSnowFall, mLastCameraPosition, 0x40);
CHECK_SIZE(cSnowFall, 0x60);

#endif /* VISUALFX_SNOWFALL_H */
