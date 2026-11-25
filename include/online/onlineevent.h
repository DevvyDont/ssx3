#ifndef ONLINE_ONLINEEVENT_H
#define ONLINE_ONLINEEVENT_H

// online::Event and its subclasses: the messages the online code (LobbyMan, BuddyMan's update, cConnMan, the setup
// manager cSetupMan) posts to the front end on LobbyMan::mEventList (a std::list<online::Event*>, online/lobbyman.h);
// the front-end states switch on mType and free them. Each class has a 2-slot vtable: 1 the deleting destructor, 2
// clone() (a heap copy). Class names are EA's, from the July 2003 preview's linkonce sections (_vt$Q26online5Event,
// _$_Q26online10ErrorEvent, clone__CQ26online15AcctLockedEvent, ...: the classes live in namespace online and their
// methods are inline, so they are declared in a header). EA's header path is not known; online/ is our choice (the
// online code's directory). There is no On Tour counterpart (On Tour has no online code).
// Struct names follow the fork's Outer_Inner convention for nested names (functions are already online_Event_*).
//
// Retail adds five event classes the preview does not have (vtables 0x00481230, 0x004811B0, 0x00481150, 0x00481130,
// 0x00481110); with only one name signal each they are not declared here (see the group I report), except ChatEvent
// (0x00481230), named after its "ChatEvent" allocation tag like its destructor and clone (online_ChatEvent_*).
//
// Layout: online::Event has one data member and declares the virtuals, so g++ 2.95 puts its vptr after it, at 0x4;
// the subclasses append from 0x8. Every event is allocated with cMemMan_alloc(size, "NetEvent" | "NetErrorEvent" |
// "ChatEvent", 0x20000000). cBXString members are one char* (bx/bxstring.h).
// A FLAT layout per class, like main/game.h: bases written out inline. Evidence tags:
//   [R] the retail destructors (slot 1) / clone (slot 2) and the allocations at the posting sites
//       (src/dirtysock/tagsunk.cpp: LobbyMan_on*, func_0025AC50, func_002636F0);
//   [U] how retail code uses the field.
// The destructors store online_Event_vtbl (g++ drops the dead stores of the derived vtables), so they do not show
// the class tree; AcctTooYoungEvent / AcctLockedEvent carry ErrorEvent's type (258) and code word (0x8), so they
// probably derive from ErrorEvent (laid out inline either way).

#include "layout.h"
#include "online/lobbyman.h"            // tLobbyStats

// online::Event::mType values whose poster names the event (names ours, after EA's LobbyMan / BuddyMan callbacks that
// post them; [R] src/dirtysock/tagsunk.cpp). The other types (200, 202, 205, 208, 209, 211, 213, 215, 217, 230, 231,
// 236-238, 242-247, 251-253, 256, 261, 263-265, 268-276) come from unnamed request functions or several posters and
// stay numbers.
enum tNetEventType {
    kNetEvent_LobbyConnect = 201,             // LobbyMan_onlobbyconnect
    kNetEvent_AccountCreated = 203,           // LobbyMan_onaccountcreated
    kNetEvent_AccountEdited = 206,            // LobbyMan_onaccountedited
    kNetEvent_LostAccountRequestSent = 207,   // LobbyMan_onlostaccountrequestsent
    kNetEvent_LostPasswordRequestSent = 210,  // LobbyMan_onlostpasswordrequestsent
    kNetEvent_Authentication = 212,           // LobbyMan_onauthentication
    kNetEvent_PersonaCreated = 214,           // LobbyMan_onpersonacreated
    kNetEvent_PersonaDeleted = 216,           // LobbyMan_onpersonadeleted
    kNetEvent_PersonaSelected = 218,          // LobbyMan_onpersonaselected
    kNetEvent_RoomSelected = 220,             // LobbyMan_onroomselected
    kNetEvent_Chat = 222,                     // func_0025AB40 (a ChatEvent)
    kNetEvent_RoomListUpdate = 223,           // LobbyMan_onroomlistupdate
    kNetEvent_RankListUpdate = 225,           // LobbyMan_onranklistupdate
    kNetEvent_News = 226,                     // LobbyMan_onNewsRetrieved (a NewsEvent)
    kNetEvent_Ping = 229,                     // LobbyMan_onPingReceived (a PingEvent)
    kNetEvent_Error = 258,                    // online::ErrorEvent (the lobby callbacks' failures)
    kNetEvent_BuddyConnect = 260,             // BuddyMan_onconnect
    kNetEvent_BuddyRosterUpdate = 262,        // BuddyMan_onrosterupdate
    kNetEvent_BuddyAdd = 266,                 // BuddyMan_onBuddyAdd
    kNetEvent_BuddyDelete = 267,              // BuddyMan_onBuddyDelete
    kNetEvent_ConnectionMsg = 277,            // LobbyMan_onconnectionmsg (and func_0025AC50)
    kNetEvent_NetError = 278                  // online::NetErrorEvent
};

