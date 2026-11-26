#ifndef SOUND_BANKMANAGER_H
#define SOUND_BANKMANAGER_H

// cBankManager and cBankInstance: the sound-bank table. cBankManager owns mMaxBanks cBankInstance slots (one per
// loaded .bnk file), loads them synchronously (BANK_SyncLoad) or through its cAsyncSys base (BANK_AsyncLoad, then the
// ASYNCSYS_On* overrides forward each step to the slot), and is the shared play-option / monitor object of the whole
// sound system through its cBankMonitor base. EA source d:/ssx/code/source/sound/bankmanager.cpp (the July 2003
// preview's assert strings "!gpBankManager->ASYNCSYS_Busy()", "mBankStatus != LOADING", "mSoundMemHandle >= 0",
// "mpNext == NULL && mpPrev == NULL", ... and the tags "StaticSoundMem", "DynamicSoundMem").
//
//   cBankManager : cAsyncSys (0x0), cBankMonitor (0x1D8), virtual cSoundSys
// Retail methods: cBankManager_cBankManager (0x28B9B8) .. cBankManager_ASYNCSYS_OnEndAsyncLoad (0x28BF38) in the
// splat unit sound/bankmanager, and the out-of-line inlines BANK_Status / BANK_Used / BANK_Loading / BANK_InUse /
// BANK_Handle (bx/seg_1BA100). cBankInstance's methods: cBankInstance_cBankInstance .. cBankInstance_Unload
// (0x28B248..0x28B588, splat unit sound/asyncsys) and cBankInstance_AllocMem .. cBankInstance_OnEndAsyncLoad
// (0x28B588..0x28B9B8, sound/bankmanager). gpBankManager (0x004A3614) is the one cBankManager [O] (On Tour static
// gpBankManager); [R] its ctor stores this, its dtor clears it.
//
// Non-virtual part 0xAD0 [R]: the ctor's in-charge branch puts the cSoundSys virtual base at this + 0xAD0 (sw
// this + 0xAD0 into 0x1D8, the cBankMonitor's virtual-base pointer, which cBankManager shares) and the
// "BankInstances" array (operator new[](count x 0x60)) at 0xACC; in cSSXAudio the object is at 0x6C98 and the next
// virtual base (cStreamSys) at 0x7768 = 0x6C98 + 0xAD0 (sound/ssxaudio.h). A complete cBankManager is 0xAD8. On Tour's
// 0xAD4 is the complete object too; its members sit 4 bytes earlier (mMaxBanks 0xAC4: its cBankMonitor's two bools are
// bytes). The preview had mBank at 0xAC0 (+0xC in retail; tools/layout_xlate.py).
// Vtables [R]: cBankManager_vtbl (0x483AD8) in the cAsyncSys vptr slot (0x1D4; slot 1 dtor, slots 2..5 the
// ASYNCSYS_On* overrides), cBankMonitor_vtbl (0x483B48) at 0xAB0 (the cBankMonitor's vptr), and D_00483AB8 (the
// cSoundSys-in-cBankManager table, stored at cSoundSys + 0x4).
//
// The struct below is the NON-VIRTUAL part, so it can be embedded where the object lives (cSSXAudio + 0x6C98). The
// cAsyncSys base is written out inline (its fields keep cAsyncSys's names, sound/asyncsys.h); the cBankMonitor base is
// embedded as the struct from sound/bankmonitor.h, as sound/ssxaudio.h already does, since callers pass
// &BankMonitor (gpBankManager + 0x1D8) to the cBankMonitor / cSndPlayOpts methods.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructors cBankManager_cBankManager / cBankInstance_cBankInstance, destructors, allocation tags;
//   [P] the July 2003 preview's out-of-line inlines BANK_Status / Used / Loading / Handle (preview mBank 0xAC0) and
//       ASYNCSYS_Busy / GetCurrOperationUserHandle, carried to retail by tools/layout_xlate.py;
//   [O] SSX On Tour's DWARF (class cBankManager 0xAD4: mMaxBanks, mBank; class cBankInstance 0x60, same offsets);
//   [U] how the retail methods use the field.

