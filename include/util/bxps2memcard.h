#ifndef UTIL_BXPS2MEMCARD_H
#define UTIL_BXPS2MEMCARD_H

// cPS2MemCard: the PS2 memory-card driver behind the abstract cMemCard interface (util/bxmemcard.cpp:
// cMemCard::CreateInstance cMemCard_CreateInstance = new cPS2MemCard, DeleteInstance cMemCard_DeleteInstance). It wraps libmc as an
// asynchronous state machine: the public calls store their arguments and call SetState (cPS2MemCard_enterState), which
// issues the sceMc* request; Update (vtable slot 3, cPS2MemCard_Update) polls sceMcSync and moves to the next state.
// Holders: D_004A33D8 (dirtysock/tagsunk.cpp creates and deletes it) and the movie player's object + 0x434
// (movie/movieplayer.cpp).
//
// Size 0x1500 [R]: cMemCard_CreateInstance cMemMan_alloc(0x1500, str_cPS2MemCard, 0x100, 0) then the ctor cPS2MemCard_cPS2MemCard.
// Base: cMemCard (On Tour DWARF: size 0x4, no data, so only its vptr; the dtor cPS2MemCard__cPS2MemCard stores cMemCard's
// vtable cMemCard_vtbl back before the base dtor runs). cPS2MemCard's vtable cPS2MemCard_vtbl (76 slots; 59 slot
// functions in this unit, 17 inline accessors emitted in seg_1D3510 at 0x002D37B8-0x002D39B0). EA's source:
// d:/ssx/code/source/util/bxps2memcard.cpp (preview asserts below); On Tour has no cPS2MemCard.
// Methods: see methods.txt (in retail they sit in the worldpainter/quadtree split, 0x002C3FA0-0x002C6D70).
//
// The libmc calls are identified by argument shape and address order (inferred, not symbol-backed):
//   sceMcOpen sceMcOpen, sceMcMkdir sceMcMkdir, sceMcClose sceMcClose, sceMcSeek sceMcSeek,
//   sceMcRead sceMcRead, sceMcWrite sceMcWrite, sceMcSync sceMcSync, sceMcGetInfo sceMcGetInfo,
//   sceMcGetDir sceMcGetDir, sceMcFormat sceMcFormat, sceMcDelete sceMcDelete, sceMcFlush sceMcFlush.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructor cPS2MemCard_cPS2MemCard, Reset cPS2MemCard_Init (vtable slot 2, called by the ctor; it sets or
//       clears every field from 0x4 to 0x14E8), the allocation size;
//   [P] the July 2003 preview: inline accessors GetState (0x4), GetDevice (0xC), GetMaxFiles (0x10 -> retail
//       0x14), GetNumDirTable (0x4C -> retail 0x54), GetMaxDevices (2) (tools/layout_xlate.py: +0x4 from 0x10,
//       +0x8 from 0x38), and assert strings "mIconFileHandle != 0", "(file >= 0) && (file < mMaxFiles)",
//       "(int)strlen(tmp) < kMCTitleStringSize", "slot >=0 && slot<kMCMaxDevices",
//       "strlen(pszInfo) < sizeof(mSourceIconPath)", "(index >= 0) && (index < mNumDirTable)",
//       "mFileDescriptor >= 0";
//   [U] how retail code uses the field;
//   [X] refs/ssx3-rust names (guesses) for some of the methods.
// Names: [P] ones as tagged. Ours, from the [O] cMemCard methods (util/bxmemcard.h) that use them: mTitle (slots 14 /
// 45 GetTitleString / SetTitleString; [P] kMCTitleStringSize), mFileName (slot 67 HasFileName tests the slot's; slots
// 34 / 35 CreateFileSlot store the card's), mDirPattern (slot 6 GetDir's name), mFileSlots / mCurrentFileSlot (the
// records the fileSlot parameters index; [P] mMaxFiles of them), mBuffer / mTransferSize (slots 30 / 41 ReadData /
// WriteData), mDataSize (slot 19 SetDataSize), mReadFailed / mWriteFailed (slots 33 / 44), mFileInfoValid (slot 75
// IsFileInfoValid). Ours from the code's use: mFilePath (the "<name>/<name>" path for sceMcOpen / sceMcMkdir /
// sceMcDelete), mIconFilePath, mDirTable (the table [P] mNumDirTable counts), mFileDirEntry, mIconFileData /
// mIconFileSize (after [P] mIconFileHandle), mGetInfoPending / mGetInfoDevice, mScanFileSlot, mTitleOffsLF
// (sceMcIconSys offsLF), mModifyTime, mOpenMode / mSeekOffset / mSeekOrigin (the sceMcOpen / sceMcSeek arguments they
// hold). The rest stay unk_XXX.