// online::Event: size 8 ([R] online_Event_clone allocates 8; func_0025F628 / func_0025FC70 / func_002642B8 post
// plain 8-byte events of types 218, 238, 246, 256, 272 ...). Vtable online_Event_vtbl (0x004812B0).
struct online_Event {
    int mType;                      // 0x0 [R] every poster stores it first (222 chat, 226 news, 227 find player,
                                    //     229 ping, 251/252 setup updates, 258 error, 259 buddy error, 269 user record,
                                    //     278 net error, ...); [U] the front end switches on it
    void** mVtbl;                   // 0x4 [R] online_Event_vtbl (clone, destructors)
};

CHECK_OFFSET(online_Event, mType, 0x0);
CHECK_OFFSET(online_Event, mVtbl, 0x4);
CHECK_SIZE(online_Event, 0x8);

// The four-character error codes of online::ErrorEvent::mError (the lobby's replies) and online::NetErrorEvent::mError
// (the disconnect reasons, '-' first), written as EA's multi-character constants: the comment gives the ASCII of the
// value's bytes from the most significant down. Names ours, after the ASCII. [U] the front end's error handlers
// (func_001A9CC0, func_001AAF90, func_001A4E28, func_001ADC30, func_001BAC90, func_001BD8C8, func_001BDA88, ...) pick
// a popup text per code.
enum tOnlineErrorCode {
    kLobbyErr_ajoi = 0x616A6F69,        // 'ajoi'
    kLobbyErr_aunr = 0x61756E72,        // 'aunr'
    kLobbyErr_auth = 0x61757468,        // 'auth'
    kLobbyErr_badc = 0x62616463,        // 'badc'
    kLobbyErr_blak = 0x626C616B,        // 'blak'
    kLobbyErr_bogu = 0x626F6775,        // 'bogu'
    kLobbyErr_born = 0x626F726E,        // 'born'
    kLobbyErr_cdev = 0x63646576,        // 'cdev'
    kLobbyErr_chal = 0x6368616C,        // 'chal'
    kLobbyErr_chet = 0x63686574,        // 'chet'
    kLobbyErr_dber = 0x64626572,        // 'dber'
    kLobbyErr_defp = 0x64656670,        // 'defp'
    kLobbyErr_dete = 0x64657465,        // 'dete'
    kLobbyErr_disa = 0x64697361,        // 'disa'
    kLobbyErr_dnsf = 0x646E7366,        // 'dnsf'
    kLobbyErr_down = 0x646F776E,        // 'down'
    kLobbyErr_dupl = 0x6475706C,        // 'dupl'
    kLobbyErr_econ = 0x65636F6E,        // 'econ'
    kLobbyErr_elen = 0x656C656E,        // 'elen'
    kLobbyErr_filt = 0x66696C74,        // 'filt'
    kLobbyErr_full = 0x66756C6C,        // 'full'
    kLobbyErr_gend = 0x67656E64,        // 'gend'
    kLobbyErr_gpar = 0x67706172,        // 'gpar'
    kLobbyErr_host = 0x686F7374,        // 'host'
    kLobbyErr_iadr = 0x69616472,        // 'iadr'
    kLobbyErr_igam = 0x6967616D,        // 'igam'
    kLobbyErr_igno = 0x69676E6F,        // 'igno'
    kLobbyErr_imst = 0x696D7374,        // 'imst'
    kLobbyErr_inam = 0x696E616D,        // 'inam'
    kLobbyErr_ingm = 0x696E676D,        // 'ingm'
    kLobbyErr_inps = 0x696E7073,        // 'inps'
    kLobbyErr_invp = 0x696E7670,        // 'invp'
    kLobbyErr_ipas = 0x69706173,        // 'ipas'
    kLobbyErr_iper = 0x69706572,        // 'iper'
    kLobbyErr_itrn = 0x6974726E,        // 'itrn'
    kLobbyErr_join = 0x6A6F696E,        // 'join'
    kLobbyErr_kick = 0x6B69636B,        // 'kick'
    kLobbyErr_kikh = 0x6B696B68,        // 'kikh'
    kLobbyErr_lock = 0x6C6F636B,        // 'lock'
    kLobbyErr_logn = 0x6C6F676E,        // 'logn'
    kLobbyErr_mail = 0x6D61696C,        // 'mail'
    kLobbyErr_many = 0x6D616E79,        // 'many'
    kLobbyErr_maut = 0x6D617574,        // 'maut'
    kLobbyErr_maxp = 0x6D617870,        // 'maxp'
    kLobbyErr_maxt = 0x6D617874,        // 'maxt'
    kLobbyErr_miss = 0x6D697373,        // 'miss'
    kLobbyErr_neml = 0x6E656D6C,        // 'neml'
    kLobbyErr_new0 = 0x6E657730,        // 'new0'
    kLobbyErr_nfnd = 0x6E666E64,        // 'nfnd'
    kLobbyErr_ngam = 0x6E67616D,        // 'ngam'
    kLobbyErr_nkik = 0x6E6B696B,        // 'nkik'
    kLobbyErr_nown = 0x6E6F776E,        // 'nown'
    kLobbyErr_npla = 0x6E706C61,        // 'npla'
    kLobbyErr_npri = 0x6E707269,        // 'npri'
    kLobbyErr_nrom = 0x6E726F6D,        // 'nrom'
    kLobbyErr_nspc = 0x6E737063,        // 'nspc'
    kLobbyErr_nsrv = 0x6E737276,        // 'nsrv'
    kLobbyErr_onln = 0x6F6E6C6E,        // 'onln'
    kLobbyErr_over = 0x6F766572,        // 'over'
    kLobbyErr_pass = 0x70617373,        // 'pass'
    kLobbyErr_paut = 0x70617574,        // 'paut'
    kLobbyErr_pmal = 0x706D616C,        // 'pmal'
    kLobbyErr_pset = 0x70736574,        // 'pset'
    kLobbyErr_quit = 0x71756974,        // 'quit'
    kLobbyErr_rang = 0x72616E67,        // 'rang'
    kLobbyErr_rank = 0x72616E6B,        // 'rank'
    kLobbyErr_raut = 0x72617574,        // 'raut'
    kLobbyErr_room = 0x726F6F6D,        // 'room'
    kLobbyErr_rsrv = 0x72737276,        // 'rsrv'
    kLobbyErr_same = 0x73616D65,        // 'same'
    kLobbyErr_self = 0x73656C66,        // 'self'
    kLobbyErr_soon = 0x736F6F6E,        // 'soon'
    kLobbyErr_spam = 0x7370616D,        // 'spam'
    kLobbyErr_strt = 0x73747274,        // 'strt'
    kLobbyErr_team = 0x7465616D,        // 'team'
    kLobbyErr_time = 0x74696D65,        // 'time'
    kLobbyErr_tooy = 0x746F6F79,        // 'tooy'
    kLobbyErr_tosa = 0x746F7361,        // 'tosa'
    kLobbyErr_tsta = 0x74737461,        // 'tsta'
    kLobbyErr_ugam = 0x7567616D,        // 'ugam'
    kLobbyErr_ulst = 0x756C7374,        // 'ulst'
    kLobbyErr_umac = 0x756D6163,        // 'umac'
    kLobbyErr_urom = 0x75726F6D,        // 'urom'
    kLobbyErr_uusr = 0x75757372,        // 'uusr'
    kLobbyErr_wolf = 0x776F6C66,        // 'wolf'
    kLobbyErr_xist = 0x78697374,        // 'xist'
    kNetErr_unknown = 0x2D3F3F3F,       // '-???'
    kNetErr_ath = 0x2D617468,           // '-ath'
    kNetErr_bsy = 0x2D627379,           // '-bsy'
    kNetErr_cfg = 0x2D636667,           // '-cfg'
    kNetErr_con = 0x2D636F6E,           // '-con'
    kNetErr_dev = 0x2D646576,           // '-dev'
    kNetErr_dif = 0x2D646966,           // '-dif'
    kNetErr_dns = 0x2D646E73,           // '-dns'
    kNetErr_ini = 0x2D696E69,           // '-ini'
    kNetErr_ton = 0x2D746F6E            // '-ton'
};

