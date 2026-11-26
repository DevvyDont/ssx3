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

// The reward kinds (names ours): the reward file's tables and cBERewardDB's members in this order, the rewardType of
// cBEUserAwards::setAward ([O] On Tour's int rewardType; its mpAwardBits has 10 entries) and the kind the reward
// interface's per-kind dispatchers take. [U] cBERewardDB_initRewards parses tables 0 trophies, 1 medals, 2 peak passes,
// 3 posters, 4 trading cards, 5 concept arts, 6 videos, 7 toys, 8 cheat characters; cBERewardDB_getRewardName returns
// nothing for 9; cBEUserAwards_setAward logs 9 as the bolt item (iCharBoltID); cBuyPopupInfo_initBuyReward labels 3 ..
// 8 "kT_OVRCMNPosterColon", "kT_OVRCMNCardColon", "kT_OVRCMNArtColon", "kT_OVRCMNVideoColon", "kT_OVRCMNToyColon" and
// "kT_OVRCMNCheatCharColon"; cFEStateRewardGalleryBase_showReward plays a kRewardVideo in a cFEStateVideoPlayer.
enum tRewardType {
    kRewardTrophy = 0,
    kRewardMedal = 1,
    kRewardPeakPass = 2,
    kRewardPoster = 3,
    kRewardCard = 4,
    kRewardArt = 5,
    kRewardVideo = 6,
    kRewardToy = 7,
    kRewardCheatChar = 8,
    kRewardBolt = 9,
    kRewardTypeNum = 10
};

// One cheat character's reward record (cBERewardDB::mpCheatChars[mNumCheatChars]). [O] struct tCheatCharInfo (0x10);
// [U] cFEStateCheatCharSelect_cFEStateCheatCharSelect walks mpCheatChars with a 0x10 stride: mCheatCharID (the byte at
// 0xF) goes to cBERewardInterface_isCheatCharOwned and mCharIDs, mpIconName is hashed into the texture bank,
// mNameStringID becomes the entry's name (fe/festatecharselect's view sCharEnt_2220).
struct tCheatCharInfo {
    char* mNameStringID;            // 0x000 [O]; [U] the character's name string id
    char* mpIconName;               // 0x004 [O]; [U] the icon element's name (GetHashValue32)
    char* mpHelpStringID;           // 0x008 [O]
    short mCost;                    // 0x00C [O]
    char mStoreNum;                 // 0x00E [O]
    signed char mCheatCharID;       // 0x00F [O] (char); [U] read with lb (the view's signed char id)
};

CHECK_OFFSET(tCheatCharInfo, mNameStringID, 0x000);
CHECK_OFFSET(tCheatCharInfo, mpIconName, 0x004);
CHECK_OFFSET(tCheatCharInfo, mpHelpStringID, 0x008);
CHECK_OFFSET(tCheatCharInfo, mCost, 0x00C);
CHECK_OFFSET(tCheatCharInfo, mStoreNum, 0x00E);
CHECK_OFFSET(tCheatCharInfo, mCheatCharID, 0x00F);
CHECK_SIZE(tCheatCharInfo, 0x10);

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

    // Methods bound to their retail symbols by asm labels.
    void purge() __asm__("cBERewardDB_purge");
    int getRewardName(int type, int idx) __asm__("cBERewardDB_getRewardName");
    void init(int async) __asm__("cBERewardDB_init");
    void update() __asm__("cBERewardDB_update");
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

// ---- The store-item records behind mpPosters / mpTradingCards / mpConceptArts / mpToys (views group D) ----
//
// Each is 0x10: [O] On Tour's tPosterInfo, tTradingCardInfo, tConceptArtInfo and tToyInfo are four identical structs
// {char* mNameStringID; char* mpTextureFileName; char* mpIconName; short mCost; char mStoreNum; char _mPad;} with
// getName / getTextureFileName / getIconName; [R] retail's buy functions (cBERewardInterface buy* /
// give*, be/beintreward) index the tables with a 0x10 stride and read the signed short at +0xC (the price).

struct tPosterInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
    short mCost;                    // 0xC [O]; [U] the buy / give function charges it (the posters (sRewardDef_157FD0))
    char mStoreNum;                 // 0xE [O]
    char _mPad;                     // 0xF [O]
};

CHECK_OFFSET(tPosterInfo, mNameStringID, 0x0);
CHECK_OFFSET(tPosterInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tPosterInfo, mpIconName, 0x8);
CHECK_OFFSET(tPosterInfo, mCost, 0xC);
CHECK_OFFSET(tPosterInfo, mStoreNum, 0xE);
CHECK_OFFSET(tPosterInfo, _mPad, 0xF);
CHECK_SIZE(tPosterInfo, 0x10);

struct tTradingCardInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
    short mCost;                    // 0xC [O]; [U] the buy / give function charges it (the trading cards (sRewardDef_1580F8))
    char mStoreNum;                 // 0xE [O]
    char _mPad;                     // 0xF [O]
};

CHECK_OFFSET(tTradingCardInfo, mNameStringID, 0x0);
CHECK_OFFSET(tTradingCardInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tTradingCardInfo, mpIconName, 0x8);
CHECK_OFFSET(tTradingCardInfo, mCost, 0xC);
CHECK_OFFSET(tTradingCardInfo, mStoreNum, 0xE);
CHECK_OFFSET(tTradingCardInfo, _mPad, 0xF);
CHECK_SIZE(tTradingCardInfo, 0x10);

