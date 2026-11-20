#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("util/statemachine", cConsoleConfig_setTimeString);
#ifdef SKIP_ASM
extern "C" int sprintf2(char* buf, const char* fmt, ...);
extern "C" void func_002C25B8(char* dst, const char* src, int size);
extern char str_pct_02d_pct_02d_pct_02d_c[];
extern char str_pct_02d_pct_02d_pct_02d_AM[];
extern char str_pct_02d_pct_02d_pct_02d_PM[];

/**
 * Formats a time of day as "hh:mm:ss", in 24-hour or AM/PM form.
 *
 * Mode 0 prints the 24-hour time. Mode 1 adds " AM" before noon; from 12 on it adds " PM" and prints
 * hour % 13 - 12 as the hour, which gives 0 for 12 and negative values from 13 on. Any other mode copies an
 * uninitialised buffer. The text goes to @p dst through func_002C25B8(), a bounded copy of 16-bit units.
 *
 * @param dst   destination buffer
 * @param size  most units func_002C25B8() copies
 * @param hour  hour, 0-23
 * @param min   minutes
 * @param sec   seconds
 * @param mode  0 for 24-hour time, 1 for AM/PM
 */
extern "C" void cConsoleConfig_setTimeString(void* self, char* dst, int size, int hour, int min, int sec, int mode)
{
    char buf[0x40];
    switch (mode) {
    case 0:
        sprintf2(buf, str_pct_02d_pct_02d_pct_02d_c, hour, min, sec);
        break;
    case 1:
        if (hour < 12) {
            sprintf2(buf, str_pct_02d_pct_02d_pct_02d_AM, hour, min, sec);
        } else {
            sprintf2(buf, str_pct_02d_pct_02d_pct_02d_PM, hour % 13 - 12, min, sec);
        }
        break;
    }
    func_002C25B8(dst, buf, size);
}
#endif

//100%
INCLUDE_ASM("util/statemachine", func_002C7038);
#ifdef SKIP_ASM
extern char str_pct_04d_pct_02d_pct_02d[];
extern char str_pct_02d_pct_02d_pct_04d[];

/**
 * Formats a date into @p dst, in one of three orders.
 *
 * Mode 0 prints "yyyy:mm:dd" (str_pct_04d_pct_02d_pct_02d, year, month, day), mode 1 "mm:dd:yyyy" and mode 2
 * "dd:mm:yyyy" (both str_pct_02d_pct_02d_pct_04d). Any other mode copies an uninitialised buffer. The text goes to
 * @p dst through func_002C25B8(), a bounded copy of 16-bit units, as in cConsoleConfig_setTimeString.
 *
 * @param dst  destination buffer
 * @param size  most units func_002C25B8() copies
 * @param day  day of the month
 * @param month  month, 1-12
 * @param year  year
 * @param mode  0 year first, 1 month first, 2 day first
 */
extern "C" void func_002C7038(void* self, char* dst, int size, int day, int month, int year, int mode)
{
    char buf[0x40];
    switch (mode) {
    case 0:
        sprintf2(buf, str_pct_04d_pct_02d_pct_02d, year, month, day);
        break;
    case 1:
        sprintf2(buf, str_pct_02d_pct_02d_pct_04d, month, day, year);
        break;
    case 2:
        sprintf2(buf, str_pct_02d_pct_02d_pct_04d, day, month, year);
        break;
    }
    func_002C25B8(dst, buf, size);
}
#endif
//100%
INCLUDE_ASM("util/statemachine", func_002C7100);
#ifdef SKIP_ASM
extern "C" void func_002C73C0(void*);

/**
 * Brings the console clock up to date (func_002C73C0) and returns the time: hour (+0x8), minute (+0xC) and second
 * (+0x10).
 *
 * @param hour  receives the hour
 * @param minute  receives the minute
 * @param second  receives the second
 */
