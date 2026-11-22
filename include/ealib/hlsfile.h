#ifndef EALIB_HLSFILE_H
#define EALIB_HLSFILE_H

// SCATTERREQUEST: one scatter load of REAL core's high-level file system (FILE_scatterload: load several named files
// into several buffers as one FILESYS_atomic operation). EA source: realcore/source/file/cmn/hlsfile.cpp (SSX On Tour:
// realcore 1.06.00; FILE_scatterload(int num, const char** names, void** buffers, int flags) keeps a local
// `SCATTERREQUEST req`). Retail code: unit ealib/seg_2DCAE8 (FILE_scatterload and its atomic callback func_003E1C00).
//
// Names, member order and offsets: [O] On Tour struct SCATTERREQUEST (0x14); [U] retail func_003E1C00 (the callback
// FILE_scatterload passes &req to) loops num times and FILE_loadz()s names[i] with flags into buffers[i].
// C struct. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct SCATTERREQUEST SCATTERREQUEST;
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

#endif /* EALIB_HLSFILE_H */
