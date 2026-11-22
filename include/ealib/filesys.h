#ifndef EALIB_FILESYS_H
#define EALIB_FILESYS_H

// EA realcore's asynchronous file system (filesys.cpp): every read / open / close is a FILEOPERATION_def taken from a
// pool (reserveop), queued by priority and run by iFILESYS_ExecCommand (views group D: C_D648 in ealib/seg_2DCAE8).
// EA source: realcore/source/file/<platform>/filesys.cpp (On Tour: realcore 1.06.00, psp). Retail code: unit
// ealib/seg_2DCAE8 (FILESYS_*, reserveop, iFILESYS_ExecCommand); the unit's own struct sFsCmd (0x30, gFileDevice's
// command pool) is this record with offset names.
// Tags: [O] SSX On Tour's DWARF (struct / union names, members, offsets, the paired functions reserveop() returning
// FILEOPERATION_def*, iFILESYS_ExecCommand(FILEOPERATION_def* fop), FILESYS_read(int filehandle, int offset,
// void* buffer, int bytes, int priority, void* userdata)); [U] retail use.

#include "layout.h"

// C units (ealib/seg_2DCAE8.c) name these types without the struct / union keyword.
#ifndef __cplusplus
typedef union OP_def OP_def;
typedef struct FILEOPERATION_def FILEOPERATION_def;
typedef struct HANDLE_def HANDLE_def;
typedef struct FILESYSOPTS FILESYSOPTS;
typedef struct FILEBIG_def FILEBIG_def;
#endif

// An open file, retail version (0x110 [R]: FILESYS_initadr / FILESYS_overhead size the handle table as maxopenfiles * 272;
// the unit's view sFsHd). [O] struct HANDLE_def (0x118: info, data, length, bigoffset, DVDstartsector, checkforEOF,
// name[255]); retail has no data / checkforEOF, so every later member sits lower. [U] the FILESYS open command
// (FILEDEV_open / getsize / getstartsector, the archive lookup) and FILESYS_open / FILESYS_exists.
struct HANDLE_def {
    int info;                           // 0x000 [O] (a union there); [U] the device's file handle (FILEDEV_open /
                                        //       readreq / close), or the archive node when bigoffset != 0
    int length;                         // 0x004 [O]; [U] FILEDEV_getsize, or the file's size inside the archive
    int bigoffset;                      // 0x008 [O]; [U] the offset inside the archive (0 = a plain file: close
                                        //       through the device)
    int DVDstartsector;                 // 0x00C [O]; [U] FILEDEV_getstartsector
    char name[0x100];                   // 0x010 [O] (char[255] at 0x18 there); [U] FILESYS_open / FILESYS_exists
                                        //       strncpy 0x100 bytes; the open command splits it at '|'
};

CHECK_OFFSET(HANDLE_def, info, 0x000);
CHECK_OFFSET(HANDLE_def, length, 0x004);
CHECK_OFFSET(HANDLE_def, bigoffset, 0x008);
CHECK_OFFSET(HANDLE_def, DVDstartsector, 0x00C);
CHECK_OFFSET(HANDLE_def, name, 0x010);
CHECK_SIZE(HANDLE_def, 0x110);

// The operation's id word. [O] union OP_def {<anonymous struct> sep; int id;} (0x4): the bit-field view `sep` has no
// member names in On Tour's dump, so only `id` is placed. [U] retail splits the word as bits 0..19 a serial number,
// 20..23 the operation type (FILESYS_read stores 4), 24..31 the operation's index in the pool (the unit's sFsCmdId).
// Blocking: code that stores the type field (view C_D648's f:4) needs sep's bit fields, which have no EA names.
union OP_def {
    int id;                         // 0x0 [O]
};

CHECK_SIZE(OP_def, 0x4);

// [O] struct FILEOPERATION_def (0x30); [U] FILESYS_read fills userdata, priority, the type bits of op, error, handle,
// data, value, amount; the queue links through next.
struct FILEOPERATION_def {
    union OP_def op;                    // 0x00 [O]; [U] the operation's handle (returned to the caller)
    int cancelled;                      // 0x04 [O]
    volatile int status;                // 0x08 [O]
    int error;                          // 0x0C [O] FILEERR (FERR_NormalEnd 0 .. FERR_RemoveBig 8)
    int priority;                       // 0x10 [O]; [U] FILESYS_read's priority
    void* userdata;                     // 0x14 [O]; [U] FILESYS_read's userdata (the callback argument)
    int value;                          // 0x18 [O]; [U] FILESYS_read: the offset
    int amount;                         // 0x1C [O]; [U] FILESYS_read: the byte count
    void* data;                         // 0x20 [O]; [U] FILESYS_read: the buffer
    struct HANDLE_def* handle;          // 0x24 [O]; [U] the open file
    void (*callback)(int, int, int);    // 0x28 [O] (the dump prints `void (*)() *`); [U] called with three words (sFsCmd::f28)
    struct FILEOPERATION_def* next;     // 0x2C [O]; [U] the queue link
};