extern "C" void func_002C7100(void *self, int *hour, int *minute, int *second) {
    func_002C73C0(self);
    *hour = (*(int *)((char*)(self) + (8)));
    *minute = (*(int *)((char*)(self) + (0xC)));
    *second = (*(int *)((char*)(self) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("util/statemachine", func_002C7160);
#ifdef SKIP_ASM
extern "C" void func_002C73C0(void*);

/**
 * Brings the console clock up to date (func_002C73C0) and returns the date: day (+0x1C), month (+0x18) and year
 * (+0x14).
 *
 * @param day  receives the day of the month
 * @param month  receives the month
 * @param year  receives the year
 */
extern "C" void func_002C7160(void *self, int *day, int *month, int *year) {
    func_002C73C0(self);
    *day = (*(int *)((char*)(self) + (0x1C)));
    *month = (*(int *)((char*)(self) + (0x18)));
    *year = (*(int *)((char*)(self) + (0x14)));
}
#endif
//100%
INCLUDE_ASM("util/statemachine", func_002C71C0);
#ifdef SKIP_ASM
/**
 * Tells whether @p year is a Gregorian leap year: divisible by 4, and not by 100 unless also by 400.
 *
 * @param year  the year
 * @return 1 for a leap year, else 0
 */
extern "C" int func_002C71C0(void* self, int year) {
    int leap;

    leap = 0;
    if (!(year & 3)) {
        leap = 1;
        if ((year % 100) == 0) {
            leap = (year % 400) == 0;
        }
    }
    return leap;
}
#endif
//100%
INCLUDE_ASM("util/statemachine", func_002C7208);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_002C71C0(void* self, int a1);

/**
 * Returns the number of days in @p month of @p year: 28 or 29 for February (func_002C71C0), otherwise 31 or 30 by the
 * alternating rule shifted after July (31 for odd months up to 7 and even months from 8 on).
 *
 * @param month  month, 1-12
 * @param year  year, for February
 * @return days in that month
 */
extern "C" int func_002C7208(int self, int month, int year)
{
    int m = month;
    if (month != 2) {
        if (m >= 8) m++;
        return (m & 1) ? 31 : 30;
    }
    m = func_002C71C0((void*)self, year);
    return m == 0 ? 28 : 29;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/statemachine", func_002C7260);
#ifdef SKIP_ASM
extern "C" int func_002C7208(int, int, int);

/**
 * Adds @p count days to the date @p day / @p month / @p year, carrying into the next month and year.
 *
 * Moves a month at a time (func_002C7208 gives each month's length), restarting at day 1 and wrapping month 13 to 1 of
 * the next year. Does nothing for @p count <= 0.
 *
 * @param day  day of the month, updated
 * @param month  month, updated
 * @param year  year, updated
 * @param count  days to add
 */
extern "C" void func_002C7260(int self, int *day, int *month, int *year, int count) {
    int dim;
    int newMonth;
    int cur;
    int rest;

    if (count > 0) {
        do {
            dim = func_002C7208(self, *month, *year);
            cur = *day;
            if (dim < cur + count) {
                rest = count - 1;
                *day = 1;
                count = rest - (dim - cur);
                newMonth = *month + 1;
                *month = newMonth;
                if (newMonth >= 13) {
                    *month = 1;
                    *year += 1;
                }
            } else {
                *day = cur + count;
                count = 0;
            }
        } while (count > 0);
    }
}
#endif
//100%
INCLUDE_ASM("util/statemachine", func_002C7328);
#ifdef SKIP_ASM
/**
 * Adds @p add seconds to the time @p hour : @p min : @p sec and returns the whole days that overflowed.
 *
 * Converts to seconds, adds, splits off the days (86400 s each) and writes back hour, minute and second. Negative
 * totals are not handled.
 *
 * @param sec  seconds, updated
 * @param min  minutes, updated
 * @param hour  hours, updated
 * @param add  seconds to add
 * @return the number of days carried
 */
extern "C" int func_002C7328(void* self, int* sec, int* min, int* hour, int add) {
    int total;
    int rem;
    int days;

    total = *hour * 3600 + *min * 0x3C + *sec + add;
    days = total / 86400;
    rem = total % 86400;
    *hour = (rem / 3600) % 24;
    *min = (rem / 60) % 60;
    *sec = rem % 60;
    return days;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/statemachine", func_002C73C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002C7328_v(void*, int*, int*, int*, int) __asm__("func_002C7328");
extern "C" void func_002C7260_v(void*, int*, int*, int*, int) __asm__("func_002C7260");

/**
 * Advances the console clock by the whole seconds elapsed since the last advance.
 *
 * The elapsed seconds are (SSXApp + 0x18, the current tick - the start tick +0x20) / SSXApp + 0x10 (ticks per second).
 * When at least one has passed it sets the start tick to now (so the sub-second remainder is dropped and the clock
 * slowly falls behind), adds them to second/minute/hour (func_002C7328 on +0x10/+0xC/+0x8) and adds the carried days to
 * day/month/year (func_002C7260 on +0x1C/+0x18/+0x14).
 */
extern "C" void func_002C73C0(void* self) {
    int rem;
    int now;
    int ticks;

    now = *(int*)((char*)SSXApp + 0x18);
    ticks = (now - *(int*)((char*)self + 0x20)) / *(int*)((char*)SSXApp + 0x10);
    if (ticks != 0) {
        *(int*)((char*)self + 0x20) = now;
        rem = func_002C7328_v(self, (int*)((char*)self + 0x10), (int*)((char*)self + 0xC), (int*)((char*)self + 8), ticks);
        if (rem != 0) {
            func_002C7260_v(self, (int*)((char*)self + 0x1C), (int*)((char*)self + 0x18), (int*)((char*)self + 0x14), rem);
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/statemachine", cConsoleConfig_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2ConsoleConfig_cPS2ConsoleConfig(void* self);
extern char str_cPS2ConsoleConfig[];

/**
 * Allocates and constructs the console configuration object.
 *
 * Allocates 0x38 bytes named "cPS2ConsoleConfig" and runs cPS2ConsoleConfig_cPS2ConsoleConfig() on them, which fills the object from
 * the console's system settings (probably the PS2 system configuration and clock). The app keeps the result in
 * ConsoleConfig.
 *
 * @return the new console configuration object
 */
extern "C" void* cConsoleConfig_construct(void)
{
    return cPS2ConsoleConfig_cPS2ConsoleConfig(cMemMan_alloc(0x38, str_cPS2ConsoleConfig, 0, 0));
}
#endif

//100%
INCLUDE_ASM("util/statemachine", cPS2ConsoleConfig_cPS2ConsoleConfig);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int func_0040BAA8(void);
extern "C" int func_0040B9E8(void);
extern "C" int func_0040BB38(void);
extern "C" int func_0040BC08(void);
extern "C" int func_00402520(unsigned char* clock);
extern "C" void func_0040BFE0(unsigned char* clock);
extern char cPS2ConsoleConfig_vtbl[];
extern void* D_004A28A8_7478 __asm__("SSXApp");

/**
 * Constructs the PS2 console configuration object from the system settings and clock.
 *
 * Clears +0x24 and points +0x34 at cPS2ConsoleConfig_vtbl. From the OSD configuration it stores 1 at +0x28 when
 * func_0040BAA8 (config bit 0) returns 0, else 0; 1 at +0x2C; the language from func_0040B9E8 (0 to 7, otherwise 1) at
 * +0x30; func_0040BB38's 0 to 2 (else 0) at +0x0 and func_0040BC08's 0 or 1 at +0x4. When func_00402520 reads the
 * real-time clock, func_0040BFE0 converts it from Japan time to the console's time zone and the BCD bytes give the
 * hour, minute and second (+0x8 to +0x10) and the year (+ 2000), month and day (+0x14 to +0x1C); otherwise the date
 * defaults to 2003-10-14 at 00:00:00. +0x20 takes the word at SSXApp+0x18.
 *
 * @param self_  the memory for the object (this)
 * @return this
 */
extern "C" void* cPS2ConsoleConfig_cPS2ConsoleConfig(void* self_)
{
    int* self = (int*)self_;
    unsigned char clock[8];
    self[9] = 0;
    *(char**)&self[13] = cPS2ConsoleConfig_vtbl;
    int osdSetting = func_0040BAA8();
    switch (osdSetting) {
    case 0:
        self[10] = 1;
        break;
    case 1:
        self[10] = 0;
        break;
    default:
        self[10] = 0;
        break;
    }
    self[11] = 1;
    switch (func_0040B9E8()) {
    case 0:
        self[12] = 0;
        break;
    case 1:
        self[12] = 1;
        break;
    case 2:
        self[12] = 2;
        break;
    case 3:
        self[12] = 3;
        break;
    case 4:
        self[12] = 4;
        break;
    case 5:
        self[12] = 5;
        break;
    case 6:
        self[12] = 6;
        break;
    case 7:
        self[12] = 7;
        break;
    default:
        self[12] = 1;
        break;
    }
    osdSetting = func_0040BB38();
    switch (osdSetting) {
    case 0:
        self[0] = 0;
        break;
    case 1:
        self[0] = osdSetting;
        break;
    case 2:
        self[0] = osdSetting;
        break;
    default:
        self[0] = 0;
        break;
    }
    osdSetting = func_0040BC08();
    switch (osdSetting) {
    case 0:
        self[1] = 0;
        break;
    case 1:
        self[1] = 1;
        break;
    default:
        self[1] = 0;
        break;
    }
    if (func_00402520(clock) != 0) {
        func_0040BFE0(clock);
        self[2] = (clock[3] & 0xF) + (clock[3] >> 4) * 10;
        self[3] = (clock[2] & 0xF) + (clock[2] >> 4) * 10;
        self[4] = (clock[1] & 0xF) + (clock[1] >> 4) * 10;
        self[5] = (clock[7] & 0xF) + (clock[7] >> 4) * 10 + 2000;
        self[6] = (clock[6] & 0xF) + (clock[6] >> 4) * 10;
        self[7] = (clock[5] & 0xF) + (clock[5] >> 4) * 10;
    } else {
        self[5] = 2003;
        self[6] = 10;
        self[7] = 14;
        self[2] = 0;
        self[3] = 0;
        self[4] = 0;
    }
    self[8] = *(int*)((char*)D_004A28A8_7478 + 0x18);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/statemachine", func_002C76C0);
#ifdef SKIP_ASM
// GPL-3.0: EA EAC HUFF_decompress. Its structure, macros and variable names follow huffdecode.cpp from EA's
// Command & Conquer Generals Zero Hour source release (Electronic Arts, published under the GNU General Public
// License, version 3), in the older version SSX 3 links (no 0x8000 4-byte-size header, no `if (n)` guard in
// SQgetbits). This function stays under GPL v3; the repository's CC0 dedication does not apply to it (README, License).
// EA HUFF codec decoder (huffman with run/delta clues).
#define HUFF_GET16BITS_76C0()                                   \
    bitsunshifted = qs[0] | (bitsunshifted << 8);              \
    bitsunshifted = qs[1] | (bitsunshifted << 8);              \
    qs += 2;

#define HUFF_REFILL_76C0()                                      \
    if (bitsleft < 0) {                                         \
        HUFF_GET16BITS_76C0()                                   \
                                                                \
        bits = bitsunshifted << (-bitsleft);                    \
        bitsleft += 16;                                         \
    }

#define HUFF_GETBITS_76C0(v, n)                                 \
    v = bits >> (32 - (n));                                     \
    bits <<= (n);                                               \
    bitsleft -= (n);                                            \
    HUFF_REFILL_76C0()

#define HUFF_GETNUM_76C0(v)                                     \
    if ((int)bits < 0) {                                        \
        HUFF_GETBITS_76C0(v, 3);                                \
        v -= 4;                                                 \
    } else {                                                    \
        int n;                                                  \
        unsigned int v1;                                        \
                                                                \
        if (bits >> 16) {                                       \
            n = 2;                                              \
            do {                                                \
                bits <<= 1;                                     \
                ++n;                                            \
            } while ((int)bits >= 0);                           \
            bits <<= 1;                                         \
            bitsleft -= (n - 1);                                \
            HUFF_REFILL_76C0()                            \
        } else {                                                \
            n = 2;                                              \
            do {                                                \
                ++n;                                            \
                HUFF_GETBITS_76C0(v, 1);                        \
            } while (!v);                                       \
        }                                                       \
        if (n > 16) {                                           \
            HUFF_GETBITS_76C0(v, n - 16);                       \
            HUFF_GETBITS_76C0(v1, 16);                          \
            v = (v1 | (v << 16)) + (1 << n) - 4;                \
        } else {                                                \
            HUFF_GETBITS_76C0(v, n);                            \
            v = (v + (1 << n) - 4);                             \
        }                                                       \
    }

/* note: requires 'amt' be a multiple of 16 */
#define HUFF_MEMSET_76C0(ptr, val, amt)                         \
    {                                                           \
        int i = amt / 16;                                       \
        int intval = (val << 24) | (val << 16) | (val << 8) | val; \
        int* lptr = (int*)ptr;                                  \
                                                                \
        while (i) {                                             \
            lptr[0] = intval;                                   \
            lptr[1] = intval;                                   \
            lptr[2] = intval;                                   \
            lptr[3] = intval;                                   \
            lptr += 4;                                          \
            --i;                                                \
        }                                                       \
    }

/**
 * Decompresses an EA HUFF stream from @p packbuf into @p unpackbuf and returns the uncompressed length.
 *
 * EA's HUFF_decompress (Huffman codes plus a clue byte for run lengths and explicit bytes): reads the 16-bit type (bit
 * 0x100: skip a stored length), the 24-bit unpacked length and the clue byte, the per-bit-length code counts and the
 * leapfrog-coded symbol table, builds 8-bit fast lookup tables, then decodes with an 8-bit fast path and a 16-bit
 * compare path. The clue introduces a run of the previous byte, the end of the stream, or an explicit byte. Type 13051
 * then undoes a byte delta and 13563 a double delta. A null @p packbuf returns 0. func_002C85A0 calls it.
 *
 * @param packbuf  compressed data
 * @param unpackbuf  output buffer, large enough for the unpacked length
 * @return the unpacked length
 */
// FAKEMATCH: stand-in constant `leap[nextchar] = !leapdelta;` for the leap-table store's literal 1; plain form 99.63%.
// leapdelta is always 0 once `while (leapdelta)` exits, so the value is 1 and behaviour is unchanged. cse1 leaves `!leapdelta`
// unfolded, so loop.c sees no invariant QImode 1 for combine_movables to merge into the SImode 1 of GETNUM's `1 << n`; that 1
// is hoisted alone (savings 2), and cse2 folds the store value to a block-local `li 1` via the loop-exit jump equivalence.
// Decodes a HUFF stream from packbuf into unpackbuf; returns the uncompressed length.
// Same layout as EA's EAC HUFF_decompress, without the 4-byte size (0x8000) header form.
extern "C" int func_002C76C0(unsigned char* packbuf, unsigned char* unpackbuf)
{
    unsigned int type;
    unsigned char clue;
    int ulen;
    unsigned int cmp;
    int bitnum = 0;
    int cluelen = 0;
    unsigned char* qs;
    unsigned char* qd;
    unsigned int bits;
    unsigned int bitsunshifted = 0;
    int numbits;
    int bitsleft;
    unsigned int v;

    qs = packbuf;
    qd = unpackbuf;
    ulen = 0L;

    if (qs) {
        {
            int mostbits;
            int i;
            int bitnumtbl[16];
            unsigned int deltatbl[16];
            unsigned int cmptbl[16];
            unsigned char codetbl[256];
            unsigned char quickcodetbl[256];
            unsigned char quicklentbl[256];

            bitsleft = -16; /* init bit stream */
            bits = 0;
            HUFF_REFILL_76C0()

            HUFF_GETBITS_76C0(type, 16);

            if (type & 0x100) { /* skip ulen */
                HUFF_GETBITS_76C0(v, 8);
                HUFF_GETBITS_76C0(v, 16);
            }
            type &= ~0x100;

            HUFF_GETBITS_76C0(v, 8); /* unpack len */
            HUFF_GETBITS_76C0(ulen, 16);
            ulen |= (v << 16);

            {
                {
                    int numchars;

                    {
                        unsigned int basecmp;

                        {
                            unsigned int t;
                            HUFF_GETBITS_76C0(t, 8); /* clue byte */
                            clue = (unsigned char)t;
                        }

                        numchars = 0;
                        numbits = 1;
                        basecmp = (unsigned int)0;

                        /* decode bitnums */
                        do {
                            basecmp <<= 1;
                            deltatbl[numbits] = basecmp - numchars;

                            HUFF_GETNUM_76C0(bitnum); /* # of codes of n bits */
                            bitnumtbl[numbits] = bitnum;

                            numchars += bitnum;
                            basecmp += bitnum;

                            cmp = 0;
                            if (bitnum) /* left justify cmp */
                                cmp = (basecmp << (16 - numbits) & 0xffff);

                            cmptbl[numbits++] = cmp;

                        } while (!bitnum || cmp); /* n+1 bits in cmp? */
                    }
                    cmptbl[numbits - 1] = 0xffffffff; /* force match on most bits */

                    mostbits = numbits - 1;

                    /* decode leapfrog code table */
                    {
                        signed char leap[256];
                        unsigned char nextchar;

                        HUFF_MEMSET_76C0(leap, 0, 256);
                        nextchar = (unsigned char)-1;

                        for (i = 0; i < numchars; ++i) {
                            int leapdelta = 0;

                            HUFF_GETNUM_76C0(leapdelta);
                            ++leapdelta;

                            do {
                                ++nextchar;
                                if (!leap[nextchar])
                                    --leapdelta;
                            } while (leapdelta);

                            leap[nextchar] = !leapdelta; /* FAKEMATCH: stand-in for 1 (leapdelta is 0 here) */
                            codetbl[i] = nextchar;
                        }
                    }
                }

                /* make fast 8 tables */
                HUFF_MEMSET_76C0(quicklentbl, 64, 256);

                {
                    int bits;
                    int bitnum;
                    int numbitentries;
                    int nextcode;
                    int nextlen;
                    int i;
                    unsigned char* codeptr;
                    unsigned char* quickcodeptr;
                    unsigned char* quicklenptr;

                    codeptr = codetbl;
                    quickcodeptr = quickcodetbl;
                    quicklenptr = quicklentbl;

                    for (bits = 1; bits <= mostbits; ++bits) {
                        bitnum = bitnumtbl[bits];
                        if (bits >= 9)
                            break;
                        numbitentries = 1 << (8 - bits);

                        while (bitnum--) {
                            nextcode = *codeptr++;
                            nextlen = bits;
                            if (nextcode == clue) {
                                cluelen = bits;
                                nextlen = 96; /* will force out of main loop */
                            }
                            for (i = 0; i < numbitentries; ++i) {
                                *quickcodeptr++ = (unsigned char)nextcode;
                                *quicklenptr++ = (unsigned char)nextlen;
                            }
                        }
                    }
                }
            }

            /* main decoder */
            for (;;) {
                unsigned char* quickcodeptr = quickcodetbl;
                unsigned char* quicklenptr = quicklentbl;

                goto nextloop;

                /* quick 8 fetch */
                do {
                    *qd++ = quickcodeptr[bits >> 24];
                    HUFF_GET16BITS_76C0();
                    bits = bitsunshifted << (16 - bitsleft);

                    /* quick 8 decode */
                nextloop:
                    numbits = quicklenptr[bits >> 24];
                    bitsleft -= numbits;

                    if (bitsleft >= 0) {
                        do {
                            *qd++ = quickcodeptr[bits >> 24];
                            bits <<= numbits;

                            numbits = quicklenptr[bits >> 24];
                            bitsleft -= numbits;
                            if (bitsleft < 0) break;
                            *qd++ = quickcodeptr[bits >> 24];
                            bits <<= numbits;

                            numbits = quicklenptr[bits >> 24];
                            bitsleft -= numbits;
                            if (bitsleft < 0) break;
                            *qd++ = quickcodeptr[bits >> 24];
                            bits <<= numbits;

                            numbits = quicklenptr[bits >> 24];
                            bitsleft -= numbits;
                            if (bitsleft < 0) break;
                            *qd++ = quickcodeptr[bits >> 24];
                            bits <<= numbits;

                            numbits = quicklenptr[bits >> 24];
                            bitsleft -= numbits;

                        } while (bitsleft >= 0);
                    }
                    bitsleft += 16;

                } while (bitsleft >= 0); /* would fetching 16 bits do it? */

                bitsleft = bitsleft - 16 + numbits; /* back to normal */

                /* 16 bit decoder */
                {
                    unsigned char code;

                    if (numbits != 96) {
                        cmp = (unsigned int)(bits >> 16); /* 16 bit left justified compare */

                        numbits = 8;
                        do {
                            ++numbits;
                        } while (cmp >= cmptbl[numbits]);
                    } else
                        numbits = cluelen;

                    cmp = bits >> (32 - (numbits));
                    bits <<= (numbits);
                    bitsleft -= (numbits);

                    code = codetbl[cmp - deltatbl[numbits]]; /* the code */

                    if (code != clue && bitsleft >= 0) {
                        *qd++ = code;
                        goto nextloop;
                    }

                    if (bitsleft < 0) {
                        HUFF_GET16BITS_76C0();
                        bits = bitsunshifted << -bitsleft;
                        bitsleft += 16;
                    }

                    if (code != clue) {
                        *qd++ = code;
                        goto nextloop;
                    }

                    /* handle clue */
                    {
                        int runlen = 0;
                        unsigned char* runCursor = qd;
                        unsigned char* dest;

                        HUFF_GETNUM_76C0(runlen);
                        if (runlen) { /* runlength sequence */
                            dest = runCursor + runlen;
                            code = *(runCursor - 1);
                            do {
                                *runCursor++ = code;
                            } while (runCursor < dest);

                            qd = runCursor;
                            goto nextloop;
                        }
                    }

                    HUFF_GETBITS_76C0(v, 1); /* end of file */
                    if (v)
                        break;

                    {
                        unsigned int t;
                        HUFF_GETBITS_76C0(t, 8); /* explicit byte */
                        code = (unsigned char)t;
                    }
                    *qd++ = code;
                    goto nextloop;
                }
            }

            /* undelta */
            {
                int i;
                int nextchar;

                if (type == 13051) { /* deltaed? */
                    i = 0;
                    qd = unpackbuf;
                    while (qd < unpackbuf + ulen) {
                        i += (int)*qd;
                        *qd++ = (unsigned char)i;
                    }
                } else if (type == 13563) { /* accelerated? */
                    i = 0;
                    nextchar = 0;
                    qd = unpackbuf;
                    while (qd < unpackbuf + ulen) {
                        i += (int)*qd;
                        nextchar += i;
                        *qd++ = (unsigned char)nextchar;
                    }
                }
            }
        }
    }
    return (ulen);
}
#endif

