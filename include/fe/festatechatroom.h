#ifndef FE_FESTATECHATROOM_H
#define FE_FESTATECHATROOM_H

// cFEStateChatRoom: the online chat room (screen "235chat_room"): the room's player list (6 visible rows
// "PlayerListItem00".."05" with connection-quality icons), the selected player's stats, the chat history and the
// chat-message menu. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the preview's __16cFEStateChatRoom,
// _$_16cFEStateChatRoom and onMenuGainFocus__16cFEStateChatRoomP8cUIGroup. EA's source file is not known: its code
// sits between cFEStateOnlineCharacterSelect and cFEStateNetConfig (festatenetconfig.cpp) in both builds, and no
// assert names it; the header is named after the class.
//
// Size 0x73C: cMemMan_alloc(0x73C, "cFEStateChatRoom") + cFEStateChatRoom_cFEStateChatRoom(mem, pEngine) at all 4 sites (func_001A9578,
// func_001B15D8, func_001BD738; fe/festatebigradio func_001941B0); the constructor's last store is the 16-byte
// mChatColour at 0x72C. Constructors cFEStateChatRoom_cFEStateChatRoom (cFEStateNet(pEngine, 0), vtable cFEStateChatRoom_vtbl, mHashName 0x3D) and
// the default cFEStateChatRoom_cFEStateChatRoom1 (fe/fepopupmisc); destructor cFEStateChatRoom__cFEStateChatRoom (slot 1; the preview's _$_16cFEStateChatRoom).
// Retail vtable cFEStateChatRoom_vtbl (32 slots: 4 onCreateScreen cFEStateChatRoom_onCreateScreen, 6 onGainFocus cFEStateChatRoom_onGainFocus, 7 cFEStateChatRoom_onGainTransition,
// 8 cFEStateChatRoom_onInputBegin, 12 onUpdate cFEStateChatRoom_onUpdate, 13 cFEStateChatRoom_onMenuGainFocus, 17 onPopupEvent cFEStateChatRoom_onPopupEvent, 18 onWidgetCreate
// cFEStateChatRoom_onWidgetCreate, 19 onWidgetEvent cFEStateChatRoom_onWidgetEvent, 20 onWidgetMenu cFEStateChatRoom_onWidgetMenu, 25 func_001B2DA0 (help), 31
// func_001B3A30 (net events), 32 func_001B22A0). Methods: fe/feasyncfile 0x1B1F88-0x1B3D20.
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
#include "ui/uithing.h"
#include "ui/uiscrollbar.h"   // cUIScrollBar (mpSlider1 / mpSlider2)

struct cUIThing;
struct cUIMenu;
struct cUITextScroll;

