#ifndef UTIL_BXPS2MEMCARD_H
#define UTIL_BXPS2MEMCARD_H

// cPS2MemCard: the PS2 memory-card driver behind the abstract cMemCard interface (util/bxmemcard.cpp:
// cMemCard::CreateInstance func_002C22C8 = new cPS2MemCard, DeleteInstance cMemCard_DeleteInstance). It wraps libmc as an
// asynchronous state machine: the public calls store their arguments and call SetState (func_002C48C0), which
// issues the sceMc* request; Update (vtable slot 3, cPS2MemCard_Update) polls sceMcSync and moves to the next state.
// Holders: D_004A33D8 (dirtysock/tagsunk.cpp creates and deletes it) and the movie player's object + 0x434
// (movie/movieplayer.cpp).
//
// Size 0x1500 [R]: func_002C22C8 cMemMan_alloc(0x1500, str_cPS2MemCard, 0x100, 0) then the ctor cPS2MemCard_cPS2MemCard.
// Base: cMemCard (On Tour DWARF: size 0x4, no data, so only its vptr; the dtor cPS2MemCard__cPS2MemCard stores cMemCard's
// vtable D_004871E8 back before the base dtor runs). cPS2MemCard's vtable cPS2MemCard_vtbl (76 slots; 59 slot
// functions in this unit, 17 inline accessors emitted in seg_1D3510 at 0x002D37B8-0x002D39B0). EA's source:
// d:/ssx/code/source/util/bxps2memcard.cpp (preview asserts below); On Tour has no cPS2MemCard.
// Methods: see methods.txt (in retail they sit in the worldpainter/quadtree split, 0x002C3FA0-0x002C6D70).
//
// The libmc calls are identified by argument shape and address order (inferred, not symbol-backed):
//   func_00409D10 sceMcOpen, func_00409E38 sceMcMkdir, func_00409E70 sceMcClose, func_00409F28 sceMcSeek,
//   func_0040A090 sceMcRead, func_0040A1A8 sceMcWrite, func_0040A360 sceMcSync, func_0040A498 sceMcGetInfo,
//   func_0040A688 sceMcGetDir, func_0040A998 sceMcFormat, func_0040AA68 sceMcDelete, func_0040AB80 sceMcFlush.
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
// Field names without a [P] name are unk_XXX.

#include "layout.h"

// One sceMcGetInfo record per memory-card port (kMCMaxDevices = 2). Names ours.
struct tPS2MemCardDevice {
    int unk_00;                     // 0x00 [U] Update stores sceMcGetInfo's type (D_004A3938)
    int unk_04;                     // 0x04 [U] Update stores sceMcGetInfo's free clusters (D_004A393C);
                                    //       cPS2MemCard_GetFreeSpace1 returns it (slot 20 for the current device)
    int unk_08;                     // 0x08 [U] last sceMcSync result (D_004A3940); SetState(1) stores it;
                                    //       GetLastError func_002C51D0 ([P] preview GetLastError reads the same
                                    //       word) and slots 9, 48, 52, 74 classify it; slot 47 resets -1 to 0
    int unk_0C;                     // 0x0C [U] sceMcGetInfo's format word (D_004A3944); slot 56 tests > 0
    int unk_10;                     // 0x10 [U] card-changed flag: Update sets it, cPS2MemCard_CardChanged tests it ([P] preview
                                    //       CardChanged reads the same word), slot 47 clears it
};

