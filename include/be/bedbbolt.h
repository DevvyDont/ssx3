#ifndef BE_BEDBBOLT_H
#define BE_BEDBBOLT_H

// cBEBoltDB: the bolt-on (equipment item) database - every character's 0x38-byte bolt definitions (tBEBoltDef,
// be/bedbprofile.h) loaded from data/char/boltps2.dat, three more record tables from the same file (the 12-byte bolt
// rules, two 8-byte tables), and per-group (character) counts / first indexes and lookup tables built over them. Not
// allocated: the one instance is a static (cBELibrary_boltDB at 0x004A6750, likely On Tour-style `static cBEBoltDB
// boltDB` of cBELibrary), constructed (cBEBoltDB_cBEBoltDB) and destroyed (cBEBoltDB__cBEBoltDB, flags 2) by
// __sti__all_in_one_be_cpp (be/bedbreward). cBEBoltInterface_getBoltDB__FPv returns it;
// cBEBoltInterface_loadBoltDB__FPv / cBELibrary_loadData load it (cBEBoltDB_load),
// cBEBoltInterface_refreshVisibleBolts__FPv rebuilds it for the riders in play (cBEBoltDB_refreshVisibleBolts ->
// func_0014C7A0). EA's source: d:/ssx/code/source/be/bedbbolt.cpp and ../source/be/bedbbolt.h (July 2003 preview assert
// paths).
//
// Size 0x554: [R] cBEBoltDB_freeData clears exactly 0x0-0x553 (the pointers and counts at 0x0-0x28 one by one, then
// eleven 30-word tables from 0x2C, 11 * 0x78 = 0x528 bytes); cBEBoltDB_buildChildTables' last table entry is
// 0x4DC + 29 * 4 = 0x550; the next static, cBELibrary_profileLib, starts at cBELibrary_boltDB + 0x558 (8-byte
// alignment).
// No vtable (the destructor is called directly with a flags word), no base class.
// The arrays are per group = character id incl. the cheat characters: [P] the preview's group loops assert
// `charID >= 0 && charID < kCHEATCHAR_NUM` right after `sltiu ..., 30`.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] cBEBoltDB_loadFile (sets 0x0-0x28 from the file), cBEBoltDB_buildChildTables (0x18 and the three
//       short-table arrays), the ctor (clears 0x0-0x28), cBEBoltDB_freeData (frees 0x0 / 0x18, clears everything);
//   [P] the July 2003 preview (no symbols): its bedbbolt.cpp code (0x14A678 ...) has the same offsets as retail
//       (loadFile: 0x0-0x28; the group indexer: 0x1C, 0x2C, 0xA4) and its asserts name three members:
//       "index >= 0 && index < mTotalNumCharBolts" (after `lw v0, 28(s1)`), "mNumCharBolts[charID] < kMaxModelBolts"
//       (on the word at this + 0x2C + charID * 4, kMaxModelBolts = 400), "mpCharBolts[index].getCharID() == charID"
//       (after `lw v0, 4(s1)`); its rule lookup (the twin of retail cBEBoltDB_getBoltRules, table at +0x8) asserts
//       its result "pRules == NULL";
//   [U] how retail code uses the field. Names marked (ours) follow the preview's mpCharBolts / mNumCharBolts /
//       mTotalNumCharBolts pattern and the retail accessor names (getNumCharRules, getCharPairs, getCharGroups, ...).
// [O] SSX On Tour has no bolt database.

#include "layout.h"

struct tBEBoltDef;                   // be/bedbprofile.h (0x38 bytes)

// (name ours) A 12-byte rule record, sorted by group and then bolt id: [U] cBEBoltDB_indexRules counts them per group;
// cBEBoltDB_countBoltRules / findBoltRules / getBoltRules look a group's rules up by bolt id (and on / off state);
// beintecon reads the rest as a link (sEconLink_151EF0).
struct tBEBoltRule {
    signed char group;               // 0x0 [U] the group (character id)
    signed char on;                  // 0x1 [U] the on / off state the rule is for (cBEBoltDB_getBoltRules matches it)
    short boltId;                    // 0x2 [U] the bolt the rule is for: the lookups' key
    char unk_04[0x1];                // 0x4
    signed char condOn;              // 0x5 (ours) [U] cBECharProfileDB::applyBoltRules: the rule applies only when
                                     //     condBoltId's equipped state equals it
    short condBoltId;                // 0x6 (ours) [U] applyBoltRules: the condition bolt (kNoBoltID: none)
    char unk_08[0x1];                // 0x8
    signed char targetOn;            // 0x9 (ours) [U] applyBoltRules: the state the rule gives targetBoltId
    short targetBoltId;              // 0xA (ours) [U] applyBoltRules: the bolt the rule changes (kNoBoltID: none)
};

