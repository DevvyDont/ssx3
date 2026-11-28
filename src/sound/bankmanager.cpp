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
 * data at +0x10 and zeroes the data pointer, the size at +0x14 and the location. Called by
 * cBankInstance::~cBankInstance and, by bank index, by cBankManager_BANK_DeallocMem.
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
 * cleared. Called by cBankInstance::~cBankInstance and, by bank index, by cBankManager_BANK_UnshareBank.
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
void cBankInstance::Attach(void* mainMemData, int mainMemSize)
{
    UnAttach();
    mpMainMemData = (char*)mainMemData;
    mMainMemDataSize = mainMemSize;
    SNDbankadd(&mBankHandle, mainMemData);
    mBankStatus = USED;
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
void cBankInstance::OnEndAsyncLoad(int id, int flag, const char* name, void* pBuffer, int dataSize)
{
    ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->RestoreSoundMemLoadLimits();
    mBankHandle = id;
    if (flag == 0) {
        mBankStatus = USED;
    } else {
        mBankStatus = EMPTY;
    }
    strcpy(mBankFilename, name);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_cBankManager);
#ifdef SKIP_ASM
extern "C" void* cBankMonitor_cBankMonitor(void* self, int a1, int heap);
extern "C" void* cAsyncSys_cAsyncSys(void* self);
extern "C" void* __13cBankInstance(void* self);
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
extern const sBmVtbl cBankManager_cSoundSys_vtbl;

struct sBmBank {
    char pad_0x0[0x60];
    sBmBank() { __13cBankInstance(this); }
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
    *(const sBmVtbl**)&self->BankMonitor.mpVSoundSys->mVtbl = &cBankManager_cSoundSys_vtbl;
    if (!embedded) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtbl vt = cBankManager_cSoundSys_vtbl;
        *(sBmVtbl**)&self->BankMonitor.mpVSoundSys->mVtbl = &vt;
        int base = (int)self->BankMonitor.mpVSoundSys - sizeof(cBankManager);
        int thisDelta = (int)self - base;
        vt.e[1].delta = cBankManager_cSoundSys_vtbl.e[1].delta + thisDelta;
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
extern "C" void _$_13cBankInstance(void* self, int flags);
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
extern const sBmVtblBB10 cBankManager_cSoundSys_vtbl_BB10 __asm__("cBankManager_cSoundSys_vtbl");



/**
 * Destroys the bank manager: frees its bank instances, tears down its monitor and async-loader parts and clears
 * gpBankManager.
 *
 * Restores the cBankManager and cBankMonitor vtables and the virtual base's vtable (cBankManager_cSoundSys_vtbl; with flags 0 the g++
 * 2.95 virtual-base this-adjust fix-up runs on a stack copy), clears gpBankManager, destroys the cBankInstance array
 * mBank last-first with cBankInstance::~cBankInstance (its count is the word 0x10 bytes before it) and frees it, then
 * runs cBankMonitor__cBankMonitor on BankMonitor and cAsyncSys__cAsyncSys (On Tour ~cAsyncSys) on the object.
 * cSoundSys__cSoundSys destroys the virtual base only when flags bit 1 is set.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object's memory, bit 1 destroys the cSoundSys virtual base
 */
extern "C" void cBankManager__cBankManager(cBankManager* self, int flags)
{
    self->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mVtbl = cBankManager_vtbl;
    self->BankMonitor.mpVSoundSys->mVtbl = (void**)&cBankManager_cSoundSys_vtbl_BB10;
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblBB10 vt = cBankManager_cSoundSys_vtbl_BB10;
        self->BankMonitor.mpVSoundSys->mVtbl = (void**)&vt;
        char* base = (char*)self->BankMonitor.mpVSoundSys - sizeof(cBankManager);
        int soundSysAdjust = (char*)self - base;
        vt.e[1].delta = cBankManager_cSoundSys_vtbl_BB10.e[1].delta + soundSysAdjust;
    }
    gpBankManager = 0;
    if (self->mBank != 0) {
        cBankInstance* bankInstance = self->mBank + ((int*)self->mBank)[-4];
        while (self->mBank != bankInstance) {
            bankInstance--;
            _$_13cBankInstance(bankInstance, 0);
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
 * (cSSXAudio_InGameLoad, cSSXAudio_FrontEndLoad, cSSXAudio_LoadGruntBanks, cSSXAudio_LoadLoadingScreenBank).
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
 * slot's allocation location (+0x8 of the instance) and @p mainMemFlag. Called from cMusicSys_LoadRandomLoopBank (with 0x100) and
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
    mBank[i].Attach(mainMemData, mainMemSize);
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
    mBank[i].OnEndAsyncLoad(soundHandle, endState, pFilename, pBuffer, dataSize);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetRadioMode);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cAudioInterface_isInGame(void);
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_PickRandomSong(cSSXAudio* self);
extern "C" int cMusicSys_GetSongTag(void* monitor);
extern "C" int cMusicSys_IsCurrentSongActive(void* monitor);
extern "C" void cSSXAudio_SetDJSpeech(void* self, int on);
// PORT: cMusicSys_SetActiveSongs takes the 64-bit bank mask as a second argument (64-bit `long`).
extern "C" void cMusicSys_SetActiveSongs_BF78(void* monitor, long mask) __asm__("cMusicSys_SetActiveSongs");

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
 * kRADIOMODE_RADIO activates the songs of MusicSys.mDefaultActiveMap (cMusicSys_SetActiveSongs), re-applies the arcade SFX setting
 * from bit 17 of cBEOptionInterface_mOption's flags (cSSXAudio_SetDJSpeech), and, in game with MusicSys.mSongPlaying
 * set and a current song tag (cMusicSys_GetSongTag) of kSONGTAG_Peak1Amb-Peak3Amb (a peak ambience), picks a new song
 * (cSSXAudio_PickRandomSong) and restarts with cSSXAudio_StartLocationMusic(self, 0, 0, kPEAKBIT_FromLocation, 0). kRADIOMODE_REQUESTLINE and
 * kRADIOMODE_PLAYLIST do the same with mRadioPlayList, returning early when that is 0, when the song tag is a peak
 * theme (kSONGTAG_Peak1-Peak3), or when the current song is still active (cMusicSys_IsCurrentSongActive) and its tag is not
 * kSONGTAG_CharSel. kRADIOMODE_MOUNTAIN turns the arcade SFX off and, in game unless the song is already a peak
 * ambience, calls cSSXAudio_StartLocationMusic the same way (which starts the peak ambience in this mode) and clears mInBigChallenge
 * and mCurBigChallengeType.
 *
 * The option block is copied whole (0x288 bytes) to the stack just to read its flags.
 *
 * @param state  the new radio mode
 */
void cSSXAudio::SetRadioMode(int state)
{
    if (mRadioModeID == state)
        return;
    mRadioModeID = state;
    switch (state) {
    case kRADIOMODE_RADIO: {
        cMusicSys_SetActiveSongs_BF78(&MusicSys, *(long*)&MusicSys.mDefaultActiveMap);
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = cBEOptionInterface_mOption_BF78;
        cSSXAudio_SetDJSpeech(this, (cfg.flags >> 17) & 1);
        if (cAudioInterface_isInGame() == 0)
            return;
        if (MusicSys.mSongPlaying == 0)
            return;
        if ((unsigned int)(cMusicSys_GetSongTag(&MusicSys) - kSONGTAG_Peak1Amb) >= 3)
            return;
        PickRandomSong();
        StartLocationMusic(0, 0, -1, 0);
        break;
    }
    case kRADIOMODE_REQUESTLINE:
    case kRADIOMODE_PLAYLIST: {
        cMusicSys_SetActiveSongs_BF78(&MusicSys, *(long*)&mRadioPlayList);
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = cBEOptionInterface_mOption_BF78;
        cSSXAudio_SetDJSpeech(this, (cfg.flags >> 17) & 1);
        if (*(long*)&mRadioPlayList == 0)
            return;
        if (cAudioInterface_isInGame() == 0)
            return;
        if (MusicSys.mSongPlaying == 0)
            return;
        int st = cMusicSys_GetSongTag(&MusicSys);
        if ((unsigned int)(st - kSONGTAG_Peak1) < 3)
            return;
        if (cMusicSys_IsCurrentSongActive(&MusicSys) != 0 && st != kSONGTAG_CharSel)
            return;
        PickRandomSong();
        StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
        break;
    }
    case kRADIOMODE_MOUNTAIN:
        cSSXAudio_SetDJSpeech(this, 0);
        if (cAudioInterface_isInGame() == 0)
            return;
        if ((unsigned int)(cMusicSys_GetSongTag(&MusicSys) - kSONGTAG_Peak1Amb) < 3)
            return;
        StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
        mInBigChallenge = 0;
        mCurBigChallengeType = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetRadioPlayList);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_PickRandomSong(cSSXAudio* self);
extern "C" void cMusicSys_SetActiveSongs(void* monitor);
extern "C" int cMusicSys_GetSongTag(void* monitor);
extern "C" int cMusicSys_IsCurrentSongActive(void* monitor);

static inline void bmRestartC2D0(cSSXAudio* self)
{
    if (cAudioInterface_isInGame() == 0) {
        return;
    }
    if (self->MusicSys.mSongPlaying == 0) {
        return;
    }
    cMusicSys* mon = &self->MusicSys;
    int st = cMusicSys_GetSongTag(mon);
    if ((unsigned int)(st - kSONGTAG_Peak1) < 3) {
        return;
    }
    if (cMusicSys_IsCurrentSongActive(mon) != 0 && st != kSONGTAG_CharSel) {
        return;
    }
    self->PickRandomSong();
    self->StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
}

/**
 * Sets the radio playlist mask mRadioPlayList to @p key and, in the request-line and playlist radio modes, restarts the
 * music from it.
 *
 * When @p key differs from mRadioPlayList it is stored, and in kRADIOMODE_REQUESTLINE and kRADIOMODE_PLAYLIST
 * (mRadioModeID) cMusicSys_SetActiveSongs rebuilds the active songs from it (the mask reaches it in the second argument register;
 * the call is declared with one argument). In those modes, changed or not, it then does what cSSXAudio_SetRadioMode does for
 * those modes: in game with MusicSys.mSongPlaying set, unless the current song tag (cMusicSys_GetSongTag) is a peak theme
 * (kSONGTAG_Peak1-Peak3) or the current song is still active (cMusicSys_IsCurrentSongActive) with a tag other than kSONGTAG_CharSel, it
 * picks a new song (cSSXAudio_PickRandomSong) and calls cSSXAudio_StartLocationMusic(self, 0, 0, kPEAKBIT_FromLocation, 0). Called by
 * cSSXAudio_InGameInit and cFEStateRequestLine_onWidgetEvent.
 *
 * @param key  the new playlist mask, one bit per song (64-bit)
 */
// PORT: 64-bit `long` key.
void cSSXAudio::SetRadioPlayList(long key)
{
    if (*(long*)&mRadioPlayList != key) {
        *(long*)&mRadioPlayList = key;
        int state = mRadioModeID;
        if (state == kRADIOMODE_REQUESTLINE || state == kRADIOMODE_PLAYLIST) {
            cMusicSys_SetActiveSongs(&MusicSys);
            bmRestartC2D0(this);
        }
    } else {
        int state = mRadioModeID;
        if (state == kRADIOMODE_REQUESTLINE || state == kRADIOMODE_PLAYLIST) {
            bmRestartC2D0(this);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PlayLoopBeat);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void cSongInstance_ApplyDelay(void* a, void* mon);


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
// The cMonitorInstance that fired (sound/bankmonitor.h); mCountReset and mData3 are its [O] member names.
struct sCue_C430 {
    char pad0[4];
    int mCountReset;        // 0x04
    char pad8[0x18 - 8];
    sObj_C430* obj;         // 0x18
    char* data;             // 0x1C
    int idx;                // 0x20
    int mData3;             // 0x24
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
 * The cMusicSys timed callback cMusicSys_OverlayBeatCallback forwards to it with the cMonitorInstance that cMusicSys_ArmBeatCallback created: mPtr
 * the cSongInstance, mData1 a random phrase start (cSongInstance_GetRandomPhraseStart), mData2 the beat index, mData3 the beat length in ms.
 * With the song's cSongConfiguration it wraps mData2 to 0 at mBeatsPerPhrase (and plays nothing when that is <= 0),
 * pushes a play-option level on the cBankManager (the inlined SFX_Begin: bank = the song's mBankIndex, patch = mData1 +
 * mData2, delay defaults 100, 0, 90, 50), points the level's volume control at mOverlayLevel, scales its volume by
 * mAsynclevel / 100 (clamped to 0..127), copies the song's delay settings in with cSongInstance_ApplyDelay and plays it with
 * cBankSys_SFX_End. It then increments mData2 and sets mCountReset to mData3, so the monitor calls back one beat later.
 *
 * @param audio  the cSSXAudio (this, adjusted from its cMusicSys base)
 * @param cue  the cMonitorInstance that fired
 * @return 0
 */
extern "C" int cSSXAudio_PlayLoopBeat(cSSXAudio* audio, sCue_C430* cue)
{
    int* pidx = &cue->idx;
    char* data = cue->data;
    int saved = cue->mData3;
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
            &audio->mOverlayLevel;
        cBankManager* bankManager = audio->MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* st2 = &bankManager->BankMonitor;
        st2->mBaseVol[bankManager->BankMonitor.mInst] =
            (int)BM_MAX_C430(BM_MIN_C430(st2->mBaseVol[st2->mInst] * (vol * 0.009999999776482582f), 127.0f), 0.0f);
        st2->mCurrent[st2->mInst].vol = BM_MAX_C430(BM_MIN_C430(st2->mBaseVol[st2->mInst], 127), 0);
        cSongInstance_ApplyDelay(audio->MusicSys.mSongInstance, &audio->MusicSys);
        cBankSys_SFX_End(audio);
        (*pidx)++;
        cue->mCountReset = saved;
    }
end:
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetJumpCountdown__FPvi);
#ifdef SKIP_ASM
/**
 * Sets mJumpCountdown to @p val.
 *
 * cSSXAudio_UpdateAirTimeVolume counts mJumpCountdown down by one per call while it is positive; no retail code reads it otherwise. The
 * only caller (main/gamestate) passes 120.
 *
 * @param val  the new count
 */
void cSSXAudio::SetJumpCountdown(int val)
{
    mJumpCountdown = val;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_UpdateAirTimeVolume);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
extern "C" cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
int cRider_getMotionState(void* rider);
int cSSXAudio_GetRiderMusicEvent(void* self);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" bool cSSXAudio_IsMusicRadioMode(cSSXAudio* self);
extern "C" void cSSXAudio_BeginWind(void* self);
extern "C" void cSSXAudio_EndWind(void* self);
extern "C" int cSSXAudio_IsBigAirEvent(void* self);
extern "C" int cSSXAudio_IsHalfpipeEvent(void* self);
extern "C" void cMusicSys_SetBaseVolume(void* mon, char v);
extern "C" void cMusicSys_SetOverlayLevel(void* mon, char v);
extern "C" int cMusicSys_StartOverlay(void* mon);
extern "C" int cMusicSys_IsOverlayArmed(void* mon);
extern "C" void cMusicSys_ClearOverlayArmed(void* mon);
extern "C" int cMusicSys_GetDuckToLoops(void* mon);
extern int cSSXAudio_longAirFlag;
extern int cSSXAudio_prevLongAirFlag;

struct sBmVtC8C8 { short delta; short index; void (*fn)(void*, int); };

static inline float bmAirTime_C8C8(cSSXAudio* self, cRider* rider)
{
    int s = rider->mAirPredictor->SimState;
    int air = 0;
    if (s == cAirPredictor::kSS_SimDone || s == cAirPredictor::kSS_SimCheckObjectsDone)
        air = 1;
    if (air) {
        if (cSSXAudio_longAirFlag == true)
            return self->mJumpAirTime;
        float v = rider->mAirPredictor->TotalAirTime;
        self->mJumpMaxAirTime = 0.0f;
        self->mJumpAirTime = v;
        return v;
    }
    if (cSSXAudio_longAirFlag == true)
        return self->mJumpAirTime;
    return 0.0f;
}

/**
 * Updates the wind sound and the music volume for the focus rider's air time; called every frame from cSSXAudio_Update.
 *
 * Counts mJumpCountdown down. For the focus rider (cSSXAudio_GetFocusRider(-1)) in motion state MS_Air with its
 * mAirPredictor's SimState kSS_SimDone or kSS_SimCheckObjectsDone, the air time is the predictor's TotalAirTime, cached
 * in mJumpAirTime on the first frame (mJumpMaxAirTime cleared); while cSSXAudio_longAirFlag is set the cached value is reused;
 * otherwise it is 0. mOverlayLevel is set to the music volume (cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0)), and
 * cSSXAudio_prevLongAirFlag takes last frame's cSSXAudio_longAirFlag, which is cleared. Above 3.9995 s, when the song's mDuckToLoops
 * (cMusicSys_GetDuckToLoops) is set, cSSXAudio_IsBigAirEvent and cSSXAudio_IsHalfpipeEvent are false and no rider event runs (cSSXAudio_GetRiderMusicEvent), the song
 * is driven too (local play). Outside replays, above 2 s: cSSXAudio_longAirFlag = 1; mJumpMaxAirTime keeps the largest current air
 * time (CurAirTime), and mJumpVolumeAirTime, set on the first frame, is that plus max((ApexTime - CurAirTime) / 2,
 * 0.5). From their ratio and a volume falling with the air time (20..127) it sets mWindVolume (then scaled by the SFX
 * volume, control 5) and, with play, the song's base volume (cMusicSys_SetBaseVolume) and overlay level (mOverlayLevel via
 * cMusicSys_SetOverlayLevel). On the first such frame it starts the wind (cSSXAudio_BeginWind) and, with play, cMusicSys_StartOverlay true
 * and the radio on (cSSXAudio_IsMusicRadioMode), sends music event 7 through MusicSys.mVtbl slot 3. Otherwise, after an airborne
 * frame, it ends the wind (cSSXAudio_EndWind), sends event 8 when cMusicSys_IsOverlayArmed reports the song's mOverlayArmed set
 * (and the radio is on) and clears it with cMusicSys_ClearOverlayArmed; then it restores base volume 127 and the overlay level.
 */
void cSSXAudio::UpdateAirTimeVolume()
{
    if (mJumpCountdown > 0)
        mJumpCountdown--;
    cRider* rider = cSSXAudio_GetFocusRider_p(this, -1);
    float airTime;
    if (cRider_getMotionState(rider) == MS_Air) {
        int simState = rider->mAirPredictor->SimState;
        int air = 0;
        if (simState == cAirPredictor::kSS_SimDone || simState == cAirPredictor::kSS_SimCheckObjectsDone)
            air = 1;
        if (air) {
            if (cSSXAudio_longAirFlag == true) {
                airTime = mJumpAirTime;
            } else {
                airTime = rider->mAirPredictor->TotalAirTime;
                mJumpMaxAirTime = 0.0f;
                mJumpAirTime = airTime;
            }
            goto have;
        }
        if (cSSXAudio_longAirFlag == true) {
            airTime = mJumpAirTime;
            goto have;
        }
    }
    airTime = 0.0f;
have:
    int play = 0;
    mOverlayLevel = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0);
    cSSXAudio_prevLongAirFlag = cSSXAudio_longAirFlag;
    cSSXAudio_longAirFlag = 0;
    if (3.9994986057281494f < airTime) {
        int duckToLoops = cMusicSys_GetDuckToLoops(&MusicSys);
        if (duckToLoops == true && !cSSXAudio_IsBigAirEvent(this) && !cSSXAudio_IsHalfpipeEvent(this)) {
            if (!GetRiderMusicEvent())
                play = duckToLoops;
        }
    }
    if (cSSXAudio_ReplayMode(this) == 0) {
        if (2.0f < airTime) {
            cSSXAudio_longAirFlag = 1;
            float lo = rider->mAirPredictor->CurAirTime;
            if (lo < mJumpMaxAirTime)
                lo = mJumpMaxAirTime;
            mJumpMaxAirTime = lo;
            float hi;
            if (cSSXAudio_prevLongAirFlag == 0) {
                hi = (rider->mAirPredictor->ApexTime - rider->mAirPredictor->CurAirTime) * 0.5f;
                if (hi < 0.5f)
                    hi = 0.5f;
                hi = hi + lo;
                mJumpVolumeAirTime = hi;
            } else {
                hi = mJumpVolumeAirTime;
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
                    float k = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0);
                    int volumeAboveMin = clampedVolume - 20;
                    float musicLevel = (float)(full - volumeAboveMin) * (k * inv);
                    mOverlayLevel = musicLevel;
                    cMusicSys_SetOverlayLevel(&MusicSys, (int)(musicLevel * 100.0f));
                }
                rawVolume = vmax - (vmax - vol) * ratio;
                clampedVolume = (int)((rawVolume <? vmax) >? vol);
                mWindVolume = (float)(full - clampedVolume) * inv;
                if (play)
                    cMusicSys_SetBaseVolume(&MusicSys, clampedVolume);
            } else {
                mWindVolume = (vmax - vol) * inv;
                if (play) {
                    cMusicSys_SetBaseVolume(&MusicSys, (int)vol);
                    float k = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0);
                    mOverlayLevel = k;
                    cMusicSys_SetOverlayLevel(&MusicSys, (int)(k * 100.0f));
                }
            }
            mWindVolume = mWindVolume * *(float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 5, 0);
            if (cSSXAudio_prevLongAirFlag == 0) {
                cSSXAudio_BeginWind(this);
                if (play) {
                    char* mon = (char*)&MusicSys;
                    if (cMusicSys_StartOverlay(mon) && IsMusicRadioMode()) {
                        sBmVtC8C8* musicVt = (sBmVtC8C8*)MusicSys.mVtbl;
                        musicVt[3].fn(vthis(mon, musicVt[3]), 7);
                    }
                }
            }
            return;
        }
    }
    char* mon = (char*)&MusicSys;
    if (cSSXAudio_prevLongAirFlag == true) {
        cSSXAudio_EndWind(this);
        if (cMusicSys_IsOverlayArmed(mon)) {
            if (IsMusicRadioMode()) {
                sBmVtC8C8* musicVt = (sBmVtC8C8*)MusicSys.mVtbl;
                musicVt[3].fn(vthis(mon, musicVt[3]), 8);
            }
            cMusicSys_ClearOverlayArmed(mon);
        }
    }
    cMusicSys_SetBaseVolume(mon, 0x7F);
    cMusicSys_SetOverlayLevel(mon, (int)(mOverlayLevel * 100.0f));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PlayPkTransSong);
#ifdef SKIP_ASM
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void cMusicSys_SetSongTag(void* self, int a1);
extern "C" void cMusicSys_SetVolume(void* self, int a1);
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
 * Through MusicSys.mVtbl slot 2 (cSSXAudio's cSSXAudio_PlaySong) it starts the song "pktrans" with 12, the music volume
 * control (cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0)) and 1; tags it 401 (cMusicSys_SetSongTag), sends music event 1 through slot 4
 * (cSSXAudio_SetMusicState) and starts it with cMusicSys_SetVolume (passing 0x7F, which that function's declaration does not take).
 */
void cSSXAudio::PlayPkTransSong()
{
    char* mon = (char*)&MusicSys;
    sBmVtCD48* vt = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), str_pktrans, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0), 1);
    cMusicSys_SetSongTag(mon, kSONGTAG_PkTrans);
    sBmVtCD48* vt2 = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), 1);
    cMusicSys_SetVolume(mon, 0x7F);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PlayCharTune);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cMusicSys_SetHoldEvents(void* self, int a1);
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
 * kSONGTAG_CharTune (cMusicSys_SetSongTag), sends the event through slot 4, sets the song's mHoldEvents (cMusicSys_SetHoldEvents) so
 * that only forced events apply, and starts it with cMusicSys_SetVolume.
 */
