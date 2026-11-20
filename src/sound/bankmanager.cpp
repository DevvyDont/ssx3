#include "common.h"
#include "sound/bankmanager.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_AllocMem);
#ifdef SKIP_ASM
extern char* gpBankManager;
extern char str_StaticSoundMem[];
extern "C" void* MEM_alloc(void* a0, int a1, int a2);

class cSndStream_B588 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual int alloc(int size);
};

/**
 * Allocates a sound bank's memory: optional sound (SPU) memory and a main-memory block ("StaticSoundMem").
 *
 * Sound memory comes from the bank manager's stream allocator; if that reports -3 the sound part is added to
 * the
 * main block instead. The state word (+0x8) becomes 1, or 2 when sound memory was obtained. Nothing happens
 * when
 * both sizes are 0. The unit declares it void*, but it returns no value.
 *
 * @param size  main-memory size in bytes (On Tour mainMemSize)
 * @param extra  sound-memory size in bytes (On Tour soundMemSize)
 * @param tag  memory flags for the main block (On Tour memflags)
 * @return no meaningful value
 */
// PORT: the unit declares this void* (a caller forwards its $v0), but no value is returned.
extern "C" void* cBankInstance_AllocMem(cBankInstance* self, int size, int extra, int tag)
{
    if (size != 0 || extra != 0) {
        self->mAllocLocation = 1;
        if (extra != 0) {
            int id = (**(cSndStream_B588***)(gpBankManager + 0x1D8))->alloc(extra);
            if (id >= 0) {
                self->mSoundMemHandle = id;
                self->mAllocLocation = 2;
            } else if (id == -3) {
                size += extra;
            }
        }
        self->mMainMemDataSize = size;
        self->mpMainMemData = (char*)MEM_alloc(str_StaticSoundMem, size, tag);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_DeallocMem);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(void*);
extern "C" void* MEM_free(void* self);
extern char* gpBankManager;

class cSndStream_B650 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void stop(int id);
};

/**
 * Unloads the bank and frees the memory it was given.
 *
 * Calls cBankInstance_Unload; then, when the allocation location at +0x8 is non-zero: for location 2 (sound memory) it
 * frees the sound-memory handle at +0xC through cPS2SoundManager_DeallocSoundMem (the virtual of the shared cSoundSys's
 * pManager, reached from gpBankManager's cBankMonitor) and sets the handle to -1; it then MEM_frees the main-memory
 * data at +0x10 and zeroes the data pointer, the size at +0x14 and the location. Called by cBankInstance__cBankInstance
 * and, by bank index, by cBankManager_BANK_DeallocMem.
 */
