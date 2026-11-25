#ifndef FE_FESTATECAREERSTATS_H
#define FE_FESTATECAREERSTATS_H

// cFEStateCareerStats: the career stats screen (a cUIState); cFEReal creates it with cMemMan_alloc(0x78, "cFEStateCareerStats").
// Size 0x78 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateCareerStats : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateCareerStats
    int mStatsState;                 // 0x048 [O]
    int mTopHighlightNumber;         // 0x04C [O]
    int mTopRaceEvent;               // 0x050 [O]
    int mTopFreestyleEvent;          // 0x054 [O]
    int mTotalRaceEvent;             // 0x058 [O]
    int mTotalOwnedRaceEvent;        // 0x05C [O]
    unsigned int mbitOwnedRaceMedal; // 0x060 [O]
    unsigned int mbitOpenedRace;     // 0x064 [O]
    int mTotalFreestyleEvent;        // 0x068 [O]
    int mTotalOwnedFSEvent;          // 0x06C [O]
    unsigned int mbitOwnedFSMedal;   // 0x070 [O]
    unsigned int mbitOpenedFS;       // 0x074 [O]

    // Methods bound to their retail symbols by asm labels.
    void setupLayout() __asm__("cFEStateCareerStats_setupLayout");
    void setupMenuFocus(int menu) __asm__("cFEStateCareerStats_setupMenuFocus");
    void setupHighlightsList() __asm__("cFEStateCareerStats_setupHighlightsList");
    void setupRacePlatinumMedalsList() __asm__("cFEStateCareerStats_setupRacePlatinumMedalsList");
    void setupFSPlatinumMedalsList() __asm__("cFEStateCareerStats_setupFSPlatinumMedalsList");
    void setTextSafe(char* name, const char* str) __asm__("cFEStateCareerStats_setTextSafe");
    void setupRidersBest() __asm__("cFEStateCareerStats_setupRidersBest");
    int getOwnedMedalByith(unsigned int mask, int n, int count) __asm__("cFEStateCareerStats_getOwnedMedalByith");
    void initTotalEventMedal() __asm__("cFEStateCareerStats_initTotalEventMedal");
};

CHECK_OFFSET(cFEStateCareerStats, mStatsState, 0x048);
CHECK_OFFSET(cFEStateCareerStats, mTopHighlightNumber, 0x04C);
CHECK_OFFSET(cFEStateCareerStats, mTopRaceEvent, 0x050);
CHECK_OFFSET(cFEStateCareerStats, mTopFreestyleEvent, 0x054);
CHECK_OFFSET(cFEStateCareerStats, mTotalRaceEvent, 0x058);
CHECK_OFFSET(cFEStateCareerStats, mTotalOwnedRaceEvent, 0x05C);
CHECK_OFFSET(cFEStateCareerStats, mbitOwnedRaceMedal, 0x060);
CHECK_OFFSET(cFEStateCareerStats, mbitOpenedRace, 0x064);
CHECK_OFFSET(cFEStateCareerStats, mTotalFreestyleEvent, 0x068);
CHECK_OFFSET(cFEStateCareerStats, mTotalOwnedFSEvent, 0x06C);
CHECK_OFFSET(cFEStateCareerStats, mbitOwnedFSMedal, 0x070);
CHECK_OFFSET(cFEStateCareerStats, mbitOpenedFS, 0x074);
CHECK_SIZE(cFEStateCareerStats, 0x78);

#endif
