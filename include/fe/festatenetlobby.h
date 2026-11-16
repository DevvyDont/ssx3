#ifndef FE_FESTATENETLOBBY_H
#define FE_FESTATENETLOBBY_H

// cFEStateLobbyMenu: the online lobby screen (four room channels - beginner, voice, advanced, expert - each a list
// box of rooms, and the list of players in the selected room). A cFEStateNet (fe/festatenet.h, 0x6D0). EA's name
// from the preview's linkonce sections (__17cFEStateLobbyMenu, _$_17cFEStateLobbyMenu,
// onGainTransition__17cFEStateLobbyMenub); EA's source d:/ssx/code/source/fe/festatenetlobby.cpp (the preview's
// assert at line 518 sits between the methods paired with retail func_001BD738 and func_001BD8C8).
//
// Size 0xB84: cMemMan_alloc(0xB84, "cFEStateLobbyMenu") + cFEStateLobbyMenu_cFEStateLobbyMenu(mem, pEngine) (func_001A8E40,
// func_001B2378, func_001B3A30, cFEStateOnlineMainMenu_onWidgetEvent); its last member, the cBXString at 0xB80, ends there. Constructors
// cFEStateLobbyMenu_cFEStateLobbyMenu and the default func_001DF780 (fe/fepopupmisc), destructor cFEStateLobbyMenu__cFEStateLobbyMenu (fe/fepopupmisc; the
// preview's _$_17cFEStateLobbyMenu). Retail vtable cFEStateLobbyMenu_vtbl (32 slots: 1 dtor, 4 onCreateScreen cFEStateLobbyMenu_onCreateScreen,
// 7 onGainTransition cFEStateLobbyMenu_onGainTransition, 12 onUpdate cFEStateLobbyMenu_onUpdate, 17 onPopupEvent cFEStateLobbyMenu_onPopupEvent, 18 onWidgetCreate
// cFEStateLobbyMenu_onWidgetCreate, 19 onWidgetEvent cFEStateLobbyMenu_onWidgetEvent, 20 onWidgetMenu cFEStateLobbyMenu_onWidgetMenu, 21 cFEStateLobbyMenu_onWidgetPair, 25
// func_001BDE48 (help), 31 func_001BD738 (net events), 32 func_001BCCA8). Methods: fe/feasyncfile
// 0x1BCB58-0x1BE720. tools/layout_xlate.py: preview 0xAC..0x128 = retail 0x6D0..0x74C (+0x624), preview 0x12C =
// retail 0x760.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). No On Tour DWARF exists for
// this class and the preview keeps no accessors for it: widget pointers are named after the UI object
// onWidgetCreate (cFEStateLobbyMenu_onWidgetCreate) binds to them, the rest from retail use. Channel arrays are indexed by the
// channel (0 beginner, 1 voice, 2 advanced, 3 expert/elite: the RoomMenus row). Evidence tags:
//   [R] retail's constructors cFEStateLobbyMenu_cFEStateLobbyMenu / func_001DF780 and destructor cFEStateLobbyMenu__cFEStateLobbyMenu (and cFEStateNet's);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.

#include "layout.h"
#include "fe/festatenet.h"

struct cUIThing;
struct cUIMenu;
struct cUIListBox;

