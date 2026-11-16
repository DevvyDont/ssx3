#ifndef ONLINE_LOBBYMAN_H
#define ONLINE_LOBBYMAN_H

// LobbyMan: the online lobby client (EA's d:/ssx/code/source/online/lobbyman.cpp, from the July 2003 preview's
// assert strings). It drives DirtySock's LobbyApi: connect to ps2ssx04.ea.com, create/edit/recover accounts,
// authenticate, select/create/delete personas, rooms, news, rank lists, find player, challenges and quick match;
// each server reply arrives through a LobbyFunctor callback (preview: invoke__Q28LobbyMant12LobbyFunctor1...
// on*__8LobbyManP12LobbyApiMsgTP12LobbyApiRefTP12LobbyApiMsgTPv) and is posted to the front end as an
// online::Event on mEventList. No C++ base class and no virtual functions (no vptr store in the constructor).
//
// Size 0x300: [R] func_0025B650 creates the singleton with cMemMan_alloc(0x300, "LobbyMan", 0x20000000) +
// the constructor LobbyMan_LobbyMan (which stores every field up to the second tLobbyAccount, 0x2B0 + 0x50 = 0x300);
// func_0025B688 destroys it (destructor LobbyMan__LobbyMan). Instance: D_004A3028 (EA: LobbyMan::mLobbyMan, preview
// assert "!mLobbyMan" in the create function).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail's constructor LobbyMan_LobbyMan / destructor LobbyMan__LobbyMan / func_00260F10 (account-form reset);
//   [P] the July 2003 preview's asserts in lobbyman.cpp, read at the preview offset: "state == ..." (preview 0x4),
//       "authenticated" (lw 0x8), "loggedin" (lw 0xC), "!disconnected" (lw 0x10), "idx >= 0 && idx < numpersonas"
//       / "numpersonas < max_personas" (lw 0x40), "idx < getnumrooms()" (lw 0x30). From 0x14 on retail inserted
//       0x2C bytes (preview 0x28 lobbyapi / 0x30 rooms / 0x40 numpersonas = retail 0x54 / 0x5C / 0x6C);
//   [U] how retail code uses the field (the LobbyApi tag names it sends or parses are quoted).
// There is no On Tour DWARF for this class (On Tour has no lobby). Unnamed fields are unk_XXX.
//
// mState values (retail; names from the preview asserts where the request matches): 0 after disconnect,
// 1 ready, 2/3 connecting (connect without / with a known server address), 4 creating_account ("acct"),
// 5 editing_account ("edit"), 6 requesting_lost_account ("lost" by MAIL), 7 requesting_lost_password ("lost" by
// NAME), 8 authenticating ("auth"), 9 selecting_persona ("pers"), 10 creating_persona ("cper"), 11 deleting_persona
// ("dper"), 12 selecting_room ("move"), 14 retrieving_news ("news"), 16 reporting_abuse ("rept"), 17 creating_room
// ("room"), 18 requesting_quickmatch ("quik"), 19 awaiting_quickmatch (preview; retail's
// LobbyMan_onQuickMatchResponse moves 18 straight to 20), 20/22/23/24 challenge states
// (22 = awaiting_game: preview li 22 before "state == awaiting_game"), 25 in game (onplaymsg), 27 "onln".

#include "layout.h"

struct LobbyApiRefT;    // DirtySock LobbyApi instance (preview mangled name P12LobbyApiRefT)