void cSSXAudio::PlayCharTune()
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    if (mRadioModeID == kRADIOMODE_MOUNTAIN) {
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
    char* mon = (char*)&MusicSys;
    mSavedSongNum = MusicSys.mCurrentSongNum;
    sBmVtCD48* vt = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), str_chartune, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0), 0);
    cMusicSys_SetSongTag(mon, kSONGTAG_CharTune);
    sBmVtCD48* vt2 = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), track);
    cMusicSys_SetHoldEvents(mon, 1);
    cMusicSys_SetVolume(mon, 0x7F);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StartLocationMusic);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getBE();
extern "C" void func_0028F108(void* self, int a1);
extern "C" int cSSXAudio_GetLocationPeakBit(void* self, int id);
extern "C" int cSSXAudio_IsPeakChallengeEvent(void* self);
extern "C" void cMusicSys_SetVolumeEnabled(void* mon, int a1);
extern "C" void cMusicSys_RequestOverlay(void* mon);
int cMusicSys_GetCurrentSongSedValue(void* self);
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
    cMusicSys_SetSongTag(mon, id);
}

/**
 * Starts the music for the current radio mode and race location.
 *
 * Returns at once in a replay (cSSXAudio_ReplayMode) unless mReplayMusicStarted is set. Clears
 * mPathfinderCanChangeVolume and the rider-event words at 0x623C and 0x6240 (cSSXAudio_StartRiderMusicEvent) and sets mMusicEventOwner
 * to kNoMusicEventOwner. Songs are started through MusicSys.mVtbl slot 2 with 12, a volume control
 * (cSSXAudio_GetCurrAppliedVolumePtr) and 0, tagged with cMusicSys_SetSongTag, and given events through slot 4.
 *
 * In kRADIOMODE_MOUNTAIN (mRadioModeID) it starts the ambience "Peak1Amb", "Peak2Amb" or "Peak3Amb" by @p type, or,
 * when that is kPEAKBIT_FromLocation, by cSSXAudio_GetLocationPeakBit of the race location (kPEAKBIT_Peak1, kPEAKBIT_Peak2, anything
 * else), on the ambient control (8) and tagged kSONGTAG_Peak1Amb-Peak3Amb; then cMusicSys_RequestOverlay, sets
 * mPathfinderCanChangeVolume to 1 and calls cMusicSys_SetVolumeEnabled(0) and cMusicSys_SetVolume. Else, at a station (kLOCATION_HUB_A or
 * above) with @p a2 and @p requestLine zero and cSSXAudio_IsPeakChallengeEvent false, it starts the theme "Peak1", "Peak2" or "Peak3"
 * (by the location) on the music control, tagged kSONGTAG_Peak1-Peak3, cMusicSys_SetVolumeEnabled(1), event 12 and cMusicSys_SetVolume.
 * Otherwise it starts the current song of the active list (null name), with MusicSys.mCurrentSongNum saved in
 * mLastPlaylistSong and the song's SEDVALUE (cMusicSys_GetCurrentSongSedValue) in mSongAnnounceNum, cMusicSys_SetVolumeEnabled(1), event @p startEvent
 * (36 when @p requestLine is set at a station), then func_0028F108 with the focus rider (cSSXAudio_GetFocusRider(-1)).
 *
 * @param startEvent  music event sent to the playlist song (callers pass 0 or 36)
 * @param skipPeakTheme  non-zero skips the peak theme (cSSXAudio_HandleMusicRequest passes 1)
 * @param type  peak for kRADIOMODE_MOUNTAIN (an ePeakBit), or kPEAKBIT_FromLocation to take it from the location
 * @param requestLine  non-zero skips the peak theme and selects event 36 on the peaks; cFEStateRequestLine_onWidgetMenu passes 1
 */