struct cFEStateLobbyMenu {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateLobbyMenu_vtbl (32 slots), stored by cFEStateLobbyMenu_cFEStateLobbyMenu / func_001DF780 after the cFEStateNet ctor and
                                    //       by the destructor cFEStateLobbyMenu__cFEStateLobbyMenu
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateLobbyMenu_cFEStateLobbyMenu stores 0x3C
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] passed to every child state's ctor
                                    //       (cMemMan_alloc(..) + ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word
                                    //       (ui/uistate.h); [R] cUIState ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's owner argument; [U] func_001A92D8 calls its slot 17
                                    //       (onPopupEvent)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
                                    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then func_001A8500 stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h)
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override func_001A87D0 sets 1; func_001DE8A8 (own
                                    //       virtual, slot 29) returns mpScreen != 0 && unk_048 != 0 (the preview's
                                    //       isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] func_001AAB68 (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] func_001AAB68 stores the frame when the session's count
                                    //       (func_00262938) becomes > 0 and 0 when it drops; func_001AA9B0 (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter cAppMan_TheAppMan + 0x18)
    unsigned int mChallengeHintFrame; // 0x054 [R] 0; [U] func_001AAB68, the same for the session's other list (the
                                    //       vector at session + 0xBC); func_001AA9B0 blinks "IconChallengeHint" with it
    int unk_058;                    // 0x058 [R] 0; [U] func_001A9668/func_001A9710 set 1 before pushing cFEStateLogin;
                                    //       func_001DE860 (slot 7, the onInputBegin override) returns it
    char mszMaskedText[0x40];       // 0x05C [U] func_001A98B8 writes one '*' per character of its string (at most 0x3F;
                                    //       " " when empty) and a NUL; cUIText_setAsciiString(.., mszMaskedText) in
                                    //       func_001ACDD0, func_001B9610, func_001B9678, func_001B9820 (password boxes)
    unsigned short mustrMessage[0x200]; // 0x09C [U] sprintf1 (EA's unicode sprintf) target that becomes the popup's
                                    //       message (func_001A4E28); extent: up to the next used offset
    unsigned short unk_49C[0x80];   // 0x49C [U] func_001AB478: sprintf1 of a status line, then setPopupConfirmation
                                    //       (func_001C5838, unicode) with it
    unsigned short unk_59C[0x80];   // 0x59C [U] func_001AB478: the second status line, used the same way
    cKeyboardPopup* mpKeyboard;     // 0x69C [R] 0; [U] func_001A91B0 stores cMemMan_alloc(0x444, "cKeyboardPopup") +
                                    //       cKeyboardPopup_cKeyboardPopup and pushes it; func_001A9090/90F0/9150 set its texts;
                                    //       func_001A97B8 (onPopupEvent) compares the closing popup with it
    cScreenPopup* mpPopup;          // 0x6A0 [R] 0; [U] createPopup func_001A9C50: cMemMan_alloc(0x360, "createPopup") +
                                    //       cScreenPopup_cScreenPopup, reused (cPopupInfo::clear) when set; func_001A9BC0
                                    //       shows it; func_001A93D0 clears it when that popup closes (event 22)
    cUIText* mpKeyboardText;        // 0x6A4 [R] 0; [U] func_001A91B0 stores its a1 (the text box the
                                    //       keyboard edits); cUIText_setAsciiString(mpKeyboardText, mszMaskedText)
    int unk_6A8;                    // 0x6A8 [R] 1; 0 in the ctors of cFEUseExistingAccount, cFEStateNetConfig,
                                    //       cFEStateAuthDNAS, cFEStateCreateStep1/2, cFEStateCreateDuplicate, cFEStateLogin;
                                    //       [U] while set, func_001A89B0 (slot 25) drops net messages other than kinds
                                    //       0x31/0x33/0x4D/0x4E and opens the disconnect popup (func_001A9930)
    int unk_6AC;                    // 0x6AC [R] 0; [U] func_001A9930 sets it when it opens the "kT_POPUPCMNNetInterrupted"/
                                    //       "kT_POPUPCMNDisconnected" popup (only when clear); func_001A9668/func_001A9710
                                    //       clear it, func_001A5010 tests it
    int unk_6B0;                    // 0x6B0 [R] 0; [U] set to 1 by func_001DE890 (inline setter), func_001BF5A8,
                                    //       func_001C0138; no read seen
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent func_001A8B88: 1 while a net event (0xC8..0x116)
                                    //       waits for slot 30 to accept it, 0 once delivered
    void* unk_6B8;                  // 0x6B8 [R] 0; [U] a heap text buffer: func_001A9CC0 frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int unk_6BC;                    // 0x6BC [R] 2; [U] func_001A92D8 (popup events): 2 on event 21, 0 on event 15;
                                    //       on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_cBXString4), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
    // --- cFEStateLobbyMenu
    cUIMenu* mpMenuPlayers;         // 0x6D0 [R] 0; [U] onWidgetCreate: widget "MenuPlayers" (input map 1, scrollable); func_001BE540
                                    //       stores the player count into it (+ 0x96)
    cUIMenu* mpMenuRoomMenus;       // 0x6D4 [R] 0; [U] widget "MenuRoomMenus"; its selected item's + 0x18 picks the channel
                                    //       (func_001BD738 event 0xDF); onGainTransition cUIMenu_setSelectedByIndex(.., miSelectedChannel)
    cUIText* mpTextPlayer[10];      // 0x6D8 [R] 0; [U] widgets "TextPlayer%d" (their + 0x18 = the index); func_001BE540 writes the
                                    //       player names from miFirstPlayer on
    cUIText* mpTextCreateRoom;      // 0x700 [U] widget "TextCreateRoom" (its + 0x18 = 4)
    cUIText* mpText3InRoom;         // 0x704 [R] 0; [U] widget "3inroom"; func_001BE540 prints "kT_231PlayersInRoom" into it
    cUIListBox* mpChannelList[4];   // 0x708 [R] 0; [U] widgets "ListBoxBeginnerChannel", "ListBoxVoiceChannel",
                                    //       "ListBoxAdvancedChannel", "ListBoxExpertChannel"; func_001BE0A8 fills them, func_001BE038 / onGainTransition
                                    //       select rows (count byte + 0x318)
    cUIThing* mpChannelPair[4];     // 0x718 [R] 0; [U] widgets "PairBeginner", "PairVoice", "PairAdvanced", "PairExpert"
                                    //       (their + 0x18 = 0..3); onGainTransition passes mpChannelPair[miSelectedChannel] to func_001BE3A8
    cUIThing* mpPlayerUp;           // 0x728 [R] 0; [U] widget "player up"; func_001BE720 shows it while miFirstPlayer > 0
    cUIThing* mpPlayerDown;         // 0x72C [R] 0; [U] widget "player down"; func_001BE720
    cUIThing* mpChannelArrow[4];    // 0x730 [R] 0; [U] widgets "ArrowBeginner", "ArrowVoice", "ArrowAdvanced", "ArrowElite";
                                    //       func_001BDF60 / func_001BDFD8 show the selected channel's
    cUIThing* mpChannelLock[4];     // 0x740 [R] 0; [U] widgets "LockBeginner", "LockVoice", "LockAdvanced", "LockElite";
                                    //       func_001BE3A8
    unsigned int miChannelRow[4];   // 0x750 [R] 0; [U] func_001BE038: the selected row of each channel list, clamped to
                                    //       its count and selected again (cUIListBox_setCurrentEntryByIndex)
    int mbRoomChanged;              // 0x760 [U] onWidgetEvent cFEStateLobbyMenu_onWidgetEvent sets it with miSelectedChannel / miSelectedRoom;
                                    //       onGainTransition cFEStateLobbyMenu_onGainTransition reselects them when set and clears it
    int miSelectedChannel;          // 0x764 [U] cFEStateLobbyMenu_onWidgetEvent / func_001BD738 (event 0xDF: the RoomMenus row); < 4 indexes the
                                    //       channel arrays
    int miSelectedRoom;             // 0x768 [U] cFEStateLobbyMenu_onWidgetEvent stores the room list's selected row (list + 0x319); onGainTransition
                                    //       clamps it to the list count and selects it
    char* mszChannelName;           // 0x76C [R] cBXString (empty rep cBXString_EmptyString; destroyed by cFEStateLobbyMenu__cFEStateLobbyMenu); [U] func_001BD738
                                    //       assigns the selected list entry's text (cUIListBox_getCurrentAsciiText)
    int mbRoomHasPassword;          // 0x770 [U] func_001BD460 / func_001BD540: 1 when a password was given when entering the room;
                                    //       func_001BD8C8 asks for the password ("kT_POPUP231RoomPassword") only when it is 0
    int miRoomId;                   // 0x774 [U] onWidgetEvent cFEStateLobbyMenu_onWidgetEvent stores the chosen room id and asks LobbyMan for it
                                    //       (func_0025D770); func_001BD738 event 0xE0 / onWidgetMenu cFEStateLobbyMenu_onWidgetMenu pass it on
    int miFirstPlayer;              // 0x778 [R] 0; [U] the players list's scroll offset: func_001BE540 shows players
                                    //       miFirstPlayer..+9, func_001BE720 the up/down arrows; cFEStateLobbyMenu_onWidgetEvent / func_001BE3A8 reset it
    int miNumPlayers;               // 0x77C [U] func_001BE540: LobbyMan's player count in the room (func_0025F1B0); func_001BE720
    unsigned short mustrPopupText[0x200]; // 0x780 [U] func_001BD8C8: sprintf1 (unicode) of "kT_POPUP231RoomPassword" with
                                    //       mszRoomName, the popup's message (+ 0xC4); extent: up to 0xB80
    char* mszRoomName;              // 0xB80 [R] cBXString (empty rep cBXString_EmptyString; destroyed by cFEStateLobbyMenu__cFEStateLobbyMenu); [U] func_001BD460 /
                                    //       func_001BD540 assign the room being entered; func_001BD428 passes it to func_001BD540
};

