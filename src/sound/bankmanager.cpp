#include "common.h"
#include "be/beinterface.h"
#include "sound/bankmonitor.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "sound/bankmanager.h"
#include "sound/ps2soundman.h"
#include "be/beintnewrace.h"
#include "be/bedbprofile.h"
#include "main/gamemode.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "sound/streamsys.h"
#include "ai/rider.h"
#include "ai/riderhandler.h"
#include "ai/airpredictor.h"
#include "main/game.h"
#include "ui/uiengine.h"
//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_AllocMem);
#ifdef SKIP_ASM
extern cBankManager* gpBankManager;
extern char str_StaticSoundMem[];
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
#include "sound/soundsys.h"

/**
 * Allocates a sound bank's memory: optional sound (SPU) memory and a main-memory block ("StaticSoundMem").
 *
 * Sound memory comes from the bank manager's stream allocator; if that reports -3 the sound part is added to the main
 * block instead. mAllocLocation becomes keSoundMemLocation_Main, or keSoundMemLocation_Sound when sound memory was
 * obtained. Nothing happens when both sizes are 0. The unit declares it void*, but it returns no value.
 *
 * @param size  main-memory size in bytes (On Tour mainMemSize)
 * @param extra  sound-memory size in bytes (On Tour soundMemSize)
 * @param tag  memory flags for the main block (On Tour memflags)
 * @return no meaningful value
 */
