#include "common.h"

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_onCreateScreen);

INCLUDE_ASM("fe/festateriderbiodetail", func_00190A08);

INCLUDE_ASM("fe/festateriderbiodetail", func_00190CD8);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillDNAInfo);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillFavesInfo);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillQnAInfo);

INCLUDE_ASM("fe/festateriderbiodetail", cFEStateRiderBio_fillBioInfo);

INCLUDE_ASM("fe/festateriderbiodetail", func_001912D0);

//100%
INCLUDE_ASM("fe/festateriderbiodetail", func_00191360);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" char func_001A1CD0(void* p, int a);
extern void* D_0046B090[];

extern "C" void* func_00191360(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046B090;
    *(int*)((char*)self + 0x48) = 0;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    *(int*)((char*)self + 0xC) = 0x14;
    return self;
}
#endif

