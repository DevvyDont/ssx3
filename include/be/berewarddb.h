#ifndef BE_BEREWARDDB_H
#define BE_BEREWARDDB_H

// cBERewardDB: the rewards database (be/bedbreward, be/beintreward): the loaded rewards file and, per reward kind
// (trophies, medals, peak passes, posters, trading cards, concept art, toys, videos, cheat characters), a pointer to
// its records and their count, plus the string table. Methods: init, initRewards, getRewardName, update, purge.
//
// Size 0x54 (On Tour's; retail's methods reach 0x50, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"

struct tCheatCharInfo;
struct tConceptArtInfo;
struct tMedalInfo;
struct tPeakPassInfo;
struct tPosterInfo;
struct tToyInfo;
struct tTradingCardInfo;
struct tTrophyInfo;
struct tVideoInfo;

struct cBERewardDB {
    int mRewardFileIndex;             // 0x000 [O]
    char* mpRewardsFile;              // 0x004 [O]
    tTrophyInfo* mpTrophies;          // 0x008 [O]
    tMedalInfo* mpMedals;             // 0x00C [O]
    tPeakPassInfo* mpPeakPasses;      // 0x010 [O]
    tPosterInfo* mpPosters;           // 0x014 [O]
    tTradingCardInfo* mpTradingCards; // 0x018 [O]
    tConceptArtInfo* mpConceptArts;   // 0x01C [O]
    tToyInfo* mpToys;                 // 0x020 [O]
    tVideoInfo* mpVideos;             // 0x024 [O]
    tCheatCharInfo* mpCheatChars;     // 0x028 [O]
    char* mpStrings;                  // 0x02C [O]
    int mNumTrophies;                 // 0x030 [O]
    int mNumMedals;                   // 0x034 [O]
    int mNumPeakPasses;               // 0x038 [O]
    int mNumPosters;                  // 0x03C [O]
    int mNumTradingCards;             // 0x040 [O]
    int mNumConceptArts;              // 0x044 [O]
    int mNumToys;                     // 0x048 [O]
    int mNumVideos;                   // 0x04C [O]
    int mNumCheatChars;               // 0x050 [O]
};

CHECK_OFFSET(cBERewardDB, mRewardFileIndex, 0x000);
CHECK_OFFSET(cBERewardDB, mpRewardsFile, 0x004);
CHECK_OFFSET(cBERewardDB, mpTrophies, 0x008);
CHECK_OFFSET(cBERewardDB, mpMedals, 0x00C);
CHECK_OFFSET(cBERewardDB, mpPeakPasses, 0x010);
CHECK_OFFSET(cBERewardDB, mpPosters, 0x014);
CHECK_OFFSET(cBERewardDB, mpTradingCards, 0x018);
CHECK_OFFSET(cBERewardDB, mpConceptArts, 0x01C);
CHECK_OFFSET(cBERewardDB, mpToys, 0x020);
CHECK_OFFSET(cBERewardDB, mpVideos, 0x024);
CHECK_OFFSET(cBERewardDB, mpCheatChars, 0x028);
CHECK_OFFSET(cBERewardDB, mpStrings, 0x02C);
CHECK_OFFSET(cBERewardDB, mNumTrophies, 0x030);
CHECK_OFFSET(cBERewardDB, mNumMedals, 0x034);
CHECK_OFFSET(cBERewardDB, mNumPeakPasses, 0x038);
CHECK_OFFSET(cBERewardDB, mNumPosters, 0x03C);
CHECK_OFFSET(cBERewardDB, mNumTradingCards, 0x040);
CHECK_OFFSET(cBERewardDB, mNumConceptArts, 0x044);
CHECK_OFFSET(cBERewardDB, mNumToys, 0x048);
CHECK_OFFSET(cBERewardDB, mNumVideos, 0x04C);
CHECK_OFFSET(cBERewardDB, mNumCheatChars, 0x050);
CHECK_SIZE(cBERewardDB, 0x54);

#endif