#include "layout.h"

// One sceMcGetInfo record per memory-card port (kMCMaxDevices = 2). Names: mType / mFreeClusters / mFormat after
// libmc's sceMcGetInfo(port, slot, type, free, format); mLastError and mCardChanged after the [P] accessors that read them.
struct tPS2MemCardDevice {
    // mLastError codes the driver itself gives a meaning (names ours; the other sceMcSync results stay numbers). [U]
    // ClearCardChanged resets kMcResChangedCard to 0 and Update raises the card-changed flag for it, GetLastError
    // reports kMcResUnknown for any code it does not know, FileExistsError tests kMcResFileExists, and Update stores
    // kMcResNotPS2Card for a card whose type is not a PS2 card's (IsInvalid tests it).
    enum { kMcResChangedCard = -1, kMcResUnknown = -10, kMcResFileExists = -11, kMcResNotPS2Card = -10002 };

    int mType;                      // 0x00 [U] Update stores sceMcGetInfo's type (D_004A3938)
    int mFreeClusters;              // 0x04 [U] Update stores sceMcGetInfo's free clusters (D_004A393C);
                                    //       cPS2MemCard_GetFreeSpace1 returns it (slot 20 for the current device)
    int mLastError;                 // 0x08 [U] last sceMcSync result (D_004A3940); SetState(1) stores it; GetLastError
                                    //       cPS2MemCard_GetLastError ([P] preview GetLastError reads the same word) and
                                    //       slots 9, 48, 52, 74 classify it; slot 47 resets -1 to 0
    int mFormat;                    // 0x0C [U] sceMcGetInfo's format word (D_004A3944); slot 56 tests > 0
    int mCardChanged;               // 0x10 [U] card-changed flag: Update sets it, cPS2MemCard_CardChanged tests it ([P]
                                    //       preview CardChanged reads the same word), slot 47 clears it
};

CHECK_OFFSET(tPS2MemCardDevice, mType, 0x00);
CHECK_OFFSET(tPS2MemCardDevice, mFreeClusters, 0x04);
CHECK_OFFSET(tPS2MemCardDevice, mLastError, 0x08);
CHECK_OFFSET(tPS2MemCardDevice, mFormat, 0x0C);
CHECK_OFFSET(tPS2MemCardDevice, mCardChanged, 0x10);
CHECK_SIZE(tPS2MemCardDevice, 0x14);

// A memory-card date/time: libmc's sceMcStDateTime layout (field names follow it). [U] cPS2MemCard_GetLastSavedFile (slot 17)
// compares Min..Year of the file entries; Update copies the directory entry's modify time into a file entry with
// one 8-byte move (ldl/ldr at 0x488).
struct tPS2MemCardTime {
    unsigned char m_resv2;          // 0x0
    unsigned char m_sec;            // 0x1
    unsigned char m_min;            // 0x2
    unsigned char m_hour;           // 0x3
    unsigned char m_day;            // 0x4
    unsigned char m_month;          // 0x5
    unsigned short m_year;          // 0x6
};