// PORT: the unit declares this void* (a caller forwards its $v0), but no value is returned.
void* cBankInstance::AllocMem(int size, int extra, int tag)
{
    if (size != 0 || extra != 0) {
        mAllocLocation = keSoundMemLocation_Main;
        if (extra != 0) {
            int id = ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->AllocSoundMem(extra);
            if (id >= 0) {
                mSoundMemHandle = id;
                mAllocLocation = keSoundMemLocation_Sound;
            } else if (id == -3) {
                size += extra;
            }
        }
        mMainMemDataSize = size;
        mpMainMemData = (char*)MEM_alloc(str_StaticSoundMem, size, tag);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_DeallocMem);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(void*);
extern "C" void* MEM_free(void* self);
extern cBankManager* gpBankManager;


/**
 * Unloads the bank and frees the memory it was given.
 *
 * Calls cBankInstance_Unload; then, when the allocation location at +0x8 is non-zero: for keSoundMemLocation_Sound it
 * frees the sound-memory handle at +0xC through cPS2SoundManager_DeallocSoundMem (the virtual of the shared cSoundSys's
 * pManager, reached from gpBankManager's cBankMonitor) and sets the handle to -1; it then MEM_frees the main-memory
 * data at +0x10 and zeroes the data pointer, the size at +0x14 and the location. Called by cBankInstance__cBankInstance
 * and, by bank index, by cBankManager_BANK_DeallocMem.
 */
void cBankInstance::DeallocMem()
{
    cBankInstance_Unload(this);
    int state = mAllocLocation;
    if (state != 0) {
        if (state == keSoundMemLocation_Sound) {
            ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->DeallocSoundMem(mSoundMemHandle);
            mSoundMemHandle = -1;
        }
        MEM_free(mpMainMemData);
        mpMainMemData = 0;
        mMainMemDataSize = 0;
        mAllocLocation = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_ShareBank);
#ifdef SKIP_ASM
/**
 * Makes bank @p dst share the memory of bank @p src.
 *
 * Links @p dst into @p src's ring of sharing banks (mpNext / mpPrev): after @p src when @p src is already in a ring,
 * else @p src and @p dst become a two-bank ring. When @p src has an allocation (mAllocLocation non-zero), copies the
 * location, the main-memory data and size (mpMainMemData, mMainMemDataSize) and the sound-memory handle
 * (mSoundMemHandle) into @p dst. Called by cBankManager_BANK_ShareBank with two banks of the manager's array.
 *
 * @param src  the bank whose memory is shared
 */
void cBankInstance::ShareBank(cBankInstance* src)
{
    int flag;

    if (src->mpNext != 0) {
        mpNext = src;
        mpPrev = src->mpPrev;
        src->mpPrev = this;
        mpPrev->mpNext = this;
    } else {
        src->mpNext = this;
        src->mpPrev = this;
        mpNext = src;
        mpPrev = src;
    }
    flag = src->mAllocLocation;
    if (flag != 0) {
        mAllocLocation = flag;
        mpMainMemData = src->mpMainMemData;
        mMainMemDataSize = src->mMainMemDataSize;
        mSoundMemHandle = src->mSoundMemHandle;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_UnshareBank);
#ifdef SKIP_ASM

/**
 * Takes the bank out of the ring of banks sharing one block of memory.
 *
 * Does nothing when next is null. Otherwise it sets the sound-memory handle unkC to -1, zeroes the main-memory data
 * unk10, its size unk14 and the location unk8 (nothing is freed), and unlinks the bank: when next == prev (a two-bank
 * ring) the other bank's next and prev are cleared, otherwise the neighbours are joined. Its own next and prev are then
 * cleared. Called by cBankInstance__cBankInstance and, by bank index, by cBankManager_BANK_UnshareBank.
 */
void cBankInstance::UnshareBank()
{
    if (mpNext != 0) {
        mSoundMemHandle = -1;
        mpMainMemData = 0;
        mMainMemDataSize = 0;
        mAllocLocation = 0;
        if (mpNext == mpPrev) {
            mpNext->mpNext = 0;
            mpNext->mpPrev = 0;
        } else {
            mpNext->mpPrev = mpPrev;
            mpPrev->mpNext = mpNext;
        }
        mpPrev = 0;
        mpNext = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_Attach);
#ifdef SKIP_ASM
extern "C" void cBankInstance_UnAttach(cBankInstance*);
extern "C" int SNDbankadd(void* sema, void* bankImage);

/**
 * Attaches a bank image already in main memory to the bank.
 *
 * Calls cBankInstance_UnAttach to drop any attached bank, stores @p mainMemData and @p mainMemSize at +0x10 and +0x14, registers
 * the image with SNDbankadd (the bank handle goes to +0x4) and sets the status word at +0x0 to 1 (loaded). Called, by
 * bank index, by cBankManager_BANK_Attach (from cScriptSoundBankManager_LinkBank and cSSXAudio_AttachAudioBank).
 *
 * @param mainMemData  address of the bank image in main memory
 * @param mainMemSize  size of the image in bytes
 */
extern "C" void cBankInstance_Attach(cBankInstance* self, void* mainMemData, int mainMemSize)
{
    self->UnAttach();
    self->mpMainMemData = (char*)mainMemData;
    self->mMainMemDataSize = mainMemSize;
    SNDbankadd(&self->mBankHandle, mainMemData);
    self->mBankStatus = USED;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_UnAttach);
#ifdef SKIP_ASM
extern "C" int SNDbankremove(int id);

/**
 * Detaches the bank attached by cBankInstance_Attach.
 *
 * Sets mBankStatus to EMPTY; when mBankHandle is not kNoBankHandle it removes the bank with SNDbankremove and sets the
 * handle to kNoBankHandle; then it zeroes the main-memory data (+0x10) and size (+0x14) without freeing them. Called by
 * cBankInstance_Attach and, by bank index, by cBankManager_BANK_UnAttach.
 */
void cBankInstance::UnAttach()
{
    mBankStatus = EMPTY;
    if (mBankHandle != cBankInstance::kNoBankHandle) {
        SNDbankremove(mBankHandle);
        mBankHandle = cBankInstance::kNoBankHandle;
    }
    mpMainMemData = 0;
    mMainMemDataSize = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_IsBankLoaded);
#ifdef SKIP_ASM
extern "C" int strcmp(void*, void*);

/**
 * Returns whether the bank holds the loaded file @p name.
 *
 * True only when mBankStatus is USED (loaded) and strcmp (strcmp) finds @p name equal to the bank's filename at +0x20.
 * cBankManager_BANK_SyncLoad and cBankManager_BANK_AsyncLoad use it to skip reloading a bank that is already loaded.
 *
 * @param name  file name to compare
 * @return 1 when the bank is loaded from @p name, else 0
 */
int cBankInstance::IsBankLoaded(void* name)
{
    int status = mBankStatus;
    if (status != USED) {
        return 0;
    }
    if (strcmp(name, (char*)&mBankFilename) != 0) {
        status = 0;
    }
    return status;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnBeginAsyncLoad);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(void*);
extern "C" void cBankInstance_SharedUnload(void* self);
extern cBankManager* gpBankManager;


/**
 * Prepares a bank slot for an asynchronous load: unloads whatever it holds and marks it loading.
 *
 * Calls cBankInstance_Unload and cBankInstance_SharedUnload (On Tour Unload and SharedUnload), sets mBankStatus to
 * LOADING and, for a bank held in sound memory (mAllocLocation keSoundMemLocation_Sound), passes its sound memory
 * handle (+0xC) to virtual slot 6 (SetSoundMemLoadLimits) of the sound manager, the pManager of the cSoundSys virtual
 * base reached through gpBankManager->BankMonitor.mpVSoundSys. On Tour asserts the bank was not already loading and
 * that the handle is valid; retail checks neither.
 */
void cBankInstance::OnBeginAsyncLoad()
{
    cBankInstance_Unload(this);
    cBankInstance_SharedUnload(this);
    mBankStatus = LOADING;
    if (mAllocLocation == keSoundMemLocation_Sound) {
        ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->SetSoundMemLoadLimits(mSoundMemHandle);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnAsyncMainMemAlloc);
#ifdef SKIP_ASM
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern char str_DynamicSoundMem[];

// PORT: real body returns the buffer; a later caller in this unit declares it void(void*, int, int)

/**
 * Supplies main memory for an asynchronous bank load, allocating it ("DynamicSoundMem") unless the bank has
 * some.
 *
 * @param size  size in bytes
 * @param flags  memory flags
 * @return the bank's main-memory block
 */
void* cBankInstance::OnAsyncMainMemAlloc(int size, int flags)
{
    if (mAllocLocation == 0) {
        mpMainMemData = (char*)MEM_alloc(str_DynamicSoundMem, size, flags);
        mMainMemDataSize = size;
    }
    return mpMainMemData;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnAsyncMainMemSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns the size of the main-memory buffer reserved for a bank's asynchronous load.
 *
 * @return the bank's main-memory data size (+0x14, On Tour mMainMemDataSize)
 */
int cBankInstance::OnAsyncMainMemSize()
{
    return mMainMemDataSize;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnEndAsyncLoad);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern cBankManager* gpBankManager;


/**
 * Finishes a bank's asynchronous load: records the sound handle, the outcome and the file name.
 *
 * Calls virtual slot 7 (RestoreSoundMemLoadLimits) of the sound manager (gpBankManager->BankMonitor.mpVSoundSys->
 * pManager), stores the sound handle as the bank
 * handle (+0x4), sets the status (+0x0) to 1 (loaded) when the end state is 0 and to 0 otherwise, and copies the file
 * name into the bank's name buffer (+0x20, 64 bytes in On Tour). On Tour also checks the buffer against the reserved
 * one and the size against it and logs "WARNING: Asynchronous bank loading operation failed." on failure; retail drops
 * those, so the last two arguments are unused. The name is copied even when the load failed.
 *
 * @param id  the sound handle the loader returned
 * @param flag  the end state (On Tour eAsyncSysEndState); 0 means success
 * @param name  the loaded file's name
 * @param pBuffer  the load buffer (unused)
 * @param dataSize  the number of bytes loaded (unused)
 */
extern "C" void cBankInstance_OnEndAsyncLoad(cBankInstance* self, int id, int flag, const char* name, void* pBuffer,
                                             int dataSize)
{
    ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->RestoreSoundMemLoadLimits();
    self->mBankHandle = id;
    if (flag == 0) {
        self->mBankStatus = USED;
    } else {
        self->mBankStatus = EMPTY;
    }
    strcpy(self->mBankFilename, name);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_cBankManager);
#ifdef SKIP_ASM
extern "C" void* cBankMonitor_cBankMonitor(void* self, int a1, int heap);
extern "C" void* cAsyncSys_cAsyncSys(void* self);
extern "C" void* cBankInstance_cBankInstance(void* self);
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern cBankManager* gpBankManager;
extern const char str_BankInstances[];
extern void* cBankMonitor_vtbl[];
extern void* cBankManager_vtbl[];

struct sBmVEntry {
    short delta;
    short index;
    void* fn;
};
struct sBmVtbl {
    sBmVEntry e[4];
} __attribute__((aligned(8)));
extern const sBmVtbl D_00483AB8;

struct sBmBank {
    char pad_0x0[0x60];
    sBmBank() { cBankInstance_cBankInstance(this); }
    void operator delete[](void* p, unsigned int size);
};

// A complete cBankManager (the in-charge ctor builds it): g++ 2.95 puts the cSoundSys virtual base after the
// non-virtual part.
struct cBankManagerComplete {
    cBankManager mBankManager;  // 0x000
    cSoundSys mSoundSys;        // 0xAD0
};

/**
 * Constructs the bank manager: async and bank-monitor parts, the bank table, and the global gpBankManager.
 *
 * With @p embedded set it also builds the cSoundSys virtual base; otherwise it patches a stack copy of the
 * bank
 * monitor's vtable with this object's offset. Allocates @p count banks ("BankInstances").
 *
 * @param embedded  g++ in-charge flag: nonzero when the virtual base is built here
 * @param count  maximum number of banks (On Tour MaxBanks)
 * @param heap  audio mode (eSOUNDDEF_MODE), passed to cSoundSys and the bank monitor
 * @return @p self
 */
extern "C" cBankManager* cBankManager_cBankManager(cBankManager* self, int embedded, int count, int heap)
{
    if (embedded) {
        self->BankMonitor.mpVSoundSys = &((cBankManagerComplete*)self)->mSoundSys;
        cSoundSys_cSoundSys(&((cBankManagerComplete*)self)->mSoundSys, heap);
    }
    cAsyncSys_cAsyncSys(self);
    cBankMonitor_cBankMonitor(&self->BankMonitor, 0, heap);
    *(const sBmVtbl**)&self->BankMonitor.mpVSoundSys->mVtbl = &D_00483AB8;
    if (!embedded) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtbl vt = D_00483AB8;
        *(sBmVtbl**)&self->BankMonitor.mpVSoundSys->mVtbl = &vt;
        int base = (int)self->BankMonitor.mpVSoundSys - sizeof(cBankManager);
        int thisDelta = (int)self - base;
        vt.e[1].delta = D_00483AB8.e[1].delta + thisDelta;
    }
    self->mMaxBanks = count;
    self->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mVtbl = cBankManager_vtbl;
    sBmBank** slot = (sBmBank**)&self->mBank;
    *slot = new (str_BankInstances, 0, 0) sBmBank[count];
    gpBankManager = (cBankManager*)self;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager__cBankManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cAsyncSys__cAsyncSys(void* self, int flags);
extern "C" void cBankInstance__cBankInstance(void* self, int flags);
extern "C" void cBankMonitor__cBankMonitor(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern cBankManager* gpBankManager;
extern void* cBankMonitor_vtbl[];
extern void* cBankManager_vtbl[];

struct sBmVEntryBB10 {
    short delta;
    short index;
    void* fn;
};
struct sBmVtblBB10 {
    sBmVEntryBB10 e[4];
} __attribute__((aligned(8)));
extern const sBmVtblBB10 D_00483AB8_BB10 __asm__("D_00483AB8");



/**
 * Destroys the bank manager: frees its bank instances, tears down its monitor and async-loader parts and clears
 * gpBankManager.
 *
 * Restores the cBankManager and cBankMonitor vtables and the virtual base's vtable (D_00483AB8; with flags 0 the g++
 * 2.95 virtual-base this-adjust fix-up runs on a stack copy), clears gpBankManager, destroys the cBankInstance array
 * mBank last-first with cBankInstance__cBankInstance (its count is the word 0x10 bytes before it) and frees it, then
 * runs cBankMonitor__cBankMonitor on BankMonitor and cAsyncSys__cAsyncSys (On Tour ~cAsyncSys) on the object.
 * cSoundSys__cSoundSys destroys the virtual base only when flags bit 1 is set.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object's memory, bit 1 destroys the cSoundSys virtual base
 */
extern "C" void cBankManager__cBankManager(cBankManager* self, int flags)
{
    self->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mVtbl = cBankManager_vtbl;
    self->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483AB8_BB10;
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblBB10 vt = D_00483AB8_BB10;
        self->BankMonitor.mpVSoundSys->mVtbl = (void**)&vt;
        char* base = (char*)self->BankMonitor.mpVSoundSys - sizeof(cBankManager);
        int soundSysAdjust = (char*)self - base;
        vt.e[1].delta = D_00483AB8_BB10.e[1].delta + soundSysAdjust;
    }
    gpBankManager = 0;
    if (self->mBank != 0) {
        cBankInstance* bankInstance = self->mBank + ((int*)self->mBank)[-4];
        while (self->mBank != bankInstance) {
            bankInstance--;
            cBankInstance__cBankInstance(bankInstance, 0);
        }
        cMemMan_free((char*)self->mBank - 0x10);
    }
    cBankMonitor__cBankMonitor(&self->BankMonitor, 0);
    cAsyncSys__cAsyncSys((self), 0);
    if (flags & 2) {
        cSoundSys__cSoundSys(self->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_SyncLoad);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" void cBankInstance_SyncLoad(void* bank, void* id, int a3);

/**
 * Loads bank file @p id into bank slot @p i now, unless that slot already holds it.
 *
 * The slots are 0x60-byte bank instances in the array at +0xACC. When cBankInstance_IsBankLoaded says slot @p i is not loaded from
 * @p id, it cancels any queued asynchronous load of the slot (cAsyncSys_ASYNCSYS_Cancel) and loads it synchronously
 * with cBankInstance_SyncLoad. Callers pass the cBankManager reached through cMusicSys's mpVStreamSys
 * (cSSXAudio_InGameLoad, cSSXAudio_FrontEndLoad, func_0029F3F8, func_0028F700).
 *
 * @param i  bank slot index
 * @param id  bank file name
 * @param mainMemFlag  passed to cBankInstance_SyncLoad (callers pass 0)
 */
void cBankManager::BANK_SyncLoad(int i, void* id, int mainMemFlag)
{
    int off = i * 0x60;
    if (((cBankInstance*)((char*)mBank + off))->IsBankLoaded(id) == 0) {
        cAsyncSys_ASYNCSYS_Cancel(this, i);
        cBankInstance_SyncLoad((char*)mBank + off, id, mainMemFlag);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_UnLoad);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(void*);

/**
 * Unloads bank slot @p i: calls cBankInstance_Unload on the 0x60-byte bank instance @p i of the array at +0xACC.
 *
 * @param i  bank slot index
 */
void cBankManager::BANK_UnLoad(int i)
{
    cBankInstance_Unload(&mBank[i]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_AsyncLoad);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" void cAsyncSys_ASYNCSYS_QueueLoad(void* self, int i, void* id, int a3, int a4);


/**
 * Queues an asynchronous load of bank file @p id into bank slot @p i, unless that slot already holds it.
 *
 * When cBankInstance_IsBankLoaded says slot @p i (a 0x60-byte bank instance of the array at +0xACC) is not loaded from @p id, it
 * cancels any queued load of the slot (cAsyncSys_ASYNCSYS_Cancel) and calls cAsyncSys_ASYNCSYS_QueueLoad with the
 * slot's allocation location (+0x8 of the instance) and @p mainMemFlag. Called from func_002B43D8 (with 0x100) and
 * func_0029B818.
 *
 * @param i  bank slot index
 * @param id  bank file name
 * @param mainMemFlag  main-memory allocation flags for the load
 */
void cBankManager::BANK_AsyncLoad(int i, void* id, int mainMemFlag)
{
    if (mBank[i].IsBankLoaded(id) == 0) {
        cAsyncSys_ASYNCSYS_Cancel(this, i);
        cAsyncSys_ASYNCSYS_QueueLoad(this, i, id, mBank[i].mAllocLocation, mainMemFlag);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_AllocMem);
#ifdef SKIP_ASM
extern "C" void* cBankInstance_AllocMem(cBankInstance* inst, int a, int b, int c);

/**
 * Allocates memory for bank slot @p i: forwards to cBankInstance_AllocMem on the 0x60-byte bank instance @p i of the
 * array at +0xACC and returns its result.
 *
 * cSSXAudio_InGameLoad uses it to reserve slots 3 (0x19000 bytes) and 12 (0x6A400 bytes) of main memory.
 *
 * @param i  bank slot index
 * @param mainMemSize  bytes of main memory
 * @param soundMemSize  bytes of sound memory
 * @param memFlags  allocation flags
 * @return cBankInstance_AllocMem's result
 */
void* cBankManager::BANK_AllocMem(int i, int mainMemSize, int soundMemSize, int memFlags)
{
    return mBank[i].AllocMem(mainMemSize, soundMemSize, memFlags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_DeallocMem);
#ifdef SKIP_ASM
extern "C" void cBankInstance_DeallocMem(cBankInstance*);

/**
 * Frees the memory of bank slot @p i: calls cBankInstance_DeallocMem on the 0x60-byte bank instance @p i of the array at +0xACC.
 *
 * cSSXAudio_InGamePurge uses it for slots 3 and 12, which cSSXAudio_InGameLoad allocated with cBankManager_BANK_AllocMem.
 *
 * @param i  bank slot index
 */
void cBankManager::BANK_DeallocMem(int i)
{
    mBank[i].DeallocMem();
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_ShareBank);
#ifdef SKIP_ASM
extern "C" void cBankInstance_ShareBank(cBankInstance* dst, cBankInstance* src);

/**
 * Makes bank slot @p i share the memory of bank slot @p j: calls cBankInstance_ShareBank with those two 0x60-byte bank instances
 * of the array at +0xACC.
 *
 * @param i  bank slot that shares
 * @param j  bank slot whose memory is shared
 */
void cBankManager::BANK_ShareBank(int i, int j)
{
    mBank[i].ShareBank(&mBank[j]);
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_UnshareBank);
#ifdef SKIP_ASM
/**
 * Takes bank slot @p i out of its sharing ring: calls cBankInstance_UnshareBank on the 0x60-byte bank instance @p i of the array at
 * +0xACC.
 *
 * @param i  bank slot index
 */
void cBankManager::BANK_UnshareBank(int i)
{
    mBank[i].UnshareBank();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_Attach);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Attach(cBankInstance* inst, void* a, int b);

/**
 * Attaches a bank image in main memory to bank slot @p i: calls cBankInstance_Attach on the 0x60-byte bank instance @p i of
 * the array at +0xACC.
 *
 * Called by cScriptSoundBankManager_LinkBank and cSSXAudio_AttachAudioBank.
 *
 * @param i  bank slot index
 * @param mainMemData  address of the bank image
 * @param mainMemSize  size of the image in bytes
 */
void cBankManager::BANK_Attach(int i, void* mainMemData, int mainMemSize)
{
    cBankInstance_Attach(&mBank[i], mainMemData, mainMemSize);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_UnAttach);
#ifdef SKIP_ASM
extern "C" void cBankInstance_UnAttach(cBankInstance*);

/**
 * Detaches the bank image from bank slot @p i: calls cBankInstance_UnAttach on the 0x60-byte bank instance @p i of the array at
 * +0xACC.
 *
 * Called by cScriptSoundBankManager_UnlinkBank and cSSXAudio_UnAttachAudioBank.
 *
 * @param i  bank slot index
 */
void cBankManager::BANK_UnAttach(int i)
{
    mBank[i].UnAttach();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_ASYNCSYS_OnBeginAsyncLoad);
#ifdef SKIP_ASM
extern "C" void cBankInstance_OnBeginAsyncLoad(cBankInstance*);

/**
 * Async-loader callback for the start of a bank load: forwards it to that bank's instance.
 *
 * Calls cBankInstance_OnBeginAsyncLoad (On Tour cBankInstance::OnBeginAsyncLoad) on mBank[handle] (array at +0xACC, 0x60-byte records).
 * Slot 2 of the cAsyncSys interface vtable D_00483728.
 *
 * @param i  the user handle given to the loader: the bank number
 */
void cBankManager::ASYNCSYS_OnBeginAsyncLoad(int i)
{
    mBank[i].OnBeginAsyncLoad();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_ASYNCSYS_OnAsyncMainMemAlloc);
#ifdef SKIP_ASM
extern "C" void cBankInstance_OnAsyncMainMemAlloc(void* inst, int a, int b);

/**
 * Async-loader callback that allocates a bank's main-memory buffer: forwards it to that bank's instance.
 *
 * Calls cBankInstance_OnAsyncMainMemAlloc on mBank[handle] (+0xACC, 0x60-byte records) with the size and memory flags.
 * On Tour returns the buffer; the C is declared void, so the pointer only passes back in $v0. Slot 3 of the cAsyncSys
 * interface vtable D_00483728.
 *
 * @param i  the user handle given to the loader: the bank number
 * @param size  the buffer size in bytes
 * @param memflags  the allocation flags
 */
void cBankManager::ASYNCSYS_OnAsyncMainMemAlloc(int i, int size, int memflags)
{
    cBankInstance_OnAsyncMainMemAlloc(&mBank[i], size, memflags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_ASYNCSYS_OnAsyncMainMemSize);
#ifdef SKIP_ASM
int cBankInstance_OnAsyncMainMemSize(void*);

/**
 * Async-loader callback asking for the size of a bank's main-memory buffer: forwards it to that bank's instance.
 *
 * Calls cBankInstance_OnAsyncMainMemSize__FPv (On Tour cBankInstance::OnAsyncMainMemSize) on mBank[handle] (+0xACC, 0x60-byte records); the C
 * is declared void, so the size only passes back in $v0. Slot 4 of the cAsyncSys interface vtable D_00483728.
 *
 * @param i  the user handle given to the loader: the bank number
 */
void cBankManager::ASYNCSYS_OnAsyncMainMemSize(int i)
{
    mBank[i].OnAsyncMainMemSize();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_ASYNCSYS_OnEndAsyncLoad);
#ifdef SKIP_ASM
extern "C" void cBankInstance_OnEndAsyncLoad(cBankInstance* inst, int a, int b, const char* c, void* d, int e);

/**
 * Async-loader callback for the end of a bank load: forwards the outcome to that bank's instance.
 *
 * Calls cBankInstance_OnEndAsyncLoad (On Tour cBankInstance::OnEndAsyncLoad) on mBank[handle] (+0xACC, 0x60-byte records) with the
 * other five arguments unchanged. Slot 5 of the cAsyncSys interface vtable D_00483728.
 *
 * @param i  the user handle given to the loader: the bank number
 * @param soundHandle  the sound handle the load produced
 * @param endState  the end state (On Tour eAsyncSysEndState; 0 = success)
 * @param pFilename  the file name
 * @param pBuffer  the load buffer
 * @param dataSize  the number of bytes loaded
 */
void cBankManager::ASYNCSYS_OnEndAsyncLoad(int i, int soundHandle, int endState, const char* pFilename,
                                           void* pBuffer, int dataSize)
{
    cBankInstance_OnEndAsyncLoad(&mBank[i], soundHandle, endState, pFilename, pBuffer, dataSize);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028BF78);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cAudioInterface_isInGame(void);
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(cSSXAudio* self);
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002B4AF0(void* monitor);
extern "C" void cSSXAudio_SetDJSpeech(void* self, int on);
// PORT: func_002B4070 takes the 64-bit bank mask as a second argument (64-bit `long`).
extern "C" void func_002B4070_BF78(void* monitor, long mask) __asm__("func_002B4070");

struct sCfg_BF78 {
    int flags;
    int v[(0x288 - 4) / 4];
};
extern sCfg_BF78 cBEOptionInterface_mOption_BF78 __asm__("cBEOptionInterface_mOption");

/**
 * Switches the radio mode (mRadioModeID) to @p state and restarts the music when the current song no longer fits it.
 *
 * Does nothing when mRadioModeID already equals @p state; otherwise stores it. cSSXAudio_InGameInit passes
 * cBERewardInterface_getRadioMode's result (eRadioModeID); the audio options menu passes its selection.
 *
 * kRADIOMODE_RADIO activates the songs of MusicSys.mDefaultActiveMap (func_002B4070), re-applies the arcade SFX setting
 * from bit 17 of cBEOptionInterface_mOption's flags (cSSXAudio_SetDJSpeech), and, in game with MusicSys.mSongPlaying
 * set and a current song tag (func_002B49E0) of kSONGTAG_Peak1Amb-Peak3Amb (a peak ambience), picks a new song
 * (func_0028D488) and restarts with func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0). kRADIOMODE_REQUESTLINE and
 * kRADIOMODE_PLAYLIST do the same with mRadioPlayList, returning early when that is 0, when the song tag is a peak
 * theme (kSONGTAG_Peak1-Peak3), or when the current song is still active (func_002B4AF0) and its tag is not
 * kSONGTAG_CharSel. kRADIOMODE_MOUNTAIN turns the arcade SFX off and, in game unless the song is already a peak
 * ambience, calls func_0028CF98 the same way (which starts the peak ambience in this mode) and clears mInBigChallenge
 * and mCurBigChallengeType.
 *
 * The option block is copied whole (0x288 bytes) to the stack just to read its flags.
 *
 * @param state  the new radio mode
 */
extern "C" void func_0028BF78(cSSXAudio* self, int state)
{
    if (self->mRadioModeID == state)
        return;
    self->mRadioModeID = state;
    switch (state) {
    case kRADIOMODE_RADIO: {
        func_002B4070_BF78(&self->MusicSys, *(long*)&self->MusicSys.mDefaultActiveMap);
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = cBEOptionInterface_mOption_BF78;
        cSSXAudio_SetDJSpeech(self, (cfg.flags >> 17) & 1);
        if (cAudioInterface_isInGame() == 0)
            return;
        if (self->MusicSys.mSongPlaying == 0)
            return;
        if ((unsigned int)(func_002B49E0(&self->MusicSys) - kSONGTAG_Peak1Amb) >= 3)
            return;
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
        break;
    }
    case kRADIOMODE_REQUESTLINE:
    case kRADIOMODE_PLAYLIST: {
        func_002B4070_BF78(&self->MusicSys, *(long*)&self->mRadioPlayList);
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = cBEOptionInterface_mOption_BF78;
        cSSXAudio_SetDJSpeech(self, (cfg.flags >> 17) & 1);
        if (*(long*)&self->mRadioPlayList == 0)
            return;
        if (cAudioInterface_isInGame() == 0)
            return;
        if (self->MusicSys.mSongPlaying == 0)
            return;
        int st = func_002B49E0(&self->MusicSys);
        if ((unsigned int)(st - kSONGTAG_Peak1) < 3)
            return;
        if (func_002B4AF0(&self->MusicSys) != 0 && st != kSONGTAG_CharSel)
            return;
        func_0028D488(self);
        func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0);
        break;
    }
    case kRADIOMODE_MOUNTAIN:
        cSSXAudio_SetDJSpeech(self, 0);
        if (cAudioInterface_isInGame() == 0)
            return;
        if ((unsigned int)(func_002B49E0(&self->MusicSys) - kSONGTAG_Peak1Amb) < 3)
            return;
        func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0);
        self->mInBigChallenge = 0;
        self->mCurBigChallengeType = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C2D0);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(cSSXAudio* self);
extern "C" void func_002B4070(void* monitor);
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002B4AF0(void* monitor);

static inline void bmRestartC2D0(cSSXAudio* self)
{
    if (cAudioInterface_isInGame() == 0) {
        return;
    }
    if (self->MusicSys.mSongPlaying == 0) {
        return;
    }
    cMusicSys* mon = &self->MusicSys;
    int st = func_002B49E0(mon);
    if ((unsigned int)(st - kSONGTAG_Peak1) < 3) {
        return;
    }
    if (func_002B4AF0(mon) != 0 && st != kSONGTAG_CharSel) {
        return;
    }
    func_0028D488(self);
    func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0);
}

/**
 * Sets the radio playlist mask mRadioPlayList to @p key and, in the request-line and playlist radio modes, restarts the
 * music from it.
 *
 * When @p key differs from mRadioPlayList it is stored, and in kRADIOMODE_REQUESTLINE and kRADIOMODE_PLAYLIST
 * (mRadioModeID) func_002B4070 rebuilds the active songs from it (the mask reaches it in the second argument register;
 * the call is declared with one argument). In those modes, changed or not, it then does what func_0028BF78 does for
 * those modes: in game with MusicSys.mSongPlaying set, unless the current song tag (func_002B49E0) is a peak theme
 * (kSONGTAG_Peak1-Peak3) or the current song is still active (func_002B4AF0) with a tag other than kSONGTAG_CharSel, it
 * picks a new song (func_0028D488) and calls func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0). Called by
 * cSSXAudio_InGameInit and cFEStateRequestLine_onWidgetEvent.
 *
 * @param key  the new playlist mask, one bit per song (64-bit)
 */
// PORT: 64-bit `long` key.
extern "C" void func_0028C2D0(cSSXAudio* self, long key)
{
    if (*(long*)&self->mRadioPlayList != key) {
        *(long*)&self->mRadioPlayList = key;
        int state = self->mRadioModeID;
        if (state == kRADIOMODE_REQUESTLINE || state == kRADIOMODE_PLAYLIST) {
            func_002B4070(&self->MusicSys);
            bmRestartC2D0(self);
        }
    } else {
        int state = self->mRadioModeID;
        if (state == kRADIOMODE_REQUESTLINE || state == kRADIOMODE_PLAYLIST) {
            bmRestartC2D0(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C430);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void func_002B2850(void* a, void* mon);


struct sDesc_C430 {
    char pad0[0x70];
    int count;          // 0x70
    char pad74[0x8C - 0x74];
    int volume;         // 0x8C
};
struct sObj_C430 {
    sDesc_C430* desc;   // 0x00
    char pad4[0x34 - 4];
    int kind;           // 0x34
};
struct sCue_C430 {
    char pad0[4];
    int f4;             // 0x04
    char pad8[0x18 - 8];
    sObj_C430* obj;     // 0x18
    char* data;         // 0x1C
    int idx;            // 0x20
    int f24;            // 0x24
};

// The inlined SFX_Begin: push a play-option level on the shared cBankManager's cBankMonitor and record its bank /
// patch.
static inline void bmPush_C430(cSSXAudio* audio, int kind, int data)
{
    cBankManager* m = audio->mpVBankManager;
    cBankMonitor* st = &m->BankMonitor;
    m->BankMonitor.mInst++;
    st->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    st->mPosStatic[m->BankMonitor.mInst] = 0;
    st->mBaseVol[m->BankMonitor.mInst] = st->mCurrent[m->BankMonitor.mInst].vol;
    st->mVolControl[m->BankMonitor.mInst] = 0;
    memset((char*)st + m->BankMonitor.mInst * 8 + 0x40, 0, 8);
    st->mPtr[m->BankMonitor.mInst] = 0;
    st->mViewport[m->BankMonitor.mInst] = 0;
    st->mVanish[m->BankMonitor.mInst] = 0;
    st->mRadius[m->BankMonitor.mInst] = 0;
    st->mDelay[m->BankMonitor.mInst].mCount = 0;
    st->mDelay[m->BankMonitor.mInst].mTime = 100;
    st->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    st->mDelay[m->BankMonitor.mInst].mLevel = 50;
    st->mExternalVol[m->BankMonitor.mInst] = 0;
    st->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    st->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    st->mMapToMaxVol[m->BankMonitor.mInst] = 127;
    st->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    audio->mBank[audio->mpVBankManager->BankMonitor.mInst] = kind;
    audio->mPatch[audio->mpVBankManager->BankMonitor.mInst] = data;
}

#define BM_MIN_C430(a, b) ((a) < (b) ? (a) : (b))
#define BM_MAX_C430(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Plays one beat of the current song's loop bank: cSSXAudio's override of cMusicSys virtual slot 1, which cMusicSys
 * leaves pure (word 3 of cSSXAudio's cMusicSys vtable D_00483688).
 *
 * The cMusicSys timed callback func_002B4540 forwards to it with the cMonitorInstance that func_002B4568 created: mPtr
 * the cSongInstance, mData1 a random phrase start (func_002B2500), mData2 the beat index, mData3 the beat length in ms.
 * With the song's cSongConfiguration it wraps mData2 to 0 at mBeatsPerPhrase (and plays nothing when that is <= 0),
 * pushes a play-option level on the cBankManager (the inlined SFX_Begin: bank = the song's unk_34, patch = mData1 +
 * mData2, delay defaults 100, 0, 90, 50), points the level's volume control at unk_60A0, scales its volume by
 * mAsynclevel / 100 (clamped to 0..127), copies the song's delay settings in with func_002B2850 and plays it with
 * cBankSys_SFX_End. It then increments mData2 and sets mCountReset to mData3, so the monitor calls back one beat later.
 *
 * @param audio  the cSSXAudio (this, adjusted from its cMusicSys base)
 * @param cue  the cMonitorInstance that fired
 * @return 0
 */
extern "C" int func_0028C430(cSSXAudio* audio, sCue_C430* cue)
{
    int* pidx = &cue->idx;
    char* data = cue->data;
    int saved = cue->f24;
    sObj_C430* obj = cue->obj;
    sDesc_C430* desc = obj->desc;
    int kind = obj->kind;
    int ivol = desc->volume;
    int count = desc->count;
    if (cue->idx >= count) {
        cue->idx = 0;
        if (count <= 0)
            goto end;
    }
    {
        float vol = (float)ivol;
        int idx = cue->idx;
        while (idx < 0)
            idx += count;
        bmPush_C430(audio, kind, (int)(data + idx));
        cStreamSys* streamSys = audio->MusicSys.mpVStreamSys;
        streamSys->mpVBankManager->BankMonitor.mVolControl[streamSys->mpVBankManager->BankMonitor.mInst] =
            &audio->unk_60A0;
        cBankManager* bankManager = audio->MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* st2 = &bankManager->BankMonitor;
        st2->mBaseVol[bankManager->BankMonitor.mInst] =
            (int)BM_MAX_C430(BM_MIN_C430(st2->mBaseVol[st2->mInst] * (vol * 0.009999999776482582f), 127.0f), 0.0f);
        st2->mCurrent[st2->mInst].vol = BM_MAX_C430(BM_MIN_C430(st2->mBaseVol[st2->mInst], 127), 0);
        func_002B2850(audio->MusicSys.mSongInstance, &audio->MusicSys);
        cBankSys_SFX_End(audio);
        (*pidx)++;
        cue->f4 = saved;
    }
end:
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C8C0__FPvi);
#ifdef SKIP_ASM
/**
 * Sets unk_6250 to @p val.
 *
 * func_0028C8C8 counts unk_6250 down by one per call while it is positive; no retail code reads it otherwise. The only
 * caller (main/gamestate) passes 120.
 *
 * @param val  the new count
 */
void func_0028C8C0(void* vself, int val)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    self->unk_6250 = val;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C8C8);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
extern "C" cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
int cRider_getMotionState(void* rider);
int func_0028D898(void* self);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" bool func_0028D960(cSSXAudio* self);
extern "C" void cSSXAudio_BeginWind(void* self);
extern "C" void cSSXAudio_EndWind(void* self);
extern "C" int func_002A4168(void* self);
extern "C" int func_002A41D8(void* self);
extern "C" void func_002B3C98(void* mon, char v);
extern "C" void func_002B3D10(void* mon, char v);
extern "C" int func_002B4620(void* mon);
extern "C" int func_002B46E8(void* mon);
extern "C" void func_002B4708(void* mon);
extern "C" int func_002B4878(void* mon);
extern int D_004A361C;
extern int D_004A3620;

struct sBmVtC8C8 { short delta; short index; void (*fn)(void*, int); };

static inline float bmAirTime_C8C8(cSSXAudio* self, cRider* rider)
{
    int s = rider->mAirPredictor->SimState;
    int air = 0;
    if (s == cAirPredictor::kSS_SimDone || s == cAirPredictor::kSS_SimCheckObjectsDone)
        air = 1;
    if (air) {
        if (D_004A361C == true)
            return self->unk_6244;
        float v = rider->mAirPredictor->TotalAirTime;
        self->unk_6248 = 0.0f;
        self->unk_6244 = v;
        return v;
    }
    if (D_004A361C == true)
        return self->unk_6244;
    return 0.0f;
}

/**
 * Updates the wind sound and the music volume for the focus rider's air time; called every frame from cSSXAudio_Update.
 *
 * Counts unk_6250 down. For the focus rider (cSSXAudio_GetFocusRider(-1)) in motion state MS_Air with its
 * mAirPredictor's SimState kSS_SimDone or kSS_SimCheckObjectsDone, the air time is the predictor's TotalAirTime, cached
 * in unk_6244 on the first frame (unk_6248 cleared); while D_004A361C is set the cached value is reused; otherwise it
 * is 0. unk_60A0 is set to the music volume (cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0)), and D_004A3620 takes last
 * frame's D_004A361C, which is cleared. Above 3.9995 s, when the song's mDuckToLoops (func_002B4878) is set,
 * func_002A4168 and func_002A41D8 are false and no rider event runs (func_0028D898), the song is driven too (local
 * play). Outside replays, above 2 s: D_004A361C = 1; unk_6248 keeps the largest current air time (CurAirTime), and
 * unk_624C, set on the first frame, is that plus max((ApexTime - CurAirTime) / 2, 0.5). From their ratio and a volume
 * falling with the air time (20..127) it sets mWindVolume (then scaled by the SFX volume, control 5) and, with play,
 * the song's base volume (func_002B3C98) and overlay level (unk_60A0 via func_002B3D10). On the first such frame it
 * starts the wind (cSSXAudio_BeginWind) and, with play, func_002B4620 true and the radio on (func_0028D960), sends
 * music event 7 through MusicSys.mVtbl slot 3. Otherwise, after an airborne frame, it ends the wind
 * (cSSXAudio_EndWind), sends event 8 when func_002B46E8 reports the song's unk_4C set (and the radio is on) and clears
 * it with func_002B4708; then it restores base volume 127 and the overlay level.
 */
extern "C" void func_0028C8C8(cSSXAudio* self)
{
    if (self->unk_6250 > 0)
        self->unk_6250--;
    cRider* rider = cSSXAudio_GetFocusRider_p(self, -1);
    float airTime;
    if (cRider_getMotionState(rider) == MS_Air) {
        int simState = rider->mAirPredictor->SimState;
        int air = 0;
        if (simState == cAirPredictor::kSS_SimDone || simState == cAirPredictor::kSS_SimCheckObjectsDone)
            air = 1;
        if (air) {
            if (D_004A361C == true) {
                airTime = self->unk_6244;
            } else {
                airTime = rider->mAirPredictor->TotalAirTime;
                self->unk_6248 = 0.0f;
                self->unk_6244 = airTime;
            }
            goto have;
        }
        if (D_004A361C == true) {
            airTime = self->unk_6244;
            goto have;
        }
    }
    airTime = 0.0f;
have:
    int play = 0;
    self->unk_60A0 = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
    D_004A3620 = D_004A361C;
    D_004A361C = 0;
    if (3.9994986057281494f < airTime) {
        int duckToLoops = func_002B4878(&self->MusicSys);
        if (duckToLoops == true && !func_002A4168(self) && !func_002A41D8(self)) {
            if (!func_0028D898(self))
                play = duckToLoops;
        }
    }
    if (cSSXAudio_ReplayMode(self) == 0) {
        if (2.0f < airTime) {
            D_004A361C = 1;
            float lo = rider->mAirPredictor->CurAirTime;
            if (lo < self->unk_6248)
                lo = self->unk_6248;
            self->unk_6248 = lo;
            float hi;
            if (D_004A3620 == 0) {
                hi = (rider->mAirPredictor->ApexTime - rider->mAirPredictor->CurAirTime) * 0.5f;
                if (hi < 0.5f)
                    hi = 0.5f;
                hi = hi + lo;
                self->unk_624C = hi;
            } else {
                hi = self->unk_624C;
            }
            float vmax = 127.0f;
            float inv = 0.007874015718698502f;
            float vol = ((1.0f - airTime * 0.8425197005271912f * 0.5f) * vmax <? vmax) >? 20.0f;
            if (lo < hi) {
                float ratio = lo / hi;
                int full = 127;
                float rawVolume = vmax - ratio * 107.0f;
                char clampedVolume = (int)((rawVolume <? vmax) >? 20.0f);
                if (play) {
                    float k = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
                    int volumeAboveMin = clampedVolume - 20;
                    float musicLevel = (float)(full - volumeAboveMin) * (k * inv);
                    self->unk_60A0 = musicLevel;
                    func_002B3D10(&self->MusicSys, (int)(musicLevel * 100.0f));
                }
                rawVolume = vmax - (vmax - vol) * ratio;
                clampedVolume = (int)((rawVolume <? vmax) >? vol);
                self->mWindVolume = (float)(full - clampedVolume) * inv;
                if (play)
                    func_002B3C98(&self->MusicSys, clampedVolume);
            } else {
                self->mWindVolume = (vmax - vol) * inv;
                if (play) {
                    func_002B3C98(&self->MusicSys, (int)vol);
                    float k = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
                    self->unk_60A0 = k;
                    func_002B3D10(&self->MusicSys, (int)(k * 100.0f));
                }
            }
            self->mWindVolume = self->mWindVolume * *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
            if (D_004A3620 == 0) {
                cSSXAudio_BeginWind(self);
                if (play) {
                    char* mon = (char*)&self->MusicSys;
                    if (func_002B4620(mon) && func_0028D960((cSSXAudio*)self)) {
                        sBmVtC8C8* musicVt = (sBmVtC8C8*)self->MusicSys.mVtbl;
                        musicVt[3].fn(vthis(mon, musicVt[3]), 7);
                    }
                }
            }
            return;
        }
    }
    char* mon = (char*)&self->MusicSys;
    if (D_004A3620 == true) {
        cSSXAudio_EndWind(self);
        if (func_002B46E8(mon)) {
            if (func_0028D960((cSSXAudio*)self)) {
                sBmVtC8C8* musicVt = (sBmVtC8C8*)self->MusicSys.mVtbl;
                musicVt[3].fn(vthis(mon, musicVt[3]), 8);
            }
            func_002B4708(mon);
        }
    }
    func_002B3C98(mon, 0x7F);
    func_002B3D10(mon, (int)(self->unk_60A0 * 100.0f));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028CD48);
#ifdef SKIP_ASM
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void func_002B49C0(void* self, int a1);
extern "C" void func_002B3C28(void* self, int a1);
extern char str_pktrans[];

struct sBmVtCD48 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadCD48)(void*, void*, int, void*, int);
typedef int (*tBmSetCD48)(void*, int);

/**
 * Starts the "pktrans" music: cGFTrackStartState_preloadUpdate calls it while a track loads.
 *
 * Through MusicSys.mVtbl slot 2 (cSSXAudio's func_0028F478) it starts the song "pktrans" with 12, the music volume
 * control (cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0)) and 1; tags it 401 (func_002B49C0), sends music event 1 through slot 4
 * (func_0028F3C8) and starts it with func_002B3C28 (passing 0x7F, which that function's declaration does not take).
 */
extern "C" void func_0028CD48(cSSXAudio* self)
{
    char* mon = (char*)&self->MusicSys;
    sBmVtCD48* vt = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), str_pktrans, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 1);
    func_002B49C0(mon, kSONGTAG_PkTrans);
    sBmVtCD48* vt2 = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), 1);
    func_002B3C28(mon, 0x7F);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028CDF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_002B4A08(void* self, int a1);
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern char str_chartune[];

struct sRace_28CDF8 {
    int mode;          // cGameModeMan::iHeatNum
    char pad4[0x14];
    int tracks[8];     // cGameModeMan::iCharacters (eBECharacters)
};

/**
 * Starts the character tune for the podium (cGFPodiumState_gainFocus).
 *
 * Does nothing in a replay (cSSXAudio_ReplayMode), in kRADIOMODE_MOUNTAIN (mRadioModeID), unless SSXApp's GameModeMan
 * is in the final heat (iHeatNum cGameModeMan::kFinalHeat), or when the first entry of the rank table
 * cGameModeMan_iRanking is 3 or more. The character iCharacters[cGameModeMan_iCharRank[0]] (kCHAR_MOBY to kCHAR_VIGGO)
 * selects music event 10, 2, 8, 1, 5, 9, 3, 7, 4 or 6; other values do nothing. It stores MusicSys.mCurrentSongNum in
 * mSavedSongNum, starts the song "chartune" through MusicSys.mVtbl slot 2 (12, the music volume control, 0), tags it
 * kSONGTAG_CharTune (func_002B49C0), sends the event through slot 4, sets the song's unk_5C (func_002B4A08) so that
 * only forced events apply, and starts it with func_002B3C28.
 */
extern "C" void func_0028CDF8(cSSXAudio* self)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    if (self->mRadioModeID == kRADIOMODE_MOUNTAIN) {
        return;
    }
    sRace_28CDF8* race = (sRace_28CDF8*)SSXApp->GameModeMan;
    if (race->mode != cGameModeMan::kFinalHeat) {
        return;
    }
    if (cGameModeMan_iRanking[0] >= 3) {
        return;
    }
    int track = -1;
    switch (race->tracks[cGameModeMan_iCharRank[0]]) {
    case kCHAR_MOBY: track = 10; break;
    case kCHAR_KAORI: track = 2; break;
    case kCHAR_ALLEGRA: track = 8; break;
    case kCHAR_MAC: track = 1; break;
    case kCHAR_ZOE: track = 5; break;
    case kCHAR_GRIFF: track = 9; break;
    case kCHAR_ELISE: track = 3; break;
    case kCHAR_NATE: track = 7; break;
    case kCHAR_PSYMON: track = 4; break;
    case kCHAR_VIGGO: track = 6; break;
    }
    if (track < 0) {
        return;
    }
    char* mon = (char*)&self->MusicSys;
    self->mSavedSongNum = self->MusicSys.mCurrentSongNum;
    sBmVtCD48* vt = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), str_chartune, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    func_002B49C0(mon, kSONGTAG_CharTune);
    sBmVtCD48* vt2 = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), track);
    func_002B4A08(mon, 1);
    func_002B3C28(mon, 0x7F);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028CF98);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getBE();
extern "C" void func_0028F108(void* self, int a1);
extern "C" int func_002A1E20(void* self, int id);
extern "C" int func_002A42C8(void* self);
extern "C" void func_002B3CC0(void* mon, int a1);
extern "C" void func_002B46D0(void* mon);
int func_002B4908(void* self);
extern char str_Peak1Amb[];
extern char str_Peak2Amb[];
extern char str_Peak3Amb[];
extern char str_Peak1_b[];
extern char str_Peak2_b[];
extern char str_Peak3_b[];



static inline void bmLoad_CF98(cSSXAudio* self, char* name, int bank, int id)
{
    char* mon = (char*)&self->MusicSys;
    sBmVtCD48* vt = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), name, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, bank, 0), 0);
    func_002B49C0(mon, id);
}

/**
 * Starts the music for the current radio mode and race location.
 *
 * Returns at once in a replay (cSSXAudio_ReplayMode) unless unk_6294 is set. Clears unk_6280 and the rider-event words
 * at 0x623C and 0x6240 (func_0028D630) and sets mMusicEventOwner to kNoMusicEventOwner. Songs are started through
 * MusicSys.mVtbl slot 2 with 12, a volume control (cSSXAudio_GetCurrAppliedVolumePtr) and 0, tagged with func_002B49C0,
 * and given events through slot 4.
 *
 * In kRADIOMODE_MOUNTAIN (mRadioModeID) it starts the ambience "Peak1Amb", "Peak2Amb" or "Peak3Amb" by @p type, or,
 * when that is kPEAKBIT_FromLocation, by func_002A1E20 of the race location (kPEAKBIT_Peak1, kPEAKBIT_Peak2, anything
 * else), on the ambient control (8) and tagged kSONGTAG_Peak1Amb-Peak3Amb; then func_002B46D0, sets unk_6280 to 1 and
 * calls func_002B3CC0(0) and func_002B3C28. Else, at a station (kLOCATION_HUB_A or above) with @p a2 and @p requestLine
 * zero and func_002A42C8 false, it starts the theme "Peak1", "Peak2" or "Peak3" (by the location) on the music control,
 * tagged kSONGTAG_Peak1-Peak3, func_002B3CC0(1), event 12 and func_002B3C28. Otherwise it starts the current song of
 * the active list (null name), with MusicSys.mCurrentSongNum saved in unk_62A4 and the song's SEDVALUE (func_002B4908)
 * in mSongAnnounceNum, func_002B3CC0(1), event @p startEvent (36 when @p requestLine is set at a station), then
 * func_0028F108 with the focus rider (cSSXAudio_GetFocusRider(-1)).
 *
 * @param startEvent  music event sent to the playlist song (callers pass 0 or 36)
 * @param skipPeakTheme  non-zero skips the peak theme (func_0028E100 passes 1)
 * @param type  peak for kRADIOMODE_MOUNTAIN (an ePeakBit), or kPEAKBIT_FromLocation to take it from the location
 * @param requestLine  non-zero skips the peak theme and selects event 36 on the peaks; cFEStateRequestLine_onWidgetMenu passes 1
 */
extern "C" void func_0028CF98(cSSXAudio* self, int startEvent, int skipPeakTheme, int type, int requestLine)
{
    if (cSSXAudio_ReplayMode(self) != 0 && self->unk_6294 == 0) {
        return;
    }
    self->mPathfinderCanChangeVolume = 0;
    self->mMusicEventOwner = kNoMusicEventOwner;
    self->unk_623C = 0;
    self->unk_6240 = 0;
    if (self->mRadioModeID == kRADIOMODE_MOUNTAIN) {
        int peakNumber = type == kPEAKBIT_FromLocation
            ? func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)))
            : type;
        if (peakNumber == kPEAKBIT_Peak1) {
            bmLoad_CF98(self, str_Peak1Amb, 8, kSONGTAG_Peak1Amb);
        } else if (peakNumber == kPEAKBIT_Peak2) {
            bmLoad_CF98(self, str_Peak2Amb, 8, kSONGTAG_Peak2Amb);
        } else {
            bmLoad_CF98(self, str_Peak3Amb, 8, kSONGTAG_Peak3Amb);
        }
        char* mon = (char*)&self->MusicSys;
        func_002B46D0(mon);
        self->mPathfinderCanChangeVolume = 1;
        func_002B3CC0(mon, 0);
        func_002B3C28(mon, 0x7F);
        return;
    }
    tBELocationDB info = *(tBELocationDB*)cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    char* mon = (char*)&self->MusicSys;
    if (info.locationID >= kLOCATION_HUB_A && skipPeakTheme == 0 && requestLine == 0 && func_002A42C8(self) == 0) {
        int peakNumber = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (peakNumber == kPEAKBIT_Peak1) {
            bmLoad_CF98(self, str_Peak1_b, 1, kSONGTAG_Peak1);
        } else if (peakNumber == kPEAKBIT_Peak2) {
            bmLoad_CF98(self, str_Peak2_b, 1, kSONGTAG_Peak2);
        } else {
            bmLoad_CF98(self, str_Peak3_b, 1, kSONGTAG_Peak3);
        }
        func_002B3CC0(mon, 1);
        sBmVtCD48* vt2 = (sBmVtCD48*)self->MusicSys.mVtbl;
        ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), 0xC);
        func_002B3C28(mon, 0x7F);
        return;
    }
    sBmVtCD48* vt = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), 0, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    self->unk_62A4 = self->MusicSys.mCurrentSongNum;
    self->mSongAnnounceNum = func_002B4908(mon);
    func_002B3CC0(mon, 1);
    if (requestLine != 0 && info.locationID >= 17) {
        startEvent = 36;
    }
    sBmVtCD48* vt2 = (sBmVtCD48*)self->MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), startEvent);
    func_0028F108(self, cSSXAudio_GetFocusRider(self, -1));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D488);
