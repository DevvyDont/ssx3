#ifndef UI_UIGROUP_H
#define UI_UIGROUP_H

// cUIGroup: a UI object that owns child objects (record type 0x10; also the root group of every cUIScreen and the base
// of cUIMenu). Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "cUIGroup") + the cUIThing ctor
// cUIThing_cUIThing, stores cUIGroup_vtbl (D_004949E8) and constructs mList (cList_cList on this + 0x74); the default
// ctors cUIGroup_cUIGroup1 / cUIGroup_cUIGroup (ui/uivector) do the same. Base: cUIThing (ui/uithing.h, 0x74) <- cListNode,
// written out flat below.
// Methods: vtable slots 0 ~cUIGroup (cUIGroup__cUIGroup), 1-2 traverseFlat/traverseRecurse, 6 setFocus, 9 setUIData,
// 13 getObjectByHashName, 14 onInput, 15 updateLocale, 16 render2D, 17 render3D, 18 update (ui/uiengine
// 0x398618-0x398998), 21 getType; the list wrappers cUIGroup_first..cUIGroup_addToEnd (ui/uivector). cUIMenu (ui/uimenu.h)
// derives from it and reuses most of these slots.
// EA's source file is not known (On Tour: ui/all_in_one_ui.cpp); ui/uigroup.h is our name.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Member: SSX On Tour's DWARF (class cUIGroup 0xA0: cList mList at 0x80 behind its
// 0x80-byte cUIThing).

#include "layout.h"
#include "ui/uithing.h"
#include "util/list.h"

struct cUIGroup : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIGroup_vtbl D_004949E8 (createObjectByStruct, cUIGroup_cUIGroup1, cUIGroup_cUIGroup, the dtor; cUIMenu /
    //       cUIScreen dtors restore it)
    // --- cUIGroup
    // cUIGroup's own slots ([R] cUIGroup_vtbl; types [O] On Tour's class cUIGroup):
    virtual char getType();                                               // 22 cUIGroup_getType__FPv
    cList mList;                     // 0x074 [O] the children (util/list.h, 0x1C); [R] ctor cList_cList on this + 0x74
                                     //       (createObjectByStruct, cUIGroup_cUIGroup1, cUIGroup_cUIGroup), dtor cUIGroup__cUIGroup
                                     //       (cList_destroyList on this + 0x74, vptrs 0x8C / 0x88 / 0x7C);
                                     //       [U] cUIGroup_first..cUIGroup_addToEnd (first/last/getByIndex/addToFront/addToEnd
                                     //       on this + 0x74), every cUIGroup_* walker
};

CHECK_OFFSET(cUIGroup, mList, 0x074);
CHECK_SIZE(cUIGroup, 0x90);

#endif /* UI_UIGROUP_H */
