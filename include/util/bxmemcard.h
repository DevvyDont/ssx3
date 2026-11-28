#ifndef UTIL_BXMEMCARD_H
#define UTIL_BXMEMCARD_H

// cMemCard: the abstract memory-card interface (views group D: cMovieDecoder_241540, _241400, _240D90, _240860,
// _23FD40, _23F578 in movie/mcoverlaymanager). cMemCard::CreateInstance (cMemCard_CreateInstance) returns a new
// cPS2MemCard (util/bxps2memcard.h, which writes the base out inline); callers hold a cMemCard* and call the
// virtuals: cMCOverlayManager::mpMemCard (movie/mcoverlaymanager.h, [P] getMemCard), sharedlogin_memCard (dirtysock).
// EA's source: util/bxmemcard.cpp (bxps2memcard.h's comment; [P] the preview's assert paths).
//
// Size 0x4: [O] On Tour class cMemCard (0x4: the vptr only, no data members). Vtable: [R] cMemCard_vtbl 0x004871E8
// (cMemCard's own: the dtor at slot 1, the pure-virtual stub func_00411530 in the pure slots), cPS2MemCard_vtbl
// 0x00486F78 (76 slots: these 75 plus one cPS2MemCard virtual at slot 76).
// Slot order: [O] On Tour's declaration order of cMemCard's virtuals (slot 1 the destructor, then the pure virtuals and
// the three non-pure ones in order); [R] every slot of cPS2MemCard_vtbl that has a retail name carries the same method
// name (Format 8, DeleteFileSlot 10, IsBusy 11, GetNumFiles 13, GetTitleString 14, ..., IsAvailable / IsAvailable1
// 52 / 53, DidErrorOccur 73, FileExistsError 74). Overloads are numbered in declaration order; for four of them retail's
// symbol suffix differs (noted per slot).
//
// A real polymorphic class, unlike the flat headers: no data, the vptr at 0, and the code only calls through it
// (never constructed or copied here, so no vtable or type_info is emitted for it); callers hold a cMemCard* and call
// `mpMemCard->IsBusy()`. cPS2MemCard (util/bxps2memcard.h) stays flat, its mVtbl at 0 being this vptr.
// Overloads keep the numbered spelling of retail's symbols (GetDir / GetDir1 ...): several stub implementations do
// not show their parameter lists. Parameters: [R] the matched implementations (cPS2MemCard_*); return types: [O].

#include "layout.h"

