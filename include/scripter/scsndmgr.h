#ifndef SCRIPTER_SCSNDMGR_H
#define SCRIPTER_SCSNDMGR_H

// cScriptSoundBankManager: the NIS script engine's table of script sound banks. Each slot (tScriptSoundBankInfo) has a
// fixed audio bank number (startBankNum + i); LinkBank attaches a script's sound data to a free slot under the
// caller's user ID (cBankManager_BANK_Attach on the audio system), UnlinkBank detaches it. One instance:
// cSSXScriptEngine::mSoundBankManager (+0xA38, scripter/ssxscriptengine.h), built by the engine ctor with
// (memFlags 0, maxBanks 10, startBankNum 17). Its twin cScriptAnimBankManager (+0xA28, scripter/scanimmgr) has the
// same three members.
// EA's source: d:/ssx/code/source/scripter/scsndmgr.cpp (July 2003 preview assert strings). Retail methods: unit
// scripter/scsndmgr (0x00283450 ctor .. 0x00283760 GetEmptyBankInfo).
//
// Size 0x10: [R] cSSXScriptEngine's ctor builds it at +0xA38 and its next member (mCurrAttachingBankNum) is at +0xA48;
// the ctor writes 0x0-0xC. [O] On Tour: 0x10 (mMemFlags 0x0, mMaxBanks 0x4, mpBankInfo 0x8).
// No base class. g++ 2.95 puts the vptr after the class's data: 0xC. Retail vtable cScriptSoundBankManager_vtbl
// (0x00482240, 2 slots: 1 dtor cScriptSoundBankManager__cScriptSoundBankManager, 2 cScriptSoundBankManager_UnlinkBank,
// the UnlinkBank(tScriptSoundBankInfo*) overload).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptSoundBankManager_cScriptSoundBankManager (allocation tag "tScriptSoundBankInfo",
//       maxBanks * 0xC) and dtor cScriptSoundBankManager__cScriptSoundBankManager;
//   [P] tools/layout_xlate.py pairs the preview's dtor with retail's (0x8 and 0xC unchanged); no inline accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptSoundBankManager 0x10, tScriptSoundBankInfo 0xC).

#include "layout.h"

// One script sound bank slot (On Tour tScriptSoundBankInfo, 0xC). cScriptObjRider::mpSoundBankInfo and
// cScriptObjEvent::mpSoundBankInfo point at one (cSSXScriptEngine_GetSoundBankInfo).
struct tScriptSoundBankInfo {
    int userID;                        // 0x0 [O]; [R] ctor -1 (free); [U] LinkBank stores the caller's ID, UnlinkBank -1,
                                       //     GetBankInfo matches it
    int bankNum;                       // 0x4 [O]; [R] ctor startBankNum + i; [U] LinkBank / UnlinkBank pass it to
                                       //     cBankManager_BANK_Attach / BANK_UnAttach; ScriptSoundIDToGameSoundInfo reads it
    char* pData;                       // 0x8 [O]; [R] ctor 0; [U] LinkBank stores the data, UnlinkBank 0; GetBankInfo /
                                       //     GetEmptyBankInfo / UnlinkBanksAll test it (0 = free)
};

CHECK_OFFSET(tScriptSoundBankInfo, userID, 0x0);
CHECK_OFFSET(tScriptSoundBankInfo, bankNum, 0x4);
CHECK_OFFSET(tScriptSoundBankInfo, pData, 0x8);
CHECK_SIZE(tScriptSoundBankInfo, 0xC);

struct cScriptSoundBankManager {
    int mMemFlags;                     // 0x0 [O]; [R] the ctor stores memFlags and allocates the slot table with them
    int mMaxBanks;                     // 0x4 [O]; [R] the ctor stores maxBanks; [U] the loop bound of GetBankInfo,
                                       //     GetEmptyBankInfo, UnlinkBanksAll (and the ctor's own loop, which re-reads it)
    tScriptSoundBankInfo* mpBankInfo;  // 0x8 [O]; [R] ctor: operator new(maxBanks * 0xC, "tScriptSoundBankInfo",
                                       //     memFlags); [U] the dtor frees it (cMemMan_free), the lookups index it
    void** mVtbl;                      // 0xC [R] cScriptSoundBankManager_vtbl (ctor and dtor); [U] UnlinkBank1 and
                                       //     UnlinkBanksAll call slot 2 (UnlinkBank) through it
};

CHECK_OFFSET(cScriptSoundBankManager, mMemFlags, 0x0);
CHECK_OFFSET(cScriptSoundBankManager, mMaxBanks, 0x4);
CHECK_OFFSET(cScriptSoundBankManager, mpBankInfo, 0x8);
CHECK_OFFSET(cScriptSoundBankManager, mVtbl, 0xC);
CHECK_SIZE(cScriptSoundBankManager, 0x10);

#endif /* SCRIPTER_SCSNDMGR_H */
