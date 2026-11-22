#ifndef FE_DEBUGFE_H
#define FE_DEBUGFE_H

// cDebugFE: the debug front end, the app module SSXApp runs instead of the real front end on debug boots: a debug menu
// screen and a debug lobby screen (one active), the debug font, two textures and a heartbeat doodle. SSXApp keeps it
// as cSSXApp::DebugFE (main/ssxapp.h, +0x80). EA's source: d:/ssx/code/source/fe/debugfe.cpp (preview assert
// strings); retail code: cDebugFE_cDebugFE ... cDebugFE_render in fe/debugfe (0x0017F7B8 ... 0x0017FBF0) and the
// out-of-line inlines getFont / getBoxTID / startlobby in main/all_in_one_main (0x001DC970 ... 0x001DC980).
//
// Size 0x238: [R] cSSXApp_startFELoad: cMemMan_alloc(0x238, "FEModule") + cDebugFE_cDebugFE; the ctor's last store is
// the heartbeat's HistoryIdx at 0x234. [O] On Tour's class is 0x234: one Screens slot (retail two), so every member
// after Screens sits 4 bytes later in retail.
// Base: cFadeScreen (main/fadescreen.h, 0xC: vptr, FadeTimer, IsEnding) : cAppModule (vptr only). The ctor runs
// cFadeScreen_cFadeScreen and stores cDebugFE_vtbl (0x0046D900, 7 slots: 1 dtor, 2 load, 3 purge, 4 loseFocus,
// 5 gainFocus, 6 update, 7 render); the dtor stores cAppModule's vtable cAppModule_vtbl.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's constructor / destructor and the allocation size;
//   [P] the July 2003 preview's inline accessors getFont / getBoxTID (lw 0x24 / 0x2C there; tools/layout_xlate.py
//       measures preview -> retail -0xC for 0x18 ... 0x2C from the paired load/update/render/startlobby);
//   [U] how retail code uses the field (the C bodies in src/fe/debugfe.cpp and src/main/all_in_one_main.cpp, the asm);
//   [O] SSX On Tour's DWARF (class cDebugFE, cFadeScreen, cHeartBeat): names, types and order.

#include "layout.h"
#include "fe/heartbeat.h"
#include "util/menusys.h" // cMenuSys (cDebugLobbyScreen::mMenuSys)
#include "fe/backgroundman.h" // cBackgroundMan (cDebugMenuScreen)
#include "fe/debuglobbymenus.h" // the debug lobby menus (cDebugLobbyScreen)

struct cFont;

// cDebugScreen: the debug front end's screen base (cDebugFE::Screens / ActiveScreen); cDebugMenuScreen and
// cDebugLobbyScreen below derive from it. Size 0x4: [O] On Tour class cDebugScreen (0x4: the vptr only; virtual ~,
// update() = 0, draw() = 0); [R] cDebugScreen_vtbl (D_0046D948, 3 slots: 1 dtor, 2 / 3 the pure-virtual stub
// func_00411530), stored back by the derived destructors; cDebugFE_update calls slot 2 (update) on ActiveScreen.
// A FLAT layout (the vptr only).
struct cDebugScreen {
    void** mVtbl;                   // 0x000 [R] cDebugScreen_vtbl (the derived dtors), cDebugMenuScreen_vtbl /
                                    //       cDebugLobbyScreen_vtbl (the derived ctors)
};

CHECK_OFFSET(cDebugScreen, mVtbl, 0x000);
CHECK_SIZE(cDebugScreen, 0x4);

