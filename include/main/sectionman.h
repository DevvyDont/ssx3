#ifndef MAIN_SECTIONMAN_H
#define MAIN_SECTIONMAN_H

// cSectionMan: the game-side section manager. It maps the back end's logical sections (eBESectionID: course parts,
// hubs, the five skies 0x2C-0x30; 0x31 = none) and locations (eBELocationID: an event's set of sections) to world
// sections, queues their load/activate/unload requests on the world (cWorldView loadSection/unloadSection/
// activateSection), swaps the sky, sets the stream bandwidth, and runs the event-switch ("mode") sequence.
// One instance: cGame_loadTrack allocates it as `cSectionMan_cSectionMan(cMemMan_alloc(0x1D4, "SectionMan"), game)`; reached
// as SSXApp->Game->SectionMan (cGame::SectionMan, main/game.h). No vtable, no base class.
// EA's source: d:/ssx/code/source/main/sectionman.cpp (preview and retail assert strings).
//
// Statics (On Tour names; retail data): mSectionMap[50] = D_00442168 (tSectionMap 0x10: SectionID, iWorldSectionID,
// State, Priority; the methods' views call them field_0x0..field_0xc), mLocationMap = D_00442488 (0x28 each),
// mLocationModeMap = D_00442820, mStreamBandWidthRates[5] = D_00442150 = {256000, 460800, 819200, 819200, 819200}.
//
// Methods (names from On Tour's cSectionMan, matched by body and call pattern):
//   main/game: cSectionMan_cSectionMan (ctor), cSectionMan__cSectionMan (dtor), cSectionMan_init (init), func_0022CEA8 (load),
//     cSectionMan_activate (activate), func_0022D278 (isLoaded), cSectionMan_isActive (isActive), cSectionMan_loadSection (loadSection),
//     cSectionMan_activateSection (activateSection), cSectionMan_unloadSection (unloadSection), cSectionMan_loadMode (loadMode),
//     cSectionMan_activateMode (activateMode), cSectionMan_eventbegin, cSectionMan_update, cSectionMan_skyLoadComplete (skyLoadComplete),
//     cSectionMan_skySwap, cSectionMan_preLoadMode (preLoadMode), func_0022DFD0 (isPreLoadModeDone), cSectionMan_changeMode (changeMode),
//     cSectionMan_isSectionLoaded (isSectionLoaded), cSectionMan_getWorldSectionIDFromBELocationID (getWorldSectionIDFromBELocationID),
//     cSectionMan_getWorldSectionIDFromBESectionID (getWorldSectionIDFromBESectionID), cSectionMan_getBESectionIDFromWorldSectionID (getBESectionIDFromWorldSectionID),
//     cSectionMan_gotoLocation (gotoLocation);
//   main/sectionman: cSectionMan_setSky, cSectionMan_initEventPool (initEventPool), func_0022E278 (allocEvent),
//     func_0022E288 (freeEvent), cSectionMan_addActiveEvent (addActiveEvent), cSectionMan_getNextActiveEvent,
//     cSectionMan_removeActiveEvent (removeActiveEvent).
//   Inline accessors used elsewhere: tiggerskyswap (sw 0x1A4), getLocationID (lw 0x1BC), transportwait /
//   iswaitingtransport (0x1C8), isStreamReady (lw 0x1D0).
//
// Size 0x1D4 [R] (the allocation). On Tour's cSectionMan is 0x218: a 61-event pool and byte bools. Retail has a
// 51-event pool (initEventPool links 50 events plus a terminator at 0x190) and keeps every bool a 4-byte word, so
// retail's tail = On Tour's order with 4-byte bools, ending exactly at 0x1D4.
//
// A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF (class cSectionMan), placed by:
//   [P] the July 2003 preview's assert strings in sectionman.cpp ("m_pFreeEventList", "pPrevEvent->pNext==pEvent",
//       "mpGame->World->isSectionLoaded(...)", "mMode==Mode");
//   [R] retail's constructor and init (cSectionMan_cSectionMan, cSectionMan_init, cSectionMan_initEventPool);
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [P], [R] or [U].
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cGame;

// One queued section request (On Tour tSectionManEvent).
struct tSectionManEvent {
    int SectionID;                  // 0x0 [O] eBESectionID; [R] initEventPool = 0x31 (none); [U] addActiveEvent stores
                                    //     the id, update indexes mSectionMap with it
    tSectionManEvent* pNext;        // 0x4 [P] assert pPrevEvent->pNext==pEvent; [R] initEventPool chains the pool
};

CHECK_OFFSET(tSectionManEvent, SectionID, 0x0);
CHECK_OFFSET(tSectionManEvent, pNext, 0x4);
CHECK_SIZE(tSectionManEvent, 0x8);