// (name ours) An 8-byte record: [U] cBEBoltDB_indexPairs counts them per group; cBEBoltDB_refreshVisibleBolts equips
// the bolt in bolts[1] when the one in bolts[0] is equipped (bolts[1] < 0: none).
struct tBEBoltPair {
    signed char group;               // 0x0 [U] the group (character id)
    char unk_01[0x3];                // 0x1
    short bolts[2];                  // 0x4 [U] the two bolt ids
};

// (name ours) An 8-byte record: [U] cBEBoltDB_indexGroups counts them per group; beintecon gives a character each
// group's prize bolt.
struct tBEBoltGroup {
    signed char group;               // 0x0 [U] the group (character id)
    char unk_01[0x3];                // 0x1
    short id;                        // 0x4 [U] beintecon's item group id
    short prize;                     // 0x6 [U] the bolt cBECharProfileDB_applyStartingBolts gives for the group
};

CHECK_OFFSET(tBEBoltRule, group, 0x0);
CHECK_OFFSET(tBEBoltRule, on, 0x1);
CHECK_OFFSET(tBEBoltRule, boltId, 0x2);
CHECK_OFFSET(tBEBoltRule, condOn, 0x5);
CHECK_OFFSET(tBEBoltRule, condBoltId, 0x6);
CHECK_OFFSET(tBEBoltRule, targetOn, 0x9);
CHECK_OFFSET(tBEBoltRule, targetBoltId, 0xA);
CHECK_SIZE(tBEBoltRule, 0xC);
CHECK_OFFSET(tBEBoltPair, group, 0x0);
CHECK_OFFSET(tBEBoltPair, bolts, 0x4);
CHECK_SIZE(tBEBoltPair, 0x8);
CHECK_OFFSET(tBEBoltGroup, group, 0x0);
CHECK_OFFSET(tBEBoltGroup, id, 0x4);
CHECK_OFFSET(tBEBoltGroup, prize, 0x6);
CHECK_SIZE(tBEBoltGroup, 0x8);

// The groups the per-group tables cover: [P] `charID < kCHEATCHAR_NUM` asserts on `sltiu ..., 30` (retail's value;
// On Tour's eBECheatCharacters ends at 82).
enum { kCHEATCHAR_NUM = 30 };

struct tBEBoltDef;

