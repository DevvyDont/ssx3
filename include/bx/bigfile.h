#ifndef BX_BIGFILE_H
#define BX_BIGFILE_H

// cBigFile: one mounted BIG archive: its file-system handle and whether it is mounted. cBigFile_open mounts the
// archive with FILESYS_addbigsync, which writes the handle through `this` (Handle), and sets Mounted; cBigFile_close
// unmounts it (FILESYS_delbigsync) when mounted. The replay player and the load screens each keep one. Methods in
// bx/bigfile.
//
// Size 0x8 [R] (cPlaybackMan's cMemMan_alloc(8, "BigFile"); On Tour's is the same). Member names are EA's, from SSX
// On Tour's PSP DWARF ([O]); every retail access through `this` falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"

struct cBigFile {
    int Handle;                     // 0x00 [O]; [R] ctors -1; [U] close passes it to FILESYS_delbigsync
    int Mounted;                    // 0x04 [O] (On Tour bool); [R] ctors 0; [U] open sets it, close tests and clears it

    // Methods bound to their retail symbols by asm labels.
    cBigFile* cBigFile1() __asm__("cBigFile_cBigFile1__FP8cBigFile");
    int close() __asm__("cBigFile_close__FP8cBigFile");
};

CHECK_OFFSET(cBigFile, Mounted, 0x04);
CHECK_SIZE(cBigFile, 0x8);

#endif /* BX_BIGFILE_H */
