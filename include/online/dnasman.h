#ifndef ONLINE_DNASMAN_H
#define ONLINE_DNASMAN_H

// DNASMan: SSX 3's DNAS (Dynamic Network Authentication System) check: loads the DNAS program (OVERLAY.DAT, linked
// with func_003FCFD8, entry points DirtyDnasRelInit / Updt / Exit) and its data (CONFIG.DAT, XOR-decoded), runs the
// check and keeps its result texts. cFEStateAuthDNAS owns one as mpAuth (include/fe/festateauthdnas.h: built with
// DNASMan_DNASMan on cMemMan_alloc(0xA50, "cFEStateAuthDNASCreate")); the debug menu's func_002560B0 runs one on
// cMemMan_alloc(2640, "DNASMAN"). Retail code: unit seg/seg_156860, DNASMan_DNASMan .. func_002565E0 (C linkage
// definitions in a .cpp). EA source: d:/ssx/code/source/online/dnasman.cpp (July 2003 preview).
//
// Name: DNASMan, from the preview's source file d:/ssx/code/source/online/dnasman.cpp, whose asserts name the members
// State (kDNAS_LoadingElf / kDNAS_LoadingAuth / kDNAS_Checking), ElfData and AuthData that retail's functions use the
// same way, and retail's allocation tag "DNASMAN" on the 2640-byte object; spelled like its online/ siblings LobbyMan
// and BuddyMan (lobbyman.cpp / buddyman.cpp, tags "LobbyMan" / "BuddyMan"). No vtable (the destructor DNASMan__DNASMan is
// called directly). Member names: State / ElfData / AuthData are EA's ([P]); the others are ours, in the same style.
//
// FLAT layout. Evidence tags: [R] the constructor's clear func_00256228; [P] the preview's asserts; [U] retail use.

#include "layout.h"

// The 8-byte key the DNAS module's parameter block carries (name ours): [U] func_002562D0 copies D_004A2ED8 into
// DNASMan::unk_074 as one two-word value (an unaligned 64-bit copy, ldl / ldr / sdl / sdr).
struct tDNASKey {
    int word[2];
};

// The 4-byte XOR key "CONFIG.DAT" is stored with (name ours): [U] func_00256510 copies D_004A2EE8 as one word and
// XORs byte i of the loaded file with b[i % 4].
struct tDNASFileKey {
    unsigned char b[4];
};

// The 908-byte result block the DNAS module fills (name ours): [U] func_002565E0 clears it (MEM_fill), passes it to
// func_003FD128 and copies three wide strings out of it into DNASMan's ErrorText1..3 (USTR_copy). The strings' lengths
// are only bounded by the next string's start.
struct tDNASResult {
    char unk_000[0x8];              // 0x000 no access seen
    unsigned short Text1[0x20];     // 0x008 [U] func_002565E0: copied to ErrorText1
    unsigned short Text2[0x80];     // 0x048 [U] func_002565E0: copied to ErrorText2
    unsigned short Text3[0x122];    // 0x148 [U] func_002565E0: copied to ErrorText3 (to the end of the block)
};

CHECK_OFFSET(tDNASResult, Text1, 0x008);
CHECK_OFFSET(tDNASResult, Text2, 0x048);
CHECK_OFFSET(tDNASResult, Text3, 0x148);
CHECK_SIZE(tDNASResult, 0x38C);