struct cDebugFE {
    // --- cAppModule / cFadeScreen (0xC; main/fadescreen.h)
    void** mVtbl;                   // 0x000 [R] cFadeScreen_vtbl, then cDebugFE_vtbl (ctor); cAppModule_vtbl (dtor);
                                    //       [P] same offset (slot 1)
    float FadeTimer;                // 0x004 [O] cFadeScreen::FadeTimer; [R] cFadeScreen_cFadeScreen 0.2
    int IsEnding;                   // 0x008 [O] cFadeScreen::IsEnding (bool, a word in retail)
    // --- cDebugFE
    cDebugScreen* Screens[2];       // 0x00C [O] (On Tour Screens[1]); [P] (preview 0x18, 0x1C); [U] load zeroes both,
                                    //       then stores the debug menu screen (cDebugMenuScreen, "DebugMenu") and the
                                    //       debug lobby screen (cDebugLobbyScreen, "LobbyMenu"); purge deletes both
                                    //       (slot 1, flags 3); startlobby reads Screens[1]
    cDebugScreen* ActiveScreen;     // 0x014 [O]; [P] (preview 0x20); [U] load picks Screens[1] when SSXApp's unk_74 is
                                    //       set, else Screens[0]; startlobby sets Screens[1]; update / render call its
                                    //       slots 2 / 3
    cFont* Font;                    // 0x018 [O]; [P] getFont (lw); [U] load builds it (cFont_cFont(cMemMan_alloc(0x84,
                                    //       "DebugFont"), "data/fonts/debugfnt.sfn") and sets its scale; purge deletes it
    int ParticleTID;                // 0x01C [O]; [P] (preview 0x28); [U] load: the "part" texture of
                                    //       data/textures/dot.ssh (GrMan slot 46), else -1; render draws the heartbeat
                                    //       with it; purge frees it (slot 50) when >= 0
    int BoxTID;                     // 0x020 [O]; [P] getBoxTID (lw); [U] load: the "menu" texture of
                                    //       data/fonts/menu.ssh; purge frees it when >= 0
    cHeartBeat HeartBeat;           // 0x024 [O] (fe/heartbeat.h, 0x214); [R] ctor zeroes Angle / Phase (+0x24..+0x30),
                                    //       History (MEM_fill 0x200 at +0x34) and HistoryIdx (+0x234); [U] update
                                    //       (cHeartBeat_update) and render (cHeartBeat_draw) pass &HeartBeat
};

CHECK_OFFSET(cDebugFE, mVtbl, 0x000);
CHECK_OFFSET(cDebugFE, FadeTimer, 0x004);
CHECK_OFFSET(cDebugFE, IsEnding, 0x008);
CHECK_OFFSET(cDebugFE, Screens, 0x00C);
CHECK_OFFSET(cDebugFE, ActiveScreen, 0x014);
CHECK_OFFSET(cDebugFE, Font, 0x018);
CHECK_OFFSET(cDebugFE, ParticleTID, 0x01C);
CHECK_OFFSET(cDebugFE, BoxTID, 0x020);
CHECK_OFFSET(cDebugFE, HeartBeat, 0x024);
CHECK_SIZE(cDebugFE, 0x238);

// cTrackFileMan: the debug front end's track list - up to 256 track locations ("data/worlds/<tag>.big" entries) with
// their location id and whether each is a BAM location and a world-circuit start. scanLocations fills it from the
// back end's 22 location files (cBENewRaceInterface_getLocationFile, tag "BAM"; checkLocation1 checks the bigfile),
// scanTrackIndex adds the names listed in an index file (location id 22); getNext/PrevLocation and
// getNext/PrevWCBAMStartLocation step through it for the debug menu's track selector. Embedded by value in the debug
// menu screen: cDebugMenuScreen_cDebugMenuScreen (main/loadscreens.cpp) calls cTrackFileMan_scanLocations(self + 0x4)
// and its next member (a cBackgroundMan) is at 0x8C10. EA's source: d:/ssx/code/source/fe/debugfe.cpp (the preview's
// cTrackFileMan methods, 0x00186EC8 .. 0x00187B40, assert "i >= 0 && i < NumLocations" and "NumLocations >= 0" with
// that path; the preview also has the assert "TrackFileMan.getNumLocations() > 0"). Retail code is in the decomp's unit
// camera/trigger/cameratriggerfactory.cpp (0x00179FC8 .. 0x0017ABC4).
// Size 0x8C0C: [R] scanLocations clears 256 * 0x8C = 0x8C00 bytes from +0x4 (MEM_fill 35840) and the two counts at
// 0x8C04 / 0x8C08; [U] the containing cDebugMenuScreen places it at 0x4 with its next member at 0x8C10 (4 + 0x8C0C).
// No base classes, no vtable.
//
// A FLAT layout, like main/game.h. Tags: [R] scanLocations (the only initialiser; there is no constructor); [U] how
// retail code uses the field (cameratriggerfactory.cpp, loadscreens.cpp); [O] SSX On Tour's DWARF (class cTrackFileMan,
// size 0x880C; struct tLocationInfo, size 0x88): the same members; On Tour's two bools are 1 byte each, retail's are
// words (scanLocations sw 1 into both), so retail's tLocationInfo is 0x8C and every later offset moves.
// No preview evidence (no accessors survive; layout_xlate has no observations).

