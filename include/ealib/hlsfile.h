#ifndef EALIB_HLSFILE_H
#define EALIB_HLSFILE_H

// SCATTERREQUEST: one scatter load of REAL core's high-level file system (FILE_scatterload: load several named files
// into several buffers as one FILESYS_atomic operation). EA source: realcore/source/file/cmn/hlsfile.cpp (SSX On Tour:
// realcore 1.06.00; FILE_scatterload(int num, const char** names, void** buffers, int flags) keeps a local
// `SCATTERREQUEST req`). Retail code: unit ealib/seg_2DCAE8 (FILE_scatterload and its atomic callback scatterloadatom).
//
// Names, member order and offsets: [O] On Tour struct SCATTERREQUEST (0x14); [U] retail scatterloadatom (the callback
// FILE_scatterload passes &req to) loops num times and FILE_loadz()s names[i] with flags into buffers[i].
// C struct. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct SCATTERREQUEST SCATTERREQUEST;
typedef struct FILEREQUEST FILEREQUEST;
#endif

struct SCATTERREQUEST {
    int num;                        // 0x00 [O]; [U] the loop bound
    const char** names;             // 0x04 [O]; [U] names[i] is loaded
    void** buffers;                 // 0x08 [O]; [U] into buffers[i]
    int flags;                      // 0x0C [O]; [U] FILE_loadz's flags
    int abortflag;                  // 0x10 [O]
};

CHECK_OFFSET(SCATTERREQUEST, num, 0x00);
CHECK_OFFSET(SCATTERREQUEST, names, 0x04);
CHECK_OFFSET(SCATTERREQUEST, buffers, 0x08);
CHECK_OFFSET(SCATTERREQUEST, flags, 0x0C);
CHECK_OFFSET(SCATTERREQUEST, abortflag, 0x10);
CHECK_SIZE(SCATTERREQUEST, 0x14);

// FILEREQUEST: the argument block of a single-file high-level load (0x14 [O]: On Tour's FILE_loadsize(const char* name,
// int* size, int flags) keeps a local `FILEREQUEST req` and hands it to FILESYS_atomic). [U] FILE_load / FILE_loadsize
// store name, abortflag 1 and flags; the z variants (FILE_loadz, FILE_loadsizez) store abortflag 0; loadfilesizeatom
// returns the size in parm. Names, member order and offsets: [O].
struct FILEREQUEST {
    const char* name;               // 0x00 [O]; [U] the path
    void* location;                 // 0x04 [O]
    int parm;                       // 0x08 [O]; [U] loadfilesizeatom stores the file size
    int abortflag;                  // 0x0C [O]; [U] 1 from FILE_load / FILE_loadsize, 0 from the z variants
    int flags;                      // 0x10 [O]; [U] the allocation flags (FILESYSOPTS::malloc)
};

CHECK_OFFSET(FILEREQUEST, name, 0x00);
CHECK_OFFSET(FILEREQUEST, location, 0x04);
CHECK_OFFSET(FILEREQUEST, parm, 0x08);
CHECK_OFFSET(FILEREQUEST, abortflag, 0x0C);
CHECK_OFFSET(FILEREQUEST, flags, 0x10);
CHECK_SIZE(FILEREQUEST, 0x14);

#endif /* EALIB_HLSFILE_H */
