#ifndef ONLINE_BUDDYMAN_H
#define ONLINE_BUDDYMAN_H

// BuddyMan: the EA Online buddy-list manager (connection to the buddy server, the roster, private messages and
// pending challenges). EA's class name is BuddyMan (no `c`): the July 2003 preview keeps its functor callbacks
// invoke__Q28BuddyMant12BuddyFunctor1PM8BuddyMan...onconnect / onReceiveMessage / onrosterupdate / onBuddyAdd /
// onBuddyDelete / onBuddySend. Its code is in EA's d:/ssx/code/source/online/lobbyman.cpp (the preview's lobbyman.cpp
// string pool holds "!mBuddyMan", "BuddyMan", "state == idle", "state == connecting", "buddyapi"); this header's
// path online/buddyman.h is a guess. Retail: unit dirtysock/tagsunk, 0x261008-0x263660.
//
// Size 0xF0: func_00260F80 (the singleton creator) does operator new(0xF0, "BuddyMan", 0x20000000) and inlines the
// constructor (0x40 = -1, 0xA4 = 0x15180, 0xEC = the empty cBXString rep cBXString_EmptyString, zeroes 0xAC..0xE8); the
// instance is D_004A3328, destroyed by func_00261008 (operator delete through func_002689B8). No base class, no
// vtable. The preview's BuddyMan is a smaller 0x34-byte class (operator new(0x34, "BuddyMan")); its connect /
// disconnect / onconnect have the same shape, which places mpBuddyApi, mpRoster, miError, mfTimeout, mState and
// mbDisconnected (preview 0x18..0x2C); retail added the connect strings in front and the vectors' layout changed.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the inlined constructor in func_00260F80 and the allocation size above;
//   [U] how retail code uses the field (BuddyMan methods, LobbyMan LobbyMan_onNewsRetrieved / func_0025AC50, the FE states
//       through D_004A3328);
//   [P] the preview's matching code (its assert strings name `state`); no preview accessors exist for BuddyMan.
// STL containers (g++ 2.95 vector<T*, BXAllocator<...>>: a 4-byte allocator, then _M_start, _M_finish,
// _M_end_of_storage) are written out as their four words.

#include "layout.h"

struct BuddyApiRefT;
struct cPrivateMessage;
struct cChallenger;

