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
// Retail code: fe/festatecharequipdetail, cFEBoltFileManager_cFEBoltFileManager (ctor) ... cFEBoltFileManager_getUseCount (methods, all func_ so far).
//
// NAMES: the class name is derived, not seen: the preview's inline accessor getBoltFileManager__3cFE returns it and
// the source file is fe/feboltfilemanager.cpp; EA's front end pairs getRiderManager / fe/feridermanager.cpp with
// cFERiderManager and getCamera with cFECamera, so the class is cFEBoltFileManager. The record name tBoltFile is
// likewise derived: the preview's constants kNumBoltFiles / kBoltFileFlag_* / kBoltFileType_* call a slot a "bolt
// file", and On Tour's sibling cache (cFEAsyncManager, fe/all_in_one_fe) keeps the same record shape as
// "struct tFEAsyncFile {fileName[256], dataPtr, asyncFileHandle, useAsync, fileSize, ...}".
//
// Size 0xB4030: [R] cFE_cFE builds it at +0x1A70 (cFEBoltFileManager_cFEBoltFileManager) and the next member, mFXLoader, is at +0xB5AA0;
// cFEBoltFileManager_init (init) writes mFiles[10][256] (0x120 each), 0xB4000, 0xB4004 and the ten words from 0xB4008.
// No base classes, no vtable (cFEBoltFileManager__cFEBoltFileManager, the destructor, takes the g++ delete flag but stores no vtable).
// A FLAT layout, like main/game.h. Evidence tags:
//   [P] the July 2003 preview's assert strings (fe/feboltfilemanager.cpp);
//   [R] retail's init / destructor (cFEBoltFileManager_init / cFEBoltFileManager_purge / cFEBoltFileManager_freeFile);
//   [U] how retail code uses the field (src/fe/festatecharequipdetail.cpp: the cache's methods cFEBoltFileManager_init ...
//       cFEBoltFileManager_getUseCount, and the sFileCache / sFileSlot view of cFEBoltFileManager_startDeferredModelPartLoads / cFEBoltFileManager_startDeferredPartTextureLoads);
//   [O] SSX On Tour's DWARF, the sibling record tFEAsyncFile (names of the shared fields).

#include "layout.h"

// tBoltFile::flags. [P] kBoltFileFlag_Done is the preview's name for 1 (pFile->flags == kBoltFileFlag_Done once a
// file is in); the other names are ours: [U] init and release store 0, cFEBoltFileManager_loadFile starts an asynchronous load (2),
// cFEBoltFileManager_requestFile queues a deferred one (3), cFEBoltFileManager_update / cFEBoltFileManager_releaseFile finish them. cFEBoltFileManager_getFileName treats -1 as no
// file, a value the cache never stores.
enum tBoltFileFlag {
    kBoltFileFlag_Invalid = -1,
    kBoltFileFlag_Idle = 0,
    kBoltFileFlag_Done = 1,
    kBoltFileFlag_Loading = 2,
    kBoltFileFlag_Queued = 3
};

// tBoltFile::fileType, the kind of file a slot holds (names ours; [P] the preview checks fileType < kBoltFileType_Num).
// [U] the callers of cFEBoltFileManager_requestFile: cFERider_loadModels requests the bolt's model part (8), cFEStateCharEquip and
// cFEStateRewards the character's icon package and the store previews (9), the trophy room, mountain room and reward
// database their reward textures (10), cFERider_loadTextures / queueLoadExtraTextures a bolt's texture (11).
// cFEBoltFileManager_startDeferredModelPartLoads / cFEBoltFileManager_startDeferredPartTextureLoads order the deferred loads of types 8 and 11.
enum tBoltFileType {
    kBoltFileType_ModelPart = 8,
    kBoltFileType_Picture = 9,
    kBoltFileType_RewardTexture = 10,
    kBoltFileType_PartTexture = 11
};