// online::ErrorEvent: type 258, size 0xC ([R] online_ErrorEvent_clone allocates 0xC). Vtable online_ErrorEvent_vtbl
// (0x00481290).
struct online_ErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 258
    void** mVtbl;                   // 0x4 [R] online_ErrorEvent_vtbl
    // --- online::ErrorEvent
    int mError;                     // 0x8 [R] clone copies it; [U] a four-character code ('time', 'dnsf' from
                                    //     func_0025AC50) or the LobbyApi reply's error (LobbyMan_onPlayerFound ...)

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_ErrorEvent_clone");
};

CHECK_OFFSET(online_ErrorEvent, mType, 0x0);
CHECK_OFFSET(online_ErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_ErrorEvent, mError, 0x8);
CHECK_SIZE(online_ErrorEvent, 0xC);

// online::BuddyErrorEvent: type 259, size 0xC ([R] func_0025AC50 allocates 0xC). Vtable online_BuddyErrorEvent_vtbl
// (0x00481270). Its clone builds an ErrorEvent (type 258, online_ErrorEvent_vtbl), as in the preview.
struct online_BuddyErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 259
    void** mVtbl;                   // 0x4 [R] online_BuddyErrorEvent_vtbl
    // --- online::BuddyErrorEvent
    int mError;                     // 0x8 [R] func_0025AC50 stores BuddyMan's miError; clone copies it

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_BuddyErrorEvent_clone");
};