class cMemCard {
public:
    virtual ~cMemCard();                                             // slot 1
    virtual void Init() = 0;                                                  // slot 2
    virtual void Update() = 0;                                                // slot 3
    virtual int GetDevice() = 0;                                              // slot 4
    virtual void CheckDevice(int device) = 0;                                 // slot 5
    virtual void GetDir(const char* name) = 0;                                // slot 6
    virtual void GetDir1(unsigned short* name) = 0;                           // slot 7
    virtual void Format() = 0;                                                // slot 8
    virtual bool DidFormatFail() = 0;                                         // slot 9
    virtual void DeleteFileSlot(int fileSlot) = 0;                            // slot 10
    virtual bool IsBusy() = 0;                                                // slot 11
    virtual void CleanUp() = 0;                                               // slot 12
    virtual int GetNumFiles() = 0;                                            // slot 13
    virtual void GetTitleString(int fileSlot, unsigned short* dst) = 0;       // slot 14
    virtual void GetFileDate(int fileSlot, unsigned short* dst) = 0;          // slot 15
    virtual void GetFileTime(int fileSlot, unsigned short* dst) = 0;          // slot 16
    virtual int GetLastSavedFile() = 0;                                       // slot 17
    virtual int GetDataSize() = 0;                                            // slot 18
    virtual void SetDataSize(int size) = 0;                                   // slot 19
    virtual int GetFreeSpace() = 0;                                           // slot 20
    virtual int GetTotalSpace() = 0;                                          // slot 21
    virtual int SpaceRequired(int byteSize) = 0;                              // slot 22
    virtual int SpaceRequired1() = 0;                                         // slot 23
    virtual int GetBlockSize() = 0;                                           // slot 24
    virtual int GetFreeDirEntries() = 0;                                      // slot 25
    virtual bool IsDeviceChanged(int device);                                 // slot 26
    virtual void ClearDeviceChanges(int device);                              // slot 27
    // 26 / 27 take the device: [R] cMCOverlayManager's card poll passes it (IsDeviceChanged(i) in its device loop,
    // ClearDeviceChanges(mCurrCheckDevice)); cMemCard's own stubs ignore it and cPS2MemCard keeps them; On Tour's
    // DWARF lists no parameters for any of cMemCard's declarations (its pure ones lose theirs too).
    virtual void SetAllDevicesChanged();                                      // slot 28
    virtual void StartFileSlotRead(int fileSlot, int buffer, int transferSize) = 0;// slot 29
    virtual void ReadData(int buffer, int transferSize) = 0;                  // slot 30
    virtual bool IsReadWaiting() = 0;                                         // slot 31
    virtual void SetReadClose() = 0;                                          // slot 32
    virtual void SetReadFailed() = 0;                                         // slot 33
    virtual void CreateFileSlot(const char* name) = 0;                        // slot 34
    virtual void CreateFileSlot1(unsigned short* name) = 0;                   // slot 35
    virtual void StartFileSlotWrite(const char* name, int writeBuffer, int writeSize) = 0;// slot 36
    virtual void StartFileSlotWrite1(unsigned short* name, int writeBuffer, int writeSize) = 0;// slot 37
    virtual void StartFileSlotWrite2(int fileSlot, int writeBuffer, int writeSize) = 0;// slot 38
    virtual void SetFileSlotInfo(const char* name, int flag) = 0;             // slot 39
    virtual void SetBrokenFile(int fileSlot) = 0;                             // slot 40
    virtual void WriteData(int buffer, int transferSize) = 0;                 // slot 41
    virtual bool IsWriteWaiting() = 0;                                        // slot 42
    virtual void SetWriteClose() = 0;                                         // slot 43
    virtual void SetWriteFailed() = 0;                                        // slot 44
    virtual void SetTitleString() = 0;                                        // slot 45
    virtual void SetTitleString1() = 0;                                       // slot 46
    virtual void ClearCardChanged() = 0;                                      // slot 47
    virtual bool IsInvalid(int device) = 0;                                   // slot 48
    virtual bool IsInvalid1() = 0;                                            // slot 49
    virtual bool IsDamaged(int device) = 0;                                   // slot 50; [R] callers pass the port
    virtual bool IsDamaged1() = 0;                                            // slot 51
    virtual bool IsAvailable(int device) = 0;                                 // slot 52
    virtual bool IsAvailable1() = 0;                                          // slot 53
    virtual bool IsActionable(int device) = 0;                                // slot 54; [R] callers pass the port
    virtual bool IsFormatted() = 0;                                           // slot 55; retail symbol cPS2MemCard_IsFormatted1
    virtual bool IsFormatted1(int device) = 0;                                // slot 56; retail symbol cPS2MemCard_IsFormatted
    virtual bool IsEncodingCorrect(int device) = 0;                           // slot 57; [R] callers pass the port
    virtual bool IsEncodingCorrect1() = 0;                                    // slot 58
    virtual bool IsWrongDevice() = 0;                                         // slot 59
    virtual bool IsWrongDevice1(int device) = 0;                              // slot 60; [R] callers pass the port
    virtual bool IsRemovable(int device) = 0;                                 // slot 61; [R] callers pass the port
    virtual bool IsRemovable1() = 0;                                          // slot 62
    virtual bool CardChanged() = 0;                                           // slot 63; retail symbol cPS2MemCard_CardChanged1
    virtual void GetStatus(unsigned short* outText) = 0;                      // slot 64; retail symbol cPS2MemCard_GetStatus1
    virtual bool IsFileSlotBroken(int fileSlot) = 0;                          // slot 65
    virtual bool IsFileSlotDisabled(int fileSlot) = 0;                        // slot 66
    virtual bool HasFileName(int fileSlot) = 0;                               // slot 67
    virtual int GetMaxFiles() = 0;                                            // slot 68
    virtual int GetMaxDevices() = 0;                                          // slot 69
    virtual void GetDeviceName(int device, unsigned short* dst, int dstLen) = 0; // slot 70; [R] callers pass a length
    virtual void GetDeviceNameRaw(int device, unsigned short* dst, int dstLen) = 0; // slot 71
    virtual bool HasCustomDeviceName() = 0;                                   // slot 72
    virtual bool DidErrorOccur() = 0;                                         // slot 73
    virtual bool FileExistsError();                                           // slot 74
    virtual bool IsFileInfoValid() = 0;                                       // slot 75

    // Methods bound to their retail symbols by asm labels.
    void AsciiToShiftJIS(short* dst, const char* src) __asm__("cMemCard_AsciiToShiftJIS");
    void ShiftJISToAscii(char* dst, const short* src) __asm__("cMemCard_ShiftJISToAscii");
};

CHECK_SIZE(cMemCard, 0x4);

#endif /* UTIL_BXMEMCARD_H */
