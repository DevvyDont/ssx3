#ifndef FE_FESTATENETLOBBY_H
#define FE_FESTATENETLOBBY_H
#pragma interface

// cFEStateLobbyMenu: the online lobby screen (four room channels - beginner, voice, advanced, expert - each a list
// box of rooms, and the list of players in the selected room). A cFEStateNet (fe/festatenet.h, 0x6D0). EA's name
// from the preview's linkonce sections (__17cFEStateLobbyMenu, _$_17cFEStateLobbyMenu,
// onGainTransition__17cFEStateLobbyMenub); EA's source d:/ssx/code/source/fe/festatenetlobby.cpp (the preview's
// assert at line 518 sits between the methods paired with retail cFEStateLobbyMenu_onNetEvent and cFEStateLobbyMenu_handleJoinRoomError).
//
// Size 0xB84: cMemMan_alloc(0xB84, "cFEStateLobbyMenu") + cFEStateLobbyMenu_cFEStateLobbyMenu(mem, pEngine) (cFEStateNet_onNetEvent,
// cFEStateChatRoom_refreshPlayerList, cFEStateChatRoom_onNetEvent, cFEStateOnlineMainMenu_onWidgetEvent); its last member, the cBXString at 0xB80, ends there. Constructors
// cFEStateLobbyMenu_cFEStateLobbyMenu and the default cFEStateLobbyMenu_cFEStateLobbyMenu1 (fe/fepopupmisc), destructor cFEStateLobbyMenu__cFEStateLobbyMenu (fe/fepopupmisc; the
// preview's _$_17cFEStateLobbyMenu). Retail vtable cFEStateLobbyMenu_vtbl (32 slots: 1 dtor, 4 onCreateScreen cFEStateLobbyMenu_onCreateScreen,
// 7 onGainTransition cFEStateLobbyMenu_onGainTransition, 12 onUpdate cFEStateLobbyMenu_onUpdate, 17 onPopupEvent cFEStateLobbyMenu_onPopupEvent, 18 onWidgetCreate
// cFEStateLobbyMenu_onWidgetCreate, 19 onWidgetEvent cFEStateLobbyMenu_onWidgetEvent, 20 onWidgetMenu cFEStateLobbyMenu_onWidgetMenu, 21 cFEStateLobbyMenu_onWidgetPair, 25
// cFEStateLobbyMenu_fillHelpPopup (help), 31 cFEStateLobbyMenu_onNetEvent (net events), 32 cFEStateLobbyMenu_publishPresence). Methods: fe/feasyncfile
// 0x1BCB58-0x1BE720. tools/layout_xlate.py: preview 0xAC..0x128 = retail 0x6D0..0x74C (+0x624), preview 0x12C =
// retail 0x760.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. No On Tour DWARF exists for this
// class and the preview keeps no accessors for it: widget pointers are named after the UI object onWidgetCreate
// (cFEStateLobbyMenu_onWidgetCreate) binds to them, the rest from retail use. Channel arrays are indexed by the
// channel (0 beginner, 1 voice, 2 advanced, 3 expert/elite: the RoomMenus row). Evidence tags:
//   [R] retail's constructors cFEStateLobbyMenu_cFEStateLobbyMenu / cFEStateLobbyMenu_cFEStateLobbyMenu1 and destructor cFEStateLobbyMenu__cFEStateLobbyMenu (and cFEStateNet's);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cUIThing;
struct cUIMenu;
struct cUIListBox;

struct cFEStateNetCreateRoomPopup;
struct cFEStateNetHelpPopup;
struct cUIState;