// An account form (0x50 bytes; field names are ours, from the LobbyApi tags): mNewAccount at 0x260 is what the
// account screens fill and pass to createaccount (func_0025B9C8(D_004A3028, D_004A3028 + 0x260)) /
// editaccount (func_0025BF18); mAccount at 0x2B0 is the logged-in account (onauthentication fills it).
// [R] built by cBXString_cBXString__FP10cBXString2 (bx/bxstringctor.cpp: 5 strings + a string[4]); the
// LobbyMan destructor destroys the 5 strings one by one and the [4] as an array.
struct tLobbyAccount {
    char* mName;            // 0x00 [U] "NAME" (createaccount, editaccount); onauthentication copies mUserName
    char* mPassword;        // 0x04 [U] "PASS" (createaccount); editaccount sends it as "CHNG" (the new password)
    char* unk_08;           // 0x08 [U] onauthentication copies mPassword into it (a confirm/old-password field?)
    char* mMail;            // 0x0C [U] "MAIL" (createaccount, editaccount); onauthentication stores the server's
    char* mParentMail;      // 0x10 [U] "PMAIL" (createaccount, sent when non-empty)
    char* mNameOptions[4];  // 0x14 [U] "OPTS": func_0025BC48 splits the server's comma list of suggested names
                            //      (onaccountcreated after a refusal); [R] destroyed as an array
    int mNumNameOptions;    // 0x24 [U] func_0025BC48 counts them (up to 4)
    int mBornMonth;         // 0x28 [U] "BORN" "%4d%02d%02d" second field; [R] func_00260F10 sets 1
    int mBornDay;           // 0x2C [U] "BORN" third field; [R] func_00260F10 sets 1
    int mBornYear;          // 0x30 [U] "BORN" first field; [R] func_00260F10 sets 0x7C1 (1985)
    int mGender;            // 0x34 [U] "GEND": non-zero 'F', else 'M'; [R] func_00260F10 sets 0
    int mSpam[2];           // 0x38 [U] "SPAM" "%c%c" ('Y'/'N' each); [R] func_00260F10 sets 1, 1;
                            //      onauthentication stores the server's two flags (0x2E8 / 0x2EC)
    int unk_40;             // 0x40 [R] func_00260F10 sets 0
    int mChangePassword;    // 0x44 [U] editaccount sends "PASS"/"CHNG" only when set
    int mChangeSpam;        // 0x48 [U] editaccount sends "SPAM" only when set
    int mChangeMail;        // 0x4C [U] editaccount sends "MAIL" only when set
};

CHECK_OFFSET(tLobbyAccount, mName, 0x00);
CHECK_OFFSET(tLobbyAccount, mPassword, 0x04);
CHECK_OFFSET(tLobbyAccount, unk_08, 0x08);
CHECK_OFFSET(tLobbyAccount, mMail, 0x0C);
CHECK_OFFSET(tLobbyAccount, mParentMail, 0x10);
CHECK_OFFSET(tLobbyAccount, mNameOptions, 0x14);
CHECK_OFFSET(tLobbyAccount, mNumNameOptions, 0x24);
CHECK_OFFSET(tLobbyAccount, mBornMonth, 0x28);
CHECK_OFFSET(tLobbyAccount, mBornDay, 0x2C);
CHECK_OFFSET(tLobbyAccount, mBornYear, 0x30);
CHECK_OFFSET(tLobbyAccount, mGender, 0x34);
CHECK_OFFSET(tLobbyAccount, mSpam, 0x38);
CHECK_OFFSET(tLobbyAccount, unk_40, 0x40);
CHECK_OFFSET(tLobbyAccount, mChangePassword, 0x44);
CHECK_OFFSET(tLobbyAccount, mChangeSpam, 0x48);
CHECK_OFFSET(tLobbyAccount, mChangeMail, 0x4C);
CHECK_SIZE(tLobbyAccount, 0x50);

// A player's lobby stats (0x14 bytes, copied as a whole: cFEStateChallengeReceivedPopup_onCreateScreen / the setup manager func_002642B8 store
// the opponent's into mOpponentStats with one block copy). Filled from the "onln" reply (func_0025D048).
struct tLobbyStats {
    int mRank;              // 0x00 [U] tag "R"; the setup manager sends it to the opponent (func_00264CB8)
    int unk_04;             // 0x04 [U] "STAT" field 2
    int unk_08;             // 0x08 [U] "STAT" fields 2 + 3 + 4
    int unk_0C;             // 0x0C [U] "STAT" field 6
    int unk_10;             // 0x10 [U] func_0025D048 stores -1
};

CHECK_OFFSET(tLobbyStats, mRank, 0x00);
CHECK_OFFSET(tLobbyStats, unk_04, 0x04);
CHECK_OFFSET(tLobbyStats, unk_08, 0x08);
CHECK_OFFSET(tLobbyStats, unk_0C, 0x0C);
CHECK_OFFSET(tLobbyStats, unk_10, 0x10);
CHECK_SIZE(tLobbyStats, 0x14);

