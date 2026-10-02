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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetNumber);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

extern "C" int cDirtysock_tag_TagFieldSetNumber(char* record, int len, char* name, int value)
{
    char temp[256 + 32];
    unsigned char* num;
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, temp, name);

    if (value < 0) {
        *data++ = '-';
        value = -value;
    }
    num = (unsigned char*)data + 31;
    *num = 0;
    while (value > 0) {
        *--num = '0' + (value % 10);
        value /= 10;
    }
    if (*num == 0)
        *--num = '0';
    do {
        *data++ = *num++;
    } while (*num != 0);
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, temp);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetBinary);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern unsigned char D_00495FC0[];
extern unsigned char D_004960C0[];

extern "C" int cDirtysock_tag_TagFieldSetBinary(char* record, int len, char* name, const void* value, int size)
{
    const unsigned char* src = (const unsigned char*)value;
    char temp[4096 + 256];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, temp, name);
    *data++ = '$';
    for (; size > 0; --size) {
        *data++ = D_00495FC0[*src];
        *data++ = D_004960C0[*src++];
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, temp);
}
#endif

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetStructure);

INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetCrypt);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetEpoch);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_00496A40[];

struct sDsTmK2 {
    int tm_sec;   // 0x0
    int tm_min;   // 0x4
    int tm_hour;  // 0x8
    int tm_mday;  // 0xC
    int tm_mon;   // 0x10
    int tm_year;  // 0x14
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

// ds_timeinsecs / ds_secstotime
extern "C" unsigned int func_003EF088();
extern "C" sDsTmK2* func_003EAEB8(sDsTmK2* tm, unsigned long secs);

// PORT: epoch is widened to 64-bit `unsigned long` (zero-extended) for ds_secstotime.
extern "C" int cDirtysock_tag_TagFieldSetEpoch(char* record, int len, char* name, unsigned int epoch)
{
    sDsTmK2 tm;
    char temp[256 + 32];
    sDsTmK2* t;
    unsigned long secs = epoch;

    if (secs == 0)
        secs = func_003EF088();
    t = func_003EAEB8(&tm, secs);
    if (t == 0)
        return -1;
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, temp, name);
    sprintf(data, D_00496A40, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return cDirtysock_tag__TagFieldSetupTerm(record, len, temp);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003ECB00);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

// TagFieldMerge: apply every field of `data` to the record; returns how many were set.
extern "C" int func_003ECB00(char* record, int len, unsigned char* data)
{
    int count = 0;
    for (;;) {
        unsigned char c = *data;
        if (c == 0)
            break;
        // skip whitespace
        if (c <= ' ') {
            ++data;
            continue;
        }
        // stop at a separator
        if ((c == '=') || (c == ':'))
            break;
        // add the field
        if (cDirtysock_tag__TagFieldSetupTerm(record, len, (char*)data) > 0)
            ++count;
        // skip to the next field
        while (*data >= ' ')
            ++data;
    }
    return count;
}
#endif

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

