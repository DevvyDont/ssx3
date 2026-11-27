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
// instance is D_004A3328, destroyed by func_00261008 (operator delete through BuddyMan__BuddyMan). No base class, no
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

// The g++ 2.95 STL vector whose four words BuddyMan writes out (mMessages*, mChallengers*), for the code that takes the
// vector itself: the vector<..., BXAllocator<..>> instantiations' out-of-line members, e.g. _M_insert_aux ([P] preview
// .gnu.linkonce.t._M_insert_aux__t6vector2ZPC15cPrivateMessageZt11BXAllocator1ZP9cBXString... and
// ...ZPC11cChallengerZt11BXAllocator1ZP11cChallenger...; retail func_00267128 / func_002672C8). [L] gcc-2.95.3
// libstdc++/stl/stl_vector.h: with an instance allocator, vector<_Tp, _Alloc> derives from _Vector_base -> 
// _Vector_alloc_base<_Tp, _Alloc, false> {_Alloc _M_data_allocator; _Tp* _M_start; _Tp* _M_finish;
// _Tp* _M_end_of_storage;}; BXAllocator is one word. Written flat with the element type as the only parameter (the
// allocator word as an int, as BuddyMan's m*Alloc members); a unit that includes the real <vector> must not include
// this definition. Use it as (vector<const cChallenger*>*)&buddyMan->mChallengersAlloc.
template <class _Tp>
struct vector {
    int _M_data_allocator;          // 0x0 [L] the BXAllocator (one word; never accessed)
    _Tp* _M_start;                  // 0x4 [L]; [R] _M_insert_aux reallocates and stores it
    _Tp* _M_finish;                 // 0x8 [L]; [R] compared with _M_end_of_storage, advanced on insert
    _Tp* _M_end_of_storage;         // 0xC [L]; [R] start + the doubled capacity after a reallocation
};

// CHECK_SIZE / CHECK_OFFSET cannot paste a template-id, so the checks are spelled out.
#ifndef DECOMP_NO_LAYOUT_CHECKS
typedef char check_vector_cChallenger_size[(sizeof(vector<const cChallenger*>) == 0x10) ? 1 : -1];
typedef char check_vector_cChallenger_M_finish[(DECOMP_OFFSETOF(vector<const cChallenger*>, _M_finish) == 0x8) ? 1 : -1];
typedef char check_vector_cChallenger_M_end_of_storage[(DECOMP_OFFSETOF(vector<const cChallenger*>, _M_end_of_storage) == 0xC) ? 1 : -1];
#endif

struct BuddyApiMsgT;

// The error a roster change reports (func_00261EF8's out word, from func_002620D8; names ours). [U] func_00261EF8:
// 0 on success, 1 when the list is full, 2 when the flag-0x100000 entries are full, 3 for the player's own persona;
// the FE shows "kT_POPUP235MaxBuddiesExceeded" (func_001ABED8) for kBuddyRosterFull.
enum eBuddyRosterError {
    kBuddyRosterOk = 0,
    kBuddyRosterFull = 1,
    kBuddyRosterTempFull = 2,
    kBuddyRosterSelf = 3
};

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

    // Methods bound to their retail symbols by asm labels.
    void onconnect(int* msg) __asm__("BuddyMan_onconnect");
    void onrosterupdate(int* msg) __asm__("BuddyMan_onrosterupdate");
    void onReceiveMessage(BuddyApiMsgT* msg) __asm__("BuddyMan_onReceiveMessage");
    void onBuddySend() __asm__("BuddyMan_onBuddySend__FPv");
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