void cSSXAudio::StartLocationMusic(int startEvent, int skipPeakTheme, int type, int requestLine)
{
    if (cSSXAudio_ReplayMode(this) != 0 && mReplayMusicStarted == 0) {
        return;
    }
    mPathfinderCanChangeVolume = 0;
    mMusicEventOwner = kNoMusicEventOwner;
    mRiderMusicEvent = 0;
    mNextRiderMusicEvent = 0;
    if (mRadioModeID == kRADIOMODE_MOUNTAIN) {
        int peakNumber = type == kPEAKBIT_FromLocation
            ? cSSXAudio_GetLocationPeakBit(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)))
            : type;
        if (peakNumber == kPEAKBIT_Peak1) {
            bmLoad_CF98(this, str_Peak1Amb, 8, kSONGTAG_Peak1Amb);
        } else if (peakNumber == kPEAKBIT_Peak2) {
            bmLoad_CF98(this, str_Peak2Amb, 8, kSONGTAG_Peak2Amb);
        } else {
            bmLoad_CF98(this, str_Peak3Amb, 8, kSONGTAG_Peak3Amb);
        }
        char* mon = (char*)&MusicSys;
        cMusicSys_RequestOverlay(mon);
        mPathfinderCanChangeVolume = 1;
        cMusicSys_SetVolumeEnabled(mon, 0);
        cMusicSys_SetVolume(mon, 0x7F);
        return;
    }
    tBELocationDB info = *(tBELocationDB*)cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    char* mon = (char*)&MusicSys;
    if (info.locationID >= kLOCATION_HUB_A && skipPeakTheme == 0 && requestLine == 0 && cSSXAudio_IsPeakChallengeEvent(this) == 0) {
        int peakNumber = cSSXAudio_GetLocationPeakBit(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (peakNumber == kPEAKBIT_Peak1) {
            bmLoad_CF98(this, str_Peak1_b, 1, kSONGTAG_Peak1);
        } else if (peakNumber == kPEAKBIT_Peak2) {
            bmLoad_CF98(this, str_Peak2_b, 1, kSONGTAG_Peak2);
        } else {
            bmLoad_CF98(this, str_Peak3_b, 1, kSONGTAG_Peak3);
        }
        cMusicSys_SetVolumeEnabled(mon, 1);
        sBmVtCD48* vt2 = (sBmVtCD48*)MusicSys.mVtbl;
        ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), 0xC);
        cMusicSys_SetVolume(mon, 0x7F);
        return;
    }
    sBmVtCD48* vt = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmLoadCD48)vt[2].fn)(vthis(mon, vt[2]), 0, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0), 0);
    mLastPlaylistSong = MusicSys.mCurrentSongNum;
    mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(mon);
    cMusicSys_SetVolumeEnabled(mon, 1);
    if (requestLine != 0 && info.locationID >= 17) {
        startEvent = 36;
    }
    sBmVtCD48* vt2 = (sBmVtCD48*)MusicSys.mVtbl;
    ((tBmSetCD48)vt2[4].fn)(vthis(mon, vt2[4]), startEvent);
    func_0028F108(this, cSSXAudio_GetFocusRider(this, -1));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PickRandomSong);
#ifdef SKIP_ASM
extern "C" unsigned int cSoundSys_SndRand(void* rng);
extern "C" int cSSXAudio_GetSongCategory(cSSXAudio* self);
extern "C" void cMusicSys_NextActiveSong(void* q, int a, int b);

/**
 * Picks a new random song from the active list, avoiding the one cSSXAudio_StartLocationMusic last started.
 *
 * Sets MusicSys.mCurrentSongNum to a random index (cSoundSys_SndRand) scaled to mNumFESongs, then steps it 1 to 11
 * times (random) to the next active song of the category cSSXAudio_GetSongCategory picks (cMusicSys_NextActiveSong with the filter on). If it
 * ends on the song saved in mLastPlaylistSong, it steps once more in that category and, if still the same, once without
 * the filter.
 *
 */