CHECK_OFFSET(tPS2MemCardTime, m_resv2, 0x0);
CHECK_OFFSET(tPS2MemCardTime, m_sec, 0x1);
CHECK_OFFSET(tPS2MemCardTime, m_min, 0x2);
CHECK_OFFSET(tPS2MemCardTime, m_hour, 0x3);
CHECK_OFFSET(tPS2MemCardTime, m_day, 0x4);
CHECK_OFFSET(tPS2MemCardTime, m_month, 0x5);
CHECK_OFFSET(tPS2MemCardTime, m_year, 0x6);
CHECK_SIZE(tPS2MemCardTime, 0x8);

// One sceMcGetDir result: libmc's sceMcTblGetDir layout (0x40 bytes; field names follow it). [U] sceMcGetDir
// fills 0x480 (one entry) and 0x4C0 (the table); cPS2MemCard_GetDataSize (slot 18) returns 0x490 = m_fileSizeByte; Update's
// view reads the table's names at 0x4E0 + i * 0x40 and cPS2MemCard_getDirEntryName copies them (= m_entryName).
struct tPS2MemCardDirEntry {
    tPS2MemCardTime m_create;       // 0x00
    tPS2MemCardTime m_modify;       // 0x08 [U] copied into tPS2MemCardFile::mModifyTime by Update
    unsigned int m_fileSizeByte;    // 0x10 [U] slot 18; Update compares it with cPS2MemCard::mDataSize
    unsigned short m_attrFile;      // 0x14
    unsigned short m_reserve1;      // 0x16
    unsigned int m_reserve2;        // 0x18
    unsigned int m_pdaAplNo;        // 0x1C
    unsigned char m_entryName[32];  // 0x20
};

CHECK_OFFSET(tPS2MemCardDirEntry, m_create, 0x00);
CHECK_OFFSET(tPS2MemCardDirEntry, m_modify, 0x08);
CHECK_OFFSET(tPS2MemCardDirEntry, m_fileSizeByte, 0x10);
CHECK_OFFSET(tPS2MemCardDirEntry, m_attrFile, 0x14);
CHECK_OFFSET(tPS2MemCardDirEntry, m_reserve1, 0x16);
CHECK_OFFSET(tPS2MemCardDirEntry, m_reserve2, 0x18);
CHECK_OFFSET(tPS2MemCardDirEntry, m_pdaAplNo, 0x1C);
CHECK_OFFSET(tPS2MemCardDirEntry, m_entryName, 0x20);
CHECK_SIZE(tPS2MemCardDirEntry, 0x40);

// One save-file slot (mMaxFiles = 6 of them, 0x78 bytes each). Names ours, from the [O] cMemCard methods that
// read them (util/bxmemcard.h slots 40, 65-67).
struct tPS2MemCardFile {
    int mBroken;                    // 0x00 [O] slot 40 SetBrokenFile sets 1; slot 65 IsFileSlotBroken tests it (and
                                    //       mIconSysUnreadable); Update clears it
    int mIconSysUnreadable;         // 0x04 [U] cPS2MemCard_markIconSysUnreadable sets 1 when the slot's icon.sys fails to open or read
                                    //       (SetState 8 / 10); slot 65 tests it; Update clears it
    int mDisabled;                  // 0x08 [O] slot 66 IsFileSlotDisabled tests it; Update clears it
    char mFileName[0x40];           // 0x0C [U] the file (directory) name: slot 67 tests strlen > 0; SetState(7/0xB/0xC)
                                    //       and cPS2MemCard_issueGetFileInfo build "<name>/<name>" paths from it; Update copies
                                    //       cPS2MemCard::mFileName
    char mTitle[0x22];              // 0x4C [U] the save title: Update copies cPS2MemCard::mTitle into it and converts it
                                    //       (cMemCard_ShiftJISToAscii ShiftJISToAscii); cPS2MemCard_GetTitleString reads +0x52
    tPS2MemCardTime mModifyTime;    // 0x6E [U] the file's modify time (cPS2MemCard_GetLastSavedFile compares Min..Year)
    char pad_76[0x2];               // 0x76
};

