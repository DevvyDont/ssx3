#include "common.h"

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag__TagFieldSetupAppend__FPcN20);
#ifdef SKIP_ASM
/**
 * Writes a field name and '=' at the append position of a tag buffer.
 *
 * With a null @p name it instead empties the buffer (writes 0 to its first byte) and leaves @p dst where it
 * was. Nothing is NUL-terminated after the '='.
 *
 * @param buf   start of the tag buffer
 * @param dst   where to write the name
 * @param name  field name, or 0
 * @return the position just after the '=', or @p dst when @p name is 0
 */
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

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag__TagFieldSetupTerm);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern signed char D_004A4A70;
extern unsigned char D_004964C0[];
extern "C" int func_00415FC8(const void* a, const void* b, unsigned int n);
extern "C" void* func_0041610C(void* dst, const void* src, unsigned int n);
extern "C" void* func_0041605C(void* dst, const void* src, unsigned int n);

/**
 * Adds a "name=value" term to a tag record, replacing an existing term of the same name.
 *
 * Names compare case-insensitively up to their '=' or ':'. A new term goes at the end of the record, or before
 * a term that starts with '=' or ':'; a name starting with '~' is always appended without a search. Each term
 * ends with the divider character (D_004A4A70, '\n'); the record's later terms are moved to make room.
 *
 * @param record  the tag record to edit (NUL-terminated)
 * @param len     size of the record buffer in bytes
 * @param field    the term to store, "name=value"; must start with a printable character
 * @return the stored term's length, 0 when an identical term was already there, or -1 on a null record, an
 *         invalid term or no room
 */