// cChallenger: one pending race challenge received from another player (BuddyMan::mChallengers*). Only BuddyMan's
// challenge handler func_00262AF0 (called by LobbyMan_onchatmsg with the fields of a "challenge" private message) makes
// one: `operator new(0x40, "cChallenger", 0x20000000)` with the constructor inlined (vtable, two cBXStrings, the words
// 0x8..0x34); func_00263018 / func_00261530 delete it through slot 1 (the only virtual: cChallenger__cChallenger, in
// unit bx/bxstringctor). Readers: func_00262E20 / func_00263128 / func_00263270 / func_00263308 (the name at 0x0) and
// cFEStateChallengeReceivedPopup_onCreateScreen (fe/feasyncfile.cpp, view sRaceSetup_FE68: the race settings).
// No EA path (no out-of-line method besides the destructor); BuddyMan's code is in EA's online/lobbyman.cpp.
// Not in SSX On Tour (no [O]); member names are from retail use.
//
// Size 0x40: [R] the allocation in func_00262AF0. No base class: its one virtual (the destructor; vtable cChallenger_vtbl
// 0x004810E0, 1 slot) puts the vptr after the data (g++ 2.95), at 0x3C ([R] func_00262AF0 and the destructor store it).
// cBXString members are written as their one word (char* Str: bx/bxstring.h), as in online/lobbyman.h.
// Tags: [R] func_00262AF0's stores, the destructor; [U] how retail code uses the field.
struct cChallenger {
    char* mName;                    // 0x00 [R] cBXString(name) (cBXString_cBXString2(this, name)); dtor destroys it;
                                    //      [U] func_00262E20 / func_00263128 / func_00263270 compare it with the
                                    //      challenger's name (strcasecmp / strcmp)
    char* mMessage;                 // 0x04 [R] cBXString(messageText) (this + 4): the whole chat message; dtor destroys
                                    //      it; [U] the challenge popup copies it to LobbyMan::mChallengeMessage
                                    //      (view `name`)
    int miLocation;                 // 0x08 [R] the message's 1st number; [U] the popup stores it in
                                    //      cBENewRaceInterface_newRace.iLocationIdx (view f8)
    int miGameMode;                 // 0x0C [R] 2nd number; [U] the popup's setGameMode (view mode)
    int miGameEvent;                // 0x10 [R] 3rd number; [U] the popup's setGameEvent (view event)
    int mbRanked;                   // 0x14 [R] 4th number as 0/1; [U] the popup: cBENetworkInterface_networkOpt
                                    //      .bIsRankedGame (view f14)
    int mbMultipliers;              // 0x18 [R] 5th number as 0/1; [U] the popup: race-option bit 6 of cBERules_mRules (the
                                    //      multipliers bit, festatenetchallenge mpLBMultipliers) (view f18)
    int mbPowerUps;                 // 0x1C [R] 6th number as 0/1; [U] the popup: race-option bit 8 (power-ups) (view f1C)
    int miNumberAI;                 // 0x20 [R] 7th number; [U] the popup's cBENewRaceInterface_setNumberAI (view numAI)
    int mbPointIcons;               // 0x24 [R] 8th number as 0/1; [U] the popup: race-option bit 7 (point icons) (view f24)
    // 0x28..0x3B: the challenger's lobby stats, a tLobbyStats (online/lobbyman.h, 0x14) that the popup block-copies into
    // LobbyMan::mOpponentStats (view info[5]); written out as its five words with tLobbyStats' names.
    int mRank;                      // 0x28 [R] the 4th number after the signature ("rank"); [U] tLobbyStats::mRank
    int unk_02C;                    // 0x2C [R] 1st number after the signature (tLobbyStats::unk_04, "STAT" field 2)
    int unk_030;                    // 0x30 [R] 2nd number after it (tLobbyStats::unk_08, "STAT" fields 2 + 3 + 4)
    int unk_034;                    // 0x34 [R] 3rd number after it (tLobbyStats::unk_0C, "STAT" field 6)
    int unk_038;                    // 0x38 never written by func_00262AF0; copied by the popup as tLobbyStats::unk_10
    void** mVtbl;                   // 0x3C [R] cChallenger_vtbl (func_00262AF0, the dtor); [U] func_00263018 deletes
                                    //      through slot 1 (flags 3)
};

CHECK_OFFSET(cChallenger, mName, 0x00);
CHECK_OFFSET(cChallenger, mMessage, 0x04);
CHECK_OFFSET(cChallenger, miLocation, 0x08);
CHECK_OFFSET(cChallenger, miGameMode, 0x0C);
CHECK_OFFSET(cChallenger, miGameEvent, 0x10);
CHECK_OFFSET(cChallenger, mbRanked, 0x14);
CHECK_OFFSET(cChallenger, mbMultipliers, 0x18);
CHECK_OFFSET(cChallenger, mbPowerUps, 0x1C);
CHECK_OFFSET(cChallenger, miNumberAI, 0x20);
CHECK_OFFSET(cChallenger, mbPointIcons, 0x24);
CHECK_OFFSET(cChallenger, mRank, 0x28);
CHECK_OFFSET(cChallenger, unk_02C, 0x2C);
CHECK_OFFSET(cChallenger, unk_030, 0x30);
CHECK_OFFSET(cChallenger, unk_034, 0x34);
CHECK_OFFSET(cChallenger, unk_038, 0x38);
CHECK_OFFSET(cChallenger, mVtbl, 0x3C);
CHECK_SIZE(cChallenger, 0x40);

#endif /* ONLINE_BUDDYMAN_H */