CHECK_OFFSET(online_BuddyErrorEvent, mType, 0x0);
CHECK_OFFSET(online_BuddyErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_BuddyErrorEvent, mError, 0x8);
CHECK_SIZE(online_BuddyErrorEvent, 0xC);

// online::NetErrorEvent: type 278, size 0xC ([R] func_002636F0 allocates 0xC with the tag "NetErrorEvent"). Vtable
// online_NetErrorEvent_vtbl (0x00481250).
struct online_NetErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 278
    void** mVtbl;                   // 0x4 [R] online_NetErrorEvent_vtbl
    // --- online::NetErrorEvent
    int mError;                     // 0x8 [R] func_002636F0 stores its argument (the disconnect reason); clone copies it

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_NetErrorEvent_clone");
};

CHECK_OFFSET(online_NetErrorEvent, mType, 0x0);
CHECK_OFFSET(online_NetErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_NetErrorEvent, mError, 0x8);
CHECK_SIZE(online_NetErrorEvent, 0xC);

// online::NewsEvent: type 226, size 0xC ([R] online_NewsEvent_clone allocates 0xC). Vtable online_NewsEvent_vtbl
// (0x00481210).
struct online_NewsEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 226
    void** mVtbl;                   // 0x4 [R] online_NewsEvent_vtbl
    // --- online::NewsEvent
    char* mText;                    // 0x8 [R] cBXString: clone builds it from the source's text, the destructor
                                    //     destroys it; [U] LobbyMan_onNewsRetrieved posts the news text

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_NewsEvent_clone");
};

CHECK_OFFSET(online_NewsEvent, mType, 0x0);
CHECK_OFFSET(online_NewsEvent, mVtbl, 0x4);
CHECK_OFFSET(online_NewsEvent, mText, 0x8);
CHECK_SIZE(online_NewsEvent, 0xC);

// online::AcctTooYoungEvent: type 258 with code 'tooy', size 0x10 ([R] clone allocates 0x10). Vtable
// online_AcctTooYoungEvent_vtbl (0x004811F0).
struct online_AcctTooYoungEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 258
    void** mVtbl;                   // 0x4 [R] online_AcctTooYoungEvent_vtbl
    // --- online::ErrorEvent (probable base, see above)
    int mError;                     // 0x8 [R] clone stores the constant 'tooy' (0x746F6F79)
    // --- online::AcctTooYoungEvent
    int mAge;                       // 0xC [R] clone copies it; [U] LobbyMan_onaccountcreated stores the reply's "AGE"

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_AcctTooYoungEvent_clone");
};

CHECK_OFFSET(online_AcctTooYoungEvent, mType, 0x0);
CHECK_OFFSET(online_AcctTooYoungEvent, mVtbl, 0x4);
CHECK_OFFSET(online_AcctTooYoungEvent, mError, 0x8);
CHECK_OFFSET(online_AcctTooYoungEvent, mAge, 0xC);
CHECK_SIZE(online_AcctTooYoungEvent, 0x10);

// online::AcctLockedEvent: type 258 with code 'lock', size 0x14 ([R] clone allocates 20). Vtable
// online_AcctLockedEvent_vtbl (0x004811D0).
struct online_AcctLockedEvent {
    // --- online::Event
    int mType;                      // 0x00 [R] 258
    void** mVtbl;                   // 0x04 [R] online_AcctLockedEvent_vtbl
    // --- online::ErrorEvent (probable base, see above)
    int mError;                     // 0x08 [R] clone stores the constant 'lock' (0x6C6F636B)
    // --- online::AcctLockedEvent
    char* mDate;                    // 0x0C [R] cBXString (clone builds it, the destructor destroys it second);
                                    //      [U] LobbyMan_onauthentication: the reply's "DATE"
    char* mIntro;                   // 0x10 [R] cBXString (destroyed first); [U] the reply's "INTRO" text

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_AcctLockedEvent_clone");
};

