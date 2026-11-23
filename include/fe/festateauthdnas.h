#ifndef FE_FESTATEAUTHDNAS_H
#define FE_FESTATEAUTHDNAS_H

// The DNAS authentication popups of the network configuration (EA's d:/ssx/code/source/fe/festatenetconfig.cpp: the
// preview's assert at line 641 sits in cFEStateAuthDNAS's onCreateScreen, retail cFEStateAuthDNAS_onCreateScreen, and
// cFEStateAuthDNASError's code lies between it and cFEStateNetAccountMem's, whose asserts name the same file). Both
// are cFEStateNet states (fe/festatenet.h, 0x6D0) pushed by cFEStateNetConfig (fe/festatenetconfig.h); this header
// could also be merged into fe/festatenetconfig.h.
//
// cFEStateAuthDNAS: screen "205AuthenticatingDNAS": runs the DNAS authentication object and reports to its parent
// (slot 17: 0xF ok, 0x10 error with the three texts copied into cFEStateNetConfig's mpustrAuthError[], 0x14 cancel).
// EA's name: the preview's __16cFEStateAuthDNAS, onGainTransition__16cFEStateAuthDNASb, onLoseTransition__16...
// Size 0x700: cMemMan_alloc(0x700, "NetConfigAuthPopup") + cFEStateAuthDNAS_cFEStateAuthDNAS(mem, pEngine, pParent) (func_001B55B0, the
// only site). Constructors cFEStateAuthDNAS_cFEStateAuthDNAS (cFEStateNet(pEngine, pParent), vtable cFEStateAuthDNAS_vtbl, mHashName 0x44, unk_6A8 =
// 0) and the default cFEStateAuthDNAS_cFEStateAuthDNAS1 (fe/fepopupmisc); destructor cFEStateAuthDNAS__cFEStateAuthDNAS (slot 1: deletes mpAuth, frees
// mpustrText). Retail vtable cFEStateAuthDNAS_vtbl (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateAuthDNAS_onCreateScreen, 7
// cFEStateAuthDNAS_onGainTransition
// (onGainTransition), 8 cFEStateAuthDNAS_onInputBegin, 10 cFEStateAuthDNAS_onLoseTransition, 12 onUpdate cFEStateAuthDNAS_onUpdate, 18
// onWidgetCreate cFEStateAuthDNAS_onWidgetCreate, 19 onWidgetEvent cFEStateAuthDNAS_onWidgetEvent, 20 cFEStateAuthDNAS_onWidgetMenu, 24 handleEvent cFEStateAuthDNAS_handleEvent, 29
// func_001E0C28, 32 func_001E0C20).
// Methods: fe/feasyncfile 0x1B6288-0x1B6A90.
//
// cFEStateAuthDNASError: screen "207DNASerror": shows the three DNAS error texts. EA's name: the allocation tag.
// Size 0x6E8: cMemMan_alloc(0x6E8, "cFEStateAuthDNASError") + cFEStateAuthDNASError_cFEStateAuthDNASError(mem, pEngine, pParent,
// mpustrAuthError[1],
// [2], [0]) (cFEStateNetConfig's func_001B5830, the only site). Constructors cFEStateAuthDNASError_cFEStateAuthDNASError (vtable cFEStateAuthDNASError_vtbl,
// mHashName
// 0x45) and the default cFEStateAuthDNASError_cFEStateAuthDNASError1 (fe/fepopupmisc); destructor cFEStateAuthDNASError__cFEStateAuthDNASError (slot 1). Retail vtable cFEStateAuthDNASError_vtbl
// (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateAuthDNASError_onCreateScreen, 7 cFEStateAuthDNASError_onGainTransition, 10 cFEStateAuthDNASError_onLoseTransition, 12 onUpdate
// cFEStateNet's cFEStateNet_onUpdate, 18 onWidgetCreate cFEStateAuthDNASError_onWidgetCreate, 19 onWidgetEvent cFEStateAuthDNASError_onWidgetEvent, 20 cFEStateAuthDNASError_onWidgetMenu, 24
// handleEvent cFEStateAuthDNASError_handleEvent, 29 func_001E0C88, 32 func_001E0C80). Methods: fe/feasyncfile 0x1B6AB8-0x1B6EA8.
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

struct DNASMan;
#include "fe/festatenet.h"

struct cUIThing;

