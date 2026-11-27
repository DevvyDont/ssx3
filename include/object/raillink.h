#ifndef OBJECT_RAILLINK_H
#define OBJECT_RAILLINK_H

// cRailLink: the intrusive doubly-linked list node that chains a placed instance's rail modifiers (the base of
// cRailModifier; tModifierBlock embeds one by value at +0x1C as the list head). Size 0xC: On Tour's DWARF (0xC), retail
// cRailModifier's state starts at 0x10 (16-byte aligned) and tModifierBlock (0x28) ends 0xC after its +0x1C head.
// EA's source: the methods sit just before cRailModifier's constructors (0x35B670..0x35B708), i.e. in
// d:/ssx/code/source/object/railmodifier.cpp; EA's header name is unknown (railmodifier.h is as likely as this file).
// Methods: cRailLink_add (0x35B670), cRailLink_append (0x35B690), unlink cRailLink_unlink (all src/object/splinemodifier.cpp),
// the destructor cRailLink__cRailLink (0x360D40) and writeToReplayFrame (0x360D90, empty) in src/object/railmodifier.cpp;
// vtable cRailLink_vtbl (0x00491200, 2 slots: ~cRailLink, writeToReplayFrame). The constructor is inline (the derived
// ctors and tModifierBlock_tModifierBlock zero Next/Prev and store cRailLink_vtbl at +0x8).
// Evidence tags: [O] On Tour cRailLink {Next 0x0, Prev 0x4} (0xC); [R] retail ctors/destructor; [U] retail use.

#include "layout.h"

struct cBxReplayFrame;

struct cRailLink;

struct cRailLink {
    cRailLink* Next;                // 0x00 [O]; [U] cRailLink_add/append walk and patch it, unlink (cRailLink_unlink)
                                    //      clears it
    cRailLink* Prev;                // 0x04 [O]; [U] cRailLink_add sets the new node's Prev; unlink patches Next->Prev
    // 0x08: the vptr. [R] cRailLink_vtbl (0x00491200), stored by the destructor and by every derived dtor before
    //   unlinking. Slots:
    virtual ~cRailLink();                                       // 1  ([R] cRailLink_vtbl; types [O])
    virtual void writeToReplayFrame(cBxReplayFrame* data) __asm__("cRailLink_writeToReplayFrame__FPv");      // 2
    void add(cRailLink* node) __asm__("cRailLink_add");
};

CHECK_OFFSET(cRailLink, Next, 0x00);
CHECK_OFFSET(cRailLink, Prev, 0x04);
CHECK_SIZE(cRailLink, 0xC);

#endif /* OBJECT_RAILLINK_H */
