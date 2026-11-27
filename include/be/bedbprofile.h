#ifndef BE_BEDBPROFILE_H
#define BE_BEDBPROFILE_H

// cBECharProfileDB: one character's career record in a player profile - collectibles per location, mission flags,
// peak locks, difficulty, the character's bolt-ons ("Profile Bolts Map"), money, visited locations, best scores and
// medals, attributes, rider stats, messages, rewards and radio mode. The back end keeps them in the static array
// cBELibrary_profileLib (0x004A6CA8): [3 profiles][10 characters]; cBEProfileLib_BackupCharProfile is one more (destroyed separately);
// belibrary.cpp copies records to / from D_004B07F8. EA's source: d:/ssx/code/source/be/bedbprofile.cpp / .h (preview assert paths). Retail code: the decomp's
// units be/beintecon.cpp (destructor, init, the bolt-on helpers cBECharProfileDB_applyStartingBolts .. cBECharProfileDB_hasBoltFlags) and be/bedbprofile.cpp
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
    short id;                       // 0x000 [R] the ctor: -1 (empty); [U] cBECharProfileDB_buildBoltList copies the item definition's id
    unsigned short flags;           // 0x002 [R] the ctor: 0; [U] 2, 4, 0x10 owned, 0x20 (cBECharProfileDB_setBoltBought / C90 / BD0)
    tBEProfileBolt() : id(-1), flags(0) {}
};
CHECK_OFFSET(tBEProfileBolt, id, 0x0);
CHECK_OFFSET(tBEProfileBolt, flags, 0x2);
CHECK_SIZE(tBEProfileBolt, 0x4);

// A bolt id that names no bolt (name ours): [R] tBEProfileBolt's constructor stores it in an empty slot; [U] a bolt
// definition's parent id is kNoBoltID at the top level (cBEBoltDB_buildChildTables, cBEBoltDB_getFirstChildBolt, cBEBoltDB_collectChildBolts, cBEBoltDB_isBoltAncestor);
// cBECharProfileDB_applyBoltRules's links use it for no target and no condition; cBERewardInterface_getRandomUnownedPeakBolt / cBERewardInterface_getRandomUnownedAwardBolt return it when no
// candidate is left, and cBERewardInterface_awardUser then logs no bolt.
enum { kNoBoltID = -1 };

// A bolt definition's group byte for a bolt in no group (name ours): [U] cBEBoltInterface_buildBoltModelFileName builds no file name for it and
// fills a name's '$' wildcards from the other bolts of the same group.
enum { kNoBoltGroup = -1 };

// One bolt-on (equipment item) definition in the back end's item database (name ours). The records are 0x38 bytes:
// [R] cBEBoltDB_getCharBolts returns a character's first one as the table's base + index * 0x38 (cBEBoltDB_getNumCharBolts counts them),
// and every walker steps 0x38. Laid out through the members the matched code reads; belibrary, beintecon,
// beintreward, feridermanager and lensfx still read the record through their own views.
struct tBEBoltDef {
    char unk_00[0x2];               // 0x00
    signed char storeNum;           // 0x02 [U] the store that sells it: cFEStateCharEquip's store lists an item only
                                    //      when this equals its mStoreNum (cFEStateCharEquip_isStoreCategoryEmpty, cFEStateCharEquip_buildItemList)
    char unk_03[0x1];               // 0x03
    short id;                       // 0x04 [U] the item id: the index into cBECharProfileDB::mpBoltsMap (cRider_collectWornParts,
                                    //      cRider_OnStartObjectUse / OnEndObjectUse, beintecon, beintreward)
    char unk_06[0x8];               // 0x06
    short price;                    // 0x0E [U] the price in tens of dollars (kBoltPriceBuiltIn below; the equip
                                    //      screen shows and charges it x 10 when positive)
    signed char modelPart;          // 0x10 [U] the rider model part it replaces: cRider_OnStartObjectUse hides it at
                                    //      LODs 0-3 and cRider_OnEndObjectUse shows it (< 0: none)
    char unk_11[0x3];               // 0x11
    char* name;                     // 0x14 [U] the item's name: the equip screen's rows (an ASCII string, or for a
                                    //      category the hashed string id), the buy popup's boltName ([O] On Tour
                                    //      cBuyPopupInfo::initBuyBolt(const char* boltName, ...)); the bolts file
                                    //      loader relocates it (cBEBoltDB_loadFile)
    const char* lodPartNames[4];    // 0x18 [U] cRider_parsePartsFromBuf: the MDF archive part per LOD (0: none)
    char unk_28[0x8];               // 0x28
    char* pictureName;              // 0x30 [U] the item's picture: cFEStateCharEquip_getIconID looks its icon up by it
                                    //      and startLargePreviewLoad builds the preview file name from it
    int flags;                      // 0x34 [U] 0x40 hidden in NIS sequences (cRider_OnStartObjectUse / OnEndObjectUse);
                                    //      0x100 / 0x200 / 0x400 the item kind (beintreward)
};
CHECK_OFFSET(tBEBoltDef, storeNum, 0x02);
CHECK_OFFSET(tBEBoltDef, id, 0x04);
CHECK_OFFSET(tBEBoltDef, price, 0x0E);
CHECK_OFFSET(tBEBoltDef, modelPart, 0x10);
CHECK_OFFSET(tBEBoltDef, name, 0x14);
CHECK_OFFSET(tBEBoltDef, lodPartNames, 0x18);
CHECK_OFFSET(tBEBoltDef, pictureName, 0x30);
CHECK_OFFSET(tBEBoltDef, flags, 0x34);
CHECK_SIZE(tBEBoltDef, 0x38);