struct BuddyMan {
    char mServerName[0x40];             // 0x000 [U] connect (func_00261248) strcpy's its host argument (LobbyMan
                                        //       passes the "BUDDYSERVERNAME" tag); func_00261058 clears byte 0
    int miPort;                         // 0x040 [R] ctor -1; [U] connect stores the "BUDDYPORT" number,
                                        //       func_00261058 -1, reconnect (func_002613C8) needs it >= 0
    char mUserName[0x20];               // 0x044 [U] connect strcpy's LobbyMan's persona name (LobbyMan + 0x80);
                                        //       reconnect passes it back; func_00261058 clears byte 0
    char mPassword[0x40];               // 0x064 [U] connect strcpy's LobbyMan + 0x4C; func_00261058 clears byte 0
    int miMsgTimeout;                   // 0x0A4 [R] ctor 0x15180 (one day); [U] LobbyMan LobbyMan_onNewsRetrieved stores the
                                        //       "BUDDYMSGTIMEOUT" number; sendMessage (func_00262690) passes it
    // --- vector<cPrivateMessage*>: received private messages (cPrivateMessage: 3 cBXStrings + vptr, 0x10 bytes,
    //     operator new(0x10, "cPrivateMessage") in func_00261CD0)
    int mMessagesAlloc;                 // 0x0A8 [U] the vector's allocator word (func_00261CD0's view: alloc); never
                                        //       read or written
    cPrivateMessage** mMessagesStart;   // 0x0AC [R] ctor 0; [U] func_00263630 / func_00262938 (size) / func_002635A8
    cPrivateMessage** mMessagesFinish;  // 0x0B0 [R] ctor 0; [U] push_back in func_00261CD0, erase in func_002635A8
    cPrivateMessage** mMessagesEnd;     // 0x0B4 [R] ctor 0; [U] func_00261CD0 compares finish with it (capacity)
    // --- vector<cChallenger*>: pending challenges (cChallenger: 0x40 bytes, vptr at 0x3C, operator new(0x40,
    //     "cChallenger") in func_00262AF0)
    int mChallengersAlloc;              // 0x0B8 [U] the vector's allocator word (views' `alloc`); never accessed
    cChallenger** mChallengersStart;    // 0x0BC [R] ctor 0; [U] func_00263308 / func_00262E20; FE states read it
    cChallenger** mChallengersFinish;   // 0x0C0 [R] ctor 0; [U] push_back in func_00262AF0, erase in func_00263018
    cChallenger** mChallengersEnd;      // 0x0C4 [R] ctor 0; [U] func_00262AF0 compares finish with it
    BuddyApiRefT* mpBuddyApi;           // 0x0C8 [R] ctor 0; [U] func_002614E8 stores the handle func_003F0710 creates
                                        //       (version "2003-10-20"); every BuddyApi call passes it;
                                        //       [P] preview 0x18 (same role)
    void* mpRoster;                     // 0x0CC [R] ctor 0; [U] onconnect ('auth' ok) stores func_003F0C90's roster;
                                        //       func_00262598 counts it, func_002625C8 indexes it; [P] preview 0x1C
    int miError;                        // 0x0D0 [R] ctor 0; [U] onconnect stores the auth error / 1 on a lost
                                        //       connection, connect and disconnect clear it, LobbyMan func_0025AC50
                                        //       reads it; [P] preview 0x20
    float mfTimeout;                    // 0x0D4 [U] connect stores 30.0f (swc1); [P] preview 0x24 (30.0f, same place)
    int mState;                         // 0x0D8 [R] ctor 0; [U] connect 1 (connecting), onconnect 'auth' ok and
                                        //       disconnect 0, isConnected (func_00261460) tests it != 1;
                                        //       [P] preview 0x28 asserted by "state == idle" (connect) and
                                        //       "state == connecting" (onconnect)
    int mbDisconnected;                 // 0x0DC [R] ctor 0; [U] onconnect sets it on a lost connection or failed
                                        //       login, update (func_00261490) then disconnects and clears it;
                                        //       [P] preview 0x2C (same flow)
    int mbLoggedIn;                     // 0x0E0 [R] ctor 0; [U] onconnect 'auth' ok 1, connect / disconnect 0;
                                        //       isConnected needs it
    int unk_0E4;                        // 0x0E4 [R] ctor 0; no other access seen
    int mbRosterDone;                   // 0x0E8 [R] ctor 0; [U] onrosterupdate (BuddyMan_onrosterupdate) sets it at the first
                                        //       'endr' message, onconnect / disconnect clear it; FE states read it
    char* mResource;                    // 0x0EC [R] ctor stores the empty cBXString rep; a cBXString: [U] connect
                                        //       assigns the "BUDDYRESOURCE" string, setPresence / addBuddy read it
};

CHECK_OFFSET(BuddyMan, mServerName, 0x000);
CHECK_OFFSET(BuddyMan, miPort, 0x040);
CHECK_OFFSET(BuddyMan, mUserName, 0x044);
CHECK_OFFSET(BuddyMan, mPassword, 0x064);
CHECK_OFFSET(BuddyMan, miMsgTimeout, 0x0A4);
CHECK_OFFSET(BuddyMan, mMessagesAlloc, 0x0A8);
CHECK_OFFSET(BuddyMan, mMessagesStart, 0x0AC);
CHECK_OFFSET(BuddyMan, mMessagesFinish, 0x0B0);
CHECK_OFFSET(BuddyMan, mMessagesEnd, 0x0B4);
CHECK_OFFSET(BuddyMan, mChallengersAlloc, 0x0B8);
CHECK_OFFSET(BuddyMan, mChallengersStart, 0x0BC);
CHECK_OFFSET(BuddyMan, mChallengersFinish, 0x0C0);
CHECK_OFFSET(BuddyMan, mChallengersEnd, 0x0C4);
CHECK_OFFSET(BuddyMan, mpBuddyApi, 0x0C8);
CHECK_OFFSET(BuddyMan, mpRoster, 0x0CC);
CHECK_OFFSET(BuddyMan, miError, 0x0D0);
CHECK_OFFSET(BuddyMan, mfTimeout, 0x0D4);
CHECK_OFFSET(BuddyMan, mState, 0x0D8);
CHECK_OFFSET(BuddyMan, mbDisconnected, 0x0DC);
CHECK_OFFSET(BuddyMan, mbLoggedIn, 0x0E0);
CHECK_OFFSET(BuddyMan, unk_0E4, 0x0E4);
CHECK_OFFSET(BuddyMan, mbRosterDone, 0x0E8);
CHECK_OFFSET(BuddyMan, mResource, 0x0EC);
CHECK_SIZE(BuddyMan, 0xF0);

// The buddy-list manager (BuddyMan::mBuddyMan; preview assert "!mBuddyMan"): set when func_00260F80 creates it.
extern BuddyMan* BuddyMan_mBuddyMan;

#endif /* ONLINE_BUDDYMAN_H */
