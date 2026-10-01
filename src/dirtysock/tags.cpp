#include "common.h"

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag__TagFieldSetupAppend__FPcN20);
#ifdef SKIP_ASM
char* cDirtysock_tag__TagFieldSetupAppend(char* buf, char* dst, char* name)
{
    if (name == 0) {
        *buf = 0;
    } else {
        while (*name != 0) {
            *dst++ = *name++;
        }
        *dst++ = '=';
    }
    return dst;
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag__TagFieldSetupTerm);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldFind);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldDelete);

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldDupl);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldDupl(char* dst, int len, const char* src)
{
    int left = len;
    while (left > 1 && *src != 0) {
        *dst++ = *src++;
        left--;
    }
    if (left > 0) {
        *dst = 0;
    }
    return len - left;
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetNumber);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFlags);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetAddress);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFourCC);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetString);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetBinary);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetStructure);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetCrypt);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetEpoch);

INCLUDE_ASM("dirtysock/tags", func_003ECB00);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetNumber);

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetFlags);
#ifdef SKIP_ASM
extern int D_004965E0[];

extern "C" int cDirtysock_tag_TagFieldGetFlags(const char* data, int defval)
{
    if (data != 0) {
        int flags = 0;
        int f;
        while ((f = D_004965E0[*data]) != 0) {
            flags |= f;
            data++;
        }
        defval = flags;
    }
    return defval;
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetAddr);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetFourCC);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetString);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetBinary);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetStructure);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetCrypt);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetTime);

INCLUDE_ASM("dirtysock/tags", func_003EDD70);