#ifdef SKIP_ASM
extern "C" unsigned int cSoundSys_SndRand(void* rng);
extern "C" int func_0028D8A0(cSSXAudio* self);
extern "C" void func_002B42B8(void* q, int a, int b);

/**
 * Picks a new random song from the active list, avoiding the one func_0028CF98 last started.
 *
 * Sets MusicSys.mCurrentSongNum to a random index (cSoundSys_SndRand) scaled to mNumFESongs, then steps it 1 to 11 times
 * (random) to the next active song of the category func_0028D8A0 picks (func_002B42B8 with the filter on). If it ends
 * on the song saved in unk_62A4, it steps once more in that category and, if still the same, once without the filter.
 *
 * @param vself  the cSSXAudio
 */
extern "C" void func_0028D488(cSSXAudio* vself)
{
    unsigned int count = (unsigned int)vself->MusicSys.mNumFESongs;
    int prev = vself->unk_62A4;
    vself->MusicSys.mCurrentSongNum = ((cSoundSys_SndRand(vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF) * count) / 0x7FFF;
    int n = ((cSoundSys_SndRand(vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF) * 10) / 0x7FFF + 1;
    int i;
    for (i = 0; i < n; i++)
        func_002B42B8(&vself->MusicSys, 1, func_0028D8A0((cSSXAudio*)(((char*)vself))));
    char* monitor = (char*)&vself->MusicSys;
    if (prev == vself->MusicSys.mCurrentSongNum) {
        func_002B42B8(monitor, 1, func_0028D8A0((cSSXAudio*)(((char*)vself))));
        if (prev == vself->MusicSys.mCurrentSongNum)
            func_002B42B8(monitor, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D5A0);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002A1E20(void* self, int id);
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);

/**
 * Restarts the music when the playing peak ambience does not belong to location @p id.
 *
 * Does nothing while mTransportTrack is kMAX_LOCATIONS (no location). Otherwise func_002A1E20 maps @p id to its peak
 * (kPEAKBIT_Peak1, kPEAKBIT_Peak2, else peak 3, expecting the song tag kSONGTAG_Peak1Amb, Peak2Amb or Peak3Amb) and,
 * when the current song tag (func_002B49E0) differs, calls func_0028CF98(self, 0, 0, peak, 0). The only caller,
 * func_0028E8C0, passes mTransportTrack.
 *
 * @param id  a race location (eBELocationID)
 */
extern "C" void func_0028D5A0(cSSXAudio* self, int id)
{
    int cur;
    int type;
    int want;
    if (self->mTransportTrack == kMAX_LOCATIONS) {
        return;
    }
    cur = func_002B49E0(&self->MusicSys);
    type = func_002A1E20(self, id);
    if (type == kPEAKBIT_Peak1) {
        want = kSONGTAG_Peak1Amb;
    } else if (type == kPEAKBIT_Peak2) {
        want = kSONGTAG_Peak2Amb;
    } else {
        want = kSONGTAG_Peak3Amb;
    }
    if (want != cur) {
        func_0028CF98((cSSXAudio*)self, 0, 0, type, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D630);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);

struct sBmVt630b { short delta; short index; int (*fn)(void*, int); };

/**
 * Starts rider music event @p id for the rider @p obj, which then owns the rider-event slot.
 *
 * Does nothing during a big challenge (mInBigChallenge or mBigChallengeStarting) or when the event in unk_623C is
 * already @p id. With no owner (mMusicEventOwner == kNoMusicEventOwner) only the focus rider
 * (cSSXAudio_GetFocusRider(-1)) can start one, and its AI index (getAIIndex, cRider virtual slot 7) becomes
 * mMusicEventOwner; otherwise @p obj must have the owner's AI index. It then sends the previous event + 1 (if one was
 * running) and @p id through MusicSys.mVtbl slot 4, stores @p id in unk_623C and clears unk_6240. Called by
 * func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
extern "C" void func_0028D630(cSSXAudio* self, int id, cRider* obj)
{
    int ok;
    if (self->mInBigChallenge != 0) return;
    if (self->mBigChallengeStarting != 0) return;
    if (self->unk_623C == id) return;
    ok = 0;
    if (self->mMusicEventOwner == kNoMusicEventOwner) {
        // PORT: cSSXAudio_GetFocusRider returns an object pointer as int
        if ((int)obj == cSSXAudio_GetFocusRider(self, -1)) {
            ok = 1;
            self->mMusicEventOwner = obj->getAIIndex();
        }
    } else {
        ok = obj->getAIIndex() == self->mMusicEventOwner;
    }
    if (ok) {
        int n = self->unk_623C;
        char* mon = (char*)&self->MusicSys;
        if (n != 0) {
            sBmVt630b* vt = (sBmVt630b*)self->MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), n + 1);
        }
        self->unk_623C = id;
        self->unk_6240 = 0;
        {
            sBmVt630b* vt = (sBmVt630b*)self->MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D740);
#ifdef SKIP_ASM
extern "C" void func_0028D7D8(void* self, void* a1);

/**
 * Ends rider music event @p id for the owning rider @p obj, or records it as the follow-up of the running one.
 *
 * Does nothing when unk_6240 already holds @p id or when @p obj is not the owner (its AI index, getAIIndex, differs
 * from mMusicEventOwner; no owner counts as not). When the running event (unk_623C) is @p id, @p id is stored in
 * unk_6240; otherwise func_0028D7D8 releases the slot. Called by func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
extern "C" void func_0028D740(cSSXAudio* self, int id, cRider* obj)
{
    int ok;
    if (self->unk_6240 == id) {
        return;
    }
    ok = 0;
    if (self->mMusicEventOwner != kNoMusicEventOwner) {
        ok = obj->getAIIndex() == self->mMusicEventOwner;
    }
    if (ok) {
        if (self->unk_623C == id) {
            self->unk_6240 = id;
            return;
        }
        func_0028D7D8(self, obj);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D7D8);
#ifdef SKIP_ASM
struct sBmVtD7D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares func_0028D7D8 as returning void; it really returns 0/1.
int func_0028D7D8_impl(cSSXAudio* self, cRider* obj) __asm__("func_0028D7D8");

/**
 * Releases the rider-event slot that func_0028D630 gave to a rider and ends its music event.
 *
 * With @p obj null, or when @p obj's AI index (getAIIndex) equals the owner mMusicEventOwner, it sets
 * mMusicEventOwner to -1; then, if an event is running (unk_623C), it sends that event + 1 through MusicSys.mVtbl
 * slot 4 and clears unk_623C and unk_6240. The unit declares it void although it returns the flag.
 *
 * @param obj  the rider (cRider*), or null to release unconditionally
 * @return 1 when the slot was released, else 0
 */
int func_0028D7D8_impl(cSSXAudio* self, cRider* obj)
{
    int ok = 0;
    if (obj == 0) {
        ok = 1;
        self->mMusicEventOwner = -1;
    } else {
        int none = -1;
        if (self->mMusicEventOwner != none) {
            if (obj->getAIIndex() == self->mMusicEventOwner) {
                self->mMusicEventOwner = none;
                ok = 1;
            }
        }
    }
    if (ok) {
        int n = self->unk_623C;
        if (n != 0) {
            char* mon = (char*)&self->MusicSys;
            sBmVtD7D8* vt = (sBmVtD7D8*)self->MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), n + 1);
            self->unk_623C = 0;
            self->unk_6240 = 0;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D898__FPv);
#ifdef SKIP_ASM
/**
 * Returns the running rider music event (the word at 0x623C that func_0028D630 sets; 0 when none).
 *
 * @return the event id, or 0
 */
int func_0028D898(void* vself)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    return self->unk_623C;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D8A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A4318(void* self);
extern "C" int func_002A4368(void* self);
extern "C" int func_002A40E0(void* self, int id);
extern "C" int func_002A4158(void* self, int id);
extern "C" int func_002A41C8(void* self, int id);
extern "C" int func_002A4238(void* self, int id);

/**
 * Returns the song category (0-4) for the race in progress.
 *
 * From cBENewRaceInterface's location and game event: 0 when func_002A4318 (game events 6-8) or func_002A40E0 (location
 * below 5) holds, 1 for func_002A4368 (events 9-11) or func_002A4158 (locations 5-7), 2 for func_002A41C8 (8-10), 3 for
 * func_002A4238 (11-13), else 4; the event tests come first. func_0028D488 filters songs by it (mCategory).
 *
 * @return the category index
 */
extern "C" int func_0028D8A0(cSSXAudio* self)
{
    int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    int result;
    if (func_002A4318(self)) {
        result = 0;
    } else if (func_002A4368(self)) {
        result = 1;
    } else if (func_002A40E0(self, id)) {
        result = 0;
    } else if (func_002A4158(self, id)) {
        result = 1;
    } else if (func_002A41C8(self, id)) {
        result = 2;
    } else if (func_002A4238(self, id)) {
        result = 3;
    } else {
        result = 4;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D960);
#ifdef SKIP_ASM
/**
 * Returns whether the radio plays music: true when mRadioModeID is kRADIOMODE_RADIO, kRADIOMODE_REQUESTLINE or
 * kRADIOMODE_PLAYLIST, false for kRADIOMODE_MOUNTAIN and above kRADIOMODE_PLAYLIST.
 *
 * @return true when the radio plays music
 */
extern "C" bool func_0028D960(cSSXAudio* self)
{
    unsigned int state = (unsigned int)self->mRadioModeID;
    if (state < kRADIOMODE_MOUNTAIN || state == kRADIOMODE_PLAYLIST) {
        return true;
    }
    return false;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D988);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DEF0);
#ifdef SKIP_ASM
/**
 * Stores @p newValue in mLastMusicState and returns whether it differed from the old value.
 *
 * Callers pass 11, 13 and 18; func_0028DF08 resets the field to -1.
 *
 * @param newValue  the new value
 * @return true when the value changed, else false
 */
extern "C" bool func_0028DEF0(cSSXAudio* self, int newValue)
{
    int old = self->mLastMusicState;
    self->mLastMusicState = newValue;
    return old != newValue;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DF08__FPv);
#ifdef SKIP_ASM
/**
 * Sets mLastMusicState to -1, so that the next func_0028DEF0 call reports a change.
 */
void func_0028DF08(void* vself)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    self->mLastMusicState = -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DF18);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_DF18 {
    long v;
} __attribute__((packed));
extern const sSndKey8_DF18 D_004A3648_DF18[] __asm__("D_004A3648");
extern const sSndKey8_DF18 D_004A3650_DF18[] __asm__("D_004A3650");
extern const sSndKey8_DF18 D_004A3658_DF18[] __asm__("D_004A3658");
// PORT: cMonitor_MONITOR_Create returns int; the unit declares it void elsewhere.
extern "C" int cMonitor_MONITOR_Create_DF18(void*, int, long, int, int, int, int, int) __asm__("cMonitor_MONITOR_Create");
extern "C" int func_002A10C0(void* self, int id);
extern "C" void func_002B3D48(void* self, float v);
int func_002B4908(void* self);

struct sBmVtDF18 {
    short delta;
    short index;
    void* fn;
};

typedef int (*tBmSetDF18)(void*, int);

/**
 * Switches to the next song unless func_002A10C0 says speech 10 is playing; otherwise retries later unless @p quiet.
 *
 * When func_002A10C0(self, 10) is 0: fades the song out over 2 s (func_002B3D48) unless mDJSpeech is set, sends music
 * event 18 through MusicSys.mVtbl slot 4, picks a new song (func_0028D488) and adds two timed callbacks to the
 * cBankManager's cBankMonitor (cMonitor_MONITOR_Create): func_0028E088 after 2000 with data 1, 1 and func_0028E548 after 1500
 * with data 3 and the new song's SEDVALUE (func_002B4908). Otherwise, unless @p quiet, it adds func_0028E068 after 100,
 * which calls this again quietly.
 *
 * @param quiet  non-zero: don't schedule a retry
 * @return 1 when the switch started, else 0
 */
// PORT: 64-bit long sound key
extern "C" int func_0028DF18(cSSXAudio* self, int quiet)
{
    int ok = 0;
    if (func_002A10C0(self, 10) == 0) {
        char* mon = (char*)&self->MusicSys;
        if (self->mDJSpeech == 0) {
            func_002B3D48(mon, 2.0f);
        }
        sBmVtDF18* vt = (sBmVtDF18*)self->MusicSys.mVtbl;
        ok = 1;
        ((tBmSetDF18)vt[4].fn)(vthis(mon, vt[4]), 18);
        func_0028D488(self);
        cMonitor_MONITOR_Create_DF18(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                     2000, D_004A3648_DF18->v, 0, 1, 1, 0, 0);
        cBankMonitor* bankMonitor = &self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        const sSndKey8_DF18* key = D_004A3650_DF18;
        int sedValue = func_002B4908(mon);
        cMonitor_MONITOR_Create_DF18(bankMonitor, 1500, key->v, 0, 3, sedValue, 0, 0);
    } else if (quiet == 0) {
        cMonitor_MONITOR_Create_DF18(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                     100, D_004A3658_DF18->v, 0, 0, 0, 0, 0);
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E068);
#ifdef SKIP_ASM
/**
 * Retries func_0028DF18 quietly: the timed callback func_0028DF18 schedules when it cannot switch songs yet.
 *
 * @return true when func_0028DF18 started the switch, else false
 */
extern "C" bool func_0028E068(void* self)
{
    return func_0028DF18((cSSXAudio*)self, 1) != 0;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", func_0028E088);
#ifdef SKIP_ASM
extern "C" int func_002B2550(int);

/**
 * Timed callback that asks func_0028E100 to act once the old song has stopped.
 *
 * When there is no current song (MusicSys.mSongInstance) or func_002B2550 of it returns -1, or when @p event's mData2
 * is set, it sets mMusicRequestPending to 1 and mMusicRequest to @p event's mData1 and returns true; otherwise it
 * returns false. func_0028DF18 schedules it with data 1, 1.
 *
 * @param event  the cMonitorInstance that fired
 * @return true when the request was posted, else false
 */
extern "C" bool func_0028E088(cSSXAudio* self, cMonitorInstance* event) {
    int obj;
    int eventValue;

    obj = (int)self->MusicSys.mSongInstance;
    if (obj == 0 || func_002B2550(obj) == -1) {
        self->mMusicRequestPending = 1;
        self->mMusicRequest = event->mData1;
        return true;
    }
    eventValue = event->mData2;
    if (eventValue == true) {
        self->mMusicRequestPending = eventValue;
        self->mMusicRequest = event->mData1;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E100);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void* cAudioInterface_getBE();
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(cSSXAudio* self);
extern "C" bool func_0028D960(cSSXAudio* self);
extern "C" int func_002A1E20(void* self, int id);
extern "C" int func_002A4030(void* self);
extern "C" void func_002B3C28(void* self, int a1);
extern "C" void func_002B49C0(void* self, int a1);
extern char str_Peak1_b[];
extern char str_Peak2_b[];
extern char str_Peak3_b[];

struct sBmVtE100 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadE100)(void*, void*, int, void*, int);
typedef int (*tBmSetE100)(void*, int);

static inline void bmLoad_E100(cSSXAudio* self, char* bank, int n)
{
    char* mon = (char*)&self->MusicSys;
    sBmVtE100* vt = (sBmVtE100*)self->MusicSys.mVtbl;
    ((tBmLoadE100)vt[2].fn)(vthis(mon, vt[2]), bank, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    func_002B49C0(mon, n);
}

static inline void bmLoadBank_E100(cSSXAudio* self, int t)
{
    if (t == kPEAKBIT_Peak1) {
        bmLoad_E100(self, str_Peak1_b, kSONGTAG_Peak1);
    } else if (t == kPEAKBIT_Peak2) {
        bmLoad_E100(self, str_Peak2_b, kSONGTAG_Peak2);
    } else {
        bmLoad_E100(self, str_Peak3_b, kSONGTAG_Peak3);
    }
    char* mon = (char*)&self->MusicSys;
    sBmVtE100* vt2 = (sBmVtE100*)self->MusicSys.mVtbl;
    ((tBmSetE100)vt2[4].fn)(vthis(mon, vt2[4]), 0xC);
    func_002B3C28(mon, 0x7F);
}

/**
 * Carries out the music request that func_0028E088 posted; called every frame from cSSXAudio_Update while
 * mAudioUpdateState is 6.
 *
 * Does nothing unless mMusicRequestPending is set; it is cleared at the end. Request 0 (mMusicRequest): when the radio
 * plays music (func_0028D960), starts the peak theme for mTransportTrack's location (resetting mTransportTrack to
 * kMAX_LOCATIONS), or for the race location when mTransportTrack is already kMAX_LOCATIONS: "Peak1", "Peak2" or "Peak3"
 * by func_002A1E20 (kPEAKBIT_Peak1, kPEAKBIT_Peak2, else peak 3), on the music control, tagged kSONGTAG_Peak1-Peak3,
 * event 12 and func_002B3C28. Request 2: when the radio plays music and mTransportTrack is not kMAX_LOCATIONS: at the
 * back-country locations kLOCATION_ABC1-EBC3 (func_002A4030) a new song (func_0028D488) and func_0028CF98(self, 36, 1,
 * kPEAKBIT_FromLocation, 0), otherwise the peak theme for mTransportTrack; mTransportTrack is reset to kMAX_LOCATIONS.
 * Request 4: func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0). Other requests: when the radio plays music,
 * func_0028CF98 with event 36 for request 3, else 0.
 */
extern "C" void func_0028E100(cSSXAudio* self)
{
    if (self->mMusicRequestPending == 0)
        return;
    int st = self->mMusicRequest;
    int id;
    if (st == 0) {
        if (func_0028D960(self)) {
            int locGroup;
            id = self->mTransportTrack;
            if (id == kMAX_LOCATIONS) {
                locGroup = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            } else {
                locGroup = func_002A1E20(self, id);
                self->mTransportTrack = kMAX_LOCATIONS;
            }
            bmLoadBank_E100(self, locGroup);
        }
    } else if (st == 2) {
        if (func_0028D960(self)) {
            id = self->mTransportTrack;
            if (id != kMAX_LOCATIONS) {
                if (func_002A4030(self)) {
                    func_0028D488(self);
                    func_0028CF98(self, 36, 1, kPEAKBIT_FromLocation, 0);
                } else {
                    id = self->mTransportTrack;
                    bmLoadBank_E100(self, func_002A1E20(self, id));
                }
                self->mTransportTrack = kMAX_LOCATIONS;
            }
        }
    } else if (st == 4) {
        func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0);
    } else {
        if (func_0028D960(self)) {
            if (self->mMusicRequest == 3)
                func_0028CF98(self, 36, 0, kPEAKBIT_FromLocation, 0);
            else
                func_0028CF98(self, 0, 0, kPEAKBIT_FromLocation, 0);
        }
    }
    self->mMusicRequestPending = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E510);
#ifdef SKIP_ASM
struct sBmVtE510 { short delta; short index; int (*fn)(void*, int); };

/**
 * Monitor callback that sets the music state to the instance's mData1 through cMusicSys's virtual slot 4.
 *
 * On gSSXAudio the slot is func_0028F3C8 (cSSXAudio's override). Returns 1, so the callback runs once.
 *
 * @param ev  the cMonitorInstance (mData1 = the music state)
 * @return 1
 */
extern "C" int func_0028E510(cSSXAudio* self, cMonitorInstance* ev)
{
    char* mon = (char*)&self->MusicSys;
    sBmVtE510* vt = (sBmVtE510*)self->MusicSys.mVtbl;
    vt[4].fn(vthis(mon, vt[4]), ev->mData1);
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", func_0028E548);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int func_002A1E20(void* self, int id);
extern "C" int func_002A40E0(void* self, int id);
extern "C" int func_002A4158(void* self, int id);
extern "C" void func_002A26F0(void* self, int a1, int a2);
extern "C" int func_002A42C8(void* self);
extern "C" void func_002B11B0(void* self, int a1);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* iface, int a1, int a2);
void func_002B1758(void* self, int a1);
int func_002B4908(void* self);

struct sLocks_E548 {
    int v[1];
};

/**
 * Monitor callback for delayed music-change events: queues DJ speech and sets the speech-cue flags by event kind.
 *
 * The kind is mData1 and the argument mData2 (armed through D_004A3650 by func_0028E8C0, func_0028FAE0 and others).
 * Kinds 0, 1, 2, 3, 4 and 7 purge speech instance 0 (func_002B11B0), queue speech cue 8421 with func_002A26F0(self, a,
 * b) ((1, 0) for 0, 3 and 7; (1, 1) for 1; (0, 1) for 2; (0, 0) for 4) and restart speech (func_002B1758). Outside
 * newRace.cGameEvent modes 6-11 (func_002A42C8): kind 0 sets unk_576C and, in a peak-1 location (func_002A1E20
 * kPEAKBIT_Peak1), unk_577C = 1, unk_5780 = 2 and mFirstTimeOnTrack[kLOCATION_ABC1] = 0; kinds 1 and 2 set unk_574C.
 *
 * Kinds 3, 4 and 5 act only when mData2 (a song's SEDVALUE) is not kNoSedValue: with the peak locked
 * (cBENewPlayerInterface_isPeakLocked1) in a peak-1 location whose mFirstTimeOnTrack entry is 1, they set unk_5770 when
 * func_002A40E0 or func_002A4158 accept the location and clear the entry; then mSongAnnouncePending = 1 and
 * mSongAnnounceNum = mData2. Kind 7 sets unk_5784 and mSongAnnouncePending, stores the current song's mSedValue
 * (func_002B4908) in mSongAnnounceNum and clears mFirstTimeOnTrack[kLOCATION_DBC2] (a peak-2 location) or
 * mFirstTimeOnTrack[kLOCATION_EBC3]. Other kinds do nothing.
 *
 * @param ev  the cMonitorInstance (mData1 = event kind, mData2 = its argument)
 * @return 1
 */
extern "C" int func_0028E548(cSSXAudio* self, cMonitorInstance* ev)
{
    int st = ev->mData1;
    if (st == 0) {
        char* mon = (char*)&self->SpeechSys;
        func_002B11B0(mon, 0);
        func_002A26F0(self, 1, 0);
        func_002B1758(mon, 0);
        if (func_002A42C8(self) == 0) {
            self->unk_576C = 1;
            int locGroup = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            if (locGroup == kPEAKBIT_Peak1) {
                self->unk_577C = locGroup;
                self->unk_5780 = 2;
                self->mFirstTimeOnTrack[kLOCATION_ABC1] = 0;
            }
        }
    } else if (st == 1 || st == 2) {
        func_002B11B0(&self->SpeechSys, 0);
        if (ev->mData1 == 2)
            func_002A26F0(self, 0, 1);
        else
            func_002A26F0(self, 1, 1);
        func_002B1758(&self->SpeechSys, 0);
        if (func_002A42C8(self) == 0)
            self->unk_574C = 1;
    } else if (st == 3 || st == 5 || st == 4) {
        if (ev->mData2 != kNoSedValue) {
        if (ev->mData1 == 3 || ev->mData1 == 4) {
            func_002B11B0(&self->SpeechSys, 0);
            if (ev->mData1 == 4)
                func_002A26F0(self, 0, 0);
            else
                func_002A26F0(self, 1, 0);
            func_002B1758(&self->SpeechSys, 0);
        }
        if (func_002A42C8(self) == 0 && cBENewPlayerInterface_isPeakLocked1(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1) != 0) {
            int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0));
            int locGroup = func_002A1E20(self, id);
            if (locGroup == kPEAKBIT_Peak1) {
                int lock = ((sLocks_E548*)(&self->mFirstTimeOnTrack[0]))->v[id];
                if (lock == locGroup) {
                    if (func_002A40E0(self, id) != 0 || func_002A4158(self, id) != 0)
                        self->unk_5770 = lock;
                    ((sLocks_E548*)(&self->mFirstTimeOnTrack[0]))->v[id] = 0;
                }
            }
        }
        self->mSongAnnouncePending = 1;
        self->mSongAnnounceNum = ev->mData2;
        }
    } else if (st == 7) {
        char* mon = (char*)&self->SpeechSys;
        func_002B11B0(mon, 0);
        func_002A26F0(self, 1, 0);
        func_002B1758(mon, 0);
        self->unk_5784 = 1;
        self->mSongAnnouncePending = 1;
        self->mSongAnnounceNum = func_002B4908(&self->MusicSys);
        if (func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)))
            == kPEAKBIT_Peak2)
            self->mFirstTimeOnTrack[kLOCATION_DBC2] = 0;
        else
            self->mFirstTimeOnTrack[kLOCATION_EBC3] = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E888);
#ifdef SKIP_ASM
extern "C" void func_0028E8C0(cSSXAudio* self, int id, int a2);

/**
 * Ends the transport music: when unk_6254 is set (func_0028E8C0 event 20) it runs event 19 and clears it.
 *
 * Called with gSSXAudio by cSSXScriptEngine_ClearNISPlayListState when the playing NIS list ends.
 */
extern "C" void func_0028E888(cSSXAudio* self)
{
    if (self->unk_6254 != 0) {
        func_0028E8C0(self, 19, 1);
        self->unk_6254 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E8C0);
#ifdef SKIP_ASM
struct sSndKey8_E8C0 {
    long v;
} __attribute__((packed));
extern const sSndKey8_E8C0 D_004A3648_E8C0[] __asm__("D_004A3648");
extern const sSndKey8_E8C0 D_004A3650_E8C0[] __asm__("D_004A3650");
extern const sSndKey8_E8C0 D_004A3658_E8C0[] __asm__("D_004A3658");
extern char str_pktrans[];
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);
extern "C" void cMonitor_MONITOR_DestroyAll1(void* mon, long key);
extern "C" int func_002A3F90(void* self);
extern "C" int func_002A3FE8(void* self);
// PORT: func_002A4030 takes (self, id); the unit declares it with one parameter (func_0028E100).
extern "C" int func_002A4030_id(void* self, int id) __asm__("func_002A4030");
extern "C" int func_002A4040(void* self);
extern "C" int func_002A4078(void* self);
extern "C" int func_002A40E8(void* self);
extern "C" void func_002A4550(void* self);
extern "C" int func_002A4718(void* self);
extern "C" int func_002A4A38(void* self);
extern "C" char* cAudioInterface_getAI();
extern "C" void func_0028D5A0(cSSXAudio* self, int id);
extern "C" void cSSXAudio_WipeoutSlideEnd(void* self, int id);
extern "C" void cSSXAudio_WipeoutTumbleEnd(void* self, int id);
extern "C" void cSSXAudio_EndUberSpin(void* self, int a1);
extern "C" void cSSXAudio_StopTone(void* self, float v);
extern "C" void func_002B3A70(void* self);

struct sBmVtE8C0 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadE8C0)(void*, void*, int, void*, int);
typedef int (*tBmSetE8C0)(void*, int);

