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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFlags);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern char D_004965C0[];

extern "C" int cDirtysock_tag_TagFieldSetFlags(char* record, int len, char* name, int value)
{
    char temp[256 + 32];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, temp, name);
    const char* flags = D_004965C0;
    for (; (value != 0) && (*flags != 0); value >>= 1, flags++) {
        if (value & 1) {
            *data++ = *flags;
        }
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, temp);
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetAddress);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFourCC);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

extern "C" int cDirtysock_tag_TagFieldSetFourCC(char* record, int len, char* name, int value)
{
    char temp[256 + 32];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, temp, name);
    for (; value != 0; value <<= 8) {
        if (value > 0x20FFFFFF) {
            *data++ = (char)(value >> 24);
        }
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, temp);
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetString);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetBinary);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetStructure);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetCrypt);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetEpoch);

INCLUDE_ASM("dirtysock/tags", func_003ECB00);

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetNumber);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldGetNumber(const char* data, int defval)
{
    int sign;
    int value;
    if (data == 0) {
        return defval;
    }
    sign = 1;
    if (*data == '+') {
        ++data;
    } else if (*data == '-') {
        ++data;
        sign = -1;
    }
    for (value = 0; (*data >= '0') && (*data <= '9'); ) {
        value = (value * 10) + (*data++ & 15);
    }
    return sign * value;
}
#endif

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

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetAddr);
#ifdef SKIP_ASM
extern "C" unsigned int cDirtysock_tag_TagFieldGetAddr(const char* data, unsigned int defval)
{
    unsigned int addr = 0;
    if (data != 0) {
        for (;; data++) {
            if ((*data >= '0') && (*data <= '9')) {
                addr = (addr & 0xFFFFFF00) | (((addr & 0xFF) * 10) + (*data & 15));
            } else if (*data == '.') {
                addr <<= 8;
            } else {
                break;
            }
        }
        defval = addr;
    }
    return defval;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetFourCC);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag_TagFieldGetFourCC(const char* data, int defval)
{
    int token;
    if ((data == 0) || (*data <= ' ') || (*data >= 127)) {
        return defval;
    }
    for (token = 0x20202020; (token < 0x20FFFFFF) && (*data > ' ') && (*data < 127); data++) {
        token = (token << 8) | *data;
    }
    return token;
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetString);

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetBinary);
#ifdef SKIP_ASM
extern unsigned char D_004961C0[];
extern unsigned char D_004962C0[];

extern "C" int cDirtysock_tag_TagFieldGetBinary(const char* data, void* buffer, int len)
{
    int count;
    const char* p;
    char* buf = (char*)buffer;

    if ((data == 0) || (*data != '$')) {
        return -1;
    }
    if (buf == 0) {
        for (count = 0, p = data + 1; ((unsigned char)p[0] >= '0') && ((unsigned char)p[1] >= '0'); count++) {
            p += 2;
        }
        return count;
    }
    p = data + 1;
    if (len <= 0) {
        return -1;
    }
    for (count = 0; (count < len) && ((unsigned char)p[0] >= '0') && ((unsigned char)p[1] >= '0'); count++) {
        *buf++ = D_004961C0[(unsigned char)p[0]] | D_004962C0[(unsigned char)p[1]];
        p += 2;
    }
    return count;
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetStructure);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetCrypt);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetTime);

INCLUDE_ASM("dirtysock/tags", func_003EDD70);