struct cFEStateChatRoom : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateChatRoom_vtbl (vtables.tsv; 32 slots), stored by cFEStateChatRoom_cFEStateChatRoom /
    //       cFEStateChatRoom_cFEStateChatRoom1 after the cFEStateNet ctor and by the destructor
    //       cFEStateChatRoom__cFEStateChatRoom
    // cFEStateChatRoom's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateChatRoom_cFEStateChatRoom stores 0x3D
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateChatRoom
    cUIThing* mpQualityIcon[6];     // 0x6D0 [U] onWidgetCreate cFEStateChatRoom_onWidgetCreate: widgets "quality1".."quality6" (sprintf
                                    //       "quality%d"); func_001B3BF0 shows / hides icon i for the visible player
                                    //       row i and sets its quality (func_001AAD50)
    cUIText* mpTextRoomName;        // 0x6E8 [U] onWidgetCreate: widget "TextRoomName"; func_001B2378 sets the room
                                    //       name text on it
    cUIScrollBar* mpSlider1;        // 0x6EC [R] 0; [U] onWidgetCreate: widget "slider1" (the player list's scroll
                                    //       bar; attr bit 0 set); onCreateScreen cFEStateChatRoom_onCreateScreen stores its
                                    //       mpUpArrow / mpDownArrow / mpHideOnEmpty (+ 0x7C/0x80/0x84) and meHideState = 2
                                    //       (+ 0x88) (ui/uiscrollbar.h); func_001B3D20 updates it from miMenuScroll[0]
                                    //       (cUIScrollBar_setValues); cFEStateChatRoom_onGainTransition hides it (cUIScrollBar_setScrollVisibile)
    cUITextScroll* mpTextScrollChat; // 0x6F0 [R] 0; [U] onWidgetCreate: widget "TextScrollChat" (the chat history);
                                    //       onCreateScreen gives it mChatColour (+ 0xA8) and mpSlider2 (+ 0xB8);
                                    //       func_001B2CD0 appends "name: message" lines to it (cUITextScroll_addAsciiString,
                                    //       func_001A83D8)
    cUIScrollBar* mpSlider2;        // 0x6F4 [R] 0; [U] onWidgetCreate: widget "slider2" (the chat history's scroll
                                    //       bar); onCreateScreen sets it up like mpSlider1 and also copies the highlight
                                    //       names into its mszUpAnimation / mszDownAnimation (+ 0x8C / + 0xA5), then hands
                                    //       it to mpTextScrollChat's cUITextScroll::mpScrollBar (+ 0xB8)
    int mbExiting;                  // 0x6F8 [R] 0 (both ctors); [U] onWidgetEvent cFEStateChatRoom_onWidgetEvent (back) sets 1 and
                                    //       opens "kT_POPUP231ExitingRoom"; the net-event handler func_001B3A30 drops
                                    //       chat messages (0xDE) while it is set
    cUIMenu* mpMenu[2];             // 0x6FC [R] 0 (cFEStateChatRoom_cFEStateChatRoom clears both in a loop with miMenuScroll); [U]
                                    //       onWidgetCreate: [0] = "PlayerListMenu", [1] = "MenuChat"; cFEStateChatRoom_onGainTransition
                                    //       enables both (slot 7); onWidgetMenu cFEStateChatRoom_onWidgetMenu compares the sender
                                    //       with them
    int miMenuScroll[2];            // 0x704 [R] 0; [U] onWidgetMenu cFEStateChatRoom_onWidgetMenu (event 4) stores the menu's scroll
                                    //       value: [0] = the first visible player row (func_001B2378, func_001B3BF0,
                                    //       func_001B3D20 read it; func_001B2378 clears it with fewer than 7
                                    //       players), [1] for "MenuChat"
    char mszPlayerName[0x20];       // 0x70C [R] strcpy of LobbyMan's mPersona; [U] func_001B27B0 copies the selected
                                    //       player's name here, func_001B2378 / func_001B3A30 show that player's info
                                    //       again (func_001B3978); extent: up to mChatColour
    tARGB mChatColour;              // 0x72C [R] 1.0, 1.0, 1.0, 1.0 (an unaligned 16-byte copy: sdl/sdr); [U]
                                    //       onCreateScreen copies it to mpTextScrollChat + 0xA8 (ldl/ldr)

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateChatRoom1() __asm__("cFEStateChatRoom_cFEStateChatRoom1");
    // onUpdate stays a C function (cFEStateChatRoom_onUpdate): declared first, it is the vtable's key function, which
    // keeps the vtable out of the units defining the other overrides
    virtual void onUpdate() __asm__("cFEStateChatRoom_onUpdate");
    virtual void onWidgetCreate(cUIThing* obj) __asm__("cFEStateChatRoom_onWidgetCreate");
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateChatRoom_onGainTransition");
    virtual bool onInputBegin(cUIInput* obj) __asm__("cFEStateChatRoom_onInputBegin");
    virtual void onWidgetEvent(cUIThing* msg, unsigned int ev) __asm__("cFEStateChatRoom_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* sender, unsigned int event, unsigned int value)
        __asm__("cFEStateChatRoom_onWidgetMenu");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event) __asm__("cFEStateChatRoom_onPopupEvent");
    virtual void onMenuGainFocus(cUIGroup*) __asm__("cFEStateChatRoom_onMenuGainFocus__FPv");
};

CHECK_OFFSET(cFEStateChatRoom, mpQualityIcon, 0x6D0);
CHECK_OFFSET(cFEStateChatRoom, mpTextRoomName, 0x6E8);
CHECK_OFFSET(cFEStateChatRoom, mpSlider1, 0x6EC);
CHECK_OFFSET(cFEStateChatRoom, mpTextScrollChat, 0x6F0);
CHECK_OFFSET(cFEStateChatRoom, mpSlider2, 0x6F4);
CHECK_OFFSET(cFEStateChatRoom, mbExiting, 0x6F8);
CHECK_OFFSET(cFEStateChatRoom, mpMenu, 0x6FC);
CHECK_OFFSET(cFEStateChatRoom, miMenuScroll, 0x704);
CHECK_OFFSET(cFEStateChatRoom, mszPlayerName, 0x70C);
CHECK_OFFSET(cFEStateChatRoom, mChatColour, 0x72C);
CHECK_SIZE(cFEStateChatRoom, 0x73C);

#endif /* FE_FESTATECHATROOM_H */