struct cFEStateLobbyMenu : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateLobbyMenu_vtbl (32 slots), stored by cFEStateLobbyMenu_cFEStateLobbyMenu /
    //       cFEStateLobbyMenu_cFEStateLobbyMenu1 after the cFEStateNet ctor and by the destructor
    //       cFEStateLobbyMenu__cFEStateLobbyMenu
    // cFEStateLobbyMenu's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateLobbyMenu_cFEStateLobbyMenu stores 0x3C
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateLobbyMenu
    cUIMenu* mpMenuPlayers;         // 0x6D0 [R] 0; [U] onWidgetCreate: widget "MenuPlayers" (input map 1, scrollable); cFEStateLobbyMenu_fillPlayerRows
                                    //       stores the player count into it (+ 0x96)
    cUIMenu* mpMenuRoomMenus;       // 0x6D4 [R] 0; [U] widget "MenuRoomMenus"; its selected item's + 0x18 picks the channel
                                    //       (cFEStateLobbyMenu_onNetEvent event 0xDF); onGainTransition cUIMenu_setSelectedByIndex(.., miSelectedChannel)
    cUIText* mpTextPlayer[10];      // 0x6D8 [R] 0; [U] widgets "TextPlayer%d" (their + 0x18 = the index); cFEStateLobbyMenu_fillPlayerRows writes the
                                    //       player names from miFirstPlayer on
    cUIText* mpTextCreateRoom;      // 0x700 [U] widget "TextCreateRoom" (its + 0x18 = 4)
    cUIText* mpText3InRoom;         // 0x704 [R] 0; [U] widget "3inroom"; cFEStateLobbyMenu_fillPlayerRows prints "kT_231PlayersInRoom" into it
    cUIListBox* mpChannelList[4];   // 0x708 [R] 0; [U] widgets "ListBoxBeginnerChannel", "ListBoxVoiceChannel",
                                    //       "ListBoxAdvancedChannel", "ListBoxExpertChannel"; cFEStateLobbyMenu_rebuildChannelLists fills them, cFEStateLobbyMenu_restoreChannelSelections / onGainTransition
                                    //       select rows (count byte + 0x318)
    cUIThing* mpChannelPair[4];     // 0x718 [R] 0; [U] widgets "PairBeginner", "PairVoice", "PairAdvanced", "PairExpert"
                                    //       (their + 0x18 = 0..3); onGainTransition passes mpChannelPair[miSelectedChannel] to cFEStateLobbyMenu_selectChannelRoom
    cUIThing* mpPlayerUp;           // 0x728 [R] 0; [U] widget "player up"; cFEStateLobbyMenu_updatePlayerScrollArrows shows it while miFirstPlayer > 0
    cUIThing* mpPlayerDown;         // 0x72C [R] 0; [U] widget "player down"; cFEStateLobbyMenu_updatePlayerScrollArrows
    cUIThing* mpChannelArrow[4];    // 0x730 [R] 0; [U] widgets "ArrowBeginner", "ArrowVoice", "ArrowAdvanced", "ArrowElite";
                                    //       cFEStateLobbyMenu_showChannelArrow / cFEStateLobbyMenu_hideChannelArrows show the selected channel's
    cUIThing* mpChannelLock[4];     // 0x740 [R] 0; [U] widgets "LockBeginner", "LockVoice", "LockAdvanced", "LockElite";
                                    //       cFEStateLobbyMenu_selectChannelRoom
    unsigned int miChannelRow[4];   // 0x750 [R] 0; [U] cFEStateLobbyMenu_restoreChannelSelections: the selected row of each channel list, clamped to
                                    //       its count and selected again (cUIListBox_setCurrentEntryByIndex)
    int mbRoomChanged;              // 0x760 [U] onWidgetEvent cFEStateLobbyMenu_onWidgetEvent sets it with miSelectedChannel / miSelectedRoom;
                                    //       onGainTransition cFEStateLobbyMenu_onGainTransition reselects them when set and clears it
    int miSelectedChannel;          // 0x764 [U] cFEStateLobbyMenu_onWidgetEvent / cFEStateLobbyMenu_onNetEvent (event 0xDF: the RoomMenus row); < 4 indexes the
                                    //       channel arrays
    int miSelectedRoom;             // 0x768 [U] cFEStateLobbyMenu_onWidgetEvent stores the room list's selected row (list + 0x319); onGainTransition
                                    //       clamps it to the list count and selects it
    char* mszChannelName;           // 0x76C [R] cBXString (empty rep cBXString_EmptyString; destroyed by cFEStateLobbyMenu__cFEStateLobbyMenu); [U] cFEStateLobbyMenu_onNetEvent
                                    //       assigns the selected list entry's text (cUIListBox_getCurrentAsciiText)
    int mbRoomHasPassword;          // 0x770 [U] cFEStateLobbyMenu_joinRoomById / cFEStateLobbyMenu_joinRoomByName: 1 when a password was given when entering the room;
                                    //       cFEStateLobbyMenu_handleJoinRoomError asks for the password ("kT_POPUP231RoomPassword") only when it is 0
    int miRoomId;                   // 0x774 [U] onWidgetEvent cFEStateLobbyMenu_onWidgetEvent stores the chosen room id and asks LobbyMan for it
                                    //       (LobbyMan_peekRoom); cFEStateLobbyMenu_onNetEvent event 0xE0 / onWidgetMenu cFEStateLobbyMenu_onWidgetMenu pass it on
    int miFirstPlayer;              // 0x778 [R] 0; [U] the players list's scroll offset: cFEStateLobbyMenu_fillPlayerRows shows players
                                    //       miFirstPlayer..+9, cFEStateLobbyMenu_updatePlayerScrollArrows the up/down arrows; cFEStateLobbyMenu_onWidgetEvent / cFEStateLobbyMenu_selectChannelRoom reset it
    int miNumPlayers;               // 0x77C [U] cFEStateLobbyMenu_fillPlayerRows: LobbyMan's player count in the room (LobbyMan_getNumUsers); cFEStateLobbyMenu_updatePlayerScrollArrows
    unsigned short mustrPopupText[0x200]; // 0x780 [U] cFEStateLobbyMenu_handleJoinRoomError: sprintf1 (unicode) of "kT_POPUP231RoomPassword" with
                                    //       mszRoomName, the popup's message (+ 0xC4); extent: up to 0xB80
    char* mszRoomName;              // 0xB80 [R] cBXString (empty rep cBXString_EmptyString; destroyed by cFEStateLobbyMenu__cFEStateLobbyMenu); [U] cFEStateLobbyMenu_joinRoomById /
                                    //       cFEStateLobbyMenu_joinRoomByName assign the room being entered; cFEStateLobbyMenu_handleKeyboardText passes it to cFEStateLobbyMenu_joinRoomByName

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateLobbyMenu1() __asm__("cFEStateLobbyMenu_cFEStateLobbyMenu1");
    virtual ~cFEStateLobbyMenu();   // g++'s own symbol _$_17cFEStateLobbyMenu (the retail label renamed)
    virtual void onPopupEvent(cUIState*pPopup, unsigned int Event) __asm__("cFEStateLobbyMenu_onPopupEvent");
    virtual void onCreateScreen() __asm__("cFEStateLobbyMenu_onCreateScreen");
    virtual bool onGainTransition(int reset) __asm__("cFEStateLobbyMenu_onGainTransition");
    virtual void onWidgetCreate(cUIThing* obj) __asm__("cFEStateLobbyMenu_onWidgetCreate");
    virtual void onUpdate() __asm__("cFEStateLobbyMenu_onUpdate__FPv");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateLobbyMenu_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* obj, unsigned int msg, unsigned int index)
        __asm__("cFEStateLobbyMenu_onWidgetMenu");
    void handleKeyboardText() __asm__("cFEStateLobbyMenu_handleKeyboardText");
    void joinRoomById(int roomId, const char* str) __asm__("cFEStateLobbyMenu_joinRoomById");
    void joinRoomByName(const char* roomName, void* password) __asm__("cFEStateLobbyMenu_joinRoomByName");
    void createRoom(const char* namePtr, char* channelName, const char* extraPtr)
        __asm__("cFEStateLobbyMenu_createRoom");
    void* onNetEvent(int msgId, void* data) __asm__("cFEStateLobbyMenu_onNetEvent");
    void handleErrorEvent(int msgId, void* data) __asm__("cFEStateLobbyMenu_handleErrorEvent");
    void handleJoinRoomError(void* msgPtr) __asm__("cFEStateLobbyMenu_handleJoinRoomError");
    void handleCreateRoomError(void* msgPtr) __asm__("cFEStateLobbyMenu_handleCreateRoomError");
    void onCreateRoomPopupEvent(cFEStateNetCreateRoomPopup* popup, int event)
        __asm__("cFEStateLobbyMenu_onCreateRoomPopupEvent");
    void onRoomPasswordPopupEvent(cUIState* popup, int event) __asm__("cFEStateLobbyMenu_onRoomPasswordPopupEvent");
    void fillHelpPopup(cFEStateNetHelpPopup* popup) __asm__("cFEStateLobbyMenu_fillHelpPopup");
    void hideChannelArrows() __asm__("cFEStateLobbyMenu_hideChannelArrows");
    void rebuildChannelLists() __asm__("cFEStateLobbyMenu_rebuildChannelLists");
    void redrawPlayerList() __asm__("cFEStateLobbyMenu_redrawPlayerList");
    void fillPlayerRows(int page) __asm__("cFEStateLobbyMenu_fillPlayerRows");
    void updatePlayerScrollArrows() __asm__("cFEStateLobbyMenu_updatePlayerScrollArrows");
    void selectChannelRoom(cUIThing* id) __asm__("cFEStateLobbyMenu_selectChannelRoom");
};