struct tLocationInfo {
    char File[64];                  // 0x000 [O]; [U] scanLocations strcpy's the location file name; checkLocation /
                                    //       checkLocation1 strcasecmp against it; scanTrackIndex copies the index entry
    char BigFile[64];               // 0x040 [O]; [U] scanLocations stores "BAM" (one word), scanTrackIndex the entry name;
                                    //       checkLocation / checkLocation1 compare the bigfile tag with it
    int locationID;                 // 0x080 [O] eBELocationID; [U] scanLocations: the back-end location index j;
                                    //       scanTrackIndex: 22; legalTrackMode classifies it by game mode;
                                    //       testWCBAMStartLocation gets it
    int bIsBAM;                     // 0x084 [O] (bool; a word: sw); [U] scanLocations: 1, scanTrackIndex: 0
    int bIsWCBAMStart;              // 0x088 [O] (bool; a word); [U] scanLocations: 1 when testWCBAMStartLocation says so;
                                    //       getNext/PrevWCBAMStartLocation skip entries where it is 0
};
CHECK_OFFSET(tLocationInfo, File, 0x0);
CHECK_OFFSET(tLocationInfo, BigFile, 0x40);
CHECK_OFFSET(tLocationInfo, locationID, 0x80);
CHECK_OFFSET(tLocationInfo, bIsBAM, 0x84);
CHECK_OFFSET(tLocationInfo, bIsWCBAMStart, 0x88);
CHECK_SIZE(tLocationInfo, 0x8C);

// One section name to look for in a big file, with the answer (On Tour struct tWorldSectionScan, 0x8;
// [O] cTrackFileMan::checkLocation1(int iNumSectionScan, tWorldSectionScan*, const char* sBigFile)). [Staged by views
// group A.] [U] cTrackFileMan_checkLocation1 compares each sectionName with the location table (strcasecmp) and lets
// isValidMap set bFound from the big file's section table (view sCamName_17A978); cTrackFileMan_scanLocations passes
// the 22 built-in location files.
struct tWorldSectionScan {
    const char* sectionName;        // 0x0 [O]; [U] checkLocation1 strcasecmp's it against tLocationInfo::File
    int bFound;                     // 0x4 [O] bool (a word in retail); [U] set by the section scan, cleared again for
                                    //      names already listed
};

CHECK_OFFSET(tWorldSectionScan, sectionName, 0x0);
CHECK_OFFSET(tWorldSectionScan, bFound, 0x4);
CHECK_SIZE(tWorldSectionScan, 0x8);

struct cTrackFileMan {
    int NumLocations;               // 0x0000 [O]; [R] scanLocations: 0, then counts the entries it adds; [U] scanTrackIndex
                                    //        appends at it; legalTrackMode / getNext/PrevLocation / the WCBAM steppers
                                    //        wrap at it
    tLocationInfo LocationInfo[256]; // 0x0004 [O]; [R] scanLocations: MEM_fill(LocationInfo, 0, 0x8C00)
    int NumBAMLocations;            // 0x8C04 [O]; [R] scanLocations: 0, then +1 per BAM location added
    int NumWCBAMStartLocations;     // 0x8C08 [O]; [R] scanLocations: 0, then +1 per world-circuit start; [U]
                                    //        getNext/PrevWCBAMStartLocation return -1 while it is 0; the debug menu reads it
};
CHECK_OFFSET(cTrackFileMan, NumLocations, 0x0);
CHECK_OFFSET(cTrackFileMan, LocationInfo, 0x4);
CHECK_OFFSET(cTrackFileMan, NumBAMLocations, 0x8C04);
CHECK_OFFSET(cTrackFileMan, NumWCBAMStartLocations, 0x8C08);
CHECK_SIZE(cTrackFileMan, 0x8C0C);