struct cSectionMan {
    tSectionManEvent m_EventPool[51];      // 0x000 [O] (On Tour 61); [R] initEventPool: 50 linked + a terminator at 0x190
    tSectionManEvent* m_pFreeEventList;    // 0x198 [P] assert m_pFreeEventList; [R] = &m_EventPool[0];
                                           //       [U] allocEvent pops, freeEvent pushes
    tSectionManEvent* m_pActiveEventList;  // 0x19C [O]; [R] = 0; [U] addActiveEvent appends (dedup), getNextActiveEvent
    int m_iNumActiveEvent;                 // 0x1A0 [O]; [R] = 0; [U] addActiveEvent +1, removeActiveEvent -1
    int m_bTriggerSkySwap;                 // 0x1A4 [O] (a bool); [R] init = 0; [U] skySwap needs it set (then clears it);
                                           //       tiggerskyswap() = 1 in cGame_loadTrack, cGFTrackStartState_update,
                                           //       func_00302210; load clears it
    int m_bSkySwapPending;                 // 0x1A8 [O] (a bool); [R] init = 0; [U] skyLoadComplete = 1; update calls
                                           //       skySwap while it is set; skySwap clears it
    int m_iNextSkySectionID;               // 0x1AC [O]; [R] init = -1; [U] skyLoadComplete stores the sky section,
                                           //       skySwap uses and resets it
    cGame* mpGame;                         // 0x1B0 [P] assert mpGame->World; [R] ctor stores its Game argument;
                                           //       [U] mpGame->World->m_pWorldCache->m_WorldView / m_StreamMan
    int mModeState;                        // 0x1B4 [O] eSectionModeState; [R] init = 0; [U] loadMode/activateMode, the
                                           //       event-switch switch in update (0..4)
    int mMode;                             // 0x1B8 [P] assert mMode==Mode; [R] init = 0; [U] loadMode/activateMode store
                                           //       it, update clears it
    int mBELocationID;                     // 0x1BC [O] eBELocationID; [R] init = 0x16; [U] load/activate store it;
                                           //       getLocationID() is read by game states, path and script code
    int mbLocationIsActivated;             // 0x1C0 [O] (a bool); [R] init = 0; [U] activate = 1, load = 0, isActive
    int mStreamRate;                       // 0x1C4 [O] eStreamRate; [R] init = 0; [U] update stores 0..4 and passes
                                           //       mStreamBandWidthRates[it] to cStreamMan setBandWidthLimit
    int mWaitTransport;                    // 0x1C8 [O] (a bool); [R] init = 0; [U] transportwait() in
                                           //       cGFUserSelectState_update, func_00236960 (preview assert names
                                           //       getSectionMan()->iswaitingtransport())
    int mModeReset;                        // 0x1CC [O] (a bool); [R] init = 0; [U] gotoLocation = 1; changeMode clears
                                           //       it and calls the AI's vtable slot 5
    int m_bStreamReady;                    // 0x1D0 [O] (a bool); [R] init = 1; [U] update stores 0/1; isStreamReady()
                                           //       in cGame_update, func_00244A40
};

CHECK_OFFSET(cSectionMan, m_EventPool, 0x000);
CHECK_OFFSET(cSectionMan, m_pFreeEventList, 0x198);
CHECK_OFFSET(cSectionMan, m_pActiveEventList, 0x19C);
CHECK_OFFSET(cSectionMan, m_iNumActiveEvent, 0x1A0);
CHECK_OFFSET(cSectionMan, m_bTriggerSkySwap, 0x1A4);
CHECK_OFFSET(cSectionMan, m_bSkySwapPending, 0x1A8);
CHECK_OFFSET(cSectionMan, m_iNextSkySectionID, 0x1AC);
CHECK_OFFSET(cSectionMan, mpGame, 0x1B0);
CHECK_OFFSET(cSectionMan, mModeState, 0x1B4);
CHECK_OFFSET(cSectionMan, mMode, 0x1B8);
CHECK_OFFSET(cSectionMan, mBELocationID, 0x1BC);
CHECK_OFFSET(cSectionMan, mbLocationIsActivated, 0x1C0);
CHECK_OFFSET(cSectionMan, mStreamRate, 0x1C4);
CHECK_OFFSET(cSectionMan, mWaitTransport, 0x1C8);
CHECK_OFFSET(cSectionMan, mModeReset, 0x1CC);
CHECK_OFFSET(cSectionMan, m_bStreamReady, 0x1D0);
CHECK_SIZE(cSectionMan, 0x1D4);

#endif /* MAIN_SECTIONMAN_H */
