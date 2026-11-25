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
// Statics (On Tour names; retail data): mSectionMap[50] = cSectionMan_mSectionMap (tSectionMap 0x10: SectionID, iWorldSectionID,
// State, Priority; the methods' views call them field_0x0..field_0xc), mLocationMap = cSectionMan_mLocationMap (0x28 each),
// mLocationModeMap = D_00442820, mStreamBandWidthRates[5] = cSectionMan_mStreamBandWidthRates = {256000, 460800, 819200, 819200, 819200}.
//
// Methods (names from On Tour's cSectionMan, matched by body and call pattern):
//   main/game: cSectionMan_cSectionMan (ctor), cSectionMan__cSectionMan (dtor), cSectionMan_init, cSectionMan_load,
//     cSectionMan_activate, cSectionMan_isLoaded, cSectionMan_isActive, cSectionMan_loadSection,
//     cSectionMan_activateSection, cSectionMan_unloadSection, cSectionMan_loadMode,
//     cSectionMan_activateMode, cSectionMan_eventbegin, cSectionMan_update, cSectionMan_skyLoadComplete,
//     cSectionMan_skySwap, cSectionMan_preLoadMode, cSectionMan_isPreLoadModeDone, cSectionMan_changeMode,
//     cSectionMan_isSectionLoaded, cSectionMan_getWorldSectionIDFromBELocationID,
//     cSectionMan_getWorldSectionIDFromBESectionID, cSectionMan_getBESectionIDFromWorldSectionID,
//     cSectionMan_gotoLocation;
//   main/sectionman: cSectionMan_setSky, cSectionMan_initEventPool, cSectionMan_allocEvent,
//     cSectionMan_freeEvent, cSectionMan_addActiveEvent, cSectionMan_getNextActiveEvent,
//     cSectionMan_removeActiveEvent.
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

// One row of the static section map mSectionMap[50] (cSectionMan_mSectionMap): a back-end section and the world
// section it streams as (On Tour struct tSectionMap, 0x10, same four members; retail's table has 50 rows, the loop bound
// of cSectionMan_init). Replaces the views sGameEntry (main/game) and field_0x0..field_0xc in the methods.
// [Staged by lstruct group L2.]
struct tSectionMap {
    int SectionID;                  // 0x0 [O] eBESectionID; [U] the game's sections are looked up by it
                                    //     (getWorldSectionIDFromBESectionID / getBESectionIDFromWorldSectionID)
    int iWorldSectionID;            // 0x4 [O]; [R] cSectionMan_init stores -1, then the world section whose name matches
                                    //     the row's; [U] passed to cWorldView loadSection / activateSection / unloadSection
    int State;                      // 0x8 [O] eSectionState; [R] cSectionMan_init stores 0; [U] cSectionMan_update
                                    //     switches on it (the section's load / activate state machine)
    int Priority;                   // 0xC [O] eSectionLoadPriority; [U] indexes D_0044C208, which becomes
                                    //     cWorldView_loadSection's priority argument
};

CHECK_OFFSET(tSectionMap, SectionID, 0x0);
CHECK_OFFSET(tSectionMap, iWorldSectionID, 0x4);
CHECK_OFFSET(tSectionMap, State, 0x8);
CHECK_OFFSET(tSectionMap, Priority, 0xC);
CHECK_SIZE(tSectionMap, 0x10);

// A section-manager game mode (mMode, eventbegin's mode): [O] enum eSectionManMode, names and values. [U] the script
// mode table D_00445E40 that SetGameModeFunc and SectionFunc read holds {2, 3, 4, 5, 1, 1, 1, 0}: script modes 0-3 are
// race, slopestyle, big air and halfpipe, 4-6 freeride, 7 none.
enum eSectionManMode {
    kSMMode_None = 0,
    kSMMode_Freeride = 1,
    kSMMode_Race = 2,
    kSMMode_SlopeStype = 3,                // [O] spelling
    kSMMode_BigAir = 4,
    kSMMode_HalfPipe = 5,
    kSMMode_TimeChallenge = 6,
    kSMMode_Default = 7,
    kSMMode_Num = 8
};

