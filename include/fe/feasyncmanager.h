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
};

CHECK_OFFSET(cFEAsyncManager, fileList, 0x000);
CHECK_OFFSET(cFEAsyncManager, mMapGfxBigFile, 0x58C);
CHECK_SIZE(cFEAsyncManager, 0x590);

#endif
