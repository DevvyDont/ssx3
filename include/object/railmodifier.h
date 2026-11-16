#ifndef OBJECT_RAILMODIFIER_H
#define OBJECT_RAILMODIFIER_H

// cRailModifier: one grindable rail spline attached to a placed instance (a cMoveNode's rail list / a replay
// tModifierBlock's list). It keeps the spline's world bound box in the Nash rail-space octree through a 0x10-byte
// cRailSpaceNode ("RailNode", whose Rail field points back here) and answers rider rail queries (splineIntersect).
// Size 0x90: retail cMemMan_alloc(0x90, "RailModifier") (src/object/movenode.cpp, src/object/modifierblock.cpp),
// On Tour 0x90. EA's source path: d:/ssx/code/source/object/railmodifier.cpp (preview assert).
// Constructors cRailModifier_cRailModifier (tAddRailData, tInstanceRecord*) and cRailModifier_cRailModifier1 (replay
// frame), destructor cRailModifier__cRailModifier (all in src/object/splinemodifier.cpp); vtable cRailModifier_vtbl (0x004911D0,
// 4 slots in this order: ~cRailModifier cRailModifier__cRailModifier, writeToReplayFrame cRailModifier_writeToReplayFrame, update cRailModifier_update,
// render cRailModifier_render); base vtable cRailLink_vtbl (0x00491200: ~cRailLink, writeToReplayFrame).
// The bases are staged as their own headers: object/raillink.h (cRailLink) and object/railspacenode.h (the Node).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. The real bases are
//   cRailLink (0x00-0x0C: Next, Prev, vptr) + cRailModifierState (0x10-0x40), then cRailModifier's own fields.
// Evidence tags:
//   [R] retail constructors / destructor and the 0x90 allocation;
//   [U] how retail code uses the field (C bodies and asm);
//   [O] SSX On Tour's DWARF: cRailLink {Next 0x0, Prev 0x4} (0xC), cRailModifierState {BoundBox 0x0, SplineId 0x20,
//       Mesh 0x24} (0x30), cRailModifier {Instance 0x40, Node 0x44, XForm 0x50} (0x90); On Tour's
//       writeToReplayFrame writes 0x30 bytes from +0x10 (the state) and then Instance, as retail's cRailModifier_writeToReplayFrame.
//       Every On Tour offset equals retail.
// tBoundBox comes from world/worldview.h until it moves to a bx header.

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct cRailLink;
struct cRailSpaceNode;
struct tInstanceRecord;

struct cRailModifier {
    // --- cRailLink (On Tour 0xC)
    cRailLink* Next;              // 0x00 [O]; [R] both ctors zero it; [U] cRailLink_append / func_0035B6D0 (unlink,
                                  //      called by the destructor) walk it
    cRailLink* Prev;              // 0x04 [O]; [R] both ctors zero it
    void** mVtbl;                 // 0x08 [R] ctors store cRailModifier_vtbl; the destructor stores it, then
                                  //      cRailLink_vtbl before unlinking
    char unk_0C[0x4];             // 0x0C padding (BoundBox is 16-byte aligned)
    // --- cRailModifierState (On Tour 0x30; [R] cRailModifier_cRailModifier1 reads and cRailModifier_writeToReplayFrame writes these
    //     0x30 bytes from +0x10)
    tBoundBox BoundBox;           // 0x10 [O]; [R] ctor: the spline's box padded by 100 on each side;
                                  //      [U] cRailModifier_updateBoundBox writes it (sq 0x10/0x20), update
                                  //      (cRailModifier_update) passes old and new box to nash_Nash_movenode
    unsigned int SplineId;        // 0x30 [O] tResourceID (low 8 bits: resource set, >> 8: index); [R] ctor: -1, then
                                  //      the descriptor's id (+0x4); [U] splineIntersect (cRailModifier_splineIntersect) and the
                                  //      ctors/destructor look the spline up through it
    int Mesh;                     // 0x34 [O]; [R] ctor: descriptor + 0x8; [U] buildXform: < 0 = the instance's own
                                  //      matrix, else a link (bone) index; getInvMat (cRailModifier_getInvMat) passes it on
    char unk_38[0x8];             // 0x38 padding to the state's 0x30 size
    // --- cRailModifier
    tInstanceRecord* Instance;    // 0x40 [O]; [R] ctor stores the instance argument, ctor1 reads it from the frame;
                                  //      [U] buildXform (model matrix +0x10, link table +0x80, scale +0x84),
                                  //      updateBoundBox, getInvMat, splineIntersect, writeToReplayFrame
    cRailSpaceNode* Node;         // 0x44 [O]; [U] buildXform allocates it (0x10 bytes, "RailNode": +0x8 = 2, +0xC =
                                  //      this); ctors insert it into the Nash octree, update moves it, the
                                  //      destructor removes and frees it
    char unk_48[0x8];             // 0x48 padding (XForm is 16-byte aligned)
    t4Matrix XForm;               // 0x50 [O]; [U] buildXform writes the inverse rail transform (sqc2 0x50-0x80,
                                  //      transposes lwc1/swc1 0x54-0x74); getInvMat copies it (lqc2 0x50-0x80)
};

CHECK_OFFSET(cRailModifier, Next, 0x00);
CHECK_OFFSET(cRailModifier, Prev, 0x04);
CHECK_OFFSET(cRailModifier, mVtbl, 0x08);
CHECK_OFFSET(cRailModifier, unk_0C, 0x0C);
CHECK_OFFSET(cRailModifier, BoundBox, 0x10);
CHECK_OFFSET(cRailModifier, SplineId, 0x30);
CHECK_OFFSET(cRailModifier, Mesh, 0x34);
CHECK_OFFSET(cRailModifier, unk_38, 0x38);
CHECK_OFFSET(cRailModifier, Instance, 0x40);
CHECK_OFFSET(cRailModifier, Node, 0x44);
CHECK_OFFSET(cRailModifier, unk_48, 0x48);
CHECK_OFFSET(cRailModifier, XForm, 0x50);
CHECK_SIZE(cRailModifier, 0x90);

#endif /* OBJECT_RAILMODIFIER_H */