// Adds or replaces the "name=value" term temp in the record. Names compare
// through the D_004964C0 class table; "~..." terms are appended at the end.
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* field)
{
    unsigned char* rec = (unsigned char*)record;
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* end;
    int n;
    int sizeDelta;
    unsigned int recordClass;
    unsigned int fieldClass;
    if (rec == 0) return -1;
    if (field == 0 || (unsigned char)*field <= ' ') return -1;
    if (*field == '~') {
        cursor = rec;
        while (*cursor != 0) cursor++;
        if (cursor != rec && cursor[-1] >= 0x20 && cursor[-1] != D_004A4A70 && cursor < rec + len - 1) {
            *cursor++ = D_004A4A70;
            *cursor = 0;
        }
        end = cursor;
        start = cursor;
    } else {
        cursor = rec;
        for (;;) {
            if (*cursor == 0) {
                if (cursor != rec && cursor[-1] >= 0x20 && cursor[-1] != D_004A4A70 && cursor < rec + len - 1) {
                    *cursor++ = D_004A4A70;
                    *cursor = 0;
                }
                end = cursor;
                start = cursor;
                break;
            }
            if (*cursor <= ' ') {
                cursor++;
                continue;
            }
            if (D_004964C0[*cursor] == 1) {
                end = cursor;
                start = cursor;
                break;
            }
            recordClass = D_004964C0[*cursor];
            fieldClass = D_004964C0[*field];
            n = 0;
            while (recordClass == fieldClass && recordClass >= 2) {
                n++;
                recordClass = D_004964C0[cursor[n]];
                fieldClass = D_004964C0[field[n]];
            }
            if (recordClass + fieldClass == 2) {
                end = cursor;
                start = cursor;
                while (*end >= 0x20) end++;
                if (*end != 0) end++;
                goto found;
            }
            cursor++;
            while (*cursor >= 0x20) cursor++;
        }
    }
found:
    int old = end - cursor;
    n = 0;
    while (field[n] >= 0x20) n++;
    cursor = end;
    n++;
    sizeDelta = n - old;
    while (*cursor != 0) cursor++;
    cursor++;
    if (sizeDelta > 0 && rec + len - cursor < sizeDelta) return -1;
    if (sizeDelta == 0 && func_00415FC8(start, field, n) == 0) return 0;
    if (sizeDelta > 0) func_0041610C(end + sizeDelta, end, cursor - end);
    if (sizeDelta < 0) func_0041610C(start, start - sizeDelta, cursor - (start - sizeDelta));
    func_0041605C(start, field, n);
    if (start[n] == 0 && D_004A4A70 != '\n')
        start[n - 1] = 0;
    else
        start[n - 1] = D_004A4A70;
    return n - 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", func_003EB578);
#ifdef SKIP_ASM
extern signed char D_004A4A70;

extern "C" signed char func_003EB578(signed char arg0) {
    signed char temp_2;

    temp_2 = D_004A4A70;
    D_004A4A70 = arg0;
    return temp_2;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tags", func_003EB588);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Normalizes a record in place: header control runs become `sep` (stopping at the first
// key line), a trailing sep becomes a newline (or the end), then each control run in the
// body becomes one '\n' (a run of the same char stays separate lines).
extern "C" void func_003EB588(unsigned char* dst, unsigned char sep)
{
    unsigned char* buf = dst;
    unsigned char* src = dst;
    if (*src != 0) {
        do {
            if (*src < 0x20) {
                *dst++ = sep;
                while (*src != 0 && *src < 0x20)
                    src++;
                if ((*src == '=' || *src == ':') && src[1] < 0x20)
                    break;
            } else {
                *dst++ = *src++;
            }
        } while (*src != 0);
    }
    if (dst != buf && dst[-1] == sep)
        dst[-1] = (sep == '\n' || *src != 0) ? '\n' : 0;
    while (*src != 0) {
        if (*src < 0x20) {
            *dst++ = '\n';
            unsigned char runChar = *src++;
            while (*src != 0 && *src < 0x20 && *src != runChar)
                src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldFind);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_004963C0[];

extern "C" const unsigned char* cDirtysock_tag_TagFieldFind_impl(const unsigned char* pRecord, const unsigned char* pName) __asm__("cDirtysock_tag_TagFieldFind");
/**
 * Finds a named field in a tag record and returns its value.
 *
 * Looks for an '=' or ':' preceded by the name, compared case-insensitively, where the name starts the record
 * or follows whitespace. One space after the separator is skipped. The search ends at a lone separator between
 * whitespace and a control character.
 *
 * @param pRecord  the tag record, or 0
 * @param pName    the field name, or 0
 * @return the start of the field's value text, or 0 when the record or name is empty or the field is not found
 */
extern "C" const unsigned char* cDirtysock_tag_TagFieldFind_impl(const unsigned char* pRecord, const unsigned char* pName)
{
    const unsigned char* pTerm;
    const unsigned char* pData;
    const unsigned char* pRec;
    const unsigned char* pNam;

    if ((pRecord == 0) || (*pRecord == 0)) {
        return 0;
    }
    if ((pName == 0) || (*pName == 0)) {
        return 0;
    }

    for (pTerm = pName; *pTerm != 0; ++pTerm)
        ;
    --pTerm;

    pData = pRecord;
    for (;;) {
        if (*pData == 0) {
            return 0;
        }
        if ((*pData == '=') || (*pData == ':')) {
            if ((pData[1] < ' ') && (pData[-1] <= ' ')) {
                break;
            }
            pRec = pData - 1;
            pNam = pTerm;
            while ((pRec >= pRecord) && (D_004963C0[*pRec] == D_004963C0[*pNam])) {
                if (pNam == pName) {
                    if ((pRec == pRecord) || (pRec[-1] <= ' ')) {
                        return pData + ((pData[1] == ' ') ? 2 : 1);
                    }
                }
                pRec--;
                pNam--;
            }
        }
        ++pData;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003EB840);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char* cDirtysock_tag_TagFieldFind_B840(char* record, char* name) __asm__("cDirtysock_tag_TagFieldFind");
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void strcat_EB840(char* dst, const char* src) __asm__("strcat");

extern "C" char* func_003EB840(char* record, char* prefix, char* name)
{
    char buf[256];
    char* field;
    if (prefix == 0) {
        field = cDirtysock_tag_TagFieldFind_B840(record, name);
    } else if (name == 0) {
        field = cDirtysock_tag_TagFieldFind_B840(record, prefix);
    } else {
        strcpy(buf, prefix);
        strcat_EB840(buf, name);
        field = cDirtysock_tag_TagFieldFind_B840(record, buf);
    }
    return field;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003EB8B8);
#ifdef SKIP_ASM
extern "C" unsigned char* cDirtysock_tag_TagFieldFind(char* record, char* name);
extern char D_004A4A78[];

extern "C" unsigned char* func_003EB8B8(char* record)
{
    unsigned char* p = cDirtysock_tag_TagFieldFind(record, D_004A4A78);
    if (p != 0) {
        while ((unsigned int)(*p - 1) < 0x20U) {
            p++;
        }
        if (*p == 0) {
            p = 0;
        }
    }
    return p;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldDelete);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned char* cDirtysock_tag_TagFieldFind(char* record, char* name);

/**
 * Removes a named field from a tag record, shifting the rest of the record down.
 *
 * Deletes from the start of the field's name through its value and the control characters after it.
 *
 * @param record  the tag record to edit
 * @param name    the field name
 * @return 0 when the field was removed, -1 when it was not found
 */
extern "C" int cDirtysock_tag_TagFieldDelete(unsigned char* record, char* name)
{
    unsigned char* field = cDirtysock_tag_TagFieldFind((char*)record, name);
    unsigned char* dst;
    unsigned char* src;
    if (field == 0) {
        return -1;
    }
    dst = field;
    while (dst != record && dst[-1] > ' ') {
        dst--;
    }
    src = field;
    while (*src >= ' ') {
        src++;
    }
    while ((unsigned int)(*src - 1) < 0x1FU) {
        src++;
    }
    int curChar = *src;
    if (curChar != 0) {
        do {
            *dst = curChar;
            src++;
            curChar = *src;
            dst++;
        } while (curChar != 0);
    }
    *dst = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003EB9E8);
#ifdef SKIP_ASM
extern "C" unsigned char* cDirtysock_tag_TagFieldFind(char* record, char* name);
extern "C" unsigned int strlen(const char* s);

extern "C" int func_003EB9E8(unsigned char* record, int len, char* name, const signed char* value)
{
    unsigned char* nameEnd = cDirtysock_tag_TagFieldFind((char*)record, name);
    unsigned char* start;
    unsigned char* tail;
    int delta;

    if (nameEnd == 0) {
        return -1;
    }
    if (nameEnd[-1] == ' ') {
        nameEnd--;
    }
    start = nameEnd - 1;
    nameEnd = start;
    while (start != record && start[-1] >= 0x21) {
        start--;
    }
    delta = strlen((const char*)value) - (nameEnd - start);
    if (delta < 0) {
        unsigned char curChar = *nameEnd;
        if (curChar != 0) {
            unsigned char* dst = nameEnd + delta;
            *dst = curChar;
            while ((curChar = *nameEnd) != 0) {
                *dst = curChar;
            }
        }
        *nameEnd = 0;
    }
    if (delta > 0) {
        tail = nameEnd;
        if (*tail != 0) {
            do {
                tail++;
            } while (*tail != 0);
        }
        if (tail - record < len - delta) {
            return -1;
        }
        if (tail != nameEnd) {
            do {
                unsigned char movedChar = *tail;
                unsigned char* moveDst = tail + delta;
                tail--;
                *moveDst = movedChar;
            } while (tail != nameEnd);
        }
    }
    while (*value != 0) {
        *start = (unsigned char)*value;
        value++;
        start++;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldDupl);
#ifdef SKIP_ASM
/**
 * Copies a string into a fixed-size buffer, truncating it to fit.
 *
 * Copies at most @p len - 1 characters and adds the terminator when @p len is positive.
 *
 * @param dst  destination buffer
 * @param len  size of @p dst in bytes
 * @param src  NUL-terminated text to copy
 * @return the number of characters copied, not counting the terminator
 */
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

//100%
INCLUDE_ASM("dirtysock/tags", func_003EBBA8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int func_003EBBA8(const unsigned char* rec, char* buf, int len)
{
    int n = 0;
    if (len > 0)
        *buf = 0;
    if (rec == 0 || *rec == 0)
        return 0;
    for (const unsigned char* cursor = rec; *cursor != 0; cursor++) {
        if (*cursor != '=' && *cursor != ':')
            continue;
        if (cursor[1] < 0x20 && cursor[-1] <= 0x20)
            break;
        const unsigned char* nameStart = cursor;
        while (nameStart != rec && nameStart[-1] > 0x20)
            nameStart--;
        while (nameStart != cursor && n + 1 < len)
            buf[n++] = *nameStart++;
        buf[n] = 0;
        break;
    }
    return n;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tags", func_003EBCB0);
#ifdef SKIP_ASM
extern "C" int func_003EBCB0(char* record, int len, const char* value)
{
    char* cursor = record;
    char* end;
    for (;;) {
        unsigned char curChar = *cursor;
        if (curChar == 0) {
            break;
        }
        if (curChar == '=' || curChar == ':') {
            if ((unsigned char)cursor[1] < 0x20) {
                break;
            }
        }
        cursor++;
    }
    *cursor = 0;
    if (value == 0) {
        return 0;
    }
    end = record + len - 1;
    if (end - cursor < 5) {
        return -1;
    }
    *cursor++ = '=';
    *cursor++ = '\n';
    while (*value != 0 && cursor != end) {
        *cursor++ = *value++;
    }
    *cursor = 0;
    return 0;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tags", func_003EBD70);
#ifdef SKIP_ASM
extern signed char D_004A4A70;
extern signed char D_004A4A80[];

extern "C" int func_003EBD70(unsigned char* buf, int len)
{
    const signed char* s = D_004A4A80;
    int ret = -1;
    int n = 0;
    if (*buf != 0) {
        do {
            n++;
        } while (buf[n] != 0);
    }
    if (n > 0) {
        if (buf[n - 1] >= 0x20) {
            if (buf[n - 1] != D_004A4A70) {
                if (n < len - 1) {
                    buf[n] = (unsigned char)D_004A4A70;
                    n++;
                    buf[n] = 0;
                }
            }
        }
    }
    if (n + 4 < len) {
        while (*s != 0) {
            buf[n++] = (unsigned char)*s;
            s++;
        }
        ret = 0;
        buf[n] = 0;
    }
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetNumber);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

/**
 * Stores a signed integer in a tag record as decimal text.
 *
 * Negative values get a leading '-'. The most negative int stays negative when negated, so it is written as
 * "-0".
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param value   the number to store
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetNumber(char* record, int len, char* name, int value)
{
    char fieldBuf[256 + 32];
    unsigned char* num;
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);

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
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFlags);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern char str_at_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123[];

/**
 * Stores a bit set in a tag record as a string of flag letters.
 *
 * Bit 0 is written as '@', bit 1 as 'A', and so on through "@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123", one letter per
 * set bit, lowest bit first. TagFieldGetFlags() reads it back.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param value   the bits to store
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetFlags(char* record, int len, char* name, int value)
{
    char fieldBuf[256 + 32];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    const char* flags = str_at_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123;
    for (; (value != 0) && (*flags != 0); value >>= 1, flags++) {
        if (value & 1) {
            *data++ = *flags;
        }
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetAddress);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

/**
 * Stores an IPv4 address in a tag record as dotted decimal text, most significant byte first.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param addr    the address, as a 32-bit value
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetAddress(char* record, int len, char* name, unsigned int addr)
{
    int i;
    unsigned char bytes[4];
    char fieldBuf[256 + 32];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);

    bytes[3] = (unsigned char)addr;
    addr >>= 8;
    bytes[2] = (unsigned char)addr;
    addr >>= 8;
    bytes[1] = (unsigned char)addr;
    addr >>= 8;
    bytes[0] = (unsigned char)addr;
    for (i = 0; i < 4; i++) {
        unsigned char octet = bytes[i];
        if (i > 0)
            *data++ = '.';
        if (octet >= 10) {
            if (octet >= 100) {
                *data++ = '0' + octet / 100;
                octet %= 100;
            }
            *data++ = '0' + octet / 10;
            octet %= 10;
        }
        *data++ = '0' + octet;
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetFourCC);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

/**
 * Stores a four-character code in a tag record as text.
 *
 * Writes the code's bytes from the most significant down, skipping any byte that is a space or control
 * character or has its top bit set.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param value   the packed four-character code
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetFourCC(char* record, int len, char* name, int value)
{
    char fieldBuf[256 + 32];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    for (; value != 0; value <<= 8) {
        if (value > 0x20FFFFFF) {
            *data++ = (char)(value >> 24);
        }
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetString);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: the escaped '%' is stored through a wider stand-in, `long long pct = '%';` before the loop and `*data++ = (char)pct;`
// (same byte; the in-loop truncation is not a constant when loop.c runs, so combine_movables does not merge it into the SI
// `*value == '%'` compare constant, and cse2 folds it back to 37 in the preheader); plain form 97.15%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern int cDirtysock_tag_TagFieldDelete_s(char* record, char* name) __asm__("cDirtysock_tag_TagFieldDelete");
extern unsigned char D_00495FC0[];
extern unsigned char D_004960C0[];

extern "C" int cDirtysock_tag_TagFieldSetString_impl(char* record, int len, char* name, const unsigned char* value) __asm__("cDirtysock_tag_TagFieldSetString");
/**
 * Stores a string in a tag record, %xx-escaping the characters the format reserves.
 *
 * A null @p value deletes the field instead. The text is put in double quotes when it contains a space;
 * characters outside ' '..'~' and '=', '"', ':' and '%' become '%' plus two hex digits.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param value   the text to store, or 0 to delete the field
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result, or 0 when the field was deleted
 */
extern "C" int cDirtysock_tag_TagFieldSetString_impl(char* record, int len, char* name, const unsigned char* value)
{
    char fieldBuf[0xA00];
    char* data;
    const unsigned char* scan;

    if (value == 0)
    {
        cDirtysock_tag_TagFieldDelete_s(record, name);
        return 0;
    }
    data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);

    for (scan = value; (*scan != 0) && (*scan != ' '); ++scan)
        ;
    if (*scan == ' ')
        *data++ = '"';

    long long pct = '%';
    for (; *value != 0; ++value)
    {
        if ((*value < ' ') || (*value > '~') || (*value == '=') || (*value == '"') || (*value == ':') || (*value == '%'))
        {
            *data++ = (char)pct;
            *data++ = D_00495FC0[*value];
            *data++ = D_004960C0[*value];
        }
        else
        {
            *data++ = *value;
        }
    }

    if (*scan == ' ')
        *data++ = '"';
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetBinary);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern unsigned char D_00495FC0[];
extern unsigned char D_004960C0[];

/**
 * Stores binary data in a tag record as '$' followed by two lowercase hex digits per byte.
 *
 * TagFieldGetBinary() decodes it.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param value   the bytes to store
 * @param size    number of bytes
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetBinary(char* record, int len, char* name, const void* value, int size)
{
    const unsigned char* src = (const unsigned char*)value;
    char fieldBuf[4096 + 256];
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    *data++ = '$';
    for (; size > 0; --size) {
        *data++ = D_00495FC0[*src];
        *data++ = D_004960C0[*src++];
    }
    *data = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetStructure);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Tables are const (read-only): lets the scheduler hoist the table loads above the byte stores.
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern const unsigned char kHexHi[] __asm__("D_00495FC0");
extern const unsigned char kHexLo[] __asm__("D_004960C0");
extern const unsigned char kSafe[] __asm__("D_004964C0");

/**
 * Stores a C structure in a tag record as comma-separated values, laid out by a template string.
 *
 * Template items: an optional "#label=" (skipped), a decimal count, then a type letter: 'a' skips count bytes
 * (1 if no count), 'b', 'w' and 'l' read an 8-, 16- or 32-bit value written in hex ("-" plus the negated value
 * above 0xFFFF0000, nothing for zero), and 's' reads a count-byte string, %xx-escaping all but letters, digits,
 * '_', '=' and ':'. A '*' after an item repeats it. Trailing commas are trimmed; reading stops at the end of
 * the buffer.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param buffer  the structure to read
 * @param size    size of the structure in bytes; negative means 64 KiB
 * @param tmpl    the template describing the structure's fields
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
extern "C" int cDirtysock_tag_TagFieldSetStructure(char* record, int len, char* name, const char* buffer, int size, const char* tmpl)
{
    char fieldBuf[2560];
    const char* buf = buffer;
    const char* limit;
    char* out;
    int count;
    unsigned int val;
    int c;
    const unsigned char* str;

    val = 0;
    if (size < 0) {
        limit = buf + 65536;
    } else {
        limit = buf + size;
    }
    out = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    while (*tmpl != 0) {
        char typeChar;
        if (*tmpl == '#') {
            for (++tmpl; (*tmpl != 0) && (*tmpl++ != '='); ) {
            }
        }
        c = (unsigned char)*tmpl;
        for (count = 0; (unsigned int)(c - 48) < 10; c = (unsigned char)*tmpl) {
            tmpl++;
            count = count * 10 + (c & 15);
        }
        c = 0;
        str = 0;
        if (*tmpl == 'a') {
            if (count != 0) {
                buf += count;
            } else {
                buf += 1;
            }
        }
        typeChar = (char)*(unsigned char*)tmpl;
        if (typeChar == 'b') {
            val = *(const unsigned char*)buf;
            c = 2;
            buf += 1;
        }
        if (typeChar == 'w') {
            val = *(const unsigned short*)buf;
            c = 4;
            buf += 2;
        }
        if (typeChar == 'l') {
            val = *(const unsigned int*)buf;
            c = 8;
            buf += 4;
        }
        if (typeChar == 's') {
            str = (const unsigned char*)buf;
            buf += count;
        }
        if (limit < buf) {
            break;
        }
        if (c > 0) {
            char* digitsEnd;
            char* digitPtr;
            if (val > 0xFFFF0000U) {
                val = -val;
                *out++ = '-';
            }
            digitsEnd = out + c;
            digitPtr = digitsEnd;
            *digitsEnd = 0;
            for (; val != 0; val >>= 4) {
                *--digitPtr = kHexLo[val & 15];
            }
            if (!(out != digitPtr)) {
                out = digitsEnd;
            } else {
                while (*digitPtr != 0) {
                    *out++ = *digitPtr++;
                }
                *out++ = ',';
            }
        }
        if (str != 0) {
            while (*str != 0) {
                val = *str++;
                if (!(kSafe[val] == 0)) {
                    *out++ = val;
                } else {
                    *out++ = '%';
                    *out++ = kHexHi[val];
                    *out++ = kHexLo[val];
                }
            }
            *out++ = ',';
        }
        ++tmpl;
        if (*tmpl == '*') {
            --tmpl;
        }
    }
    if (out[-1] == ',') {
        do {
            out--;
        } while (out[-1] == ',');
        *out = 0;
    } else {
        *out = 0;
    }
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetCrypt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern const unsigned char kHexHi[] __asm__("D_00495FC0");
extern const unsigned char kHexLo[] __asm__("D_004960C0");
extern char str_GS[];

/**
 * Stores a string in a tag record lightly obfuscated with a key, as '$' followed by hex digits.
 *
 * Each byte is XORed with the previous output byte, rotated left 3 bits and XORed with the next key character
 * (the key repeats; an empty or null key means "GS"). TagFieldGetCrypt() reverses it.
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param key     the key string, or 0
 * @param data    the NUL-terminated string to store
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result
 */
// Same key handling as the matched TagFieldGetCrypt: a working copy kbase of the key, defaulted when empty.
extern "C" int cDirtysock_tag_TagFieldSetCrypt(char* record, int len, char* name, const unsigned char* key, const unsigned char* data)
{
    char fieldBuf[4096 + 256];
    const unsigned char* kbase = key;
    const unsigned char* cursor = data;
    const unsigned char* k;
    unsigned int cipherByte;
    unsigned int plainByte;
    char* out;
    if (kbase == 0 || *kbase == 0) {
        kbase = (const unsigned char*)str_GS;
    }
    out = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    cipherByte = 0;
    *out++ = '$';
    k = kbase;
    plainByte = *data;
    while (plainByte != 0) {
        unsigned int x = plainByte ^ cipherByte;
        unsigned int kc = *k++;
        cursor++;
        if (*k == 0) {
            k = kbase;
        }
        cipherByte = (kc ^ ((x << 3) | (x >> 5))) & 0xFF;
        *out++ = kHexHi[cipherByte];
        *out++ = kHexLo[cipherByte];
        plainByte = *cursor;
    }
    *out = 0;
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldSetEpoch);
#ifdef SKIP_ASM
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_d_pct_d_pct_d_pct_d_pct_02d_pct_02d[];

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

/**
 * Stores a time in a tag record as "year.month.day hour:mm:ss".
 *
 * @param record  the tag record to edit
 * @param len     size of the record buffer in bytes
 * @param name    the field name (0 clears the record first)
 * @param epoch   seconds since 1970; 0 means the current time (func_003EF088)
 * @return cDirtysock_tag__TagFieldSetupTerm()'s result, or -1 when the time cannot be converted
 */
// PORT: epoch is widened to 64-bit `unsigned long` (zero-extended) for ds_secstotime.
extern "C" int cDirtysock_tag_TagFieldSetEpoch(char* record, int len, char* name, unsigned int epoch)
{
    sDsTmK2 tm;
    char fieldBuf[256 + 32];
    sDsTmK2* timeParts;
    unsigned long secs = epoch;

    if (secs == 0)
        secs = func_003EF088();
    timeParts = func_003EAEB8(&tm, secs);
    if (timeParts == 0)
        return -1;
    char* data = cDirtysock_tag__TagFieldSetupAppend(record, fieldBuf, name);
    sprintf(data, str_pct_d_pct_d_pct_d_pct_d_pct_02d_pct_02d, timeParts->tm_year + 1900, timeParts->tm_mon + 1, timeParts->tm_mday, timeParts->tm_hour, timeParts->tm_min, timeParts->tm_sec);
    return cDirtysock_tag__TagFieldSetupTerm(record, len, fieldBuf);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", func_003EC8B8);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int value, int size);
// PORT: ds_tmtosecs returns a 64-bit `unsigned long`.
extern "C" unsigned long func_003EB090(sDsTmK2* tm);

extern "C" int func_003EC8B8(char* record, int len, char* name, int year, int mon, int day, int hour, int min, int sec, int adj)
{
    sDsTmK2 tm;
    int secs;

    func_00416210(&tm, 0, sizeof(tm));
    tm.tm_isdst = -1;
    tm.tm_year = year - 1900;
    tm.tm_mon = mon - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = sec;
    secs = (int)func_003EB090(&tm);
    return cDirtysock_tag_TagFieldSetEpoch(record, len, name, secs != 0 ? secs - adj : 0);
}
#endif
//100%
INCLUDE_ASM("dirtysock/tags", func_003EC9A0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int value, int size);
// PORT: ds_tmtosecs returns a 64-bit `unsigned long`.
extern "C" unsigned long func_003EB090(sDsTmK2* tm);

extern "C" int func_003EC9A0(char* record, int len, char* name, int date, int time, int adj)
{
    sDsTmK2 tm;
    int secs;

    func_00416210(&tm, 0, sizeof(tm));
    tm.tm_isdst = -1;
    tm.tm_year = ((date >> 16) & 0xFFFF) - 1900;
    tm.tm_mon = ((date >> 8) & 0xFF) - 1;
    tm.tm_mday = date & 0xFF;
    tm.tm_hour = (time >> 16) & 0xFF;
    tm.tm_min = (time >> 8) & 0xFF;
    tm.tm_sec = time & 0xFF;
    secs = (int)func_003EB090(&tm);
    return cDirtysock_tag_TagFieldSetEpoch(record, len, name, secs != 0 ? secs - adj : 0);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003ECA88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char* cDirtysock_tag_TagFieldFind_CA88(char* record, char* name) __asm__("cDirtysock_tag_TagFieldFind");
extern "C" int cDirtysock_tag__TagFieldSetupTerm(char* record, int len, char* temp);

extern "C" int func_003ECA88(char* record, int len, char* name, char* data)
{
    char* field = cDirtysock_tag_TagFieldFind_CA88(data, name);
    int result = 0;
    if (field != 0) {
        while (field != data) {
            if ((unsigned char)field[-1] < 0x20U) {
                break;
            }
            field--;
        }
        result = cDirtysock_tag__TagFieldSetupTerm(record, len, field);
    }
    return result;
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
        unsigned char curChar = *data;
        if (curChar == 0)
            break;
        // skip whitespace
        if (curChar <= ' ') {
            ++data;
            continue;
        }
        // stop at a separator
        if ((curChar == '=') || (curChar == ':'))
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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003ECBD8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_003ECBD8_va_list;
#define func_003ECBD8_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
#define func_003ECBD8_va_arg(ap, T) (ap += 8, *(T*)(ap - 8))

extern "C" int cDirtysock_tag_TagFieldSetString(char* buf, int size, const char* key, const char* value);
extern char D_004A4A90[];

// TagFieldPrintf-style formatter: copies fmt into buf, turning whitespace runs into the
// field separator and %s/%d/%a/%f/%e/%r into tag fields from the arguments.
extern "C" int func_003ECBD8(char* buf, int len, const char* fmt, ...)
{
    func_003ECBD8_va_list args;
    func_003ECBD8_va_start(args);
    int size = len;
    unsigned char c = *fmt++;
    while (c != 0 && len >= 2) {
        if (c <= ' ') {
            *buf++ = D_004A4A70;
            len--;
            while (*fmt != 0 && *fmt <= ' ')
                fmt++;
        } else if (c != '%') {
            *buf++ = c;
            len--;
        } else if (*fmt != 0) {
            int n = 0;
            c = *fmt++;
            if (buf[-1] == '=') {
                buf--;
                len++;
            }
            *buf = 0;
            if (c == 's') {
                n = cDirtysock_tag_TagFieldSetString(buf, len, D_004A4A90, func_003ECBD8_va_arg(args, char*));
            } else if (c == 'd') {
                n = cDirtysock_tag_TagFieldSetNumber(buf, len, D_004A4A90, func_003ECBD8_va_arg(args, int));
            } else if (c == 'a') {
                n = cDirtysock_tag_TagFieldSetAddress(buf, len, D_004A4A90, func_003ECBD8_va_arg(args, unsigned int));
            } else if (c == 'f') {
                n = cDirtysock_tag_TagFieldSetFlags(buf, len, D_004A4A90, func_003ECBD8_va_arg(args, int));
            } else if (c == 'e') {
                n = cDirtysock_tag_TagFieldSetEpoch(buf, len, D_004A4A90, func_003ECBD8_va_arg(args, unsigned int));
            } else if (c == 'r') {
                const char* r = func_003ECBD8_va_arg(args, char*);
                while (*r != 0 && len > 0) {
                    *buf++ = *r++;
                    len--;
                }
            }
            if (n > 0) {
                buf += n;
                len -= n;
            }
        }
        c = *fmt++;
    }
    if (len > 0)
        *buf = 0;
    return size - len;
}
#endif
//100%
INCLUDE_ASM("dirtysock/tags", func_003ECE38);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Finds the next "=" or ":" field separator in p and copies the following value into out (or dflt when there is none); returns the value start or 0.
extern "C" unsigned char* func_003ECE38(unsigned char* src, int unused, char* out, int size, const char* dflt)
{
    int i;
    unsigned char* cursor = src;
    while (*cursor != 0) {
        if ((*cursor == '=' || *cursor == ':') && cursor[1] < 0x20) {
            if (cursor[1] == 0)
                break;
            cursor += 2;
            if (out != 0) {
                size--;
                for (i = 0; i < size && cursor[i] != 0; i++)
                    out[i] = cursor[i];
                out[i] = 0;
            }
            return cursor;
        }
        cursor++;
    }
    if (out != 0 && dflt != 0) {
        size--;
        for (i = 0; i < size && dflt[i] != 0; i++)
            out[i] = dflt[i];
        out[i] = 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetNumber);
#ifdef SKIP_ASM
/**
 * Parses a signed decimal integer from a tag field value.
 *
 * Accepts one leading '+' or '-' and stops at the first non-digit, with no overflow check. Only a null @p data
 * yields @p defval: text without digits parses as 0.
 *
 * @param data    field text, or 0
 * @param defval  value returned when @p data is 0
 * @return the parsed value, or @p defval
 */
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

/**
 * Parses a string of flag letters from a tag field value back into a bit set.
 *
 * ORs together the bit of each letter (the D_004965E0 table, the inverse of TagFieldSetFlags()'s letters) up to
 * the first character that is not one. A non-null @p data with no flag letters gives 0, not @p defval.
 *
 * @param data    field text, or 0
 * @param defval  value returned when @p data is 0
 * @return the bit set, or @p defval
 */
extern "C" int cDirtysock_tag_TagFieldGetFlags(const char* data, int defval)
{
    if (data != 0) {
        int flags = 0;
        int flagBit;
        while ((flagBit = D_004965E0[*data]) != 0) {
            flags |= flagBit;
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
/**
 * Parses a dotted-decimal IPv4 address from a tag field value.
 *
 * Digits accumulate into the low byte and each '.' shifts the address left by 8 bits; parsing stops at any other
 * character. There is no range or format check, and only a null @p data yields @p defval.
 *
 * @param data    field text such as "10.0.0.1", or 0
 * @param defval  value returned when @p data is 0
 * @return the address with the first number in the top byte, or @p defval
 */
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
/**
 * Packs the start of a tag field value into a four-character code.
 *
 * Shifts in up to four characters, stopping at the first one outside '!'..'~'; a shorter code keeps space
 * characters (0x20) in its unused high bytes.
 *
 * @param data    field text, or 0
 * @param defval  value returned when @p data is 0 or does not start with a character in '!'..'~'
 * @return the packed code, or @p defval
 */
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

//100%
INCLUDE_ASM("dirtysock/tags", func_003ED0F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int strlen_D0F8(const char* s) __asm__("strlen");
extern unsigned char D_004961C0[];
extern unsigned char D_004962C0[];

// TagFieldGetString variant that first skips iIndex fields split by iDelim.
extern "C" int func_003ED0F8(const char* pData, char* pBuffer, int iLength, const char* pDefval, int iIndex, int iDelim)
{
    int iSize;
    unsigned char cQuote;
    const unsigned char* p;

    if (pData != 0) {
        if ((*pData == '"') || (*pData == '\'')) {
            cQuote = *pData++;
        } else {
            cQuote = 0;
        }
        for (; iIndex > 0; --iIndex) {
            for (;;) {
                char c = *pData;
                if ((c == cQuote) || (c < ' ') || (c == iDelim)) {
                    break;
                }
                ++pData;
            }
            if (*pData++ != iDelim) {
                pData = 0;
                break;
            }
        }
    }
    if (pData == 0) {
        if (pDefval == 0) {
            return -1;
        }
        if (pBuffer == 0) {
            return strlen_D0F8(pDefval);
        }
        for (iSize = 1; (iSize < iLength) && (*pDefval != 0); ++iSize) {
            *pBuffer++ = *pDefval++;
        }
        *pBuffer = 0;
        return iSize - 1;
    }
    if (pBuffer == 0) {
        p = (const unsigned char*)pData;
        for (iSize = 0; (*p != cQuote) && (*p >= ' ') && (*p != iDelim); ++iSize) {
            if ((p[0] == '%') && (p[1] >= ' ') && (p[2] >= ' ')) {
                p += 2;
            }
            ++p;
        }
        return iSize;
    }
    if (iLength <= 0) {
        return -1;
    }
    p = (const unsigned char*)pData;
    for (iSize = 1; (iSize < iLength) && (*p != cQuote) && (*p >= ' ') && (*p != iDelim);) {
        if ((p[0] == '%') && (p[1] == '%')) {
            *pBuffer++ = '%';
            p += 2;
            ++iSize;
        } else if ((p[0] == '%') && (p[1] >= ' ') && (p[2] >= ' ')) {
            ++iSize;
            *pBuffer++ = D_004961C0[p[1]] | D_004962C0[p[2]];
            p += 3;
        } else {
            ++iSize;
            *pBuffer++ = *p++;
        }
    }
    *pBuffer = 0;
    return iSize - 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetString);
#ifdef SKIP_ASM
/**
 * Copies a tag field value into a buffer, removing quotes and decoding %xx escapes.
 *
 * A value may be wrapped in double or single quotes; it ends at the closing quote or a control character, and
 * "%%" gives '%'. With a null @p pData the default string is copied instead. With a null @p pBuffer nothing is
 * copied and the decoded length is returned.
 *
 * @param pData    field text, or 0
 * @param pBuffer  destination buffer, or 0 to measure
 * @param iLength  size of the buffer, including the terminator
 * @param pDefval  default string used when @p pData is 0, or 0
 * @return the number of characters copied (or needed), or -1 when there is no data and no default, or
 *         @p iLength is not positive
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Copies a tag field value (quoted or bare, with %xx decoding) into pBuffer, or the default.
extern "C" int cDirtysock_tag_TagFieldGetString(const char* pData, char* pBuffer, int iLength, const char* pDefval)
{
    int iSize;
    unsigned char cQuote;
    const unsigned char* cursor;

    if (pData == 0) {
        if (pDefval == 0) {
            return -1;
        }
        if (pBuffer == 0) {
            return strlen(pDefval);
        }
        for (iSize = 1; (iSize < iLength) && (*pDefval != 0); ++iSize) {
            *pBuffer++ = *pDefval++;
        }
        *pBuffer = 0;
        return iSize - 1;
    }
    if ((*pData == '"') || (*pData == '\'')) {
        cQuote = *pData++;
    } else {
        cQuote = 0;
    }
    if (pBuffer == 0) {
        cursor = (const unsigned char*)pData;
        for (iSize = 0; (*cursor != cQuote) && (*cursor >= ' '); ++iSize) {
            if ((cursor[0] == '%') && (cursor[1] >= ' ') && (cursor[2] >= ' ')) {
                cursor += 2;
            }
            ++cursor;
        }
        return iSize;
    }
    if (iLength <= 0) {
        return -1;
    }
    cursor = (const unsigned char*)pData;
    for (iSize = 1; (iSize < iLength) && (*cursor != cQuote) && (*cursor >= ' ');) {
        if ((cursor[0] == '%') && (cursor[1] == '%')) {
            *pBuffer++ = '%';
            cursor += 2;
            ++iSize;
        } else if ((cursor[0] == '%') && (cursor[1] >= ' ') && (cursor[2] >= ' ')) {
            ++iSize;
            *pBuffer++ = D_004961C0[cursor[1]] | D_004962C0[cursor[2]];
            cursor += 3;
        } else {
            ++iSize;
            *pBuffer++ = *cursor++;
        }
    }
    *pBuffer = 0;
    return iSize - 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetBinary);
#ifdef SKIP_ASM
extern unsigned char D_004961C0[];
extern unsigned char D_004962C0[];

/**
 * Decodes a '$'-prefixed hex tag field value into bytes.
 *
 * With a null @p buffer nothing is written and the number of encoded bytes is returned.
 *
 * @param data    field text, or 0
 * @param buffer  destination buffer, or 0 to measure
 * @param len     size of the buffer in bytes
 * @return the number of bytes decoded (or available), or -1 when @p data is 0, does not start with '$', or
 *         @p len is not positive
 */
extern "C" int cDirtysock_tag_TagFieldGetBinary(const char* data, void* buffer, int len)
{
    int count;
    const char* cursor;
    char* buf = (char*)buffer;

    if ((data == 0) || (*data != '$')) {
        return -1;
    }
    if (buf == 0) {
        for (count = 0, cursor = data + 1; ((unsigned char)cursor[0] >= '0') && ((unsigned char)cursor[1] >= '0'); count++) {
            cursor += 2;
        }
        return count;
    }
    cursor = data + 1;
    if (len <= 0) {
        return -1;
    }
    for (count = 0; (count < len) && ((unsigned char)cursor[0] >= '0') && ((unsigned char)cursor[1] >= '0'); count++) {
        *buf++ = D_004961C0[(unsigned char)cursor[0]] | D_004962C0[(unsigned char)cursor[1]];
        cursor += 2;
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetStructure);
#ifdef SKIP_ASM
extern char D_004A4A90[];

/**
 * Decodes comma-separated tag value text into a C structure, laid out by a template string.
 *
 * The template is TagFieldSetStructure()'s: 'a' skips bytes, 'b', 'w' and 'l' read up to 2, 4 or 8 hex digits
 * (a leading '-' negates) and 's' a count-byte string, undoing %xx escapes and zero-filling. A null @p data
 * reads as empty. A null @p buffer decodes into a 1 KiB scratch array, but the limit was taken from the null
 * pointer, so decoding stops after the first item.
 *
 * @param data    the value text to decode, or 0
 * @param buffer  the structure to fill, or 0 to only measure
 * @param len     size of @p buffer in bytes; negative allows 65535
 * @param tmpl    the template describing the structure's fields
 * @return the number of structure bytes filled or skipped
 */
extern "C" int cDirtysock_tag_TagFieldGetStructure(const char* data, void* buffer, int len, const char* tmpl)
{
    int count;
    int size;
    int sign;
    unsigned int value;
    char* buf = (char*)buffer;
    char* limit;
    char scratchBuf[1024];

    if (len < 0) {
        limit = buf + 65535;
    } else {
        limit = buf + len;
    }
    if (data == 0) {
        data = D_004A4A90;
    }
    if (buf == 0) {
        buf = (char*)(buffer = scratchBuf);
    }
    while (*tmpl != 0) {
        if (*tmpl == '#') {
            for (++tmpl; (*tmpl != 0) && (*tmpl++ != '='); ) {
            }
        }
        for (count = 0; (*tmpl >= '0') && (*tmpl <= '9'); ++tmpl) {
            count = (count * 10) + (*tmpl & 15);
        }
        if (*tmpl == 'a') {
            if (count != 0) {
                buf += count;
            } else {
                buf += 1;
            }
        }
        size = 0;
        if (*tmpl == 'b') {
            size = 2;
        }
        if (*tmpl == 'w') {
            size = 4;
        }
        if (*tmpl == 'l') {
            size = 8;
        }
        if (size > 0) {
            sign = *data;
            if (sign == '-') {
                ++data;
            }
            for (value = 0; (size > 0) && (*data >= '0'); --size) {
                value = (value << 4) | D_004962C0[*data++];
            }
            if ((size > 0) && (*data == ',')) {
                ++data;
            }
            if (sign == '-') {
                value = -value;
            }
            if (*tmpl == 'b') {
                *buf++ = value;
            }
            if (*tmpl == 'w') {
                *(short*)buf = value;
                buf += 2;
            }
            if (*tmpl == 'l') {
                *(int*)buf = value;
                buf += 4;
            }
        }
        if ((*tmpl == 's') && (count > 0)) {
            for (size = 0; ((*data >= '0') || (*data == '%')) && (size + 1 < count); ++size) {
                if (*data == '%') {
                    buf[size] = D_004961C0[data[1]] | D_004962C0[data[2]];
                    data += 3;
                } else {
                    buf[size] = *data++;
                }
            }
            for (; size < count; ++size) {
                buf[size] = 0;
            }
            if (*data == ',') {
                ++data;
            }
            buf += size;
        }
        if (buf >= limit) {
            break;
        }
        ++tmpl;
        if (*tmpl == '*') {
            --tmpl;
        }
    }
    return buf - (char*)buffer;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetCrypt);
#ifdef SKIP_ASM
extern unsigned char D_004961C0[];
extern unsigned char D_004962C0[];
extern char str_GS[];
extern "C" int cDirtysock_tag_TagFieldGetString(const char* data, char* buf, int len, const char* defval);

/**
 * Decodes a string stored by TagFieldSetCrypt(), or copies a plain value through TagFieldGetString().
 *
 * A value starting with '$' is hex decoded and de-obfuscated with the key (an empty or null key means "GS");
 * anything else is treated as plain text, with @p defval as its default.
 *
 * @param data    field text, or 0
 * @param buf     destination buffer, or 0 to measure
 * @param len     size of the buffer, including the terminator
 * @param key     the key string, or 0
 * @param defval  default string for the plain-text path
 * @return the number of characters written (or encoded bytes when @p buf is 0), or -1 when @p data is 0 or
 *         @p len is not positive
 */
extern "C" int cDirtysock_tag_TagFieldGetCrypt(const char* data, char* buf, int len, const unsigned char* key, const char* defval)
{
    int count;
    const char* cursor;
    const unsigned char* kbase = key;
    const unsigned char* k;
    int prev;
    if (data == 0) {
        return -1;
    }
    if (*data != '$') {
        return cDirtysock_tag_TagFieldGetString(data, buf, len, defval);
    }
    if (kbase == 0 || *kbase == 0) {
        kbase = (const unsigned char*)str_GS;
    }
    if (buf == 0) {
        for (count = 0, cursor = data + 1; (unsigned char)cursor[0] >= '0' && (unsigned char)cursor[1] >= '0'; count++) {
            cursor += 2;
        }
        return count;
    }
    count = 1;
    if (len <= 0) {
        return -1;
    }
    cursor = data + 1;
    k = kbase;
    prev = 0;
    for (; count < len && (unsigned char)cursor[0] >= '0' && (unsigned char)cursor[1] >= '0'; count++) {
        unsigned int decoded = *k ^ (D_004961C0[(unsigned char)cursor[0]] | D_004962C0[(unsigned char)cursor[1]]);
        if (*++k == 0) {
            k = kbase;
        }
        *buf++ = ((decoded << 5) | (decoded >> 3)) ^ prev;
        int hi = D_004961C0[(unsigned char)cursor[0]];
        int lo = D_004962C0[(unsigned char)cursor[1]];
        prev = lo | hi;
        cursor += 2;
    }
    *buf = 0;
    return count - 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tags", cDirtysock_tag_TagFieldGetTime);
#ifdef SKIP_ASM
// _TagFieldParseNumber / memset / ds_tmtosecs
extern "C" char* func_003EAE68(const char* data, int* value);
extern "C" void* func_00416210(void* dst, int value, int size);
// PORT: ds_tmtosecs returns a 64-bit `unsigned long`.
extern "C" unsigned long func_003EB090(sDsTmK2* tm);

/**
 * Parses a time from a tag field value, as seconds since 1970.
 *
 * Accepts '$' plus hex digits, a plain decimal number of seconds, or "year.month.day hour:min:sec" (the date
 * may use '-' instead of '.'). A date outside 1970-2099 or a field out of range is rejected.
 *
 * @param data    field text, or 0
 * @param defval  value used when nothing could be parsed
 * @return the time in seconds; when parsing gives 0, @p defval, or the current time (func_003EF088) when that
 *         is 0 too
 */
extern "C" unsigned int cDirtysock_tag_TagFieldGetTime(char* data, unsigned int defval)
{
    sDsTmK2 tm;
    int value;
    unsigned int result = 0;

    if (data != 0) {
        if (*data == '$') {
            for (++data; *data >= '0'; ++data) {
                result = (result << 4) | D_004962C0[*data];
            }
        } else if ((*data >= '0') && (*data <= '9')) {
            const char* parseEnd = func_003EAE68(data, &value);
            if (*parseEnd <= ' ') {
                result = value;
            } else if ((*data >= '0') && (*data <= '9')) {
                func_00416210(&tm, 0, sizeof(tm));
                tm.tm_isdst = -1;
                data = func_003EAE68(data, &tm.tm_year);
                if ((unsigned char)(*data - '-') <= 1) {
                    data++;
                }
                data = func_003EAE68(data, &tm.tm_mon);
                if ((unsigned char)(*data - '-') <= 1) {
                    data++;
                }
                data = func_003EAE68(data, &tm.tm_mday);
                if (*data == ' ') {
                    data++;
                }
                data = func_003EAE68(data, &tm.tm_hour);
                if (*data == ':') {
                    data++;
                }
                data = func_003EAE68(data, &tm.tm_min);
                if (*data == ':') {
                    data++;
                }
                data = func_003EAE68(data, &tm.tm_sec);
                if ((tm.tm_year < 1970) || (tm.tm_year > 2099) || (tm.tm_mon < 1) || (tm.tm_mon > 12) || (tm.tm_mday < 1) || (tm.tm_mday > 31)) {
                    tm.tm_year = 0;
                }
                if ((tm.tm_hour < 0) || (tm.tm_hour > 23) || (tm.tm_min < 0) || (tm.tm_min > 59) || (tm.tm_sec < 0) || (tm.tm_sec > 61)) {
                    tm.tm_year = 0;
                }
                if (tm.tm_year != 0) {
                    tm.tm_year -= 1900;
                    tm.tm_mon -= 1;
                    result = func_003EB090(&tm);
                }
            }
        }
    }
    if (result == 0) {
        result = defval;
        if (result == 0) {
            result = func_003EF088();
        }
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tags", func_003EDD70);
#ifdef SKIP_ASM
extern "C" unsigned int cDirtysock_tag_TagFieldGetTime(char* data, unsigned int dflt);

// TagFieldGetDate-style accessor: split a time tag into calendar fields.
extern "C" int func_003EDD70(char* data, int* year, int* month, int* day, int* hour, int* min, int* sec, int offset)
{
    sDsTmK2 tm;
    sDsTmK2* calendar;
    unsigned int secs = cDirtysock_tag_TagFieldGetTime(data, 1);
    if (secs < 2)
        return -1;
    calendar = func_003EAEB8(&tm, secs + offset);
    if (calendar == 0)
        return -1;
    if (year)
        *year = calendar->tm_year + 1900;
    if (month)
        *month = calendar->tm_mon + 1;
    if (day)
        *day = calendar->tm_mday;
    if (hour)
        *hour = calendar->tm_hour;
    if (min)
        *min = calendar->tm_min;
    if (sec)
        *sec = calendar->tm_sec;
    return 0;
}
#endif


//100%
INCLUDE_ASM("dirtysock/tags", func_003EDE80);
#ifdef SKIP_ASM
extern "C" int func_003EDE80(char* data, unsigned int* date, unsigned int* time, int offset)
{
    sDsTmK2 tm;
    sDsTmK2* calendar;
    unsigned int secs = cDirtysock_tag_TagFieldGetTime(data, 1);
    if (secs < 2)
        return -1;
    calendar = func_003EAEB8(&tm, secs + offset);
    if (calendar == 0)
        return -1;
    if (date)
        *date = ((calendar->tm_year + 1900) << 16) | ((calendar->tm_mon + 1) << 8) | calendar->tm_mday;
    if (time)
        *time = (calendar->tm_hour << 16) | (calendar->tm_min << 8) | calendar->tm_sec;
    return 0;
}
#endif