// A bolt definition's price (the short at +0xE, in tens of dollars) for an item every character starts with (name
// ours): [U] cBECharProfileDB_applyStartingBolts marks the items priced 0 free (flag 2) and makes the ones priced kBoltPriceBuiltIn owned
// too.
enum { kBoltPriceBuiltIn = -1 };

// A character's attribute points (a tBEStatDB byte) at the bottom and top of the scale (names ours): [U]
// cBEEconInterface_sellAttribute refuses at kAttribPointsMin and cBEEconInterface_buyAttribute at kAttribPointsMax;
// the price table is indexed by points / 5 (11 levels of 5 points).
enum { kAttribPointsMin = 5, kAttribPointsMax = 55 };

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

// The score types: the index of a location / event pair's record in cBECharProfileDB::mScoreStats and of its table in
// cBELibrary_highScores (names ours). [U] cBELibrary_getScoreType maps the time and points challenges to 0 .. 5 and
// the other pairs through its map, and returns kNoScoreType (one past the 26 records) for a pair with none;
// getScoreStats, updateScoreStats and getHighScoreTable then have no record.
enum { kNumScoreTypes = 26, kNoScoreType = kNumScoreTypes };

// [O] struct tBEPeakMedals (8).
struct tBEPeakMedals {
    unsigned int uiNumRaceMedals;   // 0x000 [O]; [U] the word at +0xBA0 + peak * 8 (beintplayer / updateMedalStats)
    unsigned int uiNumFreeStyleMedals; // 0x004 [O]; [U] the word at +0xBA4 + peak * 8
};
CHECK_OFFSET(tBEPeakMedals, uiNumRaceMedals, 0x0);
CHECK_OFFSET(tBEPeakMedals, uiNumFreeStyleMedals, 0x4);
CHECK_SIZE(tBEPeakMedals, 0x8);

// A relation's state (tBERelationDB::aggState). [O] On Tour's enum eBEAggState, names and values; [U] cBECharProfileDB_resetRelations
// sets 0 for a friend .. 3 for an enemy; cBEAggressionInterface_applyAggression caps a friend at aggression level 2
// and turns a neutral rider pushed to level 4 into an enemy, regressAggression turns an enemy back to neutral at 0.
enum eBEAggState {
    kAGG_FRIEND = 0, kAGG_NEUTRAL = 1, kAGG_RIVAL = 2, kAGG_ENEMY = 3, kAGG_NUMSTATES = 4
};

// A contact between riders (cBEAggressionInterface::applyAggression's event). [O] On Tour's enum eBEAggBehaviors, names
// and values; [U] applyAggression adds 1, 2, 4 and 6 aggression points for them.
enum eBEAggBehaviors {
    kAGG_MILDCONTACT = 0, kAGG_MODCONTACT = 1, kAGG_HARDCONTACT = 2, kAGG_BERTUZZI = 3, kAGG_NUMBEHAVIORS = 4
};