// ---------------------------------------------------------------------------------------------------------------------
// cDebugMenuScreen: the debug front end's race-setup menu (cDebugFE::Screens[0], "DebugMenu"): game path / mode /
// event, first-time flag, track, character, control map, board, AI riders, screen calibration, HDTV, playlist and
// network rows, then "Lobby" and "Real Front End". EA's source file is not established (the preview's asserts name
// fe/debugfe.cpp and main/debugmenu.cpp; On Tour: fe/all_in_one_fe). Retail code: the ctor and update in
// main/loadscreens (0x0017B1B0, 0x0017B4E0), draw / setGamePath / getMaxAIRiders in fe/debugfe (0x0017C5F8, 0x0017D008,
// 0x0017D1B8) and the dtor in main/all_in_one_main (0x001DCD00).
//
// Size 0x8C48:
//   [R] cDebugFE_load: cMemMan_alloc(0x8C48, "DebugMenu") + cDebugMenuScreen_cDebugMenuScreen; the last word written is
//       0x8C44;
//   [O] On Tour's class is 0x8850: the same order, but its cTrackFileMan is 0x400 smaller (1-byte bools) and it has
//       CurrentHomo / CurrentBoardTexture (after CurrentBoard), CurrentMissionIdx (after CurrentMap) and CurrentMovie
//       (after CurrentBack), which retail does not; retail has a playlist word where CurrentMovie is and a network word
//       after LastGamePathGameMode. Retail fields are placed by their use, not by On Tour's offsets.
// Base: cDebugScreen (On Tour 0x4: the vptr only; vtable cDebugScreen_vtbl D_0046D948, slots 2 / 3 the pure-virtual
// stub). Vtable cDebugMenuScreen_vtbl (0x0046D798, 3 slots: 1 dtor, 2 update, 3 draw); the dtor destroys BackgroundMan
// and stores cDebugScreen_vtbl.
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's ctor / dtor / allocation; [U] how retail code uses the
// field (src/main/loadscreens.cpp views sLS_B1B0 / sLSSub_B1B0, src/fe/debugfe.cpp views sDebugMenuScreen /
// sDbgFE17D008, and the asm); [O] SSX On Tour's DWARF (class cDebugMenuScreen, cDebugScreen): names, types and order.

struct cDebugMenuScreen {
    // --- cDebugScreen (On Tour 0x4: the vptr)
    void** mVtbl;                   // 0x0000 [R] cDebugMenuScreen_vtbl (ctor); cDebugScreen_vtbl (dtor); [U]
                                    //       cDebugFE_update / render call slots 2 / 3
    // --- cDebugMenuScreen
    cTrackFileMan TrackFileMan;     // 0x0004 [O] (0x8C0C, above); [R] the ctor runs cTrackFileMan_scanLocations(this +
                                    //       0x4); [U] the ctor / update step the track row through it
                                    //       (getNext/PrevLocation, the WCBAM steppers, legalTrackMode, the entries'
                                    //       locationID / bIsBAM)
    cBackgroundMan BackgroundMan;   // 0x8C10 [O] (fe/backgroundman.h); [R] cBackgroundMan_cBackgroundMan + loadImages
                                    //       (ctor), cBackgroundMan__cBackgroundMan(.., 2) (dtor); [U] update sets the
                                    //       active image; draw renders it instead of the menu while miActiveImage
                                    //       (0x8C18) >= 0
    int SelectedItem;               // 0x8C1C [O] tItems (an enum, a word); [R] ctor 4; [U] the cursor row: update moves
                                    //       it over the 15 rows (12 network is skipped without a comm system, 13 Lobby,
                                    //       14 Real Front End), draw highlights it (lw 0x8C1C through this + 0x8000)
    int currentLocation;            // 0x8C20 [O]; [R] ctor 0, then the first WCBAM start location; [U] the track row:
                                    //       the TrackFileMan.LocationInfo index the race uses
    int CurrentBoard;               // 0x8C24 [O] tBoardTypes; [R] ctor 0, also stored into the back end's first word;
                                    //       [U] the board row: update cycles it (% 1) and stores it there; draw prints
                                    //       D_004A12E0[CurrentBoard]
    int CurrentChar;                // 0x8C28 [O]; [R] ctor 2 (player 0's character); [U] the character row: update
                                    //       cycles it over 10 and sets player 0's character; reloaded from the back end
                                    //       after a game
    int CurrentMap;                 // 0x8C2C [O]; [R] ctor: player 0's controller mapping; [U] the control-map row:
                                    //       update toggles it and sets the mapping
    int CurrentBack;                // 0x8C30 [O]; [R] ctor -1; [U] the screen-calibration row: update cycles it over
                                    //       BackgroundMan's images (-1 = none) and makes it the active image; draw
                                    //       prints it
    int CurrentPlaylist;            // 0x8C34 [U] the playlist row (name from its use; On Tour's CurrentMovie sits in
                                    //       this place): ctor 0 and starts that playlist entry (bxAudio + 0x60A4 + 0x14
                                    //       * it); update cycles it and plays it; draw prints the entry's name
    int LastGamePathGameMode[3];    // 0x8C38 [O]; [R] ctor 4, 4, 4; [U] setGamePath saves the current game mode under
                                    //       the current path and restores the new path's
    int NetworkMode;                // 0x8C44 [U] the network row (name from its use; no On Tour member): ctor 0; update
                                    //       cycles 0 ... 4 (1 / 2 only with a net connection) and sets
                                    //       networkOpt.bIsNetworkGame / bIsHost (1 = host) from it; draw prints
                                    //       D_0045CFB8's name for it
};