CHECK_OFFSET(cFEStateLobbyMenu, mpMenuPlayers, 0x6D0);
CHECK_OFFSET(cFEStateLobbyMenu, mpMenuRoomMenus, 0x6D4);
CHECK_OFFSET(cFEStateLobbyMenu, mpTextPlayer, 0x6D8);
CHECK_OFFSET(cFEStateLobbyMenu, mpTextCreateRoom, 0x700);
CHECK_OFFSET(cFEStateLobbyMenu, mpText3InRoom, 0x704);
CHECK_OFFSET(cFEStateLobbyMenu, mpChannelList, 0x708);
CHECK_OFFSET(cFEStateLobbyMenu, mpChannelPair, 0x718);
CHECK_OFFSET(cFEStateLobbyMenu, mpPlayerUp, 0x728);
CHECK_OFFSET(cFEStateLobbyMenu, mpPlayerDown, 0x72C);
CHECK_OFFSET(cFEStateLobbyMenu, mpChannelArrow, 0x730);
CHECK_OFFSET(cFEStateLobbyMenu, mpChannelLock, 0x740);
CHECK_OFFSET(cFEStateLobbyMenu, miChannelRow, 0x750);
CHECK_OFFSET(cFEStateLobbyMenu, mbRoomChanged, 0x760);
CHECK_OFFSET(cFEStateLobbyMenu, miSelectedChannel, 0x764);
CHECK_OFFSET(cFEStateLobbyMenu, miSelectedRoom, 0x768);
CHECK_OFFSET(cFEStateLobbyMenu, mszChannelName, 0x76C);
CHECK_OFFSET(cFEStateLobbyMenu, mbRoomHasPassword, 0x770);
CHECK_OFFSET(cFEStateLobbyMenu, miRoomId, 0x774);
CHECK_OFFSET(cFEStateLobbyMenu, miFirstPlayer, 0x778);
CHECK_OFFSET(cFEStateLobbyMenu, miNumPlayers, 0x77C);
CHECK_OFFSET(cFEStateLobbyMenu, mustrPopupText, 0x780);
CHECK_OFFSET(cFEStateLobbyMenu, mszRoomName, 0xB80);
CHECK_SIZE(cFEStateLobbyMenu, 0xB84);
__asm__("_vt$17cFEStateLobbyMenu = cFEStateLobbyMenu_vtbl");   // g++'s name for the retail vtable

#endif /* FE_FESTATENETLOBBY_H */
