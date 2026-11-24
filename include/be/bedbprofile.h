#ifndef BE_BEDBPROFILE_H
#define BE_BEDBPROFILE_H

// cBECharProfileDB: one character's career record in a player profile - collectibles per location, mission flags,
// peak locks, difficulty, the character's bolt-ons ("Profile Bolts Map"), money, visited locations, best scores and
// medals, attributes, rider stats, messages, rewards and radio mode. The back end keeps them in the static array
// cBELibrary_profileLib (0x004A6CA8): [3 profiles][10 characters]; cBEProfileLib_BackupCharProfile is one more (destroyed separately);
// belibrary.cpp copies records to / from D_004B07F8. EA's source: d:/ssx/code/source/be/bedbprofile.cpp / .h (preview assert paths). Retail code: the decomp's
// units be/beintecon.cpp (destructor, init, the bolt-on helpers func_001513B8 .. func_00152368) and be/bedbprofile.cpp
// (getScoreStats .. updateMedalStats); most readers index cBELibrary_profileLib directly (be/beint*.cpp, belibrary.cpp).
// Size 0xF88: [R] the static initialiser __sti__all_in_one_be_cpp (be/bedbreward) constructs and destroys the array
// with a 0xF88 stride (cBELibrary_profileLib[3][10], 0x9B50 per profile; the inlined ctor sets the 525 bolt slots at
// +0x290 to {-1, 0} and clears +0x288) and calls cBECharProfileDB__cBECharProfileDB on each at exit; [U] every reader
// multiplies the character by 0xF88 (3976).
// The 64-bit `unsigned long` bitfield at 0x278 (cBECharProfileDB_init does ld/sd on it) makes the struct 8-aligned:
// 0xF84 bytes of members round up to 0xF88. No base classes, no vtable.
//
// A FLAT layout, like main/game.h. Tags: [R] cBECharProfileDB_init (sets or clears nearly every member), the destructor,
// the static initialiser; [U] how retail code uses the field; [O] SSX On Tour's DWARF (class cBECharProfileDB, size
// 0xF98, and its member types): member names and, where they agree, order. On Tour adds strProfileName, mHighScores,
// leaderBoard, the series records and more, and stores bools in 1 byte, so its offsets are not used.
// No preview evidence (no accessors survive; layout_xlate has no observations).

#include "layout.h"

// [O] struct tBEProfileCollectBits (0xC): one location's collectibles.
struct tBEProfileCollectBits {
    signed char cTotalCollected;    // 0x000 [O]; [R] init clears it; [U] beintmission sums it per peak / stores it
    char unk_001[0x3];              // 0x001 padding
    unsigned int uiCollectedBits[2]; // 0x004 [O]; [R] init clears 8 bytes
};
CHECK_OFFSET(tBEProfileCollectBits, cTotalCollected, 0x0);
CHECK_OFFSET(tBEProfileCollectBits, unk_001, 0x1);
CHECK_OFFSET(tBEProfileCollectBits, uiCollectedBits, 0x4);
CHECK_SIZE(tBEProfileCollectBits, 0xC);

// One bolt-on (equipment item) slot. No On Tour counterpart; the type name follows the "Profile Bolts Map" tag.
struct tBEProfileBolt {
    short id;                       // 0x000 [R] the ctor: -1 (empty); [U] func_00151A88 copies the item definition's id
    unsigned short flags;           // 0x002 [R] the ctor: 0; [U] 2, 4, 0x10 owned, 0x20 (func_00151C48 / C90 / BD0)
    tBEProfileBolt() : id(-1), flags(0) {}
};
CHECK_OFFSET(tBEProfileBolt, id, 0x0);
CHECK_OFFSET(tBEProfileBolt, flags, 0x2);
CHECK_SIZE(tBEProfileBolt, 0x4);

// [O] struct tBETrackScore (8): one location / event best.
struct tBETrackScore {
    char hasPlayed;                 // 0x000 [O]; [R] init: 0
    signed char BestMedal;          // 0x001 [O]; [R] init: -1
    unsigned short iNumGoldOrPlat;  // 0x002 [O]; [R] init: 0
    unsigned int uiHighScore;       // 0x004 [O]; [U] updateScoreStats compares and stores the new value
};
CHECK_OFFSET(tBETrackScore, hasPlayed, 0x0);
CHECK_OFFSET(tBETrackScore, BestMedal, 0x1);
CHECK_OFFSET(tBETrackScore, iNumGoldOrPlat, 0x2);
CHECK_OFFSET(tBETrackScore, uiHighScore, 0x4);
CHECK_SIZE(tBETrackScore, 0x8);

