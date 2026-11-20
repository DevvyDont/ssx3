#include "common.h"
#include "render/irradiance.h"

//100%
INCLUDE_ASM("render/irradiance", cIrradianceDataBase_Load);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern const char str_data_worlds_irr_dat[];
extern const char str_IrradianceNames[];
extern const char str_IrradianceArray[];
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct cIrradianceDataBase_ACD0 {
    int* file;      // 0x0
    int count;      // 0x4
    void* samples;  // 0x8, 0xA0 bytes each
    void* index;    // 0xC, 8 bytes each
};

/**
 * Loads the irradiance database from "data/worlds/irr.dat" into its own entry and sample arrays.
 *
 * The file holds a count n, then n 8-byte entries and n 0xA0-byte samples; they are copied into new
 * "IrradianceNames" (+0xC) and "IrradianceArray" (+0x8) blocks, n goes to +0x4 and the loaded file is freed,
 * leaving +0x0 pointing at the freed block.
 */
extern "C" void cIrradianceDataBase_Load(cIrradianceDataBase* self)
{
    int* file = (int*)FILE_load(str_data_worlds_irr_dat, 0x100);
    self->mData = (char*)file;
    int n = file[0];
    char* src = (char*)(file + 1);
    int indexSize = n * 8;
    int sampleSize = n * 160;
    self->mNumItem = n;
    self->mIrradianceNames = (char*)operator_new_tag(indexSize, str_IrradianceNames, 0, 0);
    char* src2 = src + indexSize;
    self->mIrradianceArray = (cIrradiance*)operator_new_tag(sampleSize, str_IrradianceArray, 0x2000000, 0);
    func_0041605C(self->mIrradianceNames, src, indexSize);
    func_0041605C(self->mIrradianceArray, src2, sampleSize);
    if ((int*)self->mData != 0) {
        cMemMan_free((int*)self->mData);
    }
}
#endif

//100%
INCLUDE_ASM("render/irradiance", cIrradianceDataBase_Purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees the irradiance database's loaded arrays when it holds any items.
 *
 * When the word at +0x4 is non-zero it frees the blocks at +0xC and +0x8 with cMemMan_free, then zeroes +0xC, +0x8 and
 * +0x4. Callers: cRealFE_purge and cGame_purge.
 */
extern "C" void cIrradianceDataBase_Purge(cIrradianceDataBase* self)
{
    if (self->mNumItem != 0) {
        if (self->mIrradianceNames != 0) {
            cMemMan_free(self->mIrradianceNames);
        }
        if (self->mIrradianceArray != 0) {
            cMemMan_free(self->mIrradianceArray);
        }
        self->mIrradianceNames = 0;
        self->mIrradianceArray = 0;
        self->mNumItem = 0;
    }
}
#endif