CHECK_OFFSET(tPS2MemCardFile, mBroken, 0x00);
CHECK_OFFSET(tPS2MemCardFile, mIconSysUnreadable, 0x04);
CHECK_OFFSET(tPS2MemCardFile, mDisabled, 0x08);
CHECK_OFFSET(tPS2MemCardFile, mFileName, 0x0C);
CHECK_OFFSET(tPS2MemCardFile, mTitle, 0x4C);
CHECK_OFFSET(tPS2MemCardFile, mModifyTime, 0x6E);
CHECK_OFFSET(tPS2MemCardFile, pad_76, 0x76);
CHECK_SIZE(tPS2MemCardFile, 0x78);

struct cPS2MemCard {
    // The two mState values the driver's accessors name (names ours, from them; the other states stay numbers). [U]
    // IsWriteWaiting tests kStateWriteWaiting and IsReadWaiting kStateReadWaiting.
    enum { kStateWriteWaiting = 52, kStateReadWaiting = 53 };

    // --- cMemCard (base, no data) ---
    void** mVtbl;                   // 0x0000 [R] ctor stores cPS2MemCard_vtbl (cPS2MemCard); dtor stores cMemCard_vtbl (cMemCard)
    // --- cPS2MemCard ---
    int mState;                     // 0x0004 [P] GetState; [R] Reset 0; [U] SetState stores the new state; slots 31/42
                                    //       test 0x35/0x34
    int mTitleOffsLF;               // 0x0008 [R] Reset 0x1C; [U] the title's line-break offset: SetState(0x26) copies
                                    //       it (> 0) into the icon's offsLF (asm: lhu, a narrowing load)
    int mDevice;                    // 0x000C [P] GetDevice; [R] Reset 0; [U] the port of every sceMc* call; indexes
                                    //       mDevices; retail slot 4 cPS2MemCard_GetDevice
    int mGetInfoDevice;             // 0x0010 [R] Reset 0; [U] the port of the pending sceMcGetInfo: slot 5
                                    //       cPS2MemCard_CheckDevice and cPS2MemCard_selectDevice store it before SetState(3),
                                    //       Update files the results under mDevices[mGetInfoDevice]
    int mMaxFiles;                  // 0x0014 [P] GetMaxFiles, "(file >= 0) && (file < mMaxFiles)"; [R] Reset 6 (the
                                    //       mFileSlots count); retail slot 68 cPS2MemCard_GetMaxFiles
    int mFileDescriptor;            // 0x0018 [P] "mFileDescriptor >= 0"; [R] Reset -1; [U] sceMcOpen's result, passed
                                    //       to sceMcClose/Read/Write/Seek/Flush, -1 after close
    int mCurrentFileSlot;           // 0x001C [R] Reset -1; [U] the current file slot (slots 10/76 store it; indexes
                                    //       mFileSlots); SetState(1) sets -1
    int mTransferSize;              // 0x0020 [R] Reset 0; [U] transfer size for sceMcWrite/Read in SetState(0x17/0x1D)
                                    //       ([O] ReadData / WriteData transferSize; slots 30/36/37/41 store it);
                                    //       cPS2MemCard_startMakeDir stores it
    int mDataSize;                  // 0x0024 [R] Reset 0; [O] slot 19 SetDataSize stores it; Update compares it with
                                    //       the dir entry's m_fileSizeByte; cPS2MemCard_startReadFile/6BD0 store the
                                    //       sceMcRead/Write size
    int mScanFileSlot;              // 0x0028 [R] Reset 0; [U] the file slot that SetState(7) .. SetState(0xF) check
                                    //       (icon.sys, directory entry); state 7 loops it over 0..mMaxFiles
    int mBuffer;                    // 0x002C [R] Reset 0; [O] ReadData / WriteData buffer: the caller's buffer for
                                    //       sceMcWrite/Read (slots 30/36/37/41, cPS2MemCard_startReadFile/6BD0 store it);
                                    //       SetState(1/0x17) clears it
    void* mIconFileData;            // 0x0030 [R] Reset 0; [U] the icon file's data (ASYNCFILE_release(mIconFileHandle,
                                    //       &mIconFileData, &mIconFileSize); [O] its address out-parameter is void**),
                                    //       written with sceMcWrite, MEM_free'd; dtor frees it
    int mIconFileSize;              // 0x0034 [R] Reset 0; [U] the icon file's size (see mIconFileData)
    int mIconFileHandle;            // 0x0038 [P] "mIconFileHandle != 0"; [R] Reset -1; [U] SetState(0x28/0x2C/0x30) =
                                    //       ASYNCFILE_load(icon path, 0x100) (async open); Update polls and releases it
    int mFileInfoValid;             // 0x003C [R] Reset 0; [U] Update sets 0/1 (operation outcome); cPS2MemCard_issueGetFileInfo,
                                    //       cPS2MemCard_startDeleteSave, SetState(0xB) clear it; [O] slot 75 IsFileInfoValid (retail
                                    //       cPS2MemCard_IsFileInfoValid) returns it
    int mGetInfoPending;            // 0x0040 [R] Reset 0; [U] whether cPS2MemCard_startGetInfo's sceMcGetInfo was accepted; slot
                                    //       11 (busy test), cPS2MemCard_startGetInfo, cPS2MemCard_CleanUp
    int mReadFailed;                // 0x0044 [R] Reset 0; [O] slot 33 SetReadFailed sets 1 (deferred: Update then
                                    //       SetState(0x1E)); SetState(0x1C) clears it
    int mWriteFailed;               // 0x0048 [R] Reset 0; [O] slot 44 SetWriteFailed sets 1 (deferred: Update then
                                    //       SetState(0x18)); SetState(0x24) clears it
    int unk_04C;                    // 0x004C [R] Reset 0; [U] cPS2MemCard_startGetInfo stores 5
    int unk_050;                    // 0x0050 [R] Reset 0; [U] cPS2MemCard_clearDirData clears it; Update stores a count and loops
                                    //       over it
    int mNumDirTable;               // 0x0054 [P] GetNumDirTable, "(index >= 0) && (index < mNumDirTable)"; [R] Reset 0;
                                    //       [U] cPS2MemCard_clearDirData / cPS2MemCard_startListDir store it; Update stores it
    char mTitle[0x21];              // 0x0058 [U] the save title ([P] kMCTitleStringSize likely 0x21): SetState(0x26)
                                    //       converts it into the icon's title (cMemCard_AsciiToShiftJIS AsciiToShiftJIS); slot 45
                                    //       cPS2MemCard_SetTitleString fills it; Update copies it into the file slot
    char mIconFilePath[0x40];       // 0x0079 [R] Reset clears it; [U] the icon file path "<name>/<icon file>" (sprintf
                                    //       target before sceMcOpen / sceMcDelete of the icon files)
    char mFileName[0x40];           // 0x00B9 [R] Reset clears it; [U] the file name (slots 34-37, cPS2MemCard_startLoadFile)
    char mFilePath[0x40];           // 0x00F9 [R] Reset clears it; [U] the file path "<name>/<name>" (sprintf
                                    //       str_pct_s_pct_s_h) for sceMcOpen/Mkdir/Delete
    char mDirPattern[0x40];         // 0x0139 [R] Reset clears it; [U] the sceMcGetDir pattern (slot 6 cPS2MemCard_GetDir)
    char pad_179[0x3];              // 0x0179
    tPS2MemCardDevice mDevices[2];  // 0x017C [R] Reset clears both; [P] kMCMaxDevices (GetMaxDevices returns 2); [U]
                                    //       indexed by mDevice
    tPS2MemCardFile mFileSlots[6];  // 0x01A4 [R] Reset clears 0x2D0 bytes; [U] indexed by mCurrentFileSlot /
                                    //       mScanFileSlot (0x78 stride), count mMaxFiles
    char pad_474[0xC];              // 0x0474 never accessed
    tPS2MemCardDirEntry mFileDirEntry; // 0x0480 [R] Reset clears it; [U] sceMcGetDir with maxent 1 (cPS2MemCard_issueGetFileInfo,
                                    //       SetState(0xB/0xC)); slot 18 returns +0x10
    tPS2MemCardDirEntry mDirTable[64]; // 0x04C0 [R] Reset clears 0x1000; [U] the directory table (cPS2MemCard_issueGetDir,
                                    //       cPS2MemCard_issueListDir sceMcGetDir; cPS2MemCard_clearDirData clears it); mNumDirTable entries
    char mSourceIconPath[0x20];     // 0x14C0 [P] "strlen(pszInfo) < sizeof(mSourceIconPath)" (0x20 fits the gap);
                                    //       [R] Reset clears it; [U] slot 39 cPS2MemCard_SetFileSlotInfo copies it; SetState(0x28/0x2C/
                                    //       0x30) builds the icon file paths from it
    int mOpenMode;                  // 0x14E0 [R] Reset 0; [U] sceMcOpen mode (cPS2MemCard_startOpenFile stores, cPS2MemCard_issueOpenFile uses)
    int mSeekOffset;                // 0x14E4 [R] Reset 0; [U] sceMcSeek offset (cPS2MemCard_startSeekFile stores, cPS2MemCard_issueSeekFile)
    int mSeekOrigin;                // 0x14E8 [R] Reset 0; [U] sceMcSeek origin (see mSeekOffset)
    char pad_14EC[0x14];            // 0x14EC never accessed (allocation size 0x1500)