CHECK_OFFSET(tPS2MemCardDevice, unk_00, 0x00);
CHECK_OFFSET(tPS2MemCardDevice, unk_04, 0x04);
CHECK_OFFSET(tPS2MemCardDevice, unk_08, 0x08);
CHECK_OFFSET(tPS2MemCardDevice, unk_0C, 0x0C);
CHECK_OFFSET(tPS2MemCardDevice, unk_10, 0x10);
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
// view reads the table's names at 0x4E0 + i * 0x40 and func_002C68C8 copies them (= m_entryName).
struct tPS2MemCardDirEntry {
    tPS2MemCardTime m_create;       // 0x00
    tPS2MemCardTime m_modify;       // 0x08 [U] copied into tPS2MemCardFile::unk_6E by Update
    unsigned int m_fileSizeByte;    // 0x10 [U] slot 18; Update compares it with unk_024
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

// One save-file slot (mMaxFiles = 6 of them, 0x78 bytes each). Names ours.
struct tPS2MemCardFile {
    int unk_00;                     // 0x00 [U] slot 40 sets 1; slot 65 (empty test) needs it and unk_04 0; Update
                                    //       clears it
    int unk_04;                     // 0x04 [U] slot 65; Update clears it
    int unk_08;                     // 0x08 [U] slot 66 tests > 0; Update clears it
    char unk_0C[0x40];              // 0x0C [U] the file (directory) name: slot 67 tests strlen > 0; SetState(7/0xB/0xC)
                                    //       and func_002C42B0 build "<name>/<name>" paths from it; Update copies unk_0B9
    char unk_4C[0x22];              // 0x4C [U] the save title: Update copies unk_058 into it and converts it
                                    //       (func_002C2268 ShiftJISToAscii); cPS2MemCard_GetTitleString reads +0x52
    tPS2MemCardTime unk_6E;         // 0x6E [U] the file's modify time (cPS2MemCard_GetLastSavedFile compares Min..Year)
    char pad_76[0x2];               // 0x76
};

CHECK_OFFSET(tPS2MemCardFile, unk_00, 0x00);
CHECK_OFFSET(tPS2MemCardFile, unk_04, 0x04);
CHECK_OFFSET(tPS2MemCardFile, unk_08, 0x08);
CHECK_OFFSET(tPS2MemCardFile, unk_0C, 0x0C);
CHECK_OFFSET(tPS2MemCardFile, unk_4C, 0x4C);
CHECK_OFFSET(tPS2MemCardFile, unk_6E, 0x6E);
CHECK_OFFSET(tPS2MemCardFile, pad_76, 0x76);
CHECK_SIZE(tPS2MemCardFile, 0x78);

struct cPS2MemCard {
    // --- cMemCard (base, no data) ---
    void** mVtbl;                   // 0x0000 [R] ctor stores cPS2MemCard_vtbl (cPS2MemCard); dtor stores D_004871E8 (cMemCard)
    // --- cPS2MemCard ---
    int mState;                     // 0x0004 [P] GetState; [R] Reset 0; [U] SetState stores the new state; slots 31/42
                                    //       test 0x35/0x34
    int unk_008;                    // 0x0008 [R] Reset 0x1C; [U] SetState(0x26) copies it (> 0) into the icon's
                                    //       offsLF (asm: lhu, a narrowing load)
    int mDevice;                    // 0x000C [P] GetDevice; [R] Reset 0; [U] the port of every sceMc* call; indexes
                                    //       unk_17C; retail slot 4 cPS2MemCard_GetDevice
    int unk_010;                    // 0x0010 [R] Reset 0; [U] slot 5 cPS2MemCard_CheckDevice and func_002C67E8 store the device
                                    //       before SetState(3)
    int mMaxFiles;                  // 0x0014 [P] GetMaxFiles, "(file >= 0) && (file < mMaxFiles)"; [R] Reset 6 (the
                                    //       unk_1A4 count); retail slot 68 func_002D3970
    int mFileDescriptor;            // 0x0018 [P] "mFileDescriptor >= 0"; [R] Reset -1; [U] sceMcOpen's result, passed
                                    //       to sceMcClose/Read/Write/Seek/Flush, -1 after close
    int unk_01C;                    // 0x001C [R] Reset -1; [U] the current file slot (slots 10/76 store it; indexes
                                    //       unk_1A4); SetState(1) sets -1
    int unk_020;                    // 0x0020 [R] Reset 0; [U] transfer size for sceMcWrite/Read in SetState(0x17/0x1D)
                                    //       (slots 30/36/37/41 store it); func_002C6C50 stores it
    int unk_024;                    // 0x0024 [R] Reset 0; [U] slot 19 stores it; Update compares it with the dir
                                    //       entry's m_fileSizeByte; func_002C6B50/6BD0 store the sceMcRead/Write size
    int unk_028;                    // 0x0028 [R] Reset 0; [U] SetState(7)'s file-slot loop index (0..mMaxFiles)
    int unk_02C;                    // 0x002C [R] Reset 0; [U] the caller's buffer for sceMcWrite/Read (slots 30/36/37/
                                    //       41, func_002C6B50/6BD0 store it); SetState(1/0x17) clears it
    int unk_030;                    // 0x0030 [R] Reset 0; [U] the icon file's data (ASYNCFILE_release(mIconFileHandle,
                                    //       &unk_030, &unk_034)), written with sceMcWrite, MEM_free'd; dtor frees it
    int unk_034;                    // 0x0034 [R] Reset 0; [U] the icon file's size (see unk_030)
    int mIconFileHandle;            // 0x0038 [P] "mIconFileHandle != 0"; [R] Reset -1; [U] SetState(0x28/0x2C/0x30) =
                                    //       ASYNCFILE_load(icon path, 0x100) (async open); Update polls and releases it
    int unk_03C;                    // 0x003C [R] Reset 0; [U] Update sets 0/1 (operation outcome); func_002C42B0,
                                    //       func_002C4410, SetState(0xB) clear it; retail slot 75 func_002D39B0 returns it
    int unk_040;                    // 0x0040 [R] Reset 0; [U] nonzero while a sceMcSync is pending ([X] Rust); slot 11
                                    //       (busy test), func_002C50E0, cPS2MemCard_CleanUp
    int unk_044;                    // 0x0044 [R] Reset 0; [U] slot 33 sets 1 (deferred: Update then SetState(0x1E));
                                    //       SetState(0x1C) clears it
    int unk_048;                    // 0x0048 [R] Reset 0; [U] slot 44 sets 1 (deferred: Update then SetState(0x18));
                                    //       SetState(0x24) clears it
    int unk_04C;                    // 0x004C [R] Reset 0; [U] func_002C50E0 stores 5
    int unk_050;                    // 0x0050 [R] Reset 0; [U] func_002C5278 clears it; Update stores a count and loops
                                    //       over it
    int mNumDirTable;               // 0x0054 [P] GetNumDirTable, "(index >= 0) && (index < mNumDirTable)"; [R] Reset 0;
                                    //       [U] func_002C5278 / func_002C6810 store it; Update stores it
    char unk_058[0x21];             // 0x0058 [U] the save title ([P] kMCTitleStringSize likely 0x21): SetState(0x26)
                                    //       converts it into the icon's title (func_002C2210 AsciiToShiftJIS); slot 45
                                    //       func_002C5500 fills it; Update copies it into the file slot
    char unk_079[0x40];             // 0x0079 [R] Reset clears it; [U] scratch path (sprintf target before sceMcOpen /
                                    //       sceMcDelete of the icon files)
    char unk_0B9[0x40];             // 0x00B9 [R] Reset clears it; [U] the file name (slots 34-37, func_002C4788)
    char unk_0F9[0x40];             // 0x00F9 [R] Reset clears it; [U] the file path "<name>/<name>" (sprintf
                                    //       str_pct_s_pct_s_h) for sceMcOpen/Mkdir/Delete
    char unk_139[0x40];             // 0x0139 [R] Reset clears it; [U] the sceMcGetDir pattern (slot 6 cPS2MemCard_GetDir)
    char pad_179[0x3];              // 0x0179
    tPS2MemCardDevice unk_17C[2];   // 0x017C [R] Reset clears both; [P] kMCMaxDevices (GetMaxDevices returns 2);
                                    //       [U] indexed by mDevice
    tPS2MemCardFile unk_1A4[6];     // 0x01A4 [R] Reset clears 0x2D0 bytes; [U] indexed by unk_01C / unk_028 (0x78
                                    //       stride), count mMaxFiles
    char pad_474[0xC];              // 0x0474 never accessed
    tPS2MemCardDirEntry unk_480;    // 0x0480 [R] Reset clears it; [U] sceMcGetDir with maxent 1 (func_002C42B0,
                                    //       SetState(0xB/0xC)); slot 18 returns +0x10
    tPS2MemCardDirEntry unk_4C0[64]; // 0x04C0 [R] Reset clears 0x1000; [U] the directory table (func_002C4210,
                                    //       func_002C6848 sceMcGetDir; func_002C5278 clears it); mNumDirTable entries
    char mSourceIconPath[0x20];     // 0x14C0 [P] "strlen(pszInfo) < sizeof(mSourceIconPath)" (0x20 fits the gap);
                                    //       [R] Reset clears it; [U] slot 39 cPS2MemCard_SetFileSlotInfo copies it; SetState(0x28/0x2C/
                                    //       0x30) builds the icon file paths from it
    int unk_14E0;                   // 0x14E0 [R] Reset 0; [U] sceMcOpen mode (func_002C6968 stores, func_002C69A0 uses)
    int unk_14E4;                   // 0x14E4 [R] Reset 0; [U] sceMcSeek offset (func_002C6A60 stores, func_002C6A88)
    int unk_14E8;                   // 0x14E8 [R] Reset 0; [U] sceMcSeek origin (see unk_14E4)
    char pad_14EC[0x14];            // 0x14EC never accessed (allocation size 0x1500)
};

CHECK_OFFSET(cPS2MemCard, mVtbl, 0x0000);
CHECK_OFFSET(cPS2MemCard, mState, 0x0004);
CHECK_OFFSET(cPS2MemCard, unk_008, 0x0008);
CHECK_OFFSET(cPS2MemCard, mDevice, 0x000C);
CHECK_OFFSET(cPS2MemCard, unk_010, 0x0010);
CHECK_OFFSET(cPS2MemCard, mMaxFiles, 0x0014);
CHECK_OFFSET(cPS2MemCard, mFileDescriptor, 0x0018);
CHECK_OFFSET(cPS2MemCard, unk_01C, 0x001C);
CHECK_OFFSET(cPS2MemCard, unk_020, 0x0020);
CHECK_OFFSET(cPS2MemCard, unk_024, 0x0024);
CHECK_OFFSET(cPS2MemCard, unk_028, 0x0028);
CHECK_OFFSET(cPS2MemCard, unk_02C, 0x002C);
CHECK_OFFSET(cPS2MemCard, unk_030, 0x0030);
CHECK_OFFSET(cPS2MemCard, unk_034, 0x0034);
CHECK_OFFSET(cPS2MemCard, mIconFileHandle, 0x0038);
CHECK_OFFSET(cPS2MemCard, unk_03C, 0x003C);
CHECK_OFFSET(cPS2MemCard, unk_040, 0x0040);
CHECK_OFFSET(cPS2MemCard, unk_044, 0x0044);
CHECK_OFFSET(cPS2MemCard, unk_048, 0x0048);
CHECK_OFFSET(cPS2MemCard, unk_04C, 0x004C);
CHECK_OFFSET(cPS2MemCard, unk_050, 0x0050);
CHECK_OFFSET(cPS2MemCard, mNumDirTable, 0x0054);
CHECK_OFFSET(cPS2MemCard, unk_058, 0x0058);
CHECK_OFFSET(cPS2MemCard, unk_079, 0x0079);
CHECK_OFFSET(cPS2MemCard, unk_0B9, 0x00B9);
CHECK_OFFSET(cPS2MemCard, unk_0F9, 0x00F9);
CHECK_OFFSET(cPS2MemCard, unk_139, 0x0139);
CHECK_OFFSET(cPS2MemCard, pad_179, 0x0179);
CHECK_OFFSET(cPS2MemCard, unk_17C, 0x017C);
CHECK_OFFSET(cPS2MemCard, unk_1A4, 0x01A4);
CHECK_OFFSET(cPS2MemCard, pad_474, 0x0474);
CHECK_OFFSET(cPS2MemCard, unk_480, 0x0480);
CHECK_OFFSET(cPS2MemCard, unk_4C0, 0x04C0);
CHECK_OFFSET(cPS2MemCard, mSourceIconPath, 0x14C0);
CHECK_OFFSET(cPS2MemCard, unk_14E0, 0x14E0);
CHECK_OFFSET(cPS2MemCard, unk_14E4, 0x14E4);
CHECK_OFFSET(cPS2MemCard, unk_14E8, 0x14E8);
CHECK_OFFSET(cPS2MemCard, pad_14EC, 0x14EC);
CHECK_SIZE(cPS2MemCard, 0x1500);

#endif /* UTIL_BXPS2MEMCARD_H */
