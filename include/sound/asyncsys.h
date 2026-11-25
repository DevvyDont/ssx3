#ifndef SOUND_ASYNCSYS_H
#define SOUND_ASYNCSYS_H

// cAsyncSys: the asynchronous sound-bank loader. It queues bank loads (user handle = bank number, file-name slot,
// memory location, main-memory flags) in four 6-entry FIFOs and runs them one at a time through ASYNCFILE_*; when a
// load starts, needs memory, or ends it calls its own virtuals ASYNCSYS_OnBeginAsyncLoad / OnAsyncMainMemAlloc /
// OnAsyncMainMemSize / OnEndAsyncLoad, which cBankManager overrides (cBankManager is the only cAsyncSys: its primary
// base at offset 0, sound/bankmanager.h). EA source d:/ssx/code/source/sound/asyncsys.cpp (the July 2003 preview's
// assert strings "mpBuffer == NULL", "AsyncSysBuffer", "Size < mBufferSize", "mMemType.Peek() == ..."); retail
// methods: cAsyncSys_cAsyncSys / cAsyncSys__cAsyncSys / cAsyncSys_DoCancel (0x289DF0..0x28A1E8, in the splat unit
// sound/ssxAudio), ASYNCSYS_Init .. ASYNCSYS_OnCurAsyncMainMemAlloc (sound/asyncsys) and the out-of-line inlines
// ASYNCSYS_Busy / GetCurrOperationUserHandle / CancelCurrent / CancelAll (bx/seg_1BA100).
//
// Size 0x1D8 [O] (On Tour class cAsyncSys, same offsets): data 0x1D4, then the vptr. cAsyncSys declares virtuals and
// has no base, so g++ 2.95 puts its vptr after its data (0x1D4) [R] (ctor stores cAsyncSys_vtbl 0x483B10: slot 1 the
// dtor, slots 2..5 __pure_virtual). gpAsyncSys (retail 0x4A3610) is the current cAsyncSys [O] (On Tour static
// gpAsyncSys, next to gpBankManager in both builds); [R] the ctor stores this, the dtor clears it.
//
// cCircBuffer is defined here because cAsyncSys embeds four of them; it used to live in sound/ssxaudio.h (cSSXAudio's
// mPlayHistory), which must include this header instead of defining it again.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructor cAsyncSys_cAsyncSys (four operator new[](0x18, "CircFIFOBuff") buffers) and destructor;
//   [P] the July 2003 preview's out-of-line inlines (ASYNCSYS_Busy, ASYNCSYS_GetCurrOperationUserHandle), carried to
//       retail by tools/layout_xlate.py (every observed offset unchanged);
//   [O] SSX On Tour's DWARF (class cAsyncSys 0x1D8; class cCircBuffer 0x10);
//   [U] how the retail methods use the field.

#include "layout.h"

// Where a sound bank's data lives (cAsyncSys::mMemType's entries, cBankInstance::mAllocLocation). [O] On Tour's enum
// eSoundMemLocation, names and values (ASYNCSYS_QueueLoad's memType; the preview asserts "mMemType.Peek() ==
// keSoundMemLocation_Dynamic"). [U] ASYNCSYS_StartLoad loads 0 and 2 into the shared buffer and asks the owner for
// main memory for 1; cBankInstance_SyncLoad loads 2 with sound-memory flags 0x100, 1 into the instance's main memory.
enum eSoundMemLocation {
    keSoundMemLocation_Dynamic = 0, keSoundMemLocation_Main = 1, keSoundMemLocation_Sound = 2
};

// How an asynchronous bank load ended (OnEndAsyncLoad's endState). [O] On Tour's enum eAsyncSysEndState, names and
// values. [U] ASYNCSYS_Update reports 0 for a loaded bank and 2 (with handle -1) for an empty file.
enum eAsyncSysEndState {
    keAsyncSysEndState_Complete = 0, keAsyncSysEndState_Cancelled = 1, keAsyncSysEndState_Error = 2
};

// cCircBuffer (On Tour class, 0x10): a FIFO of ints.
struct cCircBuffer {
    int ArraySize; // 0x0 [O]; [R] cAsyncSys ctor stores 6 into each queue's, cSSXAudio ctor 200 into mPlayHistory's
    int* Array;    // 0x4 [O]; [R] operator new(ArraySize * 4, "CircFIFOBuff")
    int Head;      // 0x8 [O]; [U] the push index (QueueLoad writes Array[Head]); cAsyncSys ctor 0
    int Tail;      // 0xC [O]; [U] the pop / peek index (DoCancel, GetCurrOperationUserHandle read Array[Tail]); ctor 0
};