struct cFEStateAuthDNAS : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateAuthDNAS_vtbl (vtables.tsv), stored by cFEStateAuthDNAS_cFEStateAuthDNAS /
    //       cFEStateAuthDNAS_cFEStateAuthDNAS1 after the cFEStateNet ctor and by the destructor
    //       cFEStateAuthDNAS__cFEStateAuthDNAS
    // cFEStateAuthDNAS's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateAuthDNAS_cFEStateAuthDNAS stores 0x44
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateAuthDNAS
    DNASMan* mpAuth;                // 0x6D0 [R] 0; [U] onCreateScreen cFEStateAuthDNAS_onCreateScreen stores
                                    //       DNASMan_DNASMan(cMemMan_alloc(0xA50, "cFEStateAuthDNASCreate")) (the DNAS
                                    //       authentication object, online/dnasman.h) and starts it in the mode of the
                                    //       profile bits cBEOptionInterface_mOption; onUpdate cFEStateAuthDNAS_onUpdate polls it (its first word:
                                    //       >= 3 ready, 5 done; + 0x1C error; error texts at + 0x450/0x650/0x850),
                                    //       cFEStateAuthDNAS_onInputBegin cancels it (+ 0x14 = 1); the destructor shuts it down
                                    //       (func_002563A8) and deletes it
    cUIText* mpTextAuth;            // 0x6D4 [R] 0; [U] onWidgetCreate cFEStateAuthDNAS_onWidgetCreate: widget "TextAuth"
                                    //       ("kT_ovrcmnloading", then "kT_205Authenticating" in onUpdate)
    cUIText* mpTextCancel;          // 0x6D8 [U] onWidgetCreate: widget "TextCancel" (hidden; onUpdate shows it with
                                    //       the next two once the object is ready)
    cUIThing* mpXButton;            // 0x6DC [U] onWidgetCreate: widget "XButton" (hidden)
    cUIThing* mpSubmitRect;         // 0x6E0 [U] onWidgetCreate: widget "submitrect" (hidden)
    int miState;                    // 0x6E4 [R] 0; [U] onCreateScreen sets 1; onUpdate cFEStateAuthDNAS_onUpdate: 1 -> 2 when the
                                    //       object is ready (starts it), 2 -> 3 when done, 3 reports ok / error to
                                    //       mpParentState, 4 (cancelled by cFEStateAuthDNAS_onInputBegin in state 2) reports 0x14
                                    //       when done
    char unk_6E8[0xC];              // 0x6E8 no access seen
    int unk_6F4;                    // 0x6F4 [R] 2 (both ctors); no other access seen
    int unk_6F8;                    // 0x6F8 [R] 0x1E (both ctors); no other access seen
    unsigned short* mpustrText;     // 0x6FC [R] 0, then operator_new_tag((USTR_length("kT_205Authenticating") + 5) *
                                    //       2, "cFEStateAuthDNAS") (cFEStateAuthDNAS_cFEStateAuthDNAS); freed by the destructor; no other
                                    //       access seen
};

CHECK_OFFSET(cFEStateAuthDNAS, mpAuth, 0x6D0);
CHECK_OFFSET(cFEStateAuthDNAS, mpTextAuth, 0x6D4);
CHECK_OFFSET(cFEStateAuthDNAS, mpTextCancel, 0x6D8);
CHECK_OFFSET(cFEStateAuthDNAS, mpXButton, 0x6DC);
CHECK_OFFSET(cFEStateAuthDNAS, mpSubmitRect, 0x6E0);
CHECK_OFFSET(cFEStateAuthDNAS, miState, 0x6E4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6E8, 0x6E8);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6F4, 0x6F4);
CHECK_OFFSET(cFEStateAuthDNAS, unk_6F8, 0x6F8);
CHECK_OFFSET(cFEStateAuthDNAS, mpustrText, 0x6FC);
CHECK_SIZE(cFEStateAuthDNAS, 0x700);

struct cFEStateAuthDNASError : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateAuthDNASError_vtbl (vtables.tsv), stored by cFEStateAuthDNASError_cFEStateAuthDNASError /
    //       cFEStateAuthDNASError_cFEStateAuthDNASError1 after the cFEStateNet ctor and by the destructor
    //       cFEStateAuthDNASError__cFEStateAuthDNASError
    // cFEStateAuthDNASError's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateAuthDNASError_cFEStateAuthDNASError stores
    //       0x45
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateAuthDNASError
    unsigned short* mpustrBody;     // 0x6D0 [R] the ctor's 4th argument (cFEStateNetConfig's mpustrAuthError[1]), 0
                                    //       in the default ctor; [U] cFEStateAuthDNASError_onGainTransition (slot 7) wraps it into mpBody, or
                                    //       hides mpBody when empty (strlen1)
    unsigned short* mpustrFooter;   // 0x6D4 [R] the 5th argument (mpustrAuthError[2]), 0; [U] cFEStateAuthDNASError_onGainTransition plays
                                    //       frame "full" and wraps it into mpFooter, or plays "short" and hides
                                    //       mpFooter when empty
    unsigned short* mpustrTitle;    // 0x6D8 [R] the 6th argument (mpustrAuthError[0]), 0; [U] cFEStateAuthDNASError_onGainTransition wraps it
                                    //       into mpTitle, or hides mpTitle when empty
    cUIText* mpFooter;              // 0x6DC [U] onWidgetCreate cFEStateAuthDNASError_onWidgetCreate: widget "Footer"
    cUIText* mpBody;                // 0x6E0 [U] onWidgetCreate: widget "Body"
    cUIText* mpTitle;               // 0x6E4 [U] onWidgetCreate: widget "Title"
};

CHECK_OFFSET(cFEStateAuthDNASError, mpustrBody, 0x6D0);
CHECK_OFFSET(cFEStateAuthDNASError, mpustrFooter, 0x6D4);
CHECK_OFFSET(cFEStateAuthDNASError, mpustrTitle, 0x6D8);
CHECK_OFFSET(cFEStateAuthDNASError, mpFooter, 0x6DC);
CHECK_OFFSET(cFEStateAuthDNASError, mpBody, 0x6E0);
CHECK_OFFSET(cFEStateAuthDNASError, mpTitle, 0x6E4);
CHECK_SIZE(cFEStateAuthDNASError, 0x6E8);

#endif /* FE_FESTATEAUTHDNAS_H */
