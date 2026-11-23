#ifndef UI_UIPAIR_H
#define UI_UIPAIR_H

// cUIPair: a UI object that pairs a label ("entry") with a widget (record type 0x15; menus use it for option rows).
// Size 0x80: cUIScreen_createObjectByStruct does operator new(0x80, "UIPair") + the cUIThing ctor cUIThing_cUIThing, stores
// cUIPair_vtbl (D_00494868), constructs mAttrPair (tAttrPair_tAttrPair on this + 0x74), sets cUIThing::mAttr bit 1, zeroes
// mpEntry / mpWidget, then adopts the two children the record names. Default ctors cUIPair_cUIPair1 / cUIPair_cUIPair
// (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source:
// d:/ssx/code/source/ui/uipair.cpp (preview assert path); retail keeps its methods in ui/uimenu (0x39BAB0-0x39C240).
// Methods: vtable slots 0 ~cUIPair (cUIPair__cUIPair), 1-2 traverseFlat/traverseRecurse, 3 setActive, 6 setFocus,
// 8 setVisible, 9 setUIData, 14 onInput, 15 updateLocale, 16 render2D, 18 update, 21 getType; cUIPair_destroyPair;
// accessors cUIPair_getEntry..cUIPair_setWidget (ui/uivector).
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUIPair 0x90; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

// EA's tAttrPair (On Tour: 2 bytes; unsigned char bitfields, each pivot in its own byte).
struct tAttrPair {
    unsigned char mEntryPivot : 6;   // byte 0 [O]; [R] cleared by tAttrPair_tAttrPair (lbu/sb 0)
    unsigned char mWidgetPivot : 6;  // byte 1 [O]; [R] cleared by tAttrPair_tAttrPair (lbu/sb 1)
};

CHECK_SIZE(tAttrPair, 0x2);

struct cUIPair : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIPair_vtbl D_00494868 (createObjectByStruct, cUIPair_cUIPair1, cUIPair_cUIPair, the dtor)
    // --- cUIPair
    // cUIPair's own slots ([R] cUIPair_vtbl; types [O] On Tour's class cUIPair):
    virtual char getType();                                               // 22 cUIPair_getType__FPv
    tAttrPair mAttrPair;             // 0x074 [O] tAttrPair (2 bytes); [R] its ctor tAttrPair_tAttrPair on this + 0x74
                                     //       (createObjectByStruct, cUIPair_cUIPair1, cUIPair_cUIPair)
    char unk_076[0x2];               // 0x076 padding
    cUIThing* mpEntry;               // 0x078 [O]; [R] = 0, then the record's first child (hash rec+0x20);
                                     //       [U] cUIPair_getEntry getEntry (lw), cUIPair_setEntry setEntry (deletes the old one);
                                     //       cUIPair_destroyPair, setActive/setVisible/setFocus, traverseRecurse, render2D, update
    cUIThing* mpWidget;              // 0x07C [O]; [R] = 0, then the second child (its mAttr bit 0 cleared);
                                     //       [U] cUIPair_getWidget getWidget (lw), cUIPair_setWidget setWidget; the same methods
};

CHECK_OFFSET(cUIPair, mAttrPair, 0x074);
CHECK_OFFSET(cUIPair, unk_076, 0x076);
CHECK_OFFSET(cUIPair, mpEntry, 0x078);
CHECK_OFFSET(cUIPair, mpWidget, 0x07C);
CHECK_SIZE(cUIPair, 0x80);

#endif /* UI_UIPAIR_H */