// [O] struct tBEPeakMedals (8).
struct tBEPeakMedals {
    unsigned int uiNumRaceMedals;   // 0x000 [O]; [U] the word at +0xBA0 + peak * 8 (beintplayer / updateMedalStats)
    unsigned int uiNumFreeStyleMedals; // 0x004 [O]; [U] the word at +0xBA4 + peak * 8
};
CHECK_OFFSET(tBEPeakMedals, uiNumRaceMedals, 0x0);
CHECK_OFFSET(tBEPeakMedals, uiNumFreeStyleMedals, 0x4);
CHECK_SIZE(tBEPeakMedals, 0x8);

// [O] struct tBERelationDB (3): how this character feels about another one (cBEAggressionInterface).
struct tBERelationDB {
    signed char aggState;           // 0x000 [O]; [U] func_001519E0: 0 friend ... 3 enemy
    signed char aggLevel;           // 0x001 [O]; [U] func_001519E0: 0 / 2
    signed char tick;               // 0x002 [O]
};
CHECK_OFFSET(tBERelationDB, aggState, 0x0);
CHECK_OFFSET(tBERelationDB, aggLevel, 0x1);
CHECK_OFFSET(tBERelationDB, tick, 0x2);
CHECK_SIZE(tBERelationDB, 0x3);

// [O] struct tBEStatDB (7): the character's attribute levels.
struct tBEStatDB {
    signed char cSpeed;             // 0x000 [O]
    signed char cAccel;             // 0x001 [O]
    signed char cTrick;             // 0x002 [O]
    signed char cEdging;            // 0x003 [O]
    signed char cSpin;              // 0x004 [O]
    signed char cToughness;         // 0x005 [O]
    signed char cStability;         // 0x006 [O]
};
CHECK_OFFSET(tBEStatDB, cSpeed, 0x0);
CHECK_OFFSET(tBEStatDB, cStability, 0x6);
CHECK_SIZE(tBEStatDB, 0x7);

// [O] struct tBEStatDefault (0xF): a character's default attributes (On Tour global cBELibrary::statDefault, one per
// character). [R] retail cBELibrary_statDefault is a table of 15-byte records indexed by character id; the stat
// interface's struct-returning getters (be/beintplayer func_001482D0 / func_00148328) copy one record out whole, and
// the braking / edging attribute divides by byte 0xB (cMaxEdging, lb: signed like tBEStatDB). Views group D:
// sCharStat_001482D0, sCharStat_00148328 (and the unit's other 15-byte views of the same table).
struct tBEStatDefault {
    signed char eCharacterID;       // 0x000 [O]
    tBEStatDB start;                // 0x001 [O] the starting attribute levels
    signed char cMaxSpeed;          // 0x008 [O]
    signed char cMaxAccel;          // 0x009 [O]
    signed char cMaxTrick;          // 0x00A [O]
    signed char cMaxEdging;         // 0x00B [O]; [U] the braking / edging attribute's divisor
    signed char cMaxSpin;           // 0x00C [O]
    signed char cMaxToughness;      // 0x00D [O]
    signed char cMaxStability;      // 0x00E [O]
};
CHECK_OFFSET(tBEStatDefault, start, 0x1);
CHECK_OFFSET(tBEStatDefault, cMaxSpeed, 0x8);
CHECK_OFFSET(tBEStatDefault, cMaxEdging, 0xB);
CHECK_OFFSET(tBEStatDefault, cMaxStability, 0xE);
CHECK_SIZE(tBEStatDefault, 0xF);