CHECK_OFFSET(online_AcctLockedEvent, mType, 0x00);
CHECK_OFFSET(online_AcctLockedEvent, mVtbl, 0x04);
CHECK_OFFSET(online_AcctLockedEvent, mError, 0x08);
CHECK_OFFSET(online_AcctLockedEvent, mDate, 0x0C);
CHECK_OFFSET(online_AcctLockedEvent, mIntro, 0x10);
CHECK_SIZE(online_AcctLockedEvent, 0x14);

// online::FindPlayerEvent: type 227, size 0x2C ([R] LobbyMan_onPlayerFound and clone allocate 44). Vtable
// online_FindPlayerEvent_vtbl (0x00481190).
struct online_FindPlayerEvent {
    // --- online::Event
    int mType;                      // 0x00 [R] 227
    void** mVtbl;                   // 0x04 [R] online_FindPlayerEvent_vtbl
    // --- online::FindPlayerEvent
    char* mName;                    // 0x08 [R] cBXString (destroyed second); [U] the reply's "PERS" (persona name)
    char* mRoom;                    // 0x0C [R] cBXString (destroyed first); [U] the reply's "ROOM"
    int mOnline;                    // 0x10 [U] onPlayerFound: "ADDR" > 0
    int mAddr;                      // 0x14 [U] onPlayerFound: the "ADDR" number
    tLobbyStats mStats;             // 0x18 [R] clone block-copies 20 bytes; [U] "RANK" and "STAT" fields, -1 last

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_FindPlayerEvent_clone");
};

CHECK_OFFSET(online_FindPlayerEvent, mType, 0x00);
CHECK_OFFSET(online_FindPlayerEvent, mVtbl, 0x04);
CHECK_OFFSET(online_FindPlayerEvent, mName, 0x08);
CHECK_OFFSET(online_FindPlayerEvent, mRoom, 0x0C);
CHECK_OFFSET(online_FindPlayerEvent, mOnline, 0x10);
CHECK_OFFSET(online_FindPlayerEvent, mAddr, 0x14);
CHECK_OFFSET(online_FindPlayerEvent, mStats, 0x18);
CHECK_SIZE(online_FindPlayerEvent, 0x2C);

// online::PingEvent: type 229, size 0x10 ([R] clone allocates 0x10). Vtable online_PingEvent_vtbl (0x00481170).
struct online_PingEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 229
    void** mVtbl;                   // 0x4 [R] online_PingEvent_vtbl
    // --- online::PingEvent
    char* mName;                    // 0x8 [R] cBXString (clone, destructor); [U] LobbyMan_onPingReceived: "NAME"
    int mPing;                      // 0xC [R] clone copies it; [U] "PING", 32656 when the reply carried an error

    // Methods bound to their retail symbols by asm labels.
    void* clone() __asm__("online_PingEvent_clone");
};

CHECK_OFFSET(online_PingEvent, mType, 0x0);
CHECK_OFFSET(online_PingEvent, mVtbl, 0x4);
CHECK_OFFSET(online_PingEvent, mName, 0x8);
CHECK_OFFSET(online_PingEvent, mPing, 0xC);
CHECK_SIZE(online_PingEvent, 0x10);

// online::ChatEvent: type 222, size 0x10 ([R] func_0025AB40 and online_ChatEvent_clone allocate 0x10 bytes tagged
// "ChatEvent"). Vtable 0x00481230 (D_00481230: 1 online_ChatEvent__ChatEvent, 2 online_ChatEvent_clone). Retail only
// (not in the preview), so the class and member names are ours.
struct online_ChatEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 222 (func_0025AB40, clone)
    void** mVtbl;                   // 0x4 [R] D_00481230; the destructor stores online_Event_vtbl back
    // --- online::ChatEvent
    char* mSender;                  // 0x8 [R] cBXString: func_0025AB40 copies its sender argument (the chat line's
                                    //     sender, from LobbyMan_onchatmsg); clone copies it; destroyed second
    char* mText;                    // 0xC [R] cBXString: the message text (func_0025AB40's text argument); destroyed
                                    //     first
};

CHECK_OFFSET(online_ChatEvent, mType, 0x0);
CHECK_OFFSET(online_ChatEvent, mVtbl, 0x4);
CHECK_OFFSET(online_ChatEvent, mSender, 0x8);
CHECK_OFFSET(online_ChatEvent, mText, 0xC);
CHECK_SIZE(online_ChatEvent, 0x10);

#endif /* ONLINE_ONLINEEVENT_H */