struct cBEBoltDB {
    char* mpBoltsFile;               // 0x000 (ours) [R] loadFile: = new char[BXFILE_sizez(file)] tagged
                                     //       "BoltsFile", the whole boltps2.dat; [U] freeData frees it; func_0014C7A0
                                     //       swaps in its compacted "Game Bolts" block (the riders' definitions, then
                                     //       the pairs and strings; also stored at 0x4)
    tBEBoltDef* mpCharBolts;         // 0x004 [P] assert "mpCharBolts[index].getCharID() == charID" on +0x4;
                                     //       [R] loadFile: = file + 8 (the definitions); [U] getCharBolts /
                                     //       findBoltIndex / collectChildBolts index it (0x38 stride); cBEBoltDB_load
                                     //       reads it as "was loaded before"; cFERider_*,
                                     //       cFEStateCharEquip_updateHeading, cBECharProfileDB_setBoltVisible /
                                     //       applyBoltRules read it via getBoltDB
    tBEBoltRule* mpCharRules;        // 0x008 (ours) [R] loadFile: the 12-byte records after the definitions; [P] the
                                     //       preview's rule lookup walks it into its `pRules`; [U] indexRules,
                                     //       countBoltRules, findBoltRules, getBoltRules (key short +0x2)
    tBEBoltPair* mpCharPairs;        // 0x00C (ours) [R] loadFile: the first 8-byte table; [U] indexPairs, getCharPairs;
                                     //       refreshVisibleBolts equips the bolt at +0x6 when the one at +0x4 is
    tBEBoltGroup* mpCharGroups;      // 0x010 (ours) [R] loadFile: the second 8-byte table; [U] indexGroups,
                                     //       getCharGroups (beintecon's item groups); cBEBoltDB_load skips loading when
                                     //       it is set
    char* mpStringPool;              // 0x014 (ours) [R] loadFile: the string pool after the tables, against which every
                                     //       definition's string offsets (+0x14, +0x18..+0x24, +0x28, +0x2C, +0x30) are
                                     //       relocated; [U] func_0014C7A0 re-points it into its block
    short* mpChildBoltsData;         // 0x018 (ours) [R] buildChildTables: = new short[kBolt_Num * 90] tagged
                                     //       "child bolts data", carved into the 90 tables at 0x11C / 0x194 / 0x4DC;
                                     //       [U] freeData frees it
    int mTotalNumCharBolts;          // 0x01C [P] assert "index < mTotalNumCharBolts" on +0x1C; [R] loadFile: the
                                     //       definition count; [U] indexCharBolts / getBoltDefIndex bound by it
    int mTotalNumCharRules;          // 0x020 (ours) [R] loadFile: mpCharRules' count; [U] indexRules
    int mTotalNumCharPairs;          // 0x024 (ours) [R] loadFile: mpCharPairs' count; [U] indexPairs, func_0014C7A0
    int mTotalNumCharGroups;         // 0x028 (ours) [R] loadFile: mpCharGroups' count; [U] indexGroups
    int mNumCharBolts[kCHEATCHAR_NUM];   // 0x02C [P] assert "mNumCharBolts[charID] < kMaxModelBolts";
                                         //       [U] indexCharBolts counts per group; getNumCharBolts / getCharBolts /
                                         //       findBoltIndex
    int mCharBoltsStart[kCHEATCHAR_NUM]; // 0x0A4 (ours, after getCharBoltsStart) [U] indexCharBolts: each group's first
                                         //       definition index; getCharBolts = &mpCharBolts[mCharBoltsStart[g]]
    short* mpNumChildBolts[kCHEATCHAR_NUM];  // 0x11C (ours) [R] buildChildTables: per group, a parent bolt id ->
                                             //       child count table; [U] countChildBolts, getFirstChildBolt,
                                             //       collectChildBolts (null: search the group instead)
    short* mpFirstChildBolt[kCHEATCHAR_NUM]; // 0x194 (ours) [R] buildChildTables: per group, a parent bolt id -> first
                                             //       child's definition index; [U] getFirstChildBolt
    int mNumCharRules[kCHEATCHAR_NUM];   // 0x20C (ours, after getNumCharRules) [U] indexRules counts per group (clears
                                         //       first); getNumCharRules, countBoltRules, findBoltRules, getBoltRules
    int mCharRulesStart[kCHEATCHAR_NUM]; // 0x284 (ours) [U] indexRules: each group's first rule index; the rule lookups
    int mNumCharPairs[kCHEATCHAR_NUM];   // 0x2FC (ours, after getNumCharPairs) [U] indexPairs; getNumCharPairs,
                                         //       getCharPairs
    int mCharPairsStart[kCHEATCHAR_NUM]; // 0x374 (ours) [U] indexPairs; getCharPairs
    int mNumCharGroups[kCHEATCHAR_NUM];  // 0x3EC (ours, after getNumCharGroups) [U] indexGroups; getNumCharGroups,
                                         //       getCharGroups
    int mCharGroupsStart[kCHEATCHAR_NUM]; // 0x464 (ours) [U] indexGroups; getCharGroups
    short* mpBoltIndex[kCHEATCHAR_NUM];  // 0x4DC (ours) [R] buildChildTables: per group, a bolt id -> definition index
                                         //       table; [U] findBoltIndex (null: search the group instead)

