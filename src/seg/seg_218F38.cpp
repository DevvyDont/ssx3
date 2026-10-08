#include "common.h"

//100%
INCLUDE_ASM("seg/seg_218F38", BXFILE_exists);
#ifdef SKIP_ASM
struct sFileInfo_7F38 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo(const char *, int, sFileInfo_7F38 *);

extern "C" int BXFILE_exists(const char *name) {
    sFileInfo_7F38 info;
    return FILESYS_bypassqueuefileinfo(name, 1, &info) == 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218F38", func_00317F60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sFileInfo_7F60 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo_7F60(const char *, int, sFileInfo_7F60 *) __asm__("FILESYS_bypassqueuefileinfo");

extern "C" int func_00317F60(const char *name) {
    sFileInfo_7F60 info;
    info.size = 0;
    bool ne = FILESYS_bypassqueuefileinfo_7F60(name, 1, &info) != 1;
    if (!ne)
        return info.size;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218F38", func_00317F98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sFileInfo_7F98 {
    int size;
    int pad[3];
};
extern "C" int FILESYS_bypassqueuefileinfo_7F98(const char *, int, sFileInfo_7F98 *) __asm__("FILESYS_bypassqueuefileinfo");

extern "C" int func_00317F98(const char *name) {
    sFileInfo_7F98 info;
    info.size = 0;
    FILESYS_bypassqueuefileinfo_7F98(name, 1, &info);
    return info.size;
}
#endif

extern "C" void func_00317FC0(void) {
}

//100%
INCLUDE_ASM("seg/seg_218F38", func_00317FC8);
#ifdef SKIP_ASM
extern "C" void func_00317FC8(void) {
    func_00317FC0();
}
#endif
