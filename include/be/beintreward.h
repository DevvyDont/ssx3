#ifndef BE_BEINTREWARD_H
#define BE_BEINTREWARD_H

// cBERewardInterface: the back end's rewards interface (trophies, medals, unlockables, songs). One instance, created
// on demand by cBERewardInterface_getThis (`cMemMan_alloc(0x18, "BERewardInt", 0, 0)`) into
// cBERewardInterface_instance; the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 13). EA's source was
// d:/ssx/code/source/be/beintreward.cpp (preview build assert path).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet.
// What the real class looked like: `class cBERewardInterface : cBEInterface` (On Tour). cBEInterface is the base of
// every BE interface (BEBoltInt, cBENewPlayerInterface, ...): three bools, then the vptr. With PS2 bools being 4 bytes
// that is 0x0/0x4/0x8 + vptr at 0xC = 0x10, which is the whole object in the July 2003 preview (getThis allocates 0x10
// there) and the size of the other BE interfaces in retail. Retail adds the last-event pair at 0x10; On Tour has the
// same two members first in the class (at 0x8 after its 8-byte base), followed by the mEventUnlockables /
// mEventAccomplishments arrays that retail does not have (On Tour size 0x298).
// cBERewardInterface overrides no virtual: its getThis stores the same vtable (cBEInterface_vtbl) as BEBoltInt's
// (cBEBoltInterface_getThis), whose slots are cBEInterface's own.
//
// Evidence tags:
//   [R] retail's own constructor (the inlined body in cBERewardInterface_getThis) and allocation tag;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBERewardInterface), member order and names.
// Methods use `self` only as a handle (they read the BE library globals), so the fields are reached mostly through
// cBERewardInterface_instance or the cBE_getInterface(.., 13) pointer.

#include "layout.h"
#include "be/beinterface.h"

struct cBERewardDB;

// A medal (cBERewardInterface::mLastEventMedal). Names [O] from On Tour's enum eBEMedalID (kMEDAL_NONE -1,
// kMEDAL_GOLD 0, kMEDAL_SILVER 1, kMEDAL_BRONZE 2); retail ranks a platinum medal first (name ours), so the others
// are one higher: [U] cOVState_RACE_FINISHOV_onCreateScreen / cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen show
// "platinum", "gold", "silver" and "bronze" for 0..3, and clearUserAwards stores kMEDAL_NONE.
enum eBEMedalID {
    kMEDAL_NONE = -1, kMEDAL_PLATINUM = 0, kMEDAL_GOLD = 1, kMEDAL_SILVER = 2, kMEDAL_BRONZE = 3
};

