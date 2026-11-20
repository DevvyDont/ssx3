#ifndef FE_FESTATECAREERSTATS_H
#define FE_FESTATECAREERSTATS_H

// cFEStateCareerStats: the career stats screen (a cUIState); cFEReal creates it with cMemMan_alloc(0x78, "cFEStateCareerStats").
// Size 0x78 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateCareerStats {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 the vtable
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
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
