#include "common.h"

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
extern "C" void cIrradianceDataBase_Load(cIrradianceDataBase_ACD0* self)
{
    int* file = (int*)FILE_load(str_data_worlds_irr_dat, 0x100);
    self->file = file;
    int n = file[0];
    char* src = (char*)(file + 1);
    int indexSize = n * 8;
    int sampleSize = n * 0xA0;
    self->count = n;
    self->index = operator_new_tag(indexSize, str_IrradianceNames, 0, 0);
    char* src2 = src + indexSize;
    self->samples = operator_new_tag(sampleSize, str_IrradianceArray, 0x2000000, 0);
    func_0041605C(self->index, src, indexSize);
    func_0041605C(self->samples, src2, sampleSize);
    if (self->file != 0) {
        cMemMan_free(self->file);
    }
}
#endif

//100%
INCLUDE_ASM("render/irradiance", func_0038ADB0);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void func_0038ADB0(void* self)
{
    if (*(int*)((char*)self + 0x4) != 0) {
        if (*(void**)((char*)self + 0xC) != 0) {
            cMemMan_free(*(void**)((char*)self + 0xC));
        }
        if (*(void**)((char*)self + 0x8) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x8));
        }
        *(void**)((char*)self + 0xC) = 0;
        *(void**)((char*)self + 0x8) = 0;
        *(int*)((char*)self + 0x4) = 0;
    }
}
#endif