// [O] struct tBERelationDB (3): how this character feels about another one (cBEAggressionInterface).
struct tBERelationDB {
    signed char aggState;           // 0x000 [O]; [U] cBECharProfileDB_resetRelations: 0 friend ... 3 enemy
    signed char aggLevel;           // 0x001 [O]; [U] cBECharProfileDB_resetRelations: 0 / 2
    signed char tick;               // 0x002 [O]
};
CHECK_OFFSET(tBERelationDB, aggState, 0x0);
CHECK_OFFSET(tBERelationDB, aggLevel, 0x1);
CHECK_OFFSET(tBERelationDB, tick, 0x2);
CHECK_SIZE(tBERelationDB, 0x3);

// An attribute, the index of its byte in tBEStatDB. [O] On Tour's enum eBEAttributes, names and values (kATTRIB_SPEED 0
// .. kATTRIB_STAB 6, kATTRIB_NUM 7), in tBEStatDB's member order; [U] cBEStatInterface_getStatFromProfile returns
// cSpeed .. cStability for 0 .. 6.
enum eBEAttributes {
    kATTRIB_SPEED = 0, kATTRIB_ACCEL = 1, kATTRIB_TRICK = 2, kATTRIB_EDGING = 3, kATTRIB_SPIN = 4, kATTRIB_TOUGH = 5,
    kATTRIB_STAB = 6, kATTRIB_NUM = 7
};

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
// interface's struct-returning getters (be/beintplayer cBEStatInterface_getStatDefault / cBEStatInterface_getRiderStatDefault) copy one record out whole, and
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
    // The ten best stats (eventStat's index). [O] On Tour's enum tRiderBest::eEventStat, names and values; [U]
    // cBEScoreInterface_processRiderBest fills 0 .. 7 from the rider stats On Tour names NumFrags, NumUberTricks,
    // NumSuperUberTricks, NumUberGrindTricks, LongestCombo, BiggestHandplantTime, BiggestRailDist and BiggestAirTime,
    // 8 with the shortest race time and 9 with the most freestyle points.
    enum eEventStat {
        keEventStat_KnockdownMax = 0, keEventStat_UbersMax = 1, keEventStat_SuperUbersMax = 2,
        keEventStat_UberGrindsMax = 3, keEventStat_ComboLongest = 4, keEventStat_HandplantSecsLongest = 5,
        keEventStat_RailLengthLongest = 6, keEventStat_AirSecsLongest = 7, keEventStat_RaceSecsMin = 8,
        keEventStat_FreeStylePointsMax = 9, keEventStat_Num = 10
    };

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

// One mission's saved state (On Tour struct tBEMissionFlags, 0x4: the five bits below, in this order; On Tour wraps it
// in tBEMissionDB {uiFlags; medal bits}, which retail does not have: its records are 4 bytes). [U] beintmission's
// isMission* / setIs* read and write bit n of cBECharProfileDB::mMissions[idx] (lw/sw 0x118 after an index-first
// addu, the setters masking with li -1 - (1 << n)), the codegen of a bitfield member of an array element.
struct tBEMissionFlags {
    unsigned int bIsLocked : 1;     // bit 0 [O]; [U] isMissionLocked / setIsLocked; [R] init from
                                    //       cBELibrary_missionInfo
    unsigned int bIsReady : 1;      // bit 1 [O]; [U] isMissionReady / setIsReady; [R] init 1
    unsigned int bIsAvailable : 1;  // bit 2 [O]; [U] isMissionAvailable / setIsAvailable; [R] init 0
    unsigned int bIsCompleted : 1;  // bit 3 [O]; [U] isMissionCompleted / setIsCompleted; [R] init 0
    unsigned int bIsShowIcon : 1;   // bit 4 [O]; [U] isMissionShowIcon / setIsShowIcon; [R] init from missionInfo
};

CHECK_SIZE(tBEMissionFlags, 0x4);