extern "C" void cBankInstance_DeallocMem(cBankInstance* self)
{
    cBankInstance_Unload(self);
    int state = self->mAllocLocation;
    if (state != 0) {
        if (state == 2) {
            (**(cSndStream_B650***)(gpBankManager + 0x1D8))->stop(self->mSoundMemHandle);
            self->mSoundMemHandle = -1;
        }
        MEM_free(self->mpMainMemData);
        self->mpMainMemData = 0;
        self->mMainMemDataSize = 0;
        self->mAllocLocation = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_ShareBank);
#ifdef SKIP_ASM
/**
 * Makes bank @p dst share the memory of bank @p src.
 *
 * Links @p dst into @p src's ring of sharing banks (the next pointer at +0x18 and prev at +0x1C): after @p src when
 * @p src is already in a ring, else @p src and @p dst become a two-bank ring. When @p src has an allocation (location
 * at +0x8 non-zero), copies the location, the main-memory data and size (+0x10, +0x14) and the sound-memory handle
 * (+0xC) into @p dst. Called by cBankManager_BANK_ShareBank with two banks of the manager's array.
 *
 * @param dst  the bank that shares
 * @param src  the bank whose memory is shared
 */
extern "C" void cBankInstance_ShareBank(void *dst, void *src) {
    int flag;

    if (*(void **)((char*)src + 0x18) != 0) {
        *(void **)((char*)dst + 0x18) = src;
        *(void **)((char*)dst + 0x1C) = *(void **)((char*)src + 0x1C);
        *(void **)((char*)src + 0x1C) = dst;
        *(void **)((char*)*(void **)((char*)dst + 0x1C) + 0x18) = dst;
    } else {
        *(void **)((char*)src + 0x18) = dst;
        *(void **)((char*)src + 0x1C) = dst;
        *(void **)((char*)dst + 0x18) = src;
        *(void **)((char*)dst + 0x1C) = src;
    }
    flag = *(int *)((char*)src + 8);
    if (flag != 0) {
        *(int *)((char*)dst + 8) = flag;
        *(int *)((char*)dst + 0x10) = *(int *)((char*)src + 0x10);
        *(int *)((char*)dst + 0x14) = *(int *)((char*)src + 0x14);
        *(int *)((char*)dst + 0xC) = *(int *)((char*)src + 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_UnshareBank);
#ifdef SKIP_ASM
struct sBankNode {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    sBankNode* next;
    sBankNode* prev;
};

/**
 * Takes the bank out of the ring of banks sharing one block of memory.
 *
 * Does nothing when next is null. Otherwise it sets the sound-memory handle unkC to -1, zeroes the main-memory data
 * unk10, its size unk14 and the location unk8 (nothing is freed), and unlinks the bank: when next == prev (a two-bank
 * ring) the other bank's next and prev are cleared, otherwise the neighbours are joined. Its own next and prev are then
 * cleared. Called by cBankInstance__cBankInstance and, by bank index, by cBankManager_BANK_UnshareBank.
 */
extern "C" void cBankInstance_UnshareBank(sBankNode* self)
{
    if (self->next != 0) {
        self->unkC = -1;
        self->unk10 = 0;
        self->unk14 = 0;
        self->unk8 = 0;
        if (self->next == self->prev) {
            self->next->next = 0;
            self->next->prev = 0;
        } else {
            self->next->prev = self->prev;
            self->prev->next = self->next;
        }
        self->prev = 0;
        self->next = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_Attach);
#ifdef SKIP_ASM
extern "C" void cBankInstance_UnAttach(cBankInstance*);
extern "C" int SNDbankadd(void* sema, int a1);

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
extern "C" void cBankInstance_Attach(cBankInstance* self, int mainMemData, int mainMemSize)
{
    cBankInstance_UnAttach(self);
    self->mpMainMemData = (char*)mainMemData;
    self->mMainMemDataSize = mainMemSize;
    SNDbankadd((char*)&self->mBankHandle, mainMemData);
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_UnAttach);
#ifdef SKIP_ASM
extern "C" int SNDbankremove(int id);

/**
 * Detaches the bank attached by cBankInstance_Attach.
 *
 * Sets the status word at +0x0 to 0; when the bank handle at +0x4 is not -1 it removes the bank with SNDbankremove and
 * sets the handle to -1; then it zeroes the main-memory data (+0x10) and size (+0x14) without freeing them. Called by
 * cBankInstance_Attach and, by bank index, by cBankManager_BANK_UnAttach.
 */
extern "C" void cBankInstance_UnAttach(cBankInstance* self)
{
    *(int*)self = 0;
    if (self->mBankHandle != -1) {
        SNDbankremove(self->mBankHandle);
        self->mBankHandle = -1;
    }
    self->mpMainMemData = 0;
    self->mMainMemDataSize = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_IsBankLoaded);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);

/**
 * Returns whether the bank holds the loaded file @p name.
 *
 * True only when the status word at +0x0 is 1 (loaded) and func_004165A8 (strcmp) finds @p name equal to the bank's
 * filename at +0x20. cBankManager_BANK_SyncLoad and cBankManager_BANK_AsyncLoad use it to skip reloading a bank that is already loaded.
 *
 * @param name  file name to compare
 * @return 1 when the bank is loaded from @p name, else 0
 */
extern "C" int cBankInstance_IsBankLoaded(cBankInstance* self, void* name)
{
    int status = *(int*)self;
    if (status != 1) {
        return 0;
    }
    if (func_004165A8(name, (char*)&self->mBankFilename) != 0) {
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
extern char* gpBankManager;

class cSndStream_B878 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5(int id);
    virtual void v6(int id);
};

/**
 * Prepares a bank slot for an asynchronous load: unloads whatever it holds and marks it loading.
 *
 * Calls cBankInstance_Unload and cBankInstance_SharedUnload (On Tour Unload and SharedUnload), sets the status (+0x0, mBankStatus) to 2
 * (loading) and, for a bank held in sound memory (mAllocLocation +0x8 == 2), passes its sound memory handle (+0xC) to
 * virtual slot 6 of the bank manager's sound system (the cSoundSys virtual base reached through gpBankManager +0x1D8).
 * On Tour asserts the bank was not already loading and that the handle is valid; retail checks neither.
 */
extern "C" void cBankInstance_OnBeginAsyncLoad(cBankInstance* self)
{
    cBankInstance_Unload(self);
    cBankInstance_SharedUnload(self);
    *(int*)self = 2;
    if (self->mAllocLocation == 2) {
        (**(cSndStream_B878***)(gpBankManager + 0x1D8))->v6(self->mSoundMemHandle);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnAsyncMainMemAlloc);
#ifdef SKIP_ASM
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern char str_DynamicSoundMem[];

// PORT: real body returns the buffer; a later caller in this unit declares it void(void*, int, int)
void* cBankInstance_OnAsyncMainMemAlloc_impl(void* self, int size, int flags) __asm__("cBankInstance_OnAsyncMainMemAlloc");

/**
 * Supplies main memory for an asynchronous bank load, allocating it ("DynamicSoundMem") unless the bank has
 * some.
 *
 * @param size  size in bytes
 * @param flags  memory flags
 * @return the bank's main-memory block
 */
void* cBankInstance_OnAsyncMainMemAlloc_impl(void* self, int size, int flags)
{
    if (*(int*)((char*)self + 0x8) == 0) {
        *(void**)((char*)self + 0x10) = MEM_alloc(str_DynamicSoundMem, size, flags);
        *(int*)((char*)self + 0x14) = size;
    }
    return *(void**)((char*)self + 0x10);
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
int cBankInstance_OnAsyncMainMemSize(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnEndAsyncLoad);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern char* gpBankManager;

class cSndStream_B930 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5(int id);
    virtual void v6(int id);
    virtual void v7();
};

/**
 * Finishes a bank's asynchronous load: records the sound handle, the outcome and the file name.
 *
 * Calls virtual slot 7 of the bank manager's sound system (gpBankManager +0x1D8), stores the sound handle as the bank
 * handle (+0x4), sets the status (+0x0) to 1 (loaded) when the end state is 0 and to 0 otherwise, and copies the file
 * name into the bank's name buffer (+0x20, 64 bytes in On Tour). On Tour also checks the buffer against the reserved
 * one and the size against it and logs "WARNING: Asynchronous bank loading operation failed." on failure; retail drops
 * those, so the last two arguments are unused. The name is copied even when the load failed.
 *
 * @param id  the sound handle the loader returned
 * @param flag  the end state (On Tour eAsyncSysEndState); 0 means success
 * @param name  the loaded file's name (a char*, declared int to match the caller)
 * @param pBuffer  the load buffer (unused)
 * @param dataSize  the number of bytes loaded (unused)
 */
// PORT: name is passed as int (matches the unit's 6-int caller declaration); d, e unused.
extern "C" void cBankInstance_OnEndAsyncLoad(cBankInstance* self, int id, int flag, int name, int pBuffer, int dataSize)
{
    (**(cSndStream_B930***)(gpBankManager + 0x1D8))->v7();
    self->mBankHandle = id;
    if (flag == 0) {
        *(int*)self = 1;
    } else {
        *(int*)self = 0;
    }
    strcpy((char*)&self->mBankFilename, (const char*)name);
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
extern char* gpBankManager;
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

struct sBankManager {
    char pad_0x0[0x1D4];
    void** vtbl;                // 0x1D4
    char* monitor;              // 0x1D8
    char pad_0x1DC[0xAB0 - 0x1DC];
    void** vtbl2;               // 0xAB0
    char pad_0xAB4[0xAC8 - 0xAB4];
    int count;                  // 0xAC8
    sBmBank* banks;             // 0xACC
    char embedded[0x4];         // 0xAD0
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
extern "C" sBankManager* cBankManager_cBankManager(sBankManager* self, int embedded, int count, int heap)
{
    if (embedded) {
        self->monitor = self->embedded;
        cSoundSys_cSoundSys(self->embedded, heap);
    }
    cAsyncSys_cAsyncSys(self);
    cBankMonitor_cBankMonitor(&self->monitor, 0, heap);
    *(const sBmVtbl**)(self->monitor + 4) = &D_00483AB8;
    if (!embedded) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtbl vt = D_00483AB8;
        *(sBmVtbl**)(self->monitor + 4) = &vt;
        char* base = self->monitor - 0xAD0;
        int thisDelta = (char*)self - base;
        vt.e[1].delta = D_00483AB8.e[1].delta + thisDelta;
    }
    self->count = count;
    self->vtbl2 = cBankMonitor_vtbl;
    self->vtbl = cBankManager_vtbl;
    sBmBank** slot = &self->banks;
    *slot = new (str_BankInstances, 0, 0) sBmBank[count];
    gpBankManager = (char*)self;
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
extern char* gpBankManager;
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

struct sBmBankBB10 {
    char pad_0x0[0x60];
};

struct sBankManagerBB10 {
    char pad_0x0[0x1D4];
    void** vtbl;                // 0x1D4
    char* monitor;              // 0x1D8
    char pad_0x1DC[0xAB0 - 0x1DC];
    void** vtbl2;               // 0xAB0
    char pad_0xAB4[0xAC8 - 0xAB4];
    int count;                  // 0xAC8
    sBmBankBB10* banks;         // 0xACC
};

/**
 * Destroys the bank manager: frees its bank instances, tears down its monitor and async-loader parts and clears
 * gpBankManager.
 *
 * Restores the cBankManager and cBankMonitor vtables and the virtual base's vtable (D_00483AB8; with flags 0 the g++
 * 2.95 virtual-base this-adjust fix-up runs on a stack copy), clears gpBankManager, destroys the cBankInstance array at
 * +0xACC last-first with cBankInstance__cBankInstance (its count is the word 0x10 bytes before it) and frees it, then runs
 * cBankMonitor__cBankMonitor on +0x1D8 and cAsyncSys__cAsyncSys (On Tour ~cAsyncSys) on the object. cSoundSys__cSoundSys
 * destroys the virtual base only when flags bit 1 is set.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object's memory, bit 1 destroys the cSoundSys virtual base
 */
extern "C" void cBankManager__cBankManager(cBankManager* self, int flags)
{
    self->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mVtbl = cBankManager_vtbl;
    *(const sBmVtblBB10**)(*(char**)&self->BankMonitor + 4) = &D_00483AB8_BB10;
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblBB10 vt = D_00483AB8_BB10;
        *(sBmVtblBB10**)(*(char**)&self->BankMonitor + 4) = &vt;
        char* base = *(char**)&self->BankMonitor - 0xAD0;
        int soundSysAdjust = (char*)self - base;
        vt.e[1].delta = D_00483AB8_BB10.e[1].delta + soundSysAdjust;
    }
    gpBankManager = 0;
    if ((sBmBankBB10*)self->mBank != 0) {
        sBmBankBB10* bankInstance = (sBmBankBB10*)self->mBank + ((int*)(sBmBankBB10*)self->mBank)[-4];
        while ((sBmBankBB10*)self->mBank != bankInstance) {
            bankInstance--;
            cBankInstance__cBankInstance(bankInstance, 0);
        }
        cMemMan_free((char*)(sBmBankBB10*)self->mBank - 0x10);
    }
    cBankMonitor__cBankMonitor((char**)&self->BankMonitor, 0);
    cAsyncSys__cAsyncSys(((sBankManagerBB10*)self), 0);
    if (flags & 2) {
        cSoundSys__cSoundSys(*(char**)&self->BankMonitor, 0);
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
extern "C" void cBankManager_BANK_SyncLoad(cBankManager* self, int i, void* id, int mainMemFlag)
{
    int off = i * 0x60;
    if (cBankInstance_IsBankLoaded((cBankInstance*)((char*)self->mBank + off), id) == 0) {
        cAsyncSys_ASYNCSYS_Cancel(self, i);
        cBankInstance_SyncLoad((char*)self->mBank + off, id, mainMemFlag);
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
extern "C" void cBankManager_BANK_UnLoad(cBankManager* self, int i)
{
    cBankInstance_Unload((char*)self->mBank + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_AsyncLoad);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" void cAsyncSys_ASYNCSYS_QueueLoad(void* self, int i, void* id, int a3, int a4);

struct sBankInst60 { char pad0[8]; int f8; char pad1[0x60 - 0xC]; };

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
extern "C" void cBankManager_BANK_AsyncLoad(cBankManager* self, int i, void* id, int mainMemFlag)
{
    if (cBankInstance_IsBankLoaded((cBankInstance*)((char*)self->mBank + i * 0x60), id) == 0) {
        cAsyncSys_ASYNCSYS_Cancel(self, i);
        cAsyncSys_ASYNCSYS_QueueLoad(self, i, id, ((sBankInst60*)self->mBank)[i].f8, mainMemFlag);
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
extern "C" void* cBankManager_BANK_AllocMem(cBankManager* self, int i, int mainMemSize, int soundMemSize, int memFlags)
{
    return cBankInstance_AllocMem((cBankInstance*)((char*)self->mBank + i * 0x60), mainMemSize, soundMemSize, memFlags);
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
extern "C" void cBankManager_BANK_DeallocMem(cBankManager* self, int i)
{
    cBankInstance_DeallocMem((cBankInstance*)((char*)self->mBank + i * 0x60));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_ShareBank);
#ifdef SKIP_ASM
extern "C" void cBankInstance_ShareBank(void* dst, void* src);

/**
 * Makes bank slot @p i share the memory of bank slot @p j: calls cBankInstance_ShareBank with those two 0x60-byte bank instances
 * of the array at +0xACC.
 *
 * @param i  bank slot that shares
 * @param j  bank slot whose memory is shared
 */
extern "C" void cBankManager_BANK_ShareBank(cBankManager* self, int i, int j)
{
    cBankInstance_ShareBank((char*)self->mBank + i * 0x60, (char*)self->mBank + j * 0x60);
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
extern "C" void cBankManager_BANK_UnshareBank(cBankManager* self, int i)
{
    cBankInstance_UnshareBank((sBankNode*)((char*)self->mBank + i * 0x60));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_BANK_Attach);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Attach(cBankInstance* inst, int a, int b);

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
extern "C" void cBankManager_BANK_Attach(cBankManager* self, int i, int mainMemData, int mainMemSize)
{
    cBankInstance_Attach((cBankInstance*)((char*)self->mBank + i * 0x60), mainMemData, mainMemSize);
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
extern "C" void cBankManager_BANK_UnAttach(cBankManager* self, int i)
{
    cBankInstance_UnAttach((cBankInstance*)((char*)self->mBank + i * 0x60));
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
extern "C" void cBankManager_ASYNCSYS_OnBeginAsyncLoad(cBankManager* self, int i)
{
    cBankInstance_OnBeginAsyncLoad((cBankInstance*)((char*)self->mBank + i * 0x60));
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
extern "C" void cBankManager_ASYNCSYS_OnAsyncMainMemAlloc(cBankManager* self, int i, int size, int memflags)
{
    cBankInstance_OnAsyncMainMemAlloc((char*)self->mBank + i * 0x60, size, memflags);
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
extern "C" void cBankManager_ASYNCSYS_OnAsyncMainMemSize(cBankManager* self, int i)
{
    cBankInstance_OnAsyncMainMemSize((char*)self->mBank + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", cBankManager_ASYNCSYS_OnEndAsyncLoad);
#ifdef SKIP_ASM
extern "C" void cBankInstance_OnEndAsyncLoad(cBankInstance* inst, int a, int b, int c, int d, int e);

/**
 * Async-loader callback for the end of a bank load: forwards the outcome to that bank's instance.
 *
 * Calls cBankInstance_OnEndAsyncLoad (On Tour cBankInstance::OnEndAsyncLoad) on mBank[handle] (+0xACC, 0x60-byte records) with the
 * other five arguments unchanged. Slot 5 of the cAsyncSys interface vtable D_00483728.
 *
 * @param i  the user handle given to the loader: the bank number
 * @param soundHandle  the sound handle the load produced
 * @param endState  the end state (On Tour eAsyncSysEndState; 0 = success)
 * @param pFilename  the file name (a char*, passed as int)
 * @param pBuffer  the load buffer
 * @param dataSize  the number of bytes loaded
 */
extern "C" void cBankManager_ASYNCSYS_OnEndAsyncLoad(cBankManager* self, int i, int soundHandle, int endState, int pFilename, int pBuffer, int dataSize)
{
    cBankInstance_OnEndAsyncLoad((cBankInstance*)((char*)self->mBank + i * 0x60), soundHandle, endState, pFilename, pBuffer, dataSize);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028BF78);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cAudioInterface_isInGame(void);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(void* self);
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002B4AF0(void* monitor);
extern "C" void cSSXAudio_SetDJSpeech(void* self, int on);
// PORT: func_002B4070 takes the 64-bit bank mask as a second argument (64-bit `long`).
extern "C" void func_002B4070_BF78(void* monitor, long mask) __asm__("func_002B4070");

struct sCfg_BF78 {
    int flags;
    int v[(0x288 - 4) / 4];
};
extern sCfg_BF78 D_00535610_BF78 __asm__("cBEOptionInterface_mOption");

/**
 * Switches the radio mode (unk_608C) to @p state and restarts the music when the current song no longer fits it.
 *
 * Does nothing when unk_608C already equals @p state; otherwise stores it. cSSXAudio_InGameInit passes
 * cBERewardInterface_getRadioMode's result, whose On Tour type eRadioModeID has 0 radio, 1 request line, 2 mountain, 3
 * playlist; the audio options menu passes its selection.
 *
 * Mode 0 activates the songs of MusicSys.mDefaultActiveMap (func_002B4070), re-applies the arcade SFX setting from bit
 * 17 of cBEOptionInterface_mOption's flags (cSSXAudio_SetDJSpeech), and, in game with MusicSys.mSongPlaying set and a
 * current song id (func_002B49E0) of 101-103 (a peak ambience), picks a new song (func_0028D488) and restarts with
 * func_0028CF98(self, 0, 0, -1, 0). Modes 1 and 3 do the same with mRadioPlayList, returning early when that is 0, when
 * the song id is 1-3, or when the current song is still active (func_002B4AF0) and its id is not 301. Mode 2 turns the
 * arcade SFX off and, in game unless the song id is already 101-103, calls func_0028CF98(self, 0, 0, -1, 0) (which
 * starts the peak ambience in this mode) and clears mInBigChallenge and mCurBigChallengeType.
 *
 * The option block is copied whole (0x288 bytes) to the stack just to read its flags.
 *
 * @param state  the new radio mode
 */
extern "C" void func_0028BF78(char* self, int state)
{
    if (*(int*)(self + 0x608C) == state)
        return;
    *(int*)(self + 0x608C) = state;
    switch (state) {
    case 0: {
        func_002B4070_BF78(self + 0x118, *(long*)(self + 0x518));
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = D_00535610_BF78;
        cSSXAudio_SetDJSpeech(self, (cfg.flags >> 17) & 1);
        if (cAudioInterface_isInGame() == 0)
            return;
        if (*(int*)(self + 0x530) == 0)
            return;
        if ((unsigned int)(func_002B49E0(self + 0x118) - 101) >= 3)
            return;
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
        break;
    }
    case 1:
    case 3: {
        func_002B4070_BF78(self + 0x118, *(long*)(self + 0x6098));
        cBE_getInterface_Fv(cBE_getBE(), 4);
        sCfg_BF78 cfg = D_00535610_BF78;
        cSSXAudio_SetDJSpeech(self, (cfg.flags >> 17) & 1);
        if (*(long*)(self + 0x6098) == 0)
            return;
        if (cAudioInterface_isInGame() == 0)
            return;
        if (*(int*)(self + 0x530) == 0)
            return;
        int st = func_002B49E0(self + 0x118);
        if ((unsigned int)(st - 1) < 3)
            return;
        if (func_002B4AF0(self + 0x118) != 0 && st != 301)
            return;
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
        break;
    }
    case 2:
        cSSXAudio_SetDJSpeech(self, 0);
        if (cAudioInterface_isInGame() == 0)
            return;
        if ((unsigned int)(func_002B49E0(self + 0x118) - 101) < 3)
            return;
        func_0028CF98(self, 0, 0, -1, 0);
        *(int*)(self + 0x5FD4) = 0;
        *(int*)(self + 0x5FD0) = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C2D0);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(void* self);
extern "C" void func_002B4070(void* monitor);
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002B4AF0(void* monitor);

static inline void bmRestartC2D0(char* self)
{
    if (cAudioInterface_isInGame() == 0) {
        return;
    }
    if (*(int*)(self + 0x530) == 0) {
        return;
    }
    char* mon = self + 0x118;
    int st = func_002B49E0(mon);
    if ((unsigned int)(st - 1) < 3) {
        return;
    }
    if (func_002B4AF0(mon) != 0 && st != 0x12D) {
        return;
    }
    func_0028D488(self);
    func_0028CF98(self, 0, 0, -1, 0);
}

/**
 * Sets the radio playlist mask mRadioPlayList to @p key and, in radio modes 1 and 3, restarts the music from it.
 *
 * When @p key differs from mRadioPlayList it is stored, and in modes 1 and 3 (unk_608C) func_002B4070 rebuilds the
 * active songs from it (the mask reaches it in the second argument register; the call is declared with one argument).
 * In modes 1 and 3, changed or not, it then does what func_0028BF78 does for those modes: in game with
 * MusicSys.mSongPlaying set, unless the current song id (func_002B49E0) is 1-3 or the current song is still active
 * (func_002B4AF0) with an id other than 301 (0x12D), it picks a new song (func_0028D488) and calls func_0028CF98(self,
 * 0, 0, -1, 0). Called by cSSXAudio_InGameInit and cFEStateRequestLine_onWidgetEvent.
 *
 * @param key  the new playlist mask, one bit per song (64-bit)
 */
// PORT: 64-bit `long` key.
extern "C" void func_0028C2D0(char* self, long key)
{
    if (*(long*)(self + 0x6098) != key) {
        *(long*)(self + 0x6098) = key;
        int state = *(int*)(self + 0x608C);
        if (state == 1 || state == 3) {
            func_002B4070(self + 0x118);
            bmRestartC2D0(self);
        }
    } else {
        int state = *(int*)(self + 0x608C);
        if (state == 1 || state == 3) {
            bmRestartC2D0(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C430);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void func_002B2850(void* a, void* mon);

struct sEntry_C430 {
    signed char kind;
    char pad[0x17];
};
struct sQuad_C430 {
    int a;
    int b;
    int c;
    int d;
};
struct sStack_C430 {
    int field_0x0;                  // 0x1D8
    sEntry_C430 cur;                // 0x1DC
    volatile int depth;             // 0x1F4
    sEntry_C430* entries;           // 0x1F8
    int flags[4];                   // 0x1FC
    int* a34;
    int* a38;
    int* a3C;
    char a40[4][8];
    int* a60;
    int* a64;
    int* a68;
    int* a6C;
    sQuad_C430* a70;
    int* a74;
    int* a78;
    int* a7C;
    int* a80;
    int* a84;
};
struct sMgr_C430 {
    char pad_0x0[0x1D8];
    sStack_C430 st;                 // 0x1D8
};

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

static inline void bmPush_C430(sMgr_C430** self, int kind, int data)
{
    sMgr_C430* m = *self;
    sStack_C430* st = &m->st;
    m->st.depth++;
    st->entries[m->st.depth] = m->st.cur;
    m->st.flags[m->st.depth] = 0;
    st->a34[m->st.depth] = 0;
    st->a38[m->st.depth] = st->entries[m->st.depth].kind;
    st->a3C[m->st.depth] = 0;
    func_00416210((char*)st + m->st.depth * 8 + 0x40, 0, 8);
    st->a60[m->st.depth] = 0;
    st->a64[m->st.depth] = 0;
    st->a68[m->st.depth] = 0;
    st->a6C[m->st.depth] = 0;
    st->a70[m->st.depth].b = 0;
    st->a70[m->st.depth].a = 100;
    st->a70[m->st.depth].c = 90;
    st->a70[m->st.depth].d = 50;
    st->a74[m->st.depth] = 0;
    st->a80[m->st.depth] = 0;
    st->a84[m->st.depth] = 0;
    st->a78[m->st.depth] = 127;
    st->a7C[m->st.depth] = 1;
    (*(int**)((char*)self + 4))[(*self)->st.depth] = kind;
    (*(int**)((char*)self + 8))[(*self)->st.depth] = data;
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
extern "C" int func_0028C430(void* audio, sCue_C430* cue)
{
    sMgr_C430** self = (sMgr_C430**)audio;
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
        bmPush_C430(self, kind, (int)(data + idx));
        sMgr_C430** pp = *(sMgr_C430***)((char*)self + 0x118);
        (*pp)->st.a3C[(*pp)->st.depth] = (int)((char*)self + 0x60A0);
        sMgr_C430* bankManager = **(sMgr_C430***)((char*)self + 0x118);
        sStack_C430* st2 = &bankManager->st;
        st2->a38[bankManager->st.depth] = (int)BM_MAX_C430(BM_MIN_C430(st2->a38[st2->depth] * (vol * 0.009999999776482582f), 127.0f), 0.0f);
        st2->entries[st2->depth].kind = BM_MAX_C430(BM_MIN_C430(st2->a38[st2->depth], 127), 0);
        func_002B2850(*(void**)((char*)self + 0x520), (char*)self + 0x118);
        cBankSys_SFX_End(self);
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
void func_0028C8C0(void* self, int val)
{
    *(int*)((char*)self + 0x6250) = val;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C8C8);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
struct sRiderC8C8;
extern "C" sRiderC8C8* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
int cRider_getMotionState(void* rider);
int func_0028D898(void* self);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_0028D960(void* self);
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

struct sApC8C8 {
    char pad0[0x98];
    float f98;          // 0x98
    float f9C;          // 0x9C
    float fA0;          // 0xA0
    char padA4[0xAC - 0xA4];
    int state;          // 0xAC
};

struct sRiderC8C8 {
    char pad0[0x788];
    sApC8C8* ap;        // 0x788
};

struct sBmC8C8 {
    char pad0[0x118];
    char mon[0x5558 - 0x118];   // 0x118
    sBmVtC8C8* monVt;           // 0x5558
    char pad555C[0x5830 - 0x555C];
    float f5830;                // 0x5830
    char pad5834[0x60A0 - 0x5834];
    float f60A0;                // 0x60A0
    char pad60A4[0x6244 - 0x60A4];
    float f6244;                // 0x6244
    float f6248;                // 0x6248
    float f624C;                // 0x624C
    int f6250;                  // 0x6250
};

static inline float bmAirTime_C8C8(sBmC8C8* self, sRiderC8C8* rider)
{
    int s = rider->ap->state;
    int air = 0;
    if (s == 1 || s == 3)
        air = 1;
    if (air) {
        if (D_004A361C == 1)
            return self->f6244;
        float v = rider->ap->f98;
        self->f6248 = 0.0f;
        self->f6244 = v;
        return v;
    }
    if (D_004A361C == 1)
        return self->f6244;
    return 0.0f;
}

/**
 * Updates the wind sound and the music volume for the focus rider's air time; called every frame from cSSXAudio_Update.
 *
 * Counts unk_6250 down. For the focus rider (cSSXAudio_GetFocusRider(-1)) in motion state 1 with its mAirPredictor's
 * state (+0xAC, On Tour SimState) 1 or 3, the air time is the predictor's +0x98 (On Tour TotalAirTime), cached in
 * unk_6244 on the first frame (unk_6248 cleared); while D_004A361C is 1 the cached value is reused; otherwise it is 0.
 * unk_60A0 is set to the music volume (cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0)), and
 * D_004A3620 takes last frame's D_004A361C, which is cleared. Above 3.9995 s, when the song's mDuckToLoops
 * (func_002B4878) is 1, func_002A4168 and func_002A41D8 are false and no rider event runs (func_0028D898), the song is
 * driven too (local play). Outside replays, above 2 s: D_004A361C = 1; unk_6248 keeps the largest current air time
 * (+0xA0), and unk_624C, set on the first frame, is that plus max((+0x9C - +0xA0) / 2, 0.5). From their ratio and a
 * volume falling with the air time (20..127) it sets mWindVolume (then scaled by the SFX volume, control 5) and, with
 * play, the song's base volume (func_002B3C98) and overlay level (unk_60A0 via func_002B3D10). On the first such frame
 * it starts the wind (cSSXAudio_BeginWind) and, with play, func_002B4620 true and the radio on (func_0028D960), sends music
 * event 7 through MusicSys.mVtbl slot 3. Otherwise, after an airborne frame, it ends the wind (cSSXAudio_EndWind),
 * sends event 8 when func_002B46E8 reports the song's unk_4C set (and the radio is on) and clears it with
 * func_002B4708; then it restores base volume 127 and the overlay level.
 */
extern "C" void func_0028C8C8(sBmC8C8* self)
{
    if (self->f6250 > 0)
        self->f6250--;
    sRiderC8C8* rider = cSSXAudio_GetFocusRider_p(self, -1);
    float airTime;
    if (cRider_getMotionState(rider) == 1) {
        int simState = rider->ap->state;
        int air = 0;
        if (simState == 1 || simState == 3)
            air = 1;
        if (air) {
            if (D_004A361C == 1) {
                airTime = self->f6244;
            } else {
                airTime = rider->ap->f98;
                self->f6248 = 0.0f;
                self->f6244 = airTime;
            }
            goto have;
        }
        if (D_004A361C == 1) {
            airTime = self->f6244;
            goto have;
        }
    }
    airTime = 0.0f;
have:
    int play = 0;
    self->f60A0 = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
    D_004A3620 = D_004A361C;
    D_004A361C = 0;
    if (3.9994986057281494f < airTime) {
        int duckToLoops = func_002B4878(self->mon);
        if (duckToLoops == 1 && !func_002A4168(self) && !func_002A41D8(self)) {
            if (!func_0028D898(self))
                play = duckToLoops;
        }
    }
    if (cSSXAudio_ReplayMode(self) == 0) {
        if (2.0f < airTime) {
            D_004A361C = 1;
            float lo = rider->ap->fA0;
            if (lo < self->f6248)
                lo = self->f6248;
            self->f6248 = lo;
            float hi;
            if (D_004A3620 == 0) {
                hi = (rider->ap->f9C - rider->ap->fA0) * 0.5f;
                if (hi < 0.5f)
                    hi = 0.5f;
                hi = hi + lo;
                self->f624C = hi;
            } else {
                hi = self->f624C;
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
                    self->f60A0 = musicLevel;
                    func_002B3D10(self->mon, (int)(musicLevel * 100.0f));
                }
                rawVolume = vmax - (vmax - vol) * ratio;
                clampedVolume = (int)((rawVolume <? vmax) >? vol);
                self->f5830 = (float)(full - clampedVolume) * inv;
                if (play)
                    func_002B3C98(self->mon, clampedVolume);
            } else {
                self->f5830 = (vmax - vol) * inv;
                if (play) {
                    func_002B3C98(self->mon, (int)vol);
                    float k = *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
                    self->f60A0 = k;
                    func_002B3D10(self->mon, (int)(k * 100.0f));
                }
            }
            self->f5830 = self->f5830 * *(float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
            if (D_004A3620 == 0) {
                cSSXAudio_BeginWind(self);
                if (play) {
                    char* mon = self->mon;
                    if (func_002B4620(mon) && func_0028D960(self))
                        self->monVt[3].fn(mon + self->monVt[3].delta, 7);
                }
            }
            return;
        }
    }
    char* mon = self->mon;
    if (D_004A3620 == 1) {
        cSSXAudio_EndWind(self);
        if (func_002B46E8(mon)) {
            if (func_0028D960(self))
                self->monVt[3].fn(mon + self->monVt[3].delta, 8);
            func_002B4708(mon);
        }
    }
    func_002B3C98(mon, 0x7F);
    func_002B3D10(mon, (int)(self->f60A0 * 100.0f));
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
extern "C" void func_0028CD48(void* self)
{
    char* mon = (char*)self + 0x118;
    sBmVtCD48* vt = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmLoadCD48)vt[2].fn)(mon + vt[2].delta, str_pktrans, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 1);
    func_002B49C0(mon, 401);
    sBmVtCD48* vt2 = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmSetCD48)vt2[4].fn)(mon + vt2[4].delta, 1);
    func_002B3C28(mon, 0x7F);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028CDF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_002B4A08(void* self, int a1);
extern int D_00536730[];
extern int D_00536708[];
extern char str_chartune[];

struct sRace_28CDF8 {
    int mode;
    char pad4[0x14];
    int tracks[8];
};

/**
 * Starts the character tune for the podium (cGFPodiumState_gainFocus).
 *
 * Does nothing in a replay (cSSXAudio_ReplayMode), in radio mode 2 (unk_608C), unless SSXApp's GameModeMan is in heat 3
 * (iHeatNum), or when the first entry of the rank table D_00536730 is 3 or more. The character
 * iCharacters[D_00536708[0]] (0-9) selects music event 10, 2, 8, 1, 5, 9, 3, 7, 4 or 6; other values do nothing. It
 * stores MusicSys.mCurrentSongNum in unk_62AC, starts the song "chartune" through MusicSys.mVtbl slot 2 (12, the music
 * volume control, 0), tags it 201 (func_002B49C0), sends the event through slot 4, sets the song's unk_5C
 * (func_002B4A08) so that only forced events apply, and starts it with func_002B3C28.
 */
extern "C" void func_0028CDF8(char* self)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    if (*(int*)(self + 0x608C) == 2) {
        return;
    }
    sRace_28CDF8* race = (sRace_28CDF8*)SSXApp->GameModeMan;
    if (race->mode != 3) {
        return;
    }
    if (D_00536730[0] >= 3) {
        return;
    }
    int track = -1;
    switch (race->tracks[D_00536708[0]]) {
    case 0: track = 10; break;
    case 1: track = 2; break;
    case 2: track = 8; break;
    case 3: track = 1; break;
    case 4: track = 5; break;
    case 5: track = 9; break;
    case 6: track = 3; break;
    case 7: track = 7; break;
    case 8: track = 4; break;
    case 9: track = 6; break;
    }
    if (track < 0) {
        return;
    }
    char* mon = self + 0x118;
    *(int*)(self + 0x62AC) = *(int*)(self + 0x508);
    sBmVtCD48* vt = *(sBmVtCD48**)(self + 0x5558);
    ((tBmLoadCD48)vt[2].fn)(mon + vt[2].delta, str_chartune, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    func_002B49C0(mon, 201);
    sBmVtCD48* vt2 = *(sBmVtCD48**)(self + 0x5558);
    ((tBmSetCD48)vt2[4].fn)(mon + vt2[4].delta, track);
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

struct sBmInfo_CF98 {
    int f0;                 // 0x0
    char pad_0x4[0x60];
};


static inline void bmLoad_CF98(void* self, char* name, int bank, int id)
{
    char* mon = (char*)self + 0x118;
    sBmVtCD48* vt = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmLoadCD48)vt[2].fn)(mon + vt[2].delta, name, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, bank, 0), 0);
    func_002B49C0(mon, id);
}

/**
 * Starts the music for the current radio mode and race location.
 *
 * Returns at once in a replay (cSSXAudio_ReplayMode) unless unk_6294 is set. Clears unk_6280 and the rider-event words
 * at 0x623C and 0x6240 (func_0028D630) and sets unk_62A0 to -1. Songs are started through MusicSys.mVtbl slot 2 with
 * 12, a volume control (cSSXAudio_GetCurrAppliedVolumePtr) and 0, tagged with func_002B49C0, and given events through slot 4.
 *
 * In radio mode 2 (unk_608C) it starts the ambience "Peak1Amb", "Peak2Amb" or "Peak3Amb" by @p type, or, when that is
 * -1, by func_002A1E20 of the race location (1, 2, anything else), on the ambient control (8) and tagged 101-103; then
 * func_002B46D0, sets unk_6280 to 1 and calls func_002B3CC0(0) and func_002B3C28. Else, at location 17 or above with @p
 * a2 and @p requestLine zero and func_002A42C8 false, it starts the theme "Peak1", "Peak2" or "Peak3" (by the location) on the
 * music control, tagged 1-3, func_002B3CC0(1), event 12 and func_002B3C28. Otherwise it starts the current song of the
 * active list (null name), with MusicSys.mCurrentSongNum saved in unk_62A4 and the song's SEDVALUE (func_002B4908) in
 * unk_5778, func_002B3CC0(1), event @p startEvent (36 when @p requestLine is set at location 17 or above), then func_0028F108 with the
 * focus rider (cSSXAudio_GetFocusRider(-1)).
 *
 * @param startEvent  music event sent to the playlist song (callers pass 0 or 36)
 * @param skipPeakTheme  non-zero skips the peak theme (func_0028E100 passes 1)
 * @param type  peak for radio mode 2 (1, 2, else 3), or -1 to take it from the location
 * @param requestLine  non-zero skips the peak theme and selects event 36 on the peaks; cFEStateRequestLine_onWidgetMenu passes 1
 */
extern "C" void func_0028CF98(void* self, int startEvent, int skipPeakTheme, int type, int requestLine)
{
    if (cSSXAudio_ReplayMode(self) != 0 && *(int*)((char*)self + 0x6294) == 0) {
        return;
    }
    *(int*)((char*)self + 0x6280) = 0;
    *(int*)((char*)self + 0x62A0) = -1;
    *(int*)((char*)self + 0x623C) = 0;
    *(int*)((char*)self + 0x6240) = 0;
    if (*(int*)((char*)self + 0x608C) == 2) {
        int peakNumber = type == -1 ? func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0))) : type;
        if (peakNumber == 1) {
            bmLoad_CF98(self, str_Peak1Amb, 8, 101);
        } else if (peakNumber == 2) {
            bmLoad_CF98(self, str_Peak2Amb, 8, 102);
        } else {
            bmLoad_CF98(self, str_Peak3Amb, 8, 103);
        }
        char* mon = (char*)self + 0x118;
        func_002B46D0(mon);
        *(int*)((char*)self + 0x6280) = 1;
        func_002B3CC0(mon, 0);
        func_002B3C28(mon, 0x7F);
        return;
    }
    sBmInfo_CF98 info = *(sBmInfo_CF98*)cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    char* mon = (char*)self + 0x118;
    if (info.f0 >= 17 && skipPeakTheme == 0 && requestLine == 0 && func_002A42C8(self) == 0) {
        int peakNumber = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (peakNumber == 1) {
            bmLoad_CF98(self, str_Peak1_b, 1, 1);
        } else if (peakNumber == 2) {
            bmLoad_CF98(self, str_Peak2_b, 1, 2);
        } else {
            bmLoad_CF98(self, str_Peak3_b, 1, 3);
        }
        func_002B3CC0(mon, 1);
        sBmVtCD48* vt2 = *(sBmVtCD48**)((char*)self + 0x5558);
        ((tBmSetCD48)vt2[4].fn)(mon + vt2[4].delta, 0xC);
        func_002B3C28(mon, 0x7F);
        return;
    }
    sBmVtCD48* vt = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmLoadCD48)vt[2].fn)(mon + vt[2].delta, 0, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    *(int*)((char*)self + 0x62A4) = *(int*)((char*)self + 0x508);
    *(int*)((char*)self + 0x5778) = func_002B4908(mon);
    func_002B3CC0(mon, 1);
    if (requestLine != 0 && info.f0 >= 17) {
        startEvent = 36;
    }
    sBmVtCD48* vt2 = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmSetCD48)vt2[4].fn)(mon + vt2[4].delta, startEvent);
    func_0028F108(self, cSSXAudio_GetFocusRider(self, -1));
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D488);
#ifdef SKIP_ASM
extern "C" unsigned int cSoundSys_SndRand(void* rng);
extern "C" int func_0028D8A0(void* self);
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
extern "C" void func_0028D488(void* vself)
{
    char* self = (char*)vself;
    unsigned int count = *(unsigned int*)(self + 0x504);
    int prev = *(int*)(self + 0x62A4);
    *(unsigned int*)(self + 0x508) = ((cSoundSys_SndRand(*(void**)(**(char***)(self + 0x118) + 0x1D8)) & 0x7FFF) * count) / 0x7FFF;
    int n = ((cSoundSys_SndRand(*(void**)(**(char***)(self + 0x118) + 0x1D8)) & 0x7FFF) * 10) / 0x7FFF + 1;
    int i;
    for (i = 0; i < n; i++)
        func_002B42B8(self + 0x118, 1, func_0028D8A0(self));
    char* monitor = self + 0x118;
    if (prev == *(int*)(self + 0x508)) {
        func_002B42B8(monitor, 1, func_0028D8A0(self));
        if (prev == *(int*)(self + 0x508))
            func_002B42B8(monitor, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D5A0);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002A1E20(void* self, int id);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);

/**
 * Restarts the music when the playing peak ambience does not belong to location @p id.
 *
 * Does nothing while unk_6284 is 23 (its idle value). Otherwise func_002A1E20 maps @p id to its peak (1, 2, else 3,
 * expecting song id 101, 102 or 103) and, when the current song id (func_002B49E0) differs, calls func_0028CF98(self,
 * 0, 0, peak, 0). The only caller, func_0028E8C0, passes unk_6284.
 *
 * @param id  a race location id
 */
extern "C" void func_0028D5A0(void* self, int id)
{
    int cur;
    int type;
    int want;
    if (*(int*)((char*)self + 0x6284) == 23) {
        return;
    }
    cur = func_002B49E0((char*)self + 0x118);
    type = func_002A1E20(self, id);
    if (type == 1) {
        want = 101;
    } else if (type == 2) {
        want = 102;
    } else {
        want = 103;
    }
    if (want != cur) {
        func_0028CF98(self, 0, 0, type, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D630);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);

struct sBmVt630 { short delta; short index; int (*fn)(void*); };
struct sBmVt630b { short delta; short index; int (*fn)(void*, int); };

/**
 * Starts rider music event @p id for the rider @p obj, which then owns the rider-event slot.
 *
 * Does nothing during a big challenge (mInBigChallenge or mBigChallengeStarting) or when the event in the word at
 * 0x623C is already @p id. With no owner (unk_62A0 == -1) only the focus rider (cSSXAudio_GetFocusRider(-1)) can start
 * one, and its AI index (virtual slot 7 of the cRider vtable at +0x6C0) becomes unk_62A0; otherwise @p obj must have
 * the owner's AI index. It then sends the previous event + 1 (if one was running) and @p id through MusicSys.mVtbl slot
 * 4, stores
 * @p id at 0x623C and clears the word at 0x6240. Called by func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
extern "C" void func_0028D630(void* self, int id, void* obj)
{
    int ok;
    if (*(int*)((char*)self + 0x5FD4) != 0) return;
    if (*(int*)((char*)self + 0x5FD8) != 0) return;
    if (*(int*)((char*)self + 0x623C) == id) return;
    ok = 0;
    if (*(int*)((char*)self + 0x62A0) == -1) {
        // PORT: cSSXAudio_GetFocusRider returns an object pointer as int
        if ((int)obj == cSSXAudio_GetFocusRider(self, -1)) {
            char* subObj = (char*)obj + 0x6C0;
            sBmVt630* vtEntry = &(*(sBmVt630**)subObj)[7];
            ok = 1;
            *(int*)((char*)self + 0x62A0) = vtEntry->fn(subObj + vtEntry->delta);
        }
    } else {
        char* subObj = (char*)obj + 0x6C0;
        sBmVt630* vtEntry = &(*(sBmVt630**)subObj)[7];
        ok = vtEntry->fn(subObj + vtEntry->delta) == *(int*)((char*)self + 0x62A0);
    }
    if (ok) {
        int n = *(int*)((char*)self + 0x623C);
        char* mon = (char*)self + 0x118;
        if (n != 0) {
            sBmVt630b* vt = *(sBmVt630b**)((char*)self + 0x5558);
            vt[4].fn(mon + vt[4].delta, n + 1);
        }
        *(int*)((char*)self + 0x623C) = id;
        *(int*)((char*)self + 0x6240) = 0;
        {
            sBmVt630b* vt = *(sBmVt630b**)((char*)self + 0x5558);
            vt[4].fn(mon + vt[4].delta, id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D740);
#ifdef SKIP_ASM
extern "C" void func_0028D7D8(void* self, void* a1);

struct sVtEnt { short delta; short index; int (*fn)(void*); };

/**
 * Ends rider music event @p id for the owning rider @p obj, or records it as the follow-up of the running one.
 *
 * Does nothing when the word at 0x6240 already holds @p id or when @p obj is not the owner (its AI index, cRider
 * virtual slot 7, differs from unk_62A0; no owner counts as not). When the running event (word at 0x623C) is @p id, @p
 * id is stored at 0x6240; otherwise func_0028D7D8 releases the slot. Called by func_0028D988.
 *
 * @param id  the music event
 * @param obj  the rider (cRider*)
 */
extern "C" void func_0028D740(void* self, int id, void* obj)
{
    int ok;
    if (*(int*)((char*)self + 0x6240) == id) {
        return;
    }
    ok = 0;
    if (*(int*)((char*)self + 0x62A0) != -1) {
        char* subObj = (char*)obj + 0x6C0;
        sVtEnt* vtEntry = &(*(sVtEnt**)subObj)[7];
        ok = vtEntry->fn(subObj + vtEntry->delta) == *(int*)((char*)self + 0x62A0);
    }
    if (ok) {
        if (*(int*)((char*)self + 0x623C) == id) {
            *(int*)((char*)self + 0x6240) = id;
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
int func_0028D7D8_impl(void* self, void* obj) __asm__("func_0028D7D8");

/**
 * Releases the rider-event slot that func_0028D630 gave to a rider and ends its music event.
 *
 * With @p obj null, or when @p obj's AI index (cRider virtual slot 7) equals the owner unk_62A0, it sets unk_62A0 to
 * -1; then, if an event is running (word at 0x623C), it sends that event + 1 through MusicSys.mVtbl slot 4 and clears
 * the words at 0x623C and 0x6240. The unit declares it void although it returns the flag.
 *
 * @param obj  the rider (cRider*), or null to release unconditionally
 * @return 1 when the slot was released, else 0
 */
int func_0028D7D8_impl(void* self, void* obj)
{
    int ok = 0;
    if (obj == 0) {
        ok = 1;
        *(int*)((char*)self + 0x62A0) = -1;
    } else {
        int none = -1;
        if (*(int*)((char*)self + 0x62A0) != none) {
            char* subObj = (char*)obj + 0x6C0;
            sVtEnt* vtEntry = &(*(sVtEnt**)subObj)[7];
            if (vtEntry->fn(subObj + vtEntry->delta) == *(int*)((char*)self + 0x62A0)) {
                *(int*)((char*)self + 0x62A0) = none;
                ok = 1;
            }
        }
    }
    if (ok) {
        int n = *(int*)((char*)self + 0x623C);
        if (n != 0) {
            char* mon = (char*)self + 0x118;
            sBmVtD7D8* vt = *(sBmVtD7D8**)((char*)self + 0x5558);
            vt[4].fn(mon + vt[4].delta, n + 1);
            *(int*)((char*)self + 0x623C) = 0;
            *(int*)((char*)self + 0x6240) = 0;
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
int func_0028D898(void* self)
{
    return *(int*)((char*)self + 0x623C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D8A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" int func_0028D8A0(void* self)
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
 * Returns whether the radio plays music: 1 when the radio mode unk_608C is 0, 1 or 3, 0 for 2 (mountain) and above 3.
 *
 * @return 1 or 0
 */
extern "C" int func_0028D960(void* self)
{
    unsigned int state = *(unsigned int*)((char*)self + 0x608c);
    if (state < 2 || state == 3) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D988);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DEF0);
#ifdef SKIP_ASM
/**
 * Stores @p newValue in unk_629C and returns whether it differed from the old value.
 *
 * Callers pass 11, 13 and 18; func_0028DF08 resets the field to -1.
 *
 * @param newValue  the new value
 * @return 1 when the value changed, else 0
 */
extern "C" int func_0028DEF0(void* self, int newValue)
{
    int old = *(int*)((char*)self + 0x629c);
    *(int*)((char*)self + 0x629c) = newValue;
    return old != newValue;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DF08__FPv);
#ifdef SKIP_ASM
/**
 * Sets unk_629C to -1, so that the next func_0028DEF0 call reports a change.
 */
void func_0028DF08(void* self)
{
    *(int*)((char*)self + 0x629C) = -1;
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
 * When func_002A10C0(self, 10) is 0: fades the song out over 2 s (func_002B3D48) unless unk_62B8 is set, sends music
 * event 18 through MusicSys.mVtbl slot 4, picks a new song (func_0028D488) and adds two timed callbacks to the
 * cBankManager's cBankMonitor (cMonitor_MONITOR_Create): func_0028E088 after 2000 with data 1, 1 and func_0028E548 after 1500
 * with data 3 and the new song's SEDVALUE (func_002B4908). Otherwise, unless @p quiet, it adds func_0028E068 after 100,
 * which calls this again quietly.
 *
 * @param quiet  non-zero: don't schedule a retry
 * @return 1 when the switch started, else 0
 */
// PORT: 64-bit long sound key
extern "C" int func_0028DF18(void* self, int quiet)
{
    int ok = 0;
    if (func_002A10C0(self, 10) == 0) {
        char* mon = (char*)self + 0x118;
        if (*(int*)((char*)self + 0x62B8) == 0) {
            func_002B3D48(mon, 2.0f);
        }
        sBmVtDF18* vt = *(sBmVtDF18**)((char*)self + 0x5558);
        ok = 1;
        ((tBmSetDF18)vt[4].fn)(mon + vt[4].delta, 18);
        func_0028D488(self);
        cMonitor_MONITOR_Create_DF18(**(char***)((char*)self + 0x118) + 0x1D8, 2000, D_004A3648_DF18->v, 0, 1, 1, 0, 0);
        char* bankMonitor = **(char***)((char*)self + 0x118) + 0x1D8;
        const sSndKey8_DF18* key = D_004A3650_DF18;
        int sedValue = func_002B4908(mon);
        cMonitor_MONITOR_Create_DF18(bankMonitor, 1500, key->v, 0, 3, sedValue, 0, 0);
    } else if (quiet == 0) {
        cMonitor_MONITOR_Create_DF18(**(char***)((char*)self + 0x118) + 0x1D8, 100, D_004A3658_DF18->v, 0, 0, 0, 0, 0);
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
 * @return 1 when func_0028DF18 started the switch, else 0
 */
extern "C" int func_0028E068(void* self)
{
    return func_0028DF18(self, 1) != 0;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", func_0028E088);
#ifdef SKIP_ASM
extern "C" int func_002B2550(int);

/**
 * Timed callback that asks func_0028E100 to act once the old song has stopped.
 *
 * When there is no current song (MusicSys.mCurrent) or func_002B2550 of it returns -1, or when @p event's mData2
 * (+0x20) is 1, it sets unk_6274 to 1 and unk_6278 to @p event's mData1 (+0x1C) and returns 1; otherwise it returns 0.
 * func_0028DF18 schedules it with data 1, 1.
 *
 * @param event  the cMonitorInstance that fired
 * @return 1 when the request was posted, else 0
 */
extern "C" int func_0028E088(char *self, char *event) {
    int obj;
    int eventValue;

    obj = *(int *)(self + 0x520);
    if (obj == 0 || func_002B2550(obj) == -1) {
        *(int *)(self + 0x6274) = 1;
        *(int *)(self + 0x6278) = *(int *)(event + 0x1C);
        return 1;
    }
    eventValue = *(int *)(event + 0x20);
    if (eventValue == 1) {
        *(int *)(self + 0x6274) = eventValue;
        *(int *)(self + 0x6278) = *(int *)(event + 0x1C);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E100);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void* cAudioInterface_getBE();
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(void* self);
extern "C" int func_0028D960(void* self);
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

static inline void bmLoad_E100(char* self, char* bank, int n)
{
    char* mon = self + 0x118;
    sBmVtE100* vt = *(sBmVtE100**)(self + 0x5558);
    ((tBmLoadE100)vt[2].fn)(mon + vt[2].delta, bank, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 0);
    func_002B49C0(mon, n);
}

static inline void bmLoadBank_E100(char* self, int t)
{
    if (t == 1) {
        bmLoad_E100(self, str_Peak1_b, 1);
    } else if (t == 2) {
        bmLoad_E100(self, str_Peak2_b, 2);
    } else {
        bmLoad_E100(self, str_Peak3_b, 3);
    }
    char* mon = self + 0x118;
    sBmVtE100* vt2 = *(sBmVtE100**)(self + 0x5558);
    ((tBmSetE100)vt2[4].fn)(mon + vt2[4].delta, 0xC);
    func_002B3C28(mon, 0x7F);
}

/**
 * Carries out the music request that func_0028E088 posted; called every frame from cSSXAudio_Update while
 * mAudioUpdateState is 6.
 *
 * Does nothing unless unk_6274 is set; it is cleared at the end. Request 0 (unk_6278): when the radio plays music
 * (func_0028D960), starts the peak theme for unk_6284's location (resetting unk_6284 to 23), or for the race location
 * when unk_6284 is already 23: "Peak1", "Peak2" or "Peak3" by func_002A1E20 (1, 2, else 3), on the music control,
 * tagged 1-3, event 12 and func_002B3C28. Request 2: when the radio plays music and unk_6284 is not 23: at locations
 * 14-16 (func_002A4030) a new song (func_0028D488) and func_0028CF98(self, 36, 1, -1, 0), otherwise the peak theme for
 * unk_6284; unk_6284 is reset to 23. Request 4: func_0028CF98(self, 0, 0, -1, 0). Other requests: when the radio plays
 * music, func_0028CF98 with event 36 for request 3, else 0.
 */
extern "C" void func_0028E100(char* self)
{
    if (*(int*)(self + 0x6274) == 0)
        return;
    int st = *(int*)(self + 0x6278);
    int id;
    if (st == 0) {
        if (func_0028D960(self)) {
            int locGroup;
            id = *(int*)(self + 0x6284);
            if (id == 23) {
                locGroup = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            } else {
                locGroup = func_002A1E20(self, id);
                *(int*)(self + 0x6284) = 23;
            }
            bmLoadBank_E100(self, locGroup);
        }
    } else if (st == 2) {
        if (func_0028D960(self)) {
            id = *(int*)(self + 0x6284);
            if (id != 23) {
                if (func_002A4030(self)) {
                    func_0028D488(self);
                    func_0028CF98(self, 36, 1, -1, 0);
                } else {
                    id = *(int*)(self + 0x6284);
                    bmLoadBank_E100(self, func_002A1E20(self, id));
                }
                *(int*)(self + 0x6284) = 23;
            }
        }
    } else if (st == 4) {
        func_0028CF98(self, 0, 0, -1, 0);
    } else {
        if (func_0028D960(self)) {
            if (*(int*)(self + 0x6278) == 3)
                func_0028CF98(self, 36, 0, -1, 0);
            else
                func_0028CF98(self, 0, 0, -1, 0);
        }
    }
    *(int*)(self + 0x6274) = 0;
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
extern "C" int func_0028E510(char* self, char* ev)
{
    char* mon = self + 0x118;
    sBmVtE510* vt = *(sBmVtE510**)(self + 0x5558);
    vt[4].fn(mon + vt[4].delta, *(int*)(ev + 0x1C));
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/bankmanager", func_0028E548);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
 * newRace.cGameEvent modes 6-11 (func_002A42C8): kind 0 sets unk_576C and, in a location of group 1 (func_002A1E20), unk_577C =
 * 1, unk_5780 = 2 and +0x57D4 = 0; kinds 1 and 2 set unk_574C.
 *
 * Kinds 3, 4 and 5 act only when mData2 != -1: with the peak locked (cBENewPlayerInterface_isPeakLocked1) in a group-1
 * location whose unk_579C entry is 1, they set unk_5770 when func_002A40E0 or func_002A4158 accept the location and
 * clear the entry; then unk_5774 = 1 and unk_5778 = mData2. Kind 7 sets unk_5784 and unk_5774, stores the current
 * song's mSedValue (func_002B4908) in unk_5778 and clears +0x57D8 (group 2) or +0x57DC. Other kinds do nothing.
 *
 * @param ev  the cMonitorInstance (mData1 = event kind, mData2 = its argument)
 * @return 1
 */
extern "C" int func_0028E548(char* self, char* ev)
{
    int st = *(int*)(ev + 0x1C);
    if (st == 0) {
        char* mon = self + 0x5560;
        func_002B11B0(mon, 0);
        func_002A26F0(self, 1, 0);
        func_002B1758(mon, 0);
        if (func_002A42C8(self) == 0) {
            *(int*)(self + 0x576C) = 1;
            int locGroup = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            if (locGroup == 1) {
                *(int*)(self + 0x577C) = locGroup;
                *(int*)(self + 0x5780) = 2;
                *(int*)(self + 0x57D4) = 0;
            }
        }
    } else if (st == 1 || st == 2) {
        func_002B11B0(self + 0x5560, 0);
        if (*(int*)(ev + 0x1C) == 2)
            func_002A26F0(self, 0, 1);
        else
            func_002A26F0(self, 1, 1);
        func_002B1758(self + 0x5560, 0);
        if (func_002A42C8(self) == 0)
            *(int*)(self + 0x574C) = 1;
    } else if (st == 3 || st == 5 || st == 4) {
        if (*(int*)(ev + 0x20) != -1) {
        if (*(int*)(ev + 0x1C) == 3 || *(int*)(ev + 0x1C) == 4) {
            func_002B11B0(self + 0x5560, 0);
            if (*(int*)(ev + 0x1C) == 4)
                func_002A26F0(self, 0, 0);
            else
                func_002A26F0(self, 1, 0);
            func_002B1758(self + 0x5560, 0);
        }
        if (func_002A42C8(self) == 0 && cBENewPlayerInterface_isPeakLocked1(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1) != 0) {
            int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0));
            int locGroup = func_002A1E20(self, id);
            if (locGroup == 1) {
                int lock = ((sLocks_E548*)(self + 0x579C))->v[id];
                if (lock == locGroup) {
                    if (func_002A40E0(self, id) != 0 || func_002A4158(self, id) != 0)
                        *(int*)(self + 0x5770) = lock;
                    ((sLocks_E548*)(self + 0x579C))->v[id] = 0;
                }
            }
        }
        *(int*)(self + 0x5774) = 1;
        *(int*)(self + 0x5778) = *(int*)(ev + 0x20);
        }
    } else if (st == 7) {
        char* mon = self + 0x5560;
        func_002B11B0(mon, 0);
        func_002A26F0(self, 1, 0);
        func_002B1758(mon, 0);
        *(int*)(self + 0x5784) = 1;
        *(int*)(self + 0x5774) = 1;
        *(int*)(self + 0x5778) = func_002B4908(self + 0x118);
        if (func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0))) == 2)
            *(int*)(self + 0x57D8) = 0;
        else
            *(int*)(self + 0x57DC) = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E888);
#ifdef SKIP_ASM
extern "C" void func_0028E8C0(void* self, int id, int a2);

/**
 * Ends the transport music: when unk_6254 is set (func_0028E8C0 event 20) it runs event 19 and clears it.
 *
 * Called with gSSXAudio by cSSXScriptEngine_ClearNISPlayListState when the playing NIS list ends.
 */
extern "C" void func_0028E888(void* self)
{
    if (*(int*)((char*)self + 0x6254) != 0) {
        func_0028E8C0(self, 19, 1);
        *(int*)((char*)self + 0x6254) = 0;
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
extern "C" void func_0028D5A0(void* self, int id);
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
 * Id 19 with a2 set and unk_627C set: when the location qualifies (func_002A3FE8) and unk_578C is 0, in a music mode
 * accepted by func_0028D960 and a newRace.cGameMode mode other than 5 and 6, it arms the D_004A3650 event (func_0028E548) with
 * kind 4 (unk_6258 clear) or 5 and the song's mSedValue, and sets music state 0. With unk_578C set it fades the song
 * out over 2 s (unless unk_62B8), then in a group-1 location arms events 0 and 1 (3000 / 2500 ms) once
 * (func_0028DEF0(self, 11)), or else picks a new song (func_0028D488) and arms kinds 1 and 7. Otherwise, for locations
 * 17-21 (func_002A3F90) without unk_6258, it queues DJ speech (func_002A26F0(self, 0, 1)) and sets unk_574C. Clears
 * unk_6258 at the end.
 *
 * Id 20 with a2 set: stores the location (unk_6284) in unk_5814, sets unk_6254, clears the speech-cue flags
 * (func_002A4550) and the three pending callbacks D_004A3648..D_004A3658 (cMonitor_MONITOR_DestroyAll1), stops the tone, cSSXAudio_WipeoutSlideEnd
 * / cSSXAudio_WipeoutTumbleEnd on rider 0 and cSSXAudio_EndUberSpin. In a qualifying music mode it either plays the "pktrans" song
 * (cMusicSys slot 2, tagged 401, state 2 or 3 by location group, volume 127, paused) for an unlocked 14-16 location, or
 * arms D_004A3648 kinds 3 / 2 and may queue speech; otherwise it calls func_0028D5A0(unk_6284) and sets state 30. Then
 * sets unk_627C. Ids 21-25 pick a new song, fade out over 1 s and arm kind 3 after 3000 ms. Any other id (and 19 / 20
 * with a2 == 0) sets the music state through slot 4.
 *
 * The NIS script engine passes its audio events here with a2 = 0; the transport-sequence starter passes 20 and
 * func_0028E888 passes 19, both with 1.
 *
 * @param vself  the cSSXAudio
 * @param id  the music event id (a music state for the default case)
 * @param transportCall  nonzero to treat ids 19 and 20 as the transport end / start
 */
// PORT: 64-bit long sound key
extern "C" void func_0028E8C0(void* vself, int id, int transportCall)
{
    char* self = (char*)vself;
    if (id == 19 && transportCall != 0 && *(int*)(self + 0x627C) != 0) {
        if (func_002A3FE8(self) != 0 && *(int*)(self + 0x578C) == 0) {
            if (func_0028D960(self)) {
                cBE_getInterface_Fv(cBE_getBE(), 0);
                int mode = cBENewRaceInterface_newRace.cGameMode;
                if (mode != 5 && mode != 6) {
                    if (*(int*)(self + 0x6258) == 0) {
                        char* monitor = **(char***)(self + 0x118) + 0x1D8;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = func_002B4908(self + 0x118);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 4, sedValue, 0, 0);
                    } else {
                        char* monitor = **(char***)(self + 0x118) + 0x1D8;
                        const sSndKey8_E8C0* key = D_004A3650_E8C0;
                        int sedValue = func_002B4908(self + 0x118);
                        cMonitor_MONITOR_Create(monitor, 0, key->v, 0, 5, sedValue, 0, 0);
                    }
                    char* mon = self + 0x118;
                    sBmVtE8C0* vt = *(sBmVtE8C0**)(self + 0x5558);
                    ((tBmSetE8C0)vt[4].fn)(mon + vt[4].delta, 0);
                }
            }
        } else if (*(int*)(self + 0x578C) != 0) {
            if (func_0028D960(self)) {
                // NOTE: the target keeps this 1 in $s1 across five calls (a named local, not a literal).
                int wantType = 1;
                int locationGroup = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
                if (*(int*)(self + 0x62B8) == 0) {
                    func_002B3D48(self + 0x118, 2.0f);
                }
                if (locationGroup == wantType) {
                    if (func_0028DEF0(self, 11)) {
                        cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 3000, D_004A3648_E8C0->v, 0, 0, 1, 0, 0);
                        cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 2500, D_004A3650_E8C0->v, 0, 0, 0, 0, 0);
                    }
                } else {
                    func_0028DF08(self);
                    func_0028D488(self);
                    cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 2000, D_004A3648_E8C0->v, 0, 1, 1, 0, 0);
                    cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 1500, D_004A3650_E8C0->v, 0, 7, 0, 0, 0);
                }
            }
        } else {
            if (func_002A3F90(self) && *(int*)(self + 0x6258) == 0) {
                func_002A26F0(self, 0, 1);
                func_002B1758(self + 0x5560, 0);
                *(int*)(self + 0x574C) = 1;
            }
        }
        *(int*)(self + 0x6258) = 0;
    } else if (id == 20) {
        if (transportCall != 0) {
            *(int*)(self + 0x6258) = 0;
            *(int*)(self + 0x577C) = 0;
            *(int*)(self + 0x6254) = 1;
            *(int*)(self + 0x5814) = *(int*)(self + 0x6284);
            func_0028DF08(self);
            char* mon = self + 0x118;
            *(int*)(self + 0x578C) = 0;
            func_002A4550(self);
            long key = D_004A3648_E8C0->v;
            cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, key);
            cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, D_004A3650_E8C0->v);
            cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, D_004A3658_E8C0->v);
            cSSXAudio_StopTone(self, 5.029983997344971f);
            cSSXAudio_WipeoutSlideEnd(self, (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_WipeoutTumbleEnd(self, (int)((cAI*)cAudioInterface_getAI())->Riders[0]);
            cSSXAudio_EndUberSpin(self, 0);
            if (func_0028D960(self)) {
                int flag = 1;
                if (func_002A4040(self) == 0) {
                    if (func_002A4078(self) || func_002A40E8(self)) {
                        if (*(int*)SSXApp->GameModeMan >= 2) {
                            func_0028D488(self);
                            flag = 0;
                            cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 10, key, 0, 3, 1, 0, 0);
                        }
                    }
                }
                if (*(int*)(self + 0x6284) == 23 || func_002A4030_id(self, *(int*)(self + 0x6284)) == 0
                    || *(int*)(self + (*(int*)(self + 0x6284) << 2) + 0x579C) != 1) {
                    if (flag && *(int*)(self + 0x6284) != 23) {
                        func_0028D488(self);
                        cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 10, D_004A3648_E8C0->v, 0, 2, 1, 0, 0);
                    }
                    if (func_002A4718(self) && func_002A4A38(self) == 0) {
                        func_002A26F0(self, 0, 1);
                        func_002B1758(self + 0x5560, 0);
                        *(int*)(self + 0x5754) = 1;
                        *(int*)(self + 0x6258) = 1;
                    }
                } else {
                    char* mon2 = self + 0x118;
                    if (func_002A4A38(self) == 0) {
                        func_002B11B0(self + 0x5560, 0);
                    }
                    sBmVtE8C0* vt = *(sBmVtE8C0**)(self + 0x5558);
                    ((tBmLoadE8C0)vt[2].fn)(mon2 + vt[2].delta, str_pktrans, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 1);
                    func_002B49C0(mon2, 401);
                    if (func_002A1E20(self, *(int*)(self + 0x6284)) == 2) {
                        sBmVtE8C0* vt2 = *(sBmVtE8C0**)(self + 0x5558);
                        ((tBmSetE8C0)vt2[4].fn)(mon2 + vt2[4].delta, 2);
                    } else {
                        sBmVtE8C0* vt2 = *(sBmVtE8C0**)(self + 0x5558);
                        ((tBmSetE8C0)vt2[4].fn)(mon2 + vt2[4].delta, 3);
                    }
                    func_002B3C28(mon2, 0x7F);
                    func_002B3A70(mon2);
                }
            } else {
                func_0028D5A0(self, *(int*)(self + 0x6284));
                sBmVtE8C0* vt = *(sBmVtE8C0**)(self + 0x5558);
                ((tBmSetE8C0)vt[4].fn)(mon + vt[4].delta, 0x1E);
            }
            *(int*)(self + 0x627C) = 1;
        }
    } else if (id >= 21 && id <= 25) {
        if (func_0028D960(self)) {
            func_0028D488(self);
            func_002B3D48(self + 0x118, 1.0f);
            cMonitor_MONITOR_Create(**(char***)(self + 0x118) + 0x1D8, 3000, D_004A3648_E8C0->v, 0, 3, 1, 0, 0);
        }
    } else {
        char* mon = self + 0x118;
        sBmVtE8C0* vt = *(sBmVtE8C0**)(self + 0x5558);
        ((tBmSetE8C0)vt[4].fn)(mon + vt[4].delta, id);
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
extern "C" void func_0028EF90(void* self)
{
    *(int*)((char*)self + 0x6258) = 0;
    *(int*)((char*)self + 0x577C) = 0;
    func_0028DF08(self);
    *(int*)((char*)self + 0x578C) = 0;
    func_0028D488(self);
    cMonitor_MONITOR_Create(**(char***)((char*)self + 0x118) + 0x1D8, 10, D_004A3648_EF90->v, 0, 2, 1, 0, 0);
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
 * When the current song's tag (func_002B49E0) is 1-3 it sets full volume (func_002B3C28(127)). Otherwise it starts a
 * new ramp when unk_6288 is not 2 (unk_6288 = 2, counter unk_628C = 0), advances the counter up to the tier's maximum
 * and sets the volume to 127 x counter / maximum, clamped to 0..1. Called by cSSXAudio_Update.
 */
extern "C" void func_0028F000(void* self)
{
    char* mon = (char*)self + 0x118;
    int st = func_002B49E0(mon);
    if (st >= 1 && st <= 3) {
        func_002B3C28(mon, 0x7F);
        return;
    }
    if (*(int*)((char*)self + 0x6288) != 2) {
        *(int*)((char*)self + 0x6288) = 2;
        *(int*)((char*)self + 0x628C) = 0;
    }
    int max = 800;
    int tier = cSSXAudio_GetCharacterLevel(self, cSSXAudio_GetFocusRider(self, -1));
    if (tier == 1) {
        max = 1000;
    } else if (tier == 2) {
        max = 1200;
    }
    int cur = *(int*)((char*)self + 0x628C);
    float ratio = (float)cur / (float)max;
    if (cur < max) {
        *(int*)((char*)self + 0x628C) = cur + 1;
    } else {
        *(int*)((char*)self + 0x628C) = max;
    }
    ratio = ratio <? 1.0f;
    ratio = ratio >? 0.0f;
    func_002B3C28((char*)self + 0x118, (int)(ratio * 127.0f));
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
 * Does nothing when mode equals unk_6290; otherwise stores it. Mode 0 maps to state 0, or 9 when the music code
 * (cMusicSys unk_41C) is not -1; modes 1 and 3-8 map to the same state; 2 and 9 map to 2; other values set nothing.
 * Called by the front-end states (title 0, rider and lodge screens 1, store 5, trophy room 6, ...).
 *
 * @param mode  the front-end music mode
 */
extern "C" void func_0028F140(void* self, int mode)
{
    int bank;
    if (mode == *(int*)((char*)self + 0x6290)) {
        return;
    }
    *(int*)((char*)self + 0x6290) = mode;
    switch (mode) {
    case 0:
        bank = (*(int*)((char*)self + 0x534) == -1) ? 0 : 9;
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
        char* mon = (char*)self + 0x118;
        sBmVtF140* vt = *(sBmVtF140**)((char*)self + 0x5558);
        ((tBmSetF140)vt[4].fn)(mon + vt[4].delta, bank);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F200);
#ifdef SKIP_ASM
extern "C" int func_0028D960(void* self);
extern "C" int func_002B49E0(void* monitor);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);

struct sSndKeyF200 {
    long v;
} __attribute__((packed));
extern const sSndKeyF200 D_004A3648[];

/**
 * Prepares the music for a replay: with no song (or a 201-tagged one) arms a delayed change, else marks a resume.
 *
 * Clears unk_6294 and unk_6298. When no song is playing (mSongPlaying 0) or the current song's tag is 201: in a music
 * mode accepted by func_0028D960 it restores mCurrentSongNum from unk_62AC for a 201 song and arms D_004A3648
 * (func_0028E088) after 10 ms with data (3, 1); then sets unk_6294. Otherwise sets unk_6298. Called from the replay
 * code; func_0028F2C0 undoes it.
 */
// PORT: 64-bit long sound key
extern "C" void func_0028F200(void* self)
{
    char* mon = (char*)self + 0x118;
    *(int*)((char*)self + 0x6294) = 0;
    *(int*)((char*)self + 0x6298) = 0;
    if (*(int*)((char*)self + 0x530) == 0 || func_002B49E0(mon) == 201) {
        if (func_0028D960(self)) {
            if (func_002B49E0(mon) == 201) {
                *(int*)((char*)self + 0x508) = *(int*)((char*)self + 0x62AC);
            }
            cMonitor_MONITOR_Create(**(char***)((char*)self + 0x118) + 0x1D8, 10, D_004A3648[0].v, 0, 3, 1, 0, 0);
        }
        *(int*)((char*)self + 0x6294) = 1;
    } else {
        *(int*)((char*)self + 0x6298) = 1;
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
 * Ends the replay music: stops the song when func_0028F200 set unk_6294, or sets music state 36 when unk_6298.
 *
 * Clears both flags.
 */
extern "C" void func_0028F2C0(void* self)
{
    if (*(int*)((char*)self + 0x6294) != 0) {
        func_002B3AC0((char*)self + 0x118);
    } else if (*(int*)((char*)self + 0x6298) != 0) {
        char* obj = (char*)self + 0x118;
        sBankMgrVEntry* vt = *(sBankMgrVEntry**)((char*)self + 0x5558);
        vt[4].fn(obj + vt[4].delta, 36);
    }
    *(int*)((char*)self + 0x6294) = 0;
    *(int*)((char*)self + 0x6298) = 0;
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
 * When the current song has no tag (func_002B49E0 == -1) and code is 10, 11 or 18 it calls cMusicSys's virtual slot 3
 * with 8 (only while D_004A3620 is 1) and clears mSongPlaying. Slot 3 of D_00483688.
 *
 * @param code  the music state
 */
extern "C" void func_0028F328(void* self, int code)
{
    void* monitor = (char*)self + 0x118;
    func_002B3B88(monitor, code);
    if (func_002B49E0(monitor) == -1) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (D_004A3620 == 1) {
                ((cBankMonitor_F328*)((char*)self + 0x118))->v3(8);
            }
            *(int*)((char*)self + 0x530) = 0;
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
 * When the current song has no tag and code is 10, 11 or 18 it calls virtual slot 3 with 8 (only while D_004A3620 is 1)
 * and clears mSongPlaying. Slot 4 of D_00483688; the music code calls it to change state.
 *
 * @param code  the new music state
 * @return func_002B3BC0's result (whether the state changed)
 */
extern "C" int func_0028F3C8(void* self, int code)
{
    void* monitor = (char*)self + 0x118;
    int changed = func_002B3BC0(monitor, code);
    if (func_002B49E0(monitor) == -1) {
        if ((unsigned)(code - 10) < 2 || code == 18) {
            if (D_004A3620 == 1) {
                ((cBankMonitor_F328*)((char*)self + 0x118))->v3(8);
            }
            *(int*)((char*)self + 0x530) = 0;
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
extern "C" void func_0028F478(void* self)
{
    func_002B35A0((char*)self + 0x118);
    if (cSSXAudio_GetCurrAppliedVolume(self, 1, 0) > 0.0f) {
        char* game = (char*)SSXApp->Game;
        if (game != 0) {
            char* owner = *(char**)(game + 0x48);
            if (owner != 0) {
                void* list = owner + 0x18;
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
extern "C" int func_0028D960(void* self);
extern "C" void* func_002B3AC0(void*);

/**
 * Stops the current song (func_002B3AC0) when the music mode allows music (func_0028D960).
 */
extern "C" void func_0028F520(void* self)
{
    if (func_0028D960(self) != 0) {
        func_002B3AC0((char*)self + 0x118);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F558);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void func_0028EF90(void* self);

/**
 * Sets the music location (unk_6284) and, when it is the race's current location, restarts its music.
 *
 * The restart is func_0028EF90.
 *
 * @param id  the location id
 */
extern "C" void func_0028F558(void* self, int id)
{
    *(int*)((char*)self + 0x6284) = id;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (iface != 0) {
        if (*cBENewRaceInterface_getLocation(iface) == *(int*)((char*)self + 0x6284)) {
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
extern "C" void func_0028F768(void* self);
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
 * sound effects (cSSXAudio_PauseSFX) and sets unk_6088 so func_0028F678 resumes them. Plays menu sound 9; outside music
 * mode 2 it fades the song out over 1 s, starts the loading-screen sound (func_0028F768), sets music state 0 and full
 * volume. Called when the map screen opens.
 */
extern "C" void func_0028F5B8(void* self)
{
    *(int*)((char*)self + 0x6088) = 0;
    if (*(int*)((char*)self + 0x5FB4) != 0) return;
    if (cAudioInterface_isInGame() == 0) return;
    if (*(int*)((char*)self + 0x5818) == 0) {
        cSSXAudio_PauseSFX(self);
        *(int*)((char*)self + 0x6088) = 1;
    }
    cSSXAudio_MenuSound(self, 9);
    if (*(int*)((char*)self + 0x608C) != 2) {
        char* mon = (char*)self + 0x118;
        func_002B3D48(mon, 1.0f);
        func_0028F768(self);
        sBmVtF5B8* vt = *(sBmVtF5B8**)((char*)self + 0x5558);
        vt[4].fn(mon + vt[4].delta, 0);
        func_002B3C28(mon, 0x7F);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F678);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResumeSFX(void* self);
extern "C" int func_002A3F90(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028FA98(void* self, float v);

/**
 * Leaves the map screen: resumes the sound effects paused by func_0028F5B8 and stops the loading-screen loop.
 *
 * When unk_6088 is set it calls cSSXAudio_ResumeSFX and clears it. When the loop is playing (unk_62A8 >= 0) it stops it
 * with a 1 s fade (func_0028FA98) and, with a1 set in locations 17-21 (func_002A3F90), restarts the location music
 * (func_0028CF98(self, 0, 0, -1, 0)).
 *
 * @param restartMusic  nonzero to restart the location music
 */
extern "C" void func_0028F678(void* self, int restartMusic)
{
    if (*(int*)((char*)self + 0x6088) != 0) {
        cSSXAudio_ResumeSFX(self);
        *(int*)((char*)self + 0x6088) = 0;
    }
    if (*(int*)((char*)self + 0x62A8) >= 0) {
        func_0028FA98(self, 1.0f);
        if (restartMusic != 0 && func_002A3F90(self) != 0) {
            func_0028CF98(self, 0, 0, -1, 0);
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
extern "C" void func_0028F700(void* self)
{
    cBankManager_BANK_SyncLoad((cBankManager*)(**(void***)((char*)self + 0x118)), 13, D_004A3624, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F730);
#ifdef SKIP_ASM
extern "C" void func_0028FA98(void* self, float v);
extern "C" void cBankManager_BANK_UnLoad(cBankManager* self, int i);

/**
 * Stops the loading-screen sound at once (func_0028FA98(0)) and unloads bank 13 (cBankManager_BANK_UnLoad).
 *
 * Called by the audio system's shutdown.
 */
extern "C" void func_0028F730(void* self)
{
    func_0028FA98(self, 0.0f);
    cBankManager_BANK_UnLoad((cBankManager*)(**(void***)((char*)self + 0x118)), 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F768);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(void* self);

struct sEntry_F768 {
    signed char kind;
    char pad[0x17];
};
struct sQuad_F768 {
    int a;
    int b;
    int c;
    int d;
};
struct sStack_F768 {
    int field_0x0;                  // 0x1D8
    sEntry_F768 cur;                // 0x1DC
    volatile int depth;             // 0x1F4
    sEntry_F768* entries;           // 0x1F8
    int flags[4];                   // 0x1FC
    int* a34;
    int* a38;
    int* a3C;
    char a40[4][8];
    int* a60;
    int* a64;
    int* a68;
    int* a6C;
    sQuad_F768* a70;
    int* a74;
    int* a78;
    int* a7C;
    int* a80;
    int* a84;
};
struct sMgr_F768 {
    char pad_0x0[0x1D8];
    sStack_F768 st;                 // 0x1D8
};

static inline void bmPush_F768(sMgr_F768** self, int kind)
{
    sMgr_F768* m = *self;
    sStack_F768* st = &m->st;
    m->st.depth++;
    st->entries[m->st.depth] = m->st.cur;
    m->st.flags[m->st.depth] = 0;
    st->a34[m->st.depth] = 0;
    st->a38[m->st.depth] = st->entries[m->st.depth].kind;
    st->a3C[m->st.depth] = 0;
    func_00416210((char*)st + m->st.depth * 8 + 0x40, 0, 8);
    st->a60[m->st.depth] = 0;
    st->a64[m->st.depth] = 0;
    st->a68[m->st.depth] = 0;
    st->a6C[m->st.depth] = 0;
    st->a70[m->st.depth].b = 0;
    st->a70[m->st.depth].a = 100;
    st->a70[m->st.depth].c = 90;
    st->a70[m->st.depth].d = 50;
    st->a74[m->st.depth] = 0;
    st->a80[m->st.depth] = 0;
    st->a84[m->st.depth] = 0;
    st->a78[m->st.depth] = 127;
    st->a7C[m->st.depth] = 1;
    (*(int**)((char*)self + 4))[(*self)->st.depth] = kind;
    (*(int**)((char*)self + 8))[(*self)->st.depth] = 0;
}

#define BM_MIN_F768(a, b) ((a) < (b) ? (a) : (b))
#define BM_MAX_F768(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Starts the loading-screen loop: plays patch 0 of bank 13 at full volume and keeps its handle in unk_62A8.
 *
 * Opens a play-option level as cBankSys_SFX_Begin2(self, 13, 0) does, sets its base volume to 127, uses music volume
 * control 1 (cSSXAudio_GetCurrAppliedVolumePtr(1, 0)) and plays it with cBankSys_SFX_End. Called by the load screen and func_0028F5B8;
 * func_0028FA98 stops it.
 *
 * @param ssxAudio  the cSSXAudio
 */
extern "C" void func_0028F768(void* ssxAudio)
{
    sMgr_F768** self = (sMgr_F768**)ssxAudio;
    bmPush_F768(self, 13);
    sMgr_F768* bankManager = **(sMgr_F768***)((char*)self + 0x118);
    sStack_F768* st2 = &bankManager->st;
    st2->a38[bankManager->st.depth] = 127;
    st2->entries[bankManager->st.depth].kind = BM_MAX_F768(BM_MIN_F768(st2->a38[st2->depth], 127), 0);
    sMgr_F768** pp = *(sMgr_F768***)((char*)self + 0x118);
    (*pp)->st.a3C[(*pp)->st.depth] = (int)cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0);
    *(int*)((char*)self + 0x62A8) = cBankSys_SFX_End(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FA98);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops the loading-screen sound started by func_0028F768 (cBankMonitor_BANKMONITOR_StopHandle, type 1).
 *
 * Does nothing when unk_62A8 is negative; clears it to -1 afterwards.
 *
 * @param fadeTime  the fade time (0 stops at once)
 */
extern "C" void func_0028FA98(void* self, float fadeTime)
{
    int idx = *(int*)((char*)self + 0x62A8);
    if (idx >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle((char*)**(void***)((char*)self + 0x118) + 0x1D8, idx, 1, fadeTime);
        *(int*)((char*)self + 0x62A8) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FAE0);
#ifdef SKIP_ASM
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028D488(void* self);
extern "C" int func_0028D960(void* self);
extern "C" int func_0028DEF0(void* self, int a1);
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
 * Only in a music mode accepted by func_0028D960. Cancels D_004A3648..D_004A3658 (cMonitor_MONITOR_DestroyAll1). When the current
 * song's tag is 1-3 or 401, or no song is playing, it records event 18 (func_0028DEF0), sets +0x5828, stops the song,
 * picks the next one (func_0028D488), starts the location music (func_0028CF98) and arms the D_004A3650 event
 * (func_0028E548) after 3000 ms with kind 4 and the song's mSedValue. Called by the map state.
 */
// PORT: 64-bit long sound keys.
extern "C" void func_0028FAE0(char* self)
{
    if (func_0028D960(self) == 0) {
        return;
    }
    cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, D_004A3648_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, D_004A3650_FAE0[0].v);
    cMonitor_MONITOR_DestroyAll1(**(char***)(self + 0x118) + 0x1D8, D_004A3658_FAE0[0].v);
    int st = func_002B49E0(self + 0x118);
    if ((unsigned int)(st - 1) < 3 || st == 401 || *(int*)(self + 0x530) == 0) {
        func_0028DEF0(self, 18);
        *(int*)(self + 0x5828) = 1;
        func_002B3AC0(self + 0x118);
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
        char* bankMonitor = **(char***)(self + 0x118) + 0x1D8;
        const sBmKeyFAE0* key = D_004A3650_FAE0;
        int sedValue = func_002B4908(self + 0x118);
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
void* func_0028FC38(void* self)
{
    return func_002B3AC0((char*)self + 0x118);
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
 * Plays the "charsel" song (cMusicSys slot 2, tagged 301) and restores the front-end music state (unk_6290).
 *
 * Uses music volume control 1 and loads synchronously. Called when the network-configuration screen closes.
 */
extern "C" void func_0028FC58(void* self)
{
    char* mon = (char*)self + 0x118;
    sBmVtFC58* vt = *(sBmVtFC58**)((char*)self + 0x5558);
    ((tBmLoadFC58)vt[2].fn)(mon + vt[2].delta, str_charsel, 0xC, cSSXAudio_GetCurrAppliedVolumePtr(self, 1, 0), 1);
    func_002B49C0(mon, 301);
    sBmVtFC58* vt2 = *(sBmVtFC58**)((char*)self + 0x5558);
    ((tBmSetFC58)vt2[4].fn)(mon + vt2[4].delta, *(int*)((char*)self + 0x6290));
}
#endif