/**
 * Handles a music event id: 19 and 20 end and start the transport sequence, 21-25 fade, others set the state.
 *
 * Id 19 with a2 set and mRaceMusicOn set: when the location qualifies (func_002A3FE8) and unk_578C is 0, in a music
 * mode accepted by func_0028D960 and a newRace.cGameMode other than kGAMEMODE_TIMECHALLENGE and
 * kGAMEMODE_PNTSCHALLENGE, it arms the D_004A3650 event (func_0028E548) with kind 4 (unk_6258 clear) or 5 and the
 * song's mSedValue, and sets music state 0. With unk_578C set it fades the song out over 2 s (unless mDJSpeech), then
 * in a peak-1 location (kPEAKBIT_Peak1) arms events 0 and 1 (3000 / 2500 ms) once (func_0028DEF0(self, 11)), or else
 * picks a new song (func_0028D488) and arms kinds 1 and 7. Otherwise, for the stations kLOCATION_HUB_A-HUB_E
 * (func_002A3F90) without unk_6258, it queues DJ speech (func_002A26F0(self, 0, 1)) and sets unk_574C. Clears unk_6258
 * at the end.
 *
 * Id 20 with a2 set: stores the location (unk_6284) in unk_5814, sets unk_6254, clears the speech-cue flags
 * (func_002A4550) and the three pending callbacks D_004A3648..D_004A3658 (cMonitor_MONITOR_DestroyAll1), stops the
 * tone, cSSXAudio_WipeoutSlideEnd / cSSXAudio_WipeoutTumbleEnd on rider 0 and cSSXAudio_EndUberSpin. In a qualifying
 * music mode it either plays the "pktrans" song (cMusicSys slot 2, tagged kSONGTAG_PkTrans, state 2 for a peak-2
 * location else 3, volume 127, paused) for a back-country location (kLOCATION_ABC1-EBC3) ridden for the first time
 * (mFirstTimeOnTrack), or arms D_004A3648 kinds 3 / 2 and may queue speech; otherwise it calls
 * func_0028D5A0(mTransportTrack) and sets state 30. Then sets mRaceMusicOn. Ids 21-25 pick a new song, fade out over 1
 * s and arm kind 3 after 3000 ms. Any other id (and 19 / 20 with a2 == 0) sets the music state through slot 4.
 *
 * The NIS script engine passes its audio events here with a2 = 0; the transport-sequence starter passes 20 and
 * func_0028E888 passes 19, both with 1.
 *
 * @param vself  the cSSXAudio
 * @param id  the music event id (a music state for the default case)
 * @param transportCall  nonzero to treat ids 19 and 20 as the transport end / start
 */