CHECK_OFFSET(cDebugMenuScreen, mVtbl, 0x0000);
CHECK_OFFSET(cDebugMenuScreen, TrackFileMan, 0x0004);
CHECK_OFFSET(cDebugMenuScreen, BackgroundMan, 0x8C10);
CHECK_OFFSET(cDebugMenuScreen, SelectedItem, 0x8C1C);
CHECK_OFFSET(cDebugMenuScreen, currentLocation, 0x8C20);
CHECK_OFFSET(cDebugMenuScreen, CurrentBoard, 0x8C24);
CHECK_OFFSET(cDebugMenuScreen, CurrentChar, 0x8C28);
CHECK_OFFSET(cDebugMenuScreen, CurrentMap, 0x8C2C);
CHECK_OFFSET(cDebugMenuScreen, CurrentBack, 0x8C30);
CHECK_OFFSET(cDebugMenuScreen, CurrentPlaylist, 0x8C34);
CHECK_OFFSET(cDebugMenuScreen, LastGamePathGameMode, 0x8C38);
CHECK_OFFSET(cDebugMenuScreen, NetworkMode, 0x8C44);
CHECK_SIZE(cDebugMenuScreen, 0x8C48);

// ---------------------------------------------------------------------------------------------------------------------
// cDebugLobbyScreen: the debug front end's online lobby screen (cDebugFE::Screens[1], "LobbyMenu"; the July 2003
// preview names the class): a menu system with eight debug menus (login, persona, room, occupants, chat, challenge) and
// a state machine over the lobby manager's events, ending in a two-player game over a DirtySock channel. No On Tour
// class (On Tour has no online front end), so its own fields are named from their retail use. Retail code: fe/debugfe:
// the ctor (0x0017D298), the dtor, update, the lobby-event handler func_0017D6F0, the error / state / channel steps
// func_0017DA30 / func_0017DB18 / func_0017DE70, the menu callbacks func_0017E030 ... func_0017E558 and draw
// (0x0017E5C8).
//
// Size 0x1C18: [R] cDebugFE_load: cMemMan_alloc(7192, "LobbyMenu") + cDebugLobbyScreen_cDebugLobbyScreen; the ctor's
// regions follow from the sub-object ctors' addresses and the dtor's item loops; the last words (0x1C10, 0x1C14) are
// used by func_0017DE70 / func_0017DA30.
// Base: cDebugScreen (the vptr only). Vtable cDebugLobbyScreen_vtbl (0x0046D350, 3 slots: 1 dtor, 2 update, 3 draw);
// the dtor clears the screen pointer D_004A4E90 (the ctor sets it; func_0017D260 sends lobby events through it),
// destroys the strings, menus and menu system and stores cDebugScreen_vtbl. The menus are raw regions: their classes
// (cMenu / cMenuItem subclasses built by AccountMenu_AccountMenu ... AbortMenu_AbortMenu) are not laid out here.
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's ctor / dtor / allocation; [U] how retail code uses the
// field (src/fe/debugfe.cpp views sDebugFE17D6F0 / 17DB18 / 17D688 / 17E5C8 / sDebugFEState17E*, and the asm).