    // Methods bound to their retail symbols by asm labels.
    int GetDevice() __asm__("cPS2MemCard_GetDevice");
    void IsInvalid1() __asm__("cPS2MemCard_IsInvalid1");
    void IsAvailable1() __asm__("cPS2MemCard_IsAvailable1");
    int IsActionable(int x) __asm__("cPS2MemCard_IsActionable");
    void IsFormatted1() __asm__("cPS2MemCard_IsFormatted1");
    void CardChanged1() __asm__("cPS2MemCard_CardChanged1");
    void GetStatus1(unsigned short* outText) __asm__("cPS2MemCard_GetStatus1");
    int GetMaxFiles() __asm__("cPS2MemCard_GetMaxFiles");
    void GetDeviceNameRaw(int device, unsigned short* dst, int dstLen) __asm__("cPS2MemCard_GetDeviceNameRaw");
    int IsFileInfoValid() __asm__("cPS2MemCard_IsFileInfoValid");
    void GetLastError1() __asm__("cPS2MemCard_GetLastError1");
    int GetState() __asm__("cPS2MemCard_GetState");
    int GetNumDirTable() __asm__("cPS2MemCard_GetNumDirTable");
    void GetDir1(unsigned short* name) __asm__("cPS2MemCard_GetDir1");
    void GetDir(const char* name) __asm__("cPS2MemCard_GetDir");
    void* DeleteFileSlot(int fileSlot) __asm__("cPS2MemCard_DeleteFileSlot__FPvi");
    int GetDataSize() __asm__("cPS2MemCard_GetDataSize__FPv");
    void SetDataSize(int val) __asm__("cPS2MemCard_SetDataSize__FPvi");
    void CheckDevice(int device) __asm__("cPS2MemCard_CheckDevice");
    void ClearCardChanged() __asm__("cPS2MemCard_ClearCardChanged");
    void CreateFileSlot(const char* name) __asm__("cPS2MemCard_CreateFileSlot");
    void CreateFileSlot1(unsigned short* name) __asm__("cPS2MemCard_CreateFileSlot1");
    void StartFileSlotWrite1(unsigned short* wname, int writeBuffer, int writeSize)
        __asm__("cPS2MemCard_StartFileSlotWrite1");
    void StartFileSlotWrite(const char* src, int writeBuffer, int writeSize) __asm__("cPS2MemCard_StartFileSlotWrite");
    void StartFileSlotWrite2(int fileSlot, int writeBuffer, int writeSize) __asm__("cPS2MemCard_StartFileSlotWrite2");
    void* WriteData(int buffer, int transferSize) __asm__("cPS2MemCard_WriteData__FPvii");
    bool IsWriteWaiting() __asm__("cPS2MemCard_IsWriteWaiting");
    void SetWriteFailed() __asm__("cPS2MemCard_SetWriteFailed__FPv");
    void StartFileSlotRead(int fileSlot, int buffer, int transferSize) __asm__("cPS2MemCard_StartFileSlotRead");
    void* ReadData(int buffer, int transferSize) __asm__("cPS2MemCard_ReadData__FPvii");
    bool IsReadWaiting() __asm__("cPS2MemCard_IsReadWaiting");
    void SetReadFailed() __asm__("cPS2MemCard_SetReadFailed__FPv");
    bool IsBusy() __asm__("cPS2MemCard_IsBusy");
    bool DidFormatFail() __asm__("cPS2MemCard_DidFormatFail");
    bool DidErrorOccur() __asm__("cPS2MemCard_DidErrorOccur__FPv");
    bool IsInvalid(int device) __asm__("cPS2MemCard_IsInvalid");
    int GetFreeSpace1(int device) __asm__("cPS2MemCard_GetFreeSpace1");
    int GetFreeSpace() __asm__("cPS2MemCard_GetFreeSpace__FPv");
    bool IsAvailable(int i) __asm__("cPS2MemCard_IsAvailable");
    bool IsFormatted(int device) __asm__("cPS2MemCard_IsFormatted");
    bool CardChanged(int device) __asm__("cPS2MemCard_CardChanged");
    void GetStatus(int idx, unsigned short* dst) __asm__("cPS2MemCard_GetStatus");
    void* SetTitleString() __asm__("cPS2MemCard_SetTitleString__FPv");
    bool HasFileName(int fileSlot) __asm__("cPS2MemCard_HasFileName");
    void SetBrokenFile(int fileSlot) __asm__("cPS2MemCard_SetBrokenFile");
    bool IsFileSlotBroken(int fileSlot) __asm__("cPS2MemCard_IsFileSlotBroken");
    bool IsFileSlotDisabled(int fileSlot) __asm__("cPS2MemCard_IsFileSlotDisabled");
    void GetTitleString(int fileSlot, unsigned short* dst) __asm__("cPS2MemCard_GetTitleString");
    void CleanUp() __asm__("cPS2MemCard_CleanUp");
    void GetDeviceName(int device, unsigned short* dst, int dstLen) __asm__("cPS2MemCard_GetDeviceName");
    void GetFileDate(int fileSlot, unsigned short* dst) __asm__("cPS2MemCard_GetFileDate");
    void GetFileTime(int fileSlot, unsigned short* dst) __asm__("cPS2MemCard_GetFileTime");
    int GetLastSavedFile() __asm__("cPS2MemCard_GetLastSavedFile");
    void SetFileSlotInfo(const char* name, int flag) __asm__("cPS2MemCard_SetFileSlotInfo");
    bool FileExistsError() __asm__("cPS2MemCard_FileExistsError");
    void IsRemovable() __asm__("cPS2MemCard_IsRemovable");
    int SpaceRequired(int byteSize) __asm__("cPS2MemCard_SpaceRequired");
    void SpaceRequired1() __asm__("cPS2MemCard_SpaceRequired1");
    void IsDamaged() __asm__("cPS2MemCard_IsDamaged");
    void* Format() __asm__("cPS2MemCard_Format__FPv");
    void* SetWriteClose() __asm__("cPS2MemCard_SetWriteClose__FPv");
    void* SetReadClose() __asm__("cPS2MemCard_SetReadClose__FPv");
    int GetBlockSize() __asm__("cPS2MemCard_GetBlockSize__FPv");
    int GetTotalSpace() __asm__("cPS2MemCard_GetTotalSpace__FPv");
    int GetFreeDirEntries() __asm__("cPS2MemCard_GetFreeDirEntries__FPv");
    void Init() __asm__("cPS2MemCard_Init");
    int GetLastError(int i) __asm__("cPS2MemCard_GetLastError");
    void Update() __asm__("cPS2MemCard_Update");
};