CHECK_OFFSET(FILEOPERATION_def, op, 0x00);
CHECK_OFFSET(FILEOPERATION_def, cancelled, 0x04);
CHECK_OFFSET(FILEOPERATION_def, status, 0x08);
CHECK_OFFSET(FILEOPERATION_def, error, 0x0C);
CHECK_OFFSET(FILEOPERATION_def, priority, 0x10);
CHECK_OFFSET(FILEOPERATION_def, userdata, 0x14);
CHECK_OFFSET(FILEOPERATION_def, value, 0x18);
CHECK_OFFSET(FILEOPERATION_def, amount, 0x1C);
CHECK_OFFSET(FILEOPERATION_def, data, 0x20);
CHECK_OFFSET(FILEOPERATION_def, handle, 0x24);
CHECK_OFFSET(FILEOPERATION_def, callback, 0x28);
CHECK_OFFSET(FILEOPERATION_def, next, 0x2C);
CHECK_SIZE(FILEOPERATION_def, 0x30);

// The file system's options, retail version (0x24 [R]: FILESYS_setopts clears 36 bytes for a short record and copies
// the caller's; On Tour 0x1C): gFileSysOpts [O] (On Tour `FILESYSOPTS gFileSysOpts`; the unit's view sC10). [O] member
// names for size / malloc / free / REAL_filedevice; retail orders the rest differently (On Tour: powercallback 0x0C,
// REAL_filedevice 0x10, DemoCdRoot 0x14, UseDisc 0x18), so only the members with a retail use that matches On Tour's
// meaning are named. [U] FILESYS_getopts / FILESYS_setopts mirror 0x0C / 0x10 / 0x14 / 0x18 into the live settings
// D_00450C58 and 0x1C into the REAL_filedevice global; iFILE_addbigreadcallback allocates and frees the archive header
// through malloc / free (also the splat symbols D_00450C14 / D_00450C18).
struct FILESYSOPTS {
    int size;                           // 0x00 [O]; [U] the record's byte count (getopts / setopts read it first)
    void* (*malloc)(char*, int, int);   // 0x04 [O]; [U] iFILE_addbigreadcallback: (name, size, memflags)
    int (*free)(void*);                 // 0x08 [O]; [U] iFILE_addbigreadcallback frees a header buffer
    int unk_0C;                         // 0x0C [U] setopts: D_00450C58 + 0x8
    int unk_10;                         // 0x10 [U] get / setopts: D_00450C58 + 0x4
    int unk_14;                         // 0x14 [U] get / setopts: D_00450C58 + 0xC (getopts stores it as a 0 / 1 flag)
    char* unk_18;                       // 0x18 [U] get / setopts: D_00450C58 + 0x0, the root prefix file names are
                                        //       matched against
    char* REAL_filedevice;              // 0x1C [O] (0x10 there); [U] getopts / setopts copy it to and from the global
                                        //       REAL_filedevice (the host-device prefix)
    void* unk_20;                       // 0x20 [U] the built-in file table ({name, ?, ?, path} records, a 0 name ends
                                        //       it); retail-only
};

CHECK_OFFSET(FILESYSOPTS, malloc, 0x04);
CHECK_OFFSET(FILESYSOPTS, free, 0x08);
CHECK_OFFSET(FILESYSOPTS, unk_0C, 0x0C);
CHECK_OFFSET(FILESYSOPTS, unk_10, 0x10);
CHECK_OFFSET(FILESYSOPTS, unk_14, 0x14);
CHECK_OFFSET(FILESYSOPTS, unk_18, 0x18);
CHECK_OFFSET(FILESYSOPTS, REAL_filedevice, 0x1C);
CHECK_OFFSET(FILESYSOPTS, unk_20, 0x20);
CHECK_SIZE(FILESYSOPTS, 0x24);

extern FILESYSOPTS gFileSysOpts; // 0x00450C10: the file system's options (FILESYS_getopts / FILESYS_setopts copy it)

// A mounted BIG archive, retail version (0x18 [U]: the records iFILE_addbigopencallback / iFILE_addbigreadcallback fill;
// On Tour 0x1C): [O] struct FILEBIG_def {header, data, headersize, memflags, handle, fop, next}, linked from the file
// device's bigfilelist (On Tour FILE_FILEDEVICE + 0x34; retail gFileDevice + 0x34, also D_00519AE4). Retail has no
// `data` word, so every later member sits 4 lower. [U] iFILE_addbigreadcallback: header is the buffer the BIG header is
// read into (BIG_typeofheader / BIG_sizeofheader), headersize its size (regrown to BIG_sizeofheader), memflags the
// allocation flags, handle the open archive (its name at +0x10 is what FILESYS_addbig / bypassqueuefileinfo compare),
// fop the add operation run once the header is in, next the list link.
struct FILEBIG_def {
    void* header;                       // 0x00 [O]; [U] the header buffer
    int headersize;                     // 0x04 [O] (0x08 there); [U] the buffer's size
    int memflags;                       // 0x08 [O] (0x0C there); [U] passed to FILESYSOPTS::malloc
    struct HANDLE_def* handle;          // 0x0C [O] (0x10 there); [U] the open archive file
    struct FILEOPERATION_def* fop;      // 0x10 [O] (0x14 there); [U] the FILESYS_addbig operation
    struct FILEBIG_def* next;           // 0x14 [O] (0x18 there); [U] the bigfilelist link
};

CHECK_OFFSET(FILEBIG_def, header, 0x00);
CHECK_OFFSET(FILEBIG_def, headersize, 0x04);
CHECK_OFFSET(FILEBIG_def, memflags, 0x08);
CHECK_OFFSET(FILEBIG_def, handle, 0x0C);
CHECK_OFFSET(FILEBIG_def, fop, 0x10);
CHECK_OFFSET(FILEBIG_def, next, 0x14);
CHECK_SIZE(FILEBIG_def, 0x18);

#endif /* EALIB_FILESYS_H */