void cSSXAudio::PickRandomSong()
{
    unsigned int count = (unsigned int)MusicSys.mNumFESongs;
    int prev = mLastPlaylistSong;
    MusicSys.mCurrentSongNum = ((cSoundSys_SndRand(MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF) * count) / 0x7FFF;
    int n = ((cSoundSys_SndRand(MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF) * 10) / 0x7FFF + 1;
    int i;
    for (i = 0; i < n; i++)
        cMusicSys_NextActiveSong(&MusicSys, 1, GetSongCategory());
    char* monitor = (char*)&MusicSys;
    if (prev == MusicSys.mCurrentSongNum) {
        cMusicSys_NextActiveSong(monitor, 1, GetSongCategory());
        if (prev == MusicSys.mCurrentSongNum)
            cMusicSys_NextActiveSong(monitor, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_RefreshPeakAmbience);
#ifdef SKIP_ASM
extern "C" int cMusicSys_GetSongTag(void* monitor);
extern "C" int cSSXAudio_GetLocationPeakBit(void* self, int id);
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);

/**
 * Restarts the music when the playing peak ambience does not belong to location @p id.
 *
 * Does nothing while mTransportTrack is kMAX_LOCATIONS (no location). Otherwise cSSXAudio_GetLocationPeakBit maps @p id to its peak
 * (kPEAKBIT_Peak1, kPEAKBIT_Peak2, else peak 3, expecting the song tag kSONGTAG_Peak1Amb, Peak2Amb or Peak3Amb) and,
 * when the current song tag (cMusicSys_GetSongTag) differs, calls cSSXAudio_StartLocationMusic(self, 0, 0, peak, 0). The only caller,
 * cSSXAudio_HandleMusicEvent, passes mTransportTrack.
 *
 * @param id  a race location (eBELocationID)
 */
void cSSXAudio::RefreshPeakAmbience(int id)
{
    int cur;
    int type;
    int want;
    if (mTransportTrack == kMAX_LOCATIONS) {
        return;
    }
    cur = cMusicSys_GetSongTag(&MusicSys);
    type = cSSXAudio_GetLocationPeakBit(this, id);
    if (type == kPEAKBIT_Peak1) {
        want = kSONGTAG_Peak1Amb;
    } else if (type == kPEAKBIT_Peak2) {
        want = kSONGTAG_Peak2Amb;
    } else {
        want = kSONGTAG_Peak3Amb;
    }
    if (want != cur) {
        StartLocationMusic(0, 0, type, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StartRiderMusicEvent);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);

struct sBmVt630b { short delta; short index; int (*fn)(void*, int); };

/**
 * Starts rider music event @p id for the rider @p obj, which then owns the rider-event slot.
 *
 * Does nothing during a big challenge (mInBigChallenge or mBigChallengeStarting) or when the event in mRiderMusicEvent
 * is already @p id. With no owner (mMusicEventOwner == kNoMusicEventOwner) only the focus rider
 * (cSSXAudio_GetFocusRider(-1)) can start one, and its AI index (getAIIndex, cRider virtual slot 7) becomes
 * mMusicEventOwner; otherwise @p obj must have the owner's AI index. It then sends the previous event + 1 (if one was
 * running) and @p id through MusicSys.mVtbl slot 4, stores @p id in mRiderMusicEvent and clears mNextRiderMusicEvent.
 * Called by func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
void cSSXAudio::StartRiderMusicEvent(int id, cRider* obj)
{
    int ok;
    if (mInBigChallenge != 0) return;
    if (mBigChallengeStarting != 0) return;
    if (mRiderMusicEvent == id) return;
    ok = 0;
    if (mMusicEventOwner == kNoMusicEventOwner) {
        // PORT: cSSXAudio_GetFocusRider returns an object pointer as int
        if ((int)obj == cSSXAudio_GetFocusRider(this, -1)) {
            ok = 1;
            mMusicEventOwner = obj->getAIIndex();
        }
    } else {
        ok = obj->getAIIndex() == mMusicEventOwner;
    }
    if (ok) {
        int n = mRiderMusicEvent;
        char* mon = (char*)&MusicSys;
        if (n != 0) {
            sBmVt630b* vt = (sBmVt630b*)MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), n + 1);
        }
        mRiderMusicEvent = id;
        mNextRiderMusicEvent = 0;
        {
            sBmVt630b* vt = (sBmVt630b*)MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_EndRiderMusicEvent);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ReleaseRiderMusicEvent(void* self, void* a1);

/**
 * Ends rider music event @p id for the owning rider @p obj, or records it as the follow-up of the running one.
 *
 * Does nothing when mNextRiderMusicEvent already holds @p id or when @p obj is not the owner (its AI index, getAIIndex,
 * differs from mMusicEventOwner; no owner counts as not). When the running event (mRiderMusicEvent) is @p id, @p id is
 * stored in mNextRiderMusicEvent; otherwise cSSXAudio_ReleaseRiderMusicEvent releases the slot. Called by func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
void cSSXAudio::EndRiderMusicEvent(int id, cRider* obj)
{
    int ok;
    if (mNextRiderMusicEvent == id) {
        return;
    }
    ok = 0;
    if (mMusicEventOwner != kNoMusicEventOwner) {
        ok = obj->getAIIndex() == mMusicEventOwner;
    }
    if (ok) {
        if (mRiderMusicEvent == id) {
            mNextRiderMusicEvent = id;
            return;
        }
        cSSXAudio_ReleaseRiderMusicEvent(this, obj);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_ReleaseRiderMusicEvent);
#ifdef SKIP_ASM
struct sBmVtD7D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares cSSXAudio_ReleaseRiderMusicEvent as returning void; it really returns 0/1.
int cSSXAudio_ReleaseRiderMusicEvent_impl(cSSXAudio* self, cRider* obj) __asm__("cSSXAudio_ReleaseRiderMusicEvent");

/**
 * Releases the rider-event slot that cSSXAudio_StartRiderMusicEvent gave to a rider and ends its music event.
 *
 * With @p obj null, or when @p obj's AI index (getAIIndex) equals the owner mMusicEventOwner, it sets mMusicEventOwner
 * to -1; then, if an event is running (mRiderMusicEvent), it sends that event + 1 through MusicSys.mVtbl slot 4 and
 * clears mRiderMusicEvent and mNextRiderMusicEvent. The unit declares it void although it returns the flag.
 *
 * @param obj  the rider (cRider*), or null to release unconditionally
 * @return 1 when the slot was released, else 0
 */
int cSSXAudio::ReleaseRiderMusicEvent(cRider* obj)
{
    int ok = 0;
    if (obj == 0) {
        ok = 1;
        mMusicEventOwner = -1;
    } else {
        int none = -1;
        if (mMusicEventOwner != none) {
            if (obj->getAIIndex() == mMusicEventOwner) {
                mMusicEventOwner = none;
                ok = 1;
            }
        }
    }
    if (ok) {
        int n = mRiderMusicEvent;
        if (n != 0) {
            char* mon = (char*)&MusicSys;
            sBmVtD7D8* vt = (sBmVtD7D8*)MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), n + 1);
            mRiderMusicEvent = 0;
            mNextRiderMusicEvent = 0;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_GetRiderMusicEvent__FPv);
#ifdef SKIP_ASM
/**
 * Returns the running rider music event (the word at 0x623C that cSSXAudio_StartRiderMusicEvent sets; 0 when none).
 *
 * @return the event id, or 0
 */
int cSSXAudio::GetRiderMusicEvent()
{
    return mRiderMusicEvent;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_GetSongCategory);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_IsPeakTimeEvent(void* self);
extern "C" int cSSXAudio_IsPeakPointsEvent(void* self);
extern "C" int cSSXAudio_IsRaceLocation(void* self, int id);
extern "C" int cSSXAudio_IsSlopestyleLocation(void* self, int id);
extern "C" int cSSXAudio_IsBigAirLocation(void* self, int id);
extern "C" int cSSXAudio_IsHalfpipeLocation(void* self, int id);

/**
 * Returns the song category (0-4) for the race in progress.
 *
 * From cBENewRaceInterface's location and game event: 0 when cSSXAudio_IsPeakTimeEvent (game events 6-8) or cSSXAudio_IsRaceLocation (location
 * below 5) holds, 1 for cSSXAudio_IsPeakPointsEvent (events 9-11) or cSSXAudio_IsSlopestyleLocation (locations 5-7), 2 for cSSXAudio_IsBigAirLocation (8-10), 3 for
 * cSSXAudio_IsHalfpipeLocation (11-13), else 4; the event tests come first. cSSXAudio_PickRandomSong filters songs by it (mCategory).
 *
 * @return the category index
 */
int cSSXAudio::GetSongCategory()
{
    int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    int result;
    if (cSSXAudio_IsPeakTimeEvent(this)) {
        result = 0;
    } else if (cSSXAudio_IsPeakPointsEvent(this)) {
        result = 1;
    } else if (cSSXAudio_IsRaceLocation(this, id)) {
        result = 0;
    } else if (cSSXAudio_IsSlopestyleLocation(this, id)) {
        result = 1;
    } else if (cSSXAudio_IsBigAirLocation(this, id)) {
        result = 2;
    } else if (cSSXAudio_IsHalfpipeLocation(this, id)) {
        result = 3;
    } else {
        result = 4;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_IsMusicRadioMode);
#ifdef SKIP_ASM
/**
 * Returns whether the radio plays music: true when mRadioModeID is kRADIOMODE_RADIO, kRADIOMODE_REQUESTLINE or
 * kRADIOMODE_PLAYLIST, false for kRADIOMODE_MOUNTAIN and above kRADIOMODE_PLAYLIST.
 *
 * @return true when the radio plays music
 */
bool cSSXAudio::IsMusicRadioMode()
{
    unsigned int state = (unsigned int)mRadioModeID;
    if (state < kRADIOMODE_MOUNTAIN || state == kRADIOMODE_PLAYLIST) {
        return true;
    }
    return false;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D988);

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetLastMusicState);
#ifdef SKIP_ASM
/**
 * Stores @p newValue in mLastMusicState and returns whether it differed from the old value.
 *
 * Callers pass 11, 13 and 18; cSSXAudio_ResetLastMusicState resets the field to -1.
 *
 * @param newValue  the new value
 * @return true when the value changed, else false
 */
bool cSSXAudio::SetLastMusicState(int newValue)
{
    int old = mLastMusicState;
    mLastMusicState = newValue;
    return old != newValue;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_ResetLastMusicState__FPv);
#ifdef SKIP_ASM
void cSSXAudio_ResetLastMusicState(void* vself);

/**
 * Sets mLastMusicState to -1, so that the next cSSXAudio_SetLastMusicState call reports a change.
 */
void cSSXAudio::ResetLastMusicState()
{
    mLastMusicState = -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SwitchToNextSong);
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
extern "C" int cSSXAudio_IsCharacterSpeaking(void* self, int id);
extern "C" void cMusicSys_FadeOut(void* self, float v);
int cMusicSys_GetCurrentSongSedValue(void* self);

struct sBmVtDF18 {
    short delta;
    short index;
    void* fn;
};

typedef int (*tBmSetDF18)(void*, int);

/**
 * Switches to the next song unless cSSXAudio_IsCharacterSpeaking says speech 10 is playing; otherwise retries later unless @p quiet.
 *
 * When cSSXAudio_IsCharacterSpeaking(self, 10) is 0: fades the song out over 2 s (cMusicSys_FadeOut) unless mDJSpeech is set, sends music
 * event 18 through MusicSys.mVtbl slot 4, picks a new song (cSSXAudio_PickRandomSong) and adds two timed callbacks to the
 * cBankManager's cBankMonitor (cMonitor_MONITOR_Create): cSSXAudio_PostMusicRequest after 2000 with data 1, 1 and cSSXAudio_HandleMusicChangeEvent after 1500
 * with data 3 and the new song's SEDVALUE (cMusicSys_GetCurrentSongSedValue). Otherwise, unless @p quiet, it adds cSSXAudio_RetrySwitchToNextSong after 100,
 * which calls this again quietly.
 *
 * @param quiet  non-zero: don't schedule a retry
 * @return 1 when the switch started, else 0
 */
// PORT: 64-bit long sound key
int cSSXAudio::SwitchToNextSong(int quiet)
{
    int ok = 0;
    if (cSSXAudio_IsCharacterSpeaking(this, 10) == 0) {
        char* mon = (char*)&MusicSys;
        if (mDJSpeech == 0) {
            cMusicSys_FadeOut(mon, 2.0f);
        }
        sBmVtDF18* vt = (sBmVtDF18*)MusicSys.mVtbl;
        ok = 1;
        ((tBmSetDF18)vt[4].fn)(vthis(mon, vt[4]), 18);
        PickRandomSong();
        cMonitor_MONITOR_Create_DF18(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                     2000, D_004A3648_DF18->v, 0, 1, 1, 0, 0);
        cBankMonitor* bankMonitor = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        const sSndKey8_DF18* key = D_004A3650_DF18;
        int sedValue = cMusicSys_GetCurrentSongSedValue(mon);
        cMonitor_MONITOR_Create_DF18(bankMonitor, 1500, key->v, 0, 3, sedValue, 0, 0);
    } else if (quiet == 0) {
        cMonitor_MONITOR_Create_DF18(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                     100, D_004A3658_DF18->v, 0, 0, 0, 0, 0);
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_RetrySwitchToNextSong);
#ifdef SKIP_ASM
/**
 * Retries cSSXAudio_SwitchToNextSong quietly: the timed callback cSSXAudio_SwitchToNextSong schedules when it cannot switch songs yet.
 *
 * @return true when cSSXAudio_SwitchToNextSong started the switch, else false
 */
bool cSSXAudio::RetrySwitchToNextSong()
{
    return ((cSSXAudio*)this)->SwitchToNextSong(1) != 0;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PostMusicRequest);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetPathStatus(int);

/**
 * Timed callback that asks cSSXAudio_HandleMusicRequest to act once the old song has stopped.
 *
 * When there is no current song (MusicSys.mSongInstance) or cSongInstance_GetPathStatus of it returns -1, or when @p event's mData2
 * is set, it sets mMusicRequestPending to 1 and mMusicRequest to @p event's mData1 and returns true; otherwise it
 * returns false. cSSXAudio_SwitchToNextSong schedules it with data 1, 1.
 *
 * @param event  the cMonitorInstance that fired
 * @return true when the request was posted, else false
 */
bool cSSXAudio::PostMusicRequest(cMonitorInstance* event)
{
    int obj;
    int eventValue;

    obj = (int)MusicSys.mSongInstance;
    if (obj == 0 || cSongInstance_GetPathStatus(obj) == -1) {
        mMusicRequestPending = 1;
        mMusicRequest = event->mData1;
        return true;
    }
    eventValue = event->mData2;
    if (eventValue == true) {
        mMusicRequestPending = eventValue;
        mMusicRequest = event->mData1;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_HandleMusicRequest);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void* cAudioInterface_getBE();
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_PickRandomSong(cSSXAudio* self);
extern "C" bool cSSXAudio_IsMusicRadioMode(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationPeakBit(void* self, int id);
extern "C" int cSSXAudio_IsBackCountryLocation(void* self);
extern "C" void cMusicSys_SetVolume(void* self, int a1);
extern "C" void cMusicSys_SetSongTag(void* self, int a1);
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
    cMusicSys_SetSongTag(mon, n);
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
    cMusicSys_SetVolume(mon, 0x7F);
}

/**
 * Carries out the music request that cSSXAudio_PostMusicRequest posted; called every frame from cSSXAudio_Update while
 * mAudioUpdateState is 6.
 *
 * Does nothing unless mMusicRequestPending is set; it is cleared at the end. Request 0 (mMusicRequest): when the radio
 * plays music (cSSXAudio_IsMusicRadioMode), starts the peak theme for mTransportTrack's location (resetting mTransportTrack to
 * kMAX_LOCATIONS), or for the race location when mTransportTrack is already kMAX_LOCATIONS: "Peak1", "Peak2" or "Peak3"
 * by cSSXAudio_GetLocationPeakBit (kPEAKBIT_Peak1, kPEAKBIT_Peak2, else peak 3), on the music control, tagged kSONGTAG_Peak1-Peak3,
 * event 12 and cMusicSys_SetVolume. Request 2: when the radio plays music and mTransportTrack is not kMAX_LOCATIONS: at the
 * back-country locations kLOCATION_ABC1-EBC3 (cSSXAudio_IsBackCountryLocation) a new song (cSSXAudio_PickRandomSong) and cSSXAudio_StartLocationMusic(self, 36, 1,
 * kPEAKBIT_FromLocation, 0), otherwise the peak theme for mTransportTrack; mTransportTrack is reset to kMAX_LOCATIONS.
 * Request 4: cSSXAudio_StartLocationMusic(self, 0, 0, kPEAKBIT_FromLocation, 0). Other requests: when the radio plays music,
 * cSSXAudio_StartLocationMusic with event 36 for request 3, else 0.
 */
void cSSXAudio::HandleMusicRequest()
{
    if (mMusicRequestPending == 0)
        return;
    int st = mMusicRequest;
    int id;
    if (st == 0) {
        if (IsMusicRadioMode()) {
            int locGroup;
            id = mTransportTrack;
            if (id == kMAX_LOCATIONS) {
                locGroup = cSSXAudio_GetLocationPeakBit(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            } else {
                locGroup = cSSXAudio_GetLocationPeakBit(this, id);
                mTransportTrack = kMAX_LOCATIONS;
            }
            bmLoadBank_E100(this, locGroup);
        }
    } else if (st == 2) {
        if (IsMusicRadioMode()) {
            id = mTransportTrack;
            if (id != kMAX_LOCATIONS) {
                if (cSSXAudio_IsBackCountryLocation(this)) {
                    PickRandomSong();
                    StartLocationMusic(36, 1, kPEAKBIT_FromLocation, 0);
                } else {
                    id = mTransportTrack;
                    bmLoadBank_E100(this, cSSXAudio_GetLocationPeakBit(this, id));
                }
                mTransportTrack = kMAX_LOCATIONS;
            }
        }
    } else if (st == 4) {
        StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
    } else {
        if (IsMusicRadioMode()) {
            if (mMusicRequest == 3)
                StartLocationMusic(36, 0, kPEAKBIT_FromLocation, 0);
            else
                StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
        }
    }
    mMusicRequestPending = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetMusicStateCallback);
#ifdef SKIP_ASM
struct sBmVtE510 { short delta; short index; int (*fn)(void*, int); };

/**
 * Monitor callback that sets the music state to the instance's mData1 through cMusicSys's virtual slot 4.
 *
 * On gSSXAudio the slot is cSSXAudio_SetMusicState (cSSXAudio's override). Returns 1, so the callback runs once.
 *
 * @param ev  the cMonitorInstance (mData1 = the music state)
 * @return 1
 */
int cSSXAudio::SetMusicStateCallback(cMonitorInstance* ev)
{
    char* mon = (char*)&MusicSys;
    sBmVtE510* vt = (sBmVtE510*)MusicSys.mVtbl;
    vt[4].fn(vthis(mon, vt[4]), ev->mData1);
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_HandleMusicChangeEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_GetLocationPeakBit(void* self, int id);
extern "C" int cSSXAudio_IsRaceLocation(void* self, int id);
extern "C" int cSSXAudio_IsSlopestyleLocation(void* self, int id);
extern "C" void cSSXAudio_SpeakDJChoiceLine(void* self, int a1, int a2);
extern "C" int cSSXAudio_IsPeakChallengeEvent(void* self);
extern "C" void cSpeechSys_PurgeInstance(void* self, int a1);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* iface, int a1, int a2);
void cSpeechSys_StartNextSpeech(void* self, int a1);
int cMusicSys_GetCurrentSongSedValue(void* self);

struct sLocks_E548 {
    int v[1];
};

/**
 * Monitor callback for delayed music-change events: queues DJ speech and sets the speech-cue flags by event kind.
 *
 * The kind is mData1 and the argument mData2 (armed through D_004A3650 by cSSXAudio_HandleMusicEvent, cSSXAudio_RestartMusicAfterMap and others).
 * Kinds 0, 1, 2, 3, 4 and 7 purge speech instance 0 (cSpeechSys_PurgeInstance), queue speech cue 8421 with cSSXAudio_SpeakDJChoiceLine(self, a,
 * b) ((1, 0) for 0, 3 and 7; (1, 1) for 1; (0, 1) for 2; (0, 0) for 4) and restart speech (cSpeechSys_StartNextSpeech). Outside
 * newRace.cGameEvent modes 6-11 (cSSXAudio_IsPeakChallengeEvent): kind 0 sets mPendingPeakLine8378 and, in a peak-1 location
 * (cSSXAudio_GetLocationPeakBit kPEAKBIT_Peak1), mPendingLine8492 = 1, mLine8492Played = 2 and mFirstTimeOnTrack[kLOCATION_ABC1] = 0;
 * kinds 1 and 2 set mPendingBetweenEventsLine.
 *
 * Kinds 3, 4 and 5 act only when mData2 (a song's SEDVALUE) is not kNoSedValue: with the peak locked
 * (cBENewPlayerInterface_isPeakLocked1) in a peak-1 location whose mFirstTimeOnTrack entry is 1, they set
 * mPendingLocationLine when cSSXAudio_IsRaceLocation or cSSXAudio_IsSlopestyleLocation accept the location and clear the entry; then
 * mSongAnnouncePending = 1 and mSongAnnounceNum = mData2. Kind 7 sets mPendingPeakLine8491 and mSongAnnouncePending,
 * stores the current song's mSedValue (cMusicSys_GetCurrentSongSedValue) in mSongAnnounceNum and clears mFirstTimeOnTrack[kLOCATION_DBC2]
 * (a peak-2 location) or mFirstTimeOnTrack[kLOCATION_EBC3]. Other kinds do nothing.
 *
 * @param ev  the cMonitorInstance (mData1 = event kind, mData2 = its argument)
 * @return 1
 */
int cSSXAudio::HandleMusicChangeEvent(cMonitorInstance* ev)
{
    int st = ev->mData1;
    if (st == 0) {
        char* mon = (char*)&SpeechSys;
        cSpeechSys_PurgeInstance(mon, 0);
        cSSXAudio_SpeakDJChoiceLine(this, 1, 0);
        cSpeechSys_StartNextSpeech(mon, 0);
        if (cSSXAudio_IsPeakChallengeEvent(this) == 0) {
            mPendingPeakLine8378 = 1;
            int locGroup = cSSXAudio_GetLocationPeakBit(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            if (locGroup == kPEAKBIT_Peak1) {
                mPendingLine8492 = locGroup;
                mLine8492Played = 2;
                mFirstTimeOnTrack[kLOCATION_ABC1] = 0;
            }
        }
    } else if (st == 1 || st == 2) {
        cSpeechSys_PurgeInstance(&SpeechSys, 0);
        if (ev->mData1 == 2)
            cSSXAudio_SpeakDJChoiceLine(this, 0, 1);
        else
            cSSXAudio_SpeakDJChoiceLine(this, 1, 1);
        cSpeechSys_StartNextSpeech(&SpeechSys, 0);
        if (cSSXAudio_IsPeakChallengeEvent(this) == 0)
            mPendingBetweenEventsLine = 1;
    } else if (st == 3 || st == 5 || st == 4) {
        if (ev->mData2 != kNoSedValue) {
        if (ev->mData1 == 3 || ev->mData1 == 4) {
            cSpeechSys_PurgeInstance(&SpeechSys, 0);
            if (ev->mData1 == 4)
                cSSXAudio_SpeakDJChoiceLine(this, 0, 0);
            else
                cSSXAudio_SpeakDJChoiceLine(this, 1, 0);
            cSpeechSys_StartNextSpeech(&SpeechSys, 0);
        }
        if (cSSXAudio_IsPeakChallengeEvent(this) == 0 && cBENewPlayerInterface_isPeakLocked1(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1) != 0) {
            int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0));
            int locGroup = cSSXAudio_GetLocationPeakBit(this, id);
            if (locGroup == kPEAKBIT_Peak1) {
                int lock = ((sLocks_E548*)(&mFirstTimeOnTrack[0]))->v[id];
                if (lock == locGroup) {
                    if (cSSXAudio_IsRaceLocation(this, id) != 0 || cSSXAudio_IsSlopestyleLocation(this, id) != 0)
                        mPendingLocationLine = lock;
                    ((sLocks_E548*)(&mFirstTimeOnTrack[0]))->v[id] = 0;
                }
            }
        }
        mSongAnnouncePending = 1;
        mSongAnnounceNum = ev->mData2;
        }
    } else if (st == 7) {
        char* mon = (char*)&SpeechSys;
        cSpeechSys_PurgeInstance(mon, 0);
        cSSXAudio_SpeakDJChoiceLine(this, 1, 0);
        cSpeechSys_StartNextSpeech(mon, 0);
        mPendingPeakLine8491 = 1;
        mSongAnnouncePending = 1;
        mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(&MusicSys);
        if (cSSXAudio_GetLocationPeakBit(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)))
            == kPEAKBIT_Peak2)
            mFirstTimeOnTrack[kLOCATION_DBC2] = 0;
        else
            mFirstTimeOnTrack[kLOCATION_EBC3] = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_EndTransportMusic);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_HandleMusicEvent(cSSXAudio* self, int id, int a2);

/**
 * Ends the transport music: when mInTransport is set (cSSXAudio_HandleMusicEvent event 20) it runs event 19 and clears it.
 *
 * Called with gSSXAudio by cSSXScriptEngine_ClearNISPlayListState when the playing NIS list ends.
 */
void cSSXAudio::EndTransportMusic()
{
    if (mInTransport != 0) {
        HandleMusicEvent(19, 1);
        mInTransport = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_HandleMusicEvent);
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
extern "C" int cSSXAudio_IsAtHub(void* self);
extern "C" int cSSXAudio_IsInBackCountry(void* self);
// PORT: cSSXAudio_IsBackCountryLocation takes (self, id); the unit declares it with one parameter (cSSXAudio_HandleMusicRequest).
extern "C" int cSSXAudio_IsBackCountryLocation_id(void* self, int id) __asm__("cSSXAudio_IsBackCountryLocation");
extern "C" int cSSXAudio_IsFreeride(void* self);
extern "C" int cSSXAudio_IsRaceEvent(void* self);
extern "C" int cSSXAudio_IsSlopestyleEvent(void* self);
extern "C" void cSSXAudio_ClearPendingLines(void* self);
extern "C" int cSSXAudio_HasTransportTrackChanged(void* self);
extern "C" int cSSXAudio_IsScriptedDJLinePlaying(void* self);
extern "C" char* cAudioInterface_getAI();
extern "C" void cSSXAudio_RefreshPeakAmbience(cSSXAudio* self, int id);
extern "C" void cSSXAudio_WipeoutSlideEnd(void* self, int id);
extern "C" void cSSXAudio_WipeoutTumbleEnd(void* self, int id);
extern "C" void cSSXAudio_EndUberSpin(void* self, int a1);
extern "C" void cSSXAudio_StopTone(void* self, float v);
extern "C" void cMusicSys_PauseSong(void* self);

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
 * Id 19 with a2 set and mRaceMusicOn set: when the location qualifies (cSSXAudio_IsInBackCountry) and mFirstTimeOnPeak is 0, in a
 * music mode accepted by cSSXAudio_IsMusicRadioMode and a newRace.cGameMode other than kGAMEMODE_TIMECHALLENGE and
 * kGAMEMODE_PNTSCHALLENGE, it arms the D_004A3650 event (cSSXAudio_HandleMusicChangeEvent) with kind 4 (mBonusLineQueued clear) or 5 and
 * the song's mSedValue, and sets music state 0. With mFirstTimeOnPeak set it fades the song out over 2 s (unless
 * mDJSpeech), then in a peak-1 location (kPEAKBIT_Peak1) arms events 0 and 1 (3000 / 2500 ms) once (cSSXAudio_SetLastMusicState(self,
 * 11)), or else picks a new song (cSSXAudio_PickRandomSong) and arms kinds 1 and 7. Otherwise, for the stations
 * kLOCATION_HUB_A-HUB_E (cSSXAudio_IsAtHub) without mBonusLineQueued, it queues DJ speech (cSSXAudio_SpeakDJChoiceLine(self, 0, 1)) and
 * sets mPendingBetweenEventsLine. Clears mBonusLineQueued at the end.
 *
 * Id 20 with a2 set: stores the location (mTransportTrack) in mLastTransportTrack, sets mInTransport, clears the
 * speech-cue flags (cSSXAudio_ClearPendingLines) and the three pending callbacks D_004A3648..D_004A3658
 * (cMonitor_MONITOR_DestroyAll1), stops the tone, cSSXAudio_WipeoutSlideEnd / cSSXAudio_WipeoutTumbleEnd on rider 0 and
 * cSSXAudio_EndUberSpin. In a qualifying music mode it either plays the "pktrans" song (cMusicSys slot 2, tagged
 * kSONGTAG_PkTrans, state 2 for a peak-2 location else 3, volume 127, paused) for a back-country location
 * (kLOCATION_ABC1-EBC3) ridden for the first time (mFirstTimeOnTrack), or arms D_004A3648 kinds 3 / 2 and may queue
 * speech; otherwise it calls cSSXAudio_RefreshPeakAmbience(mTransportTrack) and sets state 30. Then sets mRaceMusicOn. Ids 21-25 pick a
 * new song, fade out over 1 s and arm kind 3 after 3000 ms. Any other id (and 19 / 20 with a2 == 0) sets the music
 * state through slot 4.
 *
 * The NIS script engine passes its audio events here with a2 = 0; the transport-sequence starter passes 20 and
 * cSSXAudio_EndTransportMusic passes 19, both with 1.
 *
 * @param vself  the cSSXAudio
 * @param id  the music event id (a music state for the default case)
 * @param transportCall  nonzero to treat ids 19 and 20 as the transport end / start
 */
// PORT: 64-bit long sound key
void cSSXAudio::HandleMusicEvent(int id, int transportCall)
{
    if (id == 19 && transportCall != 0 && mRaceMusicOn != 0) {
        if (cSSXAudio_IsInBackCountry(((char*)this)) != 0 && mFirstTimeOnPeak == 0) {
            if (IsMusicRadioMode()) {
                cBE_getInterface_Fv(cBE_getBE(), 0);
                int mode = cBENewRaceInterface_newRace.cGameMode;
                if (mode != kGAMEMODE_TIMECHALLENGE && mode != kGAMEMODE_PNTSCHALLENGE) {
                    if (mBonusLineQueued == 0) {
                        cBankMonitor* monitor = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = cMusicSys_GetCurrentSongSedValue(&MusicSys);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 4, sedValue, 0, 0);
                    } else {
                        cBankMonitor* monitor = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = cMusicSys_GetCurrentSongSedValue(&MusicSys);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 5, sedValue, 0, 0);
                    }
                    char* mon = (char*)&MusicSys;
                    sBmVtE8C0* vt = (sBmVtE8C0*)MusicSys.mVtbl;
                    ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), 0);
                }
            }
        } else if (mFirstTimeOnPeak != 0) {
            if (IsMusicRadioMode()) {
                // NOTE: the target keeps this 1 in $s1 across five calls (a named local, not a literal).
                int wantType = kPEAKBIT_Peak1;
                int locationGroup = cSSXAudio_GetLocationPeakBit(
                        ((char*)this),
                        *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
                if (mDJSpeech == 0) {
                    cMusicSys_FadeOut(&MusicSys, 2.0f);
                }
                if (locationGroup == wantType) {
                    if (SetLastMusicState(11)) {
                        cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                3000, D_004A3648_E8C0->v, 0, 0, 1, 0, 0);
                        cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                2500, D_004A3650_E8C0->v, 0, 0, 0, 0, 0);
                    }
                } else {
                    cSSXAudio_ResetLastMusicState(((char*)this));
                    PickRandomSong();
                    cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            2000, D_004A3648_E8C0->v, 0, 1, 1, 0, 0);
                    cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            1500, D_004A3650_E8C0->v, 0, 7, 0, 0, 0);
                }
            }
        } else {
            if (cSSXAudio_IsAtHub(((char*)this)) && mBonusLineQueued == 0) {
                cSSXAudio_SpeakDJChoiceLine(((char*)this), 0, 1);
                cSpeechSys_StartNextSpeech(&SpeechSys, 0);
                mPendingBetweenEventsLine = 1;
            }
        }
        mBonusLineQueued = 0;
    } else if (id == 20) {
        if (transportCall != 0) {
            mBonusLineQueued = 0;
            mPendingLine8492 = 0;
            mInTransport = 1;
            mLastTransportTrack = mTransportTrack;
            cSSXAudio_ResetLastMusicState(((char*)this));
            char* mon = (char*)&MusicSys;
            mFirstTimeOnPeak = 0;
            cSSXAudio_ClearPendingLines(((char*)this));
            long key = D_004A3648_E8C0->v;
            cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, key);
            cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                         D_004A3650_E8C0->v);
            cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                         D_004A3658_E8C0->v);
            cSSXAudio_StopTone(((char*)this), 5.029983997344971f);
            cSSXAudio_WipeoutSlideEnd(((char*)this), (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_WipeoutTumbleEnd(((char*)this), (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_EndUberSpin(((char*)this), 0);
            if (IsMusicRadioMode()) {
                int flag = 1;
                if (cSSXAudio_IsFreeride(((char*)this)) == 0) {
                    if (cSSXAudio_IsRaceEvent(((char*)this)) || cSSXAudio_IsSlopestyleEvent(((char*)this))) {
                        if (*(int*)SSXApp->GameModeMan >= 2) {
                            PickRandomSong();
                            flag = 0;
                            cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                    10, key, 0, 3, 1, 0, 0);
                        }
                    }
                }
                if (mTransportTrack == kMAX_LOCATIONS
                    || cSSXAudio_IsBackCountryLocation_id(((char*)this), mTransportTrack) == 0
                    || mFirstTimeOnTrack[mTransportTrack] != true) {
                    if (flag && mTransportTrack != kMAX_LOCATIONS) {
                        PickRandomSong();
                        cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                10, D_004A3648_E8C0->v, 0, 2, 1, 0, 0);
                    }
                    if (cSSXAudio_HasTransportTrackChanged(((char*)this))
                        && cSSXAudio_IsScriptedDJLinePlaying(((char*)this)) == 0) {
                        cSSXAudio_SpeakDJChoiceLine(((char*)this), 0, 1);
                        cSpeechSys_StartNextSpeech(&SpeechSys, 0);
                        mPendingBonusBetweenEventsLine = 1;
                        mBonusLineQueued = 1;
                    }
                } else {
                    char* mon2 = (char*)&MusicSys;
                    if (cSSXAudio_IsScriptedDJLinePlaying(((char*)this)) == 0) {
                        cSpeechSys_PurgeInstance(&SpeechSys, 0);
                    }
                    sBmVtE8C0* vt = (sBmVtE8C0*)MusicSys.mVtbl;
                    ((tBmLoadE8C0)vt[2].fn)(vthis(mon2, vt[2]), str_pktrans, 0xC,
                                            cSSXAudio_GetCurrAppliedVolumePtr(((char*)this), 1, 0), 1);
                    cMusicSys_SetSongTag(mon2, kSONGTAG_PkTrans);
                    if (cSSXAudio_GetLocationPeakBit(((char*)this), mTransportTrack) == kPEAKBIT_Peak2) {
                        sBmVtE8C0* vt2 = (sBmVtE8C0*)MusicSys.mVtbl;
                        ((tBmSetE8C0)vt2[4].fn)(vthis(mon2, vt2[4]), 2);
                    } else {
                        sBmVtE8C0* vt2 = (sBmVtE8C0*)MusicSys.mVtbl;
                        ((tBmSetE8C0)vt2[4].fn)(vthis(mon2, vt2[4]), 3);
                    }
                    cMusicSys_SetVolume(mon2, 0x7F);
                    cMusicSys_PauseSong(mon2);
                }
            } else {
                RefreshPeakAmbience(mTransportTrack);
                sBmVtE8C0* vt = (sBmVtE8C0*)MusicSys.mVtbl;
                ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), 0x1E);
            }
            mRaceMusicOn = 1;
        }
    } else if (id >= 21 && id <= 25) {
        if (IsMusicRadioMode()) {
            PickRandomSong();
            cMusicSys_FadeOut(&MusicSys, 1.0f);
            cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    3000, D_004A3648_E8C0->v, 0, 3, 1, 0, 0);
        }
    } else {
        char* mon = (char*)&MusicSys;
        sBmVtE8C0* vt = (sBmVtE8C0*)MusicSys.mVtbl;
        ((tBmSetE8C0)vt[4].fn)(vthis(mon, vt[4]), id);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_RestartLocationMusic);
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
 * Clears mBonusLineQueued, mPendingLine8492, the last music event (cSSXAudio_ResetLastMusicState) and mFirstTimeOnPeak, picks the next
 * song (cSSXAudio_PickRandomSong) and arms the D_004A3648 callback (cSSXAudio_PostMusicRequest) after 10 ms with data (2, 1). Called by
 * cSSXAudio_SetTransportTrack.
 */