struct cAsyncSys {
    int mHandle;                // 0x000 [O]; [U] the ASYNCFILE handle of the load in progress: StartLoad stores
                                //       ASYNCFILE_loadat / ASYNCFILE_load's result, DoCancel passes it to
                                //       ASYNCFILE_cancel / ASYNCFILE_release, ASYNCSYS_Update polls it
    int mBusy;                  // 0x004 [O]; [P] ASYNCSYS_Busy (preview lw 0x4, seen); [R] ctor 0; [U] DoCancel clears,
                                //       StartLoad sets
    int mFileIndex;             // 0x008 [O]; [R] ctor 0; [U] QueueLoad strcpy()s the name into mFilename[mFileIndex]
                                //       and advances it (mod 6)
    char mFilename[6][64];      // 0x00C [O] (DWARF prints char[64][6]); [U] QueueLoad (sll 6 + 0xC), DoCancel passes
                                //       mFilename[slot] to OnEndAsyncLoad
    cCircBuffer mUserHandle;    // 0x18C [O]; [R] ctor ArraySize 6, Array "CircFIFOBuff"; [P] GetCurrOperationUserHandle
                                //       (0x190 / 0x194 / 0x198, seen)
    cCircBuffer mFilenameIndex; // 0x19C [O]; [R] ctor (6, "CircFIFOBuff")
    cCircBuffer mMemType;       // 0x1AC [O] eSoundMemLocation queue; [R] ctor (6, "CircFIFOBuff"); preview assert
                                //       "mMemType.Peek() == keSoundMemLocation_Dynamic"
    cCircBuffer mMainMemFlags;  // 0x1BC [O]; [R] ctor (6, "CircFIFOBuff")
    char* mpBuffer;             // 0x1CC [O]; [R] ctor 0; [U] ASYNCSYS_Init allocates it (operator new(size,
                                //       "AsyncSysBuffer")), DeInit frees it
    int mBufferSize;            // 0x1D0 [O]; [R] ctor 0; [U] ASYNCSYS_Init stores the size, DeInit clears it
    void** mVtbl;               // 0x1D4 [R] cAsyncSys_vtbl (ctor / dtor); cBankManager_vtbl (0x483AD8) in the
                                //       cBankManager; DoCancel / ASYNCSYS_Update call slots 2..5 through it

    // Methods bound to their retail symbols by asm labels.
    int ASYNCSYS_Busy() __asm__("cAsyncSys_ASYNCSYS_Busy");
    int ASYNCSYS_GetCurrOperationUserHandle() __asm__("cAsyncSys_ASYNCSYS_GetCurrOperationUserHandle");
    void ASYNCSYS_CancelCurrent() __asm__("cAsyncSys_ASYNCSYS_CancelCurrent");
    void ASYNCSYS_CancelAll() __asm__("cAsyncSys_ASYNCSYS_CancelAll");
    void ASYNCSYS_Init(unsigned int x, int flags) __asm__("cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi");
    void ASYNCSYS_DeInit() __asm__("cAsyncSys_ASYNCSYS_DeInit");
    void ASYNCSYS_StartLoad() __asm__("cAsyncSys_ASYNCSYS_StartLoad");
    void ASYNCSYS_QueueLoad(int bankId, const char* name, int memType, int memFlags)
        __asm__("cAsyncSys_ASYNCSYS_QueueLoad");
    void ASYNCSYS_Cancel(int id) __asm__("cAsyncSys_ASYNCSYS_Cancel");
    void ASYNCSYS_Update() __asm__("cAsyncSys_ASYNCSYS_Update");
    void ASYNCSYS_OnCurAsyncMainMemAlloc(int size) __asm__("cAsyncSys_ASYNCSYS_OnCurAsyncMainMemAlloc");
    void DoCancel(int flush) __asm__("cAsyncSys_DoCancel");
};

CHECK_OFFSET(cCircBuffer, ArraySize, 0x0);
CHECK_OFFSET(cCircBuffer, Array, 0x4);
CHECK_OFFSET(cCircBuffer, Head, 0x8);
CHECK_OFFSET(cCircBuffer, Tail, 0xC);
CHECK_SIZE(cCircBuffer, 0x10);

CHECK_OFFSET(cAsyncSys, mHandle, 0x000);
CHECK_OFFSET(cAsyncSys, mBusy, 0x004);
CHECK_OFFSET(cAsyncSys, mFileIndex, 0x008);
CHECK_OFFSET(cAsyncSys, mFilename, 0x00C);
CHECK_OFFSET(cAsyncSys, mUserHandle, 0x18C);
CHECK_OFFSET(cAsyncSys, mFilenameIndex, 0x19C);
CHECK_OFFSET(cAsyncSys, mMemType, 0x1AC);
CHECK_OFFSET(cAsyncSys, mMainMemFlags, 0x1BC);
CHECK_OFFSET(cAsyncSys, mpBuffer, 0x1CC);
CHECK_OFFSET(cAsyncSys, mBufferSize, 0x1D0);
CHECK_OFFSET(cAsyncSys, mVtbl, 0x1D4);
CHECK_SIZE(cAsyncSys, 0x1D8);

#endif /* SOUND_ASYNCSYS_H */
