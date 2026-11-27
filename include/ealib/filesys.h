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
typedef struct FILE_FILEDEVICE FILE_FILEDEVICE;
#endif

// The operation types (OP_def's type bits) and the operation errors (FILEOPERATION_def::error). [O] On Tour's enums
// FILEOPTYPE and FILEERR (names and values); [U] FILESYS_read stores type 4, FILESYS_delbig 10, FILESYS_completeop
// returns the handle for an open / addbig, the value for a size / exist and the amount for a read; a null file handle
// sets error 6.
enum FILEOPTYPE {
    FOPTYPE_unused = 0, FOPTYPE_used = 1, FOPTYPE_open = 2, FOPTYPE_close = 3, FOPTYPE_read = 4, FOPTYPE_write = 5,
    FOPTYPE_size = 6, FOPTYPE_null = 7, FOPTYPE_exist = 8, FOPTYPE_addbig = 9, FOPTYPE_delbig = 10
};
enum FILEERR {
    FERR_NormalEnd = 0, FERR_NoOps = 1, FERR_NoHandles = 2, FERR_BigClose = 3, FERR_BigOpen = 4, FERR_NoExist = 5,
    FERR_InvalidHandle = 6, FERR_DelBig = 7, FERR_RemoveBig = 8, FERR_NumErrors = 9
};

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
// member names in On Tour's dump, so its bit-field names are ours. [U] retail splits the word as bits 0..19 a serial
// number, 20..23 the operation type (FILESYS_read stores 4, FILESYS_delbig 10), 24..31 the operation's index in the
// pool (the unit's sFsCmdId; FILESYS_waitop reads it as the handle's top byte).
union OP_def {
    struct {
        unsigned int serial : 20;   // bits 0..19 (name ours); [U] compared (& 0xFFFFF) to validate a handle
        unsigned int type : 4;      // bits 20..23 (name ours); [U] the operation type
        unsigned int index : 8;     // bits 24..31 (name ours); [U] the record's index in gFileDevice.oplist
    } sep;                          // 0x0 [O]
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

// FILE_FILEDEVICE (0x38 [O]; [R] retail gFileDevice 0x00519AB0 runs to pFileDeviceMem at 0x00519AE8): the file
// system's device state. Retail has On Tour's layout member for member: [U] the roles below (the unit's old view
// sAB0) and the splat labels inside it (D_00519AB8 minpriority, D_00519AC4 mutex, D_00519AD8 oplist, D_00519AE4
// bigfilelist).
struct FILE_FILEDEVICE {
    int maxfileops;                     // 0x00 [O]; [U] the number of operation records
    int maxopen;                        // 0x04 [O]; [U] the number of open-file records
    int minpriority;                    // 0x08 [O]; [U] FILESYS_initadr writes 0xFF, FILESYS_setminpriority stores it
    int numopsqueued;                   // 0x0C [O]; [U] operations queued
    int waitlevel;                      // 0x10 [O] (volatile there); [U] the callback depth
    int mutex[4];                       // 0x14 [O] (MUTEX, ealib/stream.h: 0x10 bytes)
    FILEOPERATION_def* exec;            // 0x24 [O] (volatile there); [U] the running operation
    FILEOPERATION_def* oplist;          // 0x28 [O]; [U] the operation records (reserveop hands one out)
    HANDLE_def* handlelist;             // 0x2C [O]; [U] the open-file records (reservehandle hands one out)
    FILEOPERATION_def* opqhead;         // 0x30 [O]; [U] the queue of pending operations
    struct FILEBIG_def* bigfilelist;    // 0x34 [O]; [U] the mounted BIG archives
};

CHECK_OFFSET(FILE_FILEDEVICE, maxfileops, 0x00);
CHECK_OFFSET(FILE_FILEDEVICE, maxopen, 0x04);
CHECK_OFFSET(FILE_FILEDEVICE, minpriority, 0x08);
CHECK_OFFSET(FILE_FILEDEVICE, numopsqueued, 0x0C);
CHECK_OFFSET(FILE_FILEDEVICE, waitlevel, 0x10);
CHECK_OFFSET(FILE_FILEDEVICE, mutex, 0x14);
CHECK_OFFSET(FILE_FILEDEVICE, exec, 0x24);
CHECK_OFFSET(FILE_FILEDEVICE, oplist, 0x28);
CHECK_OFFSET(FILE_FILEDEVICE, handlelist, 0x2C);
CHECK_OFFSET(FILE_FILEDEVICE, opqhead, 0x30);
CHECK_OFFSET(FILE_FILEDEVICE, bigfilelist, 0x34);
CHECK_SIZE(FILE_FILEDEVICE, 0x38);

extern struct FILE_FILEDEVICE gFileDevice; // 0x00519AB0 [O] (On Tour `FILE_FILEDEVICE gFileDevice`)


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