// PORT: 64-bit long sound key
void cSSXAudio::RestartLocationMusic()
{
    mBonusLineQueued = 0;
    mPendingLine8492 = 0;
    ResetLastMusicState();
    mFirstTimeOnPeak = 0;
    PickRandomSong();
    cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                            10, D_004A3648_EF90->v, 0, 2, 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_UpdateMusicRamp);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_GetCharacterLevel(void* self, int rider);

/**
 * Ramps the music volume up over 800, 1000 or 1200 updates depending on the focus rider's tier (cSSXAudio_GetCharacterLevel).
 *
 * When the current song's tag (cMusicSys_GetSongTag) is a peak theme (kSONGTAG_Peak1-Peak3) it sets full volume
 * (cMusicSys_SetVolume(127)). Otherwise it starts a new ramp when mMusicRampState is not kMUSICRAMP_Ramping (it then becomes
 * kMUSICRAMP_Ramping, counter mMusicRampCounter = 0), advances the counter up to the tier's maximum and sets the volume
 * to 127 x counter / maximum, clamped to 0..1. Called by cSSXAudio_Update.
 */
void cSSXAudio::UpdateMusicRamp()
{
    char* mon = (char*)&MusicSys;
    int st = cMusicSys_GetSongTag(mon);
    if (st >= kSONGTAG_Peak1 && st <= kSONGTAG_Peak3) {
        cMusicSys_SetVolume(mon, 0x7F);
        return;
    }
    if (mMusicRampState != kMUSICRAMP_Ramping) {
        mMusicRampState = kMUSICRAMP_Ramping;
        mMusicRampCounter = 0;
    }
    int max = 800;
    int tier = cSSXAudio_GetCharacterLevel(this, cSSXAudio_GetFocusRider(this, -1));
    if (tier == 1) {
        max = 1000;
    } else if (tier == 2) {
        max = 1200;
    }
    int cur = mMusicRampCounter;
    float ratio = (float)cur / (float)max;
    if (cur < max) {
        mMusicRampCounter = cur + 1;
    } else {
        mMusicRampCounter = max;
    }
    ratio = ratio <? 1.0f;
    ratio = ratio >? 0.0f;
    cMusicSys_SetVolume(&MusicSys, (int)(ratio * 127.0f));
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028F108);

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetFrontEndMusicMode);
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
 * code (cMusicSys mMusicState) is not -1; modes 1 and 3-8 map to the same state; 2 and 9 map to 2; other values set
 * nothing. Called by the front-end states (title 0, rider and lodge screens 1, store 5, trophy room 6, ...).
 *
 * @param mode  the front-end music mode
 */