// PORT: 64-bit long sound key
extern "C" void func_0028E8C0(cSSXAudio* vself, int id, int transportCall)
{
    if (id == 19 && transportCall != 0 && vself->mRaceMusicOn != 0) {
        if (func_002A3FE8(((char*)vself)) != 0 && vself->unk_578C == 0) {
            if (func_0028D960((cSSXAudio*)vself)) {
                cBE_getInterface_Fv(cBE_getBE(), 0);
                int mode = cBENewRaceInterface_newRace.cGameMode;
                if (mode != kGAMEMODE_TIMECHALLENGE && mode != kGAMEMODE_PNTSCHALLENGE) {
                    if (vself->unk_6258 == 0) {
                        cBankMonitor* monitor = &vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = func_002B4908(&vself->MusicSys);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 4, sedValue, 0, 0);
                    } else {
                        cBankMonitor* monitor = &vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = func_002B4908(&vself->MusicSys);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 5, sedValue, 0, 0);
                    }
                    char* mon = (char*)&vself->MusicSys;
                    sBmVtE8C0* vt = (sBmVtE8C0*)vself->MusicSys.mVtbl;
                    ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), 0);
                }
            }
        } else if (vself->unk_578C != 0) {
            if (func_0028D960((cSSXAudio*)vself)) {
                // NOTE: the target keeps this 1 in $s1 across five calls (a named local, not a literal).
                int wantType = kPEAKBIT_Peak1;
                int locationGroup = func_002A1E20(((char*)vself), *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
                if (vself->mDJSpeech == 0) {
                    func_002B3D48(&vself->MusicSys, 2.0f);
                }
                if (locationGroup == wantType) {
                    if (func_0028DEF0((cSSXAudio*)vself, 11)) {
                        cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                3000, D_004A3648_E8C0->v, 0, 0, 1, 0, 0);
                        cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                2500, D_004A3650_E8C0->v, 0, 0, 0, 0, 0);
                    }
                } else {
                    func_0028DF08(((char*)vself));
                    func_0028D488(vself);
                    cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            2000, D_004A3648_E8C0->v, 0, 1, 1, 0, 0);
                    cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            1500, D_004A3650_E8C0->v, 0, 7, 0, 0, 0);
                }
            }
        } else {
            if (func_002A3F90(((char*)vself)) && vself->unk_6258 == 0) {
                func_002A26F0(((char*)vself), 0, 1);
                func_002B1758(&vself->SpeechSys, 0);
                vself->unk_574C = 1;
            }
        }
        vself->unk_6258 = 0;
    } else if (id == 20) {
        if (transportCall != 0) {
            vself->unk_6258 = 0;
            vself->unk_577C = 0;
            vself->unk_6254 = 1;
            vself->unk_5814 = vself->mTransportTrack;
            func_0028DF08(((char*)vself));
            char* mon = (char*)&vself->MusicSys;
            vself->unk_578C = 0;
            func_002A4550(((char*)vself));
            long key = D_004A3648_E8C0->v;
            cMonitor_MONITOR_DestroyAll1(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, key);
            cMonitor_MONITOR_DestroyAll1(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                         D_004A3650_E8C0->v);
            cMonitor_MONITOR_DestroyAll1(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                         D_004A3658_E8C0->v);
            cSSXAudio_StopTone(((char*)vself), 5.029983997344971f);
            cSSXAudio_WipeoutSlideEnd(((char*)vself), (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_WipeoutTumbleEnd(((char*)vself), (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_EndUberSpin(((char*)vself), 0);
            if (func_0028D960((cSSXAudio*)vself)) {
                int flag = 1;
                if (func_002A4040(((char*)vself)) == 0) {
                    if (func_002A4078(((char*)vself)) || func_002A40E8(((char*)vself))) {
                        if (*(int*)SSXApp->GameModeMan >= 2) {
                            func_0028D488(vself);
                            flag = 0;
                            cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                    10, key, 0, 3, 1, 0, 0);
                        }
                    }
                }
                if (vself->mTransportTrack == kMAX_LOCATIONS
                    || func_002A4030_id(((char*)vself), vself->mTransportTrack) == 0
                    || vself->mFirstTimeOnTrack[vself->mTransportTrack] != true) {
                    if (flag && vself->mTransportTrack != kMAX_LOCATIONS) {
                        func_0028D488(vself);
                        cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                10, D_004A3648_E8C0->v, 0, 2, 1, 0, 0);
                    }
                    if (func_002A4718(((char*)vself)) && func_002A4A38(((char*)vself)) == 0) {
                        func_002A26F0(((char*)vself), 0, 1);
                        func_002B1758(&vself->SpeechSys, 0);
                        vself->unk_5754 = 1;
                        vself->unk_6258 = 1;
                    }
                } else {
                    char* mon2 = (char*)&vself->MusicSys;
                    if (func_002A4A38(((char*)vself)) == 0) {
                        func_002B11B0(&vself->SpeechSys, 0);
                    }
                    sBmVtE8C0* vt = (sBmVtE8C0*)vself->MusicSys.mVtbl;
                    ((tBmLoadE8C0)vt[2].fn)(vthis(mon2, vt[2]), str_pktrans, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(((char*)vself), 1, 0), 1);
                    func_002B49C0(mon2, kSONGTAG_PkTrans);
                    if (func_002A1E20(((char*)vself), vself->mTransportTrack) == kPEAKBIT_Peak2) {
                        sBmVtE8C0* vt2 = (sBmVtE8C0*)vself->MusicSys.mVtbl;
                        ((tBmSetE8C0)vt2[4].fn)(vthis(mon2, vt2[4]), 2);
                    } else {
                        sBmVtE8C0* vt2 = (sBmVtE8C0*)vself->MusicSys.mVtbl;
                        ((tBmSetE8C0)vt2[4].fn)(vthis(mon2, vt2[4]), 3);
                    }
                    func_002B3C28(mon2, 0x7F);
                    func_002B3A70(mon2);
                }
            } else {
                func_0028D5A0(vself, vself->mTransportTrack);
                sBmVtE8C0* vt = (sBmVtE8C0*)vself->MusicSys.mVtbl;
                ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), 0x1E);
            }
            vself->mRaceMusicOn = 1;
        }
    } else if (id >= 21 && id <= 25) {
        if (func_0028D960((cSSXAudio*)vself)) {
            func_0028D488(vself);
            func_002B3D48(&vself->MusicSys, 1.0f);
            cMonitor_MONITOR_Create(&vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    3000, D_004A3648_E8C0->v, 0, 3, 1, 0, 0);
        }
    } else {
        char* mon = (char*)&vself->MusicSys;
        sBmVtE8C0* vt = (sBmVtE8C0*)vself->MusicSys.mVtbl;
        ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), id);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028EF90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_EF90 {
    long v;
} __attribute__((packed));
extern const sSndKey8_EF90 D_004A3648_EF90[] __asm__("D_004A3648");
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);