struct cDebugLobbyScreen {
    // --- cDebugScreen (the vptr)
    void** mVtbl;                   // 0x0000 [R] cDebugLobbyScreen_vtbl (ctor, dtor), then cDebugScreen_vtbl (dtor)
    // --- cDebugLobbyScreen
    int mState;                     // 0x0004 [R] ctor 0; [U] the lobby state (0 / 1 connecting, 2 lobby, 3 login menu,
                                    //       4 creating account, 5 authenticating, 6 / 7 persona, 8 / 9 create / delete
                                    //       persona, 10 / 11 room, 12 occupants, 13 waiting, 14 chat, 15 challenged, 16
                                    //       play game, 17 declined, 18 ... 24 errors): func_0017D6F0 / func_0017DB18 /
                                    //       the callbacks set it, draw prints its line
    cMenuSys mMenuSys;              // 0x0008 cMenuSys (util/menusys.h, 0x68); [R] cMenuSys_cMenuSys(&mMenuSys) /
                                    //       cMenuSys__cMenuSys(.., 2); the ctor sets mMenuSys.Font / Input / BoxTexture
                                    //       (0x60 / 0x64 / 0x68) from the debug FE's Font, SSXApp's Inputs[0] and BoxTID;
                                    //       [U] the open-menu stack (MenuDepth, MenuStack): cMenuSys_pushMenu / popMenu
                                    //       (func_0017E368 / func_0017E3B8 pop while MenuDepth != 0) / isActive / input /
                                    //       render
    AccountMenu mLoginMenu;         // 0x0070 [R] the "Login Method" menu (Create Account, Login): AccountMenu_AccountMenu(this +
                                    //       0x70, ...), AccountMenu__AccountMenu(.., 2) (dtor); [U] pushed on the login step
                                    //       (state 3)
    ContinueMenu mContinueMenu;     // 0x0604 [R] ContinueMenu_ContinueMenu(this + 0x604) (fe/debugfe: a cMenu + one
                                    //       cPopMenuItem at +0x130); [U] pushed under every error and "declined"
                                    //       message
    PersonaMenu mPersonaMenu;       // 0x074C [R] "Select Persona" (Create / Delete Persona): PersonaMenu_PersonaMenu /
                                    //       PersonaMenu__PersonaMenu; [U] PersonaMenu_setpersonas fills it from the lobby manager
    LobbyMenu mRoomMenu;            // 0x0D2C [R] "Select Room" (12 items): LobbyMenu_LobbyMenu; dtor loop over 12 * 0x24 from
                                    //       +0x168
    RoomMenu mOccupantsMenu;        // 0x1044 [R] "Room Occupants" (24 items, Chat): RoomMenu_RoomMenu; [U] RoomMenu_updateusers
                                    //       fills it
    ChatMenu mChatMenu;             // 0x1534 [R] the chat menu (14 lines): ChatMenu_ChatMenu; dtor loop over 14 * 0x24 from
                                    //       +0x194
    ChallengeResponseMenu mChallengedMenu;// 0x18C0 [R] "You've been Challenged!" (Accept, Decline):
                                    //       ChallengeResponseMenu_ChallengeResponseMenu(this + 0x18C0, ...); [U] func_0017D6F0 stores
                                    //       the challenger's name into its third line (lw/sw 0x1A44 = +0x184)
    AbortMenu mAbortMenu;           // 0x1AAC [R] "Abort": AbortMenu_AbortMenu; [U] func_0017DB18 pushes it while a challenge
                                    //       is pending
    char* mUserName;                // 0x1C04 cBXString (a char*, bx/bxstring.h); [R] ctor cBXString_EmptyString, dtor
                                    //       cBXString__cBXString; [U] func_0017E030 / func_0017E118 keep the typed
                                    //       name; it is copied to LobbyMan's mUserName
    char* mPassword;                // 0x1C08 cBXString; [R] as mUserName; [U] func_0017E030 / func_0017E118 keep the
                                    //       typed password; copied to LobbyMan's mPassword
    char* mChallenger;              // 0x1C0C cBXString; [R] as mUserName; [U] func_0017D6F0 copies the challenge
                                    //       event's name (+0x8) into it; func_0017E490 / func_0017E4F0 / func_0017E558
                                    //       answer that rider
    int mGameChannel;               // 0x1C10 [U] func_0017DE70 stores cCommSystem_openDSockChannel's channel;
                                    //       func_0017DB18 (state 16) waits for it to connect and builds the game comm
                                    //       on it
    int mErrorCode;                 // 0x1C14 [U] func_0017DA30 stores the lobby error code; draw prints its text
                                    //       (func_00260E50) in the error states