CHECK_OFFSET(cFEStateLobbyMenu, mpPrev, 0x000);
CHECK_OFFSET(cFEStateLobbyMenu, mpNext, 0x004);
CHECK_OFFSET(cFEStateLobbyMenu, mVtbl, 0x008);
CHECK_OFFSET(cFEStateLobbyMenu, mHashName, 0x00C);
CHECK_OFFSET(cFEStateLobbyMenu, mpEngine, 0x010);
CHECK_OFFSET(cFEStateLobbyMenu, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateLobbyMenu, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateLobbyMenu, unk_016, 0x016);
CHECK_OFFSET(cFEStateLobbyMenu, miMiscData, 0x018);
CHECK_OFFSET(cFEStateLobbyMenu, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateLobbyMenu, mpParentState, 0x020);
CHECK_OFFSET(cFEStateLobbyMenu, mScreens, 0x024);
CHECK_OFFSET(cFEStateLobbyMenu, mpScreen, 0x040);
CHECK_OFFSET(cFEStateLobbyMenu, mUserNum, 0x044);
CHECK_OFFSET(cFEStateLobbyMenu, unk_045, 0x045);
CHECK_OFFSET(cFEStateLobbyMenu, unk_048, 0x048);
CHECK_OFFSET(cFEStateLobbyMenu, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateLobbyMenu, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateLobbyMenu, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateLobbyMenu, unk_058, 0x058);
CHECK_OFFSET(cFEStateLobbyMenu, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateLobbyMenu, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateLobbyMenu, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateLobbyMenu, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateLobbyMenu, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateLobbyMenu, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateLobbyMenu, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateLobbyMenu, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateLobbyMenu, mMsgList, 0x6C8);
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

#endif /* FE_FESTATENETLOBBY_H */