void cSSXAudio::SetFrontEndMusicMode(int mode)
{
    int bank;
    if (mode == mFrontEndMusicMode) {
        return;
    }
    mFrontEndMusicMode = mode;
    switch (mode) {
    case 0:
        bank = (MusicSys.mMusicState == -1) ? 0 : 9;
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
        char* mon = (char*)&MusicSys;
        sBmVtF140* vt = (sBmVtF140*)MusicSys.mVtbl;
        ((tBmSetF140)vt[4].fn)(vthis(mon, vt[4]), bank);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_BeginReplayMusic);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsMusicRadioMode(cSSXAudio* self);
extern "C" int cMusicSys_GetSongTag(void* monitor);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);

struct sSndKeyF200 {
    long v;
} __attribute__((packed));
extern const sSndKeyF200 D_004A3648[];

/**
 * Prepares the music for a replay: with no song (or the podium tune) arms a delayed change, else marks a resume.
 *
 * Clears mReplayMusicStarted and mReplaySongPlaying. When no song is playing (mSongPlaying 0) or the current song's tag
 * is kSONGTAG_CharTune: in a music mode accepted by cSSXAudio_IsMusicRadioMode it restores mCurrentSongNum from mSavedSongNum for
 * that song and arms D_004A3648 (cSSXAudio_PostMusicRequest) after 10 ms with data (3, 1); then sets mReplayMusicStarted. Otherwise
 * sets mReplaySongPlaying. Called from the replay code; cSSXAudio_EndReplayMusic undoes it.
 */
