#include "common.h"

//100%
INCLUDE_ASM("seg/seg_218F38", BXFILE_exists);
#ifdef SKIP_ASM
struct sFileInfo_7F38 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo(const char *, int, sFileInfo_7F38 *);

/**
 * Tells whether a file exists, asking the file system directly instead of through its request queue.
 *
 * @param name  the file name
 * @return 1 when FILESYS_bypassqueuefileinfo() finds the file (returns 1), otherwise 0
 */
extern "C" int BXFILE_exists(const char *name) {
    sFileInfo_7F38 info;
    return FILESYS_bypassqueuefileinfo(name, 1, &info) == 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218F38", BXFILE_sizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sFileInfo_7F60 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo_7F60(const char *, int, sFileInfo_7F60 *) __asm__("FILESYS_bypassqueuefileinfo");

/**
 * Returns a file's size from the file system, bypassing its request queue, or 0 when the file is not found.
 *
 * @param name  the file name
 * @return the size in bytes, or 0 when FILESYS_bypassqueuefileinfo() does not return 1
 */
extern "C" int BXFILE_sizez(const char *name) {
    sFileInfo_7F60 info;
    info.size = 0;
    bool ne = FILESYS_bypassqueuefileinfo_7F60(name, 1, &info) != 1;
    if (!ne)
        return info.size;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218F38", BXFILE_size);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sFileInfo_7F98 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo_7F98(const char *, int, sFileInfo_7F98 *) __asm__("FILESYS_bypassqueuefileinfo");

/**
 * Returns a file's size from the file system, bypassing its request queue, without checking whether it was
 * found.
 *
 * The size field is zeroed first, so a missing file reads as 0 unless the lookup writes it anyway.
 *
 * @param name  the file name
 * @return the size from FILESYS_bypassqueuefileinfo()'s info record
 */
extern "C" int BXFILE_size(const char *name) {
    sFileInfo_7F98 info;
    info.size = 0;
    FILESYS_bypassqueuefileinfo_7F98(name, 1, &info);
    return info.size;
}
#endif

/**
 * Does nothing (an empty function).
 *
 * Its only caller is func_00317FC8, which nothing in retail calls.
 */
extern "C" void func_00317FC0(void) {
}

//100%
INCLUDE_ASM("seg/seg_218F38", func_00317FC8);
#ifdef SKIP_ASM
/**
 * Calls func_00317FC0, which does nothing.
 *
 * No retail code calls it.
 */
extern "C" void func_00317FC8(void) {
    func_00317FC0();
}
#endif
