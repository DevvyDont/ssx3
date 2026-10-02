#include "common.h"

//100%
INCLUDE_ASM("main/loadscreens", cBackgroundMan_loadImages);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* ptr);
extern "C" void* func_003E1908(const char* name, int flags);
extern "C" void* SHAPE_unpack(void* shape);
extern char D_0045CE30[];
extern char D_004A12A0[];
extern char D_004A12A8[];

struct sRVEntry_17ACC8 {
    short delta;
    short index;
    int (*fn)(void*, void*, const char*, int, int, int);
};

struct sRCtx_17ACC8 {
    char pad[0x10D8];
    sRVEntry_17ACC8* vt;
};
extern sRCtx_17ACC8* D_004A289C;

struct sBgFile_17ACC8 {
    int f0;
    int f4;
    int count;
    int fC;
    int f10;
    struct {
        int offset;
        int f4;
    } shapes[1];
};

struct sBgMan_17ACC8 {
    int* images;
    int count;
};

extern "C" int cBackgroundMan_loadImages(sBgMan_17ACC8* self)
{
    sBgFile_17ACC8* file = (sBgFile_17ACC8*)func_003E1908(D_0045CE30, 0x100);
    if (file == 0) {
        return 0;
    }
    int n = file->count;
    self->count = n;
    self->images = (int*)operator_new_tag(n * 4, D_004A12A0, 0, 0);
    for (int i = 0; i < self->count; i++) {
        char* shape = (char*)file + file->shapes[i].offset;
        void* img = SHAPE_unpack(shape);
        self->images[i] = D_004A289C->vt[46].fn((char*)D_004A289C + D_004A289C->vt[46].delta, img, D_004A12A8, 0, 1, -1);
        if (img != shape && img != 0) {
            cMemMan_free(img);
        }
    }
    if (file != 0) {
        cMemMan_free(file);
    }
    return 1;
}
#endif

INCLUDE_ASM("main/loadscreens", func_0017AE00);

INCLUDE_ASM("main/loadscreens", func_0017B1B0);

INCLUDE_ASM("main/loadscreens", func_0017B4E0);

