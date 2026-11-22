#ifndef FE_FEBOLTFILEMANAGER_H
#define FE_FEBOLTFILEMANAGER_H

// cFEBoltFileManager: the front end's file cache for the riders' bolt-on (equipment) files - icons, previews and the
// rider manager's model / texture files - kept per character in ten banks of 256 file slots, loaded synchronously or
// through ASYNCFILE, unpacked, reference counted and evicted least-recently-used under a 9,999,999-byte budget. It is
// embedded in the front end: cFE::mBoltFileManager (fe/fereal.h, cRealFE + 0x1A70); D_004A196C points at the one
// instance (set by the constructor, read by the qsort comparators).
// EA's source: d:/ssx/code/source/fe/feboltfilemanager.cpp (the July 2003 preview's assert path; its asserts name the
// members mFiles[charID][fileIndex] (.flags / .refCount), pFile->dataPtr / ->async, mMemUsed, mNumLoadThreads and the
// constants kCHAR_NUM, kNumBoltFiles, kMaxMemUsed, kMaxLoadThreads, kBoltFileFlag_Done, kBoltFileType_Num).
// Retail code: fe/festatecharequipdetail, func_0019CDF0 (ctor) ... func_0019E3A8 (methods, all func_ so far).
//
// NAMES: the class name is derived, not seen: the preview's inline accessor getBoltFileManager__3cFE returns it and
// the source file is fe/feboltfilemanager.cpp; EA's front end pairs getRiderManager / fe/feridermanager.cpp with
// cFERiderManager and getCamera with cFECamera, so the class is cFEBoltFileManager. The record name tBoltFile is
// likewise derived: the preview's constants kNumBoltFiles / kBoltFileFlag_* / kBoltFileType_* call a slot a "bolt
// file", and On Tour's sibling cache (cFEAsyncManager, fe/all_in_one_fe) keeps the same record shape as
// "struct tFEAsyncFile {fileName[256], dataPtr, asyncFileHandle, useAsync, fileSize, ...}".
//
// Size 0xB4030: [R] cFE_cFE builds it at +0x1A70 (func_0019CDF0) and the next member, mFXLoader, is at +0xB5AA0;
// func_0019CE68 (init) writes mFiles[10][256] (0x120 each), 0xB4000, 0xB4004 and the ten words from 0xB4008.
// No base classes, no vtable (func_0019CE20, the destructor, takes the g++ delete flag but stores no vtable).
// A FLAT layout, like main/game.h. Evidence tags:
//   [P] the July 2003 preview's assert strings (fe/feboltfilemanager.cpp);
//   [R] retail's init / destructor (func_0019CE68 / func_0019CF40 / func_0019D4B8);
//   [U] how retail code uses the field (src/fe/festatecharequipdetail.cpp, the per-function views sTableK19D250,
//       sSlotK19D*, sEquipTable*_D*, sFileCache / sFileSlot);
//   [O] SSX On Tour's DWARF, the sibling record tFEAsyncFile (names of the shared fields).

#include "layout.h"

// One cached file (0x120). flags: 0 idle, 1 done (kBoltFileFlag_Done), 2 asynchronous load in flight, 3 queued.
struct tBoltFile {
    int flags;                      // 0x000 [P] pFile->flags == kBoltFileFlag_Done, mFiles[..].flags; [R] init 0;
                                    //       [U] 0 idle, 1 loaded, 2 loading (async), 3 queued
    char fileName[0x100];           // 0x004 [P] the fileName argument (fileName[0] != '\0'); [O] tFEAsyncFile::fileName
                                    //       (char[256]); [U] func_0019DA10 strcpy, func_0019D428 / the sorters compare it
    void* dataPtr;                  // 0x104 [P] pFile->dataPtr; [O] tFEAsyncFile::dataPtr; [U] "Bolt FileData" /
                                    //       "UncompressedData" blocks, cMemMan_free on release
    int asyncFileHandle;            // 0x108 [O] tFEAsyncFile::asyncFileHandle; [U] ASYNCFILE_loadat's handle,
                                    //       ASYNCFILE_getstatus / ASYNCFILE_release
    int async;                      // 0x10C [P] pFile->async; [U] func_0019DA10 stores its async argument; an in-flight
                                    //       load (flags 2) with it set is polled by func_0019D250
    int fileType;                   // 0x110 [P] the fileType argument (fileType < kBoltFileType_Num); [R] init -1;
                                    //       [U] func_0019DA10 stores it (the rider manager uses 8 and 11)
    int refCount;                   // 0x114 [P] mFiles[charID][fileIndex].refCount (== 0 / > 0); [U] func_0019DA10
                                    //       adds a use, func_0019DC20 gives it back; eviction needs 0
    int fileSize;                   // 0x118 [O] tFEAsyncFile::fileSize; [U] BXFILE_sizez / unpacksizez; mMemUsed sums it
    unsigned int unk_11C;           // 0x11C [U] the least-recently-used / queue stamp (a bank's unk_B4008 counter, 0 =
                                    //       deferred); eviction and func_0019D140 pick the lowest (sltu)
};

CHECK_OFFSET(tBoltFile, flags, 0x000);
CHECK_OFFSET(tBoltFile, fileName, 0x004);
CHECK_OFFSET(tBoltFile, dataPtr, 0x104);
CHECK_OFFSET(tBoltFile, asyncFileHandle, 0x108);
CHECK_OFFSET(tBoltFile, async, 0x10C);
CHECK_OFFSET(tBoltFile, fileType, 0x110);
CHECK_OFFSET(tBoltFile, refCount, 0x114);
CHECK_OFFSET(tBoltFile, fileSize, 0x118);
CHECK_OFFSET(tBoltFile, unk_11C, 0x11C);
CHECK_SIZE(tBoltFile, 0x120);

struct cFEBoltFileManager {
    tBoltFile mFiles[10][256];      // 0x00000 [P] mFiles[charID][fileIndex] (charID < kCHAR_NUM, fileIndex <
                                    //       kNumBoltFiles); [R] init loops 10 x 256 at 0x12000 / 0x120
    int mMemUsed;                   // 0xB4000 [P] mMemUsed + memSize < kMaxMemUsed; [R] init 0; [U] loads add the
                                    //       file size, release subtracts it; func_0019D578 keeps it <= 9,999,999
    int mNumLoadThreads;            // 0xB4004 [P] mNumLoadThreads < kMaxLoadThreads; [R] init 0; [U] func_0019D8B8 counts
                                    //       an async load up, func_0019D250 / func_0019DC20 down; queued loads start at 0
    int unk_B4008[10];              // 0xB4008 [R] init 1 per bank; [U] each bank's next stamp (tBoltFile::unk_11C)
};

CHECK_OFFSET(cFEBoltFileManager, mFiles, 0x00000);
CHECK_OFFSET(cFEBoltFileManager, mMemUsed, 0xB4000);
CHECK_OFFSET(cFEBoltFileManager, mNumLoadThreads, 0xB4004);
CHECK_OFFSET(cFEBoltFileManager, unk_B4008, 0xB4008);
CHECK_SIZE(cFEBoltFileManager, 0xB4030);

#endif /* FE_FEBOLTFILEMANAGER_H */