struct LobbyMan {
    int mState;                     // 0x000 [P] "state == ..." asserts (preview offset 0x4); [R] ctor 0;
                                    //       [U] func_0025B608 (send request) stores the new state; values above
    int mServerAddr;                // 0x004 [R] ctor 0; [U] connect (func_0025B6D8) / update (func_0025AC50) store
                                    //       the lobby server's address and connect to it on port 11000 (0x2AF8);
                                    //       the online pause-error overlay pings it (func_002583A8)
    int mAuthenticated;             // 0x008 [P] assert "authenticated" (lw 0x8); [U] onauthentication sets 1,
                                    //       disconnect (func_0025B800) clears
    int mLoggedIn;                  // 0x00C [P] assert "loggedin" (lw 0xC); [U] onNewsRetrieved ("new0") sets 1,
                                    //       disconnect clears; update polls the room/user lists only when set
    int mDisconnected;              // 0x010 [P] assert "!disconnected" (lw 0x10); [U] onconnectionmsg sets 1,
                                    //       update clears it and disconnects
    int unk_014;                    // 0x014 [R] ctor 0; [U] connect stores its argument (non-zero: use the known
                                    //       address, state 3; zero: look the server name up, state 2)
    int mHaveCPAT;                  // 0x018 [R] ctor 0; [U] onauthentication: 1 when the reply has "CPAT";
                                    //       selectpersona (func_0025CD50) then sends mCPAT as "CDEV"
    char mCPAT[0x24];               // 0x01C [U] onauthentication decodes "CPAT" into it (func_003EFD30, up to 0x21)
    int mRequestId;                 // 0x040 [U] func_0025B608 stores the LobbyApi request (func_003E8B10); the
                                    //       cancel paths pass it to func_003E8D70; the rank list keeps it per slot
    int mFindingPlayer;             // 0x044 [R] ctor 0; [U] findplayer ("user", func_0025E6D0) sets 1,
                                    //       onPlayerFound clears, update times it out
    char* mUserName;                // 0x048 [R] ctor empty cBXString; [U] "NAME" in login/editaccount; 16 readers
                                    //       outside the unit copy it (front-end account screens)
    char* mPassword;                // 0x04C [R] ctor empty cBXString; [U] "PASS" in login/editaccount; the buddy
                                    //       login (onNewsRetrieved) passes it to BuddyMan
    int mDeletePersonaIdx;          // 0x050 [U] deletepersona (func_0025D428) stores the index, onpersonadeleted
                                    //       removes that entry
    LobbyApiRefT* mpLobbyApi;       // 0x054 [P] preview "lobbyapi" (offset 0x28 there); [R] ctor 0;
                                    //       [U] func_0025A948 stores func_003E7FF0's handle; every request uses it
    void* mpHostLookup;             // 0x058 [R] ctor 0; [U] connect stores func_003F3A98(mServerName, 0) (a
                                    //       DirtySock host lookup: result +0x4, poll fn +0x8, release fn +0xC);
                                    //       update polls/frees it through those
    void* mpRoomList;               // 0x05C [P] getnumrooms() reads it (preview 0x30); [U] func_0025A778:
                                    //       func_003E85B0(api, 0, onroomlistupdate thunk); func_0025F120/F150/F180
    void* mpUserList;               // 0x060 [U] func_003E85B0(api, 1, onuserlistupdate thunk); func_0025F1B0..
    void* mpRankList;               // 0x064 [U] func_003E85B0(api, 2, onranklistupdate thunk)
    int unk_068;                    // 0x068 no access seen
    int mNumPersonas;               // 0x06C [P] "idx < numpersonas", "numpersonas < max_personas" (preview 0x40);
                                    //       [U] onauthentication counts "PERSONAS"; createpersona appends
    char* mPersonas[4];             // 0x070 [P] "idx >= 0 && idx < max_personas" (max_personas = 4); [R] ctor inits
                                    //       4 empty strings, dtor destroys them as an array; [U] "PERSONAS" list
    char* mPersona;                 // 0x080 [U] selectpersona copies the chosen name; sent as "PERS" (onln, buddy
                                    //       login, find player); 8 readers outside the unit
    char* mRoomName;                // 0x084 [U] onroomselected ("move" reply) stores "NAME", resets on error;
                                    //       func_0025DA90 returns it when connected and non-empty
    char* mGameName;                // 0x088 [U] onplaymsg: "NAME" of the game session
    char* unk_08C;                  // 0x08C [U] cFEStateChallengeReceivedPopup_onCreateScreen copies the buddy race setup's name into it;
                                    //       func_00263CF0 hands the word to the setup manager
    char* mHostName;                // 0x090 [U] onplaymsg: "HOST"
    char* mOppoName;                // 0x094 [U] onplaymsg: "OPPO"
    char* mOpponent;                // 0x098 [U] onplaymsg: the one of HOST/OPPO that is not "SELF"; challenge
                                    //       (func_0025F4C0) stores the challenged name; onchatmsg compares; 15 readers
    char* mAuth;                    // 0x09C [U] onplaymsg: "AUTH"
    int mWhen;                      // 0x0A0 [U] onplaymsg: "WHEN" (TagFieldGetTime)
    int mMatchType;                 // 0x0A4 [U] the game-kind argument of challenge (func_0025F4C0, func_0025FB58)
                                    //       and quickmatch (func_0025EE20); onplaymsg passes it to func_00259098
    int mHostAddr;                  // 0x0A8 [U] onplaymsg: "FROM" address (func_0025A730 may override it)
    int mOppoAddr;                  // 0x0AC [U] onplaymsg: "ADDR" address
    int mSeed;                      // 0x0B0 [U] onplaymsg: "SEED"
    int mIsHost;                    // 0x0B4 [U] onplaymsg: "SELF" == "HOST"; challenge sets 1
    tLobbyStats mStats;             // 0x0B8 [U] "onln" reply (func_0025D048); challenge sends them
    tLobbyStats mOpponentStats;     // 0x0CC [U] block-copied in by cFEStateChallengeReceivedPopup_onCreateScreen / func_002642B8
    float mTimeout;                 // 0x0E0 [U] func_0025B608 stores the request timeout (20.0), connect 30.0;
                                    //       update subtracts the frame time and gives up when it goes negative
    float unk_0E4;                  // 0x0E4 [U] update subtracts the frame time (no other writer seen)
    char* mServerName;              // 0x0E8 [R] ctor cBXString(.., "ps2ssx04.ea.com"); [U] connect looks it up
    char mEventList[0x8];           // 0x0EC [R] std::list<online::Event*, BXAllocator>: allocator word at 0xEC,
                                    //       node pointer at 0xF0 (ctor allocates the 0xC-byte head node "StlBlock",
                                    //       dtor func_001DB718 + delete); [U] every reply pushes an Event before
                                    //       the head node (*(node**)(this + 0xF0)); 37 readers outside the class
    void* mpConfig;                 // 0x0F4 [R] ctor 0; [U] func_0025A650 creates it (func_003E6E08) from the
                                    //       logged-in "pers" config; func_0025A730 looks keys up; func_0025A6F8 frees
    int mRankSlot;                  // 0x0F8 [R] ctor 0; [U] func_0025E348 selects the rank-list slot (0..28)
    int unk_0FC;                    // 0x0FC [R] ctor 0 (no other access)
    void* mpSlotTag;                // 0x100 [R] ctor 0; [U] func_0025E348 stores func_003E8F08(api, 'slot')
    int mRankCount[29];             // 0x104 [U] func_0025A778 sets all -1; onRanklistRetrieved: "START" + "RANGE"
                                    //       per "SEQN" slot; func_0025E348 compares the slot's row count with it
    int mRankRequest[29];           // 0x178 [U] func_0025A778 sets all -1; func_0025E420 cancels the slot's old
                                    //       request and stores mRequestId ("snap" request)
    void* mRankSnapshot[29];        // 0x1EC [R] ctor zeroes 29 words; [U] func_0025E420 opens a snapshot per slot
                                    //       (func_003E85B0(api, 4, ..)); func_0025A850 frees them
    tLobbyAccount mNewAccount;      // 0x260 [R] ctor builds it, func_00260F10 resets it; [U] createaccount's form
    tLobbyAccount mAccount;         // 0x2B0 [R] ctor builds it; [U] onauthentication fills it
};

