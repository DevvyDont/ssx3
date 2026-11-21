#ifndef UTIL_BXMEMCARD_H
#define UTIL_BXMEMCARD_H

// cMemCard: the abstract memory-card interface (views group D: cMovieDecoder_241540, _241400, _240D90, _240860,
// _23FD40, _23F578 in movie/mcoverlaymanager). cMemCard::CreateInstance (cMemCard_CreateInstance) returns a new
// cPS2MemCard (util/bxps2memcard.h, which writes the base out inline); callers hold a cMemCard* and call the
// virtuals: cMCOverlayManager::mpMemCard (movie/mcoverlaymanager.h, [P] getMemCard), D_004A33D8 (dirtysock).
// EA's source: util/bxmemcard.cpp (bxps2memcard.h's comment; [P] the preview's assert paths).
//
// Size 0x4: [O] On Tour class cMemCard (0x4: the vptr only, no data members). Vtable: [R] cMemCard_vtbl 0x004871E8
// (cMemCard's own: the dtor at slot 1, the pure-virtual stub func_00411530 in the pure slots), cPS2MemCard_vtbl
// 0x00486F78 (76 slots: these 75 plus one cPS2MemCard virtual at slot 76).
// Slot order: [O] On Tour's declaration order of cMemCard's virtuals (slot 1 the destructor, then the pure virtuals and
// the three non-pure ones in order); [R] every slot of cPS2MemCard_vtbl that has a retail name carries the same method
// name (Format 8, DeleteFileSlot 10, IsBusy 11, GetNumFiles 13, GetTitleString 14, ..., IsAvailable / IsAvailable1
// 52 / 53, DidErrorOccur 73, FileExistsError 74). Overloads are numbered in declaration order; for four of them retail's
// symbol suffix differs (noted per slot). Calls go through mVtbl with the {short delta; short index; fn} entry
// (include/vtable.h sVEntry<F>), e.g. ((sVEntry<int(void*, int)>*)self->mVtbl)[kMemCardVt_IsAvailable].
// A FLAT layout: no C++ virtuals here.

#include "layout.h"

struct cMemCard {
    void** mVtbl;                   // 0x000 [R] cPS2MemCard_vtbl (cPS2MemCard ctor) / cMemCard_vtbl
};

CHECK_OFFSET(cMemCard, mVtbl, 0x000);
CHECK_SIZE(cMemCard, 0x4);