/**
 * Restarts the in-game music for the current location: clears the change state, picks a song and arms kind 2.
 *
 * Clears unk_6258, unk_577C, the last music event (func_0028DF08) and unk_578C, picks the next song (func_0028D488) and
 * arms the D_004A3648 callback (func_0028E088) after 10 ms with data (2, 1). Called by func_0028F558.
 */
// PORT: 64-bit long sound key
extern "C" void func_0028EF90(cSSXAudio* self)
{
    self->unk_6258 = 0;
    self->unk_577C = 0;
    func_0028DF08(self);
    self->unk_578C = 0;
    func_0028D488(self);
    cMonitor_MONITOR_Create(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                            10, D_004A3648_EF90->v, 0, 2, 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F000);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_GetCharacterLevel(void* self, int rider);

/**
 * Ramps the music volume up over 800, 1000 or 1200 updates depending on the focus rider's tier (cSSXAudio_GetCharacterLevel).
 *
 * When the current song's tag (func_002B49E0) is a peak theme (kSONGTAG_Peak1-Peak3) it sets full volume
 * (func_002B3C28(127)). Otherwise it starts a new ramp when mMusicRampState is not kMUSICRAMP_Ramping (it then becomes
 * kMUSICRAMP_Ramping, counter mMusicRampCounter = 0), advances the counter up to the tier's maximum and sets the volume
 * to 127 x counter / maximum, clamped to 0..1. Called by cSSXAudio_Update.
 */
extern "C" void func_0028F000(cSSXAudio* self)
{
    char* mon = (char*)&self->MusicSys;
    int st = func_002B49E0(mon);
    if (st >= kSONGTAG_Peak1 && st <= kSONGTAG_Peak3) {
        func_002B3C28(mon, 0x7F);
        return;
    }
    if (self->mMusicRampState != kMUSICRAMP_Ramping) {
        self->mMusicRampState = kMUSICRAMP_Ramping;
        self->mMusicRampCounter = 0;
    }
    int max = 800;
    int tier = cSSXAudio_GetCharacterLevel(self, cSSXAudio_GetFocusRider(self, -1));
    if (tier == 1) {
        max = 1000;
    } else if (tier == 2) {
        max = 1200;
    }
    int cur = self->mMusicRampCounter;
    float ratio = (float)cur / (float)max;
    if (cur < max) {
        self->mMusicRampCounter = cur + 1;
    } else {
        self->mMusicRampCounter = max;
    }
    ratio = ratio <? 1.0f;
    ratio = ratio >? 0.0f;
    func_002B3C28(&self->MusicSys, (int)(ratio * 127.0f));
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028F108);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F140);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBmVtF140 {
    short delta;
    short index;
    void* fn;
};
typedef int (*tBmSetF140)(void*, int);

