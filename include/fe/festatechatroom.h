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
// the default func_001DF580 (fe/fepopupmisc); destructor cFEStateChatRoom__cFEStateChatRoom (slot 1; the preview's _$_16cFEStateChatRoom).
// Retail vtable cFEStateChatRoom_vtbl (32 slots: 4 onCreateScreen cFEStateChatRoom_onCreateScreen, 6 onGainFocus cFEStateChatRoom_onGainFocus, 7 cFEStateChatRoom_onGainTransition,
// 8 cFEStateChatRoom_onInputBegin, 12 onUpdate cFEStateChatRoom_onUpdate, 13 func_001DF5C0, 17 onPopupEvent cFEStateChatRoom_onPopupEvent, 18 onWidgetCreate
// cFEStateChatRoom_onWidgetCreate, 19 onWidgetEvent cFEStateChatRoom_onWidgetEvent, 20 onWidgetMenu cFEStateChatRoom_onWidgetMenu, 25 func_001B2DA0 (help), 31
// func_001B3A30 (net events), 32 func_001B22A0). Methods: fe/feasyncfile 0x1B1F88-0x1B3D20.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). There is no On Tour DWARF for
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
#include "fe/festatenet.h"
#include "ui/uithing.h"

struct cUIThing;
struct cUIMenu;

struct cFEStateChatRoom {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateChatRoom_vtbl (vtables.tsv; 32 slots), stored by cFEStateChatRoom_cFEStateChatRoom /
                                    //       func_001DF580 after the cFEStateNet ctor and by the destructor
                                    //       cFEStateChatRoom__cFEStateChatRoom
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateChatRoom_cFEStateChatRoom stores
                                    //       0x3D
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
    // --- cFEStateChatRoom
    cUIThing* mpQualityIcon[6];     // 0x6D0 [U] onWidgetCreate cFEStateChatRoom_onWidgetCreate: widgets "quality1".."quality6" (sprintf
                                    //       "quality%d"); func_001B3BF0 shows / hides icon i for the visible player
                                    //       row i and sets its quality (func_001AAD50)
    cUIText* mpTextRoomName;        // 0x6E8 [U] onWidgetCreate: widget "TextRoomName"; func_001B2378 sets the room
                                    //       name text on it
    cUIThing* mpSlider1;            // 0x6EC [R] 0; [U] onWidgetCreate: widget "slider1" (the player list's scroll
                                    //       bar; attr bit 0 set); onCreateScreen cFEStateChatRoom_onCreateScreen stores its arrows /
                                    //       outline at + 0x7C/0x80/0x84 and 2 at + 0x88; func_001B3D20 updates it
                                    //       from miMenuScroll[0] (cUIScrollBar_setValues)
    cUIText* mpTextScrollChat;      // 0x6F0 [R] 0; [U] onWidgetCreate: widget "TextScrollChat" (the chat history);
                                    //       onCreateScreen gives it mChatColour (+ 0xA8) and mpSlider2 (+ 0xB8);
                                    //       func_001B2CD0 appends "name: message" lines to it (cUITextScroll_addAsciiString,
                                    //       func_001A83D8)
    cUIThing* mpSlider2;            // 0x6F4 [R] 0; [U] onWidgetCreate: widget "slider2" (the chat history's scroll
                                    //       bar); onCreateScreen sets it up like mpSlider1
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
};

CHECK_OFFSET(cFEStateChatRoom, mpPrev, 0x000);
CHECK_OFFSET(cFEStateChatRoom, mpNext, 0x004);
CHECK_OFFSET(cFEStateChatRoom, mVtbl, 0x008);
CHECK_OFFSET(cFEStateChatRoom, mHashName, 0x00C);
CHECK_OFFSET(cFEStateChatRoom, mpEngine, 0x010);
CHECK_OFFSET(cFEStateChatRoom, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateChatRoom, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateChatRoom, unk_016, 0x016);
CHECK_OFFSET(cFEStateChatRoom, miMiscData, 0x018);
CHECK_OFFSET(cFEStateChatRoom, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateChatRoom, mpParentState, 0x020);
CHECK_OFFSET(cFEStateChatRoom, mScreens, 0x024);
CHECK_OFFSET(cFEStateChatRoom, mpScreen, 0x040);
CHECK_OFFSET(cFEStateChatRoom, mUserNum, 0x044);
CHECK_OFFSET(cFEStateChatRoom, unk_045, 0x045);
CHECK_OFFSET(cFEStateChatRoom, unk_048, 0x048);
CHECK_OFFSET(cFEStateChatRoom, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateChatRoom, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateChatRoom, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateChatRoom, unk_058, 0x058);
CHECK_OFFSET(cFEStateChatRoom, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateChatRoom, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateChatRoom, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateChatRoom, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateChatRoom, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateChatRoom, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateChatRoom, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateChatRoom, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateChatRoom, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateChatRoom, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateChatRoom, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateChatRoom, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateChatRoom, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateChatRoom, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateChatRoom, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateChatRoom, mMsgList, 0x6C8);
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