struct tConceptArtInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
    short mCost;                    // 0xC [O]; [U] the buy / give function charges it (the concept art (sRewardDef_158220))
    char mStoreNum;                 // 0xE [O]
    char _mPad;                     // 0xF [O]
};

CHECK_OFFSET(tConceptArtInfo, mNameStringID, 0x0);
CHECK_OFFSET(tConceptArtInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tConceptArtInfo, mpIconName, 0x8);
CHECK_OFFSET(tConceptArtInfo, mCost, 0xC);
CHECK_OFFSET(tConceptArtInfo, mStoreNum, 0xE);
CHECK_OFFSET(tConceptArtInfo, _mPad, 0xF);
CHECK_SIZE(tConceptArtInfo, 0x10);

struct tToyInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
    short mCost;                    // 0xC [O]; [U] the buy / give function charges it (the character toys (sRewardDef_158348))
    char mStoreNum;                 // 0xE [O]
    char _mPad;                     // 0xF [O]
};

CHECK_OFFSET(tToyInfo, mNameStringID, 0x0);
CHECK_OFFSET(tToyInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tToyInfo, mpIconName, 0x8);
CHECK_OFFSET(tToyInfo, mCost, 0xC);
CHECK_OFFSET(tToyInfo, mStoreNum, 0xE);
CHECK_OFFSET(tToyInfo, _mPad, 0xF);
CHECK_SIZE(tToyInfo, 0x10);

// One peak pass reward record (cBERewardDB::mpPeakPasses[mNumPeakPasses], 3 per character). [O] struct tPeakPassInfo
// (0x8); [R] cBERewardDB_getRewardName (type 2) and cBERewardInterface_getPeakPass index the table with an 8-byte
// stride (getRewardName returns the first word, the name).
struct tPeakPassInfo {
    char* mNameStringID;            // 0x0 [O]; [U] cBERewardDB_getRewardName (type 2) returns it
    char* mpTextureFileName;        // 0x4 [O]; [U] cFEStateMountainRoom_loadPeakPass loads this file (func_0019DA10's
                                    //     file name) for cFEStateMountainRoom::mpPeakPassInfo
};

CHECK_OFFSET(tPeakPassInfo, mNameStringID, 0x0);
CHECK_OFFSET(tPeakPassInfo, mpTextureFileName, 0x4);
CHECK_SIZE(tPeakPassInfo, 0x8);

// One video reward record (cBERewardDB::mpVideos[mNumVideos]). [O] struct tVideoInfo (0x10), the store-item shape
// above with the video file in place of the texture; [R] cBERewardDB_getRewardName (type 6) and
// cBERewardInterface_buyVideo index the table with a 0x10 stride.
struct tVideoInfo {
    char* mNameStringID;            // 0x0 [O]; [U] cBERewardDB_getRewardName (type 6) returns it
    char* mpVideoFileName;          // 0x4 [O]; [U] cFEStateRewardGalleryBase_showReward hands it to
                                    //     cFEStateVideoPlayer_setVideoSourceFiles
    char* mpIconName;               // 0x8 [O]; [U] cFEStateRewardGalleryVideos_getIconID copies its first 4 chars
    short mCost;                    // 0xC [O]; [U] cBERewardInterface_buyVideo charges it (lh 0xC)
    char mStoreNum;                 // 0xE [O]
    char _mPad;                     // 0xF [O]
};

CHECK_OFFSET(tVideoInfo, mNameStringID, 0x0);
CHECK_OFFSET(tVideoInfo, mpVideoFileName, 0x4);
CHECK_OFFSET(tVideoInfo, mpIconName, 0x8);
CHECK_OFFSET(tVideoInfo, mCost, 0xC);
CHECK_OFFSET(tVideoInfo, mStoreNum, 0xE);
CHECK_OFFSET(tVideoInfo, _mPad, 0xF);
CHECK_SIZE(tVideoInfo, 0x10);


// ---- The trophy / medal / peak pass records behind mpTrophies / mpMedals / mpPeakPasses (types round 3) ----
//
// [O] On Tour's tTrophyInfo and tMedalInfo are {char* mNameStringID; char* mpTextureFileName; char* mpIconName;}
// (0xC, with getName / getTextureFileName / getIconName) and tPeakPassInfo is {char* mNameStringID;
// char* mpTextureFileName;} (0x8, getName / getTextureFileName; defined above); cBERewardInterface::getTrophy / getMedal /
// getPeakPass return const tTrophyInfo* / tMedalInfo* / tPeakPassInfo*. [R] retail's cBERewardInterface_getTrophy /
// getMedal / getPeakPass (be/beintreward) index the tables with a 0xC / 0xC / 0x8 stride.

struct tTrophyInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
};

CHECK_OFFSET(tTrophyInfo, mNameStringID, 0x0);
CHECK_OFFSET(tTrophyInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tTrophyInfo, mpIconName, 0x8);
CHECK_SIZE(tTrophyInfo, 0xC);

struct tMedalInfo {
    char* mNameStringID;            // 0x0 [O]
    char* mpTextureFileName;        // 0x4 [O]
    char* mpIconName;               // 0x8 [O]
};

CHECK_OFFSET(tMedalInfo, mNameStringID, 0x0);
CHECK_OFFSET(tMedalInfo, mpTextureFileName, 0x4);
CHECK_OFFSET(tMedalInfo, mpIconName, 0x8);
CHECK_SIZE(tMedalInfo, 0xC);

#endif