    // The menu callbacks (C++ only; no data in the screen). [P] the preview's template instances name them:
    // AccountMenu<cDebugLobbyScreen::OnCreate, cDebugLobbyScreen::OnLogin>, PersonaMenu<::OnSelectPersona,
    // ::OnCreatePersona, ::OnDeletePersona>, LobbyMenu<::OnSelectRoom>, RoomMenu<::OnSelectUser, ::OnChat>,
    // ChatMenu<::OnMessage>, ChallengeResponseMenu<::OnAcceptChallenge, ::OnDeclineChallenge>,
    // AbortMenu<::OnAbortChallenge> (linkonce _$_t9AbortMenu1ZQ217cDebugLobbyScreen16OnAbortChallenge ..., and the
    // ctors take them by const reference: __t9AbortMenu1Z..RCQ217cDebugLobbyScreen16OnAbortChallenge).
    // Each is one word: [R] cDebugLobbyScreen_cDebugLobbyScreen builds each on the stack holding this screen and passes
    // its address; the menu ctors copy the first word (fe/debuglobbymenus.h: the menus' unk_130 / unk_4E0 ...).
    // The member's name is not in evidence.
#ifdef __cplusplus
    struct OnCreate { cDebugLobbyScreen* unk_0; };          // 0x0 [R] the owning screen
    struct OnLogin { cDebugLobbyScreen* unk_0; };
    struct OnSelectPersona { cDebugLobbyScreen* unk_0; };
    struct OnCreatePersona { cDebugLobbyScreen* unk_0; };
    struct OnDeletePersona { cDebugLobbyScreen* unk_0; };
    struct OnSelectRoom { cDebugLobbyScreen* unk_0; };
    struct OnSelectUser { cDebugLobbyScreen* unk_0; };
    struct OnChat { cDebugLobbyScreen* unk_0; };
    struct OnMessage { cDebugLobbyScreen* unk_0; };
    struct OnAcceptChallenge { cDebugLobbyScreen* unk_0; };
    struct OnDeclineChallenge { cDebugLobbyScreen* unk_0; };
    struct OnAbortChallenge { cDebugLobbyScreen* unk_0; };
#endif
};

CHECK_OFFSET(cDebugLobbyScreen, mVtbl, 0x0000);
CHECK_OFFSET(cDebugLobbyScreen, mState, 0x0004);
CHECK_OFFSET(cDebugLobbyScreen, mMenuSys, 0x0008);
CHECK_OFFSET(cDebugLobbyScreen, mLoginMenu, 0x0070);
CHECK_OFFSET(cDebugLobbyScreen, mContinueMenu, 0x0604);
CHECK_OFFSET(cDebugLobbyScreen, mPersonaMenu, 0x074C);
CHECK_OFFSET(cDebugLobbyScreen, mRoomMenu, 0x0D2C);
CHECK_OFFSET(cDebugLobbyScreen, mOccupantsMenu, 0x1044);
CHECK_OFFSET(cDebugLobbyScreen, mChatMenu, 0x1534);
CHECK_OFFSET(cDebugLobbyScreen, mChallengedMenu, 0x18C0);
CHECK_OFFSET(cDebugLobbyScreen, mAbortMenu, 0x1AAC);
CHECK_OFFSET(cDebugLobbyScreen, mUserName, 0x1C04);
CHECK_OFFSET(cDebugLobbyScreen, mPassword, 0x1C08);
CHECK_OFFSET(cDebugLobbyScreen, mChallenger, 0x1C0C);
CHECK_OFFSET(cDebugLobbyScreen, mGameChannel, 0x1C10);
CHECK_OFFSET(cDebugLobbyScreen, mErrorCode, 0x1C14);
CHECK_SIZE(cDebugLobbyScreen, 0x1C18);
// CHECK_SIZE cannot paste a qualified name, so the functor checks are spelled out.
#if defined(__cplusplus) && !defined(DECOMP_NO_LAYOUT_CHECKS)
typedef char check_cDebugLobbyScreen_OnCreate_size[(sizeof(cDebugLobbyScreen::OnCreate) == 0x4) ? 1 : -1];
typedef char check_cDebugLobbyScreen_OnAbortChallenge_size[(sizeof(cDebugLobbyScreen::OnAbortChallenge) == 0x4) ? 1 : -1];
#endif

#endif /* FE_DEBUGFE_H */
