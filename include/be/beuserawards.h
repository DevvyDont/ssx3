#ifndef BE_BEUSERAWARDS_H
#define BE_BEUSERAWARDS_H

// cBEUserAwards: a player's earned awards (be/beintreward): the award data and a bit array per award kind. Methods:
// setAward, setBit / isBitSet, getAwardBits, getNumAwards / getTotalNumAwards, clear.
//
// Size 0x19C (On Tour's; retail's methods reach 0x194, inside mpAwardBits (0x174-0x19B)). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// through `this` in the methods falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"

// tUserAwardsData (0x174): the saved part of the awards log. [O] On Tour's struct tUserAwardsData has exactly these
// members at exactly these offsets; [R] cBEUserAwards_cBEUserAwards points mpAwardBits[0, 3, 4, 5, 7, 8, 2] at
// 0x146 / 0x147 / 0x14D / 0x15C / 0x169 / 0x16D / 0x170, the starts of the seven bit arrays below, and
// cBEUserAwards_clear zeroes the 0x174 bytes (MEM_fill) and stores iCharBoltID = -1.
struct tUserAwardsData {
    unsigned int iCash;             // 0x000 [O]; [R] clear zeroes it
    short iCharBoltID;              // 0x004 [O]; [R] clear stores -1 (sh); [U] setAward (reward type 9) stores the item id
    char iNumAwards[32][10];        // 0x006 [O] (On Tour char iNumAwards[10][32]: the same 320 bytes); [U] retail indexes
                                    //       [awardCategory][rewardType]: getNumAwards reads +0x6 + category * 10 + type (lb),
                                    //       setAward counts into it, getTotalNumAwards sums all 32 x 10
    unsigned char bTrophies[1];     // 0x146 [O]; [R] mpAwardBits[0] = bTrophies
    unsigned char bPosters[6];      // 0x147 [O]; [R] mpAwardBits[3] = bPosters
    unsigned char bTradingCards[15]; // 0x14D [O]; [R] mpAwardBits[4] = bTradingCards
    unsigned char bConceptArts[13]; // 0x15C [O]; [R] mpAwardBits[5] = bConceptArts
    unsigned char bToys[4];         // 0x169 [O]; [R] mpAwardBits[7] = bToys
    unsigned char bCheatChars[3];   // 0x16D [O]; [R] mpAwardBits[8] = bCheatChars
    unsigned char bPeakPasses[1];   // 0x170 [O]; [R] mpAwardBits[2] = bPeakPasses
    char unk_171[0x3];              // 0x171 padding (4-byte struct alignment)
};

CHECK_OFFSET(tUserAwardsData, iCash, 0x000);
CHECK_OFFSET(tUserAwardsData, iCharBoltID, 0x004);
CHECK_OFFSET(tUserAwardsData, iNumAwards, 0x006);
CHECK_OFFSET(tUserAwardsData, bTrophies, 0x146);
CHECK_OFFSET(tUserAwardsData, bPosters, 0x147);
CHECK_OFFSET(tUserAwardsData, bTradingCards, 0x14D);
CHECK_OFFSET(tUserAwardsData, bConceptArts, 0x15C);
CHECK_OFFSET(tUserAwardsData, bToys, 0x169);
CHECK_OFFSET(tUserAwardsData, bCheatChars, 0x16D);
CHECK_OFFSET(tUserAwardsData, bPeakPasses, 0x170);
CHECK_SIZE(tUserAwardsData, 0x174);

struct cBEUserAwards {
    tUserAwardsData mAwards;        // 0x000 [O] tUserAwardsData mAwards; [R] clear zeroes its 0x174 bytes
    unsigned char* mpAwardBits[10]; // 0x174 [O]; [R] the ctor zeroes all 10, then points 7 of them into mAwards (above);
                                    //       [U] getAwardBits returns mpAwardBits[type], setAward passes it to setBit

    // Methods bound to their retail symbols by asm labels.
    void clear() __asm__("cBEUserAwards_clear");
    int coutBits(int n, unsigned char* bits) __asm__("cBEUserAwards_coutBits");
    void setBit(int bit, unsigned char* bits) __asm__("cBEUserAwards_setBit");
    int isBitSet(int bit, unsigned char* bits) __asm__("cBEUserAwards_isBitSet");
    void setAward(int row, int col, int value) __asm__("cBEUserAwards_setAward");
    signed char getNumAwards(int row, int column) __asm__("cBEUserAwards_getNumAwards");
    const unsigned char* getAwardBits(int i) __asm__("cBEUserAwards_getAwardBits__FPvi"); // [O] return type
    int getTotalNumAwards() __asm__("cBEUserAwards_getTotalNumAwards");
};

CHECK_OFFSET(cBEUserAwards, mAwards, 0x000);
CHECK_OFFSET(cBEUserAwards, mpAwardBits, 0x174);
CHECK_SIZE(cBEUserAwards, 0x19C);

#endif