CHECK_OFFSET(LobbyMan, mState, 0x000);
CHECK_OFFSET(LobbyMan, mServerAddr, 0x004);
CHECK_OFFSET(LobbyMan, mAuthenticated, 0x008);
CHECK_OFFSET(LobbyMan, mLoggedIn, 0x00C);
CHECK_OFFSET(LobbyMan, mDisconnected, 0x010);
CHECK_OFFSET(LobbyMan, unk_014, 0x014);
CHECK_OFFSET(LobbyMan, mHaveCPAT, 0x018);
CHECK_OFFSET(LobbyMan, mCPAT, 0x01C);
CHECK_OFFSET(LobbyMan, mRequestId, 0x040);
CHECK_OFFSET(LobbyMan, mFindingPlayer, 0x044);
CHECK_OFFSET(LobbyMan, mUserName, 0x048);
CHECK_OFFSET(LobbyMan, mPassword, 0x04C);
CHECK_OFFSET(LobbyMan, mDeletePersonaIdx, 0x050);
CHECK_OFFSET(LobbyMan, mpLobbyApi, 0x054);
CHECK_OFFSET(LobbyMan, mpHostLookup, 0x058);
CHECK_OFFSET(LobbyMan, mpRoomList, 0x05C);
CHECK_OFFSET(LobbyMan, mpUserList, 0x060);
CHECK_OFFSET(LobbyMan, mpRankList, 0x064);
CHECK_OFFSET(LobbyMan, unk_068, 0x068);
CHECK_OFFSET(LobbyMan, mNumPersonas, 0x06C);
CHECK_OFFSET(LobbyMan, mPersonas, 0x070);
CHECK_OFFSET(LobbyMan, mPersona, 0x080);
CHECK_OFFSET(LobbyMan, mRoomName, 0x084);
CHECK_OFFSET(LobbyMan, mGameName, 0x088);
CHECK_OFFSET(LobbyMan, unk_08C, 0x08C);
CHECK_OFFSET(LobbyMan, mHostName, 0x090);
CHECK_OFFSET(LobbyMan, mOppoName, 0x094);
CHECK_OFFSET(LobbyMan, mOpponent, 0x098);
CHECK_OFFSET(LobbyMan, mAuth, 0x09C);
CHECK_OFFSET(LobbyMan, mWhen, 0x0A0);
CHECK_OFFSET(LobbyMan, mMatchType, 0x0A4);
CHECK_OFFSET(LobbyMan, mHostAddr, 0x0A8);
CHECK_OFFSET(LobbyMan, mOppoAddr, 0x0AC);
CHECK_OFFSET(LobbyMan, mSeed, 0x0B0);
CHECK_OFFSET(LobbyMan, mIsHost, 0x0B4);
CHECK_OFFSET(LobbyMan, mStats, 0x0B8);
CHECK_OFFSET(LobbyMan, mOpponentStats, 0x0CC);
CHECK_OFFSET(LobbyMan, mTimeout, 0x0E0);
CHECK_OFFSET(LobbyMan, unk_0E4, 0x0E4);
CHECK_OFFSET(LobbyMan, mServerName, 0x0E8);
CHECK_OFFSET(LobbyMan, mEventList, 0x0EC);
CHECK_OFFSET(LobbyMan, mpConfig, 0x0F4);
CHECK_OFFSET(LobbyMan, mRankSlot, 0x0F8);
CHECK_OFFSET(LobbyMan, unk_0FC, 0x0FC);
CHECK_OFFSET(LobbyMan, mpSlotTag, 0x100);
CHECK_OFFSET(LobbyMan, mRankCount, 0x104);
CHECK_OFFSET(LobbyMan, mRankRequest, 0x178);
CHECK_OFFSET(LobbyMan, mRankSnapshot, 0x1EC);
CHECK_OFFSET(LobbyMan, mNewAccount, 0x260);
CHECK_OFFSET(LobbyMan, mAccount, 0x2B0);
CHECK_SIZE(LobbyMan, 0x300);

// The lobby manager (LobbyMan::mLobbyMan; preview assert "!mLobbyMan"): set when func_0025B650 creates it.
extern LobbyMan* LobbyMan_mLobbyMan;

#endif /* ONLINE_LOBBYMAN_H */