// cMemCard's vtable slots ([O] names and order, [R] cPS2MemCard_vtbl). Signatures as On Tour's dump prints them: the
// dump drops most parameter lists (retail: IsAvailable(int slot), DeleteFileSlot(int file), ...).
enum {
    kMemCardVt_dtor = 1,                    // void ~cMemCard()
    kMemCardVt_Init = 2,                    // void Init()
    kMemCardVt_Update = 3,                  // void Update()
    kMemCardVt_GetDevice = 4,               // int GetDevice()
    kMemCardVt_CheckDevice = 5,             // void CheckDevice()
    kMemCardVt_GetDir = 6,                  // void GetDir()
    kMemCardVt_GetDir1 = 7,                 // void GetDir()
    kMemCardVt_Format = 8,                  // void Format()
    kMemCardVt_DidFormatFail = 9,           // bool DidFormatFail()
    kMemCardVt_DeleteFileSlot = 10,         // void DeleteFileSlot()
    kMemCardVt_IsBusy = 11,                 // bool IsBusy()
    kMemCardVt_CleanUp = 12,                // void CleanUp()
    kMemCardVt_GetNumFiles = 13,            // int GetNumFiles()
    kMemCardVt_GetTitleString = 14,         // void GetTitleString()
    kMemCardVt_GetFileDate = 15,            // void GetFileDate()
    kMemCardVt_GetFileTime = 16,            // void GetFileTime()
    kMemCardVt_GetLastSavedFile = 17,       // int GetLastSavedFile()
    kMemCardVt_GetDataSize = 18,            // int GetDataSize()
    kMemCardVt_SetDataSize = 19,            // void SetDataSize()
    kMemCardVt_GetFreeSpace = 20,           // int GetFreeSpace()
    kMemCardVt_GetTotalSpace = 21,          // int GetTotalSpace()
    kMemCardVt_SpaceRequired = 22,          // int SpaceRequired()
    kMemCardVt_SpaceRequired1 = 23,         // int SpaceRequired()
    kMemCardVt_GetBlockSize = 24,           // int GetBlockSize()
    kMemCardVt_GetFreeDirEntries = 25,      // int GetFreeDirEntries()
    kMemCardVt_IsDeviceChanged = 26,        // bool IsDeviceChanged()
    kMemCardVt_ClearDeviceChanges = 27,     // void ClearDeviceChanges()
    kMemCardVt_SetAllDevicesChanged = 28,   // void SetAllDevicesChanged()
    kMemCardVt_StartFileSlotRead = 29,      // void StartFileSlotRead()
    kMemCardVt_ReadData = 30,               // void ReadData()
    kMemCardVt_IsReadWaiting = 31,          // bool IsReadWaiting()
    kMemCardVt_SetReadClose = 32,           // void SetReadClose()
    kMemCardVt_SetReadFailed = 33,          // void SetReadFailed()
    kMemCardVt_CreateFileSlot = 34,         // void CreateFileSlot()
    kMemCardVt_CreateFileSlot1 = 35,        // void CreateFileSlot()
    kMemCardVt_StartFileSlotWrite = 36,     // void StartFileSlotWrite()
    kMemCardVt_StartFileSlotWrite1 = 37,    // void StartFileSlotWrite()
    kMemCardVt_StartFileSlotWrite2 = 38,    // void StartFileSlotWrite()
    kMemCardVt_SetFileSlotInfo = 39,        // void SetFileSlotInfo()
    kMemCardVt_SetBrokenFile = 40,          // void SetBrokenFile()
    kMemCardVt_WriteData = 41,              // void WriteData()
    kMemCardVt_IsWriteWaiting = 42,         // bool IsWriteWaiting()
    kMemCardVt_SetWriteClose = 43,          // void SetWriteClose()
    kMemCardVt_SetWriteFailed = 44,         // void SetWriteFailed()
    kMemCardVt_SetTitleString = 45,         // void SetTitleString()
    kMemCardVt_SetTitleString1 = 46,        // void SetTitleString()
    kMemCardVt_ClearCardChanged = 47,       // void ClearCardChanged()
    kMemCardVt_IsInvalid = 48,              // bool IsInvalid()
    kMemCardVt_IsInvalid1 = 49,             // bool IsInvalid()
    kMemCardVt_IsDamaged = 50,              // bool IsDamaged()
    kMemCardVt_IsDamaged1 = 51,             // bool IsDamaged()
    kMemCardVt_IsAvailable = 52,            // bool IsAvailable()
    kMemCardVt_IsAvailable1 = 53,           // bool IsAvailable()
    kMemCardVt_IsActionable = 54,           // bool IsActionable()
    kMemCardVt_IsFormatted = 55,            // bool IsFormatted(); retail symbol cPS2MemCard_IsFormatted1
    kMemCardVt_IsFormatted1 = 56,           // bool IsFormatted(); retail symbol cPS2MemCard_IsFormatted
    kMemCardVt_IsEncodingCorrect = 57,      // bool IsEncodingCorrect()
    kMemCardVt_IsEncodingCorrect1 = 58,     // bool IsEncodingCorrect()
    kMemCardVt_IsWrongDevice = 59,          // bool IsWrongDevice()
    kMemCardVt_IsWrongDevice1 = 60,         // bool IsWrongDevice()
    kMemCardVt_IsRemovable = 61,            // bool IsRemovable()
    kMemCardVt_IsRemovable1 = 62,           // bool IsRemovable()
    kMemCardVt_CardChanged = 63,            // bool CardChanged(); retail symbol cPS2MemCard_CardChanged1
    kMemCardVt_GetStatus = 64,              // void GetStatus(); retail symbol cPS2MemCard_GetStatus1
    kMemCardVt_IsFileSlotBroken = 65,       // bool IsFileSlotBroken()
    kMemCardVt_IsFileSlotDisabled = 66,     // bool IsFileSlotDisabled()
    kMemCardVt_HasFileName = 67,            // bool HasFileName()
    kMemCardVt_GetMaxFiles = 68,            // int GetMaxFiles()
    kMemCardVt_GetMaxDevices = 69,          // int GetMaxDevices()
    kMemCardVt_GetDeviceName = 70,          // void GetDeviceName()
    kMemCardVt_GetDeviceNameRaw = 71,       // void GetDeviceNameRaw()
    kMemCardVt_HasCustomDeviceName = 72,    // bool HasCustomDeviceName()
    kMemCardVt_DidErrorOccur = 73,          // bool DidErrorOccur()
    kMemCardVt_FileExistsError = 74,        // bool FileExistsError()
    kMemCardVt_IsFileInfoValid = 75         // bool IsFileInfoValid()
};

#endif /* UTIL_BXMEMCARD_H */