struct DNASMan {
    int State;                          // 0x000 [P] (asserts on kDNAS_LoadingElf / kDNAS_LoadingAuth / kDNAS_Checking);
                                        //       [R] 0; [U] func_00256258 1 (loading the program), func_00256460 2
                                        //       (loading the data), func_00256510 3 (loaded), func_002562D0 4
                                        //       (checking), func_002565E0 5 (done); func_002563F8 dispatches on it,
                                        //       cFEStateAuthDNAS_onUpdate polls it
    char* ElfData;                      // 0x004 [P] assert "!ElfData"; [R] 0; [U] func_00256258 allocates the
                                        //       0x195000-byte "ElfData" buffer and loads OVERLAY.DAT into it;
                                        //       func_002563A8 frees it
    int ElfSize;                        // 0x008 [R] 0; [U] func_00256460: the loaded size (ASYNCFILE_release)
    char* AuthData;                     // 0x00C [P] assert "!AuthData"; [R] 0; [U] func_00256460 allocates the 64 KB
                                        //       "AuthData" buffer and loads CONFIG.DAT into it; func_00256510 decodes
                                        //       it; func_002562D0 passes it on; func_002563A8 frees it
    int AuthSize;                       // 0x010 [R] 0; [U] func_00256510: the loaded size; func_002562D0 passes it on
    int CancelRequest;                  // 0x014 (name ours) [R] 0; [U] func_00256258 0; cFEStateAuthDNAS_onInputBegin
                                        //        sets 1 when the player cancels (next / previous while authenticating);
                                        //        func_002565E0 passes it to the DNAS module's update func_003FD100
    int FileHandle;                     // 0x018 [R] -1; [U] func_00256258 / func_00256460: the ASYNCFILE_loadat handle;
                                        //       func_00256460 / func_00256510 poll it
    int AuthError;                      // 0x01C (name ours) [U] func_00256258 / func_002562D0 0; func_002565E0 stores
                                        //       whether the check's result was 0; cFEStateAuthDNAS_onUpdate tests it
                                        //       (error: copies ErrorText1..3 into the parent's mpustrAuthError)
    int AuthMode;                       // 0x020 [U] func_00256258 stores its mode argument (from the profile bits);
                                        //       func_002562D0 passes it on
    char unk_024[0x48];                 // 0x024 start of the 160-byte parameter block given to DirtyDnasRelInit
                                        //       (func_002562D0 clears it with MEM_fill and calls func_003FD0D8(self +
                                        //       0x24)); not written otherwise
    char* unk_06C;                      // 0x06C [U] func_002562D0: AuthData (parameter block)
    int unk_070;                        // 0x070 [U] func_002562D0: AuthSize
    tDNASKey unk_074;                   // 0x074 [U] func_002562D0: the 8-byte key D_004A2ED8 (unaligned copy, sdl /
                                        //       sdr)
    char unk_07C[0x8];                  // 0x07C no access seen
    int unk_084;                        // 0x084 [U] func_002562D0: 8 (the key length)
    char unk_088[0x4];                  // 0x088 no access seen
    int unk_08C;                        // 0x08C [U] func_002562D0 0
    int unk_090;                        // 0x090 [U] func_002562D0 0
    char unk_094[0x4];                  // 0x094 no access seen
    char unk_098[0x10];                 // 0x098 [U] func_002562D0: strcpy of NetConnMAC()'s text
    char unk_0A8[0x10];                 // 0x0A8 [U] func_002562D0: "SLUS_20772" (an 11-byte array copy: sdl / sdr, sh
                                        //       at 0xB0, sb at 0xB2)
    int unk_0B8;                        // 0x0B8 [U] func_002562D0 0
    int unk_0BC;                        // 0x0BC [U] func_002562D0: AuthMode
    int unk_0C0;                        // 0x0C0 [U] func_002562D0 1 (end of the parameter block)
    tDNASResult Result;                 // 0x0C4 (name ours) [U] func_002565E0: the 908-byte check result (MEM_fill,
                                        //       then func_003FD128 fills it; wide strings at +0xCC, +0x10C, +0x20C)
    unsigned short ErrorText1[0x100];   // 0x450 (name ours) [U] func_002565E0: USTR_copy of the result's first text;
                                        //       cFEStateAuthDNAS_onUpdate shows it (error text)
    unsigned short ErrorText2[0x100];   // 0x650 (name ours) [U] func_002565E0: the second text; shown by
                                        //       cFEStateAuthDNAS_onUpdate
    unsigned short ErrorText3[0x100];   // 0x850 (name ours) [U] func_002565E0: the third text; shown by
                                        //       cFEStateAuthDNAS_onUpdate
};

CHECK_OFFSET(DNASMan, State, 0x000);
CHECK_OFFSET(DNASMan, ElfData, 0x004);
CHECK_OFFSET(DNASMan, ElfSize, 0x008);
CHECK_OFFSET(DNASMan, AuthData, 0x00C);
CHECK_OFFSET(DNASMan, AuthSize, 0x010);
CHECK_OFFSET(DNASMan, CancelRequest, 0x014);
CHECK_OFFSET(DNASMan, FileHandle, 0x018);
CHECK_OFFSET(DNASMan, AuthError, 0x01C);
CHECK_OFFSET(DNASMan, AuthMode, 0x020);
CHECK_OFFSET(DNASMan, unk_024, 0x024);
CHECK_OFFSET(DNASMan, unk_06C, 0x06C);
CHECK_OFFSET(DNASMan, unk_070, 0x070);
CHECK_OFFSET(DNASMan, unk_074, 0x074);
CHECK_OFFSET(DNASMan, unk_07C, 0x07C);
CHECK_OFFSET(DNASMan, unk_084, 0x084);
CHECK_OFFSET(DNASMan, unk_088, 0x088);
CHECK_OFFSET(DNASMan, unk_08C, 0x08C);
CHECK_OFFSET(DNASMan, unk_090, 0x090);
CHECK_OFFSET(DNASMan, unk_094, 0x094);
CHECK_OFFSET(DNASMan, unk_098, 0x098);
CHECK_OFFSET(DNASMan, unk_0A8, 0x0A8);
CHECK_OFFSET(DNASMan, unk_0B8, 0x0B8);
CHECK_OFFSET(DNASMan, unk_0BC, 0x0BC);
CHECK_OFFSET(DNASMan, unk_0C0, 0x0C0);
CHECK_OFFSET(DNASMan, Result, 0x0C4);
CHECK_OFFSET(DNASMan, ErrorText1, 0x450);
CHECK_OFFSET(DNASMan, ErrorText2, 0x650);
CHECK_OFFSET(DNASMan, ErrorText3, 0x850);
CHECK_SIZE(DNASMan, 0xA50);

#endif /* ONLINE_DNASMAN_H */