// A playable character (cBEPlayerDB::cCharacterID, the column of cBELibrary_profileLib[profile][10]). [O] On Tour's
// enum eBECharacters names the SSX 3 riders it keeps kCHAR_ALLEGRA, kCHAR_ELISE, kCHAR_KAORI, kCHAR_MAC, kCHAR_NATE,
// kCHAR_PSYMON and kCHAR_ZOE (with other values; MOBY, GRIFF and VIGGO are ours); [R] the values from retail's
// per-character file lists in this order: cBEBoltInterface_getBigIconFileName's icon files "data/char/mobytxp/big_moby_icons.ssh",
// "kaoritxp", "allegtxp", "mactxp", "zoetxp", "grifftxp", "elisetxp", "natetxp", "psymotxp", "viggotxp", and
// cSSXApp::mTextureBigFile ("data/char/mobytxp.big" .. "viggotxp.big"); [P] the preview asserts charID < kCHAR_NUM
// for cFEBoltFileManager::mFiles[10].
enum eBECharacters {
    kCHAR_MOBY = 0, kCHAR_KAORI = 1, kCHAR_ALLEGRA = 2, kCHAR_MAC = 3, kCHAR_ZOE = 4, kCHAR_GRIFF = 5, kCHAR_ELISE = 6,
    kCHAR_NATE = 7, kCHAR_PSYMON = 8, kCHAR_VIGGO = 9, kCHAR_NUM = 10
};