// [O] struct tRiderBest (0x54): a character's best results, the record cBEScoreInterface::getPlayerBestStats(int
// iProfileIndex, eBECharacters iChar) returns by const reference. [R] cBECharProfileDB::mRiderBest (0xDE4, 0x54 bytes,
// cleared by init) is what retail's getter cBEScoreInterface_getPlayerBestStats returns; [U] cFEStateCareerStats_setupRidersBest copies
// the whole record and reads eventStat[k] as {best value, location id} pairs (knockouts 0, handplants 5, ...).
// On Tour's element type is an 8-byte tStat variant whose member names are not in the dump (its 0x10-byte tStat
// {m_num, m_total, m_max, m_numUpdates} is a different variant), so the pair is kept as two words.
struct tRiderBest {
    int eventStat[10][2];           // 0x00 [O] tStat eventStat[10] (8 bytes each); [U] [k][0] the best value, [k][1]
                                    //      the location it was set at (cBENewRaceInterface_getLocationName)
    int comboLongestOverall;        // 0x50 [O]
};
CHECK_OFFSET(tRiderBest, eventStat, 0x00);
CHECK_OFFSET(tRiderBest, comboLongestOverall, 0x50);
CHECK_SIZE(tRiderBest, 0x54);

// The character's owned-reward bits (On Tour struct tRewardBits, 0x50; member names, types and offsets are On Tour's).
// Retail reaches every member at On Tour's offset: cBERewardInterface's own / mark / count functions pass the
// bPosters .. bCheatChars arrays (bits per reward id), buySong ORs a bit into iOwnedSongList, getOwnedSongList /
// getPlayList / setOwnedSongList / setPlayList read and write the two 64-bit words, and getNumOwnedSongs /
// getNumSongsInPlayList count 0x40 bits of them. bOwnedSongList / bPlayList: no retail access seen.
struct tRewardBits {
    char bPosters[6];               // 0x00 [O]
    char bTradingCards[15];         // 0x06 [O]
    char bConceptArts[13];          // 0x15 [O]
    char bVideos[1];                // 0x22 [O]
    char bToys[4];                  // 0x23 [O]
    char bCheatChars[3];            // 0x27 [O]
    char bOwnedSongList[8];         // 0x2A [O]
    char bPlayList[8];              // 0x32 [O]
    char pad_3A[0x6];               // 0x3A alignment of the 64-bit words
    unsigned long iOwnedSongList;   // 0x40 [O] (unsigned long long: ee-gcc's long is 64-bit)
    unsigned long iPlayList;        // 0x48 [O] (unsigned long long)
};

CHECK_OFFSET(tRewardBits, bPosters, 0x00);
CHECK_OFFSET(tRewardBits, bTradingCards, 0x06);
CHECK_OFFSET(tRewardBits, bConceptArts, 0x15);
CHECK_OFFSET(tRewardBits, bVideos, 0x22);
CHECK_OFFSET(tRewardBits, bToys, 0x23);
CHECK_OFFSET(tRewardBits, bCheatChars, 0x27);
CHECK_OFFSET(tRewardBits, bOwnedSongList, 0x2A);
CHECK_OFFSET(tRewardBits, bPlayList, 0x32);
CHECK_OFFSET(tRewardBits, iOwnedSongList, 0x40);
CHECK_OFFSET(tRewardBits, iPlayList, 0x48);
CHECK_SIZE(tRewardBits, 0x50);