// A file index (cFEBoltFileManager_requestFile's result) that names no file (name ours): [R] cFEStateCharEquip's constructor sets its
// file indices to -1; [U] the rider manager's file-index table (cBoltFileIndexManager_getPartTextureFile) returns it for a missing key, and
// cBoltFileIndexManager_setModelPartFile / cBoltFileIndexManager_setPartTextureFile take it to look a key up without creating it.
enum { kNoBoltFile = -1 };

// One cached file (0x120). flags: tBoltFileFlag (0 idle, 1 done, 2 asynchronous load in flight, 3 queued).
struct tBoltFile {
    int flags;                      // 0x000 [P] pFile->flags == kBoltFileFlag_Done, mFiles[..].flags; [R] init 0;
                                    //       [U] 0 idle, 1 loaded, 2 loading (async), 3 queued
    char fileName[0x100];           // 0x004 [P] the fileName argument (fileName[0] != '\0'); [O] tFEAsyncFile::fileName
                                    //       (char[256]); [U] cFEBoltFileManager_requestFile strcpy, cFEBoltFileManager_findFile / the sorters compare it
    void* dataPtr;                  // 0x104 [P] pFile->dataPtr; [O] tFEAsyncFile::dataPtr; [U] "Bolt FileData" /
                                    //       "UncompressedData" blocks, cMemMan_free on release
    int asyncFileHandle;            // 0x108 [O] tFEAsyncFile::asyncFileHandle; [U] ASYNCFILE_loadat's handle,
                                    //       ASYNCFILE_getstatus / ASYNCFILE_release
    int async;                      // 0x10C [P] pFile->async; [U] cFEBoltFileManager_requestFile stores its async argument; an in-flight
                                    //       load (flags 2) with it set is polled by cFEBoltFileManager_update
    int fileType;                   // 0x110 [P] the fileType argument (fileType < kBoltFileType_Num); [R] init -1;
                                    //       [U] cFEBoltFileManager_requestFile stores it (the rider manager uses 8 and 11)
    int refCount;                   // 0x114 [P] mFiles[charID][fileIndex].refCount (== 0 / > 0); [U] cFEBoltFileManager_requestFile
                                    //       adds a use, cFEBoltFileManager_releaseFile gives it back; eviction needs 0
    int fileSize;                   // 0x118 [O] tFEAsyncFile::fileSize; [U] BXFILE_sizez / unpacksizez; mMemUsed sums it
    unsigned int stamp;             // 0x11C [U] the least-recently-used / queue stamp (name ours; a bank's unk_B4008
                                    //       counter, 0 = deferred); eviction and cFEBoltFileManager_startNextQueuedLoad pick the lowest (sltu)
};

CHECK_OFFSET(tBoltFile, flags, 0x000);
CHECK_OFFSET(tBoltFile, fileName, 0x004);
CHECK_OFFSET(tBoltFile, dataPtr, 0x104);
CHECK_OFFSET(tBoltFile, asyncFileHandle, 0x108);
CHECK_OFFSET(tBoltFile, async, 0x10C);
CHECK_OFFSET(tBoltFile, fileType, 0x110);
CHECK_OFFSET(tBoltFile, refCount, 0x114);
CHECK_OFFSET(tBoltFile, fileSize, 0x118);
CHECK_OFFSET(tBoltFile, stamp, 0x11C);
CHECK_SIZE(tBoltFile, 0x120);

struct cFEBoltFileManager {
    tBoltFile mFiles[10][256];      // 0x00000 [P] mFiles[charID][fileIndex] (charID < kCHAR_NUM, fileIndex <
                                    //       kNumBoltFiles); [R] init loops 10 x 256 at 0x12000 / 0x120
    int mMemUsed;                   // 0xB4000 [P] mMemUsed + memSize < kMaxMemUsed; [R] init 0; [U] loads add the
                                    //       file size, release subtracts it; cFEBoltFileManager_makeRoom keeps it <= 9,999,999
    int mNumLoadThreads;            // 0xB4004 [P] mNumLoadThreads < kMaxLoadThreads; [R] init 0; [U] cFEBoltFileManager_loadFile counts
                                    //       an async load up, cFEBoltFileManager_update / cFEBoltFileManager_releaseFile down; queued loads start at 0
    int unk_B4008[10];              // 0xB4008 [R] init 1 per bank; [U] each bank's next stamp (tBoltFile::stamp)
};