struct cBECharProfileDB {
    int bFirstTimeWorldCircuit;     // 0x000 [O] (bool; a word) - first data member after On Tour's strProfileName;
                                    //       [R] init: 1
    tBEProfileCollectBits mCollectibles[23]; // 0x004 [O] mCollectibles[1][23]; [R] init clears 23; [U] beintmission's
                                    //       views (records of 12 bytes) read cTotalCollected per location
    tBEMissionFlags mMissions[88];  // 0x118 [O] mMissions (On Tour 52 x tBEMissionDB, 8 bytes); retail: one bitfield
                                    //       word per mission ([R] init: bit 0 = cBELibrary_missionInfo[i] +0x1A,
                                    //       bit 1 = 1, bits 2-3 = 0, bit 4 = +0x1B; beintecon's view sEconFlags_1600);
                                    //       [U] beintmission's isMission* / setIs* (tBEMissionFlags)
    // [O] the 20 peak flags, in On Tour's order; [R] init clears bits 0-5 and sets bits 6-19 (ld/sd: a 64-bit unit
    // shared with locLastHub, the struct being 8-aligned). unsigned int, not unsigned long: retail's readers
    // (cBERewardInterface_isPeakLocked1, beintplayer's) truncate to 32 bits before the mask, as an int field does
    // ([U] ld; dsll; dsra32; andi 1), and the setters' ld/and/or/sd is the same for both types
    unsigned int bPeak1RivalPtsChallengePlayed : 1;   // 0x278 bit 0
    unsigned int bPeak2RivalPtsChallengePlayed : 1;   // bit 1
    unsigned int bPeak3RivalPtsChallengePlayed : 1;   // bit 2
    unsigned int bPeak1RivalTimeChallengePlayed : 1;  // bit 3
    unsigned int bPeak2RivalTimeChallengePlayed : 1;  // bit 4
    unsigned int bPeak3RivalTimeChallengePlayed : 1;  // bit 5
    unsigned int bPeak1TimeRCLocked : 1;              // bit 6
    unsigned int bPeak2TimeRCLocked : 1;              // bit 7
    unsigned int bPeak3TimeRCLocked : 1;              // bit 8
    unsigned int bPeak1FreeRCLocked : 1;              // bit 9
    unsigned int bPeak2FreeRCLocked : 1;              // bit 10
    unsigned int bPeak3FreeRCLocked : 1;              // bit 11
    unsigned int bPeak2Locked : 1;                    // bit 12
    unsigned int bPeak3Locked : 1;                    // bit 13
    unsigned int bPeak1TCLocked : 1;                  // bit 14
    unsigned int bPeak2TCLocked : 1;                  // bit 15
    unsigned int bPeak3TCLocked : 1;                  // bit 16
    unsigned int bPeak1FCLocked : 1;                  // bit 17
    unsigned int bPeak2FCLocked : 1;                  // bit 18
    unsigned int bPeak3FCLocked : 1;                  // bit 19
    int locLastHub;                 // 0x27C [O] eBELocationID (after the flags in both games); [R] init: 17 (sw 0x27C)
    short mDifficulty;              // 0x280 [O]; [R] init: 1; [U] cBENewPlayerInterface_getDifficultyLevel(which != 0)
    short mDifference;              // 0x282 [O]; [R] init: 0; [U] cBENewPlayerInterface_getDifficultyDifference(flag != 0)
    short mFreestyleDifficulty;     // 0x284 (name ours) [R] init: 1; [U] the second difficulty level
                                    //       (getDifficultyLevel(0)); the scored three-heat events (cGMBigAirHandler's
                                    //       raceFinish) move it, the timed races (cGMRaceHandler) move mDifficulty
    short mFreestyleDifference;     // 0x286 (name ours) [R] init: 0; [U] the second difficulty difference
                                    //       (getDifficultyDifference(0)), the scored events' win/loss streak
    short* mpBoltsMap;              // 0x288 [R] the ctor zeroes it; cBECharProfileDB_buildBoltList allocates it ("Profile Bolts Map",
                                    //       one short per item id: the slot in mBolts or -1); the destructor frees it
    int mNumBolts;                  // 0x28C [U] cBECharProfileDB_buildBoltList: the character's item count; cBECharProfileDB_getBoltIdAt / cBECharProfileDB_restoreVisibleFlags
    tBEProfileBolt mBolts[525];     // 0x290 [R] the ctor sets 525 slots to {-1, 0}; [U] cBECharProfileDB_buildBoltList refills them (the
                                    //       rest id -1); the bolt-on helpers
    int mMoney;                     // 0xAC4 [O]; [R] init: 0; [U] cBEEconInterface get/set/add/reduceMoney
    unsigned int mTotalMoneyEarned; // 0xAC8 [O]; [R] init: 0; [U] cBEEconInterface's total-earned getter
    unsigned int mVisitLocFlags;    // 0xACC [O]; [R] init: 0; [U] setLocationVisited / hasVisitedPeak (bit per location)
    tBETrackScore mScoreStats[26];  // 0xAD0 [O]; [R] init: 26 x {0, -1, 0, 0}; [U] getScoreStats / updateScoreStats
    tBEPeakMedals mPeakMainEventMedals[3]; // 0xBA0 [O]; [R] init clears 0x18 bytes; [U] updateMedalStats, beintplayer
    signed char mCareerHighlights[8]; // 0xBB8 [O]; [R] init clears 8; [U] the highlight levels (beintplayer)
    signed char mCharacterID;       // 0xBC0 (name ours) [U] the character this record belongs to: cBECharProfileDB_buildBoltList stores
                                    //       k; init passes it to cBECharProfileDB_resetRelations / be_initStartingTricks; the bolt-on helpers
                                    //       read it (19 loads)
    tBERelationDB mRelations[10];   // 0xBC1 [O] tBERelationDB; [U] cBECharProfileDB_resetRelations resets 10 (one per character);
                                    //       beintaggression's view sAggrCharProfile declares 16 (indexes stay < 10)
    tBEStatDB statDB;               // 0xBDF [O]; [R] init copies D_005308D9[k] (7 bytes); [U] "the seven attribute bytes"
    char unk_BE6[0x1FE];            // 0xBE6 [U] the BAGT data: be_initStartingTricks(character, self + 0xBE6) (On Tour keeps
                                    //       careerStatDB, uberV2Unlocked, unlockedBoards/Skis/Skins, ... in this area)
    tRiderBest mRiderBest;          // 0xDE4 [O] tRiderBest (0x54: tStat eventStat[10], int comboLongestOverall); [R] init
                                    //       clears 84 bytes; [U] beintplayer's best-stats getter returns +0xDE4
    int mMessages[25][2];           // 0xE38 (name ours) [R] init zeroes the first word of each pair; [U] cBENewPlayerInterface_getRiderMessages
                                    //       (beintplayer) returns it as the character's message list ({message id,
                                    //       variant}: fepopupmisc's view sPopupList)
    unsigned int mMessageReadBits;  // 0xF00 (name ours) [R] init clears bits 0-24 (one per message); [U] fepopupmisc
                                    //       (sPopupList::mask): queueMessage clears a new message's bit, the message
                                    //       center shows the unread icon while it is clear
    signed char mNumMessages;       // 0xF04 (name ours) [R] init: 0; [U] sPopupList::count (queueMessage appends)
    char unk_F05[0x3];              // 0xF05 padding
    unsigned int mMessageGroupFlags[2]; // 0xF08 (name ours) [R] init clears bits 0-60; [U] the message flags, one per
                                    //       message group (cBENewPlayerInterface_getMessageGroupFlag / cBENewPlayerInterface_setMessageGroupFlag; fepopupmisc's isMessageFlagSet)
    char unk_F10[0x18];             // 0xF10 no retail access seen
    unsigned char mAwardsReceived[4]; // 0xF28 [O]; [R] init MEM_fills 4; [U] cBERewardInterface_isAwardReceived's bits
    char unk_F2C[0x4];              // 0xF2C no retail access seen (On Tour aligns tRewardBits to 0x7A0 the same way)
    tRewardBits mRewards;           // 0xF30 [O]; [R] init MEM_fills 80; [U] cBERewardInterface (see tRewardBits)
    int mRadioMode;                 // 0xF80 [O] eRadioModeID (after mRewards in both games); [R] init: 0; [U] the radio
                                    //       mode getter / setter

