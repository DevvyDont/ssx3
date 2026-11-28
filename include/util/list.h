#ifndef UTIL_LIST_H
#define UTIL_LIST_H
#pragma interface

// EA's intrusive doubly linked list (retail unit util/list: cListNode_isSentinel .. cList_traverseRecurse,
// 0x397718-0x397AD8; the inline members are out of line in ui/uivector 0x3A47A8-0x3A4DA8 and render/font).
// Every UI object (cUIThing and all widgets) is a cListNode; cUIGroup, cUIMenu, cUITextScroll and cUIScreen own a
// cList of their children. EA's source file is not known (On Tour: ui/all_in_one_ui.cpp); util/list.h names the unit.
//
//   cListNode (0xC)       mpPrev 0x0, mpNext 0x4, vptr 0x8 (_vt$9cListNode cListNode_vtbl: ~cListNode cListNode__cListNode,
//                         traverseFlat cListNode_traverseFlat, traverseRecurse cListNode_traverseRecurse). Ctor cListNode_cListNode self-links it.
//   cList (0x1C)          two sentinel nodes mStart (0x0) and mEnd (0xC), vptr 0x18 (cList_vtbl D_00494C98).
//                         Ctor cList_cList (also points both sentinels' vptrs at cListNode_vtbl), clear() cList_clear.
//   cListOperator (0x4)   vptr only (cListOperator_vtbl D_00494CE8): the functor cList/cListNode traverse* call per
//                         node (onListNode, pure).
//   cListOpOnInput (0x8)  + mpInput;  cListOpSetFocus (0x8) + mbState.
// Sizes: [R] cList is embedded in cUIGroup at 0x74 (alloc 0x90 = 0x74 + 0x1C) and cUITextScroll at 0x8C; their
// destructors restore cList_vtbl at +0x18 and cListNode's at +0x14 / +0x8 (cList__cList, cUIGroup__cUIGroup); every
// UI object's vptr sits at 0x8 after cListNode's two links. On Tour's DWARF (cListNode 0xC, cList 0x1C, cListOperator 0x4,
// cListOpOnInput 0x8, cListOpSetFocus 0x8) has the same members at the same offsets.
// NOTE: src/util/list.cpp's local `struct cListNode { next 0x0; prev 0x4 }` swaps EA's names: cList::first (cList_first,
// lw 0x4) returns mStart.mpNext, last (cList_last, lw 0xC) returns mEnd.mpPrev, and clear() (cList_clear) stores
// mStart.mpPrev = &mStart, mStart.mpNext = &mEnd, mEnd.mpPrev = &mStart, mEnd.mpNext = &mEnd.
//
// Real polymorphic classes (each vptr follows the class's own data, as ee-gcc 2.95.3 places it). `#pragma
// interface`: no unit emits their vtables or out-of-line copies of the inline members; the equates after each class
// bind g++'s vtable names to the retail tables. The destructors are EA's inline ones ([R] cUIGroup's destructor
// inlines ~cList and ~cListNode; cUIGroup_setFocus / cUIGroup_onInput inline the operators' constructors and
// destructors); retail's out-of-line copies (cListNode__cListNode, cList__cList, cListOperator__cListOperator,
// cListOpOnInput__cListOpOnInput, cListOpSetFocus__cListOpSetFocus in ui/uivector) stay C functions. So do the
// destructors that inline ~cList / ~cListNode for an embedded member (cUIGroup, cUIMenu, cUITextScroll, cUIScreen):
// ee-gcc 2.95.3 inlines a member's destructor through a register copy of its address (addu; sw 8(reg)), where retail
// stores relative to the outer object (cList__cList: sw 0x14($18) for mEnd).

#include "layout.h"
#include "bx/memman.h"

struct cUIInput;
struct cListOperator;

class cListNode {
public:
    cListNode* mpPrev;          // 0x0 [O]; [R] ctor cListNode_cListNode self-links it; [U] cListNode_removeFromList, clear()
                                //     cList_clear; cListNode_prev prev() (lw 0x0), cListNode_setPrev setPrev() (sw 0x0)
    cListNode* mpNext;          // 0x4 [O]; [R] self-linked; [U] cListNode_next next() (lw 0x4), cListNode_setNext setNext()
                                //     (sw 0x4); cList_first reads mStart.mpNext
    // 0x8: the vptr. [R] _vt$9cListNode cListNode_vtbl (ctor cListNode_cListNode, dtor cListNode__cListNode); slots
    // [R] that table, names and types [O]:
    virtual ~cListNode() { destroyListNode(); }                  // 1 inline (copy: cListNode__cListNode)
    virtual void* traverseFlat(cListOperator* pOperator)
        __asm__("cListNode_traverseFlat");        // 2 cListNode_traverseFlat
    virtual void* traverseRecurse(cListOperator* pOperator)
        __asm__("cListNode_traverseRecurse");     // 3 cListNode_traverseRecurse

