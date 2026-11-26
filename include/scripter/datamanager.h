#ifndef SCRIPTER_DATAMANAGER_H
#define SCRIPTER_DATAMANAGER_H

// cDataManager: a pool of attached-data records (tDataInstanceInfo). AttachData registers a loaded block (and, for a
// BIG archive, every member of it, chained through pNext) and calls the virtual OnAttachData / OnAttachDataBigFile
// hooks; DetachData / DetachAll undo it through OnDetachData / OnDetachDataBigFile. cSSXScriptEngine derives from it
// and attaches its script banks with it. EA's source: d:/ssx/code/source/scripter/datamanager.cpp (preview assert
// path); the four empty base hooks sit in scripter/all_in_one_scripter.
//
// Size 0x10: [R] the only constructor call is cSSXScriptEngine_cSSXScriptEngine's
// cDataManager_cDataManager(self + 0x51C, 0, 0x100), and cSSXScriptEngine's own members start at 0x52C; the ctor
// stores the vtable at 0xC. [O] On Tour's cDataManager is 0x10 with the same three members. No bases; the vptr
// follows the data (_vt$12cDataManager D_004825A0: slot 1 the destructor, slots 2-5 the On* hooks, preview names
// OnAttachData__12cDataManagerPCcPviRPvT4 ...).
// The same fields are written out inline in scripter/ssxscriptengine.h (cSSXScriptEngine + 0x51C).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] cDataManager_cDataManager; [U] how retail code uses the field; [O] On Tour's DWARF.

#include "layout.h"

// One attached-data record (On Tour struct tDataInstanceInfo, 0x18). The data handle AttachData returns is the
// root record; a BIG archive's members follow it through pNext.
struct tDataInstanceInfo {
    void* pUserData1;               // 0x00 [O]; [U] AttachData / AttachDataFromBigfile pass &pUserData1 (the record) as
                                    //      the hooks' void*& pUserData1; cDataManager_GetDataUserData reads it
    void* pUserData2;               // 0x04 [O]; [U] passed as record + 4 (pUserData2); GetDataUserData reads it
    void* pData;                    // 0x08 [O]; [U] AttachData stores the block; cDataManager_GetData returns it;
                                    //      DetachData returns it
    int dataSize;                   // 0x0C [O]; [U] AttachData / AttachDataFromBigfile store the size
    int isRoot;                     // 0x10 [O] (a bool in On Tour; retail sw 1 in AttachData, a word);
                                    //      [U] cDataManager_DetachAll detaches every record with it set
    tDataInstanceInfo* pNext;       // 0x14 [O]; [U] the free-list link (AddToEmptyInstanceList, GetEmptyInstance)
                                    //      and the BIG-member chain (AttachDataFromBigfile, DetachData)
};

CHECK_OFFSET(tDataInstanceInfo, pUserData1, 0x00);
CHECK_OFFSET(tDataInstanceInfo, pUserData2, 0x04);
CHECK_OFFSET(tDataInstanceInfo, pData, 0x08);
CHECK_OFFSET(tDataInstanceInfo, dataSize, 0x0C);
CHECK_OFFSET(tDataInstanceInfo, isRoot, 0x10);
CHECK_OFFSET(tDataInstanceInfo, pNext, 0x14);
CHECK_SIZE(tDataInstanceInfo, 0x18);

struct cDataManager {
    int mMaxDataInstances;                      // 0x0 [O]; [R] = maxDataInstances (0x100 from cSSXScriptEngine);
                                                //      [U] cDataManager_DetachAll's loop bound
    tDataInstanceInfo* mpDataInstanceHeap;      // 0x4 [O]; [R] new (max * 0x18, "tDataInstanceInfo", memFlags);
                                                //      [U] the destructor frees it, DetachAll walks it
    tDataInstanceInfo* mpEmptyDataInstanceList; // 0x8 [O]; [R] 0, then every record pushed (AddToEmptyInstanceList);
                                                //      [U] GetEmptyInstance pops (no empty check)
    void** mVtbl;                               // 0xC [R] the ctor and destructor store cDataManager_vtbl (D_004825A0);
                                                //      [U] AttachData calls slot 2 / 4, DetachData slot 3 / 5,
                                                //      AttachDataFromBigfile slot 2

    // Methods bound to their retail symbols by asm labels.
    tDataInstanceInfo* AttachData(int pName, void* data, int size) __asm__("cDataManager_AttachData");
    void DetachAll() __asm__("cDataManager_DetachAll");
    int GetData(tDataInstanceInfo* dataHandle) __asm__("cDataManager_GetData");
    void GetDataUserData(tDataInstanceInfo* dataHandle, int* out0, int* out1) __asm__("cDataManager_GetDataUserData");
    tDataInstanceInfo* AttachDataFromBigfile(char* big, tDataInstanceInfo* prev)
        __asm__("cDataManager_AttachDataFromBigfile");
    void AddToEmptyInstanceList(void* vnode) __asm__("cDataManager_AddToEmptyInstanceList__FPvT0");
    void* GetEmptyInstance() __asm__("cDataManager_GetEmptyInstance__FPv");
    int DetachData(tDataInstanceInfo* entry) __asm__("cDataManager_DetachData");
};

CHECK_OFFSET(cDataManager, mMaxDataInstances, 0x0);
CHECK_OFFSET(cDataManager, mpDataInstanceHeap, 0x4);
CHECK_OFFSET(cDataManager, mpEmptyDataInstanceList, 0x8);
CHECK_OFFSET(cDataManager, mVtbl, 0xC);
CHECK_SIZE(cDataManager, 0x10);

#endif /* SCRIPTER_DATAMANAGER_H */
