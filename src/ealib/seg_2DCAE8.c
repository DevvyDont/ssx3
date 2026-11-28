#include "common.h"
#include "ealib/hlafile.h"
#include "ealib/filesys.h"
#include "ealib/hlsfile.h"
#include "ealib/lobbyapi.h"
#include "ealib/locale.h"
#include "ealib/netgamelink.h"
#include "ealib/stream.h"
#include "ealib/threads.h"
//100%
INCLUDE_ASM("ealib/seg_2DCAE8", getstringidbyindex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char *bsearch_BAE8(void *, void *, unsigned int, int, void *) __asm__("bsearch");
extern char compare_BAE8[] __asm__("compare");
#include "ealib/printstr.h"
int getstringidbyindex_impl(short key, unsigned char *p) __asm__("getstringidbyindex");
/**
 * Looks a string id up in a locale file's index: returns the entry's string number (its second little-endian halfword),
 * or -1 when the id is not there.
 *
 * Binary search (bsearch, comparator compare) over the index's 4-byte entries from +0x10, count at +0x8.
 *
 * @param key  the string id
 * @param indexTable  the index chunk
 * @return the string number, or -1
 */
int getstringidbyindex_impl(short key, unsigned char *indexTable) {
    short k = key;
    unsigned char *n = indexTable + 8;
    unsigned char *found;
    found = bsearch_BAE8(&k, indexTable + 0x10, (n[3] << 24) | (n[2] << 16) | (n[1] << 8) | n[0], 4, compare_BAE8);
    if (found == 0) {
        return -1;
    }
    found += 2;
    return (found[1] << 8) | found[0];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", LOCALE_getstr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int getstringidbyindex_BB68(int, unsigned char *) __asm__("getstringidbyindex");
/* little-endian 32- and 16-bit file fields, read byte by byte */
static inline unsigned rd_BB68(const void *field) {
    const unsigned char *p = field;
    return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}
static inline unsigned rd16_BB68(const void *field) {
    const unsigned char *p = field;
    return p[1] << 8 | p[0];
}
/**
 * Returns string @p idx of a locale file's current language, or 0 when it is out of range.
 *
 * A file with an index (header flags bit 0) maps @p idx through it first (getstringidbyindex; the index chunk follows
 * the header chunk, chunksize bytes in). The language chunk is entry [languageid] of the offset table after the header;
 * its string offsets follow it, stringcount of them. All values are little-endian.
 *
 * @param table  the locale file
 * @param idx  the string id (number, or hashed id when indexed)
 * @return the string, or 0
 */
unsigned char *LOCALE_getstr(unsigned char *table, int idx) {
    LOCALE_HEADER *header = (LOCALE_HEADER *)table;
    LOCALE_LANGUAGE *bank;
    unsigned char *str;
    if (rd_BB68(&header->flags) & 1) {
        idx = getstringidbyindex_BB68(idx & 0xFFFF, table + rd_BB68(&header->chunksize));
    }
    // ORIGINAL-PTR: a locale file blob: the per-language offset table follows the header chunk and each language's
    // string offset table follows its chunk; the offsets are byte offsets from the file / the language chunk.
    bank = (LOCALE_LANGUAGE *)(table + rd_BB68((unsigned int *)(header + 1) + rd16_BB68(&header->languageid)));
    str = (unsigned char *)bank;
    if (idx >= 0 && (unsigned)idx < rd_BB68(&bank->stringcount)) {
        str += rd_BB68((unsigned int *)(bank + 1) + idx);
    } else {
        str = 0;
    }
    return str;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LOCALE_create);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged (a locale file needs no set-up). No callers.
 *
 * @param value  the value
 * @return @p value
 */
int LOCALE_create(int value) {
    return value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_length);
#ifdef SKIP_ASM
/**
 * Counts the characters of a zero-terminated 16-bit (wide) string.
 *
 * @param str  the wide string
 * @return the number of 16-bit characters before the terminating 0
 */
int USTR_length(unsigned short *str) {
    int var_3;
    unsigned short *var_4;

    var_4 = str;
    var_3 = 0;
    if (*var_4 != 0) {
        do {
            var_4 += 1;
            var_3 += 1;
        } while (*var_4 != 0);
    }
    return var_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_findchar);
#ifdef SKIP_ASM
/**
 * Returns the index of the first @p wanted character in a UCS-2 string, or 0xFFFF when it is absent.
 *
 * @param str  the string
 * @param wanted  the character
 * @return the index, or 0xFFFF
 */
int USTR_findchar(unsigned short *str, int wanted) {
    int i;
    unsigned short key = wanted;

    i = 0;
    if (*str != 0) {
        do {
            if (*str == key) {
                return i;
            }
            str++;
            i++;
        } while (*str != 0);
    }
    return 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_find);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int USTR_length_BD18(const unsigned short *) __asm__("USTR_length");
extern int USTR_ncompare_BD18(const unsigned short *, const unsigned short *, int) __asm__("USTR_ncompare");

/* Index of sub in s (UCS-2), or 0xFFFF. */
int USTR_find_impl(const unsigned short *src, const unsigned short *target) __asm__("USTR_find");
/**
 * Returns the index of the first occurrence of @p target in UCS-2 string @p src, or 0xFFFF.
 *
 * Compares @p target's length (USTR_length) at each position with USTR_ncompare.
 *
 * @param src  the string to search
 * @param target  the string to find
 * @return the index, or 0xFFFF
 */
int USTR_find_impl(const unsigned short *src, const unsigned short *target) {
    int n = USTR_length_BD18(target);
    int i;
    for (i = 0; src[i] != 0; i++) {
        if (USTR_ncompare_BD18(&src[i], target, n) == 0) {
            return i;
        }
    }
    return 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_compare);
#ifdef SKIP_ASM
extern void USTR_ncompare(int, int, int);

/**
 * Compares two UCS-2 strings: USTR_ncompare with no length limit (0x7FFFFFFF). Declared void, but its callers (e.g.
 * cMoviePlayer's file list) use the result left in the return register.
 *
 * @param lhs  the first string
 * @param rhs  the second string
 */
void USTR_compare(int lhs, int rhs) {
    USTR_ncompare(lhs, rhs, 0x7FFFFFFF);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_ncompare);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* UCS-2 strncmp. */
int USTR_ncompare_impl(const unsigned short *a, const unsigned short *b, int n) __asm__("USTR_ncompare");
/**
 * Compares up to @p n characters of two UCS-2 strings; returns the difference of the first pair that differs, 0 when
 * equal (stopping at the end of @p lhs).
 *
 * @param lhs  the first string
 * @param rhs  the second string
 * @param n  the maximum number of characters
 * @return the difference, 0 when equal
 */
int USTR_ncompare_impl(const unsigned short *lhs, const unsigned short *rhs, int n) {
    int i;
    int diff = 0;
    for (i = 0; i < n; i++, rhs++, lhs++) {
        if (diff != 0) {
            break;
        }
        diff = *lhs - *rhs;
        if (*lhs == 0) {
            break;
        }
    }
    return diff;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_copy);
#ifdef SKIP_ASM
extern void USTR_ncopy(int, int, int);

/**
 * Copies a zero-terminated wide string with no length limit.
 *
 * Calls USTR_ncopy() with a count of 0x7FFFFFFF, so the copy stops only at the source's terminator.
 *
 * @param dst  destination wide string
 * @param src  source wide string
 */
void USTR_copy(int dst, int src) {
    USTR_ncopy(dst, src, 0x7FFFFFFF);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_ncopy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void USTR_ncopy_impl(unsigned short *d, unsigned short *s, int n) __asm__("USTR_ncopy");
/**
 * Copies at most @p n characters of a wide string and always terminates the copy.
 *
 * Stops early at the source's 0, then writes a 0 after the last character copied, so @p dst needs room for
 * n + 1 characters.
 *
 * @param dst  destination wide string
 * @param src  source wide string
 * @param n  maximum number of characters to copy
 */
void USTR_ncopy_impl(unsigned short *dst, unsigned short *src, int n) {
    int i;
    for (i = 0; i < n && src[i] != 0; i++) dst[i] = src[i];
    dst[i] = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_copycstr);
#ifdef SKIP_ASM
/**
 * Copies an 8-bit C string into a 16-bit (UCS-2) string.
 *
 * Each byte is zero-extended into one character of @p dst, and the result is 0-terminated.
 *
 * @param dst  the 16-bit destination
 * @param csrc  the 8-bit source string
 */
void USTR_copycstr(short *dst, signed char *csrc) {
    short *var_3;
    int var_6;
    signed char *var_5;
    unsigned char temp_2;

    var_5 = csrc;
    var_6 = 0;
    if (*var_5 != 0) {
        var_3 = dst;
        do {
            temp_2 = *var_5;
            var_6 += 1;
            var_5 += 1;
            *var_3 = (short) temp_2;
            var_3 += 1;
        } while (*var_5 != 0);
    }
    dst[var_6] = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_format1);
#ifdef SKIP_ASM
extern void USTR_format2(int, int, int, int);

/**
 * Formats a UCS-2 string with one insert: USTR_format2 with no second string.
 *
 * @param dst  the output
 * @param fmt  the format, with "%1"
 * @param insert1  the string for "%1"
 */
void USTR_format1(int dst, int fmt, int insert1) {
    USTR_format2(dst, fmt, insert1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_format2);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void ucopy_BEF0(unsigned short *, unsigned short *) __asm__("USTR_copy");
extern int ulen_BEF0(unsigned short *) __asm__("USTR_length");

void fBEF0_BEF0(unsigned short *d, unsigned short *s, unsigned short *a2, unsigned short *a3) __asm__("USTR_format2");
/**
 * Formats a UCS-2 string: copies @p fmt to @p dst, replacing "%1" with @p insert1 and "%2" with @p insert2.
 *
 * @param dst  the output
 * @param fmt  the format
 * @param insert1  the string for "%1"
 * @param insert2  the string for "%2"
 */
void fBEF0_BEF0(unsigned short *dst, unsigned short *fmt, unsigned short *insert1, unsigned short *insert2) {
    unsigned short cur, next;
    unsigned short *insert;
    cur = *fmt;
    if (cur != 0) {
        do {
            if (cur != 37) {
                *dst++ = *fmt++;
            } else {
                next = fmt[1];
                if (next == 49) {
                    ucopy_BEF0(dst, insert1);
                    fmt += 2;
                    insert = insert1;
                    dst += ulen_BEF0(insert);
                } else if (next == 50) {
                    ucopy_BEF0(dst, insert2);
                    fmt += 2;
                    insert = insert2;
                    dst += ulen_BEF0(insert);
                } else {
                    *dst++ = *fmt++;
                }
            }
            cur = *fmt;
        } while (cur != 0);
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", format_uint);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
unsigned short *format_uint_BFD8(unsigned short *out, unsigned int n, unsigned int base, unsigned short *digits) __asm__("format_uint");
/**
 * Writes an unsigned number backwards in a base from a digit table, ending at @p out; returns the first character
 * written.
 *
 * @param out  the end of the output buffer
 * @param n  the value
 * @param base  the base
 * @param digits  the digit characters
 * @return the start of the digits
 */
unsigned short *format_uint_BFD8(unsigned short *out, unsigned int n, unsigned int base, unsigned short *digits) {
    if (n == 0) {
        *--out = digits[0];
    } else {
        do {
            *--out = digits[n % base];
            n /= base;
        } while (n != 0);
    }
    return out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", format_int);
#ifdef SKIP_ASM
extern void format_uint(int, int);

/**
 * Writes a signed number backwards (its magnitude, through format_uint). Only two arguments are declared; the base
 * and digit table reach format_uint in the untouched argument registers.
 *
 * @param out  the end of the output buffer
 * @param value  the value
 */
void format_int(int out, int value) {
    if (value < 0) {
        value = -value;
    }
    format_uint(out, value);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", format_double);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char *fcvt_C068(int, int, int *, int *) __asm__("fcvt");
extern int strlen_C068(char *) __asm__("strlen");
/**
 * Writes a double backwards with @p prec decimals: the digits from fcvt, a '.' before the decimals, zeros where fcvt
 * gave fewer digits; returns the first character written.
 *
 * @param dst  the end of the output buffer
 * @param val  the value
 * @param prec  the number of decimals
 * @return the start of the text
 */
unsigned short *format_double(unsigned short *dst, int val, int prec) {
    int exp, sign;
    int i = 0;
    int k = 0;
    char *digits = fcvt_C068(val, prec, &exp, &sign);
    int len = strlen_C068(digits);
    int total = exp + prec;
    for (; i != total + 1; i++) {
        if (i == prec) {
            *--dst = '.';
        } else {
            int digitPos = total - k;
            if (len < digitPos) {
                *--dst = '0';
            } else {
                *--dst = digits[digitPos - 1];
            }
            k++;
        }
    }
    return dst;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", output_number);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned short *f068_C148(unsigned short *, double, int) __asm__("format_double");
extern unsigned short *f038_C148(unsigned short *, int, int, unsigned short *) __asm__("format_int");
extern unsigned short *fBFD8_C148(unsigned short *, unsigned int, int, unsigned short *) __asm__("format_uint");
extern int f608_C148(double) __asm__("dptoli");
extern unsigned int f6A0_C148(double) __asm__("dptoul");
extern int f500_C148(double, double) __asm__("dpcmp");
extern int ulen_C148(unsigned short *) __asm__("USTR_length");
extern void ucopy_C148(unsigned short *, unsigned short *) __asm__("USTR_copy");

/* fill char ch in front of the text at p until it is width chars long */
#define PAD_LEFT_C148(p, ch, width) \
    { \
        unsigned short c_ = (ch); \
        int n_ = ulen_C148(p); \
        if (n_ < (width)) { \
            n_ = (width) - n_; \
            do { \
                *--(p) = c_; \
            } while (--n_ != 0); \
        } \
    }

/* append char ch at q until the length n reaches width; n becomes width */
#define PAD_RIGHT_C148(q, ch, n, width) \
    { \
        unsigned short c_ = (ch); \
        if ((n) < (width)) { \
            (n) = (width) - (n); \
            do { \
                *(q)++ = c_; \
            } while (--(n) != 0); \
            (n) = (width); \
        } \
    }

int output_number_impl(unsigned short **pp, double val, int base, unsigned int flags, int width, int prec, unsigned short *digits) __asm__("output_number");
/**
 * Formats one number for USTR_vsprintf into a wide-character string and appends it at *pp.
 *
 * Flags: 1 '+', 2 '#', 4 ' ', 8 '0', 0x10 '-', 0x20 unsigned integer, 0x40/0x80/0x100 for f/e/g. Integers
 * arrive
 * converted to double. The e and g forms are formatted exactly like f in this build.
 *
 * @param pp  the output pointer, advanced past the text (On Tour pdst)
 * @param val  the value (On Tour value)
 * @param base  number base: 8, 10 or 16
 * @param flags  format flags (above)
 * @param width  minimum field width
 * @param prec  digits after the decimal point for f, e and g
 * @param digits  the digit characters to use (lower- or upper-case hex)
 * @return number of characters written
 */
int output_number_impl(unsigned short **pp, double val, int base, unsigned int flags, int width, int prec, unsigned short *digits) {
    unsigned short *out = *pp;
    unsigned short buf[128];
    unsigned short *text = &buf[127];
    int len;
    *text = 0;
    if (flags & 0x40) {
        // the 0x80 / 0x100 / default arms are identical in the release build, but the binary keeps the
        // two tests and three separate call copies (the prototype likely differed in a debug-only detail)
        if (flags & 0x80) {
            text = f068_C148(text, val, prec);
        } else if (flags & 0x100) {
            text = f068_C148(text, val, prec);
        } else {
            text = f068_C148(text, val, prec);
        }
    } else if (flags & 0x20) {
        text = fBFD8_C148(text, f6A0_C148(val), base, digits);
    } else {
        text = f038_C148(text, f608_C148(val), base, digits);
    }
    if ((flags & 0x18) == 8) {
        PAD_LEFT_C148(text, 0x30, width);
    }
    if (base == 10 && !(flags & 0x20)) {
        if (f500_C148(val, 0.0) < 0) {
            *--text = 45;
        }
        if (f500_C148(val, 0.0) > 0 && (flags & 1)) {
            *--text = 43;
        }
        if (f500_C148(val, 0.0) == 0 && (flags & 1)) {
            *--text = 0x20;
        }
        if (f500_C148(val, 0.0) >= 0 && (flags & 5) == 4) {
            *--text = 0x20;
        }
    }
    if (base == 8 && (flags & 2)) {
        *--text = 0x30;
    }
    if (base == 16 && (flags & 2)) {
        *--text = 0x78;
        *--text = 0x30;
    }
    if (!(flags & 0x10)) {
        PAD_LEFT_C148(text, (flags & 8) ? 0x30 : 0x20, width);
    }
    len = ulen_C148(text);
    ucopy_C148(out, text);
    out += len;
    if (flags & 0x10) {
        PAD_RIGHT_C148(out, 0x20, len, width);
    }
    *pp = out;
    return len;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_vsprintf);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void USTR_copycstr(short *arg0, signed char *arg1);
extern int output_number_C450(unsigned short **, long, int, int, int, int, unsigned short *) __asm__("output_number");
extern long func_00412550_C450(int) __asm__("litodp");
extern signed char str_0123456789abcdef_C450[] __asm__("str_0123456789abcdef");
extern signed char str_0123456789ABCDEF_C450[] __asm__("str_0123456789ABCDEF");

#define VA_ARG_C450(ap, T) (*(T *)((ap += 8) - 8))

void USTR_vsprintf_impl(unsigned short *dst, unsigned short *fmt, char *ap) __asm__("USTR_vsprintf");
/**
 * Formats a wide-character (UTF-16) string printf-style into @p dst, with no length limit.
 *
 * Supports flags - + 0 space #, width and precision (also *), and c C d i o u x X e E f g G n p P s S:
 * lower-case
 * c and s take wide characters, C and S narrow ones; null strings print "(null)". The output is terminated.
 *
 * @param dst  output buffer
 * @param fmt  wide-character format
 * @param ap  the arguments (va_list)
 */
void USTR_vsprintf_impl(unsigned short *dst, unsigned short *fmt, char *ap) {
    unsigned short lower[24];
    unsigned short upper[24];
    unsigned short fmtChar;
    int count;
    int flags;
    int width;
    int prec;
    int more;

    count = 0;
    USTR_copycstr((short *)lower, str_0123456789abcdef_C450);
    USTR_copycstr((short *)upper, str_0123456789ABCDEF_C450);
    fmtChar = *fmt++;
    for (;;) {
        while (fmtChar != 0 && fmtChar != '%') {
            count++;
            *dst++ = fmtChar;
            fmtChar = *fmt++;
        }
        if (fmtChar == 0) {
            break;
        }
        more = 1;
        fmtChar = *fmt++;
        flags = 0;
        width = 0;
        prec = 6;
        while (more) {
            switch (fmtChar) {
            case '-':
                flags |= 0x10;
                fmtChar = *fmt++;
                break;
            case '+':
                flags |= 1;
                fmtChar = *fmt++;
                break;
            case '0':
                flags |= 8;
                fmtChar = *fmt++;
                break;
            case ' ':
                flags |= 4;
                fmtChar = *fmt++;
                break;
            case '#':
                flags |= 2;
                fmtChar = *fmt++;
                break;
            default:
                more = 0;
                break;
            }
        }
        switch (fmtChar) {
        case '*':
            width = VA_ARG_C450(ap, int);
            fmtChar = *fmt++;
            break;
        case '1' ... '9':
            width = 0;
            while ((unsigned int)(fmtChar - '0') < 10) {
                width = width * 10 + (fmtChar - '0');
                fmtChar = *fmt++;
            }
            break;
        }
        if (fmtChar == '.') {
            fmtChar = *fmt++;
            switch (fmtChar) {
            case '*':
                prec = VA_ARG_C450(ap, int);
                fmtChar = *fmt++;
                break;
            case '1' ... '9':
                prec = 0;
                while ((unsigned int)(fmtChar - '0') < 10) {
                    prec = prec * 10 + (fmtChar - '0');
                    fmtChar = *fmt++;
                }
                break;
            }
        }
        switch (fmtChar) {
        case 'c':
            count++;
            *dst++ = (unsigned short)VA_ARG_C450(ap, int);
            break;
        case 'C':
            count++;
            { char ch = VA_ARG_C450(ap, char); *dst++ = ch; }
            break;
        case 'd':
        case 'i':
            count += output_number_C450(&dst, func_00412550_C450(VA_ARG_C450(ap, int)), 10, flags, width, prec, lower);
            break;
        case 'o':
            flags |= 0x20;
            count += output_number_C450(&dst, func_00412550_C450(VA_ARG_C450(ap, int)), 8, flags, width, prec, lower);
            break;
        case 'u':
            flags |= 0x20;
            count += output_number_C450(&dst, func_00412550_C450(VA_ARG_C450(ap, int)), 10, flags, width, prec, lower);
            break;
        case 'x':
        case 'X':
            flags |= 0x20;
            count += output_number_C450(&dst, func_00412550_C450(VA_ARG_C450(ap, int)), 16, flags, width, prec, fmtChar == 'x' ? lower : upper);
            break;
        case 'e':
        case 'E':
            count += output_number_C450(&dst, VA_ARG_C450(ap, long), 10, flags | 0x80, width, prec, lower);
            break;
        case 'g':
        case 'G':
            count += output_number_C450(&dst, VA_ARG_C450(ap, long), 10, flags | 0x100, width, prec, lower);
            break;
        case 'f':
            count += output_number_C450(&dst, VA_ARG_C450(ap, long), 10, flags | 0x40, width, prec, lower);
            break;
        case 'n':
            *VA_ARG_C450(ap, int *) = count;
            break;
        case 'p':
        case 'P':
            flags |= 0x28;
            count += output_number_C450(&dst, func_00412550_C450(VA_ARG_C450(ap, int)), 16, flags, 8, prec, fmtChar == 'p' ? lower : upper);
            break;
        case 's': {
            unsigned short *strArg = VA_ARG_C450(ap, unsigned short *);
            if (strArg != 0) {
                unsigned short ch;
                while ((ch = *strArg++) != 0) {
                    count++;
                    *dst++ = ch;
                }
            } else {
                *dst++ = '(';
                *dst++ = 'n';
                *dst++ = 'u';
                *dst++ = 'l';
                *dst++ = 'l';
                *dst++ = ')';
                count += 6;
            }
            break;
        }
        case 'S': {
            char *strArg = VA_ARG_C450(ap, char *);
            if (strArg != 0) {
                char ch;
                while ((ch = *strArg++) != 0) {
                    count++;
                    *dst++ = ch;
                }
            } else {
                *dst++ = '(';
                *dst++ = 'n';
                *dst++ = 'u';
                *dst++ = 'l';
                *dst++ = 'l';
                *dst++ = ')';
                count += 6;
            }
            break;
        }
        default:
            count++;
            *dst++ = fmtChar;
            break;
        }
        fmtChar = *fmt++;
    }
    *dst++ = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_sprintf);
#ifdef SKIP_ASM
extern void USTR_vsprintf(char *, const char *, char *);

// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define USTR_sprintf_va_start(ap)                                        \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

/**
 * Formats a 16-bit string printf-style.
 *
 * Collects the variable arguments and passes them to USTR_vsprintf.
 *
 * @param dst  the 16-bit destination
 * @param fmt  the format string
 */
void USTR_sprintf(char *dst, const char *fmt, ...) {
    char *ap;
    USTR_sprintf_va_start(ap);
    USTR_vsprintf(dst, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_strcpyA);
#ifdef SKIP_ASM
/**
 * Copies a C string to a UCS-2 string, decoding "\xHHHH" escapes (4 lowercase hex digits) into one character each.
 *
 * @param dst  the UCS-2 output
 * @param src  the C string
 */
void USTR_strcpyA(short *dst, char *src) {
    while (*src != 0) {
        int ok = 0;
        int val = 0;
        int i;
        int digit;
        if (src[0] == '\\' && src[1] == 'x') {
            ok = 1;
            for (i = 0; i < 4 && src[i + 2] != 0; i++) {
                if (src[i + 2] >= '0' && src[i + 2] <= '9') {
                    digit = (unsigned char)src[i + 2] - '0';
                } else if (src[i + 2] >= 'a' && src[i + 2] <= 'f') {
                    digit = (unsigned char)src[i + 2] - 'a' + 10;
                } else {
                    ok = 0;
                    goto out;
                }
                val = (val << 4) + digit;
            }
        }
out:
        if (ok) {
            *dst++ = val;
            src += 6;
        } else {
            *dst++ = (unsigned char)*src;
            src++;
        }
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", USTR_strcpyW);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char str_xpct_04x_CB20[] __asm__("str_xpct_04x");
extern int sprintf_CB20(char *, const char *, ...) __asm__("sprintf");

/**
 * Copies a wide-character string to a narrow one, writing characters above 0xFF (and a literal "\x") as
 * "\x%04x" escapes.
 *
 * @param dst  the narrow output buffer; up to six bytes per input character
 * @param src  the wide-character string
 */
void USTR_strcpyW(char *dst, unsigned short *src) {
    int wideChar;
    while ((wideChar = *src) != 0) {
        if ((unsigned short)wideChar >= 0x100 || ((unsigned short)wideChar == 92 && src[1] == 0x78)) {
            sprintf_CB20(dst, str_xpct_04x_CB20, *src);
            dst += 6;
        } else {
            *dst++ = *src;
        }
        src++;
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_initadr_CBD8(int, int, int, int) __asm__("FILESYS_initadr");
extern int FILESYS_overhead_CBD8(int, int, int) __asm__("FILESYS_overhead");
extern char str_File_Sys_CBD8[] __asm__("str_File_Sys");
extern int pFileDeviceMem_CBD8[] __asm__("pFileDeviceMem");

/**
 * Sets up the EA file system once: allocates its work memory and builds the command manager.
 *
 * Only acts while the command manager (gFileDevice) is unset. It allocates FILESYS_overhead()'s size through the
 * file-system allocator hook (tag "File Sys"), keeps the block in pFileDeviceMem and hands it to FILESYS_initadr().
 * Zero arguments take FILESYS_initadr's defaults: 16 file handles of 0x110 bytes, 1000 disc-directory entries
 * and 32 command slots of 0x30 bytes.
 *
 * @param maxopenfiles  number of file handles (0 for 16)
 * @param maxdirfiles  size of the disc directory table, probably in entries (0 for 1000)
 * @param maxfileops  number of queued-command slots (0 for 32)
 * @return 1 when the file system was set up by this call, 0 if it already was
 */
int FILESYS_init(int maxopenfiles, int maxdirfiles, int maxfileops) {
    int workBuffer;
    if (gFileDevice.maxfileops == 0) {
        workBuffer = ((int (*)(char*, int, int))gFileSysOpts.malloc)(str_File_Sys_CBD8, FILESYS_overhead_CBD8(maxopenfiles, maxdirfiles, maxfileops), 0x100);
        pFileDeviceMem_CBD8[0] = workBuffer;
        return FILESYS_initadr_CBD8(maxopenfiles, maxdirfiles, maxfileops, workBuffer);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_initadr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int D_00450C60_CC88 __asm__("D_00450C60");
extern void MUTEX_create_CC88(void *) __asm__("MUTEX_create");
extern int FILESYS_overhead_CC88(int, int, int) __asm__("FILESYS_overhead");
extern void MEM_fill_CC88(int, int, int) __asm__("MEM_fill");
extern int CDVD_initFileSystem_CC88(int, int) __asm__("CDVD_initFileSystem");
extern void FILEDEV_init_CC88(int, int) __asm__("FILEDEV_init");
/**
 * Initialises the file system in caller-supplied memory; returns 0 when it was already initialised.
 *
 * Zero arguments take the defaults (16 open files, 1000 directory entries, 32 file operations). Creates the mutex,
 * clears FILESYS_overhead's worth of @p mem, places the 0x30-byte operation table and the 0x110-byte handle table in
 * it, then calls FILEDEV_init(@p maxopenfiles, v), v being CDVD_initFileSystem's result for the directory entries when
 * D_00450C60 is set and 0 otherwise.
 *
 * @param maxopenfiles  maximum open files (0: 16)
 * @param maxdirfiles  maximum directory entries (0: 1000)
 * @param maxfileops  maximum pending file operations (0: 32)
 * @param mem  the memory to use, at least FILESYS_overhead(a, b, c) bytes
 * @return 1 when initialised now, 0 when already initialised
 */
int FILESYS_initadr(int maxopenfiles, int maxdirfiles, int maxfileops, int mem) {
    int alignedDir;
    if (maxopenfiles == 0) maxopenfiles = 0x10;
    if (maxdirfiles == 0) maxdirfiles = 1000;
    if (maxfileops == 0) maxfileops = 0x20;
    if (gFileDevice.maxfileops == 0) {
        gFileDevice.minpriority = 0xFF;
        gFileDevice.maxfileops = maxfileops;
        gFileDevice.maxopen = maxopenfiles;
        MUTEX_create_CC88(&gFileDevice.mutex[0]);
        *(int*)&gFileDevice.oplist = mem;
        MEM_fill_CC88(((int)gFileDevice.oplist), 0, FILESYS_overhead_CC88(maxopenfiles, maxdirfiles, maxfileops));
        *(int*)&gFileDevice.handlelist = (int)gFileDevice.oplist + gFileDevice.maxfileops * 0x30;
        if (D_00450C60_CC88 != 0) {
            alignedDir = CDVD_initFileSystem_CC88(maxdirfiles, gFileDevice.maxopen * 272 + (int)gFileDevice.handlelist);
        } else {
            alignedDir = 0;
        }
        FILEDEV_init_CC88(maxopenfiles, alignedDir);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_overhead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C5C_CD98[] __asm__("D_00450C5C");

/**
 * Returns how many bytes of memory the file system needs for the given limits.
 *
 * Zero arguments take the defaults (16, 1000, 32). The size is @p maxfileops * 0x30 (operations) + @p maxopenfiles * 0x110 (handles)
 * + @p maxdirfiles * (D_00450C5C + 0xC) (directory entries) + 0x40.
 *
 * @param maxopenfiles  maximum open files
 * @param maxdirfiles  maximum directory entries
 * @param maxfileops  maximum pending file operations
 * @return the size in bytes
 */
int FILESYS_overhead(int maxopenfiles, int maxdirfiles, int maxfileops) {
    int x = maxopenfiles ? maxopenfiles : 0x10;
    int y = maxdirfiles ? maxdirfiles : 1000;
    int z = maxfileops ? maxfileops : 0x20;
    int sz = D_00450C5C_CD98[0] + 0xC;
    return z * 0x30 + x * 272 + y * sz + 0x40;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void CDVD_shutdownFileSystem_CDE0(void) __asm__("CDVD_shutdownFileSystem");
extern void FILESYS_closesync_CDE0(int, int) __asm__("FILESYS_closesync");
extern void MUTEX_destroy_CDE0(char *) __asm__("MUTEX_destroy");
extern void MEM_clear_CDE0(char *, int) __asm__("MEM_clear");
extern int pFileDeviceMem_CDE0[] __asm__("pFileDeviceMem");

/**
 * Shuts the file system down: calls CDVD_shutdownFileSystem, frees and closes every added archive (FILESYS_closesync on its
 * file), releases the mutex (MUTEX_destroy), clears the command manager's state and frees its memory block (pFileDeviceMem)
 * through the free callback.
 */
void FILESYS_restore(void) {
    int *n;
    CDVD_shutdownFileSystem_CDE0();
    for (n = (int*)gFileDevice.bigfilelist; n != 0; n = (int *)n[5]) {
        ((void (*)(int))gFileSysOpts.free)(n[0]);
        FILESYS_closesync_CDE0(*(int *)n[3], 0);
    }
    MUTEX_destroy_CDE0(((char*)&gFileDevice.mutex[0]));
    MEM_clear_CDE0((char*)&gFileDevice, 0x38);
    if (pFileDeviceMem_CDE0[0] != 0) {
        ((void (*)(int))gFileSysOpts.free)(pFileDeviceMem_CDE0[0]);
        pFileDeviceMem_CDE0[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_opstatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.

/* A handle word has OP_def's layout (a union over its int id): it is valid while it is not 0 and its serial (bits
   0..19) still matches the id of the pool slot its index byte names. */
static __inline__ int FILESYS_validop(int ophandle) {
    int ok = 0;
    if (ophandle != 0) {
        ok = ((gFileDevice.oplist[((OP_def *)&ophandle)->sep.index].op.id ^ ophandle) & 0xFFFFF) == 0;
    }
    return ok;
}

/**
 * Returns the status of a file operation, or -3 when the handle is stale (its serial no longer matches the slot named
 * by its index byte).
 *
 * @param ophandle  the operation handle
 * @return the status, or -3
 */
int FILESYS_opstatus(int ophandle) {
    // ORIGINAL-PTR: [O] the handle is an int (On Tour: FILESYS_opstatus(int ophandle)) and OP_def the union that
    //     splits it; [R] retail stores the argument and reads its index byte through its address (lbu 0x3 off
    //     the spilled word), so EA read the bit-fields in place.
    if (FILESYS_validop(ophandle) == 0) return -3;
    // status is volatile (FILEOPERATION_def): retail keeps this load out of the b delay slot
    return gFileDevice.oplist[((OP_def *)&ophandle)->sep.index].status;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_operror);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { unsigned char a, b, c, d; } T_CF10;

/**
 * Returns the error code (+0xC) of a file operation (no handle check).
 *
 * @param opHandle  the operation handle
 * @return the error code
 */
int FILESYS_operror(T_CF10 opHandle) {
    return gFileDevice.oplist[opHandle.d].error;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_opuserdata);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { unsigned char a, b, c, d; } T_CF40;

/**
 * Returns the user data (+0x14) of a file operation (no handle check).
 *
 * @param opHandle  the operation handle
 * @return the user data
 */
void *FILESYS_opuserdata(T_CF40 opHandle) {
    return gFileDevice.oplist[opHandle.d].userdata;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_callbackop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef void (*Fn_CF70)(int, int, int);

void FILESYS_callbackop_impl(int h, Fn_CF70 fn) __asm__("FILESYS_callbackop");
/**
 * Sets the completion callback of a pending file operation.
 *
 * The operation slot is the handle's index byte (OP_def). When the operation has already finished (its status is
 * set), @p fn is called immediately with the handle, the status and the user data.
 *
 * @param ophandle   the operation handle
 * @param fn  the completion callback
 */
void FILESYS_callbackop_impl(int ophandle, Fn_CF70 fn) {
    struct FILEOPERATION_def *fop = &gFileDevice.oplist[((OP_def *)&ophandle)->sep.index];
    fop->callback = fn;
    if (fop->status != 0) {
        fn(ophandle, fop->status, ((int)fop->userdata));
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_priorityop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct H_CFC0 { unsigned int v; };
typedef struct Ent_CFC0 {
    int f0, f4;
    volatile int busy;
    int fC;
    int prio;
    char pad[0x18];
    struct Ent_CFC0 *next;
} Ent_CFC0;
typedef struct Sys_CFC0 {
    int f0, f4, f8, count, f10;
    char mutex[0x10];
    Ent_CFC0 *cur;
    char *table;
    int f2C;
    Ent_CFC0 *head;
} Sys_CFC0;
extern void MUTEX_lock_CFC0(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_CFC0(void *) __asm__("MUTEX_unlock");

void FILESYS_priorityop_impl(struct H_CFC0 h, int prio) __asm__("FILESYS_priorityop");
extern Sys_CFC0 gFileDevice_CFC0 __asm__("gFileDevice");
extern char D_00519AC4_CFC0[] __asm__("D_00519AC4");
/**
 * Changes a pending file operation's priority (+0x10) and moves it to its place in the priority-ordered queue.
 *
 * Does nothing for a stale handle. The queue is re-sorted only when two or more operations are queued, the operation is
 * not the running one, it is not busy (+0x8) and the priority changed. Runs under the file system's mutex.
 *
 * @param opHandle  the operation handle
 * @param prio  the new priority
 */
void FILESYS_priorityop_impl(struct H_CFC0 opHandle, int prio) {
    struct H_CFC0 handleCopy;
    Ent_CFC0 *fileOp;
    Ent_CFC0 *listOp;
    Ent_CFC0 *prev;
    int old;
    unsigned int handleBits = opHandle.v;
    int ok = 0;

    fileOp = (Ent_CFC0 *)(gFileDevice_CFC0.table + ((unsigned char *)&opHandle)[3] * 0x30);
    handleCopy.v = handleBits;
    if (handleBits != 0) {
        unsigned int x = (*(unsigned int *)(((unsigned char *)&handleCopy)[3] * 0x30 + gFileDevice_CFC0.table) ^ handleBits) & 0xFFFFF;
        ok = x == 0;
    }
    if (ok == 0) return;
    MUTEX_lock_CFC0(gFileDevice_CFC0.mutex);
    old = fileOp->prio;
    fileOp->prio = prio;
    if (gFileDevice_CFC0.count >= 2 && fileOp != gFileDevice_CFC0.cur && fileOp->busy == 0 && old != prio) {
        prev = 0;
        for (listOp = gFileDevice_CFC0.head; listOp != 0 && listOp != fileOp; listOp = listOp->next) prev = listOp;
        if (prev != 0) prev->next = fileOp->next;
        else gFileDevice_CFC0.head = fileOp->next;
        prev = 0;
        for (fileOp->next = gFileDevice_CFC0.head; fileOp->next != 0 && !(fileOp->prio < fileOp->next->prio); fileOp->next = prev->next) prev = fileOp->next;
        if (prev != 0) prev->next = fileOp;
        else gFileDevice_CFC0.head = fileOp;
    }
    MUTEX_unlock_CFC0(D_00519AC4_CFC0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_cancelop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/**
 * Asks the file system to cancel a pending operation.
 *
 * The handle must still be valid (FILESYS_validop); operations of type 3 and 0xA (OP_def's type bits) cannot be
 * cancelled, the others get their cancelled flag set.
 *
 * @param ophandle  the operation handle
 */
void FILESYS_cancelop(int ophandle) {
    // ORIGINAL-PTR: [O] the handle is an int (On Tour: FILESYS_cancelop(int ophandle)) and OP_def the union that
    //     splits it; [R] retail stores the argument and reads its index byte through its address (lbu 0x3 off
    //     the spilled word), so EA read the bit-fields in place.
    int opType;
    if (FILESYS_validop(ophandle) != 0) {
        opType = ((unsigned int)ophandle >> 20) & 0xF;
        if (opType != FOPTYPE_close && opType != FOPTYPE_delbig) {
            gFileDevice.oplist[((OP_def *)&ophandle)->sep.index].cancelled = 1;
        }
    }
}
#endif

#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* gFileDevice: the file system's command manager (FILESYS_initadr sets it up). Layout from every use in this unit;
   field names are FILE_FILEDEVICE's. Several fields are also reached through their own splat symbols (declared
   below), which the functions that address them directly must keep using. */
struct sFsCmdId {
    unsigned int id : 20;
    unsigned int type : 4;
    unsigned int idx : 8;           /* index in gFileDevice.oplist */
};
/* Member names: FILE_FILEDEVICE's ([O], ealib/filesys.h) at the same offsets. */
struct sAB0 {
    int maxfileops;                 /* number of commands */
    int maxopen;                    /* number of open files */
    int minpriority;                /* highest priority run (also D_00519AB8) */
    int numopsqueued;               /* commands queued */
    int waitlevel;                  /* callback depth */
    int mutex[4];                   /* mutex (also D_00519AC4) */
    struct FILEOPERATION_def *exec;             /* running */
    struct FILEOPERATION_def *oplist;           /* commands (also D_00519AD8) */
    struct HANDLE_def *handlelist;              /* open files */
    struct FILEOPERATION_def *opqhead;          /* queue */
    struct FILEBIG_def *bigfilelist;            /* archives (also D_00519AE4) */
    int pFileDeviceMem;             /* (also pFileDeviceMem) */
};
struct sAB0Lock {                   /* gFileDevice from the mutex on (D_00519AC4) */
    int mutex[4];                   /* the mutex */
    struct FILEOPERATION_def *exec;
    struct FILEOPERATION_def *oplist;
    struct HANDLE_def *handlelist;
    struct FILEOPERATION_def *opqhead;          /* the queue (gFileDevice.opqhead) */
    struct FILEBIG_def *bigfilelist;
    int pFileDeviceMem;
};
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_waitop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int THREAD_iscurrent(THREAD *);
extern void THREAD_yield(int);
extern int SYNCTASK_run(int);

/* A handle word has OP_def's layout (a union over its int id), so its pool index is read through OP_def::sep. */
static inline int hValid_D1D8(int h) {
    return h != 0
        && ((h & 0xFFFFF) == (((unsigned int)gFileDevice.oplist[((OP_def *)&h)->sep.index].op.id) & 0xFFFFF));
}

/**
 * Blocks until a file operation completes and returns its result.
 *
 * While the operation's status is 0, runs SYNCTASK_run when THREAD_iscurrent allows it, else sleeps with
 * THREAD_yield(1). Returns -3 as soon as the handle stops being valid.
 *
 * @param ophandle  the operation handle
 * @return the operation's result, or -3 for an invalid handle
 */
int FILESYS_waitop(int ophandle) {
    // ORIGINAL-PTR: [O] the handle is an int (On Tour: FILESYS_waitop(int ophandle)) and OP_def the union that
    //     splits it; [R] retail stores the argument and reads its index byte through its address (lbu 0x3 off
    //     the spilled word), so EA read the bit-fields in place.
    FILEOPERATION_def *fop = &gFileDevice.oplist[((OP_def *)&ophandle)->sep.index];
    if (!hValid_D1D8(ophandle)) {
        return -3;
    }
    while (fop->status == 0) {
        if (THREAD_iscurrent(0) != 0) {
            SYNCTASK_run(0);
        } else {
            THREAD_yield(1);
        }
        if (!hValid_D1D8(ophandle)) {
            return -3;
        }
    }
    return fop->status;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_completeop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void freeop_p(void *) __asm__("freeop");
extern void freehandle_p(void *) __asm__("freehandle");

int FILESYS_completeop_a(int ophandle) __asm__("FILESYS_completeop");
/**
 * Finishes a completed file operation: returns its result and releases its resources.
 *
 * Picks the result by operation type (a size, a handle, a byte count...), frees a handle the operation owned with
 * freehandle and the operation slot with freeop.
 *
 * @param ophandle  the operation handle
 * @return the operation's result
 */
int FILESYS_completeop_a(int ophandle) {
    int ret = 0;
    struct FILEOPERATION_def *slot = &gFileDevice.oplist[((OP_def *)&ophandle)->sep.index];
    int sum;
    struct HANDLE_def *fileHandle;

    switch (slot->status) {
    case 0:
        break;
    case 1:
        switch (slot->op.sep.type) {
        case FOPTYPE_open:
        case FOPTYPE_addbig:
            ret = (int)slot->handle;
            break;
        case FOPTYPE_close:
        case FOPTYPE_null:
        case FOPTYPE_delbig:
            ret = slot->status;
            break;
        case FOPTYPE_size:
        case FOPTYPE_exist:
            ret = slot->value;
            break;
        case FOPTYPE_read:
            ret = slot->amount;
            break;
        case FOPTYPE_write:
            fileHandle = slot->handle;
            ret = slot->amount;
            sum = ret + slot->value;
            if (fileHandle->length < sum) {
                fileHandle->length = sum;
            }
            break;
        }
        break;
    case -2:
    case -1: {
        int type = ((unsigned int)slot->op.id >> 20) & 0xF;
        if (type == FOPTYPE_open || type == FOPTYPE_addbig) {
            freehandle_p(slot->handle);
        }
        break;
    }
    }
    freeop_p(slot);
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_exists);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *f_E670_D438() __asm__("reserveop");
extern struct HANDLE_def *f_E800_D438() __asm__("reservehandle");
extern void f_E668_D438(int *) __asm__("iFILE_perror");
extern char *f_strncpy_D438(char *, char *, int) __asm__("strncpy");
extern void f_Exec_D438(int *) __asm__("iFILESYS_ExecCommand");

/**
 * Queues a file-exists operation (type 8) for a path; returns its handle.
 *
 * Takes an operation (reserveop) and a handle slot (reservehandle) for the name; without a free slot the error is
 * set to 2 (iFILE_perror). The name is copied into the slot and the operation is run (iFILESYS_ExecCommand).
 *
 * @param fileName  the path
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_exists(char *fileName, int priority, void *userData) {
    struct FILEOPERATION_def *fileOp = (struct FILEOPERATION_def *)f_E670_D438();
    fileOp->userdata = userData;
    fileOp->priority = priority;
    fileOp->op.id = (fileOp->op.id & 0xFF0FFFFF) | 0x800000;
    fileOp->value = 1;
    fileOp->handle = f_E800_D438();
    if (fileOp->handle == 0) {
        fileOp->error = FERR_NoHandles;
        f_E668_D438((int *)fileOp);
    }
    f_strncpy_D438(fileOp->handle->name, fileName, 0x100);
    f_Exec_D438((int *)fileOp);
    return fileOp->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_open);
#ifdef SKIP_ASM
extern struct HANDLE_def *reservehandle();
extern void iFILE_perror();

/**
 * Queues an asynchronous file open and returns the operation handle.
 *
 * Reserves an operation slot and a file handle (failing the operation with error 2 when none is free), records
 * the open flags, priority and user data, copies the name into the handle and hands the command to
 * iFILESYS_ExecCommand.
 *
 * @param name  the file name
 * @param modeflags  open mode flags
 * @param priority  queue priority
 * @param userdata  user data passed to the completion callback
 * @return the operation handle
 */
int FILESYS_open(char *name, int modeflags, int priority, void *userdata) {
    FILEOPERATION_def* fop = (FILEOPERATION_def*)reserveop();
    struct HANDLE_def *fileHandle;
    fop->userdata = userdata;
    fop->value = modeflags;
    fop->priority = priority;
    *(int *)fop = (*(int *)fop & 0xFF0FFFFF) | 0x200000;
    fileHandle = reservehandle();
    fop->handle = fileHandle;
    if (fileHandle == 0) {
        fop->error = FERR_NoHandles;
        iFILE_perror(((char*)fop));
    }
    strncpy(fop->handle->name, name, 0x100);
    iFILESYS_ExecCommand(((char*)fop));
    return *(int *)fop;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_close);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern FILEOPERATION_def *reserveop_D5A0(void) __asm__("reserveop");
extern void iFILE_perror_D5A0(FILEOPERATION_def *) __asm__("iFILE_perror");
extern void iFILESYS_ExecCommand_D5A0(FILEOPERATION_def *) __asm__("iFILESYS_ExecCommand");

int FILESYS_close_impl(int filehandle, int priority, void *userdata) __asm__("FILESYS_close");
/**
 * Queues an asynchronous file close and returns the operation handle.
 *
 * Reserves an operation slot (type 3), records @p filehandle, the priority and the user data; when the handle is a
 * mounted BIG archive's (gFileDevice.bigfilelist) the operation is failed with error 3 instead. The command goes to
 * iFILESYS_ExecCommand.
 *
 * @param filehandle  the file handle
 * @param priority  queue priority
 * @param userdata  user data passed to the completion callback
 * @return the operation handle
 */
int FILESYS_close_impl(int filehandle, int priority, void *userdata) {
    // ORIGINAL-PTR: [O] EA's API hands file handles out as ints (On Tour: FILESYS_close(int filehandle, ...),
    //     FILESYS_open returns int) while FILEOPERATION_def::handle is the HANDLE_def* the int carries.
    FILEOPERATION_def *fop;
    struct FILEBIG_def *n;

    fop = reserveop_D5A0();
    n = gFileDevice.bigfilelist;
    fop->op.sep.type = FOPTYPE_close;
    fop->priority = priority;
    fop->userdata = userdata;
    fop->handle = (struct HANDLE_def *)filehandle;
    for (; n != 0; n = n->next) {
        if ((int)n->handle == filehandle) {
            fop->error = FERR_BigClose;
            iFILE_perror_D5A0(fop);
            break;
        }
    }
    iFILESYS_ExecCommand_D5A0(fop);
    return fop->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_read);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern FILEOPERATION_def *reserveop_D648(void) __asm__("reserveop");
extern void iFILE_perror_D648(FILEOPERATION_def *) __asm__("iFILE_perror");
extern void iFILESYS_ExecCommand_D648(FILEOPERATION_def *) __asm__("iFILESYS_ExecCommand");

int FILESYS_read_impl(HANDLE_def *filehandle, int offset, void *buffer, int bytes, int priority, void *userdata)
    __asm__("FILESYS_read");
/**
 * Queues an asynchronous read and returns the operation handle.
 *
 * Reserves an operation slot (type 4), records the handle, offset, buffer, priority and user data, clamps the
 * byte count so the read stops at the end of the file (the handle's length) and hands the command to
 * iFILESYS_ExecCommand. A null handle fails the operation with error 6.
 *
 * @param filehandle  the file handle
 * @param offset  the file offset
 * @param buffer  the destination buffer
 * @param bytes  the number of bytes to read
 * @param priority  queue priority
 * @param userdata  user data passed to the completion callback
 * @return the operation handle
 */
int FILESYS_read_impl(HANDLE_def *filehandle, int offset, void *buffer, int bytes, int priority, void *userdata) {
    FILEOPERATION_def *fop;
    int n;

    fop = reserveop_D648();
    fop->userdata = userdata;
    fop->priority = priority;
    fop->op.sep.type = FOPTYPE_read;
    if (filehandle == 0) {
        fop->error = FERR_InvalidHandle;
        iFILE_perror_D648(fop);
    }
    fop->handle = filehandle;
    n = filehandle->length;
    fop->data = buffer;
    if (n < offset + bytes) bytes = n - offset;
    fop->value = offset;
    fop->amount = bytes;
    iFILESYS_ExecCommand_D648(fop);
    return fop->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_write);
#ifdef SKIP_ASM
extern void iFILE_perror();

/**
 * Queues a write operation (type 5) on an open file; returns its handle. A null file sets error 6 (iFILE_perror).
 *
 * @param file  the file handle
 * @param offset  the file offset
 * @param buffer  the data
 * @param size  the byte count
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_write(int file, int offset, void *buffer, int size, int priority, void *userData) {
    // ORIGINAL-PTR: [O] EA's API hands file handles out as ints (On Tour: FILESYS_write(int filehandle, ...),
    //     FILESYS_open returns int) while FILEOPERATION_def::handle is the HANDLE_def* the int carries.
    FILEOPERATION_def* cmd = (FILEOPERATION_def*)reserveop();
    cmd->userdata = userData;
    *(int *)cmd = (*(int *)cmd & 0xFF0FFFFF) | 0x500000;
    if (file == 0) {
        cmd->error = FERR_InvalidHandle;
        iFILE_perror(((char*)cmd));
    }
    cmd->handle = (struct HANDLE_def*)file;
    cmd->priority = priority;
    cmd->amount = size;
    cmd->value = offset;
    cmd->data = buffer;
    iFILESYS_ExecCommand(((char*)cmd));
    return *(int *)cmd;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_size);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern struct FILEOPERATION_def *reserveop_D7E0() __asm__("reserveop");
extern void iFILE_perror_D7E0(struct FILEOPERATION_def *) __asm__("iFILE_perror");

/**
 * Creates a size operation (type 6) for an open file; returns its handle. The size (the handle's length) is stored as
 * the result (value) and the operation completes at once; a null file sets error 6 instead.
 *
 * @param file  the file handle
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_size(struct HANDLE_def *file, int priority, void *userData) {
    struct FILEOPERATION_def *fop;

    fop = reserveop_D7E0();
    fop->userdata = userData;
    fop->priority = priority;
    fop->op.id = (fop->op.id & 0xFF0FFFFF) | 0x600000;
    if (file != 0) {
        fop->value = file->length;
        fop->status = 1;
    } else {
        fop->error = FERR_InvalidHandle;
        iFILE_perror_D7E0(fop);
    }
    return fop->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_null);
#ifdef SKIP_ASM
extern int *reserveop();
extern void iFILESYS_ExecCommand(int);
extern struct FILEOPERATION_def *reserveop_D878(void) __asm__("reserveop");
extern void iFILESYS_ExecCommand_D878(struct FILEOPERATION_def *) __asm__("iFILESYS_ExecCommand");

/**
 * Queues an operation of type 7 that carries no file and no data (a no-op that completes in queue order); returns its
 * handle. The first argument is not used.
 *
 * @param fileDevice  unused (a file device)
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_null(int fileDevice, int priority, void *userData) {
    struct FILEOPERATION_def *fop = reserveop_D878();
    fop->userdata = userData;
    fop->priority = priority;
    fop->op.id = (fop->op.id & 0xFF0FFFFF) | 0x700000;
    iFILESYS_ExecCommand_D878(fop);
    return fop->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", iFILE_addbigreadcallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* gFileSysOpts: the file system's settings (0x24 bytes: FILESYS_getopts hands out a copy, FILESYS_setopts takes one).
   Field names are their offsets; f04 / f08 are also reached through their own splat symbols. */
struct sC10File { char *name; int a; int b; char *path; };   /* built-in file table entry (f20) */
/* gFileDevice: the file system's command manager (FILESYS_initadr sets it up). Layout from every use in this unit;
   field names are FILE_FILEDEVICE's. Several fields are also reached through their own splat symbols (declared
   below), which the functions that address them directly must keep using. */
extern int FILESYS_completeop_D8E8(int) __asm__("FILESYS_completeop");
extern int BIG_typeofheader_D8E8(int) __asm__("BIG_typeofheader");
extern int BIG_sizeofheader_D8E8(int) __asm__("BIG_sizeofheader");
extern char str_bigfile_buf_D8E8[] __asm__("str_bigfile_buf");
extern void iFILESYS_ExecCommand_D8E8(void *) __asm__("iFILESYS_ExecCommand");
extern char iFILE_addbigreadcallback_D8E8[] __asm__("iFILE_addbigreadcallback");

/**
 * Read callback of adding an archive: completes the read and checks the BIG header; reads the whole header when it is
 * larger than the buffer, else links the archive (gFileDevice.bigfilelist) and runs the add operation.
 *
 * A buffer that does not hold a BIG header (BIG_typeofheader 0) is freed. A larger header gets a new buffer of
 * BIG_sizeofheader bytes and another FILESYS_read with this callback.
 *
 * @param opHandle  the read operation's handle
 * @param opStatus  its status
 * @param bigNode  the archive record
 */
void iFILE_addbigreadcallback(int opHandle, int opStatus, struct FILEBIG_def *bigNode) {
    int n, k;
    struct HANDLE_def *fileHd;
    struct FILESYSOPTS *fs;
    struct FILEOPERATION_def *fileCmd = &gFileDevice.oplist[((OP_def *)&opHandle)->sep.index];
    fileHd = fileCmd->handle;
    k = fileCmd->priority;
    bigNode->handle = fileHd;
    ((struct FILEOPERATION_def*)bigNode->fop)->handle = fileHd;
    FILESYS_completeop_D8E8(opHandle);
    if (BIG_typeofheader_D8E8(((int)bigNode->header)) == 0) {
        gFileSysOpts.free((void *)((int)bigNode->header));
    }
    n = BIG_sizeofheader_D8E8(((int)bigNode->header));
    if (bigNode->headersize < n) {
        fs = &gFileSysOpts;
        fs->free((void *)((int)bigNode->header));
        bigNode->headersize = n;
        (*(int*)&bigNode->header) = (int)fs->malloc(str_bigfile_buf_D8E8, n, bigNode->memflags);
        opHandle = FILESYS_read(bigNode->handle, 0, ((int)bigNode->header), n, k, bigNode);
        FILESYS_callbackop(opHandle, iFILE_addbigreadcallback_D8E8);
    } else {
        bigNode->next = (struct FILEBIG_def*)gFileDevice.bigfilelist;
        gFileDevice.bigfilelist = bigNode;
        iFILESYS_ExecCommand_D8E8(((struct FILEOPERATION_def*)bigNode->fop));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", iFILE_addbigopencallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int FILESYS_completeop_DA10(int) __asm__("FILESYS_completeop");
extern int FILESYS_read_DA10(int, int, void *, int, int, struct FILEBIG_def *) __asm__("FILESYS_read");
extern void FILESYS_callbackop_DA10(int, void *) __asm__("FILESYS_callbackop");
extern void iFILESYS_ExecCommand_DA10(struct FILEOPERATION_def *) __asm__("iFILESYS_ExecCommand");
extern char iFILE_addbigreadcallback_DA10[] __asm__("iFILE_addbigreadcallback");

/**
 * Open callback of adding an archive: on success reads the first 0x800 bytes into the record's header buffer (at the
 * open operation's priority) with callback iFILE_addbigreadcallback; on failure sets error 4 on the add operation (fop)
 * and runs it.
 *
 * @param opHandle  the open operation's handle
 * @param opStatus  its status (1 = opened)
 * @param bigRec  the archive record
 */
void iFILE_addbigopencallback(int opHandle, int opStatus, struct FILEBIG_def *bigRec) {
    // ORIGINAL-PTR: [O] the handle is an int (On Tour: iFILE_addbigopencallback(int ophandle, ...)) and
    //     OP_def the union that splits it; [R] retail stores the argument and reads its index byte through its
    //     address (lbu 0x3 off the spilled word), so EA read the bit-fields in place.
    int priority = gFileDevice.oplist[((OP_def *)&opHandle)->sep.index].priority;
    int fileHandle = FILESYS_completeop_DA10(opHandle);
    if (opStatus != 1) {
        bigRec->fop->error = FERR_BigOpen;
        iFILESYS_ExecCommand_DA10(bigRec->fop);
    } else {
        opHandle = FILESYS_read_DA10(fileHandle, 0, bigRec->header, 0x800, priority, bigRec);
        FILESYS_callbackop_DA10(opHandle, iFILE_addbigreadcallback_DA10);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_addbig);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef struct { unsigned int pad0 : 20; unsigned int f : 4; unsigned int pad1 : 8; int a; int b; int c; int d; int e; int f1; int g; int h; int i; } C_DAC0;
typedef struct Q_DAC0 { int pad[3]; char *name; int p10; struct Q_DAC0 *next; } Q_DAC0;
typedef struct { int buf; int size; int b; int pad; C_DAC0 *cmd; } R_DAC0;
extern C_DAC0 *reserveop_DAC0(void) __asm__("reserveop");
extern void iFILESYS_ExecCommand_DAC0(C_DAC0 *) __asm__("iFILESYS_ExecCommand");
extern int strncmp_DAC0(char *, char *, int) __asm__("strncmp");
extern char str_bigfile_buf_DAC0[] __asm__("str_bigfile_buf");
extern int FILESYS_open_DAC0(char *, int, int, R_DAC0 *) __asm__("FILESYS_open");
extern void fCF70_DAC0(int, void *) __asm__("FILESYS_callbackop");
extern char iFILE_addbigopencallback_DAC0[] __asm__("iFILE_addbigopencallback");

/**
 * Queues adding a BIG archive (type 9); returns the operation handle.
 *
 * An archive already added under that name is linked to the operation, which runs at once. Quirk: that path returns the
 * operation's address, not its handle (On Tour's returns the handle). Otherwise an 0x18-byte record and an 0x800 buffer
 * are allocated (gFileSysOpts alloc) and the file is opened with callback iFILE_addbigopencallback.
 *
 * @param name  the archive path
 * @param memFlags  the memory flags
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_addbig(char *name, int memFlags, int priority, int userData) {
    Q_DAC0 *bigNode = (Q_DAC0*)gFileDevice.bigfilelist;
    C_DAC0 *fileOp = reserveop_DAC0();
    R_DAC0 *bigRec;
    fileOp->f = 9;
    fileOp->e = userData;
    fileOp->d = priority;
    for (; bigNode != 0; bigNode = bigNode->next) {
        if (strncmp_DAC0(name, bigNode->name + 0x10, 0x100) == 0) {
            fileOp->i = (int)bigNode->name;
            iFILESYS_ExecCommand_DAC0(fileOp);
            return (int)fileOp;
        }
    }
    bigNode = gFileSysOpts.malloc(name, 0x18, memFlags);
    ((R_DAC0 *)bigNode)->cmd = fileOp;
    ((R_DAC0 *)bigNode)->size = 0x800;
    ((R_DAC0 *)bigNode)->b = memFlags;
    ((R_DAC0 *)bigNode)->buf = (int)gFileSysOpts.malloc(str_bigfile_buf_DAC0, 0x800, memFlags);
    fCF70_DAC0(FILESYS_open_DAC0(name, 1, priority, (R_DAC0 *)bigNode), iFILE_addbigopencallback_DAC0);
    return *(int *)fileOp;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", iFILE_delbigclosecallback);
#ifdef SKIP_ASM
extern void FILESYS_completeop();
extern void iFILESYS_ExecCommand(int);

/**
 * Close callback of removing an archive: completes the close and runs the remove operation (@p userData).
 *
 * @param request  the close operation's handle
 * @param status  its status
 * @param userData  the remove operation
 */
void iFILE_delbigclosecallback(int request, int status, int userData) {
    FILESYS_completeop();
    iFILESYS_ExecCommand(userData);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_delbig);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern FILEOPERATION_def *reserveop_DC30(void) __asm__("reserveop");
extern int FILESYS_close_DC30(HANDLE_def *, int, FILEOPERATION_def *) __asm__("FILESYS_close");
extern void FILESYS_callbackop_DC30(int, void *) __asm__("FILESYS_callbackop");
extern void iFILE_delbigclosecallback_DC30(int, int, int) __asm__("iFILE_delbigclosecallback");

/**
 * Queues removing a BIG archive (operation type 10); returns the operation handle.
 *
 * Error 7 (FERR_DelBig) when the archive file itself lies inside another archive (its bigoffset is set), 8
 * (FERR_RemoveBig, and status -2) when a file opened from it is still open, 5 (FERR_NoExist) when it is not in the
 * archive list. The record and its header buffer are freed and the file closed with callback
 * iFILE_delbigclosecallback. Quirk: an archive missing from the list still has its (null) node unlinked and freed.
 *
 * @param bigHandle  the archive's open file
 * @param priority  the priority
 * @param userData  the user data
 * @return the operation handle
 */
int FILESYS_delbig(HANDLE_def *bigHandle, int priority, void *userData) {
    FILEBIG_def *prev;
    FILEBIG_def *big;
    FILEOPERATION_def *fileOp;
    HANDLE_def *openFile;
    int i;

    prev = 0;
    big = gFileDevice.bigfilelist;
    fileOp = reserveop_DC30();
    openFile = gFileDevice.handlelist;
    fileOp->userdata = userData;
    fileOp->priority = priority;
    fileOp->op.sep.type = FOPTYPE_delbig;
    if (bigHandle->bigoffset != 0) {
        fileOp->error = FERR_DelBig;
    }
    for (i = 0; i < gFileDevice.maxopen; i++, openFile++) {
        /* a file inside an archive (bigoffset set) keeps the archive's node in info */
        if (openFile != 0 && openFile->bigoffset != 0 && ((FILEBIG_def *)openFile->info)->handle == bigHandle) {
            fileOp->status = -2;
            fileOp->error = FERR_RemoveBig;
            break;
        }
    }
    while (big != 0 && big->handle != bigHandle) {
        prev = big;
        big = big->next;
    }
    if (big == 0) {
        fileOp->error = FERR_NoExist;
    }
    if (prev != 0) {
        prev->next = big->next;
    } else {
        gFileDevice.bigfilelist = gFileDevice.bigfilelist->next;
    }
    gFileSysOpts.free(big->header);
    gFileSysOpts.free(big);
    FILESYS_callbackop_DC30(FILESYS_close_DC30(bigHandle, priority, fileOp), iFILE_delbigclosecallback_DC30);
    return fileOp->op.id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_atomic);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void iFILESYS_ExecCommand(int);

int FILESYS_atomic_DDF0(int (*arg0)(int, int), int arg1, int arg2, int arg3) __asm__("FILESYS_atomic");
/**
 * Runs a file-system callback with the command queue's priority limit set to a given priority.
 *
 * Saves the limit (gFileDevice word 2; only queued commands at or below it may start), sets it to @p priority, calls
 * arg0(arg2, arg3), restores the old limit and then restarts the queue with iFILESYS_ExecCommand(0). The
 * FILE_load family uses it to load synchronously at sync_priority.
 *
 * @param func  callback to run, called with (priority, @p userData)
 * @param fileDevice  unused
 * @param priority  priority limit to use while the callback runs
 * @param userData  argument passed through to the callback
 * @return the callback's result
 */
int FILESYS_atomic_DDF0(int (*func)(int, int), int fileDevice, int priority, int userData) {
    int prevMinPriority;
    int temp_2;

    prevMinPriority = gFileDevice.minpriority;
    gFileDevice.minpriority = priority;
    temp_2 = func(priority, userData);
    gFileDevice.minpriority = prevMinPriority;
    iFILESYS_ExecCommand(0);
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_setminpriority);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void iFILESYS_ExecCommand_DE50(int) __asm__("iFILESYS_ExecCommand");
/**
 * Sets the file system's run limit (gFileDevice +0x8: queued operations run only while their priority is at most this)
 * and restarts the queue (iFILESYS_ExecCommand(0)).
 *
 * @param priorityLimit  the priority limit
 */
void FILESYS_setminpriority(int priorityLimit) {
    gFileDevice.minpriority = priorityLimit;
    iFILESYS_ExecCommand_DE50(0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_setcurrentpath);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int strlen_DE78(char *) __asm__("strlen");
extern char *strcpy_DE78(char *, char *) __asm__("strcpy");
extern int strncmp_DE78(char *, char *, int) __asm__("strncmp");
extern void strcat_DE78(char *, char *) __asm__("strcat");
extern char *REAL_filedevice_DE78[] __asm__("REAL_filedevice");
extern char *str_I_DE78[] __asm__("str_I");
extern int currentfilesystem_DE78[] __asm__("currentfilesystem");
extern int availablefilesystems_DE78[] __asm__("availablefilesystems");
extern char currentdirectory_DE78[] __asm__("currentdirectory");
extern char str_slash_e_DE78[] __asm__("str_slash_e");

/**
 * Sets the file system's current path (currentdirectory) from @p path.
 *
 * With the 3-character device prefix (str_I) the call is ignored when the device mode (availablefilesystems & 3) is 2, else
 * device 1 is selected (currentfilesystem) and the prefix skipped; with the host root prefix (REAL_filedevice) it is ignored unless
 * mode bit 2 is set, else device 2 is selected. The rest is copied and, when it fits, ends with '/'. ps2main passes
 * "cd:".
 *
 * @param path  the path
 */
void FILESYS_setcurrentpath(char *path) {
    int n, len;
    n = strlen_DE78(REAL_filedevice_DE78[0]);
    if (strncmp_DE78(path, str_I_DE78[0], 3) == 0) {
        if ((availablefilesystems_DE78[0] & 3) == 2) return;
        currentfilesystem_DE78[0] = 1;
        path += 3;
    } else if (strncmp_DE78(path, REAL_filedevice_DE78[0], n) == 0) {
        if (!(availablefilesystems_DE78[0] & 2)) return;
        currentfilesystem_DE78[0] = 2;
        path += n;
    }
    strcpy_DE78(currentdirectory_DE78, path);
    if (currentdirectory_DE78[0xFF] == 0) {
        len = strlen_DE78(currentdirectory_DE78);
        if (len > 0 && currentdirectory_DE78[len - 1] != 47) {
            strcat_DE78(currentdirectory_DE78, str_slash_e_DE78);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_getcurrentpath);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char currentdirectory_DF80[] __asm__("currentdirectory");
void strncpy_DF80(char *, char *, int) __asm__("strncpy");
/**
 * Copies the file system's current path (currentdirectory, up to 0x100 bytes) to @p dst.
 *
 * @param dst  the output buffer
 */
void FILESYS_getcurrentpath(char *dst) {
    strncpy_DF80(dst, currentdirectory_DF80, 0x100);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", iFILESYS_ExecCommand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* gFileDevice: the file system's command manager (FILESYS_initadr sets it up). Layout from every use in this unit;
   field names are FILE_FILEDEVICE's. Several fields are also reached through their own splat symbols (declared
   below), which the functions that address them directly must keep using. */
extern void MUTEX_lock_FX(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_FX(void *) __asm__("MUTEX_unlock");
extern char *strchr_FX(char *, int) __asm__("strchr");
extern int FILEDEV_open_FX(char *, int, void *) __asm__("FILEDEV_open");
extern int FILEDEV_getsize_FX(int) __asm__("FILEDEV_getsize");
extern int FILEDEV_getstartsector_FX(int) __asm__("FILEDEV_getstartsector");
extern int FILEDEV_close_FX(int) __asm__("FILEDEV_close");
extern int FILEDEV_readreq_FX(int, int, int, int) __asm__("FILEDEV_readreq");
extern int FILEDEV_write_FX(int, int, int, int) __asm__("FILEDEV_write");
extern int strcmp_FX(char *, char *) __asm__("strcmp");
extern int BIG_locateentryz_FX(int, char *, int, int *, int *) __asm__("BIG_locateentryz");
extern void freehandle_FX(void *) __asm__("freehandle");
extern void iFILESYS_CommandCompleteCallback_FX(int) __asm__("iFILESYS_CommandCompleteCallback");

void iFILESYS_ExecCommand_impl(struct FILEOPERATION_def *c) __asm__("iFILESYS_ExecCommand");
extern struct sAB0Lock D_00519AC4;
extern struct sAB0 gFileDevice_DFA8 __asm__("gFileDevice");
/**
 * Queues a file-system command and, when the device is idle, starts the next queued command.
 *
 * A non-null @p fileOp is inserted into the pending list ordered by priority (after any of equal priority). Nothing
 * starts while a command is current or a completion callback is running, or when the head's priority is above
 * the limit. The started command runs by type (bits 20-23 of its first word): 2 opens and 8 checks a file
 * (a plain name opens a device file, "|name" or "archive|name" looks inside the mounted BIG archives), 3
 * closes, 4 reads, 5 writes, 6 reports the file size, and 7, 9 and 10 complete at once. An aborted command
 * completes at once with status -1.
 *
 * @param fileOp  command to queue, or 0 just to try starting the next one
 */
void iFILESYS_ExecCommand_impl(struct FILEOPERATION_def *fileOp) {
    char path[256];
    char file[256];
    int off;
    int x;
    struct FILEBIG_def *n;
    struct HANDLE_def *hd;
    int mode;
    int found;
    MUTEX_lock_FX(D_00519AC4.mutex);
    if (fileOp != 0) {
        struct FILEOPERATION_def *prev = 0;
        fileOp->next = D_00519AC4.opqhead;
        while (fileOp->next != 0 && !(fileOp->priority < fileOp->next->priority)) {
            prev = fileOp->next;
            fileOp->next = prev->next;
        }
        if (prev != 0) {
            prev->next = fileOp;
        } else {
            gFileDevice_DFA8.opqhead = fileOp;
        }
        gFileDevice_DFA8.numopsqueued++;
    }
    if (gFileDevice_DFA8.exec != 0 || gFileDevice_DFA8.waitlevel != 0) {
        MUTEX_unlock_FX(gFileDevice_DFA8.mutex);
        return;
    }
    fileOp = gFileDevice_DFA8.opqhead;
    if (fileOp != 0 && fileOp->priority <= gFileDevice_DFA8.minpriority) {
        gFileDevice_DFA8.opqhead = fileOp->next;
        gFileDevice_DFA8.numopsqueued--;
    } else {
        fileOp = 0;
    }
    gFileDevice_DFA8.exec = fileOp;
    MUTEX_unlock_FX(gFileDevice_DFA8.mutex);
    if (fileOp == 0) return;
    if (fileOp->cancelled != 0) {
        iFILESYS_CommandCompleteCallback_FX(0);
        return;
    }
    switch ((((unsigned int)fileOp->op.id) >> 20) & 0xF) {
    case FOPTYPE_open:
    case FOPTYPE_exist:
        file[0] = 0;
        if (strchr_FX(fileOp->handle->name, '|')) {
            if (*fileOp->handle->name == '|') {
                mode = 2;
            } else {
                int len;
                char *bar = strchr_FX(fileOp->handle->name, '|');
                mode = 4;
                len = bar - fileOp->handle->name;
                path[0] = 0;
                strncpy(path, fileOp->handle->name, len);
                path[len] = 0;
            }
            strcpy(file, strchr_FX(fileOp->handle->name, '|') + 1);
        } else {
            mode = 1;
            if (fileOp->value & 1) {
                mode = 3;
                strcpy(file, fileOp->handle->name);
            }
        }
        found = 0;
        if ((mode & 1) && FILEDEV_open_FX(fileOp->handle->name, fileOp->value, fileOp->handle)) {
            found = 1;
            if (((((unsigned int)fileOp->op.id) >> 20) & 0xF) == FOPTYPE_exist) {
                FILEDEV_close_FX(fileOp->handle->info);
            } else {
                fileOp->handle->length = FILEDEV_getsize_FX(fileOp->handle->info);
                fileOp->handle->DVDstartsector = FILEDEV_getstartsector_FX(fileOp->handle->info);
            }
        }
        if (!found && (mode & 6)) {
            for (n = gFileDevice_DFA8.bigfilelist; n != 0 && !found; n = n->next) {
                if ((mode & 4) && strcmp_FX(n->handle->name, path)) continue;
                if (BIG_locateentryz_FX(((int)n->header), file, 0, &x, &off)) {
                    found = 1;
                    fileOp->handle->info = (int)n;
                    fileOp->handle->length = off;
                    fileOp->handle->bigoffset = x;
                }
            }
        }
        if (((((unsigned int)fileOp->op.id) >> 20) & 0xF) == FOPTYPE_exist) {
            freehandle_FX(fileOp->handle);
            fileOp->value = found;
        }
        iFILESYS_CommandCompleteCallback_FX(found);
        break;
    case FOPTYPE_close:
        fileOp->error = 1;
        hd = fileOp->handle;
        if (hd != 0) {
            if (hd->bigoffset == 0) {
                fileOp->error = FILEDEV_close_FX(hd->info);
            } else {
                fileOp->error = 0;
            }
            freehandle_FX(fileOp->handle);
            fileOp->handle = 0;
        }
        iFILESYS_CommandCompleteCallback_FX(fileOp->error == 0);
        break;
    case FOPTYPE_read:
        if (fileOp->amount > 0) {
            if (fileOp->handle->bigoffset != 0) {
                FILEDEV_readreq_FX(((struct HANDLE_def*)((struct FILEBIG_def *)fileOp->handle->info)->handle)->info, ((int)fileOp->data), fileOp->handle->bigoffset + fileOp->value, fileOp->amount);
            } else {
                FILEDEV_readreq_FX(fileOp->handle->info, ((int)fileOp->data), fileOp->value, fileOp->amount);
            }
        } else {
            iFILESYS_CommandCompleteCallback_FX(1);
        }
        break;
    case FOPTYPE_write:
        FILEDEV_write_FX(fileOp->handle->info, ((int)fileOp->data), fileOp->value, fileOp->amount);
        break;
    case FOPTYPE_size:
        fileOp->value = fileOp->handle->length;
        iFILESYS_CommandCompleteCallback_FX(1);
        break;
    case FOPTYPE_null:
    case FOPTYPE_addbig:
    case FOPTYPE_delbig:
        iFILESYS_CommandCompleteCallback_FX(fileOp->error == 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", iFILESYS_CommandCompleteCallback);
#ifdef SKIP_ASM
struct G_IFS { char pad[0x10]; volatile int busy; char pad2[0x10]; struct FILEOPERATION_def *cur; };
extern void iFILESYS_ExecCommand_IFS(int) __asm__("iFILESYS_ExecCommand");

void iFILESYS_CommandCompleteCallback_impl(int ok) __asm__("iFILESYS_CommandCompleteCallback");
extern struct G_IFS gFileDevice_IFS __asm__("gFileDevice");
/**
 * Finishes the current file-system command: sets its status, runs its callback and starts the next one.
 *
 * The status is -1 for an aborted command, otherwise 1 on success and -2 on failure; the command's callback gets
 * (handle, status, user data). The nesting count is raised around the callback, and the queue is restarted
 * with iFILESYS_ExecCommand(0) once it drops back to 0. With no current command it returns early and leaves
 * the count raised.
 *
 * @param ok  nonzero if the command succeeded
 */
void iFILESYS_CommandCompleteCallback_impl(int ok) {
    struct FILEOPERATION_def *fileOp;
    gFileDevice_IFS.busy++;
    fileOp = gFileDevice_IFS.cur;
    if (fileOp == 0) {
        return;
    }
    if (fileOp->cancelled != 0) {
        fileOp->status = -1;
    } else {
        fileOp->status = ok ? 1 : -2;
    }
    gFileDevice_IFS.cur = 0;
    if (fileOp->callback != 0) {
        fileOp->callback(fileOp->op.id, fileOp->status, ((int)fileOp->userdata));
    }
    gFileDevice_IFS.busy--;
    if (gFileDevice_IFS.busy == 0) {
        iFILESYS_ExecCommand_IFS(0);
    }
}
#endif

/**
 * Does nothing. No callers.
 */
void func_003DE4C8(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_bypassqueuefileinfo);
#ifdef SKIP_ASM
extern char *strchr_E4D0(char *, int) __asm__("strchr");
extern int FILEDEV_open_E4D0(char *, int, int *) __asm__("FILEDEV_open");
extern int FILEDEV_getsize_E4D0(int) __asm__("FILEDEV_getsize");
extern int FILEDEV_close_E4D0(int) __asm__("FILEDEV_close");
extern int strcmp_E4D0(char *, char *) __asm__("strcmp");
extern int BIG_locateentryz_E4D0(int, char *, int, int *, int *) __asm__("BIG_locateentryz");
/**
 * Looks a file up immediately, without going through the command queue, and reports its size.
 *
 * Uses the same name rules as the queued open: a plain name tries a device file only (and also the BIG
 * archives when bit 0 of @p flags is set), "|name" searches every mounted archive, and "archive|name" only
 * the archive with that name. A device file is opened, sized and closed again.
 *
 * @param name   file name, optionally with an archive prefix
 * @param flags  open flags; bit 0 also searches the archives for a plain name
 * @param out    receives the file's size, or 0 when it is not found
 * @return 1 if the file was found, otherwise 0
 */
int FILESYS_bypassqueuefileinfo(char *name, int flags, int *out) {
    char path[256];
    char file[256];
    int handle;
    int x;
    int off;
    int found = 0;
    int mode;
    struct FILEBIG_def *n;

    *out = 0;
    file[0] = 0;
    if (strchr_E4D0(name, '|')) {
        if (*name == '|') {
            mode = 2;
        } else {
            int len = strchr_E4D0(name, '|') - name;
            mode = 4;
            strncpy(path, name, len);
            path[len] = 0;
        }
        strcpy(file, strchr_E4D0(name, '|') + 1);
    } else {
        mode = 1;
        if (flags & 1) {
            mode = 3;
            strcpy(file, name);
        }
    }
    if ((mode & 1) && FILEDEV_open_E4D0(name, flags, &handle)) {
        found = 1;
        *out = FILEDEV_getsize_E4D0(handle);
        FILEDEV_close_E4D0(handle);
    }
    if (!found && (mode & 6)) {
        for (n = (struct FILEBIG_def*)gFileDevice.bigfilelist; n != 0 && !found; n = n->next) {
            if ((mode & 4) && strcmp_E4D0(n->handle->name, path)) continue;
            if (BIG_locateentryz_E4D0(((int)n->header), file, 0, &x, &off)) {
                found = 1;
                *out = off;
            }
        }
    }
    return found;
}
#endif

/**
 * Does nothing: the file operation error report, empty in this build. The op creators call it after setting an error.
 */
void iFILE_perror(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", reserveop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int opcount_E670 __asm__("opcount");
extern void MUTEX_unlock_E670(void *) __asm__("MUTEX_unlock");

struct FILEOPERATION_def *reserveop_impl(void) __asm__("reserveop");
/**
 * Takes a free file operation (gFileDevice +0x28, 0x30 bytes each) under the mutex: marks it in use and stamps its index
 * and a new 20-bit id (opcount, wrapping to 1). Spins forever when none is free.
 *
 * @return the operation
 */
struct FILEOPERATION_def *reserveop_impl(void) {
    int i = 0;
    int nextId;
    MUTEX_lock(gFileDevice.mutex);
    for (; i < gFileDevice.maxfileops; i++) {
        if (((struct sFsCmdId*)&gFileDevice.oplist[i].op)->type == 0) {
            ((struct sFsCmdId*)&gFileDevice.oplist[i].op)->type = 1;
            ((struct sFsCmdId*)&gFileDevice.oplist[i].op)->idx = i;
            nextId = opcount_E670;
            ((struct sFsCmdId*)&gFileDevice.oplist[i].op)->id = nextId;
            opcount_E670 = nextId + 1;
            if (nextId + 1 > 0xFFFFF) {
                opcount_E670 = 1;
            }
            break;
        }
    }
    MUTEX_unlock_E670(gFileDevice.mutex);
    if (i == gFileDevice.maxfileops) {
        while (1) {
        }
    }
    return &gFileDevice.oplist[i];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", freeop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock(void *);

/**
 * Releases a file operation slot.
 *
 * Clears its 0x30 bytes with MEM_fill while holding the file-system mutex.
 *
 * @param fop  the operation
 */
void freeop(int fop) {
    MUTEX_lock(gFileDevice.mutex);
    MEM_fill(fop, 0, 0x30);
    MUTEX_unlock(gFileDevice.mutex);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", reservehandle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_E800(void *) __asm__("MUTEX_unlock");
struct HANDLE_def *reservehandle_impl(void) __asm__("reservehandle");
/**
 * Takes a free file handle slot (gFileDevice.handlelist, maxopen entries) under the mutex: a free slot has info 0, a
 * taken one gets 1 until the open fills it. Spins forever when none is free.
 *
 * @return the handle slot
 */
struct HANDLE_def *reservehandle_impl(void) {
    int i;
    MUTEX_lock(((char*)&gFileDevice.mutex[0]));
    for (i = 0; i < gFileDevice.maxopen; i++) {
        if (gFileDevice.handlelist[i].info == 0) {
            gFileDevice.handlelist[i].info = 1;
            break;
        }
    }
    MUTEX_unlock_E800(((char*)&gFileDevice.mutex[0]));
    if (i == gFileDevice.maxopen) {
        for (;;) {
        }
    }
    return &gFileDevice.handlelist[i];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", freehandle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock(void *);

/**
 * Releases a file handle slot.
 *
 * Clears its 0x110 bytes with MEM_fill while holding the file-system mutex.
 *
 * @param handle  the handle
 */
void freehandle(int handle) {
    MUTEX_lock(gFileDevice.mutex);
    MEM_fill(handle, 0, 272);
    MUTEX_unlock(gFileDevice.mutex);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_setmemcallbacks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

extern int gFileSysOpts_E910[4] __asm__("gFileSysOpts");
/**
 * Sets the file system's allocate and free callbacks (gFileSysOpts +0x4 and +0x8).
 *
 * @param mallocFn  the allocate function
 * @param freeFn  the free function
 */
void FILESYS_setmemcallbacks(int mallocFn, int freeFn) {
    gFileSysOpts_E910[1] = mallocFn;
    gFileSysOpts_E910[2] = freeFn;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_getopts);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* gFileSysOpts (FILESYSOPTS's names at these offsets) and the live CD settings FILESYS_cdSettings (CDVD_initFileSystem's
   CdCfg_4040 names, names ours). */
struct A_E928 { int size, malloc, free, cdMediaMode, cdNameLen, useDisc, cdRoot, REAL_filedevice; };
struct B_E928 { int root, namelen, mmode, useDisc; };
extern struct B_E928 FILESYS_cdSettings_E928 __asm__("FILESYS_cdSettings");
extern int REAL_filedevice_E928[] __asm__("REAL_filedevice");
extern void MEM_clear_E928(int, int) __asm__("MEM_clear");
extern void MEM_copy_E928(int *, void *, int) __asm__("MEM_copy");

extern struct A_E928 gFileSysOpts_E928 __asm__("gFileSysOpts");
/**
 * Copies the file system's options (gFileSysOpts) to the caller's record, whose first word is its size; a record of 37
 * bytes or more is cleared first. Refreshes the copied fields from the live settings first.
 *
 * @param opts  the options record (size first)
 * @return 1
 */
int FILESYS_getopts(int *opts) {
    int n = *opts;
    if (n >= 37) {
        MEM_clear_E928(opts, n);
    }
    gFileSysOpts_E928.cdRoot = FILESYS_cdSettings_E928.root;
    gFileSysOpts_E928.cdNameLen = FILESYS_cdSettings_E928.namelen;
    gFileSysOpts_E928.useDisc = FILESYS_cdSettings_E928.useDisc != 0;
    gFileSysOpts_E928.REAL_filedevice = REAL_filedevice_E928[0];
    MEM_copy_E928(opts, &gFileSysOpts_E928, n);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_setopts);
#ifdef SKIP_ASM
/* The live CD settings (CDVD_initFileSystem's CdCfg_4040 names, names ours). */
struct B_E9B8 { int root, namelen, mmode, useDisc; };
extern struct B_E9B8 FILESYS_cdSettings_E9B8 __asm__("FILESYS_cdSettings");
extern int REAL_filedevice_E9B8[] __asm__("REAL_filedevice");
extern void MEM_clear_E9B8(void *, int) __asm__("MEM_clear");
extern void MEM_copy_E9B8(void *, int *, int) __asm__("MEM_copy");

/**
 * Takes the file system's options from the caller's record (size first; a short record clears the 36-byte options
 * first) and applies them to the live settings (FILESYS_cdSettings, REAL_filedevice).
 *
 * @param opts  the options record (size first)
 * @return 1
 */
int FILESYS_setopts(int *opts) {
    if (*opts < 36) {
        MEM_clear_E9B8(&gFileSysOpts, 36);
    }
    MEM_copy_E9B8(&gFileSysOpts, opts, *opts);
    FILESYS_cdSettings_E9B8.root = ((int)gFileSysOpts.cdRoot);
    FILESYS_cdSettings_E9B8.namelen = gFileSysOpts.cdNameLen;
    FILESYS_cdSettings_E9B8.mmode = gFileSysOpts.cdMediaMode;
    FILESYS_cdSettings_E9B8.useDisc = gFileSysOpts.useDisc;
    REAL_filedevice_E9B8[0] = ((int)gFileSysOpts.REAL_filedevice);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", synccallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct S_EA48 { volatile int e, a, b, d, r, n; int c; int (*fn)(int, int, int, int, int, void *); int h; };
extern int FILESYS_completeop_EA48(int) __asm__("FILESYS_completeop");
extern char synccallback_EA48[] __asm__("synccallback");

/**
 * Chunk callback of a blocking transfer: completes the chunk, advances buffer, offset and total by its size, and starts
 * the next chunk (at most 0x8000 bytes) through the job's transfer function with this callback, until the bytes run out
 * or a chunk comes back short. Clears the remaining count when it stops.
 *
 * @param request  the chunk operation's handle
 * @param mode  its status (1 = done)
 * @param job  the transfer job
 */
void synccallback(int request, int mode, struct S_EA48 *job) {
    int n = FILESYS_completeop_EA48(request);
    job->h = 0;
    if (mode == 1) {
        job->b += n;
        job->c += n;
        job->r += n;
        if (n < job->n) {
            job->d = 0;
        } else {
            job->d -= n;
        }
        if (job->d <= 0) {
            return;
        }
        if (job->d > 0x8000) {
            job->n = 0x8000;
        } else {
            job->n = job->d;
        }
        job->h = job->fn(job->a, job->b, job->c, job->n, job->e, job);
        if (job->h != 0) {
            FILESYS_callbackop(job->h, synccallback_EA48);
            return;
        }
    }
    job->d = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", syncblockio);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void FILESYS_callbackop_EB50(int, void (*)(void)) __asm__("FILESYS_callbackop");
extern int FILESYS_waitop(int);
extern void synccallback_EB50(void) __asm__("synccallback");

/**
 * Performs a synchronous read or write in blocks of at most 0x8000 bytes.
 *
 * Starts each block with @p fn (FILESYS_read or a write), chains the next block from the completion callback
 * synccallback and waits for the operation with FILESYS_waitop.
 *
 * @param filehandle   the file handle
 * @param offset   the file offset
 * @param buffer   the buffer
 * @param bytes   the total number of bytes
 * @param priority   queue priority
 * @param fn  the asynchronous transfer function to use
 * @return the number of bytes transferred
 */
int syncblockio(int filehandle, int offset, int buffer, int bytes, int priority, int (*fn)(int, int, int, int, int, void *)) {
    struct S_EA48 desc;
    desc.e = priority;
    desc.a = filehandle;
    desc.b = offset;
    desc.d = bytes;
    desc.r = 0;
    desc.n = bytes;
    desc.c = buffer;
    desc.fn = fn;
    if (desc.n > 0x8000) {
        desc.n = 0x8000;
    }
    desc.h = fn(filehandle, offset, desc.c, desc.n, priority, &desc);
    if (desc.h) {
        FILESYS_callbackop_EB50(desc.h, synccallback_EB50);
        do {
            FILESYS_waitop(desc.h);
        } while (desc.d || desc.h);
    }
    return desc.r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_opensync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_open_EBF0(int, int, int, int) __asm__("FILESYS_open");
extern void FILESYS_waitop_EBF0(int) __asm__("FILESYS_waitop");
extern int FILESYS_opstatus_EBF0(int) __asm__("FILESYS_opstatus");
extern int FILESYS_completeop_EBF0(int) __asm__("FILESYS_completeop");

int FILESYS_opensync_impl(int a0, int a1, int a2, int *a3) __asm__("FILESYS_opensync");
/**
 * Opens a file and waits: returns 1 when the open succeeded (status 1) and stores the file handle (0 when the operation
 * could not be queued).
 *
 * @param name  the path
 * @param mode  the open mode flags
 * @param priority  the priority
 * @param outHandle  receives the file handle
 * @return 1 on success, else 0
 */
int FILESYS_opensync_impl(int name, int mode, int priority, int *outHandle) {
    int ok = 0;
    int request = FILESYS_open_EBF0(name, mode, priority, 0);
    if (request != 0) {
        FILESYS_waitop_EBF0(request);
        ok = FILESYS_opstatus_EBF0(request) == 1;
        *outHandle = FILESYS_completeop_EBF0(request);
    } else {
        *outHandle = 0;
    }
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_readsync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void FILESYS_read_EC60(void) __asm__("FILESYS_read");
extern void syncblockio_EC60(int, int, int, int, int, void (*)(void)) __asm__("syncblockio");

/**
 * Reads from a file synchronously.
 *
 * Calls syncblockio with FILESYS_read as the transfer function.
 *
 * @param filehandle  the file handle
 * @param offset  the file offset
 * @param buffer  the destination buffer
 * @param bytes  the number of bytes
 * @param priority  queue priority
 */
void FILESYS_readsync(int filehandle, int offset, int buffer, int bytes, int priority) {
    syncblockio_EC60(filehandle, offset, buffer, bytes, priority, FILESYS_read_EC60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_writesync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void FILESYS_write_EC80(void) __asm__("FILESYS_write");
extern void syncblockio_EC60(int, int, int, int, int, void (*)(void)) __asm__("syncblockio");

/**
 * Writes to a file synchronously.
 *
 * Calls syncblockio with FILESYS_write as the transfer function.
 *
 * @param filehandle  the file handle
 * @param offset  the file offset
 * @param buffer  the source buffer
 * @param bytes  the number of bytes
 * @param priority  queue priority
 */
void FILESYS_writesync(int filehandle, int offset, int buffer, int bytes, int priority) {
    syncblockio_EC60(filehandle, offset, buffer, bytes, priority, FILESYS_write_EC80);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_closesync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_waitop_ECA0(int) __asm__("FILESYS_waitop");
int FILESYS_completeop_ECA0(int) __asm__("FILESYS_completeop");
int FILESYS_close_ECA0(int, int, int) __asm__("FILESYS_close");

/**
 * Closes a file synchronously.
 *
 * Queues the close with FILESYS_close, waits for it with FILESYS_waitop and collects the result with
 * FILESYS_completeop.
 *
 * @param filehandle  the file handle
 * @param priority  queue priority
 * @return 1 when the close succeeded, otherwise 0
 */
int FILESYS_closesync(int filehandle, int priority) {
    int temp_2;
    int var_2 = 0;

    temp_2 = FILESYS_close_ECA0(filehandle, priority, 0);
    if (temp_2 != 0) {
        FILESYS_waitop_ECA0(temp_2);
        var_2 = FILESYS_completeop_ECA0(temp_2) != 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_sizesync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_waitop_ECF8(int) __asm__("FILESYS_waitop");
int FILESYS_completeop_ECF8(int) __asm__("FILESYS_completeop");
int FILESYS_size_ECF8(int, int, int) __asm__("FILESYS_size");

/**
 * Returns a file's size, waiting for the size operation (FILESYS_size); 0 when it could not be queued.
 *
 * @param file  the file handle
 * @param priority  the priority
 * @return the size in bytes
 */
int FILESYS_sizesync(int file, int priority) {
    int temp_2;
    int var_2 = 0;

    temp_2 = FILESYS_size_ECF8(file, priority, 0);
    if (temp_2 != 0) {
        FILESYS_waitop_ECF8(temp_2);
        var_2 = FILESYS_completeop_ECF8(temp_2);
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_addbigsync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_addbig_ED50(int, int, int, int) __asm__("FILESYS_addbig");
extern void FILESYS_waitop_ED50(int) __asm__("FILESYS_waitop");
extern int FILESYS_opstatus_ED50(int) __asm__("FILESYS_opstatus");
extern int FILESYS_completeop_ED50(int) __asm__("FILESYS_completeop");

/**
 * Adds a BIG archive and waits: returns 1 when it was added (status 1) and stores the archive handle (0 when the
 * operation could not be queued). The game mounts its .big files with it.
 *
 * @param name  the archive path
 * @param allocFlags  the memory flags
 * @param priority  the priority
 * @param outResult  receives the archive handle
 * @return 1 on success, else 0
 */
int FILESYS_addbigsync(int name, int allocFlags, int priority, int *outResult) {
    int ok = 0;
    int request = FILESYS_addbig_ED50(name, allocFlags, priority, 0);
    if (request != 0) {
        FILESYS_waitop_ED50(request);
        ok = FILESYS_opstatus_ED50(request) == 1;
        *outResult = FILESYS_completeop_ED50(request);
    } else {
        *outResult = 0;
    }
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_delbigsync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_waitop_EDC0(int) __asm__("FILESYS_waitop");
int FILESYS_completeop_EDC0(int) __asm__("FILESYS_completeop");
int FILESYS_delbig_EDC0(int, int, int) __asm__("FILESYS_delbig");

/**
 * Removes a BIG archive and waits; returns nonzero when the operation completed with a nonzero result.
 *
 * @param file  the archive handle
 * @param priority  the priority
 * @return nonzero on success, 0 when it failed or could not be queued
 */
int FILESYS_delbigsync(int file, int priority) {
    int temp_2;
    int var_2 = 0;

    temp_2 = FILESYS_delbig_EDC0(file, priority, 0);
    if (temp_2 != 0) {
        FILESYS_waitop_EDC0(temp_2);
        var_2 = FILESYS_completeop_EDC0(temp_2) != 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILESYS_existssync);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_waitop_EE18(int) __asm__("FILESYS_waitop");
int FILESYS_completeop_EE18(int) __asm__("FILESYS_completeop");
int FILESYS_exists_EE18(int, int, int) __asm__("FILESYS_exists");

/**
 * Returns whether a file exists, waiting for the exists operation (FILESYS_exists).
 *
 * @param name  the path
 * @param priority  the priority
 * @return nonzero when it exists
 */
int FILESYS_existssync(int name, int priority) {
    int temp_2;
    int var_2 = 0;

    temp_2 = FILESYS_exists_EE18(name, priority, 0);
    if (temp_2 != 0) {
        FILESYS_waitop_EE18(temp_2);
        var_2 = FILESYS_completeop_EE18(temp_2) != 0;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", queueadd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_qa(void *) __asm__("MUTEX_unlock");
extern char hlafile_mutex_9C08[] __asm__("hlafile_mutex");

/**
 * Appends an async-file request to the end of a request queue (head, tail), under the async-file mutex.
 *
 * @param list  the queue
 * @param node  the request to append; its link (nextreq) is cleared
 */
void queueadd(REQUESTQUEUE *list, REQUESTSTRUCTtag *node) {
    MUTEX_lock(hlafile_mutex_9C08);
    if (list->head == 0) {
        list->head = node;
    } else {
        list->tail->nextreq = node;
    }
    list->tail = node;
    node->nextreq = 0;
    MUTEX_unlock_qa(hlafile_mutex_9C08);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", queuefetch);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_EED8(void *) __asm__("MUTEX_unlock");
extern char hlafile_mutex_9C08[] __asm__("hlafile_mutex");

/**
 * Takes the first request off a request queue, or returns null when it is empty.
 *
 * The head (+0x0) advances to the request's next link (+0x4) under hlafile_mutex.
 *
 * @param queue  the queue
 * @return the request, or null
 */
void *queuefetch(void **queue) {
    REQUESTSTRUCTtag *temp_2;
    REQUESTSTRUCTtag* req;

    MUTEX_lock(hlafile_mutex_9C08);
    temp_2 = *queue;
    req = 0;
    if (temp_2 != 0) {
        req = temp_2;
        *queue = req->nextreq;
    }
    MUTEX_unlock_EED8(hlafile_mutex_9C08);
    return req;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", newrequestid);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int hlafile_requestidcounter_EF40[] __asm__("hlafile_requestidcounter");

/**
 * Gives an async-file request a fresh id.
 *
 * hlafile_requestidcounter advances by 0x100 (wrapping to 0x100, never 0) and is ORed above the slot index in
 * the request's first byte.
 *
 * @param req  the request
 */
void newrequestid(unsigned char *req) {
    int temp_2;

    temp_2 = hlafile_requestidcounter_EF40[0] + 0x100;
    hlafile_requestidcounter_EF40[0] = temp_2;
    if (temp_2 == 0) {
        hlafile_requestidcounter_EF40[0] = 0x100;
    }
    *(int *)req = (int) (*req | hlafile_requestidcounter_EF40[0]);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", locaterequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern REQUESTSTRUCTtag *hlafile_request_0C34 __asm__("hlafile_request");
extern int numrequests_9BF0[] __asm__("numrequests");

/**
 * Finds the async-file request with an id, or null when the id is stale or invalid.
 *
 * Ids below 0x100 or with a slot (low byte) at or past numrequests are rejected; otherwise the slot in
 * hlafile_request (0x30 bytes each) is returned when its id word equals @p requestId.
 *
 * @param requestId  the request id
 * @return the request, or null
 */
REQUESTSTRUCTtag *locaterequest(int requestId) {
    REQUESTSTRUCTtag *temp_2;
    int temp_6;

    temp_6 = requestId & 0xFF;
    if ((requestId < 0x100) || (temp_6 >= numrequests_9BF0[0])) {
        return 0;
    }
    temp_2 = &hlafile_request_0C34[temp_6];
    return ((temp_2->id ^ requestId) != 0) ? 0 : temp_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", releaserequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void queueadd_rr(char *, char *) __asm__("queueadd");
extern char freequeue_9BF8[] __asm__("freequeue");

/**
 * Returns an async-file request to the free queue, freeing its buffer through the free hook when it owns one.
 *
 * The request's in-flight operation is cleared and its flags reduced to the low byte.
 *
 * @param request  the request
 */
void releaserequest(REQUESTSTRUCTtag* request) {
    unsigned int temp_4;

    if (request->cancelflag != 0) {
        temp_4 = (unsigned int)request->loadaddr;
        if (temp_4 >= 2U) {
            ((void (*)(unsigned int))gFileSysOpts.free)(temp_4);
        }
    }
    request->fop = 0;
    *(int *)request = *(unsigned char *)request;
    queueadd_rr(freequeue_9BF8, ((char*)request));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", finishrequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char hlafile_mutex_F028[] __asm__("hlafile_mutex");
extern void MUTEX_lock_F028(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_F028(void *) __asm__("MUTEX_unlock");
extern void releaserequest_F028(void *) __asm__("releaserequest");

/**
 * Completes an async-file request: notifies its owner or frees it if it was cancelled.
 *
 * Clears the busy flag (+0x1C) under hlafile_mutex. A cancelled request (+0x10) that is not being waited on
 * (+0xC) is freed with releaserequest; otherwise the callback (+0x18) is called with the request id.
 *
 * @param req  the request
 */
void finishrequest(REQUESTSTRUCTtag* req) {
    int (*callback)(int);
    int cancelFlag;
    int waitFlag;

    MUTEX_lock_F028(hlafile_mutex_F028);
    req->fop = 0;
    cancelFlag = (req->cancelflag);
    callback = ((int (*)(int))req->callback);
    waitFlag = (req->releaseflag);
    MUTEX_unlock_F028(hlafile_mutex_F028);
    if (cancelFlag != 0) {
        if (waitFlag == 0) {
            releaserequest_F028(req);
        }
    } else if (callback != 0) {
        callback((req->id));
    }
}
#endif

#ifdef SKIP_ASM
extern struct REQUESTSTRUCTtag *hlafile_request;
struct sReqQueue { struct REQUESTSTRUCTtag *f00; struct REQUESTSTRUCTtag *f04; };  /* head, tail */
extern struct sReqQueue freequeue;             /* free requests */
extern char hlafile_mutex[];                       /* request list mutex */
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadfileclosecallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void FILESYS_completeop_F0B8(int) __asm__("FILESYS_completeop");

/* Callback (third argument is the stream); later code takes its address as void (*)(void *). */
void loadfileclosecallback_impl(int arg0, int arg1, struct REQUESTSTRUCTtag *s) __asm__("loadfileclosecallback");
/**
 * Close callback of an asynchronous file load: completes the close and finishes the request (finishrequest).
 *
 * @param opHandle  the close operation's handle
 * @param opStatus  its status
 * @param request  the load request
 */
void loadfileclosecallback_impl(int opHandle, int opStatus, struct REQUESTSTRUCTtag *request) {
    FILESYS_completeop_F0B8(request->fop);
    finishrequest((REQUESTSTRUCTtag*)request);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", loadfilereadcallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void FILESYS_callbackop(int, void *);
extern int FILESYS_completeop_F0F0(int) __asm__("FILESYS_completeop");
extern int FILESYS_close(int, int, void *);
extern int FILESYS_read(int, int, int, int, int, void *);
extern void loadfileclosecallback(void *);

/**
 * Read callback of an asynchronous file load: adds the chunk to the bytes read and the destination, then reads the next
 * chunk (at most 0x8000 bytes, priority 99) with this callback, or closes the file (callback loadfileclosecallback) once a
 * chunk comes back short or the request was cancelled (+0x10).
 *
 * @param request  the read operation's handle
 * @param status  its status
 * @param job  the load request
 */
void loadfilereadcallback(int request, int status, int *job) {
    int n = FILESYS_completeop_F0F0(*(volatile int *)&job[7]);
    job[2] += n;
    job[11] += n;
    if (n <= 0x7FFF || job[4] != 0) {
        *(volatile int *)&job[7] = FILESYS_close(job[8], 99, job);
        if (*(volatile int *)&job[7] != 0) {
            FILESYS_callbackop(*(volatile int *)&job[7], loadfileclosecallback);
        }
    } else {
        int off = job[9] + n;
        int rem = job[10] - n;
        job[9] = off;
        job[10] = rem;
        *(volatile int *)&job[7] = FILESYS_read(job[8], off, job[11], rem <= 0x8000 ? rem : 0x8000, 99, job);
        if (*(volatile int *)&job[7] != 0) {
            FILESYS_callbackop(*(volatile int *)&job[7], loadfilereadcallback);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", loadfilesizecallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void fCF70_F1E0(int, char *) __asm__("FILESYS_callbackop");
extern int fD310_F1E0(int) __asm__("FILESYS_completeop");
extern int fD5A0_F1E0(int, int, void *) __asm__("FILESYS_close");
extern int fD648_F1E0(int, int, int, int, int, void *) __asm__("FILESYS_read");
extern char D_5DA8_F1E0[] __asm__("D_00495DA8");
extern char fF0B8_F1E0[] __asm__("loadfileclosecallback");
extern char fF0F0_F1E0[] __asm__("loadfilereadcallback");

/**
 * Continues an async file load once its size is known: allocates the buffer and starts reading, or closes a
 * cancelled load.
 *
 * The buffer comes from the allocator hook ("ASYNCFILEBUF", D_00450C14); reads go in chunks of at most 32 KB.
 *
 * @param opHandle  unused (callback argument)
 * @param opStatus  unused (callback argument)
 * @param userdata  the load request (On Tour void* userdata)
 */
void loadfilesizecallback(int opHandle, int opStatus, REQUESTSTRUCTtag* userdata) {
    int fileSize, buffer, readSize, old;
    fileSize = fD310_F1E0(userdata->fop);
    if (userdata->cancelflag != 0) {
        userdata->fop = fD5A0_F1E0(userdata->fhandle, 99, userdata);
        if (userdata->fop != 0) fCF70_F1E0(userdata->fop, fF0B8_F1E0);
    } else {
        old = userdata->readsize;
        userdata->readsize = fileSize;
        buffer = ((int (*)(char*, int, int))gFileSysOpts.malloc)(D_5DA8_F1E0, fileSize, old);
        readSize = userdata->readsize;
        userdata->loadaddr = (char*)buffer;
        userdata->destaddr = (char*)buffer;
        userdata->fop = fD648_F1E0(userdata->fhandle, userdata->foffset, buffer, readSize <= 0x8000 ? readSize : 0x8000, 99, userdata);
        if (userdata->fop != 0) fCF70_F1E0(userdata->fop, fF0F0_F1E0);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadfileopencallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int FILESYS_completeop_F2C0(int) __asm__("FILESYS_completeop");
extern int FILESYS_size_F2C0(int, int, void *) __asm__("FILESYS_size");
extern char loadfileclosecallback_F2C0[] __asm__("loadfileclosecallback");
extern char loadfilereadcallback_F2C0[] __asm__("loadfilereadcallback");
extern char loadfilesizecallback_F2C0[] __asm__("loadfilesizecallback");

/**
 * Open callback of an asynchronous file load: stores the file handle (+0x20) and starts the next step; a failed open
 * finishes the request.
 *
 * A cancelled request (+0x10) closes the file (loadfileclosecallback); a load into a given address (+0x14 clear) reads the
 * first chunk (loadfilereadcallback); otherwise it asks for the file size first (FILESYS_size, then loadfilesizecallback).
 *
 * @param opHandle  the open operation's handle
 * @param opStatus  its status
 * @param request  the load request
 */
void loadfileopencallback(int opHandle, int opStatus, struct REQUESTSTRUCTtag *request) {
    int n = FILESYS_completeop_F2C0(request->fop);
    request->fhandle = n;
    if (n == 0) {
        finishrequest((REQUESTSTRUCTtag*)request);
        return;
    }
    if (request->cancelflag != 0) {
        request->fop = FILESYS_close(n, 99, request);
        if (request->fop != 0) {
            FILESYS_callbackop(request->fop, loadfileclosecallback_F2C0);
        }
    } else if ((request->loadaddr) == 0) {
        int remaining = request->readsize;
        request->fop = FILESYS_read(n, request->foffset, ((int)request->destaddr), remaining <= 0x8000 ? remaining : 0x8000, 99, request);
        if (request->fop != 0) {
            FILESYS_callbackop(request->fop, loadfilereadcallback_F2C0);
        }
    } else {
        request->fop = FILESYS_size_F2C0(n, 99, request);
        if (request->fop != 0) {
            FILESYS_callbackop(request->fop, loadfilesizecallback_F2C0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", readfilereadcallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_completeop_F3C0(int) __asm__("FILESYS_completeop");

/**
 * Read callback of an asynchronous read (ASYNCFILE_read): adds the chunk to the bytes read and the destination, and
 * reads the next chunk (at most 0x8000 bytes, priority 99) with this callback; a short chunk or a cancel (+0x10)
 * finishes the request instead. The file stays open.
 *
 * @param opHandle  the read operation's handle
 * @param opStatus  its status
 * @param request  the read request
 */
void readfilereadcallback(int opHandle, int opStatus, struct REQUESTSTRUCTtag *request) {
    int n = FILESYS_completeop_F3C0(request->fop);
    int x;
    int chunkSize;
    request->bytesread += n;
    (*(int*)&request->destaddr) += n;
    if (n <= 0x7FFF || request->cancelflag != 0) {
        finishrequest((REQUESTSTRUCTtag*)request);
        return;
    }
    x = request->readsize - n;
    chunkSize = x > 0x8000 ? 0x8000 : x;
    request->foffset += n;
    request->readsize = x;
    request->fop = FILESYS_read(request->fhandle, request->foffset, ((int)request->destaddr), chunkSize, 99, request);
    if (request->fop != 0) {
        FILESYS_callbackop(request->fop, readfilereadcallback);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
/* hlafile.cpp's REQUESTSTRUCTtag (0x30, ealib/hlafile.h): [O] member names at these offsets. */
typedef struct E_F488 { int id; struct E_F488 *next; int pad[5]; volatile int fop; int pad2[4]; } E_F488;
extern E_F488 *hlafile_request_F488 __asm__("hlafile_request");
extern int numrequests_F488 __asm__("numrequests");
extern E_F488 *freequeue_F488[2] __asm__("freequeue");
extern char hlafile_mutex_F488[] __asm__("hlafile_mutex");
extern char str_ASYNCFILE_F488[] __asm__("str_ASYNCFILE");
extern void MUTEX_create_F488(void *) __asm__("MUTEX_create");

/**
 * Sets up the async-file system once: a pool of @p n requests ("ASYNCFILE") on the free queue and its mutex.
 *
 * Does nothing when already set up or when @p n is above 256.
 *
 * @param n  number of requests (On Tour requests)
 * @param x  memory type for the pool (On Tour memtype)
 */
void ASYNCFILE_init(int n, int x) {
    E_F488 *reqs;
    int i;
    if (hlafile_request_F488 == 0 && n < 257) {
        numrequests_F488 = n;
        reqs = ((E_F488*(*)(char*, int, int))gFileSysOpts.malloc)(str_ASYNCFILE_F488, n * 0x30, x);
        freequeue_F488[1] = &reqs[n - 1];
        freequeue_F488[0] = reqs;
        hlafile_request_F488 = reqs;
        MUTEX_create_F488(hlafile_mutex_F488);
        for (i = 0; i < n; i++) {
            hlafile_request_F488[i].id = i;
            hlafile_request_F488[i].next = &hlafile_request_F488[i + 1];
            hlafile_request_F488[i].fop = 0;
        }
        hlafile_request_F488[n - 1].next = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int numrequests;
extern int ASYNCFILE_cancel(int);
extern int THREAD_iscurrent(THREAD *);
extern void THREAD_yield(int);
extern int SYNCTASK_run(int);
extern void MUTEX_destroy_F570(char *) __asm__("MUTEX_destroy");

/**
 * Shuts the async-file layer down: cancels all requests, waits for them, frees the request table.
 *
 * Does nothing when hlafile_request is null. Otherwise cancels each of numrequests requests, pumps SYNCTASK_run
 * until none is busy, frees the table with gFileSysOpts' free function and destroys hlafile_mutex.
 */
void ASYNCFILE_restore(void) {
    if (hlafile_request != 0) {
        int i;
        for (i = 0; i < numrequests; i++) {
            ASYNCFILE_cancel(hlafile_request[i].id);
        }
        for (;;) {
            int busy = 0;
            for (i = 0; i < numrequests; i++) {
                if (hlafile_request[i].fop != 0) busy = 1;
            }
            if (!busy) break;
            if (THREAD_iscurrent(0) != 0) SYNCTASK_run(0);
            THREAD_yield(0);
        }
        gFileSysOpts.free(hlafile_request);
        MUTEX_destroy_F570(hlafile_mutex);
        hlafile_request = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char freequeue_F690[] __asm__("freequeue");
extern int *f_EED8_F690(char *) __asm__("queuefetch");
extern void f_EF40_F690(void *) __asm__("newrequestid");
extern int f_D4E8_F690(int, int, int, void *) __asm__("FILESYS_open");
extern void f_CF70_F690(int, void *) __asm__("FILESYS_callbackop");
extern char f_F2C0_F690[] __asm__("loadfileopencallback");

/**
 * Starts loading a whole file into newly allocated memory; returns the request id (0 on failure).
 *
 * Takes a request from freequeue, gives it an id, records the memory type, opens the file with FILESYS_open and
 * chains the size/read/close steps from the callback loadfileopencallback. A failed open puts the request back.
 *
 * @param name  the file name
 * @param memtype  allocation flags for the buffer (On Tour memtype)
 * @return the request id, or 0
 */
int ASYNCFILE_load(int name, int memtype) {
    struct REQUESTSTRUCTtag *req = (struct REQUESTSTRUCTtag *)f_EED8_F690(freequeue_F690);
    if (req != 0) {
        f_EF40_F690(req);
        req->readsize = memtype;
        req->bytesread = 0;
        req->releaseflag = 0;
        req->cancelflag = 0;
        req->callback = 0;
        req->foffset = 0;
        req->destaddr = 0;
        (*(int*)&req->loadaddr) = 1;
        req->fop = f_D4E8_F690(name, 1, 100, req);
        if (req->fop != 0) {
            f_CF70_F690(req->fop, f_F2C0_F690);
            return req->id;
        }
        return 0;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_loadat);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void loadfileopencallback_F748(void) __asm__("loadfileopencallback");
extern int freequeue_F748[] __asm__("freequeue");

/**
 * Starts loading a whole file asynchronously into a caller buffer; returns the request id, or 0 when no request is free
 * or the open could not be queued.
 *
 * Takes a free request (queuefetch, newrequestid), clears its state, sets the destination and size limit and opens the
 * file (priority 100) with callback loadfileopencallback.
 *
 * @param fileName  the path
 * @param address  the destination
 * @param maxSize  the destination size
 * @return the request id, or 0
 */
int ASYNCFILE_loadat(int fileName, int address, int maxSize) {
    struct REQUESTSTRUCTtag *request = (struct REQUESTSTRUCTtag *)queuefetch((void **)freequeue_F748);
    if (request == 0) return 0;
    newrequestid(request);
    request->readsize = maxSize;
    (*(int*)&request->destaddr) = address;
    request->bytesread = 0;
    request->releaseflag = 0;
    request->cancelflag = 0;
    request->loadaddr = 0;
    request->callback = 0;
    request->foffset = 0;
    request->fop = FILESYS_open(fileName, 1, 100, request);
    if (request->fop == 0) return 0;
    FILESYS_callbackop(request->fop, loadfileopencallback_F748);
    return request->id;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_read);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void fCF70_F808(int, char *) __asm__("FILESYS_callbackop");
extern int fD648_F808(int, int, int, int, int, void *) __asm__("FILESYS_read");
extern char *fEED8_F808(char *) __asm__("queuefetch");
extern void fEF40_F808(char *) __asm__("newrequestid");
extern char D_9BF8_F808[] __asm__("freequeue");
extern char fF3C0_F808[] __asm__("readfilereadcallback");

/**
 * Starts an asynchronous read from an open file into a buffer; returns the request id, or 0 when no request is free or
 * the read could not be queued. Reads in chunks of at most 0x8000 bytes through readfilereadcallback.
 *
 * @param file  the file handle
 * @param offset  the file offset
 * @param buffer  the destination
 * @param size  the byte count
 * @return the request id, or 0
 */
int ASYNCFILE_read(int file, int offset, int buffer, int size) {
    REQUESTSTRUCTtag* job = (REQUESTSTRUCTtag*)fEED8_F808(D_9BF8_F808);
    if (((char*)job) != 0) {
        fEF40_F808(((char*)job));
        job->bytesread = 0;
        job->releaseflag = 0;
        job->cancelflag = 0;
        job->loadaddr = 0;
        job->callback = 0;
        job->fhandle = file;
        job->foffset = offset;
        job->readsize = size;
        job->destaddr = (char*)buffer;
        job->fop = fD648_F808(file, offset, buffer, size <= 0x8000 ? size : 0x8000, 100, job);
        if (job->fop != 0) {
            fCF70_F808(job->fop, fF3C0_F808);
            return *(int *)job;
        }
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_setcallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_F8E8(void *) __asm__("MUTEX_unlock");
extern char hlafile_mutex_9C08[] __asm__("hlafile_mutex");
extern char *locaterequest_F8E8(int) __asm__("locaterequest");

void ASYNCFILE_setcallback_impl(int h, void (*fn)(int)) __asm__("ASYNCFILE_setcallback");
/**
 * Sets the done callback (+0x18) of an asynchronous file request; calls it at once when the request has already
 * finished (no operation pending). Does nothing for an unknown request or one that already has a callback.
 *
 * @param reqId  the request id
 * @param fn  the callback, given the request id
 */
void ASYNCFILE_setcallback_impl(int reqId, void (*fn)(int)) {
    REQUESTSTRUCTtag* request = (REQUESTSTRUCTtag*)locaterequest_F8E8(reqId);
    int done;
    if (((char*)request) == 0 || request->callback != 0) {
        return;
    }
    MUTEX_lock(hlafile_mutex_9C08);
    done = request->fop == 0;
    request->callback = fn;
    MUTEX_unlock_F8E8(hlafile_mutex_9C08);
    if (done) {
        fn(reqId);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_getstatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void mlock_F980(char *) __asm__("MUTEX_lock");
extern void munlock_F980(char *) __asm__("MUTEX_unlock");

/**
 * Reports whether an asynchronous file request has finished.
 *
 * Looks the request up under the request-list mutex (locaterequest checks the handle's sequence number).
 *
 * @param handle  request handle
 * @return 1 when the request has no file-system command pending, 0 while one is, -1 for an invalid handle
 */
int ASYNCFILE_getstatus(int handle) {
    struct REQUESTSTRUCTtag *req;
    int status;
    mlock_F980(hlafile_mutex);
    req = locaterequest(handle);
    status = req ? req->fop == 0 : -1;
    munlock_F980(hlafile_mutex);
    return status;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_release);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void MUTEX_unlock_AR(void *) __asm__("MUTEX_unlock");
extern int FILESYS_waitop(int);

int ASYNCFILE_release(int h, int *a, int *b);
/**
 * Waits for an asynchronous file request to finish, reports what it loaded and frees the request.
 *
 * Fails at once for an invalid or already-cancelled request; otherwise sets its release flag and waits on its
 * pending command with FILESYS_waitop(). If it was cancelled meanwhile, both outputs are zeroed. Either way
 * releaserequest() returns it to the free list.
 *
 * @param handle  request handle
 * @param address  if not 0, receives the start of the loaded data
 * @param size  if not 0, receives the number of bytes loaded
 * @return 1 on success, -1 for an invalid or cancelled request
 */
int ASYNCFILE_release(int handle, int *address, int *size) {
    struct REQUESTSTRUCTtag *req;
    int busy = 0;
    MUTEX_lock(hlafile_mutex);
    req = locaterequest(handle);
    if (req != 0) {
        busy = req->cancelflag;
        if (busy == 0) {
            req->releaseflag = 1;
        }
    }
    MUTEX_unlock_AR(hlafile_mutex);
    if (req == 0 || busy != 0) {
        return -1;
    }
    while (req->fop != 0) {
        FILESYS_waitop(req->fop);
    }
    if (req->cancelflag != 0) {
        if (address != 0) *address = 0;
        if (size != 0) *size = 0;
        releaserequest((REQUESTSTRUCTtag*)req);
        return -1;
    }
    if (address != 0) *address = ((int)req->destaddr) - req->bytesread;
    if (size != 0) *size = req->bytesread;
    releaserequest((REQUESTSTRUCTtag*)req);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", ASYNCFILE_cancel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void mlock_FAF0(char *) __asm__("MUTEX_lock");
extern void munlock_FAF0(char *) __asm__("MUTEX_unlock");
extern void fD148_FAF0(int) __asm__("FILESYS_cancelop");
extern char *fEF70_FAF0(int) __asm__("locaterequest");
extern void rel_FAF0(char *) __asm__("releaserequest");
extern char D_9C08_FAF0[] __asm__("hlafile_mutex");

/**
 * Cancels an async file request: stops its in-flight operation, or releases it when it has not started.
 *
 * @param handle  the request handle
 * @return 1 when cancelled; -1 for a bad handle, an already-cancelled request or one that has completed
 */
int ASYNCFILE_cancel(int handle) {
    int releaseFlag, fileOp, cancelFlag, lockState;
    REQUESTSTRUCTtag* req;
    fileOp = 0;
    cancelFlag = 0;
    mlock_FAF0(D_9C08_FAF0);
    releaseFlag = 0;
    req = (REQUESTSTRUCTtag*)fEF70_FAF0(handle);
    if (((char*)req) != 0 && ((fileOp = req->fop, cancelFlag = req->cancelflag, releaseFlag = req->releaseflag, fileOp != 0) || releaseFlag == 0)) {
        req->cancelflag = 1;
    }
    munlock_FAF0(D_9C08_FAF0);
    if (((char*)req) != 0 && cancelFlag == 0) {
        if (req->cancelflag != 0) {
            if (fileOp != 0) {
                fD148_FAF0(fileOp);
                return 1;
            }
            if (releaseFlag == 0) {
                rel_FAF0(((char*)req));
            }
            return 1;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", validatehandle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int validatehandle_impl(int **arg0, int **arg1, int ***arg2) __asm__("validatehandle");
/**
 * Checks a stream tap handle and resolves its stream header.
 *
 * @p handle points at the tap, whose first word points at the stream header; the header must start with 'STRM'.
 *
 * @param handle  the tap handle
 * @param strmptr  receives the stream header
 * @param tapptr  receives the tap
 * @return 0 when valid, 1 otherwise
 */
int validatehandle_impl(int **handle, int **strmptr, int ***tapptr) {
    int *temp_7;

    if (handle == 0) {
        return 1;
    }
    temp_7 = *handle;
    if (*temp_7 != STREAM_HEADER_ID) {
        return 1;
    }
    *tapptr = handle;
    *strmptr = temp_7;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", inbetween);
#ifdef SKIP_ASM
/**
 * Returns whether @p addr lies in the ring-buffer range [@p start, @p end); a range with @p end below @p start wraps.
 *
 * @param start  the range start
 * @param end  the range end
 * @param addr  the address to test
 * @return 1 when inside, else 0
 */
int inbetween(unsigned int start, unsigned int end, unsigned int addr) {
    int inside;
    if (end >= start) {
        inside = 0;
        if (addr >= start) {
            if (addr < end) inside = 1;
        }
        return inside;
    }
    inside = 0;
    if (!(addr < start) || addr < end) {
        inside = 1;
    }
    return inside;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", decbufferusage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void MUTEX_lock(void *);
void MUTEX_unlock_FC48(void *) __asm__("MUTEX_unlock");
void FILESYS_priorityop_FC48(int, int) __asm__("FILESYS_priorityop");

/**
 * Lowers a stream's bufferusage by @p size under its mutex; when that takes it below the greedylevel the greedystate
 * is set and, while the stream is reading (state 1), its read operation (fop) gets the high priority again
 * (FILESYS_priorityop with priorityhigh).
 *
 * @param stream  the stream header
 * @param size  the bytes released
 */
void decbufferusage(STREAMHEADERtag *stream, int size) {
    MUTEX *mutex;
    int old, nw, lim;

    mutex = &stream->mutex;
    MUTEX_lock(mutex);
    old = stream->bufferusage;
    nw = old - size;
    stream->bufferusage = nw;
    MUTEX_unlock_FC48(mutex);
    lim = stream->greedylevel;
    if (old >= lim && nw < lim) {
        stream->greedystate = 1;
        if (stream->state == 1) {
            FILESYS_priorityop_FC48(stream->fop, stream->priorityhigh);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", getfreerequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int stream_requestidcounter_FCD8[] __asm__("stream_requestidcounter");
extern STREAMREQUESTtag *getfreerequest_impl(STREAMHEADERtag *strm) __asm__("getfreerequest");

/**
 * Takes a free request from a stream and gives it a new id; null when none is free.
 *
 * Pops the free list (freereq) under the stream's mutex and ORs a fresh stream_requestidcounter value (step 0x100,
 * never 0) above the request's slot byte.
 *
 * @param strm  the stream header
 * @return the request, or null
 */
STREAMREQUESTtag *getfreerequest_impl(STREAMHEADERtag *strm) {
    STREAMREQUESTtag *req;
    int nextId;
    MUTEX_lock(&strm->mutex);
    req = 0;
    if (strm->freereq != 0) {
        req = strm->freereq;
        nextId = stream_requestidcounter_FCD8[0] + 0x100;
        strm->freereq = req->next;
        stream_requestidcounter_FCD8[0] = nextId;
        if (nextId == 0) {
            stream_requestidcounter_FCD8[0] = 0x100;
        }
        req->id = (unsigned char)req->id | stream_requestidcounter_FCD8[0];
    }
    MUTEX_unlock(&strm->mutex);
    return req;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", queuerequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_lock_FD58(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_FD58(void *) __asm__("MUTEX_unlock");

/**
 * Appends a request to a stream's queue.
 *
 * Marks the request pending (state 1, no next link) and links it after the tail (lastreq), or makes it firstreq,
 * curreq and lastreq of an empty queue, under the stream's mutex.
 *
 * @param strm  the stream header
 * @param req  the request
 */
void queuerequest(STREAMHEADERtag *strm, STREAMREQUESTtag *req) {
    STREAMREQUESTtag *last;

    req->next = 0;
    req->state = 1;
    MUTEX_lock_FD58(&strm->mutex);
    last = strm->lastreq;
    if (last == 0) {
        req->prev = 0;
        strm->firstreq = req;
        strm->curreq = req;
        strm->lastreq = req;
    } else {
        req->prev = last;
        strm->lastreq->next = req;
        strm->lastreq = req;
    }
    MUTEX_unlock_FD58(&strm->mutex);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", locaterequest1);
#ifdef SKIP_ASM
/**
 * Returns a stream's request with this id (slot = low byte, into the request array of `requests` entries) when the id
 * matches and the request is in use (state not 0); otherwise 0.
 *
 * @param stream  the stream header
 * @param requestId  the request id
 * @return the request, or 0
 */
STREAMREQUESTtag *locaterequest1(STREAMHEADERtag *stream, int requestId) {
    int slot;
    STREAMREQUESTtag *req;

    slot = requestId & 0xFF;
    if (slot >= stream->requests) {
        return 0;
    }
    req = &stream->request[slot];
    if (requestId == req->id) {
        return (req->state == 0) ? 0 : req;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", freerequest);
#ifdef SKIP_ASM
/**
 * Unlinks a request from a stream's queue (firstreq, lastreq; curreq moves to a neighbour) and pushes it on the free
 * list (freereq), marking it unused.
 *
 * @param stream  the stream header
 * @param n  the request
 */
void freerequest(STREAMHEADERtag *stream, STREAMREQUESTtag *n) {
    if (n == stream->firstreq) {
        stream->firstreq = n->next;
    } else {
        n->prev->next = n->next;
    }
    if (n == stream->lastreq) {
        stream->lastreq = n->prev;
    } else {
        n->next->prev = n->prev;
    }
    if (n == stream->curreq) {
        STREAMREQUESTtag *neighbor = n->next;
        if (neighbor == 0) neighbor = n->prev;
        stream->curreq = neighbor;
    }
    n->state = 0;
    n->next = stream->freereq;
    stream->freereq = n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", filterchunk);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_FE88 { int mask; int value; int result; };
struct T_FE88 { char pad[0x1C]; struct E_FE88 *ents; int count; };

/**
 * Returns the tap for a stream chunk: the first filter (+0x1C, count +0x20) whose mask/value matches the chunk's type,
 * or -2.
 *
 * @param table  the stream header
 * @param key  the chunk header (type first)
 * @return the tap number, or -2
 */
int filterchunk(struct T_FE88 *table, int *key) {
    int i;
    int k = *key;
    for (i = 0; i < table->count; i++) {
        if ((k & table->ents[i].mask) == table->ents[i].value) {
            return table->ents[i].result;
        }
    }
    return -2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", parsechunks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int filterchunk_FED0(STREAMHEADERtag *, STREAMCHUNKHDR *) __asm__("filterchunk");
extern void MUTEX_unlock_FED0(void *) __asm__("MUTEX_unlock");

/* pads n so that p + n ends on a 64-byte boundary */
#define ALIGN64_FED0(n, p) ((((n) + ((int)(p) & 0x3F) + 0x3F) & ~0x3F) - ((int)(p) & 0x3F))

/**
 * Hands the newly read chunks of a stream to its taps; returns 1 when the current request's end chunk was reached.
 *
 * Walks the chunk headers from datatail while a whole header and chunk are in (up to dataend). A size with a top byte
 * set marks a padding chunk. The end chunk (type = the current request's endchunkid) is padded to 64 bytes. Each chunk
 * is tagged with its tap (filterchunk) and added to the tap's gettable, the stream's bufferusage goes up (clearing the
 * greedystate at the greedylevel) and datatail moves on; a chunk with no tap is marked -2. A cancelled request (state
 * 4) stops the walk.
 *
 * @return 1 at the end chunk, else 0
 */
int parsechunks(STREAMHEADERtag *self) {
    STREAMREQUESTtag *current;
    STREAMCHUNKHDR *chunk;
    MUTEX *mutex;
    int len;
    int tapnum;
    int busy;
    int old;
    int nw;
    int lim;
    TAPSTRUCTtag *tap;

    current = self->curreq;
    while (self->dataend - self->datatail >= 8) {
        chunk = (STREAMCHUNKHDR *)self->datatail;
        len = chunk->size;
        if (len & 0xFF000000) {
            chunk->type = current->endchunkid;
            len = 8;
            chunk->size = 8;
        }
        if (self->dataend < self->datatail + len) {
            break;
        }
        if (chunk->type == current->endchunkid) {
            len = ALIGN64_FED0(len, self->datatail);
        }
        tapnum = filterchunk_FED0(self, chunk);
        if (tapnum < 0) {
            mutex = &self->mutex;
            MUTEX_lock(mutex);
            busy = current->state == 4;
            if (!busy) {
                chunk->type = -2;
                self->datatail += len;
            }
            MUTEX_unlock_FED0(mutex);
        } else {
            chunk->size = len | (tapnum << 24);
            MUTEX_lock(&self->mutex);
            busy = current->state == 4;
            if (!busy) {
                tap = &self->tap[tapnum - 1];
                tap->gettable += len;
                if (tap->gettable == len) {
                    tap->getptr = (char *)chunk;
                }
                old = self->bufferusage;
                nw = old + len;
                self->bufferusage = nw;
                self->datatail += len;
                lim = self->greedylevel;
                if (old < lim && nw >= lim) {
                    self->greedystate = 0;
                }
            }
            MUTEX_unlock_FED0(&self->mutex);
        }
        if (busy) {
            if (chunk->type != current->endchunkid) {
                len = ALIGN64_FED0(len, self->datatail);
                chunk->size = len | (tapnum << 24);
            }
            return 0;
        }
        if (chunk->type == current->endchunkid) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", opencallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int FILESYS_completeop_00D8(int) __asm__("FILESYS_completeop");
void restartstream_00D8(void *, int) __asm__("restartstream");

/**
 * Open callback of a stream: completes the open, stores the file handle (fhandle) and, when it opened, restarts the
 * stream (restartstream with priorityhigh).
 *
 * @param request  the open operation's handle
 * @param status  its status
 * @param stream  the stream header
 */
void opencallback(int request, int status, STREAMHEADERtag *stream) {
    int fileHandle;

    fileHandle = FILESYS_completeop_00D8(stream->fop);
    stream->fhandle = fileHandle;
    if (fileHandle != 0) {
        restartstream_00D8(stream, stream->priorityhigh);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", closecallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_callbackop_0118(int, void *) __asm__("FILESYS_callbackop");
void FILESYS_completeop_0118(int) __asm__("FILESYS_completeop");
int FILESYS_open_0118(void *, int, int, void *) __asm__("FILESYS_open");

/**
 * Close callback of a stream: completes the close and opens the next file (fname, priorityhigh) with callback
 * opencallback.
 *
 * @param request  the close operation's handle
 * @param status  its status
 * @param stream  the stream header
 */
void closecallback(int request, int status, STREAMHEADERtag *stream) {
    int opHandle;

    FILESYS_completeop_0118(stream->fop);
    opHandle = FILESYS_open_0118(stream->fname, 1, stream->priorityhigh, stream);
    stream->fop = opHandle;
    if (opHandle != 0) {
        FILESYS_callbackop_0118(opHandle, (void *)opencallback);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", readcallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void MUTEX_lock_0170(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_0170(void *) __asm__("MUTEX_unlock");
extern int fD310_0170(int) __asm__("FILESYS_completeop");
extern int fFED0_0170(STREAMHEADERtag *) __asm__("parsechunks");
extern void f0270_0170(STREAMHEADERtag *, int) __asm__("startnextrequest");
extern void f03D0_0170(STREAMHEADERtag *, int) __asm__("restartstream");

/**
 * Read callback of a stream: accounts the bytes read, hands complete chunks to the taps (parsechunks) and starts the
 * next read or the next request.
 *
 * A memory request (type 1) counts its whole readsize; a file request counts what the read returned
 * (FILESYS_completeop). The request is done at its end (parm, the memory length), on a short read, at its end chunk,
 * or when cancelled (state 4); then it is marked done (3, unless cancelled) and startnextrequest runs. Otherwise the
 * stream restarts at one below priorityhigh (restartstream).
 *
 * @param request  the read operation's handle
 * @param status  its status
 * @param stream  the stream header
 */
void readcallback(int request, int status, STREAMHEADERtag *stream) {
    STREAMREQUESTtag *current = stream->curreq;
    int n;
    int flag;
    int finished;
    if (current->type == 1) {
        n = stream->readsize;
        flag = stream->foffset + n >= current->parm;
    } else {
        n = fD310_0170(stream->fop);
        flag = n < stream->readsize;
    }
    stream->foffset += n;
    stream->dataend += n;
    finished = fFED0_0170(stream);
    if (current->state == 4 || flag || finished != 0) {
        if (current->state != 4) {
            MUTEX_lock_0170(&stream->mutex);
            if (current->state != 4) current->state = 3;
            MUTEX_unlock_0170(&stream->mutex);
        }
        f0270_0170(stream, stream->priorityhigh);
    } else {
        f03D0_0170(stream, stream->priorityhigh - 1);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", startnextrequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void MUTEX_unlock_0270(void *) __asm__("MUTEX_unlock");
extern int strcmp_0270(char *, char *) __asm__("strcmp");
extern char *strcpy_0270(char *, char *) __asm__("strcpy");
extern int FILESYS_open_0270(char *, int, int, void *) __asm__("FILESYS_open");
extern void cb_00D8_0270(void) __asm__("opencallback");
extern void cb_0118_0270(void) __asm__("closecallback");
extern void restartstream_0270(STREAMHEADERtag *, int) __asm__("restartstream");

/**
 * Moves a stream on to its next queued file request.
 *
 * Picks the next request under the stream mutex; if it names the file already open, reading restarts with
 * restartstream, otherwise the current file is closed (FILESYS_close) and the new one opened (FILESYS_open)
 * with a completion callback.
 *
 * @param priority  queue priority for the file operations
 */
void startnextrequest(STREAMHEADERtag *self, int priority) {
    STREAMREQUESTtag *job;
    int none;
    char *tail;
    job = 0;
    MUTEX_lock(&self->mutex);
    if (self->curreq == 0) {
        none = 1;
    } else {
        STREAMREQUESTtag *cur = self->curreq;
        none = 0;
        if (cur->state != 1) {
            if (cur->next == 0) {
                none = 1;
            } else {
                self->curreq = cur->next;
            }
        }
    }
    if (none) {
        self->state = 0;
    } else {
        job = self->curreq;
        tail = self->datatail;
        job->state = 2;
        job->datastart = tail;
    }
    MUTEX_unlock_0270(&self->mutex);
    if (!none) {
        self->dataend = self->datatail;
        if (job->type == 1) {
            self->foffset = 0;
        } else {
            char *nm = self->fname;
            int opHandle;
            self->foffset = job->parm;
            if (strcmp_0270(job->fname, nm) != 0) {
                strcpy_0270(nm, job->fname);
                if (self->fhandle == 0) {
                    opHandle = FILESYS_open_0270(nm, 1, priority, self);
                    self->fop = opHandle;
                    if (opHandle != 0) {
                        FILESYS_callbackop(opHandle, cb_00D8_0270);
                    }
                } else {
                    opHandle = FILESYS_close(self->fhandle, priority, self);
                    self->fop = opHandle;
                    if (opHandle != 0) {
                        FILESYS_callbackop(opHandle, cb_0118_0270);
                    }
                }
                return;
            }
        }
        restartstream_0270(self, priority);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", restartstream);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void MUTEX_lock(void *);
extern void MUTEX_unlock_03D0(void *) __asm__("MUTEX_unlock");
extern int inbetween(unsigned int, unsigned int, unsigned int);
extern void freerequest_03D0(STREAMHEADERtag *, STREAMREQUESTtag *) __asm__("freerequest");
extern void MEM_copy_03D0(char *, char *, int) __asm__("MEM_copy");
extern void readcallback_03D0(int, int, STREAMHEADERtag *) __asm__("readcallback");
extern int FILESYS_read_03D0(int, int, char *, int, int, STREAMHEADERtag *) __asm__("FILESYS_read");
extern void FILESYS_callbackop_03D0(int, void (*)(int, int, STREAMHEADERtag *)) __asm__("FILESYS_callbackop");

/**
 * Restarts a stream's reading: drops consumed chunks, frees finished requests, makes room in the buffer and starts the
 * next read of the current request.
 *
 * Skips the wrap (-1) and released (-2) chunks at datastart. Under the mutex, frees (freerequest) the first requests
 * whose data has been consumed. With less than a read block (readblocksize) free at the end, the unread data is moved
 * to the buffer start (MEM_copy, 64-byte aligned) and a wrap marker left; when even that leaves no room the state
 * becomes 2 (stalled). A memory request (type 1) is copied in and its read callback (readcallback) called directly; a
 * file request reads a block at @p priority priority with that callback.
 *
 * @param strm  the stream header
 * @param priority  the read priority
 */
void restartstream(STREAMHEADERtag *strm, int priority) {
    STREAMCHUNKHDR *chunk;
    char *rd;
    char *wr;
    int avail;
    STREAMREQUESTtag *cur;

    while (strm->datastart != strm->datatail) {
        chunk = (STREAMCHUNKHDR *)strm->datastart;
        if (chunk->type == -1) {
            strm->datastart = strm->bufferstart;
        } else if (chunk->type == -2) {
            strm->datastart += chunk->size;
        } else {
            break;
        }
    }
    MUTEX_lock(&strm->mutex);
    for (;;) {
        STREAMREQUESTtag *first = strm->firstreq;
        STREAMREQUESTtag *nextReq = first->next;
        if (nextReq == 0 || nextReq->state == 1
            || inbetween((unsigned int)strm->datastart, (unsigned int)strm->dataend,
                         (unsigned int)(nextReq->datastart - 1))) {
            break;
        }
        freerequest_03D0(strm, first);
    }
    MUTEX_unlock_03D0(&strm->mutex);
    rd = strm->datastart;
    wr = strm->dataend;
    if (wr < rd) {
        avail = rd - wr - 65;
    } else {
        int used;
        avail = strm->bufferend - wr - 0x40;
        if (avail >= strm->readblocksize) {
            goto fill;
        }
        used = wr - strm->datatail;
        if (strm->curreq->type == 1) {
            if (rd - strm->bufferstart < used + 1) {
                strm->state = 2;
                return;
            }
        } else if (rd - strm->bufferstart - 0x40 < used + 1) {
            strm->state = 2;
            return;
        }
        if ((used & 0x3F) == 0 || strm->curreq->type == 1) {
            strm->bufferstart = strm->actualbufferstart;
        } else {
            strm->bufferstart = strm->actualbufferstart - (used % 0x40 - 0x40);
        }
        MEM_copy_03D0(strm->bufferstart, strm->datatail, used);
        chunk = (STREAMCHUNKHDR *)strm->datatail;
        chunk->type = -1;
        chunk->size = 8;
        strm->datatail = strm->bufferstart;
        strm->dataend = strm->bufferstart + used;
        if (((STREAMCHUNKHDR *)strm->datastart)->type == -1) {
            strm->datastart = strm->bufferstart;
            avail = strm->bufferend - strm->dataend - 0x40;
        } else {
            avail = strm->datastart - strm->dataend - 1;
        }
    }
    if (avail < strm->readblocksize) {
        strm->state = 2;
        return;
    }
fill:
    cur = strm->curreq;
    if (cur->type == 1) {
        if (cur->parm < strm->foffset + avail) {
            strm->readsize = cur->parm - strm->foffset;
        } else {
            strm->readsize = avail;
        }
        MEM_copy_03D0(strm->dataend, cur->address, strm->readsize);
        cur->address += strm->readsize;
        readcallback_03D0(0, 0, strm);
    } else {
        strm->readsize = strm->readblocksize;
        strm->fop = FILESYS_read_03D0(strm->fhandle, strm->foffset, strm->dataend, strm->readblocksize, priority, strm);
        if (strm->fop != 0) {
            FILESYS_callbackop_03D0(strm->fop, readcallback_03D0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_overhead);
#ifdef SKIP_ASM
/**
 * Returns the bookkeeping memory a stream needs for its request, filter and tap counts.
 *
 * @p requests * 0x124 + @p filters * 0xC + @p taps * 0x10 + 0x1C0 bytes.
 *
 * @param requests  the number of requests
 * @param filters  the number of filters
 * @param taps  the number of taps
 * @return the size in bytes
 */
int STREAM_overhead(int requests, int filters, int taps) {
    int subtotal = filters * 0xC + 0x180;
    return requests * 292 + subtotal + taps * 0x10 + 0x40;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_create);
#ifdef SKIP_ASM
extern void MUTEX_create_06D8(void *) __asm__("MUTEX_create");
extern void MEM_clear_06D8(void *, int) __asm__("MEM_clear");

/**
 * Creates a stream in a caller-supplied buffer; returns its handle (the first tap; 0 on failure).
 *
 * The space left after STREAM_overhead's bookkeeping must hold at least 0x1800 bytes of data, and the slot count
 * must be 2..0x100. Lays out the header ('STRM'), request slots, filter and tap tables, creates the mutex, clears the
 * file name with MEM_clear and chains the requests into the free list.
 *
 * @param nslots  the number of requests
 * @param nq      the number of filters
 * @param nreq    the number of taps
 * @param mem     the buffer
 * @param size    the buffer size
 * @return the stream handle, or 0
 */
void *STREAM_create(int nslots, int nq, int nreq, char *mem, int size) {
    int avail;
    STREAMHEADERtag *strm;
    STREAMREQUESTtag *slots;
    FILTERSTRUCTtag *qs;
    TAPSTRUCTtag *reqs;
    char *data;
    int i;

    avail = size - STREAM_overhead(nslots, nq, nreq);
    if (avail < 0x1800) {
        return 0;
    }
    if (nslots < 2) {
        return 0;
    }
    if (nslots > 0x100) {
        return 0;
    }
    if (nq < 1 || nq > 0x10) {
        return 0;
    }
    if (nreq <= 0 || nq < nreq) {
        return 0;
    }
    strm = (STREAMHEADERtag *)mem;
    strm->id = STREAM_HEADER_ID;
    MUTEX_create_06D8(&strm->mutex);
    /* PORT: pointer arithmetic done in int */
    // ORIGINAL-PTR: carves one caller-supplied block (allocator-style): the request / filter / tap arrays follow the
    // 0x180-byte STREAMHEADERtag and the data area starts at the next 64-byte boundary (STREAM_overhead's sizes).
    slots = (STREAMREQUESTtag *)(mem + 0x180);
    strm->request = slots;
    strm->requests = nslots;
    qs = (FILTERSTRUCTtag *)&slots[nslots];
    strm->filter = qs;
    strm->filters = nq;
    reqs = (TAPSTRUCTtag *)&qs[nq];
    strm->tap = reqs;
    strm->taps = nreq;
    data = (char *)(((int)&reqs[nreq] & ~0x3F) + 0x40);
    strm->actualbufferstart = data;
    strm->bufferstart = data;
    strm->bufferend = mem + size;
    strm->state = 0;
    strm->prioritylow = 150;
    strm->priorityhigh = 50;
    strm->greedylevel = 0;
    strm->greedystate = 0;
    strm->bufferusage = 0;
    strm->datastart = data;
    strm->datatail = data;
    strm->dataend = data;
    strm->firstreq = 0;
    strm->curreq = 0;
    strm->lastreq = 0;
    strm->freereq = slots;
    MEM_clear_06D8(strm->fname, 0x100);
    strm->fhandle = 0;
    if (avail < 0x4000) {
        strm->readblocksize = 0x800;
    } else if (avail < 0x8000) {
        strm->readblocksize = 0x1000;
    } else if (avail < 0x10000) {
        strm->readblocksize = 0x2000;
    } else if (avail < 0x20000) {
        strm->readblocksize = 0x4000;
    } else {
        strm->readblocksize = 0x8000;
    }
    for (i = 0; i < nslots; i++) {
        STREAMREQUESTtag *entry;

        entry = &strm->request[i];
        entry->id = i;
        entry->state = 0;
        entry->next = &strm->request[i + 1];
    }
    strm->request[nslots - 1].next = 0;
    for (i = 0; i < nq; i++) {
        FILTERSTRUCTtag *entry;

        entry = &strm->filter[i];
        entry->mask = 0;
        entry->value = 0;
        entry->tapnum = 1;
    }
    for (i = 0; i < nreq; i++) {
        TAPSTRUCTtag *entry;

        entry = &strm->tap[i];
        entry->stream = strm;
        entry->tapnum = i + 1;
        entry->gettable = 0;
    }
    return strm->tap;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_setfilter);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int fFBD0_0948(int, void *, void *) __asm__("validatehandle");

/**
 * Sets filter @p channel (1-based) of an idle stream: chunks whose type & @p typeMask equals @p typeValue go to tap @p tapNum (-1 and -2
 * are special targets). The last filter may only be the catch-all (mask and value 0). Ignored unless the stream is idle
 * (state 0) and the tap is valid.
 *
 * @param handle  the stream handle
 * @param channel  the filter number
 * @param typeMask  the type mask
 * @param typeValue  the type value
 * @param tapNum  the tap number
 */
void STREAM_setfilter(int handle, int channel, int typeMask, int typeValue, int tapNum) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    if (fFBD0_0948(handle, &stream, &tap) == 0 && channel > 0) {
        int count = stream->filters;
        if (count >= channel && (channel != count || (typeMask | typeValue) == 0)
            && (tapNum > 0 || tapNum == -1 || tapNum == -2) && stream->taps >= tapNum && stream->state == 0) {
            FILTERSTRUCTtag *filter = &stream->filter[channel - 1];
            filter->mask = typeMask;
            filter->value = typeValue;
            filter->tapnum = tapNum;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_FBD0_0A28(int, void *, void *) __asm__("validatehandle");
extern void f_1110_0A28(int) __asm__("STREAM_kill");
extern int f_5440_0A28(int) __asm__("THREAD_iscurrent");
extern void f_SYNC_0A28(int) __asm__("SYNCTASK_run");
extern void f_5398_0A28(int) __asm__("THREAD_yield");
extern void f_56E0_0A28(void *) __asm__("MUTEX_destroy");
extern void f_ECA0_0A28(int, int) __asm__("FILESYS_closesync");

/**
 * Destroys a stream: stops it, waits for its I/O, closes its file.
 *
 * For a valid handle (validatehandle), kills the stream with STREAM_kill, pumps SYNCTASK_run while a read is
 * pending (state 1), clears the header's id, destroys the mutex and closes the open file (fhandle) with
 * FILESYS_closesync at priority 0x64.
 *
 * @param handle  the stream handle
 */
void STREAM_destroy(int handle) {
    STREAMHEADERtag *strm;
    TAPSTRUCTtag *tap;
    if (f_FBD0_0A28(handle, &strm, &tap) == 0) {
        f_1110_0A28(handle);
        while (strm->state == 1) {
            if (f_5440_0A28(0) != 0)
                f_SYNC_0A28(0);
            f_5398_0A28(0);
        }
        strm->id = 0;
        f_56E0_0A28(&strm->mutex);
        if (strm->fhandle != 0)
            f_ECA0_0A28(strm->fhandle, 100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_setpriority);
#ifdef SKIP_ASM
/**
 * Sets a stream's two read priorities: low (+0x3C) and high (+0x40).
 *
 * @param handle  the stream handle
 * @param priority  the low priority
 * @param lowBufPriority  the high priority (used while the buffer is below the greedy level)
 */
void STREAM_setpriority(int handle, int priority, int lowBufPriority) {
    int *stream;
    int *entry;

    if (validatehandle(handle, &stream, &entry) == 0) {
        stream[0x3C / 4] = priority;
        stream[0x40 / 4] = lowBufPriority;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_setgreedylevel);
#ifdef SKIP_ASM
extern int validatehandle(int, void *, void *);
extern void STREAM_setgreedystate(int, int);

/**
 * Sets a stream's greedylevel; when the bufferusage moves across it, the greedy state follows
 * (STREAM_setgreedystate).
 *
 * @param handle  the stream handle
 * @param threshold  the new greedy level in bytes
 */
void STREAM_setgreedylevel(int handle, int threshold) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    if (validatehandle(handle, &stream, &tap) == 0) {
        int old = stream->greedylevel;
        stream->greedylevel = threshold;
        {
            int lim = stream->bufferusage;
            if ((lim < old) != (lim < threshold)) {
                STREAM_setgreedystate(handle, lim < threshold);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_setgreedystate);
#ifdef SKIP_ASM
extern void FILESYS_priorityop(int, int);

/**
 * Sets a stream's greedy state; turning it on while the stream reads (state 1) raises its pending read (fop) to the
 * high priority (FILESYS_priorityop with priorityhigh).
 *
 * @param streamHandle  the stream handle
 * @param greedyState  the greedy state
 */
void STREAM_setgreedystate(int streamHandle, int greedyState) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(streamHandle, &stream, &tap) == 0) {
        stream->greedystate = greedyState;
        if (greedyState != 0) {
            if (stream->state == 1) {
                FILESYS_priorityop(stream->fop, stream->priorityhigh);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_taphandle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int validatehandle_0BF8(int, void *, void *) __asm__("validatehandle");

/**
 * Returns the handle of a stream's tap @p k (1-based; its entry in the tap array), or 0 for a bad handle or tap.
 *
 * @param handle  the stream handle
 * @param k  the tap number
 * @return the tap handle, or 0
 */
TAPSTRUCTtag *STREAM_taphandle(int handle, int k) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle_0BF8(handle, &stream, &tap) != 0) {
        return 0;
    }
    if (k <= 0 || stream->taps < k) {
        return 0;
    }
    return &stream->tap[k - 1];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_tapnum);
#ifdef SKIP_ASM
extern int validatehandle(int, void *, void *);

/**
 * Returns the tap number (tapnum) of a tap handle, or 0 for a bad handle.
 *
 * @param handle  the tap handle
 * @return the tap number, or 0
 */
int STREAM_tapnum(int handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    return tap->tapnum;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_queuefile);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int validatehandle_0C88(int, void *, void *) __asm__("validatehandle");
extern STREAMREQUESTtag *getfreerequest_0C88(STREAMHEADERtag *) __asm__("getfreerequest");
extern void queuerequest_0C88(STREAMHEADERtag *, STREAMREQUESTtag *) __asm__("queuerequest");
extern void startnextrequest_0C88(STREAMHEADERtag *, int) __asm__("startnextrequest");
extern void MUTEX_lock_0C88(MUTEX *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_0C88(MUTEX *) __asm__("MUTEX_unlock");

/**
 * Queues a file (from an offset up to an end chunk id) for streaming; returns the request id (0 on failure).
 *
 * Validates the handle, takes a free request, records the name, offset and end id, appends it to the queue and,
 * when the stream is idle, starts it with startnextrequest.
 *
 * @param handle    the stream handle
 * @param name  the file name
 * @param offset  the offset to start at
 * @param endchunkid  the chunk id to stop at
 * @return the request id, or 0
 */
int STREAM_queuefile(int handle, char *name, int offset, int endchunkid) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    STREAMREQUESTtag *node;
    int old;
    if (validatehandle_0C88(handle, &stream, &tap) != 0) {
        return 0;
    }
    node = getfreerequest_0C88(stream);
    if (node == 0) {
        return 0;
    }
    node->type = 0;
    strncpy(node->fname, name, 0xFF);
    node->parm = offset;
    node->endchunkid = endchunkid;
    queuerequest_0C88(stream, node);
    MUTEX_lock_0C88(&stream->mutex);
    old = stream->state;
    if (old == 0) {
        stream->state = 1;
    }
    MUTEX_unlock_0C88(&stream->mutex);
    if (old == 0) {
        if (stream->greedystate != 0) {
            startnextrequest_0C88(stream, stream->priorityhigh);
        } else {
            startnextrequest_0C88(stream, stream->prioritylow);
        }
    }
    return node->id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_queuemem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int validatehandle_0D80(int, STREAMHEADERtag **, TAPSTRUCTtag **) __asm__("validatehandle");
extern STREAMREQUESTtag *getfreerequest_0D80(STREAMHEADERtag *) __asm__("getfreerequest");
extern void queuerequest_0D80(STREAMHEADERtag *, STREAMREQUESTtag *) __asm__("queuerequest");
extern void MUTEX_lock_0D80(MUTEX *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_0D80(MUTEX *) __asm__("MUTEX_unlock");
extern void startnextrequest_0D80(STREAMHEADERtag *, int) __asm__("startnextrequest");

/**
 * Queues a memory block on a stream; returns the request id, or 0 for a bad handle or no free request.
 *
 * With @p n 0 the length runs up to and including the chunk whose type is @p key. The request (memory type 1) keeps
 * the data (address), length (parm) and end chunk type (endchunkid); an idle stream starts reading (startnextrequest).
 *
 * @param handle  the stream handle
 * @param data  the chunk data
 * @param n  the length in bytes, or 0 to find it from @p key
 * @param key  the end chunk type
 * @return the request id, or 0
 */
int STREAM_queuemem(int handle, char *data, int n, int key) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    STREAMREQUESTtag *request;
    STREAMCHUNKHDR *chunk;
    int chunkLen, prevState;
    if (validatehandle_0D80(handle, &stream, &tap) != 0) return 0;
    request = getfreerequest_0D80(stream);
    if (request == 0) return 0;
    if (n == 0) {
        chunk = (STREAMCHUNKHDR *)data;
        if (chunk->type != key) {
            do {
                chunkLen = chunk->size;
                // ORIGINAL-PTR: stream chunks are variable-length records ({type, size} header, size bytes each).
                chunk = (STREAMCHUNKHDR *)((char *)chunk + chunkLen);
                n += chunkLen;
            } while (chunk->type != key);
        }
        n += chunk->size;
    }
    request->parm = n;
    request->address = data;
    request->endchunkid = key;
    request->type = 1;
    queuerequest_0D80(stream, request);
    MUTEX_lock_0D80(&stream->mutex);
    prevState = stream->state;
    if (prevState == 0) stream->state = 1;
    MUTEX_unlock_0D80(&stream->mutex);
    if (prevState == 0) startnextrequest_0D80(stream, 0);
    return request->id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_cancelrequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_unlock_0E90(void *) __asm__("MUTEX_unlock");
extern STREAMCHUNKHDR *STREAM_get_0E90(TAPSTRUCTtag *) __asm__("STREAM_get");
extern void STREAM_release_0E90(TAPSTRUCTtag *, STREAMCHUNKHDR *) __asm__("STREAM_release");

/**
 * Cancels a stream request and throws away the chunks it already delivered.
 *
 * A request not yet started is just freed; a running one is marked cancelled (4). For each tap with data, chunks of the
 * request's range (from its start, or the stream data start for the first request, up to the next request's start or
 * the data tail) that belong to the tap are released: marked -2 and taken off the tap's count and the buffer usage
 * (decbufferusage). A tap whose read position is inside the range is drained through STREAM_get / STREAM_release.
 *
 * @param hnd  the stream handle
 * @param id  the request id
 */
void STREAM_cancelrequest(int hnd, int id) {
    STREAMHEADERtag *st;
    TAPSTRUCTtag *tap;
    char *head;
    char *from;
    char *to;
    STREAMREQUESTtag *request;
    STREAMREQUESTtag *nx;
    int fail;
    int i;

    from = 0;
    to = 0;
    head = 0;
    if (validatehandle(hnd, &st, &tap) != 0) {
        return;
    }
    MUTEX_lock(&st->mutex);
    request = locaterequest1(st, id);
    if (request == 0 || request->state == 4) {
        fail = 1;
    } else if (request->state == 1) {
        freerequest(st, request);
        fail = 1;
    } else {
        request->state = 4;
        head = st->datastart;
        if (request == st->firstreq) {
            from = head;
        } else {
            from = request->datastart;
        }
        nx = request->next;
        if (nx == 0 || nx->state == 1) {
            to = st->datatail;
        } else {
            to = nx->datastart;
        }
        fail = 0;
    }
    MUTEX_unlock_0E90(&st->mutex);
    if (fail) {
        return;
    }
    for (i = 0; i < st->taps; i++) {
        tap = &st->tap[i];
        if (tap->gettable <= 0) {
            continue;
        }
        if (inbetween((unsigned int)head, (unsigned int)from, (unsigned int)tap->getptr)) {
            STREAMCHUNKHDR *chunk = (STREAMCHUNKHDR *)from;
            int tag = tap->tapnum << 24;
            int len;

            while ((char *)chunk != to) {
                if (chunk->type == -1) {
                    chunk = (STREAMCHUNKHDR *)st->bufferstart;
                } else {
                    len = chunk->size & 0xFFFFFF;
                    if ((chunk->size & 0xFF000000) == tag) {
                        MUTEX_lock(&st->mutex);
                        tap->gettable -= len;
                        MUTEX_unlock_0E90(&st->mutex);
                        decbufferusage(st, len);
                        chunk->type = -2;
                        chunk->size = len;
                    }
                    // ORIGINAL-PTR: stream chunks are variable-length records ({type, size} header, size bytes each).
                    chunk = (STREAMCHUNKHDR *)((char *)chunk + len);
                }
            }
        } else if (inbetween((unsigned int)from, (unsigned int)to, (unsigned int)tap->getptr)) {
            do {
                STREAMCHUNKHDR *taken = STREAM_get_0E90(tap);
                STREAM_release_0E90(tap, taken);
            } while (tap->gettable > 0 && inbetween((unsigned int)from, (unsigned int)to, (unsigned int)tap->getptr));
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_kill);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: a per-function volatile view of the heap header (the landed heap functions read it plain). Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
typedef struct { int f0; int f4; } BLK_1110;
typedef struct { int f0; int f4; int f8; int fC; } ENT_1110;
typedef struct { int f0; int f4; } JOB_1110;
typedef struct {
    char pad0[0x24];
    ENT_1110 *f24;
    int f28;
    char *f2C;
    char *f30;
    int pad34;
    int f38;
    char pad3C[0x4C - 0x3C];
    int f4C;
    char *f50;
    char *f54;
    char pad58[4];
    int f5C;
    JOB_1110 *f60;
    JOB_1110 *f64;
} HEAP_1110;
extern int validatehandle_1110(int, volatile HEAP_1110 **, int *) __asm__("validatehandle");
extern void decbufferusage_1110(volatile HEAP_1110 *, int) __asm__("decbufferusage");
extern void freerequest_1110(volatile HEAP_1110 *, int) __asm__("freerequest");
extern void STREAM_cancelrequest_1110(int, int) __asm__("STREAM_cancelrequest");

/**
 * Stops a stream: cancels its started requests (STREAM_cancelrequest), frees the queued ones, marks the current one cancelled,
 * empties every tap, gives back the whole buffer usage and marks every buffered chunk released. A stalled stream (state
 * 2) gets its buffer reset (or the tail padded to 64 bytes) and goes idle.
 *
 * @param streamHandle  the stream handle
 */
void STREAM_kill(int streamHandle) {
    volatile HEAP_1110 *heap;
    int other;
    JOB_1110 *j;
    BLK_1110 *chunk;
    int i, sz, pad;

    sz = 0;
    if (validatehandle_1110(streamHandle, &heap, &other) != 0) {
        return;
    }
    j = heap->f64;
    if (j == 0) {
        return;
    }
    while ((unsigned int)(j->f4 - 1) < 2) {
        STREAM_cancelrequest_1110(streamHandle, j->f0);
        j = heap->f64;
    }
    while (heap->f5C != (int)heap->f60) {
        freerequest_1110(heap, heap->f5C);
    }
    heap->f60->f4 = 4;
    for (i = 0; i < heap->f28; i++) {
        heap->f24[i].f8 = 0;
    }
    decbufferusage_1110(heap, heap->f4C);
    chunk = (BLK_1110 *)heap->f50;
    while ((char *)chunk != heap->f54) {
        if (chunk->f0 == -1) {
            chunk = (BLK_1110 *)heap->f30;
        } else {
            sz = chunk->f4 & 0xFFFFFF;
            chunk->f0 = -2;
            chunk->f4 = sz;
            chunk = (BLK_1110 *)((char *)chunk + sz);
        }
    }
    if (heap->f38 == 2) {
        if ((char *)chunk == heap->f30) {
            heap->f30 = heap->f54 = heap->f2C;
        } else {
            pad = 0x40 - ((int)heap->f54 & 0x3F);
            if (pad == 0x40) {
                pad = 0;
            }
            chunk = (BLK_1110 *)(heap->f54 - sz);
            sz += pad;
            chunk->f4 = sz;
            heap->f54 += pad;
        }
        heap->f38 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_get);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int validatehandle_12E0(int, STREAMHEADERtag **, TAPSTRUCTtag **) __asm__("validatehandle");
extern void MUTEX_lock_12E0(void *) __asm__("MUTEX_lock");
extern void MUTEX_unlock_12E0(void *) __asm__("MUTEX_unlock");
/**
 * Returns the next chunk for a tap and moves the tap on: takes the tap's read position (getptr), clears the tap tag in
 * its size, lowers the tap's byte count (gettable) and, while data is left, finds the next chunk tagged for the tap
 * (following wrap markers to bufferstart). Returns 0 for a bad handle or an empty tap.
 *
 * @param tapHandle  the tap handle
 * @return the chunk, or 0
 */
STREAMCHUNKHDR *STREAM_get(int tapHandle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    char *chunkPtr;
    STREAMCHUNKHDR *chunk;
    char *next;
    int n;
    int rem;
    int tag;
    if (validatehandle_12E0(tapHandle, &stream, &tap) != 0) return 0;
    if (tap->gettable == 0) return 0;
    chunkPtr = tap->getptr;
    chunk = (STREAMCHUNKHDR *)chunkPtr;
    n = chunk->size & 0xFFFFFF;
    chunk->size = n;
    MUTEX_lock_12E0(&stream->mutex);
    rem = tap->gettable - n;
    tap->gettable = rem;
    MUTEX_unlock_12E0(&stream->mutex);
    if (rem > 0) {
        next = chunkPtr + n;
        tag = tap->tapnum << 24;
        while ((((STREAMCHUNKHDR *)next)->size & 0xFF000000) != tag) {
            if (((STREAMCHUNKHDR *)next)->type == -1) {
                next = stream->bufferstart;
            } else {
                n = ((STREAMCHUNKHDR *)next)->size & 0xFFFFFF;
                next += n;
            }
        }
        tap->getptr = next;
    }
    return chunk;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_release);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int fFBD0_13E8(int, void *, void *) __asm__("validatehandle");
extern void fFC48_13E8(STREAMHEADERtag *, int) __asm__("decbufferusage");
extern void fMLK_13E8(MUTEX *) __asm__("MUTEX_lock");
extern void fMUL_13E8(MUTEX *) __asm__("MUTEX_unlock");
extern void f03D0_13E8(STREAMHEADERtag *, int) __asm__("restartstream");

/**
 * Gives a chunk back to its stream: marks it released (-2), lowers the buffer usage (decbufferusage) and restarts a
 * stalled stream (state 2 to 1; restartstream with priorityhigh when greedy, else prioritylow). Chunks outside the
 * buffer (bufferstart .. bufferend - 8) or already released are ignored.
 *
 * @param handle  the tap handle
 * @param chunk  the chunk
 */
void STREAM_release(int handle, STREAMCHUNKHDR *chunk) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    int state;
    if (fFBD0_13E8(handle, &stream, &tap) == 0 && (char *)chunk >= stream->bufferstart
        && stream->bufferend - 8 >= (char *)chunk && chunk->type != -2) {
        chunk->type = -2;
        fFC48_13E8(stream, chunk->size);
        fMLK_13E8(&stream->mutex);
        state = stream->state;
        if (state == 2) stream->state = 1;
        fMUL_13E8(&stream->mutex);
        if (state == 2) {
            if (stream->greedystate != 0) f03D0_13E8(stream, stream->priorityhigh);
            else f03D0_13E8(stream, stream->prioritylow);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_gettable);
#ifdef SKIP_ASM
extern int validatehandle(int, void *, void *);

/**
 * Returns the bytes a tap has ready (gettable), or 0 for a bad handle.
 *
 * @param handle  the tap handle
 * @return the byte count
 */
int STREAM_gettable(int handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    return tap->gettable;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_state);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/**
 * Returns a stream's state (0 idle, 1 reading, 2 stalled for room), or 0 for a bad handle.
 *
 * @param handle  the stream or tap handle
 * @return the state
 */
int STREAM_state(int handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    return stream->state;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_isendofstream);
#ifdef SKIP_ASM
/**
 * Returns whether a stream has ended for a tap: the stream is idle and the tap has nothing left (gettable 0).
 *
 * @param handle  the tap handle
 * @return 1 at the end, else 0 (also for a bad handle)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int STREAM_isendofstream(int handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    return stream->state == 0 && tap->gettable == 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_buffersize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int validatehandle_1580(void *, void *, void *) __asm__("validatehandle");

/**
 * Returns a stream's buffer size: bufferend minus actualbufferstart; 0 for a bad handle.
 *
 * @param handle  the stream handle
 * @return the size in bytes
 */
int STREAM_buffersize(void *handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle_1580(handle, &stream, &tap) != 0) {
        return 0;
    }
    return stream->bufferend - stream->actualbufferstart;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_bufferusage);
#ifdef SKIP_ASM
extern int validatehandle(int, void *, void *);

/**
 * Returns a stream's bufferusage, or 0 for a bad handle.
 *
 * @param handle  the stream handle
 * @return the bytes in use
 */
int STREAM_bufferusage(int handle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    return stream->bufferusage;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_taprequestid);
#ifdef SKIP_ASM
extern int inbetween();
extern void MUTEX_unlock_15E8(MUTEX *) __asm__("MUTEX_unlock");

/**
 * Returns the id of the request a tap is currently reading: the queued request whose data range holds the tap's read
 * position (getptr), found under the mutex. 0 for a bad handle or an empty tap.
 *
 * @param tapHandle  the tap handle
 * @return the request id, or 0
 */
int STREAM_taprequestid(int tapHandle) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    STREAMREQUESTtag *cur;
    STREAMREQUESTtag *request;
    char *start;
    char *reqStart;
    char *readPos;
    if (validatehandle(tapHandle, &stream, &tap)) return 0;
    if (tap->gettable == 0) return 0;
    MUTEX_lock(&stream->mutex);
    cur = stream->firstreq;
    start = stream->datastart;
    readPos = tap->getptr;
    while ((request = cur->next) != 0) {
        if (request->state == 1) break;
        reqStart = request->datastart;
        if (inbetween((unsigned int)start, (unsigned int)reqStart, (unsigned int)readPos)) break;
        cur = cur->next;
        start = reqStart;
    }
    MUTEX_unlock_15E8(&stream->mutex);
    return cur->id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", STREAM_requeststate);
#ifdef SKIP_ASM
/**
 * Returns the state of a stream request (1 queued, 3 done, 4 cancelled), or 0 for a bad handle or an unknown request.
 *
 * @param handle  the stream handle
 * @param requestId  the request id
 * @return the state, or 0
 */
int STREAM_requeststate(int handle, int requestId) {
    STREAMHEADERtag *stream;
    TAPSTRUCTtag *tap;
    STREAMREQUESTtag *request;

    if (validatehandle(handle, &stream, &tap) != 0) {
        return 0;
    }
    request = locaterequest1(stream, requestId);
    if (request == 0) {
        return 0;
    }
    return request->state;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_exists);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int sync_priority_1700[] __asm__("sync_priority");
void FILESYS_existssync_1700(int, int) __asm__("FILESYS_existssync");
/**
 * Returns whether a file exists: FILESYS_existssync at the synchronous priority (sync_priority). Declared void; the result is
 * left in the return register.
 *
 * @param fileName  the path
 */
void FILE_exists(int fileName) {
    FILESYS_existssync_1700(fileName, sync_priority_1700[0]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", filesizeatom);
#ifdef SKIP_ASM
extern int FILESYS_opensync(int, int, int, void *);

/**
 * File-size step for FILESYS_atomic: opens the file named in the argument block, reads its size and closes it.
 *
 * @param priority  the priority (the size and close run one below it)
 * @param namePtr  the argument block; word 0 is the path
 * @return the size, 0 when the file does not open
 */
int filesizeatom(int priority, int *namePtr) {
    int file;
    int size;
    if (FILESYS_opensync(*namePtr, 1, priority, &file) != 0) {
        priority = priority - 1;
        size = FILESYS_sizesync(file, priority);
        FILESYS_closesync(file, priority);
        return size;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_size);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1798(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void filesizeatom_1798(void) __asm__("filesizeatom");
extern int sync_priority_1798[] __asm__("sync_priority");

/**
 * Returns a file's size through FILESYS_atomic (filesizeatom, sync priority), with 1 in the argument block's fourth
 * word. Declared void; the result is left in the return register.
 *
 * @param fileName  the path
 */
void FILE_size(int fileName) {
    int buf[8];

    buf[0] = fileName;
    buf[3] = 1;
    FILESYS_atomic_1798(filesizeatom_1798, 0, sync_priority_1798[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_sizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_17D8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void filesizeatom_17D8(void) __asm__("filesizeatom");
extern int sync_priority_17D8[] __asm__("sync_priority");

/**
 * Returns a file's size through FILESYS_atomic (filesizeatom, sync priority), with 0 in the argument block's fourth
 * word. Declared void; the result is left in the return register.
 *
 * @param fileName  the path
 */
void FILE_sizez(int fileName) {
    int buf[8];

    buf[0] = fileName;
    buf[3] = 0;
    FILESYS_atomic_17D8(filesizeatom_17D8, 0, sync_priority_17D8[0], buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadfileatom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Whole-file load step for FILESYS_atomic: opens the file, allocates its size through the allocate callback
 * (D_00450C14, with the flags in word 4), reads it and closes it; returns the buffer, or 0.
 *
 * @param priority  the priority (the rest runs one below it)
 * @param loadArgs  the argument block: path, then flags at word 4
 * @return the loaded data, or 0
 */
int loadfileatom(int priority, int *loadArgs) {
    int fileHandle;
    int fileSize, buffer;
    if (FILESYS_opensync(loadArgs[0], 1, priority, &fileHandle)) {
        int subPriority = priority - 1;
        fileSize = FILESYS_sizesync(fileHandle, subPriority);
        buffer = ((int (*)(int, int, int))gFileSysOpts.malloc)(loadArgs[0], fileSize, loadArgs[4]);
        if (buffer) {
            FILESYS_readsync(fileHandle, 0, buffer, fileSize, subPriority);
            FILESYS_closesync(fileHandle, subPriority);
            return buffer;
        }
        FILESYS_closesync(fileHandle, subPriority);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1900(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void loadfileatom_18C8(void) __asm__("loadfileatom");
extern int sync_priority_18C8[] __asm__("sync_priority");

/**
 * Loads a whole file into newly allocated memory, synchronously.
 *
 * Runs loadfileatom through FILESYS_atomic() at sync_priority: it opens the file, allocates its size through
 * the file-system allocator hook (with @p flags as the allocation flags) and reads it. The C is declared void,
 * but the asm leaves FILESYS_atomic's result, the buffer or 0, in the return register, and callers use it.
 * FILE_loadz() differs only in the 0 it stores in the argument block's fourth word, which loadfileatom
 * does not read.
 *
 * @param name  file name
 * @param flags  allocation flags for the buffer
 */
void FILE_load(int name, int flags) {
    int buf[8];

    buf[0] = name;
    buf[3] = 1;
    buf[4] = flags;
    FILESYS_atomic_1900(loadfileatom_18C8, 0, sync_priority_18C8[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1908(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void loadfileatom_1908(void) __asm__("loadfileatom");
extern int sync_priority_1908[] __asm__("sync_priority");

/**
 * Loads a whole file into newly allocated memory, synchronously; the variant of FILE_load().
 *
 * Same call as FILE_load() (loadfileatom through FILESYS_atomic() at sync_priority) except that the argument
 * block's fourth word is 0 instead of 1. The C is declared void, but the buffer (or 0) is left in the return
 * register and callers use it.
 *
 * @param name  file name
 * @param flags  allocation flags for the buffer
 */
void FILE_loadz(int name, int flags) {
    int buf[8];

    buf[0] = name;
    buf[3] = 0;
    buf[4] = flags;
    FILESYS_atomic_1908(loadfileatom_1908, 0, sync_priority_1908[0], buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadfilesizeatom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Whole-file load step for FILESYS_atomic that also reports the size: like loadfileatom, storing the size in word 2 of
 * the argument block.
 *
 * @param priority  the priority
 * @param loadArgs  the argument block: path, size out at word 2, flags at word 4
 * @return the loaded data, or 0
 */
int loadfilesizeatom(int priority, int *loadArgs) {
    int fileHandle;
    int fileSize, buffer;
    if (FILESYS_opensync(loadArgs[0], 1, priority, &fileHandle)) {
        int subPriority = priority - 1;
        fileSize = FILESYS_sizesync(fileHandle, subPriority);
        buffer = ((int (*)(int, int, int))gFileSysOpts.malloc)(loadArgs[0], fileSize, loadArgs[4]);
        if (buffer) {
            FILESYS_readsync(fileHandle, 0, buffer, fileSize, subPriority);
            FILESYS_closesync(fileHandle, subPriority);
            loadArgs[2] = fileSize;
            return buffer;
        }
        FILESYS_closesync(fileHandle, subPriority);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadsize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_atomic_1A10(void (*)(void), int, int, void*) __asm__("FILESYS_atomic");
extern int sync_priority_0C38[] __asm__("sync_priority");
extern void loadfilesizeatom_1948(void) __asm__("loadfilesizeatom");

/**
 * Loads a whole file into allocated memory and reports its size: runs loadfilesizeatom through FILESYS_atomic at the sync
 * priority with a FILEREQUEST (abortflag 1, the allocation flags; the size comes back in parm).
 *
 * @param fileName  the path
 * @param sizeOut  receives the size, or 0
 * @param memFlags  the allocation flags
 * @return the data, or 0
 */
int FILE_loadsize(const char *fileName, int *sizeOut, int memFlags) {
    FILEREQUEST loadArgs;
    int data;

    loadArgs.name = fileName;
    loadArgs.abortflag = 1;
    loadArgs.flags = memFlags;
    data = FILESYS_atomic_1A10(loadfilesizeatom_1948, 0, sync_priority_0C38[0], &loadArgs);
    if (sizeOut != 0) {
        *sizeOut = loadArgs.parm;
    }
    return data;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadsizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int FILESYS_atomic_1ACC(void (*)(void), int, int, void*) __asm__("FILESYS_atomic");
extern int sync_priority_0C38[] __asm__("sync_priority");
extern void loadfilesizeatom_1948(void) __asm__("loadfilesizeatom");

/**
 * Loads a whole file into newly allocated memory, synchronously, and reports its size.
 *
 * Runs loadfilesizeatom through FILESYS_atomic() at sync_priority with a FILEREQUEST (abortflag 0); it works like
 * FILE_load's loader but also stores the file size in the request's parm.
 *
 * @param name  file name
 * @param size  if not 0, receives the file size
 * @param flags  allocation flags for the buffer
 * @return the loaded buffer, or 0 on failure
 */
int FILE_loadsizez(int name, int *size, int flags) {
    FILEREQUEST req;
    int buffer;

    req.name = (const char *)name;
    req.abortflag = 0;
    req.flags = flags;
    buffer = FILESYS_atomic_1ACC(loadfilesizeatom_1948, 0, sync_priority_0C38[0], &req);
    if (size != 0) {
        *size = req.parm;
    }
    return buffer;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadfileatatom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int FILESYS_opensync_1AD0(int, int, int, int *) __asm__("FILESYS_opensync");
extern int FILESYS_readsync_1AD0(int, int, int, int, int) __asm__("FILESYS_readsync");
extern void FILESYS_closesync_1AD0(int, int) __asm__("FILESYS_closesync");
typedef struct { int h; int s; int n; } B_1AD0;

// The unit later declares loadfileatatom as a data array; define this one under a local name.
int loadfileatatom_impl(int a, B_1AD0 *b) __asm__("loadfileatatom");
/**
 * Load-at step for FILESYS_atomic: opens the file and reads up to the block's maximum (any size when 0 or less) into
 * its address; returns the bytes read, 0 when the file does not open.
 *
 * @param priority  the priority
 * @param loadReq  the argument block {path, address, maximum}
 * @return the bytes read
 */
int loadfileatatom_impl(int priority, B_1AD0 *loadReq) {
    int file;
    int n = loadReq->n;
    if (n <= 0) n = 0x7FFFFFFF;
    if (FILESYS_opensync_1AD0(loadReq->h, 1, priority, &file)) {
        n = FILESYS_readsync_1AD0(file, 0, loadReq->s, n, priority - 1);
        FILESYS_closesync_1AD0(file, priority - 1);
        return n;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadat);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int sync_priority_0C38[] __asm__("sync_priority");
extern void FILESYS_atomic(int *, int, int, void *);
extern int loadfileatatom[];

/**
 * Loads a file into memory at a given address, synchronously.
 *
 * Runs loadfileatatom through FILESYS_atomic at sync_priority with the block {name, address, maxsize, 1, 0}; the
 * 1 is what distinguishes it from FILE_loadatz.
 *
 * @param name  the file name
 * @param address  where to load it
 * @param maxsize  the maximum number of bytes
 */
void FILE_loadat(int name, int address, int maxsize) {
    int req[5];
    req[0] = name;
    req[1] = address;
    req[2] = maxsize;
    req[3] = 1;
    req[4] = 0;
    FILESYS_atomic(loadfileatatom, 0, sync_priority_0C38[0], req);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadatz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a; int b; int c; int d; int e; int pad[3]; } S_1BB8;
extern int sync_priority_1BB8[] __asm__("sync_priority");
extern void f_1BB8_cb(void) __asm__("loadfileatatom");
extern int FILESYS_atomic_1BB8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");

/**
 * Loads a file into memory at a given address, synchronously (the "z" variant).
 *
 * Runs loadfileatatom through FILESYS_atomic at sync_priority with the block {name, address, maxsize, 0, 0}.
 *
 * @param name  the file name
 * @param address  where to load it
 * @param maxsize  the maximum number of bytes
 */
void FILE_loadatz(int name, int address, int maxsize) {
    S_1BB8 req;
    req.a = name;
    req.b = address;
    req.c = maxsize;
    req.d = 0;
    req.e = 0;
    FILESYS_atomic_1BB8(f_1BB8_cb, 0, sync_priority_1BB8[0], &req);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", scatterloadatom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *FILE_loadz_1C00(const char *, int) __asm__("FILE_loadz");

/**
 * Batch load step for FILESYS_atomic: loads each name of the batch with FILE_loadz (the batch's flags) into its
 * buffer slot; returns 1 when all loaded. The first argument (the priority) is not used.
 *
 * @param priority  the priority (unused)
 * @param batch  {count, names, buffers, flags}
 * @return 1 when every file loaded, else 0
 */
int scatterloadatom(void *priority, SCATTERREQUEST* batch) {
    int index;
    int allOk;

    index = 0;
    allOk = 1;
    if ((batch->num) > 0) {
        do {
            batch->buffers[index] = FILE_loadz_1C00(batch->names[index], batch->flags);
            allOk = (batch->buffers[index] == 0) ? 0 : allOk;
            index++;
        } while (index < (batch->num));
    }
    return allOk;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_scatterload);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cb_1C98(void) __asm__("scatterloadatom");
extern int FILESYS_atomic_1C98(void (*)(void), int, int, SCATTERREQUEST *) __asm__("FILESYS_atomic");
extern int sync_priority_0C38[] __asm__("sync_priority");

/**
 * Loads several files into allocated memory in one FILESYS_atomic run (scatterloadatom) with a SCATTERREQUEST whose
 * abortflag is 1; returns 1 when all loaded, 0 for a count of 0 or less.
 *
 * @param fileCount  the number of files
 * @param names  the names
 * @param buffers  receives the buffers
 * @param memFlags  the allocation flags
 * @return 1 on success, else 0
 */
int FILE_scatterload(int fileCount, const char **names, void **buffers, int memFlags) {
    SCATTERREQUEST req;

    if (fileCount <= 0) return 0;
    {
        req.num = fileCount;
        req.names = names;
        req.buffers = buffers;
        req.flags = memFlags;
        req.abortflag = 1;
        return FILESYS_atomic_1C98(cb_1C98, 0, sync_priority_0C38[0], &req) == 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_scatterloadz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cb_1D00(void) __asm__("scatterloadatom");
extern int FILESYS_atomic_1D00(void (*)(void), int, int, SCATTERREQUEST *) __asm__("FILESYS_atomic");
extern int sync_priority_0C38[] __asm__("sync_priority");

/**
 * Loads several files in one FILESYS_atomic run (scatterloadatom), abortflag 0; otherwise as FILE_scatterload. The
 * batch step ignores abortflag, so both behave the same.
 *
 * @param fileCount  the number of files
 * @param names  the names
 * @param buffers  receives the buffers
 * @param memFlags  the allocation flags
 * @return 1 on success, else 0
 */
int FILE_scatterloadz(int fileCount, const char **names, void **buffers, int memFlags) {
    SCATTERREQUEST req;

    if (fileCount <= 0) return 0;
    {
        req.num = fileCount;
        req.names = names;
        req.buffers = buffers;
        req.flags = memFlags;
        req.abortflag = 0;
        return FILESYS_atomic_1D00(cb_1D00, 0, sync_priority_0C38[0], &req) == 1;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", loadbigheaderatom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* gFileSysOpts: the file system's settings (0x24 bytes: FILESYS_getopts hands out a copy, FILESYS_setopts takes one).
   Field names are their offsets; f04 / f08 are also reached through their own splat symbols. */
/* built-in file table entry (f20) */
extern int FILESYS_opensync(int, int, int, void *);

extern int FILESYS_closesync(int a, int b);
extern int BIG_typeofheader_1D68(char *) __asm__("BIG_typeofheader");
extern int BIG_sizeofheader_1D68(char *) __asm__("BIG_sizeofheader");
extern void MEM_copy_1D68(char *, char *, int) __asm__("MEM_copy");


/**
 * BIG header load step for FILESYS_atomic: reads the first 2704 bytes of an archive into allocated memory and, when the
 * header is larger (BIG_sizeofheader), reallocates and reads the rest; returns the header, or 0 when the file does not
 * open, the memory runs out or it is not a BIG file (BIG_typeofheader 0).
 *
 * @param priority  the priority
 * @param loadArgs  the argument block: path, flags at word 2
 * @return the header, or 0
 */
int loadbigheaderatom(int priority, int *loadArgs) {
    int fileHandle;
    if (FILESYS_opensync(loadArgs[0], 1, priority, &fileHandle)) {
        char *hdr = gFileSysOpts.malloc((char *)loadArgs[0], 2704, loadArgs[2]);
        if (hdr) {
            FILESYS_readsync(fileHandle, 0, (int)hdr, 2704, priority - 1);
            if (BIG_typeofheader_1D68(hdr) == 0) {
                gFileSysOpts.free(hdr);
                FILESYS_closesync(fileHandle, priority - 1);
                return 0;
            }
            {
                int size = BIG_sizeofheader_1D68(hdr);
                if (size > 2704) {
                    char *buf = gFileSysOpts.malloc((char *)loadArgs[0], size, loadArgs[2]);
                    if (buf == 0) {
                        gFileSysOpts.free(hdr);
                        FILESYS_closesync(fileHandle, priority - 1);
                        return 0;
                    }
                    MEM_copy_1D68(buf, hdr, 2704);
                    gFileSysOpts.free(hdr);
                    hdr = buf;
                    FILESYS_readsync(fileHandle, 2704, (int)(hdr + 0xA90), size - 2704, priority - 1);
                }
            }
            FILESYS_closesync(fileHandle, priority - 1);
            return (int)hdr;
        }
        FILESYS_closesync(fileHandle, priority - 1);
        return 0;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadbigheader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1EC8(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void loadbigheaderatom_1EC8(void) __asm__("loadbigheaderatom");
extern int sync_priority_1EC8[] __asm__("sync_priority");

/**
 * Loads a BIG archive's header through FILESYS_atomic (loadbigheaderatom, sync priority), fourth word 1. Declared void; the
 * header is left in the return register.
 *
 * @param fileName  the archive path
 * @param memFlags  the allocation flags
 */
void FILE_loadbigheader(int fileName, int memFlags) {
    int buf[8];

    buf[0] = fileName;
    buf[2] = memFlags;
    buf[3] = 1;
    FILESYS_atomic_1EC8(loadbigheaderatom_1EC8, 0, sync_priority_1EC8[0], buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadbigheaderz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILESYS_atomic_1F08(void (*)(void), int, int, void *) __asm__("FILESYS_atomic");
void loadbigheaderatom_1F08(void) __asm__("loadbigheaderatom");
extern int sync_priority_1F08[] __asm__("sync_priority");

/**
 * Loads a BIG archive's header through FILESYS_atomic (loadbigheaderatom, sync priority), fourth word 0. Declared void.
 *
 * @param fileName  the archive path
 * @param memFlags  the allocation flags
 */
void FILE_loadbigheaderz(int fileName, int memFlags) {
    int buf[8];

    buf[0] = fileName;
    buf[3] = 0;
    buf[2] = memFlags;
    FILESYS_atomic_1F08(loadbigheaderatom_1F08, 0, sync_priority_1F08[0], buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_save);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *REAL_filedevice_1F48[] __asm__("REAL_filedevice");
extern int strncmp_1F48(char *, char *, int) __asm__("strncmp");
extern void strcat_1F48(char *, char *) __asm__("strcat");
extern int sceOpen_1F48(char *, int) __asm__("sceOpen");
extern void sceClose_1F48(int) __asm__("sceClose");
extern void sceWrite_1F48(int, char *, int) __asm__("sceWrite");

/**
 * Writes @p len bytes to a host file (the host root REAL_filedevice is prefixed unless the name has it), in 4 KB writes,
 * ignoring write errors; returns 1 when the file opened.
 *
 * @param name  the file name
 * @param data  the data
 * @param len  the byte count
 * @return 1 when the file opened, else 0
 */
unsigned int FILE_save(char *name, char *data, int len) {
    char buf[256];
    int fd;
    buf[0] = 0;
    if (strncmp_1F48(name, REAL_filedevice_1F48[0], strlen(REAL_filedevice_1F48[0])) != 0) {
        strcpy(buf, REAL_filedevice_1F48[0]);
    }
    strcat_1F48(buf, name);
    fd = sceOpen_1F48(buf, 1538);
    if (fd >= 0) {
        do {
            if (len > 0x1000) {
                sceWrite_1F48(fd, data, 0x1000);
                len -= 0x1000;
                data += 0x1000;
            } else {
                sceWrite_1F48(fd, data, len);
                len = 0;
            }
        } while (len > 0);
        sceClose_1F48(fd);
    }
    return (unsigned int)~fd >> 31;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_savez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *REAL_filedevice_2030[] __asm__("REAL_filedevice");
extern int strncmp_2030(char *, char *, int) __asm__("strncmp");
extern void strcat_2030(char *, char *) __asm__("strcat");
extern int sceOpen_2030(char *, int) __asm__("sceOpen");
extern int sceClose_2030(int) __asm__("sceClose");
extern int sceWrite_2030(int, char *, int) __asm__("sceWrite");

/**
 * Writes @p len bytes to a host file like FILE_save, but returns 0 as soon as a write or the close fails.
 *
 * @param name  the file name
 * @param data  the data
 * @param len  the byte count
 * @return 1 on success, else 0
 */
unsigned int FILE_savez(char *name, char *data, int len) {
    char buf[256];
    int fd;
    buf[0] = 0;
    if (strncmp_2030(name, REAL_filedevice_2030[0], strlen(REAL_filedevice_2030[0])) != 0) {
        strcpy(buf, REAL_filedevice_2030[0]);
    }
    strcat_2030(buf, name);
    fd = sceOpen_2030(buf, 1538);
    if (fd >= 0) {
        do {
            if (len > 0x1000) {
                if (sceWrite_2030(fd, data, 0x1000) < 0) {
                    return 0;
                }
                len -= 0x1000;
                data += 0x1000;
            } else {
                if (sceWrite_2030(fd, data, len) < 0) {
                    return 0;
                }
                len = 0;
            }
        } while (len > 0);
        if (sceClose_2030(fd) < 0) {
            return 0;
        }
    }
    return (unsigned int)~fd >> 31;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_unpacksizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int FILE_loadatz_2130(int, void *, int) __asm__("FILE_loadatz");
int unpacksizez_2130(void *) __asm__("unpacksizez");

/**
 * Returns a packed file's unpacked size: loads its first 0x80 bytes (FILE_loadatz) and reads the size from the pack
 * header (unpacksizez); 0 when the load fails.
 *
 * @param fileName  the path
 * @return the unpacked size, or 0
 */
int FILE_unpacksizez(int fileName) {
    char buf[0x80];
    int var_2;

    if (FILE_loadatz_2130(fileName, buf, 0x80) != 0) {
        var_2 = unpacksizez_2130(buf);
    } else {
        var_2 = 0;
    }
    return var_2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_unpacksize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void FILE_loadatz_2168(int, char *, int) __asm__("FILE_loadatz");
void unpacksizez_2168(char *) __asm__("unpacksizez");
/**
 * Returns a packed file's unpacked size like FILE_unpacksizez, without checking the load. Declared void; the size is left
 * in the return register.
 *
 * @param fileName  the path
 */
void FILE_unpacksize(int fileName) {
    char buf[0x80];
    FILE_loadatz_2168(fileName, buf, 0x80);
    unpacksizez_2168(buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadpackz);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int unpacksizez_2190(int) __asm__("unpacksizez");
extern int unpackz_2190(int, int) __asm__("unpackz");
extern void func_003E6574_2190(int, int, int) __asm__("MEM_copy");
// PORT: PS2-only data-cache index writeback loop (sync + cache op 0x14, way 0/1, 64 sets of 64 bytes).
#define DCACHE_WB_ALL_2190() __asm__ volatile( \
    ".set push\n\t" \
    ".set noreorder\n\t" \
    "li $8, 0\n" \
    "1:\n\t" \
    "sync\n\t" \
    "cache 0x14, 0($8)\n\t" \
    "sync\n\t" \
    "cache 0x14, 1($8)\n\t" \
    "sync\n\t" \
    "addiu $8, $8, 0x40\n\t" \
    "slti $9, $8, 0x1000\n\t" \
    "bnez $9, 1b\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    ".set pop" : : : "$8", "$9", "memory")
int FILE_loadpackz_impl(int a, int size) __asm__("FILE_loadpackz");
/**
 * Loads a file and unpacks it if it is compressed; returns the (unpacked) data.
 *
 * Loads with FILE_loadsizez; data that unpacksizez does not recognise is returned as is. Otherwise the packed
 * bytes are copied to a temporary buffer, the load buffer is freed, a buffer of the unpacked size is allocated
 * with @p size's flags and filled by unpackz, and the temporary buffer is freed.
 *
 * @param name     the file name
 * @param size  allocation flags (On Tour flags)
 * @return the data, or null on failure
 */
int FILE_loadpackz_impl(int name, int size) {
    int sz;
    int data, buf, packedCopy;
    data = FILE_loadsizez(name, &sz, size ^ 0x100);
    if (data == 0) return 0;
    buf = unpacksizez_2190(data);
    if (sz == 0 || buf == 0) return data;
    packedCopy = (int)gFileSysOpts.malloc((char *)name, sz, size ^ 0x100);
    if (packedCopy == 0) {
        gFileSysOpts.free((void *)data);
        return 0;
    }
    func_003E6574_2190(packedCopy, data, sz);
    gFileSysOpts.free((void *)data);
    data = (int)gFileSysOpts.malloc((char *)name, buf, size);
    if (data != 0) {
        if (unpackz_2190(packedCopy, data) == 0) {
            gFileSysOpts.free((void *)data);
            data = 0;
        }
    }
    gFileSysOpts.free((void *)packedCopy);
    DCACHE_WB_ALL_2190();
    return data;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadpack);
#ifdef SKIP_ASM
extern void FILE_loadpackz();

/**
 * Loads a file and unpacks it if compressed (the public entry point); forwards to FILE_loadpackz.
 *
 * Declared without parameters: the name and flags pass through in $4 and $5.
 */
void FILE_loadpack(void) {
    FILE_loadpackz();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadpackatz);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int FILE_loadsizez_pkz(int, int *, int) __asm__("FILE_loadsizez");
extern int unpackz_pkz(int, int) __asm__("unpackz");
extern int unpacksizez_pkz(int) __asm__("unpacksizez");
extern void MEM_copy_pkz(int, int, int) __asm__("MEM_copy");

int FILE_loadpackatz_pkz(int name, int dst, int max) __asm__("FILE_loadpackatz");
/**
 * Loads a file into a caller's buffer, unpacking it first when it is RefPack-compressed.
 *
 * Loads the file with FILE_loadsizez(). When unpacksizez reports an unpacked size (a 0xFB-tagged header) it
 * unpacks into a second allocation with unpackz. It copies at most @p max bytes to @p dst, frees the
 * temporary buffers and writes back the whole data cache.
 *
 * @param name  file name
 * @param dst   destination buffer
 * @param max   maximum number of bytes to copy, or 0 or less for no limit
 * @return the number of bytes copied, or 0 if loading or unpacking failed
 */
// PORT: hand-written EE data-cache writeback of every line (sync + cache 0x14 on ways 0/1, 64 sets of 0x40 bytes), the
// same loop (same text) as the landed FILE_loadpackz DCACHE_WB_ALL_2190 macro; evidence: `addiu $8,$0,0` (gcc emits move = daddu for 0), fixed
// $8/$9 and the unfilled `bnez; nop; nop` (a C loop around sync/cache asm scores 96.61); a port replaces it with its
// platform's data-cache flush (or nothing).
int FILE_loadpackatz_pkz(int name, int dst, int max) {
    int size;
    int buffer, n, buf;
    buffer = FILE_loadsizez_pkz(name, &size, 0x100);
    if (buffer == 0) return 0;
    n = unpacksizez_pkz(buffer);
    if (size > 0 && n > 0) {
        buf = (int)gFileSysOpts.malloc((char *)name, n, 0x100);
        if (buf != 0) {
            if (unpackz_pkz(buffer, buf) == 0) {
                gFileSysOpts.free((void *)buf);
                buf = 0;
            }
        }
        gFileSysOpts.free((void *)buffer);
        if (buf == 0) return 0;
        size = n;
        buffer = buf;
    }
    if (max > 0 && max < size) size = max;
    MEM_copy_pkz(dst, buffer, size);
    gFileSysOpts.free((void *)buffer);
    /* PORT: hand-written data-cache writeback loop (see the PORT line above the function). */
    __asm__ __volatile__(
        ".set push\n\t"
        ".set noreorder\n\t"
        "li $8, 0\n"
        "1:\n\t"
        "sync\n\t"
        "cache 0x14, 0($8)\n\t"
        "sync\n\t"
        "cache 0x14, 1($8)\n\t"
        "sync\n\t"
        "addiu $8, $8, 0x40\n\t"
        "slti $9, $8, 0x1000\n\t"
        "bnez $9, 1b\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set pop" : : : "$8", "$9", "memory");
    return size;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILE_loadpackat);
#ifdef SKIP_ASM
extern void FILE_loadpackatz();

/**
 * Thin wrapper that calls FILE_loadpackatz().
 *
 * Declared without parameters: the caller's arguments reach FILE_loadpackatz() unchanged in their registers.
 */
void FILE_loadpackat(void) {
    FILE_loadpackatz();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_stricmp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_00499C81_2490[] __asm__("D_00499C81");
static inline int low_2490(unsigned char *t, int c) {
    int r = c;
    if (*(unsigned char *)(c + (int)t) & 1) r = c + 0x20;
    return r;
}

/**
 * Compares two strings ignoring case (upper-case letters per the ctype table D_00499C81 fold to lower case); returns
 * the difference at the first mismatch, 0 when equal.
 *
 * @param lhs  the first string
 * @param rhs  the second string
 * @return the difference
 */
int BIG_stricmp(signed char *lhs, signed char *rhs) {
    unsigned char *ctypeTable = D_00499C81_2490;
    signed char *lhsPtr = lhs;
    signed char *rhsPtr = rhs;
    signed char lhsChar = *lhsPtr;
    signed char rhsChar = *rhsPtr;
    int diff;
    while ((diff = low_2490(ctypeTable, lhsChar) - low_2490(ctypeTable, rhsChar)) == 0 && lhsChar != 0) {
        rhsPtr++;
        lhsPtr++;
        lhsChar = *lhsPtr;
        rhsChar = *rhsPtr;
    }
    return diff;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_typeofheader);
#ifdef SKIP_ASM
/**
 * Identifies a big-file (archive) header by its magic.
 *
 * @param header  the header
 * @return 1 for the compact 0xC0FB form, 2 for "BIGF", 3 for another "BIG" variant, 0 when not a big file
 */
int BIG_typeofheader(unsigned char *header) {
    int type;
    unsigned char byte0 = header[0];
    unsigned char byte1 = header[1];
    unsigned int magic;
    if (((byte0 << 8) | byte1) == 49403) {
        type = 1;
    } else {
        magic = (byte0 << 24) | (byte1 << 16) | (header[2] << 8) | header[3];
        if (magic == 'BIGF') {
            type = 2;
        } else {
            type = (((magic & 0xFFFFFF00) ^ 0x42494700) != 0) ? 0 : 3;
        }
    }
    return type;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_sizeofheader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int BIG_typeofheader_SZ(unsigned char *) __asm__("BIG_typeofheader");

int BIG_sizeofheader_impl(unsigned char *h) __asm__("BIG_sizeofheader");
/**
 * Returns the header size of a BIG archive from its first bytes.
 *
 * For a 0xC0FB header (BIG_typeofheader() type 1) the size is the big-endian 16-bit value at byte 2 plus 4. For
 * "BIGF" and other "BIG" headers (types 2 and 3) it is the big-endian 32-bit value at byte 12.
 *
 * @param bigFile  start of the archive
 * @return the header size in bytes, or 0 if @p bigFile is not a BIG archive
 */
int BIG_sizeofheader_impl(unsigned char *bigFile) {
    int headerSize = 0;
    int headerType = BIG_typeofheader_SZ(bigFile);
    if (headerType == 1) {
        headerSize = ((bigFile[2] << 8) | bigFile[3]) + 4;
    } else if (headerType > 0) {
        if (headerType < 4) {
            unsigned char *sizeField = bigFile + 12;
            headerSize = (sizeField[0] << 24) | (sizeField[1] << 16) | (sizeField[2] << 8) | sizeField[3];
        }
    }
    return headerSize;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_debuginfo);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int BIG_sizeofheader_debuginfo(char *) __asm__("BIG_sizeofheader");
/**
 * Checks a big-file header for the 8-byte debug trailer: a letter and three digits, then a 4-byte tag.
 *
 * @param base  the big-file header
 * @param num  receives the three-digit number, or 0
 * @param tag  receives the tag, or 0
 * @return 8 (the trailer's size) when present, otherwise 0
 */
int BIG_debuginfo(char *base, int *num, int *tag) {
    int infoSize = 0;
    // ORIGINAL-PTR: a BIG file header blob: the optional 8-byte debug trailer ends the variable-size header
    // (BIG_sizeofheader); On Tour's BIG_debuginfo keeps a byte pointer into it (`const char* venomptr`).
    unsigned char *headerEnd = (unsigned char *)base + BIG_sizeofheader_debuginfo(base);
    unsigned char firstChar = headerEnd[-8];
    unsigned char *venomPtr = headerEnd - 8;
    if (((unsigned)(firstChar - 65) < 26 || (unsigned)(firstChar - 97) < 26)
        && (unsigned)(venomPtr[1] - 0x30) < 10
        && (unsigned)(venomPtr[2] - 0x30) < 10
        && (unsigned)(venomPtr[3] - 0x30) < 10) {
        infoSize = 8;
        if (num != 0) {
            *num = ((signed char)venomPtr[1] - 0x30) * 100 + ((signed char)venomPtr[2] - 0x30) * 10 + ((signed char)venomPtr[3] - 0x30);
        }
        if (tag != 0) {
            unsigned char *flagsPtr = headerEnd - 4;
            *tag = (flagsPtr[0] << 24) | (flagsPtr[1] << 16) | (flagsPtr[2] << 8) | flagsPtr[3];
        }
    }
    return infoSize;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_version);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void BIG_debuginfo_2740(int, int *, int) __asm__("BIG_debuginfo");
/**
 * Returns the version number of a BIG archive (the first value BIG_debuginfo reports), or 0.
 *
 * @param bigFile  the archive header
 * @return the version
 */
int BIG_version(int bigFile) {
    int x = 0;
    BIG_debuginfo_2740(bigFile, &x, 0);
    return x;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locateentryz);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int BIG_debuginfo_LOC(char *, int *, int *) __asm__("BIG_debuginfo");
extern int BIG_sizeofheader_LOC(char *) __asm__("BIG_sizeofheader");
extern int BIG_typeofheader_LOC(char *) __asm__("BIG_typeofheader");
extern int BIG_stricmp_LOC(char *, char *) __asm__("BIG_stricmp");
extern int strlen(const char *);

static inline unsigned rd4_LOC(unsigned char *p, int n) {
    if (n == 1) return p[0];
    if (n == 2) return p[0] << 8 | p[1];
    if (n == 3) return p[0] << 16 | p[1] << 8 | p[2];
    if (n == 4) return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
    return 0;
}

static inline long rd8_LOC(unsigned char *p, int n) {
    if (n < 5) return rd4_LOC(p, n);
    {
        long hi = rd4_LOC(p, n - 4);
        return (hi << 32) | rd4_LOC(p + n - 4, 4);
    }
}

char *BIG_locateentryz_LOC(char *base, char *name, int idx, int *out1, int *out2) __asm__("BIG_locateentryz");
/**
 * Finds a big-file directory entry by name (case-insensitive) or, without a name, by index.
 *
 * Offset and size field widths follow the header type (3 bytes each in the compact form).
 *
 * @param base  the big-file header
 * @param name  the entry name, or 0 to look up by @p idx
 * @param idx  the entry index when @p name is 0
 * @param out1  receives the entry's offset, or 0
 * @param out2  receives the entry's size, or 0
 * @return the entry's name in the directory, or 0 when not found
 */
char *BIG_locateentryz_LOC(char *base, char *name, int idx, int *out1, int *out2) {
    // ORIGINAL-PTR: a BIG directory: variable-length entries (offset and size fields of 3..5 bytes, then a zero-
    // terminated name) after the 16-byte (compact type: 6-byte) header; On Tour walks it with byte pointers
    // (`const unsigned char* bigentry`, `unsigned char* hend`).
    unsigned char *bigEntry = (unsigned char *)base + 0x10;
    int sizeOfOffset = 4;
    int sizeOfLen = 4;
    int i = 0;
    int dbg;
    unsigned char *end;
    int headerType;

    dbg = BIG_debuginfo_LOC(base, 0, 0);
    end = (unsigned char *)base + BIG_sizeofheader_LOC(base) - dbg;
    headerType = BIG_typeofheader_LOC(base);
    switch (headerType) {
    case 2:
        break;
    case 3:
        sizeOfOffset = base[3] - 0x30;
        sizeOfLen = sizeOfOffset;
        if (sizeOfOffset > 4) sizeOfLen = 4;
        break;
    case 1:
        bigEntry = (unsigned char *)base + 6;
        sizeOfOffset = 3;
        sizeOfLen = 3;
        break;
    }
    if (name != 0) {
        while (bigEntry < end) {
            unsigned char *key = bigEntry + sizeOfOffset + sizeOfLen;
            if (BIG_stricmp_LOC((char *)key, name) == 0) {
                if (out1 != 0) *out1 = rd8_LOC(bigEntry, sizeOfOffset);
                if (out2 == 0) return (char *)key;
                *out2 = rd4_LOC(bigEntry + sizeOfOffset, sizeOfLen);
                return (char *)key;
            }
            i = strlen((char *)key);
            bigEntry = key + i + 1;
        }
    } else {
        while (bigEntry < end) {
            unsigned char *key = bigEntry + sizeOfOffset + sizeOfLen;
            if (i == idx) {
                if (out1 != 0) *out1 = rd8_LOC(bigEntry, sizeOfOffset);
                if (out2 == 0) return (char *)key;
                *out2 = rd4_LOC(bigEntry + sizeOfOffset, sizeOfLen);
                return (char *)key;
            }
            i++;
            bigEntry = key + strlen((char *)key) + 1;
        }
    }
    if (out1 != 0) *out1 = 0;
    if (out2 != 0) *out2 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locateentry);
#ifdef SKIP_ASM
extern void BIG_locateentryz();

/**
 * Looks up a BIG archive entry by name or number: passes its five arguments (archive, name, number, offset out, size
 * out) straight to BIG_locateentryz and returns its result (the entry name, or 0). Declared void(void).
 */
void BIG_locateentry(void) {
    BIG_locateentryz();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locateoffset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void BIG_locateentry_2D00(int, int, int, void *, int) __asm__("BIG_locateentry");

/**
 * Returns the offset of a named entry in a BIG archive (through BIG_locateentry), or 0 for no name or no such entry.
 *
 * @param bigfileptr  the archive header
 * @param name  the entry name
 * @return the offset, or 0
 */
int BIG_locateoffset(bigfileptr, name)
int bigfileptr;
int name;
{
    int offset;

    offset = 0;
    if (name != 0) {
        BIG_locateentry_2D00(bigfileptr, name, 0, &offset, 0);
    }
    return offset;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locateoffsetz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void BIG_locateentryz_2D30(int, int, int, void *, int) __asm__("BIG_locateentryz");

/**
 * Returns the offset of a named entry in a BIG archive, calling BIG_locateentryz directly; 0 for no name or no such
 * entry.
 *
 * @param bigfileptr  the archive header
 * @param name  the entry name
 * @return the offset, or 0
 */
int BIG_locateoffsetz(bigfileptr, name)
int bigfileptr;
int name;
{
    int offset;

    offset = 0;
    if (name != 0) {
        BIG_locateentryz_2D30(bigfileptr, name, 0, &offset, 0);
    }
    return offset;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_pointer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_2D60_cb(int, int, int, int *, int) __asm__("BIG_locateentry");

/**
 * Returns a pointer to entry number @p x of a BIG archive (header + offset, through BIG_locateentry), or 0.
 *
 * @param base  the archive (its header first)
 * @param x  the entry number
 * @return the entry data, or 0
 */
int BIG_pointer(int base, int x) {
    int out;
    if (f_2D60_cb(base, 0, x, &out, 0) != 0)
        return base + out;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_pointerz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_2DA8_cb(int, int, int, int *, int) __asm__("BIG_locateentryz");

/**
 * Returns a pointer to entry number @p x of a BIG archive (header + offset, through BIG_locateentryz), or 0.
 *
 * @param base  the archive (its header first)
 * @param x  the entry number
 * @return the entry data, or 0
 */
int BIG_pointerz(int base, int x) {
    int out;
    if (f_2DA8_cb(base, 0, x, &out, 0) != 0)
        return base + out;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locate);
#ifdef SKIP_ASM
extern int BIG_locateoffset();

/**
 * Returns a pointer to a named entry of a BIG archive (header + BIG_locateoffset's offset), or 0. Declared with one
 * parameter; the name passes through in the second argument register.
 *
 * @param bigBase  the archive (its header first)
 * @return the entry data, or 0
 */
int BIG_locate(int bigBase) {
    int temp_2;

    temp_2 = BIG_locateoffset();
    return (temp_2 == 0) ? 0 : (bigBase + temp_2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locatez);
#ifdef SKIP_ASM
extern int BIG_locateoffsetz();

/**
 * Returns a pointer to a named entry of a BIG archive (header + BIG_locateoffsetz's offset), or 0. Declared with one
 * parameter; the name passes through in the second argument register.
 *
 * @param bigBase  the archive (its header first)
 * @return the entry data, or 0
 */
int BIG_locatez(int bigBase) {
    int temp_2;

    temp_2 = BIG_locateoffsetz();
    return (temp_2 == 0) ? 0 : (bigBase + temp_2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_locatemany);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int BIG_locate_2E50(int, signed char *) __asm__("BIG_locate");
extern signed char *strchr_2E50(signed char *, int) __asm__("strchr");

/**
 * Looks up a comma-separated list of entry names in a BIG archive, storing each entry pointer (BIG_locate, 0 when
 * missing) in turn in @p out. The commas are cut and put back, so @p names must be writable.
 *
 * @param bigBase  the archive (its header first)
 * @param names  the entry names, separated by commas
 * @param out  receives one pointer per name
 */
void BIG_locatemany(int bigBase, signed char *names, int *out) {
    signed char *temp_2;
    signed char *name = names;
    do {
        temp_2 = strchr_2E50(name, 44);
        if (temp_2 != 0) *temp_2 = 0;
        *out = BIG_locate_2E50(bigBase, name);
        out++;
        if (temp_2 == 0) break;
        *temp_2 = 44;
        temp_2++; name = temp_2;
    } while (temp_2 != 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_count);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int BIG_typeofheader_2EE0(unsigned char *) __asm__("BIG_typeofheader");

int BIG_count_impl(unsigned char *h) __asm__("BIG_count");
/**
 * Returns the number of entries in a BIG archive: the big-endian halfword at +0x4 for header type 1, the big-endian
 * word at +0x8 for types 2 and 3 (BIG_typeofheader), else 0.
 *
 * @param header  the archive header
 * @return the entry count
 */
int BIG_count_impl(unsigned char *header) {
    int fileCount = 0;
    int headerType = BIG_typeofheader_2EE0(header);
    if (headerType == 1) {
        fileCount = (header[4] << 8) | header[5];
    } else if (headerType > 0) {
        if (headerType < 4) {
            unsigned char *countField = header + 8;
            fileCount = (countField[0] << 24) | (countField[1] << 16) | (countField[2] << 8) | countField[3];
        }
    }
    return fileCount;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_name);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *strcpy(char *, const char *);
char *BIG_locateentry_2F70(int, int, int, int, int) __asm__("BIG_locateentry");

/**
 * Copies the name of entry number @p index of a BIG archive (BIG_locateentry) into @p dst; returns @p dst, or 0 with an
 * empty @p dst when there is no such entry.
 *
 * @param bigBase  the archive header
 * @param index  the entry number
 * @param dst  receives the name
 * @return @p dst, or 0
 */
char *BIG_name(int bigBase, int index, char *dst) {
    char *entryName = BIG_locateentry_2F70(bigBase, 0, index, 0, 0);
    char *result;
    if (!entryName) {
        *dst = 0;
        result = 0;
    } else {
        strcpy(dst, entryName);
        result = dst;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", BIG_namez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *strcpy(char *, const char *);
char *BIG_locateentryz_2FC8(int, int, int, int, int) __asm__("BIG_locateentryz");

/**
 * Copies the name of entry number @p index of a BIG archive (BIG_locateentryz) into @p dst; returns @p dst, or 0 with
 * an empty @p dst when there is no such entry.
 *
 * @param bigBase  the archive header
 * @param index  the entry number
 * @param dst  receives the name
 * @return @p dst, or 0
 */
char *BIG_namez(int bigBase, int index, char *dst) {
    char *entryName = BIG_locateentryz_2FC8(bigBase, 0, index, 0, 0);
    char *result;
    if (!entryName) {
        *dst = 0;
        result = 0;
    } else {
        strcpy(dst, entryName);
        result = dst;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SYNCTASK_add(void (*)(void), int, int);
extern void FILEDEV_readexec(void);
extern int currentfilesystem_3020[] __asm__("currentfilesystem");
extern int availablefilesystems_3020[] __asm__("availablefilesystems");

/**
 * Starts the file device layer: clears the device flags (availablefilesystems), adds the host read task FILEDEV_readexec as a sync
 * task and marks the host device available (flag 2); with a disc it also marks the disc (flag 1) and makes it the
 * default device (currentfilesystem = 1), otherwise the host is the default (2). The first argument is not used.
 *
 * @param maxOpenFiles  the maximum open files (unused)
 * @param discAvail  nonzero when the CD file table was built
 */
void FILEDEV_init(int maxOpenFiles, int discAvail) {
    int temp_3;

    availablefilesystems_3020[0] = 0;
    SYNCTASK_add(FILEDEV_readexec, 0, 0);
    temp_3 = availablefilesystems_3020[0];
    availablefilesystems_3020[0] = temp_3 | 2;
    if (discAvail != 0) {
        availablefilesystems_3020[0] = temp_3 | 3;
        currentfilesystem_3020[0] = 1;
        return;
    }
    currentfilesystem_3020[0] = 2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_resolvename);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* gFileSysOpts: the file system's settings (0x24 bytes: FILESYS_getopts hands out a copy, FILESYS_setopts takes one).
   Field names are their offsets; f04 / f08 are also reached through their own splat symbols. */
/* built-in file table entry (f20) */
extern char *REAL_filedevice;
extern char currentdirectory[0x100];                  /* the current directory */
extern char *strchr_3098(char *, int) __asm__("strchr");
extern int strlen_3098(char *) __asm__("strlen");
extern int strncmp_3098(char *, char *, int) __asm__("strncmp");
extern char *strcat(char *, char *);
extern char *CDdevicename;
extern int currentfilesystem;

void FILEDEV_resolvename_i(char *path, char *out, int *kind) __asm__("FILEDEV_resolvename");
/**
 * Turns a file name into a device path and says which device it is on (1 disc, 2 host).
 *
 * The default device is currentfilesystem. A name with a ':' must start with "cd:" (disc; the prefix is dropped) or the host
 * root REAL_filedevice ("host0:", host; dropped too); any other prefix is refused (empty @p out) on a disc default, or
 * unless its third character is the ':'. Host paths get the host root back in front; a name not starting with '\\', '/'
 * or '~' gets the current directory (currentdirectory, set by FILESYS_setcurrentpath) in front. Backslashes become slashes. Quirk: a
 * refused name returns before @p kind is written, so the caller's value stays uninitialised.
 *
 * @param path  the file name
 * @param out  receives the device path (256 bytes)
 * @param kind  receives the device, or 0
 */
void FILEDEV_resolvename_i(char *path, char *out, int *kind) {
    int k;
    out[0] = 0;
    k = currentfilesystem;
    if (strchr_3098(path, 58) != 0) {
        int n = strlen_3098(REAL_filedevice);
        if (strncmp_3098(path, CDdevicename, 3) == 0) {
            k = 1;
            path += 3;
        } else if (strncmp_3098(path, REAL_filedevice, n) == 0) {
            path += n;
            k = 2;
        } else {
            if (currentfilesystem == 1) return;
            if (path[2] != 58) return;
        }
    }
    if (k == 2) {
        strcat(out, REAL_filedevice);
    }
    if (path[0] != 92 && path[0] != 47 && path[0] != 0x7E) {
        strcat(out, currentdirectory);
    }
    strcat(out, path);
    for (; *out != 0; out++) {
        if (*out == 92) {
            *out = 47;
        }
    }
    if (kind != 0) {
        *kind = k;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_open);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void FILEDEV_resolvename(int, char *, int *);
extern int CDVD_findFileNumber_3208(char *, int, int *) __asm__("CDVD_findFileNumber");
extern int sceOpen(char *, int);

/**
 * Opens a file on its device; returns 1 with the device handle (device in the top byte, 1 disc or 2 host) in
 * @p out, or 0.
 *
 * The name is resolved by FILEDEV_resolvename. Disc files are looked up in the CD file table (CDVD_findFileNumber). Host files are
 * opened by open mode (flags & 7): 1 read-only (1), 0 read-write appending (0x103), 2 read-write creating (0x203), 6
 * read-write creating and truncating (0x603); any other mode gives -1.
 *
 * @param path  the file name
 * @param flags  the open flags
 * @param out  receives the device handle
 * @return 1 when it opened, else 0
 */
int FILEDEV_open(int path, unsigned int flags, int *out) {
    char buf[256];
    int kind;
    int mode, fd;

    FILEDEV_resolvename(path, buf, &kind);
    if (kind == 1) {
        if (CDVD_findFileNumber_3208(buf, flags, out) == 0) {
            return 0;
        }
        *out = 0x01000000 | *out;
        return 1;
    }
    if (kind == 2) {
        switch (flags & 7) {
        case 1:
            mode = 1;
            break;
        case 0:
            mode = 259;
            break;
        case 2:
            mode = 515;
            break;
        case 6:
            mode = 1539;
            break;
        default:
            *out = -1;
            return 0;
        }
        fd = sceOpen(buf, mode);
        *out = fd;
        if (fd >= 0) {
            *out = fd | (kind << 24);
            return 1;
        }
        *out = 0;
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_close);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void sceClose(int);
extern int FILEDEV_lasthandle_0C40[] __asm__("FILEDEV_lasthandle");

/**
 * Closes a device handle: a host file is closed (forgetting it as the last-read file FILEDEV_lasthandle); disc handles need
 * nothing. Returns 0.
 *
 * @param fsHandle  the device handle
 * @return 0
 */
int FILEDEV_close(int fsHandle) {
    int fileDesc;
    int temp_5;

    temp_5 = fsHandle >> 0x18;
    fileDesc = fsHandle & 0xFFFFFF;
    if (temp_5 == 2) {
        sceClose(fileDesc);
        if (FILEDEV_lasthandle_0C40[0] == fileDesc) {
            FILEDEV_lasthandle_0C40[0] = -1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_readreq);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct S_3350 { int a, b, c, d, e; };
extern volatile struct S_3350 gHarddriveReadRequest_3350 __asm__("gHarddriveReadRequest");
extern int func_003E4648(int);

/**
 * Starts a read on a device handle. A disc read goes to func_003E4648 (the other arguments pass through in their
 * registers) and its result is returned. A host read is only recorded (gHarddriveReadRequest) for the sync task FILEDEV_readexec,
 * which does it and completes the command; returns 1.
 *
 * @param fsHandle  the device handle
 * @param buffer  the destination
 * @param fileOffset  the file offset
 * @param byteCount  the byte count
 * @return 1 for a host read, else the disc read's result
 */
int FILEDEV_readreq(int fsHandle, int buffer, int fileOffset, int byteCount) {
    int lo = fsHandle & 0xFFFFFF;
    if ((fsHandle >> 24) == 1) {
        return func_003E4648(lo);
    }
    gHarddriveReadRequest_3350.a = 1;
    gHarddriveReadRequest_3350.b = lo;
    gHarddriveReadRequest_3350.c = buffer;
    gHarddriveReadRequest_3350.d = fileOffset;
    gHarddriveReadRequest_3350.e = byteCount;
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_readexec);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int FILEDEV_lasthandle_33B0 __asm__("FILEDEV_lasthandle");
extern int D_00450C4C_33B0 __asm__("D_00450C4C");
extern int sceRead_33B0(int, int, int) __asm__("sceRead");
extern void iFILESYS_CommandCompleteCallback_33B0(int) __asm__("iFILESYS_CommandCompleteCallback");

int FILEDEV_readexec_i(void) __asm__("FILEDEV_readexec");
/**
 * Sync task of the host device: does the read FILEDEV_readreq recorded. It seeks unless the file and offset are where the
 * last read ended (FILEDEV_lasthandle, D_00450C4C), reads, remembers the new position and completes the command
 * (iFILESYS_CommandCompleteCallback, success when every byte was read). Returns 0.
 *
 * @return 0
 */
int FILEDEV_readexec_i(void) {
    int bytesRead;
    int ok;
    if (gHarddriveReadRequest_3350.a != 0) {
        if (FILEDEV_lasthandle_33B0 != gHarddriveReadRequest_3350.b || D_00450C4C_33B0 != gHarddriveReadRequest_3350.d) {
            sceLseek(gHarddriveReadRequest_3350.b, gHarddriveReadRequest_3350.d, 0);
        }
        bytesRead = sceRead_33B0(gHarddriveReadRequest_3350.b, gHarddriveReadRequest_3350.c, gHarddriveReadRequest_3350.e);
        ok = bytesRead == gHarddriveReadRequest_3350.e;
        FILEDEV_lasthandle_33B0 = gHarddriveReadRequest_3350.b;
        D_00450C4C_33B0 = gHarddriveReadRequest_3350.d + gHarddriveReadRequest_3350.e;
        gHarddriveReadRequest_3350.a = 0;
        iFILESYS_CommandCompleteCallback_33B0(ok);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_write);
#ifdef SKIP_ASM
/**
 * Writes to a host file: seeks to @p offset, writes and completes the command as a success
 * (iFILESYS_CommandCompleteCallback(1)) whatever happened. Returns 0. Quirk: the remembered read position (FILEDEV_lasthandle,
 * D_00450C4C) is not cleared, so a later read of the same file at that position skips its seek and reads after the
 * written data.
 *
 * @param handle  the device handle
 * @param buffer  the data
 * @param offset  the file offset
 * @param size  the byte count
 * @return 0
 */
int FILEDEV_write(int handle, int buffer, int offset, int size) {
    int fileDesc = handle & 0xFFFFFF;
    sceLseek(fileDesc, offset, 0);
    sceWrite(fileDesc, buffer, size);
    iFILESYS_CommandCompleteCallback(1);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_getsize);
#ifdef SKIP_ASM
extern void CDVD_readFileEntry(int, int, int, int);
extern void sceLseek(int, int, int);

/**
 * Returns the size of an open file: a disc file's from the CD file table (CDVD_readFileEntry), a host file's by seeking to
 * its end. Declared void; the size is left in the return register.
 *
 * @param handle  the device handle
 */
void FILEDEV_getsize(int handle) {
    int temp_4;

    temp_4 = handle & 0xFFFFFF;
    if ((handle >> 0x18) == 1) {
        CDVD_readFileEntry(temp_4, 0, 0, 0);
        return;
    }
    sceLseek(temp_4, 0, 2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", FILEDEV_getstartsector);
#ifdef SKIP_ASM
/**
 * Returns the start sector of an open disc file (from the CD file table, CDVD_readFileEntry), or 0 for a host file.
 *
 * @param handle  the device handle
 * @return the sector, or 0
 */
int FILEDEV_getstartsector(int handle) {
    int startSector;
    int idx = handle & 0xFFFFFF;

    if ((handle >> 24) == 1) {
        CDVD_readFileEntry(idx, 0, 0, &startSector);
    } else {
        startSector = 0;
    }
    return startSector;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_signalAlarmSema);
#ifdef SKIP_ASM
void iSignalSema_3588(int) __asm__("iSignalSema");
/**
 * Alarm handler of CDVD_sleepHsyncs: signals the waiting semaphore and re-enables interrupts.
 *
 * @param alarmId  the alarm id
 * @param alarmTime  the alarm time
 * @param semaId  the semaphore
 */
void CDVD_signalAlarmSema(int alarmId, int alarmTime, int semaId) {
    iSignalSema_3588(semaId);
    // PORT: MIPS sync + EE ei (no C equivalent)
    __asm__ volatile("sync\n\tei");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_sleepHsyncs);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* The kernel's semaphore parameters (Sony's SemaParam; the field names are CDVD_initFileSystem's SemaParam_4040's). */
typedef struct Cfg_35B0 { int count, max_count, init_count, wait_threads, attr, option; } Cfg_35B0;
extern void cb_35B0(void) __asm__("CDVD_signalAlarmSema");
extern void SetAlarm_35B0(int, void (*)(void), int) __asm__("SetAlarm");
extern int CreateSema_35B0(Cfg_35B0 *) __asm__("CreateSema");

/**
 * Sleeps the calling thread for @p hsyncTicks alarm ticks (H-sync periods): sets an alarm (handler CDVD_signalAlarmSema) that
 * signals a new semaphore, waits on it and deletes it.
 *
 * @param hsyncTicks  the delay in H-sync periods
 */
void CDVD_sleepHsyncs(unsigned short hsyncTicks) {
    Cfg_35B0 semaParam;
    int sema;

    semaParam.max_count = 1;
    semaParam.init_count = 0;
    semaParam.option = 0;
    sema = CreateSema_35B0(&semaParam);
    SetAlarm_35B0(hsyncTicks, cb_35B0, sema);
    WaitSema(sema);
    DeleteSema(sema);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_readCurrentSector);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int CDVD_state_3618[] __asm__("CDVD_state");
extern char D_00519C80_3618[] __asm__("D_00519C80");
extern int FILESYS_cdSettings_3618[] __asm__("FILESYS_cdSettings");
extern int sceCdRead_3618(int, int, void *, int *) __asm__("sceCdRead");
extern int sceCdSync_3618(int) __asm__("sceCdSync");
extern void sceCdInit_3618(int) __asm__("sceCdInit");
extern void sceCdMmode_3618(int) __asm__("sceCdMmode");
extern void sceCdDiskReady_3618(int) __asm__("sceCdDiskReady");
extern int sceCdGetError_3618(void) __asm__("sceCdGetError");
/**
 * Reads one disc sector, the one at the CD state's sector (CDVD_state +0x2C), into the sector buffer D_00519C80 and
 * returns it. Retries until the read reports no error, waiting 480 ticks between polls; every 10 failures the CD is
 * initialised again (sceCdInit(0), media mode from FILESYS_cdSettings +0x8).
 *
 * @return the sector buffer
 */
char *CDVD_readCurrentSector(void) {
    int n = 10;
    int syncBusy;
    int done;
    do {
        if (n == 0) {
            sceCdInit_3618(0);
            n = 10;
            sceCdMmode_3618(FILESYS_cdSettings_3618[2]);
        }
        sceCdDiskReady_3618(0);
        while (sceCdRead_3618(CDVD_state_3618[11], 1, D_00519C80_3618, &CDVD_state_3618[7]) == 0) {
            CDVD_sleepHsyncs(0x1E0);
        }
        do {
            syncBusy = sceCdSync_3618(1);
            CDVD_sleepHsyncs(0x1E0);
        } while (syncBusy != 0);
        done = sceCdGetError_3618() == 0;
        if (!done) n--;
    } while (!done);
    return D_00519C80_3618;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_hashname);
#ifdef SKIP_ASM
extern int strlen();

/**
 * Hashes a string for the CD file table: from @p seed, hash = hash * 33 + byte for each byte from the last to the
 * first. Quirk: an empty string reads the byte before it.
 *
 * @param str  the string
 * @param seed  the starting value
 * @return the hash
 */
int CDVD_hashname(const char *str, int seed) {
    int hash = seed;
    const char *cursor = str + strlen(str) - 1;

    do {
        signed char chr = *cursor;
        cursor -= 1;
        hash = hash * 33 + chr;
    } while (cursor >= str);
    return hash;
}
#endif

INCLUDE_ASM("ealib/seg_2DCAE8", func_003E3758);

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", fi_cmp);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int hashcollision_3968[] __asm__("hashcollision");

/**
 * Sort comparator of the CD file table: orders entries by their first word (the name hash), unsigned; equal hashes
 * return 0 and count a collision in hashcollision.
 *
 * @param lhs  the first entry
 * @param rhs  the second entry
 * @return -1, 0 or 1
 */
int fi_cmp(unsigned int *lhs, unsigned int *rhs) {
    long diff = (long)*lhs - (long)*rhs;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    hashcollision_3968[0]++;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_buildFileTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CDVD_hashname_39A8(char *, int) __asm__("CDVD_hashname");
extern void qsort_39A8(char *, int, int, void *) __asm__("qsort");
extern char *strcpy_39A8(char *, char *) __asm__("strcpy");
extern void fi_cmp_39A8(void) __asm__("fi_cmp");
extern int FILESYS_cdSettings_39A8[] __asm__("FILESYS_cdSettings");
extern int hashcollision_39A8[] __asm__("hashcollision");

/**
 * Builds the sorted CD file table from the raw directory list and returns the hash seed it settled on.
 *
 * Each 0x108-byte source entry (path, sector, size) becomes {hash of the path (CDVD_hashname), sector, size, then the
 * path when FILESYS_cdSettings +0x4, the name length, is above 0} in entries of name length + 0xC bytes, sorted by hash
 * (qsort with fi_cmp). On any equal hashes the seed becomes seed * 101 + 7 and it starts over. Quirks:
 * two identical paths hash alike for every seed, so the loop never ends; the path is copied with no length limit.
 *
 * @param src  the directory list
 * @param dst  the file table
 * @param n  the number of files
 * @param seed  the first hash seed
 * @return the seed used
 */
int CDVD_buildFileTable(char *src, char *dst, int n, int seed) {
    int done = 0;
    char *dstEntry;
    do {
        char *srcEntry;
        dstEntry = dst;
        srcEntry = src;
        if (n > 0) {
            int i = n;
            do {
                int sz;
                *(int *)dstEntry = CDVD_hashname_39A8(srcEntry, seed);
                *(int *)(dstEntry + 4) = *(int *)(srcEntry + 0x100);
                *(int *)(dstEntry + 8) = *(int *)(srcEntry + 0x104);
                if (FILESYS_cdSettings_39A8[1] > 0) {
                    strcpy_39A8(dstEntry + 0xC, srcEntry);
                }
                sz = FILESYS_cdSettings_39A8[1];
                i--;
                srcEntry += 0x108;
                dstEntry += sz + 0xC;
            } while (i != 0);
        }
        hashcollision_39A8[0] = 0;
        qsort_39A8(dst, n, FILESYS_cdSettings_39A8[1] + 0xC, fi_cmp_39A8);
        if (hashcollision_39A8[0] != 0) {
            seed = seed * 101 + 7;
        } else {
            done = 1;
        }
    } while (!done);
    return seed;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_wakeReader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CDVD_state_3AD8[] __asm__("CDVD_state");
void iSignalSema_3AD8(int) __asm__("iSignalSema");
/**
 * CD callback: wakes the CD reader thread (CDVD_readerThread) by signalling its semaphore (CDVD_state +0xC).
 */
void CDVD_wakeReader(void) {
    iSignalSema_3AD8(CDVD_state_3AD8[3]);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_issueStepRead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int CDVD_state_3B00[] __asm__("CDVD_state");
extern int sceCdRead_3B00(int, int, int, int *) __asm__("sceCdRead");
extern void sceCdSync_3B00(int) __asm__("sceCdSync");
/**
 * Issues the CD read of the current step: sectors +0x30 from sector +0x2C into the sector buffer (+0x3C) when flag bit
 * 0 is set, else straight into the destination (+0x34). Up to three tries, waiting and syncing between them; it sets
 * flag 2 (reading) on success or flags 2 and 4 (failed, for the recovery in CDVD_checkFailedRead) otherwise.
 */
void CDVD_issueStepRead(void) {
    int *readState = &CDVD_state_3B00[8];
    int *cdState = CDVD_state_3B00;
    int readBuf;
    int ok;
    int i;
    if (cdState[0] & 1) {
        readBuf = cdState[15];
    } else {
        readBuf = readState[5];
    }
    ok = 0;
    for (i = 0; i < 3; i++) {
        if (sceCdRead_3B00(readState[3], readState[4], readBuf, &CDVD_state_3B00[7]) != 0) {
            CDVD_state_3B00[0] |= 2;
            ok = 1;
            break;
        }
        CDVD_sleepHsyncs(i + 1);
        sceCdSync_3B00(0);
    }
    if (!ok) {
        CDVD_state_3B00[0] |= 6;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_readerThread);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* Field names: CDVD_initFileSystem's CdState_4040 view of the same record where they agree (names ours). */
struct sC40Mode {                   /* the CD read mode handed to sceCdRead */
    unsigned char f00;
    unsigned char f01;
    unsigned char f02;
    unsigned char f03;
};
struct sC40Read {                   /* the read in progress */
    int stepBytes;                  /* bytes in this step */
    int restBytes;                  /* bytes left */
    int ofs;                        /* offset of the data in the sector buffer */
    int lba;                        /* sector */
    int nsect;                      /* sectors in this step */
    int buf;                        /* destination */
    char *files;                    /* the file table (also D_00519C78) */
    int *sector;                    /* sector buffer (D_00519C80) */
};
struct sC40 {                       /* 0x40, CDVD_state: the CD file system */
    int flags;                      /* flags */
    int count;                      /* number of files */
    int tid;                        /* reader thread */
    int sema;                       /* semaphore (also D_00519C4C) */
    int retries;                    /* read retries left (CDVD_issueStepRead restarts the read) */
    int cached;                     /* the sector in the sector buffer */
    int sorted;
    struct sC40Mode mode;
    struct sC40Read read;
};
extern struct sC40 CDVD_state;
extern void CDVD_issueStepRead_3BE0(void) __asm__("CDVD_issueStepRead");
extern int sceCdGetError_3618(void) __asm__("sceCdGetError");
extern void MEM_copy_2190(int, int, int) __asm__("MEM_copy");
extern void iFILESYS_CommandCompleteCallback_33B0(int) __asm__("iFILESYS_CommandCompleteCallback");

void CDVD_readerThread_impl(void) __asm__("CDVD_readerThread");
/**
 * CD reader thread: waits on the semaphore for each finished CD command and moves the current disc read on.
 *
 * On a CD error (or flag 8) a read in progress is retried while its retry count (+0x10) lasts, else flagged as failed
 * (4). After a step read into the sector buffer (flag bit 0), the wanted bytes are copied to the destination and that
 * sector is remembered (+0x14). While bytes are left (+0x24) it plans the next step (a part sector or an unaligned
 * destination goes through the sector buffer one sector at a time, otherwise whole sectors go straight in) and issues
 * it (CDVD_issueStepRead); when none are left it clears the reading flag and completes the command
 * (iFILESYS_CommandCompleteCallback(1)). Never returns.
 */
void CDVD_readerThread_impl(void) {
    struct sC40Read *curRead = &CDVD_state.read;

    while (1) {
        int fl;
        WaitSema(CDVD_state.sema);
        if (sceCdGetError_3618() != 0 || (CDVD_state.flags & 8)) {
            fl = CDVD_state.flags & ~8;
            CDVD_state.flags = fl;
            if (fl & 2) {
                if (--CDVD_state.retries > 0) {
                    CDVD_issueStepRead_3BE0();
                } else {
                    CDVD_state.flags = fl | 4;
                }
            }
            continue;
        }
        if (CDVD_state.flags & 1) {
            CDVD_state.flags &= ~1;
            CDVD_state.cached = curRead->lba;
            MEM_copy_2190(curRead->buf, (int)CDVD_state.read.sector + curRead->ofs, curRead->stepBytes);
        }
        if (curRead->restBytes > 0) {
            curRead->buf += curRead->stepBytes;
            curRead->ofs = 0;
            curRead->lba += curRead->nsect;
            if (curRead->restBytes < 0x800) {
                curRead->stepBytes = curRead->restBytes;
                CDVD_state.flags |= 1;
                curRead->nsect = 1;
            } else if ((curRead->buf & 0x3F) != 0) {
                curRead->stepBytes = 0x800;
                CDVD_state.flags |= 1;
                curRead->nsect = 1;
            } else {
                int k = curRead->restBytes & -0x800;
                CDVD_state.flags &= ~1;
                curRead->nsect = k >> 11;
                curRead->stepBytes = k;
            }
            curRead->restBytes -= curRead->stepBytes;
            CDVD_issueStepRead_3BE0();
        } else {
            CDVD_state.flags &= ~2;
            iFILESYS_CommandCompleteCallback_33B0(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_recoverRead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MEM_copy_3D78(void *, void *, int) __asm__("MEM_copy");
extern int sceCdCallback_3D78(void *) __asm__("sceCdCallback");
extern void sceCdInitEeCB_3D78(int, void *, int) __asm__("sceCdInitEeCB");
extern int sceCdSync_3D78(int) __asm__("sceCdSync");
extern int sceCdInit_3D78(int) __asm__("sceCdInit");
extern void sceCdDiskReady_3D78(int) __asm__("sceCdDiskReady");
extern int sceCdRead_3D78(int, int, void *, void *) __asm__("sceCdRead");
extern int sceCdGetError_3D78(void) __asm__("sceCdGetError");
extern void sceCdMmode_3D78(int) __asm__("sceCdMmode");
extern void SignalSema_3D78(int) __asm__("SignalSema");

struct CdReq_3D78 { int left; int f4; int f8; int lba; int count; char *buf; };
struct CdState_3D78 {
    int flags;
    int f4;
    int f8;
    int fC;
    char pad10[0x1C - 0x10];
    char mode[4];
    struct CdReq_3D78 req;
    char pad38[0x3C - 0x38];
    char *buf;
};

extern struct CdState_3D78 CDVD_state_3D78 __asm__("CDVD_state");
extern int FILESYS_cdSettings[];
extern char D_0051A480[];

/**
 * Recovers a failed disc read synchronously. Removes the CD callback, then, until no error is left: initialises the CD
 * again (shutting it down when that fails), sets the media mode, waits (480 ticks per attempt so far), test-reads
 * sector 0x10 and reads the current step's sectors one at a time. Then it restarts the CD callback thread, installs
 * CDVD_wakeReader again and signals the reader so the read carries on. Returns 0.
 *
 * @return 0
 */
int CDVD_recoverRead(void) {
    struct CdReq_3D78 req;
    int tries;
    int busy;

    tries = 0;
    sceCdCallback_3D78(0);
    MEM_copy_3D78(&req, &CDVD_state_3D78.req, 0x18);
    do {
        tries++;
        while (sceCdInit_3D78(0) == 0) {
            CDVD_sleepHsyncs(7200);
            while (sceCdInit_3D78(5) == 0) {
                CDVD_sleepHsyncs(7200);
            }
        }
        sceCdMmode_3D78(FILESYS_cdSettings[2]);
        CDVD_sleepHsyncs(tries * 0x1E0);
        while (sceCdRead_3D78(0x10, 1, CDVD_state_3D78.buf, CDVD_state_3D78.mode) == 0) {
            CDVD_sleepHsyncs(tries * 0x1E0);
        }
        do {
            busy = sceCdSync_3D78(1);
            CDVD_sleepHsyncs(tries * 0x1E0);
        } while (busy != 0);
        if (sceCdGetError_3D78() == 0 && req.count > 0) {
            do {
                sceCdDiskReady_3D78(0);
                while (sceCdRead_3D78(req.lba, 1, (CDVD_state_3D78.flags & 1) ? CDVD_state_3D78.buf : req.buf, CDVD_state_3D78.mode) == 0) {
                    CDVD_sleepHsyncs(tries * 0x1E0);
                }
                do {
                    busy = sceCdSync_3D78(1);
                    CDVD_sleepHsyncs(tries * 0x1E0);
                } while (busy != 0);
                if (sceCdGetError_3D78() != 0) {
                    CDVD_sleepHsyncs(7200);
                    break;
                }
                req.lba++;
                req.count--;
                req.buf += 0x800;
                req.left -= 0x800;
            } while (req.count > 0);
        }
    } while (sceCdGetError_3D78() != 0);
    sceCdInitEeCB_3D78(11, D_0051A480, 0x800);
    sceCdCallback_3D78(CDVD_wakeReader);
    SignalSema_3D78(CDVD_state_3D78.fC);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_checkFailedRead);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CDVD_state_4000[] __asm__("CDVD_state");
void CDVD_recoverRead_4000(void) __asm__("CDVD_recoverRead");

/**
 * Sync task of the CD file system: when a read has failed (flags 2 and 4 both set) it clears them and recovers the read
 * (CDVD_recoverRead). Returns 0.
 *
 * @return 0
 */
int CDVD_checkFailedRead(void) {
    int cdFlags = CDVD_state_4000[0];
    if ((cdFlags & 6) == 6) {
        CDVD_state_4000[0] = cdFlags & ~6;
        CDVD_recoverRead_4000();
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_initFileSystem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MEM_fill_4040(void *, int, int) __asm__("MEM_fill");
extern int sceCdInit_4040(int) __asm__("sceCdInit");
extern void sceCdMmode_4040(int) __asm__("sceCdMmode");
extern int sceCdDiskReady_4040(int) __asm__("sceCdDiskReady");
extern int sceCdGetDiskType_4040(void) __asm__("sceCdGetDiskType");
extern int sceCdGetToc_4040(void *) __asm__("sceCdGetToc");
extern void FlushCache_4040(int) __asm__("FlushCache");
extern void MEM_clear_4040(void *, int) __asm__("MEM_clear");
extern char *strcat_4040(char *, char *) __asm__("strcat");
extern int strncmp_4040(char *, char *, int) __asm__("strncmp");
extern int CDVD_buildFileTable_4040(void *, void *, unsigned int, int) __asm__("CDVD_buildFileTable");
extern int CreateSema_4040(void *) __asm__("CreateSema");
extern int CreateThread_4040(void *) __asm__("CreateThread");
extern int StartThread_4040(int, int) __asm__("StartThread");
extern void sceCdInitEeCB_4040(int, void *, int) __asm__("sceCdInitEeCB");
extern int sceCdCallback_4040(void *) __asm__("sceCdCallback");
extern void CDVD_readerThread_4040(void) __asm__("CDVD_readerThread");
extern void CDVD_wakeReader_4040(void) __asm__("CDVD_wakeReader");
extern void CDVD_checkFailedRead_4040(void) __asm__("CDVD_checkFailedRead");
extern int strlen(char *);

/* ISO 9660 directory record (byte-packed) */
struct IsoDirRec_4040 {
    signed char len;
    signed char ext_len;
    unsigned int lba;
    unsigned int lba_be;
    unsigned int size;
    unsigned int size_be;
    char date[7];
    unsigned char flags;
    char unit_size;
    char gap;
    short vol;
    short vol_be;
    signed char name_len;
    char name[1];
} __attribute__((packed));

struct IsoPvd_4040 {
    char pad[0x9C];
    struct IsoDirRec_4040 root;
} __attribute__((packed));

struct IsoEntry_4040 { char path[0x100]; unsigned int lba; unsigned int size; };
struct EmbFile_4040 { char *name; unsigned int lba; unsigned int size; char *path; };
struct Alloc_4040 {
    int f0;
    void *(*alloc)(char *, int, int);
    void (*free)(void *);
    char pad[0x20 - 0xC];
    struct EmbFile_4040 *files;
};
struct CdCfg_4040 { char *root; int namelen; int mmode; };
struct CdState_4040 {
    int flags;
    unsigned int count;
    int tid;
    int sema;
    int state;
    int cached;
    int sorted;
    unsigned char mode[4];
    int left;
    int rest;
    int ofs;
    int lba;
    int nsect;
    char *buf;
    void *files;
    char *sector;
};
struct ThreadParam_4040 { int status; void (*func)(void); void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr; unsigned int option; };
struct SemaParam_4040 { int count; int max_count; int init_count; int wait_threads; unsigned int attr; unsigned int option; };

extern struct CdState_4040 CDVD_state_4040 __asm__("CDVD_state");
extern struct CdCfg_4040 FILESYS_cdSettings_4040 __asm__("FILESYS_cdSettings");
extern struct IsoEntry_4040 *D_00450C68;
extern unsigned int D_00450C6C;
extern int D_00450C70;
extern char str_Named_Dir[];
extern char str_slash_c[];
extern char D_00519C80[];
extern char D_0051A480[];
extern char D_0051AC80[];
extern char D_0051EC80[];
extern int D_004A30F0;
extern struct IsoDirRec_4040 *CDVD_readCurrentSector_4040(void) __asm__("CDVD_readCurrentSector");
extern void func_003E3758_4040(unsigned int, unsigned int) __asm__("func_003E3758");

/**
 * Starts the CD file system and builds its file table; returns the number of files, or 0.
 *
 * Fails (0) when the CD does not initialise or the disc is not a PS2 CD or DVD (disc types 18, 20). It reads the disc's
 * TOC (sceCdGetToc), sets the read mode and sector 0x10 and lists every file into a temporary table (n * 264 bytes,
 * gFileSysOpts allocator): from the built-in file list (gFileSysOpts +0x20; the 7-character device prefix and the
 * 2-character version suffix are dropped and backslashes become slashes), or by walking the ISO 9660 directories
 * (func_003E3758) from the root or from the root directory FILESYS_cdSettings names. More than @p n files fails. Otherwise it
 * builds the sorted hash table into @p files (CDVD_buildFileTable, seed D_00450C70), creates the semaphore and the reader
 * thread CDVD_readerThread (priority 12), starts the CD callback thread, installs CDVD_wakeReader and adds the sync task
 * CDVD_checkFailedRead.
 *
 * @param n  the table capacity
 * @param files  the file table
 * @return the number of files, or 0
 */
unsigned int CDVD_initFileSystem(unsigned int n, void *files) {
    struct ThreadParam_4040 tp;
    struct SemaParam_4040 sp;
    char toc[0x1000];
    char path[0x100];
    char name[0x10];
    int ok;
    struct EmbFile_4040 *builtinFile;
    struct IsoEntry_4040 *dirEntry;
    char *pathChar;
    struct IsoPvd_4040 *pvd;
    struct IsoDirRec_4040 *rec;

    ok = 0;
    MEM_fill_4040(&CDVD_state_4040, 0, 0x40);
    MEM_fill_4040(files, 0, (FILESYS_cdSettings_4040.namelen + 0xC) * n);
    if (sceCdInit_4040(0) == 0) {
        return 0;
    }
    sceCdMmode_4040(FILESYS_cdSettings_4040.mmode);
    sceCdDiskReady_4040(0);
    if (sceCdGetDiskType_4040() == 18 || sceCdGetDiskType_4040() == 20) {
        ok = 1;
    }
    if (!ok) {
        return 0;
    }
    sceCdDiskReady_4040(0);
    sceCdGetToc_4040(toc);
    CDVD_state_4040.mode[1] = 1;
    CDVD_state_4040.lba = 0x10;
    CDVD_state_4040.mode[0] = 0;
    CDVD_state_4040.mode[2] = 0;
    FlushCache_4040(0);
    D_00450C68 = gFileSysOpts.malloc(str_Named_Dir, n * 264, 0);
    MEM_clear_4040(D_00450C68, n * 264);
    if ((struct EmbFile_4040*)gFileSysOpts.builtinFiles != 0) {
        builtinFile = (struct EmbFile_4040*)gFileSysOpts.builtinFiles;
        dirEntry = D_00450C68;
        while (*builtinFile->name != 0) {
            strcpy(path, builtinFile->path + 7);
            for (pathChar = path; *pathChar != 0; pathChar++) {
                if (*pathChar == '\\') {
                    *pathChar = '/';
                }
            }
            strcpy(name, builtinFile->name);
            name[strlen(builtinFile->name) - 2] = 0;
            strcat_4040(path, name);
            strcpy(dirEntry->path, path);
            dirEntry->lba = builtinFile->lba;
            dirEntry->size = builtinFile->size;
            D_00450C6C++;
            dirEntry++;
            builtinFile++;
        }
    } else {
        pvd = (struct IsoPvd_4040 *)CDVD_readCurrentSector_4040();
        D_0051EC80[1] = 0;
        D_0051EC80[0] = '/';
        if (*FILESYS_cdSettings_4040.root != 0) {
            CDVD_state_4040.lba = pvd->root.lba;
            for (rec = CDVD_readCurrentSector_4040(); (char *)rec <= D_00519C80 + 0x800; rec = (struct IsoDirRec_4040 *)((char *)rec + rec->len)) {
                if (strncmp_4040(rec->name, FILESYS_cdSettings_4040.root, rec->name_len) == 0) {
                    strcat_4040(D_0051EC80, FILESYS_cdSettings_4040.root);
                    strcat_4040(D_0051EC80, str_slash_c);
                    func_003E3758_4040(rec->lba, (rec->size + 0x7FF) >> 11);
                    break;
                }
            }
        } else {
            func_003E3758_4040(pvd->root.lba, (pvd->root.size + 0x7FF) >> 11);
        }
    }
    if (n < D_00450C6C) {
        ((void (*)(void*))gFileSysOpts.free)(D_00450C68);
        D_00450C68 = 0;
        D_00450C6C = 0;
        return 0;
    }
    CDVD_state_4040.files = files;
    CDVD_state_4040.count = D_00450C6C;
    CDVD_state_4040.sorted = CDVD_buildFileTable_4040(D_00450C68, files, D_00450C6C, D_00450C70);
    ((void (*)(void*))gFileSysOpts.free)(D_00450C68);
    D_00450C6C = 0;
    sp.init_count = 0;
    sp.max_count = 0x20;
    D_00450C68 = 0;
    CDVD_state_4040.sema = CreateSema_4040(&sp);
    tp.func = CDVD_readerThread_4040;
    tp.stack = D_0051AC80;
    tp.gpReg = &D_004A30F0;
    tp.stackSize = 0x4000;
    tp.initPriority = 0xC;
    tp.option = 0;
    CDVD_state_4040.tid = CreateThread_4040(&tp);
    StartThread_4040(CDVD_state_4040.tid, 0);
    sceCdInitEeCB_4040(11, D_0051A480, 0x800);
    sceCdCallback_4040(CDVD_wakeReader_4040);
    CDVD_state_4040.sector = D_00519C80;
    SYNCTASK_add(CDVD_checkFailedRead_4040, 0, 0);
    return CDVD_state_4040.count;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_shutdownFileSystem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SYNCTASK_del_44B0(void (*)(void)) __asm__("SYNCTASK_del");
extern void sceCdCallback(int);
extern void sceCdInit(int);
extern void DeleteThread(int);
extern void TerminateThread(int);
extern void DeleteSema(int);
extern int CDVD_state_9C40[] __asm__("CDVD_state");
extern void CDVD_checkFailedRead_4000(void) __asm__("CDVD_checkFailedRead");

/**
 * Shuts the CD file system down: removes its sync task (CDVD_checkFailedRead) and the CD callback, terminates and deletes the
 * reader thread, deletes its semaphore and shuts the CD down (sceCdInit(5)).
 */
void CDVD_shutdownFileSystem(void) {
    SYNCTASK_del_44B0(CDVD_checkFailedRead_4000);
    sceCdCallback(0);
    TerminateThread(CDVD_state_9C40[2]);
    DeleteThread(CDVD_state_9C40[2]);
    DeleteSema(CDVD_state_9C40[3]);
    sceCdInit(5);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_findFileNumber);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
#include "fakematch.h"
extern int CDVD_hashname_4508(char *, int) __asm__("CDVD_hashname");
extern unsigned int bsearch_4508(int *, int, int, int, void *) __asm__("bsearch");
extern void fi_cmp_4508(void) __asm__("fi_cmp");
extern int FILESYS_cdSettings_4508[] __asm__("FILESYS_cdSettings");
extern char D_00499C81_4508[] __asm__("D_00499C81");
extern int CDVD_state_4508[] __asm__("CDVD_state");

int CDVD_findFileNumber_4508(char *path, int unused, int *out) __asm__("CDVD_findFileNumber");
/**
 * Looks a path up in the disc's hashed file table and gives its 1-based entry number.
 *
 * Builds a normalised copy of @p path in a 256-byte buffer: exactly one leading '/', every '\\' turned into '/', and
 * lower-case letters upper-cased (flag 2 of the ctype table D_00499C81). Hashes it with CDVD_hashname (seed word 6 of
 * the file-system state CDVD_state) and binary-searches (bsearch, called bsearch-style) the entry table (word 14,
 * D_00519C78) of word 1 entries, each D_00450C5C + 12 bytes, comparing the hashes with fi_cmp. Used by
 * FILEDEV_open, which tags the number with device 1 in the top byte, and by CDVD_findFileEntry.
 *
 * @param path  the file path ('/' or '\\' separated)
 * @param modeFlags  the open-mode flags FILEDEV_open passes (not used)
 * @param out  receives the 1-based entry number, or 0 when not found
 * @return 1 when found, 0 otherwise
 */
// FAKEMATCH: register pin `register int slash FM_REG("$2") = '/'; buf[0] = slash;` (fakematch.h) breaks the
// local-alloc tie between the '/' constant for buf[0] and the first path char c0 (retail: v0 / v1, ours swapped); plain form 99.62%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int CDVD_findFileNumber_4508(char *path, int modeFlags, int *out) {
    char buf[256];
    int key;
    char *bufCursor;
    char *pathCursor;
    unsigned int foundEntry;
    char pathChar;
    int upperChar;
    pathCursor = path;
    {
        char firstChar = *pathCursor++;
        if (firstChar != '/') pathCursor = path;
    }
    {
        register int slash FM_REG("$2") = '/';
        buf[0] = slash;
    }
    bufCursor = buf + 1;
    while (*pathCursor != 0) {
        pathChar = *pathCursor;
        if (pathChar == '\\') {
            *bufCursor = '/';
        } else {
            upperChar = pathChar - 0x20;
            if (!(D_00499C81_4508[pathChar] & 2)) upperChar = pathChar;
            *bufCursor = upperChar;
        }
        pathCursor++;
        bufCursor++;
    }
    *bufCursor = 0;
    key = CDVD_hashname_4508(buf, CDVD_state_4508[6]);
    foundEntry = bsearch_4508(&key, CDVD_state_4508[14], CDVD_state_4508[1], FILESYS_cdSettings_4508[1] + 0xC, fi_cmp_4508);
    if (foundEntry == 0) {
        *out = 0;
        return 0;
    }
    *out = (foundEntry - CDVD_state_4508[14]) / (unsigned)(FILESYS_cdSettings_4508[1] + 0xC) + 1;
    return 1;
}
#endif

INCLUDE_ASM("ealib/seg_2DCAE8", func_003E4648);

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_readFileEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450C5C_48D8[] __asm__("D_00450C5C");
extern char *D_00519C78_48D8[] __asm__("D_00519C78");
extern char *strncpy(char *, const char *, int);

int CDVD_readFileEntry_48D8(int, char *, int *, int *) __asm__("CDVD_readFileEntry");

/**
 * Reads one entry of the disc file table by its 1-based number.
 *
 * Entries sit in the table D_00519C78, each D_00450C5C + 12 bytes: +4 the start sector, +8 the size, then the stored
 * name (D_00450C5C bytes, copied with strncpy). Every output pointer may be 0. FILEDEV_getsize calls it for the size of
 * an open disc file (device 1) and FILEDEV_getstartsector for its start sector.
 *
 * @param fileIndex  the 1-based entry number
 * @param nameOut  receives the stored name, or 0
 * @param sizeOut  receives the size (+8), or 0
 * @param sectorOut  receives the start sector (+4), or 0
 * @return the entry's size (+8)
 */
int CDVD_readFileEntry_48D8(int fileIndex, char *nameOut, int *sizeOut, int *sectorOut) {
    char *entry;
    entry = D_00519C78_48D8[0] + (fileIndex - 1) * (D_00450C5C_48D8[0] + 0xC);
    if (nameOut != 0) {
        strncpy(nameOut, entry + 0xC, D_00450C5C_48D8[0]);
    }
    if (sizeOut != 0) {
        *sizeOut = *(int *)(entry + 8);
    }
    if (sectorOut != 0) {
        *sectorOut = *(int *)(entry + 4);
    }
    return *(int *)(entry + 8);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", CDVD_findFileEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CDVD_findFileNumber(int, int, int *);
extern int D_00450C5C_0C5C[] __asm__("D_00450C5C");
extern int D_00519C78_9C78[] __asm__("D_00519C78");

/**
 * Returns the disc file-table entry for a path, or 0 when the table has none.
 *
 * Finds the entry number with CDVD_findFileNumber and turns it into a pointer into D_00519C78 (entries of D_00450C5C + 12
 * bytes).
 *
 * @param filePath  the file path
 * @return the entry (start sector at +4, size at +8, name at +0xC), or 0
 */
int CDVD_findFileEntry(int filePath) {
    int entryNumber;
    int entry;

    if (CDVD_findFileNumber(filePath, 0, &entryNumber) == 0) {
        entry = 0;
    } else {
        entry = D_00519C78_9C78[0] + (entryNumber - 1) * (D_00450C5C_0C5C[0] + 0xC);
    }
    return entry;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", REAL_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*exitfunctions_0C78[])(void) __asm__("exitfunctions");

/**
 * Runs and clears every registered exit handler.
 *
 * Walks the 64-slot handler table (exitfunctions, filled by REAL_addexit) from the last slot to the first,
 * calling each non-null entry and then clearing its slot. REAL_exit() calls it before trapping.
 */
void REAL_restore(void) {
    int i;
    for (i = 0x3F; i >= 0; i--) {
        void (*fn)(void) = exitfunctions_0C78[i];
        if (fn != 0) {
            fn();
        }
        exitfunctions_0C78[i] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", REAL_exit);
#ifdef SKIP_ASM
extern void REAL_restore(void);

/**
 * Calls REAL_restore() and then stops at a break trap (code 0xFFFF) instead of returning.
 */
void REAL_exit(void) {
    REAL_restore();
    __asm__ volatile("break 0xFFFF");  // PORT: trap
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", REAL_addexit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int exitfunctions_4A30[] __asm__("exitfunctions");

/**
 * Registers a routine to run at exit, once.
 *
 * Does nothing when @p exitFunc is already in the 64-entry exit table exitfunctions (the routines REAL_restore runs); otherwise
 * stores it in the first empty slot, and drops it silently when the table is full. The timer setup TIMER_init
 * registers its teardown TIMER_restore here; the sound library (System_VectorToReal5) registers one too.
 *
 * @param exitFunc  the routine to call at exit
 */
void REAL_addexit(int exitFunc) {
    int i;
    for (i = 0; i < 0x40; i++) {
        if (exitfunctions_4A30[i] == exitFunc) return;
    }
    i = 0;
    while (i < 0x40) {
        if (exitfunctions_4A30[i] == 0) {
            exitfunctions_4A30[i] = exitFunc;
            break;
        }
        i++;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", REAL_removeexit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void (*exitfunctions[64])(void);     /* exit handlers (REAL_restore runs them) */
/**
 * Removes a routine from the exit table.
 *
 * Clears the first slot of the 64-entry table exitfunctions (the routines REAL_restore runs) that holds @p exitFunc; does nothing
 * when it isn't there. The counterpart of REAL_addexit. Nothing in the build calls it.
 *
 * @param exitFunc  the routine to remove
 */
void REAL_removeexit(void (*exitFunc)(void)) {
    int i;
    for (i = 0; i < 0x40; i++) {
        if (exitfunctions[i] == exitFunc) {
            exitfunctions[i] = 0;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void TIMER_setHblankRate_4AF0(int) __asm__("TIMER_setHblankRate");
extern int AddIntcHandler_4AF0(int, void (*)(void), int) __asm__("AddIntcHandler");
extern int EnableIntc_4AF0(int) __asm__("EnableIntc");
extern void REAL_addexit_4AF0(void (*)(void)) __asm__("REAL_addexit");
extern void tmrint_4AF0(void) __asm__("tmrint");
extern void TIMER_restore_4AF0(void) __asm__("TIMER_restore");

extern int TIMER_hblankRate;
extern int TIMERhz;
/* tick counter, incremented by the timer interrupt handler (tmrint) */
extern volatile int TIMER_ticks;
extern int hTimerId;
extern unsigned short D_00450DE0;

#define T0_COUNT_4AF0 (*(volatile int *)0x10000000)
#define T0_MODE_4AF0 (*(volatile int *)0x10000010)
#define T1_COUNT_4AF0 (*(volatile int *)0x10000800)
#define T1_MODE_4AF0 (*(volatile int *)0x10000810)
#define T1_COMP_4AF0 (*(volatile int *)0x10000820)

/**
 * Starts the library tick timer at a requested rate and returns the rate it actually runs at.
 *
 * When the interrupt handler is already installed (hTimerId) it only returns TIMERhz. A @p hz of 0 means 100. It
 * picks EE timer 1's clock for the rate (bus clock / 256 below 196 Hz, / 16 below 3150 Hz, / 1 above: 0x8CA00, 0x8CA000
 * or 0x8CA0000 counts a second), stores the compare period in D_00450DE0 and the resulting rate in TIMERhz, programs
 * T1 (count 0, the compare value, the clock mode | 0x180: compare interrupt and count enable), installs tmrint
 * as the INTC 10 (timer 1) handler (AddIntcHandler) and enables it (EnableIntc). While the H-blank rate TIMER_hblankRate is
 * still 0 it calibrates the video mode: it counts H-blanks on timer 0 (mode 0x83) over one second of ticks and passes 1
 * (15701-15799, NTSC), 2 (15576-15674, PAL) or 3 (28801-31999) to TIMER_setHblankRate. Finally it registers the teardown
 * TIMER_restore with REAL_addexit. systemInit calls it with 60.
 *
 * Likely bug: after the calibration both saved timer-0 values (the count and the mode) are written back to T0_COUNT
 * (0x10000000), so timer 0's mode stays 0x83.
 *
 * @param hz  the wanted tick rate in Hz (0 means 100)
 * @return the tick rate in effect (TIMERhz)
 */
int TIMER_init(int hz) {
    int mode;
    int count;
    int tmode;
    int end;
    int hblankCount;

    if (hTimerId != 0) {
        return TIMERhz;
    }
    if (hz == 0) {
        hz = 100;
    }
    if (hz < 3150) {
        if (hz < 196) {
            D_00450DE0 = (hz / 2 + 0x8CA00) / hz;
            TIMERhz = 0x8CA00 / D_00450DE0;
            mode = 2;
        } else {
            D_00450DE0 = (hz / 2 + 0x8CA000) / hz;
            TIMERhz = 0x8CA000 / D_00450DE0;
            mode = 1;
        }
    } else {
        D_00450DE0 = (hz / 2 + 0x8CA0000) / hz;
        TIMERhz = 0x8CA0000 / D_00450DE0;
        mode = 0;
    }
    T1_COUNT_4AF0 = 0;
    T1_COMP_4AF0 = D_00450DE0;
    T1_MODE_4AF0 = mode | 0x180;
    hTimerId = AddIntcHandler_4AF0(10, tmrint_4AF0, 0);
    EnableIntc_4AF0(10);
    if (TIMER_hblankRate == 0) {
        count = T0_COUNT_4AF0;
        tmode = T0_MODE_4AF0;
        T0_MODE_4AF0 = 131;
        end = TIMER_ticks + TIMERhz;
        if (TIMER_ticks < end) {
            do {
            } while (TIMER_ticks < end);
        }
        hblankCount = T0_COUNT_4AF0 - count;
        T0_COUNT_4AF0 = count;
        T0_COUNT_4AF0 = tmode;
        if (hblankCount < 0) {
            hblankCount += 0x10000;
        }
        if ((unsigned int)(hblankCount - 15701) < 99) {
            TIMER_setHblankRate_4AF0(1);
        } else if ((unsigned int)(hblankCount - 15576) < 99) {
            TIMER_setHblankRate_4AF0(2);
        } else if ((unsigned int)(hblankCount - 28801) < 3199) {
            TIMER_setHblankRate_4AF0(3);
        }
    }
    REAL_addexit_4AF0(TIMER_restore_4AF0);
    return TIMERhz;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void RemoveIntcHandler(int, int);
extern void DisableIntc(int);
extern int hTimerId_0DDC[] __asm__("hTimerId");

/**
 * Stops the library tick timer.
 *
 * When a handler is installed (hTimerId) it clears T1_MODE (0x10000810), disables INTC 10 with DisableIntc, removes
 * the handler with RemoveIntcHandler and clears hTimerId. TIMER_init registers it as an exit routine.
 */
void TIMER_restore(void) {
    volatile int *reg;

    if (hTimerId_0DDC[0] != 0) {
        reg = (volatile int *)0x10000810;
        *reg = 0;
        DisableIntc(10);
        RemoveIntcHandler(10, hTimerId_0DDC[0]);
        hTimerId_0DDC[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", tmrint);
#ifdef SKIP_ASM
// PORT: PS2-only (EE timer 1 registers, inline asm).
/* tmrint: the EE timer 1 compare interrupt (inittmr.c; On Tour's PSP tmrint has the same tick/callback body). */
extern unsigned short D_00450DE0;               /* compare period */
extern volatile int TIMER_ticks;                 /* ticks */
extern volatile int libticks;                 /* libticks */
extern int bInTimerSubroutine;                          /* bInTimerSubroutine */
extern void (*tmrsub[8])(void);             /* tmrsub */

#define T1_COUNT (*(volatile int *)0x10000800)
#define T1_MODE (*(volatile int *)0x10000810)
#define T1_COMP (*(volatile int *)0x10000820)

/**
 * Handles the EE timer 1 compare interrupt: one library tick.
 *
 * Moves the compare value on by one period (D_00450DE0) from where the count stands, resets T1_COUNT and acknowledges
 * the interrupt (T1_MODE | 0x400), increments the tick counters TIMER_ticks and libticks, then calls each of the 8
 * timer routines in tmrsub that is set, with bInTimerSubroutine set to 1 around the call (the thread and mutex code checks
 * it to pick the interrupt-safe kernel calls). Re-enables interrupts (sync.l, ei) before returning. Installed by
 * TIMER_init.
 *
 * @return 0
 */
int tmrint(void) {
    int i;

    T1_COMP = D_00450DE0 + T1_COMP - T1_COUNT;
    T1_COUNT = 0;
    T1_MODE |= 0x400;
    TIMER_ticks++;
    libticks++;
    for (i = 0; i < 8; i++) {
        if (tmrsub[i]) {
            bInTimerSubroutine = 1;
            tmrsub[i]();
            bInTimerSubroutine = 0;
        }
    }
    __asm__ volatile("sync.l\n\tei");
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_gettick);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern volatile int TIMER_ticks;

/**
 * Returns the library tick counter.
 *
 * TIMER_ticks, incremented by the timer interrupt tmrint at the rate TIMER_init set (TIMER_getfrequency returns
 * it). Used for timeouts throughout the library, the sound code and the quadtree loader.
 *
 * @return the current tick count
 */
int TIMER_gettick(void)
{
    return TIMER_ticks;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_elapsedticks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMER_lastElapsedTick_4EA8[] __asm__("TIMER_lastElapsedTick");

/**
 * Returns the ticks elapsed since the previous call.
 *
 * Stores the current tick count (TIMER_gettick) in TIMER_lastElapsedTick and returns it minus the value stored last time.
 *
 * @return ticks since the last call (since the reset in TIMER_reset on the first call)
 */
int TIMER_elapsedticks(void) {
    int old = TIMER_lastElapsedTick_4EA8[0];
    int now = TIMER_gettick();
    TIMER_lastElapsedTick_4EA8[0] = now;
    return now - old;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_reset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMER_deadline_4EE8[] __asm__("TIMER_deadline");
extern int TIMER_lastElapsedTick_4EE8[] __asm__("TIMER_lastElapsedTick");
extern volatile int TIMER_ticks_4EE8[] __asm__("TIMER_ticks");

/**
 * Resets the tick bookkeeping to zero.
 *
 * Clears the last-call mark TIMER_lastElapsedTick (TIMER_elapsedticks), the tick counter TIMER_ticks and the deadline TIMER_deadline
 * (TIMER_settimeout / TIMER_testtimeout).
 */
// Resets the timer state.
void TIMER_reset(void) {
    int *deadlinePtr = TIMER_deadline_4EE8;
    TIMER_lastElapsedTick_4EE8[0] = 0;
    TIMER_ticks_4EE8[0] = 0;
    deadlinePtr[0] = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_settimeout);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int TIMER_gettick_4F08(void) __asm__("TIMER_gettick");
extern int TIMER_deadline_4F08[] __asm__("TIMER_deadline");

/**
 * Sets the tick deadline a number of ticks from now.
 *
 * Stores the current tick count (TIMER_gettick) plus @p delayTicks in TIMER_deadline; TIMER_testtimeout tests it and TIMER_waittimeout
 * waits for it.
 *
 * @param delayTicks  ticks from now
 */
void TIMER_settimeout(int delayTicks) {
    int *deadlinePtr = TIMER_deadline_4F08;
    *deadlinePtr = TIMER_gettick_4F08() + delayTicks;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_waittimeout);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMER_testtimeout();
extern void SYNCTASK_run_4F40(int) __asm__("SYNCTASK_run");
extern void THREAD_yield_4F40(int) __asm__("THREAD_yield");

/**
 * Waits for the tick deadline set by TIMER_settimeout.
 *
 * Until TIMER_testtimeout reports it reached, runs the sync tasks (SYNCTASK_run(0)) and yields the thread
 * (THREAD_yield(0)) on each pass.
 */
void TIMER_waittimeout(void) {
    while (TIMER_testtimeout() == 0) {
        SYNCTASK_run_4F40(0);
        THREAD_yield_4F40(0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_testtimeout);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMER_gettick();
extern int TIMER_deadline_4F80[] __asm__("TIMER_deadline");

/**
 * Tells whether the tick deadline has been reached.
 *
 * Compares the current tick count (TIMER_gettick) with the deadline TIMER_deadline set by TIMER_settimeout.
 *
 * @return 1 when the deadline is now or past, 0 before it
 */
int TIMER_testtimeout(void) {
    int sinceDeadline = TIMER_gettick() - TIMER_deadline_4F80[0]; if (sinceDeadline > -1) return 1; return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_waitticks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SYNCTASK_run_4FB0(int) __asm__("SYNCTASK_run");
void THREAD_yield_4FB0(int) __asm__("THREAD_yield");

/**
 * Waits a number of library ticks, running the sync tasks meanwhile.
 *
 * Until the tick counter (TIMER_gettick) reaches the start plus @p ticks it calls SYNCTASK_run(0) and yields the thread
 * with THREAD_yield(0). cAsyncSys_ASYNCSYS_DeInit waits 2 ticks this way.
 *
 * @param ticks  the number of ticks to wait
 */
void TIMER_waitticks(int ticks) {
    int end = TIMER_gettick() + ticks;
    while (TIMER_gettick() - end < 0) {
        SYNCTASK_run_4FB0(0);
        THREAD_yield_4FB0(0);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_getfrequency);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMERhz_5008[4] __asm__("TIMERhz");

/**
 * Returns the library tick rate in ticks per second.
 *
 * TIMERhz, as set by TIMER_init. THREAD_yieldticks and the millisecond timers in seg_2AEDC0 divide by it to
 * convert ticks.
 *
 * @return ticks per second
 */
int TIMER_getfrequency(void) {
    return TIMERhz_5008[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int GetThreadId_5018(void) __asm__("GetThreadId");
extern void ChangeThreadPriority_5018(int, int) __asm__("ChangeThreadPriority");
extern void MUTEX_create_5018(void *) __asm__("MUTEX_create");
extern int D_00450DF8_5018[] __asm__("D_00450DF8");
extern int D_0051ED88_5018[] __asm__("D_0051ED88");

/**
 * Sets up the thread layer for the main thread.
 *
 * Records the calling thread in the main-thread record D_00450DF8 (signature 0x50533254 'PS2T', then the id from
 * GetThreadId), sets that thread's priority to 100 and creates the library mutex D_0051ED88 with MUTEX_create. Called
 * by systemInit.
 */
void THREAD_init(void) {
    int mainThreadId = GetThreadId_5018();
    D_00450DF8_5018[0] = 0x50533254;
    D_00450DF8_5018[1] = mainThreadId;
    ChangeThreadPriority_5018(mainThreadId, 100);
    MUTEX_create_5018(D_0051ED88_5018);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MUTEX_destroy(void *);
extern void MEM_clear_5068(int *, int) __asm__("MEM_clear");
extern int D_00450DF8_5068[] __asm__("D_00450DF8");
extern int D_0051ED88_5068[] __asm__("D_0051ED88");

/**
 * Shuts the thread layer down.
 *
 * Destroys the library mutex D_0051ED88 (MUTEX_destroy) and clears the 8-byte main-thread record D_00450DF8 that
 * THREAD_init filled. Nothing in the build calls it.
 */
void THREAD_restore(void) {
    MUTEX_destroy(D_0051ED88_5068);
    MEM_clear_5068(D_00450DF8_5068, 8);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_create);
#ifdef SKIP_ASM
extern void THREAD_createparam(int, int, int, int, int, int);

/**
 * Creates and starts a thread without a start parameter.
 *
 * Calls THREAD_createparam with the same arguments and a start parameter of 0.
 *
 * @param thread  the thread record to fill
 * @param entryProc  the thread's entry routine
 * @param stackAddr  the stack base
 * @param stackSize  the stack size in bytes
 * @param priority  the priority code (97-103, anything else means 100)
 */
void THREAD_create(int thread, int entryProc, int stackAddr, int stackSize, int priority) {
    THREAD_createparam(thread, entryProc, 0, stackAddr, stackSize, priority);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_createparam);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CreateThread(void *);
extern void StartThread(int, int);
extern int D_004A30F0;

struct sReq_50C8 {
    int w0;
    int a;
    int b;
    int c;
    int *tbl;
    int kind;
    int more[4];
};

int THREAD_createparam_a(int *self, int arg1, int arg2, int arg3, int arg4, int arg5) __asm__("THREAD_createparam");
/**
 * Creates a kernel thread and starts it with a parameter.
 *
 * Fills a ThreadParam on the stack (entry @p entryProc, stack @p stackAddr / @p stackSize, gp D_004A30F0, initial priority @p priority
 * when it is 97, 98, 99, 101, 102 or 103, else 100) and calls CreateThread. The id goes to the record's +4 even on
 * failure; on success the record's +0 gets the signature 0x50533254 ('PS2T') and the thread is started with StartThread
 * and @p startParam. THREAD_create calls it with a parameter of 0; the quadtree loader (gameLink_createThread) starts its thread
 * here.
 *
 * @param entryProc  the entry routine
 * @param startParam  the parameter StartThread passes to it
 * @param stackAddr  the stack base
 * @param stackSize  the stack size in bytes
 * @param priority  the priority code (97-103; anything else means 100)
 * @return 1 when the thread was created and started, 0 otherwise
 */
int THREAD_createparam_a(int *self, int entryProc, int startParam, int stackAddr, int stackSize, int priority) {
    struct sReq_50C8 req;
    int threadId;
    int kind;

    switch (priority) {
    case 97:
        kind = 97;
        break;
    case 98:
        kind = 98;
        break;
    case 99:
        kind = 99;
        break;
    case 101:
        kind = 101;
        break;
    case 102:
        kind = 102;
        break;
    case 103:
        kind = 103;
        break;
    default:
        kind = 100;
        break;
    }
    req.a = entryProc;
    req.b = stackAddr;
    req.c = stackSize;
    req.kind = kind;
    req.tbl = &D_004A30F0;
    threadId = CreateThread(&req);
    self[1] = threadId;
    if (threadId != 0) {
        self[0] = 0x50533254;
        StartThread(threadId, startParam);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void iTerminateThread_51A0(int) __asm__("iTerminateThread");
extern int bInTimerSubroutine_51A0[] __asm__("bInTimerSubroutine");

void THREAD_destroy_51A0(THREAD *thread) __asm__("THREAD_destroy");
/**
 * Ends and deletes a thread.
 *
 * Takes the thread id from the record (threadId), or the calling thread's when @p thread is -1, and reads its status
 * (iReferThreadStatus inside the timer interrupt, flag bInTimerSubroutine, else ReferThreadStatus). A thread that is running,
 * ready, waiting or suspended (status & 0xF) is terminated (iTerminateThread / TerminateThread) and deleted; a dormant
 * one (status & 0x10) is only deleted.
 *
 * @param thread  the thread record, or -1 for the calling thread
 */
void THREAD_destroy_51A0(THREAD *thread) {
    int threadId;
    int st[12];

    if (thread == (THREAD *)0xFFFFFFFF) {
        threadId = GetThreadId();
    } else {
        threadId = thread->threadId;
    }
    if (bInTimerSubroutine_51A0[0] != 0) {
        iReferThreadStatus(threadId, st);
    } else {
        ReferThreadStatus(threadId, st);
    }
    if (st[0] & 0xF) {
        if (bInTimerSubroutine_51A0[0] != 0) {
            iTerminateThread_51A0(threadId);
        } else {
            TerminateThread(threadId);
        }
        DeleteThread(threadId);
        return;
    }
    if (st[0] & 0x10) {
        DeleteThread(threadId);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_remove);
#ifdef SKIP_ASM
extern void THREAD_destroy();

/**
 * Destroys a thread through THREAD_destroy.
 *
 * Calls THREAD_destroy without loading an argument, so whatever the caller left in the first argument register is passed
 * through (the C declares no parameter). Nothing in the build calls it.
 */
void THREAD_remove(void) {
    THREAD_destroy();
}
#endif

/**
 * Does nothing: an empty function that returns at once. Nothing in the build references it.
 */
void func_003E5288(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_wakeOnAlarm);
#ifdef SKIP_ASM
extern void iWakeupThread(int);

/**
 * Kernel alarm handler that wakes the thread THREAD_yieldticks put to sleep.
 *
 * THREAD_yieldticks sets an alarm with this handler and its own thread id as the argument, then sleeps; on expiry this
 * passes that id (@p threadId) to iWakeupThread.
 *
 * @param alarmId  the alarm id (unused)
 * @param alarmTime  the alarm time (unused)
 * @param threadId  the argument given to SetAlarm: the sleeping thread's id
 */
void THREAD_wakeOnAlarm(int alarmId, int alarmTime, int threadId) {
    iWakeupThread(threadId);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_yieldticks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void abm_YT(char *) __asm__("REAL_abortmessage");
extern int f5008_YT(void) __asm__("TIMER_getfrequency");
extern int f3B20_YT(int, char *, int) __asm__("SetAlarm");
extern void f3C50_YT(int) __asm__("RotateThreadReadyQueue");
extern int f3C90_YT(void) __asm__("GetThreadId");
extern void f3CA0_YT(int, int *) __asm__("ReferThreadStatus");
extern void f3CB0_YT(int, int *) __asm__("iReferThreadStatus");
extern void f3CC0_YT(void) __asm__("SleepThread");
extern void f4CE8_YT(int) __asm__("iRotateThreadReadyQueue");
extern int D_0DA0_YT[] __asm__("TIMER_hblankRate");
extern int D_0DE4_YT[] __asm__("bInTimerSubroutine");
extern char D_5E20_YT[] __asm__("D_00495E20");
extern char fE5290_YT[] __asm__("THREAD_wakeOnAlarm");

/**
 * Puts the calling thread to sleep for a number of library timer ticks, or just yields when given none.
 *
 * For @p numTicks > 0 it converts the ticks to H-blank counts (a0 times the H-blank rate TIMER_hblankRate, divided by the
 * tick rate), sets a kernel alarm whose handler wakes this thread, and sleeps; it aborts with "ERROR SETTING
 * WAKEUP ALARM" if the alarm can't be set. Otherwise it rotates the ready queue at the thread's current
 * priority (through iRotateThreadReadyQueue inside a timer handler).
 *
 * @param numTicks  ticks to sleep, or 0 or less to yield
 */
void THREAD_yieldticks(int numTicks) {
    int buf[12];
    int scaledTicks;
    if (numTicks > 0) {
        scaledTicks = ((numTicks * D_0DA0_YT[0]) / f5008_YT()) & 0xFFFF;
        if (f3B20_YT(scaledTicks, fE5290_YT, f3C90_YT()) < 0) {
            abm_YT(D_5E20_YT);
        }
        f3CC0_YT();
        return;
    }
    if (D_0DE4_YT[0] != 0) {
        f3CB0_YT(f3C90_YT(), buf);
        f4CE8_YT(buf[6]);
        return;
    }
    f3CA0_YT(f3C90_YT(), buf);
    f3C50_YT(buf[6]);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_yield);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMERhz_5398 __asm__("TIMERhz");
extern int bInTimerSubroutine_5398 __asm__("bInTimerSubroutine");
extern void THREAD_yieldticks_5398(int) __asm__("THREAD_yieldticks");
extern int GetThreadId_5398(void) __asm__("GetThreadId");
extern void iReferThreadStatus_5398(int, void *) __asm__("iReferThreadStatus");
extern void iRotateThreadReadyQueue_5398(int) __asm__("iRotateThreadReadyQueue");
extern void ReferThreadStatus_5398(int, void *) __asm__("ReferThreadStatus");
extern void RotateThreadReadyQueue_5398(int) __asm__("RotateThreadReadyQueue");

void THREAD_yield_impl(int ms) __asm__("THREAD_yield");
/**
 * Yields the calling thread, for a number of milliseconds or just once.
 *
 * With @p ms nonzero it converts the milliseconds to ticks (ms * TIMERhz / 1000, at least 1) and sleeps with
 * THREAD_yieldticks. With 0 it rotates the ready queue at the thread's current priority (inside the timer interrupt,
 * flag bInTimerSubroutine, with iReferThreadStatus and iRotateThreadReadyQueue; else ReferThreadStatus and RotateThreadReadyQueue),
 * letting other threads of that priority run. Loading and streaming loops call it with 0 while they wait.
 *
 * @param ms  milliseconds to sleep, or 0 to yield once
 */
void THREAD_yield_impl(int ms) {
    int info[12];
    if (ms != 0) {
        int yieldTicks = ms * TIMERhz_5398 / 1000;
        if (yieldTicks <= 0) {
            yieldTicks = 1;
        }
        THREAD_yieldticks_5398(yieldTicks);
    } else if (bInTimerSubroutine_5398 != 0) {
        iReferThreadStatus_5398(GetThreadId_5398(), info);
        iRotateThreadReadyQueue_5398(info[6]);
    } else {
        ReferThreadStatus_5398(GetThreadId_5398(), info);
        RotateThreadReadyQueue_5398(info[6]);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_iscurrent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00450DFC_0DFC[] __asm__("D_00450DFC");

/**
 * Tells whether a thread is the calling thread.
 *
 * For a null @p thread it compares GetThreadId with the main thread's id (D_00450DFC, set by THREAD_init); -1 always
 * matches; otherwise it compares with the record's threadId. The file and stream code uses it with 0 to run the sync
 * tasks only on the main thread.
 *
 * @param thread  the thread record, 0 for the main thread, or -1 for the calling thread
 * @return 1 when it is the calling thread, 0 otherwise
 */
int THREAD_iscurrent(THREAD *thread) {
    int cur = GetThreadId();
    if (thread == 0) {
        return cur == D_00450DFC_0DFC[0];
    }
    if (thread == (THREAD *)-1) {
        return 1;
    }
    return cur == thread->threadId;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_getpriority);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int GetThreadId();
extern void ReferThreadStatus(int, void *);
extern void iReferThreadStatus(int, void *);
extern int bInTimerSubroutine_5498[] __asm__("bInTimerSubroutine");
extern int D_00450DFC_5498[] __asm__("D_00450DFC");

/**
 * Returns a thread's current priority.
 *
 * Picks the id like THREAD_setpriority (0: main thread D_00450DFC, -1: calling thread, else the record's +4) and reads the
 * current-priority word (6) of its status (iReferThreadStatus inside the timer interrupt, else ReferThreadStatus).
 * Nothing in the build calls it.
 *
 * @param thread  the thread record, 0 for the main thread, or -1 for the calling thread
 * @return the thread's current priority
 */
int THREAD_getpriority(int *thread) {
    int sp[12];
    int threadId;
    if (thread == 0) {
        threadId = D_00450DFC_5498[0];
    } else if (thread == (int *)-1) {
        threadId = GetThreadId();
    } else {
        threadId = thread[1];
    }
    if (bInTimerSubroutine_5498[0] != 0) {
        iReferThreadStatus(threadId, sp);
    } else {
        ReferThreadStatus(threadId, sp);
    }
    return sp[6];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_setpriority);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void ChangeThreadPriority_5508(int, int) __asm__("ChangeThreadPriority");
extern void iChangeThreadPriority_5508(int, int) __asm__("iChangeThreadPriority");
extern int GetThreadId_5508(void) __asm__("GetThreadId");
extern int bInTimerSubroutine_5508[] __asm__("bInTimerSubroutine");
extern int D_00450DFC_5508[] __asm__("D_00450DFC");

/**
 * Changes a thread's priority.
 *
 * Picks the id (0: main thread D_00450DFC, -1: calling thread, else the record's threadId) and calls
 * iChangeThreadPriority inside the timer interrupt (flag bInTimerSubroutine), ChangeThreadPriority otherwise.
 *
 * @param thread  the thread record, 0 for the main thread, or -1 for the calling thread
 * @param priority  the new kernel priority
 * @return 1
 */
int THREAD_setpriority(THREAD *thread, int priority) {
    int var_4;

    if (thread == 0) {
        var_4 = D_00450DFC_5508[0];
    } else if (thread == (THREAD *)0xFFFFFFFF) {
        var_4 = GetThreadId_5508();
    } else {
        var_4 = thread->threadId;
    }
    if (bInTimerSubroutine_5508[0] != 0) {
        iChangeThreadPriority_5508(var_4, priority);
    } else {
        ChangeThreadPriority_5508(var_4, priority);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_testexit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void ReferThreadStatus(int, void *);
extern void iReferThreadStatus(int, void *);
extern int bInTimerSubroutine_0DE4[] __asm__("bInTimerSubroutine");
/* THREAD: ealib/threads.h */

/**
 * Tells whether a thread has stopped running.
 *
 * Reads the status of the record's thread (threadId; iReferThreadStatus inside the timer interrupt, else
 * ReferThreadStatus).
 * Nothing in the build calls it.
 *
 * @param thread  the thread record
 * @return 1 when the thread is dormant (status bit 0x10) or the status is 0, else 0
 */
int THREAD_testexit(THREAD *thread) {
    struct { int v; char rest[44]; } st;
    int finished;

    if (bInTimerSubroutine_0DE4[0] != 0) {
        iReferThreadStatus(thread->threadId, &st);
    } else {
        ReferThreadStatus(thread->threadId, &st);
    }
    finished = 0;
    if ((st.v & 0x10) || st.v == 0) {
        finished = 1;
    }
    return finished;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_waitexit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void THREAD_yieldticks_55E0(int) __asm__("THREAD_yieldticks");
extern int TIMER_gettick_55E0() __asm__("TIMER_gettick");
extern void ReferThreadStatus_55E0(int, int *) __asm__("ReferThreadStatus");

/**
 * Waits for a thread to finish.
 *
 * Polls the record's thread (+4) with ReferThreadStatus and stops when it is dormant (bit 0x10) or gone (status 0),
 * yielding with THREAD_yieldticks(0) between polls. A @p timeout of 0 waits for ever.
 *
 * Likely bug: with a nonzero timeout the loop only continues while the deadline (start tick + @p timeout) is already
 * past (deadline < now), so before the deadline it gives up after one yield.
 *
 * @param task  the thread record
 * @param timeout  ticks to wait, or 0 for no limit
 * @return 1 when the thread ended up dormant, 0 otherwise
 */
int THREAD_waitexit(int *task, int timeout) {
    int sp[12];
    int threadId;
    int deadline;

    deadline = TIMER_gettick_55E0() + timeout;
    threadId = task[1];
    do {
        ReferThreadStatus_55E0(threadId, sp);
        if ((sp[0] & 0x10) || sp[0] == 0) break;
        THREAD_yieldticks_55E0(0);
    } while (timeout == 0 || deadline < TIMER_gettick_55E0());
    return sp[0] == 0x10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", THREAD_exit);
#ifdef SKIP_ASM
extern void ExitDeleteThread();

/**
 * Ends the calling thread and deletes it (ExitDeleteThread). Nothing in the build calls it.
 */
void THREAD_exit(void) {
    ExitDeleteThread();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MUTEX_create);
#ifdef SKIP_ASM
/* the Sony EE kernel's semaphore parameters (eekernel.h) */
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
extern int CreateSema(struct SemaParam *);

/**
 * Creates a recursive mutex: no owner, count 0, and a binary semaphore (CreateSema, max and initial count 1).
 *
 * @param mutex  the mutex (a MUTEX handle; PS2MUTEX layout)
 * @return 0
 */
int MUTEX_create(void *mutex) {
    PS2MUTEX *pmutex = mutex;
    struct SemaParam optParam;
    pmutex->lockcount = 0;
    pmutex->lockthreadid = 0;
    optParam.maxCount = 1;
    optParam.initCount = 1;
    pmutex->mutex = CreateSema(&optParam);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MUTEX_destroy);
#ifdef SKIP_ASM
extern void DeleteSema(int);

/**
 * Destroys a mutex by deleting its kernel semaphore (DeleteSema on PS2MUTEX::mutex).
 *
 * Used for the library mutex (THREAD_restore), memory classes (MEMCLASS_remove), the file system, async files, streams,
 * the quadtree loader and cCommSystem's destructor.
 *
 * @param mutex  the mutex
 */
void MUTEX_destroy(void *mutex) {
    PS2MUTEX *pmutex = mutex;
    DeleteSema(pmutex->mutex);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MUTEX_lock);
#ifdef SKIP_ASM
extern int GetThreadId();
extern void WaitSema(int);

/**
 * Locks a recursive mutex: a thread that already owns it only bumps the count, others wait on its semaphore.
 *
 * @param mutex  the mutex (On Tour MUTEX*; PS2MUTEX layout)
 */
void MUTEX_lock(void *mutex) {
    PS2MUTEX *pmutex = mutex;
    if (pmutex->lockthreadid == GetThreadId()) {
        pmutex->lockcount = pmutex->lockcount + 1;
        return;
    }
    WaitSema(pmutex->mutex);
    pmutex->lockthreadid = GetThreadId();
    pmutex->lockcount = pmutex->lockcount + 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MUTEX_unlock);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int GetThreadId();
extern void SignalSema(int);
extern void iSignalSema(int);
extern int bInTimerSubroutine_5760[] __asm__("bInTimerSubroutine");

void MUTEX_unlock_5760(void *arg0) __asm__("MUTEX_unlock");
/**
 * Releases one level of a recursive EA mutex.
 *
 * When the calling thread owns it, the lock count drops by one and only a count of 0 clears the owner and
 * signals the semaphore. A caller that does not own it signals the semaphore anyway. Inside a timer handler
 * the interrupt-safe signal is used.
 *
 * @param mutex  the mutex (PS2MUTEX layout: lockthreadid, lockcount, the semaphore)
 */
void MUTEX_unlock_5760(void *mutex) {
    PS2MUTEX *pmutex = mutex;
    if (pmutex->lockthreadid == GetThreadId()) {
        if (--pmutex->lockcount != 0) return;
        pmutex->lockthreadid = 0;
    }
    if (bInTimerSubroutine_5760[0] != 0) {
        iSignalSema(pmutex->mutex);
    } else {
        SignalSema(pmutex->mutex);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SYNCTASK_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char systemtasksubs_57D0[] __asm__("systemtasksubs");
void MEM_fill_57D0(char *, int, int) __asm__("MEM_fill");
/**
 * Clears the synchronous task table.
 *
 * Zeroes all 16 entries (256 bytes) of systemtasksubs, the tasks SYNCTASK_run() calls.
 */
void SYNCTASK_init(void) {
    MEM_fill_57D0(systemtasksubs_57D0, 0, 0x100);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SYNCTASK_add);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct T_STA { int fn; int mode; int due; int fC; };
extern struct T_STA systemtasksubs_STA[16] __asm__("systemtasksubs");
extern int reentry_STA __asm__("reentry");
extern volatile int libticks_STA __asm__("libticks");

void SYNCTASK_add_impl(int fn, int mode, int delay) __asm__("SYNCTASK_add");
/**
 * Schedules a function in the synchronous task table, or reschedules it if it is already there.
 *
 * Reuses the function's existing slot, otherwise takes a free one; a full table drops the request. A reentrant
 * call skips one free slot per nesting level, probably so nested adds don't pick the same slot. @p mode becomes
 * the period between runs, with -1 mapped to 0 and 0 mapped to 1.
 *
 * @param fn     task function, called as fn(arg, lateness) by SYNCTASK_run()
 * @param mode   period in library ticks (-1 means 0, 0 means 1)
 * @param delay  ticks from now until the first run
 */
void SYNCTASK_add_impl(int fn, int mode, int delay) {
    int i;
    int idx;
    int skip;

    idx = -1;
    if (mode == -1) {
        mode = 0;
    } else if (mode == 0) {
        mode = 1;
    }
    skip = reentry_STA++;
    for (i = 0; i < 16; i++) {
        if (systemtasksubs_STA[i].fn == fn) {
            idx = i;
        } else if (systemtasksubs_STA[i].fn == 0 && idx == -1) {
            if (skip != 0) {
                skip--;
            } else {
                idx = i;
            }
        }
    }
    if (idx != -1) {
        systemtasksubs_STA[idx].fn = fn;
        systemtasksubs_STA[idx].mode = mode;
        systemtasksubs_STA[idx].due = libticks_STA + delay;
        systemtasksubs_STA[idx].fC = 0;
    }
    reentry_STA--;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SYNCTASK_del);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int fn; int b; int c; int d; } STsk_5924;
extern STsk_5924 systemtasksubs_ED98[] __asm__("systemtasksubs");

/**
 * Removes a function from the synchronous task table.
 *
 * Clears the first of the 16 slots whose function is @p taskFunc; does nothing if none is.
 *
 * @param taskFunc  task function to remove
 */
void SYNCTASK_del(int taskFunc) {
    int i = 0;

    while (i < 16 && systemtasksubs_ED98[i].fn != taskFunc) i++;
    if (i < 16 && systemtasksubs_ED98[i].fn == taskFunc) systemtasksubs_ED98[i].fn = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SYNCTASK_run);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSyncTask {                  /* 16 bytes, systemtasksubs[16]: a task SYNCTASK_run calls when it is due */
    int (*func)(int, int);
    int rate;                        /* period */
    int next;                        /* due time (libticks) */
    int exec;                        /* running */
};
extern struct sSyncTask systemtasksubs[16];
extern int SYNCTASK_runTick_5928 __asm__("SYNCTASK_runTick");

/**
 * Runs every synchronous task that is due.
 *
 * Records the current library tick in SYNCTASK_runTick. Each task whose due time has passed and that isn't
 * already running is marked running, called with (@p mode, ticks late), and rescheduled one period after now.
 *
 * @param mode  value passed to every task
 * @return the OR of the results of the tasks that ran
 */
int SYNCTASK_run(int mode) {
    int i;
    int result = 0;

    SYNCTASK_runTick_5928 = libticks_STA;
    for (i = 0; i < 16; i++) {
        int (*fn)(int, int) = systemtasksubs[i].func;
        if (fn != 0 && libticks_STA >= systemtasksubs[i].next && systemtasksubs[i].exec == 0) {
            systemtasksubs[i].exec = 1;
            result |= fn(mode, libticks_STA - systemtasksubs[i].next);
            systemtasksubs[i].exec = 0;
            systemtasksubs[i].next = libticks_STA + systemtasksubs[i].rate;
        }
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", abortablewait);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SYNCTASK_run_5A10(int) __asm__("SYNCTASK_run");

/**
 * Runs the sync tasks for up to a number of ticks or until one reports something.
 *
 * Calls SYNCTASK_run(0) repeatedly while the tick counter (TIMER_gettick) is below the start plus @p ticks and the last
 * result was 0. Nothing in the build calls it.
 *
 * @param ticks  the time limit in ticks
 * @return the last SYNCTASK_run result (0 when the time ran out first)
 */
int abortablewait(int ticks) {
    int deadline;
    int result;

    result = 0;
    deadline = TIMER_gettick() + ticks;
    while (TIMER_gettick() < deadline && result == 0) {
        result = SYNCTASK_run_5A10(0);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_movedevice);
#ifdef SKIP_ASM


/**
 * Switches a debug print device on or off.
 *
 * Copies bit 0 of @p enable into bit 0 of the device's flags (PRINTdevicelist entry +0x8).
 *
 * @param device  the device index
 * @param enable  nonzero to enable
 */
void PRINT_movedevice(int device, int enable) {
    PRINTDEVICEINFO* temp_4;

    temp_4 = &PRINTdevicelist[device];
    temp_4->enabled = enable & 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_movechannel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Switches a debug print channel (or a group of them) on or off.
 *
 * @p channel 0 sets all 64 channels, 1 the channels whose group flag (real) is set, any other value that one channel.
 *
 * @param channel  the channel (0: all, 1: flagged group)
 * @param enable  nonzero to enable
 */
void PRINT_movechannel(int channel, int enable) {
    int i;
    if (channel == 0) {
        for (i = 0; i < 64; i++) {
            PRINTchannellist[i].enabled = (enable != 0);
        }
    } else if (channel == 1) {
        for (i = 0; i < 64; i++) {
            if (PRINTchannellist[i].real) {
                PRINTchannellist[i].enabled = (enable != 0);
            }
        }
    } else {
        PRINTchannellist[channel].enabled = (enable != 0);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int PRINTinitialized;                          /* debug output set up (PRINT_init) */
extern void PRINT_setchannelstate(int, int);
extern void PRINT_setdevicestate(int, int);

/**
 * Initialises the debug print devices and channels once.
 *
 * When PRINTinitialized is 0: sets it, switches the 8 devices and 64 channels off, clears the entries of devices
 * 4..7 and channels 14..63, then enables channel 2 and device 2 with PRINT_setchannelstate(2, 1) and
 * PRINT_setdevicestate(2, 1).
 */
void PRINT_init(void) {
    int i;
    if (PRINTinitialized == 0) {
        PRINTinitialized = 1;
        for (i = 0; i < 8; i++) {
            PRINTdevicelist[i].enabled = 0;
        }
        for (i = 0; i < 64; i++) {
            PRINTCHANNELINFO *channel = &PRINTchannellist[i];
            channel->enabled = 0;
        }
        for (i = 4; i < 8; i++) {
            PRINTdevicelist[i].name = 0;
            PRINTdevicelist[i].func = 0;
        }
        for (i = 14; i < 64; i++) {
            PRINTCHANNELINFO *channel = &PRINTchannellist[i];
            channel->name = 0;
            channel->real = 0;
        }
        PRINT_setchannelstate(2, 1);
        PRINT_setdevicestate(2, 1);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int PRINTinitialized_5C60[4] __asm__("PRINTinitialized");

/**
 * Marks the debug print system uninitialised.
 *
 * Clears PRINTinitialized when it is set, so the next print call runs PRINT_init again. Nothing in the build calls it.
 */
void PRINT_restore(void) {
    if (PRINTinitialized_5C60[0] != 0) {
        PRINTinitialized_5C60[0] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_vstring);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int PRINTinitialized_5C78[] __asm__("PRINTinitialized");

void PRINT_vstring_5C78(int, char *, char *) __asm__("PRINT_vstring");
/**
 * Formats a message into an 8 KB buffer and passes it to every enabled print handler, if the channel is
 * enabled.
 *
 * Sets the print system up on first use.
 *
 * @param idx  the print channel (On Tour PRINTCHANNEL channel)
 * @param fmt  printf-style format
 * @param ap  the arguments (va_list)
 */
void PRINT_vstring_5C78(int idx, char *fmt, char *ap) {
    char buf[0x2000];
    int i;
    if (PRINTinitialized_5C78[0] == 0) {
        PRINT_init();
    }
    if (PRINTchannellist[idx].enabled) {
        vsprintf(buf, fmt, ap);
        for (i = 0; i < 8; i++) {
            PRINTDEVICEINFO *device = &PRINTdevicelist[i];
            if (device->enabled) {
                if (device->func) {
                    device->func(idx, buf);
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_string);
#ifdef SKIP_ASM
extern void PRINT_vstring(char *, const char *, char *);

// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define PRINT_string_va_start(ap)                                        \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

/**
 * Formats a message and sends it to the debug print outputs.
 *
 * Passes its variable arguments to PRINT_vstring(), which formats into an 8 KB buffer and hands the text to
 * every enabled output when the channel is enabled. Despite its type, @p dst is that channel number: callers
 * pass 2 (SYSTEM_abortmessage, memstd).
 *
 * @param dst  debug channel number, declared as a char pointer
 * @param fmt  printf-style format; the arguments follow
 */
void PRINT_string(char *dst, const char *fmt, ...) {
    char *ap;
    PRINT_string_va_start(ap);
    PRINT_vstring(dst, fmt, ap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_string1);
#ifdef SKIP_ASM
extern void PRINT_vstring_5D78(int, int, char *) __asm__("PRINT_vstring");

// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define PRINT_string1_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

/**
 * Prints a printf-style message on print channel 2.
 *
 * @param format  the format (const char*, passed as int; On Tour format)
 */
void PRINT_string1(int format, ...) {
    char *ap;
    PRINT_string1_va_start(ap);
    PRINT_vstring_5D78(2, format, ap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_setdevicestate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5DC8 { int a; int b; int c; };
extern void PRINT_movedevice(int, int);
extern void PRINT_init(void);
extern int PRINTinitialized_5DC8[] __asm__("PRINTinitialized");
extern struct S_5DC8 PRINTdevicelist_5DC8[] __asm__("PRINTdevicelist");

/**
 * Enables or disables a debug print device, initialising printing first if needed.
 *
 * Only acts on a device that is present in PRINTdevicelist.
 *
 * @param device  the device index
 * @param enable  nonzero to enable
 */
void PRINT_setdevicestate(int device, int enable) {
    if (PRINTdevicelist_5DC8[device].b != 0) {
        if (PRINTinitialized_5DC8[0] == 0) {
            PRINT_init();
        }
        PRINT_movedevice(device, enable);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_getdevicestate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void PRINT_init(void);

extern int PRINTinitialized_1008[] __asm__("PRINTinitialized");

/**
 * Tells whether a debug print device is enabled.
 *
 * Runs PRINT_init first when printing is not set up, then returns bit 0 of the device's flags (PRINTdevicelist entry,
 * +8). Nothing in the build calls it.
 *
 * @param device  the device index (0-7)
 * @return 1 when the device is enabled, 0 otherwise
 */
int PRINT_getdevicestate(int device) {
    PRINTDEVICEINFO* entry;
    if (PRINTinitialized_1008[0] == 0) {
        PRINT_init();
    }
    entry = &PRINTdevicelist[device];
    return entry->enabled;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_setdevicename);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5E88 { int a; int b; int c; };
extern void PRINT_init(void);
extern struct S_5E88 PRINTdevicelist_5E88[] __asm__("PRINTdevicelist");
extern int PRINTinitialized_5E88[] __asm__("PRINTinitialized");

/**
 * Installs a debug print device: its name and its output routine.
 *
 * Runs PRINT_init first when printing is not set up, then stores @p deviceName and @p outputFunc in the device's PRINTdevicelist entry
 * (+0 and +4; device 2 is preset to "debugger" with PRINT_debuggerfunc). The device's enable flag is left as it was. Nothing
 * in the build calls it.
 *
 * @param device  the device index (0-7)
 * @param deviceName  the device name
 * @param outputFunc  the routine that receives (channel, text) for every message
 */
void PRINT_setdevicename(int device, int deviceName, int outputFunc) {
    if (PRINTinitialized_5E88[0] == 0) {
        PRINT_init();
    }
    PRINTdevicelist_5E88[device].a = deviceName;
    PRINTdevicelist_5E88[device].b = outputFunc;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_setchannelstate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void PRINT_movechannel(int, int);
extern void PRINT_init(void);
extern int PRINTinitialized_1008[] __asm__("PRINTinitialized");

/**
 * Enables or disables a debug print channel, initialising printing first if needed.
 *
 * @param channel  the channel (0: all, 1: flagged group)
 * @param enable  nonzero to enable
 */
void PRINT_setchannelstate(int channel, int enable) {
    if (PRINTinitialized_1008[0] == 0) {
        PRINT_init();
    }
    PRINT_movechannel(channel, enable);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_getchannelstate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_5F48 { int a; int b; };
extern struct P_5F48 PRINTchannellist_5F48[] __asm__("PRINTchannellist");
extern int PRINTinitialized_5F48[] __asm__("PRINTinitialized");
void PRINT_init_5F48(void) __asm__("PRINT_init");

/**
 * Tells whether a debug print channel is enabled.
 *
 * Runs PRINT_init first when printing is not set up, then returns bit 0 of the channel's flags (PRINTchannellist entry,
 * +4). Nothing in the build calls it.
 *
 * @param channel  the channel index (0-63)
 * @return 1 when the channel is enabled, 0 otherwise
 */
int PRINT_getchannelstate(int channel) {
    if (PRINTinitialized_5F48[0] == 0) {
        PRINT_init_5F48();
    }
    return (PRINTchannellist_5F48 + channel)->b & 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_setchannelname);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int PRINTchannellist_0E08[] __asm__("PRINTchannellist");
extern int PRINTinitialized_1008[] __asm__("PRINTinitialized");
void PRINT_init_5F98(void) __asm__("PRINT_init");

/**
 * Sets a debug print channel's name.
 *
 * Runs PRINT_init first when printing is not set up, then stores @p channelName in the channel's PRINTchannellist entry (+0;
 * the preset ones are "all", "real", "alert", "general" and so on). Nothing in the build calls it.
 *
 * @param channel  the channel index (0-63)
 * @param channelName  the channel name
 */
void PRINT_setchannelname(int channel, int channelName) {
    if (PRINTinitialized_1008[0] == 0) {
        PRINT_init_5F98();
    }
    PRINTchannellist_0E08[channel * 2] = channelName;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_todevice);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef struct { int a; void (*fn)(int, int); int c; } E_5FF0;
extern int PRINTinitialized_5FF0[] __asm__("PRINTinitialized");
extern E_5FF0 PRINTdevicelist_5FF0[] __asm__("PRINTdevicelist");
extern void PRINT_init_5FF0(void) __asm__("PRINT_init");

/**
 * Sends text straight to one debug print device.
 *
 * Runs PRINT_init first when printing is not set up, then calls the device's output routine (PRINTdevicelist entry +4)
 * with @p channel and @p text when it has one, whether or not the device or channel is enabled. Nothing in the build calls it.
 *
 * @param i  the device index (0-7)
 * @param channel  the channel number passed to the routine
 * @param text  the text
 */
void PRINT_todevice(int i, int channel, int text) {
    E_5FF0 *deviceEntry;
    if (PRINTinitialized_5FF0[0] == 0) PRINT_init_5FF0();
    if (PRINTdevicelist_5FF0[i].fn != 0) {
        deviceEntry = &PRINTdevicelist_5FF0[i];
        deviceEntry->fn(channel, text);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", REAL_abortmessage);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define REAL_abortmessage_va_start(ap)                                   \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern int vsprintf(char *dst, const char *fmt, char *ap);
extern void (*REALabortmessage_6070[])(char *, char *) __asm__("REALabortmessage");
extern char str_pct_s_b_6070[] __asm__("str_pct_s_b");

/**
 * Formats an abort message and hands it to the installed abort handler.
 *
 * Formats into a 512-byte buffer (empty for a null @p fmt) and calls the handler in REALabortmessage with "%s" and
 * the text. The default handler is SYSTEM_abortmessage(), which prints it and exits.
 *
 * @param fmt  printf-style format, or 0; the arguments follow
 */
void REAL_abortmessage(const char *fmt, ...) {
    char buf[0x200];
    if (fmt) {
        char *ap;
        REAL_abortmessage_va_start(ap);
        vsprintf(buf, fmt, ap);
    } else {
        buf[0] = 0;
    }
    REALabortmessage_6070[0](str_pct_s_b_6070, buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SYSTEM_abortmessage);
#ifdef SKIP_ASM
extern void REAL_exit(void);
extern void PRINT_string_60E8(int, char *, ...) __asm__("PRINT_string");
extern int vsprintf_60E8(char *, char *, char *) __asm__("vsprintf");
extern char str_ERROR_pct_s[];
extern char str_FILE_pct_s_LINE_pct_d[];
extern char *REALabortfilename_100C[] __asm__("REALabortfilename");
extern int REALabortlinenum_1010[] __asm__("REALabortlinenum");

// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define SYSTEM_abortmessage_va_start(ap)                                 \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

/**
 * Prints "ERROR: " and a formatted message, plus the recorded source file and line when set, then exits the
 * program.
 *
 * @param fmt  printf-style format, or 0 for no message
 */
void SYSTEM_abortmessage(char *fmt, ...) {
    char buf[512];
    if (fmt != 0) {
        char *ap;
        SYSTEM_abortmessage_va_start(ap);
        vsprintf_60E8(buf, fmt, ap);
    } else {
        buf[0] = 0;
    }
    PRINT_string_60E8(2, str_ERROR_pct_s, buf);
    if (REALabortfilename_100C[0] != 0) {
        PRINT_string_60E8(2, str_FILE_pct_s_LINE_pct_d, REALabortfilename_100C[0], REALabortlinenum_1010[0]);
    }
    REAL_exit();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_move);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MEM_copy_6188(unsigned char *, unsigned char *, int) __asm__("MEM_copy");
void MEM_move_6188(unsigned char *dst, unsigned char *src, int n) __asm__("MEM_move");

/**
 * Copies memory correctly even when the source and destination overlap.
 *
 * Uses MEM_copy() unless @p dst starts inside the source range above @p src; then it copies byte by byte from
 * the end.
 *
 * @param dst  destination
 * @param src  source
 * @param n    number of bytes
 */
void MEM_move_6188(unsigned char *dst, unsigned char *src, int n) {
    unsigned char *srcEnd;
    if (src >= dst || dst >= (srcEnd = src + n)) {
        MEM_copy_6188(dst, src, n);
    } else {
        dst += n;
        n--;
        src = srcEnd;
        while (n != -1) {
            src--;
            dst--;
            *dst = *src;
            n--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", blockmove);
#ifdef SKIP_ASM
extern void MEM_move(int, int);

/**
 * Copies memory with the source first: MEM_move with the two pointers swapped (bcopy order).
 *
 * The byte count is not touched: it passes through in the third argument register to MEM_move (the C declares only two
 * parameters). Nothing in the build calls it.
 *
 * @param src  the source
 * @param dst  the destination
 */
void blockmove(int src, int dst) {
    MEM_move(dst, src);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_clear);
#ifdef SKIP_ASM
extern void MEM_fill(int, int, int);

/**
 * Zeroes a block of memory.
 *
 * Calls MEM_fill(@p dest, 0, @p count).
 *
 * @param dest  the memory
 * @param count  the number of bytes
 */
void MEM_clear(int dest, int count) {
    MEM_fill(dest, 0, count);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEMSYS_validaddress);
#ifdef SKIP_ASM
/**
 * Tells whether an address lies in a region the EE can use.
 *
 * Valid: main RAM above the kernel (0x00100000-0x07FFFFFF), the register area 0x10000000-0x14000000, the uncached
 * (0x20100000-0x27FFFFFF) and uncached-accelerated (0x30100000-0x37FFFFFF) RAM mirrors, and the scratchpad
 * (0x70000000-0x70003FFF). memstd's checkfree uses it to vet free-list pointers.
 *
 * @param addr  the address
 * @return 1 when valid, 0 otherwise
 */
int MEMSYS_validaddress(unsigned int addr) {
    int valid = 0;
    if (addr - 0x100000 <= 0x7EFFFFF || addr - 0x10000000 <= 0x4000000 ||
        addr - 0x20100000 <= 0x7EFFFFF || addr - 0x30100000 <= 0x7EFFFFF ||
        addr - 0x70000000 < 0x4000) {
        valid = 1;
    }
    return valid;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", DEBUG_breakIfEnabled);
#ifdef SKIP_ASM
extern int DEBUG_breakEnabled[];

/**
 * Breaks into the debugger (break 6) when the debug-break switch DEBUG_breakEnabled is set; does nothing otherwise.
 *
 * Called on error paths in seg_2AEDC0 (RCMP_AV_PLAYER_Init, RCMP_AV_AUDIO_AV_AUDIO).
 */
void DEBUG_breakIfEnabled(void) {
    if (DEBUG_breakEnabled[0] != 0) {
        __asm__ volatile("break 6");  // PORT: trap
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_crc16);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short crctab_1018[] __asm__("crctab");

/**
 * Computes a table-driven CRC-16 of a block of bytes.
 *
 * For each byte: crc = crctab[(crc ^ byte) & 0xFF] ^ (crc >> 8), starting from @p seed. Every caller uses seed
 * 64490 (0xFBEA): the save-game and profile checks (cBESaveInterface, cBE_writeToData), the trophy-room memory card
 * read and memstd's block checks.
 *
 * @param data  the data
 * @param byteCount  the number of bytes
 * @param seed  the starting value
 * @return the CRC (16 bits)
 */
int MEM_crc16(unsigned char *data, int byteCount, int seed) {
    unsigned char *cursor;
    unsigned int crc;
    unsigned char *end;
    cursor = data;
    crc = seed & 0xFFFF;
    end = cursor + byteCount;
    if (cursor < end) {
        do {
            unsigned char dataByte = *cursor;
            cursor += 1;
            crc = crctab_1018[(dataByte ^ crc) & 0xFF] ^ (crc >> 8);
        } while (cursor < end);
    }
    return crc & 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", SORT_shell);
#ifdef SKIP_ASM
extern int inctable[];

/**
 * Shell-sorts an array of keys in descending order, moving a second array along with it.
 *
 * Starts at the largest gap in the table inctable (the same as BXSORT_shell's) below @p n and works down to gap 1,
 * inserting each key / value pair among the earlier ones with larger or equal keys. cAI_UpdateRankingPlacement uses it
 * to order the riders.
 *
 * @param n  the number of entries
 * @param keys  the keys, sorted in place (largest first)
 * @param vals  the values moved with their keys
 */
void SORT_shell(int n, int *keys, int *vals) {
    int k = 1;
    int gap;
    int i;
    while (inctable[k] < n) {
        k++;
    }
    for (;;) {
        gap = inctable[--k];
        for (i = gap; i < n; i++) {
            int key = keys[i];
            int val = vals[i];
            int j = i - gap;
            do {
                if (keys[j] >= key) {
                    break;
                }
                keys[j + gap] = keys[j];
                vals[j + gap] = vals[j];
                j -= gap;
            } while (j >= 0);
            keys[j + gap] = key;
            vals[j + gap] = val;
        }
        if (gap < 2) {
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PRINT_debuggerfunc);
#ifdef SKIP_ASM
extern void printf(int);

/**
 * Output routine of the "debugger" print device: passes the text to printf.
 *
 * PRINTdevicelist presets device 2 with the name "debugger" and this routine; PRINT_vstring calls it for every message
 * on an enabled channel while the device is enabled.
 *
 * @param channel  the print channel (unused)
 * @param text  the text
 */
void PRINT_debuggerfunc(int channel, int text) {
    printf(text);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_fill);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void MEM_fill_memset(char *p, int c, int n) __asm__("MEM_fill");
/**
 * Fills memory with a value, using the widest aligned stores it can (64-bit for blocks of 64 bytes).
 *
 * The value is stored as a 32-bit pattern, not repeated per byte: only values whose bytes are all equal (such
 * as
 * 0) give a uniform byte fill.
 *
 * @param dest  start of the memory (On Tour dest)
 * @param fillValue  the fill value (On Tour unsigned int val)
 * @param n  number of bytes
 */
void MEM_fill_memset(char *dest, int fillValue, int n) {
    // ORIGINAL-PTR: a memset-style fill of untyped memory: 1-, 2-, 4- and 8-byte stores through the advancing byte
    // pointer (On Tour's MEM_fill keeps no typed pointer locals, only `val64` / `val32[2]`).
    if (n > 0 && ((int)dest & 1)) {
        *dest = fillValue;
        n--;
        dest++;
    }
    if (n >= 2 && ((int)dest & 2)) {
        *(short *)dest = fillValue;
        n -= 2;
        dest += 2;
    }
    if (n >= 4 && ((int)dest & 4)) {
        *(int *)dest = fillValue;
        n -= 4;
        dest += 4;
    }
    if (n >= 64) {
        long fillWide;
        int *fillHalves = (int *)&fillWide;
        fillHalves[1] = fillValue;
        fillHalves[0] = fillValue;
        do {
            ((long *)dest)[0] = fillWide;
            n -= 64;
            ((long *)dest)[1] = fillWide;
            ((long *)dest)[2] = fillWide;
            ((long *)dest)[3] = fillWide;
            ((long *)dest)[4] = fillWide;
            ((long *)dest)[5] = fillWide;
            ((long *)dest)[6] = fillWide;
            ((long *)dest)[7] = fillWide;
            dest += 64;
        } while (n >= 64);
    }
    if (n >= 16) {
        do {
            ((int *)dest)[0] = fillValue;
            n -= 16;
            ((int *)dest)[1] = fillValue;
            ((int *)dest)[2] = fillValue;
            ((int *)dest)[3] = fillValue;
            dest += 16;
        } while (n >= 16);
    }
    while (n >= 4) {
        *(int *)dest = fillValue;
        n -= 4;
        dest += 4;
    }
    if (n >= 2) {
        *(short *)dest = fillValue;
        n -= 2;
        dest += 2;
    }
    if (n > 0) {
        *dest = fillValue;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_copy);
#ifdef SKIP_ASM
/**
 * Copies n bytes from src to dst (hand-written EE memcpy).
 *
 * Copies under 18 bytes, or from a source that is not 16-byte aligned, byte by byte. An unaligned destination is
 * first aligned by MEM_copy_2(); aligned copies move 128-byte quadword blocks, then 8-byte and single-byte
 * tails. Overlapping ranges are not handled, and nothing is returned.
 *
 * @param dst  destination
 * @param src  source
 * @param n    number of bytes to copy
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written memcpy (0x80-byte lq/sq blocks, ld/sd and byte tails, unfilled delay slots; it branches into MEM_copy_2, which jumps back into it); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl MEM_copy\n.type MEM_copy, @function\nMEM_copy:\n"
    "slti $8, $6, 0x12\n"
    ".word 0x1500002A /* bnez $8, .L003E6624: raw word */\n"
    "andi $8, $4, 0xF\n"
    ".word 0x15000036 /* bnez $8, MEM_copy_2: raw word */\n"
    "nop\n"
    ".L003E6588:\n"
    "andi $8, $5, 0xF\n"
    ".word 0x15000025 /* bnez $8, .L003E6624: raw word */\n"
    "nop\n"
    "addiu $6, $6, -0x80\n"
    ".word 0x04C00015 /* bltz $6, .L003E65F0: raw word */\n"
    "nop\n"
    ".L003E65A0:\n"
    "lq $9, 0x0($5)\n"
    "lq $10, 0x10($5)\n"
    "lq $11, 0x20($5)\n"
    "lq $12, 0x30($5)\n"
    "sq $9, 0x0($4)\n"
    "sq $10, 0x10($4)\n"
    "sq $11, 0x20($4)\n"
    "sq $12, 0x30($4)\n"
    "lq $9, 0x40($5)\n"
    "lq $10, 0x50($5)\n"
    "lq $11, 0x60($5)\n"
    "lq $12, 0x70($5)\n"
    "sq $9, 0x40($4)\n"
    "sq $10, 0x50($4)\n"
    "sq $11, 0x60($4)\n"
    "sq $12, 0x70($4)\n"
    "addiu $6, $6, -0x80\n"
    "addiu $5, $5, 0x80\n"
    ".word 0x04C1FFED /* bgez $6, .L003E65A0: raw word */\n"
    "addiu $4, $4, 0x80\n"
    ".L003E65F0:\n"
    "addiu $6, $6, 0x78\n"
    ".word 0x04C0000A /* bltz $6, .L003E6620: raw word */\n"
    "nop\n"
    ".L003E65FC:\n"
    "ld $9, 0x0($5)\n"
    "addiu $5, $5, 0x8\n"
    "sd $9, 0x0($4)\n"
    "addiu $6, $6, -0x8\n"
    "nop\n"
    ".word 0x04C1FFFA /* bgez $6, .L003E65FC: raw word */\n"
    "addiu $4, $4, 0x8\n"
    ".word 0x04C1FFF8 /* bgez $6, .L003E65FC: raw word */\n"
    "nop\n"
    ".L003E6620:\n"
    "addiu $6, $6, 0x8\n"
    ".L003E6624:\n"
    "addiu $6, $6, -0x1\n"
    ".word 0x04C0000A /* bltz $6, .L003E6654: raw word */\n"
    "nop\n"
    ".L003E6630:\n"
    "addiu $6, $6, -0x1\n"
    "lbu $9, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $9, 0x0($4)\n"
    "nop\n"
    ".word 0x04C1FFFA /* bgez $6, .L003E6630: raw word */\n"
    "addiu $4, $4, 0x1\n"
    ".word 0x04C1FFF8 /* bgez $6, .L003E6630: raw word */\n"
    "nop\n"
    ".L003E6654:\n"
    "jr $31\n"
    "nop\n"
    ".size MEM_copy, . - MEM_copy\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", MEM_copy_2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written byte-align head of the memcpy above (jumps back into MEM_copy once the destination is 16-byte aligned); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl MEM_copy_2\n.type MEM_copy_2, @function\nMEM_copy_2:\n"
    "andi $8, $4, 0xF\n"
    ".word 0x1100FFC9 /* beqz $8, .L003E6588 in MEM_copy: raw word */\n"
    "nop\n"
    "addiu $6, $6, -0x1\n"
    ".word 0x18C00005 /* blez $6, .L003E6684: raw word */\n"
    "lbu $9, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $9, 0x0($4)\n"
    "j MEM_copy_2\n"
    "addiu $4, $4, 0x1\n"
    ".L003E6684:\n"
    "jr $31\n"
    "nop\n"
    ".size MEM_copy_2, . - MEM_copy_2\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", TIMER_setHblankRate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TIMER_hblankRate_0DA0[] __asm__("TIMER_hblankRate");

/**
 * Sets the H-blank rate used for timing from a video mode.
 *
 * Stores the line rate in TIMER_hblankRate: 15750 for mode 1 (NTSC), 15625 for 2 (PAL), 28800 for 3 (the 31 kHz modes); any
 * other value leaves it. THREAD_yieldticks converts ticks to H-blank counts with it. Called by the timer calibration in
 * TIMER_init, by systemInit with 1 and by cPS2GraphicsMan_applyDisplayMode (3 or 1).
 *
 * @param videoMode  the video mode (1 NTSC, 2 PAL, 3 31 kHz)
 */
void TIMER_setHblankRate(int videoMode) {
    switch (videoMode) {
    case 1:
        TIMER_hblankRate_0DA0[0] = 15750;
        return;
    case 2:
        TIMER_hblankRate_0DA0[0] = 15625;
        return;
    case 3:
        TIMER_hblankRate_0DA0[0] = 28800;
        return;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *RpcAlloc_66F0(int) __asm__("RpcAlloc");
extern void NetLib_callService_66F0(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_66F0(int *, int, int) __asm__("NetLib_resolveService");
extern void NetLib_writeBackRange_66F0(void *, int) __asm__("NetLib_writeBackRange");
extern void memset_66F0(void *, int, int) __asm__("memset");

/**
 * Creates a DirtySock game link on the IOP and returns its EE-side block.
 *
 * Allocates @p n + 916 bytes with RpcAlloc and zeroes them: the 0x180-byte NetGameLinkRefT followed by three
 * 64-byte-aligned buffers, the receive buffer (@p n bytes; recvbuf, recvsize), the 336-byte status area (statbuf) and
 * a 4-byte word (sendreadpos) where the IOP reports its send-buffer read position. Sets statstate to -1 (no status
 * read yet) and the sendwindow to @p n, passes the three buffers to NetLib_writeBackRange, then sends 'gmlk'/'crea'
 * (NetLib_resolveService binds the request to that command, NetLib_callService sends it) with @p commRef, @p n and the address of
 * sendreadpos, waiting for the IOP's link reference, which lands in iopref. The channel (cDSockChannel) keeps the
 * block as mGameLink; @p owner is not sent.
 *
 * @param commRef  the connection (comm ref) from NetGameUtilComplete
 * @param owner  owner value (unused)
 * @param n  the receive buffer size in bytes (the channel passes 9084)
 * @return the link block
 */
// `buf` is advanced in place, so the statbuf store is not forwarded to the first call (it reloads the field).
NetGameLinkRefT *NetGameLinkCreate(int commRef, int owner, int n) {
    int msg[4];
    int size;
    int buf;
    NetGameLinkRefT *block;
    size = n + 916;
    block = RpcAlloc_66F0(size);
    memset_66F0(block, 0, size);
    /* PORT: the buffer addresses are aligned in int */
    buf = ((int)block + 0x1BF) & ~0x3F;
    block->recvbuf = (char *)buf;
    block->statstate = -1;
    block->recvsize = n;
    buf += n;
    buf += 0x3F;
    buf &= ~0x3F;
    block->sendwindow = n;
    block->statbuf = (char *)buf;
    buf += 399;
    buf &= ~0x3F;
    block->sendreadpos = (int *)buf;
    NetLib_writeBackRange_66F0(block->statbuf, 336);
    NetLib_writeBackRange_66F0(block->recvbuf, block->recvsize);
    NetLib_writeBackRange_66F0(block->sendreadpos, 4);
    NetLib_resolveService_66F0(msg, 'gmlk', 'crea');
    NetLib_callService_66F0(msg, 1, commRef, n, (int)&block->sendreadpos, 4, 0, 0, 1, (int)block);
    return block;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkDestroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetGameLink_destroyService_67F8[] __asm__("NetGameLink_destroyService");
extern void RpcFree(int *);
extern void NetLib_callService(int *, int, int, int, int, int, int, int, int, int);
extern void NetLib_resolveService(int *, int, int);

/**
 * Destroys a DirtySock game link (EE-side stub of the IOP call).
 *
 * Builds the 'gmlk'/'dest' request in NetGameLink_destroyService with NetLib_resolveService, sends it with NetLib_callService carrying the
 * link's IOP reference (*@p linkRef), then releases the EE-side ref with RpcFree.
 *
 * @param linkRef  the game link ref
 */
void NetGameLinkDestroy(int *linkRef) {
    NetLib_resolveService(NetGameLink_destroyService_67F8, 'gmlk', 'dest');
    NetLib_callService(NetGameLink_destroyService_67F8, 0, *linkRef, 0, 0, 0, 0, 0, 1, 0);
    RpcFree(linkRef);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLink_completeStatus);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* NetGameLinkRefT (ealib/netgamelink.h): its member names at these offsets. */
struct S_6878 { int iopref; int statstate; char pad[0x158 - 8]; int statbuf; };

/**
 * Completion routine of the asynchronous game-link status request (NetGameLinkStatus).
 *
 * Copies the 336-byte status the IOP wrote into the block's status area (+0x158, read through the uncached mirror
 * 0x20000000) to the block's +8 and clears the busy flag +4. The SIF reply dispatcher passes it the request key, its
 * argument, the reply words and the reply data; only the block (the last argument) is used.
 *
 * @param requestKey  the request key (unused)
 * @param requestFlags  the request flags (unused; as NetSession_onRecvDone's)
 * @param replyWord10  reply word +0x10 (unused)
 * @param result  the reply's result word (unused)
 * @param replyData  the reply data address (unused)
 * @param replyLen  the reply data length (unused)
 * @param replyWord8  reply word +0x8 (unused)
 * @param replyWordC  reply word +0xC (unused)
 * @return 0
 */
int NetGameLink_completeStatus(int requestKey, int requestFlags, int replyWord10, int result, int replyData, int replyLen,
        int replyWord8, int replyWordC, struct S_6878 *self) {
    char *src = (char *)((self->statbuf & 0x0FFFFFFF) | 0x20000000);
    __builtin_memcpy((char *)self + 8, src, 336);
    self->statstate = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
/* NetGameLinkRefT (ealib/netgamelink.h): its member names at these offsets. */
typedef struct { int iopref; int statstate; char pad8[0x150]; int statbuf; } G_6958;
extern int NetLib_callService_6958(int *, int, int, int, int, int, int, int, void *, void *) __asm__("NetLib_callService");
extern int NetLib_resolveService_6958(int *, int, int) __asm__("NetLib_resolveService");
extern int NetGameLink_completeStatus_6958(int, int, int, int, int, int, int, int, G_6958 *) __asm__("NetGameLink_completeStatus");
extern int NetGameLink_statusService_6958[] __asm__("NetGameLink_statusService");

/**
 * Returns the game link's latest status, starting a refresh when none is in flight.
 *
 * The status is the 336-byte copy at the block's +8. The busy flag +4 is -1 on a new link: then the first 'gmlk'/'stat'
 * request is sent and waited for (mode 1, completion 1) and copied in with NetGameLink_completeStatus at once. When it is 0 (idle),
 * it is set to 1 and an asynchronous request goes out (into NetGameLink_statusService) with NetGameLink_completeStatus as completion, which copies
 * the status and clears the flag; the caller gets the previous copy meanwhile.
 *
 * @return the link's status copy (block +8)
 */
char *NetGameLinkStatus(G_6958 *self) {
    int was = 0;
    if (self->statstate == -1) {
        self->statstate = 0;
        was = 1;
    }
    if (self->statstate == 0) {
        self->statstate = 1;
        NetLib_resolveService_6958(NetGameLink_statusService_6958, 'gmlk', 'stat');
        if (was == 0) {
            NetLib_callService_6958(NetGameLink_statusService_6958, 0, self->iopref, 0, 0, 0, self->statbuf, 336, NetGameLink_completeStatus_6958, self);
        } else {
            NetLib_callService_6958(NetGameLink_statusService_6958, 1, self->iopref, 0, 0, 0, self->statbuf, 336, (void *)1, 0);
            NetGameLink_completeStatus_6958(0, 0, 0, 0, 0, 0, 0, 0, self);
        }
    }
    return (char *)self + 8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkPeek);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_0041605C_6A70(short *, int, int) __asm__("func_0041605C");

/**
 * Looks at the next packet in the game link's receive buffer without consuming it.
 *
 * When @p outPacket is not null: if the read position (readpos) is before the end of the received data (readend) it
 * copies the packet's 4-byte head from the uncached receive buffer (recvbuf) into the buffer *@p outPacket points to,
 * then the whole packet (its length is the head's first halfword) with func_0041605C; otherwise it sets *@p outPacket
 * to 0. NetGameLinkRecv uses it for single-packet reads.
 *
 * @param block  the link block
 * @param outPacket  points to the destination packet buffer pointer, or 0 to only ask for the count
 * @return the number of received bytes not yet read
 */
int NetGameLinkPeek(NetGameLinkRefT *block, short **outPacket) {
    if (outPacket != 0) {
        int readPos = block->readpos;
        if (readPos < block->readend) {
            // ORIGINAL-PTR: a hardware address: the IOP fills the receive buffer, so the EE reads it through the
            // uncached alias of its address ((address & 0x0FFFFFFF) | 0x20000000).
            *(NetGameLinkPacketHead *)*outPacket =
                *(NetGameLinkPacketHead *)((((int)block->recvbuf + readPos) & 0x0FFFFFFF) | 0x20000000);
            func_0041605C_6A70(*outPacket, (((int)block->recvbuf + block->readpos) & 0x0FFFFFFF) | 0x20000000,
                               **outPacket);
        } else {
            *outPacket = 0;
        }
    }
    return block->readend - block->readpos;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkRecv);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int NetLib_checkSharedBlock(int, int *);
extern int NetTick(void);
extern int NetLib_resolveService_6B10(int *, int, int) __asm__("NetLib_resolveService");
extern int NetLib_callService_6B10(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern void *func_0041605C_6B10(short *, int, int) __asm__("func_0041605C");
extern int NetGameLink_recvService_6B10[] __asm__("NetGameLink_recvService");

int NetGameLinkRecv_impl(char *self, short *out, int n) __asm__("NetGameLinkRecv");
/**
 * Reads packets from the game link's receive buffer, asking the IOP for more once it is drained.
 *
 * When nothing is buffered (end +0x164 negative) it asks NetLib_checkSharedBlock whether the IOP filled the receive buffer
 * (+0x16C); a fill with a new id (compared with +0x15C) sets the read position +0x160 to 0x10 (past the fill header)
 * and the end to the fill length + 0x10. With data waiting, @p n of 1 copies one packet into @p out (NetGameLinkPeek) and
 * advances by its length; any other @p n copies all the waiting bytes when they fit in @p n (nothing otherwise). Once
 * the buffer is consumed it resets the positions (0, -1), stamps +0x170 with NetTick and sends 'gmlk'/'recv'
 * (into NetGameLink_recvService) for the next fill.
 *
 * @param out  the destination buffer
 * @param n  1 to read one packet, otherwise the buffer size in bytes
 * @return the number of bytes copied (0 when nothing was read)
 */
int NetGameLinkRecv_impl(char *self, short *out, int n) {
    int loc;
    int ret = 0;
    int fillId;

    if (*(int *)(self + 0x164) < 0) {
        fillId = NetLib_checkSharedBlock(*(int *)(self + 0x16C), &loc);
        if (fillId >= 0) {
            if (fillId != *(int *)(self + 0x15C)) {
                *(int *)(self + 0x160) = 0x10;
                *(int *)(self + 0x15C) = fillId;
                *(int *)(self + 0x164) = loc + 0x10;
            }
        }
    }
    if (*(int *)(self + 0x160) < *(int *)(self + 0x164)) {
        if (n == 1) {
            NetGameLinkPeek((NetGameLinkRefT *)self, &out);
            ret = *out;
            *(int *)(self + 0x160) += ret;
        } else {
            int rem = *(int *)(self + 0x164) - *(int *)(self + 0x160);
            if (n >= rem) {
                ret = rem;
                func_0041605C_6B10(out, ((*(int *)(self + 0x16C) + *(int *)(self + 0x160)) & 0x0FFFFFFF) | 0x20000000, ret);
                *(int *)(self + 0x160) = *(int *)(self + 0x164);
            }
        }
    }
    if (*(int *)(self + 0x160) == *(int *)(self + 0x164)) {
        *(int *)(self + 0x160) = 0;
        *(int *)(self + 0x164) = -1;
        *(int *)(self + 0x170) = NetTick();
        NetLib_resolveService_6B10(NetGameLink_recvService_6B10, 'gmlk', 'recv');
        NetLib_callService_6B10(NetGameLink_recvService_6B10, 1, *(int *)self, *(int *)(self + 0x16C), 0, 0, 0, 0, 0, 0);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkSend);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef struct { char b[4]; } U4_6C80;
/* NetGameLinkRefT (ealib/netgamelink.h): its member names at these offsets. */
typedef struct { int id; char pad[0x170]; int sendwindow; int sendpos; unsigned int sendreadpos; } S_6C80;
extern int NetLib_callService_6C80(int *, int, int, int, char *, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_6C80(int *, int, int) __asm__("NetLib_resolveService");
extern int NetGameLink_sendService_6C80[] __asm__("NetGameLink_sendService");

/**
 * Queues a packet on a game link's (@p self) IOP send buffer and tells the IOP side ("gmlk" "send") to send
 * it.
 *
 * A length of 1 means "work it out from the packet header" (rounded up to 4 bytes).
 *
 * @param msg  the packet (On Tour NetGamePacketT* pkt)
 * @param len  packet length in bytes, or 1
 * @return 1 when queued, 0 when the send buffer is full
 */
int NetGameLinkSend(S_6C80 *self, unsigned char *msg, int len) {
    U4_6C80 readPos;
    int w;
    if (len == 1) {
        len = (msg[3] + 0x17) & 0x7FFC;
        *(short *)msg = len;
    }
    readPos = *(U4_6C80 *)((self->sendreadpos & 0x0FFFFFFF) | 0x20000000);
    w = self->sendpos;
    if (self->sendwindow - (w - *(int *)&readPos) < len) return 0;
    self->sendpos = w + len;
    NetLib_resolveService_6C80(NetGameLink_sendService_6C80, 'gmlk', 'send');
    NetLib_callService_6C80(NetGameLink_sendService_6C80, 1, self->id, 0, (char *)msg, len, 0, 0, 0, 0);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameLinkControl);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_6D78(int *, int, int, int, int *, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_6D78(int *, int, int) __asm__("NetLib_resolveService");
extern int NetGameLink_controlService_6D78[] __asm__("NetGameLink_controlService");

int NetGameLinkControl_impl(int *p, int b, int c) __asm__("NetGameLinkControl");
/**
 * Sends a control request to the game link on the IOP and returns its answer.
 *
 * Sends 'gmlk'/'ctrl' (into NetGameLink_controlService) with the link's IOP reference (*@p link), @p selector and the 4 bytes of @p value, and waits
 * for the result. Nothing in the build calls it.
 *
 * @param link  the link block (its +0 is the IOP reference)
 * @param selector  the control selector
 * @param value  the value
 * @return the IOP's result
 */
int NetGameLinkControl_impl(int *link, int selector, int value) {
    int out;

    NetLib_resolveService_6D78(NetGameLink_controlService_6D78, 'gmlk', 'ctrl');
    NetLib_callService_6D78(NetGameLink_controlService_6D78, 1, *link, selector, &value, 4, 0, 0, 1, &out);
    return out;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilCreate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_6E08(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_6E08(int *, int, int) __asm__("NetLib_resolveService");

/**
 * Creates a DirtySock game-util (connection setup) object on the IOP.
 *
 * Sends 'gmut'/'crea' and waits for the IOP's reference. cDSockChannel_connect and cDSockChannel_listen store it as
 * mGameUtil; LobbyMan's LobbyMan_createConfig creates its mpConfig with it to advertise the persona.
 *
 * @return the IOP game-util reference
 */
int NetGameUtilCreate(void) {
    int msg[4];
    int result;

    NetLib_resolveService_6E08(msg, 'gmut', 'crea');
    NetLib_callService_6E08(msg, 1, 0, 0, 0, 0, 0, 0, 1, &result);
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilDestroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_6E70(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_6E70(int *, int, int) __asm__("NetLib_resolveService");

/**
 * Destroys a DirtySock game-util (connection setup) object (EE-side stub of the IOP call).
 *
 * Builds the 'gmut'/'dest' request with NetLib_resolveService and sends it with NetLib_callService with @p utilRef as the IOP
 * reference.
 *
 * @param utilRef  the game util ref
 */
void NetGameUtilDestroy(int utilRef) {
    int request[4];

    NetLib_resolveService_6E70(request, 'gmut', 'dest');
    NetLib_callService_6E70(request, 1, utilRef, 0, 0, 0, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilReset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_6ED8(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_6ED8(int *, int, int) __asm__("NetLib_resolveService");

/**
 * Resets a game-util object on the IOP ('gmut'/'rset', waited for). Nothing in the build calls it.
 *
 * @param handle  the IOP game-util reference
 */
void NetGameUtilReset(int handle) {
    int msg[4];

    NetLib_resolveService_6ED8(msg, 'gmut', 'rset');
    NetLib_callService_6ED8(msg, 1, handle, 0, 0, 0, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilConnect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_6F40(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_6F40(int *, int, int) __asm__("NetLib_resolveService");

int NetGameUtilConnect_impl(int a, int b, int c) __asm__("NetGameUtilConnect");
/**
 * Starts a game-util connection (listen or connect) on the IOP.
 *
 * Sends 'gmut'/'conn' with @p handle, @p connectKind and the string @p address (length -1: up to its terminator) and waits for the
 * result. The connection is picked up later with NetGameUtilComplete.
 *
 * @param handle  the IOP game-util reference
 * @param connectKind  the connection kind (listen or connect, as the channel passes it)
 * @param address  the address string the channel formats
 * @return the IOP's result
 */
int NetGameUtilConnect_impl(int handle, int connectKind, int address) {
    int msg[4];
    int result;

    NetLib_resolveService_6F40(msg, 'gmut', 'conn');
    NetLib_callService_6F40(msg, 1, handle, connectKind, address, -1, 0, 0, 1, &result);
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilComplete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_6FC8(void *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_6FC8(void *, int, int) __asm__("NetLib_resolveService");
struct S_6FC8 { int a[4]; int out; };

/**
 * Asks the IOP whether a game-util connection has completed.
 *
 * Sends 'gmut'/'comp' and waits for the answer: the connection (comm reference) once it is up, which cDSockChannel_updateConnecting
 * passes to NetGameLinkCreate to create the game link.
 *
 * @param handle  the IOP game-util reference
 * @return the comm reference, or 0 while still connecting
 */
int NetGameUtilComplete(int handle) {
    struct S_6FC8 msg;

    NetLib_resolveService_6FC8(&msg, 'gmut', 'comp');
    NetLib_callService_6FC8(&msg, 1, handle, 0, 0, 0, 0, 0, 1, &msg.out);
    return msg.out;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtil_queryStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_7038(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_7038(int *, int, int) __asm__("NetLib_resolveService");

int NetGameUtil_queryStatus_impl(int a, int b) __asm__("NetGameUtil_queryStatus");
/**
 * Queries a game-util object's status on the IOP.
 *
 * Sends 'gmut'/'stat' with @p handle and @p selector and waits for the result. Nothing in the build calls it.
 *
 * @param handle  the IOP game-util reference
 * @param selector  the status selector
 * @return the IOP's answer
 */
int NetGameUtil_queryStatus_impl(int handle, int selector) {
    int msg[4];
    int result;

    NetLib_resolveService_7038(msg, 'gmut', 'stat');
    NetLib_callService_7038(msg, 1, handle, selector, 0, 0, 0, 0, 1, &result);
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtil_postStatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_70B8(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_70B8(int *, int, int) __asm__("NetLib_resolveService");

/**
 * Sends a game-util status request to the IOP and ignores the answer.
 *
 * Sends 'gmut'/'stat' with @p handle and no selector, with no result pointer. Same command as NetGameUtil_queryStatus; nothing in
 * the build calls it.
 *
 * @param handle  the IOP game-util reference
 */
void NetGameUtil_postStatus(int handle) {
    int msg[4];

    NetLib_resolveService_70B8(msg, 'gmut', 'stat');
    NetLib_callService_70B8(msg, 1, handle, 0, 0, 0, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtil_advertise);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_7120(int *, int, int, int, char *, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_7120(int *, int, int) __asm__("NetLib_resolveService");
extern int sprintf_7120(char *, int *, int, int) __asm__("sprintf");
extern int str_pct_s_pct_s_d_7120[] __asm__("str_pct_s_pct_s_d");

/**
 * Advertises a name of a given kind on the network through the IOP game util.
 *
 * Joins @p kind and @p name with a tab ("%s\t%s", str_pct_s_pct_s_d) and sends the text with 'gmut'/'advt'. LobbyMan's
 * LobbyMan_createConfig calls it with its config util, "SSX-PS2-2004" and the persona name.
 *
 * @param util  the IOP game-util reference
 * @param kind  the advert kind (game id string)
 * @param name  the advertised name
 */
void NetGameUtil_advertise(int util, int kind, int name) {
    char advertText[256];
    int request[4];

    sprintf_7120(advertText, str_pct_s_pct_s_d_7120, kind, name);
    NetLib_resolveService_7120(request, 'gmut', 'advt');
    NetLib_callService_7120(request, 1, util, 0, advertText, -1, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilWithdraw);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void NetLib_callService_71B0(int *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern int NetLib_resolveService_71B0(int *, int, int) __asm__("NetLib_resolveService");

/**
 * Withdraws the game util's network advert ('gmut'/'with', waited for). Nothing in the build calls it.
 *
 * @param handle  the IOP game-util reference
 */
void NetGameUtilWithdraw(int handle) {
    int msg[4];

    NetLib_resolveService_71B0(msg, 'gmut', 'with');
    NetLib_callService_71B0(msg, 1, handle, 0, 0, 0, 0, 0, 1, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtil_locate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_7218(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_7218(int *, int, int) __asm__("NetLib_resolveService");
extern char str_empty_r_7218[] __asm__("str_empty_r");

int NetGameUtil_locate_impl(int a, int b, char *c) __asm__("NetGameUtil_locate");
/**
 * Looks up the address of an advertised name through the IOP game util.
 *
 * Sends 'gmut'/'loca' with @p util and the string @p name (an empty string when null) and waits for the address. @p kind is not
 * sent. LobbyMan's LobbyMan_getConfigValue uses it to find a persona's host.
 *
 * @param util  the IOP game-util reference
 * @param kind  the advert kind (unused)
 * @param name  the name to locate, or null for ""
 * @return the address found (0 when none)
 */
int NetGameUtil_locate_impl(int util, int kind, char *name) {
    int request[4];
    int foundAddress;

    if (name == 0) {
        name = str_empty_r_7218;
    }
    NetLib_resolveService_7218(request, 'gmut', 'loca');
    NetLib_callService_7218(request, 1, util, 0, (int)name, -1, 0, 0, 1, &foundAddress);
    return foundAddress;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", NetGameUtilQuery);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_72A0(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_72A0(int *, int, int) __asm__("NetLib_resolveService");
extern char str_empty_r_72A0[] __asm__("str_empty_r");

int NetGameUtilQuery_impl(int a, char *b, int c, int d) __asm__("NetGameUtilQuery");
/**
 * Queries the network adverts of a kind through the IOP game util.
 *
 * Sends 'gmut'/'quer' (mode 5) with @p util, the kind string @p kind (empty when null) and the output buffer @p buffer / @p bufferSize, and
 * waits for the result. Nothing in the build calls it.
 *
 * @param util  the IOP game-util reference
 * @param kind  the advert kind, or null for ""
 * @param buffer  receives the adverts found
 * @param bufferSize  the size of @p buffer in bytes
 * @return the number of adverts found
 */
int NetGameUtilQuery_impl(int util, char *kind, int buffer, int bufferSize) {
    int request[4];
    int count;

    if (kind == 0) {
        kind = str_empty_r_72A0;
    }
    NetLib_resolveService_72A0(request, 'gmut', 'quer');
    NetLib_callService_72A0(request, 5, util, 0, (int)kind, -1, buffer, bufferSize, 1, &count);
    return count;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PingRpc_create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int NetLib_callService_7340(int *, int, int, int, int, int, int, int, int, char *) __asm__("NetLib_callService");
extern int NetLib_resolveService_7340(int *, int, int) __asm__("NetLib_resolveService");
extern char *RpcAlloc_7340(int) __asm__("RpcAlloc");
extern void *memset_7340(void *, int, int) __asm__("memset");

/**
 * Creates a ping object on the IOP and returns its EE-side block.
 *
 * Allocates and zeroes 2132 bytes with RpcAlloc, stores the 64-byte-aligned address of its 2 KB response buffer at
 * +0x10 (block + 0x53 & ~0x3F) and sends 'ping'/'crea', waiting for the IOP reference into +0xC. The lobby
 * (LobbyApi_create) keeps one at +4 and cGameComm one at +0x140; PingRpc_destroy destroys it.
 *
 * @return the ping block
 */
char *PingRpc_create(void) {
    int buf[4];
    char *block = RpcAlloc_7340(2132);
    memset_7340(block, 0, 2132);
    *(unsigned int *)(block + 0x10) = (unsigned int)(block + 0x53) & 0xFFFFFFC0;
    NetLib_resolveService_7340(buf, 'ping', 'crea');
    NetLib_callService_7340(buf, 1, 0, 0, 0, 0, 0, 0, 1, block + 0xC);
    return block;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PingRpc_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void RpcFree_73E0(void *) __asm__("RpcFree");
extern int NetLib_callService_73E0(void *, int, int, int, int, int, int, int, int, int) __asm__("NetLib_callService");
extern void NetLib_resolveService_73E0(void *, int, int) __asm__("NetLib_resolveService");
struct S_73E0 { int a[4]; };

/**
 * Destroys a ping object made by PingRpc_create.
 *
 * Sends 'ping'/'dest' with its IOP reference (+0xC) without waiting, then frees the block with RpcFree.
 *
 * @param block  the ping block
 */
void PingRpc_destroy(void *block) {
    struct S_73E0 msg;

    NetLib_resolveService_73E0(&msg, 'ping', 'dest');
    NetLib_callService_73E0(&msg, 0, *(int *)((char*)block + 0xC), 0, 0, 0, 0, 0, 1, 0);
    RpcFree_73E0(block);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PingRpc_request);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int NetLib_callService_7450(int *, int, int, int, int, int, int, int, int, int *) __asm__("NetLib_callService");
extern void NetLib_resolveService_7450(int *, int, int) __asm__("NetLib_resolveService");

int PingRpc_request_impl(char *o, int a, int b, int c) __asm__("PingRpc_request");
/**
 * Sends a ping request through a ping object.
 *
 * Sends 'ping'/'requ' with the block's IOP reference (+0xC), @p address, @p payload and @p payloadLen, and waits for the result. The lobby
 * pings servers and players with a tag payload (LobbyApi_ping), cGameComm_update and the pause-overlay check
 * (cGameComm_startServerPing) use it too; replies are collected with PingRpc_pollResponse.
 *
 * @param block  the ping block
 * @param address  the address to ping
 * @param payload  the payload
 * @param payloadLen  the payload length in bytes
 * @return the IOP's result
 */
int PingRpc_request_impl(char *block, int address, int payload, int payloadLen) {
    int msg[4];
    int result;

    NetLib_resolveService_7450(msg, 'ping', 'requ');
    NetLib_callService_7450(msg, 1, *(int *)(block + 0xC), address, payload, payloadLen, 0, 0, 1, &result);
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PingRpc_completeResponse);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* The ping block (names ours, from use): recvLen the reply length and busy the request-in-flight flag (set by this
   RPC callback), sendTick the NetTick of the last request, iopref the IOP-side reference, respBuf the 2 KB reply
   buffer. */
struct S_7500 { volatile int recvLen; volatile int busy; int sendTick; int iopref; char *respBuf; };

/**
 * Completion routine of the ping-response request sent by PingRpc_pollResponse.
 *
 * Stores @p recvBytes (the byte count the IOP wrote into the response buffer) in the block's +0 and clears its busy flag +4.
 * The SIF reply dispatcher passes it the request key, its argument, the reply words and the reply data; the rest is
 * unused.
 *
 * @param requestKey  the request key (unused)
 * @param requestFlags  the request flags (unused; as NetSession_onRecvDone's)
 * @param replyWord10  reply word +0x10 (unused)
 * @param result  the reply's result word (unused)
 * @param replyData  the reply data address (unused)
 * @param replyLen  the reply data length (unused)
 * @param replyWord8  reply word +0x8 (unused)
 * @param recvBytes  the number of response bytes received
 * @param out  the ping block
 * @return 0
 */
int PingRpc_completeResponse(int requestKey, int requestFlags, int replyWord10, int result, int replyData, int replyLen,
        int replyWord8, int recvBytes, struct S_7500 *out)
{
    out->recvLen = recvBytes;
    out->busy = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", PingRpc_pollResponse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { char b[4]; } U4_7500;
extern int PingRpc_responseService_7500[] __asm__("PingRpc_responseService");
extern void InvalidDCache_7500(char *, char *) __asm__("InvalidDCache");
extern void func_0041605C_7500(char *, char *, int) __asm__("func_0041605C");
extern int NetLib_resolveService_7500(int *, int, int) __asm__("NetLib_resolveService");
extern void NetLib_callService_7500(int *, int, int, int, int, int, char *, int, void *, void *) __asm__("NetLib_callService");
extern char PingRpc_completeResponse_7500[] __asm__("PingRpc_completeResponse");

int PingRpc_pollResponse_impl(struct S_7500 *self, char *buf, int *a2, int *a3) __asm__("PingRpc_pollResponse");
/**
 * Collects a ping reply, if one has arrived, and keeps a response request outstanding.
 *
 * When the block holds a reply (+0 > 0 bytes in the 2 KB buffer +0x10): the result is its first word; the 16-byte
 * header is copied to @p response and the payload (up to *@p bufLen bytes, *@p bufLen updated) to @p buf; then the count and the time
 * stamp +8 are cleared. Then, when no request is in flight (+4 == 0) and no reply waits, it sends a new 'ping'/'resp'
 * request (into PingRpc_responseService) with the 2 KB buffer and PingRpc_completeResponse as completion, at most once per 6 units of
 * NetTick (+8 holds the last send) and, for a fresh buffer, after passing it to InvalidDCache and marking the
 * count -1.
 *
 * @param buf  receives the reply payload, or 0
 * @param bufLen  in: the size of @p buf; out: the payload bytes copied (or 0 to skip the payload)
 * @param response  receives the 16-byte reply header (round-trip time, address, ...), or 0
 * @return the reply's first word (the round-trip time), 0 when no reply was waiting
 */
int PingRpc_pollResponse_impl(struct S_7500 *self, char *buf, int *bufLen, int *response) {
    int res = 0;

    if (self->recvLen > 0) {
        *(U4_7500 *)&res = *(U4_7500 *)self->respBuf;
        if (response != 0) {
            *response = res;
            *(U4_7500 *)(response + 1) = *(U4_7500 *)(self->respBuf + 4);
            *(U4_7500 *)(response + 2) = *(U4_7500 *)(self->respBuf + 8);
            *(U4_7500 *)(response + 3) = *(U4_7500 *)(self->respBuf + 12);
        }
        if (bufLen != 0) {
            if (self->recvLen - 0x10 < *bufLen) {
                *bufLen = self->recvLen - 0x10;
            }
            func_0041605C_7500(buf, self->respBuf + 0x10, *bufLen);
        }
        self->recvLen = 0;
        self->sendTick = 0;
    }
    if (self->busy == 0 && self->recvLen <= 0) {
        if (self->sendTick != 0) {
            if (NetTick() - self->sendTick < 6) return res;
        }
        self->sendTick = NetTick();
        self->busy = 1;
        if (self->recvLen == 0) {
            InvalidDCache_7500(self->respBuf, self->respBuf + 0x7FF);
            self->recvLen = -1;
        }
        NetLib_resolveService_7500(PingRpc_responseService_7500, 'ping', 'resp');
        NetLib_callService_7500(PingRpc_responseService_7500, 1, self->iopref, 0, 0, 0, self->respBuf, 0x800, PingRpc_completeResponse_7500, self);
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_debugPrintf);
#ifdef SKIP_ASM
extern void vsprintf_76A8(char *, const char *, char *) __asm__("vsprintf");

/**
 * Formats a debug message for the lobby and hands it to the lobby's debug routine.
 *
 * Formats @p fmt and the variable arguments into a 4 KB buffer (vsprintf, vsprintf-style) and, when a routine is
 * set at +0x548, calls it with the data word +0x544 and the text (both given to LobbyApi_create). Used for the
 * "connecting to %08x:%d" note, the incoming-message dump LobbyApi_printMessage and the server ping timeout.
 *
 * @param fmt  printf-style format; the arguments follow
 */
// PORT: SN-specific va_start
void LobbyApi_debugPrintf(char *self, const char *fmt, ...) {
    char buf[0x1000];
    char *ap;
    void (*cb)(int, char *);
    ap = (char*)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    vsprintf_76A8(buf, fmt, ap);
    cb = *(void (**)(int, char *))(self + 0x548);
    if (cb != 0) {
        cb(*(int *)(self + 0x544), buf);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_queueRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *lobbyman_alloc_7718(int) __asm__("lobbyman_alloc");

/**
 * Queues a pending-request record on the lobby and returns it.
 *
 * Allocates a 68-byte record (lobbyman_alloc): +0 a request id taken from the counter +0x54C (which then advances, kept
 * below 2^31), +4 the command @p tag, +0x38 / +0x3C the completion routine and its context. It is linked into the
 * request list +0x53C in front of the first 'ping' record, so command requests stay ahead of pings. The lobby's command
 * (LobbyApi_sendCommand), connect (LobbyApi_connect) and ping (LobbyApi_ping) routines use it.
 *
 * @param lobby  the lobby
 * @param tag  the command FourCC ('auth', 'skey', 'ping', ...)
 * @param callback  the routine to call when the reply arrives
 * @param context  the context passed to it
 * @return the record, or 0 when the allocation failed
 */
void *LobbyApi_queueRequest(char *lobby, int tag, int callback, int context) {
    char **pp;
    char *n;
    char *cur;
    char *nx;
    char *head;

    n = lobbyman_alloc_7718(68);
    if (n != 0) {
        *(int*)(n + 4) = tag;
        *(int*)(n + 0x38) = callback;
        *(int*)(n + 0x3C) = context;
        pp = (char**)(lobby + 0x53C);
        *(char**)(n + 0x40) = 0;
        *(int*)n = *(int*)(lobby + 0x54C);
        head = *(char**)(lobby + 0x53C);
        *(int*)(lobby + 0x54C) = (*(int*)(lobby + 0x54C) + 1) & 0x7FFFFFFF;
        if (head != 0 && *(int*)(head + 4) != 'ping') {
            do {
                cur = *pp;
                nx = *(char**)(cur + 0x40);
                pp = (char**)(cur + 0x40);
            } while (nx != 0 && *(int*)(nx + 4) != 'ping');
        }
        cur = *pp;
        *pp = n;
        *(char**)(n + 0x40) = cur;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_takeRequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct N_77F0 { int pad0; int key; int w[14]; struct N_77F0 *next; } N_77F0;
extern void *memset_77F0(void *, int, unsigned int) __asm__("memset");
extern void lobbyman_free_77F0(void *) __asm__("lobbyman_free");
/**
 * Takes the first pending-request record off the lobby's list when it matches a command.
 *
 * Clears @p dst (68 bytes) and, when the head of the request list +0x53C has the command @p key (or @p key is 'DQUE',
 * which takes any), unlinks it, copies it into @p dst and frees it (lobbyman_free). LobbyApi_update uses it to find the
 * completion routine of each reply; LobbyApi_dropRequests drains the list with 'DQUE'.
 *
 * @param dst  receives the record (all zero when none was taken)
 * @param key  the reply's command FourCC, or 'DQUE' for any
 * @return nonzero while records remain on the list
 */
int LobbyApi_takeRequest(char *self, N_77F0 *dst, int key) {
    N_77F0 *n;
    memset_77F0(dst, 0, 68);
    n = *(N_77F0 **)(self + 0x53C);
    if (n != 0 && (n->key == key || key == 'DQUE')) {
        *(N_77F0 **)(self + 0x53C) = n->next;
        *dst = *n;
        lobbyman_free_77F0(n);
    }
    return *(int *)(self + 0x53C) != 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_takePing);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct N_7918 { int f0; int tag; int id; int fC; int f10; int f14; char name[0x28]; struct N_7918 *next; };
struct L_7918 { char pad[0x53C]; struct N_7918 *head; int cnt; };
extern int strcmp_7918(char *, char *) __asm__("strcmp");
extern void lobbyman_free_7918(void *) __asm__("lobbyman_free");

/**
 * Takes a matching ping record off the lobby's request list.
 *
 * Walks the list +0x53C for a 'ping' record that either has address @p id and the name @p name (strcmp), or,
 * when @p id is -1, a deadline (+0xC) before @p cutoff. The first match is unlinked, copied into @p out, freed and the
 * outstanding-ping count +0x540 decremented. Used by LobbyApi_handlePings for replies and for timeouts.
 *
 * @param list  the lobby
 * @param out  receives the record
 * @param name  the pinged name (ignored when @p id is -1)
 * @param id  the pinged address, or -1 to look for an expired ping
 * @param cutoff  the current time, for expired pings
 * @return 1 when a record was taken, 0 otherwise
 */
int LobbyApi_takePing(struct L_7918 *list, struct N_7918 *out, char *name, int id, int cutoff) {
    struct N_7918 **pp = &list->head;
    struct N_7918 *n;

    while (*pp != 0) {
        n = *pp;
        if (n->tag == 'ping' &&
            ((n->id == id && strcmp_7918(name, n->name) == 0) || (id == -1 && n->fC < cutoff))) {
            *pp = n->next;
            *out = *n;
            lobbyman_free_7918(n);
            list->cnt--;
            return 1;
        }
        pp = &(*pp)->next;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_dropRequests);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int LobbyApi_takeRequest_7AC0(int, void *, int) __asm__("LobbyApi_takeRequest");

/**
 * Drops every pending request record of the lobby (LobbyApi_takeRequest with 'DQUE' until the list is empty).
 *
 * @param lobby  the lobby
 */
void LobbyApi_dropRequests(int lobby) {
    char buf[80];
    while (LobbyApi_takeRequest_7AC0(lobby, buf, 0x44515545) != 0) {
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_setRoomPopulation);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SortList_setValue(int, int, void *);
extern void *SortList_getValue(int, int);
extern int HashTable_findInt(int);
extern void sprintf(void *, int *, int);
extern int str_pct_d_e_7B00[] __asm__("str_pct_d_e");

/**
 * Updates a room's user count in the lobby's room list.
 *
 * When the room list (slot 0: index +0x310, list +0x314) exists, finds room @p roomId (HashTable_findInt, SortList_getValue) and,
 * if its count (+0x10) differs from @p population, stores the count, writes it as text at +0x14 ("%d") and puts the record
 * back (SortList_setValue). LobbyApi_update calls it for "+who", "+pop", "+usr" and move / room replies.
 *
 * @param lobby  the lobby
 * @param roomId  the room id
 * @param population  the room's user count
 */
void LobbyApi_setRoomPopulation(void *lobby, int roomId, int population) {
    int temp_2;
    int temp_4;
    void *temp_2_2;

    temp_4 = (*(int *)((char*)(lobby) + (0x310)));
    if (temp_4 != 0) {
        temp_2 = HashTable_findInt(temp_4);
        if (temp_2 != 0) {
            temp_2_2 = SortList_getValue((*(int *)((char*)(lobby) + (0x314))), temp_2);
            if ((temp_2_2 != 0) && (population != (*(int *)((char*)(temp_2_2) + (0x10))))) {
                (*(int *)((char*)(temp_2_2) + (0x10))) = population;
                sprintf(temp_2_2 + 0x14, str_pct_d_e_7B00, population);
                SortList_setValue((*(int *)((char*)(lobby) + (0x314))), temp_2, temp_2_2);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_notifyListChanges);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SortList_update_7BA0(int) __asm__("SortList_update");

struct M_7BA0 { int type; int idx; int zero; int h; };
/* A list slot of LobbyApiRefT::mLists: {index, list, routine, context, ?} (names ours, as the header's). */
struct S_7BA0 { int h; int list; void (*fn)(void *, struct M_7BA0 *, int); int context; int pad; };
struct O_7BA0 { char pad[0x310]; struct S_7BA0 s[8]; };

void LobbyApi_notifyListChanges_impl(struct O_7BA0 *self) __asm__("LobbyApi_notifyListChanges");
/**
 * Notifies the owners of the lobby's lists that changed.
 *
 * For each of the 8 list slots (+0x310, 0x14 bytes each: index, list, routine, context) that is open and has a routine,
 * when SortList_update reports changes on its list (> 0) it calls the routine with the lobby, an event (type 2, the slot
 * number, 0, the list) and the slot's context. Called from LobbyApi_update.
 */
void LobbyApi_notifyListChanges_impl(struct O_7BA0 *self) {
    int i;
    for (i = 0; i < 8; i++) {
        struct S_7BA0 *listSlot = &self->s[i];
        if (listSlot->h != 0 && listSlot->fn != 0) {
            if (SortList_update_7BA0(listSlot->list) > 0) {
                struct M_7BA0 changeEvent;
                changeEvent.h = listSlot->list;
                changeEvent.type = 2;
                changeEvent.idx = i;
                changeEvent.zero = 0;
                listSlot->fn(self, &changeEvent, listSlot->context);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_handlePings);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cDirtysock_tag_TagFieldFind_7C40(char *, char *) __asm__("cDirtysock_tag_TagFieldFind");
extern unsigned int cDirtysock_tag_TagFieldGetAddr_7C40(int, unsigned int) __asm__("cDirtysock_tag_TagFieldGetAddr");
extern int cDirtysock_tag_TagFieldGetString_7C40(int, char *, int, char *) __asm__("cDirtysock_tag_TagFieldGetString");
extern int cDirtysock_tag_TagFieldSetNumber_7C40(char *, int, char *, int) __asm__("cDirtysock_tag_TagFieldSetNumber");
extern void cDirtysock_tag_TagFieldSetString_7C40(char *, int, char *, char *) __asm__("cDirtysock_tag_TagFieldSetString");
extern int PingRpc_pollResponse_7C40(int, char *, int *, int *) __asm__("PingRpc_pollResponse");
extern int SortList_getPassCount_7C40(int) __asm__("SortList_getPassCount");
extern char *SortList_getAt_7C40(int, int) __asm__("SortList_getAt");
extern void SortList_bumpChanged_7C40(int, int) __asm__("SortList_bumpChanged");
extern int NetSession_getTick_7C40(int) __asm__("NetSession_getTick");
extern int strcasecmp_7C40(char *, char *) __asm__("strcasecmp");
extern void sprintf_7C40(char *, char *, int) __asm__("sprintf");

extern char str_ORIG[];
extern char str_NAME_b[];
extern char str_empty_k[];
extern char str_ADDR_b[];
extern char str_PING_b[];
extern char str_pct_dms[];

struct Rec_7C40 {
    int f0;
    int tag;
    int id;
    int fC;
    int f10;
    int f14;
    char name[0x20];
    void (*fn)(char *, struct LobbyApiMsgT *, int);
    int ctx;
    struct Rec_7C40 *next;
};

extern int LobbyApi_takePing_7C40(char *, struct Rec_7C40 *, char *, int, int) __asm__("LobbyApi_takePing");

/**
 * Handles ping replies and expired pings for the lobby.
 *
 * For every reply PingRpc_pollResponse delivers on the lobby's ping object (+4) it reads the reply's "ORIG" address and
 * "NAME"; the matching pending ping (LobbyApi_takePing) gets its routine called with a 'ping' event whose tags carry NAME,
 * ADDR and PING (the round-trip time). The time is also written as "%dms" into the room-list entry with that address
 * (slot 0, +0x64 / +0x5C) and the user-list entry with that name (slot 1, +8 / +0x28), each followed by
 * SortList_bumpChanged(list, 1). Then every ping whose deadline has passed (protocol time from NetSession_getTick) is completed
 * with code -1 and PING 0.
 */
void LobbyApi_handlePings(char *self) {
    int info[2];
    char buf[0x100];
    char name[0x20];
    struct Rec_7C40 rec;
    struct LobbyApiMsgT ev;
    char evbuf[0x200];
    int len;
    unsigned int addr;
    int currentTick;

    for (;;) {
        len = 0x100;
        if (PingRpc_pollResponse_7C40(*(int *)(self + 4), buf, &len, info) == 0) {
            break;
        }
        addr = cDirtysock_tag_TagFieldGetAddr_7C40(cDirtysock_tag_TagFieldFind_7C40(buf, str_ORIG), info[1]);
        cDirtysock_tag_TagFieldGetString_7C40(cDirtysock_tag_TagFieldFind_7C40(buf, str_NAME_b), name, 0x20,
                                              str_empty_k);
        if (LobbyApi_takePing_7C40(self, &rec, name, addr, 0) != 0 && rec.fn != 0) {
            ev.mType = 0;
            ev.mKind = 'ping';
            ev.mStatus = 0;
            ev.mpTagText = evbuf;
            evbuf[0] = 0;
            cDirtysock_tag_TagFieldSetString_7C40(evbuf, 0x200, str_NAME_b, name);
            cDirtysock_tag_TagFieldSetNumber_7C40(evbuf, 0x200, str_ADDR_b, rec.id);
            cDirtysock_tag_TagFieldSetNumber_7C40(evbuf, 0x200, str_PING_b, info[0]);
            rec.fn(self, &ev, rec.ctx);
        }
        if (*(int *)(self + 0x310) != 0) {
            char *listEntry;

            for (len = 0; (listEntry = SortList_getAt_7C40(*(int *)(self + 0x314), len)) != 0; len++) {
                if (*(unsigned int *)(listEntry + 0x64) == addr) {
                    sprintf_7C40(listEntry + 0x5C, str_pct_dms, info[0]);
                    if (len < SortList_getPassCount_7C40(*(int *)(self + 0x314))) {
                        SortList_bumpChanged_7C40(*(int *)(self + 0x314), 1);
                    }
                }
            }
        }
        if (*(int *)(self + 0x324) != 0) {
            char *listEntry;

            for (len = 0; (listEntry = SortList_getAt_7C40(*(int *)(self + 0x328), len)) != 0; len++) {
                if (strcasecmp_7C40(listEntry + 8, name) == 0) {
                    sprintf_7C40(listEntry + 0x28, str_pct_dms, info[0]);
                    SortList_bumpChanged_7C40(*(int *)(self + 0x328), 1);
                }
            }
        }
    }
    currentTick = NetSession_getTick_7C40(*(int *)self);
    while (LobbyApi_takePing_7C40(self, &rec, name, -1, currentTick) != 0) {
        if (rec.fn != 0) {
            ev.mType = 0;
            ev.mKind = 'ping';
            ev.mStatus = -1;
            ev.mpTagText = evbuf;
            evbuf[0] = 0;
            cDirtysock_tag_TagFieldSetString_7C40(evbuf, 0x200, str_NAME_b, rec.name);
            cDirtysock_tag_TagFieldSetNumber_7C40(evbuf, 0x200, str_ADDR_b, rec.id);
            cDirtysock_tag_TagFieldSetNumber_7C40(evbuf, 0x200, str_PING_b, 0);
            rec.fn(self, &ev, rec.ctx);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_resetServerPing);
#ifdef SKIP_ASM
/**
 * Resets the lobby's server-ping state.
 *
 * Clears the failure count +0x20 and the next-ping time +0x18, and sets the last ping result +0x1C to -1 (none). See
 * LobbyApi_onServerPing.
 *
 * @param lobby  the lobby
 */
void LobbyApi_resetServerPing(LobbyApiRefT* lobby) {
    lobby->mPingFailures = 0;
    lobby->mNextPingTime = 0;
    *(unsigned *)&lobby->mLastPing = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_dropConnection);
#ifdef SKIP_ASM
/**
 * Drops the lobby connection with a reason and reports it.
 *
 * Sets the state +0xC to 'term', ORs @p flags into the event flags +8, resets the server-ping state (LobbyApi_resetServerPing) and
 * calls the connection routine (+0x52C, context +0x530) with an event of type 3, kind 'disc' and code @p reason.
 * LobbyApi_update calls it on timeouts ('time', 0x800), lost connections and failed logins ('auth', 2).
 *
 * @param lobby  the lobby
 * @param reason  the code passed in the 'disc' event
 * @param flags  the event flags to raise
 */
void LobbyApi_dropConnection(LobbyApiRefT* lobby, int reason, int flags) {
    int sp[4];
    int (*fn)(char *, int *, int);

    lobby->mState = 'term';
    lobby->mEventFlags |= flags;
    LobbyApi_resetServerPing((struct LobbyApiRefT *)lobby);
    fn = (int (*)(char*, int*, int))lobby->mEventProcs[3][0];
    if (fn != 0) {
        sp[0] = 3; sp[1] = 'disc'; sp[2] = reason; sp[3] = 0;
        fn(((char*)lobby), sp, lobby->mEventProcs[3][1]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char str_VERS_c_7FF0[] __asm__("str_VERS_c");
extern void *lobbyman_alloc_7FF0(int) __asm__("lobbyman_alloc");
extern void memset_7FF0(void *, int, int) __asm__("memset");
extern char *strchr_7FF0(char *, int) __asm__("strchr");
extern void cDirtysock_tag_TagFieldDupl_7FF0(void *, int, char *) __asm__("cDirtysock_tag_TagFieldDupl");
extern void cDirtysock_tag_TagFieldSetString_7FF0(void *, int, char *, char *) __asm__("cDirtysock_tag_TagFieldSetString");
extern int NetSession_create_7FF0(int) __asm__("NetSession_create");
extern int PingRpc_create_7FF0(void) __asm__("PingRpc_create");
extern void LobbyApi_disconnect_7FF0(void *) __asm__("LobbyApi_disconnect");

/**
 * Creates a lobby client object (1360 bytes) and returns it.
 *
 * Zeroes the block and stores the default tags (+0x34, 256 bytes): @p tagString is duplicated as tags when it contains '=',
 * otherwise stored as the "VERS" tag. Creates the lobby protocol (NetSession_create(0x8000), +0) and a ping object
 * (PingRpc_create, +4), stores the debug routine and its data (+0x548, +0x544; see LobbyApi_debugPrintf) and resets everything
 * with LobbyApi_disconnect. LobbyMan (LobbyMan_createLobbyApi) creates its mpLobbyApi with it.
 *
 * @param tagString  the default tags, or a version string, or 0
 * @param debugData  data passed to the debug routine
 * @param debugProc  the debug output routine, or 0
 * @return the lobby, or 0 when the allocation failed
 */
void *LobbyApi_create(char *tagString, int debugData, int debugProc) {
    LobbyApiRefT* lobbyRef;
    int pingRef;

    lobbyRef = (LobbyApiRefT*)lobbyman_alloc_7FF0(1360);
    if (((char*)lobbyRef) != 0) {
        memset_7FF0(lobbyRef, 0, 1360);
        if (tagString != 0) {
            if (strchr_7FF0(tagString, 61) != 0) {
                cDirtysock_tag_TagFieldDupl_7FF0(lobbyRef->mTags, 0x100, tagString);
            } else {
                cDirtysock_tag_TagFieldSetString_7FF0(lobbyRef->mTags, 0x100, str_VERS_c_7FF0, tagString);
            }
        }
        *(int*)lobbyRef = NetSession_create_7FF0(0x8000);
        pingRef = PingRpc_create_7FF0();
        lobbyRef->mpDebugData = (void*)debugData;
        lobbyRef->mpDebugProc = (void (*)(void*, const char*))debugProc;
        lobbyRef->mpPing = (void*)pingRef;
        LobbyApi_disconnect_7FF0(lobbyRef);
    }
    return ((char*)lobbyRef);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_disconnect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void lobbyman_free_80C0(void *) __asm__("lobbyman_free");
extern void LobbyApi_resetServerPing_80C0(void *) __asm__("LobbyApi_resetServerPing");
extern void *SortList_remove_80C0(int, int) __asm__("SortList_remove");
extern int HashTable_takeAny_80C0(int) __asm__("HashTable_takeAny");
extern void NetSession_disconnect_80C0(int) __asm__("NetSession_disconnect");

struct E_80C0 { int a, b, c, d, e; };
/* LobbyApiRefT (ealib/lobbyapi.h): its member names at these offsets. */
struct S_80C0 {
    int mpSession, mpPing, mEventFlags, mState;
    char pad[0x310 - 0x10];
    struct E_80C0 e[8];
    char pad2[0x53C - 0x3B0];
    char *mpRequests;
    int mPingsOutstanding;
    char pad3[0x54C - 0x544];
    int mRequestId;
};

/**
 * Disconnects the lobby and clears its state.
 *
 * Sets the state +0xC to 'offl', disconnects the protocol (NetSession_disconnect), empties and frees the entries of every open
 * list slot (+0x310), frees all pending request records (+0x53C), resets the server-ping state (LobbyApi_resetServerPing) and
 * clears the outstanding-ping count +0x540 and the request id counter +0x54C. Used by LobbyApi_create, LobbyApi_destroy and
 * LobbyMan's disconnect.
 *
 * @param lobby  the lobby
 */
void LobbyApi_disconnect(struct S_80C0 *lobby) {
    int i, entryRef;
    char *request;
    lobby->mState = 'offl';
    NetSession_disconnect_80C0(lobby->mpSession);
    for (i = 0; i < 8; i++) {
        if (lobby->e[i].a != 0) {
            while ((entryRef = HashTable_takeAny_80C0(lobby->e[i].a)) != 0) {
                lobbyman_free_80C0(SortList_remove_80C0(lobby->e[i].b, entryRef));
            }
        }
    }
    while (lobby->mpRequests != 0) {
        request = lobby->mpRequests;
        lobby->mpRequests = *(char **)(request + 0x40);
        lobbyman_free_80C0(request);
    }
    LobbyApi_resetServerPing_80C0(lobby);
    lobby->mPingsOutstanding = 0;
    lobby->mRequestId = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void LobbyApi_disconnect_81C8(void *) __asm__("LobbyApi_disconnect");
extern void HashTable_destroy_81C8(int) __asm__("HashTable_destroy");
extern void SortList_destroy_81C8(int) __asm__("SortList_destroy");
extern void PingRpc_destroy_81C8(int) __asm__("PingRpc_destroy");
extern void NetSession_destroy_81C8(int) __asm__("NetSession_destroy");
extern void lobbyman_free_81C8(void *) __asm__("lobbyman_free");

struct S_81C8 { int h; int list; int pad[3]; };
/* LobbyApiRefT's member names at these offsets. */
struct O_81C8 { int mpSession; int mpPing; char pad[0x310 - 8]; struct S_81C8 s[8]; };

void LobbyApi_destroy_impl(struct O_81C8 *self) __asm__("LobbyApi_destroy");
/**
 * Destroys a lobby made by LobbyApi_create.
 *
 * Disconnects it (LobbyApi_disconnect), destroys every open list slot's index and list (HashTable_destroy, SortList_destroy), the
 * ping object (PingRpc_destroy) and the protocol (NetSession_destroy), then frees the lobby (lobbyman_free).
 */
void LobbyApi_destroy_impl(struct O_81C8 *self) {
    int i;
    LobbyApi_disconnect_81C8(self);
    for (i = 0; i < 8; i++) {
        if (self->s[i].h != 0) {
            HashTable_destroy_81C8(self->s[i].h);
            SortList_destroy_81C8(self->s[i].list);
        }
    }
    PingRpc_destroy_81C8(self->mpPing);
    NetSession_destroy_81C8(self->mpSession);
    lobbyman_free_81C8(self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_setEventRoutine);
#ifdef SKIP_ASM
typedef struct { int a; int b; } E8248_pair;
typedef struct { char pad[0x514]; E8248_pair e[1]; } E8248_S;

/**
 * Sets one of the lobby's five event routines (+0x514 + 8 * @p i: routine, context).
 *
 * Slot 0 receives every message, 1 chat messages, 3 connection events and 4 game-session ("play") events
 * (LobbyApi_update); LobbyMan (LobbyMan_createLobbyApi) installs 0, 1, 3 and 4.
 *
 * @param table  the lobby
 * @param i  the routine slot (0-4)
 * @param callback  the routine
 * @param context  its context
 */
void LobbyApi_setEventRoutine(E8248_S *table, int i, int callback, int context) {
    table->e[i].a = callback;
    table->e[i].b = context;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_connect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cDirtysock_tag_TagFieldDupl_8260(void *, int, void *) __asm__("cDirtysock_tag_TagFieldDupl");
extern void LobbyApi_debugPrintf_8260(void *, int *, int, int) __asm__("LobbyApi_debugPrintf");
extern int LobbyApi_queueRequest_8260(void *, int, int, int) __asm__("LobbyApi_queueRequest");
extern void LobbyApi_dropRequests_8260() __asm__("LobbyApi_dropRequests");
extern void LobbyApi_resetServerPing_8260(void *) __asm__("LobbyApi_resetServerPing");
extern void NetSession_connect_8260(int, int, int) __asm__("NetSession_connect");
extern void NetSession_disconnect_8260(int) __asm__("NetSession_disconnect");
extern int NetSession_send_8260(int, int, int, void *, int) __asm__("NetSession_send");
extern int NetSession_getTick_8260(int) __asm__("NetSession_getTick");
extern int str_connecting_to_pct_08x_pct_d_8260[] __asm__("str_connecting_to_pct_08x_pct_d");

/* LobbyApiRefT (ealib/lobbyapi.h): its member names at these offsets. */
struct S_8260 {
    int mpSession, mpPing, mEventFlags, mState, mTimeout, mStateTime;
    char pad[0x34 - 0x18];
    char mTags[0x3B0 - 0x34];
    int mPort, mAddress;
    char mDownText;
    char pad2[0x50C - 0x3B9];
    int mPendingSnapshots;
};

/**
 * Starts connecting the lobby to a server.
 *
 * Stores the address (+0x3B4) and port (+0x3B0), drops pending requests (LobbyApi_dropRequests), clears the snapshot counter
 * +0x50C, resets the server-ping state and queues an 'skey' request record with the routine and context (returns -1
 * when that fails). Then reconnects the protocol (NetSession_disconnect, NetSession_connect), prints "connecting to %08x:%d" through
 * LobbyApi_debugPrintf, sends '@dir' with the default tags (+0x34), sets the state +0xC to 'rdir', the timeout +0x10 to now +
 * 120000 and +0x14 to now, and clears the server-down text +0x3B8. LobbyMan calls it with port 11000.
 *
 * @param lobby  the lobby
 * @param serverAddr  the server address
 * @param serverPort  the server port
 * @param callback  the routine to call when the session key exchange completes
 * @param context  its context
 * @return 0, or -1 when the request record could not be made
 */
int LobbyApi_connect(struct S_8260 *lobby, int serverAddr, int serverPort, int callback, int context) {
    char buf[0x100];
    lobby->mAddress = serverAddr;
    lobby->mPort = serverPort;
    LobbyApi_dropRequests_8260();
    lobby->mPendingSnapshots = 0;
    LobbyApi_resetServerPing_8260(lobby);
    if (LobbyApi_queueRequest_8260(lobby, 'skey', callback, context) == 0) {
        return -1;
    }
    NetSession_disconnect_8260(lobby->mpSession);
    NetSession_connect_8260(lobby->mpSession, lobby->mAddress, lobby->mPort);
    LobbyApi_debugPrintf_8260(lobby, str_connecting_to_pct_08x_pct_d_8260, lobby->mAddress, lobby->mPort);
    cDirtysock_tag_TagFieldDupl_8260(buf, 0x100, lobby->mTags);
    NetSession_send_8260(lobby->mpSession, 0x40646972, 0, buf, -1);
    lobby->mState = 'rdir';
    lobby->mTimeout = NetSession_getTick_8260(lobby->mpSession) + 0x1D4C0;
    lobby->mStateTime = NetSession_getTick_8260(lobby->mpSession);
    lobby->mDownText = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_setListEnabled);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void lobbyman_free_8368(void *) __asm__("lobbyman_free");
extern int SortList_create_8368(int, int, int) __asm__("SortList_create");
extern void SortList_destroy_8368(int) __asm__("SortList_destroy");
extern void *SortList_remove_8368(int, int) __asm__("SortList_remove");
extern int HashTable_create_8368(int, int) __asm__("HashTable_create");
extern void HashTable_destroy_8368(int) __asm__("HashTable_destroy");
extern int HashTable_takeAny_8368(int) __asm__("HashTable_takeAny");
extern void NetSession_send_8368(int, int, int, char *, int) __asm__("NetSession_send");
extern void strcat_8368(char *, char *) __asm__("strcat");

/* "ROOMS=", "RANKS=", "USERS=", "1", "0" (in .sdata, 8-aligned); the original used string literals */
extern char D_004A48B0[] __attribute__((aligned(8)));
extern char D_004A48B8[] __attribute__((aligned(8)));
extern char D_004A48C0[] __attribute__((aligned(8)));
extern char D_004A48C8[];
extern char D_004A48D0[];

struct S_8368 { int h; int list; int a; int b; int pad; };
struct O_8368 { int lobby; char pad[0x8]; int state; char pad2[0x300]; struct S_8368 s[8]; };

/**
 * Opens or closes one of the lobby's list slots and returns the slot's list.
 *
 * Opening (@p on nonzero) a closed slot creates its index (HashTable_create(1000, 2000)) and list (SortList_create(100, 100,
 * 0)); closing an open one frees its entries and destroys both. For slots 0 (rooms), 1 (users) and 2 (ranks), when the
 * lobby is connected (state not 'offl', 'rdir' or 'conn') it tells the server with a 'sele' command "ROOMS=1",
 * "USERS=1" or "RANKS=1" ("=0" when closing). Nothing in the build calls it.
 *
 * @param i  the list slot (0-7)
 * @param on  nonzero to open, 0 to close
 * @return the slot's list (0 when closed or @p i is out of range)
 */
int LobbyApi_setListEnabled(struct O_8368 *self, unsigned int i, int on) {
    char buf[256];
    int entryRef;

    if (i >= 8) {
        return 0;
    }
    if (i == 0) {
        __builtin_memcpy(buf, D_004A48B0, 7);
    } else if (i == 2) {
        __builtin_memcpy(buf, D_004A48B8, 7);
    } else if (i == 1) {
        __builtin_memcpy(buf, D_004A48C0, 7);
    } else {
        buf[0] = 0;
    }
    if (on) {
        if (self->s[i].h == 0) {
            int state;

            self->s[i].h = HashTable_create_8368(1000, 2000);
            self->s[i].list = SortList_create_8368(100, 100, 0);
            state = self->state;
            if (state != 'offl' && state != 'rdir' && state != 'conn' && buf[0] != 0) {
                strcat_8368(buf, D_004A48C8);
                NetSession_send_8368(self->lobby, 'sele', 0, buf, -1);
            }
        }
    }
    if (!on) {
        if (self->s[i].h != 0) {
            int state;

            while ((entryRef = HashTable_takeAny_8368(self->s[i].h)) != 0) {
                lobbyman_free_8368(SortList_remove_8368(self->s[i].list, entryRef));
            }
            HashTable_destroy_8368(self->s[i].h);
            self->s[i].h = 0;
            SortList_destroy_8368(self->s[i].list);
            self->s[i].list = 0;
            state = self->state;
            if (state != 'offl' && state != 'rdir' && state != 'conn' && buf[0] != 0) {
                strcat_8368(buf, D_004A48D0);
                NetSession_send_8368(self->lobby, 'sele', 0, buf, -1);
            }
        }
    }
    return self->s[i].list;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_openList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int HashTable_create_85B0(int, int) __asm__("HashTable_create");
extern int SortList_create_85B0(int, int, int) __asm__("SortList_create");
extern void SortList_setUserWord_85B0(int, int) __asm__("SortList_setUserWord");

struct S_85B0 { int h; int list; int a; int b; int pad; };
struct O_85B0 { char pad[0x310]; struct S_85B0 s[8]; };

int LobbyApi_openList_impl(struct O_85B0 *self, unsigned int i, int a, int b) __asm__("LobbyApi_openList");
/**
 * Opens one of the lobby's list slots with a change routine and returns its list.
 *
 * Stores the routine and context in the slot (+0x310 + 0x14 * @p i, +8 / +0xC), creates the index (HashTable_create(1000,
 * 2000)) and the list (SortList_create(100, 100, 0)) and links them (SortList_setUserWord). Does not check whether the slot was
 * already open. LobbyMan opens the room, user and rank lists (slots 0-2) and rank snapshots (slot 4) with it.
 *
 * @param i  the list slot (0-7)
 * @param callback  the routine LobbyApi_notifyListChanges calls when the list changes
 * @param context  its context
 * @return the new list, or 0 when @p i is out of range
 */
int LobbyApi_openList_impl(struct O_85B0 *self, unsigned int i, int callback, int context) {
    int listRef = 0;
    if (i < 8) {
    self->s[i].a = callback;
    self->s[i].b = context;
    self->s[i].h = HashTable_create_85B0(1000, 2000);
    self->s[i].list = SortList_create_85B0(100, 100, 0);
    SortList_setUserWord_85B0(self->s[i].list, self->s[i].h);
    listRef = self->s[i].list;
    }
    return listRef;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_setListRoutine);
#ifdef SKIP_ASM
typedef struct { int a, b, c, d, e; } Q_8650;
typedef struct { char pad[0x314]; Q_8650 q[8]; } S_8650;

/**
 * Changes a lobby list slot's change routine and context, and returns its list.
 *
 * @param table  the lobby
 * @param i  the list slot (0-7)
 * @param callback  the new routine
 * @param context  its context
 * @return the slot's list, or 0 when @p i is out of range
 */
int LobbyApi_setListRoutine(S_8650 *table, unsigned int i, int callback, int context) {
    int listRef = 0;
    if (i < 8U) {
        table->q[i].b = callback;
        table->q[i].c = context;
        listRef = table->q[i].a;
    }
    return listRef;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_closeList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef struct { int a; int b; int x; int y; int d; } E_8688;
typedef struct { char pad[0x310]; E_8688 e[8]; } T_8688;
extern int SortList_getUserWord_8688(int) __asm__("SortList_getUserWord");
extern int HashTable_takeAny_8688(int) __asm__("HashTable_takeAny");
extern void *SortList_remove_8688(int, int) __asm__("SortList_remove");
extern void lobbyman_free_8688(void *) __asm__("lobbyman_free");
extern void HashTable_destroy_8688(int) __asm__("HashTable_destroy");
extern void SortList_destroy_8688(int) __asm__("SortList_destroy");

/**
 * Destroys a list opened with LobbyApi_openList.
 *
 * Frees every entry (through the index SortList_getUserWord returns for the list), destroys the index and the list; when the
 * slot still refers to this list its index and list words are cleared. The slot's routine and context are cleared in
 * any case. LobbyMan closes its room, user, rank and snapshot lists with it.
 *
 * @param i  the list slot (0-7)
 * @param listRef  the list
 */
void LobbyApi_closeList(T_8688 *self, unsigned int i, int listRef) {
    int iter;
    int item;
    if (i < 8 && listRef != 0) {
        iter = SortList_getUserWord_8688(listRef);
        while ((item = HashTable_takeAny_8688(iter)) != 0) {
            lobbyman_free_8688(SortList_remove_8688(listRef, item));
        }
        HashTable_destroy_8688(iter);
        SortList_destroy_8688(listRef);
        if (self->e[i].b == listRef) {
            self->e[i].a = 0;
            self->e[i].b = 0;
        }
        self->e[i].x = 0;
        self->e[i].y = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_addListEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *lobbyman_alloc_8778(int) __asm__("lobbyman_alloc");
extern void memset_8778(void *, int, int) __asm__("memset");
extern int HashTable_findString_8778(int, char *) __asm__("HashTable_findString");
extern int SortList_add_8778(int, void *) __asm__("SortList_add");
extern int HashTable_addString_8778(int, char *, int) __asm__("HashTable_addString");

struct S_8778 { int h; int list; int a; int b; int pad; };
struct O_8778 { char pad[0x310]; struct S_8778 s[8]; };
struct B_8778 { char b[0xA8]; };

int LobbyApi_addListEntry_impl(struct O_8778 *self, unsigned int i, char *arg) __asm__("LobbyApi_addListEntry");
/**
 * Adds an entry to one of the lobby's keyed lists (slots 3-7), unless its name is already there.
 *
 * Slot 3 takes a name: a zeroed 312-byte record is made with the name at +8. Slots 4-7 take a 168-byte record, copied,
 * keyed by its name at +8. The record is added to the slot's list (SortList_add) and indexed by name (HashTable_addString).
 * Nothing in the build calls it.
 *
 * @param i  the list slot (3-7)
 * @param record  the name (slot 3) or the record to copy (slots 4-7)
 * @return 1 when added, 0 when the name was present, -1 when @p i is invalid or the slot is closed
 */
int LobbyApi_addListEntry_impl(struct O_8778 *self, unsigned int i, char *record) {
    if (i < 8) {
        if (self->s[i].h == 0) {
            return -1;
        }
        if (i == 3) {
            char *newRecord;
            int entryRef;
            if (HashTable_findString_8778(self->s[3].h, record) != 0) {
                return 0;
            }
            newRecord = lobbyman_alloc_8778(312);
            memset_8778(newRecord, 0, 312);
            strcpy(newRecord + 8, record);
            entryRef = SortList_add_8778(self->s[3].list, newRecord);
            HashTable_addString_8778(self->s[3].h, newRecord + 8, entryRef);
            return 1;
        }
        if (i - 4 < 4) {
            char *newRecord;
            int entryRef;
            if (HashTable_findString_8778(self->s[i].h, record + 8) != 0) {
                return 0;
            }
            newRecord = lobbyman_alloc_8778(168);
            *(struct B_8778 *)newRecord = *(struct B_8778 *)record;
            entryRef = SortList_add_8778(self->s[i].list, newRecord);
            HashTable_addString_8778(self->s[i].h, newRecord + 8, entryRef);
            return 1;
        }
        return -1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_removeListEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int HashTable_findString_8968(int, int) __asm__("HashTable_findString");
extern int HashTable_removeString_8968(int, int) __asm__("HashTable_removeString");
extern void *SortList_remove_8968(int, int) __asm__("SortList_remove");
extern void lobbyman_free_8968(void *) __asm__("lobbyman_free");

struct S_8968 { int h; int list; int a; int b; int pad; };
struct O_8968 { char pad[0x310]; struct S_8968 s[8]; };

int LobbyApi_removeListEntry_impl(struct O_8968 *self, unsigned int i, int arg) __asm__("LobbyApi_removeListEntry");
/**
 * Removes a named entry from one of the lobby's keyed lists (slots 3-7).
 *
 * Looks the name up in the slot's index (HashTable_findString), removes it (HashTable_removeString) and frees the entry taken from the
 * list (SortList_remove). Nothing in the build calls it.
 *
 * @param i  the list slot (3-7)
 * @param entryName  the entry's name
 * @return 1 when removed, 0 when not found, -1 when @p i is invalid or the slot is closed
 */
int LobbyApi_removeListEntry_impl(struct O_8968 *self, unsigned int i, int entryName) {
    int entryRef;
    if (i < 8) {
        if (self->s[i].h == 0) {
            return -1;
        }
        if (i - 3 >= 5) {
            return -1;
        }
        entryRef = HashTable_findString_8968(self->s[i].h, entryName);
        if (entryRef == 0) {
            return 0;
        }
        HashTable_removeString_8968(self->s[i].h, entryName);
        lobbyman_free_8968(SortList_remove_8968(self->s[i].list, entryRef));
        return 1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_hasListEntry);
#ifdef SKIP_ASM
extern int HashTable_findString(int, int);

/**
 * Tells whether a lobby list slot's index holds a name.
 *
 * @param lobby  the lobby
 * @param slot  the list slot (0-7)
 * @param entryName  the name or key to look up (HashTable_findString)
 * @return 1 when present, 0 when not, -1 when @p slot is invalid or closed
 */
int LobbyApi_hasListEntry(struct LobbyApiRefT *lobby, unsigned int slot, int entryName) {
    int temp_4;

    if (slot < 8U) {
        temp_4 = lobby->mLists[slot][0];
        if (temp_4 != 0) {
            return HashTable_findString(temp_4, entryName) != 0;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_getText);
#ifdef SKIP_ASM
/**
 * Returns one of the lobby's text fields by FourCC.
 *
 * 'user' or 'acct': the account name from the login reply (+0x134); 'pers': the persona (+0x174); 'room': the room name
 * (+0x1C0); 'play': the game-session record (+0x200) when its WHEN time (+0x2CC) is set; 'down': the server-down
 * message (+0x3B8) when not empty.
 *
 * @param lobby  the lobby
 * @param k  the field FourCC
 * @return the field, or 0 when unknown or empty
 */
char *LobbyApi_getText(LobbyApiRefT* lobby, int k) {
    if (k == 'user' || k == 'acct') return lobby->mAccount;
    if (k == 'pers') return lobby->mPersona;
    if (k == 'room') return lobby->mRoomName;
    if (k == 'play') return (lobby->mGameWhen == 0) ? 0 : lobby->mGameSession;
    if (k == 'down') return (lobby->mDownText[0] == 0) ? 0 : lobby->mDownText;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_clearField);
#ifdef SKIP_ASM
/**
 * Clears a lobby field by FourCC: for 'play' it zeroes the game session's WHEN time (+0x2CC), so LobbyApi_getText stops
 * returning the session. Other keys do nothing.
 *
 * @param lobby  the lobby
 * @param key  the field FourCC
 */
void LobbyApi_clearField(LobbyApiRefT* lobby, int key) {
    if (key == 'play') {
        lobby->mGameWhen = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_sendCommand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cDirtysock_tag_TagFieldDupl_8B10(char *, int, char *) __asm__("cDirtysock_tag_TagFieldDupl");
extern int cDirtysock_tag_TagFieldFind_8B10(char *, char *) __asm__("cDirtysock_tag_TagFieldFind");
extern int cDirtysock_tag_TagFieldGetNumber_8B10(int, int) __asm__("cDirtysock_tag_TagFieldGetNumber");
extern int cDirtysock_tag_TagFieldGetString_8B10(int, char *, int, char *) __asm__("cDirtysock_tag_TagFieldGetString");
extern void cDirtysock_tag_TagFieldSetCrypt_8B10(char *, int, char *, char *, char *) __asm__("cDirtysock_tag_TagFieldSetCrypt");
extern void cDirtysock_tag_TagFieldSetNumber_8B10(char *, int, char *, int) __asm__("cDirtysock_tag_TagFieldSetNumber");
extern void cDirtysock_tag_TagFieldSetString_8B10(char *, int, char *, char *) __asm__("cDirtysock_tag_TagFieldSetString");
extern void lobbyman_free_8B10(void *) __asm__("lobbyman_free");
extern int *LobbyApi_queueRequest_8B10(void *, int, int, int) __asm__("LobbyApi_queueRequest");
extern int cDirtysock_tag_TagFieldMerge_8B10(char *, int, char *) __asm__("cDirtysock_tag_TagFieldMerge");
extern void *SortList_remove_8B10(int, int) __asm__("SortList_remove");
extern int HashTable_takeAny_8B10(int) __asm__("HashTable_takeAny");
extern void LobbyCrypt_encodeText_8B10(char *, int, char *, char *, int, int) __asm__("LobbyCrypt_encodeText");
extern void NetSession_send_8B10(int, int, int, char *, int) __asm__("NetSession_send");

extern char str_empty_k[];
extern char str_PASS_b[];
extern char str_MASK[];
extern char str_CHAN_b[];

struct S_8B10 { int h; int list; int a; int b; int pad; };
/* LobbyApiRefT (ealib/lobbyapi.h): its member names at these offsets. */
struct O_8B10 {
    int mpSession, mpPing, mEventFlags, mState;
    char pad10[0x24 - 0x10];
    char mSessionKey[0x10];
    char mTags[0x310 - 0x34];
    struct S_8B10 s[8];
    char pad3B0[0x4B8 - 0x3B0];
    int mSess;
    char mMask[0x508 - 0x4BC];
    int mChannel;
    int mPendingSnapshots;
};

/**
 * Sends a command to the lobby server and returns its request id.
 *
 * Allowed only in the states 'idle', 'auth', 'acct' and 'skey' (else -1). Queues a request record with the routine and
 * context (LobbyApi_queueRequest; -2 when that fails). For 'auth', 'acct' and 'pass' the tags are copied, the lobby's default
 * tags (+0x34) merged in (cDirtysock_tag_TagFieldMerge) and PASS replaced: without a session key (+0x24 empty) by TagFieldSetCrypt
 * with the MASK text from the '@dir' reply (+0x4BC) plus a MASK tag holding its SESS number (+0x4B8); with one by '~'
 * and the password encrypted with it (LobbyCrypt_encodeText). A 'snap' command empties the list slot named by its CHAN tag
 * (4-7), clears +0x508 and counts a pending snapshot (+0x50C). The command goes out with NetSession_send. LobbyMan's
 * requests (LobbyMan_sendRequest and others) use it.
 *
 * @param cmd  the command FourCC
 * @param tags  the command's tag text
 * @param callback  the routine for the reply, or 0
 * @param context  its context
 * @return the request id, -1 in the wrong state, -2 when the record could not be made
 */
int LobbyApi_sendCommand(struct O_8B10 *self, int cmd, char *tags, int callback, int context) {
    char buf[0x200];
    char str[0x20];
    int *n;
    int st;
    int k;
    int entryRef;

    st = self->mState;
    if (st != 'idle' && st != 'auth' && st != 'acct' && st != 'skey') {
        return -1;
    }
    n = LobbyApi_queueRequest_8B10(self, cmd, callback, context);
    if (n == 0) {
        return -2;
    }
    if (cmd == 'auth' || cmd == 'acct' || cmd == 'pass') {
        cDirtysock_tag_TagFieldDupl_8B10(buf, 0x200, tags);
        cDirtysock_tag_TagFieldMerge_8B10(buf, 0x200, self->mTags);
        cDirtysock_tag_TagFieldGetString_8B10(cDirtysock_tag_TagFieldFind_8B10(buf, str_PASS_b), str + 1, 0x1F,
                                              str_empty_k);
        if (self->mSessionKey[0] == 0) {
            cDirtysock_tag_TagFieldSetCrypt_8B10(buf, 0x200, str_PASS_b, self->mMask, str + 1);
            cDirtysock_tag_TagFieldSetNumber_8B10(buf, 0x200, str_MASK, self->mSess);
        } else {
            str[0] = '~';
            LobbyCrypt_encodeText_8B10(str + 1, 0x1F, str + 1, self->mSessionKey, 0x10, 0x10);
            cDirtysock_tag_TagFieldSetString_8B10(buf, 0x200, str_PASS_b, str);
        }
        tags = buf;
    }
    if (cmd == 'snap') {
        k = cDirtysock_tag_TagFieldGetNumber_8B10(cDirtysock_tag_TagFieldFind_8B10(tags, str_CHAN_b), 0);
        if (k >= 4 && k < 8 && self->s[k].h != 0) {
            while ((entryRef = HashTable_takeAny_8B10(self->s[k].h)) != 0) {
                lobbyman_free_8B10(SortList_remove_8B10(self->s[k].list, entryRef));
            }
        }
        self->mChannel = 0;
        self->mPendingSnapshots++;
    }
    NetSession_send_8B10(self->mpSession, cmd, 0, tags, -1);
    return *n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_cancelCallbacks);
#ifdef SKIP_ASM
/**
 * Cancels the reply routine of pending lobby requests.
 *
 * Clears the routine (+0x38) of every queued request record with id @p entryId, or of all of them for -1; the records
 * stay queued. LobbyMan uses it to abandon requests it timed out.
 *
 * @param lobby  the lobby
 * @param entryId  the request id, or -1 for all
 * @return 1 when at least one record matched, 0 otherwise
 */
int LobbyApi_cancelCallbacks(void *lobby, int entryId) {
    int var_6;
    void **var_4;
    void *temp_2;
    void *temp_3;

    var_6 = 0;
    var_4 = lobby + 0x53C;
    if ((*(int *)((char*)(lobby) + (0x53C))) != 0) {
        do {
            temp_3 = *var_4;
            if (((*(int *)((char*)(temp_3) + (0))) == entryId) || (entryId == -1)) {
                (*(int *)((char*)(temp_3) + (0x38))) = 0;
                var_6 = 1;
            }
            temp_2 = *var_4;
            var_4 = temp_2 + 0x40;
        } while ((*(int *)((char*)(temp_2) + (0x40))) != 0);
    }
    return var_6;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_ping);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
/* LobbyApiRefT (ealib/lobbyapi.h): its member names at these offsets. */
typedef struct {
    int mpSession; int mpPing; int mEventFlags; int mState; char pad[0x530]; int mPingsOutstanding;
} S_8DC0;
typedef struct { int id; int f4; int addr; int t; char pad[8]; char name[0x20]; } N_8DC0;
extern N_8DC0 *LobbyApi_queueRequest_8DC0(S_8DC0 *, int, int, int) __asm__("LobbyApi_queueRequest");
extern int NetSession_getTick_8DC0(int) __asm__("NetSession_getTick");
extern char *strncpy_8DC0(char *, char *, int) __asm__("strncpy");
extern int TagFieldSetString_8DC0(char *, int, char *, char *) __asm__("cDirtysock_tag_TagFieldSetString");
extern int TagFieldSetAddress_8DC0(char *, int, char *, int) __asm__("cDirtysock_tag_TagFieldSetAddress");
extern int PingRpc_request_8DC0(int, int, char *, int) __asm__("PingRpc_request");
extern char str_NAME_b_8DC0[] __asm__("str_NAME_b");
extern char str_ADDR_b_8DC0[] __asm__("str_ADDR_b");

/**
 * Pings a host through the lobby's ping object and returns the ping's request id.
 *
 * Only in the states 'conn', 'idle', 'auth' and 'acct' (-1 otherwise), with a ping object (+4; -2) and fewer than 64
 * pings outstanding (+0x540; -3). Queues a 'ping' record (LobbyApi_queueRequest; -4 when that fails) with the address, the
 * deadline (protocol time + @p x) and the name, then sends the ping with NAME and ADDR tags (PingRpc_request) and counts
 * it. The routine gets the 'ping' event built by LobbyApi_handlePings. LobbyMan pings players with a 5000 timeout;
 * LobbyApi_pingServer pings the server.
 *
 * @param name  the host's name
 * @param addr  the host's address
 * @param x  the timeout, in protocol time units
 * @param callback  the routine for the result
 * @param context  its context
 * @return the ping's request id, or a negative error
 */
int LobbyApi_ping(S_8DC0 *self, char *name, int addr, int x, int callback, int context) {
    char buf[0x100];
    N_8DC0 *n;
    int st = self->mState;
    if (st != 'conn' && st != 'idle' && st != 'auth' && st != 'acct') return -1;
    if (self->mpPing == 0) return -2;
    if (self->mPingsOutstanding >= 0x40) return -3;
    n = LobbyApi_queueRequest_8DC0(self, 'ping', callback, context);
    if (n == 0) return -4;
    n->addr = addr;
    n->t = NetSession_getTick_8DC0(self->mpSession) + x;
    strncpy_8DC0(n->name, name, 0x1F);
    buf[0] = 0;
    TagFieldSetString_8DC0(buf, 0x100, str_NAME_b_8DC0, name);
    TagFieldSetAddress_8DC0(buf, 0x100, str_ADDR_b_8DC0, addr);
    PingRpc_request_8DC0(self->mpPing, addr, buf, 0x100);
    self->mPingsOutstanding++;
    return n->id;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_getStatus);
#ifdef SKIP_ASM
/**
 * Returns one of the lobby's status values by FourCC.
 *
 * 'stat': the state +0xC; 'rflg': the room flags +0x1BC; 'extn': the external address +0x4FC; 'locl': the local address
 * +0x500; 'inat': 1 when both addresses are known and differ (behind NAT); 'more': +0x504 (from the 'sele' reply's
 * MORE); 'slot': +0x510 (its SLOTS).
 *
 * @param lobby  the lobby
 * @param key  the value FourCC
 * @return the value, or 0 for an unknown key
 */
int LobbyApi_getStatus(LobbyApiRefT* lobby, unsigned key) {
    int extAddr, localAddr;
    if (key == 'stat') return lobby->mState;
    if (key == 'rflg') return lobby->mRoomFlags;
    if (key == 'extn') return lobby->mExternalAddr;
    if (key == 'locl') return lobby->mLocalAddr;
    if (key == 'inat') {
        extAddr = lobby->mExternalAddr;
        if (extAddr != 0) {
            localAddr = lobby->mLocalAddr;
            if (localAddr != 0)
                return extAddr != localAddr;
        }
        return 0;
    }
    if (key == 'more') return lobby->mMore;
    if (key == 'slot') return lobby->mSlots;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_takeEvent);
#ifdef SKIP_ASM
/**
 * Takes the lowest pending event flag off the lobby and returns it.
 *
 * Clears the lowest set bit of the event flags +8 (set by LobbyApi_update and LobbyApi_dropConnection) and returns that bit, 0 when
 * none is set. LobbyMan's update polls it.
 *
 * @param lobby  the lobby
 * @return the event bit taken, or 0
 */
int LobbyApi_takeEvent(LobbyApiRefT* lobby) {
    int temp_2;
    int temp_3;

    temp_2 = lobby->mEventFlags;
    temp_3 = temp_2 & (temp_2 - 1);
    lobby->mEventFlags = temp_3;
    return temp_2 ^ temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_printMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void LobbyApi_debugPrintf_8FD0(void *, const char *, ...) __asm__("LobbyApi_debugPrintf");
extern char str_recv_pct_cpct_cpct_cpct_c_pct_cpct_cpct_8FD0[] __asm__("str_recv_pct_cpct_cpct_cpct_c_pct_cpct_cpct");
extern char str_null_data_8FD0[] __asm__("str_null_data");
extern char str_binary_data_8FD0[] __asm__("str_binary_data");
extern char str_long_data_8FD0[] __asm__("str_long_data");
extern char str_pct_s_e_8FD0[] __asm__("str_pct_s_e");

/**
 * Prints an incoming lobby message for debugging.
 *
 * Prints "recv: %c%c%c%c/%c%c%c%c" through LobbyApi_debugPrintf with the event's kind and code FourCCs (kind -1 shows as
 * '****' with code 'conn', 'disc' or '----'; a 0 code shows as '----'), then the text: " [null data]" when empty, "
 * [binary data]" when it has control characters other than tab, newline and return, " [long data]" when over 240
 * characters, otherwise the text itself. LobbyMan_onincomingmsg calls it.
 *
 * @param out  the lobby (for its debug routine)
 * @param event  the event (+4 kind, +8 code, +0xC text)
 */
void LobbyApi_printMessage(void *out, int *event) {
    int eventKind = event[1];
    int eventCode = event[2];
    char *eventText = (char *)event[3];
    int n;
    int bad;

    if (eventCode == 0) eventCode = 0x2D2D2D2D;
    if (eventKind == -1) {
        eventKind = 0x2A2A2A2A;
        if (eventCode == -1) {
            eventCode = 'conn';
        } else if (eventCode == (int)0xFEFEFEFE) {
            eventCode = 'disc';
        } else {
            eventCode = 0x2D2D2D2D;
        }
    }
    LobbyApi_debugPrintf_8FD0(out, str_recv_pct_cpct_cpct_cpct_c_pct_cpct_cpct_8FD0, eventKind >> 24, eventKind >> 16, eventKind >> 8, eventKind, eventCode >> 24, eventCode >> 16, eventCode >> 8, eventCode);
    n = 0;
    bad = 0;
    for (; eventText[n] != 0 && eventText[n] < 0x7F; n++) {
        if (eventText[n] < 0x20 && !(eventText[n] == '\t' || eventText[n] == '\n') && eventText[n] != '\r') {
            bad = 1;
            break;
        }
    }
    if (n == 0) {
        LobbyApi_debugPrintf_8FD0(out, str_null_data_8FD0);
    } else if (bad == 1) {
        LobbyApi_debugPrintf_8FD0(out, str_binary_data_8FD0);
    } else if (n > 0xF0) {
        LobbyApi_debugPrintf_8FD0(out, str_long_data_8FD0);
    } else {
        LobbyApi_debugPrintf_8FD0(out, str_pct_s_e_8FD0, eventText);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_rankList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern void *lobbyman_alloc_9160(int) __asm__("lobbyman_alloc");
extern void lobbyman_free_9160(void *) __asm__("lobbyman_free");
extern void memset_9160(void *, int, int) __asm__("memset");
extern int *SortList_getAt_9160(int, int) __asm__("SortList_getAt");
extern int SortList_getCount_9160(int) __asm__("SortList_getCount");
extern int HashTable_removeString_9160(int, void *) __asm__("HashTable_removeString");
extern void *SortList_remove_9160(int, int) __asm__("SortList_remove");
extern char str_bogus_9160[] __asm__("str_bogus") __attribute__((aligned(8)));

/**
 * Ranks the entries of the lobby's rank list and drops those below a cut-off.
 *
 * Works on list slot 2 (+0x338 index, +0x33C list; -1 when not open). Builds a histogram of the scores (8192 shorts);
 * entries with a score outside 1-8191 get 500 and the name "bogus". Turning the counts into ranks from the top, once
 * more than @p limit entries are counted the lower scores are zeroed (when the list holds that many). Every remaining
 * entry gets its rank at +4; the zeroed ones are removed from the index and the list. Nothing in the build calls it.
 *
 * @param limit  how many top entries to keep
 * @return the number of ranked entries kept
 */
int LobbyApi_rankList(LobbyApiRefT* self, int limit) {
    int rankList = self->mLists[2][1];
    int rankIndex = self->mLists[2][0];
    int k, sum, res;
    short *hist;
    short *histSlot;
    int *rankEntry;
    if (rankIndex == 0 || rankList == 0) {
        return -1;
    }
    k = 0;
    hist = (short *)lobbyman_alloc_9160(0x4000);
    memset_9160(hist, 0, 0x4000);
    while ((rankEntry = SortList_getAt_9160(rankList, k)) != 0) {
        if ((unsigned)(rankEntry[0] - 1) >= 0x1FFF) {
            rankEntry[0] = 500;
            __builtin_memcpy(rankEntry + 2, str_bogus_9160, 6);
        }
        k++;
        hist[rankEntry[0]]++;
    }
    sum = 1;
    k = 0x1FFF;
    histSlot = hist + 0x1FFF;
    do {
        res = *histSlot;
        *histSlot = sum;
        sum += res;
        if (sum > limit) {
            int cnt = SortList_getCount_9160(rankList);
            if (cnt >= sum) {
                int i = 0;
                while ((rankEntry = SortList_getAt_9160(rankList, i)) != 0) {
                    if (rankEntry[0] <= k) {
                        rankEntry[0] = 0;
                    }
                    i++;
                }
            }
            break;
        }
        k--;
        histSlot--;
    } while (k >= 0);
    res = 0;
    k = 0;
    while ((rankEntry = SortList_getAt_9160(rankList, k)) != 0) {
        if (rankEntry[0] > 0) {
            res++;
            rankEntry[1] = hist[rankEntry[0]];
        }
        k++;
    }
    k = 0;
    while ((rankEntry = SortList_getAt_9160(rankList, k)) != 0) {
        if (rankEntry[0] == 0) {
            lobbyman_free_9160(SortList_remove_9160(rankList, HashTable_removeString_9160(rankIndex, rankEntry + 2)));
        } else {
            k++;
        }
    }
    lobbyman_free_9160(hist);
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_pickRoom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* A room record (LobbyApi_update's Room_95C8; names ours, from the tags). */
struct N_9378 { int id; int f4; unsigned int flags; int userLimit; int userCount; int userCountText; int flagText;
    char name[1]; };
extern char D_004A4888_9378[] __asm__("str_empty_k");
extern int SortList_update_9378(int) __asm__("SortList_update");
extern int SortList_bumpChanged(int, int);
extern int SortList_getPassCount_9378(int) __asm__("SortList_getPassCount");
extern struct N_9378 *SortList_getAt_9378(int, int) __asm__("SortList_getAt");
extern int strlen(const char *);
extern int strncmp(const char *, const char *, int);

int LobbyApi_pickRoom_impl(char *self, int limit, unsigned int mask, unsigned int val, char *name, int def) __asm__("LobbyApi_pickRoom");
/**
 * Picks a room in the lobby's room list for joining.
 *
 * Among the room-list entries (slot 0) whose flags (+8) masked with @p mask equal @p val and whose name begins with
 * @p name, prefers the fullest room below @p limit percent full (users T at +0x10 * 100 / limit L at +0xC), otherwise
 * the emptiest. It first calls SortList_bumpChanged on the room list with SortList_update's result. Nothing in the build calls
 * it.
 *
 * @param limit  the fullness limit in percent
 * @param mask  the flag bits to test
 * @param val  the value they must have
 * @param name  the room-name prefix, or 0 for any
 * @param def  the index returned when no room matches
 * @return the chosen room's index in the list, or @p def
 */
int LobbyApi_pickRoom_impl(char *self, int limit, unsigned int mask, unsigned int val, char *name, int def) {
    int i;
    int best = -1;
    int minv = 999;
    int mini = def;

    if (name == 0) {
        name = D_004A4888_9378;
    }
    if (*(int *)(self + 0x310) != 0) {
    i = 0;
    SortList_bumpChanged(*(int *)(self + 0x314), SortList_update_9378(*(int *)(self + 0x314)));
    for (; i < SortList_getPassCount_9378(*(int *)(self + 0x314)); i++) {
        struct N_9378 *n = SortList_getAt_9378(*(int *)(self + 0x314), i);
        int fullness;
        if ((n->flags & mask) != val) continue;
        if (strncmp(n->name, name, strlen(name)) != 0) continue;
        fullness = n->userCount * 100 / n->userLimit;
        if (fullness < limit) {
            if (best < fullness) {
                def = i;
                best = fullness;
            }
        }
        if (fullness < minv) {
            minv = fullness;
            mini = i;
        }
    }
    if (best < 0) {
        best = minv;
        def = mini;
    }
    }
    return def;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_onServerPing);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cDirtysock_tag_TagFieldFind_94F0(int, char *) __asm__("cDirtysock_tag_TagFieldFind");
extern int cDirtysock_tag_TagFieldGetNumber_94F0(int, int) __asm__("cDirtysock_tag_TagFieldGetNumber");
extern void LobbyApi_debugPrintf_94F0(void *, char *) __asm__("LobbyApi_debugPrintf");
extern int func_003F4330_94F0(void) __asm__("func_003F4330");
extern char str_server_ICMP_timeout_94F0[] __asm__("str_server_ICMP_timeout");
extern char str_PING_b_94F0[] __asm__("str_PING_b");

/**
 * Ping routine for the lobby's server pings (installed by LobbyApi_pingServer).
 *
 * Handles 'ping' events only. A PING of 0 (timeout) prints "server ICMP timeout" and counts a failure (+0x20): after 3
 * the count restarts at 0, otherwise the next ping is due at once (+0x18 = func_003F4330()). A nonzero PING is stored
 * (+0x1C, later reported to the server in '~png' replies) and the next ping is scheduled 30000 later with the failure
 * count cleared.
 *
 * @param lobby  the lobby
 * @param event  the event (+4 kind, +0xC tag text)
 */
void LobbyApi_onServerPing(void *lobby, void *event) {
    int n;
    if (*(int *)((char *)event + 4) == 'ping') {
        n = cDirtysock_tag_TagFieldGetNumber_94F0(cDirtysock_tag_TagFieldFind_94F0(*(int *)((char *)event + 0xC), str_PING_b_94F0), 0);
        if (n == 0) {
            LobbyApi_debugPrintf_94F0(lobby, str_server_ICMP_timeout_94F0);
            *(int *)((char *)lobby + 0x20) += 1;
            if (*(int *)((char *)lobby + 0x20) == 3) {
                *(int *)((char *)lobby + 0x20) = 0;
            } else {
                *(int *)((char *)lobby + 0x18) = func_003F4330_94F0();
            }
        } else {
            *(int *)((char *)lobby + 0x1C) = n;
            *(int *)((char *)lobby + 0x18) = func_003F4330_94F0() + 30000;
            *(int *)((char *)lobby + 0x20) = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_pingServer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void LobbyApi_ping_9590(void *, void *, int, int, void *, int) __asm__("LobbyApi_ping");
extern char str_server_9590[] __asm__("str_server");
extern char LobbyApi_onServerPing_9590[] __asm__("LobbyApi_onServerPing");

/**
 * Pings the lobby server: LobbyApi_ping with the name "@server", the server address (+0x3B4), a 3000 timeout and
 * LobbyApi_onServerPing as the routine. Called by LobbyApi_update after the redirect.
 *
 * @param lobby  the lobby
 */
void LobbyApi_pingServer(LobbyApiRefT* lobby) {
    LobbyApi_ping_9590(lobby, str_server_9590, lobby->mAddress, 3000, LobbyApi_onServerPing_9590, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct Ev_95C8 { int type; int kind; int code; char *data; };
struct Rec_95C8 {
    int f0;
    int tag;
    int f8[12];
    void (*fn)(void *, struct Ev_95C8 *, int);
    int ctx;
    struct Rec_95C8 *next;
};
struct Slot_95C8 { int h; int list; int routine; int context; int spare; };
struct Cb_95C8 { void (*fn)(void *, struct Ev_95C8 *, int); int ctx; };
struct Sess_95C8 {                  /* 0x110 */
    char name[0x20];
    char self[0x20];
    char host[0x20];
    char oppo[0x20];
    char player1[0x10];             /* the P1 .. P4 tags */
    char player2[0x10];
    char player3[0x10];
    char player4[0x10];
    int addr;
    int from;
    int seed;
    int when;
    char auth[0x40];
};
struct Room_95C8 {                  /* 0x68 */
    int id;
    int f04;
    int flags;
    int userLimit;                  /* the L tag (names ours, from the tags) */
    int userCount;                  /* the T tag */
    char userCountText[4];          /* userCount printed */
    char flagText[4];               /* "No" / "Yes" from flags bit 0x10000 */
    char name[0x20];
    char host[0x20];
    char ping[8];
    int addr;
};
struct User_95C8 {                  /* 0x138 */
    int id;
    int flags;
    char name[0x20];
    char ping[8];                   /* the P tag (names ours, from the tags) */
    int addr;
    int rank;                       /* the R tag (as Rank_95C8's; LobbyMan_getUserInfo reads it as the rank) */
    char stats[0x80];               /* the S tag */
    char extra[0x80];               /* the X tag */
};
struct Rank_95C8 {                  /* 0xA8 */
    int score;
    int rank;                       /* the R tag of a '+snp' entry, 0 for '+rnk' (names ours) */
    char name[0x20];
    char stats[0x80];               /* the S tag */
};
struct Lob_95C8 {
    int mpSession;
    int mpPing;
    unsigned int mEventFlags;
    int mState;
    int mTimeout;
    int mStateTime;
    unsigned int mNextPingTime;
    unsigned int mLastPing;
    int mPingFailures;
    char mSessionKey[0x10];
    char mTags[0x100];
    char mAccount[0x40];
    char mPersona[0x40];
    int mUserFlags;
    int mRoomId;
    int mRoomFlags;
    char mRoomName[0x40];
    struct Sess_95C8 sess;
    struct Slot_95C8 s[8];
    int mPort;
    int mAddress;
    char mDownText[0x100];
    int mSess;
    char mMask[0x40];
    int mExternalAddr;
    int mLocalAddr;
    int mMore;
    int mChannel;
    int mPendingSnapshots;
    int mSlots;
    struct Cb_95C8 cb[5];
};

extern int TagFind_95C8(char *, char *) __asm__("cDirtysock_tag_TagFieldFind");
extern int TagGetNumber_95C8(int, int) __asm__("cDirtysock_tag_TagFieldGetNumber");
extern int TagGetAddr_95C8(int, int) __asm__("cDirtysock_tag_TagFieldGetAddr");
extern int TagGetFlags_95C8(int, int) __asm__("cDirtysock_tag_TagFieldGetFlags");
extern int TagGetTime_95C8(int, int) __asm__("cDirtysock_tag_TagFieldGetTime");
extern int TagGetString_95C8(int, char *, int, char *) __asm__("cDirtysock_tag_TagFieldGetString");
extern int TagGetBinary_95C8(int, void *, int) __asm__("cDirtysock_tag_TagFieldGetBinary");
extern int TagDupl_95C8(char *, int, char *) __asm__("cDirtysock_tag_TagFieldDupl");
extern int TagSetNumber_95C8(char *, int, char *, int) __asm__("cDirtysock_tag_TagFieldSetNumber");
extern int TagSetAddress_95C8(char *, int, char *, int) __asm__("cDirtysock_tag_TagFieldSetAddress");
extern int TagSetBinary_95C8(char *, int, char *, void *, int) __asm__("cDirtysock_tag_TagFieldSetBinary");
extern void *lobbyman_alloc_95C8(int) __asm__("lobbyman_alloc");
extern void lobbyman_free_95C8(void *) __asm__("lobbyman_free");
extern void *memset_95C8(void *, int, int) __asm__("memset");
extern int sprintf_95C8(char *, char *, ...) __asm__("sprintf");
extern char *strcpy(char *, const char *);
extern void LobbyApi_debugPrintf_95C8(void *, char *, ...) __asm__("LobbyApi_debugPrintf");
extern int LobbyApi_takeRequest_95C8(void *, struct Rec_95C8 *, int) __asm__("LobbyApi_takeRequest");
extern void LobbyApi_setRoomPopulation_95C8(void *, int, int) __asm__("LobbyApi_setRoomPopulation");
extern void LobbyApi_notifyListChanges_95C8(void *) __asm__("LobbyApi_notifyListChanges");
extern void LobbyApi_handlePings_95C8(void *) __asm__("LobbyApi_handlePings");
extern void LobbyApi_dropConnection_95C8(void *, int, int) __asm__("LobbyApi_dropConnection");
extern void LobbyApi_pingServer_95C8(void *) __asm__("LobbyApi_pingServer");
extern int SortList_add_95C8(int, void *) __asm__("SortList_add");
extern void SortList_setValue_95C8(int, int, void *) __asm__("SortList_setValue");
extern void *SortList_getValue_95C8(int, int) __asm__("SortList_getValue");
extern void *SortList_remove_95C8(int, int) __asm__("SortList_remove");
extern int HashTable_takeAny_95C8(int) __asm__("HashTable_takeAny");
extern void HashTable_addString_95C8(int, char *, int) __asm__("HashTable_addString");
extern int HashTable_findString_95C8(int, char *) __asm__("HashTable_findString");
extern int HashTable_removeString_95C8(int, char *) __asm__("HashTable_removeString");
extern void HashTable_addInt_95C8(int, int, int) __asm__("HashTable_addInt");
extern int HashTable_findInt_95C8(int, int) __asm__("HashTable_findInt");
extern void HashTable_removeInt_95C8(int, int) __asm__("HashTable_removeInt");
extern void NetSession_connect_95C8(int, int, int) __asm__("NetSession_connect");
extern int NetSession_disconnect_95C8(int) __asm__("NetSession_disconnect");
extern int NetSession_getStatus_95C8(int, int) __asm__("NetSession_getStatus");
extern int NetSession_send_95C8(int, int, int, char *, int) __asm__("NetSession_send");
extern int NetSession_peek_95C8(int, int *, int *, char **) __asm__("NetSession_peek");
extern void NetSession_recv_95C8(int, int, int, int, int) __asm__("NetSession_recv");
extern unsigned int NetSession_getTick_95C8(int) __asm__("NetSession_getTick");

extern char str_connecting_to_pct_08x_pct_d_95C8[] __asm__("str_connecting_to_pct_08x_pct_d");
extern char D_00495FB0_95C8[] __asm__("D_00495FB0");
extern char str_pct_d_e_95C8[] __asm__("str_pct_d_e");
extern char str_NAME_b_95C8[] __asm__("str_NAME_b");
extern char D_004A4888_95C8[] __asm__("str_empty_k");
extern char str_ADDR_b_95C8[] __asm__("str_ADDR_b");
extern char str_MASK_95C8[] __asm__("str_MASK");
extern char str_CHAN_b_95C8[] __asm__("str_CHAN_b");
extern char D_004A4908_95C8[] __asm__("D_004A4908");
extern char D_004A4910_95C8[] __asm__("D_004A4910");
extern char D_004A4918_95C8[] __asm__("D_004A4918");
extern char D_004A4920_95C8[] __asm__("D_004A4920");
extern char D_004A4928_95C8[] __asm__("D_004A4928");
extern char D_004A4930_95C8[] __asm__("D_004A4930");
extern char D_004A4938_95C8[] __asm__("D_004A4938");
extern char D_004A4940_95C8[] __asm__("D_004A4940");
extern char D_004A4948_95C8[] __asm__("D_004A4948");
extern char D_004A4950_95C8[] __asm__("D_004A4950");
extern char D_004A4958_95C8[] __asm__("D_004A4958");
extern char D_004A4960_95C8[] __asm__("D_004A4960");
extern char D_004A4968_95C8[] __asm__("D_004A4968");
extern char D_004A4970_95C8[] __asm__("D_004A4970");
extern char D_004A4978_95C8[] __asm__("D_004A4978");
extern char D_004A4980_95C8[] __asm__("D_004A4980");
extern char D_004A4988_95C8[] __asm__("D_004A4988");
extern char D_004A4990_95C8[] __asm__("D_004A4990");
extern char D_004A4998_95C8[] __asm__("D_004A4998");
extern char D_004A49A0_95C8[] __asm__("D_004A49A0");
extern char D_004A49A8_95C8[] __asm__("D_004A49A8");
extern char D_004A49B0_95C8[] __asm__("D_004A49B0");
extern char D_004A49B8_95C8[] __asm__("D_004A49B8");
extern char D_004A49C0_95C8[] __asm__("D_004A49C0");
extern char D_004A49C8_95C8[] __asm__("D_004A49C8");
extern char D_004A49D0_95C8[] __asm__("D_004A49D0");
extern char D_004A49D8_95C8[] __asm__("D_004A49D8");
extern char D_004A49E0_95C8[] __asm__("D_004A49E0");
extern char D_004A49E8_95C8[] __asm__("D_004A49E8");
extern char D_004A49F0_95C8[] __asm__("D_004A49F0");
extern char D_004A49F8_95C8[] __asm__("D_004A49F8");
extern char D_004A4A00_95C8[] __asm__("D_004A4A00");
extern char D_004A4A08_95C8[] __asm__("D_004A4A08");
extern char D_004A4A10_95C8[] __asm__("D_004A4A10");
extern char D_004A4A18_95C8[] __asm__("D_004A4A18") __attribute__((aligned(8)));
extern char D_004A4A20_95C8[] __asm__("D_004A4A20");
extern char D_004A4A28_95C8[] __asm__("D_004A4A28");
extern char D_004A4A30_95C8[] __asm__("D_004A4A30");
extern char D_004A4A38_95C8[] __asm__("D_004A4A38");
extern char D_004A4A40_95C8[] __asm__("D_004A4A40");
extern char D_004A4A48_95C8[] __asm__("D_004A4A48");
extern char D_004A4A50_95C8[] __asm__("D_004A4A50");
extern char D_004A4A58_95C8[] __asm__("D_004A4A58");
extern char D_004A4A60_95C8[] __asm__("D_004A4A60");
extern char D_004A4A68_95C8[] __asm__("D_004A4A68");

/**
 * Runs the lobby client: timeouts, pings and every message from the server.
 *
 * Does nothing without a protocol (+0). Unless offline it tracks the inactivity timeout (+0x10; 60 s after each
 * message, a gap over 5000 extends it) and disconnects with 'time' (LobbyApi_dropConnection) when it passes; stops in the 'term'
 * and 'offl' states. Handles ping replies (LobbyApi_handlePings), then reads each message (NetSession_peek): '~png' keep-alives
 * are answered (with the last server ping as TIME), anything else goes to event routine 0 and to the matching request
 * record's routine (LobbyApi_takeRequest). The protocol flow: in 'rdir' the '@dir' reply gives the real server (ADDR, PORT,
 * SESS, MASK; or the DOWN text and flag 0x400) and moves to 'conn' with a server ping (LobbyApi_pingServer); the connection
 * sends 'skey' with "Public Key"; the 'skey' / 'sele' reply stores the session key and enters 'idle' (flag 1,
 * connection routine 3); 'auth' / 'acct' replies store the account name and external address (flag 2); 'sele' stores
 * MORE and SLOTS; 'pers' the persona. Server pushes: "+ses" fills the game session and calls routine 4; "+msg" calls
 * routine 1 with 'chat', 'cast' or 'priv'; "+who", "+rom", "+pop", "+usr", "+rnk" and "+snp" maintain the room, user,
 * rank and snapshot lists (flags 8, 0x20, 0x40, 0x80); 'move' / 'room' replies update the current room. Finally
 * notifies changed lists (LobbyApi_notifyListChanges). LobbyMan's update calls it.
 */
void LobbyApi_update(struct Lob_95C8 *self)
{
    char buf[0x100];
    struct Ev_95C8 ev;
    struct Rec_95C8 rec;
    char pop[0x200];
    int kind;
    int code;
    char *data;
    unsigned int now;
    int len;
    int entryRef;
    int id;

    if (self->mpSession == 0) {
        return;
    }
    now = NetSession_getTick_95C8(self->mpSession);
    if (self->mState != 'offl') {
        int diff = now - self->mStateTime;
        if (self->mStateTime != 0 && diff > 5000) {
            self->mTimeout += diff;
        }
        if ((int)(self->mTimeout - now) < 0) {
            LobbyApi_dropConnection_95C8(self, 'time', 0x800);
        }
    }
    self->mStateTime = now;
    if (self->mState == 'term' || self->mState == 'offl') {
        return;
    }
    LobbyApi_handlePings_95C8(self);
    while ((len = NetSession_peek_95C8(self->mpSession, &kind, &code, &data)) >= 0) {
        self->mTimeout = now + 60000;
        if (kind == 0x7E706E67) {
            if (self->mLastPing != 0xFFFFFFFF) {
                TagDupl_95C8(buf, 0x100, data);
                TagSetNumber_95C8(buf, 0x100, D_004A4908_95C8, self->mLastPing);
                data = buf;
                len = -1;
            }
            NetSession_send_95C8(self->mpSession, kind, code, data, len);
        } else {
            ev.kind = kind;
            ev.code = code;
            ev.data = data;
            ev.type = 0;
            if (self->cb[0].fn != 0) {
                self->cb[0].fn(self, &ev, self->cb[0].ctx);
            }
            LobbyApi_takeRequest_95C8(self, &rec, kind);
            if (rec.fn != 0) {
                rec.fn(self, &ev, rec.ctx);
            }
            if (self->mState == 'rdir') {
                if (kind == 0x40646972) {
                    int port = NetSession_getStatus_95C8(self->mpSession, 'lprt');
                    self->mLocalAddr = NetSession_getStatus_95C8(self->mpSession, 'ladr');
                    if (TagFind_95C8(data, D_004A4910_95C8) == 0) {
                        self->mAddress = TagGetAddr_95C8(TagFind_95C8(data, str_ADDR_b_95C8), 0);
                        self->mPort = TagGetNumber_95C8(TagFind_95C8(data, D_004A4918_95C8), 0);
                    }
                    self->mSess = TagGetNumber_95C8(TagFind_95C8(data, D_004A4920_95C8), 0);
                    TagGetString_95C8(TagFind_95C8(data, str_MASK_95C8), self->mMask, 0x40, D_004A4888_95C8);
                    NetSession_disconnect_95C8(self->mpSession);
                    if (self->mAddress != 0) {
                        self->mState = 'conn';
                        LobbyApi_pingServer_95C8(self);
                        LobbyApi_debugPrintf_95C8(self, str_connecting_to_pct_08x_pct_d_95C8, self->mAddress, self->mPort);
                        NetSession_connect_95C8(self->mpSession, self->mAddress, self->mPort);
                        buf[0] = 0;
                        TagSetAddress_95C8(buf, 0x100, str_ADDR_b_95C8, self->mLocalAddr);
                        TagSetNumber_95C8(buf, 0x100, D_004A4918_95C8, port);
                        NetSession_send_95C8(self->mpSession, 'addr', 0, buf, -1);
                        break;
                    }
                    TagGetString_95C8(TagFind_95C8(data, D_004A4928_95C8), self->mDownText, 0x100, D_004A4888_95C8);
                    self->mEventFlags |= 0x400;
                }
                kind = 0;
            }
            if (kind == -1 && code == -1) {
                buf[0] = 0;
                TagSetBinary_95C8(buf, 0x100, D_004A4930_95C8, D_00495FB0_95C8, 10);
                NetSession_send_95C8(self->mpSession, 'skey', 0, buf, -1);
                self->mMore = 1;
                self->mState = 'skey';
                kind = 'conn';
                code = 0;
            }
            if (self->mState == 'skey' && (kind == 'skey' || kind == 'sele') && code == 0) {
                if (kind == 'skey') {
                    TagGetBinary_95C8(TagFind_95C8(data, D_004A4930_95C8), self->mSessionKey, 0x10);
                }
                self->mEventFlags |= 1;
                self->mState = 'idle';
                kind = 'conn';
                code = 0;
                self->mNextPingTime = now;
                if (self->cb[3].fn != 0) {
                    ev.type = 3;
                    ev.kind = 'conn';
                    ev.code = 0;
                    ev.data = 0;
                    self->cb[3].fn(self, &ev, self->cb[3].ctx);
                }
            }
            if (kind == -1 && code == (int)0xFEFEFEFE && self->mState != 'conn') {
                LobbyApi_dropConnection_95C8(self, 0xFEFEFEFE, 0x800);
            }
            if (code == 'auth') {
                LobbyApi_dropConnection_95C8(self, 'auth', 2);
            }
            if (kind == 'auth' && code == 0) {
                self->mState = 'auth';
                self->mEventFlags |= 2;
                TagGetString_95C8(TagFind_95C8(data, str_NAME_b_95C8), self->mAccount, 0x40, D_004A4888_95C8);
                self->mExternalAddr = TagGetAddr_95C8(TagFind_95C8(data, str_ADDR_b_95C8), 0);
            }
            if (kind == 'acct' && code == 0) {
                self->mState = 'acct';
                self->mEventFlags |= 2;
            }
            if (kind == 'snap' && --self->mPendingSnapshots == 0 && code == 0) {
                self->mChannel = TagGetNumber_95C8(TagFind_95C8(data, str_CHAN_b_95C8), 0);
            }
            if (kind == 'sele' && code == 0) {
                self->mMore = TagGetNumber_95C8(TagFind_95C8(data, D_004A4938_95C8), 0);
                self->mSlots = TagGetNumber_95C8(TagFind_95C8(data, D_004A4940_95C8), 0);
            }
            if (kind == 'pers' && code == 0) {
                TagGetString_95C8(TagFind_95C8(data, D_004A4948_95C8), self->mPersona, 0x40, D_004A4888_95C8);
                self->mNextPingTime = now;
            }
            if (kind == 0x2B736573) {
                memset_95C8(&self->sess, 0, sizeof(self->sess));
                TagGetString_95C8(TagFind_95C8(data, str_NAME_b_95C8), self->sess.name, 0x20, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4950_95C8), self->sess.self, 0x20, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4958_95C8), self->sess.host, 0x20, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4960_95C8), self->sess.oppo, 0x20, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4968_95C8), self->sess.player1, 0x10, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4970_95C8), self->sess.player2, 0x10, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4978_95C8), self->sess.player3, 0x10, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4980_95C8), self->sess.player4, 0x10, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4988_95C8), self->sess.auth, 0x40, D_004A4888_95C8);
                self->sess.addr = TagGetAddr_95C8(TagFind_95C8(data, str_ADDR_b_95C8), 0);
                self->sess.from = TagGetAddr_95C8(TagFind_95C8(data, D_004A4990_95C8), 0);
                self->sess.seed = TagGetNumber_95C8(TagFind_95C8(data, D_004A4998_95C8), 0);
                self->sess.when = TagGetTime_95C8(TagFind_95C8(data, D_004A49A0_95C8), 0);
                self->mEventFlags |= 0x200;
                if (self->cb[4].fn != 0) {
                    ev.type = 4;
                    ev.kind = 'play';
                    ev.data = data;
                    ev.code = 0;
                    self->cb[4].fn(self, &ev, self->cb[4].ctx);
                }
            }
            if (kind == 0x2B6D7367) {
                struct Cb_95C8 *cb = &self->cb[1];
                if (cb->fn != 0) {
                    ev.type = 1;
                    ev.kind = 'chat';
                    ev.code = 0;
                    ev.data = data;
                    ev.code = TagGetFlags_95C8(TagFind_95C8(data, D_004A49A8_95C8), 0);
                    if (ev.code & 4) {
                        ev.kind = 'cast';
                    }
                    if (ev.code & 0x10000) {
                        ev.kind = 'priv';
                    }
                    cb->fn(self, &ev, cb->ctx);
                }
            }
            if (kind == 0x2B77686F) {
                int room;
                int count;
                TagGetString_95C8(TagFind_95C8(data, D_004A49B0_95C8), self->mPersona, 0x40, D_004A4888_95C8);
                self->mUserFlags = TagGetFlags_95C8(TagFind_95C8(data, D_004A49A8_95C8), 0);
                room = TagGetNumber_95C8(TagFind_95C8(data, D_004A49B8_95C8), 0);
                count = TagGetNumber_95C8(TagFind_95C8(data, D_004A49C0_95C8), 0);
                if (room != self->mRoomId) {
                    self->mRoomId = room;
                    LobbyApi_setRoomPopulation_95C8(self, room, count);
                    TagGetString_95C8(TagFind_95C8(data, D_004A49C8_95C8), self->mRoomName, 0x40, D_004A4888_95C8);
                    self->mRoomFlags = TagGetFlags_95C8(TagFind_95C8(data, D_004A49D0_95C8), 0);
                    self->mEventFlags |= 8;
                    if (self->s[1].h != 0) {
                        while ((entryRef = HashTable_takeAny_95C8(self->s[1].h)) != 0) {
                            lobbyman_free_95C8(SortList_remove_95C8(self->s[1].list, entryRef));
                        }
                        self->mEventFlags |= 0x40;
                    }
                }
            }
            if (kind == 0x2B726F6D && self->s[0].h != 0) {
                id = TagGetNumber_95C8(TagFind_95C8(data, D_004A49D8_95C8), -1);
                if (id >= 0) {
                    if (TagFind_95C8(data, D_004A49B0_95C8) != 0) {
                        struct Room_95C8 *roomRecord = lobbyman_alloc_95C8(sizeof(struct Room_95C8));
                        memset_95C8(roomRecord, 0, sizeof(struct Room_95C8));
                        roomRecord->id = id;
                        TagGetString_95C8(TagFind_95C8(data, D_004A49B0_95C8), roomRecord->name, 0x20, D_004A4888_95C8);
                        TagGetString_95C8(TagFind_95C8(data, D_004A49E0_95C8), roomRecord->host, 0x20, D_004A4888_95C8);
                        roomRecord->flags = TagGetFlags_95C8(TagFind_95C8(data, D_004A49A8_95C8), -1);
                        roomRecord->addr = TagGetAddr_95C8(TagFind_95C8(data, D_004A49E8_95C8), 0);
                        roomRecord->userCount = TagGetNumber_95C8(TagFind_95C8(data, D_004A49F0_95C8), 0);
                        roomRecord->userLimit = TagGetNumber_95C8(TagFind_95C8(data, D_004A49F8_95C8), 0);
                        sprintf_95C8(roomRecord->userCountText, str_pct_d_e_95C8, roomRecord->userCount);
                        strcpy(roomRecord->flagText, (roomRecord->flags & 0x10000) ? D_004A4A00_95C8 : D_004A4A08_95C8);
                        roomRecord->ping[0] = 0;
                        len = TagGetNumber_95C8(TagFind_95C8(data, D_004A4A10_95C8), -1);
                        if (len == 0) {
                            __builtin_memcpy(roomRecord->ping, D_004A4A18_95C8, 4);
                        } else if (len > 0) {
                            sprintf_95C8(roomRecord->ping, D_004A4A20_95C8, len);
                        }
                        entryRef = HashTable_findInt_95C8(self->s[0].h, roomRecord->id);
                        if (entryRef != 0) {
                            struct Room_95C8 *old = SortList_getValue_95C8(self->s[0].list, entryRef);
                            if (old != 0 && old->ping[0] >= '0' && old->ping[0] <= '9') {
                                strcpy(roomRecord->ping, old->ping);
                            }
                            lobbyman_free_95C8(old);
                            SortList_setValue_95C8(self->s[0].list, entryRef, roomRecord);
                        } else {
                            HashTable_addInt_95C8(self->s[0].h, roomRecord->id, SortList_add_95C8(self->s[0].list,
                                    roomRecord));
                        }
                    } else {
                        entryRef = HashTable_findInt_95C8(self->s[0].h, id);
                        if (entryRef != 0) {
                            HashTable_removeInt_95C8(self->s[0].h, id);
                            lobbyman_free_95C8(SortList_remove_95C8(self->s[0].list, entryRef));
                            self->mEventFlags |= 0x20;
                        }
                    }
                    self->mEventFlags |= 0x20;
                }
            }
            if (kind == 0x2B706F70 && self->s[0].h != 0) {
                char *popCursor;
                TagGetString_95C8(TagFind_95C8(data, D_004A4A28_95C8), pop, sizeof(pop), D_004A4888_95C8);
                popCursor = pop;
                while (*popCursor != 0) {
                    int count;
                    {
                        unsigned int digitChar;
                        for (id = 0; (digitChar = (unsigned char)*popCursor) - '0' < 10; popCursor++) {
                            id = id * 10 + (digitChar & 0xF);
                        }
                    }
                    if (id == 0) {
                        break;
                    }
                    if (*popCursor != 0) {
                        popCursor++;
                    }
                    {
                        unsigned int digitChar;
                        for (count = 0; (digitChar = (unsigned char)*popCursor) - '0' < 10; popCursor++) {
                            count = count * 10 + (digitChar & 0xF);
                        }
                    }
                    if (*popCursor != 0) {
                        popCursor++;
                    }
                    if (id != self->mRoomId) {
                        LobbyApi_setRoomPopulation_95C8(self, id, count);
                    }
                }
            }
            if (kind == 0x2B757372 && self->s[1].h != 0) {
                id = TagGetNumber_95C8(TagFind_95C8(data, D_004A49D8_95C8), -1);
                if (id >= 0) {
                    if (TagFind_95C8(data, D_004A49B0_95C8) != 0) {
                        struct User_95C8 *userRecord = lobbyman_alloc_95C8(sizeof(struct User_95C8));
                        memset_95C8(userRecord, 0, sizeof(struct User_95C8));
                        userRecord->id = id;
                        TagGetString_95C8(TagFind_95C8(data, D_004A49B0_95C8), userRecord->name, 0x20, D_004A4888_95C8);
                        userRecord->addr = TagGetAddr_95C8(TagFind_95C8(data, D_004A49E8_95C8), 0);
                        TagGetString_95C8(TagFind_95C8(data, D_004A4A10_95C8), userRecord->ping, 8, D_004A4888_95C8);
                        userRecord->flags = TagGetFlags_95C8(TagFind_95C8(data, D_004A49A8_95C8), 0);
                        TagGetString_95C8(TagFind_95C8(data, D_004A4A30_95C8), userRecord->stats, 0x80,
                                D_004A4888_95C8);
                        userRecord->rank = TagGetNumber_95C8(TagFind_95C8(data, D_004A49C8_95C8), 0);
                        TagGetString_95C8(TagFind_95C8(data, D_004A4A38_95C8), userRecord->extra, 0x80,
                                D_004A4888_95C8);
                        entryRef = HashTable_findInt_95C8(self->s[1].h, userRecord->id);
                        if (entryRef != 0) {
                            struct User_95C8 *old = SortList_getValue_95C8(self->s[1].list, entryRef);
                            __builtin_memcpy(old, userRecord, sizeof(struct User_95C8));
                            SortList_setValue_95C8(self->s[1].list, entryRef, old);
                            lobbyman_free_95C8(userRecord);
                            userRecord = old;
                        } else {
                            HashTable_addInt_95C8(self->s[1].h, userRecord->id, SortList_add_95C8(self->s[1].list,
                                    userRecord));
                        }
                        if (userRecord->flags & 0x200000) {
                            self->mUserFlags = userRecord->flags;
                        }
                    } else {
                        entryRef = HashTable_findInt_95C8(self->s[1].h, id);
                        if (entryRef != 0) {
                            HashTable_removeInt_95C8(self->s[1].h, id);
                            lobbyman_free_95C8(SortList_remove_95C8(self->s[1].list, entryRef));
                        }
                    }
                    LobbyApi_setRoomPopulation_95C8(self, self->mRoomId, TagGetNumber_95C8(TagFind_95C8(data, D_004A49F0_95C8), 0));
                    self->mEventFlags |= 0x40;
                }
            }
            if (kind == 0x2B726E6B && self->s[2].h != 0) {
                if (TagGetNumber_95C8(TagFind_95C8(data, D_004A4A40_95C8), 0) == -2) {
                    while ((entryRef = HashTable_takeAny_95C8(self->s[2].h)) != 0) {
                        lobbyman_free_95C8(SortList_remove_95C8(self->s[2].list, entryRef));
                    }
                } else {
                    struct Rank_95C8 *k = lobbyman_alloc_95C8(sizeof(struct Rank_95C8));
                    k->rank = 0;
                    k->score = TagGetNumber_95C8(TagFind_95C8(data, D_004A49E8_95C8), 0);
                    TagGetString_95C8(TagFind_95C8(data, D_004A49B0_95C8), k->name, 0x20, D_004A4888_95C8);
                    TagGetString_95C8(TagFind_95C8(data, D_004A4A30_95C8), k->stats, 0x80, D_004A4888_95C8);
                    if (k->score >= 0x2000) {
                        k->score = 0x1FFF;
                    }
                    entryRef = HashTable_findString_95C8(self->s[2].h, k->name);
                    if (entryRef == 0 && k->score > 0) {
                        HashTable_addString_95C8(self->s[2].h, k->name, SortList_add_95C8(self->s[2].list, k));
                    } else if (entryRef != 0) {
                        if (k->score > 0) {
                            struct Rank_95C8 *old = SortList_getValue_95C8(self->s[2].list, entryRef);
                            __builtin_memcpy(old, k, sizeof(struct Rank_95C8));
                            SortList_setValue_95C8(self->s[2].list, entryRef, old);
                        } else {
                            lobbyman_free_95C8(SortList_remove_95C8(self->s[2].list, HashTable_removeString_95C8(self->s[2].h,
                                    k->name)));
                        }
                        lobbyman_free_95C8(k);
                    } else {
                        lobbyman_free_95C8(k);
                    }
                }
                self->mEventFlags |= 0x80;
            }
            if (kind == 0x2B736E70 && self->mChannel >= 4 && self->mChannel < 8 && self->s[self->mChannel].h != 0) {
                struct Rank_95C8 *k = lobbyman_alloc_95C8(sizeof(struct Rank_95C8));
                k->rank = TagGetNumber_95C8(TagFind_95C8(data, D_004A49C8_95C8), 0);
                k->score = TagGetNumber_95C8(TagFind_95C8(data, D_004A4A10_95C8), 0);
                TagGetString_95C8(TagFind_95C8(data, D_004A49B0_95C8), k->name, 0x20, D_004A4888_95C8);
                TagGetString_95C8(TagFind_95C8(data, D_004A4A30_95C8), k->stats, 0x80, D_004A4888_95C8);
                if (k->score >= 0x2000) {
                    k->score = 0x1FFF;
                }
                entryRef = HashTable_findString_95C8(self->s[self->mChannel].h, k->name);
                if (entryRef == 0) {
                    int ref = SortList_add_95C8(self->s[self->mChannel].list, k);
                    HashTable_addString_95C8(self->s[self->mChannel].h, k->name, ref);
                } else {
                    struct Rank_95C8 *old = SortList_getValue_95C8(self->s[self->mChannel].list, entryRef);
                    __builtin_memcpy(old, k, sizeof(struct Rank_95C8));
                    SortList_setValue_95C8(self->s[self->mChannel].list, entryRef, old);
                    lobbyman_free_95C8(k);
                }
                self->mEventFlags |= 0x80;
            }
            if ((kind == 'move' || kind == 'room') && code == 0) {
                int ident = TagGetNumber_95C8(TagFind_95C8(data, D_004A4A48_95C8), -1);
                if (ident >= 0) {
                    LobbyApi_setRoomPopulation_95C8(self, ident, TagGetNumber_95C8(TagFind_95C8(data, D_004A4A50_95C8), 0));
                }
            }
            if (kind == 'move') {
                int room = TagGetNumber_95C8(TagFind_95C8(data, D_004A4A58_95C8), 0);
                int count = TagGetNumber_95C8(TagFind_95C8(data, D_004A4A60_95C8), 0);
                if (room != self->mRoomId) {
                    self->mRoomId = room;
                    LobbyApi_setRoomPopulation_95C8(self, room, count);
                    TagGetString_95C8(TagFind_95C8(data, str_NAME_b_95C8), self->mRoomName, 0x40, D_004A4888_95C8);
                    self->mRoomFlags = TagGetFlags_95C8(TagFind_95C8(data, D_004A4A68_95C8), 0);
                    self->mEventFlags |= 8;
                    if (self->s[1].h != 0) {
                        while ((entryRef = HashTable_takeAny_95C8(self->s[1].h)) != 0) {
                            lobbyman_free_95C8(SortList_remove_95C8(self->s[1].list, entryRef));
                        }
                        self->mEventFlags |= 0x40;
                    }
                }
            }
        }
        NetSession_recv_95C8(self->mpSession, 0, 0, 0, 0);
    }
    LobbyApi_notifyListChanges_95C8(self);
    if (self->mNextPingTime != 0 && self->mNextPingTime <= now) {
        LobbyApi_pingServer_95C8(self);
        self->mNextPingTime = now + 30000;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_parseDecimal);
#ifdef SKIP_ASM
/**
 * Reads an unsigned decimal number from the start of a string.
 *
 * Accumulates the leading digits into *@p out (0 when there are none) and returns the position after them. The tag time
 * parser (cDirtysock_tag_TagFieldGetTime) reads its date fields with it.
 *
 * @param str  the text
 * @param out  receives the number
 * @return the first character after the digits
 */
unsigned char *LobbyApi_parseDecimal(unsigned char *str, int *out) {
    unsigned int chr;

    *out = 0;
    chr = *str;
    if (chr - 0x30 < 10) {
        do {
            str++;
            *out = *out * 10 + (chr & 0xF);
            chr = *str;
        } while (chr - 0x30 < 10);
    }
    return str;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_secondsToCalendar);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern int LobbyApi_monthDaysTable[];

struct sTm_3EAEB8 {
    int sec;
    int min;
    int hour;
    int mday;
    int mon;
    int year;
    int wday;
    int yday;
    int isdst;
};

/**
 * Converts seconds since 1970 into calendar fields (UTC) and returns @p tm.
 *
 * Fills seconds, minutes, hours, day of the month (1-based), month (0-based), year since 1900 and day of the year; the
 * weekday is not set and isdst is 0. Leap years follow the Gregorian rules; the month lengths come from LobbyApi_monthDaysTable.
 * Used by the tag time routines (TagFieldSetEpoch and others) and by LobbyApi_calendarToSeconds.
 *
 * @param tm  the 9-word time structure to fill
 * @param seconds  seconds since 1970-01-01 (unsigned)
 * @return @p tm
 */
// Seconds since 1970 -> broken-down time (no wday). LobbyApi_monthDaysTable holds two rows of month
// lengths: row 0 is picked for a 366-day year, row 1 for a 365-day year.
struct sTm_3EAEB8* LobbyApi_secondsToCalendar(struct sTm_3EAEB8* tm, long seconds)
{
    int days;
    int rem;
    int y = 1970;
    int ylen;
    int* ip;
    days = (unsigned int)seconds / 86400;
    rem = (unsigned int)seconds % 86400;
    tm->sec = rem % 60;
    rem /= 60;
    tm->min = rem % 60;
    tm->hour = rem / 60;
    for (;;) {
        ylen = ((y & 3) == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
        if (days < ylen) {
            break;
        }
        {
            int yg = y + days / 366;
            if (yg == y) {
                yg++;
            }
            days -= (yg - y) * 365;
            days += ((y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400);
            days -= ((yg - 1) / 4 - (yg - 1) / 100 + (yg - 1) / 400);
            y = yg;
        }
    }
    tm->year = y - 1900;
    tm->yday = days;
    tm->mon = 0;
    ip = LobbyApi_monthDaysTable + (ylen & 1) * 12;
    while (days >= *ip) {
        days -= *ip;
        ip++;
        tm->mon++;
    }
    tm->mday = days + 1;
    tm->isdst = 0;
    return tm;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", LobbyApi_calendarToSeconds);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void LobbyApi_secondsToCalendar_B090(int *, unsigned long) __asm__("LobbyApi_secondsToCalendar");
/**
 * Converts calendar fields back into seconds since 1970 by binary search.
 *
 * Searches 0 to 0xFFFFFFFF with LobbyApi_secondsToCalendar, comparing year, month, day, hour, minute and second in that order. Used
 * by cDirtysock_tag_TagFieldGetTime and two other tag time helpers.
 *
 * @param key  the time fields (as LobbyApi_secondsToCalendar fills them)
 * @return the matching seconds, or 0 when the fields name no exact time
 */
unsigned long LobbyApi_calendarToSeconds(int *key) {
    int buf[12];
    unsigned int lo = 0;
    unsigned int hi = 0xFFFFFFFF;
    unsigned int mid;
    int diff;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        LobbyApi_secondsToCalendar_B090(buf, mid);
        if ((diff = buf[5] - key[5]) != 0 || (diff = buf[4] - key[4]) != 0 ||
            (diff = buf[3] - key[3]) != 0 || (diff = buf[2] - key[2]) != 0 ||
            (diff = buf[1] - key[1]) != 0 || (diff = buf[0] - key[0]) != 0) {
            if (diff > 0) hi = mid - 1;
            else lo = mid + 1;
        } else {
            break;
        }
    }
    if (diff != 0) return 0;
    return mid;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2DCAE8", func_003EB190);
#ifdef SKIP_ASM
/**
 * Returns 0. Nothing in the build references it.
 *
 * @return 0
 */
int func_003EB190(void) {
    return 0;
}
#endif
