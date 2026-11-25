#ifndef FE_FEASYNCMANAGER_H
#define FE_FEASYNCMANAGER_H

// cFEAsyncManager: the front end's asynchronous file loader (cSSXApp::mFEAsyncManager, allocated as
// "FEAsyncManager"): five file slots, each a name, a buffer, an async handle and a status. Methods in
// fe/feasyncfile and fe/ovstatemission.
// Size 0x590 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

// One file slot (On Tour tFEAsyncFile, 0x11C); [U] cFEAsyncManager_Init clears every field of each slot.
struct tFEAsyncFile {
    char fileName[256];             // 0x000 [O]
    void* dataPtr;                  // 0x100 [O]
    int asyncFileHandle;            // 0x104 [O]
    int useAsync;                   // 0x108 [O] (bool there)
    int fileSize;                   // 0x10C [O]
    int texID;                      // 0x110 [O]
    int fileStatus;                 // 0x114 [O]
    int safeToDeleteTick;           // 0x118 [O]
};

CHECK_OFFSET(tFEAsyncFile, useAsync, 0x108);
CHECK_SIZE(tFEAsyncFile, 0x11C);

struct cFEAsyncManager {
    tFEAsyncFile fileList[5];       // 0x000 [O]
    int mMapGfxBigFile;             // 0x58C [O]

    // Methods bound to their retail symbols by asm labels.
    void Load3PeakPic(int slot, int useAsync) __asm__("cFEAsyncManager_Load3PeakPic");
    void Load1PeakPic(int pic, int slot, int useAsync) __asm__("cFEAsyncManager_Load1PeakPic");
    void LoadLocationPic(int id, int slot, int useAsync) __asm__("cFEAsyncManager_LoadLocationPic");
    void UnloadFEAsyncFile(int idx) __asm__("cFEAsyncManager_UnloadFEAsyncFile");
    int GetFileStatus(int index) __asm__("cFEAsyncManager_GetFileStatus__FP15cFEAsyncManageri");
    void SetFileStatus(int index, int status) __asm__("cFEAsyncManager_SetFileStatus__FP15cFEAsyncManagerii");
    void Update() __asm__("cFEAsyncManager_Update");
    int GetFreeAsyncFile() __asm__("cFEAsyncManager_GetFreeAsyncFile");
    void FlushFEAsyncTextures() __asm__("cFEAsyncManager_FlushFEAsyncTextures");
    void LoadDataFile(int idx) __asm__("cFEAsyncManager_LoadDataFile");
    void Init() __asm__("cFEAsyncManager_Init");
    void UnloadAll() __asm__("cFEAsyncManager_UnloadAll");
};

CHECK_OFFSET(cFEAsyncManager, fileList, 0x000);
CHECK_OFFSET(cFEAsyncManager, mMapGfxBigFile, 0x58C);
CHECK_SIZE(cFEAsyncManager, 0x590);

#endif
