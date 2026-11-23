#ifndef FE_FESTATEONLINEAGREEMENT_H
#define FE_FESTATEONLINEAGREEMENT_H

// cFEStateOnlineAgreement: the online terms-of-service screen: scrolls the 0xB2 "kT_TOS..." text lines into
// "AgTextScroll", one per frame, and accepts / declines. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the
// preview's _$_23cFEStateOnlineAgreement / onLoseTransition__23cFEStateOnlineAgreement and the allocation tag. EA's
// source file is not known (its code follows cFEStateOnlineMainMenu's, festatenetmainmenu.cpp, in both builds).
//
// Size 0x6DC: cMemMan_alloc(0x6DC, "cFEStateOnlineAgreement") + cFEStateOnlineAgreement_cFEStateOnlineAgreement(mem, pEngine) at all 4 sites
// (cFEStateCreateStep1/2's onWidgetEvent, cFEStateLogin_onWidgetEvent, cFEStateNetSSXWorld_onWidgetEvent). Constructors cFEStateOnlineAgreement_cFEStateOnlineAgreement
// (cFEStateNet(pEngine,
// 0), vtable cFEStateOnlineAgreement_vtbl, mHashName 0x43), cFEStateOnlineAgreement_cFEStateOnlineAgreement1 (cFEStateNet(pEngine, pParent), the same stores; no caller
// found) and the default cFEStateOnlineAgreement_cFEStateOnlineAgreement2 (cFEStateNet(0, 0), no mHashName); destructor cFEStateOnlineAgreement__cFEStateOnlineAgreement (slot 1: frees
// mpTOSLines). Retail vtable cFEStateOnlineAgreement_vtbl (32 slots: 4 onCreateScreen cFEStateOnlineAgreement_onCreateScreen, 7 cFEStateOnlineAgreement_onGainTransition, 10 cFEStateOnlineAgreement_onLoseTransition
// (onLoseTransition), 12 onUpdate cFEStateOnlineAgreement_onUpdate, 18 onWidgetCreate cFEStateOnlineAgreement_onWidgetCreate, 19 onWidgetEvent cFEStateOnlineAgreement_onWidgetEvent, 20
// cFEStateOnlineAgreement_onWidgetMenu, 25 func_001C0EA8 (help), 31 func_001C0E88). Methods: fe/feasyncfile 0x1C03D0-0x1C0EA8.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
// these classes (On Tour has no online FE), so the own fields are named from their retail use unless noted.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (its default constructors, destructors and inline accessors, see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (onPopupEvent / the cFEStateNet popup handlers): 0xF + i = option i chosen, 0x14 = back, 0x16 = the
// popup closed (fe/screenpopup.h). The popup id is the cScreenPopup's miMiscData (+ 0x18), set to 0x8xx by its
// creator.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cFEStateOnlineAgreement : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOnlineAgreement_vtbl (vtables.tsv; 32 slots), stored by
    //       cFEStateOnlineAgreement_cFEStateOnlineAgreement / cFEStateOnlineAgreement_cFEStateOnlineAgreement1 /
    //       cFEStateOnlineAgreement_cFEStateOnlineAgreement2 after the cFEStateNet ctor and by the destructor
    //       cFEStateOnlineAgreement__cFEStateOnlineAgreement
    // cFEStateOnlineAgreement's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateOnlineAgreement_cFEStateOnlineAgreement /
    //       cFEStateOnlineAgreement_cFEStateOnlineAgreement1 store 0x43
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateOnlineAgreement
    int* mpTOSLines;                // 0x6D0 [R] 0 (all three ctors); [U] func_001C0540 frees it and stores
                                    //       operator_new_tag(miNumTOSLines * 4, "cFEStateOnlineAgreement") filled
                                    //       with the hashes of "kT_TOS000".."kT_TOS165" (with the a/b/.. variants);
                                    //       onUpdate cFEStateOnlineAgreement_onUpdate hands mpTOSLines[miNextTOSLine++] to
                                    //       "AgTextScroll" (cUITextScroll_addUnicodeStringByID); the destructor frees it
    unsigned int miNumTOSLines;     // 0x6D4 [U] func_001C0540 stores 0xB2; onUpdate compares miNextTOSLine with it
                                    //       (sltu)
    unsigned int miNextTOSLine;     // 0x6D8 [R] 0 (all three ctors); [U] onUpdate cFEStateOnlineAgreement_onUpdate post-increments it
                                    //       per line added
};

CHECK_OFFSET(cFEStateOnlineAgreement, mpTOSLines, 0x6D0);
CHECK_OFFSET(cFEStateOnlineAgreement, miNumTOSLines, 0x6D4);
CHECK_OFFSET(cFEStateOnlineAgreement, miNextTOSLine, 0x6D8);
CHECK_SIZE(cFEStateOnlineAgreement, 0x6DC);

#endif /* FE_FESTATEONLINEAGREEMENT_H */