    // Methods bound to their retail symbols by asm labels.
    bool hasVisitedPeak(int kind) __asm__("cBECharProfileDB_hasVisitedPeak");
    void setLocationVisited(int bit, int on) __asm__("cBECharProfileDB_setLocationVisited");
    bool updateScoreStats(int locationId, int stat, unsigned int value) __asm__("cBECharProfileDB_updateScoreStats");
    void* getScoreStats(int locID, int event) __asm__("cBECharProfileDB_getScoreStats__FPvii");
};
CHECK_OFFSET(cBECharProfileDB, bFirstTimeWorldCircuit, 0x0);
CHECK_OFFSET(cBECharProfileDB, mCollectibles, 0x4);
CHECK_OFFSET(cBECharProfileDB, mMissions, 0x118);
CHECK_OFFSET(cBECharProfileDB, locLastHub, 0x27C);
CHECK_OFFSET(cBECharProfileDB, mDifficulty, 0x280);
CHECK_OFFSET(cBECharProfileDB, mDifference, 0x282);
CHECK_OFFSET(cBECharProfileDB, mFreestyleDifficulty, 0x284);
CHECK_OFFSET(cBECharProfileDB, mFreestyleDifference, 0x286);
CHECK_OFFSET(cBECharProfileDB, mpBoltsMap, 0x288);
CHECK_OFFSET(cBECharProfileDB, mNumBolts, 0x28C);
CHECK_OFFSET(cBECharProfileDB, mBolts, 0x290);
CHECK_OFFSET(cBECharProfileDB, mMoney, 0xAC4);
CHECK_OFFSET(cBECharProfileDB, mTotalMoneyEarned, 0xAC8);
CHECK_OFFSET(cBECharProfileDB, mVisitLocFlags, 0xACC);
CHECK_OFFSET(cBECharProfileDB, mScoreStats, 0xAD0);
CHECK_OFFSET(cBECharProfileDB, mPeakMainEventMedals, 0xBA0);
CHECK_OFFSET(cBECharProfileDB, mCareerHighlights, 0xBB8);
CHECK_OFFSET(cBECharProfileDB, mCharacterID, 0xBC0);
CHECK_OFFSET(cBECharProfileDB, mRelations, 0xBC1);
CHECK_OFFSET(cBECharProfileDB, statDB, 0xBDF);
CHECK_OFFSET(cBECharProfileDB, unk_BE6, 0xBE6);
CHECK_OFFSET(cBECharProfileDB, mRiderBest, 0xDE4);
CHECK_OFFSET(cBECharProfileDB, mMessages, 0xE38);
CHECK_OFFSET(cBECharProfileDB, mMessageReadBits, 0xF00);
CHECK_OFFSET(cBECharProfileDB, mNumMessages, 0xF04);
CHECK_OFFSET(cBECharProfileDB, unk_F05, 0xF05);
CHECK_OFFSET(cBECharProfileDB, mMessageGroupFlags, 0xF08);
CHECK_OFFSET(cBECharProfileDB, unk_F10, 0xF10);
CHECK_OFFSET(cBECharProfileDB, mAwardsReceived, 0xF28);
CHECK_OFFSET(cBECharProfileDB, unk_F2C, 0xF2C);
CHECK_OFFSET(cBECharProfileDB, mRewards, 0xF30);
CHECK_OFFSET(cBECharProfileDB, mRadioMode, 0xF80);
CHECK_SIZE(cBECharProfileDB, 0xF88);

#endif /* BE_BEDBPROFILE_H */
