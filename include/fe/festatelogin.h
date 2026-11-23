#ifndef FE_FESTATELOGIN_H
#define FE_FESTATELOGIN_H

// cFEStateLogin: screen "201login_type": the online login choice (use the stored account, create one
// (cFEStateCreateStep1), use an existing one (cFEUseExistingAccount), load one from the memory card
// (cFEStateNetAccountMem)), the "verify" popup while logging in, persona selection (cFEStateSelectPersona) and the
// "disconnect?" question on back. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the preview's
// __13cFEStateLogin / _$_13cFEStateLogin and the allocation tags. EA's source file is not known (its code sits
// between cFEStateNetCreateRoomPopup's and cFEStateOnlineMainMenu's in both builds; no assert of its own).
//
// Size 0x6D4: cMemMan_alloc(0x6D4, "cFEStateLogin" / "cFEStateNetLogin" / "cFEStateLogin MemoryCard") +
// cFEStateLogin_cFEStateLogin(mem, pEngine) at 4 sites (feasyncfile 4248, 4398, 12745, 20767); mpustrDisconnect ends at 0x6D4.
// Constructors cFEStateLogin_cFEStateLogin (cFEStateNet(pEngine, 0), vtable cFEStateLogin_vtbl, mHashName 0x33, unk_6A8 = 0) and the
// default cFEStateLogin_cFEStateLogin1 (fe/fepopupmisc); destructor cFEStateLogin__cFEStateLogin (slot 1: frees mpustrDisconnect). Retail vtable
// cFEStateLogin_vtbl (33 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateLogin_onCreateScreen, 12 onUpdate cFEStateLogin_onUpdate, 17
// onPopupEvent cFEStateLogin_onPopupEvent, 18 onWidgetCreate cFEStateLogin_onWidgetCreate, 19 onWidgetEvent cFEStateLogin_onWidgetEvent, 25 func_001BFAE0
// (help), 29 func_001E0E90, 31 func_001BF228 (net events), 32 func_001E0E88, 33 func_001BF378). Methods:
// fe/feasyncfile 0x1BEE30-0x1BFAE0.
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

struct cFEStateLogin : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateLogin_vtbl (33 slots), stored by cFEStateLogin_cFEStateLogin / cFEStateLogin_cFEStateLogin1 after
    //       the cFEStateNet ctor and by the destructor cFEStateLogin__cFEStateLogin
    // cFEStateLogin's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateLogin_cFEStateLogin stores 0x33
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateLogin
    unsigned short* mpustrDisconnect; // 0x6D0 [R] 0 (both ctors); [U] onWidgetEvent cFEStateLogin_onWidgetEvent (back): frees it
                                    //       and stores operator_new_tag(.., "cFEStateLogin") holding
                                    //       "kT_POPUPCMNDisconnecting" + "kT_POPUPCMNDisconnect" (sprintf2), then
                                    //       hands it to the question popup (popup + 0xC4, text type 1); the
                                    //       destructor cFEStateLogin__cFEStateLogin frees it
};

CHECK_OFFSET(cFEStateLogin, mpustrDisconnect, 0x6D0);
CHECK_SIZE(cFEStateLogin, 0x6D4);

#endif /* FE_FESTATELOGIN_H */
