#ifndef SCRIPTER_SCANIMMGR_H
#define SCRIPTER_SCANIMMGR_H

// cScriptAnimBankManager: the NIS script engine's table of script animation banks. Each slot (tScriptAnimBankInfo)
// has a fixed anim-database bank number (startBankNum + i); LinkBank wraps a script's attached animation data in a
// cAdfDataSource, installs it in the anim database's bank table (cAnimDataBase_TheAnimDataBase) at the slot's bank
// number and builds the table of the bank's non-empty anim indices; UnlinkBank takes it out again. One instance:
// cSSXScriptEngine::mAnimationBankManager (+0xA28, scripter/ssxscriptengine.h), built by the engine ctor with
// (memFlags 0, maxBanks 10, startBankNum 0xF0). Its twin cScriptSoundBankManager (+0xA38, scripter/scsndmgr.h) has the
// same three members and vptr.
// EA's source: d:/ssx/code/source/scripter/scanimmgr.cpp (July 2003 preview assert strings). Retail methods: unit
// scripter/scanimmgr (0x002753B8 ctor .. 0x002757F0 GetEmptyBankInfo).
//
// Size 0x10: [R] cSSXScriptEngine's ctor builds it at +0xA28 and its next member (mSoundBankManager) is at +0xA38;
// the ctor writes 0x0-0xC. [O] On Tour: 0x10 (mMemFlags 0x0, mMaxBanks 0x4, mpBankInfo 0x8).
// No base class. g++ 2.95 puts the vptr after the class's data: 0xC. Retail vtable cScriptAnimBankManager_vtbl
// (0x004825D8, 2 slots: 1 dtor cScriptAnimBankManager__cScriptAnimBankManager, 2 cScriptAnimBankManager_UnlinkBank1,
// the UnlinkBank(tScriptAnimBankInfo*) overload).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptAnimBankManager_cScriptAnimBankManager (allocation tag "tScriptAnimBankInfo",
//       maxBanks * 0x14) and dtor cScriptAnimBankManager__cScriptAnimBankManager;
//   [P] tools/layout_xlate.py pairs the preview's dtor with retail's (0x8 and 0xC unchanged); no inline accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptAnimBankManager 0x10, tScriptAnimBankInfo 0x14).

#include "layout.h"

// One script animation bank slot (On Tour tScriptAnimBankInfo, 0x14). cSSXScriptEngine's GetBankInfo forwarder
// returns one.
struct tScriptAnimBankInfo {
    int userID;                         // 0x00 [O]; [R] ctor -1 (free); [U] LinkBank stores the caller's ID,
                                        //      UnlinkBank1 -1, GetBankInfo matches it
    int bankNum;                        // 0x04 [O]; [R] ctor startBankNum + i; [U] LinkBank's return value; LinkBank /
                                        //      UnlinkBank1 index the anim database's bank table with its low byte (lbu 0x4)
    char* pData;                        // 0x08 [O]; [R] ctor 0; [U] LinkBank stores the data, UnlinkBank1 0; GetBankInfo /
                                        //      GetEmptyBankInfo / UnlinkBanksAll test it (0 = free)
    int numAnims;                       // 0x0C [O]; [R] ctor 0; [U] LinkBank counts the bank's non-null anims into it,
                                        //      UnlinkBank1 frees pTableAnimID when it is non-zero and clears it
    int* pTableAnimID;                  // 0x10 [O]; [R] ctor 0; [U] LinkBank allocates numAnims * 4 bytes
                                        //      ("pTableAnimID") and fills the anim indices, UnlinkBank1 frees it, 0
};

CHECK_OFFSET(tScriptAnimBankInfo, userID, 0x00);
CHECK_OFFSET(tScriptAnimBankInfo, bankNum, 0x04);
CHECK_OFFSET(tScriptAnimBankInfo, pData, 0x08);
CHECK_OFFSET(tScriptAnimBankInfo, numAnims, 0x0C);
CHECK_OFFSET(tScriptAnimBankInfo, pTableAnimID, 0x10);
CHECK_SIZE(tScriptAnimBankInfo, 0x14);

struct cScriptAnimBankManager {
    int mMemFlags;                      // 0x0 [O]; [R] the ctor stores memFlags and allocates the slot table with them;
                                        //     [U] LinkBank allocates the cAdfDataSource and pTableAnimID with them
    int mMaxBanks;                      // 0x4 [O]; [R] the ctor stores maxBanks; [U] the loop bound of GetBankInfo,
                                        //     GetEmptyBankInfo, UnlinkBanksAll (and the ctor's own loop, which re-reads it)
    tScriptAnimBankInfo* mpBankInfo;    // 0x8 [O]; [R] ctor: operator new(maxBanks * 0x14, "tScriptAnimBankInfo",
                                        //     memFlags); [U] the dtor frees it (cMemMan_free), the lookups index it
    void** mVtbl;                       // 0xC [R] cScriptAnimBankManager_vtbl (ctor and dtor); [U] UnlinkBank and
                                        //     UnlinkBanksAll call slot 2 (UnlinkBank1) through it
};

CHECK_OFFSET(cScriptAnimBankManager, mMemFlags, 0x0);
CHECK_OFFSET(cScriptAnimBankManager, mMaxBanks, 0x4);
CHECK_OFFSET(cScriptAnimBankManager, mpBankInfo, 0x8);
CHECK_OFFSET(cScriptAnimBankManager, mVtbl, 0xC);
CHECK_SIZE(cScriptAnimBankManager, 0x10);

#endif /* SCRIPTER_SCANIMMGR_H */