    // Methods bound to their retail symbols by asm labels.
    void loadFile() __asm__("cBEBoltDB_loadFile");
    void buildChildTables() __asm__("cBEBoltDB_buildChildTables");
    void indexCharBolts() __asm__("cBEBoltDB_indexCharBolts");
    void indexRules() __asm__("cBEBoltDB_indexRules");
    void indexPairs() __asm__("cBEBoltDB_indexPairs");
    void indexGroups() __asm__("cBEBoltDB_indexGroups");
    void setBoltModelPart(int groupId, int key, int value) __asm__("cBEBoltDB_setBoltModelPart");
    void setBoltEquipPoints(int groupId, int key, int value) __asm__("cBEBoltDB_setBoltEquipPoints");
    void* load() __asm__("cBEBoltDB_load");
    void refreshVisibleBolts() __asm__("cBEBoltDB_refreshVisibleBolts");
    void freeData() __asm__("cBEBoltDB_freeData");
    int getBoltDefIndex(tBEBoltDef* item) __asm__("cBEBoltDB_getBoltDefIndex");
    int findBoltIndex(int groupId, int key) __asm__("cBEBoltDB_findBoltIndex");
    int countChildBolts(int groupId, int key) __asm__("cBEBoltDB_countChildBolts");
    tBEBoltDef* getFirstChildBolt(int groupId, int key) __asm__("cBEBoltDB_getFirstChildBolt");
    int collectChildBolts(int groupId, int key, tBEBoltDef** out, int depth, int all)
        __asm__("cBEBoltDB_collectChildBolts");
    int collectCategoryBolts(int groupId, int key, tBEBoltDef** out, int depth, int all)
        __asm__("cBEBoltDB_collectCategoryBolts");
    int isBoltAncestor(tBEBoltDef* boltDef, int key) __asm__("cBEBoltDB_isBoltAncestor");
    int getNumCharBolts(int i) __asm__("cBEBoltDB_getNumCharBolts__FPvi");
    tBEBoltDef* getCharBolts(int i) __asm__("cBEBoltDB_getCharBolts");
    tBEBoltDef* getCharBolts1(int i) __asm__("cBEBoltDB_getCharBolts1");
    int getCharBoltsStart(int groupId) __asm__("cBEBoltDB_getCharBoltsStart");
    int getNumCharRules(int groupId) __asm__("cBEBoltDB_getNumCharRules");
    int countBoltRules(int groupId, int key) __asm__("cBEBoltDB_countBoltRules");
    void* findBoltRule(int groupId, int key) __asm__("cBEBoltDB_findBoltRule");
    int findBoltRules(int groupId, int wantKey, int* out) __asm__("cBEBoltDB_findBoltRules");
    tBEBoltRule* getBoltRules(int groupId, int key, int want, int* out) __asm__("cBEBoltDB_getBoltRules");
    int getNumCharPairs(int i) __asm__("cBEBoltDB_getNumCharPairs__FPvi");
    tBEBoltPair* getCharPairs(int i) __asm__("cBEBoltDB_getCharPairs");
    int getNumCharGroups(int i) __asm__("cBEBoltDB_getNumCharGroups__FPvi");
    tBEBoltGroup* getCharGroups(int i) __asm__("cBEBoltDB_getCharGroups");
};

CHECK_OFFSET(cBEBoltDB, mpBoltsFile, 0x000);
CHECK_OFFSET(cBEBoltDB, mpCharBolts, 0x004);
CHECK_OFFSET(cBEBoltDB, mpCharRules, 0x008);
CHECK_OFFSET(cBEBoltDB, mpCharPairs, 0x00C);
CHECK_OFFSET(cBEBoltDB, mpCharGroups, 0x010);
CHECK_OFFSET(cBEBoltDB, mpStringPool, 0x014);
CHECK_OFFSET(cBEBoltDB, mpChildBoltsData, 0x018);
CHECK_OFFSET(cBEBoltDB, mTotalNumCharBolts, 0x01C);
CHECK_OFFSET(cBEBoltDB, mTotalNumCharRules, 0x020);
CHECK_OFFSET(cBEBoltDB, mTotalNumCharPairs, 0x024);
CHECK_OFFSET(cBEBoltDB, mTotalNumCharGroups, 0x028);
CHECK_OFFSET(cBEBoltDB, mNumCharBolts, 0x02C);
CHECK_OFFSET(cBEBoltDB, mCharBoltsStart, 0x0A4);
CHECK_OFFSET(cBEBoltDB, mpNumChildBolts, 0x11C);
CHECK_OFFSET(cBEBoltDB, mpFirstChildBolt, 0x194);
CHECK_OFFSET(cBEBoltDB, mNumCharRules, 0x20C);
CHECK_OFFSET(cBEBoltDB, mCharRulesStart, 0x284);
CHECK_OFFSET(cBEBoltDB, mNumCharPairs, 0x2FC);
CHECK_OFFSET(cBEBoltDB, mCharPairsStart, 0x374);
CHECK_OFFSET(cBEBoltDB, mNumCharGroups, 0x3EC);
CHECK_OFFSET(cBEBoltDB, mCharGroupsStart, 0x464);
CHECK_OFFSET(cBEBoltDB, mpBoltIndex, 0x4DC);
CHECK_SIZE(cBEBoltDB, 0x554);

// The bolt database ([O] On Tour pattern: a static member of cBELibrary).
extern cBEBoltDB cBELibrary_boltDB;

#endif /* BE_BEDBBOLT_H */