#include "layout.h"
#include "sound/asyncsys.h"
#include "sound/bankmonitor.h"

// cBankInstance::mBankStatus: [O] On Tour's enum eBankStatus, names and values (the preview asserts "mBankStatus !=
// LOADING"). [U] SyncLoad / OnEndAsyncLoad set 1 for a loaded bank, OnBeginAsyncLoad 2 while it loads, Unload 0;
// IsBankLoaded tests 1.
enum eBankStatus { EMPTY = 0, USED = 1, LOADING = 2 };

// cBankInstance (0x60 [R] operator new[](count x 0x60, "BankInstances"); [O] same size and offsets): one bank slot.
// Its On Tour enums are kept as ints: eBankStatus (above), eSoundMemLocation (sound/asyncsys.h).
struct cBankInstance {
    int mBankStatus;            // 0x00 [O] eBankStatus; [R] ctor 0; [P] BANK_Status / BANK_Used (== 1) / BANK_Loading
                                //       (== 2) / BANK_Handle; [U] SyncLoad / OnEndAsyncLoad set 1 or 0,
                                //       OnBeginAsyncLoad 2, Unload / UnAttach 0, Attach 1, IsBankLoaded tests 1
    int mBankHandle;            // 0x04 [O]; [R] ctor -1; [P] BANK_Handle returns it when used; [U]
                                //       SNDbankadd(&mBankHandle) in SyncLoad / Attach, OnEndAsyncLoad stores it, Unload
                                //       / UnAttach SNDbankremove it and set -1
    // mBankHandle when no bank is added (name ours). [R] ctor -1; [U] Unload / UnAttach reset it and remove the bank
    // only when it differs.
    enum { kNoBankHandle = -1 };
    int mAllocLocation;         // 0x08 [O] eSoundMemLocation; [R] ctor 0; [U] AllocMem 1 or 2, DeallocMem 0 (frees
                                //       sound memory when 2), SyncLoad switches on it, BANK_AsyncLoad passes it to
                                //       ASYNCSYS_QueueLoad, ShareBank copies it
    int mSoundMemHandle;        // 0x0C [O]; [R] ctor -1; [U] AllocMem stores the sound-memory id, DeallocMem frees it
                                //       (-1), SyncLoad / OnBeginAsyncLoad pass it to the sound manager, ShareBank
                                //       copies it, UnshareBank -1
    char* mpMainMemData;        // 0x10 [O]; [R] ctor 0; [U] AllocMem MEM_alloc("StaticSoundMem"), OnAsyncMainMemAlloc
                                //       MEM_alloc("DynamicSoundMem"), Attach stores the image, SyncLoad loads into it,
                                //       DeallocMem / Unload free it
    int mMainMemDataSize;       // 0x14 [O]; [R] ctor 0; [U] AllocMem, Attach, SyncLoad, OnAsyncMainMemAlloc store it,
                                //       OnAsyncMainMemSize returns it
    cBankInstance* mpNext;      // 0x18 [O]; [R] ctor 0; [U] ShareBank / UnshareBank link the ring of banks sharing one
                                //       memory block, SharedUnload walks it
    cBankInstance* mpPrev;      // 0x1C [O]; [R] ctor 0; [U] ShareBank / UnshareBank
    char mBankFilename[64];     // 0x20 [O]; [R] ctor clears byte 0; [U] SyncLoad / OnEndAsyncLoad strcpy() the file
                                //       name, IsBankLoaded compares it

