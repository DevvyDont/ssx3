#ifndef BE_BEINTSCORE_H
#define BE_BEINTSCORE_H

// cBEScoreInterface: the back end's score interface - processes a finished event for each human rider (the event
// stats, the event's top-5 high-score table, the character's best records and medals) and keeps, per profile, the last
// event's stats and rank and whether it set a new personal / track high score. Like every BE interface it is a
// cBEInterface (three word bools + vptr); it adds the new-high-score flags and indexes and the last-event stats. Its
// other methods ignore `this` and work on the high-score tables and the profile library. One instance, created on
// demand by cBEScoreInterface_getThis (`cMemMan_alloc(0x230, "BEScoreInt", 0, 0)` + the constructor
// cBEScoreInterface_cBEScoreInterface, then the inlined resets below) into cBEScoreInterface::instance (retail
// cBEScoreInterface_instance, D_004A1248); the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 8). EA's
// source: d:/ssx/code/source/be/beintscore.cpp (preview assert string).
//
// Size 0x230 [R] (the allocation). The July 2003 preview's object is 0x19A0 bytes with a different layout (its
// isNewHighScore / isNewTrackHighScore read 0x1990/0x1994 and 0x1998/0x199C), so the preview gives no offsets here.
// Base cBEInterface (0x00-0x0F), written out inline (the class itself: be/beinterface.h, whose virtuals give the slots):
// vtable cBEScoreInterface_vtbl (0x45ACA8), slot 1 submit = cBEScoreInterface_submit (empty), slot 2 discard =
// cBEScoreInterface_discard, slots 3/4 its own cBEScoreInterface_writeToReplayFrame / readFromReplayFrame (empty).
// [O] On Tour's cBEScoreInterface (size 0x2E8): bNewHighScore[2], mNewTrackHighScoreIndex[2], m_iBestScoreInEvent,
// LastEventStats[4] (tLastEventRiderStats, 0xB4 there). Retail has no room for m_iBestScoreInEvent (LastEventStats
// starts at 0x20, right after the index pair) and its tLastEventRiderStats is 0xB0 (tRiderStats is 0xAC here).
//
// Evidence tags:
//   [R] retail's getThis (allocation; bIsActive = 0, 0x10/0x14 = 0, 0x18/0x1C = -1), constructor (vptr) and
//       cBEScoreInterface_discard (the same resets, then zeroes 0x20-0x17F and sets the two ranks at 0xCC / 0x17C to
//       -1);
//   [P] the July 2003 preview's inline methods __17cBEScoreInterface (stores the vtable at 0xC) and setScoreActive;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBEScoreInterface / tLastEventRiderStats; On Tour's bools are 1 byte,
//       retail's are words).

#include "layout.h"
#include "be/beinterface.h"
#include "ai/ridermetrix.h"

// A profile's last finished event: its rider stats and finishing rank (On Tour tLastEventRiderStats, 0xB4 there).
struct tLastEventRiderStats {
    tRiderStats ridingStats;         // 0x00 [O]; [R] cBEScoreInterface_processEventRiderStats copies the event's
                                     //       0xAC-byte stats in
    int rank;                        // 0xAC [O]; [R] discard and processEventRiderStats set -1; processRiderRanking
                                     //       stores the finishing rank (0 = first)
};

CHECK_OFFSET(tLastEventRiderStats, ridingStats, 0x00);
CHECK_OFFSET(tLastEventRiderStats, rank, 0xAC);
CHECK_SIZE(tLastEventRiderStats, 0xB0);

// tBEHighScore: one entry of a location / event's top-5 high-score table. [O] On Tour struct tBEHighScore (0x20:
// uiHighScore, cCharacter, strUserName[24]), the table cBELibrary::highScores (tBEHighScore[..][26]) and
// tBEHighScore* getHighScoreTable(eBELocationID, eBEGameEvent); [R] retail cBELibrary_getHighScoreTable returns
// &cBELibrary_highScores[cBELibrary_getScoreType(location, event)] with a 0x64-byte stride (5 entries of 0x14; the
// array is 2600 bytes = 26 x 0x64), so retail's name is 12 bytes. [U] cBEScoreInterface's high-score getters
// (func_001550E8 / func_00155130 return uiHighScore, func_00155190 the character's entry for cCharacter,
// func_00155208 strUserName).
struct tBEHighScore {
    unsigned int uiHighScore;        // 0x00 [O]; [U] the score (func_001550E8 / func_00155130 return it)
    int cCharacter;                  // 0x04 [O]; [U] the character id (func_00155190 indexes D_00530990 with it)
    char strUserName[12];            // 0x08 [O] (24 bytes in On Tour); [R] the 0x14-byte entry stride; [U] func_00155208
                                     //      returns it
};