    // Methods bound to their retail symbols by asm labels.
    cListNode* next() __asm__("cListNode_next");
    void setNext(cListNode* next) __asm__("cListNode_setNext");
    void destroyListNode() __asm__("cListNode_destroyListNode");
    bool isSentinel() __asm__("cListNode_isSentinel__FP9cListNode");
    void removeFromList() __asm__("cListNode_removeFromList");
};

CHECK_OFFSET(cListNode, mpPrev, 0x0);
CHECK_OFFSET(cListNode, mpNext, 0x4);
CHECK_SIZE(cListNode, 0xC);
__asm__("_vt$9cListNode = cListNode_vtbl");   // g++'s name for the retail vtable

class cList {
public:
    cListNode mStart;           // 0x00 [O] start sentinel; [R] ctor cList_cList / clear() cList_clear;
                                //      [U] cList_first (lw 0x4), cList_addToFront (lw/sw 0x4), cList_size, traverse*
    cListNode mEnd;             // 0x0C [O] end sentinel; [R] ditto; [U] cList_last last() (lw 0xC),
                                //      cList_addToEnd (lw/sw 0xC)
    // 0x18: the vptr. [R] cList_vtbl D_00494C98 (ctor cList_cList, dtor cList__cList); slots [R], names [O]:
    virtual ~cList() { destroyList(); }                          // 1 inline (copy: cList__cList); then
                                                                 //   ~mEnd and ~mStart
    virtual void* traverseFlat(cListOperator* pOperator) __asm__("cList_traverseFlat");        // 2 cList_traverseFlat
    virtual void* traverseRecurse(cListOperator* pOperator)
        __asm__("cList_traverseRecurse");     // 3 cList_traverseRecurse

    // Methods bound to their retail symbols by asm labels.
    cListNode* first() __asm__("cList_first__FP5cList");
    int * last() __asm__("cList_last");
    cListNode* getByIndex(int count) __asm__("cList_getByIndex");
    void addToFront(cListNode* newNode) __asm__("cList_addToFront__FP5cListP9cListNode");
    void addToEnd(cListNode* newNode) __asm__("cList_addToEnd__FP5cListP9cListNode");
    void destroyList() __asm__("cList_destroyList");
    int size() __asm__("cList_size");
    void clear() __asm__("cList_clear__FPv");
};

CHECK_OFFSET(cList, mStart, 0x00);
CHECK_OFFSET(cList, mEnd, 0x0C);
CHECK_SIZE(cList, 0x1C);
__asm__("_vt$5cList = cList_vtbl");   // g++'s name for the retail vtable

class cListOperator {
public:
    // 0x0: the vptr (a class with no data puts it first). [R] cListOperator_vtbl D_00494CE8; slots [R], names and
    // types [O] (On Tour class cListOperator):
    virtual ~cListOperator() {}                                  // 1 inline (copy: cListOperator__cListOperator)
    virtual void* onListNode(cListNode* pNode) = 0;              // 2 __pure_virtual
};

CHECK_SIZE(cListOperator, 0x4);
__asm__("_vt$13cListOperator = cListOperator_vtbl");   // g++'s name for the retail vtable

class cListOpOnInput : public cListOperator {
public:
    // cListOperator: the vptr 0x0 ([R] cListOpOnInput_vtbl; cListOperator_vtbl again in the implicit destructor,
    // retail's copy cListOpOnInput__cListOpOnInput)
    const cUIInput* mpInput;    // 0x4 [O]; [U] onListNode passes it to each node's onInput (slot 15)

    // [O] EA's inline constructor ([R] inlined in cUIGroup_onInput).
    cListOpOnInput(const cUIInput* input) : mpInput(input) {}
    virtual void* onListNode(cListNode* pNode) __asm__("cListOpOnInput_onListNode");   // 2
};

CHECK_OFFSET(cListOpOnInput, mpInput, 0x4);
CHECK_SIZE(cListOpOnInput, 0x8);
__asm__("_vt$14cListOpOnInput = cListOpOnInput_vtbl");   // g++'s name for the retail vtable

class cListOpSetFocus : public cListOperator {
public:
    // cListOperator: the vptr 0x0 ([R] cListOpSetFocus_vtbl; cListOperator_vtbl again in the implicit destructor,
    // retail's copy cListOpSetFocus__cListOpSetFocus)
    int mbState;                // 0x4 [O] bool (a word in retail: lw); [U] onListNode passes it to each node's
                                //     setFocus (slot 7)

    // [O] EA's inline constructor ([R] inlined in cUIGroup_setFocus).
    cListOpSetFocus(int b) : mbState(b) {}
    virtual void* onListNode(cListNode* pNode) __asm__("cListOpSetFocus_onListNode");   // 2
};

CHECK_OFFSET(cListOpSetFocus, mbState, 0x4);
CHECK_SIZE(cListOpSetFocus, 0x8);
__asm__("_vt$15cListOpSetFocus = cListOpSetFocus_vtbl");   // g++'s name for the retail vtable

#endif /* UTIL_LIST_H */