    // Methods bound to their retail symbols by asm labels.
    void SharedUnload() __asm__("cBankInstance_SharedUnload");
    void* AllocMem(int size, int extra, int tag) __asm__("cBankInstance_AllocMem");
    void DeallocMem() __asm__("cBankInstance_DeallocMem");
    void ShareBank(cBankInstance* src) __asm__("cBankInstance_ShareBank");
    void UnshareBank() __asm__("cBankInstance_UnshareBank");
    int IsBankLoaded(void* name) __asm__("cBankInstance_IsBankLoaded");
    int OnAsyncMainMemSize() __asm__("cBankInstance_OnAsyncMainMemSize__FPv");
    void SyncLoad(const char* file, int align) __asm__("cBankInstance_SyncLoad");
    void Unload() __asm__("cBankInstance_Unload");
    void UnAttach() __asm__("cBankInstance_UnAttach");
    void OnBeginAsyncLoad() __asm__("cBankInstance_OnBeginAsyncLoad");
    void* OnAsyncMainMemAlloc(int size, int flags) __asm__("cBankInstance_OnAsyncMainMemAlloc");
};

struct cBankManager {
    // --- cAsyncSys (base at 0x0; sound/asyncsys.h; data 0x1D4, then the shared vptr)
    int mHandle;                // 0x000 [O] cAsyncSys::mHandle (ASYNCFILE handle of the current load)
    int mBusy;                  // 0x004 [O] cAsyncSys::mBusy; [P] ASYNCSYS_Busy (preview lw 0x4)
    int mFileIndex;             // 0x008 [O] cAsyncSys::mFileIndex
    char mFilename[6][64];      // 0x00C [O] cAsyncSys::mFilename
    cCircBuffer mUserHandle;    // 0x18C [O] cAsyncSys::mUserHandle (bank numbers); [P] GetCurrOperationUserHandle
    cCircBuffer mFilenameIndex; // 0x19C [O] cAsyncSys::mFilenameIndex
    cCircBuffer mMemType;       // 0x1AC [O] cAsyncSys::mMemType
    cCircBuffer mMainMemFlags;  // 0x1BC [O] cAsyncSys::mMainMemFlags
    char* mpBuffer;             // 0x1CC [O] cAsyncSys::mpBuffer ("AsyncSysBuffer")
    int mBufferSize;            // 0x1D0 [O] cAsyncSys::mBufferSize
    void** mVtbl;               // 0x1D4 [R] cBankManager_vtbl (ctor / dtor; cAsyncSys's vptr slot, cAsyncSys_vtbl while
                                //       the base is built)
    // --- cBankMonitor (base at 0x1D8; sound/bankmonitor.h, non-virtual part 0x8F0)
    cBankMonitor BankMonitor;   // 0x1D8 [R] ctor cBankMonitor_cBankMonitor(this + 0x1D8, 0, mode), dtor
                                //       cBankMonitor__cBankMonitor(this + 0x1D8, 0); BankMonitor.mpVSoundSys (0x1D8) is
                                //       cBankManager's own cSoundSys pointer (in-charge ctor: this + 0xAD0),
                                //       BankMonitor.mVtbl (0xAB0) gets cBankMonitor_vtbl; [U] gpBankManager + 0x1D8 is
                                //       how every sound class reaches the play options and monitors
    // --- cBankManager
    int mMaxBanks;              // 0xAC8 [O]; [R] ctor stores its count argument
    cBankInstance* mBank;       // 0xACC [O]; [R] operator new[](count x 0x60, "BankInstances") with
                                //       cBankInstance_cBankInstance; the dtor destroys the array (count from its new[]
                                //       cookie, cBankInstance__cBankInstance) and frees it; [P] BANK_Status / Used /
                                //       Loading / Handle (preview 0xAC0, seen); [U] every BANK_* / ASYNCSYS_On* method
                                //       indexes it by bank number