CHECK_OFFSET(cPS2MemCard, mVtbl, 0x0000);
CHECK_OFFSET(cPS2MemCard, mState, 0x0004);
CHECK_OFFSET(cPS2MemCard, mTitleOffsLF, 0x0008);
CHECK_OFFSET(cPS2MemCard, mDevice, 0x000C);
CHECK_OFFSET(cPS2MemCard, mGetInfoDevice, 0x0010);
CHECK_OFFSET(cPS2MemCard, mMaxFiles, 0x0014);
CHECK_OFFSET(cPS2MemCard, mFileDescriptor, 0x0018);
CHECK_OFFSET(cPS2MemCard, mCurrentFileSlot, 0x001C);
CHECK_OFFSET(cPS2MemCard, mTransferSize, 0x0020);
CHECK_OFFSET(cPS2MemCard, mDataSize, 0x0024);
CHECK_OFFSET(cPS2MemCard, mScanFileSlot, 0x0028);
CHECK_OFFSET(cPS2MemCard, mBuffer, 0x002C);
CHECK_OFFSET(cPS2MemCard, mIconFileData, 0x0030);
CHECK_OFFSET(cPS2MemCard, mIconFileSize, 0x0034);
CHECK_OFFSET(cPS2MemCard, mIconFileHandle, 0x0038);
CHECK_OFFSET(cPS2MemCard, mFileInfoValid, 0x003C);
CHECK_OFFSET(cPS2MemCard, mGetInfoPending, 0x0040);
CHECK_OFFSET(cPS2MemCard, mReadFailed, 0x0044);
CHECK_OFFSET(cPS2MemCard, mWriteFailed, 0x0048);
CHECK_OFFSET(cPS2MemCard, unk_04C, 0x004C);
CHECK_OFFSET(cPS2MemCard, unk_050, 0x0050);
CHECK_OFFSET(cPS2MemCard, mNumDirTable, 0x0054);
CHECK_OFFSET(cPS2MemCard, mTitle, 0x0058);
CHECK_OFFSET(cPS2MemCard, mIconFilePath, 0x0079);
CHECK_OFFSET(cPS2MemCard, mFileName, 0x00B9);
CHECK_OFFSET(cPS2MemCard, mFilePath, 0x00F9);
CHECK_OFFSET(cPS2MemCard, mDirPattern, 0x0139);
CHECK_OFFSET(cPS2MemCard, pad_179, 0x0179);
CHECK_OFFSET(cPS2MemCard, mDevices, 0x017C);
CHECK_OFFSET(cPS2MemCard, mFileSlots, 0x01A4);
CHECK_OFFSET(cPS2MemCard, pad_474, 0x0474);
CHECK_OFFSET(cPS2MemCard, mFileDirEntry, 0x0480);
CHECK_OFFSET(cPS2MemCard, mDirTable, 0x04C0);
CHECK_OFFSET(cPS2MemCard, mSourceIconPath, 0x14C0);
CHECK_OFFSET(cPS2MemCard, mOpenMode, 0x14E0);
CHECK_OFFSET(cPS2MemCard, mSeekOffset, 0x14E4);
CHECK_OFFSET(cPS2MemCard, mSeekOrigin, 0x14E8);
CHECK_OFFSET(cPS2MemCard, pad_14EC, 0x14EC);
CHECK_SIZE(cPS2MemCard, 0x1500);

#endif /* UTIL_BXPS2MEMCARD_H */
