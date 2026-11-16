#ifndef UTIL_LIST_H
#define UTIL_LIST_H

// EA's intrusive doubly linked list (retail unit util/list: cListNode_isSentinel .. cList_traverseRecurse,
// 0x397718-0x397AD8; the inline members are out of line in ui/uivector 0x3A47A8-0x3A4DA8 and render/font).
// Every UI object (cUIThing and all widgets) is a cListNode; cUIGroup, cUIMenu, cUITextScroll and cUIScreen own a
// cList of their children. EA's source file is not known (On Tour: ui/all_in_one_ui.cpp); util/list.h names the unit.
//
//   cListNode (0xC)       mpPrev 0x0, mpNext 0x4, vptr 0x8 (_vt$9cListNode D_00494CC0: ~cListNode cListNode__cListNode,
//                         traverseFlat func_00397788, traverseRecurse func_003977B8). Ctor cListNode_cListNode self-links it.
//   cList (0x1C)          two sentinel nodes mStart (0x0) and mEnd (0xC), vptr 0x18 (cList_vtbl D_00494C98).
//                         Ctor func_003977E8 (also points both sentinels' vptrs at D_00494CC0), clear() func_00397930.
//   cListOperator (0x4)   vptr only (cListOperator_vtbl D_00494CE8): the functor cList/cListNode traverse* call per node.
//   cListOpOnInput (0x8)  + mpInput;  cListOpSetFocus (0x8) + mbState.
// Sizes: [R] cList is embedded in cUIGroup at 0x74 (alloc 0x90 = 0x74 + 0x1C) and cUITextScroll at 0x8C; their
// destructors restore cList_vtbl at +0x18 and cListNode's at +0x14 / +0x8 (cList__cList, cUIGroup__cUIGroup); every
// UI object's vptr sits at 0x8 after cListNode's two links. On Tour's DWARF (cListNode 0xC, cList 0x1C, cListOperator 0x4,
// cListOpOnInput 0x8, cListOpSetFocus 0x8) has the same members at the same offsets.
// NOTE: src/util/list.cpp's local `struct cListNode { next 0x0; prev 0x4 }` swaps EA's names: cList::first (cList_first,
// lw 0x4) returns mStart.mpNext, last (func_00397858, lw 0xC) returns mEnd.mpPrev, and clear() (func_00397930) stores
// mStart.mpPrev = &mStart, mStart.mpNext = &mEnd, mEnd.mpPrev = &mStart, mEnd.mpNext = &mEnd.
//
// FLAT layouts, like main/game.h (no C++ inheritance or virtuals; vptrs are `void** mVtbl` fields).

#include "layout.h"

struct cUIInput;

struct cListNode {
    cListNode* mpPrev;          // 0x0 [O]; [R] ctor cListNode_cListNode self-links it; [U] cListNode_removeFromList, clear()
                                //     func_00397930; func_003A4848 prev() (lw 0x0), func_003A4858 setPrev() (sw 0x0)
    cListNode* mpNext;          // 0x4 [O]; [R] self-linked; [U] func_003A4850 next() (lw 0x4), func_003A4860 setNext()
                                //     (sw 0x4); cList_first reads mStart.mpNext
    void** mVtbl;               // 0x8 [R] _vt$9cListNode D_00494CC0 (ctor cListNode_cListNode, dtor cListNode__cListNode)
};

CHECK_OFFSET(cListNode, mpPrev, 0x0);
CHECK_OFFSET(cListNode, mpNext, 0x4);
CHECK_OFFSET(cListNode, mVtbl, 0x8);
CHECK_SIZE(cListNode, 0xC);

struct cList {
    cListNode mStart;           // 0x00 [O] start sentinel; [R] ctor func_003977E8 / clear() func_00397930;
                                //      [U] cList_first (lw 0x4), cList_addToFront (lw/sw 0x4), cList_size, traverse*
    cListNode mEnd;             // 0x0C [O] end sentinel; [R] ditto; [U] func_00397858 last() (lw 0xC),
                                //      cList_addToEnd (lw/sw 0xC)
    void** mVtbl;               // 0x18 [R] cList_vtbl D_00494C98 (ctor func_003977E8, dtor cList__cList)
};

CHECK_OFFSET(cList, mStart, 0x00);
CHECK_OFFSET(cList, mEnd, 0x0C);
CHECK_OFFSET(cList, mVtbl, 0x18);
CHECK_SIZE(cList, 0x1C);

struct cListOperator {
    void** mVtbl;               // 0x0 [R] cListOperator_vtbl D_00494CE8 (dtor cListOperator__cListOperator); a class
                                //     with no data puts its vptr first
};

CHECK_OFFSET(cListOperator, mVtbl, 0x0);
CHECK_SIZE(cListOperator, 0x4);

struct cListOpOnInput {
    // --- cListOperator
    void** mVtbl;               // 0x0 [R] its own vtable, then cListOperator_vtbl in the dtor cListOpOnInput__cListOpOnInput
    // --- cListOpOnInput
    const cUIInput* mpInput;    // 0x4 [O]; [U] cListOpOnInput_onListNode passes it to each node's onInput (slot 14)
};

CHECK_OFFSET(cListOpOnInput, mVtbl, 0x0);
CHECK_OFFSET(cListOpOnInput, mpInput, 0x4);
CHECK_SIZE(cListOpOnInput, 0x8);

struct cListOpSetFocus {
    // --- cListOperator
    void** mVtbl;               // 0x0 [R] its own vtable, then cListOperator_vtbl in the dtor cListOpSetFocus__cListOpSetFocus
    // --- cListOpSetFocus
    int mbState;                // 0x4 [O] bool (a word in retail: lw); [U] cListOpSetFocus_onListNode passes it to each
                                //     node's setFocus (slot 6)
};

CHECK_OFFSET(cListOpSetFocus, mVtbl, 0x0);
CHECK_OFFSET(cListOpSetFocus, mbState, 0x4);
CHECK_SIZE(cListOpSetFocus, 0x8);

#endif /* UTIL_LIST_H */