/**
 * Switches the front-end music to a mode (0-9) by setting the matching music state through cMusicSys's slot 4.
 *
 * Does nothing when mode equals mFrontEndMusicMode; otherwise stores it. Mode 0 maps to state 0, or 9 when the music
 * code (cMusicSys unk_41C) is not -1; modes 1 and 3-8 map to the same state; 2 and 9 map to 2; other values set
 * nothing. Called by the front-end states (title 0, rider and lodge screens 1, store 5, trophy room 6, ...).
 *
 * @param mode  the front-end music mode
 */
extern "C" void func_0028F140(cSSXAudio* self, int mode)
{
    int bank;
    if (mode == self->mFrontEndMusicMode) {
        return;
    }
    self->mFrontEndMusicMode = mode;
    switch (mode) {
    case 0:
        bank = (self->MusicSys.unk_41C == -1) ? 0 : 9;
        break;
    case 1: bank = 1; break;
    case 3: bank = 3; break;
    case 4: bank = 4; break;
    case 5: bank = 5; break;
    case 6: bank = 6; break;
    case 7: bank = 7; break;
    case 8: bank = 8; break;
    case 2:
    case 9: bank = 2; break;
    default: bank = -1; break;
    }
    if (bank >= 0) {
        char* mon = (char*)&self->MusicSys;
        sBmVtF140* vt = (sBmVtF140*)self->MusicSys.mVtbl;
        ((tBmSetF140)vt[4].fn)(vthis(mon, vt[4]), bank);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F200);
#ifdef SKIP_ASM
extern "C" bool func_0028D960(cSSXAudio* self);
extern "C" int func_002B49E0(void* monitor);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);

struct sSndKeyF200 {
    long v;
} __attribute__((packed));
extern const sSndKeyF200 D_004A3648[];

/**
 * Prepares the music for a replay: with no song (or the podium tune) arms a delayed change, else marks a resume.
 *
 * Clears unk_6294 and mReplaySongPlaying. When no song is playing (mSongPlaying 0) or the current song's tag is
 * kSONGTAG_CharTune: in a music mode accepted by func_0028D960 it restores mCurrentSongNum from mSavedSongNum for that
 * song and arms D_004A3648 (func_0028E088) after 10 ms with data (3, 1); then sets unk_6294. Otherwise sets
 * mReplaySongPlaying. Called from the replay code; func_0028F2C0 undoes it.
 */
// PORT: 64-bit long sound key
extern "C" void func_0028F200(cSSXAudio* self)
{
    char* mon = (char*)&self->MusicSys;
    self->unk_6294 = 0;
    self->mReplaySongPlaying = 0;
    if (self->MusicSys.mSongPlaying == 0 || func_002B49E0(mon) == kSONGTAG_CharTune) {
        if (func_0028D960((cSSXAudio*)self)) {
            if (func_002B49E0(mon) == kSONGTAG_CharTune) {
                self->MusicSys.mCurrentSongNum = self->mSavedSongNum;
            }
            cMonitor_MONITOR_Create(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    10, D_004A3648[0].v, 0, 3, 1, 0, 0);
        }
        self->unk_6294 = 1;
    } else {
        self->mReplaySongPlaying = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F2C0);
#ifdef SKIP_ASM
extern "C" void* func_002B3AC0(void*);

struct sBankMgrVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Ends the replay music: stops the song when func_0028F200 set unk_6294, or sets music state 36 when
 * mReplaySongPlaying.
 *
 * Clears both flags.
 */
extern "C" void func_0028F2C0(cSSXAudio* self)
{
    if (self->unk_6294 != 0) {
        func_002B3AC0(&self->MusicSys);
    } else if (self->mReplaySongPlaying != 0) {
        char* obj = (char*)&self->MusicSys;
        sBankMgrVEntry* vt = (sBankMgrVEntry*)self->MusicSys.mVtbl;
        vt[4].fn(vthis(obj, vt[4]), 36);
    }
    self->unk_6294 = 0;
    self->mReplaySongPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F328);
#ifdef SKIP_ASM
extern "C" void func_002B3B88(void* self, int a1);
extern "C" int func_002B49E0(void* monitor);
extern int D_004A3620;

class cBankMonitor_F328 {
public:
    char data[0x5440];
    virtual void v1();
    virtual void v2();
    virtual void v3(int a1);
};

/**
 * cSSXAudio's override of cMusicSys virtual slot 3: calls func_002B3B88, then handles codes 10, 11 and 18.
 *
 * When the current song has no tag (func_002B49E0 == kSONGTAG_None) and code is 10, 11 or 18 it calls cMusicSys's
 * virtual slot 3 with 8 (only while D_004A3620 is set) and clears mSongPlaying. Slot 3 of D_00483688.
 *
 * @param code  the music state
 */
extern "C" void func_0028F328(cSSXAudio* self, int code)
{
    void* monitor = (char*)&self->MusicSys;
    func_002B3B88(monitor, code);
    if (func_002B49E0(monitor) == kSONGTAG_None) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (D_004A3620 == true) {
                ((cBankMonitor_F328*)(&self->MusicSys))->v3(8);
            }
            self->MusicSys.mSongPlaying = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F3C8);
#ifdef SKIP_ASM
extern "C" int func_002B3BC0(void* self, int v);

/**
 * cSSXAudio's override of cMusicSys virtual slot 4: sets the music state (func_002B3BC0), then as func_0028F328.
 *
 * When the current song has no tag (kSONGTAG_None) and code is 10, 11 or 18 it calls virtual slot 3 with 8 (only while
 * D_004A3620 is set) and clears mSongPlaying. Slot 4 of D_00483688; the music code calls it to change state.
 *
 * @param code  the new music state
 * @return func_002B3BC0's result (whether the state changed)
 */
extern "C" int func_0028F3C8(cSSXAudio* self, int code)
{
    void* monitor = (char*)&self->MusicSys;
    int changed = func_002B3BC0(monitor, code);
    if (func_002B49E0(monitor) == kSONGTAG_None) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (D_004A3620 == true) {
                ((cBankMonitor_F328*)(&self->MusicSys))->v3(8);
            }
            self->MusicSys.mSongPlaying = 0;
        }
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F478);
#ifdef SKIP_ASM
class cUiObj_F478 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual int handleEvent(int a, int b);
};