struct cBERewardInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bCanSubmit: (On Tour: a bool at 0x0); no retail access seen
    //   bDataChanged: (On Tour: a bool at 0x1); no retail access seen
    //   bIsActive: (On Tour: a bool at 0x2); [R] getThis sets 0; [U] set 0/1 on every BE interface together
    //       (cBERaceState_onExit/cBERaceState_onEnter,
    //       cBEPostRaceState_onExit/cBEPostRaceState_onEnter/cBEPreRaceState_onExit/cBEPreRaceState_onEnter)
    //   the vptr (0x0C): [R] getThis stores cBEInterface_vtbl (cBEInterface's vtable, shared with BEBoltInt)
    // cBERewardInterface
    unsigned int mLastEventCashReward; // 0x010 [O] (On Tour 0x008, first own member); [U] cBERewardInterface_calculateUserTrackMedalRewards stores the cash
                                     //       from cBEEconInterface_getCashReward (halved when the medal isn't better); clearUserAwards
                                     //       zeroes it; cOVStateManager_gotoOverlay/cOVStateManager_gotoNextOverlay test it != 0
    int mLastEventMedal;             // 0x014 [O] eBEMedalID (On Tour 0x00C); [U] cBERewardInterface_calculateUserTrackMedalRewards stores the event's medal;
                                     //       clearUserAwards sets -1 (no medal, as in isBetterMedal); compared with
                                     //       0..3 by cOVState_RACE_FINISHOV_onCreateScreen/cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen, ==0 by func_002A3860, read by
                                     //       cOVState_REWARDS_checkRewardList (asm)

    // Methods bound to their retail symbols by asm labels.
    void awardUser(int profile, int iCharacterID, int bit) __asm__("cBERewardInterface_awardUser");
    char* getTrophy(int peakId, int trophyId) __asm__("cBERewardInterface_getTrophy");
    char* getMedal(int trophyId, int medalId) __asm__("cBERewardInterface_getMedal");
    char* getPeakPass(int peakId, int charId) __asm__("cBERewardInterface_getPeakPass");
    int getTrophies() __asm__("cBERewardInterface_getTrophies");
    int getMedals() __asm__("cBERewardInterface_getMedals");
    int getPeakPasses() __asm__("cBERewardInterface_getPeakPasses");
    int getPosters() __asm__("cBERewardInterface_getPosters");
    int getTradingCards() __asm__("cBERewardInterface_getTradingCards");
    int getConceptArts() __asm__("cBERewardInterface_getConceptArts");
    int getToys() __asm__("cBERewardInterface_getToys");
    int getVideos() __asm__("cBERewardInterface_getVideos");
    int getRandomLockedRewardIndex(int profile, int characterId, int kind)
        __asm__("cBERewardInterface_getRandomLockedRewardIndex");
    int getTotalNumRewards(int kind) __asm__("cBERewardInterface_getTotalNumRewards");
    int countOwnedRewards(int count, signed char* bits) __asm__("cBERewardInterface_countOwnedRewards");
    void markRewardOwned(int bit, char* bits) __asm__("cBERewardInterface_markRewardOwned");
    int isRewardOwned(int bit, char* bits) __asm__("cBERewardInterface_isRewardOwned");
    int isRewardOwned1(int profileIndex, int characterId, int kind, int bit)
        __asm__("cBERewardInterface_isRewardOwned1");
    int getNumOwnedRewards(int profileIndex, int characterId, int kind)
        __asm__("cBERewardInterface_getNumOwnedRewards");
    int getNumOwnedPosters(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedPosters");
    int getNumOwnedTradingCards(int profileIndex, int characterId)
        __asm__("cBERewardInterface_getNumOwnedTradingCards");
    int getNumOwnedConceptArts(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedConceptArts");
    int getNumOwnedToys(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedToys");
    int getNumOwnedVideos(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedVideos");
    int getNumOwnedCheatChars(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedCheatChars");
    int isMountainConquered(int iProfileIndex, int iCharacterID) __asm__("cBERewardInterface_isMountainConquered");
    int isMountainCompleted(int iProfileIndex, int iCharacterID) __asm__("cBERewardInterface_isMountainCompleted");
    int isPeakConquered(int iProfileIndex, int iCharacterID, int k) __asm__("cBERewardInterface_isPeakConquered");
    int isPeakCompleted(int iProfileIndex, int iCharacterID, int peakID) __asm__("cBERewardInterface_isPeakCompleted");
    int isPeakLocked(int rider, int peak) __asm__("cBERewardInterface_isPeakLocked");
    int isPeakLocked1(int profileIndex, int characterId, int peak) __asm__("cBERewardInterface_isPeakLocked1");
    int isTrophyOwned(int iProfileIndex, int iCharacterID, int k, int i) __asm__("cBERewardInterface_isTrophyOwned");
    int isPosterOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isPosterOwned");
    int isTradingCardOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isTradingCardOwned");
    int isConceptArtOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isConceptArtOwned");
    int isToyOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isToyOwned");
    int isVideoOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isVideoOwned");
    int isCheatCharOwned(int profileIndex, int characterId, int bit) __asm__("cBERewardInterface_isCheatCharOwned");
    int buyReward(int profileIndex, int characterId, int kind, int bit, int give)
        __asm__("cBERewardInterface_buyReward");
    void buyPoster(int profileIndex, int characterId, int bit, int give) __asm__("cBERewardInterface_buyPoster");
    void buyTradingCard(int profileIndex, int characterId, int bit, int give)
        __asm__("cBERewardInterface_buyTradingCard");
    void buyConceptArt(int profileIndex, int characterId, int bit, int give)
        __asm__("cBERewardInterface_buyConceptArt");
    void buyToy(int profileIndex, int characterId, int bit, int give) __asm__("cBERewardInterface_buyToy");
    void buyVideo(int profile, int characterId, int idx, int award) __asm__("cBERewardInterface_buyVideo");
    void buySong(int profile, int characterId, int bit, int award) __asm__("cBERewardInterface_buySong");
    void buyCheatChar(int profile, int characterId, int idx, int award) __asm__("cBERewardInterface_buyCheatChar");
    int getRadioMode(int profileIndex, int characterId) __asm__("cBERewardInterface_getRadioMode");
    long getOwnedSongList(int profileIndex, int characterId) __asm__("cBERewardInterface_getOwnedSongList");
    long getPlayList(int profileIndex, int characterId) __asm__("cBERewardInterface_getPlayList");
    int getNumOwnedSongs(int profileIndex, int characterId) __asm__("cBERewardInterface_getNumOwnedSongs");
    int getNumSongsInPlayList(int iProfileIndex, int iCharacterID) __asm__("cBERewardInterface_getNumSongsInPlayList");
    void setRadioMode(int profileIndex, int characterId, int value) __asm__("cBERewardInterface_setRadioMode");
    void setOwnedSongList(int profileIndex, int characterId, long value) __asm__("cBERewardInterface_setOwnedSongList");
    void setPlayList(int profileIndex, int characterId, long value) __asm__("cBERewardInterface_setPlayList");
    int getCollectibleMedalTarget(int peakID, int medal) __asm__("cBERewardInterface_getCollectibleMedalTarget");
    int getChallengesMedalTarget(int peakID, int medal) __asm__("cBERewardInterface_getChallengesMedalTarget");
    int calculateCollectibleMedal(int idx, int numCollected) __asm__("cBERewardInterface_calculateCollectibleMedal");
    int getTrackHighScore(int profileIndex, int characterId, int event, int locationId)
        __asm__("cBERewardInterface_getTrackHighScore");
    int getCollectiblesMedal(int iProfileIndex, int iCharacterID, int kind)
        __asm__("cBERewardInterface_getCollectiblesMedal");
    int getBigChallengesMedal(int iProfileIndex, int iCharacterID, int kind)
        __asm__("cBERewardInterface_getBigChallengesMedal");
    int getEarningsMedal(int iProfileIndex, int iCharacterID, int medal) __asm__("cBERewardInterface_getEarningsMedal");
    int calculateBigChallengeMedal(int idx, int numCompleted) __asm__("cBERewardInterface_calculateBigChallengeMedal");
    int calculateEarningMedal(int idx, unsigned int totalCash) __asm__("cBERewardInterface_calculateEarningMedal");
    void* calculateNextEarningsLevel(int iProfileIndex, int iCharacterID)
        __asm__("cBERewardInterface_calculateNextEarningsLevel");
    int calculateNextCollectiblesLevel(int iProfileIndex, int iCharacterID, int kind)
        __asm__("cBERewardInterface_calculateNextCollectiblesLevel");
    int calculateNextBigChallengesLevel(int iProfileIndex, int iCharacterID, int kind)
        __asm__("cBERewardInterface_calculateNextBigChallengesLevel");
    void clearUserAwards() __asm__("cBERewardInterface_clearUserAwards");
    void awardUserPeakPass(int profile, int characterId, int row, int mode)
        __asm__("cBERewardInterface_awardUserPeakPass");
    int getAwardTrophyCategory(int iProfileIndex, int iCharacterID, int kind, int tier)
        __asm__("cBERewardInterface_getAwardTrophyCategory");
    void awardUserTrophy(int iProfileIndex, int iCharacterID, int kind, int tier)
        __asm__("cBERewardInterface_awardUserTrophy");
    void calculateUserTrackMedalRewards(int profile, int charID, int mode, int event, int medal)
        __asm__("cBERewardInterface_calculateUserTrackMedalRewards");
    void awardUserCash(int rider, int money) __asm__("cBERewardInterface_awardUserCash");
    void awardUserCash1(int profileIndex, int characterId, int amount) __asm__("cBERewardInterface_awardUserCash1");
    void awardUserCollectible(int rider, int k) __asm__("cBERewardInterface_awardUserCollectible");
    void awardUserBigChallenge(int rider, int k) __asm__("cBERewardInterface_awardUserBigChallenge");
    void awardUser1(int rider, int bit) __asm__("cBERewardInterface_awardUser1");
    int isAwardReceived(int iProfileIndex, int iCharacterID, int bit) __asm__("cBERewardInterface_isAwardReceived");
    int isBitSet(int bit, unsigned char* bits) __asm__("cBERewardInterface_isBitSet");
    void setBit(int bit, unsigned char* bits) __asm__("cBERewardInterface_setBit");
    int getTrackPeakID(int locationID) __asm__("cBERewardInterface_getTrackPeakID");
    int getTrackTrophyID(int eventID, int locationID) __asm__("cBERewardInterface_getTrackTrophyID");
    cBERewardDB* getDB() __asm__("cBERewardInterface_getDB");
};

CHECK_OFFSET(cBERewardInterface, mLastEventCashReward, 0x10);
CHECK_OFFSET(cBERewardInterface, mLastEventMedal, 0x14);
CHECK_SIZE(cBERewardInterface, 0x18);

// The one instance (On Tour: cBERewardInterface::instance), created by cBERewardInterface_getThis.
extern cBERewardInterface* cBERewardInterface_instance;

#endif /* BE_BEINTREWARD_H */
