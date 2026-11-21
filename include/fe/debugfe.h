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
// 5 gainFocus, 6 update, 7 render); the dtor stores cAppModule's vtable D_0046D970.
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

struct cDebugScreen;
struct cFont;

struct cDebugFE {
    // --- cAppModule / cFadeScreen (0xC; main/fadescreen.h)
    void** mVtbl;                   // 0x000 [R] cFadeScreen_vtbl, then cDebugFE_vtbl (ctor); D_0046D970 (dtor);
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

#endif /* FE_DEBUGFE_H */