CHECK_OFFSET(tBEHighScore, uiHighScore, 0x00);
CHECK_OFFSET(tBEHighScore, cCharacter, 0x04);
CHECK_OFFSET(tBEHighScore, strUserName, 0x08);
CHECK_SIZE(tBEHighScore, 0x14);

struct tRiderStats;

struct cBEScoreInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] setScoreActive; [U] cBEScoreInterface_setScoreActive (static:
    //       instance->bIsActive = arg); cBERaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] cBEScoreInterface_cBEScoreInterface stores cBEScoreInterface_vtbl; [P]
    //       __17cBEScoreInterface; [U] cBELibrary_readFromData, cBESaveInterface_generateSaveProfile and loadSaveGame
    //       call slot 2 (discard)
    // cBEScoreInterface
    int bNewHighScore[2];            // 0x010 [O] bool[2] (On Tour 0x008); [R] getThis / discard set 0;
                                     //       [U] processEventRiderStats clears [profile], then stores
                                     //       cBECharProfileDB_updateScoreStats's result (a new best for the character);
                                     //       isNewPersonalHighScore (preview isNewHighScore) tests [0] || [1]
    int mNewTrackHighScoreIndex[2];  // 0x018 [O] (On Tour 0x00C); [R] getThis / discard set -1; [U]
                                     //       processEventRiderStats stores the profile's place in the event's top-5
                                     //       table (-1 none) and moves the other profile's down; isNewTrackHighScore
                                     //       ([0] >= 0 || [1] >= 0), GetRiderNewTrackHighScoreIndex ([i]);
                                     //       cOVStateHiScoreList_onCreateScreen, cOVState_TOPTIMES_onGainFocus and
                                     //       cOVStateManager_gotoOverlay read both
    tLastEventRiderStats LastEventStats[3]; // 0x020 [O] (On Tour [4] at 0x018); [R] discard zeroes [0] and [1] (0x160
                                     //       bytes) and sets their ranks to -1; [U] processEventRiderStats /
                                     //       processRiderRanking write [profile] for profiles 0 and 1,
                                     //       getPlayerLastCompletedEventStats returns &[i] (festateloadhint passes a
                                     //       rider slot from getHumanRider). Three elements: the allocation (0x230 =
                                     //       0x20 + 3 * 0xB0) leaves room for exactly one more, matching
                                     //       cBELibrary_getProfileIndex's three profiles (0, 1 humans, 2 computer); no
                                     //       retail access to [2] (0x180-0x22F) seen

    // Methods bound to their retail symbols by asm labels.
    int isNewPersonalHighScore() __asm__("cBEScoreInterface_isNewPersonalHighScore");
    int isNewTrackHighScore() __asm__("cBEScoreInterface_isNewTrackHighScore");
    int convertRankToMedal(int rank) __asm__("cBEScoreInterface_convertRankToMedal");
    void processEventRiderStats(int rider, tRiderStats* stats, unsigned int score)
        __asm__("cBEScoreInterface_processEventRiderStats");
    void processRiderRanking(int rider, int result, int score) __asm__("cBEScoreInterface_processRiderRanking");
    void* getPlayerLastCompletedEventStats(int profileIndex)
        __asm__("cBEScoreInterface_getPlayerLastCompletedEventStats");
    void* getPlayerBestStats(int profileIndex, int characterId) __asm__("cBEScoreInterface_getPlayerBestStats");
    bool isBetterMedal(int currentMedal, int newMedal) __asm__("cBEScoreInterface_isBetterMedal");
    bool isEligibleForHighScore(int rider) __asm__("cBEScoreInterface_isEligibleForHighScore");
    int getNumberOfWinsOnTrack(int rider, int locationID, int eventID)
        __asm__("cBEScoreInterface_getNumberOfWinsOnTrack");
    int getBestMedalOnTrack(int rider, int locationID, int eventID) __asm__("cBEScoreInterface_getBestMedalOnTrack");
    void setBestMedalOnTrack(int rider, int locationID, int eventID, int medal)
        __asm__("cBEScoreInterface_setBestMedalOnTrack");
};

CHECK_OFFSET(cBEScoreInterface, bNewHighScore, 0x10);
CHECK_OFFSET(cBEScoreInterface, mNewTrackHighScoreIndex, 0x18);
CHECK_OFFSET(cBEScoreInterface, LastEventStats, 0x20);
CHECK_SIZE(cBEScoreInterface, 0x230);

// The one score interface (On Tour: cBEScoreInterface::instance), created by cBEScoreInterface_getThis.
extern cBEScoreInterface* cBEScoreInterface_instance;

#endif /* BE_BEINTSCORE_H */
