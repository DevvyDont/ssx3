#ifndef MAIN_GAMEOBJNODES_H
#define MAIN_GAMEOBJNODES_H

// The three scene-node wrappers cGame_load puts around its per-frame managers (cGame::AIVisualEffectsMan,
// VisualEffectsMan and SSXStatsMan, main/game.h +0x50 / +0x54 / +0x58): each is a bare cObjNode plus a pointer to the
// manager it drives. update / render (cObjNode slots 2 and 4) forward to the manager's update / render; the deleting
// dtor deletes the manager, then runs cObjNode__cObjNode.
//   cAIVisualEffectsObjNode  bucket 6, owns a cAIVisualEffectsMan (really the cPS2AIVisualEffectsMan that
//                            cAIVisualEffectsMan_construct builds; visualfx/aivisualeffectsman.h): virtual slots 2 / 3
//                            through its vptr at +0x4, deleted through slot 1.
//   cVisualEffectsObjNode    bucket 3, owns a cVisualEffectsMan (the cPS2VisualEffectsMan from cVisualEffectsMan_construct;
//                            visualfx/visualeffectsman.h): slots 2 / 3 through its vptr at +0x44, deleted through slot 1.
//   cSSXStatsObjNode         bucket 4, owns a cSSXStatsMan (a 1-byte object from cSSXStatsMan_construct): calls the
//                            empty cSSXStatsMan_update / cSSXStatsMan_render directly; frees it with operator delete.
// EA's source path is not in the preview's assert strings; On Tour compiles all three in main/all_in_one_main.cpp and
// retail keeps their methods in splat unit main/ssxappdtor (0x00244410 .. 0x00244A18), hence main/.
//
// Size 0x14 each: [R] cGame_load does operator new(20, "AIVisualEffectsMan" / "VisualEffectsMan" / "SSXStatsMan"),
// cObjNode_cObjNode1(node, bucket), stores the vtable at 0xC and the new manager at 0x10 (the ctors are inlined there:
// src/main/gameload.cpp, view sNamedGL). On Tour: 0x10 (its cObjNode is 0xC, 4 bytes smaller; the pointer is at 0xC).
// Base cObjNode (retail 0x10: unk_00, Next, Prev, vptr; see object/instancenode.h and visualfx/transitionman.h),
// written out inline. Vtables (13 cObjNode slots: 1 dtor, 2 update, 4 render; the rest cObjNode's defaults):
// cAIVisualEffectsObjNode_vtbl 0x0047CB60, cVisualEffectsObjNode_vtbl 0x0047D070, cSSXStatsObjNode_vtbl 0x0047CAE8.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the inlined ctors in cGame_load (above), the retail dtors and the 0x14 allocations;
//   [P] tools/layout_xlate.py pairs each class's three preview methods with retail: 0xC and 0x10, preview == retail;
//       no inline accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cAIVisualEffectsObjNode::AIVisualEffectsMan, cVisualEffectsObjNode::VisualEffectsMan,
//       cSSXStatsObjNode::SSXStatsMan, each the class's only member).

#include "layout.h"

struct cObjNode;
struct cAIVisualEffectsMan;
struct cVisualEffectsMan;
struct cSSXStatsMan;

struct cAIVisualEffectsObjNode {
    // --- cObjNode (retail 0x10; object/instancenode.h)
    int unk_00;                                 // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                             // 0x04 [O] cObjNode::Next; [R] cObjNode_cObjNode1 links it into the bucket
    cObjNode* Prev;                             // 0x08 [O] cObjNode::Prev
    void** mVtbl;                               // 0x0C [R] cAIVisualEffectsObjNode_vtbl (cGame_load, dtor)
    // --- cAIVisualEffectsObjNode
    cAIVisualEffectsMan* AIVisualEffectsMan;    // 0x10 [O]; [R] cGame_load stores cAIVisualEffectsMan_construct()'s
                                                //      result; [U] update / render call its slots 2 / 3, the dtor its
                                                //      slot 1 (flags 3) when non-null
};

CHECK_OFFSET(cAIVisualEffectsObjNode, unk_00, 0x00);
CHECK_OFFSET(cAIVisualEffectsObjNode, Next, 0x04);
CHECK_OFFSET(cAIVisualEffectsObjNode, Prev, 0x08);
CHECK_OFFSET(cAIVisualEffectsObjNode, mVtbl, 0x0C);
CHECK_OFFSET(cAIVisualEffectsObjNode, AIVisualEffectsMan, 0x10);
CHECK_SIZE(cAIVisualEffectsObjNode, 0x14);

struct cVisualEffectsObjNode {
    // --- cObjNode (retail 0x10; object/instancenode.h)
    int unk_00;                                 // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                             // 0x04 [O] cObjNode::Next; [R] cObjNode_cObjNode1 links it into the bucket
    cObjNode* Prev;                             // 0x08 [O] cObjNode::Prev
    void** mVtbl;                               // 0x0C [R] cVisualEffectsObjNode_vtbl (cGame_load, dtor)
    // --- cVisualEffectsObjNode
    cVisualEffectsMan* VisualEffectsMan;        // 0x10 [O]; [R] cGame_load stores cVisualEffectsMan_construct()'s
                                                //      result; [U] update / render call its slots 2 / 3, the dtor its
                                                //      slot 1 (flags 3) when non-null
};

CHECK_OFFSET(cVisualEffectsObjNode, unk_00, 0x00);
CHECK_OFFSET(cVisualEffectsObjNode, Next, 0x04);
CHECK_OFFSET(cVisualEffectsObjNode, Prev, 0x08);
CHECK_OFFSET(cVisualEffectsObjNode, mVtbl, 0x0C);
CHECK_OFFSET(cVisualEffectsObjNode, VisualEffectsMan, 0x10);
CHECK_SIZE(cVisualEffectsObjNode, 0x14);

struct cSSXStatsObjNode {
    // --- cObjNode (retail 0x10; object/instancenode.h)
    int unk_00;                                 // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                             // 0x04 [O] cObjNode::Next; [R] cObjNode_cObjNode1 links it into the bucket
    cObjNode* Prev;                             // 0x08 [O] cObjNode::Prev
    void** mVtbl;                               // 0x0C [R] cSSXStatsObjNode_vtbl (cGame_load, dtor)
    // --- cSSXStatsObjNode
    cSSXStatsMan* SSXStatsMan;                  // 0x10 [O]; [R] cGame_load stores cSSXStatsMan_construct()'s result;
                                                //      [U] the dtor frees it (operator delete, no dtor call); update /
                                                //      render pass it to cSSXStatsMan_update / cSSXStatsMan_render
};

CHECK_OFFSET(cSSXStatsObjNode, unk_00, 0x00);
CHECK_OFFSET(cSSXStatsObjNode, Next, 0x04);
CHECK_OFFSET(cSSXStatsObjNode, Prev, 0x08);
CHECK_OFFSET(cSSXStatsObjNode, mVtbl, 0x0C);
CHECK_OFFSET(cSSXStatsObjNode, SSXStatsMan, 0x10);
CHECK_SIZE(cSSXStatsObjNode, 0x14);

#endif /* MAIN_GAMEOBJNODES_H */