// PORT: 64-bit long sound key
void cSSXAudio::BeginReplayMusic()
{
    char* mon = (char*)&MusicSys;
    mReplayMusicStarted = 0;
    mReplaySongPlaying = 0;
    if (MusicSys.mSongPlaying == 0 || cMusicSys_GetSongTag(mon) == kSONGTAG_CharTune) {
        if (IsMusicRadioMode()) {
            if (cMusicSys_GetSongTag(mon) == kSONGTAG_CharTune) {
                MusicSys.mCurrentSongNum = mSavedSongNum;
            }
            cMonitor_MONITOR_Create(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    10, D_004A3648[0].v, 0, 3, 1, 0, 0);
        }
        mReplayMusicStarted = 1;
    } else {
        mReplaySongPlaying = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_EndReplayMusic);
#ifdef SKIP_ASM
extern "C" void* cMusicSys_StopSong(void*);

struct sBankMgrVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Ends the replay music: stops the song when cSSXAudio_BeginReplayMusic set mReplayMusicStarted, or sets music state 36 when
 * mReplaySongPlaying.
 *
 * Clears both flags.
 */
void cSSXAudio::EndReplayMusic()
{
    if (mReplayMusicStarted != 0) {
        cMusicSys_StopSong(&MusicSys);
    } else if (mReplaySongPlaying != 0) {
        char* obj = (char*)&MusicSys;
        sBankMgrVEntry* vt = (sBankMgrVEntry*)MusicSys.mVtbl;
        vt[4].fn(vthis(obj, vt[4]), 36);
    }
    mReplayMusicStarted = 0;
    mReplaySongPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SendMusicEvent);
#ifdef SKIP_ASM
extern "C" void cMusicSys_SendMusicEvent(void* self, int a1);
extern "C" int cMusicSys_GetSongTag(void* monitor);
extern int cSSXAudio_prevLongAirFlag;

class cBankMonitor_F328 {
public:
    char data[0x5440];
    virtual void v1();
    virtual void v2();
    virtual void SendMusicEvent(int event);  // slot 3 (name ours): cMusicSys_SetMusicState sends a new music state and
                                             // cSSXAudio_UpdateAirTimeVolume music events 7 / 8 through it
};

/**
 * cSSXAudio's override of cMusicSys virtual slot 3: calls cMusicSys_SendMusicEvent, then handles codes 10, 11 and 18.
 *
 * When the current song has no tag (cMusicSys_GetSongTag == kSONGTAG_None) and code is 10, 11 or 18 it calls cMusicSys's
 * virtual slot 3 with 8 (only while cSSXAudio_prevLongAirFlag is set) and clears mSongPlaying. Slot 3 of D_00483688.
 *
 * @param code  the music state
 */
void cSSXAudio::SendMusicEvent(int code)
{
    void* monitor = (char*)&MusicSys;
    cMusicSys_SendMusicEvent(monitor, code);
    if (cMusicSys_GetSongTag(monitor) == kSONGTAG_None) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (cSSXAudio_prevLongAirFlag == true) {
                ((cBankMonitor_F328*)(&MusicSys))->SendMusicEvent(8);
            }
            MusicSys.mSongPlaying = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetMusicState);
#ifdef SKIP_ASM
extern "C" int cMusicSys_SetMusicState(void* self, int v);

/**
 * cSSXAudio's override of cMusicSys virtual slot 4: sets the music state (cMusicSys_SetMusicState), then as cSSXAudio_SendMusicEvent.
 *
 * When the current song has no tag (kSONGTAG_None) and code is 10, 11 or 18 it calls virtual slot 3 with 8 (only while
 * cSSXAudio_prevLongAirFlag is set) and clears mSongPlaying. Slot 4 of D_00483688; the music code calls it to change state.
 *
 * @param code  the new music state
 * @return cMusicSys_SetMusicState's result (whether the state changed)
 */
int cSSXAudio::SetMusicState(int code)
{
    void* monitor = (char*)&MusicSys;
    int changed = cMusicSys_SetMusicState(monitor, code);
    if (cMusicSys_GetSongTag(monitor) == kSONGTAG_None) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (cSSXAudio_prevLongAirFlag == true) {
                ((cBankMonitor_F328*)(&MusicSys))->SendMusicEvent(8);
            }
            MusicSys.mSongPlaying = 0;
        }
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PlaySong);
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
extern "C" void cMusicSys_PlaySong(void* monitor);
extern "C" float cSSXAudio_GetCurrAppliedVolume(void* self, int a, int b);
extern "C" cUiObj_F478* cUIStateStack_findState(void* list, int hash);

/**
 * cSSXAudio's override of cMusicSys virtual slot 2: plays the song (cMusicSys_PlaySong), then refreshes the HUD.
 *
 * When music volume control 1 (cSSXAudio_GetCurrAppliedVolume(self, 1, 0)) is above 0 it finds the "hud" state on the game's UI state
 * stack (cUIStateStack_findState) and calls its virtual slot 24 with (5, 0). Slot 2 of D_00483688.
 */