struct cSectionMan {
    tSectionManEvent m_EventPool[51];      // 0x000 [O] (On Tour 61); [R] initEventPool: 50 linked + a terminator at 0x190
    tSectionManEvent* m_pFreeEventList;    // 0x198 [P] assert m_pFreeEventList; [R] = &m_EventPool[0];
                                           //       [U] allocEvent pops, freeEvent pushes
    tSectionManEvent* m_pActiveEventList;  // 0x19C [O]; [R] = 0; [U] addActiveEvent appends (dedup), getNextActiveEvent
    int m_iNumActiveEvent;                 // 0x1A0 [O]; [R] = 0; [U] addActiveEvent +1, removeActiveEvent -1
    int m_bTriggerSkySwap;                 // 0x1A4 [O] (a bool); [R] init = 0; [U] skySwap needs it set (then clears it);
                                           //       tiggerskyswap() = 1 in cGame_loadTrack, cGFTrackStartState_update,
                                           //       SectionFunc; load clears it
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
                                           //       cGFUserSelectState_update, cGFSectionLoadState_update (preview assert names
                                           //       getSectionMan()->iswaitingtransport())
    int mModeReset;                        // 0x1CC [O] (a bool); [R] init = 0; [U] gotoLocation = 1; changeMode clears
                                           //       it and calls the AI's vtable slot 5
    int m_bStreamReady;                    // 0x1D0 [O] (a bool); [R] init = 1; [U] update stores 0/1; isStreamReady()
                                           //       in cGame_update, cGame_updateStreamLoading

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cSectionMan_init");
    void activate(int idx, int mode) __asm__("cSectionMan_activate");
    int isActive() __asm__("cSectionMan_isActive");
    void loadSection(int idx) __asm__("cSectionMan_loadSection");
    void activateSection(int i) __asm__("cSectionMan_activateSection");
    void unloadSection(int idx) __asm__("cSectionMan_unloadSection");
    void loadMode(int Mode) __asm__("cSectionMan_loadMode");
    void activateMode(int flag) __asm__("cSectionMan_activateMode");
    void eventbegin(int mode) __asm__("cSectionMan_eventbegin");
    void update() __asm__("cSectionMan_update");
    void skyLoadComplete(int SectionID) __asm__("cSectionMan_skyLoadComplete");
    void skySwap() __asm__("cSectionMan_skySwap");
    void preLoadMode() __asm__("cSectionMan_preLoadMode");
    void* isPreLoadModeDone() __asm__("cSectionMan_isPreLoadModeDone__FPv");
    void changeMode() __asm__("cSectionMan_changeMode");
    int isSectionLoaded(int SectionID) __asm__("cSectionMan_isSectionLoaded");
    int getWorldSectionIDFromBELocationID(int LocationID) __asm__("cSectionMan_getWorldSectionIDFromBELocationID");
    int getWorldSectionIDFromBESectionID(int SectionID) __asm__("cSectionMan_getWorldSectionIDFromBESectionID");
    int getBESectionIDFromWorldSectionID(int WorldSectionID) __asm__("cSectionMan_getBESectionIDFromWorldSectionID");
    void gotoLocation(int location) __asm__("cSectionMan_gotoLocation");
    void setSky(int sky) __asm__("cSectionMan_setSky");
    void initEventPool() __asm__("cSectionMan_initEventPool");
    void* allocEvent() __asm__("cSectionMan_allocEvent__FPv");
    int freeEvent(void* node) __asm__("cSectionMan_freeEvent__FPvT0");
    void addActiveEvent(int id) __asm__("cSectionMan_addActiveEvent");
    int getNextActiveEvent(tSectionManEvent* pPrevEvent) __asm__("cSectionMan_getNextActiveEvent");
    void removeActiveEvent(tSectionManEvent* prev, tSectionManEvent* node) __asm__("cSectionMan_removeActiveEvent");
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