struct cBECharProfileDB {
    int bFirstTimeWorldCircuit;     // 0x000 [O] (bool; a word) - first data member after On Tour's strProfileName;
                                    //       [R] init: 1
    tBEProfileCollectBits mCollectibles[23]; // 0x004 [O] mCollectibles[1][23]; [R] init clears 23; [U] beintmission's
                                    //       views (records of 12 bytes) read cTotalCollected per location
    unsigned int mMissions[88];     // 0x118 [O] mMissions (On Tour 52 x tBEMissionDB, 8 bytes); retail: one bitfield word
                                    //       per mission ([R] init: bit 0 = cBELibrary_missionInfo[i] +0x1A, bit 1 = 1,
                                    //       bits 2-3 = 0, bit 4 = +0x1B); [U] beintmission's sEconFlags_001544D0
    // [O] the 20 peak flags, in On Tour's order; [R] init clears bits 0-5 and sets bits 6-19 (ld/sd: a 64-bit unit)
    unsigned long bPeak1RivalPtsChallengePlayed : 1;  // 0x278 bit 0
    unsigned long bPeak2RivalPtsChallengePlayed : 1;  // bit 1
    unsigned long bPeak3RivalPtsChallengePlayed : 1;  // bit 2
    unsigned long bPeak1RivalTimeChallengePlayed : 1; // bit 3
    unsigned long bPeak2RivalTimeChallengePlayed : 1; // bit 4
    unsigned long bPeak3RivalTimeChallengePlayed : 1; // bit 5
    unsigned long bPeak1TimeRCLocked : 1;             // bit 6
    unsigned long bPeak2TimeRCLocked : 1;             // bit 7
    unsigned long bPeak3TimeRCLocked : 1;             // bit 8
    unsigned long bPeak1FreeRCLocked : 1;             // bit 9
    unsigned long bPeak2FreeRCLocked : 1;             // bit 10
    unsigned long bPeak3FreeRCLocked : 1;             // bit 11
    unsigned long bPeak2Locked : 1;                   // bit 12
    unsigned long bPeak3Locked : 1;                   // bit 13
    unsigned long bPeak1TCLocked : 1;                 // bit 14
    unsigned long bPeak2TCLocked : 1;                 // bit 15
    unsigned long bPeak3TCLocked : 1;                 // bit 16
    unsigned long bPeak1FCLocked : 1;                 // bit 17
    unsigned long bPeak2FCLocked : 1;                 // bit 18
    unsigned long bPeak3FCLocked : 1;                 // bit 19
    int locLastHub;                 // 0x27C [O] eBELocationID (after the flags in both games); [R] init: 17 (sw 0x27C)
    short mDifficulty;              // 0x280 [O]; [R] init: 1; [U] cBENewPlayerInterface_getDifficultyLevel(which != 0)
    short mDifference;              // 0x282 [O]; [R] init: 0; [U] cBENewPlayerInterface_getDifficultyDifference(flag != 0)
    short unk_284;                  // 0x284 [R] init: 1; [U] the second difficulty level (getDifficultyLevel(0))
    short unk_286;                  // 0x286 [R] init: 0; [U] the second difficulty difference (getDifficultyDifference(0))
    short* mpBoltsMap;              // 0x288 [R] the ctor zeroes it; func_00151A88 allocates it ("Profile Bolts Map",
                                    //       one short per item id: the slot in mBolts or -1); the destructor frees it
    int mNumBolts;                  // 0x28C [U] func_00151A88: the character's item count; func_00151C20 / func_001515B8
    tBEProfileBolt mBolts[525];     // 0x290 [R] the ctor sets 525 slots to {-1, 0}; [U] func_00151A88 refills them (the
                                    //       rest id -1); the bolt-on helpers
    int mMoney;                     // 0xAC4 [O]; [R] init: 0; [U] cBEEconInterface get/set/add/reduceMoney
    unsigned int mTotalMoneyEarned; // 0xAC8 [O]; [R] init: 0; [U] cBEEconInterface's total-earned getter
    unsigned int mVisitLocFlags;    // 0xACC [O]; [R] init: 0; [U] setLocationVisited / hasVisitedPeak (bit per location)
    tBETrackScore mScoreStats[26];  // 0xAD0 [O]; [R] init: 26 x {0, -1, 0, 0}; [U] getScoreStats / updateScoreStats
    tBEPeakMedals mPeakMainEventMedals[3]; // 0xBA0 [O]; [R] init clears 0x18 bytes; [U] updateMedalStats, beintplayer
    signed char mCareerHighlights[8]; // 0xBB8 [O]; [R] init clears 8; [U] the highlight levels (beintplayer)
    signed char unk_BC0;            // 0xBC0 [U] the character this record belongs to: func_00151A88 stores k; init passes it
                                    //       to func_001519E0 / func_00150558; the bolt-on helpers read it (19 loads)
    tBERelationDB mRelations[10];   // 0xBC1 [O] tBERelationDB; [U] func_001519E0 resets 10 (one per character);
                                    //       beintaggression's view sAggrCharProfile declares 16 (indexes stay < 10)
    tBEStatDB statDB;               // 0xBDF [O]; [R] init copies D_005308D9[k] (7 bytes); [U] "the seven attribute bytes"
    char unk_BE6[0x1FE];            // 0xBE6 [U] the BAGT data: func_00150558(character, self + 0xBE6) (On Tour keeps
                                    //       careerStatDB, uberV2Unlocked, unlockedBoards/Skis/Skins, ... in this area)
    tRiderBest mRiderBest;          // 0xDE4 [O] tRiderBest (0x54: tStat eventStat[10], int comboLongestOverall); [R] init
                                    //       clears 84 bytes; [U] beintplayer's best-stats getter returns +0xDE4
    int unk_E38[25][2];             // 0xE38 [R] init zeroes the first word of each pair; [U] func_00147908 (beintplayer)
                                    //       returns it as the character's message list
    unsigned int unk_F00;           // 0xF00 [R] init clears bits 0-24 (one per message)
    signed char unk_F04;            // 0xF04 [R] init: 0
    char unk_F05[0x3];              // 0xF05 padding
    unsigned int unk_F08[2];        // 0xF08 [R] init clears bits 0-60; [U] the message flags (func_00147980 / func_00147A30)
    char unk_F10[0x18];             // 0xF10 no retail access seen
    unsigned char mAwardsReceived[4]; // 0xF28 [O]; [R] init MEM_fills 4; [U] cBERewardInterface_isAwardReceived's bits
    char unk_F2C[0x4];              // 0xF2C no retail access seen (On Tour aligns tRewardBits to 0x7A0 the same way)
    tRewardBits mRewards;           // 0xF30 [O]; [R] init MEM_fills 80; [U] cBERewardInterface (see tRewardBits)
    int mRadioMode;                 // 0xF80 [O] eRadioModeID (after mRewards in both games); [R] init: 0; [U] the radio
                                    //       mode getter / setter
};
CHECK_OFFSET(cBECharProfileDB, bFirstTimeWorldCircuit, 0x0);
CHECK_OFFSET(cBECharProfileDB, mCollectibles, 0x4);
CHECK_OFFSET(cBECharProfileDB, mMissions, 0x118);
CHECK_OFFSET(cBECharProfileDB, locLastHub, 0x27C);
CHECK_OFFSET(cBECharProfileDB, mDifficulty, 0x280);
CHECK_OFFSET(cBECharProfileDB, mDifference, 0x282);
CHECK_OFFSET(cBECharProfileDB, unk_284, 0x284);
CHECK_OFFSET(cBECharProfileDB, unk_286, 0x286);
CHECK_OFFSET(cBECharProfileDB, mpBoltsMap, 0x288);
CHECK_OFFSET(cBECharProfileDB, mNumBolts, 0x28C);
CHECK_OFFSET(cBECharProfileDB, mBolts, 0x290);
CHECK_OFFSET(cBECharProfileDB, mMoney, 0xAC4);
CHECK_OFFSET(cBECharProfileDB, mTotalMoneyEarned, 0xAC8);
CHECK_OFFSET(cBECharProfileDB, mVisitLocFlags, 0xACC);
CHECK_OFFSET(cBECharProfileDB, mScoreStats, 0xAD0);
CHECK_OFFSET(cBECharProfileDB, mPeakMainEventMedals, 0xBA0);
CHECK_OFFSET(cBECharProfileDB, mCareerHighlights, 0xBB8);
CHECK_OFFSET(cBECharProfileDB, unk_BC0, 0xBC0);
CHECK_OFFSET(cBECharProfileDB, mRelations, 0xBC1);
CHECK_OFFSET(cBECharProfileDB, statDB, 0xBDF);
CHECK_OFFSET(cBECharProfileDB, unk_BE6, 0xBE6);
CHECK_OFFSET(cBECharProfileDB, mRiderBest, 0xDE4);
CHECK_OFFSET(cBECharProfileDB, unk_E38, 0xE38);
CHECK_OFFSET(cBECharProfileDB, unk_F00, 0xF00);
CHECK_OFFSET(cBECharProfileDB, unk_F04, 0xF04);
CHECK_OFFSET(cBECharProfileDB, unk_F05, 0xF05);
CHECK_OFFSET(cBECharProfileDB, unk_F08, 0xF08);
CHECK_OFFSET(cBECharProfileDB, unk_F10, 0xF10);
CHECK_OFFSET(cBECharProfileDB, mAwardsReceived, 0xF28);
CHECK_OFFSET(cBECharProfileDB, unk_F2C, 0xF2C);
CHECK_OFFSET(cBECharProfileDB, mRewards, 0xF30);
CHECK_OFFSET(cBECharProfileDB, mRadioMode, 0xF80);
CHECK_SIZE(cBECharProfileDB, 0xF88);

#endif /* BE_BEDBPROFILE_H */