void cSSXAudio::PlaySong()
{
    cMusicSys_PlaySong(&MusicSys);
    if (cSSXAudio_GetCurrAppliedVolume(this, 1, 0) > 0.0f) {
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
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StopRadioSong);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsMusicRadioMode(cSSXAudio* self);
extern "C" void* cMusicSys_StopSong(void*);

/**
 * Stops the current song (cMusicSys_StopSong) when the music mode allows music (cSSXAudio_IsMusicRadioMode).
 */
void cSSXAudio::StopRadioSong()
{
    if (IsMusicRadioMode() != 0) {
        cMusicSys_StopSong(&MusicSys);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cSSXAudio_SetTransportTrack);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void cSSXAudio_RestartLocationMusic(cSSXAudio* self);

/**
 * Sets the music location (mTransportTrack) and, when it is the race's current location, restarts its music.
 *
 * The restart is cSSXAudio_RestartLocationMusic.
 *
 * @param id  the location id
 */
void cSSXAudio::SetTransportTrack(int id)
{
    mTransportTrack = id;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (iface != 0) {
        if (*cBENewRaceInterface_getLocation(iface) == mTransportTrack) {
            RestartLocationMusic();
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_EnterMapScreen);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" void cSSXAudio_PauseSFX(void* self);
extern "C" void cSSXAudio_MenuSound(void* self, int a1);
extern "C" void cMusicSys_FadeOut(void* self, float v);
extern "C" void cSSXAudio_StartLoadingLoop(cSSXAudio* self);
extern "C" void cMusicSys_SetVolume(void* self, int a1);

struct sBmVtF5B8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Switches the audio to the map screen: pauses the sound effects and starts the loading-screen loop at state 0.
 *
 * Clears unk_6088 and returns when the audio is paused (mAudioPaused) or not in game. Without mScriptedDJLine it pauses
 * the sound effects (cSSXAudio_PauseSFX) and sets unk_6088 so cSSXAudio_LeaveMapScreen resumes them. Plays menu sound 9; outside
 * kRADIOMODE_MOUNTAIN it fades the song out over 1 s, starts the loading-screen sound (cSSXAudio_StartLoadingLoop), sets music state
 * 0 and full volume. Called when the map screen opens.
 */
void cSSXAudio::EnterMapScreen()
{
    mPausedSFXForMapScreen = 0;
    if (mAudioPaused != 0) return;
    if (cAudioInterface_isInGame() == 0) return;
    if (mScriptedDJLine == 0) {
        cSSXAudio_PauseSFX(this);
        mPausedSFXForMapScreen = 1;
    }
    cSSXAudio_MenuSound(this, 9);
    if (mRadioModeID != kRADIOMODE_MOUNTAIN) {
        char* mon = (char*)&MusicSys;
        cMusicSys_FadeOut(mon, 1.0f);
        StartLoadingLoop();
        sBmVtF5B8* vt = (sBmVtF5B8*)MusicSys.mVtbl;
        vt[4].fn(vthis(mon, vt[4]), 0);
        cMusicSys_SetVolume(mon, 0x7F);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_LeaveMapScreen);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResumeSFX(void* self);
extern "C" int cSSXAudio_IsAtHub(void* self);
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_StopLoadingLoop(cSSXAudio* self, float v);

/**
 * Leaves the map screen: resumes the sound effects paused by cSSXAudio_EnterMapScreen and stops the loading-screen loop.
 *
 * When unk_6088 is set it calls cSSXAudio_ResumeSFX and clears it. When the loop is playing (mLoadingLoopHandle >= 0)
 * it stops it with a 1 s fade (cSSXAudio_StopLoadingLoop) and, with a1 set in locations 17-21 (cSSXAudio_IsAtHub), restarts the
 * location music (cSSXAudio_StartLocationMusic(self, 0, 0, -1, 0)).
 *
 * @param restartMusic  nonzero to restart the location music
 */
void cSSXAudio::LeaveMapScreen(int restartMusic)
{
    if (mPausedSFXForMapScreen != 0) {
        cSSXAudio_ResumeSFX(this);
        mPausedSFXForMapScreen = 0;
    }
    if (mLoadingLoopHandle >= 0) {
        StopLoadingLoop(1.0f);
        if (restartMusic != 0 && cSSXAudio_IsAtHub(this) != 0) {
            StartLocationMusic(0, 0, -1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_LoadLoadingScreenBank);
#ifdef SKIP_ASM
extern "C" void cBankManager_BANK_SyncLoad(cBankManager* self, int i, void* id, int a3);
extern void* cSSXAudio_loadingScreenBankName;

/**
 * Loads the loading-screen sound bank ("data/audio/audio.big|data\\audio\\LoadingScreen.bnk") into bank 13.
 *
 * Synchronous (cBankManager_BANK_SyncLoad on the cBankManager). Called by the audio system's initialisation.
 */
void cSSXAudio::LoadLoadingScreenBank()
{
    MusicSys.mpVStreamSys->mpVBankManager->BANK_SyncLoad(13, cSSXAudio_loadingScreenBankName, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cSSXAudio_UnloadLoadingScreenBank);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_StopLoadingLoop(cSSXAudio* self, float v);
extern "C" void cBankManager_BANK_UnLoad(cBankManager* self, int i);

/**
 * Stops the loading-screen sound at once (cSSXAudio_StopLoadingLoop(0)) and unloads bank 13 (cBankManager_BANK_UnLoad).
 *
 * Called by the audio system's shutdown.
 */
void cSSXAudio::UnloadLoadingScreenBank()
{
    StopLoadingLoop(0.0f);
    MusicSys.mpVStreamSys->mpVBankManager->BANK_UnLoad(0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StartLoadingLoop);
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
 * control 1 (cSSXAudio_GetCurrAppliedVolumePtr(1, 0)) and plays it with cBankSys_SFX_End. Called by the load screen and cSSXAudio_EnterMapScreen;
 * cSSXAudio_StopLoadingLoop stops it.
 *
 */
void cSSXAudio::StartLoadingLoop()
{
    bmPush_F768(this, 13);
    cBankManager* bankManager = MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* st2 = &bankManager->BankMonitor;
    st2->mBaseVol[bankManager->BankMonitor.mInst] = 127;
    st2->mCurrent[bankManager->BankMonitor.mInst].vol = BM_MAX_F768(BM_MIN_F768(st2->mBaseVol[st2->mInst], 127), 0);
    cStreamSys* streamSys = MusicSys.mpVStreamSys;
    streamSys->mpVBankManager->BankMonitor.mVolControl[streamSys->mpVBankManager->BankMonitor.mInst] =
        (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0);
    mLoadingLoopHandle = cBankSys_SFX_End(this);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StopLoadingLoop);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops the loading-screen sound started by cSSXAudio_StartLoadingLoop (cBankMonitor_BANKMONITOR_StopHandle, type 1).
 *
 * Does nothing when mLoadingLoopHandle is negative; clears it to -1 afterwards.
 *
 * @param fadeTime  the fade time (0 stops at once)
 */
void cSSXAudio::StopLoadingLoop(float fadeTime)
{
    int idx = mLoadingLoopHandle;
    if (idx >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, idx, 1, fadeTime);
        mLoadingLoopHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_RestartMusicAfterMap);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_StartLocationMusic(cSSXAudio* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_PickRandomSong(cSSXAudio* self);
extern "C" bool cSSXAudio_IsMusicRadioMode(cSSXAudio* self);
extern "C" bool cSSXAudio_SetLastMusicState(cSSXAudio* self, int a1);
extern int cMonitor_MONITOR_Create_FAE0(void*, int, long, int, int, int, int, int) __asm__("cMonitor_MONITOR_Create");
extern "C" void cMonitor_MONITOR_DestroyAll1(void* mon, long key);
extern "C" void* cMusicSys_StopSong(void*);
int cMusicSys_GetCurrentSongSedValue(void* mon);
extern "C" int cMusicSys_GetSongTag(void* monitor);

struct sBmKeyFAE0 {
    long v;
} __attribute__((packed));
extern const sBmKeyFAE0 D_004A3648_FAE0[] __asm__("D_004A3648");
extern const sBmKeyFAE0 D_004A3650_FAE0[] __asm__("D_004A3650");
extern const sBmKeyFAE0 D_004A3658_FAE0[] __asm__("D_004A3658");

/**
 * Restarts the in-game music after the map screen: cancels pending events, stops the song and arms a new one.
 *
 * Only in a music mode accepted by cSSXAudio_IsMusicRadioMode. Cancels D_004A3648..D_004A3658 (cMonitor_MONITOR_DestroyAll1). When
 * the current song's tag is a peak theme (kSONGTAG_Peak1-Peak3) or kSONGTAG_PkTrans, or no song is playing, it records
 * event 18 (cSSXAudio_SetLastMusicState), sets mStopSpeechOnResume, stops the song, picks the next one (cSSXAudio_PickRandomSong), starts the
 * location music (cSSXAudio_StartLocationMusic) and arms the D_004A3650 event (cSSXAudio_HandleMusicChangeEvent) after 3000 ms with kind 4 and the song's
 * mSedValue. Called by the map state.
 */
// PORT: 64-bit long sound keys.
void cSSXAudio::RestartMusicAfterMap()
{
    if (IsMusicRadioMode() == 0) {
        return;
    }
    cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3648_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3650_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A3658_FAE0[0].v);
    int st = cMusicSys_GetSongTag(&MusicSys);
    if ((unsigned int)(st - kSONGTAG_Peak1) < 3 || st == kSONGTAG_PkTrans || MusicSys.mSongPlaying == 0) {
        SetLastMusicState(18);
        mStopSpeechOnResume = 1;
        cMusicSys_StopSong(&MusicSys);
        PickRandomSong();
        StartLocationMusic(0, 0, kPEAKBIT_FromLocation, 0);
        cBankMonitor* bankMonitor = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        const sBmKeyFAE0* key = D_004A3650_FAE0;
        int sedValue = cMusicSys_GetCurrentSongSedValue(&MusicSys);
        cMonitor_MONITOR_Create_FAE0(bankMonitor, 3000, key->v, 0, 4, sedValue, 0, 0);
    }
}
#endif

extern "C" void* cMusicSys_StopSong(void*);

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_StopSong__FPv);
#ifdef SKIP_ASM
/**
 * Stops the current song (cMusicSys_StopSong on the music system).
 *
 * Called when the network-configuration screen closes.
 *
 * @return cMusicSys_StopSong's return value
 */
void* cSSXAudio::StopSong()
{
    return cMusicSys_StopSong(&MusicSys);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cSSXAudio_PlayCharSelSong);
#ifdef SKIP_ASM
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void cMusicSys_SetSongTag(void* self, int a1);
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
void cSSXAudio::PlayCharSelSong()
{
    char* mon = (char*)&MusicSys;
    sBmVtFC58* vt = (sBmVtFC58*)MusicSys.mVtbl;
    ((tBmLoadFC58)vt[2].fn)(vthis(mon, vt[2]), str_charsel, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(this, 1, 0), 1);
    cMusicSys_SetSongTag(mon, kSONGTAG_CharSel);
    sBmVtFC58* vt2 = (sBmVtFC58*)MusicSys.mVtbl;
    ((tBmSetFC58)vt2[4].fn)(vthis(mon, vt2[4]), mFrontEndMusicMode);
}
#endif