int GetHashValue32(char*);
extern char str_hud_f[];
extern "C" void func_002B35A0(void* monitor);
extern "C" float cSSXAudio_GetCurrAppliedVolume(void* self, int a, int b);
extern "C" cUiObj_F478* cUIStateStack_findState(void* list, int hash);

/**
 * cSSXAudio's override of cMusicSys virtual slot 2: plays the song (func_002B35A0), then refreshes the HUD.
 *
 * When music volume control 1 (cSSXAudio_GetCurrAppliedVolume(self, 1, 0)) is above 0 it finds the "hud" state on the game's UI state
 * stack (cUIStateStack_findState) and calls its virtual slot 24 with (5, 0). Slot 2 of D_00483688.
 */
extern "C" void func_0028F478(cSSXAudio* self)
{
    func_002B35A0(&self->MusicSys);
    if (cSSXAudio_GetCurrAppliedVolume(self, 1, 0) > 0.0f) {
        cGame* game = SSXApp->Game;
        if (game != 0) {
            cUIEngine* ui = game->UI;
            if (ui != 0) {
                cUIStateStack* list = &ui->mStateStack;
                if (list != 0) {
                    cUiObj_F478* obj = cUIStateStack_findState(list, GetHashValue32(str_hud_f));
                    if (obj != 0) {
                        obj->handleEvent(5, 0);
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F520);
#ifdef SKIP_ASM
extern "C" bool func_0028D960(cSSXAudio* self);
extern "C" void* func_002B3AC0(void*);

/**
 * Stops the current song (func_002B3AC0) when the music mode allows music (func_0028D960).
 */
extern "C" void func_0028F520(cSSXAudio* self)
{
    if (func_0028D960((cSSXAudio*)self) != 0) {
        func_002B3AC0(&self->MusicSys);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F558);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void func_0028EF90(cSSXAudio* self);

/**
 * Sets the music location (unk_6284) and, when it is the race's current location, restarts its music.
 *
 * The restart is func_0028EF90.
 *
 * @param id  the location id
 */
extern "C" void func_0028F558(cSSXAudio* self, int id)
{
    self->mTransportTrack = id;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (iface != 0) {
        if (*cBENewRaceInterface_getLocation(iface) == self->mTransportTrack) {
            func_0028EF90(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F5B8);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" void cSSXAudio_PauseSFX(void* self);
extern "C" void cSSXAudio_MenuSound(void* self, int a1);
extern "C" void func_002B3D48(void* self, float v);
extern "C" void func_0028F768(cSSXAudio* self);
extern "C" void func_002B3C28(void* self, int a1);

struct sBmVtF5B8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Switches the audio to the map screen: pauses the sound effects and starts the loading-screen loop at state 0.
 *
 * Clears unk_6088 and returns when the audio is paused (mAudioPaused) or not in game. Without unk_5818 it pauses the
 * sound effects (cSSXAudio_PauseSFX) and sets unk_6088 so func_0028F678 resumes them. Plays menu sound 9; outside
 * kRADIOMODE_MOUNTAIN it fades the song out over 1 s, starts the loading-screen sound (func_0028F768), sets music state
 * 0 and full volume. Called when the map screen opens.
 */
extern "C" void func_0028F5B8(cSSXAudio* self)
{
    self->mPausedSFXForMapScreen = 0;
    if (self->mAudioPaused != 0) return;
    if (cAudioInterface_isInGame() == 0) return;
    if (self->unk_5818 == 0) {
        cSSXAudio_PauseSFX(self);
        self->mPausedSFXForMapScreen = 1;
    }
    cSSXAudio_MenuSound(self, 9);
    if (self->mRadioModeID != kRADIOMODE_MOUNTAIN) {
        char* mon = (char*)&self->MusicSys;
        func_002B3D48(mon, 1.0f);
        func_0028F768(self);
        sBmVtF5B8* vt = (sBmVtF5B8*)self->MusicSys.mVtbl;
        vt[4].fn(vthis(mon, vt[4]), 0);
        func_002B3C28(mon, 0x7F);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F678);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResumeSFX(void* self);
extern "C" int func_002A3F90(void* self);
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028FA98(cSSXAudio* self, float v);

/**
 * Leaves the map screen: resumes the sound effects paused by func_0028F5B8 and stops the loading-screen loop.
 *
 * When unk_6088 is set it calls cSSXAudio_ResumeSFX and clears it. When the loop is playing (mLoadingLoopHandle >= 0)
 * it stops it with a 1 s fade (func_0028FA98) and, with a1 set in locations 17-21 (func_002A3F90), restarts the
 * location music (func_0028CF98(self, 0, 0, -1, 0)).
 *
 * @param restartMusic  nonzero to restart the location music
 */
extern "C" void func_0028F678(cSSXAudio* self, int restartMusic)
{
    if (self->mPausedSFXForMapScreen != 0) {
        cSSXAudio_ResumeSFX(self);
        self->mPausedSFXForMapScreen = 0;
    }
    if (self->mLoadingLoopHandle >= 0) {
        func_0028FA98(self, 1.0f);
        if (restartMusic != 0 && func_002A3F90(self) != 0) {
            func_0028CF98((cSSXAudio*)self, 0, 0, -1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F700);
#ifdef SKIP_ASM
extern "C" void cBankManager_BANK_SyncLoad(cBankManager* self, int i, void* id, int a3);
extern void* D_004A3624;

/**
 * Loads the loading-screen sound bank ("data/audio/audio.big|data\\audio\\LoadingScreen.bnk") into bank 13.
 *
 * Synchronous (cBankManager_BANK_SyncLoad on the cBankManager). Called by the audio system's initialisation.
 */
extern "C" void func_0028F700(cSSXAudio* self)
{
    self->MusicSys.mpVStreamSys->mpVBankManager->BANK_SyncLoad(13, D_004A3624, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F730);
#ifdef SKIP_ASM
extern "C" void func_0028FA98(cSSXAudio* self, float v);
extern "C" void cBankManager_BANK_UnLoad(cBankManager* self, int i);

/**
 * Stops the loading-screen sound at once (func_0028FA98(0)) and unloads bank 13 (cBankManager_BANK_UnLoad).
 *
 * Called by the audio system's shutdown.
 */
extern "C" void func_0028F730(cSSXAudio* self)
{
    func_0028FA98(self, 0.0f);
    self->MusicSys.mpVStreamSys->mpVBankManager->BANK_UnLoad(0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F768);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(void* self);

// The inlined SFX_Begin(kind, 0): push a play-option level on the shared cBankManager's cBankMonitor and record its
// bank / patch.
static inline void bmPush_F768(cSSXAudio* audio, int kind)
{
    cBankManager* m = audio->mpVBankManager;
    cBankMonitor* st = &m->BankMonitor;
    m->BankMonitor.mInst++;
    st->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    st->mPosStatic[m->BankMonitor.mInst] = 0;
    st->mBaseVol[m->BankMonitor.mInst] = st->mCurrent[m->BankMonitor.mInst].vol;
    st->mVolControl[m->BankMonitor.mInst] = 0;
    memset((char*)st + m->BankMonitor.mInst * 8 + 0x40, 0, 8);
    st->mPtr[m->BankMonitor.mInst] = 0;
    st->mViewport[m->BankMonitor.mInst] = 0;
    st->mVanish[m->BankMonitor.mInst] = 0;
    st->mRadius[m->BankMonitor.mInst] = 0;
    st->mDelay[m->BankMonitor.mInst].mCount = 0;
    st->mDelay[m->BankMonitor.mInst].mTime = 100;
    st->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    st->mDelay[m->BankMonitor.mInst].mLevel = 50;
    st->mExternalVol[m->BankMonitor.mInst] = 0;
    st->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    st->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    st->mMapToMaxVol[m->BankMonitor.mInst] = 127;
    st->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    audio->mBank[audio->mpVBankManager->BankMonitor.mInst] = kind;
    audio->mPatch[audio->mpVBankManager->BankMonitor.mInst] = 0;
}

#define BM_MIN_F768(a, b) ((a) < (b) ? (a) : (b))
#define BM_MAX_F768(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Starts the loading-screen loop: plays patch 0 of bank 13 at full volume and keeps its handle in mLoadingLoopHandle.
 *
 * Opens a play-option level as cBankSys_SFX_Begin2(self, 13, 0) does, sets its base volume to 127, uses music volume
 * control 1 (cSSXAudio_GetCurrAppliedVolumePtr(1, 0)) and plays it with cBankSys_SFX_End. Called by the load screen and func_0028F5B8;
 * func_0028FA98 stops it.
 *
 * @param ssxAudio  the cSSXAudio
 */
extern "C" void func_0028F768(cSSXAudio* ssxAudio)
{
    bmPush_F768(ssxAudio, 13);
    cBankManager* bankManager = ssxAudio->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* st2 = &bankManager->BankMonitor;
    st2->mBaseVol[bankManager->BankMonitor.mInst] = 127;
    st2->mCurrent[bankManager->BankMonitor.mInst].vol = BM_MAX_F768(BM_MIN_F768(st2->mBaseVol[st2->mInst], 127), 0);
    cStreamSys* streamSys = ssxAudio->MusicSys.mpVStreamSys;
    streamSys->mpVBankManager->BankMonitor.mVolControl[streamSys->mpVBankManager->BankMonitor.mInst] =
        (float*)cSSXAudio_GetCurrAppliedVolumePtr(ssxAudio, 1, 0);
    ssxAudio->mLoadingLoopHandle = cBankSys_SFX_End(ssxAudio);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FA98);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops the loading-screen sound started by func_0028F768 (cBankMonitor_BANKMONITOR_StopHandle, type 1).
 *
 * Does nothing when mLoadingLoopHandle is negative; clears it to -1 afterwards.
 *
 * @param fadeTime  the fade time (0 stops at once)
 */
extern "C" void func_0028FA98(cSSXAudio* self, float fadeTime)
{
    int idx = self->mLoadingLoopHandle;
    if (idx >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, idx, 1, fadeTime);
        self->mLoadingLoopHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FAE0);
#ifdef SKIP_ASM
extern "C" void func_0028CF98(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(cSSXAudio* self);
extern "C" bool func_0028D960(cSSXAudio* self);
extern "C" bool func_0028DEF0(cSSXAudio* self, int a1);
extern int cMonitor_MONITOR_Create_FAE0(void*, int, long, int, int, int, int, int) __asm__("cMonitor_MONITOR_Create");
extern "C" void cMonitor_MONITOR_DestroyAll1(void* mon, long key);
extern "C" void* func_002B3AC0(void*);
int func_002B4908(void* mon);
extern "C" int func_002B49E0(void* monitor);

struct sBmKeyFAE0 {
    long v;
} __attribute__((packed));
extern const sBmKeyFAE0 D_004A3648_FAE0[] __asm__("D_004A3648");
extern const sBmKeyFAE0 D_004A3650_FAE0[] __asm__("D_004A3650");
extern const sBmKeyFAE0 D_004A3658_FAE0[] __asm__("D_004A3658");

/**
 * Restarts the in-game music after the map screen: cancels pending events, stops the song and arms a new one.
 *
 * Only in a music mode accepted by func_0028D960. Cancels D_004A3648..D_004A3658 (cMonitor_MONITOR_DestroyAll1). When
 * the current song's tag is a peak theme (kSONGTAG_Peak1-Peak3) or kSONGTAG_PkTrans, or no song is playing, it records
 * event 18 (func_0028DEF0), sets mStopSpeechOnResume, stops the song, picks the next one (func_0028D488), starts the
 * location music (func_0028CF98) and arms the D_004A3650 event (func_0028E548) after 3000 ms with kind 4 and the song's
 * mSedValue. Called by the map state.
 */
// PORT: 64-bit long sound keys.
extern "C" void func_0028FAE0(cSSXAudio* self)
{
    if (func_0028D960((cSSXAudio*)self) == 0) {
        return;
    }
    cMonitor_MONITOR_DestroyAll1(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3648_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3650_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3658_FAE0[0].v);
    int st = func_002B49E0(&self->MusicSys);
    if ((unsigned int)(st - kSONGTAG_Peak1) < 3 || st == kSONGTAG_PkTrans || self->MusicSys.mSongPlaying == 0) {
        func_0028DEF0((cSSXAudio*)self, 18);
        self->mStopSpeechOnResume = 1;
        func_002B3AC0(&self->MusicSys);
        func_0028D488(self);
        func_0028CF98((cSSXAudio*)self, 0, 0, kPEAKBIT_FromLocation, 0);
        cBankMonitor* bankMonitor = &self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        const sBmKeyFAE0* key = D_004A3650_FAE0;
        int sedValue = func_002B4908(&self->MusicSys);
        cMonitor_MONITOR_Create_FAE0(bankMonitor, 3000, key->v, 0, 4, sedValue, 0, 0);
    }
}
#endif

extern "C" void* func_002B3AC0(void*);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FC38__FPv);
#ifdef SKIP_ASM
/**
 * Stops the current song (func_002B3AC0 on the music system).
 *
 * Called when the network-configuration screen closes.
 *
 * @return func_002B3AC0's return value
 */
void* func_0028FC38(void* vself)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    return func_002B3AC0(&self->MusicSys);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FC58);
#ifdef SKIP_ASM
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void func_002B49C0(void* self, int a1);
extern char str_charsel[];

struct sBmVtFC58 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadFC58)(void*, void*, int, void*, int);
typedef int (*tBmSetFC58)(void*, int);

/**
 * Plays the "charsel" song (cMusicSys slot 2, tagged kSONGTAG_CharSel) and restores the front-end music state
 * (mFrontEndMusicMode).
 *
 * Uses music volume control 1 and loads synchronously. Called when the network-configuration screen closes.
 */
extern "C" void func_0028FC58(cSSXAudio* self)
{
    char* mon = (char*)&self->MusicSys;
    sBmVtFC58* vt = (sBmVtFC58*)self->MusicSys.mVtbl;
    ((tBmLoadFC58)vt[2].fn)(vthis(mon, vt[2]), str_charsel, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 1);
    func_002B49C0(mon, kSONGTAG_CharSel);
    sBmVtFC58* vt2 = (sBmVtFC58*)self->MusicSys.mVtbl;
    ((tBmSetFC58)vt2[4].fn)(vthis(mon, vt2[4]), self->mFrontEndMusicMode);
}
#endif

