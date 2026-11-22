#ifndef SEG_NEWLIB_STAT_H
#define SEG_NEWLIB_STAT_H

// newlib 1.9.0's struct stat, as the game's C library fills it (src/seg/seg_325220.c: _fstat, libgloss's fstat stub
// that _fstat_r calls: "Always pretend to be a tty").
//
// [L] refs/newlib-1.9.0-src/newlib-1.9.0/newlib/libc/include/sys/stat.h (the struct, the non-svr4 branch) with the
// member types of libc/include/sys/types.h for a MIPS target (dev_t short, ino_t unsigned short, mode_t unsigned int,
// nlink_t / uid_t / gid_t unsigned short, off_t long) and machine/types.h (time_t long); libgloss/fstat.c stores
// st_mode = S_IFCHR (0020000 = 0x2000) and st_blksize = 0, the two stores of retail's _fstat ([R] sw 0x2000 at +0x4,
// sd zero at +0x48). newlib's headers carry no notice of their own (COPYING.NEWLIB: each source carries its own).
//
// EE note: `long` is 64-bit on the EE (include/gcc/stdint.h: typedef long int64_t), so every long member is 8 bytes
// and 8-aligned: the struct is 0x68 bytes. Offsets in the comments are the EE's; the CHECK lines hold where long's
// size does not change them (clang's mipsel target has a 32-bit long).

#include "layout.h"

struct stat {
    short st_dev;                   // 0x00 [L] dev_t
    unsigned short st_ino;          // 0x02 [L] ino_t
    unsigned int st_mode;           // 0x04 [L] mode_t; [R] _fstat stores S_IFCHR (0x2000)
    unsigned short st_nlink;        // 0x08 [L] nlink_t
    unsigned short st_uid;          // 0x0A [L] uid_t
    unsigned short st_gid;          // 0x0C [L] gid_t
    short st_rdev;                  // 0x0E [L] dev_t
    long st_size;                   // 0x10 [L] off_t
    long st_atime;                  // 0x18 [L] time_t
    long st_spare1;                 // 0x20 [L]
    long st_mtime;                  // 0x28 [L] time_t
    long st_spare2;                 // 0x30 [L]
    long st_ctime;                  // 0x38 [L] time_t
    long st_spare3;                 // 0x40 [L]
    long st_blksize;                // 0x48 [L]; [R] _fstat stores 0 (an sd: 8 bytes)
    long st_blocks;                 // 0x50 [L]
    long st_spare4[2];              // 0x58 [L]
};

// CHECK_OFFSET pastes the type name, so the checks of `struct stat` are spelled out (as in seg/newlib_reent.h).
#ifndef DECOMP_NO_LAYOUT_CHECKS
typedef char check_stat_st_dev[(DECOMP_OFFSETOF(struct stat, st_dev) == 0x00) ? 1 : -1];
typedef char check_stat_st_ino[(DECOMP_OFFSETOF(struct stat, st_ino) == 0x02) ? 1 : -1];
typedef char check_stat_st_mode[(DECOMP_OFFSETOF(struct stat, st_mode) == 0x04) ? 1 : -1];
typedef char check_stat_st_nlink[(DECOMP_OFFSETOF(struct stat, st_nlink) == 0x08) ? 1 : -1];
typedef char check_stat_st_uid[(DECOMP_OFFSETOF(struct stat, st_uid) == 0x0A) ? 1 : -1];
typedef char check_stat_st_gid[(DECOMP_OFFSETOF(struct stat, st_gid) == 0x0C) ? 1 : -1];
typedef char check_stat_st_rdev[(DECOMP_OFFSETOF(struct stat, st_rdev) == 0x0E) ? 1 : -1];
typedef char check_stat_st_size[(DECOMP_OFFSETOF(struct stat, st_size) == 0x10) ? 1 : -1];
#endif

#endif /* SEG_NEWLIB_STAT_H */