CHECK_OFFSET(cFEBoltFileManager, mFiles, 0x00000);
CHECK_OFFSET(cFEBoltFileManager, mMemUsed, 0xB4000);
CHECK_OFFSET(cFEBoltFileManager, mNumLoadThreads, 0xB4004);
CHECK_OFFSET(cFEBoltFileManager, unk_B4008, 0xB4008);
CHECK_SIZE(cFEBoltFileManager, 0xB4030);

// The file-index hash table beside the cache (names ours: [R] cFEBoltFileManager_init allocates it as cMemMan_alloc(8004,
// "BoltFileIndexManager") into D_004A1968, so the class is cBoltFileIndexManager). The rider manager keeps, per
// profile index and item id, the cache slots of the item's model-part and part-texture files in it (cBoltFileIndexManager_getModelPartFile /
// cBoltFileIndexManager_setModelPartFile and cBoltFileIndexManager_getPartTextureFile / cBoltFileIndexManager_setPartTextureFile). Retail code: fe/festatecharequipdetail cBoltFileIndexManager_allocNode ...
// cBoltFileIndexManager_setPartTextureFile; no vtable. [R] cBoltFileIndexManager_reset (reset) / cBoltFileIndexManager_findNode (lookup) / cBoltFileIndexManager_freeNode (release).
struct tBoltFileIndexNode {
    int key;                        // 0x0 [U] (profile index << 16) | item id (cBoltFileIndexManager_findNode); -1 when free
    short partTextureMask;          // 0x4 [U] bit per profile index: partTextureFile is set (cBoltFileIndexManager_getPartTextureFile / _CD38)
    short modelPartMask;            // 0x6 [U] bit per profile index: modelPartFile is set (cBoltFileIndexManager_getModelPartFile / _CC30)
    short partTextureFile;          // 0x8 [U] the cache slot of the item's kBoltFileType_PartTexture file
    short modelPartFile;            // 0xA [U] the cache slot of the item's kBoltFileType_ModelPart file
    short prev;                     // 0xC [U] the previous node's index in its bucket chain or the free list, -1 none
    short next;                     // 0xE [U] the next node's index, -1 none
};

CHECK_OFFSET(tBoltFileIndexNode, key, 0x0);
CHECK_OFFSET(tBoltFileIndexNode, partTextureMask, 0x4);
CHECK_OFFSET(tBoltFileIndexNode, modelPartMask, 0x6);
CHECK_OFFSET(tBoltFileIndexNode, partTextureFile, 0x8);
CHECK_OFFSET(tBoltFileIndexNode, modelPartFile, 0xA);
CHECK_OFFSET(tBoltFileIndexNode, prev, 0xC);
CHECK_OFFSET(tBoltFileIndexNode, next, 0xE);
CHECK_SIZE(tBoltFileIndexNode, 0x10);

struct cBoltFileIndexManager {
    tBoltFileIndexNode mNodes[400];     // 0x0000 [R] cBoltFileIndexManager_reset fills 6400 bytes with 0xFF and chains them
    tBoltFileIndexNode* mBuckets[400];  // 0x1900 [R] cBoltFileIndexManager_reset clears 1600 bytes; [U] chain heads by key % 400
    tBoltFileIndexNode* mpFreeList;     // 0x1F40 [R] cBoltFileIndexManager_reset points it at node 0; [U] cBoltFileIndexManager_allocNode pops it
};

CHECK_OFFSET(cBoltFileIndexManager, mNodes, 0x0000);
CHECK_OFFSET(cBoltFileIndexManager, mBuckets, 0x1900);
CHECK_OFFSET(cBoltFileIndexManager, mpFreeList, 0x1F40);
CHECK_SIZE(cBoltFileIndexManager, 0x1F44);

#endif /* FE_FEBOLTFILEMANAGER_H */