    // Methods bound to their retail symbols by asm labels.
    int BANK_Status(int bankId) __asm__("cBankManager_BANK_Status");
    bool BANK_Used(int bankId) __asm__("cBankManager_BANK_Used");
    bool BANK_Loading(int bankId) __asm__("cBankManager_BANK_Loading");
    int BANK_InUse(int idx) __asm__("cBankManager_BANK_InUse");
    int BANK_Handle(int bankId) __asm__("cBankManager_BANK_Handle");
    void BANK_SyncLoad(int i, void* id, int mainMemFlag) __asm__("cBankManager_BANK_SyncLoad");
    void BANK_UnLoad(int i) __asm__("cBankManager_BANK_UnLoad");
    void BANK_AsyncLoad(int i, void* id, int mainMemFlag) __asm__("cBankManager_BANK_AsyncLoad");
    void* BANK_AllocMem(int i, int mainMemSize, int soundMemSize, int memFlags) __asm__("cBankManager_BANK_AllocMem");
    void BANK_DeallocMem(int i) __asm__("cBankManager_BANK_DeallocMem");
    void BANK_ShareBank(int i, int j) __asm__("cBankManager_BANK_ShareBank");
    void BANK_UnshareBank(int i) __asm__("cBankManager_BANK_UnshareBank");
    void BANK_Attach(int i, int mainMemData, int mainMemSize) __asm__("cBankManager_BANK_Attach");
    void BANK_UnAttach(int i) __asm__("cBankManager_BANK_UnAttach");
    void ASYNCSYS_OnBeginAsyncLoad(int i) __asm__("cBankManager_ASYNCSYS_OnBeginAsyncLoad");
    void ASYNCSYS_OnAsyncMainMemAlloc(int i, int size, int memflags)
        __asm__("cBankManager_ASYNCSYS_OnAsyncMainMemAlloc");
    void ASYNCSYS_OnAsyncMainMemSize(int i) __asm__("cBankManager_ASYNCSYS_OnAsyncMainMemSize");
    void ASYNCSYS_OnEndAsyncLoad(int i, int soundHandle, int endState, int pFilename, int pBuffer, int dataSize)
        __asm__("cBankManager_ASYNCSYS_OnEndAsyncLoad");
};

CHECK_OFFSET(cBankInstance, mBankStatus, 0x00);
CHECK_OFFSET(cBankInstance, mBankHandle, 0x04);
CHECK_OFFSET(cBankInstance, mAllocLocation, 0x08);
CHECK_OFFSET(cBankInstance, mSoundMemHandle, 0x0C);
CHECK_OFFSET(cBankInstance, mpMainMemData, 0x10);
CHECK_OFFSET(cBankInstance, mMainMemDataSize, 0x14);
CHECK_OFFSET(cBankInstance, mpNext, 0x18);
CHECK_OFFSET(cBankInstance, mpPrev, 0x1C);
CHECK_OFFSET(cBankInstance, mBankFilename, 0x20);
CHECK_SIZE(cBankInstance, 0x60);

CHECK_OFFSET(cBankManager, mHandle, 0x000);
CHECK_OFFSET(cBankManager, mBusy, 0x004);
CHECK_OFFSET(cBankManager, mFileIndex, 0x008);
CHECK_OFFSET(cBankManager, mFilename, 0x00C);
CHECK_OFFSET(cBankManager, mUserHandle, 0x18C);
CHECK_OFFSET(cBankManager, mFilenameIndex, 0x19C);
CHECK_OFFSET(cBankManager, mMemType, 0x1AC);
CHECK_OFFSET(cBankManager, mMainMemFlags, 0x1BC);
CHECK_OFFSET(cBankManager, mpBuffer, 0x1CC);
CHECK_OFFSET(cBankManager, mBufferSize, 0x1D0);
CHECK_OFFSET(cBankManager, mVtbl, 0x1D4);
CHECK_OFFSET(cBankManager, BankMonitor, 0x1D8);
CHECK_OFFSET(cBankManager, mMaxBanks, 0xAC8);
CHECK_OFFSET(cBankManager, mBank, 0xACC);
CHECK_SIZE(cBankManager, 0xAD0);

#endif /* SOUND_BANKMANAGER_H */
